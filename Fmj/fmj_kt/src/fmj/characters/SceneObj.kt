package fmj.characters

import java.ObjectOutput
import java.ObjectInput

class SceneObj : NPC() {

    override fun setData(buf: ByteArray, offset: Int) {
        type = buf[offset].toInt() and 0xFF
        index = buf[offset + 1].toInt() and 0xFF
        // 动作状态
        state = State.fromInt(buf[offset + 4].toInt() and 0xFF)
        // 姓名
        name = getString(buf, offset + 9)
        // 延时
        mDelay = buf[offset + 0x15].toInt() and 0xFF
        // 行走图
        setWalkingSprite(WalkingSprite(4, buf[offset + 0x16].toInt() and 0xFF))
        // 面向
        direction = Direction.North
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
        out.writeInt(posInMap.x)
        out.writeInt(posInMap.y)
    }

    override fun decode(coder: ObjectInput) {
        type = coder.readInt()
        index = coder.readInt()
        state = State.fromInt(coder.readInt())
        name = coder.readString()
        mDelay = coder.readInt()
        setWalkingSprite(WalkingSprite(4, coder.readInt()))
        direction = Direction.fromInt(coder.readInt())
        step = coder.readInt()
        setPosInMap(coder.readInt(), coder.readInt())
    }

    override fun walk() {}

    override fun walk(d: Direction) {}

    override fun walkStay(d: Direction) {}

    // @Override
    // public void drawWalkingSprite(Canvas canvas, Point posMapScreen) {
    // }

}
