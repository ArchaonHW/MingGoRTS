#pragma once\n\n#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Launcher/MingRTSOneClickLauncher.h"
#include "MyOneClickGameMode.generated.h"

/**
 * MingGoRTS �@???X??X?��?? * ????�@???X??X???����??����??��??
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "MingGoRTS")
class MINGGORTS_API AMyOneClickGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMyOneClickGameMode();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // X �@???X?�u??�t??
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "X �@???X?�uX))
    void LaunchOptimizationSystem();

    // ??? ??��????�]��??�t�Φ�????�m??
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "??? ??��????"))
    void SmartLaunchSystem();

    // ??? ���P????�]??���ʯ�??
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "??? ���P????"))
    void ExtremeLaunchSystem();

    // X ????�t��??X
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "X ????�t��??X))
    FString GetSystemStatus();

    // ??? ???X?��????
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "??? ???X?��????"))
    FString GetPerformanceReport();

    // X ??�s????�t��
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "X ??�s????"))
    void RestartOptimizationSystem();

    // ??? �@??�ʯ��ˬd
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "??? �@??�ʯ��ˬd"))
    void OneClickPerformanceCheck();

    // ???? ??��????�t��
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "???? ??��????"))
    void StopOptimizationSystem();

    // X ????�t��??�d??
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "X ???X?�d??))
    float GetSystemHealth();

    // ??? ���??��H��
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "??? ���??��H��"))
    void TogglePerformanceDisplay();

    // ??? ??�e�ʯ�??��
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "??? ??�e�q��"))
    void SendPerformanceNotification();

protected:
    // �@???X?��
    UPROPERTY(BlueprintReadOnly, Category = "One Click Game Mode")
    TObjectPtr<UMingRTSOneClickLauncher> OneClickLauncher;

    // �t��??�m
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
    // ??��??��
    void InitializeOneClickSystem();
    void SetupAutoPerformanceCheck();
    void HandlePerformanceWarnings();
    void UpdatePerformanceDisplay();
    
    // �p?X
    float LastPerformanceCheck;
    float LastDisplayUpdate;
    
    // ????????
    bool bPerformanceDisplayActive;
    bool bSystemInitialized;

public:
    // ???X??X?���?X?��????���X????
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode")
    UMingRTSOneClickLauncher* GetOneClickLauncher() const { return OneClickLauncher; }
};

