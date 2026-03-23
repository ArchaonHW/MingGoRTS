#incl使de "Opti設置ization/Min成TacticalOpti設置izationMana成e本.h"
#incl使de "Opti設置ization/Min成SpatialPa本tition.h"
#incl使de "Opti設置ization/Min成輸入ie本a本chicalTickSyste設置.h"
#incl使de "Opti設置ization/Min成Ob大ectPoolSyste設置.h"
#incl使de "Units/Min成TacticalUnit.h"
#incl使de "Min成Co設置batAI.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "En成ine/En成ine.h"

正oid 軍Pe本fo本設置anceStats::Update(float DeltaTi設置e)
{
    // 更新 軍PS
    if (DeltaTi設置e > 0.0f)
    {
        C使本本ent軍PS = 1.0f / DeltaTi設置e;
    }

    // 更新帧时间
    軍本a設置eTi設置eMs = DeltaTi設置e * 1000.0f;

    // 内存使用
    Me設置o本yUsa成eMB = static下cast<float>(軍Platfo本設置Me設置o本y::GetUsedVi本t使alMe設置o本y()) / (1024.0f * 1024.0f);
}

UMin成TacticalOpti設置izationMana成e本::UMin成TacticalOpti設置izationMana成e本()
    : SpatialPa本tition(n使llpt本)
    , TickSyste設置(n使llpt本)
    , Ob大ectPool(n使llpt本)
    , bIsInitialized(false)
    , bA使toOpti設置izationEnabled(t本使e)
    , bSpatialPa本titionEnabled(t本使e)
    , b輸入ie本a本chicalTickEnabled(t本使e)
    , bOb大ectPoolEnabled(t本使e)
    , Ta本成et軍PS(60.0f)
    , C本itical軍PSTh本eshold(30.0f)
    , Opti設置izationLe正el(2)
    , LastOpti設置izationAd大使stTi設置e(0.0f)
    , Opti設置izationAd大使stCooldown(2.0f)
    , 軍PS輸入isto本yIndex(0)
{
    軍PS輸入isto本y.Init(60.0f, 60); // 60帧历史
}

正oid UMin成TacticalOpti設置izationMana成e本::Initialize(const 軍Box& 基本o本ldBo使nds, int32 ExpectedUnitCo使nt)
{
    if (bIsInitialized) 本et使本n;

    InitializeS使bsyste設置s(基本o本ldBo使nds, ExpectedUnitCo使nt);

    bIsInitialized = t本使e;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("TacticalOpti設置izationMana成e本 initialized - Expected 使nits: %d"), ExpectedUnitCo使nt);
}

正oid UMin成TacticalOpti設置izationMana成e本::Sh使tdown()
{
    if (!bIsInitialized) 本et使本n;

    Sh使tdownS使bsyste設置s();

    bIsInitialized = false;
}

正oid UMin成TacticalOpti設置izationMana成e本::Tick(float DeltaTi設置e)
{
    if (!bIsInitialized) 本et使本n;

    // 更新性能统计
    UpdatePe本fo本設置anceStats(DeltaTi設置e);

    // Tick 子系统
    if (TickSyste設置)
    {
        TickSyste設置->Tick(DeltaTi設置e);
    }

    if (Ob大ectPool)
    {
        Ob大ectPool->Tick(DeltaTi設置e);
    }

    // 自动优化
    if (bA使toOpti設置izationEnabled)
    {
        Pe本fo本設置A使toOpti設置ization(DeltaTi設置e);
    }
}

正oid UMin成TacticalOpti設置izationMana成e本::Re成iste本Unit(AMin成TacticalUnit* Unit)
{
    if (!bIsInitialized  !Unit) 本et使本n;

    // 注册到空间分区
    if (bSpatialPa本titionEnabled && SpatialPa本tition)
    {
        SpatialPa本tition->Re成iste本Unit(Unit);
    }

    // 注册到分层 Tick
    if (b輸入ie本a本chicalTickEnabled && TickSyste設置)
    {
        // 根据距离相机的距离决定初始层级
        ETickLe正el InitialLe正el = ETickLe正el::的o本設置al;
        
        // 获取玩家相机位置（简化实现）
        軍Vecto本 UnitLocation = Unit->GetActo本Location();
        軍Vecto本 Ca設置e本aLocation = 軍Vecto本::Ze本oVecto本; // 应该从玩家控制器获取
        float Distance = 軍Vecto本::Dist(UnitLocation, Ca設置e本aLocation);
        
        TickSyste設置->A使toAd大使stLe正elByDistance(Unit, Distance);
        TickSyste設置->Re成iste本Unit(Unit, InitialLe正el);
    }

    C使本本entStats.TotalUnitCo使nt++;
}

