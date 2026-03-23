#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSCompilationAPI.generated.h"

UENUM(BlueprintType)
enum class EAPIRequestType : uint8 {
    Compile,
    Analyze,
    Fix,
    Report,
    Status
};

UENUM(BlueprintType)
enum class EAPIResponseCode : uint8 {
    Success UMETA(DisplayName = "Success"),
    Error UMETA(DisplayName = "Error"),
    Processing UMETA(DisplayName = "Processing"),
    Timeout UMETA(DisplayName = "Timeout"),
    InvalidRequest UMETA(DisplayName = "Invalid Request")
};

USTRUCT(BlueprintType)
struct FAPIRequest
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString RequestID;

    UPROPERTY(BlueprintReadOnly)
    EAPIRequestType RequestType;

    UPROPERTY(BlueprintReadOnly)
    FString Parameters;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    FAPIRequest()
    {
        RequestType = EAPIRequestType::Compile;
        Timestamp = FDateTime::Now();
    }
};

USTRUCT(BlueprintType)
struct FAPIResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString RequestID;

    UPROPERTY(BlueprintReadOnly)
    EAPIResponseCode ResponseCode;

    UPROPERTY(BlueprintReadOnly)
    FString Message;

    UPROPERTY(BlueprintReadOnly)
    FString Data;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    FAPIResponse()
    {
        ResponseCode = EAPIResponseCode::Success;
        Timestamp = FDateTime::Now();
    }
};

USTRUCT(BlueprintType)
struct FCompilationJob
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString JobID;

    UPROPERTY(BlueprintReadOnly)
    FString ProjectPath;

    UPROPERTY(BlueprintReadOnly)
    FString Target;

    UPROPERTY(BlueprintReadOnly)
    FString Configuration;

    UPROPERTY(BlueprintReadOnly)
    bool bIsRunning;

    UPROPERTY(BlueprintReadOnly)
    FDateTime StartTime;

    UPROPERTY(BlueprintReadOnly)
    FDateTime EndTime;

    FCompilationJob()
    {
        bIsRunning = false;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAPIRequest, const FAPIRequest&, Request);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAPIResponse, const FAPIResponse&, Response);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompilationJobStarted, const FString&, JobID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCompilationJobCompleted, const FString&, JobID, bool, bSuccess);

/**
 * MingGoRTS Compilation API
 * 
 * Provides RESTful API interface for self-learning compilation system
 * enabling external tools and scripts to interact with the compiler.
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSCompilationAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSCompilationAPI();

    // ========== API Server Management ==========
    UFUNCTION(BlueprintCallable, Category = "Compilation API")
    bool StartAPIServer(int32 Port = 8080);

    UFUNCTION(BlueprintCallable, Category = "Compilation API")
    void StopAPIServer();

    UFUNCTION(BlueprintPure, Category = "Compilation API")
    bool IsAPIServerRunning() const;

    UFUNCTION(BlueprintPure, Category = "Compilation API")
    int32 GetAPIPort() const;

    // ========== Request Handling ==========
    UFUNCTION(BlueprintCallable, Category = "Compilation API")
    FAPIResponse HandleAPIRequest(const FAPIRequest& Request);

    UFUNCTION(BlueprintCallable, Category = "Compilation API")
    void SendAPIResponse(const FAPIResponse& Response);

    // ========== Compilation Jobs ==========
    UFUNCTION(BlueprintCallable, Category = "Compilation API")
    FString CreateCompilationJob(const FString& ProjectPath, const FString& Configuration);

    UFUNCTION(BlueprintCallable, Category = "Compilation API")
    bool StartCompilationJob(const FString& JobID);

    UFUNCTION(BlueprintCallable, Category = "Compilation API")
    bool StopCompilationJob(const FString& JobID);

    UFUNCTION(BlueprintCallable, Category = "Compilation API")
    FCompilationJob GetCompilationJob(const FString& JobID) const;

    UFUNCTION(BlueprintCallable, Category = "Compilation API")
    TArray<FCompilationJob> GetAllCompilationJobs() const;

    // ========== Learning Integration ==========
    UFUNCTION(BlueprintCallable, Category = "Compilation API")
    void UpdateLearningFromAPI(const FString& LearningData);

    UFUNCTION(BlueprintCallable, Category = "Compilation API")
    FString GetLearningDataAsJSON() const;

    UFUNCTION(BlueprintCallable, Category = "Compilation API")
    void ApplyLearningRules(const FString& RulesJSON);

    // ========== Status and Monitoring ==========
    UFUNCTION(BlueprintPure, Category = "Compilation API")
    FString GetAPIStatus() const;

    UFUNCTION(BlueprintPure, Category = "Compilation API")
    int32 GetActiveJobsCount() const;

    UFUNCTION(BlueprintPure, Category = "Compilation API")
    float GetAverageCompilationTime() const;

    UFUNCTION(BlueprintCallable, Category = "Compilation API")
    void GenerateAPIReport(const FString& ReportPath);

    // ========== Events ==========
    UPROPERTY(BlueprintAssignable)
    FOnAPIRequest OnAPIRequest;

    UPROPERTY(BlueprintAssignable)
    FOnAPIResponse OnAPIResponse;

    UPROPERTY(BlueprintAssignable)
    FOnCompilationJobStarted OnCompilationJobStarted;

    UPROPERTY(BlueprintAssignable)
    FOnCompilationJobCompleted OnCompilationJobCompleted;

protected:
    // ========== API Server ==========
    UPROPERTY()
    bool bAPIServerRunning;

    UPROPERTY()
    int32 APIPort;

    // ========== Job Management ==========
    UPROPERTY()
    TMap<FString, FCompilationJob> CompilationJobs;

    UPROPERTY()
    TArray<FString> ActiveJobs;

    // ========== Statistics ==========
    UPROPERTY()
    TMap<FString, int32> RequestCounts;

    UPROPERTY()
    TArray<float> CompilationTimes;

    UPROPERTY()
    int32 TotalRequests;

    UPROPERTY()
    int32 SuccessfulRequests;

    // ========== Internal Methods ==========
    void ProcessAPIRequest(const FString& RequestJSON);
    FAPIResponse HandleCompileRequest(const FString& Parameters);
    FAPIResponse HandleAnalyzeRequest(const FString& Parameters);
    FAPIResponse HandleFixRequest(const FString& Parameters);
    FAPIResponse HandleReportRequest(const FString& Parameters);
    FAPIResponse HandleStatusRequest(const FString& Parameters);

    // ========== JSON Processing ==========
    FString RequestToJSON(const FAPIRequest& Request) const;
    FAPIResponse JSONToResponse(const FString& ResponseJSON) const;
    TMap<FString, FString> ParseJSONParameters(const FString& JSON) const;
    FString CreateJSONResponse(const FAPIResponse& Response) const;

    // ========== Job Management ==========
    FString GenerateJobID() const;
    void UpdateJobStatus(const FString& JobID, bool bIsRunning, const FDateTime& EndTime = FDateTime::MinValue());
    void CleanupCompletedJobs();

    // ========== Statistics ==========
    void UpdateStatistics(const FString& RequestType, bool bSuccess, float CompilationTime = 0.0f);
    void CalculateAverageCompilationTime();

    // ========== Learning Integration ==========
    void IntegrateWithSelfLearningCompiler();
    void ForwardLearningData(const FString& Data);
    void ReceiveLearningUpdates(const FString& Updates);
};
