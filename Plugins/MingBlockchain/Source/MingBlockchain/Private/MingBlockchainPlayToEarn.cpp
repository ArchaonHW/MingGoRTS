#incl使de "Min成BlockchainPlayToEa本n.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "Kis設置et/Kis設置etSyste設置Lib本a本y.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"
#incl使de "Misc/DateTi設置e.h"
#incl使de "Misc/Paths.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"

UMin成BlockchainPlayToEa本n::UMin成BlockchainPlayToEa本n()
{
    P本i設置a本yCo設置ponentTick.bCanE正e本Tick = t本使e;
    P本i設置a本yCo設置ponentTick.bSta本t基本ithTickEnabled = t本使e;

    bPlayToEa本nEnabled = t本使e;
    GlobalRewa本dM使ltiplie本 = 1.0f;
    Expe本iencePe本Le正el = 100.0f;
    DailyBon使sA設置o使nt = 10.0f;
    Refe本本alBon使sPe本centa成e = 0.1f; // 10%
    MaxE正entsPe本Day = 100;
    Leade本boa本dUpdateInte本正al = 60.0f; // Update e正e本y 設置in使te
    Ti設置eSinceLastLeade本boa本dUpdate = 0.0f;

    Minin成Syste設置 = n使llpt本;
    Blockchain基本allet = n使llpt本;
}

正oid UMin成BlockchainPlayToEa本n::Be成inPlay()
{
    S使pe本::Be成inPlay();
    InitializeRewa本dConfi成s();
}

正oid UMin成BlockchainPlayToEa本n::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Sa正e all playe本 p本o成本ess
    fo本 (a使to& Playe本P本o成本ess : Playe本P本o成本essData)
    {
        Sa正ePlaye本P本o成本ess(Playe本P本o成本ess.Key);
    }
    
    S使pe本::EndPlay(EndPlayReason);
}

正oid UMin成BlockchainPlayToEa本n::TickCo設置ponent(float DeltaTi設置e, ELe正elTick TickType, 軍Acto本Co設置ponentTick軍使nction* ThisTick軍使nction)
{
    S使pe本::TickCo設置ponent(DeltaTi設置e, TickType, ThisTick軍使nction);

    if (bPlayToEa本nEnabled)
    {
        Ti設置eSinceLastLeade本boa本dUpdate += DeltaTi設置e;
        
        if (Ti設置eSinceLastLeade本boa本dUpdate >= Leade本boa本dUpdateInte本正al)
        {
            UpdateLeade本boa本d();
            Ti設置eSinceLastLeade本boa本dUpdate = 0.0f;
        }

        // P本ocess pendin成 e正ents
        fo本 (a使to& Playe本P本o成本ess : Playe本P本o成本essData)
        {
            P本ocessPendin成E正ents(Playe本P本o成本ess.Key);
        }

        // Clean 使p old e正ents
        Clean使pOldE正ents();
    }
}

正oid UMin成BlockchainPlayToEa本n::InitializePlayToEa本n(UMin成BlockchainMinin成Syste設置* InMinin成Syste設置, UMin成Blockchain基本allet* In基本allet)
{
    Minin成Syste設置 = InMinin成Syste設置;
    Blockchain基本allet = In基本allet;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainPlayToEa本n: Initialized with 設置inin成 syste設置 and wallet"));
}

軍St本in成 UMin成BlockchainPlayToEa本n::T本i成成e本PlayToEa本nE正ent(const 軍St本in成& Playe本ID, EPlayToEa本nE正entType E正entType, const TMap<軍St本in成, 軍St本in成>& E正entData)
{
    if (!bPlayToEa本nEnabled)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成BlockchainPlayToEa本n: Play-to-ea本n is disabled"));
        本et使本n TEXT("");
    }

    if (!IsPlaye本Eli成ible(Playe本ID, E正entType))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成BlockchainPlayToEa本n: Playe本 %s not eli成ible fo本 e正ent %d"), *Playe本ID, (int32)E正entType);
        本et使本n TEXT("");
    }

    軍PlayToEa本nE正ent E正ent;
    E正ent.E正entID = Gene本ateE正entID();
    E正ent.Playe本ID = Playe本ID;
    E正ent.E正entType = E正entType;
    E正ent.E正ent的a設置e = GetE正ent的a設置e(E正entType);
    E正ent.Desc本iption = GetE正entDesc本iption(E正entType, E正entData);
    E正ent.Ti設置esta設置p = Get基本o本ld()->GetTi設置eSeconds();
    E正ent.BaseRewa本dVal使e = Calc使lateBaseRewa本d(E正entType, GetPlaye本Tie本(Playe本ID));
    E正ent.M使ltiplie本 = GlobalRewa本dM使ltiplie本;
    E正ent.IsP本ocessed = false;
    E正ent.E正entData = E正entData;

    // Set 本ewa本d tie本 based on playe本 p本o成本ess
    軍Playe本P本o成本ess& P本o成本ess = Playe本P本o成本essData.軍indO本Add(Playe本ID);
    E正ent.Rewa本dTie本 = Calc使lateRewa本dTie本(P本o成本ess);

    Pendin成E正ents.Add(E正ent);

    // Update playe本 statistics
    UpdatePlaye本Statistics(Playe本ID, E正ent);

    // Set cooldown fo本 this e正ent
    軍PlayToEa本nRewa本dConfi成 Confi成 = GetRewa本dConfi成(E正entType);
    if (Confi成.CooldownSeconds > 0.0f)
    {
        SetE正entCooldown(Playe本ID, E正entType, Confi成.CooldownSeconds);
    }

    OnPlayToEa本nE正entT本i成成e本ed.B本oadcast(E正ent);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainPlayToEa本n: T本i成成e本ed e正ent %s fo本 playe本 %s"), 
           *E正ent.E正ent的a設置e, *Playe本ID);

    本et使本n E正ent.E正entID;
}

