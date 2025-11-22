package fmj

/**
 * 统一日志接口 - 提供简化的日志方法来替代项目中的所有日志调用
 * 
 * 这个类提供了便捷的静态方法来替代 println(), console.log(), cmdPrint() 等
 * 所有日志都会通过 DebugLogger 系统进行统一管理和分类
 */
object UnifiedLogger {
    
    // === 系统级日志方法 ===
    
    /**
     * 系统启动日志 - 替代游戏初始化相关的 println
     */
    fun startup(component: String, message: String) {
        DebugLogger.System.startup(component, message)
    }
    
    /**
     * 开发工具日志 - 替代 DevTools 相关的 console.log
     */
    fun devTools(message: String, details: String? = null) {
        DebugLogger.System.devToolsStatus("INFO", message, details)
    }
    
    /**
     * 屏幕管理日志 - 替代屏幕切换相关的 println
     */
    fun screenChange(from: String, to: String) {
        DebugLogger.System.screenChange(from, to)
    }
    
    /**
     * 屏幕栈操作日志
     */
    fun screenStack(action: String, screenName: String) {
        DebugLogger.UI.screenStackAction(action, screenName)
    }
    
    /**
     * 资源编译日志 - 替代资源加载相关的 println
     */
    fun assetCompilation(asset: String, status: String) {
        DebugLogger.System.assetCompilation(asset, status)
    }
    
    // === 游戏玩法日志方法 ===
    
    /**
     * 脚本命令日志 - 替代所有 cmdPrint 调用
     */
    fun scriptCommand(command: String, params: String = "") {
        DebugLogger.Command.execute(command, params)
    }
    
    /**
     * 地图操作日志
     */
    fun mapOperation(operation: String, params: String = "") {
        DebugLogger.Command.mapAction(operation, params)
    }
    
    /**
     * NPC操作日志
     */
    fun npcOperation(operation: String, npcId: Int, details: String? = null) {
        DebugLogger.Command.npcAction(operation, npcId, details)
    }
    
    /**
     * 玩家操作日志
     */
    fun playerAction(action: String, details: String? = null) {
        DebugLogger.UI.playerAction(action, details)
    }
    
    /**
     * 战斗系统日志
     */
    fun combat(action: String, details: String? = null) {
        DebugLogger.Command.combatAction(action, details)
    }
    
    // === 错误和警告日志方法 ===
    
    /**
     * 错误日志 - 替代错误相关的 println
     */
    fun error(component: String, message: String, details: String? = null) {
        DebugLogger.error(DebugLogger.Tags.ERROR_HANDLING, component, message, details)
    }
    
    /**
     * 无效值错误
     */
    fun invalidValue(component: String, field: String, value: String, expected: String = "") {
        DebugLogger.Error.invalidValue(component, field, value, expected)
    }
    
    /**
     * 资源未找到错误
     */
    fun notFound(component: String, resourceType: String, identifier: String) {
        DebugLogger.Error.notFound(component, resourceType, identifier)
    }
    
    /**
     * 操作失败错误
     */
    fun operationFailed(component: String, operation: String, reason: String) {
        DebugLogger.Error.operationFailed(component, operation, reason)
    }
    
    // === 调试信息方法 ===
    
    /**
     * 一般调试信息 - 提供通用的调试日志
     */
    fun debug(tag: String, component: String, message: String) {
        DebugLogger.debug(tag, component, message)
    }
    
    /**
     * 信息日志 - 提供通用的信息日志
     */
    fun info(tag: String, component: String, message: String) {
        DebugLogger.info(tag, component, message)
    }
    
    /**
     * 警告日志
     */
    fun warn(tag: String, component: String, message: String) {
        DebugLogger.warn(tag, component, message)
    }
    
    // === 便捷方法用于快速迁移 ===
    
    /**
     * 快速替代 println() - 自动判断合适的标签
     */
    fun println(component: String, message: String) {
        // 根据组件名自动选择合适的标签
        val tag = when {
            component.contains("Screen", ignoreCase = true) -> DebugLogger.Tags.SCREEN_MANAGEMENT
            component.contains("Game", ignoreCase = true) -> DebugLogger.Tags.SYSTEM_STARTUP
            component.contains("Script", ignoreCase = true) -> DebugLogger.Tags.SCRIPT_EXECUTION
            component.contains("NPC", ignoreCase = true) -> DebugLogger.Tags.NPC_MANAGEMENT
            component.contains("Map", ignoreCase = true) -> DebugLogger.Tags.MAP_LOADING
            component.contains("Player", ignoreCase = true) -> DebugLogger.Tags.PLAYER_ACTIONS
            component.contains("Combat", ignoreCase = true) -> DebugLogger.Tags.COMBAT_SYSTEM
            component.contains("Error", ignoreCase = true) -> DebugLogger.Tags.ERROR_HANDLING
            component.contains("Asset", ignoreCase = true) || component.contains("Dat", ignoreCase = true) -> DebugLogger.Tags.ASSET_COMPILATION
            else -> DebugLogger.Tags.ERROR_HANDLING // 默认为通用调试
        }
        DebugLogger.info(tag, component, message)
    }
    
    /**
     * 快速替代 console.log() - DevTools 相关
     */
    fun consoleLog(message: String, details: String? = null) {
        DebugLogger.System.devToolsStatus("LOG", message, details)
    }
    
    /**
     * 快速替代 cmdPrint() - 脚本命令相关
     */
    fun cmdPrint(message: String) {
        // 解析命令和参数
        val parts = message.split(" ", limit = 2)
        val command = parts.getOrNull(0) ?: "unknown"
        val params = parts.getOrNull(1) ?: ""
        DebugLogger.Command.execute(command, params)
    }
}

/**
 * 全局扩展函数，提供更简洁的日志调用方式
 */

/**
 * 字符串扩展 - 将字符串作为日志消息输出
 */
fun String.logAsInfo(tag: String, component: String) {
    DebugLogger.info(tag, component, this)
}

fun String.logAsError(component: String) {
    DebugLogger.error(DebugLogger.Tags.ERROR_HANDLING, component, this)
}

fun String.logAsDebug(tag: String, component: String) {
    DebugLogger.debug(tag, component, this)
}

/**
 * 类扩展 - 提供基于类名的日志方法
 */
fun Any.log(tag: String, message: String) {
    val className = this::class.simpleName ?: "Unknown"
    DebugLogger.info(tag, className, message)
}

fun Any.logError(message: String) {
    val className = this::class.simpleName ?: "Unknown"
    DebugLogger.error(DebugLogger.Tags.ERROR_HANDLING, className, message)
}

fun Any.logDebug(tag: String, message: String) {
    val className = this::class.simpleName ?: "Unknown"
    DebugLogger.debug(tag, className, message)
}