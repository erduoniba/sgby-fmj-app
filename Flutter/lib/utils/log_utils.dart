import 'package:flutter/foundation.dart';

class LogUtils {
  static void d(String message) {
    if (kDebugMode) {
      print(message);
    }
  }
}