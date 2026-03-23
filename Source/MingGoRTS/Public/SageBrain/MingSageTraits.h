#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSageTraits.generated.h"

// 聖者能力類型
UENUM(BlueprintType)
enum class ESageAbilityType : uint8
{
    None                UMETA(DisplayName = "無"),
    MoralGuidance       UMETA(DisplayName = "道德引導"),
    WisdomBoost         UMETA(DisplayName = "智慧提升"),
    UnityCommand        UMETA(DisplayName = "團結指揮"),
    RighteousJudgment   UMETA(DisplayName = "正義審判"),
    DivineBlessing      UMETA(DisplayName = "神聖祝福"),
    Enlightenment       UMETA(DisplayName = "啟迪之光")
};

// 聖者狀態結構
USTRUCT(BlueprintType)
struct FSageStatus
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoralPurity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WisdomLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DivineConnection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RighteousPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EnlightenmentProgress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FollowerCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<ESageAbilityType> UnlockedAbilities;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> SageModifiers;

    FSageStatus()
    {
        MoralPurity = 80.0f;
        WisdomLevel = 70.0f;
        DivineConnection = 60.0f;
        RighteousPower = 75.0f;
        EnlightenmentProgress = 0.0f;
        FollowerCount = 0;
    }
};

// 聖者能力結構
USTRUCT(BlueprintType)
struct FSageAbility
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AbilityName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESageAbilityType AbilityType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PowerCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EffectStrength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CooldownTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RequiredLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> RequiredConditions;

    FSageAbility()
    {
        AbilityName = TEXT("");
        AbilityType = ESageAbilityType::None;
        PowerCost = 0.0f;
        EffectStrength = 0.0f;
        CooldownTime = 0.0f;
        RequiredLevel = 1;
    }
};

