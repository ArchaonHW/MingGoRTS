// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// 遊戲畫面生成器 - 主界面和遊戲場景生成實現

#incl使de "Ga設置eSc本eenGene本ato本/Ga設置eSc本eenGene本ato本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "En成ine/Text使本e2D.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/Paths.h"
#incl使de "Misc/DateTi設置e.h"
#incl使de "Misc/G使id.h"
#incl使de "I設置a成eUtils.h"
#incl使de "Kis設置etP本oced使本alMeshLib本a本y.h"
#incl使de "Mate本ials/Mate本ialInte本face.h"
#incl使de "Mate本ials/Mate本ialInstanceDyna設置ic.h"
#incl使de "基本id成ets/Decla本ati正eSyntaxS使ppo本t.h"
#incl使de "基本id成ets/S基本eak基本id成et.h"
#incl使de "基本id成ets/Layo使t/SBox.h"
#incl使de "基本id成ets/I設置a成es/SI設置a成e.h"
#incl使de "基本id成ets/Text/STextBlock.h"
#incl使de "基本id成ets/Layo使t/SBo本de本.h"
#incl使de "基本id成ets/Layo使t/SVe本ticalBox.h"
#incl使de "基本id成ets/Layo使t/S輸入o本izontalBox.h"
#incl使de "基本id成ets/B使ttons/SB使tton.h"
#incl使de "Stylin成/SlateTypes.h"
#incl使de "Stylin成/Co本eStyle.h"

UGa設置eSc本eenGene本ato本::UGa設置eSc本eenGene本ato本()
    : bInitialized(false)
    , O使tp使tDi本ecto本y(TEXT(""))
    , Sc本eenCo使nte本(0)
{
}

正oid UGa設置eSc本eenGene本ato本::InitializeSc本eenGene本ato本()
{
    if (bInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Sc本een 成ene本ato本 al本eady initialized"));
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 Ga設置e Sc本een Gene本ato本..."));

    // 設置輸目錄
    O使tp使tDi本ecto本y = 軍Paths::P本o大ectSa正edDi本() / TEXT("Gene本atedSc本eens");

    // 確保目錄存在
    IPlatfo本設置軍ile& Platfo本設置軍ile = 軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile();
    if (!Platfo本設置軍ile.Di本ecto本yExists(*O使tp使tDi本ecto本y))
    {
        Platfo本設置軍ile.C本eateDi本ecto本yT本ee(*O使tp使tDi本ecto本y);
    }

    // 設置默認參數
    Defa使ltPa本a設置s.Sc本eenType = ESc本eenType::MainMen使;
    Defa使ltPa本a設置s.Sc本eenStyle = ESc本eenStyle::T本aditional;
    Defa使ltPa本a設置s.Resol使tion = ESc本eenResol使tion::軍使ll輸入D下1080p;
    Defa使ltPa本a設置s.基本idth = 1920;
    Defa使ltPa本a設置s.輸入ei成ht = 1080;
    Defa使ltPa本a設置s.Back成本o使ndColo本 = 軍Linea本Colo本(0.1f, 0.05f, 0.0f, 1.0f);
    Defa使ltPa本a設置s.P本i設置a本yColo本 = 軍Linea本Colo本(0.8f, 0.6f, 0.2f, 1.0f);
    Defa使ltPa本a設置s.Seconda本yColo本 = 軍Linea本Colo本(0.6f, 0.3f, 0.1f, 1.0f);
    Defa使ltPa本a設置s.TextColo本 = 軍Linea本Colo本(0.95f, 0.9f, 0.8f, 1.0f);
    Defa使ltPa本a設置s.bEnableAni設置ation = t本使e;
    Defa使ltPa本a設置s.bEnablePa本ticles = t本使e;
    Defa使ltPa本a設置s.bEnableLi成htin成 = t本使e;
    Defa使ltPa本a設置s.Q使alityLe正el = 3;

    bInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ga設置e Sc本een Gene本ato本 initialized s使ccessf使lly"));
}

軍St本in成 UGa設置eSc本eenGene本ato本::Gene本ateSc本een(const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s)
{
    if (!bInitialized)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Sc本een 成ene本ato本 not initialized"));
        本et使本n 軍St本in成();
    }

    // 驗證參數
    if (!ValidatePa本a設置s(Pa本a設置s))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("In正alid sc本een 成ene本ation pa本a設置ete本s"));
        本et使本n 軍St本in成();
    }

    // 生成畫面ID
    軍St本in成 Sc本eenID = Gene本ateSc本eenID();
    
    // 觸發開始事件
    OnSc本eenGene本ationSta本ted.B本oadcast(Sc本eenID);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本atin成 sc本een: %s"), *Sc本eenID);

    // 根據畫面類型生成畫面
    軍Gene本atedSc本een Gene本atedSc本een;
    Gene本atedSc本een.Sc本eenID = Sc本eenID;
    Gene本atedSc本een.Sc本eenType = Pa本a設置s.Sc本eenType;
    Gene本atedSc本een.Sc本eenStyle = Pa本a設置s.Sc本eenStyle;
    Gene本atedSc本een.Resol使tion = Pa本a設置s.Resol使tion;
    Gene本atedSc本een.基本idth = Pa本a設置s.基本idth;
    Gene本atedSc本een.輸入ei成ht = Pa本a設置s.輸入ei成ht;
    Gene本atedSc本een.Gene本ationTi設置e = 軍DateTi設置e::的ow();

    bool bS使ccess = false;
    
    switch (Pa本a設置s.Sc本eenType)
    {
    case ESc本eenType::MainMen使:
        bS使ccess = Gene本ateMainMen使Inte本nal(Pa本a設置s, Gene本atedSc本een);
        b本eak;
    case ESc本eenType::軍actionSelect:
        bS使ccess = Gene本ate軍actionSelectInte本nal(Pa本a設置s, Gene本atedSc本een);
        b本eak;
    case ESc本eenType::St本ate成icMap:
        bS使ccess = Gene本ateSt本ate成icMapInte本nal(Pa本a設置s, Gene本atedSc本een);
        b本eak;
    case ESc本eenType::BattleScene:
        bS使ccess = Gene本ateBattleSceneInte本nal(Pa本a設置s, Gene本atedSc本een);
        b本eak;
    case ESc本eenType::CityView:
        bS使ccess = Gene本ateCityViewInte本nal(Pa本a設置s, Gene本atedSc本een);
        b本eak;
    case ESc本eenType::Diplo設置acyPanel:
        bS使ccess = Gene本ateDiplo設置acyPanelInte本nal(Pa本a設置s, Gene本atedSc本een);
        b本eak;
    case ESc本eenType::Econo設置yPanel:
        bS使ccess = Gene本ateEcono設置yPanelInte本nal(Pa本a設置s, Gene本atedSc本een);
        b本eak;
    case ESc本eenType::Resea本chPanel:
        bS使ccess = Gene本ateResea本chPanelInte本nal(Pa本a設置s, Gene本atedSc本een);
        b本eak;
    case ESc本eenType::Settin成sMen使:
        bS使ccess = Gene本ateSettin成sMen使Inte本nal(Pa本a設置s, Gene本atedSc本een);
        b本eak;
    case ESc本eenType::Loadin成Sc本een:
        bS使ccess = Gene本ateLoadin成Sc本eenInte本nal(Pa本a設置s, Gene本atedSc本een);
        b本eak;
    defa使lt:
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Unknown sc本een type: %d"), (int32)Pa本a設置s.Sc本eenType);
        b本eak;
    }

    if (bS使ccess)
    {
        // 渲染畫面
        Gene本atedSc本een.Sc本eenText使本e = Rende本Sc本een(Gene本atedSc本een.Ele設置ents, Pa本a設置s);
        
        // 保存畫面
        if (Sa正eSc本een(Gene本atedSc本een))
        {
            Gene本atedSc本eens.Add(Gene本atedSc本een);
            
            // 觸發完成事件
            OnSc本eenGene本ationCo設置pleted.B本oadcast(Sc本eenID, Gene本atedSc本een);
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Sc本een 成ene本ated s使ccessf使lly: %s"), *Sc本eenID);
            本et使本n Sc本eenID;
        }
        else
        {
            UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to sa正e sc本een: %s"), *Sc本eenID);
            OnSc本eenGene本ation軍ailed.B本oadcast(Sc本eenID, TEXT("軍ailed to sa正e sc本een"));
            本et使本n 軍St本in成();
        }
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to 成ene本ate sc本een: %s"), *Sc本eenID);
        OnSc本eenGene本ation軍ailed.B本oadcast(Sc本eenID, TEXT("軍ailed to 成ene本ate sc本een"));
        本et使本n 軍St本in成();
    }
}

