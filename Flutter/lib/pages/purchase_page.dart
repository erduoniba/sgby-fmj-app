import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:url_launcher/url_launcher.dart';
import '../services/iap_manager.dart';
import '../services/iap_code_service.dart';
import '../utils/app_config.dart';
import '../utils/log_utils.dart';
import '../utils/js_bridge.dart';
import '../utils/user_id_manager.dart';
import '../utils/multiplier_settings.dart';

class PurchasePage extends StatefulWidget {
  const PurchasePage({super.key});

  @override
  State<PurchasePage> createState() => _PurchasePageState();
}

class _PurchasePageState extends State<PurchasePage> {
  final IAPManager _iapManager = IAPManager();
  final IAPCodeService _codeService = IAPCodeService();
  final TextEditingController _codeController = TextEditingController();
  bool _isProcessing = false;
  bool _showCodeInput = false;
  String _userId = '';
  String _deviceId = '';

  // 倍率值状态 (1x, 2x, 5x, 10x)
  int _expMultiplier = 5;
  int _goldMultiplier = 5;
  int _itemMultiplier = 5;

  final List<int> _multiplierValues = [1, 2, 5, 10];
  final List<String> _multiplierNames = ["1x", "2x", "5x", "10x"];

  @override
  void initState() {
    super.initState();
    _iapManager.addListener(_onPurchaseStateChanged);
    _loadUserInfo();
    _printVipStatus();
    _ensureVipStatusPersisted();
    _loadMultiplierSettings();
  }

  /// 加载倍率设置
  Future<void> _loadMultiplierSettings() async {
    final settings = MultiplierSettings();
    final expMultiplier = await settings.expMultiplier;
    final goldMultiplier = await settings.goldMultiplier;
    final itemMultiplier = await settings.itemMultiplier;

    if (mounted) {
      setState(() {
        _expMultiplier = expMultiplier;
        _goldMultiplier = goldMultiplier;
        _itemMultiplier = itemMultiplier;
      });
    }

    LogUtils.d('Loaded multiplier settings: exp=$expMultiplier, gold=$goldMultiplier, item=$itemMultiplier');
  }

  /// 打印VIP购买状态（调试用）
  void _printVipStatus() {
    LogUtils.d('=== VIP会员页面状态检查 ===');
    LogUtils.d('是否已购买VIP: ${_iapManager.isVip}');
    LogUtils.d('已购买的产品数量: ${_iapManager.purchasedProductIds.length}');
    LogUtils.d('已购买的产品列表: ${_iapManager.purchasedProductIds}');
    LogUtils.d('当前游戏: ${AppConfig.shared.appName}');
    LogUtils.d('===========================');
  }

  /// 确保VIP状态已持久化保存（进入页面时检查）
  Future<void> _ensureVipStatusPersisted() async {
    if (_iapManager.isVip && _iapManager.purchasedProductIds.isNotEmpty) {
      LogUtils.d('检测到VIP已购买，确保状态已持久化保存...');
      await _iapManager.ensureVipPersisted();
    }
  }

  /// 加载用户信息
  Future<void> _loadUserInfo() async {
    try {
      final userId = await UserIdManager.getUserId();
      final deviceId = await UserIdManager.getDeviceIdentifier();

      if (mounted) {
        setState(() {
          _userId = userId;
          _deviceId = deviceId;
        });
      }

      LogUtils.d('Purchase Page - User ID: $_userId');
      LogUtils.d('Purchase Page - Device ID: $_deviceId');
    } catch (e) {
      LogUtils.d('Failed to load user info: $e');
    }
  }

  @override
  void dispose() {
    _iapManager.removeListener(_onPurchaseStateChanged);
    _codeController.dispose();
    super.dispose();
  }

  void _onPurchaseStateChanged() {
    if (mounted) {
      setState(() {});
    }
  }

  /// 添加所有物品到游戏（VIP特权）
  Future<void> _addAllItems() async {
    // 检查是否为伏魔记游戏
    if (AppConfig.shared.appName != AppName.hdFmjApp) {
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(
          content: Text('此功能仅在伏魔记游戏中可用'),
          backgroundColor: Colors.orange,
          duration: Duration(seconds: 2),
        ),
      );
      return;
    }

