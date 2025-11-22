//
//  HDWebJSBridge.swift
//  HDBayeApp
//
//  Created by harrydeng on 2025/4/11.
//

import Foundation
import WebKit

protocol HDWKScriptMessageHandler: NSObject {
    func userContentController(_ userContentController: WKUserContentController, didReceive message: WKScriptMessage)
}

class HDWebJSBridge: NSObject {
    private var wkWebView: WKWebView?
    // 当前选择的霸业lib路径，用户保存游戏记录
    private var currentBayeLib = ""
    
    static func addCustomJS(userContentController: WKUserContentController) {
        let webJSBridge = HDWebJSBridge()
        userContentController.addUserScript(webJSBridge.timingHandlerScript())
        userContentController.add(webJSBridge, name: webJSBridge.scriptName())
        
        // 注入 JS Bridge 方法，让 H5 可以调用，h5存入数据，App启动的时候写入数据
        userContentController.add(webJSBridge, name: "sysStorageSet")
        
        // 霸业选择了那个lib的mod
        userContentController.add(webJSBridge, name: "chooseLib")
        
    }
    
    private func timingHandlerScript() -> WKUserScript {
        let scriptJS = """
        window.addEventListener('load', function() {
            console.log('performance.timing:', performance.timing);
            // 将performance.timing数据发送给iOS原生应用
                const jsonString = JSON.stringify(performance.timing);
                if (!jsonString) {
                    console.log("message to json error");
                    return;
                }
            window.webkit.messageHandlers.timingHandler.postMessage(jsonString);
        });
        """
        let script = WKUserScript(source: scriptJS, injectionTime: .atDocumentStart, forMainFrameOnly: true)
        return script
    }
    
    private func scriptName() -> String {
        return "timingHandler"
    }
}

extension HDWebJSBridge: WKScriptMessageHandler {
    func userContentController(_ userContentController: WKUserContentController, didReceive message: WKScriptMessage) {
        switch message.name {
        case "sysStorageSet":
            if let dict = message.body as? [String: Any],
               let path = dict["path"] as? String,
               let value = dict["value"] as? String {
                setStorage(path: path, value: value)
            }
        case "chooseLib":
            if let dict = message.body as? [String: Any],
               let path = dict["path"] as? String {
                wkWebView = message.webView
                chooseLib(path: path)
            }
        case "timingHandler":
            /*
             {"navigationStart":1744357166795,"unloadEventStart":0,"unloadEventEnd":0,"redirectStart":0,"redirectEnd":0,"fetchStart":1744357166795,"domainLookupStart":1744357166795,"domainLookupEnd":1744357166795,"connectStart":1744357166795,"connectEnd":1744357166795,"secureConnectionStart":0,"requestStart":1744357166795,"responseStart":1744357166795,"responseEnd":1744357166799,"domLoading":1744357166800,"domInteractive":1744357166829,"domContentLoadedEventStart":1744357166829,"domContentLoadedEventEnd":1744357166834,"domComplete":1744357166843,"loadEventStart":1744357166843,"loadEventEnd":0}
             */
            debugPrint("performance.timing: \(message.body)")
        default:
            break
        }
    }
}

extension HDWebJSBridge {
    // MARK: - 原生存储逻辑（使用 UserDefaults）
    private func setStorage(path: String, value: String) {
        // 霸业有多个mod，所以key需要带上mod名称，防止相互覆盖存档
        if HDAppsTool.hdAppName() == .hdBayeApp {
            let key = currentBayeLib + "_" + path
            debugPrint("HDWebJSBridge setStorage.key: \(key)")
            UserDefaults.standard.set(value, forKey: key)
            return
        }
        
        // 伏魔记也有多个mod，所以key需要带上mod名称，防止相互覆盖存档
        if HDAppsTool.hdAppName() == .hdFmjApp {
            if let tKey = HDAppData.shared.choiceLib["key"] {
                // 如果是伏魔记原版，不添加tKey作为前缀，兼容老版本
                if tKey == "FMJ" {
                    UserDefaults.standard.set(value, forKey: path)
                }
                else {
                    // 非原版，添加tKey作为前缀进行App缓存
                    var key = tKey + "_" + path
                    
                    // 伏魔记神女轮舞曲(木子02/03/04) 复用 伏魔记圆梦前奏曲(木子01) 存档
                    if tKey == "FMJSNLWQ" || tKey == "FMJMVKXQ" || tKey == "FMJHMAHQ" {
                      key = "FMJYMQZQ" + "_" + path
                    }
                    debugPrint("HDWebJSBridge setStorage.key: \(key)")
                    UserDefaults.standard.set(value, forKey: key)
                }
                return
            }
        }
        
        UserDefaults.standard.set(value, forKey: path)
    }
    
