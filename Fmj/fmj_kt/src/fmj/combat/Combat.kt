package fmj.combat

import fmj.Global
import fmj.ScreenViewType
import fmj.config.GameSettings
import fmj.characters.FightingCharacter
import fmj.characters.Monster
import fmj.characters.Player
import fmj.combat.actions.*
import fmj.combat.ui.CombatSuccess
import fmj.combat.ui.CombatUI
import fmj.goods.BaseGoods
import fmj.goods.GoodsManage
import fmj.goods.Throwable
import fmj.lib.DatLib
import fmj.lib.ResImage
import fmj.lib.ResSrs
import fmj.magic.MagicAttack
import fmj.magic.MagicRestore
import fmj.views.BaseScreen
import fmj.views.GameNode

import graphics.Bitmap
import graphics.Canvas
import graphics.Point
import java.*
import kotlin.math.sqrt
import kotlin.math.min
import kotlin.js.js
import kotlin.js.JsExport

class Combat private constructor(override val parent: GameNode) : BaseScreen, CombatUI.CallBack {

    private var mScrb: Int = 0
    private var mScrl: Int = 0
    private var mScrR: Int = 0

    private var mCombatState = CombatState.SelectAction

    /** 是否自动攻击，围攻状态 */
    private var mIsAutoAttack = false

    /** 动作队列，一个回合中，双方的决策 */
    private val mActionQueue = ArrayQueue.create<Action>()

    /** 动作队列的执行者 */
    private val mActionExecutor = ActionExecutor(mActionQueue, this)

    /** 战斗的UI */
    private val mCombatUI = CombatUI(this, this, 0)

    /** 随机战斗中，可能出现的敌人类型 */
    private var mMonsterType: IntArray = intArrayOf()

    /** 参加战斗的怪物队列 */
    private var mMonsterList: MutableList<Monster> = mutableListOf()

    /** 参加战斗的玩家角色队列 */
    private var mPlayerList: List<Player> = listOf()

    /** 每个玩家上一回合的动作，用于重复功能 */
    private val mLastPlayerActions = mutableMapOf<Int, Action>()

    /** 当前选择动作的角色在[.mPlayerList]中的序号 */
    private var mCurSelActionPlayerIndex = 0

    /** 当前回合 */
    private var mRoundCnt: Int = 0

    private var mHasEventExed: Boolean = false

    /** 最多回合数，0为无限 */
    private var mMaxRound: Int = 0

    /** 触发事件的回合，以及对应的事件号 */
    private var mEventRound: IntArray? = null
    private var mEventNum: IntArray? = null

    /** 战斗失败跳转地址，战斗成功跳转地址 */
    private var mLossAddr: Int = 0
    private var mWinAddr: Int = 0

    private val mFlyPeach: ResSrs = run {
        val res = DatLib.getRes(DatLib.ResType.SRS, 1, 249, false)
        if (res is ResSrs) {
            res
        } else {
            println("Warning: Failed to load mFlyPeach animation, using empty SRS")
            ResSrs() // 返回空的 ResSrs 对象
        }
    }

    private var mIsWin = false

    /**战斗背景图 */
    internal var mBackground: Bitmap = Bitmap(0, 0)

    /** 战斗胜利能获得的金钱和经验 */
    private var mWinMoney: Int = 0
    private var mWinExp: Int = 0

    private var mCombatSuccess: CombatSuccess? = null

    private var mTimeCnt: Long = 0

    /** 是否有玩家角色存活 */
    private val isAnyPlayerAlive: Boolean
        get() {
            return mPlayerList.any { it.hp > 0 }
        }

    /** 怪物是否都挂了 */
    private val isAllMonsterDead: Boolean
        get() = firstAliveMonster == null

    /**
     * 获取下一个存活的主角序号
     * @return 没有就返回-1
     */
    private val nextAlivePlayerIndex: Int
        get() =
            (mCurSelActionPlayerIndex + 1 until mPlayerList.size)
                    .firstOrNull { mPlayerList[it].isAlive }
                    ?: -1

    private val preAlivePlayerIndex: Int
        get() =
            (mCurSelActionPlayerIndex - 1 downTo 0)
                    .firstOrNull { mPlayerList[it].isAlive }
                    ?: -1

    private val firstAlivePlayerIndex: Int
        get() =
            mPlayerList.indices.firstOrNull { mPlayerList[it].isAlive }
                    ?: -1
    /**
     *
     * @return 第一个活着的怪物，`null`怪物都挂了
     */
    val firstAliveMonster: Monster?
        get() = mMonsterList.firstOrNull { it.isAlive }

    /**
     * 随机获取一个或者的玩家角色
     * @return `null`全死了
     */
    val randomAlivePlayer: Player?
        get() {
            var cnt = 0
            for (p in mPlayerList) {
                if (p.isAlive) {
                    ++cnt
                }
            }

            if (cnt == 0) return null

            val arr = arrayOfNulls<Player>(cnt)
            var i = 0
            for (p in mPlayerList) {
                if (p.isAlive) {
                    arr[i++] = p
                }
            }

            return arr[sRandom.nextInt(cnt)]
        }

    private enum class CombatState {
        SelectAction, // 玩家操作阶段，制定攻击策略
        PerformAction, // 执行动作队列，播放攻击动画
        Win, // 赢得战斗
        Loss, // 战斗失败
        Exit
    }

    /**
     * 简单的图片缩放算法 - 最近邻插值
     */
    private fun scaleBitmap(src: Bitmap, newWidth: Int, newHeight: Int): Bitmap {
        val dst = Bitmap.createBitmap(newWidth, newHeight)
        val xRatio = src.width.toFloat() / newWidth
        val yRatio = src.height.toFloat() / newHeight
        
        for (y in 0 until newHeight) {
            for (x in 0 until newWidth) {
                val srcX = (x * xRatio).toInt().coerceIn(0, src.width - 1)
                val srcY = (y * yRatio).toInt().coerceIn(0, src.height - 1)
                val color = src.buffer[srcY * src.width + srcX]
                dst.buffer[y * newWidth + x] = color
            }
        }
        
        return dst
    }

