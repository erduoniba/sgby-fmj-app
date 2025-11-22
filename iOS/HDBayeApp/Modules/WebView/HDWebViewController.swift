//
//  ViewController.swift
//  HDBayeApp
//
//  Created by harrydeng on 2025/4/9.
//

import UIKit

// @preconcurrency 告知编译器该模块不使用 Swift Concurrency（如 async/await）
@preconcurrency import WebKit

import SSZipArchive
import AVFoundation

/*
 超详细 WKWebView 开发和使用经验 https://www.51cto.com/article/676860.html
 */

class HDWebViewController: HDBaseViewController {
  // 0: 远程调试、1:沙盒资源调试
  // 远程调试下，需要设置手机代理为localHost
  private var mode = 1
  private var localHost = ""
  
  private var indexPageName: String = ""
  var wkWebView: WKWebView?
  private var isNavigationBarHidden = false
  private var aboutButton: UIBarButtonItem?
  
  var changeLibButton: UIBarButtonItem?
  
  // 离线包相关
  private var offlineZipName = ""
  private var offlineDirName = ""
  
  var alreadyInjectJS: Bool = false
  
  private var bottomMargin: CGFloat = 0
  
  var colorFilterMode: String = HDAppData.shared.selectedFilter
  var needUpdateColorFilterMode = false
  var needUpdateGameSpeed = false
  var needUpdateCombatProbability = false
  var gameSpeed: Float = HDAppData.shared.gameSpeed
  var useNewEngine: Bool = HDAppData.shared.useNewEngine
  var showMapContainer: Bool = HDAppData.shared.showMapContainer
  var combatProbability: Int = HDAppData.shared.combatProbability
    
  var libsList: [String: String] = [:]
  
  
  // 导航栏控制按钮
  private var navigationToggleButton: UIButton?
  private var buttonAutoHideTimer: Timer?
  
  override func viewDidLoad() {
    super.viewDidLoad()
    
    if HDAppsTool.hdAppName() == .hdBayeApp {
      localHost = "\(HDFeedbackService.localHost):12345/baye/baye_offline"
    }
    else {
      localHost = "\(HDFeedbackService.localHost):12345/fmj/fmj_offline"
    }
    
    indexPageName = HDAppsTool.indexPageName()
    offlineZipName = HDAppsTool.offlineZipName()
    offlineDirName = HDAppsTool.offlineDirName()
    
    unzipOfflinePackageIfNeeded()
    setupAboutButton()
    setupWebView()
    loadWebPage(name: indexPageName)
    
    let window = UIApplication.shared.windows.filter({ $0.isKeyWindow }).last
    bottomMargin = window?.safeAreaInsets.bottom ?? 0
    
    registerNotifiction()
    
    if HDAppsTool.hdAppName() == .hdFmjApp {
      setupNavigationBar()
      aboutButton?.tintColor = .white
      
      self.title = HDAppData.shared.choiceLib["value"] ?? HDAppsTool.title()
    }
    else {
      self.title = HDAppsTool.title()
      
      // 配置导航栏样式
      if let navigationBar = navigationController?.navigationBar {
        navigationBar.isTranslucent = true
        navigationBar.backgroundColor = .secondarySystemGroupedBackground
        
        // 去掉导航栏下面的黑线
        navigationBar.shadowImage = UIImage()
        navigationBar.setBackgroundImage(UIImage(), for: .default)
        
        let appearance = UINavigationBarAppearance()
        appearance.configureWithOpaqueBackground()
        appearance.backgroundColor = .secondarySystemGroupedBackground
        appearance.shadowColor = .clear  // iOS 13+ 去掉阴影线
        appearance.shadowImage = UIImage()  // 清除阴影图片
        
        navigationBar.standardAppearance = appearance
        navigationBar.scrollEdgeAppearance = appearance
        if #available(iOS 15.0, *) {
          navigationBar.compactScrollEdgeAppearance = appearance
        }
      }
    }
    
    // 检查是否需要显示版本升级提示
    checkAndShowVersionUpgradeAlert()
    
    // 增加应用启动次数并检查App Store版本更新
    HDAppData.shared.incrementAppLaunchCount()
    HDVersionChecker.shared.checkForUpdateIfNeeded()
  }
  
  deinit {
    // 移除通知观察者
    NotificationCenter.default.removeObserver(self)
    
    // 停止设备方向监听
    UIDevice.current.endGeneratingDeviceOrientationNotifications()
  }
  
  override func viewWillAppear(_ animated: Bool) {
    super.viewWillAppear(animated)
    
    updateComonCapacity()
  }
  
  override func viewWillLayoutSubviews() {
    super.viewWillLayoutSubviews()
    
    updateWebViewFrame()
  }
}

