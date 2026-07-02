# Skeet

Skeet 是一个基于 Visual Studio / MSBuild 的 C++ x64 工程，当前解决方案入口为 `Skeet.sln`，主工程为 `Skeet.vcxproj`，Release x64 产物输出到 `x64/Skeet/Skeet.exe`。

## 当前菜单功能

### 1. 自瞄 / AimBot

- 总开关：启动、自瞄启用、锁定切换、可视检查、动态范围。
- 自动扳机：自动扳机开关、阈值、延迟、距离限制。
- 目标选择：优先锁定模式、跳过倒地、击杀切换、空弹不锁、开镜检查。
- 弹道相关：弹道预判、显示预瞄、显示 FOV、显示锁胎 FOV。
- 车辆轮胎：瞄准车胎、锁胎 FOV、瞄准车胎平滑。
- 后座控制：后座抑制、原始弹道、保持压枪、全局压枪、压枪 Y 轴参数。
- 自瞄参数：最大距离、范围/FOV、X/Y 灵敏度、X/Y 平滑度、吸附强度、平滑比例、最大帧率、切换延迟。
- 部位选择：第一目标、第二目标、倒地目标独立骨骼点配置。
- 控制器配置：本机 / 外部设备控制模式、COM 端口连接、测试移动、配置保存。
- 武器设置：按倍镜配置压枪幅度和压枪延迟，支持不同武器/倍镜参数。

### 2. ESP / 玩家显示

- 玩家绘制：方框、骨骼、头部骨骼、血条、距离、名称、队伍、武器等显示项。
- 状态信息：玩家击杀、伤害、观战数、KDA、段位图标、段位数据。
- 视觉辅助：被瞄射线、漏哪变色、ESP 预览、血条位置和样式选择。
- 玩家列表：独立玩家列表窗口，可按名单类型、玩家类型和战绩数据过滤。

### 3. 物资 / Item ESP

- 物品透视：物品显示、物品叠加、配件过滤、图标显示。
- 距离和样式：物品显示距离、字体/图标比例、颜色设置。
- 智能过滤：医疗、投掷物、弹药、配件等按数量阈值过滤。
- 物资分组：支持武器、配件、弹药、药品、投掷物、特殊物品等分类选择。
- 空投：空投显示、空投物品显示、距离和字体大小设置。
- 盒子：死亡盒子显示、盒内物品显示、距离和字体大小设置。

### 4. 载具 / Vehicle

- 载具显示开关。
- 载具距离限制和字体大小。
- 载具血量、油量显示。
- 内置载具实体表，覆盖飞机、船、摩托、轿车、吉普、装甲车、自行车、滑翔机等类型。

### 5. 雷达 / Radar

- 大地图雷达：玩家、载具、空投、死亡盒显示；地图比例和图标比例调整。
- 小地图雷达：玩家、载具、空投、死亡盒显示；地图比例和图标比例调整。
- 网页雷达：菜单内保留浏览器打开入口。
- 预警雷达：启动预警、距离显示、最大距离、图标比例。

### 6. 投掷物 / 预警

- 手雷爆炸提示。
- 爆炸范围显示。
- 高抛预判。
- 文本计时、掐雷计时、血条计时。
- 字体大小调整。

### 7. 软件 / Overlay 设置

- 菜单主题和背景样式。
- 机器人/目标颜色设置。
- 指向模式。
- 垂直同步。
- 备用相机。
- 独立线程。
- 过滤迷雾。
- 融合模式。

## 框架结构

```text
Skeet.sln / Skeet.vcxproj
├─ Main.cpp / Main.h                 程序入口、主循环和初始化
├─ Source/Common                     通用数据结构、偏移、实体、数学、配置
│  ├─ Offset.h                       全局 offset 常量和 GameData.Offset 初始化
│  ├─ Data.h                         全局 GameData、配置结构、运行时数据
│  ├─ Entitys.*                      物品、载具、空投、盒子等实体表
│  ├─ VectorHelper.*                 坐标转换、角度和屏幕投影辅助
│  └─ VisibleScene.h                 可视检测/物理场景相关封装
├─ Source/Hack                       游戏数据读取、玩家/物资/载具处理逻辑
│  ├─ Decrypt.*                      Xenuine / CIndex 解密逻辑
│  ├─ Process.*                      游戏进程、世界对象、核心数据更新
│  ├─ Players.h / Actors.h           玩家和 Actor 解析
│  ├─ Items.h / Vehicles.h           物资和载具解析
│  ├─ Radar.h / VisibleCheck.h       雷达和可视检查
│  └─ MeshPatcher.h / LineTrace.h    骨骼、射线和物理辅助
├─ Source/Overlay                    DirectX11 + ImGui 覆盖层和菜单
│  ├─ Overlay.cpp                    Overlay 渲染、贴图加载、主绘制流程
│  ├─ Menu.h                         主菜单 UI
│  ├─ MenuPlayerLists.h              玩家列表 UI
│  ├─ Texture.*                      图标和贴图资源
│  └─ GlobalConfig.h                 Overlay 相关全局配置
├─ Source/Utils                      输入设备、盒子、网络/串口、计时等工具
│  ├─ InputManager.*                 输入管理
│  ├─ KmBox.* / MoBox.* / Lurker.*   外部控制设备适配
│  └─ Timer.* / Throttler.*          计时和节流工具
├─ Include/DMALibrary                DMA / VMM / LeechCore 相关封装
├─ Include/imgui                     ImGui UI 框架
├─ Include/sdk                       DirectX SDK 头文件和库
├─ lib                               embree / tbb / zstd 等依赖库
└─ Assets                            图标、物品、载具、武器等资源
```

## 构建方式

```powershell
D:\VS\Community\MSBuild\Current\Bin\MSBuild.exe D:\AMD\Skeet\Skeet.sln /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m /v:minimal /nologo
```

输出目录：

```text
D:\AMD\Skeet\x64\Skeet\
```

主要产物：

```text
Skeet.exe
Skeet.pdb
Skeet.lib
```

## 依赖说明

运行目录需要随 `Skeet.exe` 放置的主要 DLL：

```text
ddll64.dll
embree4.dll
freetype.dll
FTD3XX.dll
leechcore.dll
tbb12.dll
vmm.dll
VMProtectSDK64.dll
zstd.dll
```

当前工程已移除 MQTT / Mosquitto 依赖，代码和工程文件不再引用 `mosquitto.lib`、`mosquitto.dll` 或 `mqtt` 目录。

## 配置与日志

- 默认配置文件：`Config/SkeetConfig.bak`。
- 兼容旧配置：启动时如果存在 `Config/AKMConfig.bak` 会自动读取，并迁移保存为 `Config/SkeetConfig.bak`。
- 运行日志：`Config/Skeet.log`，记录配置加载/保存、线程启动、关键异常等低频事件。
- 高频渲染/射线/PhysX 异常日志已做限流，避免日志 IO 影响 DMA 读取与渲染帧率。
- Overlay 默认启用独立线程模式，以降低渲染和数据读取互相阻塞的概率。
- 菜单关键性能选项带 `(?)` 提示，鼠标悬停即可查看用途，方便快速理解和操作。

## Offset 更新

Offset 集中维护在：

```text
Source/Common/Offset.h
```

新的 offset 合入后会同步到 `Offset::C` 常量，并通过 `Offset::Init()` 写入 `GameData.Offset[...]`，以保持旧代码的 map 访问方式不变。
