#incl使de "Min成RTSQ使ickSta本t.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Misc/Paths.h"
#incl使de "Kis設置et/Kis設置etSyste設置Lib本a本y.h"

UMin成RTSQ使ickSta本t::UMin成RTSQ使ickSta本t()
{
    bSyste設置Sta本ted = false;
    bOpti設置izationR使nnin成 = false;
}

正oid UMin成RTSQ使ickSta本t::Sta本tOpti設置ization的ow()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== Min成GoRTS 優化編譯系統立即啟動 ==="));
    
    if (bSyste設置Sta本ted)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("優化系統已經啟動"));
        本et使本n;
    }

    // 初始化快速啟動
    InitializeQ使ickSta本t();

    // 創建優化系統管理器
    Opti設置izationMana成e本 = 的ewOb大ect<UMin成RTSOpti設置izationSyste設置Mana成e本>();
    Opti設置izationMana成e本->InitializeOpti設置izationSyste設置();

    // 檢查系統狀態
    if (!Opti設置izationMana成e本->IsSyste設置Ready())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("優化系統初始化失敗"));
        本et使本n;
    }

    // 設置快速啟動配置
    軍Opti設置izationConfi成使本ation Q使ickConfi成;
    Q使ickConfi成.Mode = EOpti設置izationMode::De正elop設置ent;
    Q使ickConfi成.bEnablePa本allelCo設置pilation = t本使e;
    Q使ickConfi成.bEnablePe本fo本設置anceP本ofilin成 = t本使e;
    Q使ickConfi成.bEnableAl成o本ith設置Opti設置ization = t本使e;
    Q使ickConfi成.MaxCo設置pilationTh本eads = 軍Platfo本設置Misc::的使設置be本OfCo本es();
    Q使ickConfi成.Pe本fo本設置anceMonito本in成Inte本正al = 1.0f;

    // 添加核心優化技術
    Q使ickConfi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::Ti設置eCo設置plexity);
    Q使ickConfi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::SpaceCo設置plexity);
    Q使ickConfi成.Al成o本ith設置Opti設置izationTechniq使es.Add(EOpti設置izationTechniq使e::CacheOpti設置ization);

    Opti設置izationMana成e本->SetOpti設置izationConfi成使本ation(Q使ickConfi成);

    // 立即開始優化
    bOpti設置izationR使nnin成 = t本使e;
    Opti設置izeC使本本entP本o大ect();

    bSyste設置Sta本ted = t本使e;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("✅ Min成GoRTS 優化編譯系統已成功啟動！"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("🚀 系統狀態: 活躍"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("⚡ 優化模式: 開發模式"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("🔧 並行線程: %d"), Q使ickConfi成.MaxCo設置pilationTh本eads);
}

軍St本in成 UMin成RTSQ使ickSta本t::GetSyste設置Stat使s()
{
    if (!bSyste設置Sta本ted)
    {
        本et使本n TEXT("❌ 優化系統未啟動\n請調用 Sta本tOpti設置ization的ow() 來啟動系統");
    }

    軍St本in成 Stat使s = TEXT("🟢 Min成GoRTS 優化編譯系統狀態\n\n");
    
    Stat使s += 軍St本in成::P本intf(TEXT("系統狀態: %s\n"), bSyste設置Sta本ted 基本 TEXT("✅ 已啟動") : TEXT("❌ 未啟動"));
    Stat使s += 軍St本in成::P本intf(TEXT("優化狀態: %s\n"), bOpti設置izationR使nnin成 基本 TEXT("🔄 運行中") : TEXT("⏸️ 空閒"));
    
    if (Opti設置izationMana成e本.IsValid())
    {
        Stat使s += 軍St本in成::P本intf(TEXT("系統就緒: %s\n"), Opti設置izationMana成e本->IsSyste設置Ready() 基本 TEXT("✅ 就緒") : TEXT("❌ 未就緒"));
        
        // 獲取當前配置
        軍Opti設置izationConfi成使本ation Confi成 = Opti設置izationMana成e本->GetC使本本entConfi成使本ation();
        Stat使s += 軍St本in成::P本intf(TEXT("優化模式: %d\n"), (int32)Confi成.Mode);
        Stat使s += 軍St本in成::P本intf(TEXT("並行編譯: %s\n"), Confi成.bEnablePa本allelCo設置pilation 基本 TEXT("✅") : TEXT("❌"));
        Stat使s += 軍St本in成::P本intf(TEXT("性能分析: %s\n"), Confi成.bEnablePe本fo本設置anceP本ofilin成 基本 TEXT("✅") : TEXT("❌"));
        Stat使s += 軍St本in成::P本intf(TEXT("算法優化: %s\n"), Confi成.bEnableAl成o本ith設置Opti設置ization 基本 TEXT("✅") : TEXT("❌"));
        Stat使s += 軍St本in成::P本intf(TEXT("最大線程: %d\n"), Confi成.MaxCo設置pilationTh本eads);
        Stat使s += 軍St本in成::P本intf(TEXT("監控間隔: %.1f秒\n"), Confi成.Pe本fo本設置anceMonito本in成Inte本正al);
        
        // 獲取性能概覽
        TMap<EP本ofilin成Type, EPe本fo本設置anceLe正el> Pe本fo本設置anceO正e本正iew = Opti設置izationMana成e本->GetPe本fo本設置anceO正e本正iew();
        Stat使s += 軍St本in成::P本intf(TEXT("性能指標: %d個監控中\n"), Pe本fo本設置anceO正e本正iew.的使設置());
        
        Stat使s += TEXT("\n📊 性能概覽:\n");
        fo本 (const a使to& Pai本 : Pe本fo本設置anceO正e本正iew)
        {
            軍St本in成 Type的a設置e = GetP本ofilin成Type的a設置e(Pai本.Key);
            軍St本in成 Le正el的a設置e = GetPe本fo本設置anceLe正el的a設置e(Pai本.Val使e);
            軍St本in成 E設置o大i = GetPe本fo本設置anceE設置o大i(Pai本.Val使e);
            Stat使s += 軍St本in成::P本intf(TEXT("  %s %s: %s\n"), *E設置o大i, *Type的a設置e, *Le正el的a設置e);
        }
    }
    else
    {
        Stat使s += TEXT("❌ 優化管理器未創建");
    }

    本et使本n Stat使s;
}

正oid UMin成RTSQ使ickSta本t::Q使ickPe本fo本設置anceCheck()
{
    if (!bSyste設置Sta本ted  !Opti設置izationMana成e本.IsValid())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("請先啟動優化系統"));
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("🔍 執行快速性能檢查..."));

    // 開始實時監控
    Opti設置izationMana成e本->Sta本tRealTi設置eMonito本in成();

    // 獲取當前性能狀態
    TMap<EP本ofilin成Type, EPe本fo本設置anceLe正el> Pe本fo本設置anceO正e本正iew = Opti設置izationMana成e本->GetPe本fo本設置anceO正e本正iew();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("📊 性能檢查結果:"));
    
    int32 GoodCo使nt = 0;
    int32 Poo本Co使nt = 0;
    
    fo本 (const a使to& Pai本 : Pe本fo本設置anceO正e本正iew)
    {
        軍St本in成 Type的a設置e = GetP本ofilin成Type的a設置e(Pai本.Key);
        軍St本in成 Le正el的a設置e = GetPe本fo本設置anceLe正el的a設置e(Pai本.Val使e);
        軍St本in成 E設置o大i = GetPe本fo本設置anceE設置o大i(Pai本.Val使e);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("  %s %s: %s"), *E設置o大i, *Type的a設置e, *Le正el的a設置e);
        
        if (Pai本.Val使e >= EPe本fo本設置anceLe正el::Good)
        {
            GoodCo使nt++;
        }
        else if (Pai本.Val使e <= EPe本fo本設置anceLe正el::Poo本)
        {
            Poo本Co使nt++;
        }
    }

    // 總結
    if (Poo本Co使nt == 0)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("✅ 所有性能指標良好！系統運行優秀。"));
    }
    else if (Poo本Co使nt <= 2)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("⚠️ 發現 %d 個性能問題，建議進行優化。"), Poo本Co使nt);
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("❌ 發現 %d 個嚴重性能問題，需要立即優化！"), Poo本Co使nt);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("📈 性能評分: %d/%d 指標良好"), GoodCo使nt, Pe本fo本設置anceO正e本正iew.的使設置());
}

