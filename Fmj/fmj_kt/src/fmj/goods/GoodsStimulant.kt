package fmj.goods

/**
 * 12兴奋剂
 * @author Chen
 */
class GoodsStimulant : BaseGoods() {

    private var mdfPercent: Int = 0
    private var matPercent: Int = 0
    private var mSpeedPercent: Int = 0
    private var mForAll: Boolean = false

    override fun setOtherData(buf: ByteArray, offset: Int) {
        mdfPercent = buf[offset + 0x18].toInt() and 0xff
        matPercent = buf[offset + 0x19].toInt() and 0xff
        mSpeedPercent = buf[offset + 0x1b].toInt() and 0xff
        mForAll = buf[offset + 0x1c].toInt() and 0x10 != 0
    }

    override fun effectAll(): Boolean {
        return mForAll
    }
}
