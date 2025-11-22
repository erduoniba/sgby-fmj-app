import UIKit

class GameSpeedCell: UITableViewCell {
    static let identifier = "GameSpeedCell"
    
    var speedValueChanged: ((Float) -> Void)?
    private let speedValues: [Float] = [0.5, 1.0, 2.0, 3.0]
    
    private let titleLabel: UILabel = {
        let label = UILabel()
        label.text = "变速器"
        label.font = UIFont.systemFont(ofSize: 16)
        return label
    }()
    
    private let segmentedControl: UISegmentedControl = {
        let items = ["0.5x", "1x", "2x", "3x"]
        let control = UISegmentedControl(items: items)
        control.selectedSegmentIndex = 1 // 默认选中1x
        return control
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
        
        titleLabel.translatesAutoresizingMaskIntoConstraints = false
        segmentedControl.translatesAutoresizingMaskIntoConstraints = false
        
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
            titleLabel.bottomAnchor.constraint(lessThanOrEqualTo: contentView.bottomAnchor, constant: -8)
        ])
        
        segmentedControl.addTarget(self, action: #selector(segmentedControlValueChanged), for: .valueChanged)
    }
    
    @objc private func segmentedControlValueChanged() {
        let selectedSpeed = speedValues[segmentedControl.selectedSegmentIndex]
        speedValueChanged?(selectedSpeed)
    }
    
    func configure(with speed: Float) {
        if let index = speedValues.firstIndex(of: speed) {
            segmentedControl.selectedSegmentIndex = index
        }
    }
} 
