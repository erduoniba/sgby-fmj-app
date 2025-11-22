package fmj.combat.actions

import fmj.characters.FightingCharacter
import fmj.characters.Player
import fmj.goods.BaseGoods
import fmj.goods.GoodsMedicine
import fmj.lib.DatLib
import fmj.lib.ResSrs

import graphics.Canvas

class ActionUseItemAll(attacker: FightingCharacter,
                       targets: List<FightingCharacter>, internal val goods: BaseGoods) : ActionMultiTarget(attacker, targets) {

    private var mState = 1

    private var mAni: ResSrs? = null
    
    // 动画显示位置
    internal var mAnix: Int = 0
    internal var mAniy: Int = 0

    internal var ox: Int = 0
    internal var oy: Int = 0

    override fun preproccess() {
        val attacker = mAttacker?:return
        println("ActionUseItemAll: 准备使用物品，使用者: ${attacker.name}, 目标数量: ${mTargets.size}")
        mTargets.forEach { 
            println("  目标: ${it.name} at (${it.combatX}, ${it.combatY}), isPlayer=${it is Player}")
            it.backupStatus() 
        }

        ox = attacker.combatX
        oy = attacker.combatY
        if (goods is GoodsMedicine) {
            mAni = goods.ani
            mTargets.forEach { target ->
                if (target is Player) {
                    goods.eat(target)
                } else {
                    println("Warning: Target ${target.name} is not a Player, skipping medicine effect")
                }
            }
        } else {
            val aniRes = DatLib.getRes(DatLib.ResType.SRS, 2, 1, true)
            mAni = if (aniRes is ResSrs) aniRes else {
                println("Warning: Failed to load SRS animation for ActionUseItemAll")
                null
            }
        }
        mAni?.start()
        mAni?.setIteratorNum(2)
        
        // 动画显示在第一个目标的位置
        if (mTargets.isNotEmpty()) {
            val firstTarget = mTargets[0]
            mAnix = firstTarget.combatX
            mAniy = firstTarget.combatY - (firstTarget.fightingSprite?.height ?: 16) / 2
            println("ActionUseItemAll: 动画位置计算完成（第一个目标位置） -> ($mAnix, $mAniy)")
        }
        
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

            STATE_ANI -> if (mAni?.update(delta) != true) { // 魔法动画完成
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
            println("ActionUseItemAll: 绘制动画 at ($mAnix, $mAniy)")
            mAni?.drawAbsolutely(canvas, mAnix, mAniy)
        } else if (mState == STATE_AFT) {
            drawRaiseAnimation(canvas)
        }
    }

    override fun cancel() {
        Player.sGoodsList.addGoods(goods)
    }

    companion object {

        private val STATE_PRE = 1 // 起手动画
        private val STATE_ANI = 2 // 魔法动画
        private val STATE_AFT = 3 // 伤害动画
    }

}
