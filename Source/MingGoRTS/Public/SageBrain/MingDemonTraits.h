#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingDemonTraits.generated.h"

// 魔王能力類型
UENUM(BlueprintType)
enum class EDemonAbilityType : uint8
{
    None                UMETA(DisplayName = "無"),
    FearAura            UMETA(DisplayName = "恐懼光環"),
    DarkCommand         UMETA(DisplayName = "黑暗統治"),
    ChaosTactics        UMETA(DisplayName = "混沌戰術"),
    Intimidation        UMETA(DisplayName = "威懾"),
    Corruption          UMETA(DisplayName = "腐化"),
    Destruction         UMETA(DisplayName = "毀滅之力")
};

// 魔王狀態結構
USTRUCT(BlueprintType)
struct FDemonStatus
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FearPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DarkEnergy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ChaosLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DestructionPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MinionCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TerrorRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EDemonAbilityType> UnlockedAbilities;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> DemonModifiers;

    FDemonStatus()
    {
        FearPower = 70.0f;
        DarkEnergy = 80.0f;
        ChaosLevel = 60.0f;
        DestructionPower = 75.0f;
        MinionCount = 0;
        TerrorRadius = 100.0f;
    }
};

// 魔王能力結構
USTRUCT(BlueprintType)
struct FDemonAbility
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AbilityName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDemonAbilityType AbilityType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DarkEnergyCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EffectStrength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CooldownTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RequiredLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> RequiredConditions;

    FDemonAbility()
    {
        AbilityName = TEXT("");
        AbilityType = EDemonAbilityType::None;
        DarkEnergyCost = 0.0f;
        EffectStrength = 0.0f;
        CooldownTime = 0.0f;
        RequiredLevel = 1;
    }
};

