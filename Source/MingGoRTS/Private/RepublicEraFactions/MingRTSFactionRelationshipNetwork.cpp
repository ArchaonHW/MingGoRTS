// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 势力关系网络系统实现

#include "RepublicEraFactions/MingRTSFactionRelationshipNetwork.h"
#include "RepublicEraFactions/MingRTSFactionBase.h"
#include "RepublicEraFactions/MingRTSFactionManager.h"
#include "MingGoRTS.h"

void UMingRTSFactionRelationshipNetwork::InitializeRelationshipNetwork()
{
    NetworkNodes.Empty();
    RegisteredFactions.Empty();
    
    UE_LOG(LogMingGoRTS, Log, TEXT("FactionRelationshipNetwork: Initialized"));
    
    // 应用历史预设关系
    ApplyHistoricalRelations();
}

void UMingRTSFactionRelationshipNetwork::RegisterFaction(UMingRTSFactionBase* Faction)
{
    if (!Faction)
    {
        UE_LOG(LogMingGoRTS, Warning, TEXT("FactionRelationshipNetwork: Cannot register null faction"));
        return;
    }
    
    FName FactionID = Faction->GetFactionAttributes().FactionID;
    
    if (RegisteredFactions.Contains(FactionID))
    {
        UE_LOG(LogMingGoRTS, Warning, TEXT("FactionRelationshipNetwork: Faction %s already registered"), *FactionID.ToString());
        return;
    }
    
    RegisteredFactions.Add(FactionID, Faction);
    
    // 创建网络节点
    FFactionNetworkNode Node;
    Node.FactionID = FactionID;
    Node.FactionRef = Faction;
    Node.DiplomaticPower = Faction->GetFactionAttributes().DiplomaticStrength;
    Node.InternationalReputation = Faction->GetFactionAttributes().StartingPosition.InitialReputation;
    
    NetworkNodes.Add(FactionID, Node);
    
    UE_LOG(LogMingGoRTS, Log, TEXT("FactionRelationshipNetwork: Registered faction %s"), *FactionID.ToString());
}

void UMingRTSFactionRelationshipNetwork::UnregisterFaction(FName FactionID)
{
    if (!RegisteredFactions.Contains(FactionID))
    {
        return;
    }
    
    RegisteredFactions.Remove(FactionID);
    
    // 从所有其他势力的关系中移除此势力
    for (auto& NodePair : NetworkNodes)
    {
        if (NodePair.Key != FactionID)
        {
            NodePair.Value.Relations.Remove(FactionID);
        }
    }
    
    NetworkNodes.Remove(FactionID);
    
    UE_LOG(LogMingGoRTS, Log, TEXT("FactionRelationshipNetwork: Unregistered faction %s"), *FactionID.ToString());
}

FFactionRelation UMingRTSFactionRelationshipNetwork::GetFactionRelation(FName FactionA, FName FactionB)
{
    if (!NetworkNodes.Contains(FactionA) || !NetworkNodes.Contains(FactionB))
    {
        return FFactionRelation();
    }
    
    FFactionNetworkNode& NodeA = NetworkNodes[FactionA];
    
    if (NodeA.Relations.Contains(FactionB))
    {
        return NodeA.Relations[FactionB];
    }
    
    // 返回默认中立关系
    FFactionRelation DefaultRelation;
    DefaultRelation.TargetFactionID = FactionB;
    return DefaultRelation;
}

void UMingRTSFactionRelationshipNetwork::SetFactionRelation(FName FactionA, FName FactionB, EFactionRelationType RelationType, int32 Strength)
{
    if (!NetworkNodes.Contains(FactionA) || !NetworkNodes.Contains(FactionB))
    {
        return;
    }
    
    // 确保关系是双向的
    FFactionRelation RelationAtoB;
    RelationAtoB.TargetFactionID = FactionB;
    RelationAtoB.RelationType = RelationType;
    RelationAtoB.RelationStrength = FMath::Clamp(Strength, -100, 100);
    RelationAtoB.LastInteractionTime = FDateTime::Now();
    RelationAtoB.bIsActive = true;
    
    NetworkNodes[FactionA].Relations.Add(FactionB, RelationAtoB);
    
    // 反向关系
    FFactionRelation RelationBtoA;
    RelationBtoA.TargetFactionID = FactionA;
    RelationBtoA.RelationType = RelationType;
    RelationBtoA.RelationStrength = FMath::Clamp(Strength, -100, 100);
    RelationBtoA.LastInteractionTime = FDateTime::Now();
    RelationBtoA.bIsActive = true;
    
    NetworkNodes[FactionB].Relations.Add(FactionA, RelationBtoA);
    
    // 触发事件
    OnRelationChanged.Broadcast(FactionA, FactionB, RelationAtoB);
    
    // 特殊关系类型的事件
    if (RelationType == EFactionRelationType::War)
    {
        OnWarDeclared.Broadcast(FactionA, FactionB);
    }
    else if (RelationType == EFactionRelationType::Alliance)
    {
        OnAllianceFormed.Broadcast(FactionA, FactionB);
    }
    
    // 通知第三方
    NotifyThirdParties(FactionA, FactionB, EFactionRelationType::Neutral, RelationType);
}

