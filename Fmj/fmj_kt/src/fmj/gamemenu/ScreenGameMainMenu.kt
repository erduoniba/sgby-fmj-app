package fmj.gamemenu

import fmj.Global
import fmj.characters.Player
import fmj.graphics.TextRender
import fmj.graphics.Util
import fmj.magic.BaseMagic
import fmj.magic.MagicRestore
import fmj.magic.ScreenMagic
import fmj.views.BaseScreen
import fmj.views.GameNode

import graphics.Canvas
import graphics.Rect
import java.gbkBytes

class ScreenGameMainMenu(override val parent: GameNode): BaseScreen {
    private val bmpFrame1 = Util.getFrameBitmap(93, 16 + 6)
    private val bmpFrame2 = Util.getFrameBitmap(32 + 6, 64 + 6)
    private val menuItemsRect: Rect
    private var menuItems: ByteArray = "属性魔法物品系统".gbkBytes()
    private val menuItemsS = arrayOf("属性", "魔法", "物品", "系统")

    private var mSelIndex = 0

    override val isPopup: Boolean
        get() = true

    private var screenSelectActor: BaseScreen = object : BaseScreen {
        override val parent = this@ScreenGameMainMenu

        private var index = 0

        private val mFrameRect = Rect(
               39, 29,
               125, 67 - 32 + game.playerList.size * 16)

        private val bmpFrame = Util.getFrameBitmap(mFrameRect.width(), mFrameRect.height())

        private var mNames: Array<String>

        private var mSum: Int = 0

        override val isPopup: Boolean
            get() = true

        init {
            val list = game.playerList
            mSum = list.size
            mNames = list.map { it.name }.toTypedArray()
        }

        override fun update(delta: Long) {}

        override fun draw(canvas: Canvas) {
            canvas.drawBitmap(this.bmpFrame, this.mFrameRect.left, this.mFrameRect.top)
            for (i in 0 until mSum) {
                if (i == index) {
                    TextRender.drawSelText(canvas, mNames[i], this.mFrameRect.left + 3,
                            this.mFrameRect.top + 3 + 16 * i)
                } else {
                    TextRender.drawText(canvas, mNames[i], this.mFrameRect.left + 3,
                            this.mFrameRect.top + 3 + 16 * i)
                }
            }
        }

        override fun onKeyDown(key: Int) {
            if (key == Global.KEY_DOWN) {
                ++index
                if (index >= mSum) {
                    index = 0
                }
            } else if (key == Global.KEY_UP) {
                --index
                if (index < 0) {
                    index = mSum - 1
                }
            }
        }

        override fun onKeyUp(key: Int) {
            if (key == Global.KEY_CANCEL) {
                popScreen()
            } else if (key == Global.KEY_ENTER) {
                popScreen()
                getScreenMagic(index)?.let {
                    pushScreen(it)
                }
            }
        }
    }

    init {
        val l = 9 + 3
        val t = 3 + 16 + 6 - 1 + 3
        val r = l + 32
        val b = t + 64
        menuItemsRect = Rect(l, t, r, b)
    }

    override fun update(delta: Long) {}

    override fun draw(canvas: Canvas) {
        canvas.drawBitmap(bmpFrame1, 9, 3)
        TextRender.drawText(canvas, "金钱:" + Player.sMoney, 9 + 3, 3 + 3)
        canvas.drawBitmap(bmpFrame2, 9, 3 + 16 + 6 - 1)
        TextRender.drawText(canvas, menuItems, 0, menuItemsRect)
        TextRender.drawSelText(canvas, menuItemsS[mSelIndex], menuItemsRect.left,
                menuItemsRect.top + mSelIndex * 16)
    }

    override fun onKeyDown(key: Int) {
        if (key == Global.KEY_UP) {
            if (--mSelIndex < 0) {
                mSelIndex = 3
            }
        } else if (key == Global.KEY_DOWN) {
            if (++mSelIndex > 3) {
                mSelIndex = 0
            }
        }
    }

    override fun onKeyUp(key: Int) {
        if (key == Global.KEY_ENTER) {
            val screen =
                    when (mSelIndex) {
                        0 -> ScreenMenuProperties(this)
                        1 -> if (game.playerList.size > 1)
                            screenSelectActor
                        else
                            getScreenMagic(0)
                        2 -> ScreenMenuGoods(this)
                        3 -> ScreenMenuSystem(this)
                        else -> null
                    }
            if (screen != null) {
                pushScreen(screen)
            }
        } else if (key == Global.KEY_CANCEL) {
            popScreen()
        }
    }

    /**
     *
     * @param id 0 1 2
     * @return
     */
    private fun getScreenMagic(id: Int): ScreenMagic? {
        val player = game.playerList[id]
        val magics = player.getAllMagics()

        if (magics.isEmpty()) return null

        return ScreenMagic(this, magics,
                player.mp,
                object : ScreenMagic.OnItemSelectedListener {
                    override fun onItemSelected(magic: BaseMagic) {
                        if (magic is MagicRestore) {
                            pushScreen(ScreenUseMagic(this@ScreenGameMainMenu, magic,
                                    game.playerList[id]))
                        } else {
                            showMessage("此处无法使用!", 1000)
                        }
                    }
                })
    }
}
