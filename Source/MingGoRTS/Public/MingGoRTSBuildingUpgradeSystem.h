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
 * Âª∫Á??áÁ?Á≥ªÁµ±
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSBuildingUpgradeSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSBuildingUpgradeSystem();

    // ?ùÂ??ñÁ≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    void InitializeUpgradeSystem();

    // Ë®ªÂ?Âª∫Á??áÁ?Ë∑ØÂ?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    void RegisterUpgradePath(const FBuildingUpgradePath& UpgradePath);

    // ?ãÂ?Âª∫Á??áÁ?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool StartBuildingUpgrade(const FString& BuildingID, const FString& PathID, int32 TargetLevel);

    // ?´Â??áÁ?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool PauseUpgrade(const FString& BuildingID);

    // ?¢Âæ©?áÁ?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool ResumeUpgrade(const FString& BuildingID);

    // ?ñÊ??áÁ?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool CancelUpgrade(const FString& BuildingID);

    // Á´ãÂç≥ÂÆåÊ??áÁ?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool InstantCompleteUpgrade(const FString& BuildingID);

    // ?≤Â??áÁ??≤Â∫¶
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    FBuildingUpgradeProgress GetUpgradeProgress(const FString& BuildingID) const;

    // ?≤Â??Ä?âÂ?Á¥öË∑ØÂæ?    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    TArray<FBuildingUpgradePath> GetAllUpgradePaths() const;

    // ?≤Â??áÂ?È°ûÂà•?ÑÂ?Á¥öË∑ØÂæ?    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    TArray<FBuildingUpgradePath> GetUpgradePathsByType(EMingBuildingType BuildingType) const;

    // ?≤Â?Âª∫Á??∂Â?Á≠âÁ?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    int32 GetBuildingCurrentLevel(const FString& BuildingID) const;

    // ?≤Â??áÁ?Áµ±Ë?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    FBuildingUpgradeStatistics GetUpgradeStatistics() const;

    // Ê™¢Êü•?áÁ??ØÂê¶?ØÁî®
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool CanUpgrade(const FString& BuildingID, const FString& PathID, int32 TargetLevel) const;

    // ?≤Â??áÁ??êÊú¨
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    TMap<EMingUpgradeResourceType, float> GetUpgradeCost(const FString& BuildingID, const FString& PathID, int32 TargetLevel) const;

    // ?≤Â??áÁ??ÇÈ?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    float GetUpgradeTime(const FString& BuildingID, const FString& PathID, int32 TargetLevel) const;

    // ?≤Â??áÁ??àÊ?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    TArray<FBuildingUpgradeEffect> GetUpgradeEffects(const FString& PathID, int32 Level) const;

    // Ëß???áÁ?Ë∑ØÂ?
    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    void UnlockUpgradePath(const FString& PathID, EMingBuildingType BuildingType);

    // Ê™¢Êü•?áÁ?Ë∑ØÂ??ØÂê¶Â∑≤Ëß£??    UFUNCTION(BlueprintCallable, Category = "Building Upgrade")
    bool IsUpgradePathUnlocked(const FString& PathID) const;

    // ‰∫ã‰ª∂ÂßîÊ¥æ
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
    // ?áÁ?Ë∑ØÂ??†Â?
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    TMap<FString, FBuildingUpgradePath> UpgradePaths;

    // Ê¥ªÂ??áÁ??†Â?
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    TMap<FString, FBuildingUpgradeProgress> ActiveUpgrades;

    // Âª∫Á?Á≠âÁ??†Â?
    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    TMap<FString, int32> BuildingLevels;

    // Â∑≤Ëß£?ñË∑ØÂæëÈ???    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    TSet<FString> UnlockedPaths;

    // ?¥Êñ∞?áÁ??≤Â∫¶
    UFUNCTION()
    void UpdateUpgradeProgress(float DeltaTime);

    // ÂÆåÊ??áÁ?
    void CompleteUpgrade(const FString& BuildingID);

    // ?âÁî®?áÁ??àÊ?
    void ApplyUpgradeEffects(const FString& BuildingID, const TArray<FBuildingUpgradeEffect>& Effects);

    // Ë®àÁ?ÂØ¶È??áÁ??ÇÈ?
    float CalculateActualUpgradeTime(const FString& BuildingID, const FString& PathID, int32 TargetLevel) const;

    // Ê™¢Êü•Ë≥áÊ??ØÂê¶Ë∂≥Â?
    bool HasEnoughResources(const TMap<EMingUpgradeResourceType, float>& Cost) const;

    // Ê∂àËÄóË?Ê∫?    bool ConsumeResources(const TMap<EMingUpgradeResourceType, float>& Cost);

private:
    // ?ùÂ??ñÊ?Ë®?    bool bInitialized;
};

// ‰∫ã‰ª∂ÂßîÊ¥æÈ°ûÂ??≤Ê?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpgradeStarted, const FString&, BuildingID, const FString&, PathID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpgradeProgress, const FString&, BuildingID, float, Progress, float, TimeRemaining);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpgradeCompleted, const FString&, BuildingID, const TArray<FBuildingUpgradeEffect>&, Effects);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpgradeFailed, const FString&, BuildingID, const FString&, ErrorMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpgradePaused, const FString&, BuildingID, const FString&, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpgradeResumed, const FString&, BuildingID, const FString&, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpgradePathUnlocked, const FString&, PathID, EMingBuildingType, BuildingType);
