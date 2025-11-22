import 'package:flutter/foundation.dart';
import 'package:in_app_purchase/in_app_purchase.dart';
import 'package:shared_preferences/shared_preferences.dart';
import '../utils/app_config.dart';
import '../utils/log_utils.dart';
import '../utils/secure_vip_storage.dart';

/// In-App Purchase管理器
/// 基于iOS版本的IAP功能实现
class IAPManager extends ChangeNotifier {
  static final IAPManager _instance = IAPManager._internal();
  factory IAPManager() => _instance;
  IAPManager._internal();

  final InAppPurchase _inAppPurchase = InAppPurchase.instance;
  late SharedPreferences _prefs;
  final SecureVipStorage _secureStorage = SecureVipStorage();

  // Purchase states
  bool _isAvailable = false;
  bool _isLoading = false;
  List<ProductDetails> _products = [];
  Set<String> _purchasedProductIds = <String>{};

  // Product IDs based on iOS implementation
  static const String _vipProductId = 'com.harry.fmj.vip';
  static const String _doubleExpProductId = 'com.harry.fmj.doubleExp';
  static const String _doubleGoldProductId = 'com.harry.fmj.doubleGold';

  // Getters
  bool get isAvailable => _isAvailable;
  bool get isLoading => _isLoading;
  List<ProductDetails> get products => _products;

  // VIP and feature access
  bool get isVip {
    // 检查是否购买了当前应用的VIP
    if (_purchasedProductIds.contains(_vipProductId)) return true;

    // 兼容旧的通用VIP产品ID（服务器可能返回）
    if (_purchasedProductIds.contains('com.game.vip')) return true;

    // 检查其他游戏的VIP（跨游戏共享）
    if (_purchasedProductIds.contains('com.harry.fmj.vip')) return true;
    if (_purchasedProductIds.contains('com.harry.baye.vip')) return true;

    return false;
  }

  bool get hasDoubleExp => isVip || _purchasedProductIds.contains(_doubleExpProductId);
  bool get hasDoubleGold => isVip || _purchasedProductIds.contains(_doubleGoldProductId);
  bool get hasAllFeatures => isVip; // VIP unlocks everything

  /// 获取已购买的产品ID列表（调试用）
  List<String> get purchasedProductIds => _purchasedProductIds.toList();

  /// 初始化IAP管理器
  Future<void> initialize() async {
    try {
      _prefs = await SharedPreferences.getInstance();
      await _loadPurchaseHistory();

      _isAvailable = await _inAppPurchase.isAvailable();
      if (!_isAvailable) {
        LogUtils.d('IAP not available on this device');
        return;
      }

      // 监听购买更新
      _inAppPurchase.purchaseStream.listen(
        _onPurchaseUpdated,
        onError: (error) {
          LogUtils.d('Purchase stream error: $error');
        },
      );

      await _loadProducts();
      await restorePurchases();

      LogUtils.d('IAP Manager initialized successfully');
    } catch (e) {
      LogUtils.d('Failed to initialize IAP Manager: $e');
    }
  }

  /// 加载产品信息
  Future<void> _loadProducts() async {
    if (!_isAvailable) return;

    _isLoading = true;
    notifyListeners();

    try {
      final Set<String> productIds = _getProductIdsForCurrentApp();
      final ProductDetailsResponse response = await _inAppPurchase.queryProductDetails(productIds);

      if (response.notFoundIDs.isNotEmpty) {
        LogUtils.d('Products not found: ${response.notFoundIDs}');
      }

      _products = response.productDetails;
      LogUtils.d('Loaded ${_products.length} products');
    } catch (e) {
      LogUtils.d('Failed to load products: $e');
    }

    _isLoading = false;
    notifyListeners();
  }

  /// 根据当前应用获取产品ID列表
  Set<String> _getProductIdsForCurrentApp() {
    final appName = AppConfig.shared.appName;
    final baseIds = <String>{};

    switch (appName) {
      case AppName.hdFmjApp:
        baseIds.addAll({
          'com.harry.fmj.vip',
          'com.harry.fmj.doubleExp',
          'com.harry.fmj.doubleGold',
        });
        break;
      case AppName.hdBayeApp:
        baseIds.addAll({
          'com.harry.baye.vip',
          'com.harry.baye.doubleExp',
          'com.harry.baye.doubleGold',
        });
        break;
      default:
        break;
    }

    return baseIds;
  }

