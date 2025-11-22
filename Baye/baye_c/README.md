<style>
img[alt=m] {
    max-width: 350px;
}
img[alt=b] {
    max-width: 500px;
}
</style>

# Baye 三国游戏

步步高电子词典经典游戏 -- 三国霸业 多平台移植版本

目前已支持的平台：
- HTML5
- Web版本 (WebAssembly)
- 微信小游戏

交流QQ群：526266208

![游戏截图](images/ios-h5-k.png)

## 项目简介

Baye是一款三国题材的策略游戏，让玩家在三国时代扮演一位君主，通过管理城池、武将和军队，最终统一天下。游戏提供了多个历史时期的剧本，包括董卓弄权、曹操崛起、赤壁之战和三国鼎立等经典场景。

## 功能特点

- 🎮 多个历史剧本可选（董卓弄权、曹操崛起、赤壁之战、三国鼎立等）
- 👥 丰富的武将系统（超过200名历史武将，每位武将拥有独特属性和技能）
- 🏰 城池管理（内政发展、资源生产、军队训练）
- ⚔️ 战斗系统（战术选择、地形影响、武将特技）
- 💾 存档系统（支持多个存档位，云同步功能）
- 🌐 跨平台支持（Web、微信小游戏）
- 🛠️ MOD支持（自定义武将、城池、剧本和规则）
- 🎨 保留原汁原味的游戏体验，优化触摸屏操作

## 支持平台

- 🌐 Web版本（通过WebAssembly实现）
- 📱 微信小游戏版本

## 系统需求

### Web版本
- **浏览器**: 支持WebAssembly的现代浏览器（Chrome 57+, Firefox 53+, Safari 11+, Edge 16+）
- **处理器**: 任何现代处理器
- **内存**: 2GB RAM
- **网络**: 稳定的互联网连接

### 微信小游戏版本
- **微信版本**: 6.6.7或更高版本
- **系统要求**: 与微信兼容的iOS或Android设备

## 如何运行

### Web版本

Web版本通过WebAssembly技术实现，需要现代浏览器支持。

1. 确保安装了必要的构建工具：
   - CMake
   - Emscripten（用于编译WebAssembly）

2. 构建步骤：
```bash
# 创建构建目录
mkdir build && cd build

# 配置CMake
cmake ..

# 编译
make
```

### 主要目录说明

- `src/`: 包含游戏的核心逻辑，使用C语言编写
  - 战斗系统
  - 城市管理
  - 武将系统
  - 游戏引擎
  
- `js/`: Web版本实现
  - WebAssembly模块
  - Web界面代码

### 详细代码结构

#### 核心游戏文件

```
src/
├── main.c                 # 程序入口点
├── gamEng.c               # 游戏引擎主要实现
├── config.c               # 游戏配置管理
├── datman.c               # 数据管理
├── fsys.c                 # 文件系统抽象
├── data-bind.c            # 数据绑定实现
├── bind-objects.c         # 对象绑定系统
```

#### 战斗系统相关文件

```
src/
├── Fight.c                # 战斗系统主要实现
├── FightSub.c             # 战斗子系统
├── FgtCount.c             # 战斗计数和统计
├── FgtPkAi.c              # 战斗AI实现
├── tactic.c               # 战术系统
```

#### 城市管理相关文件

```
src/
├── citycmd.c              # 城市命令主模块
├── citycmdb.c             # 城市命令B组
├── citycmdc.c             # 城市命令C组
├── citycmdd.c             # 城市命令D组
├── citycmde.c             # 城市命令E组
├── cityedit.c             # 城市编辑功能
```

#### 平台特定实现

```
src/platform/
├── common/                # 跨平台共享代码
│   ├── bios.c             # 基本输入输出系统
│   ├── gui.c              # 通用图形界面
│   ├── sys.c              # 系统功能
│   └── sem.h              # 信号量定义
├── js/                    # Web/JavaScript平台实现
│   ├── exportjs.c         # JavaScript导出函数
│   ├── fsys.c             # Web文件系统
│   ├── sys-js.c           # Web系统功能
│   └── script.c           # 脚本处理
└── win/                   # Windows平台实现
    ├── fsys.c             # Windows文件系统
    ├── sys.c              # Windows系统功能
    └── window.c           # Windows窗口管理
```

