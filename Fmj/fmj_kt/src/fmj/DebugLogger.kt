package fmj

import kotlin.js.JsExport
import kotlin.js.js

/**
 * 调试日志系统 - 支持标签化控制的日志输出
 * 用于定位和分析游戏中的各种问题，特别是存档加载后boss消失等问题
 */
@JsExport
object DebugLogger {
    
    // 日志标签定义
    object Tags {
        // === 核心游戏系统 ===
        const val BOSS_TRACKING = "BOSS_TRACKING"      // Boss状态追踪
        const val SAVE_LOAD = "SAVE_LOAD"              // 存档加载
        const val NPC_MANAGEMENT = "NPC_MANAGEMENT"    // NPC管理
        const val SCRIPT_EXECUTION = "SCRIPT_EXECUTION" // 脚本执行
        const val COMBAT_SYSTEM = "COMBAT_SYSTEM"      // 战斗系统
        const val MAP_LOADING = "MAP_LOADING"          // 地图加载
        const val CHARACTER_STATE = "CHARACTER_STATE"  // 角色状态
        const val RESOURCE_LOADING = "RESOURCE_LOADING" // 资源加载
        
        // === 游戏玩法 ===
        const val PLAYER_ACTIONS = "PLAYER_ACTIONS"    // 玩家操作
        const val ITEM_MANAGEMENT = "ITEM_MANAGEMENT"  // 物品管理
        const val MAGIC_SYSTEM = "MAGIC_SYSTEM"        // 魔法系统
        const val EQUIPMENT = "EQUIPMENT"              // 装备系统
        const val LEVELING = "LEVELING"                // 升级系统
        const val DIALOGUE = "DIALOGUE"                // 对话系统
        
        // === 技术系统 ===
        const val INPUT_EVENTS = "INPUT_EVENTS"        // 输入事件
        const val RENDER_SYSTEM = "RENDER_SYSTEM"      // 渲染系统
        const val AUDIO_SYSTEM = "AUDIO_SYSTEM"        // 音频系统
        const val UI_EVENTS = "UI_EVENTS"              // UI事件
        const val PERFORMANCE = "PERFORMANCE"          // 性能监控
        const val SYSTEM_STARTUP = "SYSTEM_STARTUP"    // 系统启动
        const val SCREEN_MANAGEMENT = "SCREEN_MANAGEMENT" // 屏幕管理
        const val ASSET_COMPILATION = "ASSET_COMPILATION" // 资源编译
        const val COMMAND_EXECUTION = "COMMAND_EXECUTION" // 命令执行
        const val DEV_TOOLS = "DEV_TOOLS"              // 开发工具
        
        // === 错误调试 ===
        const val ERROR_HANDLING = "ERROR_HANDLING"    // 错误处理
        const val MEMORY_DEBUG = "MEMORY_DEBUG"        // 内存调试
        const val NETWORK_DEBUG = "NETWORK_DEBUG"      // 网络调试
    }
    
    // 日志级别
    enum class Level(val prefix: String, val color: String) {
        DEBUG("🔍", "#6B7280"),    // 调试信息
        INFO("ℹ️", "#3B82F6"),     // 一般信息
        WARN("⚠️", "#F59E0B"),     // 警告
        ERROR("❌", "#EF4444"),    // 错误
        TRACE("🔎", "#8B5CF6")     // 详细追踪
    }
    
    // 标签开关状态 - 默认全部关闭，通过dev tools开启
    private val tagStates = mutableMapOf<String, Boolean>().apply {
        // 手动初始化所有标签（Kotlin/JS不支持反射）
        // 核心游戏系统
        this[Tags.BOSS_TRACKING] = false
        this[Tags.SAVE_LOAD] = false
        this[Tags.NPC_MANAGEMENT] = false
        this[Tags.SCRIPT_EXECUTION] = false
        this[Tags.COMBAT_SYSTEM] = false
        this[Tags.MAP_LOADING] = false
        this[Tags.CHARACTER_STATE] = false
        this[Tags.RESOURCE_LOADING] = false
        
        // 游戏玩法
        this[Tags.PLAYER_ACTIONS] = false
        this[Tags.ITEM_MANAGEMENT] = false
        this[Tags.MAGIC_SYSTEM] = false
        this[Tags.EQUIPMENT] = false
        this[Tags.LEVELING] = false
        this[Tags.DIALOGUE] = false
        
        // 技术系统
        this[Tags.INPUT_EVENTS] = false
        this[Tags.RENDER_SYSTEM] = false
        this[Tags.AUDIO_SYSTEM] = false
        this[Tags.UI_EVENTS] = false
        this[Tags.PERFORMANCE] = false
        this[Tags.SYSTEM_STARTUP] = false
        this[Tags.SCREEN_MANAGEMENT] = false
        this[Tags.ASSET_COMPILATION] = false
        this[Tags.COMMAND_EXECUTION] = false
        this[Tags.DEV_TOOLS] = false
        
        // 错误调试
        this[Tags.ERROR_HANDLING] = false
        this[Tags.MEMORY_DEBUG] = false
        this[Tags.NETWORK_DEBUG] = false
    }
    
