// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Service Governance - Comprehensive Service Management Implementation

#include "Process/MingRTSServiceGovernance.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"
#include "Misc/Guid.h"

UMingRTSServiceGovernance::UMingRTSServiceGovernance()
{
    InitializeGovernance();
}

void UMingRTSServiceGovernance::InitializeGovernance()
{
    DefaultEnforcementLevel = EPolicyEnforcementLevel::Warn;
    bAutoRemediationEnabled = true;
    ComplianceThreshold = 0.8f;
    
    UE_LOG(LogTemp, Log, TEXT("Service Governance initialized"));
}

FString UMingRTSServiceGovernance::CreateGovernancePolicy(const FString& Name, EGovernancePolicyType PolicyType, EPolicyEnforcementLevel EnforcementLevel)
{
    FGovernancePolicy NewPolicy;
    NewPolicy.PolicyID = GeneratePolicyID();
    NewPolicy.Name = Name;
    NewPolicy.PolicyType = PolicyType;
    NewPolicy.EnforcementLevel = EnforcementLevel;
    NewPolicy.bEnabled = true;
    NewPolicy.CreatedAt = FDateTime::Now();
    NewPolicy.LastUpdated = FDateTime::Now();
    NewPolicy.ViolationCount = 0;

    if (RegisterPolicy(NewPolicy))
    {
        return NewPolicy.PolicyID;
    }

    return FString();
}

bool UMingRTSServiceGovernance::RegisterPolicy(const FGovernancePolicy& Policy)
{
    if (Policy.PolicyID.IsEmpty() || Policy.Name.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid policy data for registration"));
        return false;
    }

    GovernancePolicies.Add(Policy.PolicyID, Policy);
    UpdateMetrics();
    
    UE_LOG(LogTemp, Log, TEXT("Registered governance policy: %s (%s)"), *Policy.PolicyID, *Policy.Name);
    return true;
}

bool UMingRTSServiceGovernance::UpdatePolicy(const FString& PolicyID, const FGovernancePolicy& UpdatedPolicy)
{
    if (FGovernancePolicy* ExistingPolicy = GovernancePolicies.Find(PolicyID))
    {
        *ExistingPolicy = UpdatedPolicy;
        ExistingPolicy->PolicyID = PolicyID; // Preserve original ID
        ExistingPolicy->LastUpdated = FDateTime::Now();
        
        UE_LOG(LogTemp, Log, TEXT("Updated governance policy: %s"), *PolicyID);
        return true;
    }
    
    return false;
}

bool UMingRTSServiceGovernance::RemovePolicy(const FString& PolicyID)
{
    if (GovernancePolicies.Remove(PolicyID) > 0)
    {
        UpdateMetrics();
        UE_LOG(LogTemp, Log, TEXT("Removed governance policy: %s"), *PolicyID);
        return true;
    }
    
    return false;
}

bool UMingRTSServiceGovernance::EnablePolicy(const FString& PolicyID)
{
    if (FGovernancePolicy* Policy = GovernancePolicies.Find(PolicyID))
    {
        Policy->bEnabled = true;
        UE_LOG(LogTemp, Log, TEXT("Enabled governance policy: %s"), *PolicyID);
        return true;
    }
    
    return false;
}

bool UMingRTSServiceGovernance::DisablePolicy(const FString& PolicyID)
{
    if (FGovernancePolicy* Policy = GovernancePolicies.Find(PolicyID))
    {
        Policy->bEnabled = false;
        UE_LOG(LogTemp, Log, TEXT("Disabled governance policy: %s"), *PolicyID);
        return true;
    }
    
    return false;
}

FGovernancePolicy UMingRTSServiceGovernance::GetPolicy(const FString& PolicyID) const
{
    if (const FGovernancePolicy* Policy = GovernancePolicies.Find(PolicyID))
    {
        return *Policy;
    }
    
    return FGovernancePolicy();
}

TArray<FGovernancePolicy> UMingRTSServiceGovernance::GetAllPolicies() const
{
    TArray<FGovernancePolicy> Result;
    GovernancePolicies.GenerateValueArray(Result);
    return Result;
}