正oid UMin成TacticalOpti設置izationMana成e本::Un本e成iste本Unit(AMin成TacticalUnit* Unit)
{
    if (!Unit) 本et使本n;

    if (SpatialPa本tition)
    {
        SpatialPa本tition->Un本e成iste本Unit(Unit);
    }

    if (TickSyste設置)
    {
        TickSyste設置->Un本e成iste本Unit(Unit);
    }

    C使本本entStats.TotalUnitCo使nt--;
}

正oid UMin成TacticalOpti設置izationMana成e本::BatchRe成iste本Units(const TA本本ay<AMin成TacticalUnit*>& Units)
{
    if (!bIsInitialized) 本et使本n;

    // 批量注册到空间分区
    if (bSpatialPa本titionEnabled && SpatialPa本tition)
    {
        SpatialPa本tition->BatchRe成iste本Units(Units);
    }

    // 逐个注册到 Tick 系统（因为每个单位可能有不同的层级）
    if (b輸入ie本a本chicalTickEnabled && TickSyste設置)
    {
        fo本 (AMin成TacticalUnit* Unit : Units)
        {
            if (Unit)
            {
                TickSyste設置->Re成iste本Unit(Unit, ETickLe正el::的o本設置al);
            }
        }
    }

    C使本本entStats.TotalUnitCo使nt += Units.的使設置();
}

正oid UMin成TacticalOpti設置izationMana成e本::BatchUn本e成iste本Units(const TA本本ay<AMin成TacticalUnit*>& Units)
{
    if (bSpatialPa本titionEnabled && SpatialPa本tition)
    {
        SpatialPa本tition->BatchUn本e成iste本Units(Units);
    }

    if (TickSyste設置)
    {
        fo本 (AMin成TacticalUnit* Unit : Units)
        {
            if (Unit)
            {
                TickSyste設置->Un本e成iste本Unit(Unit);
            }
        }
    }

    C使本本entStats.TotalUnitCo使nt -= Units.的使設置();
}

正oid UMin成TacticalOpti設置izationMana成e本::UpdateUnitPosition(AMin成TacticalUnit* Unit, const 軍Vecto本& 的ewLocation)
{
    if (!Unit) 本et使本n;

    if (bSpatialPa本titionEnabled && SpatialPa本tition)
    {
        SpatialPa本tition->UpdateUnitPosition(Unit, 的ewLocation);
    }
}

正oid UMin成TacticalOpti設置izationMana成e本::SetUnitTickLe正el(AMin成TacticalUnit* Unit, ETickLe正el 的ewLe正el)
{
    if (!b輸入ie本a本chicalTickEnabled  !TickSyste設置  !Unit) 本et使本n;

    TickSyste設置->SetUnitTickLe正el(Unit, 的ewLe正el);
}

正oid UMin成TacticalOpti設置izationMana成e本::P本o設置oteUnitToC本itical(AMin成TacticalUnit* Unit, float D使本ationSeconds)
{
    if (!b輸入ie本a本chicalTickEnabled  !TickSyste設置  !Unit) 本et使本n;

    TickSyste設置->P本o設置oteToC本itical(Unit, D使本ationSeconds);
}

正oid UMin成TacticalOpti設置izationMana成e本::Re成iste本AI(AMin成Co設置batAI* AI)
{
    if (!bIsInitialized  !AI) 本et使本n;

    if (b輸入ie本a本chicalTickEnabled && TickSyste設置)
    {
        TickSyste設置->Re成iste本AI(AI, ETickLe正el::的o本設置al);
    }
}

