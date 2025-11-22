package fmj.lib

import fmj.Global
import fmj.graphics.TextRender

import graphics.Bitmap
import graphics.Canvas
import graphics.Color
import java.System

class ResImage : ResBase() {
    /**
     * 切片宽
     */
    var width: Int = 0
        protected set

    /**
     * 切片高
     */
    var height: Int = 0
        protected set

    /**
     * 切片数量
     */
    /**
     *
     * @return 切片数量
     */
    var number: Int = 0
        protected set

    /**
     * 是否透明
     */
    protected var mTransparent: Boolean = false

    /**
     * 图像数据 不透明：一位一像素，0白，1黑。
     * 透明：两位一像素，高位（0不透明，1透明），低位（0白，1黑）。
     * 注意：有冗余数据。
     */
    protected var mData: ByteArray? = null

    /**
     * 图片数组
     */
    protected var mBitmaps: Array<Bitmap>? = null
    
    init {
        // 初始化空的位图数组，避免 null 引用问题
        mBitmaps = arrayOf()
    }

    /**
     * 得到资源的大小
     * @return
     */
    val bytesCount: Int
        get() = mData!!.size + 6

    override fun setData(buf: ByteArray, offset: Int) {
        type = buf[offset].toInt() and 0xFF
        index = (buf[offset + 1].toInt() and 0xFF)
        width = buf[offset + 2].toInt() and 0xFF
        height = buf[offset + 3].toInt() and 0xFF
        number = buf[offset + 4].toInt() and 0xFF
        mTransparent = buf[offset + 5].toInt() == 2

        // 检查多种数据损坏模式
        val isCorrupted = when {
            // 所有值都是0（另一种损坏模式，但number=0是合法的空图像）
            type == 0 && index == 0 && width == 0 && height == 0 && number == 0 -> false // 这是合法的空图像
            // 异常大的尺寸值（可能导致内存溢出）
            width > 250 && height > 250 && number > 250 -> true // BBK屏幕只有160x96，超过200明显异常
            else -> false
        }
        
        if (isCorrupted) {
            // 创建空占位符资源，避免游戏崩溃
            this.type = 0
            this.index = 0
            this.width = 0
            this.height = 0
            this.number = 0
            this.mTransparent = false
            this.mData = ByteArray(0)
            createBitmaps()
            return
        }

        // 防护性检查输入参数 - 允许0值但不允许负值
        if (width < 0 || height < 0 || number < 0) {
            throw Error("ResImage.setData: negative dimensions width=$width, height=$height, number=$number")
        }
        
        // 检查合理的尺寸限制
        if (width > 255 || height > 255 || number > 255) {
            throw Error("ResImage.setData: dimensions exceed reasonable limits")
        }
        
        // 如果任何维度为0，创建一个空的图像
        if (width == 0 || height == 0 || number == 0) {
            mData = ByteArray(0)
            createBitmaps()
            return
        }
        
        val bytesPerRow = width / 8 + if (width % 8 != 0) 1 else 0
        val typeMultiplier = buf[offset + 5].toInt() and 0xFF
        
        // typeMultiplier可能为0，这是合法的（表示空图像数据）
        if (typeMultiplier == 0) {
            mData = ByteArray(0)
            createBitmaps()
            return
        }
        
        // 使用长整型计算，防止溢出
        val lenLong = number.toLong() * bytesPerRow.toLong() * height.toLong() * typeMultiplier.toLong()
        
        // 检查计算结果
        if (lenLong < 0 || lenLong > Int.MAX_VALUE) {
            throw Error("ResImage.setData: data length overflow - width=$width, height=$height, number=$number, typeMultiplier=$typeMultiplier, calculated length=$lenLong")
        }
        
        // 添加内存使用限制（10MB）
        val maxReasonableSize = 10 * 1024 * 1024
        if (lenLong > maxReasonableSize) {
            throw Error("ResImage.setData: calculated data length too large: ${lenLong} bytes")
        }
        
        val len = lenLong.toInt()
        
        // 允许长度为0的情况（空图像），但仍然要创建一个空数组
        if (len < 0) {
            throw Error("ResImage.setData: negative data length len=$len")
        }
        
        // 检查数据边界
        if (len > 0) {
            val requiredBufSize = offset + 6 + len
            if (requiredBufSize > buf.size) {
                throw Error("ResImage.setData: buffer overflow - required $requiredBufSize bytes but buffer size is ${buf.size}")
            }
        }
        
        mData = ByteArray(len)
        if (len > 0) {
            System.arraycopy(buf, offset + 6, mData!!, 0, len)
        }
        createBitmaps()
    }

