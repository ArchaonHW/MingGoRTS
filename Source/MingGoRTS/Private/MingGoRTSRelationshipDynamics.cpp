#include "MingGoRTSRelationshipDynamics.h"
#include "MingGoRTSRelationshipNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UMingGoRTSRelationshipDynamics::UMingGoRTSRelationshipDynamics()
{
    BaseDecayRate = 0.01f; // 基礎衰減率
    InfluenceDecayRate = 0.05f; // 影響衰減率
    bIsInitialized = false;
}

void UMingGoRTSRelationshipDynamics::InitializeRelationshipDynamics()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("關係動態系統已經初始化"));
        return;
    }

    // 獲取關係網絡系統引用
    // TODO: 從遊戲實例獲取關係網絡系統

    bIsInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("關係動態系統初始化完成"));
}

bool UMingGoRTSRelationshipDynamics::ApplyRelationshipChange(const FString& SourceID, const FString& TargetID, ERelationshipChangeType ChangeType, float ChangeValue, const FString& Reason)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("關係動態系統未初始化"));
        return false;
    }

    // 創建關係變化
    FRelationshipChange NewChange;
    NewChange.ChangeID = GenerateChangeID(SourceID, TargetID);
    NewChange.SourceCharacterID = SourceID;
    NewChange.TargetCharacterID = TargetID;
    NewChange.ChangeType = ChangeType;
    NewChange.ChangeReason = Reason;
    NewChange.ChangeTime = FDateTime::Now();
    NewChange.ChangeMagnitude = FMath::Abs(ChangeValue);
    NewChange.bPropagates = (ChangeType != ERelationshipChangeType::Stabilization);

    // 獲取當前關係值
    if (RelationshipNetwork)
    {
        FRelationshipConnection CurrentConnection = RelationshipNetwork->GetRelationship(SourceID, TargetID);
        NewChange.OldValue = CurrentConnection.RelationshipValue;
        NewChange.NewValue = FMath::Clamp(CurrentConnection.RelationshipValue + ChangeValue, 0.0f, 100.0f);
    }
    else
    {
        NewChange.OldValue = 50.0f; // 默認認值
        NewChange.NewValue = FMath::Clamp(50.0f + ChangeValue, 0.0f, 100.0f);
    }

    // 處理關係變化
    ProcessRelationshipChange(NewChange);

    // 添加到歷史記錄
    RelationshipChanges.Add(NewChange);

    // 廣播關係變化事件
    OnRelationshipChanged.Broadcast(SourceID, TargetID, ChangeType);

    UE_LOG(LogTemp, Log, TEXT("應用關係變化：%s -> %s，類型：%s，變化：%.1f"), *SourceID, *TargetID, *StaticEnum<ERelationshipChangeType>()->GetValueAsString(ChangeType), ChangeValue);
    return true;
}

FString UMingGoRTSRelationshipDynamics::PropagateInfluence(const FString& SourceID, const FString& InfluenceType, float InfluenceValue, EInfluencePropagationType PropagationType, int32 MaxDepth)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("關係動態系統未初始化"));
        return TEXT("");
    }

    FString PropagationID = GeneratePropagationID(SourceID, InfluenceType);
    
    FRelationshipInfluencePropagation NewPropagation;
    NewPropagation.PropagationID = PropagationID;
    NewPropagation.SourceCharacterID = SourceID;
    NewPropagation.InfluenceType = InfluenceType;
    NewPropagation.PropagationType = PropagationType;
    NewPropagation.InitialInfluence = InfluenceValue;
    NewPropagation.PropagationStrength = 1.0f;
    NewPropagation.MaxDepth = MaxDepth;
    NewPropagation.AttenuationFactor = 0.5f;
    NewPropagation.StartTime = FDateTime::Now();
    NewPropagation.EndTime = FDateTime::Now() + FTimespan::FromHours(1.0);
    NewPropagation.bIsActive = true;

    // 執行影響傳播
    ExecuteInfluencePropagation(NewPropagation);

    // 添加到活動傳播
    ActivePropagations.Add(NewPropagation);

    // 廣播傳播事件
    OnInfluencePropagated.Broadcast(PropagationID, NewPropagation.AffectedCharacters);

    UE_LOG(LogTemp, Log, TEXT("開始影響傳播：%s，類型：%s，強度：%.1f"), *SourceID, *InfluenceType, InfluenceValue);
    return PropagationID;
}

void UMingGoRTSRelationshipDynamics::AnalyzeNetworkStability(const FString& NetworkID)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("關係動態系統未初始化"));
        return;
    }

    FRelationshipStabilityAnalysis Analysis;
    Analysis.NetworkID = NetworkID;
    Analysis.AnalysisTime = FDateTime::Now();

    if (RelationshipNetwork)
    {
        FRelationshipNetwork Network = RelationshipNetwork->GetNetwork(NetworkID);
        
        // 計算網絡密度和凝聚力
        Analysis.StabilityScore = (Network.NetworkDensity + Network.NetworkCohesion) / 2.0f;
        
        // 計算角色穩定性
        for (const FString& CharacterID : Network.CharacterIDs)
        {
            TArray<FRelationshipConnection> Relationships = RelationshipNetwork->GetCharacterRelationships(CharacterID);
            float CharacterStability = 0.0f;
            
            for (const FRelationshipConnection& Connection : Relationships)
            {
                CharacterStability += Connection.RelationshipValue;
            }
            
            if (Relationships.Num() > 0)
            {
                CharacterStability /= Relationships.Num();
            }
            
            Analysis.CharacterStability.Add(CharacterID, CharacterStability);
            
            // 檢查不穩定關係
            if (CharacterStability < 30.0f)
            {
                Analysis.UnstableRelationships.Add(CharacterID);
            }
        }
        
        // 確定整體穩定性
        if (Analysis.StabilityScore > 0.8f)
        {
            Analysis.OverallStability = ERelationshipStability::VeryStable;
        }
        else if (Analysis.StabilityScore > 0.6f)
        {
            Analysis.OverallStability = ERelationshipStability::Stable;
        }
        else if (Analysis.StabilityScore > 0.4f)
        {
            Analysis.OverallStability = ERelationshipStability::Moderate;
        }
        else if (Analysis.StabilityScore > 0.2f)
        {
            Analysis.OverallStability = ERelationshipStability::Unstable;
        }
        else
        {
            Analysis.OverallStability = ERelationshipStability::VeryUnstable;
        }
        
        // 預測未來穩定性
        Analysis.PredictedStability = PredictNetworkChanges(NetworkID);
    }

    // 保存分析結果
    StabilityAnalyses.Add(NetworkID, Analysis);

    // 廣播分析完成事件
    OnStabilityAnalysisComplete.Broadcast(NetworkID, Analysis);

    UE_LOG(LogTemp, Log, TEXT("網絡穩定性分析完成：%s，穩定性：%.2f"), *NetworkID, Analysis.StabilityScore);
}

