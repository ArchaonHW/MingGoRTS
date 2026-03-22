#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCombatAIManager.generated.h"

// AI X??X
UENUM(BlueprintType)
enum class EMingAIDifficulty : uint8
{
    Recruit,        // X?L
    Regular,        // ?`X
    Veteran,        // X?L
    Elite,          // ??U
    Legendary       // X
};

// AI ?`?IXUENUM(BlueprintType)
enum class EMingAINodeResult : uint8
{
    Running,        // X    Success,        // X
    Failure         // ??X
};

// AI X??X?n
UENUM(BlueprintType)
enum class EMingStrategicPreference : uint8
{
    Aggressive,     // X    Defensive,      // X?mX    Balanced,       // ????X    Economic,       // ?gX    Diplomatic  //~X};

// AI X????
UENUM(BlueprintType)
enum class EMingTacticalStyle : uint8
{
    DirectAssault,  // X??X
    Flanking,       // X?lX
    Guerrilla,       // ??X    Siege,          // X??X    Mobile  //~X};

// ?x????X
UENUM(BlueprintType)
enum class EMingWarlordType : uint8
{
    Northeastern,    // X?x??
    Northwestern,    // ??X?x??
    Southwestern,    // ??X?x??
    Central,         // ?????x??
    Independent      // X?x??
};

// AI ??X?H??
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAIStatistics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 TotalAIUnits;

    UPROPERTY(BlueprintReadOnly)
    int32 ActiveAIControllers;

    UPROPERTY(BlueprintReadOnly)
    float AverageDecisionTime;

    UPROPERTY(BlueprintReadOnly)
    float SuccessRate;

    UPROPERTY(BlueprintReadOnly)
    int32 TotalDecisionsMade;

    UPROPERTY(BlueprintReadOnly)
    int32 SuccessfulAttacks;

    UPROPERTY(BlueprintReadOnly)
    int32 SuccessfulDefenses;
};

// AI ?WXUSTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAIContext
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    class AController* AIController;

    UPROPERTY(BlueprintReadOnly)
    TArray<class AActor*> ControlledUnits;

    UPROPERTY(BlueprintReadOnly)
    TArray<class AActor*> EnemyUnits;

    UPROPERTY(BlueprintReadOnly)
    TArray<class AActor*> FriendlyUnits;

    UPROPERTY(BlueprintReadOnly)
    FVector CurrentTarget;

    UPROPERTY(BlueprintReadOnly)
    float CurrentTime;

    UPROPERTY(BlueprintReadOnly)
    EMingAIDifficulty Difficulty;
};

// AI X?]?m
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAIDifficultySettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReactionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DecisionAccuracy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TacticalSkill;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CoordinationAbility;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LearningCapability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MistakeRate;
};

/**
 * X?? AI ??X * ?t?d??X??XAI X??X??
 */
UCLASS(BlueprintType, Blueprintable)
class MINGTACTICAL_API UMingCombatAIManager : public UObject
{
    GENERATED_BODY()

public:
    // ??X    UMingCombatAIManager();

    // XAI ??X    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void Initialize();

    // AI X?s?D?`X    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void UpdateCombatAI(float DeltaTime};

    // ??X AI X??X    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void RegisterAIController(class AController* AIController};

    // ???? AI X??X    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void UnregisterAIController(class AController* AIController};

    // ?]?mX AI X
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void SetAIDifficulty(EMingAIDifficulty Difficulty};

    // X AI X?]?m
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    FMingAIDifficultySettings GetDifficultySettings(EMingAIDifficulty Difficulty) const;

    // X AI ??X?H??
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    FMingAIStatistics GetAIStatistics() const;

    // X AI X?s
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void PauseAI();

    // X?_ AI X?s
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ResumeAI();

    // X?mXAI
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ResetAllAI();

protected:
    // AI X??X    TArray<TWeakObjectPtr<class AController>> AIControllers;

    // AI X?sX
    float AIUpdateInterval;

    // X?]?m
    EMingAIDifficulty CurrentDifficulty;

    // X?_X
    bool bIsPaused;

    // ??X
    FMingAIStatistics AIStatistics;

    // X?]?mX
    TMap<EMingAIDifficulty, FMingAIDifficultySettings> DifficultySettings;

    // X????]X    void InitializeDifficultySettings();

    // X?s??X?H??
    void UpdateStatistics();

    // X AI X?sX
    float GetAIUpdateTime() const;

private:
    // X?_?wX?lX
    bool bInitialized = false;

    // ??X
    float AccumulatedTime;
};

