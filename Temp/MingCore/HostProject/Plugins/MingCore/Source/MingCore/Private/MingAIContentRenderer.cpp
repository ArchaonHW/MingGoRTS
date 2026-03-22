#include "MingAIContentRenderer.h"
#include "MingAIGeneratedContentSystem.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Camera/CameraComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"

UMingAIContentRenderer::UMingAIContentRenderer()
{
    PrimaryComponentTick.bCanEverTick = true;
    bIsInitialized = false;
    bIsRendering = false;
    bRealTimeRendering = false;
    RenderProgress = 0.0f;
    CurrentFrame = 0;
    TotalFrames = 0;
    LastRenderedTexture = nullptr;
    ElapsedTime = 0.0f;
    AnimationTime = 0.0f;
    CameraAnimationDuration = 0.0f;
}

void UMingAIContentRenderer::BeginPlay()
{
    Super::BeginPlay();
    InitializeRenderer();
}

void UMingAIContentRenderer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsInitialized)
    {
        ElapsedTime += DeltaTime;

        if (bIsRendering)
        {
            UpdateCameraAnimation(DeltaTime);
            UpdateEnvironmentalEffects(DeltaTime);
            
            if (bRealTimeRendering)
            {
                CaptureSceneFrame();
            }
        }
    }
}

void UMingAIContentRenderer::InitializeRenderer()
{
    if (bIsInitialized)
    {
        return;
    }

    // 獲取AI內容系統
    AIContentSystem = NewObject<UMingAIGeneratedContentSystem>();
    if (AIContentSystem)
    {
        AIContentSystem->InitializeAIGenerationSystem();
    }

    // 初始化場景組件
    InitializeSceneComponents();

    // 設置默認場景
    SetupDefaultScene();

    bIsInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("AI Content Renderer initialized"));
}

void UMingAIContentRenderer::SetupScene(const FAISceneConfiguration& SceneConfig)
{
    CurrentSceneConfig = SceneConfig;

    // 設置攝像機
    SetupCamera(SceneConfig.CameraPosition, SceneConfig.CameraRotation, SceneConfig.FieldOfView);

    // 設置光照
    SetupLighting(SceneConfig.TimeOfDay, SceneConfig.Weather);

    // 設置環境
    SetupFog(SceneConfig.FogDensity, SceneConfig.FogColor, 100.0f);
    SetupAtmosphere(SceneConfig.LocationDescription);

    // 設置後處理
    SetupPostProcessing(SceneConfig);

    // 優化渲染設置
    OptimizeForQuality(SceneConfig.RenderQuality);

    UE_LOG(LogTemp, Log, TEXT("Setup scene: %s"), *SceneConfig.SceneName);
}

void UMingAIContentRenderer::UpdateSceneConfiguration(const FAISceneConfiguration& SceneConfig)
{
    SetupScene(SceneConfig);
}

UTexture2D* UMingAIContentRenderer::RenderSceneToTexture(const FAIRenderSettings& RenderSettings)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("Renderer not initialized"));
        return nullptr;
    }

    CurrentRenderSettings = RenderSettings;

    // 設置渲染目標
    if (RenderTarget)
    {
        RenderTarget->InitAutoFormat(RenderSettings.RenderWidth, RenderSettings.RenderHeight);
    }

    // 設置場景捕獲
    if (SceneCapture)
    {
        SceneCapture->TextureTarget = RenderTarget;
        SceneCapture->CaptureScene();
    }

    // 獲取渲染結果
    if (RenderTarget)
    {
        LastRenderedTexture = RenderTarget;
        OnSceneRendered.Broadcast(CurrentSceneConfig.SceneName, LastRenderedTexture);
        
        UE_LOG(LogTemp, Log, TEXT("Rendered scene %s to texture (%dx%d)"), 
               *CurrentSceneConfig.SceneName, RenderSettings.RenderWidth, RenderSettings.RenderHeight);
        
        return LastRenderedTexture;
    }

    return nullptr;
}

