#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSServerValidation.generated.h"

/**
 * È©óË?ÁµêÊ?
 */
USTRUCT(BlueprintType)
struct FValidationResult
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsValid;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ValidationMessage;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ConfidenceScore;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Violations;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Timestamp;
    
    FValidationResult()
        : bIsValid(true)
        , ConfidenceScore(1.0f)
    {}
};

/**
 * ?äÊà≤?Ä?ãÂø´?? */
USTRUCT(BlueprintType)
struct FGameStateSnapshot
{
    GENERATED_BODY()
    
    UPROPERTY()
    FString MatchID;
    
    UPROPERTY()
    FString PlayerID;
    
    UPROPERTY()
    int32 FrameNumber;
    
    UPROPERTY()
    float GameTime;
    
    UPROPERTY()
    int32 ResourceChecksum;
    
    UPROPERTY()
    int32 UnitChecksum;
    
    UPROPERTY()
    int32 BuildingChecksum;
    
    UPROPERTY()
    FString Timestamp;
    
    FGameStateSnapshot()
        : FrameNumber(0)
        , GameTime(0.0f)
        , ResourceChecksum(0)
        , UnitChecksum(0)
        , BuildingChecksum(0)
    {}
};

/**
 * ÂÆ¢Êà∂Á´ØÂëΩ‰ª§È?Ë≠? */
USTRUCT(BlueprintType)
struct FClientCommandValidation
{
    GENERATED_BODY()
    
    UPROPERTY()
    FString CommandID;
    
    UPROPERTY()
    FString PlayerID;
    
    UPROPERTY()
    FString CommandType;
    
    UPROPERTY()
    int32 SequenceNumber;
    
    UPROPERTY()
    float Timestamp;
    
    UPROPERTY()
    bool bIsValid;
    
    UPROPERTY()
    FString RejectionReason;
    
    FClientCommandValidation()
        : SequenceNumber(0)
        , Timestamp(0.0f)
        , bIsValid(true)
    {}
};

/**
 * ?çÂ??®È?Ë≠âÁ≥ªÁµ?- Ê¨äÂ??çÂ??®È?Ë≠? */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSServerValidation : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSServerValidation(};
    
    // ?ùÂ???    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void InitializeServerValidation(};
    
    // È©óË?ÂÆ¢Êà∂Á´ØÂëΩ‰ª?    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    FClientCommandValidation ValidateClientCommand(const FString& PlayerID, const FString& CommandType, 
        const TArray<uint8>& CommandData};
    
    // È©óË??äÊà≤?Ä??    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    FValidationResult ValidateGameState(const FGameStateSnapshot& ClientSnapshot};
    
    // ÊØîË??Ä?ãÊ†°È©óÂ?
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    bool VerifyStateChecksum(int32 ClientChecksum, int32 ServerChecksum};
    
    // Ë®òÈ?ÂÆ¢Êà∂Á´ØÂëΩ‰ª?    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void RecordClientCommand(const FString& PlayerID, const FString& CommandID, const FString& CommandType};
    
    // Ê™¢Êü•?Ω‰ª§Â∫èÂ?
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    bool VerifyCommandSequence(const FString& PlayerID, int32 ExpectedSequence};
    
    // Ê™¢Ê∏¨?∞Â∏∏?Ä??    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    TArray<FString> DetectAnomalousState(const FGameStateSnapshot& Snapshot};
    
    // ?≤Â??çÂ??®Ê?Â®ÅÁ???    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    FGameStateSnapshot GetAuthoritativeState(const FString& MatchID, const FString& PlayerID};
    
    // ?âÁî®?çÂ??®Ê†°Ê≠?    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void ApplyServerCorrection(const FString& PlayerID, const FGameStateSnapshot& CorrectedState};
    
    // ?ãÂ??ÉË©±È©óË?
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void StartSessionValidation(const FString& MatchID};
    
    // ÁµêÊ??ÉË©±È©óË?
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void EndSessionValidation(const FString& MatchID};
    
    // Ë®≠ÁΩÆÈ©óË??¥Ê†ºÊ®°Â?
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void SetStrictValidation(bool bStrict};
    
    // ‰∫ã‰ª∂ÂßîË?
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnValidationFailed, const FString&, PlayerID, const FValidationResult&, Result};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStateMismatch, const FString&, MatchID, const FValidationResult&, MismatchInfo};
    
    UPROPERTY(BlueprintAssignable, Category = "ServerValidation|Events")
    FOnValidationFailed OnValidationFailed;
    
    UPROPERTY(BlueprintAssignable, Category = "ServerValidation|Events")
    FOnStateMismatch OnStateMismatch;
    
private:
    UPROPERTY()
    TMap<FString, FGameStateSnapshot> AuthoritativeStates;
    
    UPROPERTY()
    TMap<FString, TArray<FClientCommandValidation>> CommandHistory;
    
    UPROPERTY()
    TMap<FString, int32> LastSequenceNumbers;
    
    UPROPERTY()
    bool bStrictMode;
    
    UPROPERTY()
    TArray<FString> ActiveSessions;
    
    int32 CalculateResourceChecksum(const FString& MatchID, const FString& PlayerID};
    int32 CalculateUnitChecksum(const FString& MatchID, const FString& PlayerID};
    int32 CalculateBuildingChecksum(const FString& MatchID, const FString& PlayerID};
    
    bool IsCommandAllowed(const FString& CommandType, const FGameStateSnapshot& State};
    bool IsResourceAmountValid(int32 ResourceAmount, const FGameStateSnapshot& State};
    bool IsUnitCountValid(int32 UnitCount, const FGameStateSnapshot& State};
    bool IsPositionValid(const FVector& Position, const FGameStateSnapshot& State};
    
    void LogValidationFailure(const FString& PlayerID, const FValidationResult& Result};
    void ReportToAntiCheat(const FString& PlayerID, const FValidationResult& Result};
};

