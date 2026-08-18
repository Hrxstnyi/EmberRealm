# 项目状态 - 烬域：卡拉比亚战区

**最后更新**: 2026-08-19  
**当前阶段**: Phase 0 完成 + Phase 1 核心框架搭建  
**版本**: 0.1.0 (Pre-Alpha)

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

### Phase 1 - 核心系统框架 ✅ (框架级)
- [x] **事件总线** (Core/EventBus) - 订阅/发布模式，20+常用事件常量
- [x] **游戏时钟** (Core/GameClock) - 三阶时间流速（季度/周/四小时），自动切换逻辑
- [x] **游戏模式** (Core/ERGameModeBase) - 回合管理，回合开始/结束流程
- [x] **游戏实例** (Core/ERGameInstance) - 全局数据，事件总线生命周期
- [x] **战略地图管理器** (StrategyLayer/StrategyMapManager) - 阵地管理、控制权变更、收入计算、行军消耗
- [x] **兵种数据资产** (DataSystem/ERUnitData) - 17种兵种、军衔体系、勋级技能
- [x] **存档管理器** (SaveSystem/SaveManager) - 多槽位存档/加载/删除
- [x] **FPS角色基类** (FPSLayer/FPSCharacterBase) - 移动、射击、瞄准基础框架
- [x] **阵营数据资产** (DataSystem/ERFactionData) - 三大阵营配置
- [x] **阵地数据结构** (StrategyLayer/StrategyTile) - 8种地形、海拔、经济属性

### 文档 ✅
- [x] 游戏设计文档 (GDD.md) - 完整玩法设计
- [x] 技术设计文档 (TDD.md) - 架构、模块、编码规范
- [x] 桌游规则审查 (BoardGame_Review.md) - 10处漏洞+完善建议

---

## 进行中

- [ ] 战略层可玩原型 (地图渲染、单位移动、经济结算)
- [ ] 战斗结算系统
- [ ] AI对手基础逻辑

---

## 待开始

### Phase 1 剩余
- [ ] 3D地图渲染 (World Partition)
- [ ] 单位Actor与移动
- [ ] 经济系统完整实现
- [ ] 科技树框架
- [ ] 自动战斗结算

### Phase 2 - FPS原型
- [ ] FPS关卡 (一个测试关卡)
- [ ] 武器系统 (一种武器)
- [ ] 敌人AI (基础巡逻/战斗)
- [ ] 双视角切换

### Phase 3 - 联动
- [ ] FPS任务生成系统
- [ ] 战果回写战略层

### Phase 4+
- [ ] AI完整逻辑
- [ ] 内鬼系统
- [ ] PVP联机
- [ ] 历史时刻CG
- [ ] Steam EA

---

## 技术栈

- **引擎**: Unreal Engine 5.5
- **语言**: C++ (核心) + Blueprints (UI/交互)
- **版本控制**: Git + GitHub
- **启动器**: Python 3.8+ (Tkinter)
- **目标平台**: PC (Windows)

---

## 代码统计

- 源文件: 20+
- 代码行数: ~2500行
- 文档: 3份，~5000字
- Git提交: 6次

---

## 下一步建议

1. 在本地UE5.5中打开 `EmberRealm.uproject`，编译验证
2. 创建测试地图，验证GameMode和GameClock
3. 实现战略地图的3D渲染
4. 实现单位移动和基本经济循环

---

## 已知问题

- EventBus的Subscribe模板方法尚未完整实现（当前仅支持单参数委托）
- 存档系统的数据序列化尚未对接游戏状态
- FPS角色的武器组件(UERWeaponComponent)尚未创建
- 缺少Content目录下的实际资源（地图、模型、材质）
