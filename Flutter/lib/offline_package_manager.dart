import 'dart:io';
import 'dart:convert';
import 'package:flutter/services.dart';
import 'package:path_provider/path_provider.dart';
import 'package:archive/archive.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:crypto/crypto.dart';
import 'package:http/http.dart' as http;
import 'utils/log_utils.dart';
import 'utils/app_config.dart';
import 'utils/platform_utils.dart';

class OfflinePackageManager {
  static const String _versionKey = 'offlinePackageVersion';
  static const String _serverUrlKey = 'offlinePackageServerUrl';
  static const String _lastCheckTimeKey = 'lastOfflinePackageCheckTime';
  
  // 服务器配置
  static const String _defaultServerUrl = 'http://localhost:3000'; // 默认服务器地址
  static const Duration _checkInterval = Duration(hours: 6); // 检查间隔
  
  // 伏魔记核心文件防篡改
  static const List<String> _fmjCoreFiles = [
    'index.html',
    'js/fmj.core.v2.js', 
    'js/m-native-bridge.js'
  ];
  
  static const String _fmjHashesKey = 'fmj_core_files_hashes';

  /// 获取离线包目录路径
  static Future<String> get offlineDirPath async {
    if (PlatformUtils.isWeb) {
      // Web平台使用固定路径
      final offlineDirName = AppConfig.shared.offlineDirName;
      return '/assets/web/$offlineDirName';
    } else {
      // 使用Documents目录
      final documentPath = await getApplicationDocumentsDirectory();
      final offlineDirName = AppConfig.shared.offlineDirName;
      return '${documentPath.path}/$offlineDirName';
    }
  }

  /// 计算文件的SHA256哈希值
  static Future<String> _calculateFileHash(String filePath) async {
    try {
      final file = File(filePath);
      if (!await file.exists()) {
        LogUtils.d('文件不存在: $filePath');
        return '';
      }
      
      final bytes = await file.readAsBytes();
      final digest = sha256.convert(bytes);
      return digest.toString();
    } catch (e) {
      LogUtils.d('计算文件哈希失败: $filePath, 错误: $e');
      return '';
    }
  }

  /// 保存伏魔记核心文件的哈希值
  static Future<void> _saveFmjCoreFilesHashes(String dirPath) async {
    if (AppConfig.shared.appName != AppName.hdFmjApp) {
      return; // 只对伏魔记进行校验
    }
    
    try {
      final prefs = await SharedPreferences.getInstance();
      final hashes = <String, String>{};
      
      for (final fileName in _fmjCoreFiles) {
        final filePath = '$dirPath/$fileName';
        final hash = await _calculateFileHash(filePath);
        if (hash.isNotEmpty) {
          hashes[fileName] = hash;
          LogUtils.d('保存核心文件哈希: $fileName -> ${hash.substring(0, 16)}...');
        } else {
          LogUtils.d('警告：核心文件哈希计算失败: $fileName');
        }
      }
      
      if (hashes.length == _fmjCoreFiles.length) {
        await prefs.setString(_fmjHashesKey, jsonEncode(hashes));
        LogUtils.d('✅ 伏魔记核心文件哈希值已保存 (${hashes.length}/${_fmjCoreFiles.length})');
      } else {
        LogUtils.d('❌ 部分核心文件哈希计算失败，未保存校验数据');
      }
    } catch (e) {
      LogUtils.d('保存伏魔记核心文件哈希失败: $e');
    }
  }

