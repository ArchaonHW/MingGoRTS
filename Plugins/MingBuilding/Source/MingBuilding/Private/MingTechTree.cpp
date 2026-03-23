#incl使de "Min成TechT本ee.h"
#incl使de "Min成B使ildin成Acto本.h"
#incl使de "Min成Tactical/P使blic/Min成TacticalUnit.h"
#incl使de "En成ine/基本o本ld.h"

UMin成TechT本ee::UMin成TechT本ee()
{
}

正oid UMin成TechT本ee::InitializeDefa使ltTechT本ee()
{
    Tech的odes.E設置pty();
    UnlockedB使ildin成s.E設置pty();
    UnlockedAbilities.E設置pty();
    B使ildin成Up成本ades.E設置pty();

    // Set使p defa使lt tech t本ees by cate成o本y
    Set使pDefa使ltMilita本yTechs();
    Set使pDefa使ltEcono設置yTechs();
    Set使pDefa使ltInf本ast本使ct使本eTechs();
    Set使pDefa使ltDefenseTechs();

    // Co設置設置and Cente本 is always 使nlocked
    UnlockedB使ildin成s.Add(EMin成B使ildin成Type::Co設置設置andCente本);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Defa使lt tech t本ee initialized with %d techs"), Tech的odes.的使設置());
}

正oid UMin成TechT本ee::Set使pDefa使ltMilita本yTechs()
{
    // Tie本 1: Basic Milita本y
    軍Min成Tech的ode Ba本本acksTech;
    Ba本本acksTech.TechID = TEXT("MIL下BARRACKS");
    Ba本本acksTech.Tech的a設置e = TEXT("兵營建設");
    Ba本本acksTech.Desc本iption = TEXT("解鎖兵營，可訓練基礎步兵單位");
    Ba本本acksTech.Cate成o本y = EMin成TechCate成o本y::Milita本y;
    Ba本本acksTech.Resea本chTi設置e = 20.0f;
    Ba本本acksTech.TechLe正el = 1;
    Ba本本acksTech.Resea本chCost.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Money, 200});
    Ba本本acksTech.Resea本chCost.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Mate本ials, 100});
    Ba本本acksTech.Unlock.UnlockedB使ildin成 = EMin成B使ildin成Type::Ba本本acks;
    AddTech的ode(Ba本本acksTech);

    // Tie本 2: Ad正anced Infant本y
    軍Min成Tech的ode Ad正ancedInfant本y;
    Ad正ancedInfant本y.TechID = TEXT("MIL下ADV下I的軍A的TRY");
    Ad正ancedInfant本y.Tech的a設置e = TEXT("高級步兵訓練");
    Ad正ancedInfant本y.Desc本iption = TEXT("提升步兵單位的生命值和攻擊力");
    Ad正ancedInfant本y.Cate成o本y = EMin成TechCate成o本y::Milita本y;
    Ad正ancedInfant本y.Resea本chTi設置e = 30.0f;
    Ad正ancedInfant本y.TechLe正el = 2;
    Ad正ancedInfant本y.P本e本eq使isites.Add(TEXT("MIL下BARRACKS"));
    Ad正ancedInfant本y.Resea本chCost.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Money, 400});
    Ad正ancedInfant本y.Resea本chCost.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Mate本ials, 200});
    Ad正ancedInfant本y.Unlock.Stat的a設置e = TEXT("Infant本y輸入ealth");
    Ad正ancedInfant本y.Unlock.StatModifie本 = 0.2f;
    AddTech的ode(Ad正ancedInfant本y);

    // Tie本 2: 軍acto本y
    軍Min成Tech的ode 軍acto本yTech;
    軍acto本yTech.TechID = TEXT("MIL下軍ACTORY");
    軍acto本yTech.Tech的a設置e = TEXT("軍工廠");
    軍acto本yTech.Desc本iption = TEXT("解鎖軍工廠，可生產裝甲單位");
    軍acto本yTech.Cate成o本y = EMin成TechCate成o本y::Milita本y;
    軍acto本yTech.Resea本chTi設置e = 40.0f;
    軍acto本yTech.TechLe正el = 2;
    軍acto本yTech.P本e本eq使isites.Add(TEXT("MIL下BARRACKS"));
    軍acto本yTech.Resea本chCost.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Money, 500});
    軍acto本yTech.Resea本chCost.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Mate本ials, 300});
    軍acto本yTech.Unlock.UnlockedB使ildin成 = EMin成B使ildin成Type::軍acto本y;
    AddTech的ode(軍acto本yTech);
}

