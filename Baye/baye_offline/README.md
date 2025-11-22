# 三国霸业 Web版

三国霸业是一款经典的步步高电子词典游戏，这是其HTML5重制版本，让您可以在现代浏览器中重温这款经典的三国题材策略游戏。

## 项目简介

本项目是三国霸业的Web移植版本，完整还原了原版游戏的玩法和特色，并增加了更多现代化的功能特性。

- 游戏引擎源码：http://gitee.com/bgwp/iBaye
- 在线游戏地址：https://www.bbkgames.com/
- 游戏修改教程：https://bgwp.gitee.io/baye-doc/script/index.html

## 主要特性

### 多样化操作模式
- 横屏触控模式：适合移动设备使用
- 横屏手势模式：提供更丰富的操作体验
- 竖屏键盘模式：适合PC端使用
- 竖屏键盘触控混合模式：兼具键盘和触控的优势

### 游戏体验优化
- 支持多种分辨率选择
  - 经典4988分辨率
  - 加大分辨率选项
- 跨平台支持
  - 自动检测设备类型
  - 针对移动端和桌面端优化

### 现代化功能
- 在线存档管理
- 存档导入导出
- 支持多个游戏版本选择

## 项目结构

```
baye_offline/
├── index.html                 # 游戏主页面
├── choose.html                # 版本选择页面
├── m.html                     # 移动端游戏界面
├── libs.json                  # 游戏模组配置
├── manifest.json              # PWA配置文件
├── css/                       # 样式文件
│   ├── baye.css              # 主样式
│   └── flex.css              # 弹性布局
├── js/                        # 游戏引擎和脚本
│   ├── baye.v2.js            # WebAssembly游戏引擎
│   ├── baye.v2.wasm          # WebAssembly二进制文件
│   ├── baye.v2.wasm.map      # 源映射文件
│   ├── bridge.js             # JavaScript桥接层
│   ├── lcd.js                # 显示逻辑
│   └── [其他库文件]           # jQuery、工具库等
├── libs/                      # 游戏模组和资源库
│   ├── dat-mod.lib           # 经典三国霸业
│   ├── qmlw-*.lib            # 群侠系列模组
│   └── [其他模组文件]        # 各种游戏版本
├── mapeditor/                 # 地图编辑器
│   ├── index.html            # 编辑器主页面
│   ├── img/                  # 地形素材
│   └── js/                   # 编辑器脚本
├── scripts/                   # 构建脚本
│   ├── build.js              # 离线包构建脚本
│   └── build.config.json     # 构建配置
├── package.json               # Node.js项目配置
├── CLAUDE.md                  # Claude Code指导文档
├── LICENSE                    # 许可证文件
└── README.md                  # 项目说明(本文件)
```

## 快速开始

### 本地运行
```bash
# 项目为纯静态网站，可使用任何HTTP服务器运行
python -m http.server 8000        # Python
npx http-server                   # Node.js
# 或使用IDE的Live Server插件
```

### 游戏操作
1. 打开游戏首页 (`index.html`)
2. 选择合适的操作模式和分辨率
3. 点击"选择版本"选择想玩的游戏版本
4. 点击"进入游戏"开始游戏

## 社区支持

- QQ交流群: 526266208
- 问题反馈和建议都可以在QQ群中联系

## 技术架构

### 核心技术
- **WebAssembly**: 游戏引擎使用 Emscripten 编译的 C++ 代码
- **HTML5 Canvas**: 游戏渲染和显示
- **JavaScript ES5+**: 桥接层和 UI 逻辑  
- **PWA**: 支持离线运行和桌面安装

### 浏览器兼容性
- **推荐**: Chrome 57+, Firefox 52+, Safari 11+
- **必需**: 支持 WebAssembly 和 HTML5 Canvas
- **移动端**: iOS Safari 11+, Android Chrome 57+

### 引擎架构
```
┌─────────────────────────────────────────┐
│            HTML5 用户界面层              │
├─────────────────────────────────────────┤
│     JavaScript 桥接层 (bridge.js)       │
├─────────────────────────────────────────┤
│   WebAssembly 游戏引擎 (baye.v2.wasm)   │
├─────────────────────────────────────────┤
│          Canvas 2D 渲染层               │
└─────────────────────────────────────────┘
```

## 离线包生成

本项目支持生成离线包，方便在无网络环境下使用。

### 生成步骤

1. 安装依赖
```bash
npm install
```

2. 执行打包命令
```bash
# 使用npm脚本
npm run build

# 或直接运行构建脚本
node scripts/build.js
```

### 打包说明

- 生成的离线包使用日期格式命名：`baye-offline-YYYYMMDD##.zip`
- 文件位于 `dist/` 目录
- 离线包包含以下资源：
  - HTML游戏页面 (`index.html`, `choose.html`, `m.html`)
  - CSS样式文件和字体
  - WebAssembly游戏引擎 (`baye.v2.js`, `baye.v2.wasm`)
  - JavaScript桥接层和工具库
  - 完整的游戏模组库 (`libs/`)
  - PWA配置和图标文件
  - 地图编辑器 (`mapeditor/`)
- 自动版本号管理和文件压缩优化

## 地图编辑器

项目内置可视化地图编辑器，支持创建自定义地图：

### 使用方法
1. 访问 `mapeditor/index.html` 
2. 点击"生成空白地图"创建 32x32 地图
3. 使用右侧工具面板选择地形进行绘制
4. 支持基础地形、山地、河流等多种地形类型
5. 使用智能河流工具自动连接河流
6. 完成后点击"导出地图"生成代码

### 地图特性
- **固定尺寸**: 32x32 网格地图
- **地形类型**: 45种不同地形（平原、山地、河流、城池等）
- **智能连接**: 河流地形自动匹配连接样式
- **可视化编辑**: 所见即所得的编辑体验
- **代码导出**: 直接生成可用的地图代码

## 游戏模组管理

### 模组配置 (`libs.json`)
```json
{
  "libs": [
    {
      "id": "dat-mod", 
      "name": "经典三国霸业",
      "file": "dat-mod.lib",
      "description": "原版游戏体验"
    }
  ]
}
```

### 支持的游戏版本
- **三国霸业系列**: 经典版、重置版、各种平衡性调整版
- **群侠系列**: 鹿鼎记、射雕英雄传、笑傲江湖等
- **自制模组**: 支持自定义游戏内容和剧情

### 添加新模组
1. 将 `.lib` 文件放入 `libs/` 目录
2. 在 `libs.json` 中添加对应配置
3. 可选择添加 `.about` 说明文件