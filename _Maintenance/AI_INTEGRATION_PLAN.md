# AI集成系統規劃

## 基於 api_spec.md 的 AI 集成架構

### 1. Stable Diffusion 圖像生成集成

#### 1.1 C++ HTTP 客戶端
```cpp
#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"

struct FStableDiffusionRequest
{
    FString Prompt;
    FString NegativePrompt;
    int32 Width = 512;
    int32 Height = 768;
    int32 Steps = 30;
    FString SamplerName = TEXT("DPM++ 2M Karras");
};

struct FStableDiffusionResponse
{
    TArray<FString> Images;
    FString Parameters;
    FString Info;
};

class MINGGORTS_API UStableDiffusionManager : public UGameInstanceSubsystem
{
public:
    UFUNCTION(BlueprintCallable, Category = "AI Generation")
    void GenerateCharacterPortrait(const FString& CharacterDescription);
    
    UFUNCTION(BlueprintCallable, Category = "AI Generation")
    void GenerateBuildingTexture(const FString& BuildingType);
    
private:
    void OnImageRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response);
    void OnImageRequestFailed(FHttpRequestPtr Request, FHttpResponsePtr Response);
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Generation")
    FString APIEndpoint = TEXT("http://localhost:7860/sdapi/v1/txt2img");
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Generation")
    bool bUseLocalAPI = true;
};
```

#### 1.2 動態紋理生成
```cpp
// 在單位創建時生成紋理
void ARTSUnit::BeginPlay()
{
    Super::BeginPlay();
    
    if (UStableDiffusionManager* SDManager = GetGameInstance()->GetSubsystem<UStableDiffusionManager>())
    {
        FString Prompt = FString::Printf(TEXT("民國風格的%s單位"), *UnitType);
        SDManager->GenerateCharacterPortrait(Prompt);
    }
}
```

### 2. AIVA 音樂生成集成

#### 2.1 音樂管理器
```cpp
#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundBase.h"

struct FAIVACompositionRequest
{
    FString Title;
    FString Style = TEXT("chinese-orchestral-epic");
    int32 Duration = 180;
    FString Mood = TEXT("dramatic");
};

class MINGGORTS_API UAIVAMusicManager : public UGameInstanceSubsystem
{
public:
    UFUNCTION(BlueprintCallable, Category = "AI Music")
    void GenerateBackgroundMusic(const FString& GameSituation);
    
    UFUNCTION(BlueprintCallable, Category = "AI Music")
    void GenerateBattleMusic(const FString& BattleIntensity);
    
    UFUNCTION(BlueprintCallable, Category = "AI Music")
    void PlayGeneratedMusic();
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Music")
    FString APIEndpoint = TEXT("https://api.aiva.ai/v1/compositions");
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Music")
    FString APIKey;
    
    UPROPERTY(BlueprintReadOnly, Category = "AI Music")
    class USoundBase* CurrentMusic;
};
```

#### 2.2 動態音樂生成
```cpp
// 根據遊戲狀態生成音樂
void UAIVAMusicManager::GenerateMusicForGameState(const FString& GameState)
{
    FAIVACompositionRequest Request;
    
    if (GameState == TEXT("peaceful"))
    {
        Request.Title = TEXT("MingGoRTS - 和平建設");
        Request.Mood = TEXT("calm");
    }
    else if (GameState == TEXT("battle"))
    {
        Request.Title = TEXT("MingGoRTS - 激烈戰鬥");
        Request.Mood = TEXT("dramatic");
    }
    
    // 發送API請求
    SendMusicRequest(Request);
}
```

### 3. AI 行為系統

#### 3.1 AI 決策樹
```cpp
UENUM(BlueprintType)
enum class EAIState : uint8
{
    Idle,
    Moving,
    Attacking,
    Gathering,
    Building
};

USTRUCT(BlueprintType)
struct FAIDecision
{
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EAIState RecommendedAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Priority;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;
};

class MINGGORTS_API UAIDecisionMaker : public UGameInstanceSubsystem
{
public:
    UFUNCTION(BlueprintCallable, Category = "AI")
    FAIDecision MakeDecision(const AActor* AIUnit);
    
    UFUNCTION(BlueprintCallable, Category = "AI")
    TArray<FAIDecision> EvaluatePossibleActions(const AActor* AIUnit);
};
```

#### 3.2 機器學習集成
```cpp
// 使用訓練好的模型進行決策
class MINGGORTS_API UAIMachineLearning : public UGameInstanceSubsystem
{
public:
    UFUNCTION(BlueprintCallable, Category = "AI")
    float PredictBattleOutcome(const TArray<AActor*>& FriendlyUnits, const TArray<AActor*>& EnemyUnits);
    
    UFUNCTION(BlueprintCallable, Category = "AI")
    FVector OptimalResourceGatheringLocation(const AActor* AIUnit);
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FString ModelEndpoint = TEXT("http://localhost:8080/predict");
};
```

### 4. 內容生成管線

#### 4.1 生成管線管理
```cpp
class MINGGORTS_API UContentPipelineManager : public UGameInstanceSubsystem
{
public:
    UFUNCTION(BlueprintCallable, Category = "Content Pipeline")
    void GenerateUnitVariations(const FString& BaseUnitType, int32 VariationCount);
    
    UFUNCTION(BlueprintCallable, Category = "Content Pipeline")
    void GenerateBuildingVariations(const FString& BaseBuildingType);
    
    UFUNCTION(BlueprintCallable, Category = "Content Pipeline")
    void GenerateTerrainTextures(const FString& BiomeType, int32 TextureCount);
    
private:
    void QueueImageGeneration(const FString& Prompt, TFunction<void(const FString&)> Callback);
    void ProcessGenerationQueue();
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content Pipeline")
    int32 MaxConcurrentGenerations = 3;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content Pipeline")
    float GenerationTimeout = 30.0f;
};
```

### 5. 實現時間表

#### 階段 1: 基礎集成 (2週)
- Stable Diffusion HTTP 客戶端
- 基礎圖像生成功能
- 單位紋理動態生成

#### 階段 2: 音樂系統 (2週)
- AIVA API 集成
- 動態背景音樂生成
- 戰鬥音樂生成

#### 階段 3: AI 行為 (3週)
- AI 決策樹實現
- 機器學習集成
- 智能單位行為

#### 階段 4: 內容管線 (2週)
- 批量內容生成
- 生成管線優化
- 內容緩存系統

### 6. 技術要求

#### 6.1 性能指標
- 圖像生成時間：< 5秒
- 音樂生成時間：< 30秒
- AI 決策時間：< 100ms
- 並發生成：支持最多3個同時任務

#### 6.2 錯誤處理
- API 調用失敗重試機制
- 網絡連接檢測
- 降級方案（預設資產）
- 用戶友好的錯誤訊息

#### 6.3 配置管理
- 可配置的 API 端點
- 本地/遠端 API 切換
- 生成參數調整
- 內容品質控制
