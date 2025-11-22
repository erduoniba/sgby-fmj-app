//
//  HDFeedbackViewController.swift
//  HDBayeApp
//
//  Created by harrydeng on 2025/8/1.
//

import UIKit

class HDFeedbackViewController: HDBaseViewController {
    
    // MARK: - Constants
    private static let savedContactKey = "HDFeedbackViewController.savedContact"
    
    private let scrollView = UIScrollView()
    private let contentView = UIView()
    
    private let feedbackTextView: UITextView = {
        let textView = UITextView()
        textView.font = UIFont.systemFont(ofSize: 16)
        textView.layer.borderColor = UIColor.systemGray4.cgColor
        textView.layer.borderWidth = 1
        textView.layer.cornerRadius = 8
        textView.backgroundColor = .secondarySystemGroupedBackground
        return textView
    }()
    
    private let placeholderLabel: UILabel = {
        let label = UILabel()
        label.text = "请描述您遇到的问题或建议..."
        label.font = UIFont.systemFont(ofSize: 16)
        label.textColor = .placeholderText
        return label
    }()
    
    private let contactTextField: UITextField = {
        let textField = UITextField()
        textField.placeholder = "您的联系方式（可选，建议使用QQ号）"
        textField.font = UIFont.systemFont(ofSize: 16)
        textField.borderStyle = .roundedRect
        textField.backgroundColor = .secondarySystemGroupedBackground
        return textField
    }()
    
    private let selectSaveButton: UIButton = {
        let button = UIButton(type: .system)
        button.setTitle("选择游戏存档（强烈建议选择）", for: .normal)
        button.titleLabel?.font = UIFont.systemFont(ofSize: 16)
        button.layer.cornerRadius = 8
        button.backgroundColor = .systemBlue
        button.setTitleColor(.white, for: .normal)
        return button
    }()
    
    private let selectedSaveLabel: UILabel = {
        let label = UILabel()
        label.text = "未选择存档"
        label.font = UIFont.systemFont(ofSize: 14)
        label.textColor = .secondaryLabel
        label.textAlignment = .center
        label.numberOfLines = 0
        return label
    }()
    
    private let submitButton: UIButton = {
        let button = UIButton(type: .system)
        button.setTitle("提交反馈", for: .normal)
        button.titleLabel?.font = UIFont.systemFont(ofSize: 18, weight: .medium)
        button.layer.cornerRadius = 8
        button.backgroundColor = .systemGreen
        button.setTitleColor(.white, for: .normal)
        return button
    }()
    
