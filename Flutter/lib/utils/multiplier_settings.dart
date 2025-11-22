import 'package:shared_preferences/shared_preferences.dart';
import 'log_utils.dart';

/// 倍率设置管理类
/// 管理经验、金币、物品掉落倍率值（1x, 2x, 5x, 10x）
class MultiplierSettings {
  static final MultiplierSettings _instance = MultiplierSettings._internal();
  factory MultiplierSettings() => _instance;
  MultiplierSettings._internal();

  static const String _expMultiplierKey = 'expMultiplier';
  static const String _goldMultiplierKey = 'goldMultiplier';
  static const String _itemMultiplierKey = 'itemMultiplier';

  SharedPreferences? _prefs;

  /// 初始化
  Future<void> init() async {
    _prefs = await SharedPreferences.getInstance();
    LogUtils.d('MultiplierSettings initialized');
  }

  /// 确保已初始化
  Future<SharedPreferences> get _preferences async {
    if (_prefs == null) {
      await init();
    }
    return _prefs!;
  }

  // ==================== 经验倍率 ====================

  /// 获取经验倍率（1x, 2x, 5x, 10x）
  Future<int> get expMultiplier async {
    final prefs = await _preferences;
    // 默认5倍经验
    return prefs.getInt(_expMultiplierKey) ?? 5;
  }

  /// 设置经验倍率
  Future<void> setExpMultiplier(int multiplier) async {
    final prefs = await _preferences;
    await prefs.setInt(_expMultiplierKey, multiplier);
    LogUtils.d('MultiplierSettings: 经验倍率 = ${multiplier}x');
  }

  // ==================== 金币倍率 ====================

  /// 获取金币倍率（1x, 2x, 5x, 10x）
  Future<int> get goldMultiplier async {
    final prefs = await _preferences;
    // 默认5倍金币
    return prefs.getInt(_goldMultiplierKey) ?? 5;
  }

  /// 设置金币倍率
  Future<void> setGoldMultiplier(int multiplier) async {
    final prefs = await _preferences;
    await prefs.setInt(_goldMultiplierKey, multiplier);
    LogUtils.d('MultiplierSettings: 金币倍率 = ${multiplier}x');
  }

  // ==================== 物品倍率 ====================

  /// 获取物品倍率（1x, 2x, 5x, 10x）
  Future<int> get itemMultiplier async {
    final prefs = await _preferences;
    // 默认5倍物品（兼容旧的3倍，将3映射到2）
    final saved = prefs.getInt(_itemMultiplierKey) ?? 5;
    // 兼容旧的3倍设置
    if (saved == 3) {
      return 2;
    }
    return saved;
  }

  /// 设置物品倍率
  Future<void> setItemMultiplier(int multiplier) async {
    final prefs = await _preferences;
    await prefs.setInt(_itemMultiplierKey, multiplier);
    LogUtils.d('MultiplierSettings: 物品倍率 = ${multiplier}x');
  }

  // ==================== 同步方法（用于 WebView 注入） ====================

  /// 同步获取经验倍率（用于 WebView）
  int get expMultiplierSync {
    if (_prefs == null) return 5;
    return _prefs!.getInt(_expMultiplierKey) ?? 5;
  }

  /// 同步获取金币倍率（用于 WebView）
  int get goldMultiplierSync {
    if (_prefs == null) return 5;
    return _prefs!.getInt(_goldMultiplierKey) ?? 5;
  }

  /// 同步获取物品倍率（用于 WebView）
  int get itemMultiplierSync {
    if (_prefs == null) return 5;
    final saved = _prefs!.getInt(_itemMultiplierKey) ?? 5;
    // 兼容旧的3倍设置
    if (saved == 3) {
      return 2;
    }
    return saved;
  }

  // ==================== 工具方法 ====================

  /// 重置所有设置为默认值
  Future<void> resetToDefaults() async {
    await setExpMultiplier(5);
    await setGoldMultiplier(5);
    await setItemMultiplier(5);
    LogUtils.d('MultiplierSettings: Reset to defaults');
  }

  /// 获取所有设置（调试用）
  Future<Map<String, int>> getAllSettings() async {
    return {
      'expMultiplier': await expMultiplier,
      'goldMultiplier': await goldMultiplier,
      'itemMultiplier': await itemMultiplier,
    };
  }
}