    /**
     * 获取所有可用的日志标签
     */
    fun getAllTags(): Array<String> {
        return arrayOf(
            // 核心游戏系统
            Tags.BOSS_TRACKING,
            Tags.SAVE_LOAD,
            Tags.NPC_MANAGEMENT,
            Tags.SCRIPT_EXECUTION,
            Tags.COMBAT_SYSTEM,
            Tags.MAP_LOADING,
            Tags.CHARACTER_STATE,
            Tags.RESOURCE_LOADING,
            
            // 游戏玩法
            Tags.PLAYER_ACTIONS,
            Tags.ITEM_MANAGEMENT,
            Tags.MAGIC_SYSTEM,
            Tags.EQUIPMENT,
            Tags.LEVELING,
            Tags.DIALOGUE,
            
            // 技术系统
            Tags.INPUT_EVENTS,
            Tags.RENDER_SYSTEM,
            Tags.AUDIO_SYSTEM,
            Tags.UI_EVENTS,
            Tags.PERFORMANCE,
            Tags.SYSTEM_STARTUP,
            Tags.SCREEN_MANAGEMENT,
            Tags.ASSET_COMPILATION,
            Tags.COMMAND_EXECUTION,
            Tags.DEV_TOOLS,
            
            // 错误调试
            Tags.ERROR_HANDLING,
            Tags.MEMORY_DEBUG,
            Tags.NETWORK_DEBUG
        )
    }
    
    /**
     * 获取标签的当前状态
     */
    fun isTagEnabled(tag: String): Boolean {
        return Global.ENABLE_DEBUG_LOGS && tagStates[tag] == true
    }
    
    /**
     * 设置标签状态
     */
    fun setTagEnabled(tag: String, enabled: Boolean) {
        tagStates[tag] = enabled
        log(Tags.RESOURCE_LOADING, Level.INFO, "Logger", 
            "标签 [$tag] ${if (enabled) "已开启" else "已关闭"}")
    }
    
    /**
     * 获取所有标签状态
     */
    fun getAllTagStates(): Map<String, Boolean> {
        return tagStates.toMap()
    }
    
    /**
     * 批量设置标签状态
     */
    fun setMultipleTagsEnabled(tags: Array<String>, enabled: Boolean) {
        tags.forEach { setTagEnabled(it, enabled) }
    }
    
    /**
     * 开启所有标签
     */
    fun enableAllTags() {
        getAllTags().forEach { setTagEnabled(it, true) }
    }
    
    /**
     * 关闭所有标签
     */
    fun disableAllTags() {
        getAllTags().forEach { setTagEnabled(it, false) }
    }
    
    /**
     * 预设标签组合控制
     */
    fun enableBossDebugging() {
        disableAllTags()
        setTagEnabled(Tags.BOSS_TRACKING, true)
        setTagEnabled(Tags.SAVE_LOAD, true)
        setTagEnabled(Tags.NPC_MANAGEMENT, true)
        setTagEnabled(Tags.SCRIPT_EXECUTION, true)
        setTagEnabled(Tags.COMBAT_SYSTEM, true)
    }
    
    fun enableCoreGameSystems() {
        disableAllTags()
        setTagEnabled(Tags.SAVE_LOAD, true)
        setTagEnabled(Tags.MAP_LOADING, true)
        setTagEnabled(Tags.SCRIPT_EXECUTION, true)
        setTagEnabled(Tags.CHARACTER_STATE, true)
        setTagEnabled(Tags.RESOURCE_LOADING, true)
    }
    
