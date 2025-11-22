package fmj.characters

import java.Random
import java.Coder
import java.ObjectOutput
import java.ObjectInput


open class NPC : Character(), Coder {

    /**
     * 暂停状态，等到延时到了后转变为巡逻状态
     */
    protected var mDelay: Int = 0

    private var mCanWalk: ICanWalk? = null

    private val mRandom = Random()
    private var mPauseCnt = (mDelay * 100).toLong()
    private var mActiveCnt: Long = 0
    private var mWalkingCnt: Long = 0

    override fun setData(buf: ByteArray, offset: Int) {
        type = buf[offset].toInt() and 0xFF
        index = buf[offset + 1].toInt() and 0xFF
        // 动作状态
        val st = buf[offset + 4].toInt() and 0xFF
        state = State.fromInt(st)
        // 姓名
        name = getString(buf, offset + 9)
        // 延时
        mDelay = buf[offset + 0x15].toInt() and 0xFF
        if (mDelay == 0) {
            state = State.STOP
        }
        // 行走图
        setWalkingSprite(WalkingSprite(2,
                buf[offset + 0x16].toInt() and 0xFF))
        // 面向
        val faceto = buf[offset + 2].toInt() and 0xFF
        direction = Direction.fromInt(faceto)
        // 脚步
        step = buf[offset + 3].toInt() and 0xFF
    }

    override fun encode(out: ObjectOutput) {
        out.writeInt(type)
        out.writeInt(index)
        out.writeInt(state.v)
        out.writeString(name)
        out.writeInt(mDelay)
        out.writeInt(walkingSpriteId)
        out.writeInt(direction.v)
        out.writeInt(step)
        out.writeLong(mPauseCnt)
        out.writeLong(mActiveCnt)
        out.writeLong(mWalkingCnt)
        out.writeInt(posInMap.x)
        out.writeInt(posInMap.y)
    }

    override fun decode(coder: ObjectInput) {
        type = coder.readInt()
        index = coder.readInt()
        state = State.fromInt(coder.readInt())
        name = coder.readString()
        mDelay = coder.readInt()
        setWalkingSprite(WalkingSprite(2, coder.readInt()))
        direction = Direction.fromInt(coder.readInt())
        step = coder.readInt()
        mPauseCnt = coder.readLong()
        mActiveCnt = coder.readLong()
        mWalkingCnt = coder.readLong()
        setPosInMap(coder.readInt(), coder.readInt())
    }

    interface ICanWalk {
        fun canWalk(x: Int, y: Int): Boolean
    }

    fun setICanWalk(arg: ICanWalk) {
        mCanWalk = arg
    }

    fun update(delta: Long) {
        when (state) {
            State.PAUSE -> {
                mPauseCnt -= delta
                if (mPauseCnt < 0) {
                    state = State.WALKING
                }
            }

            State.FORCE_MOVE, State.WALKING -> {
                mWalkingCnt += delta
                if (mWalkingCnt < 500) return
                mWalkingCnt = 0
                if (mRandom.nextInt(5) == 0) { // 五分之一的概率暂停
                    mPauseCnt = (mDelay * 100).toLong()
                    state = State.PAUSE
                } else if (mRandom.nextInt(5) == 0) { // 五分之一的概率改变方向
                    val i = mRandom.nextInt(4)
                    var d = Direction.North
                    when (i) {
                        0 -> d = Direction.North
                        1 -> d = Direction.East
                        2 -> d = Direction.South
                        3 -> d = Direction.West
                    }
                    direction = d
                    walk()
                } else {
                    walk()
                }
            }

            State.STOP -> {
            }

            State.ACTIVE -> {
                mActiveCnt += delta
                if (mActiveCnt > 100) {
                    mActiveCnt = 0
                    walkStay()
                }
            }
        }
    }

    override fun walk() {
        var x = posInMap.x
        var y = posInMap.y
        when (direction) {
            Direction.North -> --y
            Direction.East -> ++x
            Direction.South -> ++y
            Direction.West -> --x
        }
        if (mCanWalk!!.canWalk(x, y)) {
            super.walk()
        }
    }

    val isEmpty
        get() = type == 0

    companion object {
        val empty = NPC()
    }
}