  /// 发起购买
  Future<bool> makePurchase(ProductDetails product) async {
    if (!_isAvailable) {
      LogUtils.d('IAP not available');
      return false;
    }

    try {
      LogUtils.d('Making purchase for: ${product.id}');
      final PurchaseParam purchaseParam = PurchaseParam(productDetails: product);
      await _inAppPurchase.buyNonConsumable(purchaseParam: purchaseParam);
      return true;
    } catch (e) {
      LogUtils.d('Failed to make purchase: $e');
      return false;
    }
  }

  /// 恢复购买
  Future<void> restorePurchases() async {
    if (!_isAvailable) return;

    try {
      LogUtils.d('Restoring purchases...');
      await _inAppPurchase.restorePurchases();
      LogUtils.d('Purchase restoration completed');
    } catch (e) {
      LogUtils.d('Failed to restore purchases: $e');
    }
  }

  /// 处理购买更新
  void _onPurchaseUpdated(List<PurchaseDetails> purchaseDetailsList) {
    for (final PurchaseDetails purchaseDetails in purchaseDetailsList) {
      LogUtils.d('Purchase update: ${purchaseDetails.productID} - ${purchaseDetails.status}');

      if (purchaseDetails.status == PurchaseStatus.purchased) {
        _handlePurchaseSuccess(purchaseDetails);
      } else if (purchaseDetails.status == PurchaseStatus.error) {
        _handlePurchaseError(purchaseDetails);
      } else if (purchaseDetails.status == PurchaseStatus.restored) {
        _handlePurchaseRestored(purchaseDetails);
      }

      // 完成购买流程
      if (purchaseDetails.pendingCompletePurchase) {
        _inAppPurchase.completePurchase(purchaseDetails);
      }
    }
  }

  /// 处理购买成功
  void _handlePurchaseSuccess(PurchaseDetails purchaseDetails) {
    LogUtils.d('Purchase successful: ${purchaseDetails.productID}');
    _purchasedProductIds.add(purchaseDetails.productID);
    _savePurchaseHistory();
    notifyListeners();
  }

  /// 处理购买错误
  void _handlePurchaseError(PurchaseDetails purchaseDetails) {
    LogUtils.d('Purchase error: ${purchaseDetails.error}');
    // 可以在这里显示错误消息给用户
  }

  /// 处理购买恢复
  void _handlePurchaseRestored(PurchaseDetails purchaseDetails) {
    LogUtils.d('Purchase restored: ${purchaseDetails.productID}');
    _purchasedProductIds.add(purchaseDetails.productID);
    _savePurchaseHistory();
    notifyListeners();
  }

  /// 保存购买历史到本地（同时保存到安全存储和SharedPreferences）
  Future<void> _savePurchaseHistory() async {
    final purchaseList = _purchasedProductIds.toList();

    // 1. 保存到 SharedPreferences（快速访问，但卸载后会清除）
    _prefs.setStringList('purchased_products', purchaseList);
    LogUtils.d('Saved purchase history to SharedPreferences: $purchaseList');

    // 2. 保存到安全存储（持久化，卸载后依然保留）
    await _secureStorage.savePurchasedProducts(purchaseList);
    LogUtils.d('Saved purchase history to secure storage: $purchaseList');
  }

  /// 从本地加载购买历史（优先从安全存储加载）
  Future<void> _loadPurchaseHistory() async {
    // 1. 首先尝试从安全存储加载（卸载后依然保留的数据）
    final secureProducts = await _secureStorage.loadPurchasedProducts();

    // 2. 然后从 SharedPreferences 加载
    final prefsProducts = _prefs.getStringList('purchased_products') ?? [];

    // 3. 合并两个来源的数据（去重）
    final allProducts = <String>{...secureProducts, ...prefsProducts};
    _purchasedProductIds = allProducts;

    LogUtils.d('Loaded purchase history - Secure: $secureProducts, Prefs: $prefsProducts, Merged: $allProducts');

    // 4. 如果安全存储有数据，更新 SharedPreferences
    if (secureProducts.isNotEmpty && allProducts.isNotEmpty) {
      _prefs.setStringList('purchased_products', allProducts.toList());
      LogUtils.d('Updated SharedPreferences with merged data');
    }
  }