  /// 验证伏魔记核心文件完整性
  static Future<bool> verifyFmjCoreFiles() async {
    if (AppConfig.shared.appName != AppName.hdFmjApp) {
      return true; // 非伏魔记应用直接通过
    }
    
    try {
      final prefs = await SharedPreferences.getInstance();
      final savedHashesStr = prefs.getString(_fmjHashesKey);
      
      if (savedHashesStr == null || savedHashesStr.isEmpty) {
        LogUtils.d('❌ 未找到保存的核心文件哈希值，可能是首次解压');
        return false;
      }
      
      final savedHashes = Map<String, String>.from(jsonDecode(savedHashesStr));
      final dirPath = await offlineDirPath;
      
      LogUtils.d('开始验证伏魔记核心文件完整性...');
      
      for (final fileName in _fmjCoreFiles) {
        final filePath = '$dirPath/$fileName';
        final currentHash = await _calculateFileHash(filePath);
        final savedHash = savedHashes[fileName];
        
        if (savedHash == null) {
          LogUtils.d('❌ 核心文件缺少保存的哈希值: $fileName');
          return false;
        }
        
        if (currentHash.isEmpty) {
          LogUtils.d('❌ 核心文件读取失败或不存在: $fileName');
          return false;
        }
        
        if (currentHash != savedHash) {
          LogUtils.d('❌ 核心文件被篡改: $fileName');
          LogUtils.d('  预期哈希: ${savedHash.substring(0, 16)}...');
          LogUtils.d('  实际哈希: ${currentHash.substring(0, 16)}...');
          return false;
        }
        
        LogUtils.d('✅ 核心文件验证通过: $fileName');
      }
      
      LogUtils.d('✅ 所有伏魔记核心文件验证通过');
      return true;
      
    } catch (e) {
      LogUtils.d('❌ 验证伏魔记核心文件失败: $e');
      return false;
    }
  }

  /// 清除保存的核心文件哈希值（用于重新初始化）
  static Future<void> clearFmjCoreFilesHashes() async {
    try {
      final prefs = await SharedPreferences.getInstance();
      await prefs.remove(_fmjHashesKey);
      LogUtils.d('已清除保存的伏魔记核心文件哈希值');
    } catch (e) {
      LogUtils.d('清除核心文件哈希值失败: $e');
    }
  }

  /// 检测并设置正确的游戏类型
  /// 通过检查assets目录中的离线包文件来判断当前是哪个游戏
  static Future<void> _detectAndSetGameType() async {
    try {
      // 检查assets目录中有哪些离线包文件
      final manifestContent = await rootBundle.loadString('AssetManifest.json');
      final Map<String, dynamic> manifestMap = json.decode(manifestContent);

      // 查找离线包文件
      bool hasBayePackage = false;
      bool hasFmjPackage = false;

      for (final key in manifestMap.keys) {
        if (key.contains('assets/web/baye-offline') && key.endsWith('.zip')) {
          hasBayePackage = true;
          LogUtils.d('检测到三国霸业离线包: $key');
        } else if (key.contains('assets/web/fmj-offline') && key.endsWith('.zip')) {
          hasFmjPackage = true;
          LogUtils.d('检测到伏魔记离线包: $key');
        }
      }

      // 根据检测结果设置游戏类型
      // 如果只有一个游戏包，自动切换到该游戏
      if (hasBayePackage && !hasFmjPackage) {
        LogUtils.d('仅检测到三国霸业，自动切换到三国霸业');
        await AppConfig.shared.switchToGame(AppName.hdBayeApp);
      } else if (hasFmjPackage && !hasBayePackage) {
        LogUtils.d('仅检测到伏魔记，保持伏魔记设置');
        await AppConfig.shared.switchToGame(AppName.hdFmjApp);
      } else if (hasBayePackage && hasFmjPackage) {
        // 两个都有，保持用户之前的选择或使用默认值
        LogUtils.d('同时检测到两个游戏包，使用当前配置: ${AppConfig.shared.appName}');
      } else {
        // 都没有，使用默认的伏魔记
        LogUtils.d('未检测到任何游戏包，使用默认伏魔记');
      }
    } catch (e) {
      LogUtils.d('检测游戏类型失败: $e, 使用默认配置');
    }
  }

