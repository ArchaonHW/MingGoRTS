#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "MingSageTraits.generated.h"

// 聖者特性類型
UENUM(BlueprintType)
enum class ESageTraitType : uint8
{
    Leadership,      // 領導力
    Wisdom,          // 智慧
    Charisma,        // 魅力
    Strategy,        // 策略
    Corruption,      // 墮落抗性
    Balance          // 平衡
};

// 聖者特性數據結構
USTRUCT(BlueprintType)
struct MINGGORTS_API FSageTraitData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Trait")
    ESageTraitType TraitType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Trait")
    float TraitValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Trait")
    FString TraitDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Trait")
    bool bIsPassive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Trait")
    float CooldownTime;
};

// 聖者專屬特性
USTRUCT(BlueprintType)
struct MINGGORTS_API FSageSpecialTraits
{
    GENERATED_BODY()

    /** 正逆策略無懲罰切換 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Special Traits")
    bool NoStrategyPenalty = true;

    /** 墮落閾值提升 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Special Traits")
    float CorruptionThresholdBonus = 50.0f;

    /** 平衡型數值修正 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Special Traits")
    float BalancedStatModifier = 1.1f;

    /** 覺醒狀態加成 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Special Traits")
    float EnlightenmentBonus = 1.2f;

    /** 淨化能力 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Special Traits")
    float PurificationPower = 1.5f;

    /** 領導力光環 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Special Traits")
    float LeadershipAura = 100.0f;

    /** 智慧洞察 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Special Traits")
    float WisdomInsight = 1.3f;

    /** 魅力影響範圍 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Special Traits")
    float CharismaRange = 200.0f;
};
