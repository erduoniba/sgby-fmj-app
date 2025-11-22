//
//  HDWebURLSchemeHandler.swift
//  HDBayeApp
//
//  Created by harrydeng on 2025/4/11.
//

import WebKit

class HDWebURLSchemeHandler: NSObject {
    static let bayeScheme = "weboffline"
    static func addCustomURLScheme(config: WKWebViewConfiguration) {
        config.setURLSchemeHandler(HDWebURLSchemeHandler(), forURLScheme: bayeScheme)
    }
}

extension HDWebURLSchemeHandler: WKURLSchemeHandler {
    private func mimeTypeForPath(_ url: URL) -> String {
        let fileType = url.pathExtension.lowercased()
        let mimeTypes: [String: String] = [
            "html": "text/html",
            "js": "application/javascript",
            "json": "application/json",
            "css": "text/css",
            "png": "image/png",
            "jpg": "image/jpeg",
            "jpeg": "image/jpeg",
            "gif": "image/gif",
            "woff": "application/font-woff",
            "woff2": "application/font-woff2",
            "ttf": "application/font-ttf",
            "wasm": "application/wasm",
            "about": "text/plain"
        ]
        return mimeTypes[fileType] ?? "application/octet-stream"
    }
    
    func webView(_ webView: WKWebView, start urlSchemeTask: any WKURLSchemeTask) {
        guard let url = urlSchemeTask.request.url else {
            print(" ⚠️⚠️⚠️⚠️ webView start file not exist")
            let error = NSError(domain: "com.harry.weboffline", code: 404, userInfo: nil)
            urlSchemeTask.didFailWithError(error)
            return
        }
        
        // 从URL路径中获取相对路径
        let relativePath = url.path.replacingOccurrences(of: "/", with: "", options: .anchored)
        if FileManager.default.fileExists(atPath: relativePath),
           let fileData = NSData(contentsOfFile: relativePath) as? Data {
            let mimeType = mimeTypeForPath(url)
            debugPrint("webView start file exist: \(relativePath) mimeType: \(mimeType)")
            
            let response = URLResponse(url: url, mimeType: mimeType, expectedContentLength: fileData.count, textEncodingName: "UTF-8")
            urlSchemeTask.didReceive(response)
            urlSchemeTask.didReceive(fileData)
            urlSchemeTask.didFinish()
        }
        else {
            print(" ⚠️⚠️⚠️⚠️⚠️ webView start file not exist: \(relativePath)")
            let error = NSError(domain: "com.harry.bayeoffline", code: 404, userInfo: nil)
            urlSchemeTask.didFailWithError(error)
        }
    }
    
    func webView(_ webView: WKWebView, stop urlSchemeTask: any WKURLSchemeTask) {
        let url = urlSchemeTask.request.url
        let urlString = url?.path ?? ""
        debugPrint("webView stop: \(urlString)")
    }
}
