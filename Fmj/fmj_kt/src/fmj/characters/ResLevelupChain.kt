package fmj.characters

import fmj.lib.ResBase

import java.System
import kotlin.math.pow

class ResLevelupChain : ResBase() {

    var maxLevel: Int = 0
        private set // 最高级别

    private var mLevelData = ByteArray(maxLevel * LEVEL_BYTES) // 级别数据

    override fun setData(buf: ByteArray, offset: Int) {
        type = buf[offset].toInt() and 0xff
        index = buf[offset + 1].toInt() and 0xff
        maxLevel = buf[offset + 2].toInt() and 0xff
        if (maxLevel <= 0) {
            maxLevel = 99
        }
        mLevelData = ByteArray(maxLevel * LEVEL_BYTES)

        System.arraycopy(buf, offset + 4, mLevelData, 0, mLevelData.size)
    }

    fun getMaxHP(level: Int): Int {
        return if (level <= maxLevel) {
            get2BytesInt(mLevelData, level * LEVEL_BYTES - LEVEL_BYTES)
        } else 0
    }

    fun getHP(level: Int): Int {
        return if (level <= maxLevel) {
            get2BytesInt(mLevelData, 2 + level * LEVEL_BYTES - LEVEL_BYTES)
        } else 0
    }

    fun getMaxMP(l: Int): Int {
        return if (l <= maxLevel) {
            get2BytesInt(mLevelData, 4 + l * LEVEL_BYTES - LEVEL_BYTES)
        } else 0
    }

    fun getMP(l: Int): Int {
        return if (l <= maxLevel) {
            get2BytesInt(mLevelData, 6 + l * LEVEL_BYTES - LEVEL_BYTES)
        } else 0
    }

    fun getAttack(l: Int): Int {
        return if (l <= maxLevel) {
            get2BytesInt(mLevelData, 8 + l * LEVEL_BYTES - LEVEL_BYTES)
        } else 0
    }

    fun getDefend(l: Int): Int {
        return if (l <= maxLevel) {
            get2BytesInt(mLevelData, 10 + l * LEVEL_BYTES - LEVEL_BYTES)
        } else 0
    }

    fun getNextLevelExp(l: Int): Int {
        return if (l <= maxLevel) {
            val exp = get2BytesInt(mLevelData, 14 + l * LEVEL_BYTES - LEVEL_BYTES)
            // 如果读取的经验值无效（小于等于0），使用指数增长算法
            if (exp <= 0) {
                // 基础值100，每级增长50%，确保后期需要更多经验
                val baseExp = 100
                val growth = 1.5
                (baseExp * growth.pow(l.toDouble())).toInt()
                // 0级→1级: 100, 1级→2级: 150, 2级→3级: 225, 10级→11级: 5766
            } else {
                exp
            }
        } else 0
    }

    fun getSpeed(l: Int): Int {
        return if (l <= maxLevel) {
            mLevelData[l * LEVEL_BYTES - LEVEL_BYTES + 16].toInt() and 0xff
        } else 0
    }

    fun getLingli(l: Int): Int {
        return if (l <= maxLevel) {
            mLevelData[l * LEVEL_BYTES - LEVEL_BYTES + 17].toInt() and 0xff
        } else 0
    }

    fun getLuck(l: Int): Int {
        return if (l <= maxLevel) {
            mLevelData[l * LEVEL_BYTES - LEVEL_BYTES + 18].toInt() and 0xff
        } else 0
    }

    fun getLearnMagicNum(l: Int): Int {
        return if (l <= maxLevel) {
            mLevelData[l * LEVEL_BYTES - LEVEL_BYTES + 19].toInt() and 0xff
        } else 0
    }

    companion object {
        private val LEVEL_BYTES = 20 // 一个级别数据所占字节数
    }
}
