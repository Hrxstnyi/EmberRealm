# 烬域：卡拉比亚战区 (EmberRealm: Calabia War Zone)

> 回合制4X策略 + FPS精英小队双视角现代战争游戏  
> Unreal Engine 5.5+ | C++ & Blueprints

---

## 游戏概述

《烬域：卡拉比亚战区》是一款融合了大战略与第一人称战术的现代战争游戏。玩家在战略层扮演阵营指挥官，管理经济、科技、外交与军事部署；在关键时刻可切换至第一人称视角，亲自操控精英小队执行特种作战任务，其结果将直接影响战略战局。

### 核心特色

- **三阶时间流速系统**：和平时期以季度为单位，战时以周为单位，战役以四小时为单位，时间随战局动态切换
- **双视角联动**：战略层4X策略 ↔ FPS精英小队特种行动，战术成果实时回写战略层
- **AI军官内鬼系统**：内鬼从AI军官中随机刷新，通过战报异常察觉，军法处置需承担误杀风险
- **多元发展路线**：军事征服、经济霸权、科技领先、谍报渗透、外交纵横、民心控制
- **硬核真实FPS**：塔科夫式弹道物理、部位伤害、弹药管理、极简UI，还原战争残酷感
- **分区指挥多人**：同阵营2-3名玩家分区指挥，共享资源但部队独立

### 阵营

| 阵营 | 风格 | 特色 |
|------|------|------|
| **戍卫军** | 正规军 | 阵地防守、后勤强势、稳扎稳打 |
| **合约团** | 雇佣武装 | 高速机动、突袭战术、外交博弈 |
| **自治同盟** | 地方武装 | 地形游击、隐蔽作战、持续消耗 |

---

## 技术架构

### 模块划分

```
EmberRealm/
├── Core/              # 核心：游戏时钟、回合管理、事件系统
├── StrategyLayer/     # 战略层：地图、经济、科技、外交、军事指挥
├── FPSLayer/          # FPS层：武器系统、AI敌人、任务系统、物理
├── DataSystem/        # 数据：阵营、兵种、配置表（DataAsset驱动）
├── SaveSystem/        # 存档：序列化、多存档位
├── AISystem/          # AI：敌方阵营AI、军官AI、内鬼行为
└── Network/           # 网络：同时回合同步、PVP联机
```

### 技术栈

- **引擎**：Unreal Engine 5.5+（Nanite, Lumen, World Partition, Enhanced Input）
- **语言**：C++（核心系统）+ Blueprints（UI、交互、关卡事件）
- **版本控制**：Git + Git LFS
- **目标平台**：PC (Windows)，后续考虑主机

---

## 开发路线图

| 阶段 | 内容 | 状态 |
|------|------|------|
| Phase 0 | 项目初始化、Git仓库、基础框架 | 🔄 进行中 |
| Phase 1 | 战略层核心：3D地图、回合系统、三阶时间、经济、基本军事 | ⏳ 待开始 |
| Phase 2 | FPS层原型：关卡、武器、敌人、移动射击 | ⏳ 待开始 |
| Phase 3 | 双视角联动：FPS任务生成、结果回写 | ⏳ 待开始 |
| Phase 4 | AI对手 + PVE完整循环 | ⏳ 待开始 |
| Phase 5 | 内鬼系统、科技树、名誉军衔 | ⏳ 待开始 |
| Phase 6 | PVP联机、多人匹配、残局接管 | ⏳ 待开始 |
| Phase 7 | 历史CG、音乐、美术polish、Steam EA | ⏳ 待开始 |

---

## 快速开始

### 环境要求

- Unreal Engine 5.5 或更高版本
- Visual Studio 2022 (Windows) / Xcode (Mac) / Rider
- Git LFS

### 克隆与构建

```bash
git clone <repository-url>
cd EmberRealm
# 右键 EmberRealm.uproject -> Generate Visual Studio project files
# 打开 EmberRealm.sln，编译 Development Editor
# 或在UE5编辑器中直接打开 EmberRealm.uproject
```

---

## 设计文档

详细设计文档见 `Documentation/` 目录：

- [游戏设计文档 (GDD)](Documentation/GDD.md)
- [技术设计文档 (TDD)](Documentation/TDD.md)
- [桌游规则审查与完善](Documentation/BoardGame_Review.md)

---

## 许可证

本项目为个人独立开发项目，保留所有权利。未经授权不得商用。

---

*"在烬域之中，每一个决策都可能改变战争的走向。"*
