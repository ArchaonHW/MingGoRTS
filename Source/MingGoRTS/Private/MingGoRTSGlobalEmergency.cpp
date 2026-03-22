#include "MingGoRTSGlobalEmergency.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"

UMingGoRTSGlobalEmergency::UMingGoRTSGlobalEmergency()
    : bGlobalEmergencyActive(false)
    , GlobalThreatLevel(0.0f)
    , LastUpdateTime(0.0f)
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // 初始化緊急事件計時器
    EmergencyTicker = FTickerDelegate::CreateUObject(this, &UMingGoRTSGlobalEmergency::OnEmergencyTick);
}

void UMingGoRTSGlobalEmergency::BeginPlay()
{
    Super::BeginPlay();
    
    UE_LOG(LogTemp, Log, TEXT("Global Emergency Response System Initialized"));
    
    // 初始化可用資源
    AvailableResources = {
        TEXT("Medical Teams"),
        TEXT("Search and Rescue Units"),
        TEXT("Emergency Supplies"),
        TEXT("Transportation Fleet"),
        TEXT("Communication Equipment"),
        TEXT("Temporary Shelters"),
        TEXT("Food and Water"),
        TEXT("Power Generation"),
        TEXT("Medical Supplies"),
        TEXT("Emergency Personnel")
    };
    
    // 初始化國際合作夥伴
    InternationalPartners = {
        TEXT("United Nations"),
        TEXT("Red Cross"),
        TEXT("World Health Organization"),
        TEXT("International Rescue Committee"),
        TEXT("Doctors Without Borders"),
        TEXT("Global Disaster Relief")
    };
}

void UMingGoRTSGlobalEmergency::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    if (bGlobalEmergencyActive)
    {
        // 處理活躍緊急事件
        ProcessEmergencies();
        
        // 更新全球狀態
        UpdateGlobalStatus();
        
        // 協調響應努力
        CoordinateResponseEfforts();
        
        // 監控資源水平
        MonitorResourceLevels();
        
        // 更新緊急警報
        UpdateEmergencyAlerts();
    }
}

void UMingGoRTSGlobalEmergency::DetectEmergency(const FEmergencyEvent& EmergencyEvent)
{
    UE_LOG(LogTemp, Warning, TEXT("Emergency detected: %s"), *EmergencyEvent.EventName);
    
    // 添加到活躍緊急事件列表
    ActiveEmergencies.Add(EmergencyEvent);
    
    // 生成響應計劃
    FEmergencyResponsePlan ResponsePlan = GenerateResponsePlan(EmergencyEvent);
    ResponsePlans.Add(ResponsePlan);
    
    // 激活響應
    ActivateEmergencyResponse(EmergencyEvent);
    
    // 發出警報
    IssueEmergencyWarning(EmergencyEvent);
    
    // 通知事件
    NotifyEmergencyDetected(EmergencyEvent);
    
    // 更新全球威脅級別
    UpdateGlobalThreatLevel();
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, 
            FString::Printf(TEXT("🚨 EMERGENCY: %s 🚨"), *EmergencyEvent.EventName));
    }
}

void UMingGoRTSGlobalEmergency::ActivateEmergencyResponse(const FEmergencyEvent& Emergency)
{
    UE_LOG(LogTemp, Log, TEXT("Activating emergency response for: %s"), *Emergency.EventName);
    
    // 查找對應的響應計劃
    for (FEmergencyResponsePlan& Plan : ResponsePlans)
    {
        if (Plan.TargetEmergency == Emergency.EmergencyType && !Plan.bActive)
        {
            ExecuteResponsePlan(Plan);
            break;
        }
    }
    
    // 協調全球響應
    CoordinateGlobalResponse(Emergency);
    
    // 部署國際援助
    if (Emergency.RequiredResponse >= EEmergencyResponse::International)
    {
        DeployInternationalAid(Emergency.EventName);
    }
}

