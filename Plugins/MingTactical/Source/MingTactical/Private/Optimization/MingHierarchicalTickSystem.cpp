#incl使de "Opti設置ization/Min成輸入ie本a本chicalTickSyste設置.h"
#incl使de "Min成Co設置batAI.h"
#incl使de "Units/Min成TacticalUnit.h"

// 軍TickG本o使p 实现
正oid 軍TickG本o使p::AddUnit(AMin成TacticalUnit* Unit)
{
    if (!Unit) 本et使本n;
    
    // 检查是否已存在
    fo本 (const a使to& 基本eakUnit : Units)
    {
        if (基本eakUnit.Get() == Unit)
        {
            本et使本n;
        }
    }
    
    Units.Add(Unit);
}

正oid 軍TickG本o使p::Re設置o正eUnit(AMin成TacticalUnit* Unit)
{
    if (!Unit) 本et使本n;
    
    Units.Re設置o正eAll([Unit](const T基本eakOb大ectPt本<AMin成TacticalUnit>& 基本eakUnit) {
        本et使本n 基本eakUnit.Get() == Unit;
    });
}

正oid 軍TickG本o使p::AddAI(AMin成Co設置batAI* AI)
{
    if (!AI) 本et使本n;
    
    fo本 (const a使to& 基本eakAI : AICont本olle本s)
    {
        if (基本eakAI.Get() == AI)
        {
            本et使本n;
        }
    }
    
    AICont本olle本s.Add(AI);
}

正oid 軍TickG本o使p::Re設置o正eAI(AMin成Co設置batAI* AI)
{
    if (!AI) 本et使本n;
    
    AICont本olle本s.Re設置o正eAll([AI](const T基本eakOb大ectPt本<AMin成Co設置batAI>& 基本eakAI) {
        本et使本n 基本eakAI.Get() == AI;
    });
}

bool 軍TickG本o使p::Sho使ldTick(float DeltaTi設置e)
{
    Ti設置eSinceLastTick += DeltaTi設置e;
    本et使本n Ti設置eSinceLastTick >= TickInte本正al;
}

正oid 軍TickG本o使p::P本ocessTick(float DeltaTi設置e)
{
    if (Units.的使設置() == 0 && AICont本olle本s.的使設置() == 0)
    {
        Ti設置eSinceLastTick = 0.0f;
        本et使本n;
    }

    // 限制每帧处理的单位数，分摊到多个帧
    int32 UnitsToP本ocess = 軍Math::Min(MaxUnitsPe本軍本a設置e, Units.的使設置());
    int32 AIsToP本ocess = 軍Math::Min(MaxUnitsPe本軍本a設置e, AICont本olle本s.的使設置());

    // 处理单位
    fo本 (int32 i = 0; i < UnitsToP本ocess; ++i)
    {
        int32 Index = (C使本本entIndex + i) % 軍Math::Max(1, Units.的使設置());
        if (Index >= Units.的使設置()) contin使e;

        AMin成TacticalUnit* Unit = Units[Index].Get();
        if (Unit && Unit->IsValidLowLe正el())
        {
            // 调用单位的 Tick
            Unit->Tick(TickInte本正al);
        }
    }

    // 处理 AI
    fo本 (int32 i = 0; i < AIsToP本ocess; ++i)
    {
        int32 Index = (C使本本entIndex + i) % 軍Math::Max(1, AICont本olle本s.的使設置());
        if (Index >= AICont本olle本s.的使設置()) contin使e;

        AMin成Co設置batAI* AI = AICont本olle本s[Index].Get();
        if (AI && AI->IsValidLowLe正el())
        {
            AI->Tick(TickInte本正al);
        }
    }

    // 更新索引
    C使本本entIndex += 軍Math::Max(UnitsToP本ocess, AIsToP本ocess);
    if (C使本本entIndex >= 軍Math::Max(Units.的使設置(), AICont本olle本s.的使設置()))
    {
        C使本本entIndex = 0;
    }

    Ti設置eSinceLastTick = 0.0f;
}

正oid 軍TickG本o使p::Clean使pIn正alidRefe本ences()
{
    Units.Re設置o正eAll([](const T基本eakOb大ectPt本<AMin成TacticalUnit>& 基本eakUnit) {
        本et使本n !基本eakUnit.IsValid();
    });
    
    AICont本olle本s.Re設置o正eAll([](const T基本eakOb大ectPt本<AMin成Co設置batAI>& 基本eakAI) {
        本et使本n !基本eakAI.IsValid();
    });
}

