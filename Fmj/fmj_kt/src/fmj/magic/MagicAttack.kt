package fmj.magic

import fmj.characters.BuffMan
import fmj.characters.FightingCharacter
import fmj.characters.Player
import fmj.characters.Monster
import fmj.combat.actions.CalcDamage
import fmj.config.GameSettings
import fmj.scene.SaveLoadGame
import kotlin.math.abs
import kotlin.math.max

/**
 * 01攻击型
 * @author Chen
 */
class MagicAttack : BaseMagic() {

    companion object {
        // 调试开关：设为 true 时输出详细的魔法伤害计算日志
        private const val DEBUG_DAMAGE = false
    }

    // 调试日志输出方法
    private fun debugLog(message: String) {
        if (DEBUG_DAMAGE) {
            println(message)
        }
    }

    private var mHp: Int = 0//-8000~+8000，为正数时表示敌人损失生命的基数，为负数时表示从敌人身上吸取生命的基数
    private var mMp: Int = 0//-8000~+8000，为正数时表示敌人损失真气的基数，为负数时表示从敌人身上吸取真气的基数
    private var mDf: Int = 0//0~100，表示敌人的防御力减弱的百分比
    private var mAt: Int = 0//0~100，表示敌人的攻击力减弱的百分比
    private var mBuff: Int = 0//高四位 持续回合，低四位毒、乱、封、眠
    private var mSu: Int = 0//速 0~100，表示敌人的身法减慢的百分比

    private val buff: BuffMan
        get() {
            val atbuff = BuffMan.fromInt(mBuff)
            atbuff.buffs[FightingCharacter.maskToIndex(FightingCharacter.BUFF_MASK_FANG)].value = mDf
            atbuff.buffs[FightingCharacter.maskToIndex(FightingCharacter.BUFF_MASK_GONG)].value = mAt
            atbuff.buffs[FightingCharacter.maskToIndex(FightingCharacter.BUFF_MASK_SU)].value = mSu
            return atbuff
        }

    override fun setOtherData(buf: ByteArray, offset: Int) {
        mHp = get2BytesSInt(buf, offset + 0x12)
        mMp = get2BytesSInt(buf, offset + 0x14)
        mDf = buf[offset + 0x16].toInt() and 0xff
        mAt = buf[offset + 0x17].toInt() and 0xff
        mBuff = buf[offset + 0x18].toInt() and 0xff
        mSu = buf[offset + 0x19].toInt() and 0xff
    }

    /**
     * 计算魔法伤害 - 支持原版/简化公式切换
     * 原版公式：基于C引擎文档的精确伤害计算
     * 简化公式：基于灵力差值的简单计算
     */
    private fun calcHurt(src: FightingCharacter, dst: FightingCharacter, hp: Int): Int {
        if (hp == 0) return 0

        debugLog("  【calcHurt】基础hp=$hp, 公式=${if (GameSettings.useOriginalDamageFormula) "原版" else "简化"}")

        val result = if (hp > 0) {
            // 攻击性魔法（正伤害）
            if (GameSettings.useOriginalDamageFormula) {
                // 原版C引擎公式
                val damage = calcMagicDamageOriginal(src, dst, hp)
                debugLog("  【calcHurt】原版公式计算伤害: $damage")
                damage
            } else {
                // 简化公式
                val add = (src.lingli - dst.lingli).toDouble() / 100
                val damage = max(hp + (hp * add).toInt(), 0)
                debugLog("  【calcHurt】简化公式: $hp + ($hp * $add) = $damage")
                damage
            }
        } else {
            // 吸血类魔法（负伤害）
            val rate = when {
                dst.level <= 8 -> 1
                dst.level <= 16 -> 2
                else -> 3
            }
            val absorbDamage = hp * rate
            debugLog("  【calcHurt】吸血魔法: $hp * $rate = $absorbDamage")
            absorbDamage
        }

        return result
    }
    