TArray<FGovernancePolicy> UMingRTSServiceGovernance::GetPoliciesByType(EGovernancePolicyType PolicyType) const
{
    TArray<FGovernancePolicy> Result;
    
    for (const auto& PolicyPair : GovernancePolicies)
    {
        if (PolicyPair.Value.PolicyType == PolicyType)
        {
            Result.Add(PolicyPair.Value);
        }
    }
    
    return Result;
}

TArray<FGovernancePolicy> UMingRTSServiceGovernance::GetPoliciesForService(const FString& ServiceID) const
{
    TArray<FGovernancePolicy> Result;
    
    for (const auto& PolicyPair : GovernancePolicies)
    {
        const FGovernancePolicy& Policy = PolicyPair.Value;
        if (Policy.bEnabled && (Policy.TargetServices.Num() == 0 || Policy.TargetServices.Contains(ServiceID)))
        {
            Result.Add(Policy);
        }
    }
    
    return Result;
}

bool UMingRTSServiceGovernance::EvaluateServiceCompliance(const FString& ServiceID)
{
    TArray<FGovernancePolicy> ApplicablePolicies = GetPoliciesForService(ServiceID);
    FServiceComplianceReport Report;
    Report.ServiceID = ServiceID;
    Report.LastAssessed = FDateTime::Now();
    
    int32 CompliantPolicies = 0;
    int32 TotalPolicies = ApplicablePolicies.Num();
    
    for (const FGovernancePolicy& Policy : ApplicablePolicies)
    {
        TMap<FString, FString> Context;
        Context.Add(TEXT("ServiceID"), ServiceID);
        Context.Add(TEXT("Timestamp"), FDateTime::Now().ToString());
        
        bool bCompliant = !CheckPolicyViolation(Policy.PolicyID, ServiceID, Context);
        
        EServiceComplianceStatus PolicyStatus = bCompliant ? EServiceComplianceStatus::Compliant : EServiceComplianceStatus::NonCompliant;
        Report.PolicyCompliance.Add(Policy.PolicyID, PolicyStatus);
        
        if (bCompliant)
        {
            CompliantPolicies++;
        }
        else
        {
            Report.Violations.Add(FString::Printf(TEXT("Violation of policy: %s"), *Policy.Name));
        }
    }
    
    // Calculate overall compliance status
    Report.ComplianceScore = TotalPolicies > 0 ? float(CompliantPolicies) / TotalPolicies : 0.0f;
    
    if (Report.ComplianceScore >= ComplianceThreshold)
    {
        Report.OverallStatus = EServiceComplianceStatus::Compliant;
    }
    else if (Report.ComplianceScore > 0.0f)
    {
        Report.OverallStatus = EServiceComplianceStatus::NonCompliant;
    }
    else
    {
        Report.OverallStatus = EServiceComplianceStatus::Unknown;
    }
    
    // Store report
    ComplianceReports.Add(ServiceID, Report);
    ServiceComplianceStatus.Add(ServiceID, Report.OverallStatus);
    
    // Generate recommendations
    if (Report.OverallStatus == EServiceComplianceStatus::NonCompliant)
    {
        Report.Recommendations.Add(TEXT("Review and address policy violations"));
        Report.Recommendations.Add(TEXT("Implement security best practices"));
        Report.Recommendations.Add(TEXT("Update service configuration"));
    }
    
    OnComplianceStatusChanged.Broadcast(ServiceID, Report.OverallStatus);
    UpdateMetrics();
    
    UE_LOG(LogTemp, Log, TEXT("Evaluated compliance for service %s: %.2f%%"), *ServiceID, Report.ComplianceScore * 100.0f);
    return Report.OverallStatus == EServiceComplianceStatus::Compliant;
}

bool UMingRTSServiceGovernance::EnforcePolicy(const FString& PolicyID, const FString& ServiceID)
{
    if (const FGovernancePolicy* Policy = GovernancePolicies.Find(PolicyID))
    {
        if (!Policy->bEnabled)
        {
            return true; // Disabled policies don't need enforcement
        }
        
        TMap<FString, FString> Context;
        Context.Add(TEXT("ServiceID"), ServiceID);
        Context.Add(TEXT("Enforcement"), TEXT("Active"));
        
        bool bViolation = CheckPolicyViolation(PolicyID, ServiceID, Context);
        
        if (bViolation)
        {
            HandlePolicyViolation(PolicyID, ServiceID, TEXT("Policy enforcement detected violation"));
            return false;
        }
        
        return true;
    }
    
    return false;
}

