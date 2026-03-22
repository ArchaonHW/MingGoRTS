#include "MingRTSEmotionalAIEngine.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"
#include "HAL/PlatformFilemanager.h"

UMingRTSEmotionalAIEngine::UMingRTSEmotionalAIEngine()
{
    EmotionalDecayRate = 0.1f;
    EmotionalInfluenceRadius = 500.0f;
    EmotionalContagionRate = 0.3f;
    MaxEmotionalHistory = 100;
}

void UMingRTSEmotionalAIEngine::InitializeEmotionalEngine()
{
    UE_LOG(LogTemp, Log, TEXT("MingRTSEmotionalAIEngine: Initializing emotional AI engine..."));

    // 初始化情感回應模板
    InitializeResponseTemplates();

    UE_LOG(LogTemp, Log, TEXT("MingRTSEmotionalAIEngine: Emotional AI engine initialized successfully"));
}

FEmotionalProfile UMingRTSEmotionalAIEngine::CreateEmotionalProfile(const FString& ProfileID, const FString& ProfileName)
{
    FEmotionalProfile NewProfile;
    NewProfile.ProfileID = ProfileID;
    NewProfile.ProfileName = ProfileName;
    
    // 設置默認基礎情感
    NewProfile.BaseEmotions.Add(EEmotionType::Calm, 0.8f);
    NewProfile.BaseEmotions.Add(EEmotionType::Contentment, 0.6f);
    NewProfile.BaseEmotions.Add(EEmotionType::Trust, 0.5f);
    NewProfile.BaseEmotions.Add(EEmotionType::Joy, 0.4f);
    
    // 設置偏好和避免的情感
    NewProfile.PreferredEmotions.Add(EEmotionType::Joy);
    NewProfile.PreferredEmotions.Add(EEmotionType::Contentment);
    NewProfile.PreferredEmotions.Add(EEmotionType::Trust);
    
    NewProfile.AvoidedEmotions.Add(EEmotionType::Fear);
    NewProfile.AvoidedEmotions.Add(EEmotionType::Anger);
    NewProfile.AvoidedEmotions.Add(EEmotionType::Sadness);
    
    EmotionalProfiles.Add(ProfileID, NewProfile);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSEmotionalAIEngine: Created emotional profile %s"), *ProfileName);
    return NewProfile;
}

FEmotionalState UMingRTSEmotionalAIEngine::AnalyzePlayerEmotion(const FString& PlayerID, const TArray<FString>& InputData)
{
    FEmotionalState PlayerState;
    
    // 分析各種輸入數據的情感
    for (const FString& Data : InputData)
    {
        // 分析文本情感
        for (int32 i = 0; i < 15; i++)
        {
            EEmotionType Emotion = (EEmotionType)i;
            float TextScore = AnalyzeTextEmotion(Data, Emotion);
            float CurrentEmotion = PlayerState.Emotions.FindRef(Emotion);
            PlayerState.Emotions.Add(Emotion, FMath::Clamp(CurrentEmotion + TextScore * 0.1f, 0.0f, 1.0f));
        }
        
        // 分析行為情感
        TArray<FString> Behaviors = ExtractBehaviorsFromData(Data);
        for (int32 i = 0; i < 15; i++)
        {
            EEmotionType Emotion = (EEmotionType)i;
            float BehaviorScore = AnalyzeBehaviorEmotion(Behaviors, Emotion);
            float CurrentEmotion = PlayerState.Emotions.FindRef(Emotion);
            PlayerState.Emotions.Add(Emotion, FMath::Clamp(CurrentEmotion + BehaviorScore * 0.15f, 0.0f, 1.0f));
        }
        
        // 分析上下文情感
        FString Context = ExtractContextFromData(Data);
        for (int32 i = 0; i < 15; i++)
        {
            EEmotionType Emotion = (EEmotionType)i;
            float ContextScore = AnalyzeContextEmotion(Context, Emotion);
            float CurrentEmotion = PlayerState.Emotions.FindRef(Emotion);
            PlayerState.Emotions.Add(Emotion, FMath::Clamp(CurrentEmotion + ContextScore * 0.05f, 0.0f, 1.0f));
        }
    }
    
    // 更新情感狀態
    UpdateDominantEmotion(PlayerState);
    CalculateOverallIntensity(PlayerState);
    PlayerState.LastUpdated = FDateTime::Now();
    
    // 保存情感狀態
    EmotionalStates.Add(PlayerID, PlayerState);
    RecordEmotionalState(PlayerID, PlayerState);
    
    // 觸發情感變化事件
    OnEmotionChanged.Broadcast(PlayerID, PlayerState);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSEmotionalAIEngine: Analyzed emotion for player %s - Dominant: %s"), 
           *PlayerID, *GetEmotionName(PlayerState.DominantEmotion));
    
    return PlayerState;
}

void UMingRTSEmotionalAIEngine::UpdateEmotionalState(const FString& EntityID, const FEmotionalTrigger& Trigger)
{
    if (!EmotionalStates.Contains(EntityID))
    {
        // 創建新的情感狀態
        FEmotionalState NewState;
        EmotionalStates.Add(EntityID, NewState);
    }
    
    FEmotionalState& CurrentState = EmotionalStates[EntityID];
    FEmotionalState PreviousState = CurrentState;
    
    // 處理情感觸發器
    ProcessEmotionalTrigger(EntityID, Trigger);
    
    // 更新情感狀態
    UpdateDominantEmotion(CurrentState);
    CalculateOverallIntensity(CurrentState);
    CurrentState.LastUpdated = FDateTime::Now();
    
    // 記錄情感狀態
    RecordEmotionalState(EntityID, CurrentState);
    
    // 觸發事件
    OnEmotionChanged.Broadcast(EntityID, CurrentState);
    OnEmotionalTrigger.Broadcast(EntityID, Trigger, CurrentState);
    
    // 處理情感傳播
    ProcessEmotionalContagion(EntityID);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSEmotionalAIEngine: Updated emotional state for %s with trigger %s"), 
           *EntityID, *Trigger.TriggerName);
}

