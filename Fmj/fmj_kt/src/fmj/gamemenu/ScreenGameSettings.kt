package fmj.gamemenu

import fmj.Global
import fmj.combat.Combat
import fmj.config.GameSettings
import fmj.graphics.TextRender
import fmj.graphics.Util
import fmj.script.ScriptResources
import fmj.views.BaseScreen
import fmj.views.GameNode
import graphics.Canvas
import graphics.Rect
import java.sysGetChoiceLibName

/** 游戏设置菜单屏幕 */
class ScreenGameSettings(override val parent: GameNode) : BaseScreen {
    private val frameWidth = 240
    private val frameHeight = 160
    private val bmpFrame = Util.getFrameBitmap(frameWidth, frameHeight)
    // 居中显示在320x192的屏幕上
    private val frameX = (320 - frameWidth) / 2
    private val frameY = (192 - frameHeight) / 2
    private val frameRect = Rect(frameX, frameY, frameX + frameWidth, frameY + frameHeight)

    // 动态菜单项列表
    private val baseMenuItems =
            mutableListOf(
                "Miss 功能", 
                "地图信息",
                "原版伤害", 
                "属性增强", 
                "查找键（功能键1）", 
                "插入键（功能键2）", 
                "修改键（功能键3）", 
                "删除键（功能键4）", 
                "对话历史")
    private val hiddenMenuItems =
            mutableListOf(
                    "Miss 功能",
                    "地图信息",
                    "穿墙模式",
                    "原版伤害",
                    "属性增强",
                    "查找键（功能键1）",
                    "插入键（功能键2）",
                    "修改键（功能键3）",
                    "删除键（功能键4）",
                    "对话历史"
            )

    private fun buildMenuItems(): Array<String> {
        val items = if (GameSettings.isWallWalkingUnlocked) hiddenMenuItems else baseMenuItems

        // 如果队伍人数超过3人，添加角色管理器选项
        if (game.playerList.size > 3) {
            if (!items.contains("角色管理器")) {
                val insertIndex = items.indexOf("对话历史") + 1
                if (insertIndex > 0) {
                    items.add(insertIndex, "角色管理器")
                }
            }
        }

        // 始终在末尾添加这些选项
        if (!items.contains("重置设置")) {
            items.add("重置设置")
        }
        if (!items.contains("返回")) {
            items.add("返回")
        }

        return items.toTypedArray()
    }

    private val menuItems: Array<String>
        get() = buildMenuItems()

    private var selectedIndex = 0
    private var scrollOffset = 0 // 滚动偏移量

    // 可见菜单项数量（根据框架高度计算）
    // frameHeight = 160, 标题占用 28px, 每个菜单项 16px
    // 可用高度 = 160 - 28 - 8(底部边距) = 124px
    // 可见项数 = 124 / 16 = 7 项（向下取整保留完整显示）
    private val visibleItemCount = 7

    override val isPopup: Boolean
        get() = true

    override fun update(delta: Long) {}

    override fun draw(canvas: Canvas) {
        // 绘制框架
        canvas.drawBitmap(bmpFrame, frameRect.left, frameRect.top)

        // 绘制标题
        TextRender.drawText(canvas, "游戏设置", frameRect.left + 8, frameRect.top + 8)

        // 计算可见菜单项的范围
        val startIndex = scrollOffset
        val endIndex = minOf(scrollOffset + visibleItemCount, menuItems.size)

        // 绘制可见的菜单项
        for (i in startIndex until endIndex) {
            val displayIndex = i - scrollOffset // 显示位置索引（0-6）
            val y = frameRect.top + 28 + displayIndex * 16

            if (i == selectedIndex) {
                TextRender.drawSelText(canvas, menuItems[i], frameRect.left + 8, y)
            } else {
                TextRender.drawText(canvas, menuItems[i], frameRect.left + 8, y)
            }

            // 显示功能状态 - 增加间距，右对齐显示
            when (menuItems[i]) {
                "Miss 功能" -> {
                    val statusText = if (GameSettings.allowMiss) "[开启]" else "[关闭]"
                    TextRender.drawText(canvas, statusText, frameRect.right - 60, y)
                }
                "地图信息" -> {
                    val statusText = if (GameSettings.showCoordinates) "[开启]" else "[关闭]"
                    TextRender.drawText(canvas, statusText, frameRect.right - 60, y)
                }
                "穿墙模式" -> {
                    val statusText = if (GameSettings.allowWallWalking) "[开启]" else "[关闭]"
                    TextRender.drawText(canvas, statusText, frameRect.right - 60, y)
                }
                "原版伤害" -> {
                    val statusText = if (GameSettings.useOriginalDamageFormula) "[开启]" else "[关闭]"
                    TextRender.drawText(canvas, statusText, frameRect.right - 60, y)
                }
                "属性增强" -> {
                    val statusText = if (GameSettings.enableEnhancedLimits) "[开启]" else "[关闭]"
                    TextRender.drawText(canvas, statusText, frameRect.right - 60, y)
                }
            }
        }

        // 绘制滚动指示器
        if (menuItems.size > visibleItemCount) {
            val indicatorX = frameRect.right - 12
            val indicatorStartY = frameRect.top + 28
            val indicatorHeight = visibleItemCount * 16

            // 显示上箭头（如果不在顶部）
            if (scrollOffset > 0) {
                TextRender.drawText(canvas, "↑", indicatorX, indicatorStartY - 4)
            }

            // 显示下箭头（如果不在底部）
            if (scrollOffset + visibleItemCount < menuItems.size) {
                TextRender.drawText(canvas, "↓", indicatorX, indicatorStartY + indicatorHeight + 4)
            }
        }
    }

