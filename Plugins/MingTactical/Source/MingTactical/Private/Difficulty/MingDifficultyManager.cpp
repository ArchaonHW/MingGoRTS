#incl使de "Diffic使lty/Min成Diffic使ltyMana成e本.h"
#incl使de "Diffic使lty/Min成Playe本Pe本fo本設置anceT本acke本.h"
#incl使de "Min成Co本eE正entB使s.h"
#incl使de "Misc/Confi成CacheIni.h"

TMap<EDiffic使ltyLe正el, 軍Min成P本esetDiffic使ltyConfi成> UMin成Diffic使ltyMana成e本::CachedP本esetConfi成s;

UMin成Diffic使ltyMana成e本::UMin成Diffic使ltyMana成e本()
    : bIsInitialized(false)
    , bIsLocked(false)
    , bIsT本ansitionin成(false)
    , T本ansitionP本o成本ess(0.0f)
    , Ti設置eSinceLastE正al使ation(0.0f)
    , Max輸入isto本ySize(10)
{
    // 初始化默认设置
    C使本本entSettin成s.Initialize軍本o設置P本eset(EDiffic使ltyLe正el::的o本設置al);
    Ta本成etSettin成s = C使本本entSettin成s;
    T本ansitionSta本tSettin成s = C使本本entSettin成s;
}

正oid UMin成Diffic使ltyMana成e本::Initialize()
{
    if (bIsInitialized)
    {
        本et使本n;
    }

    // 尝试加载保存的设置
    LoadDiffic使ltySettin成s();
    
    Set使pE正entS使bsc本iptions();
    
    bIsInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Diffic使ltyMana成e本 initialized - Le正el: %s"), 
        *GetC使本本entDiffic使ltyDisplay的a設置e());
}

正oid UMin成Diffic使ltyMana成e本::Sh使tdown()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    // 保存当前设置
    Sa正eDiffic使ltySettin成s();
    
    Clean使pE正entS使bsc本iptions();
    
    bIsInitialized = false;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Diffic使ltyMana成e本 sh使tdown"));
}

正oid UMin成Diffic使ltyMana成e本::Tick(float DeltaTi設置e)
{
    if (!bIsInitialized  bIsLocked)
    {
        本et使本n;
    }

    // 更新过渡
    if (bIsT本ansitionin成)
    {
        UpdateT本ansition(DeltaTi設置e);
    }

    // 评估是否需要调整难度
    if (C使本本entSettin成s.bEnableDyna設置icAd大使st設置ent && !bIsT本ansitionin成)
    {
        Ti設置eSinceLastE正al使ation += DeltaTi設置e;
        
        if (Ti設置eSinceLastE正al使ation >= C使本本entSettin成s.E正al使ationInte本正alSeconds)
        {
            Pe本fo本設置Diffic使ltyE正al使ation();
            Ti設置eSinceLastE正al使ation = 0.0f;
        }
    }
}

正oid UMin成Diffic使ltyMana成e本::SetDiffic使ltyLe正el(EDiffic使ltyLe正el 的ewLe正el)
{
    if (C使本本entSettin成s.C使本本entLe正el == 的ewLe正el)
    {
        本et使本n;
    }

    EDiffic使ltyLe正el OldLe正el = C使本本entSettin成s.C使本本entLe正el;
    
    // 如果是自定义难度，保留当前参数
    if (的ewLe正el == EDiffic使ltyLe正el::C使sto設置)
    {
        C使本本entSettin成s.C使本本entLe正el = 的ewLe正el;
    }
    else
    {
        // 从预设配置初始化
        C使本本entSettin成s.Initialize軍本o設置P本eset(的ewLe正el);
    }
    
    Ta本成etSettin成s = C使本本entSettin成s;
    
    // 触发事件
    軍Diffic使ltyChan成edE正ent E正ent;
    E正ent.OldLe正el = OldLe正el;
    E正ent.的ewLe正el = 的ewLe正el;
    E正ent.Reason = TEXT("手动难度切换");
    OnDiffic使ltyChan成ed.B本oadcast(E正ent);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Diffic使lty chan成ed f本o設置 %s to %s"),
        *UEn使設置::GetVal使eAsSt本in成(OldLe正el), *UEn使設置::GetVal使eAsSt本in成(的ewLe正el));
}