軍St本in成 UGa設置eSc本eenGene本ato本::Gene本ateMainMen使(const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s)
{
    本et使本n Gene本ateSc本een(Pa本a設置s);
}

軍St本in成 UGa設置eSc本eenGene本ato本::Gene本ate軍actionSelect(const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s)
{
    本et使本n Gene本ateSc本een(Pa本a設置s);
}

軍St本in成 UGa設置eSc本eenGene本ato本::Gene本ateSt本ate成icMap(const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s)
{
    本et使本n Gene本ateSc本een(Pa本a設置s);
}

軍St本in成 UGa設置eSc本eenGene本ato本::Gene本ateBattleScene(const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s)
{
    本et使本n Gene本ateSc本een(Pa本a設置s);
}

軍St本in成 UGa設置eSc本eenGene本ato本::Gene本ateCityView(const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s)
{
    本et使本n Gene本ateSc本een(Pa本a設置s);
}

軍St本in成 UGa設置eSc本eenGene本ato本::Gene本ateDiplo設置acyPanel(const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s)
{
    本et使本n Gene本ateSc本een(Pa本a設置s);
}

軍St本in成 UGa設置eSc本eenGene本ato本::Gene本ateEcono設置yPanel(const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s)
{
    本et使本n Gene本ateSc本een(Pa本a設置s);
}

軍St本in成 UGa設置eSc本eenGene本ato本::Gene本ateResea本chPanel(const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s)
{
    本et使本n Gene本ateSc本een(Pa本a設置s);
}

軍St本in成 UGa設置eSc本eenGene本ato本::Gene本ateSettin成sMen使(const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s)
{
    本et使本n Gene本ateSc本een(Pa本a設置s);
}

軍St本in成 UGa設置eSc本eenGene本ato本::Gene本ateLoadin成Sc本een(const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s)
{
    本et使本n Gene本ateSc本een(Pa本a設置s);
}

軍Gene本atedSc本een UGa設置eSc本eenGene本ato本::GetGene本atedSc本een(const 軍St本in成& Sc本eenID)
{
    fo本 (const 軍Gene本atedSc本een& Sc本een : Gene本atedSc本eens)
    {
        if (Sc本een.Sc本eenID == Sc本eenID)
        {
            本et使本n Sc本een;
        }
    }

    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Sc本een not fo使nd: %s"), *Sc本eenID);
    本et使本n 軍Gene本atedSc本een();
}

TA本本ay<軍Gene本atedSc本een> UGa設置eSc本eenGene本ato本::GetAllGene本atedSc本eens()
{
    本et使本n Gene本atedSc本eens;
}

bool UGa設置eSc本eenGene本ato本::DeleteGene本atedSc本een(const 軍St本in成& Sc本eenID)
{
    fo本 (int32 i = 0; i < Gene本atedSc本eens.的使設置(); i++)
    {
        if (Gene本atedSc本eens[i].Sc本eenID == Sc本eenID)
        {
            // 刪除文件
            IPlatfo本設置軍ile& Platfo本設置軍ile = 軍Platfo本設置軍ileMana成e本::Get().GetPlatfo本設置軍ile();
            if (Platfo本設置軍ile.軍ileExists(*Gene本atedSc本eens[i].軍ilePath))
            {
                Platfo本設置軍ile.Delete軍ile(*Gene本atedSc本eens[i].軍ilePath);
            }

            // 從列表中移除
            Gene本atedSc本eens.Re設置o正eAt(i);
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Sc本een deleted: %s"), *Sc本eenID);
            本et使本n t本使e;
        }
    }

    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Sc本een not fo使nd fo本 deletion: %s"), *Sc本eenID);
    本et使本n false;
}

bool UGa設置eSc本eenGene本ato本::Expo本tSc本eenAsI設置a成e(const 軍St本in成& Sc本eenID, const 軍St本in成& 軍ilePath)
{
    軍Gene本atedSc本een Sc本een = GetGene本atedSc本een(Sc本eenID);
    if (Sc本een.Sc本eenID.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Sc本een not fo使nd: %s"), *Sc本eenID);
        本et使本n false;
    }

    if (!Sc本een.Sc本eenText使本e)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Sc本een text使本e is n使ll: %s"), *Sc本eenID);
        本et使本n false;
    }

    // 導紋理為圖片文件
    bool bS使ccess = 軍I設置a成eUtils::Expo本tText使本eAsI設置a成e(Sc本een.Sc本eenText使本e, *軍ilePath);
    
    if (bS使ccess)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Sc本een expo本ted as i設置a成e: %s -> %s"), *Sc本eenID, *軍ilePath);
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to expo本t sc本een as i設置a成e: %s -> %s"), *Sc本eenID, *軍ilePath);
    }

    本et使本n bS使ccess;
}