void UMingRTSFactionRelationshipNetwork::ModifyRelationStrength(FName FactionA, FName FactionB, int32 Delta)
{
    if (!NetworkNodes.Contains(FactionA) || !NetworkNodes.Contains(FactionB))
    {
        return;
    }
    
    FFactionRelation& RelationAtoB = NetworkNodes[FactionA].Relations.FindOrAdd(FactionB);
    RelationAtoB.RelationStrength = FMath::Clamp(RelationAtoB.RelationStrength + Delta, -100, 100);
    RelationAtoB.LastInteractionTime = FDateTime::Now();
    RelationAtoB.RelationHistory.Add(FString::Printf(TEXT("Strength modified by %d"), Delta));
    
    FFactionRelation& RelationBtoA = NetworkNodes[FactionB].Relations.FindOrAdd(FactionA);
    RelationBtoA.RelationStrength = FMath::Clamp(RelationBtoA.RelationStrength + Delta, -100, 100);
    RelationBtoA.LastInteractionTime = FDateTime::Now();
    
    // 更新关系类型基于强度
    if (RelationAtoB.RelationStrength >= 80)
        RelationAtoB.RelationType = EFactionRelationType::Alliance;
    else if (RelationAtoB.RelationStrength >= 40)
        RelationAtoB.RelationType = EFactionRelationType::Friendly;
    else if (RelationAtoB.RelationStrength >= -20)
        RelationAtoB.RelationType = EFactionRelationType::Neutral;
    else if (RelationAtoB.RelationStrength >= -50)
        RelationAtoB.RelationType = EFactionRelationType::Unfriendly;
    else if (RelationAtoB.RelationStrength >= -80)
        RelationAtoB.RelationType = EFactionRelationType::Hostile;
    else
        RelationAtoB.RelationType = EFactionRelationType::War;
    
    RelationBtoA.RelationType = RelationAtoB.RelationType;
    
    OnRelationChanged.Broadcast(FactionA, FactionB, RelationAtoB);
}

void UMingRTSFactionRelationshipNetwork::ModifyTrustLevel(FName FactionA, FName FactionB, int32 Delta)
{
    if (!NetworkNodes.Contains(FactionA) || !NetworkNodes.Contains(FactionB))
    {
        return;
    }
    
    FFactionRelation& RelationAtoB = NetworkNodes[FactionA].Relations.FindOrAdd(FactionB);
    RelationAtoB.TrustLevel = FMath::Clamp(RelationAtoB.TrustLevel + Delta, 0, 100);
    
    FFactionRelation& RelationBtoA = NetworkNodes[FactionB].Relations.FindOrAdd(FactionA);
    RelationBtoA.TrustLevel = FMath::Clamp(RelationBtoA.TrustLevel + Delta, 0, 100);
}

