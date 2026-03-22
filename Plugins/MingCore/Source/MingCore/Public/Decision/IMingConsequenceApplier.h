#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MingDecisionConsequenceManager.h"
#include "IMingConsequenceApplier.generated.h"

// ??- ?X?XUINTERFACE(MinimalAPI, BlueprintType)
class UMingConsequenceApplier : public UInterface
{
    GENERATED_BODY()
};

/**
 * ?X摧毀X * tdN??X故事選項?@??
 */
class MINGCORE_API IMingConsequenceApplier
{
    GENERATED_BODY()

public:
    // ??Ϋ??
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    bool Apply(const FMingConsequenceResult& ConsequenceResult);

    
    bool CanApply(const FMingConsequenceResult& ConsequenceResult) const;

    // ??P??
    UFUNCTION(BlueprintNativeEvent, Category = "Ming|Decision")
    bool Undo(const FString& ConsequenceID);

    
    FString GetApplierName() const;

    
    FString GetApplierDescription() const;

    
    int32 GetPriority() const;
};

