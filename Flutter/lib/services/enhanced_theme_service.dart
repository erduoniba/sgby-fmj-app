import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import '../utils/app_config.dart';

/// Enhanced theme service providing consistent styling across the app
class EnhancedThemeService {
  static final EnhancedThemeService _instance = EnhancedThemeService._internal();
  static EnhancedThemeService get instance => _instance;

  EnhancedThemeService._internal();

  /// Get theme data for the current app
  ThemeData getThemeData(AppName appName) {
    switch (appName) {
      case AppName.hdFmjApp:
        return _getFmjTheme();
      case AppName.hdBayeApp:
        return _getBayeTheme();
      default:
        return _getBayeTheme();
    }
  }

  /// Get FMJ (伏魔记) dark theme
  ThemeData _getFmjTheme() {
    const primaryColor = Colors.amber;
    const backgroundColor = Color(0xFF1A1A1A);
    const surfaceColor = Color(0xFF2D2D2D);

    final colorScheme = ColorScheme.dark(
      primary: primaryColor,
      secondary: primaryColor.withValues(alpha: 0.8),
      surface: surfaceColor,
      // background: backgroundColor, // Deprecated - using surface instead
      onPrimary: Colors.black87,
      onSecondary: Colors.black87,
      onSurface: Colors.white,
      // onBackground: Colors.white, // Deprecated - using onSurface instead
      error: const Color(0xFFFF6B6B),
      onError: Colors.white,
    );

    return ThemeData(
      useMaterial3: true,
      colorScheme: colorScheme,
      scaffoldBackgroundColor: backgroundColor,

      // App Bar Theme
      appBarTheme: AppBarTheme(
        backgroundColor: const Color(0xFF2D2D2D),
        foregroundColor: Colors.white,
        elevation: 2,
        shadowColor: Colors.black.withValues(alpha: 0.3),
        systemOverlayStyle: const SystemUiOverlayStyle(
          statusBarColor: Colors.transparent,
          statusBarIconBrightness: Brightness.light,
        ),
        titleTextStyle: const TextStyle(
          color: Colors.white,
          fontSize: 20,
          fontWeight: FontWeight.w600,
        ),
      ),

      // Card Theme
      cardTheme: CardTheme(
        color: surfaceColor,
        elevation: 4,
        shadowColor: Colors.black.withValues(alpha: 0.3),
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(12),
        ),
      ),

      // List Tile Theme
      listTileTheme: const ListTileThemeData(
        textColor: Colors.white,
        iconColor: primaryColor,
        tileColor: Colors.transparent,
        selectedTileColor: Color(0xFF3D3D3D),
      ),

      // Input Decoration Theme
      inputDecorationTheme: InputDecorationTheme(
        filled: true,
        fillColor: surfaceColor,
        border: OutlineInputBorder(
          borderRadius: BorderRadius.circular(12),
          borderSide: BorderSide.none,
        ),
        focusedBorder: OutlineInputBorder(
          borderRadius: BorderRadius.circular(12),
          borderSide: const BorderSide(color: primaryColor, width: 2),
        ),
        labelStyle: const TextStyle(color: Colors.white70),
        hintStyle: const TextStyle(color: Colors.white38),
      ),

      // Elevated Button Theme
      elevatedButtonTheme: ElevatedButtonThemeData(
        style: ElevatedButton.styleFrom(
          backgroundColor: primaryColor,
          foregroundColor: Colors.black87,
          elevation: 3,
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(12),
          ),
          padding: const EdgeInsets.symmetric(horizontal: 24, vertical: 16),
        ),
      ),

