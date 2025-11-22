//
//  AppDelegate.swift
//  HDBayeApp
//
//  Created by harrydeng on 2025/4/9.
//

import UIKit

@main
class AppDelegate: UIResponder, UIApplicationDelegate {

    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {
        
        HDAppsTool.setupApps()
        
        return true
    }

    // MARK: UISceneSession Lifecycle

    func application(_ application: UIApplication, configurationForConnecting connectingSceneSession: UISceneSession, options: UIScene.ConnectionOptions) -> UISceneConfiguration {
        // Called when a new scene session is being created.
        // Use this method to select a configuration to create the new scene with.
        return UISceneConfiguration(name: "Default Configuration", sessionRole: connectingSceneSession.role)
    }

    func application(_ application: UIApplication, didDiscardSceneSessions sceneSessions: Set<UISceneSession>) {
        // Called when the user discards a scene session.
        // If any sessions were discarded while the application was not running, this will be called shortly after application:didFinishLaunchingWithOptions.
        // Use this method to release any resources that were specific to the discarded scenes, as they will not return.
    }

    // MARK: - Orientation Control

    func application(_ application: UIApplication, supportedInterfaceOrientationsFor window: UIWindow?) -> UIInterfaceOrientationMask {
        let orientationSetting = HDAppData.shared.bayeDisplayOrientation

        switch orientationSetting {
        case "auto":
            // 跟随系统：支持所有方向
            return .all
        case "landscape":
            // 强制横屏：只支持横屏方向
            return .landscape
        case "portrait":
            // 强制竖屏：只支持竖屏方向
            return .portrait
        default:
            // 默认跟随系统
            return .all
        }
    }


}

