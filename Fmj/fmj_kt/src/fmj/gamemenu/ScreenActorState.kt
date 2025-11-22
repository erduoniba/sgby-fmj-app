package fmj.gamemenu

import graphics.Canvas
import fmj.Global
import fmj.graphics.Util
import fmj.scene.ScreenMainGame
import fmj.views.BaseScreen
import fmj.views.GameNode

class ScreenActorState(override val parent: GameNode) : BaseScreen {

    private var mPage = 0

    private val mPlayerList = game.playerList

    private var mCurPlayer = 0

    override fun update(delta: Long) {}

    override fun draw(canvas: Canvas) {
        canvas.drawColor(Global.COLOR_WHITE)

        var i = 0
        while (i < mPlayerList.size) {
            mPlayerList.get(i).drawHead(canvas, 10, 2 + 32 * i)
            ++i
        }

        if (mPlayerList.isNotEmpty()) {
            mPlayerList.get(mCurPlayer).drawState(canvas, mPage)
            Util.drawTriangleCursor(canvas, 3, 10 + 32 * mCurPlayer)
        }
    }

    override fun onKeyDown(key: Int) {
        if (key == Global.KEY_PAGEDOWN || key == Global.KEY_PAGEUP) {
            mPage = 1 - mPage
        } else if (key == Global.KEY_DOWN) {
            ++mCurPlayer
            if (mCurPlayer >= mPlayerList.size) {
                mCurPlayer = 0
            }
        } else if (key == Global.KEY_UP) {
            --mCurPlayer
            if (mCurPlayer < 0) {
                mCurPlayer = mPlayerList.size - 1
            }
        }
    }

    override fun onKeyUp(key: Int) {
        if (key == Global.KEY_CANCEL) {
            popScreen()
        }
    }
}
