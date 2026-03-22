#include "Innovation/MingSocialDynamicsSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Misc/DateTime.h"
#include "Math/UnrealMathUtility.h"
#include "Algo/RandomShuffle.h"

UMingSocialDynamicsSystem::UMingSocialDynamicsSystem()
{
    SimulationSpeed = 1.0f;
    RelationshipDecayRate = 0.01f;
    CulturalTransmissionRate = 0.1f;
    SocialMobilityProbability = 0.05f;
    GroupBehaviorThreshold = 0.7f;
    MaxIndividuals = 1000;
    MaxRelationshipsPerIndividual = 50;
    bEnableCulturalEvolution = true;
    bEnableSocialMobility = true;
    bEnableGroupBehaviors = true;
    
    bIsInitialized = false;
    ActiveIndividualCount = 0;
    ActiveRelationshipCount = 0;
    AverageSocialInfluence = 1.0f;
    DominantSocialClass = ESocialClass::Middle;
    
    LastSimulationTime = 0.0f;
    TotalEventsProcessed = 0;
    TotalRelationshipsFormed = 0;
    TotalCulturalTransmissions = 0;
}

bool UMingSocialDynamicsSystem::InitializeSocialDynamicsSystem()
{
    if (bIsInitialized)
    {
        return true;
    }
    
    // 初始化統計數據
    SystemStats.Empty();
    SystemStats.Add(TEXT("ActiveIndividuals"), 0.0f);
    SystemStats.Add(TEXT("ActiveRelationships"), 0.0f);
    SystemStats.Add(TEXT("SocialEvents"), 0.0f);
    SystemStats.Add(TEXT("CulturalElements"), 0.0f);
    SystemStats.Add(TEXT("GroupBehaviors"), 0.0f);
    SystemStats.Add(TEXT("AverageInfluence"), 1.0f);
    SystemStats.Add(TEXT("NetworkDensity"), 0.0f);
    
    bIsInitialized = true;
    
    // 設置定時器
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            SimulationTimerHandle,
            this,
            &UMingSocialDynamicsSystem::UpdateRelationships,
            1.0f,
            true
        );
        
        World->GetTimerManager().SetTimer(
            RelationshipUpdateTimerHandle,
            this,
            &UMingSocialDynamicsSystem::ProcessSocialEvents,
            0.5f,
            true
        );
        
        World->GetTimerManager().SetTimer(
            EventProcessingTimerHandle,
            this,
            &UMingSocialDynamicsSystem::UpdateCulturalElements,
            2.0f,
            true
        );
        
        World->GetTimerManager().SetTimer(
            CulturalEvolutionTimerHandle,
            this,
            &UMingSocialDynamicsSystem::UpdateGroupBehaviors,
            1.5f,
            true
        );
        
        World->GetTimerManager().SetTimer(
            SocialMobilityTimerHandle,
            [this]()
            {
                if (bEnableSocialMobility)
                {
                    ProcessSocialMobility(1.0f);
                }
            },
            5.0f,
            true
        );
    }
    
    return true;
}

FString UMingSocialDynamicsSystem::CreateIndividual(const FString& FirstName, const FString& LastName, int32 Age, const FString& Gender, ESocialClass SocialClass)
{
    if (!bIsInitialized || Individuals.Num() >= MaxIndividuals)
    {
        return FString();
    }
    
    if (!ValidateIndividualCreation(FirstName, LastName, Age, Gender, SocialClass))
    {
        return FString();
    }
    
    FString IndividualID = GenerateUniqueIndividualID();
    FSocialIndividual Individual = CreateDefaultIndividual(IndividualID, FirstName, LastName, Age, Gender, SocialClass);
    
    Individuals.Add(IndividualID, Individual);
    ActiveIndividualCount++;
    
    // 更新統計
    SystemStats[TEXT("ActiveIndividuals")] = static_cast<float>(ActiveIndividualCount);
    
    // 觸發事件
    OnIndividualCreated.Broadcast(Individual);
    
    return IndividualID;
}

