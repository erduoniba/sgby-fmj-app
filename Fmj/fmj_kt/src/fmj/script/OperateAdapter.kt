package fmj.script

import graphics.Canvas

abstract class OperateAdapter : Operate {

    override fun update(delta: Long): Boolean {
        return false
    }

    override fun draw(canvas: Canvas) {}

    override fun onKeyDown(key: Int) {}

    override fun onKeyUp(key: Int) {}

}
