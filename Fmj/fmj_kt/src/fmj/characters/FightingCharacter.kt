package fmj.characters

import fmj.config.GameSettings
import fmj.combat.actions.CalcDamage
import fmj.combat.anim.Animation
import fmj.combat.anim.RaiseAnimation
import fmj.combat.anim.RaiseBitmapAnimation
import fmj.lib.DatLib
import fmj.magic.BaseMagic
import fmj.magic.ResMagicChain
import fmj.scene.SaveLoadGame
import java.Coder
import java.ObjectInput
import java.ObjectOutput
import java.random
import kotlin.math.max
import kotlin.math.min
import kotlin.math.sqrt
import kotlin.sequences.sequence

class Buff(var value: Int, var round: Int) {
    fun reset() {
        round = 0
        value = 0
    }

    fun add(round: Int) {
        if (round == 0) {
            value += 1
        } else {
            if (this.round == 0) {
                value += 1
            }
            this.round = max(this.round, round)
        }
    }

    fun fill(other: Buff) {
        other.value = value
        other.round = round
    }

    fun diffFrom(other: Buff): Buff {
        return Buff(value - other.value,
                round - other.round)
    }

    fun decay(mode: Int) {
        if (round > 0) {
            round -= 1
            if (round == 0) {
                when (mode) {
                    0 -> value -= 1
                    1 -> value = 0
                }
            }
        }
    }
}

class BuffMan(val buffs: Array<Buff> = Array(8) { Buff(0, 0) })
    : Coder
{
    override fun encode(out: ObjectOutput) {
        out.writeIntArray(buffs.map { it.value }.toIntArray())
        out.writeIntArray(buffs.map { it.round }.toIntArray())
    }

    override fun decode(coder: ObjectInput) {
        val values = coder.readIntArray()
        val rounds = coder.readIntArray()
        values.zip(rounds).forEachIndexed { index, pair ->
            buffs[index].value = pair.first
            buffs[index].round = pair.second
        }
    }

    fun getBuffs(mask: Int): Sequence<Buff> {
        return FightingCharacter.maskToIndexes(mask).map { buffs[it] }
    }

    fun hasBuff(mask: Int): Boolean {
        return getBuffs(mask).first().value > 0
    }

    fun addBuff(mask: Int, round: Int) {
        getBuffs(mask).forEach {
            it.add(round)
        }
    }

    fun delBuff(mask: Int) {
        getBuffs(mask).forEach {
            if (it.value > 0)
                it.value -= 1
        }
    }

    fun reset() {
        buffs.forEach { it.reset() }
    }

    fun fill(other: BuffMan) {
        buffs.zip(other.buffs).forEach {
            it.first.fill(it.second)
        }
    }

    fun diffFrom(other: BuffMan): BuffMan {
        val newbuffs = buffs.zip(other.buffs).map {
            it.first.diffFrom(it.second)
        }.toTypedArray()
        return BuffMan(newbuffs)
    }

    fun decay() {
        (0..3).forEach {
            buffs[it].decay(0)
        }
        (5..7).forEach {
            buffs[it].decay(1)
        }
    }

    companion object {
        fun fromInt(v: Int): BuffMan {
            val round = (v and 0xf0) shr 4
            val mask = v and 0xf
            return fromRoundAndMask(round, mask)
        }

        fun fromRoundAndMask(round:Int, mask: Int): BuffMan {
            val inds = FightingCharacter.maskToIndexes(mask)
            val man = BuffMan()
            inds.forEach {
                man.buffs[it].value = 1
                man.buffs[it].round = round
            }
            return man
        }
    }
}

fun health(mask: Int, obj: BuffMan) {
    obj.getBuffs(mask).forEach {
        it.value = 0
        it.round = 0
    }
}

