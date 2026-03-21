# MingWarRTS ?‹åŠ¨ç¼–è??‡å?

## ?š¨ **ç¼–è?å¤±è´¥è§?†³?¹æ?**

### **?®é?ï¼?* "MingWarRTS could not be compiled. Try rebuilding from source manually."

è¿™æ˜¯ UE5 ?¨é?æ¬¡æ?å¼€é¡¹ç›®?¶å¸¸è§ç??™è¯¯ï¼Œé€šå¸¸?¯å?ä¸ºé¡¹?®æ?ä»¶æœª?Ÿæ???Visual Studio C++ ç»„ä»¶ç¼ºå¤±??
---

## **è§?†³?¹æ? 1ï¼šç¡®ä¿?Visual Studio C++ å·²å?è£?*

### **æ£€??Visual Studio ?ˆæœ¬**
?“å? Visual Studio Installerï¼Œç¡®è®¤å?è£…ä?ï¼?
- **Visual Studio 2019** ??**Visual Studio 2022**
- å·¥ä?è´Ÿè½½ï¼?*Desktop development with C++** ??- ç»„ä»¶ï¼?  - ??Windows 10/11 SDK (10.0.19041.0 ?–æ›´é«?
  - ??MSVC v142 - VS 2019 C++ x64/x86 build tools (??v143 for VS 2022)
  - ??C++ ATL for latest build tools (?¯é€‰ä??¨è?)

### **å¿«é€Ÿé?è¯?*
?“å? **Developer Command Prompt** (VS 2022) ??**x64 Native Tools Command Prompt** (VS 2019)ï¼Œè?è¡Œï?
```cmd
cl
```
å¦‚æ??¾ç¤º?ˆæœ¬ä¿¡æ¯ï¼Œè¯´??C++ ç¼–è??¨å¯?¨ã€?
---

## **è§?†³?¹æ? 2ï¼šæ??¨ç??é¡¹?®æ?ä»?*

### **?¹æ? Aï¼šä½¿??UnrealBuildTool (?¨è?)**

```cmd
cd C:\HW\MingGoRTS

REM ä½¿ç”¨?­è·¯å¾„é¿?ç©º?¼é—®é¢?set UE5=C:\PROGRA~1\Epic Games\UE_5.7

REM ?Ÿæ?é¡¹ç›®?‡ä»¶
"%UE5%\Engine\Binaries\DotNET\UnrealBuildTool.exe" ^
  -projectfiles ^
  -project="MingWarRTS.uproject" ^
  -game ^
  -progress
```

### **?¹æ? Bï¼šä½¿??GenerateProjectFiles.bat**

```cmd
cd C:\HW\MingGoRTS

REM æ£€?¥æ­£ç¡®ç?è·¯å?
dir "C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\GenerateProjectFiles.bat"

REM å¦‚æ?å­˜åœ¨ï¼Œè?è¡Œå?
"C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\GenerateProjectFiles.bat" ^
  -project="MingWarRTS.uproject" ^
  -game ^
  -engine ^
  -progress
```

---

## **è§?†³?¹æ? 3ï¼šæ??¥é¡¹?®æ?ä»¶ç???*

ç¡®è®¤ä»¥ä??‡ä»¶å­˜åœ¨ï¼?
```
??Source/MingWarRTS/Public/MingWarRTS.h
??Source/MingWarRTS/Private/MingWarRTS.cpp
??Source/MingWarRTS/Public/RTSUnit.h
??Source/MingWarRTS/Private/RTSUnit.cpp
??Source/MingWarRTS/Public/RTSBuilding.h
??Source/MingWarRTS/Private/RTSBuilding.cpp
??Source/MingWarRTS/Public/RTSPlayerController.h
??Source/MingWarRTS/Private/RTSPlayerController.cpp
??Source/MingWarRTS/Public/ResourceManager.h
??Source/MingWarRTS/Private/ResourceManager.cpp
??Source/MingWarRTS/MingWarRTS.Build.cs  <-- ?³é”®ï¼?```

**ç¼ºå¤± Build.csï¼?* ?‘å·²ç¶“å‰µå»ºä?ï¼Œè?æª¢æŸ¥ï¼?```
Source/MingWarRTS/MingWarRTS.Build.cs
```

---

## **è§?†³?¹æ? 4ï¼šç›´?¥ç?è¯?C++ ä»??**

### **æ­¥éª¤ 1ï¼šæ?å¼€å¼€?‘è€…å‘½ä»¤æ?ç¤?*

1. ??`Win+R`ï¼Œè???`cmd`
2. ?–æ?ç´?"Developer Command Prompt for VS 2022"
3. ?–ä»¥ç®¡ç??˜èº«ä»½æ?å¼€ "x64 Native Tools Command Prompt"

### **æ­¥éª¤ 2ï¼šå¯¼?ªåˆ°é¡¹ç›®?®å?**
```cmd
cd C:\HW\MingGoRTS
```

### **æ­¥éª¤ 3ï¼šè?è¡?UBT ç¼–è?**
```cmd
REM è®¾ç½® UE5 ?¯å?
set UE5=C:\Program Files\Epic Games\UE_5.7

REM ä½¿ç”¨ UBT ç¼–è?
"%UE5%\Engine\Binaries\DotNET\UnrealBuildTool.exe" ^
  Win64 ^
  Development ^
  -Project="MingWarRTS.uproject" ^
  -Target="MingWarRTS" ^
  -NoUBTMakefiles
```

**æ³¨æ?ï¼?* `-Target="MingWarRTS"` ?‡å?äº†æ¨¡çµ„å?ç¨±ï?ä¾†è‡ª Build.cs ä¸­ç? `public class MingWarRTS`ï¼‰ã€?
---

## **è§?†³?¹æ? 5ï¼šä¿®å¤å¸¸è§é?è¯?*

### **?™è¯¯ï¼š`The specified module could not be found`**

**?Ÿå?ï¼?* Build.cs ä¸­ä?èµ–ç?æ¨¡å??ªæ­£ç¡®é?ç½®ã€?
**å¿«é€Ÿä¿®å¤ï?** ç®€??Build.csï¼Œåªä¿ç??ºç?ä¾è?ï¼?
```csharp
using UnrealBuildTool;

public class MingWarRTS : ModuleRules
{
    public MingWarRTS(ReadOnlyTargetRules Target) : base(Target)
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

**?´æ–° Build.csï¼?*
- ? é™¤å¤æ??ç½®
- ?ªä???Core, Engine, InputCore, Slate ç­‰åŸºç¡€æ¨¡å?
- ?æ–°ç¼–è?

---

### **?™è¯¯ï¼š`cannot open source file`**

**?Ÿå?ï¼?* æºæ?ä»¶è·¯å¾„ä?å¯¹æ??‡ä»¶?é?è¯¯ã€?
**æ£€?¥ï?**
1. `Source/MingWarRTS/Public/` ?€??`.h` ?‡ä»¶?¯å¦?½åœ¨ï¼?2. `Source/MingWarRTS/Private/` ?€??`.cpp` ?‡ä»¶?¯å¦?½åœ¨ï¼?3. ç±»å??¯å¦?¹é?ï¼Ÿ`MINGWARRTS_API` å®æ˜¯?¦ä??´ï?

---

### **?™è¯¯ï¼š`The module 'MingWarRTS' could not be loaded`**

**?Ÿå?ï¼?* ç¼–è?å¤±è´¥??DLL ?ªç??ã€?
**è§?†³?¹æ?ï¼?*
1. ? é™¤ `Binaries/`, `Intermediate/`, `Saved/` ?‡ä»¶å¤?2. ?æ–°è¿è? UBT
3. æ£€??`Log.txt` ?‡ä»¶ï¼š`%TEMP%\UnrealBuildTool\*.log`

---

## **è§?†³?¹æ? 6ï¼šç??–å?å§‹ç???*

å¦‚æ?ä¸Šè¿°?½å¤±è´¥ï??‘ä»¬?¯ä»¥?›å»ºä¸€ä¸?*?ç??ˆæœ¬**?ˆè®©é¡¹ç›®è¿è?èµ·æ¥ï¼?
### **?ç? Build.cs**
```csharp
using UnrealBuildTool;

public class MingWarRTS : ModuleRules
{
    public MingWarRTS(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Engine", "Slate", "SlateCore" });
    }
}
```

### **?ç? GameMode å¤´æ?ä»?*
```cpp
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MingWarRTS.generated.h"

UCLASS()
class MINGWARRTS_API AMingWarRTS : public AGameModeBase
{
    GENERATED_BODY()
};
```

### **?ç? GameMode æºæ?ä»?*
```cpp
#include "MingWarRTS.h"
AMingWarRTS::AMingWarRTS()
{
}
```

**?ˆç?è¯‘è?ä¸ªæ?ç®€?ˆæœ¬**ï¼Œæ??Ÿå??é€æ­¥æ·»å??¶ä?ç±»ã€?
---

## **å¿«é€Ÿä¿®å¤è???*

?‘å??›å»ºä¸€ä¸ªè‡ª?¨ä¿®å¤è??¬ï?å®ƒä?ï¼?1. æ£€??VS C++ å®‰è?
2. ç®€??Build.cs
3. æ¸…ç?å¹¶é??°ç?è¯?4. ?Ÿæ?é¡¹ç›®?‡ä»¶

è¯·ç?ç­?..

---

## **ä¸‹ä?æ­¥æ?ä½?*

1. **?ˆå?è¯•è§£?³æ–¹æ¡?2**ï¼šæ??¨ç??é¡¹?®æ?ä»?2. **å¦‚æ?å¤±è´¥**ï¼Œè?è¡Œæ??³å??›å»º?„ä¿®å¤è???3. **å¦‚æ?ä»æ??®é?**ï¼Œæ?ä»¬ä½¿?¨æ?ç®€?ˆæœ¬?æ­¥æ·»å??Ÿèƒ½

è¯·å?è¯‰æ?ï¼?- ?¨æ˜¯?¦å?è£…ä? Visual Studioï¼?- ??VS 2019 è¿˜æ˜¯ VS 2022ï¼?- Windows SDK ?¯å¦å®‰è?ï¼?
è¿™ä?ä¿¡æ¯å°†å¸®?©æ??´ç²¾ç¡®åœ°å®šä??®é?ï¼ğ??
---

*?´æ–°?¥æ?ï¼?026-03-15 20:25 GMT+8*
