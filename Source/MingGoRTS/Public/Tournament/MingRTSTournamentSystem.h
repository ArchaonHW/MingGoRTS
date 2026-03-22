#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSTournamentSystem.generated.h"

/**
 * 錦標賽類型
 */
UENUM(BlueprintType)
enum class ETournamentType: uint8 {
    SingleElimination     UMETA(DisplayName = "單敗淘汰"),
    DoubleElimination     UMETA(DisplayName = "雙敗淘汰"),
    RoundRobin            UMETA(DisplayName = "循環賽"),
    Swiss                 UMETA(DisplayName = "瑞士制"),
    League                UMETA(DisplayName = "聯賽"),
    Cup                   UMETA(DisplayName = "杯賽"),
    Invitational          UMETA(DisplayName = "邀請賽"),
    Qualifier             UMETA(DisplayName = "資格賽"),
    Custom                UMETA(DisplayName = "自定義")
};

/**
 * 錦標賽狀態
 */
UENUM(BlueprintType)
enum class ETournamentStatus: uint8 {
    Registration          UMETA(DisplayName = "報名中"),
    Starting              UMETA(DisplayName = "即將開始"),
    InProgress            UMETA(DisplayName = "進行中"),
    Paused                UMETA(DisplayName = "暫停"),
    Finished              UMETA(DisplayName = "已結束"),
    Cancelled             UMETA(DisplayName = "已取消")
};

/**
 * 比賽狀態
 */
UENUM(BlueprintType)
enum class EMatchStatus: uint8 {
    Pending               UMETA(DisplayName = "待定"),
    Scheduled             UMETA(DisplayName = "已排程"),
    InProgress            UMETA(DisplayName = "進行中"),
    Finished              UMETA(DisplayName = "已結束"),
    Forfeited             UMETA(DisplayName = "棄權"),
    Disputed              UMETA(DisplayName = "爭議中")
};

/**
 * 參賽者信息
 */
USTRUCT(BlueprintType)
struct FTournamentParticipant
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Seed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Score;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Wins;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Losses;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Draws;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEliminated;
    
    FTournamentParticipant()
        : Seed(0)
        , Score(0)
        , Wins(0)
        , Losses(0)
        , Draws(0)
        , bEliminated(false)
    {}
};

/**
 * 比賽信息
 */
USTRUCT(BlueprintType)
struct FTournamentMatch
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MatchID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TournamentID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Round;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Player1ID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Player2ID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString WinnerID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Player1Score;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Player2Score;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMatchStatus Status;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ScheduledTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString StartTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EndTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsFinal;
    
    FTournamentMatch()
        : Round(0)
        , Player1Score(0)
        , Player2Score(0)
        , Status(EMatchStatus::Pending)
        , bIsFinal(false)
    {}
};

/**
 * 錦標賽信息
 */
USTRUCT(BlueprintType)
struct FTournamentInfo
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TournamentID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETournamentType Type;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETournamentStatus Status;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HostID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxParticipants;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentParticipants;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RegistrationStart;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RegistrationEnd;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TournamentStart;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TournamentEnd;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 PrizePool;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bRequiresPassword;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Password;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MinRankRequired;
    
    FTournamentInfo()
        : Type(ETournamentType::SingleElimination)
        , Status(ETournamentStatus::Registration)
        , MaxParticipants(32)
        , CurrentParticipants(0)
        , PrizePool(0)
        , bRequiresPassword(false)
        , MinRankRequired(0)
    {}
};

/**
 * 排行榜條目
 */
USTRUCT(BlueprintType)
struct FLeaderboardEntry
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Rank;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Rating;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Wins;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Losses;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TournamentsPlayed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TournamentsWon;
    
    FLeaderboardEntry()
        : Rank(0)
        , Rating(1000)
        , Wins(0)
        , Losses(0)
        , TournamentsPlayed(0)
        , TournamentsWon(0)
    {}
};

