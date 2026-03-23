#incl使de "Min成RTSOpti設置izationInte成本ation.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Ga設置e軍本a設置ewo本k/Ga設置eModeBase.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

UMin成RTSOpti設置izationInte成本ation::UMin成RTSOpti設置izationInte成本ation()
{
    bSyste設置Inte成本ated = false;
    bOpti設置izationActi正e = false;
}

正oid UMin成RTSOpti設置izationInte成本ation::Inte成本ateOpti設置izationSyste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: Inte成本atin成 opti設置ization syste設置 into Min成GoRTS"));

    // 創建優化系統管理器
    Opti設置izationMana成e本 = 的ewOb大ect<UMin成RTSOpti設置izationSyste設置Mana成e本>();
    Opti設置izationMana成e本->InitializeOpti設置izationSyste設置();

    // 設置遊戲特定優化
    Set使pGa設置eSpecificOpti設置izations();

    // 配置優化參數
    Confi成使本eGa設置eOpti設置izationPa本a設置ete本s();

    // 集成到遊戲系統
    Inte成本ate基本ithGa設置eSyste設置s();

    // 設置事件處理
    Set使pE正ent輸入andle本s();

    bSyste設置Inte成本ated = t本使e;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: Opti設置ization syste設置 inte成本ated s使ccessf使lly"));
}

正oid UMin成RTSOpti設置izationInte成本ation::Sta本tGa設置eOpti設置ization()
{
    if (!bSyste設置Inte成本ated)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成RTSOpti設置izationInte成本ation: Syste設置 not inte成本ated"));
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: Sta本tin成 成a設置e opti設置ization"));

    // 設置遊戲開始時的優化配置
    軍Opti設置izationConfi成使本ation Ga設置eConfi成;
    Ga設置eConfi成.Mode = EOpti設置izationMode::De正elop設置ent;
    Ga設置eConfi成.bEnablePa本allelCo設置pilation = t本使e;
    Ga設置eConfi成.bEnablePe本fo本設置anceP本ofilin成 = t本使e;
    Ga設置eConfi成.bEnableAl成o本ith設置Opti設置ization = t本使e;
    Ga設置eConfi成.MaxCo設置pilationTh本eads = 軍Platfo本設置Misc::的使設置be本OfCo本es() / 2;
    Ga設置eConfi成.Pe本fo本設置anceMonito本in成Inte本正al = 1.0f;

    // 添加遊戲特定的算法優化
    Ga設置eConfi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::Ti設置eCo設置plexity);
    Ga設置eConfi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::CacheOpti設置ization);
    Ga設置eConfi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::Pa本allelization);

    Opti設置izationMana成e本->SetOpti設置izationConfi成使本ation(Ga設置eConfi成);

    // 開始實時監控
    Opti設置izationMana成e本->Sta本tRealTi設置eMonito本in成();

    // 優化遊戲循環
    Opti設置izeGa設置eLoop();

    bOpti設置izationActi正e = t本使e;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: Ga設置e opti設置ization sta本ted"));
}

正oid UMin成RTSOpti設置izationInte成本ation::R使nti設置eOpti設置ization()
{
    if (!bOpti設置izationActi正e)
    {
        本et使本n;
    }

    // 持續監控遊戲性能
    Monito本Ga設置ePe本fo本設置ance();

    // 自動優化基於性能
    A使toOpti設置izeBasedOnPe本fo本設置ance();

    // 優化各個遊戲系統
    Opti設置izeRende本in成();
    Opti設置izeAI();
    Opti設置izeA使dio();
    Opti設置izePhysics();
}

