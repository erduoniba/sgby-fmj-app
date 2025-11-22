
//  HDOfflinePackageUpdateManager.swift
//  HDBayeApp
//
//  Created by harrydeng on 2025/8/6.
//

import Foundation
import SSZipArchive

class HDOfflinePackageUpdateManager {
    static let shared = HDOfflinePackageUpdateManager()
    
    // 服务器配置
    private let lastCheckTimeKey = "lastOfflinePackageCheckTime"
    private let checkInterval: TimeInterval = 10 // 10秒
    
    private init() {}
    
    // MARK: - Public Methods
    
    /// 应用启动时清理旧的ZIP文件
    func cleanupOldZipFilesOnStartup() {
        let fileManager = FileManager.default
        guard let documentsDir = fileManager.urls(for: .documentDirectory, in: .userDomainMask).first else {
            HDAppsTool.debugLog("❌ 获取Documents目录失败")
            return
        }
        
        // 获取当前版本号
        let defaults = UserDefaults.standard
        guard let currentVersion = defaults.string(forKey: "offlinePackageVersion") else {
            HDAppsTool.debugLog("无当前版本记录，跳过清理")
            return
        }
        
        HDAppsTool.debugLog("启动时清理ZIP文件，当前版本: \(currentVersion)")
        cleanupOldZipFiles(currentVersion: currentVersion, documentsDir: documentsDir)
    }
    
    /// 检查是否需要检查服务器更新
    func shouldCheckForUpdates() -> Bool {
        let defaults = UserDefaults.standard
        let lastCheckTime = defaults.double(forKey: lastCheckTimeKey)
        let currentTime = Date().timeIntervalSince1970
        
        return (currentTime - lastCheckTime) > checkInterval
    }
    
    /// 从服务器检查离线包更新
    private func checkServerUpdate(completion: @escaping (Result<[String: Any]?, Error>) -> Void) {
        guard shouldCheckForUpdates() else {
            HDAppsTool.debugLog("距离上次检查时间太短，跳过更新检查")
            completion(.success(nil))
            return
        }
        
        // 统一管理接口域名
        let apiBaseURL = HDFeedbackService.shared.apiBaseURL
        
        let defaults = UserDefaults.standard
        let gameType = HDAppsTool.hdAppName() == .hdBayeApp ? "baye" : "fmj"
        let currentVersion = defaults.string(forKey: "offlinePackageVersion") ?? ""
        let realVersion = currentVersion.components(separatedBy: "-").last ?? ""
        
        let urlString = "\(apiBaseURL)/offline-packages/check/\(gameType)?currentVersion=\(realVersion)"
        
        guard let url = URL(string: urlString) else {
            completion(.failure(NSError(domain: "HDOfflinePackageUpdateManager", code: -1, userInfo: [NSLocalizedDescriptionKey: "无效的URL"])))
            return
        }
        
        HDAppsTool.debugLog("检查服务器更新: \(urlString)")
        
        var request = URLRequest(url: url)
        request.setValue("application/json", forHTTPHeaderField: "Accept")
        request.timeoutInterval = 10.0
        
        let task = URLSession.shared.dataTask(with: request) { [weak self] data, response, error in
            DispatchQueue.main.async {
                if let error = error {
                    HDAppsTool.debugLog("检查服务器更新失败: \(error.localizedDescription)")
                    completion(.failure(error))
                    return
                }
                
                guard let data = data else {
                    completion(.failure(NSError(domain: "HDOfflinePackageUpdateManager", code: -1, userInfo: [NSLocalizedDescriptionKey: "无响应数据"])))
                    return
                }
                
                do {
                    if let json = try JSONSerialization.jsonObject(with: data, options: []) as? [String: Any] {
                        if let success = json["success"] as? Bool, success {
                            // 更新最后检查时间
                            defaults.set(Date().timeIntervalSince1970, forKey: self?.lastCheckTimeKey ?? "")
                            
                            if let hasUpdate = json["hasUpdate"] as? Bool, hasUpdate,
                               let packageInfo = json["packageInfo"] as? [String: Any] {
                                HDAppsTool.debugLog("发现新版本: \(packageInfo["version"] ?? "")")
                                // 添加服务器返回的下载URL到packageInfo中
                                var updatedPackageInfo = packageInfo
                                if let downloadUrl = json["downloadUrl"] as? String {
                                    updatedPackageInfo["downloadUrl"] = downloadUrl
                                }
                                completion(.success(updatedPackageInfo))
                            } else {
                                HDAppsTool.debugLog("当前已是最新版本")
                                completion(.success(nil))
                            }
                        } else {
                            let message = json["message"] as? String ?? "服务器返回错误"
                            HDAppsTool.debugLog("服务器返回错误: \(message)")
                            completion(.failure(NSError(domain: "HDOfflinePackageUpdateManager", code: -1, userInfo: [NSLocalizedDescriptionKey: message])))
                        }
                    } else {
                        completion(.failure(NSError(domain: "HDOfflinePackageUpdateManager", code: -1, userInfo: [NSLocalizedDescriptionKey: "无法解析JSON响应"])))
                    }
                } catch {
                    completion(.failure(error))
                }
            }
        }
        
        task.resume()
    }
    
