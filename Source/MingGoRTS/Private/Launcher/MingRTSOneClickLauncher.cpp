#incl使de "Min成RTSOneClickLa使nche本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "輸入AL/Platfo本設置Misc.h"
#incl使de "Misc/Paths.h"
#incl使de "Kis設置et/Kis設置etSyste設置Lib本a本y.h"

UMin成RTSOneClickLa使nche本::UMin成RTSOneClickLa使nche本()
{
    bSyste設置La使nched = false;
    bOpti設置izationActi正e = false;
    La使nchTi設置e = 軍DateTi設置e::的ow();
}

UMin成RTSOneClickLa使nche本* UMin成RTSOneClickLa使nche本::La使nchOpti設置izationSyste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("🚀 Min成GoRTS 一鍵啟動優化編譯系統"));
    
    // 創建啟動器實例
    UMin成RTSOneClickLa使nche本* La使nche本 = 的ewOb大ect<UMin成RTSOneClickLa使nche本>();
    
    // 初始化並快速啟動
    La使nche本->InitializeLa使nche本();
    La使nche本->Q使ickLa使nchAndOpti設置ize();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("✅ 一鍵啟動完成！系統已準備就緒"));
    
    本et使本n La使nche本;
}

正oid UMin成RTSOneClickLa使nche本::Q使ickLa使nchAndOpti設置ize()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("⚡ 快速啟動並優化..."));
    
    if (bSyste設置La使nched)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("系統已經啟動"));
        本et使本n;
    }

    // 初始化啟動器
    InitializeLa使nche本();

    // 配置開發模式（平衡性能和速度）
    Confi成使本e軍o本Mode(EOpti設置izationMode::De正elop設置ent);

    // 開始優化
    Sta本tOpti設置ization();

    // 顯示啟動成功信息
    ShowLa使nchS使ccess();

    bSyste設置La使nched = t本使e;
    La使nchTi設置e = 軍DateTi設置e::的ow();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("🎯 快速啟動完成！系統正在運行"));
}

正oid UMin成RTSOneClickLa使nche本::S設置a本tLa使nch()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("🎯 智能啟動..."));
    
    if (bSyste設置La使nched)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("系統已經啟動"));
        本et使本n;
    }

    // 初始化啟動器
    InitializeLa使nche本();

    // 根據系統資源智能選擇模式
    EOpti設置izationMode S設置a本tMode = Dete本設置ineBestMode();
    Confi成使本e軍o本Mode(S設置a本tMode);

    // 開始優化
    Sta本tOpti設置ization();

    // 顯示系統信息
    DisplaySyste設置Info();

    bSyste設置La使nched = t本使e;
    La使nchTi設置e = 軍DateTi設置e::的ow();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("🧠 智能啟動完成！使用模式: %s"), *GetLa使nchMode的a設置e(S設置a本tMode));
}

正oid UMin成RTSOneClickLa使nche本::Ext本e設置eLa使nch()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("🔥 極致啟動..."));
    
    if (bSyste設置La使nched)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("系統已經啟動，切換到極致模式"));
    }

    // 初始化啟動器
    InitializeLa使nche本();

    // 配置極致性能模式
    Confi成使本e軍o本Mode(EOpti設置izationMode::Pe本fo本設置ance);

    // 開始優化
    Sta本tOpti設置ization();

    bSyste設置La使nched = t本使e;
    La使nchTi設置e = 軍DateTi設置e::的ow();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("🔥 極致啟動完成！系統以最高性能運行"));
}

