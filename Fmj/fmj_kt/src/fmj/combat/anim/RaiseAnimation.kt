package fmj.combat.anim

import fmj.characters.FightingCharacter
import fmj.graphics.Util
import fmj.lib.DatLib
import fmj.lib.ResSrs

import graphics.Bitmap
import graphics.Canvas

class RaiseAnimation(private val x: Int, private val y: Int, hitpoint: Int, buff: Int): Animation {

    private var dy = 0
    private var dt = 0

    private val raiseNum: Bitmap = Util.getSmallSignedNumBitmap(hitpoint)

    private val srsList: MutableList<ResSrs>

    private var bShowNum: Boolean = false

    private var cnt: Long = 0

    private fun loadSafeResSrs(index: Int): ResSrs? {
        val res = DatLib.getRes(DatLib.ResType.SRS, 1, index, true)
        return if (res is ResSrs) {
            res
        } else {
            println("Warning: Failed to load SRS resource index=$index")
            null
        }
    }

    init {
        this.bShowNum = hitpoint != 0
        srsList = mutableListOf()
        if (buff and FightingCharacter.BUFF_MASK_DU == FightingCharacter.BUFF_MASK_DU) {
            loadSafeResSrs(243)?.let {
                srsList.add(it)
                it.start()
            }
        }
        if (buff and FightingCharacter.BUFF_MASK_LUAN == FightingCharacter.BUFF_MASK_LUAN) {
            loadSafeResSrs(244)?.let {
                srsList.add(it)
                it.start()
            }
        }
        if (buff and FightingCharacter.BUFF_MASK_FENG == FightingCharacter.BUFF_MASK_FENG) {
            loadSafeResSrs(245)?.let {
                srsList.add(it)
                it.start()
            }
        }
        if (buff and FightingCharacter.BUFF_MASK_MIAN == FightingCharacter.BUFF_MASK_MIAN) {
            loadSafeResSrs(246)?.let {
                srsList.add(it)
                it.start()
            }
        }
        if (buff and FightingCharacter.BUFF_MASK_GONG == FightingCharacter.BUFF_MASK_GONG) {
            loadSafeResSrs(240)?.let {
                srsList.add(it)
                it.start()
            }
        }
        if (buff and FightingCharacter.BUFF_MASK_FANG == FightingCharacter.BUFF_MASK_FANG) {
            loadSafeResSrs(241)?.let {
                srsList.add(it)
                it.start()
            }
        }
        if (buff and FightingCharacter.BUFF_MASK_SU == FightingCharacter.BUFF_MASK_SU) {
            loadSafeResSrs(242)?.let {
                srsList.add(it)
                it.start()
            }
        }
    }

    override fun update(delta: Long): Boolean {
        if (bShowNum) {
            cnt += delta
            if (cnt > 50) {
                cnt = 0
                ++dt
                dy -= dt
                if (dt > 4) {
                    bShowNum = false
                }
            }
        } else {
            if (srsList.isEmpty()) {
                return false
            } else {
                if (!srsList.first().update(delta)) {
                    srsList.removeAt(0)
                    return !srsList.isEmpty()
                }
            }
        }
        return true
    }

    override fun draw(canvas: Canvas) {
        if (bShowNum) {
            canvas.drawBitmap(raiseNum, x, y + dy)
        } else {
            if (srsList.size > 0) {
                srsList.first().drawAbsolutely(canvas, x, y)
            }
        }
    }

}

class RaiseBitmapAnimation(private val x: Int, private val y: Int, val image: Bitmap): Animation {

    private var dy = 0
    private var dt = 0

    private var cnt: Long = 0

    override fun update(delta: Long): Boolean {
        cnt += delta
        if (cnt > 50) {
            cnt = 0
            ++dt
            dy -= dt
            if (dt > 4) {
                return false
            }
        }
        return true
    }

    override fun draw(canvas: Canvas) {
        canvas.drawBitmap(image, x, y + dy)
    }
}
