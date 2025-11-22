//
//  HDSaveDetailViewController.swift
//  HDBayeApp
//
//  Created by harrydeng on 2025/7/27.
//

import UIKit
import Toast_Swift

class HDSaveDetailViewController: HDBaseViewController {
    
    private var saveData: SaveData
    
    // UI Components
    private let scrollView = UIScrollView()
    private let contentView = UIView()
    private let titleLabel = UILabel()
    private let keyLabel = UILabel()
    private let dateLabel = UILabel()
    private let textView = UITextView()
    private let copyButton = UIBarButtonItem()
    
    private let saveButton = UIBarButtonItem()
    private var isEditingEnabled = false
    private var originalContent: String = ""
    
    init(saveData: SaveData) {
        self.saveData = saveData
        super.init(nibName: nil, bundle: nil)
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        setupUI()
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(animated)
    }
    
    deinit {
        NotificationCenter.default.removeObserver(self)
    }
    
    private func setupUI() {
        title = "存档详情"
        view.backgroundColor = .systemGroupedBackground
        
        setupNavigationBar()
        setupTitleTapGesture()
        setupScrollView()
        setupContentView()
    }
    
    private func setupTitleTapGesture() {
        // 添加标题栏点击手势
        let tapGesture = UITapGestureRecognizer(target: self, action: #selector(navigationBarTapped))
        tapGesture.numberOfTapsRequired = 5
        navigationController?.navigationBar.isUserInteractionEnabled = true
        
        // 为整个导航栏添加手势
        navigationController?.navigationBar.addGestureRecognizer(tapGesture)
    }
    
    @objc private func navigationBarTapped() {
        unlockEditMode()
    }
    
    private func unlockEditMode() {
        // 震动反馈
        let impactFeedback = UIImpactFeedbackGenerator(style: .medium)
        impactFeedback.impactOccurred()
        
        // 添加编辑按钮
        let editButton = UIBarButtonItem(
            title: "编辑",
            style: .plain,
            target: self,
            action: #selector(editButtonTapped)
        )
        
        // 更新导航栏按钮
        navigationItem.rightBarButtonItems = [copyButton, editButton, saveButton]
        
        // 显示提示
        view.makeToast("编辑模式已解锁", duration: 2.0, position: .center)
    }
    
    private func setupNavigationBar() {
        // 返回按钮
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
        
        // 复制按钮
        copyButton.title = "复制"
        copyButton.style = .plain
        copyButton.target = self
        copyButton.action = #selector(copyButtonTapped)
        
        // 保存按钮（默认隐藏，通过5次点击导航栏激活）
        saveButton.title = "保存"
        saveButton.style = .plain
        saveButton.target = self
        saveButton.action = #selector(saveButtonTapped)
        saveButton.isEnabled = false // 默认禁用，编辑时启用
        
        // 默认只显示复制按钮
        navigationItem.rightBarButtonItem = copyButton
    }
    
    private func setupScrollView() {
        scrollView.translatesAutoresizingMaskIntoConstraints = false
        view.addSubview(scrollView)
        
        NSLayoutConstraint.activate([
            scrollView.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor),
            scrollView.leadingAnchor.constraint(equalTo: view.leadingAnchor),
            scrollView.trailingAnchor.constraint(equalTo: view.trailingAnchor),
            scrollView.bottomAnchor.constraint(equalTo: view.bottomAnchor)
        ])
    }
    
    private func setupContentView() {
        contentView.translatesAutoresizingMaskIntoConstraints = false
        scrollView.addSubview(contentView)
        
        NSLayoutConstraint.activate([
            contentView.topAnchor.constraint(equalTo: scrollView.topAnchor),
            contentView.leadingAnchor.constraint(equalTo: scrollView.leadingAnchor),
            contentView.trailingAnchor.constraint(equalTo: scrollView.trailingAnchor),
            contentView.bottomAnchor.constraint(equalTo: scrollView.bottomAnchor),
            contentView.widthAnchor.constraint(equalTo: scrollView.widthAnchor)
        ])
        
        setupLabels()
        setupTextView()
        setupConstraints()
    }
    
