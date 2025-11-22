import 'package:shared_preferences/shared_preferences.dart';
import '../models/save_data.dart';
import 'app_config.dart';
import 'log_utils.dart';

class SaveManager {
  static final SaveManager _instance = SaveManager._internal();
  factory SaveManager() => _instance;
  SaveManager._internal();

  static SaveManager get shared => _instance;

  // Dynamic game mapping, similar to iOS libsList
  Map<String, String> get gameMapping {
    // TODO: Can be enhanced to load from config or API
    return {
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
  }

  /// Load all save data based on current app type
  Future<List<SaveData>> loadAllSaveData() async {
    final prefs = await SharedPreferences.getInstance();
    List<SaveData> allSaveData = [];
    
    if (AppConfig.shared.appName == AppName.hdBayeApp) {
      allSaveData = await _loadBayeSaveData(prefs);
    } else {
      allSaveData = await _loadFMJSaveData(prefs);
    }
    
    return allSaveData;
  }

  /// Load Baye (三国霸业) save data
  Future<List<SaveData>> _loadBayeSaveData(SharedPreferences prefs) async {
    List<SaveData> saveDataList = [];
    final allKeys = prefs.getKeys();
    
    // Filter save-related keys (Baye saves contain mod name prefix)
    for (String key in allKeys) {
      if (key.contains("_baye//data//") || key.startsWith("baye//data//")) {
        final content = prefs.getString(key);
        if (content != null && content.isNotEmpty) {
          final gameInfo = _extractGameInfo(key);
          final saveData = SaveData(
            key: key,
            title: _generateSaveTitle(key),
            content: content,
            createdDate: DateTime.now(), // TODO: Extract from save content if possible
            gameKey: gameInfo.$1,
            gameName: gameInfo.$2,
          );
          saveDataList.add(saveData);
        }
      }
    }
    
    return saveDataList;
  }

  /// Load FMJ (伏魔记) save data
  Future<List<SaveData>> _loadFMJSaveData(SharedPreferences prefs) async {
    List<SaveData> saveDataList = [];
    final fmjSaveKeys = ["sav/fmjsave0", "sav/fmjsave1", "sav/fmjsave2", "sav/fmjsave3", "sav/fmjsave4"];
    
    // Get current selected mod
    final currentMod = AppConfig.shared.choiceLib['key'] ?? "FMJ";
    
    // Load current mod saves
    for (String baseKey in fmjSaveKeys) {
      String actualKey = baseKey;
      // If not original version, add mod prefix
      if (currentMod != "FMJ") {
        actualKey = "${currentMod}_$baseKey";
      }
      
      final content = prefs.getString(actualKey);
      if (content != null && content.isNotEmpty) {
        final gameInfo = _extractGameInfo(actualKey);
        final saveData = SaveData(
          key: actualKey,
          title: _generateSaveTitle(baseKey),
          content: content,
          createdDate: DateTime.now(),
          gameKey: gameInfo.$1,
          gameName: gameInfo.$2,
        );
        saveDataList.add(saveData);
        LogUtils.d("Loaded FMJ save: $actualKey (${content.length} chars)");
      }
    }
    
    // Also check other mod saves and direct fmjsave saves
    final allKeys = prefs.getKeys();
    for (String key in allKeys) {
      // Check prefixed other mod saves
      if (key.contains("_sav/fmjsave") && !key.startsWith("${currentMod}_")) {
        final content = prefs.getString(key);
        if (content != null && content.isNotEmpty) {
          final gameInfo = _extractGameInfo(key);
          final saveData = SaveData(
            key: key,
            title: _generateSaveTitle(key),
            content: content,
            createdDate: DateTime.now(),
            gameKey: gameInfo.$1,
            gameName: gameInfo.$2,
          );
          saveDataList.add(saveData);
        }
      }
      // Check direct sav/fmjsave saves (when current mod is not FMJ, also include original saves)
      else if (key.startsWith("sav/fmjsave") && currentMod != "FMJ") {
        final content = prefs.getString(key);
        if (content != null && content.isNotEmpty) {
          final gameInfo = _extractGameInfo(key);
          final saveData = SaveData(
            key: key,
            title: _generateSaveTitle(key),
            content: content,
            createdDate: DateTime.now(),
            gameKey: gameInfo.$1,
            gameName: gameInfo.$2,
          );
          saveDataList.add(saveData);
        }
      }
    }
    
    return saveDataList;
  }

  /// Generate user-friendly title from save key
  String _generateSaveTitle(String key) {
    if (key.contains("fmjsave0")) {
      return "存档位置 1";
    } else if (key.contains("fmjsave1")) {
      return "存档位置 2";
    } else if (key.contains("fmjsave2")) {
      return "存档位置 3";
    } else if (key.contains("fmjsave3")) {
      return "存档位置 4";
    } else if (key.contains("fmjsave4")) {
      return "存档位置 5";
    } else if (key.contains("_baye//data//") || key.contains("_sav/")) {
      // Baye saves, extract mod name
      final components = key.split(RegExp(r'_(?:baye//data//|sav/)'));
      if (components.length > 1) {
        final modName = components[0];
        final saveFileName = components.last;
        return "$modName - $saveFileName";
      }
    } else if (key.startsWith("baye//data//") || key.startsWith("sav/")) {
      // Direct save key
      final prefix = key.startsWith("baye//data//") ? "baye//data//" : "sav/";
      final fileName = key.substring(prefix.length);
      return fileName;
    }
    
    return key;
  }

  /// Extract game info from save key
  (String, String) _extractGameInfo(String key) {
    // From save key extract game prefix
    if (key.contains("_baye//data//") || key.contains("_sav/")) {
      // Format like: "XKX_baye//data//xxx" or "FMJWMB_sav/fmjsave0"
      final separator = key.contains("_baye//data//") ? "_baye//data//" : "_sav/";
      final components = key.split(separator);
      if (components.isNotEmpty) {
        final gameKey = components.first;
        final gameName = gameMapping[gameKey] ?? gameKey;
        return (gameKey, gameName);
      }
    } else if (key.startsWith("baye//data//") || key.startsWith("sav/")) {
      // Direct save key, like baye//data//sango0.sav, sav/fmjsave0, etc.
      if (key.contains("fmjsave") || key.startsWith("sav/")) {
        // No prefix fmjsave saves belong to FMJ original
        return ("FMJ", gameMapping["FMJ"] ?? "伏魔记");
      }
      // Other direct saves, might also be FMJ related
      // For display purposes, classify under FMJ for now
      return ("FMJ", gameMapping["FMJ"] ?? "伏魔记");
    }
    
    return ("UNKNOWN", "未知游戏");
  }

  /// Extract save position from key for sorting
  int _extractSavePosition(String key) {
    // Extract save position number from key for sorting
    
    // Handle standard fmjsave saves first
    if (key.contains("fmjsave0")) {
      return 0;
    } else if (key.contains("fmjsave1")) {
      return 1;
    } else if (key.contains("fmjsave2")) {
      return 2;
    } else if (key.contains("fmjsave3")) {
      return 3;
    } else if (key.contains("fmjsave4")) {
      return 4;
    }
    
    // Handle other save files, try to extract number from filename
    String fileName = "";
    if (key.contains("_baye//data//")) {
      final components = key.split("_baye//data//");
      fileName = components.last;
    } else if (key.contains("_sav/")) {
      final components = key.split("_sav/");
      fileName = components.last;
    } else if (key.startsWith("baye//data//")) {
      fileName = key.substring("baye//data//".length);
    } else if (key.startsWith("sav/")) {
      fileName = key.substring("sav/".length);
    } else {
      fileName = key;
    }
    
    // Try to extract number sequence from filename
    final digitMatches = RegExp(r'\d+').allMatches(fileName);
    if (digitMatches.isNotEmpty) {
      final firstNumber = int.tryParse(digitMatches.first.group(0) ?? "0") ?? 0;
      return firstNumber + 1000; // Add offset to ensure after standard saves
    }
    
    // If no number, use alphabetical sorting
    return 2000 + fileName.hashCode.abs() % 1000; // Use limited range hash to avoid too large numbers
  }

  /// Group save data by game
  List<GameSection> groupSaveDataByGame(List<SaveData> saveDataList) {
    Map<String, List<SaveData>> gameGroups = {};
    
    // Group by game
    for (SaveData saveData in saveDataList) {
      if (!gameGroups.containsKey(saveData.gameKey)) {
        gameGroups[saveData.gameKey] = [];
      }
      gameGroups[saveData.gameKey]!.add(saveData);
    }
    
    // Convert to GameSection list
    List<GameSection> gameSections = gameGroups.entries.map((entry) {
      final gameKey = entry.key;
      final saveList = entry.value;
      
      // Sort by save position
      saveList.sort((save1, save2) {
        final position1 = _extractSavePosition(save1.key);
        final position2 = _extractSavePosition(save2.key);
        return position1.compareTo(position2);
      });
      
      final gameName = gameMapping[gameKey] ?? gameKey;
      return GameSection(
        gameKey: gameKey,
        gameName: gameName,
        saveDataList: saveList,
      );
    }).toList();
    
    // Sort sections by game name
    gameSections.sort((a, b) => a.gameName.compareTo(b.gameName));
    
    return gameSections;
  }

  /// Save content to specific key
  Future<bool> saveSaveData(String key, String content) async {
    try {
      final prefs = await SharedPreferences.getInstance();
      await prefs.setString(key, content);
      LogUtils.d("Save data saved successfully for key: $key");
      return true;
    } catch (e) {
      LogUtils.d("Failed to save data for key $key: $e");
      return false;
    }
  }

  /// Delete save data by key
  Future<bool> deleteSaveData(String key) async {
    try {
      final prefs = await SharedPreferences.getInstance();
      await prefs.remove(key);
      LogUtils.d("Save data deleted successfully for key: $key");
      return true;
    } catch (e) {
      LogUtils.d("Failed to delete data for key $key: $e");
      return false;
    }
  }
}