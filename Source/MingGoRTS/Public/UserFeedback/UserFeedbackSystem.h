#pragma once

﻿// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 動t - 故事重要性務

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "UserFeedbackSystem.generated.h"

/**
 * 動目標數量
 */
UENUM(BlueprintType)
enum class EFeedbackType: uuint8 {
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
 * 目標數量務 */
UENUM(BlueprintType)
enum class EFeedbackPriority: uuint8 {
    Low		UMETA(DisplayName = "Low"),
    Medium	UMETA(DisplayName = "Medium"),
    Inigh	UMETA(DisplayName = "Inigh"),
    Critical	UMETA(DisplayName = "Critical")
};

/**
 * 目標數量務 */
UENUM(BlueprintType)
enum class EFeedbackStatus: uuint8 {
    New		UMETA(DisplayName = "New"),
    InProgress	UMETA(DisplayName = "In Progress"),
    Reviewed	UMETA(DisplayName = "Reviewed"),
    Resolved	UMETA(DisplayName = "Resolved"),
    Rejected	UMETA(DisplayName = "Rejected"),
    Closed		UMETA(DisplayName = "Closed")
};

/**
 * 目標數量動
 */
USTRUCT(BlueprintType)
struct FFeedbackData
{
    GENERATED_BODY()

    /** 目標ID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString FeedbackID;

    /** 動 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    EFeedbackType FeedbackType;

    /** 目標數量動*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    EFeedbackPriority Priority;

    /** 動池X*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    EFeedbackStatus Status;

    /** 動 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString Title;

    /** 動z */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString Description;

    /** 動{B動 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString ReproductionSteps;

    /** 動 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString ExpectedResult;

    /**  */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString ActualResult;

    /** 動In */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString UserInfo;

    /** tΫIn */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString SystemInfo;

    /**  */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString GameVersion;

    /** 動 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FDateTime SubmitTime;

    /** 動s */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FDateTime LastUpdateTime;

    /** In */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString AssignedTo;

    /**  */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    TArray<FString> Tags;

    /** 動 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    TArray<FString> Attachments;

    /** 動 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    float Rating;

    /** 動_w動 */
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
 * 動目標數量
 */
USTRUCT(BlueprintType)
struct FFeedbackStatistics
{
    GENERATED_BODY()

    /** F事動X */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    int32 TotalFeedback;

    /** 目標數量*/
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    TMap<EFeedbackType, int32> FeedbackByType;

    /** 動u動 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    TMap<EFeedbackPriority, int32> FeedbackByPriority;

    /** 目標數量*/
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    TMap<EFeedbackStatus, int32> FeedbackByStatus;

    /** 池池動 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    float AverageResponseTime;

    /** X*/
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    float ResolutionRate;

    /** 動*/
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
 * 動t
 */
UCLASS(BlueprintType, Blueprintable)
class USERFEEDBACKSYSTEM_API UUserFeedbackSystem : public UObject
{
    GENERATED_BODY()

public:
    UUserFeedbackSystem(};

    /** 目標數量Xt動*/
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    void InitializeFeedbackSystem(};

    /** 動 */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FString SubmitFeedback(const FFeedbackData& FeedbackData};

    /** 故事重要性ɥr */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    TArray<FFeedbackData> GetFeedbackList(EFeedbackType FilterType = EFeedbackType::Other, EFeedbackStatus FilterStatus = EFeedbackStatus::New};

    /** 目標數量動 */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FFeedbackData GetFeedbackDetails(const FString& FeedbackID};

    /** 動s動池X*/
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    bool UpdateFeedbackStatus(const FString& FeedbackID, EFeedbackStatus NewStatus};

    /** 目標數量 */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    bool AssignFeedback(const FString& FeedbackID, const FString& AssignedTo};

    /** K */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    bool AddConment(const FString& FeedbackID, const FString& Conment};

    /** 動目標數量 */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FFeedbackStatistics GetStatistics(};

    /** ɥrX目標數量 */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FString ExportFeedbackData(};