    private fun createBackgroundBitmap(scrb: Int, scrl: Int, scrr: Int) {
        mScrb = scrb
        mScrl = scrl
        mScrR = scrr
        
        mBackground = Bitmap.createBitmap(Global.SCREEN_WIDTH, Global.SCREEN_HEIGHT)
        val canvas = Canvas(mBackground)
        val currentGame = sysGetChoiceLibName().uppercase()
        if (currentGame == "XJQXZSHYMYX") {
            // 如果背景图片完全获取失败，使用纯色背景
            canvas.drawColor(Global.COLOR_WHITE)
            return
        }

        // 获取背景图片
        val bgImgRes = DatLib.getRes(DatLib.ResType.PIC, 4, scrb, true)
        val bgImg = if (bgImgRes is ResImage) bgImgRes else null

        // 获取原始图片的 bitmap
        val originalBitmap = bgImg?.getBitmap(0)
        
        if (originalBitmap != null) {
            // 使用单张图片放大填充整个屏幕，避免多图拼接造成的不连续问题
            // 这样在不同游戏中背景图案会保持连续性，提供更好的视觉体验
            val scaledBitmap = scaleBitmap(originalBitmap, Global.SCREEN_WIDTH, Global.SCREEN_HEIGHT)
            
            // 绘制放大后的背景
            canvas.drawBitmap(scaledBitmap, 0, 0)
        } else if (bgImg != null) {
            // 如果获取失败，使用原来的平铺方式作为后备方案
            val tileW = bgImg.width
            val tileH = bgImg.height

            // 计算需要多少个完整的瓦片来覆盖屏幕
            val tilesX = (Global.SCREEN_WIDTH) / (tileW + 1)  // 向上取整
            val tilesY = (Global.SCREEN_HEIGHT) / tileH // 向上取整

            for (tileY in 0 until tilesY) {
                for (tileX in 0 until tilesX) {
                    val x = tileX * tileW
                    val y = tileY * tileH
                    if (tileX > 0) {
                        bgImg.draw(canvas, 1, x - 4, y)
                    }
                    else {
                        bgImg.draw(canvas, 1, x + 4, y)
                    }
                }
            }
        } else {
            // 如果背景图片完全获取失败，使用纯色背景
            canvas.drawColor(Global.COLOR_BLACK)
            println("Warning: Failed to load background image for combat, using black background")
        }

        // var img: ResImage? 
        // img = DatLib.getRes(DatLib.ResType.PIC, 4, scrl) as ResImage
        // img.draw(canvas, 1, 0, 192 - img.height) // 左下角
        // img = DatLib.getRes(DatLib.ResType.PIC, 4, scrr) as ResImage
        // img.draw(canvas, 1, 320 - img.width, 0) // 右上角
    }

    private fun prepareForNewCombat() {
        mActionQueue.clear()
        
        // R键功能：从静态存储加载玩家动作历史
        mLastPlayerActions.clear()
        if (sGlobalPlayerActions.isNotEmpty()) {
            mLastPlayerActions.putAll(sGlobalPlayerActions)
            println("[战斗准备] 从全局存储加载 ${mLastPlayerActions.size} 个动作记录用于R键功能")
            mLastPlayerActions.forEach { (index, action) ->
                println("[战斗准备]   玩家$index: ${action::class.simpleName}")
            }
        } else {
            println("[战斗准备] 没有历史动作记录")
        }

        mIsAutoAttack = false
        mCombatState = CombatState.SelectAction

        mCurSelActionPlayerIndex = 0
        
        // 只使用前MAX_COMBAT_PLAYERS个角色参战（这些是在角色管理器中设置为上线的角色）
        val allPlayers = game.playerList
        mPlayerList = allPlayers.take(MAX_COMBAT_PLAYERS).filter { it.hp > 0 }
        
        // 如果没有活着的角色，至少要有第一个角色
        if (mPlayerList.isEmpty() && allPlayers.isNotEmpty()) {
            val firstPlayer = allPlayers[0]
            if (firstPlayer.hp <= 0) {
                firstPlayer.hp = 1
            }
            mPlayerList = listOf(firstPlayer)
        }
        
        println("[战斗准备] 参战角色: ${mPlayerList.map { it.name }.joinToString(", ")}")

        mCombatUI.reset()
        mCombatUI.setCurrentPlayerIndex(0)
        mCombatUI.setMonsterList(mMonsterList)
        mCombatUI.setPlayerList(mPlayerList)

        mPlayerList.forEach {
            it.resetDebuff()
        }

        setOriginalPlayerPos()
        setOriginalMonsterPos()

        mRoundCnt = 0

        mHasEventExed = false

        // 检查玩家血量
        for (p in mPlayerList) {
            if (p.hp <= 0) { // 确保血量大于0
                p.hp = 1
            }
            p.setFrameByState()
        }

        // 怪物血量设置为其最大值
        for (m in mMonsterList) {
            m.hp = m.maxHP
        }

        // 计算战斗胜利能获得的金钱和经验
        mWinMoney = 0
        mWinExp = 0
        for (m in mMonsterList) {
            // 读取JavaScript中的倍数变量，如果没有设置则默认为1
            val winMoneyMultiple = sysGetWinMoneyMultiple()
            val winExpMultiple = sysGetWinExpMultiple()
            
            mWinMoney += m.money * winMoneyMultiple
            mWinExp += m.exp * winExpMultiple
        }

        if (!sIsRandomFight && mMonsterList.size == 1) { // 剧情战斗，只有一个怪时，怪的位置在中间
            val m = mMonsterList[0]
            val res = DatLib.getRes(DatLib.ResType.ARS, m.type, m.index)
            if (res is Monster) {
                val n = res
                n.hp = -1
                n.isVisiable = false
                mMonsterList.add(0, n) // 加入一个看不见的怪
                setOriginalMonsterPos() // 重置位置
            } else {
                println("Warning: Combat init monster type mismatch - expected Monster but got ${res?.let { it::class.simpleName }}")
            }
        }

        mFlyPeach.start()
        mFlyPeach.setIteratorNum(5)
    }

    private fun exitCurrentCombat() {
        if (!sIsRandomFight) {
            // 检查是否因为达到最大回合数而退出（既不是胜利也不是真正的失败）
            val isMaxRoundExit = mMaxRound > 0 && mRoundCnt >= mMaxRound && !mIsWin
            
            if (isMaxRoundExit) {
                // 达到最大回合数：不跳转，从 EnterFight 的下一条指令继续
                println("[Combat] Exiting due to max rounds, continuing from next instruction")
                
                // 获取脚本进程
                val scriptProcess = game.mainScene.scriptProcess
                
                // 设置索引为 EnterFight 时记录的索引 + 1
                if (sEnterFightScriptIndex >= 0) {
                    val nextIndex = sEnterFightScriptIndex + 1
                    println("[Combat] Setting script index from $sEnterFightScriptIndex to $nextIndex")
                    scriptProcess.setCurrentIndex(nextIndex)
                } else {
                    println("[Combat] Warning: No recorded script index")
                }
                
                // 脚本被 exitScript 停止了，需要重新启动
                if (!scriptProcess.running) {
                    scriptProcess.start()
                }
                
                // 确保可以继续执行
                scriptProcess.goonExecute = true
                
                // 执行当前索引的命令
                println("[Combat] Executing command at index ${scriptProcess.getCurrentIndex()}")
                scriptProcess.executeCurrentCommand()
            } else {
                // 正常的胜利或失败：跳转到相应地址
                val targetAddress = if (mIsWin) mWinAddr else mLossAddr
                game.gotoAddress(targetAddress)
                // 确保可以继续执行
                game.mainScene.scriptProcess.goonExecute = true
            }
            
            sIsRandomFight = true
            sInstance = sInstanceBk
            sInstanceBk = null
            sEnterFightScriptIndex = -1  // 清理索引记录
        } else {
            if (!mIsWin) { // 死了，游戏结束
                game.changeScreen(ScreenViewType.SCREEN_MENU)
            }
        }

        sIsFighting = false
        mActionQueue.clear()
        mActionExecutor.reset()
        mCombatUI.reset()
        mIsAutoAttack = false

        // 恢复一定的血量
        for (p in mPlayerList) {
            if (p.hp <= 0) {
                p.hp = 1
            }
            if (p.mp <= 0) {
                p.mp = 1
            }
            p.hp = (p.hp + (p.maxHP - p.hp) / 10)
            p.mp = (p.mp + p.maxMP / 5)
            if (p.mp > p.maxMP) {
                p.mp = p.maxMP
            }
        }
    }

