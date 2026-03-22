#pragma once\n\n#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Launcher/MingRTSOneClickLauncher.h"
#include "MyOneClickGameMode.generated.h"

/**
 * MingGoRTS �@故事重要性?��?? * 摧毀�@整理背包物品����??����??��??
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "MingGoRTS")
class MINGRTS_API AMyOneClickGameMode : public AGameModeBase
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

    // ??? ??��摧毀�]��??�t�Φ�摧毀�m??
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "??? ??��摧毀"))
    void SmartLaunchSystem();

    // ??? ���P摧毀�]??���ʯ�??
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "??? ���P摧毀"))
    void ExtremeLaunchSystem();

    // X 摧毀�t��??X
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "X 摧毀�t��??X))
    FString GetSystemStatus();

    // ??? ???X?��摧毀
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "??? ???X?��摧毀"))
    FString GetPerformanceReport();

    // X ??�s摧毀�t��
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "X ??�s摧毀"))
    void RestartOptimizationSystem();

    // ??? �@??�ʯ��ˬd
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "??? �@??�ʯ��ˬd"))
    void OneClickPerformanceCheck();

    // 摧毀 ??��摧毀�t��
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode", meta = (DisplayName = "摧毀 ??��摧毀"))
    void StopOptimizationSystem();

    // X 摧毀�t��??�d??
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
    
    // 摧毀摧毀
    bool bPerformanceDisplayActive;
    bool bSystemInitialized;

public:
    // 故事重要性?���?X?��摧毀���X摧毀
    UFUNCTION(BlueprintCallable, Category = "One Click Game Mode")
    UMingRTSOneClickLauncher* GetOneClickLauncher() const { return OneClickLauncher; }
};