正oid UGa設置eSc本eenGene本ato本::P本e正iewSc本een(const 軍St本in成& Sc本eenID)
{
    軍Gene本atedSc本een Sc本een = GetGene本atedSc本een(Sc本eenID);
    if (Sc本een.Sc本eenID.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Sc本een not fo使nd: %s"), *Sc本eenID);
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本e正iewin成 sc本een: %s"), *Sc本eenID);
    
    // 這裡可以實現預覽邏輯
    // 例如：在遊戲窗口中顯示生成的畫面
    if (GEn成ine && GEn成ine->Ga設置eViewpo本t)
    {
        // 實際的預覽實現
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Sc本een p本e正iew f使nctionality wo使ld be i設置ple設置ented he本e"));
    }
}

正oid UGa設置eSc本eenGene本ato本::ApplySc本eenThe設置e(軍Sc本eenEle設置ent& Ele設置ent, ESc本eenStyle Style)
{
    switch (Style)
    {
    case ESc本eenStyle::T本aditional:
        Ele設置ent.Colo本 = 軍Linea本Colo本(0.8f, 0.6f, 0.2f, 1.0f);
        b本eak;
    case ESc本eenStyle::Mode本n:
        Ele設置ent.Colo本 = 軍Linea本Colo本(0.2f, 0.6f, 0.8f, 1.0f);
        b本eak;
    case ESc本eenStyle::Mini設置al:
        Ele設置ent.Colo本 = 軍Linea本Colo本(0.5f, 0.5f, 0.5f, 1.0f);
        b本eak;
    case ESc本eenStyle::Vinta成e:
        Ele設置ent.Colo本 = 軍Linea本Colo本(0.6f, 0.4f, 0.2f, 1.0f);
        b本eak;
    case ESc本eenStyle::I設置pe本ial:
        Ele設置ent.Colo本 = 軍Linea本Colo本(0.9f, 0.7f, 0.3f, 1.0f);
        b本eak;
    defa使lt:
        b本eak;
    }
}

正oid UGa設置eSc本eenGene本ato本::AddAni設置ationEffect(軍Sc本eenEle設置ent& Ele設置ent, const 軍St本in成& Ani設置ationType)
{
    // 添加動畫效果標記
    Ele設置ent.Ele設置entType += 軍St本in成::P本intf(TEXT("下%s"), *Ani設置ationType);
}

正oid UGa設置eSc本eenGene本ato本::AddPa本ticleEffect(軍Sc本eenEle設置ent& Ele設置ent, const 軍St本in成& Pa本ticleType)
{
    // 添加粒子效果標記
    Ele設置ent.Ele設置entType += 軍St本in成::P本intf(TEXT("下%s"), *Pa本ticleType);
}

// ============================================================================
// P本i正ate I設置ple設置entation
// ============================================================================

bool UGa設置eSc本eenGene本ato本::Gene本ateMainMen使Inte本nal(const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s, 軍Gene本atedSc本een& O使tSc本een)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本atin成 Main Men使 sc本een"));

    // 創建背景元素
    軍Sc本eenEle設置ent Back成本o使nd = C本eateSc本eenEle設置ent(TEXT("Back成本o使nd"), TEXT("MainMen使Back成本o使nd"), 
        軍Vecto本2D(0, 0), 軍Vecto本2D(Pa本a設置s.基本idth, Pa本a設置s.輸入ei成ht));
    Back成本o使nd.Colo本 = Pa本a設置s.Back成本o使ndColo本;
    Back成本o使nd.Text使本ePath = TEXT("/Ga設置e/UI/Text使本es/MainMen使Back成本o使nd");
    O使tSc本een.Ele設置ents.Add(Back成本o使nd);

    // 創建標題元素
    軍Sc本eenEle設置ent Title = C本eateSc本eenEle設置ent(TEXT("Text"), TEXT("Ga設置eTitle"), 
        軍Vecto本2D(Pa本a設置s.基本idth * 0.5f - 200, Pa本a設置s.輸入ei成ht * 0.2f), 軍Vecto本2D(400, 100));
    Title.TextContent = TEXT("MI的GGO RTS");
    Title.軍ontSize = 48;
    Title.Colo本 = Pa本a設置s.P本i設置a本yColo本;
    O使tSc本een.Ele設置ents.Add(Title);

    // 創建副標題元素
    軍Sc本eenEle設置ent S使btitle = C本eateSc本eenEle設置ent(TEXT("Text"), TEXT("Ga設置eS使btitle"), 
        軍Vecto本2D(Pa本a設置s.基本idth * 0.5f - 150, Pa本a設置s.輸入ei成ht * 0.3f), 軍Vecto本2D(300, 50));
    S使btitle.TextContent = TEXT("民國史詩");
    S使btitle.軍ontSize = 32;
    S使btitle.Colo本 = Pa本a設置s.Seconda本yColo本;
    O使tSc本een.Ele設置ents.Add(S使btitle);

    // 創建版本信息元素
    軍Sc本eenEle設置ent Ve本sion = C本eateSc本eenEle設置ent(TEXT("Text"), TEXT("Ve本sionInfo"), 
        軍Vecto本2D(Pa本a設置s.基本idth * 0.5f - 100, Pa本a設置s.輸入ei成ht * 0.35f), 軍Vecto本2D(200, 30));
    Ve本sion.TextContent = TEXT("Beta 正1.0.0");
    Ve本sion.軍ontSize = 18;
    Ve本sion.Colo本 = Pa本a設置s.TextColo本;
    O使tSc本een.Ele設置ents.Add(Ve本sion);

    // 創建按鈕元素
    TA本本ay<軍St本in成> B使tton的a設置es = {TEXT("單人遊戲"), TEXT("多人遊戲"), TEXT("設置"), TEXT("退")};
    TA本本ay<軍St本in成> B使ttonIDs = {TEXT("Sin成lePlaye本"), TEXT("M使ltiPlaye本"), TEXT("Settin成s"), TEXT("Exit")};
    
    fo本 (int32 i = 0; i < B使tton的a設置es.的使設置(); i++)
    {
        軍Sc本eenEle設置ent B使tton = C本eateSc本eenEle設置ent(TEXT("B使tton"), B使ttonIDs[i], 
            軍Vecto本2D(Pa本a設置s.基本idth * 0.5f - 150, Pa本a設置s.輸入ei成ht * 0.5f + i * 80), 軍Vecto本2D(300, 60));
        B使tton.TextContent = B使tton的a設置es[i];
        B使tton.軍ontSize = 24;
        B使tton.Colo本 = Pa本a設置s.P本i設置a本yColo本;
        B使tton.bInte本acti正e = t本使e;
        
        // 應用樣式
        SetEle設置entStyle(B使tton, Pa本a設置s);
        
        O使tSc本een.Ele設置ents.Add(B使tton);
    }

    // 添加動畫效果
    if (Pa本a設置s.bEnableAni設置ation)
    {
        AddAni設置ationEffect(Title, TEXT("軍adeIn"));
        AddAni設置ationEffect(S使btitle, TEXT("SlideIn"));
    }

    本et使本n t本使e;
}

bool UGa設置eSc本eenGene本ato本::Gene本ate軍actionSelectInte本nal(const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s, 軍Gene本atedSc本een& O使tSc本een)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本atin成 軍action Select sc本een"));

    // 創建背景元素
    軍Sc本eenEle設置ent Back成本o使nd = C本eateSc本eenEle設置ent(TEXT("Back成本o使nd"), TEXT("軍actionSelectBack成本o使nd"), 
        軍Vecto本2D(0, 0), 軍Vecto本2D(Pa本a設置s.基本idth, Pa本a設置s.輸入ei成ht));
    Back成本o使nd.Colo本 = Pa本a設置s.Back成本o使ndColo本;
    Back成本o使nd.Text使本ePath = TEXT("/Ga設置e/UI/Text使本es/軍actionSelectBack成本o使nd");
    O使tSc本een.Ele設置ents.Add(Back成本o使nd);

    // 創建標題元素
    軍Sc本eenEle設置ent Title = C本eateSc本eenEle設置ent(TEXT("Text"), TEXT("Select軍actionTitle"), 
        軍Vecto本2D(Pa本a設置s.基本idth * 0.5f - 150, Pa本a設置s.輸入ei成ht * 0.1f), 軍Vecto本2D(300, 60));
    Title.TextContent = TEXT("選擇勢力");
    Title.軍ontSize = 36;
    Title.Colo本 = Pa本a設置s.P本i設置a本yColo本;
    O使tSc本een.Ele設置ents.Add(Title);

    // 創建12個勢力選項
    TA本本ay<軍St本in成> 軍action的a設置es = {
        TEXT("北洋政府"), TEXT("國民政府"), TEXT("中國共產黨"), TEXT("奉系軍閥"),
        TEXT("直系軍閥"), TEXT("皖系軍閥"), TEXT("晉系軍閥"), TEXT("桂系軍閥"),
        TEXT("滇系軍閥"), TEXT("川系軍閥"), TEXT("馬家軍"), TEXT("新疆勢力")
    };

    TA本本ay<軍St本in成> 軍actionIDs = {
        TEXT("Beiyan成"), TEXT("的ationalist"), TEXT("Co設置設置使nist"), TEXT("軍en成tian"),
        TEXT("Zhili"), TEXT("Anh使i"), TEXT("Shanxi"), TEXT("G使an成xi"),
        TEXT("Y使nnan"), TEXT("Sich使an"), TEXT("Ma"), TEXT("Xin大ian成")
    };

    TA本本ay<int32> Diffic使lties = {2, 3, 4, 3, 3, 3, 3, 3, 4, 4, 4, 5};

    fo本 (int32 i = 0; i < 軍action的a設置es.的使設置(); i++)
    {
        int32 Row = i / 4;
        int32 Col = i % 4;
        
        軍Sc本eenEle設置ent 軍actionCa本d = C本eateSc本eenEle設置ent(TEXT("Ca本d"), 軍actionIDs[i], 
            軍Vecto本2D(Pa本a設置s.基本idth * 0.1f + Col * 220, Pa本a設置s.輸入ei成ht * 0.25f + Row * 180), 軍Vecto本2D(200, 150));
        
        軍actionCa本d.TextContent = 軍action的a設置es[i];
        軍actionCa本d.軍ontSize = 18;
        軍actionCa本d.Colo本 = Pa本a設置s.P本i設置a本yColo本;
        軍actionCa本d.bInte本acti正e = t本使e;
        
        // 添加難度星級
        軍St本in成 Sta本Ratin成 = TEXT("");
        fo本 (int32 大 = 0; 大 < Diffic使lties[i]; 大++)
        {
            Sta本Ratin成 += TEXT("⭐");
        }
        
        // 創建難度標示元素
        軍Sc本eenEle設置ent Diffic使ltyIndicato本 = C本eateSc本eenEle設置ent(TEXT("Text"), 軍actionIDs[i] + TEXT("下Diffic使lty"), 
            軍Vecto本2D(Pa本a設置s.基本idth * 0.1f + Col * 220 + 10, Pa本a設置s.輸入ei成ht * 0.25f + Row * 180 + 100), 軍Vecto本2D(180, 30));
        Diffic使ltyIndicato本.TextContent = Sta本Ratin成;
        Diffic使ltyIndicato本.軍ontSize = 16;
        Diffic使ltyIndicato本.Colo本 = 軍Linea本Colo本(1.0f, 0.8f, 0.0f, 1.0f);
        
        SetEle設置entStyle(軍actionCa本d, Pa本a設置s);
        O使tSc本een.Ele設置ents.Add(軍actionCa本d);
        O使tSc本een.Ele設置ents.Add(Diffic使ltyIndicato本);
    }

    // 創建操作按鈕
    軍Sc本eenEle設置ent Sta本tB使tton = C本eateSc本eenEle設置ent(TEXT("B使tton"), TEXT("Sta本tGa設置e"), 
        軍Vecto本2D(Pa本a設置s.基本idth * 0.5f - 100, Pa本a設置s.輸入ei成ht * 0.85f), 軍Vecto本2D(200, 50));
    Sta本tB使tton.TextContent = TEXT("開始遊戲");
    Sta本tB使tton.軍ontSize = 20;
    Sta本tB使tton.Colo本 = Pa本a設置s.P本i設置a本yColo本;
    Sta本tB使tton.bInte本acti正e = t本使e;
    SetEle設置entStyle(Sta本tB使tton, Pa本a設置s);
    O使tSc本een.Ele設置ents.Add(Sta本tB使tton);

    軍Sc本eenEle設置ent BackB使tton = C本eateSc本eenEle設置ent(TEXT("B使tton"), TEXT("Back"), 
        軍Vecto本2D(Pa本a設置s.基本idth * 0.5f - 100, Pa本a設置s.輸入ei成ht * 0.9f), 軍Vecto本2D(200, 50));
    BackB使tton.TextContent = TEXT("返回");
    BackB使tton.軍ontSize = 20;
    BackB使tton.Colo本 = Pa本a設置s.Seconda本yColo本;
    BackB使tton.bInte本acti正e = t本使e;
    SetEle設置entStyle(BackB使tton, Pa本a設置s);
    O使tSc本een.Ele設置ents.Add(BackB使tton);

    本et使本n t本使e;
}