FString UMingSocialDynamicsSystem::EstablishRelationship(const FString& IndividualA_ID, const FString& IndividualB_ID, ESocialRelationType RelationType, float Strength)
{
    if (!bIsInitialized || !Individuals.Contains(IndividualA_ID) || !Individuals.Contains(IndividualB_ID))
    {
        return FString();
    }
    
    if (!ValidateRelationshipEstablishment(IndividualA_ID, IndividualB_ID, RelationType, Strength))
    {
        return FString();
    }
    
    FString RelationshipID = GenerateUniqueRelationshipID();
    FSocialRelationship Relationship = CreateDefaultRelationship(RelationshipID, IndividualA_ID, IndividualB_ID, RelationType, Strength);
    
    Relationships.Add(RelationshipID, Relationship);
    ActiveRelationshipCount++;
    TotalRelationshipsFormed++;
    
    // 更新個體關係
    Individuals[IndividualA_ID].Relationships.Add(IndividualB_ID, Strength);
    Individuals[IndividualB_ID].Relationships.Add(IndividualA_ID, Strength);
    
    // 更新統計
    SystemStats[TEXT("ActiveRelationships")] = static_cast<float>(ActiveRelationshipCount);
    
    // 觸發事件
    OnRelationshipFormed.Broadcast(Relationship);
    
    return RelationshipID;
}

FString UMingSocialDynamicsSystem::TriggerSocialEvent(ESocialEventType EventType, const TArray<FString>& Participants, const FString& Location, float Impact)
{
    if (!bIsInitialized)
    {
        return FString();
    }
    
    if (!ValidateSocialEvent(EventType, Participants, Location, Impact))
    {
        return FString();
    }
    
    FString EventID = GenerateUniqueEventID();
    FSocialEvent Event = CreateDefaultEvent(EventID, EventType, Participants, Location, Impact);
    
    SocialEvents.Add(Event);
    TotalEventsProcessed++;
    
    // 處理事件的社會後果
    for (const FString& ParticipantID : Participants)
    {
        if (Individuals.Contains(ParticipantID))
        {
            FSocialIndividual& Individual = Individuals[ParticipantID];
            
            // 根據事件類型調整個體屬性
            switch (EventType)
            {
            case ESocialEventType::Promotion:
                Individual.PoliticalPower += Impact * 0.1f;
                Individual.EconomicStatus += Impact * 0.05f;
                break;
            case ESocialEventType::Marriage:
                Individual.SocialInfluence += Impact * 0.2f;
                break;
            case ESocialEventType::Achievement:
                Individual.SocialInfluence += Impact * 0.15f;
                break;
            case ESocialEventType::Scandal:
                Individual.SocialInfluence -= Impact * 0.1f;
                break;
            }
        }
    }
    
    // 更新統計
    SystemStats[TEXT("SocialEvents")] = static_cast<float>(SocialEvents.Num());
    
    // 觸發事件
    OnSocialEventOccurred.Broadcast(Event);
    
    return EventID;
}

bool UMingSocialDynamicsSystem::SpreadCulturalElement(const FString& ElementID, const TArray<FString>& Targets, ECulturalTransmissionType TransmissionType)
{
    if (!bIsInitialized || !CulturalElements.Contains(ElementID))
    {
        return false;
    }
    
    FCulturalElement& Element = CulturalElements[ElementID];
    int32 SuccessfulTransmissions = 0;
    
    for (const FString& TargetID : Targets)
    {
        if (!Individuals.Contains(TargetID))
        {
            continue;
        }
        
        FSocialIndividual& Individual = Individuals[TargetID];
        
        // 計算傳播成功率
        float TransmissionProbability = CulturalTransmissionRate * Element.TransmissionStrength;
        
        // 根據傳播類型調整概率
        switch (TransmissionType)
        {
        case ECulturalTransmissionType::Vertical:
            // 垂直傳播：考慮年齡差異
            TransmissionProbability *= 0.8f;
            break;
        case ECulturalTransmissionType::Horizontal:
            // 水平傳播：考慮同輩關係
            TransmissionProbability *= 1.2f;
            break;
        case ECulturalTransmissionType::MassMedia:
            // 大眾媒體：高覆蓋率但低影響力
            TransmissionProbability *= 0.6f;
            break;
        case ECulturalTransmissionType::Education:
            // 教育系統：高成功率
            TransmissionProbability *= 1.5f;
            break;
        }
        
        // 執行傳播
        if (FMath::RandRange(0.0f, 1.0f) < TransmissionProbability)
        {
            // 添加文化元素到個體
            if (!Individual.Beliefs.Contains(Element.ElementName))
            {
                Individual.Beliefs.Add(Element.ElementName);
                Individual.Values.Append(Element.AssociatedValues);
                SuccessfulTransmissions++;
            }
        }
    }
    
    // 更新文化元素的傳播數據
    Element.AdoptionRate += static_cast<float>(SuccessfulTransmissions) / Targets.Num();
    Element.Carriers.Append(Targets);
    
    TotalCulturalTransmissions += SuccessfulTransmissions;
    
    // 觸發事件
    OnCulturalElementSpread.Broadcast(Element);
    
    return SuccessfulTransmissions > 0;
}

