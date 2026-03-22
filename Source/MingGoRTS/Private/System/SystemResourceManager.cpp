#include "SystemResourceManager.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Stats/Stats.h"

USystemResourceManager::USystemResourceManager()
    : bAdaptivePerformanceEnabled(true)
    , PerformanceMonitorInterval(1.0f)
    , SystemPerformanceScore(0.0f)
{
}

void USystemResourceManager::InitializeResourceManager()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing System Resource Manager"));
    
    // 初始化資源使用情況映射
    InitializeResourceUsageMap();
    
    // 初始化資源閾值
    InitializeResourceThresholds();
    
    // 初始化默認性能配置
    InitializeDefaultPerformanceConfiguration();
    
    // 記錄初始化時間
    LastPerformanceUpdate = FDateTime::Now();
    
    // 計算初始性能評分
    SystemPerformanceScore = CalculatePerformanceScore();
    
    UE_LOG(LogTemp, Log, TEXT("System Resource Manager initialized successfully"));
}

void USystemResourceManager::InitializeResourceUsageMap()
{
    ResourceUsageMap.Empty();
    
    // 初始化各種資源類型的使用情況
    FResourceUsage CPUUsage;
    CPUUsage.ResourceType = ESystemResourceType::CPU;
    CPUUsage.CurrentUsage = 0.0f;
    CPUUsage.MaxUsage = 100.0f;
    CPUUsage.UsagePercentage = 0.0f;
    CPUUsage.AverageUsage = 0.0f;
    CPUUsage.PeakUsage = 0.0f;
    CPUUsage.LastUpdateTime = FDateTime::Now();
    ResourceUsageMap.Add(ESystemResourceType::CPU, CPUUsage);
    
    FResourceUsage MemoryUsage;
    MemoryUsage.ResourceType = ESystemResourceType::Memory;
    MemoryUsage.CurrentUsage = 0.0f;
    MemoryUsage.MaxUsage = 100.0f;
    MemoryUsage.UsagePercentage = 0.0f;
    MemoryUsage.AverageUsage = 0.0f;
    MemoryUsage.PeakUsage = 0.0f;
    MemoryUsage.LastUpdateTime = FDateTime::Now();
    ResourceUsageMap.Add(ESystemResourceType::Memory, MemoryUsage);
    
    FResourceUsage GPUUsage;
    GPUUsage.ResourceType = ESystemResourceType::GPU;
    GPUUsage.CurrentUsage = 0.0f;
    GPUUsage.MaxUsage = 100.0f;
    GPUUsage.UsagePercentage = 0.0f;
    GPUUsage.AverageUsage = 0.0f;
    GPUUsage.PeakUsage = 0.0f;
    GPUUsage.LastUpdateTime = FDateTime::Now();
    ResourceUsageMap.Add(ESystemResourceType::GPU, GPUUsage);
    
    FResourceUsage NetworkUsage;
    NetworkUsage.ResourceType = ESystemResourceType::Network;
    NetworkUsage.CurrentUsage = 0.0f;
    NetworkUsage.MaxUsage = 100.0f;
    NetworkUsage.UsagePercentage = 0.0f;
    NetworkUsage.AverageUsage = 0.0f;
    NetworkUsage.PeakUsage = 0.0f;
    NetworkUsage.LastUpdateTime = FDateTime::Now();
    ResourceUsageMap.Add(ESystemResourceType::Network, NetworkUsage);
    
    FResourceUsage DiskUsage;
    DiskUsage.ResourceType = ESystemResourceType::Disk;
    DiskUsage.CurrentUsage = 0.0f;
    DiskUsage.MaxUsage = 100.0f;
    DiskUsage.UsagePercentage = 0.0f;
    DiskUsage.AverageUsage = 0.0f;
    DiskUsage.PeakUsage = 0.0f;
    DiskUsage.LastUpdateTime = FDateTime::Now();
    ResourceUsageMap.Add(ESystemResourceType::Disk, DiskUsage);
}