    private fun setOriginalPlayerPos() {
        for (i in mPlayerList.indices) {
            // sPlayerPos array only has 3 positions, so we need to ensure we don't access out of bounds
            if (i < sPlayerPos.size) {
                mPlayerList[i].setCombatPos(sPlayerPos[i].x, sPlayerPos[i].y)
            } else {
                // If we have more than 3 players, place additional players at a default position
                // Using the last available position as a fallback
                val lastPos = sPlayerPos[sPlayerPos.size - 1]
                mPlayerList[i].setCombatPos(lastPos.x, lastPos.y)
            }
        }
    }

    private fun setOriginalMonsterPos() {
        for (i in mMonsterList.indices) {
            mMonsterList[i].setOriginalCombatPos(i)
        }
    }

    override fun update(delta: Long) {
        mTimeCnt += delta
        when (mCombatState) {
            Combat.CombatState.SelectAction -> {
                if (!mHasEventExed && !sIsRandomFight) {
                    mHasEventExed = true
                    for (i in mEventRound!!.indices) {
                        if (mRoundCnt == mEventRound!![i] && mEventNum!![i] != 0) {
                            game.triggerEvent(mEventNum!![i])
                        }
                    }
                }
                if (mIsAutoAttack) { // 自动生成动作队列
                    generateAutoActionQueue()
                    mCombatState = CombatState.PerformAction
                } else { // 玩家决策
                    mCombatUI.update(delta)
                }
            }

            Combat.CombatState.PerformAction -> if (!mActionExecutor.update(delta)) { // 动作执行完毕
                if (isAllMonsterDead) { // 怪物全挂
                    mTimeCnt = 0 // 计时器清零
                    mCombatState = CombatState.Win

                    Player.sMoney += mWinMoney // 获得金钱
                    val lvuplist = mutableListOf<Player>()
                    for (p in mPlayerList) { // 获得经验
                        if (p.isAlive) {
                            // 新版魔塔，最大的等级为0，只需要添加exp即可
                            if (p.levelupChain.maxLevel <= 0) {
                                val nextExp = p.levelupChain.getNextLevelExp(p.level)
                                val exp = mWinExp + p.currentExp
                                p.currentExp = exp
                                continue
                            }
                            if (p.level >= p.levelupChain.maxLevel)
                            // 满级
                                continue
                            val nextExp = p.levelupChain.getNextLevelExp(p.level)
                            val exp = mWinExp + p.currentExp
                            if (exp < nextExp) {
                                p.currentExp = exp
                            } else { // 升级
                                val cl = p.level // 当前等级
                                p.currentExp = exp - nextExp
                                println("[Combat] ${p.name} 战斗升级: Level $cl -> ${cl + 1}, 剩余经验: ${p.currentExp}")
                                // 使用统一的 levelUp 方法，确保逻辑一致
                                p.levelUp(cl + 1)
                                lvuplist.add(p)
                            }
                        }
                    }

                    // 最大幸运值
                    var ppt = 10
                    for (p in mPlayerList) {
                        if (p.luck > ppt) {
                            ppt = p.luck
                        }
                    }
                    ppt -= 10
                    if (ppt > 100) {
                        ppt = 100
                    } else if (ppt < 0) {
                        ppt = 10
                    }

                    // 战利品链表
                    val gm = GoodsManage()
                    val gl = mutableListOf<BaseGoods>()

                    // 获取物品掉落倍数（独立控制）
                    val itemMultiple = sysGetWinItemMultiple()

                    for (m in mMonsterList) {
                        val g = m.dropGoods
                        if (g != null && sRandom.nextInt(101) < ppt) { //  ppt%掉率
                            val finalGoodsNum = g.goodsNum * itemMultiple
                            gm.addGoods(g.type, g.index, finalGoodsNum)
                            Player.sGoodsList.addGoods(g.type, g.index, finalGoodsNum) // 添加到物品链表
                        }
                    }
                    gl.addAll(gm.goodsList)
                    gl.addAll(gm.equipList)
                    mCombatSuccess = CombatSuccess(this, mWinExp, mWinMoney, gl, lvuplist) // 显示玩家的收获
                } else { // 还有怪物存活
                    if (isAnyPlayerAlive) { // 有玩家角色没挂，继续打怪
                        ++mRoundCnt
                        
                        // 检查是否达到最大回合数（参考C代码的逻辑）
                        if (mMaxRound > 0 && mRoundCnt >= mMaxRound) {
                            println("[Combat] Max rounds reached ($mRoundCnt/$mMaxRound), ending combat")
                            // 达到最大回合数，不算失败，但需要结束战斗
                            // 根据C代码，这种情况应该继续执行脚本
                            mIsWin = false  // 不算胜利
                            mCombatState = CombatState.Exit  // 直接退出，让exitCurrentCombat处理
                        } else {
                            // 继续下一回合
                            mHasEventExed = false  // 重置事件标志，允许新回合触发事件
                            updateFighterState()
                            mCombatState = CombatState.SelectAction
                            mCurSelActionPlayerIndex = firstAlivePlayerIndex
                            mCombatUI.setCurrentPlayerIndex(mCurSelActionPlayerIndex)
                            for (p in mPlayerList) {
                                p.setFrameByState()
                            }
                        }
                    } else { // 玩家角色全挂，战斗失败
                        mTimeCnt = 0
                        mCombatState = CombatState.Loss
                    }
                }
            }

            Combat.CombatState.Win -> {
                mIsWin = true
                if (mCombatSuccess?.update(delta) ?: true) {
                    mCombatState = CombatState.Exit
                }
            }

            Combat.CombatState.Loss ->
                if (sIsRandomFight && mFlyPeach.update(delta)) {

                } else {
                    mIsWin = false
                    mCombatState = CombatState.Exit
                }

            Combat.CombatState.Exit -> exitCurrentCombat()
        }
    }