    /// 从服务器下载并更新离线包
    private func downloadAndUpdateFromServer(packageInfo: [String: Any],
                                   onProgress: ((Double) -> Void)? = nil,
                                   completion: @escaping (Bool) -> Void) {
        guard let version = packageInfo["version"] as? String,
              let downloadUrlString = packageInfo["downloadUrl"] as? String else {
            HDAppsTool.debugLog("下载信息不完整: 缺少版本号或下载链接")
            completion(false)
            return
        }
        
        guard let downloadUrl = URL(string: downloadUrlString) else {
            HDAppsTool.debugLog("下载链接格式错误: \(downloadUrlString)")
            completion(false)
            return
        }
        
        HDAppsTool.debugLog("开始下载离线包: \(downloadUrlString), 版本: \(version)")
        
        let task = URLSession.shared.downloadTask(with: downloadUrl) { [weak self] tempURL, response, error in
            DispatchQueue.main.async {
                if let error = error {
                    HDAppsTool.debugLog("❌ 下载失败: \(error.localizedDescription)")
                    completion(false)
                    return
                }
                
                guard let tempURL = tempURL else {
                    HDAppsTool.debugLog("❌ 下载失败: 无临时文件")
                    completion(false)
                    return
                }
                
                HDAppsTool.debugLog("✅ HTTP下载成功，临时文件路径: \(tempURL.path)")
                
                // 验证临时文件
                let fileManager = FileManager.default
                if fileManager.fileExists(atPath: tempURL.path) {
                    do {
                        let attributes = try fileManager.attributesOfItem(atPath: tempURL.path)
                        let fileSize = attributes[.size] as? Int64 ?? 0
                        HDAppsTool.debugLog("下载文件大小: \(fileSize) bytes")
                    } catch {
                        HDAppsTool.debugLog("❌ 获取下载文件属性失败: \(error.localizedDescription)")
                    }
                } else {
                    HDAppsTool.debugLog("❌ 下载的临时文件不存在")
                    completion(false)
                    return
                }
                
                // 验证文件哈希（如果服务器提供）
                if let httpResponse = response as? HTTPURLResponse,
                   let expectedHash = httpResponse.allHeaderFields["X-Package-Hash"] as? String {
                    HDAppsTool.debugLog("服务器提供的文件哈希: \(expectedHash)")
                    // TODO: 在这里可以添加文件哈希验证逻辑
                }
                
                HDAppsTool.debugLog("开始保存和解压下载的离线包...")
                
                // 首先将ZIP文件保存到Documents目录
                guard let documentsDir = fileManager.urls(for: .documentDirectory, in: .userDomainMask).first else {
                    HDAppsTool.debugLog("❌ 获取Documents目录失败")
                    completion(false)
                    return
                }
                
                let gameType = (HDAppsTool.hdAppName() == .hdBayeApp) ? "baye" : "fmj"
                let zipFileName = "\(gameType)-offline-\(version).zip"
                let savedZipURL = documentsDir.appendingPathComponent(zipFileName)
                
                do {
                    // 如果目标文件已存在，先删除
                    if fileManager.fileExists(atPath: savedZipURL.path) {
                        try fileManager.removeItem(at: savedZipURL)
                        HDAppsTool.debugLog("删除旧的ZIP文件: \(savedZipURL.path)")
                    }
                    
                    // 复制临时文件到Documents目录
                    try fileManager.copyItem(at: tempURL, to: savedZipURL)
                    HDAppsTool.debugLog("✅ ZIP文件已保存到: \(savedZipURL.path)")
                    
                    // 验证保存的文件
                    let attributes = try fileManager.attributesOfItem(atPath: savedZipURL.path)
                    let savedFileSize = attributes[.size] as? Int64 ?? 0
                    HDAppsTool.debugLog("保存的ZIP文件大小: \(savedFileSize) bytes")
                    
                    // 清理旧的ZIP文件，只保留当前版本
                    self?.cleanupOldZipFiles(currentVersion: version, documentsDir: documentsDir)
                    
                } catch {
                    HDAppsTool.debugLog("❌ 保存ZIP文件失败: \(error.localizedDescription)")
                    // 继续使用临时文件进行解压
                }
                
                // 列出Documents目录内容，确认ZIP文件是否保存成功
                self?.listDocumentsDirectory()
                
                // 解压下载的文件（使用原临时文件）
                let success = self?.extractDownloadedPackage(from: tempURL, version: version) ?? false
                
                if success {
                    HDAppsTool.debugLog("✅ 离线包更新成功: \(version)")
                } else {
                    HDAppsTool.debugLog("❌ 离线包解压失败")
                }
                
                completion(success)
            }
        }
        
        task.resume()
    }
    
