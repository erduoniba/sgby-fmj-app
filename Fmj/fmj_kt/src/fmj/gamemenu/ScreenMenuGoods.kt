package fmj.gamemenu

import fmj.Global
import fmj.characters.Player
import fmj.gamemenu.ScreenGoodsList.Mode
import fmj.gamemenu.ScreenGoodsList.OnItemSelectedListener
import fmj.goods.BaseGoods
import fmj.goods.GoodsEquipment
import fmj.graphics.TextRender
import fmj.graphics.Util
import fmj.lib.DatLib
import fmj.scene.ScreenMainGame
import fmj.views.BaseScreen
import fmj.views.GameNode

import graphics.Canvas

import java.gbkBytes
import java.System

class ScreenMenuGoods(override val parent: GameNode): BaseScreen, OnItemSelectedListener {

    private val mFrameBmp = Util.getFrameBitmap(77 - 39 + 1, 93 - 39 + 1)
    private val strs = arrayOf("使用", "装备", "丢弃")
    private var mSelId = 0

    override val isPopup: Boolean
        get() = true

    override fun update(delta: Long) {}

    override fun draw(canvas: Canvas) {
        canvas.drawBitmap(mFrameBmp, 39, 39)
        for (i in strs.indices) {
            if (i == mSelId) {
                TextRender.drawSelText(canvas, strs[i], 39 + 3, 39 + 3 + 16 * i)
            } else {
                TextRender.drawText(canvas, strs[i], 39 + 3, 39 + 3 + 16 * i)
            }
        }
    }

    override fun onKeyDown(key: Int) {
        if (key == Global.KEY_UP && mSelId > 0) {
            mSelId--
        } else if (key == Global.KEY_DOWN && mSelId < strs.size - 1) {
            mSelId++
        }
    }

    override fun onKeyUp(key: Int) {
        if (key == Global.KEY_CANCEL) {
            popScreen()
        } else if (key == Global.KEY_ENTER) {
            popScreen()
            val list = when (mSelId) {
                0 -> Player.sGoodsList.goodsList  // 使用：只显示消耗品
                1 -> Player.sGoodsList.equipList  // 装备：只显示装备
                2 -> Player.sGoodsList.allGoodsList  // 丢弃：显示所有物品
                else -> Player.sGoodsList.goodsList
            }
            pushScreen(ScreenGoodsList(this, list, this, Mode.Use))
        }
    }

    override fun onItemSelected(goods: BaseGoods, index: Int) {
        if (mSelId == 0) { // 使用
            goodsSelected(goods)
        } else if (mSelId == 1) { // 装备
            equipSelected(goods)
        } else if (mSelId == 2) { // 丢弃
            discardSelected(goods)
        }
    }

    private fun goodsSelected(goods: BaseGoods) {
        when (goods.type) {
            8 // 暗器
                , 12 // 兴奋剂
            -> showMessage("战斗中才能使用!", 1000)

            13 // 土遁
            -> {
                // TODO 迷宫中的用法，调用脚本
                game.triggerEvent(255)
                while (getCurScreen() !is ScreenMainGame) {
                    popScreen()
                }
            }

            14 // 剧情类
            ->
            {
                val gut = DatLib.getRes(DatLib.ResType.GUT, 255, goods.index, true)
                if (gut == null) {
                    showMessage("当前无法使用!", 1000)
                } else {
                    while (getCurScreen() !is ScreenMainGame) {
                        popScreen()
                    }
                    game.mainScene.callChapter(255, goods.index)
                }
            }

            9 // 药物
                , 10 // 灵药
                , 11 // 仙药
            -> pushScreen(ScreenTakeMedicine(this, goods))
        }
    }

