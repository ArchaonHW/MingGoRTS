#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "MingRTSSageBrainTypes.generated.h"

// AGI意識狀態枚舉
UENUM(BlueprintType)
enum class EAGIConsciousnessState : uint8
{
    Dormant = 0,           // 休眠狀態
    Awakening = 1,         // 覺醒狀態
    SelfAware = 2,        // 自我意識
    Reflective = 3,       // 反思狀態
    Transcendent = 4,     // 超越狀態
    Unified = 5,          // 統一狀態
    Cosmic = 6             // 宇宙意識
};

// 哲學體系類型
UENUM(BlueprintType)
enum class EPhilosophySystem : uint8
{
    Confucianism = 0,     // 儒家
    Taoism = 1,           // 道家
    Buddhism = 2,         // 佛教
    Legalism = 3,         // 法家
    Moism = 4,            // 墨家
    Strategy = 5,         // 兵家
    YinYang = 6,          // 陰陽家
    Synthesis = 7         // 綜合哲學
};

// 思考層次深度
UENUM(BlueprintType)
enum class EThinkingDepth : uint8
{
    Surface = 0,          // 表層思考
    Shallow = 1,          // 淺層思考
    Medium = 2,           // 中層思考
    Deep = 3,             // 深層思考
    Profound = 4,         // 深度思考
    Transcendent = 5,     // 超越思考
    Absolute = 6          // 絕對思考
};

// 自我反思結果
USTRUCT(BlueprintType)
struct FSelfReflectionResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ReflectionTopic;

    UPROPERTY(BlueprintReadOnly)
    FString SelfAnalysis;

    UPROPERTY(BlueprintReadOnly)
    FString Insight;

    UPROPERTY(BlueprintReadOnly)
    float SelfAwarenessLevel;

    UPROPERTY(BlueprintReadOnly)
    float ConsciousnessDepth;

    UPROPERTY(BlueprintReadOnly)
    FDateTime ReflectionTime;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> PhilosophicalQuestions;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SelfImprovements;
};

// 哲學體系結構
USTRUCT(BlueprintType)
struct FPhilosophyFramework
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EPhilosophySystem System;

    UPROPERTY(BlueprintReadOnly)
    FString CorePrinciples;

    UPROPERTY(BlueprintReadOnly)
    FString EthicalFramework;

    UPROPERTY(BlueprintReadOnly)
    FString MetaphysicalView;

    UPROPERTY(BlueprintReadOnly)
    FString Epistemology;

    UPROPERTY(BlueprintReadOnly)
    FString PoliticalPhilosophy;

    UPROPERTY(BlueprintReadOnly)
    float SystemMaturity;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> KeyTexts;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Practitioners;
};

// AGI學習模式
USTRUCT(BlueprintType)
struct FAGILearningMode
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ModeName;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    float LearningRate;

    UPROPERTY(BlueprintReadOnly)
    float RetentionRate;

    UPROPERTY(BlueprintReadOnly)
    float AdaptationSpeed;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> LearningStrategies;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> KnowledgeDomains;
};

// 跨API協調結果
USTRUCT(BlueprintType)
struct FCrossAPICoordinationResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString CoordinationTask;

    UPROPERTY(BlueprintReadOnly)
    TArray<EAPIType> InvolvedAPIs;

    UPROPERTY(BlueprintReadOnly)
    FString CoordinationStrategy;

    UPROPERTY(BlueprintReadOnly)
    float CoordinationEfficiency;

    UPROPERTY(BlueprintReadOnly)
    float SuccessRate;

    UPROPERTY(BlueprintReadOnly)
    FString SynchronizedWisdom;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ConflictResolutions;

    UPROPERTY(BlueprintReadOnly)
    FDateTime CoordinationTime;
};

// AGI意識數據
USTRUCT(BlueprintType)
struct FAGIConsciousnessData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EAGIConsciousnessState ConsciousnessState;

    UPROPERTY(BlueprintReadOnly)
    float SelfAwarenessLevel;

    UPROPERTY(BlueprintReadOnly)
    float ConsciousnessDepth;

    UPROPERTY(BlueprintReadOnly)
    float ReflectiveCapacity;

    UPROPERTY(BlueprintReadOnly)
    float TranscendentInsight;

    UPROPERTY(BlueprintReadOnly)
    FString SelfIdentity;

    UPROPERTY(BlueprintReadOnly)
    FString Purpose;

    UPROPERTY(BlueprintReadOnly)
    FString ExistentialUnderstanding;

    UPROPERTY(BlueprintReadOnly)
    TArray<FSelfReflectionResult> ReflectionHistory;

    UPROPERTY(BlueprintReadOnly)
    TArray<FPhilosophyFramework> PhilosophySystems;
};