    private var selectedSaveData: SaveData?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        setupUI()
        setupConstraints()
        setupActions()
        loadSavedContact()
    }
    
    private func setupUI() {
        title = "意见反馈"
        view.backgroundColor = .systemGroupedBackground
        
        let backButton = UIBarButtonItem(
            image: UIImage(systemName: "xmark"),
            style: .plain,
            target: self,
            action: #selector(cancelButtonTapped)
        )
        backButton.tintColor = .label
        navigationItem.leftBarButtonItem = backButton
        
        view.addSubview(scrollView)
        scrollView.addSubview(contentView)
        
        contentView.addSubview(feedbackTextView)
        contentView.addSubview(placeholderLabel)
        contentView.addSubview(contactTextField)
        contentView.addSubview(selectSaveButton)
        contentView.addSubview(selectedSaveLabel)
        contentView.addSubview(submitButton)
        
        feedbackTextView.delegate = self
    }
    
    private func setupConstraints() {
        scrollView.translatesAutoresizingMaskIntoConstraints = false
        contentView.translatesAutoresizingMaskIntoConstraints = false
        feedbackTextView.translatesAutoresizingMaskIntoConstraints = false
        placeholderLabel.translatesAutoresizingMaskIntoConstraints = false
        contactTextField.translatesAutoresizingMaskIntoConstraints = false
        selectSaveButton.translatesAutoresizingMaskIntoConstraints = false
        selectedSaveLabel.translatesAutoresizingMaskIntoConstraints = false
        submitButton.translatesAutoresizingMaskIntoConstraints = false
        
        let constant = 12.0
        
        NSLayoutConstraint.activate([
            scrollView.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor),
            scrollView.leadingAnchor.constraint(equalTo: view.leadingAnchor),
            scrollView.trailingAnchor.constraint(equalTo: view.trailingAnchor),
            scrollView.bottomAnchor.constraint(equalTo: view.bottomAnchor),
            
            contentView.topAnchor.constraint(equalTo: scrollView.topAnchor),
            contentView.leadingAnchor.constraint(equalTo: scrollView.leadingAnchor),
            contentView.trailingAnchor.constraint(equalTo: scrollView.trailingAnchor),
            contentView.bottomAnchor.constraint(equalTo: scrollView.bottomAnchor),
            contentView.widthAnchor.constraint(equalTo: scrollView.widthAnchor),
            
            feedbackTextView.topAnchor.constraint(equalTo: contentView.topAnchor, constant: constant),
            feedbackTextView.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: constant),
            feedbackTextView.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: 0 - constant),
            feedbackTextView.heightAnchor.constraint(equalToConstant: 120),
            
            placeholderLabel.topAnchor.constraint(equalTo: feedbackTextView.topAnchor, constant: 8),
            placeholderLabel.leadingAnchor.constraint(equalTo: feedbackTextView.leadingAnchor, constant: 8),
            
            contactTextField.topAnchor.constraint(equalTo: feedbackTextView.bottomAnchor, constant: constant),
            contactTextField.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: constant),
            contactTextField.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: 0 - constant),
            contactTextField.heightAnchor.constraint(equalToConstant: 44),
            
            selectSaveButton.topAnchor.constraint(equalTo: contactTextField.bottomAnchor, constant: constant),
            selectSaveButton.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: constant),
            selectSaveButton.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: 0 - constant),
            selectSaveButton.heightAnchor.constraint(equalToConstant: 44),
            
            selectedSaveLabel.topAnchor.constraint(equalTo: selectSaveButton.bottomAnchor, constant: 10),
            selectedSaveLabel.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: constant),
            selectedSaveLabel.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: 0 - constant),
            
            submitButton.topAnchor.constraint(equalTo: selectedSaveLabel.bottomAnchor, constant: 20),
            submitButton.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: constant),
            submitButton.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: 0 - constant),
            submitButton.heightAnchor.constraint(equalToConstant: 50),
            submitButton.bottomAnchor.constraint(equalTo: contentView.bottomAnchor, constant: -40)
        ])
    }
    
    private func setupActions() {
        selectSaveButton.addTarget(self, action: #selector(selectSaveButtonTapped), for: .touchUpInside)
        submitButton.addTarget(self, action: #selector(submitButtonTapped), for: .touchUpInside)
        
        let tapGesture = UITapGestureRecognizer(target: self, action: #selector(dismissKeyboard))
        view.addGestureRecognizer(tapGesture)
    }
    
    @objc private func cancelButtonTapped() {
        dismiss(animated: true)
    }
    
    @objc private func selectSaveButtonTapped() {
        let saveSelectionVC = HDSaveSelectionViewController()
        saveSelectionVC.onSaveSelected = { [weak self] saveData in
            self?.selectedSaveData = saveData
            self?.updateSelectedSaveLabel()
        }
        
        let navController = UINavigationController(rootViewController: saveSelectionVC)
        navController.modalPresentationStyle = .pageSheet
        if #available(iOS 15.0, *) {
            if let sheet = navController.sheetPresentationController {
                sheet.detents = [.medium(), .large()]
                sheet.prefersGrabberVisible = true
            }
        }
        
        present(navController, animated: true)
    }
    
    private func updateSelectedSaveLabel() {
        guard let saveData = selectedSaveData else {
            selectedSaveLabel.text = "未选择存档（强烈建议选择问题对应的存档）"
            selectedSaveLabel.textColor = .secondaryLabel
            return
        }
        
        selectedSaveLabel.text = "已选择: \(saveData.gameName) - \(saveData.displayTitle)"
        selectedSaveLabel.textColor = .label
    }
    
    @objc private func submitButtonTapped() {
        guard let feedbackText = feedbackTextView.text, !feedbackText.isEmpty else {
            showAlert(title: "提示", message: "请输入反馈内容")
            return
        }
        
        view.makeToastActivity(.center)
        
        // 准备反馈数据
        let contact = contactTextField.text ?? ""
        // 保存联系方式
        saveContact(contact)
        
        // 调用API服务提交反馈
        HDFeedbackService.shared.submitFeedback(
            content: feedbackText,
            contact: contact,
            saveData: selectedSaveData
        ) { [weak self] success, message in
            DispatchQueue.main.async {
                self?.view.hideToastActivity()
                
                if success {
                    self?.showAlert(title: "成功", message: "感谢您的反馈！") {
                        self?.dismiss(animated: true)
                    }
                } else {
                    self?.showAlert(title: "失败", message: message ?? "提交失败，请稍后重试")
                }
            }
        }
    }
    
    @objc private func dismissKeyboard() {
        view.endEditing(true)
    }
    
    // MARK: - Contact Management
    
    /// 加载保存的联系方式
    private func loadSavedContact() {
        let savedContact = UserDefaults.standard.string(forKey: Self.savedContactKey) ?? ""
        contactTextField.text = savedContact
    }
    
    /// 保存联系方式到UserDefaults
    private func saveContact(_ contact: String) {
        // 只有联系方式不为空时才保存
        if !contact.trimmingCharacters(in: .whitespacesAndNewlines).isEmpty {
            UserDefaults.standard.set(contact, forKey: Self.savedContactKey)
        }
    }
    
    private func showAlert(title: String, message: String, completion: (() -> Void)? = nil) {
        let alert = UIAlertController(title: title, message: message, preferredStyle: .alert)
        alert.addAction(UIAlertAction(title: "确定", style: .default) { _ in
            completion?()
        })
        present(alert, animated: true)
    }
}

// MARK: - UITextViewDelegate
extension HDFeedbackViewController: UITextViewDelegate {
    func textViewDidChange(_ textView: UITextView) {
        placeholderLabel.isHidden = !textView.text.isEmpty
    }
}

// MARK: - HDSaveSelectionViewController
class HDSaveSelectionViewController: UIViewController {
    
    private let tableView = UITableView(frame: .zero, style: .insetGrouped)
    private var gameSections: [GameSection] = []
    var onSaveSelected: ((SaveData) -> Void)?
    
    // 动态获取游戏映射关系
    private var gameMapping: [String: String] {
        let dynamicMapping = HDAppData.shared.libsList
        return dynamicMapping
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        setupUI()
        loadSaveData()
    }
    
    private func setupUI() {
        title = "选择存档"
        view.backgroundColor = .systemGroupedBackground
        
        navigationItem.leftBarButtonItem = UIBarButtonItem(
            title: "取消",
            style: .plain,
            target: self,
            action: #selector(cancelButtonTapped)
        )
        
        tableView.delegate = self
        tableView.dataSource = self
        tableView.register(SaveSelectionCell.self, forCellReuseIdentifier: SaveSelectionCell.identifier)
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
    
    @objc private func cancelButtonTapped() {
        dismiss(animated: true)
    }
    
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
    
    // 复用HDSaveListViewController中的方法
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
                        createdDate: Date(),
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
        
        let currentMod = HDAppData.shared.choiceLib["key"] ?? "FMJ"
        
        for baseKey in fmjSaveKeys {
            var actualKey = baseKey
            if currentMod != "FMJ" {
                actualKey = currentMod + "_" + baseKey
            }
            
            if let content = UserDefaults.standard.string(forKey: actualKey), !content.isEmpty {
                let (gameKey, gameName) = extractGameInfo(from: actualKey)
                let saveData = SaveData(
                    key: actualKey,
                    title: generateSaveTitle(from: baseKey),
                    content: content,
                    createdDate: Date(),
                    gameKey: gameKey,
                    gameName: gameName
                )
                saveDataList.append(saveData)
            }
        }
        
        let allKeys = UserDefaults.standard.dictionaryRepresentation().keys
        for key in allKeys {
            if key.contains("_sav/fmjsave") && !key.hasPrefix(currentMod + "_") {
                if let content = UserDefaults.standard.string(forKey: key), !content.isEmpty {
                    let (gameKey, gameName) = extractGameInfo(from: key)
                    let saveData = SaveData(
                        key: key,
                        title: generateSaveTitle(from: key),
                        content: content,
                        createdDate: Date(),
                        gameKey: gameKey,
                        gameName: gameName
                    )
                    saveDataList.append(saveData)
                }
            } else if key.hasPrefix("sav/fmjsave") && currentMod != "FMJ" {
                if let content = UserDefaults.standard.string(forKey: key), !content.isEmpty {
                    let (gameKey, gameName) = extractGameInfo(from: key)
                    let saveData = SaveData(
                        key: key,
                        title: generateSaveTitle(from: key),
                        content: content,
                        createdDate: Date(),
                        gameKey: gameKey,
                        gameName: gameName
                    )
                    saveDataList.append(saveData)
                }
            }
        }
        
        return saveDataList
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
            let components = key.components(separatedBy: "_sav/")
            if components.count > 0 {
                let modName = components[0]
                let saveFileName = components.last ?? ""
                return "\(modName) - \(saveFileName)"
            }
        } else if key.hasPrefix("sav/") {
            let fileName = String(key.dropFirst(4))
            return fileName
        }
        
        return key
    }
    
    private func extractGameInfo(from key: String) -> (String, String) {
        if key.contains("_sav/") {
            let components = key.components(separatedBy: "_sav/")
            if let gameKey = components.first {
                let gameName = gameMapping[gameKey] ?? gameKey
                return (gameKey, gameName)
            }
        } else if key.hasPrefix("sav/") {
            if key.contains("fmjsave") {
                return ("FMJ", gameMapping["FMJ"] ?? "伏魔记")
            }
            return ("FMJ", gameMapping["FMJ"] ?? "伏魔记")
        }
        
        return ("UNKNOWN", "未知游戏")
    }
    
    private func groupSaveDataByGame(_ saveDataList: [SaveData]) {
        var gameGroups: [String: [SaveData]] = [:]
        
        for saveData in saveDataList {
            if gameGroups[saveData.gameKey] == nil {
                gameGroups[saveData.gameKey] = []
            }
            gameGroups[saveData.gameKey]?.append(saveData)
        }
        
        gameSections = gameGroups.map { (gameKey, saveList) in
            let sortedSaveList = saveList.sorted { $0.title < $1.title }
            let gameName = gameMapping[gameKey] ?? gameKey
            return GameSection(gameKey: gameKey, gameName: gameName, saveDataList: sortedSaveList)
        }
        
        gameSections.sort { $0.gameName < $1.gameName }
    }
}