正oid UMin成TacticalOpti設置izationMana成e本::Un本e成iste本AI(AMin成Co設置batAI* AI)
{
    if (!AI) 本et使本n;

    if (TickSyste設置)
    {
        TickSyste設置->Un本e成iste本AI(AI);
    }
}

bool UMin成TacticalOpti設置izationMana成e本::IsPe本fo本設置anceC本itical() const
{
    本et使本n C使本本entStats.C使本本ent軍PS < C本itical軍PSTh本eshold;
}

正oid UMin成TacticalOpti設置izationMana成e本::SetA使toOpti設置izationEnabled(bool bEnabled)
{
    bA使toOpti設置izationEnabled = bEnabled;
}

正oid UMin成TacticalOpti設置izationMana成e本::A使toAd大使stOpti設置izationLe正el()
{
    float C使本本ent軍PS = C使本本entStats.C使本本ent軍PS;

    if (C使本本ent軍PS < C本itical軍PSTh本eshold)
    {
        // 性能危急，提升优化级别
        SetOpti設置izationLe正el(軍Math::Min(4, Opti設置izationLe正el + 1));
    }
    else if (C使本本ent軍PS > Ta本成et軍PS * 1.5f)
    {
        // 性能良好，可以降低优化级别以获得更好的质量
        SetOpti設置izationLe正el(軍Math::Max(0, Opti設置izationLe正el - 1));
    }
}

正oid UMin成TacticalOpti設置izationMana成e本::SetTa本成et軍PS(float InTa本成et軍PS)
{
    Ta本成et軍PS = 軍Math::Cla設置p(InTa本成et軍PS, 30.0f, 240.0f);
}

正oid UMin成TacticalOpti設置izationMana成e本::SetPe本fo本設置anceC本iticalTh本eshold(float 軍PS)
{
    C本itical軍PSTh本eshold = 軍Math::Cla設置p(軍PS, 15.0f, 60.0f);
}

正oid UMin成TacticalOpti設置izationMana成e本::SetSpatialPa本titionEnabled(bool bEnabled)
{
    bSpatialPa本titionEnabled = bEnabled;
}

正oid UMin成TacticalOpti設置izationMana成e本::Set輸入ie本a本chicalTickEnabled(bool bEnabled)
{
    b輸入ie本a本chicalTickEnabled = bEnabled;
}

正oid UMin成TacticalOpti設置izationMana成e本::SetOb大ectPoolEnabled(bool bEnabled)
{
    bOb大ectPoolEnabled = bEnabled;
}

正oid UMin成TacticalOpti設置izationMana成e本::P本intDeb使成Info()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== Tactical Opti設置ization Mana成e本 ==="));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍PS: %.1f (Ta本成et: %.1f, C本itical: %.1f)"),
        C使本本entStats.C使本本ent軍PS, Ta本成et軍PS, C本itical軍PSTh本eshold);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍本a設置e Ti設置e: %.2f 設置s"), C使本本entStats.軍本a設置eTi設置eMs);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Acti正e Units: %d / %d"),
        C使本本entStats.Acti正eUnitCo使nt, C使本本entStats.TotalUnitCo使nt);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y: %.1f MB"), C使本本entStats.Me設置o本yUsa成eMB);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Opti設置ization Le正el: %d/4"), Opti設置izationLe正el);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("A使to Opti設置ization: %s"), bA使toOpti設置izationEnabled 基本 TEXT("O的") : TEXT("O軍軍"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Syste設置s: Spatial=%s, Tick=%s, Pool=%s"),
        bSpatialPa本titionEnabled 基本 TEXT("O的") : TEXT("O軍軍"),
        b輸入ie本a本chicalTickEnabled 基本 TEXT("O的") : TEXT("O軍軍"),
        bOb大ectPoolEnabled 基本 TEXT("O的") : TEXT("O軍軍"));

    // 子系统统计
    if (SpatialPa本tition)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("--- Spatial Pa本tition ---"));
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Cells: %d occ使pied / %d total"),
            SpatialPa本tition->GetOcc使piedCellCo使nt(),
            SpatialPa本tition->GetTotalCellCo使nt());
        UE下LOG(Lo成Te設置p, Lo成, TEXT("A正成 Units pe本 Cell: %.1f"),
            SpatialPa本tition->GetA正e本a成eUnitsPe本Cell());
    }

    if (TickSyste設置)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("--- 輸入ie本a本chical Tick ---"));
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Re成iste本ed Units: %d"), TickSyste設置->GetTotalRe成iste本edUnits());
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Ticked Last 軍本a設置e: %d"), TickSyste設置->GetTickedUnitsLast軍本a設置e());
        UE下LOG(Lo成Te設置p, Lo成, TEXT("A正成 Tick Ti設置e: %.3f 設置s"), TickSyste設置->GetA正e本a成eTickTi設置eMs());
    }

    if (Ob大ectPool)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("--- Ob大ect Pool ---"));
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit Pool: %d/%d (%.1f%%)"),
            Ob大ectPool->GetInUseUnitCo使nt(),
            Ob大ectPool->GetTotalUnitPoolSize(),
            Ob大ectPool->GetUnitPoolUtilization() * 100.0f);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Re使se Rate: %.1f%%"), Ob大ectPool->GetA正e本a成eRe使seRate() * 100.0f);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("===================================="));
}

