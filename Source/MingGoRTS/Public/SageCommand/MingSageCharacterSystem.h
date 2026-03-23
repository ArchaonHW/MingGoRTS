#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "MingSageCharacterSystem.generated.h"

// ============================================================================
// 前向聲明
// ============================================================================

class UMingThreePowerSystem;
class UMingYinYangFiveElements;

// ============================================================================
// 聖者角色類型枚舉定義
// ============================================================================

/** 聖者角色類型 */
UENUM(BlueprintType)
enum class ESageCharacterType : uint8
{
    Sage        UMETA(DisplayName = "聖者"),
    DemonLord   UMETA(DisplayName = "魔王"),
    FalseSage   UMETA(DisplayName = "偽聖者")
};

/** 角色狀態 */
UENUM(BlueprintType)
enum class ESageCharacterState : uint8
{
    Normal      UMETA(DisplayName = "正常"),
    Corrupted   UMETA(DisplayName = "墮落"),
    Enlightened UMETA(DisplayName = "覺醒"),
    Fallen      UMETA(DisplayName = "沉淪")
};

/** 角色成長階段 */
UENUM(BlueprintType)
enum class ESageGrowthStage : uint8
{
    Novice      UMETA(DisplayName = "初學者"),
    Adept       UMETA(DisplayName = "熟練者"),
    Master      UMETA(DisplayName = "大師"),
    Supreme     UMETA(DisplayName = "至聖者")
};

// ============================================================================
// 聖者角色數據結構
// ============================================================================

/** 聖者角色基礎數據 */
USTRUCT(BlueprintType)
struct MINGRTS_API FSageCharacterData
{
    GENERATED_BODY()

    /** 角色類型 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
    ESageCharacterType CharacterType = ESageCharacterType::Sage;

    /** 角色名稱 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
    FString CharacterName;

    /** 角色等級 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
    int32 Level = 1;

    /** 經驗值 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
    int32 Experience = 0;

    /** 靈力值 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
    float SpiritualPower = 100.0f;

    /** 靈力最大值 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
    float MaxSpiritualPower = 100.0f;

    /** 魅力值 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
    float Charisma = 50.0f;

    /** 智慧值 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
    float Wisdom = 50.0f;

    /** 領導力 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
    float Leadership = 50.0f;

    /** 墮落值 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
    float CorruptionValue = 0.0f;

    /** 墮落閾值 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
    float CorruptionThreshold = 150.0f;

    /** 角色狀態 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
    ESageCharacterState CharacterState = ESageCharacterState::Normal;

    /** 成長階段 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
    ESageGrowthStage GrowthStage = ESageGrowthStage::Novice;

    /** 是否可以切換正逆策略 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
    bool CanSwitchStrategy = true;

    /** 策略切換冷卻時間 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
    float StrategySwitchCooldown = 0.0f;
};

/** 聖者特性數據 */
USTRUCT(BlueprintType)
struct MINGRTS_API FSageCharacterTraits
{
    GENERATED_BODY()

    /** 聖者特性：正逆策略切換無懲罰 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Traits")
    bool NoStrategyPenalty = true;

    /** 聖者特性：墮落閾值提升 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Traits")
    float CorruptionThresholdBonus = 50.0f;

    /** 聖者特性：平衡型數值修正 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Traits")
    float BalancedStatModifier = 1.1f;

    /** 魔王特性：攻擊力加成 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Traits")
    float AttackPowerBonus = 1.5f;

    /** 魔王特性：防禦力減免 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Traits")
    float DefenseReduction = 0.8f;

    /** 魔王特性：墮落速度加快 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demon Traits")
    float CorruptionRateMultiplier = 1.5f;

    /** 偽聖者特性：欺騙成功率 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "False Sage Traits")
    float DeceptionSuccessRate = 0.7f;

    /** 偽聖者特性：背叛機率 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "False Sage Traits")
    float BetrayalChance = 0.3f;

    /** 偽聖者特性：隱藏真實身份 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "False Sage Traits")
    bool HideTrueIdentity = true;
};

// ============================================================================
// 聖者角色系統主類
// ============================================================================

/**
 * 聖者角色系統
 * 管理聖者、魔王、偽聖者三種角色的特性、狀態和成長
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingSageCharacterSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingSageCharacterSystem();

    // ============================================================================
    // 系統初始化
    // ============================================================================

    /** 初始化角色系統 */
    UFUNCTION(BlueprintCallable, Category = "Sage Character System")
    void InitializeCharacterSystem(ESageCharacterType InCharacterType);

    /** 設置角色數據 */
    UFUNCTION(BlueprintCallable, Category = "Sage Character System")
    void SetCharacterData(const FSageCharacterData& InCharacterData);

    /** 獲取角色數據 */
    UFUNCTION(BlueprintCallable, Category = "Sage Character System")
    FSageCharacterData GetCharacterData() const { return CharacterData; }

