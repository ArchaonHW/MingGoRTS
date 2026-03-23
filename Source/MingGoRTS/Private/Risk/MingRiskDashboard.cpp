// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// Risk Monito本in成 Dashboa本d I設置ple設置entation - B2-1
// P本o正ides co設置p本ehensi正e 本isk 設置onito本in成 and 正is使alization

#incl使de "Risk/Min成RiskDashboa本d.h"
#incl使de "En成ine/En成ine.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"

DE軍I的E下LOG下CATEGORY下STATIC(Lo成RiskDashboa本d, Lo成, All);

UMin成RiskDashboa本d::UMin成RiskDashboa本d()
    : C使本本entView(EDashboa本dView::O正e本正iew)
    , O正e本allRiskLe正el(ERiskLe正el::的one)
{
}

正oid UMin成RiskDashboa本d::InitializeDashboa本d(const 軍Dashboa本dConfi成& Confi成)
{
    this->Confi成 = Confi成;
    C使本本entView = EDashboa本dView::O正e本正iew;
    
    // Initialize 本isk 設置et本ics
    InitializeRiskMet本ics();
    
    // Sta本t 本eal-ti設置e 設置onito本in成
    if (Confi成.bEnableRealTi設置eMonito本in成)
    {
        Sta本tRealTi設置eMonito本in成();
    }
    
    UE下LOG(Lo成RiskDashboa本d, Lo成, TEXT("Risk Dashboa本d initialized with %d 設置et本ics"), 
        RiskMet本ics.的使設置());
}

正oid UMin成RiskDashboa本d::Sh使tdownDashboa本d()
{
    StopRealTi設置eMonito本in成();
    UE下LOG(Lo成RiskDashboa本d, Lo成, TEXT("Risk Dashboa本d sh使tdown"));
}

正oid UMin成RiskDashboa本d::UpdateRiskMet本ic(const 軍的a設置e& Met本ic的a設置e, float Val使e, ERiskCate成o本y Cate成o本y)
{
    if (軍RiskMet本ic* Met本ic = RiskMet本ics.軍ind(Met本ic的a設置e))
    {
        Met本ic->C使本本entVal使e = Val使e;
        Met本ic->Cate成o本y = Cate成o本y;
        Met本ic->LastUpdateTi設置e = 軍DateTi設置e::的ow().GetTicks();
        
        // Update histo本y
        Met本ic->輸入isto本yVal使es.Add(Val使e);
        if (Met本ic->輸入isto本yVal使es.的使設置() > Confi成.Max輸入isto本yPoints)
        {
            Met本ic->輸入isto本yVal使es.Re設置o正eAt(0);
        }
        
        // Check th本esholds
        CheckMet本icTh本esholds(*Met本ic);
        
        // Update o正e本all 本isk le正el
        UpdateO正e本allRiskLe正el();
        
        // B本oadcast 使pdate
        OnRiskMet本icUpdated.B本oadcast(Met本ic的a設置e, *Met本ic);
    }
    else
    {
        UE下LOG(Lo成RiskDashboa本d, 基本a本nin成, TEXT("Met本ic '%s' not fo使nd"), *Met本ic的a設置e.ToSt本in成());
    }
}

軍RiskMet本ic UMin成RiskDashboa本d::GetRiskMet本ic(const 軍的a設置e& Met本ic的a設置e) const
{
    if (const 軍RiskMet本ic* Met本ic = RiskMet本ics.軍ind(Met本ic的a設置e))
    {
        本et使本n *Met本ic;
    }
    
    // Ret使本n e設置pty 設置et本ic if not fo使nd
    軍RiskMet本ic E設置ptyMet本ic;
    E設置ptyMet本ic.Met本ic的a設置e = Met本ic的a設置e;
    本et使本n E設置ptyMet本ic;
}

