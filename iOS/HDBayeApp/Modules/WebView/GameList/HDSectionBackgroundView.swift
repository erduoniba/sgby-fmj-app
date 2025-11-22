//
//  HDSectionBackgroundView.swift
//  HDBayeApp
//
//  Created by harrydeng on 2025/9/14.
//

import UIKit

// MARK: - 分组背景视图
class HDSectionBackgroundView: UICollectionReusableView {

    static let identifier = "HDSectionBackgroundView"

    override init(frame: CGRect) {
        super.init(frame: frame)
        setupUI()
    }

    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    private func setupUI() {
        backgroundColor = UIColor.systemBackground.withAlphaComponent(0.8)
        layer.cornerRadius = 12
        layer.masksToBounds = true

        // 添加阴影
        layer.shadowColor = UIColor.black.cgColor
        layer.shadowOffset = CGSize(width: 0, height: 2)
        layer.shadowRadius = 4
        layer.shadowOpacity = 0.05
        layer.masksToBounds = false
    }
}