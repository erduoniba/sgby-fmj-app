package fmj.combat.actions

import fmj.characters.FightingCharacter
import fmj.characters.Monster
import fmj.characters.Player
import fmj.combat.anim.RaiseAnimation
import graphics.Canvas
import java.random

class ActionPhysicalAttackOne(attacker: FightingCharacter,
                              target: FightingCharacter) : ActionSingleTarget(attacker, target) {

    private val TOTAL_FRAME = 5
    private var dx: Float = 0.toFloat()
    private var dy: Float = 0.toFloat()
    private var ox: Int = 0
    private var oy: Int = 0

//    private val buffRound: Int = 0

    private var mTotalMark = true

    override fun preproccess() {
        val attacker = mAttacker?:return
        val target = mTarget

        target.backupStatus()

        ox = attacker.combatX
        oy = attacker.combatY
        dx = (target.combatX - attacker.combatX).toFloat() / TOTAL_FRAME
        dy = (target.combatY - attacker.combatY).toFloat() / TOTAL_FRAME

        attacker.attack(target)
        mRaiseAnimations.add(target.diffToAnimation())
    }

    override fun update(delta: Long): Boolean {
        super.update(delta)
        if (mCurrentFrame < TOTAL_FRAME) { // 发起动作
            mAttacker!!.setCombatPos((ox + dx * mCurrentFrame).toInt(), (oy + dy * mCurrentFrame).toInt())
            if (mAttacker is Monster) {
                val fs = (mAttacker as Monster).fightingSprite!!
                fs.currentFrame = fs.frameCnt * mCurrentFrame / TOTAL_FRAME + 1
            } else if (mAttacker is Player) {
                val fs = mAttacker!!.fightingSprite!!
                fs.currentFrame = 5 * mCurrentFrame / TOTAL_FRAME + 1
            }
        } else if (mCurrentFrame > TOTAL_FRAME) { // 扣血、异常状态的动画
            if (!updateRaiseAnimation(delta)) {
                if (mTarget is Player) {
                    (mTarget as Player).setFrameByState()
                } else {
                    mTarget.fightingSprite!!.move(-2, -2)
                }
                return false
            }
        } else if (mTotalMark) {
            mTotalMark = false
            mAttacker!!.setCombatPos(ox, oy)
            if (mAttacker is Monster) {
                val fs = (mAttacker as Monster).fightingSprite!!
                fs.currentFrame = 1
            } else if (mAttacker is Player) {
                (mAttacker as Player).setFrameByState()
            }
            if (mTarget is Player) {
                mTarget.fightingSprite!!.currentFrame = 10
            } else {
                mTarget.fightingSprite!!.move(2, 2)
            }
        }
        return true
    }

    override fun draw(canvas: Canvas) {
        if (mCurrentFrame >= TOTAL_FRAME) {
            drawRaiseAnimation(canvas)
        }
    }

}