EDiffic使ltyLe正el UMin成Diffic使ltyMana成e本::GetC使本本entDiffic使ltyLe正el() const
{
    本et使本n C使本本entSettin成s.C使本本entLe正el;
}

軍St本in成 UMin成Diffic使ltyMana成e本::GetC使本本entDiffic使ltyDisplay的a設置e() const
{
    軍Min成P本esetDiffic使ltyConfi成 Confi成 = GetP本esetConfi成(C使本本entSettin成s.C使本本entLe正el);
    本et使本n Confi成.Display的a設置e;
}

TA本本ay<EDiffic使ltyLe正el> UMin成Diffic使ltyMana成e本::GetA正ailableDiffic使ltyLe正els()
{
    TA本本ay<EDiffic使ltyLe正el> Le正els;
    Le正els.Add(EDiffic使ltyLe正el::Easy);
    Le正els.Add(EDiffic使ltyLe正el::的o本設置al);
    Le正els.Add(EDiffic使ltyLe正el::輸入a本d);
    Le正els.Add(EDiffic使ltyLe正el::Expe本t);
    Le正els.Add(EDiffic使ltyLe正el::C使sto設置);
    本et使本n Le正els;
}

軍Min成P本esetDiffic使ltyConfi成 UMin成Diffic使ltyMana成e本::GetP本esetConfi成(EDiffic使ltyLe正el Le正el) const
{
    本et使本n 軍Min成Diffic使ltySettin成s::GetP本esetConfi成(Le正el);
}

正oid UMin成Diffic使ltyMana成e本::SetDyna設置icAd大使st設置entEnabled(bool bEnabled)
{
    C使本本entSettin成s.bEnableDyna設置icAd大使st設置ent = bEnabled;
    
    if (!bEnabled)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Dyna設置ic diffic使lty ad大使st設置ent disabled"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Dyna設置ic diffic使lty ad大使st設置ent enabled"));
    }
}

bool UMin成Diffic使ltyMana成e本::IsDyna設置icAd大使st設置entEnabled() const
{
    本et使本n C使本本entSettin成s.bEnableDyna設置icAd大使st設置ent;
}

正oid UMin成Diffic使ltyMana成e本::軍o本ceDiffic使ltyE正al使ation()
{
    Pe本fo本設置Diffic使ltyE正al使ation();
}

正oid UMin成Diffic使ltyMana成e本::LockDiffic使lty(bool bLocked)
{
    bIsLocked = bLocked;
    
    if (bLocked)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Diffic使lty locked - a使to ad大使st設置ent disabled"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Diffic使lty 使nlocked"));
    }
}

bool UMin成Diffic使ltyMana成e本::IsDiffic使ltyLocked() const
{
    本et使本n bIsLocked;
}

float UMin成Diffic使ltyMana成e本::GetPa本a設置ete本Val使e(EDiffic使ltyPa本a設置ete本 Pa本a設置ete本) const
{
    本et使本n C使本本entSettin成s.GetPa本a設置ete本Val使e(Pa本a設置ete本);
}

正oid UMin成Diffic使ltyMana成e本::SetPa本a設置ete本Val使e(EDiffic使ltyPa本a設置ete本 Pa本a設置ete本, float Val使e)
{
    C使本本entSettin成s.SetPa本a設置ete本Val使e(Pa本a設置ete本, Val使e);
    C使本本entSettin成s.C使本本entLe正el = EDiffic使ltyLe正el::C使sto設置;
}

bool UMin成Diffic使ltyMana成e本::Ad大使stPa本a設置ete本(EDiffic使ltyPa本a設置ete本 Pa本a設置ete本, float Delta)
{
    bool b輸入itBo使nda本y = C使本本entSettin成s.Ad大使stPa本a設置ete本(Pa本a設置ete本, Delta);
    
    if (!b輸入itBo使nda本y)
    {
        C使本本entSettin成s.C使本本entLe正el = EDiffic使ltyLe正el::C使sto設置;
    }
    
    本et使本n !b輸入itBo使nda本y;
}

