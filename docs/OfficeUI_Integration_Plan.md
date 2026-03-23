# MingGoRTS 办公界面集成方案

## Star-Office-UI 研究总结

**项目定位**: 像素风格办公界面，将AI Agent工作状态可视化为温馨的办公空间
**技术栈**: Flask后端 + HTML/JS前端，MIT协议开源
**核心功能**:
- 6种状态映射（idle/writing/researching/executing/syncing/error）
- 多Agent协作空间
- 昨日工作记录展示
- 中英日三语支持
- AI生成背景图
- 移动端适配

## 集成架构设计

### 方案一：独立服务模式（推荐）
```
MingGoRTS (UE5)  <--->  Star-Office-UI (Flask:19000)
     |                        |
     | 状态同步API            | 像素办公界面
     |                        |
 游戏状态更新            Web/桌面宠物展示
```

### 方案二：嵌入式组件模式
将办公界面作为UE5的WebUI组件嵌入游戏

## 状态映射设计

| 游戏状态 | 办公状态 | 视觉表现 |
|---------|---------|---------|
| 菜单/待机 | idle | 角色休息 |
| 战略规划 | researching | 角色研究 |
| 战斗执行 | executing | 角色忙碌 |
| 资源管理 | writing | 角色记录 |
| 多人在线 | syncing | 多人协作场景 |
| 错误/崩溃 | error | 角色困惑 |

## API集成点

### MingGoRTS → Star-Office-UI
```cpp
// 游戏状态更新时调用
void UMingOfficeBridge::UpdateOfficeState(EGameState NewState, const FString& Description)
{
    // POST /set_state
    // 同步到办公界面
}
```

### 所需文件结构
```
MingGoRTS/
├── Tools/
│   └── office-ui/              # Star-Office-UI 集成目录
│       ├── backend/            # Flask后端
│       ├── frontend/           # 前端资源
│       └── ming-bridge/        # MingGoRTS专用桥接
├── Source/
│   └── MingGoRTS/
│       └── Private/
│           └── OfficeUI/
│               ├── MingOfficeBridge.cpp
│               └── MingOfficeStateSync.cpp
└── Config/
    └── office-ui.json          # 办公界面配置
```

## 启动流程

1. 启动Star-Office-UI后端 (python app.py)
2. 启动MingGoRTS游戏
3. 桥接模块自动连接并同步状态
4. 浏览器访问 http://127.0.0.1:19000 查看办公界面

## 扩展功能

- 将RTS单位显示为办公室"访客"
- 战斗胜利/失败作为办公室事件
- 资源收集进度可视化
- 多玩家办公协作空间
