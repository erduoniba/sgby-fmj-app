import java.io.FileInputStream
import java.util.Properties

plugins {
    id("com.android.application")
    id("kotlin-android")
    // The Flutter Gradle Plugin must be applied after the Android and Kotlin Gradle plugins.
    id("dev.flutter.flutter-gradle-plugin")
}

// 加载签名配置
val keystoreProperties = Properties()
val keystorePropertiesFile = rootProject.file("key.properties")
if (keystorePropertiesFile.exists()) {
    keystoreProperties.load(FileInputStream(keystorePropertiesFile))
}

android {
    namespace = "com.harry.sgbaye"
    // 指定了项目编译所使用的Android SDK版本。
    // 编译器会基于这个版本来决定可用的API和资源。
    // 建议设置为项目所需的最高API级别，以利用最新的功能和安全更新。
    compileSdk = flutter.compileSdkVersion
    //  Android SDK at /Users/harrydeng/Library/Android/sdk 中可以查看
    ndkVersion = "27.0.12077973"

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }

    kotlinOptions {
        jvmTarget = JavaVersion.VERSION_11.toString()
    }

    // 签名配置
    signingConfigs {
        if (keystoreProperties.containsKey("keyAlias")) {
            create("release") {
                keyAlias = keystoreProperties["keyAlias"] as String
                keyPassword = keystoreProperties["keyPassword"] as String
                storeFile = rootProject.file(keystoreProperties["storeFile"] as String)
                storePassword = keystoreProperties["storePassword"] as String
            }
        }
    }

    defaultConfig {
        // TODO: Specify your own unique Application ID (https://developer.android.com/studio/build/application-id.html).
        applicationId = "com.harry.sgbaye"
        // You can update the following values to match your application needs.
        // For more information, see: https://flutter.dev/to/review-gradle-config.
        // 定义了应用程序能够运行的最低Android版本。确保设置合理，既能覆盖目标用户，又不限制不必要的设备兼容性。
        // 21对应 Android5.0 （2014 年 10 月）
        minSdk = 21
        // 声明了应用程序针对的目标Android版本。影响应用在不同Android版本上的行为和外观，
        // 建议设置为最新版本，以充分利用新功能和优化。
        // https://developer.android.google.cn/tools/releases/platforms?hl=zh-cn#11
        // 35对应Android 15，2024 年 6 月
        targetSdk = 35
        versionCode = flutter.versionCode
        versionName = flutter.versionName
    }
    
    buildTypes {
        debug {
            isDebuggable = true
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
        release {
            isMinifyEnabled = false
            isShrinkResources = false
            // 使用签名配置
            if (keystoreProperties.containsKey("keyAlias")) {
                signingConfig = signingConfigs.getByName("release")
            }
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }
}

flutter {
    source = "../.."
}