#### 头文件结构

```
src/baye/                  # 核心头文件
├── attribute.h            # 属性定义
├── comm.h                 # 通用定义
├── consdef.h              # 常量定义
├── data-bind.h            # 数据绑定接口
├── enghead.h              # 引擎头文件
├── fight.h                # 战斗系统接口
├── fsys.h                 # 文件系统接口
└── version.h              # 版本信息

src/inc/                   # 辅助头文件
├── DictAfx.h              # 字典前缀
├── GuiEdit.h              # GUI编辑器
├── SysResv.h              # 系统保留
└── keytable.h             # 按键表
```

#### 资源文件

```
src/
├── Gamhzk.bin             # 游戏数据二进制文件
├── font.bin               # 字体数据
├── res-msgpack.c          # MessagePack资源处理
└── res-msgpack.h          # MessagePack资源头文件
```

#### 工具和辅助脚本

```
src/
├── con.py                 # Python辅助脚本
└── genver.py              # 版本生成脚本
```

#### 核心功能模块

1. **游戏引擎** (`gamEng.c`)
   - 游戏主循环
   - 状态管理
   - 场景切换

2. **数据管理** (`datman.c`, `data-bind.c`)
   - 游戏数据加载和保存
   - 数据结构定义
   - 数据绑定系统

3. **战斗系统** (`Fight.c`, `FightSub.c`, `FgtPkAi.c`)
   - 战斗逻辑
   - 战斗AI
   - 战术实现
   - 战斗结果计算

4. **城市管理** (`citycmd*.c`)
   - 城市建设
   - 资源管理
   - 人口控制
   - 军队征募

5. **界面系统** (`platform/common/gui.c`)
   - 菜单渲染
   - 用户输入处理
   - 界面元素绘制

## 技术栈

- 🔧 核心游戏逻辑：C语言
- 🌐 Web版本：
  - WebAssembly (编译自C代码)
  - JavaScript
- 🛠️ 构建工具：
  - CMake
  - Emscripten (用于WebAssembly)
  
## 游戏玩法

1. 选择历史剧本
   - 董卓弄权
   - 曹操崛起
   - 赤壁之战
   - 三国鼎立

2. 选择要扮演的君主

3. 游戏主要功能
   - 城池管理
   - 武将任命
   - 军队调配
   - 战斗指挥
   - 外交往来

4. 存档功能
   - 支持多个存档位
   - 可以随时保存游戏进度
   - 可以读取之前的存档继续游戏

## 游戏操作指南

### 基本操作

#### 移动端（触摸屏）
- **点击**: 选择菜单项、确认操作
- **滑动**: 在地图上移动视角
- **双击**: 进入城池或选择武将
- **长按**: 查看详细信息

#### 桌面端（Windows）
- **鼠标左键**: 选择菜单项、确认操作
- **鼠标右键**: 返回上一级菜单
- **方向键**: 在地图上移动视角
- **空格键**: 确认选择
- **ESC键**: 返回上一级菜单

### 界面说明

1. **主地图界面**
   - 顶部: 回合信息、日期、金钱
   - 中部: 地图区域
   - 底部: 功能菜单

2. **城池界面**
   - 城池信息: 人口、防御、粮食等
   - 武将列表: 驻守在城池的武将
   - 设施建造: 可建造的城池设施
   - 军队征募: 征募新军队

3. **武将界面**
   - 武将属性: 统率、武力、智力、政治
   - 装备栏: 武将当前装备
   - 技能列表: 武将特殊技能
   - 行动选项: 可执行的命令

### 游戏技巧

1. **开局建议**
   - 优先发展经济，增加城池收入
   - 集中训练少数精锐武将，而非平均发展
   - 注意城池间的连接，保持补给线畅通

2. **战斗策略**
   - 利用地形优势：山地防守，平原进攻
   - 合理使用战术：计谋可以扭转战局
   - 注意武将相性：某些武将搭配会产生额外效果

