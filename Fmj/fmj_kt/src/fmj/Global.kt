package fmj

import graphics.Color
import kotlin.js.JsExport

@JsExport
object Global {
    val COLOR_WHITE = Color.WHITE
    val COLOR_BLACK = Color.BLACK
    val COLOR_TRANSP = Color.TRANSP

    var fgColor = Color.BLACK
    var bgColor = Color.WHITE

    val SCREEN_WIDTH = 320
    val SCREEN_HEIGHT = 192
    
    // 🛡️ 安全开关：生产环境必须设为 false
    const val ENABLE_DEV_TOOLS = true  // 开发时 true，发布时改为 false
    
    // 📝 调试日志控制开关
    var ENABLE_DEBUG_LOGS = true  // 控制所有调试日志是否输出

    val MAP_LEFT_OFFSET = 0

    val KEY_UP = 1
    val KEY_DOWN = 2
    val KEY_LEFT = 3
    val KEY_RIGHT = 4
    val KEY_PAGEUP = 5
    val KEY_PAGEDOWN = 6
    val KEY_ENTER = 7
    val KEY_CANCEL = 8
    val KEY_REPEAT = 9
    val KEY_DEBUG = 10  // 调试菜单快捷键 (可映射到F1或其他键)
    val KEY_HELP = 11   // H键 - 游戏设置菜单

    var disableSave = false
    const val delta = 40
}

enum class ScreenViewType {
    SCREEN_DEV_LOGO,
    SCREEN_GAME_LOGO,
    SCREEN_MENU,
    SCREEN_MAIN_GAME,
    SCREEN_GAME_FAIL,
    SCREEN_SAVE_GAME,
    SCREEN_LOAD_GAME
}