TArray<FRelationshipChange> UMingGoRTSRelationshipDynamics::GetRelationshipChangeHistory(const FString& SourceID, const FString& TargetID) const
{
    TArray<FRelationshipChange> History;
    
    for (const FRelationshipChange& Change : RelationshipChanges)
    {
        if (Change.SourceCharacterID == SourceID && Change.TargetCharacterID == TargetID)
        {
            History.Add(Change);
        }
    }
    
    return History;
}

TArray<FRelationshipInfluencePropagation> UMingGoRTSRelationshipDynamics::GetActivePropagations() const
{
    TArray<FRelationshipInfluencePropagation> ActiveOnes;
    
    for (const FRelationshipInfluencePropagation& Propagation : ActivePropagations)
    {
        if (Propagation.bIsActive)
        {
            ActiveOnes.Add(Propagation);
        }
    }
    
    return ActiveOnes;
}

FRelationshipStabilityAnalysis UMingGoRTSRelationshipDynamics::GetStabilityAnalysis(const FString& NetworkID) const
{
    if (const FRelationshipStabilityAnalysis* Analysis = StabilityAnalyses.Find(NetworkID))
    {
        return *Analysis;
    }
    
    return FRelationshipStabilityAnalysis();
}

TArray<float> UMingGoRTSRelationshipDynamics::PredictRelationshipTrend(const FString& SourceID, const FString& TargetID, int32 TimeSteps) const
{
    TArray<float> Trend;
    
    // 獲取歷史變化
    TArray<FRelationshipChange> History = GetRelationshipChangeHistory(SourceID, TargetID);
    
    if (History.Num() < 2)
    {
        // 如果沒有足夠的歷史數據，返回平穩趨勢
        for (int32 i = 0; i < TimeSteps; ++i)
        {
            Trend.Add(50.0f); // 默認值
        }
        return Trend;
    }
    
    // 計算趨勢
    float CurrentValue = History.Last().NewValue;
    float TrendSlope = 0.0f;
    
    if (History.Num() >= 3)
    {
        // 使用最近3個變化計算趨勢
        TArray<float> RecentValues;
        for (int32 i = History.Num() - 3; i < History.Num(); ++i)
        {
            RecentValues.Add(History[i].NewValue);
        }
        
        TrendSlope = CalculateTrendSlope(RecentValues);
    }
    
    // 預測未來值
    for (int32 i = 0; i < TimeSteps; ++i)
    {
        float PredictedValue = CurrentValue + (TrendSlope * (i + 1));
        PredictedValue = FMath::Clamp(PredictedValue, 0.0f, 100.0f);
        Trend.Add(PredictedValue);
    }
    
    return Trend;
}

float UMingGoRTSRelationshipDynamics::CalculateRelationshipDecay(const FString& SourceID, const FString& TargetID, float TimeElapsed) const
{
    // 獲取關係變化頻率
    float ChangeFrequency = GetRelationshipChangeFrequency(SourceID, TargetID);
    
    // 計算衰減率
    float DecayRate = BaseDecayRate;
    
    // 如果關係變化頻繁，衰減率較低
    if (ChangeFrequency > 0.1f)
    {
        DecayRate *= 0.5f;
    }
    
    // 計算衰減
    float DecayAmount = DecayRate * TimeElapsed;
    
    return FMath::Max(0.0f, DecayAmount);
}

void UMingGoRTSRelationshipDynamics::ApplyTimeDecay(float DeltaTime)
{
    if (!bIsInitialized || !RelationshipNetwork)
    {
        return;
    }

    // 清理過期的傳播
    CleanupExpiredPropagations();

    // 應用時間衰減到所有關係
    TArray<FRelationshipNetwork> AllNetworks = RelationshipNetwork->GetAllNetworks();
    
    for (const FRelationshipNetwork& Network : AllNetworks)
    {
        for (const FRelationshipConnection& Connection : Network.Connections)
        {
            float DecayAmount = CalculateRelationshipDecay(Connection.SourceCharacterID, Connection.TargetCharacterID, DeltaTime);
            
            if (DecayAmount > 0.0f)
            {
                float NewValue = FMath::Max(0.0f, Connection.RelationshipValue - DecayAmount);
                RelationshipNetwork->UpdateRelationship(Connection.SourceCharacterID, Connection.TargetCharacterID, NewValue, TEXT("時間衰減"));
            }
        }
    }
    
    // 更新統計數據
    UpdateStatistics();
}

TArray<FString> UMingGoRTSRelationshipDynamics::CheckRelationshipConflicts(const FString& NetworkID) const
{
    TArray<FString> Conflicts;
    
    if (!RelationshipNetwork)
    {
        return Conflicts;
    }

    FRelationshipNetwork Network = RelationshipNetwork->GetNetwork(NetworkID);
    
    // 檢查每個角色的關係衝突
    for (const FString& CharacterID : Network.CharacterIDs)
    {
        TArray<FRelationshipConnection> Relationships = RelationshipNetwork->GetCharacterRelationships(CharacterID);
        
        // 檢查是否有衝突的關係類型
        for (const FRelationshipConnection& Connection : Relationships)
        {
            // 檢查是否有敵對關係同時存在友好關係
            if (Connection.RelationshipType == ERelationshipType::Enemy)
            {
                for (const FRelationshipConnection& OtherConnection : Relationships)
                {
                    if (OtherConnection.TargetCharacterID == Connection.TargetCharacterID && 
                        OtherConnection.RelationshipType == ERelationshipType::Friend)
                    {
                        FString Conflict = FString::Printf(TEXT("%s 與 %s 同時存在敵對和友好關係"), *CharacterID, *Connection.TargetCharacterID);
                        Conflicts.Add(Conflict);
                    }
                }
            }
        }
    }
    
    return Conflicts;
}