  /// 获取产品详情
  ProductDetails? getProductDetails(String productId) {
    try {
      return _products.firstWhere((product) => product.id == productId);
    } catch (e) {
      return null;
    }
  }

  /// 检查是否已购买某个产品
  bool isPurchased(String productId) {
    return _purchasedProductIds.contains(productId);
  }

  /// 获取VIP产品详情
  ProductDetails? get vipProduct => getProductDetails(_getVipProductIdForCurrentApp());

  /// 获取双倍经验产品详情
  ProductDetails? get doubleExpProduct => getProductDetails(_getDoubleExpProductIdForCurrentApp());

  /// 获取双倍金币产品详情
  ProductDetails? get doubleGoldProduct => getProductDetails(_getDoubleGoldProductIdForCurrentApp());

  /// 根据当前应用获取VIP产品ID
  String _getVipProductIdForCurrentApp() {
    switch (AppConfig.shared.appName) {
      case AppName.hdFmjApp:
        return 'com.harry.fmj.vip';
      case AppName.hdBayeApp:
        return 'com.harry.baye.vip';
      default:
        return 'com.harry.fmj.vip';
    }
  }

  /// 根据当前应用获取双倍经验产品ID
  String _getDoubleExpProductIdForCurrentApp() {
    switch (AppConfig.shared.appName) {
      case AppName.hdFmjApp:
        return 'com.harry.fmj.doubleExp';
      case AppName.hdBayeApp:
        return 'com.harry.baye.doubleExp';
      default:
        return 'com.harry.fmj.doubleExp';
    }
  }

  /// 根据当前应用获取双倍金币产品ID
  String _getDoubleGoldProductIdForCurrentApp() {
    switch (AppConfig.shared.appName) {
      case AppName.hdFmjApp:
        return 'com.harry.fmj.doubleGold';
      case AppName.hdBayeApp:
        return 'com.harry.baye.doubleGold';
      default:
        return 'com.harry.fmj.doubleGold';
    }
  }

  /// 清理资源
  @override
  void dispose() {
    // Note: 不能dispose InAppPurchase.instance，它是单例
    super.dispose();
  }

  /// 开发测试方法 - 清除所有购买记录
  Future<void> clearAllPurchasesForTesting() async {
    if (kDebugMode) {
      _purchasedProductIds.clear();
      _prefs.remove('purchased_products');
      await _secureStorage.clearAll();
      notifyListeners();
      LogUtils.d('Cleared all purchases (including secure storage) for testing');
    }
  }

  /// 开发测试方法 - 模拟购买VIP
  void simulateVipPurchaseForTesting() {
    if (kDebugMode) {
      _purchasedProductIds.add(_getVipProductIdForCurrentApp());
      _savePurchaseHistory();
      notifyListeners();
      LogUtils.d('Simulated VIP purchase for testing');
    }
  }

  /// 通过内购码激活VIP（用于生产环境）
  /// 此方法由 IAPCodeService 在成功兑换内购码后调用
  void activateVipByRedemptionCode(String productId) {
    LogUtils.d('Activating VIP by redemption code: $productId');
    _purchasedProductIds.add(productId);
    _savePurchaseHistory();
    notifyListeners();
    LogUtils.d('VIP activated successfully via redemption code');
  }

  /// 确保VIP状态已持久化（供外部调用）
  /// 当用户进入VIP页面时，确保已购买的VIP状态保存到安全存储
  Future<void> ensureVipPersisted() async {
    if (_purchasedProductIds.isNotEmpty) {
      LogUtils.d('Ensuring VIP status is persisted to secure storage...');
      await _savePurchaseHistory();
      LogUtils.d('VIP status persistence confirmed - Products: $_purchasedProductIds');
    } else {
      LogUtils.d('No purchased products to persist');
    }
  }
}