FString UMingSocialDynamicsSystem::SimulateGroupBehavior(const FString& GroupID, EGroupBehaviorType BehaviorType, const TArray<FString>& Participants)
{
    if (!bIsInitialized || Participants.Num() == 0)
    {
        return FString();
    }
    
    FString BehaviorID = GenerateUniqueBehaviorID();
    FGroupBehavior Behavior = CreateDefaultGroupBehavior(BehaviorID, GroupID, BehaviorType, Participants);
    
    // 計算群體行為強度
    float TotalInfluence = 0.0f;
    for (const FString& ParticipantID : Participants)
    {
        if (Individuals.Contains(ParticipantID))
        {
            TotalInfluence += Individuals[ParticipantID].SocialInfluence;
        }
    }
    
    Behavior.Intensity = TotalInfluence / Participants.Num();
    Behavior.bIsOngoing = true;
    
    // 根據行為類型產生效果
    switch (BehaviorType)
    {
    case EGroupBehaviorType::Cooperation:
        // 合作行為：增強參與者關係
        for (int32 i = 0; i < Participants.Num(); i++)
        {
            for (int32 j = i + 1; j < Participants.Num(); j++)
            {
                FString RelationshipID = EstablishRelationship(
                    Participants[i],
                    Participants[j],
                    ESocialRelationType::Friendship,
                    Behavior.Intensity * 0.1f
                );
            }
        }
        break;
        
    case EGroupBehaviorType::Conflict:
        // 衝突行為：降低參與者關係
        for (int32 i = 0; i < Participants.Num(); i++)
        {
            for (int32 j = i + 1; j < Participants.Num(); j++)
            {
                FString RelationshipID = EstablishRelationship(
                    Participants[i],
                    Participants[j],
                    ESocialRelationType::Rivalry,
                    Behavior.Intensity * 0.15f
                );
            }
        }
        break;
        
    case EGroupBehaviorType::Innovation:
        // 創新行為：傳播新文化元素
        if (CulturalElements.Num() > 0)
        {
            FCulturalElement& RandomElement = CulturalElements[FMath::RandRange(0, CulturalElements.Num() - 1)];
            SpreadCulturalElement(RandomElement.ElementID, Participants, ECulturalTransmissionType::Horizontal);
        }
        break;
    }
    
    GroupBehaviors.Add(Behavior);
    
    // 觸發事件
    OnGroupBehaviorStarted.Broadcast(Behavior);
    
    return BehaviorID;
}

bool UMingSocialDynamicsSystem::ProcessSocialMobility(const FString& IndividualID, ESocialClass TargetClass)
{
    if (!bIsInitialized || !Individuals.Contains(IndividualID))
    {
        return false;
    }
    
    FSocialIndividual& Individual = Individuals[IndividualID];
    
    // 計算流動概率
    float MobilityProbability = SocialMobilityProbability;
    
    // 根據當前階層和目標階層調整概率
    int32 CurrentClassIndex = static_cast<int32>(Individual.SocialClass);
    int32 TargetClassIndex = static_cast<int32>(TargetClass);
    
    if (TargetClassIndex > CurrentClassIndex)
    {
        // 向上流動：較低概率
        MobilityProbability *= 0.3f;
    }
    else if (TargetClassIndex < CurrentClassIndex)
    {
        // 向下流動：較高概率
        MobilityProbability *= 0.7f;
    }
    
    // 執行流動
    if (FMath::RandRange(0.0f, 1.0f) < MobilityProbability)
    {
        ESocialClass OldClass = Individual.SocialClass;
        Individual.SocialClass = TargetClass;
        
        // 調整相關屬性
        float ClassMultiplier = (TargetClassIndex - CurrentClassIndex) * 0.1f;
        Individual.EconomicStatus += ClassMultiplier;
        Individual.PoliticalPower += ClassMultiplier * 0.5f;
        
        // 觸發事件
        OnSocialClassChanged.Broadcast(IndividualID, TargetClass);
        
        return true;
    }
    
    return false;
}

float UMingSocialDynamicsSystem::CalculateSocialInfluence(const FString& IndividualID)
{
    if (!Individuals.Contains(IndividualID))
    {
        return 0.0f;
    }
    
    const FSocialIndividual& Individual = Individuals[IndividualID];
    
    // 基礎影響力
    float Influence = Individual.SocialInfluence;
    
    // 關係網絡影響
    int32 RelationshipCount = Individual.Relationships.Num();
    float NetworkEffect = FMath::Sqrt(static_cast<float>(RelationshipCount)) * 0.1f;
    
    // 社會階層影響
    float ClassEffect = static_cast<float>(static_cast<int32>(Individual.SocialClass)) * 0.2f;
    
    // 經濟狀況影響
    float EconomicEffect = Individual.EconomicStatus * 0.3f;
    
    // 政治權力影響
    float PoliticalEffect = Individual.PoliticalPower * 0.4f;
    
    return Influence + NetworkEffect + ClassEffect + EconomicEffect + PoliticalEffect;
}

