#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/SceneComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/StaticMeshActor.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/PostProcessComponent.h"
#include "Camera/CameraComponent.h"
#include "MingAIContentRenderer.generated.h"

class UMingAIGeneratedContentSystem;
class UMaterialInterface;
class UTexture2D;
class USoundBase;

UENUM(BlueprintType)
enum class ERenderQuality : uint8
{
    Low,        // ‰ΩéË≥™??    Medium,     // ‰∏≠Á?Ë≥™È?
    High,       // È´òË≥™??    Ultra,      // Ë∂ÖÈ?Ë≥™È?
    Cinematic   // ?ªÂΩ±Á¥öË≥™??};

UENUM(BlueprintType)
enum class ETimeOfDay : uint8
{
    Dawn,       // ÈªéÊ?
    Morning,    // ?©Êô®
    Noon,       // Ê≠??
    Afternoon,  // ‰∏ãÂ?
    Dusk,       // ÈªÉÊ?
    Night,      // Â§úÊ?
    Midnight    // ?àÂ?
};

UENUM(BlueprintType)
enum class EWeatherType : uint8
{
    Clear,      // ?¥Ê?
    Cloudy,     // Â§öÈõ≤
    Overcast,   // ?∞Â§©
    Rainy,      // ‰∏ãÈõ®
    Stormy,     // ?¥Èõ®
    Snowy,      // ‰∏ãÈõ™
    Foggy,      // ?âÈúß
    Windy       // ?âÈ¢®
};

UENUM(BlueprintType)
enum class ESceneMood : uint8
{
    Peaceful,   // Âπ≥Â?
    Dramatic,   // ?≤Â???    Mysterious, // Á•ûÁ?
    Epic,       // ?≤Ë©©
    Intimate,   // Ë¶™Â?
    Tense,      // Á∑äÂºµ
    Joyful,     // Ê≠°Âø´
    Melancholy  // ?ÇÈ¨±
};

USTRUCT(BlueprintType)
struct FAISceneConfiguration
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Scene Configuration")
    FString SceneName;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Configuration")
    FString LocationDescription;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Configuration")
    ETimeOfDay TimeOfDay;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Configuration")
    EWeatherType Weather;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Configuration")
    ESceneMood Mood;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Configuration")
    ERenderQuality RenderQuality;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Configuration")
    FVector CameraPosition;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Configuration")
    FRotator CameraRotation;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Configuration")
    float FieldOfView;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Configuration")
    FLinearColor AmbientColor;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Configuration")
    FLinearColor DirectionalLightColor;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Configuration")
    FVector DirectionalLightDirection;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Configuration")
    float FogDensity;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Configuration")
    FLinearColor FogColor;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Configuration")
    bool bEnablePostProcessing;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Configuration")
    bool bEnableVolumetricFog;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Configuration")
    bool bEnableLensFlare;

    UPROPERTY(BlueprintReadWrite, Category = "Scene Configuration")
    bool bEnableBloom;

    FAISceneConfiguration()
    {
        SceneName = TEXT("AIScene");
        LocationDescription = TEXT("Beautiful landscape");
        TimeOfDay = ETimeOfDay::Morning;
        Weather = EWeatherType::Clear;
        Mood = ESceneMood::Peaceful;
        RenderQuality = ERenderQuality::High;
        CameraPosition = FVector(0.0f, 0.0f, 200.0f);
        CameraRotation = FRotator(0.0f, 0.0f, 0.0f);
        FieldOfView = 90.0f;
        AmbientColor = FLinearColor(0.2f, 0.2f, 0.2f, 1.0f);
        DirectionalLightColor = FLinearColor(1.0f, 0.95f, 0.8f, 1.0f);
        DirectionalLightDirection = FVector(-1.0f, -1.0f, -1.0f);
        FogDensity = 0.0f;
        FogColor = FLinearColor(0.5f, 0.6f, 0.7f, 1.0f);
        bEnablePostProcessing = true;
        bEnableVolumetricFog = false;
        bEnableLensFlare = true;
        bEnableBloom = true;
    }
};

