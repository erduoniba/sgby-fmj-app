import 'dart:convert';
import 'package:flutter_inappwebview/flutter_inappwebview.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'app_config.dart';
import 'log_utils.dart';
import 'platform_utils.dart';

class JSBridge {
  static InAppWebViewController? _webViewController;
  static String _currentBayeLib = "DEFAULT"; // 默认MOD标识，避免空字符串导致的存档问题

  static void setup(InAppWebViewController controller) {
    _webViewController = controller;

    // 调试：显示当前应用配置
    final currentApp = AppConfig.shared.appName;
    LogUtils.d("=== JSBridge Setup 调试信息 ===");
    LogUtils.d("当前应用类型: $currentApp");
    LogUtils.d("应用标题: ${AppConfig.shared.title}");
    LogUtils.d("离线包名称: ${AppConfig.shared.offlineZipName}");
    LogUtils.d("=== 调试信息结束 ===");

    // Skip JS handler setup on Web platform as it's not supported
    if (PlatformUtils.isWeb) {
      LogUtils.d('Skipping JS handler setup on Web platform');
      return;
    }

    // Add JS handlers
    try {
      controller.addJavaScriptHandler(
        handlerName: 'sysStorageSet',
        callback: (args) {
          if (args.isNotEmpty && args[0] is Map) {
            final data = args[0] as Map<String, dynamic>;
            final path = data['path'] as String?;
            final value = data['value'] as String?;
            if (path != null && value != null) {
              _setStorage(path, value);
            }
          }
        },
      );

      controller.addJavaScriptHandler(
        handlerName: 'chooseLib',
        callback: (args) {
          if (args.isNotEmpty && args[0] is Map) {
            final data = args[0] as Map<String, dynamic>;
            final path = data['path'] as String?;
            final title = data['title'] as String?;
            if (path != null) {
              _chooseLib(path, title);
            }
          }
        },
      );

      // Add handler for FMJ lib choice (for FMJ app mod switching)
      controller.addJavaScriptHandler(
        handlerName: 'chooseFmjLib',
        callback: (args) {
          if (args.isNotEmpty && args[0] is Map) {
            final data = args[0] as Map<String, dynamic>;
            final path = data['path'] as String?;
            if (path != null) {
              _chooseFmjLib(path);
            }
          }
        },
      );

      controller.addJavaScriptHandler(
        handlerName: 'loadMapToApp',
        callback: (args) {
          if (args.isNotEmpty && args[0] is Map) {
            final data = args[0] as Map<String, dynamic>;
            final index = data['index'] as int?;
            if (index != null) {
              _loadMapToApp(index);
            }
          }
        },
      );

      controller.addJavaScriptHandler(
        handlerName: 'triggerMapPosition',
        callback: (args) {
          if (args.isNotEmpty && args[0] is Map) {
            final data = args[0] as Map<String, dynamic>;
            final x = data['x'] as int?;
            final y = data['y'] as int?;
            if (x != null && y != null) {
              _triggerMapPosition(x, y);
            }
          }
        },
      );

      controller.addJavaScriptHandler(
        handlerName: 'timingHandler',
        callback: (args) {
          if (args.isNotEmpty) {
            LogUtils.d('Performance timing: ${args[0]}');
          }
        },
      );
    } catch (e) {
      LogUtils.d('Failed to setup JS handlers: $e');
    }
  }

  static Future<void> _setStorage(String path, String value) async {
    final prefs = await SharedPreferences.getInstance();

    // Handle different app types
    if (AppConfig.shared.appName == AppName.hdBayeApp) {
      final key = "${_currentBayeLib}_$path";
      LogUtils.d("JSBridge setStorage.key: $key");
      await prefs.setString(key, value);
      return;
    }

    if (AppConfig.shared.appName == AppName.hdFmjApp) {
      final choiceLib = AppConfig.shared.choiceLib;
      final tKey = choiceLib['key'];
      if (tKey != null) {
        if (tKey == "FMJ") {
          // Original version, no prefix for compatibility
          await prefs.setString(path, value);
        } else {
          // Non-original version, add prefix
          var key = "${tKey}_$path";
          
          // 伏魔记神女轮舞曲(木子02/03/04) 复用 伏魔记圆梦前奏曲(木子01) 存档
          if (tKey == "FMJSNLWQ" || tKey == "FMJMVKXQ" || tKey == "FMJHMAHQ") {
            key = "FMJYMQZQ_$path";
          }
          LogUtils.d("JSBridge setStorage.key: $key");
          await prefs.setString(key, value);
        }
        return;
      }
    }

    await prefs.setString(path, value);
  }

