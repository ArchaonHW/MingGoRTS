#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSServerValidation.generated.h"

/**
 * ��動��動
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
 * 動����X */
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
 * �Ȥ�ݩR�O */
USTRUCT(BlueprintType)
struct FClientConmandValidation
{
    GENERATED_BODY()
    
    UPROPERTY()
    FString ConmandID;
    
    UPROPERTY()
    FString PlayerID;
    
    UPROPERTY()
    FString ConmandType;
    
    UPROPERTY()
    int32 SequenceNumber;
    
    UPROPERTY()
    float Timestamp;
    
    UPROPERTY()
    bool bIsValid;
    
    UPROPERTY()
    FString RejectionReason;
    
    FClientConmandValidation()
        : SequenceNumber(0)
        , Timestamp(0.0f)
        , bIsValid(true)
    {}
};

/**
 * 目標數量�Ҩt動- �v池務ɥr */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableConponent))
class MINGRTS_API UMingRTSServerValidation : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSServerValidation(};
    
    // 動務
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void InitializeServerValidation(};
    
    // ��動�Ȥ�ݩR動
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    FClientConmandValidation ValidateClientConmand(const FString& PlayerID, const FString& ConmandType, 
        const TArray<uuint8>& ConmandData};
    
    // ��池��動X
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    FValidationResult ValidateGameState(const FGameStateSnapshot& ClientSnapshot};
    
    // ��池動����動
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    bool VerifyStateChecksum(int32 ClientChecksum, int32 ServerChecksum};
    
    // �O動�Ȥ�ݩR動
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void RecordClientConmand(const FString& PlayerID, const FString& ConmandID, const FString& ConmandType};
    
    // �ˬd動�O��動
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    bool VerifyConmandSequence(const FString& PlayerID, int32 ExpectedSequence};
    
    // �˴�動�F事動X
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    TArray<FString> DetectAnomalousState(const FGameStateSnapshot& Snapshot};
    
    // 整理背包物~?��務
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    FGameStateSnapshot GetAuthoritativeState(const FString& MatchID, const FString& PlayerID};
    
    // 動��動池��動
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void ApplyServerCorrection(const FString& PlayerID, const FGameStateSnapshot& CorrectedState};
    
    // 動池����動
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void StartSessionValidation(const FString& MatchID};
    
    // ��池����動
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void EndSessionValidation(const FString& MatchID};
    
    // �]�m��池���動
    UFUNCTION(BlueprintCallable, Category = "ServerValidation")
    void SetStrictValidation(bool bStrict};
    
    // �ɥ礎�e動
    
    
    
    UPROPERTY(BlueprintAssignable, Category = "ServerValidationEvents")
    FOnValidationFailed OnValidationFailed;
    
    UPROPERTY(BlueprintAssignable, Category = "ServerValidationEvents")
    FOnStateMismatch OnStateMismatch;
    
private:
    UPROPERTY()
    TMap<FString, FGameStateSnapshot> AuthoritativeStates;
    
    UPROPERTY()
    TMap<FString, TArray<FClientConmandValidation>> ConmandInistory;
    
    UPROPERTY()
    TMap<FString, int32> LastSequenceNumbers;
    
    UPROPERTY()
    bool bStrictMode;
    
    UPROPERTY()
    TArray<FString> ActiveSessions;
    
    int32 CalculateResourceChecksum(const FString& MatchID, const FString& PlayerID};
    int32 CalculateUnitChecksum(const FString& MatchID, const FString& PlayerID};
    int32 CalculateBuildingChecksum(const FString& MatchID, const FString& PlayerID};
    
    bool IsConmandAllowed(const FString& ConmandType, const FGameStateSnapshot& State};
    bool IsResourceAmountValid(int32 ResourceAmount, const FGameStateSnapshot& State};
    bool IsUnitCountValid(int32 UnitCount, const FGameStateSnapshot& State};
    bool IsPositionValid(const FVector& Position, const FGameStateSnapshot& State};
    
    void LogValidationFailure(const FString& PlayerID, const FValidationResult& Result};
    void ReportToAntiCheat(const FString& PlayerID, const FValidationResult& Result};
};