bool UMingGoRTSRelationshipDynamics::ResolveRelationshipConflict(const FString& SourceID, const FString& TargetID)
{
    if (!RelationshipNetwork)
    {
        return false;
    }

    FRelationshipConnection Connection = RelationshipNetwork->GetRelationship(SourceID, TargetID);
    
    if (Connection.SourceCharacterID.IsEmpty())
    {
        return false;
    }

    // 解決衝突：選擇關係值較高的關係類型
    if (Connection.RelationshipValue > 50.0f)
    {
        // 保持正面關係
        if (Connection.RelationshipType == ERelationshipType::Enemy)
        {
            // 轉變為競爭關係
            RelationshipNetwork->UpdateRelationship(SourceID, TargetID, Connection.RelationshipValue, TEXT("解決衝突：轉變為競爭關係"));
        }
    }
    else
    {
        // 保持負面關係
        if (Connection.RelationshipType == ERelationshipType::Friend)
        {
            // 轉變為熟人關係
            RelationshipNetwork->UpdateRelationship(SourceID, TargetID, Connection.RelationshipValue, TEXT("解決衝突：轉變為熟人關係"));
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("解決關係衝突：%s -> %s"), *SourceID, *TargetID);
    return true;
}

TMap<FString, float> UMingGoRTSRelationshipDynamics::CalculateNetworkDynamics(const FString& NetworkID) const
{
    TMap<FString, float> Dynamics;
    
    if (!RelationshipNetwork)
    {
        return Dynamics;
    }

    FRelationshipNetwork Network = RelationshipNetwork->GetNetwork(NetworkID);
    
    // 計算各種動態指標
    Dynamics.Add(TEXT("ChangeFrequency"), 0.0f);
    Dynamics.Add(TEXT("InfluenceStrength"), 0.0f);
    Dynamics.Add(TEXT("StabilityIndex"), 0.0f);
    Dynamics.Add(TEXT("ResilienceIndex"), 0.0f);
    
    // 計算變化頻率
    float TotalChangeFrequency = 0.0f;
    int32 RelationshipCount = 0;
    
    for (const FRelationshipConnection& Connection : Network.Connections)
    {
        float ChangeFrequency = GetRelationshipChangeFrequency(Connection.SourceCharacterID, Connection.TargetCharacterID);
        TotalChangeFrequency += ChangeFrequency;
        RelationshipCount++;
    }
    
    if (RelationshipCount > 0)
    {
        Dynamics[TEXT("ChangeFrequency")] = TotalChangeFrequency / RelationshipCount;
    }
    
    // 計算影響力強度
    float TotalInfluence = 0.0f;
    
    for (const FString& CharacterID : Network.CharacterIDs)
    {
        float Influence = RelationshipNetwork->CalculateInfluence(CharacterID);
        TotalInfluence += Influence;
    }
    
    if (Network.CharacterIDs.Num() > 0)
    {
        Dynamics[TEXT("InfluenceStrength")] = TotalInfluence / Network.CharacterIDs.Num();
    }
    
    // 計算穩定性指數
    if (StabilityAnalyses.Contains(NetworkID))
    {
        const FRelationshipStabilityAnalysis& Analysis = StabilityAnalyses[NetworkID];
        Dynamics[TEXT("StabilityIndex")] = Analysis.StabilityScore;
    }
    
    // 計算韌性指數
    Dynamics[TEXT("ResilienceIndex")] = CalculateNetworkResilienceIndex(NetworkID);
    
    return Dynamics;
}

TArray<FString> UMingGoRTSRelationshipDynamics::GetRelationshipChangeSuggestions(const FString& CharacterID) const
{
    TArray<FString> Suggestions;
    
    if (!RelationshipNetwork)
    {
        return Suggestions;
    }

    // 獲取角色的所有關係
    TArray<FRelationshipConnection> Relationships = RelationshipNetwork->GetCharacterRelationships(CharacterID);
    
    // 分析關係狀況並提供建議
    for (const FRelationshipConnection& Connection : Relationships)
    {
        if (Connection.RelationshipValue < 30.0f)
        {
            Suggestions.Add(FString::Printf(TEXT("建議改善與 %s 的關係"), *Connection.TargetCharacterID));
        }
        else if (Connection.RelationshipValue > 80.0f)
        {
            Suggestions.Add(FString::Printf(TEXT("與 %s 的關係良好，可以加強合作"), *Connection.TargetCharacterID));
        }
        
        // 檢查互動頻率
        if (Connection.InteractionFrequency < 1.0f)
        {
            Suggestions.Add(FString::Printf(TEXT("建議增加與 %s 的互動"), *Connection.TargetCharacterID));
        }
    }
    
    // 檢查潛在的新關係
    TArray<FString> IndirectConnections = RelationshipNetwork->GetIndirectConnections(CharacterID);
    for (const FString& IndirectID : IndirectConnections)
    {
        Suggestions.Add(FString::Printf(TEXT("考慮與 %s 建立直接關係"), *IndirectID));
    }
    
    return Suggestions;
}

void UMingGoRTSRelationshipDynamics::SimulateRelationshipDevelopment(const FString& NetworkID, int32 SimulationSteps)
{
    if (!bIsInitialized || !RelationshipNetwork)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("開始模擬關係發展：%s，步數：%d"), *NetworkID, SimulationSteps);
    
    for (int32 Step = 0; Step < SimulationSteps; ++Step)
    {
        // 模擬隨機關係變化
        FRelationshipNetwork Network = RelationshipNetwork->GetNetwork(NetworkID);
        
        for (const FRelationshipConnection& Connection : Network.Connections)
        {
            // 隨機決定是否發生變化
            if (FMath::RandRange(0.0f, 1.0f) < 0.1f) // 10% 概率
            {
                float ChangeValue = FMath::RandRange(-10.0f, 10.0f);
                ERelationshipChangeType ChangeType = (ChangeValue > 0) ? ERelationshipChangeType::Improvement : ERelationshipChangeType::Deterioration;
                
                ApplyRelationshipChange(Connection.SourceCharacterID, Connection.TargetCharacterID, ChangeType, ChangeValue, TEXT("模擬變化"));
            }
        }
        
        // 應用時間衰減
        ApplyTimeDecay(1.0f); // 每步代表1個時間單位
        
        // 檢查並解決衝突
        TArray<FString> Conflicts = CheckRelationshipConflicts(NetworkID);
        for (const FString& Conflict : Conflicts)
        {
            // 簡化處理：提取角色ID
            TArray<FString> Parts;
            Conflict.ParseIntoArray(Parts, TEXT(" "), true);
            if (Parts.Num() >= 4)
            {
                ResolveRelationshipConflict(Parts[0], Parts[2]);
            }
        }
    }
    
    // 分析最終穩定性
    AnalyzeNetworkStability(NetworkID);
    
    UE_LOG(LogTemp, Log, TEXT("關係發展模擬完成：%s"), *NetworkID);
}

