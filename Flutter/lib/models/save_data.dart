class SaveData {
  final String key;
  final String title;
  final String content;
  final DateTime? createdDate;
  final String gameKey;
  final String gameName;
  
  SaveData({
    required this.key,
    required this.title,
    required this.content,
    this.createdDate,
    required this.gameKey,
    required this.gameName,
  });
  
  String get displayTitle {
    if (title.isEmpty) {
      return key;
    }
    return title;
  }
  
  String get displayContent {
    if (content.length > 100) {
      return '${content.substring(0, 100)}...';
    }
    return content;
  }
  
  String get formattedDate {
    if (createdDate == null) return "未知时间";
    final date = createdDate!;
    return '${date.year}-${date.month.toString().padLeft(2, '0')}-${date.day.toString().padLeft(2, '0')} '
           '${date.hour.toString().padLeft(2, '0')}:${date.minute.toString().padLeft(2, '0')}';
  }
  
  SaveData copyWith({
    String? key,
    String? title, 
    String? content,
    DateTime? createdDate,
    String? gameKey,
    String? gameName,
  }) {
    return SaveData(
      key: key ?? this.key,
      title: title ?? this.title,
      content: content ?? this.content,
      createdDate: createdDate ?? this.createdDate,
      gameKey: gameKey ?? this.gameKey,
      gameName: gameName ?? this.gameName,
    );
  }
}

class GameSection {
  final String gameKey;
  final String gameName;
  final List<SaveData> saveDataList;
  
  GameSection({
    required this.gameKey,
    required this.gameName,
    required this.saveDataList,
  });
}