FDiplomaticActionResult UMingRTSFactionRelationshipNetwork::ExecuteDiplomaticAction(
    FName SourceFaction, FName TargetFaction, EDiplomaticAction Action, int32 Intensity)
{
    FDiplomaticActionResult Result;
    
    if (!CanExecuteDiplomaticAction(SourceFaction, TargetFaction, Action))
    {
        Result.bSuccess = false;
        Result.Message = FText::FromString(TEXT("行动条件不满足"));
        return Result;
    }
    
    float SuccessChance = CalculateActionSuccessChance(SourceFaction, TargetFaction, Action);
    bool bSuccess = FMath::FRand() <= SuccessChance;
    
    Result.bSuccess = bSuccess;
    
    int32 RelationChange = 0;
    int32 TrustChange = 0;
    
    switch (Action)
    {
    case EDiplomaticAction::ProposeAlliance:
        RelationChange = bSuccess ? 10 : -5;
        TrustChange = bSuccess ? 5 : -3;
        if (bSuccess)
        {
            SetFactionRelation(SourceFaction, TargetFaction, EFactionRelationType::Alliance, 80);
            Result.Message = FText::FromString(TEXT("同盟提议被接受"));
        }
        else
        {
            Result.Message = FText::FromString(TEXT("同盟提议被拒绝"));
        }
        break;
        
    case EDiplomaticAction::DeclareWar:
        SetFactionRelation(SourceFaction, TargetFaction, EFactionRelationType::War, -90);
        RelationChange = -40;
        TrustChange = -20;
        Result.Message = FText::FromString(TEXT("已宣战"));
        Result.bSuccess = true;
        break;
        
    case EDiplomaticAction::OfferPeace:
        RelationChange = bSuccess ? 20 : -10;
        TrustChange = bSuccess ? 3 : -2;
        if (bSuccess)
        {
            SetFactionRelation(SourceFaction, TargetFaction, EFactionRelationType::Neutral, 0);
            OnPeaceMade.Broadcast(SourceFaction, TargetFaction);
            Result.Message = FText::FromString(TEXT("和平协议已签署"));
        }
        else
        {
            Result.Message = FText::FromString(TEXT("和平提议被拒绝"));
        }
        break;
        
    case EDiplomaticAction::SendGift:
        RelationChange = FMath::Clamp(Intensity / 5, 5, 20);
        TrustChange = FMath::Clamp(Intensity / 10, 2, 10);
        Result.Message = FText::FromString(TEXT("礼物已送达，关系改善"));
        Result.bSuccess = true;
        break;
        
    case EDiplomaticAction::Insult:
        RelationChange = -15;
        TrustChange = -8;
        Result.Message = FText::FromString(TEXT("侮辱导致关系恶化"));
        Result.bSuccess = true;
        break;
        
    case EDiplomaticAction::ImproveRelations:
        RelationChange = FMath::Clamp(Intensity / 4, 3, 15);
        TrustChange = FMath::Clamp(Intensity / 8, 1, 5);
        Result.Message = FText::FromString(TEXT("关系改善努力取得成效"));
        Result.bSuccess = true;
        break;
        
    default:
        Result.Message = FText::FromString(TEXT("外交行动已执行"));
        break;
    }
    
    Result.RelationChange = RelationChange;
    Result.TrustChange = TrustChange;
    
    if (RelationChange != 0)
    {
        ModifyRelationStrength(SourceFaction, TargetFaction, RelationChange);
    }
    
    if (TrustChange != 0)
    {
        ModifyTrustLevel(SourceFaction, TargetFaction, TrustChange);
    }
    
    // 记录互动
    RecordDiplomaticInteraction(SourceFaction, TargetFaction, Action, bSuccess);
    
    // 触发事件
    OnDiplomaticActionExecuted.Broadcast(SourceFaction, TargetFaction, Action);
    
    return Result;
}

bool UMingRTSFactionRelationshipNetwork::CanExecuteDiplomaticAction(FName SourceFaction, FName TargetFaction, EDiplomaticAction Action)
{
    if (!NetworkNodes.Contains(SourceFaction) || !NetworkNodes.Contains(TargetFaction))
    {
        return false;
    }
    
    if (SourceFaction == TargetFaction)
    {
        return false;
    }
    
    FFactionRelation Relation = GetFactionRelation(SourceFaction, TargetFaction);
    
    switch (Action)
    {
    case EDiplomaticAction::ProposeAlliance:
        return Relation.RelationType == EFactionRelationType::Friendly && Relation.TrustLevel >= 60;
        
    case EDiplomaticAction::DeclareWar:
        return Relation.RelationType != EFactionRelationType::Alliance && 
               Relation.RelationType != EFactionRelationType::War;
        
    case EDiplomaticAction::OfferPeace:
        return Relation.RelationType == EFactionRelationType::War;
        
    case EDiplomaticAction::BreakAlliance:
        return Relation.RelationType == EFactionRelationType::Alliance;
        
    default:
        return true;
    }
}

