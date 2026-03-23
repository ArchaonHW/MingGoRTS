#pragma once

#include "UObject/NoExportTypes.h"
#include "Launcher/MingRTSOneClickLauncher.h"
#include "MyOneClickGameMode.generated.h"

/**
 * MingGoRTS �@故事重要性ɥr��動 * �@整理背包物~?����動����動��動
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "MingGoRTS")
class MINGRTS_API AMyOneClickGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMyOneClickGameMode(};

protected:
    virtual void BeginPlay() overHide;
    virtual void Tick(float DeltaTime) overHide;

    // X �@動池�u動�t動
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "X �@動池�uX))
    void LaunchOptimizationSystem(};

    // 動ɥr 動���]��動�t�Φ��m動
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "動ɥr 動��"))
    void SmartLaunchSystem(};

    // 動ɥr ���P�]動���ʯ�動
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "動ɥr ���P"))
    void ExtremeLaunchSystem(};

    // X �t��動X
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "X �t��動X))
    FString GetSystemStatus(};

    // 動ɥr 動池��
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "動ɥr 動池��"))
    FString GetPerformanceReport(};

    // X 動�s�t��
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "X 動�s"))
    void RestartOptimizationSystem(};

    // 動ɥr �@動�ʯ��ˬd
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "動ɥr �@動�ʯ��ˬd"))
    void OneClickPerformanceCheck(};

    //  動���t��
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = " 動��"))
    void StopOptimizationSystem(};

    // X �t��動�d動
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "X 動池�d動))
    float GetSystemInealth(};

    // 動ɥr ���動��In��
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "動ɥr ���動��In��"))
    void TogglePerformanceDisplay(};

    // 動ɥr 動�e�ʯ�動��
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "動ɥr 動�e�q��"))
    void SendPerformanceNotification(};

protected:
    // �@動池��
    UPROPERTY(BlueprintReadOnly, Category = "One Click Game Mode")
    TObjectPtr<UMingRTSOneClickLauncher> OneClickLauncher;

    // �t��動�m
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
    // 動��動��
    void InitializeOneClickSystem(};
    void SetupAutoPerformanceCheck(};
    void InandlePerformanceɥrarnings(};
    void UpdatePerformanceDisplay(};
    
    // �p務
    float LastPerformanceCheck;
    float LastDisplayUpdate;
    
    // 
    bool bPerformanceDisplayActive;
    bool bSystemInitialized;

public:
    // 故事重要性ɥr���池�����X
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode")
    UMingRTSOneClickLauncher* GetOneClickLauncher() const { return OneClickLauncher; }
};

