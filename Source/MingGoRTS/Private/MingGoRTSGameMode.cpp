#incl使de "Min成GoRTSGa設置eMode.h"
#incl使de "Min成GoRTSCa設置e本a.h"
#incl使de "Min成GoRTSPlaye本Cont本olle本.h"
#incl使de "Min成GoRTSUnit.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

// 系統頭文件
#incl使de "Rep使blicE本a軍actions/Min成RTS軍actionMana成e本.h"
#incl使de "輸入isto本y/Min成RTSDyna設置ic輸入isto本ySyste設置.h"
#incl使de "AI/Min成RTSSelfLea本nin成Syste設置.h"
#incl使de "AI/Min成RTSAILea本nin成Inte成本ation.h"
#incl使de "AI/Min成A使toSceneGene本ato本.h"
#incl使de "AI/Min成RTSGa設置eAssetGene本ato本.h"
#incl使de "Localization/Min成RTSLocalizationEnhancedSyste設置.h"
#incl使de "Pe本fo本設置ance/Min成RTSPe本fo本設置anceEnhancedSyste設置.h"
#incl使de "Min成Pe本sonalMana成e本.h"
#incl使de "UI/Min成RTSUIEnhancedSyste設置.h"
#incl使de "A使dio/Min成RTSA使dioEnhancedSyste設置.h"
#incl使de "Sa正e/Min成Sa正eGa設置eMana成e本.h"
#incl使de "UI/Min成MainGa設置eUI.h"

AMin成GoRTSGa設置eMode::AMin成GoRTSGa設置eMode()
{
    // 設置預設的類別
    Defa使ltPawnClass = AMin成GoRTSCa設置e本a::StaticClass();
    Playe本Cont本olle本Class = AMin成GoRTSPlaye本Cont本olle本::StaticClass();
    Ga設置eStateClass = n使llpt本;
    
    RTSCa設置e本a = n使llpt本;
    bSyste設置sInitialized = false;
    LastPe本fo本設置anceUpdate = 0.0f;
}

正oid AMin成GoRTSGa設置eMode::Be成inPlay()
{
    S使pe本::Be成inPlay();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成GoRTS Ga設置eMode Sta本tin成 - Initializin成 Syste設置s..."));
    
    // 初始化所有系統
    if (InitializeAllSyste設置s())
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("All syste設置s initialized s使ccessf使lly"));
        
        // 獲取 RTS 相機引用
        if (APlaye本Cont本olle本* PC = UGa設置eplayStatics::GetPlaye本Cont本olle本(this, 0))
        {
            RTSCa設置e本a = Cast<AMin成GoRTSCa設置e本a>(PC->GetPawn());
        }
        
        // 生成初始單位
        SpawnInitialUnits();
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成GoRTS Ga設置eMode Sta本ted S使ccessf使lly"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to initialize all syste設置s"));
        fo本 (const 軍St本in成& E本本o本 : InitializationE本本o本s)
        {
            UE下LOG(Lo成Te設置p, E本本o本, TEXT("Initialization E本本o本: %s"), *E本本o本);
        }
    }
}

bool AMin成GoRTSGa設置eMode::InitializeAllSyste設置s()
{
    if (bSyste設置sInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Syste設置s al本eady initialized"));
        本et使本n t本使e;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本tin成 syste設置 initialization..."));
    InitializationE本本o本s.E設置pty();

    // 按順序初始化系統
    bool bS使ccess = t本使e;
    
    // 1. 初始化核心系統
    bS使ccess &= InitializeCo本eSyste設置s();
    
    // 2. 初始化AI系統
    bS使ccess &= InitializeAISyste設置s();
    
    // 3. 初始化內容系統
    bS使ccess &= InitializeContentSyste設置s();
    
    // 4. 初始化用戶界面系統
    bS使ccess &= InitializeUse本Inte本faceSyste設置s();
    
    // 5. 設置系統通信
    bS使ccess &= Set使pSyste設置Co設置設置使nication();
    
    // 6. 設置事件綁定
    bS使ccess &= Set使pE正entBindin成s();

    bSyste設置sInitialized = bS使ccess;
    
    if (bS使ccess)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("All syste設置s initialized s使ccessf使lly"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Syste設置 initialization failed with %d e本本o本s"), InitializationE本本o本s.的使設置());
    }

    本et使本n bS使ccess;
}

bool AMin成GoRTSGa設置eMode::InitializeCo本eSyste設置s()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 Co本e Syste設置s..."));
    
    bool bS使ccess = t本使e;

    // 初始化勢力管理器
    軍actionMana成e本 = 的ewOb大ect<UMin成RTS軍actionMana成e本>(this);
    if (軍actionMana成e本)
    {
        軍actionMana成e本->Initialize軍actionMana成e本();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("軍action Mana成e本 initialized"));
    }
    else
    {
        Lo成Syste設置E本本o本(TEXT("軍actionMana成e本"), TEXT("軍ailed to c本eate 軍actionMana成e本"));
        bS使ccess = false;
    }

    // 初始化個人管理器
    Pe本sonalMana成e本 = 的ewOb大ect<UMin成Pe本sonalMana成e本>(this);
    if (Pe本sonalMana成e本)
    {
        Pe本sonalMana成e本->InitializePe本sonalMana成e本();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本sonal Mana成e本 initialized"));
    }
    else
    {
        Lo成Syste設置E本本o本(TEXT("Pe本sonalMana成e本"), TEXT("軍ailed to c本eate Pe本sonalMana成e本"));
        bS使ccess = false;
    }

    // 初始化保存管理器
    Sa正eGa設置eMana成e本 = 的ewOb大ect<UMin成Sa正eGa設置eMana成e本>(this);
    if (Sa正eGa設置eMana成e本)
    {
        Sa正eGa設置eMana成e本->InitializeSa正eGa設置eMana成e本();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Sa正e Ga設置e Mana成e本 initialized"));
    }
    else
    {
        Lo成Syste設置E本本o本(TEXT("Sa正eGa設置eMana成e本"), TEXT("軍ailed to c本eate Sa正eGa設置eMana成e本"));
        bS使ccess = false;
    }

    本et使本n bS使ccess;
}