float UMingGoRTSRelationshipDynamics::PredictRelationshipStability(const FString& SourceID, const FString& TargetID) const
{
    // 獲取關係變化歷史
    TArray<FRelationshipChange> History = GetRelationshipChangeHistory(SourceID, TargetID);
    
    if (History.Num() < 2)
    {
        return 0.5f; // 默認中等穩定性
    }
    
    // 計算變化幅度
    float ChangeMagnitude = CalculateRelationshipChangeMagnitude(SourceID, TargetID);
    
    // 計算變化頻率
    float ChangeFrequency = GetRelationshipChangeFrequency(SourceID, TargetID);
    
    // 計算恢復力
    float Resilience = CalculateRelationshipResilience(SourceID, TargetID);
    
    // 綜合計算穩定性
    float Stability = (1.0f - ChangeMagnitude) * (1.0f - ChangeFrequency) * Resilience;
    
    return FMath::Clamp(Stability, 0.0f, 1.0f);
}

void UMingGoRTSRelationshipDynamics::SetDecayParameters(float BaseDecay, float InfluenceDecay)
{
    BaseDecayRate = BaseDecay;
    InfluenceDecayRate = InfluenceDecay;
    
    UE_LOG(LogTemp, Log, TEXT("設置衰減參數：基礎衰減率=%.3f，影響衰減率=%.3f"), BaseDecay, InfluenceDecay);
}

TMap<FString, float> UMingGoRTSRelationshipDynamics::GetDynamicsStatistics() const
{
    TMap<FString, float> Statistics;
    
    // 基本統計
    Statistics.Add(TEXT("TotalChanges"), static_cast<float>(RelationshipChanges.Num()));
    Statistics.Add(TEXT("ActivePropagations"), static_cast<float>(ActivePropagations.Num()));
    Statistics.Add(TEXT("StabilityAnalyses"), static_cast<float>(StabilityAnalyses.Num()));
    
    // 衰減參數
    Statistics.Add(TEXT("BaseDecayRate"), BaseDecayRate);
    Statistics.Add(TEXT("InfluenceDecayRate"), InfluenceDecayRate);
    
    // 變化統計
    float TotalChangeMagnitude = 0.0f;
    for (const FRelationshipChange& Change : RelationshipChanges)
    {
        TotalChangeMagnitude += Change.ChangeMagnitude;
    }
    
    if (RelationshipChanges.Num() > 0)
    {
        Statistics.Add(TEXT("AverageChangeMagnitude"), TotalChangeMagnitude / RelationshipChanges.Num());
    }
    
    return Statistics;
}

bool UMingGoRTSRelationshipDynamics::SaveDynamicsData(const FString& SaveSlotName)
{
    // TODO: 實現動態數據保存
    UE_LOG(LogTemp, Log, TEXT("保存動態數據到：%s"), *SaveSlotName);
    return true;
}

bool UMingGoRTSRelationshipDynamics::LoadDynamicsData(const FString& SaveSlotName)
{
    // TODO: 實現動態數據載入
    UE_LOG(LogTemp, Log, TEXT("從 %s 載入動態數據"), *SaveSlotName);
    return true;
}

void UMingGoRTSRelationshipDynamics::ProcessRelationshipChange(const FRelationshipChange& Change)
{
    if (!RelationshipNetwork)
    {
        return;
    }

    // 應用關係變化到網絡
    RelationshipNetwork->UpdateRelationship(Change.SourceCharacterID, Change.TargetCharacterID, Change.NewValue, Change.ChangeReason);
    
    // 如果需要傳播影響
    if (Change.bPropagates)
    {
        EInfluencePropagationType PropagationType = EInfluencePropagationType::Direct;
        
        // 根據變化類型決定傳播類型
        switch (Change.ChangeType)
        {
        case ERelationshipChangeType::Improvement:
            PropagationType = EInfluencePropagationType::Network;
            break;
        case ERelationshipChangeType::Deterioration:
            PropagationType = EInfluencePropagationType::Cascading;
            break;
        case ERelationshipChangeType::Conflict:
            PropagationType = EInfluencePropagationType::Global;
            break;
        default:
            PropagationType = EInfluencePropagationType::Direct;
            break;
        }
        
        PropagateInfluence(Change.SourceCharacterID, Change.ChangeReason, Change.ChangeMagnitude, PropagationType, 2);
    }
    
    // 檢查關係閾值
    CheckRelationshipThreshold(Change.SourceCharacterID, Change.TargetCharacterID);
    
    // 記錄動態事件
    RecordDynamicsEvent(TEXT("RelationshipChange"), FString::Printf(TEXT("%s -> %s: %s"), *Change.SourceCharacterID, *Change.TargetCharacterID, *Change.ChangeReason));
}