  /// 检查并解压离线包
  static Future<String> unzipOfflinePackageIfNeeded() async {
    if (PlatformUtils.isWeb) {
      // Web平台直接返回assets路径
      final dirPath = await offlineDirPath;
      LogUtils.d('Web平台使用assets路径: $dirPath');
      return dirPath;
    }

    // 首先检测并设置正确的游戏类型
    await _detectAndSetGameType();

    final prefs = await SharedPreferences.getInstance();
    final offlineZipName = AppConfig.shared.offlineZipName;
    final appName = AppConfig.shared.appName;
    final versionKey = '${_versionKey}_${appName.toString()}';
    final currentVersion = prefs.getString(versionKey);
    final dirPath = await offlineDirPath;
    final dir = Directory(dirPath);
    LogUtils.d('检查离线包 - 应用: $appName, 当前版本: $currentVersion, 目标版本: $offlineZipName');

    // 如果存在旧版本的离线包且版本不一致，则删除旧包
    if (await dir.exists()) {
      if (currentVersion != offlineZipName) {
        LogUtils.d('发现旧版本离线包，开始清理...');
        await dir.delete(recursive: true);
        LogUtils.d('旧版本离线包清理完成');
      } else {
        LogUtils.d('使用已缓存的离线包: $dirPath');
        return dirPath;
      }
    }

    // 解压离线包
    try {
      LogUtils.d('开始加载新的离线包压缩文件');
      final zipBytes = await rootBundle.load('assets/web/$offlineZipName.zip');
      
      // 解压zip文件（统一使用无密码解压）
      final archive = ZipDecoder().decodeBytes(zipBytes.buffer.asUint8List());
      
      LogUtils.d('离线包压缩文件加载完成，文件数量: ${archive.length}');

      // 创建目标目录
      await dir.create(recursive: true);
      LogUtils.d('创建离线包目录: $dirPath');

      // 解压文件 - 最简化实现
      int processedFiles = 0;
      for (final file in archive) {
        if (file.isFile) {
          try {
            final filePath = '$dirPath/${file.name}';
            LogUtils.d('正在解压文件 ${processedFiles + 1}/${archive.length}: ${file.name}');
            
            final outputFile = File(filePath);
            
            // 确保目录存在
            outputFile.parent.createSync(recursive: true);
            
            // 直接写入文件内容
            outputFile.writeAsBytesSync(file.content as List<int>);
            
            processedFiles++;
            LogUtils.d('文件解压成功: ${file.name} (${file.content.length} bytes)');
          } catch (e) {
            LogUtils.d('单个文件解压失败: ${file.name}, 错误: $e');
            rethrow;
          }
        }
      }
      
      final extractedFiles = archive.where((f) => f.isFile).length;
      LogUtils.d('离线包解压完成，成功解压文件数: $extractedFiles');

      // 对伏魔记保存核心文件哈希值（用于防篡改）
      if (appName == AppName.hdFmjApp) {
        LogUtils.d('保存伏魔记核心文件哈希值...');
        await _saveFmjCoreFilesHashes(dirPath);
      }

      // 保存当前版本号
      await prefs.setString(versionKey, offlineZipName);
      LogUtils.d('离线包版本更新完成: $offlineZipName');
      return dirPath;
    } catch (e) {
      LogUtils.d('解压离线包失败: $e');
      // 如果解压失败，可能是密码错误或文件损坏，清理可能的残留文件
      if (await dir.exists()) {
        try {
          await dir.delete(recursive: true);
          LogUtils.d('清理失败解压的残留文件');
        } catch (cleanupError) {
          LogUtils.d('清理残留文件失败: $cleanupError');
        }
      }
      rethrow;
    }
  }

  /// 清理其他游戏的离线包缓存
  static Future<void> clearOtherGameCache() async {
    final documentsPath = await getApplicationDocumentsDirectory();
    final currentAppName = AppConfig.shared.appName;
    
    // 清理其他游戏的目录
    final otherDirs = <String>[];
    if (currentAppName != AppName.hdBayeApp) {
      otherDirs.add('${documentsPath.path}/baye-offline');
    }
    if (currentAppName != AppName.hdFmjApp) {
      otherDirs.add('${documentsPath.path}/fmj-offline');
    }
    
    for (final dirPath in otherDirs) {
      final dir = Directory(dirPath);
      if (await dir.exists()) {
        try {
          await dir.delete(recursive: true);
          LogUtils.d('清理其他游戏缓存: $dirPath');
        } catch (e) {
          LogUtils.d('清理缓存失败: $dirPath, 错误: $e');
        }
      }
    }
  }
  
