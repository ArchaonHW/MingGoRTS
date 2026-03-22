#include "Innovation/MingRTSSocialDynamics.h"
#include "Math/UnrealMathUtility.h"

UMingRTSSocialDynamics::UMingRTSSocialDynamics()
    : NextAgentId(1)
{
}

void UMingRTSSocialDynamics::InitializeSocialSystem()
{
    SocialAgents.Empty();
    NextAgentId = 1;
}

int32 UMingRTSSocialDynamics::CreateSocialAgent(const FString& Name, ESocialClass SocialClass)
{
    int32 NewId = NextAgentId++;
    FSocialAgent NewAgent;
    NewAgent.AgentId = NewId;
    NewAgent.AgentName = Name;
    NewAgent.SocialClass = SocialClass;

    // 基於社會階層設置初始屬性
    switch (SocialClass)
    {
    case ESocialClass::Peasant:
        NewAgent.Wealth = FMath::RandRange(10.0f, 50.0f);
        NewAgent.Influence = FMath::RandRange(1.0f, 10.0f);
        NewAgent.Reputation = FMath::RandRange(30.0f, 70.0f);
        break;
    case ESocialClass::Artisan:
        NewAgent.Wealth = FMath::RandRange(30.0f, 80.0f);
        NewAgent.Influence = FMath::RandRange(5.0f, 20.0f);
        NewAgent.Reputation = FMath::RandRange(40.0f, 80.0f);
        break;
    case ESocialClass::Merchant:
        NewAgent.Wealth = FMath::RandRange(50.0f, 150.0f);
        NewAgent.Influence = FMath::RandRange(15.0f, 40.0f);
        NewAgent.Reputation = FMath::RandRange(20.0f, 90.0f);
        break;
    case ESocialClass::Scholar:
        NewAgent.Wealth = FMath::RandRange(20.0f, 100.0f);
        NewAgent.Influence = FMath::RandRange(10.0f, 50.0f);
        NewAgent.Reputation = FMath::RandRange(50.0f, 95.0f);
        break;
    case ESocialClass::Official:
        NewAgent.Wealth = FMath::RandRange(100.0f, 300.0f);
        NewAgent.Influence = FMath::RandRange(50.0f, 100.0f);
        NewAgent.Reputation = FMath::RandRange(40.0f, 90.0f);
        break;
    case ESocialClass::Noble:
        NewAgent.Wealth = FMath::RandRange(200.0f, 1000.0f);
        NewAgent.Influence = FMath::RandRange(80.0f, 150.0f);
        NewAgent.Reputation = FMath::RandRange(60.0f, 100.0f);
        break;
    case ESocialClass::Military:
        NewAgent.Wealth = FMath::RandRange(30.0f, 120.0f);
        NewAgent.Influence = FMath::RandRange(20.0f, 60.0f);
        NewAgent.Reputation = FMath::RandRange(35.0f, 85.0f);
        break;
    case ESocialClass::Religious:
        NewAgent.Wealth = FMath::RandRange(10.0f, 80.0f);
        NewAgent.Influence = FMath::RandRange(25.0f, 70.0f);
        NewAgent.Reputation = FMath::RandRange(60.0f, 100.0f);
        break;
    case ESocialClass::Outlaw:
        NewAgent.Wealth = FMath::RandRange(5.0f, 40.0f);
        NewAgent.Influence = FMath::RandRange(5.0f, 30.0f);
        NewAgent.Reputation = FMath::RandRange(0.0f, 30.0f);
        break;
    default:
        break;
    }

    SocialAgents.Add(NewId, NewAgent);
    return NewId;
}

void UMingRTSSocialDynamics::EstablishRelation(int32 AgentA, int32 AgentB, ESocialRelationType RelationType, float Strength)
{
    FSocialAgent* A = SocialAgents.Find(AgentA);
    FSocialAgent* B = SocialAgents.Find(AgentB);

    if (A && B)
    {
        A->SocialRelations.Add(AgentB, RelationType);
        A->RelationStrength.Add(AgentB, FMath::Clamp(Strength, 0.0f, 1.0f));

        // 雙向關係
        B->SocialRelations.Add(AgentA, RelationType);
        B->RelationStrength.Add(AgentA, FMath::Clamp(Strength, 0.0f, 1.0f));
    }
}