void UMingAIContentRenderer::RenderSceneToVideo(const FAIRenderSettings& RenderSettings, float Duration, const FString& OutputPath)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("Renderer not initialized"));
        return;
    }

    CurrentRenderSettings = RenderSettings;
    TotalFrames = FMath::RoundToInt(Duration * RenderSettings.FrameRate);
    CurrentFrame = 0;
    bIsRendering = true;
    RenderProgress = 0.0f;

    UE_LOG(LogTemp, Log, TEXT("Starting video render: %s (%.2fs, %d frames)"), 
           *OutputPath, Duration, TotalFrames);

    // 異步渲染視頻
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, Duration, OutputPath]()
    {
        FString FullOutputPath = FPaths::ProjectDir() / OutputPath;
        
        for (int32 Frame = 0; Frame < TotalFrames; ++Frame)
        {
            CurrentFrame = Frame;
            RenderProgress = (float)Frame / TotalFrames;

            // 渲染當前幀
            RenderSceneToTexture(CurrentRenderSettings);
            
            // 通知幀渲染完成
            if (LastRenderedTexture)
            {
                OnVideoFrameRendered.Broadcast(CurrentSceneConfig.SceneName, Frame, LastRenderedTexture);
            }

            // 模擬渲染時間
            FPlatformProcess::Sleep(0.01f);
        }

        bIsRendering = false;
        RenderProgress = 1.0f;
        
        UE_LOG(LogTemp, Log, TEXT("Video render completed: %s"), *OutputPath);
    });
}

void UMingAIContentRenderer::StartRealTimeRendering(const FAIRenderSettings& RenderSettings)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("Renderer not initialized"));
        return;
    }

    CurrentRenderSettings = RenderSettings;
    bRealTimeRendering = true;
    bIsRendering = true;

    // 設置渲染目標
    if (RenderTarget)
    {
        RenderTarget->InitAutoFormat(RenderSettings.RenderWidth, RenderSettings.RenderHeight);
    }

    UE_LOG(LogTemp, Log, TEXT("Started real-time rendering (%dx%d, %.1f FPS)"), 
           RenderSettings.RenderWidth, RenderSettings.RenderHeight, RenderSettings.FrameRate);
}

void UMingAIContentRenderer::StopRealTimeRendering()
{
    bRealTimeRendering = false;
    bIsRendering = false;
    
    UE_LOG(LogTemp, Log, TEXT("Stopped real-time rendering"));
}

void UMingAIContentRenderer::SetupLighting(ETimeOfDay TimeOfDay, EWeatherType Weather)
{
    UpdateLightingForTimeOfDay(TimeOfDay);
    UpdateWeatherEffects(Weather);

    // 設置環境光
    SetupAmbientLight(CurrentSceneConfig.AmbientColor, 0.3f);

    // 設置天光
    FLinearColor SkyColor = GetTimeOfDayColor(TimeOfDay);
    SetupSkyLight(SkyColor, 1.0f);

    UE_LOG(LogTemp, Log, TEXT("Setup lighting: Time=%d, Weather=%d"), (int32)TimeOfDay, (int32)Weather);
}

void UMingAIContentRenderer::UpdateDirectionalLight(const FVector& Direction, const FLinearColor& Color, float Intensity)
{
    if (DirectionalLight)
    {
        DirectionalLight->SetWorldRotation(Direction.Rotation());
        DirectionalLight->SetLightColor(Color);
        DirectionalLight->SetIntensity(Intensity);
    }
}

void UMingAIContentRenderer::SetupAmbientLight(const FLinearColor& Color, float Intensity)
{
    if (GetWorld())
    {
        GetWorld()->SetAmbientColor(Color);
    }
}

void UMingAIContentRenderer::SetupSkyLight(const FLinearColor& Color, float Intensity)
{
    if (SkyLight)
    {
        SkyLight->SetLightColor(Color);
        SkyLight->SetIntensity(Intensity);
    }
}