    override fun draw(canvas: Canvas) {
        canvas.drawBitmap(mBackground, 0, 0)

        // draw the monsters and players
        for (i in mMonsterList.indices) {
            val fc = mMonsterList[i]
            if (fc.isVisiable) {
                fc.fightingSprite!!.draw(canvas)
            }
        }

        for (i in mPlayerList.indices.reversed()) {
            val f = mPlayerList[i].fightingSprite!!
            f.draw(canvas)
        }

        if (mCombatState == CombatState.SelectAction && !mIsAutoAttack) {
            mCombatUI.draw(canvas)
        } else if (mCombatState == CombatState.PerformAction) {
            mActionExecutor.draw(canvas)
        } else if (mCombatState == CombatState.Win) {
            //			TextRender.drawText(canvas, "Win", 20, 40);
            mCombatSuccess?.draw(canvas)
        } else if (mCombatState == CombatState.Loss && sIsRandomFight) {
            //			TextRender.drawText(canvas, "Loss", 20, 40);
            mFlyPeach.draw(canvas, 0, 0)
        }
    }

    override fun onKeyDown(key: Int) {
        if (mCombatState == CombatState.SelectAction) {
            if (!mIsAutoAttack) {
                if (key == Global.KEY_REPEAT) {
                    handleRepeatAction()
                } else {
                    mCombatUI.onKeyDown(key)
                }
            }
        } else if (mCombatState == CombatState.Win) {
            mCombatSuccess?.onKeyDown(key)
        }
    }

    override fun onKeyUp(key: Int) {
        if (mCombatState == CombatState.SelectAction) {
            if (!mIsAutoAttack) {
                mCombatUI.onKeyUp(key)
            }
        } else if (mCombatState == CombatState.Win) {
            mCombatSuccess?.onKeyUp(key)
        }

        if (mIsAutoAttack && key == Global.KEY_CANCEL) { // 退出“围攻”模式
            mIsAutoAttack = false
        }
    }

    private fun generateAutoActionQueue() {
        val monster = firstAliveMonster

        mActionQueue.clear()

        // 玩家的Action
        for (p in mPlayerList) {
            if (p.isAlive) {
                mActionQueue.add(if (p.hasAtbuff(FightingCharacter.BUFF_MASK_ALL))
                    ActionPhysicalAttackAll(p, mMonsterList)
                else
                    ActionPhysicalAttackOne(p, monster!!))
            }
        }

        // 怪物的Action
        generateMonstersActions()

        sortActionQueue()
    }

    /**
     * 基于C引擎文档的敌人魔法释放决策算法
     * 根据useOriginalDamageFormula切换原版/简化算法
     */
    private fun generateMonstersActions() {
        val liveMonsters = mMonsterList.filter { it.isAlive }
        for (m in liveMonsters) {
            val p = randomAlivePlayer ?: return
            
            if (GameSettings.useOriginalDamageFormula) {
                // 原版C引擎算法
                generateMonsterActionOriginal(m, p)
            } else {
                // 简化算法（保持现有逻辑）
                generateMonsterActionSimplified(m, p)
            }
        }
    }
    
    /**
     * 原版C引擎敌人魔法释放算法
     */
    private fun generateMonsterActionOriginal(monster: Monster, targetPlayer: Player) {
        // 1. 检查前置条件：敌人必须活着且未被封印
        if (!monster.isAlive || monster.isSealed) {
            // 被封印时只能物理攻击或防御
            addPhysicalAttackAction(monster, targetPlayer)
            return
        }
        
        // 2. 获取敌人智商并计算魔法释放概率
        // 原版公式：智商 = 原始智商值
        var intelligence = if (monster.mIQ < 80) {
            monster.mIQ
        } else {
            80 + (monster.mIQ - 80) / 10
        }
        
        // 3. 生成0-99随机数进行魔法释放判定
        val random = sRandom.nextInt(100)
        
        // 4. 魔法释放判定
        if (random < intelligence) {
            // 选择释放魔法
            val selectedMagic = selectMagicToUse(monster)
            if (selectedMagic != null) {
                // 魔法命中率检测
                if (calculateMagicHitChance(monster, targetPlayer)) {
                    // 创建魔法攻击动作
                    createMagicAction(monster, targetPlayer, selectedMagic)
                    return
                }
            }
        }
        
        // 5. 回退到物理攻击
        addPhysicalAttackAction(monster, targetPlayer)
    }
    
    /**
     * 魔法选择算法（基于C引擎）
     */
    private fun selectMagicToUse(monster: Monster): fmj.magic.BaseMagic? {
        val magicChain = monster.magicChain ?: return null
        
        // 安全获取学会的魔法列表
        val learnedMagics = try {
            magicChain.getAllLearntMagics(true)
        } catch (e: Exception) {
            println("Error getting learned magics for monster ${monster.name}: ${e.message}")
            return null
        }
        
        if (learnedMagics.isEmpty()) return null
        
        // 过滤可使用的魔法（MP足够）
        val availableMagics = learnedMagics.filter { magic ->
            try {
                monster.mp >= magic.costMp
            } catch (e: Exception) {
                println("Error checking MP cost for magic: ${e.message}")
                false
            }
        }
        
        if (availableMagics.isEmpty()) return null
        
        // 随机选择一个可用魔法
        return try {
            availableMagics[sRandom.nextInt(availableMagics.size)]
        } catch (e: Exception) {
            println("Error selecting random magic: ${e.message}")
            null
        }
    }
    
    /**
     * 魔法命中率计算（基于C引擎公式）
     */
    private fun calculateMagicHitChance(attacker: Monster, defender: Player): Boolean {
        // 敌人身法 = 敌人身法（无增减益系统，使用原始值）
        val enemyAgility = attacker.speed
        
        // 玩家身法 = 玩家身法 + 50（基础加成）
        val playerAgility = defender.speed + 50
        
        // 身法差值计算
        val agilityDiff = if (enemyAgility > playerAgility) {
            enemyAgility - playerAgility
        } else {
            10 // 最小命中率保底
        }
        
        // 命中判定：random % 200 < agilityDiff
        val hitRoll = sRandom.nextInt(200)
        return hitRoll < agilityDiff
    }
    
    /**
     * 创建魔法攻击动作
     */
    private fun createMagicAction(caster: Monster, target: Player, magic: fmj.magic.BaseMagic) {
        when (magic) {
            is fmj.magic.MagicAttack -> {
                if (magic.isForAll) {
                    mActionQueue.add(ActionMagicAttackAll(caster, mPlayerList, magic))
                } else {
                    mActionQueue.add(ActionMagicAttackOne(caster, target, magic))
                }
            }
            is fmj.magic.MagicRestore -> {
                if (magic.isForAll) {
                    mActionQueue.add(ActionMagicHelpAll(caster, mMonsterList, magic))
                } else {
                    // 选择血量最少的存活怪物作为治疗目标
                    val healTarget = mMonsterList.filter { it.isAlive }.minByOrNull { it.hp } ?: caster
                    mActionQueue.add(ActionMagicHelpOne(caster, healTarget, magic))
                }
            }
            else -> {
                // 其他类型魔法，使用默认处理
                if (magic is fmj.magic.MagicAttack) {
                    if (magic.isForAll) {
                        mActionQueue.add(ActionMagicAttackAll(caster, mPlayerList, magic))
                    } else {
                        mActionQueue.add(ActionMagicAttackOne(caster, target, magic))
                    }
                } else {
                    println("Warning: Expected MagicAttack but got ${magic::class.simpleName}, skipping magic action")
                }
            }
        }
    }
    
