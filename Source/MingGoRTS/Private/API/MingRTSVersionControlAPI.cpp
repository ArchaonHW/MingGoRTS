// MingGoRTS Version Control API Implementation
// 轉換自 PowerShell: MingVersionControlAPI-Core.ps1

#include "MingRTSVersionControlAPI.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

UMingRTSVersionControlAPI::UMingRTSVersionControlAPI()
{
    PrimaryComponentTick.bCanEverTick = false;
    bIsInitialized = false;
    ProjectPath = TEXT("C:\\HW\\MingGoRTS");
    MainBranch = TEXT("main");
    DevelopBranch = TEXT("develop");
    FeaturePrefix = TEXT("feature/");
    HotfixPrefix = TEXT("hotfix/");
    ReleasePrefix = TEXT("release/");
    TagPattern = TEXT("v{major}.{minor}.{patch}");
    bAutoFormat = true;
    MaxMessageLength = 100;
}

void UMingRTSVersionControlAPI::BeginPlay()
{
    Super::BeginPlay();
}

void UMingRTSVersionControlAPI::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

bool UMingRTSVersionControlAPI::InitializeVersionControl(const FString& InProjectPath)
{
    if (!InProjectPath.IsEmpty())
    {
        ProjectPath = InProjectPath;
    }
    
    LogVCMessage(TEXT("Initializing MingGoRTS Version Control..."));
    
    // 檢查是否為 Git 倉庫
    if (!IsGitRepository(ProjectPath))
    {
        LogVCMessage(FString::Printf(TEXT("Error: Not a Git repository: %s"), *ProjectPath));
        OnVCInitialized.Broadcast(false);
        return false;
    }
    
    // 更新狀態
    UpdateState();
    bIsInitialized = true;
    
    LogVCMessage(TEXT("Version Control initialized successfully"));
    LogVCMessage(FString::Printf(TEXT("Current branch: %s"), *CurrentState.CurrentBranch));
    LogVCMessage(FString::Printf(TEXT("Last commit: %s"), *CurrentState.LastCommitHash.Left(8)));
    
    OnVCInitialized.Broadcast(true);
    return true;
}

bool UMingRTSVersionControlAPI::IsGitRepository(const FString& Path)
{
    FString GitDir = FPaths::Combine(Path, TEXT(".git"));
    return FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*GitDir);
}

FVCStateInfo UMingRTSVersionControlAPI::GetVCStatus(bool bDetailed)
{
    if (!bIsInitialized)
    {
        LogVCMessage(TEXT("Version Control not initialized"));
        return FVCStateInfo();
    }
    
    UpdateState();
    
    if (bDetailed)
    {
        LogVCMessage(TEXT("========================================"));
        LogVCMessage(TEXT("MingGoRTS Version Control Status"));
        LogVCMessage(TEXT("========================================"));
        LogVCMessage(FString::Printf(TEXT("Current Branch: %s"), *CurrentState.CurrentBranch));
        LogVCMessage(FString::Printf(TEXT("Last Commit: %s"), *CurrentState.LastCommitHash));
        LogVCMessage(FString::Printf(TEXT("Uncommitted Changes: %d"), CurrentState.UncommittedCount));
        LogVCMessage(FString::Printf(TEXT("Status: %d"), (int32)CurrentState.Status));
    }
    
    return CurrentState;
}

