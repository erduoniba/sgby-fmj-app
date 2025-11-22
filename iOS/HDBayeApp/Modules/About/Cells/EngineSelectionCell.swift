import UIKit

class EngineSelectionCell: UITableViewCell {
    static let identifier = "EngineSelectionCell"
    
    var engineValueChanged: ((Bool, @escaping (Bool) -> Void) -> Void)?
    
    private let titleLabel: UILabel = {
        let label = UILabel()
        label.text = "新引擎"
        label.font = UIFont.systemFont(ofSize: 16)
        return label
    }()
    
    private let segmentedControl: UISegmentedControl = {
        let items = ["旧版", "新版"]
        let control = UISegmentedControl(items: items)
        control.selectedSegmentIndex = 0 // 默认选中旧版
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
            titleLabel.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: HDConstants.UIContant.leadingAnchor),
            titleLabel.centerYAnchor.constraint(equalTo: contentView.centerYAnchor),
            
            segmentedControl.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: HDConstants.UIContant.trailingAnchor),
            segmentedControl.centerYAnchor.constraint(equalTo: contentView.centerYAnchor),
            segmentedControl.widthAnchor.constraint(equalToConstant: 120)
        ])
        
        segmentedControl.addTarget(self, action: #selector(segmentedControlValueChanged), for: .valueChanged)
    }
    
    @objc private func segmentedControlValueChanged() {
        let useNewEngine = segmentedControl.selectedSegmentIndex == 1
        engineValueChanged?(useNewEngine) { [weak self] confirmed in
            if !confirmed {
                // 如果用户取消，恢复之前的选择
                DispatchQueue.main.async {
                    self?.segmentedControl.selectedSegmentIndex = useNewEngine ? 0 : 1
                }
            }
        }
    }
    
    func configure(with useNewEngine: Bool) {
        segmentedControl.selectedSegmentIndex = useNewEngine ? 1 : 0
    }
}