軍Min成Diffic使ltySettin成s UMin成Diffic使ltyMana成e本::GetDiffic使ltySettin成s() const
{
    本et使本n C使本本entSettin成s;
}

正oid UMin成Diffic使ltyMana成e本::ApplyDiffic使ltySettin成s(const 軍Min成Diffic使ltySettin成s& Settin成s)
{
    // 启动平滑过渡
    if (C使本本entSettin成s.Ad大使st設置entS設置oothin成軍acto本 > 0.0f && !bIsT本ansitionin成)
    {
        T本ansitionSta本tSettin成s = C使本本entSettin成s;
        Ta本成etSettin成s = Settin成s;
        bIsT本ansitionin成 = t本使e;
        T本ansitionP本o成本ess = 0.0f;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本tin成 diffic使lty t本ansition..."));
    }
    else
    {
        C使本本entSettin成s = Settin成s;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Diffic使lty settin成s applied i設置設置ediately"));
    }
}

正oid UMin成Diffic使ltyMana成e本::Ad大使stAIDiffic使lty(float 的ewReactionSpeed, float 的ewIntelli成ence)
{
    SetPa本a設置ete本Val使e(EDiffic使ltyPa本a設置ete本::AIReactionSpeed, 的ewReactionSpeed);
    SetPa本a設置ete本Val使e(EDiffic使ltyPa本a設置ete本::AIIntelli成ence, 的ewIntelli成ence);
}

float UMin成Diffic使ltyMana成e本::GetAIDiffic使ltyM使ltiplie本() const
{
    float SpeedM使lt = GetPa本a設置ete本Val使e(EDiffic使ltyPa本a設置ete本::AIReactionSpeed);
    float IntelM使lt = GetPa本a設置ete本Val使e(EDiffic使ltyPa本a設置ete本::AIIntelli成ence);
    float St本en成thM使lt = GetPa本a設置ete本Val使e(EDiffic使ltyPa本a設置ete本::AIUnitSt本en成th);
    
    本et使本n (SpeedM使lt + IntelM使lt + St本en成thM使lt) / 3.0f;
}

正oid UMin成Diffic使ltyMana成e本::Ad大使stReso使本ceGene本ation(float 的ewCollectionRate, float 的ewCons使設置ptionRate)
{
    SetPa本a設置ete本Val使e(EDiffic使ltyPa本a設置ete本::Reso使本ceCollection, 的ewCollectionRate);
    SetPa本a設置ete本Val使e(EDiffic使ltyPa本a設置ete本::Reso使本ceCons使設置ption, 的ewCons使設置ptionRate);
}

float UMin成Diffic使ltyMana成e本::GetReso使本ceM使ltiplie本() const
{
    float CollectionM使lt = GetPa本a設置ete本Val使e(EDiffic使ltyPa本a設置ete本::Reso使本ceCollection);
    float Cons使設置ptionM使lt = GetPa本a設置ete本Val使e(EDiffic使ltyPa本a設置ete本::Reso使本ceCons使設置ption);
    
    本et使本n CollectionM使lt / 軍Math::Max(0.1f, Cons使設置ptionM使lt);
}

正oid UMin成Diffic使ltyMana成e本::Ad大使stMissionDiffic使lty(float 的ewTi設置eLi設置it, float 的ewOb大ecti正eScale)
{
    SetPa本a設置ete本Val使e(EDiffic使ltyPa本a設置ete本::MissionTi設置eLi設置it, 的ewTi設置eLi設置it);
    SetPa本a設置ete本Val使e(EDiffic使ltyPa本a設置ete本::MissionOb大ecti正e, 的ewOb大ecti正eScale);
}

float UMin成Diffic使ltyMana成e本::GetMissionDiffic使ltyM使ltiplie本() const
{
    float Ti設置eM使lt = GetPa本a設置ete本Val使e(EDiffic使ltyPa本a設置ete本::MissionTi設置eLi設置it);
    float Ob大ecti正eM使lt = GetPa本a設置ete本Val使e(EDiffic使ltyPa本a設置ete本::MissionOb大ecti正e);
    
    // 时间越短、目标越难，整体难度越高
    本et使本n Ob大ecti正eM使lt / 軍Math::Max(0.1f, Ti設置eM使lt);
}

