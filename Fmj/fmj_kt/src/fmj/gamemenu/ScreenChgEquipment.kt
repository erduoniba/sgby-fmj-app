package fmj.gamemenu

import fmj.Global
import fmj.characters.Player
import fmj.goods.GoodsEquipment
import fmj.graphics.Util
import fmj.views.BaseScreen
import fmj.views.GameNode
import graphics.Canvas

class ScreenChgEquipment
/**
 *
 * @param actorList 可装备选择的物品的角色链表
 * @param goods 选择的物品
 */
(override val parent: GameNode,
 private val mActor: Player, goods: GoodsEquipment, val itemIndex: Int? = null) : BaseScreen {

    private var mGoods: Array<GoodsEquipment>
    private var mSelIndex: Int = 0

    private var mPage = 0

    init {
        val currentEquip = if (itemIndex == null) {
            mActor.getCurrentEquipment(goods.type)
        } else {
            mActor.getEquipmentByIndex(itemIndex)
        }
        if (currentEquip == null || (itemIndex == null && mActor.hasSpace(goods.type))) {
            mGoods = arrayOf(goods)
            mSelIndex = 0
        } else {
            mGoods = arrayOf(currentEquip, goods)
            mSelIndex = 1
            // 没有空间，脱掉当前装备的
            mActor.takeOff(goods.type, itemIndex)
        }
        mActor.putOn(goods, itemIndex)
    }

    override fun update(delta: Long) {}

    override fun draw(canvas: Canvas) {
        canvas.drawColor(Global.COLOR_WHITE)
        mActor.drawState(canvas, mPage)
        mActor.drawHead(canvas, 5, 60)
        mGoods.forEachIndexed { i, equipment ->
            equipment.draw(canvas, 8, 2 + 32 * i)
        }
        Util.drawTriangleCursor(canvas, 1, 10 + 32 * mSelIndex)
    }

    override fun onKeyDown(key: Int) {
        fun step(x: Int) {
            mActor.takeOff(mGoods[mSelIndex].type, itemIndex)
            mSelIndex += x
            mActor.putOn(mGoods[mSelIndex], itemIndex)
        }
        if (key == Global.KEY_UP && mSelIndex > 0) {
            step(-1)
        } else if (key == Global.KEY_DOWN && mSelIndex < mGoods.size - 1) {
            step(1)
        } else if (key == Global.KEY_PAGEDOWN || key == Global.KEY_PAGEUP) {
            mPage = 1 - mPage
        }
    }

    override fun onKeyUp(key: Int) {
        if (key == Global.KEY_CANCEL) {
            // 换上原来的装备
            mActor.takeOff(mGoods[0].type, itemIndex)
            if (mGoods.size > 1) {
                mActor.putOn(mGoods[0], itemIndex)
            }
            popScreen()
        } else if (key == Global.KEY_ENTER) {
            if (mSelIndex == mGoods.size - 1) { // 换了新装备
                // 物品链中删除该装备
                Player.sGoodsList.deleteGoods(mGoods[mGoods.size - 1].type,
                        mGoods[mGoods.size - 1].index)
                // 物品链中加入老装备
                if (mGoods.size > 1) {
                    Player.sGoodsList.addGoods(mGoods[0].type, mGoods[0].index)
                }
            }
            popScreen()
        }
    }
}