軍St本in成 UMin成RTSOneClickLa使nche本::GetLa使nchStat使s()
{
    軍St本in成 Stat使s = TEXT("🚀 Min成GoRTS 一鍵啟動系統狀態\n\n");
    
    Stat使s += 軍St本in成::P本intf(TEXT("系統狀態: %s\n"), bSyste設置La使nched 基本 TEXT("🟢 已啟動") : TEXT("🔴 未啟動"));
    Stat使s += 軍St本in成::P本intf(TEXT("優化狀態: %s\n"), bOpti設置izationActi正e 基本 TEXT("🔄 運行中") : TEXT("⏸️ 空閒"));
    
    if (bSyste設置La使nched)
    {
        軍Ti設置espan ElapsedTi設置e = 軍DateTi設置e::的ow() - La使nchTi設置e;
        Stat使s += 軍St本in成::P本intf(TEXT("運行時間: %s\n"), *ElapsedTi設置e.ToSt本in成());
        
        if (Opti設置izationMana成e本.IsValid())
        {
            Stat使s += 軍St本in成::P本intf(TEXT("系統就緒: %s\n"), Opti設置izationMana成e本->IsSyste設置Ready() 基本 TEXT("✅ 就緒") : TEXT("❌ 未就緒"));
            
            // 獲取性能概覽
            TMap<EP本ofilin成Type, EPe本fo本設置anceLe正el> Pe本fo本設置anceO正e本正iew = Opti設置izationMana成e本->GetPe本fo本設置anceO正e本正iew();
            Stat使s += 軍St本in成::P本intf(TEXT("監控指標: %d個\n"), Pe本fo本設置anceO正e本正iew.的使設置());
            
            Stat使s += TEXT("\n📊 實時性能:\n");
            fo本 (const a使to& Pai本 : Pe本fo本設置anceO正e本正iew)
            {
                軍St本in成 Type的a設置e = GetP本ofilin成Type的a設置e(Pai本.Key);
                軍St本in成 Le正el的a設置e = GetPe本fo本設置anceLe正el的a設置e(Pai本.Val使e);
                軍St本in成 E設置o大i = GetPe本fo本設置anceE設置o大i(Pai本.Val使e);
                Stat使s += 軍St本in成::P本intf(TEXT("  %s %s: %s\n"), *E設置o大i, *Type的a設置e, *Le正el的a設置e);
            }
        }
    }
    
    // 系統健康度
    float 輸入ealth = GetSyste設置輸入ealth();
    Stat使s += 軍St本in成::P本intf(TEXT("\n💚 系統健康度: %.1f%% %s\n"), 輸入ealth * 100.0f, *GetSyste設置輸入ealthStat使s(輸入ealth));
    
    本et使本n Stat使s;
}

軍St本in成 UMin成RTSOneClickLa使nche本::GetPe本fo本設置anceRepo本t()
{
    if (!bSyste設置La使nched  !Opti設置izationMana成e本.IsValid())
    {
        本et使本n TEXT("❌ 請先啟動系統");
    }

    軍St本in成 Repo本t = TEXT("🎯 Min成GoRTS 性能報告\n\n");

    // 獲取優化報告
    軍St本in成 Opti設置izationRepo本t = Opti設置izationMana成e本->GetOpti設置izationRepo本t();
    Repo本t += Opti設置izationRepo本t;

    // 添加啟動器特定信息
    Repo本t += 軍St本in成::P本intf(TEXT("\n🚀 啟動器信息:\n"));
    Repo本t += 軍St本in成::P本intf(TEXT("  啟動時間: %s\n"), *La使nchTi設置e.ToSt本in成());
    
    軍Ti設置espan ElapsedTi設置e = 軍DateTi設置e::的ow() - La使nchTi設置e;
    Repo本t += 軍St本in成::P本intf(TEXT("  運行時長: %s\n"), *ElapsedTi設置e.ToSt本in成());
    Repo本t += 軍St本in成::P本intf(TEXT("  系統健康: %.1f%%\n"), GetSyste設置輸入ealth() * 100.0f);

    本et使本n Repo本t;
}

正oid UMin成RTSOneClickLa使nche本::Resta本tSyste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("🔄 重新啟動系統..."));
    
    // 停止當前系統
    StopSyste設置();
    
    // 等待一秒
    軍Platfo本設置P本ocess::Sleep(1.0f);
    
    // 重新啟動
    Q使ickLa使nchAndOpti設置ize();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("✅ 系統重新啟動完成"));
}

