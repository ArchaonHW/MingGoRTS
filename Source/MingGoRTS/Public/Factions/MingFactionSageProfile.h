// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SageCommand/MingSageCharacterSystem.h"
#include "MingFactionSageProfile.generated.h"

/**
 * 民國時期勢力類型枚舉 (12可玩勢力)
 */
UENUM(BlueprintType)
enum class ERepublicFaction : uint8
{
    BeiyangGovernment   UMETA(DisplayName = "北洋政府"),
    NationalistGovernment UMETA(DisplayName = "国民政府"),
    CommunistParty      UMETA(DisplayName = "中国共产党"),
    FengtianClique      UMETA(DisplayName = "奉系军阀"),
    ZhiliClique         UMETA(DisplayName = "直系军阀"),
    AnhuiClique         UMETA(DisplayName = "皖系军阀"),
    ShanxiClique        UMETA(DisplayName = "晋系军阀"),
    GuangxiClique       UMETA(DisplayName = "桂系军阀"),
    YunnanClique        UMETA(DisplayName = "滇系军阀"),
    SichuanClique       UMETA(DisplayName = "川系军阀"),
    MaFamily            UMETA(DisplayName = "马家军"),
    XinjiangForces      UMETA(DisplayName = "新疆势力"),
    Count
};

/**
 * 勢力指揮哲學類型
 */
UENUM(BlueprintType)
enum class EFactionCommandPhilosophy : uint8
{
    RighteousOnly       UMETA(DisplayName = "純正策型"),      // 只用正策
    RighteousPrimary    UMETA(DisplayName = "正策為主型"),    // 以正策為主
    Balanced            UMETA(DisplayName = "平衡型"),        // 正逆平衡
    EvilPrimary         UMETA(DisplayName = "逆策為主型"),    // 以逆策為主
    EvilOnly            UMETA(DisplayName = "純逆策型"),      // 只用逆策
    Deceptive           UMETA(DisplayName = "欺騙型"),        // 表面正實際逆
    Count
};

/**
 * 勢力專屬策略修正數據
 */
USTRUCT(BlueprintType)
struct MINGRTS_API FFactionStrategyModifier
{
    GENERATED_BODY()

    // 正策效果倍率
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionStrategy")
    float RighteousStrategyMultiplier = 1.0f;

    // 逆策效果倍率
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionStrategy")
    float EvilStrategyMultiplier = 1.0f;

    // 正策冷卻時間修正
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionStrategy")
    float RighteousCooldownModifier = 1.0f;

    // 逆策冷卻時間修正
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionStrategy")
    float EvilCooldownModifier = 1.0f;

    // 逆策墮落值修正
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionStrategy")
    float EvilFallValueModifier = 1.0f;

    // 專屬正策列表
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionStrategy")
    TArray<int32> ExclusiveRighteousStrategies;

    // 專屬逆策列表
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionStrategy")
    TArray<int32> ExclusiveEvilStrategies;

    FFactionStrategyModifier()
        : RighteousStrategyMultiplier(1.0f)
        , EvilStrategyMultiplier(1.0f)
        , RighteousCooldownModifier(1.0f)
        , EvilCooldownModifier(1.0f)
        , EvilFallValueModifier(1.0f)
    {}
};

/**
 * 勢力五行偏好數據
 */
USTRUCT(BlueprintType)
struct MINGRTS_API FFactionWuXingAffinity
{
    GENERATED_BODY()

    // 主五行屬性
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionWuXing")
    int32 PrimaryElement = 0; // 0=木, 1=火, 2=土, 3=金, 4=水

    // 次五行屬性
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionWuXing")
    int32 SecondaryElement = 1;

    // 有利階段效果加成
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionWuXing")
    float FavorablePhaseBonus = 0.3f;

    // 不利階段效果減免
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionWuXing")
    float UnfavorablePhasePenaltyReduction = 0.5f;

