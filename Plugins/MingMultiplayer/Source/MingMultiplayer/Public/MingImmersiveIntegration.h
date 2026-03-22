#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MingMultiplayerTypes.h"
#include "MingEducationalManager.h"
#include "MingVRManager.h"
#include "MingARManager.h"
#include "MingImmersiveIntegration.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnImmersiveModeChanged, EMingImmersiveMode, OldMode, EMingImmersiveMode, NewMode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEducationalContentReady, const FMingEducationalContent&, Content);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVRSceneGenerated, const FString&, SceneID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnARContentPlaced, const FString&, ContentID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLearningProgressUpdated, const FString&, PlayerID, const FMingEducationalProgress&, Progress);

UENUM(BlueprintType)
enum class EMingImmersiveMode : uint8
{
    None            = 0,
    Educational     = 1,
    VR              = 2,
    AR              = 3,
    Mixed           = 4,
    Hybrid          = 5
};

USTRUCT(BlueprintType)
struct FMingImmersiveConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersive Config")
    EMingImmersiveMode PrimaryMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersive Config")
    bool bEducationalContentEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersive Config")
    bool bVREnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersive Config")
    bool bAREnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersive Config")
    EMingEducationalMode EducationalMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersive Config")
    EMingVRMode VRMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersive Config")
    bool bAREducationalMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersive Config")
    float TransitionDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersive Config")
    bool bAutoSwitchModes;

    FMingImmersiveConfig()
    {
        PrimaryMode = EMingImmersiveMode::None;
        bEducationalContentEnabled = false;
        bVREnabled = false;
        bAREnabled = false;
        EducationalMode = EMingEducationalMode::Disabled;
        VRMode = EMingVRMode::Disabled;
        bAREducationalMode = false;
        TransitionDuration = 2.0f;
        bAutoSwitchModes = true;
    }
};