extension HDWebViewController {
  private func loadWebPage(name: String) {
#if !DEBUG
    mode = 1
#endif
    
    if mode == 0 {
      // 需要使用电脑的ip地址，而不能使用 127.0.0.1
      let hostUrl = "http://\(localHost)/\(name)"
      guard let url = URL(string: hostUrl) else {
        return
      }
      wkWebView?.load(URLRequest(url: url))
      return
    }
    
    let pagePath = (offlineDirPath as NSString).appendingPathComponent(name)
    guard FileManager.default.fileExists(atPath: pagePath) else {
      debugPrint("页面文件不存在: \(pagePath)")
      return
    }
    guard let pageUrl = URL(string: "\(HDWebURLSchemeHandler.bayeScheme)://\(pagePath)") else {
      return
    }
    wkWebView?.load(URLRequest(url: pageUrl))
  }
  
  private func setupWebView() {
    let config = WKWebViewConfiguration()
    
    /*
     iOS13后，PadOS上，WKWebView的UserAgent变成了类似这样：
     Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15) AppleWebKit/605.1.15 (KHTML, like Gecko)
     preferredContentMode 设置成 .mobile 后回复正常
     */
    config.defaultWebpagePreferences.preferredContentMode = .mobile
    
    // 处理离线包自定义的 scheme，h5数据从本地离线包获取
    HDWebURLSchemeHandler.addCustomURLScheme(config: config)
    
    // 支持原生和h5中js相互调用，并且解决循环依赖导致内存泄露问题
    HDWebJSBridge.addCustomJS(userContentController: config.userContentController)
    
    let wkWebView = WKWebView(frame: view.bounds, configuration: config)
    wkWebView.navigationDelegate = self
    
    // 支持Safari调试
#if DEBUG
    if #available(iOS 16.4, *) {
      wkWebView.isInspectable = true
    }
#endif
    
    view.addSubview(wkWebView)
    self.wkWebView = wkWebView
    