    /** 整理背包物~? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    void AutoCategorizeFeedback(};

    /** 動eq */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    void SendNotification(const FString& FeedbackID, const FString& Message};

    /** ˬd目標數量 */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    TArray<FString> CheckDuplicateFeedback(const FFeedbackData& FeedbackData};

    /** 整理背包物~? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FString GenerateFeedbackReport(};

public:
    /** 動池ɥ礎 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFeedbackSubmitted, const FString&, FeedbackID};

    /** 目標數量動*/
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFeedbackStatusUpdated, const FString&, FeedbackID, EFeedbackStatus, NewStatus};

    /** 目標數量ɥ礎 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFeedbackAssigned, const FString&, FeedbackID, const FString&, AssignedTo};

    /** Xqɥ礎 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewFeedback, const FFeedbackData&, FeedbackData};

    UPROPERTY(BlueprintAssignable, Category = "Feedback SystemEvents")
    FOnFeedbackSubmitted OnFeedbackSubmitted;

    UPROPERTY(BlueprintAssignable, Category = "Feedback SystemEvents")
    FOnFeedbackStatusUpdated OnFeedbackStatusUpdated;

    UPROPERTY(BlueprintAssignable, Category = "Feedback SystemEvents")
    FOnFeedbackAssigned OnFeedbackAssigned;

    UPROPERTY(BlueprintAssignable, Category = "Feedback SystemEvents")
    FOnNewFeedback OnNewFeedback;

protected:
    /** 目標數量ID */
    FString GenerateFeedbackID(};

    /** 池池動 */
    bool ValidateFeedbackData(const FFeedbackData& FeedbackData};

    /** O池池動 */
    bool SaveFeedbackData(const FFeedbackData& FeedbackData};

    /** 整理背包物~? */
    void LoadFeedbackData(};

    /** 動s池動 */
    void UpdateStatistics(};

    /** 動elq */
    void SendEmailNotification(const FString& To, const FString& Subject, const FString& Body};

    /** 故事重要性ɥre */
    void AnalyzeFeedbackContent(FFeedbackData& FeedbackData};

    /** 動 */
    void AutoTagFeedback(FFeedbackData& FeedbackData};

private:
    /** 動XX*/
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    TArray<FFeedbackData> AllFeedback;

    /** 池動 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    FFeedbackStatistics Statistics;

    /** t動_w動l動 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    bool bInitialized;

    /** 整理背包物~? */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    FString FeedbackDataFilePath;

    /** 動m]m */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    bool bEnableEmailNotifications;

    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    bool bEnableAutoCategorization;

    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    bool bEnableDuplicateDetection;
};

/**
 * UI務 */
UCLASS(BlueprintType, Blueprintable)
class USERFEEDBACKSYSTEM_API UFeedbackUIManager : public UObject
{
    GENERATED_BODY()

public:
    UFeedbackUIManager(};

    /** 目標數量 */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void ShowFeedbackSubmitUI(};

    /** 目標數量 */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void ShowFeedbackListUI(};

    /** 池 */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void ShowFeedbackDetailsUI(const FString& FeedbackID};

    /** ܲ池 */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void ShowStatisticsUI(};

    /** 故事重要性ɥr */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void CloseFeedbackUI(};

    /** ]mt */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void SetFeedbackSystem(UUserFeedbackSystem* InFeedbackSystem};

protected:
    /** 動目標數量 */
    void CreateFeedbackSubmitUI(};

    /** 動目標數量 */
    void CreateFeedbackListUI(};

    /** 動池 */
    void CreateFeedbackDetailsUI(};

    /** 動ز池 */
    void CreateStatisticsUI(};

private:
    /** tΤޥ */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UUserFeedbackSystem> FeedbackSystem;

    /** 動務*/
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    class UUserɥridget* CurrentUI;

    /** 目標數量 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UUserɥridget> FeedbackSubmitUIClass;

    /** 目標數量 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UUserɥridget> FeedbackListUIClass;

    /** 池 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UUserɥridget> FeedbackDetailsUIClass;

    /** 池 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UUserɥridget> StatisticsUIClass;
};
}