void UMingGoRTSRelationshipDynamics::ExecuteInfluencePropagation(FRelationshipInfluencePropagation& Propagation)
{
    if (!RelationshipNetwork)
    {
        return;
    }

    Propagation.AffectedCharacters.Empty();
    Propagation.InfluenceValues.Empty();
    
    // 獲取直接連接
    TArray<FString> DirectConnections = RelationshipNetwork->GetDirectConnections(Propagation.SourceCharacterID);
    
    // 第一層傳播
    for (const FString& TargetID : DirectConnections)
    {
        float Influence = Propagation.InitialInfluence * Propagation.PropagationStrength;
        
        // 應用影響
        ERelationshipChangeType ChangeType = (Influence > 0) ? ERelationshipChangeType::Improvement : ERelationshipChangeType::Deterioration;
        ApplyRelationshipChange(Propagation.SourceCharacterID, TargetID, ChangeType, Influence, FString::Printf(TEXT("影響傳播：%s"), *Propagation.InfluenceType));
        
        Propagation.AffectedCharacters.Add(TargetID);
        Propagation.InfluenceValues.Add(TargetID, Influence);
    }
    
    // 更深層次傳播
    if (Propagation.MaxDepth > 1)
    {
        TArray<FString> CurrentLayer = DirectConnections;
        TArray<FString> NextLayer;
        
        for (int32 Depth = 2; Depth <= Propagation.MaxDepth; ++Depth)
        {
            NextLayer.Empty();
            
            for (const FString& CurrentID : CurrentLayer)
            {
                TArray<FString> Connections = RelationshipNetwork->GetDirectConnections(CurrentID);
                
                for (const FString& NextID : Connections)
                {
                    if (NextID != Propagation.SourceCharacterID && !Propagation.AffectedCharacters.Contains(NextID))
                    {
                        float AttenuatedInfluence = CalculatePropagationDecay(Propagation.InitialInfluence, Depth, Propagation.AttenuationFactor);
                        
                        ERelationshipChangeType ChangeType = (AttenuatedInfluence > 0) ? ERelationshipChangeType::Improvement : ERelationshipChangeType::Deterioration;
                        ApplyRelationshipChange(CurrentID, NextID, ChangeType, AttenuatedInfluence, FString::Printf(TEXT("影響傳播：%s"), *Propagation.InfluenceType));
                        
                        Propagation.AffectedCharacters.Add(NextID);
                        Propagation.InfluenceValues.Add(NextID, AttenuatedInfluence);
                        NextLayer.Add(NextID);
                    }
                }
            }
            
            CurrentLayer = NextLayer;
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("影響傳播完成：%s，影響角色數量：%d"), *Propagation.PropagationID, Propagation.AffectedCharacters.Num());
}

float UMingGoRTSRelationshipDynamics::CalculatePropagationDecay(float InitialValue, int32 Depth, float AttenuationFactor) const
{
    return InitialValue * FMath::Pow(AttenuationFactor, Depth - 1);
}

void UMingGoRTSRelationshipDynamics::UpdateRelationshipStability(const FString& NetworkID)
{
    AnalyzeNetworkStability(NetworkID);
}

void UMingGoRTSRelationshipDynamics::DetectRelationshipPatterns(const FString& NetworkID)
{
    // TODO: 實現關係模式檢測
    UE_LOG(LogTemp, Log, TEXT("檢測網絡 %s 的關係模式"), *NetworkID);
}

void UMingGoRTSRelationshipDynamics::PredictNetworkChanges(const FString& NetworkID)
{
    // TODO: 實現網絡變化預測
    UE_LOG(LogTemp, Log, TEXT("預測網絡 %s 的變化"), *NetworkID);
}

float UMingGoRTSRelationshipDynamics::CalculateRelationshipWeight(const FString& SourceID, const FString& TargetID) const
{
    if (!RelationshipNetwork)
    {
        return 0.5f;
    }

    FRelationshipConnection Connection = RelationshipNetwork->GetRelationship(SourceID, TargetID);
    
    if (Connection.SourceCharacterID.IsEmpty())
    {
        return 0.5f;
    }
    
    return Connection.RelationshipValue / 100.0f;
}

float UMingGoRTSRelationshipDynamics::GetChangeTypeWeight(ERelationshipChangeType ChangeType) const
{
    switch (ChangeType)
    {
    case ERelationshipChangeType::Improvement:
        return 1.0f;
    case ERelationshipChangeType::Deterioration:
        return -1.0f;
    case ERelationshipChangeType::Stabilization:
        return 0.5f;
    case ERelationshipChangeType::Conflict:
        return -0.8f;
    case ERelationshipChangeType::Resolution:
        return 0.7f;
    case ERelationshipChangeType::Transformation:
        return 1.2f;
    default:
        return 0.0f;
    }
}

bool UMingGoRTSRelationshipDynamics::CheckRelationshipThreshold(const FString& SourceID, const FString& TargetID)
{
    if (!RelationshipNetwork)
    {
        return false;
    }

    FRelationshipConnection Connection = RelationshipNetwork->GetRelationship(SourceID, TargetID);
    
    if (Connection.SourceCharacterID.IsEmpty())
    {
        return false;
    }
    
    // 檢查關係值是否超出閾值
    if (Connection.RelationshipValue < 10.0f)
    {
        // 關係破裂，可能需要特殊處理
        UE_LOG(LogTemp, Warning, TEXT("關係 %s -> %s 接近破裂"), *SourceID, *TargetID);
        return true;
    }
    else if (Connection.RelationshipValue > 90.0f)
    {
        // 關係非常強，可能產生強烈影響
        UE_LOG(LogTemp, Log, TEXT("關係 %s -> %s 非常強"), *SourceID, *TargetID);
        return true;
    }
    
    return false;
}

void UMingGoRTSRelationshipDynamics::ApplyRelationshipThreshold(const FString& SourceID, const FString& TargetID)
{
    // TODO: 實現關係閾值應用邏輯
    UE_LOG(LogTemp, Log, TEXT("應用關係閾值：%s -> %s"), *SourceID, *TargetID);
}

float UMingGoRTSRelationshipDynamics::CalculateNetworkInfluence(const FString& NetworkID) const
{
    if (!RelationshipNetwork)
    {
        return 0.0f;
    }

    FRelationshipNetwork Network = RelationshipNetwork->GetNetwork(NetworkID);
    
    float TotalInfluence = 0.0f;
    
    for (const FString& CharacterID : Network.CharacterIDs)
    {
        TotalInfluence += RelationshipNetwork->CalculateInfluence(CharacterID);
    }
    
    return TotalInfluence;
}

void UMingGoRTSRelationshipDynamics::DetectRelationshipChainReactions(const FString& NetworkID)
{
    // TODO: 實現關係連鎖反應檢測
    UE_LOG(LogTemp, Log, TEXT("檢測網絡 %s 的關係連鎖反應"), *NetworkID);
}

void UMingGoRTSRelationshipDynamics::ProcessChainReaction(const TArray<FString>& AffectedCharacters)
{
    // TODO: 實現連鎖反應處理
    UE_LOG(LogTemp, Log, TEXT("處理關係連鎖反應，影響角色數量：%d"), AffectedCharacters.Num());
}

float UMingGoRTSRelationshipDynamics::CalculateRelationshipResilience(const FString& SourceID, const FString& TargetID) const
{
    // 獲取關係變化歷史
    TArray<FRelationshipChange> History = GetRelationshipChangeHistory(SourceID, TargetID);
    
    if (History.Num() < 2)
    {
        return 0.5f; // 默認恢復力
    }
    
    // 計算恢復力基於變化模式
    float RecoveryCount = 0.0f;
    float TotalChanges = static_cast<float>(History.Num());
    
    for (const FRelationshipChange& Change : History)
    {
        if (Change.ChangeType == ERelationshipChangeType::Improvement && Change.OldValue < 50.0f)
        {
            RecoveryCount += 1.0f;
        }
    }
    
    return RecoveryCount / TotalChanges;
}

float UMingGoRTSRelationshipDynamics::PredictRelationshipRecoveryTime(const FString& SourceID, const FString& TargetID) const
{
    float Resilience = CalculateRelationshipResilience(SourceID, TargetID);
    
    // 恢復時間與恢復力成反比
    if (Resilience > 0.0f)
    {
        return 1.0f / Resilience;
    }
    
    return 10.0f; // 默認恢復時間
}

FString UMingGoRTSRelationshipDynamics::GenerateChangeID(const FString& SourceID, const FString& TargetID) const
{
    return FString::Printf(TEXT("Change_%s_%s_%s"), *SourceID, *TargetID, *FDateTime::Now().ToString());
}

FString UMingGoRTSRelationshipDynamics::GeneratePropagationID(const FString& SourceID, const FString& InfluenceType) const
{
    return FString::Printf(TEXT("Propagation_%s_%s_%s"), *SourceID, *InfluenceType, *FDateTime::Now().ToString());
}

void UMingGoRTSRelationshipDynamics::CleanupExpiredPropagations()
{
    FDateTime CurrentTime = FDateTime::Now();
    
    for (int32 i = ActivePropagations.Num() - 1; i >= 0; --i)
    {
        FRelationshipInfluencePropagation& Propagation = ActivePropagations[i];
        
        if (!Propagation.bIsActive || CurrentTime > Propagation.EndTime)
        {
            Propagation.bIsActive = false;
            ActivePropagations.RemoveAt(i);
        }
    }
}

void UMingGoRTSRelationshipDynamics::UpdateStatistics()
{
    // TODO: 實現統計數據更新
    UE_LOG(LogTemp, Log, TEXT("更新關係動態統計數據"));
}

void UMingGoRTSRelationshipDynamics::RecordDynamicsEvent(const FString& EventType, const FString& Description)
{
    // TODO: 實現動態事件記錄
    UE_LOG(LogTemp, Log, TEXT("記錄動態事件：%s - %s"), *EventType, *Description);
}

void UMingGoRTSRelationshipDynamics::AnalyzeRelationshipTrends(const FString& NetworkID)
{
    // TODO: 實現關係趨勢分析
    UE_LOG(LogTemp, Log, TEXT("分析網絡 %s 的關係趨勢"), *NetworkID);
}

float UMingGoRTSRelationshipDynamics::CalculateTrendSlope(const TArray<float>& Values) const
{
    if (Values.Num() < 2)
    {
        return 0.0f;
    }
    
    // 簡化線性回歸計算斜率
    float SumX = 0.0f;
    float SumY = 0.0f;
    float SumXY = 0.0f;
    float SumX2 = 0.0f;
    
    for (int32 i = 0; i < Values.Num(); ++i)
    {
        float X = static_cast<float>(i);
        float Y = Values[i];
        
        SumX += X;
        SumY += Y;
        SumXY += X * Y;
        SumX2 += X * X;
    }
    
    float N = static_cast<float>(Values.Num());
    float Slope = (N * SumXY - SumX * SumY) / (N * SumX2 - SumX * SumX);
    
    return Slope;
}

int32 UMingGoRTSRelationshipDynamics::PredictTrendDirection(const TArray<float>& Values) const
{
    float Slope = CalculateTrendSlope(Values);
    
    if (Slope > 0.1f)
    {
        return 1; // 上升
    }
    else if (Slope < -0.1f)
    {
        return -1; // 下降
    }
    else
    {
        return 0; // 平穩
    }
}

float UMingGoRTSRelationshipDynamics::GetRelationshipChangeFrequency(const FString& SourceID, const FString& TargetID) const
{
    TArray<FRelationshipChange> History = GetRelationshipChangeHistory(SourceID, TargetID);
    
    if (History.Num() < 2)
    {
        return 0.0f;
    }
    
    // 計算時間跨度
    FDateTime FirstTime = History[0].ChangeTime;
    FDateTime LastTime = History.Last().ChangeTime;
    FTimespan TimeSpan = LastTime - FirstTime;
    
    float TimeInHours = static_cast<float>(TimeSpan.GetTotalHours());
    
    if (TimeInHours > 0.0f)
    {
        return static_cast<float>(History.Num()) / TimeInHours;
    }
    
    return 0.0f;
}

float UMingGoRTSRelationshipDynamics::CalculateRelationshipChangeMagnitude(const FString& SourceID, const FString& TargetID) const
{
    TArray<FRelationshipChange> History = GetRelationshipChangeHistory(SourceID, TargetID);
    
    if (History.Num() < 2)
    {
        return 0.0f;
    }
    
    float TotalMagnitude = 0.0f;
    
    for (const FRelationshipChange& Change : History)
    {
        TotalMagnitude += Change.ChangeMagnitude;
    }
    
    return TotalMagnitude / History.Num();
}

bool UMingGoRTSRelationshipDynamics::CheckRelationshipBalance(const FString& NetworkID) const
{
    float Balance = CalculateNetworkBalance(NetworkID);
    return Balance > 0.7f; // 平衡閾值
}

void UMingGoRTSRelationshipDynamics::RestoreRelationshipBalance(const FString& NetworkID)
{
    // TODO: 實現關係平衡恢復
    UE_LOG(LogTemp, Log, TEXT("恢復網絡 %s 的關係平衡"), *NetworkID);
}

float UMingGoRTSRelationshipDynamics::CalculateNetworkBalance(const FString& NetworkID) const
{
    if (!RelationshipNetwork)
    {
        return 0.0f;
    }

    FRelationshipNetwork Network = RelationshipNetwork->GetNetwork(NetworkID);
    
    if (Network.CharacterIDs.Num() < 2)
    {
        return 1.0f; // 單節點網絡是平衡的
    }
    
    float TotalBalance = 0.0f;
    int32 ComparisonCount = 0;
    
    // 計算所有關係值的標準差
    TArray<float> RelationshipValues;
    
    for (const FRelationshipConnection& Connection : Network.Connections)
    {
        RelationshipValues.Add(Connection.RelationshipValue);
    }
    
    if (RelationshipValues.Num() < 2)
    {
        return 1.0f;
    }
    
    // 計算平均值
    float Mean = 0.0f;
    for (float Value : RelationshipValues)
    {
        Mean += Value;
    }
    Mean /= RelationshipValues.Num();
    
    // 計算標準差
    float Variance = 0.0f;
    for (float Value : RelationshipValues)
    {
        Variance += FMath::Square(Value - Mean);
    }
    Variance /= RelationshipValues.Num();
    
    float StandardDeviation = FMath::Sqrt(Variance);
    
    // 平衡度與標準差成反比
    float Balance = 1.0f - (StandardDeviation / 100.0f);
    
    return FMath::Clamp(Balance, 0.0f, 1.0f);
}

TArray<FString> UMingGoRTSRelationshipDynamics::DetectRelationshipAnomalies(const FString& NetworkID) const
{
    TArray<FString> Anomalies;
    
    if (!RelationshipNetwork)
    {
        return Anomalies;
    }

    FRelationshipNetwork Network = RelationshipNetwork->GetNetwork(NetworkID);
    
    for (const FRelationshipConnection& Connection : Network.Connections)
    {
        float AnomalyScore = CalculateAnomalyScore(Connection.SourceCharacterID, Connection.TargetCharacterID);
        
        if (AnomalyScore > 0.8f)
        {
            Anomalies.Add(FString::Printf(TEXT("異常關係：%s -> %s，分數：%.2f"), *Connection.SourceCharacterID, *Connection.TargetCharacterID, AnomalyScore));
        }
    }
    
    return Anomalies;
}

void UMingGoRTSRelationshipDynamics::HandleRelationshipAnomaly(const FString& SourceID, const FString& TargetID)
{
    // TODO: 實現關係異常處理
    UE_LOG(LogTemp, Log, TEXT("處理關係異常：%s -> %s"), *SourceID, *TargetID);
}

float UMingGoRTSRelationshipDynamics::CalculateAnomalyScore(const FString& SourceID, const FString& TargetID) const
{
    // 獲取關係變化歷史
    TArray<FRelationshipChange> History = GetRelationshipChangeHistory(SourceID, TargetID);
    
    if (History.Num() < 3)
    {
        return 0.0f;
    }
    
    // 計算變化幅度和頻率的異常分數
    float ChangeMagnitude = CalculateRelationshipChangeMagnitude(SourceID, TargetID);
    float ChangeFrequency = GetRelationshipChangeFrequency(SourceID, TargetID);
    
    // 獲取當前關係值
    float CurrentValue = 50.0f;
    if (RelationshipNetwork)
    {
        FRelationshipConnection Connection = RelationshipNetwork->GetRelationship(SourceID, TargetID);
        if (!Connection.SourceCharacterID.IsEmpty())
        {
            CurrentValue = Connection.RelationshipValue;
        }
    }
    
    // 計算異常分數
    float AnomalyScore = 0.0f;
    
    // 高變化幅度增加異常分數
    if (ChangeMagnitude > 20.0f)
    {
        AnomalyScore += 0.3f;
    }
    
    // 高變化頻率增加異常分數
    if (ChangeFrequency > 0.5f)
    {
        AnomalyScore += 0.3f;
    }
    
    // 極端關係值增加異常分數
    if (CurrentValue < 10.0f || CurrentValue > 90.0f)
    {
        AnomalyScore += 0.4f;
    }
    
    return FMath::Clamp(AnomalyScore, 0.0f, 1.0f);
}

float UMingGoRTSRelationshipDynamics::PredictRelationshipCollapseRisk(const FString& SourceID, const FString& TargetID) const
{
    // 獲取關係穩定性
    float Stability = PredictRelationshipStability(SourceID, TargetID);
    
    // 獲取異常分數
    float AnomalyScore = CalculateAnomalyScore(SourceID, TargetID);
    
    // 計算崩潰風險
    float CollapseRisk = (1.0f - Stability) * 0.7f + AnomalyScore * 0.3f;
    
    return FMath::Clamp(CollapseRisk, 0.0f, 1.0f);
}

float UMingGoRTSRelationshipDynamics::CalculateNetworkResilienceIndex(const FString& NetworkID) const
{
    if (!RelationshipNetwork)
    {
        return 0.0f;
    }

    FRelationshipNetwork Network = RelationshipNetwork->GetNetwork(NetworkID);
    
    if (Network.CharacterIDs.Num() == 0)
    {
        return 0.0f;
    }
    
    float TotalResilience = 0.0f;
    
    for (const FString& CharacterID : Network.CharacterIDs)
    {
        TArray<FRelationshipConnection> Relationships = RelationshipNetwork->GetCharacterRelationships(CharacterID);
        
        float CharacterResilience = 0.0f;
        for (const FRelationshipConnection& Connection : Relationships)
        {
            CharacterResilience += CalculateRelationshipResilience(Connection.SourceCharacterID, Connection.TargetCharacterID);
        }
        
        if (Relationships.Num() > 0)
        {
            CharacterResilience /= Relationships.Num();
        }
        
        TotalResilience += CharacterResilience;
    }
    
    return TotalResilience / Network.CharacterIDs.Num();
}

TArray<FString> UMingGoRTSRelationshipDynamics::DetectNetworkVulnerabilities(const FString& NetworkID) const
{
    TArray<FString> Vulnerabilities;
    
    if (!RelationshipNetwork)
    {
        return Vulnerabilities;
    }

    FRelationshipNetwork Network = RelationshipNetwork->GetNetwork(NetworkID);
    
    // 檢查每個角色的脆弱性
    for (const FString& CharacterID : Network.CharacterIDs)
    {
        TArray<FRelationshipConnection> Relationships = RelationshipNetwork->GetCharacterRelationships(CharacterID);
        
        // 檢查關係數量
        if (Relationships.Num() < 2)
        {
            Vulnerabilities.Add(FString::Printf(TEXT("孤立角色：%s"), *CharacterID));
        }
        
        // 檢查關係質量
        float AverageRelationship = 0.0f;
        for (const FRelationshipConnection& Connection : Relationships)
        {
            AverageRelationship += Connection.RelationshipValue;
        }
        
        if (Relationships.Num() > 0)
        {
            AverageRelationship /= Relationships.Num();
            
            if (AverageRelationship < 30.0f)
            {
                Vulnerabilities.Add(FString::Printf(TEXT("低質量關係角色：%s"), *CharacterID));
            }
        }
    }
    
    return Vulnerabilities;
}

float UMingGoRTSRelationshipDynamics::CalculateRelationshipContagionRisk(const FString& NetworkID) const
{
    if (!RelationshipNetwork)
    {
        return 0.0f;
    }

    FRelationshipNetwork Network = RelationshipNetwork->GetNetwork(NetworkID);
    
    float NetworkDensity = Network.NetworkDensity;
    float AverageConnection = static_cast<float>(Network.Connections.Num()) / Network.CharacterIDs.Num();
    
    // 傳染風險與網絡密度和平均連接數相關
    float ContagionRisk = NetworkDensity * 0.6f + (AverageConnection / 10.0f) * 0.4f;
    
    return FMath::Clamp(ContagionRisk, 0.0f, 1.0f);
}

void UMingGoRTSRelationshipDynamics::SimulateRelationshipContagion(const FString& NetworkID, const FString& SourceID, float ContagionStrength)
{
    if (!RelationshipNetwork)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("模擬關係傳染：%s，源角色：%s，強度：%.2f"), *NetworkID, *SourceID, ContagionStrength);
    
    // 使用影響傳播機制模擬傳染
    PropagateInfluence(SourceID, TEXT("Contagion"), ContagionStrength, EInfluencePropagationType::Cascading, 3);
}

