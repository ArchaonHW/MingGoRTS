# VR/AR 材質創建 - C++ 使用方法 (方法2)

## 概述

這是使用方法2（C++ 代碼）創建 VR/AR 材質的完整指南。

## 創建的 C++ 類

| 類名 | 文件路徑 | 說明 |
|------|----------|------|
| `UMingVRARMaterialFactory` | `Source/MingGoRTS/Public/VRAR/MingVRARMaterialFactory.h` | 核心工廠類 |
| `AMingVRARMaterialCreatorActor` | `Source/MingGoRTS/Public/VRAR/MingVRARMaterialCreatorActor.h` | 場景 Actor |
| `AMingVRARMaterialExecutor` | `Source/MingGoRTS/Public/VRAR/MingVRARMaterialExecutor.h` | 簡易執行器 |

---

## 使用方法

### 方法 A: 在藍圖中使用 (最簡單)

#### 步驟 1: 編譯項目
確保項目已經編譯成功（Development Editor 配置）。

#### 步驟 2: 在編輯器中使用
1. 打開 Unreal Editor
2. 在 Content Browser 搜索: `MingVRARMaterialExecutor`
3. 拖放到任意 Level 中
4. 選中 Actor
5. 在 Details 面板中點擊 **Execute Material Creation** 按鈕
6. 查看 Output Log 確認結果

#### 步驟 3: 驗證結果
在 Content Browser 檢查:
- `/Game/VR/Materials/` - 應有 9 個 `.uasset` 文件
- `/Game/AR/Materials/` - 應有 7 個 `.uasset` 文件

---

### 方法 B: 在 C++ 代碼中直接調用

#### 示例 1: 在 GameMode 中創建

```cpp
// MyGameMode.h
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameMode.generated.h"

UCLASS()
class MINGGORTS_API AMyGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    // 在 BeginPlay 或其他函數中調用
    UFUNCTION(BlueprintCallable, Category = "VR/AR")
    bool CreateVRARMaterials();
};
```

```cpp
// MyGameMode.cpp
#include "MyGameMode.h"
#include "VRAR/MingVRARMaterialFactory.h"

bool AMyGameMode::CreateVRARMaterials()
{
    // 創建材質工廠
    UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>();
    
    // 初始化
    Factory->Initialize(TEXT("C:/HW/MingGoRTS"));
    
    UE_LOG(LogTemp, Log, TEXT("=== Starting VR/AR Material Creation ==="));
    
    // 創建 VR 材質 (9 個)
    bool VRResult = Factory->CreateVRMaterials();
    
    // 創建 AR 材質 (7 個)
    bool ARResult = Factory->CreateARMaterials();
    
    // 獲取並顯示報告
    FString Report = Factory->GetCreationReport();
    UE_LOG(LogTemp, Log, TEXT("%s"), *Report);
    
    if (VRResult && ARResult)
    {
        UE_LOG(LogTemp, Display, TEXT("All materials created successfully!"));
        return true;
    }
    
    return false;
}
```

#### 示例 2: 創建單個材質

```cpp
#include "VRAR/MingVRARMaterialFactory.h"

void CreateSingleMaterialExample()
{
    UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>();
    Factory->Initialize(TEXT("C:/HW/MingGoRTS"));
    
    // 創建單個材質
    TMap<FString, FString> Config;
    Config.Add(TEXT("BaseColor"), TEXT("0.0, 0.8, 1.0"));
    Config.Add(TEXT("Roughness"), TEXT("0.2"));
    Config.Add(TEXT("Emissive"), TEXT("0.0, 0.8, 1.0"));
    
    UMaterial* NewMaterial = Factory->CreateMaterialFromConfig(
        TEXT("M_Custom_Material"),
        Config,
        TEXT("/Game/VR/Materials")
    );
    
    if (NewMaterial)
    {
        UE_LOG(LogTemp, Log, TEXT("Material created: %s"), *NewMaterial->GetName());
    }
}
```

#### 示例 3: 從 JSON 創建材質

```cpp
#include "VRAR/MingVRARMaterialFactory.h"

void CreateMaterialsFromJSONExample()
{
    UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>();
    Factory->Initialize(TEXT("C:/HW/MingGoRTS"));
    
    // 從 JSON 文件創建所有材質
    int32 CreatedCount = Factory->CreateMaterialsFromJSON(
        TEXT("C:/HW/MingGoRTS/Content/VR/Materials/MaterialConfigs.json"),
        TEXT("/Game/VR/Materials")
    );
    
    UE_LOG(LogTemp, Log, TEXT("Created %d materials from JSON"), CreatedCount);
}
```

---

### 方法 C: 使用藍圖函數庫

創建一個 Blueprint Function Library 以便在任意藍圖中調用：