void USystemResourceManager::InitializeResourceThresholds()
{
    ResourceThresholds.Empty();
    
    // 設置默認資源閾值
    ResourceThresholds.Add(ESystemResourceType::CPU, 80.0f);
    ResourceThresholds.Add(ESystemResourceType::Memory, 85.0f);
    ResourceThresholds.Add(ESystemResourceType::GPU, 90.0f);
    ResourceThresholds.Add(ESystemResourceType::Network, 70.0f);
    ResourceThresholds.Add(ESystemResourceType::Disk, 75.0f);
    ResourceThresholds.Add(ESystemResourceType::Audio, 60.0f);
    ResourceThresholds.Add(ESystemResourceType::Physics, 80.0f);
    ResourceThresholds.Add(ESystemResourceType::Rendering, 85.0f);
    ResourceThresholds.Add(ESystemResourceType::AI, 70.0f);
}

void USystemResourceManager::InitializeDefaultPerformanceConfiguration()
{
    CurrentPerformanceConfig.PerformanceTier = EPerformanceTier::High;
    CurrentPerformanceConfig.TargetFrameRate = 60.0f;
    CurrentPerformanceConfig.MaxConcurrentSystems = 50;
    CurrentPerformanceConfig.bEnableAdaptivePerformance = true;
    CurrentPerformanceConfig.ConfigurationName = TEXT("Default High Performance");
    
    // 設置資源限制
    CurrentPerformanceConfig.ResourceLimits.Add(ESystemResourceType::CPU, 80.0f);
    CurrentPerformanceConfig.ResourceLimits.Add(ESystemResourceType::Memory, 75.0f);
    CurrentPerformanceConfig.ResourceLimits.Add(ESystemResourceType::GPU, 85.0f);
    CurrentPerformanceConfig.ResourceLimits.Add(ESystemResourceType::Network, 60.0f);
    CurrentPerformanceConfig.ResourceLimits.Add(ESystemResourceType::Disk, 50.0f);
    
    // 設置資源閾值
    CurrentPerformanceConfig.ResourceThresholds = ResourceThresholds;
}