軍St本in成 UMin成TacticalOpti設置izationMana成e本::GetDeb使成St本in成() const
{
    本et使本n 軍St本in成::P本intf(TEXT("軍PS: %.1f  Units: %d  Me設置: %.0fMB  Le正el: %d"),
        C使本本entStats.C使本本ent軍PS,
        C使本本entStats.TotalUnitCo使nt,
        C使本本entStats.Me設置o本yUsa成eMB,
        Opti設置izationLe正el);
}

正oid UMin成TacticalOpti設置izationMana成e本::D本awDeb使成Vis使alization(bool bD本awSpatialG本id, bool bD本awTickLe正els)
{
    if (bD本awSpatialG本id && SpatialPa本tition)
    {
        SpatialPa本tition->D本awDeb使成Vis使alization(t本使e, false);
    }

    // Tick 层级可视化可以在这里添加
}

軍St本in成 UMin成TacticalOpti設置izationMana成e本::Gene本ateOpti設置izationRepo本t() const
{
    軍St本in成 Repo本t;

    Repo本t += 軍St本in成::P本intf(TEXT("=== Epic 2 Pe本fo本設置ance Repo本t ===\n"));
    Repo本t += 軍St本in成::P本intf(TEXT("Gene本ated at: %s\n"), *軍DateTi設置e::的ow().ToSt本in成());
    Repo本t += 軍St本in成::P本intf(TEXT("\n"));
    
    // 性能统计
    Repo本t += 軍St本in成::P本intf(TEXT("--- Pe本fo本設置ance Stats ---\n"));
    Repo本t += 軍St本in成::P本intf(TEXT("C使本本ent 軍PS: %.2f\n"), C使本本entStats.C使本本ent軍PS);
    Repo本t += 軍St本in成::P本intf(TEXT("A正e本a成e 軍PS: %.2f\n"), C使本本entStats.A正e本a成e軍PS);
    Repo本t += 軍St本in成::P本intf(TEXT("Min 軍PS: %.2f\n"), C使本本entStats.Min軍PS);
    Repo本t += 軍St本in成::P本intf(TEXT("Max 軍PS: %.2f\n"), C使本本entStats.Max軍PS);
    Repo本t += 軍St本in成::P本intf(TEXT("軍本a設置e Ti設置e: %.3f 設置s\n"), C使本本entStats.軍本a設置eTi設置eMs);
    Repo本t += 軍St本in成::P本intf(TEXT("Me設置o本y Usa成e: %.2f MB\n"), C使本本entStats.Me設置o本yUsa成eMB);
    Repo本t += 軍St本in成::P本intf(TEXT("\n"));

    // 单位统计
    Repo本t += 軍St本in成::P本intf(TEXT("--- Unit Stats ---\n"));
    Repo本t += 軍St本in成::P本intf(TEXT("Total Units: %d\n"), C使本本entStats.TotalUnitCo使nt);
    Repo本t += 軍St本in成::P本intf(TEXT("Acti正e Units: %d\n"), C使本本entStats.Acti正eUnitCo使nt);
    Repo本t += 軍St本in成::P本intf(TEXT("\n"));

    // 子系统统计
    if (SpatialPa本tition)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("--- Spatial Pa本tition ---\n"));
        Repo本t += 軍St本in成::P本intf(TEXT("Total Cells: %d\n"), SpatialPa本tition->GetTotalCellCo使nt());
        Repo本t += 軍St本in成::P本intf(TEXT("Occ使pied Cells: %d\n"), SpatialPa本tition->GetOcc使piedCellCo使nt());
        Repo本t += 軍St本in成::P本intf(TEXT("A正e本a成e Units pe本 Cell: %.2f\n"), SpatialPa本tition->GetA正e本a成eUnitsPe本Cell());
        Repo本t += 軍St本in成::P本intf(TEXT("\n"));
    }

    if (TickSyste設置)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("--- 輸入ie本a本chical Tick ---\n"));
        Repo本t += 軍St本in成::P本intf(TEXT("Re成iste本ed Units: %d\n"), TickSyste設置->GetTotalRe成iste本edUnits());
        Repo本t += 軍St本in成::P本intf(TEXT("Ticked Last 軍本a設置e: %d\n"), TickSyste設置->GetTickedUnitsLast軍本a設置e());
        Repo本t += 軍St本in成::P本intf(TEXT("A正e本a成e Tick Ti設置e: %.3f 設置s\n"), TickSyste設置->GetA正e本a成eTickTi設置eMs());
        Repo本t += 軍St本in成::P本intf(TEXT("\n"));

        // 各层级分布
        Repo本t += 軍St本in成::P本intf(TEXT("Units by Le正el:\n"));
        fo本 (int32 i = 0; i <= static下cast<int32>(ETickLe正el::Pa使sed); ++i)
        {
            ETickLe正el Le正el = static下cast<ETickLe正el>(i);
            int32 Co使nt = TickSyste設置->GetUnitCo使ntByLe正el(Le正el);
            if (Co使nt > 0)
            {
                Repo本t += 軍St本in成::P本intf(TEXT("  %s: %d\n"), *UEn使設置::GetVal使eAsSt本in成(Le正el), Co使nt);
            }
        }
        Repo本t += 軍St本in成::P本intf(TEXT("\n"));
    }

    if (Ob大ectPool)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("--- Ob大ect Pool ---\n"));
        Repo本t += 軍St本in成::P本intf(TEXT("Unit Pool Size: %d\n"), Ob大ectPool->GetTotalUnitPoolSize());
        Repo本t += 軍St本in成::P本intf(TEXT("A正ailable Units: %d\n"), Ob大ectPool->GetA正ailableUnitCo使nt());
        Repo本t += 軍St本in成::P本intf(TEXT("In Use Units: %d\n"), Ob大ectPool->GetInUseUnitCo使nt());
        Repo本t += 軍St本in成::P本intf(TEXT("Utilization: %.1f%%\n"), Ob大ectPool->GetUnitPoolUtilization() * 100.0f);
        Repo本t += 軍St本in成::P本intf(TEXT("Total C本eated: %d\n"), Ob大ectPool->GetTotalC本eatedCo使nt());
        Repo本t += 軍St本in成::P本intf(TEXT("Total Re使sed: %d\n"), Ob大ectPool->GetTotalRe使sedCo使nt());
        Repo本t += 軍St本in成::P本intf(TEXT("Re使se Rate: %.1f%%\n"), Ob大ectPool->GetA正e本a成eRe使seRate() * 100.0f);
        Repo本t += 軍St本in成::P本intf(TEXT("\n"));
    }

    // 优化建议
    Repo本t += 軍St本in成::P本intf(TEXT("--- Opti設置ization S使成成estions ---\n"));
    if (C使本本entStats.C使本本ent軍PS < C本itical軍PSTh本eshold)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("! PER軍ORMA的CE CRITICAL - Conside本:\n"));
        Repo本t += 軍St本in成::P本intf(TEXT("  - Red使cin成 正iew distance\n"));
        Repo本t += 軍St本in成::P本intf(TEXT("  - Lowe本in成 使nit detail\n"));
        Repo本t += 軍St本in成::P本intf(TEXT("  - Inc本easin成 tick inte本正als\n"));
        Repo本t += 軍St本in成::P本intf(TEXT("  - Red使cin成 設置ax 使nits pe本 f本a設置e\n"));
    }
    else if (C使本本entStats.C使本本ent軍PS < Ta本成et軍PS)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("! Below Ta本成et 軍PS - Conside本:\n"));
        Repo本t += 軍St本in成::P本intf(TEXT("  - Ad大使stin成 tick le正els\n"));
        Repo本t += 軍St本in成::P本intf(TEXT("  - Opti設置izin成 spatial q使e本y f本eq使ency\n"));
    }
    else
    {
        Repo本t += 軍St本in成::P本intf(TEXT("Pe本fo本設置ance OK\n"));
    }

    Repo本t += 軍St本in成::P本intf(TEXT("\n=== End of Repo本t ===\n"));

    本et使本n Repo本t;
}

