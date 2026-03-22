#include "MingGoRTSRelationshipNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UMingGoRTSRelationshipNetwork::UMingGoRTSRelationshipNetwork()
{
    bIsInitialized = false;
}

void UMingGoRTSRelationshipNetwork::InitializeRelationshipNetwork()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("關係網絡系統已經初始化"));
        return;
    }

    // 初始化歷史關係網絡
    InitializeHistoricalNetworks();

    // 構建網絡映射
    NetworkMap.Empty();
    for (const FRelationshipNetwork& Network : AllNetworks)
    {
        NetworkMap.Add(Network.NetworkID, Network);
        
        // 構建角色到網絡的映射
        for (const FString& CharacterID : Network.CharacterIDs)
        {
            CharacterNetworkMap.Add(CharacterID, Network.NetworkID);
        }
    }

    bIsInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("關係網絡系統初始化完成，共載入 %d 個網絡"), AllNetworks.Num());
}

TArray<FRelationshipNetwork> UMingGoRTSRelationshipNetwork::GetAllNetworks() const
{
    return AllNetworks;
}

FRelationshipNetwork UMingGoRTSRelationshipNetwork::GetNetwork(const FString& NetworkID) const
{
    if (const FRelationshipNetwork* Network = NetworkMap.Find(NetworkID))
    {
        return *Network;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("未找到網絡 ID：%s"), *NetworkID);
    return FRelationshipNetwork();
}

FString UMingGoRTSRelationshipNetwork::CreateNetwork(const FString& NetworkName, const TArray<FString>& CharacterIDs)
{
    FString NetworkID = GenerateNetworkID(NetworkName);
    
    FRelationshipNetwork NewNetwork;
    NewNetwork.NetworkID = NetworkID;
    NewNetwork.NetworkName = NetworkName;
    NewNetwork.Description = FString::Printf(TEXT("用戶創建的網絡：%s"), *NetworkName);
    NewNetwork.CharacterIDs = CharacterIDs;
    
    AllNetworks.Add(NewNetwork);
    NetworkMap.Add(NetworkID, NewNetwork);
    
    // 構建角色到網絡的映射
    for (const FString& CharacterID : CharacterIDs)
    {
        CharacterNetworkMap.Add(CharacterID, NetworkID);
    }
    
    UE_LOG(LogTemp, Log, TEXT("創建新網絡：%s，ID：%s"), *NetworkName, *NetworkID);
    return NetworkID;
}

bool UMingGoRTSRelationshipNetwork::AddCharacterToNetwork(const FString& NetworkID, const FString& CharacterID)
{
    if (!NetworkMap.Contains(NetworkID))
    {
        UE_LOG(LogTemp, Error, TEXT("網絡不存在：%s"), *NetworkID);
        return false;
    }

    FRelationshipNetwork& Network = NetworkMap[NetworkID];
    
    if (Network.CharacterIDs.Contains(CharacterID))
    {
        UE_LOG(LogTemp, Warning, TEXT("角色已在網絡中：%s"), *CharacterID);
        return false;
    }

    Network.CharacterIDs.Add(CharacterID);
    CharacterNetworkMap.Add(CharacterID, NetworkID);
    
    UE_LOG(LogTemp, Log, TEXT("角色 %s 已添加到網絡 %s"), *CharacterID, *NetworkID);
    return true;
}