    fun enableGameplayDebugging() {
        disableAllTags()
        setTagEnabled(Tags.PLAYER_ACTIONS, true)
        setTagEnabled(Tags.ITEM_MANAGEMENT, true)
        setTagEnabled(Tags.MAGIC_SYSTEM, true)
        setTagEnabled(Tags.EQUIPMENT, true)
        setTagEnabled(Tags.LEVELING, true)
        setTagEnabled(Tags.DIALOGUE, true)
    }
    
    fun enableTechnicalDebugging() {
        disableAllTags()
        setTagEnabled(Tags.INPUT_EVENTS, true)
        setTagEnabled(Tags.RENDER_SYSTEM, true)
        setTagEnabled(Tags.UI_EVENTS, true)
        setTagEnabled(Tags.PERFORMANCE, true)
        setTagEnabled(Tags.ERROR_HANDLING, true)
    }
    
    /**
     * 获取标签分组信息
     */
    fun getCoreSystemTags(): Array<String> {
        return arrayOf(
            Tags.BOSS_TRACKING, Tags.SAVE_LOAD, Tags.NPC_MANAGEMENT,
            Tags.SCRIPT_EXECUTION, Tags.COMBAT_SYSTEM, Tags.MAP_LOADING,
            Tags.CHARACTER_STATE, Tags.RESOURCE_LOADING
        )
    }
    
    fun getGameplayTags(): Array<String> {
        return arrayOf(
            Tags.PLAYER_ACTIONS, Tags.ITEM_MANAGEMENT, Tags.MAGIC_SYSTEM,
            Tags.EQUIPMENT, Tags.LEVELING, Tags.DIALOGUE
        )
    }
    
    fun getTechnicalTags(): Array<String> {
        return arrayOf(
            Tags.INPUT_EVENTS, Tags.RENDER_SYSTEM, Tags.AUDIO_SYSTEM,
            Tags.UI_EVENTS, Tags.PERFORMANCE, Tags.SYSTEM_STARTUP,
            Tags.SCREEN_MANAGEMENT, Tags.ASSET_COMPILATION, 
            Tags.COMMAND_EXECUTION, Tags.DEV_TOOLS
        )
    }
    
    fun getDebuggingTags(): Array<String> {
        return arrayOf(
            Tags.ERROR_HANDLING, Tags.MEMORY_DEBUG, Tags.NETWORK_DEBUG
        )
    }
    
    /**
     * 主要日志输出方法
     */
    fun log(tag: String, level: Level, source: String, message: String, details: String? = null) {
        if (!isTagEnabled(tag)) return
        
        val timestamp = getCurrentTimestamp()
        val logMessage = buildString {
            append("[$timestamp] ")
            append("${level.prefix} ")
            append("[$tag] ")
            append("[$source] ")
            append(message)
            if (details != null) {
                append(" | 详情: $details")
            }
        }
        
        // 输出到浏览器控制台
        when (level) {
            Level.ERROR -> kotlin.js.console.error(logMessage)
            Level.WARN -> kotlin.js.console.warn(logMessage)
            Level.INFO -> kotlin.js.console.info(logMessage)
            Level.DEBUG -> kotlin.js.console.log(logMessage)
            Level.TRACE -> kotlin.js.console.log(logMessage)
        }
    }
    
    // 便捷方法
    fun debug(tag: String, source: String, message: String, details: String? = null) {
        log(tag, Level.DEBUG, source, message, details)
    }
    
    fun info(tag: String, source: String, message: String, details: String? = null) {
        log(tag, Level.INFO, source, message, details)
    }
    
    fun warn(tag: String, source: String, message: String, details: String? = null) {
        log(tag, Level.WARN, source, message, details)
    }
    
    fun error(tag: String, source: String, message: String, details: String? = null) {
        log(tag, Level.ERROR, source, message, details)
    }
    
    fun trace(tag: String, source: String, message: String, details: String? = null) {
        log(tag, Level.TRACE, source, message, details)
    }
    
    /**
     * Boss相关的专用日志方法
     */
    object Boss {
        fun created(npcId: Int, monsterType: Int, x: Int, y: Int, details: String? = null) {
            info(Tags.BOSS_TRACKING, "BossCreation", 
                "Boss创建 - NPC ID: $npcId, 怪物类型: $monsterType, 位置: ($x, $y)", details)
        }
        
