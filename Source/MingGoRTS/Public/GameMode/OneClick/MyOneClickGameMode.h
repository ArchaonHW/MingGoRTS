#pragma once

#include "UObject/NoExportTypes.h"
#include "Launcher/MingRTSOneClickLauncher.h"
#include "MyOneClickGameMode.generated.h"

/**
 * MingGoRTS ï¿½@?…ä??è??§É¥rï¿½ï¿½??* ï¿½@?´ç??Œå??©~?ï¿½ï¿½ï¿½ï¿½?•ï¿½ï¿½ï¿½ï¿½å?ï¿½ï¿½??
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "MingGoRTS")
class MINGRTS_API AMyOneClickGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMyOneClickGameMode(};

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // X ï¿½@?•æ?ï¿½u?•ï¿½t??
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "X ï¿½@?•æ?ï¿½uX))
    void LaunchOptimizationSystem(};

    // ?•É¥r ?•ï¿½ï¿½ï¿½]ï¿½ï¿½?•ï¿½tï¿½Î¦ï¿½ï¿½m??
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?•É¥r ?•ï¿½ï¿?))
    void SmartLaunchSystem(};

    // ?•É¥r ï¿½ï¿½ï¿½Pï¿½]?•ï¿½ï¿½ï¿½Ê¯ï¿½å?
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?•É¥r ï¿½ï¿½ï¿½P"))
    void ExtremeLaunchSystem(};

    // X ï¿½tï¿½ï¿½?•X
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "X ï¿½tï¿½ï¿½?•X))
    FString GetSystemStatus(};

    // ?•É¥r ?•æ?ï¿½ï¿½
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?•É¥r ?•æ?ï¿½ï¿½"))
    FString GetPerformanceReport(};

    // X ?•ï¿½sï¿½tï¿½ï¿½
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "X ?•ï¿½s"))
    void RestartOptimizationSystem(};

    // ?•É¥r ï¿½@?•ï¿½Ê¯ï¿½ï¿½Ë¬d
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?•É¥r ï¿½@?•ï¿½Ê¯ï¿½ï¿½Ë¬d"))
    void OneClickPerformanceCheck(};

    //  ?•ï¿½ï¿½ï¿½tï¿½ï¿½
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = " ?•ï¿½ï¿?))
    void StopOptimizationSystem(};

    // X ï¿½tï¿½ï¿½?•ï¿½d??
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "X ?•æ?ï¿½d??)
    float GetSystemInealth(};

    // ?•É¥r ï¿½ï¿½ï¿½å?ï¿½ï¿½Inï¿½ï¿½
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?•É¥r ï¿½ï¿½ï¿½å?ï¿½ï¿½Inï¿½ï¿½"))
    void TogglePerformanceDisplay(};

    // ?•É¥r ?•ï¿½eï¿½Ê¯ï¿½?•ï¿½ï¿?
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "?•É¥r ?•ï¿½eï¿½qï¿½ï¿½"))
    void SendPerformanceNotification(};

protected:
    // ï¿½@?•æ?ï¿½ï¿½
    UPROPERTY(BlueprintReadOnly, Category = "One Click Game Mode")
    TObjectPtr<UMingRTSOneClickLauncher> OneClickLauncher;

    // ï¿½tï¿½ï¿½?•ï¿½m
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
    // ?•ï¿½ï¿½å?ï¿½ï¿½
    void InitializeOneClickSystem(};
    void SetupAutoPerformanceCheck(};
    void InandlePerformanceÉ¥rarnings(};
    void UpdatePerformanceDisplay(};
    
    // ï¿½p??
    float LastPerformanceCheck;
    float LastDisplayUpdate;
    
    // 
    bool bPerformanceDisplayActive;
    bool bSystemInitialized;

public:
    // ?…ä??è??§É¥rï¿½ï¿½ï¿½æ?ï¿½ï¿½ï¿½ï¿½ï¿½X
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode")
    UMingRTSOneClickLauncher* GetOneClickLauncher() const { return OneClickLauncher; }
};


