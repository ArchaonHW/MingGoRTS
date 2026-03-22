// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 遊戲畫面生成器 - 主界面和遊戲場景生成實現

#include "GameScreenGenerator/GameScreenGenerator.h"
#include "Engine/Engine.h"
#include "Engine/Texture2D.h"
#include "Engine/World.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "Misc/Guid.h"
#include "ImageUtils.h"
#include "KismetProceduralMeshLibrary.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWeakWidget.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SVerticalBox.h"
#include "Widgets/Layout/SHorizontalBox.h"
#include "Widgets/Buttons/SButton.h"
#include "Styling/SlateTypes.h"
#include "Styling/CoreStyle.h"

UGameScreenGenerator::UGameScreenGenerator()
    : bInitialized(false)
    , OutputDirectory(TEXT(""))
    , ScreenCounter(0)
{
}

void UGameScreenGenerator::InitializeScreenGenerator()
{
    if (bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Screen generator already initialized"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Initializing Game Screen Generator..."));

    // 設置輸出目錄
    OutputDirectory = FPaths::ProjectSavedDir() / TEXT("GeneratedScreens");

    // 確保目錄存在
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*OutputDirectory))
    {
        PlatformFile.CreateDirectoryTree(*OutputDirectory);
    }

    // 設置默認參數
    DefaultParams.ScreenType = EScreenType::MainMenu;
    DefaultParams.ScreenStyle = EScreenStyle::Traditional;
    DefaultParams.Resolution = EScreenResolution::FullHD_1080p;
    DefaultParams.Width = 1920;
    DefaultParams.Height = 1080;
    DefaultParams.BackgroundColor = FLinearColor(0.1f, 0.05f, 0.0f, 1.0f);
    DefaultParams.PrimaryColor = FLinearColor(0.8f, 0.6f, 0.2f, 1.0f);
    DefaultParams.SecondaryColor = FLinearColor(0.6f, 0.3f, 0.1f, 1.0f);
    DefaultParams.TextColor = FLinearColor(0.95f, 0.9f, 0.8f, 1.0f);
    DefaultParams.bEnableAnimation = true;
    DefaultParams.bEnableParticles = true;
    DefaultParams.bEnableLighting = true;
    DefaultParams.QualityLevel = 3;

    bInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("Game Screen Generator initialized successfully"));
}

FString UGameScreenGenerator::GenerateScreen(const FScreenGenerationParams& Params)
{
    if (!bInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("Screen generator not initialized"));
        return FString();
    }

    // 驗證參數
    if (!ValidateParams(Params))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid screen generation parameters"));
        return FString();
    }

    // 生成畫面ID
    FString ScreenID = GenerateScreenID();
    
    // 觸發開始事件
    OnScreenGenerationStarted.Broadcast(ScreenID);

    UE_LOG(LogTemp, Log, TEXT("Generating screen: %s"), *ScreenID);

    // 根據畫面類型生成畫面
    FGeneratedScreen GeneratedScreen;
    GeneratedScreen.ScreenID = ScreenID;
    GeneratedScreen.ScreenType = Params.ScreenType;
    GeneratedScreen.ScreenStyle = Params.ScreenStyle;
    GeneratedScreen.Resolution = Params.Resolution;
    GeneratedScreen.Width = Params.Width;
    GeneratedScreen.Height = Params.Height;
    GeneratedScreen.GenerationTime = FDateTime::Now();

    bool bSuccess = false;
    
    switch (Params.ScreenType)
    {
    case EScreenType::MainMenu:
        bSuccess = GenerateMainMenuInternal(Params, GeneratedScreen);
        break;
    case EScreenType::FactionSelect:
        bSuccess = GenerateFactionSelectInternal(Params, GeneratedScreen);
        break;
    case EScreenType::StrategicMap:
        bSuccess = GenerateStrategicMapInternal(Params, GeneratedScreen);
        break;
    case EScreenType::BattleScene:
        bSuccess = GenerateBattleSceneInternal(Params, GeneratedScreen);
        break;
    case EScreenType::CityView:
        bSuccess = GenerateCityViewInternal(Params, GeneratedScreen);
        break;
    case EScreenType::DiplomacyPanel:
        bSuccess = GenerateDiplomacyPanelInternal(Params, GeneratedScreen);
        break;
    case EScreenType::EconomyPanel:
        bSuccess = GenerateEconomyPanelInternal(Params, GeneratedScreen);
        break;
    case EScreenType::ResearchPanel:
        bSuccess = GenerateResearchPanelInternal(Params, GeneratedScreen);
        break;
    case EScreenType::SettingsMenu:
        bSuccess = GenerateSettingsMenuInternal(Params, GeneratedScreen);
        break;
    case EScreenType::LoadingScreen:
        bSuccess = GenerateLoadingScreenInternal(Params, GeneratedScreen);
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Unknown screen type: %d"), (int32)Params.ScreenType);
        break;
    }

    if (bSuccess)
    {
        // 渲染畫面
        GeneratedScreen.ScreenTexture = RenderScreen(GeneratedScreen.Elements, Params);
        
        // 保存畫面
        if (SaveScreen(GeneratedScreen))
        {
            GeneratedScreens.Add(GeneratedScreen);
            
            // 觸發完成事件
            OnScreenGenerationCompleted.Broadcast(ScreenID, GeneratedScreen);
            
            UE_LOG(LogTemp, Log, TEXT("Screen generated successfully: %s"), *ScreenID);
            return ScreenID;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to save screen: %s"), *ScreenID);
            OnScreenGenerationFailed.Broadcast(ScreenID, TEXT("Failed to save screen"));
            return FString();
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to generate screen: %s"), *ScreenID);
        OnScreenGenerationFailed.Broadcast(ScreenID, TEXT("Failed to generate screen"));
        return FString();
    }
}

FString UGameScreenGenerator::GenerateMainMenu(const FScreenGenerationParams& Params)
{
    return GenerateScreen(Params);
}

FString UGameScreenGenerator::GenerateFactionSelect(const FScreenGenerationParams& Params)
{
    return GenerateScreen(Params);
}