    // 相生效果額外加成
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionWuXing")
    float GeneratingCycleBonus = 0.2f;

    // 相剋效果減免
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionWuXing")
    float OvercomingCyclePenaltyReduction = 0.3f;

    FFactionWuXingAffinity()
        : PrimaryElement(0)
        , SecondaryElement(1)
        , FavorablePhaseBonus(0.3f)
        , UnfavorablePhasePenaltyReduction(0.5f)
        , GeneratingCycleBonus(0.2f)
        , OvercomingCyclePenaltyReduction(0.3f)
    {}
};

/**
 * 勢力贖罪任務配置
 */
USTRUCT(BlueprintType)
struct MINGRTS_API FFactionAtonementConfig
{
    GENERATED_BODY()

    // 勢力專屬贖罪任務名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionAtonement")
    FString FactionAtonementTaskName;

    // 贖罪任務描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionAtonement")
    FString AtonementDescription;

    // 贖罪目標類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionAtonement")
    int32 AtonementTargetType = 0; // 0=軍事勝利, 1=政治目標, 2=經濟建設, 3=外交成就

    // 贖罪完成獎勵類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionAtonement")
    int32 RewardType = 0; // 0=士氣, 1=資源, 2=威望, 3=策略效果

    // 贖罪完成獎勵數值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionAtonement")
    float RewardValue = 50.0f;

    FFactionAtonementConfig()
        : AtonementTargetType(0)
        , RewardType(0)
        , RewardValue(50.0f)
    {}
};

/**
 * 勢力至聖者指揮學屬性配置文件
 * 定義每個勢力的指揮哲學、策略偏好、五行屬性等
 */
USTRUCT(BlueprintType)
struct MINGRTS_API FFactionSageProfile
{
    GENERATED_BODY()

    // 所屬勢力
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionSage")
    ERepublicFaction Faction = ERepublicFaction::BeiyangGovernment;

    // 預設角色類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionSage")
    ESageCharacterType DefaultCharacterType = ESageCharacterType::PseudoSage;

    // 指揮哲學類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionSage")
    EFactionCommandPhilosophy Philosophy = EFactionCommandPhilosophy::Deceptive;

    // 墮落閾值修正
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionSage")
    int32 FallThresholdModifier = 0; // 基於基礎值的增減

    // 策略修正
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionSage")
    FFactionStrategyModifier StrategyModifier;

    // 五行偏好
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionSage")
    FFactionWuXingAffinity WuXingAffinity;

    // 贖罪任務配置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionSage")
    FFactionAtonementConfig AtonementConfig;

    // 勢力描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionSage")
    FString FactionDescription;

    // 指揮哲學描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FactionSage")
    FString PhilosophyDescription;

    FFactionSageProfile()
        : Faction(ERepublicFaction::BeiyangGovernment)
        , DefaultCharacterType(ESageCharacterType::PseudoSage)
        , Philosophy(EFactionCommandPhilosophy::Deceptive)
        , FallThresholdModifier(0)
    {}
};

/**
 * 勢力指揮學管理器
 * 管理所有勢力的指揮學屬性和遊戲內應用
 */
UCLASS(ClassGroup = (Faction), meta = (BlueprintSpawnableComponent))
class MINGRTS_API UMingFactionSageManager : public UObject
{
    GENERATED_BODY()

public:
    UMingFactionSageManager();

    // 初始化管理器
    UFUNCTION(BlueprintCallable, Category = "Faction|SageCommand")
    void InitializeFactionSageManager();

    // 獲取勢力指揮學屬性
    UFUNCTION(BlueprintCallable, Category = "Faction|SageCommand")
    FFactionSageProfile GetFactionProfile(ERepublicFaction Faction) const;

    // 設置當前玩家勢力
    UFUNCTION(BlueprintCallable, Category = "Faction|SageCommand")
    void SetPlayerFaction(ERepublicFaction Faction);

