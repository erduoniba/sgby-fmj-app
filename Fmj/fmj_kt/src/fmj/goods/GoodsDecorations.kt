package fmj.goods

import fmj.characters.FightingCharacter
import fmj.characters.Player
import fmj.lib.DatLib
import fmj.magic.MagicAttack

/**
 * 06饰品类
 * @author Chen
 */
class GoodsDecorations : GoodsEquipment() {

    private var mMp: Int = 0 // 表示战斗时，每回合恢复或扣除多少真气
    private var mHp: Int = 0 // 表示战斗时，每回合恢复或扣除多少生命
    private var mMagic: Int = 0 // 合体魔方序号

    val coopMagic: MagicAttack?
        get() = if (mMagic > 0) {
            val res = DatLib.getRes(DatLib.ResType.MRS, 1, mMagic, true)
            if (res is MagicAttack) res else null
        } else {
            null
        }

    override fun setOtherData(buf: ByteArray, offset: Int) {
        mMp = get1ByteSInt(buf, offset + 0x16)
        mHp = get1ByteSInt(buf, offset + 0x17)
        mdf = buf[offset + 0x18].toInt() and 0xff
        mat = buf[offset + 0x19].toInt() and 0xff
        mlingli = get1ByteSInt(buf, offset + 0x1a)
        mSpeed = get1ByteSInt(buf, offset + 0x1b)
        mMagic = buf[offset + 0x1c].toInt() and 0xff
        mLuck = get1ByteSInt(buf, offset + 0x1d)
    }

    override fun putOn(p: Player) {
        super.putOn(p)
    }

    override fun takeOff(p: Player) {
        super.takeOff(p)
    }

    override fun affect(fighter: FightingCharacter) {
        fighter.hp += mHp
        fighter.mp += mMp
    }
}
