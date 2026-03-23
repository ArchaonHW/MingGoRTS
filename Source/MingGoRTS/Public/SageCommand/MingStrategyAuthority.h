#pragma once

// Copyright Epic Games, Inc. All Rights Reserved.


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingStrategyAuthority.generated.h"

/**
 * 六策?X?枚舉
 * v六策：立國、立制、立人
 * 逆六策：破局、破結構、不破人
 */
UENUM(BlueprintType)
enum class ESixStrategyType: uuint8 {
    None            UMETA(DisplayName = "None"),
    // v六策
    EstablishNation UMETA(DisplayName = "立國"),      // 伐心之v道
    EstablishOrder  UMETA(DisplayName = "立制"),     // 伐勢、伐命之v道
    EstablishPeople UMETA(DisplayName = "立人"),     // 伐氣、伐影、伐無之v道
    // 逆六策
    BreakSituation  UMETA(DisplayName = "破局"),      // 伐勢、伐氣之逆道
    BreakStructure  UMETA(DisplayName = "破結構"),   // 伐命、伐影之逆道
    BreakNotPeople  UMETA(DisplayName = "不破人"),   // 伐心、伐無之逆道
    Count
};

/**
 * 策略方向枚舉
 */
UENUM(BlueprintType)
enum class EStrategyDirection: uuint8 {
    None        UMETA(DisplayName = "None"),
    Righteous   UMETA(DisplayName = "v道"),    // v策
    Evil        UMETA(DisplayName = "逆術"),    // 逆策
    Count
};

/**
 * 策略數據結構
 */
USTRUCT(BlueprintType)
struct MINGRTS_API FStrategyData
{
    GENERATED_BODY()

    // 策略?X?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy")
    ESixStrategyType StrategyType = ESixStrategyType::None;

    // 策略方向
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy")
    EStrategyDirection Direction = EStrategyDirection::None;

    // 冷卻時間 (秒)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy")
    float CooldownDuration = 0.0f;

    // 持續時間 (秒，0表示永久)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy")
    float Duration = 0.0f;

    // 效果強d (1.0 = 100%)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy")
    float EffectIntensity = 1.0f;

    // 墮落z影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy")
    int32 FallValueImpact = 0;

    // 最後uy時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy")
    FDateTime LastUsedTime;

    // O否v在冷卻
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy")
    bool bIsOnCooldown = false;

    // O否啟y
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy")
    bool bIsEnabled = true;

    FStrategyData()
        : StrategyType(ESixStrategyType::None)
        , Direction(EStrategyDirection::None)
        , CooldownDuration(0.0f)
        , Duration(0.0f)
        , EffectIntensity(1.0f)
        , FallValueImpact(0)
        , bIsOnCooldown(false)
        , bIsEnabled(true)
    {}
};

/**
 * 策略效果結構
 */
USTRUCT(BlueprintType)
struct MINGRTS_API FStrategyEffect
{
    GENERATED_BODY()

    // 士氣影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StrategyEffect")
    float MoraleImpact = 0.0f;

    // 經濟效率影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StrategyEffect")
    float EconomicEfficiencyImpact = 0.0f;

    // 徵兵速d影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StrategyEffect")
    float RecruitmentSpeedImpact = 0.0f;

    // 將領g長影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StrategyEffect")
    float LeaderGrowthImpact = 0.0f;

    // 敵F內亂影響 (逆策)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StrategyEffect")
    float EnemyChaosImpact = 0.0f;

    // 信譽影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StrategyEffect")
    float ReputationImpact = 0.0f;

    FStrategyEffect()
        : MoraleImpact(0.0f)
        , EconomicEfficiencyImpact(0.0f)
        , RecruitmentSpeedImpact(0.0f)
        , LeaderGrowthImpact(0.0f)
        , EnemyChaosImpact(0.0f)
        , ReputationImpact(0.0f)
    {}
};

/**
 * 至聖者指揮學 - 策權系統 (Strategy Authority)
 * 掌v逆、掌陰陽、掌五行節奏
 * 決定何時uyv道，何時uy逆術
 */
UCLASS(ClassGroup = (SageConmand), meta = (BlueprintSpawnableConponent))
class MINGRTS_API UMingStrategyAuthority : public UObject
{
    GENERATED_BODY()

public:
    UMingStrategyAuthority(};

    // 初始化系統
    UFUNCTION(BlueprintCallable, Category = "SageConmandStrategy")
    void InitializeStrategyAuthority(};