TArray<FName> UMingRTSFactionRelationshipNetwork::GetAllies(FName FactionID)
{
    TArray<FName> Allies;
    
    if (!NetworkNodes.Contains(FactionID))
    {
        return Allies;
    }
    
    for (const auto& RelationPair : NetworkNodes[FactionID].Relations)
    {
        if (RelationPair.Value.RelationType == EFactionRelationType::Alliance && RelationPair.Value.bIsActive)
        {
            Allies.Add(RelationPair.Key);
        }
    }
    
    return Allies;
}

TArray<FName> UMingRTSFactionRelationshipNetwork::GetEnemies(FName FactionID)
{
    TArray<FName> Enemies;
    
    if (!NetworkNodes.Contains(FactionID))
    {
        return Enemies;
    }
    
    for (const auto& RelationPair : NetworkNodes[FactionID].Relations)
    {
        if ((RelationPair.Value.RelationType == EFactionRelationType::War || 
             RelationPair.Value.RelationType == EFactionRelationType::Hostile) && 
            RelationPair.Value.bIsActive)
        {
            Enemies.Add(RelationPair.Key);
        }
    }
    
    return Enemies;
}

TArray<FName> UMingRTSFactionRelationshipNetwork::GetTradePartners(FName FactionID)
{
    TArray<FName> Partners;
    
    if (!NetworkNodes.Contains(FactionID))
    {
        return Partners;
    }
    
    for (const auto& RelationPair : NetworkNodes[FactionID].Relations)
    {
        if (RelationPair.Value.RelationType == EFactionRelationType::TradePartner ||
            (RelationPair.Value.RelationStrength >= 20 && RelationPair.Value.bIsActive))
        {
            Partners.Add(RelationPair.Key);
        }
    }
    
    return Partners;
}

TArray<FName> UMingRTSFactionRelationshipNetwork::FindDiplomaticPath(FName StartFaction, FName EndFaction)
{
    TArray<FName> Path;
    
    if (StartFaction == EndFaction)
    {
        Path.Add(StartFaction);
        return Path;
    }
    
    if (!NetworkNodes.Contains(StartFaction) || !NetworkNodes.Contains(EndFaction))
    {
        return Path;
    }
    
    // 简单的BFS路径查找
    TQueue<FName> Queue;
    TMap<FName, FName> CameFrom;
    TSet<FName> Visited;
    
    Queue.Enqueue(StartFaction);
    Visited.Add(StartFaction);
    
    while (!Queue.IsEmpty())
    {
        FName Current;
        Queue.Dequeue(Current);
        
        if (Current == EndFaction)
        {
            // 重建路径
            FName Step = EndFaction;
            while (Step != StartFaction)
            {
                Path.Insert(Step, 0);
                Step = CameFrom[Step];
            }
            Path.Insert(StartFaction, 0);
            return Path;
        }
        
        for (const auto& RelationPair : NetworkNodes[Current].Relations)
        {
            FName Neighbor = RelationPair.Key;
            
            // 只通过友好或中立关系
            if (!Visited.Contains(Neighbor) && 
                RelationPair.Value.RelationStrength >= 0)
            {
                Queue.Enqueue(Neighbor);
                Visited.Add(Neighbor);
                CameFrom.Add(Neighbor, Current);
            }
        }
    }
    
    return Path;
}

int32 UMingRTSFactionRelationshipNetwork::CalculateIsolationLevel(FName FactionID)
{
    if (!NetworkNodes.Contains(FactionID))
    {
        return 100;
    }
    
    int32 ActiveRelations = 0;
    int32 PositiveRelations = 0;
    
    for (const auto& RelationPair : NetworkNodes[FactionID].Relations)
    {
        if (RelationPair.Value.bIsActive)
        {
            ActiveRelations++;
            if (RelationPair.Value.RelationStrength > 0)
            {
                PositiveRelations++;
            }
        }
    }
    
    int32 TotalPossibleRelations = RegisteredFactions.Num() - 1;
    if (TotalPossibleRelations == 0)
    {
        return 0;
    }
    
    // 孤立度 = 100 - (积极关系占比 * 100)
    float Isolation = 100.0f - ((float)PositiveRelations / (float)TotalPossibleRelations * 100.0f);
    
    return FMath::Clamp((int32)Isolation, 0, 100);
}

