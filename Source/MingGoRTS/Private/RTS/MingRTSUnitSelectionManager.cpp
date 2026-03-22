// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTS/MingRTSUnitSelectionManager.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Components/DecalComponent.h"

UMingRTSUnitSelectionManager::UMingRTSUnitSelectionManager()
{
    CurrentSelectionMode = ERTSUnitSelectionMode::Single;
}

void UMingRTSUnitSelectionManager::InitializeSelectionManager()
{
    ClearSelection();
    UnitGroups.Empty();
    UE_LOG(LogTemp, Log, TEXT("RTS Unit Selection Manager initialized"));
}

void UMingRTSUnitSelectionManager::SelectUnit(AActor* Unit, bool bAdditive)
{
    if (!Unit)
    {
        return;
    }

    if (!bAdditive)
    {
        ClearSelection();
    }

    if (!SelectedUnits.Contains(Unit))
    {
        SelectedUnits.Add(Unit);
        UpdateSelectionVisuals();
        OnSelectionChanged.Broadcast();
    }
}

void UMingRTSUnitSelectionManager::SelectUnitsInBox(const FVector2D& StartScreenPos, const FVector2D& EndScreenPos, bool bAdditive)
{
    if (!bAdditive)
    {
        ClearSelection();
    }

    TArray<AActor*> UnitsInBox = GetUnitsInScreenBox(StartScreenPos, EndScreenPos);
    
    for (AActor* Unit : UnitsInBox)
    {
        if (!SelectedUnits.Contains(Unit))
        {
            SelectedUnits.Add(Unit);
        }
    }

    UpdateSelectionVisuals();
    OnSelectionChanged.Broadcast();

    UE_LOG(LogTemp, Log, TEXT("Selected %d units in box"), UnitsInBox.Num());
}

void UMingRTSUnitSelectionManager::DeselectUnit(AActor* Unit)
{
    if (SelectedUnits.Contains(Unit))
    {
        SelectedUnits.Remove(Unit);
        UpdateSelectionVisuals();
        OnSelectionChanged.Broadcast();
    }
}

void UMingRTSUnitSelectionManager::ClearSelection()
{
    for (AActor* Unit : SelectedUnits)
    {
        if (Unit)
        {
            UDecalComponent* SelectionDecal = Unit->FindComponentByClass<UDecalComponent>();
            if (SelectionDecal)
            {
                SelectionDecal->SetVisibility(false);
            }
        }
    }

    SelectedUnits.Empty();
    OnSelectionChanged.Broadcast();
}

void UMingRTSUnitSelectionManager::SelectAllUnits()
{
    ClearSelection();

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    for (TActorIterator<AActor> It(World); It; ++It)
    {
        AActor* Actor = *It;
        if (Actor && Actor->ActorHasTag(FName("SelectableUnit")))
        {
            SelectedUnits.Add(Actor);
        }
    }

    UpdateSelectionVisuals();
    OnSelectionChanged.Broadcast();

    UE_LOG(LogTemp, Log, TEXT("Selected all %d units"), SelectedUnits.Num());
}

bool UMingRTSUnitSelectionManager::IsUnitSelected(AActor* Unit) const
{
    return SelectedUnits.Contains(Unit);
}

void UMingRTSUnitSelectionManager::IssueMoveCommand(const FVector& TargetLocation, ERTSMoveCommandType CommandType)
{
    if (SelectedUnits.Num() == 0)
    {
        return;
    }

    // 为每个选中的单位发出移动命令
    for (AActor* Unit : SelectedUnits)
    {
        if (Unit)
        {
            // 这里可以调用单位的移动函数
            // 例如: Cast<IMingRTSUnitInterface>(Unit)->Execute_MoveToLocation(Unit, TargetLocation);
        }
    }

    OnMoveCommandIssued.Broadcast(TargetLocation);
    UE_LOG(LogTemp, Log, TEXT("Issued move command to %d units"), SelectedUnits.Num());
}

