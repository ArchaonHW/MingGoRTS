// Copyright (c) 2026 MingGoRTS. All rights reserved.
// ??????t - ???X??X?X
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "UserFeedbackSystem.generated.h"

/**
 * ?????X???
 */
UENUM(BlueprintType)
enum class EFeedbackType: uint8 {
    Bug			UMETA(DisplayName = "Bug Report"),
    Feature		UMETA(DisplayName = "Feature Request"),
    Balance		UMETA(DisplayName = "Balance Issue"),
    Performance	UMETA(DisplayName = "Performance Issue"),
    UI			UMETA(DisplayName = "UI/UX Issue"),
    Audio		UMETA(DisplayName = "Audio Issue"),
    Localization	UMETA(DisplayName = "Localization Issue"),
    Other		UMETA(DisplayName = "Other")
};

/**
 * ???X????X */
UENUM(BlueprintType)
enum class EFeedbackPriority: uint8 {
    Low		UMETA(DisplayName = "Low"),
    Medium	UMETA(DisplayName = "Medium"),
    High	UMETA(DisplayName = "High"),
    Critical	UMETA(DisplayName = "Critical")
};

/**
 * ???X????X */
UENUM(BlueprintType)
enum class EFeedbackStatus: uint8 {
    New		UMETA(DisplayName = "New"),
    InProgress	UMETA(DisplayName = "In Progress"),
    Reviewed	UMETA(DisplayName = "Reviewed"),
    Resolved	UMETA(DisplayName = "Resolved"),
    Rejected	UMETA(DisplayName = "Rejected"),
    Closed		UMETA(DisplayName = "Closed")
};

/**
 * ???X?????
 */
USTRUCT(BlueprintType)
struct FFeedbackData
{
    GENERATED_BODY()

    /** ????ID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString FeedbackID;

    /** ?????? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    EFeedbackType FeedbackType;

    /** ???X?????*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    EFeedbackPriority Priority;

    /** ???X?X*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    EFeedbackStatus Status;

    /** ?? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString Title;

    /** ??z */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString Description;

    /** ??{B?? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString ReproductionSteps;

    /** ?????? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString ExpectedResult;

    /** ???? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString ActualResult;

    /** ??H */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString UserInfo;

    /** tΫH */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString SystemInfo;

    /** ???? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString GameVersion;

    /** ?????? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FDateTime SubmitTime;

    /** ??s???? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FDateTime LastUpdateTime;

    /** ????H */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString AssignedTo;

    /**  */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    TArray<FString> Tags;

    /** ?? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    TArray<FString> Attachments;

    /** ?? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    float Rating;

    /** ??_w?? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    bool bIsRead;

    FFeedbackData()
        : FeedbackID(TEXT(""))
        , FeedbackType(EFeedbackType::Other)
        , Priority(EFeedbackPriority::Medium)
        , Status(EFeedbackStatus::New)
        , Title(TEXT(""))
        , Description(TEXT(""))
        , ReproductionSteps(TEXT(""))
        , ExpectedResult(TEXT(""))
        , ActualResult(TEXT(""))
        , UserInfo(TEXT(""))
        , SystemInfo(TEXT(""))
        , GameVersion(TEXT(""))
        , SubmitTime(FDateTime::Now())
        , LastUpdateTime(FDateTime::Now())
        , AssignedTo(TEXT(""))
        , Rating(0.0f)
        , bIsRead(false)
    {
    }
};

/**
 * ?????X???
 */
USTRUCT(BlueprintType)
struct FFeedbackStatistics
{
    GENERATED_BODY()

    /** `??X */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    int32 TotalFeedback;

    /** ???X???*/
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    TMap<EFeedbackType, int32> FeedbackByType;

    /** ??u?????? */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    TMap<EFeedbackPriority, int32> FeedbackByPriority;

    /** ???X???*/
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    TMap<EFeedbackStatus, int32> FeedbackByStatus;

    /** ?X??X??? */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    float AverageResponseTime;

    /** ????X*/
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    float ResolutionRate;

    /** ??????*/
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    float UserSatisfaction;

    FFeedbackStatistics()
        : TotalFeedback(0)
        , AverageResponseTime(0.0f)
        , ResolutionRate(0.0f)
        , UserSatisfaction(0.0f)
    {
    }
};

/**
 * ??????t
 */
UCLASS(BlueprintType, Blueprintable)
class USERFEEDBACKSYSTEM_API UUserFeedbackSystem : public UObject
{
    GENERATED_BODY()

public:
    UUserFeedbackSystem();

    /** ???X???Xt??*/
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    void InitializeFeedbackSystem();

