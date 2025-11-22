package fmj.script

import graphics.Canvas

abstract class OperateDrawOnce : Operate {
    private var drawCnt = 0

    abstract fun drawOnce(canvas: Canvas)

    override fun update(delta: Long): Boolean {
        if (drawCnt >= 3) {
            drawCnt = 0
            return false
        }
        return true
    }

    override fun draw(canvas: Canvas) {
        drawOnce(canvas)
        ++drawCnt
    }

    override fun onKeyDown(key: Int) {}

    override fun onKeyUp(key: Int) {}

}
