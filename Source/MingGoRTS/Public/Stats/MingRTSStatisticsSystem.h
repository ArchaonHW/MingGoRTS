#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSStatisticsSystem.generated.h"

/**
 * 参?X???摸??
 */
UENUM(BlueprintType)
enum class EStatType : uint8
{
    MatchesPlayed,         UMETA(DisplayName = "癸驹初Ω"),
    MatchesWon,            UMETA(DisplayName = "秤初Ω"),
    MatchesLost,           UMETA(DisplayName = "ア毖初Ω"),
    WinRate,               UMETA(DisplayName = "秤瞯"),
    TotalPlayTime,         UMETA(DisplayName = "羆笴栏丁"),
    UnitsKilled,           UMETA(DisplayName = "防虫计"),
    UnitsLost,             UMETA(DisplayName = "穕ア虫计秖"),
    BuildingsConstructed,  UMETA(DisplayName = "硑縱计秖"),
    BuildingsDestroyed,    UMETA(DisplayName = "篟反縱计秖"),
    ResourcesGathered,     UMETA(DisplayName = "Μ栋戈方羆计"),
    ResourcesSpent,        UMETA(DisplayName = "戈方羆计"),
    TechnologiesResearched, UMETA(DisplayName = "╯м计"),
    APM UMETA(DisplayName = "キА–だ牧巨计"),
    PeakAPM UMETA(DisplayName = "畃–だ牧巨计"),
    FavoriteFaction UMETA(DisplayName = "程稲皚犁"),
    FavoriteMap UMETA(DisplayName = "程稲瓜")
};

/**
 * 癸??参??
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
 * ??產??睵参??
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
 * ????参??
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
 * 参??╰参 - 癘?X??X?產参璸计X */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSStatisticsSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSStatisticsSystem();
    
    // ???X    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void InitializeStatisticsSystem();
    
    // 癘??癸?X???
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void RecordMatchStart(const FString& PlayerID, const FString& MatchID, const FString& MapName, const FString& Faction};
    
    // 癘??癸??挡??
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void RecordMatchEnd(const FString& PlayerID, const FString& MatchID, bool bIsVictory, const FMatchStatistics& Stats};
    
    // ??穝龟??参??
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void UpdateLiveStat(const FString& PlayerID, EStatType StatType, float Value};
    
    // 癘?X??X?炳
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void RecordUnitKilled(const FString& PlayerID, const FString& MatchID, int32 UnitValue};
    
    // 癘??戈?X???
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void RecordResourceGathered(const FString& PlayerID, const FString& MatchID, int32 Amount};
    
    // 癘??APM
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void RecordAPM(const FString& PlayerID, const FString& MatchID, float CurrentAPM};
    
    // ???X?睵参??
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    FPlayerCareerStats GetCareerStats(const FString& PlayerID) const;
    
    // ????癸??菌
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    TArray<FMatchStatistics> GetMatchHistory(const FString& PlayerID, int32 Count = 10) const;
    
    // ???X???参??
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    TArray<FFactionStatistics> GetFactionStats(const FString& PlayerID) const;
    
    // ???X???参?X    UFUNCTION(BlueprintCallable, Category = "Statistics")
    float GetStatValue(const FString& PlayerID, EStatType StatType) const;
    
    // ????参??镣墩
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    TArray<float> GetStatTrend(const FString& PlayerID, EStatType StatType, int32 Days = 30) const;
    
    // ゑ??参??
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    FString CompareStats(const FString& PlayerID1, const FString& PlayerID2) const;
    
    // ???X???篯????    UFUNCTION(BlueprintCallable, Category = "Statistics")
    int32 GetLeaderboardRank(const FString& PlayerID, EStatType StatType) const;
    
    // ????参?X???
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    FString GetStatsSummary(const FString& PlayerID) const;
    
    // 旧参?X???
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    bool ExportStatsToCSV(const FString& PlayerID, const FString& FilePath) const;
    
    // ??竚参??
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void ResetStatistics(const FString& PlayerID};
    
    // 玂??参??
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void SaveStatistics(const FString& PlayerID};
    
    // 更参??
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void LoadStatistics(const FString& PlayerID};
    
    // ㄆン〆??
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