正oid UMin成RTSOpti設置izationInte成本ation::Le正elT本ansitionOpti設置ization()
{
    if (!bOpti設置izationActi正e)
    {
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: Opti設置izin成 fo本 le正el t本ansition"));

    // 關卡切換時的優化配置
    軍Opti設置izationConfi成使本ation Le正elConfi成;
    Le正elConfi成.Mode = EOpti設置izationMode::Testin成;
    Le正elConfi成.bEnablePa本allelCo設置pilation = t本使e;
    Le正elConfi成.bEnablePe本fo本設置anceP本ofilin成 = t本使e;
    Le正elConfi成.MaxCo設置pilationTh本eads = 軍Platfo本設置Misc::的使設置be本OfCo本es();

    Opti設置izationMana成e本->SetOpti設置izationConfi成使本ation(Le正elConfi成);

    // 預加載優化
    Opti設置izeP本eloadin成();

    // 內存清理優化
    Opti設置izeMe設置o本yClean使p();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: Le正el t本ansition opti設置ization co設置pleted"));
}

正oid UMin成RTSOpti設置izationInte成本ation::Sa正eGa設置eOpti設置ization()
{
    if (!bOpti設置izationActi正e)
    {
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: Opti設置izin成 fo本 sa正e 成a設置e"));

    // 保存時的優化配置
    軍Opti設置izationConfi成使本ation Sa正eConfi成;
    Sa正eConfi成.Mode = EOpti設置izationMode::De正elop設置ent;
    Sa正eConfi成.bEnablePe本fo本設置anceP本ofilin成 = false; // 保存時不需要性能分析
    Sa正eConfi成.bEnableAl成o本ith設置Opti設置ization = false;

    Opti設置izationMana成e本->SetOpti設置izationConfi成使本ation(Sa正eConfi成);

    // 優化保存數據
    Opti設置izeSa正eData();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: Sa正e 成a設置e opti設置ization co設置pleted"));
}

正oid UMin成RTSOpti設置izationInte成本ation::LoadGa設置eOpti設置ization()
{
    if (!bOpti設置izationActi正e)
    {
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: Opti設置izin成 fo本 load 成a設置e"));

    // 加載時的優化配置
    軍Opti設置izationConfi成使本ation LoadConfi成;
    LoadConfi成.Mode = EOpti設置izationMode::De正elop設置ent;
    LoadConfi成.bEnablePe本fo本設置anceP本ofilin成 = t本使e;
    LoadConfi成.bEnableAl成o本ith設置Opti設置ization = false;

    Opti設置izationMana成e本->SetOpti設置izationConfi成使本ation(LoadConfi成);

    // 優化加載過程
    Opti設置izeLoadin成P本ocess();

    // 重新啟動實時監控
    Opti設置izationMana成e本->Sta本tRealTi設置eMonito本in成();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: Load 成a設置e opti設置ization co設置pleted"));
}

bool UMin成RTSOpti設置izationInte成本ation::IsOpti設置izationSyste設置Acti正e() const
{
    本et使本n bOpti設置izationActi正e && bSyste設置Inte成本ated && Opti設置izationMana成e本.IsValid();
}

軍St本in成 UMin成RTSOpti設置izationInte成本ation::GetGa設置ePe本fo本設置anceStats()
{
    if (!IsOpti設置izationSyste設置Acti正e())
    {
        本et使本n TEXT("Opti設置ization syste設置 not acti正e");
    }

    // 獲取性能概覽
    TMap<EP本ofilin成Type, EPe本fo本設置anceLe正el> Pe本fo本設置anceO正e本正iew = Opti設置izationMana成e本->GetPe本fo本設置anceO正e本正iew();

    // 獲取優化報告
    軍St本in成 Repo本t = Opti設置izationMana成e本->GetOpti設置izationRepo本t();

    // 生成遊戲特定統計
    軍St本in成 Ga設置eStats = TEXT("=== Min成GoRTS 性能統計 ===\n");
    Ga設置eStats += 軍St本in成::P本intf(TEXT("系統狀態: %s\n"), IsOpti設置izationSyste設置Acti正e() 基本 TEXT("活躍") : TEXT("非活躍"));
    Ga設置eStats += 軍St本in成::P本intf(TEXT("性能指標: %d個\n"), Pe本fo本設置anceO正e本正iew.的使設置());

    fo本 (const a使to& Pai本 : Pe本fo本設置anceO正e本正iew)
    {
        Ga設置eStats += 軍St本in成::P本intf(TEXT("%s: %s\n"), 
            *GetP本ofilin成Type的a設置e(Pai本.Key), 
            *GetPe本fo本設置anceLe正el的a設置e(Pai本.Val使e));
    }

    Ga設置eStats += TEXT("\n");
    Ga設置eStats += Repo本t;

    本et使本n Ga設置eStats;
}

// 私有方法實現

正oid UMin成RTSOpti設置izationInte成本ation::Set使pGa設置eSpecificOpti設置izations()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: Settin成 使p 成a設置e-specific opti設置izations"));

    // 設置Min成GoRTS特定的優化規則
    // 這裡可以添加遊戲特定的優化配置
}

正oid UMin成RTSOpti設置izationInte成本ation::Confi成使本eGa設置eOpti設置izationPa本a設置ete本s()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: Confi成使本in成 成a設置e opti設置ization pa本a設置ete本s"));

    // 配置遊戲特定的優化參數
    if (Opti設置izationMana成e本.IsValid())
    {
        // 設置默認配置
        軍Opti設置izationConfi成使本ation Defa使ltConfi成;
        Defa使ltConfi成.Mode = EOpti設置izationMode::De正elop設置ent;
        Defa使ltConfi成.bEnablePa本allelCo設置pilation = t本使e;
        Defa使ltConfi成.bEnablePe本fo本設置anceP本ofilin成 = t本使e;
        Defa使ltConfi成.bEnableAl成o本ith設置Opti設置ization = t本使e;
        Defa使ltConfi成.MaxCo設置pilationTh本eads = 軍Platfo本設置Misc::的使設置be本OfCo本es();
        Defa使ltConfi成.Pe本fo本設置anceMonito本in成Inte本正al = 1.0f;

        Opti設置izationMana成e本->SetOpti設置izationConfi成使本ation(Defa使ltConfi成);
    }
}

正oid UMin成RTSOpti設置izationInte成本ation::Inte成本ate基本ithGa設置eSyste設置s()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: Inte成本atin成 with 成a設置e syste設置s"));

    // 與遊戲系統集成
    // 這裡可以添加與其他遊戲系統的集成邏輯
}

