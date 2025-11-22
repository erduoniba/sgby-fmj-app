package fmj.combat.actions

import fmj.characters.FightingCharacter

import graphics.Canvas

class ActionDefend(fc: FightingCharacter)
    : ActionSingleTarget(fc, fc) {

    override val isTargetAlive: Boolean
        get() = true

    override val isSingleTarget: Boolean
        get() = false

    override fun preproccess() {}

    override fun postExecute() {}

    override fun targetIsMonster(): Boolean {
        return true
    }

    override fun update(delta: Long): Boolean {
        return false
    }

    override fun draw(canvas: Canvas) {}
}
