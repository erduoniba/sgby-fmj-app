package fmj.goods

import fmj.characters.BuffMan
import fmj.characters.FightingCharacter
import fmj.lib.DatLib
import fmj.lib.ResBase
import fmj.lib.ResImage
import fmj.lib.ResSrs

import graphics.Canvas

abstract class BaseGoods : ResBase() {

    /**
     * 是否可装备，最低位为主角1
     */
    private var mEnable: Int = 0

    var sumRound: Int = 0
        private set // 持续回合

    private var mImage: ResImage? = null // 物品图片

    var name = ""
        private set // 道具名称

    var buyPrice: Int = 0
        private set
    var sellPrice: Int = 0
        private set // 买价、卖价

    var description = ""
        private set // 道具说明

    /**
     * 不为0时装备该道具时，就会设置该事件，而卸下时，
     * 就会取消该事件，不能用来典当。
     */
    var eventId: Int = 0
        private set

    /**
     * 物品数量
     */
    var goodsNum = 0

    protected abstract fun setOtherData(buf: ByteArray, offset: Int)

    override fun setData(buf: ByteArray, offset: Int) {
        type = buf[offset].toInt() and 0xFF
        index = buf[offset + 1].toInt() and 0xFF
        mEnable = buf[offset + 3].toInt() and 0xFF
        sumRound = buf[offset + 4].toInt() and 0xff
        val imageRes = DatLib.Companion.getRes(DatLib.ResType.GDP, type, buf[offset + 5].toInt() and 0xff, true)
        mImage = if (imageRes is ResImage) {
            imageRes
        } else {
            println("Warning: Failed to load goods image for type=$type, index=${buf[offset + 5].toInt() and 0xff}")
            null
        }
        name = ResBase.getString(buf, offset + 6)
        buyPrice = ResBase.get2BytesInt(buf, offset + 0x12)
        sellPrice = ResBase.get2BytesInt(buf, offset + 0x14)
        description = ResBase.getString(buf, offset + 0x1e)
        eventId = ResBase.get2BytesInt(buf, offset + 0x84)
        setOtherData(buf, offset)
    }

    /**
     *
     * @param playId 1-4
     * @return
     */
    fun canPlayerUse(playId: Int): Boolean {
        return if (playId in 1..4) {
            mEnable and (1 shl playId - 1) != 0
        } else false
    }

    fun draw(canvas: Canvas, x: Int, y: Int) {
        mImage?.draw(canvas, 1, x, y)
    }

    /**
     * 增加物品数量
     * @param d 增量
     */
    fun addGoodsNum(d: Int) {
        goodsNum += d
    }

    /** 是否具有全体效果 */
    open fun effectAll(): Boolean {
        return false
    }

    /**
     * 比较物品编号是否相等
     */
    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (other !is BaseGoods) return false
        return type == other.type && index == other.index
    }

    override fun hashCode(): Int {
        var result = type
        result = 31 * result + index
        return result
    }
}

interface Throwable {
    val ani: ResSrs
    /**
     *
     * @return 当该值为正时表示敌人损失多少生命，
     * 为负时表示从敌人身上吸取多少生命到投掷者身上
     */
    val affectHp: Int
    /**
     *
     * @return 当该值为正时表示敌人损失多少真气，
     * 为负时表示从敌人身上吸取多少真气到投掷者身上
     */
    val affectMp: Int
    val buff: BuffMan

    fun attack(other: FightingCharacter) {
        other.hp -= affectHp
        other.mp -= affectMp
        other.beAttackedWithBuff(buff, 0)
    }
}

