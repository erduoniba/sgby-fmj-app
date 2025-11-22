package fmj.views

import fmj.Global
import fmj.ScreenViewType
import fmj.lib.DatLib
import fmj.lib.ResSrs
import fmj.scene.SaveLoadGame
import graphics.Canvas
import java.sysExit
import java.sysGetChoiceLibName

class ScreenMenu(override val parent: GameNode): BaseScreen {
    override val screenName: String = "ScreenMenu"
    private val mImgMenu = DatLib.getPic(2, 14)!!
    private val mLeft: Int
    private val mTop: Int
    private val mSrsSelector = (250..255).mapNotNull {
        DatLib.getRes(DatLib.ResType.SRS, 1, it, allowNull = true) as ResSrs?
    }

    private var mCurSelect = 0

    private var isCancelKeyDown = false

    init {
        mSrsSelector.forEach { it.start() }
        mLeft = (Global.SCREEN_WIDTH - mImgMenu.width) / 2
        mTop = (Global.SCREEN_HEIGHT - mImgMenu.height) / 2
    }

    override fun update(delta: Long) {
        if (!mSrsSelector[mCurSelect].update(delta)) {
            mSrsSelector[mCurSelect].start()
        }
    }

    override fun draw(canvas: Canvas) {
        canvas.drawColor(Global.COLOR_WHITE)
        mImgMenu.draw(canvas, 1, mLeft, mTop)
        val currentGame = sysGetChoiceLibName().uppercase()
        if (currentGame == "FMJ" || currentGame == "FMJWMB" || 
        currentGame == "JYQXZ" || currentGame == "FMJLL" ||
        currentGame == "FMJFYJ") {
            mSrsSelector[mCurSelect].draw(canvas, mLeft - 22, mTop/2 + 12)
            return    
        }

        mSrsSelector[mCurSelect].draw(canvas, mLeft - 0, mTop/2 + 24)
    }

    override fun onKeyDown(key: Int) {
        when (key) {
            Global.KEY_UP, Global.KEY_DOWN -> {
                mCurSelect += 1
                mCurSelect %= mSrsSelector.size
            }
            Global.KEY_CANCEL -> isCancelKeyDown = true
        }
    }

    override fun onKeyUp(key: Int) {
        if (key == Global.KEY_ENTER) {
            val index = mSrsSelector[mCurSelect].index
            when (index) {
                250 -> { // 新游戏
                    SaveLoadGame.startNewGame = true
                    game.changeScreen(ScreenViewType.SCREEN_MAIN_GAME)
                }
                251 -> // 读取进度
                    pushScreen(
                            ScreenSaveLoadGame(this, ScreenSaveLoadGame.Operate.LOAD))
                else -> {
                    // TODO
                }
            }
        } else if (key == Global.KEY_CANCEL && isCancelKeyDown) {
            sysExit()
        } else if (key == Global.KEY_PAGEUP) {
            DatLib.instance.tryCompileScripts(game.vm)
        }
    }

}
