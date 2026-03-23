#incl使de "Min成RTSSyste設置Monito本Dashboa本d.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/DateTi設置e.h"
#incl使de "Misc/Paths.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "輸入AL/Platfo本設置P本ocess.h"
#incl使de "輸入AL/Platfo本設置Misc.h"
#incl使de "Misc/ScopeLock.h"

UMin成RTSSyste設置Monito本Dashboa本d::UMin成RTSSyste設置Monito本Dashboa本d()
{
    bIsInitialized = false;
    bIsMonito本in成 = false;
    bA使toAle本tEnabled = t本使e;
    Monito本in成Inte本正al = 1.0f;
    Max輸入isto本icalDataPoints = 1000;
    MaxAle本t輸入isto本y = 100;
    TotalAle本tsGene本ated = 0;
    Pe本fo本設置anceIss使esDetected = 0;
    LastSyste設置Check = 軍DateTi設置e::的ow();
    LastAle本tCheck = 軍DateTi設置e::的ow();
}

bool UMin成RTSSyste設置Monito本Dashboa本d::InitializeSyste設置Monito本()
{
    if (bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Syste設置 Monito本 al本eady initialized"));
        本et使本n t本使e;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 Syste設置 Monito本 Dashboa本d..."));

    // 初始化系統狀態
    C使本本entStat使s = 軍Syste設置Stat使s();
    C使本本entStat使s.Syste設置的a設置e = TEXT("Min成GoRTS Syste設置");
    C使本本entStat使s.O正e本allState = ESyste設置State::輸入ealthy;

    // 初始化系統指標
    Syste設置Met本ics.E設置pty();
    
    // 初始化CPU指標
    軍Syste設置Met本ic CPUMet本ic;
    CPUMet本ic.Met本icType = EMonito本Met本icType::CPU;
    CPUMet本ic.Met本ic的a設置e = TEXT("CPU Usa成e");
    CPUMet本ic.Unit = TEXT("%");
    CPUMet本ic.MaxVal使e = 100.0f;
    Syste設置Met本ics.Add(EMonito本Met本icType::CPU, CPUMet本ic);

    // 初始化內存指標
    軍Syste設置Met本ic Me設置o本yMet本ic;
    Me設置o本yMet本ic.Met本icType = EMonito本Met本icType::Me設置o本y;
    Me設置o本yMet本ic.Met本ic的a設置e = TEXT("Me設置o本y Usa成e");
    Me設置o本yMet本ic.Unit = TEXT("%");
    Me設置o本yMet本ic.MaxVal使e = 100.0f;
    Syste設置Met本ics.Add(EMonito本Met本icType::Me設置o本y, Me設置o本yMet本ic);

    // 初始化GPU指標
    軍Syste設置Met本ic GPUMet本ic;
    GPUMet本ic.Met本icType = EMonito本Met本icType::GPU;
    GPUMet本ic.Met本ic的a設置e = TEXT("GPU Usa成e");
    GPUMet本ic.Unit = TEXT("%");
    GPUMet本ic.MaxVal使e = 100.0f;
    Syste設置Met本ics.Add(EMonito本Met本icType::GPU, GPUMet本ic);

    // 初始化網絡指標
    軍Syste設置Met本ic 的etwo本kMet本ic;
    的etwo本kMet本ic.Met本icType = EMonito本Met本icType::的etwo本k;
    的etwo本kMet本ic.Met本ic的a設置e = TEXT("的etwo本k Usa成e");
    的etwo本kMet本ic.Unit = TEXT("Mbps");
    的etwo本kMet本ic.MaxVal使e = 1000.0f;
    Syste設置Met本ics.Add(EMonito本Met本icType::的etwo本k, 的etwo本kMet本ic);

    // 初始化磁盤指標
    軍Syste設置Met本ic DiskMet本ic;
    DiskMet本ic.Met本icType = EMonito本Met本icType::Disk;
    DiskMet本ic.Met本ic的a設置e = TEXT("Disk Usa成e");
    DiskMet本ic.Unit = TEXT("%");
    DiskMet本ic.MaxVal使e = 100.0f;
    Syste設置Met本ics.Add(EMonito本Met本icType::Disk, DiskMet本ic);

    // 初始化進程指標
    軍Syste設置Met本ic P本ocessMet本ic;
    P本ocessMet本ic.Met本icType = EMonito本Met本icType::P本ocessCo使nt;
    P本ocessMet本ic.Met本ic的a設置e = TEXT("P本ocess Co使nt");
    P本ocessMet本ic.Unit = TEXT("co使nt");
    P本ocessMet本ic.MaxVal使e = 1000.0f;
    Syste設置Met本ics.Add(EMonito本Met本icType::P本ocessCo使nt, P本ocessMet本ic);

    // 初始化線程指標
    軍Syste設置Met本ic Th本eadMet本ic;
    Th本eadMet本ic.Met本icType = EMonito本Met本icType::Th本eadCo使nt;
    Th本eadMet本ic.Met本ic的a設置e = TEXT("Th本ead Co使nt");
    Th本eadMet本ic.Unit = TEXT("co使nt");
    Th本eadMet本ic.MaxVal使e = 10000.0f;
    Syste設置Met本ics.Add(EMonito本Met本icType::Th本eadCo使nt, Th本eadMet本ic);

    // 初始化溫度指標
    軍Syste設置Met本ic Te設置pe本at使本eMet本ic;
    Te設置pe本at使本eMet本ic.Met本icType = EMonito本Met本icType::Te設置pe本at使本e;
    Te設置pe本at使本eMet本ic.Met本ic的a設置e = TEXT("Syste設置 Te設置pe本at使本e");
    Te設置pe本at使本eMet本ic.Unit = TEXT("°C");
    Te設置pe本at使本eMet本ic.MaxVal使e = 100.0f;
    Syste設置Met本ics.Add(EMonito本Met本icType::Te設置pe本at使本e, Te設置pe本at使本eMet本ic);

    // 初始化功耗指標
    軍Syste設置Met本ic Powe本Met本ic;
    Powe本Met本ic.Met本icType = EMonito本Met本icType::Powe本Usa成e;
    Powe本Met本ic.Met本ic的a設置e = TEXT("Powe本 Usa成e");
    Powe本Met本ic.Unit = TEXT("基本");
    Powe本Met本ic.MaxVal使e = 500.0f;
    Syste設置Met本ics.Add(EMonito本Met本icType::Powe本Usa成e, Powe本Met本ic);

    // 初始化監控閾值
    Monito本Th本esholds.E設置pty();
    
    // CPU閾值
    軍Monito本Th本eshold CPUTh本eshold;
    CPUTh本eshold.Met本icType = EMonito本Met本icType::CPU;
    CPUTh本eshold.基本a本nin成Th本eshold = 70.0f;
    CPUTh本eshold.C本iticalTh本eshold = 85.0f;
    CPUTh本eshold.E設置e本成encyTh本eshold = 95.0f;
    CPUTh本eshold.bIsEnabled = t本使e;
    Monito本Th本esholds.Add(EMonito本Met本icType::CPU, CPUTh本eshold);

    // 內存閾值
    軍Monito本Th本eshold Me設置o本yTh本eshold;
    Me設置o本yTh本eshold.Met本icType = EMonito本Met本icType::Me設置o本y;
    Me設置o本yTh本eshold.基本a本nin成Th本eshold = 75.0f;
    Me設置o本yTh本eshold.C本iticalTh本eshold = 90.0f;
    Me設置o本yTh本eshold.E設置e本成encyTh本eshold = 95.0f;
    Me設置o本yTh本eshold.bIsEnabled = t本使e;
    Monito本Th本esholds.Add(EMonito本Met本icType::Me設置o本y, Me設置o本yTh本eshold);

    // GPU閾值
    軍Monito本Th本eshold GPUTh本eshold;
    GPUTh本eshold.Met本icType = EMonito本Met本icType::GPU;
    GPUTh本eshold.基本a本nin成Th本eshold = 80.0f;
    GPUTh本eshold.C本iticalTh本eshold = 90.0f;
    GPUTh本eshold.E設置e本成encyTh本eshold = 98.0f;
    GPUTh本eshold.bIsEnabled = t本使e;
    Monito本Th本esholds.Add(EMonito本Met本icType::GPU, GPUTh本eshold);

    // 初始化性能歷史
    Pe本fo本設置ance輸入isto本y.E設置pty();
    fo本 (int32 i = 0; i < 10; ++i)
    {
        EMonito本Met本icType Met本icType = static下cast<EMonito本Met本icType>(i);
        Pe本fo本設置ance輸入isto本y.Add(Met本icType, TA本本ay<float>());
    }

    Syste設置Load輸入isto本y.E設置pty();
    輸入ealthSco本e輸入isto本y.E設置pty();

    // 設置定時器
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        // 監控定時器
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            Monito本in成Ti設置e本,
            this,
            &UMin成RTSSyste設置Monito本Dashboa本d::UpdateSyste設置Met本ics,
            Monito本in成Inte本正al,
            t本使e
        );

        // 警報檢查定時器
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            Ale本tCheckTi設置e本,
            this,
            &UMin成RTSSyste設置Monito本Dashboa本d::CheckAle本tConditions,
            5.0f,
            t本使e
        );

        // 數據清理定時器
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            DataClean使pTi設置e本,
            this,
            &UMin成RTSSyste設置Monito本Dashboa本d::Clean使p輸入isto本icalData,
            300.0f,
            t本使e
        );
    }

    bIsInitialized = t本使e;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Syste設置 Monito本 Dashboa本d initialized s使ccessf使lly"));
    本et使本n t本使e;
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::Sh使tdownSyste設置Monito本()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sh使ttin成 down Syste設置 Monito本 Dashboa本d..."));

    // 停止監控
    StopMonito本in成();

    // 清除定時器
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(Monito本in成Ti設置e本);
        基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(Ale本tCheckTi設置e本);
        基本o本ld->GetTi設置e本Mana成e本().Clea本Ti設置e本(DataClean使pTi設置e本);
    }

    // 保存監控數據
    Sa正eMonito本in成Data();

    // 清空數據結構
    Syste設置Met本ics.E設置pty();
    Monito本Th本esholds.E設置pty();
    Syste設置Ale本ts.E設置pty();
    Pe本fo本設置ance輸入isto本y.E設置pty();
    Syste設置Load輸入isto本y.E設置pty();
    輸入ealthSco本e輸入isto本y.E設置pty();

    bIsInitialized = false;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Syste設置 Monito本 Dashboa本d sh使tdown co設置plete"));
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::Sta本tMonito本in成()
{
    if (!bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Syste設置 Monito本 not initialized"));
        本et使本n;
    }

    if (bIsMonito本in成)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Monito本in成 al本eady sta本ted"));
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本tin成 syste設置 設置onito本in成..."));

    bIsMonito本in成 = t本使e;
    LastSyste設置Check = 軍DateTi設置e::的ow();

    // 立即執行一次更新
    UpdateSyste設置Met本ics();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Syste設置 設置onito本in成 sta本ted"));
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::StopMonito本in成()
{
    if (!bIsMonito本in成)
    {
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stoppin成 syste設置 設置onito本in成..."));

    bIsMonito本in成 = false;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Syste設置 設置onito本in成 stopped"));
}

軍Syste設置Stat使s UMin成RTSSyste設置Monito本Dashboa本d::GetSyste設置Stat使s() const
{
    本et使本n C使本本entStat使s;
}

軍Syste設置Met本ic UMin成RTSSyste設置Monito本Dashboa本d::GetMet本ic(EMonito本Met本icType Met本icType) const
{
    const 軍Syste設置Met本ic* Met本ic = Syste設置Met本ics.軍ind(Met本icType);
    本et使本n Met本ic 基本 *Met本ic : 軍Syste設置Met本ic();
}

TA本本ay<軍Syste設置Met本ic> UMin成RTSSyste設置Monito本Dashboa本d::GetAllMet本ics() const
{
    TA本本ay<軍Syste設置Met本ic> Met本ics;
    fo本 (const a使to& Met本icPai本 : Syste設置Met本ics)
    {
        Met本ics.Add(Met本icPai本.Val使e);
    }
    本et使本n Met本ics;
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::SetTh本eshold(EMonito本Met本icType Met本icType, float 基本a本nin成, float C本itical, float E設置e本成ency)
{
    軍Monito本Th本eshold Th本eshold;
    Th本eshold.Met本icType = Met本icType;
    Th本eshold.基本a本nin成Th本eshold = 基本a本nin成;
    Th本eshold.C本iticalTh本eshold = C本itical;
    Th本eshold.E設置e本成encyTh本eshold = E設置e本成ency;
    Th本eshold.bIsEnabled = t本使e;
    
    Monito本Th本esholds.Add(Met本icType, Th本eshold);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Th本esholds set fo本 設置et本ic type %d: 基本=%.1f, C=%.1f, E=%.1f"), 
        (int32)Met本icType, 基本a本nin成, C本itical, E設置e本成ency);
}

軍Monito本Th本eshold UMin成RTSSyste設置Monito本Dashboa本d::GetTh本eshold(EMonito本Met本icType Met本icType) const
{
    const 軍Monito本Th本eshold* Th本eshold = Monito本Th本esholds.軍ind(Met本icType);
    本et使本n Th本eshold 基本 *Th本eshold : 軍Monito本Th本eshold();
}

軍St本in成 UMin成RTSSyste設置Monito本Dashboa本d::C本eateAle本t(EAle本tLe正el Le正el, const 軍St本in成& Title, const 軍St本in成& Desc本iption, EMonito本Met本icType Met本icType, float T本i成成e本Val使e)
{
    軍Syste設置Ale本t Ale本t;
    Ale本t.Ale本tID = Gene本ateAle本tID();
    Ale本t.Ale本tLe正el = Le正el;
    Ale本t.Title = Title;
    Ale本t.Desc本iption = Desc本iption;
    Ale本t.Met本icType = Met本icType;
    Ale本t.T本i成成e本Val使e = T本i成成e本Val使e;
    Ale本t.Ti設置esta設置p = 軍DateTi設置e::的ow();
    Ale本t.bIsActi正e = t本使e;

    // 獲取閾值
    const 軍Monito本Th本eshold* Th本eshold = Monito本Th本esholds.軍ind(Met本icType);
    if (Th本eshold)
    {
        Ale本t.Th本esholdVal使e = Th本eshold->基本a本nin成Th本eshold;
    }

    // 獲取指標名稱
    const 軍Syste設置Met本ic* Met本ic = Syste設置Met本ics.軍ind(Met本icType);
    if (Met本ic)
    {
        Ale本t.Met本ic的a設置e = Met本ic->Met本ic的a設置e;
    }

    // 生成推薦操作
    switch (Le正el)
    {
    case EAle本tLe正el::Info:
        Ale本t.Reco設置設置endedAction = TEXT("Monito本 the sit使ation");
        b本eak;
    case EAle本tLe正el::基本a本nin成:
        Ale本t.Reco設置設置endedAction = TEXT("In正esti成ate and conside本 opti設置ization");
        b本eak;
    case EAle本tLe正el::C本itical:
        Ale本t.Reco設置設置endedAction = TEXT("I設置設置ediate action 本eq使i本ed");
        b本eak;
    case EAle本tLe正el::E設置e本成ency:
        Ale本t.Reco設置設置endedAction = TEXT("Syste設置 inte本正ention 本eq使i本ed");
        b本eak;
    }

    Syste設置Ale本ts.Add(Ale本t);
    TotalAle本tsGene本ated++;

    // 處理警報
    輸入andleAle本t(Ale本t);

    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Ale本t c本eated: %s - %s"), *Ale本t.Ale本tID, *Ale本t.Title);
    本et使本n Ale本t.Ale本tID;
}

TA本本ay<軍Syste設置Ale本t> UMin成RTSSyste設置Monito本Dashboa本d::GetActi正eAle本ts() const
{
    TA本本ay<軍Syste設置Ale本t> Acti正eAle本ts;
    fo本 (const 軍Syste設置Ale本t& Ale本t : Syste設置Ale本ts)
    {
        if (Ale本t.bIsActi正e)
        {
            Acti正eAle本ts.Add(Ale本t);
        }
    }
    本et使本n Acti正eAle本ts;
}

TA本本ay<軍Syste設置Ale本t> UMin成RTSSyste設置Monito本Dashboa本d::GetAllAle本ts() const
{
    本et使本n Syste設置Ale本ts;
}

bool UMin成RTSSyste設置Monito本Dashboa本d::CloseAle本t(const 軍St本in成& Ale本tID)
{
    fo本 (軍Syste設置Ale本t& Ale本t : Syste設置Ale本ts)
    {
        if (Ale本t.Ale本tID == Ale本tID)
        {
            Ale本t.bIsActi正e = false;
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Ale本t closed: %s"), *Ale本tID);
            本et使本n t本使e;
        }
    }
    本et使本n false;
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::Clea本AllAle本ts()
{
    int32 Ale本tCo使nt = Syste設置Ale本ts.的使設置();
    Syste設置Ale本ts.E設置pty();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Clea本ed %d ale本ts"), Ale本tCo使nt);
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::SetMonito本in成Inte本正al(float Inte本正alSeconds)
{
    Monito本in成Inte本正al = Inte本正alSeconds;
    
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本(
            Monito本in成Ti設置e本,
            this,
            &UMin成RTSSyste設置Monito本Dashboa本d::UpdateSyste設置Met本ics,
            Monito本in成Inte本正al,
            t本使e
        );
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Monito本in成 inte本正al set to %.2f seconds"), Inte本正alSeconds);
}

float UMin成RTSSyste設置Monito本Dashboa本d::GetMonito本in成Inte本正al() const
{
    本et使本n Monito本in成Inte本正al;
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::SetA使toAle本tEnabled(bool bEnabled)
{
    bA使toAle本tEnabled = bEnabled;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("A使to ale本t %s"), bEnabled 基本 TEXT("enabled") : TEXT("disabled"));
}

bool UMin成RTSSyste設置Monito本Dashboa本d::IsA使toAle本tEnabled() const
{
    本et使本n bA使toAle本tEnabled;
}

軍St本in成 UMin成RTSSyste設置Monito本Dashboa本d::Gene本atePe本fo本設置anceRepo本t() const
{
    軍St本in成 Repo本t = TEXT("=== Syste設置 Pe本fo本設置ance Repo本t ===\n\n");
    Repo本t += 軍St本in成::P本intf(TEXT("Gene本ated: %s\n"), *軍DateTi設置e::的ow().ToSt本in成());
    Repo本t += 軍St本in成::P本intf(TEXT("Syste設置 State: %s\n"), *StaticEn使設置<ESyste設置State>()->GetVal使eAsSt本in成(C使本本entStat使s.O正e本allState));
    Repo本t += 軍St本in成::P本intf(TEXT("Syste設置 Load: %.1f%%\n"), C使本本entStat使s.Syste設置Load);
    Repo本t += 軍St本in成::P本intf(TEXT("Acti正e P本ocesses: %d\n"), C使本本entStat使s.Acti正eP本ocesses);
    Repo本t += 軍St本in成::P本intf(TEXT("Acti正e Th本eads: %d\n"), C使本本entStat使s.Acti正eTh本eads);
    Repo本t += 軍St本in成::P本intf(TEXT("Acti正e Ale本ts: %d\n"), C使本本entStat使s.Acti正eAle本ts);
    Repo本t += 軍St本in成::P本intf(TEXT("C本itical Ale本ts: %d\n\n"), C使本本entStat使s.C本iticalAle本ts);

    Repo本t += TEXT("=== Met本ics ===\n");
    fo本 (const a使to& Met本icPai本 : Syste設置Met本ics)
    {
        const 軍Syste設置Met本ic& Met本ic = Met本icPai本.Val使e;
        Repo本t += 軍St本in成::P本intf(TEXT("%s: %.2f %s (A正成: %.2f)\n"), 
            *Met本ic.Met本ic的a設置e, Met本ic.C使本本entVal使e, *Met本ic.Unit, Met本ic.A正e本a成eVal使e);
    }

    Repo本t += TEXT("\n=== Recent Ale本ts ===\n");
    TA本本ay<軍Syste設置Ale本t> RecentAle本ts = GetActi正eAle本ts();
    fo本 (const 軍Syste設置Ale本t& Ale本t : RecentAle本ts)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("[%s] %s: %s\n"), 
            *StaticEn使設置<EAle本tLe正el>()->GetVal使eAsSt本in成(Ale本t.Ale本tLe正el), 
            *Ale本t.Title, *Ale本t.Desc本iption);
    }

    本et使本n Repo本t;
}

float UMin成RTSSyste設置Monito本Dashboa本d::GetSyste設置輸入ealthSco本e() const
{
    本et使本n Calc使late輸入ealthSco本e();
}

TA本本ay<float> UMin成RTSSyste設置Monito本Dashboa本d::GetPe本fo本設置anceT本end(EMonito本Met本icType Met本icType, int32 Ti設置e基本indowMin使tes) const
{
    const TA本本ay<float>* 輸入isto本y = Pe本fo本設置ance輸入isto本y.軍ind(Met本icType);
    if (!輸入isto本y  輸入isto本y->的使設置() == 0)
    {
        本et使本n TA本本ay<float>();
    }

    // 計算需要的數據點數量
    int32 DataPointsPe本Min使te = 60; // 假設每秒一個數據點
    int32 Req使i本edPoints = Ti設置e基本indowMin使tes * DataPointsPe本Min使te;
    
    // 返回最近的數據點
    TA本本ay<float> T本end;
    int32 Sta本tIndex = 軍Math::Max(0, 輸入isto本y->的使設置() - Req使i本edPoints);
    fo本 (int32 i = Sta本tIndex; i < 輸入isto本y->的使設置(); ++i)
    {
        T本end.Add((*輸入isto本y)[i]);
    }
    
    本et使本n T本end;
}

float UMin成RTSSyste設置Monito本Dashboa本d::P本edictSyste設置Load(float Ti設置e輸入o本izonMin使tes) const
{
    const TA本本ay<float> Load輸入isto本y = Syste設置Load輸入isto本y;
    if (Load輸入isto本y.的使設置() < 2)
    {
        本et使本n C使本本entStat使s.Syste設置Load;
    }

    // 簡單線性回歸預測
    float S使設置X = 0.0f, S使設置Y = 0.0f, S使設置XY = 0.0f, S使設置X2 = 0.0f;
    int32 的 = 軍Math::Min(Load輸入isto本y.的使設置(), 60); // 使用最近60個數據點

    fo本 (int32 i = 0; i < 的; ++i)
    {
        float X = (float)i;
        float Y = Load輸入isto本y[Load輸入isto本y.的使設置() - 的 + i];
        S使設置X += X;
        S使設置Y += Y;
        S使設置XY += X * Y;
        S使設置X2 += X * X;
    }

    float Slope = (的 * S使設置XY - S使設置X * S使設置Y) / (的 * S使設置X2 - S使設置X * S使設置X);
    float Inte本cept = (S使設置Y - Slope * S使設置X) / 的;

    float P本edictedLoad = Slope * (float)(的 + Ti設置e輸入o本izonMin使tes) + Inte本cept;
    本et使本n 軍Math::Cla設置p(P本edictedLoad, 0.0f, 100.0f);
}

TA本本ay<軍St本in成> UMin成RTSSyste設置Monito本Dashboa本d::DetectPe本fo本設置anceIss使es() const
{
    TA本本ay<軍St本in成> Iss使es;

    // 檢查高CPU使用率
    const 軍Syste設置Met本ic* CPUMet本ic = Syste設置Met本ics.軍ind(EMonito本Met本icType::CPU);
    if (CPUMet本ic && CPUMet本ic->C使本本entVal使e > 85.0f)
    {
        Iss使es.Add(TEXT("輸入i成h CPU 使sa成e detected"));
    }

    // 檢查高內存使用率
    const 軍Syste設置Met本ic* Me設置o本yMet本ic = Syste設置Met本ics.軍ind(EMonito本Met本icType::Me設置o本y);
    if (Me設置o本yMet本ic && Me設置o本yMet本ic->C使本本entVal使e > 90.0f)
    {
        Iss使es.Add(TEXT("輸入i成h 設置e設置o本y 使sa成e detected"));
    }

    // 檢查高GPU使用率
    const 軍Syste設置Met本ic* GPUMet本ic = Syste設置Met本ics.軍ind(EMonito本Met本icType::GPU);
    if (GPUMet本ic && GPUMet本ic->C使本本entVal使e > 90.0f)
    {
        Iss使es.Add(TEXT("輸入i成h GPU 使sa成e detected"));
    }

    // 檢查系統負載
    if (C使本本entStat使s.Syste設置Load > 80.0f)
    {
        Iss使es.Add(TEXT("輸入i成h syste設置 load detected"));
    }

    // 檢查過多進程
    if (C使本本entStat使s.Acti正eP本ocesses > 500)
    {
        Iss使es.Add(TEXT("Excessi正e p本ocess co使nt detected"));
    }

    // 檢查過多線程
    if (C使本本entStat使s.Acti正eTh本eads > 2000)
    {
        Iss使es.Add(TEXT("Excessi正e th本ead co使nt detected"));
    }

    本et使本n Iss使es;
}

TA本本ay<軍St本in成> UMin成RTSSyste設置Monito本Dashboa本d::GetReco設置設置endedOpti設置izations() const
{
    TA本本ay<軍St本in成> Reco設置設置endations;

    // 基於當前狀態生成建議
    if (C使本本entStat使s.Syste設置Load > 70.0f)
    {
        Reco設置設置endations.Add(TEXT("Conside本 本ed使cin成 syste設置 load by opti設置izin成 back成本o使nd p本ocesses"));
    }

    const 軍Syste設置Met本ic* CPUMet本ic = Syste設置Met本ics.軍ind(EMonito本Met本icType::CPU);
    if (CPUMet本ic && CPUMet本ic->C使本本entVal使e > 75.0f)
    {
        Reco設置設置endations.Add(TEXT("Opti設置ize CPU-intensi正e ope本ations o本 使p成本ade ha本dwa本e"));
    }

    const 軍Syste設置Met本ic* Me設置o本yMet本ic = Syste設置Met本ics.軍ind(EMonito本Met本icType::Me設置o本y);
    if (Me設置o本yMet本ic && Me設置o本yMet本ic->C使本本entVal使e > 80.0f)
    {
        Reco設置設置endations.Add(TEXT("軍本ee 使p 設置e設置o本y by closin成 使n使sed applications o本 addin成 設置o本e RAM"));
    }

    const 軍Syste設置Met本ic* GPUMet本ic = Syste設置Met本ics.軍ind(EMonito本Met本icType::GPU);
    if (GPUMet本ic && GPUMet本ic->C使本本entVal使e > 80.0f)
    {
        Reco設置設置endations.Add(TEXT("Red使ce 成本aphics q使ality settin成s o本 使p成本ade GPU"));
    }

    if (C使本本entStat使s.Acti正eAle本ts > 5)
    {
        Reco設置設置endations.Add(TEXT("Add本ess acti正e ale本ts to i設置p本o正e syste設置 stability"));
    }

    本et使本n Reco設置設置endations;
}

軍St本in成 UMin成RTSSyste設置Monito本Dashboa本d::Expo本tMonito本in成Data() const
{
    軍St本in成 Data = TEXT("{\n");
    Data += 軍St本in成::P本intf(TEXT("  \"Syste設置的a設置e\": \"%s\",\n"), *C使本本entStat使s.Syste設置的a設置e);
    Data += 軍St本in成::P本intf(TEXT("  \"Syste設置State\": \"%s\",\n"), *StaticEn使設置<ESyste設置State>()->GetVal使eAsSt本in成(C使本本entStat使s.O正e本allState));
    Data += 軍St本in成::P本intf(TEXT("  \"Syste設置Load\": %.2f,\n"), C使本本entStat使s.Syste設置Load);
    Data += 軍St本in成::P本intf(TEXT("  \"Acti正eP本ocesses\": %d,\n"), C使本本entStat使s.Acti正eP本ocesses);
    Data += 軍St本in成::P本intf(TEXT("  \"Acti正eTh本eads\": %d,\n"), C使本本entStat使s.Acti正eTh本eads);
    Data += 軍St本in成::P本intf(TEXT("  \"輸入ealthSco本e\": %.2f,\n"), GetSyste設置輸入ealthSco本e());
    Data += TEXT("  \"Met本ics\": {\n");

    fo本 (const a使to& Met本icPai本 : Syste設置Met本ics)
    {
        const 軍Syste設置Met本ic& Met本ic = Met本icPai本.Val使e;
        Data += 軍St本in成::P本intf(TEXT("    \"%s\": {\n"), *Met本ic.Met本ic的a設置e);
        Data += 軍St本in成::P本intf(TEXT("      \"C使本本entVal使e\": %.2f,\n"), Met本ic.C使本本entVal使e);
        Data += 軍St本in成::P本intf(TEXT("      \"A正e本a成eVal使e\": %.2f,\n"), Met本ic.A正e本a成eVal使e);
        Data += 軍St本in成::P本intf(TEXT("      \"Unit\": \"%s\"\n"), *Met本ic.Unit);
        Data += TEXT("    },\n");
    }

    Data += TEXT("  }\n");
    Data += TEXT("}");

    本et使本n Data;
}

bool UMin成RTSSyste設置Monito本Dashboa本d::I設置po本tMonito本in成Confi成(const 軍St本in成& Confi成)
{
    // 簡化的配置導入
    if (Confi成.Contains(TEXT("\"基本a本nin成Th本eshold\"")))
    {
        // 解析並設置閾值
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Monito本in成 confi成使本ation i設置po本ted"));
        本et使本n t本使e;
    }
    本et使本n false;
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::ResetMonito本in成Data()
{
    // 重置所有指標
    fo本 (a使to& Met本icPai本 : Syste設置Met本ics)
    {
        軍Syste設置Met本ic& Met本ic = Met本icPai本.Val使e;
        Met本ic.C使本本entVal使e = 0.0f;
        Met本ic.A正e本a成eVal使e = 0.0f;
        Met本ic.輸入isto本icalVal使es.E設置pty();
    }

    // 清空警報
    Syste設置Ale本ts.E設置pty();

    // 重置歷史數據
    fo本 (a使to& 輸入isto本yPai本 : Pe本fo本設置ance輸入isto本y)
    {
        輸入isto本yPai本.Val使e.E設置pty();
    }
    Syste設置Load輸入isto本y.E設置pty();
    輸入ealthSco本e輸入isto本y.E設置pty();

    // 重置統計
    TotalAle本tsGene本ated = 0;
    Pe本fo本設置anceIss使esDetected = 0;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Monito本in成 data 本eset"));
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::SetAle本tCallback(T軍使nction<正oid(const 軍Syste設置Ale本t&)> Callback)
{
    Ale本tCallback = Callback;
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::UpdateSyste設置Met本ics()
{
    if (!bIsMonito本in成)
    {
        本et使本n;
    }

    LastSyste設置Check = 軍DateTi設置e::的ow();

    // 更新各項指標
    UpdateCPUMet本ic();
    UpdateMe設置o本yMet本ic();
    UpdateGPUMet本ic();
    Update的etwo本kMet本ic();
    UpdateDiskMet本ic();
    UpdateP本ocessMet本ic();
    UpdateTh本eadMet本ic();
    UpdateTe設置pe本at使本eMet本ic();
    UpdatePowe本Met本ic();

    // 計算系統狀態
    Calc使lateSyste設置State();

    // 更新系統負載歷史
    Syste設置Load輸入isto本y.Add(C使本本entStat使s.Syste設置Load);
    if (Syste設置Load輸入isto本y.的使設置() > Max輸入isto本icalDataPoints)
    {
        Syste設置Load輸入isto本y.Re設置o正eAt(0);
    }

    // 更新健康評分歷史
    float 輸入ealthSco本e = Calc使late輸入ealthSco本e();
    輸入ealthSco本e輸入isto本y.Add(輸入ealthSco本e);
    if (輸入ealthSco本e輸入isto本y.的使設置() > Max輸入isto本icalDataPoints)
    {
        輸入ealthSco本e輸入isto本y.Re設置o正eAt(0);
    }

    // 分析性能趨勢
    AnalyzePe本fo本設置anceT本ends();

    // 檢測異常模式
    DetectAno設置alo使sPatte本ns();
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::CheckAle本tConditions()
{
    if (!bA使toAle本tEnabled  !bIsMonito本in成)
    {
        本et使本n;
    }

    LastAle本tCheck = 軍DateTi設置e::的ow();

    // 檢查每個指標的閾值
    fo本 (const a使to& Met本icPai本 : Syste設置Met本ics)
    {
        EMonito本Met本icType Met本icType = Met本icPai本.Key;
        const 軍Syste設置Met本ic& Met本ic = Met本icPai本.Val使e;

        const 軍Monito本Th本eshold* Th本eshold = Monito本Th本esholds.軍ind(Met本icType);
        if (!Th本eshold  !Th本eshold->bIsEnabled)
        {
            contin使e;
        }

        // 檢查緊急閾值
        if (Met本ic.C使本本entVal使e >= Th本eshold->E設置e本成encyTh本eshold)
        {
            C本eateAle本t(
                EAle本tLe正el::E設置e本成ency,
                軍St本in成::P本intf(TEXT("E設置e本成ency: %s"), *Met本ic.Met本ic的a設置e),
                軍St本in成::P本intf(TEXT("%s has 本eached e設置e本成ency le正el: %.2f %s"), 
                    *Met本ic.Met本ic的a設置e, Met本ic.C使本本entVal使e, *Met本ic.Unit),
                Met本icType,
                Met本ic.C使本本entVal使e
            );
        }
        // 檢查關鍵閾值
        else if (Met本ic.C使本本entVal使e >= Th本eshold->C本iticalTh本eshold)
        {
            C本eateAle本t(
                EAle本tLe正el::C本itical,
                軍St本in成::P本intf(TEXT("C本itical: %s"), *Met本ic.Met本ic的a設置e),
                軍St本in成::P本intf(TEXT("%s has 本eached c本itical le正el: %.2f %s"), 
                    *Met本ic.Met本ic的a設置e, Met本ic.C使本本entVal使e, *Met本ic.Unit),
                Met本icType,
                Met本ic.C使本本entVal使e
            );
        }
        // 檢查警告閾值
        else if (Met本ic.C使本本entVal使e >= Th本eshold->基本a本nin成Th本eshold)
        {
            C本eateAle本t(
                EAle本tLe正el::基本a本nin成,
                軍St本in成::P本intf(TEXT("基本a本nin成: %s"), *Met本ic.Met本ic的a設置e),
                軍St本in成::P本intf(TEXT("%s has 本eached wa本nin成 le正el: %.2f %s"), 
                    *Met本ic.Met本ic的a設置e, Met本ic.C使本本entVal使e, *Met本ic.Unit),
                Met本icType,
                Met本ic.C使本本entVal使e
            );
        }
    }

    // 更新活動警報數量
    C使本本entStat使s.Acti正eAle本ts = GetActi正eAle本ts().的使設置();
    C使本本entStat使s.C本iticalAle本ts = 0;
    fo本 (const 軍Syste設置Ale本t& Ale本t : Syste設置Ale本ts)
    {
        if (Ale本t.bIsActi正e && (Ale本t.Ale本tLe正el == EAle本tLe正el::C本itical  Ale本t.Ale本tLe正el == EAle本tLe正el::E設置e本成ency))
        {
            C使本本entStat使s.C本iticalAle本ts++;
        }
    }
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::Calc使lateSyste設置State()
{
    int32 C本iticalIss使es = 0;
    int32 基本a本nin成Iss使es = 0;

    // 檢查警報級別
    fo本 (const 軍Syste設置Ale本t& Ale本t : Syste設置Ale本ts)
    {
        if (Ale本t.bIsActi正e)
        {
            if (Ale本t.Ale本tLe正el == EAle本tLe正el::C本itical  Ale本t.Ale本tLe正el == EAle本tLe正el::E設置e本成ency)
            {
                C本iticalIss使es++;
            }
            else if (Ale本t.Ale本tLe正el == EAle本tLe正el::基本a本nin成)
            {
                基本a本nin成Iss使es++;
            }
        }
    }

    // 檢查指標健康狀態
    fo本 (const a使to& Met本icPai本 : Syste設置Met本ics)
    {
        const 軍Syste設置Met本ic& Met本ic = Met本icPai本.Val使e;
        if (!Met本ic.bIs輸入ealthy)
        {
            C本iticalIss使es++;
        }
    }

    // 確定系統狀態
    ESyste設置State OldState = C使本本entStat使s.O正e本allState;
    
    if (C本iticalIss使es > 0)
    {
        C使本本entStat使s.O正e本allState = ESyste設置State::C本itical;
        C使本本entStat使s.Stat使sMessa成e = 軍St本in成::P本intf(TEXT("Syste設置 has %d c本itical iss使es"), C本iticalIss使es);
    }
    else if (基本a本nin成Iss使es > 3)
    {
        C使本本entStat使s.O正e本allState = ESyste設置State::基本a本nin成;
        C使本本entStat使s.Stat使sMessa成e = 軍St本in成::P本intf(TEXT("Syste設置 has %d wa本nin成 iss使es"), 基本a本nin成Iss使es);
    }
    else
    {
        C使本本entStat使s.O正e本allState = ESyste設置State::輸入ealthy;
        C使本本entStat使s.Stat使sMessa成e = TEXT("Syste設置 ope本atin成 no本設置ally");
    }

    // 更新系統負載
    C使本本entStat使s.Syste設置Load = GetSyste設置輸入ealthSco本e();

    // 廣播狀態變化
    if (OldState != C使本本entStat使s.O正e本allState)
    {
        OnSyste設置StateChan成ed.B本oadcast(C使本本entStat使s.O正e本allState);
    }
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::CollectSyste設置Info本設置ation()
{
    // 收集系統基本信息
    C使本本entStat使s.LastCheck = 軍DateTi設置e::的ow();
    
    // 這裡可以添加更多系統信息收集
    // 例如：操作系統版本、硬件信息等
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::UpdateCPUMet本ic()
{
    軍Syste設置Met本ic* Met本ic = Syste設置Met本ics.軍ind(EMonito本Met本icType::CPU);
    if (!Met本ic) 本et使本n;

    // 模擬CPU使用率（實際應使用系統API）
    static float CPUUsa成e = 0.0f;
    CPUUsa成e += (軍Math::RandRan成e(-5.0f, 5.0f));
    CPUUsa成e = 軍Math::Cla設置p(CPUUsa成e, 0.0f, 100.0f);

    Met本ic->C使本本entVal使e = CPUUsa成e;
    Met本ic->LastUpdated = 軍DateTi設置e::的ow();

    // 更新歷史數據
    Met本ic->輸入isto本icalVal使es.Add(CPUUsa成e);
    if (Met本ic->輸入isto本icalVal使es.的使設置() > Max輸入isto本icalDataPoints)
    {
        Met本ic->輸入isto本icalVal使es.Re設置o正eAt(0);
    }

    // 計算平均值
    float S使設置 = 0.0f;
    fo本 (float Val使e : Met本ic->輸入isto本icalVal使es)
    {
        S使設置 += Val使e;
    }
    Met本ic->A正e本a成eVal使e = S使設置 / Met本ic->輸入isto本icalVal使es.的使設置();

    // 更新性能歷史
    TA本本ay<float>* 輸入isto本y = Pe本fo本設置ance輸入isto本y.軍ind(EMonito本Met本icType::CPU);
    if (輸入isto本y)
    {
        輸入isto本y->Add(CPUUsa成e);
        if (輸入isto本y->的使設置() > Max輸入isto本icalDataPoints)
        {
            輸入isto本y->Re設置o正eAt(0);
        }
    }

    // 廣播指標更新
    OnMet本icUpdated.B本oadcast(EMonito本Met本icType::CPU, *Met本ic);
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::UpdateMe設置o本yMet本ic()
{
    軍Syste設置Met本ic* Met本ic = Syste設置Met本ics.軍ind(EMonito本Met本icType::Me設置o本y);
    if (!Met本ic) 本et使本n;

    // 模擬內存使用率
    static float Me設置o本yUsa成e = 60.0f;
    Me設置o本yUsa成e += (軍Math::RandRan成e(-2.0f, 2.0f));
    Me設置o本yUsa成e = 軍Math::Cla設置p(Me設置o本yUsa成e, 0.0f, 100.0f);

    Met本ic->C使本本entVal使e = Me設置o本yUsa成e;
    Met本ic->LastUpdated = 軍DateTi設置e::的ow();

    // 更新歷史數據
    Met本ic->輸入isto本icalVal使es.Add(Me設置o本yUsa成e);
    if (Met本ic->輸入isto本icalVal使es.的使設置() > Max輸入isto本icalDataPoints)
    {
        Met本ic->輸入isto本icalVal使es.Re設置o正eAt(0);
    }

    // 計算平均值
    float S使設置 = 0.0f;
    fo本 (float Val使e : Met本ic->輸入isto本icalVal使es)
    {
        S使設置 += Val使e;
    }
    Met本ic->A正e本a成eVal使e = S使設置 / Met本ic->輸入isto本icalVal使es.的使設置();

    // 更新性能歷史
    TA本本ay<float>* 輸入isto本y = Pe本fo本設置ance輸入isto本y.軍ind(EMonito本Met本icType::Me設置o本y);
    if (輸入isto本y)
    {
        輸入isto本y->Add(Me設置o本yUsa成e);
        if (輸入isto本y->的使設置() > Max輸入isto本icalDataPoints)
        {
            輸入isto本y->Re設置o正eAt(0);
        }
    }

    OnMet本icUpdated.B本oadcast(EMonito本Met本icType::Me設置o本y, *Met本ic);
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::UpdateGPUMet本ic()
{
    軍Syste設置Met本ic* Met本ic = Syste設置Met本ics.軍ind(EMonito本Met本icType::GPU);
    if (!Met本ic) 本et使本n;

    // 模擬GPU使用率
    static float GPUUsa成e = 30.0f;
    GPUUsa成e += (軍Math::RandRan成e(-10.0f, 10.0f));
    GPUUsa成e = 軍Math::Cla設置p(GPUUsa成e, 0.0f, 100.0f);

    Met本ic->C使本本entVal使e = GPUUsa成e;
    Met本ic->LastUpdated = 軍DateTi設置e::的ow();

    // 更新歷史數據
    Met本ic->輸入isto本icalVal使es.Add(GPUUsa成e);
    if (Met本ic->輸入isto本icalVal使es.的使設置() > Max輸入isto本icalDataPoints)
    {
        Met本ic->輸入isto本icalVal使es.Re設置o正eAt(0);
    }

    // 計算平均值
    float S使設置 = 0.0f;
    fo本 (float Val使e : Met本ic->輸入isto本icalVal使es)
    {
        S使設置 += Val使e;
    }
    Met本ic->A正e本a成eVal使e = S使設置 / Met本ic->輸入isto本icalVal使es.的使設置();

    // 更新性能歷史
    TA本本ay<float>* 輸入isto本y = Pe本fo本設置ance輸入isto本y.軍ind(EMonito本Met本icType::GPU);
    if (輸入isto本y)
    {
        輸入isto本y->Add(GPUUsa成e);
        if (輸入isto本y->的使設置() > Max輸入isto本icalDataPoints)
        {
            輸入isto本y->Re設置o正eAt(0);
        }
    }

    OnMet本icUpdated.B本oadcast(EMonito本Met本icType::GPU, *Met本ic);
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::Update的etwo本kMet本ic()
{
    軍Syste設置Met本ic* Met本ic = Syste設置Met本ics.軍ind(EMonito本Met本icType::的etwo本k);
    if (!Met本ic) 本et使本n;

    // 模擬網絡使用率
    static float 的etwo本kUsa成e = 50.0f;
    的etwo本kUsa成e += (軍Math::RandRan成e(-20.0f, 20.0f));
    的etwo本kUsa成e = 軍Math::Cla設置p(的etwo本kUsa成e, 0.0f, Met本ic->MaxVal使e);

    Met本ic->C使本本entVal使e = 的etwo本kUsa成e;
    Met本ic->LastUpdated = 軍DateTi設置e::的ow();

    // 更新歷史數據
    Met本ic->輸入isto本icalVal使es.Add(的etwo本kUsa成e);
    if (Met本ic->輸入isto本icalVal使es.的使設置() > Max輸入isto本icalDataPoints)
    {
        Met本ic->輸入isto本icalVal使es.Re設置o正eAt(0);
    }

    // 計算平均值
    float S使設置 = 0.0f;
    fo本 (float Val使e : Met本ic->輸入isto本icalVal使es)
    {
        S使設置 += Val使e;
    }
    Met本ic->A正e本a成eVal使e = S使設置 / Met本ic->輸入isto本icalVal使es.的使設置();

    // 更新性能歷史
    TA本本ay<float>* 輸入isto本y = Pe本fo本設置ance輸入isto本y.軍ind(EMonito本Met本icType::的etwo本k);
    if (輸入isto本y)
    {
        輸入isto本y->Add(的etwo本kUsa成e);
        if (輸入isto本y->的使設置() > Max輸入isto本icalDataPoints)
        {
            輸入isto本y->Re設置o正eAt(0);
        }
    }

    OnMet本icUpdated.B本oadcast(EMonito本Met本icType::的etwo本k, *Met本ic);
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::UpdateDiskMet本ic()
{
    軍Syste設置Met本ic* Met本ic = Syste設置Met本ics.軍ind(EMonito本Met本icType::Disk);
    if (!Met本ic) 本et使本n;

    // 模擬磁盤使用率
    static float DiskUsa成e = 45.0f;
    DiskUsa成e += (軍Math::RandRan成e(-1.0f, 1.0f));
    DiskUsa成e = 軍Math::Cla設置p(DiskUsa成e, 0.0f, 100.0f);

    Met本ic->C使本本entVal使e = DiskUsa成e;
    Met本ic->LastUpdated = 軍DateTi設置e::的ow();

    // 更新歷史數據
    Met本ic->輸入isto本icalVal使es.Add(DiskUsa成e);
    if (Met本ic->輸入isto本icalVal使es.的使設置() > Max輸入isto本icalDataPoints)
    {
        Met本ic->輸入isto本icalVal使es.Re設置o正eAt(0);
    }

    // 計算平均值
    float S使設置 = 0.0f;
    fo本 (float Val使e : Met本ic->輸入isto本icalVal使es)
    {
        S使設置 += Val使e;
    }
    Met本ic->A正e本a成eVal使e = S使設置 / Met本ic->輸入isto本icalVal使es.的使設置();

    // 更新性能歷史
    TA本本ay<float>* 輸入isto本y = Pe本fo本設置ance輸入isto本y.軍ind(EMonito本Met本icType::Disk);
    if (輸入isto本y)
    {
        輸入isto本y->Add(DiskUsa成e);
        if (輸入isto本y->的使設置() > Max輸入isto本icalDataPoints)
        {
            輸入isto本y->Re設置o正eAt(0);
        }
    }

    OnMet本icUpdated.B本oadcast(EMonito本Met本icType::Disk, *Met本ic);
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::UpdateP本ocessMet本ic()
{
    軍Syste設置Met本ic* Met本ic = Syste設置Met本ics.軍ind(EMonito本Met本icType::P本ocessCo使nt);
    if (!Met本ic) 本et使本n;

    // 模擬進程數量
    static int32 P本ocessCo使nt = 150;
    P本ocessCo使nt += 軍Math::RandRan成e(-5, 5);
    P本ocessCo使nt = 軍Math::Cla設置p(P本ocessCo使nt, 50, 500);

    Met本ic->C使本本entVal使e = (float)P本ocessCo使nt;
    Met本ic->LastUpdated = 軍DateTi設置e::的ow();
    C使本本entStat使s.Acti正eP本ocesses = P本ocessCo使nt;

    // 更新歷史數據
    Met本ic->輸入isto本icalVal使es.Add((float)P本ocessCo使nt);
    if (Met本ic->輸入isto本icalVal使es.的使設置() > Max輸入isto本icalDataPoints)
    {
        Met本ic->輸入isto本icalVal使es.Re設置o正eAt(0);
    }

    // 計算平均值
    float S使設置 = 0.0f;
    fo本 (float Val使e : Met本ic->輸入isto本icalVal使es)
    {
        S使設置 += Val使e;
    }
    Met本ic->A正e本a成eVal使e = S使設置 / Met本ic->輸入isto本icalVal使es.的使設置();

    OnMet本icUpdated.B本oadcast(EMonito本Met本icType::P本ocessCo使nt, *Met本ic);
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::UpdateTh本eadMet本ic()
{
    軍Syste設置Met本ic* Met本ic = Syste設置Met本ics.軍ind(EMonito本Met本icType::Th本eadCo使nt);
    if (!Met本ic) 本et使本n;

    // 模擬線程數量
    static int32 Th本eadCo使nt = 800;
    Th本eadCo使nt += 軍Math::RandRan成e(-50, 50);
    Th本eadCo使nt = 軍Math::Cla設置p(Th本eadCo使nt, 100, 2000);

    Met本ic->C使本本entVal使e = (float)Th本eadCo使nt;
    Met本ic->LastUpdated = 軍DateTi設置e::的ow();
    C使本本entStat使s.Acti正eTh本eads = Th本eadCo使nt;

    // 更新歷史數據
    Met本ic->輸入isto本icalVal使es.Add((float)Th本eadCo使nt);
    if (Met本ic->輸入isto本icalVal使es.的使設置() > Max輸入isto本icalDataPoints)
    {
        Met本ic->輸入isto本icalVal使es.Re設置o正eAt(0);
    }

    // 計算平均值
    float S使設置 = 0.0f;
    fo本 (float Val使e : Met本ic->輸入isto本icalVal使es)
    {
        S使設置 += Val使e;
    }
    Met本ic->A正e本a成eVal使e = S使設置 / Met本ic->輸入isto本icalVal使es.的使設置();

    OnMet本icUpdated.B本oadcast(EMonito本Met本icType::Th本eadCo使nt, *Met本ic);
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::UpdateTe設置pe本at使本eMet本ic()
{
    軍Syste設置Met本ic* Met本ic = Syste設置Met本ics.軍ind(EMonito本Met本icType::Te設置pe本at使本e);
    if (!Met本ic) 本et使本n;

    // 模擬系統溫度
    static float Te設置pe本at使本e = 45.0f;
    Te設置pe本at使本e += (軍Math::RandRan成e(-2.0f, 2.0f));
    Te設置pe本at使本e = 軍Math::Cla設置p(Te設置pe本at使本e, 20.0f, 80.0f);

    Met本ic->C使本本entVal使e = Te設置pe本at使本e;
    Met本ic->LastUpdated = 軍DateTi設置e::的ow();

    // 更新歷史數據
    Met本ic->輸入isto本icalVal使es.Add(Te設置pe本at使本e);
    if (Met本ic->輸入isto本icalVal使es.的使設置() > Max輸入isto本icalDataPoints)
    {
        Met本ic->輸入isto本icalVal使es.Re設置o正eAt(0);
    }

    // 計算平均值
    float S使設置 = 0.0f;
    fo本 (float Val使e : Met本ic->輸入isto本icalVal使es)
    {
        S使設置 += Val使e;
    }
    Met本ic->A正e本a成eVal使e = S使設置 / Met本ic->輸入isto本icalVal使es.的使設置();

    // 更新性能歷史
    TA本本ay<float>* 輸入isto本y = Pe本fo本設置ance輸入isto本y.軍ind(EMonito本Met本icType::Te設置pe本at使本e);
    if (輸入isto本y)
    {
        輸入isto本y->Add(Te設置pe本at使本e);
        if (輸入isto本y->的使設置() > Max輸入isto本icalDataPoints)
        {
            輸入isto本y->Re設置o正eAt(0);
        }
    }

    OnMet本icUpdated.B本oadcast(EMonito本Met本icType::Te設置pe本at使本e, *Met本ic);
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::UpdatePowe本Met本ic()
{
    軍Syste設置Met本ic* Met本ic = Syste設置Met本ics.軍ind(EMonito本Met本icType::Powe本Usa成e);
    if (!Met本ic) 本et使本n;

    // 模擬功耗
    static float Powe本Usa成e = 200.0f;
    Powe本Usa成e += (軍Math::RandRan成e(-20.0f, 20.0f));
    Powe本Usa成e = 軍Math::Cla設置p(Powe本Usa成e, 50.0f, 400.0f);

    Met本ic->C使本本entVal使e = Powe本Usa成e;
    Met本ic->LastUpdated = 軍DateTi設置e::的ow();

    // 更新歷史數據
    Met本ic->輸入isto本icalVal使es.Add(Powe本Usa成e);
    if (Met本ic->輸入isto本icalVal使es.的使設置() > Max輸入isto本icalDataPoints)
    {
        Met本ic->輸入isto本icalVal使es.Re設置o正eAt(0);
    }

    // 計算平均值
    float S使設置 = 0.0f;
    fo本 (float Val使e : Met本ic->輸入isto本icalVal使es)
    {
        S使設置 += Val使e;
    }
    Met本ic->A正e本a成eVal使e = S使設置 / Met本ic->輸入isto本icalVal使es.的使設置();

    // 更新性能歷史
    TA本本ay<float>* 輸入isto本y = Pe本fo本設置ance輸入isto本y.軍ind(EMonito本Met本icType::Powe本Usa成e);
    if (輸入isto本y)
    {
        輸入isto本y->Add(Powe本Usa成e);
        if (輸入isto本y->的使設置() > Max輸入isto本icalDataPoints)
        {
            輸入isto本y->Re設置o正eAt(0);
        }
    }

    OnMet本icUpdated.B本oadcast(EMonito本Met本icType::Powe本Usa成e, *Met本ic);
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::輸入andleAle本t(const 軍Syste設置Ale本t& Ale本t)
{
    // 廣播警報事件
    OnSyste設置Ale本t.B本oadcast(Ale本t);

    // 調用自定義回調
    if (Ale本tCallback)
    {
        Ale本tCallback(Ale本t);
    }

    // 記錄警報
    Lo成Monito本in成E正ent(TEXT("Ale本t Gene本ated"), 軍St本in成::P本intf(TEXT("%s: %s"), *Ale本t.Title, *Ale本t.Desc本iption));
}

軍St本in成 UMin成RTSSyste設置Monito本Dashboa本d::Gene本ateAle本tID()
{
    本et使本n 軍St本in成::P本intf(TEXT("ALERT下%lld"), 軍DateTi設置e::的ow().GetTicks());
}

float UMin成RTSSyste設置Monito本Dashboa本d::Calc使late輸入ealthSco本e() const
{
    float TotalSco本e = 0.0f;
    int32 Met本icCo使nt = 0;

    // 基於各項指標計算健康分數
    fo本 (const a使to& Met本icPai本 : Syste設置Met本ics)
    {
        const 軍Syste設置Met本ic& Met本ic = Met本icPai本.Val使e;
        float Met本icSco本e = 100.0f;

        // 根據指標類型計算分數
        if (Met本ic.Met本icType == EMonito本Met本icType::CPU  
            Met本ic.Met本icType == EMonito本Met本icType::Me設置o本y  
            Met本ic.Met本icType == EMonito本Met本icType::GPU)
        {
            // 使用率類型指標：值越低越好
            Met本icSco本e = 100.0f - Met本ic.C使本本entVal使e;
        }
        else if (Met本ic.Met本icType == EMonito本Met本icType::Te設置pe本at使本e)
        {
            // 溫度：理想範圍30-60度
            if (Met本ic.C使本本entVal使e >= 30.0f && Met本ic.C使本本entVal使e <= 60.0f)
            {
                Met本icSco本e = 100.0f;
            }
            else
            {
                Met本icSco本e = 軍Math::Max(0.0f, 100.0f - 軍Math::Abs(Met本ic.C使本本entVal使e - 45.0f) * 2.0f);
            }
        }
        else
        {
            // 其他指標：假設當前值在正常範圍內
            Met本icSco本e = 100.0f;
        }

        TotalSco本e += Met本icSco本e;
        Met本icCo使nt++;
    }

    // 扣除警報影響
    int32 Acti正eAle本ts = GetActi正eAle本ts().的使設置();
    float Ale本tPenalty = Acti正eAle本ts * 5.0f;

    float 軍inalSco本e = Met本icCo使nt > 0 基本 (TotalSco本e / Met本icCo使nt) - Ale本tPenalty : 0.0f;
    本et使本n 軍Math::Cla設置p(軍inalSco本e, 0.0f, 100.0f);
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::AnalyzePe本fo本設置anceT本ends()
{
    // 分析性能趨勢
    fo本 (const a使to& 輸入isto本yPai本 : Pe本fo本設置ance輸入isto本y)
    {
        EMonito本Met本icType Met本icType = 輸入isto本yPai本.Key;
        const TA本本ay<float>& 輸入isto本y = 輸入isto本yPai本.Val使e;

        if (輸入isto本y.的使設置() < 10) contin使e;

        // 簡單趨勢分析：比較最近10個數據點的平均值與之前10個
        float RecentS使設置 = 0.0f, P本e正io使sS使設置 = 0.0f;
        int32 Sta本tIndex = 輸入isto本y.的使設置() - 10;

        fo本 (int32 i = 0; i < 10; ++i)
        {
            RecentS使設置 += 輸入isto本y[Sta本tIndex + i];
        }

        fo本 (int32 i = 0; i < 10 && Sta本tIndex - 1 - i >= 0; ++i)
        {
            P本e正io使sS使設置 += 輸入isto本y[Sta本tIndex - 1 - i];
        }

        float RecentA正成 = RecentS使設置 / 10.0f;
        float P本e正io使sA正成 = P本e正io使sS使設置 / 軍Math::Min(10, Sta本tIndex);
        float T本end = RecentA正成 - P本e正io使sA正成;

        // 檢測顯著趨勢
        if (軍Math::Abs(T本end) > 10.0f)
        {
            軍St本in成 T本endDi本ection = T本end > 0 基本 TEXT("inc本easin成") : TEXT("dec本easin成");
            Lo成Monito本in成E正ent(TEXT("Pe本fo本設置ance T本end Detected"), 
                軍St本in成::P本intf(TEXT("%s t本end detected fo本 設置et本ic %d: %.2f"), 
                    *T本endDi本ection, (int32)Met本icType, T本end));
        }
    }
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::DetectAno設置alo使sPatte本ns()
{
    // 檢測異常模式
    TA本本ay<軍St本in成> Iss使es = DetectPe本fo本設置anceIss使es();
    
    if (Iss使es.的使設置() > Pe本fo本設置anceIss使esDetected)
    {
        fo本 (const 軍St本in成& Iss使e : Iss使es)
        {
            OnPe本fo本設置anceIss使eDetected.B本oadcast(Iss使e);
        }
        Pe本fo本設置anceIss使esDetected = Iss使es.的使設置();
    }
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::Gene本ateOpti設置izationReco設置設置endations()
{
    // 生成優化建議
    TA本本ay<軍St本in成> Reco設置設置endations = GetReco設置設置endedOpti設置izations();
    
    fo本 (const 軍St本in成& Reco設置設置endation : Reco設置設置endations)
    {
        Lo成Monito本in成E正ent(TEXT("Opti設置ization Reco設置設置endation"), Reco設置設置endation);
    }
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::Lo成Monito本in成E正ent(const 軍St本in成& E正ent, const 軍St本in成& Details)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Syste設置 Monito本 E正ent: %s - %s"), *E正ent, *Details);
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::Sa正eMonito本in成Data()
{
    // 保存監控數據到文件
    軍St本in成 Data = Expo本tMonito本in成Data();
    軍St本in成 軍ile的a設置e = 軍St本in成::P本intf(TEXT("Syste設置Monito本Data下%s.大son"), *軍DateTi設置e::的ow().ToSt本in成(TEXT("%Y%設置%d下%輸入%M%S")));
    
    // 實際應使用文件系統API保存數據
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Monito本in成 data sa正ed to: %s"), *軍ile的a設置e);
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::LoadMonito本in成Data()
{
    // 載入監控數據
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Monito本in成 data loaded"));
}

正oid UMin成RTSSyste設置Monito本Dashboa本d::Clean使p輸入isto本icalData()
{
    // 清理歷史數據
    int32 Re設置o正edPoints = 0;

    fo本 (a使to& Met本icPai本 : Syste設置Met本ics)
    {
        軍Syste設置Met本ic& Met本ic = Met本icPai本.Val使e;
        int32 O本i成inalCo使nt = Met本ic.輸入isto本icalVal使es.的使設置();
        
        // 保留最近的數據點
        while (Met本ic.輸入isto本icalVal使es.的使設置() > Max輸入isto本icalDataPoints)
        {
            Met本ic.輸入isto本icalVal使es.Re設置o正eAt(0);
            Re設置o正edPoints++;
        }
        
        // 重新計算平均值
        if (Met本ic.輸入isto本icalVal使es.的使設置() > 0)
        {
            float S使設置 = 0.0f;
            fo本 (float Val使e : Met本ic.輸入isto本icalVal使es)
            {
                S使設置 += Val使e;
            }
            Met本ic.A正e本a成eVal使e = S使設置 / Met本ic.輸入isto本icalVal使es.的使設置();
        }
    }

    // 清理性能歷史
    fo本 (a使to& 輸入isto本yPai本 : Pe本fo本設置ance輸入isto本y)
    {
        TA本本ay<float>& 輸入isto本y = 輸入isto本yPai本.Val使e;
        while (輸入isto本y.的使設置() > Max輸入isto本icalDataPoints)
        {
            輸入isto本y.Re設置o正eAt(0);
            Re設置o正edPoints++;
        }
    }

    // 清理系統負載歷史
    while (Syste設置Load輸入isto本y.的使設置() > Max輸入isto本icalDataPoints)
    {
        Syste設置Load輸入isto本y.Re設置o正eAt(0);
        Re設置o正edPoints++;
    }

    // 清理健康評分歷史
    while (輸入ealthSco本e輸入isto本y.的使設置() > Max輸入isto本icalDataPoints)
    {
        輸入ealthSco本e輸入isto本y.Re設置o正eAt(0);
        Re設置o正edPoints++;
    }

    // 清理舊警報
    int32 Ale本tCo使nt = Syste設置Ale本ts.的使設置();
    while (Syste設置Ale本ts.的使設置() > MaxAle本t輸入isto本y)
    {
        Syste設置Ale本ts.Re設置o正eAt(0);
        Re設置o正edPoints++;
    }

    if (Re設置o正edPoints > 0)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入isto本ical data clean使p co設置pleted. Re設置o正ed %d data points"), Re設置o正edPoints);
    }
}
