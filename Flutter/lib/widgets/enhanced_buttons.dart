import 'package:flutter/material.dart';
import '../services/haptic_feedback_service.dart';
import '../utils/app_config.dart';

/// Enhanced primary button with haptic feedback and smooth animations
class EnhancedPrimaryButton extends StatefulWidget {
  final String text;
  final VoidCallback? onPressed;
  final IconData? icon;
  final bool isLoading;
  final double borderRadius;
  final EdgeInsets padding;
  final Color? backgroundColor;
  final Color? textColor;
  final double elevation;
  final bool enableHaptic;

  const EnhancedPrimaryButton({
    super.key,
    required this.text,
    this.onPressed,
    this.icon,
    this.isLoading = false,
    this.borderRadius = 12.0,
    this.padding = const EdgeInsets.symmetric(horizontal: 24, vertical: 16),
    this.backgroundColor,
    this.textColor,
    this.elevation = 2.0,
    this.enableHaptic = true,
  });

  @override
  State<EnhancedPrimaryButton> createState() => _EnhancedPrimaryButtonState();
}

class _EnhancedPrimaryButtonState extends State<EnhancedPrimaryButton>
    with SingleTickerProviderStateMixin {
  late AnimationController _animationController;
  late Animation<double> _scaleAnimation;


  @override
  void initState() {
    super.initState();
    _animationController = AnimationController(
      duration: const Duration(milliseconds: 150),
      vsync: this,
    );
    _scaleAnimation = Tween<double>(
      begin: 1.0,
      end: 0.95,
    ).animate(CurvedAnimation(
      parent: _animationController,
      curve: Curves.easeInOut,
    ));
  }

  @override
  void dispose() {
    _animationController.dispose();
    super.dispose();
  }

  void _handleTapDown(TapDownDetails details) {
    _animationController.forward();
  }

  void _handleTapUp(TapUpDetails details) {
    _animationController.reverse();
  }

  void _handleTapCancel() {
    _animationController.reverse();
  }

  void _handleTap() {
    if (widget.enableHaptic) {
      HapticFeedbackService.instance.lightImpact();
    }
    widget.onPressed?.call();
  }

  @override
  Widget build(BuildContext context) {
    final theme = Theme.of(context);
    final effectiveBackgroundColor = widget.backgroundColor ??
        (AppConfig.shared.appName == AppName.hdFmjApp
            ? Colors.amber
            : theme.colorScheme.primary);
    final effectiveTextColor = widget.textColor ??
        (AppConfig.shared.appName == AppName.hdFmjApp
            ? Colors.black87
            : theme.colorScheme.onPrimary);

    return AnimatedBuilder(
      animation: _scaleAnimation,
      builder: (context, child) {
        return Transform.scale(
          scale: _scaleAnimation.value,
          child: GestureDetector(
            onTapDown: widget.onPressed != null && !widget.isLoading
                ? _handleTapDown
                : null,
            onTapUp: widget.onPressed != null && !widget.isLoading
                ? _handleTapUp
                : null,
            onTapCancel: _handleTapCancel,
            onTap: widget.onPressed != null && !widget.isLoading
                ? _handleTap
                : null,
            child: AnimatedContainer(
              duration: const Duration(milliseconds: 200),
              padding: widget.padding,
              decoration: BoxDecoration(
                color: widget.onPressed == null || widget.isLoading
                    ? effectiveBackgroundColor.withValues(alpha: 0.5)
                    : effectiveBackgroundColor,
                borderRadius: BorderRadius.circular(widget.borderRadius),
                boxShadow: widget.onPressed == null || widget.isLoading
                    ? []
                    : [
                        BoxShadow(
                          color: effectiveBackgroundColor.withValues(alpha: 0.3),
                          blurRadius: widget.elevation * 2,
                          offset: Offset(0, widget.elevation),
                        ),
                      ],
              ),
              child: Row(
                mainAxisSize: MainAxisSize.min,
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  if (widget.isLoading) ...[
                    SizedBox(
                      width: 16,
                      height: 16,
                      child: CircularProgressIndicator(
                        strokeWidth: 2,
                        valueColor: AlwaysStoppedAnimation<Color>(
                          effectiveTextColor,
                        ),
                      ),
                    ),
                    const SizedBox(width: 12),
                  ] else if (widget.icon != null) ...[
                    Icon(
                      widget.icon,
                      color: effectiveTextColor,
                      size: 20,
                    ),
                    const SizedBox(width: 12),
                  ],
                  Text(
                    widget.isLoading ? 'Loading...' : widget.text,
                    style: TextStyle(
                      color: effectiveTextColor,
                      fontSize: 16,
                      fontWeight: FontWeight.w600,
                    ),
                  ),
                ],
              ),
            ),
          ),
        );
      },
    );
  }
}