  // Add FMJ-specific cache injection
  static Future<void> injectFmjCacheJSHooks() async {
    if (_webViewController == null) return;

    // 确认当前应用类型
    final currentApp = AppConfig.shared.appName;
    if (currentApp != AppName.hdFmjApp) {
      LogUtils.d("警告：当前应用不是伏魔记，跳过存档注入");
      return;
    }

    final prefs = await SharedPreferences.getInstance();
    final choiceLib = AppConfig.shared.choiceLib;
    final tKey = choiceLib['key'] ?? "FMJ";

    LogUtils.d("=== 伏魔记存档注入开始 ===");
    LogUtils.d("当前应用: $currentApp");
    LogUtils.d("当前MOD: $tKey (${choiceLib['value'] ?? 'Unknown'})");

    // FMJ save files (伏魔记支持5个存档位置)
    final keys = [
      "sav/fmjsave0",
      "sav/fmjsave1",
      "sav/fmjsave2",
      "sav/fmjsave3",
      "sav/fmjsave4",
    ];

    int injectedCount = 0;
    for (final key in keys) {
      // Remove existing localStorage items to avoid conflicts
      await _removeLocalStorageItem(key);

      // Check if native app has cache for current MOD
      String realKey;
      if (tKey == "FMJ") {
        realKey = key; // Original version, no prefix for compatibility
      } else {
        realKey = "${tKey}_$key"; // Non-original version, add prefix
        // 伏魔记神女轮舞曲(木子02/03/04) 复用 伏魔记圆梦前奏曲(木子01) 存档
        if (tKey == "FMJSNLWQ" || tKey == "FMJMVKXQ" || tKey == "FMJHMAHQ") {
          realKey = "FMJYMQZQ_$key";
        }
      }

      final value = prefs.getString(realKey);
      if (value != null && value.isNotEmpty) {
        await _setLocalStorageItem(key, value);
        LogUtils.d("✓ 注入存档: $key ← $realKey (${value.length} chars)");
        injectedCount++;
      } else {
        LogUtils.d("✗ 无存档数据: $realKey");
      }
    }

    LogUtils.d("=== 伏魔记存档注入完成: $injectedCount/5 存档已注入 ===");
  }

  /// 根据MOD key获取对应的名称
  static String _getModNameByKey(String key) {
    // 定义MOD key到名称的映射
    final modMapping = {
      'FMJ': '伏魔记',
      'FMJ2': '伏魔记2.0增强版',
      'FMJWMB': '伏魔记完美版（旭哥出品）',
      'JYQXZ': '金庸群侠传',
      'XKX': '侠客行',
      'XKXWMB': '侠客行完美版',
      'CBZZZSYZF': '赤壁之战之谁与争峰',
      'YZCQ2': '一中传奇2',
      'XXJWMB': '仙剑奇侠传完美版',
      'XJQXZEZHJFJ': '仙剑奇侠传二之虎啸飞剑',
      'XJQXZSZLHQY': '仙剑奇侠传三之轮回情缘',
      'XJQXZSHYMYX': '仙剑奇侠传四之回梦游仙',
      'LGSCQ': '伏魔记之老观寺传奇（旭哥出品）',
      'XBMT': '新版魔塔',
      'FMJLL': '伏魔记乐乐圆梦(木子出品)',
      'YXTS': '英雄坛说',
      'WDSJ': '我的世界',
      'FMJYMQZQ': '伏魔记之圆梦前奏曲(木子01)',
      'FMJSNLWQ': '伏魔记之神女轮舞曲(木子02)',
      'FMJMVKXQ': '伏魔记之魔女狂想曲(木子03)',
      'FMJHMAHQ': '伏魔记之回梦安魂曲(木子04)',
      'FMJFYJ': '伏魔记之伏羊记'
    };

    return modMapping[key] ?? key; // 如果找不到映射，返回key本身
  }

