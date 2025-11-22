package fmj.lib

import java.System

class ResGut : ResBase() {
    /**
     * 脚本说明
     */
    /**
     *
     * @return 脚本说明
     */
    var description: String = ""
        private set

    /**
     * 脚本长度，字节总数
     */
    private var mLength: Int = 0

    /**
     * 场景事件个数
     */
    private var mNumSceneEvent: Int = 0

    /**
     * 场景事件
     */
    /**
     * 场景事件，255个(1-255)。分为NPC事件、地图事件和其他事件。 NPC事件由1到40，与其资源操作号对应；地图事件由41到255，
     * 即地图编辑器中设置的事件为1，在场景中的事件为1+40=41； 其他事件可用1到255。
     *
     * @return 场景事件
     */
    lateinit var sceneEvent: IntArray
        private set

    /**
     * 脚本，格式为 指令号+数据
     */
    /**
     *
     * @return 脚本，格式为 指令号+数据
     */
    lateinit var scriptData: ByteArray
        private set

    override fun setData(buf: ByteArray, offset: Int) {
        type = buf[offset].toInt()
        index = buf[offset + 1].toInt()
        description = ResBase.Companion.getString(buf, offset + 2)
        mLength = buf[offset + 0x19].toInt() and 0xFF shl 8 or (buf[offset + 0x18].toInt() and 0xFF)
        mNumSceneEvent = buf[offset + 0x1a].toInt() and 0xFF
        sceneEvent = IntArray(mNumSceneEvent)
        for (i in 0 until mNumSceneEvent) {
            sceneEvent[i] = buf[offset + (i shl 1) + 0x1c].toInt() and 0xFF shl 8 or (buf[offset + (i shl 1) + 0x1b].toInt() and 0xFF)
        }
        val len = mLength - mNumSceneEvent * 2 - 3
        scriptData = ByteArray(len)
        System.arraycopy(buf, offset + 0x1b + mNumSceneEvent * 2,
                scriptData, 0, len)
    }

}