        fun removed(npcId: Int, reason: String, details: String? = null) {
            info(Tags.BOSS_TRACKING, "BossRemoval", 
                "Boss移除 - NPC ID: $npcId, 原因: $reason", details)
        }
        
        fun stateChanged(npcId: Int, oldState: String, newState: String, details: String? = null) {
            debug(Tags.BOSS_TRACKING, "BossState", 
                "Boss状态变更 - NPC ID: $npcId, $oldState -> $newState", details)
        }
        
        fun encounterStarted(monsterTypes: String, winAddr: Int, loseAddr: Int) {
            info(Tags.BOSS_TRACKING, "BossEncounter", 
                "Boss战斗开始 - 怪物类型: $monsterTypes, 胜利地址: $winAddr, 失败地址: $loseAddr")
        }
        
        fun defeated(monsterType: Int, expGained: Int, moneyGained: Int) {
            info(Tags.BOSS_TRACKING, "BossDefeated", 
                "Boss被击败 - 类型: $monsterType, 获得经验: $expGained, 获得金钱: $moneyGained")
        }
        
        fun disappeared(npcId: Int, lastKnownState: String, investigationHint: String) {
            error(Tags.BOSS_TRACKING, "BossDisappearance", 
                "Boss异常消失 - NPC ID: $npcId, 最后状态: $lastKnownState, 调查线索: $investigationHint")
        }
    }
    
    /**
     * 存档加载相关的专用日志方法
     */
    object SaveLoad {
        fun saveStarted(slotId: Int, sceneName: String) {
            info(Tags.SAVE_LOAD, "SaveOperation", 
                "开始保存游戏 - 槽位: $slotId, 场景: $sceneName")
        }
        
        fun saveCompleted(slotId: Int, npcCount: Int, playerCount: Int) {
            info(Tags.SAVE_LOAD, "SaveOperation", 
                "保存完成 - 槽位: $slotId, NPC数量: $npcCount, 玩家数量: $playerCount")
        }
        
        fun loadStarted(slotId: Int) {
            info(Tags.SAVE_LOAD, "LoadOperation", 
                "开始加载游戏 - 槽位: $slotId")
        }
        
        fun loadCompleted(slotId: Int, sceneName: String, npcCount: Int) {
            info(Tags.SAVE_LOAD, "LoadOperation", 
                "加载完成 - 槽位: $slotId, 场景: $sceneName, 恢复NPC数量: $npcCount")
        }
        
        fun npcRestored(npcId: Int, npcType: String, x: Int, y: Int, state: String) {
            debug(Tags.SAVE_LOAD, "NPCRestore", 
                "NPC恢复 - ID: $npcId, 类型: $npcType, 位置: ($x, $y), 状态: $state")
        }
        
        fun bossStateRestored(npcId: Int, monsterType: Int, wasDefeated: Boolean) {
            info(Tags.SAVE_LOAD, "BossRestore", 
                "Boss状态恢复 - NPC ID: $npcId, 怪物类型: $monsterType, 已击败: $wasDefeated")
        }
    }
    
    /**
     * 系统启动相关的专用日志方法
     */
    object System {
        fun startup(component: String, message: String, details: String? = null) {
            info(Tags.SYSTEM_STARTUP, component, message, details)
        }
        
        fun devToolsStatus(status: String, message: String, details: String? = null) {
            info(Tags.DEV_TOOLS, "DevTools", "$status: $message", details)
        }
        
        fun screenChange(from: String, to: String, details: String? = null) {
            info(Tags.SCREEN_MANAGEMENT, "ScreenTransition", "从 $from 切换到 $to", details)
        }
        
        fun assetCompilation(asset: String, status: String, details: String? = null) {
            info(Tags.ASSET_COMPILATION, "AssetCompiler", "$asset: $status", details)
        }
    }
    
    /**
     * 脚本命令执行相关的专用日志方法
     */
    object Command {
        fun execute(command: String, params: String = "", details: String? = null) {
            debug(Tags.COMMAND_EXECUTION, "ScriptCommand", "$command $params", details)
        }
        
        fun npcAction(action: String, npcId: Int, details: String? = null) {
            debug(Tags.NPC_MANAGEMENT, "NPCCommand", "$action - NPC ID: $npcId", details)
        }
        
