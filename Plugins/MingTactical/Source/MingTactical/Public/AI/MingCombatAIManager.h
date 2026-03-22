#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCombatAIManager.generated.h"

// AI ????等??
UENUM(BlueprintType)
enum class EMingAIDifficulty : uint8
{
    Recruit,        // ??兵
    Regular,        // 常??
    Veteran,        // ??兵
    Elite,          // 精銳
    Legendary       // ????
};

// AI 節點????UENUM(BlueprintType)
enum class EMingAINodeResult : uint8
{
    Running,        // ??????    Success,        // ????
    Failure         // 失??
};

// AI ??略??好
UENUM(BlueprintType)
enum class EMingStrategicPreference : uint8
{
    Aggressive,     // ??????    Defensive,      // ??禦??    Balanced,       // 平衡??    Economic,       // 經????    Diplomatic,  // 外交??};

// AI ????風格
UENUM(BlueprintType)
enum class EMingTacticalStyle : uint8
{
    DirectAssault,  // ??接????
    Flanking,       // ??翼????
    Guerrilla,       // 游????    Siege,          // ??攻??    Mobile,  // 機????};

// 軍閥類??
UENUM(BlueprintType)
enum class EMingWarlordType : uint8
{
    Northeastern,    // ????軍閥
    Northwestern,    // 西??軍閥
    Southwestern,    // 西??軍閥
    Central,         // 中央軍閥
    Independent      // ????軍閥
};

// AI 統??信息
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

// AI 上????USTRUCT(BlueprintType)
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

// AI ????設置
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
 * ??鬥 AI 管???? * 負責管??????戰??AI ??制????行為
 */
UCLASS(BlueprintType, Blueprintable)
class MINGTACTICAL_API UMingCombatAIManager : public UObject
{
    GENERATED_BODY()

public:
    // 建????    UMingCombatAIManager(};

    // ??????AI 管????    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void Initialize(};

    // AI ??新主循??    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void UpdateCombatAI(float DeltaTime};

    // 註?? AI ??制??    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void RegisterAIController(class AController* AIController};

    // 移除 AI ??制??    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void UnregisterAIController(class AController* AIController};

    // 設置???? AI ????
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void SetAIDifficulty(EMingAIDifficulty Difficulty};

    // ???? AI ????設置
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    FMingAIDifficultySettings GetDifficultySettings(EMingAIDifficulty Difficulty) const;

    // ???? AI 統??信息
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    FMingAIStatistics GetAIStatistics() const;

    // ???? AI ??新
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void PauseAI(};

    // ??復 AI ??新
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ResumeAI(};

    // ??置????AI
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ResetAllAI(};

protected:
    // AI ??制??????    TArray<TWeakObjectPtr<class AController>> AIControllers;

    // AI ??新????
    float AIUpdateInterval;

    // ????????設置
    EMingAIDifficulty CurrentDifficulty;

    // ??否????
    bool bIsPaused;

    // 統??????
    FMingAIStatistics AIStatistics;

    // ????設置????
    TMap<EMingAIDifficulty, FMingAIDifficultySettings> DifficultySettings;

    // ??????難度設??    void InitializeDifficultySettings(};

    // ??新統??信息
    void UpdateStatistics(};

    // ???? AI ??新????
    float GetAIUpdateTime() const;

private:
    // ??否已??始??
    bool bInitialized = false;

    // 累??????
    float AccumulatedTime;
};