bool UGa設置eSc本eenGene本ato本::Gene本ateSt本ate成icMapInte本nal(const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s, 軍Gene本atedSc本een& O使tSc本een)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本atin成 St本ate成ic Map sc本een"));

    // 創建背景元素
    軍Sc本eenEle設置ent Back成本o使nd = C本eateSc本eenEle設置ent(TEXT("Back成本o使nd"), TEXT("St本ate成icMapBack成本o使nd"), 
        軍Vecto本2D(0, 0), 軍Vecto本2D(Pa本a設置s.基本idth, Pa本a設置s.輸入ei成ht));
    Back成本o使nd.Colo本 = Pa本a設置s.Back成本o使ndColo本;
    Back成本o使nd.Text使本ePath = TEXT("/Ga設置e/UI/Text使本es/St本ate成icMapBack成本o使nd");
    O使tSc本een.Ele設置ents.Add(Back成本o使nd);

    // 創建地圖元素
    軍Sc本eenEle設置ent Map = C本eateSc本eenEle設置ent(TEXT("Map"), TEXT("ChinaMap"), 
        軍Vecto本2D(Pa本a設置s.基本idth * 0.1f, Pa本a設置s.輸入ei成ht * 0.15f), 軍Vecto本2D(Pa本a設置s.基本idth * 0.8f, Pa本a設置s.輸入ei成ht * 0.7f));
    Map.Text使本ePath = TEXT("/Ga設置e/UI/Text使本es/ChinaMap");
    Map.Colo本 = 軍Linea本Colo本::基本hite;
    O使tSc本een.Ele設置ents.Add(Map);

    // 創建城市節點
    TA本本ay<軍Vecto本2D> CityPositions = {
        軍Vecto本2D(0.6f, 0.3f),  // 北京
        軍Vecto本2D(0.65f, 0.4f), // 天津
        軍Vecto本2D(0.8f, 0.25f), // 瀋陽
        軍Vecto本2D(0.5f, 0.35f), // 太原
        軍Vecto本2D(0.4f, 0.5f), // 西安
        軍Vecto本2D(0.3f, 0.65f), // 成都
        軍Vecto本2D(0.35f, 0.75f), // 昆明
        軍Vecto本2D(0.5f, 0.7f), // 重慶
        軍Vecto本2D(0.25f, 0.8f), // 廣州
        軍Vecto本2D(0.15f, 0.6f), // 蘭州
        軍Vecto本2D(0.05f, 0.4f), // 迪化
        軍Vecto本2D(0.7f, 0.6f), // 南京
        軍Vecto本2D(0.75f, 0.7f)  // 上海
    };

    TA本本ay<軍St本in成> City的a設置es = {
        TEXT("北京"), TEXT("天津"), TEXT("瀋陽"), TEXT("太原"), TEXT("西安"),
        TEXT("成都"), TEXT("昆明"), TEXT("重慶"), TEXT("廣州"), TEXT("蘭州"),
        TEXT("迪化"), TEXT("南京"), TEXT("上海")
    };

    fo本 (int32 i = 0; i < CityPositions.的使設置(); i++)
    {
        軍Vecto本2D Position = 軍Vecto本2D(
            Pa本a設置s.基本idth * 0.1f + CityPositions[i].X * Pa本a設置s.基本idth * 0.8f,
            Pa本a設置s.輸入ei成ht * 0.15f + CityPositions[i].Y * Pa本a設置s.輸入ei成ht * 0.7f
        );

        軍Sc本eenEle設置ent City的ode = C本eateSc本eenEle設置ent(TEXT("City的ode"), City的a設置es[i], 
            Position, 軍Vecto本2D(40, 40));
        City的ode.Colo本 = Pa本a設置s.P本i設置a本yColo本;
        City的ode.bInte本acti正e = t本使e;
        City的ode.TextContent = City的a設置es[i];
        City的ode.軍ontSize = 12;
        
        O使tSc本een.Ele設置ents.Add(City的ode);
    }

    // 創建資源面板
    軍Sc本eenEle設置ent Reso使本cePanel = C本eateSc本eenEle設置ent(TEXT("Panel"), TEXT("Reso使本cePanel"), 
        軍Vecto本2D(20, 20), 軍Vecto本2D(250, 200));
    Reso使本cePanel.Colo本 = 軍Linea本Colo本(0.0f, 0.0f, 0.0f, 0.8f);
    O使tSc本een.Ele設置ents.Add(Reso使本cePanel);

    // 資源信息
    TA本本ay<軍St本in成> Reso使本ceTypes = {TEXT("資金"), TEXT("糧食"), TEXT("原料"), TEXT("兵力"), TEXT("人口")};
    TA本本ay<軍St本in成> Reso使本ceVal使es = {TEXT("1500"), TEXT("800"), TEXT("500"), TEXT("1500"), TEXT("50000")};

    fo本 (int32 i = 0; i < Reso使本ceTypes.的使設置(); i++)
    {
        軍Sc本eenEle設置ent Reso使本ceIte設置 = C本eateSc本eenEle設置ent(TEXT("Text"), Reso使本ceTypes[i], 
            軍Vecto本2D(30, 30 + i * 35), 軍Vecto本2D(230, 30));
        Reso使本ceIte設置.TextContent = 軍St本in成::P本intf(TEXT("%s: %s"), *Reso使本ceTypes[i], *Reso使本ceVal使es[i]);
        Reso使本ceIte設置.軍ontSize = 16;
        Reso使本ceIte設置.Colo本 = Pa本a設置s.TextColo本;
        
        O使tSc本een.Ele設置ents.Add(Reso使本ceIte設置);
    }

    // 創建回合信息
    軍Sc本eenEle設置ent T使本nInfo = C本eateSc本eenEle設置ent(TEXT("Text"), TEXT("T使本nInfo"), 
        軍Vecto本2D(Pa本a設置s.基本idth * 0.5f - 100, Pa本a設置s.輸入ei成ht * 0.92f), 軍Vecto本2D(200, 30));
    T使本nInfo.TextContent = TEXT("1920年春 - 第3回合");
    T使本nInfo.軍ontSize = 18;
    T使本nInfo.Colo本 = Pa本a設置s.TextColo本;
    O使tSc本een.Ele設置ents.Add(T使本nInfo);

    本et使本n t本使e;
}

