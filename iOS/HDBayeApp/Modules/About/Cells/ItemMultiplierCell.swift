import UIKit

class ItemMultiplierCell: UITableViewCell {
    static let identifier = "ItemMultiplierCell"

    var multiplierValueChanged: ((Int) -> Void)?
    var onTapWhenNotPurchased: (() -> Void)?
    private let multiplierValues: [Int] = [1, 2, 5, 10]
    private var isPurchased: Bool = true

    private let titleLabel: UILabel = {
        let label = UILabel()
        label.text = "物品掉落倍率"
        label.font = UIFont.systemFont(ofSize: 16)
        return label
    }()

    private let segmentedControl: UISegmentedControl = {
        let items = ["1x", "2x", "5x", "10x"]
        let control = UISegmentedControl(items: items)
        control.selectedSegmentIndex = 1 // 默认选中2x (对应原来的3倍物品)
        return control
    }()

    private let lockOverlayView: UIView = {
        let view = UIView()
        view.backgroundColor = UIColor.systemGray.withAlphaComponent(0.1)
        view.isHidden = true
        return view
    }()

    override init(style: UITableViewCell.CellStyle, reuseIdentifier: String?) {
        super.init(style: style, reuseIdentifier: reuseIdentifier)
        setupUI()
    }

    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    private func setupUI() {
        selectionStyle = .none

        contentView.addSubview(titleLabel)
        contentView.addSubview(segmentedControl)
        contentView.addSubview(lockOverlayView)

        titleLabel.translatesAutoresizingMaskIntoConstraints = false
        segmentedControl.translatesAutoresizingMaskIntoConstraints = false
        lockOverlayView.translatesAutoresizingMaskIntoConstraints = false

        NSLayoutConstraint.activate([
            // Title label 约束
            titleLabel.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: HDConstants.UIContant.leadingAnchor),
            titleLabel.centerYAnchor.constraint(equalTo: contentView.centerYAnchor),

            // Segmented control 约束
            segmentedControl.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: HDConstants.UIContant.trailingAnchor),
            segmentedControl.centerYAnchor.constraint(equalTo: contentView.centerYAnchor),
            segmentedControl.widthAnchor.constraint(equalToConstant: 200),

            // 添加垂直方向的约束来定义 contentView 的高度
            segmentedControl.topAnchor.constraint(greaterThanOrEqualTo: contentView.topAnchor, constant: 8),
            segmentedControl.bottomAnchor.constraint(lessThanOrEqualTo: contentView.bottomAnchor, constant: -8),
            titleLabel.topAnchor.constraint(greaterThanOrEqualTo: contentView.topAnchor, constant: 8),
            titleLabel.bottomAnchor.constraint(lessThanOrEqualTo: contentView.bottomAnchor, constant: -8),

            // 锁定遮罩层覆盖整个segmentedControl
            lockOverlayView.leadingAnchor.constraint(equalTo: segmentedControl.leadingAnchor),
            lockOverlayView.trailingAnchor.constraint(equalTo: segmentedControl.trailingAnchor),
            lockOverlayView.topAnchor.constraint(equalTo: segmentedControl.topAnchor),
            lockOverlayView.bottomAnchor.constraint(equalTo: segmentedControl.bottomAnchor)
        ])

        segmentedControl.addTarget(self, action: #selector(segmentedControlValueChanged), for: .valueChanged)

        // 为遮罩层添加点击手势
        let tapGesture = UITapGestureRecognizer(target: self, action: #selector(handleOverlayTap))
        lockOverlayView.addGestureRecognizer(tapGesture)
    }

    @objc private func handleOverlayTap() {
        if !isPurchased {
            onTapWhenNotPurchased?()
        }
    }

    @objc private func segmentedControlValueChanged() {
        let selectedMultiplier = multiplierValues[segmentedControl.selectedSegmentIndex]
        multiplierValueChanged?(selectedMultiplier)
    }

    func configure(with multiplier: Int, isPurchased: Bool = true) {
        self.isPurchased = isPurchased

        if let index = multiplierValues.firstIndex(of: multiplier) {
            segmentedControl.selectedSegmentIndex = index
        } else if multiplier == 3 {
            // 兼容旧版的3倍，映射到2倍
            segmentedControl.selectedSegmentIndex = 1
        } else {
            // 如果值不在列表中，默认选择2x
            segmentedControl.selectedSegmentIndex = 1
        }

        // 根据购买状态控制交互和视觉效果
        segmentedControl.isEnabled = isPurchased
        lockOverlayView.isHidden = isPurchased

        // 未购买时，降低透明度以显示不可用状态
        segmentedControl.alpha = isPurchased ? 1.0 : 0.5
    }
}
