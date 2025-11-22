#!/bin/bash

# Flutter Android 签名打包脚本
# 使用方法: ./build_signed_apk.sh

set -e

echo "=============================="
echo "Flutter Android 签名打包工具"
echo "=============================="

# 检查是否存在签名配置
if [ ! -f "android/key.properties" ]; then
    echo "❌ 错误: 签名配置文件 android/key.properties 不存在"
    echo "请确保已配置签名密钥!"
    exit 1
fi

if [ ! -f "android/app/hdsgbaye.jks" ]; then
    echo "❌ 错误: 签名密钥文件 android/app/hdsgbaye.jks 不存在"
    echo "请确保已生成签名密钥!"
    exit 1
fi

echo "✅ 签名配置检查通过"

# 清理之前的构建
echo "🧹 清理之前的构建..."
flutter clean
flutter pub get

# 构建签名APK
echo "📦 开始构建签名APK..."
flutter build apk --release

if [ $? -eq 0 ]; then
    echo "✅ APK构建成功!"
    
    # 显示APK信息
    APK_PATH="build/app/outputs/flutter-apk/harry_fmj_release.apk"
    APK_SIZE=$(ls -lh "$APK_PATH" | awk '{print $5}')
    echo "📍 APK路径: $APK_PATH"
    echo "📏 APK大小: $APK_SIZE"
    
    # 验证签名
    echo "🔍 验证APK签名..."
    jarsigner -verify -verbose "$APK_PATH" | grep "签名者" | head -1
    
    echo "=============================="
    echo "✅ 构建完成!"
    echo "APK文件位置: $APK_PATH"
    echo "=============================="
else
    echo "❌ APK构建失败!"
    exit 1
fi