import 'package:flutter/material.dart';
import '../services/feedback_service.dart';
import '../models/save_data.dart';
import '../utils/save_manager.dart';
import '../utils/app_config.dart';
import '../utils/log_utils.dart';

class FeedbackPage extends StatefulWidget {
  const FeedbackPage({super.key});

  @override
  State<FeedbackPage> createState() => _FeedbackPageState();
}

class _FeedbackPageState extends State<FeedbackPage> {
  final _formKey = GlobalKey<FormState>();
  final _contentController = TextEditingController();
  final _contactController = TextEditingController();

  bool _isSubmitting = false;
  bool _includeSaveData = false;
  SaveData? _selectedSaveData;
  List<SaveData> _availableSaves = [];

  @override
  void initState() {
    super.initState();
    _loadAvailableSaves();
  }

  @override
  void dispose() {
    _contentController.dispose();
    _contactController.dispose();
    super.dispose();
  }

  Future<void> _loadAvailableSaves() async {
    try {
      final saves = await SaveManager.shared.loadAllSaveData();
      setState(() {
        _availableSaves = saves;
      });
    } catch (e) {
      LogUtils.d('加载存档列表失败: $e');
    }
  }

  Future<void> _submitFeedback() async {
    if (!_formKey.currentState!.validate()) {
      return;
    }

    setState(() {
      _isSubmitting = true;
    });

    try {
      final content = _contentController.text.trim();
      final contact = _contactController.text.trim();
      final saveData = _includeSaveData ? _selectedSaveData : null;

      final (success, message) = await FeedbackService.instance.submitFeedback(
        content: content,
        contact: contact.isNotEmpty ? contact : null,
        saveData: saveData,
      );

      if (mounted) {
        if (success) {
          _showSuccessDialog(message ?? '反馈提交成功！');
        } else {
          _showErrorDialog(message ?? '提交失败，请稍后重试');
        }
      }
    } catch (e) {
      LogUtils.d('提交反馈异常: $e');
      if (mounted) {
        _showErrorDialog('提交失败: ${e.toString()}');
      }
    } finally {
      if (mounted) {
        setState(() {
          _isSubmitting = false;
        });
      }
    }
  }