int32 UMingRTSFactionRelationshipNetwork::CalculateDiplomaticInfluenceScore(FName FactionID)
{
    if (!NetworkNodes.Contains(FactionID))
    {
        return 0;
    }
    
    FFactionNetworkNode& Node = NetworkNodes[FactionID];
    int32 Score = Node.DiplomaticPower;
    
    // 盟友加分
    int32 AllyCount = GetAllies(FactionID).Num();
    Score += AllyCount * 10;
    
    // 贸易伙伴加分
    int32 TradeCount = GetTradePartners(FactionID).Num();
    Score += TradeCount * 5;
    
    // 国际声望加分
    Score += Node.InternationalReputation / 2;
    
    // 敌对关系减分
    int32 EnemyCount = GetEnemies(FactionID).Num();
    Score -= EnemyCount * 8;
    
    return FMath::Max(0, Score);
}

TMap<FName, TMap<FName, int32>> UMingRTSFactionRelationshipNetwork::GetRelationMatrix()
{
    TMap<FName, TMap<FName, int32>> Matrix;
    
    for (const auto& NodePair : NetworkNodes)
    {
        TMap<FName, int32> Row;
        for (const auto& OtherNodePair : NetworkNodes)
        {
            if (NodePair.Key != OtherNodePair.Key)
            {
                FFactionRelation Relation = GetFactionRelation(NodePair.Key, OtherNodePair.Key);
                Row.Add(OtherNodePair.Key, Relation.RelationStrength);
            }
        }
        Matrix.Add(NodePair.Key, Row);
    }
    
    return Matrix;
}

TArray<FName> UMingRTSFactionRelationshipNetwork::FindCommonEnemies(FName FactionA, FName FactionB)
{
    TArray<FName> EnemiesA = GetEnemies(FactionA);
    TArray<FName> EnemiesB = GetEnemies(FactionB);
    
    TArray<FName> Common;
    for (FName Enemy : EnemiesA)
    {
        if (EnemiesB.Contains(Enemy))
        {
            Common.Add(Enemy);
        }
    }
    
    return Common;
}

TArray<FName> UMingRTSFactionRelationshipNetwork::FindCommonAllies(FName FactionA, FName FactionB)
{
    TArray<FName> AlliesA = GetAllies(FactionA);
    TArray<FName> AlliesB = GetAllies(FactionB);
    
    TArray<FName> Common;
    for (FName Ally : AlliesA)
    {
        if (AlliesB.Contains(Ally))
        {
            Common.Add(Ally);
        }
    }
    
    return Common;
}

float UMingRTSFactionRelationshipNetwork::CalculateNetworkStability()
{
    if (NetworkNodes.Num() < 2)
    {
        return 100.0f;
    }
    
    int32 TotalRelations = 0;
    int32 StableRelations = 0;
    
    for (const auto& NodePair : NetworkNodes)
    {
        for (const auto& RelationPair : NodePair.Value.Relations)
        {
            TotalRelations++;
            
            // 稳定关系：中立到友好
            if (RelationPair.Value.RelationStrength >= -20 && 
                RelationPair.Value.RelationStrength <= 80)
            {
                StableRelations++;
            }
        }
    }
    
    if (TotalRelations == 0)
    {
        return 100.0f;
    }
    
    return (float)StableRelations / (float)TotalRelations * 100.0f;
}

UMingRTSFactionBase* UMingRTSFactionRelationshipNetwork::GetFactionNode(FName FactionID) const
{
    if (RegisteredFactions.Contains(FactionID))
    {
        return RegisteredFactions[FactionID];
    }
    return nullptr;
}

TMap<FName, TArray<FFactionRelation>> UMingRTSFactionRelationshipNetwork::GetAllRelationsForFaction(FName FactionID)
{
    TMap<FName, TArray<FFactionRelation>> Result;
    
    if (!NetworkNodes.Contains(FactionID))
    {
        return Result;
    }
    
    TArray<FFactionRelation> Relations;
    for (const auto& RelationPair : NetworkNodes[FactionID].Relations)
    {
        Relations.Add(RelationPair.Value);
    }
    
    Result.Add(FactionID, Relations);
    return Result;
}