// 魔王事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDemonAbilityActivated, EDemonAbilityType, AbilityType, float, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFearPowerChanged, float, NewPower);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDarkEnergyChanged, float, NewEnergy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMinionSummoned, int32, MinionCount, const FString&, Type);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingDemonTraits : public UObject
{
    GENERATED_BODY()

public:
    UMingDemonTraits();

    // 初始化魔王特性
    UFUNCTION(BlueprintCallable, Category = "Demon Traits")
    void InitializeDemonTraits();

    // 獲取魔王狀態
    UFUNCTION(BlueprintPure, Category = "Demon Traits")
    FDemonStatus GetDemonStatus() const { return CurrentDemonStatus; }

    // 激活魔王能力
    UFUNCTION(BlueprintCallable, Category = "Demon Traits")
    bool ActivateDemonAbility(EDemonAbilityType AbilityType, const FString& Target);

    // 獲取可用能力
    UFUNCTION(BlueprintPure, Category = "Demon Traits")
    TArray<FDemonAbility> GetAvailableAbilities() const;

    // 恐懼光環
    UFUNCTION(BlueprintCallable, Category = "Demon Traits")
    bool ApplyFearAura(float AuraStrength, float Radius);

    // 黑暗統治
    UFUNCTION(BlueprintCallable, Category = "Demon Traits")
    bool ApplyDarkCommand(float CommandStrength);

    // 混沌戰術
    UFUNCTION(BlueprintCallable, Category = "Demon Traits")
    bool ApplyChaosTactics(float ChaosStrength);

    // 威懾
    UFUNCTION(BlueprintCallable, Category = "Demon Traits")
    bool ApplyIntimidation(const FString& Target, float IntimidationPower);

    // 腐化
    UFUNCTION(BlueprintCallable, Category = "Demon Traits")
    bool ApplyCorruption(const FString& Target, float CorruptionPower);

    // 毀滅之力
    UFUNCTION(BlueprintCallable, Category = "Demon Traits")
    bool ApplyDestruction(const FString& Target, float DestructionPower);

    // 召喚僕從
    UFUNCTION(BlueprintCallable, Category = "Demon Traits")
    bool SummonMinions(int32 Count, const FString& MinionType);

    // 提升恐懼力量
    UFUNCTION(BlueprintCallable, Category = "Demon Traits")
    void IncreaseFearPower(float Amount);

    // 獲取恐懼力量
    UFUNCTION(BlueprintPure, Category = "Demon Traits")
    float GetFearPower() const { return CurrentDemonStatus.FearPower; }

    // 計算魔王效力
    UFUNCTION(BlueprintPure, Category = "Demon Traits")
    float CalculateDemonEffectiveness() const;

    // 檢查能力可用性
    UFUNCTION(BlueprintPure, Category = "Demon Traits")
    bool IsAbilityAvailable(EDemonAbilityType AbilityType) const;

    // 應用魔王修飾符
    UFUNCTION(BlueprintCallable, Category = "Demon Traits")
    void ApplyDemonModifier(const FString& ModifierName, float ModifierValue);

    // 移除魔王修飾符
    UFUNCTION(BlueprintCallable, Category = "Demon Traits")
    void RemoveDemonModifier(const FString& ModifierName);

    // 獲取魔王等級
    UFUNCTION(BlueprintPure, Category = "Demon Traits")
    int32 GetDemonLevel() const;

    // 升級魔王
    UFUNCTION(BlueprintCallable, Category = "Demon Traits")
    void LevelUpDemon();

    // 獲取魔王稱號
    UFUNCTION(BlueprintPure, Category = "Demon Traits")
    FString GetDemonTitle() const;

    // 事件委託
    UPROPERTY(BlueprintAssignable, Category = "Demon Traits")
    FOnDemonAbilityActivated OnDemonAbilityActivated;

    UPROPERTY(BlueprintAssignable, Category = "Demon Traits")
    FOnFearPowerChanged OnFearPowerChanged;

    UPROPERTY(BlueprintAssignable, Category = "Demon Traits")
    FOnDarkEnergyChanged OnDarkEnergyChanged;

    UPROPERTY(BlueprintAssignable, Category = "Demon Traits")
    FOnMinionSummoned OnMinionSummoned;

protected:
    // 藍圖實現事件
    UFUNCTION(BlueprintImplementableEvent, Category = "Demon Traits")
    void OnDemonTraitsInitialized();

    UFUNCTION(BlueprintImplementableEvent, Category = "Demon Traits")
    void OnAbilityUnlocked(EDemonAbilityType AbilityType);

    UFUNCTION(BlueprintImplementableEvent, Category = "Demon Traits")
    void OnDemonLevelUp(int32 NewLevel);

    UFUNCTION(BlueprintImplementableEvent, Category = "Demon Traits")
    void OnTerrorRadiusChanged(float NewRadius);

private:
    // 初始化魔王能力
    void InitializeDemonAbilities();
    
    // 計算能力效果
    float CalculateAbilityEffect(EDemonAbilityType AbilityType) const;
    
    // 檢查能力條件
    bool CheckAbilityConditions(const FDemonAbility& Ability) const;
    
    // 解鎖能力
    void UnlockAbility(EDemonAbilityType AbilityType);
    
    // 更新魔王狀態
    void UpdateDemonStatus();
    
    // 計算經驗需求
    float CalculateXPRequirement() const;

protected:
    // 當前魔王狀態
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Traits")
    FDemonStatus CurrentDemonStatus;

    // 系統是否已初始化
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Traits")
    bool bIsInitialized;

    // 魔王等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Traits")
    int32 DemonLevel;

    // 當前經驗
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Traits")
    float CurrentXP;

    // 最大恐懼力量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Traits")
    float MaxFearPower;

    // 最大黑暗能量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Traits")
    float MaxDarkEnergy;

    // 能力冷卻映射
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Traits")
    TMap<EDemonAbilityType, float> AbilityCooldowns;

    // 能力映射
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Traits")
    TMap<EDemonAbilityType, FDemonAbility> DemonAbilitiesMap;

    // 最大僕從數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Traits")
    int32 MaxMinions;

    // 黑暗能量恢復速率
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Traits")
    float DarkEnergyRestoreRate;

    // 恐懼增長速率
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Traits")
    float FearGrowthRate;
};
