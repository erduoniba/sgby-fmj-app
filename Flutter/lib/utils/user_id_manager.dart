import 'dart:math';
import 'dart:io';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:crypto/crypto.dart';
import 'dart:convert';
import 'package:device_info_plus/device_info_plus.dart';
import 'log_utils.dart';

/// 用户ID管理器
/// 负责生成和缓存唯一的用户ID
///
/// 新方案：基于设备硬件标识符生成稳定的 userId
/// - iOS: 使用 identifierForVendor (同一厂商下的应用共享，卸载不变)
/// - Android: 使用 androidId (设备唯一，卸载不变)
/// - 其他平台: 降级到时间戳+随机数方案
class UserIdManager {
  static const String _userIdKey = 'user_id';
  static const String _deviceIdKey = 'device_id';
  static String? _cachedUserId;
  static const String _defaultUserId = 'default_user_000000';

  /// 获取用户ID（如果不存在则生成）
  ///
  /// 工作流程：
  /// 1. 尝试从内存缓存获取
  /// 2. 尝试从 SharedPreferences 获取
  /// 3. 尝试从设备硬件标识符生成（卸载后仍然一致）
  /// 4. 降级到随机生成（最后手段）
  static Future<String> getUserId() async {
    // 如果内存中有缓存，直接返回
    if (_cachedUserId != null && _cachedUserId!.isNotEmpty) {
      LogUtils.d('UserIdManager 从内存缓存获取用户ID: $_cachedUserId');
      return _cachedUserId!;
    }

    try {
      final prefs = await SharedPreferences.getInstance();

      // 1. 尝试从 SharedPreferences 获取已保存的 userId
      String? storedUserId = prefs.getString(_userIdKey);
      String? storedDeviceId = prefs.getString(_deviceIdKey);

      // 2. 获取当前设备的硬件标识符
      String currentDeviceId = await _getDeviceIdentifier();

      LogUtils.d('UserIdManager 设备标识符 - 已保存: $storedDeviceId, 当前: $currentDeviceId');

      // 3. 判断场景
      if (storedUserId != null && storedUserId.isNotEmpty) {
        if (storedDeviceId == currentDeviceId) {
          // 场景A: 同一设备，正常使用
          LogUtils.d('UserIdManager 从缓存获取用户ID (同一设备): $storedUserId');
          _cachedUserId = storedUserId;
          return storedUserId;
        } else {
          // 场景B: 设备标识符变化（卸载重装），但有旧的 userId
          // 这种情况下，基于新的设备标识符重新生成 userId
          LogUtils.d('UserIdManager 检测到设备标识符变化，重新生成 userId');
        }
      }

      // 4. 基于设备标识符生成稳定的 userId
      String newUserId = _generateUserIdFromDevice(currentDeviceId);

      // 5. 保存到本地存储
      await prefs.setString(_userIdKey, newUserId);
      await prefs.setString(_deviceIdKey, currentDeviceId);

      // 6. 缓存到内存
      _cachedUserId = newUserId;

      LogUtils.d('UserIdManager 生成新用户ID: $newUserId (基于设备: ${currentDeviceId.substring(0, 8)}...)');
      return newUserId;

    } catch (e) {
      LogUtils.d('UserIdManager ERROR 获取用户ID失败: $e');
      // 发生错误时使用默认ID
      return _defaultUserId;
    }
  }

  /// 获取设备唯一标识符
  ///
  /// iOS: identifierForVendor (卸载后仍然一致)
  /// Android: androidId (卸载后仍然一致)
  /// 其他平台: 生成随机ID并保存
  static Future<String> _getDeviceIdentifier() async {
    try {
      final deviceInfo = DeviceInfoPlugin();

      if (Platform.isIOS) {
        // iOS: 使用 identifierForVendor
        final iosInfo = await deviceInfo.iosInfo;
        String? identifier = iosInfo.identifierForVendor;

        if (identifier != null && identifier.isNotEmpty) {
          LogUtils.d('UserIdManager iOS identifierForVendor: $identifier');
          return identifier;
        } else {
          // 降级方案：使用设备名称 + 系统版本的哈希
          String fallback = '${iosInfo.name}_${iosInfo.systemVersion}_${iosInfo.model}';
          return _hashString(fallback);
        }

      } else if (Platform.isAndroid) {
        // Android: 使用 androidId
        final androidInfo = await deviceInfo.androidInfo;
        String androidId = androidInfo.id;

        if (androidId.isNotEmpty) {
          LogUtils.d('UserIdManager Android ID: $androidId');
          return androidId;
        } else {
          // 降级方案：使用设备信息的哈希
          String fallback = '${androidInfo.brand}_${androidInfo.device}_${androidInfo.model}';
          return _hashString(fallback);
        }

      } else {
        // 其他平台（Web、Windows、macOS、Linux）
        // 生成一个随机ID并保存
        final prefs = await SharedPreferences.getInstance();
        String? savedId = prefs.getString('fallback_device_id');

        if (savedId != null && savedId.isNotEmpty) {
          return savedId;
        }

        // 生成新的随机设备ID
        String randomId = _hashString('${DateTime.now().millisecondsSinceEpoch}_${Random().nextInt(999999999)}');
        await prefs.setString('fallback_device_id', randomId);
        LogUtils.d('UserIdManager 其他平台生成随机设备ID: $randomId');
        return randomId;
      }

    } catch (e) {
      LogUtils.d('UserIdManager ERROR 获取设备标识符失败: $e');
      // 最终降级方案
      return _hashString('fallback_${DateTime.now().millisecondsSinceEpoch}');
    }
  }

