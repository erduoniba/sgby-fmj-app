package fmj.graphics

import fmj.lib.DatLib
import fmj.lib.ResImage

import graphics.Canvas

class Tiles(index: Int) {

    private val mTileRes: ResImage = run {
        val res = DatLib.getRes(DatLib.ResType.TIL, 1, index, false)
        if (res is ResImage) {
            res
        } else {
            println("Warning: Failed to load tile $index, using empty image")
            // 创建一个空的 ResImage 作为回退
            val emptyRes = ResImage()
            val emptyData = ByteArray(6)
            emptyData[0] = 1.toByte()
            emptyData[1] = index.toByte()
            emptyRes.setData(emptyData, 0)
            emptyRes
        }
    }

    /**
     *
     * @param canvas
     * @param x
     * @param y
     * @param i
     * 图块的序号
     */
    fun draw(canvas: Canvas, x: Int, y: Int, i: Int) {
        mTileRes.draw(canvas, i + 1, x, y)
    }

    companion object {
        /**
         * 地图块的宽
         */
        val WIDTH = 16
        /**
         * 地图块的高
         */
        val HEIGHT = 16
    }
}