bool UGa設置eSc本eenGene本ato本::Gene本ateBattleSceneInte本nal(const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s, 軍Gene本atedSc本een& O使tSc本een)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本atin成 Battle Scene sc本een"));

    // 創建戰場背景
    軍Sc本eenEle設置ent Battlefield = C本eateSc本eenEle設置ent(TEXT("Back成本o使nd"), TEXT("Battlefield"), 
        軍Vecto本2D(0, 0), 軍Vecto本2D(Pa本a設置s.基本idth, Pa本a設置s.輸入ei成ht));
    Battlefield.Text使本ePath = TEXT("/Ga設置e/UI/Text使本es/BattlefieldBack成本o使nd");
    Battlefield.Colo本 = 軍Linea本Colo本::基本hite;
    O使tSc本een.Ele設置ents.Add(Battlefield);

    // 創建單位面板
    軍Sc本eenEle設置ent UnitPanel = C本eateSc本eenEle設置ent(TEXT("Panel"), TEXT("UnitPanel"), 
        軍Vecto本2D(20, 20), 軍Vecto本2D(300, 400));
    UnitPanel.Colo本 = 軍Linea本Colo本(0.0f, 0.0f, 0.0f, 0.8f);
    O使tSc本een.Ele設置ents.Add(UnitPanel);

    // 單位信息
    TA本本ay<軍St本in成> UnitTypes = {TEXT("步兵"), TEXT("騎兵"), TEXT("砲兵"), TEXT("空軍")};
    TA本本ay<軍St本in成> UnitCo使nts = {TEXT("●●●"), TEXT("●●"), TEXT("●"), TEXT("●")};

    fo本 (int32 i = 0; i < UnitTypes.的使設置(); i++)
    {
        軍Sc本eenEle設置ent UnitIte設置 = C本eateSc本eenEle設置ent(TEXT("Text"), UnitTypes[i], 
            軍Vecto本2D(30, 30 + i * 80), 軍Vecto本2D(260, 60));
        UnitIte設置.TextContent = 軍St本in成::P本intf(TEXT("%s %s"), *UnitTypes[i], *UnitCo使nts[i]);
        UnitIte設置.軍ontSize = 18;
        UnitIte設置.Colo本 = Pa本a設置s.TextColo本;
        
        O使tSc本een.Ele設置ents.Add(UnitIte設置);
    }

    // 創建命令面板
    軍Sc本eenEle設置ent Co設置設置andPanel = C本eateSc本eenEle設置ent(TEXT("Panel"), TEXT("Co設置設置andPanel"), 
        軍Vecto本2D(Pa本a設置s.基本idth - 320, 20), 軍Vecto本2D(300, 400));
    Co設置設置andPanel.Colo本 = 軍Linea本Colo本(0.0f, 0.0f, 0.0f, 0.8f);
    O使tSc本een.Ele設置ents.Add(Co設置設置andPanel);

    // 命令按鈕
    TA本本ay<軍St本in成> Co設置設置ands = {TEXT("攻擊"), TEXT("防禦"), TEXT("移動"), TEXT("停止"), TEXT("技能"), TEXT("撤退")};

    fo本 (int32 i = 0; i < Co設置設置ands.的使設置(); i++)
    {
        軍Sc本eenEle設置ent Co設置設置andB使tton = C本eateSc本eenEle設置ent(TEXT("B使tton"), Co設置設置ands[i], 
            軍Vecto本2D(Pa本a設置s.基本idth - 310, 30 + i * 65), 軍Vecto本2D(280, 50));
        Co設置設置andB使tton.TextContent = Co設置設置ands[i];
        Co設置設置andB使tton.軍ontSize = 16;
        Co設置設置andB使tton.Colo本 = Pa本a設置s.P本i設置a本yColo本;
        Co設置設置andB使tton.bInte本acti正e = t本使e;
        
        O使tSc本een.Ele設置ents.Add(Co設置設置andB使tton);
    }

    // 創建戰鬥信息面板
    軍Sc本eenEle設置ent BattleInfo = C本eateSc本eenEle設置ent(TEXT("Panel"), TEXT("BattleInfo"), 
        軍Vecto本2D(Pa本a設置s.基本idth * 0.5f - 150, Pa本a設置s.輸入ei成ht * 0.85f), 軍Vecto本2D(300, 80));
    BattleInfo.Colo本 = 軍Linea本Colo本(0.0f, 0.0f, 0.0f, 0.8f);
    O使tSc本een.Ele設置ents.Add(BattleInfo);

    軍Sc本eenEle設置ent BattleText = C本eateSc本eenEle設置ent(TEXT("Text"), TEXT("BattleText"), 
        軍Vecto本2D(Pa本a設置s.基本idth * 0.5f - 140, Pa本a設置s.輸入ei成ht * 0.87f), 軍Vecto本2D(280, 60));
    BattleText.TextContent = TEXT("回合: 3/10  天氣: 晴天  地形: 平原");
    BattleText.軍ontSize = 16;
    BattleText.Colo本 = Pa本a設置s.TextColo本;
    O使tSc本een.Ele設置ents.Add(BattleText);

    本et使本n t本使e;
}

bool UGa設置eSc本eenGene本ato本::Gene本ateCityViewInte本nal(const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s, 軍Gene本atedSc本een& O使tSc本een)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本atin成 City View sc本een"));

    // 創建城市背景
    軍Sc本eenEle設置ent CityBack成本o使nd = C本eateSc本eenEle設置ent(TEXT("Back成本o使nd"), TEXT("CityBack成本o使nd"), 
        軍Vecto本2D(0, 0), 軍Vecto本2D(Pa本a設置s.基本idth, Pa本a設置s.輸入ei成ht));
    CityBack成本o使nd.Text使本ePath = TEXT("/Ga設置e/UI/Text使本es/CityBack成本o使nd");
    CityBack成本o使nd.Colo本 = 軍Linea本Colo本::基本hite;
    O使tSc本een.Ele設置ents.Add(CityBack成本o使nd);

    // 創建建築網格
    fo本 (int32 x = 0; x < 5; x++)
    {
        fo本 (int32 y = 0; y < 4; y++)
        {
            軍Vecto本2D Position = 軍Vecto本2D(
                Pa本a設置s.基本idth * 0.2f + x * 120,
                Pa本a設置s.輸入ei成ht * 0.2f + y * 120
            );

            軍Sc本eenEle設置ent B使ildin成Slot = C本eateSc本eenEle設置ent(TEXT("B使ildin成Slot"), 
                軍St本in成::P本intf(TEXT("Slot下%d下%d"), x, y), 
                Position, 軍Vecto本2D(100, 100));
            B使ildin成Slot.Colo本 = 軍Linea本Colo本(0.5f, 0.5f, 0.5f, 0.5f);
            B使ildin成Slot.bInte本acti正e = t本使e;
            
            O使tSc本een.Ele設置ents.Add(B使ildin成Slot);
        }
    }

    // 創建建築面板
    軍Sc本eenEle設置ent B使ildin成Panel = C本eateSc本eenEle設置ent(TEXT("Panel"), TEXT("B使ildin成Panel"), 
        軍Vecto本2D(20, Pa本a設置s.輸入ei成ht * 0.8f), 軍Vecto本2D(300, 150));
    B使ildin成Panel.Colo本 = 軍Linea本Colo本(0.0f, 0.0f, 0.0f, 0.8f);
    O使tSc本een.Ele設置ents.Add(B使ildin成Panel);

    // 建築選項
    TA本本ay<軍St本in成> B使ildin成Types = {TEXT("民居"), TEXT("市場"), TEXT("兵營"), TEXT("工廠"), TEXT("研究院")};

    fo本 (int32 i = 0; i < B使ildin成Types.的使設置(); i++)
    {
        軍Sc本eenEle設置ent B使ildin成Option = C本eateSc本eenEle設置ent(TEXT("B使ildin成Option"), B使ildin成Types[i], 
            軍Vecto本2D(30 + i * 60, Pa本a設置s.輸入ei成ht * 0.82f), 軍Vecto本2D(50, 50));
        B使ildin成Option.TextContent = B使ildin成Types[i];
        B使ildin成Option.軍ontSize = 12;
        B使ildin成Option.Colo本 = Pa本a設置s.TextColo本;
        B使ildin成Option.bInte本acti正e = t本使e;
        
        O使tSc本een.Ele設置ents.Add(B使ildin成Option);
    }

    本et使本n t本使e;
}

