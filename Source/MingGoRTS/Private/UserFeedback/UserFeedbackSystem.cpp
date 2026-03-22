// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 用戶反饋系統 - 反饋收集和管理實現

#include "UserFeedback/UserFeedbackSystem.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "Misc/Guid.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Kismet/GameplayStatics.h"

UUserFeedbackSystem::UUserFeedbackSystem()
    : bInitialized(false)
    , FeedbackDataFilePath(TEXT(""))
    , bEnableEmailNotifications(true)
    , bEnableAutoCategorization(true)
    , bEnableDuplicateDetection(true)
{
}

void UUserFeedbackSystem::InitializeFeedbackSystem()
{
    if (bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Feedback system already initialized"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Initializing User Feedback System..."));

    // 設置反饋數據文件路徑
    FeedbackDataFilePath = FPaths::ProjectSavedDir() / TEXT("Feedback") / TEXT("FeedbackData.json");

    // 確保目錄存在
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    FString DirectoryPath = FPaths::GetPath(FeedbackDataFilePath);
    if (!PlatformFile.DirectoryExists(*DirectoryPath))
    {
        PlatformFile.CreateDirectoryTree(*DirectoryPath);
    }

    // 加載現有反饋數據
    LoadFeedbackData();

    // 更新統計數據
    UpdateStatistics();

    bInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("User Feedback System initialized successfully"));
}

FString UUserFeedbackSystem::SubmitFeedback(const FFeedbackData& FeedbackData)
{
    if (!bInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("Feedback system not initialized"));
        return FString();
    }

    // 驗證反饋數據
    if (!ValidateFeedbackData(FeedbackData))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid feedback data"));
        return FString();
    }

    // 檢查重複反饋
    if (bEnableDuplicateDetection)
    {
        TArray<FString> Duplicates = CheckDuplicateFeedback(FeedbackData);
        if (Duplicates.Num() > 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("Found duplicate feedback: %s"), *Duplicates[0]);
            return Duplicates[0];
        }
    }

    // 創建反饋副本
    FFeedbackData NewFeedback = FeedbackData;
    NewFeedback.FeedbackID = GenerateFeedbackID();
    NewFeedback.SubmitTime = FDateTime::Now();
    NewFeedback.LastUpdateTime = FDateTime::Now();
    NewFeedback.Status = EFeedbackStatus::New;
    NewFeedback.bIsRead = false;

    // 自動分類和標記
    if (bEnableAutoCategorization)
    {
        AutoCategorizeFeedback();
        AnalyzeFeedbackContent(NewFeedback);
        AutoTagFeedback(NewFeedback);
    }

    // 保存反饋數據
    if (SaveFeedbackData(NewFeedback))
    {
        AllFeedback.Add(NewFeedback);
        UpdateStatistics();

        // 觸發事件
        OnFeedbackSubmitted.Broadcast(NewFeedback.FeedbackID);
        OnNewFeedback.Broadcast(NewFeedback);

        // 發送通知
        SendNotification(NewFeedback.FeedbackID, TEXT("New feedback submitted"));

        UE_LOG(LogTemp, Log, TEXT("Feedback submitted successfully: %s"), *NewFeedback.FeedbackID);
        return NewFeedback.FeedbackID;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save feedback data"));
        return FString();
    }
}

TArray<FFeedbackData> UUserFeedbackSystem::GetFeedbackList(EFeedbackType FilterType, EFeedbackStatus FilterStatus)
{
    TArray<FFeedbackData> FilteredList;

    for (const FFeedbackData& Feedback : AllFeedback)
    {
        bool bTypeMatch = (FilterType == EFeedbackType::Other) || (Feedback.FeedbackType == FilterType);
        bool bStatusMatch = (FilterStatus == EFeedbackStatus::New) || (Feedback.Status == FilterStatus);

        if (bTypeMatch && bStatusMatch)
        {
            FilteredList.Add(Feedback);
        }
    }

    // 按提交時間排序
    FilteredList.Sort([](const FFeedbackData& A, const FFeedbackData& B)
    {
        return A.SubmitTime > B.SubmitTime;
    });

    return FilteredList;
}