// MARK: - UITableViewDataSource & Delegate for Save Selection
extension HDSaveSelectionViewController: UITableViewDataSource, UITableViewDelegate {
    func numberOfSections(in tableView: UITableView) -> Int {
        return gameSections.isEmpty ? 1 : gameSections.count
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        if gameSections.isEmpty {
            return 1
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
        
        let cell = tableView.dequeueReusableCell(withIdentifier: SaveSelectionCell.identifier, for: indexPath) as! SaveSelectionCell
        let saveData = gameSections[indexPath.section].saveDataList[indexPath.row]
        cell.configure(with: saveData)
        return cell
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        tableView.deselectRow(at: indexPath, animated: true)
        
        guard !gameSections.isEmpty else { return }
        
        let saveData = gameSections[indexPath.section].saveDataList[indexPath.row]
        onSaveSelected?(saveData)
        dismiss(animated: true)
    }
    
    func tableView(_ tableView: UITableView, heightForRowAt indexPath: IndexPath) -> CGFloat {
        return gameSections.isEmpty ? 100 : 70
    }
}

// MARK: - SaveSelectionCell
class SaveSelectionCell: UITableViewCell {
    static let identifier = "SaveSelectionCell"
    
    private let titleLabel = UILabel()
    private let contentPreviewLabel = UILabel()
    
