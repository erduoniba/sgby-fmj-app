package fmj.combat.actions

import fmj.characters.FightingCharacter
import fmj.combat.anim.Animation
import graphics.Canvas

abstract class Action {

    /** 动作的发起者 */
    var mAttacker: FightingCharacter? = null
        protected set

    private var mTimeCnt: Long = 0
    protected var mCurrentFrame = 0
    protected var mRaiseAnimations: MutableList<Animation> = arrayListOf()


    /**
     *
     * @return 动作发起者的身法
     */
    open val priority: Int
        get() = mAttacker!!.computedSpeed

    open val isAttackerActionable: Boolean
        get() = mAttacker!!.isAlive && !isAttackerSleep

    open val isAttackerSleep: Boolean
        get() = mAttacker!!.isSleeping

    open val isAttackerConfusing: Boolean
        get() = mAttacker!!.isConfusing

    open val isAttackerSealed: Boolean
        get() = mAttacker!!.isSealed

    open val isMagic = false

    abstract val isTargetAlive: Boolean

    abstract val isSingleTarget: Boolean

    /**
     * 动作产生的影响，播放动作动画之前执行一次。
     */
    open fun preproccess() {

    }

    /**
     * 隐藏死亡角色
     */
    abstract fun postExecute()

    open fun postAction(): PostAction {
        val attacker = mAttacker ?: return PostAction()
        return AwardAndPunishPostAction(arrayListOf(attacker))
    }

    open fun updateRaiseAnimation(delta: Long): Boolean {
        mRaiseAnimations.removeAll { !it.update(delta) }
        return !mRaiseAnimations.isEmpty()
    }

    open fun drawRaiseAnimation(canvas: Canvas) {
        mRaiseAnimations.forEach {
            it.draw(canvas)
        }
    }

    /**
     *
     * @param delta
     * @return 执行完毕返回`false`，否则返回`true`
     */
    open fun update(delta: Long): Boolean {
        mTimeCnt += delta
        if (mTimeCnt >= DELTA) {
            ++mCurrentFrame
            mTimeCnt = 0
        }
        return true
    }

    open fun draw(canvas: Canvas) {}

    abstract fun targetIsMonster(): Boolean

    open fun decay() {
        mAttacker?.decay()
    }

    open fun cancel() {}

    open fun rollbackToPhysical(): Action {
        return ActionNop(mAttacker!!)
    }

    companion object {

        private val DELTA = 1000 / 20
    }
}
