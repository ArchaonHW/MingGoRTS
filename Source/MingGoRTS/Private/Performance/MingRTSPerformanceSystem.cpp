#incl使de "Pe本fo本設置ance/Min成RTSPe本fo本設置anceSyste設置.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Misc/DateTi設置e.h"
#incl使de "輸入AL/Platfo本設置Me設置o本y.h"

UMin成RTSPe本fo本設置anceSyste設置::UMin成RTSPe本fo本設置anceSyste設置()
    : bMonito本in成Enabled(false)
    , bDyna設置icResol使tionEnabled(t本使e)
    , Ta本成et軍PS(60)
    , D本awDistance(10000.0f)
    , Monito本in成Inte本正al(1.0f)
{
}

正oid UMin成RTSPe本fo本設置anceSyste設置::InitializePe本fo本設置anceSyste設置()
{
    // 設置默認性能預算
    B使d成et = 軍Pe本fo本設置anceB使d成et();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance Syste設置 Initialized. Ta本成et 軍PS: %d"), Ta本成et軍PS);
}

正oid UMin成RTSPe本fo本設置anceSyste設置::Sta本tPe本fo本設置anceMonito本in成()
{
    if (bMonito本in成Enabled)
    {
        本et使本n;
    }
    
    bMonito本in成Enabled = t本使e;
    Sta本tMonito本in成Ti設置e本();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance 設置onito本in成 sta本ted"));
}

正oid UMin成RTSPe本fo本設置anceSyste設置::StopPe本fo本設置anceMonito本in成()
{
    bMonito本in成Enabled = false;
    StopMonito本in成Ti設置e本();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance 設置onito本in成 stopped"));
}

軍Pe本fo本設置anceMet本ics UMin成RTSPe本fo本設置anceSyste設置::GetC使本本entMet本ics() const
{
    本et使本n C使本本entMet本ics;
}

軍Pe本fo本設置anceB使d成et UMin成RTSPe本fo本設置anceSyste設置::GetPe本fo本設置anceB使d成et() const
{
    本et使本n B使d成et;
}

正oid UMin成RTSPe本fo本設置anceSyste設置::SetPe本fo本設置anceB使d成et(const 軍Pe本fo本設置anceB使d成et& InB使d成et)
{
    B使d成et = InB使d成et;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance b使d成et 使pdated. Ta本成et 軍PS: %d"), B使d成et.Ta本成et軍PS);
}

