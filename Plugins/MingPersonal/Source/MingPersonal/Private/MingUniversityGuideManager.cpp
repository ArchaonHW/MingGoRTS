#include "MingUniversityGuideManager.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

UMingUniversityGuideManager::UMingUniversityGuideManager()
    : bIsGuideActive(false)
    , bIsAudioPlaying(false)
    , CurrentContentIndex(0)
    , GuideWidget(nullptr)
    , CurrentAudioComponent(nullptr)
{
}

void UMingUniversityGuideManager::Initialize()
{
    // 初始化默認引導內容
    GenerateDefaultContent();
    
    UE_LOG(LogTemp, Log, TEXT("MingUniversityGuideManager initialized"));
}

void UMingUniversityGuideManager::Shutdown()
{
    StopUniversityGuide();
    
    if (GuideWidget)
    {
        GuideWidget->RemoveFromParent();
        GuideWidget = nullptr;
    }
    
    UE_LOG(LogTemp, Log, TEXT("MingUniversityGuideManager shutdown"));
}

void UMingUniversityGuideManager::SetGuideConfig(const FUniversityGuideConfig& NewConfig)
{
    GuideConfig = NewConfig;
    
    UE_LOG(LogTemp, Log, TEXT("Guide config updated: %s"), *GuideConfig.GuideDescription);
}

const FUniversityGuideConfig& UMingUniversityGuideManager::GetGuideConfig() const
{
    return GuideConfig;
}

void UMingUniversityGuideManager::StartUniversityGuide()
{
    if (bIsGuideActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("Guide is already active"));
        return;
    }
    
    bIsGuideActive = true;
    CurrentContentIndex = 0;
    
    // 觸發開始事件
    OnGuideStarted.Broadcast();
    
    // 顯示第一個內容
    ShowCurrentContent();
    
    UE_LOG(LogTemp, Log, TEXT("University guide started"));
}