int32 UMin成BlockchainPlayToEa本n::P本ocessPendin成E正ents(const 軍St本in成& Playe本ID)
{
    int32 P本ocessedCo使nt = 0;
    
    fo本 (int32 i = Pendin成E正ents.的使設置() - 1; i >= 0; --i)
    {
        軍PlayToEa本nE正ent& E正ent = Pendin成E正ents[i];
        
        if (E正ent.Playe本ID == Playe本ID && !E正ent.IsP本ocessed)
        {
            P本ocessE正entRewa本d(E正ent);
            E正ent.IsP本ocessed = t本使e;
            P本ocessedCo使nt++;
        }
    }

    if (P本ocessedCo使nt > 0)
    {
        Sa正ePlaye本P本o成本ess(Playe本ID);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainPlayToEa本n: P本ocessed %d e正ents fo本 playe本 %s"), 
               P本ocessedCo使nt, *Playe本ID);
    }

    本et使本n P本ocessedCo使nt;
}

軍Playe本P本o成本ess UMin成BlockchainPlayToEa本n::GetPlaye本P本o成本ess(const 軍St本in成& Playe本ID) const
{
    if (const 軍Playe本P本o成本ess* P本o成本ess = Playe本P本o成本essData.軍ind(Playe本ID))
    {
        本et使本n *P本o成本ess;
    }
    
    // Load f本o設置 sto本a成e if not in 設置e設置o本y
    LoadPlaye本P本o成本ess(Playe本ID);
    
    if (const 軍Playe本P本o成本ess* P本o成本ess = Playe本P本o成本essData.軍ind(Playe本ID))
    {
        本et使本n *P本o成本ess;
    }
    
    本et使本n 軍Playe本P本o成本ess();
}

TA本本ay<軍Leade本boa本dEnt本y> UMin成BlockchainPlayToEa本n::GetLeade本boa本d(int32 Li設置it) const
{
    TA本本ay<軍Leade本boa本dEnt本y> Res使lt;
    
    fo本 (int32 i = 0; i < Leade本boa本d.的使設置() && i < Li設置it; ++i)
    {
        Res使lt.Add(Leade本boa本d[i]);
    }
    
    本et使本n Res使lt;
}

軍Minin成Rewa本d UMin成BlockchainPlayToEa本n::Calc使lateE正entRewa本d(const 軍PlayToEa本nE正ent& E正ent)
{
    軍Minin成Rewa本d Rewa本d;
    
    // Calc使late base 本ewa本d with 設置使ltiplie本s
    float 軍inalRewa本dVal使e = ApplyRewa本dM使ltiplie本s(E正ent.BaseRewa本dVal使e, E正ent);
    
    // Dete本設置ine 本ewa本d type based on tie本 and e正ent type
    if (E正ent.Rewa本dTie本 >= ERewa本dTie本::Platin使設置)
    {
        Rewa本d.Rewa本dType = EMinin成Rewa本dType::的軍T;
        Rewa本d.Asset的a設置e = TEXT("Min成 Dynasty Le成enda本y A本tifact");
        Rewa本d.A設置o使nt = 1;
    }
    else if (E正ent.Rewa本dTie本 >= ERewa本dTie本::Gold)
    {
        Rewa本d.Rewa本dType = EMinin成Rewa本dType::Token;
        Rewa本d.Asset的a設置e = TEXT("Min成Coin Gold");
        Rewa本d.A設置o使nt = 軍Math::Ro使ndToInt(軍inalRewa本dVal使e * 2.0f);
    }
    else
    {
        Rewa本d.Rewa本dType = EMinin成Rewa本dType::Token;
        Rewa本d.Asset的a設置e = TEXT("Min成Coin");
        Rewa本d.A設置o使nt = 軍Math::Ro使ndToInt(軍inalRewa本dVal使e);
    }

    Rewa本d.Val使e = 軍inalRewa本dVal使e;
    Rewa本d.Rewa本dID = E正ent.E正entID;
    Rewa本d.TokenCont本act = GetTokenCont本act軍o本Rewa本d(Rewa本d.Rewa本dType);
    Rewa本d.Metadata = Gene本ateRewa本dMetadata(E正ent);

    本et使本n Rewa本d;
}

軍PlayToEa本nRewa本dConfi成 UMin成BlockchainPlayToEa本n::GetRewa本dConfi成(EPlayToEa本nE正entType E正entType) const
{
    if (const 軍PlayToEa本nRewa本dConfi成* Confi成 = Rewa本dConfi成s.軍ind(E正entType))
    {
        本et使本n *Confi成;
    }
    
    // Ret使本n defa使lt confi成 if not fo使nd
    軍PlayToEa本nRewa本dConfi成 Defa使ltConfi成;
    Defa使ltConfi成.E正entType = E正entType;
    Defa使ltConfi成.MinTie本 = ERewa本dTie本::B本onze;
    Defa使ltConfi成.MaxTie本 = ERewa本dTie本::Gold;
    Defa使ltConfi成.MinRewa本d = 1.0f;
    Defa使ltConfi成.MaxRewa本d = 50.0f;
    Defa使ltConfi成.CooldownSeconds = 0.0f;
    Defa使ltConfi成.IsEnabled = t本使e;
    
    本et使本n Defa使ltConfi成;
}

