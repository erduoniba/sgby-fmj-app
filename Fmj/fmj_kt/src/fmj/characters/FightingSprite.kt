package fmj.characters

import fmj.lib.DatLib
import fmj.lib.ResImage
import graphics.Canvas

class FightingSprite(resType: DatLib.ResType, index: Int) {

    private var mImage: ResImage? = null

    /**
     *
     * @param i > 0
     */
    var currentFrame = 1

    /** 在战斗场景中的屏幕坐标 */
    var combatX: Int = 0
        private set
    var combatY: Int = 0
        private set

    val width: Int
        get() = mImage!!.width

    val height: Int
        get() = mImage!!.height

    val frameCnt: Int
        get() = mImage!!.number

    init {
        if (resType == DatLib.ResType.ACP) { // 怪物的
            mImage = DatLib.Companion.getRes(DatLib.ResType.ACP, 3, index) as ResImage
        } else if (resType == DatLib.ResType.PIC) { // 玩家角色的
            mImage = DatLib.Companion.getRes(DatLib.ResType.PIC, 3, index) as ResImage
        } else {
            throw Error("resType Error. resType=$resType")
        }
    }

    fun draw(canvas: Canvas) {
        mImage!!.draw(canvas, currentFrame, combatX - mImage!!.width / 2,
                combatY - mImage!!.height / 2)
    }

    fun draw(canvas: Canvas, x: Int, y: Int) {
        mImage!!.draw(canvas, currentFrame, x, y)
    }

    fun setCombatPos(x: Int, y: Int) {
        combatX = x
        combatY = y
    }

    fun move(dx: Int, dy: Int) {
        combatX += dx
        combatY += dy
    }
}
