# AGENTS.md — 烬域：卡拉比亚战区

本文件为在本项目中工作的 AI 代理提供项目级指令。

## 项目概述

《烬域：卡拉比亚战区》是一款回合制4X策略 + FPS精英小队双视角的现代战争游戏。
- 引擎：Unreal Engine 5.5
- 语言：C++（核心）+ Blueprints（UI/交互）
- 启动器：Python 3.8+ (Tkinter)

## 已安装 Skills

本项目关联以下 skills，开发时应优先使用：

| Skill | 用途 | 触发场景 |
|-------|------|----------|
| **skill-creator** | 创建/改进项目专用 skill | 需要为重复任务创建自动化 skill 时 |
| **frontend-design** | UI/UX 设计 | 设计启动器界面、游戏菜单、HUD 时 |
| **code-simplifier** | 代码简化与重构 | 写完 C++/Python 代码后自动简化，保持可读性 |
| **document-skills** | 文档处理 | 生成设计文档、导出 PDF/Word、处理表格时 |
| **find-skills** | 搜索更多 skill | 需要新能力但不确定是否有对应 skill 时 |
| **ralph-loop** | 迭代开发循环 | 需要反复迭代直到通过测试/完成标准的任务 |

## 编码规范

- C++ 遵循 UE5 编码规范，类名前缀 A/U/F/E/I
- 头文件使用 `#pragma once`
- 优先使用 TArray/TMap/TSet 而非 STL 容器
- 日志使用 UE_LOG
- 蓝图可调用函数标记 UFUNCTION(BlueprintCallable)

## 模块结构

```
Source/EmberRealm/
├── Core/              # 事件总线、游戏时钟、回合管理
├── StrategyLayer/     # 战略地图、经济、军事
├── FPSLayer/          # FPS角色、武器、敌人AI
├── DataSystem/        # 阵营、兵种、武器数据资产
├── SaveSystem/        # 存档管理
├── AISystem/          # 阵营AI、军官AI、内鬼
└── Network/           # 多人联机（后期）
```

## Git 工作流

- 分支：main（稳定）→ dev（开发）→ feature/*
- 提交信息格式：`type(scope): 描述`
- type: feat/fix/docs/refactor/style/test/chore

## 注意事项

- 不要提交 Binaries/、Intermediate/、Saved/ 目录（已在 .gitignore）
- 大型资源文件使用 Git LFS
- 修改核心系统前先阅读对应模块的头文件
