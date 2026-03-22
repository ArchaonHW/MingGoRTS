// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingStrategyAuthority.generated.h"

/**
 * 六策類型枚舉
 * 正六策：立國、立制、立人
 * 逆六策：破局、破結構、不破人
 */
UENUM(BlueprintType)
enum class ESixStrategyType : uint8
{
    None            UMETA(DisplayName = "None"),
    // 正六策
    EstablishNation UMETA(DisplayName = "立國"),      // 伐心之正道
    EstablishOrder  UMETA(DisplayName = "立制"),     // 伐勢、伐命之正道
    EstablishPeople UMETA(DisplayName = "立人"),     // 伐氣、伐影、伐無之正道
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
enum class EStrategyDirection : uint8
{
    None        UMETA(DisplayName = "None"),
    Righteous   UMETA(DisplayName = "正道"),    // 正策
    Evil        UMETA(DisplayName = "逆術"),    // 逆策
    Count
};

/**
 * 策略數據結構
 */
USTRUCT(BlueprintType)
struct MINGGORTS_API FStrategyData
{
    GENERATED_BODY()

    // 策略類型
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

    // 效果強度 (1.0 = 100%)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy")
    float EffectIntensity = 1.0f;

    // 墮落值影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy")
    int32 FallValueImpact = 0;

    // 最後使用時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy")
    FDateTime LastUsedTime;

    // 是否正在冷卻
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy")
    bool bIsOnCooldown = false;

    // 是否啟用
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
struct MINGGORTS_API FStrategyEffect
{
    GENERATED_BODY()

    // 士氣影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StrategyEffect")
    float MoraleImpact = 0.0f;

    // 經濟效率影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StrategyEffect")
    float EconomicEfficiencyImpact = 0.0f;

    // 徵兵速度影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StrategyEffect")
    float RecruitmentSpeedImpact = 0.0f;

    // 將領成長影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StrategyEffect")
    float LeaderGrowthImpact = 0.0f;

    // 敵軍內亂影響 (逆策)
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
 * 掌正逆、掌陰陽、掌五行節奏
 * 決定何時使用正道，何時使用逆術
 */
UCLASS(ClassGroup = (SageCommand), meta = (BlueprintSpawnableComponent))
class MINGGORTS_API UMingStrategyAuthority : public UObject
{
    GENERATED_BODY()

public:
    UMingStrategyAuthority();

    // 初始化系統
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Strategy")
    void InitializeStrategyAuthority();

    // 使用策略
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Strategy")
    bool UseStrategy(ESixStrategyType StrategyType, float Multiplier = 1.0f);

    // 切換正逆策略
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Strategy")
    bool SwitchStrategyDirection(EStrategyDirection NewDirection);

    // 強制切換到正策 (由道權調用)
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Strategy")
    bool ForceSwitchToRighteous();

    // 獲取策略數據
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Strategy")
    FStrategyData GetStrategyData(ESixStrategyType StrategyType) const;

    // 獲取策略效果
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Strategy")
    FStrategyEffect GetStrategyEffect(ESixStrategyType StrategyType) const;

    // 檢查策略是否可用
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Strategy")
    bool IsStrategyAvailable(ESixStrategyType StrategyType) const;

    // 檢查是否正在使用逆策
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Strategy")
    bool IsUsingEvilStrategy() const;

    // 獲取當前策略方向
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Strategy")
    EStrategyDirection GetCurrentDirection() const { return CurrentDirection; }

    // 獲取推薦策略
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Strategy")
    TArray<ESixStrategyType> GetRecommendedStrategies() const;

    // 獲取策略描述
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Strategy")
    FString GetStrategyDescription(ESixStrategyType StrategyType) const;

    // 獲取策略方向描述
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Strategy")
    FString GetDirectionDescription(EStrategyDirection Direction) const;

    // 檢查系統健康狀況
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Strategy")
    bool IsHealthy() const;

    // 更新冷卻狀態
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Strategy")
    void UpdateCooldowns();

    // 事件：策略使用
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStrategyUsed, ESixStrategyType, StrategyType, const FStrategyEffect&, Effect);
    UPROPERTY(BlueprintAssignable, Category = "SageCommand|Strategy")
    FOnStrategyUsed OnStrategyUsed;

    // 事件：策略方向切換
    DECLARE_DYNAMIC_MULTICAST_Delegate_OneParam(FOnDirectionSwitched, EStrategyDirection, NewDirection);
    UPROPERTY(BlueprintAssignable, Category = "SageCommand|Strategy")
    FOnDirectionSwitched OnDirectionSwitched;

    // 事件：策略冷卻完成
    DECLARE_DYNAMIC_MULTICAST_Delegate_OneParam(FOnStrategyCooldownComplete, ESixStrategyType, StrategyType);
    UPROPERTY(BlueprintAssignable, Category = "SageCommand|Strategy")
    FOnStrategyCooldownComplete OnStrategyCooldownComplete;

protected:
    // 初始化默認策略
    void InitializeDefaultStrategies();

    // 應用策略效果
    void ApplyStrategyEffect(ESixStrategyType StrategyType, float Multiplier);

    // 計算冷卻時間
    void CalculateCooldown(ESixStrategyType StrategyType);

    // 檢查切換條件
    bool CanSwitchDirection(EStrategyDirection NewDirection) const;

    // 獲取策略五行對應
    FString GetStrategyWuXing(ESixStrategyType StrategyType) const;

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

    // 是否已初始化
    UPROPERTY()
    bool bIsInitialized = false;

    // 連續使用逆策次數
    UPROPERTY()
    int32 ConsecutiveEvilUses = 0;

    // 連續使用正策次數
    UPROPERTY()
    int32 ConsecutiveRighteousUses = 0;
};