    /**
     * 简化算法（原有逻辑）
     */
    private fun generateMonsterActionSimplified(monster: Monster, targetPlayer: Player) {
        val iq = monster.mIQ.toDouble() / 100.0
        
        // 安全获取魔法列表
        val magics = try {
            monster.magicChain?.getAllLearntMagics(true)?.filter {
                try {
                    it.costMp <= monster.mp
                } catch (e: Exception) {
                    println("Error checking MP cost in simplified algorithm: ${e.message}")
                    false
                }
            }
        } catch (e: Exception) {
            println("Error getting magics in simplified algorithm: ${e.message}")
            null
        }
        
        if (!monster.isSealed && magics != null && magics.isNotEmpty()) {
            val dying = monster.maxHP / monster.hp > 3

            val restoreMagic = magics.firstOrNull { it is MagicRestore }
            val attackMagic = magics.firstOrNull {
                it is MagicAttack
            } as? MagicAttack

            if (dying && restoreMagic != null && random() < sqrt(iq)) {
                if (restoreMagic.isForAll) {
                    mActionQueue.add(ActionMagicHelpAll(monster, mMonsterList, restoreMagic))
                } else {
                    mActionQueue.add(ActionMagicHelpOne(monster, monster, restoreMagic))
                }
                return
            }
            
            if (attackMagic != null && random() < iq) {
                if (attackMagic.isForAll) {
                    mActionQueue.add(ActionMagicAttackAll(monster, mPlayerList, attackMagic))
                } else {
                    mActionQueue.add(ActionMagicAttackOne(monster, targetPlayer, attackMagic))
                }
                return
            }
        }
        
        addPhysicalAttackAction(monster, targetPlayer)
    }
    
    /**
     * 添加物理攻击动作
     */
    private fun addPhysicalAttackAction(monster: Monster, targetPlayer: Player) {
        mActionQueue.add(if (monster.hasAtbuff(FightingCharacter.BUFF_MASK_ALL))
            ActionPhysicalAttackAll(monster, mPlayerList)
        else
            ActionPhysicalAttackOne(monster, targetPlayer))
    }

    /** 按敏捷从大到小排列 */
    private fun sortActionQueue() {
        mActionQueue.sortByDescending { it.priority }
    }

    /** index 之后的主角是否都挂 */
    private fun isPlayerBehindDead(index: Int): Boolean {
        return (index + 1 until mPlayerList.size)
                .none { mPlayerList[it].isAlive }
    }

    /** 更新双方状态 */
    private fun updateFighterState() {
    }

    override fun onActionSelected(action: Action) {
        // 记录当前玩家的动作用于重复功能
        mLastPlayerActions[mCurSelActionPlayerIndex] = action
        // 同步更新全局动作记录
        sGlobalPlayerActions[mCurSelActionPlayerIndex] = action
        println("[合击调试] onActionSelected - 玩家${mCurSelActionPlayerIndex}选择了动作: ${action::class.simpleName}")
        
        mActionQueue.add(action)

        mCombatUI.reset() // 重置战斗UI

        fun go() {
            generateMonstersActions()
            sortActionQueue()
            mCombatState = CombatState.PerformAction // 开始执行动作队列
        }

        if (action is ActionCoopMagic) { // 只保留合击
            println("[合击调试] 检测到合体技能，清空队列并只保留合击")
            mActionQueue.clear()
            mActionQueue.add(action)
            // 为所有参与合击的玩家记录这个动作
            action.mActors.forEach { player ->
                val playerIndex = mPlayerList.indexOf(player)
                if (playerIndex >= 0) {
                    mLastPlayerActions[playerIndex] = action
                    // 同步更新全局动作记录
                    sGlobalPlayerActions[playerIndex] = action
                    println("[合击调试] 为玩家${playerIndex}(${player.name})记录合击动作")
                }
            }
            go()
        } else if (mCurSelActionPlayerIndex >= mPlayerList.size - 1 || isPlayerBehindDead(mCurSelActionPlayerIndex)) { // 全部玩家角色的动作选择完成
            go()
        } else { // 选择下一个玩家角色的动作
            val nextIndex = nextAlivePlayerIndex
            mCurSelActionPlayerIndex = nextIndex
            mCombatUI.setCurrentPlayerIndex(nextIndex)
        }
    }

    override fun onAutoAttack() {
        // clear all the actions that has been selected, enter into auto fight mode
        mCombatUI.reset()
        mActionQueue.clear()
        mIsAutoAttack = true
        mCombatState = CombatState.SelectAction
    }

    override fun onFlee() {
        mCombatUI.reset() // 重置战斗UI

        for (i in mCurSelActionPlayerIndex until mPlayerList.size) {
            if (mPlayerList[i].isAlive && sRandom.nextBoolean() && sIsRandomFight) { // 50% 逃走
                mActionQueue.add(ActionFlee(mPlayerList[i], true, object : Runnable {

                    override fun run() {
                        // 逃跑成功后执行
                        mIsWin = true
                        mCombatState = CombatState.Exit
                    }
                }))
                break
            } else { // 逃跑失败
                mActionQueue.add(ActionFlee(mPlayerList[i], false, null))
            }
        }
        generateMonstersActions()
        sortActionQueue()
        mCombatState = CombatState.PerformAction
    }

    override fun onCancel() {
        val i = preAlivePlayerIndex
        if (i >= 0) { // 不是第一个角色
            // 重选上一个角色的动作
            mActionQueue.removeAt(mActionQueue.size-1).cancel()
            mCurSelActionPlayerIndex = i
            mCombatUI.setCurrentPlayerIndex(mCurSelActionPlayerIndex)
            if (mCurSelActionPlayerIndex > 0) {
                val p = mPlayerList[mCurSelActionPlayerIndex]
                if (p.isSleeping || p.isConfusing) {
                    onCancel()
                }
            }
            mCombatUI.reset()
        }
    }

