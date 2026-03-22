// Copyright (c) 2026 MingGoRTS. All rights reserved.
// ?®Êà∂?çÈ?Á≥ªÁµ± - ?çÈ??∂È??åÁÆ°??
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "UserFeedbackSystem.generated.h"

/**
 * ?çÈ?È°ûÂ??öË?
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
 * ?çÈ??™Â?Á¥öÊ??? */
UENUM(BlueprintType)
enum class EFeedbackPriority : uint8
{
    Low		UMETA(DisplayName = "Low"),
    Medium	UMETA(DisplayName = "Medium"),
    High	UMETA(DisplayName = "High"),
    Critical	UMETA(DisplayName = "Critical")
};

/**
 * ?çÈ??Ä?ãÊ??? */
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
 * ?çÈ??∏Ê?ÁµêÊ?
 */
USTRUCT(BlueprintType)
struct FFeedbackData
{
    GENERATED_BODY()

    /** ?çÈ?ID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString FeedbackID;

    /** ?çÈ?È°ûÂ? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    EFeedbackType FeedbackType;

    /** ?çÈ??™Â?Á¥?*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    EFeedbackPriority Priority;

    /** ?çÈ??Ä??*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    EFeedbackStatus Status;

    /** Ê®ôÈ? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString Title;

    /** ?èËø∞ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString Description;

    /** ?çÁèæÊ≠•È? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString ReproductionSteps;

    /** ?üÊ?ÁµêÊ? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString ExpectedResult;

    /** ÂØ¶È?ÁµêÊ? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString ActualResult;

    /** ?®Êà∂‰ø°ÊÅØ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString UserInfo;

    /** Á≥ªÁµ±‰ø°ÊÅØ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString SystemInfo;

    /** ?äÊà≤?àÊú¨ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString GameVersion;

    /** ?ê‰∫§?ÇÈ? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FDateTime SubmitTime;

    /** ?¥Êñ∞?ÇÈ? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FDateTime LastUpdateTime;

    /** ?ïÁ?‰∫∫Âì° */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString AssignedTo;

    /** Ê®ôÁ±§ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    TArray<FString> Tags;

    /** ?Ñ‰ª∂ */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    TArray<FString> Attachments;

    /** Ë©ïÂ? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    float Rating;

    /** ?ØÂê¶Â∑≤Ë? */
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
 * ?çÈ?Áµ±Ë??∏Ê?
 */
USTRUCT(BlueprintType)
struct FFeedbackStatistics
{
    GENERATED_BODY()

    /** Á∏ΩÂ?È•ãÊï∏ */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    int32 TotalFeedback;

    /** ?âÈ??ãÁµ±Ë®?*/
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    TMap<EFeedbackType, int32> FeedbackByType;

    /** ?âÂÑ™?àÁ?Áµ±Ë? */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    TMap<EFeedbackPriority, int32> FeedbackByPriority;

    /** ?âÁ??ãÁµ±Ë®?*/
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    TMap<EFeedbackStatus, int32> FeedbackByStatus;

    /** Âπ≥Â??øÊ??ÇÈ? */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    float AverageResponseTime;

    /** Ëß?±∫??*/
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    float ResolutionRate;

    /** ?®Êà∂ÊªøÊ?Â∫?*/
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
 * ?®Êà∂?çÈ?Á≥ªÁµ±
 */
UCLASS(BlueprintType, Blueprintable)
class USERFEEDBACKSYSTEM_API UUserFeedbackSystem : public UObject
{
    GENERATED_BODY()

public:
    UUserFeedbackSystem(};

    /** ?ùÂ??ñÂ?È•ãÁ≥ªÁµ?*/
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    void InitializeFeedbackSystem(};

    /** ?ê‰∫§?çÈ? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FString SubmitFeedback(const FFeedbackData& FeedbackData};

    /** ?≤Â??çÈ??óË°® */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    TArray<FFeedbackData> GetFeedbackList(EFeedbackType FilterType = EFeedbackType::Other, EFeedbackStatus FilterStatus = EFeedbackStatus::New};

    /** ?≤Â??çÈ?Ë©≥Ê? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FFeedbackData GetFeedbackDetails(const FString& FeedbackID};

    /** ?¥Êñ∞?çÈ??Ä??*/
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    bool UpdateFeedbackStatus(const FString& FeedbackID, EFeedbackStatus NewStatus};

    /** ?ÜÈ??çÈ? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    bool AssignFeedback(const FString& FeedbackID, const FString& AssignedTo};

    /** Ê∑ªÂ?Ë©ïË? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    bool AddComment(const FString& FeedbackID, const FString& Comment};

    /** ?≤Â?Áµ±Ë??∏Ê? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FFeedbackStatistics GetStatistics(};

    /** Â∞éÂá∫?çÈ??∏Ê? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FString ExportFeedbackData(};

    /** ?™Â??ÜÈ??çÈ? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    void AutoCategorizeFeedback(};

    /** ?ºÈÄÅÈÄöÁü• */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    void SendNotification(const FString& FeedbackID, const FString& Message};