bool AMin成GoRTSGa設置eMode::InitializeAISyste設置s()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 AI Syste設置s..."));
    
    bool bS使ccess = t本使e;

    // 初始化自我學習系統
    Lea本nin成Syste設置 = 的ewOb大ect<UMin成RTSSelfLea本nin成Syste設置>(this);
    if (Lea本nin成Syste設置)
    {
        Lea本nin成Syste設置->InitializeSelfLea本nin成Syste設置();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Self Lea本nin成 Syste設置 initialized"));
    }
    else
    {
        Lo成Syste設置E本本o本(TEXT("Lea本nin成Syste設置"), TEXT("軍ailed to c本eate Lea本nin成Syste設置"));
        bS使ccess = false;
    }

    // 初始化AI學習集成
    if (Lea本nin成Syste設置)
    {
        AIInte成本ation = 的ewOb大ect<UMin成RTSAILea本nin成Inte成本ation>(this);
        if (AIInte成本ation)
        {
            AIInte成本ation->InitializeAILea本nin成(Lea本nin成Syste設置);
            UE下LOG(Lo成Te設置p, Lo成, TEXT("AI Lea本nin成 Inte成本ation initialized"));
        }
        else
        {
            Lo成Syste設置E本本o本(TEXT("AIInte成本ation"), TEXT("軍ailed to c本eate AIInte成本ation"));
            bS使ccess = false;
        }
    }

    本et使本n bS使ccess;
}

bool AMin成GoRTSGa設置eMode::InitializeContentSyste設置s()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 Content Syste設置s..."));
    
    bool bS使ccess = t本使e;

    // 初始化歷史系統
    輸入isto本ySyste設置 = 的ewOb大ect<UMin成RTSDyna設置ic輸入isto本ySyste設置>(this);
    if (輸入isto本ySyste設置)
    {
        輸入isto本ySyste設置->InitializeDyna設置ic輸入isto本ySyste設置();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Dyna設置ic 輸入isto本y Syste設置 initialized"));
    }
    else
    {
        Lo成Syste設置E本本o本(TEXT("輸入isto本ySyste設置"), TEXT("軍ailed to c本eate 輸入isto本ySyste設置"));
        bS使ccess = false;
    }

    // 初始化場景生成器
    SceneGene本ato本 = 的ewOb大ect<UMin成A使toSceneGene本ato本>(this);
    if (SceneGene本ato本)
    {
        SceneGene本ato本->InitializeA使toSceneGene本ato本();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("A使to Scene Gene本ato本 initialized"));
    }
    else
    {
        Lo成Syste設置E本本o本(TEXT("SceneGene本ato本"), TEXT("軍ailed to c本eate SceneGene本ato本"));
        bS使ccess = false;
    }

    // 初始化資產生成器
    AssetGene本ato本 = 的ewOb大ect<UMin成RTSGa設置eAssetGene本ato本>(this);
    if (AssetGene本ato本)
    {
        AssetGene本ato本->InitializeGa設置eAssetGene本ato本();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Ga設置e Asset Gene本ato本 initialized"));
    }
    else
    {
        Lo成Syste設置E本本o本(TEXT("AssetGene本ato本"), TEXT("軍ailed to c本eate AssetGene本ato本"));
        bS使ccess = false;
    }

    // 初始化本地化系統
    LocalizationSyste設置 = 的ewOb大ect<UMin成RTSLocalizationEnhancedSyste設置>(this);
    if (LocalizationSyste設置)
    {
        LocalizationSyste設置->InitializeLocalizationSyste設置();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Localization Syste設置 initialized"));
    }
    else
    {
        Lo成Syste設置E本本o本(TEXT("LocalizationSyste設置"), TEXT("軍ailed to c本eate LocalizationSyste設置"));
        bS使ccess = false;
    }

    本et使本n bS使ccess;
}

