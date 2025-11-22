//
//  HDGameSelectionViewController.swift
//  HDBayeApp
//
//  Created by harrydeng on 2025/9/14.
//

import UIKit

protocol HDGameSelectionDelegate: AnyObject {
    func didSelectGame(key: String, value: String)
}

// MARK: - 带分组背景的左对齐布局
class GroupedLeftAlignedCollectionViewFlowLayout: UICollectionViewFlowLayout {

    override func layoutAttributesForElements(in rect: CGRect) -> [UICollectionViewLayoutAttributes]? {
        guard let attributes = super.layoutAttributesForElements(in: rect) else { return nil }

        var allAttributes: [UICollectionViewLayoutAttributes] = []

        // 添加分组背景
        let sectionBackgrounds = layoutAttributesForSectionBackgrounds(in: rect)
        allAttributes.append(contentsOf: sectionBackgrounds)

        // 处理Cell的左对齐
        var leftMargin = sectionInset.left
        var maxY: CGFloat = -1.0
        var currentSection = -1

        attributes.forEach { layoutAttribute in
            if layoutAttribute.representedElementCategory == .cell {
                // 新的section，重置左边距
                if layoutAttribute.indexPath.section != currentSection {
                    leftMargin = sectionInset.left
                    currentSection = layoutAttribute.indexPath.section
                    maxY = -1.0
                }

                if layoutAttribute.frame.origin.y >= maxY {
                    leftMargin = sectionInset.left
                }

                layoutAttribute.frame.origin.x = leftMargin
                leftMargin += layoutAttribute.frame.width + minimumInteritemSpacing
                maxY = max(layoutAttribute.frame.maxY, maxY)
            }
            allAttributes.append(layoutAttribute)
        }

        return allAttributes
    }

    private func layoutAttributesForSectionBackgrounds(in rect: CGRect) -> [UICollectionViewLayoutAttributes] {
        var backgroundAttributes: [UICollectionViewLayoutAttributes] = []

        guard let collectionView = collectionView else { return backgroundAttributes }

        let numberOfSections = collectionView.numberOfSections

        for section in 0..<numberOfSections {
            let numberOfItems = collectionView.numberOfItems(inSection: section)
            guard numberOfItems > 0 else { continue }

            let firstIndexPath = IndexPath(item: 0, section: section)
            let lastIndexPath = IndexPath(item: numberOfItems - 1, section: section)

            guard let firstCellAttributes = layoutAttributesForItem(at: firstIndexPath),
                  let lastCellAttributes = layoutAttributesForItem(at: lastIndexPath) else { continue }

            // 计算section背景frame，考虑安全区域
            let availableWidth = collectionView.frame.width - sectionInset.left - sectionInset.right
            let sectionFrame = CGRect(
                x: sectionInset.left - 8,
                y: firstCellAttributes.frame.minY - 8,
                width: availableWidth + 16,
                height: lastCellAttributes.frame.maxY - firstCellAttributes.frame.minY + 16
            )

            let backgroundIndexPath = IndexPath(item: 0, section: section)
            let backgroundAttribute = UICollectionViewLayoutAttributes(forSupplementaryViewOfKind: "SectionBackground", with: backgroundIndexPath)
            backgroundAttribute.frame = sectionFrame
            backgroundAttribute.zIndex = -1

            backgroundAttributes.append(backgroundAttribute)
        }

        return backgroundAttributes
    }
}

struct GameListSection {
    let title: String
    let games: [(key: String, value: String)]
}

class HDGameSelectionViewController: UIViewController {

    weak var delegate: HDGameSelectionDelegate?
    var libsList: [String: String] = [:]
    var currentTitle: String = ""

    private lazy var searchController: UISearchController = {
        let searchController = UISearchController(searchResultsController: nil)
        searchController.searchResultsUpdater = self
        searchController.obscuresBackgroundDuringPresentation = false
        searchController.searchBar.placeholder = "搜索游戏"
        searchController.searchBar.searchBarStyle = .minimal
        searchController.searchBar.setValue("取消", forKey: "cancelButtonText")
        return searchController
    }()