bool UMingGoRTSRelationshipNetwork::RemoveCharacterFromNetwork(const FString& NetworkID, const FString& CharacterID)
{
    if (!NetworkMap.Contains(NetworkID))
    {
        UE_LOG(LogTemp, Error, TEXT("網絡不存在：%s"), *NetworkID);
        return false;
    }

    FRelationshipNetwork& Network = NetworkMap[NetworkID];
    
    if (!Network.CharacterIDs.Contains(CharacterID))
    {
        UE_LOG(LogTemp, Warning, TEXT("角色不在網絡中：%s"), *CharacterID);
        return false;
    }

    Network.CharacterIDs.Remove(CharacterID);
    CharacterNetworkMap.Remove(CharacterID);
    
    // 移除相關的關係連接
    for (int32 i = Network.Connections.Num() - 1; i >= 0; --i)
    {
        const FRelationshipConnection& Connection = Network.Connections[i];
        if (Connection.SourceCharacterID == CharacterID || Connection.TargetCharacterID == CharacterID)
        {
            Network.Connections.RemoveAt(i);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("角色 %s 已從網絡 %s 移除"), *CharacterID, *NetworkID);
    return true;
}

bool UMingGoRTSRelationshipNetwork::CreateRelationship(const FString& SourceID, const FString& TargetID, ERelationshipType Type, ERelationshipStrength Strength)
{
    if (SourceID == TargetID)
    {
        UE_LOG(LogTemp, Warning, TEXT("不能創建自我關係：%s"), *SourceID);
        return false;
    }

    FString RelationshipID = GenerateRelationshipID(SourceID, TargetID);
    
    if (RelationshipMap.Contains(RelationshipID))
    {
        UE_LOG(LogTemp, Warning, TEXT("關係已存在：%s -> %s"), *SourceID, *TargetID);
        return false;
    }

    // 檢查關係衝突
    if (CheckRelationshipConflict(SourceID, TargetID, Type))
    {
        ResolveRelationshipConflict(SourceID, TargetID, Type);
    }

    FRelationshipConnection NewConnection;
    NewConnection.SourceCharacterID = SourceID;
    NewConnection.TargetCharacterID = TargetID;
    NewConnection.RelationshipType = Type;
    NewConnection.Strength = Strength;
    NewConnection.Status = ERelationshipStatus::Active;
    NewConnection.RelationshipValue = GetRelationshipStrengthWeight(Strength) * 100.0f;
    NewConnection.TrustLevel = CalculateTrustLevel(SourceID, TargetID);
    NewConnection.InfluenceLevel = CalculateInfluenceLevel(SourceID, TargetID);
    NewConnection.bIsReciprocal = IsRelationshipSymmetric(Type);
    NewConnection.LastInteractionDate = FDateTime::Now().ToString();
    NewConnection.InteractionFrequency = 1.0f;

    RelationshipMap.Add(RelationshipID, NewConnection);

    // 如果關係是對稱的，創建反向關係
    if (NewConnection.bIsReciprocal)
    {
        CreateSymmetricRelationship(SourceID, TargetID, Type, Strength);
    }

    // 更新網絡
    if (CharacterNetworkMap.Contains(SourceID))
    {
        FString NetworkID = CharacterNetworkMap[SourceID];
        if (NetworkMap.Contains(NetworkID))
        {
            FRelationshipNetwork& Network = NetworkMap[NetworkID];
            Network.Connections.Add(NewConnection);
            UpdateNetworkAnalysis(NetworkID);
        }
    }

    // 廣播關係變化事件
    OnRelationshipChanged.Broadcast(SourceID, TargetID);

    UE_LOG(LogTemp, Log, TEXT("創建關係：%s -> %s，類型：%s"), *SourceID, *TargetID, *StaticEnum<ERelationshipType>()->GetValueAsString(Type));
    return true;
}

bool UMingGoRTSRelationshipNetwork::UpdateRelationship(const FString& SourceID, const FString& TargetID, float NewValue, const FString& Reason)
{
    FString RelationshipID = GenerateRelationshipID(SourceID, TargetID);
    
    if (!RelationshipMap.Contains(RelationshipID))
    {
        UE_LOG(LogTemp, Warning, TEXT("關係不存在：%s -> %s"), *SourceID, *TargetID);
        return false;
    }

    FRelationshipConnection& Connection = RelationshipMap[RelationshipID];
    float OldValue = Connection.RelationshipValue;
    Connection.RelationshipValue = FMath::Clamp(NewValue, 0.0f, 100.0f);
    Connection.LastInteractionDate = FDateTime::Now().ToString();
    Connection.InteractionFrequency += 1.0f;

    // 更新關係狀態
    UpdateRelationshipStatus(SourceID, TargetID);

    // 記錄關係變化
    RecordRelationshipChange(SourceID, TargetID, Reason, OldValue, NewValue);

    // 廣播關係變化事件
    OnRelationshipChanged.Broadcast(SourceID, TargetID);

    UE_LOG(LogTemp, Log, TEXT("更新關係：%s -> %s，新值：%.1f，原因：%s"), *SourceID, *TargetID, NewValue, *Reason);
    return true;
}

FRelationshipConnection UMingGoRTSRelationshipNetwork::GetRelationship(const FString& SourceID, const FString& TargetID) const
{
    FString RelationshipID = GenerateRelationshipID(SourceID, TargetID);
    
    if (const FRelationshipConnection* Connection = RelationshipMap.Find(RelationshipID))
    {
        return *Connection;
    }
    
    return FRelationshipConnection();
}

TArray<FRelationshipConnection> UMingGoRTSRelationshipNetwork::GetCharacterRelationships(const FString& CharacterID) const
{
    TArray<FRelationshipConnection> Relationships;
    
    for (const auto& RelationshipPair : RelationshipMap)
    {
        const FRelationshipConnection& Connection = RelationshipPair.Value;
        if (Connection.SourceCharacterID == CharacterID || Connection.TargetCharacterID == CharacterID)
        {
            Relationships.Add(Connection);
        }
    }
    
    return Relationships;
}

TArray<FString> UMingGoRTSRelationshipNetwork::GetDirectConnections(const FString& CharacterID) const
{
    TArray<FString> DirectConnections;
    
    for (const auto& RelationshipPair : RelationshipMap)
    {
        const FRelationshipConnection& Connection = RelationshipPair.Value;
        if (Connection.SourceCharacterID == CharacterID)
        {
            DirectConnections.Add(Connection.TargetCharacterID);
        }
        else if (Connection.TargetCharacterID == CharacterID && Connection.bIsReciprocal)
        {
            DirectConnections.Add(Connection.SourceCharacterID);
        }
    }
    
    return DirectConnections;
}

TArray<FString> UMingGoRTSRelationshipNetwork::GetIndirectConnections(const FString& CharacterID) const
{
    TArray<FString> IndirectConnections;
    TArray<FString> DirectConnections = GetDirectConnections(CharacterID);
    TSet<FString> ProcessedCharacters;
    ProcessedCharacters.Add(CharacterID);
    
    // 添加直接連接
    for (const FString& DirectID : DirectConnections)
    {
        ProcessedCharacters.Add(DirectID);
    }
    
    // 查找二度連接
    for (const FString& DirectID : DirectConnections)
    {
        TArray<FString> SecondLevelConnections = GetDirectConnections(DirectID);
        for (const FString& SecondLevelID : SecondLevelConnections)
        {
            if (!ProcessedCharacters.Contains(SecondLevelID))
            {
                IndirectConnections.Add(SecondLevelID);
                ProcessedCharacters.Add(SecondLevelID);
            }
        }
    }
    
    return IndirectConnections;
}

float UMingGoRTSRelationshipNetwork::CalculateRelationshipStrength(const FString& SourceID, const FString& TargetID) const
{
    FRelationshipConnection Connection = GetRelationship(SourceID, TargetID);
    return Connection.RelationshipValue;
}

bool UMingGoRTSRelationshipNetwork::ApplyRelationshipInfluence(const FString& SourceID, const FString& TargetID, const FString& InfluenceType, float InfluenceValue)
{
    FRelationshipConnection Connection = GetRelationship(SourceID, TargetID);
    
    if (Connection.SourceCharacterID.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("關係不存在：%s -> %s"), *SourceID, *TargetID);
        return false;
    }

    // 應用影響
    float NewValue = Connection.RelationshipValue + InfluenceValue;
    return UpdateRelationship(SourceID, TargetID, NewValue, FString::Printf(TEXT("應用影響：%s"), *InfluenceType));
}

