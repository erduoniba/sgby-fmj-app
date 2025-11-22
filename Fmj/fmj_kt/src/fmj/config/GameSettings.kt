package fmj.config

import java.*

/**
 * 游戏设置管理
 * 统一管理所有游戏相关的设置选项，支持浏览器存储持久化
 */
object GameSettings {

    // 存储键前缀 - 使用"sav/"前缀确保保存到localStorage而不是内存
    private const val STORAGE_KEY_PREFIX = "sav/fmj_settings_"

    // 内部存储变量
    private var _useOriginalDamageFormula = true
    private var _enableEnhancedLimits = false
    private var _showCoordinates = true
    private var _allowWallWalking = false
    private var _allowMiss = false

    /**
     * 是否使用原版伤害公式
     */
    var useOriginalDamageFormula: Boolean
        get() = _useOriginalDamageFormula
        set(value) {
            _useOriginalDamageFormula = value
            saveSetting("useOriginalDamageFormula", value.toString())
        }

    /**
     * 是否启用属性上限增强
     * 开启后：HP/MP/攻击/防御上限从999提升到9999，速度/灵力/幸运上限从99提升到127
     */
    var enableEnhancedLimits: Boolean
        get() = _enableEnhancedLimits
        set(value) {
            _enableEnhancedLimits = value
            saveSetting("enableEnhancedLimits", value.toString())
        }

    /**
     * 是否显示地图坐标信息
     */
    var showCoordinates: Boolean
        get() = _showCoordinates
        set(value) {
            _showCoordinates = value
            saveSetting("showCoordinates", value.toString())
        }

    /**
     * 是否允许穿墙模式
     */
    var allowWallWalking: Boolean
        get() = _allowWallWalking
        set(value) {
            _allowWallWalking = value
            saveSetting("allowWallWalking", value.toString())
        }

    /**
     * 是否启用Miss功能
     */
    var allowMiss: Boolean
        get() = _allowMiss
        set(value) {
            _allowMiss = value
            saveSetting("allowMiss", value.toString())
        }

    /**
     * Miss功能切换次数计数器（用于解锁穿墙模式）
     * 注意：此计数器不保存，每次游戏重启后重置
     */
    var missToggleCount = 0

    /**
     * 是否解锁穿墙模式
     */
    val isWallWalkingUnlocked: Boolean
        get() = missToggleCount >= 10

    // 保存设置到浏览器存储
    private fun saveSetting(key: String, value: String) {
        try {
            sysStorageSet("${STORAGE_KEY_PREFIX}$key", value)
        } catch (e: Exception) {
            println("Failed to save setting '$key': ${e.message}")
        }
    }

    // 从浏览器存储加载设置
    private fun loadSetting(key: String, defaultValue: String): String {
        return try {
            val fullKey = "${STORAGE_KEY_PREFIX}$key"
            if (sysStorageHas(fullKey)) {
                sysStorageGet(fullKey) ?: defaultValue
            } else {
                defaultValue
            }
        } catch (e: Exception) {
            println("Failed to load setting '$key': ${e.message}")
            defaultValue
        }
    }

    /**
     * 重置所有设置为默认值并清除 localStorage
     */
    fun resetToDefaults() {
        try {
            // 清除所有保存的设置
            val settingKeys = listOf(
                "useOriginalDamageFormula",
                "enableEnhancedLimits",
                "showCoordinates",
                "allowWallWalking",
                "allowMiss"
            )

            settingKeys.forEach { key ->
                val fullKey = "${STORAGE_KEY_PREFIX}$key"
                if (sysStorageHas(fullKey)) {
                    sysStorageSet(fullKey, null)
                }
            }

            // 重置内部变量为默认值
            _useOriginalDamageFormula = true
            _enableEnhancedLimits = false
            _showCoordinates = true
            _allowWallWalking = false
            _allowMiss = false
            missToggleCount = 0

            println("GameSettings: 所有设置已重置为默认值")
        } catch (e: Exception) {
            println("Failed to reset settings: ${e.message}")
        }
    }

    /**
     * 初始化设置系统（应用启动时调用）
     */
    fun initialize() {
        try {
            println("GameSettings: 正在从 localStorage 加载设置...")

            // 从localStorage加载所有设置
            _useOriginalDamageFormula = loadSetting("useOriginalDamageFormula", "true").toBoolean()
            _enableEnhancedLimits = loadSetting("enableEnhancedLimits", "false").toBoolean()
            _showCoordinates = loadSetting("showCoordinates", "true").toBoolean()
            _allowWallWalking = loadSetting("allowWallWalking", "false").toBoolean()
            _allowMiss = loadSetting("allowMiss", "false").toBoolean()

            println("GameSettings: 设置加载完成")
            println(getSettingsSummary())
        } catch (e: Exception) {
            println("GameSettings: 初始化失败，使用默认设置: ${e.message}")
        }
    }

    /**
     * 手动保存所有设置到 localStorage
     */
    fun saveAllSettings() {
        try {
            // 直接保存所有设置
            saveSetting("useOriginalDamageFormula", _useOriginalDamageFormula.toString())
            saveSetting("enableEnhancedLimits", _enableEnhancedLimits.toString())
            saveSetting("showCoordinates", _showCoordinates.toString())
            saveSetting("allowWallWalking", _allowWallWalking.toString())
            saveSetting("allowMiss", _allowMiss.toString())

            println("GameSettings: 所有设置已保存到 localStorage")
        } catch (e: Exception) {
            println("Failed to save all settings: ${e.message}")
        }
    }

    /**
     * 检查 localStorage 是否可用
     */
    fun isLocalStorageAvailable(): Boolean {
        return try {
            val testKey = "${STORAGE_KEY_PREFIX}test"
            sysStorageSet(testKey, "test")
            val hasTest = sysStorageHas(testKey)
            if (hasTest) {
                sysStorageSet(testKey, null) // 清理测试数据
            }
            hasTest
        } catch (e: Exception) {
            false
        }
    }

    /**
     * 获取设置摘要信息（用于调试）
     */
    fun getSettingsSummary(): String {
        return """
            游戏设置摘要:
            - 原版伤害公式: ${if (useOriginalDamageFormula) "开启" else "关闭"}
            - 属性上限增强: ${if (enableEnhancedLimits) "开启" else "关闭"}
            - 地图坐标显示: ${if (showCoordinates) "开启" else "关闭"}
            - Miss功能: ${if (allowMiss) "开启" else "关闭"}
            - 穿墙模式: ${if (allowWallWalking) "开启" else "关闭"} (解锁状态: ${if (isWallWalkingUnlocked) "已解锁" else "未解锁"})
            - Miss切换次数: $missToggleCount/10
            - LocalStorage 可用: ${if (isLocalStorageAvailable()) "是" else "否"}
        """.trimIndent()
    }
}