void UMingGoRTSGlobalEmergency::MonitorActiveEmergencies()
{
    UE_LOG(LogTemp, Log, TEXT("Monitoring %d active emergencies"), ActiveEmergencies.Num());
    
    // 更新每個緊急事件的進度
    for (FEmergencyEvent& Emergency : ActiveEmergencies)
    {
        UpdateEmergencyProgress(Emergency.EventName, GetWorld()->GetDeltaSeconds());
    }
    
    // 移除已解決的事件
    for (int32 i = ActiveEmergencies.Num() - 1; i >= 0; --i)
    {
        if (!ActiveEmergencies[i].bOngoing)
        {
            UE_LOG(LogTemp, Log, TEXT("Emergency resolved: %s"), *ActiveEmergencies[i].EventName);
            ActiveEmergencies.RemoveAt(i);
        }
    }
}

void UMingGoRTSGlobalEmergency::ResolveEmergency(const FString& EventName)
{
    UE_LOG(LogTemp, Log, TEXT("Resolving emergency: %s"), *EventName);
    
    // 查找並解決緊急事件
    for (FEmergencyEvent& Emergency : ActiveEmergencies)
    {
        if (Emergency.EventName == EventName)
        {
            Emergency.bOngoing = false;
            
            // 開始恢復階段
            InitiateRecoveryPhase(EventName);
            
            // 通知狀態更新
            NotifyStatusUpdated(EventName, true);
            
            break;
        }
    }
    
    // 更新全球威脅級別
    UpdateGlobalThreatLevel();
}

bool UMingGoRTSGlobalEmergency::IsEmergencyActive(const FString& EventName) const
{
    for (const FEmergencyEvent& Emergency : ActiveEmergencies)
    {
        if (Emergency.EventName == EventName)
        {
            return Emergency.bOngoing;
        }
    }
    return false;
}

void UMingGoRTSGlobalEmergency::CoordinateGlobalResponse(const FEmergencyEvent& Emergency)
{
    UE_LOG(LogTemp, Log, TEXT("Coordinating global response for: %s"), *Emergency.EventName);
    
    // 根據需要的響應級別進行協調
    switch (Emergency.RequiredResponse)
    {
    case EEmergencyResponse::Local:
        UE_LOG(LogTemp, Log, TEXT("Coordinating local response"));
        break;
    case EEmergencyResponse::Regional:
        UE_LOG(LogTemp, Log, TEXT("Coordinating regional response"));
        break;
    case EEmergencyResponse::National:
        UE_LOG(LogTemp, Log, TEXT("Coordinating national response"));
        break;
    case EEmergencyResponse::International:
        UE_LOG(LogTemp, Log, TEXT("Coordinating international response"));
        RequestInternationalAssistance(Emergency.EventName);
        break;
    case EEmergencyResponse::Global:
        UE_LOG(LogTemp, Log, TEXT("Coordinating global response"));
        ActivateGlobalAlertSystem();
        break;
    }
}

void UMingGoRTSGlobalEmergency::DeployInternationalAid(const FString& EventName)
{
    UE_LOG(LogTemp, Log, TEXT("Deploying international aid for: %s"), *EventName);
    
    // 聯繫國際合作夥伴
    for (const FString& Partner : InternationalPartners)
    {
        UE_LOG(LogTemp, Log, TEXT("Contacting partner: %s"), *Partner);
    }
    
    // 分配國際資源
    TArray<FString> InternationalResources = {
        TEXT("International Medical Teams"),
        TEXT("Foreign Rescue Units"),
        TEXT("International Relief Supplies"),
        TEXT("Global Logistics Support")
    };
    
    AllocateEmergencyResources(EventName, InternationalResources);
}

void UMingGoRTSGlobalEmergency::EstablishEmergencyProtocols()
{
    UE_LOG(LogTemp, Log, TEXT("Establishing emergency protocols"));
    
    // 設置標準緊急程序
    TArray<FString> Protocols = {
        TEXT("Immediate threat assessment"),
        TEXT("Rapid response deployment"),
        TEXT("Resource mobilization"),
        TEXT("International coordination"),
        TEXT("Continuous monitoring"),
        TEXT("Public communication"),
        TEXT("Recovery planning")
    };
    
    for (const FString& Protocol : Protocols)
    {
        UE_LOG(LogTemp, Log, TEXT("Protocol established: %s"), *Protocol);
    }
}