float UMingGoRTSRelationshipDynamics::CalculateRelationshipIsolationEffect(const FString& NetworkID, const FString& CharacterID) const
{
    if (!RelationshipNetwork)
    {
        return 0.0f;
    }

    // 計算隔離效果基於角色的中心性
    float Influence = RelationshipNetwork->CalculateInfluence(CharacterID);
    float MaxInfluence = 0.0f;
    
    FRelationshipNetwork Network = RelationshipNetwork->GetNetwork(NetworkID);
    for (const FString& CharID : Network.CharacterIDs)
    {
        float CharInfluence = RelationshipNetwork->CalculateInfluence(CharID);
        if (CharInfluence > MaxInfluence)
        {
            MaxInfluence = CharInfluence;
        }
    }
    
    if (MaxInfluence > 0.0f)
    {
        return 1.0f - (Influence / MaxInfluence);
    }
    
    return 0.0f;
}

TArray<FString> UMingGoRTSRelationshipDynamics::PredictRelationshipEvolutionPath(const FString& SourceID, const FString& TargetID) const
{
    TArray<FString> EvolutionPath;
    
    // 獲取關係趨勢
    TArray<float> Trend = PredictRelationshipTrend(SourceID, TargetID, 5);
    
    if (Trend.Num() < 2)
    {
        EvolutionPath.Add(TEXT("無法預測演化路徑"));
        return EvolutionPath;
    }
    
    // 分析趨勢方向
    int32 Direction = PredictTrendDirection(Trend);
    
    if (Direction > 0)
    {
        EvolutionPath.Add(TEXT("關係將改善"));
        EvolutionPath.Add(TEXT("預計趨勢：上升"));
    }
    else if (Direction < 0)
    {
        EvolutionPath.Add(TEXT("關係將惡化"));
        EvolutionPath.Add(TEXT("預計趨勢：下降"));
    }
    else
    {
        EvolutionPath.Add(TEXT("關係將保持穩定"));
        EvolutionPath.Add(TEXT("預計趨勢：平穩"));
    }
    
    // 預測未來狀態
    float PredictedValue = Trend.Last();
    if (PredictedValue > 80.0f)
    {
        EvolutionPath.Add(TEXT("未來可能成為強關係"));
    }
    else if (PredictedValue < 20.0f)
    {
        EvolutionPath.Add(TEXT("未來可能成為弱關係"));
    }
    else
    {
        EvolutionPath.Add(TEXT("未來將維持中等關係"));
    }
    
    return EvolutionPath;
}

