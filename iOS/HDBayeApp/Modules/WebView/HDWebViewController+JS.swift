//
//  HDWebViewController+JS.swift
//  HDBayeApp
//
//  Created by harrydeng on 2025/7/17.
//

import UIKit

// 通知名称常量
extension Notification.Name {
    static let colorFilterChanged = Notification.Name("ColorFilterChanged")
    static let setGameSpeedMultiple = Notification.Name("setGameSpeedMultiple")
    static let engineSelectionChanged = Notification.Name("EngineSelectionChanged")
    static let saveDataChanged = Notification.Name("SaveDataChanged")
    static let grantSpecialItems = Notification.Name("GrantSpecialItems")
    static let itemGrantResult = Notification.Name("ItemGrantResult")
    static let combatProbabilityChanged = Notification.Name("CombatProbabilityChanged")
    static let bayeOrientationChanged = Notification.Name("bayeOrientationChanged")
    static let bayeResolutionChanged = Notification.Name("bayeResolutionChanged")
    static let offlinePackageUpdated = Notification.Name("OfflinePackageUpdated")
    static let expMultiplierChanged = Notification.Name("ExpMultiplierChanged")
    static let goldMultiplierChanged = Notification.Name("GoldMultiplierChanged")
    static let itemMultiplierChanged = Notification.Name("ItemMultiplierChanged")
    static let agricultureMultiplierChanged = Notification.Name("AgricultureMultiplierChanged")
    static let commerceMultiplierChanged = Notification.Name("CommerceMultiplierChanged")
    static let addAllItemsTriple = Notification.Name("AddAllItemsTriple")
}

