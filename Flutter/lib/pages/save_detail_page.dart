import 'package:flutter/material.dart';
import '../models/save_data.dart';
import '../utils/save_manager.dart';
import '../utils/js_bridge.dart';
import '../utils/app_config.dart';

class SaveDetailPage extends StatefulWidget {
  final SaveData saveData;

  const SaveDetailPage({
    super.key,
    required this.saveData,
  });

  @override
  State<SaveDetailPage> createState() => _SaveDetailPageState();
}

class _SaveDetailPageState extends State<SaveDetailPage> {
  late SaveData _saveData;
  late TextEditingController _textController;
  bool _isEditing = false;
  bool _hasChanges = false;
  bool _isSaving = false;

  @override
  void initState() {
    super.initState();
    _saveData = widget.saveData;
    _textController = TextEditingController(text: _saveData.content);
    _textController.addListener(_onTextChanged);
  }

  @override
  void dispose() {
    _textController.removeListener(_onTextChanged);
    _textController.dispose();
    super.dispose();
  }

  void _onTextChanged() {
    setState(() {
      _hasChanges = _textController.text != _saveData.content;
    });
  }

  @override
  Widget build(BuildContext context) {
    return PopScope(
      onPopInvokedWithResult: (bool didPop, dynamic result) async {
        if (!didPop && _hasChanges && _isEditing) {
          final shouldPop = await _showUnsavedChangesDialog();
          if (shouldPop && context.mounted) {
            Navigator.of(context).pop();
          }
        }
      },
      child: Scaffold(
        appBar: AppBar(
          title: const Text('存档详情'),
          backgroundColor: AppConfig.shared.appBarBackgroundColor,
          foregroundColor: AppConfig.shared.appBarForegroundColor,
          actions: [
            if (_isEditing)
              TextButton(
                onPressed: _isSaving ? null : _saveChanges,
                child: _isSaving
                    ? const SizedBox(
                        width: 20,
                        height: 20,
                        child: CircularProgressIndicator(strokeWidth: 2),
                      )
                    : const Text('保存'),
              )
            else
              TextButton(
                onPressed: _startEditing,
                child: const Text('编辑'),
              ),
          ],
        ),
        body: SingleChildScrollView(
          padding: const EdgeInsets.all(16),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              _buildInfoSection(),
              const SizedBox(height: 24),
              _buildContentSection(),
            ],
          ),
        ),
      ),
    );
  }

  Widget _buildInfoSection() {
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            _buildInfoRow('标题', _saveData.displayTitle),
            const SizedBox(height: 12),
            _buildInfoRow('游戏', _saveData.gameName),
            const SizedBox(height: 12),
            _buildInfoRow('存档键', _saveData.key),
            const SizedBox(height: 12),
            _buildInfoRow('创建时间', _saveData.formattedDate),
            const SizedBox(height: 12),
            _buildInfoRow('内容长度', '${_saveData.content.length} 字符'),
          ],
        ),
      ),
    );
  }

  Widget _buildInfoRow(String label, String value) {
    return Row(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        SizedBox(
          width: 80,
          child: Text(
            label,
            style: const TextStyle(
              fontWeight: FontWeight.w500,
              color: Colors.grey,
            ),
          ),
        ),
        Expanded(
          child: Text(
            value,
            style: const TextStyle(fontSize: 14),
          ),
        ),
      ],
    );
  }

  Widget _buildContentSection() {
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Row(
              children: [
                const Text(
                  '存档内容',
                  style: TextStyle(
                    fontSize: 16,
                    fontWeight: FontWeight.w600,
                  ),
                ),
                const Spacer(),
                if (_hasChanges && _isEditing)
                  Container(
                    padding: const EdgeInsets.symmetric(horizontal: 8, vertical: 4),
                    decoration: BoxDecoration(
                      color: Colors.orange[100],
                      borderRadius: BorderRadius.circular(12),
                    ),
                    child: const Text(
                      '已修改',
                      style: TextStyle(
                        fontSize: 12,
                        color: Colors.orange,
                      ),
                    ),
                  ),
              ],
            ),
            const SizedBox(height: 12),
            Container(
              width: double.infinity,
              height: 400,
              decoration: BoxDecoration(
                border: Border.all(
                  color: _isEditing ? Colors.blue : Colors.grey[300]!,
                  width: _isEditing ? 2 : 1,
                ),
                borderRadius: BorderRadius.circular(8),
              ),
              child: TextField(
                controller: _textController,
                enabled: _isEditing,
                maxLines: null,
                expands: true,
                style: const TextStyle(
                  fontFamily: 'Courier',
                  fontSize: 14,
                ),
                decoration: const InputDecoration(
                  contentPadding: EdgeInsets.all(12),
                  border: InputBorder.none,
                  hintText: '存档内容将显示在这里...',
                ),
                textAlignVertical: TextAlignVertical.top,
              ),
            ),
          ],
        ),
      ),
    );
  }

  void _startEditing() {
    setState(() {
      _isEditing = true;
    });
  }

  Future<void> _saveChanges() async {
    if (!_hasChanges) {
      setState(() {
        _isEditing = false;
      });
      return;
    }

    setState(() {
      _isSaving = true;
    });

    try {
      final newContent = _textController.text;
      final success = await SaveManager.shared.saveSaveData(_saveData.key, newContent);
      
      if (success) {
        // Update local data
        setState(() {
          _saveData = _saveData.copyWith(
            content: newContent,
            createdDate: DateTime.now(),
          );
          _hasChanges = false;
          _isEditing = false;
          _isSaving = false;
        });
        
        // Sync data to H5 page via JSBridge
        await _syncToWebView();
        
        if (mounted) {
          ScaffoldMessenger.of(context).showSnackBar(
            const SnackBar(
              content: Text('保存成功'),
              backgroundColor: Colors.green,
            ),
          );
        }
      } else {
        setState(() {
          _isSaving = false;
        });
        
        if (mounted) {
          ScaffoldMessenger.of(context).showSnackBar(
            const SnackBar(
              content: Text('保存失败'),
              backgroundColor: Colors.red,
            ),
          );
        }
      }
    } catch (e) {
      setState(() {
        _isSaving = false;
      });
      
      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(
            content: Text('保存失败: $e'),
            backgroundColor: Colors.red,
          ),
        );
      }
    }
  }

  Future<void> _syncToWebView() async {
    // Sync save data to H5 localStorage
    // This is similar to iOS's save data change notification handling
    try {
      // For FMJ saves, inject to localStorage
      if (_saveData.key.contains('fmjsave')) {
        await JSBridge.injectFmjCacheJSHooks();
      }
      // For Baye saves, inject cache
      else if (_saveData.key.contains('baye//data//') || _saveData.key.contains('sango')) {
        await JSBridge.injectBayeCacheJSHooks();
      }
    } catch (e) {
      // Log error but don't show to user since save was successful
      debugPrint('Failed to sync to WebView: $e');
    }
  }


  Future<bool> _showUnsavedChangesDialog() async {
    return await showDialog<bool>(
      context: context,
      builder: (context) => AlertDialog(
        title: const Text('未保存的更改'),
        content: const Text('您有未保存的更改，是否要保存？'),
        actions: [
          TextButton(
            onPressed: () => Navigator.of(context).pop(false),
            child: const Text('取消'),
          ),
          TextButton(
            onPressed: () {
              Navigator.of(context).pop(true);
              // Don't save, just discard changes
            },
            style: TextButton.styleFrom(foregroundColor: Colors.red),
            child: const Text('不保存'),
          ),
          TextButton(
            onPressed: () async {
              Navigator.of(context).pop(true);
              await _saveChanges();
            },
            child: const Text('保存'),
          ),
        ],
      ),
    ) ?? false;
  }
}