void UMingRTSFactionRelationshipNetwork::RecordDiplomaticInteraction(FName SourceFaction, FName TargetFaction, EDiplomaticAction Action, bool bSuccess)
{
    if (!NetworkNodes.Contains(SourceFaction) || !NetworkNodes.Contains(TargetFaction))
    {
        return;
    }
    
    FString HistoryEntry = FString::Printf(TEXT("[%s] %s -> %s: %s (%s)"),
        *FDateTime::Now().ToString(),
        *SourceFaction.ToString(),
        *TargetFaction.ToString(),
        *StaticEnum<EDiplomaticAction>()->GetNameStringByValue((int64)Action),
        bSuccess ? TEXT("Success") : TEXT("Failed"));
    
    if (NetworkNodes[SourceFaction].Relations.Contains(TargetFaction))
    {
        NetworkNodes[SourceFaction].Relations[TargetFaction].RelationHistory.Add(HistoryEntry);
    }
}

void UMingRTSFactionRelationshipNetwork::ProcessRelationDecay(float DeltaTime)
{
    float DecayThisTick = RelationDecayRate * DeltaTime;
    
    for (auto& NodePair : NetworkNodes)
    {
        for (auto& RelationPair : NodePair.Value.Relations)
        {
            // 只有非战争和同盟关系会衰减
            if (RelationPair.Value.RelationType != EFactionRelationType::War &&
                RelationPair.Value.RelationType != EFactionRelationType::Alliance)
            {
                // 向中立(0)衰减
                if (RelationPair.Value.RelationStrength > 0)
                {
                    RelationPair.Value.RelationStrength = FMath::Max(0, RelationPair.Value.RelationStrength - (int32)DecayThisTick);
                }
                else if (RelationPair.Value.RelationStrength < 0)
                {
                    RelationPair.Value.RelationStrength = FMath::Min(0, RelationPair.Value.RelationStrength + (int32)DecayThisTick);
                }
            }
        }
    }
}

void UMingRTSFactionRelationshipNetwork::ApplyHistoricalRelations()
{
    // 应用民国时期的预设历史关系
    // 直系与皖系敌对
    SetFactionRelation(TEXT("ZhiliClique"), TEXT("AnhuiClique"), EFactionRelationType::Hostile, -60);
    
    // 直系与奉系敌对
    SetFactionRelation(TEXT("ZhiliClique"), TEXT("FengtianClique"), EFactionRelationType::Hostile, -70);
    
    // 皖系与奉系中立偏友好
    SetFactionRelation(TEXT("AnhuiClique"), TEXT("FengtianClique"), EFactionRelationType::Neutral, -10);
    
    // 各派系与国民政府的关系
    SetFactionRelation(TEXT("GuangxiClique"), TEXT("YunnanClique"), EFactionRelationType::Friendly, 30);
    SetFactionRelation(TEXT("SichuanClique"), TEXT("XinjiangForces"), EFactionRelationType::Neutral, 0);
    SetFactionRelation(TEXT("ShanxiClique"), TEXT("MaFamilyForces"), EFactionRelationType::Neutral, 5);
    
    UE_LOG(LogMingGoRTS, Log, TEXT("FactionRelationshipNetwork: Applied historical relations"));
}

FText UMingRTSFactionRelationshipNetwork::GetRelationDescription(FName FactionA, FName FactionB)
{
    FFactionRelation Relation = GetFactionRelation(FactionA, FactionB);
    
    FString TypeStr;
    switch (Relation.RelationType)
    {
    case EFactionRelationType::Alliance: TypeStr = TEXT("同盟"); break;
    case EFactionRelationType::Friendly: TypeStr = TEXT("友好"); break;
    case EFactionRelationType::Neutral: TypeStr = TEXT("中立"); break;
    case EFactionRelationType::Unfriendly: TypeStr = TEXT("不友好"); break;
    case EFactionRelationType::Hostile: TypeStr = TEXT("敌对"); break;
    case EFactionRelationType::War: TypeStr = TEXT("战争"); break;
    case EFactionRelationType::Vassal: TypeStr = TEXT("附庸"); break;
    case EFactionRelationType::Overlord: TypeStr = TEXT("宗主"); break;
    case EFactionRelationType::TradePartner: TypeStr = TEXT("贸易伙伴"); break;
    case EFactionRelationType::NonAggression: TypeStr = TEXT("互不侵犯"); break;
    default: TypeStr = TEXT("未知"); break;
    }
    
    return FText::Format(FText::FromString(TEXT("{0} (强度: {1}, 信任: {2})"))),
        FText::FromString(TypeStr),
        FText::AsNumber(Relation.RelationStrength),
        FText::AsNumber(Relation.TrustLevel));
}

