#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCombatAIManager.generated.h"

// AI ??∫¶Á≠âÁ?
UENUM(BlueprintType)
enum class EMingAIDifficulty : uint8
{
    Recruit,        // ?∞ÂÖµ
    Regular,        // Â∏∏Ë?
    Veteran,        // ?ÅÂÖµ
    Elite,          // Á≤æÈä≥
    Legendary       // ?≥Â?
};

// AI ÁØÄÈªûÁ???UENUM(BlueprintType)
enum class EMingAINodeResult : uint8
{
    Running,        // ?ãË?‰∏?    Success,        // ?êÂ?
    Failure         // Â§±Ê?
};

// AI ?∞Áï•?èÂ•Ω
UENUM(BlueprintType)
enum class EMingStrategicPreference : uint8
{
    Aggressive,     // ?ªÊ???    Defensive,      // ?≤Á¶¶??    Balanced,       // Âπ≥Ë°°??    Economic,       // Á∂ìÊ???    Diplomatic      // Â§ñ‰∫§??};

// AI ?∞Ë?È¢®Ê†º
UENUM(BlueprintType)
enum class EMingTacticalStyle : uint8
{
    DirectAssault,  // ?¥Êé•?ªÊ?
    Flanking,       // ?¥Áøº?ÖÊ?
    Guerrilla,       // Ê∏∏Ê???    Siege,          // ?çÊîª??    Mobile          // Ê©üÂ???};

// ËªçÈñ•È°ûÂ?
UENUM(BlueprintType)
enum class EMingWarlordType : uint8
{
    Northeastern,    // ?±Â?ËªçÈñ•
    Northwestern,    // Ë•øÂ?ËªçÈñ•
    Southwestern,    // Ë•øÂ?ËªçÈñ•
    Central,         // ‰∏≠Â§ÆËªçÈñ•
    Independent      // ?®Á?ËªçÈñ•
};

// AI Áµ±Ë?‰ø°ÊÅØ
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

// AI ‰∏ä‰???USTRUCT(BlueprintType)
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

// AI ??∫¶Ë®≠ÁΩÆ
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
 * ?∞È¨• AI ÁÆ°Á??? * Ë≤†Ë≤¨ÁÆ°Á??Ä?âÊà∞È¨?AI ?ßÂà∂?®Â?Ë°åÁÇ∫
 */
UCLASS(BlueprintType, Blueprintable)
class MINGTACTICAL_API UMingCombatAIManager : public UObject
{
    GENERATED_BODY()

public:
    // Âª∫Ê?Â≠?    UMingCombatAIManager();

    // ?ùÂ???AI ÁÆ°Á???    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void Initialize();

    // AI ?¥Êñ∞‰∏ªÂæ™??    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void UpdateCombatAI(float DeltaTime);

    // Ë®ªÂ? AI ?ßÂà∂??    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void RegisterAIController(class AController* AIController);

    // ÁßªÈô§ AI ?ßÂà∂??    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void UnregisterAIController(class AController* AIController);

    // Ë®≠ÁΩÆ?®Â? AI ??∫¶
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void SetAIDifficulty(EMingAIDifficulty Difficulty);

    // ?≤Â? AI ??∫¶Ë®≠ÁΩÆ
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    FMingAIDifficultySettings GetDifficultySettings(EMingAIDifficulty Difficulty) const;

    // ?≤Â? AI Áµ±Ë?‰ø°ÊÅØ
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    FMingAIStatistics GetAIStatistics() const;

    // ?´Â? AI ?¥Êñ∞
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void PauseAI();

    // ?¢Âæ© AI ?¥Êñ∞
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ResumeAI();

    // ?çÁΩÆ?Ä??AI
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ResetAllAI();

protected:
    // AI ?ßÂà∂?®Â?Ë°?    TArray<TWeakObjectPtr<class AController>> AIControllers;

    // AI ?¥Êñ∞?ìÈ?
    float AIUpdateInterval;

    // ?∂Â???∫¶Ë®≠ÁΩÆ
    EMingAIDifficulty CurrentDifficulty;

    // ?ØÂê¶?´Â?
    bool bIsPaused;

    // Áµ±Ë??∏Ê?
    FMingAIStatistics AIStatistics;

    // ??∫¶Ë®≠ÁΩÆ?†Â?
    TMap<EMingAIDifficulty, FMingAIDifficultySettings> DifficultySettings;

    // ?ùÂ??ñÈõ£Â∫¶Ë®≠ÁΩ?    void InitializeDifficultySettings();

    // ?¥Êñ∞Áµ±Ë?‰ø°ÊÅØ
    void UpdateStatistics();

    // ?≤Â? AI ?¥Êñ∞?ÇÈ?
    float GetAIUpdateTime() const;

private:
    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    bool bInitialized = false;

    // Á¥ØË??ÇÈ?
    float AccumulatedTime;
};