  /// 初始化伏魔记MOD（从localStorage读取或设置默认值）
  static Future<void> initializeFmjLib() async {
    if (_webViewController == null) return;

    // 确认当前应用类型
    final currentApp = AppConfig.shared.appName;
    LogUtils.d("=== 初始化伏魔记MOD ===");
    LogUtils.d("当前应用类型: $currentApp");

    if (currentApp != AppName.hdFmjApp) {
      LogUtils.d("警告：当前应用不是伏魔记，跳过MOD初始化");
      return;
    }

    try {
      // 伏魔记使用choiceLibName localStorage key来保存当前选择的MOD
      final choiceLibResult = await _webViewController!.evaluateJavascript(
        source: "localStorage.getItem('choiceLibName')",
      );

      LogUtils.d("伏魔记localStorage检查:");
      LogUtils.d("  choiceLibName: $choiceLibResult");

      if (choiceLibResult != null &&
          choiceLibResult.toString().isNotEmpty &&
          choiceLibResult.toString() != "null") {
        final currentMod = choiceLibResult.toString().replaceAll(
          '"',
          '',
        ); // 移除引号

        // 从存储的MOD信息中解析key和value
        final parts = currentMod.split('|');
        if (parts.length >= 2) {
          AppConfig.shared.choiceLib = {'key': parts[0], 'value': parts[1]};
          LogUtils.d("从localStorage恢复伏魔记MOD: ${parts[0]} (名称: ${parts[1]})");
        } else {
          // 如果格式不正确，尝试作为key使用，并查找对应的名称
          final modName = _getModNameByKey(currentMod);
          AppConfig.shared.choiceLib = {'key': currentMod, 'value': modName};
          LogUtils.d("从localStorage恢复伏魔记MOD: $currentMod (名称: $modName)");
        }
      } else {
        // 如果没有保存的MOD，设置默认值（原版伏魔记）
        AppConfig.shared.choiceLib = {'key': 'FMJ', 'value': '伏魔记'};
        await _webViewController!.evaluateJavascript(
          source: "localStorage.setItem('choiceLibName', 'FMJ');",
        );
        LogUtils.d("设置默认伏魔记MOD: FMJ (伏魔记)");
      }
    } catch (e) {
      LogUtils.d("初始化伏魔记MOD失败: $e");
      AppConfig.shared.choiceLib = {'key': 'FMJ', 'value': '伏魔记'};
    }
  }

  /// 初始化三国霸业MOD（从localStorage读取或设置默认值）
  static Future<void> initializeBayeLib() async {
    if (_webViewController == null) return;

    // 确认当前应用类型
    final currentApp = AppConfig.shared.appName;
    LogUtils.d("=== 初始化三国霸业MOD ===");
    LogUtils.d("当前应用类型: $currentApp");

    if (currentApp != AppName.hdBayeApp) {
      LogUtils.d("警告：当前应用不是三国霸业，跳过MOD初始化");
      return;
    }

    try {
      // 三国霸业使用不同的localStorage key：baye/libname 和 baye/libpath
      final libNameResult = await _webViewController!.evaluateJavascript(
        source: "localStorage.getItem('baye/libname')",
      );

      final libPathResult = await _webViewController!.evaluateJavascript(
        source: "localStorage.getItem('baye/libpath')",
      );

      // 同时检查是否有错误的伏魔记MOD信息
      final wrongChoiceLibResult = await _webViewController!.evaluateJavascript(
        source: "localStorage.getItem('choiceLibName')",
      );

      if (wrongChoiceLibResult != null &&
          wrongChoiceLibResult.toString() != "null") {
        LogUtils.d("警告：检测到错误的伏魔记MOD信息: $wrongChoiceLibResult，将清除");
        await _webViewController!.evaluateJavascript(
          source: "localStorage.removeItem('choiceLibName')",
        );
      }

      LogUtils.d("三国霸业localStorage检查:");
      LogUtils.d("  baye/libname: $libNameResult");
      LogUtils.d("  baye/libpath: $libPathResult");

      if (libPathResult != null &&
          libPathResult.toString().isNotEmpty &&
          libPathResult.toString() != "null") {
        _currentBayeLib = libPathResult.toString().replaceAll(
          '"',
          '',
        ); // 移除引号，使用path作为MOD标识
        LogUtils.d(
          "从localStorage恢复三国霸业MOD: $_currentBayeLib (名称: ${libNameResult?.toString().replaceAll('"', '')})",
        );
      } else {
        // 如果没有保存的MOD，设置默认值
        _currentBayeLib = "DEFAULT";
        await _webViewController!.evaluateJavascript(
          source: """
            localStorage.setItem('baye/libname', '默认游戏');
            localStorage.setItem('baye/libpath', '$_currentBayeLib');
          """,
        );
        LogUtils.d("设置默认三国霸业MOD: $_currentBayeLib");
      }
    } catch (e) {
      LogUtils.d("初始化三国霸业MOD失败: $e");
      _currentBayeLib = "DEFAULT";
    }
  }

