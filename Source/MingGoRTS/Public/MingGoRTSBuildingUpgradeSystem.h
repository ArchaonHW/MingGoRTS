#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingBuildingActor.h"
#include "MingGoRTSBuildingUpgradeSystem.generated.h"

USTRUCT(BlueprintType)
struct FBuildingUpgradePath
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PathID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PathName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingBuildingType BuildingType;
};

USTRUCT(BlueprintType)
struct FBuildingUpgradeProgress
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString BuildingID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PathID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Progress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TimeRemaining;
};

USTRUCT(BlueprintType)
struct FBuildingUpgradeEffect
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EffectName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EBuildingUpgradeEffectType EffectType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Value;
};

USTRUCT(BlueprintType)
struct FBuildingUpgradeStatistics
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalUpgrades;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SuccessRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AverageUpgradeTime;
};

UENUM(BlueprintType)
enum class EBuildingUpgradeStatus : uint8
{
    None        UMETA(DisplayName = "None"),
    Available   UMETA(DisplayName = "Available"),
    InProgress  UMETA(DisplayName = "In Progress"),
    Completed   UMETA(DisplayName = "Completed"),
    Locked      UMETA(DisplayName = "Locked"),
    Paused      UMETA(DisplayName = "Paused"),
    Failed      UMETA(DisplayName = "Failed")
};

UENUM(BlueprintType)
enum class EBuildingUpgradeEffectType : uint8
{
    None            UMETA(DisplayName = "None"),
    HealthBoost     UMETA(DisplayName = "Health Boost"),
    DamageBoost     UMETA(DisplayName = "Damage Boost"),
    SpeedBoost      UMETA(DisplayName = "Speed Boost"),
    RangeBoost      UMETA(DisplayName = "Range Boost"),
    ResourceBoost   UMETA(DisplayName = "Resource Boost"),
    SpecialAbility  UMETA(DisplayName = "Special Ability")
};

UENUM(BlueprintType)
enum class EMingUpgradeResourceType : uint8
{
    None    UMETA(DisplayName = "None"),
    Gold    UMETA(DisplayName = "Gold"),
    Food    UMETA(DisplayName = "Food"),
    Wood    UMETA(DisplayName = "Wood"),
    Stone   UMETA(DisplayName = "Stone"),
    Iron    UMETA(DisplayName = "Iron")
};

/**
 * 建築升級系統
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSBuildingUpgradeSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSBuildingUpgradeSystem();

    // 初始化系統
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    void InitializeUpgradeSystem();

    // 註冊建築升級路徑
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    void RegisterUpgradePath(const FBuildingUpgradePath& UpgradePath);

    // 開始建築升級
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool StartBuildingUpgrade(const FString& BuildingID, const FString& PathID, int32 TargetLevel);

    // 暫停升級
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool PauseUpgrade(const FString& BuildingID);

    // 恢復升級
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool ResumeUpgrade(const FString& BuildingID);

    // 取消升級
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool CancelUpgrade(const FString& BuildingID);

    // 立即完成升級
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool InstantCompleteUpgrade(const FString& BuildingID);

    // 獲取升級進度
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    FBuildingUpgradeProgress GetUpgradeProgress(const FString& BuildingID) const;

    // 獲取所有升級路徑
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    TArray<FBuildingUpgradePath> GetAllUpgradePaths() const;

    // 獲取指定類別的升級路徑
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    TArray<FBuildingUpgradePath> GetUpgradePathsByType(EMingBuildingType BuildingType) const;

    // 獲取建築當前等級
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    int32 GetBuildingCurrentLevel(const FString& BuildingID) const;

    // 獲取升級統計
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    FBuildingUpgradeStatistics GetUpgradeStatistics() const;

    // 檢查升級是否可用
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool CanUpgrade(const FString& BuildingID, const FString& PathID, int32 TargetLevel) const;

    // 獲取升級成本
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    TMap<EMingUpgradeResourceType, float> GetUpgradeCost(const FString& BuildingID, const FString& PathID, int32 TargetLevel) const;

    // 獲取升級時間
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    float GetUpgradeTime(const FString& BuildingID, const FString& PathID, int32 TargetLevel) const;

    // 獲取升級效果
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    TArray<FBuildingUpgradeEffect> GetUpgradeEffects(const FString& PathID, int32 Level) const;

    // 解鎖升級路徑
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    void UnlockUpgradePath(const FString& PathID, EMingBuildingType BuildingType);

    // 檢查升級路徑是否已解鎖
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool IsUpgradePathUnlocked(const FString& PathID) const;

    // 事件委派
    UPROPERTY(BlueprintAssignable, Category = "Building Upgrade")
    FOnUpgradeStarted OnUpgradeStarted;

    UPROPERTY(BlueprintAssignable, Category = "Building Upgrade")
    FOnUpgradeProgress OnUpgradeProgress;

    UPROPERTY(BlueprintAssignable, Category = "Building Upgrade")
    FOnUpgradeCompleted OnUpgradeCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Building Upgrade")
    FOnUpgradeFailed OnUpgradeFailed;

    UPROPERTY(BlueprintAssignable, Category = "Building Upgrade")
    FOnUpgradePaused OnUpgradePaused;

    UPROPERTY(BlueprintAssignable, Category = "Building Upgrade")
    FOnUpgradeResumed OnUpgradeResumed;

    UPROPERTY(BlueprintAssignable, Category = "Building Upgrade")
    FOnUpgradePathUnlocked OnUpgradePathUnlocked;

protected:
    // 升級路徑映射
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    TMap<FString, FBuildingUpgradePath> UpgradePaths;

    // 活動升級映射
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    TMap<FString, FBuildingUpgradeProgress> ActiveUpgrades;

    // 建築等級映射
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    TMap<FString, int32> BuildingLevels;

    // 已解鎖路徑集合
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    TSet<FString> UnlockedPaths;

    // 更新升級進度
    UFUNCTION()
    void UpdateUpgradeProgress(float DeltaTime);

    // 完成升級
    void CompleteUpgrade(const FString& BuildingID);

    // 應用升級效果
    void ApplyUpgradeEffects(const FString& BuildingID, const TArray<FBuildingUpgradeEffect>& Effects);

    // 計算實際升級時間
    float CalculateActualUpgradeTime(const FString& BuildingID, const FString& PathID, int32 TargetLevel) const;

    // 檢查資源是否足夠
    bool HasEnoughResources(const TMap<EMingUpgradeResourceType, float>& Cost) const;

    // 消耗資源
    bool ConsumeResources(const TMap<EMingUpgradeResourceType, float>& Cost);

private:
    // 初始化標記
    bool bInitialized;
};

// 事件委派類型聲明
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpgradeStarted, const FString&, BuildingID, const FString&, PathID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpgradeProgress, const FString&, BuildingID, float, Progress, float, TimeRemaining);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpgradeCompleted, const FString&, BuildingID, const TArray<FBuildingUpgradeEffect>&, Effects);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpgradeFailed, const FString&, BuildingID, const FString&, ErrorMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpgradePaused, const FString&, BuildingID, const FString&, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpgradeResumed, const FString&, BuildingID, const FString&, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpgradePathUnlocked, const FString&, PathID, EMingBuildingType, BuildingType);