  void _showSuccessDialog(String message) {
    showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: const Text('提交成功'),
        content: Text(message),
        actions: [
          TextButton(
            onPressed: () {
              Navigator.of(context).pop(); // 关闭对话框
              Navigator.of(context).pop(); // 返回上一页
            },
            child: const Text('确定'),
          ),
        ],
      ),
    );
  }

  void _showErrorDialog(String message) {
    showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: const Text('提交失败'),
        content: Text(message),
        actions: [
          TextButton(
            onPressed: () => Navigator.of(context).pop(),
            child: const Text('确定'),
          ),
        ],
      ),
    );
  }

  Widget _buildSaveDataSelector() {
    if (!_includeSaveData) {
      return const SizedBox.shrink();
    }

    return Card(
      margin: const EdgeInsets.symmetric(horizontal: 16, vertical: 8),
      child: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            const Text(
              '选择存档',
              style: TextStyle(fontSize: 16, fontWeight: FontWeight.bold),
            ),
            const SizedBox(height: 12),
            if (_availableSaves.isEmpty)
              const Text(
                '没有可用的存档',
                style: TextStyle(color: Colors.grey),
              )
            else
              DropdownButtonFormField<SaveData>(
                value: _selectedSaveData,
                decoration: const InputDecoration(
                  labelText: '选择要上传的存档',
                  border: OutlineInputBorder(),
                ),
                items: _availableSaves.map((save) {
                  return DropdownMenuItem<SaveData>(
                    value: save,
                    child: Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      mainAxisSize: MainAxisSize.min,
                      children: [
                        Text(
                          save.displayTitle,
                          style: const TextStyle(fontWeight: FontWeight.w500),
                        ),
                        Text(
                          '${save.gameName} | ${save.formattedDate}',
                          style: const TextStyle(fontSize: 12, color: Colors.grey),
                        ),
                      ],
                    ),
                  );
                }).toList(),
                onChanged: (value) {
                  setState(() {
                    _selectedSaveData = value;
                  });
                },
                validator: (value) {
                  if (_includeSaveData && value == null) {
                    return '请选择要上传的存档';
                  }
                  return null;
                },
              ),
            const SizedBox(height: 8),
            const Text(
              '注意：存档数据将帮助开发者更好地定位和解决问题',
              style: TextStyle(fontSize: 12, color: Colors.orange),
            ),
          ],
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('意见反馈'),
        backgroundColor: AppConfig.shared.appBarBackgroundColor,
        foregroundColor: AppConfig.shared.appBarForegroundColor,
      ),
      body: Form(
        key: _formKey,
        child: ListView(
          children: [
            const SizedBox(height: 16),

            // 反馈内容输入
            Padding(
              padding: const EdgeInsets.symmetric(horizontal: 16),
              child: TextFormField(
                controller: _contentController,
                maxLines: 8,
                decoration: const InputDecoration(
                  labelText: '反馈内容 *',
                  hintText: '请详细描述您遇到的问题或建议...',
                  border: OutlineInputBorder(),
                  alignLabelWithHint: true,
                ),
                validator: (value) {
                  if (value == null || value.trim().isEmpty) {
                    return '请输入反馈内容';
                  }
                  return null;
                },
              ),
            ),

            const SizedBox(height: 16),

            // 联系方式输入
            Padding(
              padding: const EdgeInsets.symmetric(horizontal: 16),
              child: TextFormField(
                controller: _contactController,
                decoration: const InputDecoration(
                  labelText: '联系方式（可选）',
                  hintText: '邮箱、QQ、微信等，便于我们联系您',
                  border: OutlineInputBorder(),
                ),
              ),
            ),

            const SizedBox(height: 16),

            // 包含存档选项
            Card(
              margin: const EdgeInsets.symmetric(horizontal: 16),
              child: SwitchListTile(
                title: const Text('包含存档数据'),
                subtitle: const Text('上传存档有助于开发者定位问题'),
                value: _includeSaveData,
                onChanged: (value) {
                  setState(() {
                    _includeSaveData = value;
                    if (!value) {
                      _selectedSaveData = null;
                    }
                  });
                },
              ),
            ),

            // 存档选择器
            _buildSaveDataSelector(),

            const SizedBox(height: 24),

            // 提交按钮
            Padding(
              padding: const EdgeInsets.symmetric(horizontal: 16),
              child: ElevatedButton(
                onPressed: _isSubmitting ? null : _submitFeedback,
                style: ElevatedButton.styleFrom(
                  backgroundColor: AppConfig.shared.primaryColor,
                  foregroundColor: Colors.white,
                  padding: const EdgeInsets.symmetric(vertical: 16),
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(8),
                  ),
                ),
                child: _isSubmitting
                    ? const Row(
                        mainAxisAlignment: MainAxisAlignment.center,
                        children: [
                          SizedBox(
                            width: 20,
                            height: 20,
                            child: CircularProgressIndicator(
                              strokeWidth: 2,
                              valueColor: AlwaysStoppedAnimation<Color>(Colors.white),
                            ),
                          ),
                          SizedBox(width: 12),
                          Text('提交中...'),
                        ],
                      )
                    : const Text(
                        '提交反馈',
                        style: TextStyle(fontSize: 16, fontWeight: FontWeight.bold),
                      ),
              ),
            ),

            const SizedBox(height: 16),

            // 说明文字
            const Padding(
              padding: EdgeInsets.symmetric(horizontal: 16),
              child: Text(
                '您的反馈对我们非常重要！我们会认真对待每一条反馈，并尽快回复。',
                style: TextStyle(fontSize: 12, color: Colors.grey),
                textAlign: TextAlign.center,
              ),
            ),

            const SizedBox(height: 32),
          ],
        ),
      ),
    );
  }
}