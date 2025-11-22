package fmj.gamemenu

import fmj.Global
import fmj.characters.Player
import fmj.graphics.TextRender
import fmj.magic.MagicRestore
import fmj.scene.ScreenMainGame
import fmj.views.BaseScreen
import fmj.views.GameNode

import graphics.Canvas
import graphics.Rect

class ScreenUseMagic(override val parent: GameNode,
                     private val mMagic: MagicRestore,
                     private var mScr: Player) : BaseScreen {

    private var mCurPage = 0
    private var mCurActor = 0

    override fun update(delta: Long) {}

    override fun draw(canvas: Canvas) {
        canvas.drawColor(Global.COLOR_WHITE)
        TextRender.drawText(canvas, mMagic.magicName, 0, sNameRect)
        val actor = game.playerList[mCurActor]
        actor.drawState(canvas, mCurPage)
        actor.drawHead(canvas, 5, 60)
    }

    override fun onKeyDown(key: Int) {
        if (key == Global.KEY_RIGHT && mCurActor < game.playerList.size - 1) {
            ++mCurActor
        } else if (key == Global.KEY_LEFT && mCurActor > 0) {
            --mCurActor
        } else if (key == Global.KEY_PAGEDOWN || key == Global.KEY_PAGEUP) {
            mCurPage = 1 - mCurPage
        }
    }

    override fun onKeyUp(key: Int) {
        if (key == Global.KEY_CANCEL) {
            popScreen()
        } else if (key == Global.KEY_ENTER) {
            // 检查魔法是否为群体魔法
            if (mMagic.isForAll) {
                // 群体恢复魔法：对所有活着的队员使用
                println("ScreenUseMagic: 使用群体恢复魔法 ${mMagic.magicName}")
                game.playerList.forEach { player ->
                    if (player.hp > 0) {  // 只对活着的角色使用
                        println("ScreenUseMagic: 对角色 ${player.name} 使用魔法")
                        mMagic.use(mScr, player)
                    }
                }
            } else {
                // 单体魔法：只对当前选中的角色使用
                println("ScreenUseMagic: 使用单体恢复魔法 ${mMagic.magicName} 对象: ${game.playerList[mCurActor].name}")
                mMagic.use(mScr, game.playerList[mCurActor])
            }
            popScreen()
        }
    }

    companion object {

        private val sNameRect = Rect(4, 4, 37, 96)
    }

}