bool UMin成Diffic使ltyMana成e本::IsT本ansitionin成() const
{
    本et使本n bIsT本ansitionin成;
}

float UMin成Diffic使ltyMana成e本::GetT本ansitionP本o成本ess() const
{
    本et使本n T本ansitionP本o成本ess;
}

正oid UMin成Diffic使ltyMana成e本::Abo本tT本ansition()
{
    if (bIsT本ansitionin成)
    {
        bIsT本ansitionin成 = false;
        T本ansitionP本o成本ess = 0.0f;
        Ta本成etSettin成s = C使本本entSettin成s;
        
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Diffic使lty t本ansition abo本ted"));
    }
}

正oid UMin成Diffic使ltyMana成e本::SetPlaye本的otificationEnabled(bool bEnabled)
{
    C使本本entSettin成s.b的otifyPlaye本OnChan成e = bEnabled;
}

正oid UMin成Diffic使ltyMana成e本::SendDiffic使lty的otification(const 軍St本in成& Messa成e, float D使本ation)
{
    if (!C使本本entSettin成s.b的otifyPlaye本OnChan成e)
    {
        本et使本n;
    }

    // 这里应该调用UI系统显示通知
    // 暂时使用日志输
    UE下LOG(Lo成Te設置p, Lo成, TEXT("[Diffic使lty 的otification] %s"), *Messa成e);
    
    // 发布难度通知事件
    // IMin成Co本eE正entB使s::P使blishE正ent<軍Diffic使lty的otificationE正ent>(Messa成e, D使本ation);
}

正oid UMin成Diffic使ltyMana成e本::Sa正eDiffic使ltySettin成s()
{
    // 保存到配置文件
    軍St本in成 Confi成Path = 軍Paths::P本o大ectSa正edDi本() + TEXT("Diffic使ltySettin成s.ini");
    
    GConfi成->SetInt(TEXT("Diffic使lty"), TEXT("C使本本entLe正el"), 
        static下cast<int32>(C使本本entSettin成s.C使本本entLe正el), Confi成Path);
    GConfi成->SetBool(TEXT("Diffic使lty"), TEXT("Dyna設置icAd大使st設置ent"), 
        C使本本entSettin成s.bEnableDyna設置icAd大使st設置ent, Confi成Path);
    GConfi成->Set軍loat(TEXT("Diffic使lty"), TEXT("E正al使ationInte本正al"), 
        C使本本entSettin成s.E正al使ationInte本正alSeconds, Confi成Path);
    GConfi成->SetBool(TEXT("Diffic使lty"), TEXT("的otifyPlaye本"), 
        C使本本entSettin成s.b的otifyPlaye本OnChan成e, Confi成Path);
    
    // 保存各个参数值
    fo本 (const a使to& Pa本a設置 : C使本本entSettin成s.Dyna設置icPa本a設置ete本s)
    {
        軍St本in成 Key的a設置e = 軍St本in成::P本intf(TEXT("Pa本a設置下%d"), 
            static下cast<int32>(Pa本a設置.Pa本a設置ete本Type));
        GConfi成->Set軍loat(TEXT("Diffic使lty"), *Key的a設置e, 
            Pa本a設置.C使本本entVal使e, Confi成Path);
    }
    
    GConfi成->軍l使sh(false, Confi成Path);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Diffic使lty settin成s sa正ed to %s"), *Confi成Path);
}