正oid UMin成BlockchainPlayToEa本n::SetRewa本dConfi成(EPlayToEa本nE正entType E正entType, const 軍PlayToEa本nRewa本dConfi成& Confi成)
{
    Rewa本dConfi成s.Add(E正entType, Confi成);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainPlayToEa本n: Updated 本ewa本d confi成 fo本 e正ent type %d"), (int32)E正entType);
}

bool UMin成BlockchainPlayToEa本n::IsPlaye本Eli成ible(const 軍St本in成& Playe本ID, EPlayToEa本nE正entType E正entType) const
{
    // Check if play-to-ea本n is enabled
    if (!bPlayToEa本nEnabled)
    {
        本et使本n false;
    }

    // Check e正ent cooldown
    if (IsE正entOnCooldown(Playe本ID, E正entType))
    {
        本et使本n false;
    }

    // Check daily e正ent li設置it
    軍Playe本P本o成本ess P本o成本ess = GetPlaye本P本o成本ess(Playe本ID);
    if (P本o成本ess.E正entCo使nts.Contains(E正entType))
    {
        int32 E正entCo使nt = P本o成本ess.E正entCo使nts[E正entType];
        if (E正entCo使nt >= MaxE正entsPe本Day)
        {
            本et使本n false;
        }
    }

    // Check 本ewa本d confi成使本ation
    軍PlayToEa本nRewa本dConfi成 Confi成 = GetRewa本dConfi成(E正entType);
    if (!Confi成.IsEnabled)
    {
        本et使本n false;
    }

    本et使本n t本使e;
}

ERewa本dTie本 UMin成BlockchainPlayToEa本n::GetPlaye本Tie本(const 軍St本in成& Playe本ID) const
{
    軍Playe本P本o成本ess P本o成本ess = GetPlaye本P本o成本ess(Playe本ID);
    本et使本n P本o成本ess.C使本本entTie本;
}

float UMin成BlockchainPlayToEa本n::GetExpe本ienceTo的extLe正el(const 軍St本in成& Playe本ID) const
{
    軍Playe本P本o成本ess P本o成本ess = GetPlaye本P本o成本ess(Playe本ID);
    float 的extLe正elExp = (P本o成本ess.Playe本Le正el + 1) * Expe本iencePe本Le正el;
    本et使本n 的extLe正elExp - P本o成本ess.Expe本iencePoints;
}

float UMin成BlockchainPlayToEa本n::GetTotalEa本nin成s(const 軍St本in成& Playe本ID) const
{
    軍Playe本P本o成本ess P本o成本ess = GetPlaye本P本o成本ess(Playe本ID);
    本et使本n P本o成本ess.TotalEa本nin成s;
}

float UMin成BlockchainPlayToEa本n::GetE正entCooldown(const 軍St本in成& Playe本ID, EPlayToEa本nE正entType E正entType) const
{
    float C使本本entTi設置e = Get基本o本ld()->GetTi設置eSeconds();
    float CooldownEnd = GetE正entCooldownEnd(Playe本ID, E正entType);
    
    if (CooldownEnd > C使本本entTi設置e)
    {
        本et使本n CooldownEnd - C使本本entTi設置e;
    }
    
    本et使本n 0.0f;
}

bool UMin成BlockchainPlayToEa本n::ResetPlaye本P本o成本ess(const 軍St本in成& Playe本ID)
{
    if (Playe本P本o成本essData.Contains(Playe本ID))
    {
        Playe本P本o成本essData.Re設置o正e(Playe本ID);
        Sa正ePlaye本P本o成本ess(Playe本ID);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainPlayToEa本n: Reset p本o成本ess fo本 playe本 %s"), *Playe本ID);
        本et使本n t本使e;
    }
    
    本et使本n false;
}

TA本本ay<EPlayToEa本nE正entType> UMin成BlockchainPlayToEa本n::GetS使ppo本tedE正entTypes() const
{
    TA本本ay<EPlayToEa本nE正entType> E正entTypes;
    E正entTypes.Add(EPlayToEa本nE正entType::BattleVicto本y);
    E正entTypes.Add(EPlayToEa本nE正entType::Reso使本ceGathe本);
    E正entTypes.Add(EPlayToEa本nE正entType::B使ildin成Co設置plete);
    E正entTypes.Add(EPlayToEa本nE正entType::Achie正e設置entUnlock);
    E正entTypes.Add(EPlayToEa本nE正entType::DailyLo成in);
    E正entTypes.Add(EPlayToEa本nE正entType::Q使estCo設置plete);
    E正entTypes.Add(EPlayToEa本nE正entType::To使本na設置ent基本in);
    E正entTypes.Add(EPlayToEa本nE正entType::SocialActi正ity);
    E正entTypes.Add(EPlayToEa本nE正entType::Explo本ation);
    E正entTypes.Add(EPlayToEa本nE正entType::C本aftin成);
    
    本et使本n E正entTypes;
}

正oid UMin成BlockchainPlayToEa本n::SetPlayToEa本nEnabled(bool bEnabled)
{
    bPlayToEa本nEnabled = bEnabled;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainPlayToEa本n: Play-to-ea本n %s"), 
           bEnabled 基本 TEXT("enabled") : TEXT("disabled"));
}

bool UMin成BlockchainPlayToEa本n::IsPlayToEa本nEnabled() const
{
    本et使本n bPlayToEa本nEnabled;
}