bool UMingRTSServiceGovernance::CheckPolicyViolation(const FString& PolicyID, const FString& ServiceID, const TMap<FString, FString>& Context)
{
    if (const FGovernancePolicy* Policy = GovernancePolicies.Find(PolicyID))
    {
        if (!Policy->bEnabled)
        {
            return false;
        }
        
        // Check each rule in the policy
        for (const FString& Rule : Policy->Rules)
        {
            if (EvaluatePolicyRule(Rule, Context))
            {
                // Rule passed (no violation)
                continue;
            }
            else
            {
                // Rule failed (violation detected)
                LogPolicyViolation(PolicyID, ServiceID, FString::Printf(TEXT("Rule failed: %s"), *Rule));
                return true;
            }
        }
    }
    
    return false;
}

void UMingRTSServiceGovernance::HandlePolicyViolation(const FString& PolicyID, const FString& ServiceID, const FString& ViolationDetails)
{
    // Update violation count
    if (FGovernancePolicy* Policy = GovernancePolicies.Find(PolicyID))
    {
        Policy->ViolationCount++;
    }
    
    // Log violation
    LogPolicyViolation(PolicyID, ServiceID, ViolationDetails);
    
    // Broadcast event
    OnPolicyViolated.Broadcast(PolicyID, ServiceID);
    
    // Handle based on enforcement level
    if (const FGovernancePolicy* Policy = GovernancePolicies.Find(PolicyID))
    {
        switch (Policy->EnforcementLevel)
        {
        case EPolicyEnforcementLevel::None:
            // No action
            break;
            
        case EPolicyEnforcementLevel::Log:
            // Already logged above
            break;
            
        case EPolicyEnforcementLevel::Warn:
            UE_LOG(LogTemp, Warning, TEXT("Policy violation warning: %s for service %s"), *PolicyID, *ServiceID);
            break;
            
        case EPolicyEnforcementLevel::Block:
            UE_LOG(LogTemp, Error, TEXT("Policy violation blocked: %s for service %s"), *PolicyID, *ServiceID);
            // Implement service blocking logic
            break;
            
        case EPolicyEnforcementLevel::Quarantine:
            QuarantineService(ServiceID, FString::Printf(TEXT("Policy violation: %s"), *PolicyID));
            break;
            
        case EPolicyEnforcementLevel::Terminate:
            TerminateService(ServiceID, FString::Printf(TEXT("Policy violation: %s"), *PolicyID));
            break;
        }
        
        // Auto-remediation if enabled
        if (bAutoRemediationEnabled)
        {
            AutoRemediateService(ServiceID);
        }
    }
    
    UpdateMetrics();
}

FServiceComplianceReport UMingRTSServiceGovernance::GenerateComplianceReport(const FString& ServiceID)
{
    if (const FServiceComplianceReport* ExistingReport = ComplianceReports.Find(ServiceID))
    {
        return *ExistingReport;
    }
    
    // Generate new report if none exists
    EvaluateServiceCompliance(ServiceID);
    
    if (const FServiceComplianceReport* NewReport = ComplianceReports.Find(ServiceID))
    {
        return *NewReport;
    }
    
    return FServiceComplianceReport();
}

TArray<FServiceComplianceReport> UMingRTSServiceGovernance::GetAllComplianceReports() const
{
    TArray<FServiceComplianceReport> Result;
    ComplianceReports.GenerateValueArray(Result);
    return Result;
}

bool UMingRTSServiceGovernance::SetServiceComplianceStatus(const FString& ServiceID, EServiceComplianceStatus Status)
{
    EServiceComplianceStatus OldStatus = GetServiceComplianceStatus(ServiceID);
    ServiceComplianceStatus.Add(ServiceID, Status);
    
    if (OldStatus != Status)
    {
        OnComplianceStatusChanged.Broadcast(ServiceID, Status);
        UE_LOG(LogTemp, Log, TEXT("Service compliance status changed: %s %s -> %s"), 
            *ServiceID, *StaticEnum<EServiceComplianceStatus>()->GetValueAsString(OldStatus), *StaticEnum<EServiceComplianceStatus>()->GetValueAsString(Status));
        return true;
    }
    
    return false;
}

