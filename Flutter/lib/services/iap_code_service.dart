import 'package:http/http.dart' as http;
import 'dart:convert';
import '../utils/log_utils.dart';
import '../utils/user_id_manager.dart';
import 'iap_manager.dart';

/// VIP内购码兑换服务
class IAPCodeService {
  static final IAPCodeService _instance = IAPCodeService._internal();
  factory IAPCodeService() => _instance;
  IAPCodeService._internal();

  // 服务器API地址
  static const String _apiBaseUrl = 'http://www.xxxx.com';

  /// 验证内购码是否有效
  ///
  /// 返回值：
  /// - {'success': true, 'productId': '...', 'productName': '...'} 验证成功
  /// - {'success': false, 'error': '错误信息'} 验证失败
  Future<Map<String, dynamic>> validateCode(String code) async {
    try {
      LogUtils.d('开始验证内购码: $code');

      final url = Uri.parse('$_apiBaseUrl/api/iap-codes/validate/$code');
      final response = await http.get(url).timeout(
        const Duration(seconds: 10),
        onTimeout: () {
          throw Exception('请求超时，请检查网络连接');
        },
      );

      LogUtils.d('验证响应状态码: ${response.statusCode}');
      LogUtils.d('验证响应内容: ${response.body}');

      if (response.statusCode == 200) {
        final data = json.decode(response.body);
        return {
          'success': data['success'] == true,
          'productId': data['data']?['productId'],
          'productName': data['data']?['productName'],
          'error': data['success'] == true ? null : data['error'],
        };
      } else {
        return {
          'success': false,
          'error': '服务器错误: ${response.statusCode}',
        };
      }
    } catch (e) {
      LogUtils.d('验证内购码异常: $e');
      return {
        'success': false,
        'error': '验证失败: ${e.toString()}',
      };
    }
  }

  /// 兑换内购码
  ///
  /// 成功后会自动更新IAPManager的购买状态
  Future<Map<String, dynamic>> redeemCode(String code) async {
    try {
      LogUtils.d('开始兑换内购码: $code');

      // 获取用户ID
      final userId = await UserIdManager.getUserId();
      LogUtils.d('用户ID: $userId');

      final url = Uri.parse('$_apiBaseUrl/api/iap-codes/redeem');
      final response = await http.post(
        url,
        headers: {
          'Content-Type': 'application/json',
        },
        body: json.encode({
          'code': code,
          'userId': userId,
        }),
      ).timeout(
        const Duration(seconds: 10),
        onTimeout: () {
          throw Exception('请求超时，请检查网络连接');
        },
      );

      LogUtils.d('兑换响应状态码: ${response.statusCode}');
      LogUtils.d('兑换响应内容: ${response.body}');

      if (response.statusCode == 200) {
        final data = json.decode(response.body);

        if (data['success'] == true) {
          // 兑换成功，更新本地购买状态
          final productId = data['data']?['productId'] as String?;
          if (productId != null) {
            LogUtils.d('兑换成功，产品ID: $productId');

            // 将购买状态保存到IAPManager
            final iapManager = IAPManager();
            iapManager.activateVipByRedemptionCode(productId); // 通过内购码激活VIP

            return {
              'success': true,
              'productId': productId,
              'productName': data['data']?['productName'],
              'message': data['message'] ?? '兑换成功',
            };
          }
        }

        return {
          'success': false,
          'error': data['error'] ?? '兑换失败',
        };
      } else if (response.statusCode == 400) {
        final data = json.decode(response.body);
        return {
          'success': false,
          'error': data['error'] ?? '内购码无效或已使用',
        };
      } else {
        return {
          'success': false,
          'error': '服务器错误: ${response.statusCode}',
        };
      }
    } catch (e) {
      LogUtils.d('兑换内购码异常: $e');
      return {
        'success': false,
        'error': '兑换失败: ${e.toString()}',
      };
    }
  }

  /// 查询用户已兑换的内购码历史
  Future<List<Map<String, dynamic>>> getRedemptionHistory() async {
    try {
      final userId = await UserIdManager.getUserId();

      final url = Uri.parse('$_apiBaseUrl/api/iap-codes/user/$userId/history');
      final response = await http.get(url).timeout(
        const Duration(seconds: 10),
        onTimeout: () {
          throw Exception('请求超时，请检查网络连接');
        },
      );

      if (response.statusCode == 200) {
        final data = json.decode(response.body);
        if (data['success'] == true) {
          final List<dynamic> history = data['data'] ?? [];
          return history.cast<Map<String, dynamic>>();
        }
      }

      return [];
    } catch (e) {
      LogUtils.d('获取兑换历史异常: $e');
      return [];
    }
  }
}