    /// 检查离线包更新（需要用户确认）
    func checkAndPromptForUpdate(from viewController: UIViewController,
                                 onStatusChange: ((String) -> Void)? = nil,
                                 onProgress: ((Double) -> Void)? = nil,
                                 completion: @escaping (Bool) -> Void) {
        
        HDAppsTool.debugLog("🔍 开始iOS更新检查流程...")
        onStatusChange?("检查服务器更新...")
        
        checkServerUpdate { result in
            DispatchQueue.main.async {
                switch result {
                case .success(let updateInfo):
                    guard let updateInfo = updateInfo else {
                        HDAppsTool.debugLog("没有可用更新，当前已是最新版本")
                        onStatusChange?("当前已是最新版本")
                        completion(false)
                        return
                    }
                    
                    let newVersion = updateInfo["version"] as? String ?? "未知版本"
                    let changelog = updateInfo["changelog"] as? String ?? "暂无更新说明"
                    let downloadUrl = updateInfo["downloadUrl"] as? String ?? "未知URL"
                    
                    HDAppsTool.debugLog("🆕 发现新版本: \(newVersion)")
                    HDAppsTool.debugLog("📝 更新说明: \(changelog)")
                    HDAppsTool.debugLog("📥 下载URL: \(downloadUrl)")
                    
                    // 显示更新确认对话框
                    self.showUpdateConfirmationDialog(
                        from: viewController,
                        version: newVersion,
                        changelog: changelog,
                        updateInfo: updateInfo,
                        onStatusChange: onStatusChange,
                        onProgress: onProgress,
                        completion: completion
                    )
                    
                case .failure(let error):
                    DispatchQueue.main.async {
                        onStatusChange?("没有可用更新，当前已是最新版本")
                        HDAppsTool.debugLog("❌ 检查服务器更新失败: \(error.localizedDescription)")
                        completion(false)
                    }
                }
            }
        }
    }
    
