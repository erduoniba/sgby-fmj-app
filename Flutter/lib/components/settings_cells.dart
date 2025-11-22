import 'package:flutter/material.dart';
import '../services/orientation_service.dart';


class ColorFilterCell extends StatefulWidget {
  final String? initialValue;
  final ValueChanged<String> onChanged;

  const ColorFilterCell({
    super.key,
    required this.initialValue,
    required this.onChanged,
  });

  @override
  State<ColorFilterCell> createState() => _ColorFilterCellState();
}

class _ColorFilterCellState extends State<ColorFilterCell> {
  final List<String> _filters = [
    "none",
    "vintage1980",
    "refreshing"
  ];

  final List<String> _filterNames = [
    "无",
    "复古",
    "清爽"
  ];

  late String _selectedFilter;

  @override
  void initState() {
    super.initState();
    // 处理旧的滤镜值，映射到新的滤镜系统
    String initialFilter = widget.initialValue ?? "none";
    if (initialFilter == "reset") {
      initialFilter = "none";
    } else if (!_filters.contains(initialFilter)) {
      // 如果是不支持的滤镜（如green、red），默认设为none
      initialFilter = "none";
    }
    _selectedFilter = initialFilter;
  }

  @override
  Widget build(BuildContext context) {
    return Card(
      margin: const EdgeInsets.symmetric(horizontal: 16, vertical: 4),
      child: Padding(
        padding: const EdgeInsets.all(10),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text(
              '滤镜效果',
              style: TextStyle(fontSize: 16, fontWeight: FontWeight.w500),
            ),
            const SizedBox(height: 12),
            Container(
              decoration: BoxDecoration(
                color: Colors.grey[200],
                borderRadius: BorderRadius.circular(8),
              ),
              child: Row(
                children: _filters.asMap().entries.map((entry) {
                  final index = entry.key;
                  final filter = entry.value;
                  final name = _filterNames[index];
                  final isSelected = _selectedFilter == filter;
                  
                  return Expanded(
                    child: GestureDetector(
                      onTap: () {
                        setState(() {
                          _selectedFilter = filter;
                        });
                        widget.onChanged(filter);
                      },
                      child: Container(
                        padding: const EdgeInsets.symmetric(vertical: 12, horizontal: 8),
                        decoration: BoxDecoration(
                          color: isSelected ? Theme.of(context).primaryColor : Colors.transparent,
                          borderRadius: BorderRadius.circular(6),
                          border: isSelected ? null : Border.all(color: Colors.transparent),
                        ),
                        child: Text(
                          name,
                          textAlign: TextAlign.center,
                          style: TextStyle(
                            color: isSelected ? Colors.white : Colors.black87,
                            fontSize: 13,
                            fontWeight: isSelected ? FontWeight.w600 : FontWeight.normal,
                          ),
                        ),
                      ),
                    ),
                  );
                }).toList(),
              ),
            ),
          ],
        ),
      ),
    );
  }
}

class GameSpeedCell extends StatefulWidget {
  final double initialValue;
  final ValueChanged<double> onChanged;

  const GameSpeedCell({
    super.key,
    required this.initialValue,
    required this.onChanged,
  });

  @override
  State<GameSpeedCell> createState() => _GameSpeedCellState();
}

class _GameSpeedCellState extends State<GameSpeedCell> {
  late double _currentValue;

  @override
  void initState() {
    super.initState();
    _currentValue = widget.initialValue;
  }

  @override
  Widget build(BuildContext context) {
    return Card(
      margin: const EdgeInsets.symmetric(horizontal: 16, vertical: 4),
      child: Padding(
        padding: const EdgeInsets.all(10),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text(
              '游戏速度: ${_currentValue.toStringAsFixed(1)}x',
              style: const TextStyle(fontSize: 16, fontWeight: FontWeight.w500),
            ),
            const SizedBox(height: 12),
            Slider(
              value: _currentValue,
              min: 0.5,
              max: 3.0,
              divisions: 25,
              onChanged: (value) {
                setState(() {
                  _currentValue = value;
                });
                widget.onChanged(value);
              },
            ),
          ],
        ),
      ),
    );
  }
}

class PortraitModeCell extends StatefulWidget {
  final bool initialValue;
  final ValueChanged<bool> onChanged;

  const PortraitModeCell({
    super.key,
    required this.initialValue,
    required this.onChanged,
  });

  @override
  State<PortraitModeCell> createState() => _PortraitModeCellState();
}

class _PortraitModeCellState extends State<PortraitModeCell> {
  late bool _isPortrait;

  @override
  void initState() {
    super.initState();
    _isPortrait = widget.initialValue;
  }

