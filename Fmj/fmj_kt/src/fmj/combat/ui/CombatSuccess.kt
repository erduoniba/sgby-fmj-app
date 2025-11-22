package fmj.combat.ui

import fmj.Global
import fmj.characters.Player
import fmj.goods.BaseGoods
import fmj.graphics.TextRender
import fmj.graphics.Util
import fmj.lib.DatLib
import fmj.lib.ResImage
import fmj.views.BaseScreen
import fmj.views.GameNode

import graphics.Bitmap
import graphics.Canvas
import graphics.Paint
import graphics.Paint.Style
import java.gbkBytes

class CombatSuccess(private val parent: GameNode, exp: Int, money: Int, private val mGoodsList: MutableList<BaseGoods>, lvuplist: List<Player>) {

    private val mMsgList: MutableList<BaseScreen> = mutableListOf()

    private val mLvupList: MutableList<BaseScreen>

    private var mCnt: Long = 0

    private var mIsAnyKeyPressed = false

    init {
        val e = exp.toString()
        mMsgList.add(MsgScreen(parent, 18, "获得经验     ".substring(0, 9 - e.length) + e))
        val m = money.toString()
        mMsgList.add(MsgScreen(parent, 46, "战斗获得        ".substring(0, 10 - m.length) + m + "钱"))

        mLvupList = mutableListOf()
        for (p in lvuplist) {
            mLvupList.add(MsgScreen(parent,p.name + "修行提升"))
            mLvupList.add(LevelupScreen(parent, p))
            val magicChain = p.magicChain
            if (magicChain != null) {
                val newNum = p.levelupChain.getLearnMagicNum(p.level)
                val oldNum = p.levelupChain.getLearnMagicNum(p.level - 1)
                val maxMagicCount = magicChain.getMagicCount()
                (oldNum until kotlin.math.min(newNum, maxMagicCount)).mapTo(mLvupList) {
                    LearnMagicScreen(parent, p.name, magicChain.getMagic(it).magicName)
                }
            }
        }
    }

    /**
     *
     * @param delta
     * @return `true` 所以内容显示完毕
     */
    fun update(delta: Long): Boolean {
        mCnt += delta
        if (mCnt > 1000 || mIsAnyKeyPressed) {
            mCnt = 0
            mIsAnyKeyPressed = false
            if (mGoodsList.size == 0) {
                if (mLvupList.size == 0) {
                    return true
                } else {
                    mMsgList.add(mLvupList.removeAt(0))
                }
            } else {
                val g = mGoodsList.removeAt(0)
                mMsgList.add(MsgScreen(parent,"得到 ${g.name} x${g.goodsNum}"))
            }
        }
        return false
    }

    fun draw(canvas: Canvas) {
        for (s in mMsgList) {
            s.draw(canvas)
        }
    }

    fun onKeyDown(key: Int) {}

    fun onKeyUp(key: Int) {
        mIsAnyKeyPressed = true
    }
}

class LevelupScreen(override val parent: GameNode, p: Player) : BaseScreen {

    private val mInfo: Bitmap

    init {
        val riRes = DatLib.getRes(DatLib.ResType.PIC, 2, 9, false)
        val ri = if (riRes is ResImage) riRes else {
            println("Warning: Failed to load PIC resource 2,9 for CombatSuccess")
            ResImage() // 使用空的 ResImage
        }
        mInfo = ri.getBitmap(0) ?: run {
            println("Warning: Failed to get bitmap from resource")
            Bitmap(120, 96) // 创建默认大小的空位图
        }

        val canvas = Canvas(mInfo)
        val lc = p.levelupChain
        val curl = p.level
        Util.drawSmallNum(canvas, p.hp, 37, 9)
        p.hp = p.maxHP
        Util.drawSmallNum(canvas, p.maxHP - (lc.getMaxHP(curl) - lc.getMaxHP(curl - 1)), 56, 9)
        Util.drawSmallNum(canvas, p.maxHP, 86, 9)
        Util.drawSmallNum(canvas, p.maxHP, 105, 9)
        Util.drawSmallNum(canvas, p.mp, 37, 21)
        p.mp = p.maxMP
        Util.drawSmallNum(canvas, p.maxMP - (lc.getMaxMP(curl) - lc.getMaxMP(curl - 1)), 56, 21)
        Util.drawSmallNum(canvas, p.maxMP, 86, 21)
        Util.drawSmallNum(canvas, p.maxMP, 105, 21)
        Util.drawSmallNum(canvas, p.attack - (lc.getAttack(curl) - lc.getAttack(curl - 1)), 47, 33)
        Util.drawSmallNum(canvas, p.attack, 96, 33)
        Util.drawSmallNum(canvas, p.defend - (lc.getDefend(curl) - lc.getDefend(curl - 1)), 47, 45)
        Util.drawSmallNum(canvas, p.defend, 96, 45)
        Util.drawSmallNum(canvas, p.speed - (lc.getSpeed(curl) - lc.getSpeed(curl - 1)), 47, 57)
        Util.drawSmallNum(canvas, p.speed, 96, 57)
        Util.drawSmallNum(canvas, p.lingli - (lc.getLingli(curl) - lc.getLingli(curl - 1)), 47, 69)
        Util.drawSmallNum(canvas, p.lingli, 96, 69)
        Util.drawSmallNum(canvas, p.luck - (lc.getLuck(curl) - lc.getLuck(curl - 1)), 47, 81)
        Util.drawSmallNum(canvas, p.luck, 96, 81)
    }

