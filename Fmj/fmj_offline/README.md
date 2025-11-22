# 伏魔记 H5 移植版技术文档

## 项目概述

本项目是步步高电子词典经典游戏"伏魔记"的H5移植版本，将原本运行在电子词典上的游戏移植到现代浏览器中，支持桌面端和移动端设备。项目采用Kotlin游戏引擎转译为JavaScript，配合HTML5 Canvas技术实现游戏渲染和交互。

### 核心特性
- 🎮 完整移植步步高电子词典经典游戏
- 📱 支持移动端和桌面端跨平台运行
- 🎯 支持摇杆和按钮两种控制模式
- 🔄 支持横屏/竖屏自适应切换
- ⚡ 支持游戏倍速和多种视觉滤镜
- 🗺️ 内置小地图和位置显示功能
- 📦 加密打包部署系统

### 引擎源码
- 游戏引擎：[http://gitee.com/bgwp/fmj.kt](http://gitee.com/bgwp/fmj.kt)
- 相关项目：[三国霸业](http://gitee.com/bgwp/iBaye)

## 技术架构

### 整体架构图
```
┌─────────────────────────────────────────────────┐
│                  HTML5 前端层                    │
├─────────────────────────────────────────────────┤
│  index.html + CSS样式 + 控制UI组件               │
├─────────────────────────────────────────────────┤
│                 JavaScript层                    │
│  ┌─────────────┬─────────────┬─────────────────┐ │
│  │ 输入系统     │ 屏幕适配     │ 系统接口层      │ │
│  │ keyboard.js │ responsive  │ sys.js         │ │
│  │ nipplejs   │ CSS        │ m-native-bridge│ │
│  └─────────────┴─────────────┴─────────────────┘ │
├─────────────────────────────────────────────────┤
│                Kotlin引擎层                     │
│  ┌─────────────────────────────────────────────┐ │
│  │     fmj.core.v2.js (Kotlin转译)            │ │
│  │   - 游戏逻辑引擎                             │ │
│  │   - 状态管理                                │ │
│  │   - 渲染控制                                │ │
│  └─────────────────────────────────────────────┘ │
├─────────────────────────────────────────────────┤
│               Canvas渲染层                       │
│  HTML5 Canvas + 2D Context                     │
└─────────────────────────────────────────────────┘
```

### 核心组件说明

#### 1. 游戏引擎层
- **fmj.core.v2.js**: Kotlin转译的游戏核心引擎
  - 游戏逻辑处理
  - 状态管理和存档
  - 地图数据和角色系统
  - 战斗和剧情系统

- **fmj.core.js**: v1版本引擎（向后兼容）

#### 2. 系统接口层
- **sys.js**: 系统底层接口
  - 屏幕绘制接口
  - 键盘事件监听
  - 定时器管理
  - GBK编码支持

- **keyboard.js**: 键盘映射系统
  ```javascript
  var KEY_UP = 1       // 上方向键
  var KEY_DOWN = 2     // 下方向键  
  var KEY_LEFT = 3     // 左方向键
  var KEY_RIGHT = 4    // 右方向键
  var KEY_PAGEUP = 5   // 上翻页
  var KEY_PAGEDOWN = 6 // 下翻页
  var KEY_ENTER = 7    // 确认键
  var KEY_CANCEL = 8   // 取消键
  ```

#### 3. 输入控制系统
- **摇杆模式**: 基于nipplejs库实现虚拟摇杆
- **按钮模式**: 独立方向按钮和功能按钮
- **键盘支持**: 物理键盘映射支持

#### 4. 渲染系统
- **Canvas渲染**: HTML5 Canvas 2D渲染
- **动态尺寸**: 根据引擎版本自动设置画布尺寸
  - v2引擎: 320x192
  - v1引擎: 160x96

## 项目文件结构

```
fm/
├── index.html              # 主页面入口
├── CLAUDE.md              # Claude开发指导文档
├── RES.md                 # 项目说明文档
├── pack-game.sh           # 通用打包脚本
├── pack-fmj.sh           # 伏魔记专用打包脚本
├── js/                    # JavaScript核心文件
│   ├── fmj.core.v2.js    # v2游戏引擎(Kotlin转译)
│   ├── fmj.core.js       # v1游戏引擎
│   ├── sys.js            # 系统接口层
│   ├── keyboard.js       # 键盘处理
│   ├── m.js              # 移动端适配
│   ├── nipplejs.min.js   # 虚拟摇杆库
│   ├── jquery.min.js     # jQuery框架
│   ├── encoding.js       # 文本编码处理
│   ├── font.js           # 字体处理
│   └── rom.js            # ROM数据
├── style/                # CSS样式文件
│   ├── baye.css          # 主样式
│   └── joystick.css      # 摇杆样式
├── images/               # 控制图标
│   ├── up.png            # 上方向
│   ├── down.png          # 下方向
│   ├── left.png          # 左方向
│   ├── right.png         # 右方向
│   ├── enter.png         # 确认
│   ├── exit.png          # 取消
│   └── joystick.png      # 摇杆
├── games/                # 游戏ROM文件
│   ├── fmj/              # 伏魔记
│   ├── jyqxz/            # 金庸群侠传
│   ├── 侠客行/           # 侠客行
│   ├── 赤壁之战之乱世枭雄/
│   └── 赤壁之战之谁与争锋/
└── dist/                # 打包输出目录
```

## 输入系统详解

### 键位映射系统
游戏采用数字键位映射，兼容原电子词典的按键布局：

```javascript
// 基础键位定义
方向键：1(上) 2(下) 3(左) 4(右)
功能键：5(上翻页) 6(下翻页) 7(确认) 8(取消)

// 屏幕方向影响
横屏模式：键位保持原始映射
竖屏模式：方向键需要旋转映射
  - 原上(1) -> 实际右(4)
  - 原下(2) -> 实际左(3)  
  - 原左(3) -> 实际上(1)
  - 原右(4) -> 实际下(2)
```

### 控制模式

#### 1. 摇杆模式（默认）
- 使用nipplejs库创建虚拟摇杆
- 支持8方向输入
- 长按自动重复
- 位置自适应屏幕方向

#### 2. 按钮模式
- 独立的方向按钮组
- 独立的功能按钮组  
- 支持长按重复输入
- 视觉反馈和触觉效果

### 长按处理机制
```javascript
function setIntervalKeyDown(key) {
    onKeyDown(key);           // 立即触发
    clearInterval(interval);   
    interval = setInterval("onKeyDown(" + key + ")", 200); // 200ms重复
}
```

## 屏幕适配系统

### 响应式设计原理

#### 1. 屏幕方向检测
```javascript
var orientation = window.orientation;
switch (orientation) {
    case 90:
    case -90:
        modal = "landscape"; // 横屏
        break;
    default:
        modal = "portrait";  // 竖屏
        break;
}
```

#### 2. 画布旋转处理
```css
/* 竖屏模式下的画布变换 */
#lcd {
    transform-origin: left top;
    transform: rotate(-90deg) translateX(-heightpx);
}
```

#### 3. 控制器重布局
- **横屏**: 摇杆在左下角，按钮在右侧
- **竖屏**: 控制器整体旋转90度，重新定位

### 尺寸自适应
```javascript
function initScreen() {
    // 完全依赖 CSS 媒体查询控制尺寸
    // 不再动态设置容器尺寸
}
```

## 游戏特性功能

### 1. 倍速系统
```javascript
// 游戏速度倍率控制
window.gameSpeedMultiple(2); // 2倍速
```

### 2. 滤镜系统
支持多种视觉滤镜效果：
- `vintage1980`: 复古80年代效果
- `green`: 绿色单色效果  
- `red`: 红色单色效果
- `refreshing`: 清新效果

```javascript
window.setPresetFilter("refreshing");
```

### 3. 小地图功能
```javascript
// 显示/隐藏小地图
window.showMapContainer(true);

// 玩家位置闪烁动画
@keyframes blink {
    0%, 50% { opacity: 1; }
    51%, 100% { opacity: 0.3; }
}
```

## 打包部署系统

### 打包脚本功能

#### 1. pack-game.sh - 通用打包脚本
```bash
# 用法: ./pack-game.sh <包名称> [选项]
# 输出: <包名称>-offline-yyyymmddxx.zip 或 <包名称>-offline-yyyymmddxx-encrypted.zip

# 可选参数:
#   --encrypt    创建加密zip文件
#   --no-ios     不复制到iOS项目

# 使用示例:
./pack-game.sh fmj                    # 普通打包，复制到iOS
./pack-game.sh fmj --encrypt          # 加密打包，复制到iOS  
./pack-game.sh fmj --no-ios          # 普通打包，不复制到iOS
./pack-game.sh fmj --encrypt --no-ios # 加密打包，不复制到iOS
```

**主要流程：**
1. 复制index.html作为首页，写入版本号
2. 复制js/、style/、images/依赖文件，清理source maps
3. 根据--encrypt参数决定是否加密
4. 加密模式：创建临时包计算SHA256，生成密码并创建加密包
5. 非加密模式：直接创建标准zip包
6. 根据--no-ios参数决定是否复制到iOS项目并更新版本引用
7. 清理临时文件

#### 2. pack-fmj.sh - 伏魔记专用脚本
```bash
# 用法: ./pack-fmj.sh [--encrypt]  
# 输出: fmj-offline-yyyymmddxx.zip 或 fmj-offline-yyyymmddxx-encrypted.zip
```
专门为伏魔记游戏设计的打包脚本，支持加密参数控制。

### 安全机制

#### 1. SHA256完整性校验
- 基于压缩包内容生成SHA256哈希值
- 用于验证文件完整性，防止篡改
- 作为加密密码的组成部分

#### 2. 密码保护
```bash
# 密码格式
PASSWORD="${OUTPUT_FILE}-${SHA256}"
# 示例: fmj-offline-20250726-38ec9d4b62da97a761e17ea9ffb0f799c8a584daac8c757442a4e3fb90e8546d
```

#### 3. 加密流程
1. 创建临时压缩包 → 计算SHA256
2. 生成组合密码
3. 使用密码创建加密压缩包  
4. 删除临时文件，只保留加密版本

### 部署文件结构
```
解压后的离线包：
├── index.html           # 游戏入口页面
├── js/                  # JavaScript引擎和依赖
├── style/              # CSS样式文件  
├── images/             # 控制图标资源
└── favicon.png         # 站点图标（可选）
```

## 开发环境搭建

### 1. 环境要求
- 现代浏览器（支持HTML5 Canvas）
- Web服务器（处理跨域限制）
- bash环境（用于打包脚本）

### 2. 本地开发
```bash
# 1. 克隆项目
git clone <repository-url>

# 2. 启动本地服务器
python -m http.server 8000
# 或
npx serve .

# 3. 访问游戏
http://localhost:8000
```

### 3. 打包部署
```bash
# 打包伏魔记游戏（非加密）
./pack-fmj.sh

# 打包伏魔记游戏（加密）
./pack-fmj.sh --encrypt

# 打包其他游戏（非加密，复制到iOS）
./pack-game.sh fmj

# 打包其他游戏（加密，复制到iOS）
./pack-game.sh fmj --encrypt

# 打包其他游戏（非加密，不复制到iOS）
./pack-game.sh fmj --no-ios

# 打包其他游戏（加密，不复制到iOS）
./pack-game.sh fmj --encrypt --no-ios
```

## 性能优化

### 1. 资源优化
- JavaScript文件压缩（.min.js）
- 图片资源优化（PNG格式，适当尺寸）
- CSS样式合并和压缩

### 2. 渲染优化
- Canvas硬件加速
- 避免频繁DOM操作
- 合理的重绘频率控制

### 3. 内存管理
- 及时清理事件监听器
- 合理使用定时器
- 避免内存泄漏

## 浏览器兼容性

### 支持的浏览器
- **桌面端**: Chrome 60+, Firefox 55+, Safari 12+, Edge 79+
- **移动端**: iOS Safari 12+, Android Chrome 60+

### 关键API支持
- HTML5 Canvas 2D Context
- Touch Events (移动端)
- Orientation Events (屏幕旋转)
- Web Audio API (可选)

## 故障排除

### 常见问题

#### 1. 游戏无法加载
- 检查JavaScript控制台错误
- 确认所有资源文件路径正确
- 验证服务器MIME类型配置

#### 2. 控制无响应
- 检查触摸事件绑定
- 确认键位映射正确
- 验证事件传播没有被阻止

#### 3. 屏幕适配问题
- 检查CSS样式加载
- 确认屏幕方向检测正常
- 验证变换矩阵计算

#### 4. 性能问题
- 检查Canvas渲染频率
- 确认定时器没有冲突
- 优化事件处理频率

### 调试工具
```javascript
// 开启调试模式
window.debugMode = true;

// 查看当前状态
console.log('Current modal:', modal);
console.log('Orientation:', window.orientation);
console.log('Screen size:', window.innerWidth, 'x', window.innerHeight);
```

## API参考

### 全局接口
```javascript
// 游戏控制
window.gameSpeedMultiple(speed)     // 设置游戏倍速

// 视觉效果
window.setPresetFilter(name)        // 设置预设滤镜
window.showMapContainer(visible)    // 显示/隐藏小地图

// 引擎交互
window.fmjSendKeyDown(key)         // 发送按键按下
window.fmjSendKeyUp(key)           // 发送按键释放

// 版本信息
window.getOfflinePackageVersion()  // 获取当前离线包版本号
```

### 键位常量
```javascript
const KEYS = {
    UP: 1,         // 上方向
    DOWN: 2,       // 下方向  
    LEFT: 3,       // 左方向
    RIGHT: 4,      // 右方向
    PAGEUP: 5,     // 上翻页
    PAGEDOWN: 6,   // 下翻页
    ENTER: 7,      // 确认
    CANCEL: 8      // 取消
};
```

## 版本历史

### v2.0.1+ (当前版本) - 2025年8月
**最新更新:**
- ✨ **打包脚本增强**: 添加`--no-ios`参数支持，允许跳过iOS项目复制
- 🔧 **版本号注入**: 自动将版本号写入`index.html`，支持JavaScript运行时读取
- 🛠️ **多参数支持**: 支持`--encrypt`和`--no-ios`参数组合使用
- 📝 **文档完善**: 更新README.md包含完整的参数使用说明和示例
- 🐛 **Source Maps清理**: 构建时自动移除生产环境source map引用

**核心功能:**
- 新引擎升级到2.0.1版本
- 代码格式化和结构优化
- 小地图展示隐藏物品功能
- 按键频率从100ms改为200ms
- 改造使用index.html作为游戏页面

### 历史版本
- **v1.1.1**: 游戏肖像模式支持
- **v1.0.x**: 基础功能实现

## 许可证

本项目基于开源协议发布，具体许可证信息请查看项目根目录的LICENSE文件。

## 贡献指南

欢迎提交Issue和Pull Request来改进项目：

1. Fork项目仓库
2. 创建特性分支
3. 提交更改
4. 推送到分支
5. 创建Pull Request

## 技术支持

如需技术支持或有疑问，请通过以下方式联系：

- 项目Issue: 在GitHub/Gitee上提交Issue
- 引擎源码: [http://gitee.com/bgwp/fmj.kt](http://gitee.com/bgwp/fmj.kt)

---

*最后更新时间: 2025年8月25日*