void UMingGoRTSRelationshipNetwork::PropagateRelationshipInfluence(const FString& SourceID, const FString& TargetID, const FString& InfluenceType, float InfluenceValue, int32 MaxDepth)
{
    if (MaxDepth <= 0)
    {
        return;
    }

    // 應用直接影響
    ApplyRelationshipInfluence(SourceID, TargetID, InfluenceType, InfluenceValue);

    // 傳播到間接連接
    TArray<FString> IndirectConnections = GetIndirectConnections(TargetID);
    for (const FString& IndirectID : IndirectConnections)
    {
        if (IndirectID != SourceID)
        {
            float AttenuatedValue = InfluenceValue * 0.5f; // 衰減係數
            PropagateRelationshipInfluence(TargetID, IndirectID, InfluenceType, AttenuatedValue, MaxDepth - 1);
        }
    }
}

void UMingGoRTSRelationshipNetwork::AnalyzeNetwork(const FString& NetworkID)
{
    if (!NetworkMap.Contains(NetworkID))
    {
        UE_LOG(LogTemp, Error, TEXT("網絡不存在：%s"), *NetworkID);
        return;
    }

    FRelationshipNetwork& Network = NetworkMap[NetworkID];
    
    // 計算直接連接
    CalculateDirectConnections(NetworkID);
    
    // 計算間接連接
    CalculateIndirectConnections(NetworkID);
    
    // 計算網絡統計
    CalculateNetworkStatistics(NetworkID);
    
    // 分析關係模式
    AnalyzeRelationshipPatterns(NetworkID);

    // 廣播分析完成事件
    OnNetworkAnalysisComplete.Broadcast(NetworkID, Network);

    UE_LOG(LogTemp, Log, TEXT("網絡分析完成：%s"), *NetworkID);
}

float UMingGoRTSRelationshipNetwork::CalculateNetworkDensity(const FString& NetworkID) const
{
    if (!NetworkMap.Contains(NetworkID))
    {
        return 0.0f;
    }

    const FRelationshipNetwork& Network = NetworkMap[NetworkID];
    int32 NodeCount = Network.CharacterIDs.Num();
    
    if (NodeCount < 2)
    {
        return 0.0f;
    }

    int32 PossibleConnections = NodeCount * (NodeCount - 1) / 2;
    int32 ActualConnections = Network.Connections.Num();
    
    return static_cast<float>(ActualConnections) / static_cast<float>(PossibleConnections);
}

float UMingGoRTSRelationshipNetwork::CalculateNetworkCohesion(const FString& NetworkID) const
{
    if (!NetworkMap.Contains(NetworkID))
    {
        return 0.0f;
    }

    const FRelationshipNetwork& Network = NetworkMap[NetworkID];
    
    if (Network.CharacterIDs.Num() < 2)
    {
        return 1.0f;
    }

    float TotalStrength = 0.0f;
    int32 ConnectionCount = 0;
    
    for (const FRelationshipConnection& Connection : Network.Connections)
    {
        TotalStrength += Connection.RelationshipValue;
        ConnectionCount++;
    }
    
    if (ConnectionCount == 0)
    {
        return 0.0f;
    }
    
    return TotalStrength / (ConnectionCount * 100.0f);
}

FString UMingGoRTSRelationshipNetwork::FindCentralCharacter(const FString& NetworkID) const
{
    if (!NetworkMap.Contains(NetworkID))
    {
        return TEXT("");
    }

    const FRelationshipNetwork& Network = NetworkMap[NetworkID];
    FString CentralCharacter;
    float MaxInfluence = 0.0f;
    
    for (const FString& CharacterID : Network.CharacterIDs)
    {
        float Influence = CalculateInfluence(CharacterID);
        if (Influence > MaxInfluence)
        {
            MaxInfluence = Influence;
            CentralCharacter = CharacterID;
        }
    }
    
    return CentralCharacter;
}

TArray<FString> UMingGoRTSRelationshipNetwork::FindKeyInfluencers(const FString& NetworkID) const
{
    TArray<FString> KeyInfluencers;
    
    if (!NetworkMap.Contains(NetworkID))
    {
        return KeyInfluencers;
    }

    const FRelationshipNetwork& Network = NetworkMap[NetworkID];
    TMap<FString, float> InfluenceMap;
    
    // 計算每個角色的影響力
    for (const FString& CharacterID : Network.CharacterIDs)
    {
        float Influence = CalculateInfluence(CharacterID);
        InfluenceMap.Add(CharacterID, Influence);
    }
    
    // 找到影響力最高的角色
    float AverageInfluence = 0.0f;
    for (const auto& InfluencePair : InfluenceMap)
    {
        AverageInfluence += InfluencePair.Value;
    }
    AverageInfluence /= InfluenceMap.Num();
    
    for (const auto& InfluencePair : InfluenceMap)
    {
        if (InfluencePair.Value > AverageInfluence * 1.5f)
        {
            KeyInfluencers.Add(InfluencePair.Key);
        }
    }
    
    return KeyInfluencers;
}

TArray<FString> UMingGoRTSRelationshipNetwork::FindBridgingCharacters(const FString& NetworkID) const
{
    TArray<FString> BridgingCharacters;
    
    if (!NetworkMap.Contains(NetworkID))
    {
        return BridgingCharacters;
    }

    const FRelationshipNetwork& Network = NetworkMap[NetworkID];
    
    // 簡化實現：找到連接不同群組的角色
    for (const FString& CharacterID : Network.CharacterIDs)
    {
        TArray<FString> Connections = GetDirectConnections(CharacterID);
        if (Connections.Num() >= 3)
        {
            BridgingCharacters.Add(CharacterID);
        }
    }
    
    return BridgingCharacters;
}