  // Add Baye-specific cache injection
  static Future<void> injectBayeCacheJSHooks() async {
    if (_webViewController == null) return;

    // 确认当前应用类型
    final currentApp = AppConfig.shared.appName;
    if (currentApp != AppName.hdBayeApp) {
      LogUtils.d("警告：当前应用不是三国霸业，跳过存档注入");
      return;
    }

    final prefs = await SharedPreferences.getInstance();

    LogUtils.d("=== 三国霸业存档注入开始 ===");
    LogUtils.d("当前应用: $currentApp");
    LogUtils.d("当前MOD: $_currentBayeLib");

    // Baye save files (三国霸业支持8个存档位置)
    final keys = [
      "baye//data//sango0.sav",
      "baye//data//sango1.sav",
      "baye//data//sango2.sav",
      "baye//data//sango3.sav",
      "baye//data//sango4.sav",
      "baye//data//sango5.sav",
      "baye//data//sango6.sav",
      "baye//data//sango7.sav",
    ];

    int injectedCount = 0;
    for (final key in keys) {
      final libKey = "$key.lib";
      final nameKey = "$key.name";

      // Remove existing localStorage items to avoid conflicts
      await _removeLocalStorageItem(key);
      await _removeLocalStorageItem(libKey);
      await _removeLocalStorageItem(nameKey);

      // Check if native app has cache for current MOD
      final realKey = "${_currentBayeLib}_$key";
      final value = prefs.getString(realKey);
      if (value != null && value.isNotEmpty) {
        await _setLocalStorageItem(key, value);
        LogUtils.d("✓ 注入存档: $key ← $realKey (${value.length} chars)");
        injectedCount++;
      } else {
        LogUtils.d("✗ 无存档数据: $realKey");
      }
    }

    LogUtils.d("=== 三国霸业存档注入完成: $injectedCount/8 存档已注入 ===");
  }

  static void _chooseLib(String path, String? title) {
    // 如果title为空，尝试从path推断标题
    String finalTitle = title ?? _getLibTitleFromPath(path);

    LogUtils.d("JSBridge chooseLib: $path (名称: $finalTitle)");
    _currentBayeLib = path;

    // 更新localStorage以保持与JavaScript代码的一致性
    _webViewController?.evaluateJavascript(
      source: """
        localStorage.setItem('baye/libname', '$finalTitle');
        localStorage.setItem('baye/libpath', '$path');
      """,
    );

    // Inject cache JS hooks after choosing lib
    injectBayeCacheJSHooks();
  }

  // 从路径推断游戏库的标题
  static String _getLibTitleFromPath(String path) {
    // 三国霸业游戏库路径到标题的映射
    final Map<String, String> pathToTitle = {
      'libs/dat-v2-mod.lib': '三国霸业-词典原版',
      'libs/dat-mod.lib': '三国霸业-词典原版',
      'libs/qmlw-XSHX.lib': '群魔乱舞-血色华夏',
      'libs/qmlw-SHYH.lib': '群魔乱舞-水浒英豪',
      'libs/qmlw-LZSG.lib': '群魔乱舞-乱战三国',
      'libs/balance2.01.lib': '平衡版2.0三国战纪',
      'libs/yhf-mod.lib': '三国战略版',
    };

    return pathToTitle[path] ?? '未知游戏';
  }