void UMingGoRTSGlobalEmergency::ActivateGlobalAlertSystem()
{
    UE_LOG(LogTemp, Warning, TEXT("🌍 GLOBAL ALERT SYSTEM ACTIVATED 🌍"));
    
    // 廣播全球警報
    BroadcastGlobalAlert(TEXT("Global emergency declared. All nations requested to assist."));
    
    // 激活所有響應系統
    for (const FEmergencyEvent& Emergency : ActiveEmergencies)
    {
        if (Emergency.RequiredResponse == EEmergencyResponse::Global)
        {
            ActivateEmergencyResponse(Emergency);
        }
    }
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("🌍 GLOBAL ALERT SYSTEM ACTIVATED 🌍"));
    }
}

void UMingGoRTSGlobalEmergency::AllocateEmergencyResources(const FString& EventName, const TArray<FString>& Resources)
{
    UE_LOG(LogTemp, Log, TEXT("Allocating emergency resources for: %s"), *EventName);
    
    for (const FString& Resource : Resources)
    {
        UE_LOG(LogTemp, Log, TEXT("Allocated: %s"), *Resource);
    }
}

void UMingGoRTSGlobalEmergency::RequestInternationalAssistance(const FString& EventName)
{
    UE_LOG(LogTemp, Log, TEXT("Requesting international assistance for: %s"), *EventName);
    
    // 向所有國際合作夥伴發出請求
    for (const FString& Partner : InternationalPartners)
    {
        UE_LOG(LogTemp, Log, TEXT("Assistance requested from: %s"), *Partner);
    }
}

void UMingGoRTSGlobalEmergency::ManageResourceDistribution()
{
    UE_LOG(LogTemp, Log, TEXT("Managing emergency resource distribution"));
    
    // 分析資源需求
    for (const FEmergencyEvent& Emergency : ActiveEmergencies)
    {
        if (Emergency.ImmediateNeeds.Num() > 0)
        {
            AllocateEmergencyResources(Emergency.EventName, Emergency.ImmediateNeeds);
        }
    }
}

TArray<FString> UMingGoRTSGlobalEmergency::GetAvailableEmergencyResources() const
{
    return AvailableResources;
}

void UMingGoRTSGlobalEmergency::IssueEmergencyWarning(const FEmergencyEvent& Emergency)
{
    UE_LOG(LogTemp, Warning, TEXT("Emergency warning issued for: %s"), *Emergency.EventName);
    
    // 根據嚴重程度發出不同級別的警報
    FString WarningMessage = FString::Printf(TEXT("EMERGENCY ALERT: %s - Severity: %.1f"), 
        *Emergency.EventName, Emergency.Severity);
    
    if (Emergency.Severity >= 8.0f)
    {
        BroadcastGlobalAlert(WarningMessage);
    }
    else
    {
        // 區域警報
        UE_LOG(LogTemp, Warning, TEXT("Regional alert: %s"), *WarningMessage);
    }
}

void UMingGoRTSGlobalEmergency::BroadcastGlobalAlert(const FString& AlertMessage)
{
    UE_LOG(LogTemp, Error, TEXT("GLOBAL ALERT: %s"), *AlertMessage);
    
    // 通知全球警報
    NotifyGlobalAlert(AlertMessage);
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, 
            FString::Printf(TEXT("🚨 GLOBAL ALERT: %s 🚨"), *AlertMessage));
    }
}

void UMingGoRTSGlobalEmergency::UpdateAlertLevels()
{
    UE_LOG(LogTemp, Log, TEXT("Updating emergency alert levels"));
    
    // 根據活躍緊急事件更新警報級別
    float MaxSeverity = 0.0f;
    for (const FEmergencyEvent& Emergency : ActiveEmergencies)
    {
        if (Emergency.Severity > MaxSeverity)
        {
            MaxSeverity = Emergency.Severity;
        }
    }
    
    GlobalThreatLevel = MaxSeverity;
}

void UMingGoRTSGlobalEmergency::EvacuateAffectedAreas(const FString& EventName)
{
    UE_LOG(LogTemp, Log, TEXT("Evacuating affected areas for: %s"), *EventName);
    
    // 查找對應的緊急事件
    for (const FEmergencyEvent& Emergency : ActiveEmergencies)
    {
        if (Emergency.EventName == EventName)
        {
            UE_LOG(LogTemp, Log, TEXT("Evacuating %.0f people"), Emergency.AffectedPopulation);
            
            // 部署疏散資源
            TArray<FString> EvacuationResources = {
                TEXT("Evacuation Vehicles"),
                TEXT("Temporary Shelters"),
                TEXT("Emergency Personnel"),
                TEXT("Communication Systems")
            };
            
            AllocateEmergencyResources(EventName, EvacuationResources);
            break;
        }
    }
}

