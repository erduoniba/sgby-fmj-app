//
//  HDAppColor.swift
//  HDBayeApp
//
//  Created by 邓立兵 on 2025/6/21.
//

import UIKit

class HDAppColor {
    static func fmjThemeColor() -> UIColor {
        return UIColor(red: 19/255.0, green: 39/255.0, blue: 72/255.0, alpha: 1)
    }
    
    // MARK: - 霸业App滤镜主题色
    
    // 复古滤镜颜色
    struct Vintage {
        static let tintColor = UIColor(red: 0.6, green: 0.4, blue: 0.3, alpha: 1.0)  // 深棕色
        static let barTintColor = UIColor(red: 179/255.0, green: 163/255.0, blue: 142/255.0, alpha: 1.0)  // 浅棕色背景
        static let titleTextColor = UIColor(red: 0.3, green: 0.2, blue: 0.1, alpha: 1.0)  // 深棕色文字
        static let backgroundColor = barTintColor
        static let webViewBackgroundColor = barTintColor
    }
    
    // 清爽滤镜颜色
    struct Refreshing {
        static let tintColor = UIColor(red: 0.2, green: 0.6, blue: 0.9, alpha: 1.0)  // 明亮蓝色
        static let barTintColor = UIColor(red: 182/255.0, green: 207/255.0, blue: 207/255.0, alpha: 1.0)
        static let titleTextColor = UIColor(red: 0.1, green: 0.3, blue: 0.5, alpha: 1.0)  // 深蓝色文字
        static let backgroundColor = barTintColor
        static let webViewBackgroundColor = barTintColor
    }
    
    // 默认主题颜色
    struct Default {
        static let tintColor = UIColor.systemBlue
        static let barTintColor = UIColor.systemBackground
        static let titleTextColor = UIColor.label
        static let backgroundColor = UIColor.systemBackground
        static let webViewBackgroundColor = UIColor.systemBackground
    }
    
    // 根据滤镜类型获取主题色
    static func themeColors(for filterMode: String) -> (tint: UIColor, barTint: UIColor, titleText: UIColor, background: UIColor, webViewBackground: UIColor) {
        switch filterMode {
        case "vintage1980":
            return (Vintage.tintColor, Vintage.barTintColor, Vintage.titleTextColor, Vintage.backgroundColor, Vintage.webViewBackgroundColor)
        case "refreshing":
            return (Refreshing.tintColor, Refreshing.barTintColor, Refreshing.titleTextColor, Refreshing.backgroundColor, Refreshing.webViewBackgroundColor)
        default:
            return (Default.tintColor, Default.barTintColor, Default.titleTextColor, Default.backgroundColor, Default.webViewBackgroundColor)
        }
    }
}