  // Handle FMJ lib choice (similar to chooseLib but for FMJ)
  static void _chooseFmjLib(String path) {
    LogUtils.d("JSBridge chooseFmjLib: $path");

    // Update choice lib in AppConfig
    // This should match how web_view_page.dart updates the config
    final parts = path.split('|'); // Expecting format like "FMJWMB|伏魔记完美版"
    if (parts.length >= 2) {
      AppConfig.shared.choiceLib = {'key': parts[0], 'value': parts[1]};
      LogUtils.d("Updated choiceLib to: ${AppConfig.shared.choiceLib}");

      // 更新localStorage以保持与JavaScript代码的一致性
      _webViewController?.evaluateJavascript(
        source: "localStorage.setItem('choiceLibName', '$path');",
      );

      // Immediately inject FMJ cache hooks with new lib
      injectFmjCacheJSHooks();
    } else {
      LogUtils.d("警告：FMJ MOD路径格式不正确: $path");
    }
  }

  static Future<void> _setLocalStorageItem(String key, String value) async {
    final script = "localStorage.setItem('$key', '$value');";
    try {
      await _webViewController?.evaluateJavascript(source: script);
      LogUtils.d("JSBridge setLocalStorage success: $key");
    } catch (error) {
      LogUtils.d("JSBridge setLocalStorage failed: $error");
    }
  }

  static Future<void> _removeLocalStorageItem(String key) async {
    final script = "localStorage.removeItem('$key');";
    try {
      await _webViewController?.evaluateJavascript(source: script);
      LogUtils.d("JSBridge removeItem success: $key");
    } catch (error) {
      LogUtils.d("JSBridge removeItem failed: $error");
    }
  }

  static void _loadMapToApp(int index) {
    LogUtils.d("loadMapToApp: $index");
  }

  static void _triggerMapPosition(int x, int y) {
    LogUtils.d("triggerMapPosition: $x $y");
  }

  // JavaScript utility functions
  static Future<void> setLocalStorage(String key, String value) async {
    await _setLocalStorageItem(key, value);
  }

  static Future<void> disableTouchCallout() async {
    const script = """
      document.documentElement.style.webkitTouchCallout='none';
      document.body.style.webkitTouchCallout='none';
    """;
    try {
      await _webViewController?.evaluateJavascript(source: script);
      LogUtils.d("Disabled touch callout");
    } catch (error) {
      LogUtils.d("Failed to disable touch callout: $error");
    }
  }

  // Settings update functions

  static Future<void> updateColorFilter(String filter) async {
    // 使用与iOS一致的FilterBridge API调用方式
    final script = """
      (function() {
        if (typeof window.FilterBridge !== 'undefined' && window.FilterBridge.setPresetFilter) {
          // 使用统一的 FilterBridge API
          return window.FilterBridge.setPresetFilter('$filter');
        } else if (typeof window.setPresetFilter === 'function') {
          // 降级到直接调用（兼容旧版本）
          return window.setPresetFilter('$filter');
        } else {
          console.warn('滤镜API未找到，可能页面还未完全加载');
          return false;
        }
      })();
    """;
    try {
      await _webViewController?.evaluateJavascript(source: script);
      LogUtils.d("Updated color filter: $filter");
    } catch (error) {
      LogUtils.d("Failed to update color filter: $error");
    }
  }

  static Future<void> updateGameSpeed(double speed) async {
    // 使用与iOS一致的setGameSpeed API调用方式，同时兼容旧版本的gameSpeedMultiple变量
    final script = """
      if (typeof window.setGameSpeed === 'function') {
        window.setGameSpeed($speed);
        console.log('通过setGameSpeed API设置游戏速度: $speed');
      } else {
        // 降级到直接设置变量（兼容旧版本）
        window.gameSpeedMultiple = $speed;
        console.log('通过变量设置游戏速度: $speed');
      }
    """;
    try {
      await _webViewController?.evaluateJavascript(source: script);
      LogUtils.d("Updated game speed: $speed");
    } catch (error) {
      LogUtils.d("Failed to update game speed: $error");
    }
  }

