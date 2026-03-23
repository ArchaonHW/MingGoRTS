#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAIAdapterSystem.generated.h"

// AI 难度等级
UENUM(BlueprintType)
enum class EAIAdapterDifficulty : uint8
{
    Easy,      // 简单
    Normal,    // 普通
    Hard,      // 困难
    Expert,    // 专家
    Master     // 大师
};

// AI 行为类型
UENUM(BlueprintType)
enum class EAIAdapterBehavior : uint8
{
    Aggressive,    // 侵略型
    Defensive,     // 防御型
    Balanced,      // 平衡型
    Adaptive       // 适应型
};

// AI 策略选择
UENUM(BlueprintType)
enum class EAIAdapterStrategy : uint8
{
    Righteous,    // 正策倾向
    Evil,         // 逆策倾向
    Mixed         // 混合策略
};

// AI 适配数据
USTRUCT(BlueprintType)
struct MINGGORTS_API FAIAdapterData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EAIAdapterDifficulty Difficulty;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EAIAdapterBehavior Behavior;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EAIAdapterStrategy Strategy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReactionSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DecisionAccuracy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LearningRate;
};

// AI 适配系统主类
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingAIAdapterSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingAIAdapterSystem();

    UFUNCTION(BlueprintCallable, Category = "AI Adapter")
    void InitializeSystem();

    // 设置 AI 难度
    UFUNCTION(BlueprintCallable, Category = "AI Adapter")
    void SetAIDifficulty(EAIAdapterDifficulty Difficulty);

    // 设置 AI 行为
    UFUNCTION(BlueprintCallable, Category = "AI Adapter")
    void SetAIBehavior(EAIAdapterBehavior Behavior);

    // 设置 AI 策略
    UFUNCTION(BlueprintCallable, Category = "AI Adapter")
    void SetAIStrategy(EAIAdapterStrategy Strategy);

    // 获取 AI 决策
    UFUNCTION(BlueprintCallable, Category = "AI Adapter")
    FString GetAIDecision(const FString& GameState);

    // 学习玩家行为
    UFUNCTION(BlueprintCallable, Category = "AI Adapter")
    void LearnPlayerBehavior(const FString& Action, float SuccessRate);

    // 适应游戏节奏
    UFUNCTION(BlueprintCallable, Category = "AI Adapter")
    void AdaptToGameRhythm(float CurrentRhythm);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Adapter")
    FAIAdapterData AdapterData;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Adapter")
    bool bSystemInitialized;

private:
    void InitializeDefaultValues();
    void CalculateBehaviorWeights();
};
