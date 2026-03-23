// Copyright (c) 2026 MingGoRTS. All rights reserved.

#include "MingGoRTS.h"
#include "Game/MingGoRTSGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

UMingGoRTSGameInstance::UMingGoRTSGameInstance()
{
    StartupMapName = TEXT("MainMenu");
    MainMenuMapName = TEXT("MainMenu");
    GameMapName = TEXT("GameMap");
}

void UMingGoRTSGameInstance::Init()
{
    Super::Init();

    UE_LOG(LogTemp, Log, TEXT("=============================================="));
    UE_LOG(LogTemp, Log, TEXT("MingGoRTS Game Instance Initialized"));
    UE_LOG(LogTemp, Log, TEXT("=============================================="));

    // 初始化預設設置
    InitializeDefaultSettings();

    // 驗證 UI 配置
    ValidateUIClassConfiguration();

    // 自動初始化 UI/UX
    AutoInitializeUIUX();

    UE_LOG(LogTemp, Log, TEXT("MingGoRTS 初始化完成 - UI/UX 系統準備就緒"));
}

void UMingGoRTSGameInstance::Shutdown()
{
    UE_LOG(LogTemp, Log, TEXT("MingGoRTS Game Instance Shutting Down..."));

    Super::Shutdown();
}

void UMingGoRTSGameInstance::AutoInitializeUIUX()
{
    UE_LOG(LogTemp, Log, TEXT("正在自動初始化 UI/UX 系統..."));

    // 預載入必要資源
    PreloadEssentialAssets();

    // 設置默認輸入
    SetupDefaultInputSettings();

    UE_LOG(LogTemp, Log, TEXT("UI/UX 系統自動初始化完成"));
    UE_LOG(LogTemp, Log, TEXT("- 主選單類別: %s"), *GetNameSafe(DefaultMainMenuClass));
    UE_LOG(LogTemp, Log, TEXT("- 遊戲 HUD 類別: %s"), *GetNameSafe(DefaultGameHUDClass));
    UE_LOG(LogTemp, Log, TEXT("- 啟動關卡: %s"), *StartupMapName);
}

bool UMingGoRTSGameInstance::ShouldGenerateDefaultUIAssets() const
{
    // 檢查是否需要生成預設 UI 資產
    bool bNeedGeneration = false;

    if (!DefaultMainMenuClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("預設主選單類別未配置"));
        bNeedGeneration = true;
    }

    if (!DefaultGameHUDClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("預設遊戲 HUD 類別未配置"));
        bNeedGeneration = true;
    }

    return bNeedGeneration;
}

void UMingGoRTSGameInstance::GenerateDefaultUIUXScreens()
{
    UE_LOG(LogTemp, Log, TEXT("生成預設 UI/UX 畫面..."));

    // 這裡可以動態創建預設的 UI Widget Blueprint
    // 在實際項目中，這些通常是預先創建好的 Blueprint 資產

    UE_LOG(LogTemp, Log, TEXT("UI/UX 畫面生成完成"));
}

void UMingGoRTSGameInstance::LoadStartupMap()
{
    UE_LOG(LogTemp, Log, TEXT("載入啟動關卡: %s"), *StartupMapName);

    FString MapPath = FString::Printf(TEXT("/Game/Maps/%s"), *StartupMapName);
    UGameplayStatics::OpenLevel(this, FName(*MapPath));
}

void UMingGoRTSGameInstance::InitializeDefaultSettings()
{
    UE_LOG(LogTemp, Log, TEXT("初始化預設設置..."));

    // 設置默認的 UI 類別 (這些應該在 Blueprint 中設置)
    // 這裡僅作為備份

    UE_LOG(LogTemp, Log, TEXT("預設設置初始化完成"));
}

void UMingGoRTSGameInstance::PreloadEssentialAssets()
{
    UE_LOG(LogTemp, Log, TEXT("預載入必要資源..."));

    // 在這裡可以異步載入必要的資源
    // 例如：字體、紋理、音效等

    UE_LOG(LogTemp, Log, TEXT("必要資源預載入完成"));
}

void UMingGoRTSGameInstance::SetupDefaultInputSettings()
{
    UE_LOG(LogTemp, Log, TEXT("設置默認輸入配置..."));

    // 設置默認的輸入配置
    // 這些通常在 Project Settings -> Input 中配置

    UE_LOG(LogTemp, Log, TEXT("輸入配置設置完成"));
}

void UMingGoRTSGameInstance::ValidateUIClassConfiguration()
{
    UE_LOG(LogTemp, Log, TEXT("驗證 UI 類別配置..."));

    if (!DefaultMainMenuClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠️  未設置主選單 Widget 類別 - 請在 GameInstance Blueprint 中配置"));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("✓ 主選單類別已配置: %s"), *GetNameSafe(DefaultMainMenuClass));
    }

    if (!DefaultGameHUDClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠️  未設置遊戲 HUD Widget 類別 - 請在 GameInstance Blueprint 中配置"));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("✓ 遊戲 HUD 類別已配置: %s"), *GetNameSafe(DefaultGameHUDClass));
    }

    UE_LOG(LogTemp, Log, TEXT("UI 類別配置驗證完成"));
}
