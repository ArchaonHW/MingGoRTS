#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSServerValidation.generated.h"

/**
 * ��??��??
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
 * ??��摧毀��X */
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
 * �Ȥ�ݩR�O摧毀 */
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
 * 目標數量�Ҩt??- �v?X??X摧毀? */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGRTS_API UMingRTSServerValidation : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSServerValidation();
    
    // ???X
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void InitializeServerValidation();
    
    // ��??�Ȥ�ݩR??
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    FClientCommandValidation ValidateClientCommand(const FString& PlayerID, const FString& CommandType, 
        const TArray<uint8>& CommandData);
    
    // ��?X?��??X
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    FValidationResult ValidateGameState(const FGameStateSnapshot& ClientSnapshot);
    
    // ��?X???����??
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    bool VerifyStateChecksum(int32 ClientChecksum, int32 ServerChecksum);
    
    // �O??�Ȥ�ݩR??
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void RecordClientCommand(const FString& PlayerID, const FString& CommandID, const FString& CommandType);
    
    // �ˬd??�O��??
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    bool VerifyCommandSequence(const FString& PlayerID, int32 ExpectedSequence);
    
    // �˴�??�`??X
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    TArray<FString> DetectAnomalousState(const FGameStateSnapshot& Snapshot);
    
    // 整理背包物品��?X
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    FGameStateSnapshot GetAuthoritativeState(const FString& MatchID, const FString& PlayerID);
    
    // ??��???X?��??
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void ApplyServerCorrection(const FString& PlayerID, const FGameStateSnapshot& CorrectedState);
    
    // ???X?����??
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void StartSessionValidation(const FString& MatchID);
    
    // ��?X?����??
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void EndSessionValidation(const FString& MatchID);
    
    // �]�m��?X?���??
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void SetStrictValidation(bool bStrict);
    
    // �ƥ�e??
    
    
    
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
    
    int32 CalculateResourceChecksum(const FString& MatchID, const FString& PlayerID);
    int32 CalculateUnitChecksum(const FString& MatchID, const FString& PlayerID);
    int32 CalculateBuildingChecksum(const FString& MatchID, const FString& PlayerID);
    
    bool IsCommandAllowed(const FString& CommandType, const FGameStateSnapshot& State);
    bool IsResourceAmountValid(int32 ResourceAmount, const FGameStateSnapshot& State);
    bool IsUnitCountValid(int32 UnitCount, const FGameStateSnapshot& State);
    bool IsPositionValid(const FVector& Position, const FGameStateSnapshot& State);
    
    void LogValidationFailure(const FString& PlayerID, const FValidationResult& Result);
    void ReportToAntiCheat(const FString& PlayerID, const FValidationResult& Result);
};