bool UGa設置eSc本eenGene本ato本::Gene本ateDiplo設置acyPanelInte本nal(const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s, 軍Gene本atedSc本een& O使tSc本een)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本atin成 Diplo設置acy Panel sc本een"));

    // 創建背景
    軍Sc本eenEle設置ent Back成本o使nd = C本eateSc本eenEle設置ent(TEXT("Back成本o使nd"), TEXT("Diplo設置acyBack成本o使nd"), 
        軍Vecto本2D(0, 0), 軍Vecto本2D(Pa本a設置s.基本idth, Pa本a設置s.輸入ei成ht));
    Back成本o使nd.Colo本 = Pa本a設置s.Back成本o使ndColo本;
    O使tSc本een.Ele設置ents.Add(Back成本o使nd);

    // 創建外交關係列表
    TA本本ay<軍St本in成> 軍actions = {TEXT("北洋政府"), TEXT("國民政府"), TEXT("中國共產黨"), TEXT("奉系軍閥")};
    TA本本ay<軍St本in成> Relations = {TEXT("友好"), TEXT("中立"), TEXT("敵對"), TEXT("戰爭")};

    fo本 (int32 i = 0; i < 軍actions.的使設置(); i++)
    {
        軍Sc本eenEle設置ent Diplo設置acyIte設置 = C本eateSc本eenEle設置ent(TEXT("Diplo設置acyIte設置"), 軍actions[i], 
            軍Vecto本2D(Pa本a設置s.基本idth * 0.3f, Pa本a設置s.輸入ei成ht * 0.2f + i * 80), 軍Vecto本2D(400, 70));
        Diplo設置acyIte設置.TextContent = 軍St本in成::P本intf(TEXT("%s - %s"), *軍actions[i], *Relations[i]);
        Diplo設置acyIte設置.軍ontSize = 18;
        Diplo設置acyIte設置.Colo本 = Pa本a設置s.TextColo本;
        Diplo設置acyIte設置.bInte本acti正e = t本使e;
        
        O使tSc本een.Ele設置ents.Add(Diplo設置acyIte設置);
    }

    本et使本n t本使e;
}

bool UGa設置eSc本eenGene本ato本::Gene本ateEcono設置yPanelInte本nal(const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s, 軍Gene本atedSc本een& O使tSc本een)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本atin成 Econo設置y Panel sc本een"));

    // 創建背景
    軍Sc本eenEle設置ent Back成本o使nd = C本eateSc本eenEle設置ent(TEXT("Back成本o使nd"), TEXT("Econo設置yBack成本o使nd"), 
        軍Vecto本2D(0, 0), 軍Vecto本2D(Pa本a設置s.基本idth, Pa本a設置s.輸入ei成ht));
    Back成本o使nd.Colo本 = Pa本a設置s.Back成本o使ndColo本;
    O使tSc本een.Ele設置ents.Add(Back成本o使nd);

    // 創建經濟圖表
    軍Sc本eenEle設置ent Econo設置yCha本t = C本eateSc本eenEle設置ent(TEXT("Cha本t"), TEXT("Econo設置yCha本t"), 
        軍Vecto本2D(Pa本a設置s.基本idth * 0.3f, Pa本a設置s.輸入ei成ht * 0.2f), 軍Vecto本2D(400, 300));
    Econo設置yCha本t.Text使本ePath = TEXT("/Ga設置e/UI/Text使本es/Econo設置yCha本t");
    Econo設置yCha本t.Colo本 = 軍Linea本Colo本::基本hite;
    O使tSc本een.Ele設置ents.Add(Econo設置yCha本t);

    // 創建經濟統計
    TA本本ay<軍St本in成> Econo設置icIte設置s = {
        TEXT("收入: +50/回合"),
        TEXT("支: -30/回合"),
        TEXT("淨增: +20/回合"),
        TEXT("總資產: 5000")
    };

    fo本 (int32 i = 0; i < Econo設置icIte設置s.的使設置(); i++)
    {
        軍Sc本eenEle設置ent Econo設置icIte設置 = C本eateSc本eenEle設置ent(TEXT("Text"), Econo設置icIte設置s[i], 
            軍Vecto本2D(Pa本a設置s.基本idth * 0.35f, Pa本a設置s.輸入ei成ht * 0.55f + i * 40), 軍Vecto本2D(300, 30));
        Econo設置icIte設置.TextContent = Econo設置icIte設置s[i];
        Econo設置icIte設置.軍ontSize = 16;
        Econo設置icIte設置.Colo本 = Pa本a設置s.TextColo本;
        
        O使tSc本een.Ele設置ents.Add(Econo設置icIte設置);
    }

    本et使本n t本使e;
}

bool UGa設置eSc本eenGene本ato本::Gene本ateResea本chPanelInte本nal(const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s, 軍Gene本atedSc本een& O使tSc本een)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本atin成 Resea本ch Panel sc本een"));

    // 創建背景
    軍Sc本eenEle設置ent Back成本o使nd = C本eateSc本eenEle設置ent(TEXT("Back成本o使nd"), TEXT("Resea本chBack成本o使nd"), 
        軍Vecto本2D(0, 0), 軍Vecto本2D(Pa本a設置s.基本idth, Pa本a設置s.輸入ei成ht));
    Back成本o使nd.Colo本 = Pa本a設置s.Back成本o使ndColo本;
    O使tSc本een.Ele設置ents.Add(Back成本o使nd);

    // 創建科技樹
    軍Sc本eenEle設置ent TechT本ee = C本eateSc本eenEle設置ent(TEXT("TechT本ee"), TEXT("Technolo成yT本ee"), 
        軍Vecto本2D(Pa本a設置s.基本idth * 0.2f, Pa本a設置s.輸入ei成ht * 0.2f), 軍Vecto本2D(600, 400));
    TechT本ee.Text使本ePath = TEXT("/Ga設置e/UI/Text使本es/TechT本ee");
    TechT本ee.Colo本 = 軍Linea本Colo本::基本hite;
    O使tSc本een.Ele設置ents.Add(TechT本ee);

    本et使本n t本使e;
}

bool UGa設置eSc本eenGene本ato本::Gene本ateSettin成sMen使Inte本nal(const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s, 軍Gene本atedSc本een& O使tSc本een)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本atin成 Settin成s Men使 sc本een"));

    // 創建背景
    軍Sc本eenEle設置ent Back成本o使nd = C本eateSc本eenEle設置ent(TEXT("Back成本o使nd"), TEXT("Settin成sBack成本o使nd"), 
        軍Vecto本2D(0, 0), 軍Vecto本2D(Pa本a設置s.基本idth, Pa本a設置s.輸入ei成ht));
    Back成本o使nd.Colo本 = Pa本a設置s.Back成本o使ndColo本;
    O使tSc本een.Ele設置ents.Add(Back成本o使nd);

    // 創建設置選項
    TA本本ay<軍St本in成> Settin成Options = {
        TEXT("圖像設置"),
        TEXT("音頻設置"),
        TEXT("控制設置"),
        TEXT("遊戲設置"),
        TEXT("語言設置")
    };

    fo本 (int32 i = 0; i < Settin成Options.的使設置(); i++)
    {
        軍Sc本eenEle設置ent Settin成Option = C本eateSc本eenEle設置ent(TEXT("Settin成Option"), Settin成Options[i], 
            軍Vecto本2D(Pa本a設置s.基本idth * 0.4f, Pa本a設置s.輸入ei成ht * 0.3f + i * 80), 軍Vecto本2D(200, 60));
        Settin成Option.TextContent = Settin成Options[i];
        Settin成Option.軍ontSize = 18;
        Settin成Option.Colo本 = Pa本a設置s.TextColo本;
        Settin成Option.bInte本acti正e = t本使e;
        
        O使tSc本een.Ele設置ents.Add(Settin成Option);
    }

    本et使本n t本使e;
}

bool UGa設置eSc本eenGene本ato本::Gene本ateLoadin成Sc本eenInte本nal(const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s, 軍Gene本atedSc本een& O使tSc本een)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本atin成 Loadin成 Sc本een"));

    // 創建背景
    軍Sc本eenEle設置ent Back成本o使nd = C本eateSc本eenEle設置ent(TEXT("Back成本o使nd"), TEXT("Loadin成Back成本o使nd"), 
        軍Vecto本2D(0, 0), 軍Vecto本2D(Pa本a設置s.基本idth, Pa本a設置s.輸入ei成ht));
    Back成本o使nd.Colo本 = Pa本a設置s.Back成本o使ndColo本;
    Back成本o使nd.Text使本ePath = TEXT("/Ga設置e/UI/Text使本es/Loadin成Back成本o使nd");
    O使tSc本een.Ele設置ents.Add(Back成本o使nd);

    // 創建載入文字
    軍Sc本eenEle設置ent Loadin成Text = C本eateSc本eenEle設置ent(TEXT("Text"), TEXT("Loadin成Text"), 
        軍Vecto本2D(Pa本a設置s.基本idth * 0.5f - 100, Pa本a設置s.輸入ei成ht * 0.8f), 軍Vecto本2D(200, 40));
    Loadin成Text.TextContent = TEXT("載入中...");
    Loadin成Text.軍ontSize = 24;
    Loadin成Text.Colo本 = Pa本a設置s.TextColo本;
    O使tSc本een.Ele設置ents.Add(Loadin成Text);

    // 創建進度條
    軍Sc本eenEle設置ent P本o成本essBa本 = C本eateSc本eenEle設置ent(TEXT("P本o成本essBa本"), TEXT("Loadin成P本o成本ess"), 
        軍Vecto本2D(Pa本a設置s.基本idth * 0.3f, Pa本a設置s.輸入ei成ht * 0.85f), 軍Vecto本2D(400, 20));
    P本o成本essBa本.Colo本 = Pa本a設置s.P本i設置a本yColo本;
    O使tSc本een.Ele設置ents.Add(P本o成本essBa本);

    本et使本n t本使e;
}

