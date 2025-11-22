# HDBayeApp iOS项目

## 1、项目介绍

HDBayeApp是一个基于WebView的混合开发iOS应用，主要用于展示和运行离线Web内容。项目采用原生iOS与Web混合开发架构，具有以下特点：

- 支持离线Web内容加载和展示
- 原生与Web端双向通信机制
- 文件解压缩功能支持（SSZipArchive）
- 自动化打包发布流程（Fastlane）

离线Web内容来源于开源项目：[baye-alpha](https://gitee.com/harrydeng/baye-alpha)，该仓库包含了所有的Web端源码和资源文件。

### 1.1、主要功能模块

1. **Web内容展示（HDWebViewController）**
   - 基于WKWebView实现
   - 支持离线资源加载
   - JavaScript与原生交互能力

2. **关于页面（HDAboutViewController）**
   - 展示应用版本信息
   - 提供基本设置选项

3. **离线资源管理**
   - 离线Web资源打包
   - ZIP文件解压支持
   - 静态资源缓存机制

### 1.2技术特点

- 采用Swift语言开发
- 遵循iOS最新的应用生命周期管理（Scene-based）
- 使用CocoaPods进行依赖管理
- 集成Fastlane实现自动化构建和发布

### 1.3、H5性能数据

![](./Asserts/timestamp-diagram.svg)
#### 时间戳阶段说明
##### 游戏（血色华夏）页面（本App耗时，离线包策略）
|           时间戳 | 阶段                         | 说明                          |   阶段耗时(ms) |   累计耗时(ms) |
|---------------|----------------------------|-----------------------------|------------|------------|
| 1744357166795 | navigationStart            | 页面导航起始 (基准时间点)              |          0 |          0 |
| 1744357166795 | fetchStart                 | 开始获取资源 (缓存检查)               |          0 |          0 |
| 1744357166795 | domainLookupStart          | DNS查询开始 (解析域名)              |          0 |          0 |
| 1744357166795 | domainLookupEnd            | DNS查询完成                     |          0 |          0 |
| 1744357166795 | connectStart               | TCP连接开始 (建立通信通道)            |          0 |          0 |
| 1744357166795 | connectEnd                 | TCP连接完成                     |          0 |          0 |
| 1744357166795 | requestStart               | HTTP请求开始 (发送请求头)            |          0 |          0 |
| 1744357166795 | responseStart              | 接收首字节 (TTFB时间)              |          0 |          0 |
| 1744357166799 | responseEnd                | 响应完成 (接收最后字节)               |          4 |          4 |
| 1744357166800 | domLoading                 | 开始DOM解析 (解析HTML文档)          |          1 |          5 |
| 1744357166829 | domInteractive             | DOM解析完成 (可交互状态)             |         29 |         34 |
| 1744357166829 | domContentLoadedEventStart | DOMContentLoaded开始 (事件触发)   |          0 |         34 |
| 1744357166834 | domContentLoadedEventEnd   | DOMContentLoaded结束 (回调执行完成) |          5 |         39 |
| 1744357166843 | domComplete                | 页面完全加载 (所有资源就绪)             |          9 |         48 |
| 1744357166843 | loadEventStart             | load事件触发 (最终加载完成)           |          0 |         48 |


#### 未生效的零值阶段说明

- redirectStart/End=0：无重定向发生
- secureConnectionStart=0：未使用HTTPS协议（或SSL握手瞬时完成）
- unloadEventStart/End=0：前页面没有注册unload事件
- loadEventEnd=0：页面仍在运行未完全结束（或测量时尚未完成）

#### 关键性能指标解读

- DNS查询耗时：0ms（domainLookupEnd - domainLookupStart）
- TCP连接耗时：0ms（connectEnd - connectStart）
- 请求响应耗时：4ms（responseEnd - responseStart）
- DOM解析耗时：29ms（domInteractive - domLoading）
- DOMContentLoaded耗时：5ms（domContentLoadedEventEnd - domContentLoadedEventStart）
- 页面完全加载耗时：48ms（domComplete - navigationStart）

##### 游戏（血色华夏）页面（网页版本，包含了webview的缓存情况下）
|       时间戳(ms) | 阶段                         | 说明                           |   阶段耗时(ms) |   累计耗时(ms) |
|---------------|----------------------------|------------------------------|------------|------------|
| 1744364257656 | navigationStart            | 页面导航起始 (基准时间点)               |          0 |          0 |
| 1744364257656 | fetchStart                 | 开始获取资源 (缓存检查)                |          0 |          0 |
| 1744364257657 | domainLookupStart          | DNS查询开始 (解析域名)               |          1 |          1 |
| 1744364257714 | domainLookupEnd            | DNS查询完成                      |         57 |         58 |
| 1744364257716 | connectStart               | TCP连接开始 (建立通信通道)             |          2 |         60 |
| 1744364257779 | secureConnectionStart      | secureConnectionStart (未知阶段) |         63 |        123 |
| 1744364257931 | connectEnd                 | TCP连接完成                      |        152 |        275 |
| 1744364257934 | requestStart               | HTTP请求开始 (发送请求头)             |          3 |        278 |
| 1744364257989 | responseStart              | 接收首字节 (TTFB时间)               |         55 |        333 |
| 1744364258018 | responseEnd                | 响应完成 (接收最后字节)                |         29 |        362 |
| 1744364258021 | unloadEventStart           | unloadEventStart (未知阶段)      |          3 |        365 |
| 1744364258024 | unloadEventEnd             | unloadEventEnd (未知阶段)        |          3 |        368 |
| 1744364258059 | domLoading                 | 开始DOM解析 (解析HTML文档)           |         35 |        403 |
| 1744364258564 | domInteractive             | DOM解析完成 (可交互状态)              |        505 |        908 |
| 1744364258564 | domContentLoadedEventStart | DOMContentLoaded开始 (事件触发)    |          0 |        908 |
| 1744364258566 | domContentLoadedEventEnd   | DOMContentLoaded结束 (回调执行完成)  |          2 |        910 |
| 1744364258566 | domComplete                | 页面完全加载 (所有资源就绪)              |          0 |        910 |
| 1744364258566 | loadEventStart             | load事件触发 (最终加载完成)            |          0 |        910 |
| 1744364258566 | loadEventEnd               | loadEventEnd (未知阶段)          |          0 |        910 |


## 2、项目结构

```
HDBayeApp/
├── HDBayeApp/              # 主项目目录
│   ├── AppDelegate.swift    # App生命周期管理
│   ├── SceneDelegate.swift  # Scene生命周期管理
│   ├── WebView/             # Web视图相关组件
│   │   ├── HDWebViewController.swift    # Web视图控制器
│   │   ├── HDWebJSBridge.swift         # JS桥接实现
│   │   ├── HDBayeHooks.swift           # 钩子函数实现
│   │   └── HDWebURLSchemeHandler.swift  # URL请求处理器
│   ├── About/               # 关于页面组件
│   │   └── HDAboutViewController.swift  # 关于页面控制器
│   └── Resources/           # 资源文件目录
│       ├── Assets.xcassets  # 图片资源
│       ├── Info.plist       # 项目配置文件
│       └── baye-offline-2025040903.zip  # 离线资源包
├── Pods/                    # 第三方依赖
│   └── SSZipArchive/        # ZIP解压缩库
├── Asserts/                 # 项目资源
│   └── timestamp-diagram.svg # 性能数据图表
├── fastlane/                # 自动化打包配置
│   ├── Appfile              # App标识配置
│   ├── Fastfile             # 打包脚本配置
│   └── Pluginfile           # 插件配置
├── parse_performance_data.py # 性能数据解析脚本
└── 项目资料/                 # 项目相关资料
    ├── 市场图/               # 应用截图资源
    └── 证书资料/             # 开发证书和描述文件
```

## 3、Fastlane打包上传说明

### 3.1、环境配置

1. 安装Ruby环境（推荐使用rbenv管理Ruby版本）
2. 安装fastlane：
```bash
gem install fastlane
```
3. 安装项目依赖：
```bash
bundle install
```

### 3.2、打包命令

#### 上传到App Store
```bash
bundle exec fastlane build_upload platform:appstore
```

#### 上传到Firim
```bash
bundle exec fastlane build_upload2 platform:firim
```

### 3.3、注意事项

1. 确保已配置好开发者账号信息（在Appfile中）
2. 确保证书和描述文件已正确安装
3. 打包过程会自动生成dSYM文件，用于后续崩溃分析
4. 完整打包上传流程大约需要5-6分钟

### 3.4、相关配置文件

- `fastlane/Appfile`: 配置App ID、开发者账号等信息
- `fastlane/Fastfile`: 定义打包脚本和上传流程
- `fastlane/Pluginfile`: 配置所需的fastlane插件



## 4、性能数据解析工具

项目提供了`parse_performance_data.py`脚本用于解析和格式化性能数据。该工具可以将性能测试数据转换为Markdown表格格式，便于文档展示和分析。

#### 使用方法

1. 准备输入数据：
   - 将性能测试数据保存为JSON格式
   - 打开 parse_performance_data.py 文件，将JSON数据复制到 input_data 中

2. 运行脚本：

```bash
pip3 install tabulate

python3 parse_performance_data.py 
```

3. 输出格式：
   - 生成的Markdown表格包含时间戳、事件名称、说明、阶段耗时和累计耗时等列
   - 表格格式符合Markdown标准，可直接用于文档展示