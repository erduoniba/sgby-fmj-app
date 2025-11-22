package fmj.views

import fmj.Global
import fmj.ScreenViewType
import fmj.config.GameConfig
import fmj.graphics.TextRender
import fmj.graphics.Util
import fmj.lib.DatLib
import fmj.lib.ResImage
import fmj.scene.SaveLoadGame
import fmj.script.ScriptResources

import graphics.Canvas
import java.*

class ScreenSaveLoadGame(override val parent: GameNode, private val mOperate: Operate, private val popDepth: Int = 3) : BaseScreen {
    override val screenName: String = "ScreenSaveLoadGame(${mOperate.name})"

    // 动态存档数量配置
    private val saveSlotCount: Int = GameConfig.saveSlotCount
    private val mTextPos = GameConfig.getSaveSlotPositions()
    private var index = 0
    private val mEmpty = "空档案    "
    private val mText = GameConfig.getEmptySaveTexts()
    private val mHeadImgs = ArrayList<ArrayList<ResImage>>()

    private val mFileNames = GameConfig.saveFileNames
    
    // 滚动相关变量
    private val maxVisibleItems = 5  // 屏幕最多显示5个存档
    private var scrollOffset = 0     // 滚动偏移量（第一个显示的存档索引）

    private val mImgBg: ResImage    // 背景图片

    var callback: (() -> Unit)? = null

    enum class Operate {
        SAVE, // 保存进度
        LOAD    // 读取进度
    }

    init {
        mImgBg = DatLib.getRes(DatLib.ResType.PIC, 2,
                if (mOperate == Operate.LOAD) 16 else 15) as ResImage
        
        // 初始化存档槽的头像列表
        for (i in 0 until saveSlotCount) {
            mHeadImgs.add(ArrayList())
        }
        
        // 加载存档信息
        for (i in 0 until saveSlotCount) {
            val file = File("sav/" + mFileNames[i])
            if (file.exists()) {
                mText[i] = format(getSceneNameAndHeads(file, mHeadImgs[i]))
            }
        }
    }

    private fun format(s: String): String {
        var tmp = s
        while (tmp.gbkBytes().size < mEmpty.gbkBytes().size)
            tmp += " "
        return tmp
    }

    private fun getSceneNameAndHeads(f: File, heads: ArrayList<ResImage>): String {
        val file = objectInputOf(f)
        val name =  file.readString()
        val actorNum =  file.readInt()
        for (i in 0 until actorNum) {
            heads.add(DatLib.getRes(DatLib.ResType.PIC, 1,  file.readInt()) as ResImage)
        }
        file.close()
        return name
    }

    override fun update(delta: Long) {}
    
    /**
     * 更新滚动偏移量，确保当前选中的存档可见
     */
    private fun updateScrollOffset() {
        if (saveSlotCount <= maxVisibleItems) {
            // 如果存档数量不超过最大可见数量，不需要滚动
            scrollOffset = 0
            return
        }
        
        // 确保当前选中项在可见范围内
        if (index < scrollOffset) {
            // 当前项在可见区域上方，向上滚动
            scrollOffset = index
        } else if (index >= scrollOffset + maxVisibleItems) {
            // 当前项在可见区域下方，向下滚动
            scrollOffset = index - maxVisibleItems + 1
        }
        
        // 确保滚动偏移量在有效范围内
        scrollOffset = maxOf(0, minOf(scrollOffset, saveSlotCount - maxVisibleItems))
    }

    override fun draw(canvas: Canvas) {
        // 320x192大屏适配：重新设计存档界面布局
        
        // 1. 绘制标题背景和标题文本
        val titleHeight = 25
        val titleBg = Util.getFrameBitmap(Global.SCREEN_WIDTH - 40, titleHeight)
        canvas.drawBitmap(titleBg, 20, 10)
        val titleText = if (mOperate == Operate.LOAD) "读取进度" else "保存进度"
        TextRender.drawText(canvas, titleText, 25, 18)
        
        // 2. 计算存档列表区域
        val listStartY = 40
        val itemHeight = 25
        val itemSpacing = 0
        
        // 3. 更新滚动偏移量
        updateScrollOffset()
        
        // 4. 绘制可见的存档槽
        val endIndex = minOf(scrollOffset + maxVisibleItems, saveSlotCount)
        for (i in scrollOffset until endIndex) {
            val displayIndex = i - scrollOffset  // 在屏幕上的显示位置
            val itemY = listStartY + displayIndex * (itemHeight + itemSpacing)
            
            // 绘制存档槽背景框
            val itemBg = Util.getFrameBitmap(Global.SCREEN_WIDTH - 40, itemHeight)
            canvas.drawBitmap(itemBg, 20, itemY)
            
            // 绘制选中高亮
            if (i == index) {
                val selectBg = Util.getFrameBitmap(Global.SCREEN_WIDTH - 44, itemHeight - 4)
                canvas.drawBitmap(selectBg, 22, itemY + 2)
            }
            
            // 绘制存档序号
            TextRender.drawText(canvas, "${i + 1}.", 25, itemY + 8)
            
            // 绘制角色头像
            for (j in 0 until mHeadImgs[i].size) {
                val img = mHeadImgs[i][j]
                img.draw(canvas, 7, 50 + 20 * j, itemY + 4)
            }
            
            // 绘制存档信息文本
            val textX = 50 + mHeadImgs[i].size * 20 + 10
            if (i == index) {
                TextRender.drawSelText(canvas, mText[i], textX, itemY + 8)
            } else {
                TextRender.drawText(canvas, mText[i], textX, itemY + 8)
            }
        }
    }

    override fun onKeyDown(key: Int) {
        if (key == Global.KEY_UP) {
            if (--index < 0) {
                index = saveSlotCount - 1
            }
        } else if (key == Global.KEY_DOWN) {
            if (++index >= saveSlotCount) {
                index = 0
            }
        }
    }

    private fun exit() {
        (0 until popDepth).forEach {
            popScreen()
        }
        callback?.invoke()
    }

    override fun onKeyUp(key: Int) {
        if (key == Global.KEY_CANCEL) {
            popScreen()
            callback?.invoke()
        } else if (key == Global.KEY_ENTER) {
            val file = File("sav/" + mFileNames[index])
            if (mOperate == Operate.LOAD) { // 加载存档
                if (!file.exists()) {
                    return
                }
                if (!loadGame(file)) {
                    return
                }
                SaveLoadGame.startNewGame = false
                game.changeScreen(ScreenViewType.SCREEN_MAIN_GAME)
            } else { // 保存存档
                if (!file.exists()) {
                    file.createNewFile()
                    saveGame(file)
                    exit()
                } else { // 询问是否覆盖存档
                    pushScreen(ScreenMessageBox(this, "覆盖原进度?",
                            object : ScreenMessageBox.OnOKClickListener {
                                override fun onOKClick() {
                                    saveGame(file)
                                    exit()
                                }
                            }))
                }
            }
        }
    }

    private fun loadGame(file: File): Boolean {
        val ioIn = objectInputOf(file)
        if (!SaveLoadGame.read(game, ioIn))
            return false
        ScriptResources.read(ioIn)
        ioIn.close()
        return true
    }

    fun saveGame(file: File) {
        val o = objectOutputOf(file)
        SaveLoadGame.write(game, o)
        ScriptResources.write(o)
        o.close()
    }

}