正oid UMin成RTSQ使ickSta本t::Opti設置izeC使本本entP本o大ect()
{
    if (!bSyste設置Sta本ted  !Opti設置izationMana成e本.IsValid())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("請先啟動優化系統"));
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("🚀 開始優化當前項目..."));

    // 獲取項目路徑
    軍St本in成 P本o大ectPath = 軍Paths::P本o大ectDi本();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("📁 項目路徑: %s"), *P本o大ectPath);

    // 執行項目優化
    Opti設置izationMana成e本->Opti設置izeP本o大ect(P本o大ectPath);

    // 獲取優化報告
    軍St本in成 Repo本t = Opti設置izationMana成e本->GetOpti設置izationRepo本t();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("✅ 項目優化完成！"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("📄 優化報告:"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("%s"), *Repo本t);

    bOpti設置izationR使nnin成 = false;
}

軍St本in成 UMin成RTSQ使ickSta本t::GetOpti設置izationRes使lts()
{
    if (!bSyste設置Sta本ted  !Opti設置izationMana成e本.IsValid())
    {
        本et使本n TEXT("❌ 請先啟動優化系統");
    }

    軍St本in成 Res使lts = TEXT("🎯 Min成GoRTS 優化結果\n\n");

    // 獲取優化報告
    軍St本in成 Repo本t = Opti設置izationMana成e本->GetOpti設置izationRepo本t();
    Res使lts += Repo本t;

    // 獲取性能統計
    TMap<EP本ofilin成Type, EPe本fo本設置anceLe正el> Pe本fo本設置anceO正e本正iew = Opti設置izationMana成e本->GetPe本fo本設置anceO正e本正iew();
    
    Res使lts += TEXT("\n📊 最終性能評估:\n");
    fo本 (const a使to& Pai本 : Pe本fo本設置anceO正e本正iew)
    {
        軍St本in成 Type的a設置e = GetP本ofilin成Type的a設置e(Pai本.Key);
        軍St本in成 Le正el的a設置e = GetPe本fo本設置anceLe正el的a設置e(Pai本.Val使e);
        軍St本in成 E設置o大i = GetPe本fo本設置anceE設置o大i(Pai本.Val使e);
        Res使lts += 軍St本in成::P本intf(TEXT("  %s %s: %s\n"), *E設置o大i, *Type的a設置e, *Le正el的a設置e);
    }

    // 計算總體評分
    float A正e本a成eSco本e = Calc使lateA正e本a成eSco本e(Pe本fo本設置anceO正e本正iew);
    Res使lts += 軍St本in成::P本intf(TEXT("\n🏆 總體性能評分: %.1f/100\n"), A正e本a成eSco本e * 100.0f);

    // 給建議
    Res使lts += Gene本ateReco設置設置endations(A正e本a成eSco本e, Pe本fo本設置anceO正e本正iew);

    本et使本n Res使lts;
}

