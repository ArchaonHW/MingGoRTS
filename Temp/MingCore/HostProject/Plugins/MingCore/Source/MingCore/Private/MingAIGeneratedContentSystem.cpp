#include "MingAIGeneratedContentSystem.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Engine/Texture2D.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Sound/SoundBase.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UMingAIGeneratedContentSystem::UMingAIGeneratedContentSystem()
    : DefaultModelProvider(EAIModelProvider::StableDiffusion)
    , DefaultQuality(EGenerationQuality::Standard)
    , OutputDirectory(TEXT("/Game/AIGenerated/"))
    , MaxConcurrentGenerations(3)
    , bEnableCache(true)
    , bAutoSaveResults(true)
    , CacheExpirationTime(3600.0f)
    , bIsInitialized(false)
    , CurrentGenerationCount(0)
{
}

void UMingAIGeneratedContentSystem::InitializeAIGenerationSystem()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("AI Generation System already initialized"));
        return;
    }

    // 設置默認API密鑰（從環境變量或配置文件讀取）
    APIKeys.Add(EAIModelProvider::StableDiffusion, TEXT("your-stable-diffusion-key"));
    APIKeys.Add(EAIModelProvider::DALL_E, TEXT("your-dalle-key"));
    APIKeys.Add(EAIModelProvider::Midjourney, TEXT("your-midjourney-key"));
    APIKeys.Add(EAIModelProvider::AIVA, TEXT("your-aiva-key"));
    APIKeys.Add(EAIModelProvider::ElevenLabs, TEXT("your-elevenlabs-key"));
    APIKeys.Add(EAIModelProvider::OpenAI, TEXT("your-openai-key"));

    // 初始化統計
    for (int32 i = 0; i < (int32)EAIGenerationType::Voice + 1; ++i)
    {
        EAIGenerationType Type = (EAIGenerationType)i;
        AverageGenerationTimes.Add(Type, 0.0f);
        TotalGeneratedCounts.Add(Type, 0);
    }

    // 加載預設
    InitializeDefaultPresets();

    // 確保輸出目錄存在
    FString FullOutputPath = FPaths::ProjectDir() / OutputDirectory;
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*FullOutputPath))
    {
        PlatformFile.CreateDirectoryTree(*FullOutputPath);
    }

    bIsInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("AI Generation System initialized"));
}

FString UMingAIGeneratedContentSystem::GenerateImage(const FString& Prompt, const FString& NegativePrompt, int32 Width, int32 Height, EGenerationQuality Quality)
{
    FAIGenerationRequest Request;
    Request.RequestID = GenerateRequestID();
    Request.GenerationType = EAIGenerationType::Image;
    Request.ModelProvider = DefaultModelProvider;
    Request.Prompt = Prompt;
    Request.NegativePrompt = NegativePrompt;
    Request.Width = Width;
    Request.Height = Height;
    Request.Quality = Quality;
    Request.bIsAsync = false;

    ProcessGenerationRequest(Request);
    return Request.RequestID;
}

FString UMingAIGeneratedContentSystem::GenerateImageAsync(const FAIGenerationRequest& Request)
{
    FAIGenerationRequest ModifiedRequest = Request;
    ModifiedRequest.RequestID = GenerateRequestID();
    ModifiedRequest.bIsAsync = true;

    ActiveRequests.Add(ModifiedRequest.RequestID, ModifiedRequest);
    RequestProgress.Add(ModifiedRequest.RequestID, 0.0f);

    // 異步處理
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, ModifiedRequest]()
    {
        ProcessGenerationRequest(ModifiedRequest);
    });

    return ModifiedRequest.RequestID;
}

FString UMingAIGeneratedContentSystem::GenerateVideo(const FString& Prompt, float Duration, int32 FPS, EGenerationQuality Quality)
{
    FAIGenerationRequest Request;
    Request.RequestID = GenerateRequestID();
    Request.GenerationType = EAIGenerationType::Video;
    Request.ModelProvider = DefaultModelProvider;
    Request.Prompt = Prompt;
    Request.Duration = Duration;
    Request.Quality = Quality;
    Request.bIsAsync = true;

    Request.AdditionalParameters.Add(TEXT("fps"), FString::FromInt(FPS));

    ActiveRequests.Add(Request.RequestID, Request);
    RequestProgress.Add(Request.RequestID, 0.0f);

    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request]()
    {
        ProcessGenerationRequest(Request);
    });

    return Request.RequestID;
}

