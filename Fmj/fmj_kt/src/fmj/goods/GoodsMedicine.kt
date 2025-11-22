package fmj.goods

import fmj.characters.FightingCharacter
import fmj.characters.Player
import fmj.characters.health
import fmj.lib.DatLib
import fmj.lib.ResSrs

/**
 * 09药物类
 * 普通药物，任何人都可以用
 * @author Chen
 */
class GoodsMedicine : BaseGoods(), IEatMedicine {

    private var mHp: Int = 0
    private var mMp: Int = 0
    var ani: ResSrs? = null
        private set
    private var mBitMask: Int = 0 // 治疗 毒、乱、封、眠

    override fun setOtherData(buf: ByteArray, offset: Int) {
        mHp = get2BytesInt(buf, offset + 0x16)
        mMp = get2BytesInt(buf, offset + 0x18)
        val index = buf[offset + 0x1a].toInt() and 0xff
        if (index > 0) {
            ani = DatLib.getRes(DatLib.ResType.SRS, 2/*(int)buf[offset + 0x1b] & 0xff*/,
                    index) as ResSrs?
        }
        mBitMask = buf[offset + 0x1c].toInt() and 0xff
    }

    override fun eat(player: Player) {
        println("GoodsMedicine.eat: 角色 ${player.name} 使用药物前 HP: ${player.hp}/${player.maxHP}, 药品HP恢复: $mHp")
        
        // 检查角色是否已经阵亡（HP <= 0 表示阵亡）
        val wasAlive = player.hp > 0
        val wasDead = player.hp <= 0
        
        // 普通药物（GoodsMedicine）不具有复活功能，只能对存活角色使用
        if (wasAlive && mHp > 0) {
            println("GoodsMedicine.eat: 角色存活，增加HP $mHp")
            // 角色活着时，正常增加HP
            player.hp = player.hp + mHp
            if (player.hp > player.maxHP) {
                player.hp = player.maxHP
            }
        } else if (wasDead) {
            println("GoodsMedicine.eat: 角色已死，普通药物无法复活")
            // 普通药物对阵亡角色无效，不做任何处理
        } else {
            println("GoodsMedicine.eat: 药品无HP恢复效果 (mHp=$mHp)")
        }
        
        // MP恢复逻辑（只对存活角色有效）
        if (wasAlive) {
            player.mp = player.mp + mMp
            if (player.mp > player.maxMP) {
                player.mp = player.maxMP
            }
        }
        
        // 状态治疗（只对存活角色有效）
        if (wasAlive) {
            health(mBitMask, player.debuff)
        }
        
        println("GoodsMedicine.eat: 角色 ${player.name} 使用药物后 HP: ${player.hp}/${player.maxHP}")
    }

    /**
     * 是具有全体治疗效果
     * @return
     */
    override fun effectAll(): Boolean {
        return mBitMask and 0x10 != 0
    }
}