  /// 清理当前游戏的离线包缓存（用于重新初始化）
  static Future<void> clearCurrentGameCache() async {
    try {
      final dirPath = await offlineDirPath;
      final dir = Directory(dirPath);
      
      if (await dir.exists()) {
        await dir.delete(recursive: true);
        LogUtils.d('已清理当前游戏缓存: $dirPath');
      }
      
      // 清理版本记录
      final prefs = await SharedPreferences.getInstance();
      final appName = AppConfig.shared.appName;
      final versionKey = '${_versionKey}_${appName.toString()}';
      await prefs.remove(versionKey);
      
    } catch (e) {
      LogUtils.d('清理当前游戏缓存失败: $e');
    }
  }

  /// 检查是否需要检查服务器更新
  static Future<bool> shouldCheckForUpdates() async {
    try {
      final prefs = await SharedPreferences.getInstance();
      final lastCheckTime = prefs.getInt(_lastCheckTimeKey) ?? 0;
      final currentTime = DateTime.now().millisecondsSinceEpoch;
      
      // 如果超过检查间隔，则需要检查更新
      return (currentTime - lastCheckTime) > _checkInterval.inMilliseconds;
    } catch (e) {
      LogUtils.d('检查更新时间失败: $e');
      return true; // 出错时也检查更新
    }
  }

  /// 从服务器检查离线包更新
  static Future<Map<String, dynamic>?> checkServerUpdate() async {
    try {
      final prefs = await SharedPreferences.getInstance();
      final serverUrl = prefs.getString(_serverUrlKey) ?? _defaultServerUrl;
      final gameType = AppConfig.shared.gameType;
      final currentVersion = await getCurrentVersion();

      final url = '$serverUrl/api/offline-packages/check/$gameType?currentVersion=$currentVersion';
      
      LogUtils.d('检查服务器更新: $url');
      LogUtils.d('当前缓存版本: $currentVersion');
      
      final response = await http.get(
        Uri.parse(url),
        headers: {'Accept': 'application/json'},
      ).timeout(const Duration(seconds: 30));

      if (response.statusCode == 200) {
        final data = json.decode(response.body);
        if (data['success'] == true) {
          // 更新最后检查时间
          await prefs.setInt(_lastCheckTimeKey, DateTime.now().millisecondsSinceEpoch);
          
          if (data['hasUpdate'] == true) {
            final serverVersion = data['currentActiveVersion'] as String?;
            LogUtils.d('✅ 服务器发现新版本: $currentVersion -> $serverVersion');
            // 包含服务器返回的完整包信息和下载URL
            final packageInfo = Map<String, dynamic>.from(data['packageInfo']);
            packageInfo['downloadUrl'] = data['downloadUrl']; // 添加下载URL
            return packageInfo;
          } else {
            LogUtils.d('✅ 当前已是最新版本');
            return null;
          }
        } else {
          LogUtils.d('服务器返回错误: ${data['message']}');
          return null;
        }
      } else {
        LogUtils.d('服务器请求失败: ${response.statusCode}');
        return null;
      }
    } catch (e) {
      LogUtils.d('检查服务器更新失败: $e');
      return null;
    }
  }

