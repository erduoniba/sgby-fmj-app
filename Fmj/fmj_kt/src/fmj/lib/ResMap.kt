package fmj.lib

import fmj.Global
import fmj.graphics.TextRender
import fmj.graphics.Tiles

import graphics.Canvas
import graphics.Color
import graphics.Paint
import java.System
import java.gbkString

class ResMap : ResBase() {

    /**
     * 该地图所用的til图块资源的索引号
     */
    private var mTilIndex: Int = 0

    /**
     * 地图名称
     */
    var mapName: String? = null
        private set

    /**
     * 地图宽
     */
    var mapWidth: Int = 0
        private set

    /**
     * 地图高
     */
    var mapHeight: Int = 0
        private set

    /**
     * 地图数据 两个字节表示一个地图快（从左到右，从上到下）
     * （低字节：最高位1表示可行走，0不可行走。高字节：事件号）
     */
    private var mData: ByteArray? = null

    /**
     * 地图使用的地图块
     */
    private var mTiles: Tiles? = null

    override fun setData(buf: ByteArray, offset: Int) {
        type = buf[offset].toInt() and 0xFF
        index = buf[offset + 1].toInt() and 0xFF
        mTilIndex = buf[offset + 2].toInt()

        var i = 0
        while (buf[offset + 3 + i].toInt() != 0)
            ++i
        mapName = buf.gbkString(offset + 3, i)

        mapWidth = buf[offset + 0x10].toInt()
        mapHeight = buf[offset + 0x11].toInt()

        val len = mapWidth * mapHeight * 2
        mData = ByteArray(len)
        System.arraycopy(buf, offset + 0x12, mData!!, 0, len)
    }

    /**
     * 判断地图(x,y)是否可行走
     * @param x
     * @param y
     * @return
     */
    fun canWalk(x: Int, y: Int): Boolean {
        if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) {
            return false
        }