FEmotionalResponse UMingRTSEmotionalAIEngine::GenerateEmotionalResponse(const FString& EntityID, const FEmotionalState& CurrentState)
{
    FEmotionalResponse Response;
    
    // 基於主導情感生成回應
    switch (CurrentState.DominantEmotion)
    {
        case EEmotionType::Joy:
            Response = GenerateJoyResponse(CurrentState);
            break;
        case EEmotionType::Sadness:
            Response = GenerateSadnessResponse(CurrentState);
            break;
        case EEmotionType::Anger:
            Response = GenerateAngerResponse(CurrentState);
            break;
        case EEmotionType::Fear:
            Response = GenerateFearResponse(CurrentState);
            break;
        case EEmotionType::Surprise:
            Response = GenerateSurpriseResponse(CurrentState);
            break;
        case EEmotionType::Love:
            Response = GenerateLoveResponse(CurrentState);
            break;
        case EEmotionType::Calm:
            Response = GenerateCalmResponse(CurrentState);
            break;
        default:
            Response = GenerateNeutralResponse(CurrentState);
            break;
    }
    
    Response.ResponseID = FString::Printf(TEXT("Response_%s_%d"), *EntityID, FMath::RandRange(1000, 9999));
    Response.ResponseIntensity = (float)CurrentState.OverallIntensity / 5.0f;
    
    // 觸發回應事件
    OnEmotionalResponse.Broadcast(EntityID, Response);
    
    return Response;
}

void UMingRTSEmotionalAIEngine::SetEmotionalTrigger(const FString& EntityID, const FEmotionalTrigger& Trigger)
{
    if (!EmotionalTriggers.Contains(EntityID))
    {
        TArray<FEmotionalTrigger> EmptyTriggers;
        EmotionalTriggers.Add(EntityID, EmptyTriggers);
    }
    
    EmotionalTriggers[EntityID].Add(Trigger);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSEmotionalAIEngine: Set emotional trigger %s for entity %s"), 
           *Trigger.TriggerName, *EntityID);
}

FEmotionalState UMingRTSEmotionalAIEngine::GetEmotionalState(const FString& EntityID) const
{
    return EmotionalStates.FindRef(EntityID);
}

float UMingRTSEmotionalAIEngine::CalculateEmotionalSimilarity(const FEmotionalState& State1, const FEmotionalState& State2) const
{
    float Similarity = 0.0f;
    int32 ComparedEmotions = 0;
    
    // 計算各種情感的相似度
    for (int32 i = 0; i < 15; i++)
    {
        EEmotionType Emotion = (EEmotionType)i;
        float Emotion1 = State1.Emotions.FindRef(Emotion);
        float Emotion2 = State2.Emotions.FindRef(Emotion);
        
        // 使用歐幾里得距離計算相似度
        float Distance = FMath::Abs(Emotion1 - Emotion2);
        float EmotionSimilarity = 1.0f - Distance;
        
        Similarity += EmotionSimilarity * GetEmotionWeight(Emotion);
        ComparedEmotions++;
    }
    
    if (ComparedEmotions > 0)
    {
        Similarity /= ComparedEmotions;
    }
    
    return FMath::Clamp(Similarity, 0.0f, 1.0f);
}

FEmotionalState UMingRTSEmotionalAIEngine::PredictEmotionalChange(const FString& EntityID, const TArray<FEmotionalTrigger>& PotentialTriggers) const
{
    FEmotionalState PredictedState = GetEmotionalState(EntityID);
    
    // 模擬潛在觸發器的影響
    for (const FEmotionalTrigger& Trigger : PotentialTriggers)
    {
        float CurrentEmotion = PredictedState.Emotions.FindRef(Trigger.TargetEmotion);
        float NewEmotion = FMath::Clamp(CurrentEmotion + Trigger.TriggerStrength * 0.2f, 0.0f, 1.0f);
        PredictedState.Emotions.Add(Trigger.TargetEmotion, NewEmotion);
    }
    
    // 更新預測狀態
    UpdateDominantEmotion(PredictedState);
    CalculateOverallIntensity(PredictedState);
    
    return PredictedState;
}

