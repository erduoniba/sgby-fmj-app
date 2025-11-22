package fmj.graphics

import fmj.Global
import fmj.lib.DatLib
import fmj.lib.ResImage

import graphics.Bitmap
import graphics.Canvas
import graphics.Paint
import graphics.Paint.Style
import graphics.Rect
import java.gbkBytes

object Util {

    // 显示message的方框
    private var bmpInformationBg: Array<Bitmap>

    // 屏幕两边留白
    private var bmpSideFrame: Bitmap

    // 用于菜单的矩形框，黑框白边
    private var drawFramePaint = Paint()

    private var bmpTriangleCursor: Bitmap

    private var imgSmallNum: ResImage

    var bmpChuandai: Bitmap

    var sBlackPaint: Paint

    init {
        val canvas = Canvas()
        val paint = Paint()
        paint.color = Global.COLOR_WHITE
        paint.style = Style.FILL_AND_STROKE

        var ind = 0
        bmpInformationBg = Array(5) {
            val bmp = Bitmap.createBitmap(240, 23 + 16 * ind)
            canvas.setBitmap(bmp)
            canvas.drawColor(Global.COLOR_BLACK)
            canvas.drawRect(1, 1, 237, 20 + 16 * ind, paint)
            canvas.drawRect(238, 0, 240, 3, paint)
            canvas.drawLine(0, 21 + 16 * ind, 3, 21 + 16 * ind, paint)
            canvas.drawLine(0, 22 + 16 * ind, 3, 22 + 16 * ind, paint)
            ind++
            bmp
        }

        bmpSideFrame = Bitmap.createBitmap(8, Global.SCREEN_HEIGHT)
        canvas.setBitmap(bmpSideFrame)
        canvas.drawColor(Global.COLOR_WHITE)
        paint.color = Global.COLOR_BLACK

        for (i in 0..3) {
            canvas.drawLine(i*2, 0, i*2, Global.SCREEN_HEIGHT, paint)
        }

        bmpTriangleCursor = Bitmap.createBitmap(7, 13)
        canvas.setBitmap(bmpTriangleCursor)
        canvas.drawColor(Global.COLOR_WHITE)
        for (i in 0..6) {
            canvas.drawLine(i, i, i, 13 - i, paint)
        }

        imgSmallNum = DatLib.getRes(DatLib.ResType.PIC, 2, 5) as ResImage

        bmpChuandai = Bitmap.createBitmap(22, 39)
        val b = Global.COLOR_BLACK
        val w = Global.COLOR_WHITE
        val pixels = arrayOf(w, w, w, w, w, w, w, w, w, b, b, b, w, w, w, w, w, w, w, w, w, w, w, w, w, b, b, w, w, b, b, b, b, b, b, b, b, b, b, b, b, b, w, w, w, w, b, b, b, b, b, w, w, w, w, w, w, w, w, b, b, b, b, b, w, w, w, w, b, b, w, w, w, b, b, b, w, w, b, b, b, w, w, w, b, b, w, w, w, w, w, b, w, w, b, w, w, w, w, w, w, w, w, w, w, w, b, b, w, w, w, w, w, b, w, w, b, b, b, b, b, b, b, b, b, b, b, w, w, w, w, w, w, w, w, w, w, w, w, b, w, w, w, w, b, b, w, w, w, w, w, w, w, w, w, w, w, w, w, w, b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, w, w, w, w, w, w, b, b, w, w, w, b, b, b, b, w, b, b, b, b, b, b, w, w, w, w, w, w, w, w, w, b, b, b, b, b, b, w, w, w, b, b, b, w, w, w, w, w, w, w, b, b, b, b, w, w, b, b, b, w, w, w, w, w, w, w, b, b, b, b, b, b, b, b, w, w, w, w, b, b, b, w, w, w, w, w, w, w, w, b, b, b, b, w, w, w, w, w, w, w, b, b, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, b, b, b, b, b, b, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, b, b, b, b, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, b, w, w, w, w, b, b, w, w, w, b, w, w, w, w, w, w, w, w, b, b, b, b, b, b, b, w, b, b, w, w, b, b, w, w, w, w, w, w, w, b, b, w, w, b, w, w, w, w, b, b, w, b, w, w, w, w, w, w, w, w, w, b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, w, w, w, w, b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, w, w, w, w, w, w, w, w, b, b, b, b, b, b, b, w, b, b, w, b, b, w, w, w, w, w, w, w, w, w, b, b, w, b, b, b, b, w, b, b, w, b, b, w, w, w, w, w, w, w, w, w, b, w, b, b, w, w, b, w, w, b, w, b, b, w, w, w, w, w, w, w, w, w, b, b, b, b, b, b, b, w, w, b, b, b, b, w, w, w, w, w, w, w, w, w, b, b, b, b, b, b, b, w, w, b, b, b, w, w, w, w, w, w, w, w, w, w, w, b, b, b, b, b, w, w, w, w, b, b, b, w, w, w, w, w, w, b, b, b, b, b, w, w, w, w, b, b, b, b, b, b, b, b, b, b, w, w, w, w, w, w, b, b, b, w, w, b, b, w, b, b, w, w, b, b, b, b, b, b, b, w, w, b, b, w, w, w, w, w, w, b, w, w, w, w, w, b, b, b, b, b, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, b, b, w, w)
        bmpChuandai.setPixels(pixels, 0, 0, 0, 22, 39)
    }

