# MingGoRTS 自動場景生成系統使用指南

## 概述

MingGoRTS 自動場景生成系統是一個強大的工具，能夠根據需求自動生成各種素材並建立完整的遊戲場景。系統整合了AI內容生成、場景渲染和資產管理，為遊戲開發者提供一站式場景創建解決方案。

## 系統架構

### 核心組件

1. **MingAutoSceneGenerator** - 自動場景生成器
2. **MingAIContentManager** - AI內容管理器
3. **MingAIGeneratedContentSystem** - AI內容生成系統
4. **MingAIContentRenderer** - AI內容渲染器

### 支持的場景類型

- **戰鬥場景** (Battle) - 戰爭和戰鬥相關場景
- **村莊場景** (Village) - 和平的村莊生活場景
- **宮殿場景** (Palace) - 宏偉的宮殿建築場景
- **自然場景** (Forest, Mountain, River, Desert) - 各種自然環境
- **城市場景** (City) - 繁華的城市場景
- **寺廟場景** (Temple) - 宗教建築場景
- **戰場場景** (Battlefield) - 大規模戰場場景
- **自定義場景** (Custom) - 用戶自定義場景

## 功能特性

### 🎨 場景生成

- **智能素材生成** - 根據場景需求自動生成所需素材
- **多種風格支持** - 10種藝術風格選擇
- **複雜度控制** - 4個複雜度級別
- **場景變體生成** - 自動生成場景變體

### 🎬 場景渲染

- **實時預覽** - 即時預覽生成場景
- **高質量渲染** - 電影級質量輸出
- **環境控制** - 完整的環境設置
- **特效系統** - 視覺和音效特效

### 📁 場景管理

- **場景庫管理** - 組織和管理生成場景
- **模板系統** - 預設場景模板
- **批量操作** - 高效的批量處理
- **版本控制** - 場景版本管理

### ⚡ 性能優化

- **異步生成** - 非阻塞的場景生成
- **智能緩存** - 避免重複生成
- **資源監控** - 實時性能監控
- **自動優化** - 自動場景優化

## 快速開始

### 1. 初始化系統

```cpp
// 在遊戲開始時初始化自動場景生成器
UMingAutoSceneGenerator* SceneGenerator = NewObject<UMingAutoSceneGenerator>();
SceneGenerator->InitializeAutoSceneGenerator();
```

### 2. 使用預設模板生成場景

```cpp
// 生成戰鬥場景
FSceneRequirement BattleRequirements = SceneGenerator->GetBattleSceneTemplate(TEXT("Ming dynasty epic battle"));
FString BattleSceneName = SceneGenerator->GenerateScene(BattleRequirements);

// 生成村莊場景
FSceneRequirement VillageRequirements = SceneGenerator->GetVillageSceneTemplate(TEXT("Peaceful Ming village"));
FString VillageSceneName = SceneGenerator->GenerateScene(VillageRequirements);

// 生成宮殿場景
FSceneRequirement PalaceRequirements = SceneGenerator->GetPalaceSceneTemplate(TEXT("Magnificent Ming imperial palace"));
FString PalaceSceneName = SceneGenerator->GenerateScene(PalaceRequirements);
```

### 3. 自定義場景生成

```cpp
// 創建自定義場景需求
FSceneRequirement CustomRequirements;
CustomRequirements.SceneType = ESceneType::Forest;
CustomRequirements.Complexity = ESceneComplexity::Complex;
CustomRequirements.Style = ESceneStyle::Photorealistic;
CustomRequirements.Theme = TEXT("Ming Dynasty Nature");
CustomRequirements.Description = TEXT("Ancient Chinese bamboo forest with mist and sunlight");
CustomRequirements.Duration = 45.0f;
CustomRequirements.bHasCharacters = true;
CustomRequirements.bHasBuildings = false;
CustomRequirements.bHasEnvironment = true;
CustomRequirements.bHasEffects = true;

// 設置所需資產
CustomRequirements.RequiredAssets = {
    EAIContentType::Image,
    EAIContentType::Music,
    EAIContentType::SoundEffect,
    EAIContentType::Texture,
    EAIContentType::Material
};

// 設置資產提示
CustomRequirements.AssetPrompts = {
    {EAIContentType::Image, TEXT("Ancient Chinese bamboo forest with mist and sunlight, photorealistic")},
    {EAIContentType::Music, TEXT("Peaceful Chinese nature music with bamboo flute")},
    {EAIContentType::SoundEffect, TEXT("Forest sounds: wind through bamboo, birds, gentle stream")},
    {EAIContentType::Texture, TEXT("Bamboo forest ground texture with fallen leaves")},
    {EAIContentType::Material, TEXT("Natural bamboo and moss material")}
};

// 生成場景
FString CustomSceneName = SceneGenerator->GenerateScene(CustomRequirements);
```

