// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSageCharacterSystem.generated.h"

/**
 * 至聖者指揮學 - 角色類型枚舉
 * 聖者：能用而能停，正逆切換無懲罰
 * 魔王：能用而不能停，逆策效果+50%但無法停止
 * 偽聖者：不能用卻假裝不用，外交加成但無法使用逆策
 */
UENUM(BlueprintType)
enum class ESageCharacterType: uint8 {
    None            UMETA(DisplayName = "None"),
    Sage            UMETA(DisplayName = "聖者"),      // 至聖者 - 正邪皆兵而不墮
    DemonKing       UMETA(DisplayName = "魔王"),      // 魔王 - 能用邪而不能停
    PseudoSage      UMETA(DisplayName = "偽聖者"),    // 偽聖者 - 不能用而假裝不用
    Count
};

/**
 * 角色特性數據結構
 */
USTRUCT(BlueprintType)
struct MINGGORTS_API FSageCharacterTraits
{
    GENERATED_BODY()

    // 正策效果修正 (1.0 = 100%)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    float RighteousStrategyMultiplier = 1.0f;

    // 逆策效果修正
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    float EvilStrategyMultiplier = 1.0f;

    // 墮落閾值 (達到此值進入墮落狀態)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    int32 FallThreshold = 100;

    // 外交加成
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    float DiplomacyBonus = 0.0f;

    // 能否使用逆策
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
struct MINGGORTS_API FSageCharacterData
{
    GENERATED_BODY()

    // 角色類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    ESageCharacterType CharacterType = ESageCharacterType::None;

    // 角色名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    FString CharacterName;

    // 角色描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    FString CharacterDescription;

    // 角色特性
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    FSageCharacterTraits Traits;

    // 當前墮落值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    int32 CurrentFallValue = 0;

    // 是否處於墮落狀態
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    bool bIsFallen = false;

    // 已使用逆策次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SageCharacter")
    int32 EvilStrategyUseCount = 0;

    // 已使用正策次數
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
 * 至聖者指揮學 - 角色類型系統
 * 管理聖者、魔王、偽聖者三種角色類型
 */
UCLASS(ClassGroup = (SageCommand), meta = (BlueprintSpawnableComponent))
class MINGGORTS_API UMingSageCharacterSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingSageCharacterSystem();

    // 初始化系統
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Character")
    void InitializeSageCharacterSystem();

    // 創建角色
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Character")
    FSageCharacterData CreateCharacter(ESageCharacterType CharacterType, const FString& CharacterName);

    // 獲取角色特性
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Character")
    FSageCharacterTraits GetCharacterTraits(ESageCharacterType CharacterType) const;

    // 獲取角色描述
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Character")
    FString GetCharacterDescription(ESageCharacterType CharacterType) const;

    // 檢查角色是否可以使用逆策
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Character")
    bool CanUseEvilStrategy(const FSageCharacterData& CharacterData) const;

    // 檢查角色是否可以停止逆策
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Character")
    bool CanStopEvilStrategy(const FSageCharacterData& CharacterData) const;

    // 使用逆策 (增加墮落值)
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Character")
    bool UseEvilStrategy(FSageCharacterData& CharacterData, int32 FallValueIncrease);

    // 使用正策 (減少墮落值)
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Character")
    bool UseRighteousStrategy(FSageCharacterData& CharacterData, int32 FallValueDecrease);

    // 檢查是否墮落
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Character")
    bool CheckAndApplyFall(FSageCharacterData& CharacterData);

    // 贖罪 (大幅減少墮落值)
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Character")
    bool Atonement(FSageCharacterData& CharacterData);

    // 獲取角色類型顯示名稱
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Character")
    FString GetCharacterTypeDisplayName(ESageCharacterType CharacterType) const;

    // 獲取所有可用角色類型
    UFUNCTION(BlueprintCallable, Category = "SageCommand|Character")
    TArray<ESageCharacterType> GetAvailableCharacterTypes() const;

    // 事件：角色墮落
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterFallen, const FSageCharacterData&, CharacterData);
    UPROPERTY(BlueprintAssignable, Category = "SageCommand|Character")
    FOnCharacterFallen OnCharacterFallen;

    // 事件：墮落值變化
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFallValueChanged, const FSageCharacterData&, CharacterData, int32, NewValue);
    UPROPERTY(BlueprintAssignable, Category = "SageCommand|Character")
    FOnFallValueChanged OnFallValueChanged;

protected:
    // 初始化默認角色特性
    void InitializeDefaultTraits();

    // 初始化角色描述
    void InitializeCharacterDescriptions();

    // 應用墮落效果
    void ApplyFallenEffects(FSageCharacterData& CharacterData);

    // 檢查魔王強制逆策
    void CheckDemonKingForcedEvil(FSageCharacterData& CharacterData);

protected:
    // 角色特性映射
    UPROPERTY()
    TMap<ESageCharacterType, FSageCharacterTraits> CharacterTraitsMap;

    // 角色描述映射
    UPROPERTY()
    TMap<ESageCharacterType, FString> CharacterDescriptionMap;

    // 是否已初始化
    UPROPERTY()
    bool bIsInitialized = false;

    // 魔王強制逆策檢查間隔 (秒)
    UPROPERTY(EditDefaultsOnly, Category = "SageCommand|Character")
    float DemonKingCheckInterval = 30.0f;

    // 贖罪減少的墮落值
    UPROPERTY(EditDefaultsOnly, Category = "SageCommand|Character")
    int32 AtonementFallReduction = 50;

    // 每次使用正策減少的墮落值
    UPROPERTY(EditDefaultsOnly, Category = "SageCommand|Character")
    int32 RighteousStrategyFallReduction = 10;
};