void UMingRTSVersionControlAPI::UpdateState()
{
    FString Output, Error;
    
    // 取得當前分支
    if (ExecuteGitCommand(TEXT("rev-parse --abbrev-ref HEAD"), Output, Error))
    {
        CurrentState.CurrentBranch = Output.TrimStartAndEnd();
    }
    
    // 取得最後提交
    if (ExecuteGitCommand(TEXT("log -1 --format=%H"), Output, Error))
    {
        CurrentState.LastCommitHash = Output.TrimStartAndEnd();
    }
    
    // 取得未提交的更改
    CurrentState.PendingChanges.Empty();
    if (ExecuteGitCommand(TEXT("status --porcelain"), Output, Error))
    {
        TArray<FString> Lines;
        Output.ParseIntoArrayLines(Lines);
        for (const FString& Line : Lines)
        {
            if (Line.Len() > 3)
            {
                CurrentState.PendingChanges.Add(Line);
            }
        }
    }
    
    CurrentState.UncommittedCount = CurrentState.PendingChanges.Num();
    
    // 取得最後標籤
    if (ExecuteGitCommand(TEXT("describe --tags --abbrev=0"), Output, Error))
    {
        CurrentState.LastTag = Output.TrimStartAndEnd();
    }
    
    // 更新狀態
    if (CurrentState.UncommittedCount > 0)
    {
        CurrentState.Status = EVCStatus::Dirty;
    }
    else
    {
        CurrentState.Status = EVCStatus::Ready;
    }
}

FVCVersionInfo UMingRTSVersionControlAPI::GetCurrentVersion()
{
    // 從最後標籤解析版本
    FString LastTag = CurrentState.LastTag;
    if (LastTag.IsEmpty())
    {
        LastTag = TEXT("v0.1.0");
    }
    
    // 移除 'v' 前綴
    if (LastTag.StartsWith(TEXT("v")))
    {
        LastTag = LastTag.RightChop(1);
    }
    
    TArray<FString> VersionParts;
    LastTag.ParseIntoArray(VersionParts, TEXT("."));
    
    FVCVersionInfo Version;
    if (VersionParts.Num() >= 3)
    {
        Version.Major = FCString::Atoi(*VersionParts[0]);
        Version.Minor = FCString::Atoi(*VersionParts[1]);
        Version.Patch = FCString::Atoi(*VersionParts[2]);
    }
    
    Version.Tag = CurrentState.LastTag;
    Version.FullVersion = FString::Printf(TEXT("%d.%d.%d"), Version.Major, Version.Minor, Version.Patch);
    Version.bIsTagged = !CurrentState.LastTag.IsEmpty();
    
    return Version;
}

FString UMingRTSVersionControlAPI::GetCurrentBranch()
{
    UpdateState();
    return CurrentState.CurrentBranch;
}

TArray<FVCBranchInfo> UMingRTSVersionControlAPI::GetAllBranches()
{
    TArray<FVCBranchInfo> Branches;
    FString Output, Error;
    
    if (!ExecuteGitCommand(TEXT("branch -a"), Output, Error))
    {
        return Branches;
    }
    
    TArray<FString> Lines;
    Output.ParseIntoArrayLines(Lines);
    
    for (const FString& Line : Lines)
    {
        FString BranchName = Line.TrimStartAndEnd();
        
        // 移除 '*' 標記
        if (BranchName.StartsWith(TEXT("* ")))
        {
            BranchName = BranchName.RightChop(2);
        }
        
        FVCBranchInfo Info;
        Info.BranchName = BranchName;
        Info.bIsCurrent = Line.Contains(TEXT("* "));
        
        // 取得分支的提交資訊
        if (Info.bIsCurrent)
        {
            Info.LastCommitHash = CurrentState.LastCommitHash;
        }
        
        Branches.Add(Info);
    }
    
    return Branches;
}

bool UMingRTSVersionControlAPI::SwitchBranch(const FString& BranchName, bool bCreateIfNotExists)
{
    FString Output, Error;
    
    // 檢查分支是否存在
    TArray<FVCBranchInfo> Branches = GetAllBranches();
    bool bExists = false;
    for (const FVCBranchInfo& Branch : Branches)
    {
        if (Branch.BranchName == BranchName)
        {
            bExists = true;
            break;
        }
    }
    
    if (!bExists && bCreateIfNotExists)
    {
        // 建立新分支
        if (!ExecuteGitCommand(FString::Printf(TEXT("checkout -b %s"), *BranchName), Output, Error))
        {
            LogVCMessage(FString::Printf(TEXT("Failed to create branch: %s"), *Error));
            return false;
        }
    }
    else if (!bExists)
    {
        LogVCMessage(FString::Printf(TEXT("Branch does not exist: %s"), *BranchName));
        return false;
    }
    else
    {
        // 切換到現有分支
        if (!ExecuteGitCommand(FString::Printf(TEXT("checkout %s"), *BranchName), Output, Error))
        {
            LogVCMessage(FString::Printf(TEXT("Failed to switch branch: %s"), *Error));
            return false;
        }
    }
    
    UpdateState();
    OnBranchSwitched.Broadcast(BranchName);
    BroadcastStatusChange();
    
    LogVCMessage(FString::Printf(TEXT("Switched to branch: %s"), *BranchName));
    return true;
}