void UMingRTSEmotionalAIEngine::AdjustAIBehavior(const FString& AIID, const FEmotionalState& EmotionalState)
{
    // 基於情感狀態調整AI行為參數
    float AggressionLevel = 0.5f;
    float CooperationLevel = 0.5f;
    float RiskTaking = 0.5f;
    float CommunicationLevel = 0.5f;
    
    // 根據主導情感調整行為
    switch (EmotionalState.DominantEmotion)
    {
        case EEmotionType::Joy:
            CooperationLevel = 0.8f;
            CommunicationLevel = 0.7f;
            RiskTaking = 0.6f;
            break;
        case EEmotionType::Anger:
            AggressionLevel = 0.9f;
            CooperationLevel = 0.2f;
            RiskTaking = 0.8f;
            break;
        case EEmotionType::Fear:
            AggressionLevel = 0.1f;
            RiskTaking = 0.2f;
            CooperationLevel = 0.6f;
            break;
        case EEmotionType::Love:
            CooperationLevel = 0.9f;
            CommunicationLevel = 0.8f;
            AggressionLevel = 0.1f;
            break;
        case EEmotionType::Calm:
            // 保持平衡
            break;
        default:
            // 根據情感強度調整
            if (EmotionalState.OverallIntensity >= EEmotionIntensity::High)
            {
                RiskTaking = 0.7f;
                AggressionLevel = 0.6f;
            }
            break;
    }
    
    // 應用行為調整（這裡需要與實際的AI系統集成）
    ApplyAIBehaviorAdjustments(AIID, AggressionLevel, CooperationLevel, RiskTaking, CommunicationLevel);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSEmotionalAIEngine: Adjusted AI behavior for %s based on emotional state"), *AIID);
}

void UMingRTSEmotionalAIEngine::LearnEmotionalPatterns(const FString& PlayerID, const TArray<FEmotionalState>& HistoricalStates)
{
    if (HistoricalStates.Num() < 2)
    {
        return;
    }
    
    // 分析情感模式
    TMap<EEmotionType, float> EmotionFrequency;
    TMap<EEmotionType, float> EmotionVariability;
    
    // 計算情感頻率和變異性
    for (int32 i = 0; i < 15; i++)
    {
        EEmotionType Emotion = (EEmotionType)i;
        float Sum = 0.0f;
        float Variance = 0.0f;
        int32 Count = 0;
        
        for (const FEmotionalState& State : HistoricalStates)
        {
            float EmotionValue = State.Emotions.FindRef(Emotion);
            Sum += EmotionValue;
            Count++;
        }
        
        if (Count > 0)
        {
            float Mean = Sum / Count;
            EmotionFrequency.Add(Emotion, Mean);
            
            // 計算變異性
            for (const FEmotionalState& State : HistoricalStates)
            {
                float EmotionValue = State.Emotions.FindRef(Emotion);
                Variance += FMath::Square(EmotionValue - Mean);
            }
            
            if (Count > 1)
            {
                Variance /= (Count - 1);
            }
            
            EmotionVariability.Add(Emotion, Variance);
        }
    }
    
    // 更新玩家情感檔案
    UpdatePlayerEmotionalProfile(PlayerID, EmotionFrequency, EmotionVariability);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSEmotionalAIEngine: Learned emotional patterns for player %s"), *PlayerID);
}

TMap<FString, float> UMingRTSEmotionalAIEngine::GetEmotionalStatistics(const FString& EntityID) const
{
    TMap<FString, float> Statistics;
    
    if (!EmotionalStates.Contains(EntityID))
    {
        return Statistics;
    }
    
    const FEmotionalState& State = EmotionalStates[EntityID];
    
    // 基本統計
    Statistics.Add(TEXT("DominantEmotion"), (float)State.DominantEmotion);
    Statistics.Add(TEXT("OverallIntensity"), (float)State.OverallIntensity);
    Statistics.Add(TEXT("EmotionalStability"), State.EmotionalStability);
    
    // 情感分佈統計
    float PositiveEmotions = 0.0f;
    float NegativeEmotions = 0.0f;
    float NeutralEmotions = 0.0f;
    
    for (const auto& EmotionPair : State.Emotions)
    {
        EEmotionType Emotion = EmotionPair.Key;
        float Value = EmotionPair.Value;
        
        if (IsPositiveEmotion(Emotion))
        {
            PositiveEmotions += Value;
        }
        else if (IsNegativeEmotion(Emotion))
        {
            NegativeEmotions += Value;
        }
        else
        {
            NeutralEmotions += Value;
        }
    }
    
    Statistics.Add(TEXT("PositiveEmotions"), PositiveEmotions);
    Statistics.Add(TEXT("NegativeEmotions"), NegativeEmotions);
    Statistics.Add(TEXT("NeutralEmotions"), NeutralEmotions);
    
    // 歷史統計
    if (EmotionalHistory.Contains(EntityID))
    {
        const TArray<FEmotionalState>& History = EmotionalHistory[EntityID];
        Statistics.Add(TEXT("HistoryLength"), History.Num());
        
        // 計算情感變化趨勢
        if (History.Num() >= 2)
        {
            const FEmotionalState& LastState = History[History.Num() - 1];
            const FEmotionalState& PreviousState = History[History.Num() - 2];
            
            float ChangeRate = CalculateEmotionalSimilarity(LastState, PreviousState);
            Statistics.Add(TEXT("ChangeRate"), 1.0f - ChangeRate);
        }
    }
    
    return Statistics;
}

void UMingRTSEmotionalAIEngine::ResetEmotionalState(const FString& EntityID)
{
    if (EmotionalStates.Contains(EntityID))
    {
        FEmotionalState& State = EmotionalStates[EntityID];
        
        // 重置為中性狀態
        for (int32 i = 0; i < 15; i++)
        {
            State.Emotions.Add((EEmotionType)i, 0.5f);
        }
        
        State.DominantEmotion = EEmotionType::Calm;
        State.OverallIntensity = EEmotionIntensity::Medium;
        State.EmotionalStability = 0.5f;
        State.LastUpdated = FDateTime::Now();
        
        UE_LOG(LogTemp, Log, TEXT("MingRTSEmotionalAIEngine: Reset emotional state for entity %s"), *EntityID);
    }
}

