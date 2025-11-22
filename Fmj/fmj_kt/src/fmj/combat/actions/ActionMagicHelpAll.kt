package fmj.combat.actions

import fmj.characters.FightingCharacter
import fmj.characters.Player
import fmj.combat.anim.RaiseAnimation
import fmj.lib.ResSrs
import fmj.magic.BaseMagic

import graphics.Canvas

class ActionMagicHelpAll(attacker: FightingCharacter,
                         targets: List<FightingCharacter>, internal var magic: BaseMagic) : ActionMultiTarget(attacker, targets) {

    private var state = 1

    private lateinit var animation: ResSrs
    
    // 动画显示位置
    internal var mAnix: Int = 0
    internal var mAniy: Int = 0

    internal var ox: Int = 0
    internal var oy: Int = 0

    override val isMagic = true

    override fun preproccess() {
        val attacker = mAttacker?:return
        println("ActionMagicHelpAll: 准备执行群体魔法，施术者: ${attacker.name}, 目标数量: ${mTargets.size}")
        mTargets.forEach { 
            println("  目标: ${it.name} at (${it.combatX}, ${it.combatY}), isPlayer=${it is Player}")
            it.backupStatus() 
        }

        ox = attacker.combatX
        oy = attacker.combatY
        animation = magic.magicAni!!
        animation.start()
        animation.setIteratorNum(2)
        
        // 动画显示在第一个目标的位置
        if (mTargets.isNotEmpty()) {
            val firstTarget = mTargets[0]
            mAnix = firstTarget.combatX
            mAniy = firstTarget.combatY - (firstTarget.fightingSprite?.height ?: 16) / 2
            println("ActionMagicHelpAll: 动画位置计算完成（第一个目标位置） -> ($mAnix, $mAniy)")
        }
        
        // 修复：群体恢复魔法只消耗一次MP
        val currentMagic = magic
        if (currentMagic is fmj.magic.MagicRestore) {
            // 恢复魔法：只消耗一次MP，对所有目标应用效果
            if (attacker.mp >= currentMagic.costMp) {
                attacker.mp = attacker.mp - currentMagic.costMp
                mTargets.forEach {
                    currentMagic.applyEffect(attacker, it)
                }
            }
        } else {
            // 其他类型的群体辅助魔法，暂时保持原逻辑
            mTargets.forEach {
                magic.use(attacker, it)
            }
        }
        mRaiseAnimations.addAll(mTargets.map { it.diffToAnimation(false) })
    }

    override fun update(delta: Long): Boolean {
        super.update(delta)
        when (state) {
            STATE_PRE -> if (mCurrentFrame < 10) {
                if (mAttacker is Player) {
                    mAttacker!!.fightingSprite!!.currentFrame = mCurrentFrame * 3 / 10 + 6
                } else {
                    mAttacker!!.setCombatPos(ox + 2, oy + 2)
                }
            } else {
                state = STATE_ANI
            }

            STATE_ANI -> if (!animation.update(delta)) { // 魔法动画完成
                state = STATE_AFT
                if (mAttacker is Player) {
                    (mAttacker as Player).setFrameByState()
                } else {
                    mAttacker!!.fightingSprite!!.move(-2, -2)
                }
            }

            STATE_AFT -> return updateRaiseAnimation(delta)
        }//			break;
        return true
    }

    override fun draw(canvas: Canvas) {
        if (state == STATE_ANI) {
            println("ActionMagicHelpAll: 绘制动画 at ($mAnix, $mAniy)")
            animation.drawAbsolutely(canvas, mAnix, mAniy)
        } else if (state == STATE_AFT) {
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