void USystemResourceManager::UpdateResourceUsage()
{
    FDateTime CurrentTime = FDateTime::Now();
    
    // 更新CPU使用率
    if (ResourceUsageMap.Contains(ESystemResourceType::CPU))
    {
        FResourceUsage& CPUUsage = ResourceUsageMap[ESystemResourceType::CPU];
        float PreviousUsage = CPUUsage.CurrentUsage;
        
        // 獲取實際CPU使用率（這裡使用模擬數據）
        CPUUsage.CurrentUsage = GetActualCPUUsage();
        CPUUsage.UsagePercentage = (CPUUsage.CurrentUsage / CPUUsage.MaxUsage) * 100.0f;
        CPUUsage.AverageUsage = (CPUUsage.AverageUsage + CPUUsage.CurrentUsage) / 2.0f;
        CPUUsage.PeakUsage = FMath::Max(CPUUsage.PeakUsage, CPUUsage.CurrentUsage);
        CPUUsage.LastUpdateTime = CurrentTime;
        
        // 檢查是否有顯著變化
        if (FMath::Abs(CPUUsage.UsagePercentage - PreviousUsage) > 5.0f)
        {
            OnResourceUsageChanged.Broadcast(ESystemResourceType::CPU, CPUUsage.UsagePercentage);
        }
    }
    
    // 更新內存使用率
    if (ResourceUsageMap.Contains(ESystemResourceType::Memory))
    {
        FResourceUsage& MemoryUsage = ResourceUsageMap[ESystemResourceType::Memory];
        float PreviousUsage = MemoryUsage.CurrentUsage;
        
        // 獲取實際內存使用率
        MemoryUsage.CurrentUsage = GetActualMemoryUsage();
        MemoryUsage.UsagePercentage = (MemoryUsage.CurrentUsage / MemoryUsage.MaxUsage) * 100.0f;
        MemoryUsage.AverageUsage = (MemoryUsage.AverageUsage + MemoryUsage.CurrentUsage) / 2.0f;
        MemoryUsage.PeakUsage = FMath::Max(MemoryUsage.PeakUsage, MemoryUsage.CurrentUsage);
        MemoryUsage.LastUpdateTime = CurrentTime;
        
        if (FMath::Abs(MemoryUsage.UsagePercentage - PreviousUsage) > 5.0f)
        {
            OnResourceUsageChanged.Broadcast(ESystemResourceType::Memory, MemoryUsage.UsagePercentage);
        }
    }
    
    // 更新GPU使用率
    if (ResourceUsageMap.Contains(ESystemResourceType::GPU))
    {
        FResourceUsage& GPUUsage = ResourceUsageMap[ESystemResourceType::GPU];
        float PreviousUsage = GPUUsage.CurrentUsage;
        
        // 獲取實際GPU使用率
        GPUUsage.CurrentUsage = GetActualGPUUsage();
        GPUUsage.UsagePercentage = (GPUUsage.CurrentUsage / GPUUsage.MaxUsage) * 100.0f;
        GPUUsage.AverageUsage = (GPUUsage.AverageUsage + GPUUsage.CurrentUsage) / 2.0f;
        GPUUsage.PeakUsage = FMath::Max(GPUUsage.PeakUsage, GPUUsage.CurrentUsage);
        GPUUsage.LastUpdateTime = CurrentTime;
        
        if (FMath::Abs(GPUUsage.UsagePercentage - PreviousUsage) > 5.0f)
        {
            OnResourceUsageChanged.Broadcast(ESystemResourceType::GPU, GPUUsage.UsagePercentage);
        }
    }
    
    // 更新其他資源類型...
    UpdateOtherResourceUsage();
    
    // 檢查資源閾值
    CheckResourceThresholds();
}

float USystemResourceManager::GetActualCPUUsage() const
{
    // 在實際實現中，這裡應該獲取真實的CPU使用率
    // 這裡使用模擬數據
    static float SimulatedCPU = 30.0f;
    SimulatedCPU += FMath::FRandRange(-5.0f, 5.0f);
    SimulatedCPU = FMath::Clamp(SimulatedCPU, 0.0f, 100.0f);
    return SimulatedCPU;
}

float USystemResourceManager::GetActualMemoryUsage() const
{
    // 在實際實現中，這裡應該獲取真實的內存使用率
    static float SimulatedMemory = 45.0f;
    SimulatedMemory += FMath::FRandRange(-3.0f, 3.0f);
    SimulatedMemory = FMath::Clamp(SimulatedMemory, 0.0f, 100.0f);
    return SimulatedMemory;
}

float USystemResourceManager::GetActualGPUUsage() const
{
    // 在實際實現中，這裡應該獲取真實的GPU使用率
    static float SimulatedGPU = 60.0f;
    SimulatedGPU += FMath::FRandRange(-8.0f, 8.0f);
    SimulatedGPU = FMath::Clamp(SimulatedGPU, 0.0f, 100.0f);
    return SimulatedGPU;
}

void USystemResourceManager::UpdateOtherResourceUsage()
{
    // 更新網絡、磁盤等其他資源使用率
    // 這裡使用簡化的實現
    for (auto& ResourcePair : ResourceUsageMap)
    {
        ESystemResourceType ResourceType = ResourcePair.Key;
        FResourceUsage& Usage = ResourcePair.Value;
        
        if (ResourceType == ESystemResourceType::CPU || 
            ResourceType == ESystemResourceType::Memory || 
            ResourceType == ESystemResourceType::GPU)
        {
            continue; // 已經處理過
        }
        
        // 模擬其他資源使用率
        float PreviousUsage = Usage.CurrentUsage;
        Usage.CurrentUsage += FMath::FRandRange(-2.0f, 2.0f);
        Usage.CurrentUsage = FMath::Clamp(Usage.CurrentUsage, 0.0f, Usage.MaxUsage);
        Usage.UsagePercentage = (Usage.CurrentUsage / Usage.MaxUsage) * 100.0f;
        Usage.AverageUsage = (Usage.AverageUsage + Usage.CurrentUsage) / 2.0f;
        Usage.PeakUsage = FMath::Max(Usage.PeakUsage, Usage.CurrentUsage);
        Usage.LastUpdateTime = FDateTime::Now();
        
        if (FMath::Abs(Usage.UsagePercentage - PreviousUsage) > 5.0f)
        {
            OnResourceUsageChanged.Broadcast(ResourceType, Usage.UsagePercentage);
        }
    }
}

