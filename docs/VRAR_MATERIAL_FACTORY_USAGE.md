# VR/AR Material Factory - C++ Usage Guide (方法2)

## 概述

本指南說明如何使用 C++ MaterialFactory 類在 UE5 編輯器中創建 VR/AR 材質資產。

## 創建的文件

### C++ 源文件
| 文件 | 路徑 | 說明 |
|------|------|------|
| MaterialFactory Header | `Source/MingGoRTS/Public/VRAR/MingVRARMaterialFactory.h` | 核心工廠類 |
| MaterialFactory Implementation | `Source/MingGoRTS/Private/VRAR/MingVRARMaterialFactory.cpp` | 實現 |
| CreatorActor Header | `Source/MingGoRTS/Public/VRAR/MingVRARMaterialCreatorActor.h` | 易用 Actor |
| CreatorActor Implementation | `Source/MingGoRTS/Private/VRAR/MingVRARMaterialCreatorActor.cpp` | 實現 |
| Automation Test | `Source/MingGoRTS/Private/VRAR/Tests/MingVRARMaterialFactoryTest.cpp` | 測試 |

### 輔助文件
| 文件 | 路徑 | 說明 |
|------|------|------|
| Editor Utility Script | `Tools/build/EditorUtility_MaterialCreation.py` | 編輯器小工具腳本 |
| 執行啟動器 | `Tools/build/ExecuteMaterialCreation.ps1` | PowerShell 啟動器 |

---

## 編譯步驟

### 1. 確保模組包含
在 `Source/MingGoRTS/MingGoRTS.Build.cs` 中添加必要的依賴：

```csharp
PrivateDependencyModuleNames.AddRange(new string[] {
    "MaterialEditor",      // 用於材質編輯功能
    "UnrealEd",          // 用於編輯器功能
    "AssetTools",        // 用於資產創建
    "EditorAssetLibrary" // 用於資產操作
});
```

### 2. 編譯項目

**方法 A - 使用 IDE:**
1. 打開 `MingGoRTS.sln` (Visual Studio)
2. 選擇配置: `Development Editor`
3. 編譯整個解決方案 (Ctrl+Shift+B)

**方法 B - 使用 UBT:**
```powershell
cd C:\HW\MingGoRTS
C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat MingGoRTSEditor Win64 Development -Project="%CD%\MingGoRTS.uproject" -WaitMutex
```

**方法 C - 使用 Build 腳本:**
```powershell
.\BUILD_OPTIMIZED.cmd
```

---

## 使用方法

### 方法 1: 使用 MaterialCreatorActor (推薦)

#### 步驟 1: 創建測試關卡
1. 打開 UE5 編輯器
2. 創建新關卡: `File > New Level` (選擇 Empty)
3. 保存關卡: `Content/VRAR/MaterialCreationTestMap.umap`

#### 步驟 2: 放置 Actor
1. 在 Content Browser 搜索: `MingVRARMaterialCreatorActor`
2. 拖放到關卡中
3. 選中 Actor，在 Details 面板設置:
   - **Project Path**: `C:/HW/MingGoRTS` (默認已設置)

#### 步驟 3: 執行創建
1. 選中 Actor
2. 在 Details 面板找到 **VR/AR Material Creation** 部分
3. 點擊 **Create VR Materials** 按鈕
4. 點擊 **Create AR Materials** 按鈕
5. 查看 Output Log (Window > Developer Tools > Output Log)

#### 結果檢查
創建成功後，在 Content Browser 查看:
- `/Game/VR/Materials/` - 9 個 VR 材質
- `/Game/AR/Materials/` - 7 個 AR 材質

---

### 方法 2: 使用 C++ 代碼直接調用

#### 在 GameMode 中集成