  static Future<void> updatePortraitMode(bool isPortrait) async {
    final script =
        "window.setPortraitMode && window.setPortraitMode($isPortrait);";
    try {
      await _webViewController?.evaluateJavascript(source: script);
      LogUtils.d("Updated portrait mode: $isPortrait");
    } catch (error) {
      LogUtils.d("Failed to update portrait mode: $error");
    }
  }

  static Future<void> updateMapDisplay(bool showMap) async {
    final script = """
      window.showMapContainer && window.showMapContainer($showMap);
      localStorage.setItem('showMapContainer', '$showMap');
    """;
    try {
      await _webViewController?.evaluateJavascript(source: script);
      LogUtils.d("Updated map display: $showMap");
    } catch (error) {
      LogUtils.d("Failed to update map display: $error");
    }
  }

  static Future<void> updateCombatProbability(int probability) async {
    // 根据iOS版本的逻辑，实际传递给游戏的是 100 - 用户设置值
    final actualProbability = 100 - probability;
    final script = """
      window.combat_probability = $actualProbability;
      localStorage.setItem('combat_probability', '$actualProbability');
    """;
    try {
      await _webViewController?.evaluateJavascript(source: script);
      LogUtils.d(
        "Updated combat probability: $probability% (actual: $actualProbability)",
      );
    } catch (error) {
      LogUtils.d("Failed to update combat probability: $error");
    }
  }

  static Future<void> setExpMultiple(double multiple) async {
    final script =
        "if (typeof window.winExpMultiple !== 'undefined') { window.winExpMultiple = $multiple; }";
    try {
      await _webViewController?.evaluateJavascript(source: script);
      LogUtils.d("Set exp multiple: $multiple");
    } catch (error) {
      LogUtils.d("Failed to set exp multiple: $error");
    }
  }

  static Future<void> setGoldMultiple(double multiple) async {
    final script =
        "if (typeof window.winMoneyMultiple !== 'undefined') { window.winMoneyMultiple = $multiple; }";
    try {
      await _webViewController?.evaluateJavascript(source: script);
      LogUtils.d("Set gold multiple: $multiple");
    } catch (error) {
      LogUtils.d("Failed to set gold multiple: $error");
    }
  }

  static Future<void> setItemMultiple(double multiple) async {
    final script =
        "if (typeof window.winItemMultiple !== 'undefined') { window.winItemMultiple = $multiple; }";
    try {
      await _webViewController?.evaluateJavascript(source: script);
      LogUtils.d("Set item multiple: $multiple");
    } catch (error) {
      LogUtils.d("Failed to set item multiple: $error");
    }
  }

  /// 添加全部物品到游戏（VIP特权）
  /// 调用游戏引擎的 iOSGrantIAPItems 接口
  static Future<Map<String, dynamic>> addAllItems(String productId) async {
    try {
      // 确保JS函数已加载
      final checkScript = """
        (function() {
          if (typeof window.iOSGrantIAPItems === 'function') {
            return true;
          } else {
            console.error('iOSGrantIAPItems function not found');
            return false;
          }
        })();
      """;

      final isReady = await _webViewController?.evaluateJavascript(
        source: checkScript,
      );

      if (isReady != true) {
        LogUtils.d("iOSGrantIAPItems function not ready yet");
        return {'success': false, 'message': '游戏引擎尚未准备好，请稍后再试'};
      }

      // 调用游戏的物品发放接口
      final script = "window.iOSGrantIAPItems('$productId');";
      final result = await _webViewController?.evaluateJavascript(
        source: script,
      );

      LogUtils.d("Add all items result: $result");

      // 解析返回的 JSON 字符串
      if (result != null && result is String) {
        try {
          final data = jsonDecode(result) as Map<String, dynamic>;
          return {
            'success': data['success'] ?? false,
            'message': data['message'] ?? '物品添加完成',
            'action': data['action'] ?? 'completed',
          };
        } catch (e) {
          LogUtils.d("Failed to parse add items result: $e");
        }
      }

      // 默认返回成功
      return {'success': true, 'message': '全部物品添加成功'};
    } catch (error) {
      LogUtils.d("Failed to add all items: $error");
      return {'success': false, 'message': '添加物品失败: $error'};
    }
  }

