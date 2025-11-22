package fmj.combat.actions

import fmj.characters.FightingCharacter
import fmj.characters.Monster
import fmj.goods.BaseGoods

abstract class ActionSingleTarget(attacker: FightingCharacter,
                                  protected var mTarget: FightingCharacter) : Action() {

    override val isTargetAlive: Boolean
        get() = mTarget.isAlive

    override val isSingleTarget: Boolean
        get() = true

    init {
        mAttacker = attacker
    }

    override fun postExecute() {
        mTarget.isVisiable = mTarget.isAlive
    }

    override fun targetIsMonster(): Boolean {
        return mTarget is Monster
    }

    fun setTarget(fc: FightingCharacter) {
        mTarget = fc
    }

    fun steal(attacker: FightingCharacter): BaseGoods? {
        val target = mTarget
        if (target is Monster) {
            return target.stealGoods(attacker)
        }
        return null
    }
}
