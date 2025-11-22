//
//  HDAppData.swift
//  HDBayeApp
//
//  Created by harrydeng on 2025/6/22.
//

import Foundation

class HDAppData {
    static let shared = HDAppData()
    
    // 当前游戏的攻略链接
    var currentGameHomeURL = ""
    var currentGameTitle = ""
    
    private let defaults = UserDefaults.standard
    
    // 键值定义
    private let watchAdsStatusKey = "watchAdsStatus"
    private let selectedFilterKey = "selectedFilter"
    private let gameSpeedKey = "gameSpeed"
    private let choiceLibKey = "choiceLib"
    private let libsListKey = "libsList"
    private let useNewEngineKey = "useNewEngine"
    private let showMapContainerKey = "showMapContainer"
    private let mapUsageCountKey = "mapUsageCount"
    private let mapFreeExtensionCountKey = "mapFreeExtensionCount"
    private let hasShownUpgradeAlertForVersionKey = "hasShownUpgradeAlertForVersion"
    private let appLaunchCountKey = "appLaunchCount"
    private let lastVersionCheckKey = "lastVersionCheck"
    private let combatProbabilityKey = "combatProbability"
    private let hasViewedEnemyMagicGuideKey = "hasViewedEnemyMagicGuide"
    private let bayeDisplayOrientationKey = "bayeDisplayOrientation"
    private let bayeResolutionKey = "bayeResolution"
    private let expMultiplierKey = "expMultiplier"  // 经验倍率
    private let goldMultiplierKey = "goldMultiplier"  // 金币倍率
    private let itemMultiplierKey = "itemMultiplier"  // 物品倍率
    private let agricultureMultiplierKey = "agricultureMultiplier"  // 三国霸业-农业开发倍率
    private let commerceMultiplierKey = "commerceMultiplier"  // 三国霸业-商业开发倍率

    // 默认值定义
    private let defaultWatchAdsStatus: Bool = true
    private let defaultGameSpeed: Float = 1.0
    private let defaultChoiceLib = ["key": "FMJ", "value" : "伏魔记"]
    private let defaultLibsList: [String: String] = [:]
    private let defaultUseNewEngine: Bool = true
    private let defaultShowMapContainer: Bool = true
    private let defaultMapUsageCount: Int = 0
    private let defaultMapFreeExtensionCount: Int = 0
    private let defaultHasShownUpgradeAlertForVersion: String = ""
    private let defaultAppLaunchCount: Int = 0
    private let defaultLastVersionCheck: String = ""
    private let defaultCombatProbability: Int = 20  // 默认20，实际概率为20%
    private let defaultHasViewedEnemyMagicGuide: Bool = false
    private let defaultBayeDisplayOrientation: String = "auto"  // 默认跟随系统
    // 0:词典分辨率、1:高清分辨率
    private let defaultBayeResolution: String = "1"
    private let defaultExpMultiplier: Int = 5   // 默认5倍经验
    private let defaultGoldMultiplier: Int = 5  // 默认5倍金币
    private let defaultItemMultiplier: Int = 5  // 默认5倍物品
    private let defaultAgricultureMultiplier: Int = 1  // 默认1倍农业开发
    private let defaultCommerceMultiplier: Int = 1  // 默认1倍商业开发

    static func isBayeOrigin() -> Bool {
        return HDAppData.shared.currentGameTitle == "三国霸业-词典原版"
    }
    
    
    var watchAdsStatus: Bool {
        get {
            return defaults.bool(forKey: watchAdsStatusKey)
        }
        set {
            defaults.set(newValue, forKey: watchAdsStatusKey)
        }
    }
    
    var selectedFilter: String {
        get {
            return defaults.string(forKey: selectedFilterKey) ?? ""
        }
        set {
            defaults.set(newValue, forKey: selectedFilterKey)
        }
    }
    
    var gameSpeed: Float {
        get {
            return defaults.float(forKey: gameSpeedKey)
        }
        set {
            defaults.set(newValue, forKey: gameSpeedKey)
        }
    }
    
    
    var choiceLib: [String: String] {
        get {
            return (defaults.dictionary(forKey: choiceLibKey) as? [String: String]) ?? defaultChoiceLib
        }
        set {
            defaults.set(newValue, forKey: choiceLibKey)
        }
    }
    
    var libsList: [String: String] {
        get {
            return (defaults.dictionary(forKey: libsListKey) as? [String: String]) ?? defaultLibsList
        }
        set {
            defaults.set(newValue, forKey: libsListKey)
        }
    }
    
    var useNewEngine: Bool {
        get {
            return defaults.bool(forKey: useNewEngineKey)
        }
        set {
            defaults.set(newValue, forKey: useNewEngineKey)
        }
    }
    
    var showMapContainer: Bool {
        get {
            return defaults.bool(forKey: showMapContainerKey)
        }
        set {
            defaults.set(newValue, forKey: showMapContainerKey)
        }
    }
    
    var mapUsageCount: Int {
        get {
            return defaults.integer(forKey: mapUsageCountKey)
        }
        set {
            defaults.set(newValue, forKey: mapUsageCountKey)
        }
    }
    