  /// 调试：显示当前伏魔记存档状态
  static Future<void> debugFmjSaveStatus() async {
    if (_webViewController == null) return;

    final prefs = await SharedPreferences.getInstance();
    final choiceLib = AppConfig.shared.choiceLib;
    final tKey = choiceLib['key'] ?? "FMJ";

    LogUtils.d("=== 伏魔记存档状态调试 ===");
    LogUtils.d("当前MOD: $tKey (${choiceLib['value'] ?? 'Unknown'})");

    final keys = [
      "sav/fmjsave0",
      "sav/fmjsave1",
      "sav/fmjsave2",
      "sav/fmjsave3",
      "sav/fmjsave4",
    ];

    for (final key in keys) {
      String realKey;
      if (tKey == "FMJ") {
        realKey = key; // Original version, no prefix
      } else {
        realKey = "${tKey}_$key"; // Non-original version, add prefix
        // 伏魔记神女轮舞曲(木子02/03/04) 复用 伏魔记圆梦前奏曲(木子01) 存档
        if (tKey == "FMJSNLWQ" || tKey == "FMJMVKXQ" || tKey == "FMJHMAHQ") {
          realKey = "FMJYMQZQ_$key";
        }
      }

      final value = prefs.getString(realKey);

      if (value != null && value.isNotEmpty) {
        LogUtils.d("存档存在: $realKey (${value.length} chars)");
      } else {
        LogUtils.d("存档为空: $realKey");
      }
    }

    LogUtils.d("=== 存档状态调试结束 ===");
  }

  /// 调试：显示当前三国霸业存档状态
  static Future<void> debugBayeSaveStatus() async {
    if (_webViewController == null) return;

    final prefs = await SharedPreferences.getInstance();

    LogUtils.d("=== 三国霸业存档状态调试 ===");
    LogUtils.d("当前MOD: $_currentBayeLib");

    final keys = [
      "baye//data//sango0.sav",
      "baye//data//sango1.sav",
      "baye//data//sango2.sav",
      "baye//data//sango3.sav",
      "baye//data//sango4.sav",
      "baye//data//sango5.sav",
      "baye//data//sango6.sav",
      "baye//data//sango7.sav",
    ];

    for (final key in keys) {
      final realKey = "${_currentBayeLib}_$key";
      final value = prefs.getString(realKey);

      if (value != null && value.isNotEmpty) {
        LogUtils.d("存档存在: $realKey (${value.length} chars)");
      } else {
        LogUtils.d("存档为空: $realKey");
      }
    }

    LogUtils.d("=== 存档状态调试结束 ===");
  }

  /// 重置所有激励效果（经验倍数、金币倍数、大地图）
  static Future<void> resetRewardEffects() async {
    try {
      final script = """
        // 重置经验倍数为1倍
        if (typeof window.winExpMultiple !== 'undefined') {
          window.winExpMultiple = 1.0;
        }
        // 重置金币倍数为1倍
        if (typeof window.winMoneyMultiple !== 'undefined') {
          window.winMoneyMultiple = 1.0;
        }
        // 重置大地图显示
        if (typeof window.showMapContainer === 'function') {
          window.showMapContainer(false);
        }
        localStorage.setItem('showMapContainer', 'false');
        console.log('已重置所有激励效果');
      """;

      await _webViewController?.evaluateJavascript(source: script);
      LogUtils.d("Reset all reward effects successfully");
    } catch (error) {
      LogUtils.d("Failed to reset reward effects: $error");
    }
  }

  // Inject performance timing script
  static String get performanceTimingScript {
    return """
      window.addEventListener('load', function() {
        console.log('performance.timing:', performance.timing);
        const jsonString = JSON.stringify(performance.timing);
        if (!jsonString) {
          console.log("message to json error");
          return;
        }
        window.flutter_inappwebview.callHandler('timingHandler', jsonString);
      });
    """;
  }
}
