import 'package:flutter/material.dart';
import 'app_config.dart';
import 'log_utils.dart';

/// 游戏切换器工具类
class GameSwitcher {
  /// 显示游戏切换确认对话框
  static Future<bool> showSwitchConfirmDialog(
    BuildContext context, 
    AppName currentGame, 
    AppName newGame
  ) async {
    final currentTitle = _getGameTitle(currentGame);
    final newTitle = _getGameTitle(newGame);
    
    final result = await showDialog<bool>(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: const Text('切换游戏'),
          content: Text('确定要从 $currentTitle 切换到 $newTitle 吗？\n\n切换游戏会重新加载内容。'),
          actions: [
            TextButton(
              onPressed: () => Navigator.of(context).pop(false),
              child: const Text('取消'),
            ),
            TextButton(
              onPressed: () => Navigator.of(context).pop(true),
              child: const Text('确定'),
            ),
          ],
        );
      },
    );
    
    return result ?? false;
  }

  /// 获取游戏标题
  static String _getGameTitle(AppName game) {
    switch (game) {
      case AppName.hdBayeApp:
        return '三国霸业';
      case AppName.hdFmjApp:
        return '伏魔记';
      default:
        return '未知游戏';
    }
  }

  /// 显示切换成功提示
  static void showSwitchSuccessMessage(BuildContext context, AppName newGame) {
    final gameTitle = _getGameTitle(newGame);
    ScaffoldMessenger.of(context).showSnackBar(
      SnackBar(
        content: Text('已切换到 $gameTitle'),
        duration: const Duration(seconds: 2),
      ),
    );
  }

  /// 记录游戏切换日志
  static void logGameSwitch(AppName fromGame, AppName toGame) {
    final fromTitle = _getGameTitle(fromGame);
    final toTitle = _getGameTitle(toGame);
    LogUtils.d('游戏切换: $fromTitle -> $toTitle');
  }
}