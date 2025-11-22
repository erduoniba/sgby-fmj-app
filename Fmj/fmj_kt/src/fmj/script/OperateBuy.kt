package fmj.script

import fmj.Global
import fmj.characters.Player
import fmj.gamemenu.ScreenGoodsList
import fmj.gamemenu.ScreenGoodsList.Mode
import fmj.goods.BaseGoods
import fmj.graphics.TextRender
import fmj.graphics.Util
import fmj.lib.DatLib
import fmj.views.BaseScreen
import fmj.views.Control
import fmj.views.GameNode

import graphics.Canvas

class OperateBuy(override val parent: GameNode, data: ByteArray)
    : Control, Operate, ScreenGoodsList.OnItemSelectedListener {
    private val goodsList: MutableList<BaseGoods> =  mutableListOf()
    private val mBuyScreen = BuyGoodsScreen(this)

    init {
        var i = 0
        while (data[i].toInt() != 0) {
            var g = Player.sGoodsList.getGoods(
                    data[i + 1].toInt() and 0xff,
                    data[i].toInt() and 0xff)

            if (g == null) {
                g = DatLib.getRes(DatLib.ResType.GRS,
                        data[i + 1].toInt() and 0xff,
                        data[i].toInt() and 0xff) as BaseGoods
                g.goodsNum = 0
            }

            goodsList.add(g)
            i += 2
        }
        run()
    }

    private fun run() {
        pushScreen(ScreenGoodsList(this, goodsList, this, Mode.Buy))
    }

    override fun update(delta: Long): Boolean {
        return false
    }

    override fun draw(canvas: Canvas) {
        game.mainScene.drawSceneWithoutClear(canvas)
    }

    override fun onKeyDown(key: Int) {}

    override fun onKeyUp(key: Int) {}

    override fun onItemSelected(goods: BaseGoods, index: Int) {
        if (Player.sMoney < goods.buyPrice) {
            showMessage("金钱不足!", 1000)
        } else {
            mBuyScreen.init(goods)
            pushScreen(mBuyScreen)
        }
    }

    private class BuyGoodsScreen(override val parent: GameNode): BaseScreen {
        private var goods: BaseGoods? = null
        private var buyCnt: Int = 0
        private var money: Int = 0
        private val bmpBg by lazy {
            Util.getFrameBitmap(136, 55)
        }

        override val isPopup: Boolean
            get() = true

        fun init(goods: BaseGoods) {
            this.goods = goods
            buyCnt = 0
            money = Player.sMoney
        }

        override fun update(delta: Long) {}

        override fun draw(canvas: Canvas) {
            canvas.drawBitmap(bmpBg, 12, 21)
            TextRender.drawText(canvas, "金钱：" + money, 15, 24)
            TextRender.drawText(canvas, goods!!.name, 15, 40)
            TextRender.drawText(canvas, ": " + goods!!.goodsNum, 93, 40)
            TextRender.drawText(canvas, "买入个数　：" + buyCnt, 15, 56)
        }

        override fun onKeyUp(key: Int) {
            if (key == Global.KEY_ENTER) {
                Player.sMoney = money
                if (buyCnt == goods!!.goodsNum && buyCnt > 0) {
                    Player.sGoodsList.addGoods(goods!!.type, goods!!.index, buyCnt)
                }
                popScreen()
            } else if (key == Global.KEY_CANCEL) {
                goods!!.addGoodsNum(-buyCnt)
                popScreen()
            }
        }

        override fun onKeyDown(key: Int) {
            if (key == Global.KEY_UP && goods!!.goodsNum < 99) {
                if (money >= goods!!.buyPrice) {
                    ++buyCnt
                    goods!!.addGoodsNum(1)
                    money -= goods!!.buyPrice
                } else {
                    showMessage("金钱不足!", 1000)
                }
            } else if (key == Global.KEY_DOWN && buyCnt > 0) {
                --buyCnt
                goods!!.addGoodsNum(-1)
                money += goods!!.buyPrice
            }
        }
    }

}
