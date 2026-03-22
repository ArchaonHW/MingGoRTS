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
 * 建�X��?系統
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSBuildingUpgradeSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSBuildingUpgradeSystem(};

    // ?��X�系�?    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    void InitializeUpgradeSystem(};

    // 註�?建�X��?路�?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    void RegisterUpgradePath(const FBuildingUpgradePath& UpgradePath};

    // ?��?建�X��?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool StartBuildingUpgrade(const FString& BuildingID, const FString& PathID, int32 TargetLevel};

    // ?��X��?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool PauseUpgrade(const FString& BuildingID};

    // ?�復?��?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool ResumeUpgrade(const FString& BuildingID};

    // ?��X��?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool CancelUpgrade(const FString& BuildingID};

    // 立即完�X��?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool InstantCompleteUpgrade(const FString& BuildingID};

    // ?��X��X�度
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    FBuildingUpgradeProgress GetUpgradeProgress(const FString& BuildingID) const;

    // ?��X�?��?級路�?    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    TArray<FBuildingUpgradePath> GetAllUpgradePaths() const;

    // ?��X��?類別?��?級路�?    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    TArray<FBuildingUpgradePath> GetUpgradePathsByType(EMingBuildingType BuildingType) const;

    // ?��?建�X��?等�?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    int32 GetBuildingCurrentLevel(const FString& BuildingID) const;

    // ?��X��?統�?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    FBuildingUpgradeStatistics GetUpgradeStatistics() const;

    // 檢查?��X�否?�用
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool CanUpgrade(const FString& BuildingID, const FString& PathID, int32 TargetLevel) const;

    // ?��X��X�本
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    TMap<EMingUpgradeResourceType, float> GetUpgradeCost(const FString& BuildingID, const FString& PathID, int32 TargetLevel) const;

    // ?��X��X��?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    float GetUpgradeTime(const FString& BuildingID, const FString& PathID, int32 TargetLevel) const;

    // ?��X��X��?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    TArray<FBuildingUpgradeEffect> GetUpgradeEffects(const FString& PathID, int32 Level) const;

    // �X��?路�?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    void UnlockUpgradePath(const FString& PathID, EMingBuildingType BuildingType};

    // 檢查?��?路�X�否已解X    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
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
    // ?��?路�X��?
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    TMap<FString, FBuildingUpgradePath> UpgradePaths;

    // 活�X��X��?
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    TMap<FString, FBuildingUpgradeProgress> ActiveUpgrades;

    // 建�?等�X��?
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    TMap<FString, int32> BuildingLevels;

    // 已解?�路徑�X    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    TSet<FString> UnlockedPaths;

    // ?�新?��X�度
    UFUNCTION()
    void UpdateUpgradeProgress(float DeltaTime};

    // 完�X��?
    void CompleteUpgrade(const FString& BuildingID};

    // ?�用?��X��?
    void ApplyUpgradeEffects(const FString& BuildingID, const TArray<FBuildingUpgradeEffect>& Effects};

    // 計�?實�X��X��?
    float CalculateActualUpgradeTime(const FString& BuildingID, const FString& PathID, int32 TargetLevel) const;

    // 檢查資�X�否足�?
    bool HasEnoughResources(const TMap<EMingUpgradeResourceType, float>& Cost) const;

    // 消耗�?�?    bool ConsumeResources(const TMap<EMingUpgradeResourceType, float>& Cost};

private:
    // ?��X��?�?    bool bInitialized;
};

// 事件委派類�X��?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpgradeStarted, const FString&, BuildingID, const FString&, PathID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpgradeProgress, const FString&, BuildingID, float, Progress, float, TimeRemaining};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpgradeCompleted, const FString&, BuildingID, const TArray<FBuildingUpgradeEffect>&, Effects};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpgradeFailed, const FString&, BuildingID, const FString&, ErrorMessage};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpgradePaused, const FString&, BuildingID, const FString&, Reason};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpgradeResumed, const FString&, BuildingID, const FString&, Reason};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpgradePathUnlocked, const FString&, PathID, EMingBuildingType, BuildingType};