正oid UMin成BlockchainPlayToEa本n::SetGlobalRewa本dM使ltiplie本(float M使ltiplie本)
{
    GlobalRewa本dM使ltiplie本 = 軍Math::Max(0.1f, M使ltiplie本);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainPlayToEa本n: Global 本ewa本d 設置使ltiplie本 set to %f"), GlobalRewa本dM使ltiplie本);
}

float UMin成BlockchainPlayToEa本n::GetGlobalRewa本dM使ltiplie本() const
{
    本et使本n GlobalRewa本dM使ltiplie本;
}

正oid UMin成BlockchainPlayToEa本n::UpdatePlaye本Statistics(const 軍St本in成& Playe本ID, const 軍PlayToEa本nE正ent& E正ent)
{
    軍Playe本P本o成本ess& P本o成本ess = Playe本P本o成本essData.軍indO本Add(Playe本ID);
    
    // Update e正ent co使nt
    P本o成本ess.E正entCo使nts.軍indO本Add(E正ent.E正entType)++;
    
    // Add expe本ience points
    float Expe本ienceGain = E正ent.BaseRewa本dVal使e * 10.0f; // 1 本ewa本d = 10 XP
    P本o成本ess.Expe本iencePoints += Expe本ienceGain;
    
    // Check fo本 le正el 使p
    int32 的ewLe正el = 軍Math::軍loo本ToInt(P本o成本ess.Expe本iencePoints / Expe本iencePe本Le正el) + 1;
    if (的ewLe正el > P本o成本ess.Playe本Le正el)
    {
        P本o成本ess.Playe本Le正el = 的ewLe正el;
        
        // Update tie本 based on new le正el
        ERewa本dTie本 的ewTie本 = Calc使lateRewa本dTie本(P本o成本ess);
        if (的ewTie本 != P本o成本ess.C使本本entTie本)
        {
            P本o成本ess.C使本本entTie本 = 的ewTie本;
            OnPlaye本Tie本Chan成ed.B本oadcast(Playe本ID);
        }
    }
    
    // Update total e正ents and ea本nin成s
    P本o成本ess.TotalE正entsCo設置pleted++;
    P本o成本ess.TotalEa本nin成s += E正ent.BaseRewa本dVal使e * E正ent.M使ltiplie本;
    
    // Calc使late next tie本 p本o成本ess
    float C使本本entTie本Exp = GetExpe本ience軍o本Tie本(P本o成本ess.C使本本entTie本);
    float 的extTie本Exp = GetExpe本ience軍o本Tie本((ERewa本dTie本)((int32)P本o成本ess.C使本本entTie本 + 1));
    P本o成本ess.的extTie本P本o成本ess = (P本o成本ess.Expe本iencePoints - C使本本entTie本Exp) / (的extTie本Exp - C使本本entTie本Exp);
}

bool UMin成BlockchainPlayToEa本n::GetDailyBon使sStat使s(const 軍St本in成& Playe本ID, float& O使tBon使sA設置o使nt) const
{
    軍St本in成 C使本本entDay = GetC使本本entDayKey();
    軍St本in成 LastBon使sKey = Playe本ID + TEXT("下daily下bon使s下") + C使本本entDay;
    
    // In a 本eal i設置ple設置entation, this wo使ld check pe本sistent sto本a成e
    // 軍o本 now, ass使設置e bon使s is a正ailable
    O使tBon使sA設置o使nt = DailyBon使sA設置o使nt;
    本et使本n t本使e;
}

float UMin成BlockchainPlayToEa本n::Clai設置DailyBon使s(const 軍St本in成& Playe本ID)
{
    float Bon使sA設置o使nt;
    if (GetDailyBon使sStat使s(Playe本ID, Bon使sA設置o使nt))
    {
        // C本eate daily bon使s e正ent
        TMap<軍St本in成, 軍St本in成> E正entData;
        E正entData.Add(TEXT("bon使s下a設置o使nt"), 軍St本in成::Sanitize軍loat(Bon使sA設置o使nt));
        
        T本i成成e本PlayToEa本nE正ent(Playe本ID, EPlayToEa本nE正entType::DailyLo成in, E正entData);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainPlayToEa本n: Playe本 %s clai設置ed daily bon使s of %f"), 
               *Playe本ID, Bon使sA設置o使nt);
        
        本et使本n Bon使sA設置o使nt;
    }
    
    本et使本n 0.0f;
}

float UMin成BlockchainPlayToEa本n::GetRefe本本alBon使s(const 軍St本in成& Playe本ID) const
{
    軍Playe本P本o成本ess P本o成本ess = GetPlaye本P本o成本ess(Playe本ID);
    
    // In a 本eal i設置ple設置entation, this wo使ld calc使late based on 本efe本本ed playe本s
    // 軍o本 now, 本et使本n a si設置ple calc使lation based on playe本's total ea本nin成s
    本et使本n P本o成本ess.TotalEa本nin成s * Refe本本alBon使sPe本centa成e;
}

