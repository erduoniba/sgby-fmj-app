package fmj.goods

import fmj.lib.DatLib
import java.ObjectInput
import java.ObjectOutput


/**
 * 物品管理
 * @author Chen
 */
class GoodsManage {

    /**
     * 保存只能用一次的物品，暗器、药品等
     */
    internal var mGoodsList = mutableListOf<BaseGoods>()
    /**
     * 装备链表
     */
    internal var mEquipList = mutableListOf<BaseGoods>()

    val goodsList: List<BaseGoods>
        get() = mGoodsList

    val equipList: List<BaseGoods>
        get() = mEquipList

    /**
     * 获取所有物品（装备 + 消耗品）
     */
    val allGoodsList: List<BaseGoods>
        get() = mEquipList + mGoodsList

    /**
     * 返回装备种类数目
     * @return
     */
    val equitTypeNum: Int
        get() = mEquipList.size

    /**
     * 返回一次性物品种类数目
     * @return
     */
    val goodsTypeNum: Int
        get() = mGoodsList.size

    /**
     * 获取装备链表中的i号装备
     * @param i
     * @return 当`i < 0 || i >= list.size()`时，返回`null`
     */
    fun getEquip(i: Int): BaseGoods? {
        return if (i >= 0 && i < mEquipList.size) {
            mEquipList[i]
        } else null
    }

    /**
     * 获取一次性物品链表中i位置的物品
     * @param i
     * @return 当`i < 0 || i >= list.size()`时，返回`null`
     */
    fun getGoods(i: Int): BaseGoods? {
        return if (i >= 0 && i < mGoodsList.size) {
            mGoodsList[i]
        } else null
    }

    /**
     * 获取链表中type index号物品
     * @param type
     * @param index
     * @return 当该种物品不存在时返回`null`
     */
    fun getGoods(type: Int, index: Int): BaseGoods? {
        if (type >= 1 && type <= 7) {
            return getGoods(mEquipList, type, index)
        } else if (type >= 8 && type <= 14) {
            return getGoods(mGoodsList, type, index)
        }
        return null
    }

    private fun getGoods(list: MutableList<BaseGoods>, type: Int, index: Int): BaseGoods? {
        return list.firstOrNull { it.type == type && it.index == index }
    }

    /**
     * 得到type index号物品的数量
     * @param type
     * @param index
     * @return 物品数量
     */
    fun getGoodsNum(type: Int, index: Int): Int {
        var num = 0
        if (type in 1..7) {
            num = getGoodsNum(mEquipList, type, index)
        } else if (type in 8..14) {
            num = getGoodsNum(mGoodsList, type, index)
        }
        return num
    }

    private fun getGoodsNum(list: MutableList<BaseGoods>, type: Int, index: Int): Int {
        return list.firstOrNull { it.type == type && it.index == index }?.goodsNum ?: 0
    }

    /**
     *
     * @param goods
     */
    fun addGoods(type: Int, index: Int, num: Int) {
        if (type in 1..7) {
            addGoods(mEquipList, type, index, num)
        } else if (type in 8..14) {
            addGoods(mGoodsList, type, index, num)
        }
    }

    fun addGoods(type: Int, index: Int) {
        if (type in 1..7) { // 装备
            addGoods(mEquipList, type, index, 1)
        } else if (type in 8..14) { // 物品，使用一次
            addGoods(mGoodsList, type, index, 1)
        }
    }

    fun addGoods(goods: BaseGoods) {
        addGoods(goods.type, goods.index)
    }

    fun deleteGoods(goods: BaseGoods) {
        deleteGoods(goods.type, goods.index)
    }

    /**
     * 将type index号物品添加到list链表中
     *
     *
     * 如果有该号物品，则计数加一
     *
     *
     * 没有则加入该物品
     * @param list
     * @param type
     * @param index
     */
    private fun addGoods(list: MutableList<BaseGoods>, type: Int, index: Int, num: Int) {
        list.forEach {
            if (it.type == type && it.index == index) {
                it.addGoodsNum(num)
                return
            }
        }
        // 加入链表
        val item = DatLib.getRes(DatLib.ResType.GRS, type, index) as BaseGoods
        item.goodsNum = num
        list.add(item)
    }

    /**
     * 物品数量减一，如果数量为0，则删除物品
     * @param type
     * @param index
     * @return 删除成功返回`true` 不存在该物品返回`false`
     */
    fun deleteGoods(type: Int, index: Int): Boolean {
        return useGoodsNum(type, index, 1)
    }

    /**
     * 物品数量减num，如果数量不足则返回false
     * @param type
     * @param index
     * @return
     */
    fun useGoodsNum(type: Int, index: Int, num: Int): Boolean {
        if (type in 1..7) { // 装备
            return deleteGoods(mEquipList, type, index, num)
        } else if (type in 8..14) { // 物品，使用一次
            return deleteGoods(mGoodsList, type, index, num)
        }
        return false
    }

    private fun deleteGoods(list: MutableList<BaseGoods>, type: Int, index: Int, num: Int): Boolean {
        // TODO: refactor
        val iter = list.iterator()
        while (iter.hasNext()) {
            val i = iter.next()
            if (i.type == type && i.index == index) { // 数量-num
                if (i.goodsNum < num) {
                    return false
                }
                i.addGoodsNum(-num)
                if (i.goodsNum <= 0) {
                    iter.remove()
                }
                return true
            }
        }
        return false
    }

    fun clear() {
        mEquipList.clear()
        mGoodsList.clear()
    }

    fun read(coder: ObjectInput) {
        clear()
        var size = coder.readInt()
        for (i in 0 until size) {
            val g = DatLib.getRes(DatLib.ResType.GRS, coder.readInt(), coder.readInt()) as BaseGoods
            g.goodsNum = coder.readInt()
            mEquipList.add(g)
        }

        size = coder.readInt()
        for (i in 0 until size) {
            val g = DatLib.getRes(DatLib.ResType.GRS, coder.readInt(), coder.readInt()) as BaseGoods
            g.goodsNum = coder.readInt()
            mGoodsList.add(g)
        }
    }

    fun write(out: ObjectOutput) {
        var size = mEquipList.size
        out.writeInt(size)
        for (i in 0 until size) {
            val g = mEquipList[i]
            out.writeInt(g.type)
            out.writeInt(g.index)
            out.writeInt(g.goodsNum)
        }

        size = mGoodsList.size
        out.writeInt(size)
        for (i in 0 until size) {
            val g = mGoodsList[i]
            out.writeInt(g.type)
            out.writeInt(g.index)
            out.writeInt(g.goodsNum)
        }
    }
}
