import 'package:flutter_secure_storage/flutter_secure_storage.dart';
import 'log_utils.dart';

/// 安全的VIP状态持久化存储
/// 使用 Keychain (iOS) 和 EncryptedSharedPreferences (Android)
/// 数据在卸载后依然保留
class SecureVipStorage {
  static final SecureVipStorage _instance = SecureVipStorage._internal();
  factory SecureVipStorage() => _instance;
  SecureVipStorage._internal();

  // 使用 flutter_secure_storage 进行安全存储
  final FlutterSecureStorage _storage = const FlutterSecureStorage(
    aOptions: AndroidOptions(
      encryptedSharedPreferences: true,
      // 使用 EncryptedSharedPreferences，卸载后数据保留
      resetOnError: true,
    ),
    iOptions: IOSOptions(
      accessibility: KeychainAccessibility.first_unlock,
      // 使用 Keychain，卸载后数据保留
    ),
  );

  static const String _vipProductsKey = 'vip_purchased_products';

  /// 保存已购买的VIP产品列表
  Future<void> savePurchasedProducts(List<String> productIds) async {
    try {
      final value = productIds.join(',');
      await _storage.write(key: _vipProductsKey, value: value);
      LogUtils.d('SecureVipStorage: Saved VIP products to secure storage: $productIds');
    } catch (e) {
      LogUtils.d('SecureVipStorage: Failed to save VIP products: $e');
    }
  }

  /// 加载已购买的VIP产品列表
  Future<List<String>> loadPurchasedProducts() async {
    try {
      final value = await _storage.read(key: _vipProductsKey);
      if (value != null && value.isNotEmpty) {
        final products = value.split(',').where((p) => p.isNotEmpty).toList();
        LogUtils.d('SecureVipStorage: Loaded VIP products from secure storage: $products');
        return products;
      }
      LogUtils.d('SecureVipStorage: No VIP products found in secure storage');
      return [];
    } catch (e) {
      LogUtils.d('SecureVipStorage: Failed to load VIP products: $e');
      return [];
    }
  }

  /// 添加一个购买的产品
  Future<void> addPurchasedProduct(String productId) async {
    try {
      final products = await loadPurchasedProducts();
      if (!products.contains(productId)) {
        products.add(productId);
        await savePurchasedProducts(products);
        LogUtils.d('SecureVipStorage: Added product to secure storage: $productId');
      }
    } catch (e) {
      LogUtils.d('SecureVipStorage: Failed to add product: $e');
    }
  }

  /// 检查是否购买了某个产品
  Future<bool> hasPurchasedProduct(String productId) async {
    try {
      final products = await loadPurchasedProducts();
      return products.contains(productId);
    } catch (e) {
      LogUtils.d('SecureVipStorage: Failed to check product: $e');
      return false;
    }
  }

  /// 清除所有VIP数据（仅用于测试）
  Future<void> clearAll() async {
    try {
      await _storage.delete(key: _vipProductsKey);
      LogUtils.d('SecureVipStorage: Cleared all VIP data');
    } catch (e) {
      LogUtils.d('SecureVipStorage: Failed to clear VIP data: $e');
    }
  }

  /// 检查安全存储是否可用
  Future<bool> isAvailable() async {
    try {
      await _storage.read(key: 'test_key');
      return true;
    } catch (e) {
      LogUtils.d('SecureVipStorage: Secure storage not available: $e');
      return false;
    }
  }
}