    /// 显示更新确认对话框
    private func showUpdateConfirmationDialog(from viewController: UIViewController,
                                            version: String,
                                            changelog: String,
                                            updateInfo: [String: Any],
                                            onStatusChange: ((String) -> Void)?,
                                            onProgress: ((Double) -> Void)?,
                                            completion: @escaping (Bool) -> Void) {
        
        let alert = UIAlertController(
            title: "发现新版本",
            message: "版本: \(version)\n\(changelog)",
            preferredStyle: .alert
        )
        
        // 取消按钮
        let cancelAction = UIAlertAction(title: "暂不更新", style: .cancel) { _ in
            HDAppsTool.debugLog("用户取消了离线包更新")
            completion(false)
        }
        
        // 更新按钮
        let updateAction = UIAlertAction(title: "立即更新", style: .default) { _ in
            HDAppsTool.debugLog("用户确认更新离线包")
            onStatusChange?("用户确认，开始下载...")
            
            self.downloadAndUpdateFromServer(
                packageInfo: updateInfo,
                onProgress: onProgress
            ) { success in
                DispatchQueue.main.async {
                    if success {
                        onStatusChange?("更新完成")
                        HDAppsTool.debugLog("✅ 离线包更新成功，新版本: \(version)")
                        
                        // 验证更新后的状态
                        let currentVersion = UserDefaults.standard.string(forKey: "offlinePackageVersion") ?? "无"
                        let documentPath = NSSearchPathForDirectoriesInDomains(.documentDirectory, .userDomainMask, true)[0]
                        let offlineDirName = HDAppsTool.offlineDirName()
                        let offlineDirPath = (documentPath as NSString).appendingPathComponent(offlineDirName)
                        let dirExists = FileManager.default.fileExists(atPath: offlineDirPath)
                        
                        HDAppsTool.debugLog("更新后验证: 当前版本=\(currentVersion), 目录存在=\(dirExists), 路径=\(offlineDirPath)")
                    } else {
                        onStatusChange?("更新失败")
                        HDAppsTool.debugLog("❌ 离线包更新失败")
                    }
                    completion(success)
                }
            }
        }
        
        alert.addAction(cancelAction)
        alert.addAction(updateAction)
        
        viewController.present(alert, animated: true)
    }
    
    /// 检查并自动更新离线包（保持向后兼容）
    func checkAndAutoUpdate(onStatusChange: ((String) -> Void)? = nil,
                           onProgress: ((Double) -> Void)? = nil,
                           completion: @escaping (Bool) -> Void) {
        
        HDAppsTool.debugLog("🔍 开始iOS自动更新检查流程...")
        onStatusChange?("检查服务器更新...")
        
        checkServerUpdate { result in
            switch result {
            case .success(let updateInfo):
                guard let updateInfo = updateInfo else {
                    HDAppsTool.debugLog("没有可用更新，当前已是最新版本")
                    onStatusChange?("当前已是最新版本")
                    completion(false)
                    return
                }
                
                let newVersion = updateInfo["version"] as? String ?? "未知版本"
                let downloadUrl = updateInfo["downloadUrl"] as? String ?? "未知URL"
                HDAppsTool.debugLog("🆕 发现新版本: \(newVersion)")
                HDAppsTool.debugLog("📥 下载URL: \(downloadUrl)")
                
                onStatusChange?("发现新版本，开始下载...")
                
                HDAppsTool.debugLog("开始下载和安装更新包...")
                self.downloadAndUpdateFromServer(
                    packageInfo: updateInfo,
                    onProgress: onProgress
                ) { success in
                    if success {
                        onStatusChange?("更新完成")
                        HDAppsTool.debugLog("✅ 离线包自动更新成功，新版本: \(newVersion)")
                        
                        // 验证更新后的状态
                        let currentVersion = UserDefaults.standard.string(forKey: "offlinePackageVersion") ?? "无"
                        let documentPath = NSSearchPathForDirectoriesInDomains(.documentDirectory, .userDomainMask, true)[0]
                        let offlineDirName = HDAppsTool.offlineDirName()
                        let offlineDirPath = (documentPath as NSString).appendingPathComponent(offlineDirName)
                        let dirExists = FileManager.default.fileExists(atPath: offlineDirPath)
                        
                        HDAppsTool.debugLog("更新后验证: 当前版本=\(currentVersion), 目录存在=\(dirExists), 路径=\(offlineDirPath)")
                    } else {
                        onStatusChange?("更新失败")
                        HDAppsTool.debugLog("❌ 离线包自动更新失败")
                    }
                    completion(success)
                }
                
            case .failure(let error):
                onStatusChange?("更新失败: \(error.localizedDescription)")
                HDAppsTool.debugLog("❌ 自动更新过程失败: \(error.localizedDescription)")
                completion(false)
            }
        }
    }
    
