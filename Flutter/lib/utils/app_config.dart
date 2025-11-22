import 'package:flutter/material.dart';
import 'package:shared_preferences/shared_preferences.dart';

enum AppName {
  none,
  hdBayeApp,
  hdFmjApp,
}

class AppConfig extends ChangeNotifier {
  static final AppConfig _instance = AppConfig._internal();
  factory AppConfig() => _instance;
  AppConfig._internal();

  late SharedPreferences _prefs;
  AppName _appName = AppName.none;

  static AppConfig get shared => _instance;

  Future<void> initialize() async {
    _prefs = await SharedPreferences.getInstance();
    _setupAppName();
    // 每次启动重置激励功能
    _resetRewardFeatures();
  }

  /// 每次启动重置激励功能（大地图和倍数加成）
  void _resetRewardFeatures() {
    // 重置大地图显示
    showMapContainer = false;
    // 注意：经验和金币倍数不在AppConfig中存储，需要在WebView加载后通过JS重置
  }

  void _setupAppName() {
    // 从SharedPreferences读取上次选择的游戏
    final gameString = _prefs.getString('selectedAppName');
    if (gameString != null) {
      _appName = gameString == 'hdBayeApp' ? AppName.hdBayeApp : AppName.hdFmjApp;
    } else {
      // 默认设置为伏魔记
      _appName = AppName.hdFmjApp;
    }
  }

  // Method to switch games
  Future<void> switchToGame(AppName newGame) async {
    _appName = newGame;
    notifyListeners(); // 通知监听器主题需要更新
    final gameString = newGame == AppName.hdBayeApp ? 'hdBayeApp' : 'hdFmjApp';
    await _prefs.setString('selectedAppName', gameString);
  }

  AppName get appName => _appName;

  // App Information
  String get title {
    switch (_appName) {
      case AppName.hdBayeApp:
        return "三国霸业";
      case AppName.hdFmjApp:
        return "伏魔记";
      default:
        return "";
    }
  }

  String get indexPageName {
    switch (_appName) {
      case AppName.hdBayeApp:
        return "choose.html";
      case AppName.hdFmjApp:
        return "index.html";
      default:
        return "";
    }
  }

  String get strategyPageName {
    switch (_appName) {
      case AppName.hdBayeApp:
        return "choose.html";
      case AppName.hdFmjApp:
        return "games/fmj/strategy.html";
      default:
        return "";
    }
  }

  String get offlineZipName {
    switch (_appName) {
      case AppName.hdBayeApp:
        return "baye-offline-2025110101"; // AUTO-GENERATED: 构建脚本会自动替换此版本号
      case AppName.hdFmjApp:
        return "fmj-offline-2025111501"; // AUTO-GENERATED: 构建脚本会自动替换此版本号
      default:
        return "";
    }
  }

  String get offlineDirName {
    switch (_appName) {
      case AppName.hdBayeApp:
        return "baye-offline";
      case AppName.hdFmjApp:
        return "fmj-offline";
      default:
        return "";
    }
  }

  String get gameType {
    switch (_appName) {
      case AppName.hdBayeApp:
        return "baye";
      case AppName.hdFmjApp:
        return "fmj";
      default:
        return "";
    }
  }

  String get appStoreUrl {
    return "http://fir.xcxwo.com/byfmj";
    // switch (_appName) {
    //   case AppName.hdBayeApp:
    //     return "itms-apps://itunes.apple.com/app/id6744382643";
    //   case AppName.hdFmjApp:
    //     return "itms-apps://itunes.apple.com/app/id6747572873";
    //   default:
    //     return "";
    // }
  }

  String get shareUrl {
    return "http://fir.xcxwo.com/byfmj";
    // switch (_appName) {
    //   case AppName.hdBayeApp:
    //     return "https://apps.apple.com/cn/app/relatree/id6744382643";
    //   case AppName.hdFmjApp:
    //     return "https://apps.apple.com/cn/app/relatree/id6747572873";
    //   default:
    //     return "";
    // }
  }

  List<String> get relationApps {
    switch (_appName) {
      case AppName.hdBayeApp:
        return ["伏魔记"];
      case AppName.hdFmjApp:
        return ["三国霸业"];
      default:
        return [];
    }
  }

  List<String> get relationAppsScheme {
    switch (_appName) {
      case AppName.hdBayeApp:
        return ["hdfmj"];
      case AppName.hdFmjApp:
        return ["hdbaye"];
      default:
        return [];
    }
  }

  List<String> get relationAppsLink {
    switch (_appName) {
      case AppName.hdBayeApp:
        return ["itms-apps://itunes.apple.com/app/id6747572873"];
      case AppName.hdFmjApp:
        return ["itms-apps://itunes.apple.com/app/id6744382643"];
      default:
        return [];
    }
  }