    // 显示确认对话框
    final confirmed = await showDialog<bool>(
      context: context,
      builder: (context) => AlertDialog(
        title: Row(
          children: [
            Icon(Icons.inventory_2, color: AppConfig.shared.primaryColor),
            const SizedBox(width: 12),
            const Text('添加所有物品'),
          ],
        ),
        content: const Text(
          '确定要添加所有物品到当前存档吗？\n\n'
          '注意：\n'
          '• 每种物品将添加 3 个\n'
          '• 请确保游戏已加载完成\n'
          '• 添加后请手动保存游戏',
        ),
        actions: [
          TextButton(
            onPressed: () => Navigator.pop(context, false),
            child: const Text('取消'),
          ),
          ElevatedButton(
            onPressed: () => Navigator.pop(context, true),
            style: ElevatedButton.styleFrom(
              backgroundColor: AppConfig.shared.primaryColor,
            ),
            child: const Text('确认添加'),
          ),
        ],
      ),
    );

    if (confirmed != true) return;

    // 显示加载指示器
    if (mounted) {
      showDialog(
        context: context,
        barrierDismissible: false,
        builder: (context) => const Center(
          child: Card(
            child: Padding(
              padding: EdgeInsets.all(24),
              child: Column(
                mainAxisSize: MainAxisSize.min,
                children: [
                  CircularProgressIndicator(),
                  SizedBox(height: 16),
                  Text('正在添加物品...'),
                ],
              ),
            ),
          ),
        ),
      );
    }

