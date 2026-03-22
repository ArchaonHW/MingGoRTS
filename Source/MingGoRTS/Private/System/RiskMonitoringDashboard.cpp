#include "RiskMonitoringDashboard.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

URiskMonitoringDashboard::URiskMonitoringDashboard()
    : AutoAssessmentInterval(300.0f) // 5分鐘
    , bAutoAssessmentEnabled(true)
{
}

void URiskMonitoringDashboard::InitializeDashboard()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing Risk Monitoring Dashboard"));
    
    // 初始化風險閾值
    InitializeRiskThresholds();
    
    // 初始化默認風險項目
    InitializeDefaultRisks();
    
    // 記錄初始化時間
    LastAssessmentTime = FDateTime::Now();
    
    // 計算初始風險指標
    CurrentMetrics = CalculateRiskMetrics();
    
    UE_LOG(LogTemp, Log, TEXT("Risk Monitoring Dashboard initialized with %d risk items"), RiskItems.Num());
}

void URiskMonitoringDashboard::InitializeRiskThresholds()
{
    RiskThresholds.Empty();
    
    // 設置默認風險閾值
    RiskThresholds.Add(ERiskLevel::Critical, 80.0f);
    RiskThresholds.Add(ERiskLevel::High, 60.0f);
    RiskThresholds.Add(ERiskLevel::Medium, 40.0f);
    RiskThresholds.Add(ERiskLevel::Low, 20.0f);
    RiskThresholds.Add(ERiskLevel::Minimal, 0.0f);
}