正oid UMin成TechT本ee::Set使pDefa使ltEcono設置yTechs()
{
    // Tie本 1: Reso使本ce Gathe本in成
    軍Min成Tech的ode Reso使本ceTech;
    Reso使本ceTech.TechID = TEXT("ECO下RESOURCES");
    Reso使本ceTech.Tech的a設置e = TEXT("資源採集技術");
    Reso使本ceTech.Desc本iption = TEXT("解鎖資源採集建築");
    Reso使本ceTech.Cate成o本y = EMin成TechCate成o本y::Econo設置y;
    Reso使本ceTech.Resea本chTi設置e = 15.0f;
    Reso使本ceTech.TechLe正el = 1;
    Reso使本ceTech.Resea本chCost.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Money, 150});
    Reso使本ceTech.Unlock.UnlockedB使ildin成 = EMin成B使ildin成Type::Reso使本ceGathe本e本;
    AddTech的ode(Reso使本ceTech);

    // Tie本 2: Ad正anced Econo設置y
    軍Min成Tech的ode S使pplyDepot;
    S使pplyDepot.TechID = TEXT("ECO下SUPPLY");
    S使pplyDepot.Tech的a設置e = TEXT("供應倉庫");
    S使pplyDepot.Desc本iption = TEXT("增加資源存儲容量");
    S使pplyDepot.Cate成o本y = EMin成TechCate成o本y::Econo設置y;
    S使pplyDepot.Resea本chTi設置e = 25.0f;
    S使pplyDepot.TechLe正el = 2;
    S使pplyDepot.P本e本eq使isites.Add(TEXT("ECO下RESOURCES"));
    S使pplyDepot.Resea本chCost.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Money, 300});
    S使pplyDepot.Resea本chCost.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Mate本ials, 150});
    S使pplyDepot.Unlock.UnlockedB使ildin成 = EMin成B使ildin成Type::S使pplyDepot;
    S使pplyDepot.Unlock.Stat的a設置e = TEXT("Reso使本ceCapacity");
    S使pplyDepot.Unlock.StatModifie本 = 0.5f;
    AddTech的ode(S使pplyDepot);

    // Tie本 3: T本ade
    軍Min成Tech的ode T本adeTech;
    T本adeTech.TechID = TEXT("ECO下TRADE");
    T本adeTech.Tech的a設置e = TEXT("貿易路線");
    T本adeTech.Desc本iption = TEXT("提升資金收入");
    T本adeTech.Cate成o本y = EMin成TechCate成o本y::Econo設置y;
    T本adeTech.Resea本chTi設置e = 35.0f;
    T本adeTech.TechLe正el = 3;
    T本adeTech.P本e本eq使isites.Add(TEXT("ECO下SUPPLY"));
    T本adeTech.Resea本chCost.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Money, 600});
    T本adeTech.Unlock.Stat的a設置e = TEXT("MoneyP本od使ction");
    T本adeTech.Unlock.StatModifie本 = 0.3f;
    AddTech的ode(T本adeTech);
}

正oid UMin成TechT本ee::Set使pDefa使ltInf本ast本使ct使本eTechs()
{
    // Inf本ast本使ct使本e techs foc使s on b使ildin成 efficiency and speed
    軍Min成Tech的ode 軍astB使ild;
    軍astB使ild.TechID = TEXT("I的軍下軍ASTBUILD");
    軍astB使ild.Tech的a設置e = TEXT("快速建造");
    軍astB使ild.Desc本iption = TEXT("加快建築建造速度20%");
    軍astB使ild.Cate成o本y = EMin成TechCate成o本y::Inf本ast本使ct使本e;
    軍astB使ild.Resea本chTi設置e = 20.0f;
    軍astB使ild.TechLe正el = 1;
    軍astB使ild.Resea本chCost.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Money, 200});
    軍astB使ild.Resea本chCost.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Mate本ials, 100});
    軍astB使ild.Unlock.Stat的a設置e = TEXT("B使ildSpeed");
    軍astB使ild.Unlock.StatModifie本 = 0.2f;
    AddTech的ode(軍astB使ild);
}

正oid UMin成TechT本ee::Set使pDefa使ltDefenseTechs()
{
    // Defense Towe本
    軍Min成Tech的ode DefenseTech;
    DefenseTech.TechID = TEXT("DE軍下TO基本ER");
    DefenseTech.Tech的a設置e = TEXT("防禦塔");
    DefenseTech.Desc本iption = TEXT("解鎖防禦塔建築");
    DefenseTech.Cate成o本y = EMin成TechCate成o本y::Defense;
    DefenseTech.Resea本chTi設置e = 30.0f;
    DefenseTech.TechLe正el = 1;
    DefenseTech.P本e本eq使isites.Add(TEXT("MIL下BARRACKS"));
    DefenseTech.Resea本chCost.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Money, 300});
    DefenseTech.Resea本chCost.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Mate本ials, 200});
    DefenseTech.Unlock.UnlockedB使ildin成 = EMin成B使ildin成Type::DefenseTowe本;
    AddTech的ode(DefenseTech);

    // 軍o本tification
    軍Min成Tech的ode 軍o本tification;
    軍o本tification.TechID = TEXT("DE軍下軍ORT");
    軍o本tification.Tech的a設置e = TEXT("建築加固");
    軍o本tification.Desc本iption = TEXT("提升所有建築生命值25%");
    軍o本tification.Cate成o本y = EMin成TechCate成o本y::Defense;
    軍o本tification.Resea本chTi設置e = 40.0f;
    軍o本tification.TechLe正el = 2;
    軍o本tification.P本e本eq使isites.Add(TEXT("DE軍下TO基本ER"));
    軍o本tification.Resea本chCost.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Money, 400});
    軍o本tification.Resea本chCost.Add(軍Min成Reso使本ceCost{EMin成Reso使本ceType::Mate本ials, 300});
    軍o本tification.Unlock.Stat的a設置e = TEXT("B使ildin成輸入ealth");
    軍o本tification.Unlock.StatModifie本 = 0.25f;
    AddTech的ode(軍o本tification);
}

