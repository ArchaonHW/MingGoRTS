#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "Engine/StaticMesh.h"
#include "Sound/SoundBase.h"
#include "Materials/MaterialInterface.h"
#include "Components/StaticMeshComponent.h"
#include "MingAIGeneratedContentSystem.generated.h"

class UTextureRenderTarget2D;
class USceneCaptureComponent2D;
class UMaterialInstanceDynamic;
class UAudioComponent;

UENUM(BlueprintType)
enum class EAIGenerationType : uint8
{
    Image,          // 圖像生成
    Video,          // 影片生成  
    Music,          // 音樂生成
    SoundEffect,    // 音效生成
    Texture,        // 貼圖生成
    Mesh,           // 模型生成
    Material,       // 材質生成
    Animation,      // 動畫生成
    Level,          // 關卡生成
    UI,             // UI生成
    Text,           // 文本生成
    Voice           // 語音生成
};

UENUM(BlueprintType)
enum class EAIModelProvider : uint8
{
    StableDiffusion,    // Stable Diffusion
    DALL_E,            // DALL-E
    Midjourney,        // Midjourney
    AIVA,              // AIVA (音樂)
    ElevenLabs,        // ElevenLabs (語音)
    OpenAI,            // OpenAI GPT
    Local,             // 本地模型
    Custom             // 自定義模型
};

UENUM(BlueprintType)
enum class EGenerationQuality : uint8
{
    Draft,          // 草稿質量
    Standard,       // 標準質量
    High,           // 高質量
    Ultra,          // 超高質量
    Custom          // 自定義質量
};

USTRUCT(BlueprintType)
struct FAIGenerationRequest
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "AI Generation")
    FString RequestID;

    UPROPERTY(BlueprintReadWrite, Category = "AI Generation")
    EAIGenerationType GenerationType;

    UPROPERTY(BlueprintReadWrite, Category = "AI Generation")
    EAIModelProvider ModelProvider;

    UPROPERTY(BlueprintReadWrite, Category = "AI Generation")
    FString Prompt;

    UPROPERTY(BlueprintReadWrite, Category = "AI Generation")
    FString NegativePrompt;

    UPROPERTY(BlueprintReadWrite, Category = "AI Generation")
    EGenerationQuality Quality;

    UPROPERTY(BlueprintReadWrite, Category = "AI Generation")
    int32 Width;

    UPROPERTY(BlueprintReadWrite, Category = "AI Generation")
    int32 Height;

    UPROPERTY(BlueprintReadWrite, Category = "AI Generation")
    float Duration; // 用於音頻/視頻

    UPROPERTY(BlueprintReadWrite, Category = "AI Generation")
    TMap<FString, FString> AdditionalParameters;

    UPROPERTY(BlueprintReadWrite, Category = "AI Generation")
    bool bIsAsync;

    UPROPERTY(BlueprintReadWrite, Category = "AI Generation")
    FString OutputPath;

    FAIGenerationRequest()
    {
        RequestID = TEXT("");
        GenerationType = EAIGenerationType::Image;
        ModelProvider = EAIModelProvider::StableDiffusion;
        Prompt = TEXT("");
        NegativePrompt = TEXT("");
        Quality = EGenerationQuality::Standard;
        Width = 512;
        Height = 512;
        Duration = 5.0f;
        bIsAsync = true;
        OutputPath = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FAIGenerationResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "AI Generation")
    FString RequestID;

    UPROPERTY(BlueprintReadOnly, Category = "AI Generation")
    bool bSuccess;

    UPROPERTY(BlueprintReadOnly, Category = "AI Generation")
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly, Category = "AI Generation")
    UTexture2D* GeneratedTexture;

    UPROPERTY(BlueprintReadOnly, Category = "AI Generation")
    USoundBase* GeneratedSound;

    UPROPERTY(BlueprintReadOnly, Category = "AI Generation")
    UStaticMesh* GeneratedMesh;

    UPROPERTY(BlueprintReadOnly, Category = "AI Generation")
    UMaterialInterface* GeneratedMaterial;

    UPROPERTY(BlueprintReadOnly, Category = "AI Generation")
    FString GeneratedFilePath;

    UPROPERTY(BlueprintReadOnly, Category = "AI Generation")
    float GenerationTime;

    UPROPERTY(BlueprintReadOnly, Category = "AI Generation")
    TMap<FString, FString> Metadata;

    FAIGenerationResult()
    {
        RequestID = TEXT("");
        bSuccess = false;
        ErrorMessage = TEXT("");
        GeneratedTexture = nullptr;
        GeneratedSound = nullptr;
        GeneratedMesh = nullptr;
        GeneratedMaterial = nullptr;
        GeneratedFilePath = TEXT("");
        GenerationTime = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FAIGameSceneSetup
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "AI Scene Setup")
    FString SceneName;

    UPROPERTY(BlueprintReadWrite, Category = "AI Scene Setup")
    FString SceneDescription;

    UPROPERTY(BlueprintReadWrite, Category = "AI Scene Setup")
    FString TimeOfDay;

    UPROPERTY(BlueprintReadWrite, Category = "AI Scene Setup")
    FString Weather;

    UPROPERTY(BlueprintReadWrite, Category = "AI Scene Setup")
    FString Location;

    UPROPERTY(BlueprintReadWrite, Category = "AI Scene Setup")
    FString Mood;

    UPROPERTY(BlueprintReadWrite, Category = "AI Scene Setup")
    TArray<FString> RequiredAssets;

    UPROPERTY(BlueprintReadWrite, Category = "AI Scene Setup")
    bool bAutoGenerateAssets;

    UPROPERTY(BlueprintReadWrite, Category = "AI Scene Setup")
    bool bCreateLighting;

    UPROPERTY(BlueprintReadWrite, Category = "AI Scene Setup")
    bool bAddEffects;

    FAIGameSceneSetup()
    {
        SceneName = TEXT("");
        SceneDescription = TEXT("");
        TimeOfDay = TEXT("day");
        Weather = TEXT("clear");
        Location = TEXT("countryside");
        Mood = TEXT("peaceful");
        bAutoGenerateAssets = true;
        bCreateLighting = true;
        bAddEffects = true;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIGenerationCompleted, const FAIGenerationResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIGenerationProgress, const FString&, RequestID, float, Progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSceneGenerated, const FString&, SceneName);