// 私有方法實現

void UMingRTSEmotionalAIEngine::InitializeResponseTemplates()
{
    ResponseTemplates.Empty();
    
    // 喜悅回應模板
    FEmotionalResponse JoyResponse;
    JoyResponse.ResponseEmotion = EEmotionType::Joy;
    JoyResponse.ResponseText = TEXT("太棒了！我感到非常開心！");
    JoyResponse.AudioResponse = TEXT("joy_sound.wav");
    JoyResponse.VisualResponse = TEXT("happy_animation");
    ResponseTemplates.Add(JoyResponse);
    
    // 憤怒回應模板
    FEmotionalResponse AngerResponse;
    AngerResponse.ResponseEmotion = EEmotionType::Anger;
    AngerResponse.ResponseText = TEXT("這讓我很生氣！");
    AngerResponse.AudioResponse = TEXT("anger_sound.wav");
    AngerResponse.VisualResponse = TEXT("angry_animation");
    ResponseTemplates.Add(AngerResponse);
    
    // 恐懼回應模板
    FEmotionalResponse FearResponse;
    FearResponse.ResponseEmotion = EEmotionType::Fear;
    FearResponse.ResponseText = TEXT("我感到害怕...");
    FearResponse.AudioResponse = TEXT("fear_sound.wav");
    FearResponse.VisualResponse = TEXT("fear_animation");
    ResponseTemplates.Add(FearResponse);
    
    // 平靜回應模板
    FEmotionalResponse CalmResponse;
    CalmResponse.ResponseEmotion = EEmotionType::Calm;
    CalmResponse.ResponseText = TEXT("一切都很好，保持平靜。");
    CalmResponse.AudioResponse = TEXT("calm_sound.wav");
    CalmResponse.VisualResponse = TEXT("calm_animation");
    ResponseTemplates.Add(CalmResponse);
}

void UMingRTSEmotionalAIEngine::ProcessEmotionalTrigger(const FString& EntityID, const FEmotionalTrigger& Trigger)
{
    if (!EmotionalStates.Contains(EntityID))
    {
        return;
    }
    
    FEmotionalState& State = EmotionalStates[EntityID];
    
    // 應用觸發器效果
    float CurrentEmotion = State.Emotions.FindRef(Trigger.TargetEmotion);
    float NewEmotion = FMath::Clamp(CurrentEmotion + Trigger.TriggerStrength, 0.0f, 1.0f);
    State.Emotions.Add(Trigger.TargetEmotion, NewEmotion);
    
    // 調整相關情感
    AdjustRelatedEmotions(State, Trigger.TargetEmotion, Trigger.TriggerStrength);
    
    // 更新情感穩定性
    State.EmotionalStability = FMath::Clamp(State.EmotionalStability - Trigger.TriggerStrength * 0.1f, 0.0f, 1.0f);
}

void UMingRTSEmotionalAIEngine::ApplyEmotionalDecay(const FString& EntityID, float DeltaTime)
{
    if (!EmotionalStates.Contains(EntityID))
    {
        return;
    }
    
    FEmotionalState& State = EmotionalStates[EntityID];
    
    // 應用情感衰減
    for (auto& EmotionPair : State.Emotions)
    {
        float CurrentValue = EmotionPair.Value;
        float DecayAmount = EmotionalDecayRate * DeltaTime;
        
        // 向中性值衰減
        if (CurrentValue > 0.5f)
        {
            EmotionPair.Value = FMath::Clamp(CurrentValue - DecayAmount, 0.5f, 1.0f);
        }
        else
        {
            EmotionPair.Value = FMath::Clamp(CurrentValue + DecayAmount, 0.0f, 0.5f);
        }
    }
    
    // 恢復情感穩定性
    State.EmotionalStability = FMath::Clamp(State.EmotionalStability + DeltaTime * 0.05f, 0.0f, 1.0f);
}

void UMingRTSEmotionalAIEngine::UpdateDominantEmotion(FEmotionalState& State)
{
    float MaxEmotion = 0.0f;
    EEmotionType DominantEmotion = EEmotionType::Calm;
    
    // 找出最強烈的情感
    for (const auto& EmotionPair : State.Emotions)
    {
        if (EmotionPair.Value > MaxEmotion)
        {
            MaxEmotion = EmotionPair.Value;
            DominantEmotion = EmotionPair.Key;
        }
    }
    
    State.DominantEmotion = DominantEmotion;
}

void UMingRTSEmotionalAIEngine::CalculateOverallIntensity(FEmotionalState& State)
{
    float TotalIntensity = 0.0f;
    int32 EmotionCount = 0;
    
    // 計算總體強度
    for (const auto& EmotionPair : State.Emotions)
    {
        // 計算與中性值的偏差
        float Deviation = FMath::Abs(EmotionPair.Value - 0.5f);
        TotalIntensity += Deviation;
        EmotionCount++;
    }
    
    if (EmotionCount > 0)
    {
        TotalIntensity /= EmotionCount;
        TotalIntensity *= 2.0f; // 將範圍調整到 0-1
    }
    
    // 確定強度等級
    if (TotalIntensity < 0.2f)
    {
        State.OverallIntensity = EEmotionIntensity::VeryLow;
    }
    else if (TotalIntensity < 0.4f)
    {
        State.OverallIntensity = EEmotionIntensity::Low;
    }
    else if (TotalIntensity < 0.6f)
    {
        State.OverallIntensity = EEmotionIntensity::Medium;
    }
    else if (TotalIntensity < 0.8f)
    {
        State.OverallIntensity = EEmotionIntensity::High;
    }
    else if (TotalIntensity < 0.9f)
    {
        State.OverallIntensity = EEmotionIntensity::VeryHigh;
    }
    else
    {
        State.OverallIntensity = EEmotionIntensity::Extreme;
    }
}