void UMingUniversityGuideManager::StopUniversityGuide()
{
    if (!bIsGuideActive)
    {
        return;
    }
    
    bIsGuideActive = false;
    
    // 停止音頻
    StopAudio();
    
    // 隱藏Widget
    if (GuideWidget)
    {
        GuideWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    
    // 觸發停止事件
    OnGuideStopped.Broadcast();
    
    UE_LOG(LogTemp, Log, TEXT("University guide stopped"));
}

void UMingUniversityGuideManager::PauseUniversityGuide()
{
    if (bIsGuideActive)
    {
        bIsGuideActive = false;
        PauseAudio();
        
        UE_LOG(LogTemp, Log, TEXT("University guide paused"));
    }
}

void UMingUniversityGuideManager::ResumeUniversityGuide()
{
    if (!bIsGuideActive && GuideContents.Num() > 0)
    {
        bIsGuideActive = true;
        ResumeAudio();
        
        UE_LOG(LogTemp, Log, TEXT("University guide resumed"));
    }
}

bool UMingUniversityGuideManager::IsGuideActive() const
{
    return bIsGuideActive;
}

void UMingUniversityGuideManager::NavigateToNextContent()
{
    if (!bIsGuideActive)
    {
        return;
    }
    
    if (CurrentContentIndex < GuideContents.Num() - 1)
    {
        CurrentContentIndex++;
        ShowCurrentContent();
        
        // 觸發導航事件
        OnGuideNavigated.Broadcast(CurrentContentIndex, GuideContents[CurrentContentIndex].ContentTitle);
        
        UE_LOG(LogTemp, Log, TEXT("Navigated to content %d: %s"), CurrentContentIndex, *GuideContents[CurrentContentIndex].ContentTitle);
    }
    else
    {
        // 已到達最後一個內容，結束引導
        OnGuideCompleted.Broadcast();
        StopUniversityGuide();
        
        UE_LOG(LogTemp, Log, TEXT("University guide completed"));
    }
}

void UMingUniversityGuideManager::NavigateToPreviousContent()
{
    if (!bIsGuideActive)
    {
        return;
    }
    
    if (CurrentContentIndex > 0)
    {
        CurrentContentIndex--;
        ShowCurrentContent();
        
        // 觸發導航事件
        OnGuideNavigated.Broadcast(CurrentContentIndex, GuideContents[CurrentContentIndex].ContentTitle);
        
        UE_LOG(LogTemp, Log, TEXT("Navigated to content %d: %s"), CurrentContentIndex, *GuideContents[CurrentContentIndex].ContentTitle);
    }
}

void UMingUniversityGuideManager::NavigateToContent(int32 ContentIndex)
{
    if (!bIsGuideActive)
    {
        return;
    }
    
    if (ContentIndex >= 0 && ContentIndex < GuideContents.Num())
    {
        CurrentContentIndex = ContentIndex;
        ShowCurrentContent();
        
        // 觸發導航事件
        OnGuideNavigated.Broadcast(CurrentContentIndex, GuideContents[CurrentContentIndex].ContentTitle);
        
        UE_LOG(LogTemp, Log, TEXT("Navigated to content %d: %s"), CurrentContentIndex, *GuideContents[CurrentContentIndex].ContentTitle);
    }
}

int32 UMingUniversityGuideManager::GetCurrentContentIndex() const
{
    return CurrentContentIndex;
}

FUniversityGuideContent UMingUniversityGuideManager::GetCurrentContent() const
{
    if (GuideContents.IsValidIndex(CurrentContentIndex))
    {
        return GuideContents[CurrentContentIndex];
    }
    return FUniversityGuideContent();
}

int32 UMingUniversityGuideManager::GetTotalContentCount() const
{
    return GuideContents.Num();
}

float UMingUniversityGuideManager::GetGuideProgress() const
{
    if (GuideContents.Num() == 0)
    {
        return 0.0f;
    }
    
    return static_cast<float>(CurrentContentIndex + 1) / static_cast<float>(GuideContents.Num());
}

TArray<FUniversityGuideContent> UMingUniversityGuideManager::GetAllContents() const
{
    return GuideContents;
}

void UMingUniversityGuideManager::AddGuideContent(const FUniversityGuideContent& Content)
{
    GuideContents.Add(Content);
    
    UE_LOG(LogTemp, Log, TEXT("Added guide content: %s"), *Content.ContentTitle);
}

void UMingUniversityGuideManager::RemoveGuideContent(int32 ContentIndex)
{
    if (GuideContents.IsValidIndex(ContentIndex))
    {
        GuideContents.RemoveAt(ContentIndex);
        
        // 調整當前索引
        if (CurrentContentIndex >= GuideContents.Num())
        {
            CurrentContentIndex = FMath::Max(0, GuideContents.Num() - 1);
        }
        
        UE_LOG(LogTemp, Log, TEXT("Removed guide content at index %d"), ContentIndex);
    }
}

void UMingUniversityGuideManager::ClearAllContents()
{
    GuideContents.Empty();
    CurrentContentIndex = 0;
    
    UE_LOG(LogTemp, Log, TEXT("Cleared all guide contents"));
}

void UMingUniversityGuideManager::PlayAudio()
{
    if (!bIsGuideActive || GuideContents.Num() == 0)
    {
        return;
    }
    
    FUniversityGuideContent CurrentContent = GetCurrentContent();
    if (!CurrentContent.AudioFilePath.IsEmpty())
    {
        // 播放音頻邏輯
        USoundCue* SoundCue = LoadObject<USoundCue>(nullptr, *CurrentContent.AudioFilePath);
        if (SoundCue)
        {
            if (CurrentAudioComponent)
            {
                CurrentAudioComponent->Stop();
            }
            
            CurrentAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), SoundCue);
            bIsAudioPlaying = true;
            
            UE_LOG(LogTemp, Log, TEXT("Playing audio: %s"), *CurrentContent.AudioFilePath);
        }
    }
}

