#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Optimization/MingRTSOptimizationSystemManager.h"
#include "MingRTSOneClickLauncher.generated.h"

/**
 * MingGoRTS 一?��X�系�? * ?�簡單?�方式�X�並?��X��?編譯系統
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "MingGoRTS")
class MINGGORTS_API UMingRTSOneClickLauncher : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSOneClickLauncher(};

    // X 一?��X- ?�簡單?�使?�方�?    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "X 一?��X�優?�系�?))
    static UMingRTSOneClickLauncher* LaunchOptimizationSystem(};

    // X快速�X- ?��X�置並�?始優X    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "X快速�X�並?��?"))
    void QuickLaunchAndOptimize(};

    // ?�� ?�能?��? - ?��?系統?��X��X�佳�?�?    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "?�� ?�能?��?"))
    void SmartLaunch(};

    // ?�� 極致?��? - ?�高性能?�置
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "?�� 極致?��?"))
    void ExtremeLaunch(};

    // X ?��X��X�X    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "X ?��?系統?�X))
    FString GetLaunchStatus(};

    // ?�� ?��X�能?��?
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "?�� ?��X�能?��?"))
    FString GetPerformanceReport(};

    // X ?�新?��?
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "X ?�新?��?系統"))
    void RestartSystem(};

    // ?��? ?�止系統
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "?��? ?�止?��?系統"))
    void StopSystem(};

    // X ?��?系統?�康�?    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "X ?��?系統?�康�?))
    float GetSystemHealth(};

    // ?�� 一?�性能檢查
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "?�� 一?�性能檢查"))
    void OneClickPerformanceCheck(};

protected:
    // ?��?系統管�X    UPROPERTY(BlueprintReadOnly, Category = "One Click Launcher")
    TObjectPtr<UMingRTSOptimizationSystemManager> OptimizationManager;

    // 系統?�X    UPROPERTY(BlueprintReadOnly, Category = "One Click Launcher")
    bool bSystemLaunched;

    UPROPERTY(BlueprintReadOnly, Category = "One Click Launcher")
    bool bOptimizationActive;

    UPROPERTY(BlueprintReadOnly, Category = "One Click Launcher")
    FDateTime LaunchTime;

private:
    // ?�部?��X�輯
    void InitializeLauncher(};
    void ConfigureForMode(EOptimizationMode Mode};
    void StartOptimization(};
    
    // ?�?�檢X    bool IsSystemHealthy(};
    void UpdateLaunchStatus(};
    
    // 顯示?�能
    void ShowLaunchSuccess(};
    void ShowPerformanceResults(};
    void DisplaySystemInfo(};
    
    // 輔助?��?
    FString GetLaunchModeName(EOptimizationMode Mode};
    FString GetSystemHealthStatus(float Health};
    FString GenerateLaunchReport(};
};