bool UMingSocialDynamicsSystem::UpdateSocialNetwork(const FString& NetworkID, float DeltaTime)
{
    if (!SocialNetworks.Contains(NetworkID))
    {
        return false;
    }
    
    FSocialNetwork& Network = SocialNetworks[NetworkID];
    
    // 計算網絡密度
    int32 NodeCount = Network.Nodes.Num();
    int32 MaxPossibleEdges = NodeCount * (NodeCount - 1) / 2;
    Network.Density = static_cast<float>(Network.Edges.Num()) / MaxPossibleEdges;
    
    // 計算聚類係數
    float TotalClustering = 0.0f;
    for (const FString& NodeID : Network.Nodes)
    {
        TArray<FString> Neighbors;
        for (const FSocialRelationship& Edge : Network.Edges)
        {
            if (Edge.IndividualA_ID == NodeID)
            {
                Neighbors.Add(Edge.IndividualB_ID);
            }
            else if (Edge.IndividualB_ID == NodeID)
            {
                Neighbors.Add(Edge.IndividualA_ID);
            }
        }
        
        if (Neighbors.Num() < 2)
        {
            continue;
        }
        
        int32 NeighborEdges = 0;
        for (int32 i = 0; i < Neighbors.Num(); i++)
        {
            for (int32 j = i + 1; j < Neighbors.Num(); j++)
            {
                for (const FSocialRelationship& Edge : Network.Edges)
                {
                    if ((Edge.IndividualA_ID == Neighbors[i] && Edge.IndividualB_ID == Neighbors[j]) ||
                        (Edge.IndividualA_ID == Neighbors[j] && Edge.IndividualB_ID == Neighbors[i]))
                    {
                        NeighborEdges++;
                        break;
                    }
                }
            }
        }
        
        float Clustering = static_cast<float>(NeighborEdges) / (Neighbors.Num() * (Neighbors.Num() - 1) / 2);
        TotalClustering += Clustering;
    }
    
    Network.ClusteringCoefficient = NodeCount > 0 ? TotalClustering / NodeCount : 0.0f;
    
    // 更新網絡屬性
    Network.LastUpdated = FDateTime::Now();
    
    return true;
}

bool UMingSocialDynamicsSystem::SimulateCulturalEvolution(float DeltaTime)
{
    if (!bEnableCulturalEvolution)
    {
        return false;
    }
    
    // 模擬文化元素的演變
    for (auto& ElementPair : CulturalElements)
    {
        FCulturalElement& Element = ElementPair.Value;
        
        if (!Element.bIsEvolving)
        {
            continue;
        }
        
        // 隨機變異
        if (FMath::RandRange(0.0f, 1.0f) < 0.01f * DeltaTime)
        {
            // 添加新的相關價值觀
            FString NewValue = FString::Printf(TEXT("EvolvingValue_%d"), FMath::RandRange(1000, 9999));
            Element.AssociatedValues.Add(NewValue);
        }
        
        // 調整傳播強度
        Element.TransmissionStrength *= (1.0f + FMath::RandRange(-0.05f, 0.05f) * DeltaTime);
        Element.TransmissionStrength = FMath::Clamp(Element.TransmissionStrength, 0.1f, 2.0f);
    }
    
    return true;
}

FSocialIndividual UMingSocialDynamicsSystem::GetIndividual(const FString& IndividualID) const
{
    if (Individuals.Contains(IndividualID))
    {
        return Individuals[IndividualID];
    }
    
    return FSocialIndividual();
}

TMap<FString, float> UMingSocialDynamicsSystem::GetSystemStatistics() const
{
    return SystemStats;
}

// 私有方法實現
FString UMingSocialDynamicsSystem::GenerateUniqueIndividualID() const
{
    return FString::Printf(TEXT("Individual_%s_%d"), *FDateTime::Now().ToString(), FMath::RandRange(1000, 9999));
}

FString UMingSocialDynamicsSystem::GenerateUniqueRelationshipID() const
{
    return FString::Printf(TEXT("Relationship_%s_%d"), *FDateTime::Now().ToString(), FMath::RandRange(1000, 9999));
}