  @override
  Widget build(BuildContext context) {
    return Card(
      margin: const EdgeInsets.symmetric(horizontal: 16, vertical: 4),
      child: Padding(
        padding: const EdgeInsets.all(10),
        child: Row(
          children: [
            Expanded(
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  const Text(
                    '屏幕方向',
                    style: TextStyle(fontSize: 16, fontWeight: FontWeight.w500),
                  ),
                  Text(
                    _isPortrait ? '竖屏模式' : '横屏模式',
                    style: TextStyle(fontSize: 12, color: Colors.grey[600]),
                  ),
                ],
              ),
            ),
            Switch(
              value: _isPortrait,
              onChanged: (value) async {
                setState(() {
                  _isPortrait = value;
                });

                // 更新系统方向
                await OrientationService.instance.setOrientationMode(value);

                // 通知回调
                widget.onChanged(value);
              },
            ),
          ],
        ),
      ),
    );
  }
}

class CombatProbabilityCell extends StatefulWidget {
  final int initialValue;
  final ValueChanged<int> onChanged;

  const CombatProbabilityCell({
    super.key,
    required this.initialValue,
    required this.onChanged,
  });

  @override
  State<CombatProbabilityCell> createState() => _CombatProbabilityCellState();
}

class _CombatProbabilityCellState extends State<CombatProbabilityCell> {
  late int _currentValue;

  @override
  void initState() {
    super.initState();
    _currentValue = widget.initialValue;
  }

  @override
  Widget build(BuildContext context) {
    return Card(
      margin: const EdgeInsets.symmetric(horizontal: 16, vertical: 4),
      child: Padding(
        padding: const EdgeInsets.all(10),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text(
              '遭遇概率: $_currentValue%',
              style: const TextStyle(fontSize: 16, fontWeight: FontWeight.w500),
            ),
            const SizedBox(height: 8),
            const Text(
              '调整随机遭遇战斗的概率（数值越小遭遇越少）',
              style: TextStyle(fontSize: 12, color: Colors.grey),
            ),
            const SizedBox(height: 12),
            Slider(
              value: _currentValue.toDouble(),
              min: 1,
              max: 99,
              divisions: 98,
              onChanged: (value) {
                setState(() {
                  _currentValue = value.round();
                });
                widget.onChanged(_currentValue);
              },
            ),
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                Text(
                  '1% (很少)',
                  style: TextStyle(fontSize: 10, color: Colors.grey[600]),
                ),
                Text(
                  '99% (频繁)',
                  style: TextStyle(fontSize: 10, color: Colors.grey[600]),
                ),
              ],
            ),
          ],
        ),
      ),
    );
  }
}
// ==================== 倍率设置 Cell ====================

/// 经验倍率选择Cell
class ExpMultiplierCell extends StatefulWidget {
  final int initialValue;
  final ValueChanged<int> onChanged;

  const ExpMultiplierCell({
    super.key,
    required this.initialValue,
    required this.onChanged,
  });

  @override
  State<ExpMultiplierCell> createState() => _ExpMultiplierCellState();
}

class _ExpMultiplierCellState extends State<ExpMultiplierCell> {
  final List<int> _multiplierValues = [1, 2, 5, 10];
  final List<String> _multiplierNames = ["1x", "2x", "5x", "10x"];
  late int _selectedMultiplier;

  @override
  void initState() {
    super.initState();
    _selectedMultiplier = widget.initialValue;
    // 如果值不在列表中，默认选择5x
    if (!_multiplierValues.contains(_selectedMultiplier)) {
      _selectedMultiplier = 5;
    }
  }

  @override
  Widget build(BuildContext context) {
    return Card(
      margin: const EdgeInsets.symmetric(horizontal: 16, vertical: 4),
      child: Padding(
        padding: const EdgeInsets.all(10),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text(
              '经验倍率',
              style: TextStyle(fontSize: 16, fontWeight: FontWeight.w500),
            ),
            const SizedBox(height: 12),
            Container(
              decoration: BoxDecoration(
                color: Colors.grey[200],
                borderRadius: BorderRadius.circular(8),
              ),
              child: Row(
                children: _multiplierValues.asMap().entries.map((entry) {
                  final index = entry.key;
                  final multiplier = entry.value;
                  final name = _multiplierNames[index];
                  final isSelected = _selectedMultiplier == multiplier;

                  return Expanded(
                    child: GestureDetector(
                      onTap: () {
                        setState(() {
                          _selectedMultiplier = multiplier;
                        });
                        widget.onChanged(multiplier);
                      },
                      child: Container(
                        padding: const EdgeInsets.symmetric(vertical: 12, horizontal: 8),
                        decoration: BoxDecoration(
                          color: isSelected ? Theme.of(context).primaryColor : Colors.transparent,
                          borderRadius: BorderRadius.circular(6),
                          border: isSelected ? null : Border.all(color: Colors.transparent),
                        ),
                        child: Text(
                          name,
                          textAlign: TextAlign.center,
                          style: TextStyle(
                            color: isSelected ? Colors.white : Colors.black87,
                            fontSize: 13,
                            fontWeight: isSelected ? FontWeight.w600 : FontWeight.normal,
                          ),
                        ),
                      ),
                    ),
                  );
                }).toList(),
              ),
            ),
          ],
        ),
      ),
    );
  }
}

/// 金币倍率选择Cell
class GoldMultiplierCell extends StatefulWidget {
  final int initialValue;
  final ValueChanged<int> onChanged;

  const GoldMultiplierCell({
    super.key,
    required this.initialValue,
    required this.onChanged,
  });