正oid UMin成RTSOneClickLa使nche本::StopSyste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("⏹️ 停止優化系統..."));
    
    if (Opti設置izationMana成e本.IsValid())
    {
        Opti設置izationMana成e本->StopRealTi設置eMonito本in成();
    }
    
    bOpti設置izationActi正e = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("✅ 系統已停止"));
}

float UMin成RTSOneClickLa使nche本::GetSyste設置輸入ealth()
{
    if (!bSyste設置La使nched  !Opti設置izationMana成e本.IsValid())
    {
        本et使本n 0.0f;
    }

    // 獲取性能概覽
    TMap<EP本ofilin成Type, EPe本fo本設置anceLe正el> Pe本fo本設置anceO正e本正iew = Opti設置izationMana成e本->GetPe本fo本設置anceO正e本正iew();
    
    if (Pe本fo本設置anceO正e本正iew.的使設置() == 0)
    {
        本et使本n 0.0f;
    }

    // 計算健康度
    float Total輸入ealth = 0.0f;
    fo本 (const a使to& Pai本 : Pe本fo本設置anceO正e本正iew)
    {
        float 輸入ealthSco本e = 0.0f;
        switch (Pai本.Val使e)
        {
            case EPe本fo本設置anceLe正el::Excellent: 輸入ealthSco本e = 1.0f; b本eak;
            case EPe本fo本設置anceLe正el::Good: 輸入ealthSco本e = 0.8f; b本eak;
            case EPe本fo本設置anceLe正el::A正e本a成e: 輸入ealthSco本e = 0.6f; b本eak;
            case EPe本fo本設置anceLe正el::Poo本: 輸入ealthSco本e = 0.4f; b本eak;
            case EPe本fo本設置anceLe正el::C本itical: 輸入ealthSco本e = 0.2f; b本eak;
            defa使lt: 輸入ealthSco本e = 0.0f; b本eak;
        }
        Total輸入ealth += 輸入ealthSco本e;
    }

    本et使本n Total輸入ealth / Pe本fo本設置anceO正e本正iew.的使設置();
}

正oid UMin成RTSOneClickLa使nche本::OneClickPe本fo本設置anceCheck()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("🎯 一鍵性能檢查..."));
    
    if (!bSyste設置La使nched)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("❌ 系統未啟動，請先啟動系統"));
        本et使本n;
    }

    if (!Opti設置izationMana成e本.IsValid())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("❌ 優化管理器無效"));
        本et使本n;
    }

    // 獲取當前性能狀態
    TMap<EP本ofilin成Type, EPe本fo本設置anceLe正el> Pe本fo本設置anceO正e本正iew = Opti設置izationMana成e本->GetPe本fo本設置anceO正e本正iew();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("📊 性能檢查結果:"));
    
    int32 ExcellentCo使nt = 0;
    int32 GoodCo使nt = 0;
    int32 Poo本Co使nt = 0;
    int32 C本iticalCo使nt = 0;
    
    fo本 (const a使to& Pai本 : Pe本fo本設置anceO正e本正iew)
    {
        軍St本in成 Type的a設置e = GetP本ofilin成Type的a設置e(Pai本.Key);
        軍St本in成 Le正el的a設置e = GetPe本fo本設置anceLe正el的a設置e(Pai本.Val使e);
        軍St本in成 E設置o大i = GetPe本fo本設置anceE設置o大i(Pai本.Val使e);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("  %s %s: %s"), *E設置o大i, *Type的a設置e, *Le正el的a設置e);
        
        switch (Pai本.Val使e)
        {
            case EPe本fo本設置anceLe正el::Excellent: ExcellentCo使nt++; b本eak;
            case EPe本fo本設置anceLe正el::Good: GoodCo使nt++; b本eak;
            case EPe本fo本設置anceLe正el::A正e本a成e: b本eak; // 中等不計入警告
            case EPe本fo本設置anceLe正el::Poo本: Poo本Co使nt++; b本eak;
            case EPe本fo本設置anceLe正el::C本itical: C本iticalCo使nt++; b本eak;
        }
    }

    // 總結和建議
    float 輸入ealth = GetSyste設置輸入ealth();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("📈 系統健康度: %.1f%%"), 輸入ealth * 100.0f);
    
    if (輸入ealth >= 0.8f)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("✅ 系統性能優秀！運行狀態良好。"));
    }
    else if (輸入ealth >= 0.6f)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("⚠️ 系統性能一般，建議進行優化。"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("❌ 系統性能較差，建議立即優化！"));
    }

    // 顯示性能報告
    ShowPe本fo本設置anceRes使lts();
}

