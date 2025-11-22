package fmj.script

import graphics.Canvas
import java.Coder
import java.ObjectInput
import java.ObjectOutput

class ScriptProcess
/**
 *
 * @param commands 一个脚本文件对应的操作表
 * @param eventIndex eventIndex[i]等于触发事件i+1时，要执行的Operate在list中的序号
 * @param map 地址偏移-序号
 */
(private val name: String, private val commands: ArrayList<Command>,
 /**
  * eventIndex[i]等于触发事件i+1时，要执行的Operate在list中的序号，
  * -1表示不存在
  */
 private val eventIndex: IntArray,
 /**
  * address offset --- curOp's index of commands
  */
 private val mMapAddrOffsetIndex: HashMap<Int, Int>,
 /**
  * code数据前的长度
  */
 private val mHeaderCnt: Int) {

    /**
     * 当前正在执行的操作在mOperateList中的位置
     */
    private var mCurExeOperateIndex: Int = 0

    /**
     * 当前是否正在执行 update() draw()
     */
    private var curOp: Operate? = null

    var running = false
        private set

    var prev: ScriptProcess? = null

    var timer: Int = 0
    var timerCounter: Int = 0
    var timerEventId: Int = 0
    
    // 防护：防止process重入的标志
    var isProcessing = false

    fun start() {
        running = true
        goonExecute = true
    }

    fun stop() {
        running = false
        goonExecute = false
    }
    
    fun getCurrentIndex(): Int {
        return mCurExeOperateIndex
    }
    
    fun setCurrentIndex(index: Int) {
        if (index >= 0 && index < commands.size) {
            mCurExeOperateIndex = index
            println("[ScriptProcess] Set current index to $mCurExeOperateIndex")
        }
    }

    fun getCommandsSize(): Int {
        return commands.size
    }
    
    fun executeCurrentCommand() {
        // 执行当前索引位置的命令
        if (mCurExeOperateIndex < commands.size) {
            val cmd = commands[mCurExeOperateIndex]
            println("[ScriptProcess] Executing command at index $mCurExeOperateIndex: ${cmd::class.simpleName}")
            
            // 记录执行前的索引，用于检测是否执行了 goto
            val indexBeforeExecution = mCurExeOperateIndex
            
            curOp = cmd.run(this)
            
            // 如果命令不需要 update/draw（curOp 为 null），继续执行
            if (curOp == null) {
                // 检查是否执行了 goto（索引发生了变化）
                if (mCurExeOperateIndex != indexBeforeExecution) {
                    // goto 改变了索引，需要重置 goonExecute 并继续执行
                    println("[ScriptProcess] Goto detected, continuing from new index $mCurExeOperateIndex")
                    goonExecute = true
                    process()
                } else {
                    // 正常命令，移动到下一条
                    mCurExeOperateIndex++
                    // 继续执行后续命令
                    process()
                }
            }
        }
    }

    /**
     * 触发地图事件,场景切换，NPC对话，开宝箱等
     * @param eventId 是否成功触发
     */
    fun triggerEvent(eventId: Int): Boolean {
        // 防护：检查是否有正在执行的操作
        if (curOp != null) {
            println("[ScriptProcess.triggerEvent] 警告: 有操作正在执行，延迟触发事件 $eventId")
            return false
        }

        if (eventId > eventIndex.size) {
            println("[ScriptProcess.triggerEvent] 事件ID $eventId 超出范围 (max: ${eventIndex.size})，停止执行")
            stop()
            return false
        }

        val index = eventIndex[eventId - 1]
        println("[ScriptProcess.triggerEvent] 事件 $eventId 对应的脚本索引: $index")

        if (index != -1) {
            // 防护：检查索引有效性
            if (index >= commands.size) {
                println("[ScriptProcess.triggerEvent] 错误: 事件索引越界 $index >= ${commands.size}")
                stop()
                return false
            }

            println("[ScriptProcess.triggerEvent] 成功触发事件 $eventId，跳转到脚本索引 $index")
            mCurExeOperateIndex = index
            curOp = null
            start()
            return true
        }
        stop()
        return false
    }

    fun gotoAddress(address: Int) {
        // 防护：检查地址映射是否存在
        val targetIndex = mMapAddrOffsetIndex[address - mHeaderCnt] as? Int
        if (targetIndex == null) {
            println("[ScriptProcess] 错误: 无效的跳转地址 $address")
            running = true
            return
        }
        
        // 防护：检查目标索引有效性
        if (targetIndex >= commands.size || targetIndex < 0) {
            println("[ScriptProcess] 错误: 跳转目标索引越界 $targetIndex")
            stop()
            return
        }
        
        mCurExeOperateIndex = targetIndex
        if (curOp != null) { // 不在Operate.process()中调用的gotoAddress
            curOp = null
            --mCurExeOperateIndex
        }
        // 不再设置 goonExecute = false，让同步命令能在同一帧内继续执行
        // 这样 cmd_testmoney 等同步命令就不会在每次 goto 后等待 40ms
        running = true
    }

    fun process() {
        // 防护：防止重入
        if (isProcessing) {
            println("[ScriptProcess] 防止process重入")
            return
        }
        
        try {
            isProcessing = true
            
            if (curOp == null) {
                // 检查是否已经到达脚本末尾
                if (mCurExeOperateIndex >= commands.size) {
                    // 脚本已经完成，设置running为false停止重复调用
                    if (running) {
                        println("ScriptProcess.process: Script completed, stopping execution")
                        running = false
                        // 重要修复：重置goonExecute确保下次能正确执行
                        goonExecute = true
                    }
                    return
                }
                
                while (mCurExeOperateIndex < commands.size && goonExecute) {
                    // 记录执行前的索引，用于检测 goto
                    val indexBeforeExecution = mCurExeOperateIndex
                    val cmd = commands[mCurExeOperateIndex]
                    curOp = cmd.run(this)
                    if (curOp != null) { // 执行 update draw
                        return
                    }
                    if (!goonExecute) {
                        goonExecute = true
                        return
                    }

                    // 检查是否执行了 goto（索引被 gotoAddress 改变）
                    // 如果执行了 goto，索引已经被更新，不需要递增
                    if (mCurExeOperateIndex == indexBeforeExecution) {
                        ++mCurExeOperateIndex
                    }
                    // else: goto 已经改变了索引，直接继续执行新位置的命令
                }
                
                // 到达命令列表末尾
                if (mCurExeOperateIndex >= commands.size && running) {
                    println("ScriptProcess.process: Reached end of command list, stopping execution")
                    running = false
                    goonExecute = true
                }
            }
        } finally {
            isProcessing = false
        }
    }

    fun update(delta: Long) {
        curOp?.update(delta)?.let {
            if (!it) { // 退出当前操作
                curOp = null
                ++mCurExeOperateIndex
            }
        }
    }

    fun timerStep(delta: Long) {
        if (timer > 0 && timerEventId > 0) {
            timerCounter -= delta.toInt()
            if (timerCounter <= 0) {
                timerCounter += timer
                triggerEvent(timerEventId)
            }
        }
    }

    fun setTimer(timer: Int, eventId: Int) {
        this.timer = timer * 500
        timerCounter = this.timer
        timerEventId = eventId
    }

    fun draw(canvas: Canvas) {
        curOp?.draw(canvas)
    }
    
    fun hasDrawContent(): Boolean {
        return curOp != null
    }

    fun keyDown(key: Int) {
        curOp?.onKeyDown(key)
    }

    fun keyUp(key: Int) {
        curOp?.onKeyUp(key)
    }

    fun encode(coder: ObjectOutput) {
        coder.writeInt(timer)
        coder.writeInt(timerCounter)
        coder.writeInt(timerEventId)
    }

    fun decode(coder: ObjectInput) {
        timer = coder.readInt()
        timerCounter = coder.readInt()
        timerEventId = coder.readInt()
    }

    var goonExecute = true
}