/// Enhanced secondary button with outline style
class EnhancedSecondaryButton extends StatefulWidget {
  final String text;
  final VoidCallback? onPressed;
  final IconData? icon;
  final double borderRadius;
  final EdgeInsets padding;
  final Color? borderColor;
  final Color? textColor;
  final bool enableHaptic;

  const EnhancedSecondaryButton({
    super.key,
    required this.text,
    this.onPressed,
    this.icon,
    this.borderRadius = 12.0,
    this.padding = const EdgeInsets.symmetric(horizontal: 24, vertical: 16),
    this.borderColor,
    this.textColor,
    this.enableHaptic = true,
  });

  @override
  State<EnhancedSecondaryButton> createState() =>
      _EnhancedSecondaryButtonState();
}

class _EnhancedSecondaryButtonState extends State<EnhancedSecondaryButton>
    with SingleTickerProviderStateMixin {
  late AnimationController _animationController;
  late Animation<double> _scaleAnimation;

  @override
  void initState() {
    super.initState();
    _animationController = AnimationController(
      duration: const Duration(milliseconds: 150),
      vsync: this,
    );
    _scaleAnimation = Tween<double>(
      begin: 1.0,
      end: 0.96,
    ).animate(CurvedAnimation(
      parent: _animationController,
      curve: Curves.easeInOut,
    ));
  }

  @override
  void dispose() {
    _animationController.dispose();
    super.dispose();
  }

  void _handleTap() {
    _animationController.forward().then((_) {
      _animationController.reverse();
    });

    if (widget.enableHaptic) {
      HapticFeedbackService.instance.selectionClick();
    }
    widget.onPressed?.call();
  }

  @override
  Widget build(BuildContext context) {
    final theme = Theme.of(context);
    final effectiveBorderColor = widget.borderColor ??
        (AppConfig.shared.appName == AppName.hdFmjApp
            ? Colors.amber
            : theme.colorScheme.primary);
    final effectiveTextColor = widget.textColor ?? effectiveBorderColor;

    return AnimatedBuilder(
      animation: _scaleAnimation,
      builder: (context, child) {
        return Transform.scale(
          scale: _scaleAnimation.value,
          child: GestureDetector(
            onTap: widget.onPressed != null ? _handleTap : null,
            child: Container(
              padding: widget.padding,
              decoration: BoxDecoration(
                color: Colors.transparent,
                borderRadius: BorderRadius.circular(widget.borderRadius),
                border: Border.all(
                  color: widget.onPressed == null
                      ? effectiveBorderColor.withValues(alpha: 0.5)
                      : effectiveBorderColor,
                  width: 2,
                ),
              ),
              child: Row(
                mainAxisSize: MainAxisSize.min,
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  if (widget.icon != null) ...[
                    Icon(
                      widget.icon,
                      color: effectiveTextColor,
                      size: 20,
                    ),
                    const SizedBox(width: 12),
                  ],
                  Text(
                    widget.text,
                    style: TextStyle(
                      color: effectiveTextColor,
                      fontSize: 16,
                      fontWeight: FontWeight.w600,
                    ),
                  ),
                ],
              ),
            ),
          ),
        );
      },
    );
  }
}

/// Enhanced icon button with ripple effect
class EnhancedIconButton extends StatefulWidget {
  final IconData icon;
  final VoidCallback? onPressed;
  final Color? color;
  final Color? backgroundColor;
  final double size;
  final double iconSize;
  final String? tooltip;
  final bool enableHaptic;

  const EnhancedIconButton({
    super.key,
    required this.icon,
    this.onPressed,
    this.color,
    this.backgroundColor,
    this.size = 48.0,
    this.iconSize = 24.0,
    this.tooltip,
    this.enableHaptic = true,
  });

  @override
  State<EnhancedIconButton> createState() => _EnhancedIconButtonState();
}