  /// 从服务器下载并更新离线包
  static Future<bool> downloadAndUpdateFromServer({
    required Map<String, dynamic> packageInfo,
    Function(double progress)? onProgress,
  }) async {
    try {
      final downloadUrl = packageInfo['downloadUrl'] as String?;
      final version = packageInfo['version'] as String?;
      
      if (downloadUrl == null || downloadUrl.isEmpty) {
        LogUtils.d('下载链接不可用');
        return false;
      }
      
      if (version == null || version.isEmpty) {
        LogUtils.d('版本信息不可用');
        return false;
      }
      
      LogUtils.d('开始下载离线包: $downloadUrl, 版本: $version');

      final response = await http.get(Uri.parse(downloadUrl));

      if (response.statusCode == 200) {
        LogUtils.d('HTTP响应成功，文件大小: ${response.bodyBytes.length} bytes');
        
        // 验证文件哈希（如果服务器提供）
        final expectedHash = response.headers['x-package-hash'];
        if (expectedHash != null) {
          final actualHash = md5.convert(response.bodyBytes).toString();
          if (actualHash != expectedHash) {
            LogUtils.d('文件哈希验证失败: 期望=$expectedHash, 实际=$actualHash');
            return false;
          }
          LogUtils.d('文件哈希验证成功: $actualHash');
        }

        // 保存下载的文件到Documents目录
        final documentsDir = await getApplicationDocumentsDirectory();
        LogUtils.d('Documents目录: ${documentsDir.path}');
        
        final tempFile = File('${documentsDir.path}/temp_offline_package.zip');
        await tempFile.writeAsBytes(response.bodyBytes);
        LogUtils.d('临时文件保存成功: ${tempFile.path}');

        // 验证临时文件是否写入成功
        final tempFileExists = await tempFile.exists();
        final tempFileSize = tempFileExists ? await tempFile.length() : 0;
        LogUtils.d('临时文件验证: 存在=$tempFileExists, 大小=$tempFileSize bytes');

        if (!tempFileExists) {
          LogUtils.d('临时文件写入失败');
          return false;
        }

        LogUtils.d('离线包下载完成，开始解压...');

        // 解压文件
        final success = await _extractDownloadedPackage(tempFile, version);
        
        // 清理临时文件
        try {
          if (await tempFile.exists()) {
            await tempFile.delete();
            LogUtils.d('临时文件删除成功');
          }
        } catch (e) {
          LogUtils.d('删除临时文件失败: $e');
        }

        if (success) {
          LogUtils.d('✅ 离线包更新成功: $version');
          
          // 验证解压结果
          final targetDir = await offlineDirPath;
          final targetDirExists = await Directory(targetDir).exists();
          LogUtils.d('解压目录验证: $targetDir, 存在=$targetDirExists');
          
          if (targetDirExists) {
            final fileCount = await Directory(targetDir).list().length;
            LogUtils.d('解压文件数量: $fileCount');
          }
          
          return true;
        } else {
          LogUtils.d('❌ 离线包解压失败');
          return false;
        }
      } else {
        LogUtils.d('下载失败: HTTP ${response.statusCode}');
        return false;
      }
    } catch (e) {
      LogUtils.d('❌ 下载和更新离线包失败: $e');
      return false;
    }
  }

  /// 解压下载的离线包
  static Future<bool> _extractDownloadedPackage(File zipFile, String version) async {
    try {
      LogUtils.d('开始解压离线包文件: ${zipFile.path}');
      
      // 验证ZIP文件
      final zipExists = await zipFile.exists();
      final zipSize = zipExists ? await zipFile.length() : 0;
      LogUtils.d('ZIP文件状态: 存在=$zipExists, 大小=$zipSize bytes');
      
      if (!zipExists || zipSize == 0) {
        LogUtils.d('❌ ZIP文件无效或不存在');
        return false;
      }

      final bytes = await zipFile.readAsBytes();
      LogUtils.d('成功读取ZIP文件内容: ${bytes.length} bytes');
      
      final archive = ZipDecoder().decodeBytes(bytes);
      LogUtils.d('ZIP解码成功，包含文件数: ${archive.length}');

      final dirPath = await offlineDirPath;
      final dir = Directory(dirPath);
      LogUtils.d('目标解压路径: $dirPath');

      // 清理旧版本
      if (await dir.exists()) {
        LogUtils.d('清理旧版本目录...');
        await dir.delete(recursive: true);
        LogUtils.d('旧版本目录清理完成');
      }
      
      await dir.create(recursive: true);
      LogUtils.d('创建目标目录成功: $dirPath');

      // 解压新版本
      int extractedCount = 0;
      int totalFiles = archive.where((f) => f.isFile).length;
      LogUtils.d('准备解压 $totalFiles 个文件...');
      
      for (final file in archive) {
        final fileName = file.name;
        if (file.isFile) {
          try {
            final data = file.content as List<int>;
            final filePath = '$dirPath/$fileName';
            final newFile = File(filePath);
            
            LogUtils.d('解压文件 ${extractedCount + 1}/$totalFiles: $fileName (${data.length} bytes)');
            
            // 创建目录
            await newFile.parent.create(recursive: true);
            await newFile.writeAsBytes(data);
            
            // 验证文件是否写入成功
            final fileExists = await newFile.exists();
            final fileSize = fileExists ? await newFile.length() : 0;
            
            if (!fileExists || fileSize != data.length) {
              LogUtils.d('❌ 文件写入失败: $fileName, 预期大小=${data.length}, 实际大小=$fileSize');
              return false;
            }
            
            extractedCount++;
            LogUtils.d('✅ 文件解压成功: $fileName');
          } catch (e) {
            LogUtils.d('❌ 解压单个文件失败: $fileName, 错误: $e');
            return false;
          }
        }
      }

      LogUtils.d('所有文件解压完成: $extractedCount/$totalFiles');

      // 验证解压结果
      final extractedDir = Directory(dirPath);
      if (await extractedDir.exists()) {
        final actualFileCount = await extractedDir.list(recursive: true).where((entity) => entity is File).length;
        LogUtils.d('解压后目录文件数量: $actualFileCount');
      }

      // 更新版本记录
      final prefs = await SharedPreferences.getInstance();
      final appName = AppConfig.shared.appName;
      final versionKey = '${_versionKey}_${appName.toString()}';
      await prefs.setString(versionKey, version);
      LogUtils.d('版本记录更新完成: $version');

      // 如果是伏魔记，保存核心文件哈希
      if (AppConfig.shared.gameType == 'fmj') {
        LogUtils.d('保存伏魔记核心文件哈希...');
        await _saveFmjCoreFilesHashes(dirPath);
      }

      LogUtils.d('✅ 离线包解压完成: $version, 目标路径: $dirPath');
      return true;
    } catch (e) {
      LogUtils.d('❌ 解压离线包失败: $e');
      // 打印堆栈跟踪以便调试
      LogUtils.d('错误堆栈: ${e.toString()}');
      return false;
    }
  }

