// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Service Governance - Comprehensive Service Management
// Provides advanced service governance with policy enforcement, monitoring, and compliance

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Containers/Map.h"
#include "Containers/Array.h"
#include "MingRTSServiceGovernance.generated.h"

UENUM(BlueprintType)
enum class EGovernancePolicyType : uint8 {
    Security         UMETA(DisplayName = "Security"),
    Performance      UMETA(DisplayName = "Performance"),
    Compliance       UMETA(DisplayName = "Compliance"),
    Operational      UMETA(DisplayName = "Operational"),
    Resource         UMETA(DisplayName = "Resource"),
    Network          UMETA(DisplayName = "Network"),
    Data             UMETA(DisplayName = "Data"),
    Custom           UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EPolicyEnforcementLevel : uint8 {
    None            UMETA(DisplayName = "None"),
    Log             UMETA(DisplayName = "Log Only"),
    Warn            UMETA(DisplayName = "Warning"),
    Block           UMETA(DisplayName = "Block"),
    Quarantine      UMETA(DisplayName = "Quarantine"),
    Terminate       UMETA(DisplayName = "Terminate")
};

UENUM(BlueprintType)
enum class EServiceComplianceStatus : uint8 {
    Compliant       UMETA(DisplayName = "Compliant"),
    NonCompliant    UMETA(DisplayName = "Non-Compliant"),
    Pending         UMETA(DisplayName = "Pending"),
    Exempt          UMETA(DisplayName = "Exempt"),
    Unknown         UMETA(DisplayName = "Unknown")
};

UENUM(BlueprintType)
enum class EServiceRiskLevel : uint8 {
    Low             UMETA(DisplayName = "Low"),
    Medium          UMETA(DisplayName = "Medium"),
    High            UMETA(DisplayName = "High"),
    Critical        UMETA(DisplayName = "Critical")
};

USTRUCT(BlueprintType)
struct FGovernancePolicy
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Governance Policy")
    FString PolicyID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Governance Policy")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Governance Policy")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Governance Policy")
    EGovernancePolicyType PolicyType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Governance Policy")
    EPolicyEnforcementLevel EnforcementLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Governance Policy")
    TArray<FString> TargetServices;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Governance Policy")
    TArray<FString> Rules;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Governance Policy")
    TMap<FString, FString> Parameters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Governance Policy")
    bool bEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Governance Policy")
    FDateTime CreatedAt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Governance Policy")
    FDateTime LastUpdated;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Governance Policy")
    int32 ViolationCount;

    FGovernancePolicy()
        : PolicyType(EGovernancePolicyType::Security)
        , EnforcementLevel(EPolicyEnforcementLevel::Warn)
        , bEnabled(true)
        , ViolationCount(0)
    {}
};

