#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MingDecisionConsequenceManager.h"
#include "IMingConsequenceApplier.generated.h"

// 介面??- 後?X?用XUINTERFACE(MinimalAPI, BlueprintType)
class UMingConsequenceApplier : public UInterface
{
    GENERATED_BODY()
};

/**
 * 後?X?用???X * 負責將??算出???X??X?到??戲世??
 */
class MINGCORE_API IMingConsequenceApplier
{
    GENERATED_BODY()

public:
    // ??用後??
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    bool Apply(const FMingConsequenceResult& ConsequenceResult};

    
    bool CanApply(const FMingConsequenceResult& ConsequenceResult) const;

    // ??銷後??
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    bool Undo(const FString& ConsequenceID};

    
    FString GetApplierName() const;

    
    FString GetApplierDescription() const;

    
    int32 GetPriority() const;
};

