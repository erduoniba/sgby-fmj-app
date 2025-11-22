package fmj.views

import fmj.Global
import fmj.graphics.TextRender
import graphics.Bitmap
import graphics.Paint
import graphics.Canvas
import graphics.Paint.Style

class ScreenMessageBox(
        override val parent: GameNode, msg: String?,
        private val mOnOkClickListener: OnOKClickListener?): BaseScreen {

    private var index = 0

    private var mMsg = ""

    override val isPopup: Boolean
        get() = true

    interface OnOKClickListener {
        fun onOKClick()
    }

    init {
        if (msg != null) {
            mMsg = msg
        }
    }

    override fun update(delta: Long) {}

    override fun draw(canvas: Canvas) {
        canvas.drawBitmap(bmpBg, 27, 15)
        TextRender.drawText(canvas, mMsg, 33, 23)
        if (index == 0) {
            TextRender.drawSelText(canvas, "是 ", 45, 53)
            TextRender.drawText(canvas, "否 ", 93, 53)
        } else if (index == 1) {
            TextRender.drawText(canvas, "是 ", 45, 53)
            TextRender.drawSelText(canvas, "否 ", 93, 53)
        }
    }

    override fun onKeyDown(key: Int) {
        if (key == Global.KEY_LEFT || key == Global.KEY_RIGHT) {
            index = 1 - index
        }
    }

    override fun onKeyUp(key: Int) {
        if (key == Global.KEY_ENTER) {
            if (index == 0 && mOnOkClickListener != null) {
                mOnOkClickListener.onOKClick()
            }
            exit()
        } else if (key == Global.KEY_CANCEL) {
            exit()
        }
    }

    private fun exit() {
        popScreen()
    }

    companion object {

        private var bmpBg = Bitmap.createBitmap(137 - 27 + 1, 81 - 15 + 1)

        init {
            val c = Canvas(bmpBg)
            c.drawColor(Global.COLOR_WHITE)
            val p = Paint()
            p.color = Global.COLOR_BLACK
            p.style = Style.STROKE
            c.drawRect(1, 1, bmpBg.width - 5, bmpBg.height - 5, p)
            c.drawRect(43 - 27, 51 - 15, 70 - 27, 70 - 15, p)
            c.drawRect(91 - 27, 51 - 15, 118 - 27, 70 - 15, p)
            p.style = Style.FILL_AND_STROKE
            c.drawRect(32 - 27, 77 - 15, 137 - 27, 81 - 15, p)
            c.drawRect(133 - 27, 20 - 15, bmpBg.width - 1, bmpBg.height - 1, p)
        }
    }
}