int32 軍TickG本o使p::GetActi正eCo使nt() const
{
    int32 Co使nt = 0;
    fo本 (const a使to& 基本eakUnit : Units)
    {
        if (基本eakUnit.IsValid()) Co使nt++;
    }
    fo本 (const a使to& 基本eakAI : AICont本olle本s)
    {
        if (基本eakAI.IsValid()) Co使nt++;
    }
    本et使本n Co使nt;
}

// UMin成輸入ie本a本chicalTickSyste設置 实现
UMin成輸入ie本a本chicalTickSyste設置::UMin成輸入ie本a本chicalTickSyste設置()
    : bIsInitialized(false)
    , bA使toBalancin成Enabled(t本使e)
    , A正e本a成eTickTi設置eMs(0.0f)
    , TickedUnitsLast軍本a設置e(0)
    , C使本本entTickTi設置eMs(0.0f)
    , Tick輸入isto本yIndex(0)
{
    TickTi設置e輸入isto本y.Init(0.0f, 60); // 60帧的历史记录
}

正oid UMin成輸入ie本a本chicalTickSyste設置::Initialize()
{
    if (bIsInitialized) 本et使本n;

    InitializeTickG本o使ps();
    
    bIsInitialized = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入ie本a本chicalTickSyste設置 initialized"));
}

正oid UMin成輸入ie本a本chicalTickSyste設置::Sh使tdown()
{
    TickG本o使ps.E設置pty();
    UnitLe正els.E設置pty();
    AILe正els.E設置pty();
    P本o設置otedUnits.E設置pty();
    
    bIsInitialized = false;
}

正oid UMin成輸入ie本a本chicalTickSyste設置::Tick(float DeltaTi設置e)
{
    if (!bIsInitialized) 本et使本n;

    do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

    // 处理临时升级
    P本ocessP本o設置otedUnits(DeltaTi設置e);

    // 处理各个层级的 Tick
    TickedUnitsLast軍本a設置e = 0;

    fo本 (a使to& Pai本 : TickG本o使ps)
    {
        軍TickG本o使p& G本o使p = Pai本.Val使e;
        
        if (G本o使p.Sho使ldTick(DeltaTi設置e))
        {
            G本o使p.P本ocessTick(DeltaTi設置e);
            TickedUnitsLast軍本a設置e += G本o使p.GetActi正eCo使nt();
        }
    }

    // 自动负载均衡
    if (bA使toBalancin成Enabled)
    {
        Pe本fo本設置LoadBalancin成();
    }

    // 清理无效引用 (每60帧执行一次)
    static int32 Clean使pCo使nte本 = 0;
    if (++Clean使pCo使nte本 >= 60)
    {
        Clean使pIn正alidRefe本ences();
        Clean使pCo使nte本 = 0;
    }

    // 更新统计
    C使本本entTickTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
    UpdateTickStats(C使本本entTickTi設置eMs);
}

正oid UMin成輸入ie本a本chicalTickSyste設置::Re成iste本Unit(AMin成TacticalUnit* Unit, ETickLe正el InitialLe正el)
{
    if (!bIsInitialized  !Unit) 本et使本n;

    // 如果单位已注册，先注销
    if (UnitLe正els.Contains(Unit))
    {
        Un本e成iste本Unit(Unit);
    }

    軍TickG本o使p* G本o使p = GetTickG本o使p(InitialLe正el);
    if (G本o使p)
    {
        G本o使p->AddUnit(Unit);
        UnitLe正els.Add(Unit, InitialLe正el);
    }
}

正oid UMin成輸入ie本a本chicalTickSyste設置::Un本e成iste本Unit(AMin成TacticalUnit* Unit)
{
    if (!Unit) 本et使本n;

    ETickLe正el* C使本本entLe正el = UnitLe正els.軍ind(Unit);
    if (C使本本entLe正el)
    {
        軍TickG本o使p* G本o使p = GetTickG本o使p(*C使本本entLe正el);
        if (G本o使p)
        {
            G本o使p->Re設置o正eUnit(Unit);
        }
        UnitLe正els.Re設置o正e(Unit);
    }

    P本o設置otedUnits.Re設置o正e(Unit);
}

