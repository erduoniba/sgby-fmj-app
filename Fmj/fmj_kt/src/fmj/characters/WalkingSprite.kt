package fmj.characters

import fmj.Global
import fmj.lib.DatLib

import graphics.Canvas

class WalkingSprite(type: Int, id: Int) {
    private val resImage = DatLib.getACP(type, id)!!
    private var offset = 1 // 面向

    val id: Int
        get() = resImage.index

    var step: Int = 0
        set(step) {
            field = step % 4
        }

    fun setDirection(d: Direction) {
        offset = when (d) {
            Direction.North -> 1
            Direction.East -> 4
            Direction.South -> 7
            Direction.West -> 10
        }
    }

    fun walk(d: Direction) {
        setDirection(d)
        walk()
    }

    fun walk() {
        ++this.step
        this.step %= 4
    }

    fun draw(canvas: Canvas, x: Int, y: Int) {
        var tmpY = y
        tmpY = tmpY + 16 - resImage.height
        if (x + resImage.width > 0 && x < Global.SCREEN_WIDTH - 16 &&
                tmpY + resImage.height > 0 && tmpY < Global.SCREEN_HEIGHT) {
            resImage.draw(canvas, offset + OFFSET[this.step], x + Global.MAP_LEFT_OFFSET, tmpY)
        }
    }

    companion object {
        private val OFFSET = intArrayOf(0, 1, 2, 1)
    }
}