void UMingUniversityGuideManager::StopAudio()
{
    if (CurrentAudioComponent)
    {
        CurrentAudioComponent->Stop();
        CurrentAudioComponent = nullptr;
    }
    
    bIsAudioPlaying = false;
    
    UE_LOG(LogTemp, Log, TEXT("Audio stopped"));
}

void UMingUniversityGuideManager::PauseAudio()
{
    if (CurrentAudioComponent)
    {
        CurrentAudioComponent->SetPaused(true);
        bIsAudioPlaying = false;
        
        UE_LOG(LogTemp, Log, TEXT("Audio paused"));
    }
}

void UMingUniversityGuideManager::ResumeAudio()
{
    if (CurrentAudioComponent)
    {
        CurrentAudioComponent->SetPaused(false);
        bIsAudioPlaying = true;
        
        UE_LOG(LogTemp, Log, TEXT("Audio resumed"));
    }
}

bool UMingUniversityGuideManager::IsAudioPlaying() const
{
    return bIsAudioPlaying;
}

void UMingUniversityGuideManager::ShowGuideWidget()
{
    // Widget創建和顯示邏輯
    // 實際項目中需要在這裡創建UMG Widget
    
    UE_LOG(LogTemp, Log, TEXT("Guide widget shown"));
}

void UMingUniversityGuideManager::HideGuideWidget()
{
    if (GuideWidget)
    {
        GuideWidget->SetVisibility(ESlateVisibility::Hidden);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Guide widget hidden"));
}

void UMingUniversityGuideManager::GenerateDefaultContent()
{
    // 清空現有內容
    GuideContents.Empty();
    
    // 內容1: 歡迎
    {
        FUniversityGuideContent Content;
        Content.ContentTitle = TEXT("歡迎來到MingGoRTS");
        Content.ContentDescription = TEXT("本引導將帶您了解遊戲的基本玩法和歷史背景。MingGoRTS是一款以民國時期為背景的即時戰略遊戲，融合了歷史教育與策略玩法。");
        Content.ContentIndex = 0;
        Content.bHasNextButton = true;
        Content.bHasPreviousButton = false;
        Content.bHasAudio = false;
        Content.bHasVideo = false;
        Content.bHasInteractiveElements = false;
        Content.EstimatedTimeSeconds = 30;
        GuideContents.Add(Content);
    }
    
    // 內容2: 基礎操作
    {
        FUniversityGuideContent Content;
        Content.ContentTitle = TEXT("基礎操作");
        Content.ContentDescription = TEXT("使用滑鼠左鍵選擇單位，右鍵移動或攻擊。滾輪縮放視角，按住中鍵拖動視角。鍵盤快捷鍵：Q-選擇所有單位，W-建造選單，E-經濟資訊。");
        Content.ContentIndex = 1;
        Content.bHasNextButton = true;
        Content.bHasPreviousButton = true;
        Content.bHasAudio = true;
        Content.bHasVideo = false;
        Content.bHasInteractiveElements = false;
        Content.EstimatedTimeSeconds = 45;
        GuideContents.Add(Content);
    }
    
    // 內容3: 資源系統
    {
        FUniversityGuideContent Content;
        Content.ContentTitle = TEXT("資源系統");
        Content.ContentDescription = TEXT("遊戲中有四種主要資源：黃金（貿易、建築）、食物（維持單位）、木材（建築、生產）、鐵礦（武器、裝備）。合理管理資源是勝利的關鍵。");
        Content.ContentIndex = 2;
        Content.bHasNextButton = true;
        Content.bHasPreviousButton = true;
        Content.bHasAudio = true;
        Content.bHasVideo = false;
        Content.bHasInteractiveElements = false;
        Content.EstimatedTimeSeconds = 60;
        GuideContents.Add(Content);
    }
    
    // 內容4: 建築系統
    {
        FUniversityGuideContent Content;
        Content.ContentTitle = TEXT("建築系統");
        Content.ContentDescription = TEXT("建築分為民居（人口）、農場（食物）、礦場（資源）、兵營（軍事）、市場（貿易）。點擊W打開建造選單，選擇建築後在合適位置放置。");
        Content.ContentIndex = 3;
        Content.bHasNextButton = true;
        Content.bHasPreviousButton = true;
        Content.bHasAudio = true;
        Content.bHasVideo = false;
        Content.bHasInteractiveElements = true;
        Content.EstimatedTimeSeconds = 60;
        GuideContents.Add(Content);
    }
    
    // 內容5: 戰鬥系統
    {
        FUniversityGuideContent Content;
        Content.ContentTitle = TEXT("戰鬥系統");
        Content.ContentDescription = TEXT("單位分為步兵、騎兵、弓兵、火器四大類。步兵克制弓兵，騎兵克制步兵，弓兵克制騎兵，火器對建築有效。利用地形和兵種克制取得優勢。");
        Content.ContentIndex = 4;
        Content.bHasNextButton = true;
        Content.bHasPreviousButton = true;
        Content.bHasAudio = true;
        Content.bHasVideo = true;
        Content.bHasInteractiveElements = true;
        Content.EstimatedTimeSeconds = 90;
        GuideContents.Add(Content);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Generated %d default guide contents"), GuideContents.Num());
}

void UMingUniversityGuideManager::ShowCurrentContent()
{
    if (!GuideContents.IsValidIndex(CurrentContentIndex))
    {
        return;
    }
    
    FUniversityGuideContent CurrentContent = GuideContents[CurrentContentIndex];
    
    // 更新Widget顯示
    if (GuideWidget)
    {
        // 實際項目中需要在這裡更新Widget的內容
    }
    
    // 播放音頻（如果有）
    if (CurrentContent.bHasAudio)
    {
        PlayAudio();
    }
    
    // 觸發內容更新事件
    OnGuideContentUpdated.Broadcast(CurrentContentIndex, CurrentContent.ContentTitle);
    
    UE_LOG(LogTemp, Log, TEXT("Showing content %d: %s"), CurrentContentIndex, *CurrentContent.ContentTitle);
}

FString UMingUniversityGuideManager::GenerateGuideSummary() const
{
    FString Summary = FString::Printf(TEXT("高校引導摘要：\n"));
    Summary += FString::Printf(TEXT("總內容數: %d\n"), GuideContents.Num());
    Summary += FString::Printf(TEXT("當前位置: %d\n"), CurrentContentIndex + 1);
    Summary += FString::Printf(TEXT("進度: %.1f%%\n"), GetGuideProgress() * 100.0f);
    Summary += FString::Printf(TEXT("活躍狀態: %s\n"), bIsGuideActive ? TEXT("進行中") : TEXT("已停止"));
    
    return Summary;
}

void UMingUniversityGuideManager::SaveGuideProgress(const FString& SaveSlotName)
{
    // 保存引導進度邏輯
    // 實際項目中需要使用SaveGame系統
    
    UE_LOG(LogTemp, Log, TEXT("Guide progress saved to slot: %s"), *SaveSlotName);
}

bool UMingUniversityGuideManager::LoadGuideProgress(const FString& SaveSlotName)
{
    // 載入引導進度邏輯
    // 實際項目中需要使用SaveGame系統
    
    UE_LOG(LogTemp, Log, TEXT("Guide progress loaded from slot: %s"), *SaveSlotName);
    return true;
}

void UMingUniversityGuideManager::ResetGuideProgress()
{
    CurrentContentIndex = 0;
    
    UE_LOG(LogTemp, Log, TEXT("Guide progress reset"));
}

void UMingUniversityGuideManager::SetGuideWidget(UMingUniversityGuideWidget* Widget)
{
    GuideWidget = Widget;
    
    UE_LOG(LogTemp, Log, TEXT("Guide widget set"));
}

UMingUniversityGuideWidget* UMingUniversityGuideManager::GetGuideWidget() const
{
    return GuideWidget;
}
