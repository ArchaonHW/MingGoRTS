#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSAchievementSystem.generated.h"

/**
 * ?êÂ∞±Á®Ä?âÂ∫¶
 */
UENUM(BlueprintType)
enum class EAchievementRarity : uint8
{
    Common          UMETA(DisplayName = "?ÆÈÄ?),
    Uncommon        UMETA(DisplayName = "Á®Ä??),
    Rare            UMETA(DisplayName = "?≤Ë©©"),
    Epic            UMETA(DisplayName = "?≥Ë™™"),
    Legendary       UMETA(DisplayName = "Á•ûË©±")
};

/**
 * ?êÂ∞±È°ûÂ?
 */
UENUM(BlueprintType)
enum class EAchievementType : uint8
{
    Campaign        UMETA(DisplayName = "?∞ÂΩπ?êÂ∞±"),
    Combat          UMETA(DisplayName = "?∞È¨•?êÂ∞±"),
    Economic        UMETA(DisplayName = "Á∂ìÊ??êÂ∞±"),
    Strategic       UMETA(DisplayName = "Á≠ñÁï•?êÂ∞±"),
    Collection      UMETA(DisplayName = "?∂È??êÂ∞±"),
    Social          UMETA(DisplayName = "Á§æ‰∫§?êÂ∞±"),
    Challenge       UMETA(DisplayName = "?ëÊà∞?êÂ∞±"),
    Hidden          UMETA(DisplayName = "?±Ë??êÂ∞±")
};

/**
 * ?êÂ∞±?∏Ê?
 */
USTRUCT(BlueprintType)
struct FAchievementData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName AchievementID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Title;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EAchievementType Type;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EAchievementRarity Rarity;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString IconPath;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Points;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsSecret;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsHidden;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString UnlockCondition;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FName> Prerequisites;
    
    FAchievementData()
        : Type(EAchievementType::Campaign)
        , Rarity(EAchievementRarity::Common)
        , Points(0)
        , bIsSecret(false)
        , bIsHidden(false)
    {}
};

/**
 * ?©ÂÆ∂?êÂ∞±?≤Â∫¶
 */
USTRUCT(BlueprintType)
struct FPlayerAchievementProgress
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName AchievementID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentProgress;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetProgress;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsCompleted;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString UnlockDate;
    
    FPlayerAchievementProgress()
        : CurrentProgress(0)
        , TargetProgress(1)
        , bIsCompleted(false)
    {}
};

/**
 * ?êÂ∞±Áµ±Ë?
 */
USTRUCT(BlueprintType)
struct FAchievementStats
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalAchievements;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 UnlockedAchievements;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalPoints;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EarnedPoints;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CompletionPercentage;
    
    FAchievementStats()
        : TotalAchievements(0)
        , UnlockedAchievements(0)
        , TotalPoints(0)
        , EarnedPoints(0)
        , CompletionPercentage(0.0f)
    {}
};