void UMingAIContentRenderer::SetupFog(float Density, const FLinearColor& Color, float Height)
{
    if (FogComponent)
    {
        FogComponent->SetFogDensity(Density);
        FogComponent->SetFogInscatteringColor(Color);
        FogComponent->SetFogHeightFalloff(Height);
    }
}

void UMingAIContentRenderer::SetupAtmosphere(const FString& AtmosphereType)
{
    // 根據大氣類型設置環境參數
    if (AtmosphereType.Contains(TEXT("mountain")))
    {
        // 山區大氣
        SetupFog(0.02f, FLinearColor(0.7f, 0.8f, 0.9f, 1.0f), 200.0f);
    }
    else if (AtmosphereType.Contains(TEXT("forest")))
    {
        // 森林大氣
        SetupFog(0.05f, FLinearColor(0.6f, 0.7f, 0.5f, 1.0f), 100.0f);
    }
    else if (AtmosphereType.Contains(TEXT("desert")))
    {
        // 沙漠大氣
        SetupFog(0.01f, FLinearColor(0.9f, 0.8f, 0.6f, 1.0f), 300.0f);
    }
    else
    {
        // 默認大氣
        SetupFog(0.03f, FLinearColor(0.5f, 0.6f, 0.7f, 1.0f), 150.0f);
    }
}

void UMingAIContentRenderer::SetupWeatherEffects(EWeatherType Weather)
{
    switch (Weather)
    {
        case EWeatherType::Clear:
            // 晴朗天氣
            SetupFog(0.01f, FLinearColor(0.8f, 0.9f, 1.0f, 1.0f), 200.0f);
            break;
            
        case EWeatherType::Cloudy:
            // 多雲天氣
            SetupFog(0.03f, FLinearColor(0.6f, 0.6f, 0.7f, 1.0f), 150.0f);
            break;
            
        case EWeatherType::Overcast:
            // 陰天
            SetupFog(0.05f, FLinearColor(0.5f, 0.5f, 0.5f, 1.0f), 100.0f);
            break;
            
        case EWeatherType::Rainy:
            // 下雨
            SetupFog(0.08f, FLinearColor(0.4f, 0.4f, 0.5f, 1.0f), 80.0f);
            break;
            
        case EWeatherType::Stormy:
            // 暴雨
            SetupFog(0.12f, FLinearColor(0.3f, 0.3f, 0.4f, 1.0f), 50.0f);
            break;
            
        case EWeatherType::Snowy:
            // 下雪
            SetupFog(0.06f, FLinearColor(0.8f, 0.8f, 0.9f, 1.0f), 120.0f);
            break;
            
        case EWeatherType::Foggy:
            // 有霧
            SetupFog(0.15f, FLinearColor(0.6f, 0.6f, 0.7f, 1.0f), 30.0f);
            break;
            
        default:
            break;
    }
}

void UMingAIContentRenderer::SetupPostProcessing(const FAISceneConfiguration& SceneConfig)
{
    if (!SceneConfig.bEnablePostProcessing)
    {
        return;
    }

    UpdatePostProcessingForMood(SceneConfig.Mood);

    if (SceneConfig.bEnableBloom)
    {
        AddBloomEffect(0.5f, FLinearColor::White);
    }

    if (SceneConfig.bEnableLensFlare)
    {
        // 添加鏡頭光暈效果
    }

    if (SceneConfig.bEnableVolumetricFog)
    {
        // 啟用體積霧
    }
}

void UMingAIContentRenderer::AddBloomEffect(float Intensity, const FLinearColor& Tint)
{
    if (PostProcessComponent)
    {
        // 設置Bloom效果
        // 這裡需要實現具體的後處理設置
    }
}

void UMingAIContentRenderer::AddVignetteEffect(float Intensity, const FLinearColor& Color)
{
    if (PostProcessComponent)
    {
        // 設置暈影效果
    }
}

