import 'package:flutter/services.dart';
import '../utils/log_utils.dart';

/// 屏幕方向管理服务
class OrientationService {
  static final OrientationService _instance = OrientationService._internal();
  static OrientationService get instance => _instance;
  OrientationService._internal();

  /// 当前是否为竖屏模式
  bool _isPortraitMode = false;
  bool get isPortraitMode => _isPortraitMode;

  /// 初始化方向服务（默认为竖屏）
  Future<void> initialize({bool portraitMode = true}) async {
    _isPortraitMode = portraitMode;
    await _updateSystemOrientation();
    LogUtils.d('OrientationService initialized: portraitMode=$portraitMode');
  }

  /// 切换到竖屏模式
  Future<void> setPortraitMode() async {
    if (_isPortraitMode) return;

    _isPortraitMode = true;
    await _updateSystemOrientation();
    LogUtils.d('Switched to portrait mode');
  }

  /// 切换到横屏模式
  Future<void> setLandscapeMode() async {
    if (!_isPortraitMode) return;

    _isPortraitMode = false;
    await _updateSystemOrientation();
    LogUtils.d('Switched to landscape mode');
  }

  /// 切换方向
  Future<void> toggleOrientation() async {
    if (_isPortraitMode) {
      await setLandscapeMode();
    } else {
      await setPortraitMode();
    }
  }

  /// 设置方向模式
  Future<void> setOrientationMode(bool portraitMode) async {
    if (portraitMode) {
      await setPortraitMode();
    } else {
      await setLandscapeMode();
    }
  }

  /// 更新系统方向设置
  Future<void> _updateSystemOrientation() async {
    try {
      if (_isPortraitMode) {
        // 设置为竖屏
        await SystemChrome.setPreferredOrientations([
          DeviceOrientation.portraitUp,
          DeviceOrientation.portraitDown,
        ]);
      } else {
        // 设置为横屏
        await SystemChrome.setPreferredOrientations([
          DeviceOrientation.landscapeLeft,
          DeviceOrientation.landscapeRight,
        ]);
      }
    } catch (e) {
      LogUtils.d('Failed to update system orientation: $e');
    }
  }

  /// 允许所有方向（用于完全自由旋转）
  Future<void> enableAllOrientations() async {
    try {
      await SystemChrome.setPreferredOrientations([
        DeviceOrientation.portraitUp,
        DeviceOrientation.portraitDown,
        DeviceOrientation.landscapeLeft,
        DeviceOrientation.landscapeRight,
      ]);
      LogUtils.d('All orientations enabled');
    } catch (e) {
      LogUtils.d('Failed to enable all orientations: $e');
    }
  }

  /// 获取方向描述文本
  String get orientationDescription {
    return _isPortraitMode ? '竖屏' : '横屏';
  }

  /// 同步当前方向状态（用于从外部设置更新）
  void syncOrientationState(bool portraitMode) {
    _isPortraitMode = portraitMode;
    LogUtils.d('Orientation state synced: portraitMode=$portraitMode');
  }
}