package fmj.gamemenu

import fmj.Global
import fmj.ScreenViewType
import fmj.graphics.TextRender
import fmj.graphics.Util
import fmj.views.BaseScreen
import fmj.views.GameNode
import fmj.views.ScreenSaveLoadGame
import fmj.views.ScreenSaveLoadGame.Operate

import graphics.Bitmap
import graphics.Canvas
import graphics.Paint

class ScreenMenuSystem(override val parent: GameNode) : BaseScreen {

    private var first = 0
    private var index = 0
    private val str = arrayOf("读入进度", "存储进度", "游戏设置", "结束游戏")
    private val strX = 42
    private var strY = 32
    private var selY = strY

    private val bmpFrame = Util.getFrameBitmap(109 - 39 + 1, 91 - 29 + 1)
    private val bmpArrowUp = Bitmap.createBitmap(7, 4)
    private val bmpArrowDown = Bitmap.createBitmap(7, 4)
    private val bmpArr = arrayOf(bmpArrowDown, bmpArrowUp)
    private val arrowX = 70
    private var arrowY = 82
    private var bmpi = 0

    override val isPopup: Boolean
        get() = true

    init {
        val p = Paint()
        p.color = Global.COLOR_BLACK

        val c = Canvas(bmpArrowUp)
        c.drawColor(Global.COLOR_WHITE)
        c.drawLine(3, 0, 4, 0, p)
        c.drawLine(2, 1, 5, 1, p)
        c.drawLine(1, 2, 6, 2, p)
        c.drawLine(0, 3, 7, 3, p)

        c.setBitmap(bmpArrowDown)
        c.drawColor(Global.COLOR_WHITE)
        c.drawLine(0, 0, 7, 0, p)
        c.drawLine(1, 1, 6, 1, p)
        c.drawLine(2, 2, 5, 2, p)
        c.drawLine(3, 3, 4, 3, p)
    }

    override fun update(delta: Long) {}

    override fun draw(canvas: Canvas) {
        canvas.drawBitmap(bmpFrame, 39, 29)
        TextRender.drawText(canvas, str[first], strX, strY)
        TextRender.drawText(canvas, str[first + 1], strX, strY + 16)
        TextRender.drawText(canvas, str[first + 2], strX, strY + 32)
        TextRender.drawSelText(canvas, str[index], strX, selY)
        canvas.drawBitmap(bmpArr[bmpi], arrowX, arrowY)
    }

    override fun onKeyDown(key: Int) {
        if (key == Global.KEY_UP) {
            --index
            selY -= 16
        } else if (key == Global.KEY_DOWN) {
            ++index
            selY += 16
        }

        if (index == 0 || index == 4) {
            index = 0
            selY = 32
            arrowY = 82
            bmpi = 0
            first = 0
            strY = 32
        } else if (index == 3 || index == -1) {
            index = 3
            selY = 72
            arrowY = 34
            bmpi = 1
            first = 1
            strY = 40
        }
    }

    override fun onKeyUp(key: Int) {
        if (key == Global.KEY_CANCEL) {
            popScreen()
        } else if (key == Global.KEY_ENTER) {
            when (index) {
                0 -> pushScreen(ScreenSaveLoadGame(this, Operate.LOAD))

                1 -> {
                    if (Global.disableSave)
                        showMessage("当前不能存档")
                    else if (game.mainScene.scriptProcess.prev != null)
                        showMessage("副本中不能存档")
                    else
                        pushScreen(ScreenSaveLoadGame(this, Operate.SAVE))
                }

                2 -> {
                }

                3 -> game.changeScreen(ScreenViewType.SCREEN_MENU)
            }
        }
    }

}