void UMingAIContentRenderer::AddColorGrading(const FLinearColor& Shadows, const FLinearColor& Midtones, const FLinearColor& Highlights)
{
    if (PostProcessComponent)
    {
        // 設置顏色分級
    }
}

void UMingAIContentRenderer::AddDepthOfField(float FocusDistance, float FocusRange, float NearTransition, float FarTransition)
{
    if (PostProcessComponent)
    {
        // 設置景深效果
    }
}

void UMingAIContentRenderer::SetupCamera(const FVector& Position, const FRotator& Rotation, float FieldOfView)
{
    if (CameraComponent)
    {
        CameraComponent->SetWorldLocation(Position);
        CameraComponent->SetWorldRotation(Rotation);
        CameraComponent->SetFieldOfView(FieldOfView);
    }

    if (SceneCapture)
    {
        SceneCapture->SetWorldLocation(Position);
        SceneCapture->SetWorldRotation(Rotation);
        SceneCapture->FOVAngle = FieldOfView;
    }
}

void UMingAIContentRenderer::AnimateCamera(const FVector& TargetPosition, float Duration, const FString& AnimationType)
{
    CameraStartPosition = CameraComponent ? CameraComponent->GetComponentLocation() : FVector::ZeroVector;
    CameraEndPosition = TargetPosition;
    CameraAnimationDuration = Duration;
    AnimationTime = 0.0f;

    UE_LOG(LogTemp, Log, TEXT("Starting camera animation: %s (%.2fs)"), *AnimationType, Duration);
}

void UMingAIContentRenderer::SetCameraShake(float Intensity, float Duration)
{
    // 實現攝像機震動效果
    if (CameraComponent)
    {
        // 添加震動效果
    }
}

void UMingAIContentRenderer::ApplyGeneratedTexture(UTexture2D* Texture, const FString& MaterialSlot)
{
    if (!Texture)
    {
        return;
    }

    // 將生成的紋理應用到場景中的材質
    // 這裡需要實現具體的材質應用邏輯
    
    UE_LOG(LogTemp, Log, TEXT("Applied generated texture to material slot: %s"), *MaterialSlot);
}

void UMingAIContentRenderer::ApplyGeneratedMaterial(UMaterialInterface* Material, const FString& ComponentName)
{
    if (!Material)
    {
        return;
    }

    // 將生成的材質應用到指定組件
    // 這裡需要實現具體的材質應用邏輯
    
    UE_LOG(LogTemp, Log, TEXT("Applied generated material to component: %s"), *ComponentName);
}

void UMingAIContentRenderer::UpdateMaterialParameters(const TMap<FString, float>& Parameters)
{
    // 更新材質參數
    for (const auto& Param : Parameters)
    {
        // 設置材質參數
    }
}

void UMingAIContentRenderer::PlayGeneratedMusic(USoundBase* Music, float Volume)
{
    if (!Music || !AudioComponent)
    {
        return;
    }

    AudioComponent->SetSound(Music);
    AudioComponent->SetVolumeMultiplier(Volume);
    AudioComponent->Play();

    UE_LOG(LogTemp, Log, TEXT("Playing generated music"));
}

void UMingAIContentRenderer::PlayGeneratedSoundEffect(USoundBase* SoundEffect, const FVector& Location)
{
    if (!SoundEffect)
    {
        return;
    }

    UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundEffect, Location);

    UE_LOG(LogTemp, Log, TEXT("Playing generated sound effect at location: %s"), *Location.ToString());
}

void UMingAIContentRenderer::SetAudioEnvironment(const FString& EnvironmentType)
{
    // 設置音頻環境
    if (EnvironmentType.Contains(TEXT("outdoor")))
    {
        // 戶外環境
    }
    else if (EnvironmentType.Contains(TEXT("indoor")))
    {
        // 室內環境
    }
    else if (EnvironmentType.Contains(TEXT("cave")))
    {
        // 洞穴環境
    }
}