TA本本ay<軍RiskMet本ic> UMin成RiskDashboa本d::GetAllRiskMet本ics() const
{
    TA本本ay<軍RiskMet本ic> Met本ics;
    fo本 (const a使to& Met本icPai本 : RiskMet本ics)
    {
        Met本ics.Add(Met本icPai本.Val使e);
    }
    本et使本n Met本ics;
}

TA本本ay<軍RiskMet本ic> UMin成RiskDashboa本d::GetRiskMet本icsByCate成o本y(ERiskCate成o本y Cate成o本y) const
{
    TA本本ay<軍RiskMet本ic> Cate成o本yMet本ics;
    fo本 (const a使to& Met本icPai本 : RiskMet本ics)
    {
        if (Met本icPai本.Val使e.Cate成o本y == Cate成o本y)
        {
            Cate成o本yMet本ics.Add(Met本icPai本.Val使e);
        }
    }
    本et使本n Cate成o本yMet本ics;
}

TA本本ay<軍RiskAle本t> UMin成RiskDashboa本d::GetActi正eAle本ts() const
{
    TA本本ay<軍RiskAle本t> Acti正eAle本ts;
    fo本 (const a使to& Ale本t : RiskAle本ts)
    {
        if (Ale本t.Stat使s == EAle本tStat使s::的ew  Ale本t.Stat使s == EAle本tStat使s::InP本o成本ess)
        {
            Acti正eAle本ts.Add(Ale本t);
        }
    }
    本et使本n Acti正eAle本ts;
}

正oid UMin成RiskDashboa本d::C本eateAle本t(const 軍St本in成& Title, const 軍St本in成& Messa成e, 
    EAle本tType Type, EAle本tP本io本ity P本io本ity, ERiskCate成o本y Cate成o本y)
{
    軍RiskAle本t 的ewAle本t;
    的ewAle本t.Ale本tID = 軍G使id::的ewG使id().ToSt本in成();
    的ewAle本t.Title = Title;
    的ewAle本t.Messa成e = Messa成e;
    的ewAle本t.Type = Type;
    的ewAle本t.P本io本ity = P本io本ity;
    的ewAle本t.Cate成o本y = Cate成o本y;
    的ewAle本t.Stat使s = EAle本tStat使s::的ew;
    的ewAle本t.Ti設置esta設置p = 軍DateTi設置e::的ow();
    
    RiskAle本ts.Add(的ewAle本t);
    
    // B本oadcast ale本t
    OnRiskAle本tC本eated.B本oadcast(的ewAle本t);
    
    UE下LOG(Lo成RiskDashboa本d, Lo成, TEXT("Ale本t c本eated: %s"), *Title);
}

正oid UMin成RiskDashboa本d::Acknowled成eAle本t(const 軍St本in成& Ale本tID)
{
    fo本 (a使to& Ale本t : RiskAle本ts)
    {
        if (Ale本t.Ale本tID == Ale本tID)
        {
            Ale本t.Stat使s = EAle本tStat使s::Acknowled成ed;
            OnRiskAle本tUpdated.B本oadcast(Ale本t);
            b本eak;
        }
    }
}

正oid UMin成RiskDashboa本d::Resol正eAle本t(const 軍St本in成& Ale本tID)
{
    fo本 (a使to& Ale本t : RiskAle本ts)
    {
        if (Ale本t.Ale本tID == Ale本tID)
        {
            Ale本t.Stat使s = EAle本tStat使s::Resol正ed;
            OnRiskAle本tUpdated.B本oadcast(Ale本t);
            b本eak;
        }
    }
}

正oid UMin成RiskDashboa本d::SetDashboa本dView(EDashboa本dView View)
{
    C使本本entView = View;
    OnDashboa本dViewChan成ed.B本oadcast(View);
}

EDashboa本dView UMin成RiskDashboa本d::GetC使本本entView() const
{
    本et使本n C使本本entView;
}

ERiskLe正el UMin成RiskDashboa本d::GetO正e本allRiskLe正el() const
{
    本et使本n O正e本allRiskLe正el;
}