EServiceComplianceStatus UMingRTSServiceGovernance::GetServiceComplianceStatus(const FString& ServiceID) const
{
    if (const EServiceComplianceStatus* Status = ServiceComplianceStatus.Find(ServiceID))
    {
        return *Status;
    }
    return EServiceComplianceStatus::Unknown;
}

float UMingRTSServiceGovernance::GetServiceComplianceScore(const FString& ServiceID) const
{
    if (const FServiceComplianceReport* Report = ComplianceReports.Find(ServiceID))
    {
        return Report->ComplianceScore;
    }
    return 0.0f;
}

FServiceRiskAssessment UMingRTSServiceGovernance::AssessServiceRisk(const FString& ServiceID)
{
    FServiceRiskAssessment Assessment;
    Assessment.ServiceID = ServiceID;
    Assessment.LastAssessed = FDateTime::Now();
    
    // Calculate risk factors
    float SecurityRisk = 0.0f;
    float PerformanceRisk = 0.0f;
    float ComplianceRisk = 0.0f;
    float OperationalRisk = 0.0f;
    
    // Security risk based on security policy violations
    TArray<FGovernancePolicy> SecurityPolicies = GetPoliciesByType(EGovernancePolicyType::Security);
    for (const FGovernancePolicy& Policy : SecurityPolicies)
    {
        if (Policy.TargetServices.Num() == 0 || Policy.TargetServices.Contains(ServiceID))
        {
            SecurityRisk += Policy.ViolationCount * 0.1f;
        }
    }
    
    // Performance risk based on performance metrics (placeholder)
    PerformanceRisk = 0.2f; // Would be calculated from actual metrics
    
    // Compliance risk based on compliance score
    float ComplianceScore = GetServiceComplianceScore(ServiceID);
    ComplianceRisk = 1.0f - ComplianceScore;
    
    // Operational risk based on service age and stability (placeholder)
    OperationalRisk = 0.15f;
    
    // Store risk factors
    Assessment.RiskFactors.Add(TEXT("Security"), SecurityRisk);
    Assessment.RiskFactors.Add(TEXT("Performance"), PerformanceRisk);
    Assessment.RiskFactors.Add(TEXT("Compliance"), ComplianceRisk);
    Assessment.RiskFactors.Add(TEXT("Operational"), OperationalRisk);
    
    // Calculate overall risk score
    Assessment.RiskScore = (SecurityRisk + PerformanceRisk + ComplianceRisk + OperationalRisk) / 4.0f;
    
    // Determine risk level
    if (Assessment.RiskScore >= 0.8f)
    {
        Assessment.RiskLevel = EServiceRiskLevel::Critical;
    }
    else if (Assessment.RiskScore >= 0.6f)
    {
        Assessment.RiskLevel = EServiceRiskLevel::High;
    }
    else if (Assessment.RiskScore >= 0.3f)
    {
        Assessment.RiskLevel = EServiceRiskLevel::Medium;
    }
    else
    {
        Assessment.RiskLevel = EServiceRiskLevel::Low;
    }
    
    // Generate vulnerabilities and mitigations
    if (SecurityRisk > 0.5f)
    {
        Assessment.Vulnerabilities.Add(TEXT("Security vulnerabilities detected"));
        Assessment.RiskMitigations.Add(TEXT("Implement security patches"));
        Assessment.RiskMitigations.Add(TEXT("Enable enhanced monitoring"));
    }
    
    if (ComplianceRisk > 0.5f)
    {
        Assessment.Vulnerabilities.Add(TEXT("Compliance issues detected"));
        Assessment.RiskMitigations.Add(TEXT("Address policy violations"));
        Assessment.RiskMitigations.Add(TEXT("Update service configuration"));
    }
    
    // Store assessment
    RiskAssessments.Add(ServiceID, Assessment);
    ServiceRiskLevels.Add(ServiceID, Assessment.RiskLevel);
    
    OnRiskLevelChanged.Broadcast(ServiceID, Assessment.RiskLevel);
    UpdateMetrics();
    
    UE_LOG(LogTemp, Log, TEXT("Risk assessment for service %s: %s (score: %.2f)"), 
        *ServiceID, *StaticEnum<EServiceRiskLevel>()->GetValueAsString(Assessment.RiskLevel), Assessment.RiskScore);
    
    return Assessment;
}

