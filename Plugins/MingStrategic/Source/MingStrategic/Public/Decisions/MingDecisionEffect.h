#pragma once

#include "CoreMinimal.h"
#include "MingDecisionEffect.generated.h"

/**
 * 決策效果類型
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
 * 決策效果結構
 * 定義選擇某個決策選項後的效果
 */
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingDecisionEffect
{
    GENERATED_BODY()
    
    // 效果ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FString EffectId;
    
    // 效果類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    EDecisionEffectType EffectType;
    
    // 目標對象 (資源類型、角色ID等)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FString TargetKey;
    
    // 效果數值 (正值為增加，負值為減少)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    float EffectValue;
    
    // 持續時間 (0為永久)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    float Duration;
    
    // 效果描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    FText EffectDescription;
    
    // 是否立即執行
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
    bool bImmediate;
    
    FMingDecisionEffect()
        : EffectType(EDecisionEffectType::Custom)
        , EffectValue(0.0f)
        , Duration(0.0f)
        , bImmediate(true)
    {}
};