TArray<FString> UMingGoRTSRelationshipNetwork::FindShortestPath(const FString& SourceID, const FString& TargetID) const
{
    TArray<FString> ShortestPath;
    
    if (SourceID == TargetID)
    {
        ShortestPath.Add(SourceID);
        return ShortestPath;
    }

    // 簡化實現：使用BFS算法
    TMap<FString, FString> ParentMap;
    TQueue<FString> Queue;
    TSet<FString> Visited;
    
    Queue.Enqueue(SourceID);
    Visited.Add(SourceID);
    
    while (!Queue.IsEmpty())
    {
        FString Current;
        Queue.Dequeue(Current);
        
        if (Current == TargetID)
        {
            // 重建路徑
            FString Node = TargetID;
            while (Node != SourceID)
            {
                ShortestPath.Insert(Node, 0);
                Node = ParentMap[Node];
            }
            ShortestPath.Insert(SourceID, 0);
            return ShortestPath;
        }
        
        TArray<FString> Connections = GetDirectConnections(Current);
        for (const FString& Connection : Connections)
        {
            if (!Visited.Contains(Connection))
            {
                Visited.Add(Connection);
                ParentMap.Add(Connection, Current);
                Queue.Enqueue(Connection);
            }
        }
    }
    
    return ShortestPath; // 無路徑
}

float UMingGoRTSRelationshipNetwork::CalculateInfluence(const FString& CharacterID) const
{
    float TotalInfluence = 0.0f;
    TArray<FRelationshipConnection> Relationships = GetCharacterRelationships(CharacterID);
    
    for (const FRelationshipConnection& Connection : Relationships)
    {
        float Weight = CalculateRelationshipWeight(Connection);
        TotalInfluence += Weight;
    }
    
    return TotalInfluence;
}

TArray<FString> UMingGoRTSRelationshipNetwork::GetRelationshipSuggestions(const FString& CharacterID) const
{
    TArray<FString> Suggestions;
    
    // 獲取間接連接
    TArray<FString> IndirectConnections = GetIndirectConnections(CharacterID);
    
    // 獲取現有關係
    TArray<FRelationshipConnection> ExistingRelationships = GetCharacterRelationships(CharacterID);
    TSet<FString> ExistingConnections;
    for (const FRelationshipConnection& Connection : ExistingRelationships)
    {
        ExistingConnections.Add(Connection.TargetCharacterID);
    }
    
    // 建議與間接連接建立關係
    for (const FString& IndirectID : IndirectConnections)
    {
        if (!ExistingConnections.Contains(IndirectID))
        {
            Suggestions.Add(FString::Printf(TEXT("建議與 %s 建立關係"), *IndirectID));
        }
    }
    
    return Suggestions;
}

void UMingGoRTSRelationshipNetwork::RecordRelationshipEvent(const FString& EventName, const TArray<FString>& InvolvedCharacters, const FString& Description)
{
    FRelationshipEvent NewEvent;
    NewEvent.EventID = FString::Printf(TEXT("Event_%s"), *FDateTime::Now().ToString());
    NewEvent.EventName = EventName;
    NewEvent.Description = Description;
    NewEvent.InvolvedCharacters = InvolvedCharacters;
    NewEvent.EventType = TEXT("UserEvent");
    NewEvent.EventDate = FDateTime::Now().ToString();
    NewEvent.EventImpact = 0.0f;
    NewEvent.bIsPositive = true;
    NewEvent.bIsHistorical = false;
    
    RelationshipHistory.Add(NewEvent);
    
    // 廣播關係事件
    OnRelationshipEvent.Broadcast(NewEvent.EventID, NewEvent.EventName, InvolvedCharacters);
    
    UE_LOG(LogTemp, Log, TEXT("記錄關係事件：%s"), *EventName);
}

TArray<FRelationshipEvent> UMingGoRTSRelationshipNetwork::GetRelationshipHistory(const FString& CharacterID) const
{
    TArray<FRelationshipEvent> CharacterHistory;
    
    for (const FRelationshipEvent& Event : RelationshipHistory)
    {
        if (Event.InvolvedCharacters.Contains(CharacterID))
        {
            CharacterHistory.Add(Event);
        }
    }
    
    return CharacterHistory;
}

bool UMingGoRTSRelationshipNetwork::SaveRelationshipData(const FString& SaveSlotName)
{
    // TODO: 實現關係數據保存
    UE_LOG(LogTemp, Log, TEXT("保存關係數據到：%s"), *SaveSlotName);
    return true;
}

bool UMingGoRTSRelationshipNetwork::LoadRelationshipData(const FString& SaveSlotName)
{
    // TODO: 實現關係數據載入
    UE_LOG(LogTemp, Log, TEXT("從 %s 載入關係數據"), *SaveSlotName);
    return true;
}

void UMingGoRTSRelationshipNetwork::InitializeHistoricalNetworks()
{
    CreateNationalistNetwork();
    CreateCommunistNetwork();
    CreateWarlordNetwork();
    CreateDiplomaticNetwork();
    CreateFamilyNetwork();
    CreateBusinessNetwork();
}

void UMingGoRTSRelationshipNetwork::CreateNationalistNetwork()
{
    FRelationshipNetwork NationalistNetwork;
    NationalistNetwork.NetworkID = GenerateNetworkID(TEXT("Nationalist"));
    NationalistNetwork.NetworkName = TEXT("國民黨網絡");
    NationalistNetwork.Description = TEXT("國民黨內部關係網絡");
    
    // 添加核心人物
    NationalistNetwork.CharacterIDs.Add(TEXT("ChiangKaiShek"));
    NationalistNetwork.CharacterIDs.Add(TEXT("SoongMeiLing"));
    NationalistNetwork.CharacterIDs.Add(TEXT("ChenCheng"));
    NationalistNetwork.CharacterIDs.Add(TEXT("HeYingqin"));
    
    // 創建關係連接
    CreateRelationship(TEXT("ChiangKaiShek"), TEXT("SoongMeiLing"), ERelationshipType::Family, ERelationshipStrength::VeryStrong);
    CreateRelationship(TEXT("ChiangKaiShek"), TEXT("ChenCheng"), ERelationshipType::Subordinate, ERelationshipStrength::Strong);
    CreateRelationship(TEXT("ChiangKaiShek"), TEXT("HeYingqin"), ERelationshipType::Colleague, ERelationshipStrength::Strong);
    
    AllNetworks.Add(NationalistNetwork);
}