USTRUCT(BlueprintType)
struct FAIRenderSettings
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Render Settings")
    int32 RenderWidth;

    UPROPERTY(BlueprintReadWrite, Category = "Render Settings")
    int32 RenderHeight;

    UPROPERTY(BlueprintReadWrite, Category = "Render Settings")
    float FrameRate;

    UPROPERTY(BlueprintReadWrite, Category = "Render Settings")
    int32 AntiAliasingQuality;

    UPROPERTY(BlueprintReadWrite, Category = "Render Settings")
    int32 TextureQuality;

    UPROPERTY(BlueprintReadWrite, Category = "Render Settings")
    int32 ShadowQuality;

    UPROPERTY(BlueprintReadWrite, Category = "Render Settings")
    int32 PostProcessingQuality;

    UPROPERTY(BlueprintReadWrite, Category = "Render Settings")
    bool bEnableMotionBlur;

    UPROPERTY(BlueprintReadWrite, Category = "Render Settings")
    bool bEnableDepthOfField;

    UPROPERTY(BlueprintReadWrite, Category = "Render Settings")
    bool bEnableAmbientOcclusion;

    UPROPERTY(BlueprintReadWrite, Category = "Render Settings")
    bool bEnableScreenSpaceReflections;

    UPROPERTY(BlueprintReadWrite, Category = "Render Settings")
    float RenderScale;

    UPROPERTY(BlueprintReadWrite, Category = "Render Settings")
    bool bUseTemporalAA;

    UPROPERTY(BlueprintReadWrite, Category = "Render Settings")
    bool bEnableRayTracing;

    FAIRenderSettings()
    {
        RenderWidth = 1920;
        RenderHeight = 1080;
        FrameRate = 60.0f;
        AntiAliasingQuality = 4;
        TextureQuality = 2;
        ShadowQuality = 3;
        PostProcessingQuality = 2;
        bEnableMotionBlur = true;
        bEnableDepthOfField = true;
        bEnableAmbientOcclusion = true;
        bEnableScreenSpaceReflections = true;
        RenderScale = 1.0f;
        bUseTemporalAA = true;
        bEnableRayTracing = false;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSceneRendered, const FString&, SceneName, UTexture2D*, RenderedTexture);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnVideoFrameRendered, const FString&, SceneName, int32, FrameIndex, UTexture2D*, FrameTexture);