FString UMingAIGeneratedContentSystem::GenerateMusic(const FString& Style, float Duration, EGenerationQuality Quality)
{
    FAIGenerationRequest Request;
    Request.RequestID = GenerateRequestID();
    Request.GenerationType = EAIGenerationType::Music;
    Request.ModelProvider = EAIModelProvider::AIVA;
    Request.Prompt = Style;
    Request.Duration = Duration;
    Request.Quality = Quality;
    Request.bIsAsync = true;

    ActiveRequests.Add(Request.RequestID, Request);
    RequestProgress.Add(Request.RequestID, 0.0f);

    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request]()
    {
        ProcessGenerationRequest(Request);
    });

    return Request.RequestID;
}

FString UMingAIGeneratedContentSystem::GenerateSoundEffect(const FString& Description, float Duration, EGenerationQuality Quality)
{
    FAIGenerationRequest Request;
    Request.RequestID = GenerateRequestID();
    Request.GenerationType = EAIGenerationType::SoundEffect;
    Request.ModelProvider = EAIModelProvider::StableDiffusion; // 使用支持音效的模型
    Request.Prompt = Description;
    Request.Duration = Duration;
    Request.Quality = Quality;
    Request.bIsAsync = true;

    ActiveRequests.Add(Request.RequestID, Request);
    RequestProgress.Add(Request.RequestID, 0.0f);

    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request]()
    {
        ProcessGenerationRequest(Request);
    });

    return Request.RequestID;
}

FString UMingAIGeneratedContentSystem::GenerateVoice(const FString& Text, const FString& VoiceID, EGenerationQuality Quality)
{
    FAIGenerationRequest Request;
    Request.RequestID = GenerateRequestID();
    Request.GenerationType = EAIGenerationType::Voice;
    Request.ModelProvider = EAIModelProvider::ElevenLabs;
    Request.Prompt = Text;
    Request.Quality = Quality;
    Request.bIsAsync = true;

    if (!VoiceID.IsEmpty())
    {
        Request.AdditionalParameters.Add(TEXT("voice_id"), VoiceID);
    }

    ActiveRequests.Add(Request.RequestID, Request);
    RequestProgress.Add(Request.RequestID, 0.0f);

    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request]()
    {
        ProcessGenerationRequest(Request);
    });

    return Request.RequestID;
}

UTexture2D* UMingAIGeneratedContentSystem::GenerateTexture(const FString& Description, int32 Width, int32 Height)
{
    FAIGenerationRequest Request;
    Request.RequestID = GenerateRequestID();
    Request.GenerationType = EAIGenerationType::Texture;
    Request.ModelProvider = DefaultModelProvider;
    Request.Prompt = Description;
    Request.Width = Width;
    Request.Height = Height;
    Request.bIsAsync = false;

    FAIGenerationResult Result;
    Result.RequestID = Request.RequestID;
    Result.bSuccess = false;

    // 檢查緩存
    if (bEnableCache)
    {
        Result = LoadFromCache(Request);
        if (Result.bSuccess)
        {
            return Result.GeneratedTexture;
        }
    }

    // 生成紋理
    UTexture2D* GeneratedTexture = GenerateImageInternal(Request);
    
    Result.bSuccess = (GeneratedTexture != nullptr);
    Result.GeneratedTexture = GeneratedTexture;

    if (Result.bSuccess)
    {
        CacheResult(Result);
        UpdateStatistics(EAIGenerationType::Texture, 0.0f);
    }

    return GeneratedTexture;
}

UStaticMesh* UMingAIGeneratedContentSystem::GenerateMesh(const FString& Description, float Complexity)
{
    FAIGenerationRequest Request;
    Request.RequestID = GenerateRequestID();
    Request.GenerationType = EAIGenerationType::Mesh;
    Request.ModelProvider = EAIModelProvider::StableDiffusion;
    Request.Prompt = Description;
    Request.AdditionalParameters.Add(TEXT("complexity"), FString::SanitizeFloat(Complexity));
    Request.bIsAsync = false;

    UStaticMesh* GeneratedMesh = GenerateMeshInternal(Request);
    
    if (GeneratedMesh)
    {
        UpdateStatistics(EAIGenerationType::Mesh, 0.0f);
    }

    return GeneratedMesh;
}