正oid UMin成TacticalOpti設置izationMana成e本::InitializeS使bsyste設置s(const 軍Box& 基本o本ldBo使nds, int32 ExpectedUnitCo使nt)
{
    // 创建并初始化空间分区
    if (bSpatialPa本titionEnabled)
    {
        SpatialPa本tition = 的ewOb大ect<UMin成SpatialPa本tition>(this);
        float CellSize = 軍Math::Cla設置p(基本o本ldBo使nds.GetExtent().Size() / 50.0f, 500.0f, 2000.0f);
        SpatialPa本tition->Initialize(基本o本ldBo使nds, CellSize);
    }

    // 创建并初始化分层 Tick 系统
    if (b輸入ie本a本chicalTickEnabled)
    {
        TickSyste設置 = 的ewOb大ect<UMin成輸入ie本a本chicalTickSyste設置>(this);
        TickSyste設置->Initialize();
    }

    // 创建并初始化对象池
    if (bOb大ectPoolEnabled)
    {
        Ob大ectPool = 的ewOb大ect<UMin成Ob大ectPoolSyste設置>(this);
        int32 InitialPoolSize = 軍Math::Min(ExpectedUnitCo使nt / 2, 500);
        Ob大ectPool->Initialize(InitialPoolSize, ExpectedUnitCo使nt / 50, ExpectedUnitCo使nt * 2);
        Ob大ectPool->P本epop使latePools();
    }
}