void UMingGoRTSRelationshipNetwork::CreateCommunistNetwork()
{
    FRelationshipNetwork CommunistNetwork;
    CommunistNetwork.NetworkID = GenerateNetworkID(TEXT("Communist"));
    CommunistNetwork.NetworkName = TEXT("共產黨網絡");
    CommunistNetwork.Description = TEXT("共產黨內部關係網絡");
    
    // 添加核心人物
    CommunistNetwork.CharacterIDs.Add(TEXT("MaoZedong"));
    CommunistNetwork.CharacterIDs.Add(TEXT("ZhouEnlai"));
    CommunistNetwork.CharacterIDs.Add(TEXT("ZhuDe"));
    CommunistNetwork.CharacterIDs.Add(TEXT("LiuShaoqi"));
    
    // 創建關係連接
    CreateRelationship(TEXT("MaoZedong"), TEXT("ZhouEnlai"), ERelationshipType::Colleague, ERelationshipStrength::VeryStrong);
    CreateRelationship(TEXT("MaoZedong"), TEXT("ZhuDe"), ERelationshipType::Colleague, ERelationshipStrength::Strong);
    CreateRelationship(TEXT("ZhouEnlai"), TEXT("ZhuDe"), ERelationshipType::Colleague, ERelationshipStrength::Strong);
    
    AllNetworks.Add(CommunistNetwork);
}

void UMingGoRTSRelationshipNetwork::CreateWarlordNetwork()
{
    FRelationshipNetwork WarlordNetwork;
    WarlordNetwork.NetworkID = GenerateNetworkID(TEXT("Warlord"));
    WarlordNetwork.NetworkName = TEXT("軍閥網絡");
    WarlordNetwork.Description = TEXT("軍閥之間的關係網絡");
    
    // 添加軍閥人物
    WarlordNetwork.CharacterIDs.Add(TEXT("ZhangZuolin"));
    WarlordNetwork.CharacterIDs.Add(TEXT("ZhangXueliang"));
    WarlordNetwork.CharacterIDs.Add(TEXT("FengYuxiang"));
    WarlordNetwork.CharacterIDs.Add(TEXT("YanXishan"));
    
    // 創建關係連接
    CreateRelationship(TEXT("ZhangZuolin"), TEXT("ZhangXueliang"), ERelationshipType::Family, ERelationshipStrength::VeryStrong);
    CreateRelationship(TEXT("ZhangZuolin"), TEXT("FengYuxiang"), ERelationshipType::Rival, ERelationshipStrength::Moderate);
    CreateRelationship(TEXT("FengYuxiang"), TEXT("YanXishan"), ERelationshipType::Ally, ERelationshipStrength::Moderate);
    
    AllNetworks.Add(WarlordNetwork);
}

void UMingGoRTSRelationshipNetwork::CreateDiplomaticNetwork()
{
    FRelationshipNetwork DiplomaticNetwork;
    DiplomaticNetwork.NetworkID = GenerateNetworkID(TEXT("Diplomatic"));
    DiplomaticNetwork.NetworkName = TEXT("外交網絡");
    DiplomaticNetwork.Description = TEXT("國際外交關係網絡");
    
    // 添加外交人物
    DiplomaticNetwork.CharacterIDs.Add(TEXT("ZhouEnlai"));
    DiplomaticNetwork.CharacterIDs.Add(TEXT("SoongMeiLing"));
    DiplomaticNetwork.CharacterIDs.Add(TEXT("Chennault"));
    DiplomaticNetwork.CharacterIDs.Add(TEXT("Stilwell"));
    
    // 創建關係連接
    CreateRelationship(TEXT("ZhouEnlai"), TEXT("SoongMeiLing"), ERelationshipType::Diplomatic, ERelationshipStrength::Moderate);
    CreateRelationship(TEXT("SoongMeiLing"), TEXT("Chennault"), ERelationshipType::Diplomatic, ERelationshipStrength::Strong);
    CreateRelationship(TEXT("Chennault"), TEXT("Stilwell"), ERelationshipType::Colleague, ERelationshipStrength::Moderate);
    
    AllNetworks.Add(DiplomaticNetwork);
}

void UMingGoRTSRelationshipNetwork::CreateFamilyNetwork()
{
    FRelationshipNetwork FamilyNetwork;
    FamilyNetwork.NetworkID = GenerateNetworkID(TEXT("Family"));
    FamilyNetwork.NetworkName = TEXT("家庭網絡");
    FamilyNetwork.Description = TEXT("家庭關係網絡");
    
    // 添加家庭成員
    FamilyNetwork.CharacterIDs.Add(TEXT("ChiangKaiShek"));
    FamilyNetwork.CharacterIDs.Add(TEXT("ChiangChingkuo"));
    FamilyNetwork.CharacterIDs.Add(TEXT("ChiangWeiguo"));
    FamilyNetwork.CharacterIDs.Add(TEXT("SoongMeiLing"));
    
    // 創建關係連接
    CreateRelationship(TEXT("ChiangKaiShek"), TEXT("ChiangChingkuo"), ERelationshipType::Family, ERelationshipStrength::VeryStrong);
    CreateRelationship(TEXT("ChiangKaiShek"), TEXT("ChiangWeiguo"), ERelationshipType::Family, ERelationshipStrength::Strong);
    CreateRelationship(TEXT("ChiangKaiShek"), TEXT("SoongMeiLing"), ERelationshipType::Romantic, ERelationshipStrength::VeryStrong);
    
    AllNetworks.Add(FamilyNetwork);
}

void UMingGoRTSRelationshipNetwork::CreateBusinessNetwork()
{
    FRelationshipNetwork BusinessNetwork;
    BusinessNetwork.NetworkID = GenerateNetworkID(TEXT("Business"));
    BusinessNetwork.NetworkName = TEXT("商業網絡");
    BusinessNetwork.Description = TEXT("商業關係網絡");
    
    // 添加商業人物
    BusinessNetwork.CharacterIDs.Add(TEXT("SoongTseven"));
    BusinessNetwork.CharacterIDs.Add(TEXT("KongXiangxi"));
    BusinessNetwork.CharacterIDs.Add(TEXT("ChenGuofu"));
    BusinessNetwork.CharacterIDs.Add(TEXT("ChenLifu"));
    
    // 創建關係連接
    CreateRelationship(TEXT("SoongTseven"), TEXT("KongXiangxi"), ERelationshipType::Business, ERelationshipStrength::Strong);
    CreateRelationship(TEXT("SoongTseven"), TEXT("ChenGuofu"), ERelationshipType::Business, ERelationshipStrength::Moderate);
    CreateRelationship(TEXT("ChenGuofu"), TEXT("ChenLifu"), ERelationshipType::Family, ERelationshipStrength::Strong);
    
    AllNetworks.Add(BusinessNetwork);
}

