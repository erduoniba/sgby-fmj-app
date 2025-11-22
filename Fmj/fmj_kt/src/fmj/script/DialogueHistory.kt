package fmj.script

import fmj.lib.DatLib

/**
 * 对话历史记录管理器
 * 用于保存和管理游戏中的所有对话内容
 */
object DialogueHistory {
    data class DialogueEntry(
        val speaker: String,      // 说话者名称（从picNum解析或"旁白"）
        val content: String,       // 对话内容
        val mapInfo: String? = null  // 地图信息（可选）
    )
    
    // 对话历史列表
    private val dialogueList = mutableListOf<DialogueEntry>()
    
    // 最大保存条数
    private const val MAX_HISTORY_SIZE = 500
    
    /**
     * 添加对话记录
     * @param content 对话内容
     * @param mapInfo 当前地图信息（可选）
     */
    fun addDialogue(content: String, mapInfo: String? = null) {
        val entry = DialogueEntry(
            speaker = "",
            content = content,
            mapInfo = mapInfo
        )
        
        dialogueList.add(entry)
        
        // 限制历史记录大小
        if (dialogueList.size > MAX_HISTORY_SIZE) {
            dialogueList.removeAt(0)
        }
    }
    
    /**
     * 获取所有对话历史
     * @return 对话历史列表
     */
    fun getAllDialogues(): List<DialogueEntry> {
        return dialogueList.toList()
    }
    
    /**
     * 获取最近的对话历史
     * @param count 获取数量
     * @return 最近的对话列表
     */
    fun getRecentDialogues(count: Int): List<DialogueEntry> {
        val size = dialogueList.size
        return if (size <= count) {
            dialogueList.toList()
        } else {
            dialogueList.subList(size - count, size).toList()
        }
    }
    
    /**
     * 搜索对话历史
     * @param keyword 搜索关键词
     * @return 包含关键词的对话列表
     */
    fun searchDialogues(keyword: String): List<DialogueEntry> {
        return dialogueList.filter { 
            it.content.contains(keyword, ignoreCase = true) ||
            it.speaker.contains(keyword, ignoreCase = true)
        }
    }
    
    /**
     * 清空所有对话历史
     */
    fun clearHistory() {
        dialogueList.clear()
    }
    
    /**
     * 导出对话历史为文本
     * @return 格式化的对话历史文本
     */
    fun exportAsText(): String {
        val sb = StringBuilder()
        sb.appendLine("=== 伏魔记对话历史 ===")
        sb.appendLine("总计: ${dialogueList.size} 条对话")
        sb.appendLine("=" .repeat(30))
        sb.appendLine()
        
        dialogueList.forEachIndexed { index, entry ->
            sb.appendLine("${index + 1}. ${entry.content}")
            if (entry.mapInfo != null) {
                sb.appendLine("   地点: ${entry.mapInfo}")
            }
            sb.appendLine("-" .repeat(20))
        }
        
        return sb.toString()
    }
    
    /**
     * 获取对话统计信息
     */
    fun getStatistics(): Map<String, Any> {
        val stats = mutableMapOf<String, Any>()
        stats["totalCount"] = dialogueList.size
        
        // 统计各角色的对话数量
        val speakerCounts = dialogueList.groupingBy { it.speaker }.eachCount()
        stats["speakerCounts"] = speakerCounts
        
        // 统计最活跃的角色
        val mostActiveSpeaker = speakerCounts.maxByOrNull { it.value }
        stats["mostActiveSpeaker"] = mostActiveSpeaker?.key ?: "无"
        stats["mostActiveSpeakerCount"] = mostActiveSpeaker?.value ?: 0
        
        return stats
    }
}