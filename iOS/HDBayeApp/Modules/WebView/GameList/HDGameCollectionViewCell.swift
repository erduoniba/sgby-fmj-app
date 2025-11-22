//
//  HDGameCollectionViewCell.swift
//  HDBayeApp
//
//  Created by harrydeng on 2025/9/14.
//

import UIKit

class HDGameCollectionViewCell: UICollectionViewCell {

    static let identifier = "HDGameCollectionViewCell"

    private let containerView: UIView = {
        let view = UIView()
        view.translatesAutoresizingMaskIntoConstraints = false
        return view
    }()

    private let gameLabel: UILabel = {
        let label = UILabel()
        label.font = UIFont.systemFont(ofSize: 12, weight: .regular)
        label.textColor = .label
        label.numberOfLines = 2
        label.textAlignment = .center
        label.translatesAutoresizingMaskIntoConstraints = false
        return label
    }()


    override init(frame: CGRect) {
        super.init(frame: frame)
        setupUI()
    }

    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    private func setupUI() {
        // 设置阴影
        layer.shadowColor = UIColor.black.cgColor
        layer.shadowOffset = CGSize(width: 0, height: 2)
        layer.shadowRadius = 4
        layer.shadowOpacity = 0.1

        // 添加容器视图
        contentView.addSubview(containerView)
        containerView.addSubview(gameLabel)

        // 设置容器样式
        containerView.layer.cornerRadius = 12
        containerView.layer.masksToBounds = true
        containerView.backgroundColor = .systemBackground

        NSLayoutConstraint.activate([
            containerView.topAnchor.constraint(equalTo: contentView.topAnchor),
            containerView.leadingAnchor.constraint(equalTo: contentView.leadingAnchor),
            containerView.trailingAnchor.constraint(equalTo: contentView.trailingAnchor),
            containerView.bottomAnchor.constraint(equalTo: contentView.bottomAnchor),

            gameLabel.leadingAnchor.constraint(equalTo: containerView.leadingAnchor, constant: 0),
            gameLabel.trailingAnchor.constraint(equalTo: containerView.trailingAnchor, constant: 0),
            gameLabel.centerYAnchor.constraint(equalTo: containerView.centerYAnchor)
        ])
    }


    override var isHighlighted: Bool {
        didSet {
            UIView.animate(withDuration: 0.15, delay: 0, usingSpringWithDamping: 0.8, initialSpringVelocity: 0.5) {
                self.transform = self.isHighlighted ? CGAffineTransform(scaleX: 0.96, y: 0.96) : .identity
            }
        }
    }

    func configure(with gameName: String, isSelected: Bool) {
        gameLabel.text = gameName
        self.isSelected = isSelected
        updateAppearance(isSelected: isSelected)
    }

    private func updateAppearance(isSelected: Bool) {
        if isSelected {
            // 使用纯色背景替代渐变，确保文字可见
            containerView.backgroundColor = .systemBlue

            // 设置白色文字和加粗字体
            gameLabel.textColor = .white
            gameLabel.font = UIFont.systemFont(ofSize: 12, weight: .bold)

            // 添加文字阴影增强可读性
            gameLabel.layer.shadowColor = UIColor.black.cgColor
            gameLabel.layer.shadowOffset = CGSize(width: 0, height: 1)
            gameLabel.layer.shadowRadius = 1
            gameLabel.layer.shadowOpacity = 0.3

            // 增强Cell阴影
            layer.shadowOpacity = 0.25
            layer.shadowRadius = 6
            layer.shadowColor = UIColor.systemBlue.withAlphaComponent(0.3).cgColor
        } else {
            containerView.backgroundColor = .secondarySystemBackground
            gameLabel.textColor = .label
            gameLabel.font = UIFont.systemFont(ofSize: 12, weight: .regular)

            // 移除文字阴影
            gameLabel.layer.shadowOpacity = 0

            // 恢复普通阴影
            layer.shadowOpacity = 0.1
            layer.shadowRadius = 4
            layer.shadowColor = UIColor.black.cgColor
        }
    }
}
