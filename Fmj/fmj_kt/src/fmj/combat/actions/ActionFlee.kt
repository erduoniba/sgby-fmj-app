package fmj.combat.actions

import fmj.characters.Player
import graphics.Canvas

import java.Runnable

class ActionFlee
/**
 *
 * @param p 逃跑者
 * @param fleeSuc 是否逃跑成功
 * @param runAft 逃跑动作完毕后，执行之
 */
(private val player: Player, private val fleeSucceed: Boolean, private val runAfterFlee: Runnable?) : Action() {

    private val FRAME_CNT = 5

    private var ox: Int = 0
    private var oy: Int = 0
    private var dy: Int = 0

    override val priority: Int
        get() = player.computedSpeed * 100

    override val isAttackerActionable: Boolean
        get() = true

    override val isAttackerConfusing: Boolean
        get() = false

    override val isTargetAlive: Boolean
        get() = true

    override val isSingleTarget: Boolean
        get() = false

    override fun preproccess() {
        ox = player.combatX
        oy = player.combatY
        dy = (96 - oy) / FRAME_CNT
        player.fightingSprite!!.currentFrame = 1
    }

    override fun update(delta: Long): Boolean {
        super.update(delta)
        if (mCurrentFrame < FRAME_CNT) {
            player.setCombatPos(ox, oy + dy * mCurrentFrame)
            return true
        } else if (!fleeSucceed && mCurrentFrame < FRAME_CNT + 2) {
            player.setCombatPos(ox, oy)
            player.fightingSprite!!.currentFrame = 11
        } else if (!fleeSucceed) {
            player.setFrameByState()
        }
        return false
    }

    override fun postExecute() {
        if (fleeSucceed && runAfterFlee != null) {
            runAfterFlee.run()
        } else {
            player.setCombatPos(ox, oy)
        }
    }

    override fun updateRaiseAnimation(delta: Long): Boolean {
        return false
    }

    override fun drawRaiseAnimation(canvas: Canvas) {
    }

    override fun targetIsMonster(): Boolean {
        return false
    }
}
