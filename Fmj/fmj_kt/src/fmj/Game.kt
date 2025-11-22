package fmj

import fmj.characters.Player
import fmj.combat.Combat
import fmj.config.GameSettings
import fmj.scene.ScreenMainGame
import fmj.scene.SaveLoadGame
import fmj.script.ScriptVM
import fmj.views.*

import graphics.Canvas
import graphics.Bitmap
import java.*
// 统一日志系统 - 已实现但暂不引入避免影响现有功能
// import fmj.UnifiedLogger

class MainGame: Game {
    companion object {
        // 静态实例引用，供开发工具使用
        var instance: MainGame? = null
    }
    val version  = "2.1.2"
    private  val canvas = Canvas(Bitmap(Global.SCREEN_WIDTH, Global.SCREEN_HEIGHT))
    private  val screenStack = ScreenStack(this)

    override val vm = ScriptVM(this)
    override val parent get() = screenStack
    override val game = this
    override var mainScene: ScreenMainGame
    override val playerList = arrayListOf<Player>()
    override val bag get() = Player.sGoodsList
    override val combat get() = Combat.Companion


    constructor() {
        // 设置静态实例引用
        instance = this
        mainScene = ScreenMainGame(this, vm)
    }

    fun start() {
        // 在游戏启动时设置全局引用
        setupGlobalWindow()

        // 初始化游戏设置（从 localStorage 加载）
        GameSettings.initialize()

        listenUIEvents()
        val scr = ScreenAnimation(this, 247)
        screenStack.pushScreen(scr)
    }
    
    /**
     * 设置游戏实例到浏览器全局window对象
     * JavaScript执行代码已从原setToGlobalWindow方法重构，提高可维护性
     */
    private fun setupGlobalWindow() {
        try {
            executeWindowSetupScript()
        } catch (e: Exception) {
            println("Failed to set window.fmjGame: ${e.message}")
        }
    }
    
    /**
     * 执行窗口设置的JavaScript代码
     * 抽离出来便于维护，同时避免线上构建时的外部依赖问题
     */
    private fun executeWindowSetupScript() {
        js("""
            setTimeout(function() {
                if (typeof window !== 'undefined') {
                    window.fmjGame = this;
                    
                    // 🛡️ 安全检查：只在开发模式下初始化开发工具
                    if (""" + Global.ENABLE_DEV_TOOLS + """) {
                        console.log('🔧 Development mode: DevTools enabled');
                        
                        // 暴露 DevToolsIntegration 到全局
                        if (typeof window.fmj === 'undefined') {
                            window.fmj = {};
                        }
                        if (typeof window.fmj.devtools === 'undefined') {
                            window.fmj.devtools = {};
                        }
                        // 暴露编译后的 DevToolsIntegration
                        if (typeof fmj !== 'undefined' && typeof fmj.devtools !== 'undefined' && typeof fmj.devtools.DevToolsIntegration !== 'undefined') {
                            window.fmj.devtools.DevToolsIntegration = fmj.devtools.DevToolsIntegration;
                            console.log('✅ DevToolsIntegration 已暴露到 window.fmj.devtools.DevToolsIntegration');
                        } else {
                            console.warn('⚠️ DevToolsIntegration 编译后对象未找到');
                        }
                        
                        // 如果外部DevTools模块可用，初始化它
                        if (typeof window.FMJDevTools !== 'undefined') {
                            try {
                                window.FMJDevTools.initialize(this);
                            } catch(e) {
                                console.warn('DevTools module found but initialization failed:', e);
                            }
                        } else {
                            console.log('🔧 DevTools module not loaded - this is normal for production builds');
                        }
                    } else {
                        console.log('🛡️ Production mode: DevTools disabled for security');
                    }
                }
            }.bind(this), 100)
        """)
    }

    fun draw() {
        screenStack.draw(canvas)
    }

    fun update(delta: Long) {
        screenStack.update(delta)
    }

    private fun keyDown(key: Int) {
        screenStack.keyDown(key)
    }

    private fun keyUp(key: Int) {
        screenStack.keyUp(key)
    }

    override fun changeScreen(screenType: ScreenViewType) {
        println("Changing screen to: $screenType")
        val scr: BaseScreen =
                when (screenType) {
                    ScreenViewType.SCREEN_DEV_LOGO -> ScreenAnimation(this, 247)
                    ScreenViewType.SCREEN_GAME_LOGO -> ScreenAnimation(this, 248)
                    ScreenViewType.SCREEN_MENU -> ScreenMenu(this)
                    ScreenViewType.SCREEN_MAIN_GAME -> {
                        mainScene = ScreenMainGame(this, vm)
                        mainScene
                    }
                    ScreenViewType.SCREEN_GAME_FAIL -> ScreenAnimation(this, 249)
                    ScreenViewType.SCREEN_SAVE_GAME -> ScreenSaveLoadGame(this, ScreenSaveLoadGame.Operate.SAVE)
                    ScreenViewType.SCREEN_LOAD_GAME -> ScreenSaveLoadGame(this, ScreenSaveLoadGame.Operate.LOAD)
                }
        println("Created screen instance: ${when (screenType) {
            ScreenViewType.SCREEN_DEV_LOGO -> "ScreenAnimation(DEV_LOGO)"
            ScreenViewType.SCREEN_GAME_LOGO -> "ScreenAnimation(GAME_LOGO)"
            ScreenViewType.SCREEN_MENU -> "ScreenMenu"
            ScreenViewType.SCREEN_MAIN_GAME -> "ScreenMainGame"
            ScreenViewType.SCREEN_GAME_FAIL -> "ScreenAnimation(GAME_FAIL)"
            ScreenViewType.SCREEN_SAVE_GAME -> "ScreenSaveLoadGame(SAVE)"
            ScreenViewType.SCREEN_LOAD_GAME -> "ScreenSaveLoadGame(LOAD)"
        }}")
        screenStack.changeScreen(scr)
    }

    fun listenUIEvents() {
        val delta = Global.delta.toLong()
        sysAddKeyDownListener {
            keyDown(it)
        }
        sysAddKeyUpListener {
            keyUp(it)
        }

        // sysSetInterval 现在内部处理速度控制，这里只传递标准间隔
        sysSetInterval(Global.delta) {
            update(delta)
            draw()
            sysDrawScreen(canvas.buffer, canvas.width, canvas.height)
        }
    }
}
val game = MainGame()
fun main(args: Array<String>) {
    game.start()
}