void USystemResourceManager::CheckResourceThresholds()
{
    for (const auto& ResourcePair : ResourceUsageMap)
    {
        ESystemResourceType ResourceType = ResourcePair.Key;
        const FResourceUsage& Usage = ResourcePair.Value;
        
        if (ResourceThresholds.Contains(ResourceType))
        {
            float Threshold = ResourceThresholds[ResourceType];
            if (Usage.UsagePercentage > Threshold)
            {
                UE_LOG(LogTemp, Warning, TEXT("Resource threshold exceeded: %s usage %.1f%% > %.1f%%"), 
                    *UEnum::GetDisplayValueAsText(ResourceType).ToString(), 
                    Usage.UsagePercentage, Threshold);
                
                OnResourceThresholdExceeded.Broadcast(ResourceType, Usage.UsagePercentage);
            }
        }
    }
}

FResourceUsage USystemResourceManager::GetResourceUsage(ESystemResourceType ResourceType) const
{
    if (ResourceUsageMap.Contains(ResourceType))
    {
        return ResourceUsageMap[ResourceType];
    }
    return FResourceUsage();
}

TArray<FResourceUsage> USystemResourceManager::GetAllResourceUsage() const
{
    TArray<FResourceUsage> AllUsage;
    
    for (const auto& ResourcePair : ResourceUsageMap)
    {
        AllUsage.Add(ResourcePair.Value);
    }
    
    return AllUsage;
}

bool USystemResourceManager::RequestResourceAllocation(const FString& SystemName, ESystemResourceType ResourceType, float Amount, EResourcePriority Priority)
{
    // 檢查資源是否可用
    if (!IsResourceAvailable(ResourceType, Amount))
    {
        UE_LOG(LogTemp, Warning, TEXT("Resource allocation failed: %s not available for %s"), 
            *UEnum::GetDisplayValueAsText(ResourceType).ToString(), *SystemName);
        return false;
    }
    
    // 創建資源分配
    FResourceAllocation Allocation;
    Allocation.SystemName = SystemName;
    Allocation.ResourceType = ResourceType;
    Allocation.RequestedAmount = Amount;
    Allocation.Priority = Priority;
    Allocation.AllocatedAmount = Amount;
    Allocation.bIsAllocated = true;
    Allocation.AllocationTime = FDateTime::Now();
    
    // 添加到分配列表
    ResourceAllocations.Add(Allocation);
    
    UE_LOG(LogTemp, Log, TEXT("Resource allocated: %.2f of %s to %s"), 
        Amount, *UEnum::GetDisplayValueAsText(ResourceType).ToString(), *SystemName);
    
    return true;
}

void USystemResourceManager::ReleaseResourceAllocation(const FString& SystemName, ESystemResourceType ResourceType)
{
    for (int32 i = ResourceAllocations.Num() - 1; i >= 0; --i)
    {
        const FResourceAllocation& Allocation = ResourceAllocations[i];
        if (Allocation.SystemName == SystemName && Allocation.ResourceType == ResourceType)
        {
            ResourceAllocations.RemoveAt(i);
            UE_LOG(LogTemp, Log, TEXT("Resource released: %s from %s"), 
                *UEnum::GetDisplayValueAsText(ResourceType).ToString(), *SystemName);
            break;
        }
    }
}

