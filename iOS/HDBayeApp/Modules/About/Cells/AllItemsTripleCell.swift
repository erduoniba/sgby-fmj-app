import UIKit

class AllItemsTripleCell: UITableViewCell {
    static let identifier = "AllItemsTripleCell"

    var onPurchaseTapped: (() -> Void)?
    var onAddToSaveTapped: (() -> Void)?
    private var isPurchased: Bool = false

    private let titleLabel: UILabel = {
        let label = UILabel()
        label.text = "获取全物品"
        label.font = UIFont.systemFont(ofSize: 16)
        return label
    }()

    private let purchaseButton: UIButton = {
        let button = UIButton(type: .system)
        button.setTitle("购买", for: .normal)
        button.titleLabel?.font = UIFont.systemFont(ofSize: 14)
        button.layer.cornerRadius = 8
        button.layer.borderWidth = 1
        button.layer.borderColor = UIColor.systemBlue.cgColor
        button.contentEdgeInsets = UIEdgeInsets(top: 6, left: 12, bottom: 6, right: 12)
        return button
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
        contentView.addSubview(purchaseButton)

        titleLabel.translatesAutoresizingMaskIntoConstraints = false
        purchaseButton.translatesAutoresizingMaskIntoConstraints = false

        NSLayoutConstraint.activate([
            // Title label 约束
            titleLabel.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: HDConstants.UIContant.leadingAnchor),
            titleLabel.centerYAnchor.constraint(equalTo: contentView.centerYAnchor),

            // Purchase button 约束
            purchaseButton.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: HDConstants.UIContant.trailingAnchor),
            purchaseButton.centerYAnchor.constraint(equalTo: contentView.centerYAnchor),
            purchaseButton.heightAnchor.constraint(equalToConstant: 32),

            // 添加垂直方向的约束来定义 contentView 的高度
            titleLabel.topAnchor.constraint(greaterThanOrEqualTo: contentView.topAnchor, constant: 8),
            titleLabel.bottomAnchor.constraint(lessThanOrEqualTo: contentView.bottomAnchor, constant: -8),
            purchaseButton.topAnchor.constraint(greaterThanOrEqualTo: contentView.topAnchor, constant: 8),
            purchaseButton.bottomAnchor.constraint(lessThanOrEqualTo: contentView.bottomAnchor, constant: -8)
        ])

        purchaseButton.addTarget(self, action: #selector(buttonTapped), for: .touchUpInside)
    }

    @objc private func buttonTapped() {
        if isPurchased {
            onAddToSaveTapped?()
        } else {
            onPurchaseTapped?()
        }
    }

    func configure(isPurchased: Bool) {
        self.isPurchased = isPurchased

        if isPurchased {
            purchaseButton.setTitle("添加到存档", for: .normal)
            purchaseButton.backgroundColor = .systemGreen.withAlphaComponent(0.1)
            purchaseButton.layer.borderColor = UIColor.systemGreen.cgColor
            purchaseButton.setTitleColor(.systemGreen, for: .normal)
        } else {
            purchaseButton.setTitle("购买", for: .normal)
            purchaseButton.backgroundColor = .systemBlue.withAlphaComponent(0.1)
            purchaseButton.layer.borderColor = UIColor.systemBlue.cgColor
            purchaseButton.setTitleColor(.systemBlue, for: .normal)
        }
    }
}