    /** ?????? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FString SubmitFeedback(const FFeedbackData& FeedbackData);

    /** ???X??X? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    TArray<FFeedbackData> GetFeedbackList(EFeedbackType FilterType = EFeedbackType::Other, EFeedbackStatus FilterStatus = EFeedbackStatus::New);

    /** ???X????? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FFeedbackData GetFeedbackDetails(const FString& FeedbackID);

    /** ??s???X?X*/
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    bool UpdateFeedbackStatus(const FString& FeedbackID, EFeedbackStatus NewStatus);

    /** ???X??? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    bool AssignFeedback(const FString& FeedbackID, const FString& AssignedTo);

    /** K???? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    bool AddComment(const FString& FeedbackID, const FString& Comment);

    /** ?????X??? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FFeedbackStatistics GetStatistics();

    /** ɥX???X??? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FString ExportFeedbackData();

    /** ???X??X??? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    void AutoCategorizeFeedback();

    /** ??eq */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    void SendNotification(const FString& FeedbackID, const FString& Message);

    /** ˬd???X??? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    TArray<FString> CheckDuplicateFeedback(const FFeedbackData& FeedbackData);

    /** ???X??X??? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FString GenerateFeedbackReport();

public:
    /** ???X?ƥ */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFeedbackSubmitted, const FString&, FeedbackID);

    /** ???X?????????*/
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFeedbackStatusUpdated, const FString&, FeedbackID, EFeedbackStatus, NewStatus);

    /** ???X???ƥ */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFeedbackAssigned, const FString&, FeedbackID, const FString&, AssignedTo);

    /** ????Xqƥ */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewFeedback, const FFeedbackData&, FeedbackData);

    UPROPERTY(BlueprintAssignable, Category = "Feedback System|Events")
    FOnFeedbackSubmitted OnFeedbackSubmitted;

    UPROPERTY(BlueprintAssignable, Category = "Feedback System|Events")
    FOnFeedbackStatusUpdated OnFeedbackStatusUpdated;

    UPROPERTY(BlueprintAssignable, Category = "Feedback System|Events")
    FOnFeedbackAssigned OnFeedbackAssigned;

    UPROPERTY(BlueprintAssignable, Category = "Feedback System|Events")
    FOnNewFeedback OnNewFeedback;

protected:
    /** ???X???ID */
    FString GenerateFeedbackID();

    /** ?X??X??? */
    bool ValidateFeedbackData(const FFeedbackData& FeedbackData);

    /** O?X??X??? */
    bool SaveFeedbackData(const FFeedbackData& FeedbackData);

    /** ???X??X??? */
    void LoadFeedbackData();

    /** ??s?X??? */
    void UpdateStatistics();

    /** ??elq */
    void SendEmailNotification(const FString& To, const FString& Subject, const FString& Body);

    /** ???X??X?e */
    void AnalyzeFeedbackContent(FFeedbackData& FeedbackData);

    /** ?????? */
    void AutoTagFeedback(FFeedbackData& FeedbackData);

private:
    /** ??????XX*/
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    TArray<FFeedbackData> AllFeedback;

    /** ?X??? */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    FFeedbackStatistics Statistics;

    /** t??_w??l?? */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    bool bInitialized;

    /** ???X??X??? */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    FString FeedbackDataFilePath;

    /** ??m]m */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    bool bEnableEmailNotifications;

    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    bool bEnableAutoCategorization;

    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    bool bEnableDuplicateDetection;
};

/**
 * ????UI?X */
UCLASS(BlueprintType, Blueprintable)
class USERFEEDBACKSYSTEM_API UFeedbackUIManager : public UObject
{
    GENERATED_BODY()

public:
    UFeedbackUIManager();

    /** ???X??? */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void ShowFeedbackSubmitUI();

    /** ???X??? */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void ShowFeedbackListUI();

    /** ?????X? */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void ShowFeedbackDetailsUI(const FString& FeedbackID);

    /** ܲ?X? */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void ShowStatisticsUI();

    /** ???X??X? */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void CloseFeedbackUI();

    /** ]m????t */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void SetFeedbackSystem(UUserFeedbackSystem* InFeedbackSystem);

protected:
    /** ?????X??? */
    void CreateFeedbackSubmitUI();

    /** ?????X??? */
    void CreateFeedbackListUI();

    /** ???????X? */
    void CreateFeedbackDetailsUI();

    /** ??ز?X? */
    void CreateStatisticsUI();

private:
    /** ????tΤޥ */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UUserFeedbackSystem> FeedbackSystem;

    /** ???????X*/
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    class UUserWidget* CurrentUI;

    /** ???X??? */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UUserWidget> FeedbackSubmitUIClass;

    /** ???X??? */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UUserWidget> FeedbackListUIClass;

    /** ?????X? */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UUserWidget> FeedbackDetailsUIClass;

    /** ?X? */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UUserWidget> StatisticsUIClass;
};