//
//  HDVersionChecker.swift
//  HDBayeApp
//
//  Created by harrydeng on 2025/07/23.
//

import Foundation
import UIKit

// MARK: - App Store版本检查器
class HDVersionChecker {
    static let shared = HDVersionChecker()
    
    private init() {}
    
    // MARK: - 版本检查主方法
    func checkForUpdateIfNeeded() {
        let currentLaunchCount = HDAppData.shared.appLaunchCount
        let threshold = HDConstants.AppStoreVersionCheck.launchCountThreshold
        
        // 只有在达到启动次数阈值时才检查
        guard currentLaunchCount >= threshold else {
            HDAppsTool.debugLog("启动次数不足，当前: \(currentLaunchCount)，需要: \(threshold)")
            return
        }
        
        // 检查App Store版本
        checkAppStoreVersion { [weak self] result in
            DispatchQueue.main.async {
                switch result {
                case .success((let shouldUpdate, let newVersion)):
                    if shouldUpdate {
                        self?.showUpdateAlert(for: newVersion)
                    } else {
                        HDAppsTool.debugLog("当前版本已是最新或不需要强制更新")
                    }
                    // 重置启动计数
                    HDAppData.shared.resetAppLaunchCount()
                case .failure(let error):
                    HDAppsTool.debugLog("版本检查失败: \(error.localizedDescription)")
                }
            }
        }
    }
    
    // MARK: - App Store版本检查
    private func checkAppStoreVersion(completion: @escaping (Result<(shouldUpdate: Bool, newVersion: String), Error>) -> Void) {
        guard let url = URL(string: HDConstants.iTunesAPI.searchURL) else {
            completion(.failure(VersionCheckError.invalidURL))
            return
        }
        
        let task = URLSession.shared.dataTask(with: url) { data, response, error in
            if let error = error {
                completion(.failure(error))
                return
            }
            
            guard let data = data else {
                completion(.failure(VersionCheckError.noData))
                return
            }
            
            do {
                let result = try JSONDecoder().decode(iTunesSearchResult.self, from: data)
                guard let appInfo = result.results.first else {
                    completion(.failure(VersionCheckError.appNotFound))
                    return
                }
                
                let currentVersion = HDAppData.shared.getCurrentVersion()
                let appStoreVersion = appInfo.version
                
                HDAppsTool.debugLog("当前版本: \(currentVersion), App Store版本: \(appStoreVersion)")
                
                let shouldUpdate = self.shouldShowUpdateAlert(currentVersion: currentVersion, appStoreVersion: appStoreVersion)
                completion(.success((shouldUpdate, appStoreVersion)))
                
            } catch {
                completion(.failure(error))
            }
        }
        
        task.resume()
    }
    
    // MARK: - 版本比较逻辑
    private func shouldShowUpdateAlert(currentVersion: String, appStoreVersion: String) -> Bool {
        let currentComponents = parseVersion(currentVersion)
        let appStoreComponents = parseVersion(appStoreVersion)
        
        // 检查App Store版本是否更新
        guard isNewerVersion(appStore: appStoreComponents, current: currentComponents) else {
            return false
        }
        
        // 检查是否满足升级条件：y == 0 && z == 0 或者 z == 99
        let (x, y, z) = appStoreComponents
        var shouldUpdate = false
        // 大版本需要提示升级
        if x > currentComponents.x {
            shouldUpdate = true
        }
        // 中版本和小版本一样，并且是2的倍数，需要升级
        if y == z, y % 2 == 0 {
            shouldUpdate = true
        }
        
        HDAppsTool.debugLog("版本分析 - App Store: \(appStoreComponents), 当前: \(currentComponents), 需要更新: \(shouldUpdate)")
        
        return true
    }
    
    // MARK: - 版本解析
    private func parseVersion(_ version: String) -> (x: Int, y: Int, z: Int) {
        let components = version.split(separator: ".").compactMap { Int($0) }
        let x = components.count > 0 ? components[0] : 0
        let y = components.count > 1 ? components[1] : 0
        let z = components.count > 2 ? components[2] : 0
        return (x, y, z)
    }
    
    // MARK: - 版本比较
    private func isNewerVersion(appStore: (x: Int, y: Int, z: Int), current: (x: Int, y: Int, z: Int)) -> Bool {
        if appStore.x != current.x {
            return appStore.x > current.x
        }
        if appStore.y != current.y {
            return appStore.y > current.y
        }
        return appStore.z > current.z
    }
    
    // MARK: - 显示更新提示
    private func showUpdateAlert(for newVersion: String) {
        guard let topViewController = getTopViewController() else { return }
        
        let alert = UIAlertController(
            title: HDConstants.AppStoreVersionCheck.title,
            message: HDConstants.AppStoreVersionCheck.message(for: newVersion),
            preferredStyle: .alert
        )
        
        // 立即更新按钮
        let updateAction = UIAlertAction(
            title: HDConstants.AppStoreVersionCheck.confirmButtonTitle,
            style: .default
        ) { _ in
            self.openAppStore()
        }
        
        // 稍后提醒按钮
        let laterAction = UIAlertAction(
            title: HDConstants.AppStoreVersionCheck.cancelButtonTitle,
            style: .cancel
        ) { _ in
            // 用户选择稍后提醒，不做任何操作
            HDAppsTool.debugLog("用户选择稍后更新")
        }
        
        alert.addAction(updateAction)
        alert.addAction(laterAction)
        
        topViewController.present(alert, animated: true)
    }
    
    // MARK: - 打开App Store
    private func openAppStore() {
        let appStoreURL = HDAppsTool.appStoreUrl()
        HDAppsTool.openURL(appStoreURL)
    }
    
    // MARK: - 获取顶层视图控制器
    private func getTopViewController() -> UIViewController? {
        guard let windowScene = UIApplication.shared.connectedScenes.first as? UIWindowScene,
              let window = windowScene.windows.first(where: { $0.isKeyWindow }) else {
            return nil
        }
        
        var topController = window.rootViewController
        while let presentedController = topController?.presentedViewController {
            topController = presentedController
        }
        
        return topController
    }
}

// MARK: - 版本检查错误类型
enum VersionCheckError: LocalizedError {
    case invalidURL
    case noData
    case appNotFound
    
    var errorDescription: String? {
        switch self {
        case .invalidURL:
            return "无效的iTunes API URL"
        case .noData:
            return "未收到数据响应"
        case .appNotFound:
            return "在App Store中未找到应用"
        }
    }
}

// MARK: - iTunes Search API 响应模型
struct iTunesSearchResult: Codable {
    let results: [AppInfo]
}

struct AppInfo: Codable {
    let version: String
    let trackId: Int
    let bundleId: String
    
    private enum CodingKeys: String, CodingKey {
        case version
        case trackId
        case bundleId
    }
}
