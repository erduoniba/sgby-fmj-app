package fmj.combat.actions

import fmj.characters.FightingCharacter
import fmj.characters.Player
import fmj.combat.anim.RaiseAnimation
import fmj.goods.BaseGoods
import fmj.goods.GoodsMedicine
import fmj.goods.IEatMedicine
import fmj.lib.DatLib
import fmj.lib.ResSrs

import graphics.Canvas

class ActionUseItemOne(attacker: FightingCharacter, target: FightingCharacter, internal var goods: BaseGoods) : ActionSingleTarget(attacker, target) {

    private var mState = 1

    internal var mAni: ResSrs = ResSrs()

    internal var mAnix: Int = 0
    internal var mAniy: Int = 0

    internal var ox: Int = 0
    internal var oy: Int = 0

    override fun preproccess() {
        mTarget.backupStatus()
        if (goods is IEatMedicine) {
            // 处理所有可食用药物（GoodsMedicine, GoodsMedicineLife, GoodsMedicineChg4Ever）
            val targetPlayer = mTarget as Player
            val wasTargetAlive = targetPlayer.hp > 0
            println("ActionUseItemOne: 使用药物 ${goods::class.simpleName}(type=${goods.type}) 在目标 ${targetPlayer.name} (HP: ${targetPlayer.hp}/${targetPlayer.maxHP}, 存活状态: $wasTargetAlive)")
            
            val currentGoods = goods // 使用局部变量避免智能转换问题
            if (currentGoods is GoodsMedicine) {
                mAni = currentGoods.ani!!
            } else {
                val aniRes = DatLib.getRes(DatLib.ResType.SRS, 2, 1, false)
                mAni = if (aniRes is ResSrs) aniRes else {
                    println("Warning: Failed to load SRS animation for ActionUseItemOne")
                    ResSrs() // 使用空的 ResSrs
                }
            }
            // 统一调用eat方法，支持所有药物类型
            if (currentGoods is IEatMedicine) {
                currentGoods.eat(targetPlayer)
            } else {
                println("Warning: Item ${currentGoods::class.simpleName} does not implement IEatMedicine interface")
            }
            
            val isTargetAliveAfter = targetPlayer.hp > 0
            println("ActionUseItemOne: 使用药物后目标 ${targetPlayer.name} (HP: ${targetPlayer.hp}/${targetPlayer.maxHP}, 存活状态: $isTargetAliveAfter)")
        } else {
            val aniRes = DatLib.getRes(DatLib.ResType.SRS, 2, 1, false)
            mAni = if (aniRes is ResSrs) aniRes else {
                println("Warning: Failed to load SRS animation for ActionUseItemOne fallback")
                ResSrs() // 使用空的 ResSrs
            }
        }
        mAni.start()
        mAni.setIteratorNum(2)
        mAnix = mTarget.combatX
        mAniy = mTarget.combatY
        mRaiseAnimations.add(mTarget.diffToAnimation())
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

            STATE_ANI -> if (!mAni.update(delta)) { // 魔法动画完成
                mState = STATE_AFT
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
        if (mState == STATE_ANI) {
            mAni.drawAbsolutely(canvas, mAnix, mAniy)
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
