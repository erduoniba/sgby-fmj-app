package fmj.gamemenu

import fmj.Global
import fmj.graphics.TextRender
import fmj.graphics.Util
import fmj.views.BaseScreen
import fmj.views.GameNode
import graphics.Bitmap
import graphics.Canvas
import java.gbkBytes


class ScreenCommonMenu(override val parent: GameNode, items: Array<String>, private val callback: (Int) -> Unit): BaseScreen {
    // TODO: 滚动
    private var curSel = 0
    private val paddedItems: List<ByteArray>
    private val top: Int
    private val left: Int
    private val bg: Bitmap
    private val padx = 3
    private val pady = 3
    init {
        val byteItems = items.map { it.gbkBytes() }
        val colCount = byteItems.map { it.size }.maxOrNull() ?: 2
        val width = 8 * colCount
        val height = 16 * items.size
        bg = Util.getFrameBitmap(width+padx*2, height+pady*2)
        left = (Global.SCREEN_WIDTH - width) / 2
        top = (Global.SCREEN_HEIGHT - height) / 2

        paddedItems = byteItems.map {
            val s = it.toMutableList()
            while (s.size < colCount) s.add(' '.toByte())
            s.toByteArray()
        }
    }

    override val isPopup: Boolean
        get() = true

    override fun update(delta: Long) {}

    override fun onKeyUp(key: Int) {
        if (key == Global.KEY_ENTER) {
            popScreen()
            callback(curSel+1)
        } else if (key == Global.KEY_CANCEL) {
            popScreen()
            callback(0)
        }
    }

    override fun onKeyDown(key: Int) {
        if (key == Global.KEY_DOWN && curSel < paddedItems.size - 1) {
            ++curSel
        } else if (key == Global.KEY_UP && curSel > 0) {
            --curSel
        }
    }

    override fun draw(canvas: Canvas) {
        canvas.drawBitmap(bg, left-padx, top-pady)
        for (i in paddedItems.indices) {
            if (i != curSel) {
                TextRender.drawText(canvas, paddedItems[i], left, top + 16 * i)
            } else {
                TextRender.drawSelText(canvas, paddedItems[i], left, top + 16 * i)
            }
        }
    }

    fun reset() {
        curSel = 0
    }
}
