# 技术设计文档 (TDD)
## 烬域：卡拉比亚战区

**版本**: 0.1.0  
**引擎**: Unreal Engine 5.5+  
**语言**: C++ & Blueprints

---

## 1. 技术架构

### 1.1 模块划分

```
EmberRealm (主模块)
├── Core/              # 核心系统
│   ├── GameClock      # 三阶时间时钟
│   ├── GameMode       # 回合管理
│   ├── GameInstance   # 全局数据
│   └── EventBus       # 全局事件总线
├── StrategyLayer/     # 战略层
│   ├── StrategyTile   # 阵地数据
│   ├── StrategyMap    # 地图管理
│   ├── UnitManager    # 部队管理
│   ├── EconomySystem  # 经济系统
│   ├── TechTree       # 科技树
│   └── Diplomacy      # 外交系统
├── FPSLayer/          # FPS战术层
│   ├── FPSCharacter   # 玩家角色
│   ├── WeaponSystem   # 武器系统
│   ├── EnemyAI        # 敌人AI
│   ├── MissionSystem  # 任务系统
│   └── DamageSystem   # 部位伤害
├── DataSystem/        # 数据驱动
│   ├── FactionData    # 阵营配置
│   ├── UnitData       # 兵种配置
│   ├── WeaponData     # 武器配置
│   └── TileData       # 阵地配置
├── SaveSystem/        # 存档
│   ├── SaveGame       # 存档对象
│   └── SaveManager    # 存档管理
├── AISystem/          # AI
│   ├── FactionAI      # 阵营战略AI
│   ├── OfficerAI      # 军官行为树
│   └── SaboteurAI     # 内鬼行为
└── Network/           # 网络（后期）
    ├── NetSync        # 回合同步
    └── Matchmaking    # 匹配系统
```

### 1.2 核心设计模式
- **数据驱动**: 所有游戏配置用UDataAsset，运行时加载
- **事件驱动**: 全局事件总线解耦模块间通信
- **组件化**: 武器、伤害、感知等用ActorComponent
- **状态机**: 游戏状态、AI状态、战斗状态

---

## 2. 核心系统设计

### 2.1 游戏时钟 (GameClock)
```cpp
enum class ETimeScale : uint8
{
    Quarterly,  // 90天/回合
    Weekly,     // 7天/回合
    FourHour,   // 4小时/回合
    Paused
};

class FGameClock
{
    void AdvanceTurn();
    void SetTimeScale(ETimeScale NewScale);
    void EvaluateTimeScaleChange(bool bCombat, bool bCampaign);
private:
    ETimeScale CurrentTimeScale;
    int32 TotalDaysElapsed;
    int32 ConsecutivePeaceTurns;
};
```

### 2.2 回合流程
1. 时序状态判定更新
2. 全局资源结算（税收、开采、维护）
3. 行动点回复
4. 外交阶段
5. 建设运营阶段
6. 侦察部署、视野结算
7. 宣战、行军、阵地抢占
8. 战役结算（自动/FPS）
9. 成长结算（升勋、名誉、军衔）
10. 事件触发
11. 胜利条件检测

### 2.3 双视角切换
- 战略层 → FPS: 战役阶段触发，加载FPS关卡，传递任务数据
- FPS → 战略层: 任务完成/超时，回写结果，恢复战略层
- 切换时战略层暂停（单机）或异步（多人）

### 2.4 存档系统
- UERSaveGame继承USaveGame
- 战略层数据二进制序列化
- 支持多存档位、自动存档、云存档预留
- 版本号检查，不兼容时提示

---

## 3. 网络架构（后期）

### 3.1 同时回合制同步
- 所有玩家同时操作，回合结束时统一结算
- 用Server Authority架构
- 操作指令缓存，回合结束时批量同步

### 3.2 FPS异步
- 玩家进入FPS时，战略层AI托管
- 其他玩家继续战略操作
- FPS结果回写时同步给所有客户端

---

## 4. 性能优化

### 4.1 战略层
- 地图用World Partition分区
- 单位用Instanced Static Mesh
- LOD系统，远处简化

### 4.2 FPS层
- Nanite高精度模型
- Lumen动态光照
- 对象池管理敌人和子弹
- 视锥体剔除

### 4.3 内存
- 数据资产按需加载
- 关卡流送
- 纹理压缩

---

## 5. 工具链

- **版本控制**: Git + Git LFS
- **项目管理**: GitHub Issues / Projects
- **CI/CD**: GitHub Actions（后期自动构建）
- **美术资源**: UE5 Marketplace + 程序化生成
- **音效**: Wwise 或 UE内置音频

---

## 6. 编码规范

- C++遵循UE5编码规范
- 类名前缀: A(Actor), U(Object), F(Struct), E(Enum), I(Interface)
- 头文件保护: #pragma once
- 指针优先用TUniquePtr/TSharedPtr
- 容器用TArray/TMap/TSet
- 日志用UE_LOG
- 蓝图可调用函数标记UFUNCTION(BlueprintCallable)
- 蓝图可读属性标记UPROPERTY(BlueprintReadOnly)