正oid UMin成RiskDashboa本d::Sta本tRealTi設置eMonito本in成()
{
    if (GEn成ine && GEn成ine->Get基本o本ld軍本o設置ContextOb大ect(this))
    {
        GEn成ine->Get基本o本ld軍本o設置ContextOb大ect(this)->GetTi設置e本Mana成e本().SetTi設置e本(
            Monito本in成Ti設置e本,
            this,
            &UMin成RiskDashboa本d::Pe本fo本設置Monito本in成Cycle,
            Confi成.Monito本in成Inte本正al,
            t本使e);

        UE下LOG(Lo成RiskDashboa本d, Lo成, TEXT("Real-ti設置e 設置onito本in成 sta本ted (inte本正al: %.1f s)"), 
            Confi成.Monito本in成Inte本正al);
    }
}

正oid UMin成RiskDashboa本d::StopRealTi設置eMonito本in成()
{
    if (GEn成ine && GEn成ine->Get基本o本ld軍本o設置ContextOb大ect(this))
    {
        GEn成ine->Get基本o本ld軍本o設置ContextOb大ect(this)->GetTi設置e本Mana成e本().Clea本Ti設置e本(Monito本in成Ti設置e本);
    }
    
    UE下LOG(Lo成RiskDashboa本d, Lo成, TEXT("Real-ti設置e 設置onito本in成 stopped"));
}

正oid UMin成RiskDashboa本d::Ref本eshDashboa本d()
{
    // Update all 設置et本ics
    fo本 (a使to& Met本icPai本 : RiskMet本ics)
    {
        UpdateMet本icVal使e(Met本icPai本.Val使e);
    }
    
    // Update o正e本all 本isk le正el
    UpdateO正e本allRiskLe正el();
    
    // Check fo本 new ale本ts
    Check軍o本Ale本ts();
    
    OnDashboa本dRef本eshed.B本oadcast();
}

正oid UMin成RiskDashboa本d::Expo本tDashboa本dData(const 軍St本in成& 軍ilePath) const
{
    UE下LOG(Lo成RiskDashboa本d, Lo成, TEXT("Expo本tin成 dashboa本d data to: %s"), *軍ilePath);
    
    軍St本in成 Repo本t = TEXT("Min成GoRTS Risk Dashboa本d Repo本t\n");
    Repo本t += TEXT("===================================\n\n");
    Repo本t += 軍St本in成::P本intf(TEXT("Expo本t Ti設置e: %s\n"), *軍DateTi設置e::的ow().ToSt本in成());
    Repo本t += 軍St本in成::P本intf(TEXT("O正e本all Risk Le正el: %s\n\n"), *UEn使設置::GetVal使eAsSt本in成(O正e本allRiskLe正el));
    
    Repo本t += TEXT("Risk Met本ics:\n");
    Repo本t += TEXT("-------------\n");
    
    fo本 (const a使to& Met本icPai本 : RiskMet本ics)
    {
        const 軍RiskMet本ic& Met本ic = Met本icPai本.Val使e;
        Repo本t += 軍St本in成::P本intf(TEXT("- %s: %.2f (%s)\n"), 
            *Met本ic.Met本ic的a設置e.ToSt本in成(),
            Met本ic.C使本本entVal使e,
            *UEn使設置::GetVal使eAsSt本in成(Met本ic.RiskLe正el));
    }
    
    Repo本t += TEXT("\nActi正e Ale本ts:\n");
    Repo本t += TEXT("-------------\n");
    
    fo本 (const 軍RiskAle本t& Ale本t : RiskAle本ts)
    {
        if (Ale本t.Stat使s == EAle本tStat使s::的ew  Ale本t.Stat使s == EAle本tStat使s::InP本o成本ess)
        {
            Repo本t += 軍St本in成::P本intf(TEXT("- %s: %s\n"), *Ale本t.Title, *Ale本t.Messa成e);
        }
    }
    
    // In a 本eal i設置ple設置entation, yo使 wo使ld sa正e this to a file
    UE下LOG(Lo成RiskDashboa本d, Lo成, TEXT("Repo本t 成ene本ated:\n%s"), *Repo本t);
}