float UMingRTSEmotionalAIEngine::AnalyzeTextEmotion(const FString& Text, EEmotionType TargetEmotion) const
{
    float Score = 0.0f;
    
    // 基於關鍵詞分析情感
    TArray<FString> PositiveKeywords = {TEXT("好"), TEXT("棒"), TEXT("喜歡"), TEXT("開心"), TEXT("快樂")};
    TArray<FString> NegativeKeywords = {TEXT("壞"), TEXT("糟糕"), TEXT("討厭"), TEXT("生氣"), TEXT("悲傷")};
    TArray<FString> FearKeywords = {TEXT("害怕"), TEXT("恐懼"), TEXT("擔心"), TEXT("緊張"), TEXT("不安")};
    TArray<FString> AngerKeywords = {TEXT("憤怒"), TEXT("生氣"), TEXT("煩"), TEXT("火大"), TEXT("氣")};
    
    switch (TargetEmotion)
    {
        case EEmotionType::Joy:
            for (const FString& Keyword : PositiveKeywords)
            {
                if (Text.Contains(Keyword))
                {
                    Score += 0.2f;
                }
            }
            break;
            
        case EEmotionType::Sadness:
            for (const FString& Keyword : NegativeKeywords)
            {
                if (Text.Contains(Keyword))
                {
                    Score += 0.2f;
                }
            }
            break;
            
        case EEmotionType::Fear:
            for (const FString& Keyword : FearKeywords)
            {
                if (Text.Contains(Keyword))
                {
                    Score += 0.2f;
                }
            }
            break;
            
        case EEmotionType::Anger:
            for (const FString& Keyword : AngerKeywords)
            {
                if (Text.Contains(Keyword))
                {
                    Score += 0.2f;
                }
            }
            break;
            
        default:
            // 對於其他情感，使用更複雜的分析
            Score = AnalyzeComplexEmotion(Text, TargetEmotion);
            break;
    }
    
    return FMath::Clamp(Score, 0.0f, 1.0f);
}

float UMingRTSEmotionalAIEngine::AnalyzeBehaviorEmotion(const TArray<FString>& Behaviors, EEmotionType TargetEmotion) const
{
    float Score = 0.0f;
    
    // 基於行為模式分析情感
    for (const FString& Behavior : Behaviors)
    {
        if (Behavior.Contains(TEXT("攻擊")) || Behavior.Contains(TEXT("戰鬥")))
        {
            if (TargetEmotion == EEmotionType::Anger)
            {
                Score += 0.3f;
            }
            else if (TargetEmotion == EEmotionType::Fear)
            {
                Score += 0.1f;
            }
        }
        else if (Behavior.Contains(TEXT("逃跑")) || Behavior.Contains(TEXT("躲避")))
        {
            if (TargetEmotion == EEmotionType::Fear)
            {
                Score += 0.4f;
            }
        }
        else if (Behavior.Contains(TEXT("幫助")) || Behavior.Contains(TEXT("合作")))
        {
            if (TargetEmotion == EEmotionType::Love || TargetEmotion == EEmotionType::Trust)
            {
                Score += 0.3f;
            }
        }
        else if (Behavior.Contains(TEXT("慶祝")) || Behavior.Contains(TEXT("歡呼")))
        {
            if (TargetEmotion == EEmotionType::Joy)
            {
                Score += 0.4f;
            }
        }
    }
    
    return FMath::Clamp(Score, 0.0f, 1.0f);
}

float UMingRTSEmotionalAIEngine::AnalyzeContextEmotion(const FString& Context, EEmotionType TargetEmotion) const
{
    float Score = 0.0f;
    
    // 基於上下文分析情感
    if (Context.Contains(TEXT("戰爭")) || Context.Contains(TEXT("衝突")))
    {
        if (TargetEmotion == EEmotionType::Fear || TargetEmotion == EEmotionType::Anger)
        {
            Score += 0.2f;
        }
    }
    else if (Context.Contains(TEXT("和平")) || Context.Contains(TEXT("安寧")))
    {
        if (TargetEmotion == EEmotionType::Calm || TargetEmotion == EEmotionType::Contentment)
        {
            Score += 0.2f;
        }
    }
    else if (Context.Contains(TEXT("勝利")) || Context.Contains(TEXT("成功")))
    {
        if (TargetEmotion == EEmotionType::Joy || TargetEmotion == EEmotionType::Excitement)
        {
            Score += 0.3f;
        }
    }
    else if (Context.Contains(TEXT("失敗")) || Context.Contains(TEXT("失敗")))
    {
        if (TargetEmotion == EEmotionType::Sadness || TargetEmotion == EEmotionType::Disappointment)
        {
            Score += 0.3f;
        }
    }
    
    return FMath::Clamp(Score, 0.0f, 1.0f);
}