TMap<ERelationshipType, float> UMingGoRTSRelationshipDynamics::CalculateRelationshipEvolutionProbabilities(const FString& SourceID, const FString& TargetID) const
{
    TMap<ERelationshipType, float> Probabilities;
    
    // 獲取當前關係
    FRelationshipConnection CurrentConnection;
    if (RelationshipNetwork)
    {
        CurrentConnection = RelationshipNetwork->GetRelationship(SourceID, TargetID);
    }
    
    // 基於當前關係值和趨勢計算演化概率
    float CurrentValue = CurrentConnection.RelationshipValue;
    
    // 簡化實現：基於關係值分配概率
    if (CurrentValue > 70.0f)
    {
        Probabilities.Add(ERelationshipType::Friend, 0.4f);
        Probabilities.Add(ERelationshipType::Ally, 0.3f);
        Probabilities.Add(ERelationshipType::Colleague, 0.2f);
        Probabilities.Add(ERelationshipType::Acquaintance, 0.1f);
    }
    else if (CurrentValue > 40.0f)
    {
        Probabilities.Add(ERelationshipType::Friend, 0.2f);
        Probabilities.Add(ERelationshipType::Colleague, 0.3f);
        Probabilities.Add(ERelationshipType::Acquaintance, 0.3f);
        Probabilities.Add(ERelationshipType::Stranger, 0.2f);
    }
    else
    {
        Probabilities.Add(ERelationshipType::Acquaintance, 0.3f);
        Probabilities.Add(ERelationshipType::Stranger, 0.4f);
        Probabilities.Add(ERelationshipType::Rival, 0.2f);
        Probabilities.Add(ERelationshipType::Enemy, 0.1f);
    }
    
    return Probabilities;
}

FString UMingGoRTSRelationshipDynamics::GenerateRelationshipDevelopmentReport(const FString& NetworkID) const
{
    FString Report = FString::Printf(TEXT("關係發展報告：%s\n"), *NetworkID);
    
    if (StabilityAnalyses.Contains(NetworkID))
    {
        const FRelationshipStabilityAnalysis& Analysis = StabilityAnalyses[NetworkID];
        
        Report += FString::Printf(TEXT("網絡穩定性：%s\n"), *StaticEnum<ERelationshipStability>()->GetValueAsString(Analysis.OverallStability));
        Report += FString::Printf(TEXT("穩定性分數：%.2f\n"), Analysis.StabilityScore);
        Report += FString::Printf(TEXT("預測穩定性：%.2f\n"), Analysis.PredictedStability);
        Report += FString::Printf(TEXT("分析時間：%s\n"), *Analysis.AnalysisTime.ToString());
    }
    
    // 添加動態統計
    TMap<FString, float> Dynamics = GetDynamicsStatistics();
    Report += TEXT("\n動態統計：\n");
    
    for (const auto& StatPair : Dynamics)
    {
        Report += FString::Printf(TEXT("%s：%.2f\n"), *StatPair.Key, StatPair.Value);
    }
    
    return Report;
}
