package fmj.gamemenu

import fmj.Global
import fmj.characters.Player
import fmj.graphics.TextRender
import fmj.graphics.Util
import fmj.views.BaseScreen
import fmj.views.GameNode
import graphics.Canvas
import graphics.Rect

/**
 * 角色管理器界面
 * 允许用户管理队伍中的角色，选择哪些角色上线（参战）或下线（待机）
 * 战斗时只有前3个上线的角色会参战
 */
class ScreenPlayerManager(override val parent: GameNode) : BaseScreen {
    
    companion object {
        const val MAX_ACTIVE_PLAYERS = 3  // 最多3个角色可以上线
    }
    
    override val isPopup: Boolean
        get() = true
    
    override val screenName: String
        get() = "ScreenPlayerManager"
    
    private val frameWidth = 280
    private val frameHeight = 160
    private val bmpFrame = Util.getFrameBitmap(frameWidth, frameHeight)
    // 居中显示在320x192的屏幕上
    private val frameX = (320 - frameWidth) / 2
    private val frameY = (192 - frameHeight) / 2
    private val frameRect = Rect(frameX, frameY, frameX + frameWidth, frameY + frameHeight)
    
    private var allPlayers: List<Player> = listOf()
    private val activePlayers = mutableListOf<Player>()  // 上线的角色（最多3个）
    private val inactivePlayers = mutableListOf<Player>()  // 下线的角色
    
    private var cursorPosition = 0
    
    init {
        refreshPlayerList()
    }
    
    /**
     * 刷新角色列表
     */
    private fun refreshPlayerList() {
        allPlayers = game.playerList
        
        // 根据当前队伍顺序分配角色状态
        activePlayers.clear()
        inactivePlayers.clear()
        
        for ((index, player) in allPlayers.withIndex()) {
            if (index < MAX_ACTIVE_PLAYERS) {
                activePlayers.add(player)
            } else {
                inactivePlayers.add(player)
            }
        }
    }
    
    override fun update(delta: Long) {
        // 不需要更新逻辑
    }
    
    override fun draw(canvas: Canvas) {
        // 绘制背景框架
        canvas.drawBitmap(bmpFrame, frameRect.left, frameRect.top)
        
        // 绘制标题
        TextRender.drawText(canvas, "角色管理器", frameRect.left + 100, frameRect.top + 8)
        
        // 绘制说明
        TextRender.drawText(canvas, "上线角色(参战):", frameRect.left + 8, frameRect.top + 25)
        TextRender.drawText(canvas, "下线角色(待机):", frameRect.left + 145, frameRect.top + 25)
        
        var y = frameRect.top + 40
        val leftColumnX = frameRect.left + 8
        val rightColumnX = frameRect.left + 145
        
        // 绘制上线角色（左列）
        for ((index, player) in activePlayers.withIndex()) {
            val isSelected = cursorPosition == index
            val text = "${index + 1}. ${player.name}"
            
            if (isSelected) {
                TextRender.drawText(canvas, ">", leftColumnX - 8, y + index * 15)
                TextRender.drawSelText(canvas, text, leftColumnX, y + index * 15)
            } else {
                TextRender.drawText(canvas, text, leftColumnX, y + index * 15)
            }
        }
        
        // 绘制下线角色（右列）
        for ((index, player) in inactivePlayers.withIndex()) {
            val globalIndex = activePlayers.size + index
            val isSelected = cursorPosition == globalIndex
            val text = player.name
            
            if (isSelected) {
                TextRender.drawText(canvas, ">", rightColumnX - 8, y + index * 15)
                TextRender.drawSelText(canvas, text, rightColumnX, y + index * 15)
            } else {
                TextRender.drawText(canvas, text, rightColumnX, y + index * 15)
            }
        }
    }
    
    override fun onKeyDown(key: Int) {
        when (key) {
            Global.KEY_UP -> {
                if (cursorPosition > 0) {
                    cursorPosition--
                }
            }
            Global.KEY_DOWN -> {
                val totalPlayers = activePlayers.size + inactivePlayers.size
                if (cursorPosition < totalPlayers - 1) {
                    cursorPosition++
                }
            }
            Global.KEY_LEFT -> {
                // 如果在右列，跳到左列
                if (cursorPosition >= activePlayers.size && activePlayers.isNotEmpty()) {
                    cursorPosition = minOf(cursorPosition - activePlayers.size, activePlayers.size - 1)
                }
            }
            Global.KEY_RIGHT -> {
                // 如果在左列，跳到右列
                if (cursorPosition < activePlayers.size && inactivePlayers.isNotEmpty()) {
                    cursorPosition = activePlayers.size
                }
            }
        }
    }
    
    override fun onKeyUp(key: Int) {
        when (key) {
            Global.KEY_CANCEL -> {
                // 取消按钮 - 保存更改并关闭页面
                saveChanges()
                parent.popScreen()
            }
            Global.KEY_ENTER -> {
                // 点击确认键切换角色状态
                togglePlayerStatus()
            }
            Global.KEY_PAGEDOWN -> {
                // PageDown键也可以保存并退出
                saveChanges()
                parent.popScreen()
            }
        }
    }
    
    /**
     * 切换选中角色的上线/下线状态
     */
    private fun togglePlayerStatus() {
        if (cursorPosition < activePlayers.size) {
            // 选中的是上线角色，将其下线
            val player = activePlayers[cursorPosition]
            
            // 检查是否是第一个角色（第一个角色不能下线）
            if (cursorPosition == 0) {
                showMessage("第一个角色必须保持上线！", 1500)
                return
            }
            
            // 将角色下线
            activePlayers.removeAt(cursorPosition)
            inactivePlayers.add(player)
            
            // 调整光标位置
            if (cursorPosition >= activePlayers.size && activePlayers.isNotEmpty()) {
                cursorPosition = activePlayers.size - 1
            }
            
            showMessage("${player.name} 已下线", 1000)
        } else {
            // 选中的是下线角色，尝试将其上线
            val inactiveIndex = cursorPosition - activePlayers.size
            if (inactiveIndex < inactivePlayers.size) {
                val player = inactivePlayers[inactiveIndex]
                
                // 检查是否已达上线上限
                if (activePlayers.size < MAX_ACTIVE_PLAYERS) {
                    inactivePlayers.removeAt(inactiveIndex)
                    activePlayers.add(player)
                    
                    // 光标跟随角色移动到上线列表
                    cursorPosition = activePlayers.size - 1
                    
                    showMessage("${player.name} 已上线", 1000)
                } else {
                    showMessage("最多只能有${MAX_ACTIVE_PLAYERS}个角色同时上线！", 1500)
                }
            }
        }
    }
    
    /**
     * 保存更改，重新排列游戏的玩家列表
     */
    private fun saveChanges() {
        // 重新排列玩家列表：上线的在前，下线的在后
        game.playerList.clear()
        game.playerList.addAll(activePlayers)
        game.playerList.addAll(inactivePlayers)
        
        // 不在退出时显示消息，避免屏幕栈问题
        // showMessage("角色状态已更新", 1000)
        
        // 打印调试信息
        println("[角色管理器] 队伍更新完成")
        println("[角色管理器] 上线角色: ${activePlayers.map { it.name }.joinToString(", ")}")
        println("[角色管理器] 下线角色: ${inactivePlayers.map { it.name }.joinToString(", ")}")
    }
}