3. **资源管理**
   - 平衡军费和民生支出
   - 保持足够的粮食储备
   - 适时征税，但注意不要引起民变

4. **外交技巧**
   - 与强国结盟，共同对抗威胁
   - 利用外交手段分化敌人
   - 适时使用间谍获取情报

## 开发相关

如果你想参与开发，需要了解：

1. 构建系统使用CMake
2. 核心游戏逻辑使用C语言编写
3. 支持Web平台：
   - Web (WebAssembly)
   - 微信小游戏

### 编译指南

部分参考文档：https://juejin.cn/post/7133254352587718663

#### 通用步骤

1. 克隆代码库
```bash
git clone https://github.com/yourusername/baye.git
cd baye
```

2. 创建构建目录
```bash
mkdir build
cd build
```

#### 安装Emscripten (用于Web版本)

**Ubuntu系统**:
```bash
sudo apt install emscripten
```

**其他系统**:
参考[Emscripten官方文档](http://kripken.github.io/emscripten-site/docs/getting_started/downloads.html#platform-notes-installation-instructions-portable-sdk)

或者，您也可以下载配置好开发环境的虚拟机：[百度云下载](https://pan.baidu.com/s/1eRFehjW)

#### 开发环境配置

### 🚀 快速开始

1. **确保已安装Emscripten**
   ```bash
   # 检查emcc是否可用
   emcc --version
   ```

2. **如果未安装，有以下方式安装：**

   **Ubuntu/Debian:**
   ```bash
   sudo apt install emscripten
   ```

   **macOS/其他系统:**
   ```bash
   # 使用项目内置的emsdk（推荐）
   cd emsdk
   ./emsdk install latest
   ./emsdk activate latest
   source ./emsdk_env.sh
   ```

   或参考[官方文档](http://kripken.github.io/emscripten-site/docs/getting_started/downloads.html#platform-notes-installation-instructions-portable-sdk)

3. **下载霸业源码，编译生成Web版本**

```bash
git clone https://git.oschina.net/bgwp/iBaye.git
cd iBaye

# 激活Emscripten环境（如使用项目内置emsdk）
source emsdk/emsdk_env.sh

# 一键构建（推荐）
./simple_build.sh
```

#### 编译方式对比

本项目提供了多种编译方式，选择最适合你的：

| 方式 | 复杂度 | 命令数 | 维护性 | 推荐度 | 适用场景 |
|------|--------|--------|--------|--------|----------|
| **标准CMake方式** | ⭐ | 3个 | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ | 学习和理解构建过程 |
| **simple_build.sh** | ⭐⭐ | 1个 | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ | 日常开发推荐 |

#### 推荐使用方式

**🥇 方式一：simple_build.sh（推荐）**
```bash
# 一键构建，基于标准CMake流程
./simple_build.sh
```

**🥈 方式二：标准CMake方式**
```bash
# 手动执行标准流程
cd js
emconfigure cmake -DCMAKE_BUILD_TYPE=Release ../src
make
```

#### 结论

- **日常开发**: 使用 `simple_build.sh`（简单可靠）
- **学习构建**: 使用标准CMake命令（透明度高）

所有方式都会生成相同的WebAssembly文件，选择你觉得最舒适的即可。

#### 构建Web包（用于HTML5版本）

```bash
cd js/web
npm install
npm run build
```

#### 🌐 开发服务器

为了避免跨域问题，需要启动本地HTTP服务器：

**方式一：使用Live Server（推荐）**
```bash
cd js/web
npm run dev    # 启动Live Server并自动打开浏览器
# 或
npm run serve  # 仅启动服务器，不打开浏览器
```

**方式二：使用Python内置服务器**
```bash
cd js/web
python3 -m http.server 8009
```

然后访问：
- 🧪 **测试页面**: http://localhost:8009/test_wasm.html
- 📱 **移动版**: http://localhost:8009/m.html
- 🖥️ **桌面版**: http://localhost:8009/pc.html

##### Live Server 特性

- ✅ 自动刷新：只监听 `js/web` 目录变化
- ✅ 忽略源码：`src` 目录修改不会触发刷新
- ✅ 专注开发：专为前端调试优化

#### emcc 和 make 工具作用

**emcc（Emscripten Compiler）**
- **作用**：将 C/C++ 代码编译成 WebAssembly 和 JavaScript 代码
- **功能特点**：
  - 编译 C 源码为 WebAssembly 二进制文件（.wasm）
  - 生成 JavaScript 胶水代码（.js），处理 WebAssembly 与浏览器的交互
  - 提供运行时支持，包括内存管理、文件系统模拟等
  - 支持 ASYNCIFY 模式，使同步 C 代码能在异步 Web 环境中运行
- **在本项目中的应用**：
  - 将 `src/` 目录下的 C 游戏引擎代码编译为可在浏览器中运行的 WebAssembly
  - 处理平台抽象层，使游戏能在 Web 平台正常运行

**make（构建工具）**
- **作用**：根据 Makefile 规则自动化编译过程
- **工作流程**：
  - 读取 CMake 生成的 Makefile 文件
  - 分析源码依赖关系，确定编译顺序
  - 调用 emcc 编译器编译各个 C 源文件
  - 链接所有目标文件生成最终的 WebAssembly 模块
- **优势**：
  - 增量编译：只重新编译修改过的文件
  - 并行编译：支持多核并行加速编译过程
  - 依赖管理：自动处理文件依赖关系

**实际执行顺序和协作流程**：

**第一阶段：配置阶段**
1. `emconfigure cmake ../src` - 使用 Emscripten 的 CMake 包装器
   - emconfigure 设置 Emscripten 环境变量
   - 告诉 CMake 使用 emcc 作为 C 编译器
   - 生成适用于 WebAssembly 编译的 Makefile

**第二阶段：编译阶段**
2. `make` - 执行实际编译过程
   - 读取 CMake 生成的 Makefile
   - 分析源码依赖关系，确定编译顺序
   - 对每个 C 源文件调用 emcc 进行编译
   - emcc 将各个 C 文件编译为 WebAssembly 对象文件
   - 最后调用 emcc 进行链接，生成最终的 .js 和 .wasm 文件

**注意**：脚本中的 `emcc --version` 只是版本检查，实际编译时：
- emconfigure 负责配置编译环境
- make 负责管理编译流程  
- emcc 负责实际的代码编译和链接工作

#### 编译完成后，会生成以下文件：
   - `baye.js` - JavaScript胶水代码
   - `baye.wasm` - WebAssembly二进制文件
   - `baye.wasm.map` - 源代码映射文件（用于调试）

#### 微信小游戏版本

微信小游戏版本基于Web版本构建，需要额外的配置步骤：

1. 完成Web版本的编译
2. 将生成的文件复制到微信小游戏项目目录
3. 按照微信小游戏开发文档进行配置

### 贡献代码

我们欢迎开发者贡献代码，以下是贡献流程：

1. Fork项目仓库
2. 创建功能分支 (`git checkout -b feature/amazing-feature`)
3. 提交更改 (`git commit -m 'Add some amazing feature'`)
4. 推送到分支 (`git push origin feature/amazing-feature`)
5. 创建Pull Request

### 创建MOD

游戏支持MOD系统，允许玩家修改游戏内容。MOD可以修改以下内容：

1. 武将属性
2. 城池数据
3. 剧本设置
4. 游戏规则

#### MOD文件结构

MOD文件是一个ZIP格式的压缩包，包含以下结构：

```
mod_name/
├── info.json       # MOD信息
├── generals/       # 武将数据
├── cities/         # 城池数据
├── scenarios/      # 剧本数据
└── rules/          # 规则修改
```

#### info.json示例

```json
{
  "name": "我的三国MOD",
  "version": "1.0.0",
  "author": "MOD作者",
  "description": "这是一个示例MOD，修改了部分武将属性",
  "compatibility": "1.2.0"
}
```

#### 安装MOD

1. 将MOD文件放入游戏的`mods`目录
2. 在游戏中选择"MOD"菜单
3. 启用你的MOD
4. 重启游戏以应用MOD

## 注意事项

- 游戏数据保存在本地
- Web版本需要现代浏览器支持WebAssembly

## 性能优化建议

### Web版本优化
- 使用最新版本的Chrome或Firefox浏览器以获得最佳性能
- 关闭不必要的浏览器扩展
- 确保浏览器有足够的缓存空间
- 建议使用有线网络连接以获得更稳定的体验

### 移动设备优化
- 关闭后台应用以释放内存
- 开启设备的性能模式（如有）
- 保持设备有足够的存储空间（建议至少500MB可用空间）
- 定期清理设备缓存

### 桌面版本优化
- 更新显卡驱动到最新版本
- 关闭不必要的后台程序
- 调整系统电源计划为"高性能"模式
- 定期进行系统维护和清理

### 通用优化建议
1. **存档管理**
   - 定期清理不需要的存档
   - 重要存档建议备份
   - 不要同时保存过多存档（建议不超过20个）

2. **游戏设置**
   - 根据设备性能调整动画效果
   - 适当降低音效数量可提升性能
   - 在性能较差的设备上建议关闭部分特效

3. **运行环境**
   - 确保系统处于最新版本
   - 保持足够的可用内存
   - 避免在系统负载较高时运行游戏

## 游戏历史与背景

Baye三国游戏最初是一款在掌上设备上运行的策略游戏，经过多年发展，现已支持多个平台。游戏的核心理念是提供一个沉浸式的三国时代体验，让玩家能够重温这段波澜壮阔的历史。

### 开发历程

- **初始版本** - 最初为掌上设备开发
- **跨平台扩展** - 逐步支持iOS、Web、macOS和Windows平台
- **开源发布** - 代码开源，允许社区贡献和创建MOD

### 游戏设计理念

1. **历史准确性** - 尽可能还原三国时期的历史背景和人物特点
2. **策略深度** - 提供深度的策略玩法，考验玩家的统筹规划能力
3. **易于上手** - 简洁的界面和操作，降低新玩家的入门门槛
4. **高度可定制** - 通过MOD系统支持玩家创造和分享内容

## 常见问题解答(FAQ)

### Q: 游戏存档在哪里？
**A:** Web平台的存档位置：
- Web: 浏览器的localStorage

### Q: 如何转移存档？
**A:** 可以通过导出存档功能，生成一个.save文件，然后在其他设备上导入。

### Q: 游戏支持多语言吗？
**A:** 目前支持简体中文和英文。

### Q: 如何报告Bug？
**A:** 请在GitHub项目页面提交Issue，并尽可能提供详细的复现步骤和环境信息。

### Q: 如何联系开发者？
**A:** 可以通过GitHub项目页面或者游戏官方论坛联系我们。

## 未来计划

我们计划在未来版本中添加以下功能：

- [ ] 多人联机对战模式
- [ ] 更多历史剧本
- [ ] 改进的AI系统
- [ ] 更丰富的MOD支持
- [ ] 更多语言支持

## 游戏截图

### 移动端Web版本
![移动端Web界面](images/ios-h5-k.png)
![移动端Web界面2](images/ios-h5-k2.png)
![移动端Web界面3](images/ios-h5-k3.png)

### Web版本
![Web版本界面](images/pc-h5.png)



### MOD支持
![MOD界面](images/mod.png)
![MOD示例1](images/mod1.png)
![MOD示例2](images/mod2.png)

## 致谢

感谢所有为Baye三国游戏做出贡献的开发者和社区成员。特别感谢：

- 原始游戏设计者和开发团队
- 所有提交代码、报告问题和提供建议的贡献者
- 创建精彩MOD的社区成员
- 测试游戏并提供反馈的玩家

## 技术支持

如果你在游戏过程中遇到任何问题，可以通过以下方式获取支持：

1. 查阅[游戏Wiki](https://github.com/yourusername/baye/wiki)
2. 在[GitHub Issues](https://github.com/yourusername/baye/issues)提交问题
3. 加入我们的[Discord社区](https://discord.gg/bayegame)
4. 访问[游戏官方论坛](https://forum.bayegame.com)

## 许可证

本项目采用MIT许可证 - 详情请参阅[LICENSE](LICENSE)文件

```
MIT License

Copyright (c) 2023 Baye Game Team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```