```cpp
// 在 GameMode 的 BeginPlay 或自定義函數中

#include "VRAR/MingVRARMaterialFactory.h"

void AMyGameMode::CreateVRARMaterials()
{
    // 創建工廠
    UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>();
    Factory->Initialize(TEXT("C:/HW/MingGoRTS"));
    
    // 創建 VR 材質 (9 個)
    UE_LOG(LogTemp, Log, TEXT("Creating VR materials..."));
    bool VRResult = Factory->CreateVRMaterials();
    
    // 創建 AR 材質 (7 個)
    UE_LOG(LogTemp, Log, TEXT("Creating AR materials..."));
    bool ARResult = Factory->CreateARMaterials();
    
    // 獲取報告
    FString Report = Factory->GetCreationReport();
    UE_LOG(LogTemp, Log, TEXT("%s"), *Report);
    
    if (VRResult && ARResult)
    {
        UE_LOG(LogTemp, Display, TEXT("All materials created successfully!"));
    }
}
```

#### 在 Editor Module 中集成

```cpp
// 創建一個 Editor-only 的 Blutility 類

UCLASS()
class UMyMaterialCreationBlutility : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
    
    UFUNCTION(BlueprintCallable, Category = "VR/AR Materials")
    static bool CreateAllVRARMaterials()
    {
        UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>();
        Factory->Initialize(TEXT("C:/HW/MingGoRTS"));
        
        bool VRResult = Factory->CreateVRMaterials();
        bool ARResult = Factory->CreateARMaterials();
        
        return VRResult && ARResult;
    }
};
```

---

### 方法 3: 使用自動化測試

#### 步驟 1: 打開 Session Frontend
1. 打開 `Window > Developer Tools > Session Frontend`
2. 選擇 `Automation` 標籤
3. 找到 `MingGoRTS.VRAR.MaterialFactory.BasicTest`

#### 步驟 2: 運行測試
1. 勾選該測試
2. 點擊 **Start Tests** 按鈕
3. 查看測試結果

#### 命令行運行測試
```powershell
UnrealEditor-Cmd.exe C:\HW\MingGoRTS\MingGoRTS.uproject -run=AutomationTest -TestFilter="MingGoRTS.VRAR" -log
```

---

### 方法 4: 使用 Editor Utility Widget

#### 步驟 1: 創建 Editor Utility Widget
1. 在 Content Browser 右鍵
2. 選擇 `Editor Utilities > Editor Utility Widget`
3. 命名: `WBP_VRARMaterialCreator`
4. 雙擊打開 Widget Blueprint

#### 步驟 2: 設計界面
添加以下控件:
- **Text**: "VR/AR Material Creator"
- **Button**: "Create VR Materials"
- **Button**: "Create AR Materials"  
- **Button**: "Create All Materials"
- **Text Block**: 用於顯示結果 (命名: ResultText)

#### 步驟 3: 添加事件
1. 選中 "Create VR Materials" 按鈕
2. 在 Graph 中添加 `On Clicked` 事件
3. 添加 `Execute Python Script` 節點
4. Python 代碼:

```python
import unreal
factory = unreal.load_class(None, "/Script/MingGoRTS.MingVRARMaterialFactory")
instance = unreal.new_object(factory)
instance.initialize("C:/HW/MingGoRTS")
result = instance.create_vr_materials()
print("VR Materials Created: " + str(result))
```

#### 步驟 4: 運行 Widget
1. 點擊 `Launch this Editor Utility` 按鈕
2. 在新窗口中點擊按鈕創建材質

---

## 材質清單 (16 個)

