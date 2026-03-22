#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSAntiCheat.generated.h"

/**
 * 作??檢測類??
 */
UENUM(BlueprintType)
enum class ECheatType: uint8 {
    None                    UMETA(DisplayName = "無"),
    MemoryModification      UMETA(DisplayName = "記憶體修改"),
    SpeedHack               UMETA(DisplayName = "??度修改"),
    AimBot                  UMETA(DisplayName = "目標數量"),
    MacroScript             UMETA(DisplayName = "腳本/宏"),
    MapHack                 UMETA(DisplayName = "摧毀外??"),
    ResourceHack            UMETA(DisplayName = "資??修改"),
    Injection               UMETA(DisplayName = "注入作??"),
    Bypass                  UMETA(DisplayName = "繞??檢測"),
    Collusion               UMETA(DisplayName = "摧毀作??")
};

/**
 * ??罰等??
 */
UENUM(BlueprintType)
enum class EPenaltyLevel: uint8 {
    Warning         UMETA(DisplayName = "警??"),
    Restriction     UMETA(DisplayName = "??制"),
    Suspension      UMETA(DisplayName = "摧毀封??"),
    PermanentBan    UMETA(DisplayName = "永??封??"),
    LegalAction     UMETA(DisplayName = "法??追訴")
};

/**
 * 檢測置信?? */
UENUM(BlueprintType)
enum class EDetectionConfidence: uint8 {
    VeryLow     UMETA(DisplayName = "很低"),
    Low         UMETA(DisplayName = "低"),
    Medium      UMETA(DisplayName = "中等"),
    High        UMETA(DisplayName = "高"),
    VeryHigh    UMETA(DisplayName = "很高")
};

/**
 * 檢測結??
 */
USTRUCT(BlueprintType)
struct FCheatDetectionResult
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECheatType CheatType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDetectionConfidence Confidence;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MatchID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DetectionDetails;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SeverityScore;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Timestamp;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<uint8> EvidenceData;
    
    FCheatDetectionResult()
        : CheatType(ECheatType::None)
        , Confidence(EDetectionConfidence::Low)
        , SeverityScore(0.0f)
    {}
};

/**
 * ??家行為記??
 */
USTRUCT(BlueprintType)
struct FPlayerBehaviorRecord
{
    GENERATED_BODY()
    
    UPROPERTY()
    FString PlayerID;
    
    UPROPERTY()
    float AverageAPM;
    
    UPROPERTY()
    float MaxAPM;
    
    UPROPERTY()
    float ActionVariance;
    
    UPROPERTY()
    float TimingConsistency;
    
    UPROPERTY()
    int32 SuspiciousActions;
    
    UPROPERTY()
    TArray<float> APMHistory;
    
    UPROPERTY()
    FString LastUpdated;
    
    FPlayerBehaviorRecord()
        : AverageAPM(0.0f)
        , MaxAPM(0.0f)
        , ActionVariance(0.0f)
        , TimingConsistency(0.0f)
        , SuspiciousActions(0)
    {}
};

/**
 * 摧毀弊核心系?? */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGRTS_API UMingRTSAntiCheat : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSAntiCheat();
    
    // ???X
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    void InitializeAntiCheat();
    
    // 摧毀檢測
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    void StartMonitoring();
    
    // ??止檢測
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    void StopMonitoring();
    
    // 目標數量行為
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    void ReportSuspiciousBehavior(const FString& PlayerID, ECheatType Type, const FString& Details);
    
    // 檢測記憶體修X
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    bool DetectMemoryModification(const FString& PlayerID);
    
    // 檢測??度作??
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    bool DetectSpeedHack(const FString& PlayerID, float CurrentSpeed);
    
    // 檢測選項文本
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    bool DetectMacroUsage(const FString& PlayerID, const TArray<float>& ActionTimings);
    
    // ???X?家行為模??
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    void AnalyzePlayerBehavior(const FString& PlayerID);
    
    // 摧毀檢測結??
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    void ProcessDetection(const FCheatDetectionResult& Result);
    
    // ??用??罰
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    void ApplyPenalty(const FString& PlayerID, EPenaltyLevel Penalty, const FString& Reason);
    
    // ???X?家摧毀歷史
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    TArray<FCheatDetectionResult> GetPlayerViolationHistory(const FString& PlayerID) const;
    
    // 檢查??家??否被摧毀
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    bool IsPlayerBanned(const FString& PlayerID) const;
    
    // 摧毀封?? (管?X??X
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    void UnbanPlayer(const FString& PlayerID, const FString& AdminID, const FString& Reason);
    
    // 上傳檢測故事重要性?器
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    void UploadDetectionReport(const FCheatDetectionResult& Result);
    
    // 事件委??
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheatDetected, const FCheatDetectionResult&, Detection);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPlayerPenalized, const FString&, PlayerID, EPenaltyLevel, Penalty, const FText&, Reason);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerBanned, const FString&, PlayerID);
    
    UPROPERTY(BlueprintAssignable, Category = "AntiCheat|Events")
    FOnCheatDetected OnCheatDetected;
    
    UPROPERTY(BlueprintAssignable, Category = "AntiCheat|Events")
    FOnPlayerPenalized OnPlayerPenalized;
    
    UPROPERTY(BlueprintAssignable, Category = "AntiCheat|Events")
    FOnPlayerBanned OnPlayerBanned;
    
private:
    UPROPERTY()
    bool bIsMonitoring;
    
    UPROPERTY()
    TArray<FCheatDetectionResult> DetectionHistory;
    
    UPROPERTY()
    TMap<FString, FPlayerBehaviorRecord> PlayerBehaviorMap;
    
    UPROPERTY()
    TArray<FString> BannedPlayers;
    
    UPROPERTY()
    TMap<FString, EPenaltyLevel> ActivePenalties;
    
    FTimerHandle MonitoringTimerHandle;
    
    void StartBackgroundChecks();
    void StopBackgroundChecks();
    void PerformPeriodicChecks();
    
    bool ValidateGameFiles();
    bool CheckForKnownCheats();
    bool VerifyProcessIntegrity();
    
    float CalculateBehaviorAnomalyScore(const FPlayerBehaviorRecord& Record);
    EPenaltyLevel DeterminePenaltyLevel(const FCheatDetectionResult& Detection);
    
    void LogDetection(const FCheatDetectionResult& Result);
    void NotifyServer(const FCheatDetectionResult& Result);
};