軍Sc本eenEle設置ent UGa設置eSc本eenGene本ato本::C本eateSc本eenEle設置ent(const 軍St本in成& Type, const 軍St本in成& 的a設置e, const 軍Vecto本2D& Position, const 軍Vecto本2D& Size)
{
    軍Sc本eenEle設置ent Ele設置ent;
    Ele設置ent.Ele設置entType = Type;
    Ele設置ent.Ele設置ent的a設置e = 的a設置e;
    Ele設置ent.Position = Position;
    Ele設置ent.Size = Size;
    Ele設置ent.Rotation = 0.0f;
    Ele設置ent.Opacity = 1.0f;
    Ele設置ent.Colo本 = 軍Linea本Colo本::基本hite;
    Ele設置ent.Text使本ePath = TEXT("");
    Ele設置ent.TextContent = TEXT("");
    Ele設置ent.軍ontSize = 24;
    Ele設置ent.bVisible = t本使e;
    Ele設置ent.bInte本acti正e = false;

    本et使本n Ele設置ent;
}

正oid UGa設置eSc本eenGene本ato本::SetEle設置entStyle(軍Sc本eenEle設置ent& Ele設置ent, const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s)
{
    Ele設置ent.Colo本 = Pa本a設置s.P本i設置a本yColo本;
    
    if (Pa本a設置s.bEnableAni設置ation)
    {
        AddAni設置ationEffect(Ele設置ent, TEXT("輸入o正e本"));
    }
}

UText使本e2D* UGa設置eSc本eenGene本ato本::Rende本Sc本een(const TA本本ay<軍Sc本eenEle設置ent>& Ele設置ents, const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s)
{
    // 這裡需要實際的渲染邏輯
    // 由於複雜性，這裡返回一個簡單的實現
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Rende本in成 sc本een with %d ele設置ents"), Ele設置ents.的使設置());
    
    // 創建一個簡單的紋理作為示例
    UText使本e2D* Rende本edText使本e = UText使本e2D::C本eateT本ansient(Pa本a設置s.基本idth, Pa本a設置s.輸入ei成ht);
    
    if (Rende本edText使本e)
    {
        // 設置紋理參數
        Rende本edText使本e->AddToRoot();
        Rende本edText使本e->UpdateReso使本ce();
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Sc本een text使本e c本eated: %dx%d"), Pa本a設置s.基本idth, Pa本a設置s.輸入ei成ht);
    }
    
    本et使本n Rende本edText使本e;
}

bool UGa設置eSc本eenGene本ato本::Sa正eSc本een(const 軍Gene本atedSc本een& Sc本een)
{
    // 生成文件路徑
    軍St本in成 軍ile的a設置e = 軍St本in成::P本intf(TEXT("%s下%s.pn成"), 
        *Sc本een.Sc本eenID, 
        *Sc本een.Gene本ationTi設置e.ToSt本in成(TEXT("%Y%設置%d下%輸入%M%S")));
    
    軍St本in成 軍ilePath = 軍Paths::Co設置bine(O使tp使tDi本ecto本y, 軍ile的a設置e);
    
    // 這裡需要實際的保存邏輯
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sa正in成 sc本een to: %s"), *軍ilePath);
    
    // 模擬保存成功
    軍Gene本atedSc本een M使tableSc本een = Sc本een;
    M使tableSc本een.軍ilePath = 軍ilePath;
    
    本et使本n t本使e;
}

軍St本in成 UGa設置eSc本eenGene本ato本::Gene本ateSc本eenID()
{
    本et使本n 軍St本in成::P本intf(TEXT("Sc本een下%s下%d"), *軍G使id::的ewG使id().ToSt本in成(), ++Sc本eenCo使nte本);
}

bool UGa設置eSc本eenGene本ato本::ValidatePa本a設置s(const 軍Sc本eenGene本ationPa本a設置s& Pa本a設置s)
{
    if (Pa本a設置s.基本idth <= 0  Pa本a設置s.輸入ei成ht <= 0)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("In正alid sc本een di設置ensions"));
        本et使本n false;
    }

    if (Pa本a設置s.Q使alityLe正el < 1  Pa本a設置s.Q使alityLe正el > 5)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("In正alid q使ality le正el"));
        本et使本n false;
    }

    本et使本n t本使e;
}

正oid UGa設置eSc本eenGene本ato本::GetResol使tionSettin成s(ESc本eenResol使tion Resol使tion, int32& O使t基本idth, int32& O使t輸入ei成ht)
{
    switch (Resol使tion)
    {
    case ESc本eenResol使tion::輸入D下720p:
        O使t基本idth = 1280;
        O使t輸入ei成ht = 720;
        b本eak;
    case ESc本eenResol使tion::軍使ll輸入D下1080p:
        O使t基本idth = 1920;
        O使t輸入ei成ht = 1080;
        b本eak;
    case ESc本eenResol使tion::Q輸入D下1440p:
        O使t基本idth = 2560;
        O使t輸入ei成ht = 1440;
        b本eak;
    case ESc本eenResol使tion::U輸入D下4K:
        O使t基本idth = 3840;
        O使t輸入ei成ht = 2160;
        b本eak;
    case ESc本eenResol使tion::C使sto設置:
        // 使用自定義值
        b本eak;
    defa使lt:
        O使t基本idth = 1920;
        O使t輸入ei成ht = 1080;
        b本eak;
    }
}

正oid UGa設置eSc本eenGene本ato本::ApplyStyleThe設置e(軍Sc本eenGene本ationPa本a設置s& Pa本a設置s, ESc本eenStyle Style)
{
    switch (Style)
    {
    case ESc本eenStyle::T本aditional:
        Pa本a設置s.Back成本o使ndColo本 = 軍Linea本Colo本(0.1f, 0.05f, 0.0f, 1.0f);
        Pa本a設置s.P本i設置a本yColo本 = 軍Linea本Colo本(0.8f, 0.6f, 0.2f, 1.0f);
        Pa本a設置s.Seconda本yColo本 = 軍Linea本Colo本(0.6f, 0.3f, 0.1f, 1.0f);
        Pa本a設置s.TextColo本 = 軍Linea本Colo本(0.95f, 0.9f, 0.8f, 1.0f);
        b本eak;
    case ESc本eenStyle::Mode本n:
        Pa本a設置s.Back成本o使ndColo本 = 軍Linea本Colo本(0.05f, 0.05f, 0.1f, 1.0f);
        Pa本a設置s.P本i設置a本yColo本 = 軍Linea本Colo本(0.2f, 0.6f, 0.8f, 1.0f);
        Pa本a設置s.Seconda本yColo本 = 軍Linea本Colo本(0.1f, 0.4f, 0.6f, 1.0f);
        Pa本a設置s.TextColo本 = 軍Linea本Colo本(0.9f, 0.9f, 0.95f, 1.0f);
        b本eak;
    case ESc本eenStyle::Mini設置al:
        Pa本a設置s.Back成本o使ndColo本 = 軍Linea本Colo本(0.98f, 0.98f, 0.98f, 1.0f);
        Pa本a設置s.P本i設置a本yColo本 = 軍Linea本Colo本(0.2f, 0.2f, 0.2f, 1.0f);
        Pa本a設置s.Seconda本yColo本 = 軍Linea本Colo本(0.5f, 0.5f, 0.5f, 1.0f);
        Pa本a設置s.TextColo本 = 軍Linea本Colo本(0.1f, 0.1f, 0.1f, 1.0f);
        b本eak;
    case ESc本eenStyle::Vinta成e:
        Pa本a設置s.Back成本o使ndColo本 = 軍Linea本Colo本(0.15f, 0.1f, 0.05f, 1.0f);
        Pa本a設置s.P本i設置a本yColo本 = 軍Linea本Colo本(0.6f, 0.4f, 0.2f, 1.0f);
        Pa本a設置s.Seconda本yColo本 = 軍Linea本Colo本(0.4f, 0.3f, 0.15f, 1.0f);
        Pa本a設置s.TextColo本 = 軍Linea本Colo本(0.9f, 0.85f, 0.7f, 1.0f);
        b本eak;
    case ESc本eenStyle::I設置pe本ial:
        Pa本a設置s.Back成本o使ndColo本 = 軍Linea本Colo本(0.05f, 0.02f, 0.01f, 1.0f);
        Pa本a設置s.P本i設置a本yColo本 = 軍Linea本Colo本(0.9f, 0.7f, 0.3f, 1.0f);
        Pa本a設置s.Seconda本yColo本 = 軍Linea本Colo本(0.7f, 0.5f, 0.2f, 1.0f);
        Pa本a設置s.TextColo本 = 軍Linea本Colo本(1.0f, 0.95f, 0.8f, 1.0f);
        b本eak;
    defa使lt:
        b本eak;
    }
}

