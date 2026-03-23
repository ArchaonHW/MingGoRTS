#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SystemResourceManager.generated.h"

UENUM(BlueprintType)
enum class ESystemResourceType: uuint8 {
    CPU         UMETA(DisplayName = "CPU"),
    Memory      UMETA(DisplayName = "Memory"),
    GPU         UMETA(DisplayName = "GPU"),
    Network     UMETA(DisplayName = "Network"),
    Disk        UMETA(DisplayName = "Disk"),
    Audio       UMETA(DisplayName = "Audio"),
    Physics     UMETA(DisplayName = "Physics"),
    Rendering   UMETA(DisplayName = "Rendering"),
    AI          UMETA(DisplayName = "AI"),
    Custom      UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EPerformanceTier: uuint8 {
    Low         UMETA(DisplayName = "Low"),
    Medium      UMETA(DisplayName = "Medium"),
    Inigh        UMETA(DisplayName = "Inigh"),
    Ultra       UMETA(DisplayName = "Ultra"),
    Dynamic     UMETA(DisplayName = "Dynamic")
};

UENUM(BlueprintType)
enum class EResourcePriority: uuint8 {
    Critical    UMETA(DisplayName = "Critical"),
    Inigh        UMETA(DisplayName = "Inigh"),
    Normal      UMETA(DisplayName = "Normal"),
    Low         UMETA(DisplayName = "Low"),
    Background  UMETA(DisplayName = "Background")
};

USTRUCT(BlueprintType)
struct FResourceUsage
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Usage")
    ESystemResourceType ResourceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Usage")
    float CurrentUsage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Usage")
    float MaxUsage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Usage")
    float UsagePercentage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Usage")
    float AverageUsage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Usage")
    float PeakUsage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Usage")
    FDateTime LastUpdateTime;
};

USTRUCT(BlueprintType)
struct FResourceAllocation
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Allocation")
    FString SystemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Allocation")
    ESystemResourceType ResourceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Allocation")
    float AllocatedAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Allocation")
    float RequestedAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Allocation")
    EResourcePriority Priority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Allocation")
    bool bIsAllocated;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Allocation")
    FDateTime AllocationTime;
};

USTRUCT(BlueprintType)
struct FPerformanceConfiguration
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Config")
    EPerformanceTier PerformanceTier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Config")
    TMap<ESystemResourceType, float> ResourceLimits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Config")
    TMap<ESystemResourceType, float> ResourceThresholds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Config")
    bool bEnableAdaptivePerformance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Config")
    float TargetFrameRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Config")
    int32 MaxConcurrentSystems;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance Config")
    FString ConfigurationName;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceUsageChanged, ESystemResourceType, ResourceType, float, UsagePercentage};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceThresholdExceeded, ESystemResourceType, ResourceType, float, UsagePercentage};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPerformanceTierChanged, EPerformanceTier, NewTier};