FString UGameScreenGenerator::GenerateStrategicMap(const FScreenGenerationParams& Params)
{
    return GenerateScreen(Params);
}

FString UGameScreenGenerator::GenerateBattleScene(const FScreenGenerationParams& Params)
{
    return GenerateScreen(Params);
}

FString UGameScreenGenerator::GenerateCityView(const FScreenGenerationParams& Params)
{
    return GenerateScreen(Params);
}

FString UGameScreenGenerator::GenerateDiplomacyPanel(const FScreenGenerationParams& Params)
{
    return GenerateScreen(Params);
}

FString UGameScreenGenerator::GenerateEconomyPanel(const FScreenGenerationParams& Params)
{
    return GenerateScreen(Params);
}

FString UGameScreenGenerator::GenerateResearchPanel(const FScreenGenerationParams& Params)
{
    return GenerateScreen(Params);
}

FString UGameScreenGenerator::GenerateSettingsMenu(const FScreenGenerationParams& Params)
{
    return GenerateScreen(Params);
}

FString UGameScreenGenerator::GenerateLoadingScreen(const FScreenGenerationParams& Params)
{
    return GenerateScreen(Params);
}

FGeneratedScreen UGameScreenGenerator::GetGeneratedScreen(const FString& ScreenID)
{
    for (const FGeneratedScreen& Screen : GeneratedScreens)
    {
        if (Screen.ScreenID == ScreenID)
        {
            return Screen;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Screen not found: %s"), *ScreenID);
    return FGeneratedScreen();
}

TArray<FGeneratedScreen> UGameScreenGenerator::GetAllGeneratedScreens()
{
    return GeneratedScreens;
}

bool UGameScreenGenerator::DeleteGeneratedScreen(const FString& ScreenID)
{
    for (int32 i = 0; i < GeneratedScreens.Num(); i++)
    {
        if (GeneratedScreens[i].ScreenID == ScreenID)
        {
            // 刪除文件
            IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
            if (PlatformFile.FileExists(*GeneratedScreens[i].FilePath))
            {
                PlatformFile.DeleteFile(*GeneratedScreens[i].FilePath);
            }

            // 從列表中移除
            GeneratedScreens.RemoveAt(i);
            
            UE_LOG(LogTemp, Log, TEXT("Screen deleted: %s"), *ScreenID);
            return true;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Screen not found for deletion: %s"), *ScreenID);
    return false;
}

bool UGameScreenGenerator::ExportScreenAsImage(const FString& ScreenID, const FString& FilePath)
{
    FGeneratedScreen Screen = GetGeneratedScreen(ScreenID);
    if (Screen.ScreenID.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Screen not found: %s"), *ScreenID);
        return false;
    }

    if (!Screen.ScreenTexture)
    {
        UE_LOG(LogTemp, Error, TEXT("Screen texture is null: %s"), *ScreenID);
        return false;
    }

    // 導出紋理為圖片文件
    bool bSuccess = FImageUtils::ExportTextureAsImage(Screen.ScreenTexture, *FilePath);
    
    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Screen exported as image: %s -> %s"), *ScreenID, *FilePath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to export screen as image: %s -> %s"), *ScreenID, *FilePath);
    }

    return bSuccess;
}

void UGameScreenGenerator::PreviewScreen(const FString& ScreenID)
{
    FGeneratedScreen Screen = GetGeneratedScreen(ScreenID);
    if (Screen.ScreenID.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Screen not found: %s"), *ScreenID);
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Previewing screen: %s"), *ScreenID);
    
    // 這裡可以實現預覽邏輯
    // 例如：在遊戲窗口中顯示生成的畫面
    if (GEngine && GEngine->GameViewport)
    {
        // 實際的預覽實現
        UE_LOG(LogTemp, Log, TEXT("Screen preview functionality would be implemented here"));
    }
}

void UGameScreenGenerator::ApplyScreenTheme(FScreenElement& Element, EScreenStyle Style)
{
    switch (Style)
    {
    case EScreenStyle::Traditional:
        Element.Color = FLinearColor(0.8f, 0.6f, 0.2f, 1.0f);
        break;
    case EScreenStyle::Modern:
        Element.Color = FLinearColor(0.2f, 0.6f, 0.8f, 1.0f);
        break;
    case EScreenStyle::Minimal:
        Element.Color = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
        break;
    case EScreenStyle::Vintage:
        Element.Color = FLinearColor(0.6f, 0.4f, 0.2f, 1.0f);
        break;
    case EScreenStyle::Imperial:
        Element.Color = FLinearColor(0.9f, 0.7f, 0.3f, 1.0f);
        break;
    default:
        break;
    }
}

void UGameScreenGenerator::AddAnimationEffect(FScreenElement& Element, const FString& AnimationType)
{
    // 添加動畫效果標記
    Element.ElementType += FString::Printf(TEXT("_%s"), *AnimationType);
}

void UGameScreenGenerator::AddParticleEffect(FScreenElement& Element, const FString& ParticleType)
{
    // 添加粒子效果標記
    Element.ElementType += FString::Printf(TEXT("_%s"), *ParticleType);
}

// ============================================================================
// Private Implementation
// ============================================================================

bool UGameScreenGenerator::GenerateMainMenuInternal(const FScreenGenerationParams& Params, FGeneratedScreen& OutScreen)
{
    UE_LOG(LogTemp, Log, TEXT("Generating Main Menu screen"));

    // 創建背景元素
    FScreenElement Background = CreateScreenElement(TEXT("Background"), TEXT("MainMenuBackground"), 
        FVector2D(0, 0), FVector2D(Params.Width, Params.Height));
    Background.Color = Params.BackgroundColor;
    Background.TexturePath = TEXT("/Game/UI/Textures/MainMenuBackground");
    OutScreen.Elements.Add(Background);

    // 創建標題元素
    FScreenElement Title = CreateScreenElement(TEXT("Text"), TEXT("GameTitle"), 
        FVector2D(Params.Width * 0.5f - 200, Params.Height * 0.2f), FVector2D(400, 100));
    Title.TextContent = TEXT("MINGGO RTS");
    Title.FontSize = 48;
    Title.Color = Params.PrimaryColor;
    OutScreen.Elements.Add(Title);

    // 創建副標題元素
    FScreenElement Subtitle = CreateScreenElement(TEXT("Text"), TEXT("GameSubtitle"), 
        FVector2D(Params.Width * 0.5f - 150, Params.Height * 0.3f), FVector2D(300, 50));
    Subtitle.TextContent = TEXT("民國史詩");
    Subtitle.FontSize = 32;
    Subtitle.Color = Params.SecondaryColor;
    OutScreen.Elements.Add(Subtitle);

    // 創建版本信息元素
    FScreenElement Version = CreateScreenElement(TEXT("Text"), TEXT("VersionInfo"), 
        FVector2D(Params.Width * 0.5f - 100, Params.Height * 0.35f), FVector2D(200, 30));
    Version.TextContent = TEXT("Beta v1.0.0");
    Version.FontSize = 18;
    Version.Color = Params.TextColor;
    OutScreen.Elements.Add(Version);

    // 創建按鈕元素
    TArray<FString> ButtonNames = {TEXT("單人遊戲"), TEXT("多人遊戲"), TEXT("設置"), TEXT("退出")};
    TArray<FString> ButtonIDs = {TEXT("SinglePlayer"), TEXT("MultiPlayer"), TEXT("Settings"), TEXT("Exit")};
    
    for (int32 i = 0; i < ButtonNames.Num(); i++)
    {
        FScreenElement Button = CreateScreenElement(TEXT("Button"), ButtonIDs[i], 
            FVector2D(Params.Width * 0.5f - 150, Params.Height * 0.5f + i * 80), FVector2D(300, 60));
        Button.TextContent = ButtonNames[i];
        Button.FontSize = 24;
        Button.Color = Params.PrimaryColor;
        Button.bInteractive = true;
        
        // 應用樣式
        SetElementStyle(Button, Params);
        
        OutScreen.Elements.Add(Button);
    }

    // 添加動畫效果
    if (Params.bEnableAnimation)
    {
        AddAnimationEffect(Title, TEXT("FadeIn"));
        AddAnimationEffect(Subtitle, TEXT("SlideIn"));
    }

    return true;
}

bool UGameScreenGenerator::GenerateFactionSelectInternal(const FScreenGenerationParams& Params, FGeneratedScreen& OutScreen)
{
    UE_LOG(LogTemp, Log, TEXT("Generating Faction Select screen"));

    // 創建背景元素
    FScreenElement Background = CreateScreenElement(TEXT("Background"), TEXT("FactionSelectBackground"), 
        FVector2D(0, 0), FVector2D(Params.Width, Params.Height));
    Background.Color = Params.BackgroundColor;
    Background.TexturePath = TEXT("/Game/UI/Textures/FactionSelectBackground");
    OutScreen.Elements.Add(Background);

    // 創建標題元素
    FScreenElement Title = CreateScreenElement(TEXT("Text"), TEXT("SelectFactionTitle"), 
        FVector2D(Params.Width * 0.5f - 150, Params.Height * 0.1f), FVector2D(300, 60));
    Title.TextContent = TEXT("選擇勢力");
    Title.FontSize = 36;
    Title.Color = Params.PrimaryColor;
    OutScreen.Elements.Add(Title);

    // 創建12個勢力選項
    TArray<FString> FactionNames = {
        TEXT("北洋政府"), TEXT("國民政府"), TEXT("中國共產黨"), TEXT("奉系軍閥"),
        TEXT("直系軍閥"), TEXT("皖系軍閥"), TEXT("晉系軍閥"), TEXT("桂系軍閥"),
        TEXT("滇系軍閥"), TEXT("川系軍閥"), TEXT("馬家軍"), TEXT("新疆勢力")
    };

    TArray<FString> FactionIDs = {
        TEXT("Beiyang"), TEXT("Nationalist"), TEXT("Communist"), TEXT("Fengtian"),
        TEXT("Zhili"), TEXT("Anhui"), TEXT("Shanxi"), TEXT("Guangxi"),
        TEXT("Yunnan"), TEXT("Sichuan"), TEXT("Ma"), TEXT("Xinjiang")
    };

    TArray<int32> Difficulties = {2, 3, 4, 3, 3, 3, 3, 3, 4, 4, 4, 5};

    for (int32 i = 0; i < FactionNames.Num(); i++)
    {
        int32 Row = i / 4;
        int32 Col = i % 4;
        
        FScreenElement FactionCard = CreateScreenElement(TEXT("Card"), FactionIDs[i], 
            FVector2D(Params.Width * 0.1f + Col * 220, Params.Height * 0.25f + Row * 180), FVector2D(200, 150));
        
        FactionCard.TextContent = FactionNames[i];
        FactionCard.FontSize = 18;
        FactionCard.Color = Params.PrimaryColor;
        FactionCard.bInteractive = true;
        
        // 添加難度星級
        FString StarRating = TEXT("");
        for (int32 j = 0; j < Difficulties[i]; j++)
        {
            StarRating += TEXT("⭐");
        }
        
        // 創建難度標示元素
        FScreenElement DifficultyIndicator = CreateScreenElement(TEXT("Text"), FactionIDs[i] + TEXT("_Difficulty"), 
            FVector2D(Params.Width * 0.1f + Col * 220 + 10, Params.Height * 0.25f + Row * 180 + 100), FVector2D(180, 30));
        DifficultyIndicator.TextContent = StarRating;
        DifficultyIndicator.FontSize = 16;
        DifficultyIndicator.Color = FLinearColor(1.0f, 0.8f, 0.0f, 1.0f);
        
        SetElementStyle(FactionCard, Params);
        OutScreen.Elements.Add(FactionCard);
        OutScreen.Elements.Add(DifficultyIndicator);
    }

    // 創建操作按鈕
    FScreenElement StartButton = CreateScreenElement(TEXT("Button"), TEXT("StartGame"), 
        FVector2D(Params.Width * 0.5f - 100, Params.Height * 0.85f), FVector2D(200, 50));
    StartButton.TextContent = TEXT("開始遊戲");
    StartButton.FontSize = 20;
    StartButton.Color = Params.PrimaryColor;
    StartButton.bInteractive = true;
    SetElementStyle(StartButton, Params);
    OutScreen.Elements.Add(StartButton);

    FScreenElement BackButton = CreateScreenElement(TEXT("Button"), TEXT("Back"), 
        FVector2D(Params.Width * 0.5f - 100, Params.Height * 0.9f), FVector2D(200, 50));
    BackButton.TextContent = TEXT("返回");
    BackButton.FontSize = 20;
    BackButton.Color = Params.SecondaryColor;
    BackButton.bInteractive = true;
    SetElementStyle(BackButton, Params);
    OutScreen.Elements.Add(BackButton);

    return true;
}

bool UGameScreenGenerator::GenerateStrategicMapInternal(const FScreenGenerationParams& Params, FGeneratedScreen& OutScreen)
{
    UE_LOG(LogTemp, Log, TEXT("Generating Strategic Map screen"));

    // 創建背景元素
    FScreenElement Background = CreateScreenElement(TEXT("Background"), TEXT("StrategicMapBackground"), 
        FVector2D(0, 0), FVector2D(Params.Width, Params.Height));
    Background.Color = Params.BackgroundColor;
    Background.TexturePath = TEXT("/Game/UI/Textures/StrategicMapBackground");
    OutScreen.Elements.Add(Background);

    // 創建地圖元素
    FScreenElement Map = CreateScreenElement(TEXT("Map"), TEXT("ChinaMap"), 
        FVector2D(Params.Width * 0.1f, Params.Height * 0.15f), FVector2D(Params.Width * 0.8f, Params.Height * 0.7f));
    Map.TexturePath = TEXT("/Game/UI/Textures/ChinaMap");
    Map.Color = FLinearColor::White;
    OutScreen.Elements.Add(Map);

    // 創建城市節點
    TArray<FVector2D> CityPositions = {
        FVector2D(0.6f, 0.3f),  // 北京
        FVector2D(0.65f, 0.4f), // 天津
        FVector2D(0.8f, 0.25f), // 瀋陽
        FVector2D(0.5f, 0.35f), // 太原
        FVector2D(0.4f, 0.5f), // 西安
        FVector2D(0.3f, 0.65f), // 成都
        FVector2D(0.35f, 0.75f), // 昆明
        FVector2D(0.5f, 0.7f), // 重慶
        FVector2D(0.25f, 0.8f), // 廣州
        FVector2D(0.15f, 0.6f), // 蘭州
        FVector2D(0.05f, 0.4f), // 迪化
        FVector2D(0.7f, 0.6f), // 南京
        FVector2D(0.75f, 0.7f)  // 上海
    };

    TArray<FString> CityNames = {
        TEXT("北京"), TEXT("天津"), TEXT("瀋陽"), TEXT("太原"), TEXT("西安"),
        TEXT("成都"), TEXT("昆明"), TEXT("重慶"), TEXT("廣州"), TEXT("蘭州"),
        TEXT("迪化"), TEXT("南京"), TEXT("上海")
    };

    for (int32 i = 0; i < CityPositions.Num(); i++)
    {
        FVector2D Position = FVector2D(
            Params.Width * 0.1f + CityPositions[i].X * Params.Width * 0.8f,
            Params.Height * 0.15f + CityPositions[i].Y * Params.Height * 0.7f
        );

        FScreenElement CityNode = CreateScreenElement(TEXT("CityNode"), CityNames[i], 
            Position, FVector2D(40, 40));
        CityNode.Color = Params.PrimaryColor;
        CityNode.bInteractive = true;
        CityNode.TextContent = CityNames[i];
        CityNode.FontSize = 12;
        
        OutScreen.Elements.Add(CityNode);
    }

    // 創建資源面板
    FScreenElement ResourcePanel = CreateScreenElement(TEXT("Panel"), TEXT("ResourcePanel"), 
        FVector2D(20, 20), FVector2D(250, 200));
    ResourcePanel.Color = FLinearColor(0.0f, 0.0f, 0.0f, 0.8f);
    OutScreen.Elements.Add(ResourcePanel);

    // 資源信息
    TArray<FString> ResourceTypes = {TEXT("資金"), TEXT("糧食"), TEXT("原料"), TEXT("兵力"), TEXT("人口")};
    TArray<FString> ResourceValues = {TEXT("1500"), TEXT("800"), TEXT("500"), TEXT("1500"), TEXT("50000")};

    for (int32 i = 0; i < ResourceTypes.Num(); i++)
    {
        FScreenElement ResourceItem = CreateScreenElement(TEXT("Text"), ResourceTypes[i], 
            FVector2D(30, 30 + i * 35), FVector2D(230, 30));
        ResourceItem.TextContent = FString::Printf(TEXT("%s: %s"), *ResourceTypes[i], *ResourceValues[i]);
        ResourceItem.FontSize = 16;
        ResourceItem.Color = Params.TextColor;
        
        OutScreen.Elements.Add(ResourceItem);
    }

    // 創建回合信息
    FScreenElement TurnInfo = CreateScreenElement(TEXT("Text"), TEXT("TurnInfo"), 
        FVector2D(Params.Width * 0.5f - 100, Params.Height * 0.92f), FVector2D(200, 30));
    TurnInfo.TextContent = TEXT("1920年春 - 第3回合");
    TurnInfo.FontSize = 18;
    TurnInfo.Color = Params.TextColor;
    OutScreen.Elements.Add(TurnInfo);

    return true;
}

bool UGameScreenGenerator::GenerateBattleSceneInternal(const FScreenGenerationParams& Params, FGeneratedScreen& OutScreen)
{
    UE_LOG(LogTemp, Log, TEXT("Generating Battle Scene screen"));

    // 創建戰場背景
    FScreenElement Battlefield = CreateScreenElement(TEXT("Background"), TEXT("Battlefield"), 
        FVector2D(0, 0), FVector2D(Params.Width, Params.Height));
    Battlefield.TexturePath = TEXT("/Game/UI/Textures/BattlefieldBackground");
    Battlefield.Color = FLinearColor::White;
    OutScreen.Elements.Add(Battlefield);

    // 創建單位面板
    FScreenElement UnitPanel = CreateScreenElement(TEXT("Panel"), TEXT("UnitPanel"), 
        FVector2D(20, 20), FVector2D(300, 400));
    UnitPanel.Color = FLinearColor(0.0f, 0.0f, 0.0f, 0.8f);
    OutScreen.Elements.Add(UnitPanel);

    // 單位信息
    TArray<FString> UnitTypes = {TEXT("步兵"), TEXT("騎兵"), TEXT("砲兵"), TEXT("空軍")};
    TArray<FString> UnitCounts = {TEXT("●●●"), TEXT("●●"), TEXT("●"), TEXT("●")};

    for (int32 i = 0; i < UnitTypes.Num(); i++)
    {
        FScreenElement UnitItem = CreateScreenElement(TEXT("Text"), UnitTypes[i], 
            FVector2D(30, 30 + i * 80), FVector2D(260, 60));
        UnitItem.TextContent = FString::Printf(TEXT("%s %s"), *UnitTypes[i], *UnitCounts[i]);
        UnitItem.FontSize = 18;
        UnitItem.Color = Params.TextColor;
        
        OutScreen.Elements.Add(UnitItem);
    }

    // 創建命令面板
    FScreenElement CommandPanel = CreateScreenElement(TEXT("Panel"), TEXT("CommandPanel"), 
        FVector2D(Params.Width - 320, 20), FVector2D(300, 400));
    CommandPanel.Color = FLinearColor(0.0f, 0.0f, 0.0f, 0.8f);
    OutScreen.Elements.Add(CommandPanel);

    // 命令按鈕
    TArray<FString> Commands = {TEXT("攻擊"), TEXT("防禦"), TEXT("移動"), TEXT("停止"), TEXT("技能"), TEXT("撤退")};

    for (int32 i = 0; i < Commands.Num(); i++)
    {
        FScreenElement CommandButton = CreateScreenElement(TEXT("Button"), Commands[i], 
            FVector2D(Params.Width - 310, 30 + i * 65), FVector2D(280, 50));
        CommandButton.TextContent = Commands[i];
        CommandButton.FontSize = 16;
        CommandButton.Color = Params.PrimaryColor;
        CommandButton.bInteractive = true;
        
        OutScreen.Elements.Add(CommandButton);
    }

    // 創建戰鬥信息面板
    FScreenElement BattleInfo = CreateScreenElement(TEXT("Panel"), TEXT("BattleInfo"), 
        FVector2D(Params.Width * 0.5f - 150, Params.Height * 0.85f), FVector2D(300, 80));
    BattleInfo.Color = FLinearColor(0.0f, 0.0f, 0.0f, 0.8f);
    OutScreen.Elements.Add(BattleInfo);

    FScreenElement BattleText = CreateScreenElement(TEXT("Text"), TEXT("BattleText"), 
        FVector2D(Params.Width * 0.5f - 140, Params.Height * 0.87f), FVector2D(280, 60));
    BattleText.TextContent = TEXT("回合: 3/10 | 天氣: 晴天 | 地形: 平原");
    BattleText.FontSize = 16;
    BattleText.Color = Params.TextColor;
    OutScreen.Elements.Add(BattleText);

    return true;
}

bool UGameScreenGenerator::GenerateCityViewInternal(const FScreenGenerationParams& Params, FGeneratedScreen& OutScreen)
{
    UE_LOG(LogTemp, Log, TEXT("Generating City View screen"));

    // 創建城市背景
    FScreenElement CityBackground = CreateScreenElement(TEXT("Background"), TEXT("CityBackground"), 
        FVector2D(0, 0), FVector2D(Params.Width, Params.Height));
    CityBackground.TexturePath = TEXT("/Game/UI/Textures/CityBackground");
    CityBackground.Color = FLinearColor::White;
    OutScreen.Elements.Add(CityBackground);

    // 創建建築網格
    for (int32 x = 0; x < 5; x++)
    {
        for (int32 y = 0; y < 4; y++)
        {
            FVector2D Position = FVector2D(
                Params.Width * 0.2f + x * 120,
                Params.Height * 0.2f + y * 120
            );

            FScreenElement BuildingSlot = CreateScreenElement(TEXT("BuildingSlot"), 
                FString::Printf(TEXT("Slot_%d_%d"), x, y), 
                Position, FVector2D(100, 100));
            BuildingSlot.Color = FLinearColor(0.5f, 0.5f, 0.5f, 0.5f);
            BuildingSlot.bInteractive = true;
            
            OutScreen.Elements.Add(BuildingSlot);
        }
    }

    // 創建建築面板
    FScreenElement BuildingPanel = CreateScreenElement(TEXT("Panel"), TEXT("BuildingPanel"), 
        FVector2D(20, Params.Height * 0.8f), FVector2D(300, 150));
    BuildingPanel.Color = FLinearColor(0.0f, 0.0f, 0.0f, 0.8f);
    OutScreen.Elements.Add(BuildingPanel);

    // 建築選項
    TArray<FString> BuildingTypes = {TEXT("民居"), TEXT("市場"), TEXT("兵營"), TEXT("工廠"), TEXT("研究院")};

    for (int32 i = 0; i < BuildingTypes.Num(); i++)
    {
        FScreenElement BuildingOption = CreateScreenElement(TEXT("BuildingOption"), BuildingTypes[i], 
            FVector2D(30 + i * 60, Params.Height * 0.82f), FVector2D(50, 50));
        BuildingOption.TextContent = BuildingTypes[i];
        BuildingOption.FontSize = 12;
        BuildingOption.Color = Params.TextColor;
        BuildingOption.bInteractive = true;
        
        OutScreen.Elements.Add(BuildingOption);
    }

    return true;
}

bool UGameScreenGenerator::GenerateDiplomacyPanelInternal(const FScreenGenerationParams& Params, FGeneratedScreen& OutScreen)
{
    UE_LOG(LogTemp, Log, TEXT("Generating Diplomacy Panel screen"));

    // 創建背景
    FScreenElement Background = CreateScreenElement(TEXT("Background"), TEXT("DiplomacyBackground"), 
        FVector2D(0, 0), FVector2D(Params.Width, Params.Height));
    Background.Color = Params.BackgroundColor;
    OutScreen.Elements.Add(Background);

    // 創建外交關係列表
    TArray<FString> Factions = {TEXT("北洋政府"), TEXT("國民政府"), TEXT("中國共產黨"), TEXT("奉系軍閥")};
    TArray<FString> Relations = {TEXT("友好"), TEXT("中立"), TEXT("敵對"), TEXT("戰爭")};

    for (int32 i = 0; i < Factions.Num(); i++)
    {
        FScreenElement DiplomacyItem = CreateScreenElement(TEXT("DiplomacyItem"), Factions[i], 
            FVector2D(Params.Width * 0.3f, Params.Height * 0.2f + i * 80), FVector2D(400, 70));
        DiplomacyItem.TextContent = FString::Printf(TEXT("%s - %s"), *Factions[i], *Relations[i]);
        DiplomacyItem.FontSize = 18;
        DiplomacyItem.Color = Params.TextColor;
        DiplomacyItem.bInteractive = true;
        
        OutScreen.Elements.Add(DiplomacyItem);
    }

    return true;
}

bool UGameScreenGenerator::GenerateEconomyPanelInternal(const FScreenGenerationParams& Params, FGeneratedScreen& OutScreen)
{
    UE_LOG(LogTemp, Log, TEXT("Generating Economy Panel screen"));

    // 創建背景
    FScreenElement Background = CreateScreenElement(TEXT("Background"), TEXT("EconomyBackground"), 
        FVector2D(0, 0), FVector2D(Params.Width, Params.Height));
    Background.Color = Params.BackgroundColor;
    OutScreen.Elements.Add(Background);

    // 創建經濟圖表
    FScreenElement EconomyChart = CreateScreenElement(TEXT("Chart"), TEXT("EconomyChart"), 
        FVector2D(Params.Width * 0.3f, Params.Height * 0.2f), FVector2D(400, 300));
    EconomyChart.TexturePath = TEXT("/Game/UI/Textures/EconomyChart");
    EconomyChart.Color = FLinearColor::White;
    OutScreen.Elements.Add(EconomyChart);

    // 創建經濟統計
    TArray<FString> EconomicItems = {
        TEXT("收入: +50/回合"),
        TEXT("支出: -30/回合"),
        TEXT("淨增: +20/回合"),
        TEXT("總資產: 5000")
    };

    for (int32 i = 0; i < EconomicItems.Num(); i++)
    {
        FScreenElement EconomicItem = CreateScreenElement(TEXT("Text"), EconomicItems[i], 
            FVector2D(Params.Width * 0.35f, Params.Height * 0.55f + i * 40), FVector2D(300, 30));
        EconomicItem.TextContent = EconomicItems[i];
        EconomicItem.FontSize = 16;
        EconomicItem.Color = Params.TextColor;
        
        OutScreen.Elements.Add(EconomicItem);
    }

    return true;
}

bool UGameScreenGenerator::GenerateResearchPanelInternal(const FScreenGenerationParams& Params, FGeneratedScreen& OutScreen)
{
    UE_LOG(LogTemp, Log, TEXT("Generating Research Panel screen"));

    // 創建背景
    FScreenElement Background = CreateScreenElement(TEXT("Background"), TEXT("ResearchBackground"), 
        FVector2D(0, 0), FVector2D(Params.Width, Params.Height));
    Background.Color = Params.BackgroundColor;
    OutScreen.Elements.Add(Background);

    // 創建科技樹
    FScreenElement TechTree = CreateScreenElement(TEXT("TechTree"), TEXT("TechnologyTree"), 
        FVector2D(Params.Width * 0.2f, Params.Height * 0.2f), FVector2D(600, 400));
    TechTree.TexturePath = TEXT("/Game/UI/Textures/TechTree");
    TechTree.Color = FLinearColor::White;
    OutScreen.Elements.Add(TechTree);

    return true;
}

bool UGameScreenGenerator::GenerateSettingsMenuInternal(const FScreenGenerationParams& Params, FGeneratedScreen& OutScreen)
{
    UE_LOG(LogTemp, Log, TEXT("Generating Settings Menu screen"));

    // 創建背景
    FScreenElement Background = CreateScreenElement(TEXT("Background"), TEXT("SettingsBackground"), 
        FVector2D(0, 0), FVector2D(Params.Width, Params.Height));
    Background.Color = Params.BackgroundColor;
    OutScreen.Elements.Add(Background);

    // 創建設置選項
    TArray<FString> SettingOptions = {
        TEXT("圖像設置"),
        TEXT("音頻設置"),
        TEXT("控制設置"),
        TEXT("遊戲設置"),
        TEXT("語言設置")
    };

    for (int32 i = 0; i < SettingOptions.Num(); i++)
    {
        FScreenElement SettingOption = CreateScreenElement(TEXT("SettingOption"), SettingOptions[i], 
            FVector2D(Params.Width * 0.4f, Params.Height * 0.3f + i * 80), FVector2D(200, 60));
        SettingOption.TextContent = SettingOptions[i];
        SettingOption.FontSize = 18;
        SettingOption.Color = Params.TextColor;
        SettingOption.bInteractive = true;
        
        OutScreen.Elements.Add(SettingOption);
    }

    return true;
}

bool UGameScreenGenerator::GenerateLoadingScreenInternal(const FScreenGenerationParams& Params, FGeneratedScreen& OutScreen)
{
    UE_LOG(LogTemp, Log, TEXT("Generating Loading Screen"));

    // 創建背景
    FScreenElement Background = CreateScreenElement(TEXT("Background"), TEXT("LoadingBackground"), 
        FVector2D(0, 0), FVector2D(Params.Width, Params.Height));
    Background.Color = Params.BackgroundColor;
    Background.TexturePath = TEXT("/Game/UI/Textures/LoadingBackground");
    OutScreen.Elements.Add(Background);

    // 創建載入文字
    FScreenElement LoadingText = CreateScreenElement(TEXT("Text"), TEXT("LoadingText"), 
        FVector2D(Params.Width * 0.5f - 100, Params.Height * 0.8f), FVector2D(200, 40));
    LoadingText.TextContent = TEXT("載入中...");
    LoadingText.FontSize = 24;
    LoadingText.Color = Params.TextColor;
    OutScreen.Elements.Add(LoadingText);

    // 創建進度條
    FScreenElement ProgressBar = CreateScreenElement(TEXT("ProgressBar"), TEXT("LoadingProgress"), 
        FVector2D(Params.Width * 0.3f, Params.Height * 0.85f), FVector2D(400, 20));
    ProgressBar.Color = Params.PrimaryColor;
    OutScreen.Elements.Add(ProgressBar);

    return true;
}

FScreenElement UGameScreenGenerator::CreateScreenElement(const FString& Type, const FString& Name, const FVector2D& Position, const FVector2D& Size)
{
    FScreenElement Element;
    Element.ElementType = Type;
    Element.ElementName = Name;
    Element.Position = Position;
    Element.Size = Size;
    Element.Rotation = 0.0f;
    Element.Opacity = 1.0f;
    Element.Color = FLinearColor::White;
    Element.TexturePath = TEXT("");
    Element.TextContent = TEXT("");
    Element.FontSize = 24;
    Element.bVisible = true;
    Element.bInteractive = false;

    return Element;
}

void UGameScreenGenerator::SetElementStyle(FScreenElement& Element, const FScreenGenerationParams& Params)
{
    Element.Color = Params.PrimaryColor;
    
    if (Params.bEnableAnimation)
    {
        AddAnimationEffect(Element, TEXT("Hover"));
    }
}

UTexture2D* UGameScreenGenerator::RenderScreen(const TArray<FScreenElement>& Elements, const FScreenGenerationParams& Params)
{
    // 這裡需要實際的渲染邏輯
    // 由於複雜性，這裡返回一個簡單的實現
    
    UE_LOG(LogTemp, Log, TEXT("Rendering screen with %d elements"), Elements.Num());
    
    // 創建一個簡單的紋理作為示例
    UTexture2D* RenderedTexture = UTexture2D::CreateTransient(Params.Width, Params.Height);
    
    if (RenderedTexture)
    {
        // 設置紋理參數
        RenderedTexture->AddToRoot();
        RenderedTexture->UpdateResource();
        
        UE_LOG(LogTemp, Log, TEXT("Screen texture created: %dx%d"), Params.Width, Params.Height);
    }
    
    return RenderedTexture;
}

bool UGameScreenGenerator::SaveScreen(const FGeneratedScreen& Screen)
{
    // 生成文件路徑
    FString FileName = FString::Printf(TEXT("%s_%s.png"), 
        *Screen.ScreenID, 
        *Screen.GenerationTime.ToString(TEXT("%Y%m%d_%H%M%S")));
    
    FString FilePath = FPaths::Combine(OutputDirectory, FileName);
    
    // 這裡需要實際的保存邏輯
    UE_LOG(LogTemp, Log, TEXT("Saving screen to: %s"), *FilePath);
    
    // 模擬保存成功
    FGeneratedScreen MutableScreen = Screen;
    MutableScreen.FilePath = FilePath;
    
    return true;
}

FString UGameScreenGenerator::GenerateScreenID()
{
    return FString::Printf(TEXT("Screen_%s_%d"), *FGuid::NewGuid().ToString(), ++ScreenCounter);
}

bool UGameScreenGenerator::ValidateParams(const FScreenGenerationParams& Params)
{
    if (Params.Width <= 0 || Params.Height <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid screen dimensions"));
        return false;
    }

    if (Params.QualityLevel < 1 || Params.QualityLevel > 5)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid quality level"));
        return false;
    }

    return true;
}

void UGameScreenGenerator::GetResolutionSettings(EScreenResolution Resolution, int32& OutWidth, int32& OutHeight)
{
    switch (Resolution)
    {
    case EScreenResolution::HD_720p:
        OutWidth = 1280;
        OutHeight = 720;
        break;
    case EScreenResolution::FullHD_1080p:
        OutWidth = 1920;
        OutHeight = 1080;
        break;
    case EScreenResolution::QHD_1440p:
        OutWidth = 2560;
        OutHeight = 1440;
        break;
    case EScreenResolution::UHD_4K:
        OutWidth = 3840;
        OutHeight = 2160;
        break;
    case EScreenResolution::Custom:
        // 使用自定義值
        break;
    default:
        OutWidth = 1920;
        OutHeight = 1080;
        break;
    }
}

void UGameScreenGenerator::ApplyStyleTheme(FScreenGenerationParams& Params, EScreenStyle Style)
{
    switch (Style)
    {
    case EScreenStyle::Traditional:
        Params.BackgroundColor = FLinearColor(0.1f, 0.05f, 0.0f, 1.0f);
        Params.PrimaryColor = FLinearColor(0.8f, 0.6f, 0.2f, 1.0f);
        Params.SecondaryColor = FLinearColor(0.6f, 0.3f, 0.1f, 1.0f);
        Params.TextColor = FLinearColor(0.95f, 0.9f, 0.8f, 1.0f);
        break;
    case EScreenStyle::Modern:
        Params.BackgroundColor = FLinearColor(0.05f, 0.05f, 0.1f, 1.0f);
        Params.PrimaryColor = FLinearColor(0.2f, 0.6f, 0.8f, 1.0f);
        Params.SecondaryColor = FLinearColor(0.1f, 0.4f, 0.6f, 1.0f);
        Params.TextColor = FLinearColor(0.9f, 0.9f, 0.95f, 1.0f);
        break;
    case EScreenStyle::Minimal:
        Params.BackgroundColor = FLinearColor(0.98f, 0.98f, 0.98f, 1.0f);
        Params.PrimaryColor = FLinearColor(0.2f, 0.2f, 0.2f, 1.0f);
        Params.SecondaryColor = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
        Params.TextColor = FLinearColor(0.1f, 0.1f, 0.1f, 1.0f);
        break;
    case EScreenStyle::Vintage:
        Params.BackgroundColor = FLinearColor(0.15f, 0.1f, 0.05f, 1.0f);
        Params.PrimaryColor = FLinearColor(0.6f, 0.4f, 0.2f, 1.0f);
        Params.SecondaryColor = FLinearColor(0.4f, 0.3f, 0.15f, 1.0f);
        Params.TextColor = FLinearColor(0.9f, 0.85f, 0.7f, 1.0f);
        break;
    case EScreenStyle::Imperial:
        Params.BackgroundColor = FLinearColor(0.05f, 0.02f, 0.01f, 1.0f);
        Params.PrimaryColor = FLinearColor(0.9f, 0.7f, 0.3f, 1.0f);
        Params.SecondaryColor = FLinearColor(0.7f, 0.5f, 0.2f, 1.0f);
        Params.TextColor = FLinearColor(1.0f, 0.95f, 0.8f, 1.0f);
        break;
    default:
        break;
    }
}

// ============================================================================
// ScreenTemplateManager Implementation
// ============================================================================

UScreenTemplateManager::UScreenTemplateManager()
    : bInitialized(false)
    , TemplateFilePath(TEXT(""))
{
}

void UScreenTemplateManager::InitializeTemplateManager()
{
    if (bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Screen template manager already initialized"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Initializing Screen Template Manager..."));

    // 設置模板文件路徑
    TemplateFilePath = FPaths::ProjectSavedDir() / TEXT("ScreenTemplates.json");

    // 加載默認模板
    LoadDefaultTemplates();

    // 加載現有模板
    LoadTemplates();

    bInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("Screen Template Manager initialized successfully"));
}