### VR 材質 (9 個)
| 材質名 | 路徑 | 類型 | 特性 |
|--------|------|------|------|
| M_VRController_Body | /Game/VR/Materials/ | Opaque | 深藍灰, Roughness 0.3 |
| M_VRController_Button | /Game/VR/Materials/ | Emissive | 青色發光 |
| M_VRPointer_Laser | /Game/VR/Materials/ | Translucent | 透明激光效果 |
| M_VRPointer_Cursor | /Game/VR/Materials/ | Emissive | 綠色發光 |
| M_VRUI_Panel | /Game/VR/Materials/ | Translucent | 半透明面板 |
| M_VRUI_Border | /Game/VR/Materials/ | Emissive | 青色邊框 |
| M_VRTeleport_Valid | /Game/VR/Materials/ | Emissive | 綠色有效標記 |
| M_VRTeleport_Invalid | /Game/VR/Materials/ | Emissive | 紅色無效標記 |
| M_VRComfort_Vignette | /Game/VR/Materials/ | Translucent | 舒適暈影 |

### AR 材質 (7 個)
| 材質名 | 路徑 | 類型 | 特性 |
|--------|------|------|------|
| M_ARPlane_Floor | /Game/AR/Materials/ | Translucent + Grid | 地板網格 |
| M_ARPlane_Wall | /Game/AR/Materials/ | Translucent | 牆面標記 |
| M_ARPlane_Boundary | /Game/AR/Materials/ | Emissive | 邊界線 |
| M_ARCursor_Default | /Game/AR/Materials/ | Emissive | 青色游標 |
| M_ARAnchor_Default | /Game/AR/Materials/ | Emissive | 綠色錨點 |
| M_AROverlay_Content | /Game/AR/Materials/ | Translucent | 內容疊加 |
| M_AROverlay_Hologram | /Game/AR/Materials/ | Translucent + Hologram | 全息效果 |

---

## 故障排除

### 問題 1: 編譯錯誤 "MaterialEditor module not found"
**解決**: 在 `MingGoRTS.Build.cs` 中添加 `MaterialEditor` 到 `PrivateDependencyModuleNames`

### 問題 2: "Failed to load MingVRARMaterialFactory class"
**解決**: 確保項目已完全編譯 (Development Editor 配置)

### 問題 3: 材質創建成功但沒有顯示
**解決**: 
1. 在 Content Browser 點擊 `View Options > Show Engine Content`
2. 檢查 `/Game/VR/Materials/` 和 `/Game/AR/Materials/` 路徑
3. 查看 Output Log 確認沒有錯誤

### 問題 4: 材質參數沒有正確設置
**解決**: 
1. 雙擊材質打開 Material Editor
2. 檢查參數節點是否正確連接
3. 手動調整參數值測試

---

## 驗證步驟

1. **檢查文件數量**
   - VR 路徑應有 9 個 `.uasset` 文件
   - AR 路徑應有 7 個 `.uasset` 文件

2. **檢查材質屬性**
   - 雙擊每個材質查看:
     - Base Color 是否正確 (檢查 RGB 值)
     - Emissive 材質是否使用 Unlit Shading Model
     - Translucent 材質是否設置了正確的 Blend Mode

3. **測試材質實例**
   - 右鍵材質 > `Create Material Instance`
   - 修改參數看是否正常更新

---

## 進階使用

### 創建材質實例變體
```cpp
// 創建藍色變體
UMaterial* OriginalMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/VR/Materials/M_VRController_Button"));
UMaterialInstanceConstant* Instance = NewObject<UMaterialInstanceConstant>();
Instance->SetParentEditorOnly(OriginalMaterial);
// 修改實例參數...
```

### 批量創建材質
```cpp
// 使用 JSON 配置直接創建
UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>();
int32 CreatedCount = Factory->CreateMaterialsFromJSON(
    TEXT("C:/HW/MingGoRTS/Content/VR/Materials/MaterialConfigs.json"),
    TEXT("/Game/VR/Materials")
);
```

---

## 下一步

創建材質後:
1. 將材質應用到對應的 Mesh 資產
2. 創建 Blueprint 使用這些材質
3. 測試在 VR/AR 場景中的視覺效果
4. 根據需要調整材質參數

---

*文檔版本: 1.0*  
*適用於: UE 5.4+ | MingGoRTS Epic 9.1*  
*創建日期: 2026-03-23*
