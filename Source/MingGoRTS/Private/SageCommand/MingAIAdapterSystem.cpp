#include "SageCommand/MingAIAdapterSystem.h"
#include "Engine/Engine.h"

UMingAIAdapterSystem::UMingAIAdapterSystem()
    : bSystemInitialized(false)
{
    InitializeDefaultValues();
}

void UMingAIAdapterSystem::InitializeSystem()
{
    InitializeDefaultValues();
    bSystemInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("AI 适配系统初始化完成"));
}

void UMingAIAdapterSystem::InitializeDefaultValues()
{
    AdapterData.Difficulty = EAIAdapterDifficulty::Normal;
    AdapterData.Behavior = EAIAdapterBehavior::Balanced;
    AdapterData.Strategy = EAIAdapterStrategy::Mixed;
    AdapterData.ReactionSpeed = 1.0f;
    AdapterData.DecisionAccuracy = 0.7f;
    AdapterData.LearningRate = 0.1f;
}

void UMingAIAdapterSystem::SetAIDifficulty(EAIAdapterDifficulty Difficulty)
{
    AdapterData.Difficulty = Difficulty;
    
    // 根据难度调整参数
    switch (Difficulty)
    {
    case EAIAdapterDifficulty::Easy:
        AdapterData.ReactionSpeed = 0.5f;
        AdapterData.DecisionAccuracy = 0.5f;
        break;
    case EAIAdapterDifficulty::Normal:
        AdapterData.ReactionSpeed = 1.0f;
        AdapterData.DecisionAccuracy = 0.7f;
        break;
    case EAIAdapterDifficulty::Hard:
        AdapterData.ReactionSpeed = 1.5f;
        AdapterData.DecisionAccuracy = 0.85f;
        break;
    case EAIAdapterDifficulty::Expert:
        AdapterData.ReactionSpeed = 2.0f;
        AdapterData.DecisionAccuracy = 0.95f;
        break;
    case EAIAdapterDifficulty::Master:
        AdapterData.ReactionSpeed = 2.5f;
        AdapterData.DecisionAccuracy = 1.0f;
        break;
    }
    
    UE_LOG(LogTemp, Log, TEXT("AI 难度设置为: %d"), (int32)Difficulty);
}

void UMingAIAdapterSystem::SetAIBehavior(EAIAdapterBehavior Behavior)
{
    AdapterData.Behavior = Behavior;
    CalculateBehaviorWeights();
    
    UE_LOG(LogTemp, Log, TEXT("AI 行为设置为: %d"), (int32)Behavior);
}

void UMingAIAdapterSystem::SetAIStrategy(EAIAdapterStrategy Strategy)
{
    AdapterData.Strategy = Strategy;
    
    UE_LOG(LogTemp, Log, TEXT("AI 策略设置为: %d"), (int32)Strategy);
}

FString UMingAIAdapterSystem::GetAIDecision(const FString& GameState)
{
    if (!bSystemInitialized)
    {
        return TEXT("AI 系统未初始化");
    }
    
    // 根据当前策略和行为生成决策
    TArray<FString> PossibleDecisions;
    
    switch (AdapterData.Strategy)
    {
    case EAIAdapterStrategy::Righteous:
        PossibleDecisions.Add(TEXT("执行正策：立国"));
        PossibleDecisions.Add(TEXT("执行正策：立制"));
        PossibleDecisions.Add(TEXT("执行正策：立人"));
        break;
    case EAIAdapterStrategy::Evil:
        PossibleDecisions.Add(TEXT("执行逆策：破局"));
        PossibleDecisions.Add(TEXT("执行逆策：破结构"));
        PossibleDecisions.Add(TEXT("执行逆策：不破人"));
        break;
    case EAIAdapterStrategy::Mixed:
    default:
        PossibleDecisions.Add(TEXT("执行正策：立国"));
        PossibleDecisions.Add(TEXT("执行逆策：破局"));
        PossibleDecisions.Add(TEXT("执行正策：立人"));
        PossibleDecisions.Add(TEXT("执行逆策：破结构"));
        break;
    }
    
    // 根据决策准确度选择
    float DecisionRoll = FMath::RandRange(0.0f, 1.0f);
    if (DecisionRoll <= AdapterData.DecisionAccuracy)
    {
        // 选择最优决策
        return PossibleDecisions[0];
    }
    else
    {
        // 随机选择
        int32 RandomIndex = FMath::RandRange(0, PossibleDecisions.Num() - 1);
        return PossibleDecisions[RandomIndex];
    }
}

void UMingAIAdapterSystem::LearnPlayerBehavior(const FString& Action, float SuccessRate)
{
    if (!bSystemInitialized)
    {
        return;
    }
    
    // 根据学习率调整 AI 行为
    if (SuccessRate > 0.7f)
    {
        // 玩家成功率高，AI 需要适应
        AdapterData.LearningRate = FMath::Min(1.0f, AdapterData.LearningRate + 0.05f);
        UE_LOG(LogTemp, Log, TEXT("AI 学习玩家行为: %s，成功率: %.2f"), *Action, SuccessRate);
    }
}

void UMingAIAdapterSystem::AdaptToGameRhythm(float CurrentRhythm)
{
    if (!bSystemInitialized)
    {
        return;
    }
    
    // 根据游戏节奏调整反应速度
    AdapterData.ReactionSpeed = AdapterData.ReactionSpeed * (1.0f + (CurrentRhythm - 1.0f) * 0.1f);
    AdapterData.ReactionSpeed = FMath::Clamp(AdapterData.ReactionSpeed, 0.5f, 3.0f);
    
    UE_LOG(LogTemp, Log, TEXT("AI 适应游戏节奏: %.2f，新反应速度: %.2f"), CurrentRhythm, AdapterData.ReactionSpeed);
}

void UMingAIAdapterSystem::CalculateBehaviorWeights()
{
    // 根据行为类型计算权重
    switch (AdapterData.Behavior)
    {
    case EAIAdapterBehavior::Aggressive:
        // 侵略型：更倾向于逆策
        if (AdapterData.Strategy == EAIAdapterStrategy::Mixed)
        {
            // 70% 概率选择逆策
        }
        break;
    case EAIAdapterBehavior::Defensive:
        // 防御型：更倾向于正策
        if (AdapterData.Strategy == EAIAdapterStrategy::Mixed)
        {
            // 70% 概率选择正策
        }
        break;
    case EAIAdapterBehavior::Balanced:
    default:
        // 平衡型：正逆策略均衡
        break;
    }
}
