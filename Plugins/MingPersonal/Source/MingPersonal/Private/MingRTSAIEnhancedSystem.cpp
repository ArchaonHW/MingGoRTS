#include "MingRTSAIEnhancedSystem.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"
#include "MingRTSAIController.h"
#include "MingRTSCombatSystem.h"
#include "MingRTSCampaignSystem.h"

UMingRTSAIEnhancedSystem::UMingRTSAIEnhancedSystem()
{
    CurrentWorld = nullptr;
    AIUpdateInterval = 0.5f; // 每500ms更新
    LastAIUpdateTime = 0.0f;
    MemoryDecayRate = 0.01f; // 每秒衰減1%
    LearningRate = 0.1f; // 學習率10%
    AdaptationRate = 0.05f; // 適應率5%
    MaxMemoryCount = 100;
    MaxDecisionCount = 50;
    MaxStrategyCount = 20;
}

void UMingRTSAIEnhancedSystem::InitializeAIEnhancedSystem(UWorld* World)
{
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSAIEnhancedSystem: Invalid world provided"));
        return;
    }

    CurrentWorld = World;
    LastAIUpdateTime = World->GetTimeSeconds();
    
    // 清空現有數據
    AIPersonalities.Empty();
    AIMemories.Empty();
    TacticalDecisions.Empty();
    AIStrategies.Empty();
    ActiveStrategies.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSAIEnhancedSystem initialized successfully"));
}

void UMingRTSAIEnhancedSystem::UpdateAIEnhancedSystem(float DeltaTime)
{
    if (!CurrentWorld)
    {
        return;
    }

    float CurrentTime = CurrentWorld->GetTimeSeconds();
    
    // 定期更新AI系統
    if (CurrentTime - LastAIUpdateTime >= AIUpdateInterval)
    {
        ProcessAIPersonalityUpdates(DeltaTime);
        ProcessMemorySystem(DeltaTime);
        ProcessTacticalDecisions(DeltaTime);
        ProcessAIStrategies(DeltaTime);
        ProcessLearningSystem(DeltaTime);
        ProcessEmotionalSystem(DeltaTime);
        
        LastAIUpdateTime = CurrentTime;
    }
    
    // 處理記憶衰減
    DecayMemories(DeltaTime);
}

