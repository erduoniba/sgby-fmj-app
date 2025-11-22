//
//  HDSaveListViewController.swift
//  HDBayeApp
//
//  Created by harrydeng on 2025/7/27.
//

import UIKit

struct GameSection {
    let gameKey: String
    let gameName: String
    var saveDataList: [SaveData]
}

class HDSaveListViewController: HDBaseViewController {
    private let tableView = UITableView(frame: .zero, style: .insetGrouped)
    private var gameSections: [GameSection] = []
    
    // 动态获取游戏映射关系
    private var gameMapping: [String: String] {
        let dynamicMapping = HDAppData.shared.libsList
        return dynamicMapping
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        setupUI()
        loadSaveData()
        
        #if DEBUG
        // 监听存档更新通知
        NotificationCenter.default.addObserver(
            self,
            selector: #selector(handleSaveDataUpdated(_:)),
            name: NSNotification.Name("SaveDataUpdated"),
            object: nil
        )
        #endif
    }
    
    deinit {
        NotificationCenter.default.removeObserver(self)
    }
    
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        // 每次页面出现时刷新数据，确保从详情页返回后能看到最新的数据
        loadSaveData()
    }
    
    private func setupUI() {
        title = "存档列表"
        view.backgroundColor = .systemGroupedBackground
        
        // 设置导航栏
        let backButton = UIBarButtonItem(
            image: UIImage(systemName: "chevron.left"),
            style: .plain,
            target: self,
            action: #selector(backButtonTapped)
        )
        backButton.tintColor = .label
        navigationItem.leftBarButtonItem = backButton
        
        // 启用滑动返回手势
        navigationController?.interactivePopGestureRecognizer?.isEnabled = true
        navigationController?.interactivePopGestureRecognizer?.delegate = self
        
        // 设置tableView
        tableView.delegate = self
        tableView.dataSource = self
        tableView.register(SaveDataCell.self, forCellReuseIdentifier: SaveDataCell.identifier)
        tableView.register(UITableViewCell.self, forCellReuseIdentifier: "EmptyCell")
        
        view.addSubview(tableView)
        tableView.translatesAutoresizingMaskIntoConstraints = false
        NSLayoutConstraint.activate([
            tableView.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor),
            tableView.leadingAnchor.constraint(equalTo: view.leadingAnchor),
            tableView.trailingAnchor.constraint(equalTo: view.trailingAnchor),
            tableView.bottomAnchor.constraint(equalTo: view.bottomAnchor)
        ])
    }
    
    @objc private func backButtonTapped() {
        navigationController?.popViewController(animated: true)
    }
    
    #if DEBUG
    @objc private func handleSaveDataUpdated(_ notification: Notification) {
        // 存档更新后重新加载数据
        loadSaveData()
    }
    #endif
    
    private func loadSaveData() {
        gameSections.removeAll()
        
        // 收集所有存档数据
        var allSaveData: [SaveData] = []
        
        if HDAppsTool.hdAppName() == .hdBayeApp {
            allSaveData = loadBayeSaveData()
        } else {
            allSaveData = loadFMJSaveData()
        }
        
        // 按游戏分组
        groupSaveDataByGame(allSaveData)
        
        tableView.reloadData()
    }
    
    private func loadBayeSaveData() -> [SaveData] {
        var saveDataList: [SaveData] = []
        let allKeys = UserDefaults.standard.dictionaryRepresentation().keys
        
        // 三国霸业存档路径格式: MOD名称_baye//data//sangoX.sav
        let bayeSavePattern = "baye//data//sango"
        
        for key in allKeys {
            // 检查是否是三国霸业存档，只显示 sango0.sav、sango2.sav、sango4.sav
            if key.contains(bayeSavePattern) && (key.contains("sango0.sav") || key.contains("sango2.sav") || key.contains("sango4.sav")) {
                if let content = UserDefaults.standard.string(forKey: key), !content.isEmpty {
                    let (gameKey, gameName) = extractBayeGameInfo(from: key)
                    let saveData = SaveData(
                        key: key,
                        title: generateBayeSaveTitle(from: key),
                        content: content,
                        createdDate: extractDateFromSaveContent(content) ?? Date(),
                        gameKey: gameKey,
                        gameName: gameName
                    )
                    saveDataList.append(saveData)
                }
            }
        }
        
        return saveDataList
    }
    
    private func loadFMJSaveData() -> [SaveData] {
        var saveDataList: [SaveData] = []
        let fmjSaveKeys = ["sav/fmjsave0", "sav/fmjsave1", "sav/fmjsave2", "sav/fmjsave3", "sav/fmjsave4"]
        
        // 获取当前选择的mod
        let currentMod = HDAppData.shared.choiceLib["key"] ?? "FMJ"
        
        for baseKey in fmjSaveKeys {
            var actualKey = baseKey
            // 如果不是原版，需要添加mod前缀
            if currentMod != "FMJ" {
                actualKey = currentMod + "_" + baseKey
            }
            
            if let content = UserDefaults.standard.string(forKey: actualKey), !content.isEmpty {
                let (gameKey, gameName) = extractGameInfo(from: actualKey)
                let saveData = SaveData(
                    key: actualKey,
                    title: generateSaveTitle(from: baseKey),
                    content: content,
                    createdDate: extractDateFromSaveContent(content) ?? Date(),
                    gameKey: gameKey,
                    gameName: gameName
                )
                saveDataList.append(saveData)
            }
        }
        
        // 也检查其他mod的存档和直接的fmjsave存档
        let allKeys = UserDefaults.standard.dictionaryRepresentation().keys
        for key in allKeys {
            // 检查带前缀的其他mod存档
            if key.contains("_sav/fmjsave") && !key.hasPrefix(currentMod + "_") {
                if let content = UserDefaults.standard.string(forKey: key), !content.isEmpty {
                    let (gameKey, gameName) = extractGameInfo(from: key)
                    let saveData = SaveData(
                        key: key,
                        title: generateSaveTitle(from: key),
                        content: content,
                        createdDate: extractDateFromSaveContent(content) ?? Date(),
                        gameKey: gameKey,
                        gameName: gameName
                    )
                    saveDataList.append(saveData)
                }
            }
            // 检查直接的 sav/fmjsave 存档（当当前mod不是FMJ时，也要包含原版存档）
            else if key.hasPrefix("sav/fmjsave") && currentMod != "FMJ" {
                if let content = UserDefaults.standard.string(forKey: key), !content.isEmpty {
                    let (gameKey, gameName) = extractGameInfo(from: key)
                    let saveData = SaveData(
                        key: key,
                        title: generateSaveTitle(from: key),
                        content: content,
                        createdDate: extractDateFromSaveContent(content) ?? Date(),
                        gameKey: gameKey,
                        gameName: gameName
                    )
                    saveDataList.append(saveData)
                }
            }
        }
        
        return saveDataList
    }
    
    private func generateBayeSaveTitle(from key: String) -> String {
        // 三国霸业存档标题生成: MOD名称_baye//data//sangoX.sav
        if let modName = extractBayeModName(from: key) {
            // 提取存档序号
            if key.contains("sango0.sav") {
                return "\(modName) - 存档位置 1"
            } else if key.contains("sango2.sav") {
                return "\(modName) - 存档位置 2"
            } else if key.contains("sango4.sav") {
                return "\(modName) - 存档位置 3"
            } 
        }
        return key
    }
    
    private func generateSaveTitle(from key: String) -> String {
        if key.contains("fmjsave0") {
            return "存档位置 1"
        } else if key.contains("fmjsave1") {
            return "存档位置 2"
        } else if key.contains("fmjsave2") {
            return "存档位置 3"
        } else if key.contains("fmjsave3") {
            return "存档位置 4"
        } else if key.contains("fmjsave4") {
            return "存档位置 5"
        } else if key.contains("_sav/") {
            // 霸业存档，提取mod名称
            let components = key.components(separatedBy: "_sav/")
            if components.count > 0 {
                let modName = components[0]
                let saveFileName = components.last ?? ""
                return "\(modName) - \(saveFileName)"
            }
        } else if key.hasPrefix("sav/") {
            // 直接的存档key
            let fileName = String(key.dropFirst(4)) // 去掉"sav/"前缀
            return fileName
        }
        
        return key
    }
    
    private func extractDateFromSaveContent(_ content: String) -> Date? {
        // 尝试从存档内容中提取时间信息
        // 这里可以根据游戏存档的实际格式来解析时间
        // 暂时返回当前时间
        return Date()
    }
    
    private func extractBayeModName(from key: String) -> String? {
        // 从 MOD名称_baye//data//sangoX.sav 中提取 MOD名称
        if let range = key.range(of: "_baye//data//sango") {
            return String(key[..<range.lowerBound])
        }
        return nil
    }
    
    private func extractBayeGameInfo(from key: String) -> (String, String) {
        // 从三国霸业存档key中提取游戏信息
        if let modName = extractBayeModName(from: key) {
            let gameName = gameMapping[modName] ?? modName
            return (modName, gameName)
        }
        return ("UNKNOWN", "未知游戏")
    }
    
    private func extractGameInfo(from key: String) -> (String, String) {
        // 从存档key中提取游戏前缀
        if key.contains("_sav/") {
            // 格式如: "XKX_sav/xxx" 或 "FMJWMB_sav/fmjsave0"
            let components = key.components(separatedBy: "_sav/")
            if let gameKey = components.first {
                let gameName = gameMapping[gameKey] ?? gameKey
                return (gameKey, gameName)
            }
        } else if key.hasPrefix("sav/") {
            // 直接的存档key，如 sav/fmjsave0, sav/fmjsave1 等
            if key.contains("fmjsave") {
                // 没有前缀的 fmjsave 存档属于伏魔记原版
                return ("FMJ", gameMapping["FMJ"] ?? "伏魔记")
            }
            // 其他直接存档，也可能是伏魔记相关
            // 为了确保显示，先归类到伏魔记
            return ("FMJ", gameMapping["FMJ"] ?? "伏魔记")
        }
        
        return ("UNKNOWN", "未知游戏")
    }
    
    private func extractSavePosition(from key: String) -> Int {
        // 从key中提取存档位置序号，用于排序
        
        // 处理三国霸业存档 sangoX.sav (只显示0、2、4作为存档一、二、三)
        if key.contains("sango0.sav") {
            return 0
        } else if key.contains("sango2.sav") {
            return 1
        } else if key.contains("sango4.sav") {
            return 2
        }
        
        // 处理伏魔记存档 fmjsaveX
        else if key.contains("fmjsave0") {
            return 0
        } else if key.contains("fmjsave1") {
            return 1
        } else if key.contains("fmjsave2") {
            return 2
        } else if key.contains("fmjsave3") {
            return 3
        } else if key.contains("fmjsave4") {
            return 4
        }
        
        // 处理其他存档文件，尝试从文件名中提取数字
        var fileName = ""
        if key.contains("_sav/") {
            let components = key.components(separatedBy: "_sav/")
            fileName = components.last ?? key
        } else if key.hasPrefix("sav/") {
            fileName = String(key.dropFirst(4))
        } else {
            fileName = key
        }
        
        // 尝试从文件名中提取数字序号
        let digits = fileName.compactMap { $0.wholeNumberValue }
        if let firstNumber = digits.first {
            return firstNumber + 1000  // 加上偏移量，确保在标准存档之后
        }
        
        // 如果没有数字，使用字母排序
        return 2000 + fileName.hashValue % 1000  // 使用有限范围的hash避免过大的数字
    }
    
    private func groupSaveDataByGame(_ saveDataList: [SaveData]) {
        var gameGroups: [String: [SaveData]] = [:]
        
        // 按游戏分组
        for saveData in saveDataList {
            if gameGroups[saveData.gameKey] == nil {
                gameGroups[saveData.gameKey] = []
            }
            gameGroups[saveData.gameKey]?.append(saveData)
        }
        
        // 转换为 GameSection 数组
        gameSections = gameGroups.map { (gameKey, saveList) in
            // 按存档位置排序
            let sortedSaveList = saveList.sorted { save1, save2 in
                let position1 = extractSavePosition(from: save1.key)
                let position2 = extractSavePosition(from: save2.key)
                return position1 < position2
            }
            
            let gameName = gameMapping[gameKey] ?? gameKey
            return GameSection(gameKey: gameKey, gameName: gameName, saveDataList: sortedSaveList)
        }
        
        // 按游戏名称排序sections
        gameSections.sort { $0.gameName < $1.gameName }
    }
}

