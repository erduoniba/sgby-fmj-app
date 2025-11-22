import 'package:flutter/material.dart';
import '../models/save_data.dart';
import '../utils/save_manager.dart';
import 'save_detail_page.dart';
import '../utils/app_config.dart';

class SaveListPage extends StatefulWidget {
  const SaveListPage({super.key});

  @override
  State<SaveListPage> createState() => _SaveListPageState();
}

class _SaveListPageState extends State<SaveListPage> {
  List<GameSection> _gameSections = [];
  bool _isLoading = true;

  @override
  void initState() {
    super.initState();
    _loadSaveData();
  }

  Future<void> _loadSaveData() async {
    setState(() {
      _isLoading = true;
    });

    try {
      final allSaveData = await SaveManager.shared.loadAllSaveData();
      final gameSections = SaveManager.shared.groupSaveDataByGame(allSaveData);
      
      setState(() {
        _gameSections = gameSections;
        _isLoading = false;
      });
    } catch (e) {
      setState(() {
        _isLoading = false;
      });
      
      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          SnackBar(
            content: Text('加载存档数据失败: $e'),
            backgroundColor: Colors.red,
          ),
        );
      }
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('存档列表'),
        backgroundColor: AppConfig.shared.appBarBackgroundColor,
        foregroundColor: AppConfig.shared.appBarForegroundColor,
      ),
      body: _isLoading
          ? const Center(child: CircularProgressIndicator())
          : _gameSections.isEmpty
              ? _buildEmptyState()
              : _buildSaveList(),
    );
  }

  Widget _buildEmptyState() {
    return const Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          Icon(
            Icons.save_outlined,
            size: 64,
            color: Colors.grey,
          ),
          SizedBox(height: 16),
          Text(
            '暂无存档数据',
            style: TextStyle(
              fontSize: 18,
              color: Colors.grey,
            ),
          ),
          SizedBox(height: 8),
          Text(
            '开始游戏并保存进度后，存档将显示在这里',
            style: TextStyle(
              fontSize: 14,
              color: Colors.grey,
            ),
            textAlign: TextAlign.center,
          ),
        ],
      ),
    );
  }

  Widget _buildSaveList() {
    return ListView.builder(
      itemCount: _gameSections.length,
      itemBuilder: (context, sectionIndex) {
        final section = _gameSections[sectionIndex];
        return _buildGameSection(section);
      },
    );
  }

  Widget _buildGameSection(GameSection section) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        // Section header
        Container(
          width: double.infinity,
          padding: const EdgeInsets.symmetric(horizontal: 16, vertical: 12),
          color: Colors.grey[100],
          child: Text(
            section.gameName,
            style: const TextStyle(
              fontSize: 16,
              fontWeight: FontWeight.w600,
              color: Colors.black87,
            ),
          ),
        ),
        // Save data list
        ...section.saveDataList.map((saveData) => _buildSaveDataTile(saveData)),
      ],
    );
  }

  Widget _buildSaveDataTile(SaveData saveData) {
    return Card(
      margin: const EdgeInsets.symmetric(horizontal: 12, vertical: 4),
      child: ListTile(
        title: Text(
          saveData.displayTitle,
          style: const TextStyle(
            fontSize: 16,
            fontWeight: FontWeight.w500,
          ),
        ),
        subtitle: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text(
              saveData.displayContent,
              style: const TextStyle(
                fontSize: 14,
                color: Colors.grey,
              ),
              maxLines: 2,
              overflow: TextOverflow.ellipsis,
            ),
            const SizedBox(height: 4),
            Text(
              'Key: ${saveData.key}',
              style: const TextStyle(
                fontSize: 12,
                color: Colors.grey,
              ),
            ),
          ],
        ),
        trailing: const Icon(Icons.arrow_forward_ios, size: 16),
        onTap: () => _openSaveDetail(saveData),
      ),
    );
  }

  void _openSaveDetail(SaveData saveData) {
    Navigator.push(
      context,
      MaterialPageRoute(
        builder: (context) => SaveDetailPage(saveData: saveData),
      ),
    ).then((_) {
      // Refresh data when returning from detail page
      _loadSaveData();
    });
  }
}