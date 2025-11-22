package fmj.gamemenu

import fmj.Global
import fmj.characters.Player
import fmj.gamemenu.ScreenGoodsList.Mode
import fmj.goods.BaseGoods
import fmj.goods.GoodsEquipment
import fmj.graphics.TextRender
import fmj.graphics.Util
import fmj.views.BaseScreen
import fmj.views.GameNode

import graphics.Canvas
import graphics.Point
import graphics.Rect

import java.Stack
import java.gbkBytes

class ScreenActorWearing(override val parent: GameNode): BaseScreen {

    private val mPos: Array<Point>
    private var mEquipments: Array<GoodsEquipment?>
    private val mItemName = arrayOf("装饰", "装饰", "护腕", "脚蹬", "手持", "身穿", "肩披", "头戴")
    private var mCurItem = 0

    private var mActorIndex = -1

    private var showingDesc = false
    private val bmpName = Util.getFrameBitmap(92 - 9 + 1, 29 - 10 + 1)//Bitmap.createBitmap(92 - 9 + 1, 29 - 10 + 1, Config.ARGB_8888);
    private val bmpDesc = Util.getFrameBitmap(151 - 9 + 1, 65 - 28 + 1)//Bitmap.createBitmap(151 - 9 + 1, 65 - 28 + 1, Config.ARGB_8888);
    private var mTextName = byteArrayOf()
    private var mTextDesc = byteArrayOf()
    private var mToDraw = 0 // 当前要画的描述中的字节
    private var mNextToDraw = 0 // 下一个要画的描述中的字节
    private val mStackLastToDraw = Stack.create<Int>() // 保存上次描述所画位置

    init {
        mEquipments = game.playerList[0].equipmentsArray
        mActorIndex = 0
        // 320x192大屏适配，装备分布更均匀
        mPos = arrayOf(
            Point(80, 20),   // 装饰1
            Point(60, 60),   // 装饰2
            Point(110, 100), // 护腕
            Point(150, 120), // 脚蹬
            Point(200, 100), // 手持
            Point(240, 60),  // 身穿
            Point(220, 25),  // 肩披
            Point(140, 10)   // 头戴
        )
    }

    private fun getGBKBytes(s: String): ByteArray {
        return s.gbkBytes()
    }

    override fun update(delta: Long) {}

    override fun draw(canvas: Canvas) {
        canvas.drawColor(Global.COLOR_WHITE)
        // 穿戴底图放右下角
        canvas.drawBitmap(Util.bmpChuandai, Global.SCREEN_WIDTH - Util.bmpChuandai.width - 10, Global.SCREEN_HEIGHT - Util.bmpChuandai.height - 10)

        // 画装备
        for (i in 0..7) {
            mEquipments[i]?.draw(canvas, mPos[i].x + 1, mPos[i].y + 1)
        }
        canvas.drawRect(mPos[mCurItem].x, mPos[mCurItem].y,
                mPos[mCurItem].x + 32, mPos[mCurItem].y + 32, Util.sBlackPaint)
        TextRender.drawText(canvas, mItemName[mCurItem], 200, 60)

        // 画人物头像、姓名，居中偏上
        if (mActorIndex >= 0) {
            val p = game.playerList[mActorIndex]
            p.drawHead(canvas, 140, 40)
            TextRender.drawText(canvas, p.name, 140, 80)
        }

        if (showingDesc) {
            // 320x192大屏适配：说明框放在底部，充分利用屏幕宽度
            val descWidth = Global.SCREEN_WIDTH - 40  // 留出左右边距
            val descHeight = 80  // 底部说明区域高度
            val descBg = Util.getFrameBitmap(descWidth, descHeight)
            val descY = Global.SCREEN_HEIGHT - descHeight - 10  // 距离底部10像素
            
            canvas.drawBitmap(descBg, 20, descY)
            
            // 显示装备名称在说明框顶部
            TextRender.drawText(canvas, "装备: ", 25, descY + 5)
            TextRender.drawText(canvas, mTextName, 65, descY + 5)
            
            // 显示装备描述，使用整个底部宽度
            mNextToDraw = TextRender.drawText(canvas, mTextDesc, mToDraw, 
                Rect(25, descY + 22, Global.SCREEN_WIDTH - 25, descY + descHeight - 5))
        }
    }

    private fun resetDesc() {
        if (showingDesc) {
            showingDesc = false
            mNextToDraw = 0
            mToDraw = mNextToDraw
            mStackLastToDraw.clear()
        }
    }

    override fun onKeyDown(key: Int) {
        if (key == Global.KEY_DOWN && mCurItem < 8 - 1) {
            ++mCurItem
            resetDesc()
        } else if (key == Global.KEY_UP && mCurItem > 0) {
            --mCurItem
            resetDesc()
        } else if (key == Global.KEY_RIGHT && mActorIndex < game.playerList.size - 1) {
            ++mActorIndex
            mEquipments = game.playerList[mActorIndex].equipmentsArray
            resetDesc()
        } else if (key == Global.KEY_LEFT && mActorIndex > 0) {
            --mActorIndex
            mEquipments = game.playerList[mActorIndex].equipmentsArray
            resetDesc()
        } else if (showingDesc) {
            if (key == Global.KEY_PAGEDOWN) {
                if (mNextToDraw < mTextDesc.size) {
                    mStackLastToDraw.push(mToDraw)
                    mToDraw = mNextToDraw
                }
            } else if (key == Global.KEY_PAGEUP && mToDraw != 0) {
                mStackLastToDraw.pop()?.let {
                    mToDraw = it
                }
            }
        }
    }

    override fun onKeyUp(key: Int) {
        if (key == Global.KEY_CANCEL) {
            popScreen()
        } else if (key == Global.KEY_ENTER) {
            if (!showingDesc && mEquipments[mCurItem] != null) {
                showingDesc = true
                mTextName = getGBKBytes(mEquipments[mCurItem]?.name ?: "")
                mTextDesc = getGBKBytes(mEquipments[mCurItem]?.description ?: "")
            } else { // put change equipment screen
                resetDesc()
                pushScreen(ScreenGoodsList(this, getTheEquipList(Player.sEquipTypes[mCurItem]),
                        object : ScreenGoodsList.OnItemSelectedListener {
                            override fun onItemSelected(goods: BaseGoods, index: Int) {
                                val actor = game.playerList[mActorIndex]
                                popScreen()
                                pushScreen(ScreenChgEquipment(this@ScreenActorWearing, actor, goods as GoodsEquipment, mCurItem))
                            }
                        }, Mode.Use))
            }
        }
    }

    private fun getTheEquipList(type: Int): List<BaseGoods> {
        val currentPlayer = game.playerList[mActorIndex]
        return Player.sGoodsList.equipList
                .filter { it.type == type && it.canPlayerUse(currentPlayer.index) }
    }

    companion object {
        private val sRectDesc = Rect(9 + 3, 28 + 3, 151, 65)
    }


}
