//
//  HDStoryWebViewController.swift
//  HDBayeApp
//
//  Created by harrydeng on 2025/6/30.
//

import UIKit
@preconcurrency import WebKit

class HDStrategyWebViewController: HDBaseViewController {
    
    private var wkWebView: WKWebView?
    var customPageName: String?
    var customURL: String?  // 新增：支持自定义 URL
    
    private var strategyPageName: String {
        return customPageName ?? HDAppsTool.strategyPageName()
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        setupWebView()
        loadStrategyPage()
        
        if let navigationBar = navigationController?.navigationBar {
            navigationBar.isTranslucent = true
            navigationBar.backgroundColor = .secondarySystemGroupedBackground
            let appearance = UINavigationBarAppearance()
            appearance.configureWithOpaqueBackground()
            appearance.backgroundColor = .secondarySystemGroupedBackground
            navigationBar.standardAppearance = appearance
            navigationBar.scrollEdgeAppearance = appearance
            if #available(iOS 15.0, *) {
                navigationBar.compactScrollEdgeAppearance = appearance
            }
        }
        
        view.backgroundColor = .systemGroupedBackground
        navigationController?.navigationBar.shadowImage = UIImage()

        if let viewControllers = navigationController?.viewControllers, viewControllers.count > 1 {
            let backButton = UIBarButtonItem(image: UIImage(systemName: "chevron.backward"), style: .plain, target: self, action: #selector(backButtonTapped))
            backButton.tintColor = .label
            navigationItem.leftBarButtonItem = backButton
        }
        else {
            // 自定义关闭按钮
            let cancelButton = UIBarButtonItem(
                image: UIImage(systemName: "xmark"),
                style: .plain,
                target: self,
                action: #selector(cancelButtonTapped)
            )
            cancelButton.tintColor = .label
            navigationItem.leftBarButtonItem = cancelButton
        }
        
        // 如果使用自定义URL，添加分享按钮
        if customURL != nil {
            let shareButton = UIBarButtonItem(
                image: UIImage(systemName: "square.and.arrow.up"),
                style: .plain,
                target: self,
                action: #selector(shareButtonTapped)
            )
            shareButton.tintColor = .label
            navigationItem.rightBarButtonItem = shareButton
        }
        
        // 确保启用滑动返回手势
        enableSwipeBackGesture()
    }
    
    override func viewWillLayoutSubviews() {
        super.viewWillLayoutSubviews()
        // AutoLayout会自动处理WebView的布局，不需要手动设置frame
    }
    
    private func setupWebView() {
        let config = WKWebViewConfiguration()
        
        // 设置为移动模式
        config.defaultWebpagePreferences.preferredContentMode = .mobile
        
        // 添加自定义URL Scheme处理器
        HDWebURLSchemeHandler.addCustomURLScheme(config: config)
        
        let wkWebView = WKWebView(frame: view.bounds, configuration: config)
        wkWebView.navigationDelegate = self
        
        // 支持Safari调试
        #if DEBUG
        if #available(iOS 16.4, *) {
            wkWebView.isInspectable = true
        }
        #endif
        
        view.addSubview(wkWebView)
        
        // 使用AutoLayout确保正确布局
        wkWebView.translatesAutoresizingMaskIntoConstraints = false

        // 设置AutoLayout约束
        NSLayoutConstraint.activate([
            wkWebView.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor),
            wkWebView.leadingAnchor.constraint(equalTo: view.leadingAnchor),
            wkWebView.trailingAnchor.constraint(equalTo: view.trailingAnchor),
            wkWebView.bottomAnchor.constraint(equalTo: view.safeAreaLayoutGuide.bottomAnchor)
        ])
        
        self.wkWebView = wkWebView
    }
    
    @objc private func backButtonTapped() {
        navigationController?.popViewController(animated: true)
    }
  
    @objc private func cancelButtonTapped() {
        dismiss(animated: true)
    }
    
    @objc private func shareButtonTapped() {
        guard let urlString = customURL, !urlString.isEmpty else {
            debugPrint("没有可分享的自定义URL")
            return
        }
        
        guard let url = URL(string: urlString) else {
            debugPrint("无效的自定义URL: \(urlString)")
            return
        }
        
        // 创建分享内容
        let shareText = "分享攻略链接"
        let items: [Any] = [shareText, url]
        
        // 创建分享控制器
        let activityViewController = UIActivityViewController(
            activityItems: items,
            applicationActivities: nil
        )
        
        // 在 iPad 上需要设置 popoverPresentationController
        if let popover = activityViewController.popoverPresentationController {
            popover.barButtonItem = navigationItem.rightBarButtonItem
        }
        
        // 显示分享控制器
        present(activityViewController, animated: true)
        
        debugPrint("分享自定义URL: \(urlString)")
    }
    
    private func loadStrategyPage() {
        // 如果设置了自定义 URL，直接加载网络 URL
        if let customURL = customURL, !customURL.isEmpty {
            guard let url = URL(string: customURL) else {
                debugPrint("无效的自定义 URL: \(customURL)")
                return
            }
            debugPrint("加载自定义攻略 URL: \(customURL)")
            wkWebView?.load(URLRequest(url: url))
            return
        }
        
        // 否则加载本地文件
        let offlineDirName = HDAppsTool.offlineDirName()
        let documentPath = NSSearchPathForDirectoriesInDomains(.documentDirectory, .userDomainMask, true)[0]
        let offlineDirPath = (documentPath as NSString).appendingPathComponent(offlineDirName)
        
        let pagePath = (offlineDirPath as NSString).appendingPathComponent(strategyPageName)
        guard FileManager.default.fileExists(atPath: pagePath) else {
            debugPrint("策略页面文件不存在: \(pagePath)")
            return
        }
        
        // 使用自定义scheme加载本地文件
        guard let pageUrl = URL(string: "\(HDWebURLSchemeHandler.bayeScheme)://\(pagePath)") else {
            debugPrint("无法创建策略页面URL")
            return
        }
        
        debugPrint("加载本地攻略页面: \(pageUrl)")
        wkWebView?.load(URLRequest(url: pageUrl))
    }
    
    // 启用滑动返回手势
    private func enableSwipeBackGesture() {
        // 确保导航控制器的滑动返回手势是启用的
        navigationController?.interactivePopGestureRecognizer?.isEnabled = true
        
        // 设置手势代理
        navigationController?.interactivePopGestureRecognizer?.delegate = self
        
        // 确保WKWebView不会拦截边缘滑动手势
        if let wkWebView = self.wkWebView {
            // 允许WKWebView的滚动视图识别多个手势
            wkWebView.scrollView.panGestureRecognizer.require(toFail: navigationController?.interactivePopGestureRecognizer ?? UIGestureRecognizer())
            
            // 禁用WKWebView的边缘滑动手势（如果有）
            wkWebView.allowsBackForwardNavigationGestures = false
        }
    }
}

