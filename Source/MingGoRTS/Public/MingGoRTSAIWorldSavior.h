#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "MingGoRTSAIWorldSavior.generated.h"

UENUM(BlueprintType)
enum class EWorldThreatLevel : uint8
{
    None,              // ?°Â???    Low,               // ‰ΩéÂ???    Medium,            // ‰∏≠Á?Â®ÅË?
    High,              // È´òÂ???    Critical,          // ?±ÊÄ•Â???    Catastrophic       // ?ΩÈõ£?ßÂ???};

UENUM(BlueprintType)
enum class ESaviorStrategy : uint8
{
    Defensive,         // ?≤Á¶¶Á≠ñÁï•
    Offensive,         // ?ªÊ?Á≠ñÁï•
    Diplomatic,        // Â§ñ‰∫§Á≠ñÁï•
    Scientific,        // ÁßëÁ?Á≠ñÁï•
    Humanitarian,      // ‰∫∫È?‰∏ªÁæ©Á≠ñÁï•
    Economic,          // Á∂ìÊ?Á≠ñÁï•
    Environmental,     // ?∞Â?Á≠ñÁï•
    Technological      // ?ÄË°ìÁ???};

UENUM(BlueprintType)
enum class EResourcePriority : uint8
{
    Critical,          // ?úÈçµË≥áÊ?
    High,              // È´òÂÑ™?àÁ?
    Medium,            // ‰∏≠Á??™Â?Á¥?    Low,               // ‰ΩéÂÑ™?àÁ?
    Optional           // ?ØÈÅ∏Ë≥áÊ?
};

USTRUCT(BlueprintType)
struct FWorldThreatData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat")
    FString ThreatName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat")
    EWorldThreatLevel ThreatLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat")
    FString ThreatDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat")
    FVector2D ThreatLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat")
    float ThreatRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat")
    float TimeToImpact;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat")
    TArray<FString> AffectedRegions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat")
    ESaviorStrategy RecommendedStrategy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat")
    float Confidence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Threat")
    TArray<FString> RequiredResources;
};

USTRUCT(BlueprintType)
struct FSaviorActionPlan
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Plan")
    FString PlanName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Plan")
    ESaviorStrategy Strategy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Plan")
    TArray<FString> ActionSteps;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Plan")
    float EstimatedSuccessRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Plan")
    float ExecutionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Plan")
    TArray<FString> RequiredUnits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Plan")
    TArray<FString> RequiredResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Plan")
    TArray<FString> PotentialRisks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Plan")
    TArray<FString> MitigationStrategies;
};