    private func setupLabels() {
        // 标题标签
        titleLabel.text = saveData.displayTitle
        titleLabel.font = UIFont.systemFont(ofSize: 20, weight: .bold)
        titleLabel.textColor = .label
        titleLabel.numberOfLines = 0
        titleLabel.translatesAutoresizingMaskIntoConstraints = false
        
        // Key标签
        keyLabel.text = "Key: \(saveData.key)"
        keyLabel.font = UIFont.systemFont(ofSize: 14)
        keyLabel.textColor = .secondaryLabel
        keyLabel.numberOfLines = 0
        keyLabel.translatesAutoresizingMaskIntoConstraints = false
        
        // 日期标签 - 不显示
        
        [titleLabel, keyLabel].forEach {
            contentView.addSubview($0)
        }
    }
    
    private func setupTextView() {
        textView.text = saveData.content
        textView.font = UIFont.monospacedSystemFont(ofSize: 14, weight: .regular)
        textView.backgroundColor = .secondarySystemGroupedBackground
        textView.layer.cornerRadius = 8
        textView.layer.borderWidth = 1
        textView.layer.borderColor = UIColor.separator.cgColor
        textView.textContainerInset = UIEdgeInsets(top: 12, left: 12, bottom: 12, right: 12)
        
        // 默认不可编辑
        textView.isEditable = false
        originalContent = saveData.content
        textView.isSelectable = true
        textView.translatesAutoresizingMaskIntoConstraints = false
        
        // 添加长按手势和上下文菜单
        textView.isUserInteractionEnabled = true
        let longPressGesture = UILongPressGestureRecognizer(target: self, action: #selector(handleLongPress(_:)))
        longPressGesture.minimumPressDuration = 0.5
        textView.addGestureRecognizer(longPressGesture)
        
        // 添加文本变化监听
        textView.delegate = self
        contentView.addSubview(textView)
    }
    
    private func setupConstraints() {
        NSLayoutConstraint.activate([
            // 标题
            titleLabel.topAnchor.constraint(equalTo: contentView.topAnchor, constant: 20),
            titleLabel.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: 16),
            titleLabel.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: -16),
            
            // Key
            keyLabel.topAnchor.constraint(equalTo: titleLabel.bottomAnchor, constant: 8),
            keyLabel.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: 16),
            keyLabel.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: -16),
            