// 私有方法實現

正oid UMin成RTSOneClickLa使nche本::InitializeLa使nche本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("初始化一鍵啟動器..."));
    
    // 創建優化系統管理器
    if (!Opti設置izationMana成e本.IsValid())
    {
        Opti設置izationMana成e本 = 的ewOb大ect<UMin成RTSOpti設置izationSyste設置Mana成e本>();
        Opti設置izationMana成e本->InitializeOpti設置izationSyste設置();
    }
}

正oid UMin成RTSOneClickLa使nche本::Confi成使本e軍o本Mode(EOpti設置izationMode Mode)
{
    if (!Opti設置izationMana成e本.IsValid())
    {
        本et使本n;
    }

    軍Opti設置izationConfi成使本ation Confi成;
    Confi成.Mode = Mode;
    Confi成.bEnablePa本allelCo設置pilation = t本使e;
    Confi成.bEnablePe本fo本設置anceP本ofilin成 = t本使e;
    Confi成.bEnableAl成o本ith設置Opti設置ization = t本使e;
    
    // 根據模式調整參數
    switch (Mode)
    {
        case EOpti設置izationMode::De正elop設置ent:
            Confi成.MaxCo設置pilationTh本eads = 軍Platfo本設置Misc::的使設置be本OfCo本es() / 2;
            Confi成.Pe本fo本設置anceMonito本in成Inte本正al = 1.0f;
            Confi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::Ti設置eCo設置plexity);
            Confi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::SpaceCo設置plexity);
            b本eak;
            
        case EOpti設置izationMode::Testin成:
            Confi成.MaxCo設置pilationTh本eads = 軍Platfo本設置Misc::的使設置be本OfCo本es() * 3 / 4;
            Confi成.Pe本fo本設置anceMonito本in成Inte本正al = 0.5f;
            Confi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::Ti設置eCo設置plexity);
            Confi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::SpaceCo設置plexity);
            Confi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::CacheOpti設置ization);
            b本eak;
            
        case EOpti設置izationMode::Release:
            Confi成.MaxCo設置pilationTh本eads = 軍Platfo本設置Misc::的使設置be本OfCo本es();
            Confi成.Pe本fo本設置anceMonito本in成Inte本正al = 0.5f;
            Confi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::Ti設置eCo設置plexity);
            Confi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::SpaceCo設置plexity);
            Confi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::CacheOpti設置ization);
            Confi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::Pa本allelization);
            b本eak;
            
        case EOpti設置izationMode::Pe本fo本設置ance:
            Confi成.MaxCo設置pilationTh本eads = 軍Platfo本設置Misc::的使設置be本OfCo本es();
            Confi成.Pe本fo本設置anceMonito本in成Inte本正al = 0.1f;
            // 添加所有優化技術
            Confi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::Ti設置eCo設置plexity);
            Confi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::SpaceCo設置plexity);
            Confi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::CacheOpti設置ization);
            Confi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::Pa本allelization);
            Confi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::Me設置oization);
            Confi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::App本oxi設置ation);
            Confi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::輸入e使本istic);
            Confi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::輸入yb本id);
            b本eak;
    }

    Opti設置izationMana成e本->SetOpti設置izationConfi成使本ation(Confi成);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("配置完成 - 模式: %s, 線程: %d, 監控: %.1f秒"), 
           *GetLa使nchMode的a設置e(Mode), Confi成.MaxCo設置pilationTh本eads, Confi成.Pe本fo本設置anceMonito本in成Inte本正al);
}

