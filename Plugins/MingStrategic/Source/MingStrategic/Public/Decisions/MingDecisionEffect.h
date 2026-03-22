#pragma once

#include "CoreMinimal.h"
#include "MingDecisionEffect.generated.h"

/**
 * æ±ºç??ˆæ?é¡å?
 */
UENUM(BlueprintType)
enum class EDecisionEffectType : uint8
{
    ResourceChange      UMETA(DisplayName = "Resource Change"),
    FactionRelation     UMETA(DisplayName = "Faction Relation"),
    CharacterAttribute  UMETA(DisplayName = "Character Attribute"),
    UnlockContent       UMETA(DisplayName = "Unlock Content"),
    TriggerEvent        UMETA(DisplayName = "Trigger Event"),
    Custom              UMETA(DisplayName = "Custom")
};

/**
 * æ±ºç??ˆæ?çµæ?
 * å®šç¾©?¸æ??å€‹æ±ºç­–é¸?…å??„æ??? */
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingDecisionEffect
{
    GENERATED_BODY()
    
    // ?ˆæ?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FString EffectId;
    
    // ?ˆæ?é¡å?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    EDecisionEffectType EffectType;
    
    // ?®æ?å°è±¡ (è³‡æ?é¡å??è??²IDç­?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FString TargetKey;
    
    // ?ˆæ??¸å€?(æ­?€¼ç‚ºå¢å?ï¼Œè??¼ç‚ºæ¸›å?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    float EffectValue;
    
    // ?ç??‚é? (0?ºæ°¸ä¹?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    float Duration;
    
    // ?ˆæ??è¿°
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FText EffectDescription;
    
    // ?¯å¦ç«‹å³?·è?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    bool bImmediate;
    
    FMingDecisionEffect()
        : EffectType(EDecisionEffectType::Custom)
        , EffectValue(0.0f)
        , Duration(0.0f)
        , bImmediate(true)
    {}
};
