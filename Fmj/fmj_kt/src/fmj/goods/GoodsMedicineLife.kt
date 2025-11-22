package fmj.goods

import fmj.characters.Player

/**
 * 10灵药类
 * 对生命的恢复0~100,表示恢复被使用者??%的生命，
 * 并解除死亡状态，但被使用者必须是死亡状态。
 * @author Chen
 */
class GoodsMedicineLife : BaseGoods(), IEatMedicine {

    private var mPercent: Int = 0 // 恢复百分比

    override fun setOtherData(buf: ByteArray, offset: Int) {
        mPercent = buf[offset + 0x17].toInt() and 0xff
        if (mPercent > 100) {
            mPercent = 100
        }
    }

    override fun eat(player: Player) {
        println("GoodsMedicineLife.eat: 角色 ${player.name} 使用灵药前 HP: ${player.hp}/${player.maxHP}, 恢复百分比: $mPercent%")
        
        // 根据注释，这是复活药品，恢复HP百分比并解除死亡状态
        // 复活时恢复指定百分比的生命值
        // 死亡的时候吃仙药
        if (player.hp <= 0) {
            player.hp = player.maxHP * mPercent / 100
        }
        else {
            // 活着的时候吃仙药，添加血量
            player.hp += player.maxHP * mPercent / 100
        }
        
        if (player.hp > player.maxHP) {
            player.hp = player.maxHP
        }
        // 如果恢复后仍然是0血，至少给1点血确保复活
        if (player.hp <= 0) {
            player.hp = 1
        }
        
        if (player.mp > player.maxMP) {
            player.mp = player.maxMP
        }
        
        println("GoodsMedicineLife.eat: 角色 ${player.name} 使用灵药后 HP: ${player.hp}/${player.maxHP}")
        // 物品删除由调用方统一处理，此处不需要重复删除
    }
}
