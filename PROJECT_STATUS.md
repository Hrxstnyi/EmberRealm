# 项目状态 - 烬域：卡拉比亚战区

**最后更新**: 2026-08-19  
**当前阶段**: Phase 1 核心系统完成（框架+经济+战斗+单位）  
**版本**: 0.2.0 (Pre-Alpha)

---

## 已完成

### Phase 0 - 项目初始化 ✅
- [x] UE5.5 C++ 项目骨架 (.uproject, Build.cs, Target.cs)
- [x] 模块化目录结构 (7大模块)
- [x] 标准配置文件 (DefaultEngine/Game/Input)
- [x] .gitignore (UE5标准)
- [x] README.md
- [x] Git 仓库初始化
- [x] GitHub 远端仓库: https://github.com/Hrxstnyi/EmberRealm
- [x] Python 游戏启动器 (深色主题、存档管理、设置)

### Phase 1 - 核心系统 ✅
- [x] **事件总线** (Core/EventBus)
- [x] **游戏时钟** (Core/GameClock) - 三阶时间流速
- [x] **游戏模式** (Core/ERGameModeBase) - 回合管理
- [x] **游戏实例** (Core/ERGameInstance) - 战略地图+经济系统+回合结算
- [x] **战略地图管理器** (StrategyLayer/StrategyMapManager)
- [x] **经济系统** (StrategyLayer/EconomySystem) - 6种资源、建造队列
- [x] **战斗结算器** (StrategyLayer/CombatResolver) - 兵种克制、地形、士气
- [x] **单位Actor** (StrategyLayer/UnitActor) - 可移动可攻击的部队
- [x] **数据蓝图库** (DataSystem/ERDataLibrary) - 17种兵种+3大阵营默认数值
- [x] **兵种数据资产** (DataSystem/ERUnitData)
- [x] **存档管理器** (SaveSystem/SaveManager)
- [x] **FPS角色基类** (FPSLayer/FPSCharacterBase)
- [x] **阵营数据资产** (DataSystem/ERFactionData)
- [x] **阵地数据结构** (StrategyLayer/StrategyTile)

### 外部 Skills 集成 ✅
- [x] skill-creator, frontend-design, code-simplifier
- [x] document-skills, find-skills, ralph-loop

### 文档 ✅
- [x] 游戏设计文档 (GDD.md)
- [x] 技术设计文档 (TDD.md)
- [x] 桌游规则审查 (BoardGame_Review.md)
- [x] 项目级AI指令 (AGENTS.md)
- [x] Skill使用指南 (SKILLS_USAGE.md)
- [x] PVP匹配设计 (PVP_MATCHMAKING_DESIGN.md) - 特遣队介入、异步任务、残局保护

---

## 代码统计
- 源文件: 30+
- 代码行数: ~3000行C++
- 文档: 6份

---

## 下一步（你要做的）

1. **在本地UE5.5中打开 `EmberRealm.uproject`，编译验证**（最关键）
2. 编译通过后，在编辑器中调用 `ERDataLibrary.CreateDefaultUnitData()` 生成默认兵种
3. 创建测试地图，放置UnitActor验证移动逻辑
4. 告诉我编译结果，我继续写3D地图渲染和UI

---

## 已知问题
- EventBus的Subscribe模板方法尚未完整实现
- 存档系统的数据序列化尚未对接游戏状态
- FPS角色的武器组件尚未创建
- 缺少Content目录下的实际资源（地图、模型、材质）
- UnitActor的AttackTile需要集成CombatResolver
- 经济系统的BaseIncome和UpkeepCost需要外部系统计算后传入