UMaterialInterface* UMingAIGeneratedContentSystem::GenerateMaterial(const FString& Description, EGenerationQuality Quality)
{
    FAIGenerationRequest Request;
    Request.RequestID = GenerateRequestID();
    Request.GenerationType = EAIGenerationType::Material;
    Request.ModelProvider = DefaultModelProvider;
    Request.Prompt = Description;
    Request.Quality = Quality;
    Request.bIsAsync = false;

    UMaterialInterface* GeneratedMaterial = GenerateMaterialInternal(Request);
    
    if (GeneratedMaterial)
    {
        UpdateStatistics(EAIGenerationType::Material, 0.0f);
    }

    return GeneratedMaterial;
}

FString UMingAIGeneratedContentSystem::GenerateAnimation(const FString& Description, float Duration)
{
    FAIGenerationRequest Request;
    Request.RequestID = GenerateRequestID();
    Request.GenerationType = EAIGenerationType::Animation;
    Request.ModelProvider = DefaultModelProvider;
    Request.Prompt = Description;
    Request.Duration = Duration;
    Request.bIsAsync = true;

    ActiveRequests.Add(Request.RequestID, Request);
    RequestProgress.Add(Request.RequestID, 0.0f);

    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request]()
    {
        ProcessGenerationRequest(Request);
    });

    return Request.RequestID;
}

FString UMingAIGeneratedContentSystem::GenerateLevel(const FString& Description, int32 Size)
{
    FAIGenerationRequest Request;
    Request.RequestID = GenerateRequestID();
    Request.GenerationType = EAIGenerationType::Level;
    Request.ModelProvider = EAIModelProvider::OpenAI;
    Request.Prompt = Description;
    Request.AdditionalParameters.Add(TEXT("size"), FString::FromInt(Size));
    Request.bIsAsync = true;

    ActiveRequests.Add(Request.RequestID, Request);
    RequestProgress.Add(Request.RequestID, 0.0f);

    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request]()
    {
        ProcessGenerationRequest(Request);
    });

    return Request.RequestID;
}

FString UMingAIGeneratedContentSystem::GenerateUI(const FString& Description, const FString& UIType)
{
    FAIGenerationRequest Request;
    Request.RequestID = GenerateRequestID();
    Request.GenerationType = EAIGenerationType::UI;
    Request.ModelProvider = DefaultModelProvider;
    Request.Prompt = Description;
    Request.AdditionalParameters.Add(TEXT("ui_type"), UIType);
    Request.bIsAsync = true;

    ActiveRequests.Add(Request.RequestID, Request);
    RequestProgress.Add(Request.RequestID, 0.0f);

    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request]()
    {
        ProcessGenerationRequest(Request);
    });

    return Request.RequestID;
}

FString UMingAIGeneratedContentSystem::GenerateText(const FString& Prompt, const FString& Style)
{
    FAIGenerationRequest Request;
    Request.RequestID = GenerateRequestID();
    Request.GenerationType = EAIGenerationType::Text;
    Request.ModelProvider = EAIModelProvider::OpenAI;
    Request.Prompt = Prompt;
    Request.AdditionalParameters.Add(TEXT("style"), Style);
    Request.bIsAsync = true;

    ActiveRequests.Add(Request.RequestID, Request);
    RequestProgress.Add(Request.RequestID, 0.0f);

    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request]()
    {
        ProcessGenerationRequest(Request);
    });

    return Request.RequestID;
}

void UMingAIGeneratedContentSystem::GenerateGameScene(const FAIGameSceneSetup& SceneSetup)
{
    FString SceneName = SceneSetup.SceneName.IsEmpty() ? GenerateRequestID() : SceneSetup.SceneName;
    SceneSetups.Add(SceneName, SceneSetup);

    // 生成場景所需的資產
    if (SceneSetup.bAutoGenerateAssets)
    {
        TArray<FAIGenerationRequest> AssetRequests;

        // 生成背景圖像
        FAIGenerationRequest BackgroundRequest;
        BackgroundRequest.RequestID = GenerateRequestID();
        BackgroundRequest.GenerationType = EAIGenerationType::Image;
        BackgroundRequest.Prompt = FString::Printf(TEXT("%s %s %s %s"), 
                                                   *SceneSetup.Location, 
                                                   *SceneSetup.TimeOfDay, 
                                                   *SceneSetup.Weather, 
                                                   *SceneSetup.Mood);
        BackgroundRequest.Width = 1920;
        BackgroundRequest.Height = 1080;
        AssetRequests.Add(BackgroundRequest);

        // 生成環境音樂
        FAIGenerationRequest MusicRequest;
        MusicRequest.RequestID = GenerateRequestID();
        MusicRequest.GenerationType = EAIGenerationType::Music;
        MusicRequest.Prompt = FString::Printf(TEXT("%s %s ambient music"), *SceneSetup.Mood, *SceneSetup.Location);
        MusicRequest.Duration = 60.0f;
        AssetRequests.Add(MusicRequest);

        // 生成音效
        FAIGenerationRequest SoundRequest;
        SoundRequest.RequestID = GenerateRequestID();
        SoundRequest.GenerationType = EAIGenerationType::SoundEffect;
        SoundRequest.Prompt = FString::Printf(TEXT("%s %s environmental sounds"), *SceneSetup.Location, *SceneSetup.Weather);
        SoundRequest.Duration = 10.0f;
        AssetRequests.Add(SoundRequest);

        // 批量生成資產
        BatchGenerateAssets(AssetRequests);
    }

    // 設置場景
    GenerateSceneLighting(SceneSetup);
    GenerateSceneEnvironment(SceneSetup);
    GenerateSceneEffects(SceneSetup);
    SetupSceneCamera(SceneName);

    NotifySceneGenerated(SceneName);
}

