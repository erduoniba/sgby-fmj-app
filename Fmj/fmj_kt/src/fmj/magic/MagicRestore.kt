package fmj.magic

import fmj.characters.FightingCharacter
import fmj.characters.health

/**
 * 03恢复型
 * @author Chen
 */
class MagicRestore : BaseMagic() {

    private var mHp: Int = 0 //  0~8000，表示被施展者恢复生命的数值。
    private var mBuff: Int = 0 // 低四位，毒、乱、封、眠 是否具有医疗相应异常状态的能力

    override fun setOtherData(buf: ByteArray, offset: Int) {
        mHp = Companion.get2BytesInt(buf, offset + 0x12)
        mBuff = buf[offset + 0x18].toInt()
    }

    override fun use(src: FightingCharacter, dst: FightingCharacter) {
        if (src.mp < costMp) return

        src.mp = src.mp - costMp
        applyEffect(src, dst)
    }
    
    /**
     * 应用恢复效果但不消耗MP（用于群体魔法）
     */
    fun applyEffect(src: FightingCharacter, dst: FightingCharacter) {
        println("MagicRestore.applyEffect: 角色 ${dst.name} 使用恢复魔法前 HP: ${dst.hp}/${dst.maxHP}, 魔法HP恢复: $mHp")
        
        // 检查目标角色是否已经阵亡（HP <= 0 表示阵亡）
        val wasAlive = dst.hp > 0
        val wasDead = dst.hp <= 0
        
        // 恢复型魔法（MagicRestore）不具有复活功能，只能对存活角色使用，类似普通药物（GoodsMedicine）
        if (wasAlive && mHp > 0) {
            println("MagicRestore.applyEffect: 角色存活，增加HP $mHp")
            // 角色活着时，正常增加HP
            dst.hp = dst.hp + mHp
            if (dst.hp > dst.maxHP) {
                dst.hp = dst.maxHP
            }
        } else if (wasDead) {
            println("MagicRestore.applyEffect: 角色已死，普通恢复魔法无法复活")
            // 普通恢复魔法对阵亡角色无效，不做任何处理
        } else {
            println("MagicRestore.applyEffect: 魔法无HP恢复效果 (mHp=$mHp)")
        }
        
        // 状态治疗（只对存活角色有效）
        if (wasAlive) {
            health(mBuff, dst.debuff)
        }
        
        println("MagicRestore.use: 角色 ${dst.name} 使用恢复魔法后 HP: ${dst.hp}/${dst.maxHP}")
    }
}