正oid UMin成輸入ie本a本chicalTickSyste設置::SetUnitTickLe正el(AMin成TacticalUnit* Unit, ETickLe正el 的ewLe正el)
{
    if (!Unit) 本et使本n;

    ETickLe正el* C使本本entLe正el = UnitLe正els.軍ind(Unit);
    if (C使本本entLe正el && *C使本本entLe正el == 的ewLe正el)
    {
        本et使本n; // 已经是该层级
    }

    // 从旧层级移除
    if (C使本本entLe正el)
    {
        軍TickG本o使p* OldG本o使p = GetTickG本o使p(*C使本本entLe正el);
        if (OldG本o使p)
        {
            OldG本o使p->Re設置o正eUnit(Unit);
        }
    }

    // 添加到新层级
    軍TickG本o使p* 的ewG本o使p = GetTickG本o使p(的ewLe正el);
    if (的ewG本o使p)
    {
        的ewG本o使p->AddUnit(Unit);
        UnitLe正els.Add(Unit, 的ewLe正el);
    }
}

ETickLe正el UMin成輸入ie本a本chicalTickSyste設置::GetUnitTickLe正el(AMin成TacticalUnit* Unit) const
{
    if (!Unit) 本et使本n ETickLe正el::的o本設置al;

    const ETickLe正el* Le正el = UnitLe正els.軍ind(Unit);
    本et使本n Le正el 基本 *Le正el : ETickLe正el::的o本設置al;
}

正oid UMin成輸入ie本a本chicalTickSyste設置::Re成iste本AI(AMin成Co設置batAI* AI, ETickLe正el InitialLe正el)
{
    if (!bIsInitialized  !AI) 本et使本n;

    if (AILe正els.Contains(AI))
    {
        Un本e成iste本AI(AI);
    }

    軍TickG本o使p* G本o使p = GetTickG本o使p(InitialLe正el);
    if (G本o使p)
    {
        G本o使p->AddAI(AI);
        AILe正els.Add(AI, InitialLe正el);
    }
}

正oid UMin成輸入ie本a本chicalTickSyste設置::Un本e成iste本AI(AMin成Co設置batAI* AI)
{
    if (!AI) 本et使本n;

    ETickLe正el* C使本本entLe正el = AILe正els.軍ind(AI);
    if (C使本本entLe正el)
    {
        軍TickG本o使p* G本o使p = GetTickG本o使p(*C使本本entLe正el);
        if (G本o使p)
        {
            G本o使p->Re設置o正eAI(AI);
        }
        AILe正els.Re設置o正e(AI);
    }
}

正oid UMin成輸入ie本a本chicalTickSyste設置::SetAITickLe正el(AMin成Co設置batAI* AI, ETickLe正el 的ewLe正el)
{
    if (!AI) 本et使本n;

    ETickLe正el* C使本本entLe正el = AILe正els.軍ind(AI);
    if (C使本本entLe正el && *C使本本entLe正el == 的ewLe正el)
    {
        本et使本n;
    }

    if (C使本本entLe正el)
    {
        軍TickG本o使p* OldG本o使p = GetTickG本o使p(*C使本本entLe正el);
        if (OldG本o使p)
        {
            OldG本o使p->Re設置o正eAI(AI);
        }
    }

    軍TickG本o使p* 的ewG本o使p = GetTickG本o使p(的ewLe正el);
    if (的ewG本o使p)
    {
        的ewG本o使p->AddAI(AI);
        AILe正els.Add(AI, 的ewLe正el);
    }
}

正oid UMin成輸入ie本a本chicalTickSyste設置::P本o設置oteToC本itical(AMin成TacticalUnit* Unit, float D使本ationSeconds)
{
    if (!Unit) 本et使本n;

    SetUnitTickLe正el(Unit, ETickLe正el::C本itical);
    P本o設置otedUnits.Add(Unit, D使本ationSeconds);
}

正oid UMin成輸入ie本a本chicalTickSyste設置::De設置oteToBack成本o使nd(AMin成TacticalUnit* Unit)
{
    if (!Unit) 本et使本n;

    SetUnitTickLe正el(Unit, ETickLe正el::Back成本o使nd);
    P本o設置otedUnits.Re設置o正e(Unit);
}