TArray<FServiceRiskAssessment> UMingRTSServiceGovernance::GetAllRiskAssessments() const
{
    TArray<FServiceRiskAssessment> Result;
    RiskAssessments.GenerateValueArray(Result);
    return Result;
}

bool UMingRTSServiceGovernance::UpdateServiceRiskLevel(const FString& ServiceID, EServiceRiskLevel RiskLevel)
{
    EServiceRiskLevel OldLevel = GetServiceRiskLevel(ServiceID);
    ServiceRiskLevels.Add(ServiceID, RiskLevel);
    
    if (OldLevel != RiskLevel)
    {
        OnRiskLevelChanged.Broadcast(ServiceID, RiskLevel);
        UE_LOG(LogTemp, Log, TEXT("Service risk level changed: %s %s -> %s"), 
            *ServiceID, *StaticEnum<EServiceRiskLevel>()->GetValueAsString(OldLevel), *StaticEnum<EServiceRiskLevel>()->GetValueAsString(RiskLevel));
        return true;
    }
    
    return false;
}

EServiceRiskLevel UMingRTSServiceGovernance::GetServiceRiskLevel(const FString& ServiceID) const
{
    if (const EServiceRiskLevel* Level = ServiceRiskLevels.Find(ServiceID))
    {
        return *Level;
    }
    return EServiceRiskLevel::Medium;
}

TArray<FString> UMingRTSServiceGovernance::GetHighRiskServices() const
{
    TArray<FString> HighRiskServices;
    
    for (const auto& RiskPair : ServiceRiskLevels)
    {
        if (RiskPair.Value == EServiceRiskLevel::High || RiskPair.Value == EServiceRiskLevel::Critical)
        {
            HighRiskServices.Add(RiskPair.Key);
        }
    }
    
    return HighRiskServices;
}

bool UMingRTSServiceGovernance::QuarantineService(const FString& ServiceID, const FString& Reason)
{
    QuarantinedServices.Add(ServiceID);
    
    UE_LOG(LogTemp, Warning, TEXT("Service quarantined: %s (Reason: %s)"), *ServiceID, *Reason);
    
    // In a real implementation, this would:
    // - Stop the service
    // - Isolate it from the network
    // - Notify administrators
    // - Create incident ticket
    
    return true;
}

bool UMingRTSServiceGovernance::ReleaseServiceFromQuarantine(const FString& ServiceID)
{
    if (QuarantinedServices.Remove(ServiceID) > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Service released from quarantine: %s"), *ServiceID);
        
        // In a real implementation, this would:
        // - Restart the service
        // - Restore network connectivity
        // - Verify compliance before full restoration
        
        return true;
    }
    
    return false;
}

bool UMingRTSServiceGovernance::TerminateService(const FString& ServiceID, const FString& Reason)
{
    UE_LOG(LogTemp, Error, TEXT("Service terminated: %s (Reason: %s)"), *ServiceID, *Reason);
    
    // In a real implementation, this would:
    // - Force stop the service
    // - Clean up resources
    // - Log incident
    // - Notify stakeholders
    
    // Remove from governance tracking
    ServiceComplianceStatus.Remove(ServiceID);
    ServiceRiskLevels.Remove(ServiceID);
    ComplianceReports.Remove(ServiceID);
    RiskAssessments.Remove(ServiceID);
    QuarantinedServices.Remove(ServiceID);
    
    UpdateMetrics();
    return true;
}

bool UMingRTSServiceGovernance::ApplyServiceRemediation(const FString& ServiceID, const TArray<FString>& Actions)
{
    UE_LOG(LogTemp, Log, TEXT("Applying remediation to service: %s"), *ServiceID);
    
    for (const FString& Action : Actions)
    {
        UE_LOG(LogTemp, Log, TEXT("Remediation action: %s"), *Action);
        
        // In a real implementation, this would:
        // - Execute the remediation action
        // - Verify the action was successful
        // - Update service status
        // - Log the remediation
    }
    
    // Re-evaluate compliance after remediation
    EvaluateServiceCompliance(ServiceID);
    
    return true;
}

