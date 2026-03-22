// Copyright (c) 2026 MingGoRTS. All rights reserved.
// VR/AR UI Manager Implementation

#include "VRAR/MingVRARUIManager.h"
#include "VRAR/MingVRARSystem.h"
#include "Components/WidgetComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

UMingVRARUIManager::UMingVRARUIManager()
    : CurrentInteractionMode(EVRUIInteractionMode::LaserPointer)
    , PanelFadeDistance(500.0f)
    , bComfortModeEnabled(false)
    , bDynamicLODEnabled(true)
    , CurvatureAmount(0.3f)
    , UnitCommandWheel(nullptr)
    , ResourcePanel(nullptr)
{
}

void UMingVRARUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UE_LOG(LogTemp, Log, TEXT("MingVRARUIManager: Initializing VR/AR UI manager..."));

    // Get VR/AR system reference
    if (GetWorld() && GetWorld()->GetGameInstance())
    {
        VRARSystem = GetWorld()->GetGameInstance()->GetSubsystem<UMingVRARSystem>();
    }

    UE_LOG(LogTemp, Log, TEXT("MingVRARUIManager: VR/AR UI manager initialized"));
}

void UMingVRARUIManager::Deinitialize()
{
    UE_LOG(LogTemp, Log, TEXT("MingVRARUIManager: Deinitializing..."));

    // Clean up all panels
    for (const auto& Pair : PanelWidgets)
    {
        if (Pair.Value)
        {
            Pair.Value->DestroyComponent();
        }
    }
    PanelWidgets.Empty();
    ActivePanels.Empty();

    Super::Deinitialize();
}

void UMingVRARUIManager::Tick(float DeltaTime)
{
    if (!VRARSystem || VRARSystem->GetSessionState() != EVRARSessionState::Running)
    {
        return;
    }

    UpdatePanelPositions();
    UpdateInteractionLaser();
    FadePanelsByDistance();

    if (bDynamicLODEnabled)
    {
        UpdateDynamicLOD();
    }

    if (CurrentInteractionMode == EVRUIInteractionMode::Gaze)
    {
        // Gaze interaction handled in ProcessGazeInput
    }
}

bool UMingVRARUIManager::ShouldCreateSubsystem(UObject* Outer) const
{
    // Only create if VR/AR is potentially available
    return true;
}

TStatId UMingVRARUIManager::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UMingVRARUIManager, STATGROUP_Tickables);
}

void UMingVRARUIManager::CreateVRPanel(EVRUIPanelType PanelType, const FString& PanelName, const FVRUILayout& Layout)
{
    if (PanelWidgets.Contains(PanelName))
    {
        UE_LOG(LogTemp, Warning, TEXT("MingVRARUIManager: Panel %s already exists, updating layout"), *PanelName);
        UpdateVRPanelLayout(PanelName, Layout);
        return;
    }

    // Create widget component
    UWidgetComponent* WidgetComp = NewObject<UWidgetComponent>(GetWorld()->GetCurrentLevel());
    if (WidgetComp)
    {
        WidgetComp->SetWidgetSpace(EWidgetSpace::World);
        WidgetComp->SetDrawSize(FVector2D(1000.0f, 600.0f)); // Default size
        WidgetComp->SetPivot(FVector2D(0.5f, 0.5f));
        WidgetComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        WidgetComp->RegisterComponent();

        // Position panel
        FVector PlayerPos = VRARSystem->GetHeadPosition();
        FRotator PlayerRot = VRARSystem->GetHeadRotation();
        FVector PanelPos = CalculatePanelPosition(Layout, PlayerPos, PlayerRot);
        FRotator PanelRot = CalculatePanelRotation(Layout, PlayerPos);

        WidgetComp->SetWorldLocation(PanelPos);
        WidgetComp->SetWorldRotation(PanelRot);

        // Store panel
        PanelWidgets.Add(PanelName, WidgetComp);

        FVRUIPanel NewPanel;
        NewPanel.PanelType = PanelType;
        NewPanel.PanelName = PanelName;
        NewPanel.Layout = Layout;
        NewPanel.bIsVisible = true;
        ActivePanels.Add(NewPanel);

        ApplyCurvatureToPanel(WidgetComp, CurvatureAmount);

        UE_LOG(LogTemp, Log, TEXT("MingVRARUIManager: Created panel %s of type %s"),
            *PanelName, *UEnum::GetValueAsString(PanelType));
    }
}