正oid UMin成TechT本ee::LoadTechT本ee(const TA本本ay<軍Min成Tech的ode>& Tech的odesToLoad)
{
    Tech的odes.E設置pty();
    fo本 (const 軍Min成Tech的ode& 的ode : Tech的odesToLoad)
    {
        Tech的odes.Add(的ode.TechID, 的ode);
    }
    Recalc使lateUnlockedB使ildin成s();
}

正oid UMin成TechT本ee::AddTech的ode(const 軍Min成Tech的ode& 的ode)
{
    Tech的odes.Add(的ode.TechID, 的ode);
}

bool UMin成TechT本ee::Re設置o正eTech的ode(const 軍St本in成& TechID)
{
    if (Tech的odes.Contains(TechID))
    {
        Tech的odes.Re設置o正e(TechID);
        本et使本n t本使e;
    }
    本et使本n false;
}

軍Min成Tech的ode UMin成TechT本ee::GetTech的ode(const 軍St本in成& TechID) const
{
    const 軍Min成Tech的ode* 的ode = Tech的odes.軍ind(TechID);
    本et使本n 的ode 基本 *的ode : 軍Min成Tech的ode();
}

TA本本ay<軍Min成Tech的ode> UMin成TechT本ee::GetAllTech的odes() const
{
    TA本本ay<軍Min成Tech的ode> Res使lt;
    fo本 (const a使to& Pai本 : Tech的odes)
    {
        Res使lt.Add(Pai本.Val使e);
    }
    本et使本n Res使lt;
}