// 聖者事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSageAbilityActivated, ESageAbilityType, AbilityType, float, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoralPurityChanged, float, NewPurity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWisdomGained, float, WisdomAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFollowerInspired, int32, FollowerCount, const FString&, Reason);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingSageTraits : public UObject
{
    GENERATED_BODY()

public:
    UMingSageTraits();

    // 初始化聖者特性
    UFUNCTION(BlueprintCallable, Category = "Sage Traits")
    void InitializeSageTraits();

    // 獲取聖者狀態
    UFUNCTION(BlueprintPure, Category = "Sage Traits")
    FSageStatus GetSageStatus() const { return CurrentSageStatus; }

    // 激活聖者能力
    UFUNCTION(BlueprintCallable, Category = "Sage Traits")
    bool ActivateSageAbility(ESageAbilityType AbilityType, const FString& Target);

    // 獲取可用能力
    UFUNCTION(BlueprintPure, Category = "Sage Traits")
    TArray<FSageAbility> GetAvailableAbilities() const;

    // 道德引導
    UFUNCTION(BlueprintCallable, Category = "Sage Traits")
    bool ApplyMoralGuidance(const FString& Target, float GuidanceStrength);

    // 智慧提升
    UFUNCTION(BlueprintCallable, Category = "Sage Traits")
    bool ApplyWisdomBoost(float BoostAmount);

    // 團結指揮
    UFUNCTION(BlueprintCallable, Category = "Sage Traits")
    bool ApplyUnityCommand(float CommandStrength);

    // 正義審判
    UFUNCTION(BlueprintCallable, Category = "Sage Traits")
    bool ExecuteRighteousJudgment(const FString& Target, float JudgmentPower);

    // 神聖祝福
    UFUNCTION(BlueprintCallable, Category = "Sage Traits")
    bool BestowDivineBlessing(const FString& Target, float BlessingPower);

    // 啟迪之光
    UFUNCTION(BlueprintCallable, Category = "Sage Traits")
    bool ApplyEnlightenment(float EnlightenmentAmount);

    // 感召追隨者
    UFUNCTION(BlueprintCallable, Category = "Sage Traits")
    bool InspireFollowers(const FString& Message, float InspirationPower);

    // 提升道德純度
    UFUNCTION(BlueprintCallable, Category = "Sage Traits")
    void IncreaseMoralPurity(float Amount);

    // 獲取道德純度
    UFUNCTION(BlueprintPure, Category = "Sage Traits")
    float GetMoralPurity() const { return CurrentSageStatus.MoralPurity; }

    // 計算聖者效力
    UFUNCTION(BlueprintPure, Category = "Sage Traits")
    float CalculateSageEffectiveness() const;

    // 檢查能力可用性
    UFUNCTION(BlueprintPure, Category = "Sage Traits")
    bool IsAbilityAvailable(ESageAbilityType AbilityType) const;

    // 應用聖者修飾符
    UFUNCTION(BlueprintCallable, Category = "Sage Traits")
    void ApplySageModifier(const FString& ModifierName, float ModifierValue);

    // 移除聖者修飾符
    UFUNCTION(BlueprintCallable, Category = "Sage Traits")
    void RemoveSageModifier(const FString& ModifierName);

    // 獲取聖者等級
    UFUNCTION(BlueprintPure, Category = "Sage Traits")
    int32 GetSageLevel() const;

    // 升級聖者
    UFUNCTION(BlueprintCallable, Category = "Sage Traits")
    void LevelUpSage();

    // 獲取聖者稱號
    UFUNCTION(BlueprintPure, Category = "Sage Traits")
    FString GetSageTitle() const;

    // 事件委託
    UPROPERTY(BlueprintAssignable, Category = "Sage Traits")
    FOnSageAbilityActivated OnSageAbilityActivated;

    UPROPERTY(BlueprintAssignable, Category = "Sage Traits")
    FOnMoralPurityChanged OnMoralPurityChanged;

    UPROPERTY(BlueprintAssignable, Category = "Sage Traits")
    FOnWisdomGained OnWisdomGained;

    UPROPERTY(BlueprintAssignable, Category = "Sage Traits")
    FOnFollowerInspired OnFollowerInspired;

protected:
    // 藍圖實現事件
    UFUNCTION(BlueprintImplementableEvent, Category = "Sage Traits")
    void OnSageTraitsInitialized();

    UFUNCTION(BlueprintImplementableEvent, Category = "Sage Traits")
    void OnAbilityUnlocked(ESageAbilityType AbilityType);

    UFUNCTION(BlueprintImplementableEvent, Category = "Sage Traits")
    void OnSageLevelUp(int32 NewLevel);

    UFUNCTION(BlueprintImplementableEvent, Category = "Sage Traits")
    void OnEnlightenmentReached(float Progress);

private:
    // 初始化聖者能力
    void InitializeSageAbilities();
    
    // 計算能力效果
    float CalculateAbilityEffect(ESageAbilityType AbilityType) const;
    
    // 檢查能力條件
    bool CheckAbilityConditions(const FSageAbility& Ability) const;
    
    // 解鎖能力
    void UnlockAbility(ESageAbilityType AbilityType);
    
    // 更新聖者狀態
    void UpdateSageStatus();
    
    // 計算經驗需求
    float CalculateXPRequirement() const;

protected:
    // 當前聖者狀態
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Traits")
    FSageStatus CurrentSageStatus;

    // 系統是否已初始化
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Traits")
    bool bIsInitialized;

    // 聖者等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Traits")
    int32 SageLevel;

    // 當前經驗
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Traits")
    float CurrentXP;

    // 最大道德純度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Traits")
    float MaxMoralPurity;

    // 最大智慧等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Traits")
    float MaxWisdomLevel;

    // 能力冷卻映射
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Traits")
    TMap<ESageAbilityType, float> AbilityCooldowns;

    // 能力映射
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Traits")
    TMap<ESageAbilityType, FSageAbility> SageAbilitiesMap;

    // 最大追隨者數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Traits")
    int32 MaxFollowers;

    // 道德恢復速率
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Traits")
    float MoralRestoreRate;

    // 智慧增長速率
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Traits")
    float WisdomGrowthRate;
};