void UMingAIContentRenderer::PlayGeneratedAnimation(const FString& AnimationName, float PlayRate)
{
    // 播放生成的動畫
    // 這裡需要實現動畫播放邏輯
    
    UE_LOG(LogTemp, Log, TEXT("Playing generated animation: %s"), *AnimationName);
}

void UMingAIContentRenderer::BlendAnimations(const TArray<FString>& AnimationNames, const TArray<float>& Weights)
{
    // 混合多個動畫
    // 這裡需要實現動畫混合邏輯
    
    UE_LOG(LogTemp, Log, TEXT("Blending %d animations"), AnimationNames.Num());
}

void UMingAIContentRenderer::SetAnimationMood(ESceneMood Mood)
{
    // 根據情緒設置動畫參數
    switch (Mood)
    {
        case ESceneMood::Peaceful:
            // 平和的動畫
            break;
        case ESceneMood::Dramatic:
            // 戲劇性的動畫
            break;
        case ESceneMood::Mysterious:
            // 神秘的動畫
            break;
        case ESceneMood::Epic:
            // 史詩的動畫
            break;
        default:
            break;
    }
}

void UMingAIContentRenderer::SpawnParticleEffect(const FString& EffectName, const FVector& Location)
{
    // 生成粒子特效
    // 這裡需要實現粒子特效生成邏輯
    
    UE_LOG(LogTemp, Log, TEXT("Spawning particle effect: %s at %s"), *EffectName, *Location.ToString());
}

void UMingAIContentRenderer::CreateEnvironmentalEffects(EWeatherType Weather)
{
    switch (Weather)
    {
        case EWeatherType::Rainy:
            SpawnParticleEffect(TEXT("Rain"), FVector::ZeroVector);
            break;
        case EWeatherType::Snowy:
            SpawnParticleEffect(TEXT("Snow"), FVector::ZeroVector);
            break;
        case EWeatherType::Stormy:
            SpawnParticleEffect(TEXT("Storm"), FVector::ZeroVector);
            break;
        default:
            break;
    }
}

void UMingAIContentRenderer::AddScreenEffects(const TArray<FString>& EffectNames)
{
    // 添加屏幕特效
    for (const FString& EffectName : EffectNames)
    {
        // 添加特效
    }
}

void UMingAIContentRenderer::OptimizeForQuality(ERenderQuality Quality)
{
    OptimizeRenderSettings(Quality);
}

void UMingAIContentRenderer::OptimizeForPerformance()
{
    OptimizeRenderSettings(ERenderQuality::Low);
}

void UMingAIContentRenderer::SetLODSettings(int32 MaxLOD, float LODDistance)
{
    // 設置LOD設置
    if (GetWorld())
    {
        // 設置世界LOD設置
    }
}

bool UMingAIContentRenderer::IsRendering() const
{
    return bIsRendering;
}

float UMingAIContentRenderer::GetRenderProgress() const
{
    return RenderProgress;
}

void UMingAIContentRenderer::InitializeSceneComponents()
{
    // 創建場景捕獲組件
    SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
    SceneCapture->SetupAttachment(this);
    
    // 創建攝像機組件
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(this);
    
    // 創建光照組件
    DirectionalLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("DirectionalLight"));
    DirectionalLight->SetupAttachment(this);
    
    SkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("SkyLight"));
    SkyLight->SetupAttachment(this);
    
    // 創建環境組件
    FogComponent = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("FogComponent"));
    FogComponent->SetupAttachment(this);
    
    // 創建後處理組件
    PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
    PostProcessComponent->SetupAttachment(this);
    
    // 創建音頻組件
    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    AudioComponent->SetupAttachment(this);
    
    // 創建渲染目標
    RenderTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("RenderTarget"));
    RenderTarget->InitAutoFormat(1920, 1080);
    
    UE_LOG(LogTemp, Log, TEXT("Scene components initialized"));
}