    private lazy var collectionView: UICollectionView = {
        let layout = GroupedLeftAlignedCollectionViewFlowLayout()
        layout.scrollDirection = .vertical
        layout.minimumInteritemSpacing = 8
        layout.minimumLineSpacing = 12
        layout.sectionInset = UIEdgeInsets(top: 12, left: 12, bottom: 12, right: 12)

        let collectionView = UICollectionView(frame: .zero, collectionViewLayout: layout)
        collectionView.delegate = self
        collectionView.dataSource = self
        collectionView.register(HDGameCollectionViewCell.self, forCellWithReuseIdentifier: HDGameCollectionViewCell.identifier)
        collectionView.register(HDSectionBackgroundView.self, forSupplementaryViewOfKind: "SectionBackground", withReuseIdentifier: HDSectionBackgroundView.identifier)
        collectionView.translatesAutoresizingMaskIntoConstraints = false
        collectionView.backgroundColor = UIColor.clear
        collectionView.keyboardDismissMode = .onDrag
        return collectionView
    }()

    private var GameListSections: [GameListSection] = []
    private var filteredSections: [GameListSection] = []
    private var isFiltering: Bool {
        return searchController.isActive && !searchBarIsEmpty
    }
    private var searchBarIsEmpty: Bool {
        return searchController.searchBar.text?.isEmpty ?? true
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        setupUI()
        prepareData()
        updateEmptyState()
    }

    private func setupUI() {
        view.backgroundColor = UIColor.systemGroupedBackground

        // 设置导航栏样式
        navigationItem.title = "游戏选择"

        // 自定义关闭按钮
        let cancelButton = UIBarButtonItem(
            image: UIImage(systemName: "xmark"),
            style: .plain,
            target: self,
            action: #selector(cancelButtonTapped)
        )
        cancelButton.tintColor = .label
        navigationItem.leftBarButtonItem = cancelButton

        // 设置搜索控制器
        navigationItem.searchController = searchController
        navigationItem.hidesSearchBarWhenScrolling = false
        definesPresentationContext = true

        // 添加集合视图
        view.addSubview(collectionView)

        NSLayoutConstraint.activate([
            collectionView.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor),
            collectionView.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor),
            collectionView.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor),
            collectionView.bottomAnchor.constraint(equalTo: view.safeAreaLayoutGuide.bottomAnchor),
        ])
    }

    private func prepareData() {
        // 按游戏名称分组
        let sortedGames = libsList.map { (key: $0.key, value: $0.value) }
            .sorted { $0.value < $1.value }

        // 根据游戏中文名前三个字符分组
        let groupedGames = Dictionary(grouping: sortedGames) { game in
            let gameName = game.value
            let prefixLength = min(3, gameName.count)
            return String(gameName.prefix(prefixLength))
        }

        // 创建分组数据，但标题设为空字符串（不显示但保持分组）
        GameListSections = groupedGames.map { (prefix, games) in
            GameListSection(title: "", games: games.sorted { $0.value < $1.value })
        }.sorted { $0.games.first?.value ?? "" < $1.games.first?.value ?? "" } // 按每组第一个游戏名称排序

        filteredSections = GameListSections
    }

    private func filterContentForSearchText(_ searchText: String) {
        if searchText.isEmpty {
            filteredSections = GameListSections
        } else {
            filteredSections = GameListSections.compactMap { section in
                let filteredGames = section.games.filter { game in
                    return game.value.lowercased().contains(searchText.lowercased()) ||
                           game.key.lowercased().contains(searchText.lowercased())
                }

                return filteredGames.isEmpty ? nil : GameListSection(title: "", games: filteredGames)
            }
        }

        // 更新空状态视图
        updateEmptyState()
        collectionView.reloadData()
    }

    private func updateEmptyState() {
        let shouldShowEmptyState = isFiltering && filteredSections.allSatisfy { $0.games.isEmpty }
        collectionView.isHidden = shouldShowEmptyState
    }

    @objc private func cancelButtonTapped() {
        dismiss(animated: true)
    }
}

// MARK: - UICollectionViewDataSource
extension HDGameSelectionViewController: UICollectionViewDataSource {

    func numberOfSections(in collectionView: UICollectionView) -> Int {
        let sections = isFiltering ? filteredSections : GameListSections
        return sections.count
    }

