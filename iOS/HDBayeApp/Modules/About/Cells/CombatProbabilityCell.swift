import UIKit

class CombatProbabilityCell: UITableViewCell {
    static let identifier = "CombatProbabilityCell"
    
    var probabilityValueChanged: ((Int) -> Void)?
    
    private let titleLabel: UILabel = {
        let label = UILabel()
        label.text = "遇敌概率(默认20%)"
        label.font = UIFont.systemFont(ofSize: 16)
        return label
    }()
    
    private let slider: UISlider = {
        let slider = UISlider()
        slider.minimumValue = 1
        slider.maximumValue = 99
        slider.value = 20 // 默认值20
        return slider
    }()
    
    private let valueLabel: UILabel = {
        let label = UILabel()
        label.text = "20%"
        label.font = UIFont.systemFont(ofSize: 12)
        label.textColor = .secondaryLabel
        label.textAlignment = .right
        return label
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
        contentView.addSubview(slider)
        contentView.addSubview(valueLabel)
        
        titleLabel.translatesAutoresizingMaskIntoConstraints = false
        slider.translatesAutoresizingMaskIntoConstraints = false
        valueLabel.translatesAutoresizingMaskIntoConstraints = false
        
        NSLayoutConstraint.activate([
            titleLabel.leadingAnchor.constraint(equalTo: contentView.leadingAnchor, constant: HDConstants.UIContant.leadingAnchor),
            titleLabel.centerYAnchor.constraint(equalTo: contentView.centerYAnchor),
            titleLabel.widthAnchor.constraint(equalToConstant: 150),
            
            valueLabel.trailingAnchor.constraint(equalTo: contentView.trailingAnchor, constant: HDConstants.UIContant.trailingAnchor),
            valueLabel.centerYAnchor.constraint(equalTo: contentView.centerYAnchor),
            valueLabel.widthAnchor.constraint(equalToConstant: 35),
            
            slider.leadingAnchor.constraint(equalTo: titleLabel.trailingAnchor, constant: 4),
            slider.trailingAnchor.constraint(equalTo: valueLabel.leadingAnchor, constant: -4),
            slider.centerYAnchor.constraint(equalTo: contentView.centerYAnchor)
        ])
        
        slider.addTarget(self, action: #selector(sliderValueChanged), for: .valueChanged)
    }
    
    @objc private func sliderValueChanged() {
        let value = Int(slider.value)
        let actualProbability = value  // 使用100-输入值的逻辑
        valueLabel.text = "\(actualProbability)%"
        probabilityValueChanged?(value)
    }
    
    func configure(with probability: Int) {
        slider.value = Float(probability)
        let actualProbability = probability
        valueLabel.text = "\(actualProbability)%"
    }
}
