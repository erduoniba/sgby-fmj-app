package fmj.gamemenu

import fmj.Global
import fmj.graphics.TextRender
import fmj.graphics.Util
import fmj.views.BaseScreen
import fmj.views.GameNode

import graphics.Canvas

class ScreenMenuProperties(override val parent: GameNode) : BaseScreen {

    private val mFrameBmp = Util.getFrameBitmap(77 - 39 + 1, 54 - 16 + 1)
    private val strs = arrayOf("状态", "穿戴")
    private var mSelId = 0

    override val isPopup: Boolean
        get() = true

    override fun update(delta: Long) {}

    override fun draw(canvas: Canvas) {
        canvas.drawBitmap(mFrameBmp, 39, 16)
        if (mSelId == 0) {
            TextRender.drawSelText(canvas, strs[0], 39 + 3, 16 + 3)
            TextRender.drawText(canvas, strs[1], 39 + 3, 16 + 3 + 16)
        } else if (mSelId == 1) {
            TextRender.drawText(canvas, strs[0], 39 + 3, 16 + 3)
            TextRender.drawSelText(canvas, strs[1], 39 + 3, 16 + 3 + 16)
        }
    }

    override fun onKeyDown(key: Int) {
        if (key == Global.KEY_UP || key == Global.KEY_DOWN) {
            mSelId = 1 - mSelId
        }
    }

    override fun onKeyUp(key: Int) {
        if (key == Global.KEY_CANCEL) {
            popScreen()
        } else if (key == Global.KEY_ENTER) {
            popScreen()
            if (mSelId == 0) {
                pushScreen(ScreenActorState(this))
            } else {
                pushScreen(ScreenActorWearing(this))
            }
        }
    }

}
