package fmj.characters

import fmj.lib.ResBase
import graphics.Canvas
import graphics.Point

abstract class Character : ResBase() {

    var name = ""

    var state = State.STOP

    /**
     * 角色在地图中的位置
     */
    val posInMap = Point()

    /**
     * 角色在地图中的面向
     */
    var direction = Direction.South
        set(d) {
            field = d
            mWalkingSprite?.setDirection(d)
        }

    /**
     * 角色行走图
     */
    private var mWalkingSprite: WalkingSprite? = null

    val walkingSpriteId: Int
        get() = mWalkingSprite?.id ?: 0

    /**
     * 设置脚步
     * @param step 0—迈左脚；1—立正；2—迈右脚
     */
    var step: Int
        get() = mWalkingSprite?.step ?: 0
        set(step) {
            mWalkingSprite?.let { it.step = step }
        }

    fun setPosInMap(x: Int, y: Int) {
        posInMap.set(x, y)
    }

    fun getPosOnScreen(posMapScreen: Point): Point {
        return Point(posInMap.x - posMapScreen.x,
                posInMap.y - posMapScreen.y)
    }

    fun setPosOnScreen(x: Int, y: Int, posMapScreen: Point) {
        posInMap.set(x + posMapScreen.x, y + posMapScreen.y)
    }

    fun setWalkingSprite(sprite: WalkingSprite) {
        mWalkingSprite = sprite
        sprite.setDirection(direction)
    }

    open fun walk() {
        mWalkingSprite!!.walk()
        updatePosInMap(direction)
    }

    open fun walk(d: Direction) {
        if (d == direction) {
            mWalkingSprite!!.walk()
        } else {
            mWalkingSprite!!.walk(d)
            direction = d
        }
        updatePosInMap(d)
    }

    private fun updatePosInMap(d: Direction) {
        when (d) {
            Direction.East -> posInMap.x++
            Direction.West -> posInMap.x--
            Direction.North -> posInMap.y--
            Direction.South -> posInMap.y++
        }
    }

    /**
     * 原地踏步
     */
    open fun walkStay(d: Direction) {
        if (d == direction) {
            mWalkingSprite!!.walk()
        } else {
            mWalkingSprite!!.walk(d)
            direction = d
        }
    }

    /**
     * 原地踏步，面向不变
     */
    fun walkStay() {
        mWalkingSprite!!.walk()
    }

    fun drawWalkingSprite(canvas: Canvas, posMapScreen: Point) {
        val p = getPosOnScreen(posMapScreen)
        mWalkingSprite!!.draw(canvas, p.x * 16, p.y * 16)
        //		if (p.x >= 0 && p.x <= 9 && p.y >= 0 && p.y <= 5) {
        //			mWalkingSprite.draw(canvas, p.x * 16, p.y * 16);
        //		}
    }

    enum class State(val v: Int) {
        /**
         * 角色的动作状态
         */
        /**
         * 停止状态，不作运动驱动
         */
        STOP(0),
        /**
         * 强制移动状态，效果同2
         */
        FORCE_MOVE(1),
        /**
         * 巡逻状态，自由行走
         */
        WALKING(2),
        /**
         * 暂停状态，等到延时到了后转变为巡逻状态
         */
        PAUSE(3),
        /**
         * 激活状态，只换图片，不改变位置（适合动态的场景对象，比如：伏魔灯）
         */
        ACTIVE(4);
        companion object {
            fun fromInt(v: Int): State {
                return when (v) {
                    0 -> STOP
                    1 -> FORCE_MOVE
                    2 -> WALKING
                    3 -> PAUSE
                    4 -> ACTIVE
                    else -> {
                        println("State.fromInt invalid v=$v")
                        STOP
                    }
                }
            }
        }
    }
}