    /** 
     * 尝试为玩家重复上次的动作
     * @param player 执行动作的玩家
     * @param lastAction 上次执行的动作
     * @return 可执行的动作，如果条件不满足则返回null
     */
    private fun tryRepeatAction(player: Player, lastAction: Action?): Action? {
        if (lastAction == null) return null
        
        val hasAliveEnemies = mMonsterList.any { it.isAlive }
        
        return when (lastAction) {
            // 1. 普通攻击
            is ActionPhysicalAttackOne -> {
                if (hasAliveEnemies) {
                    firstAliveMonster?.let { ActionPhysicalAttackOne(player, it) }
                } else null
            }
            
            // 2. 全体攻击（围攻）
            is ActionPhysicalAttackAll -> {
                if (hasAliveEnemies && player.hasAtbuff(FightingCharacter.BUFF_MASK_ALL)) {
                    ActionPhysicalAttackAll(player, mMonsterList)
                } else null
            }
            
            // 3. 单体魔法攻击
            is ActionMagicAttackOne -> {
                val magic = lastAction.magic
                if (hasAliveEnemies && player.mp >= magic.costMp && !player.isSealed) {
                    firstAliveMonster?.let { ActionMagicAttackOne(player, it, magic) }
                } else null
            }
            
            // 4. 全体魔法攻击
            is ActionMagicAttackAll -> {
                val magic = lastAction.magic
                if (hasAliveEnemies && player.mp >= magic.costMp && !player.isSealed) {
                    ActionMagicAttackAll(player, mMonsterList, magic)
                } else null
            }
            
            // 5. 单体辅助魔法
            is ActionMagicHelpOne -> {
                val magic = lastAction.magic
                if (player.mp >= magic.costMp && !player.isSealed) {
                    // 选择血量最少的存活玩家
                    mPlayerList.filter { it.isAlive }.minByOrNull { it.hp }?.let {
                        ActionMagicHelpOne(player, it, magic)
                    }
                } else null
            }
            
            // 6. 全体辅助魔法
            is ActionMagicHelpAll -> {
                val magic = lastAction.magic
                if (player.mp >= magic.costMp && !player.isSealed) {
                    ActionMagicHelpAll(player, mPlayerList, magic)
                } else null
            }
            
            // 7. 使用单体道具
            is ActionUseItemOne -> {
                val item = lastAction.goods
                if (Player.sGoodsList.getGoodsNum(item.type, item.index) > 0) {
                    // 判断是否为复活药物
                    val isRevivalMedicine = item.type == 10 // GoodsMedicineLife
                    
                    val target = if (isRevivalMedicine) {
                        // 复活药物：优先选择阵亡角色
                        mPlayerList.firstOrNull { !it.isAlive }
                            ?: mPlayerList.filter { it.isAlive }.minByOrNull { it.hp }
                    } else {
                        // 普通药物：选择存活的血少角色
                        mPlayerList.filter { it.isAlive }.minByOrNull { it.hp }
                    }
                    
                    target?.let {
                        Player.sGoodsList.useGoodsNum(item.type, item.index, 1)
                        ActionUseItemOne(player, it, item)
                    }
                } else null
            }
            
            // 8. 使用全体道具
            is ActionUseItemAll -> {
                val item = lastAction.goods
                if (Player.sGoodsList.getGoodsNum(item.type, item.index) > 0) {
                    Player.sGoodsList.useGoodsNum(item.type, item.index, 1)
                    ActionUseItemAll(player, mPlayerList, item)
                } else null
            }
            
            // 9. 投掷单体道具
            is ActionThrowItemOne -> {
                val item = lastAction.weapon
                if (hasAliveEnemies && item is BaseGoods && item is Throwable &&
                    Player.sGoodsList.getGoodsNum(item.type, item.index) > 0) {
                    firstAliveMonster?.let {
                        Player.sGoodsList.useGoodsNum(item.type, item.index, 1)
                        ActionThrowItemOne(player, it, item)
                    }
                } else null
            }

            // 10. 投掷全体道具
            is ActionThrowItemAll -> {
                val item = lastAction.weapon
                if (hasAliveEnemies && item is BaseGoods && item is Throwable &&
                    Player.sGoodsList.getGoodsNum(item.type, item.index) > 0) {
                    Player.sGoodsList.useGoodsNum(item.type, item.index, 1)
                    ActionThrowItemAll(player, mMonsterList, item)
                } else null
            }
            
            // 11. 防御
            is ActionDefend -> {
                ActionDefend(player)
            }
            
            // 12. 逃跑
            is ActionFlee -> {
                // 逃跑需要特殊处理，这里暂时不重复逃跑动作
                null
            }
            
            // 13. 合体技能（已在主函数中单独处理）
            is ActionCoopMagic -> null
            
            // 其他动作类型
            else -> null
        }
    }
    
