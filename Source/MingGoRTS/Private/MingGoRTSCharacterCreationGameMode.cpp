#include "MingGoRTSCharacterCreationGameMode.h"
#include "MingGoRTSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

AMingGoRTSCharacterCreationGameMode::AMingGoRTSCharacterCreationGameMode()
{
    PrimaryActorTick.bCanEverTick = false;

    // 設置默認的 UI 類別
    CharacterCreationUIClass = nullptr;
    MainMenuUIClass = nullptr;
    GameStartLevelName = TEXT("MainGameLevel");
}

void AMingGoRTSCharacterCreationGameMode::BeginPlay()
{
    Super::BeginPlay();

    // 初始化角色創建管理器
    InitializeCharacterCreationManager();

    // 顯示角色創建 UI
    ShowCharacterCreationUI();
}

void AMingGoRTSCharacterCreationGameMode::ShowCharacterCreationUI()
{
    if (CharacterCreationUIClass)
    {
        // 移除當前 UI
        RemoveCurrentUI();

        // 創建並顯示角色創建 UI
        CurrentUI = CreateAndShowWidget(CharacterCreationUIClass);

        UE_LOG(LogTemp, Log, TEXT("角色創建 UI 已顯示"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("角色創建 UI 類別未設置"));
    }
}

void AMingGoRTSCharacterCreationGameMode::HideCharacterCreationUI()
{
    RemoveCurrentUI();
    UE_LOG(LogTemp, Log, TEXT("角色創建 UI 已隱藏"));
}

void AMingGoRTSCharacterCreationGameMode::OnCharacterCreationCompleted(const FMingCharacterData& CharacterData)
{
    UE_LOG(LogTemp, Log, TEXT("角色創建完成：%s"), *CharacterData.CharacterName);

    // 保存角色數據
    SaveCharacterToGameInstance(CharacterData);

    // 保存角色到存檔
    if (CharacterCreationManager)
    {
        FString SlotName = FString::Printf(TEXT("Character_%s"), *CharacterData.CharacterName);
        CharacterCreationManager->SaveCharacterToSlot(CharacterData, SlotName);
    }

    // 隱藏角色創建 UI
    HideCharacterCreationUI();

    // 開始遊戲
    StartGameWithCharacter(CharacterData);
}

void AMingGoRTSCharacterCreationGameMode::OnCharacterCreationCancelled()
{
    UE_LOG(LogTemp, Log, TEXT("角色創建已取消"));

    // 隱藏角色創建 UI
    HideCharacterCreationUI();

    // 返回主菜單
    ReturnToMainMenu();
}

void AMingGoRTSCharacterCreationGameMode::InitializeCharacterCreationManager()
{
    if (!CharacterCreationManager)
    {
        CharacterCreationManager = NewObject<UMingGoRTSCharacterCreationManager>(this);
        CharacterCreationManager->Initialize();

        // 綁定角色創建完成事件
        if (CharacterCreationManager)
        {
            // 注意：這裡需要根據實際的事件委託進行綁定
            // CharacterCreationManager->OnCharacterCreated.AddDynamic(this, &AMingGoRTSCharacterCreationGameMode::OnCharacterCreationCompleted);
        }

        UE_LOG(LogTemp, Log, TEXT("角色創建管理器已初始化"));
    }
}

UUserWidget* AMingGoRTSCharacterCreationGameMode::CreateAndShowWidget(TSubclassOf<class UUserWidget> WidgetClass)
{
    if (!WidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Widget 類別為空"));
        return nullptr;
    }

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Warning, TEXT("無法獲取 Player Controller"));
        return nullptr;
    }

    UUserWidget* Widget = CreateWidget<UUserWidget>(PlayerController, WidgetClass);
    if (Widget)
    {
        Widget->AddToViewport();
        
        // 設置輸入模式為 UI 模式
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(Widget->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PlayerController->SetInputMode(InputMode);
        PlayerController->bShowMouseCursor = true;
    }

    return Widget;
}

void AMingGoRTSCharacterCreationGameMode::RemoveCurrentUI()
{
    if (CurrentUI)
    {
        CurrentUI->RemoveFromViewport();
        CurrentUI = nullptr;

        // 恢復輸入模式
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            FInputModeGameOnly InputMode;
            PlayerController->SetInputMode(InputMode);
            PlayerController->bShowMouseCursor = false;
        }
    }
}

void AMingGoRTSCharacterCreationGameMode::StartGameWithCharacter(const FMingCharacterData& CharacterData)
{
    UE_LOG(LogTemp, Log, TEXT("開始遊戲，角色：%s"), *CharacterData.CharacterName);

    // 載入遊戲主場景
    if (!GameStartLevelName.IsEmpty())
    {
        UGameplayStatics::OpenLevel(this, *GameStartLevelName);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("遊戲開始場景名稱未設置"));
    }
}

void AMingGoRTSCharacterCreationGameMode::ReturnToMainMenu()
{
    UE_LOG(LogTemp, Log, TEXT("返回主菜單"));

    // 載入主菜單場景
    UGameplayStatics::OpenLevel(this, TEXT("MainMenuLevel"));
}

void AMingGoRTSCharacterCreationGameMode::SaveCharacterToGameInstance(const FMingCharacterData& CharacterData)
{
    UMingGoRTSGameInstance* GameInstance = GetGameInstance<UMingGoRTSGameInstance>();
    if (GameInstance)
    {
        // 假設 GameInstance 中有保存角色數據的方法
        // GameInstance->SetCurrentCharacterData(CharacterData);
        UE_LOG(LogTemp, Log, TEXT("角色數據已保存到 Game Instance"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("無法獲取 Game Instance"));
    }
}
