// Copyright (c) 2026 MingGoRTS. All rights reserved.
// VR/AR UI Manager - Handles 3D UI adaptation for VR/AR environments

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MingVRARSystem.h"
#include "MingVRARUIManager.generated.h"

// VR UI panel types
UENUM(BlueprintType)
enum class EVRUIPanelType: uint8 {
    MainMenu            UMETA(DisplayName = "Main Menu"),
    HUD                 UMETA(DisplayName = "HUD"),
    UnitSelection       UMETA(DisplayName = "Unit Selection"),
    CommandMenu         UMETA(DisplayName = "Command Menu"),
    ResourcePanel       UMETA(DisplayName = "Resource Panel"),
    Map                 UMETA(DisplayName = "Mini Map"),
    Dialog              UMETA(DisplayName = "Dialog"),
    Settings            UMETA(DisplayName = "Settings"),
    PauseMenu           UMETA(DisplayName = "Pause Menu"),
    VictoryDefeat       UMETA(DisplayName = "Victory/Defeat"),
    Custom              UMETA(DisplayName = "Custom")
};

// VR UI interaction modes
UENUM(BlueprintType)
enum class EVRUIInteractionMode: uint8 {
    LaserPointer        UMETA(DisplayName = "Laser Pointer"),
    DirectTouch         UMETA(DisplayName = "Direct Touch"),
    Gaze                UMETA(DisplayName = "Gaze Selection"),
    HandGesture         UMETA(DisplayName = "Hand Gesture"),
    Voice               UMETA(DisplayName = "Voice Control")
};

// VR UI layout configurations
USTRUCT(BlueprintType)
struct FVRUILayout
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector PanelPosition = FVector(100.0f, 0.0f, 0.0f); // In front of player

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRotator PanelRotation = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D PanelScale = FVector2D(1.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DistanceFromPlayer = 200.0f; // cm

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Curvature = 0.0f; // 0 = flat, 1 = fully curved

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bFacePlayer = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bFollowPlayer = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bDockToWrist = false;
};

// VR UI panel data
USTRUCT(BlueprintType)
struct FVRUIPanel
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EVRUIPanelType PanelType = EVRUIPanelType::HUD;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PanelName = TEXT("DefaultPanel");

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVRUILayout Layout;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsVisible = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Opacity = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bInteractive = true;
};

// VR/AR UI Manager - Manages 3D UI elements in VR/AR space
UCLASS()
class MINGPERSONAL_API UMingVRARUIManager : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UMingVRARUIManager();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual void Tick(float DeltaTime) override;
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
    virtual TStatId GetStatId() const override;

    // Panel management
    UFUNCTION(BlueprintCallable, Category = "VR/AR UI|Panels")
    void CreateVRPanel(EVRUIPanelType PanelType, const FString& PanelName, const FVRUILayout& Layout);

    UFUNCTION(BlueprintCallable, Category = "VR/AR UI|Panels")
    void DestroyVRPanel(const FString& PanelName);

    UFUNCTION(BlueprintCallable, Category = "VR/AR UI|Panels")
    void ShowVRPanel(const FString& PanelName);

    UFUNCTION(BlueprintCallable, Category = "VR/AR UI|Panels")
    void HideVRPanel(const FString& PanelName);

    UFUNCTION(BlueprintCallable, Category = "VR/AR UI|Panels")
    void UpdateVRPanelLayout(const FString& PanelName, const FVRUILayout& NewLayout);

    UFUNCTION(BlueprintPure, Category = "VR/AR UI|Panels")
    bool IsPanelVisible(const FString& PanelName) const;

    // Layout presets
    UFUNCTION(BlueprintCallable, Category = "VR/AR UI|Layouts")
    FVRUILayout GetWristMenuLayout();

    UFUNCTION(BlueprintCallable, Category = "VR/AR UI|Layouts")
    FVRUILayout GetFloatingHUDLayout();

    UFUNCTION(BlueprintCallable, Category = "VR/AR UI|Layouts")
    FVRUILayout GetWorldMapLayout();

    UFUNCTION(BlueprintCallable, Category = "VR/AR UI|Layouts")
    FVRUILayout GetTheaterScreenLayout();

    UFUNCTION(BlueprintCallable, Category = "VR/AR UI|Layouts")
    FVRUILayout GetCommandPaletteLayout();

    // Interaction mode
    UFUNCTION(BlueprintCallable, Category = "VR/AR UI|Interaction")
    void SetUIInteractionMode(EVRUIInteractionMode NewMode);

    UFUNCTION(BlueprintPure, Category = "VR/AR UI|Interaction")
    EVRUIInteractionMode GetUIInteractionMode() const { return CurrentInteractionMode; }

    // Input handling
    UFUNCTION(BlueprintCallable, Category = "VR/AR UI|Input")
    void ProcessControllerInput(FVector ControllerPosition, FVector ControllerDirection, bool bTriggerPressed);

    UFUNCTION(BlueprintCallable, Category = "VR/AR UI|Input")
    void ProcessHandGestureInput(const TArray<FVector>& FingerPositions, EVRHandGesture Gesture);

    UFUNCTION(BlueprintCallable, Category = "VR/AR UI|Input")
    void ProcessGazeInput(FVector GazeOrigin, FVector GazeDirection, float GazeDuration);

    // RTS-specific UI
    UFUNCTION(BlueprintCallable, Category = "VR/AR UI|RTS")
    void ShowUnitCommandWheel(const FVector& WorldPosition);

    UFUNCTION(BlueprintCallable, Category = "VR/AR UI|RTS")
    void HideUnitCommandWheel();

    UFUNCTION(BlueprintCallable, Category = "VR/AR UI|RTS")
    void UpdateResourceDisplay(float Wood, float Stone, float Gold, float Food);

    UFUNCTION(BlueprintCallable, Category = "VR/AR UI|RTS")
    void ShowBattleNotification(const FString& Message, float Duration);

    // Comfort options
    UFUNCTION(BlueprintCallable, Category = "VR/AR UI|Comfort")
    void EnableComfortMode(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "VR/AR UI|Comfort")
    void SetPanelFadeDistance(float Distance);

    UFUNCTION(BlueprintCallable, Category = "VR/AR UI|Comfort")
    void EnableDynamicLOD(bool bEnable);

    // Events
    UPROPERTY(BlueprintAssignable, Category = "VR/AR UI|Events")
    FOnVRPanelShown OnPanelShown;

    UPROPERTY(BlueprintAssignable, Category = "VR/AR UI|Events")
    FOnVRPanelHidden OnPanelHidden;

    UPROPERTY(BlueprintAssignable, Category = "VR/AR UI|Events")
    FOnVRUIInteraction OnUIInteraction;