void UMingRTSUnitSelectionManager::IssueMoveCommandToUnit(AActor* TargetUnit, ERTSMoveCommandType CommandType)
{
    if (!TargetUnit || SelectedUnits.Num() == 0)
    {
        return;
    }

    FVector TargetLocation = TargetUnit->GetActorLocation();
    IssueMoveCommand(TargetLocation, CommandType);
}

void UMingRTSUnitSelectionManager::AssignSelectionToGroup(int32 GroupIndex)
{
    if (GroupIndex >= 0 && GroupIndex <= 9)
    {
        UnitGroups.Add(GroupIndex, SelectedUnits);
        UE_LOG(LogTemp, Log, TEXT("Assigned %d units to group %d"), SelectedUnits.Num(), GroupIndex);
    }
}

void UMingRTSUnitSelectionManager::RecallGroup(int32 GroupIndex)
{
    if (UnitGroups.Contains(GroupIndex))
    {
        ClearSelection();
        SelectedUnits = UnitGroups[GroupIndex];
        UpdateSelectionVisuals();
        OnSelectionChanged.Broadcast();
        UE_LOG(LogTemp, Log, TEXT("Recalled group %d with %d units"), GroupIndex, SelectedUnits.Num());
    }
}

void UMingRTSUnitSelectionManager::AddToGroup(int32 GroupIndex)
{
    if (GroupIndex >= 0 && GroupIndex <= 9)
    {
        TArray<AActor*>& Group = UnitGroups.FindOrAdd(GroupIndex);
        for (AActor* Unit : SelectedUnits)
        {
            if (!Group.Contains(Unit))
            {
                Group.Add(Unit);
            }
        }
    }
}

void UMingRTSUnitSelectionManager::RemoveFromGroup(int32 GroupIndex)
{
    if (UnitGroups.Contains(GroupIndex))
    {
        TArray<AActor*>& Group = UnitGroups[GroupIndex];
        for (AActor* Unit : SelectedUnits)
        {
            Group.Remove(Unit);
        }
    }
}

void UMingRTSUnitSelectionManager::UpdateSelectionVisuals()
{
    for (AActor* Unit : SelectedUnits)
    {
        if (Unit)
        {
            UDecalComponent* SelectionDecal = Unit->FindComponentByClass<UDecalComponent>();
            if (SelectionDecal)
            {
                SelectionDecal->SetVisibility(true);
            }
        }
    }
}

void UMingRTSUnitSelectionManager::ClearInvalidSelections()
{
    for (int32 i = SelectedUnits.Num() - 1; i >= 0; --i)
    {
        if (!SelectedUnits[i] || SelectedUnits[i]->IsPendingKillPending())
        {
            SelectedUnits.RemoveAt(i);
        }
    }
}

TArray<AActor*> UMingRTSUnitSelectionManager::GetUnitsInScreenBox(const FVector2D& StartPos, const FVector2D& EndPos)
{
    TArray<AActor*> Result;
    UWorld* World = GetWorld();

    if (!World)
    {
        return Result;
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
    if (!PC)
    {
        return Result;
    }

    FVector2D MinPos(FMath::Min(StartPos.X, EndPos.X), FMath::Min(StartPos.Y, EndPos.Y));
    FVector2D MaxPos(FMath::Max(StartPos.X, EndPos.X), FMath::Max(StartPos.Y, EndPos.Y));

    for (TActorIterator<AActor> It(World); It; ++It)
    {
        AActor* Actor = *It;
        if (!Actor || !Actor->ActorHasTag(FName("SelectableUnit")))
        {
            continue;
        }

        FVector Location = Actor->GetActorLocation();
        FVector2D ScreenPos;
        if (PC->ProjectWorldLocationToScreen(Location, ScreenPos))
        {
            if (ScreenPos.X >= MinPos.X && ScreenPos.X <= MaxPos.X &&
                ScreenPos.Y >= MinPos.Y && ScreenPos.Y <= MaxPos.Y)
            {
                Result.Add(Actor);
            }
        }
    }

    return Result;
}
