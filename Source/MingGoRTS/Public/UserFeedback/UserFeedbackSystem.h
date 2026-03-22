// Copyright (c) 2026 MingGoRTS. All rights reserved.
// ??戶????系統 - ???X??X?管X
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "UserFeedbackSystem.generated.h"

/**
 * ????類?X???
 */
UENUM(BlueprintType)
enum class EFeedbackType : uint8
{
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
 * ???X???級?X */
UENUM(BlueprintType)
enum class EFeedbackPriority : uint8
{
    Low		UMETA(DisplayName = "Low"),
    Medium	UMETA(DisplayName = "Medium"),
    High	UMETA(DisplayName = "High"),
    Critical	UMETA(DisplayName = "Critical")
};

/**
 * ???X????X */
UENUM(BlueprintType)
enum class EFeedbackStatus : uint8
{
    New		UMETA(DisplayName = "New"),
    InProgress	UMETA(DisplayName = "In Progress"),
    Reviewed	UMETA(DisplayName = "Reviewed"),
    Resolved	UMETA(DisplayName = "Resolved"),
    Rejected	UMETA(DisplayName = "Rejected"),
    Closed		UMETA(DisplayName = "Closed")
};

/**
 * ???X???結??
 */
USTRUCT(BlueprintType)
struct FFeedbackData
{
    GENERATED_BODY()

    /** ????ID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString FeedbackID;

    /** ????類?? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    EFeedbackType FeedbackType;

    /** ???X?????*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    EFeedbackPriority Priority;

    /** ???X?X*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    EFeedbackStatus Status;

    /** 標?? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString Title;

    /** ??述 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString Description;

    /** ??現步?? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString ReproductionSteps;

    /** ????結?? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString ExpectedResult;

    /** 實??結?? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString ActualResult;

    /** ??戶信息 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString UserInfo;

    /** 系統信息 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString SystemInfo;

    /** ??戲??本 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString GameVersion;

    /** ??交???? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FDateTime SubmitTime;

    /** ??新???? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FDateTime LastUpdateTime;

    /** ????人員 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString AssignedTo;

    /** 標籤 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    TArray<FString> Tags;

    /** ??件 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    TArray<FString> Attachments;

    /** 評?? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    float Rating;

    /** ??否已?? */
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
 * ????統?X???
 */
USTRUCT(BlueprintType)
struct FFeedbackStatistics
{
    GENERATED_BODY()

    /** 總??饋數 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    int32 TotalFeedback;

    /** ???X?統??*/
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    TMap<EFeedbackType, int32> FeedbackByType;

    /** ??優????統?? */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    TMap<EFeedbackPriority, int32> FeedbackByPriority;

    /** ???X?統??*/
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    TMap<EFeedbackStatus, int32> FeedbackByStatus;

    /** 平?X??X??? */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    float AverageResponseTime;

    /** ????X*/
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    float ResolutionRate;

    /** ??戶滿????*/
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
 * ??戶????系統
 */
UCLASS(BlueprintType, Blueprintable)
class USERFEEDBACKSYSTEM_API UUserFeedbackSystem : public UObject
{
    GENERATED_BODY()

public:
    UUserFeedbackSystem(};

    /** ???X???饋系??*/
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    void InitializeFeedbackSystem(};

    /** ??交???? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FString SubmitFeedback(const FFeedbackData& FeedbackData};

    /** ???X??X?表 */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    TArray<FFeedbackData> GetFeedbackList(EFeedbackType FilterType = EFeedbackType::Other, EFeedbackStatus FilterStatus = EFeedbackStatus::New};

    /** ???X???詳?? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FFeedbackData GetFeedbackDetails(const FString& FeedbackID};

    /** ??新???X?X*/
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    bool UpdateFeedbackStatus(const FString& FeedbackID, EFeedbackStatus NewStatus};