fun calcBuff(at: BuffMan, df: BuffMan, st: BuffMan, luck: Int): BuffMan {
    val rv = BuffMan()
    val pluck = sqrt(luck.toDouble() / 100)
    (0..3).forEach {
        if (random() + 0.01 < pluck) {
            return@forEach
        }

        val a = at.buffs[it]
        val d = df.buffs[it]
        val s = st.buffs[it]

        if (d.value == 0 && a.value > 0) {
            s.add(a.round)
            rv.buffs[it].value = 1
        }
    }
    (5..7).forEach {
        if (at.buffs[it].value != 0) {
            st.buffs[it].value = -at.buffs[it].value
            st.buffs[it].round = at.buffs[it].round
        }
    }
    return rv
}

abstract class FightingCharacter : Character() {
    class Diff {
        var debuff = BuffMan()
        var hp = 0
        var mp = 0

        fun toAnimation(x: Int, y: Int): Animation {
            val buff = debuff.buffs.mapIndexed { index, buff ->
                if (buff.value != 0 || buff.round != 0) {
                    FightingCharacter.indexToMask(index)
                } else {
                    0
                }
            }.reduce { acc, i -> acc + i }

            return RaiseAnimation(x, y, hp, buff)
        }
    }

    /**
     * 人物战斗图
     */
    var fightingSprite: FightingSprite? = null

    /**中心坐标 */
    val combatX: Int
        get() = fightingSprite!!.combatX

    /**中心坐标 */
    val combatY: Int
        get() = fightingSprite!!.combatY

    val combatLeft: Int
        get() = fightingSprite!!.combatX - fightingSprite!!.width / 2

    val combatTop: Int
        get() = fightingSprite!!.combatY - fightingSprite!!.height / 2

    /**
     * 魔法链
     */
    var magicChain: ResMagicChain? = null

    /**
     * 等级
     */
    var level: Int = 0

    var maxHP: Int = 0
        set(maxHP) {
            // Monster类不设置上限，Player类才限制
            if (this is Monster) {
                field = maxHP
                if (maxHP > 9999) {
                    println("[FightingCharacter] Monster '$name' has maxHP: $maxHP (no limit applied)")
                }
            } else {
                val limit = if (GameSettings.enableEnhancedLimits) 9999 else 999
                field = min(limit, maxHP)
            }
        }

    var maxMP: Int = 0
        set(maxMP) {
            // Monster类不设置上限，Player类才限制
            if (this is Monster) {
                field = maxMP
                if (maxMP > 9999) {
                    println("[FightingCharacter] Monster '$name' has maxMP: $maxMP (no limit applied)")
                }
            } else {
                val limit = if (GameSettings.enableEnhancedLimits) 9999 else 999
                field = min(limit, maxMP)
            }
        }

    var hp: Int = 0
        set(hp) {
            if (field != hp && this is Player) {
                println("[HP_CHANGE] ${name} HP: $field -> $hp (变化: ${hp - field}) maxHP: $maxHP")
            }
            field = min(maxHP, hp)
        }

    val isAlive: Boolean
        get() = hp > 0

    var isVisiable = true

    var mp: Int = 0
        set(mp) {
            field = min(maxMP, mp)
        }

    var attack: Int = 0
        set(at) {
            // Monster类不设置上限，Player类才限制
            if (this is Monster) {
                field = max(0, at)
                if (at > 9999) {
                    println("[FightingCharacter] Monster '$name' has attack: $at (no limit applied)")
                }
            } else {
                val limit = if (GameSettings.enableEnhancedLimits) 9999 else 999
                field = min(limit, max(0, at))
            }
        } // 攻击

    var defend: Int = 0
        set(d) {
            // Monster类不设置上限，Player类才限制
            if (this is Monster) {
                field = max(0, d)
                if (d > 9999) {
                    println("[FightingCharacter] Monster '$name' has defend: $d (no limit applied)")
                }
            } else {
                val limit = if (GameSettings.enableEnhancedLimits) 9999 else 999
                field = min(limit, max(0, d))
            }
        } // 防御