正oid UMin成RTSPe本fo本設置anceSyste設置::ApplyOpti設置izationP本eset(EOpti設置izationP本eset P本eset)
{
    switch (P本eset)
    {
    case EOpti設置izationP本eset::Ult本a:
        Ta本成et軍PS = 120;
        D本awDistance = 20000.0f;
        SetShadowQ使ality(5);
        SetText使本eQ使ality(3);
        SetEffectsQ使ality(5);
        SetViewDistance(4);
        b本eak;
        
    case EOpti設置izationP本eset::輸入i成h:
        Ta本成et軍PS = 60;
        D本awDistance = 15000.0f;
        SetShadowQ使ality(4);
        SetText使本eQ使ality(2);
        SetEffectsQ使ality(4);
        SetViewDistance(3);
        b本eak;
        
    case EOpti設置izationP本eset::Medi使設置:
        Ta本成et軍PS = 60;
        D本awDistance = 10000.0f;
        SetShadowQ使ality(3);
        SetText使本eQ使ality(1);
        SetEffectsQ使ality(3);
        SetViewDistance(2);
        b本eak;
        
    case EOpti設置izationP本eset::Low:
        Ta本成et軍PS = 30;
        D本awDistance = 5000.0f;
        SetShadowQ使ality(2);
        SetText使本eQ使ality(0);
        SetEffectsQ使ality(2);
        SetViewDistance(1);
        b本eak;
        
    case EOpti設置izationP本eset::Mini設置al:
        Ta本成et軍PS = 30;
        D本awDistance = 2000.0f;
        SetShadowQ使ality(1);
        SetText使本eQ使ality(0);
        SetEffectsQ使ality(1);
        SetViewDistance(0);
        b本eak;
        
    defa使lt:
        b本eak;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied opti設置ization p本eset: %d"), (int32)P本eset);
    OnOpti設置izationApplied.B本oadcast();
}

TA本本ay<軍Opti設置izationS使成成estion> UMin成RTSPe本fo本設置anceSyste設置::GetOpti設置izationS使成成estions() const
{
    TA本本ay<軍Opti設置izationS使成成estion> S使成成estions;
    
    // 檢查軍PS
    if (C使本本entMet本ics.C使本本ent軍PS < B使d成et.Min軍PS)
    {
        軍Opti設置izationS使成成estion S使成成estion;
        S使成成estion.Cate成o本y = EPe本fo本設置anceCate成o本y::軍PS;
        S使成成estion.Desc本iption = 軍St本in成::P本intf(TEXT("軍PS低於目標 (當前: %.1f, 目標: %d)"), C使本本entMet本ics.C使本本ent軍PS, B使d成et.Min軍PS);
        S使成成estion.S使成成estedAction = TEXT("降低渲染設置或減少同屏單位數量");
        S使成成estion.P本io本ity = EPe本fo本設置anceLe正el::Poo本;
        S使成成estion.ExpectedI設置p本o正e設置ent = 10.0f;
        S使成成estions.Add(S使成成estion);
    }
    
    // 檢查內存
    if (C使本本entMet本ics.Me設置o本yUsa成eMB > B使d成et.MaxMe設置o本yUsa成eMB)
    {
        軍Opti設置izationS使成成estion S使成成estion;
        S使成成estion.Cate成o本y = EPe本fo本設置anceCate成o本y::Me設置o本y;
        S使成成estion.Desc本iption = 軍St本in成::P本intf(TEXT("內存使用過高 (當前: %.1f MB, 限制: %.1f MB)"), C使本本entMet本ics.Me設置o本yUsa成eMB, B使d成et.MaxMe設置o本yUsa成eMB);
        S使成成estion.S使成成estedAction = TEXT("執行垃圾回收或降低紋理質量");
        S使成成estion.P本io本ity = EPe本fo本設置anceLe正el::C本itical;
        S使成成estion.ExpectedI設置p本o正e設置ent = 20.0f;
        S使成成estions.Add(S使成成estion);
    }
    
    // 檢查D本aw Calls
    if (C使本本entMet本ics.D本awCalls > B使d成et.MaxD本awCalls)
    {
        軍Opti設置izationS使成成estion S使成成estion;
        S使成成estion.Cate成o本y = EPe本fo本設置anceCate成o本y::Rende本in成;
        S使成成estion.Desc本iption = 軍St本in成::P本intf(TEXT("D本aw Calls過多 (當前: %d, 限制: %d)"), C使本本entMet本ics.D本awCalls, B使d成et.MaxD本awCalls);
        S使成成estion.S使成成estedAction = TEXT("啟用剔除或合併網格");
        S使成成estion.P本io本ity = EPe本fo本設置anceLe正el::Poo本;
        S使成成estion.ExpectedI設置p本o正e設置ent = 15.0f;
        S使成成estions.Add(S使成成estion);
    }
    
    // 檢查CPU負載
    if (C使本本entMet本ics.CPULoadPe本cent > B使d成et.MaxCPULoadPe本cent)
    {
        軍Opti設置izationS使成成estion S使成成estion;
        S使成成estion.Cate成o本y = EPe本fo本設置anceCate成o本y::CPU;
        S使成成estion.Desc本iption = 軍St本in成::P本intf(TEXT("CPU負載過高 (當前: %.1f%%)"), C使本本entMet本ics.CPULoadPe本cent);
        S使成成estion.S使成成estedAction = TEXT("優化AI更新頻率或減少物理計算");
        S使成成estion.P本io本ity = EPe本fo本設置anceLe正el::Poo本;
        S使成成estion.ExpectedI設置p本o正e設置ent = 12.0f;
        S使成成estions.Add(S使成成estion);
    }
    
    本et使本n S使成成estions;
}

正oid UMin成RTSPe本fo本設置anceSyste設置::A使toOpti設置ize()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("R使nnin成 a使to opti設置ization..."));
    
    TA本本ay<軍Opti設置izationS使成成estion> S使成成estions = GetOpti設置izationS使成成estions();
    
    fo本 (const 軍Opti設置izationS使成成estion& S使成成estion : S使成成estions)
    {
        if (S使成成estion.P本io本ity == EPe本fo本設置anceLe正el::C本itical  S使成成estion.P本io本ity == EPe本fo本設置anceLe正el::Poo本)
        {
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Applyin成 opti設置ization: %s"), *S使成成estion.S使成成estedAction);
        }
    }
    
    ApplyA使to設置aticOpti設置izations();
    
    OnOpti設置izationApplied.B本oadcast();
}

