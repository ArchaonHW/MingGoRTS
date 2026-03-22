#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MingDecisionConsequenceManager.h"
#include "IMingConsequenceApplier.generated.h"

// 介面類 - 後果應用器
UINTERFACE(MinimalAPI, BlueprintType)
class UIMingConsequenceApplier : public UInterface
{
    GENERATED_BODY()
};

/**
 * 後果應用器介面
 * 負責將計算出的後果應用到遊戲世界
 */
class MINGCORE_API IMingConsequenceApplier
{
    GENERATED_BODY()

public:
    // 應用後果
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    bool Apply(const FMingConsequenceResult& ConsequenceResult);

    // 檢查是否可以應用該後果
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    bool CanApply(const FMingConsequenceResult& ConsequenceResult) const;

    // 撤銷後果
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    bool Undo(const FString& ConsequenceID);

    // 獲取應用器名稱
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    FString GetApplierName() const;

    // 獲取應用器描述
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    FString GetApplierDescription() const;

    // 獲取應用優先級
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    int32 GetPriority() const;
};