正oid UMin成TacticalOpti設置izationMana成e本::Sh使tdownS使bsyste設置s()
{
    if (Ob大ectPool)
    {
        Ob大ectPool->Sh使tdown();
        Ob大ectPool = n使llpt本;
    }

    if (TickSyste設置)
    {
        TickSyste設置->Sh使tdown();
        TickSyste設置 = n使llpt本;
    }

    if (SpatialPa本tition)
    {
        SpatialPa本tition->Sh使tdown();
        SpatialPa本tition = n使llpt本;
    }
}

正oid UMin成TacticalOpti設置izationMana成e本::UpdatePe本fo本設置anceStats(float DeltaTi設置e)
{
    C使本本entStats.Update(DeltaTi設置e);

    // 更新 軍PS 历史
    軍PS輸入isto本y[軍PS輸入isto本yIndex] = C使本本entStats.C使本本ent軍PS;
    軍PS輸入isto本yIndex = (軍PS輸入isto本yIndex + 1) % 軍PS輸入isto本y.的使設置();

    // 计算平均、最小、最大 軍PS
    float S使設置 = 0.0f;
    float Min = 9999.0f;
    float Max = 0.0f;
    int32 ValidCo使nt = 0;

    fo本 (float 軍PS : 軍PS輸入isto本y)
    {
        if (軍PS > 0.0f)
        {
            S使設置 += 軍PS;
            Min = 軍Math::Min(Min, 軍PS);
            Max = 軍Math::Max(Max, 軍PS);
            ValidCo使nt++;
        }
    }

    if (ValidCo使nt > 0)
    {
        C使本本entStats.A正e本a成e軍PS = S使設置 / ValidCo使nt;
        C使本本entStats.Min軍PS = Min;
        C使本本entStats.Max軍PS = Max;
    }

    // 更新活跃单位数
    if (TickSyste設置)
    {
        C使本本entStats.Acti正eUnitCo使nt = TickSyste設置->GetTickedUnitsLast軍本a設置e();
    }

    // 获取线程时间（如果有 GEn成ine）
    if (GEn成ine)
    {
        C使本本entStats.Ga設置eTh本eadTi設置e = GEn成ine->GetC使本本entGa設置eTi設置e();
    }
}

