package fmj.lib

import fmj.characters.Monster
import fmj.characters.NPC
import fmj.characters.Player
import fmj.characters.ResLevelupChain
import fmj.characters.SceneObj
import fmj.goods.BaseGoods
import fmj.goods.GoodsDecorations
import fmj.goods.GoodsDrama
import fmj.goods.GoodsEquipment
import fmj.goods.GoodsHiddenWeapon
import fmj.goods.GoodsMedicine
import fmj.goods.GoodsMedicineChg4Ever
import fmj.goods.GoodsMedicineLife
import fmj.goods.GoodsStimulant
import fmj.goods.GoodsTudun
import fmj.goods.GoodsWeapon
import fmj.graphics.TextRender
import fmj.magic.*
import fmj.script.ScriptVM
import graphics.Bitmap
import graphics.Canvas
import java.File
import java.sysGetChoiceLibName


class DatLib(buffer: ByteArray) {

    /**
     * DAT.LIB文件的所有内容
     */
    private var mBuffer = buffer

    /**
     * 保存资源数据相对文件首字节的偏移量
     */
    private val mDataOffset = HashMap<Int, Int>(2048)

    data class Res(val type: Int, val index: Int)
    private val guts = arrayListOf<Res>()

    init {
        getAllResOffset()
    }

    fun tryCompileScripts(vm: ScriptVM) {
        println("Trying to compile all guts")
        guts.forEach {
            vm.compileScript(it.type, it.index)
        }
        println("All guts compile OK")
    }

    private fun getAllResOffset() {
        var i = 0x10
        var j = 0x2000

        while (i < mBuffer.size && mBuffer[i].toInt() != -1) {
            val resType = mBuffer[i++].toInt()
            val type = mBuffer[i++].toInt() and 0xFF
            val index = mBuffer[i++].toInt() and 0xFF
            val key = getKey(resType, type, index)
            val block = mBuffer[j++].toInt() and 0xFF
            val low = mBuffer[j++].toInt() and 0xFF
            val high = mBuffer[j++].toInt() and 0xFF
            val value = block * 0x4000 or (high shl 8 or low)
            
            // 防护性检查：确保计算出的偏移量在有效范围内
            if (value < 0 || value >= mBuffer.size - 6) {
                continue
            }
            
            // 检查数据是否损坏
            if (value + 6 <= mBuffer.size) {
                val testWidth = mBuffer[value + 2].toInt() and 0xFF
                val testHeight = mBuffer[value + 3].toInt() and 0xFF
                val testNumber = mBuffer[value + 4].toInt() and 0xFF
                
                // 检查多种损坏模式
                val isCorrupted = when {
                    // 异常大的尺寸（超出合理范围）
                    testWidth > 250 && testHeight > 250 && testNumber > 250 -> true
                    else -> false
                }
                
                if (isCorrupted) {
                    continue
                }
            }
            
            mDataOffset.put(key, value)
            if (resType == 1) {
                guts.add(Res(type, index))
            }
        }
    }

    /**
     *
     * @param resType
     * 资源文件类型号1-12
     * @param type
     * 资源类型
     * @param index
     * 资源索引号
     * @return 资源对象，不存在则返回`null`
     */
    fun getRes(resType: ResType, type: Int, index: Int, allowNull: Boolean = false): ResBase? {
        val offset = getDataOffset(resType, type, index)
        var res = (if (offset != -1) {
            val res: ResBase? =
                    when (resType) {
                        ResType.GUT -> ResGut()

                        ResType.MAP -> ResMap()

                        ResType.ARS -> when (type) {
                            1 // 玩家角色
                            -> Player()

                            2 // NPC角色
                            -> NPC()

                            3 // 敌人角色
                            -> Monster()

                            4 // 场景对象
                            -> SceneObj()

                            else -> null
                        }

                        ResType.MRS -> getMagic(type)

                        ResType.SRS -> ResSrs()

                        ResType.GRS -> getGoods(type)

                        ResType.TIL, ResType.ACP, ResType.GDP, ResType.GGJ, ResType.PIC -> ResImage()

                        ResType.MLR -> when(type) {
                            1 -> ResMagicChain()
                            2 -> ResLevelupChain()
                            else -> null
                        }
                    }
            res?.setData(mBuffer, offset)
            res
        } else {
            // 资源不存在
            null
        })
        if (allowNull) return res
        if (res == null) {
            res = when (resType) {
                ResType.SRS -> ResSrs()
                ResType.TIL, ResType.ACP, ResType.GDP, ResType.GGJ, ResType.PIC -> ResImage()
                ResType.MLR -> when(type) {
                    1 -> ResMagicChain()
                    2 -> ResLevelupChain()
                    else -> null
                }
                else -> null
            }
        }
        if (res == null) {
            // Log warning for debugging but return a default resource instead of crashing
            println("WARNING: res not found:resType=$resType,type=$type,index=$index")
            
            // Return an empty default resource based on type to prevent crashes
            when (resType) {
                ResType.MRS -> {
                    // For invalid magic resources, return a basic MagicAttack with minimal properties
                    if (type == 0 || index == 0) {
                        return null // Type 0 or index 0 is intentionally null for MRS
                    }
                }
                else -> {}
            }
        }
        
        if (res == null) {
            // 对于图像资源，返回空占位符而不是崩溃
            when (resType) {
                ResType.TIL, ResType.ACP, ResType.GDP, ResType.GGJ, ResType.PIC -> {
                    val emptyImg = ResImage()
                    val emptyData = ByteArray(6)
                    emptyData[0] = type.toByte()
                    emptyData[1] = index.toByte()
                    emptyImg.setData(emptyData, 0)
                    return emptyImg
                }
                else -> return null
            }
        }
        
        return res
    }