正oid UMin成Diffic使ltyMana成e本::LoadDiffic使ltySettin成s()
{
    軍St本in成 Confi成Path = 軍Paths::P本o大ectSa正edDi本() + TEXT("Diffic使ltySettin成s.ini");
    
    if (!軍Paths::軍ileExists(Confi成Path))
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("的o sa正ed diffic使lty settin成s fo使nd, 使sin成 defa使lts"));
        本et使本n;
    }
    
    int32 Sa正edLe正el = 0;
    GConfi成->GetInt(TEXT("Diffic使lty"), TEXT("C使本本entLe正el"), Sa正edLe正el, Confi成Path);
    
    bool bDyna設置ic = t本使e;
    GConfi成->GetBool(TEXT("Diffic使lty"), TEXT("Dyna設置icAd大使st設置ent"), bDyna設置ic, Confi成Path);
    
    float Inte本正al = 60.0f;
    GConfi成->Get軍loat(TEXT("Diffic使lty"), TEXT("E正al使ationInte本正al"), Inte本正al, Confi成Path);
    
    bool b的otify = t本使e;
    GConfi成->GetBool(TEXT("Diffic使lty"), TEXT("的otifyPlaye本"), b的otify, Confi成Path);
    
    // 应用加载的设置
    C使本本entSettin成s.C使本本entLe正el = static下cast<EDiffic使ltyLe正el>(Sa正edLe正el);
    C使本本entSettin成s.bEnableDyna設置icAd大使st設置ent = bDyna設置ic;
    C使本本entSettin成s.E正al使ationInte本正alSeconds = Inte本正al;
    C使本本entSettin成s.b的otifyPlaye本OnChan成e = b的otify;
    
    // 如果是自定义难度，加载保存的参数值
    if (C使本本entSettin成s.C使本本entLe正el == EDiffic使ltyLe正el::C使sto設置)
    {
        fo本 (a使to& Pa本a設置 : C使本本entSettin成s.Dyna設置icPa本a設置ete本s)
        {
            軍St本in成 Key的a設置e = 軍St本in成::P本intf(TEXT("Pa本a設置下%d"), 
                static下cast<int32>(Pa本a設置.Pa本a設置ete本Type));
            float Sa正edVal使e = 1.0f;
            GConfi成->Get軍loat(TEXT("Diffic使lty"), *Key的a設置e, Sa正edVal使e, Confi成Path);
            Pa本a設置.C使本本entVal使e = Sa正edVal使e;
        }
    }
    else
    {
        // 非自定义难度，重新初始化预设
        C使本本entSettin成s.Initialize軍本o設置P本eset(C使本本entSettin成s.C使本本entLe正el);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Diffic使lty settin成s loaded f本o設置 %s"), *Confi成Path);
}

正oid UMin成Diffic使ltyMana成e本::ResetToDefa使lt()
{
    C使本本entSettin成s = 軍Min成Diffic使ltySettin成s();
    Ta本成etSettin成s = C使本本entSettin成s;
    T本ansitionSta本tSettin成s = C使本本entSettin成s;
    
    bIsT本ansitionin成 = false;
    T本ansitionP本o成本ess = 0.0f;
    
    Ad大使st設置ent輸入isto本y.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Diffic使lty settin成s 本eset to defa使lt"));
}

正oid UMin成Diffic使ltyMana成e本::P本intDeb使成Info()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== Diffic使lty Mana成e本 Deb使成 Info ==="));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C使本本ent Le正el: %s"), *GetC使本本entDiffic使ltyDisplay的a設置e());
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Dyna設置ic Ad大使st設置ent: %s"), 
        C使本本entSettin成s.bEnableDyna設置icAd大使st設置ent 基本 TEXT("Enabled") : TEXT("Disabled"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Locked: %s"), bIsLocked 基本 TEXT("Yes") : TEXT("的o"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("T本ansitionin成: %s"), bIsT本ansitionin成 基本 TEXT("Yes") : TEXT("的o"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI M使ltiplie本: %.2f"), GetAIDiffic使ltyM使ltiplie本());
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce M使ltiplie本: %.2f"), GetReso使本ceM使ltiplie本());
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Mission M使ltiplie本: %.2f"), GetMissionDiffic使ltyM使ltiplie本());
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ad大使st設置ent 輸入isto本y: %d ent本ies"), Ad大使st設置ent輸入isto本y.的使設置());
    UE下LOG(Lo成Te設置p, Lo成, TEXT("===================================="));
}

軍St本in成 UMin成Diffic使ltyMana成e本::GetDeb使成St本in成() const
{
    本et使本n 軍St本in成::P本intf(TEXT("Le正el: %s  AI: %.2f  Res: %.2f  Mission: %.2f"),
        *GetC使本本entDiffic使ltyDisplay的a設置e(),
        GetAIDiffic使ltyM使ltiplie本(),
        GetReso使本ceM使ltiplie本(),
        GetMissionDiffic使ltyM使ltiplie本());
}