void UMingGoRTSGlobalEmergency::LaunchRescueOperations(const FString& EventName)
{
    UE_LOG(LogTemp, Log, TEXT("Launching rescue operations for: %s"), *EventName);
    
    // 部署搜救隊伍
    DeployMedicalTeams(EventName);
    CoordinateSearchAndRescue(EventName);
}

void UMingGoRTSGlobalEmergency::DeployMedicalTeams(const FString& EventName)
{
    UE_LOG(LogTemp, Log, TEXT("Deploying medical teams for: %s"), *EventName);
    
    // 分配醫療資源
    TArray<FString> MedicalResources = {
        TEXT("Emergency Medical Teams"),
        TEXT("Medical Supplies"),
        TEXT("Field Hospitals"),
        TEXT("Medical Evacuation")
    };
    
    AllocateEmergencyResources(EventName, MedicalResources);
}

void UMingGoRTSGlobalEmergency::EstablishReliefCamps(const FString& EventName)
{
    UE_LOG(LogTemp, Log, TEXT("Establishing relief camps for: %s"), *EventName);
    
    // 建立救濟營地
    TArray<FString> CampResources = {
        TEXT("Temporary Shelters"),
        TEXT("Food and Water"),
        TEXT("Sanitation Facilities"),
        TEXT("Security Personnel")
    };
    
    AllocateEmergencyResources(EventName, CampResources);
}

void UMingGoRTSGlobalEmergency::CoordinateSearchAndRescue(const FString& EventName)
{
    UE_LOG(LogTemp, Log, TEXT("Coordinating search and rescue for: %s"), *EventName);
    
    // 協調搜救行動
    TArray<FString> SARResources = {
        TEXT("Search and Rescue Teams"),
        TEXT("Rescue Equipment"),
        TEXT("Search Dogs"),
        TEXT("Aerial Support")
    };
    
    AllocateEmergencyResources(EventName, SARResources);
}

void UMingGoRTSGlobalEmergency::InitiateRecoveryPhase(const FString& EventName)
{
    UE_LOG(LogTemp, Log, TEXT("Initiating recovery phase for: %s"), *EventName);
    
    // 開始重建工作
    CoordinateReconstructionEfforts(EventName);
    ProvideLongTermSupport(EventName);
}

void UMingGoRTSGlobalEmergency::CoordinateReconstructionEfforts(const FString& EventName)
{
    UE_LOG(LogTemp, Log, TEXT("Coordinating reconstruction efforts for: %s"), *EventName);
    
    // 協調重建資源
    TArray<FString> ReconstructionResources = {
        TEXT("Construction Teams"),
        TEXT("Building Materials"),
        TEXT("Engineering Support"),
        TEXT("Infrastructure Equipment")
    };
    
    AllocateEmergencyResources(EventName, ReconstructionResources);
}

void UMingGoRTSGlobalEmergency::ProvideLongTermSupport(const FString& EventName)
{
    UE_LOG(LogTemp, Log, TEXT("Providing long-term support for: %s"), *EventName);
    
    // 提供長期支持
    TArray<FString> SupportResources = {
        TEXT("Psychological Support"),
        TEXT("Economic Assistance"),
        TEXT("Educational Support"),
        TEXT("Community Development")
    };
    
    AllocateEmergencyResources(EventName, SupportResources);
}

void UMingGoRTSGlobalEmergency::MonitorRecoveryProgress(const FString& EventName)
{
    UE_LOG(LogTemp, Log, TEXT("Monitoring recovery progress for: %s"), *EventName);
    
    // 監控恢復進度
    float Progress = FMath::FRandRange(0.3, 0.9);
    UE_LOG(LogTemp, Log, TEXT("Recovery progress: %.1f%%"), Progress * 100);
}