        val i = y * mapWidth + x
        return mData!![i * 2].toInt() and 0x80 != 0
    }

    fun canPlayerWalk(x: Int, y: Int): Boolean {
        return (canWalk(x, y) && x >= 4 && x < mapWidth - 4
                && y >= 3 && y < mapHeight - 2)
    }

    fun getEventNum(x: Int, y: Int): Int {
        if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) {
            return -1
        }

        val i = y * mapWidth + x
        return mData!![i * 2 + 1].toInt() and 0xFF
    }

    /**
     *
     * @param x
     * 图块的x坐标
     * @param y
     * 图块的y坐标
     * @return map中(x, y)位置的图块在til中的序号
     */
    private fun getTileIndex(x: Int, y: Int): Int {
        val i = y * mapWidth + x
        return mData!![i * 2].toInt() and 0x7F
    }

    /**
     * 水平方向 left --- left+WIDTH <br></br>
     * 竖直方向 top --- top + HEIGHT
     *
     * @param canvas
     * @param left
     * 地图的最左边
     * @param top
     * 地图的最上边
     */
    fun drawMap(canvas: Canvas, left: Int, top: Int, treasureBoxes: List<TreasureBoxInfo> = emptyList(), showEvents: Boolean = false) {
        if (mTiles == null) {
            mTiles = Tiles(mTilIndex)
        }

        // 绘制完整的视口，使用相邻图块填充空白区域
        for (y in 0 until HEIGHT) {
            for (x in 0 until WIDTH) {
                val mapX = left + x
                val mapY = top + y
                
                // 计算要使用的图块坐标（处理边界情况）
                val tileX = when {
                    mapX < mapWidth -> mapX
                    mapWidth > 0 -> mapWidth - 1  // 使用最右边的图块
                    else -> 0
                }
                
                val tileY = when {
                    mapY < mapHeight -> mapY
                    mapHeight > 0 -> mapHeight - 1  // 使用最下面的图块
                    else -> 0
                }
                
                // 确保坐标在有效范围内
                val safeTileX = maxOf(0, minOf(tileX, mapWidth - 1))
                val safeTileY = maxOf(0, minOf(tileY, mapHeight - 1))
                
                mTiles!!.draw(canvas, x * Tiles.WIDTH + Global.MAP_LEFT_OFFSET,
                        y * Tiles.HEIGHT, getTileIndex(safeTileX, safeTileY))
                
                // 检查该位置是否有事件点或宝箱（只在视口范围内且坐标有效时检查）
                if (mapX >= 0 && mapX < mapWidth && mapY >= 0 && mapY < mapHeight) {
                    val sx = x * Tiles.WIDTH + Global.MAP_LEFT_OFFSET + 2
                    val sy = y * Tiles.HEIGHT + 2
                    
                    // 检查该位置是否有宝箱（宝箱优先级更高）
                    val treasureBox = treasureBoxes.find { it.x == mapX && it.y == mapY }
                    if (treasureBox != null) {
                        // 根据宝箱状态绘制不同颜色的指示符
                        val fillColor = if (treasureBox.isCollected) Color.GRAY else Color.BLUE
                        val fillPaint = Paint().apply {
                            color = fillColor
                            style = Paint.Style.FILL
                        }
                        val blackPaint = Paint().apply {
                            color = Color.BLACK
                            style = Paint.Style.STROKE
                            strokeWidth = 1
                        }
                        // 填充颜色（蓝色=未获取，灰色=已获取）
                        canvas.drawRect(sx + 4, sy + 4, sx + 8, sy + 8, fillPaint)
                        // 黑色边框
                        canvas.drawRect(sx + 4, sy + 4, sx + 8, sy + 8, blackPaint)
                    } else if (showEvents) {
                        // 如果没有宝箱但有事件点，绘制事件点指示符
                        val eventNum = getEventNum(mapX, mapY)
                        if (eventNum != 0) {
                            // 绘制事件点指示符（红色小圆点）
                            val redPaint = Paint().apply {
                                color = Color.RED
                                style = Paint.Style.FILL
                            }
                            // 绘制小方块表示事件点
                            canvas.drawRect(sx + 4, sy + 4, sx + 8, sy + 8, redPaint)
                        }
                    }
                }
            }
        }
    }
    
    data class TreasureBoxInfo(val x: Int, val y: Int, val name: String, val isCollected: Boolean = false)

    fun drawWholeMap(canvas: Canvas, x: Int, y: Int, showEvents: Boolean = true, treasureBoxes: List<TreasureBoxInfo> = emptyList()) {
        if (mTiles == null) {
            mTiles = Tiles(mTilIndex)
        }

        for (ty in 0 until mapHeight) {
            for (tx in 0 until mapWidth) {
                val sx = tx * Tiles.WIDTH + x
                val sy = ty * Tiles.HEIGHT + y
                mTiles!!.draw(canvas, sx, sy, getTileIndex(tx, ty))
                
                // 检查该位置是否有宝箱（宝箱优先级更高）
                val treasureBox = treasureBoxes.find { it.x == tx && it.y == ty }
                if (treasureBox != null) {
                    // 根据宝箱状态绘制不同颜色的指示符
                    val fillColor = if (treasureBox.isCollected) Color.GRAY else Color.BLUE
                    val fillPaint = Paint().apply {
                        color = fillColor
                        style = Paint.Style.FILL
                    }
                    val blackPaint = Paint().apply {
                        color = Color.BLACK
                        style = Paint.Style.STROKE
                        strokeWidth = 1
                    }
                    // 填充颜色（蓝色=未获取，灰色=已获取）
                    canvas.drawRect(sx + 4, sy + 4, sx + 12, sy + 12, fillPaint)
                    // 黑色边框
                    canvas.drawRect(sx + 4, sy + 4, sx + 12, sy + 12, blackPaint)
                } else {
                    // 如果没有宝箱，检查是否有事件点
                    val event = getEventNum(tx, ty)
                    if (showEvents && event != 0) {
                        // TODO: refactor
                        Global.bgColor = Color.RED
                        TextRender.drawText(canvas, event.toString(), sx, sy)
                        Global.bgColor = Global.COLOR_WHITE
                    }
                }
            }
        }
    }

    companion object {
        /**
         * 横向渲染的地图块总数
         */
        val WIDTH = Global.SCREEN_WIDTH / 16 - 1

        /**
         * 纵向渲染的地图块总数
         */
        val HEIGHT = Global.SCREEN_HEIGHT / 16
    }
}