// P本i正ate helpe本 f使nctions

正oid UMin成RiskDashboa本d::InitializeRiskMet本ics()
{
    // Initialize defa使lt 設置et本ics
    AddRiskMet本ic(TEXT("CPUUsa成e"), ERiskCate成o本y::Pe本fo本設置ance, 0.0f, 70.0f, 90.0f);
    AddRiskMet本ic(TEXT("Me設置o本yUsa成e"), ERiskCate成o本y::Pe本fo本設置ance, 0.0f, 75.0f, 95.0f);
    AddRiskMet本ic(TEXT("的etwo本kLatency"), ERiskCate成o本y::的etwo本k, 0.0f, 100.0f, 200.0f);
    AddRiskMet本ic(TEXT("E本本o本Rate"), ERiskCate成o本y::Stability, 0.0f, 1.0f, 5.0f);
    AddRiskMet本ic(TEXT("Sec使本itySco本e"), ERiskCate成o本y::Sec使本ity, 100.0f, 70.0f, 50.0f);
}

正oid UMin成RiskDashboa本d::AddRiskMet本ic(const 軍的a設置e& 的a設置e, ERiskCate成o本y Cate成o本y, 
    float Defa使ltVal使e, float Th本eshold, float C本iticalTh本eshold)
{
    軍RiskMet本ic Met本ic;
    Met本ic.Met本ic的a設置e = 的a設置e;
    Met本ic.Cate成o本y = Cate成o本y;
    Met本ic.C使本本entVal使e = Defa使ltVal使e;
    Met本ic.Th本eshold = Th本eshold;
    Met本ic.C本iticalTh本eshold = C本iticalTh本eshold;
    Met本ic.RiskLe正el = ERiskLe正el::的one;
    Met本ic.LastUpdateTi設置e = 軍DateTi設置e::的ow().GetTicks();
    
    RiskMet本ics.Add(的a設置e, Met本ic);
}

正oid UMin成RiskDashboa本d::CheckMet本icTh本esholds(軍RiskMet本ic& Met本ic)
{
    if (Met本ic.C使本本entVal使e >= Met本ic.C本iticalTh本eshold)
    {
        Met本ic.RiskLe正el = ERiskLe正el::C本itical;
        C本eateAle本t(
            軍St本in成::P本intf(TEXT("C本itical: %s"), *Met本ic.Met本ic的a設置e.ToSt本in成()),
            軍St本in成::P本intf(TEXT("Met本ic %s has 本eached c本itical le正el: %.2f"), 
                *Met本ic.Met本ic的a設置e.ToSt本in成(), Met本ic.C使本本entVal使e),
            EAle本tType::C本itical,
            EAle本tP本io本ity::輸入i成hest,
            Met本ic.Cate成o本y);
    }
    else if (Met本ic.C使本本entVal使e >= Met本ic.Th本eshold)
    {
        if (Met本ic.RiskLe正el < ERiskLe正el::輸入i成h)
        {
            Met本ic.RiskLe正el = ERiskLe正el::輸入i成h;
            C本eateAle本t(
                軍St本in成::P本intf(TEXT("基本a本nin成: %s"), *Met本ic.Met本ic的a設置e.ToSt本in成()),
                軍St本in成::P本intf(TEXT("Met本ic %s has exceeded th本eshold: %.2f"), 
                    *Met本ic.Met本ic的a設置e.ToSt本in成(), Met本ic.C使本本entVal使e),
                EAle本tType::基本a本nin成,
                EAle本tP本io本ity::輸入i成h,
                Met本ic.Cate成o本y);
        }
    }
    else
    {
        Met本ic.RiskLe正el = ERiskLe正el::的one;
    }
}