void UMingGoRTSGlobalEmergency::StrengthenPreparednessSystems()
{
    UE_LOG(LogTemp, Log, TEXT("Strengthening emergency preparedness systems"));
    
    // 加強準備系統
    TArray<FString> PreparednessActions = {
        TEXT("Update emergency protocols"),
        TEXT("Conduct risk assessments"),
        TEXT("Train emergency personnel"),
        TEXT("Stockpile emergency supplies"),
        TEXT("Establish communication networks")
    };
    
    for (const FString& Action : PreparednessActions)
    {
        UE_LOG(LogTemp, Log, TEXT("Preparedness action: %s"), *Action);
    }
}

void UMingGoRTSGlobalEmergency::ConductEmergencyDrills()
{
    UE_LOG(LogTemp, Log, TEXT("Conducting emergency drills"));
    
    // 進行緊急演習
    TArray<FString> DrillTypes = {
        TEXT("Evacuation Drill"),
        TEXT("Medical Response Drill"),
        TEXT("Search and Rescue Drill"),
        TEXT("Communication Drill"),
        TEXT("Coordination Drill")
    };
    
    for (const FString& Drill : DrillTypes)
    {
        UE_LOG(LogTemp, Log, TEXT("Conducting: %s"), *Drill);
    }
}

void UMingGoRTSGlobalEmergency::UpdateEmergencyProtocols()
{
    UE_LOG(LogTemp, Log, TEXT("Updating emergency protocols"));
    
    // 基於經驗更新協議
    if (ActiveEmergencies.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Updating protocols based on active emergencies"));
    }
}

void UMingGoRTSGlobalEmergency::TrainEmergencyResponders()
{
    UE_LOG(LogTemp, Log, TEXT("Training emergency responders"));
    
    // 訓練緊急響應人員
    TArray<FString> TrainingAreas = {
        TEXT("Emergency Medical Response"),
        TEXT("Search and Rescue Techniques"),
        TEXT("Disaster Assessment"),
        TEXT("Crisis Communication"),
        TEXT("International Coordination")
    };
    
    for (const FString& Area : TrainingAreas)
    {
        UE_LOG(LogTemp, Log, TEXT("Training area: %s"), *Area);
    }
}

void UMingGoRTSGlobalEmergency::AnalyzeEmergencyPatterns()
{
    UE_LOG(LogTemp, Log, TEXT("Analyzing emergency patterns"));
    
    // 分析緊急事件模式
    if (ActiveEmergencies.Num() >= 3)
    {
        UE_LOG(LogTemp, Log, TEXT("Pattern detected: Increasing frequency of emergencies"));
        UE_LOG(LogTemp, Log, TEXT("Recommendation: Strengthen global coordination"));
    }
}

void UMingGoRTSGlobalEmergency::GenerateEmergencyReport()
{
    UE_LOG(LogTemp, Log, TEXT("Generating emergency report"));
    
    // 生成緊急事件報告
    FString Report = FString::Printf(TEXT("Emergency Report - Active Events: %d, Global Threat Level: %.1f"), 
        ActiveEmergencies.Num(), GlobalThreatLevel);
    
    UE_LOG(LogTemp, Log, TEXT("Report: %s"), *Report);
}

void UMingGoRTSGlobalEmergency::UpdateResponseStrategies()
{
    UE_LOG(LogTemp, Log, TEXT("Updating response strategies"));
    
    // 基於分析結果更新策略
    if (GlobalThreatLevel > 7.0f)
    {
        UE_LOG(LogTemp, Log, TEXT("Strategy updated: Enhanced global coordination"));
    }
}

void UMingGoRTSGlobalEmergency::ShareLessonsLearned()
{
    UE_LOG(LogTemp, Log, TEXT("Sharing lessons learned"));
    
    // 分享經驗教訓
    TArray<FString> Lessons = {
        TEXT("Importance of early detection"),
        TEXT("Value of international cooperation"),
        TEXT("Need for rapid response"),
        TEXT("Significance of resource coordination"),
        TEXT("Role of community involvement")
    };
    
    for (const FString& Lesson : Lessons)
    {
        UE_LOG(LogTemp, Log, TEXT("Lesson: %s"), *Lesson);
    }
}