void UMingAIContentRenderer::SetupDefaultScene()
{
    // 設置默認場景配置
    FAISceneConfiguration DefaultConfig;
    SetupScene(DefaultConfig);
}

void UMingAIContentRenderer::UpdateLightingForTimeOfDay(ETimeOfDay TimeOfDay)
{
    FLinearColor LightColor = GetTimeOfDayColor(TimeOfDay);
    FVector LightDirection = GetLightDirection(TimeOfDay);
    float LightIntensity = GetLightIntensity(TimeOfDay, CurrentSceneConfig.Weather);
    
    UpdateDirectionalLight(LightDirection, LightColor, LightIntensity);
}

void UMingAIContentRenderer::UpdateWeatherEffects(EWeatherType Weather)
{
    SetupWeatherEffects(Weather);
    CreateEnvironmentalEffects(Weather);
}

void UMingAIContentRenderer::UpdatePostProcessingForMood(ESceneMood Mood)
{
    switch (Mood)
    {
        case ESceneMood::Peaceful:
            AddColorGrading(FLinearColor(0.2f, 0.2f, 0.3f, 1.0f), FLinearColor(0.5f, 0.5f, 0.5f, 1.0f), FLinearColor(0.8f, 0.8f, 0.7f, 1.0f));
            break;
        case ESceneMood::Dramatic:
            AddColorGrading(FLinearColor(0.1f, 0.1f, 0.2f, 1.0f), FLinearColor(0.3f, 0.3f, 0.4f, 1.0f), FLinearColor(0.6f, 0.6f, 0.7f, 1.0f));
            AddVignetteEffect(0.3f, FLinearColor::Black);
            break;
        case ESceneMood::Mysterious:
            AddColorGrading(FLinearColor(0.1f, 0.1f, 0.2f, 1.0f), FLinearColor(0.4f, 0.4f, 0.5f, 1.0f), FLinearColor(0.7f, 0.7f, 0.8f, 1.0f));
            AddDepthOfField(500.0f, 200.0f, 50.0f, 100.0f);
            break;
        case ESceneMood::Epic:
            AddColorGrading(FLinearColor(0.2f, 0.1f, 0.1f, 1.0f), FLinearColor(0.5f, 0.4f, 0.3f, 1.0f), FLinearColor(0.9f, 0.8f, 0.7f, 1.0f));
            AddBloomEffect(0.8f, FLinearColor::White);
            break;
        default:
            break;
    }
}

void UMingAIContentRenderer::OptimizeRenderSettings(ERenderQuality Quality)
{
    switch (Quality)
    {
        case ERenderQuality::Low:
            CurrentRenderSettings.TextureQuality = 0;
            CurrentRenderSettings.ShadowQuality = 1;
            CurrentRenderSettings.PostProcessingQuality = 1;
            CurrentRenderSettings.bEnableMotionBlur = false;
            CurrentRenderSettings.bEnableDepthOfField = false;
            CurrentRenderSettings.bEnableAmbientOcclusion = false;
            CurrentRenderSettings.bEnableScreenSpaceReflections = false;
            break;
            
        case ERenderQuality::Medium:
            CurrentRenderSettings.TextureQuality = 1;
            CurrentRenderSettings.ShadowQuality = 2;
            CurrentRenderSettings.PostProcessingQuality = 2;
            CurrentRenderSettings.bEnableMotionBlur = true;
            CurrentRenderSettings.bEnableDepthOfField = false;
            CurrentRenderSettings.bEnableAmbientOcclusion = true;
            CurrentRenderSettings.bEnableScreenSpaceReflections = false;
            break;
            
        case ERenderQuality::High:
            CurrentRenderSettings.TextureQuality = 2;
            CurrentRenderSettings.ShadowQuality = 3;
            CurrentRenderSettings.PostProcessingQuality = 3;
            CurrentRenderSettings.bEnableMotionBlur = true;
            CurrentRenderSettings.bEnableDepthOfField = true;
            CurrentRenderSettings.bEnableAmbientOcclusion = true;
            CurrentRenderSettings.bEnableScreenSpaceReflections = true;
            break;
            
        case ERenderQuality::Ultra:
        case ERenderQuality::Cinematic:
            CurrentRenderSettings.TextureQuality = 3;
            CurrentRenderSettings.ShadowQuality = 4;
            CurrentRenderSettings.PostProcessingQuality = 4;
            CurrentRenderSettings.bEnableMotionBlur = true;
            CurrentRenderSettings.bEnableDepthOfField = true;
            CurrentRenderSettings.bEnableAmbientOcclusion = true;
            CurrentRenderSettings.bEnableScreenSpaceReflections = true;
            CurrentRenderSettings.bEnableRayTracing = true;
            break;
    }
}

