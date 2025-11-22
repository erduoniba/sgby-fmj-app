//
//  HDFeedbackService.swift
//  HDBayeApp
//
//  Created by harrydeng on 2025/8/1.
//

import Foundation
import UIKit

class HDFeedbackService {
    static let shared = HDFeedbackService()
    
    static let localHost = "192.168.5.58"
    
    private init() {}
    
    // MARK: - Environment Configuration
    
    /// 获取API基础URL（根据开发/生产环境）
    var apiBaseURL: String {
        return "http://www.xxxxx.com/"
    }
    
    /// 获取当前App版本信息
    private var appVersion: String {
        return Bundle.main.infoDictionary?["CFBundleShortVersionString"] as? String ?? "1.0.0"
    }
    
    /// 获取当前离线包版本信息
    private var offlinePackageVersion: String {
        return HDAppsTool.offlineZipName()
    }
    
    /// 获取当前App名称
    private var appName: String {
        let hdAppName = HDAppsTool.hdAppName()
        switch hdAppName {
        case .hdBayeApp:
            return "三国霸业"
        case .hdFmjApp:
            return "伏魔记"
        default:
            return "未知应用"
        }
    }
    
    // MARK: - Public Methods
    
    /// 提交用户反馈
    /// - Parameters:
    ///   - content: 反馈内容
    ///   - contact: 联系方式（可选）
    ///   - saveData: 游戏存档数据（可选）
    ///   - completion: 完成回调，返回成功状态和消息
    func submitFeedback(content: String, contact: String, saveData: SaveData?, completion: @escaping (Bool, String?) -> Void) {
        // 1. 准备请求参数
        var parameters: [String: Any] = [
            "content": content,
            "contactInfo": contact,
            "appVersion": appVersion,
            "offlinePackageVersion": offlinePackageVersion,
            "deviceInfo": [
                "platform": "iOS",
                "model": UIDevice.current.model,
                "systemVersion": UIDevice.current.systemVersion,
                "appName": appName
            ],
            "feedbackType": "user_feedback",
            "timestamp": Int(Date().timeIntervalSince1970 * 1000) // 毫秒时间戳
        ]
        
        // 2. 处理存档数据信息
        if let saveData = saveData {
            var saveDataInfo: [String: Any] = [
                "hasData": true,
                "saveKey": saveData.key,
                "saveTitle": saveData.displayTitle,
                "gameName": saveData.gameName,
                "gameKey": saveData.gameKey,
                "dataSize": saveData.content.count,
                "saveContent": saveData.content // 完整的存档内容
            ]
            
            // 添加创建时间信息
            if let createdDate = saveData.createdDate {
                saveDataInfo["createdDate"] = ISO8601DateFormatter().string(from: createdDate)
            }
            
            parameters["saveDataInfo"] = saveDataInfo
        } else {
            parameters["saveDataInfo"] = [
                "hasData": false,
                "saveKey": "",
                "saveTitle": "",
                "gameName": "",
                "gameKey": "",
                "dataSize": 0,
                "saveContent": "",
                "createdDate": ""
            ]
        }
        
        // 3. 创建网络请求
        guard let url = URL(string: "\(apiBaseURL)/feedback") else {
            completion(false, "API地址配置错误")
            return
        }
        
        var request = URLRequest(url: url)
        request.httpMethod = "POST"
        request.setValue("application/json", forHTTPHeaderField: "Content-Type")
        request.timeoutInterval = 30.0
        
        // 4. 序列化请求体
        do {
            request.httpBody = try JSONSerialization.data(withJSONObject: parameters, options: .prettyPrinted)
        } catch {
            completion(false, "数据准备失败: \(error.localizedDescription)")
            return
        }
        
        // 5. 发送请求
        URLSession.shared.dataTask(with: request) { [weak self] data, response, error in
            DispatchQueue.main.async {
                self?.handleFeedbackResponse(data: data, response: response, error: error, completion: completion)
            }
        }.resume()
    }
    
    // MARK: - Private Methods
    
    /// 处理反馈提交响应
    private func handleFeedbackResponse(data: Data?, response: URLResponse?, error: Error?, completion: @escaping (Bool, String?) -> Void) {
        // 处理网络错误
        if let error = error {
            let errorMessage = "网络连接失败: \(error.localizedDescription)"
            HDAppsTool.debugLog("Feedback submission failed: \(errorMessage)")
            completion(false, errorMessage)
            return
        }
        
        // 检查HTTP响应状态
        guard let httpResponse = response as? HTTPURLResponse else {
            completion(false, "服务器响应格式错误")
            return
        }
        
        HDAppsTool.debugLog("Feedback API response status: \(httpResponse.statusCode)")
        
        // 处理不同的HTTP状态码
        switch httpResponse.statusCode {
        case 200...299:
            // 成功响应，解析响应数据
            if let data = data {
                do {
                    let json = try JSONSerialization.jsonObject(with: data) as? [String: Any]
                    let success = json?["success"] as? Bool ?? false
                    let message = json?["message"] as? String
                    let feedbackId = json?["feedbackId"] as? String
                    
                    if success {
                        let successMessage = "反馈提交成功！"
                        HDAppsTool.debugLog("Feedback submitted successfully with ID: \(feedbackId ?? "unknown")")
                        completion(true, successMessage)
                    } else {
                        let failureMessage = message ?? "提交失败，请稍后重试"
                        HDAppsTool.debugLog("Feedback submission failed: \(failureMessage)")
                        completion(false, failureMessage)
                    }
                } catch {
                    HDAppsTool.debugLog("Failed to parse response JSON: \(error)")
                    completion(false, "服务器响应解析失败")
                }
            } else {
                completion(true, "反馈提交成功！")
            }
            
        case 400:
            completion(false, "请求参数错误，请检查输入内容")
            
        case 500...599:
            completion(false, "服务器内部错误，请稍后重试")
            
        default:
            completion(false, "服务器错误 (状态码: \(httpResponse.statusCode))")
        }
    }
    
    /// 检查服务器连接状态
    /// - Parameter completion: 完成回调，返回连接状态
    func checkServerConnection(completion: @escaping (Bool, String?) -> Void) {
        guard let url = URL(string: "\(apiBaseURL.replacingOccurrences(of: "/api", with: ""))/health") else {
            completion(false, "健康检查地址配置错误")
            return
        }
        
        var request = URLRequest(url: url)
        request.httpMethod = "GET"
        request.timeoutInterval = 10.0
        
        URLSession.shared.dataTask(with: request) { data, response, error in
            DispatchQueue.main.async {
                if let error = error {
                    completion(false, "服务器连接失败: \(error.localizedDescription)")
                    return
                }
                
                if let httpResponse = response as? HTTPURLResponse,
                   httpResponse.statusCode == 200 {
                    completion(true, "服务器连接正常")
                } else {
                    completion(false, "服务器状态异常")
                }
            }
        }.resume()
    }
}