bool UMingRTSVersionControlAPI::CreateBranch(const FString& BranchName, EVCBranchStrategy Strategy)
{
    FString FullBranchName = GetBranchPrefix(Strategy) + BranchName;
    return SwitchBranch(FullBranchName, true);
}

bool UMingRTSVersionControlAPI::MergeBranch(const FString& SourceBranch, const FString& TargetBranch)
{
    // 切換到目標分支
    if (!SwitchBranch(TargetBranch, false))
    {
        return false;
    }
    
    FString Output, Error;
    if (!ExecuteGitCommand(FString::Printf(TEXT("merge %s"), *SourceBranch), Output, Error))
    {
        LogVCMessage(FString::Printf(TEXT("Merge failed: %s"), *Error));
        return false;
    }
    
    UpdateState();
    BroadcastStatusChange();
    
    LogVCMessage(FString::Printf(TEXT("Merged %s into %s"), *SourceBranch, *TargetBranch));
    return true;
}

bool UMingRTSVersionControlAPI::DeleteBranch(const FString& BranchName, bool bForce)
{
    FString Output, Error;
    FString Flag = bForce ? TEXT("-D") : TEXT("-d");
    
    if (!ExecuteGitCommand(FString::Printf(TEXT("branch %s %s"), *Flag, *BranchName), Output, Error))
    {
        LogVCMessage(FString::Printf(TEXT("Failed to delete branch: %s"), *Error));
        return false;
    }
    
    LogVCMessage(FString::Printf(TEXT("Deleted branch: %s"), *BranchName));
    return true;
}

bool UMingRTSVersionControlAPI::StageFiles(const TArray<FString>& Files)
{
    FString Output, Error;
    
    for (const FString& File : Files)
    {
        if (!ExecuteGitCommand(FString::Printf(TEXT("add \"%s\""), *File), Output, Error))
        {
            LogVCMessage(FString::Printf(TEXT("Failed to stage file: %s"), *File));
            return false;
        }
    }
    
    UpdateState();
    BroadcastStatusChange();
    return true;
}

bool UMingRTSVersionControlAPI::StageAll()
{
    FString Output, Error;
    
    if (!ExecuteGitCommand(TEXT("add ."), Output, Error))
    {
        LogVCMessage(FString::Printf(TEXT("Failed to stage all files: %s"), *Error));
        return false;
    }
    
    UpdateState();
    BroadcastStatusChange();
    
    LogVCMessage(TEXT("Staged all changes"));
    return true;
}

bool UMingRTSVersionControlAPI::UnstageFiles(const TArray<FString>& Files)
{
    FString Output, Error;
    
    for (const FString& File : Files)
    {
        if (!ExecuteGitCommand(FString::Printf(TEXT("reset HEAD \"%s\""), *File), Output, Error))
        {
            LogVCMessage(FString::Printf(TEXT("Failed to unstage file: %s"), *File));
            return false;
        }
    }
    
    UpdateState();
    BroadcastStatusChange();
    return true;
}

