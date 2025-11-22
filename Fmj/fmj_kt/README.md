# 伏魔记 (FMJ) H5版本

这是一个使用Kotlin语言开发的《伏魔记》游戏H5移植版本。原版《伏魔记》是步步高电子词典上的经典RPG游戏，本项目旨在将其移植到现代Web平台，让更多玩家能够体验这款经典游戏。

## 项目介绍

- 游戏类型：回合制RPG
- 开发语言：Kotlin
- 运行平台：H5 (Web Browser)
- 屏幕分辨率：160x96 (保持原版风格)

## 核心功能

### 1. 战斗系统
- 经典回合制战斗
- 支持普通攻击、魔法攻击、物品使用
- 完整的状态系统（睡眠、混乱等）
- 战斗胜利奖励（经验、金钱、物品掉落）
- 支持随机战斗和剧情战斗
- AI控制的敌人行为系统

### 2. 角色系统
- 多角色管理
- 详细的属性系统（生命值、魔法值、攻击力、防御力等）
- 等级成长系统
- 装备系统（8个装备位置）
- 技能学习系统

### 3. 场景系统
- 地图场景切换
- NPC交互系统
- 事件触发系统
- 存档/读档功能

### 4. 物品系统
- 装备管理（武器、防具等）
- 药品系统（恢复道具）
- 道具效果
- 物品栏管理
- 物品掉落机制

### 5. 魔法系统
- 攻击法术
- 辅助法术
- 恢复法术
- 特殊法术
- MP消耗机制

### 6. 脚本系统
游戏使用强大的脚本系统来控制游戏剧情和事件，支持79种不同的指令：

#### 场景控制指令
- LOADMAP：加载新地图
- SCREENS：设置屏幕位置
- SETSCENENAME：设置场景名称
- SHOWSCENENAME：显示场景名称

#### 角色控制指令
- CREATEACTOR：创建角色
- CREATENPC：创建NPC
- MOVE：移动角色
- NPCSTEP：NPC步进
- FACETOFACE：角色面对面

#### 战斗系统指令
- INITFIGHT：初始化战斗
- FIGHTENABLE：启用战斗
- ENTERFIGHT：进入战斗
- SETFIGHTMISS：设置战斗闪避
- SETARMSTOSS：设置武器投掷

#### 物品系统指令
- GAINGOODS：获得物品
- USEGOODS：使用物品
- TESTGOODSNUM：检测物品数量
- DELETEGOODS：删除物品

#### 对话系统指令
- SAY：对话框显示
- MESSAGE：消息显示
- MENU：菜单显示
- CHOICE：选择框显示

#### 剧情控制指令
- STARTCHAPTER：开始新章节
- CALLCHAPTER：调用章节
- SETEVENT：设置事件
- CLREVENT：清除事件

#### 存档系统指令
- GAMESAVE：游戏存档
- ENABLESAVE：允许存档
- DISABLESAVE：禁止存档

#### 属性系统指令
- ATTRIBTEST：属性检测
- ATTRIBSET：设置属性
- ATTRIBADD：增加属性

#### 其他系统指令
- MOVIE：播放动画
- SHOWGUT：显示卷轴
- TIMEMSG：定时消息
- RANDRADE：随机概率

## 技术实现

### 技术架构
- 前端：Kotlin/JS
- 渲染：HTML5 Canvas (160x96像素)
- 资源处理：
  - 使用Python脚本将二进制资源文件转换为JavaScript
  - 支持DAT.LIB格式的游戏数据文件
  - 使用自定义的字体文件（ASC16和HZK16）进行文字渲染

### 性能优化
- 使用Canvas进行高效的2D渲染
- 资源文件预加载
- 使用JavaScript数组缓存字体数据

### 浏览器兼容性
- 需要支持HTML5 Canvas的现代浏览器
- 推荐使用Chrome或Firefox的最新版本
- 移动设备支持：需要虚拟按键支持（未实现）