    /**
     * 原版C引擎魔法伤害计算
     * 基于敌人魔法释放计算公式文档.md
     */
    private fun calcMagicDamageOriginal(caster: FightingCharacter, target: FightingCharacter, baseDamage: Int): Int {
        // 基础伤害 = 法术基础伤害值
        var damage = baseDamage
        
        when {
            // 敌人攻击玩家的情况
            caster is Monster && target is Player -> {
                // 施法者(敌人)灵力加成：baseDamage += caster.spirit * (baseDamage >> 6)
                damage += caster.lingli * (damage shr 6)
                
                // 目标(玩家)灵力抗性：baseDamage -= target.spirit * (baseDamage >> 6)  
                damage -= target.lingli * (damage shr 6)
                
                // 随机浮动：baseDamage += (randomValue % baseDamage) >> 4
                if (damage > 0) {
                    val randomValue = kotlin.random.Random.nextInt(1000)
                    damage += (randomValue % damage) shr 4
                }
                
                // 特殊减伤检测（暂不实现，需要具体的装备和状态系统）
                // 原代码：if (特殊防护状态) damage -= damage >> 2
                
                // 最终伤害限制
                if (damage > target.hp) {
                    damage = target.hp
                }
            }
            
            // 玩家攻击敌人的情况（逆向计算）
            caster is Player && target is Monster -> {
                // 施法者(玩家)灵力加成
                damage += caster.lingli * (damage shr 6)
                
                // 目标(敌人)灵力抗性
                damage -= target.lingli * (damage shr 6)
                
                // 随机浮动
                if (damage > 0) {
                    val randomValue = kotlin.random.Random.nextInt(1000)
                    damage += (randomValue % damage) shr 4
                }
                
                // 最终伤害限制
                if (damage > target.hp) {
                    damage = target.hp
                }
            }
            
            // 敌人攻击敌人的情况（治疗魔法）
            caster is Monster && target is Monster -> {
                if (damage < 0) {
                    // 治疗魔法：HP恢复计算
                    var healing = abs(damage)
                    
                    // 施法者灵力加成
                    healing += caster.lingli * (healing shr 6)
                    
                    // 目标灵力影响（降低恢复效果）
                    healing -= target.lingli * (healing shr 6)
                    
                    // 随机浮动
                    if (healing > 0) {
                        val randomValue = kotlin.random.Random.nextInt(1000)
                        healing += (randomValue % healing) shr 4
                    }
                    
                    // 最终恢复量限制
                    val maxHealing = target.maxHP - target.hp
                    if (healing > maxHealing) {
                        healing = maxHealing
                    }
                    
                    damage = -healing // 返回负数表示治疗
                }
            }
            
            // 其他情况使用默认计算
            else -> {
                damage += caster.lingli * (damage shr 6) - target.lingli * (damage shr 6)
                if (damage > 0) {
                    val randomValue = kotlin.random.Random.nextInt(1000)
                    damage += (randomValue % damage) shr 4
                }
            }
        }
        
        // 确保最小伤害值
        return max(damage, if (damage > 0) 1 else damage)
    }
    
    /**
     * 计算MP伤害 - 基于C引擎文档
     */
    private fun calcMpHurt(src: FightingCharacter, dst: FightingCharacter, mp: Int): Int {
        if (mp == 0) return 0
        
        if (GameSettings.useOriginalDamageFormula) {
            // 原版C引擎MP伤害公式
            return calcMpDamageOriginal(src, dst, mp)
        } else {
            // 简化公式：使用与HP相同的计算方式
            val add = (src.lingli - dst.lingli).toDouble() / 100
            return max(mp + (mp * add).toInt(), 0)
        }
    }
    