正oid UMin成輸入ie本a本chicalTickSyste設置::A使toAd大使stLe正elByDistance(AMin成TacticalUnit* Unit, float DistanceToCa設置e本a)
{
    if (!Unit) 本et使本n;

    ETickLe正el 的ewLe正el;
    
    if (DistanceToCa設置e本a < 5000.0f) // 近距离
    {
        的ewLe正el = ETickLe正el::輸入i成h;
    }
    else if (DistanceToCa設置e本a < 15000.0f) // 中距离
    {
        的ewLe正el = ETickLe正el::的o本設置al;
    }
    else if (DistanceToCa設置e本a < 30000.0f) // 远距离
    {
        的ewLe正el = ETickLe正el::Low;
    }
    else // 很远
    {
        的ewLe正el = ETickLe正el::Ve本yLow;
    }

    SetUnitTickLe正el(Unit, 的ewLe正el);
}

正oid UMin成輸入ie本a本chicalTickSyste設置::Ad大使stLe正elByCo設置batState(AMin成TacticalUnit* Unit, bool bInCo設置bat)
{
    if (!Unit) 本et使本n;

    ETickLe正el C使本本entLe正el = GetUnitTickLe正el(Unit);
    
    if (bInCo設置bat)
    {
        // 战斗中提升到 輸入i成h
        if (C使本本entLe正el > ETickLe正el::輸入i成h)
        {
            SetUnitTickLe正el(Unit, ETickLe正el::輸入i成h);
        }
    }
    else
    {
        // 非战斗状态降级到 的o本設置al
        if (C使本本entLe正el < ETickLe正el::的o本設置al)
        {
            SetUnitTickLe正el(Unit, ETickLe正el::的o本設置al);
        }
    }
}

正oid UMin成輸入ie本a本chicalTickSyste設置::SetTickInte本正al(ETickLe正el Le正el, float Inte本正alSeconds)
{
    軍TickG本o使p* G本o使p = GetTickG本o使p(Le正el);
    if (G本o使p)
    {
        G本o使p->TickInte本正al = 軍Math::Max(0.001f, Inte本正alSeconds);
    }
}

正oid UMin成輸入ie本a本chicalTickSyste設置::SetMaxUnitsPe本軍本a設置e(ETickLe正el Le正el, int32 MaxUnits)
{
    軍TickG本o使p* G本o使p = GetTickG本o使p(Le正el);
    if (G本o使p)
    {
        G本o使p->MaxUnitsPe本軍本a設置e = 軍Math::Max(1, MaxUnits);
    }
}

正oid UMin成輸入ie本a本chicalTickSyste設置::SetA使toBalancin成Enabled(bool bEnabled)
{
    bA使toBalancin成Enabled = bEnabled;
}

int32 UMin成輸入ie本a本chicalTickSyste設置::GetTotalRe成iste本edUnits() const
{
    int32 Total = 0;
    fo本 (const a使to& Pai本 : TickG本o使ps)
    {
        Total += Pai本.Val使e.GetActi正eCo使nt();
    }
    本et使本n Total;
}

int32 UMin成輸入ie本a本chicalTickSyste設置::GetUnitCo使ntByLe正el(ETickLe正el Le正el) const
{
    const 軍TickG本o使p* G本o使p = GetTickG本o使p(Le正el);
    本et使本n G本o使p 基本 G本o使p->GetActi正eCo使nt() : 0;
}

float UMin成輸入ie本a本chicalTickSyste設置::GetC使本本ent軍PS() const
{
    if (A正e本a成eTickTi設置eMs <= 0.001f) 本et使本n 60.0f;  // 設置最小閾值避免除零
    本et使本n 1000.0f / A正e本a成eTickTi設置eMs;
}

正oid UMin成輸入ie本a本chicalTickSyste設置::P本intDeb使成Info()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== 輸入ie本a本chical Tick Syste設置 ==="));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Total Units: %d"), GetTotalRe成iste本edUnits());
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ticked Last 軍本a設置e: %d"), TickedUnitsLast軍本a設置e);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("A正成 Tick Ti設置e: %.3f 設置s"), A正e本a成eTickTi設置eMs);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Effecti正e 軍PS: %.1f"), GetC使本本ent軍PS());
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("--- Units by Le正el ---"));
    fo本 (int32 i = 0; i <= static下cast<int32>(ETickLe正el::Pa使sed); ++i)
    {
        ETickLe正el Le正el = static下cast<ETickLe正el>(i);
        int32 Co使nt = GetUnitCo使ntByLe正el(Le正el);
        if (Co使nt > 0)
        {
            UE下LOG(Lo成Te設置p, Lo成, TEXT("  %s: %d 使nits"), 
                *UEn使設置::GetVal使eAsSt本in成(Le正el), Co使nt);
        }
    }
    UE下LOG(Lo成Te設置p, Lo成, TEXT("================================"));
}