      // Text Button Theme
      textButtonTheme: TextButtonThemeData(
        style: TextButton.styleFrom(
          foregroundColor: primaryColor,
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(12),
          ),
        ),
      ),

      // Icon Theme
      iconTheme: const IconThemeData(
        color: primaryColor,
        size: 24,
      ),

      // Divider Theme
      dividerTheme: DividerThemeData(
        color: Colors.white.withValues(alpha: 0.1),
        thickness: 1,
      ),

      // Bottom Sheet Theme
      bottomSheetTheme: const BottomSheetThemeData(
        backgroundColor: surfaceColor,
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.vertical(top: Radius.circular(20)),
        ),
      ),
    );
  }

  /// Get Baye (三国霸业) light theme
  ThemeData _getBayeTheme() {
    const primaryColor = Color(0xFF1976D2); // Blue
    const backgroundColor = Colors.white;
    const surfaceColor = Color(0xFFF5F5F5);

    final colorScheme = ColorScheme.light(
      primary: primaryColor,
      secondary: primaryColor.withValues(alpha: 0.8),
      surface: surfaceColor,
      // background: backgroundColor, // Deprecated - using surface instead
      onPrimary: Colors.white,
      onSecondary: Colors.white,
      onSurface: Colors.black87,
      // onBackground: Colors.black87, // Deprecated - using onSurface instead
      error: const Color(0xFFD32F2F),
      onError: Colors.white,
    );

    return ThemeData(
      useMaterial3: true,
      colorScheme: colorScheme,
      scaffoldBackgroundColor: backgroundColor,

      // App Bar Theme
      appBarTheme: AppBarTheme(
        backgroundColor: primaryColor,
        foregroundColor: Colors.white,
        elevation: 2,
        shadowColor: Colors.black.withValues(alpha: 0.2),
        systemOverlayStyle: const SystemUiOverlayStyle(
          statusBarColor: Colors.transparent,
          statusBarIconBrightness: Brightness.light,
        ),
        titleTextStyle: const TextStyle(
          color: Colors.white,
          fontSize: 20,
          fontWeight: FontWeight.w600,
        ),
      ),

      // Card Theme
      cardTheme: CardTheme(
        color: Colors.white,
        elevation: 2,
        shadowColor: Colors.black.withValues(alpha: 0.1),
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(12),
        ),
      ),

      // List Tile Theme
      listTileTheme: const ListTileThemeData(
        textColor: Colors.black87,
        iconColor: primaryColor,
        tileColor: Colors.transparent,
        selectedTileColor: Color(0xFFE3F2FD),
      ),

      // Input Decoration Theme
      inputDecorationTheme: InputDecorationTheme(
        filled: true,
        fillColor: surfaceColor,
        border: OutlineInputBorder(
          borderRadius: BorderRadius.circular(12),
          borderSide: BorderSide.none,
        ),
        focusedBorder: OutlineInputBorder(
          borderRadius: BorderRadius.circular(12),
          borderSide: const BorderSide(color: primaryColor, width: 2),
        ),
        labelStyle: const TextStyle(color: Colors.black54),
        hintStyle: const TextStyle(color: Colors.black38),
      ),

      // Elevated Button Theme
      elevatedButtonTheme: ElevatedButtonThemeData(
        style: ElevatedButton.styleFrom(
          backgroundColor: primaryColor,
          foregroundColor: Colors.white,
          elevation: 2,
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(12),
          ),
          padding: const EdgeInsets.symmetric(horizontal: 24, vertical: 16),
        ),
      ),

      // Text Button Theme
      textButtonTheme: TextButtonThemeData(
        style: TextButton.styleFrom(
          foregroundColor: primaryColor,
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(12),
          ),
        ),
      ),

      // Icon Theme
      iconTheme: const IconThemeData(
        color: primaryColor,
        size: 24,
      ),

      // Divider Theme
      dividerTheme: DividerThemeData(
        color: Colors.black.withValues(alpha: 0.1),
        thickness: 1,
      ),

      // Bottom Sheet Theme
      bottomSheetTheme: const BottomSheetThemeData(
        backgroundColor: Colors.white,
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.vertical(top: Radius.circular(20)),
        ),
      ),
    );
  }

  /// Get consistent text styles
  TextStyle getHeadingStyle(AppName appName) {
    return TextStyle(
      fontSize: 24,
      fontWeight: FontWeight.bold,
      color: appName == AppName.hdFmjApp ? Colors.white : Colors.black87,
    );
  }

  TextStyle getSubheadingStyle(AppName appName) {
    return TextStyle(
      fontSize: 18,
      fontWeight: FontWeight.w600,
      color: appName == AppName.hdFmjApp ? Colors.white70 : Colors.black54,
    );
  }

  TextStyle getBodyStyle(AppName appName) {
    return TextStyle(
      fontSize: 16,
      fontWeight: FontWeight.normal,
      color: appName == AppName.hdFmjApp ? Colors.white : Colors.black87,
    );
  }

  TextStyle getCaptionStyle(AppName appName) {
    return TextStyle(
      fontSize: 14,
      fontWeight: FontWeight.normal,
      color: appName == AppName.hdFmjApp ? Colors.white60 : Colors.black54,
    );
  }

  /// Get consistent spacing values
  static const double spacingTiny = 4.0;
  static const double spacingSmall = 8.0;
  static const double spacingMedium = 16.0;
  static const double spacingLarge = 24.0;
  static const double spacingXLarge = 32.0;

  /// Get consistent border radius
  static const double borderRadiusSmall = 8.0;
  static const double borderRadiusMedium = 12.0;
  static const double borderRadiusLarge = 16.0;

  /// Get consistent elevation values
  static const double elevationLow = 2.0;
  static const double elevationMedium = 4.0;
  static const double elevationHigh = 8.0;

  /// Get consistent animation durations
  static const Duration animationFast = Duration(milliseconds: 200);
  static const Duration animationMedium = Duration(milliseconds: 300);
  static const Duration animationSlow = Duration(milliseconds: 500);
}