bool USystemResourceManager::IsResourceAvailable(ESystemResourceType ResourceType, float RequiredAmount) const
{
    if (!ResourceUsageMap.Contains(ResourceType))
    {
        return false;
    }
    
    const FResourceUsage& Usage = ResourceUsageMap[ResourceType];
    float AvailableAmount = Usage.MaxUsage - Usage.CurrentUsage;
    
    // 檢查是否有足夠的可用資源
    if (AvailableAmount >= RequiredAmount)
    {
        return true;
    }
    
    // 檢查是否可以釋放低優先級的分配
    float ReleasableAmount = 0.0f;
    for (const FResourceAllocation& Allocation : ResourceAllocations)
    {
        if (Allocation.ResourceType == ResourceType && 
            Allocation.Priority <= EResourcePriority::Low)
        {
            ReleasableAmount += Allocation.AllocatedAmount;
        }
    }
    
    return (AvailableAmount + ReleasableAmount) >= RequiredAmount;
}

void USystemResourceManager::SetPerformanceConfiguration(const FPerformanceConfiguration& Config)
{
    EPerformanceTier OldTier = CurrentPerformanceConfig.PerformanceTier;
    CurrentPerformanceConfig = Config;
    
    // 更新資源閾值
    ResourceThresholds = Config.ResourceThresholds;
    
    UE_LOG(LogTemp, Log, TEXT("Performance configuration changed to: %s"), *Config.ConfigurationName);
    
    // 觸發性能配置變化事件
    if (OldTier != Config.PerformanceTier)
    {
        OnPerformanceTierChanged.Broadcast(Config.PerformanceTier);
    }
}

FPerformanceConfiguration USystemResourceManager::GetCurrentPerformanceConfiguration() const
{
    return CurrentPerformanceConfig;
}

void USystemResourceManager::AutoAdjustPerformance()
{
    if (!bAdaptivePerformanceEnabled)
    {
        return;
    }
    
    // 計算當前性能評分
    float CurrentScore = CalculatePerformanceScore();
    
    // 根據性能評分調整配置
    EPerformanceTier NewTier = CurrentPerformanceConfig.PerformanceTier;
    
    if (CurrentScore < 30.0f)
    {
        NewTier = EPerformanceTier::Low;
    }
    else if (CurrentScore < 50.0f)
    {
        NewTier = EPerformanceTier::Medium;
    }
    else if (CurrentScore < 70.0f)
    {
        NewTier = EPerformanceTier::High;
    }
    else if (CurrentScore < 85.0f)
    {
        NewTier = EPerformanceTier::Ultra;
    }
    else
    {
        NewTier = EPerformanceTier::Dynamic;
    }
    
    if (NewTier != CurrentPerformanceConfig.PerformanceTier)
    {
        FPerformanceConfiguration NewConfig = GetRecommendedPerformanceConfiguration();
        NewConfig.PerformanceTier = NewTier;
        SetPerformanceConfiguration(NewConfig);
    }
}

