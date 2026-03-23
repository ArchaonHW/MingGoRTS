# MingGoRTS Game Architecture

## 后端与 UI/UX 分离架构 (Backend-Frontend Separation)

```
Game/
├── Core/                    # 后端核心系统
│   ├── Engine/             # 游戏引擎核心
│   ├── Campaign/           # 战役层（回合制策略）
│   ├── Battle/             # 战斗层（即时战术）
│   ├── Faction/            # 派系系统
│   ├── Unit/               # 单位系统
│   ├── Politics/           # 政治系统
│   ├── Economy/            # 经济系统
│   └── Tech/               # 科技树系统
├── Interface/              # UI/UX 接口层
│   ├── API/                # 后端暴露的 API
│   ├── Events/             # 事件系统
│   └── Protocol/           # 通信协议
├── Frontend/               # 前端实现（可选替换）
│   ├── Console/            # 控制台界面
│   └── GUI/                # 图形界面（预留）
└── Main.cpp                # 程序入口
```

## 核心设计原则

1. **Backend-Frontend 分离**: 后端纯逻辑，通过 Interface 层暴露 API
2. **Total War 模式**: 战役层（回合制）+ 战斗层（即时战术）
3. **派系系统**: 文化分组 + 独特机制
4. **事件驱动**: 后端通过事件通知前端更新

## Total War 机制映射

| Total War 机制 | MingGoRTS 实现 |
|---------------|----------------|
| Grand Campaign | CampaignLayer |
| Real-time Battles | BattleLayer |
| Faction Cultures | FactionSystem |
| Politics (Gravitas) | PoliticsSystem |
| Tech Trees | TechTreeSystem |
| Units & Armies | UnitSystem |
| Trade Routes | EconomySystem |

## 通信模式

```cpp
Backend (Core)  <--->  Interface Layer  <--->  Frontend (UI/UX)
     |                      |                     |
     v                      v                     v
[Game Logic]          [API/Events]          [Rendering]
```
