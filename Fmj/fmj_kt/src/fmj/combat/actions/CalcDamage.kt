package fmj.combat.actions

import fmj.characters.FightingCharacter
import fmj.characters.Player
import fmj.characters.Monster
import fmj.config.GameSettings
import fmj.scene.SaveLoadGame
import java.random
import kotlin.math.max

object CalcDamage {

    /**
     * 计算基础物理伤害 - 支持原版/简化公式切换
     * 原版公式有不同的防御计算：
     * - 对玩家：damage = attack / ((defense >> 2) + 1) + random % ((attack >> 4) + 1)
     * - 对敌人：damage = attack / ((defense >> 3) + 1) + random % ((attack >> 2) + 1)
     * 简化公式：damage = max(attack - defense, 0) + random * 10
     */
    fun calcBaseDamage(attack: Int, defense: Int, targetIsPlayer: Boolean = false): Int {
        return if (GameSettings.useOriginalDamageFormula) {
            // 原版公式：根据目标类型使用不同的防御计算
            val baseDamage = if (targetIsPlayer) {
                // 玩家受伤：defense >> 2 (防御效果较低)
                attack / ((defense shr 2) + 1)
            } else {
                // 敌人受伤：defense >> 3 (防御效果较高)
                attack / ((defense shr 3) + 1)
            }

            val randomDamage = if (attack > 0) {
                if (targetIsPlayer) {
                    // 对玩家：attack >> 4 (较小的随机波动)
                    (random() * ((attack shr 4) + 1)).toInt()
                } else {
                    // 对敌人：attack >> 2 (较大的随机波动)
                    (random() * ((attack shr 2) + 1)).toInt()
                }
            } else {
                0
            }
            max(baseDamage + randomDamage, 1)
        } else {
            // 简化公式
            var damage = max(attack - defense, 0)
            if (damage <= 0) {
                damage = 1
            }
            damage + (random() * 10).toInt()
        }
    }

    /**
     * 计算魔法伤害给玩家 - 支持原版/简化公式切换
     * 原公式：damage = magicDamage / ((playerDefense >> 2) + 1) + random % ((magicDamage >> 4) + 1)
     */
    fun calcMagicDamageToPlayer(magicDamage: Int, playerDefense: Int): Int {
        return if (GameSettings.useOriginalDamageFormula) {
            // 原版公式：对玩家防御力 >> 2
            val baseDamage = magicDamage / ((playerDefense shr 2) + 1)
            val randomDamage = if (magicDamage > 0) {
                (random() * ((magicDamage shr 4) + 1)).toInt()
            } else {
                0
            }
            max(baseDamage + randomDamage, 1)
        } else {
            // 简化公式
            max(magicDamage, 1)
        }
    }

    /**
     * 计算魔法伤害给敌人 - 支持原版/简化公式切换  
     * 原公式：damage = magicDamage / ((enemyDefense >> 3) + 1) + random % ((magicDamage >> 2) + 1)
     */
    fun calcMagicDamageToEnemy(magicDamage: Int, enemyDefense: Int): Int {
        return if (GameSettings.useOriginalDamageFormula) {
            // 原版公式：对敌人防御力 >> 3，随机部分 >> 2
            val baseDamage = magicDamage / ((enemyDefense shr 3) + 1)
            val randomDamage = if (magicDamage > 0) {
                (random() * ((magicDamage shr 2) + 1)).toInt()
            } else {
                0
            }
            max(baseDamage + randomDamage, 1)
        } else {
            // 简化公式
            max(magicDamage, 1)
        }
    }

    /**
     * 计算命中率 - 基于原版公式
     * 原公式：身法差值 >= random % 200 则命中
     */
    fun randomMiss(attacker: FightingCharacter, defender: FightingCharacter): Boolean {
        // 如果禁用了 Miss 功能，所有攻击都会命中（不会 miss）
        if (!GameSettings.allowMiss) {
            return false // 强制命中，不会 miss
        }
        
        val attackerAgility: Int
        val defenderAgility: Int
        
        when {
            // 敌人攻击玩家
            attacker is Monster && defender is Player -> {
                // 玩家身法 = 玩家身法 + 50（基础加成）
                defenderAgility = defender.computedSpeed + 50
                attackerAgility = attacker.computedSpeed
            }
            // 玩家攻击敌人 
            attacker is Player && defender is Monster -> {
                // 玩家身法 = 玩家身法 + 50（基础加成）
                attackerAgility = attacker.computedSpeed + 50
                defenderAgility = defender.computedSpeed
            }
            // 其他情况使用默认计算
            else -> {
                attackerAgility = attacker.computedSpeed
                defenderAgility = defender.computedSpeed
            }
        }
        
        // 计算身法差值
        val agilityDiff = if (attackerAgility > defenderAgility) {
            attackerAgility - defenderAgility
        } else {
            10 // 最小命中率保底
        }
        
        // 命中检测：random % 200 < agilityDiff
        val hitRoll = (random() * 200).toInt()
        return hitRoll >= agilityDiff // true = miss, false = hit
    }
}
