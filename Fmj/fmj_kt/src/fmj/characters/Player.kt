package fmj.characters

import fmj.goods.GoodsEquipment
import fmj.goods.GoodsManage
import fmj.graphics.TextRender
import fmj.graphics.Util
import fmj.lib.DatLib
import fmj.lib.ResImage
import fmj.magic.BaseMagic

import graphics.Canvas

import java.Coder
import java.ObjectInput
import java.ObjectOutput
import kotlin.math.min


class Player : FightingCharacter(), Coder {

    private var mImgHead: ResImage? = null

    lateinit var levelupChain: ResLevelupChain
        private set

    var currentExp: Int = 0 // 当前经验值

    /**
     * 0装饰 1装饰 2护腕 3脚蹬 4手持 5身穿 6肩披 7头戴
     */
    val equipmentsArray = arrayOfNulls<GoodsEquipment>(8)

    // 不受限制的总属性值（用于正确处理装备加减）
    var totalMaxHP: Int = 0
    var totalMaxMP: Int = 0
    var totalAttack: Int = 0
    var totalDefend: Int = 0
    var totalSpeed: Int = 0
    var totalLingli: Int = 0
    var totalLuck: Int = 0

    var privateLearntMagics = arrayListOf<BaseMagic>()

    // 控制状态查看时的打印标志
    private var hasLoggedStateInfo = false

    // 记录上一次的属性值，用于变化检测
    private var lastHP = 0
    private var lastMP = 0
    private var lastMaxHP = 0
    private var lastMaxMP = 0
    private var lastAttack = 0
    private var lastDefend = 0
    private var lastSpeed = 0
    private var lastLingli = 0
    private var lastLuck = 0
    // 记录不受限制的总属性值
    private var lastTotalMaxHP = 0
    private var lastTotalMaxMP = 0
    private var lastTotalAttack = 0
    private var lastTotalDefend = 0
    private var lastTotalSpeed = 0
    private var lastTotalLingli = 0
    private var lastTotalLuck = 0