    var speed: Int = 0
        set(s) {
            // Monster类不设置上限，Player类才限制
            if (this is Monster) {
                field = max(0, s)
                if (s > 127) {
                    println("[FightingCharacter] Monster '$name' has speed: $s (no limit applied)")
                }
            } else {
                val limit = if (GameSettings.enableEnhancedLimits) 127 else 99
                field = min(limit, max(0, s))
            }
        } // 身法

    var lingli: Int = 0
        set(l) {
            // Monster类不设置上限，Player类才限制
            if (this is Monster) {
                field = max(0, l)
                if (l > 127) {
                    println("[FightingCharacter] Monster '$name' has lingli: $l (no limit applied)")
                }
            } else {
                val limit = if (GameSettings.enableEnhancedLimits) 127 else 99
                field = min(limit, max(0, l))
            }
        } // 灵力

    var luck: Int = 0
        set(l) {
            // Monster类不设置上限，Player类才限制
            if (this is Monster) {
                field = max(0, l)
                if (l > 127) {
                    println("[FightingCharacter] Monster '$name' has luck: $l (no limit applied)")
                }
            } else {
                val limit = if (GameSettings.enableEnhancedLimits) 127 else 99
                field = min(limit, max(0, l))
            }
        } // 幸运

    var missed: Boolean = false

    /** 免疫毒乱封眠，不同装备可能具有相同的免疫效果，叠加之 */
    protected var buff = BuffMan()
    /** 身中毒乱封眠 */
    var debuff = BuffMan()
    /** 普通攻击产生(全体)毒乱封眠，对于主角，只有武器具有该效果 */
    protected var atbuff = BuffMan()

    val computedSpeed: Int
        get() = speed + speed*debuff.getBuffs(BUFF_MASK_SU).first().value / 100

    val computedAttack: Int
        get() = attack + attack*debuff.getBuffs(BUFF_MASK_GONG).first().value / 100

    val computedDefend: Int
        get() = defend + defend*debuff.getBuffs(BUFF_MASK_FANG).first().value / 100

    private val backup = Diff()

    val isPoisoned: Boolean
        get() = hasDebuff(BUFF_MASK_DU)

    val isConfusing: Boolean
        get() = hasDebuff(BUFF_MASK_LUAN)

    val isSealed: Boolean
        get() = hasDebuff(BUFF_MASK_FENG)

    val isSleeping: Boolean
        get() = hasDebuff(BUFF_MASK_MIAN)

    /** 设置中心坐标 */
    fun setCombatPos(x: Int, y: Int) {
        fightingSprite!!.setCombatPos(x, y)
    }

    /**
     * 是否免疫异常状态
     * @param mask 只能为下面几个值，或者他们的位或
     *
     *
     * `BUFF_MASK_DU`，
     * `BUFF_MASK_LUAN`，
     * `BUFF_MASK_FENG`，
     * `BUFF_MASK_MIAN`，
     * @return 是否免疫mask状态
     */
    fun hasBuff(mask: Int): Boolean {
        return buff.hasBuff(mask)
    }

    /**
     * 是否身中异常状态
     * @param mask 只能为下面几个值，或者他们的位或中的任意一个
     *
     *
     * `BUFF_MASK_DU`，
     * `BUFF_MASK_LUAN`，
     * `BUFF_MASK_FENG`，
     * `BUFF_MASK_MIAN`，
     * @return 是否身中mask状态
     */
    fun hasDebuff(mask: Int): Boolean {
        return debuff.hasBuff(mask)
    }

    fun resetDebuff() {
        debuff.reset()
    }

    /**
     * 攻击是否能够产生异常状态
     * @param mask 只能为下面几个值，或者他们的位或中的任意一个
     *
     *
     * `BUFF_MASK_DU`，
     * `BUFF_MASK_LUAN`，
     * `BUFF_MASK_FENG`，
     * `BUFF_MASK_MIAN`，
     * @return 物理攻击是否具有mask效果
     */
    fun hasAtbuff(mask: Int): Boolean {
        return atbuff.hasBuff(mask)
    }

