package fmj.magic

import fmj.characters.FightingCharacter
import kotlin.math.min

/**
 * 04辅助型
 * @author Chen
 */
class MagicAuxiliary : BaseMagic() {

    private var mHp: Int = 0//0~100，表示被施展者恢复生命的百分比（起死回生）

    override fun setOtherData(buf: ByteArray, offset: Int) {
        mHp = get2BytesInt(buf, offset + 0x12)
    }

    override fun use(src: FightingCharacter, dst: FightingCharacter) {
        if (src.mp < costMp) return

        src.mp = src.mp - costMp

        println("MagicAuxiliary.use: 角色 ${dst.name} 使用辅助魔法(起死回生)前 HP: ${dst.hp}/${dst.maxHP}, 恢复百分比: $mHp%")
        
        // 辅助型魔法具有起死回生功能，类似仙药（GoodsMedicineLife）
        // 可以复活阵亡角色，也可以给存活角色加血
        if (dst.hp <= 0) {
            // 死亡的时候使用起死回生魔法
            println("MagicAuxiliary.use: 目标已死，起死回生魔法复活并恢复 $mHp% HP")
            dst.hp = dst.maxHP * mHp / 100
        } else {
            // 活着的时候使用起死回生魔法，添加血量
            println("MagicAuxiliary.use: 目标存活，增加HP ${dst.maxHP * mHp / 100}")
            dst.hp += dst.maxHP * mHp / 100
        }
        
        if (dst.hp > dst.maxHP) {
            dst.hp = dst.maxHP
        }
        // 如果恢复后仍然是0血，至少给1点血确保复活
        if (dst.hp <= 0) {
            dst.hp = 1
        }
        
        println("MagicAuxiliary.use: 角色 ${dst.name} 使用辅助魔法后 HP: ${dst.hp}/${dst.maxHP}")
    }
}