void UMingGoRTSRelationshipNetwork::UpdateNetworkAnalysis(const FString& NetworkID)
{
    if (!NetworkMap.Contains(NetworkID))
    {
        return;
    }

    FRelationshipNetwork& Network = NetworkMap[NetworkID];
    
    // 計算網絡密度
    Network.NetworkDensity = CalculateNetworkDensity(NetworkID);
    
    // 計算網絡凝聚力
    Network.NetworkCohesion = CalculateNetworkCohesion(NetworkID);
    
    // 找到中心角色
    Network.CentralCharacterID = FindCentralCharacter(NetworkID);
    
    // 找到關鍵影響者
    Network.KeyInfluencers = FindKeyInfluencers(NetworkID);
    
    // 找到橋接角色
    Network.BridgingCharacters = FindBridgingCharacters(NetworkID);
    
    // 判斷網絡狀態
    Network.bIsStable = Network.NetworkCohesion > 0.7f;
    Network.bIsGrowing = Network.NetworkDensity > 0.5f;
}

void UMingGoRTSRelationshipNetwork::CalculateDirectConnections(const FString& NetworkID)
{
    if (!NetworkMap.Contains(NetworkID))
    {
        return;
    }

    FRelationshipNetwork& Network = NetworkMap[NetworkID];
    Network.DirectConnections.Empty();
    
    for (const FString& CharacterID : Network.CharacterIDs)
    {
        TArray<FString> Connections = GetDirectConnections(CharacterID);
        Network.DirectConnections.Add(CharacterID, Connections);
    }
}

void UMingGoRTSRelationshipNetwork::CalculateIndirectConnections(const FString& NetworkID)
{
    if (!NetworkMap.Contains(NetworkID))
    {
        return;
    }

    FRelationshipNetwork& Network = NetworkMap[NetworkID];
    Network.IndirectConnections.Empty();
    
    for (const FString& CharacterID : Network.CharacterIDs)
    {
        TArray<FString> Connections = GetIndirectConnections(CharacterID);
        Network.IndirectConnections.Add(CharacterID, Connections);
    }
}

void UMingGoRTSRelationshipNetwork::CalculateNetworkStatistics(const FString& NetworkID)
{
    if (!NetworkMap.Contains(NetworkID))
    {
        return;
    }

    FRelationshipNetwork& Network = NetworkMap[NetworkID];
    
    // 計算網絡密度
    Network.NetworkDensity = CalculateNetworkDensity(NetworkID);
    
    // 計算網絡凝聚力
    Network.NetworkCohesion = CalculateNetworkCohesion(NetworkID);
    
    // 找到中心角色
    Network.CentralCharacterID = FindCentralCharacter(NetworkID);
    
    // 找到關鍵影響者
    Network.KeyInfluencers = FindKeyInfluencers(NetworkID);
    
    // 找到橋接角色
    Network.BridgingCharacters = FindBridgingCharacters(NetworkID);
}

void UMingGoRTSRelationshipNetwork::ApplyRelationshipChange(const FString& SourceID, const FString& TargetID, float ChangeValue)
{
    UpdateRelationship(SourceID, TargetID, ChangeValue, TEXT("關係變化"));
}

void UMingGoRTSRelationshipNetwork::PropagateInfluenceToNetwork(const FString& NetworkID, const FString& SourceID, const FString& InfluenceType, float InfluenceValue)
{
    if (!NetworkMap.Contains(NetworkID))
    {
        return;
    }

    const FRelationshipNetwork& Network = NetworkMap[NetworkID];
    
    for (const FString& CharacterID : Network.CharacterIDs)
    {
        if (CharacterID != SourceID)
        {
            PropagateRelationshipInfluence(SourceID, CharacterID, InfluenceType, InfluenceValue, 3);
        }
    }
}

FString UMingGoRTSRelationshipNetwork::GenerateRelationshipID(const FString& SourceID, const FString& TargetID) const
{
    return FString::Printf(TEXT("%s_%s"), *SourceID, *TargetID);
}

FString UMingGoRTSRelationshipNetwork::GenerateNetworkID(const FString& BaseName) const
{
    return FString::Printf(TEXT("NET_%s_%s"), *BaseName, *FDateTime::Now().ToString());
}

bool UMingGoRTSRelationshipNetwork::ValidateRelationshipConnection(const FRelationshipConnection& Connection) const
{
    if (Connection.SourceCharacterID.IsEmpty() || Connection.TargetCharacterID.IsEmpty())
    {
        return false;
    }
    
    if (Connection.SourceCharacterID == Connection.TargetCharacterID)
    {
        return false;
    }
    
    return true;
}

float UMingGoRTSRelationshipNetwork::CalculateRelationshipWeight(const FRelationshipConnection& Connection) const
{
    float TypeWeight = GetRelationshipTypeWeight(Connection.RelationshipType);
    float StrengthWeight = GetRelationshipStrengthWeight(Connection.Strength);
    float ValueWeight = Connection.RelationshipValue / 100.0f;
    
    return TypeWeight * StrengthWeight * ValueWeight;
}

bool UMingGoRTSRelationshipNetwork::CheckRelationshipConflict(const FString& SourceID, const FString& TargetID, ERelationshipType NewType) const
{
    FString RelationshipID = GenerateRelationshipID(SourceID, TargetID);
    
    if (!RelationshipMap.Contains(RelationshipID))
    {
        return false;
    }

    const FRelationshipConnection& ExistingConnection = RelationshipMap[RelationshipID];
    
    // 檢查衝突的關係類型
    if (ExistingConnection.RelationshipType == ERelationshipType::Enemy && NewType == ERelationshipType::Friend)
    {
        return true;
    }
    
    if (ExistingConnection.RelationshipType == ERelationshipType::Rival && NewType == ERelationshipType::Ally)
    {
        return true;
    }
    
    return false;
}

