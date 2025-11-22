package fmj.script

import graphics.Canvas

interface Command {
    val description: String?

    /**
     * 指令长度
     */
    val len: Int
    /**
     * 处理一条指令
     *
     * @return `Operate`继续执行
     *
     *
     * `null`指令执行完毕
     */
    fun run(p: ScriptProcess): Operate?
}

interface Operate {
    /**
     *
     * @param delta
     * @return `false`退出当前操作
     */
    fun update(delta: Long): Boolean

    fun draw(canvas: Canvas)

    fun onKeyDown(key: Int)

    fun onKeyUp(key: Int)
}

