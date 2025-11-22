package fmj.goods

import fmj.characters.BuffMan
import fmj.characters.Player
import fmj.lib.ResSrs


/**
 * 07武器类
 * @author Chen
 */
class GoodsWeapon : GoodsEquipment(), Throwable {
    override val ani = ResSrs()
    override var affectMp: Int = 0
        private set
    override val affectHp: Int
        get() = mat * 50
    override val buff = BuffMan()

    override fun putOn(p: Player) {
        super.putOn(p)
        p.setAtbuff(mBitEffect, sumRound)
    }

    override fun takeOff(p: Player) {
        super.takeOff(p)
        p.resetAtbuff()
    }

    fun attackAll(): Boolean {
        return mBitEffect and 0x10 != 0
    }
}
