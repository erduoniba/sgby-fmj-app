import 'package:flutter/material.dart';
import '../utils/app_config.dart';

/// Enhanced loading widget with smooth animations and better visual design
class EnhancedLoadingWidget extends StatefulWidget {
  final String message;
  final Color? color;
  final double size;
  final bool showMessage;

  const EnhancedLoadingWidget({
    super.key,
    this.message = 'Loading...',
    this.color,
    this.size = 50.0,
    this.showMessage = true,
  });

  @override
  State<EnhancedLoadingWidget> createState() => _EnhancedLoadingWidgetState();
}

class _EnhancedLoadingWidgetState extends State<EnhancedLoadingWidget>
    with TickerProviderStateMixin {
  late AnimationController _rotationController;
  late AnimationController _scaleController;
  late AnimationController _fadeController;

  late Animation<double> _rotationAnimation;
  late Animation<double> _scaleAnimation;
  late Animation<double> _fadeAnimation;

  @override
  void initState() {
    super.initState();

    // Rotation animation for spinner
    _rotationController = AnimationController(
      duration: const Duration(milliseconds: 1200),
      vsync: this,
    );
    _rotationAnimation = Tween<double>(
      begin: 0.0,
      end: 1.0,
    ).animate(CurvedAnimation(
      parent: _rotationController,
      curve: Curves.linear,
    ));

    // Scale animation for pulse effect
    _scaleController = AnimationController(
      duration: const Duration(milliseconds: 800),
      vsync: this,
    );
    _scaleAnimation = Tween<double>(
      begin: 0.8,
      end: 1.0,
    ).animate(CurvedAnimation(
      parent: _scaleController,
      curve: Curves.easeInOut,
    ));

    // Fade animation for message
    _fadeController = AnimationController(
      duration: const Duration(milliseconds: 600),
      vsync: this,
    );
    _fadeAnimation = Tween<double>(
      begin: 0.0,
      end: 1.0,
    ).animate(CurvedAnimation(
      parent: _fadeController,
      curve: Curves.easeIn,
    ));

    // Start animations
    _rotationController.repeat();
    _scaleController.repeat(reverse: true);
    _fadeController.forward();
  }

  @override
  void dispose() {
    _rotationController.dispose();
    _scaleController.dispose();
    _fadeController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final effectiveColor = widget.color ??
        (AppConfig.shared.appName == AppName.hdFmjApp
            ? Colors.white
            : Theme.of(context).colorScheme.primary);

    return Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          // Enhanced spinner with multiple animation layers
          AnimatedBuilder(
            animation: Listenable.merge([_rotationController, _scaleController]),
            builder: (context, child) {
              return Transform.scale(
                scale: _scaleAnimation.value,
                child: Transform.rotate(
                  angle: _rotationAnimation.value * 2.0 * 3.14159,
                  child: Container(
                    width: widget.size,
                    height: widget.size,
                    decoration: BoxDecoration(
                      shape: BoxShape.circle,
                      border: Border.all(
                        color: effectiveColor.withValues(alpha: 0.2),
                        width: 3,
                      ),
                    ),
                    child: Stack(
                      children: [
                        // Rotating gradient arc
                        CustomPaint(
                          size: Size(widget.size, widget.size),
                          painter: _LoadingSpinnerPainter(
                            color: effectiveColor,
                            strokeWidth: 3.0,
                          ),
                        ),
                        // Center dot
                        Center(
                          child: Container(
                            width: 8,
                            height: 8,
                            decoration: BoxDecoration(
                              shape: BoxShape.circle,
                              color: effectiveColor,
                            ),
                          ),
                        ),
                      ],
                    ),
                  ),
                ),
              );
            },
          ),

          if (widget.showMessage) ...[
            const SizedBox(height: 24),
            // Animated message text
            AnimatedBuilder(
              animation: _fadeController,
              builder: (context, child) {
                return Opacity(
                  opacity: _fadeAnimation.value,
                  child: Text(
                    widget.message,
                    style: TextStyle(
                      fontSize: 16,
                      fontWeight: FontWeight.w500,
                      color: effectiveColor.withValues(alpha: 0.8),
                    ),
                    textAlign: TextAlign.center,
                  ),
                );
              },
            ),
          ],
        ],
      ),
    );
  }
}

