#pragma once

#include "CoreMinimal.h"
#include "MingDecisionEffect.generated.h"

/**
 * 決�X��?類�?
 */
UENUM(BlueprintType)
enum class EDecisionEffectType: uint8 {
    ResourceChange      UMETA(DisplayName = "Resource Change"),
    FactionRelation     UMETA(DisplayName = "Faction Relation"),
    CharacterAttribute  UMETA(DisplayName = "Character Attribute"),
    UnlockContent       UMETA(DisplayName = "Unlock Content"),
    TriggerEvent        UMETA(DisplayName = "Trigger Event"),
    Custom              UMETA(DisplayName = "Custom")
};

/**
 * 決�X��?結�?
 * 定義?��X�個決策選?��X��X */
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingDecisionEffect
{
    GENERATED_BODY()
    
    // ?��?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FString EffectId;
    
    // ?��?類�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    EDecisionEffectType EffectType;
    
    // ?��?對象 (資�?類�X��X�ID�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FString TargetKey;
    
    // ?��X��?(�?��為增�?，�X�為減�?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    float EffectValue;
    
    // ?��X��? (0?�永�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    float Duration;
    
    // ?��X�述
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FText EffectDescription;
    
    // ?�否立即?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    bool bImmediate;
    
    FMingDecisionEffect()
        : EffectType(EDecisionEffectType::Custom)
        , EffectValue(0.0f)
        , Duration(0.0f)
        , bImmediate(true)
    {}
};

