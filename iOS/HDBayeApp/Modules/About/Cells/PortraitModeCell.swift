import UIKit

enum OrientationMode: String, CaseIterable {
    case followSystem = "auto"
    case landscape = "landscape"
    case portrait = "portrait"

    var displayTitle: String {
        switch self {
        case .followSystem: return "系统"
        case .landscape: return "横屏"
        case .portrait: return "竖屏"
        }
    }
}

class PortraitModeCell: UITableViewCell {
    static let identifier = "PortraitModeCell"

    var orientationValueChanged: ((OrientationMode) -> Void)?

    private let titleLabel: UILabel = {
        let label = UILabel()
        label.text = "横竖屏模式"
        label.font = UIFont.systemFont(ofSize: 16)
        return label
    }()

    private let segmentedControl: UISegmentedControl = {
        let items = OrientationMode.allCases.map { $0.displayTitle }
        let control = UISegmentedControl(items: items)
        control.selectedSegmentIndex = 0 // 默认选中跟随系统
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
            segmentedControl.widthAnchor.constraint(equalToConstant: 180)
        ])

        segmentedControl.addTarget(self, action: #selector(segmentedControlValueChanged), for: .valueChanged)
    }

    @objc private func segmentedControlValueChanged() {
        let selectedMode = OrientationMode.allCases[segmentedControl.selectedSegmentIndex]
        orientationValueChanged?(selectedMode)
    }

    func configure(with orientationMode: OrientationMode) {
        if let index = OrientationMode.allCases.firstIndex(of: orientationMode) {
            segmentedControl.selectedSegmentIndex = index
        } else {
            segmentedControl.selectedSegmentIndex = 0 // 默认跟随系统
        }
    }
}