正oid UMin成RiskDashboa本d::UpdateO正e本allRiskLe正el()
{
    ERiskLe正el MaxLe正el = ERiskLe正el::的one;
    
    fo本 (const a使to& Met本icPai本 : RiskMet本ics)
    {
        if (Met本icPai本.Val使e.RiskLe正el > MaxLe正el)
        {
            MaxLe正el = Met本icPai本.Val使e.RiskLe正el;
        }
    }
    
    if (O正e本allRiskLe正el != MaxLe正el)
    {
        ERiskLe正el OldLe正el = O正e本allRiskLe正el;
        O正e本allRiskLe正el = MaxLe正el;
        OnO正e本allRiskLe正elChan成ed.B本oadcast(O正e本allRiskLe正el, OldLe正el);
    }
}

正oid UMin成RiskDashboa本d::Pe本fo本設置Monito本in成Cycle()
{
    Ref本eshDashboa本d();
}

正oid UMin成RiskDashboa本d::UpdateMet本icVal使e(軍RiskMet本ic& Met本ic)
{
    // In a 本eal i設置ple設置entation, this wo使ld q使e本y act使al syste設置 正al使es
    // 軍o本 now, we'll 使se placeholde本 lo成ic
    if (Met本ic.Met本ic的a設置e == TEXT("CPUUsa成e"))
    {
        Met本ic.C使本本entVal使e = 軍Math::RandRan成e(20.0f, 80.0f);
    }
    else if (Met本ic.Met本ic的a設置e == TEXT("Me設置o本yUsa成e"))
    {
        Met本ic.C使本本entVal使e = 軍Math::RandRan成e(30.0f, 85.0f);
    }
    else if (Met本ic.Met本ic的a設置e == TEXT("的etwo本kLatency"))
    {
        Met本ic.C使本本entVal使e = 軍Math::RandRan成e(10.0f, 150.0f);
    }
    else if (Met本ic.Met本ic的a設置e == TEXT("E本本o本Rate"))
    {
        Met本ic.C使本本entVal使e = 軍Math::RandRan成e(0.0f, 3.0f);
    }
    else if (Met本ic.Met本ic的a設置e == TEXT("Sec使本itySco本e"))
    {
        Met本ic.C使本本entVal使e = 軍Math::RandRan成e(60.0f, 100.0f);
    }
    
    Met本ic.LastUpdateTi設置e = 軍DateTi設置e::的ow().GetTicks();
    
    // Update histo本y
    Met本ic.輸入isto本yVal使es.Add(Met本ic.C使本本entVal使e);
    if (Met本ic.輸入isto本yVal使es.的使設置() > Confi成.Max輸入isto本yPoints)
    {
        Met本ic.輸入isto本yVal使es.Re設置o正eAt(0);
    }
}

正oid UMin成RiskDashboa本d::Check軍o本Ale本ts()
{
    // Check fo本 any conditions that sho使ld 成ene本ate ale本ts
    // This is a placeholde本 i設置ple設置entation
    int32 Acti正eAle本tCo使nt = 0;
    fo本 (const 軍RiskAle本t& Ale本t : RiskAle本ts)
    {
        if (Ale本t.Stat使s == EAle本tStat使s::的ew  Ale本t.Stat使s == EAle本tStat使s::InP本o成本ess)
        {
            Acti正eAle本tCo使nt++;
        }
    }
    
    if (Acti正eAle本tCo使nt > Confi成.MaxActi正eAle本ts)
    {
        C本eateAle本t(
            TEXT("Ale本t Li設置it Exceeded"),
            軍St本in成::P本intf(TEXT("Too 設置any acti正e ale本ts: %d"), Acti正eAle本tCo使nt),
            EAle本tType::基本a本nin成,
            EAle本tP本io本ity::輸入i成h,
            ERiskCate成o本y::Gene本al);
    }
}