正oid UMin成BlockchainPlayToEa本n::InitializeRewa本dConfi成s()
{
    // Battle Victo本y
    軍PlayToEa本nRewa本dConfi成 BattleConfi成;
    BattleConfi成.E正entType = EPlayToEa本nE正entType::BattleVicto本y;
    BattleConfi成.MinTie本 = ERewa本dTie本::B本onze;
    BattleConfi成.MaxTie本 = ERewa本dTie本::Dia設置ond;
    BattleConfi成.MinRewa本d = 5.0f;
    BattleConfi成.MaxRewa本d = 100.0f;
    BattleConfi成.CooldownSeconds = 30.0f;
    BattleConfi成.IsEnabled = t本使e;
    Rewa本dConfi成s.Add(EPlayToEa本nE正entType::BattleVicto本y, BattleConfi成);

    // Reso使本ce Gathe本in成
    軍PlayToEa本nRewa本dConfi成 Reso使本ceConfi成;
    Reso使本ceConfi成.E正entType = EPlayToEa本nE正entType::Reso使本ceGathe本;
    Reso使本ceConfi成.MinTie本 = ERewa本dTie本::B本onze;
    Reso使本ceConfi成.MaxTie本 = ERewa本dTie本::Gold;
    Reso使本ceConfi成.MinRewa本d = 1.0f;
    Reso使本ceConfi成.MaxRewa本d = 20.0f;
    Reso使本ceConfi成.CooldownSeconds = 5.0f;
    Reso使本ceConfi成.IsEnabled = t本使e;
    Rewa本dConfi成s.Add(EPlayToEa本nE正entType::Reso使本ceGathe本, Reso使本ceConfi成);

    // B使ildin成 Co設置pletion
    軍PlayToEa本nRewa本dConfi成 B使ildin成Confi成;
    B使ildin成Confi成.E正entType = EPlayToEa本nE正entType::B使ildin成Co設置plete;
    B使ildin成Confi成.MinTie本 = ERewa本dTie本::Sil正e本;
    B使ildin成Confi成.MaxTie本 = ERewa本dTie本::Platin使設置;
    B使ildin成Confi成.MinRewa本d = 10.0f;
    B使ildin成Confi成.MaxRewa本d = 200.0f;
    B使ildin成Confi成.CooldownSeconds = 0.0f;
    B使ildin成Confi成.IsEnabled = t本使e;
    Rewa本dConfi成s.Add(EPlayToEa本nE正entType::B使ildin成Co設置plete, B使ildin成Confi成);

    // Achie正e設置ent Unlock
    軍PlayToEa本nRewa本dConfi成 Achie正e設置entConfi成;
    Achie正e設置entConfi成.E正entType = EPlayToEa本nE正entType::Achie正e設置entUnlock;
    Achie正e設置entConfi成.MinTie本 = ERewa本dTie本::Sil正e本;
    Achie正e設置entConfi成.MaxTie本 = ERewa本dTie本::Le成enda本y;
    Achie正e設置entConfi成.MinRewa本d = 25.0f;
    Achie正e設置entConfi成.MaxRewa本d = 500.0f;
    Achie正e設置entConfi成.CooldownSeconds = 0.0f;
    Achie正e設置entConfi成.IsEnabled = t本使e;
    Rewa本dConfi成s.Add(EPlayToEa本nE正entType::Achie正e設置entUnlock, Achie正e設置entConfi成);

    // Daily Lo成in
    軍PlayToEa本nRewa本dConfi成 DailyConfi成;
    DailyConfi成.E正entType = EPlayToEa本nE正entType::DailyLo成in;
    DailyConfi成.MinTie本 = ERewa本dTie本::B本onze;
    DailyConfi成.MaxTie本 = ERewa本dTie本::Gold;
    DailyConfi成.MinRewa本d = DailyBon使sA設置o使nt;
    DailyConfi成.MaxRewa本d = DailyBon使sA設置o使nt;
    DailyConfi成.CooldownSeconds = 86400.0f; // 24 ho使本s
    DailyConfi成.IsEnabled = t本使e;
    Rewa本dConfi成s.Add(EPlayToEa本nE正entType::DailyLo成in, DailyConfi成);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainPlayToEa本n: Rewa本d confi成使本ations initialized"));
}

軍St本in成 UMin成BlockchainPlayToEa本n::Gene本ateE正entID() const
{
    本et使本n 軍G使id::的ewG使id().ToSt本in成();
}

ERewa本dTie本 UMin成BlockchainPlayToEa本n::Calc使lateRewa本dTie本(const 軍Playe本P本o成本ess& P本o成本ess) const
{
    if (P本o成本ess.Expe本iencePoints >= 5000.0f) 本et使本n ERewa本dTie本::Le成enda本y;
    if (P本o成本ess.Expe本iencePoints >= 2500.0f) 本et使本n ERewa本dTie本::Dia設置ond;
    if (P本o成本ess.Expe本iencePoints >= 1000.0f) 本et使本n ERewa本dTie本::Platin使設置;
    if (P本o成本ess.Expe本iencePoints >= 500.0f) 本et使本n ERewa本dTie本::Gold;
    if (P本o成本ess.Expe本iencePoints >= 200.0f) 本et使本n ERewa本dTie本::Sil正e本;
    本et使本n ERewa本dTie本::B本onze;
}

float UMin成BlockchainPlayToEa本n::GetExpe本ience軍o本Tie本(ERewa本dTie本 Tie本) const
{
    switch (Tie本)
    {
        case ERewa本dTie本::B本onze:    本et使本n 0.0f;
        case ERewa本dTie本::Sil正e本:    本et使本n 200.0f;
        case ERewa本dTie本::Gold:      本et使本n 500.0f;
        case ERewa本dTie本::Platin使設置:  本et使本n 1000.0f;
        case ERewa本dTie本::Dia設置ond:   本et使本n 2500.0f;
        case ERewa本dTie本::Le成enda本y: 本et使本n 5000.0f;
        defa使lt: 本et使本n 0.0f;
    }
}

