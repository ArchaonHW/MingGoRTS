#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSTutorialSystem.generated.h"

class UUserWidget;

/**
 * ?ôÂ≠∏?éÊÆµÈ°ûÂ?
 */
UENUM(BlueprintType)
enum class ETutorialStage : uint8
{
    None                UMETA(DisplayName = "??),
    BasicControls       UMETA(DisplayName = "?∫Á??ç‰?"),
    ResourceManagement  UMETA(DisplayName = "Ë≥áÊ?ÁÆ°Á?"),
    CombatBasics        UMETA(DisplayName = "?∞È¨•?∫Á?"),
    AdvancedTactics     UMETA(DisplayName = "?≤È??∞Ë?"),
    FactionSpecific     UMETA(DisplayName = "?¢Â??πËâ≤"),
    Completed           UMETA(DisplayName = "Â∑≤Â???)
};

/**
 * ?ôÂ≠∏Ê≠•È??∏Ê?
 */
USTRUCT(BlueprintType)
struct FTutorialStep
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName StepID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Title;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TargetActorTag;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bRequiresAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AutoAdvanceDelay;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> HighlightUIElements;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bPauseGame;
    
    FTutorialStep()
        : bRequiresAction(true)
        , AutoAdvanceDelay(0.0f)
        , bPauseGame(true)
    {}
};

/**
 * ?ôÂ≠∏?éÊÆµ?çÁΩÆ
 */
USTRUCT(BlueprintType)
struct FTutorialStageConfig
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETutorialStage Stage;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText StageName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText StageDescription;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FTutorialStep> Steps;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EstimatedDurationMinutes;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString UnlockCondition;
    
    FTutorialStageConfig()
        : Stage(ETutorialStage::None)
        , EstimatedDurationMinutes(10.0f)
    {}
};

/**
 * ?ôÂ≠∏Á≥ªÁµ± - ÁÆ°Á??Ä?âÊ?Â≠∏ÂÖßÂÆπÂ?ÊµÅÁ?
 */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSTutorialSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSTutorialSystem();
    
    // ?ùÂ???    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void InitializeTutorialSystem();
    
    // ?ãÂ??πÂ??éÊÆµ?ôÂ≠∏
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void StartTutorialStage(ETutorialStage Stage);
    
    // ÁµêÊ??∂Â??ôÂ≠∏
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void EndCurrentTutorial();
    
    // ?∂Â?Ê≠•È?ÂÆåÊ?
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void CompleteCurrentStep();
    
    // Ë∑≥È??ôÂ≠∏
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void SkipTutorial();
    
    // Ê™¢Êü•?ôÂ≠∏?ØÂê¶ÂÆåÊ?
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    bool IsStageCompleted(ETutorialStage Stage) const;
    
    // ?≤Â?‰∏ã‰??ãÊú™ÂÆåÊ??éÊÆµ
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    ETutorialStage GetNextUncompletedStage() const;
    
    // ?≤Â??Ä?âÂ∑≤ÂÆåÊ??éÊÆµ
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    TArray<ETutorialStage> GetCompletedStages() const;
    
    // ?çÁΩÆ?ôÂ≠∏?≤Â∫¶
    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void ResetTutorialProgress();
    
    // Ë®≠ÁΩÆ?ôÂ≠∏?üÁî®?Ä??    UFUNCTION(BlueprintCallable, Category = "Tutorial")
    void SetTutorialEnabled(bool bEnabled);
    
    // ‰øùÂ?/ËºâÂÖ•?≤Â∫¶
    void SaveTutorialProgress();
    void LoadTutorialProgress();
    
    // ‰∫ã‰ª∂ÂßîË?
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTutorialStageStarted, ETutorialStage, Stage, const FText&, StageName);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTutorialStageCompleted, ETutorialStage, Stage);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTutorialStepChanged, int32, StepIndex, const FText&, StepTitle);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTutorialCompleted);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTutorialSkipped);
    
    UPROPERTY(BlueprintAssignable, Category = "Tutorial|Events")
    FOnTutorialStageStarted OnTutorialStageStarted;
    
    UPROPERTY(BlueprintAssignable, Category = "Tutorial|Events")
    FOnTutorialStageCompleted OnTutorialStageCompleted;
    
    UPROPERTY(BlueprintAssignable, Category = "Tutorial|Events")
    FOnTutorialStepChanged OnTutorialStepChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Tutorial|Events")
    FOnTutorialCompleted OnTutorialCompleted;
    
    UPROPERTY(BlueprintAssignable, Category = "Tutorial|Events")
    FOnTutorialSkipped OnTutorialSkipped;
    
private:
    UPROPERTY()
    TArray<FTutorialStageConfig> StageConfigs;
    
    UPROPERTY()
    ETutorialStage CurrentStage;
    
    UPROPERTY()
    int32 CurrentStepIndex;
    
    UPROPERTY()
    TArray<ETutorialStage> CompletedStages;
    
    UPROPERTY()
    bool bTutorialEnabled;
    
    UPROPERTY()
    UUserWidget* CurrentTutorialWidget;
    
    void LoadStageConfigs();
    void ShowCurrentStep();
    void AdvanceToNextStep();
    void OnStepActionCompleted(FName StepID);
    void HighlightGameElement(const FString& ElementTag);
    void ClearHighlight();
    
    FTimerHandle AutoAdvanceTimerHandle;
};
