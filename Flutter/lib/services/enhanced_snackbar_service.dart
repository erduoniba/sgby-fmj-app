import 'package:flutter/material.dart';
import '../utils/app_config.dart';
import 'haptic_feedback_service.dart';

/// Enhanced snackbar service with better styling and animations
class EnhancedSnackbarService {
  static final EnhancedSnackbarService _instance = EnhancedSnackbarService._internal();
  static EnhancedSnackbarService get instance => _instance;

  EnhancedSnackbarService._internal();

  /// Show a success message with green styling
  void showSuccess(
    BuildContext context,
    String message, {
    Duration duration = const Duration(seconds: 3),
    VoidCallback? onAction,
    String? actionLabel,
  }) {
    _showEnhancedSnackbar(
      context,
      message,
      SnackBarType.success,
      duration: duration,
      onAction: onAction,
      actionLabel: actionLabel,
    );
    HapticFeedbackService.instance.success();
  }

  /// Show an error message with red styling
  void showError(
    BuildContext context,
    String message, {
    Duration duration = const Duration(seconds: 4),
    VoidCallback? onAction,
    String? actionLabel,
  }) {
    _showEnhancedSnackbar(
      context,
      message,
      SnackBarType.error,
      duration: duration,
      onAction: onAction,
      actionLabel: actionLabel,
    );
    HapticFeedbackService.instance.error();
  }

  /// Show a warning message with orange styling
  void showWarning(
    BuildContext context,
    String message, {
    Duration duration = const Duration(seconds: 3),
    VoidCallback? onAction,
    String? actionLabel,
  }) {
    _showEnhancedSnackbar(
      context,
      message,
      SnackBarType.warning,
      duration: duration,
      onAction: onAction,
      actionLabel: actionLabel,
    );
    HapticFeedbackService.instance.mediumImpact();
  }

  /// Show an info message with blue styling
  void showInfo(
    BuildContext context,
    String message, {
    Duration duration = const Duration(seconds: 3),
    VoidCallback? onAction,
    String? actionLabel,
  }) {
    _showEnhancedSnackbar(
      context,
      message,
      SnackBarType.info,
      duration: duration,
      onAction: onAction,
      actionLabel: actionLabel,
    );
    HapticFeedbackService.instance.lightImpact();
  }

  /// Show a loading message with progress indicator
  void showLoading(
    BuildContext context,
    String message, {
    Duration duration = const Duration(seconds: 5),
  }) {
    _showEnhancedSnackbar(
      context,
      message,
      SnackBarType.loading,
      duration: duration,
    );
  }

  void _showEnhancedSnackbar(
    BuildContext context,
    String message,
    SnackBarType type, {
    Duration duration = const Duration(seconds: 3),
    VoidCallback? onAction,
    String? actionLabel,
  }) {
    // Remove any existing snackbar
    ScaffoldMessenger.of(context).removeCurrentSnackBar();

    final colorScheme = _getColorScheme(context, type);
    final icon = _getIcon(type);

    final snackBar = SnackBar(
      content: Row(
        children: [
          Icon(
            icon,
            color: colorScheme.onPrimary,
            size: 24,
          ),
          const SizedBox(width: 12),
          Expanded(
            child: Text(
              message,
              style: TextStyle(
                color: colorScheme.onPrimary,
                fontSize: 14,
                fontWeight: FontWeight.w500,
              ),
            ),
          ),
          if (type == SnackBarType.loading)
            SizedBox(
              width: 16,
              height: 16,
              child: CircularProgressIndicator(
                strokeWidth: 2,
                valueColor: AlwaysStoppedAnimation<Color>(colorScheme.onPrimary),
              ),
            ),
        ],
      ),
      backgroundColor: colorScheme.primary,
      duration: duration,
      behavior: SnackBarBehavior.floating,
      shape: RoundedRectangleBorder(
        borderRadius: BorderRadius.circular(12),
      ),
      margin: const EdgeInsets.all(16),
      elevation: 8,
      action: (onAction != null && actionLabel != null)
          ? SnackBarAction(
              label: actionLabel,
              textColor: colorScheme.onPrimary,
              onPressed: onAction,
            )
          : null,
    );

    ScaffoldMessenger.of(context).showSnackBar(snackBar);
  }

