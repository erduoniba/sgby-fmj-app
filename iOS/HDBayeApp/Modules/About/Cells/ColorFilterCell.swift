import UIKit

class ColorFilterCell: UITableViewCell {
    static let identifier = "ColorFilterCell"
    
    private let titleLabel: UILabel = {
        let label = UILabel()
        label.text = "滤镜效果"
        label.font = UIFont.systemFont(ofSize: 16)
        return label
    }()
    
    private let segmentedControl: UISegmentedControl = {
        let items = ["无", "复古", "清爽"]
        let control = UISegmentedControl(items: items)
        control.apportionsSegmentWidthsByContent = true
        return control
    }()
    
    private let filters = [
        "none",          // 无滤镜
        "vintage1980",   // 复古1980
        "refreshing"     // 清新明亮
    ]
    
    var filterValueChanged: ((String) -> Void)?
    
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
        
        segmentedControl.addTarget(self, action: #selector(segmentChanged), for: .valueChanged)
    }
    
    @objc private func segmentChanged(_ sender: UISegmentedControl) {
        let filter = filters[sender.selectedSegmentIndex]
        filterValueChanged?(filter)
        
        // 发送通知
        NotificationCenter.default.post(
            name: NSNotification.Name("ColorFilterChanged"),
            object: nil,
            userInfo: ["filter": filter]
        )
    }
    
    func configure(with filter: String?) {
        if let filter = filter, let index = filters.firstIndex(of: filter) {
            segmentedControl.selectedSegmentIndex = index
        } else {
            segmentedControl.selectedSegmentIndex = UISegmentedControl.noSegment
        }
    }
} 