    /**
     * 记录属性变化日志
     * @param reason 变化原因
     */
    fun logAttributeChange(reason: String) {
        val hpChanged = hp != lastHP
        val mpChanged = mp != lastMP
        val maxHPChanged = maxHP != lastMaxHP
        val maxMPChanged = maxMP != lastMaxMP
        val attackChanged = attack != lastAttack
        val defendChanged = defend != lastDefend
        val speedChanged = speed != lastSpeed
        val lingliChanged = lingli != lastLingli
        val luckChanged = luck != lastLuck
        
        val totalMaxHPChanged = totalMaxHP != lastTotalMaxHP
        val totalMaxMPChanged = totalMaxMP != lastTotalMaxMP
        val totalAttackChanged = totalAttack != lastTotalAttack
        val totalDefendChanged = totalDefend != lastTotalDefend
        val totalSpeedChanged = totalSpeed != lastTotalSpeed
        val totalLingliChanged = totalLingli != lastTotalLingli
        val totalLuckChanged = totalLuck != lastTotalLuck

        if (hpChanged || mpChanged || maxHPChanged || maxMPChanged || 
            attackChanged || defendChanged || speedChanged || lingliChanged || luckChanged ||
            totalMaxHPChanged || totalMaxMPChanged || totalAttackChanged || totalDefendChanged ||
            totalSpeedChanged || totalLingliChanged || totalLuckChanged) {
            
            println("[PlayerStats] ===== ${this.name}: $reason =====")
            
            // 打印所有属性的前后对比
            println("[PlayerStats] Before: HP=$lastHP/$lastMaxHP, MP=$lastMP/$lastMaxMP")
            println("[PlayerStats] After:  HP=$hp/$maxHP, MP=$mp/$maxMP")
            
            println("[PlayerStats] Before: Attack=$lastAttack, Defend=$lastDefend, Speed=$lastSpeed")
            println("[PlayerStats] After:  Attack=$attack, Defend=$defend, Speed=$speed")
            
            println("[PlayerStats] Before: Lingli=$lastLingli, Luck=$lastLuck")
            println("[PlayerStats] After:  Lingli=$lingli, Luck=$luck")
            
            println("[PlayerStats] Before(Total): MaxHP=$lastTotalMaxHP, MaxMP=$lastTotalMaxMP")
            println("[PlayerStats] After(Total):  MaxHP=$totalMaxHP, MaxMP=$totalMaxMP")
            
            println("[PlayerStats] Before(Total): Attack=$lastTotalAttack, Defend=$lastTotalDefend")
            println("[PlayerStats] After(Total):  Attack=$totalAttack, Defend=$totalDefend")
            
            println("[PlayerStats] Before(Total): Speed=$lastTotalSpeed, Lingli=$lastTotalLingli, Luck=$lastTotalLuck")
            println("[PlayerStats] After(Total):  Speed=$totalSpeed, Lingli=$totalLingli, Luck=$totalLuck")
            
            // 显示变化量
            if (hpChanged || mpChanged || maxHPChanged || maxMPChanged) {
                println("[PlayerStats] 变化量: HP${if (hp != lastHP) " " + (if (hp > lastHP) "+" else "") + (hp - lastHP) else ""}, " +
                       "MP${if (mp != lastMP) " " + (if (mp > lastMP) "+" else "") + (mp - lastMP) else ""}, " +
                       "MaxHP${if (maxHP != lastMaxHP) " " + (if (maxHP > lastMaxHP) "+" else "") + (maxHP - lastMaxHP) else ""}, " +
                       "MaxMP${if (maxMP != lastMaxMP) " " + (if (maxMP > lastMaxMP) "+" else "") + (maxMP - lastMaxMP) else ""}")
            }
            
            if (attackChanged || defendChanged || speedChanged || lingliChanged || luckChanged) {
                println("[PlayerStats] 变化量: Attack${if (attackChanged) " " + (if (attack > lastAttack) "+" else "") + (attack - lastAttack) else ""}, " +
                       "Defend${if (defendChanged) " " + (if (defend > lastDefend) "+" else "") + (defend - lastDefend) else ""}, " +
                       "Speed${if (speedChanged) " " + (if (speed > lastSpeed) "+" else "") + (speed - lastSpeed) else ""}, " +
                       "Lingli${if (lingliChanged) " " + (if (lingli > lastLingli) "+" else "") + (lingli - lastLingli) else ""}, " +
                       "Luck${if (luckChanged) " " + (if (luck > lastLuck) "+" else "") + (luck - lastLuck) else ""}")
            }
            
            println("[PlayerStats] =====================================")

            updateLastAttributes()
        }
    }

    /**
     * 更新记录的上一次属性值
     */
    private fun updateLastAttributes() {
        lastHP = hp
        lastMP = mp
        lastMaxHP = maxHP
        lastMaxMP = maxMP
        lastAttack = attack
        lastDefend = defend
        lastSpeed = speed
        lastLingli = lingli
        lastLuck = luck
        
        // 更新总属性值记录
        lastTotalMaxHP = totalMaxHP
        lastTotalMaxMP = totalMaxMP
        lastTotalAttack = totalAttack
        lastTotalDefend = totalDefend
        lastTotalSpeed = totalSpeed
        lastTotalLingli = totalLingli
        lastTotalLuck = totalLuck
    }

    /**
     * 初始化属性记录（在角色加载后调用）
     */
    fun initAttributeTracking() {
        updateLastAttributes()
        println("[PlayerStats] ===== ${this.name}: 属性跟踪已初始化 =====")
        println("[PlayerStats] 初始状态: HP=$hp/$maxHP, MP=$mp/$maxMP")
        println("[PlayerStats] 初始状态: Attack=$attack, Defend=$defend, Speed=$speed")
        println("[PlayerStats] 初始状态: Lingli=$lingli, Luck=$luck")
        println("[PlayerStats] 初始状态(Total): MaxHP=$totalMaxHP, MaxMP=$totalMaxMP")
        println("[PlayerStats] 初始状态(Total): Attack=$totalAttack, Defend=$totalDefend")
        println("[PlayerStats] 初始状态(Total): Speed=$totalSpeed, Lingli=$totalLingli, Luck=$totalLuck")
        println("[PlayerStats] =======================================")
    }

