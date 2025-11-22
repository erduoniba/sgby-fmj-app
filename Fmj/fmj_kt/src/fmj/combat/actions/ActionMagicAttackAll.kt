package fmj.combat.actions

import fmj.characters.FightingCharacter
import fmj.characters.Player
import fmj.lib.ResSrs
import fmj.magic.MagicAttack

import graphics.Canvas

class ActionMagicAttackAll(attacker: FightingCharacter,
                           targets: List<FightingCharacter>, internal val magic: MagicAttack
) : ActionMultiTarget(attacker, targets) {

    private var mState = 1

    private var mAni: ResSrs? = null
    
    // 动画显示位置
    private var mAnix: Int = 0
    private var mAniy: Int = 0

    private var ox: Int = 0
    private var oy: Int = 0

    override val isMagic = true

    override fun preproccess() {
        val attacker = mAttacker?:return
        println("ActionMagicAttackAll: 准备执行群体攻击，施术者: ${attacker.name}, 目标数量: ${mTargets.size}")
        attacker.backupStatus()
        mTargets.forEach { 
            println("  目标: ${it.name} at (${it.combatX}, ${it.combatY}), isPlayer=${it is Player}")
            it.backupStatus() 
        }

        ox = attacker.combatX
        oy = attacker.combatY
        mAni = magic.magicAni
        mAni!!.start()
        mAni!!.setIteratorNum(2)
        
        // 动画显示在第一个目标的位置
        if (mTargets.isNotEmpty()) {
            val firstTarget = mTargets[0]
            mAnix = firstTarget.combatX
            mAniy = firstTarget.combatY - (firstTarget.fightingSprite?.height ?: 16) / 2
            println("ActionMagicAttackAll: 动画位置计算完成（第一个目标位置） -> ($mAnix, $mAniy)")
        }
        
        // 过滤出活着的敌人，但不修改原始mTargets列表
        val aliveTargets = mTargets.filter { it.isAlive }
        
        magic.use(attacker, aliveTargets)

        mRaiseAnimations.add(attacker.diffToAnimation())
        // 只为活着的敌人生成动画
        mRaiseAnimations.addAll(aliveTargets.map { it.diffToAnimation() })
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

            STATE_ANI -> if (!mAni!!.update(delta)) {
                mState = STATE_AFT
                if (mAttacker is Player) {
                    (mAttacker as Player).setFrameByState()
                } else {
                    mAttacker!!.fightingSprite!!.move(-2, -2)
                }
                if (mTargets[0] is Player) {
                    for (fc in mTargets) {
                        fc.fightingSprite!!.currentFrame = 10
                    }
                } else {
                    for (fc in mTargets) {
                        fc.fightingSprite!!.move(2, 2)
                    }
                }
            }

            STATE_AFT -> if (!updateRaiseAnimation(delta)) {
                if (mTargets[0] is Player) {
                    for (fc in mTargets) {
                        (fc as Player).setFrameByState()
                    }
                } else {
                    for (fc in mTargets) {
                        fc.fightingSprite!!.move(-2, -2)
                    }
                }
                return false
            }
        }
        return true
    }

    override fun draw(canvas: Canvas) {
        super.draw(canvas)
        if (mState == STATE_ANI) {
            println("ActionMagicAttackAll: 绘制动画 at ($mAnix, $mAniy)")
            mAni!!.drawAbsolutely(canvas, mAnix, mAniy)
        } else if (mState == STATE_AFT) {
            drawRaiseAnimation(canvas)
        }
    }

    override fun rollbackToPhysical(): Action {
        val attacker = mAttacker!!
        return if (attacker.hasAtbuff(FightingCharacter.BUFF_MASK_ALL))
            ActionPhysicalAttackAll(attacker, mTargets)
        else
            ActionPhysicalAttackOne(attacker, mTargets[0])
    }

    companion object {

        private val STATE_PRE = 1 // 起手动画
        private val STATE_ANI = 2 // 魔法动画
        private val STATE_AFT = 3 // 伤害动画
    }

}