void UMingVRARUIManager::DestroyVRPanel(const FString& PanelName)
{
    if (PanelWidgets.Contains(PanelName))
    {
        if (UWidgetComponent* WidgetComp = PanelWidgets[PanelName])
        {
            WidgetComp->DestroyComponent();
        }
        PanelWidgets.Remove(PanelName);
        ActivePanels.RemoveAll([&](const FVRUIPanel& Panel) { return Panel.PanelName == PanelName; });

        UE_LOG(LogTemp, Log, TEXT("MingVRARUIManager: Destroyed panel %s"), *PanelName);
    }
}

void UMingVRARUIManager::ShowVRPanel(const FString& PanelName)
{
    if (PanelWidgets.Contains(PanelName))
    {
        if (UWidgetComponent* WidgetComp = PanelWidgets[PanelName])
        {
            WidgetComp->SetVisibility(true);
            AnimatePanelTransition(PanelName, true);
        }

        for (auto& Panel : ActivePanels)
        {
            if (Panel.PanelName == PanelName)
            {
                Panel.bIsVisible = true;
                break;
            }
        }

        OnPanelShown.Broadcast(PanelName);
    }
}

void UMingVRARUIManager::HideVRPanel(const FString& PanelName)
{
    if (PanelWidgets.Contains(PanelName))
    {
        AnimatePanelTransition(PanelName, false);

        for (auto& Panel : ActivePanels)
        {
            if (Panel.PanelName == PanelName)
            {
                Panel.bIsVisible = false;
                break;
            }
        }

        OnPanelHidden.Broadcast(PanelName);
    }
}

void UMingVRARUIManager::UpdateVRPanelLayout(const FString& PanelName, const FVRUILayout& NewLayout)
{
    if (PanelWidgets.Contains(PanelName))
    {
        if (UWidgetComponent* WidgetComp = PanelWidgets[PanelName])
        {
            FVector PlayerPos = VRARSystem->GetHeadPosition();
            FRotator PlayerRot = VRARSystem->GetHeadRotation();

            FVector NewPos = CalculatePanelPosition(NewLayout, PlayerPos, PlayerRot);
            FRotator NewRot = CalculatePanelRotation(NewLayout, PlayerPos);

            WidgetComp->SetWorldLocation(NewPos);
            WidgetComp->SetWorldRotation(NewRot);

            ApplyCurvatureToPanel(WidgetComp, CurvatureAmount);
        }

        for (auto& Panel : ActivePanels)
        {
            if (Panel.PanelName == PanelName)
            {
                Panel.Layout = NewLayout;
                break;
            }
        }
    }
}

bool UMingVRARUIManager::IsPanelVisible(const FString& PanelName) const
{
    for (const auto& Panel : ActivePanels)
    {
        if (Panel.PanelName == PanelName)
        {
            return Panel.bIsVisible;
        }
    }
    return false;
}

FVRUILayout UMingVRARUIManager::GetWristMenuLayout()
{
    FVRUILayout Layout;
    Layout.DistanceFromPlayer = 30.0f; // Close to wrist
    Layout.PanelScale = FVector2D(0.5f, 0.5f);
    Layout.bDockToWrist = true;
    Layout.bFacePlayer = true;
    Layout.Curvature = 0.5f;
    return Layout;
}