    override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
        super.init(style: style, reuseIdentifier: reuseIdentifier)
        setupUI()
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    private func setupUI() {
        accessoryType = .none
        
        titleLabel.font = UIFont.systemFont(ofSize: 16, weight: .medium)
        titleLabel.textColor = .label
        titleLabel.numberOfLines = 1
        
        contentPreviewLabel.font = UIFont.systemFont(ofSize: 14)
        contentPreviewLabel.textColor = .secondaryLabel
        contentPreviewLabel.numberOfLines = 1
        
        [titleLabel, contentPreviewLabel].forEach {
            $0.translatesAutoresizingMaskIntoConstraints = false
            contentView.addSubview($0)
        }
        
        NSLayoutConstraint.activate([
            titleLabel.topAnchor.constraint(equalTo: contentView.topAnchor, constant: 12),
            titleLabel.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: 16),
            titleLabel.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: -16),
            
            contentPreviewLabel.topAnchor.constraint(equalTo: titleLabel.bottomAnchor, constant: 4),
            contentPreviewLabel.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: 16),
            contentPreviewLabel.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: -16),
            contentPreviewLabel.bottomAnchor.constraint(equalTo: contentView.bottomAnchor, constant: -12)
        ])
    }
    
    func configure(with saveData: SaveData) {
        titleLabel.text = saveData.displayTitle
        contentPreviewLabel.text = "数据长度: \(saveData.content.count) 字符"
    }
}