void UMingAIGeneratedContentSystem::GenerateGameSceneAsync(const FAIGameSceneSetup& SceneSetup)
{
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, SceneSetup]()
    {
        GenerateGameScene(SceneSetup);
    });
}

UTexture2D* UMingAIGeneratedContentSystem::RenderSceneToTexture(const FString& SceneName, int32 Width, int32 Height)
{
    // 創建場景捕獲組件
    USceneCaptureComponent2D* SceneCapture = NewObject<USceneCaptureComponent2D>();
    SceneCapture->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
    SceneCapture->TextureTarget = NewObject<UTextureRenderTarget2D>();
    SceneCapture->TextureTarget->InitAutoFormat(Width, Height);
    
    // 設置捕獲位置和角度
    SceneCapture->SetWorldLocation(FVector(0.0f, 0.0f, 500.0f));
    SceneCapture->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));
    
    // 捕獲場景
    SceneCapture->CaptureScene();
    
    // 獲取渲染結果
    UTexture2D* RenderedTexture = SceneCapture->TextureTarget;
    
    // 清理
    SceneCapture->DestroyComponent();
    
    return RenderedTexture;
}

FString UMingAIGeneratedContentSystem::RenderSceneToVideo(const FString& SceneName, float Duration, int32 FPS)
{
    FString VideoID = GenerateRequestID();
    
    // 設置視頻生成請求
    FAIGenerationRequest Request;
    Request.RequestID = VideoID;
    Request.GenerationType = EAIGenerationType::Video;
    Request.Prompt = FString::Printf(TEXT("Render scene %s to video"), *SceneName);
    Request.Duration = Duration;
    Request.AdditionalParameters.Add(TEXT("fps"), FString::FromInt(FPS));
    Request.AdditionalParameters.Add(TEXT("scene_name"), SceneName);
    
    ActiveRequests.Add(VideoID, Request);
    RequestProgress.Add(VideoID, 0.0f);
    
    // 異步處理視頻渲染
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Request]()
    {
        // 模擬視頻渲染過程
        float TotalFrames = Request.Duration * FPS;
        for (int32 Frame = 0; Frame < TotalFrames; ++Frame)
        {
            float Progress = (float)Frame / TotalFrames;
            UpdateGenerationProgress(Request.RequestID, Progress);
            
            // 渲染單幀
            RenderSceneToTexture(Request.AdditionalParameters.FindRef(TEXT("scene_name")), 1920, 1080);
            
            // 模擬渲染時間
            FPlatformProcess::Sleep(0.01f);
        }
        
        // 完成視頻生成
        FAIGenerationResult Result;
        Result.RequestID = Request.RequestID;
        Result.bSuccess = true;
        Result.GeneratedFilePath = FString::Printf(TEXT("%s_%s.mp4"), *Request.RequestID, *FDateTime::Now().ToString());
        
        CompleteGeneration(Request.RequestID, Result);
    });
    
    return VideoID;
}

TArray<FString> UMingAIGeneratedContentSystem::BatchGenerateAssets(const TArray<FAIGenerationRequest>& Requests)
{
    TArray<FString> RequestIDs;
    
    for (const FAIGenerationRequest& Request : Requests)
    {
        FString RequestID = GenerateRequestID();
        FAIGenerationRequest ModifiedRequest = Request;
        ModifiedRequest.RequestID = RequestID;
        
        ActiveRequests.Add(RequestID, ModifiedRequest);
        RequestProgress.Add(RequestID, 0.0f);
        RequestIDs.Add(RequestID);
        
        // 異步處理
        AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, ModifiedRequest]()
        {
            ProcessGenerationRequest(ModifiedRequest);
        });
    }
    
    return RequestIDs;
}