void UMingRTSSocialDynamics::SimulateSocialTick(float DeltaTime)
{
    UpdateSocialMobility();
    SpreadReputation();
    ResolveSocialConflicts();
}

FSocietyMetrics UMingRTSSocialDynamics::GetSocietyMetrics() const
{
    FSocietyMetrics Metrics;
    int32 TotalAgents = SocialAgents.Num();

    if (TotalAgents == 0)
    {
        return Metrics;
    }

    // 計算階層分布
    for (const auto& Pair : SocialAgents)
    {
        if (Metrics.ClassDistribution.Contains(Pair.Value.SocialClass))
        {
            Metrics.ClassDistribution[Pair.Value.SocialClass] += 1.0f;
        }
        else
        {
            Metrics.ClassDistribution.Add(Pair.Value.SocialClass, 1.0f);
        }
    }

    // 標準化分布
    for (auto& Pair : Metrics.ClassDistribution)
    {
        Pair.Value /= TotalAgents;
    }

    // 計算平均幸福度
    float TotalHappiness = 0.0f;
    for (const auto& Pair : SocialAgents)
    {
        // 基於財富、聲望、影響力計算幸福度
        float Happiness = (Pair.Value.Wealth / 500.0f + Pair.Value.Reputation / 100.0f + Pair.Value.Influence / 150.0f) / 3.0f;
        TotalHappiness += FMath::Clamp(Happiness, 0.0f, 1.0f);
    }
    Metrics.AverageHappiness = TotalHappiness / TotalAgents;

    // 計算經濟不平等
    float TotalWealth = 0.0f;
    float MaxWealth = 0.0f;
    for (const auto& Pair : SocialAgents)
    {
        TotalWealth += Pair.Value.Wealth;
        MaxWealth = FMath::Max(MaxWealth, Pair.Value.Wealth);
    }
    float AverageWealth = TotalWealth / TotalAgents;
    float WealthVariance = 0.0f;
    for (const auto& Pair : SocialAgents)
    {
        WealthVariance += FMath::Square(Pair.Value.Wealth - AverageWealth);
    }
    Metrics.EconomicInequality = FMath::Clamp(WealthVariance / FMath::Max(TotalWealth * TotalWealth, 1.0f), 0.0f, 1.0f);

    // 社會穩定性基於不平等和幸福度
    Metrics.SocialStability = (1.0f - Metrics.EconomicInequality) * Metrics.AverageHappiness;

    // 社會流動性（簡化計算）
    Metrics.SocialMobility = 0.5f;

    // 文化凝聚力基於關係密度
    int32 TotalRelations = 0;
    for (const auto& Pair : SocialAgents)
    {
        TotalRelations += Pair.Value.SocialRelations.Num();
    }
    int32 MaxPossibleRelations = TotalAgents * (TotalAgents - 1);
    Metrics.CulturalCohesion = FMath::Clamp((float)TotalRelations / FMath::Max(MaxPossibleRelations, 1), 0.0f, 1.0f);

    return Metrics;
}