  ColorScheme _getColorScheme(BuildContext context, SnackBarType type) {
    final isDarkMode = AppConfig.shared.appName == AppName.hdFmjApp;

    switch (type) {
      case SnackBarType.success:
        return ColorScheme.fromSeed(
          seedColor: Colors.green,
          brightness: isDarkMode ? Brightness.dark : Brightness.light,
        );
      case SnackBarType.error:
        return ColorScheme.fromSeed(
          seedColor: Colors.red,
          brightness: isDarkMode ? Brightness.dark : Brightness.light,
        );
      case SnackBarType.warning:
        return ColorScheme.fromSeed(
          seedColor: Colors.orange,
          brightness: isDarkMode ? Brightness.dark : Brightness.light,
        );
      case SnackBarType.info:
        return ColorScheme.fromSeed(
          seedColor: Colors.blue,
          brightness: isDarkMode ? Brightness.dark : Brightness.light,
        );
      case SnackBarType.loading:
        return ColorScheme.fromSeed(
          seedColor: Colors.grey,
          brightness: isDarkMode ? Brightness.dark : Brightness.light,
        );
    }
  }

  IconData _getIcon(SnackBarType type) {
    switch (type) {
      case SnackBarType.success:
        return Icons.check_circle;
      case SnackBarType.error:
        return Icons.error;
      case SnackBarType.warning:
        return Icons.warning;
      case SnackBarType.info:
        return Icons.info;
      case SnackBarType.loading:
        return Icons.hourglass_empty;
    }
  }
}

/// Enhanced toast-style messages that appear as overlays
class EnhancedToastService {
  static final EnhancedToastService _instance = EnhancedToastService._internal();
  static EnhancedToastService get instance => _instance;

  EnhancedToastService._internal();

  OverlayEntry? _currentOverlay;

  /// Show a toast message
  void showToast(
    BuildContext context,
    String message, {
    Duration duration = const Duration(seconds: 2),
    ToastPosition position = ToastPosition.bottom,
    SnackBarType type = SnackBarType.info,
  }) {
    _removeCurrentToast();

    final overlay = Overlay.of(context);
    final colorScheme = _getColorScheme(context, type);
    final icon = _getIcon(type);

    _currentOverlay = OverlayEntry(
      builder: (context) => _ToastWidget(
        message: message,
        icon: icon,
        colorScheme: colorScheme,
        position: position,
        duration: duration,
        onRemove: _removeCurrentToast,
      ),
    );

    overlay.insert(_currentOverlay!);
    HapticFeedbackService.instance.lightImpact();
  }

  void _removeCurrentToast() {
    _currentOverlay?.remove();
    _currentOverlay = null;
  }

  ColorScheme _getColorScheme(BuildContext context, SnackBarType type) {
    final isDarkMode = AppConfig.shared.appName == AppName.hdFmjApp;

    switch (type) {
      case SnackBarType.success:
        return ColorScheme.fromSeed(
          seedColor: Colors.green,
          brightness: isDarkMode ? Brightness.dark : Brightness.light,
        );
      case SnackBarType.error:
        return ColorScheme.fromSeed(
          seedColor: Colors.red,
          brightness: isDarkMode ? Brightness.dark : Brightness.light,
        );
      case SnackBarType.warning:
        return ColorScheme.fromSeed(
          seedColor: Colors.orange,
          brightness: isDarkMode ? Brightness.dark : Brightness.light,
        );
      case SnackBarType.info:
        return ColorScheme.fromSeed(
          seedColor: Colors.blue,
          brightness: isDarkMode ? Brightness.dark : Brightness.light,
        );
      case SnackBarType.loading:
        return ColorScheme.fromSeed(
          seedColor: Colors.grey,
          brightness: isDarkMode ? Brightness.dark : Brightness.light,
        );
    }
  }