    /**
     * 原版C引擎MP伤害计算
     */
    private fun calcMpDamageOriginal(caster: FightingCharacter, target: FightingCharacter, baseMpDamage: Int): Int {
        // 基础MP伤害 = 法术基础MP伤害
        var mpDamage = baseMpDamage
        
        when {
            // 敌人攻击玩家的MP伤害
            caster is Monster && target is Player -> {
                // 施法者(敌人)灵力影响（减少MP伤害）
                mpDamage -= caster.lingli * (mpDamage shr 6)
                
                // 目标(玩家)灵力影响（增加MP伤害）
                mpDamage += target.lingli * (mpDamage shr 6)
                
                // 随机浮动
                if (mpDamage > 0) {
                    val randomValue = kotlin.random.Random.nextInt(1000)
                    mpDamage += (randomValue % mpDamage) shr 4
                }
                
                // 最终MP伤害限制
                if (mpDamage > target.mp) {
                    mpDamage = target.mp
                }
            }
            
            // 其他情况使用简化计算
            else -> {
                mpDamage -= caster.lingli * (mpDamage shr 6)
                mpDamage += target.lingli * (mpDamage shr 6)
                if (mpDamage > 0) {
                    val randomValue = kotlin.random.Random.nextInt(1000)
                    mpDamage += (randomValue % mpDamage) shr 4
                }
                if (mpDamage > target.mp) {
                    mpDamage = target.mp
                }
            }
        }
        
        return max(mpDamage, 0)
    }

    override fun use(src: FightingCharacter, dst: FightingCharacter) {
        if (GameSettings.allowMiss) {
            if (CalcDamage.randomMiss(src, dst)) {
                dst.missed = true
                return
            }
        }
        src.mp = src.mp - costMp

        debugLog("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━")
        debugLog("【魔法使用】${this.magicName}")
        debugLog("【施法者】${src.name} - 灵力:${src.lingli}, HP:${src.hp}/${src.maxHP}, MP:${src.mp}/${src.maxMP}")
        debugLog("【目标】${dst.name} - 灵力:${dst.lingli}, 等级:${dst.level}, HP:${dst.hp}/${dst.maxHP}")
        debugLog("【魔法数据】mHp基础值:$mHp, mMp基础值:$mMp")

        val hpHurt = calcHurt(src, dst, mHp)
        debugLog("【计算结果】hpHurt = $hpHurt")
        debugLog("【执行前】目标HP: ${dst.hp}")

        dst.hp -= abs(hpHurt)
        debugLog("【执行后】目标HP: ${dst.hp} (变化: ${-abs(hpHurt)})")

        if (hpHurt < 0) {
            val oldHp = src.hp
            src.hp -= hpHurt
            debugLog("【吸血】施法者HP: $oldHp → ${src.hp} (恢复: ${-hpHurt})")
        }

        val mpHurt = calcMpHurt(src, dst, mMp)
        if (mpHurt != 0) {
            debugLog("【MP伤害】mpHurt = $mpHurt")
        }
        dst.mp -= abs(mpHurt)
        if (mpHurt < 0) {
            src.mp -= mpHurt
            debugLog("【吸蓝】施法者MP恢复: ${-mpHurt}")
        }
        debugLog("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━")

        dst.beAttackedWithBuff(buff)
    }

    fun use(src: FightingCharacter, dst: List<FightingCharacter>) {
        src.mp = src.mp - costMp
        val buff = this.buff

        debugLog("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━")
        debugLog("【群体魔法】${this.magicName} - 目标数量:${dst.size}")
        debugLog("【施法者】${src.name} - 灵力:${src.lingli}")

        for ((index, fc) in dst.withIndex()) {
            debugLog("  ▸ 目标${index + 1}: ${fc.name} (HP:${fc.hp}/${fc.maxHP}, 灵力:${fc.lingli}, 等级:${fc.level})")

            if (GameSettings.allowMiss) {
                if (CalcDamage.randomMiss(src, fc)) {
                    fc.missed = true
                    debugLog("    ✗ MISS")
                    continue
                }
            }

            val hpHurt = calcHurt(src, fc, mHp)
            val oldHp = fc.hp
            fc.hp -= abs(hpHurt)
            debugLog("    ✓ 伤害:${abs(hpHurt)}, HP: $oldHp → ${fc.hp}")

            if (hpHurt < 0) {
                val oldSrcHp = src.hp
                src.hp -= hpHurt
                debugLog("    ⚡ 施法者吸血: $oldSrcHp → ${src.hp} (+${-hpHurt})")
            }

            fc.beAttackedWithBuff(buff)
        }
        debugLog("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━")
    }
}