    // 獲取當前玩家勢力
    UFUNCTION(BlueprintCallable, Category = "Faction|SageCommand")
    ERepublicFaction GetPlayerFaction() const { return PlayerFaction; }

    // 獲取當前勢力的角色類型
    UFUNCTION(BlueprintCallable, Category = "Faction|SageCommand")
    ESageCharacterType GetPlayerCharacterType() const;

    // 獲取當前勢力的墮落閾值
    UFUNCTION(BlueprintCallable, Category = "Faction|SageCommand")
    int32 GetPlayerFallThreshold() const;

    // 計算策略效果修正
    UFUNCTION(BlueprintCallable, Category = "Faction|SageCommand")
    float CalculateStrategyEffectModifier(ESixStrategyType Strategy, bool bIsEvil) const;

    // 計算五行階段加成
    UFUNCTION(BlueprintCallable, Category = "Faction|SageCommand")
    float CalculateWuXingPhaseBonus(int32 CurrentPhase) const;

    // 檢查是否為有利階段
    UFUNCTION(BlueprintCallable, Category = "Faction|SageCommand")
    bool IsFavorableWuXingPhase(int32 CurrentPhase) const;

    // 獲取勢力指揮建議
    UFUNCTION(BlueprintCallable, Category = "Faction|SageCommand")
    FString GetFactionCommandAdvice() const;

    // 獲取勢力專屬贖罪任務名稱
    UFUNCTION(BlueprintCallable, Category = "Faction|SageCommand")
    FString GetFactionAtonementTaskName() const;

    // 獲取勢力專屬贖罪任務描述
    UFUNCTION(BlueprintCallable, Category = "Faction|SageCommand")
    FString GetFactionAtonementDescription() const;

    // 檢查是否可使用特定策略
    UFUNCTION(BlueprintCallable, Category = "Faction|SageCommand")
    bool CanUseStrategy(ESixStrategyType Strategy, bool bIsEvil) const;

    // 獲取可用策略列表
    UFUNCTION(BlueprintCallable, Category = "Faction|SageCommand")
    TArray<ESixStrategyType> GetAvailableStrategies(bool bIsEvil) const;

    // 註冊勢力配置
    UFUNCTION(BlueprintCallable, Category = "Faction|SageCommand")
    void RegisterFactionProfile(const FFactionSageProfile& Profile);

protected:
    // 初始化所有12勢力的預設配置
    void InitializeDefaultFactionProfiles();

    // 創建北洋政府配置
    FFactionSageProfile CreateBeiyangProfile() const;

    // 創建国民政府配置
    FFactionSageProfile CreateNationalistProfile() const;

    // 創建中共配置
    FFactionSageProfile CreateCommunistProfile() const;

    // 創建奉系配置
    FFactionSageProfile CreateFengtianProfile() const;

    // 創建直系配置
    FFactionSageProfile CreateZhiliProfile() const;

    // 創建皖系配置
    FFactionSageProfile CreateAnhuiProfile() const;

    // 創建晉系配置
    FFactionSageProfile CreateShanxiProfile() const;

    // 創建桂系配置
    FFactionSageProfile CreateGuangxiProfile() const;

    // 創建滇系配置
    FFactionSageProfile CreateYunnanProfile() const;

    // 創建川系配置
    FFactionSageProfile CreateSichuanProfile() const;

    // 創建馬家軍配置
    FFactionSageProfile CreateMaFamilyProfile() const;

    // 創建新疆配置
    FFactionSageProfile CreateXinjiangProfile() const;

protected:
    // 勢力配置映射
    UPROPERTY()
    TMap<ERepublicFaction, FFactionSageProfile> FactionProfiles;

    // 當前玩家勢力
    UPROPERTY()
    ERepublicFaction PlayerFaction = ERepublicFaction::BeiyangGovernment;

    // 是否已初始化
    UPROPERTY()
    bool bIsInitialized = false;
};