void UMingGoRTSGlobalEmergency::ActivateGlobalEmergencyMode()
{
    if (bGlobalEmergencyActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("Global emergency mode already active"));
        return;
    }
    
    bGlobalEmergencyActive = true;
    
    UE_LOG(LogTemp, Warning, TEXT("🌍 GLOBAL EMERGENCY MODE ACTIVATED 🌍"));
    
    // 啟動所有緊急系統
    EstablishEmergencyProtocols();
    ActivateGlobalAlertSystem();
    
    // 啟動監控計時器
    if (!EmergencyTickerHandle.IsValid())
    {
        EmergencyTickerHandle = FTicker::GetCoreTicker().AddTicker(EmergencyTicker, 1.0f);
    }
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("🌍 GLOBAL EMERGENCY MODE ACTIVATED 🌍"));
    }
}

void UMingGoRTSGlobalEmergency::DeactivateGlobalEmergencyMode()
{
    bGlobalEmergencyActive = false;
    
    UE_LOG(LogTemp, Log, TEXT("Global emergency mode deactivated"));
    
    // 停止監控計時器
    if (EmergencyTickerHandle.IsValid())
    {
        FTicker::GetCoreTicker().RemoveTicker(EmergencyTickerHandle);
        EmergencyTickerHandle.Reset();
    }
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Gray, TEXT("Global Emergency Mode Deactivated"));
    }
}

bool UMingGoRTSGlobalEmergency::IsGlobalEmergencyActive() const
{
    return bGlobalEmergencyActive;
}

void UMingGoRTSGlobalEmergency::ResetEmergencySystem()
{
    UE_LOG(LogTemp, Log, TEXT("Resetting emergency system"));
    
    // 重置所有數據
    ActiveEmergencies.Empty();
    ResponsePlans.Empty();
    GlobalThreatLevel = 0.0f;
    bGlobalEmergencyActive = false;
    
    UE_LOG(LogTemp, Log, TEXT("Emergency system reset completed"));
}

// Private helper functions
void UMingGoRTSGlobalEmergency::ProcessEmergencies()
{
    // 處理所有活躍的緊急事件
    for (FEmergencyEvent& Emergency : ActiveEmergencies)
    {
        if (Emergency.bOngoing)
        {
            // 更新緊急事件狀態
            Emergency.TimeRemaining -= GetWorld()->GetDeltaSeconds();
            
            if (Emergency.TimeRemaining <= 0)
            {
                Emergency.bOngoing = false;
                NotifyStatusUpdated(Emergency.EventName, true);
            }
        }
    }
}

void UMingGoRTSGlobalEmergency::UpdateGlobalStatus()
{
    // 更新全球狀態
    UpdateAlertLevels();
    GenerateEmergencyReport();
}

void UMingGoRTSGlobalEmergency::CoordinateResponseEfforts()
{
    // 協調響應努力
    ManageResourceDistribution();
}

void UMingGoRTSGlobalEmergency::MonitorResourceLevels()
{
    // 監控資源水平
    for (const FString& Resource : AvailableResources)
    {
        // 模擬資源水平監控
        float ResourceLevel = FMath::FRandRange(0.6, 1.0);
        if (ResourceLevel < 0.3)
        {
            UE_LOG(LogTemp, Warning, TEXT("Low resource level: %s"), *Resource);
        }
    }
}

void UMingGoRTSGlobalEmergency::UpdateEmergencyAlerts()
{
    // 更新緊急警報
    if (GlobalThreatLevel > 8.0f)
    {
        BroadcastGlobalAlert(TEXT("Critical global threat level detected"));
    }
}

bool UMingGoRTSGlobalEmergency::OnEmergencyTick(float DeltaTime)
{
    if (!bGlobalEmergencyActive)
    {
        return false;
    }
    
    // 執行緊急監控任務
    MonitorActiveEmergencies();
    AnalyzeEmergencyPatterns();
    
    return bGlobalEmergencyActive;
}

void UMingGoRTSGlobalEmergency::NotifyEmergencyDetected(const FEmergencyEvent& Emergency)
{
    OnEmergencyDetected.Broadcast(Emergency);
}

void UMingGoRTSGlobalEmergency::NotifyResponseActivated(const FEmergencyResponsePlan& Plan)
{
    OnEmergencyResponseActivated.Broadcast(Plan);
}