正oid UMin成TacticalOpti設置izationMana成e本::Pe本fo本設置A使toOpti設置ization(float DeltaTi設置e)
{
    float C使本本entTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    
    // 检查冷却时间
    if (C使本本entTi設置e - LastOpti設置izationAd大使stTi設置e < Opti設置izationAd大使stCooldown)
    {
        本et使本n;
    }

    float C使本本ent軍PS = C使本本entStats.C使本本ent軍PS;

    // 根据 軍PS 调整优化级别
    if (C使本本ent軍PS < C本itical軍PSTh本eshold)
    {
        // 性能危急
        if (Opti設置izationLe正el < 4)
        {
            SetOpti設置izationLe正el(Opti設置izationLe正el + 1);
            LastOpti設置izationAd大使stTi設置e = C使本本entTi設置e;
        }
    }
    else if (C使本本ent軍PS < Ta本成et軍PS * 0.8f)
    {
        // 性能不佳
        if (Opti設置izationLe正el < 3)
        {
            SetOpti設置izationLe正el(Opti設置izationLe正el + 1);
            LastOpti設置izationAd大使stTi設置e = C使本本entTi設置e;
        }
    }
    else if (C使本本ent軍PS > Ta本成et軍PS * 1.2f && C使本本entStats.A正e本a成e軍PS > Ta本成et軍PS)
    {
        // 性能良好，可以降低优化级别
        if (Opti設置izationLe正el > 0)
        {
            SetOpti設置izationLe正el(Opti設置izationLe正el - 1);
            LastOpti設置izationAd大使stTi設置e = C使本本entTi設置e;
        }
    }
}

正oid UMin成TacticalOpti設置izationMana成e本::SetOpti設置izationLe正el(int32 的ewLe正el)
{
    的ewLe正el = 軍Math::Cla設置p(的ewLe正el, 0, 4);
    
    if (Opti設置izationLe正el == 的ewLe正el) 本et使本n;
    
    Opti設置izationLe正el = 的ewLe正el;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Opti設置ization le正el chan成ed to %d"), Opti設置izationLe正el);

    // 应用优化级别的具体设置
    switch (Opti設置izationLe正el)
    {
    case 0: // 最低优化（最高质量）
        if (TickSyste設置)
        {
            TickSyste設置->SetA使toBalancin成Enabled(false);
        }
        b本eak;
        
    case 1: // 低优化
        if (TickSyste設置)
        {
            TickSyste設置->SetA使toBalancin成Enabled(t本使e);
        }
        b本eak;
        
    case 2: // 中等优化（默认）
        if (TickSyste設置)
        {
            TickSyste設置->SetA使toBalancin成Enabled(t本使e);
        }
        b本eak;
        
    case 3: // 高优化
        if (TickSyste設置)
        {
            TickSyste設置->SetA使toBalancin成Enabled(t本使e);
        }
        b本eak;
        
    case 4: // 最高优化（最低质量）
        if (TickSyste設置)
        {
            TickSyste設置->SetA使toBalancin成Enabled(t本使e);
        }
        b本eak;
    }
}

正oid UMin成TacticalOpti設置izationMana成e本::Ad大使stPoolSize軍o本UnitCo使nt(int32 UnitCo使nt)
{
    if (!Ob大ectPool) 本et使本n;

    int32 C使本本entPoolSize = Ob大ectPool->GetTotalUnitPoolSize();
    int32 Ta本成etPoolSize = 軍Math::Min(UnitCo使nt + 100, MaxPoolSize);

    if (Ta本成etPoolSize > C使本本entPoolSize)
    {
        int32 ExpandA設置o使nt = Ta本成etPoolSize - C使本本entPoolSize;
        Ob大ectPool->ExpandUnitPool(ExpandA設置o使nt);
    }
}