FString UMingSocialDynamicsSystem::GenerateUniqueEventID() const
{
    return FString::Printf(TEXT("Event_%s_%d"), *FDateTime::Now().ToString(), FMath::RandRange(1000, 9999));
}

FString UMingSocialDynamicsSystem::GenerateUniqueElementID() const
{
    return FString::Printf(TEXT("Element_%s_%d"), *FDateTime::Now().ToString(), FMath::RandRange(1000, 9999));
}

FString UMingSocialDynamicsSystem::GenerateUniqueBehaviorID() const
{
    return FString::Printf(TEXT("Behavior_%s_%d"), *FDateTime::Now().ToString(), FMath::RandRange(1000, 9999));
}

FString UMingSocialDynamicsSystem::GenerateUniqueNetworkID() const
{
    return FString::Printf(TEXT("Network_%s_%d"), *FDateTime::Now().ToString(), FMath::RandRange(1000, 9999));
}

FSocialIndividual UMingSocialDynamicsSystem::CreateDefaultIndividual(const FString& IndividualID, const FString& FirstName, const FString& LastName, int32 Age, const FString& Gender, ESocialClass SocialClass)
{
    FSocialIndividual Individual;
    Individual.IndividualID = IndividualID;
    Individual.FirstName = FirstName;
    Individual.LastName = LastName;
    Individual.Age = Age;
    Individual.Gender = Gender;
    Individual.SocialClass = SocialClass;
    Individual.Occupation = TEXT("Unemployed");
    Individual.Education = TEXT("Basic");
    Individual.Location = TEXT("Unknown");
    Individual.SocialInfluence = 1.0f;
    Individual.EconomicStatus = 1.0f;
    Individual.PoliticalPower = 1.0f;
    Individual.BirthDate = FDateTime::Now() - FTimespan::FromDays(Age * 365);
    Individual.LastUpdated = FDateTime::Now();
    Individual.bIsActive = true;
    
    // 根據社會階層設置初始屬性
    switch (SocialClass)
    {
    case ESocialClass::Upper:
        Individual.EconomicStatus = 3.0f;
        Individual.PoliticalPower = 2.5f;
        Individual.SocialInfluence = 2.0f;
        break;
    case ESocialClass::UpperMiddle:
        Individual.EconomicStatus = 2.0f;
        Individual.PoliticalPower = 1.5f;
        Individual.SocialInfluence = 1.5f;
        break;
    case ESocialClass::Middle:
        Individual.EconomicStatus = 1.0f;
        Individual.PoliticalPower = 1.0f;
        Individual.SocialInfluence = 1.0f;
        break;
    case ESocialClass::LowerMiddle:
        Individual.EconomicStatus = 0.7f;
        Individual.PoliticalPower = 0.5f;
        Individual.SocialInfluence = 0.8f;
        break;
    case ESocialClass::Lower:
        Individual.EconomicStatus = 0.3f;
        Individual.PoliticalPower = 0.2f;
        Individual.SocialInfluence = 0.5f;
        break;
    case ESocialClass::Outcast:
        Individual.EconomicStatus = 0.1f;
        Individual.PoliticalPower = 0.1f;
        Individual.SocialInfluence = 0.2f;
        break;
    }
    
    return Individual;
}

FSocialRelationship UMingSocialDynamicsSystem::CreateDefaultRelationship(const FString& RelationshipID, const FString& IndividualA_ID, const FString& IndividualB_ID, ESocialRelationType RelationType, float Strength)
{
    FSocialRelationship Relationship;
    Relationship.RelationshipID = RelationshipID;
    Relationship.IndividualA_ID = IndividualA_ID;
    Relationship.IndividualB_ID = IndividualB_ID;
    Relationship.RelationType = RelationType;
    Relationship.Strength = FMath::Clamp(Strength, 0.0f, 10.0f);
    Relationship.Trust = Strength * 0.8f;
    Relationship.Influence = Strength * 0.6f;
    Relationship.Duration = 0.0f;
    Relationship.RelationshipStatus = TEXT("Active");
    Relationship.FormationDate = FDateTime::Now();
    Relationship.LastInteraction = FDateTime::Now();
    Relationship.bIsActive = true;
    
    return Relationship;
}