### 4. 場景預覽和導出

```cpp
// 啟動場景預覽
SceneGenerator->StartScenePreview(CustomSceneName);

// 導出場景為視頻
SceneGenerator->ExportSceneAsVideo(CustomSceneName, 30.0f, TEXT("/Game/ExportedScenes/ForestScene.mp4"));

// 生成場景縮略圖
SceneGenerator->GenerateSceneThumbnail(CustomSceneName);
```

## 高級用法

### 場景變體生成

```cpp
// 生成場景變體
TArray<FSceneRequirement> Variants = SceneGenerator->GenerateSceneVariants(CustomRequirements, 5);

// 批量生成變體
SceneGenerator->GenerateSceneVariantsAsync(CustomRequirements, 5);
```

### 智能推薦

```cpp
// 獲取推薦場景
TArray<FSceneRequirement> Recommendations = SceneGenerator->GetRecommendedScenes(TEXT("epic battle with Ming dynasty warriors"));

// 優化場景需求
FSceneRequirement OptimizedRequirements = SceneGenerator->OptimizeSceneRequirements(CustomRequirements);
```

### 批量操作

```cpp
// 批量生成場景
TArray<FSceneRequirement> RequirementsList = {
    BattleRequirements,
    VillageRequirements,
    PalaceRequirements
};
TArray<FString> SceneNames = SceneGenerator->GenerateMultipleScenes(RequirementsList);

// 批量導出場景
SceneGenerator->BatchExportScenes(SceneNames, TEXT("fbx"));
```

### 場景模板管理

```cpp
// 保存自定義模板
SceneGenerator->SaveSceneTemplate(TEXT("MyCustomBattle"), CustomRequirements);

// 加載模板
FSceneRequirement LoadedTemplate = SceneGenerator->LoadSceneTemplate(TEXT("MyCustomBattle"));

// 獲取可用模板
TArray<FString> AvailableTemplates = SceneGenerator->GetAvailableTemplates();
```

## 場景配置

### 場景類型

```cpp
enum class ESceneType : uint8
{
    Battle,         // 戰鬥場景
    Village,        // 村莊場景
    Palace,         // 宮殿場景
    Forest,         // 森林場景
    Mountain,       // 山地場景
    River,          // 河流場景
    Desert,         // 沙漠場景
    City,           // 城市場景
    Temple,         // 寺廟場景
    Battlefield,    // 戰場場景
    Custom          // 自定義場景
};
```

### 複雜度級別

```cpp
enum class ESceneComplexity : uint8
{
    Simple,         // 簡單場景 - 快速生成，資產較少
    Medium,         // 中等場景 - 平衡質量和速度
    Complex,        // 複雜場景 - 高質量，資產豐富
    Epic            // 史詩場景 - 最高質量，大量資產
};
```

### 藝術風格

```cpp
enum class ESceneStyle : uint8
{
    Realistic,      // 寫實風格
    Stylized,       // 風格化
    Cartoon,        // 卡通風格
    Watercolor,     // 水彩風格
    Ink,            // 水墨風格
    OilPainting,    // 油畫風格
    PixelArt,       // 像素藝術
    LowPoly,        // 低多邊形
    CelShaded,      // 賽璐璐
    Photorealistic  // 照片級真實
};
```

## 事件系統

### 場景生成事件