    override fun onKeyDown(key: Int) {
        when (key) {
            Global.KEY_UP -> {
                selectedIndex--
                if (selectedIndex < 0) selectedIndex = menuItems.size - 1

                // 自动滚动：向上移动时，如果选中项在可视区域之上，则向上滚动
                if (selectedIndex < scrollOffset) {
                    scrollOffset = selectedIndex
                } else if (selectedIndex == menuItems.size - 1) {
                    // 循环到底部时，滚动到底部
                    scrollOffset = maxOf(0, menuItems.size - visibleItemCount)
                }
            }
            Global.KEY_DOWN -> {
                selectedIndex++
                if (selectedIndex >= menuItems.size) selectedIndex = 0

                // 自动滚动：向下移动时，如果选中项在可视区域之下，则向下滚动
                if (selectedIndex >= scrollOffset + visibleItemCount) {
                    scrollOffset = selectedIndex - visibleItemCount + 1
                } else if (selectedIndex == 0) {
                    // 循环到顶部时，滚动到顶部
                    scrollOffset = 0
                }
            }
        }
    }

    override fun popScreen() {
        // 在关闭页面前自动保存所有设置
        try {
            GameSettings.saveAllSettings()
            println("GameSettings: 设置页面关闭，已自动保存所有设置")
        } catch (e: Exception) {
            println("GameSettings: 自动保存设置失败: ${e.message}")
        }
        // 调用父类方法关闭页面
        super.popScreen()
    }

    override fun onKeyUp(key: Int) {
        when (key) {
            Global.KEY_CANCEL -> {
                popScreen()
            }
            Global.KEY_ENTER -> {
                when (menuItems[selectedIndex]) {
                    "Miss 功能" -> {
                        GameSettings.allowMiss = !GameSettings.allowMiss
                        GameSettings.missToggleCount++

                        val statusText = if (GameSettings.allowMiss) "开启" else "关闭"

                        // 检查是否达到解锁条件
                        if (GameSettings.missToggleCount == 10) {
                            showMessage("Miss 功能已$statusText，穿墙模式已解锁！", 3000)
                        } else {
                            showMessage("Miss 功能已$statusText", 1000)
                        }
                    }
                    "地图信息" -> {
                        GameSettings.showCoordinates = !GameSettings.showCoordinates
                        val statusText = if (GameSettings.showCoordinates) "开启" else "关闭"
                        showMessage("地图信息已$statusText", 1000)
                    }
                    "穿墙模式" -> {
                        GameSettings.allowWallWalking = !GameSettings.allowWallWalking
                        if (GameSettings.allowWallWalking) {
                            showMessage("穿墙模式已开启 - 兼容某些游戏在创建主角在死角位置，答应作者，请勿乱用！", 5000)
                        } else {
                            showMessage("穿墙模式已关闭", 1000)
                        }
                    }
                    "原版伤害" -> {
                        GameSettings.useOriginalDamageFormula =
                                !GameSettings.useOriginalDamageFormula
                        val statusText = if (GameSettings.useOriginalDamageFormula) "开启" else "关闭"
                        showMessage("原版伤害公式已$statusText", 1000)
                    }
                    "属性增强" -> {
                        GameSettings.enableEnhancedLimits = !GameSettings.enableEnhancedLimits
                        val statusText = if (GameSettings.enableEnhancedLimits) "开启" else "关闭"
                        val limitsText =
                                if (GameSettings.enableEnhancedLimits)
                                        "HP/MP/攻击/防御提升至9999，速度/灵力/幸运提升至127"
                                else "恢复原版属性上限"
                        showMessage("属性增强已$statusText - $limitsText", 3000)
                    }
                    "查找键（功能键1）" -> {
                        // 直接执行查找指令（触发剧情 0-6）
                        println("🔧 执行查找指令 - 触发剧情 0-6")
                        popScreen() // 先关闭设置菜单
                        game.mainScene.callChapter(0, 6)
                        showMessage("已执行查找指令", 2000)
                    }
                    "插入键（功能键2）" -> {
                        // 直接执行插入指令（触发剧情 0-7）
                        println("🔧 执行插入指令 - 触发剧情 0-7")
                        popScreen() // 先关闭设置菜单
                        game.mainScene.callChapter(0, 7)
                        showMessage("已执行插入指令", 2000)
                    }
                    "修改键（功能键3）" -> {
                        // 直接执行修改指令（触发剧情 0-8）
                        println("🔧 执行修改指令 - 触发剧情 0-8")
                        popScreen() // 先关闭设置菜单
                        game.mainScene.callChapter(0, 8)
                        showMessage("已执行修改指令", 2000)
                    }
                    "删除键（功能键4）" -> {
                        // 直接执行删除指令（触发剧情 0-9）
                        println("🔧 执行删除指令 - 触发剧情 0-9")
                        popScreen() // 先关闭设置菜单
                        game.mainScene.callChapter(0, 9)
                        showMessage("已执行删除指令", 2000)
                    }
                    "对话历史" -> {
                        // 显示对话历史界面
                        pushScreen(ScreenDialogueHistory(this))
                    }
                    "角色管理器" -> {
                        // 显示角色管理器界面
                        pushScreen(ScreenPlayerManager(this))
                    }
                    "重置设置" -> {
                        GameSettings.resetToDefaults()
                        showMessage("所有设置已重置为默认值并清除本地存储", 2000)
                    }
                    "返回" -> {
                        popScreen()
                    }
                }
            }
        }
    }
}
