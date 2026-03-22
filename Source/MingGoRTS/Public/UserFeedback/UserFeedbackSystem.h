// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 用戶反饋系統 - 反饋收集和管理

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "UserFeedbackSystem.generated.h"

/**
 * 反饋類型枚舉
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
 * 反饋優先級枚舉
 */
UENUM(BlueprintType)
enum class EFeedbackPriority : uint8
{
    Low		UMETA(DisplayName = "Low"),
    Medium	UMETA(DisplayName = "Medium"),
    High	UMETA(DisplayName = "High"),
    Critical	UMETA(DisplayName = "Critical")
};

/**
 * 反饋狀態枚舉
 */
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
 * 反饋數據結構
 */
USTRUCT(BlueprintType)
struct FFeedbackData
{
    GENERATED_BODY()

    /** 反饋ID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString FeedbackID;

    /** 反饋類型 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    EFeedbackType FeedbackType;

    /** 反饋優先級 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    EFeedbackPriority Priority;

    /** 反饋狀態 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    EFeedbackStatus Status;

    /** 標題 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString Title;

    /** 描述 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString Description;

    /** 重現步驟 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString ReproductionSteps;

    /** 期望結果 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString ExpectedResult;

    /** 實際結果 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString ActualResult;

    /** 用戶信息 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString UserInfo;

    /** 系統信息 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString SystemInfo;

    /** 遊戲版本 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString GameVersion;

    /** 提交時間 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FDateTime SubmitTime;

    /** 更新時間 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FDateTime LastUpdateTime;

    /** 處理人員 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    FString AssignedTo;

    /** 標籤 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    TArray<FString> Tags;

    /** 附件 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    TArray<FString> Attachments;

    /** 評分 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feedback")
    float Rating;

    /** 是否已讀 */
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
 * 反饋統計數據
 */
USTRUCT(BlueprintType)
struct FFeedbackStatistics
{
    GENERATED_BODY()

    /** 總反饋數 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    int32 TotalFeedback;

    /** 按類型統計 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    TMap<EFeedbackType, int32> FeedbackByType;

    /** 按優先級統計 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    TMap<EFeedbackPriority, int32> FeedbackByPriority;

    /** 按狀態統計 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    TMap<EFeedbackStatus, int32> FeedbackByStatus;

    /** 平均響應時間 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    float AverageResponseTime;

    /** 解決率 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback Statistics")
    float ResolutionRate;

    /** 用戶滿意度 */
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
 * 用戶反饋系統
 */
UCLASS(BlueprintType, Blueprintable)
class USERFEEDBACKSYSTEM_API UUserFeedbackSystem : public UObject
{
    GENERATED_BODY()

public:
    UUserFeedbackSystem();

    /** 初始化反饋系統 */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    void InitializeFeedbackSystem();

    /** 提交反饋 */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FString SubmitFeedback(const FFeedbackData& FeedbackData);

    /** 獲取反饋列表 */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    TArray<FFeedbackData> GetFeedbackList(EFeedbackType FilterType = EFeedbackType::Other, EFeedbackStatus FilterStatus = EFeedbackStatus::New);

    /** 獲取反饋詳情 */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FFeedbackData GetFeedbackDetails(const FString& FeedbackID);

    /** 更新反饋狀態 */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    bool UpdateFeedbackStatus(const FString& FeedbackID, EFeedbackStatus NewStatus);

    /** 分配反饋 */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    bool AssignFeedback(const FString& FeedbackID, const FString& AssignedTo);

    /** 添加評論 */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    bool AddComment(const FString& FeedbackID, const FString& Comment);

    /** 獲取統計數據 */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FFeedbackStatistics GetStatistics();

    /** 導出反饋數據 */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FString ExportFeedbackData();

    /** 自動分類反饋 */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    void AutoCategorizeFeedback();

    /** 發送通知 */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    void SendNotification(const FString& FeedbackID, const FString& Message);

    /** 檢查重複反饋 */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    TArray<FString> CheckDuplicateFeedback(const FFeedbackData& FeedbackData);

    /** 生成反饋報告 */
    UFUNCTION(BlueprintCallable, Category = "Feedback System")
    FString GenerateFeedbackReport();

public:
    /** 反饋提交事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFeedbackSubmitted, const FString&, FeedbackID);

    /** 反饋狀態更新事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFeedbackStatusUpdated, const FString&, FeedbackID, EFeedbackStatus, NewStatus);

    /** 反饋分配事件 */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFeedbackAssigned, const FString&, FeedbackID, const FString&, AssignedTo);

    /** 新反饋通知事件 */
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
    /** 生成反饋ID */
    FString GenerateFeedbackID();

    /** 驗證反饋數據 */
    bool ValidateFeedbackData(const FFeedbackData& FeedbackData);

    /** 保存反饋數據 */
    bool SaveFeedbackData(const FFeedbackData& FeedbackData);

    /** 加載反饋數據 */
    void LoadFeedbackData();

    /** 更新統計數據 */
    void UpdateStatistics();

    /** 發送郵件通知 */
    void SendEmailNotification(const FString& To, const FString& Subject, const FString& Body);

    /** 分析反饋內容 */
    void AnalyzeFeedbackContent(FFeedbackData& FeedbackData);

    /** 自動標記 */
    void AutoTagFeedback(FFeedbackData& FeedbackData);

private:
    /** 所有反饋數據 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    TArray<FFeedbackData> AllFeedback;

    /** 統計數據 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    FFeedbackStatistics Statistics;

    /** 系統是否已初始化 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    bool bInitialized;

    /** 反饋數據文件路徑 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    FString FeedbackDataFilePath;

    /** 配置設置 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    bool bEnableEmailNotifications;

    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    bool bEnableAutoCategorization;

    UPROPERTY(BlueprintReadOnly, Category = "Feedback System", meta = (AllowPrivateAccess = "true"))
    bool bEnableDuplicateDetection;
};

/**
 * 反饋UI管理器
 */
UCLASS(BlueprintType, Blueprintable)
class USERFEEDBACKSYSTEM_API UFeedbackUIManager : public UObject
{
    GENERATED_BODY()

public:
    UFeedbackUIManager();

    /** 顯示反饋提交界面 */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void ShowFeedbackSubmitUI();

    /** 顯示反饋列表界面 */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void ShowFeedbackListUI();

    /** 顯示反饋詳情界面 */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void ShowFeedbackDetailsUI(const FString& FeedbackID);

    /** 顯示統計界面 */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void ShowStatisticsUI();

    /** 關閉反饋界面 */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void CloseFeedbackUI();

    /** 設置反饋系統 */
    UFUNCTION(BlueprintCallable, Category = "Feedback UI")
    void SetFeedbackSystem(UUserFeedbackSystem* InFeedbackSystem);

protected:
    /** 創建反饋提交界面 */
    void CreateFeedbackSubmitUI();

    /** 創建反饋列表界面 */
    void CreateFeedbackListUI();

    /** 創建反饋詳情界面 */
    void CreateFeedbackDetailsUI();

    /** 創建統計界面 */
    void CreateStatisticsUI();

private:
    /** 反饋系統引用 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UUserFeedbackSystem> FeedbackSystem;

    /** 當前顯示的界面 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    class UUserWidget* CurrentUI;

    /** 反饋提交界面 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UUserWidget> FeedbackSubmitUIClass;

    /** 反饋列表界面 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UUserWidget> FeedbackListUIClass;

    /** 反饋詳情界面 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UUserWidget> FeedbackDetailsUIClass;

    /** 統計界面 */
    UPROPERTY(BlueprintReadOnly, Category = "Feedback UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UUserWidget> StatisticsUIClass;
};