    // ============================================================================
    // 角色類型管理
    // ============================================================================

    /** 創建指定類型的角色 */
    UFUNCTION(BlueprintCallable, Category = "Sage Character System")
    bool CreateCharacter(ESageCharacterType CharacterType, const FString& CharacterName);

    /** 切換角色類型（特殊情況下使用） */
    UFUNCTION(BlueprintCallable, Category = "Sage Character System")
    bool SwitchCharacterType(ESageCharacterType NewType);

    /** 獲取角色類型描述 */
    UFUNCTION(BlueprintCallable, Category = "Sage Character System")
    FString GetCharacterTypeDescription() const;

    // ============================================================================
    // 角色特性管理
    // ============================================================================

    /** 應用角色特性 */
    UFUNCTION(BlueprintCallable, Category = "Sage Character System")
    void ApplyCharacterTraits();

    /** 獲取角色特性 */
    UFUNCTION(BlueprintCallable, Category = "Sage Character System")
    FSageCharacterTraits GetCharacterTraits() const { return CharacterTraits; }

    /** 更新角色數值 */
    UFUNCTION(BlueprintCallable, Category = "Sage Character System")
    void UpdateCharacterStats();

    // ============================================================================
    // 墮落系統
    // ============================================================================

    /** 增加墮落值 */
    UFUNCTION(BlueprintCallable, Category = "Sage Character System")
    void AddCorruption(float Amount);

    /** 減少墮落值 */
    UFUNCTION(BlueprintCallable, Category = "Sage Character System")
    void ReduceCorruption(float Amount);

    /** 檢查墮落狀態 */
    UFUNCTION(BlueprintCallable, Category = "Sage Character System")
    bool CheckCorruptionStatus();

    /** 處理墮落轉變 */
    UFUNCTION(BlueprintCallable, Category = "Sage Character System")
    void HandleCorruptionTransformation();

    // ============================================================================
    // 成長系統
    // ============================================================================

    /** 增加經驗值 */
    UFUNCTION(BlueprintCallable, Category = "Sage Character System")
    void AddExperience(int32 Amount);

    /** 檢查升級 */
    UFUNCTION(BlueprintCallable, Category = "Sage Character System")
    bool CheckLevelUp();

    /** 提升成長階段 */
    UFUNCTION(BlueprintCallable, Category = "Sage Character System")
    bool PromoteGrowthStage();

    /** 獲取成長階段描述 */
    UFUNCTION(BlueprintCallable, Category = "Sage Character System")
    FString GetGrowthStageDescription() const;

    // ============================================================================
    // 策略系統
    // ============================================================================

    /** 檢查是否可以切換策略 */
    UFUNCTION(BlueprintCallable, Category = "Sage Character System")
    bool CanSwitchStrategy() const;

    /** 執行策略切換 */
    UFUNCTION(BlueprintCallable, Category = "Sage Character System")
    bool ExecuteStrategySwitch();

    /** 更新策略冷卻 */
    UFUNCTION(BlueprintCallable, Category = "Sage Character System")
    void UpdateStrategyCooldown(float DeltaTime);

    // ============================================================================
    // 事件系統
    // ============================================================================

    /** 角色狀態改變事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterStateChanged, ESageCharacterState, OldState, ESageCharacterState, NewState);
    UPROPERTY(BlueprintAssignable, Category = "Sage Character System")
    FOnCharacterStateChanged OnCharacterStateChanged;

    /** 角色升級事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterLevelUp, int32, OldLevel, int32, NewLevel);
    UPROPERTY(BlueprintAssignable, Category = "Sage Character System")
    FOnCharacterLevelUp OnCharacterLevelUp;

    /** 角色墮落事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterCorrupted, float, CorruptionValue);
    UPROPERTY(BlueprintAssignable, Category = "Sage Character System")
    FOnCharacterCorrupted OnCharacterCorrupted;

protected:
    // ============================================================================
    // 數據成員
    // ============================================================================

    /** 角色數據 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sage Character System")
    FSageCharacterData CharacterData;

    /** 角色特性 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sage Character System")
    FSageCharacterTraits CharacterTraits;

    /** 三權系統引用 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sage Character System")
    TObjectPtr<UMingThreePowerSystem> ThreePowerSystem;

    /** 陰陽五行系統引用 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sage Character System")
    TObjectPtr<UMingYinYangFiveElements> YinYangSystem;

    /** 是否已初始化 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sage Character System")
    bool bIsInitialized = false;

private:
    // ============================================================================
    // 私有方法
    // ============================================================================

    /** 初始化聖者特性 */
    void InitializeSageTraits();

    /** 初始化魔王特性 */
    void InitializeDemonTraits();

    /** 初始化偽聖者特性 */
    void InitializeFalseSageTraits();

    /** 計算數值修正 */
    void CalculateStatModifiers();

    /** 更新角色狀態 */
    void UpdateCharacterState();
};