  // Theme colors
  Color get primaryColor {
    switch (_appName) {
      case AppName.hdFmjApp:
        return const Color.fromRGBO(19, 39, 72, 1.0); // 伏魔记深蓝色
      case AppName.hdBayeApp:
      default:
        return Colors.deepPurple; // 三国霸业默认紫色
    }
  }

  Color get backgroundColor {
    switch (_appName) {
      case AppName.hdFmjApp:
        return const Color.fromRGBO(245, 247, 251, 1.0); // 伏魔记浅蓝灰色背景
      case AppName.hdBayeApp:
      default:
        return Colors.white; // 三国霸业白色背景
    }
  }

  Color get appBarBackgroundColor {
    switch (_appName) {
      case AppName.hdFmjApp:
        return const Color.fromRGBO(19, 39, 72, 1.0); // 伏魔记深蓝色
      case AppName.hdBayeApp:
      default:
        return Colors.white; // 三国霸业白色
    }
  }

  Color get appBarForegroundColor {
    switch (_appName) {
      case AppName.hdFmjApp:
        return Colors.white; // 伏魔记白色文字
      case AppName.hdBayeApp:
      default:
        return Colors.black; // 三国霸业黑色文字
    }
  }

  // WebView页面专用背景色（游戏运行时保持深色）
  Color get webViewBackgroundColor {
    switch (_appName) {
      case AppName.hdFmjApp:
        return const Color.fromRGBO(19, 39, 72, 1.0); // 伏魔记深蓝色背景
      case AppName.hdBayeApp:
      default:
        return Colors.white; // 三国霸业白色背景
    }
  }

  // Settings Properties

  String get selectedFilter {
    String filter = _prefs.getString('selectedFilter') ?? "";
    // 迁移旧的滤镜值到新系统
    if (filter == "reset") {
      return "none";
    } else if (filter == "green" || filter == "red") {
      // 不支持的滤镜设为无滤镜
      return "none";
    }
    return filter;
  }

  set selectedFilter(String value) => _prefs.setString('selectedFilter', value);

  double get gameSpeed => _prefs.getDouble('gameSpeed') ?? 1.0;
  set gameSpeed(double value) => _prefs.setDouble('gameSpeed', value);

  bool get portraitMode => _prefs.getBool('portraitMode') ?? true;
  set portraitMode(bool value) => _prefs.setBool('portraitMode', value);


  bool get showMapContainer => _prefs.getBool('showMapContainer') ?? false;
  set showMapContainer(bool value) => _prefs.setBool('showMapContainer', value);

  int get combatProbability => _prefs.getInt('combatProbability') ?? 20;
  set combatProbability(int value) => _prefs.setInt('combatProbability', value);

  Map<String, String> get choiceLib {
    final key = _prefs.getString('choiceLib_key') ?? 'FMJ';
    final value = _prefs.getString('choiceLib_value') ?? '伏魔记';
    return {'key': key, 'value': value};
  }

  set choiceLib(Map<String, String> value) {
    _prefs.setString('choiceLib_key', value['key'] ?? 'FMJ');
    _prefs.setString('choiceLib_value', value['value'] ?? '伏魔记');
  }


  String get hasShownUpgradeAlertForVersion => _prefs.getString('hasShownUpgradeAlertForVersion') ?? "";
  set hasShownUpgradeAlertForVersion(String value) => _prefs.setString('hasShownUpgradeAlertForVersion', value);

  int get appLaunchCount => _prefs.getInt('appLaunchCount') ?? 0;
  set appLaunchCount(int value) => _prefs.setInt('appLaunchCount', value);

  String get lastVersionCheck => _prefs.getString('lastVersionCheck') ?? "";
  set lastVersionCheck(String value) => _prefs.setString('lastVersionCheck', value);

  // Helper methods
  bool hasShownUpgradeAlert(String version) {
    return hasShownUpgradeAlertForVersion == version;
  }

  void markUpgradeAlertShown(String version) {
    hasShownUpgradeAlertForVersion = version;
  }

  void incrementAppLaunchCount() {
    appLaunchCount = appLaunchCount + 1;
  }

  void resetAppLaunchCount() {
    appLaunchCount = 0;
  }

  // Share text messages
  List<String> get shareTexts => [
    "发现一款超有意思的宝藏应用！玩法新奇又上头，强烈推荐给大家试试！",
    "被这款应用狠狠拿捏了！各种惊喜小细节，越玩越上瘾，分享给我的宝子们！",
    "最近挖到一款超好玩的应用，一玩就停不下来！必须安利给你们，快来一起快乐～",
    "救命！怎么会有这么宝藏的应用！好玩到忘记时间，按头安利给你们！",
    "挖到宝了！这款应用真的超绝，好玩又解压，忍不住分享给你们，赶紧码住！"
  ];

  String getRandomShareText() {
    final index = DateTime.now().millisecondsSinceEpoch % shareTexts.length;
    return shareTexts[index];
  }
}