void UMingRTSSocialDynamics::UpdateSocialMobility()
{
    for (auto& Pair : SocialAgents)
    {
        FSocialAgent& Agent = Pair.Value;

        // 基於財富和聲望評估階層流動
        float MobilityChance = 0.01f;

        switch (Agent.SocialClass)
        {
        case ESocialClass::Peasant:
            if (Agent.Wealth > 80.0f && Agent.Reputation > 60.0f)
            {
                if (FMath::RandRange(0.0f, 1.0f) < MobilityChance)
                {
                    // 向上流動為工匠或商人
                    Agent.SocialClass = FMath::RandRange(0.0f, 1.0f) < 0.7f ? ESocialClass::Artisan : ESocialClass::Merchant;
                }
            }
            break;
        case ESocialClass::Artisan:
            if (Agent.Wealth > 120.0f && Agent.Reputation > 70.0f)
            {
                if (FMath::RandRange(0.0f, 1.0f) < MobilityChance * 0.5f)
                {
                    Agent.SocialClass = ESocialClass::Merchant;
                }
            }
            break;
        case ESocialClass::Merchant:
            if (Agent.Wealth > 250.0f && Agent.Reputation > 75.0f)
            {
                if (FMath::RandRange(0.0f, 1.0f) < MobilityChance * 0.3f)
                {
                    Agent.SocialClass = ESocialClass::Noble;
                }
            }
            break;
        default:
            break;
        }
    }
}

void UMingRTSSocialDynamics::SpreadReputation()
{
    // 聲望在社會關係網絡中傳播
    for (auto& Pair : SocialAgents)
    {
        FSocialAgent& Agent = Pair.Value;

        for (const auto& RelPair : Agent.SocialRelations)
        {
            int32 OtherId = RelPair.Key;
            ESocialRelationType Relation = RelPair.Value;

            if (FSocialAgent* Other = SocialAgents.Find(OtherId))
            {
                float Strength = Agent.RelationStrength.FindRef(OtherId);

                // 根據關係類型傳播聲望
                float ReputationImpact = 0.0f;
                switch (Relation)
                {
                case ESocialRelationType::Family:
                case ESocialRelationType::Friend:
                    ReputationImpact = Agent.Reputation * Strength * 0.01f;
                    break;
                case ESocialRelationType::Business:
                    ReputationImpact = (Agent.Reputation - 50.0f) * Strength * 0.005f;
                    break;
                case ESocialRelationType::Political:
                    ReputationImpact = (Agent.Reputation - 50.0f) * Strength * 0.008f;
                    break;
                case ESocialRelationType::Rival:
                case ESocialRelationType::Enemy:
                    ReputationImpact = (50.0f - Agent.Reputation) * Strength * 0.005f;
                    break;
                default:
                    break;
                }

                Other->Reputation = FMath::Clamp(Other->Reputation + ReputationImpact, 0.0f, 100.0f);
            }
        }
    }
}

void UMingRTSSocialDynamics::ResolveSocialConflicts()
{
    // 識別並處理社會衝突
    TArray<int32> AgentIds;
    SocialAgents.GetKeys(AgentIds);

    for (int32 i = 0; i < AgentIds.Num(); ++i)
    {
        for (int32 j = i + 1; j < AgentIds.Num(); ++j)
        {
            int32 A = AgentIds[i];
            int32 B = AgentIds[j];

            if (FSocialAgent* AgentA = SocialAgents.Find(A))
            {
                if (FSocialAgent* AgentB = SocialAgents.Find(B))
                {
                    // 檢查是否存在敵對關係
                    if (AgentA->SocialRelations.Contains(B))
                    {
                        ESocialRelationType Relation = AgentA->SocialRelations.FindRef(B);
                        if (Relation == ESocialRelationType::Enemy || Relation == ESocialRelationType::Rival)
                        {
                            // 衝突可能影響雙方的聲望和財富
                            float ConflictSeverity = AgentA->RelationStrength.FindRef(B);

                            AgentA->Reputation = FMath::Max(0.0f, AgentA->Reputation - ConflictSeverity * 2.0f);
                            AgentB->Reputation = FMath::Max(0.0f, AgentB->Reputation - ConflictSeverity * 2.0f);

                            // 經濟損失
                            AgentA->Wealth = FMath::Max(0.0f, AgentA->Wealth - ConflictSeverity * 5.0f);
                            AgentB->Wealth = FMath::Max(0.0f, AgentB->Wealth - ConflictSeverity * 5.0f);
                        }
                    }
                }
            }
        }
    }
}