FGovernanceMetrics UMingRTSServiceGovernance::GetGovernanceMetrics() const
{
    return Metrics;
}

void UMingRTSServiceGovernance::UpdateGovernanceMetrics()
{
    UpdateMetrics();
    OnGovernanceMetricsUpdated.Broadcast(Metrics);
}

TArray<FString> UMingRTSServiceGovernance::GetPolicyViolations(const FString& ServiceID) const
{
    TArray<FString> Violations;
    
    if (const FServiceComplianceReport* Report = ComplianceReports.Find(ServiceID))
    {
        Violations = Report->Violations;
    }
    
    return Violations;
}

TArray<FString> UMingRTSServiceGovernance::GetServicesWithViolations() const
{
    TArray<FString> ServicesWithViolations;
    
    for (const auto& ReportPair : ComplianceReports)
    {
        if (ReportPair.Value.Violations.Num() > 0)
        {
            ServicesWithViolations.Add(ReportPair.Key);
        }
    }
    
    return ServicesWithViolations;
}

void UMingRTSServiceGovernance::GenerateComplianceReport()
{
    UE_LOG(LogTemp, Log, TEXT("Generating comprehensive compliance report"));
    
    // Generate compliance reports for all tracked services
    TArray<FString> AllServices;
    ServiceComplianceStatus.GetKeys(AllServices);
    
    for (const FString& ServiceID : AllServices)
    {
        GenerateComplianceReport(ServiceID);
    }
    
    UpdateMetrics();
}

void UMingRTSServiceGovernance::ScheduleComplianceAudit(const FString& ServiceID, const FDateTime& AuditTime)
{
    UE_LOG(LogTemp, Log, TEXT("Scheduled compliance audit for service %s at %s"), 
        *ServiceID, *AuditTime.ToString());
    
    // In a real implementation, this would:
    // - Schedule the audit in a task scheduler
    // - Send notifications
    // - Prepare audit checklist
}

void UMingRTSServiceGovernance::SetPolicyEnforcementMode(EPolicyEnforcementLevel DefaultLevel)
{
    DefaultEnforcementLevel = DefaultLevel;
    UE_LOG(LogTemp, Log, TEXT("Set default policy enforcement level to: %s"), 
        *StaticEnum<EPolicyEnforcementLevel>()->GetValueAsString(DefaultLevel));
}