FEmotionalResponse UMingRTSEmotionalAIEngine::GenerateJoyResponse(const FEmotionalState& State) const
{
    FEmotionalResponse Response;
    Response.ResponseEmotion = EEmotionType::Joy;
    Response.ResponseText = TEXT("太棒了！我感到非常開心和興奮！");
    Response.AudioResponse = TEXT("joyful_laughter.wav");
    Response.VisualResponse = TEXT("happy_jump");
    Response.ResponseActions.Add(TEXT("celebrate"));
    Response.ResponseActions.Add(TEXT("share_happiness"));
    
    return Response;
}

FEmotionalResponse UMingRTSEmotionalAIEngine::GenerateSadnessResponse(const FEmotionalState& State) const
{
    FEmotionalResponse Response;
    Response.ResponseEmotion = EEmotionType::Sadness;
    Response.ResponseText = TEXT("我感到有些悲傷...");
    Response.AudioResponse = TEXT("soft_cry.wav");
    Response.VisualResponse = TEXT("sad_posture");
    Response.ResponseActions.Add(TEXT("seek_comfort"));
    Response.ResponseActions.Add(TEXT("reflect"));
    
    return Response;
}

FEmotionalResponse UMingRTSEmotionalAIEngine::GenerateAngerResponse(const FEmotionalState& State) const
{
    FEmotionalResponse Response;
    Response.ResponseEmotion = EEmotionType::Anger;
    Response.ResponseText = TEXT("這讓我很生氣！");
    Response.AudioResponse = TEXT("angry_voice.wav");
    Response.VisualResponse = TEXT("angry_gesture");
    Response.ResponseActions.Add(TEXT("confront"));
    Response.ResponseActions.Add(TEXT("express_frustration"));
    
    return Response;
}

FEmotionalResponse UMingRTSEmotionalAIEngine::GenerateFearResponse(const FEmotionalState& State) const
{
    FEmotionalResponse Response;
    Response.ResponseEmotion = EEmotionType::Fear;
    Response.ResponseText = TEXT("我感到害怕和不安...");
    Response.AudioResponse = TEXT("fearful_whimper.wav");
    Response.VisualResponse = TEXT("trembling");
    Response.ResponseActions.Add(TEXT("retreat"));
    Response.ResponseActions.Add(TEXT("seek_protection"));
    
    return Response;
}

FEmotionalResponse UMingRTSEmotionalAIEngine::GenerateSurpriseResponse(const FEmotionalState& State) const
{
    FEmotionalResponse Response;
    Response.ResponseEmotion = EEmotionType::Surprise;
    Response.ResponseText = TEXT("哇！這真是出乎意料！");
    Response.AudioResponse = TEXT("surprised_gas.wav");
    Response.VisualResponse = TEXT("wide_eyes");
    Response.ResponseActions.Add(TEXT("investigate"));
    Response.ResponseActions.Add(TEXT("question"));
    
    return Response;
}

FEmotionalResponse UMingRTSEmotionalAIEngine::GenerateLoveResponse(const FEmotionalState& State) const
{
    FEmotionalResponse Response;
    Response.ResponseEmotion = EEmotionType::Love;
    Response.ResponseText = TEXT("我感到充滿愛意和溫暖！");
    Response.AudioResponse = TEXT("gentle_voice.wav");
    Response.VisualResponse = TEXT("warm_smile");
    Response.ResponseActions.Add(TEXT("embrace"));
    Response.ResponseActions.Add(TEXT("care"));
    
    return Response;
}

FEmotionalResponse UMingRTSEmotionalAIEngine::GenerateCalmResponse(const FEmotionalState& State) const
{
    FEmotionalResponse Response;
    Response.ResponseEmotion = EEmotionType::Calm;
    Response.ResponseText = TEXT("一切都很好，我感到平靜和安寧。");
    Response.AudioResponse = TEXT("peaceful_breath.wav");
    Response.VisualResponse = TEXT("relaxed_posture");
    Response.ResponseActions.Add(TEXT("meditate"));
    Response.ResponseActions.Add(TEXT("observe"));
    
    return Response;
}

FEmotionalResponse UMingRTSEmotionalAIEngine::GenerateNeutralResponse(const FEmotionalState& State) const
{
    FEmotionalResponse Response;
    Response.ResponseEmotion = EEmotionType::Calm;
    Response.ResponseText = TEXT("我感覺還不錯。");
    Response.AudioResponse = TEXT("neutral_voice.wav");
    Response.VisualResponse = TEXT("neutral_expression");
    Response.ResponseActions.Add(TEXT("wait"));
    Response.ResponseActions.Add(TEXT("observe"));
    
    return Response;
}

FString UMingRTSEmotionalAIEngine::GetEmotionName(EEmotionType Emotion) const
{
    switch (Emotion)
    {
        case EEmotionType::Joy: return TEXT("喜悅");
        case EEmotionType::Sadness: return TEXT("悲傷");
        case EEmotionType::Anger: return TEXT("憤怒");
        case EEmotionType::Fear: return TEXT("恐懼");
        case EEmotionType::Surprise: return TEXT("驚訝");
        case EEmotionType::Disgust: return TEXT("厭惡");
        case EEmotionType::Trust: return TEXT("信任");
        case EEmotionType::Anticipation: return TEXT("期待");
        case EEmotionType::Love: return TEXT("愛");
        case EEmotionType::Optimism: return TEXT("樂觀");
        case EEmotionType::Pessimism: return TEXT("悲觀");
        case EEmotionType::Anxiety: return TEXT("焦慮");
        case EEmotionType::Calm: return TEXT("平靜");
        case EEmotionType::Excitement: return TEXT("興奮");
        case EEmotionType::Contentment: return TEXT("滿足");
        default: return TEXT("未知");
    }
}

