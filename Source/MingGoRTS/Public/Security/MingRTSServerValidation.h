#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSServerValidation.generated.h"

/**
 * 驗�?結�?
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
 * ?�戲?�?�快X */
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
 * 客戶端命令�?�? */
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
 * ?��X��?證系�?- 權�X��X��?�? */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSServerValidation : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSServerValidation(};
    
    // ?��X    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void InitializeServerValidation(};
    
    // 驗�?客戶端命�?    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    FClientCommandValidation ValidateClientCommand(const FString& PlayerID, const FString& CommandType, 
        const TArray<uint8>& CommandData};
    
    // 驗�X�戲?�X    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    FValidationResult ValidateGameState(const FGameStateSnapshot& ClientSnapshot};
    
    // 比�X�?�校驗�?
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    bool VerifyStateChecksum(int32 ClientChecksum, int32 ServerChecksum};
    
    // 記�?客戶端命�?    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void RecordClientCommand(const FString& PlayerID, const FString& CommandID, const FString& CommandType};
    
    // 檢查?�令序�?
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    bool VerifyCommandSequence(const FString& PlayerID, int32 ExpectedSequence};
    
    // 檢測?�常?�X    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    TArray<FString> DetectAnomalousState(const FGameStateSnapshot& Snapshot};
    
    // ?��X��X��?威�X    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    FGameStateSnapshot GetAuthoritativeState(const FString& MatchID, const FString& PlayerID};
    
    // ?�用?��X�校�?    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void ApplyServerCorrection(const FString& PlayerID, const FGameStateSnapshot& CorrectedState};
    
    // ?��X�話驗�?
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void StartSessionValidation(const FString& MatchID};
    
    // 結�X�話驗�?
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void EndSessionValidation(const FString& MatchID};
    
    // 設置驗�X�格模�?
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void SetStrictValidation(bool bStrict};
    
    // 事件委�?
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