    /**
     * 增加角色能够免疫的状态
     * @see {@link .hasBuff
     * @param mask
     * @param rounds
     */
    fun addBuff(mask: Int, rounds: Int = 0) {
        buff.addBuff(mask, rounds)
    }

    fun delBuff(mask: Int) {
        buff.delBuff(mask)
    }

    /**
     * 增加角色攻击能够产生的异常状态
     * @see {@link .hasAtbuff
     * @param mask
     */
    fun setAtbuff(mask: Int, rounds: Int) {
        atbuff.reset()
        atbuff.addBuff(mask, rounds)
    }

    fun resetAtbuff() {
        atbuff.reset()
    }

    fun attack(other: FightingCharacter, rate: Double = 1.0, allowMiss: Boolean = true): BuffMan {
        if (GameSettings.allowMiss && allowMiss) {
            if (CalcDamage.randomMiss(this, other)) {
                other.missed = true
                return BuffMan()
            }
        }
        // 使用原版精确的伤害计算公式，随机已包含在calcBaseDamage中
        val damage = (CalcDamage.calcBaseDamage(computedAttack, other.computedDefend, other is Player) * rate).toInt()
        other.hp = other.hp - damage
        return other.beAttackedWithBuff(atbuff)
    }

    fun beAttackedWithBuff(b: BuffMan, luck: Int? = null): BuffMan {
        return calcBuff(b, buff, debuff, luck?:this.luck)
    }

    fun backupStatus() {
        backup.hp = hp
        backup.mp = mp
        debuff.fill(backup.debuff)
        missed = false
    }

    fun diff(withBuff: Boolean): Diff {
        val diff = Diff()
        diff.mp = mp - backup.mp
        diff.hp = hp - backup.hp
        if (withBuff)
            diff.debuff = debuff.diffFrom(backup.debuff)
        return diff
    }

    fun diffToAnimation(withBuff: Boolean = true): Animation {
        return if (missed) {
            RaiseBitmapAnimation(combatX, combatY, DatLib.missBitmap)
        } else {
            diff(withBuff).toAnimation(combatX, combatY)
        }
    }

    fun decay() {
        buff.decay()
        debuff.decay()
    }

    open fun getAllMagics(): Collection<BaseMagic> {
        return magicChain?.getAllLearntMagics() ?: listOf()
    }

    companion object {

        val BUFF_MASK_ALL = 16
        val BUFF_MASK_DU = 8
        val BUFF_MASK_LUAN = 4
        val BUFF_MASK_FENG = 2
        val BUFF_MASK_MIAN = 1

        val BUFF_MASK_GONG = 32
        val BUFF_MASK_FANG = 64
        val BUFF_MASK_SU = 128

        fun isMaskSet(mask: Int, b: Int): Boolean
        {
            return mask and b != 0
        }

        fun maskToIndex(mask: Int): Int
        {
            return maskToIndexes(mask).first()
        }

        fun indexToMask(i: Int): Int
        {
            return when (i) {
                0 -> BUFF_MASK_MIAN
                1 -> BUFF_MASK_FENG
                2 -> BUFF_MASK_LUAN
                3 -> BUFF_MASK_DU
                4 -> BUFF_MASK_ALL
                5 -> BUFF_MASK_GONG
                6 -> BUFF_MASK_FANG
                7 -> BUFF_MASK_SU
                else -> 0
            }
        }

        fun maskToIndexes(mask: Int): Sequence<Int> = sequence {
            if (mask and 0x1 != 0) yield(0)
            if (mask and 0x2 != 0) yield(1)
            if (mask and 0x4 != 0) yield(2)
            if (mask and 0x8 != 0) yield(3)
            if (mask and 0x10 != 0) yield(4)
            if (mask and 0x20 != 0) yield(5)
            if (mask and 0x40 != 0) yield(6)
            if (mask and 0x80 != 0) yield(7)
        }
    }

}