FVRUILayout UMingVRARUIManager::GetFloatingHUDLayout()
{
    FVRUILayout Layout;
    Layout.DistanceFromPlayer = 150.0f;
    Layout.PanelPosition = FVector(0.0f, 0.0f, -50.0f); // Slightly below eye level
    Layout.PanelScale = FVector2D(0.8f, 0.4f);
    Layout.bFollowPlayer = true;
    Layout.bFacePlayer = true;
    Layout.Curvature = 0.3f;
    return Layout;
}

FVRUILayout UMingVRARUIManager::GetWorldMapLayout()
{
    FVRUILayout Layout;
    Layout.DistanceFromPlayer = 200.0f;
    Layout.PanelPosition = FVector(0.0f, 0.0f, -100.0f);
    Layout.PanelScale = FVector2D(1.5f, 1.5f);
    Layout.bFollowPlayer = false; // Fixed in world
    Layout.Curvature = 0.6f;
    return Layout;
}

FVRUILayout UMingVRARUIManager::GetTheaterScreenLayout()
{
    FVRUILayout Layout;
    Layout.DistanceFromPlayer = 500.0f;
    Layout.PanelPosition = FVector(500.0f, 0.0f, 0.0f);
    Layout.PanelScale = FVector2D(2.0f, 1.125f); // 16:9
    Layout.Curvature = 0.0f; // Flat for theater feel
    return Layout;
}

FVRUILayout UMingVRARUIManager::GetCommandPaletteLayout()
{
    FVRUILayout Layout;
    Layout.DistanceFromPlayer = 100.0f;
    Layout.PanelScale = FVector2D(0.6f, 0.8f);
    Layout.bFollowPlayer = true;
    Layout.bFacePlayer = true;
    Layout.Curvature = 0.4f;
    return Layout;
}

void UMingVRARUIManager::SetUIInteractionMode(EVRUIInteractionMode NewMode)
{
    CurrentInteractionMode = NewMode;
    UE_LOG(LogTemp, Log, TEXT("MingVRARUIManager: UI interaction mode set to %s"),
        *UEnum::GetValueAsString(NewMode));
}

void UMingVRARUIManager::ProcessControllerInput(FVector ControllerPosition, FVector ControllerDirection, bool bTriggerPressed)
{
    if (CurrentInteractionMode != EVRUIInteractionMode::LaserPointer)
    {
        return;
    }

    FString HitPanel;
    if (RaycastAgainstUI(ControllerPosition, ControllerDirection, HitPanel))
    {
        // Calculate hit point
        FVector HitPoint = ControllerPosition + ControllerDirection * 100.0f; // Approximate
        HandlePanelInteraction(HitPanel, HitPoint, bTriggerPressed);
    }
}

void UMingVRARUIManager::ProcessHandGestureInput(const TArray<FVector>& FingerPositions, EVRHandGesture Gesture)
{
    // Hand gesture handling for direct touch
    if (CurrentInteractionMode == EVRUIInteractionMode::DirectTouch &&
        Gesture == EVRHandGesture::Point)
    {
        // Use index finger tip as pointing position
        if (FingerPositions.Num() > 0)
        {
            FVector FingerTip = FingerPositions[0];
            FVector FingerDirection = FVector::ForwardVector; // Approximate

            FString HitPanel;
            if (RaycastAgainstUI(FingerTip, FingerDirection, HitPanel))
            {
                HandlePanelInteraction(HitPanel, FingerTip, true);
            }
        }
    }
}

void UMingVRARUIManager::ProcessGazeInput(FVector GazeOrigin, FVector GazeDirection, float GazeDuration)
{
    if (CurrentInteractionMode != EVRUIInteractionMode::Gaze)
    {
        return;
    }

    FString HitPanel;
    if (RaycastAgainstUI(GazeOrigin, GazeDirection, HitPanel))
    {
        // Gaze dwell selection after 1 second
        if (GazeDuration >= 1.0f)
        {
            FVector HitPoint = GazeOrigin + GazeDirection * 200.0f;
            HandlePanelInteraction(HitPanel, HitPoint, true);
        }
    }
}

