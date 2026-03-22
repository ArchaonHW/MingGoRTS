#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSAchievementSystem.generated.h"

/**
 * ?�就稀?�度
 */
UENUM(BlueprintType)
enum class EAchievementRarity: uint8 {
    Common          UMETA(DisplayName = "普通"),
    Uncommon        UMETA(DisplayName = "稀有"),
    Rare            UMETA(DisplayName = "史詩"),
    Epic            UMETA(DisplayName = "傳說"),
    Legendary       UMETA(DisplayName = "神話")
};

/**
 * ?�就類�?
 */
UENUM(BlueprintType)
enum class EAchievementType: uint8 {
    Campaign        UMETA(DisplayName = "?�役?�就"),
    Combat          UMETA(DisplayName = "?�鬥?�就"),
    Economic        UMETA(DisplayName = "經�X�就"),
    Strategic       UMETA(DisplayName = "策略?�就"),
    Collection      UMETA(DisplayName = "?��X�就"),
    Social          UMETA(DisplayName = "社交?�就"),
    Challenge       UMETA(DisplayName = "?�戰?�就"),
    Hidden          UMETA(DisplayName = "?��X�就")
};

/**
 * ?�就?��?
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
 * ?�家?�就?�度
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
 * ?�就統�?
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
 * ?�就系統 - 管�X�戲?�就
 */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGRTS_API UMingRTSAchievementSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSAchievementSystem();
    
    // ?��X
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    void InitializeAchievementSystem();
    
    // 載入?�就定義
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    void LoadAchievementDefinitions();
    
    // ?��X�?��?�?
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    TArray<FAchievementData> GetAllAchievements() const;
    
    // ?��X��?類�X�就
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    TArray<FAchievementData> GetAchievementsByType(EAchievementType Type) const;
    
    // ?��X��?稀?�度?�就
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    TArray<FAchievementData> GetAchievementsByRarity(EAchievementRarity Rarity) const;
    
    // ?��X�就詳�?
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    bool GetAchievementData(FName AchievementID, FAchievementData& OutData) const;
    
    // ?��X�家?�就?�度
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    FPlayerAchievementProgress GetPlayerProgress(const FString& PlayerID, FName AchievementID) const;
    
    // ?�新?�就?�度
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    void UpdateAchievementProgress(const FString& PlayerID, FName AchievementID, int32 Progress);
    
    // 增�X�就?�度
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    void IncrementAchievementProgress(const FString& PlayerID, FName AchievementID, int32 Amount = 1);
    
    // �X�就
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    void UnlockAchievement(const FString& PlayerID, FName AchievementID);
    
    // 檢查?�就?�否已解X
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    bool IsAchievementUnlocked(const FString& PlayerID, FName AchievementID) const;
    
    // ?��?已解?��?�?
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    TArray<FPlayerAchievementProgress> GetUnlockedAchievements(const FString& PlayerID) const;
    
    // ?��X��?中�?�?
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    TArray<FPlayerAchievementProgress> GetInProgressAchievements(const FString& PlayerID) const;
    
    // ?��X�就統�?
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    FAchievementStats GetAchievementStats(const FString& PlayerID) const;
    
    // ?��X�近解?��X�就
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    TArray<FPlayerAchievementProgress> GetRecentlyUnlockedAchievements(const FString& PlayerID, int32 Count = 5) const;
    
    // ?�置?�家?�就
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    void ResetPlayerAchievements(const FString& PlayerID);
    
    // 保�X�就?��?
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    void SaveAchievementData(const FString& PlayerID);
    
    // 載入?�就?��?
    UFUNCTION(BlueprintCallable, Category = "Achievement")
    void LoadAchievementData(const FString& PlayerID);
    
    // 事件委�?
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAchievementUnlocked, const FString&, PlayerID, FName, AchievementID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAchievementProgressUpdated, const FString&, PlayerID, FName, AchievementID, int32, NewProgress);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAchievementCompleted, const FString&, PlayerID, const FAchievementData&, AchievementData);
    
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
    
    void InitializeDefaultAchievements();
    void CheckPrerequisites(const FString& PlayerID, FName AchievementID);
    void CheckRelatedAchievements(const FString& PlayerID, FName AchievementID);
    void BroadcastAchievementUnlock(const FString& PlayerID, FName AchievementID);
    
    FAchievementData CreateAchievement(FName ID, const FText& Title, const FText& Description,
        EAchievementType Type, EAchievementRarity Rarity, int32 Points, bool bIsSecret = false);
};
};

