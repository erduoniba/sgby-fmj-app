package fmj.script

import fmj.DebugLogger
import java.ObjectInput
import java.ObjectOutput
import java.sysInitBoxMapping
import java.sysAddBoxMapping
import java.sysLoadBoxMappingsIntoKotlin

object ScriptResources {

    /**
     * 全局事件标志1-2400
     */
    var globalEvents = BooleanArray(2401)

    /**
     * 全局变量0-199,局部变量200-239,扩展变量240-799
     */
    var variables = IntArray(800)
    
    /**
     * 宝箱位置到事件ID的映射表
     * key: "mapType_mapIndex_x_y_boxType_boxIndex" 格式的唯一标识符
     * value: 对应的全局事件ID (如1004、1005等)
     * 
     * 这个映射表用于记录每个宝箱对应的事件ID，
     * 通过动态学习建立映射，并持久化到存档中
     */
    private val boxEventMapping = mutableMapOf<String, Int>()
    
    /**
     * 当前正在触发的宝箱信息（临时变量）
     */
    data class TriggeringBoxInfo(
        val mapType: Int,
        val mapIndex: Int,
        val npcId: Int,
        val boxType: Int,
        val boxIndex: Int,
        val x: Int,
        val y: Int
    ) {
        fun toKey(): String = "${mapType}_${mapIndex}_${x}_${y}_${boxType}_${boxIndex}"
    }
    
    var currentTriggeringBox: TriggeringBoxInfo? = null

    /**
     * 初始化局部变量
     */
    fun initLocalVar() {
        for (i in 200..239) {
            variables[i] = 0
        }
    }

    /**
     * 安全获取变量值，越界时返回0
     */
    fun getVariable(index: Int): Int {
        return if (index >= 0 && index < variables.size) {
            variables[index]
        } else {
            println("[ScriptResources] Warning: variable[$index] out of bounds (size=${variables.size}), returning 0")
            0
        }
    }

    /**
     * 安全设置变量值，越界时忽略
     */
    fun setVariable(index: Int, value: Int) {
        if (index >= 0 && index < variables.size) {
            variables[index] = value
        } else {
            println("[ScriptResources] Warning: variable[$index] out of bounds (size=${variables.size}), ignoring set")
        }
    }

    /**
     * 初始化全局变量
     */
    fun initGlobalVar() {
        for (i in 0..199) {
            variables[i] = 0
        }
        // 初始化扩展变量区域
        for (i in 240..799) {
            variables[i] = 0
        }
    }

    /**
     * 初始化全局事件
     */
    fun initGlobalEvents() {
        for (i in 1..2400) {
            globalEvents[i] = false
        }
        // 新游戏时清空宝箱映射
        boxEventMapping.clear()
        
        // 初始化JavaScript端的宝箱映射
        initBoxMappingFromJS()
    }
    
    /**
     * 设置宝箱与事件ID的映射关系
     */
    fun setBoxEventMapping(boxKey: String, eventId: Int) {
        if (!boxEventMapping.containsKey(boxKey)) {
            boxEventMapping[boxKey] = eventId
            println("[BOX_MAPPING] Established: $boxKey -> Event $eventId")
            
            // 通过外部函数保存到Cookie
            try {
                sysAddBoxMapping(boxKey, eventId)
            } catch (e: Exception) {
                println("[BOX_MAPPING] Failed to save to cookie: ${e.message}")
            }
        }
    }
    
    /**
     * 获取宝箱对应的事件ID
     */
    fun getBoxEventId(boxKey: String): Int? {
        return boxEventMapping[boxKey]
    }
    
    /**
     * 检查宝箱是否已被收集
     */
    fun isBoxCollected(boxKey: String): Boolean {
        val eventId = boxEventMapping[boxKey]
        return if (eventId != null) {
            globalEvents[eventId]
        } else {
            false
        }
    }

    /**
     * 将全局事件num标志设置为true
     * @param num 1-2400
     */
    fun setEvent(num: Int) {
        globalEvents[num] = true
    }

    /**
     * 将全局事件num标志设置为false
     */
    fun clearEvent(num: Int) {
        globalEvents[num] = false
    }

    fun write(out: ObjectOutput) {
        // 写全局事件
        for (i in 1..2400) {
            out.writeBoolean(globalEvents[i])
        }

        // 写全局变量&局部变量
        for (i in 0..239) {
            out.writeInt(variables[i])
        }
    }
    

    fun read(coder: ObjectInput) {
        // 读全局事件
        for (i in 1..2400) {
            globalEvents[i] = coder.readBoolean()
        }

        // 读全局变量&局部变量（保持兼容性，只读取原有的240个变量）
        for (i in 0..239) {
            variables[i] = coder.readInt()
        }
        // 扩展变量区域初始化为0
        for (i in 240..799) {
            variables[i] = 0
        }
        
        // 清空宝箱映射，然后从JS加载
        boxEventMapping.clear()
        initBoxMappingFromJS()
    }
    
    /**
     * 从JavaScript端初始化宝箱映射
     */
    private fun initBoxMappingFromJS() {
        try {
            // 初始化JavaScript端（自动识别游戏MOD）
            sysInitBoxMapping()
            
            // 通过回调函数加载映射
            sysLoadBoxMappingsIntoKotlin { key, value ->
                boxEventMapping[key] = value
            }
            
            println("[BOX_MAPPING] Loaded ${boxEventMapping.size} mappings from JavaScript/Cookie")
        } catch (e: Exception) {
            println("[BOX_MAPPING] Failed to load from JavaScript: ${e.message}")
        }
    }
}