/**
 * 競技與錦標賽系統
 */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSTournamentSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSTournamentSystem();
    
    // 初始化
    UFUNCTION(BlueprintCallable, Category = "Tournament")
    void InitializeTournamentSystem();
    
    // 創建錦標賽
    UFUNCTION(BlueprintCallable, Category = "Tournament")
    FString CreateTournament(const FTournamentInfo& Info);
    
    // 刪除錦標賽
    UFUNCTION(BlueprintCallable, Category = "Tournament")
    void DeleteTournament(const FString& TournamentID);
    
    // 報名參加
    UFUNCTION(BlueprintCallable, Category = "Tournament")
    bool RegisterPlayer(const FString& TournamentID, const FString& PlayerID, const FString& Password);
    
    // 取消報名
    UFUNCTION(BlueprintCallable, Category = "Tournament")
    void UnregisterPlayer(const FString& TournamentID, const FString& PlayerID);
    
    // 開始錦標賽
    UFUNCTION(BlueprintCallable, Category = "Tournament")
    bool StartTournament(const FString& TournamentID);
    
    // 結束錦標賽
    UFUNCTION(BlueprintCallable, Category = "Tournament")
    void EndTournament(const FString& TournamentID);
    
    // 提交比賽結果
    UFUNCTION(BlueprintCallable, Category = "Tournament")
    bool SubmitMatchResult(const FString& MatchID, const FString& WinnerID, int32 Score1, int32 Score2);
    
    // 獲取錦標賽列表
    UFUNCTION(BlueprintCallable, Category = "Tournament")
    TArray<FTournamentInfo> GetTournamentList(ETournamentStatus StatusFilter) const;
    
    // 獲取錦標賽詳情
    UFUNCTION(BlueprintCallable, Category = "Tournament")
    bool GetTournamentInfo(const FString& TournamentID, FTournamentInfo& OutInfo) const;
    
    // 獲取參賽者列表
    UFUNCTION(BlueprintCallable, Category = "Tournament")
    TArray<FTournamentParticipant> GetParticipants(const FString& TournamentID) const;
    
    // 獲取比賽列表
    UFUNCTION(BlueprintCallable, Category = "Tournament")
    TArray<FTournamentMatch> GetMatches(const FString& TournamentID) const;
    
    // 獲取排行榜
    UFUNCTION(BlueprintCallable, Category = "Tournament")
    TArray<FLeaderboardEntry> GetLeaderboard(int32 TopCount = 100) const;
    
    // 更新玩家排名
    UFUNCTION(BlueprintCallable, Category = "Tournament")
    void UpdatePlayerRating(const FString& PlayerID, int32 RatingChange);
    
    // 獲取玩家排名
    UFUNCTION(BlueprintCallable, Category = "Tournament")
    int32 GetPlayerRank(const FString& PlayerID) const;
    
    // 獲取玩家錦標賽歷史
    UFUNCTION(BlueprintCallable, Category = "Tournament")
    TArray<FString> GetPlayerTournamentHistory(const FString& PlayerID) const;
    
    // 設置直播房間
    UFUNCTION(BlueprintCallable, Category = "Tournament")
    void SetTournamentSpectatorRoom(const FString& TournamentID, const FString& RoomID);
    
    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTournamentCreated, const FString&, TournamentID, const FTournamentInfo&, Info);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTournamentStarted, const FString&, TournamentID, const FString&, StartTime);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTournamentEnded, const FString&, TournamentID, const FString&, WinnerID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerRegistered, const FString&, TournamentID, const FString&, PlayerID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMatchCompleted, const FString&, MatchID, const FString&, WinnerID, int32, Round);
    
    UPROPERTY(BlueprintAssignable, Category = "Tournament|Events")
    FOnTournamentCreated OnTournamentCreated;
    
    UPROPERTY(BlueprintAssignable, Category = "Tournament|Events")
    FOnTournamentStarted OnTournamentStarted;
    
    UPROPERTY(BlueprintAssignable, Category = "Tournament|Events")
    FOnTournamentEnded OnTournamentEnded;
    
    UPROPERTY(BlueprintAssignable, Category = "Tournament|Events")
    FOnPlayerRegistered OnPlayerRegistered;
    
    UPROPERTY(BlueprintAssignable, Category = "Tournament|Events")
    FOnMatchCompleted OnMatchCompleted;
    
private:
    UPROPERTY()
    TMap<FString, FTournamentInfo> Tournaments;
    
    UPROPERTY()
    TMap<FString, TArray<FTournamentParticipant>> TournamentParticipants;
    
    UPROPERTY()
    TMap<FString, TArray<FTournamentMatch>> TournamentMatches;
    
    UPROPERTY()
    TMap<FString, FLeaderboardEntry> Leaderboard;
    
    UPROPERTY()
    TMap<FString, TArray<FString>> PlayerHistory;
    
    void GenerateBracket(const FString& TournamentID);
    void AdvanceWinner(const FString& MatchID, const FString& WinnerID);
    void UpdateLeaderboard(const FString& PlayerID, bool bWon);
    FString GenerateMatchID();
    int32 CalculateNewRating(int32 CurrentRating, int32 OpponentRating, bool bWon);
};
