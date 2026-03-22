#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSStatisticsSystem.generated.h"

/**
 * 統�X��?類�?
 */
UENUM(BlueprintType)
enum class EStatType : uint8
{
    MatchesPlayed         UMETA(DisplayName = "對�X),
    MatchesWon            UMETA(DisplayName = "?�場X),
    MatchesLost           UMETA(DisplayName = "?�場X),
    WinRate               UMETA(DisplayName = "?��?"),
    TotalPlayTime         UMETA(DisplayName = "總�X��X),
    UnitsKilled           UMETA(DisplayName = "消�X��X),
    UnitsLost             UMETA(DisplayName = "?�失?��X),
    BuildingsConstructed  UMETA(DisplayName = "建造建築數"),
    BuildingsDestroyed    UMETA(DisplayName = "?��?建�X),
    ResourcesGathered     UMETA(DisplayName = "?��?資�?總�?"),
    ResourcesSpent        UMETA(DisplayName = "消耗�?源總X),
    TechnologiesResearched UMETA(DisplayName = "?�究科�X),
    APM                   UMETA(DisplayName = "平�?APM"),
    PeakAPM               UMETA(DisplayName = "峰值APM"),
    FavoriteFaction       UMETA(DisplayName = "?�?�勢X),
    FavoriteMap           UMETA(DisplayName = "?�?�地X)
};

/**
 * 對�?統�?
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
 * ?�家?�涯統�?
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
 * ?��?統�?
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
 * 統�?系統 - 記�X��X�玩家統計數X */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSStatisticsSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSStatisticsSystem(};
    
    // ?��X    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void InitializeStatisticsSystem(};
    
    // 記�?對�X��?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void RecordMatchStart(const FString& PlayerID, const FString& MatchID, const FString& MapName, const FString& Faction};
    
    // 記�?對�?結�?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void RecordMatchEnd(const FString& PlayerID, const FString& MatchID, bool bIsVictory, const FMatchStatistics& Stats};
    
    // ?�新實�?統�?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void UpdateLiveStat(const FString& PlayerID, EStatType StatType, float Value};
    
    // 記�X��X�殺
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void RecordUnitKilled(const FString& PlayerID, const FString& MatchID, int32 UnitValue};
    
    // 記�?資�X��?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void RecordResourceGathered(const FString& PlayerID, const FString& MatchID, int32 Amount};
    
    // 記�?APM
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void RecordAPM(const FString& PlayerID, const FString& MatchID, float CurrentAPM};
    
    // ?��X�涯統�?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    FPlayerCareerStats GetCareerStats(const FString& PlayerID) const;
    
    // ?��?對�?歷史
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    TArray<FMatchStatistics> GetMatchHistory(const FString& PlayerID, int32 Count = 10) const;
    
    // ?��X��?統�?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    TArray<FFactionStatistics> GetFactionStats(const FString& PlayerID) const;
    
    // ?��X��?統�X    UFUNCTION(BlueprintCallable, Category = "Statistics")
    float GetStatValue(const FString& PlayerID, EStatType StatType) const;
    
    // ?��?統�?趨勢
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    TArray<float> GetStatTrend(const FString& PlayerID, EStatType StatType, int32 Days = 30) const;
    
    // 比�?統�?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    FString CompareStats(const FString& PlayerID1, const FString& PlayerID2) const;
    
    // ?��X��?榜�?�?    UFUNCTION(BlueprintCallable, Category = "Statistics")
    int32 GetLeaderboardRank(const FString& PlayerID, EStatType StatType) const;
    
    // ?��?統�X��?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    FString GetStatsSummary(const FString& PlayerID) const;
    
    // 導出統�X��?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    bool ExportStatsToCSV(const FString& PlayerID, const FString& FilePath) const;
    
    // ?�置統�?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void ResetStatistics(const FString& PlayerID};
    
    // 保�?統�?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void SaveStatistics(const FString& PlayerID};
    
    // 載入統�?
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void LoadStatistics(const FString& PlayerID};
    
    // 事件委�?
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