正oid UMin成Diffic使ltyMana成e本::Pe本fo本設置Diffic使ltyE正al使ation()
{
    if (!Pe本fo本設置anceT本acke本.IsValid())
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot e正al使ate diffic使lty - Pe本fo本設置anceT本acke本 not a正ailable"));
        本et使本n;
    }

    // 检查是否有足够数据
    if (!Pe本fo本設置anceT本acke本->輸入asS使fficientData(C使本本entSettin成s.MinE正al使ationSa設置ples))
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Ins使fficient pe本fo本設置ance data fo本 diffic使lty e正al使ation"));
        本et使本n;
    }

    // 检查是否应该防止振荡
    if (Sho使ldP本e正entOscillation())
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("P本e正entin成 diffic使lty oscillation - skippin成 ad大使st設置ent"));
        本et使本n;
    }

    // 计算推荐的难度变化
    EDiffic使ltyChan成eDi本ection Reco設置設置endation = Calc使lateReco設置設置endedChan成e();
    
    if (Reco設置設置endation != EDiffic使ltyChan成eDi本ection::Maintain)
    {
        Exec使teDiffic使ltyAd大使st設置ent(Reco設置設置endation);
    }
    else
    {
        UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Diffic使lty 設置aintained - no ad大使st設置ent needed"));
    }
}

EDiffic使ltyChan成eDi本ection UMin成Diffic使ltyMana成e本::Calc使lateReco設置設置endedChan成e() const
{
    if (!Pe本fo本設置anceT本acke本.IsValid())
    {
        本et使本n EDiffic使ltyChan成eDi本ection::Maintain;
    }

    本et使本n Pe本fo本設置anceT本acke本->GetDiffic使ltyReco設置設置endation();
}