    var mapFreeExtensionCount: Int {
        get {
            return defaults.integer(forKey: mapFreeExtensionCountKey)
        }
        set {
            defaults.set(newValue, forKey: mapFreeExtensionCountKey)
        }
    }
    
    var hasShownUpgradeAlertForVersion: String {
        get {
            return defaults.string(forKey: hasShownUpgradeAlertForVersionKey) ?? defaultHasShownUpgradeAlertForVersion
        }
        set {
            defaults.set(newValue, forKey: hasShownUpgradeAlertForVersionKey)
        }
    }
    
    var appLaunchCount: Int {
        get {
            return defaults.integer(forKey: appLaunchCountKey)
        }
        set {
            defaults.set(newValue, forKey: appLaunchCountKey)
        }
    }
    
    var lastVersionCheck: String {
        get {
            return defaults.string(forKey: lastVersionCheckKey) ?? defaultLastVersionCheck
        }
        set {
            defaults.set(newValue, forKey: lastVersionCheckKey)
        }
    }
    
    var combatProbability: Int {
        get {
            return defaults.integer(forKey: combatProbabilityKey)
        }
        set {
            defaults.set(newValue, forKey: combatProbabilityKey)
        }
    }
    
    var hasViewedEnemyMagicGuide: Bool {
        get {
            return defaults.bool(forKey: hasViewedEnemyMagicGuideKey)
        }
        set {
            defaults.set(newValue, forKey: hasViewedEnemyMagicGuideKey)
        }
    }
    
    var bayeDisplayOrientation: String {
        get {
            return defaults.string(forKey: bayeDisplayOrientationKey) ?? defaultBayeDisplayOrientation
        }
        set {
            defaults.set(newValue, forKey: bayeDisplayOrientationKey)
        }
    }
    
    var bayeResolution: String {
        get {
            return defaults.string(forKey: bayeResolutionKey) ?? defaultBayeResolution
        }
        set {
            defaults.set(newValue, forKey: bayeResolutionKey)
        }
    }
    
    var expMultiplier: Int {
        get {
            return defaults.integer(forKey: expMultiplierKey)
        }
        set {
            defaults.set(newValue, forKey: expMultiplierKey)
        }
    }

    var goldMultiplier: Int {
        get {
            return defaults.integer(forKey: goldMultiplierKey)
        }
        set {
            defaults.set(newValue, forKey: goldMultiplierKey)
        }
    }

    var itemMultiplier: Int {
        get {
            return defaults.integer(forKey: itemMultiplierKey)
        }
        set {
            defaults.set(newValue, forKey: itemMultiplierKey)
        }
    }

    var agricultureMultiplier: Int {
        get {
            return defaults.integer(forKey: agricultureMultiplierKey)
        }
        set {
            defaults.set(newValue, forKey: agricultureMultiplierKey)
        }
    }

    var commerceMultiplier: Int {
        get {
            return defaults.integer(forKey: commerceMultiplierKey)
        }
        set {
            defaults.set(newValue, forKey: commerceMultiplierKey)
        }
    }

    private init() {
        // 设置默认值 - 使用更高效的 register(defaults:) 方法
        let defaultValues: [String: Any] = [
            watchAdsStatusKey: defaultWatchAdsStatus,
            gameSpeedKey: defaultGameSpeed,
            choiceLibKey: defaultChoiceLib,
            useNewEngineKey: defaultUseNewEngine,
            showMapContainerKey: defaultShowMapContainer,
            mapUsageCountKey: defaultMapUsageCount,
            mapFreeExtensionCountKey: defaultMapFreeExtensionCount,
            hasShownUpgradeAlertForVersionKey: defaultHasShownUpgradeAlertForVersion,
            appLaunchCountKey: defaultAppLaunchCount,
            lastVersionCheckKey: defaultLastVersionCheck,
            combatProbabilityKey: defaultCombatProbability,
            hasViewedEnemyMagicGuideKey: defaultHasViewedEnemyMagicGuide,
            bayeDisplayOrientationKey: defaultBayeDisplayOrientation,
            bayeResolutionKey: defaultBayeResolution,
            expMultiplierKey: defaultExpMultiplier,
            goldMultiplierKey: defaultGoldMultiplier,
            itemMultiplierKey: defaultItemMultiplier,
            agricultureMultiplierKey: defaultAgricultureMultiplier,
            commerceMultiplierKey: defaultCommerceMultiplier
        ]
        defaults.register(defaults: defaultValues)
    }
    
    // MARK: - Version Update Alert Helper
    /// 检查指定版本是否已显示过升级提示
    func hasShownUpgradeAlert(for version: String) -> Bool {
        return hasShownUpgradeAlertForVersion == version
    }
    
    /// 标记指定版本已显示过升级提示
    func markUpgradeAlertShown(for version: String) {
        hasShownUpgradeAlertForVersion = version
    }
    
    /// 获取当前应用版本号
    func getCurrentVersion() -> String {
        let version = Bundle.main.infoDictionary?["CFBundleShortVersionString"] as? String ?? "1.0.0"
        return version
    }
    
    // MARK: - App Launch Count Helper
    /// 增加应用启动次数
    func incrementAppLaunchCount() {
        appLaunchCount += 1
    }
    
    /// 重置应用启动次数（版本检查后）
    func resetAppLaunchCount() {
        appLaunchCount = 0
    }
}