            // 文本视图 - 直接连接到keyLabel
            textView.topAnchor.constraint(equalTo: keyLabel.bottomAnchor, constant: 20),
            textView.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: 16),
            textView.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: -16),
            textView.heightAnchor.constraint(greaterThanOrEqualToConstant: 400),
            textView.bottomAnchor.constraint(equalTo: contentView.bottomAnchor, constant: -20)
        ])
    }
    
    
    // MARK: - Actions
    @objc private func backButtonTapped() {
        navigationController?.popViewController(animated: true)
    }
    
    @objc private func copyButtonTapped() {
        // 复制存档内容到剪贴板
        UIPasteboard.general.string = textView.text
        
        // 显示复制成功提示
        view.makeToast("已复制到剪贴板", duration: 1.5, position: .center)
    }
    
    @objc private func handleLongPress(_ gesture: UILongPressGestureRecognizer) {
        if gesture.state == .began {
            // 震动反馈
            let impactFeedback = UIImpactFeedbackGenerator(style: .medium)
            impactFeedback.impactOccurred()
            
            // 显示复制菜单
            let menuController = UIMenuController.shared
            menuController.showMenu(from: textView, rect: textView.bounds)
        }
    }
    
    // MARK: - Edit Mode Actions
    @objc private func editButtonTapped() {
        isEditingEnabled = !isEditingEnabled
        textView.isEditable = isEditingEnabled
        
        // 更新导航栏按钮
        if isEditingEnabled {
            // 进入编辑模式
            navigationItem.rightBarButtonItems?[1] = UIBarButtonItem(
                title: "取消",
                style: .plain,
                target: self,
                action: #selector(editButtonTapped)
            )
            saveButton.isEnabled = true
            
            // 显示编辑提示
            view.makeToast("进入编辑模式", duration: 1.5, position: .top)
            
            // 让文本框获得焦点
            textView.becomeFirstResponder()
        } else {
            // 退出编辑模式
            navigationItem.rightBarButtonItems?[1] = UIBarButtonItem(
                title: "编辑",
                style: .plain,
                target: self,
                action: #selector(editButtonTapped)
            )
            saveButton.isEnabled = false
            
            // 恢复原始内容
            textView.text = originalContent
            textView.resignFirstResponder()
            
            view.makeToast("取消编辑", duration: 1.5, position: .top)
        }
    }
    
    @objc private func saveButtonTapped() {
        guard isEditingEnabled else { return }
        
        // 验证编辑内容
        let editedContent = textView.text ?? ""
        
        // 基本验证：确保内容不为空
        guard !editedContent.isEmpty else {
            let alert = UIAlertController(
                title: "保存失败",
                message: "存档内容不能为空",
                preferredStyle: .alert
            )
            alert.addAction(UIAlertAction(title: "确定", style: .default))
            present(alert, animated: true)
            return
        }
        
        // 显示确认对话框
        let alert = UIAlertController(
            title: "确认保存",
            message: "修改存档内容可能导致游戏异常，是否确认保存？",
            preferredStyle: .alert
        )
        
        alert.addAction(UIAlertAction(title: "取消", style: .cancel))
        alert.addAction(UIAlertAction(title: "保存", style: .destructive) { [weak self] _ in
            self?.performSave(editedContent)
        })
        
        present(alert, animated: true)
    }
    
    private func performSave(_ content: String) {
        // 创建备份（可选）
        HDSaveManager.shared.backupSaveData(saveData)
        
        // 更新存档数据
        saveData.content = content
        originalContent = content
        
        // 保存到UserDefaults
        if HDSaveManager.shared.updateSaveData(saveData) {
            // 保存成功
            view.makeToast("存档已保存", duration: 2.0, position: .center)
            
            // 退出编辑模式
            isEditingEnabled = false
            textView.isEditable = false
            textView.resignFirstResponder()
            
            // 更新按钮状态
            navigationItem.rightBarButtonItems?[1] = UIBarButtonItem(
                title: "编辑",
                style: .plain,
                target: self,
                action: #selector(editButtonTapped)
            )
            saveButton.isEnabled = false
            
            // 发送通知，通知列表页面刷新
            NotificationCenter.default.post(
                name: NSNotification.Name("SaveDataUpdated"),
                object: nil,
                userInfo: ["saveData": saveData]
            )
        } else {
            // 保存失败
            let alert = UIAlertController(
                title: "保存失败",
                message: "无法保存存档，请稍后重试",
                preferredStyle: .alert
            )
            alert.addAction(UIAlertAction(title: "确定", style: .default))
            present(alert, animated: true)
        }
    }
}


// MARK: - UIGestureRecognizerDelegate
extension HDSaveDetailViewController: UIGestureRecognizerDelegate {
    func gestureRecognizer(_ gestureRecognizer: UIGestureRecognizer, shouldBeRequiredToFailBy otherGestureRecognizer: UIGestureRecognizer) -> Bool {
        return true
    }
}

// MARK: - UITextViewDelegate
extension HDSaveDetailViewController: UITextViewDelegate {
    func textViewDidChange(_ textView: UITextView) {
        // 文本发生变化时，确保保存按钮可用
        if isEditingEnabled {
            saveButton.isEnabled = true
            
            // 可选：实时显示字符数
            let charCount = textView.text.count
            title = "存档详情 (\(charCount)字符)"
        }
    }
    
    func textViewDidBeginEditing(_ textView: UITextView) {
        // 开始编辑时的处理
        if isEditingEnabled {
            // 可以添加高亮边框等视觉效果
            textView.layer.borderColor = UIColor.systemBlue.cgColor
            textView.layer.borderWidth = 2
        }
    }
    
    func textViewDidEndEditing(_ textView: UITextView) {
        // 结束编辑时的处理
        textView.layer.borderColor = UIColor.separator.cgColor
        textView.layer.borderWidth = 1
        
        // 恢复标题
        title = "存档详情"
    }
}