  @override
  State<GoldMultiplierCell> createState() => _GoldMultiplierCellState();
}

class _GoldMultiplierCellState extends State<GoldMultiplierCell> {
  final List<int> _multiplierValues = [1, 2, 5, 10];
  final List<String> _multiplierNames = ["1x", "2x", "5x", "10x"];
  late int _selectedMultiplier;

  @override
  void initState() {
    super.initState();
    _selectedMultiplier = widget.initialValue;
    // 如果值不在列表中，默认选择5x
    if (!_multiplierValues.contains(_selectedMultiplier)) {
      _selectedMultiplier = 5;
    }
  }

  @override
  Widget build(BuildContext context) {
    return Card(
      margin: const EdgeInsets.symmetric(horizontal: 16, vertical: 4),
      child: Padding(
        padding: const EdgeInsets.all(10),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text(
              '金币倍率',
              style: TextStyle(fontSize: 16, fontWeight: FontWeight.w500),
            ),
            const SizedBox(height: 12),
            Container(
              decoration: BoxDecoration(
                color: Colors.grey[200],
                borderRadius: BorderRadius.circular(8),
              ),
              child: Row(
                children: _multiplierValues.asMap().entries.map((entry) {
                  final index = entry.key;
                  final multiplier = entry.value;
                  final name = _multiplierNames[index];
                  final isSelected = _selectedMultiplier == multiplier;

                  return Expanded(
                    child: GestureDetector(
                      onTap: () {
                        setState(() {
                          _selectedMultiplier = multiplier;
                        });
                        widget.onChanged(multiplier);
                      },
                      child: Container(
                        padding: const EdgeInsets.symmetric(vertical: 12, horizontal: 8),
                        decoration: BoxDecoration(
                          color: isSelected ? Theme.of(context).primaryColor : Colors.transparent,
                          borderRadius: BorderRadius.circular(6),
                          border: isSelected ? null : Border.all(color: Colors.transparent),
                        ),
                        child: Text(
                          name,
                          textAlign: TextAlign.center,
                          style: TextStyle(
                            color: isSelected ? Colors.white : Colors.black87,
                            fontSize: 13,
                            fontWeight: isSelected ? FontWeight.w600 : FontWeight.normal,
                          ),
                        ),
                      ),
                    ),
                  );
                }).toList(),
              ),
            ),
          ],
        ),
      ),
    );
  }
}

/// 物品倍率选择Cell
class ItemMultiplierCell extends StatefulWidget {
  final int initialValue;
  final ValueChanged<int> onChanged;

  const ItemMultiplierCell({
    super.key,
    required this.initialValue,
    required this.onChanged,
  });

  @override
  State<ItemMultiplierCell> createState() => _ItemMultiplierCellState();
}

class _ItemMultiplierCellState extends State<ItemMultiplierCell> {
  final List<int> _multiplierValues = [1, 2, 5, 10];
  final List<String> _multiplierNames = ["1x", "2x", "5x", "10x"];
  late int _selectedMultiplier;

  @override
  void initState() {
    super.initState();
    _selectedMultiplier = widget.initialValue;
    // 兼容旧的3倍，映射到2倍
    if (_selectedMultiplier == 3) {
      _selectedMultiplier = 2;
    }
    // 如果值不在列表中，默认选择2x
    if (!_multiplierValues.contains(_selectedMultiplier)) {
      _selectedMultiplier = 2;
    }
  }

  @override
  Widget build(BuildContext context) {
    return Card(
      margin: const EdgeInsets.symmetric(horizontal: 16, vertical: 4),
      child: Padding(
        padding: const EdgeInsets.all(10),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text(
              '物品掉落倍率',
              style: TextStyle(fontSize: 16, fontWeight: FontWeight.w500),
            ),
            const SizedBox(height: 12),
            Container(
              decoration: BoxDecoration(
                color: Colors.grey[200],
                borderRadius: BorderRadius.circular(8),
              ),
              child: Row(
                children: _multiplierValues.asMap().entries.map((entry) {
                  final index = entry.key;
                  final multiplier = entry.value;
                  final name = _multiplierNames[index];
                  final isSelected = _selectedMultiplier == multiplier;

                  return Expanded(
                    child: GestureDetector(
                      onTap: () {
                        setState(() {
                          _selectedMultiplier = multiplier;
                        });
                        widget.onChanged(multiplier);
                      },
                      child: Container(
                        padding: const EdgeInsets.symmetric(vertical: 12, horizontal: 8),
                        decoration: BoxDecoration(
                          color: isSelected ? Theme.of(context).primaryColor : Colors.transparent,
                          borderRadius: BorderRadius.circular(6),
                          border: isSelected ? null : Border.all(color: Colors.transparent),
                        ),
                        child: Text(
                          name,
                          textAlign: TextAlign.center,
                          style: TextStyle(
                            color: isSelected ? Colors.white : Colors.black87,
                            fontSize: 13,
                            fontWeight: isSelected ? FontWeight.w600 : FontWeight.normal,
                          ),
                        ),
                      ),
                    ),
                  );
                }).toList(),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