/**
 * AI ?ßÂÆπÊ∏≤Ê??? * Ë≤†Ë≤¨Â∞áAI?üÊ??ÑÂÖßÂÆπÊ∏≤?ìÂà∞?äÊà≤?´Èù¢
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingAIContentRenderer : public USceneComponent
{
    GENERATED_BODY()

public:
    UMingAIContentRenderer();

    // ?ùÂ???    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void InitializeRenderer();

    // ?¥ÊôØË®≠ÁΩÆ
    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void SetupScene(const FAISceneConfiguration& SceneConfig);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void UpdateSceneConfiguration(const FAISceneConfiguration& SceneConfig);

    // Ê∏≤Ê??ßÂà∂
    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    UTexture2D* RenderSceneToTexture(const FAIRenderSettings& RenderSettings);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void RenderSceneToVideo(const FAIRenderSettings& RenderSettings, float Duration, const FString& OutputPath);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void StartRealTimeRendering(const FAIRenderSettings& RenderSettings);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void StopRealTimeRendering();

    // ?âÁÖßË®≠ÁΩÆ
    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void SetupLighting(ETimeOfDay TimeOfDay, EWeatherType Weather);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void UpdateDirectionalLight(const FVector& Direction, const FLinearColor& Color, float Intensity);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void SetupAmbientLight(const FLinearColor& Color, float Intensity);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void SetupSkyLight(const FLinearColor& Color, float Intensity);

    // ?∞Â?Ë®≠ÁΩÆ
    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void SetupFog(float Density, const FLinearColor& Color, float Height);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void SetupAtmosphere(const FString& AtmosphereType);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void SetupWeatherEffects(EWeatherType Weather);

    // ÂæåË???    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void SetupPostProcessing(const FAISceneConfiguration& SceneConfig);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void AddBloomEffect(float Intensity, const FLinearColor& Tint);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void AddVignetteEffect(float Intensity, const FLinearColor& Color);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void AddColorGrading(const FLinearColor& Shadows, const FLinearColor& Midtones, const FLinearColor& Highlights);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void AddDepthOfField(float FocusDistance, float FocusRange, float NearTransition, float FarTransition);

    // ?ùÂ?Ê©üÊéß??    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void SetupCamera(const FVector& Position, const FRotator& Rotation, float FieldOfView);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void AnimateCamera(const FVector& TargetPosition, float Duration, const FString& AnimationType);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void SetCameraShake(float Intensity, float Duration);

    // ?êË≥™?åÁ???    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void ApplyGeneratedTexture(UTexture2D* Texture, const FString& MaterialSlot);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void ApplyGeneratedMaterial(UMaterialInterface* Material, const FString& ComponentName);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void UpdateMaterialParameters(const TMap<FString, float>& Parameters);

    // ?≥È†ª?ÜÊ?
    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void PlayGeneratedMusic(USoundBase* Music, float Volume = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void PlayGeneratedSoundEffect(USoundBase* SoundEffect, const FVector& Location);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void SetAudioEnvironment(const FString& EnvironmentType);

    // ?ïÁï´Á≥ªÁµ±
    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void PlayGeneratedAnimation(const FString& AnimationName, float PlayRate = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void BlendAnimations(const TArray<FString>& AnimationNames, const TArray<float>& Weights);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void SetAnimationMood(ESceneMood Mood);

    // ?πÊ?Á≥ªÁµ±
    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void SpawnParticleEffect(const FString& EffectName, const FVector& Location);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void CreateEnvironmentalEffects(EWeatherType Weather);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void AddScreenEffects(const TArray<FString>& EffectNames);

    // ?ßËÉΩ?™Â?
    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void OptimizeForQuality(ERenderQuality Quality);

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void OptimizeForPerformance();

    UFUNCTION(BlueprintCallable, Category = "AI Content Renderer")
    void SetLODSettings(int32 MaxLOD, float LODDistance);

    // ?Ä?ãÊü•Ë©?    UFUNCTION(BlueprintPure, Category = "AI Content Renderer")
    bool IsRendering() const;

    UFUNCTION(BlueprintPure, Category = "AI Content Renderer")
    float GetRenderProgress() const;

    UFUNCTION(BlueprintPure, Category = "AI Content Renderer")
    FAISceneConfiguration GetCurrentSceneConfiguration() const { return CurrentSceneConfig; }

    UFUNCTION(BlueprintPure, Category = "AI Content Renderer")
    UTexture2D* GetLastRenderedTexture() const { return LastRenderedTexture; }

    // ‰∫ã‰ª∂
    UPROPERTY(BlueprintAssignable, Category = "AI Renderer Events")
    FOnSceneRendered OnSceneRendered;

    UPROPERTY(BlueprintAssignable, Category = "AI Renderer Events")
    FOnVideoFrameRendered OnVideoFrameRendered;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    // ?¥ÊôØÁµÑ‰ª∂
    UPROPERTY()
    class USceneCaptureComponent2D* SceneCapture;

    UPROPERTY()
    class UCameraComponent* CameraComponent;

    UPROPERTY()
    class UDirectionalLightComponent* DirectionalLight;

    UPROPERTY()
    class USkyLightComponent* SkyLight;

    UPROPERTY()
    class UExponentialHeightFogComponent* FogComponent;

    UPROPERTY()
    class UPostProcessComponent* PostProcessComponent;

    UPROPERTY()
    class UAudioComponent* AudioComponent;

    // Ê∏≤Ê??ÆÊ?
    UPROPERTY()
    UTextureRenderTarget2D* RenderTarget;

    // ?¥ÊôØ?çÁΩÆ
    UPROPERTY()
    FAISceneConfiguration CurrentSceneConfig;

    UPROPERTY()
    FAIRenderSettings CurrentRenderSettings;

    // AI ?ßÂÆπÁ≥ªÁµ±ÂºïÁî®
    UPROPERTY()
    TObjectPtr<UMingAIGeneratedContentSystem> AIContentSystem;

    // ?Ä??    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    bool bIsRendering;

    UPROPERTY()
    bool bRealTimeRendering;

    UPROPERTY()
    float RenderProgress;

    UPROPERTY()
    int32 CurrentFrame;

    UPROPERTY()
    float TotalFrames;

    UPROPERTY()
    UTexture2D* LastRenderedTexture;

    // ?ïÁï´?åÊ???    UPROPERTY()
    float ElapsedTime;

    UPROPERTY()
    float AnimationTime;

    UPROPERTY()
    FVector CameraStartPosition;

    UPROPERTY()
    FVector CameraEndPosition;

    UPROPERTY()
    float CameraAnimationDuration;

    // ?ßÈÉ®?ΩÊï∏
    void InitializeSceneComponents();
    void SetupDefaultScene();
    void UpdateLightingForTimeOfDay(ETimeOfDay TimeOfDay);
    void UpdateWeatherEffects(EWeatherType Weather);
    void UpdatePostProcessingForMood(ESceneMood Mood);
    void OptimizeRenderSettings(ERenderQuality Quality);
    void CaptureSceneFrame();
    void ProcessVideoFrame();
    void UpdateCameraAnimation(float DeltaTime);
    void UpdateEnvironmentalEffects(float DeltaTime);

    // ËºîÂä©?ΩÊï∏
    FLinearColor GetTimeOfDayColor(ETimeOfDay TimeOfDay) const;
    FVector GetLightDirection(ETimeOfDay TimeOfDay) const;
    float GetLightIntensity(ETimeOfDay TimeOfDay, EWeatherType Weather) const;
    FLinearColor GetFogColor(EWeatherType Weather) const;
    float GetFogDensity(EWeatherType Weather) const;
};