    func collectionView(_ collectionView: UICollectionView, numberOfItemsInSection section: Int) -> Int {
        let sections = isFiltering ? filteredSections : GameListSections
        return sections[section].games.count
    }

    func collectionView(_ collectionView: UICollectionView, cellForItemAt indexPath: IndexPath) -> UICollectionViewCell {
        let cell = collectionView.dequeueReusableCell(withReuseIdentifier: HDGameCollectionViewCell.identifier, for: indexPath) as! HDGameCollectionViewCell

        let sections = isFiltering ? filteredSections : GameListSections
        let game = sections[indexPath.section].games[indexPath.item]
        let isSelected = (game.value == currentTitle)

        cell.configure(with: game.value, isSelected: isSelected)

        return cell
    }

    func collectionView(_ collectionView: UICollectionView, viewForSupplementaryElementOfKind kind: String, at indexPath: IndexPath) -> UICollectionReusableView {
        if kind == "SectionBackground" {
            let backgroundView = collectionView.dequeueReusableSupplementaryView(ofKind: kind, withReuseIdentifier: HDSectionBackgroundView.identifier, for: indexPath) as! HDSectionBackgroundView
            return backgroundView
        }

        return UICollectionReusableView()
    }
}

// MARK: - UICollectionViewDelegate
extension HDGameSelectionViewController: UICollectionViewDelegate {

    func collectionView(_ collectionView: UICollectionView, didSelectItemAt indexPath: IndexPath) {
        let sections = isFiltering ? filteredSections : GameListSections
        let game = sections[indexPath.section].games[indexPath.item]

        // 如果是在搜索状态下选择游戏，先关闭搜索
        if isFiltering {
            searchController.isActive = false
        }

        // 如果选择的是当前游戏，不需要切换
        if game.value == currentTitle {
            dismiss(animated: true)
            return
        }

        // 显示确认对话框
        let alert = UIAlertController(
            title: "切换游戏",
            message: "切换前记得存档，确定要切换到「\(game.value)」吗？",
            preferredStyle: .alert
        )

        let cancelAction = UIAlertAction(title: "取消", style: .cancel)
        let confirmAction = UIAlertAction(title: "切换", style: .default) { [weak self] _ in
            // 通知代理切换游戏
            self?.delegate?.didSelectGame(key: game.key, value: game.value)
            self?.dismiss(animated: true)
        }

        alert.addAction(cancelAction)
        alert.addAction(confirmAction)

        present(alert, animated: true)
    }
}

// MARK: - UICollectionViewDelegateFlowLayout
extension HDGameSelectionViewController: UICollectionViewDelegateFlowLayout {

    func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, sizeForItemAt indexPath: IndexPath) -> CGSize {
        let sections = isFiltering ? filteredSections : GameListSections
        let gameName = sections[indexPath.section].games[indexPath.item].value

        // 计算文字宽度，考虑两行显示
        let font = UIFont.systemFont(ofSize: 12, weight: .medium)
        let textSize = gameName.size(withAttributes: [.font: font])

        // 设置最小宽度和最大宽度，让游戏名称完整显示
        let minWidth: CGFloat = 100
        let sectionInsets = (collectionViewLayout as? UICollectionViewFlowLayout)?.sectionInset ?? UIEdgeInsets.zero
        let availableWidth = collectionView.frame.width - sectionInsets.left - sectionInsets.right - 20 // 减去边距和间距
        let maxWidth: CGFloat = availableWidth * 0.8
        let cellWidth = max(minWidth, min(maxWidth, textSize.width + 10))

        return CGSize(width: cellWidth, height: 44)
    }

    func collectionView(_ collectionView: UICollectionView, layout collectionViewLayout: UICollectionViewLayout, referenceSizeForHeaderInSection section: Int) -> CGSize {
        return CGSize(width: collectionView.frame.width, height: 0)
    }
}

// MARK: - UISearchResultsUpdating
extension HDGameSelectionViewController: UISearchResultsUpdating {
    func updateSearchResults(for searchController: UISearchController) {
        let searchText = searchController.searchBar.text ?? ""
        filterContentForSearchText(searchText)
    }
}