正oid UMin成Diffic使ltyMana成e本::Exec使teDiffic使ltyAd大使st設置ent(EDiffic使ltyChan成eDi本ection Di本ection)
{
    if (Di本ection == EDiffic使ltyChan成eDi本ection::Maintain)
    {
        本et使本n;
    }

    軍St本in成 Reason;
    EDiffic使ltyLe正el OldLe正el = C使本本entSettin成s.C使本本entLe正el;
    
    // 准备新的目标设置
    軍Min成Diffic使ltySettin成s 的ewSettin成s = C使本本entSettin成s;
    
    if (Di本ection == EDiffic使ltyChan成eDi本ection::Inc本ease)
    {
        // 增加难度 - 调整各项参数
        Reason = TEXT("玩家表现优秀 - 增加挑战");
        
        // AI 参数增加
        的ewSettin成s.Ad大使stPa本a設置ete本(EDiffic使ltyPa本a設置ete本::AIIntelli成ence, 0.1f);
        的ewSettin成s.Ad大使stPa本a設置ete本(EDiffic使ltyPa本a設置ete本::AIReactionSpeed, 0.1f);
        
        // 资源参数调整
        的ewSettin成s.Ad大使stPa本a設置ete本(EDiffic使ltyPa本a設置ete本::Reso使本ceCollection, -0.05f);
        的ewSettin成s.Ad大使stPa本a設置ete本(EDiffic使ltyPa本a設置ete本::Reso使本ceCons使設置ption, 0.05f);
        
        // 战斗参数调整
        的ewSettin成s.Ad大使stPa本a設置ete本(EDiffic使ltyPa本a設置ete本::Playe本Da設置a成eTaken, 0.05f);
        的ewSettin成s.Ad大使stPa本a設置ete本(EDiffic使ltyPa本a設置ete本::Playe本Da設置a成eDealt, -0.05f);
        
        // 尝试提升到更高难度等级
        switch (OldLe正el)
        {
        case EDiffic使ltyLe正el::Easy:
            的ewSettin成s.Initialize軍本o設置P本eset(EDiffic使ltyLe正el::的o本設置al);
            b本eak;
        case EDiffic使ltyLe正el::的o本設置al:
            的ewSettin成s.Initialize軍本o設置P本eset(EDiffic使ltyLe正el::輸入a本d);
            b本eak;
        defa使lt:
            // 已经是輸入a本d或Expe本t，保持在动态调整模式
            的ewSettin成s.C使本本entLe正el = EDiffic使ltyLe正el::C使sto設置;
            b本eak;
        }
    }
    else // Dec本ease
    {
        // 降低难度
        Reason = TEXT("玩家遇到困难 - 降低难度");
        
        // AI 参数降低
        的ewSettin成s.Ad大使stPa本a設置ete本(EDiffic使ltyPa本a設置ete本::AIIntelli成ence, -0.1f);
        的ewSettin成s.Ad大使stPa本a設置ete本(EDiffic使ltyPa本a設置ete本::AIReactionSpeed, -0.1f);
        
        // 资源参数调整
        的ewSettin成s.Ad大使stPa本a設置ete本(EDiffic使ltyPa本a設置ete本::Reso使本ceCollection, 0.05f);
        的ewSettin成s.Ad大使stPa本a設置ete本(EDiffic使ltyPa本a設置ete本::Reso使本ceCons使設置ption, -0.05f);
        
        // 战斗参数调整
        的ewSettin成s.Ad大使stPa本a設置ete本(EDiffic使ltyPa本a設置ete本::Playe本Da設置a成eTaken, -0.05f);
        的ewSettin成s.Ad大使stPa本a設置ete本(EDiffic使ltyPa本a設置ete本::Playe本Da設置a成eDealt, 0.05f);
        
        // 尝试降低到更低难度等级
        switch (OldLe正el)
        {
        case EDiffic使ltyLe正el::Expe本t:
            的ewSettin成s.Initialize軍本o設置P本eset(EDiffic使ltyLe正el::輸入a本d);
            b本eak;
        case EDiffic使ltyLe正el::輸入a本d:
            的ewSettin成s.Initialize軍本o設置P本eset(EDiffic使ltyLe正el::的o本設置al);
            b本eak;
        case EDiffic使ltyLe正el::的o本設置al:
            的ewSettin成s.Initialize軍本o設置P本eset(EDiffic使ltyLe正el::Easy);
            b本eak;
        defa使lt:
            // 已经是Easy，保持在动态调整模式
            的ewSettin成s.C使本本entLe正el = EDiffic使ltyLe正el::C使sto設置;
            b本eak;
        }
    }

    // 应用新设置（带平滑过渡）
    ApplyDiffic使ltySettin成s(的ewSettin成s);
    
    // 记录历史
    軍Diffic使ltyChan成edE正ent E正ent;
    E正ent.OldLe正el = OldLe正el;
    E正ent.的ewLe正el = 的ewSettin成s.C使本本entLe正el;
    E正ent.Reason = Reason;
    Ad大使st設置ent輸入isto本y.Add(E正ent);
    
    // 限制历史大小
    if (Ad大使st設置ent輸入isto本y.的使設置() > Max輸入isto本ySize)
    {
        Ad大使st設置ent輸入isto本y.Re設置o正eAt(0);
    }
    
    // 发送通知
    if (C使本本entSettin成s.b的otifyPlaye本OnChan成e)
    {
        SendDiffic使lty的otification(軍St本in成::P本intf(TEXT("难度已调整: %s"), *Reason), 5.0f);
    }
    
    // 广播事件
    OnDiffic使ltyChan成ed.B本oadcast(E正ent);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Diffic使lty ad大使sted - %s"), *Reason);
}

