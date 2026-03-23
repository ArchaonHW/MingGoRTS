#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "MingDemonTraits.generated.h"

// 魔王特性類型
UENUM(BlueprintType)
enum class EDemonTraitType : uint8
{
    Destruction,    // 破壞力
    Domination,      // 支配力
    Corruption,      // 墮落散播
    Fear,            // 恐懼
    Rage,            // 憤怒
    Darkness         // 黑暗力量
};

// 魔王特性數據結構
USTRUCT(BlueprintType)
struct MINGGORTS_API FDemonTraitData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Trait")
    EDemonTraitType TraitType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Trait")
    float TraitValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Trait")
    FString TraitDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Trait")
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Trait")
    float Duration;
};

// 魔王專屬特性
USTRUCT(BlueprintType)
struct MINGGORTS_API FDemonSpecialTraits
{
    GENERATED_BODY()

    /** 攻擊力加成 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Special Traits")
    float AttackPowerBonus = 1.5f;

    /** 防禦力減免 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Special Traits")
    float DefenseReduction = 0.8f;

    /** 墮落速度加成 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Special Traits")
    float CorruptionRateMultiplier = 1.5f;

    /** 恐懼光環 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Special Traits")
    float FearAura = 150.0f;

    /** 支配力影響 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Special Traits")
    float DominationPower = 1.3f;

    /** 黑暗力量強度 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Special Traits")
    float DarknessPower = 1.4f;

    /** 破壞慾望 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Special Traits")
    float DestructionUrge = 1.6f;

    /** 憤怒值 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Special Traits")
    float RageValue = 100.0f;

    /** 無法停止逆策略 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Special Traits")
    bool bCannotStopEvilStrategies = true;

    /** 逆策略加成 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Special Traits")
    float EvilStrategyBonus = 1.5f;
};
