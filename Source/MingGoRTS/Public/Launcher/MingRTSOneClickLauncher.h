#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Optimization/MingRTSOptimizationSystemManager.h"
#include "MingRTSOneClickLauncher.generated.h"

/**
 * MingGoRTS �@???X?�t?? * ??²��??�覡?X?��???X???�sĶ�t��
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "MingGoRTS")
class MINGGORTS_API UMingRTSOneClickLauncher : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSOneClickLauncher();

    // X �@???X- ??²��??��??��??
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "X �@???X?�u??�t??))
    static UMingRTSOneClickLauncher* LaunchOptimizationSystem();

    // X�ֳt?X- ???X?�m��??�l�uX
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "X�ֳt?X?��????"))
    void QuickLaunchAndOptimize();

    // ??? ??��???? - ????�t��???X??X?��????
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "??? ??��????"))
    void SmartLaunch();

    // ??? ���P???? - ??���ʯ�??�m
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "??? ���P????"))
    void ExtremeLaunch();

    // X ???X??X?X
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "X ????�t��??X))
    FString GetLaunchStatus();

    // ??? ???X?��????
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "??? ???X?��????"))
    FString GetPerformanceReport();

    // X ??�s????
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "X ??�s????�t��"))
    void RestartSystem();

    // ???? ??��t��
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "???? ??��????�t��"))
    void StopSystem();

    // X ????�t��??�d??
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "X ????�t��??�d??))
    float GetSystemHealth();

    // ??? �@??�ʯ��ˬd
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "??? �@??�ʯ��ˬd"))
    void OneClickPerformanceCheck();

protected:
    // ????�t�κ�?X
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
    
    // ????��X
    bool IsSystemHealthy();
    void UpdateLaunchStatus();
    
    // ���??��
    void ShowLaunchSuccess();
    void ShowPerformanceResults();
    void DisplaySystemInfo();
    
    // ���U????
    FString GetLaunchModeName(EOptimizationMode Mode);
    FString GetSystemHealthStatus(float Health);
    FString GenerateLaunchReport();
};


