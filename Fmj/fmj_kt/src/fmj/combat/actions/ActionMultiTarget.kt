package fmj.combat.actions

import fmj.characters.FightingCharacter
import fmj.characters.Monster

import graphics.Canvas

open class ActionMultiTarget(attacker: FightingCharacter,
                             targets: List<FightingCharacter>) : Action() {

    protected val mTargets: MutableList<FightingCharacter> = mutableListOf()

    override val isTargetAlive: Boolean
        get() {
            return mTargets.any { it.isAlive }
        }

    override val isSingleTarget: Boolean
        get() = false

    init {
        mAttacker = attacker
        mTargets.addAll(targets)
    }

    override fun postExecute() {
        for (fc in mTargets) {
            fc.isVisiable = fc.isAlive
        }
    }

    override fun targetIsMonster(): Boolean {
        return mTargets[0] is Monster
    }

}