    // MARK: - Private Methods
    
    /// 解压下载的离线包
    private func extractDownloadedPackage(from tempURL: URL, version: String) -> Bool {
        let fileManager = FileManager.default
        let defaults = UserDefaults.standard
        
        HDAppsTool.debugLog("开始解压离线包文件: \(tempURL.path)")
        
        // 验证临时文件
        guard fileManager.fileExists(atPath: tempURL.path) else {
            HDAppsTool.debugLog("❌ 临时ZIP文件不存在: \(tempURL.path)")
            return false
        }
        
        do {
            let fileAttributes = try fileManager.attributesOfItem(atPath: tempURL.path)
            let fileSize = fileAttributes[.size] as? Int64 ?? 0
            HDAppsTool.debugLog("临时ZIP文件大小: \(fileSize) bytes")
            
            if fileSize == 0 {
                HDAppsTool.debugLog("❌ 临时ZIP文件为空")
                return false
            }
        } catch {
            HDAppsTool.debugLog("❌ 无法获取临时文件属性: \(error.localizedDescription)")
            return false
        }
        
        // 获取目标目录 - 使用Documents目录
        let documentPath = NSSearchPathForDirectoriesInDomains(.documentDirectory, .userDomainMask, true)[0]
        let offlineDirName = HDAppsTool.offlineDirName()
        let offlineDirPath = (documentPath as NSString).appendingPathComponent(offlineDirName)
        
        HDAppsTool.debugLog("Documents目录: \(documentPath)")
        HDAppsTool.debugLog("目标解压路径: \(offlineDirPath)")
        
        do {
            // 清理旧版本
            if fileManager.fileExists(atPath: offlineDirPath) {
                // 先列出旧目录的内容
                let oldContents = try? fileManager.contentsOfDirectory(atPath: offlineDirPath)
                HDAppsTool.debugLog("清理旧版本目录，当前文件数: \(oldContents?.count ?? 0)")
                if let oldContents = oldContents, !oldContents.isEmpty {
                    HDAppsTool.debugLog("旧目录内容: \(Array(oldContents.prefix(3)))")
                }
                
                try fileManager.removeItem(atPath: offlineDirPath)
                HDAppsTool.debugLog("✅ 旧版本目录清理完成")
            } else {
                HDAppsTool.debugLog("目标目录不存在，无需清理")
            }
            
            // 确保父目录存在
            let parentDir = (offlineDirPath as NSString).deletingLastPathComponent
            if !fileManager.fileExists(atPath: parentDir) {
                try fileManager.createDirectory(atPath: parentDir, withIntermediateDirectories: true, attributes: nil)
                HDAppsTool.debugLog("创建父目录: \(parentDir)")
            }
            
            // 解压新版本
            let password = HDAppsTool.zipPassword()
            var success = false
            
            HDAppsTool.debugLog("开始解压，密码保护: \(!password.isEmpty)")
            HDAppsTool.debugLog("源文件: \(tempURL.path)")
            HDAppsTool.debugLog("目标路径: \(offlineDirPath)")
            
            // 验证ZIP文件是否为有效的ZIP格式
            do {
                let zipData = try Data(contentsOf: tempURL)
                let hexHeader = zipData.prefix(4).map { String(format: "%02X", $0) }.joined()
                HDAppsTool.debugLog("ZIP文件头部: \(hexHeader) (应该是504B0304或504B0506)")
                
                if !hexHeader.hasPrefix("504B") {
                    HDAppsTool.debugLog("❌ 文件不是有效的ZIP格式")
                    return false
                }
            } catch {
                HDAppsTool.debugLog("❌ 无法读取ZIP文件: \(error.localizedDescription)")
                return false
            }
            
            if password.isEmpty {
                success = SSZipArchive.unzipFile(atPath: tempURL.path, toDestination: offlineDirPath)
            } else {
                success = ((try? SSZipArchive.unzipFile(atPath: tempURL.path, toDestination: offlineDirPath, overwrite: true, password: password)) != nil)
            }
            
            HDAppsTool.debugLog("SSZipArchive解压结果: \(success)")
            
            if success {
                // 验证解压结果
                if fileManager.fileExists(atPath: offlineDirPath) {
                    let contents = try? fileManager.contentsOfDirectory(atPath: offlineDirPath)
                    HDAppsTool.debugLog("解压后目录文件数量: \(contents?.count ?? 0)")
                    
                    if let contents = contents, !contents.isEmpty {
                        HDAppsTool.debugLog("解压文件列表 (前5个): \(Array(contents.prefix(5)))")
                        
                        // 检查是否需要处理子目录结构
                        if contents.count == 1, 
                           let subDirName = contents.first,
                           subDirName.hasPrefix("baye-offline-") || subDirName.hasPrefix("fmj-offline-") {
                            let subDirPath = (offlineDirPath as NSString).appendingPathComponent(subDirName)
                            let indexInSubDir = (subDirPath as NSString).appendingPathComponent("index.html")
                            
                            if fileManager.fileExists(atPath: indexInSubDir) {
                                HDAppsTool.debugLog("发现子目录结构，正在移动文件到根目录")
                                
                                // 移动子目录中的所有内容到根目录
                                let subContents = try fileManager.contentsOfDirectory(atPath: subDirPath)
                                for item in subContents {
                                    let sourcePath = (subDirPath as NSString).appendingPathComponent(item)
                                    let targetPath = (offlineDirPath as NSString).appendingPathComponent(item)
                                    
                                    // 如果目标已存在，先删除
                                    if fileManager.fileExists(atPath: targetPath) {
                                        try fileManager.removeItem(atPath: targetPath)
                                    }
                                    
                                    try fileManager.moveItem(atPath: sourcePath, toPath: targetPath)
                                }
                                
                                // 删除空的子目录
                                try fileManager.removeItem(atPath: subDirPath)
                                HDAppsTool.debugLog("✅ 子目录内容已移动到根目录")
                            }
                        }
                    }
                    
                    // 验证关键文件
                    let indexPath = (offlineDirPath as NSString).appendingPathComponent("index.html")
                    let indexExists = fileManager.fileExists(atPath: indexPath)
                    HDAppsTool.debugLog("关键文件index.html存在: \(indexExists)")
                    
                    if !indexExists {
                        HDAppsTool.debugLog("❌ 关键文件缺失，可能解压不完整")
                        return false
                    }
                } else {
                    HDAppsTool.debugLog("❌ 解压后目标目录不存在")
                    return false
                }
                
                // 最终验证：再次检查解压后的目录状态
                let finalContents = try? fileManager.contentsOfDirectory(atPath: offlineDirPath)
                HDAppsTool.debugLog("🔍 最终验证 - 目录文件数: \(finalContents?.count ?? 0)")
                if let finalContents = finalContents, !finalContents.isEmpty {
                    HDAppsTool.debugLog("🔍 最终文件列表: \(Array(finalContents.prefix(5)))")
                    
                    // 检查关键文件的时间戳
                    let indexPath = (offlineDirPath as NSString).appendingPathComponent("index.html")
                    if let attributes = try? fileManager.attributesOfItem(atPath: indexPath),
                       let modificationDate = attributes[.modificationDate] as? Date {
                        HDAppsTool.debugLog("🔍 index.html修改时间: \(modificationDate)")
                    }
                }
                
                // 更新版本记录
                defaults.set(version, forKey: "offlinePackageVersion")
                HDAppsTool.debugLog("✅ 离线包解压完成: \(version), 路径: \(offlineDirPath)")
                return true
            } else {
                HDAppsTool.debugLog("❌ SSZipArchive解压失败")
                return false
            }
        } catch {
            HDAppsTool.debugLog("❌ 解压离线包时发生错误: \(error.localizedDescription)")
            return false
        }
    }
    
