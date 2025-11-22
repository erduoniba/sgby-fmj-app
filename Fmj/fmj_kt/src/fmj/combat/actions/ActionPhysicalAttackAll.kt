package fmj.combat.actions

import fmj.characters.FightingCharacter
import fmj.characters.Monster
import fmj.characters.Player
import fmj.combat.anim.RaiseAnimation

import graphics.Canvas
import java.random

class ActionPhysicalAttackAll(attacker: FightingCharacter,
                              targets: List<FightingCharacter>) : ActionMultiTarget(attacker, targets) {

    private val TOTAL_FRAME = 5
    private var dx: Float = 0.toFloat()
    private var dy: Float = 0.toFloat()
    private var ox: Int = 0
    private var oy: Int = 0

    override fun preproccess() {
        val attacker = mAttacker?:return
        ox = attacker.combatX
        oy = attacker.combatY
        dx = (44.0f - attacker.combatX) / TOTAL_FRAME
        dy = (14.0f - attacker.combatY) / TOTAL_FRAME
        // 过滤出活着的敌人，但不修改原始mTargets列表
        val aliveTargets = mTargets.filter { it.isAlive }
        aliveTargets.forEach { fc ->
            fc.backupStatus()
            attacker.attack(fc)
        }
        
        // 只为活着的敌人生成动画
        mRaiseAnimations.addAll(aliveTargets.map { it.diffToAnimation(true) })
    }

    override fun update(delta: Long): Boolean {
        super.update(delta)
        if (mCurrentFrame < TOTAL_FRAME) { // 发起动作
            mAttacker!!.setCombatPos((ox + dx * mCurrentFrame).toInt(), (oy + dy * mCurrentFrame).toInt())
            if (mAttacker is Monster) {
                val fs = mAttacker!!.fightingSprite!!
                fs.currentFrame = fs.frameCnt * mCurrentFrame / TOTAL_FRAME + 1
            } else if (mAttacker is Player) {
                val fs = mAttacker!!.fightingSprite!!
                fs.currentFrame = 5 * mCurrentFrame / TOTAL_FRAME + 1
            }
        } else if (mCurrentFrame > TOTAL_FRAME) { // 扣血、异常状态的动画
            return updateRaiseAnimation(delta)
        } else {
            mAttacker!!.setCombatPos(ox, oy)
            if (mAttacker is Monster) {
                mAttacker!!.fightingSprite!!.currentFrame = 1
            } else if (mAttacker is Player) {
                val fs = mAttacker!!.fightingSprite!!
                fs.currentFrame = 1
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
