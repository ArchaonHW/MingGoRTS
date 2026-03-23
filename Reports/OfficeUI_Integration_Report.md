# MingGoRTS Office UI 集成报告

## 研究总结 - Star-Office-UI

**项目地址**: https://github.com/ringhyacinth/Star-Office-UI
**技术栈**: Flask + HTML/JS 像素风格办公界面
**核心功能**:
- 6种工作状态可视化 (idle/writing/researching/executing/syncing/error)
- 多Agent协作空间
- 昨日工作记录展示
- AI生成背景图
- 移动端适配
- 中英日三语支持

## 已创建的文件

### 1. 集成方案文档
`docs/OfficeUI_Integration_Plan.md`
- 架构设计方案
- 状态映射表
- API接口定义

### 2. C++桥接系统
`Source/MingGoRTS/Public/OfficeUI/MingOfficeBridge.h`
`Source/MingGoRTS/Private/OfficeUI/MingOfficeBridge.cpp`
- UE5与办公界面的HTTP通信
- 游戏状态自动同步
- 6种办公状态枚举

### 3. 启动脚本
`Tools/workflow/Start-MingOfficeUI.ps1`
- 自动安装依赖
- 启动Flask服务
- 状态检查功能

## 文件结构
```
MingGoRTS/
├── Tools/office-ui/
│   ├── backend/app.py          # Flask服务
│   ├── config/state.json       # 状态文件
│   └── frontend/index.html     # 像素办公界面
├── Source/MingGoRTS/
│   ├── Public/OfficeUI/
│   │   └── MingOfficeBridge.h  # UE5桥接头文件
│   └── Private/OfficeUI/
│       └── MingOfficeBridge.cpp # UE5桥接实现
└── docs/OfficeUI_Integration_Plan.md
```

## 手动启动步骤

1. 安装Python依赖:
   ```bash
   pip install Flask==2.3.3 Flask-CORS==4.0.0
   ```

2. 启动Flask服务:
   ```bash
   cd Tools/office-ui/backend
   python app.py
   ```

3. 访问办公界面:
   ```
   http://127.0.0.1:19000
   ```

## 状态映射

| 游戏状态 | 办公状态 | 图标 |
|---------|---------|------|
| 菜单/待机 | idle | 😴 |
| 战略规划 | researching | 🔍 |
| 战斗执行 | executing | ⚔️ |
| 资源管理 | writing | 📝 |
| 多人游戏 | syncing | 🔄 |
| 错误状态 | error | ❌ |

## API端点

- GET /health - 健康检查
- GET /status - 获取当前状态
- POST /set_state - 设置新状态

## 下一步建议

1. 完善Flask后端代码
2. 创建像素风格前端界面
3. 在UE5中测试桥接系统
4. 添加更多游戏状态映射

## 参考链接

- Star-Office-UI GitHub: https://github.com/ringhyacinth/Star-Office-UI
- Flask文档: https://flask.palletsprojects.com/
- UE5 HTTP模块: https://docs.unrealengine.com/

---
报告生成时间: 2026-03-24
集成状态: 设计完成，等待实现