void UMingAIGeneratedContentSystem::SaveGeneratedAsset(const FString& RequestID, const FString& AssetPath)
{
    if (GenerationResults.Contains(RequestID))
    {
        const FAIGenerationResult& Result = GenerationResults[RequestID];
        
        // 保存資產到指定路徑
        FString FullPath = FPaths::ProjectDir() / AssetPath;
        
        if (Result.GeneratedTexture)
        {
            // 保存紋理
            UKismetRenderingLibrary::ExportRenderTarget(Result.GeneratedTexture, FullPath);
        }
        else if (Result.GeneratedSound)
        {
            // 保存音頻
            // 實現音頻保存邏輯
        }
        else if (Result.GeneratedMesh)
        {
            // 保存模型
            // 實現模型保存邏輯
        }
        
        UE_LOG(LogTemp, Log, TEXT("Saved generated asset %s to %s"), *RequestID, *AssetPath);
    }
}

UTexture2D* UMingAIGeneratedContentSystem::LoadGeneratedTexture(const FString& AssetPath)
{
    FString FullPath = FPaths::ProjectDir() / AssetPath;
    return LoadObject<UTexture2D>(nullptr, *FullPath);
}

USoundBase* UMingAIGeneratedContentSystem::LoadGeneratedSound(const FString& AssetPath)
{
    FString FullPath = FPaths::ProjectDir() / AssetPath;
    return LoadObject<USoundBase>(nullptr, *FullPath);
}

UStaticMesh* UMingAIGeneratedContentSystem::LoadGeneratedMesh(const FString& AssetPath)
{
    FString FullPath = FPaths::ProjectDir() / AssetPath;
    return LoadObject<UStaticMesh>(nullptr, *FullPath);
}

bool UMingAIGeneratedContentSystem::IsGenerationInProgress(const FString& RequestID) const
{
    return ActiveRequests.Contains(RequestID);
}

float UMingAIGeneratedContentSystem::GetGenerationProgress(const FString& RequestID) const
{
    if (RequestProgress.Contains(RequestID))
    {
        return RequestProgress[RequestID];
    }
    return 0.0f;
}

FAIGenerationResult UMingAIGeneratedContentSystem::GetGenerationResult(const FString& RequestID) const
{
    if (GenerationResults.Contains(RequestID))
    {
        return GenerationResults[RequestID];
    }
    return FAIGenerationResult();
}

TArray<FString> UMingAIGeneratedContentSystem::GetActiveRequests() const
{
    TArray<FString> ActiveRequestIDs;
    
    for (const auto& RequestPair : ActiveRequests)
    {
        ActiveRequestIDs.Add(RequestPair.Key);
    }
    
    return ActiveRequestIDs;
}

void UMingAIGeneratedContentSystem::SetModelProvider(EAIModelProvider Provider, const FString& APIKey)
{
    DefaultModelProvider = Provider;
    if (!APIKey.IsEmpty())
    {
        APIKeys.Add(Provider, APIKey);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Set default model provider to %d"), (int32)Provider);
}

void UMingAIGeneratedContentSystem::SetDefaultQuality(EGenerationQuality Quality)
{
    DefaultQuality = Quality;
    UE_LOG(LogTemp, Log, TEXT("Set default quality to %d"), (int32)Quality);
}

void UMingAIGeneratedContentSystem::SetOutputDirectory(const FString& Directory)
{
    OutputDirectory = Directory;
    
    // 確保目錄存在
    FString FullOutputPath = FPaths::ProjectDir() / OutputDirectory;
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*FullOutputPath))
    {
        PlatformFile.CreateDirectoryTree(*FullOutputPath);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Set output directory to %s"), *Directory);
}

void UMingAIGeneratedContentSystem::LoadGenerationPresets()
{
    // 加載預設模板
    // 這裡可以從配置文件或數據庫加載預設
}

FAIGenerationRequest UMingAIGeneratedContentSystem::GetPresetRequest(const FString& PresetName) const
{
    // 返回預設請求
    FAIGenerationRequest PresetRequest;
    // 實現預設加載邏輯
    return PresetRequest;
}

void UMingAIGeneratedContentSystem::SavePresetRequest(const FString& PresetName, const FAIGenerationRequest& Request)
{
    // 保存預設請求
    // 實現預設保存邏輯
}