/**
 * ?êÂ∞±Á≥ªÁµ± - ÁÆ°Á??äÊà≤?êÂ∞±
 */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSAchievementSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSAchievementSystem(};
    
    // ?ùÂ???    UFUNCTION(BlueprintCallable, Category = "Achievement")
    void InitializeAchievementSystem(};
    
    // ËºâÂÖ•?êÂ∞±ÂÆöÁæ©
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    void LoadAchievementDefinitions(};
    
    // ?≤Â??Ä?âÊ?Â∞?    UFUNCTION(BlueprintCallable, Category = "Achievement")
    TArray<FAchievementData> GetAllAchievements() const;
    
    // ?≤Â??πÂ?È°ûÂ??êÂ∞±
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    TArray<FAchievementData> GetAchievementsByType(EAchievementType Type) const;
    
    // ?≤Â??πÂ?Á®Ä?âÂ∫¶?êÂ∞±
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    TArray<FAchievementData> GetAchievementsByRarity(EAchievementRarity Rarity) const;
    
    // ?≤Â??êÂ∞±Ë©≥Ê?
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    bool GetAchievementData(FName AchievementID, FAchievementData& OutData) const;
    
    // ?≤Â??©ÂÆ∂?êÂ∞±?≤Â∫¶
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    FPlayerAchievementProgress GetPlayerProgress(const FString& PlayerID, FName AchievementID) const;
    
    // ?¥Êñ∞?êÂ∞±?≤Â∫¶
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    void UpdateAchievementProgress(const FString& PlayerID, FName AchievementID, int32 Progress};
    
    // Â¢ûÂ??êÂ∞±?≤Â∫¶
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    void IncrementAchievementProgress(const FString& PlayerID, FName AchievementID, int32 Amount = 1};
    
    // Ëß???êÂ∞±
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    void UnlockAchievement(const FString& PlayerID, FName AchievementID};
    
    // Ê™¢Êü•?êÂ∞±?ØÂê¶Â∑≤Ëß£??    UFUNCTION(BlueprintCallable, Category = "Achievement")
    bool IsAchievementUnlocked(const FString& PlayerID, FName AchievementID) const;
    
    // ?≤Â?Â∑≤Ëß£?ñÊ?Â∞?    UFUNCTION(BlueprintCallable, Category = "Achievement")
    TArray<FPlayerAchievementProgress> GetUnlockedAchievements(const FString& PlayerID) const;
    
    // ?≤Â??≤Ë?‰∏≠Ê?Â∞?    UFUNCTION(BlueprintCallable, Category = "Achievement")
    TArray<FPlayerAchievementProgress> GetInProgressAchievements(const FString& PlayerID) const;
    
    // ?≤Â??êÂ∞±Áµ±Ë?
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    FAchievementStats GetAchievementStats(const FString& PlayerID) const;
    
    // ?≤Â??ÄËøëËß£?ñÁ??êÂ∞±
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    TArray<FPlayerAchievementProgress> GetRecentlyUnlockedAchievements(const FString& PlayerID, int32 Count = 5) const;
    
    // ?çÁΩÆ?©ÂÆ∂?êÂ∞±
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    void ResetPlayerAchievements(const FString& PlayerID};
    
    // ‰øùÂ??êÂ∞±?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    void SaveAchievementData(const FString& PlayerID};
    
    // ËºâÂÖ•?êÂ∞±?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    void LoadAchievementData(const FString& PlayerID};
    
    // ‰∫ã‰ª∂ÂßîË?
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAchievementUnlocked, const FString&, PlayerID, FName, AchievementID};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAchievementProgressUpdated, const FString&, PlayerID, FName, AchievementID, int32, NewProgress};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAchievementCompleted, const FString&, PlayerID, const FAchievementData&, AchievementData};
    
    UPROPERTY(BlueprintAssignable, Category = "Achievement|Events")
    FOnAchievementUnlocked OnAchievementUnlocked;
    
    UPROPERTY(BlueprintAssignable, Category = "Achievement|Events")
    FOnAchievementProgressUpdated OnAchievementProgressUpdated;
    
    UPROPERTY(BlueprintAssignable, Category = "Achievement|Events")
    FOnAchievementCompleted OnAchievementCompleted;
    
private:
    UPROPERTY()
    TMap<FName, FAchievementData> AchievementDefinitions;
    
    UPROPERTY()
    TMap<FString, TMap<FName, FPlayerAchievementProgress>> PlayerAchievements;
    
    void InitializeDefaultAchievements(};
    void CheckPrerequisites(const FString& PlayerID, FName AchievementID};
    void CheckRelatedAchievements(const FString& PlayerID, FName AchievementID};
    void BroadcastAchievementUnlock(const FString& PlayerID, FName AchievementID};
    
    FAchievementData CreateAchievement(FName ID, const FText& Title, const FText& Description,
        EAchievementType Type, EAchievementRarity Rarity, int32 Points, bool bIsSecret = false};
};

