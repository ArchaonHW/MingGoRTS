// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 势力关系网络系统头文件

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/Object.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "MingRTSFactionRelationshipNetwork.generated.h"

class UMingRTSFactionBase;

/**
 * 势力关系类型枚举
 */
UENUM(BlueprintType)
enum class EFactionRelationType : uint8
{
    Alliance        UMETA(DisplayName = "同盟"),
    Friendly        UMETA(DisplayName = "友好"),
    Neutral         UMETA(DisplayName = "中立"),
    Unfriendly      UMETA(DisplayName = "不友好"),
    Hostile         UMETA(DisplayName = "敌对"),
    War             UMETA(DisplayName = "战争"),
    Vassal          UMETA(DisplayName = "附庸"),
    Overlord        UMETA(DisplayName = "宗主"),
    TradePartner    UMETA(DisplayName = "贸易伙伴"),
    NonAggression   UMETA(DisplayName = "互不侵犯")
};

/**
 * 势力关系数据
 */
USTRUCT(BlueprintType)
struct FFactionRelation
{
    GENERATED_BODY()
    
    /** 目标势力ID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName TargetFactionID;
    
    /** 关系类型 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFactionRelationType RelationType;
    
    /** 关系强度 (-100 到 100) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "-100", ClampMax = "100"))
    int32 RelationStrength;
    
    /** 信任度 (0 到 100) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "100"))
    int32 TrustLevel;
    
    /** 上次互动时间 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime LastInteractionTime;
    
    /** 关系历史记录 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> RelationHistory;
    
    /** 是否活跃 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsActive;
    
    /** 关系条约 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FName> ActiveTreaties;
    
    FFactionRelation()
        : RelationType(EFactionRelationType::Neutral)
        , RelationStrength(0)
        , TrustLevel(50)
        , bIsActive(true)
    {}
};

/**
 * 外交行动类型
 */
UENUM(BlueprintType)
enum class EDiplomaticAction : uint8
{
    ProposeAlliance         UMETA(DisplayName = "提议同盟"),
    BreakAlliance           UMETA(DisplayName = "断绝同盟"),
    DeclareWar              UMETA(DisplayName = "宣战"),
    OfferPeace              UMETA(DisplayName = "议和"),
    ProposeTrade            UMETA(DisplayName = "提议贸易"),
    SendGift                UMETA(DisplayName = "赠送礼物"),
    DemandTribute           UMETA(DisplayName = "要求朝贡"),
    Insult                  UMETA(DisplayName = "侮辱"),
    ImproveRelations        UMETA(DisplayName = "改善关系"),
    RequestMilitaryAccess   UMETA(DisplayName = "请求军事通行权"),
    FormNonAggressionPact   UMETA(DisplayName = "签订互不侵犯条约"),
    ProposeVassalization    UMETA(DisplayName = "提议附庸"),
    RequestIndependence     UMETA(DisplayName = "请求独立")
};

/**
 * 外交行动结果
 */
USTRUCT(BlueprintType)
struct FDiplomaticActionResult
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSuccess;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Message;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RelationChange;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TrustChange;
    
    FDiplomaticActionResult()
        : bSuccess(false)
        , RelationChange(0)
        , TrustChange(0)
    {}
};

/**
 * 势力网络节点
 */
USTRUCT(BlueprintType)
struct FFactionNetworkNode
{
    GENERATED_BODY()
    
    /** 势力ID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName FactionID;
    
    /** 势力引用 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UMingRTSFactionBase> FactionRef;
    
    /** 与其他势力的关系 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FName, FFactionRelation> Relations;
    
    /** 外交影响力 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DiplomaticPower;
    
    /** 国际声望 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 InternationalReputation;
    
    FFactionNetworkNode()
        : DiplomaticPower(0)
        , InternationalReputation(50)
    {}
};

/**
 * 势力关系网络系统
 * 管理势力间的外交关系和互动
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGGORTS_API UMingRTSFactionRelationshipNetwork : public UObject
{
    GENERATED_BODY()
    
public:
    /** 初始化关系网络 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    void InitializeRelationshipNetwork();
    
    /** 注册势力到网络 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    void RegisterFaction(UMingRTSFactionBase* Faction);
    
    /** 移除势力从网络 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    void UnregisterFaction(FName FactionID);
    
    /** 获取势力关系 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    FFactionRelation GetFactionRelation(FName FactionA, FName FactionB);
    
    /** 设置势力关系 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    void SetFactionRelation(FName FactionA, FName FactionB, EFactionRelationType RelationType, int32 Strength);
    
    /** 修改关系强度 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    void ModifyRelationStrength(FName FactionA, FName FactionB, int32 Delta);
    
    /** 修改信任度 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    void ModifyTrustLevel(FName FactionA, FName FactionB, int32 Delta);
    
    /** 执行外交行动 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    FDiplomaticActionResult ExecuteDiplomaticAction(FName SourceFaction, FName TargetFaction, EDiplomaticAction Action, int32 Intensity = 50);
    
    /** 检查是否可进行某项外交行动 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    bool CanExecuteDiplomaticAction(FName SourceFaction, FName TargetFaction, EDiplomaticAction Action);
    
    /** 获取势力的所有盟友 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    TArray<FName> GetAllies(FName FactionID);
    
    /** 获取势力的所有敌人 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    TArray<FName> GetEnemies(FName FactionID);
    
    /** 获取势力的贸易伙伴 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    TArray<FName> GetTradePartners(FName FactionID);
    
    /** 计算势力间的最短外交路径 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    TArray<FName> FindDiplomaticPath(FName StartFaction, FName EndFaction);
    
    /** 获取势力的外交孤立度 (0-100) */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    int32 CalculateIsolationLevel(FName FactionID);
    
