package fmj.gamemenu

import fmj.Global
import fmj.characters.Player
import fmj.goods.BaseGoods
import fmj.graphics.TextRender
import fmj.graphics.Util
import fmj.views.BaseScreen
import fmj.views.GameNode

import graphics.Bitmap
import graphics.Canvas
import graphics.Rect

import java.Stack
import java.gbkBytes

class ScreenGoodsList(
        override val parent: GameNode,
        private val goodsList: List<BaseGoods>,
        private val itemSelectedListener: OnItemSelectedListener,
        private val mode: Mode,
        initialCursorIndex: Int = 0): BaseScreen {

    private var description = "".gbkBytes()

    private var toDraw = 0 // 当前要画的描述中的字节
    private var nextToDraw = 0 // 下一个要画的描述中的字节
    private val stackLastToDraw = Stack.create<Int>() // 保存上次描述所画位置

    private var firstDisplayItemIndex = 0 // 界面上显示的第一个物品的序号

    private var curItemIndex = initialCursorIndex // 当前光标所在位置物品的序号，使用传入的初始位置
    private var lastDownKey = -1

    enum class Mode {
        Sale,
        Buy,
        Use
    }

    interface OnItemSelectedListener {
        fun onItemSelected(goods: BaseGoods, index: Int = 0)
    }

    private fun resetDescription() {
        description = if (goodsList.isNotEmpty()) {
            "说明:${goodsList[curItemIndex].description}".gbkBytes()
        } else {
            "".gbkBytes()
        }
        nextToDraw = 0
        toDraw = nextToDraw
        stackLastToDraw.clear()
    }

    override fun willAppear() {
        if (goodsList.isEmpty()) {
            curItemIndex = 0
        } else if (curItemIndex >= goodsList.size) {
            curItemIndex = goodsList.size - 1
        }
        
        // 计算合适的 firstDisplayItemIndex，确保当前项可见
        if (curItemIndex >= itemNumberPerPage) {
            firstDisplayItemIndex = curItemIndex - itemNumberPerPage + 1
            if (firstDisplayItemIndex < 0) {
                firstDisplayItemIndex = 0
            }
        }
        
        resetDescription()
    }

    override fun update(delta: Long) {
        if (goodsList.isEmpty()) {
            popScreen()
        }
    }

    override fun draw(canvas: Canvas) {
        // 320x192大屏适配，充分利用屏幕空间
        canvas.drawBitmap(bgImage, 10, 10)
        if (goodsList.isEmpty()) return

        while (curItemIndex >= goodsList.size) showPreItem()

        val g = goodsList[curItemIndex]

        // 右侧显示物品详细信息，确保在线框内
        TextRender.drawText(canvas, if (mode == Mode.Buy) "金钱:" + Player.sMoney else "数量:" + g.goodsNum, 85, 20)
        TextRender.drawText(canvas, "名称:${g.name}", 85, 38)
        TextRender.drawText(canvas, "价格:" + if (mode == Mode.Buy) g.buyPrice else g.sellPrice, 85, 55)
        
        // 光标位置调整，在左侧物品列表区域内
        Util.drawTriangleCursor(canvas, 20, 25 + 30 * (curItemIndex - firstDisplayItemIndex))

        // 物品列表显示在左侧框内，调整间距
        var i = firstDisplayItemIndex
        while (i < firstDisplayItemIndex + itemNumberPerPage && i < goodsList.size) {
            goodsList[i].draw(canvas, 40, 25 + 30 * (i - firstDisplayItemIndex))
            i++
        }

        // 描述区域在右下方框内
        nextToDraw = TextRender.drawText(canvas, description, toDraw, displayRect)
    }

    private fun showNextItem() {
        ++curItemIndex
        if (curItemIndex >= firstDisplayItemIndex + itemNumberPerPage) {
            ++firstDisplayItemIndex
        }
        resetDescription()
    }

    private fun showPreItem() {
        --curItemIndex
        if (curItemIndex < firstDisplayItemIndex) {
            --firstDisplayItemIndex
        }
        resetDescription()
    }

    override fun onKeyDown(key: Int) {
        if (key == Global.KEY_UP && curItemIndex > 0) {
            showPreItem()
        } else if (key == Global.KEY_DOWN && curItemIndex + 1 < goodsList.size) {
            showNextItem()
        } else if (key == Global.KEY_PAGEDOWN) {
            val len = description.size
            if (nextToDraw < len) {
                stackLastToDraw.push(toDraw)
                toDraw = nextToDraw
            }
        } else if (key == Global.KEY_PAGEUP && toDraw != 0) {
            stackLastToDraw.pop()?.let {
                toDraw = it
            }
        }
        lastDownKey = key
    }

    override fun onKeyUp(key: Int) {
        if (key == Global.KEY_ENTER && lastDownKey == Global.KEY_ENTER) {
            itemSelectedListener.onItemSelected(goodsList[curItemIndex], curItemIndex)
        } else if (key == Global.KEY_CANCEL) {
            popScreen()
        }
    }

    companion object {

        private val bgImage by lazy {
            val bmp = Bitmap.createBitmap(300, 172)
            val c = Canvas(bmp)
            c.drawColor(Global.COLOR_WHITE)
            
            // 绘制外边框
            c.drawRect(0, 0, 299, 172, Util.sBlackPaint)
            
            // 左侧物品列表区域框线
            c.drawRect(5, 5, 65, 165, Util.sBlackPaint)
            
            // 右侧信息区域框线  
            c.drawRect(70, 5, 294, 65, Util.sBlackPaint)
            
            // 下方描述区域框线 - 扩大高度
            c.drawRect(70, 70, 294, 165, Util.sBlackPaint)
            
            bmp
        }

        private val displayRect = Rect(85, 85, 295, 175)

        private val itemNumberPerPage = 5 // 界面上显示的条目数
    }
}