    // MARK: - Helper Methods
    
    /// 清理旧的ZIP文件，只保留当前版本
    private func cleanupOldZipFiles(currentVersion: String, documentsDir: URL) {
        let fileManager = FileManager.default
        let gameType = HDAppsTool.hdAppName() == .hdBayeApp ? "baye" : "fmj"
        let currentFileName = "\(gameType)-offline-\(currentVersion).zip"
        
        HDAppsTool.debugLog("🧹 开始清理旧ZIP文件，保留版本: \(currentVersion)")
        
        do {
            let contents = try fileManager.contentsOfDirectory(at: documentsDir, includingPropertiesForKeys: nil)
            let zipFiles = contents.filter { url in
                let fileName = url.lastPathComponent
                return fileName.hasPrefix("\(gameType)-offline-") && fileName.hasSuffix(".zip")
            }
            
            HDAppsTool.debugLog("找到 \(zipFiles.count) 个ZIP文件")
            
            for zipFile in zipFiles {
                let fileName = zipFile.lastPathComponent
                if fileName != currentFileName {
                    do {
                        try fileManager.removeItem(at: zipFile)
                        HDAppsTool.debugLog("🗑️ 已删除旧ZIP文件: \(fileName)")
                    } catch {
                        HDAppsTool.debugLog("❌ 删除旧ZIP文件失败: \(fileName), 错误: \(error.localizedDescription)")
                    }
                } else {
                    HDAppsTool.debugLog("✅ 保留当前ZIP文件: \(fileName)")
                }
            }
            
        } catch {
            HDAppsTool.debugLog("❌ 读取Documents目录失败: \(error.localizedDescription)")
        }
    }
    
