package fmj.views

import fmj.graphics.Util
import fmj.scene.ScreenMainGame
import graphics.Canvas
import java.Stack

class ScreenStack(override val parent: GameNode): GameNode {
    private val mScreenStack = Stack.create<BaseScreen>()
    
    private fun getScreenName(screen: BaseScreen?): String {
        return screen?.screenName ?: "None"
    }

    fun clear() {
        mScreenStack.clear()
    }

    fun keyDown(key: Int) {
        mScreenStack.peek()?.onKeyDown(key)
    }

    fun keyUp(key: Int) {
        mScreenStack.peek()?.onKeyUp(key)
    }

    fun changeScreen(scr: BaseScreen) {
        val previousScreen = if (mScreenStack.size > 0) getScreenName(mScreenStack.peek()) else "None"
        mScreenStack.clear()
        mScreenStack.push(scr)
        println("ScreenStack: Changed from $previousScreen to ${getScreenName(scr)}")
        scr.willAppear()
    }

    override fun pushScreen(scr: BaseScreen) {
        val currentScreen = if (mScreenStack.size > 0) mScreenStack.peek()!!::class.simpleName else "None"
        mScreenStack.push(scr)
        println("ScreenStack: Pushed ${scr::class.simpleName} on top of $currentScreen")
        scr.willAppear()
    }

    override fun popScreen() {
        val poppedScreen = getScreenName(mScreenStack.peek())
        mScreenStack.pop()
        val currentScreen = getScreenName(mScreenStack.peek())
        println("ScreenStack: Popped $poppedScreen, now showing $currentScreen")
        mScreenStack.peek()?.willAppear()
    }

    override fun getCurScreen(): BaseScreen = mScreenStack.peek()!!

    override fun showMessage(msg:String, delay:Long) {
        pushScreen(object: BaseScreen {
            override val parent: GameNode
                get() = this@ScreenStack

            internal var cnt:Long = 0

            override val isPopup:Boolean
                get() {
                    return true
                }

            override fun update(delta:Long) {
                cnt += delta
                if (cnt > delay)
                {
                    popScreen()
                }
            }
            override fun draw(canvas:Canvas) {
                Util.showMessage(canvas, msg)
            }
            override fun onKeyUp(key:Int) {}
            override fun onKeyDown(key:Int) {
                popScreen()
            }
        })
    }

    fun draw(canvas: Canvas) {
        // TODO: optimize: redraw dirty layers only
        for (scr in mScreenStack) {
            scr.draw(canvas)
        }
    }
    fun update(delta: Long) {
        mScreenStack.peek()?.update(delta)
    }
}