void UMingGoRTSRelationshipNetwork::ResolveRelationshipConflict(const FString& SourceID, const FString& TargetID, ERelationshipType NewType)
{
    FString RelationshipID = GenerateRelationshipID(SourceID, TargetID);
    
    if (RelationshipMap.Contains(RelationshipID))
    {
        FRelationshipConnection& Connection = RelationshipMap[RelationshipID];
        Connection.RelationshipType = NewType;
        Connection.Status = ERelationshipStatus::Strained;
        
        UE_LOG(LogTemp, Log, TEXT("解決關係衝突：%s -> %s，新類型：%s"), *SourceID, *TargetID, *StaticEnum<ERelationshipType>()->GetValueAsString(NewType));
    }
}

void UMingGoRTSRelationshipNetwork::UpdateRelationshipStatus(const FString& SourceID, const FString& TargetID)
{
    FString RelationshipID = GenerateRelationshipID(SourceID, TargetID);
    
    if (!RelationshipMap.Contains(RelationshipID))
    {
        return;
    }

    FRelationshipConnection& Connection = RelationshipMap[RelationshipID];
    
    // 根據關係值更新狀態
    if (Connection.RelationshipValue < 20.0f)
    {
        Connection.Status = ERelationshipStatus::Broken;
    }
    else if (Connection.RelationshipValue < 40.0f)
    {
        Connection.Status = ERelationshipStatus::Strained;
    }
    else if (Connection.RelationshipValue < 60.0f)
    {
        Connection.Status = ERelationshipStatus::Dormant;
    }
    else if (Connection.RelationshipValue > 80.0f)
    {
        Connection.Status = ERelationshipStatus::Improving;
    }
    else
    {
        Connection.Status = ERelationshipStatus::Active;
    }
}

float UMingGoRTSRelationshipNetwork::CalculateTrustLevel(const FString& SourceID, const FString& TargetID) const
{
    // 簡化實現：基於關係類型和強度計算信任度
    FRelationshipConnection Connection = GetRelationship(SourceID, TargetID);
    
    if (Connection.SourceCharacterID.IsEmpty())
    {
        return 50.0f; // 默認信任度
    }
    
    float BaseTrust = 50.0f;
    
    // 根據關係類型調整
    switch (Connection.RelationshipType)
    {
    case ERelationshipType::Family:
        BaseTrust = 85.0f;
        break;
    case ERelationshipType::Friend:
        BaseTrust = 75.0f;
        break;
    case ERelationshipType::Ally:
        BaseTrust = 70.0f;
        break;
    case ERelationshipType::Enemy:
        BaseTrust = 10.0f;
        break;
    case ERelationshipType::Rival:
        BaseTrust = 25.0f;
        break;
    default:
        BaseTrust = 50.0f;
        break;
    }
    
    // 根據關係強度調整
    float StrengthModifier = GetRelationshipStrengthWeight(Connection.Strength) - 0.5f;
    BaseTrust += StrengthModifier * 50.0f;
    
    return FMath::Clamp(BaseTrust, 0.0f, 100.0f);
}

float UMingGoRTSRelationshipNetwork::CalculateInfluenceLevel(const FString& SourceID, const FString& TargetID) const
{
    // 簡化實現：基於關係值和信任度計算影響度
    FRelationshipConnection Connection = GetRelationship(SourceID, TargetID);
    
    if (Connection.SourceCharacterID.IsEmpty())
    {
        return 50.0f; // 默認影響度
    }
    
    float BaseInfluence = Connection.RelationshipValue;
    float TrustBonus = Connection.TrustLevel * 0.3f;
    
    return FMath::Clamp(BaseInfluence + TrustBonus, 0.0f, 100.0f);
}

float UMingGoRTSRelationshipNetwork::GetRelationshipTypeWeight(ERelationshipType Type) const
{
    switch (Type)
    {
    case ERelationshipType::Family:
        return 1.0f;
    case ERelationshipType::Romantic:
        return 0.95f;
    case ERelationshipType::Friend:
        return 0.9f;
    case ERelationshipType::Ally:
        return 0.85f;
    case ERelationshipType::Mentor:
        return 0.8f;
    case ERelationshipType::Colleague:
        return 0.7f;
    case ERelationshipType::Business:
        return 0.6f;
    case ERelationshipType::Political:
        return 0.65f;
    case ERelationshipType::Military:
        return 0.7f;
    case ERelationshipType::Subordinate:
        return 0.5f;
    case ERelationshipType::Superior:
        return 0.55f;
    case ERelationshipType::Rival:
        return 0.3f;
    case ERelationshipType::Enemy:
        return 0.1f;
    case ERelationshipType::Acquaintance:
        return 0.4f;
    case ERelationshipType::Stranger:
        return 0.2f;
    default:
        return 0.5f;
    }
}

float UMingGoRTSRelationshipNetwork::GetRelationshipStrengthWeight(ERelationshipStrength Strength) const
{
    switch (Strength)
    {
    case ERelationshipStrength::VeryWeak:
        return 0.1f;
    case ERelationshipStrength::Weak:
        return 0.3f;
    case ERelationshipStrength::Moderate:
        return 0.5f;
    case ERelationshipStrength::Strong:
        return 0.8f;
    case ERelationshipStrength::VeryStrong:
        return 1.0f;
    default:
        return 0.5f;
    }
}

bool UMingGoRTSRelationshipNetwork::IsRelationshipSymmetric(ERelationshipType Type) const
{
    switch (Type)
    {
    case ERelationshipType::Family:
    case ERelationshipType::Friend:
    case ERelationshipType::Ally:
    case ERelationshipType::Rival:
    case ERelationshipType::Enemy:
    case ERelationshipType::Colleague:
    case ERelationshipType::Acquaintance:
        return true;
    case ERelationshipType::Mentor:
    case ERelationshipType::Subordinate:
    case ERelationshipType::Superior:
    case ERelationshipType::Business:
    case ERelationshipType::Political:
    case ERelationshipType::Military:
    case ERelationshipType::Romantic:
        return false;
    default:
        return true;
    }
}