USTRUCT(BlueprintType)
struct FServiceComplianceReport
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Compliance Report")
    FString ServiceID;

    UPROPERTY(BlueprintReadOnly, Category = "Compliance Report")
    FString ServiceName;

    UPROPERTY(BlueprintReadOnly, Category = "Compliance Report")
    EServiceComplianceStatus OverallStatus;

    UPROPERTY(BlueprintReadOnly, Category = "Compliance Report")
    TMap<FString, EServiceComplianceStatus> PolicyCompliance;

    UPROPERTY(BlueprintReadOnly, Category = "Compliance Report")
    TArray<FString> Violations;

    UPROPERTY(BlueprintReadOnly, Category = "Compliance Report")
    TArray<FString> Recommendations;

    UPROPERTY(BlueprintReadOnly, Category = "Compliance Report")
    float ComplianceScore;

    UPROPERTY(BlueprintReadOnly, Category = "Compliance Report")
    FDateTime LastAssessed;

    FServiceComplianceReport()
        : OverallStatus(EServiceComplianceStatus::Unknown)
        , ComplianceScore(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FServiceRiskAssessment
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Risk Assessment")
    FString ServiceID;

    UPROPERTY(BlueprintReadOnly, Category = "Risk Assessment")
    EServiceRiskLevel RiskLevel;

    UPROPERTY(BlueprintReadOnly, Category = "Risk Assessment")
    TMap<FString, float> RiskFactors;

    UPROPERTY(BlueprintReadOnly, Category = "Risk Assessment")
    TArray<FString> RiskMitigations;

    UPROPERTY(BlueprintReadOnly, Category = "Risk Assessment")
    TArray<FString> Vulnerabilities;

    UPROPERTY(BlueprintReadOnly, Category = "Risk Assessment")
    float RiskScore;

    UPROPERTY(BlueprintReadOnly, Category = "Risk Assessment")
    FDateTime LastAssessed;

    FServiceRiskAssessment()
        : RiskLevel(EServiceRiskLevel::Medium)
        , RiskScore(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FGovernanceMetrics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Governance Metrics")
    int32 TotalPolicies;

    UPROPERTY(BlueprintReadOnly, Category = "Governance Metrics")
    int32 ActivePolicies;

    UPROPERTY(BlueprintReadOnly, Category = "Governance Metrics")
    int32 TotalViolations;

    UPROPERTY(BlueprintReadOnly, Category = "Governance Metrics")
    int32 ServicesCompliant;

    UPROPERTY(BlueprintReadOnly, Category = "Governance Metrics")
    int32 ServicesNonCompliant;

    UPROPERTY(BlueprintReadOnly, Category = "Governance Metrics")
    float AverageComplianceScore;

    UPROPERTY(BlueprintReadOnly, Category = "Governance Metrics")
    TMap<EServiceRiskLevel, int32> ServicesByRiskLevel;

    UPROPERTY(BlueprintReadOnly, Category = "Governance Metrics")
    TMap<EGovernancePolicyType, int32> ViolationsByPolicyType;

    FGovernanceMetrics()
        : TotalPolicies(0)
        , ActivePolicies(0)
        , TotalViolations(0)
        , ServicesCompliant(0)
        , ServicesNonCompliant(0)
        , AverageComplianceScore(0.0f)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPolicyViolated, const FString&, PolicyID, const FString&, ServiceID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnComplianceStatusChanged, const FString&, ServiceID, EServiceComplianceStatus, NewStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRiskLevelChanged, const FString&, ServiceID, EServiceRiskLevel, NewRiskLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGovernanceMetricsUpdated, const FGovernanceMetrics&, Metrics);

/**
 * Service Governance Manager
 * Provides comprehensive service governance with policy enforcement and compliance monitoring
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSServiceGovernance : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSServiceGovernance();

    // Policy Management
    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    FString CreateGovernancePolicy(const FString& Name, EGovernancePolicyType PolicyType, EPolicyEnforcementLevel EnforcementLevel);

    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    bool RegisterPolicy(const FGovernancePolicy& Policy);

    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    bool UpdatePolicy(const FString& PolicyID, const FGovernancePolicy& UpdatedPolicy);

    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    bool RemovePolicy(const FString& PolicyID);

    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    bool EnablePolicy(const FString& PolicyID);

    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    bool DisablePolicy(const FString& PolicyID);

    UFUNCTION(BlueprintPure, Category = "Service Governance")
    FGovernancePolicy GetPolicy(const FString& PolicyID) const;

    UFUNCTION(BlueprintPure, Category = "Service Governance")
    TArray<FGovernancePolicy> GetAllPolicies() const;

    UFUNCTION(BlueprintPure, Category = "Service Governance")
    TArray<FGovernancePolicy> GetPoliciesByType(EGovernancePolicyType PolicyType) const;

    UFUNCTION(BlueprintPure, Category = "Service Governance")
    TArray<FGovernancePolicy> GetPoliciesForService(const FString& ServiceID) const;

    // Policy Enforcement
    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    bool EvaluateServiceCompliance(const FString& ServiceID);

    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    bool EnforcePolicy(const FString& PolicyID, const FString& ServiceID);

    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    bool CheckPolicyViolation(const FString& PolicyID, const FString& ServiceID, const TMap<FString, FString>& Context);

    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    void HandlePolicyViolation(const FString& PolicyID, const FString& ServiceID, const FString& ViolationDetails);

    // Compliance Management
    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    FServiceComplianceReport GenerateComplianceReport(const FString& ServiceID);

    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    TArray<FServiceComplianceReport> GetAllComplianceReports() const;

    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    bool SetServiceComplianceStatus(const FString& ServiceID, EServiceComplianceStatus Status);

    UFUNCTION(BlueprintPure, Category = "Service Governance")
    EServiceComplianceStatus GetServiceComplianceStatus(const FString& ServiceID) const;

    UFUNCTION(BlueprintPure, Category = "Service Governance")
    float GetServiceComplianceScore(const FString& ServiceID) const;

    // Risk Assessment
    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    FServiceRiskAssessment AssessServiceRisk(const FString& ServiceID);

    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    TArray<FServiceRiskAssessment> GetAllRiskAssessments() const;

    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    bool UpdateServiceRiskLevel(const FString& ServiceID, EServiceRiskLevel RiskLevel);

    UFUNCTION(BlueprintPure, Category = "Service Governance")
    EServiceRiskLevel GetServiceRiskLevel(const FString& ServiceID) const;

    UFUNCTION(BlueprintPure, Category = "Service Governance")
    TArray<FString> GetHighRiskServices() const;

    // Governance Actions
    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    bool QuarantineService(const FString& ServiceID, const FString& Reason);

    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    bool ReleaseServiceFromQuarantine(const FString& ServiceID);

    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    bool TerminateService(const FString& ServiceID, const FString& Reason);

    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    bool ApplyServiceRemediation(const FString& ServiceID, const TArray<FString>& Actions);

    // Monitoring and Analytics
    UFUNCTION(BlueprintPure, Category = "Service Governance")
    FGovernanceMetrics GetGovernanceMetrics() const;

    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    void UpdateGovernanceMetrics();

    UFUNCTION(BlueprintPure, Category = "Service Governance")
    TArray<FString> GetPolicyViolations(const FString& ServiceID) const;

    UFUNCTION(BlueprintPure, Category = "Service Governance")
    TArray<FString> GetServicesWithViolations() const;

    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    void GenerateComplianceReport();

    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    void ScheduleComplianceAudit(const FString& ServiceID, const FDateTime& AuditTime);

    // Configuration
    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    void SetPolicyEnforcementMode(EPolicyEnforcementLevel DefaultLevel);

    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    void EnableAutoRemediation(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Service Governance")
    void SetComplianceThreshold(float Threshold);

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Service Governance Events")
    FOnPolicyViolated OnPolicyViolated;

    UPROPERTY(BlueprintAssignable, Category = "Service Governance Events")
    FOnComplianceStatusChanged OnComplianceStatusChanged;

    UPROPERTY(BlueprintAssignable, Category = "Service Governance Events")
    FOnRiskLevelChanged OnRiskLevelChanged;

    UPROPERTY(BlueprintAssignable, Category = "Service Governance Events")
    FOnGovernanceMetricsUpdated OnGovernanceMetricsUpdated;

protected:
    // Policy Storage
    UPROPERTY()
    TMap<FString, FGovernancePolicy> GovernancePolicies;

    // Compliance Storage
    UPROPERTY()
    TMap<FString, FServiceComplianceReport> ComplianceReports;

    // Risk Assessment Storage
    UPROPERTY()
    TMap<FString, FServiceRiskAssessment> RiskAssessments;

    // Governance State
    UPROPERTY()
    TMap<FString, EServiceComplianceStatus> ServiceComplianceStatus;

    UPROPERTY()
    TMap<FString, EServiceRiskLevel> ServiceRiskLevels;

    UPROPERTY()
    TSet<FString> QuarantinedServices;

    // Configuration
    UPROPERTY()
    EPolicyEnforcementLevel DefaultEnforcementLevel;

    UPROPERTY()
    bool bAutoRemediationEnabled;

    UPROPERTY()
    float ComplianceThreshold;

    // Metrics
    UPROPERTY()
    FGovernanceMetrics Metrics;

    // Internal Methods
    void InitializeGovernance();
    FString GeneratePolicyID() const;
    bool EvaluatePolicyRule(const FString& Rule, const TMap<FString, FString>& Context) const;
    float CalculateComplianceScore(const FString& ServiceID) const;
    float CalculateRiskScore(const FString& ServiceID) const;
    void UpdateMetrics();
    void AutoRemediateService(const FString& ServiceID);
    bool IsServiceQuarantined(const FString& ServiceID) const;
    TArray<FString> GetApplicablePolicies(const FString& ServiceID) const;
    void LogPolicyViolation(const FString& PolicyID, const FString& ServiceID, const FString& Details);
};