    private func chooseLib(path: String) {
        debugPrint("HDWebJSBridge chooseLib: \(path)")
        currentBayeLib = path
        
        // 从 libs.json 获取对应游戏的攻略链接
        updateGameHomeURL(for: path)
        
        // 选择mod后，需要将原生的存档，写入到cookie中
        injectBayeCacheJSHooks()
    }
    
    // 从 libs.json 获取对应游戏的攻略链接
    private func updateGameHomeURL(for libPath: String) {
        guard let documentsPath = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first else {
            debugPrint("HDWebJSBridge: 无法获取 Documents 目录")
            return
        }
        
        // 构建 libs.json 文件路径
        let libsJsonPath = documentsPath
            .appendingPathComponent("baye-offline")
            .appendingPathComponent("libs.json")
        
        // 检查文件是否存在
        guard FileManager.default.fileExists(atPath: libsJsonPath.path) else {
            debugPrint("HDWebJSBridge: libs.json 文件不存在: \(libsJsonPath.path)")
            return
        }
        
        // 读取并解析 JSON 文件
        guard let jsonData = try? Data(contentsOf: libsJsonPath) else { return }
        let libsArray = try? JSONSerialization.jsonObject(with: jsonData) as? [[String: Any]]
        guard let libs = libsArray else { return }
        
        // 查找匹配的游戏配置
        for lib in libs {
            guard let path = lib["path"] as? String, path == libPath else { continue }
            guard let homeURL = lib["home"] as? String else { return }
            HDAppData.shared.currentGameHomeURL = homeURL
            debugPrint("HDWebJSBridge: 找到攻略链接: \(homeURL)")
            
            guard let title = lib["title"] as? String else { return }
            HDAppData.shared.currentGameTitle = title
        }
    }

    private func injectBayeCacheJSHooks() {
        // 注入JavaScript代码
        // 缓存文件
        let keys = [
            "baye//data//sango0.sav",
            "baye//data//sango1.sav",
            "baye//data//sango2.sav",
            "baye//data//sango3.sav",
            "baye//data//sango4.sav",
            "baye//data//sango5.sav",
            "baye//data//sango6.sav",
            "baye//data//sango7.sav",
        ]
        for key in keys {
            let libKey = key + ".lib"
            let nameKey = key + ".name"
            removeItem(key: key)
            removeItem(key: libKey)
            removeItem(key: nameKey)
            
            // App原生有缓存，则写入到cookie中
            let realKey = currentBayeLib + "_" + key
            if let value = UserDefaults.standard.string(forKey: realKey), value.count > 0 {
                setLocalStorage(key: key, value: value)
            }
        }
    }
    
    private func setLocalStorage(key: String, value: String) {
        let jsCode = "localStorage.setItem('\(key)', '\(value)');"
        executeJavaScript(jsCode, context: "HDWebJSBridge 设置localStorage")
    }
    
    private func removeItem(key: String) {
        let jsCode = "localStorage.removeItem('\(key)');"
        executeJavaScript(jsCode, context: "HDWebJSBridge removeItem")
    }
    
    // 控制霸业游戏H5页面的分辨率
    func updateBayeResolution(_ resolution: String) {
        // 调用离线包中统一的分辨率设置API
        let jsCode = HDBayeHooks.bayeResolutionJS(resolution)
        executeJavaScript(jsCode, context: "设置霸业游戏分辨率")
    }
}

// JavaScript执行工具
extension HDWebJSBridge {
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

