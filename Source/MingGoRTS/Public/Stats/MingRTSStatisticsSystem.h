#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSStatisticsSystem.generated.h"

/**
 * Áµ±Ë??∏Ê?È°ûÂ?
 */
UENUM(BlueprintType)
enum class EStatType : uint8
{
    MatchesPlayed         UMETA(DisplayName = "Â∞çÂ???),
    MatchesWon            UMETA(DisplayName = "?ùÂ†¥??),
    MatchesLost           UMETA(DisplayName = "?óÂ†¥??),
    WinRate               UMETA(DisplayName = "?ùÁ?"),
    TotalPlayTime         UMETA(DisplayName = "Á∏ΩÈ??≤Ê???),
    UnitsKilled           UMETA(DisplayName = "Ê∂àÊ??Æ‰???),
    UnitsLost             UMETA(DisplayName = "?çÂ§±?Æ‰???),
    BuildingsConstructed  UMETA(DisplayName = "Âª∫ÈÄ†Âª∫ÁØâÊï∏"),
    BuildingsDestroyed    UMETA(DisplayName = "?ßÊ?Âª∫Á???),
    ResourcesGathered     UMETA(DisplayName = "?∂È?Ë≥áÊ?Á∏ΩÈ?"),
    ResourcesSpent        UMETA(DisplayName = "Ê∂àËÄóË?Ê∫êÁ∏Ω??),
    TechnologiesResearched UMETA(DisplayName = "?îÁ©∂ÁßëÊ???),
    APM                   UMETA(DisplayName = "Âπ≥Â?APM"),
    PeakAPM               UMETA(DisplayName = "Â≥∞ÂÄºAPM"),
    FavoriteFaction       UMETA(DisplayName = "?Ä?õÂã¢??),
    FavoriteMap           UMETA(DisplayName = "?Ä?õÂú∞??)
};

/**
 * Â∞çÂ?Áµ±Ë?
 */
USTRUCT(BlueprintType)
struct FMatchStatistics
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MatchID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MapName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Faction;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OpponentFaction;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsVictory;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 UnitsKilled;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 UnitsLost;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 BuildingsConstructed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 BuildingsDestroyed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ResourcesGathered;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AverageAPM;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PeakAPM;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Timestamp;
    
    FMatchStatistics()
        : Duration(0.0f)
        , bIsVictory(false)
        , UnitsKilled(0)
        , UnitsLost(0)
        , BuildingsConstructed(0)
        , BuildingsDestroyed(0)
        , ResourcesGathered(0)
        , AverageAPM(0.0f)
        , PeakAPM(0.0f)
    {}
};

/**
 * ?©ÂÆ∂?üÊ∂ØÁµ±Ë?
 */
USTRUCT(BlueprintType)
struct FPlayerCareerStats
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalMatches;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Wins;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Losses;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Draws;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WinRate;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TotalPlayTimeHours;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 TotalUnitsKilled;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 TotalUnitsLost;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 TotalBuildingsConstructed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 TotalBuildingsDestroyed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 TotalResourcesGathered;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AverageMatchDuration;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AverageAPM;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HighestAPM;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FavoriteFaction;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FavoriteMap;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentWinStreak;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 LongestWinStreak;
    
    FPlayerCareerStats()
        : TotalMatches(0)
        , Wins(0)
        , Losses(0)
        , Draws(0)
        , WinRate(0.0f)
        , TotalPlayTimeHours(0.0f)
        , TotalUnitsKilled(0)
        , TotalUnitsLost(0)
        , TotalBuildingsConstructed(0)
        , TotalBuildingsDestroyed(0)
        , TotalResourcesGathered(0)
        , AverageMatchDuration(0.0f)
        , AverageAPM(0.0f)
        , HighestAPM(0.0f)
        , CurrentWinStreak(0)
        , LongestWinStreak(0)
    {}
};

/**
 * ?¢Â?Áµ±Ë?
 */
USTRUCT(BlueprintType)
struct FFactionStatistics
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FactionName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MatchesPlayed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Wins;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WinRate;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AverageMatchDuration;
    
    FFactionStatistics()
        : MatchesPlayed(0)
        , Wins(0)
        , WinRate(0.0f)
        , AverageMatchDuration(0.0f)
    {}
};

/**
 * Áµ±Ë?Á≥ªÁµ± - Ë®òÈ??åÂ??êÁé©ÂÆ∂Áµ±Ë®àÊï∏?? */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSStatisticsSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSStatisticsSystem(};
    
    // ?ùÂ???    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void InitializeStatisticsSystem(};
    
    // Ë®òÈ?Â∞çÂ??ãÂ?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void RecordMatchStart(const FString& PlayerID, const FString& MatchID, const FString& MapName, const FString& Faction};
    
    // Ë®òÈ?Â∞çÂ?ÁµêÊ?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void RecordMatchEnd(const FString& PlayerID, const FString& MatchID, bool bIsVictory, const FMatchStatistics& Stats};
    
    // ?¥Êñ∞ÂØ¶Ê?Áµ±Ë?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void UpdateLiveStat(const FString& PlayerID, EStatType StatType, float Value};
    
    // Ë®òÈ??Æ‰??äÊÆ∫
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void RecordUnitKilled(const FString& PlayerID, const FString& MatchID, int32 UnitValue};
    
    // Ë®òÈ?Ë≥áÊ??∂È?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void RecordResourceGathered(const FString& PlayerID, const FString& MatchID, int32 Amount};
    
    // Ë®òÈ?APM
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void RecordAPM(const FString& PlayerID, const FString& MatchID, float CurrentAPM};
    
    // ?≤Â??üÊ∂ØÁµ±Ë?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    FPlayerCareerStats GetCareerStats(const FString& PlayerID) const;
    
    // ?≤Â?Â∞çÂ?Ê≠∑Âè≤
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    TArray<FMatchStatistics> GetMatchHistory(const FString& PlayerID, int32 Count = 10) const;
    
    // ?≤Â??¢Â?Áµ±Ë?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    TArray<FFactionStatistics> GetFactionStats(const FString& PlayerID) const;
    
    // ?≤Â??πÂ?Áµ±Ë???    UFUNCTION(BlueprintCallable, Category = "Statistics")
    float GetStatValue(const FString& PlayerID, EStatType StatType) const;
    
    // ?≤Â?Áµ±Ë?Ë∂®Âã¢
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    TArray<float> GetStatTrend(const FString& PlayerID, EStatType StatType, int32 Days = 30) const;
    
    // ÊØîË?Áµ±Ë?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    FString CompareStats(const FString& PlayerID1, const FString& PlayerID2) const;
    
    // ?≤Â??íË?Ê¶ú‰?ÁΩ?    UFUNCTION(BlueprintCallable, Category = "Statistics")
    int32 GetLeaderboardRank(const FString& PlayerID, EStatType StatType) const;
    
    // ?≤Â?Áµ±Ë??òË?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    FString GetStatsSummary(const FString& PlayerID) const;
    
    // Â∞éÂá∫Áµ±Ë??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    bool ExportStatsToCSV(const FString& PlayerID, const FString& FilePath) const;
    
    // ?çÁΩÆÁµ±Ë?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void ResetStatistics(const FString& PlayerID};
    
    // ‰øùÂ?Áµ±Ë?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void SaveStatistics(const FString& PlayerID};
    
    // ËºâÂÖ•Áµ±Ë?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void LoadStatistics(const FString& PlayerID};
    
    // ‰∫ã‰ª∂ÂßîË?
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStatUpdated, const FString&, PlayerID, EStatType, StatType, float, NewValue};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMatchRecorded, const FString&, PlayerID, const FMatchStatistics&, MatchStats};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMilestoneReached, const FString&, MilestoneText};
    
    UPROPERTY(BlueprintAssignable, Category = "Statistics|Events")
    FOnStatUpdated OnStatUpdated;
    
    UPROPERTY(BlueprintAssignable, Category = "Statistics|Events")
    FOnMatchRecorded OnMatchRecorded;
    
    UPROPERTY(BlueprintAssignable, Category = "Statistics|Events")
    FOnMilestoneReached OnMilestoneReached;
    
private:
    UPROPERTY()
    TMap<FString, FPlayerCareerStats> CareerStats;
    
    UPROPERTY()
    TMap<FString, TArray<FMatchStatistics>> MatchHistory;
    
    UPROPERTY()
    TMap<FString, FMatchStatistics> ActiveMatches;
    
    UPROPERTY()
    TMap<FString, TMap<FString, FFactionStatistics>> FactionStats;
    
    void CalculateWinRate(FPlayerCareerStats& Stats};
    void UpdateFactionStats(const FString& PlayerID, const FString& Faction, bool bIsVictory, float Duration};
    void CheckMilestones(const FString& PlayerID, const FPlayerCareerStats& Stats};
    void SaveStatsToFile(const FString& PlayerID};
    void LoadStatsFromFile(const FString& PlayerID};
    FString GetStatsFilePath(const FString& PlayerID) const;
};