### 1. 脚本系统实现
```kotlin
class ScriptVM {
    // 指令处理器数组
    private val instructions: Array<Instruct?>
    
    // 脚本加载和编译
    fun loadScript(type: Int, index: Int): ScriptProcess
    fun compileScript(type: Int, index: Int): ScriptProcess
    
    // 指令执行
    fun execute(command: Command)
}
```

### 2. 战斗系统实现
```kotlin
class Combat {
    // 战斗状态管理
    private enum class CombatState {
        SelectAction,    // 玩家操作阶段
        PerformAction,   // 执行动作队列
        Win,            // 战斗胜利
        Loss,           // 战斗失败
        Exit            // 退出战斗
    }
}
```

### 3. 角色系统实现
```kotlin
class Player {
    // 基本属性
    var hp: Int
    var mp: Int
    var attack: Int
    var defend: Int
    var speed: Int
    var lingli: Int
    var luck: Int
    
    // 装备系统
    val equipments: Array<BaseGoods?>
    
    // 等级系统
    var level: Int
    var exp: Int
}
```

## 特色功能

1. **完整的剧情系统**
   - 强大的脚本引擎
   - 丰富的剧情指令
   - 支持分支剧情
   - 动态事件触发

2. **灵活的战斗系统**
   - 回合制战斗
   - 多样化的战斗指令
   - 战斗状态管理
   - AI决策系统

3. **丰富的角色系统**
   - 多角色管理
   - 属性成长
   - 装备系统
   - 技能系统

4. **完善的存档系统**
   - 多个存档位
   - 存档保护
   - 进度记录
   - 角色状态保存

## 控制说明

- 方向键：移动/选择
- 确认键(ENTER)：确认/对话
- 取消键(ESC)：返回/菜单
- 自动战斗：战斗中可开启自动模式

## 开发说明

### 环境要求
- Kotlin 开发环境
- Web浏览器（支持H5）

### 构建步骤

#### 环境要求
- IntelliJ IDEA（推荐使用最新版本）
- Kotlin插件（需支持Kotlin/JS）
- Python（用于资源文件构建）
- Web浏览器（支持HTML5）
- Gradle 7.6+ (项目构建工具)

#### Gradle 构建说明
项目使用 Gradle 进行构建管理，主要构建任务如下：

1. **构建命令**
```bash
# 完整构建+拷贝到 fmj_offline 工程中（推荐）
./build-and-copy.sh

# 完整构建
./gradlew buildGame    # 构建项目并复制所有必要资源

# 单独任务
./gradlew build         # 仅构建项目
./gradlew copyResources # 仅复制资源文件
```

2. **构建输出**
所有文件将输出到 `out/production/fmj.core/` 目录：
- `fmj.core.v2.js` - 游戏主程序
- `kotlin.js` - Kotlin 运行时
- `*.js.map` - Source maps 用于调试

3. **开发流程**
```bash
# 1. 修改代码后重新构建
./gradlew buildGame

# 2. 启动本地服务器
python3 -m http.server 8000

# 3. 在浏览器中访问
# 打开 http://localhost:8000
```

4. **调试支持**
- 源码映射已启用，可在浏览器开发工具中直接调试 Kotlin 源码
- 构建使用 DEVELOPMENT 模式，保留完整调试信息
- 支持浏览器开发工具的源码断点调试

5. **部署文件**
构建完成后，需要部署以下文件：
- `game.html` - 游戏主页面
- `out/production/fmj.core/kotlin.js` - Kotlin 运行时
- `out/production/fmj.core/fmj.core.v2.js` - 游戏主程序
- 其他资源文件（图片、音频等）

#### 构建游戏资源
游戏资源文件包括：
- `DAT.LIB`：主要游戏数据文件
- `ASC16`：ASCII字符字体文件
- `HZK16`：中文汉字字体文件
- `cb.lib`, `cbzz.lib`, `fmj.lib`, `fmj2.lib`：其他游戏数据文件

构建步骤：
```bash
# 进入assets目录
cd assets

# 确保脚本有执行权限
chmod +x bin2js.py
chmod +x buildrom.sh

# 运行资源构建脚本
./buildrom.sh
```