float UMingAIGeneratedContentSystem::GetAverageGenerationTime(EAIGenerationType GenerationType) const
{
    if (AverageGenerationTimes.Contains(GenerationType))
    {
        return AverageGenerationTimes[GenerationType];
    }
    return 0.0f;
}

int32 UMingAIGeneratedContentSystem::GetTotalGeneratedCount(EAIGenerationType GenerationType) const
{
    if (TotalGeneratedCounts.Contains(GenerationType))
    {
        return TotalGeneratedCounts[GenerationType];
    }
    return 0;
}

float UMingAIGeneratedContentSystem::GetSystemLoad() const
{
    return (float)CurrentGenerationCount / MaxConcurrentGenerations;
}

void UMingAIGeneratedContentSystem::ClearCache()
{
    // 清理緩存
    GenerationResults.Empty();
    UE_LOG(LogTemp, Log, TEXT("Cleared generation cache"));
}

void UMingAIGeneratedContentSystem::CancelGeneration(const FString& RequestID)
{
    if (ActiveRequests.Contains(RequestID))
    {
        ActiveRequests.Remove(RequestID);
        RequestProgress.Remove(RequestID);
        UE_LOG(LogTemp, Log, TEXT("Cancelled generation %s"), *RequestID);
    }
}

void UMingAIGeneratedContentSystem::CancelAllGenerations()
{
    ActiveRequests.Empty();
    RequestProgress.Empty();
    CurrentGenerationCount = 0;
    UE_LOG(LogTemp, Log, TEXT("Cancelled all generations"));
}

void UMingAIGeneratedContentSystem::InitializeDefaultPresets()
{
    // 初始化默認預設
    // 可以在這裡設置一些常用的生成參數
}

FString UMingAIGeneratedContentSystem::GenerateRequestID()
{
    return FString::Printf(TEXT("REQ_%s_%d"), *FDateTime::Now().ToString(), FMath::RandRange(1000, 9999));
}

void UMingAIGeneratedContentSystem::ProcessGenerationRequest(const FAIGenerationRequest& Request)
{
    if (!ValidateRequest(Request))
    {
        FAIGenerationResult Result;
        Result.RequestID = Request.RequestID;
        Result.bSuccess = false;
        Result.ErrorMessage = TEXT("Invalid request");
        CompleteGeneration(Request.RequestID, Result);
        return;
    }

    // 檢查緩存
    if (bEnableCache)
    {
        FAIGenerationResult CachedResult = LoadFromCache(Request);
        if (CachedResult.bSuccess)
        {
            CompleteGeneration(Request.RequestID, CachedResult);
            return;
        }
    }

    // 根據生成類型處理請求
    FAIGenerationResult Result;
    Result.RequestID = Request.RequestID;
    Result.bSuccess = false;

    double StartTime = FDateTime::Now().GetTicks();

    switch (Request.GenerationType)
    {
        case EAIGenerationType::Image:
            Result.GeneratedTexture = GenerateImageInternal(Request);
            Result.bSuccess = (Result.GeneratedTexture != nullptr);
            break;
            
        case EAIGenerationType::Music:
            Result.GeneratedSound = GenerateMusicInternal(Request);
            Result.bSuccess = (Result.GeneratedSound != nullptr);
            break;
            
        case EAIGenerationType::SoundEffect:
            Result.GeneratedSound = GenerateSoundEffectInternal(Request);
            Result.bSuccess = (Result.GeneratedSound != nullptr);
            break;
            
        case EAIGenerationType::Mesh:
            Result.GeneratedMesh = GenerateMeshInternal(Request);
            Result.bSuccess = (Result.GeneratedMesh != nullptr);
            break;
            
        case EAIGenerationType::Material:
            Result.GeneratedMaterial = GenerateMaterialInternal(Request);
            Result.bSuccess = (Result.GeneratedMaterial != nullptr);
            break;
            
        default:
            // 處理其他類型
            break;
    }

    double EndTime = FDateTime::Now().GetTicks();
    Result.GenerationTime = (EndTime - StartTime) / ETimespan::TicksPerSecond;

    if (Result.bSuccess)
    {
        CacheResult(Result);
        UpdateStatistics(Request.GenerationType, Result.GenerationTime);
    }

    CompleteGeneration(Request.RequestID, Result);
}