USTRUCT(BlueprintType)
struct FMingEducationalVRContent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational VR Content")
    FString ContentID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational VR Content")
    FString LessonID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational VR Content")
    FString VRSceneID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational VR Content")
    FVector3d WorldPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational VR Content")
    FRotator3d WorldRotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational VR Content")
    FVector3d Scale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational VR Content")
    bool bInteractive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational VR Content")
    TArray<FString> RequiredAssets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational VR Content")
    TArray<FString> AudioClips;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational VR Content")
    FString Description;

    FMingEducationalVRContent()
    {
        ContentID = TEXT("");
        LessonID = TEXT("");
        VRSceneID = TEXT("");
        WorldPosition = FVector3d::ZeroVector;
        WorldRotation = FRotator3d::ZeroRotator;
        Scale = FVector3d::OneVector;
        bInteractive = false;
        Description = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FMingEducationalARContent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational AR Content")
    FString ContentID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational AR Content")
    FString HistoricalMarkerID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational AR Content")
    FString AnchorID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational AR Content")
    EMingARPlaneType TargetPlaneType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational AR Content")
    FVector3d LocalPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational AR Content")
    FRotator3d LocalRotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational AR Content")
    FVector3d Scale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational AR Content")
    bool bPersistent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational AR Content")
    TArray<FString> MediaAssets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational AR Content")
    FString EducationalText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational AR Content")
    TArray<FString> InteractiveElements;

    FMingEducationalARContent()
    {
        ContentID = TEXT("");
        HistoricalMarkerID = TEXT("");
        AnchorID = TEXT("");
        TargetPlaneType = EMingARPlaneType::HorizontalUp;
        LocalPosition = FVector3d::ZeroVector;
        LocalRotation = FRotator3d::ZeroRotator;
        Scale = FVector3d::OneVector;
        bPersistent = false;
        EducationalText = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FMingImmersiveTransition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersive Transition")
    EMingImmersiveMode FromMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersive Transition")
    EMingImmersiveMode ToMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersive Transition")
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersive Transition")
    bool bFadeToBlack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersive Transition")
    bool bLoadAssets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersive Transition")
    TArray<FString> AssetsToLoad;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immersive Transition")
    FString TransitionEffect;

    FMingImmersiveTransition()
    {
        FromMode = EMingImmersiveMode::None;
        ToMode = EMingImmersiveMode::None;
        Duration = 2.0f;
        bFadeToBlack = true;
        bLoadAssets = true;
        TransitionEffect = TEXT("Default");
    }
};

/**
 * Unified Immersive Experience Manager
 * Integrates educational, VR, and AR systems for seamless immersive learning experiences
 */
UCLASS(ClassGroup = (Immersive, Learning), BlueprintType)
class MINGMULTIPLAYER_API UMingImmersiveIntegration : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UMingImmersiveIntegration();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override { return true; }
    virtual TStatId GetStatId() const override;

    // Immersive Mode Management
    UFUNCTION(BlueprintCallable, Category = "Immersive")
    bool SetImmersiveMode(EMingImmersiveMode Mode);

    UFUNCTION(BlueprintCallable, Category = "Immersive")
    bool SwitchToMode(EMingImmersiveMode Mode, bool bSmoothTransition = true);

    UFUNCTION(BlueprintPure, Category = "Immersive")
    EMingImmersiveMode GetCurrentImmersiveMode() const;

    UFUNCTION(BlueprintCallable, Category = "Immersive")
    void ConfigureImmersiveExperience(const FMingImmersiveConfig& Config);

    UFUNCTION(BlueprintPure, Category = "Immersive")
    FMingImmersiveConfig GetCurrentConfig() const;

    // Educational-VR Integration
    UFUNCTION(BlueprintCallable, Category = "Educational VR")
    bool CreateEducationalVRScene(const FString& LessonID, const FVector3d& Location, const FRotator3d& Rotation);

    UFUNCTION(BlueprintCallable, Category = "Educational VR")
    bool LoadEducationalVRContent(const FString& ContentID);

    UFUNCTION(BlueprintCallable, Category = "Educational VR")
    void UnloadEducationalVRContent(const FString& ContentID);

    UFUNCTION(BlueprintPure, Category = "Educational VR")
    TArray<FMingEducationalVRContent> GetLoadedVRContent() const;

    UFUNCTION(BlueprintCallable, Category = "Educational VR")
    bool StartVRHistoricalTour(const TArray<FString>& HistoricalEventIDs);

    UFUNCTION(BlueprintCallable, Category = "Educational VR")
    void StopVRHistoricalTour();

    // Educational-AR Integration
    UFUNCTION(BlueprintCallable, Category = "Educational AR")
    bool PlaceEducationalARContent(const FString& HistoricalMarkerID, const FVector3d& Location);

    UFUNCTION(BlueprintCallable, Category = "Educational AR")
    bool CreateARHistoricalTimeline(const TArray<FString>& EventIDs);

    UFUNCTION(BlueprintCallable, Category = "Educational AR")
    bool StartAREducationalMode();

    UFUNCTION(BlueprintCallable, Category = "Educational AR")
    void StopAREducationalMode();

    UFUNCTION(BlueprintPure, Category = "Educational AR")
    TArray<FMingEducationalARContent> GetPlacedARContent() const;

    // Cross-Platform Learning
    UFUNCTION(BlueprintCallable, Category = "Cross-Platform")
    bool SyncLearningProgressAcrossPlatforms();

    UFUNCTION(BlueprintCallable, Category = "Cross-Platform")
    void TransferLearningData(EMingImmersiveMode FromPlatform, EMingImmersiveMode ToPlatform);

    UFUNCTION(BlueprintPure, Category = "Cross-Platform")
    bool IsCrossPlatformSyncEnabled() const;

    // Adaptive Immersive Learning
    UFUNCTION(BlueprintCallable, Category = "Adaptive Learning")
    void EnableAdaptiveImmersiveLearning(bool bEnabled);

    UFUNCTION(BlueprintPure, Category = "Adaptive Learning")
    bool IsAdaptiveImmersiveLearningEnabled() const;

    UFUNCTION(BlueprintCallable, Category = "Adaptive Learning")
    void UpdateImmersiveDifficultyBasedOnPerformance();

    UFUNCTION(BlueprintCallable, Category = "Adaptive Learning")
    FMingEducationalContent GetAdaptedImmersiveContent(const FString& ContentID);

    // Multi-User Immersive Learning
    UFUNCTION(BlueprintCallable, Category = "Multi-User")
    bool EnableMultiUserImmersiveLearning();

    UFUNCTION(BlueprintCallable, Category = "Multi-User")
    void StartCooperativeImmersiveSession(const TArray<int32>& PlayerIDs);

    UFUNCTION(BlueprintCallable, Category = "Multi-User")
    void ShareImmersiveLearningProgress();

    UFUNCTION(BlueprintCallable, Category = "Multi-User")
    void ReceiveSharedProgress(const FMingEducationalProgress& Progress);

    // Immersive Analytics
    UFUNCTION(BlueprintPure, Category = "Analytics")
    FMingEducationalProgress GetImmersiveLearningAnalytics() const;

    UFUNCTION(BlueprintPure, Category = "Analytics")
    float GetImmersiveEngagementScore() const;

    UFUNCTION(BlueprintPure, Category = "Analytics")
    TArray<EMingImmersiveMode> GetMostUsedModes() const;

    UFUNCTION(BlueprintCallable, Category = "Analytics")
    void GenerateImmersiveLearningReport();

    // Content Management
    UFUNCTION(BlueprintCallable, Category = "Content Management")
    bool PreloadImmersiveContent(const TArray<FString>& ContentIDs);

    UFUNCTION(BlueprintCallable, Category = "Content Management")
    void UnloadUnusedContent();

    UFUNCTION(BlueprintPure, Category = "Content Management")
    float GetContentLoadProgress() const;

    UFUNCTION(BlueprintCallable, Category = "Content Management")
    void OptimizeContentForCurrentPlatform();

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnImmersiveModeChanged OnImmersiveModeChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnEducationalContentReady OnEducationalContentReady;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnVRSceneGenerated OnVRSceneGenerated;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnARContentPlaced OnARContentPlaced;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnLearningProgressUpdated OnLearningProgressUpdated;

protected:
    UPROPERTY()
    EMingImmersiveMode CurrentImmersiveMode;

    UPROPERTY()
    FMingImmersiveConfig CurrentConfig;

    UPROPERTY()
    TArray<FMingEducationalVRContent> LoadedVRContent;

    UPROPERTY()
    TArray<FMingEducationalARContent> PlacedARContent;

    UPROPERTY()
    TArray<FMingImmersiveTransition> TransitionQueue;

    UPROPERTY()
    bool bAdaptiveImmersiveLearningEnabled;

    UPROPERTY()
    bool bMultiUserImmersiveLearningEnabled;

    UPROPERTY()
    bool bCrossPlatformSyncEnabled;

    UPROPERTY()
    float CurrentTransitionProgress;

    UPROPERTY()
    float LastUpdateTime;

    UPROPERTY()
    float UpdateInterval;

    // Subsystem References
    UPROPERTY()
    TObjectPtr<UMingEducationalManager> EducationalManager;

    UPROPERTY()
    TObjectPtr<UMingVRManager> VRManager;

    UPROPERTY()
    TObjectPtr<UMingARManager> ARManager;

    // Internal Functions
    void UpdateImmersiveIntegration(float DeltaTime);
    void ProcessModeTransitions(float DeltaTime);
    void UpdateEducationalContent(float DeltaTime);
    void UpdateVRContent(float DeltaTime);
    void UpdateARContent(float DeltaTime);
    void SyncEducationalSystems(float DeltaTime);
    void ValidateImmersiveState();

    // Transition Management
    void StartModeTransition(EMingImmersiveMode FromMode, EMingImmersiveMode ToMode);
    void ProcessTransition(float DeltaTime);
    void CompleteModeTransition(EMingImmersiveMode NewMode);
    void CancelCurrentTransition();

    // Content Integration
    void IntegrateEducationalContentWithVR(const FString& LessonID);
    void IntegrateEducationalContentWithAR(const FString& HistoricalMarkerID);
    void GenerateCrossPlatformContent(const FString& ContentID);
    void OptimizeContentForMode(EMingImmersiveMode Mode);

    // Learning Analytics
    void UpdateLearningAnalytics(float DeltaTime);
    void CalculateEngagementScore();
    void TrackModeUsage(EMingImmersiveMode Mode);
    void GenerateLearningInsights();

private:
    bool ValidateImmersiveConfig(const FMingImmersiveConfig& Config) const;
    bool CanTransitionToMode(EMingImmersiveMode Mode) const;
    FString GenerateContentID() const;
    void CleanupImmersiveResources();
    void ResetImmersiveSystem();
};