    /// 列出Documents目录中的所有文件
    private func listDocumentsDirectory() {
        guard let documentsDir = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first else {
            HDAppsTool.debugLog("❌ 获取Documents目录失败")
            return
        }
        
        HDAppsTool.debugLog("📁 Documents目录路径: \(documentsDir.path)")
        
        do {
            let contents = try FileManager.default.contentsOfDirectory(atPath: documentsDir.path)
            HDAppsTool.debugLog("📁 Documents目录文件数量: \(contents.count)")
            
            for (index, item) in contents.enumerated() {
                let itemPath = documentsDir.appendingPathComponent(item).path
                let attributes = try? FileManager.default.attributesOfItem(atPath: itemPath)
                let fileSize = attributes?[.size] as? Int64 ?? 0
                let isDirectory = attributes?[.type] as? FileAttributeType == .typeDirectory
                let typeSymbol = isDirectory ? "📁" : "📄"
                
                HDAppsTool.debugLog("  \(index + 1). \(typeSymbol) \(item) (\(fileSize) bytes)")
            }
            
            // 特别检查ZIP文件
            let zipFiles = contents.filter { $0.hasSuffix(".zip") }
            if !zipFiles.isEmpty {
                HDAppsTool.debugLog("🗜️ 找到ZIP文件: \(zipFiles)")
            } else {
                HDAppsTool.debugLog("⚠️ Documents目录中没有找到ZIP文件")
            }
            
        } catch {
            HDAppsTool.debugLog("❌ 读取Documents目录失败: \(error.localizedDescription)")
        }
    }
    
    
    // MARK: - 缓存管理功能
    
    /// 获取离线包缓存大小
    func getCacheSize() -> Int64 {
        let fileManager = FileManager.default
        let documentPath = NSSearchPathForDirectoriesInDomains(.documentDirectory, .userDomainMask, true)[0]
        let offlineDirName = HDAppsTool.offlineDirName()
        let offlineDirPath = (documentPath as NSString).appendingPathComponent(offlineDirName)
        
        guard fileManager.fileExists(atPath: offlineDirPath) else {
            return 0
        }
        
        var totalSize: Int64 = 0
        
        if let enumerator = fileManager.enumerator(atPath: offlineDirPath) {
            while let fileName = enumerator.nextObject() as? String {
                let filePath = (offlineDirPath as NSString).appendingPathComponent(fileName)
                
                do {
                    let attributes = try fileManager.attributesOfItem(atPath: filePath)
                    if let fileSize = attributes[.size] as? Int64 {
                        totalSize += fileSize
                    }
                } catch {
                    HDAppsTool.debugLog("获取文件大小失败: \(filePath), 错误: \(error.localizedDescription)")
                }
            }
        }
        
        HDAppsTool.debugLog("离线包缓存大小: \(String(format: "%.2f", Double(totalSize) / 1024 / 1024)) MB")
        return totalSize
    }
    
