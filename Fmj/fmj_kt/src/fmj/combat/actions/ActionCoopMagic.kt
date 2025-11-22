package fmj.combat.actions

import fmj.characters.FightingCharacter
import fmj.characters.Player
import fmj.combat.anim.RaiseAnimation
import fmj.goods.GoodsDecorations
import fmj.lib.DatLib
import fmj.lib.ResSrs
import fmj.magic.MagicAttack

import graphics.Canvas

class ActionCoopMagic : Action {

    private var mState = STATE_MOV

    internal var mActors: List<Player>

    private var mMonsters: MutableList<FightingCharacter> = mutableListOf()

    private var mMonster: FightingCharacter
        get() = mMonsters[0]
        set(value) {
            mMonsters = mutableListOf(value)
        }

    override var isSingleTarget: Boolean = false
        internal set

    internal var magic: MagicAttack? = null

    private var mAni: ResSrs = ResSrs()

    private var dxy: Array<FloatArray>? = null
    private var oxy: Array<IntArray>? = null

    private var mAniX: Int = 0
    private var mAniY: Int = 0

    private val coopMagic: MagicAttack?
        get() {
            val firstPlayer = mActors[0]
            val dc = firstPlayer.equipmentsArray[0] as GoodsDecorations?
            return dc?.coopMagic
        }

    override val priority: Int
        get() = mActors[0].computedDefend

    override val isAttackerActionable: Boolean
        get() = mActors.filter { it.isAlive && !it.isSleeping }.size >= 2

    override val isAttackerSleep: Boolean
        get() = mActors.all { it.isSleeping }

    override val isAttackerConfusing: Boolean
        get() = false

    override val isAttackerSealed: Boolean
        get() = false

    override val isTargetAlive: Boolean
        get() = if (isSingleTarget) {
            mMonster.isAlive
        } else {
            mMonsters.any { it.isAlive }
        }

    constructor(actors: List<Player>, monster: FightingCharacter) {
        mActors = actors
        mMonster = monster
        isSingleTarget = true

        magic = coopMagic
    }

    constructor(actors: List<Player>, monsters: List<FightingCharacter>) {
        mActors = actors
        mMonsters = mutableListOf()
        mMonsters.addAll(monsters)
        isSingleTarget = false

        magic = coopMagic
    }

    override fun preproccess() {
        mMonsters.removeAll { !it.isAlive }
        if (mMonsters.isEmpty())
            return
        mMonsters.forEach { it.backupStatus() }
        mActors.forEach { it.backupStatus() }

        // 调整合体攻击位置向右下角移动30像素
        val midpos = arrayOf(intArrayOf(92 + 30, 52 + 30), intArrayOf(109 + 30, 63 + 30), intArrayOf(126 + 30, 74 + 30))
        dxy = Array(mActors.size) { FloatArray(2) }
        oxy = Array(mActors.size) { IntArray(2) }
        for (i in mActors.indices) {
            oxy!![i][0] = mActors[i].combatX
            oxy!![i][1] = mActors[i].combatY
        }
        for (i in dxy!!.indices) {
            dxy!![i][0] = (midpos[i][0] - oxy!![i][0]).toFloat()
            dxy!![i][0] /= MOV_FRAME.toFloat()
            dxy!![i][1] = (midpos[i][1] - oxy!![i][1]).toFloat()
            dxy!![i][1] /= MOV_FRAME.toFloat()
        }

        if (isSingleTarget) {
            mAniX = mMonster.combatX
            mAniY = mMonster.combatY - mMonster.fightingSprite!!.height / 2
        } else {
            mAniY = 0
            mAniX = mAniY
        }

        mAni = if (magic == null) {
            val aniRes = DatLib.getRes(DatLib.ResType.SRS, 2, 240, false)
            if (aniRes is ResSrs) aniRes else {
                println("Warning: Failed to load SRS animation for ActionCoopMagic")
                ResSrs() // 使用空的 ResSrs
            }
        } else {
            magic!!.magicAni!!
        }
        val mgc = magic
        mActors.forEach {
            val player = it
            if (mgc != null) {
                if (isSingleTarget) {
                    mgc.use(it, mMonster)
                } else {
                    mgc.use(it, mMonsters)
                }
            } else {
                mMonsters.forEach {
                    player.attack(it, 1.6)
                }
            }
        }
        mMonsters.forEach { mRaiseAnimations.add(it.diffToAnimation()) }
        mActors.forEach { mRaiseAnimations.add(it.diffToAnimation()) }
        mAni.start()
    }

    override fun update(delta: Long): Boolean {
        if (mMonsters.isEmpty())
            return false
        super.update(delta)
        when (mState) {
            STATE_MOV -> if (mCurrentFrame < MOV_FRAME) {
                for (i in mActors.indices) {
                    val x = (oxy!![i][0] + dxy!![i][0] * mCurrentFrame)
                    val y = (oxy!![i][1] + dxy!![i][1] * mCurrentFrame)
                    mActors[i].setCombatPos(x.toInt(), y.toInt())
                }
            } else {
                mState = STATE_PRE
            }

            STATE_PRE -> if (mCurrentFrame < 10 + MOV_FRAME) {
                for (i in mActors.indices) {
                    mActors[i].fightingSprite!!.currentFrame = (mCurrentFrame - MOV_FRAME) * 3 / 10 + 6
                }
            } else {
                mState = STATE_ANI
            }

            STATE_ANI -> if (!mAni.update(delta)) {
                mState = STATE_AFT
                for (i in mActors.indices) {
                    mActors[i].setFrameByState()
                    mActors[i].setCombatPos(oxy!![i][0], oxy!![i][1])
                }
            }

            STATE_AFT -> if (!updateRaiseAnimation(delta)) {
                return false
            }
        }
        return true
    }

    override fun draw(canvas: Canvas) {
        when (mState) {
            STATE_ANI -> mAni.drawAbsolutely(canvas, mAniX, mAniY)
            STATE_AFT -> drawRaiseAnimation(canvas)
        }
    }

    override fun postExecute() {
        mMonsters.forEach { it.isVisiable = it.isAlive }
    }

    override fun postAction(): PostAction {
        return AwardAndPunishPostAction(mActors)
    }

    override fun targetIsMonster(): Boolean {
        return true
    }

    override fun decay() {
        mActors.forEach { it.decay() }
    }

    companion object {

        private val STATE_MOV = 0 // 移位动画
        private val STATE_PRE = 1 // 起手动画
        private val STATE_ANI = 2 // 魔法动画
        private val STATE_AFT = 3 // 伤害动画

        private val MOV_FRAME = 5 // 移位帧数
    }


}
