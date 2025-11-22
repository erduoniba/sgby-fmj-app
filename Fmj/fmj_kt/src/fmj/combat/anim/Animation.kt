package fmj.combat.anim

import graphics.Canvas

interface Animation {
    fun update(delta: Long): Boolean
    fun draw(canvas: Canvas)
}

class SequencialAnimation(vararg animations: Animation): Animation {
    val animations = animations.toMutableList()

    override fun update(delta: Long): Boolean {
        if (!animations.first().update(delta)) {
            animations.removeAt(0)
        }
        return !animations.isEmpty()
    }

    override fun draw(canvas: Canvas) {
        if (animations.size == 0) return
        animations.first().draw(canvas)
    }
}