FVCCommitInfo UMingRTSVersionControlAPI::CreateCommit(const FString& Message, ECommitType Type)
{
    FVCCommitInfo Commit;
    Commit.Message = bAutoFormat ? FormatCommitMessage(Message, Type, TEXT("")) : Message;
    Commit.Type = Type;
    Commit.Date = FDateTime::Now();
    
    FString Output, Error;
    
    if (!ExecuteGitCommand(FString::Printf(TEXT("commit -m \"%s\""), *Commit.Message), Output, Error))
    {
        LogVCMessage(FString::Printf(TEXT("Failed to create commit: %s"), *Error));
        return Commit;
    }
    
    // 取得新提交的資訊
    if (ExecuteGitCommand(TEXT("log -1 --format=%H"), Output, Error))
    {
        Commit.Hash = Output.TrimStartAndEnd();
        Commit.ShortHash = Commit.Hash.Left(8);
    }
    
    if (ExecuteGitCommand(TEXT("log -1 --format=%an"), Output, Error))
    {
        Commit.Author = Output.TrimStartAndEnd();
    }
    
    UpdateState();
    OnCommitCreated.Broadcast(Commit);
    BroadcastStatusChange();
    
    LogVCMessage(FString::Printf(TEXT("Created commit: %s - %s"), *Commit.ShortHash, *Commit.Message));
    return Commit;
}

TArray<FVCCommitInfo> UMingRTSVersionControlAPI::GetCommitHistory(int32 Count)
{
    TArray<FVCCommitInfo> Commits;
    FString Output, Error;
    
    FString Command = FString::Printf(TEXT("log -%d --format=%%H|%%an|%%ad|%%s --date=iso"), Count);
    
    if (!ExecuteGitCommand(Command, Output, Error))
    {
        return Commits;
    }
    
    TArray<FString> Lines;
    Output.ParseIntoArrayLines(Lines);
    
    for (const FString& Line : Lines)
    {
        TArray<FString> Parts;
        Line.ParseIntoArray(Parts, TEXT("|"));
        
        if (Parts.Num() >= 4)
        {
            FVCCommitInfo Commit;
            Commit.Hash = Parts[0];
            Commit.ShortHash = Commit.Hash.Left(8);
            Commit.Author = Parts[1];
            FDateTime::Parse(Parts[2], Commit.Date);
            Commit.Message = Parts[3];
            Commit.Type = ParseCommitType(Commit.Message);
            
            Commits.Add(Commit);
        }
    }
    
    return Commits;
}

bool UMingRTSVersionControlAPI::AmendLastCommit(const FString& NewMessage)
{
    FString Output, Error;
    FString Message = bAutoFormat ? FormatCommitMessage(NewMessage, ECommitType::Chore, TEXT("")) : NewMessage;
    
    if (!ExecuteGitCommand(FString::Printf(TEXT("commit --amend -m \"%s\""), *Message), Output, Error))
    {
        LogVCMessage(FString::Printf(TEXT("Failed to amend commit: %s"), *Error));
        return false;
    }
    
    UpdateState();
    LogVCMessage(TEXT("Amended last commit"));
    return true;
}

bool UMingRTSVersionControlAPI::CreateTag(const FString& TagName, const FString& Message)
{
    FString Output, Error;
    
    if (!ExecuteGitCommand(FString::Printf(TEXT("tag -a %s -m \"%s\""), *TagName, *Message), Output, Error))
    {
        LogVCMessage(FString::Printf(TEXT("Failed to create tag: %s"), *Error));
        return false;
    }
    
    UpdateState();
    LogVCMessage(FString::Printf(TEXT("Created tag: %s"), *TagName));
    return true;
}

bool UMingRTSVersionControlAPI::DeleteTag(const FString& TagName)
{
    FString Output, Error;
    
    if (!ExecuteGitCommand(FString::Printf(TEXT("tag -d %s"), *TagName), Output, Error))
    {
        LogVCMessage(FString::Printf(TEXT("Failed to delete tag: %s"), *Error));
        return false;
    }
    
    UpdateState();
    LogVCMessage(FString::Printf(TEXT("Deleted tag: %s"), *TagName));
    return true;
}

TArray<FString> UMingRTSVersionControlAPI::GetAllTags()
{
    TArray<FString> Tags;
    FString Output, Error;
    
    if (!ExecuteGitCommand(TEXT("tag -l"), Output, Error))
    {
        return Tags;
    }
    
    Output.ParseIntoArrayLines(Tags);
    return Tags;
}

