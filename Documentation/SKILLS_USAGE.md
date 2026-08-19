# Skills 使用指南 — 烬域项目

本项目已安装 6 个外部 skills，本文档说明每个 skill 在项目中的具体应用场景。

---

## 1. skill-creator

**用途**：创建、修改和优化项目专用 skills。

**项目应用场景**：
- 为 UE5 C++ 代码生成创建专用 skill（如"自动生成 UDataAsset 类"）
- 为关卡设计流程创建 skill
- 优化现有 skill 的触发描述

**使用示例**：
```
使用 skill-creator 创建一个"UE5兵种数据资产生成器"skill，
输入兵种名称和属性，自动生成 UERUnitData 子类和配置。
```

---

## 2. frontend-design

**用途**： distinctive UI/UX 设计，避免模板化默认样式。

**项目应用场景**：
- 设计 Python 启动器的界面风格（当前深色军事主题可进一步优化）
- 设计游戏主菜单、设置界面、阵营选择界面
- 设计战略层 HUD（时间流速指示器、资源面板、小地图）
- 设计 FPS 层极简 HUD（准星、血量、弹药）

**使用示例**：
```
使用 frontend-design 重新设计启动器界面，
要求：军事指挥中心风格，橙色(#ff6b35)强调色，
极简但有辨识度，不要模板化。
```

---

## 3. code-simplifier

**用途**：在不改变功能的前提下简化代码，提升清晰度和可维护性。

**项目应用场景**：
- 每次写完 C++ 类后，自动检查并简化
- 重构复杂的嵌套逻辑（如战略地图的寻路算法）
- 统一代码风格
- 减少不必要的抽象和冗余

**使用示例**：
```
使用 code-simplifier 检查 Source/EmberRealm/StrategyLayer/StrategyMapManager.cpp，
简化其中的 CalculateTotalIncome 和 CanMoveTo 方法。
```

**当前项目代码审查结果**：
- `StrategyMapManager.cpp` — 已简化，提取3个辅助函数
- `GameClock.cpp` — 状态机逻辑清晰，无需大改
- `SaveManager.cpp` — 错误处理完整，可简化重复的槽位检查逻辑

---

## 4. document-skills

**用途**：Word/PDF/PPT/Excel 文档的创建、编辑、分析。

**项目应用场景**：
- 将 GDD.md 导出为 Word/PDF 供团队审阅
- 生成设计评审 PPT
- 用 Excel 做兵种数值平衡表
- 处理用户上传的桌游规则书（已完成审查）

**使用示例**：
```
使用 document-skills 将 Documentation/GDD.md 转换为格式精美的 PDF，
包含目录、页码、标题层级。
```

---

## 5. find-skills

**用途**：搜索和发现更多可用的 agent skills。

**项目应用场景**：
- 需要 UE5 专用 skill 时搜索
- 需要音频/视频生成 skill 时搜索
- 需要 3D 模型处理 skill 时搜索
- 不确定是否有对应 skill 时先搜索

**使用示例**：
```
使用 find-skills 搜索"Unreal Engine"相关的 skills，
看看有没有能加速 UE5 开发的工具。
```

---

## 6. ralph-loop

**用途**：迭代式自引用开发循环，AI 反复执行同一任务直到完成标准。

**项目应用场景**：
- 让 AI 反复迭代直到单元测试全部通过
- 从零构建一个完整系统（如经济系统），AI 自主迭代完善
- 代码重构 + 编译验证循环
- 数值平衡调参（反复运行模拟直到平衡）

**使用示例**：
```
使用 ralph-loop 实现战略层经济系统：
1. 实现 EconomySystem 类
2. 编写单元测试
3. 运行测试
4. 修复失败
5. 重复直到所有测试通过
完成标准：所有测试通过 + 代码编译无警告
--max-iterations 30
```

**注意**：ralph-loop 需要配置 stop hook，运行 `bash scripts/setup-ralph-loop.sh` 安装。

---

## Skill 组合使用示例

### 场景：设计并实现新 UI 界面
1. `frontend-design` — 设计界面风格和布局
2. `skill-creator` — 为重复的 UI 创建流程创建专用 skill
3. `code-simplifier` — 简化实现后的 UI 代码

### 场景：生成设计文档并分享
1. `document-skills` — 将 Markdown 转为 Word/PDF
2. `frontend-design` — 设计文档封面和排版风格

### 场景：迭代开发新系统
1. `ralph-loop` — 反复迭代实现直到测试通过
2. `code-simplifier` — 最终简化代码
3. `document-skills` — 生成技术文档