    /**
     * 根据[.mData]创建位图数组
     */
    private fun createBitmaps() {
        // 如果宽度或高度为0，或者没有图片数量，创建一个空的位图数组
        if (width <= 0 || height <= 0 || number <= 0) {
            mBitmaps = arrayOf()
            return
        }
        
        // 如果数据为空或长度为0，创建空的位图数组
        if (mData == null || mData!!.isEmpty()) {
            mBitmaps = arrayOf()
            return
        }
        
        // 防护性检查
        val totalPixels = width.toLong() * height.toLong()
        if (totalPixels > Int.MAX_VALUE || totalPixels < 0) {
            throw Error("ResImage createBitmaps: dimensions too large width=$width, height=$height, total=$totalPixels")
        }
        
        var iOfData = 0
        mBitmaps = Array(number) {
            val tmp = Array((width * height).toInt()) { Color.WHITE }
            if (mTransparent) {
                var cnt = 0
                var iOfTmp = 0
                for (y in 0 until height) {
                    for (x in 0 until width) {
                        if (iOfData < mData!!.size && mData!![iOfData].toInt() shl cnt and 0x80 != 0) {
                            tmp[iOfTmp] = Global.COLOR_TRANSP
                        } else {
                            tmp[iOfTmp] = if (iOfData < mData!!.size && mData!![iOfData].toInt() shl cnt shl 1 and 0x80 != 0)
                                Global.COLOR_BLACK
                            else
                                Global.COLOR_WHITE
                        }
                        ++iOfTmp
                        cnt += 2
                        if (cnt >= 8) {
                            cnt = 0
                            ++iOfData
                        }
                    }

                    if (cnt in 1..7) {
                        cnt = 0
                        ++iOfData
                    }
                    if (iOfData % 2 != 0) ++iOfData
                }
                // RETURN:
                Bitmap.createBitmap(tmp, width, height)
            } else { // 不透明
                var cnt = 0
                var iOfTmp = 0
                for (y in 0 until height) {
                    for (x in 0 until width) {
                        tmp[iOfTmp++] = if (iOfData < mData!!.size && mData!![iOfData].toInt() shl cnt and 0x80 != 0)
                            Global.COLOR_BLACK
                        else
                            Global.COLOR_WHITE
                        if (++cnt >= 8) {
                            cnt = 0
                            ++iOfData
                        }
                    }
                    if (cnt != 0) { // 不足一字节的舍去
                        cnt = 0
                        ++iOfData
                    }
                } // end for (int y = ...
                // RETURN:
                Bitmap.createBitmap(tmp, width, height)
            } // end if
        }
    }

    /**
     *
     * @param canvas
     * 目标画布
     * @param num
     * 要画的切片编号,>0
     * @param left
     * 画到画布的最左端位置
     * @param top
     * 画到画布的最上端位置
     */
    fun draw(canvas: Canvas, num: Int, left: Int, top: Int) {
        // 检查是否有有效的位图数组
        if (mBitmaps == null || mBitmaps!!.isEmpty()) {
            // 如果没有位图，显示占位符
            TextRender.drawText(canvas, "烫", left, top)
            return
        }

        // 确保索引在有效范围内
        if (num <= number && num > 0 && num <= mBitmaps!!.size) {
            val bitmap = mBitmaps!![num - 1]
            if (bitmap != null) {
                canvas.drawBitmap(bitmap, left, top)
            }
        } else {
            if (number > 0) {
                canvas.drawBitmap(mBitmaps!![0], left, top)
            } else {
                TextRender.drawText(canvas, "烫", left, top)
            }
        }
    }

    fun getBitmap(index: Int): Bitmap? {
        // 检查是否有有效的位图数组
        if (mBitmaps == null || mBitmaps!!.isEmpty() || index >= number || index < 0) {
            return null
        }
        return mBitmaps!![index].copy()
    }
}