    /**
     * 手动触发属性变化检测（用于外部修改HP/MP等情况）
     * @param reason 变化原因
     */
    fun checkAndLogAttributeChange(reason: String) {
        logAttributeChange(reason)
    }

    fun drawHead(canvas: Canvas, x: Int, y: Int) {
        if (mImgHead != null) {
            mImgHead!!.draw(canvas, 1, x, y)
        }
    }

    fun setFrameByState() {
        if (isAlive) {
            // 血量低于25%需要受伤状态
            if (isSleeping || hp < maxHP / 4) {
                fightingSprite!!.currentFrame = 11
            } else {
                fightingSprite!!.currentFrame = 1
            }
        } else {
            fightingSprite!!.currentFrame = 12
        }
    }

    override fun setData(buf: ByteArray, offset: Int) {
        type = buf[offset].toInt() and 0xFF
        index = buf[offset + 1].toInt() and 0xFF
        if (index > 0)
            mImgHead = DatLib.getRes(DatLib.ResType.PIC, 1, index) as ResImage?
        setWalkingSprite(WalkingSprite(type, buf[offset + 0x16].toInt() and 0xFF))
        fightingSprite = FightingSprite(DatLib.ResType.PIC, index)
        direction = Direction.fromInt(buf[offset + 2].toInt() and 0xFF)
        step = buf[offset + 3].toInt() and 0xff
        setPosInMap(buf[offset + 5].toInt() and 0xFF, buf[offset + 6].toInt() and 0xFF)
        val magicChainId = buf[offset + 0x17].toInt() and 0xff
        magicChain = DatLib.getMlr(1, magicChainId, true)
        magicChain?.learnNum = buf[offset + 9].toInt() and 0xff
        name = getString(buf, offset + 0x0a)
        level = buf[offset + 0x20].toInt() and 0xff
        maxHP = get2BytesInt(buf, offset + 0x26)
        hp = get2BytesInt(buf, offset + 0x28)
        maxMP = get2BytesInt(buf, offset + 0x2a)
        mp = get2BytesInt(buf, offset + 0x2c)
        attack = get2BytesInt(buf, offset + 0x2e)
        defend = get2BytesInt(buf, offset + 0x30)
        speed = buf[offset + 0x36].toInt() and 0xff
        lingli = buf[offset + 0x37].toInt() and 0xff
        luck = buf[offset + 0x38].toInt() and 0xff

        // 初始化总属性值（这里还包含了装备的加成，但我们暂时这样初始化）
        totalMaxHP = maxHP
        totalMaxMP = maxMP
        totalAttack = attack
        totalDefend = defend
        totalSpeed = speed
        totalLingli = lingli
        totalLuck = luck
        currentExp = get2BytesInt(buf, offset + 0x32)
        val levelupRes = DatLib.getRes(DatLib.ResType.MLR, 2, index, true)
        levelupChain = if (levelupRes is ResLevelupChain) levelupRes else ResLevelupChain()

        var tmp = buf[offset + 0x1e].toInt() and 0xff
        if (tmp != 0) {
            val equipment = DatLib.getRes(DatLib.ResType.GRS, 6, tmp)
            equipmentsArray[0] = if (equipment is GoodsEquipment) equipment else {
                println("Warning: Failed to load equipment[0] for player $name")
                null
            }
        }

        tmp = buf[offset + 0x1f].toInt() and 0xff
        if (tmp != 0) {
            val equipment = DatLib.getRes(DatLib.ResType.GRS, 6, tmp)
            equipmentsArray[1] = if (equipment is GoodsEquipment) equipment else {
                println("Warning: Failed to load equipment[1] for player $name")
                null
            }
        }

        tmp = buf[offset + 0x1b].toInt() and 0xff
        if (tmp != 0) {
            val equipment = DatLib.getRes(DatLib.ResType.GRS, 5, tmp)
            equipmentsArray[2] = if (equipment is GoodsEquipment) equipment else {
                println("Warning: Failed to load equipment[2] for player $name")
                null
            }
        }

        tmp = buf[offset + 0x1d].toInt() and 0xff
        if (tmp != 0) {
            val equipment = DatLib.getRes(DatLib.ResType.GRS, 3, tmp)
            equipmentsArray[3] = if (equipment is GoodsEquipment) equipment else {
                println("Warning: Failed to load equipment[3] for player $name")
                null
            }
        }

        tmp = buf[offset + 0x1c].toInt() and 0xff
        if (tmp != 0) {
            val equipment = DatLib.getRes(DatLib.ResType.GRS, 7, tmp)
            equipmentsArray[4] = if (equipment is GoodsEquipment) equipment else {
                println("Warning: Failed to load equipment[4] for player $name")
                null
            }
        }

        tmp = buf[offset + 0x19].toInt() and 0xff
        if (tmp != 0) {
            val equipment = DatLib.getRes(DatLib.ResType.GRS, 2, tmp)
            equipmentsArray[5] = if (equipment is GoodsEquipment) equipment else {
                println("Warning: Failed to load equipment[5] for player $name")
                null
            }
        }

        tmp = buf[offset + 0x1a].toInt() and 0xff
        if (tmp != 0) {
            val equipment = DatLib.getRes(DatLib.ResType.GRS, 4, tmp)
            equipmentsArray[6] = if (equipment is GoodsEquipment) equipment else {
                println("Warning: Failed to load equipment[6] for player $name")
                null
            }
        }

        tmp = buf[offset + 0x18].toInt() and 0xff
        if (tmp != 0) {
            val equipment = DatLib.getRes(DatLib.ResType.GRS, 1, tmp)
            equipmentsArray[7] = if (equipment is GoodsEquipment) equipment else {
                println("Warning: Failed to load equipment[7] for player $name")
                null
            }
        }
        
        // 初始化属性跟踪
        initAttributeTracking()
    }