FVCVersionInfo UMingRTSVersionControlAPI::IncrementVersion(int32 Major, int32 Minor, int32 Patch)
{
    FVCVersionInfo Current = GetCurrentVersion();
    
    FVCVersionInfo NewVersion;
    NewVersion.Major = Current.Major + Major;
    NewVersion.Minor = (Major > 0) ? Minor : Current.Minor + Minor;
    NewVersion.Patch = (Major > 0 || Minor > 0) ? Patch : Current.Patch + Patch;
    
    NewVersion.FullVersion = FString::Printf(TEXT("%d.%d.%d"), NewVersion.Major, NewVersion.Minor, NewVersion.Patch);
    NewVersion.Tag = FString::Printf(TEXT("v%s"), *NewVersion.FullVersion);
    
    // 建立新標籤
    CreateTag(NewVersion.Tag, FString::Printf(TEXT("Version %s"), *NewVersion.FullVersion));
    
    CurrentVersion = NewVersion;
    return NewVersion;
}

bool UMingRTSVersionControlAPI::PushToRemote(const FString& BranchName, const FString& RemoteName)
{
    FString Output, Error;
    
    if (!ExecuteGitCommand(FString::Printf(TEXT("push %s %s"), *RemoteName, *BranchName), Output, Error))
    {
        LogVCMessage(FString::Printf(TEXT("Failed to push: %s"), *Error));
        return false;
    }
    
    LogVCMessage(FString::Printf(TEXT("Pushed %s to %s"), *BranchName, *RemoteName));
    return true;
}

bool UMingRTSVersionControlAPI::PullFromRemote(const FString& BranchName, const FString& RemoteName)
{
    FString Output, Error;
    
    if (!ExecuteGitCommand(FString::Printf(TEXT("pull %s %s"), *RemoteName, *BranchName), Output, Error))
    {
        LogVCMessage(FString::Printf(TEXT("Failed to pull: %s"), *Error));
        return false;
    }
    
    UpdateState();
    BroadcastStatusChange();
    
    LogVCMessage(FString::Printf(TEXT("Pulled from %s/%s"), *RemoteName, *BranchName));
    return true;
}

bool UMingRTSVersionControlAPI::FetchFromRemote(const FString& RemoteName)
{
    FString Output, Error;
    
    if (!ExecuteGitCommand(FString::Printf(TEXT("fetch %s"), *RemoteName), Output, Error))
    {
        LogVCMessage(FString::Printf(TEXT("Failed to fetch: %s"), *Error));
        return false;
    }
    
    LogVCMessage(FString::Printf(TEXT("Fetched from %s"), *RemoteName));
    return true;
}

FString UMingRTSVersionControlAPI::FormatCommitMessage(const FString& Message, ECommitType Type, const FString& Scope)
{
    FString TypeStr = GetCommitTypeString(Type);
    FString Formatted;
    
    if (Scope.IsEmpty())
    {
        Formatted = FString::Printf(TEXT("%s: %s"), *TypeStr, *Message);
    }
    else
    {
        Formatted = FString::Printf(TEXT("%s(%s): %s"), *TypeStr, *Scope, *Message);
    }
    
    // 截斷過長的訊息
    if (Formatted.Len() > MaxMessageLength)
    {
        Formatted = Formatted.Left(MaxMessageLength - 3) + TEXT("...");
    }
    
    return Formatted;
}

bool UMingRTSVersionControlAPI::HasUncommittedChanges()
{
    UpdateState();
    return CurrentState.UncommittedCount > 0;
}

TArray<FString> UMingRTSVersionControlAPI::GetUncommittedFiles()
{
    UpdateState();
    return CurrentState.PendingChanges;
}

bool UMingRTSVersionControlAPI::GenerateVCReport(const FString& OutputPath)
{
    FString Content = GenerateReportContent();
    return FFileHelper::SaveStringToFile(Content, *OutputPath);
}