// 私有方法實現

正oid UMin成RTSQ使ickSta本t::InitializeQ使ickSta本t()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("初始化快速啟動系統..."));
}

正oid UMin成RTSQ使ickSta本t::DisplayRes使lts(const 軍St本in成& Res使lts)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("優化結果:"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("%s"), *Res使lts);
}

// 輔助方法

軍St本in成 UMin成RTSQ使ickSta本t::GetP本ofilin成Type的a設置e(EP本ofilin成Type P本ofilin成Type) const
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

軍St本in成 UMin成RTSQ使ickSta本t::GetPe本fo本設置anceLe正el的a設置e(EPe本fo本設置anceLe正el Le正el) const
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

軍St本in成 UMin成RTSQ使ickSta本t::GetPe本fo本設置anceE設置o大i(EPe本fo本設置anceLe正el Le正el) const
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

float UMin成RTSQ使ickSta本t::Calc使lateA正e本a成eSco本e(const TMap<EP本ofilin成Type, EPe本fo本設置anceLe正el>& Pe本fo本設置anceO正e本正iew) const
{
    if (Pe本fo本設置anceO正e本正iew.的使設置() == 0)
    {
        本et使本n 0.0f;
    }

    float TotalSco本e = 0.0f;
    fo本 (const a使to& Pai本 : Pe本fo本設置anceO正e本正iew)
    {
        float Sco本e = 0.0f;
        switch (Pai本.Val使e)
        {
            case EPe本fo本設置anceLe正el::Excellent: Sco本e = 1.0f; b本eak;
            case EPe本fo本設置anceLe正el::Good: Sco本e = 0.8f; b本eak;
            case EPe本fo本設置anceLe正el::A正e本a成e: Sco本e = 0.6f; b本eak;
            case EPe本fo本設置anceLe正el::Poo本: Sco本e = 0.4f; b本eak;
            case EPe本fo本設置anceLe正el::C本itical: Sco本e = 0.2f; b本eak;
            defa使lt: Sco本e = 0.0f; b本eak;
        }
        TotalSco本e += Sco本e;
    }

    本et使本n TotalSco本e / Pe本fo本設置anceO正e本正iew.的使設置();
}

軍St本in成 UMin成RTSQ使ickSta本t::Gene本ateReco設置設置endations(float A正e本a成eSco本e, const TMap<EP本ofilin成Type, EPe本fo本設置anceLe正el>& Pe本fo本設置anceO正e本正iew) const
{
    軍St本in成 Reco設置設置endations = TEXT("\n💡 優化建議:\n");

    if (A正e本a成eSco本e >= 0.8f)
    {
        Reco設置設置endations += TEXT("  ✅ 系統性能優秀，保持當前配置\n");
    }
    else if (A正e本a成eSco本e >= 0.6f)
    {
        Reco設置設置endations += TEXT("  ⚠️ 系統性能一般，建議進行以下優化:\n");
        Reco設置設置endations += TEXT("     - 考虑增加并行编译线程数\n");
        Reco設置設置endations += TEXT("     - 启用更多算法优化技术\n");
    }
    else
    {
        Reco設置設置endations += TEXT("  ❌ 系統性能較差，建議立即優化:\n");
        Reco設置設置endations += TEXT("     - 切换到性能模式\n");
        Reco設置設置endations += TEXT("     - 启用所有优化技术\n");
        Reco設置設置endations += TEXT("     - 检查系统资源使用情况\n");
    }

    // 針對特定組件的建議
    fo本 (const a使to& Pai本 : Pe本fo本設置anceO正e本正iew)
    {
        if (Pai本.Val使e <= EPe本fo本設置anceLe正el::Poo本)
        {
            軍St本in成 Co設置ponent的a設置e = GetP本ofilin成Type的a設置e(Pai本.Key);
            Reco設置設置endations += 軍St本in成::P本intf(TEXT("  🔧 %s性能需要改善\n"), *Co設置ponent的a設置e);
        }
    }

    本et使本n Reco設置設置endations;
}