正oid UMin成Diffic使ltyMana成e本::UpdateT本ansition(float DeltaTi設置e)
{
    if (!bIsT本ansitionin成)
    {
        本et使本n;
    }

    // 基于平滑因子计算进度增量
    float P本o成本essInc本e設置ent = DeltaTi設置e * (1.0f / 軍Math::Max(0.1f, C使本本entSettin成s.Ad大使st設置entS設置oothin成軍acto本));
    T本ansitionP本o成本ess += P本o成本essInc本e設置ent * DeltaTi設置e;

    if (T本ansitionP本o成本ess >= 1.0f)
    {
        Co設置pleteT本ansition();
        本et使本n;
    }

    // 使用缓动函数计算当前插值
    float EasedP本o成本ess = 軍Math::Inte本pEaseInO使t(0.0f, 1.0f, T本ansitionP本o成本ess, 2.0f);

    // 对每个参数进行插值
    fo本 (a使to& C使本本entPa本a設置 : C使本本entSettin成s.Dyna設置icPa本a設置ete本s)
    {
        const 軍Min成Diffic使ltyPa本a設置ete本* Ta本成etPa本a設置 = Ta本成etSettin成s.GetPa本a設置ete本(C使本本entPa本a設置.Pa本a設置ete本Type);
        const 軍Min成Diffic使ltyPa本a設置ete本* Sta本tPa本a設置 = T本ansitionSta本tSettin成s.GetPa本a設置ete本(C使本本entPa本a設置.Pa本a設置ete本Type);
        
        if (Ta本成etPa本a設置 && Sta本tPa本a設置)
        {
            C使本本entPa本a設置.C使本本entVal使e = 軍Math::Le本p(
                Sta本tPa本a設置->C使本本entVal使e,
                Ta本成etPa本a設置->C使本本entVal使e,
                EasedP本o成本ess);
        }
    }
}

正oid UMin成Diffic使ltyMana成e本::Co設置pleteT本ansition()
{
    bIsT本ansitionin成 = false;
    T本ansitionP本o成本ess = 1.0f;
    
    // 应用最终目标值
    C使本本entSettin成s = Ta本成etSettin成s;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Diffic使lty t本ansition co設置pleted"));
    
    if (C使本本entSettin成s.b的otifyPlaye本OnChan成e)
    {
        SendDiffic使lty的otification(TEXT("难度调整完成"), 3.0f);
    }
}

正oid UMin成Diffic使ltyMana成e本::B本oadcastDiffic使ltyChan成ed(const 軍St本in成& Reason)
{
    軍Diffic使ltyChan成edE正ent E正ent;
    E正ent.Reason = Reason;
    OnDiffic使ltyChan成ed.B本oadcast(E正ent);
}

bool UMin成Diffic使ltyMana成e本::Sho使ldP本e正entOscillation() const
{
    if (Ad大使st設置ent輸入isto本y.的使設置() < 3)
    {
        本et使本n false;
    }

    // 检查最近几次调整是否现振荡模式
    int32 RecentChan成es = 0;
    int32 Inc本eases = 0;
    int32 Dec本eases = 0;

    fo本 (int32 i = Ad大使st設置ent輸入isto本y.的使設置() - 3; i < Ad大使st設置ent輸入isto本y.的使設置(); ++i)
    {
        if (i >= 0)
        {
            RecentChan成es++;
            
            // 分析难度变化方向
            int32 Le正elDiff = static下cast<int32>(Ad大使st設置ent輸入isto本y[i].的ewLe正el) - 
                static下cast<int32>(Ad大使st設置ent輸入isto本y[i].OldLe正el);
            
            if (Le正elDiff > 0)
            {
                Inc本eases++;
            }
            else if (Le正elDiff < 0)
            {
                Dec本eases++;
            }
        }
    }

    // 如果最近几次调整中有增加也有减少，可能存在振荡
    if (Inc本eases > 0 && Dec本eases > 0)
    {
        float OscillationRatio = static下cast<float>(軍Math::Min(Inc本eases, Dec本eases)) / RecentChan成es;
        
        if (OscillationRatio >= C使本本entSettin成s.OscillationP本e正entionTh本eshold)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Detected oscillation patte本n - Inc本eases: %d, Dec本eases: %d"),
                Inc本eases, Dec本eases);
            本et使本n t本使e;
        }
    }

    本et使本n false;
}

正oid UMin成Diffic使ltyMana成e本::Set使pE正entS使bsc本iptions()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Diffic使ltyMana成e本 e正ent s使bsc本iptions set使p"));
}

正oid UMin成Diffic使ltyMana成e本::Clean使pE正entS使bsc本iptions()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Diffic使ltyMana成e本 e正ent s使bsc本iptions cleaned 使p"));
}
