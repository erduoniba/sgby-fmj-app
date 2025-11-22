package fmj.magic

import fmj.characters.BuffMan
import fmj.characters.FightingCharacter

/**
 * 02增强型
 * @author Chen
 */
class MagicEnhance : BaseMagic() {

//    private var mDf: Int = 0//0~100，被施展者的防御力增强的百分比
//    private var mAt: Int = 0//0~100，被施展者的攻击力增强的百分比
//    private var mRound: Int = 0//持续回合
//    private var mBuff: Int = 0//速 0~100，被施展者的身法加快的百分比
    private val buff = BuffMan()

    override fun setOtherData(buf: ByteArray, offset: Int) {
        val at = buff.getBuffs(FightingCharacter.BUFF_MASK_GONG).first()
        val df = buff.getBuffs(FightingCharacter.BUFF_MASK_FANG).first()
        val v = buff.getBuffs(FightingCharacter.BUFF_MASK_SU).first()

        at.value = -(buf[offset + 0x17].toInt() and 0xff)
        df.value = -(buf[offset + 0x16].toInt() and 0xff)
        v.value = -(buf[offset + 0x19].toInt() and 0xff)

        val round = (buf[offset + 0x18].toInt() shr 4) and 0xf
        arrayOf(at, df, v).forEach { it.round = round }
    }

    override fun use(src: FightingCharacter, dst: FightingCharacter) {
        dst.beAttackedWithBuff(buff, 0)
    }
}