// MARK: - UIGestureRecognizerDelegate
extension HDStrategyWebViewController: UIGestureRecognizerDelegate {
    // 允许同时识别多个手势
    func gestureRecognizer(_ gestureRecognizer: UIGestureRecognizer, shouldRecognizeSimultaneouslyWith otherGestureRecognizer: UIGestureRecognizer) -> Bool {
        return true
    }
    
    // 确保导航控制器的滑动返回手势能够工作
    func gestureRecognizerShouldBegin(_ gestureRecognizer: UIGestureRecognizer) -> Bool {
        // 只有当是导航控制器的交互式弹出手势时才特殊处理
        if gestureRecognizer == navigationController?.interactivePopGestureRecognizer {
            // 确保不是根视图控制器
            return navigationController?.viewControllers.count ?? 0 > 1
        }
        return true
    }
}

// MARK: - WKNavigationDelegate
extension HDStrategyWebViewController: WKNavigationDelegate {
    // 页面开始加载时调用
    func webView(_ webView: WKWebView, didStartProvisionalNavigation navigation: WKNavigation!) {
        debugPrint("策略页面开始加载: \(webView.url?.absoluteString ?? "")")
    }
    
    // 页面加载完成之后调用
    func webView(_ webView: WKWebView, didFinish navigation: WKNavigation!) {
        debugPrint("策略页面加载完成: \(webView.url?.absoluteString ?? "")")
        
        // 动态获取页面标题
        webView.evaluateJavaScript("document.title") { [weak self] (result, error) in
            DispatchQueue.main.async {
                if let title = result as? String, !title.isEmpty {
                    self?.title = title
                    
                    if title.contains("三国霸业"), !HDAppData.shared.currentGameTitle.isEmpty {
                        self?.title = title.replacingOccurrences(of: "三国霸业", with: HDAppData.shared.currentGameTitle)
                    }
                    debugPrint("动态设置页面标题: \(title)")
                }
            }
        }
    }
    
    // 页面加载失败时调用
    func webView(_ webView: WKWebView, didFail navigation: WKNavigation!, withError error: Error) {
        debugPrint("策略页面加载失败: \(webView.url?.absoluteString ?? ""), 错误: \(error)")
    }
    
    // 在发送请求之前，决定是否跳转
    func webView(_ webView: WKWebView, decidePolicyFor navigationAction: WKNavigationAction, decisionHandler: @escaping (WKNavigationActionPolicy) -> Void) {
        debugPrint("准备加载页面: \(navigationAction.request.url?.absoluteString ?? "")")
        
        // 检查是否是外部链接
        if let url = navigationAction.request.url {
            let scheme = url.scheme?.lowercased() ?? ""
            
            if (url.absoluteString == "http://harrydeng2025.xyz/fmj_index/fmjll.html") {
                UIApplication.shared.open(url, options: [:], completionHandler: nil)
                decisionHandler(.cancel)
                return
            }
            
            if url.host == "harrydeng2025.xyz" {
                // 允许其他链接跳转
                decisionHandler(.allow)
                return
            }
            
            // 如果是http/https链接且不是我们的自定义scheme，则使用Safari打开
            if (scheme == "http" || scheme == "https") && scheme != HDWebURLSchemeHandler.bayeScheme {
                UIApplication.shared.open(url, options: [:], completionHandler: nil)
                decisionHandler(.cancel)
                return
            }
        }
        
        // 允许其他链接跳转
        decisionHandler(.allow)
    }
}