FSocialEvent UMingSocialDynamicsSystem::CreateDefaultEvent(const FString& EventID, ESocialEventType EventType, const TArray<FString>& Participants, const FString& Location, float Impact)
{
    FSocialEvent Event;
    Event.EventID = EventID;
    Event.EventType = EventType;
    Event.EventName = FString::Printf(TEXT("%s Event"), *StaticEnum<ESocialEventType>()->GetNameStringByValue(static_cast<int64>(EventType)));
    Event.Description = FString::Printf(TEXT("A %s event occurred"), *StaticEnum<ESocialEventType>()->GetNameStringByValue(static_cast<int64>(EventType)));
    Event.Participants = Participants;
    Event.Location = Location;
    Event.Impact = FMath::Clamp(Impact, 0.0f, 10.0f);
    Event.Duration = Impact * 60.0f; // 影響持續時間
    Event.EventTime = FDateTime::Now();
    Event.bIsPublic = true;
    Event.bIsRecurring = false;
    
    return Event;
}

FCulturalElement UMingSocialDynamicsSystem::CreateDefaultCulturalElement(const FString& ElementID, const FString& ElementName, const FString& ElementCategory)
{
    FCulturalElement Element;
    Element.ElementID = ElementID;
    Element.ElementName = ElementName;
    Element.ElementCategory = ElementCategory;
    Element.Description = FString::Printf(TEXT("Cultural element: %s"), *ElementName);
    Element.AdoptionRate = 0.0f;
    Element.TransmissionStrength = 1.0f;
    Element.OriginTime = FDateTime::Now();
    Element.bIsDominant = false;
    Element.bIsEvolving = true;
    
    return Element;
}

FGroupBehavior UMingSocialDynamicsSystem::CreateDefaultGroupBehavior(const FString& BehaviorID, const FString& GroupID, EGroupBehaviorType BehaviorType, const TArray<FString>& Participants)
{
    FGroupBehavior Behavior;
    Behavior.BehaviorID = BehaviorID;
    Behavior.GroupID = GroupID;
    Behavior.BehaviorType = BehaviorType;
    Behavior.BehaviorName = FString::Printf(TEXT("%s Behavior"), *StaticEnum<EGroupBehaviorType>()->GetNameStringByValue(static_cast<int64>(BehaviorType)));
    Behavior.Description = FString::Printf(TEXT("Group behavior: %s"), *StaticEnum<EGroupBehaviorType>()->GetNameStringByValue(static_cast<int64>(BehaviorType)));
    Behavior.Participants = Participants;
    Behavior.Intensity = 1.0f;
    Behavior.Duration = 0.0f;
    Behavior.StartTime = FDateTime::Now();
    Behavior.SocialImpact = 1.0f;
    Behavior.bIsOngoing = false;
    
    return Behavior;
}

FSocialNetwork UMingSocialDynamicsSystem::CreateDefaultSocialNetwork(const FString& NetworkID, const FString& NetworkName, const FString& NetworkType)
{
    FSocialNetwork Network;
    Network.NetworkID = NetworkID;
    Network.NetworkName = NetworkName;
    Network.NetworkType = NetworkType;
    Network.Density = 0.0f;
    Network.ClusteringCoefficient = 0.0f;
    Network.AveragePathLength = 0.0f;
    Network.CreationTime = FDateTime::Now();
    Network.LastUpdated = FDateTime::Now();
    Network.bIsActive = true;
    
    return Network;
}

void UMingSocialDynamicsSystem::UpdateRelationships(float DeltaTime)
{
    // 更新關係強度（衰減）
    for (auto& RelationshipPair : Relationships)
    {
        FSocialRelationship& Relationship = RelationshipPair.Value;
        
        if (!Relationship.bIsActive)
        {
            continue;
        }
        
        // 關係衰減
        Relationship.Strength *= (1.0f - RelationshipDecayRate * DeltaTime);
        Relationship.Strength = FMath::Max(Relationship.Strength, 0.1f);
        
        // 更新持續時間
        Relationship.Duration += DeltaTime;
        
        // 更新最後交互時間
        if (FMath::RandRange(0.0f, 1.0f) < 0.1f * DeltaTime)
        {
            Relationship.LastInteraction = FDateTime::Now();
        }
    }
    
    // 移除過期關係
    for (auto It = Relationships.CreateIterator(); It; ++It)
    {
        if (It->Value.Strength < 0.1f)
        {
            It->Value.bIsActive = false;
            It.RemoveCurrent();
            ActiveRelationshipCount--;
        }
    }
    
    // 更新統計
    SystemStats[TEXT("ActiveRelationships")] = static_cast<float>(ActiveRelationshipCount);
}

