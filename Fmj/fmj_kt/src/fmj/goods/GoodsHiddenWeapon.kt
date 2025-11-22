package fmj.goods

import fmj.characters.BuffMan
import fmj.lib.DatLib
import fmj.lib.ResBase
import fmj.lib.ResSrs

/**
 * 08暗器
 * @author Chen
 */
class GoodsHiddenWeapon : BaseGoods(), Throwable {

    /**
     *
     * @return 当该值为正时表示敌人损失多少生命，
     * 为负时表示从敌人身上吸取多少生命到投掷者身上
     */
    override var affectHp: Int = 0
        private set // 当该值为正时表示敌人损失多少生命，为负时表示从敌人身上吸取多少生命到投掷者身上
    /**
     *
     * @return 当该值为正时表示敌人损失多少真气，
     * 为负时表示从敌人身上吸取多少真气到投掷者身上
     */
    override var affectMp: Int = 0
        private set // 当该值为正时表示敌人损失多少真气，为负时表示从敌人身上吸取多少真气到投掷者身上
    override var ani: ResSrs = ResSrs()
        private set
    private var mBitMask: Int = 0 // 000 全体否 毒乱封眠

    override var buff = BuffMan()

    override fun setOtherData(buf: ByteArray, offset: Int) {
        affectHp = ResBase.get2BytesSInt(buf, offset + 0x16)
        affectMp = ResBase.get2BytesSInt(buf, offset + 0x18)
        val type = buf[offset + 0x1b].toInt() and 0xff
        val index = buf[offset + 0x1a].toInt() and 0xff
        ani = DatLib.getRes(DatLib.ResType.SRS, type, index) as ResSrs
        mBitMask = buf[offset + 0x1c].toInt() and 0xff
        buff = BuffMan.fromRoundAndMask(sumRound, mBitMask)
    }

    override fun effectAll(): Boolean {
        return mBitMask and 0x10 != 0
    }
}
