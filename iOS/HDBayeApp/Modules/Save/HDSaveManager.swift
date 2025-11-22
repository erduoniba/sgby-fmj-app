//
//  HDSaveManager.swift
//  HDBayeApp
//
//  Created by Assistant on 2025/8/19.
//

import Foundation

// MARK: - SaveData Model
struct SaveData {
    let key: String
    let title: String
    var content: String
    let createdDate: Date?
    let gameKey: String  // 游戏前缀key
    let gameName: String // 游戏名称
    
    var displayTitle: String {
        if title.isEmpty {
            return key
        }
        return title
    }
    
    var displayContent: String {
        if content.count > 100 {
            return String(content.prefix(100)) + "..."
        }
        return content
    }
    
    var formattedDate: String {
        guard let date = createdDate else { return "未知时间" }
        let formatter = DateFormatter()
        formatter.dateStyle = .medium
        formatter.timeStyle = .short
        formatter.locale = Locale(identifier: "zh_CN")
        return formatter.string(from: date)
    }
    
    init(key: String, title: String = "", content: String, createdDate: Date? = nil, gameKey: String = "", gameName: String = "") {
        self.key = key
        self.title = title
        self.content = content
        self.createdDate = createdDate
        self.gameKey = gameKey.isEmpty ? Self.extractGameKey(from: key) : gameKey
        self.gameName = gameName.isEmpty ? Self.getGameName(for: self.gameKey) : gameName
    }
    
    private static func extractGameKey(from key: String) -> String {
        // 从key中提取游戏前缀
        if key.hasPrefix("baye_") {
            return "baye"
        } else if key.hasPrefix("fmj_") {
            return "fmj"
        }
        return "unknown"
    }
    
    private static func getGameName(for gameKey: String) -> String {
        switch gameKey {
        case "baye":
            return "三国霸业"
        case "fmj":
            return "伏魔记"
        default:
            return "未知游戏"
        }
    }
}

// MARK: - HDSaveManager
class HDSaveManager {
    static let shared = HDSaveManager()
    
    private init() {}
    
    // MARK: - Public Methods
    
    /// 获取所有存档（从UserDefaults）
    func getAllSaves() -> [SaveData] {
        var saves: [SaveData] = []
        let allKeys = UserDefaults.standard.dictionaryRepresentation().keys
        
        for key in allKeys {
            // 检查是否是存档key（包含sav/或者_sav/或者baye//data//sango）
            if isSaveKey(key) {
                if let content = UserDefaults.standard.string(forKey: key), !content.isEmpty {
                    let title = extractTitle(from: content)
                    let saveData = SaveData(key: key, title: title, content: content, createdDate: Date())
                    saves.append(saveData)
                }
            }
        }
        
        // 按key排序
        return saves.sorted { $0.key < $1.key }
    }
    
    /// 获取指定的存档
    func getSaveData(for key: String) -> SaveData? {
        guard let content = UserDefaults.standard.string(forKey: key), !content.isEmpty else {
            return nil
        }
        
        let title = extractTitle(from: content)
        return SaveData(key: key, title: title, content: content, createdDate: Date())
    }
    
    /// 保存或更新存档到UserDefaults
    func saveSaveData(_ saveData: SaveData) -> Bool {
        UserDefaults.standard.set(saveData.content, forKey: saveData.key)
        return UserDefaults.standard.synchronize()
    }
    
    /// 更新存档（用于编辑功能）
    func updateSaveData(_ saveData: SaveData) -> Bool {
        // 先备份
        backupSaveData(saveData)
        
        // 更新存档
        UserDefaults.standard.set(saveData.content, forKey: saveData.key)
        return UserDefaults.standard.synchronize()
    }
    
    /// 删除存档
    func deleteSaveData(for key: String) -> Bool {
        UserDefaults.standard.removeObject(forKey: key)
        return UserDefaults.standard.synchronize()
    }
    
    /// 判断是否是存档key
    private func isSaveKey(_ key: String) -> Bool {
        // FMJ存档格式
        if key.contains("sav/fmjsave") {
            return true
        }
        // 三国霸业存档格式
        if key.contains("baye//data//sango") && (key.contains("sango0.sav") || key.contains("sango2.sav") || key.contains("sango4.sav")) {
            return true
        }
        return false
    }
    
    /// 验证存档内容格式
    func validateSaveContent(_ content: String) -> (isValid: Bool, error: String?) {
        // 基本验证
        guard !content.isEmpty else {
            return (false, "存档内容不能为空")
        }
        
        guard content.count > 10 else {
            return (false, "存档内容太短，可能已损坏")
        }
        
        // 可以添加更多验证逻辑
        // 例如：检查JSON格式、特定字段等
        
        return (true, nil)
    }
    
    /// 备份存档（在编辑前）
    @discardableResult
    func backupSaveData(_ saveData: SaveData) -> Bool {
        let backupKey = "\(saveData.key)_backup_\(Int(Date().timeIntervalSince1970))"
        UserDefaults.standard.set(saveData.content, forKey: backupKey)
        return UserDefaults.standard.synchronize()
    }
    
    /// 从WebView同步存档数据
    func syncFromWebView(key: String, content: String) -> Bool {
        UserDefaults.standard.set(content, forKey: key)
        return UserDefaults.standard.synchronize()
    }
    
    /// 从内容中提取标题
    private func extractTitle(from content: String) -> String {
        // 尝试从存档内容中提取有意义的标题
        // 这里可以根据游戏的存档格式进行解析
        let lines = content.components(separatedBy: "\n")
        if let firstNonEmptyLine = lines.first(where: { !$0.trimmingCharacters(in: .whitespacesAndNewlines).isEmpty }) {
            // 返回第一个非空行作为标题，限制长度
            let title = String(firstNonEmptyLine.prefix(50))
            return title.trimmingCharacters(in: .whitespacesAndNewlines)
        }
        return "未命名存档"
    }
}