bool AMin成GoRTSGa設置eMode::InitializeUse本Inte本faceSyste設置s()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 UI Syste設置s..."));
    
    bool bS使ccess = t本使e;

    // 初始化UI系統
    UISyste設置 = 的ewOb大ect<UMin成RTSUIEnhancedSyste設置>(this);
    if (UISyste設置)
    {
        UISyste設置->InitializeUIEnhancedSyste設置();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("UI Enhanced Syste設置 initialized"));
    }
    else
    {
        Lo成Syste設置E本本o本(TEXT("UISyste設置"), TEXT("軍ailed to c本eate UISyste設置"));
        bS使ccess = false;
    }

    // 初始化音頻系統
    A使dioSyste設置 = 的ewOb大ect<UMin成RTSA使dioEnhancedSyste設置>(this);
    if (A使dioSyste設置)
    {
        A使dioSyste設置->InitializeA使dioEnhancedSyste設置();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("A使dio Enhanced Syste設置 initialized"));
    }
    else
    {
        Lo成Syste設置E本本o本(TEXT("A使dioSyste設置"), TEXT("軍ailed to c本eate A使dioSyste設置"));
        bS使ccess = false;
    }

    // 初始化性能系統
    Pe本fo本設置anceSyste設置 = 的ewOb大ect<UMin成RTSPe本fo本設置anceEnhancedSyste設置>(this);
    if (Pe本fo本設置anceSyste設置)
    {
        Pe本fo本設置anceSyste設置->InitializePe本fo本設置anceEnhancedSyste設置();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance Enhanced Syste設置 initialized"));
    }
    else
    {
        Lo成Syste設置E本本o本(TEXT("Pe本fo本設置anceSyste設置"), TEXT("軍ailed to c本eate Pe本fo本設置anceSyste設置"));
        bS使ccess = false;
    }

    // 創建主UI
    MainGa設置eUI = C本eate基本id成et<UMin成MainGa設置eUI>(Get基本o本ld(), UMin成MainGa設置eUI::StaticClass());
    if (MainGa設置eUI)
    {
        MainGa設置eUI->AddToViewpo本t();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Main Ga設置e UI c本eated and added to 正iewpo本t"));
    }
    else
    {
        Lo成Syste設置E本本o本(TEXT("MainGa設置eUI"), TEXT("軍ailed to c本eate MainGa設置eUI"));
        bS使ccess = false;
    }

    本et使本n bS使ccess;
}

bool AMin成GoRTSGa設置eMode::Set使pSyste設置Co設置設置使nication()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 使p Syste設置 Co設置設置使nication..."));
    
    Set使pSyste設置Co設置設置使nicationChannels();
    
    // 這裡可以設置系統間的通信接口
    // 例如：學習系統與AI系統的通信，歷史系統與勢力系統的通信等
    
    本et使本n t本使e;
}

bool AMin成GoRTSGa設置eMode::Set使pE正entBindin成s()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 使p E正ent Bindin成s..."));
    
    // 綁定系統間事件
    if (輸入isto本ySyste設置 && Lea本nin成Syste設置)
    {
        // 歷史事件觸發學習
        輸入isto本ySyste設置->On輸入isto本icalE正entGene本ated.AddDyna設置ic(
            Lea本nin成Syste設置, &UMin成RTSSelfLea本nin成Syste設置::On輸入isto本icalE正entOcc使本本ed);
    }
    
    if (軍actionMana成e本 && 輸入isto本ySyste設置)
    {
        // 勢力變化觸發歷史事件
        軍actionMana成e本->On軍actionRelationshipChan成ed.AddDyna設置ic(
            輸入isto本ySyste設置, &UMin成RTSDyna設置ic輸入isto本ySyste設置::On軍actionDiplo設置acyChan成ed);
    }
    
    本et使本n t本使e;
}

正oid AMin成GoRTSGa設置eMode::Tick(float DeltaTi設置e)
{
    S使pe本::Tick(DeltaTi設置e);
    
    if (bSyste設置sInitialized && bEnablePe本fo本設置anceMonito本in成)
    {
        UpdatePe本fo本設置anceMet本ics(DeltaTi設置e);
    }
}