void UMingAIGeneratedContentSystem::UpdateGenerationProgress(const FString& RequestID, float Progress)
{
    if (RequestProgress.Contains(RequestID))
    {
        RequestProgress[RequestID] = FMath::Clamp(Progress, 0.0f, 1.0f);
        NotifyGenerationProgress(RequestID, Progress);
    }
}

void UMingAIGeneratedContentSystem::CompleteGeneration(const FString& RequestID, const FAIGenerationResult& Result)
{
    GenerationResults.Add(RequestID, Result);
    ActiveRequests.Remove(RequestID);
    RequestProgress.Remove(RequestID);
    
    CurrentGenerationCount = FMath::Max(0, CurrentGenerationCount - 1);
    
    NotifyGenerationCompleted(Result);
    
    UE_LOG(LogTemp, Log, TEXT("Completed generation %s: %s"), *RequestID, Result.bSuccess ? TEXT("Success") : TEXT("Failed"));
}

void UMingAIGeneratedContentSystem::NotifyGenerationCompleted(const FAIGenerationResult& Result)
{
    OnAIGenerationCompleted.Broadcast(Result);
}

void UMingAIGeneratedContentSystem::NotifyGenerationProgress(const FString& RequestID, float Progress)
{
    OnAIGenerationProgress.Broadcast(RequestID, Progress);
}

void UMingAIGeneratedContentSystem::NotifySceneGenerated(const FString& SceneName)
{
    OnSceneGenerated.Broadcast(SceneName);
}

UTexture2D* UMingAIGeneratedContentSystem::GenerateImageInternal(const FAIGenerationRequest& Request)
{
    // 實現圖像生成邏輯
    // 這裡可以調用Stable Diffusion API或其他圖像生成服務
    
    // 創建示例紋理
    UTexture2D* GeneratedTexture = UTexture2D::CreateTransient(Request.Width, Request.Height);
    GeneratedTexture->UpdateResource();
    
    // 填充示例數據（實際實現中應該調用AI服務）
    FTexture2DMipMap* MipMap = &GeneratedTexture->GetPlatformData()->Mips[0];
    void* TextureData = MipMap->BulkData.Lock(LOCK_READ_WRITE);
    FMemory::Memzero(TextureData, Request.Width * Request.Height * 4);
    MipMap->BulkData.Unlock();
    GeneratedTexture->UpdateResource();
    
    return GeneratedTexture;
}

USoundBase* UMingAIGeneratedContentSystem::GenerateMusicInternal(const FAIGenerationRequest& Request)
{
    // 實現音樂生成邏輯
    // 調用AIVA API或其他音樂生成服務
    
    // 創建示例音頻資源
    USoundBase* GeneratedSound = NewObject<USoundBase>();
    
    return GeneratedSound;
}

USoundBase* UMingAIGeneratedContentSystem::GenerateSoundEffectInternal(const FAIGenerationRequest& Request)
{
    // 實現音效生成邏輯
    
    USoundBase* GeneratedSound = NewObject<USoundBase>();
    
    return GeneratedSound;
}

UStaticMesh* UMingAIGeneratedContentSystem::GenerateMeshInternal(const FAIGenerationRequest& Request)
{
    // 實現模型生成邏輯
    // 可以調用3D生成API或使用程序化生成
    
    UStaticMesh* GeneratedMesh = NewObject<UStaticMesh>();
    
    return GeneratedMesh;
}

UMaterialInterface* UMingAIGeneratedContentSystem::GenerateMaterialInternal(const FAIGenerationRequest& Request)
{
    // 實現材質生成邏輯
    
    UMaterialInstanceDynamic* GeneratedMaterial = UMaterialInstanceDynamic::Create(nullptr);
    
    return GeneratedMaterial;
}

void UMingAIGeneratedContentSystem::GenerateSceneLighting(const FAIGameSceneSetup& SceneSetup)
{
    // 實現場景光照設置
    // 根據時間和天氣設置光照參數
    
    UE_LOG(LogTemp, Log, TEXT("Generated lighting for scene %s"), *SceneSetup.SceneName);
}

void UMingAIGeneratedContentSystem::GenerateSceneEnvironment(const FAIGameSceneSetup& SceneSetup)
{
    // 實現場景環境設置
    // 根據地點和描述生成環境元素
    
    UE_LOG(LogTemp, Log, TEXT("Generated environment for scene %s"), *SceneSetup.SceneName);
}

void UMingAIGeneratedContentSystem::GenerateSceneEffects(const FAIGameSceneSetup& SceneSetup)
{
    // 實現場景特效設置
    // 根據天氣和情緒添加特效
    
    UE_LOG(LogTemp, Log, TEXT("Generated effects for scene %s"), *SceneSetup.SceneName);
}