正oid UMin成RTSOpti設置izationInte成本ation::Set使pE正ent輸入andle本s()
{
    if (!Opti設置izationMana成e本.IsValid())
    {
        本et使本n;
    }

    // 設置性能警告處理
    Opti設置izationMana成e本->OnPe本fo本設置ance基本a本nin成.AddDyna設置ic(this, &UMin成RTSOpti設置izationInte成本ation::輸入andlePe本fo本設置ance基本a本nin成s);

    // 設置優化完成處理
    Opti設置izationMana成e本->OnOpti設置izationCo設置pleted.AddDyna設置ic(this, &UMin成RTSOpti設置izationInte成本ation::OnOpti設置izationCo設置pleted);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: E正ent handle本s set使p co設置pleted"));
}

正oid UMin成RTSOpti設置izationInte成本ation::Opti設置izeGa設置eLoop()
{
    // 優化遊戲主循環
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: Opti設置izin成 成a設置e loop"));

    // 這裡可以添加遊戲循環特定的優化邏輯
}

正oid UMin成RTSOpti設置izationInte成本ation::Opti設置izeRende本in成()
{
    // 優化渲染系統
    // 這裡可以添加渲染特定的優化邏輯
}

正oid UMin成RTSOpti設置izationInte成本ation::Opti設置izeAI()
{
    // 優化AI系統
    // 這裡可以添加AI特定的優化邏輯
}

正oid UMin成RTSOpti設置izationInte成本ation::Opti設置izeA使dio()
{
    // 優化音頻系統
    // 這裡可以添加音頻特定的優化邏輯
}

正oid UMin成RTSOpti設置izationInte成本ation::Opti設置izePhysics()
{
    // 優化物理系統
    // 這裡可以添加物理特定的優化邏輯
}

正oid UMin成RTSOpti設置izationInte成本ation::Monito本Ga設置ePe本fo本設置ance()
{
    // 監控遊戲性能
    // 這裡可以添加性能監控邏輯
}

正oid UMin成RTSOpti設置izationInte成本ation::輸入andlePe本fo本設置ance基本a本nin成s(const 軍St本in成& Co設置ponent, const 軍St本in成& 基本a本nin成, float Se正e本ity)
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成RTSOpti設置izationInte成本ation: Pe本fo本設置ance wa本nin成 - %s: %s (Se正e本ity: %.2f)"), 
           *Co設置ponent, *基本a本nin成, Se正e本ity);

    // 處理性能警告
    if (Se正e本ity > 0.8f)
    {
        // 高嚴重度警告，立即處理
        輸入andleC本iticalPe本fo本設置anceIss使e(Co設置ponent, 基本a本nin成);
    }
    else if (Se正e本ity > 0.5f)
    {
        // 中等嚴重度警告，記錄並監控
        輸入andleMedi使設置Pe本fo本設置anceIss使e(Co設置ponent, 基本a本nin成);
    }
}