class _EnhancedIconButtonState extends State<EnhancedIconButton>
    with SingleTickerProviderStateMixin {
  late AnimationController _rippleController;
  late Animation<double> _rippleAnimation;

  @override
  void initState() {
    super.initState();
    _rippleController = AnimationController(
      duration: const Duration(milliseconds: 300),
      vsync: this,
    );
    _rippleAnimation = Tween<double>(
      begin: 0.0,
      end: 1.0,
    ).animate(CurvedAnimation(
      parent: _rippleController,
      curve: Curves.easeOut,
    ));
  }

  @override
  void dispose() {
    _rippleController.dispose();
    super.dispose();
  }

  void _handleTap() {
    _rippleController.forward().then((_) {
      _rippleController.reset();
    });

    if (widget.enableHaptic) {
      HapticFeedbackService.instance.lightImpact();
    }
    widget.onPressed?.call();
  }

  @override
  Widget build(BuildContext context) {
    final theme = Theme.of(context);
    final effectiveColor = widget.color ??
        (AppConfig.shared.appName == AppName.hdFmjApp
            ? Colors.white
            : theme.colorScheme.onSurface);

    final button = Material(
      color: widget.backgroundColor ?? Colors.transparent,
      borderRadius: BorderRadius.circular(widget.size / 2),
      child: InkWell(
        onTap: widget.onPressed != null ? _handleTap : null,
        borderRadius: BorderRadius.circular(widget.size / 2),
        child: Container(
          width: widget.size,
          height: widget.size,
          decoration: BoxDecoration(
            shape: BoxShape.circle,
            color: widget.backgroundColor,
          ),
          child: Stack(
            alignment: Alignment.center,
            children: [
              // Ripple effect
              AnimatedBuilder(
                animation: _rippleAnimation,
                builder: (context, child) {
                  return Container(
                    width: widget.size * _rippleAnimation.value,
                    height: widget.size * _rippleAnimation.value,
                    decoration: BoxDecoration(
                      shape: BoxShape.circle,
                      color: effectiveColor.withValues(
                        alpha: 0.2 * (1.0 - _rippleAnimation.value),
                      ),
                    ),
                  );
                },
              ),
              // Icon
              Icon(
                widget.icon,
                size: widget.iconSize,
                color: widget.onPressed == null
                    ? effectiveColor.withValues(alpha: 0.5)
                    : effectiveColor,
              ),
            ],
          ),
        ),
      ),
    );

    if (widget.tooltip != null) {
      return Tooltip(
        message: widget.tooltip!,
        child: button,
      );
    }

    return button;
  }
}

/// Enhanced floating action button with custom styling
class EnhancedFloatingActionButton extends StatefulWidget {
  final VoidCallback? onPressed;
  final Widget child;
  final Color? backgroundColor;
  final double elevation;
  final bool mini;
  final String? tooltip;
  final bool enableHaptic;

  const EnhancedFloatingActionButton({
    super.key,
    this.onPressed,
    required this.child,
    this.backgroundColor,
    this.elevation = 6.0,
    this.mini = false,
    this.tooltip,
    this.enableHaptic = true,
  });

  @override
  State<EnhancedFloatingActionButton> createState() =>
      _EnhancedFloatingActionButtonState();
}

class _EnhancedFloatingActionButtonState
    extends State<EnhancedFloatingActionButton>
    with SingleTickerProviderStateMixin {
  late AnimationController _scaleController;
  late Animation<double> _scaleAnimation;

  @override
  void initState() {
    super.initState();
    _scaleController = AnimationController(
      duration: const Duration(milliseconds: 200),
      vsync: this,
    );
    _scaleAnimation = Tween<double>(
      begin: 1.0,
      end: 1.1,
    ).animate(CurvedAnimation(
      parent: _scaleController,
      curve: Curves.elasticOut,
    ));
  }

  @override
  void dispose() {
    _scaleController.dispose();
    super.dispose();
  }

  void _handleTap() {
    _scaleController.forward().then((_) {
      _scaleController.reverse();
    });

    if (widget.enableHaptic) {
      HapticFeedbackService.instance.mediumImpact();
    }
    widget.onPressed?.call();
  }

  @override
  Widget build(BuildContext context) {
    final effectiveBackgroundColor = widget.backgroundColor ??
        (AppConfig.shared.appName == AppName.hdFmjApp
            ? Colors.amber
            : Theme.of(context).colorScheme.primary);

    return AnimatedBuilder(
      animation: _scaleAnimation,
      builder: (context, child) {
        return Transform.scale(
          scale: _scaleAnimation.value,
          child: FloatingActionButton(
            onPressed: widget.onPressed != null ? _handleTap : null,
            backgroundColor: effectiveBackgroundColor,
            elevation: widget.elevation,
            mini: widget.mini,
            tooltip: widget.tooltip,
            child: widget.child,
          ),
        );
      },
    );
  }
}