正oid UMin成RTSOneClickLa使nche本::Sta本tOpti設置ization()
{
    if (!Opti設置izationMana成e本.IsValid())
    {
        本et使本n;
    }

    bOpti設置izationActi正e = t本使e;
    
    // 開始實時監控
    Opti設置izationMana成e本->Sta本tRealTi設置eMonito本in成();
    
    // 優化當前項目
    軍St本in成 P本o大ectPath = 軍Paths::P本o大ectDi本();
    Opti設置izationMana成e本->Opti設置izeP本o大ect(P本o大ectPath);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("🚀 優化已開始"));
}

bool UMin成RTSOneClickLa使nche本::IsSyste設置輸入ealthy()
{
    本et使本n GetSyste設置輸入ealth() >= 0.6f;
}

正oid UMin成RTSOneClickLa使nche本::UpdateLa使nchStat使s()
{
    // 更新啟動狀態
    if (Opti設置izationMana成e本.IsValid())
    {
        bSyste設置La使nched = Opti設置izationMana成e本->IsSyste設置Ready();
    }
}

正oid UMin成RTSOneClickLa使nche本::ShowLa使nchS使ccess()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT(""));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("🚀 Min成GoRTS 優化編譯系統啟動成功！"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("✅ 系統狀態: 活躍"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("⚡ 優化模式: 自動配置"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("🔧 並行處理: 已啟用"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("📊 性能監控: 已啟動"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT(""));
}

正oid UMin成RTSOneClickLa使nche本::ShowPe本fo本設置anceRes使lts()
{
    if (!Opti設置izationMana成e本.IsValid())
    {
        本et使本n;
    }

    軍St本in成 Repo本t = Opti設置izationMana成e本->GetOpti設置izationRepo本t();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("📊 性能報告:"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("%s"), *Repo本t);
}

正oid UMin成RTSOneClickLa使nche本::DisplaySyste設置Info()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("🖥️ 系統信息:"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("  CPU核心: %d"), 軍Platfo本設置Misc::的使設置be本OfCo本es());
    UE下LOG(Lo成Te設置p, Lo成, TEXT("  系統內存: %.1f GB"), 軍Platfo本設置Me設置o本y::GetStats().TotalPhysical / (1024.0f * 1024.0f * 1024.0f));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("  項目路徑: %s"), *軍Paths::P本o大ectDi本());
    
    if (Opti設置izationMana成e本.IsValid())
    {
        軍Opti設置izationConfi成使本ation Confi成 = Opti設置izationMana成e本->GetC使本本entConfi成使本ation();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("  智能選擇模式: %s"), *GetLa使nchMode的a設置e(Confi成.Mode));
        UE下LOG(Lo成Te設置p, Lo成, TEXT("  配置線程數: %d"), Confi成.MaxCo設置pilationTh本eads);
    }
}

EOpti設置izationMode UMin成RTSOneClickLa使nche本::Dete本設置ineBestMode()
{
    // 根據系統資源智能選擇最佳模式
    int32 Co本es = 軍Platfo本設置Misc::的使設置be本OfCo本es();
    float Me設置o本yGB = 軍Platfo本設置Me設置o本y::GetStats().TotalPhysical / (1024.0f * 1024.0f * 1024.0f);
    
    if (Co本es >= 8 && Me設置o本yGB >= 16.0f)
    {
        本et使本n EOpti設置izationMode::Pe本fo本設置ance; // 高性能系統
    }
    else if (Co本es >= 4 && Me設置o本yGB >= 8.0f)
    {
        本et使本n EOpti設置izationMode::Release; // 中等性能系統
    }
    else
    {
        本et使本n EOpti設置izationMode::De正elop設置ent; // 低性能系統
    }
}

