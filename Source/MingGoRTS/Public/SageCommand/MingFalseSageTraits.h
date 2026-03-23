#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "MingFalseSageTraits.generated.h"

// 偽聖者特性類型
UENUM(BlueprintType)
enum class EFalseSageTraitType : uint8
{
    Deception,      // 欺騙
    Betrayal,       // 背叛
    Infiltration,   // 滲透
    Manipulation,   // 操控
    Stealth,        // 隱匿
    Sabotage        // 破壞
};

// 偽聖者特性數據結構
USTRUCT(BlueprintType)
struct MINGGORTS_API FFalseSageTraitData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "False Sage Trait")
    EFalseSageTraitType TraitType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "False Sage Trait")
    float TraitValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "False Sage Trait")
    FString TraitDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "False Sage Trait")
    bool bIsHidden;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "False Sage Trait")
    float DetectionChance;
};

// 偽聖者專屬特性
USTRUCT(BlueprintType)
struct MINGGORTS_API FFalseSageSpecialTraits
{
    GENERATED_BODY()

    /** 欺騙成功率 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "False Sage Special Traits")
    float DeceptionSuccessRate = 0.7f;

    /** 背叛機率 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "False Sage Special Traits")
    float BetrayalChance = 0.3f;

    /** 隱藏真實身份 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "False Sage Special Traits")
    bool bHideTrueIdentity = true;

    /** 滲透能力 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "False Sage Special Traits")
    float InfiltrationAbility = 1.4f;

    /** 操控影響力 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "False Sage Special Traits")
    float ManipulationPower = 1.3f;

    /** 隱匿效果 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "False Sage Special Traits")
    float StealthEffectiveness = 1.5f;

    /** 破壞能力 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "False Sage Special Traits")
    float SabotagePower = 1.2f;

    /** 偽裝持續時間 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "False Sage Special Traits")
    float DisguiseDuration = 300.0f;

    /** 背叛傷害加成 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "False Sage Special Traits")
    float BetrayalDamageBonus = 2.0f;

    /** 身份揭露懲罰 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "False Sage Special Traits")
    float IdentityRevealPenalty = 1.5f;

    /** 混亂傳播 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "False Sage Special Traits")
    float ChaosSpread = 1.6f;
};
