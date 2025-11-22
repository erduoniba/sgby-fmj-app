import 'package:flutter/material.dart';

class CommonCard extends StatelessWidget {
  final String title;
  final List<Widget> children;
  final EdgeInsetsGeometry margin;

  const CommonCard({
    super.key,
    required this.title,
    required this.children,
    this.margin = const EdgeInsets.symmetric(horizontal: 8, vertical: 4),  // 减少垂直间距
  });

  @override
  Widget build(BuildContext context) {
    return Card(
      margin: margin,
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Padding(
            padding: const EdgeInsets.only(left: 16, top: 4, bottom: 2),  // 减少顶部padding，添加小的底部padding
            child: Text(
              title,
              style: const TextStyle(fontSize: 12, color: Colors.grey),
            ),
          ),
          ...children,
        ],
      ),
    );
  }
}