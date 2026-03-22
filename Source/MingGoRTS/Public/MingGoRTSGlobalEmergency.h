#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/ActorComponent.h"
#include "MingGoRTSGlobalEmergency.generated.h"

UENUM(BlueprintType)
enum class EEmergencyType: uint8 {
    NaturalDisaster,    // ??�M??�`
    Pandemic,          // ???X?�o
    ClimateCrisis,     // 摧毀�M??
    ResourceShortage,  // ��?X?��
    Technological,     // ??�N�M??
    Economic,          // �g?X???
    Political,         // ??�v摧毀
    Military,          // �x?X???
    Environmental,     // 目標數量
    Social             // ��?X???
};

UENUM(BlueprintType)
enum class EEmergencyResponse: uint8 {
    Local,             // ??�a摧毀
    Regional,          // 摧毀�TX
    National,          // ??�a摧毀
    International,     // 目標數量
    Global             // 目標數量
};

USTRUCT(BlueprintType)
struct FEmergencyEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emergency")
    FString EventName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emergency")
    EEmergencyType EmergencyType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emergency")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emergency")
    FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emergency")
    float Severity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emergency")
    float AffectedPopulation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emergency")
    float EstimatedDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emergency")
    EEmergencyResponse RequiredResponse;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emergency")
    TArray<FString> ImmediateNeeds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emergency")
    TArray<FString> LongTermNeeds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emergency")
    bool bOngoing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emergency")
    float TimeRemaining;
};

USTRUCT(BlueprintType)
struct FEmergencyResponsePlan
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Response Plan")
    FString PlanName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Response Plan")
    EEmergencyType TargetEmergency;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Response Plan")
    EEmergencyResponse ResponseLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Response Plan")
    TArray<FString> ResponseActions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Response Plan")
    TArray<FString> RequiredResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Response Plan")
    TArray<FString> CoordinatingAgencies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Response Plan")
    float EstimatedCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Response Plan")
    float EstimatedSuccessRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Response Plan")
    float DeploymentTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Response Plan")
    bool bActive;
};






UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINGRTS_API UMingGoRTSGlobalEmergency : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingGoRTSGlobalEmergency();

    // ���??���X
    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void DetectEmergency(const FEmergencyEvent& EmergencyEvent);

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void ActivateEmergencyResponse(const FEmergencyEvent& Emergency);

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void MonitorActiveEmergencies();

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    TArray<FEmergencyEvent> GetActiveEmergencies() const { return ActiveEmergencies; }

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void ResolveEmergency(const FString& EventName);

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    bool IsEmergencyActive(const FString& EventName) const;

    // 故事重要性?��
    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void CoordinateGlobalResponse(const FEmergencyEvent& Emergency);

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void DeployInternationalAid(const FString& EventName);

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void EstablishEmergencyProtocols();

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void ActivateGlobalAlertSystem();

    // ��?X???
    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void AllocateEmergencyResources(const FString& EventName, const TArray<FString>& Resources);

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void RequestInternationalAssistance(const FString& EventName);

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void ManageResourceDistribution();

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    TArray<FString> GetAvailableEmergencyResources() const;

    // ??ĵ�t��
    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void IssueEmergencyWarning(const FEmergencyEvent& Emergency);

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void BroadcastGlobalAlert(const FString& AlertMessage);

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void UpdateAlertLevels();

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void EvacuateAffectedAreas(const FString& EventName);

    // ??����??
    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void LaunchRescueOperations(const FString& EventName);

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void DeployMedicalTeams(const FString& EventName);

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void EstablishReliefCamps(const FString& EventName);

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void CoordinateSearchAndRescue(const FString& EventName);

    // ??�_摧毀??
    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void InitiateRecoveryPhase(const FString& EventName);

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void CoordinateReconstructionEfforts(const FString& EventName);

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void ProvideLongTermSupport(const FString& EventName);

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void MonitorRecoveryProgress(const FString& EventName);

    // ??��???X
    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void StrengthenPreparednessSystems();

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void ConductEmergencyDrills();

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void UpdateEmergencyProtocols();

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void TrainEmergencyResponders();

    // 故事重要性?��??
    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void AnalyzeEmergencyPatterns();

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void GenerateEmergencyReport();

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void UpdateResponseStrategies();

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void ShareLessonsLearned();

    // �t��??��
    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void ActivateGlobalEmergencyMode();

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void DeactivateGlobalEmergencyMode();

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    bool IsGlobalEmergencyActive() const { return bGlobalEmergencyActive; }

    UFUNCTION(BlueprintCallable, Category = "Global Emergency")
    void ResetEmergencySystem();

    // �e??�ƥ�
    UPROPERTY(BlueprintAssignable, Category = "Global Emergency")
    FOnEmergencyDetected OnEmergencyDetected;

    UPROPERTY(BlueprintAssignable, Category = "Global Emergency")
    FOnEmergencyResponseActivated OnEmergencyResponseActivated;

    UPROPERTY(BlueprintAssignable, Category = "Global Emergency")
    FOnEmergencyStatusUpdated OnEmergencyStatusUpdated;

    UPROPERTY(BlueprintAssignable, Category = "Global Emergency")
    FOnGlobalEmergencyAlert OnGlobalEmergencyAlert;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY()
    bool bGlobalEmergencyActive;

    UPROPERTY()
    TArray<FEmergencyEvent> ActiveEmergencies;

    UPROPERTY()
    TArray<FEmergencyResponsePlan> ResponsePlans;

    UPROPERTY()
    TArray<FString> AvailableResources;

    UPROPERTY()
    TArray<FString> InternationalPartners;

    UPROPERTY()
    float GlobalThreatLevel;

    UPROPERTY()
    float LastUpdateTime;

    UPROPERTY()
    FTickerDelegate EmergencyTicker;

    UPROPERTY()
    FDelegateHandle EmergencyTickerHandle;

    void ProcessEmergencies();
    void UpdateGlobalStatus();
    void CoordinateResponseEfforts();
    void MonitorResourceLevels();
    void UpdateEmergencyAlerts();
    bool OnEmergencyTick(float DeltaTime);
    void NotifyEmergencyDetected(const FEmergencyEvent& Emergency);
    void NotifyResponseActivated(const FEmergencyResponsePlan& Plan);
    void NotifyStatusUpdated(const FString& EventName, bool bResolved);
    void NotifyGlobalAlert(const FString& AlertMessage);
    FEmergencyResponsePlan GenerateResponsePlan(const FEmergencyEvent& Emergency);
    void ExecuteResponsePlan(const FEmergencyResponsePlan& Plan);
    void UpdateEmergencyProgress(const FString& EventName, float DeltaTime);
);