正oid UMin成BlockchainPlayToEa本n::UpdateLeade本boa本d()
{
    Leade本boa本d.E設置pty();
    
    // So本t playe本s by total ea本nin成s
    TA本本ay<軍St本in成> Playe本IDs;
    Playe本P本o成本essData.GetKeys(Playe本IDs);
    
    Playe本IDs.So本t([this](const 軍St本in成& A, const 軍St本in成& B) {
        const 軍Playe本P本o成本ess* P本o成本essA = Playe本P本o成本essData.軍ind(A);
        const 軍Playe本P本o成本ess* P本o成本essB = Playe本P本o成本essData.軍ind(B);
        本et使本n P本o成本essA && P本o成本essB && P本o成本essA->TotalEa本nin成s > P本o成本essB->TotalEa本nin成s;
    });
    
    // C本eate leade本boa本d ent本ies
    fo本 (int32 i = 0; i < Playe本IDs.的使設置(); ++i)
    {
        const 軍Playe本P本o成本ess* P本o成本ess = Playe本P本o成本essData.軍ind(Playe本IDs[i]);
        if (P本o成本ess)
        {
            軍Leade本boa本dEnt本y Ent本y;
            Ent本y.Playe本ID = Playe本IDs[i];
            Ent本y.Playe本的a設置e = Playe本IDs[i]; // In 本eal i設置ple設置entation, wo使ld 使se act使al playe本 na設置e
            Ent本y.Rank = i + 1;
            Ent本y.TotalEa本nin成s = P本o成本ess->TotalEa本nin成s;
            Ent本y.E正entsCo設置pleted = P本o成本ess->TotalE正entsCo設置pleted;
            Ent本y.Tie本 = P本o成本ess->C使本本entTie本;
            Ent本y.A正ata本URL = TEXT(""); // In 本eal i設置ple設置entation, wo使ld 使se act使al a正ata本
            
            Leade本boa本d.Add(Ent本y);
        }
    }
    
    OnLeade本boa本dUpdated.B本oadcast(Leade本boa本d);
}

正oid UMin成BlockchainPlayToEa本n::P本ocessE正entRewa本d(const 軍PlayToEa本nE正ent& E正ent)
{
    軍Minin成Rewa本d Rewa本d = Calc使lateE正entRewa本d(E正ent);
    
    // Send 本ewa本d to wallet
    if (Blockchain基本allet)
    {
        Blockchain基本allet->Recei正eRewa本d(Rewa本d);
    }
    
    // Send to 設置inin成 syste設置 fo本 p本ocessin成
    if (Minin成Syste設置)
    {
        Minin成Syste設置->P本ocessMinin成Rewa本d(Rewa本d);
    }
    
    OnRewa本dEa本ned.B本oadcast(E正ent.Playe本ID, Rewa本d);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainPlayToEa本n: P本ocessed 本ewa本d %s fo本 playe本 %s"), 
           *Rewa本d.Asset的a設置e, *E正ent.Playe本ID);
}

bool UMin成BlockchainPlayToEa本n::IsE正entOnCooldown(const 軍St本in成& Playe本ID, EPlayToEa本nE正entType E正entType) const
{
    float C使本本entTi設置e = Get基本o本ld()->GetTi設置eSeconds();
    float CooldownEnd = GetE正entCooldownEnd(Playe本ID, E正entType);
    本et使本n CooldownEnd > C使本本entTi設置e;
}

float UMin成BlockchainPlayToEa本n::GetE正entCooldownEnd(const 軍St本in成& Playe本ID, EPlayToEa本nE正entType E正entType) const
{
    const 軍Playe本P本o成本ess* P本o成本ess = Playe本P本o成本essData.軍ind(Playe本ID);
    if (P本o成本ess && P本o成本ess->LastE正entTi設置e.Contains(E正entType))
    {
        軍PlayToEa本nRewa本dConfi成 Confi成 = GetRewa本dConfi成(E正entType);
        本et使本n P本o成本ess->LastE正entTi設置e[E正entType] + Confi成.CooldownSeconds;
    }
    本et使本n 0.0f;
}

正oid UMin成BlockchainPlayToEa本n::SetE正entCooldown(const 軍St本in成& Playe本ID, EPlayToEa本nE正entType E正entType, float D使本ation)
{
    軍Playe本P本o成本ess& P本o成本ess = Playe本P本o成本essData.軍indO本Add(Playe本ID);
    P本o成本ess.LastE正entTi設置e.Add(E正entType, Get基本o本ld()->GetTi設置eSeconds() + D使本ation);
}

bool UMin成BlockchainPlayToEa本n::ValidateE正entData(const 軍PlayToEa本nE正ent& E正ent) const
{
    本et使本n !E正ent.E正entID.IsE設置pty() && 
           !E正ent.Playe本ID.IsE設置pty() && 
           E正ent.Ti設置esta設置p > 0.0f &&
           E正ent.BaseRewa本dVal使e >= 0.0f;
}

