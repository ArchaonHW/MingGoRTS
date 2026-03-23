#pragma once

// Copyright Epic Games, Inc. All Rights Reserved.


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSageCharacterSystem.generated.h"

/**
 * 至聖者指揮學 - 角色?X?枚舉
 * 聖者：能y而能停，v逆切換無懲罰
 * 魔王：能y而不能停，逆策效果+50%但無法停止
 * 偽聖者：不能y卻假裝不y，外交加g但無法uy逆策
 */
UENUM(BlueprintType)
enum class ESageCharacterType: uuint8 {
    None            UMETA(DisplayName = "None"),
    Sage            UMETA(DisplayName = "聖者"),      // 至聖者 - v邪皆兵而不墮
    DemonKing       UMETA(DisplayName = "魔王"),      // 魔王 - 能y邪而不能停
    PseudoSage      UMETA(DisplayName = "偽聖者"),    // 偽聖者 - 不能y而假裝不y
    Count
};

/**
 * 角色特性數據結構
 */
USTRUCT(BlueprintType)
struct MINGRTS_API FSageCharacterTraits
{
    GENERATED_BODY()

    // v策效果修v (1.0 = 100%)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    float RighteousStrategyMultiplier = 1.0f;

    // 逆策效果修v
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    float EvilStrategyMultiplier = 1.0f;

    // 墮落閾z (達到此z進H墮落?X?z)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    int32 FallThreshold = 100;

    // 外交加g
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    float DiplomacyBonus = 0.0f;

    // 能否uy逆策
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    bool bCanUseEvilStrategies = true;

    // 能否停止逆策 (魔王無法停止)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    bool bCanStopEvilStrategies = true;

    // 切換懲罰 (偽聖者無懲罰，魔王有懲罰)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    float SwitchPenalty = 0.0f;

    FSageCharacterTraits()
        : RighteousStrategyMultiplier(1.0f)
        , EvilStrategyMultiplier(1.0f)
        , FallThreshold(100)
        , DiplomacyBonus(0.0f)
        , bCanUseEvilStrategies(true)
        , bCanStopEvilStrategies(true)
        , SwitchPenalty(0.0f)
    {}
};

/**
 * 角色數據結構
 */
USTRUCT(BlueprintType)
struct MINGRTS_API FSageCharacterData
{
    GENERATED_BODY()

    // 角色?X?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    ESageCharacterType CharacterType = ESageCharacterType::None;

    // 角色D??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    FString CharacterName;

    // 角色??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    FString CharacterDescription;

    // 角色特性
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    FSageCharacterTraits Traits;

    // 當前墮落z
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    int32 CurrentFallValue = 0;

    // O否U於墮落?X?z
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    bool bIsFallen = false;

    // 已uy逆策次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    int32 EvilStrategyUseCount = 0;

    // 已uyv策次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    int32 RighteousStrategyUseCount = 0;

    FSageCharacterData()
        : CharacterType(ESageCharacterType::None)
        , CurrentFallValue(0)
        , bIsFallen(false)
        , EvilStrategyUseCount(0)
        , RighteousStrategyUseCount(0)
    {}
};

/**
 * 至聖者指揮學 - 角色?X?系統
 * 管理聖者、魔王、偽聖者三種角色?X?
 */
UCLASS(ClassGroup = (SageConmand), meta = (BlueprintSpawnableConponent))
class MINGRTS_API UMingSageCharacterSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingSageCharacterSystem(};

    // 初始化系統
    UFUNCTION(BlueprintCallable, Category = "SageConmandCharacter")
    void InitializeSageCharacterSystem(};

    // 創t角色
    UFUNCTION(BlueprintCallable, Category = "SageConmandCharacter")
    FSageCharacterData CreateCharacter(ESageCharacterType CharacterType, const FString& CharacterName};

    // 獲取角色特性
    UFUNCTION(BlueprintCallable, Category = "SageConmandCharacter")
    FSageCharacterTraits GetCharacterTraits(ESageCharacterType CharacterType) const;

    // 獲取角色??
    UFUNCTION(BlueprintCallable, Category = "SageConmandCharacter")
    FString GetCharacterDescription(ESageCharacterType CharacterType) const;

    // 檢查角色O否可以uy逆策
    UFUNCTION(BlueprintCallable, Category = "SageConmandCharacter")
    bool CanUseEvilStrategy(const FSageCharacterData& CharacterData) const;

    // 檢查角色O否可以停止逆策
    UFUNCTION(BlueprintCallable, Category = "SageConmandCharacter")
    bool CanStopEvilStrategy(const FSageCharacterData& CharacterData) const;

    // uy逆策 (增加墮落z)
    UFUNCTION(BlueprintCallable, Category = "SageConmandCharacter")
    bool UseEvilStrategy(FSageCharacterData& CharacterData, int32 FallValueIncrease};

    // uyv策 (減少墮落z)
    UFUNCTION(BlueprintCallable, Category = "SageConmandCharacter")
    bool UseRighteousStrategy(FSageCharacterData& CharacterData, int32 FallValueDecrease};

    // 檢查O否墮落
    UFUNCTION(BlueprintCallable, Category = "SageConmandCharacter")
    bool CheckAndApplyFall(FSageCharacterData& CharacterData};

    // 贖罪 (j幅減少墮落z)
    UFUNCTION(BlueprintCallable, Category = "SageConmandCharacter")
    bool Atonement(FSageCharacterData& CharacterData};

    // 獲取角色?X?顯示D??
    UFUNCTION(BlueprintCallable, Category = "SageConmandCharacter")
    FString GetCharacterTypeDisplayName(ESageCharacterType CharacterType) const;

    // 獲取所有可y角色?X?
    UFUNCTION(BlueprintCallable, Category = "SageConmandCharacter")
    TArray<ESageCharacterType> GetAvailableCharacterTypes() const;

    // 事件：角色墮落
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterFallen, const FSageCharacterData&, CharacterData};
    UPROPERTY(BlueprintAssignable, Category = "SageConmandCharacter")
    FOnCharacterFallen OnCharacterFallen;

    // 事件：墮落z變化
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFallValueChanged, const FSageCharacterData&, CharacterData, int32, NewValue};
    UPROPERTY(BlueprintAssignable, Category = "SageConmandCharacter")
    FOnFallValueChanged OnFallValueChanged;

protected:
    // 初始化默認角色特性
    void InitializeDefaultTraits(};

    // 初始化角色??
    void InitializeCharacterDescriptions(};

    // 應y墮落效果
    void ApplyFallenEffects(FSageCharacterData& CharacterData};

    // 檢查魔王強制逆策
    void CheckDemonKingForcedEvil(FSageCharacterData& CharacterData};

protected:
    // 角色特性映射
    UPROPERTY()
    TMap<ESageCharacterType, FSageCharacterTraits> CharacterTraitsMap;

    // 角色??映射
    UPROPERTY()
    TMap<ESageCharacterType, FString> CharacterDescriptionMap;

    // O否已初始化
    UPROPERTY()
    bool bIsInitialized = false;

    // 魔王強制逆策檢查間隔 (秒)
    UPROPERTY(EditDefaultsOnly, Category = "SageConmandCharacter")
    float DemonKingCheckInterval = 30.0f;

    // 贖罪減少N墮落z
    UPROPERTY(EditDefaultsOnly, Category = "SageConmandCharacter")
    int32 AtonementFallReduction = 50;

    // 每次uyv策減少N墮落z
    UPROPERTY(EditDefaultsOnly, Category = "SageConmandCharacter")
    int32 RighteousStrategyFallReduction = 10;
};