FString UMingRTSAIEnhancedSystem::CreateAIPersonality(const FString& PersonalityID, EAIType AIType, EAIBehaviorPattern PrimaryPattern)
{
    if (PersonalityID.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid personality ID"));
        return TEXT("");
    }
    
    if (AIPersonalities.Contains(PersonalityID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Personality already exists: %s"), *PersonalityID);
        return TEXT("");
    }
    
    // 創建AI個性
    FAIPersonality Personality = GetPersonalityTemplate(AIType);
    Personality.PrimaryPattern = PrimaryPattern;
    
    // 根據主要模式調整特質
    switch (PrimaryPattern)
    {
        case EAIBehaviorPattern::Rush:
            Personality.Aggression = 0.9f;
            Personality.Caution = 0.1f;
            Personality.RiskTolerance = 0.8f;
            break;
        case EAIBehaviorPattern::Turtle:
            Personality.Aggression = 0.1f;
            Personality.Caution = 0.9f;
            Personality.RiskTolerance = 0.2f;
            break;
        case EAIBehaviorPattern::Boom:
            Personality.Aggression = 0.7f;
            Personality.Creativity = 0.8f;
            Personality.RiskTolerance = 0.9f;
            break;
        case EAIBehaviorPattern::Raid:
            Personality.Aggression = 0.8f;
            Personality.Caution = 0.3f;
            Personality.RiskTolerance = 0.6f;
            break;
        case EAIBehaviorPattern::Scout:
            Personality.Caution = 0.7f;
            Personality.Discipline = 0.8f;
            Personality.RiskTolerance = 0.3f;
            break;
        case EAIBehaviorPattern::Expand:
            Personality.Creativity = 0.7f;
            Personality.Discipline = 0.6f;
            Personality.RiskTolerance = 0.5f;
            break;
        case EAIBehaviorPattern::Tech:
            Personality.Creativity = 0.9f;
            Personality.Discipline = 0.8f;
            Personality.RiskTolerance = 0.4f;
            break;
        case EAIBehaviorPattern::Economic:
            Personality.Creativity = 0.6f;
            Personality.Discipline = 0.9f;
            Personality.RiskTolerance = 0.3f;
            break;
        case EAIBehaviorPattern::Defensive:
            Personality.Aggression = 0.2f;
            Personality.Caution = 0.8f;
            Personality.RiskTolerance = 0.2f;
            break;
        case EAIBehaviorPattern::Aggressive:
            Personality.Aggression = 0.8f;
            Personality.Caution = 0.2f;
            Personality.RiskTolerance = 0.7f;
            break;
        case EAIBehaviorPattern::Balanced:
            // 保持默認值
            break;
        case EAIBehaviorPattern::Adaptive:
            Personality.Adaptability = 0.9f;
            Personality.LearningRate = 0.2f;
            break;
    }
    
    AIPersonalities.Add(PersonalityID, Personality);
    
    UE_LOG(LogTemp, Log, TEXT("Created AI personality: %s (Type: %d, Pattern: %d)"), 
        *PersonalityID, (int32)AIType, (int32)PrimaryPattern);
    
    return PersonalityID;
}

FAIPersonality UMingRTSAIEnhancedSystem::GetAIPersonality(const FString& PersonalityID) const
{
    const FAIPersonality* Personality = AIPersonalities.Find(PersonalityID);
    if (Personality)
    {
        return *Personality;
    }
    return FAIPersonality();
}

void UMingRTSAIEnhancedSystem::UpdateAIPersonality(const FString& PersonalityID, const FAIPersonality& NewPersonality)
{
    FAIPersonality* Personality = AIPersonalities.Find(PersonalityID);
    if (!Personality)
    {
        return;
    }
    
    FAIPersonality OldPersonality = *Personality;
    *Personality = NewPersonality;
    
    // 觸發個性變化事件
    OnAIPersonalityChanged.Broadcast(NewPersonality);
    
    UE_LOG(LogTemp, Log, TEXT("Updated AI personality: %s"), *PersonalityID);
}

FString UMingRTSAIEnhancedSystem::CreateAIMemory(const FString& PersonalityID, EAIMemoryType MemoryType, const FString& Description)
{
    if (PersonalityID.IsEmpty() || Description.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid personality ID or description"));
        return TEXT("");
    }
    
    if (!AIPersonalities.Contains(PersonalityID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Personality not found: %s"), *PersonalityID);
        return TEXT("");
    }
    
    if (AIMemories.Num() >= MaxMemoryCount)
    {
        UE_LOG(LogTemp, Warning, TEXT("Maximum memory count reached"));
        return TEXT("");
    }
    
    // 創建記憶
    FAIMemory Memory;
    Memory.MemoryID = FString::Printf(TEXT("Memory_%s_%d"), *PersonalityID, AIMemories.Num());
    Memory.MemoryType = MemoryType;
    Memory.Description = Description;
    Memory.CreationTime = CurrentWorld->GetTimeSeconds();
    Memory.LastAccessed = Memory.CreationTime;
    Memory.Importance = 0.5f;
    Memory.Confidence = 0.5f;
    Memory.AccessCount = 0;
    Memory.bIsActive = true;
    
    // 根據記憶類型設置初始重要性
    switch (MemoryType)
    {
        case EAIMemoryType::Strategic:
            Memory.Importance = 0.8f;
            break;
        case EAIMemoryType::Tactical:
            Memory.Importance = 0.6f;
            break;
        case EAIMemoryType::Emotional:
            Memory.Importance = 0.7f;
            break;
        case EAIMemoryType::Social:
            Memory.Importance = 0.5f;
            break;
        case EAIMemoryType::LongTerm:
            Memory.Importance = 0.9f;
            break;
        case EAIMemoryType::ShortTerm:
            Memory.Importance = 0.3f;
            break;
    }
    
    AIMemories.Add(Memory.MemoryID, Memory);
    
    // 觸發記憶創建事件
    OnAIMemoryCreated.Broadcast(Memory);
    
    UE_LOG(LogTemp, Log, TEXT("Created AI memory: %s for personality: %s"), *Memory.MemoryID, *PersonalityID);
    
    return Memory.MemoryID;
}

FAIMemory UMingRTSAIEnhancedSystem::GetAIMemory(const FString& MemoryID) const
{
    const FAIMemory* Memory = AIMemories.Find(MemoryID);
    if (Memory)
    {
        return *Memory;
    }
    return FAIMemory();
}

TArray<FAIMemory> UMingRTSAIEnhancedSystem::RetrieveAIMemories(const FString& PersonalityID, EAIMemoryType MemoryType, const FString& Query) const
{
    TArray<FAIMemory> RetrievedMemories;
    
    // 簡化實現：返回所有匹配類型的記憶
    for (const TPair<FString, FAIMemory>& MemoryPair : AIMemories)
    {
        const FAIMemory& Memory = MemoryPair.Value;
        
        if (Memory.MemoryType == MemoryType && Memory.bIsActive)
        {
            // 如果有查詢，檢查描述是否包含查詢
            if (Query.IsEmpty() || Memory.Description.Contains(Query))
            {
                RetrievedMemories.Add(Memory);
            }
        }
    }
    
    // 按重要性排序
    RetrievedMemories.Sort([](const FAIMemory& A, const FAIMemory& B)
    {
        return A.Importance > B.Importance;
    });
    
    return RetrievedMemories;
}

void UMingRTSAIEnhancedSystem::UpdateMemoryImportance(const FString& MemoryID, float NewImportance)
{
    FAIMemory* Memory = AIMemories.Find(MemoryID);
    if (!Memory)
    {
        return;
    }
    
    Memory->Importance = FMath::Clamp(NewImportance, 0.0f, 1.0f);
    Memory->LastAccessed = CurrentWorld->GetTimeSeconds();
    Memory->AccessCount++;
    
    UE_LOG(LogTemp, Log, TEXT("Updated memory importance: %s to %.2f"), *MemoryID, NewImportance);
}

void UMingRTSAIEnhancedSystem::DecayMemories(float DeltaTime)
{
    for (TPair<FString, FAIMemory>& MemoryPair : AIMemories)
    {
        FAIMemory& Memory = MemoryPair.Value;
        
        if (!Memory.bIsActive)
        {
            continue;
        }
        
        // 根據記憶類型衰減
        float DecayRate = MemoryDecayRate;
        
        switch (Memory.MemoryType)
        {
            case EAIMemoryType::ShortTerm:
                DecayRate *= 3.0f; // 短期記憶衰減更快
                break;
            case EAIMemoryType::LongTerm:
                DecayRate *= 0.1f; // 長期記憶衰減很慢
                break;
            case EAIMemoryType::Strategic:
                DecayRate *= 0.2f; // 戰略記憶衰減較慢
                break;
            default:
                break;
        }
        
        // 衰減重要性
        Memory.Importance *= (1.0f - DecayRate * DeltaTime);
        
        // 如果重要性太低，停用記憶
        if (Memory.Importance < 0.1f)
        {
            Memory.bIsActive = false;
        }
    }
}

FString UMingRTSAIEnhancedSystem::MakeTacticalDecision(const FString& PersonalityID, const FString& Context)
{
    if (!AIPersonalities.Contains(PersonalityID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Personality not found: %s"), *PersonalityID);
        return TEXT("");
    }
    
    if (TacticalDecisions.Num() >= MaxDecisionCount)
    {
        UE_LOG(LogTemp, Warning, TEXT("Maximum decision count reached"));
        return TEXT("");
    }
    
    const FAIPersonality& Personality = AIPersonalities[PersonalityID];
    
    // 生成決策選項
    TArray<FAITacticalDecision> DecisionOptions = GenerateDecisionOptions(PersonalityID, Context);
    
    if (DecisionOptions.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No decision options generated"));
        return TEXT("");
    }
    
    // 選擇最佳決策
    FAITacticalDecision BestDecision = DecisionOptions[0];
    float BestWeight = CalculateDecisionWeight(BestDecision, Personality);
    
    for (const FAITacticalDecision& Decision : DecisionOptions)
    {
        float Weight = CalculateDecisionWeight(Decision, Personality);
        if (Weight > BestWeight)
        {
            BestWeight = Weight;
            BestDecision = Decision;
        }
    }
    
    // 創建決策
    FString DecisionID = FString::Printf(TEXT("Decision_%s_%d"), *PersonalityID, TacticalDecisions.Num());
    BestDecision.DecisionID = DecisionID;
    BestDecision.StartTime = CurrentWorld->GetTimeSeconds();
    
    TacticalDecisions.Add(DecisionID, BestDecision);
    
    // 觸發決策事件
    OnAITacticalDecisionMade.Broadcast(BestDecision);
    
    UE_LOG(LogTemp, Log, TEXT("Made tactical decision: %s for personality: %s"), *DecisionID, *PersonalityID);
    
    return DecisionID;
}

FAITacticalDecision UMingRTSAIEnhancedSystem::GetTacticalDecision(const FString& DecisionID) const
{
    const FAITacticalDecision* Decision = TacticalDecisions.Find(DecisionID);
    if (Decision)
    {
        return *Decision;
    }
    return FAITacticalDecision();
}

void UMingRTSAIEnhancedSystem::ExecuteTacticalDecision(const FString& DecisionID)
{
    FAITacticalDecision* Decision = TacticalDecisions.Find(DecisionID);
    if (!Decision)
    {
        return;
    }
    
    Decision->bIsExecuting = true;
    Decision->StartTime = CurrentWorld->GetTimeSeconds();
    
    UE_LOG(LogTemp, Log, TEXT("Executing tactical decision: %s"), *DecisionID);
    
    // 這裡應該實現決策的實際執行邏輯
    // 例如移動單位、攻擊目標、建造建築等
}

void UMingRTSAIEnhancedSystem::EvaluateDecisionResult(const FString& DecisionID, bool bSuccess, const FString& Outcome)
{
    FAITacticalDecision* Decision = TacticalDecisions.Find(DecisionID);
    if (!Decision)
    {
        return;
    }
    
    Decision->bIsExecuting = false;
    
    // 更新成功概率
    if (bSuccess)
    {
        Decision->SuccessProbability = FMath::Min(Decision->SuccessProbability + 0.1f, 1.0f);
    }
    else
    {
        Decision->SuccessProbability = FMath::Max(Decision->SuccessProbability - 0.1f, 0.0f);
    }
    
    // 創建學習記憶
    FString PersonalityID = DecisionID.Left(DecisionID.Find(TEXT("_Decision_")));
    FString MemoryDescription = FString::Printf(TEXT("Decision %s: %s - Result: %s"), 
        *Decision->Description, bSuccess ? TEXT("Success") : TEXT("Failure"), *Outcome);
    
    CreateAIMemory(PersonalityID, EAIMemoryType::Tactical, MemoryDescription);
    
    UE_LOG(LogTemp, Log, TEXT("Evaluated decision result: %s - %s"), *DecisionID, bSuccess ? TEXT("Success") : TEXT("Failure"));
}

FString UMingRTSAIEnhancedSystem::CreateAIStrategy(const FString& PersonalityID, EAIBehaviorPattern Pattern)
{
    if (!AIPersonalities.Contains(PersonalityID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Personality not found: %s"), *PersonalityID);
        return TEXT("");
    }
    
    if (AIStrategies.Num() >= MaxStrategyCount)
    {
        UE_LOG(LogTemp, Warning, TEXT("Maximum strategy count reached"));
        return TEXT("");
    }
    
    // 創建策略
    FAIStrategy Strategy = GetStrategyTemplate(Pattern);
    Strategy.StrategyID = FString::Printf(TEXT("Strategy_%s_%d"), *PersonalityID, AIStrategies.Num());
    
    AIStrategies.Add(Strategy.StrategyID, Strategy);
    
    UE_LOG(LogTemp, Log, TEXT("Created AI strategy: %s for personality: %s"), *Strategy.StrategyID, *PersonalityID);
    
    return Strategy.StrategyID;
}

FAIStrategy UMingRTSAIEnhancedSystem::GetAIStrategy(const FString& StrategyID) const
{
    const FAIStrategy* Strategy = AIStrategies.Find(StrategyID);
    if (Strategy)
    {
        return *Strategy;
    }
    return FAIStrategy();
}

void UMingRTSAIEnhancedSystem::ActivateAIStrategy(const FString& StrategyID)
{
    FAIStrategy* Strategy = AIStrategies.Find(StrategyID);
    if (!Strategy)
    {
        return;
    }
    
    // 停用當前策略
    for (TPair<FString, FString>& ActiveStrategy : ActiveStrategies)
    {
        if (ActiveStrategy.Value == StrategyID)
        {
            FAIStrategy* OldStrategy = AIStrategies.Find(ActiveStrategy.Value);
            if (OldStrategy)
            {
                OldStrategy->bIsActive = false;
            }
            break;
        }
    }
    
    // 激活新策略
    Strategy->bIsActive = true;
    Strategy->StartTime = CurrentWorld->GetTimeSeconds();
    Strategy->CurrentPhase = 0.0f;
    
    // 更新活躍策略映射
    FString PersonalityID = StrategyID.Left(StrategyID.Find(TEXT("_Strategy_")));
    ActiveStrategies.Add(PersonalityID, StrategyID);
    
    // 觸發策略激活事件
    OnAIStrategyActivated.Broadcast(*Strategy);
    
    UE_LOG(LogTemp, Log, TEXT("Activated AI strategy: %s"), *StrategyID);
}

void UMingRTSAIEnhancedSystem::UpdateAIStrategy(const FString& StrategyID, float DeltaTime)
{
    FAIStrategy* Strategy = AIStrategies.Find(StrategyID);
    if (!Strategy || !Strategy->bIsActive)
    {
        return;
    }
    
    // 更新策略階段
    Strategy->CurrentPhase += DeltaTime;
    
    // 檢查是否需要切換階段
    for (const TPair<FString, float>& PhaseTiming : Strategy->PhaseTimings)
    {
        if (Strategy->CurrentPhase >= PhaseTiming.Value)
        {
            // 執行階段切換邏輯
            UE_LOG(LogTemp, Log, TEXT("Strategy phase transition: %s - %s"), *StrategyID, *PhaseTiming.Key);
        }
    }
}

void UMingRTSAIEnhancedSystem::LearnFromExperience(const FString& PersonalityID, const FString& Experience, float Outcome)
{
    if (!AIPersonalities.Contains(PersonalityID))
    {
        return;
    }
    
    FAIPersonality* Personality = AIPersonalities.Find(PersonalityID);
    
    // 創建學習記憶
    FString MemoryDescription = FString::Printf(TEXT("Experience: %s - Outcome: %.2f"), *Experience, Outcome);
    CreateAIMemory(PersonalityID, EAIMemoryType::LongTerm, MemoryDescription);
    
    // 根據結果調整個性特質
    if (Outcome > 0.5f) // 成功經驗
    {
        // 增強相關特質
        if (Experience.Contains(TEXT("aggressive")))
        {
            Personality->Aggression = FMath::Min(Personality->Aggression + Personality->LearningRate * 0.1f, 1.0f);
        }
        if (Experience.Contains(TEXT("cautious")))
        {
            Personality->Caution = FMath::Min(Personality->Caution + Personality->LearningRate * 0.1f, 1.0f);
        }
    }
    else // 失敗經驗
    {
        // 減弱相關特質
        if (Experience.Contains(TEXT("aggressive")))
        {
            Personality->Aggression = FMath::Max(Personality->Aggression - Personality->LearningRate * 0.1f, 0.0f);
        }
        if (Experience.Contains(TEXT("cautious")))
        {
            Personality->Caution = FMath::Max(Personality->Caution - Personality->LearningRate * 0.1f, 0.0f);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("AI learned from experience: %s - %.2f"), *Experience, Outcome);
}

void UMingRTSAIEnhancedSystem::AdaptToPlayerStrategy(const FString& PersonalityID, const FString& PlayerStrategy)
{
    if (!AIPersonalities.Contains(PersonalityID))
    {
        return;
    }
    
    FAIPersonality* Personality = AIPersonalities.Find(PersonalityID);
    
    // 創建適應記憶
    FString MemoryDescription = FString::Printf(TEXT("Player strategy: %s"), *PlayerStrategy);
    CreateAIMemory(PersonalityID, EAIMemoryType::Strategic, MemoryDescription);
    
    // 根據玩家策略調整AI行為
    if (PlayerStrategy.Contains(TEXT("aggressive")))
    {
        // 玩家激進，AI變得更謹慎
        Personality->Caution = FMath::Min(Personality->Caution + AdaptationRate * 0.2f, 1.0f);
        Personality->Aggression = FMath::Max(Personality->Aggression - AdaptationRate * 0.1f, 0.0f);
    }
    else if (PlayerStrategy.Contains(TEXT("defensive")))
    {
        // 玩家防禦，AI變得更激進
        Personality->Aggression = FMath::Min(Personality->Aggression + AdaptationRate * 0.2f, 1.0f);
        Personality->Caution = FMath::Max(Personality->Caution - AdaptationRate * 0.1f, 0.0f);
    }
    
    UE_LOG(LogTemp, Log, TEXT("AI adapted to player strategy: %s"), *PlayerStrategy);
}

void UMingRTSAIEnhancedSystem::UpdateEmotionalState(const FString& PersonalityID, EAIEmotionalState NewState)
{
    FAIPersonality* Personality = AIPersonalities.Find(PersonalityID);
    if (!Personality)
    {
        return;
    }
    
    EAIEmotionalState OldState = Personality->CurrentEmotionalState;
    Personality->CurrentEmotionalState = NewState;
    
    // 根據情感狀態調整特質
    switch (NewState)
    {
        case EAIEmotionalState::Aggressive:
            Personality->Aggression = FMath::Min(Personality->Aggression + 0.2f, 1.0f);
            Personality->Caution = FMath::Max(Personality->Caution - 0.1f, 0.0f);
            break;
        case EAIEmotionalState::Cautious:
            Personality->Caution = FMath::Min(Personality->Caution + 0.2f, 1.0f);
            Personality->Aggression = FMath::Max(Personality->Aggression - 0.1f, 0.0f);
            break;
        case EAIEmotionalState::Confident:
            Personality->RiskTolerance = FMath::Min(Personality->RiskTolerance + 0.1f, 1.0f);
            break;
        case EAIEmotionalState::Anxious:
            Personality->Caution = FMath::Min(Personality->Caution + 0.1f, 1.0f);
            Personality->RiskTolerance = FMath::Max(Personality->RiskTolerance - 0.1f, 0.0f);
            break;
        default:
            break;
    }
    
    // 觸發情感狀態變化事件
    OnAIEmotionalStateChanged.Broadcast(NewState);
    
    UE_LOG(LogTemp, Log, TEXT("Updated emotional state: %s - %d -> %d"), *PersonalityID, (int32)OldState, (int32)NewState);
}

EAIEmotionalState UMingRTSAIEnhancedSystem::GetEmotionalState(const FString& PersonalityID) const
{
    const FAIPersonality* Personality = AIPersonalities.Find(PersonalityID);
    if (Personality)
    {
        return Personality->CurrentEmotionalState;
    }
    return EAIEmotionalState::Calm;
}

TMap<FString, float> UMingRTSAIEnhancedSystem::AnalyzeBattlefield(const FString& PersonalityID) const
{
    TMap<FString, float> BattlefieldAnalysis;
    
    // 簡化實現：返回基本戰場分析
    BattlefieldAnalysis.Add(TEXT("EnemyStrength"), 0.5f);
    BattlefieldAnalysis.Add(TEXT("OwnStrength"), 0.5f);
    BattlefieldAnalysis.Add(TEXT("TerrainAdvantage"), 0.3f);
    BattlefieldAnalysis.Add(TEXT("ResourceControl"), 0.4f);
    BattlefieldAnalysis.Add(TEXT("StrategicPosition"), 0.6f);
    
    // 根據AI個性調整分析
    const FAIPersonality* Personality = AIPersonalities.Find(PersonalityID);
    if (Personality)
    {
        if (Personality->Aggression > 0.7f)
        {
            BattlefieldAnalysis[TEXT("EnemyStrength")] *= 0.8f; // 低估敵人實力
            BattlefieldAnalysis[TEXT("OwnStrength")] *= 1.2f; // 高估自己實力
        }
        
        if (Personality->Caution > 0.7f)
        {
            BattlefieldAnalysis[TEXT("EnemyStrength")] *= 1.2f; // 高估敵人實力
            BattlefieldAnalysis[TEXT("OwnStrength")] *= 0.8f; // 低估自己實力
        }
    }
    
    return BattlefieldAnalysis;
}

TArray<FString> UMingRTSAIEnhancedSystem::PredictPlayerActions(const FString& PersonalityID) const
{
    TArray<FString> PredictedActions;
    
    // 簡化實現：返回基本預測
    PredictedActions.Add(TEXT("Expand economy"));
    PredictedActions.Add(TEXT("Build defensive structures"));
    PredictedActions.Add(TEXT("Scout territory"));
    PredictedActions.Add(TEXT("Gather resources"));
    
    // 根據AI記憶調整預測
    TArray<FAIMemory> PlayerMemories = RetrieveAIMemories(PersonalityID, EAIMemoryType::Strategic, TEXT("player"));
    
    for (const FAIMemory& Memory : PlayerMemories)
    {
        if (Memory.Description.Contains(TEXT("aggressive")))
        {
            PredictedActions.Add(TEXT("Launch attack"));
        }
        if (Memory.Description.Contains(TEXT("defensive")))
        {
            PredictedActions.Add(TEXT("Fortify position"));
        }
    }
    
    return PredictedActions;
}

TMap<FString, float> UMingRTSAIEnhancedSystem::CalculateThreatAssessment(const FString& PersonalityID) const
{
    TMap<FString, float> ThreatAssessment;
    
    // 簡化實現：返回基本威脅評估
    ThreatAssessment.Add(TEXT("MilitaryThreat"), 0.5f);
    ThreatAssessment.Add(TEXT("EconomicThreat"), 0.3f);
    ThreatAssessment.Add(TEXT("StrategicThreat"), 0.4f);
    ThreatAssessment.Add(TEXT("ResourceThreat"), 0.2f);
    ThreatAssessment.Add(TEXT("TerritorialThreat"), 0.6f);
    
    // 根據AI個性調整威脅評估
    const FAIPersonality* Personality = AIPersonalities.Find(PersonalityID);
    if (Personality)
    {
        if (Personality->Caution > 0.7f)
        {
            // 謹慎的AI會高估威脅
            for (TPair<FString, float>& Threat : ThreatAssessment)
            {
                Threat.Value *= 1.3f;
            }
        }
        
        if (Personality->Aggression > 0.7f)
        {
            // 激進的AI會低估威脅
            for (TPair<FString, float>& Threat : ThreatAssessment)
            {
                Threat.Value *= 0.7f;
            }
        }
    }
    
    return ThreatAssessment;
}

TMap<EResourceType, float> UMingRTSAIEnhancedSystem::OptimizeResourceAllocation(const FString& PersonalityID) const
{
    TMap<EResourceType, float> ResourceAllocation;
    
    // 簡化實現：返回基本資源分配
    ResourceAllocation.Add(EResourceType::Gold, 0.3f);
    ResourceAllocation.Add(EResourceType::Food, 0.2f);
    ResourceAllocation.Add(EResourceType::Wood, 0.15f);
    ResourceAllocation.Add(EResourceType::Stone, 0.15f);
    ResourceAllocation.Add(EResourceType::Iron, 0.2f);
    
    // 根據AI個性調整資源分配
    const FAIPersonality* Personality = AIPersonalities.Find(PersonalityID);
    if (Personality)
    {
        if (Personality->PrimaryPattern == EAIBehaviorPattern::Economic)
        {
            ResourceAllocation[EResourceType::Gold] = 0.5f;
            ResourceAllocation[EResourceType::Food] = 0.3f;
            ResourceAllocation[EResourceType::Wood] = 0.1f;
            ResourceAllocation[EResourceType::Stone] = 0.05f;
            ResourceAllocation[EResourceType::Iron] = 0.05f;
        }
        else if (Personality->PrimaryPattern == EAIBehaviorPattern::Aggressive)
        {
            ResourceAllocation[EResourceType::Gold] = 0.2f;
            ResourceAllocation[EResourceType::Food] = 0.2f;
            ResourceAllocation[EResourceType::Wood] = 0.1f;
            ResourceAllocation[EResourceType::Stone] = 0.1f;
            ResourceAllocation[EResourceType::Iron] = 0.4f;
        }
    }
    
    return ResourceAllocation;
}

void UMingRTSAIEnhancedSystem::CoordinateAITeam(const TArray<FString>& TeamMembers)
{
    UE_LOG(LogTemp, Log, TEXT("Coordinating AI team with %d members"), TeamMembers.Num());
    
    // 簡化實現：協調AI團隊
    for (const FString& MemberID : TeamMembers)
    {
        if (AIPersonalities.Contains(MemberID))
        {
            FAIPersonality* Personality = AIPersonalities.Find(MemberID);
            
            // 增強團隊合作特質
            Personality->Teamwork = FMath::Min(Personality->Teamwork + 0.1f, 1.0f);
            
            // 創建團隊協作記憶
            FString MemoryDescription = FString::Printf(TEXT("Team coordination with %d members"), TeamMembers.Num());
            CreateAIMemory(MemberID, EAIMemoryType::Social, MemoryDescription);
        }
    }
}

TMap<FString, float> UMingRTSAIEnhancedSystem::GetAIStatistics(const FString& PersonalityID) const
{
    TMap<FString, float> Statistics;
    
    const FAIPersonality* Personality = AIPersonalities.Find(PersonalityID);
    if (!Personality)
    {
        return Statistics;
    }
    
    // 個性統計
    Statistics.Add(TEXT("Aggression"), Personality->Aggression);
    Statistics.Add(TEXT("Caution"), Personality->Caution);
    Statistics.Add(TEXT("Creativity"), Personality->Creativity);
    Statistics.Add(TEXT("Discipline"), Personality->Discipline);
    Statistics.Add(TEXT("Adaptability"), Personality->Adaptability);
    Statistics.Add(TEXT("RiskTolerance"), Personality->RiskTolerance);
    Statistics.Add(TEXT("Teamwork"), Personality->Teamwork);
    Statistics.Add(TEXT("Leadership"), Personality->Leadership);
    Statistics.Add(TEXT("LearningRate"), Personality->LearningRate);
    
    // 記憶統計
    int32 TotalMemories = 0;
    int32 ActiveMemories = 0;
    float AverageImportance = 0.0f;
    
    for (const TPair<FString, FAIMemory>& MemoryPair : AIMemories)
    {
        if (MemoryPair.Key.Contains(PersonalityID))
        {
            TotalMemories++;
            if (MemoryPair.Value.bIsActive)
            {
                ActiveMemories++;
                AverageImportance += MemoryPair.Value.Importance;
            }
        }
    }
    
    if (ActiveMemories > 0)
    {
        AverageImportance /= ActiveMemories;
    }
    
    Statistics.Add(TEXT("TotalMemories"), (float)TotalMemories);
    Statistics.Add(TEXT("ActiveMemories"), (float)ActiveMemories);
    Statistics.Add(TEXT("AverageMemoryImportance"), AverageImportance);
    
    // 決策統計
    int32 TotalDecisions = 0;
    int32 ExecutedDecisions = 0;
    float AverageSuccessRate = 0.0f;
    
    for (const TPair<FString, FAITacticalDecision>& DecisionPair : TacticalDecisions)
    {
        if (DecisionPair.Key.Contains(PersonalityID))
        {
            TotalDecisions++;
            if (DecisionPair.Value.bIsExecuting)
            {
                ExecutedDecisions++;
            }
            AverageSuccessRate += DecisionPair.Value.SuccessProbability;
        }
    }
    
    if (TotalDecisions > 0)
    {
        AverageSuccessRate /= TotalDecisions;
    }
    
    Statistics.Add(TEXT("TotalDecisions"), (float)TotalDecisions);
    Statistics.Add(TEXT("ExecutedDecisions"), (float)ExecutedDecisions);
    Statistics.Add(TEXT("AverageSuccessRate"), AverageSuccessRate);
    
    return Statistics;
}

void UMingRTSAIEnhancedSystem::ProcessAIPersonalityUpdates(float DeltaTime)
{
    for (TPair<FString, FAIPersonality>& PersonalityPair : AIPersonalities)
    {
        FAIPersonality& Personality = PersonalityPair.Value;
        
        // 個性發展
        DevelopPersonality(PersonalityPair.Key);
        
        // 情感調節
        RegulateEmotions(PersonalityPair.Key);
        
        // 壓力管理
        ManageStress(PersonalityPair.Key);
    }
}

void UMingRTSAIEnhancedSystem::ProcessMemorySystem(float DeltaTime)
{
    // 記憶整合
    for (const TPair<FString, FAIPersonality>& PersonalityPair : AIPersonalities)
    {
        ConsolidateMemories(PersonalityPair.Key);
    }
    
    // 記憶清理
    for (const TPair<FString, FAIPersonality>& PersonalityPair : AIPersonalities)
    {
        CleanupMemories(PersonalityPair.Key);
    }
}

void UMingRTSAIEnhancedSystem::ProcessTacticalDecisions(float DeltaTime)
{
    // 決策清理
    for (const TPair<FString, FAIPersonality>& PersonalityPair : AIPersonalities)
    {
        CleanupDecisions(PersonalityPair.Key);
    }
}

void UMingRTSAIEnhancedSystem::ProcessAIStrategies(float DeltaTime)
{
    // 更新活躍策略
    for (TPair<FString, FString>& ActiveStrategy : ActiveStrategies)
    {
        UpdateAIStrategy(ActiveStrategy.Value, DeltaTime);
    }
    
    // 策略優化
    for (const TPair<FString, FAIPersonality>& PersonalityPair : AIPersonalities)
    {
        OptimizeStrategies(PersonalityPair.Key);
    }
}

void UMingRTSAIEnhancedSystem::ProcessLearningSystem(float DeltaTime)
{
    // 學習系統處理
    // 這裡可以添加更複雜的學習算法
}

void UMingRTSAIEnhancedSystem::ProcessEmotionalSystem(float DeltaTime)
{
    for (TPair<FString, FAIPersonality>& PersonalityPair : AIPersonalities)
    {
        FAIPersonality& Personality = PersonalityPair.Value;
        
        // 情感狀態自然回歸
        if (Personality.CurrentEmotionalState != EAIEmotionalState::Calm)
        {
            // 有機率回歸到平靜狀態
            if (FMath::RandRange(0.0f, 1.0f) < 0.01f) // 1%機率
            {
                UpdateEmotionalState(PersonalityPair.Key, EAIEmotionalState::Calm);
            }
        }
    }
}

float UMingRTSAIEnhancedSystem::CalculateDecisionWeight(const FAITacticalDecision& Decision, const FAIPersonality& Personality) const
{
    float Weight = Decision.Priority;
    
    // 根據個性調整權重
    switch (Decision.Pattern)
    {
        case EAIBehaviorPattern::Aggressive:
            Weight *= (1.0f + Personality.Aggression * 0.5f);
            break;
        case EAIBehaviorPattern::Defensive:
            Weight *= (1.0f + Personality.Caution * 0.5f);
            break;
        case EAIBehaviorPattern::Risky:
            Weight *= (1.0f + Personality.RiskTolerance * 0.5f);
            break;
        case EAIBehaviorPattern::Creative:
            Weight *= (1.0f + Personality.Creativity * 0.5f);
            break;
        default:
            break;
    }
    
    // 根據情感狀態調整
    switch (Personality.CurrentEmotionalState)
    {
        case EAIEmotionalState::Confident:
            Weight *= 1.2f;
            break;
        case EAIEmotionalState::Anxious:
            Weight *= 0.8f;
            break;
        case EAIEmotionalState::Aggressive:
            Weight *= 1.1f;
            break;
        case EAIEmotionalState::Cautious:
            Weight *= 0.9f;
            break;
        default:
            break;
    }
    
    return Weight;
}

float UMingRTSAIEnhancedSystem::EvaluateStrategyEffectiveness(const FAIStrategy& Strategy) const
{
    float Effectiveness = Strategy.OverallPriority;
    
    // 根據執行時間調整
    float ExecutionTime = CurrentWorld->GetTimeSeconds() - Strategy.StartTime;
    if (ExecutionTime > 0.0f)
    {
        Effectiveness *= (1.0f + ExecutionTime / 300.0f); // 每5分鐘增加效果
    }
    
    return Effectiveness;
}

void UMingRTSAIEnhancedSystem::UpdatePersonalityTraits(const FString& PersonalityID, const TMap<FString, float>& TraitChanges)
{
    FAIPersonality* Personality = AIPersonalities.Find(PersonalityID);
    if (!Personality)
    {
        return;
    }
    
    for (const TPair<FString, float>& TraitChange : TraitChanges)
    {
        if (TraitChange.Key == TEXT("Aggression"))
        {
            Personality->Aggression = FMath::Clamp(Personality->Aggression + TraitChange.Value, 0.0f, 1.0f);
        }
        else if (TraitChange.Key == TEXT("Caution"))
        {
            Personality->Caution = FMath::Clamp(Personality->Caution + TraitChange.Value, 0.0f, 1.0f);
        }
        else if (TraitChange.Key == TEXT("Creativity"))
        {
            Personality->Creativity = FMath::Clamp(Personality->Creativity + TraitChange.Value, 0.0f, 1.0f);
        }
        else if (TraitChange.Key == TEXT("Discipline"))
        {
            Personality->Discipline = FMath::Clamp(Personality->Discipline + TraitChange.Value, 0.0f, 1.0f);
        }
        else if (TraitChange.Key == TEXT("Adaptability"))
        {
            Personality->Adaptability = FMath::Clamp(Personality->Adaptability + TraitChange.Value, 0.0f, 1.0f);
        }
        else if (TraitChange.Key == TEXT("RiskTolerance"))
        {
            Personality->RiskTolerance = FMath::Clamp(Personality->RiskTolerance + TraitChange.Value, 0.0f, 1.0f);
        }
        else if (TraitChange.Key == TEXT("Teamwork"))
        {
            Personality->Teamwork = FMath::Clamp(Personality->Teamwork + TraitChange.Value, 0.0f, 1.0f);
        }
        else if (TraitChange.Key == TEXT("Leadership"))
        {
            Personality->Leadership = FMath::Clamp(Personality->Leadership + TraitChange.Value, 0.0f, 1.0f);
        }
    }
}

void UMingRTSAIEnhancedSystem::ConsolidateMemories(const FString& PersonalityID)
{
    // 記憶整合邏輯
    // 合併相似記憶，提升重要記憶
    
    TArray<FAIMemory> PersonalityMemories;
    for (const TPair<FString, FAIMemory>& MemoryPair : AIMemories)
    {
        if (MemoryPair.Key.Contains(PersonalityID) && MemoryPair.Value.bIsActive)
        {
            PersonalityMemories.Add(MemoryPair.Value);
        }
    }
    
    // 簡化實現：只是記錄整合過程
    if (PersonalityMemories.Num() > 20)
    {
        UE_LOG(LogTemp, Log, TEXT("Consolidating memories for personality: %s"), *PersonalityID);
    }
}

void UMingRTSAIEnhancedSystem::SwitchStrategy(const FString& PersonalityID, const FString& NewStrategyID)
{
    ActivateAIStrategy(NewStrategyID);
}

void UMingRTSAIEnhancedSystem::HandleCrisisSituation(const FString& PersonalityID)
{
    FAIPersonality* Personality = AIPersonalities.Find(PersonalityID);
    if (!Personality)
    {
        return;
    }
    
    // 危機處理：切換到更謹慎的狀態
    UpdateEmotionalState(PersonalityID, EAIEmotionalState::Anxious);
    
    // 創建危機記憶
    CreateAIMemory(PersonalityID, EAIMemoryType::Strategic, TEXT("Crisis situation detected"));
    
    UE_LOG(LogTemp, Log, TEXT("Handling crisis situation for personality: %s"), *PersonalityID);
}

void UMingRTSAIEnhancedSystem::IdentifyOpportunities(const FString& PersonalityID)
{
    // 機會識別邏輯
    UE_LOG(LogTemp, Log, TEXT("Identifying opportunities for personality: %s"), *PersonalityID);
}

FString UMingRTSAIEnhancedSystem::GetPersonalityName(const FString& PersonalityID) const
{
    return FString::Printf(TEXT("AI_%s"), *PersonalityID);
}

FString UMingRTSAIEnhancedSystem::GetMemorySummary(const FAIMemory& Memory) const
{
    return FString::Printf(TEXT("%s (Importance: %.2f)"), *Memory.Description, Memory.Importance);
}

float UMingRTSAIEnhancedSystem::CalculateMemorySimilarity(const FAIMemory& Memory1, const FAIMemory& Memory2) const
{
    // 簡化的相似度計算
    if (Memory1.Description == Memory2.Description)
    {
        return 1.0f;
    }
    
    // 基於標籤的相似度
    int32 CommonTags = 0;
    for (const FString& Tag1 : Memory1.Tags)
    {
        for (const FString& Tag2 : Memory2.Tags)
        {
            if (Tag1 == Tag2)
            {
                CommonTags++;
                break;
            }
        }
    }
    
    int32 TotalTags = Memory1.Tags.Num() + Memory2.Tags.Num() - CommonTags;
    if (TotalTags > 0)
    {
        return (float)CommonTags / TotalTags;
    }
    
    return 0.0f;
}

TArray<FAITacticalDecision> UMingRTSAIEnhancedSystem::GenerateDecisionOptions(const FString& PersonalityID, const FString& Context) const
{
    TArray<FAITacticalDecision> DecisionOptions;
    
    const FAIPersonality* Personality = AIPersonalities.Find(PersonalityID);
    if (!Personality)
    {
        return DecisionOptions;
    }
    
    // 根據個性和上下文生成決策選項
    FAITacticalDecision Decision = GetDecisionTemplate(Personality->PrimaryPattern);
    Decision.Description = FString::Printf(TEXT("Decision based on %s pattern"), *LexToString(Personality->PrimaryPattern));
    
    DecisionOptions.Add(Decision);
    
    return DecisionOptions;
}

float UMingRTSAIEnhancedSystem::EvaluateDecisionRisk(const FAITacticalDecision& Decision) const
{
    return Decision.RiskLevel;
}

TArray<FString> UMingRTSAIEnhancedSystem::PredictDecisionOutcome(const FAITacticalDecision& Decision) const
{
    TArray<FString> Outcomes;
    
    // 簡化實現：返回基本預測結果
    Outcomes.Add(TEXT("Success"));
    Outcomes.Add(TEXT("Partial Success"));
    Outcomes.Add(TEXT("Failure"));
    
    return Outcomes;
}

void UMingRTSAIEnhancedSystem::OptimizeDecisionExecution(const FString& DecisionID)
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing decision execution: %s"), *DecisionID);
}

TArray<FAIMemory> UMingRTSAIEnhancedSystem::OptimizeMemoryRetrieval(const FString& PersonalityID, const FString& Query) const
{
    return RetrieveAIMemories(PersonalityID, EAIMemoryType::ShortTerm, Query);
}

void UMingRTSAIEnhancedSystem::DevelopPersonality(const FString& PersonalityID)
{
    FAIPersonality* Personality = AIPersonalities.Find(PersonalityID);
    if (!Personality)
    {
        return;
    }
    
    // 個性自然發展
    float DevelopmentRate = Personality->LearningRate * 0.01f;
    
    // 微調特質
    Personality->Aggression += FMath::RandRange(-DevelopmentRate, DevelopmentRate);
    Personality->Caution += FMath::RandRange(-DevelopmentRate, DevelopmentRate);
    Personality->Creativity += FMath::RandRange(-DevelopmentRate, DevelopmentRate);
    
    // 確保值在有效範圍內
    Personality->Aggression = FMath::Clamp(Personality->Aggression, 0.0f, 1.0f);
    Personality->Caution = FMath::Clamp(Personality->Caution, 0.0f, 1.0f);
    Personality->Creativity = FMath::Clamp(Personality->Creativity, 0.0f, 1.0f);
}

void UMingRTSAIEnhancedSystem::RegulateEmotions(const FString& PersonalityID)
{
    FAIPersonality* Personality = AIPersonalities.Find(PersonalityID);
    if (!Personality)
    {
        return;
    }
    
    // 情感調節邏輯
    if (Personality->CurrentEmotionalState == EAIEmotionalState::Aggressive)
    {
        // 激進狀態有機率冷卻
        if (FMath::RandRange(0.0f, 1.0f) < 0.05f) // 5%機率
        {
            UpdateEmotionalState(PersonalityID, EAIEmotionalState::Calm);
        }
    }
}

void UMingRTSAIEnhancedSystem::ManageStress(const FString& PersonalityID)
{
    FAIPersonality* Personality = AIPersonalities.Find(PersonalityID);
    if (!Personality)
    {
        return;
    }
    
    // 壓力管理邏輯
    if (Personality->CurrentEmotionalState == EAIEmotionalState::Stressed)
    {
        // 壓力狀態下降低風險承受度
        Personality->RiskTolerance = FMath::Max(Personality->RiskTolerance - 0.01f, 0.0f);
    }
}

TMap<FString, float> UMingRTSAIEnhancedSystem::AnalyzeMotivations(const FString& PersonalityID) const
{
    TMap<FString, float> Motivations;
    
    const FAIPersonality* Personality = AIPersonalities.Find(PersonalityID);
    if (!Personality)
    {
        return Motivations;
    }
    
    // 根據個性分析動機
    Motivations.Add(TEXT("Survival"), 0.8f);
    Motivations.Add(TEXT("Expansion"), Personality->Aggression);
    Motivations.Add(TEXT("Security"), Personality->Caution);
    Motivations.Add(TEXT("Power"), Personality->Leadership);
    Motivations.Add(TEXT("Knowledge"), Personality->Creativity);
    
    return Motivations;
}

void UMingRTSAIEnhancedSystem::ResolveConflicts(const FString& PersonalityID)
{
    UE_LOG(LogTemp, Log, TEXT("Resolving conflicts for personality: %s"), *PersonalityID);
}

void UMingRTSAIEnhancedSystem::SetAIGoals(const FString& PersonalityID)
{
    UE_LOG(LogTemp, Log, TEXT("Setting AI goals for personality: %s"), *PersonalityID);
}

float UMingRTSAIEnhancedSystem::EvaluateAIPerformance(const FString& PersonalityID) const
{
    TMap<FString, float> Statistics = GetAIStatistics(PersonalityID);
    
    float Performance = 0.0f;
    int32 StatCount = 0;
    
    for (const TPair<FString, float>& Stat : Statistics)
    {
        if (Stat.Key.Contains(TEXT("SuccessRate")) || Stat.Key.Contains(TEXT("Leadership")) || 
            Stat.Key.Contains(TEXT("Teamwork")) || Stat.Key.Contains(TEXT("Adaptability")))
        {
            Performance += Stat.Value;
            StatCount++;
        }
    }
    
    if (StatCount > 0)
    {
        Performance /= StatCount;
    }
    
    return Performance;
}

void UMingRTSAIEnhancedSystem::OptimizeStrategies(const FString& PersonalityID)
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing strategies for personality: %s"), *PersonalityID);
}

void UMingRTSAIEnhancedSystem::CleanupMemories(const FString& PersonalityID)
{
    // 清理低重要性記憶
    for (auto It = AIMemories.CreateIterator(); It; ++It)
    {
        if (It->Key.Contains(PersonalityID) && It->Value.Importance < 0.1f)
        {
            It.RemoveCurrent();
        }
    }
}

void UMingRTSAIEnhancedSystem::CleanupDecisions(const FString& PersonalityID)
{
    // 清理舊決策
    float CurrentTime = CurrentWorld->GetTimeSeconds();
    
    for (auto It = TacticalDecisions.CreateIterator(); It; ++It)
    {
        if (It->Key.Contains(PersonalityID))
        {
            float DecisionAge = CurrentTime - It->Value.StartTime;
            if (DecisionAge > 600.0f) // 10分鐘前的決策
            {
                It.RemoveCurrent();
            }
        }
    }
}

FAIPersonality UMingRTSAIEnhancedSystem::GetPersonalityTemplate(EAIType AIType) const
{
    FAIPersonality Template;
    Template.AIType = AIType;
    
    switch (AIType)
    {
        case EAIType::Aggressive:
            Template.Aggression = 0.8f;
            Template.Caution = 0.2f;
            Template.RiskTolerance = 0.7f;
            Template.PrimaryPattern = EAIBehaviorPattern::Aggressive;
            break;
        case EAIType::Defensive:
            Template.Aggression = 0.2f;
            Template.Caution = 0.8f;
            Template.RiskTolerance = 0.3f;
            Template.PrimaryPattern = EAIBehaviorPattern::Defensive;
            break;
        case EAIType::Balanced:
            Template.Aggression = 0.5f;
            Template.Caution = 0.5f;
            Template.RiskTolerance = 0.5f;
            Template.PrimaryPattern = EAIBehaviorPattern::Balanced;
            break;
        case EAIType::Cowardly:
            Template.Aggression = 0.1f;
            Template.Caution = 0.9f;
            Template.RiskTolerance = 0.1f;
            Template.PrimaryPattern = EAIBehaviorPattern::Turtle;
            break;
        case EAIType::Tactical:
            Template.Creativity = 0.8f;
            Template.Discipline = 0.9f;
            Template.Adaptability = 0.7f;
            Template.PrimaryPattern = EAIBehaviorPattern::Tactical;
            break;
        case EAIType::Berserker:
            Template.Aggression = 1.0f;
            Template.Caution = 0.0f;
            Template.RiskTolerance = 1.0f;
            Template.PrimaryPattern = EAIBehaviorPattern::Rush;
            break;
        default:
            break;
    }
    
    return Template;
}

FAIStrategy UMingRTSAIEnhancedSystem::GetStrategyTemplate(EAIBehaviorPattern Pattern) const
{
    FAIStrategy Template;
    Template.Pattern = Pattern;
    
    switch (Pattern)
    {
        case EAIBehaviorPattern::Rush:
            Template.Name = TEXT("Rush Strategy");
            Template.Description = TEXT("Fast early game aggression");
            Template.OverallPriority = 0.7f;
            break;
        case EAIBehaviorPattern::Turtle:
            Template.Name = TEXT("Turtle Strategy");
            Template.Description = TEXT("Defensive buildup");
            Template.OverallPriority = 0.6f;
            break;
        case EAIBehaviorPattern::Boom:
            Template.Name = TEXT("Boom Strategy");
            Template.Description = TEXT("Economic explosion");
            Template.OverallPriority = 0.8f;
            break;
        case EAIBehaviorPattern::Balanced:
            Template.Name = TEXT("Balanced Strategy");
            Template.Description = TEXT("Balanced approach");
            Template.OverallPriority = 0.5f;
            break;
        default:
            Template.Name = TEXT("Default Strategy");
            Template.Description = TEXT("Default AI strategy");
            Template.OverallPriority = 0.5f;
            break;
    }
    
    return Template;
}

FAITacticalDecision UMingRTSAIEnhancedSystem::GetDecisionTemplate(EAIBehaviorPattern Pattern) const
{
    FAITacticalDecision Template;
    Template.Pattern = Pattern;
    
    switch (Pattern)
    {
        case EAIBehaviorPattern::Aggressive:
            Template.Description = TEXT("Aggressive tactical decision");
            Template.Priority = 0.7f;
            Template.RiskLevel = 0.6f;
            break;
        case EAIBehaviorPattern::Defensive:
            Template.Description = TEXT("Defensive tactical decision");
            Template.Priority = 0.5f;
            Template.RiskLevel = 0.3f;
            break;
        case EAIBehaviorPattern::Balanced:
            Template.Description = TEXT("Balanced tactical decision");
            Template.Priority = 0.5f;
            Template.RiskLevel = 0.5f;
            break;
        default:
            Template.Description = TEXT("Default tactical decision");
            Template.Priority = 0.5f;
            Template.RiskLevel = 0.5f;
            break;
    }
    
    return Template;
}
