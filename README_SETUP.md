# MingWarRTS 项目设置指南

## 🎯 **快速开始**

### **1. 启动项目**

**直接双击** `MingWarRTS.uproject` 文件。

UE5 Editor 会自动：
- 检测 C++ 代码
- 生成项目文件
- 编译模块

### **2. 首次编译时间**

- **首次**: 10-30 分钟（取决于电脑性能）
- **后续**: 1-5 分钟

### **3. 编译成功标志**

- ✅ 无红色错误弹窗
- ✅ 右下角进度条完成
- ✅ 可以创建 Blueprint
- ✅ 生成 `MingWarRTS.sln` 文件

---

## 🛠️ **如果编译失败**

### **最简单解决方案**

1. 打开 UE5 Editor 后，点击 **"Fix"** 按钮
2. 等待 5-10 分钟自动修复

### **如果 "Fix" 按钮无效**

1. 关闭 UE5 Editor
2. **删除** 以下文件夹（如果存在）：
   - `Intermediate`
   - `Saved`
   - `Binaries`
3. 重新双击 `MingWarRTS.uproject`

### **高级方案：手动生成项目文件**

```cmd
cd C:\HW\MingGoRTS
"C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\Build.bat" MingWarRTS Win64 Development -Project="MingWarRTS.uproject"
```

---

## 📋 **项目结构**

```
C:\HW\MingGoRTS\
├── MingWarRTS.uproject          (UE5 项目文件)
├── Source\MingWarRTS\
│   ├── Public\
│   │   ├── MingWarRTS.h         (GameMode)
│   │   ├── RTSUnit.h            (单位类)
│   │   ├── RTSBuilding.h        (建筑类)
│   │   ├── RTSPlayerController.h (控制器)
│   │   └── ResourceManager.h    (资源管理)
│   ├── Private\
│   │   ├── MingWarRTS.cpp       (GameMode 实现)
│   │   ├── RTSUnit.cpp          (单位实现)
│   │   ├── RTSBuilding.cpp      (建筑实现)
│   │   ├── RTSPlayerController.cpp (控制器实现)
│   │   └── ResourceManager.cpp  (资源管理实现)
│   └── MingWarRTS.Build.cs      (编译配置)
└── Config\ (配置文件)
```

---

## 🎮 **编译成功后下一步**

1. **创建 Blueprint**
   - Content Browser → 右键 → Blueprint Class
   - 搜索 `MingWarRTS` → 选择它
   - 命名为 `BP_MingWarGameMode`
   - 保存到 `Content/Blueprints/Game/`

2. **创建地图**
   - File → New Level
   - 添加地面
   - 保存为 `Content/Maps/TestMap/TestMap.umap`

3. **设置 GameMode**
   - Window → World Settings
   - GameMode Override → `BP_MingWarGameMode`

4. **测试 Play!**

---

## ⚠️ **常见问题处理**

### **Q: "Could not be compiled" 错误**
A: 点击 "Fix" 按钮，UE5 会自动修复

### **Q: "Missing modules" 错误**
A: 点击 "Generate Visual Studio project files" 按钮

### **Q: 编译卡住不动**
A: 等待 30 分钟，或重启 UE5 Editor

### **Q: 需要 Visual Studio？**
A: UE5 会自动调用 Visual Studio 进行编译

---

## 🎉 **编译成功后**

1. **创建 Blueprint**: `BP_MingWarGameMode`
2. **创建地图**: `TestMap.umap`
3. **设置 GameMode**: 选择 `BP_MingWarGameMode`
4. **测试游戏**: 点击 Play

---

## 📞 **需要帮助？**

如果仍然无法编译，请提供：
1. UE5 Editor 的错误截图
2. Output Log 内容
3. 运行 `Build.bat` 的输出

---

**现在请直接双击 `MingWarRTS.uproject` 文件！** 🥔✨