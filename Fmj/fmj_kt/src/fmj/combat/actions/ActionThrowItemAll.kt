package fmj.combat.actions

import fmj.characters.FightingCharacter
import fmj.characters.Player
import fmj.goods.BaseGoods
import fmj.goods.Throwable
import fmj.lib.ResSrs

import graphics.Canvas

class ActionThrowItemAll(attacker: FightingCharacter,
                         targets: List<FightingCharacter>, internal var weapon: Throwable) : ActionMultiTarget(attacker, targets) {

    private var mState = 1

    private var mAni: ResSrs = ResSrs()
    
    // 动画显示位置
    private var mAnix: Int = 0
    private var mAniy: Int = 0

    private var ox: Int = 0
    private var oy: Int = 0

    override fun preproccess() {
        val attacker = mAttacker?:return
        println("ActionThrowItemAll: 准备投掷物品，投掷者: ${attacker.name}, 目标数量: ${mTargets.size}")
        mTargets.forEach { 
            println("  目标: ${it.name} at (${it.combatX}, ${it.combatY}), isPlayer=${it is Player}")
            it.backupStatus() 
        }

        ox = attacker.combatX
        oy = attacker.combatY
        mAni = weapon.ani
        mAni.start()
        mAni.setIteratorNum(2)
        
        // 动画显示在第一个目标的位置
        if (mTargets.isNotEmpty()) {
            val firstTarget = mTargets[0]
            mAnix = firstTarget.combatX
            mAniy = firstTarget.combatY - (firstTarget.fightingSprite?.height ?: 16) / 2
            println("ActionThrowItemAll: 动画位置计算完成（第一个目标位置） -> ($mAnix, $mAniy)")
        }
        
        mTargets.forEach { weapon.attack(it) }
        mRaiseAnimations.addAll(mTargets.map { it.diffToAnimation() })
    }

    override fun update(delta: Long): Boolean {
        super.update(delta)
        when (mState) {
            STATE_PRE -> if (mCurrentFrame < 10) {
                if (mAttacker is Player) {
                    (mAttacker as Player).fightingSprite!!.currentFrame = mCurrentFrame * 3 / 10 + 6
                } else {
                    mAttacker!!.setCombatPos(ox + 2, oy + 2)
                }
            } else {
                mState = STATE_ANI
            }

            STATE_ANI -> if (!mAni.update(delta)) {
                mState = STATE_AFT
                if (mAttacker is Player) {
                    (mAttacker as Player).setFrameByState()
                } else {
                    mAttacker!!.fightingSprite!!.move(-2, -2)
                }
                if (!targetIsMonster()) {
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
                if (targetIsMonster()) {
                    for (fc in mTargets) {
                        fc.fightingSprite!!.move(-2, -2)
                    }
                } else {
                    for (fc in mTargets) {
                        (fc as Player).setFrameByState()
                    }
                }
                return false
            }
        }
        return true
    }

    override fun draw(canvas: Canvas) {
        if (mState == STATE_ANI) {
            println("ActionThrowItemAll: 绘制动画 at ($mAnix, $mAniy)")
            mAni.drawAbsolutely(canvas, mAnix, mAniy)
        } else if (mState == STATE_AFT) {
            drawRaiseAnimation(canvas)
        }
    }

    override fun cancel() {
        Player.sGoodsList.addGoods(weapon as BaseGoods)
    }

    companion object {

        private val STATE_PRE = 1 // 起手动画
        private val STATE_ANI = 2 // 魔法动画
        private val STATE_AFT = 3 // 伤害动画
    }

}