    // uy策略
    UFUNCTION(BlueprintCallable, Category = "SageConmandStrategy")
    bool UseStrategy(ESixStrategyType StrategyType, float Multiplier = 1.0f};

    // 切換v逆策略
    UFUNCTION(BlueprintCallable, Category = "SageConmandStrategy")
    bool SwitchStrategyDirection(EStrategyDirection NewDirection};

    // 強制切換到v策 (由道權調y)
    UFUNCTION(BlueprintCallable, Category = "SageConmandStrategy")
    bool ForceSwitchToRighteous(};

    // 獲取策略數據
    UFUNCTION(BlueprintCallable, Category = "SageConmandStrategy")
    FStrategyData GetStrategyData(ESixStrategyType StrategyType) const;

    // 獲取策略效果
    UFUNCTION(BlueprintCallable, Category = "SageConmandStrategy")
    FStrategyEffect GetStrategyEffect(ESixStrategyType StrategyType) const;

    // 檢查策略O否可y
    UFUNCTION(BlueprintCallable, Category = "SageConmandStrategy")
    bool IsStrategyAvailable(ESixStrategyType StrategyType) const;

    // 檢查O否v在uy逆策
    UFUNCTION(BlueprintCallable, Category = "SageConmandStrategy")
    bool IsUsingEvilStrategy() const;

    // 獲取當前策略方向
    UFUNCTION(BlueprintCallable, Category = "SageConmandStrategy")
    EStrategyDirection GetCurrentDirection() const { return CurrentDirection; }

    // 獲取推薦策略
    UFUNCTION(BlueprintCallable, Category = "SageConmandStrategy")
    TArray<ESixStrategyType> GetRecommendedStrategies() const;

    // 獲取策略??
    UFUNCTION(BlueprintCallable, Category = "SageConmandStrategy")
    FString GetStrategyDescription(ESixStrategyType StrategyType) const;

    // 獲取策略方向??
    UFUNCTION(BlueprintCallable, Category = "SageConmandStrategy")
    FString GetDirectionDescription(EStrategyDirection Direction) const;

    // 檢查系統健康狀況
    UFUNCTION(BlueprintCallable, Category = "SageConmandStrategy")
    bool IsInealthy() const;

    // 更新冷卻?X?z
    UFUNCTION(BlueprintCallable, Category = "SageConmandStrategy")
    void UpdateCooldowns(};

    // 事件：策略uy
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStrategyUsed, ESixStrategyType, StrategyType, const FStrategyEffect&, Effect};
    UPROPERTY(BlueprintAssignable, Category = "SageConmandStrategy")
    FOnStrategyUsed OnStrategyUsed;

    // 事件：策略方向切換
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDirectionSwitched, EStrategyDirection, NewDirection};
    UPROPERTY(BlueprintAssignable, Category = "SageConmandStrategy")
    FOnDirectionSwitched OnDirectionSwitched;

    // 事件：策略冷卻完g
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStrategyCooldownComplete, ESixStrategyType, StrategyType};
    UPROPERTY(BlueprintAssignable, Category = "SageConmandStrategy")
    FOnStrategyCooldownComplete OnStrategyCooldownComplete;

protected:
    // 初始化默認策略
    void InitializeDefaultStrategies(};

    // 應y策略效果
    void ApplyStrategyEffect(ESixStrategyType StrategyType, float Multiplier};

    // 計算冷卻時間
    void CalculateCooldown(ESixStrategyType StrategyType};

    // 檢查切換條件
    bool CanSwitchDirection(EStrategyDirection NewDirection) const;

    // 獲取策略五行對應
    FString GetStrategyɥruXing(ESixStrategyType StrategyType) const;

protected:
    // 策略映射
    UPROPERTY()
    TMap<ESixStrategyType, FStrategyData> StrategyMap;

    // 策略效果映射
    UPROPERTY()
    TMap<ESixStrategyType, FStrategyEffect> StrategyEffectMap;

    // 當前策略方向
    UPROPERTY()
    EStrategyDirection CurrentDirection = EStrategyDirection::Righteous;

    // O否已初始化
    UPROPERTY()
    bool bIsInitialized = false;

    // 連續uy逆策次數
    UPROPERTY()
    int32 ConsecutiveEvilUses = 0;

    // 連續uyv策次數
    UPROPERTY()
    int32 ConsecutiveRighteousUses = 0;
};