```cpp
// 綁定事件
SceneGenerator->OnSceneGenerationStarted.AddDynamic(this, &AMyGameMode::OnSceneGenerationStarted);
SceneGenerator->OnSceneGenerationProgress.AddDynamic(this, &AMyGameMode::OnSceneGenerationProgress);
SceneGenerator->OnSceneGenerationCompleted.AddDynamic(this, &AMyGameMode::OnSceneGenerationCompleted);
SceneGenerator->OnSceneGenerationFailed.AddDynamic(this, &AMyGameMode::OnSceneGenerationFailed);

// 事件處理函數
UFUNCTION()
void AMyGameMode::OnSceneGenerationStarted(const FString& SceneName, const FSceneRequirement& Requirements)
{
    UE_LOG(LogTemp, Log, TEXT("Started generating scene: %s"), *SceneName);
}

UFUNCTION()
void AMyGameMode::OnSceneGenerationProgress(const FString& SceneName, float Progress)
{
    UE_LOG(LogTemp, Log, TEXT("Scene %s progress: %.1f%%"), *SceneName, Progress * 100.0f);
}

UFUNCTION()
void AMyGameMode::OnSceneGenerationCompleted(const FString& SceneName, const FGeneratedScene& GeneratedScene)
{
    UE_LOG(LogTemp, Log, TEXT("Completed scene: %s"), *SceneName);
    // 處理完成的場景
}

UFUNCTION()
void AMyGameMode::OnSceneGenerationFailed(const FString& SceneName, const FString& ErrorMessage)
{
    UE_LOG(LogTemp, Error, TEXT("Failed to generate scene %s: %s"), *SceneName, *ErrorMessage);
}
```

## 性能監控

### 獲取系統狀態

```cpp
// 獲取系統統計信息
float AverageTime = SceneGenerator->GetAverageSceneGenerationTime();
int32 ActiveCount = SceneGenerator->GetActiveSceneGenerations();
int32 TotalCount = SceneGenerator->GetTotalGeneratedScenes();

// 獲取場景信息
FGeneratedScene Scene = SceneGenerator->GetGeneratedScene(SceneName);
float ComplexityScore = SceneGenerator->GetSceneComplexityScore(SceneName);
TArray<EAIContentType> AssetTypes = SceneGenerator->GetSceneAssetTypes(SceneName);
int32 AssetCount = SceneGenerator->GetSceneAssetCount(SceneName);
```

### 性能優化

```cpp
// 設置默認質量
SceneGenerator->SetDefaultQuality(EGenerationQuality::High);

// 設置默認風格
SceneGenerator->SetDefaultStyle(ESceneStyle::Photorealistic);

// 設置輸出目錄
SceneGenerator->SetOutputDirectory(TEXT("/Game/MyScenes/"));

// 優化所有場景
SceneGenerator->OptimizeAllScenes();

// 清理緩存
SceneGenerator->ClearSceneCache();
```

## 最佳實踐

### 1. 場景需求優化

- **明確描述**: 使用詳細的場景描述
- **合理複雜度**: 根據需求選擇合適的複雜度
- **風格一致性**: 保持場景風格的一致性
- **資產平衡**: 平衡不同類型的資產需求

### 2. 性能管理

- **異步生成**: 使用異步API避免阻塞
- **批量處理**: 使用批量操作提高效率
- **緩存利用**: 合理利用場景緩存
- **資源監控**: 監控系統資源使用

### 3. 場景組織

- **模板重用**: 創建和重用場景模板
- **命名規範**: 使用統一的命名規範
- **版本管理**: 為重要場景創建版本
- **分類管理**: 按類型組織場景

### 4. 質量控制

- **漸進生成**: 從簡單到複雜漸進生成
- **預覽驗證**: 使用預覽功能驗證效果
- **變體比較**: 生成多個變體進行比較
- **用戶反饋**: 收集用戶反饋進行改進

## 故障排除

### 常見問題

1. **生成失敗**
   - 檢查網絡連接
   - 驗證API密鑰
   - 檢查磁盤空間
   - 查看錯誤日誌

2. **質量問題**
   - 調整質量設置
   - 優化提示詞
   - 檢查風格設置
   - 重新生成場景

