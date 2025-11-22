/**
 * 伏魔记(FMJ) Kotlin/JS 项目构建配置
 * 
 * 构建命令:
 * - ./gradlew build          # 仅构建项目
 * - ./gradlew copyResources  # 仅复制资源文件
 * - ./gradlew buildGame      # 构建项目并复制所有必要资源(推荐使用)
 *
 * 构建输出:
 * - 所有文件将输出到: out/production/fmj.core/
 * - 主要输出文件:
 *   - fmj.core.v2.js     # 游戏主程序
 *   - kotlin.js       # Kotlin 运行时
 *   - *.js.map       # Source maps 用于调试
 */

plugins {
    // 使用 Kotlin/JS 编译器插件，版本 1.6.21
    kotlin("js") version "1.6.21"
}

repositories {
    // 使用 Maven Central 仓库获取依赖
    mavenCentral()
}

kotlin {
    // 使用 Legacy 编译器后端
    // 注意: Legacy 后端已被弃用，但目前项目仍需要使用它来确保兼容性
    js(LEGACY) {
        browser {
            // 配置为可执行项目
            binaries.executable()
            
            // webpack 配置
            webpackTask {
                // 设置输出文件名
                outputFileName = "fmj.core.v2.js"
                // 使用开发模式，保留调试信息
                mode = org.jetbrains.kotlin.gradle.targets.js.webpack.KotlinWebpackConfig.Mode.DEVELOPMENT
                // 生成 source maps 用于调试
                sourceMaps = true
            }
            
            // 配置 Webpack 相关选项
            commonWebpackConfig {
                // 启用 CSS 支持，允许在项目中导入和使用 CSS 文件
                cssSupport.enabled = true
                // 设置输出的库类型为 UMD，兼容多种模块加载方式（CommonJS、AMD、全局变量等）
                output?.libraryTarget = "umd"
                // 设置导出的全局库名称为 "fmj.core"，可通过 window["fmj.core"] 访问
                output?.library = "fmj.core"
            }
        }
    }
    
    sourceSets {
        val main by getting {
            // 设置源代码目录
            kotlin.srcDir("src")
            
            dependencies {
                // 添加本地 kotlin.js 运行时依赖
                implementation(files("libs/kotlin.js"))
            }
        }
    }
}

// 定义输出目录
val outputDir = file("$projectDir/out/production/fmj.core")

/**
 * 复制资源文件任务
 * 将必要的运行时文件复制到构建输出目录
 */
tasks.register<Copy>("copyResources") {
    from("libs/kotlin.js")
    into(outputDir)
} 

/**
 * 复制编译后的JS文件到输出目录（禁用DCE）
 */
tasks.register<Copy>("copyCompiledJs") {
    dependsOn("compileKotlinJs", "browserProductionWebpack")
    from("$buildDir/distributions") {
        include("*.js")
        include("*.js.map")
    }
    into(outputDir)
}

/**
 * 游戏构建任务
 * 这是推荐使用的主构建任务，它会：
 * 1. 执行Kotlin/JS编译 (compileKotlinJs)
 * 2. 复制编译后的JS文件 (copyCompiledJs)
 * 3. 复制必要的资源文件 (copyResources)
 */
tasks.register("buildGame") {
    dependsOn("compileKotlinJs", "copyCompiledJs", "copyResources")
    group = "build"
    description = "构建游戏并复制所有必要资源"
}

/**
 * 使用说明:
 * 
 * 1. 首次构建:
 *    ./gradlew buildGame
 * 
 * 2. 开发流程:
 *    a. 修改代码
 *    b. 运行 ./gradlew buildGame
 *    c. 在浏览器中打开 game.html 测试
 * 
 * 3. 调试:
 *    - 源码映射已启用，可在浏览器开发工具中直接调试 Kotlin 源码
 *    - 构建输出在 out/production/fmj.core/ 目录中
 * 
 * 4. 部署:
 *    - 确保 game.html 中的脚本引用路径正确
 *    - 复制以下文件到服务器:
 *      - game.html
 *      - out/production/fmj.core/kotlin.js
 *      - out/production/fmj.core/fmj.core.v2.js
 *      - 其他资源文件(图片、音频等)
 *    - 构建输出在 out/production/fmj.core/ 目录中
 * 
 * 4. 部署:
 *    - 确保 game.html 中的脚本引用路径正确
 *    - 复制以下文件到服务器:
 *      - game.html
 *      - out/production/fmj.core/kotlin.js
 *      - out/production/fmj.core/fmj.core.v2.js
 *      - 其他资源文件(图片、音频等)
 */ 