正oid UMin成RTSOpti設置izationInte成本ation::A使toOpti設置izeBasedOnPe本fo本設置ance()
{
    // 基於性能自動優化
    if (Opti設置izationMana成e本.IsValid())
    {
        TMap<EP本ofilin成Type, EPe本fo本設置anceLe正el> Pe本fo本設置anceO正e本正iew = Opti設置izationMana成e本->GetPe本fo本設置anceO正e本正iew();

        // 檢查是否需要調整優化級別
        bool b的eedUp成本ade = false;
        fo本 (const a使to& Pai本 : Pe本fo本設置anceO正e本正iew)
        {
            if (Pai本.Val使e <= EPe本fo本設置anceLe正el::Poo本)
            {
                b的eedUp成本ade = t本使e;
                b本eak;
            }
        }

        if (b的eedUp成本ade)
        {
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: A使to-使p成本adin成 opti設置ization le正el"));
            Up成本adeOpti設置izationLe正el();
        }
    }
}

正oid UMin成RTSOpti設置izationInte成本ation::Ad大使stQ使alitySettin成s()
{
    // 調整質量設置
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: Ad大使stin成 q使ality settin成s"));

    // 這裡可以添加質量調整邏輯
}

正oid UMin成RTSOpti設置izationInte成本ation::Opti設置izeP本eloadin成()
{
    // 優化預加載
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: Opti設置izin成 p本eloadin成"));
}

正oid UMin成RTSOpti設置izationInte成本ation::Opti設置izeMe設置o本yClean使p()
{
    // 優化內存清理
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: Opti設置izin成 設置e設置o本y clean使p"));
}

正oid UMin成RTSOpti設置izationInte成本ation::Opti設置izeSa正eData()
{
    // 優化保存數據
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: Opti設置izin成 sa正e data"));
}

正oid UMin成RTSOpti設置izationInte成本ation::Opti設置izeLoadin成P本ocess()
{
    // 優化加載過程
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: Opti設置izin成 loadin成 p本ocess"));
}

正oid UMin成RTSOpti設置izationInte成本ation::輸入andleC本iticalPe本fo本設置anceIss使e(const 軍St本in成& Co設置ponent, const 軍St本in成& 基本a本nin成)
{
    UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成RTSOpti設置izationInte成本ation: C本itical pe本fo本設置ance iss使e in %s: %s"), *Co設置ponent, *基本a本nin成);

    // 立即採取措施
    E設置e本成encyOpti設置ization();
}

正oid UMin成RTSOpti設置izationInte成本ation::輸入andleMedi使設置Pe本fo本設置anceIss使e(const 軍St本in成& Co設置ponent, const 軍St本in成& 基本a本nin成)
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成RTSOpti設置izationInte成本ation: Medi使設置 pe本fo本設置ance iss使e in %s: %s"), *Co設置ponent, *基本a本nin成);

    // 記錄並計劃優化
    Sched使leOpti設置ization(Co設置ponent, 基本a本nin成);
}

正oid UMin成RTSOpti設置izationInte成本ation::Up成本adeOpti設置izationLe正el()
{
    if (!Opti設置izationMana成e本.IsValid())
    {
        本et使本n;
    }

    // 升級優化級別
    軍Opti設置izationConfi成使本ation C使本本entConfi成 = Opti設置izationMana成e本->GetC使本本entConfi成使本ation();
    
    // 根據當前模式升級
    switch (C使本本entConfi成.Mode)
    {
        case EOpti設置izationMode::De正elop設置ent:
            C使本本entConfi成.Mode = EOpti設置izationMode::Testin成;
            b本eak;
        case EOpti設置izationMode::Testin成:
            C使本本entConfi成.Mode = EOpti設置izationMode::Release;
            b本eak;
        case EOpti設置izationMode::Release:
            C使本本entConfi成.Mode = EOpti設置izationMode::Pe本fo本設置ance;
            b本eak;
        case EOpti設置izationMode::Pe本fo本設置ance:
            // 已經是最高級別，添加更多優化技術
            C使本本entConfi成.Al成o本ith設置Opti設置izationTechniq使es.AddUniq使e(EOpti設置izationTechniq使e::輸入yb本id);
            b本eak;
    }

    Opti設置izationMana成e本->SetOpti設置izationConfi成使本ation(C使本本entConfi成);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: Up成本aded to opti設置ization 設置ode %d"), (int32)C使本本entConfi成.Mode);
}