    // 用于showscenename
    fun showInformation(canvas: Canvas, msg: String) {
        // 居中显示在320x192的屏幕上
        val boxWidth = 240
        val boxX = (Global.SCREEN_WIDTH - boxWidth) / 2  // (320 - 240) / 2 = 40
        val boxY = (Global.SCREEN_HEIGHT - 23) / 2  // 单行消息框高度为23
        
        canvas.drawBitmap(bmpInformationBg[0], boxX, boxY)
        // 文字居中在框内
        val textWidth = msg.gbkBytes().size * 8  // 估算文字宽度
        val textX = boxX + (boxWidth - textWidth) / 2
        TextRender.drawText(canvas, msg, textX, boxY + 2)
    }

    // 显示message,每行最多显示8个汉字，最多可显示5行
    fun showMessage(canvas: Canvas, msg: String) {
        showMessage(canvas, msg.gbkBytes())
    }

    // 显示message,适配320x192屏幕，每行最多显示14个汉字，最多可显示5行
    fun showMessage(canvas: Canvas, msg: ByteArray) {
        // 每个汉字16像素宽，240像素可以显示15个汉字，留出边距显示14个
        val charsPerLine = 28  // 14个汉字 = 28字节
        var lineNum = msg.size / charsPerLine
        if (msg.size % charsPerLine != 0) lineNum++
        if (lineNum >= 5) lineNum = 4
        
        // 居中显示在320x192的屏幕上
        // 消息框宽度为240像素 (背景框)
        val boxWidth = 240
        val boxX = (Global.SCREEN_WIDTH - boxWidth) / 2  // (320 - 240) / 2 = 40
        
        // 垂直居中，考虑行数
        val boxHeight = lineNum * 16 + 20  // 每行16像素高，加上边框
        val boxY = (Global.SCREEN_HEIGHT - boxHeight) / 2  // 垂直居中
        val textY = boxY + 2  // 文字相对于框的偏移
        
        canvas.drawBitmap(bmpInformationBg[lineNum], boxX, boxY - 2)
        // 文字区域宽度: 14个汉字 * 16像素 = 224像素，居中在240宽的框内
        val textWidth = 224
        val textX = boxX + (boxWidth - textWidth) / 2  // 文字在框内居中
        TextRender.drawText(canvas, msg, 0, Rect(textX, textY, textX + textWidth, textY + 16 * lineNum + 16))
    }

    fun drawSideFrame(canvas: Canvas) {
        canvas.drawBitmap(bmpSideFrame, 0, 0)
        canvas.drawBitmap(bmpSideFrame, Global.SCREEN_WIDTH - 8, 0)
    }

    init {
        drawFramePaint.color = Global.COLOR_BLACK
        drawFramePaint.style = Paint.Style.STROKE
    }

    fun getFrameBitmap(w: Int, h: Int): Bitmap {
        // 先创建Bitmap
        val bmp = Bitmap.createBitmap(w, h)
        val tmpC = Canvas(bmp)
        tmpC.drawColor(Global.COLOR_WHITE)
        tmpC.drawRect(1, 1, w - 2, h - 2, drawFramePaint)
        return bmp
    }

    fun drawTriangleCursor(canvas: Canvas, x: Int, y: Int) {
        canvas.drawBitmap(bmpTriangleCursor, x, y)
    }

    /**
     *
     * @return 画出的num宽度(像素)
     */
    fun drawSmallNum(canvas: Canvas, num: Int, x: Int, y: Int): Int {
        var tmpNum = num
        var tmpX = x
        if (tmpNum < 0) tmpNum = -tmpNum
        val digits = tmpNum.toString()

        for (i in digits.indices) {
            imgSmallNum.draw(canvas, digits[i] - '0' + 1, tmpX, y)
            tmpX += imgSmallNum.width + 1
        }

        return digits.length * imgSmallNum.width
    }

    fun getSmallSignedNumBitmap(num: Int): Bitmap {
        val digits = (if (num > 0) num else -num).toString()
        val sign = DatLib.getRes(DatLib.ResType.PIC, 2, if (num > 0) 6 else 7) as ResImage
        val bmp = Bitmap.createBitmap(
                sign.width + digits.length * imgSmallNum.width + 1 + digits.length,
                imgSmallNum.height)

        val c = Canvas(bmp)
        sign.draw(c, 1, 0, 0)

        var x = sign.width + 1
        for (i in digits.indices) {
            imgSmallNum.draw(c, digits[i] - '0' + 1, x, 0)
            x += imgSmallNum.width + 1
        }

        return bmp
    }

    init {
        sBlackPaint = Paint()
        sBlackPaint.color = Global.COLOR_BLACK
        sBlackPaint.style = Style.STROKE
        sBlackPaint.strokeWidth = 1
    }
}