```cpp
// VRARMaterialFunctionLibrary.h
#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VRARMaterialFunctionLibrary.generated.h"

UCLASS()
class MINGGORTS_API UVRARMaterialFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "VR/AR Materials", meta = (WorldContext = "WorldContextObject"))
    static bool CreateAllVRARMaterials(UObject* WorldContextObject);
    
    UFUNCTION(BlueprintCallable, Category = "VR/AR Materials", meta = (WorldContext = "WorldContextObject"))
    static bool CreateVRMaterialsOnly(UObject* WorldContextObject);
    
    UFUNCTION(BlueprintCallable, Category = "VR/AR Materials", meta = (WorldContext = "WorldContextObject"))
    static bool CreateARMaterialsOnly(UObject* WorldContextObject);
};
```

```cpp
// VRARMaterialFunctionLibrary.cpp
#include "VRARMaterialFunctionLibrary.h"
#include "VRAR/MingVRARMaterialFactory.h"

bool UVRARMaterialFunctionLibrary::CreateAllVRARMaterials(UObject* WorldContextObject)
{
    UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>();
    Factory->Initialize(TEXT("C:/HW/MingGoRTS"));
    
    bool VRResult = Factory->CreateVRMaterials();
    bool ARResult = Factory->CreateARMaterials();
    
    FString Report = Factory->GetCreationReport();
    UE_LOG(LogTemp, Log, TEXT("%s"), *Report);
    
    return VRResult && ARResult;
}

bool UVRARMaterialFunctionLibrary::CreateVRMaterialsOnly(UObject* WorldContextObject)
{
    UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>();
    Factory->Initialize(TEXT("C:/HW/MingGoRTS"));
    return Factory->CreateVRMaterials();
}

bool UVRARMaterialFunctionLibrary::CreateARMaterialsOnly(UObject* WorldContextObject)
{
    UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>();
    Factory->Initialize(TEXT("C:/HW/MingGoRTS"));
    return Factory->CreateARMaterials();
}
```

---

## 將創建的 16 個材質

### VR 材質 (9 個)
| 材質名 | 特性 |
|--------|------|
| M_VRController_Body | 深藍灰, 0.3 roughness |
| M_VRController_Button | 青色發光 |
| M_VRPointer_Laser | 透明激光 |
| M_VRPointer_Cursor | 綠色發光 |
| M_VRUI_Panel | 半透明面板 |
| M_VRUI_Border | 青色邊框 |
| M_VRTeleport_Valid | 綠色有效標記 |
| M_VRTeleport_Invalid | 紅色無效標記 |
| M_VRComfort_Vignette | 舒適暈影 |

### AR 材質 (7 個)
| 材質名 | 特性 |
|--------|------|
| M_ARPlane_Floor | 地板網格 |
| M_ARPlane_Wall | 牆面標記 |
| M_ARPlane_Boundary | 邊界線 |
| M_ARCursor_Default | 青色游標 |
| M_ARAnchor_Default | 綠色錨點 |
| M_AROverlay_Content | 內容疊加 |
| M_AROverlay_Hologram | 全息掃描線 |

---

## 驗證創建結果

### 步驟 1: 檢查文件
在 Content Browser 確認：
- 路徑 `/Game/VR/Materials/` 有 9 個材質
- 路徑 `/Game/AR/Materials/` 有 7 個材質

### 步驟 2: 檢查材質屬性
1. 雙擊任意材質打開 Material Editor
2. 檢查：
   - Base Color 是否正確
   - Emissive 材質是否使用 Unlit Shading Model
   - Translucent 材質是否設置了正確的 Blend Mode
   - 參數節點是否正確連接

### 步驟 3: 測試材質實例
1. 右鍵材質 → `Create Material Instance`
2. 修改參數值看是否正常更新

---

## 常見問題

### Q: 編譯錯誤 "MaterialEditor module not found"
**A**: 在 `MingGoRTS.Build.cs` 中添加：
```csharp
PrivateDependencyModuleNames.AddRange(new string[] {
    "MaterialEditor",
    "UnrealEd",
    "AssetTools",
    "EditorAssetLibrary"
});
```

### Q: 材質創建成功但沒有顯示
**A**: 
1. 在 Content Browser 點擊 `View Options > Show Engine Content`
2. 檢查輸出路徑是否正確
3. 查看 Output Log 確認沒有錯誤

### Q: 如何在遊戲運行時創建材質？
**A**: 這個工廠是 Editor-only 的。運行時創建材質需要不同的方法（使用 UMaterialInstanceDynamic）。

---

## 下一步

創建材質後：
1. 將材質應用到對應的 Mesh 資產
2. 創建 Blueprint 使用這些材質
3. 測試在 VR/AR 場景中的視覺效果
4. 根據需要調整材質參數

---

*文檔版本: 1.0*  
*適用於: UE 5.4+ | MingGoRTS Epic 9.1*