正oid AMin成GoRTSGa設置eMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    S使pe本::EndPlay(EndPlayReason);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成GoRTS Ga設置eMode Endin成 - Cleanin成 使p syste設置s..."));
    Clean使pAllSyste設置s();
}

bool AMin成GoRTSGa設置eMode::A本eAllSyste設置sInitialized() const
{
    本et使本n bSyste設置sInitialized;
}

軍St本in成 AMin成GoRTSGa設置eMode::GetSyste設置Pe本fo本設置anceRepo本t() const
{
    軍St本in成 Repo本t = TEXT("=== Min成GoRTS Syste設置 Pe本fo本設置ance Repo本t ===\n");
    
    fo本 (const a使to& Met本ic : Syste設置Pe本fo本設置anceMet本ics)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("%s: %.2f 設置s\n"), *Met本ic.Key, Met本ic.Val使e);
    }
    
    本et使本n Repo本t;
}

正oid AMin成GoRTSGa設置eMode::SpawnInitialUnits()
{
    // 生成一些測試單位
    fo本 (int32 i = 0; i < 5; i++)
    {
        軍Vecto本 SpawnLocation = 軍Vecto本(i * 200.0f, 0.0f, 100.0f);
        軍Rotato本 SpawnRotation = 軍Rotato本::Ze本oRotato本;
        
        if (AMin成GoRTSUnit* 的ewUnit = Get基本o本ld()->SpawnActo本<AMin成GoRTSUnit>(AMin成GoRTSUnit::StaticClass(), SpawnLocation, SpawnRotation))
        {
            Ga設置eUnits.Add(的ewUnit);
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Spawned 使nit at location: %s"), *SpawnLocation.ToSt本in成());
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Spawned %d initial 使nits"), Ga設置eUnits.的使設置());
}

正oid AMin成GoRTSGa設置eMode::Clean使pAllSyste設置s()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cleanin成 使p all syste設置s..."));
    
    // 清理所有系統
    if (MainGa設置eUI)
    {
        MainGa設置eUI->Re設置o正e軍本o設置Pa本ent();
        MainGa設置eUI = n使llpt本;
    }
    
    // 其他系統會由UE的垃圾回收機制自動清理
    bSyste設置sInitialized = false;
}

正oid AMin成GoRTSGa設置eMode::UpdatePe本fo本設置anceMet本ics(float DeltaTi設置e)
{
    LastPe本fo本設置anceUpdate += DeltaTi設置e;
    
    if (LastPe本fo本設置anceUpdate >= Pe本fo本設置anceUpdateInte本正al)
    {
        // 更新性能指標
        if (Pe本fo本設置anceSyste設置)
        {
            // 這裡可以從性能系統獲取實時數據
            Syste設置Pe本fo本設置anceMet本ics.Add(TEXT("軍本a設置eTi設置e"), DeltaTi設置e * 1000.0f);
        }
        
        LastPe本fo本設置anceUpdate = 0.0f;
    }
}

正oid AMin成GoRTSGa設置eMode::Set使pSyste設置Co設置設置使nicationChannels()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 使p co設置設置使nication channels between syste設置s..."));
    
    // 實現系統間通信邏輯
}

正oid AMin成GoRTSGa設置eMode::B本oadcastSyste設置E正ent(const 軍St本in成& E正ent的a設置e, UOb大ect* E正entData)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("B本oadcastin成 syste設置 e正ent: %s"), *E正ent的a設置e);
    
    // 實現事件廣播邏輯
}

正oid AMin成GoRTSGa設置eMode::Lo成Syste設置E本本o本(const 軍St本in成& Syste設置的a設置e, const 軍St本in成& E本本o本Messa成e)
{
    軍St本in成 軍使llE本本o本 = 軍St本in成::P本intf(TEXT("[%s] %s"), *Syste設置的a設置e, *E本本o本Messa成e);
    InitializationE本本o本s.Add(軍使llE本本o本);
    UE下LOG(Lo成Te設置p, E本本o本, TEXT("Syste設置 E本本o本: %s"), *軍使llE本本o本);
}

正oid AMin成GoRTSGa設置eMode::輸入andleSyste設置Initialization軍ail使本e(const 軍St本in成& Syste設置的a設置e)
{
    UE下LOG(Lo成Te設置p, E本本o本, TEXT("Syste設置 initialization failed: %s"), *Syste設置的a設置e);
    
    // 實現錯誤處理邏輯，可能包括：
    // 1. 嘗試重新初始化
    // 2. 降級到備用系統
    // 3. 通知用戶
}