    /** 计算外交影响力得分 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    int32 CalculateDiplomaticInfluenceScore(FName FactionID);
    
    /** 获取网络中势力间的关系矩阵 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    TMap<FName, TMap<FName, int32>> GetRelationMatrix();
    
    /** 检测势力间是否存在共同敌人 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    TArray<FName> FindCommonEnemies(FName FactionA, FName FactionB);
    
    /** 检测势力间是否存在共同盟友 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    TArray<FName> FindCommonAllies(FName FactionA, FName FactionB);
    
    /** 计算网络稳定性指数 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    float CalculateNetworkStability();
    
    /** 获取势力节点的引用 */
    UFUNCTION(BlueprintPure, Category = "Faction|Network")
    UMingRTSFactionBase* GetFactionNode(FName FactionID) const;
    
    /** 获取所有势力的关系摘要 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    TMap<FName, TArray<FFactionRelation>> GetAllRelationsForFaction(FName FactionID);
    
    /** 记录外交互动 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    void RecordDiplomaticInteraction(FName SourceFaction, FName TargetFaction, EDiplomaticAction Action, bool bSuccess);
    
    /** 定期衰减关系 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    void ProcessRelationDecay(float DeltaTime);
    
    /** 处理历史预设关系 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    void ApplyHistoricalRelations();
    
    /** 获取势力关系描述文本 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Network")
    FText GetRelationDescription(FName FactionA, FName FactionB);
    
    UPROPERTY(BlueprintAssignable, Category = "Faction|Network")
    FOnRelationChanged OnRelationChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Faction|Network")
    FOnDiplomaticActionExecuted OnDiplomaticActionExecuted;
    
    UPROPERTY(BlueprintAssignable, Category = "Faction|Network")
    FOnWarDeclared OnWarDeclared;
    
    UPROPERTY(BlueprintAssignable, Category = "Faction|Network")
    FOnPeaceMade OnPeaceMade;
    
    UPROPERTY(BlueprintAssignable, Category = "Faction|Network")
    FOnAllianceFormed OnAllianceFormed;
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRelationChanged, FName, FactionA, FName, FactionB, FFactionRelation, NewRelation);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDiplomaticActionExecuted, FName, SourceFaction, FName, TargetFaction, EDiplomaticAction, Action);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWarDeclared, FName, Attacker, FName, Defender);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPeaceMade, FName, FactionA, FName, FactionB);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAllianceFormed, FName, FactionA, FName, FactionB);

protected:
    UPROPERTY()
    TMap<FName, FFactionNetworkNode> NetworkNodes;
    
    UPROPERTY()
    TMap<FName, TObjectPtr<UMingRTSFactionBase>> RegisteredFactions;
    
    /** 获取关系类型的默认强度 */
    int32 GetDefaultRelationStrength(EFactionRelationType RelationType) const;
    
    /** 计算外交行动成功率 */
    float CalculateActionSuccessChance(FName SourceFaction, FName TargetFaction, EDiplomaticAction Action) const;
    
    /** 应用关系变化效果 */
    void ApplyRelationEffects(FName FactionA, FName FactionB);
    
    /** 通知第三方势力关系变化 */
    void NotifyThirdParties(FName FactionA, FName FactionB, EFactionRelationType OldRelation, EFactionRelationType NewRelation);

private:
    float RelationDecayRate = 0.5f; // 每游戏日衰减值
};