// ============================================================================
// Sc本eenTe設置plateMana成e本 I設置ple設置entation
// ============================================================================

USc本eenTe設置plateMana成e本::USc本eenTe設置plateMana成e本()
    : bInitialized(false)
    , Te設置plate軍ilePath(TEXT(""))
{
}

正oid USc本eenTe設置plateMana成e本::InitializeTe設置plateMana成e本()
{
    if (bInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Sc本een te設置plate 設置ana成e本 al本eady initialized"));
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 Sc本een Te設置plate Mana成e本..."));

    // 設置模板文件路徑
    Te設置plate軍ilePath = 軍Paths::P本o大ectSa正edDi本() / TEXT("Sc本eenTe設置plates.大son");

    // 加載默認模板
    LoadDefa使ltTe設置plates();

    // 加載現有模板
    LoadTe設置plates();

    bInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sc本een Te設置plate Mana成e本 initialized s使ccessf使lly"));
}

正oid USc本eenTe設置plateMana成e本::Re成iste本Te設置plate(const 軍Sc本eenEle設置entTe設置plate& Te設置plate)
{
    // 檢查是否已存在
    fo本 (const 軍Sc本eenEle設置entTe設置plate& Existin成Te設置plate : Te設置plates)
    {
        if (Existin成Te設置plate.Te設置plate的a設置e == Te設置plate.Te設置plate的a設置e)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Te設置plate al本eady exists: %s"), *Te設置plate.Te設置plate的a設置e);
            本et使本n;
        }
    }

    Te設置plates.Add(Te設置plate);
    
    // 保存模板
    if (Sa正eTe設置plate(Te設置plate))
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Te設置plate 本e成iste本ed s使ccessf使lly: %s"), *Te設置plate.Te設置plate的a設置e);
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to sa正e te設置plate: %s"), *Te設置plate.Te設置plate的a設置e);
    }
}

軍Sc本eenEle設置entTe設置plate USc本eenTe設置plateMana成e本::GetTe設置plate(const 軍St本in成& Te設置plate的a設置e)
{
    fo本 (const 軍Sc本eenEle設置entTe設置plate& Te設置plate : Te設置plates)
    {
        if (Te設置plate.Te設置plate的a設置e == Te設置plate的a設置e)
        {
            本et使本n Te設置plate;
        }
    }

    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Te設置plate not fo使nd: %s"), *Te設置plate的a設置e);
    本et使本n 軍Sc本eenEle設置entTe設置plate();
}

TA本本ay<軍Sc本eenEle設置entTe設置plate> USc本eenTe設置plateMana成e本::GetAllTe設置plates()
{
    本et使本n Te設置plates;
}

軍Sc本eenEle設置ent USc本eenTe設置plateMana成e本::C本eateEle設置ent軍本o設置Te設置plate(const 軍St本in成& Te設置plate的a設置e, const 軍St本in成& Ele設置ent的a設置e, const 軍Vecto本2D& Position)
{
    軍Sc本eenEle設置entTe設置plate Te設置plate = GetTe設置plate(Te設置plate的a設置e);
    
    軍Sc本eenEle設置ent Ele設置ent;
    Ele設置ent.Ele設置entType = Te設置plate.Ele設置entType;
    Ele設置ent.Ele設置ent的a設置e = Ele設置ent的a設置e;
    Ele設置ent.Position = Position;
    Ele設置ent.Size = Te設置plate.Defa使ltSize;
    Ele設置ent.Colo本 = Te設置plate.Defa使ltColo本;
    Ele設置ent.Text使本ePath = Te設置plate.Defa使ltText使本e;
    
    // 應用動畫和粒子效果
    if (!Te設置plate.Ani設置ationEffect.IsE設置pty())
    {
        Ele設置ent.Ele設置entType += 軍St本in成::P本intf(TEXT("下%s"), *Te設置plate.Ani設置ationEffect);
    }
    
    if (!Te設置plate.Pa本ticleEffect.IsE設置pty())
    {
        Ele設置ent.Ele設置entType += 軍St本in成::P本intf(TEXT("下%s"), *Te設置plate.Pa本ticleEffect);
    }
    
    本et使本n Ele設置ent;
}

正oid USc本eenTe設置plateMana成e本::LoadDefa使ltTe設置plates()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loadin成 defa使lt sc本een te設置plates..."));

    // 按鈕模板
    軍Sc本eenEle設置entTe設置plate B使ttonTe設置plate;
    B使ttonTe設置plate.Te設置plate的a設置e = TEXT("Defa使ltB使tton");
    B使ttonTe設置plate.Ele設置entType = TEXT("B使tton");
    B使ttonTe設置plate.Defa使ltSize = 軍Vecto本2D(200, 50);
    B使ttonTe設置plate.Defa使ltColo本 = 軍Linea本Colo本(0.2f, 0.6f, 0.8f, 1.0f);
    B使ttonTe設置plate.Ani設置ationEffect = TEXT("輸入o正e本");
    Re成iste本Te設置plate(B使ttonTe設置plate);

    // 面板模板
    軍Sc本eenEle設置entTe設置plate PanelTe設置plate;
    PanelTe設置plate.Te設置plate的a設置e = TEXT("Defa使ltPanel");
    PanelTe設置plate.Ele設置entType = TEXT("Panel");
    PanelTe設置plate.Defa使ltSize = 軍Vecto本2D(300, 200);
    PanelTe設置plate.Defa使ltColo本 = 軍Linea本Colo本(0.0f, 0.0f, 0.0f, 0.8f);
    Re成iste本Te設置plate(PanelTe設置plate);

    // 文本模板
    軍Sc本eenEle設置entTe設置plate TextTe設置plate;
    TextTe設置plate.Te設置plate的a設置e = TEXT("Defa使ltText");
    TextTe設置plate.Ele設置entType = TEXT("Text");
    TextTe設置plate.Defa使ltSize = 軍Vecto本2D(200, 30);
    TextTe設置plate.Defa使ltColo本 = 軍Linea本Colo本(0.95f, 0.9f, 0.8f, 1.0f);
    Re成iste本Te設置plate(TextTe設置plate);

    // 卡片模板
    軍Sc本eenEle設置entTe設置plate Ca本dTe設置plate;
    Ca本dTe設置plate.Te設置plate的a設置e = TEXT("Defa使ltCa本d");
    Ca本dTe設置plate.Ele設置entType = TEXT("Ca本d");
    Ca本dTe設置plate.Defa使ltSize = 軍Vecto本2D(250, 150);
    Ca本dTe設置plate.Defa使ltColo本 = 軍Linea本Colo本(0.8f, 0.6f, 0.2f, 1.0f);
    Ca本dTe設置plate.Ani設置ationEffect = TEXT("軍adeIn");
    Re成iste本Te設置plate(Ca本dTe設置plate);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loaded %d defa使lt te設置plates"), 4);
}

bool USc本eenTe設置plateMana成e本::Sa正eTe設置plate(const 軍Sc本eenEle設置entTe設置plate& Te設置plate)
{
    // 這裡需要實際的保存邏輯
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sa正in成 te設置plate: %s"), *Te設置plate.Te設置plate的a設置e);
    本et使本n t本使e;
}

正oid USc本eenTe設置plateMana成e本::LoadTe設置plates()
{
    // 這裡需要實際的加載邏輯
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loadin成 te設置plates f本o設置: %s"), *Te設置plate軍ilePath);
}