        fun mapAction(action: String, params: String, details: String? = null) {
            debug(Tags.MAP_LOADING, "MapCommand", "$action $params", details)
        }
        
        fun combatAction(action: String, details: String? = null) {
            debug(Tags.COMBAT_SYSTEM, "CombatCommand", action, details)
        }
    }
    
    /**
     * UI和输入相关的专用日志方法
     */
    object UI {
        fun screenStackAction(action: String, screenName: String, details: String? = null) {
            debug(Tags.SCREEN_MANAGEMENT, "ScreenStack", "$action: $screenName", details)
        }
        
        fun inputEvent(eventType: String, key: String, details: String? = null) {
            trace(Tags.INPUT_EVENTS, "InputHandler", "$eventType: $key", details)
        }
        
        fun playerAction(action: String, details: String? = null) {
            debug(Tags.PLAYER_ACTIONS, "PlayerInput", action, details)
        }
    }
    
    /**
     * 错误处理专用日志方法
     */
    object Error {
        fun invalidValue(component: String, field: String, value: String, expected: String) {
            error(Tags.ERROR_HANDLING, component, "无效的${field}值: $value, 期望: $expected")
        }
        
        fun notFound(component: String, resourceType: String, identifier: String) {
            error(Tags.ERROR_HANDLING, component, "未找到$resourceType: $identifier")
        }
        
        fun operationFailed(component: String, operation: String, reason: String) {
            error(Tags.ERROR_HANDLING, component, "操作失败 - $operation: $reason")
        }
    }
    
    /**
     * 兼容旧的println日志 - 提供迁移支持
     */
    @Deprecated("使用 DebugLogger 的分类方法替代", ReplaceWith("DebugLogger.info(tag, source, message)"))
    fun replacePrintln(tag: String, source: String, message: String) {
        info(tag, source, message)
    }
    
    /**
     * 兼容旧的cmdPrint日志 - 提供迁移支持  
     */
    @Deprecated("使用 DebugLogger.Command.execute() 替代", ReplaceWith("DebugLogger.Command.execute(command, params)"))
    fun replaceCmdPrint(command: String, params: String = "") {
        Command.execute(command, params)
    }
    
    /**
     * 批量日志控制预设
     */
    fun enableSystemLogging() {
        disableAllTags()
        setTagEnabled(Tags.SYSTEM_STARTUP, true)
        setTagEnabled(Tags.SCREEN_MANAGEMENT, true)
        setTagEnabled(Tags.DEV_TOOLS, true)
        setTagEnabled(Tags.ERROR_HANDLING, true)
    }
    
    fun enableScriptDebugging() {
        disableAllTags()
        setTagEnabled(Tags.COMMAND_EXECUTION, true)
        setTagEnabled(Tags.SCRIPT_EXECUTION, true)
        setTagEnabled(Tags.NPC_MANAGEMENT, true)
        setTagEnabled(Tags.MAP_LOADING, true)
        setTagEnabled(Tags.COMBAT_SYSTEM, true)
    }
    
    fun enableUIDebugging() {
        disableAllTags()
        setTagEnabled(Tags.SCREEN_MANAGEMENT, true)
        setTagEnabled(Tags.UI_EVENTS, true)
        setTagEnabled(Tags.INPUT_EVENTS, true)
        setTagEnabled(Tags.PLAYER_ACTIONS, true)
    }
    
    fun enableAssetDebugging() {
        disableAllTags()
        setTagEnabled(Tags.ASSET_COMPILATION, true)
        setTagEnabled(Tags.RESOURCE_LOADING, true)
        setTagEnabled(Tags.MAP_LOADING, true)
    }
    
    /**
     * 获取当前时间戳（简化版）
     */
    private fun getCurrentTimestamp(): String {
        // 简化时间戳实现
        try {
            val now = js("new Date()")
            val hours = now.getHours().toString().padStart(2, '0')
            val minutes = now.getMinutes().toString().padStart(2, '0')
            val seconds = now.getSeconds().toString().padStart(2, '0')
            val millis = now.getMilliseconds().toString().padStart(3, '0')
            return "$hours:$minutes:$seconds.$millis"
        } catch (e: Exception) {
            return "00:00:00.000"
        }
    }
}