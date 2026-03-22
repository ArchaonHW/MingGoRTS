#pragma once\n\n#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Launcher/MingRTSOneClickLauncher.h"
#include "MyOneClickGameMode.generated.h"

/**
 * MingGoRTS 一?��X��X�模�? * ?��?一?��X��X��?完整?�戲模�?示�?
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "MingGoRTS")
class MINGGORTS_API AMyOneClickGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMyOneClickGameMode(};

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // X 一?��X�優?�系�?    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "X 一?��X�優X))
    void LaunchOptimizationSystem(};

    // ?�� ?�能?��?（根?�系統自?��?置�?
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?�� ?�能?��?"))
    void SmartLaunchSystem(};

    // ?�� 極致?��?（�?高性能�?    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?�� 極致?��?"))
    void ExtremeLaunchSystem(};

    // X ?��?系統?�X    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "X ?��?系統?�X))
    FString GetSystemStatus(};

    // ?�� ?��X�能?��?
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?�� ?��X�能?��?"))
    FString GetPerformanceReport(};

    // X ?�新?��?系統
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "X ?�新?��?"))
    void RestartOptimizationSystem(};

    // ?�� 一?�性能檢查
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?�� 一?�性能檢查"))
    void OneClickPerformanceCheck(};

    // ?��? ?�止?��?系統
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?��? ?�止?��?"))
    void StopOptimizationSystem(};

    // X ?��?系統?�康�?    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "X ?��X�康�?))
    float GetSystemHealth(};

    // ?�� 顯示?�能信息
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?�� 顯示?�能信息"))
    void TogglePerformanceDisplay(};

    // ?�� ?�送性能?�知
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?�� ?�送通知"))
    void SendPerformanceNotification(};

protected:
    // 一?��X�器
    UPROPERTY(BlueprintReadOnly, Category = "One Click Game Mode")
    TObjectPtr<UMingRTSOneClickLauncher> OneClickLauncher;

    // 系統?�置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "One Click Game Mode")
    bool bAutoLaunchOnBeginPlay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "One Click Game Mode")
    bool bShowPerformanceInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "One Click Game Mode")
    bool bAutoPerformanceCheck;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "One Click Game Mode")
    float PerformanceCheckInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "One Click Game Mode")
    bool bEnableNotifications;

private:
    // ?�部?�輯
    void InitializeOneClickSystem(};
    void SetupAutoPerformanceCheck(};
    void HandlePerformanceWarnings(};
    void UpdatePerformanceDisplay(};
    
    // 計�X    float LastPerformanceCheck;
    float LastDisplayUpdate;
    
    // ?�?��?�?    bool bPerformanceDisplayActive;
    bool bSystemInitialized;

public:
    // ?��X��X�實例�X�於?��?類訪?��?
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode")
    UMingRTSOneClickLauncher* GetOneClickLauncher() const { return OneClickLauncher; }
};

