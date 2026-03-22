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
    Image,          // ?ñÂ??üÊ?
    Video,          // ÂΩ±Á??üÊ?  
    Music,          // ?≥Ê??üÊ?
    SoundEffect,    // ?≥Ê??üÊ?
    Texture,        // Ë≤ºÂ??üÊ?
    Mesh,           // Ê®°Â??üÊ?
    Material,       // ?êË≥™?üÊ?
    Animation,      // ?ïÁï´?üÊ?
    Level,          // ?úÂç°?üÊ?
    UI,             // UI?üÊ?
    Text,           // ?áÊú¨?üÊ?
    Voice           // Ë™ûÈü≥?üÊ?
};

UENUM(BlueprintType)
enum class EAIModelProvider : uint8
{
    StableDiffusion,    // Stable Diffusion
    DALL_E,            // DALL-E
    Midjourney,        // Midjourney
    AIVA,              // AIVA (?≥Ê?)
    ElevenLabs,        // ElevenLabs (Ë™ûÈü≥)
    OpenAI,            // OpenAI GPT
    Local,             // ?¨Âú∞Ê®°Â?
    Custom             // ?™Â?Áæ©Ê®°??};

UENUM(BlueprintType)
enum class EGenerationQuality : uint8
{
    Draft,          // ?âÁ®øË≥™È?
    Standard,       // Ê®ôÊ?Ë≥™È?
    High,           // È´òË≥™??    Ultra,          // Ë∂ÖÈ?Ë≥™È?
    Custom          // ?™Â?Áæ©Ë≥™??};

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
    float Duration; // ?®Êñº?≥È†ª/Ë¶ñÈ†ª

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
 * AI ?ßÂÆπ?üÊ?Á≥ªÁµ±
 * ?™Â??üÊ??ñÂ??ÅÂΩ±?á„ÄÅÈü≥Ê®Ç„ÄÅÈü≥?àÁ?Á¥†Ê?‰∏¶Âª∫Á´ãÈ??≤Áï´?? */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingAIGeneratedContentSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingAIGeneratedContentSystem();

    // ?ùÂ??ñÁ≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void InitializeAIGenerationSystem();

    // ?ñÂ??üÊ?
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateImage(const FString& Prompt, const FString& NegativePrompt = TEXT(""), int32 Width = 512, int32 Height = 512, EGenerationQuality Quality = EGenerationQuality::Standard);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateImageAsync(const FAIGenerationRequest& Request);

    // ÂΩ±Á??üÊ?
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateVideo(const FString& Prompt, float Duration = 5.0f, int32 FPS = 30, EGenerationQuality Quality = EGenerationQuality::Standard);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateVideoAsync(const FAIGenerationRequest& Request);

    // ?≥Ê??üÊ?
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateMusic(const FString& Style, float Duration = 30.0f, EGenerationQuality Quality = EGenerationQuality::Standard);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateMusicAsync(const FAIGenerationRequest& Request);

    // ?≥Ê??üÊ?
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateSoundEffect(const FString& Description, float Duration = 2.0f, EGenerationQuality Quality = EGenerationQuality::Standard);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateSoundEffectAsync(const FAIGenerationRequest& Request);

    // Ë™ûÈü≥?üÊ?
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateVoice(const FString& Text, const FString& VoiceID = TEXT(""), EGenerationQuality Quality = EGenerationQuality::Standard);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateVoiceAsync(const FAIGenerationRequest& Request);

    // Ë≤ºÂ??üÊ?
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    UTexture2D* GenerateTexture(const FString& Description, int32 Width = 512, int32 Height = 512);

    // Ê®°Â??üÊ?
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    UStaticMesh* GenerateMesh(const FString& Description, float Complexity = 0.5f);

    // ?êË≥™?üÊ?
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    UMaterialInterface* GenerateMaterial(const FString& Description, EGenerationQuality Quality = EGenerationQuality::Standard);

    // ?ïÁï´?üÊ?
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateAnimation(const FString& Description, float Duration = 5.0f);

    // ?úÂç°?üÊ?
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateLevel(const FString& Description, int32 Size = 1024);

    // UI?üÊ?
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateUI(const FString& Description, const FString& UIType = TEXT("panel"));

    // ?áÊú¨?üÊ?
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString GenerateText(const FString& Prompt, const FString& Style = TEXT("narrative"));

    // ?äÊà≤?¥ÊôØ?üÊ?
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void GenerateGameScene(const FAIGameSceneSetup& SceneSetup);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void GenerateGameSceneAsync(const FAIGameSceneSetup& SceneSetup);

    // ?¥ÊôØÊ∏≤Ê?
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    UTexture2D* RenderSceneToTexture(const FString& SceneName, int32 Width = 1920, int32 Height = 1080);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FString RenderSceneToVideo(const FString& SceneName, float Duration = 10.0f, int32 FPS = 30);

    // ?πÈ??üÊ?
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    TArray<FString> BatchGenerateAssets(const TArray<FAIGenerationRequest>& Requests);

