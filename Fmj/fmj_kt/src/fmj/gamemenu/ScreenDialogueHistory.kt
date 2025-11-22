package fmj.gamemenu

import fmj.Global
import fmj.graphics.TextRender
import fmj.graphics.Util
import fmj.views.BaseScreen
import fmj.views.GameNode
import graphics.Canvas
import graphics.Paint
import graphics.Rect
import kotlin.math.min

/**
 * 对话历史查看界面
 * 支持滚动查看历史对话记录
 */
class ScreenDialogueHistory(override val parent: GameNode) : BaseScreen {
    
    // 界面尺寸 - 适配 320x192 屏幕
    private val frameWidth = 300
    private val frameHeight = 170
    private val frameX = (320 - frameWidth) / 2  // 居中在 320x192 屏幕上
    private val frameY = (192 - frameHeight) / 2
    private val bmpFrame = Util.getFrameBitmap(frameWidth, frameHeight)
    private val frameRect = Rect(frameX, frameY, frameX + frameWidth, frameY + frameHeight)
    
    // 对话历史数据 - 使用 lazy 初始化避免过早访问，倒序展示（最新的在前）
    private val dialogues by lazy { 
        try {
            fmj.script.DialogueHistory.getAllDialogues().reversed()
        } catch (e: Throwable) {
            println("Failed to load dialogue history: $e")
            emptyList<fmj.script.DialogueHistory.DialogueEntry>()
        }
    }
    
    // 显示参数
    private val lineHeight = 16  // 每行高度
    private val contentAreaHeight = frameHeight - 40  // 内容区域高度（去掉标题和底部提示）
    private val maxPixelsPerLine = frameWidth - 25  // 每行最大像素宽度（留出边距和滚动条，减少右侧空白）
    private var scrollOffset = 0  // 滚动偏移量（以行为单位）
    
    // 预处理的对话行列表（每个对话可能占多行）
    private data class DialogueLine(
        val dialogueIndex: Int,  // 原始对话索引
        val lineText: String,     // 该行的文本
        val isFirstLine: Boolean  // 是否是该对话的第一行
    )
    
    private val processedLines by lazy {
        val lines = mutableListOf<DialogueLine>()
        val totalDialogues = dialogues.size
        dialogues.forEachIndexed { index, dialogue ->
            // 倒序显示，所以序号从总数递减
            val displayNumber = totalDialogues - index
            // 第一行需要预留序号空间（像素）
            val numberPixels = if (displayNumber < 10) 16 else if (displayNumber < 100) 24 else 32
            val firstLineMaxPixels = maxPixelsPerLine - numberPixels
            val continuationMaxPixels = maxPixelsPerLine - 24  // 续行缩进24像素
            
            val wrappedLines = wrapTextByPixels(dialogue.content, firstLineMaxPixels, continuationMaxPixels)
            wrappedLines.forEachIndexed { lineIndex, line ->
                lines.add(DialogueLine(displayNumber - 1, line, lineIndex == 0))  // 使用displayNumber-1作为索引（0-based）
            }
        }
        lines
    }
    
    private val linesPerPage = contentAreaHeight / lineHeight
    private val maxScroll get() = kotlin.math.max(0, processedLines.size - linesPerPage)
    
    override val isPopup: Boolean
        get() = true
    
    override fun update(delta: Long) {
        // 暂时不需要更新逻辑
    }
    
    override fun draw(canvas: Canvas) {
        // 绘制框架
        canvas.drawBitmap(bmpFrame, frameRect.left, frameRect.top)
        
        // 绘制标题
        TextRender.drawText(canvas, "对话历史", frameRect.left + 8, frameRect.top + 8)
        TextRender.drawText(canvas, "(共${dialogues.size}条)", frameRect.left + 72, frameRect.top + 8)
        
        // 绘制分隔线
        val paint = Paint()
        paint.color = Global.COLOR_WHITE
        canvas.drawLine(
            frameRect.left + 4, frameRect.top + 24,
            frameRect.right - 4, frameRect.top + 24,
            paint
        )
        
        // 绘制对话内容
        drawDialogues(canvas)
        
        // 绘制滚动条（如果内容超过一页）
        if (processedLines.size > linesPerPage) {
            drawScrollBar(canvas)
        }
        
        // 绘制提示信息
        val tipY = frameRect.bottom - 12
        TextRender.drawText(canvas, "↑↓滚动 ESC返回", frameRect.left + 8, tipY)
    }
    
    /**
     * 计算字符串的像素宽度
     * 中文字符16像素，ASCII字符8像素
     */
    private fun getTextPixelWidth(text: String): Int {
        var width = 0
        for (char in text) {
            width += if (char.code > 127) 16 else 8
        }
        return width
    }
    
    /**
     * 基于像素宽度的文本换行处理
     */
    private fun wrapTextByPixels(text: String, firstLineMaxPixels: Int, continuationMaxPixels: Int): List<String> {
        val lines = mutableListOf<String>()
        var currentPos = 0
        var isFirstLine = true
        
        while (currentPos < text.length) {
            val maxPixels = if (isFirstLine) firstLineMaxPixels else continuationMaxPixels
            var lineEnd = currentPos
            var currentWidth = 0
            
            // 计算这一行能容纳多少字符
            while (lineEnd < text.length) {
                val char = text[lineEnd]
                val charWidth = if (char.code > 127) 16 else 8
                
                if (currentWidth + charWidth > maxPixels) {
                    break
                }
                
                currentWidth += charWidth
                lineEnd++
            }
            
            // 如果一个字符都放不下（极端情况），至少放一个字符
            if (lineEnd == currentPos && currentPos < text.length) {
                lineEnd = currentPos + 1
            }
            
            lines.add(text.substring(currentPos, lineEnd))
            currentPos = lineEnd
            isFirstLine = false
        }
        
        return if (lines.isEmpty()) listOf("") else lines
    }
    