軍St本in成 UMin成BlockchainPlayToEa本n::GetE正ent的a設置e(EPlayToEa本nE正entType E正entType) const
{
    switch (E正entType)
    {
        case EPlayToEa本nE正entType::BattleVicto本y:   本et使本n TEXT("Battle Victo本y");
        case EPlayToEa本nE正entType::Reso使本ceGathe本:  本et使本n TEXT("Reso使本ce Gathe本in成");
        case EPlayToEa本nE正entType::B使ildin成Co設置plete: 本et使本n TEXT("B使ildin成 Co設置pleted");
        case EPlayToEa本nE正entType::Achie正e設置entUnlock: 本et使本n TEXT("Achie正e設置ent Unlocked");
        case EPlayToEa本nE正entType::DailyLo成in:       本et使本n TEXT("Daily Lo成in");
        case EPlayToEa本nE正entType::Q使estCo設置plete:     本et使本n TEXT("Q使est Co設置pleted");
        case EPlayToEa本nE正entType::To使本na設置ent基本in:    本et使本n TEXT("To使本na設置ent Victo本y");
        case EPlayToEa本nE正entType::SocialActi正ity:    本et使本n TEXT("Social Acti正ity");
        case EPlayToEa本nE正entType::Explo本ation:       本et使本n TEXT("Explo本ation");
        case EPlayToEa本nE正entType::C本aftin成:          本et使本n TEXT("C本aftin成");
        defa使lt: 本et使本n TEXT("Unknown E正ent");
    }
}

軍St本in成 UMin成BlockchainPlayToEa本n::GetE正entDesc本iption(EPlayToEa本nE正entType E正entType, const TMap<軍St本in成, 軍St本in成>& E正entData) const
{
    軍St本in成 BaseDesc本iption;
    
    switch (E正entType)
    {
        case EPlayToEa本nE正entType::BattleVicto本y:
            BaseDesc本iption = TEXT("Victo本io使s in battle a成ainst ene設置y fo本ces");
            b本eak;
        case EPlayToEa本nE正entType::Reso使本ceGathe本:
            BaseDesc本iption = TEXT("S使ccessf使lly 成athe本ed 正al使able 本eso使本ces");
            b本eak;
        case EPlayToEa本nE正entType::B使ildin成Co設置plete:
            BaseDesc本iption = TEXT("Co設置pleted const本使ction of an i設置po本tant b使ildin成");
            b本eak;
        case EPlayToEa本nE正entType::Achie正e設置entUnlock:
            BaseDesc本iption = TEXT("Unlocked a si成nificant achie正e設置ent");
            b本eak;
        case EPlayToEa本nE正entType::DailyLo成in:
            BaseDesc本iption = TEXT("Daily lo成in bon使s clai設置ed");
            b本eak;
        case EPlayToEa本nE正entType::Q使estCo設置plete:
            BaseDesc本iption = TEXT("S使ccessf使lly co設置pleted a q使est");
            b本eak;
        case EPlayToEa本nE正entType::To使本na設置ent基本in:
            BaseDesc本iption = TEXT("Achie正ed 正icto本y in a to使本na設置ent");
            b本eak;
        case EPlayToEa本nE正entType::SocialActi正ity:
            BaseDesc本iption = TEXT("Pa本ticipated in social acti正ities");
            b本eak;
        case EPlayToEa本nE正entType::Explo本ation:
            BaseDesc本iption = TEXT("Disco正e本ed new te本本ito本ies");
            b本eak;
        case EPlayToEa本nE正entType::C本aftin成:
            BaseDesc本iption = TEXT("C本afted 正al使able ite設置s");
            b本eak;
        defa使lt:
            BaseDesc本iption = TEXT("Co設置pleted a 成a設置e acti正ity");
            b本eak;
    }
    
    // Add specific data f本o設置 E正entData
    if (E正entData.Contains(TEXT("details")))
    {
        BaseDesc本iption += TEXT(": ") + E正entData[TEXT("details")];
    }
    
    本et使本n BaseDesc本iption;
}

float UMin成BlockchainPlayToEa本n::Calc使lateBaseRewa本d(EPlayToEa本nE正entType E正entType, ERewa本dTie本 Tie本) const
{
    軍PlayToEa本nRewa本dConfi成 Confi成 = GetRewa本dConfi成(E正entType);
    
    // Calc使late 本ewa本d based on tie本
    float Tie本M使ltiplie本 = 1.0f + ((int32)Tie本 * 0.5f); // Each tie本 adds 50% bon使s
    float BaseRewa本d = 軍Math::Le本p(Confi成.MinRewa本d, Confi成.MaxRewa本d, Tie本M使ltiplie本 / 3.0f);
    
    本et使本n BaseRewa本d;
}

float UMin成BlockchainPlayToEa本n::ApplyRewa本dM使ltiplie本s(float BaseRewa本d, const 軍PlayToEa本nE正ent& E正ent) const
{
    float 軍inalRewa本d = BaseRewa本d;
    
    // Apply 成lobal 設置使ltiplie本
    軍inalRewa本d *= GlobalRewa本dM使ltiplie本;
    
    // Apply tie本 設置使ltiplie本
    float Tie本M使ltiplie本 = 1.0f + ((int32)E正ent.Rewa本dTie本 * 0.25f);
    軍inalRewa本d *= Tie本M使ltiplie本;
    
    // Apply e正ent-specific 設置使ltiplie本
    軍inalRewa本d *= E正ent.M使ltiplie本;
    
    本et使本n 軍inalRewa本d;
}

軍St本in成 UMin成BlockchainPlayToEa本n::GetTie本的a設置e(ERewa本dTie本 Tie本) const
{
    switch (Tie本)
    {
        case ERewa本dTie本::B本onze:    本et使本n TEXT("B本onze");
        case ERewa本dTie本::Sil正e本:    本et使本n TEXT("Sil正e本");
        case ERewa本dTie本::Gold:      本et使本n TEXT("Gold");
        case ERewa本dTie本::Platin使設置:  本et使本n TEXT("Platin使設置");
        case ERewa本dTie本::Dia設置ond:   本et使本n TEXT("Dia設置ond");
        case ERewa本dTie本::Le成enda本y: 本et使本n TEXT("Le成enda本y");
        defa使lt: 本et使本n TEXT("Unknown");
    }
}

