import 'package:flutter/material.dart';
import 'package:package_info_plus/package_info_plus.dart';
import 'app_config.dart';
import 'log_utils.dart';

class VersionChecker {
  static final VersionChecker _instance = VersionChecker._internal();
  factory VersionChecker() => _instance;
  VersionChecker._internal();

  static VersionChecker get shared => _instance;

  /// Check for app updates from App Store if needed
  Future<void> checkForUpdateIfNeeded() async {
    final config = AppConfig.shared;
    
    // Only check if launch count is above threshold
    if (config.appLaunchCount < 3) {
      return;
    }

    // Check if already checked today
    final today = DateTime.now().toIso8601String().split('T')[0];
    if (config.lastVersionCheck == today) {
      return;
    }

    try {
      final hasUpdate = await _checkAppStoreVersion();
      if (hasUpdate) {
        await _showUpdateDialog();
      }
      
      // Update last check date and reset launch count
      config.lastVersionCheck = today;
      config.resetAppLaunchCount();
    } catch (e) {
      LogUtils.d('Version check failed: $e');
    }
  }

  /// Check App Store for newer version
  Future<bool> _checkAppStoreVersion() async {
    try {
      final packageInfo = await PackageInfo.fromPlatform();
      final currentVersion = packageInfo.version;
      
      // For now, return false as we can't easily check App Store API
      // In a real implementation, you would call iTunes Search API
      LogUtils.d('Current version: $currentVersion');
      return false;
    } catch (e) {
      LogUtils.d('Failed to check App Store version: $e');
      return false;
    }
  }

  /// Show version upgrade alert dialog
  Future<void> _showUpdateDialog() async {
    // This would need to be called from a context where BuildContext is available
    // For now, just log the action
    LogUtils.d('Should show update dialog');
  }

  /// Show version upgrade alert for specific version
  Future<void> checkAndShowVersionUpgradeAlert(BuildContext context) async {
    final config = AppConfig.shared;
    final packageInfo = await PackageInfo.fromPlatform();
    final currentVersion = packageInfo.version;
    const targetVersion = "2.0.0";
    
    // Only show if current version matches target and not shown before
    if (currentVersion != targetVersion) return;
    if (config.hasShownUpgradeAlert(targetVersion)) return;
    
    // Show upgrade dialog
    if (!context.mounted) return;
    await showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: const Text('🎉 伏魔记引擎重大升级 🎉'),
          content: const Text('''新版本带来全新游戏体验：

🗺️ 地图高清化支持
• 游戏地图显示更加清晰细腻

🔍 智能地图功能
• 支持显示隐藏物品位置
• 显示触发事件点位

📍 实时位置追踪
• 查看全地图和人物位置
• 实时更新人物位置信息

🚫 去广告功能
• 更纯净的游戏体验

立即体验全新的伏魔记世界！'''),
          actions: [
            TextButton(
              onPressed: () {
                config.markUpgradeAlertShown(targetVersion);
                Navigator.of(context).pop();
                
                // Show confetti animation (if available)
                _showUpgradeCompletedAnimation(context);
              },
              child: const Text('立即体验'),
            ),
          ],
        );
      },
    );
  }

  /// Show upgrade completed animation
  void _showUpgradeCompletedAnimation(BuildContext context) {
    // Show a snackbar as confetti animation substitute
    ScaffoldMessenger.of(context).showSnackBar(
      const SnackBar(
        content: Text('🎉 恭喜您，升级完成！'),
        duration: Duration(seconds: 3),
      ),
    );
  }
}