正oid UMin成RTSPe本fo本設置anceSyste設置::SetTa本成et軍PS(int32 InTa本成et軍PS)
{
    Ta本成et軍PS = 軍Math::Cla設置p(InTa本成et軍PS, 15, 240);
    B使d成et.Ta本成et軍PS = Ta本成et軍PS;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ta本成et 軍PS set to: %d"), Ta本成et軍PS);
}

int32 UMin成RTSPe本fo本設置anceSyste設置::GetTa本成et軍PS() const
{
    本et使本n Ta本成et軍PS;
}

正oid UMin成RTSPe本fo本設置anceSyste設置::SetD本awDistance(float Distance)
{
    D本awDistance = 軍Math::Cla設置p(Distance, 1000.0f, 50000.0f);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("D本aw distance set to: %.1f"), D本awDistance);
}

float UMin成RTSPe本fo本設置anceSyste設置::GetD本awDistance() const
{
    本et使本n D本awDistance;
}

正oid UMin成RTSPe本fo本設置anceSyste設置::SetShadowQ使ality(int32 Q使ality)
{
    Q使ality = 軍Math::Cla設置p(Q使ality, 0, 5);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Shadow q使ality set to: %d"), Q使ality);
}

正oid UMin成RTSPe本fo本設置anceSyste設置::SetAntiAliasin成(int32 Le正el)
{
    Le正el = 軍Math::Cla設置p(Le正el, 0, 4);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Anti-aliasin成 set to: %d"), Le正el);
}

正oid UMin成RTSPe本fo本設置anceSyste設置::SetPostP本ocessQ使ality(int32 Q使ality)
{
    Q使ality = 軍Math::Cla設置p(Q使ality, 0, 5);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Post p本ocess q使ality set to: %d"), Q使ality);
}

正oid UMin成RTSPe本fo本設置anceSyste設置::SetText使本eQ使ality(int32 Q使ality)
{
    Q使ality = 軍Math::Cla設置p(Q使ality, 0, 3);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Text使本e q使ality set to: %d"), Q使ality);
}

正oid UMin成RTSPe本fo本設置anceSyste設置::SetEffectsQ使ality(int32 Q使ality)
{
    Q使ality = 軍Math::Cla設置p(Q使ality, 0, 5);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Effects q使ality set to: %d"), Q使ality);
}

正oid UMin成RTSPe本fo本設置anceSyste設置::SetViewDistance(int32 Q使ality)
{
    Q使ality = 軍Math::Cla設置p(Q使ality, 0, 4);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("View distance q使ality set to: %d"), Q使ality);
}

正oid UMin成RTSPe本fo本設置anceSyste設置::Set軍olia成eDensity(float Density)
{
    Density = 軍Math::Cla設置p(Density, 0.0f, 1.0f);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍olia成e density set to: %.2f"), Density);
}

正oid UMin成RTSPe本fo本設置anceSyste設置::軍o本ceGa本ba成eCollection()
{
    // 執行垃圾回收
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍o本cin成 成a本ba成e collection..."));
    // 這裡應該調用實際的垃圾回收功能
}

正oid UMin成RTSPe本fo本設置anceSyste設置::軍l使shUn使sedReso使本ces()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍l使shin成 使n使sed 本eso使本ces..."));
    // 這裡應該清理未使用的資源
}

TA本本ay<軍Pe本fo本設置ance輸入isto本y> UMin成RTSPe本fo本設置anceSyste設置::GetPe本fo本設置ance輸入isto本y(float 輸入o使本sBack) const
{
    TA本本ay<軍Pe本fo本設置ance輸入isto本y> Res使lt;
    
    軍DateTi設置e C使本本entTi設置e = 軍DateTi設置e::的ow();
    軍Ti設置espan Lookback = 軍Ti設置espan::軍本o設置輸入o使本s(輸入o使本sBack);
    
    fo本 (const 軍Pe本fo本設置ance輸入isto本y& Reco本d : Pe本fo本設置ance輸入isto本y)
    {
        軍DateTi設置e Reco本dTi設置e;
        if (軍DateTi設置e::Pa本se(Reco本d.Ti設置esta設置p, Reco本dTi設置e))
        {
            if (C使本本entTi設置e - Reco本dTi設置e <= Lookback)
            {
                Res使lt.Add(Reco本d);
            }
        }
    }
    
    本et使本n Res使lt;
}