void URiskMonitoringDashboard::InitializeDefaultRisks()
{
    // 添加MingGoRTS項目的默認高風險項目
    
    // 內建作業系統複雜度風險
    FRiskItem OSComplexityRisk;
    OSComplexityRisk.RiskID = TEXT("RISK-OS-001");
    OSComplexityRisk.Title = TEXT("內建作業系統複雜度");
    OSComplexityRisk.Description = TEXT("12週開發週期，8人團隊，技術複雜度極高，可能導致進度延遲和質量問題");
    OSComplexityRisk.Category = ERiskCategory::Technical;
    OSComplexityRisk.Level = ERiskLevel::High;
    OSComplexityRisk.Status = ERiskStatus::Active;
    OSComplexityRisk.Probability = 0.7f;
    OSComplexityRisk.Impact = 0.9f;
    OSComplexityRisk.RiskScore = CalculateRiskScore(OSComplexityRisk.Probability, OSComplexityRisk.Impact);
    OSComplexityRisk.MitigationStrategy = TEXT("分階段實施，模組化設計，並行開發，原型驗證");
    OSComplexityRisk.Owner = TEXT("技術負責人");
    OSComplexityRisk.IdentifiedDate = FDateTime::Now();
    OSComplexityRisk.LastUpdated = FDateTime::Now();
    OSComplexityRisk.RelatedTasks.Add(TEXT("os-complexity-solution"));
    OSComplexityRisk.Tags.Add(TEXT("技術風險"));
    OSComplexityRisk.Tags.Add(TEXT("高優先級"));
    
    // AI生成內容質量不穩定風險
    FRiskItem AIQualityRisk;
    AIQualityRisk.RiskID = TEXT("RISK-AI-001");
    AIQualityRisk.Title = TEXT("AI生成內容質量不穩定");
    AIQualityRisk.Description = TEXT("外部API依賴，質量波動，成本控制挑戰，影響用戶體驗");
    AIQualityRisk.Category = ERiskCategory::Quality;
    AIQualityRisk.Level = ERiskLevel::High;
    AIQualityRisk.Status = ERiskStatus::Active;
    AIQualityRisk.Probability = 0.6f;
    AIQualityRisk.Impact = 0.8f;
    AIQualityRisk.RiskScore = CalculateRiskScore(AIQualityRisk.Probability, AIQualityRisk.Impact);
    AIQualityRisk.MitigationStrategy = TEXT("多層質量控制，多供應商策略，成本控制機制，內容後處理");
    AIQualityRisk.Owner = TEXT("AI負責人");
    AIQualityRisk.IdentifiedDate = FDateTime::Now();
    AIQualityRisk.LastUpdated = FDateTime::Now();
    AIQualityRisk.RelatedTasks.Add(TEXT("ai-quality-solution"));
    AIQualityRisk.Tags.Add(TEXT("質量風險"));
    AIQualityRisk.Tags.Add(TEXT("高優先級"));
    
    // 性能瓶頸風險
    FRiskItem PerformanceRisk;
    PerformanceRisk.RiskID = TEXT("RISK-PERF-001");
    PerformanceRisk.Title = TEXT("性能瓶頸");
    PerformanceRisk.Description = TEXT("50+系統運行，內存/CPU負載，移動端性能不確定性");
    PerformanceRisk.Category = ERiskCategory::Performance;
    PerformanceRisk.Level = ERiskLevel::High;
    PerformanceRisk.Status = ERiskStatus::Active;
    PerformanceRisk.Probability = 0.8f;
    PerformanceRisk.Impact = 0.7f;
    PerformanceRisk.RiskScore = CalculateRiskScore(PerformanceRisk.Probability, PerformanceRisk.Impact);
    PerformanceRisk.MitigationStrategy = TEXT("智能資源管理，分級性能配置，異步處理，智能緩存");
    PerformanceRisk.Owner = TEXT("性能負責人");
    PerformanceRisk.IdentifiedDate = FDateTime::Now();
    PerformanceRisk.LastUpdated = FDateTime::Now();
    PerformanceRisk.RelatedTasks.Add(TEXT("performance-bottleneck-solution"));
    PerformanceRisk.Tags.Add(TEXT("性能風險"));
    PerformanceRisk.Tags.Add(TEXT("高優先級"));
    
    // 技術債務積累風險
    FRiskItem TechnicalDebtRisk;
    TechnicalDebtRisk.RiskID = TEXT("RISK-TECH-001");
    TechnicalDebtRisk.Title = TEXT("技術債務積累");
    TechnicalDebtRisk.Description = TEXT("快速開發導致代碼質量下降，測試覆蓋不足，長期維護困難");
    TechnicalDebtRisk.Category = ERiskCategory::Technical;
    TechnicalDebtRisk.Level = ERiskLevel::Medium;
    TechnicalDebtRisk.Status = ERiskStatus::Active;
    TechnicalDebtRisk.Probability = 0.6f;
    TechnicalDebtRisk.Impact = 0.6f;
    TechnicalDebtRisk.RiskScore = CalculateRiskScore(TechnicalDebtRisk.Probability, TechnicalDebtRisk.Impact);
    TechnicalDebtRisk.MitigationStrategy = TEXT("定期代碼審查，自動化測試，重構計劃，質量門檻");
    TechnicalDebtRisk.Owner = TEXT("開發負責人");
    TechnicalDebtRisk.IdentifiedDate = FDateTime::Now();
    TechnicalDebtRisk.LastUpdated = FDateTime::Now();
    TechnicalDebtRisk.RelatedTasks.Add(TEXT("technical-debt-cleanup"));
    TechnicalDebtRisk.Tags.Add(TEXT("技術風險"));
    TechnicalDebtRisk.Tags.Add(TEXT("中優先級"));
    
    // 多平台兼容性風險
    FRiskItem CompatibilityRisk;
    CompatibilityRisk.RiskID = TEXT("RISK-COMPAT-001");
    CompatibilityRisk.Title = TEXT("多平台兼容性");
    CompatibilityRisk.Description = TEXT("Windows/Android/iOS差異大，測試矩陣複雜，平台特定問題");
    CompatibilityRisk.Category = ERiskCategory::Technical;
    CompatibilityRisk.Level = ERiskLevel::Medium;
    CompatibilityRisk.Status = ERiskStatus::Active;
    CompatibilityRisk.Probability = 0.5f;
    CompatibilityRisk.Impact = 0.7f;
    CompatibilityRisk.RiskScore = CalculateRiskScore(CompatibilityRisk.Probability, CompatibilityRisk.Impact);
    CompatibilityRisk.MitigationStrategy = TEXT("平台抽象層，自動化測試，早期平台測試，兼容性檢查");
    CompatibilityRisk.Owner = TEXT("平台負責人");
    CompatibilityRisk.IdentifiedDate = FDateTime::Now();
    CompatibilityRisk.LastUpdated = FDateTime::Now();
    CompatibilityRisk.Tags.Add(TEXT("平台風險"));
    CompatibilityRisk.Tags.Add(TEXT("中優先級"));
    
    // 添加到風險列表
    RiskItems.Add(OSComplexityRisk);
    RiskItems.Add(AIQualityRisk);
    RiskItems.Add(PerformanceRisk);
    RiskItems.Add(TechnicalDebtRisk);
    RiskItems.Add(CompatibilityRisk);
    
    UE_LOG(LogTemp, Log, TEXT("Added %d default risk items"), RiskItems.Num());
}