FFeedbackData UUserFeedbackSystem::GetFeedbackDetails(const FString& FeedbackID)
{
    for (const FFeedbackData& Feedback : AllFeedback)
    {
        if (Feedback.FeedbackID == FeedbackID)
        {
            return Feedback;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Feedback not found: %s"), *FeedbackID);
    return FFeedbackData();
}

bool UUserFeedbackSystem::UpdateFeedbackStatus(const FString& FeedbackID, EFeedbackStatus NewStatus)
{
    for (FFeedbackData& Feedback : AllFeedback)
    {
        if (Feedback.FeedbackID == FeedbackID)
        {
            Feedback.Status = NewStatus;
            Feedback.LastUpdateTime = FDateTime::Now();

            // 保存更新
            if (SaveFeedbackData(Feedback))
            {
                UpdateStatistics();
                OnFeedbackStatusUpdated.Broadcast(FeedbackID, NewStatus);
                
                UE_LOG(LogTemp, Log, TEXT("Feedback status updated: %s -> %d"), *FeedbackID, (int32)NewStatus);
                return true;
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Feedback not found for status update: %s"), *FeedbackID);
    return false;
}

bool UUserFeedbackSystem::AssignFeedback(const FString& FeedbackID, const FString& AssignedTo)
{
    for (FFeedbackData& Feedback : AllFeedback)
    {
        if (Feedback.FeedbackID == FeedbackID)
        {
            Feedback.AssignedTo = AssignedTo;
            Feedback.LastUpdateTime = FDateTime::Now();

            // 保存更新
            if (SaveFeedbackData(Feedback))
            {
                OnFeedbackAssigned.Broadcast(FeedbackID, AssignedTo);
                
                UE_LOG(LogTemp, Log, TEXT("Feedback assigned: %s -> %s"), *FeedbackID, *AssignedTo);
                return true;
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Feedback not found for assignment: %s"), *FeedbackID);
    return false;
}

bool UUserFeedbackSystem::AddComment(const FString& FeedbackID, const FString& Comment)
{
    // 這裡可以擴展為支持多條評論
    // 目前簡化為添加到描述中
    for (FFeedbackData& Feedback : AllFeedback)
    {
        if (Feedback.FeedbackID == FeedbackID)
        {
            Feedback.Description += FString::Printf(TEXT("\n\nComment [%s]:\n%s"), 
                *FDateTime::Now().ToString(), *Comment);
            Feedback.LastUpdateTime = FDateTime::Now();

            // 保存更新
            if (SaveFeedbackData(Feedback))
            {
                UE_LOG(LogTemp, Log, TEXT("Comment added to feedback: %s"), *FeedbackID);
                return true;
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Feedback not found for comment: %s"), *FeedbackID);
    return false;
}

FFeedbackStatistics UUserFeedbackSystem::GetStatistics()
{
    return Statistics;
}

FString UUserFeedbackSystem::ExportFeedbackData()
{
    TSharedPtr<FJsonObject> RootObject = MakeShareable(new FJsonObject);
    
    // 創建反饋數組
    TArray<TSharedPtr<FJsonValue>> FeedbackArray;
    
    for (const FFeedbackData& Feedback : AllFeedback)
    {
        TSharedPtr<FJsonObject> FeedbackObject = MakeShareable(new FJsonObject);
        
        FeedbackObject->SetStringField(TEXT("FeedbackID"), Feedback.FeedbackID);
        FeedbackObject->SetNumberField(TEXT("FeedbackType"), (int32)Feedback.FeedbackType);
        FeedbackObject->SetNumberField(TEXT("Priority"), (int32)Feedback.Priority);
        FeedbackObject->SetNumberField(TEXT("Status"), (int32)Feedback.Status);
        FeedbackObject->SetStringField(TEXT("Title"), Feedback.Title);
        FeedbackObject->SetStringField(TEXT("Description"), Feedback.Description);
        FeedbackObject->SetStringField(TEXT("UserInfo"), Feedback.UserInfo);
        FeedbackObject->SetStringField(TEXT("SystemInfo"), Feedback.SystemInfo);
        FeedbackObject->SetStringField(TEXT("GameVersion"), Feedback.GameVersion);
        FeedbackObject->SetStringField(TEXT("SubmitTime"), Feedback.SubmitTime.ToIso8601());
        FeedbackObject->SetStringField(TEXT("AssignedTo"), Feedback.AssignedTo);
        FeedbackObject->SetNumberField(TEXT("Rating"), Feedback.Rating);
        FeedbackObject->SetBoolField(TEXT("IsRead"), Feedback.bIsRead);
        
        // 添加標籤數組
        TArray<TSharedPtr<FJsonValue>> TagsArray;
        for (const FString& Tag : Feedback.Tags)
        {
            TagsArray.Add(MakeShareable(new FJsonValueString(Tag)));
        }
        FeedbackObject->SetArrayField(TEXT("Tags"), TagsArray);
        
        FeedbackArray.Add(MakeShareable(new FJsonValueObject(FeedbackObject)));
    }
    
    RootObject->SetArrayField(TEXT("Feedback"), FeedbackArray);
    
    // 添加統計信息
    TSharedPtr<FJsonObject> StatsObject = MakeShareable(new FJsonObject);
    StatsObject->SetNumberField(TEXT("TotalFeedback"), Statistics.TotalFeedback);
    StatsObject->SetNumberField(TEXT("AverageResponseTime"), Statistics.AverageResponseTime);
    StatsObject->SetNumberField(TEXT("ResolutionRate"), Statistics.ResolutionRate);
    StatsObject->SetNumberField(TEXT("UserSatisfaction"), Statistics.UserSatisfaction);
    RootObject->SetObjectField(TEXT("Statistics"), StatsObject);
    
    // 序列化為字符串
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer);
    
    return OutputString;
}

void UUserFeedbackSystem::AutoCategorizeFeedback()
{
    // 自動分類邏輯
    for (FFeedbackData& Feedback : AllFeedback)
    {
        if (Feedback.Status == EFeedbackStatus::New)
        {
            // 根據關鍵詞自動分類
            if (Feedback.Description.Contains(TEXT("crash")) || 
                Feedback.Description.Contains(TEXT("error")) || 
                Feedback.Description.Contains(TEXT("bug")))
            {
                Feedback.FeedbackType = EFeedbackType::Bug;
                Feedback.Priority = EFeedbackPriority::High;
            }
            else if (Feedback.Description.Contains(TEXT("slow")) || 
                     Feedback.Description.Contains(TEXT("lag")) || 
                     Feedback.Description.Contains(TEXT("performance")))
            {
                Feedback.FeedbackType = EFeedbackType::Performance;
                Feedback.Priority = EFeedbackPriority::Medium;
            }
            else if (Feedback.Description.Contains(TEXT("feature")) || 
                     Feedback.Description.Contains(TEXT("add")) || 
                     Feedback.Description.Contains(TEXT("request")))
            {
                Feedback.FeedbackType = EFeedbackType::Feature;
                Feedback.Priority = EFeedbackPriority::Low;
            }
            else if (Feedback.Description.Contains(TEXT("balance")) || 
                     Feedback.Description.Contains(TEXT("overpowered")) || 
                     Feedback.Description.Contains(TEXT("weak")))
            {
                Feedback.FeedbackType = EFeedbackType::Balance;
                Feedback.Priority = EFeedbackPriority::Medium;
            }
            else if (Feedback.Description.Contains(TEXT("UI")) || 
                     Feedback.Description.Contains(TEXT("interface")) || 
                     Feedback.Description.Contains(TEXT("menu")))
            {
                Feedback.FeedbackType = EFeedbackType::UI;
                Feedback.Priority = EFeedbackPriority::Medium;
            }
            else if (Feedback.Description.Contains(TEXT("sound")) || 
                     Feedback.Description.Contains(TEXT("audio")) || 
                     Feedback.Description.Contains(TEXT("music")))
            {
                Feedback.FeedbackType = EFeedbackType::Audio;
                Feedback.Priority = EFeedbackPriority::Low;
            }
            else if (Feedback.Description.Contains(TEXT("translation")) || 
                     Feedback.Description.Contains(TEXT("language")) || 
                     Feedback.Description.Contains(TEXT("localization")))
            {
                Feedback.FeedbackType = EFeedbackType::Localization;
                Feedback.Priority = EFeedbackPriority::Medium;
            }
        }
    }
}

void UUserFeedbackSystem::SendNotification(const FString& FeedbackID, const FString& Message)
{
    if (!bEnableEmailNotifications)
    {
        return;
    }

    // 獲取反饋詳情
    FFeedbackData Feedback = GetFeedbackDetails(FeedbackID);
    if (Feedback.FeedbackID.IsEmpty())
    {
        return;
    }

    // 構建郵件內容
    FString Subject = FString::Printf(TEXT("MingGoRTS Feedback: %s"), *Feedback.Title);
    FString Body = FString::Printf(TEXT("Feedback ID: %s\n\n"), *Feedback.FeedbackID);
    Body += FString::Printf(TEXT("Type: %s\n"), *UEnum::GetValueAsString(Feedback.FeedbackType));
    Body += FString::Printf(TEXT("Priority: %s\n"), *UEnum::GetValueAsString(Feedback.Priority));
    Body += FString::Printf(TEXT("Status: %s\n"), *UEnum::GetValueAsString(Feedback.Status));
    Body += FString::Printf(TEXT("User: %s\n\n"), *Feedback.UserInfo);
    Body += FString::Printf(TEXT("Description:\n%s\n\n"), *Feedback.Description);
    Body += FString::Printf(TEXT("System Info:\n%s\n"), *Feedback.SystemInfo);
    Body += FString::Printf(TEXT("Game Version: %s\n"), *Feedback.GameVersion);
    Body += FString::Printf(TEXT("Submit Time: %s\n"), *Feedback.SubmitTime.ToString());
    Body += FString::Printf(TEXT("\n%s"), *Message);

    // 發送郵件通知（這裡需要實際的郵件發送實現）
    SendEmailNotification(TEXT("support@minggorts.com"), Subject, Body);
}

TArray<FString> UUserFeedbackSystem::CheckDuplicateFeedback(const FFeedbackData& FeedbackData)
{
    TArray<FString> Duplicates;
    
    for (const FFeedbackData& ExistingFeedback : AllFeedback)
    {
        // 檢查標題相似度
        if (ExistingFeedback.Title.Equals(FeedbackData.Title, ESearchCase::IgnoreCase))
        {
            Duplicates.Add(ExistingFeedback.FeedbackID);
        }
        // 檢查描述相似度（簡化版本）
        else if (ExistingFeedback.Description.Equals(FeedbackData.Description, ESearchCase::IgnoreCase))
        {
            Duplicates.Add(ExistingFeedback.FeedbackID);
        }
    }
    
    return Duplicates;
}

FString UUserFeedbackSystem::GenerateFeedbackReport()
{
    FString Report = TEXT("=== MingGoRTS User Feedback Report ===\n\n");
    
    // 基本統計
    Report += FString::Printf(TEXT("Total Feedback: %d\n"), Statistics.TotalFeedback);
    Report += FString::Printf(TEXT("Resolution Rate: %.1f%%\n"), Statistics.ResolutionRate * 100);
    Report += FString::Printf(TEXT("User Satisfaction: %.1f/5.0\n"), Statistics.UserSatisfaction);
    Report += FString::Printf(TEXT("Average Response Time: %.1f hours\n"), Statistics.AverageResponseTime);
    Report += TEXT("\n");
    
    // 按類型統計
    Report += TEXT("Feedback by Type:\n");
    for (const auto& TypeStat : Statistics.FeedbackByType)
    {
        FString TypeName = UEnum::GetValueAsString(TypeStat.Key);
        Report += FString::Printf(TEXT("  %s: %d\n"), *TypeName, TypeStat.Value);
    }
    Report += TEXT("\n");
    
    // 按優先級統計
    Report += TEXT("Feedback by Priority:\n");
    for (const auto& PriorityStat : Statistics.FeedbackByPriority)
    {
        FString PriorityName = UEnum::GetValueAsString(PriorityStat.Key);
        Report += FString::Printf(TEXT("  %s: %d\n"), *PriorityName, PriorityStat.Value);
    }
    Report += TEXT("\n");
    
    // 按狀態統計
    Report += TEXT("Feedback by Status:\n");
    for (const auto& StatusStat : Statistics.FeedbackByStatus)
    {
        FString StatusName = UEnum::GetValueAsString(StatusStat.Key);
        Report += FString::Printf(TEXT("  %s: %d\n"), *StatusName, StatusStat.Value);
    }
    Report += TEXT("\n");
    
    // 最近反饋
    Report += TEXT("Recent Feedback:\n");
    int32 Count = 0;
    for (const FFeedbackData& Feedback : AllFeedback)
    {
        if (Count >= 10) break;
        
        Report += FString::Printf(TEXT("  [%s] %s - %s\n"), 
            *Feedback.SubmitTime.ToString(), *Feedback.FeedbackID, *Feedback.Title);
        Count++;
    }
    
    Report += TEXT("\n=== End of Report ===\n");
    
    return Report;
}

FString UUserFeedbackSystem::GenerateFeedbackID()
{
    return FGuid::NewGuid().ToString();
}

bool UUserFeedbackSystem::ValidateFeedbackData(const FFeedbackData& FeedbackData)
{
    if (FeedbackData.Title.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Feedback title is empty"));
        return false;
    }
    
    if (FeedbackData.Description.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Feedback description is empty"));
        return false;
    }
    
    if (FeedbackData.UserInfo.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Feedback user info is empty"));
    }
    
    return true;
}

bool UUserFeedbackSystem::SaveFeedbackData(const FFeedbackData& FeedbackData)
{
    // 保存到JSON文件
    FString JsonString = ExportFeedbackData();
    
    return FFileHelper::SaveStringToFile(JsonString, *FeedbackDataFilePath);
}

void UUserFeedbackSystem::LoadFeedbackData()
{
    if (!FFileHelper::FileExists(*FeedbackDataFilePath))
    {
        UE_LOG(LogTemp, Log, TEXT("Feedback data file does not exist, starting with empty data"));
        return;
    }
    
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FeedbackDataFilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load feedback data file"));
        return;
    }
    
    // 解析JSON
    TSharedPtr<FJsonObject> RootObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
    
    if (!FJsonSerializer::Deserialize(RootObject, Reader))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse feedback data JSON"));
        return;
    }
    
    // 加載反饋數據
    const TArray<TSharedPtr<FJsonValue>>* FeedbackArray;
    if (RootObject->TryGetArrayField(TEXT("Feedback"), FeedbackArray))
    {
        AllFeedback.Empty();
        
        for (const TSharedPtr<FJsonValue>& FeedbackValue : *FeedbackArray)
        {
            TSharedPtr<FJsonObject> FeedbackObject = FeedbackValue->AsObject();
            if (FeedbackObject.IsValid())
            {
                FFeedbackData Feedback;
                
                Feedback.FeedbackID = FeedbackObject->GetStringField(TEXT("FeedbackID"));
                Feedback.FeedbackType = (EFeedbackType)FeedbackObject->GetNumberField(TEXT("FeedbackType"));
                Feedback.Priority = (EFeedbackPriority)FeedbackObject->GetNumberField(TEXT("Priority"));
                Feedback.Status = (EFeedbackStatus)FeedbackObject->GetNumberField(TEXT("Status"));
                Feedback.Title = FeedbackObject->GetStringField(TEXT("Title"));
                Feedback.Description = FeedbackObject->GetStringField(TEXT("Description"));
                Feedback.UserInfo = FeedbackObject->GetStringField(TEXT("UserInfo"));
                Feedback.SystemInfo = FeedbackObject->GetStringField(TEXT("SystemInfo"));
                Feedback.GameVersion = FeedbackObject->GetStringField(TEXT("GameVersion"));
                Feedback.SubmitTime = FDateTime::ParseIso8601(*FeedbackObject->GetStringField(TEXT("SubmitTime")));
                Feedback.AssignedTo = FeedbackObject->GetStringField(TEXT("AssignedTo"));
                Feedback.Rating = FeedbackObject->GetNumberField(TEXT("Rating"));
                Feedback.bIsRead = FeedbackObject->GetBoolField(TEXT("IsRead"));
                
                // 加載標籤
                const TArray<TSharedPtr<FJsonValue>>* TagsArray;
                if (FeedbackObject->TryGetArrayField(TEXT("Tags"), TagsArray))
                {
                    for (const TSharedPtr<FJsonValue>& TagValue : *TagsArray)
                    {
                        Feedback.Tags.Add(TagValue->AsString());
                    }
                }
                
                AllFeedback.Add(Feedback);
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Loaded %d feedback entries"), AllFeedback.Num());
}

void UUserFeedbackSystem::UpdateStatistics()
{
    Statistics.TotalFeedback = AllFeedback.Num();
    
    // 清空統計數據
    Statistics.FeedbackByType.Empty();
    Statistics.FeedbackByPriority.Empty();
    Statistics.FeedbackByStatus.Empty();
    
    // 計算統計數據
    int32 ResolvedCount = 0;
    float TotalResponseTime = 0.0f;
    float TotalRating = 0.0f;
    int32 RatedCount = 0;
    
    for (const FFeedbackData& Feedback : AllFeedback)
    {
        // 按類型統計
        Statistics.FeedbackByType.FindOrAdd(Feedback.FeedbackType, 0)++;
        
        // 按優先級統計
        Statistics.FeedbackByPriority.FindOrAdd(Feedback.Priority, 0)++;
        
        // 按狀態統計
        Statistics.FeedbackByStatus.FindOrAdd(Feedback.Status, 0)++;
        
        // 計算解決率
        if (Feedback.Status == EFeedbackStatus::Resolved)
        {
            ResolvedCount++;
        }
        
        // 計算響應時間（簡化版本）
        if (Feedback.Status != EFeedbackStatus::New)
        {
            FTimespan ResponseTime = Feedback.LastUpdateTime - Feedback.SubmitTime;
            TotalResponseTime += ResponseTime.GetTotalHours();
        }
        
        // 計算用戶滿意度
        if (Feedback.Rating > 0.0f)
        {
            TotalRating += Feedback.Rating;
            RatedCount++;
        }
    }
    
    // 計算最終統計
    Statistics.ResolutionRate = (Statistics.TotalFeedback > 0) ? (float)ResolvedCount / Statistics.TotalFeedback : 0.0f;
    Statistics.AverageResponseTime = (AllFeedback.Num() > 0) ? TotalResponseTime / AllFeedback.Num() : 0.0f;
    Statistics.UserSatisfaction = (RatedCount > 0) ? TotalRating / RatedCount : 0.0f;
}

void UUserFeedbackSystem::SendEmailNotification(const FString& To, const FString& Subject, const FString& Body)
{
    // 這裡需要實際的郵件發送實現
    // 可以使用第三方郵件服務API
    UE_LOG(LogTemp, Log, TEXT("Email notification sent to %s: %s"), *To, *Subject);
    
    // 簡化版本：記錄到日誌
    UE_LOG(LogTemp, Log, TEXT("Email To: %s"), *To);
    UE_LOG(LogTemp, Log, TEXT("Subject: %s"), *Subject);
    UE_LOG(LogTemp, Log, TEXT("Body:\n%s"), *Body);
}

void UUserFeedbackSystem::AnalyzeFeedbackContent(FFeedbackData& FeedbackData)
{
    // 分析反饋內容，提取關鍵信息
    // 這裡可以添加更複雜的文本分析邏輯
    
    // 檢查是否包含系統信息
    if (FeedbackData.SystemInfo.IsEmpty())
    {
        // 自動收集系統信息
        FeedbackData.SystemInfo = FString::Printf(TEXT("OS: %s, CPU: %s, GPU: %s, RAM: %dMB"),
            *FPlatformProperties::PlatformName(),
            TEXT("Unknown"), // 需要實際獲取CPU信息
            TEXT("Unknown"), // 需要實際獲取GPU信息
            FPlatformMemory::GetStats().AvailablePhysical / (1024 * 1024)
        );
    }
    
    // 檢查是否包含遊戲版本信息
    if (FeedbackData.GameVersion.IsEmpty())
    {
        // 自動獲取遊戲版本
        FeedbackData.GameVersion = FEngineVersion::Current().ToString();
    }
}

void UUserFeedbackSystem::AutoTagFeedback(FFeedbackData& FeedbackData)
{
    // 自動添加標籤
    TArray<FString> AutoTags;
    
    // 根據反饋類型添加標籤
    switch (FeedbackData.FeedbackType)
    {
    case EFeedbackType::Bug:
        AutoTags.Add(TEXT("bug"));
        AutoTags.Add(TEXT("error"));
        break;
    case EFeedbackType::Feature:
        AutoTags.Add(TEXT("feature"));
        AutoTags.Add(TEXT("request"));
        break;
    case EFeedbackType::Performance:
        AutoTags.Add(TEXT("performance"));
        AutoTags.Add(TEXT("optimization"));
        break;
    case EFeedbackType::UI:
        AutoTags.Add(TEXT("ui"));
        AutoTags.Add(TEXT("interface"));
        break;
    case EFeedbackType::Audio:
        AutoTags.Add(TEXT("audio"));
        AutoTags.Add(TEXT("sound"));
        break;
    case EFeedbackType::Localization:
        AutoTags.Add(TEXT("localization"));
        AutoTags.Add(TEXT("translation"));
        break;
    default:
        break;
    }
    
    // 根據優先級添加標籤
    switch (FeedbackData.Priority)
    {
    case EFeedbackPriority::Critical:
        AutoTags.Add(TEXT("critical"));
        AutoTags.Add(TEXT("urgent"));
        break;
    case EFeedbackPriority::High:
        AutoTags.Add(TEXT("high"));
        break;
    case EFeedbackPriority::Medium:
        AutoTags.Add(TEXT("medium"));
        break;
    case EFeedbackPriority::Low:
        AutoTags.Add(TEXT("low"));
        break;
    default:
        break;
    }
    
    // 合併標籤
    for (const FString& Tag : AutoTags)
    {
        if (!FeedbackData.Tags.Contains(Tag))
        {
            FeedbackData.Tags.Add(Tag);
        }
    }
}

// ============================================================================
// FeedbackUIManager Implementation
// ============================================================================

UFeedbackUIManager::UFeedbackUIManager()
    : FeedbackSystem(nullptr)
    , CurrentUI(nullptr)
{
}

void UFeedbackUIManager::ShowFeedbackSubmitUI()
{
    if (!FeedbackSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("Feedback system not set"));
        return;
    }
    
    // 創建反饋提交界面
    CreateFeedbackSubmitUI();
}

void UFeedbackUIManager::ShowFeedbackListUI()
{
    if (!FeedbackSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("Feedback system not set"));
        return;
    }
    
    // 創建反饋列表界面
    CreateFeedbackListUI();
}

void UFeedbackUIManager::ShowFeedbackDetailsUI(const FString& FeedbackID)
{
    if (!FeedbackSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("Feedback system not set"));
        return;
    }
    
    // 創建反饋詳情界面
    CreateFeedbackDetailsUI();
}

void UFeedbackUIManager::ShowStatisticsUI()
{
    if (!FeedbackSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("Feedback system not set"));
        return;
    }
    
    // 創建統計界面
    CreateStatisticsUI();
}

void UFeedbackUIManager::CloseFeedbackUI()
{
    if (CurrentUI)
    {
        CurrentUI->RemoveFromParent();
        CurrentUI = nullptr;
    }
}

void UFeedbackUIManager::SetFeedbackSystem(UUserFeedbackSystem* InFeedbackSystem)
{
    FeedbackSystem = InFeedbackSystem;
}

void UFeedbackUIManager::CreateFeedbackSubmitUI()
{
    // 這裡需要實際的UI創建邏輯
    UE_LOG(LogTemp, Log, TEXT("Creating feedback submit UI"));
    
    // 關閉當前界面
    CloseFeedbackUI();
    
    // 創建新的提交界面
    if (FeedbackSubmitUIClass)
    {
        // 實際創建UI的邏輯
        // CurrentUI = CreateWidget<UUserWidget>(GetWorld(), FeedbackSubmitUIClass);
        // CurrentUI->AddToViewport();
    }
}

void UFeedbackUIManager::CreateFeedbackListUI()
{
    UE_LOG(LogTemp, Log, TEXT("Creating feedback list UI"));
    
    CloseFeedbackUI();
    
    if (FeedbackListUIClass)
    {
        // 實際創建UI的邏輯
        // CurrentUI = CreateWidget<UUserWidget>(GetWorld(), FeedbackListUIClass);
        // CurrentUI->AddToViewport();
    }
}

void UFeedbackUIManager::CreateFeedbackDetailsUI()
{
    UE_LOG(LogTemp, Log, TEXT("Creating feedback details UI"));
    
    CloseFeedbackUI();
    
    if (FeedbackDetailsUIClass)
    {
        // 實際創建UI的邏輯
        // CurrentUI = CreateWidget<UUserWidget>(GetWorld(), FeedbackDetailsUIClass);
        // CurrentUI->AddToViewport();
    }
}

void UFeedbackUIManager::CreateStatisticsUI()
{
    UE_LOG(LogTemp, Log, TEXT("Creating statistics UI"));
    
    CloseFeedbackUI();
    
    if (StatisticsUIClass)
    {
        // 實際創建UI的邏輯
        // CurrentUI = CreateWidget<UUserWidget>(GetWorld(), StatisticsUIClass);
        // CurrentUI->AddToViewport();
    }
}