    /** 处理重复上次动作的按键 - 支持多玩家一次性执行 */
    private fun handleRepeatAction() {
        println("[R键调试] ========== handleRepeatAction 开始 ==========")
        println("[R键调试] 当前战斗状态: $mCombatState")
        
        // 输出所有玩家的上次动作记录
        println("[R键调试] 所有玩家的上次动作记录:")
        mPlayerList.forEachIndexed { index, player ->
            val lastAction = mLastPlayerActions[index]
            println("[R键调试]   玩家$index(${player.name}): ${lastAction?.let { it::class.simpleName } ?: "无记录"}")
        }
        
        // 首先检查是否有合体技能需要重复
        val alivePlayersForCoop = mPlayerList.filter { it.isAlive && !it.isSleeping && !it.isConfusing }
        println("[R键调试] 活着且正常的玩家数量: ${alivePlayersForCoop.size}")
        
        if (alivePlayersForCoop.isNotEmpty()) {
            // 检查第一个玩家的上次动作是否为合体技能
            val firstPlayerIndex = mPlayerList.indexOf(alivePlayersForCoop[0])
            val firstPlayerLastAction = mLastPlayerActions[firstPlayerIndex]
            println("[R键调试] 第一个玩家索引: $firstPlayerIndex")
            println("[R键调试] 第一个玩家的上次动作: ${firstPlayerLastAction?.let { it::class.simpleName } ?: "null"}")
            
            if (firstPlayerLastAction is ActionCoopMagic && alivePlayersForCoop.size >= 2) {
                println("[R键调试] 检测到上次是合体技能，且有足够玩家")
                // 检查第一个玩家是否有合体技能装备
                val firstPlayer = alivePlayersForCoop[0]
                val decoration = firstPlayer.equipmentsArray[0] as? fmj.goods.GoodsDecorations
                val coopMagic = decoration?.coopMagic
                println("[R键调试] 装备的合体技能: ${coopMagic?.magicName ?: "null"}")
                
                // 检查是否需要MP（只有有合体魔法时才需要）
                val needMpCheck = coopMagic != null
                val hasEnoughMp = if (needMpCheck) {
                    alivePlayersForCoop.all { player ->
                        val enough = player.mp >= coopMagic!!.costMp
                        println("[R键调试] 玩家 ${player.name} MP: ${player.mp}/${coopMagic.costMp} 足够: $enough")
                        enough
                    }
                } else {
                    true // 普通合击不需要MP
                }
                
                val actionType = if (coopMagic != null) "合体技能" else "普通合击"
                println("[R键调试] 动作类型: $actionType, 需要MP检查: $needMpCheck, MP足够: $hasEnoughMp")
                
                if (hasEnoughMp) {
                    // 检查是否有活着的敌人
                    val hasAliveEnemies = mMonsterList.any { it.isAlive }
                    println("[R键调试] 有活着的敌人: $hasAliveEnemies")
                    
                    if (hasAliveEnemies) {
                        // 重复合体攻击（魔法或物理）
                        val isSingleTarget = firstPlayerLastAction.isSingleTarget
                        println("[R键调试] ${actionType}是单体: $isSingleTarget")
                        
                        val coopAction = if (isSingleTarget) {
                            val target = firstAliveMonster
                            println("[R键调试] 单体目标: ${target?.name ?: "null"}")
                            if (target != null) {
                                ActionCoopMagic(alivePlayersForCoop, target)
                            } else null
                        } else {
                            val aliveMonsters = mMonsterList.filter { it.isAlive }
                            println("[R键调试] 群体目标数量: ${aliveMonsters.size}")
                            if (aliveMonsters.isNotEmpty()) {
                                ActionCoopMagic(alivePlayersForCoop, aliveMonsters)
                            } else null
                        }
                        
                        println("[R键调试] 创建的${actionType}动作: ${coopAction?.let { it::class.simpleName } ?: "null"}")
                        if (coopAction != null) {
                            mActionQueue.add(coopAction)
                            println("[R键调试] ${actionType}动作已加入队列")
                            
                            // 为所有参与的玩家记录这次合体动作
                            alivePlayersForCoop.forEach { player ->
                                val playerIndex = mPlayerList.indexOf(player)
                                mLastPlayerActions[playerIndex] = coopAction
                                // 同步更新全局动作记录
                                sGlobalPlayerActions[playerIndex] = coopAction
                            }
                            
                            // 生成怪物动作并进入执行阶段
                            generateMonstersActions()
                            sortActionQueue()
                            mCombatState = CombatState.PerformAction
                            println("[R键调试] 战斗状态切换为 PerformAction")
                            return
                        }
                    } else {
                        println("[R键调试] 没有活着的敌人，跳过${actionType}")
                    }
                } else {
                    println("[R键调试] MP不足，无法执行${actionType}")
                }
            }
        }
        
        // 如果不是合体技能，为所有活着的玩家创建重复动作
        val playersNeedingActions = mPlayerList.filter { it.isAlive && !it.isSleeping && !it.isConfusing }
        
        if (playersNeedingActions.isEmpty()) {
            println("[R键调试] 没有可行动的玩家")
            return
        }
        
        // 检查是否有活着的敌人
        val hasAliveEnemies = mMonsterList.any { it.isAlive }
        
        // 为每个玩家创建动作
        for (player in playersNeedingActions) {
            val playerIndex = mPlayerList.indexOf(player)
            val lastAction = mLastPlayerActions[playerIndex]
            
            println("[R键调试] 处理玩家 ${player.name}，上次动作: ${lastAction?.let { it::class.simpleName } ?: "无"}")
            
            // 尝试重复上次的动作（排除合体技能，已在上面处理）
            var repeatedAction: Action? = null
            
            if (lastAction != null && lastAction !is ActionCoopMagic) {
                repeatedAction = tryRepeatAction(player, lastAction)
                
                if (repeatedAction != null) {
                    println("[R键调试] 成功重复动作: ${repeatedAction::class.simpleName}")
                } else {
                    println("[R键调试] 无法重复上次动作，条件不满足")
                }
            }
            
            // 如果无法重复上次动作，尝试执行普通攻击作为后备方案
            if (repeatedAction == null && hasAliveEnemies) {
                val target = firstAliveMonster
                if (target != null) {
                    repeatedAction = ActionPhysicalAttackOne(player, target)
                    println("[R键调试] 回退到普通攻击: ${target.name}")
                }
            }
            
            // 如果没有敌人，尝试防御
            if (repeatedAction == null && !hasAliveEnemies) {
                repeatedAction = ActionDefend(player)
                println("[R键调试] 没有敌人，执行防御")
            }
            
            // 如果成功创建了动作，加入到动作队列中
            if (repeatedAction != null) {
                mActionQueue.add(repeatedAction)
                // 记录这次动作作为该玩家的最新动作
                mLastPlayerActions[playerIndex] = repeatedAction
                // 同步更新全局动作记录
                sGlobalPlayerActions[playerIndex] = repeatedAction
                println("[R键调试] 动作已加入队列并记录")
            } else {
                println("[R键调试] 警告：无法为玩家 ${player.name} 创建任何动作")
            }
        }
        
        // 如果成功为所有玩家创建了动作，生成怪物动作并进入执行阶段
        if (!mActionQueue.isEmpty()) {
            println("[R键调试] 动作队列不为空，队列大小: ${mActionQueue.size}")
            generateMonstersActions()  // 生成怪物行动
            sortActionQueue()          // 排序动作队列
            mCombatState = CombatState.PerformAction
            println("[R键调试] 切换到执行状态")
        } else {
            println("[R键调试] 动作队列为空，无法执行重复动作")
        }
        println("[R键调试] ========== handleRepeatAction 结束 ==========")
    }