  /// 获取当前离线包版本
  static Future<String?> getCurrentVersion() async {
    try {
      final prefs = await SharedPreferences.getInstance();
      final appName = AppConfig.shared.appName;
      final versionKey = '${_versionKey}_${appName.toString()}';
      return prefs.getString(versionKey);
    } catch (e) {
      LogUtils.d('获取当前版本失败: $e');
      return null;
    }
  }

  /// 设置服务器地址
  static Future<void> setServerUrl(String url) async {
    try {
      final prefs = await SharedPreferences.getInstance();
      await prefs.setString(_serverUrlKey, url);
      LogUtils.d('服务器地址已设置: $url');
    } catch (e) {
      LogUtils.d('设置服务器地址失败: $e');
    }
  }

  /// 获取服务器地址
  static Future<String> getServerUrl() async {
    try {
      final prefs = await SharedPreferences.getInstance();
      return prefs.getString(_serverUrlKey) ?? _defaultServerUrl;
    } catch (e) {
      LogUtils.d('获取服务器地址失败: $e');
      return _defaultServerUrl;
    }
  }

  /// 检查并自动更新离线包
  static Future<bool> checkAndAutoUpdate({
    Function(String message)? onStatusChange,
    Function(double progress)? onProgress,
  }) async {
    try {
      LogUtils.d('🔍 开始自动更新检查流程...');
      
      // 检查是否需要检查更新
      if (!await shouldCheckForUpdates()) {
        LogUtils.d('距离上次检查时间太短，跳过更新检查');
        onStatusChange?.call('跳过更新检查(时间间隔)');
        return false;
      }

      LogUtils.d('✅ 满足更新检查条件，开始检查服务器更新');
      onStatusChange?.call('检查服务器更新...');
      
      // 检查服务器更新
      final updateInfo = await checkServerUpdate();
      if (updateInfo == null) {
        LogUtils.d('没有可用更新，当前已是最新版本');
        onStatusChange?.call('当前已是最新版本');
        return false;
      }

      final newVersion = updateInfo['version'] as String?;
      final downloadUrl = updateInfo['downloadUrl'] as String?;
      LogUtils.d('🆕 发现新版本: $newVersion');
      LogUtils.d('📥 下载URL: $downloadUrl');
      
      onStatusChange?.call('发现新版本，开始下载...');
      
      // 下载并安装更新
      LogUtils.d('开始下载和安装更新包...');
      final success = await downloadAndUpdateFromServer(
        packageInfo: updateInfo,
        onProgress: onProgress,
      );

      if (success) {
        onStatusChange?.call('更新完成');
        LogUtils.d('✅ 离线包自动更新成功，新版本: $newVersion');
        
        // 验证更新后的状态
        final currentVersion = await getCurrentVersion();
        final dirPath = await offlineDirPath;
        final dirExists = await Directory(dirPath).exists();
        LogUtils.d('更新后验证: 当前版本=$currentVersion, 目录存在=$dirExists, 路径=$dirPath');
        
        return true;
      } else {
        onStatusChange?.call('更新失败');
        LogUtils.d('❌ 离线包自动更新失败');
        return false;
      }
    } catch (e) {
      LogUtils.d('❌ 自动更新过程失败: $e');
      onStatusChange?.call('更新失败: $e');
      return false;
    }
  }