/**
 * AI 內容生成系統
 * 自動生成圖像、影片、音樂、音效等素材並建立遊戲畫面
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingAIGeneratedContentSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingAIGeneratedContentSystem();

    // 初始化系統
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void InitializeAIGenerationSystem();

    // 圖像生成
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateImage(const FString& Prompt, const FString& NegativePrompt = TEXT(""), int32 Width = 512, int32 Height = 512, EGenerationQuality Quality = EGenerationQuality::Standard);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateImageAsync(const FAIGenerationRequest& Request);

    // 影片生成
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateVideo(const FString& Prompt, float Duration = 5.0f, int32 FPS = 30, EGenerationQuality Quality = EGenerationQuality::Standard);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateVideoAsync(const FAIGenerationRequest& Request);

    // 音樂生成
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateMusic(const FString& Style, float Duration = 30.0f, EGenerationQuality Quality = EGenerationQuality::Standard);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateMusicAsync(const FAIGenerationRequest& Request);

    // 音效生成
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateSoundEffect(const FString& Description, float Duration = 2.0f, EGenerationQuality Quality = EGenerationQuality::Standard);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateSoundEffectAsync(const FAIGenerationRequest& Request);

    // 語音生成
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateVoice(const FString& Text, const FString& VoiceID = TEXT(""), EGenerationQuality Quality = EGenerationQuality::Standard);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateVoiceAsync(const FAIGenerationRequest& Request);

    // 貼圖生成
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    UTexture2D* GenerateTexture(const FString& Description, int32 Width = 512, int32 Height = 512);

    // 模型生成
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    UStaticMesh* GenerateMesh(const FString& Description, float Complexity = 0.5f);

    // 材質生成
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    UMaterialInterface* GenerateMaterial(const FString& Description, EGenerationQuality Quality = EGenerationQuality::Standard);

    // 動畫生成
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateAnimation(const FString& Description, float Duration = 5.0f);

    // 關卡生成
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateLevel(const FString& Description, int32 Size = 1024);

    // UI生成
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateUI(const FString& Description, const FString& UIType = TEXT("panel"));

    // 文本生成
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateText(const FString& Prompt, const FString& Style = TEXT("narrative"));

    // 遊戲場景生成
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void GenerateGameScene(const FAIGameSceneSetup& SceneSetup);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void GenerateGameSceneAsync(const FAIGameSceneSetup& SceneSetup);

    // 場景渲染
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    UTexture2D* RenderSceneToTexture(const FString& SceneName, int32 Width = 1920, int32 Height = 1080);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString RenderSceneToVideo(const FString& SceneName, float Duration = 10.0f, int32 FPS = 30);

    // 批量生成
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    TArray<FString> BatchGenerateAssets(const TArray<FAIGenerationRequest>& Requests);

    // 資產管理
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void SaveGeneratedAsset(const FString& RequestID, const FString& AssetPath);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    UTexture2D* LoadGeneratedTexture(const FString& AssetPath);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    USoundBase* LoadGeneratedSound(const FString& AssetPath);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    UStaticMesh* LoadGeneratedMesh(const FString& AssetPath);

    // 狀態查詢
    UFUNCTION(BlueprintPure, Category = "AI Content Generation")
    bool IsGenerationInProgress(const FString& RequestID) const;

    UFUNCTION(BlueprintPure, Category = "AI Content Generation")
    float GetGenerationProgress(const FString& RequestID) const;

    UFUNCTION(BlueprintPure, Category = "AI Content Generation")
    FAIGenerationResult GetGenerationResult(const FString& RequestID) const;

    UFUNCTION(BlueprintPure, Category = "AI Content Generation")
    TArray<FString> GetActiveRequests() const;

    // 配置管理
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void SetModelProvider(EAIModelProvider Provider, const FString& APIKey = TEXT(""));

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void SetDefaultQuality(EGenerationQuality Quality);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void SetOutputDirectory(const FString& Directory);

    // 預設模板
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void LoadGenerationPresets();

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FAIGenerationRequest GetPresetRequest(const FString& PresetName) const;

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void SavePresetRequest(const FString& PresetName, const FAIGenerationRequest& Request);

    // 性能監控
    UFUNCTION(BlueprintPure, Category = "AI Content Generation")
    float GetAverageGenerationTime(EAIGenerationType GenerationType) const;

    UFUNCTION(BlueprintPure, Category = "AI Content Generation")
    int32 GetTotalGeneratedCount(EAIGenerationType GenerationType) const;

    UFUNCTION(BlueprintPure, Category = "AI Content Generation")
    float GetSystemLoad() const;

    // 清理和維護
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void ClearCache();

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void CancelGeneration(const FString& RequestID);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void CancelAllGenerations();

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "AI Generation Events")
    FOnAIGenerationCompleted OnAIGenerationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "AI Generation Events")
    FOnAIGenerationProgress OnAIGenerationProgress;

    UPROPERTY(BlueprintAssignable, Category = "AI Generation Events")
    FOnSceneGenerated OnSceneGenerated;

protected:
    // 系統組件
    UPROPERTY()
    TMap<EAIModelProvider, FString> APIKeys;

    UPROPERTY()
    TMap<FString, FAIGenerationRequest> ActiveRequests;

    UPROPERTY()
    TMap<FString, FAIGenerationResult> GenerationResults;

    UPROPERTY()
    TMap<FString, float> RequestProgress;

    UPROPERTY()
    TMap<FString, FAIGameSceneSetup> SceneSetups;

    // 設置
    UPROPERTY(BlueprintReadWrite, Category = "AI Generation Settings")
    EAIModelProvider DefaultModelProvider;

    UPROPERTY(BlueprintReadWrite, Category = "AI Generation Settings")
    EGenerationQuality DefaultQuality;

    UPROPERTY(BlueprintReadWrite, Category = "AI Generation Settings")
    FString OutputDirectory;

    UPROPERTY(BlueprintReadWrite, Category = "AI Generation Settings")
    int32 MaxConcurrentGenerations;

    UPROPERTY(BlueprintReadWrite, Category = "AI Generation Settings")
    bool bEnableCache;

    UPROPERTY(BlueprintReadWrite, Category = "AI Generation Settings")
    bool bAutoSaveResults;

    UPROPERTY(BlueprintReadWrite, Category = "AI Generation Settings")
    float CacheExpirationTime;

    // 狀態
    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    int32 CurrentGenerationCount;

    // 統計
    UPROPERTY()
    TMap<EAIGenerationType, float> AverageGenerationTimes;

    UPROPERTY()
    TMap<EAIGenerationType, int32> TotalGeneratedCounts;

    // 內部函數
    void InitializeDefaultPresets();
    FString GenerateRequestID();
    void ProcessGenerationRequest(const FAIGenerationRequest& Request);
    void UpdateGenerationProgress(const FString& RequestID, float Progress);
    void CompleteGeneration(const FString& RequestID, const FAIGenerationResult& Result);
    void NotifyGenerationCompleted(const FAIGenerationResult& Result);
    void NotifyGenerationProgress(const FString& RequestID, float Progress);
    void NotifySceneGenerated(const FString& SceneName);

    // 具體生成實現
    UTexture2D* GenerateImageInternal(const FAIGenerationRequest& Request);
    USoundBase* GenerateMusicInternal(const FAIGenerationRequest& Request);
    USoundBase* GenerateSoundEffectInternal(const FAIGenerationRequest& Request);
    UStaticMesh* GenerateMeshInternal(const FAIGenerationRequest& Request);
    UMaterialInterface* GenerateMaterialInternal(const FAIGenerationRequest& Request);

    // 場景生成輔助
    void GenerateSceneLighting(const FAIGameSceneSetup& SceneSetup);
    void GenerateSceneEnvironment(const FAIGameSceneSetup& SceneSetup);
    void GenerateSceneEffects(const FAIGameSceneSetup& SceneSetup);
    void SetupSceneCamera(const FString& SceneName);

private:
    // 輔助函數
    FString GetModelAPIEndpoint(EAIModelProvider Provider) const;
    FString BuildAPIRequest(const FAIGenerationRequest& Request) const;
    FAIGenerationResult ParseAPIResponse(const FString& Response, const FString& RequestID) const;
    bool ValidateRequest(const FAIGenerationRequest& Request) const;
    void CacheResult(const FAIGenerationResult& Result);
    FAIGenerationResult LoadFromCache(const FAIGenerationRequest& Request) const;
    void UpdateStatistics(EAIGenerationType GenerationType, float GenerationTime);
};
