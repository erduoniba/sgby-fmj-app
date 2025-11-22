import 'package:flutter/services.dart';
import 'package:flutter/foundation.dart';
import '../utils/log_utils.dart';

/// Haptic feedback service to provide tactile feedback for user interactions
/// Matches iOS haptic feedback patterns for consistent user experience
class HapticFeedbackService {
  static final HapticFeedbackService _instance = HapticFeedbackService._internal();
  static HapticFeedbackService get instance => _instance;

  HapticFeedbackService._internal();

  bool _isEnabled = true;

  /// Enable or disable haptic feedback globally
  void setEnabled(bool enabled) {
    _isEnabled = enabled;
    LogUtils.d('HapticFeedbackService: ${enabled ? 'Enabled' : 'Disabled'}');
  }

  /// Check if haptic feedback is enabled
  bool get isEnabled => _isEnabled;

  /// Light impact feedback - for subtle interactions like button taps
  Future<void> lightImpact() async {
    if (!_isEnabled) return;

    try {
      await HapticFeedback.lightImpact();
      if (kDebugMode) LogUtils.d('HapticFeedbackService: Light impact');
    } catch (e) {
      LogUtils.d('HapticFeedbackService: Light impact failed: $e');
    }
  }

  /// Medium impact feedback - for more significant interactions
  Future<void> mediumImpact() async {
    if (!_isEnabled) return;

    try {
      await HapticFeedback.mediumImpact();
      if (kDebugMode) LogUtils.d('HapticFeedbackService: Medium impact');
    } catch (e) {
      LogUtils.d('HapticFeedbackService: Medium impact failed: $e');
    }
  }

  /// Heavy impact feedback - for major actions like confirmations
  Future<void> heavyImpact() async {
    if (!_isEnabled) return;

    try {
      await HapticFeedback.heavyImpact();
      if (kDebugMode) LogUtils.d('HapticFeedbackService: Heavy impact');
    } catch (e) {
      LogUtils.d('HapticFeedbackService: Heavy impact failed: $e');
    }
  }

  /// Selection click feedback - for picker and selector interactions
  Future<void> selectionClick() async {
    if (!_isEnabled) return;

    try {
      await HapticFeedback.selectionClick();
      if (kDebugMode) LogUtils.d('HapticFeedbackService: Selection click');
    } catch (e) {
      LogUtils.d('HapticFeedbackService: Selection click failed: $e');
    }
  }

  /// Vibrate for error states or warnings
  Future<void> error() async {
    if (!_isEnabled) return;

    try {
      await HapticFeedback.heavyImpact();
      await Future.delayed(const Duration(milliseconds: 100));
      await HapticFeedback.heavyImpact();
      if (kDebugMode) LogUtils.d('HapticFeedbackService: Error vibration');
    } catch (e) {
      LogUtils.d('HapticFeedbackService: Error vibration failed: $e');
    }
  }

  /// Success feedback - for completed actions
  Future<void> success() async {
    if (!_isEnabled) return;

    try {
      await HapticFeedback.lightImpact();
      await Future.delayed(const Duration(milliseconds: 50));
      await HapticFeedback.mediumImpact();
      if (kDebugMode) LogUtils.d('HapticFeedbackService: Success feedback');
    } catch (e) {
      LogUtils.d('HapticFeedbackService: Success feedback failed: $e');
    }
  }

  /// Game action feedback - for in-game interactions
  Future<void> gameAction() async {
    if (!_isEnabled) return;

    try {
      await HapticFeedback.lightImpact();
      if (kDebugMode) LogUtils.d('HapticFeedbackService: Game action');
    } catch (e) {
      LogUtils.d('HapticFeedbackService: Game action failed: $e');
    }
  }

  /// Navigation feedback - for page transitions and navigation
  Future<void> navigation() async {
    if (!_isEnabled) return;

    try {
      await HapticFeedback.selectionClick();
      if (kDebugMode) LogUtils.d('HapticFeedbackService: Navigation feedback');
    } catch (e) {
      LogUtils.d('HapticFeedbackService: Navigation feedback failed: $e');
    }
  }
}