    /// 格式化文件大小显示
    func formatFileSize(_ bytes: Int64) -> String {
        if bytes < 1024 {
            return "\(bytes) B"
        } else if bytes < 1024 * 1024 {
            return String(format: "%.1f KB", Double(bytes) / 1024)
        } else if bytes < 1024 * 1024 * 1024 {
            return String(format: "%.1f MB", Double(bytes) / 1024 / 1024)
        } else {
            return String(format: "%.1f GB", Double(bytes) / 1024 / 1024 / 1024)
        }
    }
    
    /// 清理离线包缓存
    func clearCache() -> Bool {
        let fileManager = FileManager.default
        let defaults = UserDefaults.standard
        let documentPath = NSSearchPathForDirectoriesInDomains(.documentDirectory, .userDomainMask, true)[0]
        let offlineDirName = HDAppsTool.offlineDirName()
        let offlineDirPath = (documentPath as NSString).appendingPathComponent(offlineDirName)
        
        do {
            if fileManager.fileExists(atPath: offlineDirPath) {
                try fileManager.removeItem(atPath: offlineDirPath)
                HDAppsTool.debugLog("离线包缓存清理完成")
            }
            
            // 清理版本记录
            defaults.removeObject(forKey: "offlinePackageVersion")
            
            // 清理最后检查时间
            defaults.removeObject(forKey: lastCheckTimeKey)
            
            HDAppsTool.debugLog("离线包数据清理完成")
            return true
        } catch {
            HDAppsTool.debugLog("清理离线包缓存失败: \(error.localizedDescription)")
            return false
        }
    }
    
    /// 检查缓存完整性
    func validateCache() -> Bool {
        let fileManager = FileManager.default
        let documentPath = NSSearchPathForDirectoriesInDomains(.documentDirectory, .userDomainMask, true)[0]
        let offlineDirName = HDAppsTool.offlineDirName()
        let offlineDirPath = (documentPath as NSString).appendingPathComponent(offlineDirName)
        
        guard fileManager.fileExists(atPath: offlineDirPath) else {
            HDAppsTool.debugLog("离线包目录不存在")
            return false
        }
        
        // 检查关键文件是否存在
        let indexFilePath = (offlineDirPath as NSString).appendingPathComponent("index.html")
        if !fileManager.fileExists(atPath: indexFilePath) {
            HDAppsTool.debugLog("关键文件index.html不存在，缓存可能已损坏")
            return false
        }
        
        HDAppsTool.debugLog("离线包缓存验证通过")
        return true
    }
    
    /// 获取当前离线包版本
    func getCurrentVersion() -> String {
        let defaults = UserDefaults.standard
        return defaults.string(forKey: "offlinePackageVersion") ?? "未知版本"
    }
    
    /// 获取缓存信息
    func getCacheInfo() -> [String: Any] {
        let defaults = UserDefaults.standard
        let version = defaults.string(forKey: "offlinePackageVersion")
        let size = getCacheSize()
        let isValid = validateCache()
        
        let documentPath = NSSearchPathForDirectoriesInDomains(.documentDirectory, .userDomainMask, true)[0]
        let offlineDirName = HDAppsTool.offlineDirName()
        let path = (documentPath as NSString).appendingPathComponent(offlineDirName)
        
        return [
            "version": version ?? NSNull(),
            "size": size,
            "sizeFormatted": formatFileSize(size),
            "isValid": isValid,
            "path": path,
            "lastUpdated": ISO8601DateFormatter().string(from: Date())
        ]
    }
}