void UMingGoRTSRelationshipNetwork::CreateSymmetricRelationship(const FString& SourceID, const FString& TargetID, ERelationshipType Type, ERelationshipStrength Strength)
{
    FString ReverseRelationshipID = GenerateRelationshipID(TargetID, SourceID);
    
    if (!RelationshipMap.Contains(ReverseRelationshipID))
    {
        FRelationshipConnection ReverseConnection;
        ReverseConnection.SourceCharacterID = TargetID;
        ReverseConnection.TargetCharacterID = SourceID;
        ReverseConnection.RelationshipType = Type;
        ReverseConnection.Strength = Strength;
        ReverseConnection.Status = ERelationshipStatus::Active;
        ReverseConnection.RelationshipValue = GetRelationshipStrengthWeight(Strength) * 100.0f;
        ReverseConnection.TrustLevel = CalculateTrustLevel(TargetID, SourceID);
        ReverseConnection.InfluenceLevel = CalculateInfluenceLevel(TargetID, SourceID);
        ReverseConnection.bIsReciprocal = true;
        ReverseConnection.LastInteractionDate = FDateTime::Now().ToString();
        ReverseConnection.InteractionFrequency = 1.0f;
        
        RelationshipMap.Add(ReverseRelationshipID, ReverseConnection);
    }
}

void UMingGoRTSRelationshipNetwork::RecordRelationshipChange(const FString& SourceID, const FString& TargetID, const FString& ChangeType, float OldValue, float NewValue)
{
    FRelationshipEvent NewEvent;
    NewEvent.EventID = FString::Printf(TEXT("Change_%s_%s"), *SourceID, *TargetID);
    NewEvent.EventName = FString::Printf(TEXT("關係變化：%s"), *ChangeType);
    NewEvent.Description = FString::Printf(TEXT("%s -> %s 的關係從 %.1f 變為 %.1f"), *SourceID, *TargetID, OldValue, NewValue);
    NewEvent.InvolvedCharacters.Add(SourceID);
    NewEvent.InvolvedCharacters.Add(TargetID);
    NewEvent.EventType = TEXT("RelationshipChange");
    NewEvent.EventDate = FDateTime::Now().ToString();
    NewEvent.EventImpact = FMath::Abs(NewValue - OldValue);
    NewEvent.bIsPositive = NewValue > OldValue;
    NewEvent.bIsHistorical = false;
    
    RelationshipHistory.Add(NewEvent);
}

void UMingGoRTSRelationshipNetwork::AnalyzeRelationshipPatterns(const FString& NetworkID)
{
    if (!NetworkMap.Contains(NetworkID))
    {
        return;
    }

    const FRelationshipNetwork& Network = NetworkMap[NetworkID];
    
    // 分析關係模式
    TMap<ERelationshipType, int32> TypeCount;
    TMap<ERelationshipStrength, int32> StrengthCount;
    
    for (const FRelationshipConnection& Connection : Network.Connections)
    {
        TypeCount.FindOrAdd(Connection.RelationshipType)++;
        StrengthCount.FindOrAdd(Connection.Strength)++;
    }
    
    // 這裡可以添加更複雜的模式分析
    UE_LOG(LogTemp, Log, TEXT("分析網絡 %s 的關係模式"), *NetworkID);
}

void UMingGoRTSRelationshipNetwork::PredictRelationshipDevelopment(const FString& SourceID, const FString& TargetID)
{
    FRelationshipConnection Connection = GetRelationship(SourceID, TargetID);
    
    if (Connection.SourceCharacterID.IsEmpty())
    {
        return;
    }
    
    // 簡化實現：基於當前趨勢預測發展
    float CurrentTrend = 0.0f;
    if (Connection.InteractionFrequency > 5.0f)
    {
        CurrentTrend = 5.0f; // 關係在改善
    }
    else if (Connection.InteractionFrequency < 1.0f)
    {
        CurrentTrend = -5.0f; // 關係在惡化
    }
    
    float PredictedValue = Connection.RelationshipValue + CurrentTrend;
    UE_LOG(LogTemp, Log, TEXT("預測關係 %s -> %s 的發展：%.1f"), *SourceID, *TargetID, PredictedValue);
}

FString UMingGoRTSRelationshipNetwork::GenerateRelationshipReport(const FString& NetworkID) const
{
    FString Report = FString::Printf(TEXT("關係網絡報告：%s\n"), *NetworkID);
    
    if (NetworkMap.Contains(NetworkID))
    {
        const FRelationshipNetwork& Network = NetworkMap[NetworkID];
        
        Report += FString::Printf(TEXT("網絡名稱：%s\n"), *Network.NetworkName);
        Report += FString::Printf(TEXT("角色數量：%d\n"), Network.CharacterIDs.Num());
        Report += FString::Printf(TEXT("連接數量：%d\n"), Network.Connections.Num());
        Report += FString::Printf(TEXT("網絡密度：%.2f\n"), Network.NetworkDensity);
        Report += FString::Printf(TEXT("網絡凝聚力：%.2f\n"), Network.NetworkCohesion);
        Report += FString::Printf(TEXT("中心角色：%s\n"), *Network.CentralCharacterID);
        Report += FString::Printf(TEXT("關鍵影響者數量：%d\n"), Network.KeyInfluencers.Num());
        Report += FString::Printf(TEXT("橋接角色數量：%d\n"), Network.BridgingCharacters.Num());
        Report += FString::Printf(TEXT("網絡狀態：%s\n"), Network.bIsStable ? TEXT("穩定") : TEXT("不穩定"));
        Report += FString::Printf(TEXT("發展趨勢：%s\n"), Network.bIsGrowing ? TEXT("增長") : TEXT("萎縮"));
    }
    
    return Report;
}