    fun getEquipmentByIndex(index: Int): GoodsEquipment? {
        return equipmentsArray[index]
    }

    fun getCurrentEquipment(type: Int): GoodsEquipment? {
        for (i in 0..7) {
            if (sEquipTypes[i] == type) {
                return equipmentsArray[i]
            }
        }
        return null
    }

    /**
     * 是否已经装备该装备，对装饰检测空位
     * @param type
     * @param id
     * @return
     */
    fun hasEquipt(type: Int, id: Int): Boolean {
        if (type == 6) {
            // 两个位置都装备同一件装备才返回真
            return arrayOf(equipmentsArray[0], equipmentsArray[1]).all {
                return it?.let {
                    it.type == type && it.index == id
                } ?: false
            }
        }

        for (i in 2..7) {
            equipmentsArray[i]?.let {
                if (it.type == type && it.index == id) {
                    return true
                }
            }
        }
        return false
    }

    private fun putOnAt(goods: GoodsEquipment, index: Int) {
        if (equipmentsArray[index] == null) {
            goods.putOn(this)
            equipmentsArray[index] = goods
            logAttributeChange("穿上装备: ${goods.name} (位置$index)")
        }
    }

    /**
     * 穿上goods装备
     * @param goods
     */
    fun putOn(goods: GoodsEquipment, at: Int? = null) {
        at?.let {
            putOnAt(goods, at)
            return
        }

        for (i in 0..7) {
            if (goods.type == sEquipTypes[i]) {
                if (equipmentsArray[i] == null) { // 适用2个装饰
                    goods.putOn(this)
                    equipmentsArray[i] = goods
                    logAttributeChange("穿上装备: ${goods.name} (类型${goods.type})")
                    break
                }
            }
        }
    }