void UMingSocialDynamicsSystem::ProcessSocialEvents(float DeltaTime)
{
    // 處理事件持續時間
    FDateTime CurrentTime = FDateTime::Now();
    
    for (int32 i = SocialEvents.Num() - 1; i >= 0; i--)
    {
        FSocialEvent& Event = SocialEvents[i];
        
        FTimespan TimeSinceEvent = CurrentTime - Event.EventTime;
        if (TimeSinceEvent.GetTotalSeconds() > Event.Duration)
        {
            // 事件結束，處理後果
            SocialEvents.RemoveAt(i);
        }
    }
    
    // 隨機觸發新事件
    if (Individuals.Num() > 0 && FMath::RandRange(0.0f, 1.0f) < 0.05f * DeltaTime)
    {
        TArray<FString> RandomParticipants;
        int32 ParticipantCount = FMath::RandRange(1, FMath::Min(5, Individuals.Num()));
        
        TArray<FString> IndividualIDs;
        Individuals.GetKeys(IndividualIDs);
        
        for (int32 i = 0; i < ParticipantCount; i++)
        {
            int32 RandomIndex = FMath::RandRange(0, IndividualIDs.Num() - 1);
            RandomParticipants.Add(IndividualIDs[RandomIndex]);
        }
        
        ESocialEventType RandomEventType = static_cast<ESocialEventType>(FMath::RandRange(0, static_cast<int32>(ESocialEventType::Disaster)));
        TriggerSocialEvent(RandomEventType, RandomParticipants, TEXT("Random Location"), FMath::RandRange(1.0f, 5.0f));
    }
}

void UMingSocialDynamicsSystem::UpdateCulturalElements(float DeltaTime)
{
    if (!bEnableCulturalEvolution)
    {
        return;
    }
    
    SimulateCulturalEvolution(DeltaTime);
}

void UMingSocialDynamicsSystem::UpdateGroupBehaviors(float DeltaTime)
{
    if (!bEnableGroupBehaviors)
    {
        return;
    }
    
    // 更新群體行為持續時間
    FDateTime CurrentTime = FDateTime::Now();
    
    for (int32 i = GroupBehaviors.Num() - 1; i >= 0; i--)
    {
        FGroupBehavior& Behavior = GroupBehaviors[i];
        
        FTimespan TimeSinceStart = CurrentTime - Behavior.StartTime;
        if (TimeSinceStart.GetTotalSeconds() > Behavior.Duration)
        {
            Behavior.bIsOngoing = false;
            GroupBehaviors.RemoveAt(i);
        }
    }
    
    // 隨機觸發新的群體行為
    if (Individuals.Num() > 10 && FMath::RandRange(0.0f, 1.0f) < 0.02f * DeltaTime)
    {
        TArray<FString> RandomParticipants;
        int32 ParticipantCount = FMath::RandRange(3, FMath::Min(10, Individuals.Num()));
        
        TArray<FString> IndividualIDs;
        Individuals.GetKeys(IndividualIDs);
        
        for (int32 i = 0; i < ParticipantCount; i++)
        {
            int32 RandomIndex = FMath::RandRange(0, IndividualIDs.Num() - 1);
            RandomParticipants.Add(IndividualIDs[RandomIndex]);
        }
        
        EGroupBehaviorType RandomBehaviorType = static_cast<EGroupBehaviorType>(FMath::RandRange(0, static_cast<int32>(EGroupBehaviorType::Mobilization)));
        SimulateGroupBehavior(TEXT("RandomGroup"), RandomBehaviorType, RandomParticipants);
    }
}

void UMingSocialDynamicsSystem::ProcessSocialMobility(float DeltaTime)
{
    // 處理社會階層流動
    for (auto& IndividualPair : Individuals)
    {
        FSocialIndividual& Individual = IndividualPair.Value;
        
        // 隨機決定是否嘗試流動
        if (FMath::RandRange(0.0f, 1.0f) < SocialMobilityProbability * DeltaTime)
        {
            ESocialClass TargetClass = static_cast<ESocialClass>(FMath::RandRange(0, static_cast<int32>(ESocialClass::Outcast)));
            ProcessSocialMobility(Individual.IndividualID, TargetClass);
        }
    }
}