    try {
      // 调用 JSBridge 添加所有物品
      final result = await JSBridge.addAllItems('com.harry.fmj.vip');

      if (mounted) {
        // 关闭加载指示器
        Navigator.pop(context);

        if (result['success'] == true) {
          // 成功
          showDialog(
            context: context,
            builder: (context) => AlertDialog(
              title: Row(
                children: [
                  Icon(Icons.check_circle, color: Colors.green, size: 32),
                  const SizedBox(width: 12),
                  const Text('添加成功'),
                ],
              ),
              content: Text(result['message'] ?? '所有物品已成功添加到游戏中！'),
              actions: [
                ElevatedButton(
                  onPressed: () => Navigator.pop(context),
                  style: ElevatedButton.styleFrom(
                    backgroundColor: Colors.green,
                  ),
                  child: const Text('确定'),
                ),
              ],
            ),
          );
        } else {
          // 失败
          ScaffoldMessenger.of(context).showSnackBar(
            SnackBar(
              content: Text(result['message'] ?? '添加物品失败，请重试'),
              backgroundColor: Colors.red,
              duration: const Duration(seconds: 3),
            ),
          );
        }
      }
    } catch (e) {
      LogUtils.d('添加物品异常: $e');
      if (mounted) {
        // 关闭加载指示器
        Navigator.pop(context);

        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(
            content: Text('添加物品失败: ${e.toString()}'),
            backgroundColor: Colors.red,
            duration: const Duration(seconds: 3),
          ),
        );
      }
    }
  }

  /// 跳转到QQ添加好友
  Future<void> _openQQ() async {
    const qqNumber = '820224533';

    // 尝试打开QQ
    final qqUrl = Uri.parse('mqqwpa://im/chat?chat_type=wpa&uin=$qqNumber&version=1');

    try {
      final canLaunch = await canLaunchUrl(qqUrl);
      if (canLaunch) {
        await launchUrl(qqUrl, mode: LaunchMode.externalApplication);
      } else {
        // 如果无法打开QQ，显示QQ号让用户手动添加
        if (mounted) {
          showDialog(
            context: context,
            builder: (context) => AlertDialog(
              title: const Text('添加QQ好友'),
              content: Column(
                mainAxisSize: MainAxisSize.min,
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  const Text('请手动添加QQ好友购买VIP：'),
                  const SizedBox(height: 16),
                  Row(
                    children: [
                      Expanded(
                        child: Text(
                          qqNumber,
                          style: const TextStyle(
                            fontSize: 18,
                            fontWeight: FontWeight.bold,
                            color: Colors.blue,
                          ),
                        ),
                      ),
                      IconButton(
                        icon: const Icon(Icons.copy),
                        onPressed: () {
                          Clipboard.setData(const ClipboardData(text: qqNumber));
                          ScaffoldMessenger.of(context).showSnackBar(
                            const SnackBar(
                              content: Text('QQ号已复制到剪贴板'),
                              backgroundColor: Colors.green,
                              duration: Duration(seconds: 2),
                            ),
                          );
                        },
                      ),
                    ],
                  ),
                ],
              ),
              actions: [
                TextButton(
                  onPressed: () => Navigator.pop(context),
                  child: const Text('确定'),
                ),
              ],
            ),
          );
        }
      }
    } catch (e) {
      LogUtils.d('打开QQ失败: $e');
      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          const SnackBar(
            content: Text('无法打开QQ，请手动添加QQ好友: 820224533'),
            backgroundColor: Colors.orange,
            duration: Duration(seconds: 3),
          ),
        );
      }
    }
  }

  /// 跳转到微信添加好友
  Future<void> _openWeChat() async {
    const weChatNumber = '17759190102';

    // 尝试打开微信
    final weChatUrl = Uri.parse('weixin://');

    try {
      final canLaunch = await canLaunchUrl(weChatUrl);
      if (canLaunch) {
        await launchUrl(weChatUrl, mode: LaunchMode.externalApplication);
        // 微信打开后显示微信号给用户
        if (mounted) {
          Future.delayed(const Duration(milliseconds: 500), () {
            if (mounted) {
              showDialog(
                context: context,
                builder: (context) => AlertDialog(
                  title: const Text('添加微信好友'),
                  content: Column(
                    mainAxisSize: MainAxisSize.min,
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      const Text('请在微信中搜索并添加微信号：'),
                      const SizedBox(height: 16),
                      Row(
                        children: [
                          Expanded(
                            child: Text(
                              weChatNumber,
                              style: const TextStyle(
                                fontSize: 18,
                                fontWeight: FontWeight.bold,
                                color: Colors.green,
                              ),
                            ),
                          ),
                          IconButton(
                            icon: const Icon(Icons.copy),
                            onPressed: () {
                              Clipboard.setData(const ClipboardData(text: weChatNumber));
                              ScaffoldMessenger.of(context).showSnackBar(
                                const SnackBar(
                                  content: Text('微信号已复制到剪贴板'),
                                  backgroundColor: Colors.green,
                                  duration: Duration(seconds: 2),
                                ),
                              );
                            },
                          ),
                        ],
                      ),
                    ],
                  ),
                  actions: [
                    TextButton(
                      onPressed: () => Navigator.pop(context),
                      child: const Text('确定'),
                    ),
                  ],
                ),
              );
            }
          });
        }
      } else {
        // 如果无法打开微信，直接显示微信号让用户手动添加
        if (mounted) {
          showDialog(
            context: context,
            builder: (context) => AlertDialog(
              title: const Text('添加微信好友'),
              content: Column(
                mainAxisSize: MainAxisSize.min,
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  const Text('请手动添加微信好友购买VIP：'),
                  const SizedBox(height: 16),
                  Row(
                    children: [
                      Expanded(
                        child: Text(
                          weChatNumber,
                          style: const TextStyle(
                            fontSize: 18,
                            fontWeight: FontWeight.bold,
                            color: Colors.green,
                          ),
                        ),
                      ),
                      IconButton(
                        icon: const Icon(Icons.copy),
                        onPressed: () {
                          Clipboard.setData(const ClipboardData(text: weChatNumber));
                          ScaffoldMessenger.of(context).showSnackBar(
                            const SnackBar(
                              content: Text('微信号已复制到剪贴板'),
                              backgroundColor: Colors.green,
                              duration: Duration(seconds: 2),
                            ),
                          );
                        },
                      ),
                    ],
                  ),
                ],
              ),
              actions: [
                TextButton(
                  onPressed: () => Navigator.pop(context),
                  child: const Text('确定'),
                ),
              ],
            ),
          );
        }
      }
    } catch (e) {
      LogUtils.d('打开微信失败: $e');
      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          const SnackBar(
            content: Text('无法打开微信，请手动添加微信好友: 17759190102'),
            backgroundColor: Colors.orange,
            duration: Duration(seconds: 3),
          ),
        );
      }
    }
  }

  /// 兑换内购码
  Future<void> _redeemCode() async {
    final code = _codeController.text.trim();
    if (code.isEmpty) {
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(
          content: Text('请输入内购码'),
          backgroundColor: Colors.orange,
        ),
      );
      return;
    }

    // 显示确认对话框
    final confirmed = await showDialog<bool>(
      context: context,
      builder: (context) => AlertDialog(
        title: const Text('确认兑换'),
        content: Text('确定要兑换内购码 $code 吗？\n\n兑换后将无法撤销。'),
        actions: [
          TextButton(
            onPressed: () => Navigator.pop(context, false),
            child: const Text('取消'),
          ),
          ElevatedButton(
            onPressed: () => Navigator.pop(context, true),
            style: ElevatedButton.styleFrom(
              backgroundColor: AppConfig.shared.primaryColor,
            ),
            child: const Text('确认兑换'),
          ),
        ],
      ),
    );

    if (confirmed != true) return;

    setState(() {
      _isProcessing = true;
    });

    try {
      final result = await _codeService.redeemCode(code);

      if (mounted) {
        if (result['success'] == true) {
          // 兑换成功
          _codeController.clear();
          setState(() {
            _showCodeInput = false;
          });

          showDialog(
            context: context,
            barrierDismissible: false,
            builder: (context) => AlertDialog(
              title: Row(
                children: [
                  Icon(Icons.check_circle, color: Colors.green, size: 32),
                  const SizedBox(width: 12),
                  const Text('兑换成功'),
                ],
              ),
              content: Column(
                mainAxisSize: MainAxisSize.min,
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text('恭喜您成功兑换：${result['productName']}'),
                  const SizedBox(height: 12),
                  const Text(
                    'VIP功能已激活，享受以下特权：',
                    style: TextStyle(fontWeight: FontWeight.bold),
                  ),
                  const SizedBox(height: 8),
                  const Text('• 获取所有物品 x3'),
                  const Text('• 5倍经验值加成'),
                  const Text('• 5倍金币加成'),
                  const Text('• 显示游戏小地图'),
                  const Text('• 移除所有广告'),
                ],
              ),
              actions: [
                ElevatedButton(
                  onPressed: () {
                    Navigator.pop(context); // 只关闭对话框，停留在VIP页面显示已激活状态
                  },
                  style: ElevatedButton.styleFrom(
                    backgroundColor: AppConfig.shared.primaryColor,
                  ),
                  child: const Text('确定'),
                ),
              ],
            ),
          );
        } else {
          ScaffoldMessenger.of(context).showSnackBar(
            SnackBar(
              content: Text(result['error'] ?? '兑换失败'),
              backgroundColor: Colors.red,
              duration: const Duration(seconds: 3),
            ),
          );
        }
      }
    } catch (e) {
      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(
            content: Text('兑换失败: ${e.toString()}'),
            backgroundColor: Colors.red,
            duration: const Duration(seconds: 3),
          ),
        );
      }
    }

    setState(() {
      _isProcessing = false;
    });
  }

  Widget _buildVIPCard() {
    final config = AppConfig.shared;
    final isVip = _iapManager.isVip;

    return Card(
      elevation: 4,
      margin: const EdgeInsets.all(16),
      child: Container(
        decoration: BoxDecoration(
          borderRadius: BorderRadius.circular(12),
          gradient: isVip
            ? LinearGradient(
                colors: [
                  config.primaryColor.withValues(alpha: 0.2),
                  config.primaryColor.withValues(alpha: 0.05),
                ],
                begin: Alignment.topLeft,
                end: Alignment.bottomRight,
              )
            : null,
        ),
        child: Padding(
          padding: const EdgeInsets.all(20),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Row(
                children: [
                  Container(
                    padding: const EdgeInsets.all(16),
                    decoration: BoxDecoration(
                      color: isVip ? Colors.amber : config.primaryColor.withValues(alpha: 0.1),
                      borderRadius: BorderRadius.circular(16),
                    ),
                    child: Icon(
                      Icons.diamond,
                      color: isVip ? Colors.white : config.primaryColor,
                      size: 40,
                    ),
                  ),
                  const SizedBox(width: 16),
                  Expanded(
                    child: Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: [
                        Row(
                          children: [
                            Text(
                              'VIP会员',
                              style: Theme.of(context).textTheme.headlineSmall?.copyWith(
                                fontWeight: FontWeight.bold,
                                color: isVip ? Colors.amber[700] : null,
                              ),
                            ),
                            if (isVip) ...[
                              const SizedBox(width: 8),
                              Container(
                                padding: const EdgeInsets.symmetric(horizontal: 8, vertical: 4),
                                decoration: BoxDecoration(
                                  color: Colors.green,
                                  borderRadius: BorderRadius.circular(12),
                                ),
                                child: const Text(
                                  '已激活',
                                  style: TextStyle(
                                    color: Colors.white,
                                    fontSize: 12,
                                    fontWeight: FontWeight.bold,
                                  ),
                                ),
                              ),
                            ],
                          ],
                        ),
                        const SizedBox(height: 4),
                        Text(
                          isVip ? '感谢您的支持！' : '解锁所有高级功能',
                          style: Theme.of(context).textTheme.bodyMedium?.copyWith(
                            color: Colors.grey[600],
                          ),
                        ),
                      ],
                    ),
                  ),
                ],
              ),
              const SizedBox(height: 20),
              const Divider(),
              const SizedBox(height: 12),
              _buildFeatureItem(
                Icons.inventory_2,
                '获取游戏中所有物品 x3',
                isVip,
                onTap: isVip ? _addAllItems : null,
              ),
              _buildFeatureItem(
                Icons.trending_up,
                '战斗成功后，掉落物品倍率',
                isVip,
                showMultiplier: isVip,
                multiplierValue: _itemMultiplier,
                onMultiplierChanged: isVip ? (multiplier) async {
                  setState(() {
                    _itemMultiplier = multiplier;
                  });

                  // 保存设置
                  await MultiplierSettings().setItemMultiplier(multiplier);

                  // 立即应用到游戏中
                  JSBridge.setItemMultiple(multiplier.toDouble());
                  LogUtils.d('物品倍率已设置为${multiplier}x');
                } : null,
              ),
              _buildFeatureItem(
                Icons.trending_up,
                '战斗成功后，经验值倍率',
                isVip,
                showMultiplier: isVip,
                multiplierValue: _expMultiplier,
                onMultiplierChanged: isVip ? (multiplier) async {
                  setState(() {
                    _expMultiplier = multiplier;
                  });

                  // 保存设置
                  await MultiplierSettings().setExpMultiplier(multiplier);

                  // 立即应用到游戏中
                  JSBridge.setExpMultiple(multiplier.toDouble());
                  LogUtils.d('经验倍率已设置为${multiplier}x');
                } : null,
              ),
              _buildFeatureItem(
                Icons.monetization_on,
                '战斗成功后，金币倍率',
                isVip,
                showMultiplier: isVip,
                multiplierValue: _goldMultiplier,
                onMultiplierChanged: isVip ? (multiplier) async {
                  setState(() {
                    _goldMultiplier = multiplier;
                  });

                  // 保存设置
                  await MultiplierSettings().setGoldMultiplier(multiplier);

                  // 立即应用到游戏中
                  JSBridge.setGoldMultiple(multiplier.toDouble());
                  LogUtils.d('金币倍率已设置为${multiplier}x');
                } : null,
              ),
              _buildFeatureItem(Icons.map_outlined, '显示游戏小地图', isVip),
              _buildFeatureItem(Icons.block, '移除所有广告', isVip),
            ],
          ),
        ),
      ),
    );
  }

  Widget _buildFeatureItem(
    IconData icon,
    String text,
    bool isActive, {
    VoidCallback? onTap,
    bool showMultiplier = false,
    int? multiplierValue,
    Function(int)? onMultiplierChanged,
  }) {
    return Padding(
      padding: const EdgeInsets.symmetric(vertical: 6),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          // 第一行：图标和文字
          Row(
            children: [
              Icon(
                isActive ? Icons.check_circle : icon,
                color: isActive ? Colors.green : AppConfig.shared.primaryColor,
                size: 20,
              ),
              const SizedBox(width: 12),
              Expanded(
                child: Text(
                  text,
                  style: TextStyle(
                    fontSize: 15,
                    color: isActive ? Colors.green[700] : null,
                    fontWeight: isActive ? FontWeight.w500 : FontWeight.normal,
                  ),
                ),
              ),
              // 如果可点击且已激活，显示箭头图标
              if (onTap != null && isActive)
                IconButton(
                  icon: Icon(
                    Icons.touch_app,
                    color: Colors.green[700],
                    size: 18,
                  ),
                  onPressed: onTap,
                ),
            ],
          ),
          // 第二行：倍率 segment 控件
          if (showMultiplier && isActive && onMultiplierChanged != null && multiplierValue != null)
            Padding(
              padding: const EdgeInsets.only(left: 32, top: 8, bottom: 4),
              child: Container(
                decoration: BoxDecoration(
                  color: Colors.grey[200],
                  borderRadius: BorderRadius.circular(8),
                ),
                child: Row(
                  mainAxisSize: MainAxisSize.min,
                  children: _multiplierValues.asMap().entries.map((entry) {
                    final index = entry.key;
                    final multiplier = entry.value;
                    final name = _multiplierNames[index];
                    final isSelected = multiplierValue == multiplier;

                    return Expanded(
                      child: GestureDetector(
                        onTap: () => onMultiplierChanged(multiplier),
                        child: Container(
                          padding: const EdgeInsets.symmetric(vertical: 8, horizontal: 12),
                          decoration: BoxDecoration(
                            color: isSelected ? AppConfig.shared.primaryColor : Colors.transparent,
                            borderRadius: BorderRadius.circular(6),
                          ),
                          child: Text(
                            name,
                            textAlign: TextAlign.center,
                            style: TextStyle(
                              color: isSelected ? Colors.white : Colors.black87,
                              fontSize: 13,
                              fontWeight: isSelected ? FontWeight.w600 : FontWeight.normal,
                            ),
                          ),
                        ),
                      ),
                    );
                  }).toList(),
                ),
              ),
            ),
        ],
      ),
    );
  }

  Widget _buildPurchaseSection() {
    if (_iapManager.isVip) {
      return const SizedBox.shrink();
    }

    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 16),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.stretch,
        children: [
          Card(
            elevation: 2,
            child: Padding(
              padding: const EdgeInsets.all(16),
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text(
                    '购买流程',
                    style: Theme.of(context).textTheme.titleMedium?.copyWith(
                      fontWeight: FontWeight.bold,
                    ),
                  ),
                  const SizedBox(height: 12),
                  _buildStepItem('1', '选择联系方式：QQ或微信', Icons.person_add),
                  _buildStepItem('2', 'QQ号: 820224533 或 微信号: 17759190102', Icons.chat),
                  _buildStepItem('3', '联系客服购买VIP会员', Icons.payment),
                  _buildStepItem('4', '获得VIP内购码', Icons.receipt),
                  _buildStepItem('5', '返回此页面兑换内购码', Icons.redeem),
                ],
              ),
            ),
          ),
          const SizedBox(height: 16),
          // QQ联系按钮
          ElevatedButton.icon(
            onPressed: _isProcessing ? null : _openQQ,
            icon: const Icon(Icons.chat, size: 24),
            label: const Text(
              '添加QQ好友购买 (820224533)',
              style: TextStyle(fontSize: 15, fontWeight: FontWeight.bold),
            ),
            style: ElevatedButton.styleFrom(
              backgroundColor: AppConfig.shared.primaryColor,
              foregroundColor: Colors.white,
              padding: const EdgeInsets.symmetric(vertical: 16),
              shape: RoundedRectangleBorder(
                borderRadius: BorderRadius.circular(12),
              ),
            ),
          ),
          const SizedBox(height: 12),
          // 微信联系按钮
          ElevatedButton.icon(
            onPressed: _isProcessing ? null : _openWeChat,
            icon: const Icon(Icons.chat_bubble, size: 24),
            label: const Text(
              '添加微信好友购买 (17759190102)',
              style: TextStyle(fontSize: 15, fontWeight: FontWeight.bold),
            ),
            style: ElevatedButton.styleFrom(
              backgroundColor: Colors.green,
              foregroundColor: Colors.white,
              padding: const EdgeInsets.symmetric(vertical: 16),
              shape: RoundedRectangleBorder(
                borderRadius: BorderRadius.circular(12),
              ),
            ),
          ),
          const SizedBox(height: 12),
          OutlinedButton.icon(
            onPressed: _isProcessing ? null : () {
              setState(() {
                _showCodeInput = !_showCodeInput;
              });
            },
            icon: Icon(_showCodeInput ? Icons.close : Icons.redeem),
            label: Text(_showCodeInput ? '取消兑换' : '我已有内购码'),
            style: OutlinedButton.styleFrom(
              foregroundColor: AppConfig.shared.primaryColor,
              padding: const EdgeInsets.symmetric(vertical: 16),
              shape: RoundedRectangleBorder(
                borderRadius: BorderRadius.circular(12),
              ),
            ),
          ),
        ],
      ),
    );
  }

  Widget _buildStepItem(String step, String text, IconData icon) {
    return Padding(
      padding: const EdgeInsets.symmetric(vertical: 4),
      child: Row(
        children: [
          Container(
            width: 28,
            height: 28,
            decoration: BoxDecoration(
              color: AppConfig.shared.primaryColor.withValues(alpha: 0.1),
              shape: BoxShape.circle,
            ),
            child: Center(
              child: Text(
                step,
                style: TextStyle(
                  color: AppConfig.shared.primaryColor,
                  fontWeight: FontWeight.bold,
                  fontSize: 14,
                ),
              ),
            ),
          ),
          const SizedBox(width: 12),
          Icon(icon, size: 18, color: Colors.grey[600]),
          const SizedBox(width: 8),
          Expanded(
            child: Text(
              text,
              style: TextStyle(
                fontSize: 14,
                color: Colors.grey[700],
              ),
            ),
          ),
        ],
      ),
    );
  }

  /// 构建用户ID信息卡片（调试用）
  Widget _buildUserIdCard() {
    // Release 模式下不显示设备信息
    if (kReleaseMode) {
      return const SizedBox.shrink();
    }

    if (_userId.isEmpty) {
      return const SizedBox.shrink();
    }

    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 16),
      child: Card(
        elevation: 1,
        color: Colors.grey[100],
        child: Padding(
          padding: const EdgeInsets.all(12),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Row(
                children: [
                  Icon(Icons.fingerprint, size: 16, color: Colors.grey[600]),
                  const SizedBox(width: 6),
                  Text(
                    '设备信息',
                    style: TextStyle(
                      fontSize: 12,
                      fontWeight: FontWeight.bold,
                      color: Colors.grey[700],
                    ),
                  ),
                ],
              ),
              const SizedBox(height: 8),
              _buildInfoRow('用户ID', _userId),
              const SizedBox(height: 4),
              _buildInfoRow('设备ID', '${_deviceId.substring(0, _deviceId.length > 16 ? 16 : _deviceId.length)}...'),
            ],
          ),
        ),
      ),
    );
  }

  Widget _buildInfoRow(String label, String value) {
    return Row(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        SizedBox(
          width: 60,
          child: Text(
            '$label:',
            style: TextStyle(
              fontSize: 11,
              color: Colors.grey[600],
            ),
          ),
        ),
        Expanded(
          child: GestureDetector(
            onLongPress: () {
              Clipboard.setData(ClipboardData(text: value));
              ScaffoldMessenger.of(context).showSnackBar(
                const SnackBar(
                  content: Text('已复制到剪贴板'),
                  duration: Duration(seconds: 1),
                ),
              );
            },
            child: Text(
              value,
              style: TextStyle(
                fontSize: 11,
                color: Colors.grey[800],
                fontFamily: 'monospace',
              ),
            ),
          ),
        ),
      ],
    );
  }

  Widget _buildCodeInputSection() {
    // 如果已购买VIP，隐藏兑换码输入
    if (_iapManager.isVip || !_showCodeInput) {
      return const SizedBox.shrink();
    }

    return Padding(
      padding: const EdgeInsets.all(16),
      child: Card(
        elevation: 3,
        color: Colors.blue[50],
        child: Padding(
          padding: const EdgeInsets.all(16),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.stretch,
            children: [
              Row(
                children: [
                  Icon(Icons.info_outline, color: Colors.blue[700]),
                  const SizedBox(width: 8),
                  Text(
                    '兑换VIP内购码',
                    style: TextStyle(
                      fontSize: 16,
                      fontWeight: FontWeight.bold,
                      color: Colors.blue[700],
                    ),
                  ),
                ],
              ),
              const SizedBox(height: 16),
              TextField(
                controller: _codeController,
                enabled: !_isProcessing,
                decoration: InputDecoration(
                  labelText: '请输入内购码',
                  hintText: '例如：ABCD-EFGH-IJKL-MNOP',
                  prefixIcon: const Icon(Icons.vpn_key),
                  border: OutlineInputBorder(
                    borderRadius: BorderRadius.circular(12),
                  ),
                  filled: true,
                  fillColor: Colors.white,
                ),
                textCapitalization: TextCapitalization.characters,
                maxLength: 19, // XXXX-XXXX-XXXX-XXXX
              ),
              const SizedBox(height: 12),
              ElevatedButton(
                onPressed: _isProcessing ? null : _redeemCode,
                style: ElevatedButton.styleFrom(
                  backgroundColor: AppConfig.shared.primaryColor,
                  foregroundColor: Colors.white,
                  padding: const EdgeInsets.symmetric(vertical: 16),
                  minimumSize: const Size(double.infinity, 50),
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(12),
                  ),
                ),
                child: _isProcessing
                  ? const SizedBox(
                      width: 20,
                      height: 20,
                      child: CircularProgressIndicator(
                        strokeWidth: 2,
                        valueColor: AlwaysStoppedAnimation<Color>(Colors.white),
                      ),
                    )
                  : const Text(
                      '立即兑换',
                      style: TextStyle(
                        fontSize: 16,
                        fontWeight: FontWeight.bold,
                      ),
                    ),
              ),
              const SizedBox(height: 8),
              Text(
                '提示：请确保内购码来自官方渠道',
                style: TextStyle(
                  fontSize: 12,
                  color: Colors.grey[600],
                ),
                textAlign: TextAlign.center,
              ),
            ],
          ),
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    final config = AppConfig.shared;

    return Scaffold(
      appBar: AppBar(
        title: const Text('VIP会员'),
        backgroundColor: config.appBarBackgroundColor,
        foregroundColor: config.appBarForegroundColor,
        elevation: 1,
      ),
      body: ListView(
        children: [
          const SizedBox(height: 8),

          // VIP会员卡片
          _buildVIPCard(),

          const SizedBox(height: 8),

          // 用户ID信息卡片
          _buildUserIdCard(),

          const SizedBox(height: 8),

          // 内购码输入区域
          _buildCodeInputSection(),

          // 购买流程说明
          _buildPurchaseSection(),

          const SizedBox(height: 24),

          // 说明文字
          if (!_iapManager.isVip)
            Padding(
              padding: const EdgeInsets.all(16),
              child: Card(
                color: Colors.amber[50],
                child: Padding(
                  padding: const EdgeInsets.all(16),
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Row(
                        children: [
                          Icon(Icons.lightbulb_outline, color: Colors.amber[700]),
                          const SizedBox(width: 8),
                          Text(
                            '温馨提示',
                            style: TextStyle(
                              fontWeight: FontWeight.bold,
                              color: Colors.amber[700],
                            ),
                          ),
                        ],
                      ),
                      const SizedBox(height: 12),
                      const Text(
                        '• VIP会员为永久有效，一次费用38元，购买终身享用\n'
                        '• 购买后可在单个设备上使用，所有游戏生效\n'
                        '• 如遇问题请联系客服: QQ 820224533 或微信 17759190102\n'
                        '• 请勿相信任何非官方渠道的购买信息',
                        style: TextStyle(
                          color: Colors.black87,
                          height: 1.6,
                        ),
                      ),
                    ],
                  ),
                ),
              ),
            ),

          const SizedBox(height: 32),
        ],
      ),
    );
  }
}