void UMingVRARUIManager::ShowUnitCommandWheel(const FVector& WorldPosition)
{
    if (!UnitCommandWheel)
    {
        UnitCommandWheel = NewObject<UWidgetComponent>(GetWorld()->GetCurrentLevel());
        UnitCommandWheel->SetWidgetSpace(EWidgetSpace::World);
        UnitCommandWheel->SetDrawSize(FVector2D(800.0f, 800.0f));
        UnitCommandWheel->RegisterComponent();
    }

    UnitCommandWheel->SetWorldLocation(WorldPosition + FVector(0.0f, 0.0f, 100.0f));
    UnitCommandWheel->SetVisibility(true);
    UnitCommandWheel->SetWorldRotation(FRotator(0.0f, VRARSystem->GetHeadRotation().Yaw, 0.0f));
}

void UMingVRARUIManager::HideUnitCommandWheel()
{
    if (UnitCommandWheel)
    {
        UnitCommandWheel->SetVisibility(false);
    }
}

void UMingVRARUIManager::UpdateResourceDisplay(float Wood, float Stone, float Gold, float Food)
{
    // Update resource panel with current values
    // This would be connected to a UMG widget
}

void UMingVRARUIManager::ShowBattleNotification(const FString& Message, float Duration)
{
    // Create temporary notification widget
    UE_LOG(LogTemp, Log, TEXT("MingVRARUIManager: Battle notification - %s"), *Message);

    // Could create a floating notification panel
}

void UMingVRARUIManager::EnableComfortMode(bool bEnable)
{
    bComfortModeEnabled = bEnable;
    ApplyComfortSettings();
}

void UMingVRARUIManager::SetPanelFadeDistance(float Distance)
{
    PanelFadeDistance = Distance;
}

void UMingVRARUIManager::EnableDynamicLOD(bool bEnable)
{
    bDynamicLODEnabled = bEnable;
}

// Internal implementations

void UMingVRARUIManager::UpdatePanelPositions()
{
    if (!VRARSystem)
    {
        return;
    }

    FVector PlayerPos = VRARSystem->GetHeadPosition();
    FRotator PlayerRot = VRARSystem->GetHeadRotation();

    for (auto& Panel : ActivePanels)
    {
        if (!Panel.bIsVisible)
        {
            continue;
        }

        if (PanelWidgets.Contains(Panel.PanelName))
        {
            UWidgetComponent* WidgetComp = PanelWidgets[Panel.PanelName];

            if (Panel.Layout.bFollowPlayer)
            {
                FVector NewPos = CalculatePanelPosition(Panel.Layout, PlayerPos, PlayerRot);
                FRotator NewRot = CalculatePanelRotation(Panel.Layout, PlayerPos);

                // Smooth interpolation
                FVector CurrentPos = WidgetComp->GetComponentLocation();
                FRotator CurrentRot = WidgetComp->GetComponentRotation();

                WidgetComp->SetWorldLocation(FMath::Lerp(CurrentPos, NewPos, 0.1f));
                WidgetComp->SetWorldRotation(FMath::Lerp(CurrentRot, NewRot, 0.1f));
            }
        }
    }
}

void UMingVRARUIManager::ApplyComfortSettings()
{
    if (bComfortModeEnabled)
    {
        // Reduce curvature for comfort
        CurvatureAmount = 0.1f;
        UE_LOG(LogTemp, Log, TEXT("MingVRARUIManager: Comfort mode enabled"));
    }
    else
    {
        CurvatureAmount = 0.3f;
    }
}

void UMingVRARUIManager::UpdateInteractionLaser()
{
    // Update visual laser pointer for controller interaction
}

void UMingVRARUIManager::CheckGazeInteractions()
{
    // Gaze tracking logic
}

void UMingVRARUIManager::PositionPanelAtWrist(const FString& PanelName)
{
    // Position panel attached to wrist position
}

void UMingVRARUIManager::PositionPanelInWorld(const FString& PanelName, const FVector& WorldPosition)
{
    if (PanelWidgets.Contains(PanelName))
    {
        PanelWidgets[PanelName]->SetWorldLocation(WorldPosition);
    }
}