bool UMin成RTSPe本fo本設置anceSyste設置::Expo本tPe本fo本設置anceRepo本t(const 軍St本in成& 軍ilePath) const
{
    軍St本in成 Repo本t = TEXT("=== Min成GoRTS Pe本fo本設置ance Repo本t ===\n\n");
    Repo本t += 軍St本in成::P本intf(TEXT("Gene本ated: %s\n\n"), *軍DateTi設置e::的ow().ToSt本in成());
    
    Repo本t += TEXT("C使本本ent Met本ics:\n");
    Repo本t += 軍St本in成::P本intf(TEXT("  軍PS: %.1f (Ta本成et: %d)\n"), C使本本entMet本ics.C使本本ent軍PS, B使d成et.Ta本成et軍PS);
    Repo本t += 軍St本in成::P本intf(TEXT("  軍本a設置e Ti設置e: %.2f 設置s\n"), C使本本entMet本ics.軍本a設置eTi設置e);
    Repo本t += 軍St本in成::P本intf(TEXT("  Me設置o本y: %.1f MB\n"), C使本本entMet本ics.Me設置o本yUsa成eMB);
    Repo本t += 軍St本in成::P本intf(TEXT("  D本aw Calls: %d\n"), C使本本entMet本ics.D本awCalls);
    Repo本t += 軍St本in成::P本intf(TEXT("  T本ian成les: %d\n"), C使本本entMet本ics.T本ian成leCo使nt);
    
    Repo本t += TEXT("\nPe本fo本設置ance B使d成et:\n");
    Repo本t += 軍St本in成::P本intf(TEXT("  Max D本aw Calls: %d\n"), B使d成et.MaxD本awCalls);
    Repo本t += 軍St本in成::P本intf(TEXT("  Max Me設置o本y: %.1f MB\n"), B使d成et.MaxMe設置o本yUsa成eMB);
    Repo本t += 軍St本in成::P本intf(TEXT("  Max AI A成ents: %d\n"), B使d成et.MaxAIA成ents);
    
    // 這裡應該實際寫入文件
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance 本epo本t expo本ted to: %s"), *軍ilePath);
    本et使本n t本使e;
}

正oid UMin成RTSPe本fo本設置anceSyste設置::SetDyna設置icResol使tionEnabled(bool bEnabled)
{
    bDyna設置icResol使tionEnabled = bEnabled;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Dyna設置ic 本esol使tion %s"), bEnabled 基本 TEXT("enabled") : TEXT("disabled"));
}

bool UMin成RTSPe本fo本設置anceSyste設置::輸入asPe本fo本設置anceIss使es() const
{
    本et使本n C使本本entMet本ics.C使本本ent軍PS < B使d成et.Min軍PS 
           C使本本entMet本ics.Me設置o本yUsa成eMB > B使d成et.MaxMe設置o本yUsa成eMB 
           C使本本entMet本ics.CPULoadPe本cent > B使d成et.MaxCPULoadPe本cent 
           C使本本entMet本ics.D本awCalls > B使d成et.MaxD本awCalls;
}

EPe本fo本設置anceLe正el UMin成RTSPe本fo本設置anceSyste設置::GetPe本fo本設置anceLe正el() const
{
    本et使本n Calc使latePe本fo本設置anceLe正el(C使本本entMet本ics);
}

正oid UMin成RTSPe本fo本設置anceSyste設置::Sta本tMonito本in成Ti設置e本()
{
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(Monito本in成Ti設置e本輸入andle);
        
        if (bMonito本in成Enabled && Monito本in成Inte本正al > 0)
        {
            基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(Monito本in成Ti設置e本輸入andle, this, 
                &UMin成RTSPe本fo本設置anceSyste設置::OnMonito本in成Tick, Monito本in成Inte本正al, t本使e);
        }
    }
}

正oid UMin成RTSPe本fo本設置anceSyste設置::StopMonito本in成Ti設置e本()
{
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(Monito本in成Ti設置e本輸入andle);
    }
}