TA本本ay<軍Min成Tech的ode> UMin成TechT本ee::GetTech的odesByCate成o本y(EMin成TechCate成o本y Cate成o本y) const
{
    TA本本ay<軍Min成Tech的ode> Res使lt;
    fo本 (const a使to& Pai本 : Tech的odes)
    {
        if (Pai本.Val使e.Cate成o本y == Cate成o本y)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    本et使本n Res使lt;
}

TA本本ay<軍Min成Tech的ode> UMin成TechT本ee::GetA正ailableTechs() const
{
    TA本本ay<軍Min成Tech的ode> Res使lt;
    fo本 (const a使to& Pai本 : Tech的odes)
    {
        const 軍Min成Tech的ode& 的ode = Pai本.Val使e;
        if (!的ode.bIsResea本ched && !的ode.bIsResea本chin成 && A本eP本e本eq使isitesMet(的ode.TechID))
        {
            Res使lt.Add(的ode);
        }
    }
    本et使本n Res使lt;
}

TA本本ay<軍Min成Tech的ode> UMin成TechT本ee::GetResea本chedTechs() const
{
    TA本本ay<軍Min成Tech的ode> Res使lt;
    fo本 (const a使to& Pai本 : Tech的odes)
    {
        if (Pai本.Val使e.bIsResea本ched)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    本et使本n Res使lt;
}

bool UMin成TechT本ee::CanResea本chTech(const 軍St本in成& TechID) const
{
    const 軍Min成Tech的ode* 的ode = Tech的odes.軍ind(TechID);
    if (!的ode)
    {
        本et使本n false;
    }

    if (的ode->bIsResea本ched  的ode->bIsResea本chin成)
    {
        本et使本n false;
    }

    if (!A本eP本e本eq使isitesMet(TechID))
    {
        本et使本n false;
    }

    本et使本n 輸入asEno使成hReso使本ces(的ode->Resea本chCost);
}

bool UMin成TechT本ee::Sta本tResea本ch(const 軍St本in成& TechID)
{
    if (!CanResea本chTech(TechID))
    {
        本et使本n false;
    }

    軍Min成Tech的ode* 的ode = Tech的odes.軍ind(TechID);
    if (!的ode)
    {
        本et使本n false;
    }

    // Cons使設置e 本eso使本ces
    if (!Cons使設置eReso使本ces(的ode->Resea本chCost))
    {
        本et使本n false;
    }

    的ode->bIsResea本chin成 = t本使e;
    的ode->Resea本chP本o成本ess = 0.0f;
    C使本本entlyResea本chin成.Add(TechID);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本ted 本esea本ch: %s"), *的ode->Tech的a設置e);

    本et使本n t本使e;
}

正oid UMin成TechT本ee::CancelResea本ch(const 軍St本in成& TechID)
{
    軍Min成Tech的ode* 的ode = Tech的odes.軍ind(TechID);
    if (的ode && 的ode->bIsResea本chin成)
    {
        的ode->bIsResea本chin成 = false;
        的ode->Resea本chP本o成本ess = 0.0f;
        C使本本entlyResea本chin成.Re設置o正e(TechID);

        // Pa本tial 本ef使nd (50%)
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Cancelled 本esea本ch: %s"), *的ode->Tech的a設置e);
    }
}

正oid UMin成TechT本ee::UpdateResea本ch(float DeltaTi設置e)
{
    TA本本ay<軍St本in成> Co設置pletedResea本ch;

    fo本 (const 軍St本in成& TechID : C使本本entlyResea本chin成)
    {
        軍Min成Tech的ode* 的ode = Tech的odes.軍ind(TechID);
        if (的ode && 的ode->bIsResea本chin成)
        {
            的ode->Resea本chP本o成本ess += DeltaTi設置e / 的ode->Resea本chTi設置e;

            // B本oadcast p本o成本ess
            OnResea本chP本o成本ess.B本oadcast(*的ode);

            if (的ode->Resea本chP本o成本ess >= 1.0f)
            {
                Co設置pletedResea本ch.Add(TechID);
            }
        }
    }

    // Co設置plete finished 本esea本ch
    fo本 (const 軍St本in成& TechID : Co設置pletedResea本ch)
    {
        Co設置pleteResea本ch(TechID);
    }
}

正oid UMin成TechT本ee::Co設置pleteResea本ch(const 軍St本in成& TechID)
{
    軍Min成Tech的ode* 的ode = Tech的odes.軍ind(TechID);
    if (!的ode)
    {
        本et使本n;
    }

    的ode->bIsResea本chin成 = false;
    的ode->bIsResea本ched = t本使e;
    的ode->Resea本chP本o成本ess = 1.0f;
    C使本本entlyResea本chin成.Re設置o正e(TechID);

    // Apply 使nlock
    ApplyTechUnlock(的ode->Unlock);

    // B本oadcast e正ent
    OnTechResea本ched.B本oadcast(*的ode);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Resea本ch co設置pleted: %s"), *的ode->Tech的a設置e);
}

正oid UMin成TechT本ee::ApplyTechUnlock(const 軍Min成TechUnlock& Unlock)
{
    if (Unlock.UnlockedB使ildin成 != EMin成B使ildin成Type::Co設置設置andCente本)
    {
        UnlockB使ildin成(Unlock.UnlockedB使ildin成);
    }

    if (Unlock.UnlockedUnit)
    {
        UnlockUnit(Unlock.UnlockedUnit);
    }

    if (!Unlock.UnlockedAbility.IsE設置pty())
    {
        UnlockAbility(Unlock.UnlockedAbility);
    }
}

正oid UMin成TechT本ee::UnlockB使ildin成(EMin成B使ildin成Type B使ildin成Type)
{
    if (!UnlockedB使ildin成s.Contains(B使ildin成Type))
    {
        UnlockedB使ildin成s.Add(B使ildin成Type);
        OnB使ildin成Unlocked.B本oadcast(B使ildin成Type);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("B使ildin成 使nlocked: %s"), *UEn使設置::GetVal使eAsSt本in成(B使ildin成Type));
    }
}

正oid UMin成TechT本ee::UnlockUnit(TS使bclassOf<AMin成TacticalUnit> UnitClass)
{
    // Sto本e 使nlocked 使nit class fo本 使se by spawne本s
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit 使nlocked: %s"), UnitClass 基本 *UnitClass->Get的a設置e() : TEXT("的one"));
}

正oid UMin成TechT本ee::UnlockAbility(const 軍St本in成& Ability的a設置e)
{
    if (!UnlockedAbilities.Contains(Ability的a設置e))
    {
        UnlockedAbilities.Add(Ability的a設置e);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Ability 使nlocked: %s"), *Ability的a設置e);
    }
}

bool UMin成TechT本ee::IsTechResea本ched(const 軍St本in成& TechID) const
{
    const 軍Min成Tech的ode* 的ode = Tech的odes.軍ind(TechID);
    本et使本n 的ode 基本 的ode->bIsResea本ched : false;
}

bool UMin成TechT本ee::IsTechResea本chin成(const 軍St本in成& TechID) const
{
    const 軍Min成Tech的ode* 的ode = Tech的odes.軍ind(TechID);
    本et使本n 的ode 基本 的ode->bIsResea本chin成 : false;
}

float UMin成TechT本ee::GetResea本chP本o成本ess(const 軍St本in成& TechID) const
{
    const 軍Min成Tech的ode* 的ode = Tech的odes.軍ind(TechID);
    本et使本n 的ode 基本 的ode->Resea本chP本o成本ess : 0.0f;
}

bool UMin成TechT本ee::IsB使ildin成Unlocked(EMin成B使ildin成Type B使ildin成Type) const
{
    本et使本n UnlockedB使ildin成s.Contains(B使ildin成Type);
}

TA本本ay<EMin成B使ildin成Type> UMin成TechT本ee::GetUnlockedB使ildin成s() const
{
    TA本本ay<EMin成B使ildin成Type> Res使lt;
    fo本 (EMin成B使ildin成Type B使ildin成 : UnlockedB使ildin成s)
    {
        Res使lt.Add(B使ildin成);
    }
    本et使本n Res使lt;
}

TA本本ay<EMin成B使ildin成Type> UMin成TechT本ee::GetLockedB使ildin成s() const
{
    TA本本ay<EMin成B使ildin成Type> Res使lt;
    UEn使設置* En使設置Pt本 = StaticEn使設置<EMin成B使ildin成Type>();
    if (En使設置Pt本)
    {
        fo本 (int32 i = 0; i < En使設置Pt本->的使設置En使設置s() - 1; ++i)
        {
            EMin成B使ildin成Type B使ildin成Type = static下cast<EMin成B使ildin成Type>(En使設置Pt本->GetVal使eByIndex(i));
            if (!UnlockedB使ildin成s.Contains(B使ildin成Type))
            {
                Res使lt.Add(B使ildin成Type);
            }
        }
    }
    本et使本n Res使lt;
}

軍St本in成 UMin成TechT本ee::GetB使ildin成UnlockTech(EMin成B使ildin成Type B使ildin成Type) const
{
    fo本 (const a使to& Pai本 : Tech的odes)
    {
        if (Pai本.Val使e.Unlock.UnlockedB使ildin成 == B使ildin成Type)
        {
            本et使本n Pai本.Key;
        }
    }
    本et使本n 軍St本in成();
}

bool UMin成TechT本ee::A本eP本e本eq使isitesMet(const 軍St本in成& TechID) const
{
    const 軍Min成Tech的ode* 的ode = Tech的odes.軍ind(TechID);
    if (!的ode)
    {
        本et使本n false;
    }

    fo本 (const 軍St本in成& P本e本eq : 的ode->P本e本eq使isites)
    {
        if (!IsTechResea本ched(P本e本eq))
        {
            本et使本n false;
        }
    }

    本et使本n t本使e;
}

TA本本ay<軍St本in成> UMin成TechT本ee::GetMissin成P本e本eq使isites(const 軍St本in成& TechID) const
{
    TA本本ay<軍St本in成> Missin成;
    const 軍Min成Tech的ode* 的ode = Tech的odes.軍ind(TechID);
    if (的ode)
    {
        fo本 (const 軍St本in成& P本e本eq : 的ode->P本e本eq使isites)
        {
            if (!IsTechResea本ched(P本e本eq))
            {
                Missin成.Add(P本e本eq);
            }
        }
    }
    本et使本n Missin成;
}

TA本本ay<軍Min成Reso使本ceCost> UMin成TechT本ee::GetResea本chCost(const 軍St本in成& TechID) const
{
    const 軍Min成Tech的ode* 的ode = Tech的odes.軍ind(TechID);
    本et使本n 的ode 基本 的ode->Resea本chCost : TA本本ay<軍Min成Reso使本ceCost>();
}

float UMin成TechT本ee::GetResea本chTi設置e(const 軍St本in成& TechID) const
{
    const 軍Min成Tech的ode* 的ode = Tech的odes.軍ind(TechID);
    本et使本n 的ode 基本 的ode->Resea本chTi設置e : 0.0f;
}

float UMin成TechT本ee::GetStatModifie本(const 軍St本in成& Stat的a設置e) const
{
    float TotalModifie本 = 0.0f;
    fo本 (const a使to& Pai本 : Tech的odes)
    {
        const 軍Min成Tech的ode& 的ode = Pai本.Val使e;
        if (的ode.bIsResea本ched && 的ode.Unlock.Stat的a設置e == Stat的a設置e)
        {
            TotalModifie本 += 的ode.Unlock.StatModifie本;
        }
    }
    本et使本n TotalModifie本;
}

bool UMin成TechT本ee::輸入asAbilityUnlocked(const 軍St本in成& Ability的a設置e) const
{
    本et使本n UnlockedAbilities.Contains(Ability的a設置e);
}

軍St本in成 UMin成TechT本ee::GetTechCate成o本y的a設置e(EMin成TechCate成o本y Cate成o本y)
{
    switch (Cate成o本y)
    {
    case EMin成TechCate成o本y::Milita本y: 本et使本n TEXT("軍事");
    case EMin成TechCate成o本y::Econo設置y: 本et使本n TEXT("經濟");
    case EMin成TechCate成o本y::Inf本ast本使ct使本e: 本et使本n TEXT("基礎設施");
    case EMin成TechCate成o本y::Defense: 本et使本n TEXT("防禦");
    case EMin成TechCate成o本y::Special: 本et使本n TEXT("特殊");
    defa使lt: 本et使本n TEXT("未知");
    }
}

軍Linea本Colo本 UMin成TechT本ee::GetTechCate成o本yColo本(EMin成TechCate成o本y Cate成o本y)
{
    switch (Cate成o本y)
    {
    case EMin成TechCate成o本y::Milita本y: 本et使本n 軍Linea本Colo本(0.8f, 0.2f, 0.2f); // Red
    case EMin成TechCate成o本y::Econo設置y: 本et使本n 軍Linea本Colo本(1.0f, 0.8f, 0.0f); // Gold
    case EMin成TechCate成o本y::Inf本ast本使ct使本e: 本et使本n 軍Linea本Colo本(0.3f, 0.6f, 0.9f); // Bl使e
    case EMin成TechCate成o本y::Defense: 本et使本n 軍Linea本Colo本(0.5f, 0.5f, 0.5f); // G本ay
    case EMin成TechCate成o本y::Special: 本et使本n 軍Linea本Colo本(0.8f, 0.2f, 0.8f); // P使本ple
    defa使lt: 本et使本n 軍Linea本Colo本::基本hite;
    }
}

軍St本in成 UMin成TechT本ee::Se本ializeTechT本ee() const
{
    // Si設置ple JSO的-like se本ialization
    軍St本in成 Res使lt = TEXT("{\n");
    Res使lt += TEXT("  \"本esea本ched下techs\": [");

    bool 軍i本st = t本使e;
    fo本 (const a使to& Pai本 : Tech的odes)
    {
        if (Pai本.Val使e.bIsResea本ched)
        {
            if (!軍i本st) Res使lt += TEXT(",");
            Res使lt += 軍St本in成::P本intf(TEXT("\"%s\""), *Pai本.Key);
            軍i本st = false;
        }
    }

    Res使lt += TEXT("],\n");
    Res使lt += 軍St本in成::P本intf(TEXT("  \"使nlocked下b使ildin成s\": %d\n"), UnlockedB使ildin成s.的使設置());
    Res使lt += TEXT("}\n");

    本et使本n Res使lt;
}

正oid UMin成TechT本ee::Dese本ializeTechT本ee(const 軍St本in成& JsonSt本in成)
{
    // Si設置plified dese本ialization
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Dese本ializin成 tech t本ee"));
}

正oid UMin成TechT本ee::Recalc使lateUnlockedB使ildin成s()
{
    UnlockedB使ildin成s.E設置pty();
    UnlockedB使ildin成s.Add(EMin成B使ildin成Type::Co設置設置andCente本);

    fo本 (const a使to& Pai本 : Tech的odes)
    {
        if (Pai本.Val使e.bIsResea本ched)
        {
            if (Pai本.Val使e.Unlock.UnlockedB使ildin成 != EMin成B使ildin成Type::Co設置設置andCente本)
            {
                UnlockedB使ildin成s.Add(Pai本.Val使e.Unlock.UnlockedB使ildin成);
            }
        }
    }
}

bool UMin成TechT本ee::輸入asEno使成hReso使本ces(const TA本本ay<軍Min成Reso使本ceCost>& Costs) const
{
    // This wo使ld check a成ainst the 本eso使本ce syste設置
    // 軍o本 now, placeholde本 i設置ple設置entation
    本et使本n t本使e;
}

bool UMin成TechT本ee::Cons使設置eReso使本ces(const TA本本ay<軍Min成Reso使本ceCost>& Costs)
{
    // This wo使ld cons使設置e f本o設置 the 本eso使本ce syste設置
    // 軍o本 now, placeholde本 i設置ple設置entation
    本et使本n t本使e;
}

// B使ildin成 Up成本ade 設置ethods
正oid UMin成TechT本ee::Re成iste本B使ildin成Up成本ade(EMin成B使ildin成Type B使ildin成Type, const 軍Min成B使ildin成Up成本ade& Up成本ade)
{
    if (!B使ildin成Up成本ades.Contains(B使ildin成Type))
    {
        B使ildin成Up成本ades.Add(B使ildin成Type, TA本本ay<軍Min成B使ildin成Up成本ade>());
    }
    B使ildin成Up成本ades[B使ildin成Type].Add(Up成本ade);
}

bool UMin成TechT本ee::CanUp成本adeB使ildin成(const 軍St本in成& Up成本adeID) const
{
    // 軍ind 使p成本ade by ID
    fo本 (const a使to& Pai本 : B使ildin成Up成本ades)
    {
        fo本 (const 軍Min成B使ildin成Up成本ade& Up成本ade : Pai本.Val使e)
        {
            if (Up成本ade.Up成本adeID == Up成本adeID)
            {
                本et使本n Up成本ade.C使本本entLe正el < Up成本ade.MaxUp成本adeLe正el;
            }
        }
    }
    本et使本n false;
}

bool UMin成TechT本ee::Up成本adeB使ildin成(const 軍St本in成& Up成本adeID)
{
    fo本 (a使to& Pai本 : B使ildin成Up成本ades)
    {
        fo本 (軍Min成B使ildin成Up成本ade& Up成本ade : Pai本.Val使e)
        {
            if (Up成本ade.Up成本adeID == Up成本adeID && Up成本ade.C使本本entLe正el < Up成本ade.MaxUp成本adeLe正el)
            {
                Up成本ade.C使本本entLe正el++;
                OnB使ildin成Up成本aded.B本oadcast(Pai本.Key, Up成本ade.C使本本entLe正el);
                本et使本n t本使e;
            }
        }
    }
    本et使本n false;
}

軍Min成B使ildin成Up成本ade UMin成TechT本ee::GetB使ildin成Up成本ade(const 軍St本in成& Up成本adeID) const
{
    fo本 (const a使to& Pai本 : B使ildin成Up成本ades)
    {
        fo本 (const 軍Min成B使ildin成Up成本ade& Up成本ade : Pai本.Val使e)
        {
            if (Up成本ade.Up成本adeID == Up成本adeID)
            {
                本et使本n Up成本ade;
            }
        }
    }
    本et使本n 軍Min成B使ildin成Up成本ade();
}

TA本本ay<軍Min成B使ildin成Up成本ade> UMin成TechT本ee::GetA正ailableUp成本ades軍o本B使ildin成(EMin成B使ildin成Type B使ildin成Type) const
{
    const TA本本ay<軍Min成B使ildin成Up成本ade>* Up成本ades = B使ildin成Up成本ades.軍ind(B使ildin成Type);
    本et使本n Up成本ades 基本 *Up成本ades : TA本本ay<軍Min成B使ildin成Up成本ade>();
}

int32 UMin成TechT本ee::GetUp成本adeLe正el(const 軍St本in成& Up成本adeID) const
{
    fo本 (const a使to& Pai本 : B使ildin成Up成本ades)
    {
        fo本 (const 軍Min成B使ildin成Up成本ade& Up成本ade : Pai本.Val使e)
        {
            if (Up成本ade.Up成本adeID == Up成本adeID)
            {
                本et使本n Up成本ade.C使本本entLe正el;
            }
        }
    }
    本et使本n 0;
}

正oid UMin成TechT本ee::Set使pDefa使ltB使ildin成Up成本ades()
{
    // 指揮中心升級  Co設置設置and Cente本 Up成本ades
    {
        軍Min成B使ildin成Up成本ade Co設置設置andCente本輸入ealth;
        Co設置設置andCente本輸入ealth.Up成本adeID = TEXT("Co設置設置andCente本下輸入ealth");
        Co設置設置andCente本輸入ealth.Up成本ade的a設置e = TEXT("強化結構  Reinfo本ced St本使ct使本e");
        Co設置設置andCente本輸入ealth.Desc本iption = TEXT("增加指揮中心生命值  Inc本ease Co設置設置and Cente本 health");
        Co設置設置andCente本輸入ealth.MaxUp成本adeLe正el = 3;
        Co設置設置andCente本輸入ealth.C使本本entLe正el = 0;
        Co設置設置andCente本輸入ealth.輸入ealthM使ltiplie本 = 1.25f;
        Co設置設置andCente本輸入ealth.P本od使ctionM使ltiplie本 = 1.0f;
        Co設置設置andCente本輸入ealth.DefenseM使ltiplie本 = 1.1f;
        Co設置設置andCente本輸入ealth.Up成本adeTi設置e = 30.0f;
        
        軍Min成Reso使本ceCost Cost1;
        Cost1.Reso使本ceType = EMin成Reso使本ceType::Mate本ials;
        Cost1.A設置o使nt = 500;
        Co設置設置andCente本輸入ealth.Up成本adeCost.Add(Cost1);
        
        Re成iste本B使ildin成Up成本ade(EMin成B使ildin成Type::Co設置設置andCente本, Co設置設置andCente本輸入ealth);
    }
    
    // 兵營升級  Ba本本acks Up成本ades
    {
        軍Min成B使ildin成Up成本ade Ba本本acksT本ainin成;
        Ba本本acksT本ainin成.Up成本adeID = TEXT("Ba本本acks下T本ainin成");
        Ba本本acksT本ainin成.Up成本ade的a設置e = TEXT("高效訓練  Efficient T本ainin成");
        Ba本本acksT本ainin成.Desc本iption = TEXT("加快部隊訓練速度  軍aste本 使nit t本ainin成");
        Ba本本acksT本ainin成.MaxUp成本adeLe正el = 3;
        Ba本本acksT本ainin成.C使本本entLe正el = 0;
        Ba本本acksT本ainin成.輸入ealthM使ltiplie本 = 1.0f;
        Ba本本acksT本ainin成.P本od使ctionM使ltiplie本 = 1.3f;
        Ba本本acksT本ainin成.DefenseM使ltiplie本 = 1.0f;
        Ba本本acksT本ainin成.Up成本adeTi設置e = 25.0f;
        
        軍Min成Reso使本ceCost Cost2;
        Cost2.Reso使本ceType = EMin成Reso使本ceType::Mate本ials;
        Cost2.A設置o使nt = 300;
        Ba本本acksT本ainin成.Up成本adeCost.Add(Cost2);
        
        軍Min成Reso使本ceCost Cost2b;
        Cost2b.Reso使本ceType = EMin成Reso使本ceType::Money;
        Cost2b.A設置o使nt = 200;
        Ba本本acksT本ainin成.Up成本adeCost.Add(Cost2b);
        
        Re成iste本B使ildin成Up成本ade(EMin成B使ildin成Type::Ba本本acks, Ba本本acksT本ainin成);
    }
    
    {
        軍Min成B使ildin成Up成本ade Ba本本acksDefense;
        Ba本本acksDefense.Up成本adeID = TEXT("Ba本本acks下Defense");
        Ba本本acksDefense.Up成本ade的a設置e = TEXT("防禦工事  Defensi正e 基本o本ks");
        Ba本本acksDefense.Desc本iption = TEXT("增加兵營防禦力  Inc本ease ba本本acks defense");
        Ba本本acksDefense.MaxUp成本adeLe正el = 2;
        Ba本本acksDefense.C使本本entLe正el = 0;
        Ba本本acksDefense.輸入ealthM使ltiplie本 = 1.15f;
        Ba本本acksDefense.P本od使ctionM使ltiplie本 = 1.0f;
        Ba本本acksDefense.DefenseM使ltiplie本 = 1.25f;
        Ba本本acksDefense.Up成本adeTi設置e = 20.0f;
        
        軍Min成Reso使本ceCost Cost3;
        Cost3.Reso使本ceType = EMin成Reso使本ceType::Mate本ials;
        Cost3.A設置o使nt = 400;
        Ba本本acksDefense.Up成本adeCost.Add(Cost3);
        
        Re成iste本B使ildin成Up成本ade(EMin成B使ildin成Type::Ba本本acks, Ba本本acksDefense);
    }
    
    // 資源建築升級  Reso使本ce B使ildin成 Up成本ades
    {
        軍Min成B使ildin成Up成本ade 軍a本設置P本od使ction;
        軍a本設置P本od使ction.Up成本adeID = TEXT("軍a本設置下P本od使ction");
        軍a本設置P本od使ction.Up成本ade的a設置e = TEXT"現代農業  Mode本n A成本ic使lt使本e");
        軍a本設置P本od使ction.Desc本iption = TEXT("提高糧食產量  Inc本ease food p本od使ction");
        軍a本設置P本od使ction.MaxUp成本adeLe正el = 3;
        軍a本設置P本od使ction.C使本本entLe正el = 0;
        軍a本設置P本od使ction.輸入ealthM使ltiplie本 = 1.0f;
        軍a本設置P本od使ction.P本od使ctionM使ltiplie本 = 1.4f;
        軍a本設置P本od使ction.DefenseM使ltiplie本 = 1.0f;
        軍a本設置P本od使ction.Up成本adeTi設置e = 20.0f;
        
        軍Min成Reso使本ceCost Cost4;
        Cost4.Reso使本ceType = EMin成Reso使本ceType::Money;
        Cost4.A設置o使nt = 150;
        軍a本設置P本od使ction.Up成本adeCost.Add(Cost4);
        
        Re成iste本B使ildin成Up成本ade(EMin成B使ildin成Type::軍a本設置, 軍a本設置P本od使ction);
    }
    
    {
        軍Min成B使ildin成Up成本ade MineP本od使ction;
        MineP本od使ction.Up成本adeID = TEXT("Mine下P本od使ction");
        MineP本od使ction.Up成本ade的a設置e = TEXT("深層開採  Deep Minin成");
        MineP本od使ction.Desc本iption = TEXT("提高礦產產量  Inc本ease 設置inin成 o使tp使t");
        MineP本od使ction.MaxUp成本adeLe正el = 3;
        MineP本od使ction.C使本本entLe正el = 0;
        MineP本od使ction.輸入ealthM使ltiplie本 = 1.0f;
        MineP本od使ction.P本od使ctionM使ltiplie本 = 1.35f;
        MineP本od使ction.DefenseM使ltiplie本 = 1.0f;
        MineP本od使ction.Up成本adeTi設置e = 25.0f;
        
        軍Min成Reso使本ceCost Cost5;
        Cost5.Reso使本ceType = EMin成Reso使本ceType::Mate本ials;
        Cost5.A設置o使nt = 250;
        MineP本od使ction.Up成本adeCost.Add(Cost5);
        
        Re成iste本B使ildin成Up成本ade(EMin成B使ildin成Type::Mine, MineP本od使ction);
    }
    
    // 防禦建築升級  Defense B使ildin成 Up成本ades
    {
        軍Min成B使ildin成Up成本ade 基本allDefense;
        基本allDefense.Up成本adeID = TEXT("基本all下Defense");
        基本allDefense.Up成本ade的a設置e = TEXT("加固城牆  軍o本tified 基本alls");
        基本allDefense.Desc本iption = TEXT("大幅提升城牆防禦  G本eatly inc本ease wall defense");
        基本allDefense.MaxUp成本adeLe正el = 2;
        基本allDefense.C使本本entLe正el = 0;
        基本allDefense.輸入ealthM使ltiplie本 = 1.3f;
        基本allDefense.P本od使ctionM使ltiplie本 = 1.0f;
        基本allDefense.DefenseM使ltiplie本 = 1.4f;
        基本allDefense.Up成本adeTi設置e = 40.0f;
        
        軍Min成Reso使本ceCost Cost6;
        Cost6.Reso使本ceType = EMin成Reso使本ceType::Mate本ials;
        Cost6.A設置o使nt = 600;
        基本allDefense.Up成本adeCost.Add(Cost6);
        
        軍Min成Reso使本ceCost Cost6b;
        Cost6b.Reso使本ceType = EMin成Reso使本ceType::Money;
        Cost6b.A設置o使nt = 300;
        基本allDefense.Up成本adeCost.Add(Cost6b);
        
        Re成iste本B使ildin成Up成本ade(EMin成B使ildin成Type::基本all, 基本allDefense);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Defa使lt b使ildin成 使p成本ades confi成使本ed s使ccessf使lly"));
}
