package fmj.combat.actions

import fmj.characters.FightingCharacter
import graphics.Canvas

class ActionSelfHurt(target: FightingCharacter): ActionSingleTarget(target, target) {
    enum class State {Acting, Hurting}
    private var state = State.Acting

    private var ox = target.combatX
    private var oy = target.combatY

    override fun preproccess() {
        val target = mTarget

        target.backupStatus()
        target.attack(target, allowMiss = false)
        mRaiseAnimations.add(target.diffToAnimation())
    }

    override fun update(delta: Long): Boolean {
        super.update(delta)
        when (state) {
            State.Acting -> if (mCurrentFrame < 10) {
                mTarget.setCombatPos(ox + 2, oy + 2)
            } else {
                mTarget.setCombatPos(ox, oy)
                state = State.Hurting
            }
            State.Hurting -> return updateRaiseAnimation(delta)
        }
        return true
    }

    override fun draw(canvas: Canvas) {
        if (state == State.Hurting) {
            drawRaiseAnimation(canvas)
        }
    }
}