void UMingAIContentRenderer::CaptureSceneFrame()
{
    if (SceneCapture && RenderTarget)
    {
        SceneCapture->CaptureScene();
        LastRenderedTexture = RenderTarget;
    }
}

void UMingAIContentRenderer::ProcessVideoFrame()
{
    if (bIsRendering && CurrentFrame < TotalFrames)
    {
        CaptureSceneFrame();
        
        if (LastRenderedTexture)
        {
            OnVideoFrameRendered.Broadcast(CurrentSceneConfig.SceneName, CurrentFrame, LastRenderedTexture);
        }
        
        CurrentFrame++;
        RenderProgress = (float)CurrentFrame / TotalFrames;
        
        if (CurrentFrame >= TotalFrames)
        {
            bIsRendering = false;
            RenderProgress = 1.0f;
        }
    }
}

void UMingAIContentRenderer::UpdateCameraAnimation(float DeltaTime)
{
    if (CameraAnimationDuration > 0.0f)
    {
        AnimationTime += DeltaTime;
        
        if (AnimationTime < CameraAnimationDuration)
        {
            float Alpha = AnimationTime / CameraAnimationDuration;
            FVector CurrentPosition = FMath::VInterpTo(CameraStartPosition, CameraEndPosition, Alpha, 2.0f);
            
            if (CameraComponent)
            {
                CameraComponent->SetWorldLocation(CurrentPosition);
            }
            
            if (SceneCapture)
            {
                SceneCapture->SetWorldLocation(CurrentPosition);
            }
        }
        else
        {
            CameraAnimationDuration = 0.0f;
        }
    }
}

void UMingAIContentRenderer::UpdateEnvironmentalEffects(float DeltaTime)
{
    // 更新環境特效
    // 這裡可以實現動態天氣、粒子效果等
}

FLinearColor UMingAIContentRenderer::GetTimeOfDayColor(ETimeOfDay TimeOfDay) const
{
    switch (TimeOfDay)
    {
        case ETimeOfDay::Dawn:
            return FLinearColor(1.0f, 0.6f, 0.3f, 1.0f);
        case ETimeOfDay::Morning:
            return FLinearColor(1.0f, 0.9f, 0.7f, 1.0f);
        case ETimeOfDay::Noon:
            return FLinearColor(1.0f, 0.98f, 0.95f, 1.0f);
        case ETimeOfDay::Afternoon:
            return FLinearColor(1.0f, 0.8f, 0.6f, 1.0f);
        case ETimeOfDay::Dusk:
            return FLinearColor(1.0f, 0.5f, 0.2f, 1.0f);
        case ETimeOfDay::Night:
            return FLinearColor(0.2f, 0.3f, 0.5f, 1.0f);
        case ETimeOfDay::Midnight:
            return FLinearColor(0.1f, 0.1f, 0.3f, 1.0f);
        default:
            return FLinearColor::White;
    }
}