protected:
    // Configuration
    UPROPERTY()
    EVRUIInteractionMode CurrentInteractionMode = EVRUIInteractionMode::LaserPointer;

    UPROPERTY()
    TArray<FVRUIPanel> ActivePanels;

    UPROPERTY()
    TMap<FString, class UWidgetComponent*> PanelWidgets;

    UPROPERTY()
    float PanelFadeDistance = 500.0f;

    UPROPERTY()
    bool bComfortModeEnabled = false;

    UPROPERTY()
    bool bDynamicLODEnabled = true;

    UPROPERTY()
    float CurvatureAmount = 0.3f;

    // Internal methods
    void UpdatePanelPositions();
    void ApplyComfortSettings();
    void UpdateInteractionLaser();
    void CheckGazeInteractions();
    void PositionPanelAtWrist(const FString& PanelName);
    void PositionPanelInWorld(const FString& PanelName, const FVector& WorldPosition);
    void PositionPanelFloating(const FString& PanelName);
    void ApplyCurvatureToPanel(class UWidgetComponent* Widget, float Curvature);
    void FadePanelsByDistance();
    FVector CalculatePanelPosition(const FVRUILayout& Layout, const FVector& PlayerPosition, const FRotator& PlayerRotation);
    FRotator CalculatePanelRotation(const FVRUILayout& Layout, const FVector& PlayerPosition);
    bool RaycastAgainstUI(FVector RayOrigin, FVector RayDirection, FString& OutHitPanel);
    void HandlePanelInteraction(const FString& PanelName, FVector HitPoint, bool bPressed);
    void AnimatePanelTransition(const FString& PanelName, bool bShowing);
    void UpdateDynamicLOD();

    // RTS-specific
    UPROPERTY()
    class UWidgetComponent* UnitCommandWheel = nullptr;

    UPROPERTY()
    class UWidgetComponent* ResourcePanel = nullptr;

    UPROPERTY()
    TArray<class UWidgetComponent*> NotificationWidgets;

    // Cached references
    UPROPERTY()
    TObjectPtr<UMingVRARSystem> VRARSystem = nullptr;
};

// Event delegates
declare dynamic multicast delegate(FOnVRPanelShown, const FString&, PanelName);
declare dynamic multicast delegate(FOnVRPanelHidden, const FString&, PanelName);
declare dynamic multicast delegate(FOnVRUIInteraction, const FString&, PanelName, FVector, HitPoint);

// Hand gesture enum for input
UENUM(BlueprintType)
enum class EVRHandGesture: uint8 {
    None                UMETA(DisplayName = "None"),
    Point               UMETA(DisplayName = "Point"),
    Pinch               UMETA(DisplayName = "Pinch"),
    Grab                UMETA(DisplayName = "Grab"),
    OpenPalm            UMETA(DisplayName = "Open Palm"),
    ThumbsUp            UMETA(DisplayName = "Thumbs Up"),
    Fist                UMETA(DisplayName = "Fist")
};
