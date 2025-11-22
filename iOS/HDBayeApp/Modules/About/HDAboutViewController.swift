//
//  HDAboutViewController.swift
//  HDBayeApp
//
//  Created by harrydeng on 2025/4/9.
//

import UIKit

import Toast_Swift

class HDAboutViewController: HDBaseViewController {
    private let tableView = UITableView(frame: .zero, style: .insetGrouped)
    private let sections = ["宗旨", "会员服务", "版本信息", "离线包管理", "页面设置", "开发者信息", "隐私政策"]
    private var versionInfo: [String] = []
    private struct Developer {
        let name: String
        let homepage: String
    }
    private var developers: [Developer] = []
    var selectedFilter: String? = HDAppData.shared.selectedFilter // 从HDAppData读取初始值
    var gameSpeed: Float = HDAppData.shared.gameSpeed // 从HDAppData读取初始值
    var useNewEngine: Bool = HDAppData.shared.useNewEngine // 从HDAppData读取初始值
    var combatProbability: Int = HDAppData.shared.combatProbability // 从HDAppData读取初始值
    var orientationMode: OrientationMode {
        get {
            return OrientationMode(rawValue: HDAppData.shared.bayeDisplayOrientation) ?? .followSystem
        }
        set {
            HDAppData.shared.bayeDisplayOrientation = newValue.rawValue
        }
    }
    
    private var closeAds = false
    
    private let headerView: UIView = {
        let view = UIView(frame: CGRect(x: 0, y: 0, width: UIScreen.main.bounds.width, height: 160))
        return view
    }()
    
    private let iconImageView: UIImageView = {
        let imageView = UIImageView(frame: CGRect(x: 0, y: 0, width: 140, height: 140))
        imageView.layer.cornerRadius = 10
        imageView.layer.masksToBounds = false
        if let appIcon = UIImage(named: "about_logo") {
            imageView.image = appIcon
        }
        imageView.isUserInteractionEnabled = true
        
        // 添加阴影效果增强可点击感
        imageView.layer.shadowColor = UIColor.systemBlue.cgColor
        imageView.layer.shadowOffset = CGSize(width: 0, height: 2)
        imageView.layer.shadowRadius = 8
        imageView.layer.shadowOpacity = 0.3
        
        return imageView
    }()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        setupUI()
        setupNavigationBar()
        setupData()
        
