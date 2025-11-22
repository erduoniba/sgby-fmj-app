import 'dart:convert';
import 'dart:io';
import 'package:http/http.dart' as http;
import 'package:device_info_plus/device_info_plus.dart';
import '../models/save_data.dart';
import '../utils/app_config.dart';
import '../utils/log_utils.dart';

class FeedbackService {
  static final FeedbackService _instance = FeedbackService._internal();
  static FeedbackService get instance => _instance;
  FeedbackService._internal();

  // 服务器配置 - 与iOS版本保持一致
  static const String _productionBaseUrl = 'http://www.xxxx.com';

  /// 获取API基础URL（根据开发/生产环境）
  String get _apiBaseUrl {
    return _productionBaseUrl;
    // 如果需要开发环境，可以根据 kDebugMode 切换
    // return kDebugMode ? _developmentBaseUrl : _productionBaseUrl;
  }

  /// 获取当前App版本信息
  String get _appVersion {
    // 可以通过 package_info_plus 获取，这里先使用配置中的版本
    return '1.0.0'; // TODO: 从PackageInfo获取
  }

  /// 获取当前离线包版本信息
  String get _offlinePackageVersion {
    return AppConfig.shared.offlineZipName;
  }

  /// 获取当前App名称
  String get _appName {
    final appName = AppConfig.shared.appName;
    switch (appName) {
      case AppName.hdBayeApp:
        return '三国霸业';
      case AppName.hdFmjApp:
        return '伏魔记';
      default:
        return '未知应用';
    }
  }

  /// 获取设备信息
  Future<Map<String, dynamic>> _getDeviceInfo() async {
    final deviceInfo = DeviceInfoPlugin();

    try {
      if (Platform.isIOS) {
        final iosInfo = await deviceInfo.iosInfo;
        return {
          'platform': 'iOS',
          'model': iosInfo.model,
          'systemVersion': iosInfo.systemVersion,
          'appName': _appName,
        };
      } else if (Platform.isAndroid) {
        final androidInfo = await deviceInfo.androidInfo;
        return {
          'platform': 'Android',
          'model': '${androidInfo.brand} ${androidInfo.model}',
          'systemVersion': 'Android ${androidInfo.version.release}',
          'appName': _appName,
        };
      } else {
        return {
          'platform': Platform.operatingSystem,
          'model': 'Unknown',
          'systemVersion': 'Unknown',
          'appName': _appName,
        };
      }
    } catch (e) {
      LogUtils.d('获取设备信息失败: $e');
      return {
        'platform': Platform.operatingSystem,
        'model': 'Unknown',
        'systemVersion': 'Unknown',
        'appName': _appName,
      };
    }
  }

  /// 提交用户反馈
  ///
  /// [content] 反馈内容
  /// [contact] 联系方式（可选）
  /// [saveData] 游戏存档数据（可选）
  ///
  /// 返回 (成功状态, 消息)
  Future<(bool, String?)> submitFeedback({
    required String content,
    String? contact,
    SaveData? saveData,
  }) async {
    try {
      // 1. 准备设备信息
      final deviceInfo = await _getDeviceInfo();

      // 2. 准备请求参数
      final parameters = <String, dynamic>{
        'content': content,
        'contactInfo': contact ?? '',
        'appVersion': _appVersion,
        'offlinePackageVersion': _offlinePackageVersion,
        'deviceInfo': deviceInfo,
        'feedbackType': 'user_feedback',
        'timestamp': DateTime.now().millisecondsSinceEpoch,
      };

      // 3. 处理存档数据信息
      if (saveData != null) {
        parameters['saveDataInfo'] = {
          'hasData': true,
          'saveKey': saveData.key,
          'saveTitle': saveData.displayTitle,
          'gameName': saveData.gameName,
          'gameKey': saveData.gameKey,
          'dataSize': saveData.content.length,
          'saveContent': saveData.content,
          'createdDate': saveData.createdDate?.toIso8601String() ?? '',
        };
      } else {
        parameters['saveDataInfo'] = {
          'hasData': false,
          'saveKey': '',
          'saveTitle': '',
          'gameName': '',
          'gameKey': '',
          'dataSize': 0,
          'saveContent': '',
          'createdDate': '',
        };
      }

      // 4. 创建网络请求
      final url = Uri.parse('$_apiBaseUrl/feedback');
      final response = await http.post(
        url,
        headers: {
          'Content-Type': 'application/json',
        },
        body: json.encode(parameters),
      ).timeout(const Duration(seconds: 5));

      // 5. 处理响应
      return _handleFeedbackResponse(response);

    } catch (e) {
      final errorMessage = '网络连接失败: ${e.toString()}';
      LogUtils.d('反馈提交失败: $errorMessage');
      return (false, errorMessage);
    }
  }

  /// 处理反馈提交响应
  (bool, String?) _handleFeedbackResponse(http.Response response) {
    LogUtils.d('反馈API响应状态: ${response.statusCode}');

    // 处理不同的HTTP状态码
    switch (response.statusCode) {
      case >= 200 && < 300:
        // 成功响应，解析响应数据
        try {
          final responseData = json.decode(response.body);
          if (responseData is Map<String, dynamic>) {
            final success = responseData['success'] as bool? ?? false;
            final message = responseData['message'] as String?;
            final feedbackId = responseData['feedbackId'] as String?;

            if (success) {
              const successMessage = '反馈提交成功！';
              LogUtils.d('反馈提交成功，ID: ${feedbackId ?? "unknown"}');
              return (true, successMessage);
            } else {
              final failureMessage = message ?? '提交失败，请稍后重试';
              LogUtils.d('反馈提交失败: $failureMessage');
              return (false, failureMessage);
            }
          } else {
            return (true, '反馈提交成功！');
          }
        } catch (e) {
          LogUtils.d('响应JSON解析失败: $e');
          return (false, '服务器响应解析失败');
        }

      case 400:
        return (false, '请求参数错误，请检查输入内容');

      case >= 500:
        return (false, '服务器内部错误，请稍后重试');

      default:
        return (false, '服务器错误 (状态码: ${response.statusCode})');
    }
  }
}