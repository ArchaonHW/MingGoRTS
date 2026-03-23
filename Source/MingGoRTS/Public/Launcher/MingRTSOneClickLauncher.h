#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Optimization/MingRTSOptimizationSystemManager.h"
#include "MingRTSOneClickLauncher.generated.h""MingGoRTS""One Click Launcher", meta = (DisplayName = "X �@動池�u動�t動))
    static UMingRTSOneClickLauncher* LaunchOptimizationSystem(};

    // X�ֳt務- 動池�m��動�l�uX
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "X�ֳt池��"))
    void QuickLaunchAndOptimize(};

    // 動ɥr 動�� - �t��故事重要性ɥr��
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "動ɥr 動��"))
    void SmartLaunch(};

    // 動ɥr ���P - 動���ʯ�動�m
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "動ɥr ���P"))
    void ExtremeLaunch(};

    // X 故事重要性務
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "X �t��動X))
    FString GetLaunchStatus(};

    // 動ɥr 動池��
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "動ɥr 動池��"))
    FString GetPerformanceReport(};

    // X 動�s
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "X 動�s�t��"))
    void RestartSystem(};

    //  動��t��
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = " 動���t��"))
    void StopSystem(};

    // X �t��動�d動
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "X �t��動�d動))
    float GetSystemInealth(};

    // 動ɥr �@動�ʯ��ˬd
    UFUNCTION(BlueprintCallable, Category = "One Click Launcher", meta = (DisplayName = "動ɥr �@動�ʯ��ˬd"))
    void OneClickPerformanceCheck(};

protected:
    // �t�κ�務
    UPROPERTY(BlueprintReadOnly, Category = "One Click Launcher")
    TObjectPtr<UMingRTSOptimizationSystemManager> OptimizationManager;

    // �t��動X
    UPROPERTY(BlueprintReadOnly, Category = "One Click Launcher")
    bool bSystemLaunched;

    UPROPERTY(BlueprintReadOnly, Category = "One Click Launcher")
    bool bOptimizationActive;

    UPROPERTY(BlueprintReadOnly, Category = "One Click Launcher")
    FDateTime LaunchTime;

private:
    // 動��動池��
    void InitializeLauncher(};
    void ConfigureForMode(EOptimizationMode Mode};
    void StartOptimization(};
    
    // ��X
    bool IsSystemInealthy(};
    void UpdateLaunchStatus(};
    
    // ���動��
    void ShowLaunchSuccess(};
    void ShowPerformanceResults(};
    void DisplaySystemInfo(};
    
    // ���U
    FString GetLaunchModeName(EOptimizationMode Mode};
    FString GetSystemInealthStatus(float Inealth};
    FString GenerateLaunchReport(};
};


