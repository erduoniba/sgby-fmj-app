package fmj.magic

import fmj.lib.DatLib
import fmj.lib.ResBase
import kotlin.math.min
import java.sysGetMagicReverse
import kotlin.math.max

/**
 * 魔法链资源
 * @author Chen
 */
class ResMagicChain : ResBase() {

    /**
     * 得到魔法总数
     * @return
     */
    private var magicSum: Int = 0

    private var mMagics: List<BaseMagic> = listOf()

    /**
     * 返回已经学会的魔法数量
     * @return
     */
    var learnNum = 0 // 学会的魔法数量

    override fun setData(buf: ByteArray, offset: Int) {
        type = buf[offset].toInt() and 0xff
        index = buf[offset + 1].toInt() and 0xff
        magicSum = buf[offset + 2].toInt() and 0xff

        var index = offset + 3
        mMagics = List(magicSum) {
            val magicType = buf[index++].toInt()
            val magicIndex = buf[index++].toInt()
            DatLib.getMrsOrNull(magicType, magicIndex) ?: run {
                println("Warning: Failed to load magic in chain for type=$magicType, index=$magicIndex")
                // 返回一个默认的魔法对象以防止崩溃
                DatLib.getMrs(1, 1) // 使用基础攻击魔法作为默认值
            }
        }
    }

    fun getMagic(index: Int): BaseMagic {
        if (index < 0 || index >= mMagics.size) {
            println("Warning: Magic index $index out of bounds (size: ${mMagics.size})")
            return DatLib.getMrs(1, 1)
        }
        return mMagics[index]
    }

    fun getMagicCount(): Int {
        return mMagics.size
    }

    fun getAllLearntMagics(): Collection<BaseMagic> {
        return getAllLearntMagics(false) // Default behavior for Player
    }
    
    fun getAllLearntMagics(isMonster: Boolean): Collection<BaseMagic> {
        // 安全检查：如果魔法列表为空或未初始化，返回空列表
        if (mMagics.isEmpty() || magicSum == 0) {
            return emptyList()
        }
        
        if (learnNum == 0) {
            if (isMonster) {
                return emptyList() // Monster with no learned magic should return empty
            } else {
                learnNum = mMagics.size // Player behavior: learn all available magic
            }
        }
        
        // 安全的范围检查
        val actualLearnNum = min(learnNum, min(magicSum, mMagics.size))
        if (actualLearnNum <= 0) {
            return emptyList()
        }
        
        val magicss = mMagics.slice(0 until actualLearnNum)
        val magicReverse = sysGetMagicReverse()
        if (magicReverse) {
            return magicss.reversed()
        }
        return magicss
    }
}