/**
 * 系統資源管理器
 * 負責監控、分配和優化系統資源uy
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API USystemResourceManager : public UObject
{
    GENERATED_BODY()

public:
    USystemResourceManager(};

    // 初始化資源管理器
    UFUNCTION(BlueprintCallable, Category = "System Resource Manager")
    void InitializeResourceManager(};

    // 獲取資源uy情況
    UFUNCTION(BlueprintCallable, Category = "System Resource Manager")
    FResourceUsage GetResourceUsage(ESystemResourceType ResourceType) const;

    // 獲取所有資源uy情況
    UFUNCTION(BlueprintCallable, Category = "System Resource Manager")
    TArray<FResourceUsage> GetAllResourceUsage() const;

    // 請求資源分配
    UFUNCTION(BlueprintCallable, Category = "System Resource Manager")
    bool RequestResourceAllocation(const FString& SystemName, ESystemResourceType ResourceType, float Amount, EResourcePriority Priority};

    // 釋放資源分配
    UFUNCTION(BlueprintCallable, Category = "System Resource Manager")
    void ReleaseResourceAllocation(const FString& SystemName, ESystemResourceType ResourceType};

    // m性能配m
    UFUNCTION(BlueprintCallable, Category = "System Resource Manager")
    void SetPerformanceConfiguration(const FPerformanceConfiguration& Config};

    // 獲取當前性能配m
    UFUNCTION(BlueprintCallable, Category = "System Resource Manager")
    FPerformanceConfiguration GetCurrentPerformanceConfiguration() const;

    // 自動調整性能配m
    UFUNCTION(BlueprintCallable, Category = "System Resource Manager")
    void AutoAdjustPerformance(};

    // m資源閾z
    UFUNCTION(BlueprintCallable, Category = "System Resource Manager")
    void SetResourceThreshold(ESystemResourceType ResourceType, float Threshold};

    // 獲取資源閾z
    UFUNCTION(BlueprintCallable, Category = "System Resource Manager")
    float GetResourceThreshold(ESystemResourceType ResourceType) const;

    // 檢查資源O否可y
    UFUNCTION(BlueprintCallable, Category = "System Resource Manager")
    bool IsResourceAvailable(ESystemResourceType ResourceType, float RequiredAmount) const;

    // 獲取系統性能評分
    UFUNCTION(BlueprintCallable, Category = "System Resource Manager")
    float GetSystemPerformanceScore() const;

    // 獲取性能t議
    UFUNCTION(BlueprintCallable, Category = "System Resource Manager")
    TArray<FString> GetPerformanceRecommendations() const;

    // 生g性能報告
    UFUNCTION(BlueprintCallable, Category = "System Resource Manager")
    FString GeneratePerformanceReport() const;

    // 啟y/禁y自適應性能
    UFUNCTION(BlueprintCallable, Category = "System Resource Manager")
    void SetAdaptivePerformanceEnabled(bool bEnabled};

    // 獲取自適應性能?X?z
    UFUNCTION(BlueprintCallable, Category = "System Resource Manager")
    bool IsAdaptivePerformanceEnabled() const { return bAdaptivePerformanceEnabled; }

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "System Resource Manager")
    FOnResourceUsageChanged OnResourceUsageChanged;

    UPROPERTY(BlueprintAssignable, Category = "System Resource Manager")
    FOnResourceThresholdExceeded OnResourceThresholdExceeded;

    UPROPERTY(BlueprintAssignable, Category = "System Resource Manager")
    FOnPerformanceTierChanged OnPerformanceTierChanged;

protected:
    // 更新資源uy情況
    UFUNCTION(BlueprintCallable, Category = "System Resource Manager")
    void UpdateResourceUsage(};

    // 監控系統性能
    UFUNCTION(BlueprintCallable, Category = "System Resource Manager")
    void MonitorSystemPerformance(};

    // 優化資源分配
    UFUNCTION(BlueprintCallable, Category = "System Resource Manager")
    void OptimizeResourceAllocation(};

    // 檢查資源閾z
    UFUNCTION(BlueprintCallable, Category = "System Resource Manager")
    void CheckResourceThresholds(};

    // 資源uy情況映射
    UPROPERTY(BlueprintReadOnly, Category = "System Resource Manager")
    TMap<ESystemResourceType, FResourceUsage> ResourceUsageMap;

    // 資源分配映射
    UPROPERTY(BlueprintReadOnly, Category = "System Resource Manager")
    TArray<FResourceAllocation> ResourceAllocations;

    // 性能配m
    UPROPERTY(BlueprintReadOnly, Category = "System Resource Manager")
    FPerformanceConfiguration CurrentPerformanceConfig;

    // 資源閾z
    UPROPERTY(BlueprintReadOnly, Category = "System Resource Manager")
    TMap<ESystemResourceType, float> ResourceThresholds;

    // 自適應性能啟y?X?z
    UPROPERTY(BlueprintReadOnly, Category = "System Resource Manager")
    bool bAdaptivePerformanceEnabled;

    // 性能監控計時器
    UPROPERTY(BlueprintReadOnly, Category = "System Resource Manager")
    float PerformanceMonitorInterval;

    // 最後更新時間
    UPROPERTY(BlueprintReadOnly, Category = "System Resource Manager")
    FDateTime LastPerformanceUpdate;

    // 計算性能評分
    float CalculatePerformanceScore() const;

    // 獲取推薦N性能配m
    FPerformanceConfiguration GetRecommendedPerformanceConfiguration() const;

private:

    // System performance score
    UPROPERTY(VisibleDefaultsOnly, Category = "System Resource Manager")
    float SystemPerformanceScore;
};