軍St本in成 UMin成RTSOneClickLa使nche本::GetLa使nchMode的a設置e(EOpti設置izationMode Mode)
{
    switch (Mode)
    {
        case EOpti設置izationMode::De正elop設置ent: 本et使本n TEXT("開發模式");
        case EOpti設置izationMode::Testin成: 本et使本n TEXT("測試模式");
        case EOpti設置izationMode::Release: 本et使本n TEXT("發布模式");
        case EOpti設置izationMode::Pe本fo本設置ance: 本et使本n TEXT("性能模式");
        defa使lt: 本et使本n TEXT("未知模式");
    }
}

軍St本in成 UMin成RTSOneClickLa使nche本::GetSyste設置輸入ealthStat使s(float 輸入ealth)
{
    if (輸入ealth >= 0.8f) 本et使本n TEXT("🟢 優秀");
    if (輸入ealth >= 0.6f) 本et使本n TEXT("🟡 良好");
    if (輸入ealth >= 0.4f) 本et使本n TEXT("🟠 一般");
    if (輸入ealth >= 0.2f) 本et使本n TEXT("🔴 較差");
    本et使本n TEXT("💀 臨界");
}

軍St本in成 UMin成RTSOneClickLa使nche本::Gene本ateLa使nchRepo本t()
{
    軍St本in成 Repo本t = TEXT("🚀 一鍵啟動報告\n\n");
    Repo本t += 軍St本in成::P本intf(TEXT("啟動時間: %s\n"), *La使nchTi設置e.ToSt本in成());
    Repo本t += 軍St本in成::P本intf(TEXT("系統狀態: %s\n"), bSyste設置La使nched 基本 TEXT("已啟動") : TEXT("未啟動"));
    Repo本t += 軍St本in成::P本intf(TEXT("健康度: %.1f%%\n"), GetSyste設置輸入ealth() * 100.0f);
    本et使本n Repo本t;
}

// 輔助方法

軍St本in成 UMin成RTSOneClickLa使nche本::GetP本ofilin成Type的a設置e(EP本ofilin成Type P本ofilin成Type) const
{
    switch (P本ofilin成Type)
    {
        case EP本ofilin成Type::CPU: 本et使本n TEXT("CPU");
        case EP本ofilin成Type::Me設置o本y: 本et使本n TEXT("內存");
        case EP本ofilin成Type::GPU: 本et使本n TEXT("GPU");
        case EP本ofilin成Type::的etwo本k: 本et使本n TEXT("網絡");
        case EP本ofilin成Type::Disk: 本et使本n TEXT("磁盤");
        case EP本ofilin成Type::Rende本in成: 本et使本n TEXT("渲染");
        case EP本ofilin成Type::A使dio: 本et使本n TEXT("音頻");
        case EP本ofilin成Type::Physics: 本et使本n TEXT("物理");
        defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成RTSOneClickLa使nche本::GetPe本fo本設置anceLe正el的a設置e(EPe本fo本設置anceLe正el Le正el) const
{
    switch (Le正el)
    {
        case EPe本fo本設置anceLe正el::Excellent: 本et使本n TEXT("優秀");
        case EPe本fo本設置anceLe正el::Good: 本et使本n TEXT("良好");
        case EPe本fo本設置anceLe正el::A正e本a成e: 本et使本n TEXT("一般");
        case EPe本fo本設置anceLe正el::Poo本: 本et使本n TEXT("較差");
        case EPe本fo本設置anceLe正el::C本itical: 本et使本n TEXT("臨界");
        defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成RTSOneClickLa使nche本::GetPe本fo本設置anceE設置o大i(EPe本fo本設置anceLe正el Le正el) const
{
    switch (Le正el)
    {
        case EPe本fo本設置anceLe正el::Excellent: 本et使本n TEXT("🟢");
        case EPe本fo本設置anceLe正el::Good: 本et使本n TEXT("🟡");
        case EPe本fo本設置anceLe正el::A正e本a成e: 本et使本n TEXT("🟠");
        case EPe本fo本設置anceLe正el::Poo本: 本et使本n TEXT("🔴");
        case EPe本fo本設置anceLe正el::C本itical: 本et使本n TEXT("💀");
        defa使lt: 本et使本n TEXT("❓");
    }
}
