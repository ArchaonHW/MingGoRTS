# MingWarRTS 手动设置指南

## 🎯 **编译失败解决步骤**

如果看到错误："MingWarRTS could not be compiled. Try rebuilding from source manually."

请按以下步骤操作：

---

## **步骤 1：在 UE5 Editor 中修复**

1. 如果 UE5 Editor 已经打开，你会看到红色错误栏
2. 点击右上角的 **"Fix"** 按钮
   - 或者点击 **"Generate Visual Studio project files"**
3. UE5 会自动运行 UBT 并生成项目文件
4. 等待 5-10 分钟完成

---

## **步骤 2：如果 "Fix" 按钮无效**

1. 关闭 UE5 Editor
2. 在项目文件夹 `C:\HW\MingGoRTS\` 中
3. **删除** 以下文件夹（如果存在）：
   - `Intermediate`
   - `Saved`
   - `Binaries`
4. 重新双击 `MingWarRTS.uproject` 打开
5. UE5 会自动重新生成所有文件

---

## **步骤 3：如果仍然失败 - 手动生成 .sln**

1. 打开 **命令提示符**（cmd.exe）
2. 运行：
   ```cmd
   cd C:\HW\MingGoRTS
   "C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\Build.bat" MingWarRTS Win64 Development -Project="MingWarRTS.uproject"
   ```
3. 等待编译完成
4. 应该会生成 `MingWarRTS.sln` 文件

---

## **步骤 4：使用 Visual Studio 编译**

1. 打开 `MingWarRTS.sln`
2. 选择配置：
   - Solution Configurations: **Development**
   - Solution Platforms: **Win64**
3. 生成 → 重新生成解决方案（Ctrl+Shift+B）
4. 等待编译完成

---

## **步骤 5：验证编译成功**

编译成功后：
- ✅ UE5 Editor 正常打开，无错误
- ✅ 可以右键创建 Blueprint Class
- ✅ Output Log 显示 "Compilation successful"
- ✅ 项目根目录有 `MingWarRTS.sln` 文件

---

## **常见问题**

### **Q: 点击 "Fix" 没反应？**
A: 关闭 Editor，删除 Intermediate/Saved/Binaries，重新打开 .uproject

### **Q: UBT 报错 "No target name"？**
A: Build.cs 配置有问题。我们已使用最简配置，应该正常。

### **Q: Visual Studio 编译失败？**
A: 确保安装了 "Desktop development with C++" 工作负载和 Windows SDK

### **Q: 编译太慢？**
A: 首次编译 10-30 分钟是正常的。去喝杯咖啡 ☕

---

## **如果以上都失败**

请截图以下信息：
1. UE5 Editor 的错误窗口（完整文字）
2. Output Log 内容（Window → Developer Tools → Output Log → Copy All）
3. 运行 `Build.bat` 的输出

---

## **项目结构确认**

确保以下文件存在：

```
C:\HW\MingGoRTS\
├── MingWarRTS.uproject          (✅)
├── Source\MingWarRTS\
│   ├── Public\
│   │   ├── MingWarRTS.h         (✅)
│   │   ├── RTSUnit.h            (✅)
│   │   ├── RTSBuilding.h        (✅)
│   │   ├── RTSPlayerController.h (✅)
│   │   └── ResourceManager.h    (✅)
│   ├── Private\
│   │   ├── MingWarRTS.cpp       (✅)
│   │   ├── RTSUnit.cpp          (✅)
│   │   ├── RTSBuilding.cpp      (✅)
│   │   ├── RTSPlayerController.cpp (✅)
│   │   └── ResourceManager.cpp  (✅)
│   └── MingWarRTS.Build.cs      (✅)
└── Config\ (✅)
```

所有文件都已就绪，问题应该在 UE5 编译流程中。

---

**最关键的一步：双击 `MingWarRTS.uproject` 并点击 "Fix"** 🛠️