bool URiskMonitoringDashboard::AddRiskItem(const FRiskItem& RiskItem)
{
    // 驗證風險項目
    if (!ValidateRiskItem(RiskItem))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid risk item: %s"), *RiskItem.RiskID);
        return false;
    }
    
    // 檢查是否已存在
    for (const FRiskItem& ExistingRisk : RiskItems)
    {
        if (ExistingRisk.RiskID == RiskItem.RiskID)
        {
            UE_LOG(LogTemp, Warning, TEXT("Risk item already exists: %s"), *RiskItem.RiskID);
            return false;
        }
    }
    
    // 添加風險項目
    RiskItems.Add(RiskItem);
    
    // 觸發事件
    OnRiskAdded.Broadcast(RiskItem);
    
    // 檢查風險閾值
    CheckRiskThresholds();
    
    // 更新風險指標
    CurrentMetrics = CalculateRiskMetrics();
    OnRiskMetricsUpdated.Broadcast(CurrentMetrics);
    
    UE_LOG(LogTemp, Log, TEXT("Risk item added: %s"), *RiskItem.RiskID);
    return true;
}

bool URiskMonitoringDashboard::UpdateRiskItem(const FString& RiskID, const FRiskItem& UpdatedRisk)
{
    // 驗證風險項目
    if (!ValidateRiskItem(UpdatedRisk))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid updated risk item: %s"), *RiskID);
        return false;
    }
    
    // 查找並更新風險項目
    for (int32 i = 0; i < RiskItems.Num(); ++i)
    {
        if (RiskItems[i].RiskID == RiskID)
        {
            ERiskLevel PreviousLevel = RiskItems[i].Level;
            RiskItems[i] = UpdatedRisk;
            RiskItems[i].LastUpdated = FDateTime::Now();
            
            // 觸發事件
            OnRiskUpdated.Broadcast(UpdatedRisk, PreviousLevel);
            
            // 檢查風險閾值
            CheckRiskThresholds();
            
            // 更新風險指標
            CurrentMetrics = CalculateRiskMetrics();
            OnRiskMetricsUpdated.Broadcast(CurrentMetrics);
            
            UE_LOG(LogTemp, Log, TEXT("Risk item updated: %s"), *RiskID);
            return true;
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Risk item not found: %s"), *RiskID);
    return false;
}

bool URiskMonitoringDashboard::RemoveRiskItem(const FString& RiskID)
{
    for (int32 i = 0; i < RiskItems.Num(); ++i)
    {
        if (RiskItems[i].RiskID == RiskID)
        {
            RiskItems.RemoveAt(i);
            
            // 更新風險指標
            CurrentMetrics = CalculateRiskMetrics();
            OnRiskMetricsUpdated.Broadcast(CurrentMetrics);
            
            UE_LOG(LogTemp, Log, TEXT("Risk item removed: %s"), *RiskID);
            return true;
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Risk item not found for removal: %s"), *RiskID);
    return false;
}

FRiskItem URiskMonitoringDashboard::GetRiskItem(const FString& RiskID) const
{
    for (const FRiskItem& Risk : RiskItems)
    {
        if (Risk.RiskID == RiskID)
        {
            return Risk;
        }
    }
    
    return FRiskItem(); // 返回空的風險項目
}

TArray<FRiskItem> URiskMonitoringDashboard::GetAllRiskItems() const
{
    return RiskItems;
}

TArray<FRiskItem> URiskMonitoringDashboard::GetRisksByCategory(ERiskCategory Category) const
{
    TArray<FRiskItem> FilteredRisks;
    
    for (const FRiskItem& Risk : RiskItems)
    {
        if (Risk.Category == Category)
        {
            FilteredRisks.Add(Risk);
        }
    }
    
    return FilteredRisks;
}

TArray<FRiskItem> URiskMonitoringDashboard::GetRisksByLevel(ERiskLevel Level) const
{
    TArray<FRiskItem> FilteredRisks;
    
    for (const FRiskItem& Risk : RiskItems)
    {
        if (Risk.Level == Level)
        {
            FilteredRisks.Add(Risk);
        }
    }
    
    return FilteredRisks;
}

TArray<FRiskItem> URiskMonitoringDashboard::GetRisksByStatus(ERiskStatus Status) const
{
    TArray<FRiskItem> FilteredRisks;
    
    for (const FRiskItem& Risk : RiskItems)
    {
        if (Risk.Status == Status)
        {
            FilteredRisks.Add(Risk);
        }
    }
    
    return FilteredRisks;
}

FRiskMetrics URiskMonitoringDashboard::CalculateRiskMetrics()
{
    FRiskMetrics Metrics;
    Metrics.TotalRisks = RiskItems.Num();
    Metrics.CriticalRisks = 0;
    Metrics.HighRisks = 0;
    Metrics.MediumRisks = 0;
    Metrics.LowRisks = 0;
    Metrics.AverageRiskScore = 0.0f;
    Metrics.RiskTrend = 0.0f;
    Metrics.LastCalculated = FDateTime::Now();
    
    // 計算各級別風險數量
    float TotalScore = 0.0f;
    for (const FRiskItem& Risk : RiskItems)
    {
        TotalScore += Risk.RiskScore;
        
        switch (Risk.Level)
        {
        case ERiskLevel::Critical:
            Metrics.CriticalRisks++;
            break;
        case ERiskLevel::High:
            Metrics.HighRisks++;
            break;
        case ERiskLevel::Medium:
            Metrics.MediumRisks++;
            break;
        case ERiskLevel::Low:
            Metrics.LowRisks++;
            break;
        case ERiskLevel::Minimal:
            // Minimal risks are included in Low count
            break;
        }
    }
    
    // 計算平均風險評分
    if (Metrics.TotalRisks > 0)
    {
        Metrics.AverageRiskScore = TotalScore / Metrics.TotalRisks;
    }
    
    // 計算類別分佈
    Metrics.CategoryDistribution.Empty();
    for (int32 i = 0; i < 8; ++i) // 8個風險類別
    {
        ERiskCategory Category = static_cast<ERiskCategory>(i);
        Metrics.CategoryDistribution.Add(Category, 0);
    }
    
    for (const FRiskItem& Risk : RiskItems)
    {
        int32* Count = Metrics.CategoryDistribution.Find(Risk.Category);
        if (Count)
        {
            (*Count)++;
        }
    }
    
    // 計算狀態分佈
    Metrics.StatusDistribution.Empty();
    for (int32 i = 0; i < 5; ++i) // 5個風險狀態
    {
        ERiskStatus Status = static_cast<ERiskStatus>(i);
        Metrics.StatusDistribution.Add(Status, 0);
    }
    
    for (const FRiskItem& Risk : RiskItems)
    {
        int32* Count = Metrics.StatusDistribution.Find(Risk.Status);
        if (Count)
        {
            (*Count)++;
        }
    }
    
    // 計算風險趨勢
    UpdateRiskTrend();
    if (RiskTrendData.Num() >= 2)
    {
        float CurrentScore = RiskTrendData.Last();
        float PreviousScore = RiskTrendData[RiskTrendData.Num() - 2];
        Metrics.RiskTrend = CurrentScore - PreviousScore;
    }
    
    return Metrics;
}

void URiskMonitoringDashboard::CreateRiskAlert(const FString& RiskID, const FString& AlertMessage, ERiskLevel Severity)
{
    FRiskAlert Alert;
    Alert.AlertID = FString::Printf(TEXT("ALERT-%s-%s"), *RiskID, *FDateTime::Now().ToString(TEXT("YYYYMMDD-HHMMSS")));
    Alert.RiskID = RiskID;
    Alert.AlertTitle = FString::Printf(TEXT("風險預警: %s"), *RiskID);
    Alert.AlertMessage = AlertMessage;
    Alert.Severity = Severity;
    Alert.Timestamp = FDateTime::Now();
    Alert.bIsRead = false;
    Alert.ActionRequired = TEXT("請立即評估並制定應對策略");
    
    // 添加到預警列表
    RiskAlerts.Add(Alert);
    
    // 觸發預警事件
    OnRiskAlert.Broadcast(Alert);
    
    UE_LOG(LogTemp, Warning, TEXT("Risk alert created: %s - %s"), *Alert.AlertID, *AlertMessage);
}

TArray<FRiskAlert> URiskMonitoringDashboard::GetRiskAlerts(bool bUnreadOnly) const
{
    if (bUnreadOnly)
    {
        TArray<FRiskAlert> UnreadAlerts;
        for (const FRiskAlert& Alert : RiskAlerts)
        {
            if (!Alert.bIsRead)
            {
                UnreadAlerts.Add(Alert);
            }
        }
        return UnreadAlerts;
    }
    
    return RiskAlerts;
}

void URiskMonitoringDashboard::MarkAlertAsRead(const FString& AlertID)
{
    for (FRiskAlert& Alert : RiskAlerts)
    {
        if (Alert.AlertID == AlertID)
        {
            Alert.bIsRead = true;
            UE_LOG(LogTemp, Log, TEXT("Alert marked as read: %s"), *AlertID);
            return;
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Alert not found: %s"), *AlertID);
}

void URiskMonitoringDashboard::ClearAlert(const FString& AlertID)
{
    for (int32 i = 0; i < RiskAlerts.Num(); ++i)
    {
        if (RiskAlerts[i].AlertID == AlertID)
        {
            RiskAlerts.RemoveAt(i);
            UE_LOG(LogTemp, Log, TEXT("Alert cleared: %s"), *AlertID);
            return;
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Alert not found for clearing: %s"), *AlertID);
}

void URiskMonitoringDashboard::ClearAllAlerts()
{
    int32 ClearedCount = RiskAlerts.Num();
    RiskAlerts.Empty();
    UE_LOG(LogTemp, Log, TEXT("Cleared %d alerts"), ClearedCount);
}

void URiskMonitoringDashboard::PerformAutomaticRiskAssessment()
{
    if (!bAutoAssessmentEnabled)
    {
        return;
    }
    
    FDateTime CurrentTime = FDateTime::Now();
    FTimespan TimeSinceLastAssessment = CurrentTime - LastAssessmentTime;
    
    if (TimeSinceLastAssessment.GetTotalSeconds() < AutoAssessmentInterval)
    {
        return; // 還沒到評估時間
    }
    
    UE_LOG(LogTemp, Log, TEXT("Performing automatic risk assessment"));
    
    // 評估所有活躍風險
    for (FRiskItem& Risk : RiskItems)
    {
        if (Risk.Status == ERiskStatus::Active)
        {
            // 模擬風險評估邏輯
            // 在實際實現中，這裡會有更複雜的評估算法
            
            // 檢查風險是否需要升級
            if (Risk.RiskScore > RiskThresholds[ERiskLevel::Critical] && Risk.Level != ERiskLevel::Critical)
            {
                ERiskLevel PreviousLevel = Risk.Level;
                Risk.Level = ERiskLevel::Critical;
                Risk.LastUpdated = CurrentTime;
                
                // 創建高優先級預警
                CreateRiskAlert(Risk.RiskID, GenerateAlertMessage(Risk), ERiskLevel::Critical);
                
                // 觸發更新事件
                OnRiskUpdated.Broadcast(Risk, PreviousLevel);
            }
            else if (Risk.RiskScore > RiskThresholds[ERiskLevel::High] && Risk.Level == ERiskLevel::Medium)
            {
                ERiskLevel PreviousLevel = Risk.Level;
                Risk.Level = ERiskLevel::High;
                Risk.LastUpdated = CurrentTime;
                
                // 創建高風險預警
                CreateRiskAlert(Risk.RiskID, GenerateAlertMessage(Risk), ERiskLevel::High);
                
                // 觸發更新事件
                OnRiskUpdated.Broadcast(Risk, PreviousLevel);
            }
        }
    }
    
    // 更新評估時間
    LastAssessmentTime = CurrentTime;
    
    // 重新計算風險指標
    CurrentMetrics = CalculateRiskMetrics();
    OnRiskMetricsUpdated.Broadcast(CurrentMetrics);
    
    UE_LOG(LogTemp, Log, TEXT("Automatic risk assessment completed"));
}

FString URiskMonitoringDashboard::GenerateRiskReport() const
{
    FString Report;
    Report += TEXT("=== 風險監控報告 ===\n\n");
    
    // 基本信息
    Report += FString::Printf(TEXT("報告生成時間: %s\n"), *FDateTime::Now().ToString());
    Report += FString::Printf(TEXT("總風險數量: %d\n"), CurrentMetrics.TotalRisks);
    Report += FString::Printf(TEXT("平均風險評分: %.1f\n"), CurrentMetrics.AverageRiskScore);
    Report += FString::Printf(TEXT("風險趨勢: %+.1f\n\n"), CurrentMetrics.RiskTrend);
    
    // 風險分級統計
    Report += TEXT("=== 風險分級統計 ===\n");
    Report += FString::Printf(TEXT("嚴重風險: %d\n"), CurrentMetrics.CriticalRisks);
    Report += FString::Printf(TEXT("高風險: %d\n"), CurrentMetrics.HighRisks);
    Report += FString::Printf(TEXT("中風險: %d\n"), CurrentMetrics.MediumRisks);
    Report += FString::Printf(TEXT("低風險: %d\n\n"), CurrentMetrics.LowRisks);
    
    // 類別分佈
    Report += TEXT("=== 風險類別分佈 ===\n");
    for (const auto& CategoryPair : CurrentMetrics.CategoryDistribution)
    {
        FString CategoryName = UEnum::GetDisplayValueAsText(CategoryPair.Key).ToString();
        Report += FString::Printf(TEXT("%s: %d\n"), *CategoryName, CategoryPair.Value);
    }
    Report += TEXT("\n");
    
    // 狀態分佈
    Report += TEXT("=== 風險狀態分佈 ===\n");
    for (const auto& StatusPair : CurrentMetrics.StatusDistribution)
    {
        FString StatusName = UEnum::GetDisplayValueAsText(StatusPair.Key).ToString();
        Report += FString::Printf(TEXT("%s: %d\n"), *StatusName, StatusPair.Value);
    }
    Report += TEXT("\n");
    
    // 高風險項目詳情
    Report += TEXT("=== 高風險項目詳情 ===\n");
    TArray<FRiskItem> HighRiskItems = GetHighRiskItems();
    for (const FRiskItem& Risk : HighRiskItems)
    {
        Report += FString::Printf(TEXT("\n風險ID: %s\n"), *Risk.RiskID);
        Report += FString::Printf(TEXT("標題: %s\n"), *Risk.Title);
        Report += FString::Printf(TEXT("級別: %s\n"), *UEnum::GetDisplayValueAsText(Risk.Level).ToString());
        Report += FString::Printf(TEXT("評分: %.1f\n"), Risk.RiskScore);
        Report += FString::Printf(TEXT("負責人: %s\n"), *Risk.Owner);
        Report += FString::Printf(TEXT("緩解策略: %s\n"), *Risk.MitigationStrategy);
    }
    
    // 預警信息
    Report += TEXT("\n=== 預警信息 ===\n");
    TArray<FRiskAlert> UnreadAlerts = GetRiskAlerts(true);
    if (UnreadAlerts.Num() > 0)
    {
        for (const FRiskAlert& Alert : UnreadAlerts)
        {
            Report += FString::Printf(TEXT("[%s] %s: %s\n"), 
                *UEnum::GetDisplayValueAsText(Alert.Severity).ToString(),
                *Alert.AlertTitle,
                *Alert.AlertMessage);
        }
    }
    else
    {
        Report += TEXT("無未讀預警\n");
    }
    
    return Report;
}

TArray<float> URiskMonitoringDashboard::GetRiskTrendAnalysis(int32 Days) const
{
    TArray<float> TrendData;
    
    if (RiskTrendData.Num() == 0)
    {
        return TrendData;
    }
    
    // 返回最近指定天數的趨勢數據
    int32 StartIndex = FMath::Max(0, RiskTrendData.Num() - Days);
    for (int32 i = StartIndex; i < RiskTrendData.Num(); ++i)
    {
        TrendData.Add(RiskTrendData[i]);
    }
    
    return TrendData;
}

TArray<FRiskItem> URiskMonitoringDashboard::GetHighRiskItems() const
{
    TArray<FRiskItem> HighRiskItems;
    
    for (const FRiskItem& Risk : RiskItems)
    {
        if (Risk.Level == ERiskLevel::Critical || Risk.Level == ERiskLevel::High)
        {
            HighRiskItems.Add(Risk);
        }
    }
    
    return HighRiskItems;
}

TArray<FRiskItem> URiskMonitoringDashboard::GetCriticalRisks() const
{
    return GetRisksByLevel(ERiskLevel::Critical);
}

void URiskMonitoringDashboard::SetRiskThreshold(ERiskLevel Level, float Threshold)
{
    RiskThresholds[Level] = Threshold;
    UE_LOG(LogTemp, Log, TEXT("Risk threshold set: %s = %.1f"), 
        *UEnum::GetDisplayValueAsText(Level).ToString(), Threshold);
}

float URiskMonitoringDashboard::GetRiskThreshold(ERiskLevel Level) const
{
    if (RiskThresholds.Contains(Level))
    {
        return RiskThresholds[Level];
    }
    return 50.0f; // 默認閾值
}

bool URiskMonitoringDashboard::ExportRiskData(const FString& FilePath) const
{
    // 創建JSON對象
    TSharedPtr<FJsonObject> RootObject = MakeShareable(new FJsonObject);
    
    // 導出風險項目
    TArray<TSharedPtr<FJsonValue>> RiskArray;
    for (const FRiskItem& Risk : RiskItems)
    {
        TSharedPtr<FJsonObject> RiskObject = MakeShareable(new FJsonObject);
        RiskObject->SetStringField(TEXT("RiskID"), Risk.RiskID);
        RiskObject->SetStringField(TEXT("Title"), Risk.Title);
        RiskObject->SetStringField(TEXT("Description"), Risk.Description);
        RiskObject->SetNumberField(TEXT("Category"), static_cast<int32>(Risk.Category));
        RiskObject->SetNumberField(TEXT("Level"), static_cast<int32>(Risk.Level));
        RiskObject->SetNumberField(TEXT("Status"), static_cast<int32>(Risk.Status));
        RiskObject->SetNumberField(TEXT("Probability"), Risk.Probability);
        RiskObject->SetNumberField(TEXT("Impact"), Risk.Impact);
        RiskObject->SetNumberField(TEXT("RiskScore"), Risk.RiskScore);
        RiskObject->SetStringField(TEXT("MitigationStrategy"), Risk.MitigationStrategy);
        RiskObject->SetStringField(TEXT("Owner"), Risk.Owner);
        
        RiskArray.Add(MakeShareable(new FJsonValueObject(RiskObject)));
    }
    RootObject->SetArrayField(TEXT("RiskItems"), RiskArray);
    
    // 導出預警
    TArray<TSharedPtr<FJsonValue>> AlertArray;
    for (const FRiskAlert& Alert : RiskAlerts)
    {
        TSharedPtr<FJsonObject> AlertObject = MakeShareable(new FJsonObject);
        AlertObject->SetStringField(TEXT("AlertID"), Alert.AlertID);
        AlertObject->SetStringField(TEXT("RiskID"), Alert.RiskID);
        AlertObject->SetStringField(TEXT("AlertTitle"), Alert.AlertTitle);
        AlertObject->SetStringField(TEXT("AlertMessage"), Alert.AlertMessage);
        AlertObject->SetNumberField(TEXT("Severity"), static_cast<int32>(Alert.Severity));
        AlertObject->SetBoolField(TEXT("IsRead"), Alert.bIsRead);
        
        AlertArray.Add(MakeShareable(new FJsonValueObject(AlertObject)));
    }
    RootObject->SetArrayField(TEXT("RiskAlerts"), AlertArray);
    
    // 序列化並保存到文件
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer);
    
    return FFileHelper::SaveStringToFile(OutputString, *FilePath);
}

bool URiskMonitoringDashboard::ImportRiskData(const FString& FilePath)
{
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load risk data file: %s"), *FilePath);
        return false;
    }
    
    TSharedPtr<FJsonObject> RootObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
    
    if (!FJsonSerializer::Deserialize(RootObject, Reader))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse risk data JSON"));
        return false;
    }
    
    // 導入風險項目
    const TArray<TSharedPtr<FJsonValue>>* RiskArray;
    if (RootObject->TryGetArrayField(TEXT("RiskItems"), RiskArray))
    {
        RiskItems.Empty();
        for (const TSharedPtr<FJsonValue>& RiskValue : *RiskArray)
        {
            const TSharedPtr<FJsonObject>* RiskObject;
            if (RiskValue->TryGetObject(RiskObject))
            {
                FRiskItem Risk;
                Risk.RiskID = (*RiskObject)->GetStringField(TEXT("RiskID"));
                Risk.Title = (*RiskObject)->GetStringField(TEXT("Title"));
                Risk.Description = (*RiskObject)->GetStringField(TEXT("Description"));
                Risk.Category = static_cast<ERiskCategory>((*RiskObject)->GetNumberField(TEXT("Category")));
                Risk.Level = static_cast<ERiskLevel>((*RiskObject)->GetNumberField(TEXT("Level")));
                Risk.Status = static_cast<ERiskStatus>((*RiskObject)->GetNumberField(TEXT("Status")));
                Risk.Probability = (*RiskObject)->GetNumberField(TEXT("Probability"));
                Risk.Impact = (*RiskObject)->GetNumberField(TEXT("Impact"));
                Risk.RiskScore = (*RiskObject)->GetNumberField(TEXT("RiskScore"));
                Risk.MitigationStrategy = (*RiskObject)->GetStringField(TEXT("MitigationStrategy"));
                Risk.Owner = (*RiskObject)->GetStringField(TEXT("Owner"));
                Risk.IdentifiedDate = FDateTime::Now();
                Risk.LastUpdated = FDateTime::Now();
                
                RiskItems.Add(Risk);
            }
        }
    }
    
    // 導入預警
    const TArray<TSharedPtr<FJsonValue>>* AlertArray;
    if (RootObject->TryGetArrayField(TEXT("RiskAlerts"), AlertArray))
    {
        RiskAlerts.Empty();
        for (const TSharedPtr<FJsonValue>& AlertValue : *AlertArray)
        {
            const TSharedPtr<FJsonObject>* AlertObject;
            if (AlertValue->TryGetObject(AlertObject))
            {
                FRiskAlert Alert;
                Alert.AlertID = (*AlertObject)->GetStringField(TEXT("AlertID"));
                Alert.RiskID = (*AlertObject)->GetStringField(TEXT("RiskID"));
                Alert.AlertTitle = (*AlertObject)->GetStringField(TEXT("AlertTitle"));
                Alert.AlertMessage = (*AlertObject)->GetStringField(TEXT("AlertMessage"));
                Alert.Severity = static_cast<ERiskLevel>((*AlertObject)->GetNumberField(TEXT("Severity")));
                Alert.bIsRead = (*AlertObject)->GetBoolField(TEXT("IsRead"));
                Alert.Timestamp = FDateTime::Now();
                Alert.ActionRequired = TEXT("請評估導入的預警");
                
                RiskAlerts.Add(Alert);
            }
        }
    }
    
    // 重新計算指標
    CurrentMetrics = CalculateRiskMetrics();
    OnRiskMetricsUpdated.Broadcast(CurrentMetrics);
    
    UE_LOG(LogTemp, Log, TEXT("Risk data imported successfully: %d risks, %d alerts"), 
        RiskItems.Num(), RiskAlerts.Num());
    
    return true;
}

float URiskMonitoringDashboard::CalculateRiskScore(float Probability, float Impact) const
{
    return Probability * Impact * 100.0f;
}

void URiskMonitoringDashboard::CheckRiskThresholds()
{
    for (const FRiskItem& Risk : RiskItems)
    {
        if (Risk.Status != ERiskStatus::Active)
        {
            continue;
        }
        
        if (RiskThresholds.Contains(Risk.Level))
        {
            float Threshold = RiskThresholds[Risk.Level];
            if (Risk.RiskScore > Threshold)
            {
                CreateRiskAlert(Risk.RiskID, GenerateAlertMessage(Risk), Risk.Level);
            }
        }
    }
}

void URiskMonitoringDashboard::UpdateRiskTrend()
{
    // 計算當前總風險評分
    float TotalScore = 0.0f;
    for (const FRiskItem& Risk : RiskItems)
    {
        if (Risk.Status == ERiskStatus::Active)
        {
            TotalScore += Risk.RiskScore;
        }
    }
    
    // 添加到趨勢數據
    RiskTrendData.Add(TotalScore);
    
    // 限制數據點數量（保留最近30天）
    if (RiskTrendData.Num() > 30)
    {
        RiskTrendData.RemoveAt(0);
    }
}

FString URiskMonitoringDashboard::GenerateAlertMessage(const FRiskItem& RiskItem) const
{
    return FString::Printf(TEXT("風險 '%s' 評分為 %.1f，超過閾值，需要立即關注。負責人：%s"), 
        *RiskItem.Title, RiskItem.RiskScore, *RiskItem.Owner);
}

bool URiskMonitoringDashboard::ValidateRiskItem(const FRiskItem& RiskItem) const
{
    if (RiskItem.RiskID.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Risk ID cannot be empty"));
        return false;
    }
    
    if (RiskItem.Title.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Risk title cannot be empty"));
        return false;
    }
    
    if (RiskItem.Probability < 0.0f || RiskItem.Probability > 1.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Risk probability must be between 0.0 and 1.0"));
        return false;
    }
    
    if (RiskItem.Impact < 0.0f || RiskItem.Impact > 1.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Risk impact must be between 0.0 and 1.0"));
        return false;
    }
    
    return true;
}