int32 UMingRTSFactionRelationshipNetwork::GetDefaultRelationStrength(EFactionRelationType RelationType) const
{
    switch (RelationType)
    {
    case EFactionRelationType::Alliance: return 80;
    case EFactionRelationType::Friendly: return 50;
    case EFactionRelationType::Neutral: return 0;
    case EFactionRelationType::Unfriendly: return -30;
    case EFactionRelationType::Hostile: return -60;
    case EFactionRelationType::War: return -90;
    case EFactionRelationType::Vassal: return 60;
    case EFactionRelationType::Overlord: return 60;
    case EFactionRelationType::TradePartner: return 40;
    case EFactionRelationType::NonAggression: return 20;
    default: return 0;
    }
}

float UMingRTSFactionRelationshipNetwork::CalculateActionSuccessChance(FName SourceFaction, FName TargetFaction, EDiplomaticAction Action) const
{
    if (!NetworkNodes.Contains(SourceFaction) || !NetworkNodes.Contains(TargetFaction))
    {
        return 0.0f;
    }
    
    FFactionRelation Relation = const_cast<UMingRTSFactionRelationshipNetwork*>(this)->GetFactionRelation(SourceFaction, TargetFaction);
    
    float BaseChance = 0.5f;
    
    // 关系强度影响
    BaseChance += Relation.RelationStrength / 200.0f; // -0.5 到 +0.5
    
    // 信任度影响
    BaseChance += (Relation.TrustLevel - 50) / 200.0f; // -0.25 到 +0.25
    
    // 特定行动修正
    switch (Action)
    {
    case EDiplomaticAction::ProposeAlliance:
        BaseChance += (Relation.RelationType == EFactionRelationType::Friendly) ? 0.2f : -0.2f;
        break;
    case EDiplomaticAction::SendGift:
        BaseChance += 0.1f;
        break;
    case EDiplomaticAction::Insult:
        BaseChance -= 0.3f; // 侮辱通常产生负面效果
        break;
    case EDiplomaticAction::OfferPeace:
        if (Relation.RelationType == EFactionRelationType::War)
        {
            // 战争越久越可能接受和平
            FTimespan WarDuration = FDateTime::Now() - Relation.LastInteractionTime;
            BaseChance += FMath::Min(WarDuration.GetDays() / 30.0f, 0.3f);
        }
        break;
    default:
        break;
    }
    
    return FMath::Clamp(BaseChance, 0.05f, 0.95f);
}

void UMingRTSFactionRelationshipNetwork::ApplyRelationEffects(FName FactionA, FName FactionB)
{
    // 应用关系变化对势力属性的影响
    // 例如：同盟关系可能提供资源加成
}

void UMingRTSFactionRelationshipNetwork::NotifyThirdParties(FName FactionA, FName FactionB, EFactionRelationType OldRelation, EFactionRelationType NewRelation)
{
    // 当两个势力关系发生重大变化时，通知第三方
    // 例如：如果A和B结盟，A的敌人可能会对B产生负面看法
    
    if (NewRelation == EFactionRelationType::Alliance)
    {
        TArray<FName> EnemiesA = GetEnemies(FactionA);
        for (FName Enemy : EnemiesA)
        {
            if (Enemy != FactionB)
            {
                // A的敌人对B的关系略微下降
                ModifyRelationStrength(Enemy, FactionB, -5);
            }
        }
        
        TArray<FName> EnemiesB = GetEnemies(FactionB);
        for (FName Enemy : EnemiesB)
        {
            if (Enemy != FactionA)
            {
                // B的敌人对A的关系略微下降
                ModifyRelationStrength(Enemy, FactionA, -5);
            }
        }
    }
    else if (NewRelation == EFactionRelationType::War)
    {
        // 战争爆发，盟友可能会被卷入
        TArray<FName> AlliesA = GetAllies(FactionA);
        for (FName Ally : AlliesA)
        {
            // 盟友对敌人的关系恶化
            ModifyRelationStrength(Ally, FactionB, -10);
        }
    }
}