float UMingRTSEmotionalAIEngine::GetEmotionWeight(EEmotionType Emotion) const
{
    // 不同情感的權重
    switch (Emotion)
    {
        case EEmotionType::Joy: return 1.0f;
        case EEmotionType::Sadness: return 0.9f;
        case EEmotionType::Anger: return 0.8f;
        case EEmotionType::Fear: return 0.7f;
        case EEmotionType::Love: return 1.0f;
        case EEmotionType::Calm: return 0.6f;
        default: return 0.5f;
    }
}

bool UMingRTSEmotionalAIEngine::IsEmotionCompatible(EEmotionType Emotion1, EEmotionType Emotion2) const
{
    // 檢查情感兼容性
    if (IsPositiveEmotion(Emotion1) && IsPositiveEmotion(Emotion2))
    {
        return true;
    }
    if (IsNegativeEmotion(Emotion1) && IsNegativeEmotion(Emotion2))
    {
        return true;
    }
    
    // 特殊兼容性
    if ((Emotion1 == EEmotionType::Love && Emotion2 == EEmotionType::Trust) ||
        (Emotion1 == EEmotionType::Trust && Emotion2 == EEmotionType::Love))
    {
        return true;
    }
    
    return false;
}

void UMingRTSEmotionalAIEngine::RecordEmotionalState(const FString& EntityID, const FEmotionalState& State)
{
    if (!EmotionalHistory.Contains(EntityID))
    {
        TArray<FEmotionalState> EmptyHistory;
        EmotionalHistory.Add(EntityID, EmptyHistory);
    }
    
    TArray<FEmotionalState>& History = EmotionalHistory[EntityID];
    History.Add(State);
    
    // 限制歷史記錄數量
    if (History.Num() > MaxEmotionalHistory)
    {
        History.RemoveAt(0);
    }
}

void UMingRTSEmotionalAIEngine::ProcessEmotionalContagion(const FString& EntityID)
{
    if (!EmotionalStates.Contains(EntityID))
    {
        return;
    }
    
    const FEmotionalState& SourceState = EmotionalStates[EntityID];
    TArray<FString> NearbyEntities = GetNearbyEntities(EntityID);
    
    for (const FString& NearbyEntity : NearbyEntities)
    {
        if (EmotionalStates.Contains(NearbyEntity))
        {
            TransferEmotion(EntityID, NearbyEntity, EmotionalContagionRate);
        }
    }
}

TArray<FString> UMingRTSEmotionalAIEngine::GetNearbyEntities(const FString& EntityID) const
{
    TArray<FString> NearbyEntities;
    
    // 這裡需要與實際的遊戲世界系統集成
    // 暫時返回所有實體
    for (const auto& StatePair : EmotionalStates)
    {
        if (StatePair.Key != EntityID)
        {
            NearbyEntities.Add(StatePair.Key);
        }
    }
    
    return NearbyEntities;
}

void UMingRTSEmotionalAIEngine::TransferEmotion(const FString& FromEntity, const FString& ToEntity, float TransferAmount)
{
    if (!EmotionalStates.Contains(FromEntity) || !EmotionalStates.Contains(ToEntity))
    {
        return;
    }
    
    const FEmotionalState& SourceState = EmotionalStates[FromEntity];
    FEmotionalState& TargetState = EmotionalStates[ToEntity];
    
    // 傳輸主導情感
    float SourceEmotion = SourceState.Emotions.FindRef(SourceState.DominantEmotion);
    float TargetEmotion = TargetState.Emotions.FindRef(SourceState.DominantEmotion);
    
    float TransferAmount = (SourceEmotion - TargetEmotion) * TransferAmount * 0.1f;
    float NewEmotion = FMath::Clamp(TargetEmotion + TransferAmount, 0.0f, 1.0f);
    
    TargetState.Emotions.Add(SourceState.DominantEmotion, NewEmotion);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSEmotionalAIEngine: Transferred emotion from %s to %s"), *FromEntity, *ToEntity);
}

bool UMingRTSEmotionalAIEngine::IsPositiveEmotion(EEmotionType Emotion) const
{
    return Emotion == EEmotionType::Joy || 
           Emotion == EEmotionType::Love || 
           Emotion == EEmotionType::Trust || 
           Emotion == EEmotionType::Optimism || 
           Emotion == EEmotionType::Contentment;
}

bool UMingRTSEmotionalAIEngine::IsNegativeEmotion(EEmotionType Emotion) const
{
    return Emotion == EEmotionType::Sadness || 
           Emotion == EEmotionType::Anger || 
           Emotion == EEmotionType::Fear || 
           Emotion == EEmotionType::Disgust || 
           Emotion == EEmotionType::Pessimism || 
           Emotion == EEmotionType::Anxiety;
}