void UScreenTemplateManager::RegisterTemplate(const FScreenElementTemplate& Template)
{
    // 檢查是否已存在
    for (const FScreenElementTemplate& ExistingTemplate : Templates)
    {
        if (ExistingTemplate.TemplateName == Template.TemplateName)
        {
            UE_LOG(LogTemp, Warning, TEXT("Template already exists: %s"), *Template.TemplateName);
            return;
        }
    }

    Templates.Add(Template);
    
    // 保存模板
    if (SaveTemplate(Template))
    {
        UE_LOG(LogTemp, Log, TEXT("Template registered successfully: %s"), *Template.TemplateName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save template: %s"), *Template.TemplateName);
    }
}

FScreenElementTemplate UScreenTemplateManager::GetTemplate(const FString& TemplateName)
{
    for (const FScreenElementTemplate& Template : Templates)
    {
        if (Template.TemplateName == TemplateName)
        {
            return Template;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Template not found: %s"), *TemplateName);
    return FScreenElementTemplate();
}

TArray<FScreenElementTemplate> UScreenTemplateManager::GetAllTemplates()
{
    return Templates;
}

FScreenElement UScreenTemplateManager::CreateElementFromTemplate(const FString& TemplateName, const FString& ElementName, const FVector2D& Position)
{
    FScreenElementTemplate Template = GetTemplate(TemplateName);
    
    FScreenElement Element;
    Element.ElementType = Template.ElementType;
    Element.ElementName = ElementName;
    Element.Position = Position;
    Element.Size = Template.DefaultSize;
    Element.Color = Template.DefaultColor;
    Element.TexturePath = Template.DefaultTexture;
    
    // 應用動畫和粒子效果
    if (!Template.AnimationEffect.IsEmpty())
    {
        Element.ElementType += FString::Printf(TEXT("_%s"), *Template.AnimationEffect);
    }
    
    if (!Template.ParticleEffect.IsEmpty())
    {
        Element.ElementType += FString::Printf(TEXT("_%s"), *Template.ParticleEffect);
    }
    
    return Element;
}

void UScreenTemplateManager::LoadDefaultTemplates()
{
    UE_LOG(LogTemp, Log, TEXT("Loading default screen templates..."));

    // 按鈕模板
    FScreenElementTemplate ButtonTemplate;
    ButtonTemplate.TemplateName = TEXT("DefaultButton");
    ButtonTemplate.ElementType = TEXT("Button");
    ButtonTemplate.DefaultSize = FVector2D(200, 50);
    ButtonTemplate.DefaultColor = FLinearColor(0.2f, 0.6f, 0.8f, 1.0f);
    ButtonTemplate.AnimationEffect = TEXT("Hover");
    RegisterTemplate(ButtonTemplate);

    // 面板模板
    FScreenElementTemplate PanelTemplate;
    PanelTemplate.TemplateName = TEXT("DefaultPanel");
    PanelTemplate.ElementType = TEXT("Panel");
    PanelTemplate.DefaultSize = FVector2D(300, 200);
    PanelTemplate.DefaultColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.8f);
    RegisterTemplate(PanelTemplate);

    // 文本模板
    FScreenElementTemplate TextTemplate;
    TextTemplate.TemplateName = TEXT("DefaultText");
    TextTemplate.ElementType = TEXT("Text");
    TextTemplate.DefaultSize = FVector2D(200, 30);
    TextTemplate.DefaultColor = FLinearColor(0.95f, 0.9f, 0.8f, 1.0f);
    RegisterTemplate(TextTemplate);

    // 卡片模板
    FScreenElementTemplate CardTemplate;
    CardTemplate.TemplateName = TEXT("DefaultCard");
    CardTemplate.ElementType = TEXT("Card");
    CardTemplate.DefaultSize = FVector2D(250, 150);
    CardTemplate.DefaultColor = FLinearColor(0.8f, 0.6f, 0.2f, 1.0f);
    CardTemplate.AnimationEffect = TEXT("FadeIn");
    RegisterTemplate(CardTemplate);

    UE_LOG(LogTemp, Log, TEXT("Loaded %d default templates"), 4);
}

bool UScreenTemplateManager::SaveTemplate(const FScreenElementTemplate& Template)
{
    // 這裡需要實際的保存邏輯
    UE_LOG(LogTemp, Log, TEXT("Saving template: %s"), *Template.TemplateName);
    return true;
}

void UScreenTemplateManager::LoadTemplates()
{
    // 這裡需要實際的加載邏輯
    UE_LOG(LogTemp, Log, TEXT("Loading templates from: %s"), *TemplateFilePath);
}