    /**
     * 脱下类型号为type的装备
     * @param type
     */
    fun takeOff(type: Int, index: Int? = null) {
        index?.let {
            takeOffByIndex(it)
            return
        }
        for (i in 0..7) {
            if (type == sEquipTypes[i]) {
                if (equipmentsArray[i] != null) {
                    val equipment = equipmentsArray[i]!!
                    equipment.takeOff(this)
                    equipmentsArray[i] = null
                    logAttributeChange("脱下装备: ${equipment.name} (类型$type)")
                    break
                }
            }
        }
    }

    private fun takeOffByIndex(index: Int) {
        val equipment = equipmentsArray[index]
        if (equipment != null) {
            equipment.takeOff(this)
            equipmentsArray[index] = null
            logAttributeChange("脱下装备: ${equipment.name} (位置$index)")
        }
    }

    /**
     * type型装备位置是否已经有装备
     * @param type 装备类型号
     * @return 是否有空
     */
    fun hasSpace(type: Int): Boolean {
        if (type == 6) { // 饰品
            return equipmentsArray[0] == null || equipmentsArray[1] == null
        } else {
            for (i in 0..7) {
                if (sEquipTypes[i] == type && equipmentsArray[i] == null) {
                    return true
                }
            }
        }
        return false
    }

    fun drawState(canvas: Canvas, page: Int) {
        // 只在第一次查看状态时打印Player对应的最大可以升级的数量
        if (!hasLoggedStateInfo) {
            println("[PlayerInfo] ${this.name} 最大升级数量: ${levelupChain.maxLevel}级")
            hasLoggedStateInfo = true
        }
        
        // 统一展示所有属性和魔法信息
        val startY = 4
        val lineHeight = 19
        var y = startY
        canvas.drawLine(37, y - 4, 37, y - 4 + lineHeight * 10, Util.sBlackPaint)
        TextRender.drawText(canvas, "等级   $level", 41, y)
        y += lineHeight
        TextRender.drawText(canvas, "生命   $hp/$maxHP", 41, y)
        y += lineHeight
        TextRender.drawText(canvas, "真气   $mp/$maxMP", 41, y)
        y += lineHeight
        TextRender.drawText(canvas, "攻击力 $attack", 41, y)
        y += lineHeight
        TextRender.drawText(canvas, "防御力 $defend", 41, y)
        y += lineHeight
        TextRender.drawText(canvas, "身法   $speed", 41, y)
        y += lineHeight
        TextRender.drawText(canvas, "灵力   $lingli", 41, y)
        y += lineHeight
        TextRender.drawText(canvas, "幸运   $luck", 41, y)
        y += lineHeight
        // 经验值
        TextRender.drawText(canvas, "经验值", 41, y)
        val w = Util.drawSmallNum(canvas, currentExp, 97, y)
        TextRender.drawText(canvas, "/", 97 + w + 2, y)
        Util.drawSmallNum(canvas, levelupChain.getNextLevelExp(level), 97 + w + 9, y + 6)
        y += lineHeight
        // 免疫
        val sb = StringBuilder("免疫   ")
        val tmp = StringBuilder()
        if (hasBuff(FightingCharacter.BUFF_MASK_DU)) tmp.append('毒')
        if (hasBuff(FightingCharacter.BUFF_MASK_LUAN)) tmp.append('乱')
        if (hasBuff(FightingCharacter.BUFF_MASK_FENG)) tmp.append('封')
        if (hasBuff(FightingCharacter.BUFF_MASK_MIAN)) tmp.append('眠')
        if (tmp.isNotEmpty()) sb.append(tmp) else sb.append('无')
        TextRender.drawText(canvas, sb.toString(), 41, y)
        y += lineHeight
        // 魔法信息
        TextRender.drawText(canvas, "已学魔法:", 41, y)
        val magicList = if (magicChain != null) magicChain!!.getAllLearntMagics() else privateLearntMagics
        val maxDisplay = minOf(4, magicList.size)
        for (i in 0 until maxDisplay) {
            val magic = magicList.elementAt(i)
            val magicText = try { magic.magicName } catch (e: Exception) { "未知" }
            TextRender.drawText(canvas, "${i + 1}. $magicText", 41, y + lineHeight * (i + 1))
        }
        if (magicList.size > 4) {
            TextRender.drawText(canvas, "... 还有${magicList.size - 4}个", 41, y + lineHeight * (maxDisplay + 1))
        }
    }