    /** Ê™¢Êü•?çË??çÈ? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    TArray<FString> CheckDuplicateFeedback(const FFeedbackData& FeedbackData};

    /** ?üÊ??çÈ??±Â? */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FString GenerateFeedbackReport(};

public:
    /** ?çÈ??ê‰∫§‰∫ã‰ª∂ */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFeedbackSubmitted, const FString&, FeedbackID};

    /** ?çÈ??Ä?ãÊõ¥?∞‰?‰ª?*/
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFeedbackStatusUpdated, const FString&, FeedbackID, EFeedbackStatus, NewStatus};

    /** ?çÈ??ÜÈ?‰∫ã‰ª∂ */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFeedbackAssigned, const FString&, FeedbackID, const FString&, AssignedTo};

    /** ?∞Â?È•ãÈÄöÁü•‰∫ã‰ª∂ */
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
    /** ?üÊ??çÈ?ID */
    FString GenerateFeedbackID(};

    /** È©óË??çÈ??∏Ê? */
    bool ValidateFeedbackData(const FFeedbackData& FeedbackData};

    /** ‰øùÂ??çÈ??∏Ê? */
    bool SaveFeedbackData(const FFeedbackData& FeedbackData};

    /** ?†Ë??çÈ??∏Ê? */
    void LoadFeedbackData(};

    /** ?¥Êñ∞Áµ±Ë??∏Ê? */
    void UpdateStatistics(};

    /** ?ºÈÄÅÈÉµ‰ª∂ÈÄöÁü• */
    void SendEmailNotification(const FString& To, const FString& Subject, const FString& Body};

    /** ?ÜÊ??çÈ??ßÂÆπ */
    void AnalyzeFeedbackContent(FFeedbackData& FeedbackData};

    /** ?™Â?Ê®ôË? */
    void AutoTagFeedback(FFeedbackData& FeedbackData};

private:
    /** ?Ä?âÂ?È•ãÊï∏??*/
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    TArray<FFeedbackData> AllFeedback;

    /** Áµ±Ë??∏Ê? */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    FFeedbackStatistics Statistics;

    /** Á≥ªÁµ±?ØÂê¶Â∑≤Â?ÂßãÂ? */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    bool bInitialized;

    /** ?çÈ??∏Ê??á‰ª∂Ë∑ØÂ? */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    FString FeedbackDataFilePath;

    /** ?çÁΩÆË®≠ÁΩÆ */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    bool bEnableEmailNotifications;

    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    bool bEnableAutoCategorization;

    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    bool bEnableDuplicateDetection;
};

/**
 * ?çÈ?UIÁÆ°Á??? */
UCLASS(BlueprintType, Blueprintable)
class USERFEEDBACKSYSTEM_API UFeedbackUIManager : public UObject
{
    GENERATED_BODY()

public:
    UFeedbackUIManager(};

    /** È°ØÁ§∫?çÈ??ê‰∫§?åÈù¢ */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void ShowFeedbackSubmitUI(};

    /** È°ØÁ§∫?çÈ??óË°®?åÈù¢ */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void ShowFeedbackListUI(};

    /** È°ØÁ§∫?çÈ?Ë©≥Ê??åÈù¢ */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void ShowFeedbackDetailsUI(const FString& FeedbackID};

    /** È°ØÁ§∫Áµ±Ë??åÈù¢ */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void ShowStatisticsUI(};

    /** ?úÈ??çÈ??åÈù¢ */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void CloseFeedbackUI(};

    /** Ë®≠ÁΩÆ?çÈ?Á≥ªÁµ± */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void SetFeedbackSystem(UUserFeedbackSystem* InFeedbackSystem};

protected:
    /** ?µÂª∫?çÈ??ê‰∫§?åÈù¢ */
    void CreateFeedbackSubmitUI(};

    /** ?µÂª∫?çÈ??óË°®?åÈù¢ */
    void CreateFeedbackListUI(};

    /** ?µÂª∫?çÈ?Ë©≥Ê??åÈù¢ */
    void CreateFeedbackDetailsUI(};

    /** ?µÂª∫Áµ±Ë??åÈù¢ */
    void CreateStatisticsUI(};

private:
    /** ?çÈ?Á≥ªÁµ±ÂºïÁî® */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UUserFeedbackSystem> FeedbackSystem;

    /** ?∂Â?È°ØÁ§∫?ÑÁ???*/
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    class UUserWidget* CurrentUI;

    /** ?çÈ??ê‰∫§?åÈù¢ */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UUserWidget> FeedbackSubmitUIClass;

    /** ?çÈ??óË°®?åÈù¢ */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UUserWidget> FeedbackListUIClass;

    /** ?çÈ?Ë©≥Ê??åÈù¢ */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UUserWidget> FeedbackDetailsUIClass;

    /** Áµ±Ë??åÈù¢ */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UUserWidget> StatisticsUIClass;
};