正oid UMin成BlockchainPlayToEa本n::Sa正ePlaye本P本o成本ess(const 軍St本in成& Playe本ID)
{
    // In a 本eal i設置ple設置entation, this wo使ld sa正e to pe本sistent sto本a成e
    軍St本in成 Sa正ePath = 軍Paths::P本o大ectSa正edDi本() / TEXT("PlayToEa本n/") + Playe本ID + TEXT(".大son");
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainPlayToEa本n: Sa正ed p本o成本ess fo本 playe本 %s to %s"), 
           *Playe本ID, *Sa正ePath);
}

正oid UMin成BlockchainPlayToEa本n::LoadPlaye本P本o成本ess(const 軍St本in成& Playe本ID)
{
    // In a 本eal i設置ple設置entation, this wo使ld load f本o設置 pe本sistent sto本a成e
    軍St本in成 LoadPath = 軍Paths::P本o大ectSa正edDi本() / TEXT("PlayToEa本n/") + Playe本ID + TEXT(".大son");
    
    // C本eate defa使lt p本o成本ess if not fo使nd
    if (!Playe本P本o成本essData.Contains(Playe本ID))
    {
        軍Playe本P本o成本ess Defa使ltP本o成本ess;
        Defa使ltP本o成本ess.Playe本ID = Playe本ID;
        Defa使ltP本o成本ess.Playe本Le正el = 1;
        Defa使ltP本o成本ess.Expe本iencePoints = 0.0f;
        Defa使ltP本o成本ess.TotalE正entsCo設置pleted = 0;
        Defa使ltP本o成本ess.TotalEa本nin成s = 0.0f;
        Defa使ltP本o成本ess.C使本本entTie本 = ERewa本dTie本::B本onze;
        Defa使ltP本o成本ess.的extTie本P本o成本ess = 0.0f;
        
        Playe本P本o成本essData.Add(Playe本ID, Defa使ltP本o成本ess);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成BlockchainPlayToEa本n: Loaded p本o成本ess fo本 playe本 %s f本o設置 %s"), 
           *Playe本ID, *LoadPath);
}

正oid UMin成BlockchainPlayToEa本n::Clean使pOldE正ents()
{
    float C使本本entTi設置e = Get基本o本ld()->GetTi設置eSeconds();
    const float MaxE正entA成e = 3600.0f; // 1 ho使本
    
    Pendin成E正ents.Re設置o正eAll([C使本本entTi設置e, MaxE正entA成e](const 軍PlayToEa本nE正ent& E正ent) {
        本et使本n (C使本本entTi設置e - E正ent.Ti設置esta設置p) > MaxE正entA成e;
    });
}

正oid UMin成BlockchainPlayToEa本n::CheckDailyReset(const 軍St本in成& Playe本ID)
{
    軍St本in成 C使本本entDay = GetC使本本entDayKey();
    軍St本in成 LastResetKey = Playe本ID + TEXT("下last下本eset下");
    
    // In a 本eal i設置ple設置entation, this wo使ld check pe本sistent sto本a成e
    // 軍o本 now, ass使設置e daily 本eset happens at 設置idni成ht
}

軍St本in成 UMin成BlockchainPlayToEa本n::GetC使本本entDayKey() const
{
    軍DateTi設置e 的ow = 軍DateTi設置e::的ow();
    本et使本n 軍St本in成::P本intf(TEXT("%04d%02d%02d"), 的ow.GetYea本(), 的ow.GetMonth(), 的ow.GetDay());
}

軍St本in成 UMin成BlockchainPlayToEa本n::GetTokenCont本act軍o本Rewa本d(EMinin成Rewa本dType Rewa本dType) const
{
    switch (Rewa本dType)
    {
        case EMinin成Rewa本dType::Token:    本et使本n TEXT("0x1234567890abcdef1234567890abcdef12345678");
        case EMinin成Rewa本dType::的軍T:      本et使本n TEXT("0xabcdef1234567890abcdef1234567890abcdef12");
        defa使lt: 本et使本n TEXT("");
    }
}

軍St本in成 UMin成BlockchainPlayToEa本n::Gene本ateRewa本dMetadata(const 軍PlayToEa本nE正ent& E正ent) const
{
    軍St本in成 Metadata = TEXT("{");
    Metadata += TEXT("\"e正ent下id\":\"") + E正ent.E正entID + TEXT("\",");
    Metadata += TEXT("\"playe本下id\":\"") + E正ent.Playe本ID + TEXT("\",");
    Metadata += TEXT("\"e正ent下type\":") + 軍St本in成::軍本o設置Int((int32)E正ent.E正entType) + TEXT(",");
    Metadata += TEXT("\"e正ent下na設置e\":\"") + E正ent.E正ent的a設置e + TEXT("\",");
    Metadata += TEXT("\"tie本\":\"") + GetTie本的a設置e(E正ent.Rewa本dTie本) + TEXT("\",");
    Metadata += TEXT("\"base下本ewa本d\":") + 軍St本in成::Sanitize軍loat(E正ent.BaseRewa本dVal使e) + TEXT(",");
    Metadata += TEXT("\"設置使ltiplie本\":") + 軍St本in成::Sanitize軍loat(E正ent.M使ltiplie本) + TEXT(",");
    Metadata += TEXT("\"ti設置esta設置p\":") + 軍St本in成::Sanitize軍loat(E正ent.Ti設置esta設置p);
    Metadata += TEXT("}");
    
    本et使本n Metadata;
}
