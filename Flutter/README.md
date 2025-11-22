# hd_sgbaye

A new Flutter project.

## Getting Started

This project is a starting point for a Flutter application.

A few resources to get you started if this is your first Flutter project:

- [Lab: Write your first Flutter app](https://docs.flutter.dev/get-started/codelab)
- [Cookbook: Useful Flutter samples](https://docs.flutter.dev/cookbook)

For help getting started with Flutter development, view the
[online documentation](https://docs.flutter.dev/), which offers tutorials,
samples, guidance on mobile development, and a full API reference.



安装教程：
https://docs.flutter.cn/get-started

更新flutter的依赖组件
```
flutter pub get
```

运行flutter项目
```
flutter run
```

问题1：如果使用下载FlutterSDK方式，找不到flutter路径
解决方案：
在 .zshrc 添加flutter环境变量
```
# flutter环境
export PUB_HOSTED_URL="https://pub.flutter-io.cn"
export FLUTTER_STORAGE_BASE_URL="https://storage.flutter-io.cn"
PATH=/Users/harrydeng/HDProject/Flutter/flutter/bin:$PATH
```

问题1：Flutter 运行IOS真机，提示无法打开“iproxy”。
解决方案：
```
sudo xattr -d com.apple.quarantine /Users/harrydeng/HDProject/Flutter/flutter/bin/cache/artifacts/usbmuxd/iproxy
```

## iOS打包教程

1. 打开终端，进入项目目录
2. 运行以下命令生成iOS工程文件：
```
flutter build ios
```
3. 使用Xcode打开`ios/Runner.xcworkspace`
4. 配置签名证书：
   - 选择Runner项目
   - 选择Signing & Capabilities标签页
   - 选择正确的Team
   - 确保Automatically manage signing已勾选
5. 选择Product > Archive生成IPA文件

## Android打包教程
参考文档：https://doc.flutterchina.club/android-release/
1. 打开终端，进入项目目录
2. 生成签名密钥（如果尚未创建）：
```
keytool -genkey -v -keystore ~/key.jks -keyalg RSA -keysize 2048 -validity 10000 -alias key
```
3. 配置`android/key.properties`文件：
```
storePassword=<your_password>
keyPassword=<your_password>
keyAlias=key
storeFile=<path_to_key.jks>
```
4. 运行以下命令生成APK文件：
```
flutter build apk --release

# 生成多个架构的APK
flutter build apk --split-per-abi
```
5. 生成的APK文件位于`build/app/outputs/flutter-apk/app-release.apk`

6. fastlane 打包并上传到蒲公英平台，生成二维码给其他人使用
```
bundle exec fastlane build_pgyer
```

## 查看Android应用签名SHA1指纹

### 方法1: 使用keytool查看密钥库证书
```bash
# 查看调试版本SHA1指纹
keytool -list -v -keystore ~/.android/debug.keystore -alias androiddebugkey -storepass android -keypass android

# 查看发布版本SHA1指纹（本项目）
keytool -list -v -keystore android/app/hdsgbaye.jks -alias hdsgbaye -storepass 123456
```

### 方法2: 使用Gradle命令生成签名报告
```bash
cd android
./gradlew signingReport
```
此命令会显示所有构建变体的签名信息，包括调试版本和发布版本的SHA1、SHA256指纹。

### 方法3: 使用Android Studio
1. 打开Android Studio
2. 右侧打开 "Gradle" 面板
3. 展开 `app` → `Tasks` → `android`
4. 双击 `signingReport`
5. 在Build输出中查看SHA1指纹

### 方法4: 查看已构建APK的签名信息
```bash
# 需要Android SDK build-tools
$ANDROID_HOME/build-tools/[version]/apksigner verify --print-certs app-release.apk
```

### 当前项目签名信息
- **调试版本SHA1**: `29:EB:5C:17:25:76:07:DB:3B:80:22:94:F4:C5:46:0E:AE:81:6A:1B`
- **发布版本SHA1**: `2E:19:22:DB:F5:AF:A6:43:96:59:85:87:23:A7:0B:1C:83:DB:80:1C`
- **密钥库位置**: `android/app/hdsgbaye.jks`
- **密钥别名**: `hdsgbaye`

### SHA1指纹用途
- Firebase项目配置
- Google Play Console应用签名验证
- 第三方SDK集成（广告SDK、支付SDK、地图SDK等）
- API密钥配置和权限验证

## APK包体积分析

1. 在终端中运行以下命令生成包体积分析报告：
```
flutter build apk --analyze-size --target-platform android-arm64

✓ Built build/app/outputs/flutter-apk/app-release.apk (14.8MB)
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
app-release.apk (total compressed)                                         14 MB
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  META-INF/
    androidx                                                                3 KB
    CERT.SF                                                                18 KB
    CERT.RSA                                                              1021 B
    MANIFEST.MF                                                            16 KB
  assets/
    dexopt                                                                  1 KB
    flutter_assets                                                          6 MB
  classes.dex                                                             807 KB
  lib/
    arm64-v8a                                                               6 MB
    Dart AOT symbols accounted decompressed size                            4 MB
      package:flutter                                                       2 MB
      dart:core                                                           239 KB
      dart:typed_data                                                     175 KB
      dart:ui                                                             168 KB
      package:material_color_utilities                                     77 KB
      dart:async                                                           75 KB
      package:flutter_inappwebview_platform_interface                      74 KB
      dart:collection                                                      62 KB
      dart:convert                                                         49 KB
      dart:io                                                              40 KB
      package:archive                                                      39 KB
      package:vector_math/
        vector_math_64.dart                                                24 KB
      dart:isolate                                                         23 KB
      package:flutter_inappwebview_android                                 17 KB
      package:hd_sgbaye                                                    14 KB
      dart:ffi                                                             11 KB
      package:url_launcher_android                                          7 KB
      package:shared_preferences_android                                    5 KB
      package:package_info_plus_platform_interface                          3 KB
      package:url_launcher_platform_interface                               3 KB
    armeabi-v7a                                                             2 KB
    x86                                                                     2 KB
    x86_64                                                                  2 KB
  kotlin/
    collections                                                             1 KB
    kotlin.kotlin_builtins                                                  5 KB
    ranges                                                                  1 KB
    reflect                                                                 1 KB
  AndroidManifest.xml                                                       2 KB
  res/
    1I.9.png                                                                2 KB
    1J.9.png                                                                2 KB
    9w.png                                                                  4 KB
    FS.png                                                                 13 KB
    G2.9.png                                                                1 KB
    IX.9.png                                                                2 KB
    Li.9.png                                                                2 KB
    MF.9.png                                                                3 KB
    N3.png                                                                  3 KB
    NA.9.png                                                                2 KB
    RJ.png                                                                 23 KB
    color-v23                                                               2 KB
    color                                                                   4 KB
    dO.xml                                                                  1 KB
    h7.9.png                                                                1 KB
    io.9.png                                                                1 KB
    nf.png                                                                  1 KB
    o-.png                                                                 36 KB
    qD.9.png                                                                3 KB
    rj.9.png                                                                1 KB
    yn.png                                                                  9 KB
    zV.9.png                                                                2 KB
  resources.arsc                                                          232 KB
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒

```
2. 命令执行完成后，您将看到详细的包体积分析报告，包括：
   - 总APK大小
   - 各组件占用空间
   - 资源文件大小
   - 代码文件大小
3. 根据分析结果，您可以优化应用体积，例如：
   - 移除未使用的资源
   - 优化图片资源
   - 使用ProGuard进行代码混淆和优化



参考资料
Flutter 设置应用包名、名称、版本号、最低支持版本、Icon、启动页以及环境判断、平台判断和打包：
https://blog.csdn.net/wsyx768/article/details/136319899

Flutter Android 打包保姆式全流程 2023 版
https://juejin.cn/post/7207078219215929402

Flutter中WebViews的真正威力
https://juejin.cn/post/6869291513508659213