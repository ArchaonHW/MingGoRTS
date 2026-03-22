#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSAntiCheat.generated.h"

/**
 * ‰ΩúÂ?Ê™¢Ê∏¨È°ûÂ?
 */
UENUM(BlueprintType)
enum class ECheatType : uint8
{
    None                    UMETA(DisplayName = "??),
    MemoryModification      UMETA(DisplayName = "Ë®òÊÜ∂È´î‰øÆ??),
    SpeedHack               UMETA(DisplayName = "?üÂ∫¶‰øÆÊîπ"),
    AimBot                  UMETA(DisplayName = "?™Â??ÑÊ?"),
    MacroScript             UMETA(DisplayName = "?≥Êú¨/ÂÆ?),
    MapHack                 UMETA(DisplayName = "?∞Â?Â§ñÊ?"),
    ResourceHack            UMETA(DisplayName = "Ë≥áÊ?‰øÆÊîπ"),
    Injection               UMETA(DisplayName = "Ê≥®ÂÖ•‰ΩúÂ?"),
    Bypass                  UMETA(DisplayName = "ÁπûÈ?Ê™¢Ê∏¨"),
    Collusion               UMETA(DisplayName = "?îÂ?‰ΩúÂ?")
};

/**
 * ?ïÁΩ∞Á≠âÁ?
 */
UENUM(BlueprintType)
enum class EPenaltyLevel : uint8
{
    Warning         UMETA(DisplayName = "Ë≠¶Â?"),
    Restriction     UMETA(DisplayName = "?êÂà∂"),
    Suspension      UMETA(DisplayName = "?´Ê?Â∞ÅÁ?"),
    PermanentBan    UMETA(DisplayName = "Ê∞∏‰?Â∞ÅÁ?"),
    LegalAction     UMETA(DisplayName = "Ê≥ïÂ?ËøΩË®¥")
};

/**
 * Ê™¢Ê∏¨ÁΩÆ‰ø°Â∫? */
UENUM(BlueprintType)
enum class EDetectionConfidence : uint8
{
    VeryLow     UMETA(DisplayName = "Âæà‰?"),
    Low         UMETA(DisplayName = "‰Ω?),
    Medium      UMETA(DisplayName = "‰∏≠Á?"),
    High        UMETA(DisplayName = "È´?),
    VeryHigh    UMETA(DisplayName = "ÂæàÈ?")
};

/**
 * Ê™¢Ê∏¨ÁµêÊ?
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
 * ?©ÂÆ∂Ë°åÁÇ∫Ë®òÈ?
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
 * ?ç‰?ÂºäÊ†∏ÂøÉÁ≥ªÁµ? */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSAntiCheat : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSAntiCheat(};
    
    // ?ùÂ???    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    void InitializeAntiCheat(};
    
    // ?üÂ?Ê™¢Ê∏¨
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    void StartMonitoring(};
    
    // ?úÊ≠¢Ê™¢Ê∏¨
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    void StopMonitoring(};
    
    // ?±Â??ØÁ?Ë°åÁÇ∫
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    void ReportSuspiciousBehavior(const FString& PlayerID, ECheatType Type, const FString& Details};
    
    // Ê™¢Ê∏¨Ë®òÊÜ∂È´î‰øÆ??    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    bool DetectMemoryModification(const FString& PlayerID};
    
    // Ê™¢Ê∏¨?üÂ∫¶‰ΩúÂ?
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    bool DetectSpeedHack(const FString& PlayerID, float CurrentSpeed};
    
    // Ê™¢Ê∏¨?™Â??≥Êú¨
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    bool DetectMacroUsage(const FString& PlayerID, const TArray<float>& ActionTimings};
    
    // ?ÜÊ??©ÂÆ∂Ë°åÁÇ∫Ê®°Â?
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    void AnalyzePlayerBehavior(const FString& PlayerID};
    
    // ?ïÁ?Ê™¢Ê∏¨ÁµêÊ?
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    void ProcessDetection(const FCheatDetectionResult& Result};
    
    // ?âÁî®?ïÁΩ∞
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    void ApplyPenalty(const FString& PlayerID, EPenaltyLevel Penalty, const FString& Reason};
    
    // ?≤Â??©ÂÆ∂?ïË?Ê≠∑Âè≤
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    TArray<FCheatDetectionResult> GetPlayerViolationHistory(const FString& PlayerID) const;
    
    // Ê™¢Êü•?©ÂÆ∂?ØÂê¶Ë¢´Â?Á¶?    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    bool IsPlayerBanned(const FString& PlayerID) const;
    
    // Ëß?ô§Â∞ÅÁ? (ÁÆ°Á??°Â???
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    void UnbanPlayer(const FString& PlayerID, const FString& AdminID, const FString& Reason};
    
    // ‰∏äÂÇ≥Ê™¢Ê∏¨?±Â??∞Ê??ôÂô®
    UFUNCTION(BlueprintCallable, Category = "AntiCheat")
    void UploadDetectionReport(const FCheatDetectionResult& Result};
    
    // ‰∫ã‰ª∂ÂßîË?
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheatDetected, const FCheatDetectionResult&, Detection};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPlayerPenalized, const FString&, PlayerID, EPenaltyLevel, Penalty, const FText&, Reason};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerBanned, const FString&, PlayerID};
    
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
    
    void StartBackgroundChecks(};
    void StopBackgroundChecks(};
    void PerformPeriodicChecks(};
    
    bool ValidateGameFiles(};
    bool CheckForKnownCheats(};
    bool VerifyProcessIntegrity(};
    
    float CalculateBehaviorAnomalyScore(const FPlayerBehaviorRecord& Record};
    EPenaltyLevel DeterminePenaltyLevel(const FCheatDetectionResult& Detection};
    
    void LogDetection(const FCheatDetectionResult& Result};
    void NotifyServer(const FCheatDetectionResult& Result};
};