// MARK: - UITableViewDataSource
extension HDSaveListViewController: UITableViewDataSource {
    func numberOfSections(in tableView: UITableView) -> Int {
        return gameSections.isEmpty ? 1 : gameSections.count
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        if gameSections.isEmpty {
            return 1  // 显示空状态
        }
        return gameSections[section].saveDataList.count
    }
    
    func tableView(_ tableView: UITableView, titleForHeaderInSection section: Int) -> String? {
        if gameSections.isEmpty {
            return nil
        }
        return gameSections[section].gameName
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        if gameSections.isEmpty {
            let cell = tableView.dequeueReusableCell(withIdentifier: "EmptyCell", for: indexPath)
            cell.textLabel?.text = "暂无存档数据"
            cell.textLabel?.textAlignment = .center
            cell.textLabel?.textColor = .secondaryLabel
            cell.selectionStyle = .none
            return cell
        }
        
        let cell = tableView.dequeueReusableCell(withIdentifier: SaveDataCell.identifier, for: indexPath) as! SaveDataCell
        let saveData = gameSections[indexPath.section].saveDataList[indexPath.row]
        cell.configure(with: saveData)
        return cell
    }
}

// MARK: - UITableViewDelegate
extension HDSaveListViewController: UITableViewDelegate {
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        tableView.deselectRow(at: indexPath, animated: true)
        
