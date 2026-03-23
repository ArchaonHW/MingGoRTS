// MingGoRTS 版本控制 API
// 轉換自 PowerShell: MingVersionControlAPI-Core.ps1

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "Misc/DateTime.h"
#include "Dom/JsonObject.h"
#include "Templates/SharedPointer.h"
#include "MingRTSVersionControlAPI.generated.h"

// 分支策略類型
UENUM(BlueprintType)
enum class EVCBranchStrategy : uint8
{
    Main        UMETA(DisplayName = "主分支"),
    Develop     UMETA(DisplayName = "開發分支"),
    Feature     UMETA(DisplayName = "功能分支"),
    Hotfix      UMETA(DisplayName = "修復分支"),
    Release     UMETA(DisplayName = "發布分支")
};

// 版本控制狀態
UENUM(BlueprintType)
enum class EVCStatus : uint8
{
    Uninitialized   UMETA(DisplayName = "未初始化"),
    Ready           UMETA(DisplayName = "就緒"),
    Dirty           UMETA(DisplayName = "有未提交更改"),
    Merging         UMETA(DisplayName = "合併中"),
    Rebasing        UMETA(DisplayName = "變基中"),
    Error           UMETA(DisplayName = "錯誤")
};

// 提交類型
UENUM(BlueprintType)
enum class ECommitType : uint8
{
    Feature     UMETA(DisplayName = "功能"),
    Fix         UMETA(DisplayName = "修復"),
    Docs        UMETA(DisplayName = "文件"),
    Style       UMETA(DisplayName = "風格"),
    Refactor    UMETA(DisplayName = "重構"),
    Test        UMETA(DisplayName = "測試"),
    Chore       UMETA(DisplayName = "維護")
};

// 分支資訊
USTRUCT(BlueprintType)
struct MINGGORTS_API FVCBranchInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch Info")
    FString BranchName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch Info")
    FString LastCommitHash;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch Info")
    FDateTime LastCommitDate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch Info")
    FString LastCommitMessage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch Info")
    bool bIsCurrent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch Info")
    bool bHasUnmergedChanges;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch Info")
    int32 CommitCount;

    FVCBranchInfo()
    {
        BranchName = TEXT("");
        LastCommitHash = TEXT("");
        LastCommitDate = FDateTime::Now();
        LastCommitMessage = TEXT("");
        bIsCurrent = false;
        bHasUnmergedChanges = false;
        CommitCount = 0;
    }
};

// 提交資訊
USTRUCT(BlueprintType)
struct MINGGORTS_API FVCCommitInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Commit Info")
    FString Hash;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Commit Info")
    FString ShortHash;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Commit Info")
    FString Author;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Commit Info")
    FDateTime Date;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Commit Info")
    FString Message;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Commit Info")
    ECommitType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Commit Info")
    TArray<FString> ChangedFiles;

    FVCCommitInfo()
    {
        Hash = TEXT("");
        ShortHash = TEXT("");
        Author = TEXT("");
        Date = FDateTime::Now();
        Message = TEXT("");
        Type = ECommitType::Chore;
        ChangedFiles = TArray<FString>();
    }
};

// 版本資訊
USTRUCT(BlueprintType)
struct MINGGORTS_API FVCVersionInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Version Info")
    int32 Major;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Version Info")
    int32 Minor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Version Info")
    int32 Patch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Version Info")
    FString Tag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Version Info")
    FString FullVersion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Version Info")
    bool bIsTagged;

    FVCVersionInfo()
    {
        Major = 0;
        Minor = 1;
        Patch = 0;
        Tag = TEXT("");
        FullVersion = TEXT("0.1.0");
        bIsTagged = false;
    }
};

// 版本控制狀態資訊
USTRUCT(BlueprintType)
struct MINGGORTS_API FVCStateInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VC State")
    FString CurrentBranch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VC State")
    FString LastCommitHash;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VC State")
    TArray<FString> PendingChanges;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VC State")
    FString LastTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VC State")
    EVCStatus Status;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VC State")
    int32 UncommittedCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VC State")
    bool bHasConflicts;

    FVCStateInfo()
    {
        CurrentBranch = TEXT("");
        LastCommitHash = TEXT("");
        PendingChanges = TArray<FString>();
        LastTag = TEXT("");
        Status = EVCStatus::Uninitialized;
        UncommittedCount = 0;
        bHasConflicts = false;
    }
};

// 委派
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVCInitialized, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVCStatusChanged, const FVCStateInfo&, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBranchSwitched, const FString&, BranchName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCommitCreated, const FVCCommitInfo&, Commit);