    /**
     * 绘制对话内容
     */
    private fun drawDialogues(canvas: Canvas) {
        val startY = frameRect.top + 28
        
        if (processedLines.isEmpty()) {
            TextRender.drawText(canvas, "暂无对话记录", frameRect.left + (frameWidth - 80) / 2, startY + 40)
            return
        }
        
        // 计算显示范围
        val startIndex = scrollOffset
        val endIndex = min(startIndex + linesPerPage, processedLines.size)
        
        // 设置内容区域边界
        val contentLeft = frameRect.left + 8
        val contentRight = frameRect.right - 12  // 减少右侧空白，只留必要的滚动条空间
        val clipTop = frameRect.top + 25
        val clipBottom = frameRect.bottom - 15
        
        // 绘制每行
        for (i in startIndex until endIndex) {
            val line = processedLines[i]
            val y = startY + (i - startIndex) * lineHeight
            
            // 只绘制在可见区域内的行
            if (y >= clipTop && y <= clipBottom - 10) {
                try {
                    if (line.isFirstLine) {
                        // 第一行显示序号
                        val number = "${line.dialogueIndex + 1}."
                        val numberWidth = if (line.dialogueIndex < 9) 16 else if (line.dialogueIndex < 99) 24 else 32
                        
                        // 确保文本不超出右边界
                        if (contentLeft + numberWidth < contentRight) {
                            TextRender.drawText(canvas, number, contentLeft, y)
                            
                            // 绘制内容，确保不超出边界
                            val textX = contentLeft + numberWidth
                            if (textX < contentRight - 8) {
                                TextRender.drawText(canvas, line.lineText, textX, y)
                            }
                        }
                    } else {
                        // 续行不显示序号，缩进显示
                        val indentX = contentLeft + 24
                        if (indentX < contentRight - 8) {
                            TextRender.drawText(canvas, line.lineText, indentX, y)
                        }
                    }
                } catch (e: Throwable) {
                    // 如果绘制失败，显示错误信息或跳过
                    println("Failed to draw dialogue line: ${e.message}")
                }
            }
        }
    }
    
    /**
     * 绘制滚动条
     */
    private fun drawScrollBar(canvas: Canvas) {
        val barX = frameRect.right - 8
        val barTop = frameRect.top + 28
        val barBottom = frameRect.bottom - 20
        val barHeight = barBottom - barTop
        
        // 绘制滚动条背景
        val paint = Paint()
        paint.color = Global.COLOR_BLACK
        canvas.drawLine(barX, barTop, barX, barBottom, paint)
        
        // 计算滑块位置和大小
        if (maxScroll > 0) {
            val thumbHeight = kotlin.math.max(5, (barHeight * linesPerPage) / processedLines.size)
            val thumbY = barTop + ((barHeight - thumbHeight) * scrollOffset) / maxScroll
            
            // 绘制滑块
            paint.color = Global.COLOR_WHITE
            // 绘制更粗的滑块（使用多条线）
            canvas.drawLine(barX - 1, thumbY, barX - 1, thumbY + thumbHeight, paint)
            canvas.drawLine(barX, thumbY, barX, thumbY + thumbHeight, paint)
            canvas.drawLine(barX + 1, thumbY, barX + 1, thumbY + thumbHeight, paint)
        }
        
        // 显示对话条数信息（倒序，最新的编号最大）
        if (processedLines.isNotEmpty()) {
            // 找出当前可见范围内的最大和最小对话编号
            val visibleLines = processedLines.subList(
                scrollOffset, 
                min(scrollOffset + linesPerPage, processedLines.size)
            )
            val dialogueNumbers = visibleLines.filter { it.isFirstLine }.map { it.dialogueIndex + 1 }
            
            if (dialogueNumbers.isNotEmpty()) {
                val maxNum = dialogueNumbers.maxOrNull() ?: dialogues.size
                val minNum = dialogueNumbers.minOrNull() ?: 1
                val scrollInfo = "$maxNum-$minNum/${dialogues.size}"
                TextRender.drawText(canvas, scrollInfo, frameRect.right - 80, frameRect.top + 8)
            }
        }
    }
    
    
    override fun onKeyDown(key: Int) {
        when (key) {
            Global.KEY_UP -> {
                // 向上滚动
                if (scrollOffset > 0) {
                    scrollOffset--
                }
            }
            Global.KEY_DOWN -> {
                // 向下滚动
                if (scrollOffset < maxScroll) {
                    scrollOffset++
                }
            }
            Global.KEY_PAGEUP -> {
                // 快速向上翻页
                scrollOffset = kotlin.math.max(0, scrollOffset - linesPerPage)
            }
            Global.KEY_PAGEDOWN -> {
                // 快速向下翻页
                scrollOffset = kotlin.math.min(maxScroll, scrollOffset + linesPerPage)
            }
        }
    }
    
    override fun onKeyUp(key: Int) {
        when (key) {
            Global.KEY_CANCEL, Global.KEY_ENTER -> {
                // ESC键或Enter键返回
                popScreen()
            }
        }
    }
    
    companion object {
        private val paint = Paint()
    }
}