void UMingRTSServiceGovernance::EnableAutoRemediation(bool bEnabled)
{
    bAutoRemediationEnabled = bEnabled;
    UE_LOG(LogTemp, Log, TEXT("Auto-remediation %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

void UMingRTSServiceGovernance::SetComplianceThreshold(float Threshold)
{
    ComplianceThreshold = FMath::Clamp(Threshold, 0.0f, 1.0f);
    UE_LOG(LogTemp, Log, TEXT("Set compliance threshold to: %.2f"), ComplianceThreshold);
}

// Internal Methods
FString UMingRTSServiceGovernance::GeneratePolicyID() const
{
    return FString::Printf(TEXT("policy_%s"), *FGuid::NewGuid().ToString());
}

bool UMingRTSServiceGovernance::EvaluatePolicyRule(const FString& Rule, const TMap<FString, FString>& Context) const
{
    // Simple rule evaluation - in a real implementation, this would be more sophisticated
    // For now, assume all rules pass unless they contain "fail"
    return !Rule.Contains(TEXT("fail"));
}

float UMingRTSServiceGovernance::CalculateComplianceScore(const FString& ServiceID) const
{
    if (const FServiceComplianceReport* Report = ComplianceReports.Find(ServiceID))
    {
        return Report->ComplianceScore;
    }
    return 0.0f;
}

float UMingRTSServiceGovernance::CalculateRiskScore(const FString& ServiceID) const
{
    if (const FServiceRiskAssessment* Assessment = RiskAssessments.Find(ServiceID))
    {
        return Assessment->RiskScore;
    }
    return 0.5f; // Default medium risk
}

void UMingRTSServiceGovernance::UpdateMetrics()
{
    // Count policies
    Metrics.TotalPolicies = GovernancePolicies.Num();
    Metrics.ActivePolicies = 0;
    
    for (const auto& PolicyPair : GovernancePolicies)
    {
        if (PolicyPair.Value.bEnabled)
        {
            Metrics.ActivePolicies++;
        }
    }
    
    // Count violations
    Metrics.TotalViolations = 0;
    for (const auto& PolicyPair : GovernancePolicies)
    {
        Metrics.TotalViolations += PolicyPair.Value.ViolationCount;
    }
    
    // Count compliance status
    Metrics.ServicesCompliant = 0;
    Metrics.ServicesNonCompliant = 0;
    float TotalComplianceScore = 0.0f;
    
    for (const auto& StatusPair : ServiceComplianceStatus)
    {
        if (StatusPair.Value == EServiceComplianceStatus::Compliant)
        {
            Metrics.ServicesCompliant++;
        }
        else if (StatusPair.Value == EServiceComplianceStatus::NonCompliant)
        {
            Metrics.ServicesNonCompliant++;
        }
        
        TotalComplianceScore += GetServiceComplianceScore(StatusPair.Key);
    }
    
    // Calculate average compliance score
    int32 TotalServices = ServiceComplianceStatus.Num();
    Metrics.AverageComplianceScore = TotalServices > 0 ? TotalComplianceScore / TotalServices : 0.0f;
    
    // Count services by risk level
    Metrics.ServicesByRiskLevel.Empty();
    for (const auto& RiskPair : ServiceRiskLevels)
    {
        int32& Count = Metrics.ServicesByRiskLevel.FindOrAdd(RiskPair.Value, 0);
        Count++;
    }
    
    // Count violations by policy type
    Metrics.ViolationsByPolicyType.Empty();
    for (const auto& PolicyPair : GovernancePolicies)
    {
        if (PolicyPair.Value.ViolationCount > 0)
        {
            int32& Count = Metrics.ViolationsByPolicyType.FindOrAdd(PolicyPair.Value.PolicyType, 0);
            Count += PolicyPair.Value.ViolationCount;
        }
    }
}

void UMingRTSServiceGovernance::AutoRemediateService(const FString& ServiceID)
{
    UE_LOG(LogTemp, Log, TEXT("Auto-remediating service: %s"), *ServiceID);
    
    TArray<FString> RemediationActions;
    
    // Get service compliance report
    if (const FServiceComplianceReport* Report = ComplianceReports.Find(ServiceID))
    {
        if (Report->OverallStatus == EServiceComplianceStatus::NonCompliant)
        {
            RemediationActions.Add(TEXT("Restart service"));
            RemediationActions.Add(TEXT("Update configuration"));
            RemediationActions.Add(TEXT("Apply security patches"));
        }
    }
    
    // Get risk assessment
    if (const FServiceRiskAssessment* Assessment = RiskAssessments.Find(ServiceID))
    {
        if (Assessment->RiskLevel >= EServiceRiskLevel::High)
        {
            RemediationActions.Add(TEXT("Enable enhanced monitoring"));
            RemediationActions.Add(TEXT("Increase security scanning"));
        }
    }
    
    if (RemediationActions.Num() > 0)
    {
        ApplyServiceRemediation(ServiceID, RemediationActions);
    }
}

bool UMingRTSServiceGovernance::IsServiceQuarantined(const FString& ServiceID) const
{
    return QuarantinedServices.Contains(ServiceID);
}

TArray<FString> UMingRTSServiceGovernance::GetApplicablePolicies(const FString& ServiceID) const
{
    TArray<FString> ApplicablePolicyIDs;
    
    for (const auto& PolicyPair : GovernancePolicies)
    {
        const FGovernancePolicy& Policy = PolicyPair.Value;
        if (Policy.bEnabled && (Policy.TargetServices.Num() == 0 || Policy.TargetServices.Contains(ServiceID)))
        {
            ApplicablePolicyIDs.Add(Policy.PolicyID);
        }
    }
    
    return ApplicablePolicyIDs;
}

void UMingRTSServiceGovernance::LogPolicyViolation(const FString& PolicyID, const FString& ServiceID, const FString& Details)
{
    UE_LOG(LogTemp, Warning, TEXT("Policy Violation - Policy: %s, Service: %s, Details: %s"), 
        *PolicyID, *ServiceID, *Details);
    
    // In a real implementation, this would also:
    // - Write to audit log
    // - Send alerts
    // - Create incident tickets
    // - Update monitoring dashboards
}