FPerformanceConfiguration USystemResourceManager::GetRecommendedPerformanceConfiguration() const
{
    FPerformanceConfiguration RecommendedConfig;
    RecommendedConfig.bEnableAdaptivePerformance = true;
    
    // 根據當前資源使用情況推薦配置
    float AverageUsage = 0.0f;
    int32 ResourceCount = 0;
    
    for (const auto& ResourcePair : ResourceUsageMap)
    {
        AverageUsage += ResourcePair.Value.UsagePercentage;
        ResourceCount++;
    }
    
    if (ResourceCount > 0)
    {
        AverageUsage /= ResourceCount;
    }
    
    if (AverageUsage > 80.0f)
    {
        RecommendedConfig.PerformanceTier = EPerformanceTier::Low;
        RecommendedConfig.TargetFrameRate = 30.0f;
        RecommendedConfig.MaxConcurrentSystems = 20;
        RecommendedConfig.ConfigurationName = TEXT("Low Performance");
    }
    else if (AverageUsage > 60.0f)
    {
        RecommendedConfig.PerformanceTier = EPerformanceTier::Medium;
        RecommendedConfig.TargetFrameRate = 45.0f;
        RecommendedConfig.MaxConcurrentSystems = 35;
        RecommendedConfig.ConfigurationName = TEXT("Medium Performance");
    }
    else if (AverageUsage > 40.0f)
    {
        RecommendedConfig.PerformanceTier = EPerformanceTier::High;
        RecommendedConfig.TargetFrameRate = 60.0f;
        RecommendedConfig.MaxConcurrentSystems = 50;
        RecommendedConfig.ConfigurationName = TEXT("High Performance");
    }
    else
    {
        RecommendedConfig.PerformanceTier = EPerformanceTier::Ultra;
        RecommendedConfig.TargetFrameRate = 120.0f;
        RecommendedConfig.MaxConcurrentSystems = 75;
        RecommendedConfig.ConfigurationName = TEXT("Ultra Performance");
    }
    
    // 設置資源限制
    RecommendedConfig.ResourceLimits = CurrentPerformanceConfig.ResourceLimits;
    RecommendedConfig.ResourceThresholds = CurrentPerformanceConfig.ResourceThresholds;
    
    return RecommendedConfig;
}

void USystemResourceManager::SetResourceThreshold(ESystemResourceType ResourceType, float Threshold)
{
    ResourceThresholds[ResourceType] = Threshold;
    CurrentPerformanceConfig.ResourceThresholds[ResourceType] = Threshold;
    
    UE_LOG(LogTemp, Log, TEXT("Resource threshold set: %s = %.1f%%"), 
        *UEnum::GetDisplayValueAsText(ResourceType).ToString(), Threshold);
}

float USystemResourceManager::GetResourceThreshold(ESystemResourceType ResourceType) const
{
    if (ResourceThresholds.Contains(ResourceType))
    {
        return ResourceThresholds[ResourceType];
    }
    return 80.0f; // 默認閾值
}

float USystemResourceManager::GetSystemPerformanceScore() const
{
    return SystemPerformanceScore;
}

float USystemResourceManager::CalculatePerformanceScore() const
{
    float TotalScore = 0.0f;
    int32 ResourceCount = 0;
    
    for (const auto& ResourcePair : ResourceUsageMap)
    {
        const FResourceUsage& Usage = ResourcePair.Value;
        
        // 計算每種資源的評分（使用率越低評分越高）
        float ResourceScore = FMath::Max(0.0f, 100.0f - Usage.UsagePercentage);
        TotalScore += ResourceScore;
        ResourceCount++;
    }
    
    if (ResourceCount > 0)
    {
        TotalScore /= ResourceCount;
    }
    
    return TotalScore;
}

TArray<FString> USystemResourceManager::GetPerformanceRecommendations() const
{
    TArray<FString> Recommendations;
    
    // 分析資源使用情況並提供建議
    for (const auto& ResourcePair : ResourceUsageMap)
    {
        ESystemResourceType ResourceType = ResourcePair.Key;
        const FResourceUsage& Usage = ResourcePair.Value;
        
        if (Usage.UsagePercentage > 85.0f)
        {
            Recommendations.Add(FString::Printf(TEXT("嚴重：%s使用率過高 (%.1f%%)，建議立即優化"), 
                *UEnum::GetDisplayValueAsText(ResourceType).ToString(), Usage.UsagePercentage));
        }
        else if (Usage.UsagePercentage > 70.0f)
        {
            Recommendations.Add(FString::Printf(TEXT("警告：%s使用率較高 (%.1f%%)，建議優化"), 
                *UEnum::GetDisplayValueAsText(ResourceType).ToString(), Usage.UsagePercentage));
        }
    }
    
    // 性能配置建議
    if (SystemPerformanceScore < 50.0f)
    {
        Recommendations.Add(TEXT("建議切換到低性能配置以穩定系統運行"));
    }
    else if (SystemPerformanceScore > 80.0f)
    {
        Recommendations.Add(TEXT("系統性能良好，可以考慮提升到更高性能配置"));
    }
    
    // 資源分配建議
    int32 HighPriorityAllocations = 0;
    for (const FResourceAllocation& Allocation : ResourceAllocations)
    {
        if (Allocation.Priority == EResourcePriority::High || Allocation.Priority == EResourcePriority::Critical)
        {
            HighPriorityAllocations++;
        }
    }
    
    if (HighPriorityAllocations > 10)
    {
        Recommendations.Add(TEXT("高優先級資源分配過多，建議重新評估優先級"));
    }
    
    if (Recommendations.Num() == 0)
    {
        Recommendations.Add(TEXT("系統性能良好，無需特別優化"));
    }
    
    return Recommendations;
}