    companion object {
        
        /** 战斗中最多允许的玩家数量 */
        const val MAX_COMBAT_PLAYERS = 3

        private var sIsEnable: Boolean = false
        private var globalDisableFighting: Boolean = false
        private var sIsFighting: Boolean = false

        private var sInstance: Combat? = null
        private var sInstanceBk: Combat? = null

        private var sIsRandomFight: Boolean = false
        private var sEnterFightScriptIndex: Int = -1  // 记录进入战斗时的脚本索引
        
        // R键功能：静态存储玩家动作历史，跨战斗保持
        private val sGlobalPlayerActions = mutableMapOf<Int, Action>()
        
        fun setEnterFightScriptIndex(index: Int) {
            sEnterFightScriptIndex = index
            println("[Combat] EnterFight script index set to: $index")
        }

        fun IsActive(): Boolean {
            return sIsEnable && sInstance != null && sIsFighting
        }

        /**
         * 开启随即战斗
         */
        fun FightEnable() {
            sIsEnable = true
        }

        /**
         * 关闭随即战斗
         */
        fun FightDisable() {
            sIsEnable = false
            if (sInstance != null) {
                sInstance = null
            }
        }

        /**
         * 初始化并开启随即战斗
         * @param monstersType 0-7 可能出现的敌人种类
         * @param scrb 战斗背景
         * @param scrl 左下角图
         * @param scrr 右上角图
         */
        fun InitFight(parent: GameNode, monstersType: IntArray, scrb: Int, scrl: Int, scrr: Int) {
            sIsEnable = true
            sIsRandomFight = true
            sIsFighting = false

            val instance = Combat(parent)
            sInstance = instance
            sInstanceBk = null

            instance.mMonsterType = monstersType.filter { it != 0 }
                    .toIntArray()
            instance.mRoundCnt = 0
            instance.mMaxRound = 0 // 回合数无限制
            instance.createBackgroundBitmap(scrb, scrl, scrr)
        }

        fun write(out: ObjectOutput) {
            val instance = sInstance
            val fightEnabled = sIsEnable && instance != null
            out.writeBoolean(fightEnabled)
            if (sIsEnable && instance != null) {
                out.writeIntArray(instance.mMonsterType)
                out.writeInt(instance.mScrb)
                out.writeInt(instance.mScrl)
                out.writeInt(instance.mScrR)
            }
        }

        fun read(parent: GameNode, coder: ObjectInput) {
            sIsEnable = coder.readBoolean()
            if (sIsEnable) {
                val monsterType = coder.readIntArray()
                val scrb = coder.readInt()
                val scrl = coder.readInt()
                val scrr = coder.readInt()
                InitFight(parent, monsterType, scrb, scrl, scrr)
            }
        }

        /**
         * 剧情战斗
         * @param roundMax 最多回合数，0为无限
         * @param monstersType 0-3 敌人
         * @param scr 0战斗背景，1左下角图，2右上角图
         * @param evtRnds 0-3 战斗中，触发事件的回合
         * @param evts 0-3 对应的事件号
         * @param lossto 战斗失败跳转的地址
         * @param winto 战斗成功跳转的地址
         */
        fun EnterFight(parent: GameNode, roundMax: Int, monstersType: IntArray, scr: IntArray, evtRnds: IntArray, evts: IntArray, lossto: Int, winto: Int) {
            sIsRandomFight = false

            sInstanceBk = sInstance // 保存当前随机战斗的引用
            val instance = Combat(parent)
            sInstance = instance

            instance.mMonsterList = mutableListOf()
            monstersType.indices
                    .filter { monstersType[it] > 0 }
                    .mapNotNull { 
                        val res = DatLib.getRes(DatLib.ResType.ARS, 3, monstersType[it])
                        if (res is Monster) {
                            res
                        } else {
                            println("Warning: Combat monster type mismatch at index $it - expected Monster but got ${res?.let { it::class.simpleName }}")
                            null
                        }
                    }
                    .forEach { instance.mMonsterList.add(it) }

            instance.mMaxRound = roundMax
            instance.mRoundCnt = 0

            prepareForNewCombat(instance)

            instance.createBackgroundBitmap(scr[0], scr[1], scr[2])

            instance.mEventRound = evtRnds
            instance.mEventNum = evts

            instance.mLossAddr = lossto
            instance.mWinAddr = winto
        }

        private fun prepareForNewCombat(combat: Combat) {
            sIsEnable = true
            sIsFighting = true
            combat.prepareForNewCombat()
        }

        private var COMBAT_PROBABILITY = 20
        private val sRandom = Random()

        /**
         * 进入一个随机战斗
         * @return `true`新战斗 `false`不开始战斗
         */
        fun StartNewRandomCombat(): Boolean {
            val instance = sInstance

            val combatProbability = sysGetCombatProbability()
            if (combatProbability > 0) {
                COMBAT_PROBABILITY = combatProbability
            }

            if (globalDisableFighting
                    || !sIsEnable
                    || instance == null
                    || instance.mMonsterType.isEmpty()
                    || sRandom.nextInt(COMBAT_PROBABILITY) != 0) {
                sIsFighting = false
                return false
            }

            // 随机添加怪物
            instance.mMonsterList.clear()
            val i = sRandom.nextInt(3)
            (0..i).forEach {
                val x = sRandom.nextInt(instance.mMonsterType.size)
                val res = DatLib.getRes(DatLib.ResType.ARS, 3, instance.mMonsterType[x])
                if (res is Monster) {
                    instance.mMonsterList.add(res)
                } else {
                    println("Warning: Random combat monster type mismatch - expected Monster but got ${res?.let { it::class.simpleName }}")
                }
            }

            instance.mRoundCnt = 0
            instance.mMaxRound = 0 // 回合不限

            prepareForNewCombat(instance)

            return true
        }

        fun Update(delta: Long) {
            sInstance?.update(delta)
        }

        fun Draw(canvas: Canvas) {
            sInstance?.draw(canvas)
        }

        fun KeyDown(key: Int) {
            sInstance?.onKeyDown(key)
        }

        fun KeyUp(key: Int) {
            sInstance?.onKeyUp(key)
        }

        /** 玩家角色中心坐标 */
        val sPlayerPos = arrayOf(
                Point(64 + 12 + (Global.SCREEN_WIDTH - 160) / 2 + 30, 52 + 18 + (Global.SCREEN_HEIGHT - 96) / 2 + 30),
                Point(96 + 12 + (Global.SCREEN_WIDTH - 160) / 2 + 30, 48 + 18 + (Global.SCREEN_HEIGHT - 96) / 2 + 30),
                Point(128 + 12 + (Global.SCREEN_WIDTH - 160) / 2 + 30, 40 + 18 + (Global.SCREEN_HEIGHT - 96) / 2 + 30))

        fun ForceWin() {
            sInstance?.mCombatState = CombatState.Win
        }

        /**
         * 动态添加Player到当前战斗中
         * 用于脚本中途通过cmd_createactor添加新角色的情况
         * @param player 要添加到战斗的玩家
         */
        fun addPlayerToCombat(player: Player) {
            val instance = sInstance
            if (instance == null) {
                println("[Combat.addPlayerToCombat] No active combat instance")
                return
            }

            // 检查是否超过最大参战人数
            if (instance.mPlayerList.size >= MAX_COMBAT_PLAYERS) {
                println("[Combat.addPlayerToCombat] Combat is full (${instance.mPlayerList.size}/$MAX_COMBAT_PLAYERS), cannot add ${player.name}")
                return
            }

            // 检查Player是否已经在战斗中
            if (instance.mPlayerList.any { it.index == player.index }) {
                println("[Combat.addPlayerToCombat] Player ${player.name} is already in combat")
                return
            }

            // 将Player添加到战斗列表（转为可变列表）
            val newPlayerList = instance.mPlayerList.toMutableList()
            newPlayerList.add(player)
            instance.mPlayerList = newPlayerList

            // 确保Player血量大于0
            if (player.hp <= 0) {
                player.hp = 1
            }

            // 重置Player的Debuff状态
            player.resetDebuff()

            // 设置Player的战斗位置
            val playerIndex = newPlayerList.size - 1
            if (playerIndex < sPlayerPos.size) {
                player.setCombatPos(sPlayerPos[playerIndex].x, sPlayerPos[playerIndex].y)
            } else {
                // 如果超过3个玩家，使用最后一个位置
                val lastPos = sPlayerPos[sPlayerPos.size - 1]
                player.setCombatPos(lastPos.x, lastPos.y)
            }

            // 更新战斗UI的玩家列表
            instance.mCombatUI.setPlayerList(instance.mPlayerList)

            // 设置Player的战斗帧状态
            player.setFrameByState()

            println("[Combat.addPlayerToCombat] Successfully added ${player.name} to combat (${instance.mPlayerList.size}/$MAX_COMBAT_PLAYERS)")
        }
    }
}