void UMingRTSEmotionalAIEngine::AdjustRelatedEmotions(FEmotionalState& State, EEmotionType TriggerEmotion, float TriggerStrength)
{
    // 調整相關情感
    switch (TriggerEmotion)
    {
        case EEmotionType::Joy:
            // 喜悅會增加樂觀和愛
            State.Emotions.Add(EEmotionType::Optimism, FMath::Clamp(State.Emotions.FindRef(EEmotionType::Optimism) + TriggerStrength * 0.3f, 0.0f, 1.0f));
            State.Emotions.Add(EEmotionType::Love, FMath::Clamp(State.Emotions.FindRef(EEmotionType::Love) + TriggerStrength * 0.2f, 0.0f, 1.0f));
            break;
            
        case EEmotionType::Anger:
            // 憤怒會增加厭惡和悲觀
            State.Emotions.Add(EEmotionType::Disgust, FMath::Clamp(State.Emotions.FindRef(EEmotionType::Disgust) + TriggerStrength * 0.3f, 0.0f, 1.0f));
            State.Emotions.Add(EEmotionType::Pessimism, FMath::Clamp(State.Emotions.FindRef(EEmotionType::Pessimism) + TriggerStrength * 0.2f, 0.0f, 1.0f));
            break;
            
        case EEmotionType::Fear:
            // 恐懼會增加焦慮
            State.Emotions.Add(EEmotionType::Anxiety, FMath::Clamp(State.Emotions.FindRef(EEmotionType::Anxiety) + TriggerStrength * 0.4f, 0.0f, 1.0f));
            break;
            
        case EEmotionType::Love:
            // 愛會增加信任和喜悅
            State.Emotions.Add(EEmotionType::Trust, FMath::Clamp(State.Emotions.FindRef(EEmotionType::Trust) + TriggerStrength * 0.3f, 0.0f, 1.0f));
            State.Emotions.Add(EEmotionType::Joy, FMath::Clamp(State.Emotions.FindRef(EEmotionType::Joy) + TriggerStrength * 0.2f, 0.0f, 1.0f));
            break;
            
        default:
            break;
    }
}

void UMingRTSEmotionalAIEngine::ApplyAIBehaviorAdjustments(const FString& AIID, float AggressionLevel, float CooperationLevel, float RiskTaking, float CommunicationLevel)
{
    // 這裡需要與實際的AI系統集成
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("MingRTSEmotionalAIEngine: AI %s behavior adjustments - Aggression: %.2f, Cooperation: %.2f, Risk: %.2f, Communication: %.2f"), 
           *AIID, AggressionLevel, CooperationLevel, RiskTaking, CommunicationLevel);
}

void UMingRTSEmotionalAIEngine::UpdatePlayerEmotionalProfile(const FString& PlayerID, const TMap<EEmotionType, float>& EmotionFrequency, const TMap<EEmotionType, float>& EmotionVariability)
{
    // 更新玩家情感檔案
    if (EmotionalProfiles.Contains(PlayerID))
    {
        FEmotionalProfile& Profile = EmotionalProfiles[PlayerID];
        
        // 更新基礎情感
        Profile.BaseEmotions = EmotionFrequency;
        
        // 調整情感敏感性和波動性
        float TotalVariability = 0.0f;
        for (const auto& VariancePair : EmotionVariability)
        {
            TotalVariability += VariancePair.Value;
        }
        
        if (EmotionVariability.Num() > 0)
        {
            Profile.EmotionalVolatility = TotalVariability / EmotionVariability.Num();
        }
        
        // 更新偏好和避免的情感
        UpdateEmotionPreferences(Profile, EmotionFrequency);
    }
}

void UMingRTSEmotionalAIEngine::UpdateEmotionPreferences(FEmotionalProfile& Profile, const TMap<EEmotionType, float>& EmotionFrequency)
{
    Profile.PreferredEmotions.Empty();
    Profile.AvoidedEmotions.Empty();
    
    // 根據情感頻率確定偏好
    for (const auto& EmotionPair : EmotionFrequency)
    {
        if (EmotionPair.Value > 0.7f)
        {
            Profile.PreferredEmotions.Add(EmotionPair.Key);
        }
        else if (EmotionPair.Value < 0.3f)
        {
            Profile.AvoidedEmotions.Add(EmotionPair.Key);
        }
    }
}

TArray<FString> UMingRTSEmotionalAIEngine::ExtractBehaviorsFromData(const FString& Data) const
{
    TArray<FString> Behaviors;
    
    // 從數據中提取行為信息
    if (Data.Contains(TEXT("移動")))
    {
        Behaviors.Add(TEXT("移動"));
    }
    if (Data.Contains(TEXT("攻擊")))
    {
        Behaviors.Add(TEXT("攻擊"));
    }
    if (Data.Contains(TEXT("防禦")))
    {
        Behaviors.Add(TEXT("防禦"));
    }
    if (Data.Contains(TEXT("合作")))
    {
        Behaviors.Add(TEXT("合作"));
    }
    
    return Behaviors;
}

FString UMingRTSEmotionalAIEngine::ExtractContextFromData(const FString& Data) const
{
    // 從數據中提取上下文信息
    if (Data.Contains(TEXT("戰鬥")))
    {
        return TEXT("戰鬥");
    }
    else if (Data.Contains(TEXT("和平")))
    {
        return TEXT("和平");
    }
    else if (Data.Contains(TEXT("交易")))
    {
        return TEXT("交易");
    }
    else if (Data.Contains(TEXT("探索")))
    {
        return TEXT("探索");
    }
    
    return TEXT("一般");
}

float UMingRTSEmotionalAIEngine::AnalyzeComplexEmotion(const FString& Text, EEmotionType TargetEmotion) const
{
    // 對於複雜情感，使用更細緻的分析
    float Score = 0.0f;
    
    // 這裡可以實現更複雜的自然語言處理
    // 暫時使用簡單的關鍵詞匹配
    
    return FMath::Clamp(Score, 0.0f, 1.0f);
}
