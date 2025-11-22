import 'package:flutter/material.dart';
import '../utils/app_config.dart';

class GameSelector extends StatelessWidget {
  final AppName currentGame;
  final ValueChanged<AppName> onGameChanged;
  final String? modName;

  const GameSelector({
    super.key,
    required this.currentGame,
    required this.onGameChanged,
    this.modName,
  });

  @override
  Widget build(BuildContext context) {
    return PopupMenuButton<AppName>(
      onSelected: onGameChanged,
      itemBuilder: (BuildContext context) => <PopupMenuEntry<AppName>>[
        PopupMenuItem<AppName>(
          value: AppName.hdBayeApp,
          child: Row(
            children: [
              Icon(
                Icons.castle,
                color: currentGame == AppName.hdBayeApp 
                    ? Theme.of(context).primaryColor 
                    : Colors.grey,
              ),
              const SizedBox(width: 8),
              Text(
                '三国霸业',
                style: TextStyle(
                  color: currentGame == AppName.hdBayeApp 
                      ? Theme.of(context).primaryColor 
                      : null,
                  fontWeight: currentGame == AppName.hdBayeApp 
                      ? FontWeight.bold 
                      : FontWeight.normal,
                ),
              ),
              if (currentGame == AppName.hdBayeApp)
                const Spacer(),
              if (currentGame == AppName.hdBayeApp)
                Icon(
                  Icons.check,
                  color: Theme.of(context).primaryColor,
                  size: 16,
                ),
            ],
          ),
        ),
        PopupMenuItem<AppName>(
          value: AppName.hdFmjApp,
          child: Row(
            children: [
              Icon(
                Icons.sports_martial_arts,
                color: currentGame == AppName.hdFmjApp 
                    ? Theme.of(context).primaryColor 
                    : Colors.grey,
              ),
              const SizedBox(width: 8),
              Text(
                '伏魔记',
                style: TextStyle(
                  color: currentGame == AppName.hdFmjApp 
                      ? Theme.of(context).primaryColor 
                      : null,
                  fontWeight: currentGame == AppName.hdFmjApp 
                      ? FontWeight.bold 
                      : FontWeight.normal,
                ),
              ),
              if (currentGame == AppName.hdFmjApp)
                const Spacer(),
              if (currentGame == AppName.hdFmjApp)
                Icon(
                  Icons.check,
                  color: Theme.of(context).primaryColor,
                  size: 16,
                ),
            ],
          ),
        ),
      ],
      child: Row(
        mainAxisSize: MainAxisSize.min,
        children: [
          Text(
            _getGameTitle(currentGame),
            style: TextStyle(
              fontSize: 18,
              fontWeight: FontWeight.bold,
              color: currentGame == AppName.hdFmjApp ? Colors.white : null,
            ),
          ),
          const SizedBox(width: 4),
          Icon(
            Icons.arrow_drop_down,
            size: 20,
            color: currentGame == AppName.hdFmjApp ? Colors.white : null,
          ),
        ],
      ),
    );
  }

  String _getGameTitle(AppName game) {
    switch (game) {
      case AppName.hdBayeApp:
        return '三国霸业';
      case AppName.hdFmjApp:
        // If mod name is provided for FMJ, show it; otherwise show default
        return modName ?? '伏魔记';
      default:
        return '游戏';
    }
  }
}