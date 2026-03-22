#include "MingCoreModule.h"
#include "MingCoreEventBus.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"

#define LOCTEXT_NAMESPACE "FMingCoreModule"

// 性能監控類
class FPerformanceMonitor
{
public:
    FPerformanceMonitor()
    {
        // 初始化性能計數器
        EventProcessingTime = 0.0;
        EventCount = 0;
        LastUpdateTime = FDateTime::Now();
    }
    
    void RecordEventProcessed(double ProcessingTimeMs)
    {
        EventProcessingTime += ProcessingTimeMs;
        EventCount++;
        
        // 每秒報告一次性能統計
        FDateTime Now = FDateTime::Now();
        if ((Now - LastUpdateTime).GetTotalSeconds() >= 1.0)
        {
            ReportPerformanceStats();
            EventProcessingTime = 0.0;
            EventCount = 0;
            LastUpdateTime = Now;
        }
    }
    
private:
    void ReportPerformanceStats()
    {
        if (EventCount > 0)
        {
            double AverageTime = EventProcessingTime / EventCount;
            UE_LOG(LogTemp, Log, TEXT("MingCore Performance: %d events processed, avg %.3fms per event"), 
                   EventCount, AverageTime);
            
            // 性能警告
            if (AverageTime > 1.0) // 超過1ms警告
            {
                UE_LOG(LogTemp, Warning, TEXT("MingCore Performance Warning: Average event processing time is high (%.3fms)"), AverageTime);
            }
        }
    }
    
    double EventProcessingTime;
    int32 EventCount;
    FDateTime LastUpdateTime;
};

void FMingCoreModule::StartupModule()
{
    // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
    
    UE_LOG(LogTemp, Log, TEXT("MingCore Module Starting Up"));
    
    // 初始化性能監控
    PerformanceMonitor = new FPerformanceMonitor();
    
    // 註冊事件類型
    RegisterEventTypes();
    
    // 設置性能監控
    SetupPerformanceMonitoring();
    
    UE_LOG(LogTemp, Log, TEXT("MingCore Module Startup Complete - Event Bus Ready"));
}

void FMingCoreModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
    
    UE_LOG(LogTemp, Log, TEXT("MingCore Module Shutting Down"));
    
    // 清理性能監控
    if (PerformanceMonitor)
    {
        delete PerformanceMonitor;
        PerformanceMonitor = nullptr;
    }
    
    UE_LOG(LogTemp, Log, TEXT("MingCore Module Shutdown Complete"));
}

void FMingCoreModule::RegisterEventTypes()
{
    // 註冊所有事件類型到事件總線
    // 這確保事件系統知道所有可用的事件類型
    
    UE_LOG(LogTemp, Log, TEXT("Registering MingCore Event Types"));
    
    // 註冊核心事件類型
    // 注意：UE5的反射系統會自動註冊USTRUCT類型
    // 這裡主要是為了驗證和初始化
    
    // 驗證事件類型是否正確註冊
    static_assert(sizeof(FUnitSelectedEvent) > 0, "FUnitSelectedEvent size invalid");
    static_assert(sizeof(FUnitMovedEvent) > 0, "FUnitMovedEvent size invalid");
    static_assert(sizeof(FResourceUpdateEvent) > 0, "FResourceUpdateEvent size invalid");
    
    UE_LOG(LogTemp, Log, TEXT("Event Types Registration Complete"));
}

void FMingCoreModule::SetupPerformanceMonitoring()
{
    // 設置事件系統性能監控
    UE_LOG(LogTemp, Log, TEXT("Setting up MingCore Performance Monitoring"));
    
    // 這裡可以添加更多性能監控設置
    // 例如：事件隊列大小監控、記憶體使用監控等
    
    UE_LOG(LogTemp, Log, TEXT("Performance Monitoring Setup Complete"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMingCoreModule, MingCore)
