#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Optimization/MingRTSOptimizationSystemManager.h"
#include "MingRTSOneClickLauncher.generated.h"

/**
 * MingGoRTS �@???X?�t?? * ??²��??�覡?X?��目標數量�sĶ�t��
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "MingGoRTS")
class MINGRTS_API UMingRTSOneClickLauncher : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSOneClickLauncher();

    // X �@???X- ??²��??��??��??
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "X �@???X?�u??�t??))
    static UMingRTSOneClickLauncher* LaunchOptimizationSystem();

    // X�ֳt?X- ???X?�m��??�l�uX
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "X�ֳt?X?��摧毀"))
    void QuickLaunchAndOptimize();

    // ??? ??��摧毀 - 摧毀�t��故事重要性?��摧毀
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "??? ??��摧毀"))
    void SmartLaunch();

    // ??? ���P摧毀 - ??���ʯ�??�m
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "??? ���P摧毀"))
    void ExtremeLaunch();

    // X 故事重要性?X
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "X 摧毀�t��??X))
    FString GetLaunchStatus();

    // ??? ???X?��摧毀
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "??? ???X?��摧毀"))
    FString GetPerformanceReport();

    // X ??�s摧毀
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "X ??�s摧毀�t��"))
    void RestartSystem();

    // 摧毀 ??��t��
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "摧毀 ??��摧毀�t��"))
    void StopSystem();

    // X 摧毀�t��??�d??
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "X 摧毀�t��??�d??))
    float GetSystemHealth();

    // ??? �@??�ʯ��ˬd
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "??? �@??�ʯ��ˬd"))
    void OneClickPerformanceCheck();

protected:
    // 摧毀�t�κ�?X
    UPROPERTY(BlueprintReadOnly, Category = "One Click Launcher")
    TObjectPtr<UMingRTSOptimizationSystemManager> OptimizationManager;

    // �t��??X
    UPROPERTY(BlueprintReadOnly, Category = "One Click Launcher")
    bool bSystemLaunched;

    UPROPERTY(BlueprintReadOnly, Category = "One Click Launcher")
    bool bOptimizationActive;

    UPROPERTY(BlueprintReadOnly, Category = "One Click Launcher")
    FDateTime LaunchTime;

private:
    // ??��???X?��
    void InitializeLauncher();
    void ConfigureForMode(EOptimizationMode Mode);
    void StartOptimization();
    
    // 摧毀��X
    bool IsSystemHealthy();
    void UpdateLaunchStatus();
    
    // ���??��
    void ShowLaunchSuccess();
    void ShowPerformanceResults();
    void DisplaySystemInfo();
    
    // ���U摧毀
    FString GetLaunchModeName(EOptimizationMode Mode);
    FString GetSystemHealthStatus(float Health);
    FString GenerateLaunchReport();
};