正oid UMin成RTSOpti設置izationInte成本ation::E設置e本成encyOpti設置ization()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: E設置e本成ency opti設置ization acti正ated"));

    // 緊急優化措施
    if (Opti設置izationMana成e本.IsValid())
    {
        // 設置最高優化級別
        軍Opti設置izationConfi成使本ation E設置e本成encyConfi成;
        E設置e本成encyConfi成.Mode = EOpti設置izationMode::Pe本fo本設置ance;
        E設置e本成encyConfi成.bEnablePa本allelCo設置pilation = t本使e;
        E設置e本成encyConfi成.bEnablePe本fo本設置anceP本ofilin成 = t本使e;
        E設置e本成encyConfi成.bEnableAl成o本ith設置Opti設置ization = t本使e;
        E設置e本成encyConfi成.MaxCo設置pilationTh本eads = 軍Platfo本設置Misc::的使設置be本OfCo本es();
        E設置e本成encyConfi成.Pe本fo本設置anceMonito本in成Inte本正al = 0.1f;

        // 添加所有優化技術
        E設置e本成encyConfi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::Ti設置eCo設置plexity);
        E設置e本成encyConfi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::SpaceCo設置plexity);
        E設置e本成encyConfi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::CacheOpti設置ization);
        E設置e本成encyConfi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::Pa本allelization);
        E設置e本成encyConfi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::Me設置oization);
        E設置e本成encyConfi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::App本oxi設置ation);
        E設置e本成encyConfi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::輸入e使本istic);
        E設置e本成encyConfi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::輸入yb本id);

        Opti設置izationMana成e本->SetOpti設置izationConfi成使本ation(E設置e本成encyConfi成);
        Opti設置izationMana成e本->DeepOpti設置ize();
    }
}

正oid UMin成RTSOpti設置izationInte成本ation::Sched使leOpti設置ization(const 軍St本in成& Co設置ponent, const 軍St本in成& 基本a本nin成)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: Sched使led opti設置ization fo本 %s"), *Co設置ponent);

    // 計劃優化
    // 這裡可以添加計劃優化邏輯
}

正oid UMin成RTSOpti設置izationInte成本ation::OnOpti設置izationCo設置pleted(const 軍St本in成& P本o大ect的a設置e, const 軍Opti設置izationConfi成使本ation& Confi成)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSOpti設置izationInte成本ation: Opti設置ization co設置pleted fo本 %s"), *P本o大ect的a設置e);

    // 優化完成後的處理
    // 這裡可以添加優化完成後的邏輯
}

// 輔助方法

軍St本in成 UMin成RTSOpti設置izationInte成本ation::GetP本ofilin成Type的a設置e(EP本ofilin成Type P本ofilin成Type) const
{
    switch (P本ofilin成Type)
    {
        case EP本ofilin成Type::CPU: 本et使本n TEXT("CPU");
        case EP本ofilin成Type::Me設置o本y: 本et使本n TEXT("Me設置o本y");
        case EP本ofilin成Type::GPU: 本et使本n TEXT("GPU");
        case EP本ofilin成Type::的etwo本k: 本et使本n TEXT("的etwo本k");
        case EP本ofilin成Type::Disk: 本et使本n TEXT("Disk");
        case EP本ofilin成Type::Rende本in成: 本et使本n TEXT("Rende本in成");
        case EP本ofilin成Type::A使dio: 本et使本n TEXT("A使dio");
        case EP本ofilin成Type::Physics: 本et使本n TEXT("Physics");
        defa使lt: 本et使本n TEXT("Unknown");
    }
}

軍St本in成 UMin成RTSOpti設置izationInte成本ation::GetPe本fo本設置anceLe正el的a設置e(EPe本fo本設置anceLe正el Le正el) const
{
    switch (Le正el)
    {
        case EPe本fo本設置anceLe正el::Excellent: 本et使本n TEXT("Excellent");
        case EPe本fo本設置anceLe正el::Good: 本et使本n TEXT("Good");
        case EPe本fo本設置anceLe正el::A正e本a成e: 本et使本n TEXT("A正e本a成e");
        case EPe本fo本設置anceLe正el::Poo本: 本et使本n TEXT("Poo本");
        case EPe本fo本設置anceLe正el::C本itical: 本et使本n TEXT("C本itical");
        defa使lt: 本et使本n TEXT("Unknown");
    }
}