void UMingAIGeneratedContentSystem::SetupSceneCamera(const FString& SceneName)
{
    // 實現攝像機設置
    // 設置場景的攝像機位置和參數
    
    UE_LOG(LogTemp, Log, TEXT("Setup camera for scene %s"), *SceneName);
}

FString UMingAIGeneratedContentSystem::GetModelAPIEndpoint(EAIModelProvider Provider) const
{
    switch (Provider)
    {
        case EAIModelProvider::StableDiffusion:
            return TEXT("https://api.stability.ai/v1/generation/stable-diffusion-xl-1024-v1-0/text-to-image");
        case EAIModelProvider::DALL_E:
            return TEXT("https://api.openai.com/v1/images/generations");
        case EAIModelProvider::Midjourney:
            return TEXT("https://api.midjourney.com/v1/imagine");
        case EAIModelProvider::AIVA:
            return TEXT("https://api.aiva.ai/v1/generate");
        case EAIModelProvider::ElevenLabs:
            return TEXT("https://api.elevenlabs.io/v1/text-to-speech");
        case EAIModelProvider::OpenAI:
            return TEXT("https://api.openai.com/v1/completions");
        default:
            return TEXT("");
    }
}

FString UMingAIGeneratedContentSystem::BuildAPIRequest(const FAIGenerationRequest& Request) const
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    
    JsonObject->SetStringField(TEXT("prompt"), Request.Prompt);
    JsonObject->SetStringField(TEXT("negative_prompt"), Request.NegativePrompt);
    JsonObject->SetNumberField(TEXT("width"), Request.Width);
    JsonObject->SetNumberField(TEXT("height"), Request.Height);
    JsonObject->SetNumberField(TEXT("duration"), Request.Duration);
    
    // 添加額外參數
    for (const auto& Param : Request.AdditionalParameters)
    {
        JsonObject->SetStringField(Param.Key, Param.Value);
    }
    
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
    
    return OutputString;
}

FAIGenerationResult UMingAIGeneratedContentSystem::ParseAPIResponse(const FString& Response, const FString& RequestID) const
{
    FAIGenerationResult Result;
    Result.RequestID = RequestID;
    
    // 解析API響應
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response);
    
    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        // 解析成功響應
        Result.bSuccess = true;
        
        // 提取生成的資產URL或其他信息
        if (JsonObject->HasField(TEXT("data")))
        {
            // 處理圖像生成響應
        }
    }
    else
    {
        Result.bSuccess = false;
        Result.ErrorMessage = TEXT("Failed to parse API response");
    }
    
    return Result;
}

bool UMingAIGeneratedContentSystem::ValidateRequest(const FAIGenerationRequest& Request) const
{
    if (Request.Prompt.IsEmpty())
    {
        return false;
    }
    
    if (!APIKeys.Contains(Request.ModelProvider) || APIKeys[Request.ModelProvider].IsEmpty())
    {
        return false;
    }
    
    if (CurrentGenerationCount >= MaxConcurrentGenerations)
    {
        return false;
    }
    
    return true;
}

void UMingAIGeneratedContentSystem::CacheResult(const FAIGenerationResult& Result)
{
    if (bEnableCache)
    {
        GenerationResults.Add(Result.RequestID, Result);
    }
}

FAIGenerationResult UMingAIGeneratedContentSystem::LoadFromCache(const FAIGenerationRequest& Request) const
{
    // 檢查是否有緩存的結果
    // 這裡可以實現基於prompt的緩存查找
    
    return FAIGenerationResult();
}

void UMingAIGeneratedContentSystem::UpdateStatistics(EAIGenerationType GenerationType, float GenerationTime)
{
    // 更新統計信息
    if (AverageGenerationTimes.Contains(GenerationType))
    {
        float CurrentAverage = AverageGenerationTimes[GenerationType];
        int32 CurrentCount = TotalGeneratedCounts[GenerationType];
        
        // 計算新的平均值
        float NewAverage = (CurrentAverage * CurrentCount + GenerationTime) / (CurrentCount + 1);
        AverageGenerationTimes[GenerationType] = NewAverage;
        TotalGeneratedCounts[GenerationType] = CurrentCount + 1;
    }
    else
    {
        AverageGenerationTimes.Add(GenerationType, GenerationTime);
        TotalGeneratedCounts.Add(GenerationType, 1);
    }
}