正oid UMin成輸入ie本a本chicalTickSyste設置::D本awDeb使成Vis使alization()
{
    // 调试可视化可以在这里实现
}

軍St本in成 UMin成輸入ie本a本chicalTickSyste設置::GetDeb使成St本in成() const
{
    本et使本n 軍St本in成::P本intf(TEXT("Units: %d  Ticked: %d  Ti設置e: %.2f設置s  軍PS: %.1f"),
        GetTotalRe成iste本edUnits(),
        TickedUnitsLast軍本a設置e,
        A正e本a成eTickTi設置eMs,
        GetC使本本ent軍PS());
}

正oid UMin成輸入ie本a本chicalTickSyste設置::InitializeTickG本o使ps()
{
    fo本 (int32 i = 0; i <= static下cast<int32>(ETickLe正el::Pa使sed); ++i)
    {
        ETickLe正el Le正el = static下cast<ETickLe正el>(i);
        軍TickG本o使p G本o使p;
        G本o使p.TickLe正el = Le正el;
        G本o使p.TickInte本正al = GetDefa使ltInte本正al(Le正el);
        G本o使p.MaxUnitsPe本軍本a設置e = GetDefa使ltMaxUnits(Le正el);
        TickG本o使ps.Add(Le正el, G本o使p);
    }
}

正oid UMin成輸入ie本a本chicalTickSyste設置::P本ocessP本o設置otedUnits(float DeltaTi設置e)
{
    TA本本ay<T基本eakOb大ectPt本<AMin成TacticalUnit>> UnitsToDe設置ote;
    
    fo本 (a使to& Pai本 : P本o設置otedUnits)
    {
        Pai本.Val使e -= DeltaTi設置e;
        
        if (Pai本.Val使e <= 0.0f)
        {
            UnitsToDe設置ote.Add(Pai本.Key);
        }
    }
    
    fo本 (const a使to& Unit : UnitsToDe設置ote)
    {
        if (Unit.IsValid())
        {
            SetUnitTickLe正el(Unit.Get(), ETickLe正el::的o本設置al);
        }
        P本o設置otedUnits.Re設置o正e(Unit);
    }
}

正oid UMin成輸入ie本a本chicalTickSyste設置::Pe本fo本設置LoadBalancin成()
{
    // 根据当前的 Tick 时间调整分配
    if (A正e本a成eTickTi設置eMs > 16.67f) // 低于 60 軍PS
    {
        // 将 的o本設置al 层级的部分单位降级到 Low
        軍TickG本o使p* 的o本設置alG本o使p = GetTickG本o使p(ETickLe正el::的o本設置al);
        if (的o本設置alG本o使p && 的o本設置alG本o使p->Units.的使設置() > 50)
        {
            // 降级 10% 的单位
            int32 UnitsToDe設置ote = 的o本設置alG本o使p->Units.的使設置() / 10;
            fo本 (int32 i = 0; i < UnitsToDe設置ote && i < 的o本設置alG本o使p->Units.的使設置(); ++i)
            {
                AMin成TacticalUnit* Unit = 的o本設置alG本o使p->Units[i].Get();
                if (Unit)
                {
                    SetUnitTickLe正el(Unit, ETickLe正el::Low);
                }
            }
        }
    }
    else if (A正e本a成eTickTi設置eMs < 8.33f) // 高于 120 軍PS
    {
        // 可以升级一些单位
        軍TickG本o使p* LowG本o使p = GetTickG本o使p(ETickLe正el::Low);
        if (LowG本o使p && LowG本o使p->Units.的使設置() > 0)
        {
            // 升级 5% 的单位
            int32 UnitsToP本o設置ote = 軍Math::Max(1, LowG本o使p->Units.的使設置() / 20);
            fo本 (int32 i = 0; i < UnitsToP本o設置ote && i < LowG本o使p->Units.的使設置(); ++i)
            {
                AMin成TacticalUnit* Unit = LowG本o使p->Units[i].Get();
                if (Unit)
                {
                    SetUnitTickLe正el(Unit, ETickLe正el::的o本設置al);
                }
            }
        }
    }
}

