# MingGoRTS ?�动编�??��?

## ?�� **编�?失败�?��?��?**

### **?��?�?* "MingGoRTS could not be compiled. Try rebuilding from source manually."

这是 UE5 ?��?次�?开项目?�常见�??�误，通常?��?为项?��?件未?��???Visual Studio C++ 组件缺失??
---

## **�?��?��? 1：确�?Visual Studio C++ 已�?�?*

### **检??Visual Studio ?�本**
?��? Visual Studio Installer，确认�?装�?�?
- **Visual Studio 2019** ??**Visual Studio 2022**
- 工�?负载�?*Desktop development with C++** ??- 组件�?  - ??Windows 10/11 SDK (10.0.19041.0 ?�更�?
  - ??MSVC v142 - VS 2019 C++ x64/x86 build tools (??v143 for VS 2022)
  - ??C++ ATL for latest build tools (?�选�??��?)

### **快速�?�?*
?��? **Developer Command Prompt** (VS 2022) ??**x64 Native Tools Command Prompt** (VS 2019)，�?行�?
```cmd
cl
```
如�??�示?�本信息，说??C++ 编�??�可?��?
---

## **�?��?��? 2：�??��??�项?��?�?*

### **?��? A：使??UnrealBuildTool (?��?)**

```cmd
cd C:\HW\MingGoRTS

REM 使用?�路径避?�空?�问�?set UE5=C:\PROGRA~1\Epic Games\UE_5.7

REM ?��?项目?�件
"%UE5%\Engine\Binaries\DotNET\UnrealBuildTool.exe" ^
  -projectfiles ^
  -project="MingGoRTS.uproject" ^
  -game ^
  -progress
```

### **?��? B：使??GenerateProjectFiles.bat**

```cmd
cd C:\HW\MingGoRTS

REM 检?�正确�?路�?
dir "C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\GenerateProjectFiles.bat"

REM 如�?存在，�?行�?
"C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\GenerateProjectFiles.bat" ^
  -project="MingGoRTS.uproject" ^
  -game ^
  -engine ^
  -progress
```

---

## **�?��?��? 3：�??�项?��?件�???*

确认以�??�件存在�?
```
??Source/MingGoRTS/Public/MingGoRTS.h
??Source/MingGoRTS/Private/MingGoRTS.cpp
??Source/MingGoRTS/Public/RTSUnit.h
??Source/MingGoRTS/Private/RTSUnit.cpp
??Source/MingGoRTS/Public/RTSBuilding.h
??Source/MingGoRTS/Private/RTSBuilding.cpp
??Source/MingGoRTS/Public/RTSPlayerController.h
??Source/MingGoRTS/Private/RTSPlayerController.cpp
??Source/MingGoRTS/Public/ResourceManager.h
??Source/MingGoRTS/Private/ResourceManager.cpp
??Source/MingGoRTS/MingGoRTS.Build.cs  <-- ?�键�?```

**缺失 Build.cs�?* ?�已經創建�?，�?檢查�?```
Source/MingGoRTS/MingGoRTS.Build.cs
```

---

## **�?��?��? 4：直?��?�?C++ �??**

### **步骤 1：�?开开?�者命令�?�?*

1. ??`Win+R`，�???`cmd`
2. ?��?�?"Developer Command Prompt for VS 2022"
3. ?�以管�??�身份�?开 "x64 Native Tools Command Prompt"

### **步骤 2：导?�到项目?��?**
```cmd
cd C:\HW\MingGoRTS
```

### **步骤 3：�?�?UBT 编�?**
```cmd
REM 设置 UE5 ?��?
set UE5=C:\Program Files\Epic Games\UE_5.7

REM 使用 UBT 编�?
"%UE5%\Engine\Binaries\DotNET\UnrealBuildTool.exe" ^
  Win64 ^
  Development ^
  -Project="MingGoRTS.uproject" ^
  -Target="MingGoRTS" ^
  -NoUBTMakefiles
```

**注�?�?* `-Target="MingGoRTS"` ?��?了模組�?稱�?來自 Build.cs 中�? `public class MingGoRTS`）�?
---

## **�?��?��? 5：修复常见�?�?*

### **?�误：`The specified module could not be found`**

**?��?�?* Build.cs 中�?赖�?模�??�正确�?置�?
**快速修复�?** 简??Build.cs，只保�??��?依�?�?
```csharp
using UnrealBuildTool;

public class MingGoRTS : ModuleRules
{
    public MingGoRTS(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "CoreUObject",
            "Engine",
            "Slate",
            "SlateCore"
        });
    }
}
```

**?�新 Build.cs�?*
- ?�除复�??�置
- ?��???Core, Engine, InputCore, Slate 等基础模�?
- ?�新编�?

---

### **?�误：`cannot open source file`**

**?��?�?* 源�?件路径�?对�??�件?��?误�?
**检?��?**
1. `Source/MingGoRTS/Public/` ?�??`.h` ?�件?�否?�在�?2. `Source/MingGoRTS/Private/` ?�??`.cpp` ?�件?�否?�在�?3. 类�??�否?��?？`MingGoRTS_API` 宏是?��??��?

---

### **?�误：`The module 'MingGoRTS' could not be loaded`**

**?��?�?* 编�?失败??DLL ?��??��?
**�?��?��?�?*
1. ?�除 `Binaries/`, `Intermediate/`, `Saved/` ?�件�?2. ?�新运�? UBT
3. 检??`Log.txt` ?�件：`%TEMP%\UnrealBuildTool\*.log`

---

## **�?��?��? 6：�??��?始�???*

如�?上述?�失败�??�们?�以?�建一�?*?��??�本**?�让项目运�?起来�?
### **?��? Build.cs**
```csharp
using UnrealBuildTool;

public class MingGoRTS : ModuleRules
{
    public MingGoRTS(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Engine", "Slate", "SlateCore" });
    }
}
```

### **?��? GameMode 头�?�?*
```cpp
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MingGoRTS.generated.h"

UCLASS()
class MingGoRTS_API AMingGoRTS : public AGameModeBase
{
    GENERATED_BODY()
};
```

### **?��? GameMode 源�?�?*
```cpp
#include "MingGoRTS.h"
AMingGoRTS::AMingGoRTS()
{
}
```

**?��?译�?个�?简?�本**，�??��??�逐步添�??��?类�?
---

## **快速修复�???*

?��??�建一个自?�修复�??��?它�?�?1. 检??VS C++ 安�?
2. 简??Build.cs
3. 清�?并�??��?�?4. ?��?项目?�件

请�?�?..

---

## **下�?步�?�?*

1. **?��?试解?�方�?2**：�??��??�项?��?�?2. **如�?失败**，�?行�??��??�建?�修复�???3. **如�?仍�??��?**，�?们使?��?简?�本?�步添�??�能

请�?诉�?�?- ?�是?��?装�? Visual Studio�?- ??VS 2019 还是 VS 2022�?- Windows SDK ?�否安�?�?
这�?信息将帮?��??�精确地定�??��?！�??
---

*?�新?��?�?026-03-15 20:25 GMT+8*
