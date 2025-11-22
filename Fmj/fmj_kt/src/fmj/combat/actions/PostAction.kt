package fmj.combat.actions

import graphics.Canvas

open class PostAction {
    open fun update(delta: Long): Boolean {
        return false
    }

    open fun draw(canvas: Canvas) {}

}