  // MARK: - 缓存管理功能

  /// 获取离线包缓存大小
  static Future<int> getCacheSize() async {
    try {
      final dirPath = await offlineDirPath;
      final dir = Directory(dirPath);
      
      if (!await dir.exists()) {
        return 0;
      }

      int totalSize = 0;
      await for (final entity in dir.list(recursive: true)) {
        if (entity is File) {
          try {
            final stat = await entity.stat();
            totalSize += stat.size;
          } catch (e) {
            LogUtils.d('获取文件大小失败: ${entity.path}, 错误: $e');
          }
        }
      }
      
      LogUtils.d('离线包缓存大小: ${(totalSize / 1024 / 1024).toStringAsFixed(2)} MB');
      return totalSize;
    } catch (e) {
      LogUtils.d('获取缓存大小失败: $e');
      return 0;
    }
  }

  /// 格式化文件大小显示
  static String formatFileSize(int bytes) {
    if (bytes < 1024) {
      return '$bytes B';
    } else if (bytes < 1024 * 1024) {
      return '${(bytes / 1024).toStringAsFixed(1)} KB';
    } else if (bytes < 1024 * 1024 * 1024) {
      return '${(bytes / 1024 / 1024).toStringAsFixed(1)} MB';
    } else {
      return '${(bytes / 1024 / 1024 / 1024).toStringAsFixed(1)} GB';
    }
  }

  /// 清理离线包缓存
  static Future<bool> clearCache() async {
    try {
      final dirPath = await offlineDirPath;
      final dir = Directory(dirPath);
      
      if (await dir.exists()) {
        await dir.delete(recursive: true);
        LogUtils.d('离线包缓存清理完成');
      }

      // 清理版本记录
      final prefs = await SharedPreferences.getInstance();
      final appName = AppConfig.shared.appName;
      final versionKey = '${_versionKey}_${appName.toString()}';
      await prefs.remove(versionKey);
      
      // 清理最后检查时间
      await prefs.remove(_lastCheckTimeKey);
      
      LogUtils.d('离线包数据清理完成');
      return true;
    } catch (e) {
      LogUtils.d('清理离线包缓存失败: $e');
      return false;
    }
  }

  /// 检查缓存完整性
  static Future<bool> validateCache() async {
    try {
      final dirPath = await offlineDirPath;
      final dir = Directory(dirPath);
      
      if (!await dir.exists()) {
        LogUtils.d('离线包目录不存在');
        return false;
      }

      // 检查关键文件是否存在
      final indexFile = File('$dirPath/index.html');
      if (!await indexFile.exists()) {
        LogUtils.d('关键文件index.html不存在，缓存可能已损坏');
        return false;
      }

      LogUtils.d('离线包缓存验证通过');
      return true;
    } catch (e) {
      LogUtils.d('验证离线包缓存失败: $e');
      return false;
    }
  }

  /// 获取缓存信息
  static Future<Map<String, dynamic>> getCacheInfo() async {
    try {
      final version = await getCurrentVersion();
      final size = await getCacheSize();
      final isValid = await validateCache();
      final dirPath = await offlineDirPath;
      
      return {
        'version': version,
        'size': size,
        'sizeFormatted': formatFileSize(size),
        'isValid': isValid,
        'path': dirPath,
        'lastUpdated': DateTime.now().toIso8601String(),
      };
    } catch (e) {
      LogUtils.d('获取缓存信息失败: $e');
      return {
        'version': null,
        'size': 0,
        'sizeFormatted': '0 B',
        'isValid': false,
        'path': '',
        'lastUpdated': DateTime.now().toIso8601String(),
      };
    }
  }
}