void UMingGoRTSGlobalEmergency::NotifyStatusUpdated(const FString& EventName, bool bResolved)
{
    OnEmergencyStatusUpdated.Broadcast(EventName, bResolved);
}

void UMingGoRTSGlobalEmergency::NotifyGlobalAlert(const FString& AlertMessage)
{
    OnGlobalEmergencyAlert.Broadcast(AlertMessage);
}

FEmergencyResponsePlan UMingGoRTSGlobalEmergency::GenerateResponsePlan(const FEmergencyEvent& Emergency)
{
    FEmergencyResponsePlan Plan;
    Plan.PlanName = FString::Printf(TEXT("Response Plan for %s"), *Emergency.EventName);
    Plan.TargetEmergency = Emergency.EmergencyType;
    Plan.ResponseLevel = Emergency.RequiredResponse;
    Plan.EstimatedSuccessRate = FMath::FRandRange(0.7, 0.95);
    Plan.DeploymentTime = FMath::FRandRange(5, 30);
    Plan.bActive = false;
    
    // 生成響應行動
    switch (Emergency.EmergencyType)
    {
    case EEmergencyType::NaturalDisaster:
        Plan.ResponseActions.Add(TEXT("Deploy search and rescue teams"));
        Plan.ResponseActions.Add(TEXT("Establish emergency shelters"));
        Plan.ResponseActions.Add(TEXT("Coordinate evacuation"));
        break;
    case EEmergencyType::Pandemic:
        Plan.ResponseActions.Add(TEXT("Deploy medical teams"));
        Plan.ResponseActions.Add(TEXT("Establish quarantine zones"));
        Plan.ResponseActions.Add(TEXT("Distribute medical supplies"));
        break;
    case EEmergencyType::ClimateCrisis:
        Plan.ResponseActions.Add(TEXT("Implement climate mitigation"));
        Plan.ResponseActions.Add(TEXT("Deploy adaptation measures"));
        Plan.ResponseActions.Add(TEXT("Coordinate international response"));
        break;
    default:
        Plan.ResponseActions.Add(TEXT("Assess situation"));
        Plan.ResponseActions.Add(TEXT("Deploy emergency resources"));
        Plan.ResponseActions.Add(TEXT("Coordinate response efforts"));
        break;
    }
    
    // 計算所需資源
    Plan.RequiredResources = Emergency.ImmediateNeeds;
    
    // 設置協調機構
    Plan.CoordinatingAgencies = InternationalPartners;
    
    return Plan;
}

void UMingGoRTSGlobalEmergency::ExecuteResponsePlan(const FEmergencyResponsePlan& Plan)
{
    UE_LOG(LogTemp, Log, TEXT("Executing response plan: %s"), *Plan.PlanName);
    
    Plan.bActive = true;
    
    // 執行響應行動
    for (const FString& Action : Plan.ResponseActions)
    {
        UE_LOG(LogTemp, Log, TEXT("Executing action: %s"), *Action);
    }
    
    // 分配資源
    for (const FString& Resource : Plan.RequiredResources)
    {
        UE_LOG(LogTemp, Log, TEXT("Allocating resource: %s"), *Resource);
    }
    
    // 通知響應激活
    NotifyResponseActivated(Plan);
}

void UMingGoRTSGlobalEmergency::UpdateEmergencyProgress(const FString& EventName, float DeltaTime)
{
    // 更新緊急事件進度
    for (FEmergencyEvent& Emergency : ActiveEmergencies)
    {
        if (Emergency.EventName == EventName)
        {
            Emergency.TimeRemaining -= DeltaTime;
            
            if (Emergency.TimeRemaining <= 0)
            {
                Emergency.bOngoing = false;
                NotifyStatusUpdated(EventName, true);
            }
            break;
        }
    }
}

void UMingGoRTSGlobalEmergency::UpdateGlobalThreatLevel()
{
    // 更新全球威脅級別
    float MaxThreat = 0.0f;
    
    for (const FEmergencyEvent& Emergency : ActiveEmergencies)
    {
        if (Emergency.Severity > MaxThreat)
        {
            MaxThreat = Emergency.Severity;
        }
    }
    
    GlobalThreatLevel = MaxThreat;
}