    private fun getGoods(type: Int): BaseGoods? {
        if (type in 1..5) {
            return GoodsEquipment()
        }
        var rtn: BaseGoods? = null
        when (type) {
            6 -> rtn = GoodsDecorations()

            7 -> rtn = GoodsWeapon()

            8 -> rtn = GoodsHiddenWeapon()

            9 -> rtn = GoodsMedicine()

            10 -> rtn = GoodsMedicineLife()

            11 -> rtn = GoodsMedicineChg4Ever()

            12 -> rtn = GoodsStimulant()

            13 -> rtn = GoodsTudun()

            14 -> rtn = GoodsDrama()
        }
        return rtn
    }

    private fun getMagic(type: Int): ResBase? {
        when (type) {
            0 -> return null // Type 0 is invalid for magic resources
            1 -> return MagicAttack()
            2 -> return MagicEnhance()
            3 -> return MagicRestore()
            4 -> return MagicAuxiliary()
            5 -> return MagicSpecial()
        }
        return null
    }

    /**
     *
     * @param resType
     * 资源文件类型号1-12
     * @param type
     * 资源类型
     * @param index
     * 资源索引号
     * @return 资源所在位置, 返回-1表示不存在
     */
    private fun getDataOffset(resType: ResType, type: Int, index: Int): Int {
        return mDataOffset[getKey(resType.v, type, index)] ?: -1
    }

    /**
     *
     * @param resType
     * 资源文件类型号1-12
     * @param type
     * 资源类型
     * @param index
     * 资源索引号
     * @return 每个资源唯一的编号，用于哈希表键
     */
    private fun getKey(resType: Int, type: Int, index: Int): Int {
        return resType shl 16 or (type shl 8) or index
    }

    enum class ResType(val v: Int) {
        GUT(1), // 剧情脚本
        MAP(2), // 地图资源
        ARS(3), // 角色资源
        MRS(4), // 魔法资源
        SRS(5), // 特效资源
        GRS(6), // 道具资源
        TIL(7), // tile资源
        ACP(8), // 角色图片
        GDP(9), // 道具图片
        GGJ(10), // 特效图片
        PIC(11), // 杂类图片
        MLR(12), // 链资源
    }

    companion object {
        val instance: DatLib by lazy {
            val choiceLibName = sysGetChoiceLibName()
            DatLib(File.contentsOf(choiceLibName+".LIB"))
        }

        fun getRes(resType: ResType, type: Int, index: Int, allowNull: Boolean = false): ResBase? {
            return instance.getRes(resType, type, index, allowNull)
        }

        fun getPic(type: Int, index: Int, allowNull: Boolean = false): ResImage? {
            val res = getRes(ResType.PIC, type, index, allowNull)
            return if (res is ResImage) res else {
                if (!allowNull) println("Warning: getPic failed for type=$type, index=$index")
                null
            }
        }

        fun getMlr(type: Int, index: Int, allowNull: Boolean = false): ResMagicChain? {
            val res = getRes(ResType.MLR, type, index, allowNull)
            return if (res is ResMagicChain) res else {
                if (!allowNull) println("Warning: getMlr failed for type=$type, index=$index")
                null
            }
        }

        fun getACP(type: Int, index: Int, allowNull: Boolean = false): ResImage? {
            val res = getRes(ResType.ACP, type, index, allowNull)
            return if (res is ResImage) res else {
                if (!allowNull) println("Warning: getACP failed for type=$type, index=$index")
                null
            }
        }

        fun getMrsOrNull(type: Int, index: Int): BaseMagic? {
            val res = getRes(ResType.MRS, type, index, true)
            return if (res is BaseMagic) res else null
        }

        fun getMrs(type: Int, index: Int): BaseMagic {
            val res = getRes(ResType.MRS, type, index, false)
            return when {
                res is BaseMagic -> res
                else -> {
                    // 创建一个默认的魔法对象以防止崩溃
                    println("Warning: getMrs failed for type=$type, index=$index, returning default magic")
                    val defaultMagic = getMrsOrNull(1, 1) // 尝试获取基础攻击魔法
                    if (defaultMagic != null) {
                        defaultMagic
                    } else {
                        // 最后的保护措施
                        throw IllegalStateException("Cannot create default magic for type=$type, index=$index")
                    }
                }
            }
        }

        val missBitmap: Bitmap by lazy {
            getPic(2, 18)?.getBitmap(0) ?: run {
                val bmp = Bitmap(8*4, 14)
                val canvas = Canvas(bmp)
                TextRender.drawText(canvas, "Miss", 0, -2)
                bmp
            }
        }
    }
}
