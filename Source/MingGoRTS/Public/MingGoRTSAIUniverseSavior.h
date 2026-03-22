// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic X: Universe Savior System

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSAIWorldSavior.h"
#include "MingGoRTSAIUniverseSavior.generated.h"

UENUM(BlueprintType)
enum class EUniverseThreatLevel : uint8
{
    None            UMETA(DisplayName = "None"),
    Stellar         UMETA(DisplayName = "Stellar"),          // Stellar level threats
    Galactic        UMETA(DisplayName = "Galactic"),          // Galactic level threats
    Intergalactic   UMETA(DisplayName = "Intergalactic"),    // Intergalactic level threats
    Cosmic          UMETA(DisplayName = "Cosmic"),          // Cosmic level threats
    Multiverse      UMETA(DisplayName = "Multiverse"),      // Multiverse level threats
    Omniversal      UMETA(DisplayName = "Omniversal"),      // Omniversal level threats
    MAX             UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EUniverseDefenseStrategy : uint8
{
    Passive         UMETA(DisplayName = "Passive"),
    Defensive       UMETA(DisplayName = "Defensive"),
    Balanced        UMETA(DisplayName = "Balanced"),
    Offensive       UMETA(DisplayName = "Offensive"),
    Aggressive      UMETA(DisplayName = "Aggressive"),
    MAX             UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FUniverseThreatData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Universe Threat")
    EUniverseThreatLevel ThreatLevel;

    UPROPERTY(BlueprintReadOnly, Category = "Universe Threat")
    float ThreatIntensity;

    UPROPERTY(BlueprintReadOnly, Category = "Universe Threat")
    FVector ThreatLocation;

    UPROPERTY(BlueprintReadOnly, Category = "Universe Threat")
    FDateTime DetectionTime;

    FUniverseThreatData()
        : ThreatLevel(EUniverseThreatLevel::None)
        , ThreatIntensity(0.0f)
        , ThreatLocation(FVector::ZeroVector)
        , DetectionTime(FDateTime::Now())
    {}
};

UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSAIUniverseSavior : public UMingGoRTSAIWorldSavior
{
    GENERATED_BODY()

public:
    UMingGoRTSAIUniverseSavior();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void InitializeUniverseSavior();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void DetectUniverseThreats();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void RespondToThreat(const FUniverseThreatData& Threat);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void SetDefenseStrategy(EUniverseDefenseStrategy Strategy);

    UFUNCTION(BlueprintPure, Category = "Universe Savior")
    EUniverseDefenseStrategy GetDefenseStrategy() const;

    UFUNCTION(BlueprintPure, Category = "Universe Savior")
    TArray<FUniverseThreatData> GetActiveThreats() const;

    UFUNCTION(BlueprintPure, Category = "Universe Savior")
    bool IsUniverseSafe() const;

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Universe Savior")
    EUniverseDefenseStrategy CurrentStrategy;

    UPROPERTY(BlueprintReadOnly, Category = "Universe Savior")
    TArray<FUniverseThreatData> ActiveThreats;

    UPROPERTY(BlueprintReadOnly, Category = "Universe Savior")
    bool bUniverseSafe;

private:
    void AnalyzeThreatPatterns();
    void CoordinateDefenseResponse();
    void UpdateThreatAssessment();
};
