#include "MingGoRTSIntroManager.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "TimerManager.h"

UMingGoRTSIntroManager::UMingGoRTSIntroManager()
{
    bIsIntroPlaying = false;
    IntroTimer = 0.0f;
    CurrentIntroStage = 0;
}

void UMingGoRTSIntroManager::StartIntroSequence()
{
    if (bIsIntroPlaying)
    {
        return;
    }

    bIsIntroPlaying = true;
    CurrentIntroStage = 0;
    IntroTimer = 0.0f;

    // 載入動畫資源
    LoadIntroAssets();

    // 開始第一階段
    ShowTitleCard();

    UE_LOG(LogTemp, Log, TEXT("MingGoRTS Intro Sequence Started"));
}

void UMingGoRTSIntroManager::SkipIntro()
{
    if (!bIsIntroPlaying)
    {
        return;
    }

    bIsIntroPlaying = false;
    CurrentIntroStage = 0;
    IntroTimer = 0.0f;

    // 直接跳到主選單
    TransitionToMainMenu();

    UE_LOG(LogTemp, Log, TEXT("Intro Sequence Skipped"));
}

void UMingGoRTSIntroManager::ShowTitleCard()
{
    if (!bIsIntroPlaying)
    {
        return;
    }

    CurrentIntroStage = 1;
    IntroTimer = 0.0f;

    // 顯示標題卡片
    DisplayIntroImage(0);
    PlayIntroSound(0);

    // 設置計時器
    if (UWorld* World = GEngine->GetCurrentPlayWorld())
    {
        FTimerHandle TimerHandle;
        World->GetTimerManager().SetTimer(TimerHandle, this, &UMingGoRTSIntroManager::ShowHistoricalBackground, 3.0f, false);
    }
}

void UMingGoRTSIntroManager::ShowHistoricalBackground()
{
    if (!bIsIntroPlaying)
    {
        return;
    }

    CurrentIntroStage = 2;
    IntroTimer = 0.0f;

    // 顯示歷史背景
    DisplayIntroImage(1);
    PlayIntroSound(1);

    // 設置計時器
    if (UWorld* World = GEngine->GetCurrentPlayWorld())
    {
        FTimerHandle TimerHandle;
        World->GetTimerManager().SetTimer(TimerHandle, this, &UMingGoRTSIntroManager::ShowGameTitle, 5.0f, false);
    }
}

void UMingGoRTSIntroManager::ShowGameTitle()
{
    if (!bIsIntroPlaying)
    {
        return;
    }

    CurrentIntroStage = 3;
    IntroTimer = 0.0f;

    // 顯示遊戲標題
    DisplayIntroImage(2);
    PlayIntroSound(2);

    // 設置計時器
    if (UWorld* World = GEngine->GetCurrentPlayWorld())
    {
        FTimerHandle TimerHandle;
        World->GetTimerManager().SetTimer(TimerHandle, this, &UMingGoRTSIntroManager::TransitionToMainMenu, 4.0f, false);
    }
}

void UMingGoRTSIntroManager::TransitionToMainMenu()
{
    bIsIntroPlaying = false;
    CurrentIntroStage = 0;
    IntroTimer = 0.0f;

    // 載入主選單
    if (UWorld* World = GEngine->GetCurrentPlayWorld())
    {
        UGameplayStatics::OpenLevel(World, TEXT("MainMenuLevel"), true);
    }

    UE_LOG(LogTemp, Log, TEXT("Intro Sequence Completed - Transitioning to Main Menu"));
}

void UMingGoRTSIntroManager::LoadIntroAssets()
{
    // 這裡應該載入實際的資源
    // 為了演示，我們使用靜態引用
    
    // 載入紋理
    static ConstructorHelpers::FObjectFinder<UTexture2D> TitleTexture(TEXT("/Game/Intro/Textures/T_TitleCard"));
    if (TitleTexture.Succeeded())
    {
        IntroTextures.Add(TitleTexture.Object);
    }

    static ConstructorHelpers::FObjectFinder<UTexture2D> HistoryTexture(TEXT("/Game/Intro/Textures/T_HistoricalBackground"));
    if (HistoryTexture.Succeeded())
    {
        IntroTextures.Add(HistoryTexture.Object);
    }

    static ConstructorHelpers::FObjectFinder<UTexture2D> GameTitleTexture(TEXT("/Game/Intro/Textures/T_GameTitle"));
    if (GameTitleTexture.Succeeded())
    {
        IntroTextures.Add(GameTitleTexture.Object);
    }

    // 載入音效
    static ConstructorHelpers::FObjectFinder<USoundBase> IntroMusic(TEXT("/Game/Intro/Audio/M_IntroMusic"));
    if (IntroMusic.Succeeded())
    {
        IntroSounds.Add(IntroMusic.Object);
    }
}

void UMingGoRTSIntroManager::PlayIntroSound(int32 StageIndex)
{
    if (StageIndex >= 0 && StageIndex < IntroSounds.Num())
    {
        if (USoundBase* Sound = IntroSounds[StageIndex])
        {
            if (UWorld* World = GEngine->GetCurrentPlayWorld())
            {
                // 播放音效
                UGameplayStatics::PlaySound2D(World, Sound, 1.0f);
            }
        }
    }
}

void UMingGoRTSIntroManager::DisplayIntroImage(int32 StageIndex)
{
    if (StageIndex >= 0 && StageIndex < IntroTextures.Num())
    {
        if (UTexture2D* Texture = IntroTextures[StageIndex])
        {
            // 這裡應該顯示紋理到 UI
            // 為了演示，我們只是記錄
            UE_LOG(LogTemp, Log, TEXT("Displaying Intro Image Stage %d"), StageIndex);
        }
    }
}