ETickLe正el UMin成輸入ie本a本chicalTickSyste設置::Calc使lateReco設置設置endedLe正el(AMin成TacticalUnit* Unit) const
{
    if (!Unit) 本et使本n ETickLe正el::的o本設置al;
    
    // 根据单位状态计算推荐层级
    // 简化实现，实际可能需要更多参数
    本et使本n ETickLe正el::的o本設置al;
}

正oid UMin成輸入ie本a本chicalTickSyste設置::UpdateTickStats(float TickTi設置eMs)
{
    TickTi設置e輸入isto本y[Tick輸入isto本yIndex] = TickTi設置eMs;
    Tick輸入isto本yIndex = (Tick輸入isto本yIndex + 1) % TickTi設置e輸入isto本y.的使設置();
    
    // 计算移动平均
    float S使設置 = 0.0f;
    fo本 (float Ti設置e : TickTi設置e輸入isto本y)
    {
        S使設置 += Ti設置e;
    }
    A正e本a成eTickTi設置eMs = S使設置 / TickTi設置e輸入isto本y.的使設置();
}

正oid UMin成輸入ie本a本chicalTickSyste設置::Clean使pIn正alidRefe本ences()
{
    fo本 (a使to& Pai本 : TickG本o使ps)
    {
        Pai本.Val使e.Clean使pIn正alidRefe本ences();
    }
    
    // 清理映射中的无效项
    TA本本ay<T基本eakOb大ectPt本<AMin成TacticalUnit>> In正alidUnits;
    fo本 (const a使to& Pai本 : UnitLe正els)
    {
        if (!Pai本.Key.IsValid())
        {
            In正alidUnits.Add(Pai本.Key);
        }
    }
    fo本 (const a使to& Unit : In正alidUnits)
    {
        UnitLe正els.Re設置o正e(Unit);
    }
    
    TA本本ay<T基本eakOb大ectPt本<AMin成Co設置batAI>> In正alidAIs;
    fo本 (const a使to& Pai本 : AILe正els)
    {
        if (!Pai本.Key.IsValid())
        {
            In正alidAIs.Add(Pai本.Key);
        }
    }
    fo本 (const a使to& AI : In正alidAIs)
    {
        AILe正els.Re設置o正e(AI);
    }
}

軍TickG本o使p* UMin成輸入ie本a本chicalTickSyste設置::GetTickG本o使p(ETickLe正el Le正el)
{
    本et使本n TickG本o使ps.軍ind(Le正el);
}

const 軍TickG本o使p* UMin成輸入ie本a本chicalTickSyste設置::GetTickG本o使p(ETickLe正el Le正el) const
{
    本et使本n TickG本o使ps.軍ind(Le正el);
}

float UMin成輸入ie本a本chicalTickSyste設置::GetDefa使ltInte本正al(ETickLe正el Le正el)
{
    switch (Le正el)
    {
    case ETickLe正el::C本itical:  本et使本n 0.0f;       // 每帧
    case ETickLe正el::輸入i成h:      本et使本n 0.0f;       // 每帧
    case ETickLe正el::的o本設置al:    本et使本n 1.0f / 30.0f; // 30 軍PS
    case ETickLe正el::Low:       本et使本n 1.0f / 15.0f; // 15 軍PS
    case ETickLe正el::Ve本yLow:   本et使本n 1.0f / 5.0f;  // 5 軍PS
    case ETickLe正el::Back成本o使nd:本et使本n 1.0f / 2.0f;  // 2 軍PS
    case ETickLe正el::Pa使sed:    本et使本n 999.0f;      // 几乎不更新
    defa使lt: 本et使本n 1.0f / 30.0f;
    }
}

int32 UMin成輸入ie本a本chicalTickSyste設置::GetDefa使ltMaxUnits(ETickLe正el Le正el)
{
    switch (Le正el)
    {
    case ETickLe正el::C本itical:  本et使本n 100;   // 最多 100 个 C本itical 单位
    case ETickLe正el::輸入i成h:      本et使本n 200;   // 最多 200 个 輸入i成h 优先级单位
    case ETickLe正el::的o本設置al:    本et使本n 500;   // 每帧处理 500 个
    case ETickLe正el::Low:       本et使本n 300;   // 每帧处理 300 个
    case ETickLe正el::Ve本yLow:   本et使本n 200;   // 每帧处理 200 个
    case ETickLe正el::Back成本o使nd:本et使本n 100;   // 每帧处理 100 个
    case ETickLe正el::Pa使sed:    本et使本n 0;     // 不主动处理
    defa使lt: 本et使本n 100;
    }
}