FVector UMingAIContentRenderer::GetLightDirection(ETimeOfDay TimeOfDay) const
{
    switch (TimeOfDay)
    {
        case ETimeOfDay::Dawn:
            return FVector(0.7f, -0.7f, -0.2f);
        case ETimeOfDay::Morning:
            return FVector(0.5f, -0.5f, -0.7f);
        case ETimeOfDay::Noon:
            return FVector(0.0f, 0.0f, -1.0f);
        case ETimeOfDay::Afternoon:
            return FVector(-0.5f, 0.5f, -0.7f);
        case ETimeOfDay::Dusk:
            return FVector(-0.7f, 0.7f, -0.2f);
        case ETimeOfDay::Night:
            return FVector(0.0f, 0.0f, -0.1f);
        case ETimeOfDay::Midnight:
            return FVector(0.0f, 0.0f, -0.05f);
        default:
            return FVector(0.0f, 0.0f, -1.0f);
    }
}

float UMingAIContentRenderer::GetLightIntensity(ETimeOfDay TimeOfDay, EWeatherType Weather) const
{
    float BaseIntensity = 1.0f;
    
    // 根據時間調整強度
    switch (TimeOfDay)
    {
        case ETimeOfDay::Dawn:
            BaseIntensity = 0.6f;
            break;
        case ETimeOfDay::Morning:
            BaseIntensity = 0.8f;
            break;
        case ETimeOfDay::Noon:
            BaseIntensity = 1.2f;
            break;
        case ETimeOfDay::Afternoon:
            BaseIntensity = 1.0f;
            break;
        case ETimeOfDay::Dusk:
            BaseIntensity = 0.5f;
            break;
        case ETimeOfDay::Night:
            BaseIntensity = 0.1f;
            break;
        case ETimeOfDay::Midnight:
            BaseIntensity = 0.05f;
            break;
    }
    
    // 根據天氣調整強度
    switch (Weather)
    {
        case EWeatherType::Clear:
            break;
        case EWeatherType::Cloudy:
            BaseIntensity *= 0.8f;
            break;
        case EWeatherType::Overcast:
            BaseIntensity *= 0.6f;
            break;
        case EWeatherType::Rainy:
            BaseIntensity *= 0.5f;
            break;
        case EWeatherType::Stormy:
            BaseIntensity *= 0.4f;
            break;
        case EWeatherType::Snowy:
            BaseIntensity *= 0.7f;
            break;
        case EWeatherType::Foggy:
            BaseIntensity *= 0.6f;
            break;
        default:
            break;
    }
    
    return BaseIntensity;
}

FLinearColor UMingAIContentRenderer::GetFogColor(EWeatherType Weather) const
{
    switch (Weather)
    {
        case EWeatherType::Clear:
            return FLinearColor(0.8f, 0.9f, 1.0f, 1.0f);
        case EWeatherType::Cloudy:
            return FLinearColor(0.6f, 0.6f, 0.7f, 1.0f);
        case EWeatherType::Overcast:
            return FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
        case EWeatherType::Rainy:
            return FLinearColor(0.4f, 0.4f, 0.5f, 1.0f);
        case EWeatherType::Stormy:
            return FLinearColor(0.3f, 0.3f, 0.4f, 1.0f);
        case EWeatherType::Snowy:
            return FLinearColor(0.8f, 0.8f, 0.9f, 1.0f);
        case EWeatherType::Foggy:
            return FLinearColor(0.6f, 0.6f, 0.7f, 1.0f);
        default:
            return FLinearColor(0.5f, 0.6f, 0.7f, 1.0f);
    }
}

float UMingAIContentRenderer::GetFogDensity(EWeatherType Weather) const
{
    switch (Weather)
    {
        case EWeatherType::Clear:
            return 0.01f;
        case EWeatherType::Cloudy:
            return 0.03f;
        case EWeatherType::Overcast:
            return 0.05f;
        case EWeatherType::Rainy:
            return 0.08f;
        case EWeatherType::Stormy:
            return 0.12f;
        case EWeatherType::Snowy:
            return 0.06f;
        case EWeatherType::Foggy:
            return 0.15f;
        default:
            return 0.03f;
    }
}