  /// 基于设备标识符生成用户ID
  ///
  /// 关键设计：使用纯设备标识符的哈希，确保卸载重装后 userId 完全一致
  ///
  /// 格式: device_[SHA256前32位]
  /// - 完全基于设备硬件标识符的 SHA256 哈希
  /// - iOS: 基于 identifierForVendor
  /// - Android: 基于 androidId
  ///
  /// 优势：
  /// 1. ✅ 同一设备卸载重装后，userId 完全一致
  /// 2. ✅ 不依赖时间戳，永久稳定
  /// 3. ✅ 唯一性由硬件标识符保证
  static String _generateUserIdFromDevice(String deviceId) {
    try {
      // 计算设备ID的完整 SHA256 哈希
      String fullHash = _hashString(deviceId);

      // 取前32位作为用户ID（足够唯一且简洁）
      String userId = 'device_${fullHash.substring(0, 32)}';

      LogUtils.d('UserIdManager 用户ID生成详情 - 设备标识符: ${deviceId.substring(0, min(16, deviceId.length))}..., 用户ID: $userId');

      return userId;

    } catch (e) {
      LogUtils.d('UserIdManager ERROR 生成用户ID异常: $e');
      // 异常时生成简单的ID（仍然基于设备ID的哈希）
      return 'device_${_hashString('fallback_$deviceId').substring(0, 32)}';
    }
  }

  /// SHA256 哈希字符串
  static String _hashString(String input) {
    var bytes = utf8.encode(input);
    var digest = sha256.convert(bytes);
    return digest.toString();
  }

  /// 重置用户ID（用于测试或特殊情况）
  static Future<void> resetUserId() async {
    try {
      final prefs = await SharedPreferences.getInstance();
      await prefs.remove(_userIdKey);
      _cachedUserId = null;
      LogUtils.d('${'UserIdManager'} 用户ID已重置');
    } catch (e) {
      LogUtils.d('${'UserIdManager ERROR'} 重置用户ID失败: $e');
    }
  }

  /// 获取当前缓存的用户ID（不生成新的）
  static String? getCachedUserId() {
    return _cachedUserId;
  }

  /// 验证用户ID格式是否正确
  ///
  /// 支持两种格式：
  /// 1. 新格式: device_[32位哈希]
  /// 2. 旧格式: timestamp_randomnum_uuid (已废弃，但仍然支持)
  static bool isValidUserId(String userId) {
    if (userId.isEmpty) return false;

    // 检查是否为默认ID
    if (userId == _defaultUserId) return true;

    // 检查新格式: device_[32位哈希]
    if (userId.startsWith('device_')) {
      List<String> parts = userId.split('_');
      if (parts.length >= 2) {
        String hash = parts[1];
        // 验证哈希部分为32位十六进制字符
        if (hash.length == 32 && RegExp(r'^[a-f0-9]{32}$').hasMatch(hash)) {
          return true;
        }
      }
    }

    // 检查旧格式: timestamp_randomnum_uuid
    List<String> parts = userId.split('_');
    if (parts.length == 3) {
      // 验证时间戳部分是否为数字
      if (int.tryParse(parts[0]) == null) return false;

      // 验证随机数部分是否为10位数字
      if (parts[1].length != 10 || int.tryParse(parts[1]) == null) return false;

      // 验证UUID部分长度
      if (parts[2].length != 8) return false;

      return true;
    }

    return false;
  }

  /// 获取用户ID的创建时间
  ///
  /// 注意：
  /// - 新格式 (device_xxx) 没有时间戳，返回 null
  /// - 旧格式 (timestamp_xxx_xxx) 返回创建时间
  static DateTime? getUserIdCreateTime(String userId) {
    try {
      if (!isValidUserId(userId) || userId == _defaultUserId) {
        return null;
      }

      // 新格式没有时间戳
      if (userId.startsWith('device_')) {
        return null;
      }

      // 旧格式解析时间戳
      List<String> parts = userId.split('_');
      if (parts.length == 3) {
        int timestamp = int.parse(parts[0]);
        return DateTime.fromMillisecondsSinceEpoch(timestamp);
      }

      return null;

    } catch (e) {
      LogUtils.d('UserIdManager ERROR 解析用户ID创建时间失败: $e');
      return null;
    }
  }

  /// 获取设备标识符（调试用）
  static Future<String> getDeviceIdentifier() async {
    return await _getDeviceIdentifier();
  }
}