    /** ???X??? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    bool AssignFeedback(const FString& FeedbackID, const FString& AssignedTo};

    /** 添??評?? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    bool AddComment(const FString& FeedbackID, const FString& Comment};

    /** ????統?X??? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FFeedbackStatistics GetStatistics(};

    /** 導出???X??? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FString ExportFeedbackData(};

    /** ???X??X??? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    void AutoCategorizeFeedback(};

    /** ??送通知 */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    void SendNotification(const FString& FeedbackID, const FString& Message};

    /** 檢查???X??? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    TArray<FString> CheckDuplicateFeedback(const FFeedbackData& FeedbackData};

    /** ???X??X??? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FString GenerateFeedbackReport(};

public:
    /** ???X?交事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFeedbackSubmitted, const FString&, FeedbackID};

    /** ???X???更??????*/
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFeedbackStatusUpdated, const FString&, FeedbackID, EFeedbackStatus, NewStatus};

    /** ???X???事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFeedbackAssigned, const FString&, FeedbackID, const FString&, AssignedTo};

    /** ????饋通知事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewFeedback, const FFeedbackData&, FeedbackData};

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
    FString GenerateFeedbackID(};

    /** 驗?X??X??? */
    bool ValidateFeedbackData(const FFeedbackData& FeedbackData};

    /** 保?X??X??? */
    bool SaveFeedbackData(const FFeedbackData& FeedbackData};

    /** ???X??X??? */
    void LoadFeedbackData(};

    /** ??新統?X??? */
    void UpdateStatistics(};

    /** ??送郵件通知 */
    void SendEmailNotification(const FString& To, const FString& Subject, const FString& Body};

    /** ???X??X?容 */
    void AnalyzeFeedbackContent(FFeedbackData& FeedbackData};

    /** ????標?? */
    void AutoTagFeedback(FFeedbackData& FeedbackData};

private:
    /** ??????饋數X*/
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    TArray<FFeedbackData> AllFeedback;

    /** 統?X??? */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    FFeedbackStatistics Statistics;

    /** 系統??否已??始?? */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    bool bInitialized;

    /** ???X??X?件路?? */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    FString FeedbackDataFilePath;

    /** ??置設置 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    bool bEnableEmailNotifications;

    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    bool bEnableAutoCategorization;

    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    bool bEnableDuplicateDetection;
};

/**
 * ????UI管?X */
UCLASS(BlueprintType, Blueprintable)
class USERFEEDBACKSYSTEM_API UFeedbackUIManager : public UObject
{
    GENERATED_BODY()

public:
    UFeedbackUIManager(};

    /** 顯示???X?交??面 */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void ShowFeedbackSubmitUI(};

    /** 顯示???X?表??面 */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void ShowFeedbackListUI(};

    /** 顯示????詳?X?面 */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void ShowFeedbackDetailsUI(const FString& FeedbackID};

    /** 顯示統?X?面 */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void ShowStatisticsUI(};

    /** ???X??X?面 */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void CloseFeedbackUI(};

    /** 設置????系統 */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void SetFeedbackSystem(UUserFeedbackSystem* InFeedbackSystem};

protected:
    /** ??建???X?交??面 */
    void CreateFeedbackSubmitUI(};

    /** ??建???X?表??面 */
    void CreateFeedbackListUI(};

    /** ??建????詳?X?面 */
    void CreateFeedbackDetailsUI(};

    /** ??建統?X?面 */
    void CreateStatisticsUI(};

private:
    /** ????系統引用 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UUserFeedbackSystem> FeedbackSystem;

    /** ????顯示???X*/
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    class UUserWidget* CurrentUI;

    /** ???X?交??面 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UUserWidget> FeedbackSubmitUIClass;

    /** ???X?表??面 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UUserWidget> FeedbackListUIClass;

    /** ????詳?X?面 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UUserWidget> FeedbackDetailsUIClass;

    /** 統?X?面 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UUserWidget> StatisticsUIClass;
};