USTRUCT(BlueprintType)
struct FResourceOptimization
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    FString ResourceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    float CurrentAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    float RequiredAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    EResourcePriority Priority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    FString AllocationStrategy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    float Efficiency;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    TArray<FString> SourceLocations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    TArray<FString> DistributionTargets;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWorldThreatDetected, const FWorldThreatData&, ThreatData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaviorPlanGenerated, const FSaviorActionPlan&, ActionPlan);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourceOptimized, const FResourceOptimization&, ResourceData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWorldStatusChanged, EWorldThreatLevel, NewLevel, const FString&, StatusMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaviorMissionCompleted, bool, bSuccess);

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingGoRTSAIWorldSavior : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingGoRTSAIWorldSavior();

    // ‰∏ñÁ?Â®ÅË???éß
    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void StartWorldMonitoring();

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void StopWorldMonitoring();

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void ScanForThreats();

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    TArray<FWorldThreatData> GetDetectedThreats() const { return DetectedThreats; }

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    EWorldThreatLevel GetCurrentWorldThreatLevel() const { return CurrentThreatLevel; }

    // AIÁ≠ñÁï•?üÊ?
    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void GenerateSaviorStrategy(const FWorldThreatData& Threat);

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void GenerateActionPlan(const FWorldThreatData& Threat, ESaviorStrategy Strategy);

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    TArray<FSaviorActionPlan> GetAvailablePlans() const { return AvailablePlans; }

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void ExecuteActionPlan(const FSaviorActionPlan& Plan);

    // Ë≥áÊ??™Â?ÁÆ°Á?
    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void OptimizeResources();

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void AllocateResources(const FString& ResourceType, float Amount, EResourcePriority Priority);

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    TArray<FResourceOptimization> GetResourceStatus() const { return ResourceStatus; }

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void RedistributeResources(const TArray<FString>& CriticalNeeds);

    // ?êÊ∏¨?åÂ???    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void PredictFutureThreats(float TimeHorizon);

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void AnalyzeThreatPatterns();

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    TArray<FWorldThreatData> GetPredictedThreats() const { return PredictedThreats; }

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    float CalculateWorldStability() const;

    // Ê±∫Á?ËºîÂä©Á≥ªÁµ±
    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void RequestDecisionSupport(const FWorldThreatData& Threat);

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void GenerateRecommendations();

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    TArray<FString> GetRecommendations() const { return Recommendations; }

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void EvaluatePlanEffectiveness(const FSaviorActionPlan& Plan);

    // ?∞Áï•Ë¶èÂ?
    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void CreateLongTermStrategy();

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void UpdateStrategicGoals();

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    TArray<FString> GetStrategicGoals() const { return StrategicGoals; }

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void MonitorStrategicProgress();

    // Á∑äÊÄ•Èüø??    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void ActivateEmergencyProtocol();

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void DeployEmergencyResources();

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void CoordinateEmergencyResponse();

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    bool IsEmergencyActive() const { return bEmergencyActive; }

    // AIÂ≠∏Á??åÈÅ©??    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void LearnFromMissionResults(bool bSuccess, const FString& MissionData);

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void AdaptStrategies();

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void UpdateAIKnowledge();

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    float GetAIEffectivenessRating() const { return AIEffectiveness; }

    // ?®Â??ßÂà∂
    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void ActivateWorldSaviorMode();

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void DeactivateWorldSaviorMode();

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    bool IsWorldSaviorActive() const { return bWorldSaviorActive; }

    UFUNCTION(BlueprintCallable, Category = "AI World Savior")
    void ResetWorldSaviorSystem();

    // ÂßîÊ?‰∫ã‰ª∂
    UPROPERTY(BlueprintAssignable, Category = "AI World Savior")
    FOnWorldThreatDetected OnWorldThreatDetected;

    UPROPERTY(BlueprintAssignable, Category = "AI World Savior")
    FOnSaviorPlanGenerated OnSaviorPlanGenerated;

    UPROPERTY(BlueprintAssignable, Category = "AI World Savior")
    FOnResourceOptimized OnResourceOptimized;

    UPROPERTY(BlueprintAssignable, Category = "AI World Savior")
    FOnWorldStatusChanged OnWorldStatusChanged;

    UPROPERTY(BlueprintAssignable, Category = "AI World Savior")
    FOnSaviorMissionCompleted OnSaviorMissionCompleted;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY()
    bool bWorldSaviorActive;

    UPROPERTY()
    bool bEmergencyActive;

    UPROPERTY()
    bool bMonitoringActive;

    UPROPERTY()
    EWorldThreatLevel CurrentThreatLevel;

    UPROPERTY()
    TArray<FWorldThreatData> DetectedThreats;

    UPROPERTY()
    TArray<FWorldThreatData> PredictedThreats;

    UPROPERTY()
    TArray<FSaviorActionPlan> AvailablePlans;

    UPROPERTY()
    TArray<FResourceOptimization> ResourceStatus;

    UPROPERTY()
    TArray<FString> Recommendations;

    UPROPERTY()
    TArray<FString> StrategicGoals;

    UPROPERTY()
    float AIEffectiveness;

    UPROPERTY()
    float MonitoringInterval;

    UPROPERTY()
    float LastMonitoringTime;

    UPROPERTY()
    FTickerDelegate MonitoringTicker;

    UPROPERTY()
    FDelegateHandle MonitoringTickerHandle;

    void ProcessWorldThreats();
    void UpdateWorldStatus();
    void GenerateEmergencyPlans();
    void OptimizeResourceAllocation();
    void AnalyzeGlobalSituation();
    void CoordinateGlobalResponse();
    void UpdateAIParameters();
    bool OnMonitoringTick(float DeltaTime);
    void NotifyWorldThreat(const FWorldThreatData& Threat);
    void NotifyPlanGenerated(const FSaviorActionPlan& Plan);
    void NotifyResourceOptimized(const FResourceOptimization& Resource);
    void NotifyWorldStatusChange(EWorldThreatLevel NewLevel, const FString& Message);
    void NotifyMissionCompleted(bool bSuccess);
};
