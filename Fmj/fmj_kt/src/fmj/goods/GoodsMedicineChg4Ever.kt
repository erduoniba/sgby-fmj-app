package fmj.goods

import fmj.characters.Player

/**
 * 11仙药类
 * 永久性改变人物属性
 * @author Chen
 */
class GoodsMedicineChg4Ever : BaseGoods(), IEatMedicine {

    private var mMpMax: Int = 0
    private var mHpMax: Int = 0
    private var mdf: Int = 0
    private var mat: Int = 0
    private var mling: Int = 0
    private var mSpeed: Int = 0
    private var mLuck: Int = 0

    override fun setOtherData(buf: ByteArray, offset: Int) {
        mMpMax = get1ByteSInt(buf, offset + 0x16)
        mHpMax = get1ByteSInt(buf, offset + 0x17)
        mdf = buf[offset + 0x18].toInt() and 0xff
        mat = buf[offset + 0x19].toInt() and 0xff
        mling = get1ByteSInt(buf, offset + 0x1a)
        mSpeed = get1ByteSInt(buf, offset + 0x1b)
        mLuck = get1ByteSInt(buf, offset + 0x1d)
    }

    override fun eat(player: Player) {
        // 更新不受限制的总属性值
        player.totalMaxMP += mMpMax
        player.totalMaxHP += mHpMax
        player.totalDefend += mdf
        player.totalAttack += mat
        player.totalLingli += mling
        player.totalSpeed += mSpeed
        player.totalLuck += mLuck

        // 应用到实际属性（受限制）
        player.maxMP = player.totalMaxMP
        player.maxHP = player.totalMaxHP
        player.defend = player.totalDefend
        player.attack = player.totalAttack
        player.lingli = player.totalLingli
        player.speed = player.totalSpeed
        player.luck = player.totalLuck
        // 物品删除由调用方统一处理，此处不需要重复删除
    }
}