    override fun decode(coder: ObjectInput) {
        type = coder.readInt()
        index = coder.readInt()
        if (index > 0)
            mImgHead = DatLib.getRes(DatLib.ResType.PIC, 1, index) as ResImage?
        levelupChain = DatLib.getRes(DatLib.ResType.MLR, 2, index, true) as ResLevelupChain? ?: ResLevelupChain()
        setWalkingSprite(WalkingSprite(type, coder.readInt()))
        fightingSprite = FightingSprite(DatLib.ResType.PIC, index)
        direction = Direction.fromInt(coder.readInt())
        step = coder.readInt()
        setPosInMap(coder.readInt(), coder.readInt())
        magicChain = DatLib.getMlr(1, coder.readInt(), true)
        magicChain?.learnNum = coder.readInt()
        name = coder.readString()
        level = coder.readInt()
        maxHP = coder.readInt()
        hp = coder.readInt()
        maxMP = coder.readInt()
        mp = coder.readInt()
        attack = coder.readInt()
        defend = coder.readInt()
        speed = coder.readInt()
        lingli = coder.readInt()
        luck = coder.readInt()
        currentExp = coder.readInt()
        for (i in 0..7) {
            val type = coder.readInt()
            val index = coder.readInt()
            if (type != 0 && index != 0) {
                equipmentsArray[i] = DatLib.getRes(DatLib.ResType.GRS, type, index) as GoodsEquipment?
            }
        }
        val size = coder.readInt()
        for (i in 0 until size) {
            val type = coder.readInt()
            val index = coder.readInt()
            // Skip invalid magic resources (type 0 or index 0)
            if (type > 0 && index > 0) {
                try {
                    val magic = DatLib.getMrsOrNull(type, index)
                    if (magic != null) {
                        privateLearntMagics.add(magic)
                    }
                } catch (e: Exception) {
                    println("WARNING: Failed to load magic type=$type, index=$index: ${e.message}")
                }
            }
        }
        if (coder.version >= 3) {
            buff.decode(coder)
            debuff.decode(coder)
            atbuff.decode(coder)
        }

        // 版本6：添加总属性值
        if (coder.version >= 6) {
            totalMaxHP = coder.readInt()
            totalMaxMP = coder.readInt()
            totalAttack = coder.readInt()
            totalDefend = coder.readInt()
            totalSpeed = coder.readInt()
            totalLingli = coder.readInt()
            totalLuck = coder.readInt()
            
            // 确保总值不小于当前值
            if (totalMaxHP < maxHP) totalMaxHP = maxHP
            if (totalMaxMP < maxMP) totalMaxMP = maxMP
            if (totalAttack < attack) totalAttack = attack
            if (totalDefend < defend) totalDefend = defend
            if (totalSpeed < speed) totalSpeed = speed
            if (totalLingli < lingli) totalLingli = lingli
            if (totalLuck < luck) totalLuck = luck
        } else {
            // 兼容旧版本：使用当前属性值初始化
            totalMaxHP = maxHP
            totalMaxMP = maxMP
            totalAttack = attack
            totalDefend = defend
            totalSpeed = speed
            totalLingli = lingli
            totalLuck = luck
        }
    }