    private fun equipSelected(goods: BaseGoods) {
        val list = game.playerList.filter { goods.canPlayerUse(it.index) }
        if (list.isEmpty()) { // 没人能装备
            showMessage("不能装备!", 1000)
        } else if (list.size == 1) { // 一个人能装备
            if (list[0].hasEquipt(goods.type, goods.index)) {
                showMessage("已装备!", 1000)
            } else {
                pushScreen(ScreenChgEquipment(this, list[0], goods as GoodsEquipment))
            }
        } else { // 多人可装备
            pushScreen(object : BaseScreen {
                override val parent = this@ScreenMenuGoods
                internal var bg = Util.getFrameBitmap(16 * 5 + 6, 6 + 16 * list.size)
                internal var curSel = 0
                internal var itemsText: Array<ByteArray> = Array(list.size) { ByteArray(11) }

                override val isPopup: Boolean
                    get() = true

                init {
                    for (i in itemsText.indices) {
                        for (j in 0..9) {
                            itemsText[i][j] = ' '.toByte()
                        }
                        val tmp = list[i].name.gbkBytes()
                        System.arraycopy(tmp, 0, itemsText[i], 0, tmp.size)
                    }
                }

                override fun update(delta: Long) {}

                override fun onKeyUp(key: Int) {
                    if (key == Global.KEY_ENTER) {
                        if (list[curSel].hasEquipt(goods.type, goods.index)) {
                            showMessage("已装备!", 1000)
                        } else {
                            popScreen()
                            pushScreen(ScreenChgEquipment(this, list[curSel], goods as GoodsEquipment))
                        }
                    } else if (key == Global.KEY_CANCEL) {
                        popScreen()
                    }
                }

                override fun onKeyDown(key: Int) {
                    if (key == Global.KEY_DOWN && curSel < itemsText.size - 1) {
                        ++curSel
                    } else if (key == Global.KEY_UP && curSel > 0) {
                        --curSel
                    }
                }

                override fun draw(canvas: Canvas) {
                    canvas.drawBitmap(bg, 50, 14)
                    for (i in itemsText.indices) {
                        if (i != curSel) {
                            TextRender.drawText(canvas, itemsText[i], 50 + 3, 14 + 3 + 16 * i)
                        } else {
                            TextRender.drawSelText(canvas, itemsText[i], 50 + 3, 14 + 3 + 16 * i)
                        }
                    }
                }
            })
        }
    }

    private fun discardSelected(goods: BaseGoods) {
        pushScreen(object : BaseScreen {
            override val parent = this@ScreenMenuGoods
            private val confirmBmp = Util.getFrameBitmap(16 * 8, 16 * 4)
            private var curSel = 0

            override val isPopup: Boolean
                get() = true

            override fun update(delta: Long) {}

            override fun draw(canvas: Canvas) {
                canvas.drawBitmap(confirmBmp, 25, 35)
                TextRender.drawText(canvas, "确认丢弃?", 28, 38)
                TextRender.drawText(canvas, "数量:${goods.goodsNum}", 28, 54)
                if (curSel == 0) {
                    TextRender.drawSelText(canvas, "全部丢弃", 28, 70)
                    TextRender.drawText(canvas, "丢弃1个", 95, 70)
                } else {
                    TextRender.drawText(canvas, "全部丢弃", 28, 70)
                    TextRender.drawSelText(canvas, "丢弃1个", 95, 70)
                }
            }

            override fun onKeyDown(key: Int) {
                if (key == Global.KEY_LEFT || key == Global.KEY_RIGHT) {
                    curSel = 1 - curSel
                }
            }

            override fun onKeyUp(key: Int) {
                if (key == Global.KEY_CANCEL) {
                    popScreen()
                } else if (key == Global.KEY_ENTER) {
                    if (curSel == 0) {
                        val totalNum = goods.goodsNum
                        Player.sGoodsList.useGoodsNum(goods.type, goods.index, totalNum)
                        popScreen()
                    } else {
                        val numBefore = goods.goodsNum
                        Player.sGoodsList.deleteGoods(goods)
                        if (numBefore <= 1) {
                            popScreen()
                        }
                    }
                }
            }
        })
    }
}
