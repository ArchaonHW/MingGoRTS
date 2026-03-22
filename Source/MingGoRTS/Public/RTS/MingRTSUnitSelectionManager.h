// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSUnitSelectionManager.generated.h"

// 单位选择模式
UENUM(BlueprintType)
enum class ERTSUnitSelectionMode: uint8 {
    Single          UMETA(DisplayName = "Single Selection"),
    Box             UMETA(DisplayName = "Box Selection"),
    Group           UMETA(DisplayName = "Group Selection"),
    Additive        UMETA(DisplayName = "Additive Selection"),
    Remove          UMETA(DisplayName = "Remove from Selection")
};

// 移动命令类型
UENUM(BlueprintType)
enum class ERTSMoveCommandType: uint8 {
    Move            UMETA(DisplayName = "Move"),
    AttackMove      UMETA(DisplayName = "Attack Move"),
    Patrol          UMETA(DisplayName = "Patrol"),
    HoldPosition    UMETA(DisplayName = "Hold Position"),
    Stop            UMETA(DisplayName = "Stop")
};

UCLASS(ClassGroup = (RTS), meta = (DisplayName = "RTS Unit Selection Manager"))
class MINGGORTS_API UMingRTSUnitSelectionManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSUnitSelectionManager();

    // 初始化系统
    UFUNCTION(BlueprintCallable, Category = "RTS|Selection")
    void InitializeSelectionManager();

    // 单位选择
    UFUNCTION(BlueprintCallable, Category = "RTS|Selection")
    void SelectUnit(AActor* Unit, bool bAdditive = false);

    UFUNCTION(BlueprintCallable, Category = "RTS|Selection")
    void SelectUnitsInBox(const FVector2D& StartScreenPos, const FVector2D& EndScreenPos, bool bAdditive = false);

    UFUNCTION(BlueprintCallable, Category = "RTS|Selection")
    void DeselectUnit(AActor* Unit);

    UFUNCTION(BlueprintCallable, Category = "RTS|Selection")
    void ClearSelection();

    UFUNCTION(BlueprintCallable, Category = "RTS|Selection")
    void SelectAllUnits();

    // 获取选择
    UFUNCTION(BlueprintPure, Category = "RTS|Selection")
    TArray<AActor*> GetSelectedUnits() const { return SelectedUnits; }

    UFUNCTION(BlueprintPure, Category = "RTS|Selection")
    int32 GetSelectedCount() const { return SelectedUnits.Num(); }

    UFUNCTION(BlueprintPure, Category = "RTS|Selection")
    bool IsUnitSelected(AActor* Unit) const;

    // 移动命令
    UFUNCTION(BlueprintCallable, Category = "RTS|Movement")
    void IssueMoveCommand(const FVector& TargetLocation, ERTSMoveCommandType CommandType = ERTSMoveCommandType::Move);

    UFUNCTION(BlueprintCallable, Category = "RTS|Movement")
    void IssueMoveCommandToUnit(AActor* TargetUnit, ERTSMoveCommandType CommandType = ERTSMoveCommandType::AttackMove);

    // 编组管理
    UFUNCTION(BlueprintCallable, Category = "RTS|Groups")
    void AssignSelectionToGroup(int32 GroupIndex);

    UFUNCTION(BlueprintCallable, Category = "RTS|Groups")
    void RecallGroup(int32 GroupIndex);

    UFUNCTION(BlueprintCallable, Category = "RTS|Groups")
    void AddToGroup(int32 GroupIndex);

    UFUNCTION(BlueprintCallable, Category = "RTS|Groups")
    void RemoveFromGroup(int32 GroupIndex);

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "RTS|Events")
    FOnSelectionChanged OnSelectionChanged;

    UPROPERTY(BlueprintAssignable, Category = "RTS|Events")
    FOnMoveCommandIssued OnMoveCommandIssued;

protected:
    UPROPERTY()
    TArray<AActor*> SelectedUnits;

    UPROPERTY()
    TMap<int32, TArray<AActor*>> UnitGroups;

    UPROPERTY()
    ERTSUnitSelectionMode CurrentSelectionMode;

    // 内部辅助函数
    void UpdateSelectionVisuals();
    void ClearInvalidSelections();
    TArray<AActor*> GetUnitsInScreenBox(const FVector2D& StartPos, const FVector2D& EndPos);

public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelectionChanged);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoveCommandIssued, FVector, TargetLocation);
};
