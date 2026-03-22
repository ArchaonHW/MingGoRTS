#include "MingGoRTSIntroGameMode.h"
#include "MingGoRTSIntroManager.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AMingGoRTSIntroGameMode::AMingGoRTSIntroGameMode()
{
    // 設置遊戲模式屬性
    bIntroCompleted = false;
    IntroManager = nullptr;
    IntroWidget = nullptr;

    // 禁用玩家控制器
    DefaultPawnClass = nullptr;
    PlayerControllerClass = nullptr;
    GameStateClass = nullptr;
}

void AMingGoRTSIntroGameMode::BeginPlay()
{
    Super::BeginPlay();

    // 設置開頭動畫系統
    SetupIntroManager();

    // 延遲開始動畫，確保所有組件都已初始化
    if (UWorld* World = GetWorld())
    {
        FTimerHandle TimerHandle;
        World->GetTimerManager().SetTimer(TimerHandle, this, &AMingGoRTSIntroGameMode::StartIntroSequence, 1.0f, false);
    }

    UE_LOG(LogTemp, Log, TEXT("MingGoRTS Intro GameMode Started"));
}

void AMingGoRTSIntroGameMode::StartIntroSequence()
{
    if (IntroManager)
    {
        IntroManager->StartIntroSequence();
    }

    UE_LOG(LogTemp, Log, TEXT("Intro Sequence Started"));
}

void AMingGoRTSIntroGameMode::SkipIntro()
{
    if (IntroManager)
    {
        IntroManager->SkipIntro();
    }

    HandleIntroCompleted();

    UE_LOG(LogTemp, Log, TEXT("Intro Sequence Skipped"));
}

void AMingGoRTSIntroGameMode::LoadMainMenu()
{
    // 載入主選單關卡
    UGameplayStatics::OpenLevel(this, TEXT("MainMenuLevel"), true);

    UE_LOG(LogTemp, Log, TEXT("Loading Main Menu"));
}

void AMingGoRTSIntroGameMode::LoadFirstLevel()
{
    // 載入第一個遊戲關卡
    UGameplayStatics::OpenLevel(this, TEXT("FirstGameLevel"), true);

    UE_LOG(LogTemp, Log, TEXT("Loading First Game Level"));
}

void AMingGoRTSIntroGameMode::SetupIntroManager()
{
    // 創建開頭動畫管理器
    IntroManager = NewObject<UMingGoRTSIntroManager>(this);
    if (IntroManager)
    {
        IntroManager->AddToRoot();
        UE_LOG(LogTemp, Log, TEXT("Intro Manager Created"));
    }
}

void AMingGoRTSIntroGameMode::SetupIntroWidget()
{
    // 暫時移除 Widget 設置，將在藍圖中實現
    UE_LOG(LogTemp, Log, TEXT("Intro Widget setup deferred to Blueprint"));
}

void AMingGoRTSIntroGameMode::HandleIntroCompleted()
{
    bIntroCompleted = true;

    // 清理開頭動畫資源
    if (IntroManager)
    {
        IntroManager->RemoveFromRoot();
        IntroManager = nullptr;
    }

    // 載入主選單
    LoadMainMenu();

    UE_LOG(LogTemp, Log, TEXT("Intro Sequence Completed"));
}