3. **性能問題**
   - 降低複雜度
   - 減少資產數量
   - 使用異步生成
   - 清理緩存

4. **預覽問題**
   - 檢查渲染設置
   - 驗證場景完整性
   - 重啟預覽
   - 檢查硬件支持

### 調試工具

```cpp
// 檢查場景狀態
FGeneratedScene Scene = SceneGenerator->GetGeneratedScene(SceneName);
if (!Scene.bIsComplete)
{
    UE_LOG(LogTemp, Warning, TEXT("Scene %s is not complete"), *SceneName);
}

// 檢查生成進度
float Progress = Scene.GenerationProgress;
UE_LOG(LogTemp, Log, TEXT("Scene %s progress: %.1f%%"), *SceneName, Progress * 100.0f);

// 檢查資產狀態
for (const auto& AssetPair : Scene.GeneratedAssets)
{
    UE_LOG(LogTemp, Log, TEXT("Asset %s: %s"), 
           *GetContentTypeString(AssetPair.Key), *AssetPair.Value);
}
```

## 擴展開發

### 自定義場景類型

```cpp
// 添加新的場景類型
enum class ESceneType : uint8
{
    // 現有類型...
    Underwater,     // 水下場景
    Space,          // 太空場景
    Cyberpunk,      // 賽博朋克場景
    Fantasy,        // 奇幻場景
    CustomType      // 自定義類型
};
```

### 自定義資產類型

```cpp
// 擴展資產類型
enum class EAIContentType : uint8
{
    // 現有類型...
    VFX,            // 視覺特效
    UI,             // 用戶界面
    Script,         // 腳本
    Data            // 數據文件
};
```

### 插件開發

1. **創建插件**: 基於UE插件系統創建擴展
2. **實現接口**: 實現場景生成接口
3. **註冊服務**: 向系統註冊新服務
4. **測試驗證**: 完整測試新功能

## 使用案例

### 案例1: 快速原型開發

```cpp
// 快速生成多個場景原型
TArray<FSceneRequirement> Prototypes = {
    SceneGenerator->GetBattleSceneTemplate(TEXT("Prototype battle")),
    SceneGenerator->GetVillageSceneTemplate(TEXT("Prototype village")),
    SceneGenerator->GetPalaceSceneTemplate(TEXT("Prototype palace"))
};

TArray<FString> PrototypeScenes = SceneGenerator->GenerateMultipleScenes(Prototypes);
```

### 案例2: 場景變體測試

```cpp
// 為關卡生成多個變體
FSceneRequirement LevelRequirements = SceneGenerator->GetBattleSceneTemplate(TEXT("Level 1 boss battle"));
TArray<FSceneRequirement> LevelVariants = SceneGenerator->GenerateSceneVariants(LevelRequirements, 3);

// 選擇最佳變體
for (const FSceneRequirement& Variant : LevelVariants)
{
    FString VariantName = SceneGenerator->GenerateScene(Variant);
    // 預覽和評估變體
}
```

### 案例3: 動態場景生成

```cpp
// 根據遊戲狀態動態生成場景
FString GenerateDynamicScene(const FString& GameContext)
{
    TArray<FSceneRequirement> Recommendations = SceneGenerator->GetRecommendedScenes(GameContext);
    
    if (Recommendations.Num() > 0)
    {
        FSceneRequirement Optimized = SceneGenerator->OptimizeSceneRequirements(Recommendations[0]);
        return SceneGenerator->GenerateScene(Optimized);
    }
    
    return TEXT("");
}
```

## 更新日誌

### v1.0.0 (2026-03-22)
- 初始版本發布
- 支持基本場景生成功能
- 集成AI內容生成系統
- 實現場景預覽和導出

### 未來計劃
- 支持更多場景類型
- 增強場景編輯功能
- 改進性能優化
- 擴展平台支持

## 支持和聯繫

- **文檔**: 查看詳細API文檔
- **社區**: 加入開發者社區
- **問題反饋**: 提交問題和建議
- **技術支持**: 聯繫技術支持團隊

---

*本指南將隨系統更新持續完善，建議定期查看最新版本。*
