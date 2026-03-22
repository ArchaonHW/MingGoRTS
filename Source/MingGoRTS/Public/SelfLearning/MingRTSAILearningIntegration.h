// Copyright (c) 2026 MingGoRTS. All rights reserved.
// AI Learning Integration System

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSAILearningIntegration.generated.h"

class UMingRTSSelfLearningSystem;

UENUM(BlueprintType)
enum class EPlayerStrategyPattern : uint8
{
    AggressiveRush, UMETA(DisplayName = "Aggressive Rush"),
    DefensiveTurtle, UMETA(DisplayName = "Defensive Turtle"),
    EconomicBoom, UMETA(DisplayName = "Economic Boom"),
    BalancedApproach, UMETA(DisplayName = "Balanced Approach"),
    MicroIntensive, UMETA(DisplayName = "Micro Intensive"),
    MacroIntensive, UMETA(DisplayName = "Macro Intensive"),
    AdaptivePlay, UMETA(DisplayName = "Adaptive Play"),
    UnknownPattern, UMETA(DisplayName = "Unknown Pattern")
};

UENUM(BlueprintType)
enum class EAICertaintyLevel : uint8
{
    VeryLow, UMETA(DisplayName = "Very Low"),
    Low, UMETA(DisplayName = "Low"),
    Medium, UMETA(DisplayName = "Medium"),
    High, UMETA(DisplayName = "High"),
    VeryHigh, UMETA(DisplayName = "Very High")
};

USTRUCT(BlueprintType)
struct FPlayerBehaviorAnalysis
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Player Behavior Analysis")
    FString PlayerID;

    UPROPERTY(BlueprintReadOnly, Category = "Player Behavior Analysis")
    EPlayerStrategyPattern StrategyPattern;

    UPROPERTY(BlueprintReadOnly, Category = "Player Behavior Analysis")
    EAICertaintyLevel ConfidenceLevel;

    UPROPERTY(BlueprintReadOnly, Category = "Player Behavior Analysis")
    float AggressionScore;

    UPROPERTY(BlueprintReadOnly, Category = "Player Behavior Analysis")
    float EconomicEfficiency;

    UPROPERTY(BlueprintReadOnly, Category = "Player Behavior Analysis")
    float MicroSkill;

    UPROPERTY(BlueprintReadOnly, Category = "Player Behavior Analysis")
    float MacroSkill;

    FPlayerBehaviorAnalysis()
        : PlayerID(TEXT(""))
        , StrategyPattern(EPlayerStrategyPattern::UnknownPattern)
        , ConfidenceLevel(EAICertaintyLevel::Low)
        , AggressionScore(0.0f)
        , EconomicEfficiency(0.0f)
        , MicroSkill(0.0f)
        , MacroSkill(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FAIAdaptationParameters
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "AI Adaptation Parameters")
    float AggressionAdjustment;

    UPROPERTY(BlueprintReadOnly, Category = "AI Adaptation Parameters")
    float DefensePriority;

    UPROPERTY(BlueprintReadOnly, Category = "AI Adaptation Parameters")
    float EconomicFocus;

    UPROPERTY(BlueprintReadOnly, Category = "AI Adaptation Parameters")
    float UnitComposition;

    UPROPERTY(BlueprintReadOnly, Category = "AI Adaptation Parameters")
    float TimingAdjustment;

    FAIAdaptationParameters()
        : AggressionAdjustment(0.0f)
        , DefensePriority(0.0f)
        , EconomicFocus(0.0f)
        , UnitComposition(0.0f)
        , TimingAdjustment(0.0f)
    {}
};

/**
 * MingGoRTS AI Learning Integration
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSAILearningIntegration : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSAILearningIntegration();
    virtual void BeginDestroy() override;

    UFUNCTION(BlueprintCallable, Category = "AI Learning")
    void InitializeAILearning(UMingRTSSelfLearningSystem* SelfLearningSystem);

    UFUNCTION(BlueprintCallable, Category = "AI Learning")
    void ShutdownAILearning();

    UFUNCTION(BlueprintCallable, Category = "AI Learning|Analysis")
    void AnalyzePlayerBehavior(const FString& PlayerID, const TMap<FString, FString>& MatchData);

    UFUNCTION(BlueprintCallable, Category = "AI Learning|Analysis")
    FPlayerBehaviorAnalysis GetPlayerAnalysis(const FString& PlayerID) const;

    UFUNCTION(BlueprintCallable, Category = "AI Learning|Analysis")
    TArray<FPlayerBehaviorAnalysis> GetAllPlayerAnalyses() const;

    UFUNCTION(BlueprintCallable, Category = "AI Learning|Adaptation")
    void AdaptAIToPlayer(const FString& PlayerID);

    UFUNCTION(BlueprintCallable, Category = "AI Learning|Adaptation")
    FAIAdaptationParameters GetRecommendedAdaptations(const FString& PlayerID) const;

    UFUNCTION(BlueprintCallable, Category = "AI Learning|Match")
    void RecordMatchStart(const FString& PlayerID, const FString& MatchID);

    UFUNCTION(BlueprintCallable, Category = "AI Learning|Match")
    void RecordMatchEnd(const FString& PlayerID, const FString& MatchID, bool bVictory);

protected:
    UPROPERTY()
    UMingRTSSelfLearningSystem* LearningSystem;

    UPROPERTY()
    TMap<FString, FPlayerBehaviorAnalysis> PlayerAnalyses;

    UPROPERTY()
    TMap<FString, FAIAdaptationParameters> AdaptationHistory;

    void ProcessPlayerData(const FString& PlayerID, const TMap<FString, FString>& Data);
    void CalculateStrategyPattern(FPlayerBehaviorAnalysis& Analysis);
    void UpdateAdaptationParameters(const FString& PlayerID);
    float CalculateAggressionScore(const TMap<FString, FString>& Data);
    float CalculateEconomicEfficiency(const TMap<FString, FString>& Data);
    float CalculateMicroSkill(const TMap<FString, FString>& Data);
    float CalculateMacroSkill(const TMap<FString, FString>& Data);
};