// 通知相关
extension HDWebViewController {
    func registerNotifiction() {
        // 添加通知监听
        NotificationCenter.default.addObserver(self, selector: #selector(handleColorFilterChanged), name: .colorFilterChanged, object: nil)
        
        // 添加游戏速度变化的通知监听
        NotificationCenter.default.addObserver(self, selector: #selector(handleGameSpeedChanged), name: .setGameSpeedMultiple, object: nil)
        
        // 添加引擎选择变化的通知监听
        NotificationCenter.default.addObserver(self, selector: #selector(handleEngineSelectionChanged), name: .engineSelectionChanged, object: nil)
        
        // 添加存档数据变化的通知监听
        NotificationCenter.default.addObserver(self, selector: #selector(handleSaveDataChanged), name: .saveDataChanged, object: nil)
        
        // 添加特殊物品购买的通知监听
        NotificationCenter.default.addObserver(self, selector: #selector(handleGrantSpecialItems), name: .grantSpecialItems, object: nil)
        
        // 添加遇敌概率变化的通知监听
        NotificationCenter.default.addObserver(self, selector: #selector(handleCombatProbabilityChanged), name: .combatProbabilityChanged, object: nil)
        
        // 添加离线包更新完成的通知监听
        NotificationCenter.default.addObserver(self, selector: #selector(handleOfflinePackageUpdated), name: .offlinePackageUpdated, object: nil)

        // 添加经验、金币和物品倍率变化的通知监听（仅FMJ应用）
        if HDAppsTool.hdAppName() == .hdFmjApp {
            NotificationCenter.default.addObserver(self, selector: #selector(handleExpMultiplierChanged), name: .expMultiplierChanged, object: nil)
            NotificationCenter.default.addObserver(self, selector: #selector(handleGoldMultiplierChanged), name: .goldMultiplierChanged, object: nil)
            NotificationCenter.default.addObserver(self, selector: #selector(handleItemMultiplierChanged), name: .itemMultiplierChanged, object: nil)
        }
        
        if HDAppsTool.hdAppName() == .hdBayeApp {

            // 添加霸业游戏方向切换的通知监听
            NotificationCenter.default.addObserver(self, selector: #selector(handleBayeOrientationChanged), name: .bayeOrientationChanged, object: nil)

            // 添加霸业游戏分辨率切换的通知监听
            NotificationCenter.default.addObserver(self, selector: #selector(handleBayeResolutionChanged), name: .bayeResolutionChanged, object: nil)

            // 添加霸业游戏农业和商业开发倍率变化的通知监听（仅霸业应用）
            NotificationCenter.default.addObserver(self, selector: #selector(handleAgricultureMultiplierChanged), name: .agricultureMultiplierChanged, object: nil)
            NotificationCenter.default.addObserver(self, selector: #selector(handleCommerceMultiplierChanged), name: .commerceMultiplierChanged, object: nil)

            // 添加全物品功能的通知监听
            NotificationCenter.default.addObserver(self, selector: #selector(handleAddAllItemsTriple), name: .addAllItemsTriple, object: nil)
        }
    }
    
    
    @objc private func handleColorFilterChanged(_ notification: Notification) {
        if let filter = notification.userInfo?["filter"] as? String {
            colorFilterMode = filter
            HDAppData.shared.selectedFilter = colorFilterMode
            needUpdateColorFilterMode = true
        }
    }
    
    @objc private func handleGameSpeedChanged(_ notification: Notification) {
        if let speed = notification.userInfo?["speed"] as? Float {
            gameSpeed = speed
            HDAppData.shared.gameSpeed = speed
            needUpdateGameSpeed = true
        }
    }
    
    
    @objc private func handleEngineSelectionChanged(_ notification: Notification) {
        if let useNew = notification.userInfo?["useNewEngine"] as? Bool {
            useNewEngine = useNew
            HDAppData.shared.useNewEngine = useNew
            updateEngineSelection()
        }
    }
    
    @objc private func handleSaveDataChanged(_ notification: Notification) {
        // 同步存档数据到H5的localStorage
        injectFmjCacheJSHooks()
    }
    
    @objc private func handleCombatProbabilityChanged(_ notification: Notification) {
        if let probability = notification.userInfo?["probability"] as? Int {
            combatProbability = probability
            HDAppData.shared.combatProbability = probability
            needUpdateCombatProbability = true
        }
    }
    
    @objc private func handleGrantSpecialItems(_ notification: Notification) {
        
    }
    
    @objc private func handleExpMultiplierChanged(_ notification: Notification) {
        guard let multiplier = notification.userInfo?["multiplier"] as? Int else { return }

        // 更新经验倍率的JS变量
        updateExpMultiplier(multiplier)
    }

    @objc private func handleGoldMultiplierChanged(_ notification: Notification) {
        guard let multiplier = notification.userInfo?["multiplier"] as? Int else { return }

        // 更新金币倍率的JS变量
        updateGoldMultiplier(multiplier)
    }

    @objc private func handleItemMultiplierChanged(_ notification: Notification) {
        guard let multiplier = notification.userInfo?["multiplier"] as? Int else { return }

        // 更新物品倍率的JS变量
        updateItemMultiplier(multiplier)
    }

    @objc private func handleAgricultureMultiplierChanged(_ notification: Notification) {
        guard let multiplier = notification.userInfo?["multiplier"] as? Int else { return }

        // 更新农业开发倍率的JS变量
        updateAgricultureMultiplier(multiplier)
    }

    @objc private func handleCommerceMultiplierChanged(_ notification: Notification) {
        guard let multiplier = notification.userInfo?["multiplier"] as? Int else { return }

        // 更新商业开发倍率的JS变量
        updateCommerceMultiplier(multiplier)
    }

    @objc private func handleAddAllItemsTriple(_ notification: Notification) {
        
    }

    @objc private func handleBayeOrientationChanged(_ notification: Notification) {
        guard let orientation = notification.userInfo?["orientation"] as? String else { return }

        // 调用JS API控制H5页面的显示方向
        updateBayeDisplayOrientation(orientation)

        // 强制更新iOS设备方向
        forceOrientationUpdate()
    }
    
    @objc private func handleBayeResolutionChanged(_ notification: Notification) {
        guard let resolution = notification.userInfo?["resolution"] as? String else { return }
        
        // 检查是否需要重新加载WebView
        let needsReload = notification.userInfo?["needsReload"] as? Bool ?? false
        
        if needsReload {
            // 需要重新加载WebView以确保分辨率变更完全生效
            reloadWebViewWithNewResolution(resolution)
        } else {
            // 仅更新分辨率设置
            updateBayeResolution(resolution)
        }
    }
    
    // 重新加载WebView并应用新分辨率
    private func reloadWebViewWithNewResolution(_ resolution: String) {
        // 先更新分辨率设置
        updateBayeResolution(resolution)
        
        // 延迟一小段时间后重新加载WebView
        DispatchQueue.main.asyncAfter(deadline: .now() + 0.3) { [weak self] in
            self?.alreadyInjectJS = false  // 重置JS注入标志
            self?.wkWebView?.reload()
            
            debugPrint("WebView已重新加载以应用新分辨率: \(resolution)")
        }
    }
}

// JavaScript执行工具
extension HDWebViewController {
    private func executeJavaScript(_ code: String, context: String? = nil, completion: ((Result<Any?, Error>) -> Void)? = nil) {
        wkWebView?.evaluateJavaScript(code) { result, error in
            if let error = error {
                if let context = context {
                    HDAppsTool.debugLog("\(context)失败: \(error)")
                }
                completion?(.failure(error))
            } else {
                if let context = context {
                    HDAppsTool.debugLog("\(context)成功")
                }
                completion?(.success(result))
            }
        }
    }
}

// JSAPI相关
extension HDWebViewController {
    // 页面加载完成，调用JSAPI
    func injectCommonJSHooks() {
        if alreadyInjectJS {
            return
        }
        
        // 通用功能 - 两个应用都需要
        
        // 更新滤镜
        updateColorFilterMode()
        
        // 对于霸业App，初始化时也应用原生主题色
        if HDAppsTool.hdAppName() == .hdBayeApp {
            applyNativeThemeForFilter()

            // 应用保存的游戏方向设置
            updateBayeDisplayOrientation(HDAppData.shared.bayeDisplayOrientation)

            // 应用保存的游戏分辨率设置
            updateBayeResolution(HDAppData.shared.bayeResolution)

            // 初始化三国霸业VIP功能倍率
            initializeBayeVIPMultipliers()
        }
        
        // 设置初始游戏速度
        updateGameSpeed()
        
        // FMJ特有功能
        if HDAppsTool.hdAppName() == .hdFmjApp {
            // 更新多倍经验
            winExpGlodMultiple()
            
            // 同步存档信息
            injectFmjCacheJSHooks()
            
            // 更新地图显示状态
            updateMapContainerVisibility()
            
            // 设置初始遇敌概率
            updateCombatProbability()
            
            // 获取游戏列表
            refreshLibInfo()
        }
        
        alreadyInjectJS = true
    }
    
    
    // 页面可见的时候，原生调用JSAPI
    func updateComonCapacity() {
        if needUpdateColorFilterMode {
            updateColorFilterMode()
            needUpdateColorFilterMode = false
        }
        
        if needUpdateGameSpeed {
            updateGameSpeed()
            needUpdateGameSpeed = false
        }
        
        if needUpdateCombatProbability {
            updateCombatProbability()
            needUpdateCombatProbability = false
        }
        
        winExpGlodMultiple()
    }
    
    private func injectFmjCacheJSHooks() {
        // 注入JavaScript代码
        // 缓存文件
        let keys = [
            "sav/fmjsave0",
            "sav/fmjsave1",
            "sav/fmjsave2",
            "sav/fmjsave3",
            "sav/fmjsave4",
        ]
        for key in keys {
            removeItem(key: key)
            
            let tKey = HDAppData.shared.choiceLib["key"] ?? "FMJ"
            // 如果是伏魔记原版，不添加tKey作为前缀，兼容老版本
            if tKey == "FMJ" {
                setLocalStorage(appKey: key, h5Key: key)
            }
            else {
                // App原生有缓存，则写入到cookie中
                var realKey = tKey + "_" + key
              
                // 伏魔记神女轮舞曲(木子02/03/04) 复用 伏魔记圆梦前奏曲(木子01) 存档
              if tKey == "FMJSNLWQ" || tKey == "FMJMVKXQ" || tKey == "FMJHMAHQ" {
                  realKey = "FMJYMQZQ" + "_" + key
                }
                setLocalStorage(appKey: realKey, h5Key: key)
            }
        }
    }
    
    
    private func updateColorFilterMode() {
        
    }
    
    private func updateGameSpeed() {
        
    }
    
    
    private func updateEngineSelection() {
        
    }
    
    private func getCurrentMapUsageLimit() -> Int {
        return 5 + (HDAppData.shared.mapFreeExtensionCount * 5)
    }
    
    private func updateMapContainerVisibility() {
        
    }
    
    private func updateCombatProbability() {
        
    }
    
    private func winExpGlodMultiple() {
        
    }
    
    private func updateExpMultiplier(_ multiplier: Int) {
        
    }

    private func updateGoldMultiplier(_ multiplier: Int) {
        
    }

    private func updateItemMultiplier(_ multiplier: Int) {
        
    }

    private func updateAgricultureMultiplier(_ multiplier: Int) {
        
    }

    private func updateCommerceMultiplier(_ multiplier: Int) {
        
    }

    private func initializeBayeVIPMultipliers() {
        
    }

    func disableTouchCallout() {
        let jsCode = """
            document.documentElement.style.webkitTouchCallout='none';
            document.body.style.webkitTouchCallout='none';
            """
        self.wkWebView?.evaluateJavaScript(jsCode, completionHandler: nil)
    }
}


extension HDWebViewController {
    private func removeItem(key: String) {
        let script = "localStorage.removeItem('\(key)');"
        executeJavaScript(script, context: HDConstants.JSContext.removeLocalStorage)
    }
    
    private func setLocalStorage(appKey: String, h5Key: String) {
        if let value = UserDefaults.standard.string(forKey: appKey), value.count > 0 {
            setLocalStorage(key: h5Key, value: value)
        }
    }
    
    // 设置localStorage的值，解决在Macos上，也是有iPad的游戏体验
    func setLocalStorage(key: String, value: String) {
        let script = "localStorage.setItem('\(key)', '\(value)');"
        executeJavaScript(script, context: HDConstants.JSContext.setLocalStorage)
    }
    
    // 应用与滤镜对应的原生主题色
    private func applyNativeThemeForFilter() {
        DispatchQueue.main.async { [weak self] in
            guard let self = self else { return }
            
            // 使用集中管理的颜色值
            let colors = HDAppColor.themeColors(for: self.colorFilterMode)
            
            // 应用到视图和WebView背景
            self.view.backgroundColor = colors.barTint
            self.wkWebView?.backgroundColor = colors.barTint
            self.wkWebView?.scrollView.backgroundColor = colors.barTint
            self.wkWebView?.scrollView.isOpaque = false
            
            // 应用到导航栏
            if let navigationBar = self.navigationController?.navigationBar {
                navigationBar.tintColor = colors.tint
                navigationBar.barTintColor = colors.barTint
                navigationBar.titleTextAttributes = [
                    NSAttributedString.Key.foregroundColor: colors.titleText
                ]
                
                // 去掉导航栏下面的黑线
                navigationBar.shadowImage = UIImage()
                navigationBar.setBackgroundImage(UIImage(), for: .default)
                
                // iOS 13+ 使用 standardAppearance
                if #available(iOS 13.0, *) {
                    let appearance = UINavigationBarAppearance()
                    appearance.configureWithOpaqueBackground()
                    appearance.backgroundColor = colors.barTint
                    appearance.titleTextAttributes = [.foregroundColor: colors.titleText]
                    appearance.largeTitleTextAttributes = [.foregroundColor: colors.titleText]
                    appearance.shadowColor = .clear  // 去掉阴影线
                    appearance.shadowImage = UIImage()  // 清除阴影图片
                    
                    navigationBar.standardAppearance = appearance
                    navigationBar.scrollEdgeAppearance = appearance
                    navigationBar.compactAppearance = appearance
                }
            }
        }
    }
    
    // 控制霸业游戏H5页面的显示方向
    func updateBayeDisplayOrientation(_ orientation: String) {
        // 调用离线包中统一的方向设置API
        let jsCode = """
            if (typeof window.setBayeOrientation === 'function') {
                window.setBayeOrientation('\(orientation)');
            } else {
                console.warn('setBayeOrientation API 未找到，可能页面还未完全加载');
                // 保存设置，待页面加载完成后应用
                localStorage.setItem('bayeDisplayOrientation', '\(orientation)');
            }
            """
        
        executeJavaScript(jsCode, context: "设置霸业游戏显示方向")
    }
    
    // 控制霸业游戏H5页面的分辨率
    func updateBayeResolution(_ resolution: String) {
        // 调用离线包中统一的分辨率设置API
        let jsCode = HDBayeHooks.bayeResolutionJS(resolution)
        executeJavaScript(jsCode, context: "设置霸业游戏分辨率")
    }

    // 强制更新设备方向
    private func forceOrientationUpdate() {
        DispatchQueue.main.async {
            // 获取当前窗口场景
            if #available(iOS 16.0, *) {
                // iOS 16+ 使用新的 API
                guard let windowScene = UIApplication.shared.connectedScenes.first as? UIWindowScene else { return }

                let orientationSetting = HDAppData.shared.bayeDisplayOrientation
                let geometryPreferences: UIWindowScene.GeometryPreferences

                switch orientationSetting {
                case "landscape":
                    geometryPreferences = UIWindowScene.GeometryPreferences.iOS(interfaceOrientations: .landscape)
                case "portrait":
                    geometryPreferences = UIWindowScene.GeometryPreferences.iOS(interfaceOrientations: .portrait)
                default: // "auto"
                    geometryPreferences = UIWindowScene.GeometryPreferences.iOS(interfaceOrientations: .all)
                }

                windowScene.requestGeometryUpdate(geometryPreferences)
            } else {
                // iOS 15 及以下使用旧的方法
                UIDevice.current.setValue(UIInterfaceOrientation.unknown.rawValue, forKey: "orientation")

                let orientationSetting = HDAppData.shared.bayeDisplayOrientation
                var targetOrientation: UIInterfaceOrientation

                switch orientationSetting {
                case "landscape":
                    targetOrientation = .landscapeLeft
                case "portrait":
                    targetOrientation = .portrait
                default: // "auto"
                    // 对于自动模式，不强制特定方向
                    return
                }

                UIDevice.current.setValue(targetOrientation.rawValue, forKey: "orientation")
            }
        }
    }
    
    // MARK: - 离线包更新处理
    
    @objc private func handleOfflinePackageUpdated(_ notification: Notification) {
        HDAppsTool.debugLog("收到离线包更新完成通知，开始重新加载WebView")
        
        DispatchQueue.main.async { [weak self] in
            self?.reloadOfflinePackage()
        }
    }
}