/// Custom painter for the loading spinner gradient effect
class _LoadingSpinnerPainter extends CustomPainter {
  final Color color;
  final double strokeWidth;

  _LoadingSpinnerPainter({
    required this.color,
    required this.strokeWidth,
  });

  @override
  void paint(Canvas canvas, Size size) {
    final center = Offset(size.width / 2, size.height / 2);
    final radius = (size.width / 2) - (strokeWidth / 2);

    // Create gradient paint
    final paint = Paint()
      ..style = PaintingStyle.stroke
      ..strokeWidth = strokeWidth
      ..strokeCap = StrokeCap.round;

    // Draw gradient arc
    const sweepAngle = 3.14159 * 1.5; // 270 degrees
    final rect = Rect.fromCircle(center: center, radius: radius);

    // Create gradient shader
    final gradient = SweepGradient(
      colors: [
        color.withValues(alpha: 0.0),
        color.withValues(alpha: 0.3),
        color.withValues(alpha: 0.7),
        color,
      ],
      stops: const [0.0, 0.3, 0.7, 1.0],
    );

    paint.shader = gradient.createShader(rect);
    canvas.drawArc(rect, -3.14159 / 2, sweepAngle, false, paint);
  }

  @override
  bool shouldRepaint(covariant CustomPainter oldDelegate) => false;
}

/// Game-specific loading widget with themed design
class GameLoadingWidget extends StatelessWidget {
  final String gameName;
  final String status;
  final double? progress;

  const GameLoadingWidget({
    super.key,
    required this.gameName,
    this.status = 'Initializing...',
    this.progress,
  });

  @override
  Widget build(BuildContext context) {
    final appName = AppConfig.shared.appName;
    final backgroundColor = AppConfig.shared.webViewBackgroundColor;
    final textColor = appName == AppName.hdFmjApp ? Colors.white : Colors.black87;

    return Container(
      width: double.infinity,
      height: double.infinity,
      color: backgroundColor,
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          // Game logo/icon placeholder
          Container(
            width: 80,
            height: 80,
            decoration: BoxDecoration(
              borderRadius: BorderRadius.circular(16),
              color: textColor.withValues(alpha: 0.1),
              border: Border.all(
                color: textColor.withValues(alpha: 0.2),
                width: 1,
              ),
            ),
            child: Icon(
              appName == AppName.hdFmjApp ? Icons.auto_fix_high : Icons.shield,
              size: 40,
              color: textColor.withValues(alpha: 0.7),
            ),
          ),

          const SizedBox(height: 24),

          // Game name
          Text(
            gameName,
            style: TextStyle(
              fontSize: 24,
              fontWeight: FontWeight.bold,
              color: textColor,
            ),
          ),

          const SizedBox(height: 32),

          // Enhanced loading spinner
          EnhancedLoadingWidget(
            message: status,
            color: textColor,
            size: 60,
          ),

          if (progress != null) ...[
            const SizedBox(height: 24),
            // Progress bar
            Container(
              width: 200,
              height: 4,
              decoration: BoxDecoration(
                borderRadius: BorderRadius.circular(2),
                color: textColor.withValues(alpha: 0.2),
              ),
              child: FractionallySizedBox(
                alignment: Alignment.centerLeft,
                widthFactor: progress! / 100,
                child: Container(
                  decoration: BoxDecoration(
                    borderRadius: BorderRadius.circular(2),
                    color: textColor.withValues(alpha: 0.7),
                  ),
                ),
              ),
            ),
            const SizedBox(height: 8),
            Text(
              '${progress!.toInt()}%',
              style: TextStyle(
                fontSize: 14,
                color: textColor.withValues(alpha: 0.7),
              ),
            ),
          ],
        ],
      ),
    );
  }
}

/// Simple loading overlay for quick operations
class LoadingOverlay extends StatelessWidget {
  final Widget child;
  final bool isLoading;
  final String message;

  const LoadingOverlay({
    super.key,
    required this.child,
    required this.isLoading,
    this.message = 'Loading...',
  });

  @override
  Widget build(BuildContext context) {
    return Stack(
      children: [
        child,
        if (isLoading)
          Container(
            color: Colors.black.withValues(alpha: 0.5),
            child: EnhancedLoadingWidget(
              message: message,
              color: Colors.white,
            ),
          ),
      ],
    );
  }
}