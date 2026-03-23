# 方法2: C++ 代碼調用 - 快速參考

## 概述

這是最直接的使用方式，通過C++代碼直接調用材質工廠創建VR/AR材質。

---

## 已創建的示例文件

| 文件 | 路徑 | 說明 |
|------|------|------|
| **示例 GameMode** | `Source/MingGoRTS/Public/VRAR/Examples/VRARMaterialGameMode.h` | 完整示例 |
| **示例實現** | `Source/MingGoRTS/Private/VRAR/Examples/VRARMaterialGameMode.cpp` | 實現代碼 |

---

## 快速開始

### 步驟 1: 編譯項目
```powershell
cd C:\HW\MingGoRTS
.\BUILD_OPTIMIZED.cmd
```

### 步驟 2: 設置 GameMode
1. 打開任意 Level
2. 打開 **World Settings** (窗口 → World Settings)
3. 設置 **GameMode Override** 為 `VRARMaterialGameMode`
4. 點擊 **Play in Editor (PIE)**

### 步驟 3: 查看結果
1. 查看 **Output Log** (窗口 → 開發者工具 → Output Log)
2. 在 Content Browser 檢查:
   - `/Game/VR/Materials/` - 應有 9 個材質
   - `/Game/AR/Materials/` - 應有 7 個材質

---

## 核心代碼片段

### 完整創建流程
```cpp
// 包含頭文件
#include "VRAR/MingVRARMaterialFactory.h"

// 創建材質
void CreateMaterials()
{
    // 1. 創建工廠
    UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>();
    
    // 2. 初始化
    Factory->Initialize(TEXT("C:/HW/MingGoRTS"));
    
    // 3. 創建 VR 材質 (9 個)
    bool VRResult = Factory->CreateVRMaterials();
    
    // 4. 創建 AR 材質 (7 個)
    bool ARResult = Factory->CreateARMaterials();
    
    // 5. 獲取報告
    FString Report = Factory->GetCreationReport();
    UE_LOG(LogTemp, Log, TEXT("%s"), *Report);
}
```

### 在任意 Actor 中使用
```cpp
void AMyActor::BeginPlay()
{
    Super::BeginPlay();
    
    // 創建材質
    UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>(this);
    Factory->Initialize(TEXT("C:/HW/MingGoRTS"));
    
    Factory->CreateVRMaterials();
    Factory->CreateARMaterials();
}
```

### 在藍圖中調用
```cpp
// 創建 Blueprint 可調用的函數
UFUNCTION(BlueprintCallable, Category = "VR/AR Materials")
static bool CreateAllMaterials();

// 實現
bool UMyBlueprintLibrary::CreateAllMaterials()
{
    UMingVRARMaterialFactory* Factory = NewObject<UMingVRARMaterialFactory>();
    Factory->Initialize(TEXT("C:/HW/MingGoRTS"));
    
    return Factory->CreateVRMaterials() && Factory->CreateARMaterials();
}
```

---

## 材質清單 (將創建 16 個)

### VR 材質 (9 個)
1. `M_VRController_Body` - 控制器材質
2. `M_VRController_Button` - 按鈕發光
3. `M_VRPointer_Laser` - 激光指針
4. `M_VRPointer_Cursor` - 游標
5. `M_VRUI_Panel` - UI面板
6. `M_VRUI_Border` - UI邊框
7. `M_VRTeleport_Valid` - 有效傳送標記
8. `M_VRTeleport_Invalid` - 無效傳送標記
9. `M_VRComfort_Vignette` - 舒適暈影

### AR 材質 (7 個)
1. `M_ARPlane_Floor` - 地板網格
2. `M_ARPlane_Wall` - 牆面
3. `M_ARPlane_Boundary` - 邊界線
4. `M_ARCursor_Default` - 游標
5. `M_ARAnchor_Default` - 錨點
6. `M_AROverlay_Content` - 內容疊加
7. `M_AROverlay_Hologram` - 全息效果

---

## 驗證結果

### 檢查文件
1. 在 Content Browser 中導航到:
   - `Content/VR/Materials/`
   - `Content/AR/Materials/`

2. 確認文件數量:
   - VR: 9 個 `.uasset` 文件
   - AR: 7 個 `.uasset` 文件

### 檢查材質屬性
1. 雙擊任意材質打開 Material Editor
2. 檢查 Base Color、Roughness、Emissive 等參數
3. 確認 Translucent 材質的 Blend Mode 正確

---

## 故障排除

### 問題: "Failed to load MingVRARMaterialFactory class"
**解決**: 確保項目已完全編譯 (Development Editor 配置)

### 問題: 編譯錯誤 "MaterialEditor module not found"
**解決**: 在 `MingGoRTS.Build.cs` 中添加:
```csharp
PrivateDependencyModuleNames.AddRange(new string[] {
    "MaterialEditor", "UnrealEd", "AssetTools", "EditorAssetLibrary"
});
```

### 問題: 材質創建成功但沒有顯示
**解決**: 
1. 檢查 Content Browser 的 View Options
2. 確認路徑 `/Game/VR/Materials/` 和 `/Game/AR/Materials/`

---

## 下一步

創建材質後:
1. 將材質應用到 Mesh 資產
2. 創建 Blueprint 使用這些材質
3. 測試在 VR/AR 場景中的效果
4. 調整材質參數以符合需求

---

*文檔版本: 1.0*  
*適用於: UE 5.4+*