        NotificationCenter.default.addObserver(self,
                                               selector: #selector(appWillEnterForeground),
                                               name: UIApplication.willEnterForegroundNotification,
                                               object: nil)
    }
    
    deinit {
        NotificationCenter.default.removeObserver(self)
    }
    
    @objc func appWillEnterForeground() {
        if closeAds {
            showConfettiView()
            
            var style = ToastStyle()
            style.messageAlignment = .center
            view.makeToast(NSLocalizedString("重启 App 后即不再有您心爱的广告了～", comment: ""), duration:3, position: .center, style: style)
            
            HDAppData.shared.watchAdsStatus = false
            tableView.reloadData()
        }
        closeAds = false
    }
    
    override func viewWillLayoutSubviews() {
        super.viewWillLayoutSubviews()
        
        tableView.frame = view.bounds
        headerView.frame = CGRect(x: 0, y: 0, width: tableView.bounds.size.width, height: 160)
        iconImageView.center = CGPoint(x: headerView.bounds.width / 2, y: headerView.bounds.height / 2 + 0)
    }
    
    private func setupUI() {
        title = "关于"
        view.backgroundColor = .systemGroupedBackground
        navigationController?.navigationBar.shadowImage = UIImage()
        
        tableView.delegate = self
        tableView.dataSource = self
        tableView.register(UITableViewCell.self, forCellReuseIdentifier: "Cell")
        tableView.register(ColorFilterCell.self, forCellReuseIdentifier: ColorFilterCell.identifier)
        tableView.register(GameSpeedCell.self, forCellReuseIdentifier: GameSpeedCell.identifier)
        tableView.register(EngineSelectionCell.self, forCellReuseIdentifier: EngineSelectionCell.identifier)
        tableView.register(CombatProbabilityCell.self, forCellReuseIdentifier: CombatProbabilityCell.identifier)
        tableView.register(PortraitModeCell.self, forCellReuseIdentifier: PortraitModeCell.identifier)
        tableView.register(ExpMultiplierCell.self, forCellReuseIdentifier: ExpMultiplierCell.identifier)
        tableView.register(GoldMultiplierCell.self, forCellReuseIdentifier: GoldMultiplierCell.identifier)
        tableView.register(ItemMultiplierCell.self, forCellReuseIdentifier: ItemMultiplierCell.identifier)
        tableView.register(AgricultureMultiplierCell.self, forCellReuseIdentifier: AgricultureMultiplierCell.identifier)
        tableView.register(CommerceMultiplierCell.self, forCellReuseIdentifier: CommerceMultiplierCell.identifier)
        tableView.register(AllItemsTripleCell.self, forCellReuseIdentifier: AllItemsTripleCell.identifier)
        view.addSubview(tableView)
        tableView.frame = view.bounds
        tableView.autoresizingMask = [.flexibleWidth, .flexibleHeight]
        
        // 配置 headerView
        headerView.frame = CGRect(x: 0, y: 0, width: tableView.bounds.size.width, height: 160)
        headerView.addSubview(iconImageView)
        iconImageView.center = CGPoint(x: headerView.bounds.width / 2, y: headerView.bounds.height / 2 + 0)
        tableView.tableHeaderView = headerView
        
        // 添加logo点击事件
        let logoTapGesture = UITapGestureRecognizer(target: self, action: #selector(logoTapped))
        iconImageView.addGestureRecognizer(logoTapGesture)
        
        // 添加脉冲动画效果提示可点击
        addPulseAnimation(to: iconImageView)
    }
    
    override func purchaseSuccessCompele(_ productId: String) {
        tableView.reloadData()
    }
    
    private func setupNavigationBar() {
        // 添加右上角分享按钮
        let shareButton = UIBarButtonItem(
            image: UIImage(systemName: "square.and.arrow.up"),
            style: .plain,
            target: self,
            action: #selector(shareAppButtonAction)
        )
        shareButton.tintColor = .label
        navigationItem.rightBarButtonItem = shareButton

        let backButton = UIBarButtonItem(image: UIImage(systemName: "xmark"), style: .plain, target: self, action: #selector(backButtonTapped))
        backButton.tintColor = .label
        navigationItem.leftBarButtonItem = backButton
    }
    
    @objc private func backButtonTapped() {
        dismiss(animated: true)
    }
    
    @objc private func logoTapped() {
        // 添加点击反馈动画
        UIView.animate(withDuration: 0.1, animations: {
            self.iconImageView.transform = CGAffineTransform(scaleX: 0.95, y: 0.95)
        }) { _ in
            UIView.animate(withDuration: 0.1) {
                self.iconImageView.transform = .identity
            }
        }
        
        // 使用HDStrategyWebViewController打开指定链接
        let webViewController = HDStrategyWebViewController()
        webViewController.customURL = "http://harrydeng2025.xyz/fmj_index/index.html"
        webViewController.title = "伏魔记故事"
        let navController = UINavigationController(rootViewController: webViewController)
        navController.modalPresentationStyle = .pageSheet
        present(navController, animated: true)
    }
    
    private func addPulseAnimation(to view: UIView) {
        // 创建脉冲动画层
        let pulseLayer = CALayer()
        pulseLayer.frame = view.bounds
        pulseLayer.cornerRadius = view.layer.cornerRadius
        pulseLayer.backgroundColor = UIColor.systemBlue.cgColor
        pulseLayer.opacity = 0
        view.layer.insertSublayer(pulseLayer, at: 0)
        
        // 创建缩放动画
        let scaleAnimation = CABasicAnimation(keyPath: "transform.scale")
        scaleAnimation.fromValue = 1.05
        scaleAnimation.toValue = 1.5
        
        // 创建透明度动画
        let opacityAnimation = CABasicAnimation(keyPath: "opacity")
        opacityAnimation.fromValue = 0.3
        opacityAnimation.toValue = 0.0
        
        // 组合动画
        let animationGroup = CAAnimationGroup()
        animationGroup.animations = [scaleAnimation, opacityAnimation]
        animationGroup.duration = 2.0
        animationGroup.repeatCount = 2 // 只播放一次
        animationGroup.timingFunction = CAMediaTimingFunction(name: .easeOut)
        animationGroup.isRemovedOnCompletion = true
        animationGroup.fillMode = .forwards
        
        pulseLayer.add(animationGroup, forKey: "pulse")
    }
    
    @objc private func shareAppButtonAction() {
        shareAppAction(ads: false)
    }
    
    @objc private func shareAppAction(ads: Bool) {
        view.makeToastActivity(.center)
        // 创建分享内容
        let shareText = NSLocalizedString(HDAppsTool.shareText(), comment: "")
        let items: [Any] = [shareText, HDAppsTool.shareUrl()]
        
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
        present(activityViewController, animated: true) {
            self.view.hideToastActivity()
        }
        
        closeAds = ads
    }
    
    private func setupData() {
        // 版本信息
        if let dictionary = Bundle.main.infoDictionary {
            versionInfo = HDAppsTool.relationApps()
            let version = dictionary["CFBundleShortVersionString"] as? String ?? "未知"
            let build = dictionary["CFBundleVersion"] as? String ?? "未知"
            versionInfo.insert("\(version) (\(build))", at: 0)
        }
        
        // 开发者信息
        developers = [
            Developer(name: "Kevin", homepage: "https://gitee.com/kvinwang"),
            Developer(name: "BlackSky", homepage: "https://gitee.com/null_331_1413"),
            Developer(name: "天际边工作室", homepage: "http://www.skysidestudio.com"),
            Developer(name: "旭哥传奇", homepage: "https://b23.tv/jFjqLIB"),
            Developer(name: "Harry", homepage: "https://gitee.com/harrydeng")
        ]
        
        #if DEBUG
        // 在调试模式下添加测试选项
        developers.append(Developer(name: "🧪 重置升级提示", homepage: "test://reset-upgrade-alert"))
        #endif
    }
    
    private func showEngineChangeAlert(useNewEngine: Bool, completion: @escaping (Bool) -> Void) {
        let alert = UIAlertController(
            title: "切换引擎",
            message: "切换引擎会导致游戏重新开始，请记得提前存档！是否确认切换？",
            preferredStyle: .alert
        )
        
        alert.addAction(UIAlertAction(title: "取消", style: .cancel) { _ in
            completion(false)
        })
        
        alert.addAction(UIAlertAction(title: "确认", style: .default) { [weak self] _ in
            self?.useNewEngine = useNewEngine
            // 保存到HDAppData
            HDAppData.shared.useNewEngine = useNewEngine
            // 发送通知，将新的引擎设置传递给webview
            NotificationCenter.default.post(name: .engineSelectionChanged, object: nil, userInfo: ["useNewEngine": useNewEngine])
            completion(true)
            
            self?.view.makeBlockToastActivity()
            DispatchQueue.main.asyncAfter(deadline: .now() + 0.3) {
                self?.view.hideToastActivity()
                self?.dismiss(animated: true, completion: {
                    
                })
            }
        })
        
        present(alert, animated: true)
    }
    
    
    private func showSpecialItemIAPView(_ productId: String, title: String, message: String) {
        // 如果是全物品包，在消息中添加额外的提示
        let isAllGoodsProduct = productId.contains("allgoods")
        let finalMessage: String
        
        if isAllGoodsProduct {
            let currentModName = HDAppData.shared.choiceLib["value"] ?? "当前游戏"
            finalMessage = "\n⚠️ 重要提示：\n• 请确保已进入\(currentModName)游戏，并开始游戏"
        } else {
            finalMessage = message
        }
        
        let buyAction = UIAlertAction(title: NSLocalizedString("立即购买", comment: ""), style: .default) { _ in
            self.purchaseSpecialItem(productId)
        }
        let nextAction = UIAlertAction(title: NSLocalizedString("下次一定", comment: ""), style: .cancel) { _ in
            
        }
        let restoreAction = UIAlertAction(title: NSLocalizedString("恢复购买", comment: ""), style: .default) { _ in
            self.restoreSpecialItem(productId)
        }
        
        let alertController = UIAlertController(title: title, message: finalMessage, preferredStyle: .alert)
        alertController.addAction(buyAction)
        alertController.addAction(nextAction)
        alertController.addAction(restoreAction)
        self.present(alertController, animated: true, completion: nil)
    }
    
    
    private func purchaseSpecialItem(_ productId: String) {
        view.makeBlockToastActivity()
        IAPManager.shared.purchaseProductWithId(productId: productId) { error in
            self.view.hideBlockToastActivity()
            if error == nil {
                debugPrint("successful purchase: \(productId)")
                self.purchaseSpecialItemSuccess(productId)
            }
            else {
                debugPrint("something wrong.. \(error.debugDescription)")
                self.purchaseFaild()
            }
        }
    }
    
    private func restoreSpecialItem(_ productId: String) {
        view.makeBlockToastActivity()
        IAPManager.shared.restoreCompletedTransactions { error in
            self.view.hideBlockToastActivity()
            if error == nil {
                debugPrint("successful restore: \(productId)")
                self.purchaseSpecialItemSuccess(productId)
            }
            else {
                debugPrint("restore failed: \(error.debugDescription)")
                if let err = error as? NSError, err.code == -1 {
                    let message = NSLocalizedString("没有可恢复购买的商品，请选择立即解锁购买", comment: "")
                    self.purchaseFaild(message: message)
                    return
                }
                self.purchaseFaild()
            }
        }
    }
    
    private func purchaseSpecialItemSuccess(_ productId: String) {
        showConfettiView()
        
        // 刷新表格以更新UI
        tableView.reloadData()
        
        // VIP购买直接传递VIP产品ID给JavaScript，让JS端处理通用逻辑
        // 其他产品也直接传递原始产品ID
        grantItemsToGame(productId)
    }
    
    private func grantItemsToGame(_ productId: String) {
        // 显示加载提示
        view.makeToastActivity(.center)
        
        // 通知 HDWebViewController 执行 JS 代码发放物品，并等待回调
        NotificationCenter.default.addObserver(
            self,
            selector: #selector(handleItemGrantResult(_:)),
            name: .itemGrantResult,
            object: nil
        )
        
        NotificationCenter.default.post(name: .grantSpecialItems, object: nil, userInfo: ["productId": productId])
        
        // 设置超时处理
        DispatchQueue.main.asyncAfter(deadline: .now() + 5.0) { [weak self] in
            self?.view.hideToastActivity()
            NotificationCenter.default.removeObserver(self as Any, name: .itemGrantResult, object: nil)
        }
    }
    
    @objc private func handleItemGrantResult(_ notification: Notification) {
        guard let userInfo = notification.userInfo,
              let success = userInfo["success"] as? Bool,
              let message = userInfo["message"] as? String else {
            return
        }
        
        // 移除观察者
        NotificationCenter.default.removeObserver(self, name: .itemGrantResult, object: nil)
        
        // 隐藏加载提示
        view.hideToastActivity()
        
        // 显示结果提示
        var style = ToastStyle()
        style.messageAlignment = .center
        
        if success {
            view.makeToast(message, duration: 3, position: .center, style: style)
        } else {
            view.makeToast(message, duration: 3, position: .center, style: style)
        }
    }
    
    private func showAddToCurrentSaveAlert(productId: String, itemName: String) {
        let alert = UIAlertController(
            title: "添加到当前存档（请确认已经进入游戏中）",
            message: "您已经购买了\(itemName)，是否要添加到当前存档？",
            preferredStyle: .alert
        )

        alert.addAction(UIAlertAction(title: "取消", style: .cancel))
        alert.addAction(UIAlertAction(title: "添加", style: .default) { [weak self] _ in
            // 调用JS方法尝试添加物品
            self?.grantItemsToGame(productId)
        })

        present(alert, animated: true)
    }

    private func addAllItemsTripleToBayeGame() {
        // 显示加载提示
        view.makeToastActivity(.center)

        // 通知 HDWebViewController 执行 JS 代码进行全物品操作，并等待回调
        NotificationCenter.default.addObserver(
            self,
            selector: #selector(handleItemGrantResult(_:)),
            name: .itemGrantResult,
            object: nil
        )

        NotificationCenter.default.post(name: .addAllItemsTriple, object: nil)

        // 设置超时处理
        DispatchQueue.main.asyncAfter(deadline: .now() + 5.0) { [weak self] in
            self?.view.hideToastActivity()
            NotificationCenter.default.removeObserver(self as Any, name: .itemGrantResult, object: nil)
        }
    }
    
    @objc private func showAllGoodsInfoAlert(_ sender: UIButton) {
        let title = "永久VIP购买说明"
        let message = """
        一次性费用，解锁所有购买内容（<3折）：包含所有游戏（包含后续新增）的经验/金币加成、全物品X3
        
        💳 购买后物品会立即添加到您的游戏背包中
        💳 切换存档后，再次点击可继续添加到您的游戏背包中
        """
        
        let alert = UIAlertController(title: title, message: message, preferredStyle: .alert)
        
        // 添加查看物品列表按钮
        alert.addAction(UIAlertAction(title: "查看物品列表", style: .default) { [weak self] _ in
            self?.showItemListWebView()
        })
        present(alert, animated: true)
    }
    
    private func showItemListWebView() {
        let currentMod = HDAppData.shared.choiceLib["key"] ?? "FMJ"
        
        // 根据不同mod确定物品列表HTML文件名
        let htmlFileName: String
        switch currentMod {
        case "FMJ", "FMJWMB", "FMJFYJ", "FMJYMQZQ", "FMJSNLWQ", "FMJMVKXQ", "FMJHMAHQ":
            htmlFileName = "goods/FMJ_goods.html"
        default:
          htmlFileName = "goods/\(currentMod)_goods.html"
        }
        
        // 使用HDStrategyWebViewController加载物品列表页面
        let webViewController = HDStrategyWebViewController()
        webViewController.customPageName = htmlFileName
        webViewController.title = "物品列表"
        self.navigationController?.pushViewController(webViewController, animated: true)
    }
    
    private func showEnemyMagicGuideWebView() {
        let currentMod = HDAppData.shared.choiceLib["key"] ?? "FMJ"
        
        // 根据不同mod确定敌人魔法指南HTML文件名
        let htmlFileName: String
        switch currentMod {
        case "FMJ", "FMJWMB", "FMJFYJ", "FMJYMQZQ", "FMJSNLWQ", "FMJMVKXQ", "FMJHMAHQ":
            htmlFileName = "magics/FMJ_magic.html"
        default:
            htmlFileName = "magics/\(currentMod)_magic.html"
        }
        
        // 使用HDStrategyWebViewController加载敌人魔法指南页面
        let webViewController = HDStrategyWebViewController()
        webViewController.customPageName = htmlFileName
        webViewController.title = "敌人及魔法列表指南"
        self.navigationController?.pushViewController(webViewController, animated: true)
    }
}

extension HDAboutViewController: UITableViewDataSource {
    func numberOfSections(in tableView: UITableView) -> Int {
        return sections.count
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        switch section {
        case 0: return 3  // 宗旨文本 + 意见反馈 + QQ群
        case 1:
            if HDAppsTool.hdAppName() == .hdBayeApp {
                if HDAppData.isBayeOrigin() {
                    // 原版不支持 农业开发 + 商业开发 + 全物品
                    return 1
                }
                // 霸业游戏：为爱发电 + 农业开发 + 商业开发 + 全物品
                return 4
            }
            else if HDAppsTool.hdAppName() == .hdFmjApp {
                // 根据不同mod显示对应的内购项目数量
                let currentMod = HDAppData.shared.choiceLib["key"] ?? "FMJ"
                switch currentMod {
                case "FMJ", "FMJWMB":
                    return 5  // VIP + 五倍经验 + 五倍金币 + 三倍物品 + FMJ全物品
                case "XKX":
                    return 5  // VIP + 五倍经验 + 五倍金币 + 三倍物品 + XKX全物品包
                case "CBZZZSYZF":
                    return 5  // VIP + 五倍经验 + 五倍金币 + 三倍物品 + CBZZ全物品包
                case "JYQXZ":
                    return 5  // VIP + 五倍经验 + 五倍金币 + 三倍物品 + JYQXZ全物品包
                case "YZCQ2":
                    return 5  // VIP + 五倍经验 + 五倍金币 + 三倍物品 + YZCQ全物品包
                default:
                    return 4  // 其他mod显示VIP + 五倍经验 + 五倍金币 + 三倍物品（不显示全物品包）
                }
            }
            else {
                return 1
            }
        case 2: return versionInfo.count  // 版本信息
        case 3: return 2  // 离线包管理：当前版本 + 检查更新
        case 4:
            if HDAppsTool.hdAppName() == .hdFmjApp {
                return 8  // 引擎选择 + 横竖屏 + 遇敌概率 + 游戏速度 + 滤镜效果 + 存档列表 + 故事和攻略 + 敌人魔法指南
            }
            else {
                if HDAppData.isBayeOrigin() {
                    // HDBaye应用: 分辨率设置 + 横竖屏 + 滤镜效果 + 变速器 + 存档列表 + 故事和攻略
                    return 6
                }
                return 5
            }
        case 5: return developers.count  // 开发者信息
        case 6: return 1  // 隐私政策
        default: return 0
        }
    }
    
    func tableView(_ tableView: UITableView, titleForHeaderInSection section: Int) -> String? {
        if section == 1, HDAppsTool.hdAppName() == .hdFmjApp {
            if let libName = HDAppData.shared.choiceLib["value"] {
                return sections[section] + "(\(libName))"
            }
        }
        return sections[section]
    }
    
    func tableView(_ tableView: UITableView, heightForHeaderInSection section: Int) -> CGFloat {
        // 如果是三国霸业，会员服务功能只有关闭广告功能
        if section == 1, HDAppsTool.hdAppName() == .hdBayeApp {
            return 30
        }
        // 所有section都显示header
        return 30
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        // 为全物品选项使用支持subtitle的cell样式
        let isAllGoodsRow = (indexPath.section == 1 && indexPath.row == 2 && HDAppsTool.hdAppName() == .hdFmjApp)
        
        let cell: UITableViewCell
        if isAllGoodsRow {
            cell = UITableViewCell(style: .subtitle, reuseIdentifier: "SubtitleCell")
        } else {
            cell = tableView.dequeueReusableCell(withIdentifier: "Cell", for: indexPath)
        }
        
        // 重置cell状态，避免复用时的显示问题
        cell.selectionStyle = .none
        cell.textLabel?.font = UIFont.systemFont(ofSize: 16)
        cell.accessoryView = nil
        cell.accessoryType = .none
        
        // 管理小红点提示（避免cell复用时重复创建）
        let redDotTag = 9999
        // 移除已存在的红点视图
        if let existingRedDot = cell.contentView.viewWithTag(redDotTag) {
            existingRedDot.removeFromSuperview()
        }
        
        switch indexPath.section {
        case 0:
            if indexPath.row == 0 {
                cell.accessoryType = .none
                cell.textLabel?.numberOfLines = 0
                cell.textLabel?.text = "没有最好，只有更好。不求回报，只求快乐\n时间带不走我们的热情，时代改变不了我们的执着！"
            } else if indexPath.row == 1 {
                cell.accessoryType = .disclosureIndicator
                cell.textLabel?.numberOfLines = 1
                cell.textLabel?.text = "意见反馈"
                cell.selectionStyle = .default
            } else {
                cell.accessoryType = .disclosureIndicator
                cell.textLabel?.numberOfLines = 1
                cell.textLabel?.text = "加入QQ群 \(HDConstants.QQGroup.groupNumber)"
                cell.selectionStyle = .default
            }
        case 1: // 会员服务部分
            var text = ""
            if HDAppsTool.hdAppName() == .hdBayeApp {
                let isPurchased = IAPManager.shared.isProductPurchased(productId: HDAppsTool.bayeForLoveId())
                if indexPath.row == 0 {
                    // VIP 永久会员
                    if isPurchased {
                        text = NSLocalizedString("您已成为发电用户", comment: "")
                        cell.accessoryType = .none
                    }
                    else {
                        text = "为爱发电，并解锁如下功能"
                        cell.accessoryType = .disclosureIndicator
                    }
                    cell.textLabel?.text = text
                    return cell
                }
                else if indexPath.row == 1 {
                    // 农业开发倍率
                    let multiplierCell = tableView.dequeueReusableCell(withIdentifier: AgricultureMultiplierCell.identifier, for: indexPath) as! AgricultureMultiplierCell
                    multiplierCell.configure(with: HDAppData.shared.agricultureMultiplier, isPurchased: isPurchased)

                    if isPurchased {
                        multiplierCell.multiplierValueChanged = { multiplier in
                            HDAppData.shared.agricultureMultiplier = multiplier
                            NotificationCenter.default.post(name: .agricultureMultiplierChanged, object: nil, userInfo: ["multiplier": multiplier])
                        }
                    } else {
                        multiplierCell.onTapWhenNotPurchased = { [weak self] in
                          self?.showSpecialItemIAPView(HDAppsTool.bayeForLoveId(), title: HDAppsTool.bayeForLoveTitle(), message: HDAppsTool.bayeForLoveMessage())
                        }
                    }
                    return multiplierCell
                }
                else if indexPath.row == 2 {
                    // 商业开发倍率
                    let multiplierCell = tableView.dequeueReusableCell(withIdentifier: CommerceMultiplierCell.identifier, for: indexPath) as! CommerceMultiplierCell
                    multiplierCell.configure(with: HDAppData.shared.commerceMultiplier, isPurchased: isPurchased)

                    if isPurchased {
                        multiplierCell.multiplierValueChanged = { multiplier in
                            HDAppData.shared.commerceMultiplier = multiplier
                            NotificationCenter.default.post(name: .commerceMultiplierChanged, object: nil, userInfo: ["multiplier": multiplier])
                        }
                    } else {
                        multiplierCell.onTapWhenNotPurchased = { [weak self] in
                            self?.showSpecialItemIAPView(HDAppsTool.bayeForLoveId(), title: HDAppsTool.bayeForLoveTitle(), message: HDAppsTool.bayeForLoveMessage())
                        }
                    }
                    return multiplierCell
                }
                else if indexPath.row == 3 {
                    // 全物品
                    let itemCell = tableView.dequeueReusableCell(withIdentifier: AllItemsTripleCell.identifier, for: indexPath) as! AllItemsTripleCell
                    itemCell.configure(isPurchased: isPurchased)

                    itemCell.onPurchaseTapped = { [weak self] in
                        self?.showSpecialItemIAPView(HDAppsTool.bayeForLoveId(), title: HDAppsTool.bayeForLoveTitle(), message: HDAppsTool.bayeForLoveMessage())
                    }

                    itemCell.onAddToSaveTapped = { [weak self] in
                        self?.addAllItemsTripleToBayeGame()
                    }

                    return itemCell
                }
            }
            
            // 根据不同mod显示对应的内购项目
            let currentMod = HDAppData.shared.choiceLib["key"] ?? "FMJ"
            
            if indexPath.row == 0 {
                // VIP 永久会员 - 第一个位置
                if IAPManager.shared.isProductPurchased(productId: HDAppsTool.allGoodsId_VIP()) {
                    text = NSLocalizedString("您已成为永久VIP会员", comment: "")
                    cell.accessoryType = .none
                }
                else {
                    text = NSLocalizedString("升级永久VIP，解锁所有购买", comment: "")
                    cell.accessoryType = .disclosureIndicator
                }
                
                // 为VIP选项创建或重用info按钮（避免cell复用时重复创建）
                let containerView: UIView
                if let existingAccessoryView = cell.accessoryView {
                    containerView = existingAccessoryView
                    // 清理现有按钮的target
                    if let existingButton = containerView.subviews.first as? UIButton {
                        existingButton.removeTarget(nil, action: nil, for: .allEvents)
                        existingButton.addTarget(self, action: #selector(showAllGoodsInfoAlert(_:)), for: .touchUpInside)
                        existingButton.tag = indexPath.row
                    }
                } else {
                    // 创建新的容器视图和按钮
                    containerView = UIView()
                    containerView.frame = CGRect(x: 0, y: 0, width: 50, height: 30)
                    
                    let infoButton = UIButton(type: .infoLight)
                    infoButton.tintColor = .label
                    infoButton.addTarget(self, action: #selector(showAllGoodsInfoAlert(_:)), for: .touchUpInside)
                    infoButton.tag = indexPath.row
                    infoButton.frame = CGRect(x: 26, y: 3, width: 24, height: 24)
                    containerView.addSubview(infoButton)
                }
                
                cell.accessoryView = containerView
                cell.accessoryType = .none
            }
            else if indexPath.row == 1 {
                let isPurchased = IAPManager.shared.isProductPurchased(productId: HDAppsTool.doubleExpId())

                // 始终显示Segment控制
                let multiplierCell = tableView.dequeueReusableCell(withIdentifier: ExpMultiplierCell.identifier, for: indexPath) as! ExpMultiplierCell
                multiplierCell.configure(with: HDAppData.shared.expMultiplier, isPurchased: isPurchased)

                if isPurchased {
                    // 已购买，正常的值变更回调
                    multiplierCell.multiplierValueChanged = { multiplier in
                        HDAppData.shared.expMultiplier = multiplier
                        // 发送通知通知游戏更新经验倍率
                        NotificationCenter.default.post(name: .expMultiplierChanged, object: nil, userInfo: ["multiplier": multiplier])
                    }
                } else {
                    // 未购买，点击时引导购买
                    multiplierCell.onTapWhenNotPurchased = { [weak self] in
                        self?.showIAPView(HDAppsTool.doubleExpId())
                    }
                }
                return multiplierCell
            }
            else if indexPath.row == 2 {
                let isPurchased = IAPManager.shared.isProductPurchased(productId: HDAppsTool.doubleGoldId())

                // 始终显示Segment控制
                let multiplierCell = tableView.dequeueReusableCell(withIdentifier: GoldMultiplierCell.identifier, for: indexPath) as! GoldMultiplierCell
                multiplierCell.configure(with: HDAppData.shared.goldMultiplier, isPurchased: isPurchased)

                if isPurchased {
                    // 已购买，正常的值变更回调
                    multiplierCell.multiplierValueChanged = { multiplier in
                        HDAppData.shared.goldMultiplier = multiplier
                        // 发送通知通知游戏更新金币倍率和小地图状态
                        NotificationCenter.default.post(name: .goldMultiplierChanged, object: nil, userInfo: ["multiplier": multiplier])
                    }
                } else {
                    // 未购买，点击时引导购买
                    multiplierCell.onTapWhenNotPurchased = { [weak self] in
                        self?.showIAPView(HDAppsTool.doubleGoldId())
                    }
                }
                return multiplierCell
            }
            else if indexPath.row == 3 {
                let isPurchased = IAPManager.shared.isProductPurchased(productId: HDAppsTool.allGoodsId_VIP())

                // 始终显示Segment控制
                let multiplierCell = tableView.dequeueReusableCell(withIdentifier: ItemMultiplierCell.identifier, for: indexPath) as! ItemMultiplierCell
                multiplierCell.configure(with: HDAppData.shared.itemMultiplier, isPurchased: isPurchased)

                if isPurchased {
                    // 已购买VIP，正常的值变更回调
                    multiplierCell.multiplierValueChanged = { multiplier in
                        HDAppData.shared.itemMultiplier = multiplier
                        // 发送通知通知游戏更新物品倍率
                        NotificationCenter.default.post(name: .itemMultiplierChanged, object: nil, userInfo: ["multiplier": multiplier])
                    }
                } else {
                    // 未购买VIP，点击时引导购买VIP
                    multiplierCell.onTapWhenNotPurchased = { [weak self] in
                        self?.showSpecialItemIAPView(HDAppsTool.allGoodsId_VIP(), title: "购买VIP会员", message: HDAppsTool.allGoodsMessage_VIP())
                    }
                }
                return multiplierCell
            }
            else if indexPath.row == 4 {
                // 第四个选项统一为"购买全部物品"
                switch currentMod {
                case "FMJ", "FMJWMB":
                    // FMJ系列：全物品包
                    if IAPManager.shared.isProductPurchased(productId: HDAppsTool.allGoodsId()) {
                        text = NSLocalizedString("您已获得全部物品x3", comment: "")
                        cell.accessoryType = .none
                    }
                    else {
                        text = NSLocalizedString("购买全部物品x3", comment: "")
                        cell.accessoryType = .disclosureIndicator
                    }
                case "XKX":
                    // 侠客行：全物品包
                    if IAPManager.shared.isProductPurchased(productId: HDAppsTool.allGoodsId_XKX()) {
                        text = NSLocalizedString("您已获得全部物品x3", comment: "")
                        cell.accessoryType = .none
                    }
                    else {
                        text = NSLocalizedString("购买全部物品x3", comment: "")
                        cell.accessoryType = .disclosureIndicator
                    }
                case "CBZZZSYZF":
                    // 赤壁之战系列：全物品包
                    if IAPManager.shared.isProductPurchased(productId: HDAppsTool.allGoodsId_CBZZ()) {
                        text = NSLocalizedString("您已获得全部物品x3", comment: "")
                        cell.accessoryType = .none
                    }
                    else {
                        text = NSLocalizedString("购买全部物品x3", comment: "")
                        cell.accessoryType = .disclosureIndicator
                    }
                case "JYQXZ":
                    // 金庸群侠传：全物品包
                    if IAPManager.shared.isProductPurchased(productId: HDAppsTool.allGoodsId_JYQXZ()) {
                        text = NSLocalizedString("您已获得全部物品x3", comment: "")
                        cell.accessoryType = .none
                    }
                    else {
                        text = NSLocalizedString("购买全部物品x3", comment: "")
                        cell.accessoryType = .disclosureIndicator
                    }
                case "YZCQ2":
                    // 一中传奇系列：全物品包
                    if IAPManager.shared.isProductPurchased(productId: HDAppsTool.allGoodsId_YZCQ()) {
                        text = NSLocalizedString("您已获得全部物品x3", comment: "")
                        cell.accessoryType = .none
                    }
                    else {
                        text = NSLocalizedString("购买全部物品x3", comment: "")
                        cell.accessoryType = .disclosureIndicator
                    }
                default:
                    // 其他未知mod：不显示全物品包选项
                    break
                }
            }
            cell.textLabel?.text = text
        case 2:
            cell.accessoryType = .disclosureIndicator
            let text = versionInfo[indexPath.row]
            cell.textLabel?.text = text
        case 3: // 离线包管理
            if indexPath.row == 0 {
                // 显示当前离线包版本
                cell.accessoryType = .none
                cell.textLabel?.text = "当前版本"
                cell.detailTextLabel?.text = HDOfflinePackageUpdateManager.shared.getCurrentVersion()
                cell.selectionStyle = .none
                
                // 设置为带详情的cell样式
                if cell.detailTextLabel == nil {
                    // 如果当前cell没有detailTextLabel，重新创建一个有subtitle样式的cell
                    let detailCell = UITableViewCell(style: .value1, reuseIdentifier: "DetailCell")
                    detailCell.textLabel?.text = "当前版本"
                    detailCell.detailTextLabel?.text = HDOfflinePackageUpdateManager.shared.getCurrentVersion()
                    detailCell.accessoryType = .none
                    detailCell.selectionStyle = .none
                    return detailCell
                }
            } else {
                // 检查离线包更新
                cell.accessoryType = .disclosureIndicator
                cell.textLabel?.text = "检查离线包更新"
                cell.selectionStyle = .default
            }
        case 4:
            if HDAppsTool.hdAppName() == .hdBayeApp {
                var indexPathRow = indexPath.row
                if HDAppData.isBayeOrigin() {
                    if indexPathRow == 0 {
                        // 分辨率设置（新增到第一位）
                        cell.accessoryView = nil
                        cell.textLabel?.text = "游戏分辨率"
                        cell.selectionStyle = .default
                        
                        // 创建分段控制器
                        let resolutionSegment = UISegmentedControl(items: ["词典", "高清"])
                        resolutionSegment.selectedSegmentIndex = HDAppData.shared.bayeResolution == "0" ? 0 : 1
                        resolutionSegment.addTarget(self, action: #selector(resolutionChanged(_:)), for: .valueChanged)
                        cell.accessoryView = resolutionSegment
                        cell.selectionStyle = .none
                        
                        return cell
                    }
                    indexPathRow = indexPathRow - 1
                }
                
                // HDBaye应用的页面设置
                if indexPathRow == 0 {
                    // 滤镜效果
                    let cell = tableView.dequeueReusableCell(withIdentifier: ColorFilterCell.identifier, for: indexPath) as! ColorFilterCell
                    cell.configure(with: selectedFilter)
                    cell.filterValueChanged = { [weak self] filter in
                        self?.selectedFilter = filter
                        HDAppData.shared.selectedFilter = filter
                        NotificationCenter.default.post(name: .colorFilterChanged, object: nil, userInfo: ["filter": filter])
                    }
                    return cell
                }
                else if indexPathRow == 1 {
                    // 横竖屏切换
                    let cell = tableView.dequeueReusableCell(withIdentifier: PortraitModeCell.identifier, for: indexPath) as! PortraitModeCell
                    cell.configure(with: orientationMode)
                    cell.orientationValueChanged = { [weak self] newMode in
                        self?.orientationMode = newMode
                        print("横竖屏模式切换: \(newMode.displayTitle)")
                        // 发送通知给 WebView
                        NotificationCenter.default.post(name: .bayeOrientationChanged, object: nil, userInfo: ["orientation": newMode.rawValue])
                    }
                    return cell
                }
                else if indexPathRow == 2 {
                    // 变速器
                    let cell = tableView.dequeueReusableCell(withIdentifier: GameSpeedCell.identifier, for: indexPath) as! GameSpeedCell
                    cell.configure(with: gameSpeed)
                    cell.speedValueChanged = { [weak self] speed in
                        self?.gameSpeed = speed
                        HDAppData.shared.gameSpeed = speed
                        NotificationCenter.default.post(name: .setGameSpeedMultiple, object: nil, userInfo: ["speed": speed])
                    }
                    return cell
                }
                else if indexPathRow == 3 {
                    // 存档列表
                    cell.accessoryType = .disclosureIndicator
                    cell.textLabel?.text = "存档列表"
                    return cell
                }
                else {
                    // 故事和攻略
                    cell.accessoryType = .disclosureIndicator
                    cell.textLabel?.text = NSLocalizedString("故事和攻略", comment: "")
                    return cell
                }
            } else {
                // FMJ应用的设置
                if indexPath.row == 0 {
                    let cell = tableView.dequeueReusableCell(withIdentifier: EngineSelectionCell.identifier, for: indexPath) as! EngineSelectionCell
                    cell.configure(with: useNewEngine)
                    cell.engineValueChanged = { [weak self] useNewEngine, completion in
                        self?.showEngineChangeAlert(useNewEngine: useNewEngine, completion: completion)
                    }
                    return cell
                }
                else if indexPath.row == 1 {
                    // 横竖屏切换
                    let cell = tableView.dequeueReusableCell(withIdentifier: PortraitModeCell.identifier, for: indexPath) as! PortraitModeCell
                    cell.configure(with: orientationMode)
                    cell.orientationValueChanged = { [weak self] newMode in
                        self?.orientationMode = newMode
                        print("横竖屏模式切换: \(newMode.displayTitle)")
                        // 发送通知给 WebView
                        NotificationCenter.default.post(name: .bayeOrientationChanged, object: nil, userInfo: ["orientation": newMode.rawValue])
                    }
                    return cell
                }
                else if indexPath.row == 2 {
                    let cell = tableView.dequeueReusableCell(withIdentifier: CombatProbabilityCell.identifier, for: indexPath) as! CombatProbabilityCell
                    cell.configure(with: combatProbability)
                    cell.probabilityValueChanged = { [weak self] probability in
                        self?.combatProbability = probability
                        // 保存到HDAppData
                        HDAppData.shared.combatProbability = probability
                        // 发送通知，将新的遇敌概率传递给webview
                        NotificationCenter.default.post(name: .combatProbabilityChanged, object: nil, userInfo: ["probability": probability])
                    }
                    return cell
                }
                else if indexPath.row == 3 {
                    let cell = tableView.dequeueReusableCell(withIdentifier: GameSpeedCell.identifier, for: indexPath) as! GameSpeedCell
                    cell.configure(with: gameSpeed)
                    cell.speedValueChanged = { [weak self] speed in
                        self?.gameSpeed = speed
                        // 保存到HDAppData
                        HDAppData.shared.gameSpeed = speed
                        // 发送通知，将新的游戏速度传递给webview
                        NotificationCenter.default.post(name: .setGameSpeedMultiple, object: nil, userInfo: ["speed": speed])
                    }
                    return cell
                }
                else if indexPath.row == 4 {
                    let cell = tableView.dequeueReusableCell(withIdentifier: ColorFilterCell.identifier, for: indexPath) as! ColorFilterCell
                    cell.configure(with: selectedFilter)
                    cell.filterValueChanged = { [weak self] filter in
                        self?.selectedFilter = filter
                        // 保存到HDAppData
                        HDAppData.shared.selectedFilter = filter
                        // 发送通知，将新的滤镜模式传递给webview
                        NotificationCenter.default.post(name: .colorFilterChanged, object: nil, userInfo: ["filter": filter])
                    }
                    return cell
                }
                else if indexPath.row == 5 {
                    cell.accessoryType = .disclosureIndicator
                    cell.textLabel?.text = "存档列表"
                    return cell
                }
                else if indexPath.row == 6 {
                    cell.accessoryType = .disclosureIndicator
                    cell.textLabel?.text = NSLocalizedString("故事和攻略", comment: "")
                    return cell
                }
                else {
                    cell.accessoryType = .disclosureIndicator
                    cell.textLabel?.text = "敌人及魔法列表指南"
                    
                    // 如果未查看过，添加小红点提示
                    if !HDAppData.shared.hasViewedEnemyMagicGuide {
                        let redDotView = UIView()
                        redDotView.tag = redDotTag
                        redDotView.backgroundColor = .red
                        redDotView.layer.cornerRadius = 4
                        redDotView.translatesAutoresizingMaskIntoConstraints = false
                        cell.contentView.addSubview(redDotView)
                        
                        // 使用AutoLayout确保红点位置正确
                        NSLayoutConstraint.activate([
                            redDotView.trailingAnchor.constraint(equalTo: cell.contentView.trailingAnchor, constant: -10),
                            redDotView.centerYAnchor.constraint(equalTo: cell.contentView.centerYAnchor),
                            redDotView.widthAnchor.constraint(equalToConstant: 8),
                            redDotView.heightAnchor.constraint(equalToConstant: 8)
                        ])
                    }
                    
                    return cell
                }
            }
        case 5:
            cell.accessoryType = .disclosureIndicator
            let developer = developers[indexPath.row]
            cell.textLabel?.text = "\(developer.name)"
        case 6:
            cell.accessoryType = .disclosureIndicator
            cell.textLabel?.text = "隐私政策"
        default:
            cell.textLabel?.text = ""
        }
        
        return cell
    }
}

extension HDAboutViewController: UITableViewDelegate {
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        tableView.deselectRow(at: indexPath, animated: true)
        
        switch indexPath.section {
        case 0:
            if indexPath.row == 1 {
                // 处理意见反馈点击
                let feedbackVC = HDFeedbackViewController()
                let navController = UINavigationController(rootViewController: feedbackVC)
                navController.modalPresentationStyle = .fullScreen
                present(navController, animated: true)
            } else if indexPath.row == 2 {
                // 处理QQ群点击
                HDAppsTool.openURL(HDConstants.QQGroup.urlScheme) { success in
                    if !success {
                        // 如果QQ没有安装，将群号复制到粘贴板并提示用户
                        DispatchQueue.main.async {
                            UIPasteboard.general.string = HDConstants.QQGroup.groupNumber
                            let alert = UIAlertController(
                                title: "加入QQ群",
                                message: "群号 \(HDConstants.QQGroup.groupNumber) 已复制到粘贴板\n请手动打开QQ并搜索群号加入",
                                preferredStyle: .alert
                            )
                            alert.addAction(UIAlertAction(title: "确定", style: .default))
                            self.present(alert, animated: true)
                        }
                    }
                }
            }
        case 1:
            if HDAppsTool.hdAppName() == .hdBayeApp {
                if indexPath.row == 0 {
                  // 为爱发电内购产品点击处理
                  if !IAPManager.shared.isProductPurchased(productId: HDAppsTool.bayeForLoveId()) {
                      showSpecialItemIAPView(HDAppsTool.bayeForLoveId(), title: HDAppsTool.bayeForLoveTitle(), message: HDAppsTool.bayeForLoveMessage())
                  }
                  else {
                      // 已购买时显示感谢信息
                      var style = ToastStyle()
                      style.messageAlignment = .center
                      view.makeToast(HDAppsTool.bayeForLoveMessage(), duration: 3, position: .center, style: style)
                  }
                }
                else if indexPath.row == 1 || indexPath.row == 2 || indexPath.row == 3 {
                    // 农业、商业倍率和全物品行，点击由Cell内部处理，这里不需要做任何事
                    return
                }
                return
            }
            
            if indexPath.row == 0 {
                // VIP 永久会员购买
                if !IAPManager.shared.isProductPurchased(productId: HDAppsTool.allGoodsId_VIP()) {
                    showSpecialItemIAPView(HDAppsTool.allGoodsId_VIP(), title: "购买VIP会员", message: HDAppsTool.allGoodsMessage_VIP())
                }
                else {
                    // VIP用户可以重新添加物品到当前存档
                    showAddToCurrentSaveAlert(productId: HDAppsTool.allGoodsId_VIP(), itemName: "全部物品x3")
                }
            }
            else if indexPath.row == 1 || indexPath.row == 2 || indexPath.row == 3 {
                // 经验、金币、物品倍率行，点击由Cell内部的遮罩层处理，这里不需要做任何事
                return
            }
            // 根据不同mod处理内购点击
            let currentMod = HDAppData.shared.choiceLib["key"] ?? "FMJ"

            if indexPath.row == 4 {
                // 第四个选项统一为"购买全部物品"
                switch currentMod {
                case "FMJ", "FMJWMB":
                    // FMJ系列：全物品包
                    if !IAPManager.shared.isProductPurchased(productId: HDAppsTool.allGoodsId()) {
                        showSpecialItemIAPView(HDAppsTool.allGoodsId(), title: "伏魔记系列-全部物品x3", message: HDAppsTool.allGoodsMessage())
                    }
                    else {
                        showAddToCurrentSaveAlert(productId: HDAppsTool.allGoodsId(), itemName: "伏魔记系列-全部物品x3")
                    }
                case "XKX":
                    // 侠客行：全物品包
                    if !IAPManager.shared.isProductPurchased(productId: HDAppsTool.allGoodsId_XKX()) {
                        showSpecialItemIAPView(HDAppsTool.allGoodsId_XKX(), title: "侠客行系列-全部物品x3", message: HDAppsTool.allGoodsMessage_XKX())
                    }
                    else {
                        showAddToCurrentSaveAlert(productId: HDAppsTool.allGoodsId_XKX(), itemName: "侠客行系列-全部物品x3")
                    }
                case "CBZZZSYZF":
                    // 赤壁之战系列：全物品包
                    if !IAPManager.shared.isProductPurchased(productId: HDAppsTool.allGoodsId_CBZZ()) {
                        showSpecialItemIAPView(HDAppsTool.allGoodsId_CBZZ(), title: "赤壁之战系列-全部物品x3", message: HDAppsTool.allGoodsMessage_CBZZ())
                    }
                    else {
                        showAddToCurrentSaveAlert(productId: HDAppsTool.allGoodsId_CBZZ(), itemName: "赤壁之战系列-全部物品x3")
                    }
                case "JYQXZ":
                    // 金庸群侠传：全物品包
                    if !IAPManager.shared.isProductPurchased(productId: HDAppsTool.allGoodsId_JYQXZ()) {
                        showSpecialItemIAPView(HDAppsTool.allGoodsId_JYQXZ(), title: "金庸群侠传系列-全部物品x3", message: HDAppsTool.allGoodsMessage_JYQXZ())
                    }
                    else {
                        showAddToCurrentSaveAlert(productId: HDAppsTool.allGoodsId_JYQXZ(), itemName: "金庸群侠传系列-全部物品x3")
                    }
                case "YZCQ2":
                    // 一中传奇系列：全物品包
                    if !IAPManager.shared.isProductPurchased(productId: HDAppsTool.allGoodsId_YZCQ()) {
                        showSpecialItemIAPView(HDAppsTool.allGoodsId_YZCQ(), title: "一中传奇系列-全部物品x3", message: HDAppsTool.allGoodsMessage_YZCQ())
                    }
                    else {
                        showAddToCurrentSaveAlert(productId: HDAppsTool.allGoodsId_YZCQ(), itemName: "一中传奇系列-全部物品x3")
                    }
                default:
                    debugPrint("")
                }
            }
        case 2:
            if indexPath.row == 0 {
                // 处理关于App的点击
                HDAppsTool.openURL(HDAppsTool.appStoreUrl())
            }
            else {
                let scheme = HDAppsTool.relationAppsScheme()[indexPath.row - 1]
                let appStoreURL = HDAppsTool.relationAppsLink()[indexPath.row - 1]
                HDAppsTool.openAppOrStore(scheme: scheme, storeURL: appStoreURL)
            }
        case 3: // 离线包管理
            if indexPath.row == 1 {
                // 仅第二行（检查离线包更新）可点击
                HDOfflinePackageUpdateManager.shared.checkAndPromptForUpdate(
                    from: self,
                    onStatusChange: { message in
                        DispatchQueue.main.async {
                            self.view.makeToast(message, duration: 2.0, position: .center)
                        }
                    },
                    onProgress: { progress in
                        HDAppsTool.debugLog("更新进度: \(Int(progress * 100))%")
                    }
                ) { success in
                    DispatchQueue.main.async {
                        if success {
                            // 发送离线包更新完成通知，让WebView重新加载
                            NotificationCenter.default.post(name: .offlinePackageUpdated, object: nil)
                            self.view.makeToast("更新完成，即将后自动返回游戏...", duration: 2.0, position: .center)
                            // 3秒后自动关闭弹窗并返回WebView页面
                            DispatchQueue.main.asyncAfter(deadline: .now() + 3.0) {
                                self.dismiss(animated: true) {
                                    
                                }
                            }
                            // 更新完成后刷新版本显示
                            self.tableView.reloadRows(at: [IndexPath(row: 0, section: 3)], with: .none)
                        }
                    }
                }
            }
            // 第一行（当前版本）不做任何处理
        case 4:
            // 处理页面设置
            if HDAppsTool.hdAppName() == .hdBayeApp {
                var indexPathRow = indexPath.row
                // 原版的会多一个引擎切换功能，需要减1
                if HDAppData.isBayeOrigin() {
                    indexPathRow = indexPathRow - 1
                }
                // HDBaye应用
                if indexPathRow == 3 {
                    let saveListVC = HDSaveListViewController()
                    self.navigationController?.pushViewController(saveListVC, animated: true)
                } else if indexPathRow == 4 {
                    let controller = HDStrategyWebViewController()
                    // 如果有动态获取的攻略链接，则使用自定义URL
                    if HDAppData.shared.currentGameHomeURL.isEmpty {
                        HDAppData.shared.currentGameHomeURL = "http://harrydeng2025.xyz/baye_index/baye_data_index.html"
                    }
                    controller.customURL = HDAppData.shared.currentGameHomeURL
                    
                    navigationController?.pushViewController(controller, animated: true)
                }
            } else {
                // FMJ应用
                if indexPath.row == 5 {
                    // 存档列表
                    let saveListVC = HDSaveListViewController()
                    self.navigationController?.pushViewController(saveListVC, animated: true)
                } else if indexPath.row == 6 {
                    // 故事和攻略
                    let controller = HDStrategyWebViewController()
                    controller.title = "故事和攻略"
                    navigationController?.pushViewController(controller, animated: true)
                } else if indexPath.row == 7 {
                    // 敌人魔法指南
                    // 标记为已查看
                    HDAppData.shared.hasViewedEnemyMagicGuide = true
                    showEnemyMagicGuideWebView()
                    
                    // 刷新当前row以移除小红点
                    DispatchQueue.main.async {
                        self.tableView.reloadRows(at: [indexPath], with: .none)
                    }
                }
            }
        case 5:
            // 处理开发者信息区域的点击
            let developer = developers[indexPath.row]
            
            #if DEBUG
            // 处理测试重置升级提示
            if developer.homepage == "test://reset-upgrade-alert" {
                // 清除版本记录，使升级提示可以重新显示
                HDAppData.shared.markUpgradeAlertShown(for: "")
                var style = ToastStyle()
                style.messageAlignment = .center
                view.makeToast("升级提示状态已重置，重启应用后生效", duration: 2, position: .center, style: style)
                return
            }
            #endif
            
            HDAppsTool.openURL(developer.homepage)
        case 6:
            // 处理隐私政策点击
            HDAppsTool.openURL(HDConstants.URLs.privacyPolicy)
        default:
            break
        }
    }
    
    
    // 处理分辨率切换
    @objc private func resolutionChanged(_ sender: UISegmentedControl) {
        let newResolution = sender.selectedSegmentIndex == 0 ? "0" : "1"  // 0: 词典分辨率(160x96), 1: 高清分辨率(208x128)
        let currentResolution = HDAppData.shared.bayeResolution
        
        // 如果没有改变，直接返回
        if newResolution == currentResolution {
            return
        }
        
        // 如果切换到高清分辨率，需要提示用户
        if newResolution == "1" {
            let alert = UIAlertController(
                title: "切换到高清分辨率",
                message: "⚠️ 重要提示：\n\n• 切换分辨率会影响游戏画面显示\n• 建议先保存当前游戏进度\n• 切换后需要重新加载游戏\n\n确认切换到高清分辨率吗？",
                preferredStyle: .alert
            )
            
            let confirmAction = UIAlertAction(title: "确认切换", style: .default) { [weak self] _ in
                self?.performResolutionChange(newResolution)
            }
            
            let cancelAction = UIAlertAction(title: "取消", style: .cancel) { _ in
                // 恢复到原来的选择
                sender.selectedSegmentIndex = currentResolution == "0" ? 0 : 1
            }
            
            alert.addAction(confirmAction)
            alert.addAction(cancelAction)
            present(alert, animated: true)
        } else {
            // 从高清切换回词典分辨率，也给出提示但相对简单
            let alert = UIAlertController(
                title: "切换到词典分辨率",
                message: "将切换回经典的词典分辨率(160×96)，游戏将重新加载。",
                preferredStyle: .alert
            )
            
            let confirmAction = UIAlertAction(title: "确认", style: .default) { [weak self] _ in
                self?.performResolutionChange(newResolution)
            }
            
            let cancelAction = UIAlertAction(title: "取消", style: .cancel) { _ in
                // 恢复到原来的选择
                sender.selectedSegmentIndex = currentResolution == "0" ? 0 : 1
            }
            
            alert.addAction(confirmAction)
            alert.addAction(cancelAction)
            present(alert, animated: true)
        }
    }
    
    // 执行分辨率切换
    private func performResolutionChange(_ resolution: String) {
        // 保存新的分辨率设置
        HDAppData.shared.bayeResolution = resolution
        
        // 显示加载提示
        let loadingAlert = UIAlertController(
            title: "正在切换分辨率",
            message: "请稍候...",
            preferredStyle: .alert
        )
        present(loadingAlert, animated: true)
        
        // 发送通知给WebView
        NotificationCenter.default.post(
            name: .bayeResolutionChanged,
            object: nil,
            userInfo: ["resolution": resolution, "needsReload": true]
        )
        
        // 延迟关闭加载提示
        DispatchQueue.main.asyncAfter(deadline: .now() + 1.5) {
            loadingAlert.dismiss(animated: true) {
                // 显示完成提示
                let completedAlert = UIAlertController(
                    title: "分辨率切换完成",
                    message: resolution == "1" ? "已切换到高清分辨率(208×128)" : "已切换到词典分辨率(160×96)",
                    preferredStyle: .alert
                )
                let sureAction = UIAlertAction(title: "确定", style: .default) { _ in
                    self.dismiss(animated: true) {
                        
                    }
                }
                completedAlert.addAction(sureAction)
                self.present(completedAlert, animated: true)
            }
        }
    }
}
