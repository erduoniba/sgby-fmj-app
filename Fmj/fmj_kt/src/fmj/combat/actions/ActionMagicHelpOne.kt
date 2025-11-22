package fmj.combat.actions

import fmj.characters.FightingCharacter
import fmj.characters.Player
import fmj.combat.anim.RaiseAnimation
import fmj.lib.ResSrs
import fmj.magic.BaseMagic
import graphics.Canvas

class ActionMagicHelpOne(attacker: FightingCharacter,
                         target: FightingCharacter, internal var magic: BaseMagic) : ActionSingleTarget(attacker, target) {
    
    init {
        println("ActionMagicHelpOne: 创建动作，施术者: ${attacker.name}, 目标: ${target.name} (HP: ${target.hp}/${target.maxHP}), 魔法: ${magic.magicName}")
    }

    private var mState = 1

    internal var mAni: ResSrs = ResSrs()

    internal var mAnix: Int = 0
    internal var mAniy: Int = 0

    internal var ox: Int = 0
    internal var oy: Int = 0

    override val isMagic = true

    override fun preproccess() {
        println("ActionMagicHelpOne: 准备执行魔法，施术者: ${mAttacker!!.name}, 目标: ${mTarget.name} (HP: ${mTarget.hp}/${mTarget.maxHP})")
        ox = mAttacker!!.combatX
        oy = mAttacker!!.combatY
        mAni = magic.magicAni!!
        mAni.start()
        mAni.setIteratorNum(2)
        // 治疗魔法的动画应该显示在目标位置（被治疗者）
        mAnix = mTarget.combatX
        mAniy = mTarget.combatY - mTarget.fightingSprite!!.height / 2
        val ohp = mTarget.hp
        println("ActionMagicHelpOne: 即将调用 magic.use，目标确认: ${mTarget.name} (HP: ${mTarget.hp})")
        magic.use(mAttacker!!, mTarget)
        val diff = mTarget.hp - ohp
        mRaiseAnimations.add(RaiseAnimation(mTarget.combatX, mTarget.combatTop, diff, 0))
    }

    override fun update(delta: Long): Boolean {
        super.update(delta)
        when (mState) {
            STATE_PRE -> if (mCurrentFrame < 10) {
                if (mAttacker is Player) {
                    mAttacker!!.fightingSprite!!.currentFrame = mCurrentFrame * 3 / 10 + 6
                } else {
                    mAttacker!!.setCombatPos(ox + 2, oy + 2)
                }
            } else {
                mState = STATE_ANI
            }

            STATE_ANI -> if (!mAni.update(delta)) { // 魔法动画完成
                mState = STATE_AFT
                if (mAttacker is Player) {
                    (mAttacker as Player).setFrameByState()
                } else {
                    mAttacker!!.fightingSprite!!.move(-2, -2)
                }
            }

            STATE_AFT -> return updateRaiseAnimation(delta)
        }
        return true
    }

    override fun draw(canvas: Canvas) {
        if (mState == STATE_ANI) {
            mAni.drawAbsolutely(canvas, mAnix, mAniy)
        } else if (mState == STATE_AFT) {
            drawRaiseAnimation(canvas)
        }
    }

    override fun rollbackToPhysical(): Action {
        val attacker = mAttacker!!
        return ActionNop(attacker)
    }

    companion object {

        private val STATE_PRE = 1 // 起手动画
        private val STATE_ANI = 2 // 魔法动画
        private val STATE_AFT = 3 // 伤害动画
    }

}