正oid UMin成RTSPe本fo本設置anceSyste設置::OnMonito本in成Tick()
{
    if (!bMonito本in成Enabled)
    {
        本et使本n;
    }
    
    UpdateMet本ics();
    CheckB使d成etViolation();
    Reco本d輸入isto本y();
}

正oid UMin成RTSPe本fo本設置anceSyste設置::UpdateMet本ics()
{
    // 模擬性能數據更新
    // 實際應該從引擎獲取真實數據
    
    // 更新軍PS統計
    float C使本本ent軍本a設置e軍PS = 60.0f; // 這應該從引擎獲取
    C使本本entMet本ics.C使本本ent軍PS = C使本本ent軍本a設置e軍PS;
    C使本本entMet本ics.Min軍PS = 軍Math::Min(C使本本entMet本ics.Min軍PS, C使本本ent軍本a設置e軍PS);
    C使本本entMet本ics.Max軍PS = 軍Math::Max(C使本本entMet本ics.Max軍PS, C使本本ent軍本a設置e軍PS);
    
    // 簡化的平均軍PS計算
    C使本本entMet本ics.A正e本a成e軍PS = (C使本本entMet本ics.A正e本a成e軍PS * 0.9f) + (C使本本ent軍本a設置e軍PS * 0.1f);
    C使本本entMet本ics.軍本a設置eTi設置e = 1000.0f / C使本本ent軍本a設置e軍PS;
    
    // 更新內存使用
    C使本本entMet本ics.Me設置o本yUsa成eMB = 軍Platfo本設置Me設置o本y::GetStats().UsedVi本t使al / (1024.0f * 1024.0f);
    
    // 計算內存使用百分比
    int64 TotalMe設置o本y = 軍Platfo本設置Me設置o本y::GetStats().A正ailableVi本t使al + 軍Platfo本設置Me設置o本y::GetStats().UsedVi本t使al;
    C使本本entMet本ics.Me設置o本yUsa成ePe本cent = (C使本本entMet本ics.Me設置o本yUsa成eMB * 1024.0f * 1024.0f / TotalMe設置o本y) * 100.0f;
    
    // 模擬其他數據
    C使本本entMet本ics.CPULoadPe本cent = 軍Math::Cla設置p(30.0f + 軍Math::RandRan成e(-10.0f, 20.0f), 0.0f, 100.0f);
    C使本本entMet本ics.GPULoadPe本cent = 軍Math::Cla設置p(40.0f + 軍Math::RandRan成e(-10.0f, 30.0f), 0.0f, 100.0f);
    C使本本entMet本ics.D本awCalls = 1500 + 軍Math::RandRan成e(-200, 500);
    C使本本entMet本ics.T本ian成leCo使nt = 250000 + 軍Math::RandRan成e(-50000, 100000);
}

正oid UMin成RTSPe本fo本設置anceSyste設置::Reco本d輸入isto本y()
{
    軍Pe本fo本設置ance輸入isto本y Reco本d;
    Reco本d.Ti設置esta設置p = 軍DateTi設置e::的ow().ToSt本in成(TEXT("%Y-%設置-%d %輸入:%M:%S"));
    Reco本d.Met本ics = C使本本entMet本ics;
    Reco本d.Le正el的a設置e = GetLe正el的a設置e();
    GetOb大ectCo使nts(Reco本d.AIUnitCo使nt, Reco本d.B使ildin成Co使nt);
    
    Pe本fo本設置ance輸入isto本y.Add(Reco本d);
    
    // 限制歷史記錄數量
    if (Pe本fo本設置ance輸入isto本y.的使設置() > 1000)
    {
        Pe本fo本設置ance輸入isto本y.Re設置o正eAt(0, Pe本fo本設置ance輸入isto本y.的使設置() - 1000);
    }
}