void UMingVRARUIManager::PositionPanelFloating(const FString& PanelName)
{
    // Position panel floating in front of player
}

void UMingVRARUIManager::ApplyCurvatureToPanel(UWidgetComponent* Widget, float Curvature)
{
    // Apply cylinder curvature shader or mesh deformation
    // This is a placeholder - actual implementation would use material parameters
}

void UMingVRARUIManager::FadePanelsByDistance()
{
    if (!VRARSystem)
    {
        return;
    }

    FVector PlayerPos = VRARSystem->GetHeadPosition();

    for (const auto& Pair : PanelWidgets)
    {
        UWidgetComponent* Widget = Pair.Value;
        float Distance = FVector::Distance(Widget->GetComponentLocation(), PlayerPos);

        float Alpha = 1.0f;
        if (Distance > PanelFadeDistance)
        {
            Alpha = FMath::Max(0.0f, 1.0f - (Distance - PanelFadeDistance) / 100.0f);
        }

        // Apply opacity through widget interface
        // Widget->SetOpacity(Alpha);
    }
}

FVector UMingVRARUIManager::CalculatePanelPosition(const FVRUILayout& Layout, const FVector& PlayerPosition, const FRotator& PlayerRotation)
{
    if (Layout.bDockToWrist)
    {
        // Return wrist position (would need hand tracking data)
        return PlayerPosition + FVector(30.0f, 0.0f, -30.0f);
    }

    FVector Forward = PlayerRotation.Vector();
    FVector Right = FRotationMatrix(PlayerRotation).GetScaledAxis(EAxis::Y);
    FVector Up = FRotationMatrix(PlayerRotation).GetScaledAxis(EAxis::Z);

    FVector BasePosition = PlayerPosition + Forward * Layout.DistanceFromPlayer;
    FVector Offset = Right * Layout.PanelPosition.Y + Up * Layout.PanelPosition.Z;

    return BasePosition + Offset;
}

FRotator UMingVRARUIManager::CalculatePanelRotation(const FVRUILayout& Layout, const FVector& PlayerPosition)
{
    if (Layout.bFacePlayer)
    {
        FVector ToPlayer = (PlayerPosition - Layout.PanelPosition).GetSafeNormal();
        return ToPlayer.Rotation();
    }

    return Layout.PanelRotation;
}

bool UMingVRARUIManager::RaycastAgainstUI(FVector RayOrigin, FVector RayDirection, FString& OutHitPanel)
{
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

    if (GetWorld()->LineTraceSingleByChannel(HitResult, RayOrigin, RayOrigin + RayDirection * 1000.0f, ECC_Visibility, Params))
    {
        // Check if we hit a widget component
        UWidgetComponent* HitWidget = Cast<UWidgetComponent>(HitResult.GetComponent());
        if (HitWidget)
        {
            // Find which panel this widget belongs to
            for (const auto& Pair : PanelWidgets)
            {
                if (Pair.Value == HitWidget)
                {
                    OutHitPanel = Pair.Key;
                    return true;
                }
            }
        }
    }

    return false;
}

void UMingVRARUIManager::HandlePanelInteraction(const FString& PanelName, FVector HitPoint, bool bPressed)
{
    OnUIInteraction.Broadcast(PanelName, HitPoint);

    // Forward interaction to the panel's widget
    // This would involve UMG widget interaction
}

void UMingVRARUIManager::AnimatePanelTransition(const FString& PanelName, bool bShowing)
{
    // Animation would be handled by UMG or timeline
    if (PanelWidgets.Contains(PanelName))
    {
        UWidgetComponent* Widget = PanelWidgets[PanelName];
        // Set visibility immediately for now
        Widget->SetVisibility(bShowing);
    }
}

void UMingVRARUIManager::UpdateDynamicLOD()
{
    // Adjust panel resolution based on distance
}
