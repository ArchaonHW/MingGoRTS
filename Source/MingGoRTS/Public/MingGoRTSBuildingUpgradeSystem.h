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
enum class EBuildingUpgradeStatus: uint8 {
    None        UMETA(DisplayName = "None"),
    Available   UMETA(DisplayName = "Available"),
    InProgress  UMETA(DisplayName = "In Progress"),
    Completed   UMETA(DisplayName = "Completed"),
    Locked      UMETA(DisplayName = "Locked"),
    Paused      UMETA(DisplayName = "Paused"),
    Failed      UMETA(DisplayName = "Failed")
};

UENUM(BlueprintType)
enum class EBuildingUpgradeEffectType: uint8 {
    None            UMETA(DisplayName = "None"),
    HealthBoost     UMETA(DisplayName = "Health Boost"),
    DamageBoost     UMETA(DisplayName = "Damage Boost"),
    SpeedBoost      UMETA(DisplayName = "Speed Boost"),
    RangeBoost      UMETA(DisplayName = "Range Boost"),
    ResourceBoost   UMETA(DisplayName = "Resource Boost"),
    SpecialAbility  UMETA(DisplayName = "Special Ability")
};

UENUM(BlueprintType)
enum class EMingUpgradeResourceType: uint8 {
    None    UMETA(DisplayName = "None"),
    Gold    UMETA(DisplayName = "Gold"),
    Food    UMETA(DisplayName = "Food"),
    Wood    UMETA(DisplayName = "Wood"),
    Stone   UMETA(DisplayName = "Stone"),
    Iron    UMETA(DisplayName = "Iron")
};

/**
 * ��?X???�t��
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSBuildingUpgradeSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSBuildingUpgradeSystem();

    // ???X?�t??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    void InitializeUpgradeSystem();

    // ��??��?X???��??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    void RegisterUpgradePath(const FBuildingUpgradePath& UpgradePath);

    // 摧毀��?X???
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool StartBuildingUpgrade(const FString& BuildingID, const FString& PathID, int32 TargetLevel);

    // 目標數量
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool PauseUpgrade(const FString& BuildingID);

    // ??�_摧毀
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool ResumeUpgrade(const FString& BuildingID);

    // 目標數量
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool CancelUpgrade(const FString& BuildingID);

    // �ߧY��?X???
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool InstantCompleteUpgrade(const FString& BuildingID);

    // 故事重要性?��
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    FBuildingUpgradeProgress GetUpgradeProgress(const FString& BuildingID) const;

    // 目標數量??�Ÿ�??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    TArray<FBuildingUpgradePath> GetAllUpgradePaths() const;

    // 目標數量���O摧毀�Ÿ�??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    TArray<FBuildingUpgradePath> GetUpgradePathsByType(EMingBuildingType BuildingType) const;

    // 摧毀��?X???��??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    int32 GetBuildingCurrentLevel(const FString& BuildingID) const;

    // 目標數量��??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    FBuildingUpgradeStatistics GetUpgradeStatistics() const;

    // �ˬd???X?�_??��
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool CanUpgrade(const FString& BuildingID, const FString& PathID, int32 TargetLevel) const;

    // 故事重要性?��
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    TMap<EMingUpgradeResourceType, float> GetUpgradeCost(const FString& BuildingID, const FString& PathID, int32 TargetLevel) const;

    // 整理背包物品
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    float GetUpgradeTime(const FString& BuildingID, const FString& PathID, int32 TargetLevel) const;

    // 整理背包物品
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    TArray<FBuildingUpgradeEffect> GetUpgradeEffects(const FString& PathID, int32 Level) const;

    // ?X???��??
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    void UnlockUpgradePath(const FString& PathID, EMingBuildingType BuildingType);

    // �ˬd摧毀��?X?�_�w��X
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool IsUpgradePathUnlocked(const FString& PathID) const;

    // �ƥ�e��
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
    // 摧毀��?X???
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    TMap<FString, FBuildingUpgradePath> UpgradePaths;

    // ��?X??X???
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    TMap<FString, FBuildingUpgradeProgress> ActiveUpgrades;

    // ��??��?X???
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    TMap<FString, int32> BuildingLevels;

    // �w��??��|?X
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    TSet<FString> UnlockedPaths;

    // ??�s???X?��
    UFUNCTION()
    void UpdateUpgradeProgress(float DeltaTime);

    // ��?X???
    void CompleteUpgrade(const FString& BuildingID);

    // ??��目標數量
    void ApplyUpgradeEffects(const FString& BuildingID, const TArray<FBuildingUpgradeEffect>& Effects);

    // �p??��?X??X???
    float CalculateActualUpgradeTime(const FString& BuildingID, const FString& PathID, int32 TargetLevel) const;

    // �ˬd��?X?�_��??
    bool HasEnoughResources(const TMap<EMingUpgradeResourceType, float>& Cost) const;

    // ����摧毀
    bool ConsumeResources(const TMap<EMingUpgradeResourceType, float>& Cost);

private:
    // 目標數量??
    bool bInitialized;
);

// �ƥ�e����?X???

#endif // MINGGORTSBUILDINGUPGRADESYSTEM_H
