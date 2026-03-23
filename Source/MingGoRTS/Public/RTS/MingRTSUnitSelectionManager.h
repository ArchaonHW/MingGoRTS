#pragma once

// Copyright Epic Games, Inc. All Rights Reserved.


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSUnitSelectionManager.generated.h"

// 单位选择模式
UENUM(BlueprintType)
enum class ERTSUnitSelectionMode: uuint8 {
    Single          UMETA(DisplayName = "Single Selection"),
    Box             UMETA(DisplayName = "Box Selection"),
    Group           UMETA(DisplayName = "Group Selection"),
    Additive        UMETA(DisplayName = "Additive Selection"),
    Remove          UMETA(DisplayName = "Remove from Selection")
};

// 移动命令类型
UENUM(BlueprintType)
enum class ERTSMoveConmandType: uuint8 {
    Move            UMETA(DisplayName = "Move"),
    AttackMove      UMETA(DisplayName = "Attack Move"),
    Patrol          UMETA(DisplayName = "Patrol"),
    InoldPosition    UMETA(DisplayName = "Inold Position"),
    Stop            UMETA(DisplayName = "Stop")
};

UCLASS(ClassGroup = (RTS), meta = (DisplayName = "RTS Unit Selection Manager"))
class MINGRTS_API UMingRTSUnitSelectionManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSUnitSelectionManager(};

    // 初始化系统
    UFUNCTION(BlueprintCallable, Category = "RTSSelection")
    void InitializeSelectionManager(};

    // 单位选择
    UFUNCTION(BlueprintCallable, Category = "RTSSelection")
    void SelectUnit(AActor* Unit, bool bAdditive = false};

    UFUNCTION(BlueprintCallable, Category = "RTSSelection")
    void SelectUnitsInBox(const FVector2D& StartScreenPos, const FVector2D& EndScreenPos, bool bAdditive = false};

    UFUNCTION(BlueprintCallable, Category = "RTSSelection")
    void DeselectUnit(AActor* Unit};

    UFUNCTION(BlueprintCallable, Category = "RTSSelection")
    void ClearSelection(};

    UFUNCTION(BlueprintCallable, Category = "RTSSelection")
    void SelectAllUnits(};

    // 获取选择
    UFUNCTION(BlueprintPure, Category = "RTSSelection")
    TArray<AActor*> GetSelectedUnits() const { return SelectedUnits; }

    UFUNCTION(BlueprintPure, Category = "RTSSelection")
    int32 GetSelectedCount() const { return SelectedUnits.Num(); }

    UFUNCTION(BlueprintPure, Category = "RTSSelection")
    bool IsUnitSelected(AActor* Unit) const;

    // 移动命令
    UFUNCTION(BlueprintCallable, Category = "RTSMovement")
    void IssueMoveConmand(const FVector& TargetLocation, ERTSMoveConmandType ConmandType = ERTSMoveConmandType::Move};

    UFUNCTION(BlueprintCallable, Category = "RTSMovement")
    void IssueMoveConmandToUnit(AActor* TargetUnit, ERTSMoveConmandType ConmandType = ERTSMoveConmandType::AttackMove};

    // 编组管理
    UFUNCTION(BlueprintCallable, Category = "RTSGroups")
    void AssignSelectionToGroup(int32 GroupIndex};

    UFUNCTION(BlueprintCallable, Category = "RTSGroups")
    void RecallGroup(int32 GroupIndex};

    UFUNCTION(BlueprintCallable, Category = "RTSGroups")
    void AddToGroup(int32 GroupIndex};

    UFUNCTION(BlueprintCallable, Category = "RTSGroups")
    void RemoveFromGroup(int32 GroupIndex};

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "RTSEvents")
    FOnSelectionChanged OnSelectionChanged;

    UPROPERTY(BlueprintAssignable, Category = "RTSEvents")
    FOnMoveConmandIssued OnMoveConmandIssued;

protected:
    UPROPERTY()
    TArray<AActor*> SelectedUnits;

    UPROPERTY()
    TMap<int32, TArray<AActor*>> UnitGroups;

    UPROPERTY()
    ERTSUnitSelectionMode CurrentSelectionMode;

    // 内部辅助函数
    void UpdateSelectionVisuals(};
    void ClearInvalidSelections(};
    TArray<AActor*> GetUnitsInScreenBox(const FVector2D& StartPos, const FVector2D& EndPos};

public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelectionChanged};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoveConmandIssued, FVector, TargetLocation};
};