    override fun encode(out: ObjectOutput) {
        out.writeInt(type)
        out.writeInt(index)
        out.writeInt(walkingSpriteId)
        out.writeInt(direction.v)
        out.writeInt(step)
        out.writeInt(posInMap.x)
        out.writeInt(posInMap.y)
        out.writeInt(magicChain?.index ?: 0)
        magicChain?.let {
            out.writeInt(it.learnNum)
        }
        out.writeString(name)
        out.writeInt(level)
        out.writeInt(maxHP)
        out.writeInt(hp)
        out.writeInt(maxMP)
        out.writeInt(mp)
        out.writeInt(attack)
        out.writeInt(defend)
        out.writeInt(speed)
        out.writeInt(lingli)
        out.writeInt(luck)
        out.writeInt(currentExp)
        for (i in 0..7) {
            equipmentsArray[i]?.let {
                out.writeInt(it.type)
                out.writeInt(it.index)
            } ?: run {
                out.writeInt(0)
                out.writeInt(0)
            }
        }
        out.writeInt(privateLearntMagics.size)
        privateLearntMagics.forEach {
            out.writeInt(it.type)
            out.writeInt(it.index)
        }
        buff.encode(out)
        debuff.encode(out)
        atbuff.encode(out)

        // 版本6：保存总属性值
        out.writeInt(totalMaxHP)
        out.writeInt(totalMaxMP)
        out.writeInt(totalAttack)
        out.writeInt(totalDefend)
        out.writeInt(totalSpeed)
        out.writeInt(totalLingli)
        out.writeInt(totalLuck)
    }

    fun setLevel(level: Int) {
        this.level = min(level, levelupChain.maxLevel)
    }

    override fun getAllMagics(): Collection<BaseMagic> {
        return privateLearntMagics + super.getAllMagics()
    }

    fun learnMagic(magic: BaseMagic) {
        privateLearntMagics.add(magic)
    }

    fun levelUp(to: Int) {
        if (to <= level )
            return
        val cl = level
        val c = levelupChain
        val p = this

        p.level = to

        // 更新不受限制的总属性值
        val hpIncrease = c.getMaxHP(to) - c.getMaxHP(cl)
        val mpIncrease = c.getMaxMP(to) - c.getMaxMP(cl)
        val attackIncrease = c.getAttack(to) - c.getAttack(cl)
        val defendIncrease = c.getDefend(to) - c.getDefend(cl)
        val speedIncrease = c.getSpeed(to) - c.getSpeed(cl)
        val lingliIncrease = c.getLingli(to) - c.getLingli(cl)
        val luckIncrease = c.getLuck(to) - c.getLuck(cl)

        p.totalMaxHP += hpIncrease
        p.totalMaxMP += mpIncrease
        p.totalAttack += attackIncrease
        p.totalDefend += defendIncrease
        p.totalSpeed += speedIncrease
        p.totalLingli += lingliIncrease
        p.totalLuck += luckIncrease

        // 应用到实际属性（受限制）
        p.maxHP = p.totalMaxHP
        p.maxMP = p.totalMaxMP
        p.attack = p.totalAttack
        p.defend = p.totalDefend
        p.speed = p.totalSpeed
        p.lingli = p.totalLingli
        p.luck = p.totalLuck

        p.hp = p.maxHP
        p.mp = p.maxMP
        p.magicChain?.learnNum = c.getLearnMagicNum(to)

        logAttributeChange("升级: Level $cl -> $to (HP恢复满, MP恢复满)")
        
        // 输出详细的升级增量信息
        println("[LevelUp] 升级增量: MaxHP+$hpIncrease, MaxMP+$mpIncrease, Attack+$attackIncrease")
        println("[LevelUp] 升级增量: Defend+$defendIncrease, Speed+$speedIncrease, Lingli+$lingliIncrease, Luck+$luckIncrease")
    }

    companion object {
        /** 装备界面从左至右的装备类型号 */
        val sEquipTypes = intArrayOf(6, 6, 5, 3, 7, 2, 4, 1)

        var sGoodsList = GoodsManage()

        var sMoney = 0
    }

}