构建过程说明：
1. `bin2js.py`脚本将二进制资源文件（DAT.LIB、ASC16、HZK16）转换为JavaScript格式
2. 生成的JavaScript文件会被合并到`rom.js`中
3. 最终的`rom.js`文件包含了所有游戏所需的资源数据

#### 使用IntelliJ IDEA构建项目
1. 打开项目：使用IntelliJ IDEA打开项目根目录
2. 配置Kotlin/JS：
   - 确保项目使用Kotlin/JS编译器
   - 编译输出目录应为 `out/production/fmj.core/`
3. 构建项目：
   - 使用IntelliJ IDEA的构建功能（Build -> Build Project）
   - 确保生成了以下文件：
     - `out/production/fmj.core/fmj.core.v2.js`
     - `out/production/fmj.core/lib/kotlin.js`

#### 运行游戏
1. 确保所有必需的文件都在正确的位置：
   - 游戏核心JS文件：`out/production/fmj.core/fmj.core.v2.js`
   - Kotlin标准库：`out/production/fmj.core/lib/kotlin.js`
   - 游戏资源文件：`assets/rom.js`
   - 其他依赖库：`src/js/jquery.min.js`, `src/js/encoding-indexes.js`, `src/js/encoding.js`, `src/js/sys.js`

2. 在浏览器中运行：
   - 使用Web服务器（如Python的SimpleHTTPServer）启动项目：
     ```bash
     # Python 3.x
     python -m http.server
     # 或 Python 2.x
     python -m SimpleHTTPServer
     ```
   - 在浏览器中访问：`http://localhost:8000/game.html`

#### 游戏控制
游戏使用以下按键映射：
- `[` -> PageUp
- `]` -> PageDown
- `Space` -> Esc
- `Enter` -> 输入/确认

#### 开发调试
1. 使用IntelliJ IDEA的Kotlin插件进行开发，可以获得：
   - 代码补全
   - 语法高亮
   - 实时错误检查
   - 重构工具

2. 调试方法：
   - 使用浏览器的开发者工具进行调试
   - 在代码中添加console.log进行日志输出
   - 利用浏览器的源码映射功能直接调试Kotlin代码

3. 资源文件修改：
   - 修改assets目录下的资源文件后，需要重新运行buildrom.sh
   - 确保资源文件的编码格式正确（通常为UTF-8）

### 开发规范
- 使用Kotlin编码规范
- 模块化设计原则
- 注释完整性要求

## 贡献指南

1. Fork 本仓库
2. 创建新的功能分支
3. 提交更改
4. 发起 Pull Request

## 版本历史

- v006: 当前版本
  - 完整的战斗系统
  - 角色成长系统
  - 物品和装备系统
  - 存档功能

## 致谢

- 原版《伏魔记》开发团队
- 开源社区贡献者

## 许可证

本项目采用 MIT 许可证。详情请参阅 [LICENSE](LICENSE) 文件。

## 联系方式

- 项目仓库：[GitHub仓库地址]
- 问题反馈：请在GitHub仓库中提交Issue
- 贡献代码：欢迎提交Pull Request

*注：请将上述占位符替换为实际的联系方式和仓库地址*


其他开源项目：
安卓版源码地址(链接已经挂了访问不了了
	https://github.com/cdd990/fmj

九年前：伏魔记 Java 版本
	https://github.com/yuanmomo/fmj_pc
	程序大部分源码来源android版伏魔记(作者不详)，这个版本修改了绘图和事件监听可在电脑端运行

四年前：Android 版本 伏魔记
	https://github.com/artharyoung/android_fmj
	Android 版本 伏魔记 根据 GitHub 上一个 fmj_pc 版本稍加修改移植到Android平台

三年前：java桌面版伏魔记
	https://github.com/jacky14/fmj_pc
	java桌面版伏魔记

五年前：BBKRPGSimulator：C#版本
	https://github.com/stratosblue/BBKRPGSimulator
	Java版本伏魔记，然后对照翻译为C#版本

三年前：GameShell适配的步步高经典游戏
	https://github.com/zzxzzk115/BBKGames_GameShell
	基于 fmj.core.js 构建的h5页面游戏