FString UMingRTSVersionControlAPI::GenerateReportContent()
{
    FString Report;
    
    Report += TEXT("# MingGoRTS Version Control Report\n\n");
    Report += FString::Printf(TEXT("**Generated:** %s\n\n"), *FDateTime::Now().ToString());
    
    // 狀態摘要
    Report += TEXT("## Repository Status\n\n");
    Report += FString::Printf(TEXT("- **Current Branch:** %s\n"), *CurrentState.CurrentBranch);
    Report += FString::Printf(TEXT("- **Last Commit:** %s\n"), *CurrentState.LastCommitHash.Left(8));
    Report += FString::Printf(TEXT("- **Uncommitted Changes:** %d\n"), CurrentState.UncommittedCount);
    Report += FString::Printf(TEXT("- **Status:** %d\n\n"), (int32)CurrentState.Status);
    
    // 版本資訊
    FVCVersionInfo Version = GetCurrentVersion();
    Report += TEXT("## Version Info\n\n");
    Report += FString::Printf(TEXT("- **Current Version:** %s\n"), *Version.FullVersion);
    Report += FString::Printf(TEXT("- **Last Tag:** %s\n\n"), *CurrentState.LastTag);
    
    // 分支列表
    Report += TEXT("## Branches\n\n");
    TArray<FVCBranchInfo> Branches = GetAllBranches();
    for (const FVCBranchInfo& Branch : Branches)
    {
        FString CurrentMarker = Branch.bIsCurrent ? TEXT("* ") : TEXT("");
        Report += FString::Printf(TEXT("- %s%s\n"), *CurrentMarker, *Branch.BranchName);
    }
    
    Report += TEXT("\n");
    
    // 最近的提交
    Report += TEXT("## Recent Commits\n\n");
    TArray<FVCCommitInfo> Commits = GetCommitHistory(10);
    for (const FVCCommitInfo& Commit : Commits)
    {
        Report += FString::Printf(TEXT("- `%s` %s - %s\n"), 
            *Commit.ShortHash, *Commit.Message, *Commit.Date.ToString());
    }
    
    return Report;
}

bool UMingRTSVersionControlAPI::ExecuteGitCommand(const FString& Command, FString& Output, FString& Error)
{
    FString FullCommand = FString::Printf(TEXT("git %s"), *Command);
    
    int32 ReturnCode;
    FPlatformProcess::ExecProcess(*FullCommand, TEXT(""), &ReturnCode, &Output, &Error, 
        *ProjectPath);
    
    return ReturnCode == 0;
}

void UMingRTSVersionControlAPI::LogVCMessage(const FString& Message)
{
    UE_LOG(LogTemp, Log, TEXT("[VC] %s"), *Message);
}

void UMingRTSVersionControlAPI::BroadcastStatusChange()
{
    OnVCStatusChanged.Broadcast(CurrentState);
}

FString UMingRTSVersionControlAPI::GetBranchPrefix(EVCBranchStrategy Strategy)
{
    switch (Strategy)
    {
        case EVCBranchStrategy::Feature: return FeaturePrefix;
        case EVCBranchStrategy::Hotfix: return HotfixPrefix;
        case EVCBranchStrategy::Release: return ReleasePrefix;
        default: return TEXT("");
    }
}

ECommitType UMingRTSVersionControlAPI::ParseCommitType(const FString& Message)
{
    if (Message.StartsWith(TEXT("feat"))) return ECommitType::Feature;
    if (Message.StartsWith(TEXT("fix"))) return ECommitType::Fix;
    if (Message.StartsWith(TEXT("docs"))) return ECommitType::Docs;
    if (Message.StartsWith(TEXT("style"))) return ECommitType::Style;
    if (Message.StartsWith(TEXT("refactor"))) return ECommitType::Refactor;
    if (Message.StartsWith(TEXT("test"))) return ECommitType::Test;
    return ECommitType::Chore;
}

FString UMingRTSVersionControlAPI::GetCommitTypeString(ECommitType Type)
{
    switch (Type)
    {
        case ECommitType::Feature: return TEXT("feat");
        case ECommitType::Fix: return TEXT("fix");
        case ECommitType::Docs: return TEXT("docs");
        case ECommitType::Style: return TEXT("style");
        case ECommitType::Refactor: return TEXT("refactor");
        case ECommitType::Test: return TEXT("test");
        default: return TEXT("chore");
    }
}