  IconData _getIcon(SnackBarType type) {
    switch (type) {
      case SnackBarType.success:
        return Icons.check_circle;
      case SnackBarType.error:
        return Icons.error;
      case SnackBarType.warning:
        return Icons.warning;
      case SnackBarType.info:
        return Icons.info;
      case SnackBarType.loading:
        return Icons.hourglass_empty;
    }
  }
}

/// Toast widget implementation
class _ToastWidget extends StatefulWidget {
  final String message;
  final IconData icon;
  final ColorScheme colorScheme;
  final ToastPosition position;
  final Duration duration;
  final VoidCallback onRemove;

  const _ToastWidget({
    required this.message,
    required this.icon,
    required this.colorScheme,
    required this.position,
    required this.duration,
    required this.onRemove,
  });

  @override
  State<_ToastWidget> createState() => _ToastWidgetState();
}

class _ToastWidgetState extends State<_ToastWidget>
    with SingleTickerProviderStateMixin {
  late AnimationController _animationController;
  late Animation<double> _fadeAnimation;
  late Animation<Offset> _slideAnimation;

  @override
  void initState() {
    super.initState();

    _animationController = AnimationController(
      duration: const Duration(milliseconds: 300),
      vsync: this,
    );

    _fadeAnimation = Tween<double>(
      begin: 0.0,
      end: 1.0,
    ).animate(CurvedAnimation(
      parent: _animationController,
      curve: Curves.easeOut,
    ));

    _slideAnimation = Tween<Offset>(
      begin: widget.position == ToastPosition.top
          ? const Offset(0, -1)
          : const Offset(0, 1),
      end: Offset.zero,
    ).animate(CurvedAnimation(
      parent: _animationController,
      curve: Curves.easeOutBack,
    ));

    _animationController.forward();

    // Auto-dismiss
    Future.delayed(widget.duration, () {
      if (mounted) {
        _animationController.reverse().then((_) => widget.onRemove());
      }
    });
  }

  @override
  void dispose() {
    _animationController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Positioned(
      top: widget.position == ToastPosition.top ? 100 : null,
      bottom: widget.position == ToastPosition.bottom ? 100 : null,
      left: 16,
      right: 16,
      child: AnimatedBuilder(
        animation: _animationController,
        builder: (context, child) {
          return FadeTransition(
            opacity: _fadeAnimation,
            child: SlideTransition(
              position: _slideAnimation,
              child: Material(
                color: Colors.transparent,
                child: Container(
                  padding: const EdgeInsets.symmetric(horizontal: 16, vertical: 12),
                  decoration: BoxDecoration(
                    color: widget.colorScheme.primary,
                    borderRadius: BorderRadius.circular(8),
                    boxShadow: [
                      BoxShadow(
                        color: Colors.black.withValues(alpha: 0.2),
                        blurRadius: 8,
                        offset: const Offset(0, 2),
                      ),
                    ],
                  ),
                  child: Row(
                    mainAxisSize: MainAxisSize.min,
                    children: [
                      Icon(
                        widget.icon,
                        color: widget.colorScheme.onPrimary,
                        size: 20,
                      ),
                      const SizedBox(width: 8),
                      Flexible(
                        child: Text(
                          widget.message,
                          style: TextStyle(
                            color: widget.colorScheme.onPrimary,
                            fontSize: 14,
                            fontWeight: FontWeight.w500,
                          ),
                        ),
                      ),
                    ],
                  ),
                ),
              ),
            ),
          );
        },
      ),
    );
  }
}

enum SnackBarType {
  success,
  error,
  warning,
  info,
  loading,
}

enum ToastPosition {
  top,
  bottom,
}