    override fun update(delta: Long) {}

    override fun draw(canvas: Canvas) {
        canvas.drawBitmap(mInfo, (Global.SCREEN_WIDTH - mInfo.width) / 2, (Global.SCREEN_HEIGHT - mInfo.height) / 2)
    }

    override fun onKeyDown(key: Int) {}

    override fun onKeyUp(key: Int) {}

}

class MsgScreen(override val parent: GameNode, private val mY: Int, _msg: String) : BaseScreen {

    private val mMsg: Bitmap

    private val mX: Int

    constructor(parent: GameNode, msg: String) : this(parent,(Global.SCREEN_HEIGHT - 24) / 2, msg)

    init {
        val msg = _msg.gbkBytes()
        val sideRes = DatLib.getRes(DatLib.ResType.PIC, 2, 8, true)
        val side = if (sideRes is ResImage) sideRes else {
            println("Warning: Failed to load PIC resource 2,8 for MsgScreen")
            ResImage() // 使用空的 ResImage
        }
        mMsg = Bitmap.createBitmap(msg.size * 8 + 8, 24)
        val c = Canvas(mMsg)
        c.drawColor(Global.COLOR_WHITE)
        side.draw(c, 1, 0, 0)
        side.draw(c, 2, mMsg.width - 3, 0)
        val p = Paint()
        p.color = Global.COLOR_BLACK
        p.style = Style.FILL_AND_STROKE
        c.drawLine(0, 1, mMsg.width, 1, p)
        c.drawLine(0, 22, mMsg.width, 22, p)
        TextRender.drawText(c, msg, 4, 4)

        mX = (Global.SCREEN_WIDTH - mMsg.width) / 2
    }

    override fun update(delta: Long) {}

    override fun draw(canvas: Canvas) {
        canvas.drawBitmap(mMsg, mX, mY)
    }

    override fun onKeyDown(key: Int) {}

    override fun onKeyUp(key: Int) {}

}

class LearnMagicScreen(override val parent: GameNode, playerName: String, magicName: String) : BaseScreen {

    private val mInfo: Bitmap = run {
        val infoRes = DatLib.getRes(DatLib.ResType.PIC, 2, 10, false)
        val infoImage = if (infoRes is ResImage) infoRes else {
            println("Warning: Failed to load PIC resource 2,10 for LearnMagicScreen")
            ResImage()
        }
        infoImage.getBitmap(0) ?: run {
            println("Warning: Failed to get bitmap for LearnMagicScreen")
            Bitmap(120, 80) // 默认大小
        }
    }

    init {
        var pn: ByteArray
        var mn: ByteArray
        try {
            pn = playerName.gbkBytes()
            mn = magicName.gbkBytes()
        } catch (e: Error) {
            pn = ByteArray(0)
            mn = ByteArray(0)
        }

        val canvas = Canvas(mInfo)
        TextRender.drawText(canvas, pn, (mInfo.width - pn.size * 8) / 2, 8)
        TextRender.drawText(canvas, mn, (mInfo.width - mn.size * 8) / 2, 42)
    }

    override fun update(delta: Long) {}

    override fun draw(canvas: Canvas) {
        canvas.drawBitmap(mInfo, (Global.SCREEN_WIDTH - mInfo.width) / 2, (Global.SCREEN_HEIGHT - mInfo.height) / 2)
    }

    override fun onKeyDown(key: Int) {}

    override fun onKeyUp(key: Int) {}
}