/**
 * MingGoRTS 版本控制 API
 * 轉換自 PowerShell: MingVersionControlAPI-Core.ps1
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|API|Version Control")
class MINGGORTS_API UMingRTSVersionControlAPI : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingRTSVersionControlAPI();

    // 初始化版本控制系統
    UFUNCTION(BlueprintCallable, Category = "Version Control")
    bool InitializeVersionControl(const FString& ProjectPath = TEXT(""));

    // 狀態查詢
    UFUNCTION(BlueprintCallable, Category = "Version Control")
    FVCStateInfo GetVCStatus(bool bDetailed = false);

    UFUNCTION(BlueprintCallable, Category = "Version Control")
    FVCVersionInfo GetCurrentVersion();

    UFUNCTION(BlueprintCallable, Category = "Version Control")
    FString GetCurrentBranch();

    // 分支操作
    UFUNCTION(BlueprintCallable, Category = "Version Control")
    TArray<FVCBranchInfo> GetAllBranches();

    UFUNCTION(BlueprintCallable, Category = "Version Control")
    bool SwitchBranch(const FString& BranchName, bool bCreateIfNotExists = false);

    UFUNCTION(BlueprintCallable, Category = "Version Control")
    bool CreateBranch(const FString& BranchName, EVCBranchStrategy Strategy);

    UFUNCTION(BlueprintCallable, Category = "Version Control")
    bool MergeBranch(const FString& SourceBranch, const FString& TargetBranch);

    UFUNCTION(BlueprintCallable, Category = "Version Control")
    bool DeleteBranch(const FString& BranchName, bool bForce = false);

    // 提交操作
    UFUNCTION(BlueprintCallable, Category = "Version Control")
    bool StageFiles(const TArray<FString>& Files);

    UFUNCTION(BlueprintCallable, Category = "Version Control")
    bool StageAll();

    UFUNCTION(BlueprintCallable, Category = "Version Control")
    bool UnstageFiles(const TArray<FString>& Files);

    UFUNCTION(BlueprintCallable, Category = "Version Control")
    FVCCommitInfo CreateCommit(const FString& Message, ECommitType Type = ECommitType::Chore);

    UFUNCTION(BlueprintCallable, Category = "Version Control")
    TArray<FVCCommitInfo> GetCommitHistory(int32 Count = 10);

    UFUNCTION(BlueprintCallable, Category = "Version Control")
    bool AmendLastCommit(const FString& NewMessage);

    // 版本標籤
    UFUNCTION(BlueprintCallable, Category = "Version Control")
    bool CreateTag(const FString& TagName, const FString& Message);

    UFUNCTION(BlueprintCallable, Category = "Version Control")
    bool DeleteTag(const FString& TagName);

    UFUNCTION(BlueprintCallable, Category = "Version Control")
    TArray<FString> GetAllTags();

    UFUNCTION(BlueprintCallable, Category = "Version Control")
    FVCVersionInfo IncrementVersion(int32 Major = 0, int32 Minor = 1, int32 Patch = 0);

    // 遠端操作
    UFUNCTION(BlueprintCallable, Category = "Version Control")
    bool PushToRemote(const FString& BranchName, const FString& RemoteName = TEXT("origin"));

    UFUNCTION(BlueprintCallable, Category = "Version Control")
    bool PullFromRemote(const FString& BranchName, const FString& RemoteName = TEXT("origin"));

    UFUNCTION(BlueprintCallable, Category = "Version Control")
    bool FetchFromRemote(const FString& RemoteName = TEXT("origin"));

    // 工具函數
    UFUNCTION(BlueprintCallable, Category = "Version Control")
    FString FormatCommitMessage(const FString& Message, ECommitType Type, const FString& Scope = TEXT(""));

    UFUNCTION(BlueprintCallable, Category = "Version Control")
    bool IsGitRepository(const FString& Path);

    UFUNCTION(BlueprintCallable, Category = "Version Control")
    bool HasUncommittedChanges();

    UFUNCTION(BlueprintCallable, Category = "Version Control")
    TArray<FString> GetUncommittedFiles();

    // 報告生成
    UFUNCTION(BlueprintCallable, Category = "Version Control")
    bool GenerateVCReport(const FString& OutputPath);

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "Version Control")
    FOnVCInitialized OnVCInitialized;

    UPROPERTY(BlueprintAssignable, Category = "Version Control")
    FOnVCStatusChanged OnVCStatusChanged;

    UPROPERTY(BlueprintAssignable, Category = "Version Control")
    FOnBranchSwitched OnBranchSwitched;

    UPROPERTY(BlueprintAssignable, Category = "Version Control")
    FOnCommitCreated OnCommitCreated;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    // 配置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FString ProjectPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FString MainBranch = TEXT("main");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FString DevelopBranch = TEXT("develop");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FString FeaturePrefix = TEXT("feature/");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FString HotfixPrefix = TEXT("hotfix/");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FString ReleasePrefix = TEXT("release/");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    FString TagPattern = TEXT("v{major}.{minor}.{patch}");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    bool bAutoFormat = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration")
    int32 MaxMessageLength = 100;

    // 狀態
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    bool bIsInitialized;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    FVCStateInfo CurrentState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    FVCVersionInfo CurrentVersion;

    // 內部方法
    bool ExecuteGitCommand(const FString& Command, FString& Output, FString& Error);
    void UpdateState();
    void LogVCMessage(const FString& Message);
    
    FString GetBranchPrefix(EVCBranchStrategy Strategy);
    ECommitType ParseCommitType(const FString& Message);
    FString GetCommitTypeString(ECommitType Type);
    
    bool IsValidBranchName(const FString& BranchName);
    bool IsValidTagName(const FString& TagName);
    
    FString GenerateReportContent();
    void BroadcastStatusChange();
};