    // Ë≥áÁî¢ÁÆ°Á?
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void SaveGeneratedAsset(const FString& RequestID, const FString& AssetPath);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    UTexture2D* LoadGeneratedTexture(const FString& AssetPath);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    USoundBase* LoadGeneratedSound(const FString& AssetPath);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    UStaticMesh* LoadGeneratedMesh(const FString& AssetPath);

    // ?Ä?ãÊü•Ë©?    UFUNCTION(BlueprintPure, Category = "AI Content Generation")
    bool IsGenerationInProgress(const FString& RequestID) const;

    UFUNCTION(BlueprintPure, Category = "AI Content Generation")
    float GetGenerationProgress(const FString& RequestID) const;

    UFUNCTION(BlueprintPure, Category = "AI Content Generation")
    FAIGenerationResult GetGenerationResult(const FString& RequestID) const;

    UFUNCTION(BlueprintPure, Category = "AI Content Generation")
    TArray<FString> GetActiveRequests() const;

    // ?çÁΩÆÁÆ°Á?
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void SetModelProvider(EAIModelProvider Provider, const FString& APIKey = TEXT(""));

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void SetDefaultQuality(EGenerationQuality Quality);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void SetOutputDirectory(const FString& Directory);

    // ?êË®≠Ê®°Êùø
    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void LoadGenerationPresets();

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    FAIGenerationRequest GetPresetRequest(const FString& PresetName) const;

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void SavePresetRequest(const FString& PresetName, const FAIGenerationRequest& Request);

    // ?ßËÉΩ??éß
    UFUNCTION(BlueprintPure, Category = "AI Content Generation")
    float GetAverageGenerationTime(EAIGenerationType GenerationType) const;

    UFUNCTION(BlueprintPure, Category = "AI Content Generation")
    int32 GetTotalGeneratedCount(EAIGenerationType GenerationType) const;

    UFUNCTION(BlueprintPure, Category = "AI Content Generation")
    float GetSystemLoad() const;

    // Ê∏ÖÁ??åÁ∂≠Ë≠?    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void ClearCache();

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void CancelGeneration(const FString& RequestID);

    UFUNCTION(BlueprintCallable, Category = "AI Content Generation")
    void CancelAllGenerations();

    // ‰∫ã‰ª∂
    UPROPERTY(BlueprintAssignable, Category = "AI Generation Events")
    FOnAIGenerationCompleted OnAIGenerationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "AI Generation Events")
    FOnAIGenerationProgress OnAIGenerationProgress;

    UPROPERTY(BlueprintAssignable, Category = "AI Generation Events")
    FOnSceneGenerated OnSceneGenerated;

protected:
    // Á≥ªÁµ±ÁµÑ‰ª∂
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

    // Ë®≠ÁΩÆ
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

    // ?Ä??    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    int32 CurrentGenerationCount;

    // Áµ±Ë?
    UPROPERTY()
    TMap<EAIGenerationType, float> AverageGenerationTimes;

    UPROPERTY()
    TMap<EAIGenerationType, int32> TotalGeneratedCounts;

    // ?ßÈÉ®?ΩÊï∏
    void InitializeDefaultPresets();
    FString GenerateRequestID();
    void ProcessGenerationRequest(const FAIGenerationRequest& Request);
    void UpdateGenerationProgress(const FString& RequestID, float Progress);
    void CompleteGeneration(const FString& RequestID, const FAIGenerationResult& Result);
    void NotifyGenerationCompleted(const FAIGenerationResult& Result);
    void NotifyGenerationProgress(const FString& RequestID, float Progress);
    void NotifySceneGenerated(const FString& SceneName);

    // ?∑È??üÊ?ÂØ¶Áèæ
    UTexture2D* GenerateImageInternal(const FAIGenerationRequest& Request);
    USoundBase* GenerateMusicInternal(const FAIGenerationRequest& Request);
    USoundBase* GenerateSoundEffectInternal(const FAIGenerationRequest& Request);
    UStaticMesh* GenerateMeshInternal(const FAIGenerationRequest& Request);
    UMaterialInterface* GenerateMaterialInternal(const FAIGenerationRequest& Request);

    // ?¥ÊôØ?üÊ?ËºîÂä©
    void GenerateSceneLighting(const FAIGameSceneSetup& SceneSetup);
    void GenerateSceneEnvironment(const FAIGameSceneSetup& SceneSetup);
    void GenerateSceneEffects(const FAIGameSceneSetup& SceneSetup);
    void SetupSceneCamera(const FString& SceneName);

private:
    // ËºîÂä©?ΩÊï∏
    FString GetModelAPIEndpoint(EAIModelProvider Provider) const;
    FString BuildAPIRequest(const FAIGenerationRequest& Request) const;
    FAIGenerationResult ParseAPIResponse(const FString& Response, const FString& RequestID) const;
    bool ValidateRequest(const FAIGenerationRequest& Request) const;
    void CacheResult(const FAIGenerationResult& Result);
    FAIGenerationResult LoadFromCache(const FAIGenerationRequest& Request) const;
    void UpdateStatistics(EAIGenerationType GenerationType, float GenerationTime);
};