        guard !gameSections.isEmpty else { return }
        
        let saveData = gameSections[indexPath.section].saveDataList[indexPath.row]
        let detailVC = HDSaveDetailViewController(saveData: saveData)
        navigationController?.pushViewController(detailVC, animated: true)
    }
    
    func tableView(_ tableView: UITableView, heightForRowAt indexPath: IndexPath) -> CGFloat {
        return gameSections.isEmpty ? 100 : UITableView.automaticDimension
    }
    
    func tableView(_ tableView: UITableView, estimatedHeightForRowAt indexPath: IndexPath) -> CGFloat {
        return 80
    }
    
    
}

// MARK: - UIGestureRecognizerDelegate
extension HDSaveListViewController: UIGestureRecognizerDelegate {
    func gestureRecognizer(_ gestureRecognizer: UIGestureRecognizer, shouldBeRequiredToFailBy otherGestureRecognizer: UIGestureRecognizer) -> Bool {
        return true
    }
}

// MARK: - SaveDataCell
class SaveDataCell: UITableViewCell {
    static let identifier = "SaveDataCell"
    
    private let titleLabel = UILabel()
    private let contentLabel = UILabel()
    
    override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
        super.init(style: style, reuseIdentifier: reuseIdentifier)
        setupUI()
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    private func setupUI() {
        accessoryType = .disclosureIndicator
        
        titleLabel.font = UIFont.systemFont(ofSize: 16, weight: .medium)
        titleLabel.textColor = .label
        titleLabel.numberOfLines = 1
        
        contentLabel.font = UIFont.systemFont(ofSize: 14)
        contentLabel.textColor = .secondaryLabel
        contentLabel.numberOfLines = 2
        
        [titleLabel, contentLabel].forEach {
            $0.translatesAutoresizingMaskIntoConstraints = false
            contentView.addSubview($0)
        }
        
        NSLayoutConstraint.activate([
            titleLabel.topAnchor.constraint(equalTo: contentView.topAnchor, constant: 12),
            titleLabel.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: 16),
            titleLabel.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: -32),
            
            contentLabel.topAnchor.constraint(equalTo: titleLabel.bottomAnchor, constant: 4),
            contentLabel.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: 16),
            contentLabel.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: -32),
            contentLabel.bottomAnchor.constraint(equalTo: contentView.bottomAnchor, constant: -12)
        ])
    }
    
    func configure(with saveData: SaveData) {
        titleLabel.text = saveData.displayTitle
        contentLabel.text = saveData.displayContent
    }
}