    if HDAppsTool.hdAppName() == .hdFmjApp {
      view.backgroundColor = HDAppColor.fmjThemeColor()
      wkWebView.scrollView.backgroundColor = HDAppColor.fmjThemeColor()
      // 重要：确保透明背景生效（默认 true 会强制白色背景）
      wkWebView.scrollView.isOpaque = false
    } else {
      // 三国霸业也需要设置透明背景以支持主题色
      wkWebView.isOpaque = false
      wkWebView.backgroundColor = UIColor.clear
      wkWebView.scrollView.isOpaque = false
      wkWebView.scrollView.backgroundColor = UIColor.clear
    }
  }
  
  private func setupNavigationBar() {
    let appearance = UINavigationBarAppearance()
    appearance.configureWithOpaqueBackground()
    appearance.backgroundColor = HDAppColor.fmjThemeColor()
    appearance.titleTextAttributes = [.foregroundColor: UIColor.white,
                                      .font: UIFont.boldSystemFont(ofSize: 15)]
    navigationController?.navigationBar.standardAppearance = appearance
    navigationController?.navigationBar.scrollEdgeAppearance = appearance
    navigationController?.navigationBar.compactAppearance = appearance
    
    // 添加缩略图控制按钮
    let systemImageName = showMapContainer ? "map.fill" : "map"
    let thumbMapButton = UIBarButtonItem(
      image: UIImage(systemName: systemImageName),
      style: .plain,
      target: self,
      action: #selector(toggleThumbMapButtonTapped)
    )
    // 选中时使用高亮色，未选中时使用半透明白色
    thumbMapButton.tintColor = showMapContainer ? UIColor.white : UIColor.gray.withAlphaComponent(0.3)
    
    // 如果已经有右侧按钮，则添加到数组中
    if let rightBarButtonItems = navigationItem.rightBarButtonItems {
      navigationItem.rightBarButtonItems = rightBarButtonItems + [thumbMapButton]
    } else {
      navigationItem.rightBarButtonItems = [thumbMapButton]
    }
    
    if HDAppData.shared.showMapContainer {
      // 增加使用次数
      HDAppData.shared.mapUsageCount += 1
    }
  }
  
  @objc private func toggleThumbMapButtonTapped() {
    // 直接显示小地图，不再检查竖屏模式
    proceedWithMapToggle()
  }
  
  private func proceedWithMapToggle() {
    // 检查是否已购买金币或使用次数是否超限
    if !IAPManager.shared.isProductPurchased(productId: HDAppsTool.doubleGoldId()) {
      let currentLimit = getCurrentMapUsageLimit()
      if HDAppData.shared.mapUsageCount >= currentLimit {
        // 超过使用限制后，默认关闭小地图
        if showMapContainer {
          showMapContainer = false
          HDAppData.shared.showMapContainer = false
          updateMapDisplayAndButton()
        }
        showMapPurchaseAlert()
        return
      }
    }
    
    // 切换地图容器的显示状态
    showMapContainer.toggle()
    
    // 保存到HDAppData
    HDAppData.shared.showMapContainer = showMapContainer
    
    updateMapDisplayAndButton()
  }
  
  private func updateMapDisplayAndButton() {
    // 调用JS方法隐藏/显示地图
    let jsCode = """
            window.showMapContainer(\(showMapContainer));
            localStorage.setItem('showMapContainer', '\(showMapContainer)');
            """
    wkWebView?.evaluateJavaScript(jsCode) { (result, error) in
      if let error = error {
        debugPrint("切换地图显示状态失败: \(error)")
      } else {
        debugPrint("切换地图显示状态成功: \(self.showMapContainer)")
      }
    }
    
    // 更新按钮状态
    updateThumbMapButtonState()
  }
  
  private func updateThumbMapButtonState() {
    // 根据地图显示状态更新按钮图标和颜色
    let systemImageName = showMapContainer ? "map.fill" : "map"
    let buttonColor = showMapContainer ? UIColor.white : UIColor.gray.withAlphaComponent(0.3)
    
    if let thumbMapButton = navigationItem.rightBarButtonItems?.first(where: { $0.action == #selector(toggleThumbMapButtonTapped) }) {
      thumbMapButton.image = UIImage(systemName: systemImageName)
      thumbMapButton.tintColor = buttonColor
    }
  }
  
  private func getCurrentMapUsageLimit() -> Int {
    return 5 + (HDAppData.shared.mapFreeExtensionCount * 5)
  }
  
  private func showMapPurchaseAlert() {
    let currentLimit = getCurrentMapUsageLimit()
    let canExtend = HDAppData.shared.mapFreeExtensionCount < 5
    
    let alert = UIAlertController(
      title: "小地图功能",
      message: "您已使用完免费的\(currentLimit)次小地图功能。购买5倍金币后可永久使用小地图功能。",
      preferredStyle: .alert
    )
    
    alert.addAction(UIAlertAction(title: "取消", style: .cancel))
    
    // 只有在白嫖次数少于5次时才显示白嫖按钮
    if canExtend {
      alert.addAction(UIAlertAction(title: "再白嫖5次", style: .default) { [weak self] _ in
        HDAppData.shared.mapFreeExtensionCount += 1
        // 可以继续使用地图
        self?.showMapContainer = true
        HDAppData.shared.showMapContainer = true
        self?.updateMapDisplayAndButton( )
      })
    }
    
    alert.addAction(UIAlertAction(title: "立即购买", style: .default) { [weak self] _ in
      self?.showIAPView(HDAppsTool.doubleGoldId())
    })
    
    present(alert, animated: true)
  }
  
  private func backToInitialPage() {
    // 清除 WebView 的导航历史记录
    wkWebView?.backForwardList.perform(Selector(("_removeAllItems")))
    loadWebPage(name: indexPageName)
  }
  
  /// 重新加载离线包内容（用于离线包更新后刷新）
  public func reloadOfflinePackage() {
    HDAppsTool.debugLog("开始重新加载离线包内容")
    
    // 重置注入标志，确保新内容能够重新注入JS设置
    alreadyInjectJS = false
    
    DispatchQueue.main.async {
      // 清除 WebView 的导航历史记录
      self.wkWebView?.backForwardList.perform(Selector(("_removeAllItems")))
      
      // 重新加载首页
      self.loadWebPage(name: self.indexPageName)
      HDAppsTool.debugLog("离线包内容重新加载完成")
    }
  }
  
  private func setupAboutButton() {
    aboutButton = UIBarButtonItem(image: UIImage(systemName: "info.circle"), style: .plain, target: self, action: #selector(aboutButtonTapped))
    aboutButton?.tintColor = .label
    
    // 如果是三国霸业应用，添加游戏攻略按钮
    if HDAppsTool.hdAppName() == .hdBayeApp {
      // 创建自定义容器视图来控制按钮间距
      let containerView = UIView()
      containerView.frame = CGRect(x: 0, y: 0, width: 70, height: 44) // 进一步减小宽度
      
      // 创建攻略按钮，间距只有6px
      let strategyBtn = UIButton(type: .system)
      strategyBtn.setImage(UIImage(systemName: "book"), for: .normal)
      strategyBtn.tintColor = .label
      strategyBtn.frame = CGRect(x: 0, y: 0, width: 30, height: 44) // 两个按钮之间只有6px间距
      strategyBtn.addTarget(self, action: #selector(strategyButtonTapped), for: .touchUpInside)
      containerView.addSubview(strategyBtn)
      
      // 创建关于按钮
      let aboutBtn = UIButton(type: .system)
      aboutBtn.setImage(UIImage(systemName: "info.circle"), for: .normal)
      aboutBtn.tintColor = .label
      aboutBtn.frame = CGRect(x: 40, y: 0, width: 30, height: 44)
      aboutBtn.addTarget(self, action: #selector(aboutButtonTapped), for: .touchUpInside)
      containerView.addSubview(aboutBtn)
      
      // 将容器视图设置为导航栏按钮
      let customBarButtonItem = UIBarButtonItem(customView: containerView)
      navigationItem.rightBarButtonItem = customBarButtonItem
    }
    else {
      navigationItem.rightBarButtonItem = aboutButton
    }
  }
  
  @objc private func aboutButtonTapped() {
    let aboutVC = HDAboutViewController()
    let navigationController = UINavigationController(rootViewController: aboutVC)
    navigationController.modalPresentationStyle = .fullScreen
    present(navigationController, animated: true)
  }
  
  @objc private func strategyButtonTapped() {
    let strategyVC = HDStrategyWebViewController()
    
    // 如果有动态获取的攻略链接，则使用自定义URL
    if HDAppData.shared.currentGameHomeURL.isEmpty {
      HDAppData.shared.currentGameHomeURL = "http://harrydeng2025.xyz/baye_index/baye_data_index.html"
    }
    strategyVC.customURL = HDAppData.shared.currentGameHomeURL
    
    let navController = UINavigationController(rootViewController: strategyVC)
    navController.modalPresentationStyle = .pageSheet
    if #available(iOS 15.0, *) {
      if let sheet = navController.sheetPresentationController {
        sheet.detents = [.medium(), .large()]
        sheet.prefersGrabberVisible = true
      }
    }
    
    present(navController, animated: true)
    
    debugPrint("打开游戏攻略: \(HDAppData.shared.currentGameHomeURL)")
  }
}

extension HDWebViewController {
  private var offlineDirPath: String {
    let documentPath = NSSearchPathForDirectoriesInDomains(.documentDirectory, .userDomainMask, true)[0]
    return (documentPath as NSString).appendingPathComponent(offlineDirName)
  }
  
  private func unzipOfflinePackageIfNeeded() {
    let fileManager = FileManager.default
    let defaults = UserDefaults.standard
    let currentVersion = defaults.string(forKey: "offlinePackageVersion")
    let bundleVersion = offlineZipName // Bundle中的版本
    
    // 检查是否已经存在离线包目录
    if fileManager.fileExists(atPath: offlineDirPath) {
      // 如果存在离线包目录，检查是否有有效的版本记录
      if let currentVersion = currentVersion, !currentVersion.isEmpty {
        // 比较版本号，提取数字部分进行比较
        let currentVersionNum = extractVersionNumber(from: currentVersion)
        let bundleVersionNum = extractVersionNumber(from: bundleVersion)
        
        // 如果Bundle版本更新（大于当前版本），需要使用Bundle版本
        if bundleVersionNum > currentVersionNum {
          HDAppsTool.debugLog("检测到应用更新，Bundle版本(\(bundleVersion))大于当前版本(\(currentVersion))，使用新版本")
          try? fileManager.removeItem(atPath: offlineDirPath)
          // 继续执行下面的解压流程
        } else {
          // Bundle版本不高于当前版本，保留现有版本（可能是从服务器下载的更新版本）
          HDAppsTool.debugLog("离线包已存在，版本: \(currentVersion)，Bundle版本: \(bundleVersion)，保留当前版本")
          
          // 验证离线包完整性
          let indexPath = (offlineDirPath as NSString).appendingPathComponent("index.html")
          let choosePath = (offlineDirPath as NSString).appendingPathComponent("choose.html")
          let hasValidContent = fileManager.fileExists(atPath: indexPath) || fileManager.fileExists(atPath: choosePath)
          
          if hasValidContent {
            HDAppsTool.debugLog("离线包验证通过，保留当前版本")
            return
          } else {
            HDAppsTool.debugLog("离线包损坏，需要重新安装")
            try? fileManager.removeItem(atPath: offlineDirPath)
          }
        }
      } else {
        // 没有版本记录但有目录，可能是损坏的，删除后重新解压
        HDAppsTool.debugLog("离线包目录存在但无版本记录，删除并重新解压")
        try? fileManager.removeItem(atPath: offlineDirPath)
      }
    }
    
    // 只有在没有离线包、离线包损坏或需要更新时，才从Bundle解压
    guard let zipPath = Bundle.main.path(forResource: offlineZipName, ofType: "zip") else {
      HDAppsTool.debugLog("Bundle中没有找到离线包: \(offlineZipName).zip")
      return
    }
    
    HDAppsTool.debugLog("从Bundle解压离线包: \(offlineZipName)")
    
    // 解压离线包
    let password = HDAppsTool.zipPassword()
    var success = false
    
    if password.isEmpty {
      // 无密码解压
      success = SSZipArchive.unzipFile(atPath: zipPath, toDestination: offlineDirPath)
    } else {
      // 密码解压
      success = ((try? SSZipArchive.unzipFile(atPath: zipPath, toDestination: offlineDirPath, overwrite: true, password: password)) != nil)
    }
    
    if !success {
      HDAppsTool.debugLog("ZIP文件解压失败: \(zipPath)")
      return
    }
    
    // 保存Bundle版本号
    defaults.set(offlineZipName, forKey: "offlinePackageVersion")
    HDAppsTool.debugLog("Bundle离线包解压成功，版本: \(offlineZipName)")
  }
  
  // 从版本字符串中提取版本号用于比较（格式：xxx-offline-YYYYMMDDHH）
  private func extractVersionNumber(from versionString: String) -> Int {
    // 提取最后的数字部分（YYYYMMDDHH）
    let components = versionString.components(separatedBy: "-")
    if let lastComponent = components.last, let number = Int(lastComponent) {
      return number
    }
    return 0
  }
}

extension HDWebViewController: WKNavigationDelegate {
  // 页面开始加载时调用
  func webView(_ webView: WKWebView, didStartProvisionalNavigation navigation: WKNavigation!) {
    debugPrint("网页开始加载: \(webView.url?.absoluteString ?? "")")
    
    if HDAppsTool.hdAppName() == .hdBayeApp {
      // 重置注入标志，确保新页面能够重新注入JS设置
      // 这对于从choose.html跳转到m.html等页面切换非常重要
      alreadyInjectJS = false
    }
  }
  
  // 当内容开始返回时调用
  func webView(_ webView: WKWebView, didCommit navigation: WKNavigation!) {
    debugPrint("网页内容开始返回: \(webView.url?.absoluteString ?? "")")
  }
  
  // 页面加载完成之后调用
  func webView(_ webView: WKWebView, didFinish navigation: WKNavigation!) {
    debugPrint("网页加载完成: \(webView.url?.absoluteString ?? "")")
    
    // 从 URL 的 name 参数提取标题并设置
    updateTitleFromURL(webView.url)
    
    // 运行在 Mac 设备上
    if #available(iOS 14.0, *) {
      if ProcessInfo.processInfo.isiOSAppOnMac {
        // 0:横屏&触控 "2"：横屏&手势 "1"竖屏&键盘 "3"竖屏&键盘&触控
        setLocalStorage(key: "baye/mpage", value: "1")
        
        // 游戏中地图大小，1是加大版本
        setLocalStorage(key: "baye/resolution", value: "1")
        
        // "mobile"：移动端 "desktop"：桌面端
        setLocalStorage(key: "baye/uiKind", value: "mobile")
      }
    }
    
    // 页面加载成功后，原生注入代码
    // Baye和FMJ都使用相同的JS注入逻辑
    injectCommonJSHooks()
    
    // 禁用长按事件
    disableTouchCallout()
  }
  
  // 从 URL 参数提取标题并设置
  private func updateTitleFromURL(_ url: URL?) {
    guard let url = url,
          let urlComponents = URLComponents(url: url, resolvingAgainstBaseURL: false),
          let queryItems = urlComponents.queryItems else {
      return
    }
    
    // 查找 name 参数
    if let nameItem = queryItems.first(where: { $0.name == "name" }),
       let encodedName = nameItem.value {
      // URL 解码
      let decodedName = encodedName.removingPercentEncoding ?? encodedName
      
      // 设置标题
      DispatchQueue.main.async { [weak self] in
        self?.title = decodedName
        debugPrint("从 URL 设置标题: \(decodedName)")
        
        // TODO: 这里属于硬编码，强制设置为词典分辨率
        if (decodedName == "平衡版2.0三国战纪") {
          self?.updateBayeResolution("0")
        }
      }
    }
  }
  
  // 页面加载失败时调用
  func webView(_ webView: WKWebView, didFail navigation: WKNavigation!, withError error: Error) {
    debugPrint("网页加载失败: \(webView.url?.absoluteString ?? ""), 错误: \(error)")
  }
  
  // 在发送请求之前，决定是否跳转
  func webView(_ webView: WKWebView, decidePolicyFor navigationAction: WKNavigationAction, decisionHandler: @escaping (WKNavigationActionPolicy) -> Void) {
    debugPrint("准备加载页面: \(navigationAction.request.url?.absoluteString ?? "")")
    if let pageName = navigationAction.request.url?.lastPathComponent {
      if pageName == "index.html", HDAppsTool.hdAppName() == .hdBayeApp {
        backToInitialPage()
        decisionHandler(.cancel)
        return
      }
    }
    // 允许跳转
    decisionHandler(.allow)
  }
  
  // 在收到响应后，决定是否跳转
  func webView(_ webView: WKWebView, decidePolicyFor navigationResponse: WKNavigationResponse, decisionHandler: @escaping (WKNavigationResponsePolicy) -> Void) {
    debugPrint("收到页面响应: \(navigationResponse.response.url?.absoluteString ?? "")")
    // 允许跳转
    decisionHandler(.allow)
  }
}

// MARK: - Version Update Alert
extension HDWebViewController {
  
  private func checkAndShowVersionUpgradeAlert() {
    let currentVersion = HDAppData.shared.getCurrentVersion()
    let targetVersion = HDConstants.VersionAlert.targetVersion
    
    // 只有当前版本是目标版本且未显示过升级提示时才显示
    guard currentVersion == targetVersion else { return }
    guard !HDAppData.shared.hasShownUpgradeAlert(for: targetVersion) else { return }
    
    // 延迟显示，确保视图已经完全加载
    DispatchQueue.main.asyncAfter(deadline: .now() + 1.0) { [weak self] in
      self?.showVersionUpgradeAlert()
    }
  }
  
  private func showVersionUpgradeAlert() {
    var title = HDConstants.VersionAlert.fmjTitle
    var message = HDConstants.VersionAlert.fmjMessage
    if HDAppsTool.hdAppName() == .hdBayeApp {
      title = HDConstants.VersionAlert.bayeTitle
      message = HDConstants.VersionAlert.bayeMessage
    }
    let alert = UIAlertController(
      title: title,
      message: message,
      preferredStyle: .alert
    )
    
    let confirmAction = UIAlertAction(
      title: HDConstants.VersionAlert.confirmButtonTitle,
      style: .default
    ) { [weak self] _ in
      // 标记当前版本已显示过升级提示
      let targetVersion = HDConstants.VersionAlert.targetVersion
      HDAppData.shared.markUpgradeAlertShown(for: targetVersion)
      
      // 可选：添加一些炫酷的动画效果
      self?.showUpgradeCompletedAnimation()
      
      // 使用HDStrategyWebViewController打开指定链接
      let webViewController = HDStrategyWebViewController()
      webViewController.customURL = "http://harrydeng2025.xyz/fmj_index/index.html"
      webViewController.title = "伏魔记故事"
      let navController = UINavigationController(rootViewController: webViewController)
      navController.modalPresentationStyle = .pageSheet
      self?.present(navController, animated: true)
    }
    
    alert.addAction(confirmAction)
    present(alert, animated: true, completion: nil)
  }
  
  private func showUpgradeCompletedAnimation() {
    // 添加升级完成的炫酷动画效果
    DispatchQueue.main.asyncAfter(deadline: .now() + 0.3) { [weak self] in
      self?.showConfettiView()
    }
  }
  
  /// 更新WebView框架
  private func updateWebViewFrame() {
    guard let webView = wkWebView else { return }
    webView.frame = view.bounds
  }  
}