FString USystemResourceManager::GeneratePerformanceReport() const
{
    FString Report;
    Report += TEXT("=== 系統性能報告 ===\n\n");
    
    // 基本信息
    Report += FString::Printf(TEXT("報告生成時間: %s\n"), *FDateTime::Now().ToString());
    Report += FString::Printf(TEXT("當前性能配置: %s\n"), *CurrentPerformanceConfig.ConfigurationName);
    Report += FString::Printf(TEXT("性能評分: %.1f/100\n"), SystemPerformanceScore);
    Report += FString::Printf(TEXT("目標幀率: %.0f FPS\n"), CurrentPerformanceConfig.TargetFrameRate);
    Report += FString::Printf(TEXT("最大並發系統: %d\n\n"), CurrentPerformanceConfig.MaxConcurrentSystems);
    
    // 資源使用情況
    Report += TEXT("=== 資源使用情況 ===\n");
    for (const auto& ResourcePair : ResourceUsageMap)
    {
        const FResourceUsage& Usage = ResourcePair.Value;
        Report += FString::Printf(TEXT("%s: %.1f%% (平均: %.1f%%, 峰值: %.1f%%)\n"),
            *UEnum::GetDisplayValueAsText(Usage.ResourceType).ToString(),
            Usage.UsagePercentage,
            Usage.AverageUsage,
            Usage.PeakUsage);
    }
    
    // 資源分配情況
    Report += TEXT("\n=== 資源分配情況 ===\n");
    TMap<ESystemResourceType, int32> AllocationCounts;
    for (const FResourceAllocation& Allocation : ResourceAllocations)
    {
        AllocationCounts.FindOrAdd(Allocation.ResourceType)++;
    }
    
    for (const auto& CountPair : AllocationCounts)
    {
        Report += FString::Printf(TEXT("%s: %d個分配\n"),
            *UEnum::GetDisplayValueAsText(CountPair.Key).ToString(),
            CountPair.Value);
    }
    
    // 資源閾值
    Report += TEXT("\n=== 資源閾值 ===\n");
    for (const auto& ThresholdPair : ResourceThresholds)
    {
        Report += FString::Printf(TEXT("%s: %.1f%%\n"),
            *UEnum::GetDisplayValueAsText(ThresholdPair.Key).ToString(),
            ThresholdPair.Value);
    }
    
    // 性能建議
    Report += TEXT("\n=== 性能建議 ===\n");
    TArray<FString> Recommendations = GetPerformanceRecommendations();
    for (int32 i = 0; i < Recommendations.Num(); ++i)
    {
        Report += FString::Printf(TEXT("%d. %s\n"), i + 1, *Recommendations[i]);
    }
    
    return Report;
}

void USystemResourceManager::SetAdaptivePerformanceEnabled(bool bEnabled)
{
    bAdaptivePerformanceEnabled = bEnabled;
    CurrentPerformanceConfig.bEnableAdaptivePerformance = bEnabled;
    
    UE_LOG(LogTemp, Log, TEXT("Adaptive performance %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}
