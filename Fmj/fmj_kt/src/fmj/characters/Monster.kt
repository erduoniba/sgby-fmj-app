package fmj.characters

import fmj.Global
import fmj.goods.BaseGoods
import fmj.lib.DatLib
import java.random

class Monster : FightingCharacter() {
    var mIQ: Int = 0 // 智商，影响魔法使用率
    /**
     * 打怪得到的金钱
     * @return
     */
    var money: Int = 0
        private set // 打挂怪物掉的钱
    /**
     * 打怪得到的经验
     * @return
     */
    var exp: Int = 0
        private set // 打挂怪物得到的经验
    private val mCarryGoods1 = IntArray(3) // 能被偷走的 type id num
    private val mCarryGoods2 = IntArray(3) // 打挂掉落的 type id num

    /**
     * 怪物掉落的物品
     * @return
     */
    val dropGoods: BaseGoods?
        get() {
            if (mCarryGoods2[0] == 0 || mCarryGoods2[1] == 0 || mCarryGoods2[2] == 0) {
                return null
            }
            val res = DatLib.Companion.getRes(DatLib.ResType.GRS, mCarryGoods2[0], mCarryGoods2[1])
            if (res !is BaseGoods) {
                println("Warning: Monster dropGoods type mismatch - expected BaseGoods but got ${res?.let { it::class.simpleName }}")
                return null
            }
            val g = res
            g.goodsNum = mCarryGoods2[2]
            return g
        }

    fun stealGoods(attacker: FightingCharacter): BaseGoods? {
        if (mCarryGoods1[0] == 0 || mCarryGoods1[1] == 0 || mCarryGoods1[2] == 0) {
            return null
        }
        if (random() < 0.2) {
            return null
        }
        mCarryGoods1[2] -= 1
        val res = DatLib.getRes(DatLib.ResType.GRS, mCarryGoods1[0], mCarryGoods1[1])
        if (res !is BaseGoods) {
            println("Warning: Monster stealGoods type mismatch - expected BaseGoods but got ${res?.let { it::class.simpleName }}")
            return null
        }
        return res
    }

    override fun setData(buf: ByteArray, offset: Int) {
        type = buf[offset].toInt() and 0xff
        index = buf[offset + 1].toInt() and 0xff
        val magicIndex = buf[offset + 0x2f].toInt() and 0xff

        magicChain = DatLib.getMlr(1, magicIndex, true)
        magicChain?.learnNum = buf[offset + 2].toInt() and 0xff
        addBuff(buf[offset + 3].toInt() and 0xff)
        val round = buf[offset + 0x17].toInt() and 0xff
        atbuff.addBuff(buf[offset + 4].toInt() and 0xff, round)
        name = getString(buf, offset + 6)
        level = buf[offset + 0x12].toInt() and 0xff
        maxHP = get2BytesInt(buf, offset + 0x18)
        hp = get2BytesInt(buf, offset + 0x1a)
        maxMP = get2BytesInt(buf, offset + 0x1c)
        mp = get2BytesInt(buf, offset + 0x1e)
        attack = get2BytesInt(buf, offset + 0x20)
        defend = get2BytesInt(buf, offset + 0x22)
        speed = buf[offset + 0x13].toInt() and 0xff
        lingli = buf[offset + 0x14].toInt() and 0xff
        luck = buf[offset + 0x16].toInt() and 0xff
        mIQ = buf[offset + 0x15].toInt() and 0xff
        money = get2BytesInt(buf, offset + 0x24)
        exp = get2BytesInt(buf, offset + 0x26)
        mCarryGoods1[0] = buf[offset + 0x28].toInt() and 0xff
        mCarryGoods1[1] = buf[offset + 0x29].toInt() and 0xff
        mCarryGoods1[2] = buf[offset + 0x2a].toInt() and 0xff
        mCarryGoods2[0] = buf[offset + 0x2b].toInt() and 0xff
        mCarryGoods2[1] = buf[offset + 0x2c].toInt() and 0xff
        mCarryGoods2[2] = buf[offset + 0x2d].toInt() and 0xff
        fightingSprite = FightingSprite(DatLib.ResType.ACP, buf[offset + 0x2e].toInt() and 0xff)
    }

    /**
     *
     * @param i 屏幕上的位置
     */
    fun setOriginalCombatPos(i: Int) {
        val fs = fightingSprite
        // arr array only has 3 positions, ensure we don't access out of bounds
        val posIndex = if (i < arr.size) i else arr.size - 1
        fs?.setCombatPos(arr[posIndex][0] - fs.width / 6 + fs.width / 2,
                arr[posIndex][1] - fs.height / 10 + fs.height / 2)
    }

    companion object {
        val arr = arrayOf(
            intArrayOf(12 + (Global.SCREEN_WIDTH - 160) / 2 - 30, 25 + (Global.SCREEN_HEIGHT - 96) / 2 - 30),
            intArrayOf(44 + (Global.SCREEN_WIDTH - 160) / 2 - 30, 14 + (Global.SCREEN_HEIGHT - 96) / 2 - 30), 
            intArrayOf(82 + (Global.SCREEN_WIDTH - 160) / 2 - 30, 11 + (Global.SCREEN_HEIGHT - 96) / 2 - 30)
        )
    }
}