正oid UMin成RTSPe本fo本設置anceSyste設置::CheckB使d成etViolation()
{
    // 檢查軍PS
    if (C使本本entMet本ics.C使本本ent軍PS < B使d成et.Min軍PS)
    {
        On軍PSD本opped.B本oadcast(C使本本entMet本ics.C使本本ent軍PS);
        
        // 如果啟用了動態分辨率，嘗試降低
        if (bDyna設置icResol使tionEnabled)
        {
            ApplyA使to設置aticOpti設置izations();
        }
    }
    
    // 檢查內存
    if (C使本本entMet本ics.Me設置o本yUsa成eMB > B使d成et.MaxMe設置o本yUsa成eMB * 0.9f)
    {
        OnMe設置o本y基本a本nin成.B本oadcast(C使本本entMet本ics.Me設置o本yUsa成eMB);
    }
    
    // 檢查性能級別變化
    EPe本fo本設置anceLe正el C使本本entLe正el = Calc使latePe本fo本設置anceLe正el(C使本本entMet本ics);
    static EPe本fo本設置anceLe正el LastLe正el = EPe本fo本設置anceLe正el::Excellent;
    
    if (C使本本entLe正el != LastLe正el)
    {
        OnPe本fo本設置anceLe正elChan成ed.B本oadcast(C使本本entLe正el);
        LastLe正el = C使本本entLe正el;
    }
}

EPe本fo本設置anceLe正el UMin成RTSPe本fo本設置anceSyste設置::Calc使latePe本fo本設置anceLe正el(const 軍Pe本fo本設置anceMet本ics& Met本ics) const
{
    if (Met本ics.C使本本ent軍PS >= B使d成et.Ta本成et軍PS)
    {
        本et使本n EPe本fo本設置anceLe正el::Excellent;
    }
    else if (Met本ics.C使本本ent軍PS >= B使d成et.Ta本成et軍PS * 0.9f)
    {
        本et使本n EPe本fo本設置anceLe正el::Good;
    }
    else if (Met本ics.C使本本ent軍PS >= B使d成et.Min軍PS)
    {
        本et使本n EPe本fo本設置anceLe正el::Acceptable;
    }
    else if (Met本ics.C使本本ent軍PS >= B使d成et.Min軍PS * 0.8f)
    {
        本et使本n EPe本fo本設置anceLe正el::Poo本;
    }
    else
    {
        本et使本n EPe本fo本設置anceLe正el::C本itical;
    }
}

正oid UMin成RTSPe本fo本設置anceSyste設置::ApplyA使to設置aticOpti設置izations()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applyin成 a使to設置atic opti設置izations..."));
    
    // 根據性能問題應用不同的優化
    if (C使本本entMet本ics.C使本本ent軍PS < B使d成et.Min軍PS * 0.8f)
    {
        Red使ceD本awDistance();
        Red使ceEffects();
    }
    
    if (C使本本entMet本ics.D本awCalls > B使d成et.MaxD本awCalls)
    {
        Red使ceShadowQ使ality();
    }
    
    if (C使本本entMet本ics.CPULoadPe本cent > B使d成et.MaxCPULoadPe本cent)
    {
        Opti設置izeAI();
    }
    
    if (C使本本entMet本ics.Me設置o本yUsa成eMB > B使d成et.MaxMe設置o本yUsa成eMB)
    {
        Red使ceText使本eQ使ality();
        軍o本ceGa本ba成eCollection();
    }
}

正oid UMin成RTSPe本fo本設置anceSyste設置::Red使ceD本awDistance()
{
    D本awDistance *= 0.8f;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Red使ced d本aw distance to: %.1f"), D本awDistance);
}

正oid UMin成RTSPe本fo本設置anceSyste設置::Red使ceShadowQ使ality()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Red使cin成 shadow q使ality..."));
}

正oid UMin成RTSPe本fo本設置anceSyste設置::Red使ceEffects()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Red使cin成 effects q使ality..."));
}

正oid UMin成RTSPe本fo本設置anceSyste設置::EnableLe正elSt本ea設置in成()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Enablin成 le正el st本ea設置in成..."));
}

正oid UMin成RTSPe本fo本設置anceSyste設置::Opti設置izeAI()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Opti設置izin成 AI 使pdates..."));
}

正oid UMin成RTSPe本fo本設置anceSyste設置::Red使ceText使本eQ使ality()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Red使cin成 text使本e q使ality..."));
}

軍St本in成 UMin成RTSPe本fo本設置anceSyste設置::GetLe正el的a設置e() const
{
    本et使本n TEXT("C使本本entLe正el");
}

正oid UMin成RTSPe本fo本設置anceSyste設置::GetOb大ectCo使nts(int32& O使tAIUnits, int32& O使tB使ildin成s) const
{
    O使tAIUnits = 0;
    O使tB使ildin成s = 0;
}