float UMingSocialDynamicsSystem::CalculateRelationshipStrength(const FString& IndividualA_ID, const FString& IndividualB_ID) const
{
    if (!Individuals.Contains(IndividualA_ID) || !Individuals.Contains(IndividualB_ID))
    {
        return 0.0f;
    }
    
    const FSocialIndividual& IndividualA = Individuals[IndividualA_ID];
    const FSocialIndividual& IndividualB = Individuals[IndividualB_ID];
    
    // 計算基於共同特徵的關係強度
    float CommonFeatures = 0.0f;
    
    // 共同技能
    for (const FString& Skill : IndividualA.Skills)
    {
        if (IndividualB.Skills.Contains(Skill))
        {
            CommonFeatures += 0.2f;
        }
    }
    
    // 共同信念
    for (const FString& Belief : IndividualA.Beliefs)
    {
        if (IndividualB.Beliefs.Contains(Belief))
        {
            CommonFeatures += 0.3f;
        }
    }
    
    // 共同價值觀
    for (const FString& Value : IndividualA.Values)
    {
        if (IndividualB.Values.Contains(Value))
        {
            CommonFeatures += 0.2f;
        }
    }
    
    // 社會階層差異
    int32 ClassDiff = FMath::Abs(static_cast<int32>(IndividualA.SocialClass) - static_cast<int32>(IndividualB.SocialClass));
    float ClassSimilarity = 1.0f - (static_cast<float>(ClassDiff) / 5.0f);
    
    return FMath::Clamp(CommonFeatures + ClassSimilarity, 0.0f, 10.0f);
}

float UMingSocialDynamicsSystem::CalculateCulturalAdoption(const FString& IndividualID, const FString& ElementID) const
{
    if (!Individuals.Contains(IndividualID) || !CulturalElements.Contains(ElementID))
    {
        return 0.0f;
    }
    
    const FSocialIndividual& Individual = Individuals[IndividualID];
    const FCulturalElement& Element = CulturalElements[ElementID];
    
    // 計算採納概率
    float AdoptionProbability = CulturalTransmissionRate * Element.TransmissionStrength;
    
    // 個體開放性影響
    float Openness = Individual.SocialInfluence * 0.3f;
    
    // 社會階層影響
    float ClassInfluence = static_cast<float>(static_cast<int32>(Individual.SocialClass)) * 0.1f;
    
    return FMath::Clamp(AdoptionProbability + Openness + ClassInfluence, 0.0f, 1.0f);
}

TArray<FString> UMingSocialDynamicsSystem::FindSocialConnections(const FString& IndividualID, int32 MaxDepth) const
{
    TArray<FString> Connections;
    TSet<FString> Visited;
    TArray<FString> Queue;
    
    Queue.Add(IndividualID);
    Visited.Add(IndividualID);
    
    while (Queue.Num() > 0 && MaxDepth > 0)
    {
        FString CurrentID = Queue[0];
        Queue.RemoveAt(0);
        
        if (Individuals.Contains(CurrentID))
        {
            const FSocialIndividual& Individual = Individuals[CurrentID];
            
            for (const auto& RelationshipPair : Individual.Relationships)
            {
                FString ConnectedID = RelationshipPair.Key;
                if (!Visited.Contains(ConnectedID))
                {
                    Connections.Add(ConnectedID);
                    Visited.Add(ConnectedID);
                    Queue.Add(ConnectedID);
                }
            }
        }
        
        MaxDepth--;
    }
    
    return Connections;
}

ESocialClass UMingSocialDynamicsSystem::DetermineSocialClass(const FSocialIndividual& Individual) const
{
    // 基於經濟狀況和政治權力確定社會階層
    float CombinedScore = Individual.EconomicStatus * 0.6f + Individual.PoliticalPower * 0.4f;
    
    if (CombinedScore >= 2.5f)
    {
        return ESocialClass::Upper;
    }
    else if (CombinedScore >= 2.0f)
    {
        return ESocialClass::UpperMiddle;
    }
    else if (CombinedScore >= 1.0f)
    {
        return ESocialClass::Middle;
    }
    else if (CombinedScore >= 0.5f)
    {
        return ESocialClass::LowerMiddle;
    }
    else if (CombinedScore >= 0.2f)
    {
        return ESocialClass::Lower;
    }
    else
    {
        return ESocialClass::Outcast;
    }
}

bool UMingSocialDynamicsSystem::ValidateIndividualCreation(const FString& FirstName, const FString& LastName, int32 Age, const FString& Gender, ESocialClass SocialClass) const
{
    return !FirstName.IsEmpty() && !LastName.IsEmpty() && Age > 0 && Age < 120 && !Gender.IsEmpty();
}

bool UMingSocialDynamicsSystem::ValidateRelationshipEstablishment(const FString& IndividualA_ID, const FString& IndividualB_ID, ESocialRelationType RelationType, float Strength) const
{
    return Individuals.Contains(IndividualA_ID) && Individuals.Contains(IndividualB_ID) && Strength > 0.0f;
}

bool UMingSocialDynamicsSystem::ValidateSocialEvent(ESocialEventType EventType, const TArray<FString>& Participants, const FString& Location, float Impact) const
{
    return Participants.Num() > 0 && Impact > 0.0f;
}
