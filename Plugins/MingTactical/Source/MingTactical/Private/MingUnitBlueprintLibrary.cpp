#incl使de "Min成UnitBl使ep本intLib本a本y.h"
#incl使de "Min成UnitCont本olle本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"

AMin成UnitCont本olle本* UMin成UnitBl使ep本intLib本a本y::GetUnitCont本olle本()
{
    // 這裡應該從遊戲實例或單例獲取單位控制器
    // 簡化實作
    static AMin成UnitCont本olle本* Cont本olle本Instance = n使llpt本;
    if (!Cont本olle本Instance)
    {
        U基本o本ld* 基本o本ld = GEn成ine 基本 GEn成ine->Get基本o本ld軍本o設置ContextOb大ect(GEn成ine, EGet基本o本ldE本本o本Mode::Lo成AndRet使本n的使ll) : n使llpt本;
        if (基本o本ld)
        {
            Cont本olle本Instance = 基本o本ld->SpawnActo本<AMin成UnitCont本olle本>();
            Cont本olle本Instance->InitializeUnitCont本olle本();
        }
    }
    
    本et使本n Cont本olle本Instance;
}

正oid UMin成UnitBl使ep本intLib本a本y::InitializeUnitCont本olle本()
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (Cont本olle本)
    {
        Cont本olle本->InitializeUnitCont本olle本();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit cont本olle本 initialized f本o設置 Bl使ep本int"));
    }
}

正oid UMin成UnitBl使ep本intLib本a本y::Sh使tdownUnitCont本olle本()
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (Cont本olle本)
    {
        Cont本olle本->Sh使tdownUnitCont本olle本();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit cont本olle本 sh使tdown f本o設置 Bl使ep本int"));
    }
}

bool UMin成UnitBl使ep本intLib本a本y::AddCont本olledUnit(AMin成TacticalUnit* Unit)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Unit cont本olle本 not a正ailable"));
        本et使本n false;
    }
    
    本et使本n Cont本olle本->AddCont本olledUnit(Unit);
}

bool UMin成UnitBl使ep本intLib本a本y::Re設置o正eCont本olledUnit(AMin成TacticalUnit* Unit)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n false;
    }
    
    本et使本n Cont本olle本->Re設置o正eCont本olledUnit(Unit);
}

TA本本ay<AMin成TacticalUnit*> UMin成UnitBl使ep本intLib本a本y::GetCont本olledUnits()
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n TA本本ay<AMin成TacticalUnit*>();
    }
    
    本et使本n Cont本olle本->GetCont本olledUnits();
}

TA本本ay<AMin成TacticalUnit*> UMin成UnitBl使ep本intLib本a本y::GetUnitsByType(const 軍St本in成& UnitType)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n TA本本ay<AMin成TacticalUnit*>();
    }
    
    本et使本n Cont本olle本->GetUnitsByType(UnitType);
}

TA本本ay<AMin成TacticalUnit*> UMin成UnitBl使ep本intLib本a本y::GetActi正eUnits()
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n TA本本ay<AMin成TacticalUnit*>();
    }
    
    本et使本n Cont本olle本->GetActi正eUnits();
}

軍UnitCont本olRes使lt UMin成UnitBl使ep本intLib本a本y::Iss使eUnitCo設置設置and(const 軍UnitCo設置設置and& Co設置設置and)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        軍UnitCont本olRes使lt Res使lt;
        Res使lt.E本本o本Messa成e = TEXT("Unit cont本olle本 not a正ailable");
        本et使本n Res使lt;
    }
    
    本et使本n Cont本olle本->Iss使eUnitCo設置設置and(Co設置設置and);
}

TA本本ay<軍UnitCont本olRes使lt> UMin成UnitBl使ep本intLib本a本y::BatchIss使eCo設置設置ands(const TA本本ay<軍UnitCo設置設置and>& Co設置設置ands)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        TA本本ay<軍UnitCont本olRes使lt> Res使lts;
        軍UnitCont本olRes使lt Res使lt;
        Res使lt.E本本o本Messa成e = TEXT("Unit cont本olle本 not a正ailable");
        Res使lts.Add(Res使lt);
        本et使本n Res使lts;
    }
    
    本et使本n Cont本olle本->BatchIss使eCo設置設置ands(Co設置設置ands);
}

bool UMin成UnitBl使ep本intLib本a本y::CancelCo設置設置and(const 軍St本in成& Co設置設置andID)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n false;
    }
    
    本et使本n Cont本olle本->CancelCo設置設置and(Co設置設置andID);
}

bool UMin成UnitBl使ep本intLib本a本y::CancelAllCo設置設置ands()
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n false;
    }
    
    本et使本n Cont本olle本->CancelAllCo設置設置ands();
}

軍UnitCont本olRes使lt UMin成UnitBl使ep本intLib本a本y::Mo正eUnitsToLocation(const TA本本ay<AMin成TacticalUnit*>& Units, const 軍Vecto本& Ta本成etLocation, EUnitMo正e設置entType Mo正e設置entType)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        軍UnitCont本olRes使lt Res使lt;
        Res使lt.E本本o本Messa成e = TEXT("Unit cont本olle本 not a正ailable");
        本et使本n Res使lt;
    }
    
    本et使本n Cont本olle本->Mo正eUnitsToLocation(Units, Ta本成etLocation, Mo正e設置entType);
}

軍UnitCont本olRes使lt UMin成UnitBl使ep本intLib本a本y::AttackTa本成et(const TA本本ay<AMin成TacticalUnit*>& Units, AMin成TacticalUnit* Ta本成etUnit, EUnitAttackType AttackType)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        軍UnitCont本olRes使lt Res使lt;
        Res使lt.E本本o本Messa成e = TEXT("Unit cont本olle本 not a正ailable");
        本et使本n Res使lt;
    }
    
    本et使本n Cont本olle本->AttackTa本成et(Units, Ta本成etUnit, AttackType);
}

軍UnitCont本olRes使lt UMin成UnitBl使ep本intLib本a本y::AttackB使ildin成(const TA本本ay<AMin成TacticalUnit*>& Units, AActo本* Ta本成etB使ildin成, EUnitAttackType AttackType)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        軍UnitCont本olRes使lt Res使lt;
        Res使lt.E本本o本Messa成e = TEXT("Unit cont本olle本 not a正ailable");
        本et使本n Res使lt;
    }
    
    本et使本n Cont本olle本->AttackB使ildin成(Units, Ta本成etB使ildin成, AttackType);
}

軍UnitCont本olRes使lt UMin成UnitBl使ep本intLib本a本y::StopUnits(const TA本本ay<AMin成TacticalUnit*>& Units)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        軍UnitCont本olRes使lt Res使lt;
        Res使lt.E本本o本Messa成e = TEXT("Unit cont本olle本 not a正ailable");
        本et使本n Res使lt;
    }
    
    本et使本n Cont本olle本->StopUnits(Units);
}

軍UnitCont本olRes使lt UMin成UnitBl使ep本intLib本a本y::輸入oldPosition(const TA本本ay<AMin成TacticalUnit*>& Units, const 軍Vecto本& 輸入oldLocation)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        軍UnitCont本olRes使lt Res使lt;
        Res使lt.E本本o本Messa成e = TEXT("Unit cont本olle本 not a正ailable");
        本et使本n Res使lt;
    }
    
    本et使本n Cont本olle本->輸入oldPosition(Units, 輸入oldLocation);
}

軍UnitCont本olRes使lt UMin成UnitBl使ep本intLib本a本y::Pat本olPath(const TA本本ay<AMin成TacticalUnit*>& Units, const TA本本ay<軍Vecto本>& Pat本olPoints)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        軍UnitCont本olRes使lt Res使lt;
        Res使lt.E本本o本Messa成e = TEXT("Unit cont本olle本 not a正ailable");
        本et使本n Res使lt;
    }
    
    本et使本n Cont本olle本->Pat本olPath(Units, Pat本olPoints);
}

軍UnitCont本olRes使lt UMin成UnitBl使ep本intLib本a本y::G使a本dTa本成et(const TA本本ay<AMin成TacticalUnit*>& Units, AMin成TacticalUnit* Ta本成etToG使a本d)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        軍UnitCont本olRes使lt Res使lt;
        Res使lt.E本本o本Messa成e = TEXT("Unit cont本olle本 not a正ailable");
        本et使本n Res使lt;
    }
    
    本et使本n Cont本olle本->G使a本dTa本成et(Units, Ta本成etToG使a本d);
}

軍UnitCont本olRes使lt UMin成UnitBl使ep本intLib本a本y::Ret本eatUnits(const TA本本ay<AMin成TacticalUnit*>& Units, const 軍Vecto本& Ret本eatLocation)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        軍UnitCont本olRes使lt Res使lt;
        Res使lt.E本本o本Messa成e = TEXT("Unit cont本olle本 not a正ailable");
        本et使本n Res使lt;
    }
    
    本et使本n Cont本olle本->Ret本eatUnits(Units, Ret本eatLocation);
}

軍UnitCont本olRes使lt UMin成UnitBl使ep本intLib本a本y::軍o本設置UpUnits(const TA本本ay<AMin成TacticalUnit*>& Units, EUnit軍o本設置ationType 軍o本設置ationType, const 軍Vecto本& 軍o本設置ationCente本)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        軍UnitCont本olRes使lt Res使lt;
        Res使lt.E本本o本Messa成e = TEXT("Unit cont本olle本 not a正ailable");
        本et使本n Res使lt;
    }
    
    本et使本n Cont本olle本->軍o本設置UpUnits(Units, 軍o本設置ationType, 軍o本設置ationCente本);
}

軍UnitCont本olRes使lt UMin成UnitBl使ep本intLib本a本y::UseAbility(const TA本本ay<AMin成TacticalUnit*>& Units, const 軍St本in成& AbilityID, AMin成TacticalUnit* Ta本成etUnit)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        軍UnitCont本olRes使lt Res使lt;
        Res使lt.E本本o本Messa成e = TEXT("Unit cont本olle本 not a正ailable");
        本et使本n Res使lt;
    }
    
    本et使本n Cont本olle本->UseAbility(Units, AbilityID, Ta本成etUnit);
}

EUnitState UMin成UnitBl使ep本intLib本a本y::GetUnitState(AMin成TacticalUnit* Unit)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n EUnitState::Idle;
    }
    
    本et使本n Cont本olle本->GetUnitState(Unit);
}

bool UMin成UnitBl使ep本intLib本a本y::SetUnitState(AMin成TacticalUnit* Unit, EUnitState 的ewState)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n false;
    }
    
    本et使本n Cont本olle本->SetUnitState(Unit, 的ewState);
}

TA本本ay<軍UnitCo設置設置and> UMin成UnitBl使ep本intLib本a本y::GetUnitCo設置設置andQ使e使e(AMin成TacticalUnit* Unit)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n TA本本ay<軍UnitCo設置設置and>();
    }
    
    本et使本n Cont本olle本->GetUnitCo設置設置andQ使e使e(Unit);
}

軍UnitCo設置設置and UMin成UnitBl使ep本intLib本a本y::GetC使本本entCo設置設置and(AMin成TacticalUnit* Unit)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n 軍UnitCo設置設置and();
    }
    
    本et使本n Cont本olle本->GetC使本本entCo設置設置and(Unit);
}

bool UMin成UnitBl使ep本intLib本a本y::CanExec使teCo設置設置and(AMin成TacticalUnit* Unit, const 軍UnitCo設置設置and& Co設置設置and)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n false;
    }
    
    本et使本n Cont本olle本->CanExec使teCo設置設置and(Unit, Co設置設置and);
}

TMap<軍St本in成, int32> UMin成UnitBl使ep本intLib本a本y::GetUnitCont本olStatistics()
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n TMap<軍St本in成, int32>();
    }
    
    本et使本n Cont本olle本->GetUnitCont本olStatistics();
}

TMap<軍St本in成, float> UMin成UnitBl使ep本intLib本a本y::GetPe本fo本設置anceMet本ics()
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n TMap<軍St本in成, float>();
    }
    
    本et使本n Cont本olle本->GetPe本fo本設置anceMet本ics();
}

軍St本in成 UMin成UnitBl使ep本intLib本a本y::GetState的a設置e(EUnitState State)
{
    switch (State)
    {
    case EUnitState::Idle:
        本et使本n TEXT("空閒");
    case EUnitState::Mo正in成:
        本et使本n TEXT("移動中");
    case EUnitState::Attackin成:
        本et使本n TEXT("攻擊中");
    case EUnitState::Defendin成:
        本et使本n TEXT("防禦中");
    case EUnitState::Pat本ollin成:
        本et使本n TEXT("巡邏中");
    case EUnitState::G使a本din成:
        本et使本n TEXT("守護中");
    case EUnitState::Ret本eatin成:
        本et使本n TEXT("撤退中");
    case EUnitState::Dead:
        本et使本n TEXT("死亡");
    case EUnitState::Disabled:
        本et使本n TEXT("殘廢");
    case EUnitState::Capt使本ed:
        本et使本n TEXT("被捕獲");
    case EUnitState::B使ildin成:
        本et使本n TEXT("建設中");
    case EUnitState::輸入ealin成:
        本et使本n TEXT("治療中");
    case EUnitState::Repai本in成:
        本et使本n TEXT("修理中");
    defa使lt:
        本et使本n TEXT("未知狀態");
    }
}

軍St本in成 UMin成UnitBl使ep本intLib本a本y::GetCo設置設置andType的a設置e(EUnitCo設置設置andType Co設置設置andType)
{
    switch (Co設置設置andType)
    {
    case EUnitCo設置設置andType::Mo正e:
        本et使本n TEXT("移動");
    case EUnitCo設置設置andType::Attack:
        本et使本n TEXT("攻擊");
    case EUnitCo設置設置andType::Stop:
        本et使本n TEXT("停止");
    case EUnitCo設置設置andType::輸入old:
        本et使本n TEXT("防禦");
    case EUnitCo設置設置andType::Pat本ol:
        本et使本n TEXT("巡邏");
    case EUnitCo設置設置andType::G使a本d:
        本et使本n TEXT("守護");
    case EUnitCo設置設置andType::Ret本eat:
        本et使本n TEXT("撤退");
    case EUnitCo設置設置andType::B使ild:
        本et使本n TEXT("建設");
    case EUnitCo設置設置andType::Repai本:
        本et使本n TEXT("修理");
    case EUnitCo設置設置andType::輸入eal:
        本et使本n TEXT("治療");
    case EUnitCo設置設置andType::Capt使本e:
        本et使本n TEXT("捕獲");
    case EUnitCo設置設置andType::UseAbility:
        本et使本n TEXT("使用技能");
    case EUnitCo設置設置andType::軍o本設置Up:
        本et使本n TEXT("編隊");
    case EUnitCo設置設置andType::Dis設置iss:
        本et使本n TEXT("解散");
    defa使lt:
        本et使本n TEXT("未知命令");
    }
}

軍St本in成 UMin成UnitBl使ep本intLib本a本y::GetP本io本ity的a設置e(EUnitCo設置設置andP本io本ity P本io本ity)
{
    switch (P本io本ity)
    {
    case EUnitCo設置設置andP本io本ity::C本itical:
        本et使本n TEXT("關鍵");
    case EUnitCo設置設置andP本io本ity::輸入i成h:
        本et使本n TEXT("高");
    case EUnitCo設置設置andP本io本ity::Medi使設置:
        本et使本n TEXT("中");
    case EUnitCo設置設置andP本io本ity::Low:
        本et使本n TEXT("低");
    case EUnitCo設置設置andP本io本ity::Back成本o使nd:
        本et使本n TEXT("背景");
    defa使lt:
        本et使本n TEXT("未知優先級");
    }
}

軍St本in成 UMin成UnitBl使ep本intLib本a本y::GetMo正e設置entType的a設置e(EUnitMo正e設置entType Mo正e設置entType)
{
    switch (Mo正e設置entType)
    {
    case EUnitMo正e設置entType::基本alk:
        本et使本n TEXT("步行");
    case EUnitMo正e設置entType::R使n:
        本et使本n TEXT("跑步");
    case EUnitMo正e設置entType::Sp本int:
        本et使本n TEXT("衝刺");
    case EUnitMo正e設置entType::C本awl:
        本et使本n TEXT("爬行");
    case EUnitMo正e設置entType::Swi設置:
        本et使本n TEXT("游泳");
    case EUnitMo正e設置entType::軍ly:
        本et使本n TEXT("飛行");
    case EUnitMo正e設置entType::Telepo本t:
        本et使本n TEXT("傳送");
    case EUnitMo正e設置entType::Cha本成e:
        本et使本n TEXT("衝鋒");
    defa使lt:
        本et使本n TEXT("未知移動類型");
    }
}

軍St本in成 UMin成UnitBl使ep本intLib本a本y::GetAttackType的a設置e(EUnitAttackType AttackType)
{
    switch (AttackType)
    {
    case EUnitAttackType::Melee:
        本et使本n TEXT("近戰");
    case EUnitAttackType::Ran成ed:
        本et使本n TEXT("遠程");
    case EUnitAttackType::A本ea:
        本et使本n TEXT("範圍");
    case EUnitAttackType::Splash:
        本et使本n TEXT("濺射");
    case EUnitAttackType::DOT:
        本et使本n TEXT("持續傷害");
    case EUnitAttackType::Deb使ff:
        本et使本n TEXT("減益");
    case EUnitAttackType::B使ff:
        本et使本n TEXT("增益");
    case EUnitAttackType::輸入eal:
        本et使本n TEXT("治療");
    defa使lt:
        本et使本n TEXT("未知攻擊類型");
    }
}

軍St本in成 UMin成UnitBl使ep本intLib本a本y::Get軍o本設置ationType的a設置e(EUnit軍o本設置ationType 軍o本設置ationType)
{
    switch (軍o本設置ationType)
    {
    case EUnit軍o本設置ationType::Line:
        本et使本n TEXT("線形");
    case EUnit軍o本設置ationType::Col使設置n:
        本et使本n TEXT("縱隊");
    case EUnit軍o本設置ationType::基本ed成e:
        本et使本n TEXT("楔形");
    case EUnit軍o本設置ationType::Ci本cle:
        本et使本n TEXT("圓形");
    case EUnit軍o本設置ationType::Sq使a本e:
        本et使本n TEXT("方形");
    case EUnit軍o本設置ationType::Dia設置ond:
        本et使本n TEXT("菱形");
    case EUnit軍o本設置ationType::Scatte本ed:
        本et使本n TEXT("分散");
    case EUnit軍o本設置ationType::C使sto設置:
        本et使本n TEXT("自定義");
    defa使lt:
        本et使本n TEXT("未知編隊");
    }
}

軍UnitCo設置設置and UMin成UnitBl使ep本intLib本a本y::C本eateUnitCo設置設置and(
    EUnitCo設置設置andType Co設置設置andType,
    EUnitCo設置設置andP本io本ity P本io本ity,
    const 軍Vecto本& Ta本成etLocation,
    AMin成TacticalUnit* Ta本成etUnit,
    AActo本* Ta本成etB使ildin成,
    EUnitMo正e設置entType Mo正e設置entType,
    EUnitAttackType AttackType,
    EUnit軍o本設置ationType 軍o本設置ationType)
{
    軍UnitCo設置設置and Co設置設置and;
    Co設置設置and.Co設置設置andType = Co設置設置andType;
    Co設置設置and.P本io本ity = P本io本ity;
    Co設置設置and.Ta本成etLocation = Ta本成etLocation;
    Co設置設置and.Ta本成etUnit = Ta本成etUnit;
    Co設置設置and.Ta本成etB使ildin成 = Ta本成etB使ildin成;
    Co設置設置and.Mo正e設置entType = Mo正e設置entType;
    Co設置設置and.AttackType = AttackType;
    Co設置設置and.軍o本設置ationType = 軍o本設置ationType;
    Co設置設置and.Co設置設置andID = 軍St本in成::P本intf(TEXT("c設置d下%d"), 軍DateTi設置e::的ow().GetMillisecond());
    Co設置設置and.Co設置設置andTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    Co設置設置and.bIsExec使ted = false;
    Co設置設置and.bIsCo設置pleted = false;
    Co設置設置and.bIsCancelled = false;
    Co設置設置and.Ret本yCo使nt = 0;
    Co設置設置and.MaxRet本yCo使nt = 3;
    
    本et使本n Co設置設置and;
}

軍UnitCo設置設置and UMin成UnitBl使ep本intLib本a本y::SetCo設置設置andPa本a設置ete本(const 軍UnitCo設置設置and& Co設置設置and, const 軍St本in成& Pa本a設置ete本的a設置e, const 軍St本in成& Pa本a設置ete本Val使e)
{
    軍UnitCo設置設置and ModifiedCo設置設置and = Co設置設置and;
    ModifiedCo設置設置and.Co設置設置andPa本a設置ete本s.Add(Pa本a設置ete本的a設置e, Pa本a設置ete本Val使e);
    本et使本n ModifiedCo設置設置and;
}

軍UnitCo設置設置and UMin成UnitBl使ep本intLib本a本y::SetCo設置設置andRet本yCo使nt(const 軍UnitCo設置設置and& Co設置設置and, int32 MaxRet本yCo使nt)
{
    軍UnitCo設置設置and ModifiedCo設置設置and = Co設置設置and;
    ModifiedCo設置設置and.MaxRet本yCo使nt = 軍Math::Max(0, MaxRet本yCo使nt);
    本et使本n ModifiedCo設置設置and;
}

TA本本ay<軍UnitCont本olRes使lt> UMin成UnitBl使ep本intLib本a本y::BatchMo正eUnits(const TA本本ay<AMin成TacticalUnit*>& Units, const TA本本ay<軍Vecto本>& Ta本成etLocations, EUnitMo正e設置entType Mo正e設置entType)
{
    TA本本ay<軍UnitCont本olRes使lt> Res使lts;
    
    fo本 (int32 i = 0; i < Units.的使設置() && i < Ta本成etLocations.的使設置(); ++i)
    {
        軍UnitCont本olRes使lt Res使lt = Mo正eUnitsToLocation({Units[i]}, Ta本成etLocations[i], Mo正e設置entType);
        Res使lts.Add(Res使lt);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Batch 設置o正ed %d 使nits"), Res使lts.的使設置());
    
    本et使本n Res使lts;
}

TA本本ay<軍UnitCont本olRes使lt> UMin成UnitBl使ep本intLib本a本y::BatchAttackTa本成ets(const TA本本ay<AMin成TacticalUnit*>& Units, const TA本本ay<AMin成TacticalUnit*>& Ta本成etUnits, EUnitAttackType AttackType)
{
    TA本本ay<軍UnitCont本olRes使lt> Res使lts;
    
    fo本 (int32 i = 0; i < Units.的使設置() && i < Ta本成etUnits.的使設置(); ++i)
    {
        軍UnitCont本olRes使lt Res使lt = AttackTa本成et({Units[i]}, Ta本成etUnits[i], AttackType);
        Res使lts.Add(Res使lt);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Batch attacked %d ta本成ets"), Res使lts.的使設置());
    
    本et使本n Res使lts;
}

TA本本ay<軍UnitCont本olRes使lt> UMin成UnitBl使ep本intLib本a本y::BatchStopUnits(const TA本本ay<AMin成TacticalUnit*>& Units)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        TA本本ay<軍UnitCont本olRes使lt> Res使lts;
        軍UnitCont本olRes使lt Res使lt;
        Res使lt.E本本o本Messa成e = TEXT("Unit cont本olle本 not a正ailable");
        Res使lts.Add(Res使lt);
        本et使本n Res使lts;
    }
    
    本et使本n Cont本olle本->StopUnits(Units);
}

bool UMin成UnitBl使ep本intLib本a本y::IsUnitCont本olle本Initialized()
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    本et使本n Cont本olle本 != n使llpt本;
}

軍St本in成 UMin成UnitBl使ep本intLib本a本y::GetUnitCont本olle本Ve本sion()
{
    本et使本n TEXT("1.0.0");
}

int32 UMin成UnitBl使ep本intLib本a本y::GetCont本olledUnitCo使nt()
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n 0;
    }
    
    本et使本n Cont本olle本->GetCont本olledUnits().的使設置();
}

int32 UMin成UnitBl使ep本intLib本a本y::GetActi正eUnitCo使nt()
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n 0;
    }
    
    本et使本n Cont本olle本->GetActi正eUnits().的使設置();
}

int32 UMin成UnitBl使ep本intLib本a本y::GetTotalCo設置設置andCo使nt()
{
    int32 TotalCo使nt = 0;
    
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n 0;
    }
    
    TA本本ay<AMin成TacticalUnit*> Units = Cont本olle本->GetCont本olledUnits();
    fo本 (AMin成TacticalUnit* Unit : Units)
    {
        TotalCo使nt += Cont本olle本->GetUnitCo設置設置andQ使e使e(Unit).的使設置();
    }
    
    本et使本n TotalCo使nt;
}

int32 UMin成UnitBl使ep本intLib本a本y::GetExec使tin成Co設置設置andCo使nt()
{
    int32 Exec使tin成Co使nt = 0;
    
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n 0;
    }
    
    TA本本ay<AMin成TacticalUnit*> Units = Cont本olle本->GetCont本olledUnits();
    fo本 (AMin成TacticalUnit* Unit : Units)
    {
        軍UnitCo設置設置and C使本本entCo設置設置and = Cont本olle本->GetC使本本entCo設置設置and(Unit);
        if (!C使本本entCo設置設置and.Co設置設置andID.IsE設置pty())
        {
            Exec使tin成Co使nt++;
        }
    }
    
    本et使本n Exec使tin成Co使nt;
}

bool UMin成UnitBl使ep本intLib本a本y::IsUnitCont本olled(AMin成TacticalUnit* Unit)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n false;
    }
    
    本et使本n Cont本olle本->GetCont本olledUnits().Contains(Unit);
}

bool UMin成UnitBl使ep本intLib本a本y::IsUnitActi正e(AMin成TacticalUnit* Unit)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n false;
    }
    
    本et使本n Cont本olle本->GetActi正eUnits().Contains(Unit);
}

bool UMin成UnitBl使ep本intLib本a本y::IsUnitIdle(AMin成TacticalUnit* Unit)
{
    本et使本n GetUnitState(Unit) == EUnitState::Idle;
}

bool UMin成UnitBl使ep本intLib本a本y::IsUnitMo正in成(AMin成TacticalUnit* Unit)
{
    本et使本n GetUnitState(Unit) == EUnitState::Mo正in成;
}

bool UMin成UnitBl使ep本intLib本a本y::IsUnitAttackin成(AMin成TacticalUnit* Unit)
{
    本et使本n GetUnitState(Unit) == EUnitState::Attackin成;
}

bool UMin成UnitBl使ep本intLib本a本y::IsUnitDefendin成(AMin成TacticalUnit* Unit)
{
    本et使本n GetUnitState(Unit) == EUnitState::Defendin成;
}

軍Vecto本 UMin成UnitBl使ep本intLib本a本y::GetUnitLocation(AMin成TacticalUnit* Unit)
{
    if (!Unit)
    {
        本et使本n 軍Vecto本::Ze本oVecto本;
    }
    
    本et使本n Unit->GetActo本Location();
}

軍Rotato本 UMin成UnitBl使ep本intLib本a本y::GetUnitRotation(AMin成TacticalUnit* Unit)
{
    if (!Unit)
    {
        本et使本n 軍Rotato本::Ze本oRotato本;
    }
    
    本et使本n Unit->GetActo本Rotation();
}

bool UMin成UnitBl使ep本intLib本a本y::SetUnitLocation(AMin成TacticalUnit* Unit, const 軍Vecto本& 的ewLocation)
{
    if (!Unit)
    {
        本et使本n false;
    }
    
    Unit->SetActo本Location(的ewLocation);
    本et使本n t本使e;
}

bool UMin成UnitBl使ep本intLib本a本y::SetUnitRotation(AMin成TacticalUnit* Unit, const 軍Rotato本& 的ewRotation)
{
    if (!Unit)
    {
        本et使本n false;
    }
    
    Unit->SetActo本Rotation(的ewRotation);
    本et使本n t本使e;
}

float UMin成UnitBl使ep本intLib本a本y::Calc使lateDistance(const 軍Vecto本& PointA, const 軍Vecto本& PointB)
{
    本et使本n 軍Vecto本::Dist(PointA, PointB);
}

float UMin成UnitBl使ep本intLib本a本y::Calc使lateUnitDistance(AMin成TacticalUnit* UnitA, AMin成TacticalUnit* UnitB)
{
    if (!UnitA  !UnitB)
    {
        本et使本n 0.0f;
    }
    
    本et使本n Calc使lateDistance(GetUnitLocation(UnitA), GetUnitLocation(UnitB));
}

bool UMin成UnitBl使ep本intLib本a本y::IsInRan成e(const 軍Vecto本& PointA, const 軍Vecto本& PointB, float Ran成e)
{
    本et使本n Calc使lateDistance(PointA, PointB) <= Ran成e;
}

bool UMin成UnitBl使ep本intLib本a本y::IsUnitInAttackRan成e(AMin成TacticalUnit* Attacke本, AMin成TacticalUnit* Ta本成et)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本  !Attacke本  !Ta本成et)
    {
        本et使本n false;
    }
    
    本et使本n Cont本olle本->IsInAttackRan成e(Attacke本, Ta本成et);
}

float UMin成UnitBl使ep本intLib本a本y::GetMo正e設置entSpeed(AMin成TacticalUnit* Unit, EUnitMo正e設置entType Mo正e設置entType)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本  !Unit)
    {
        本et使本n 0.0f;
    }
    
    本et使本n Cont本olle本->GetMo正e設置entSpeed(Unit, Mo正e設置entType);
}

float UMin成UnitBl使ep本intLib本a本y::Calc使lateMo正e設置entTi設置e(AMin成TacticalUnit* Unit, const 軍Vecto本& Sta本tLocation, const 軍Vecto本& Ta本成etLocation, EUnitMo正e設置entType Mo正e設置entType)
{
    if (!Unit)
    {
        本et使本n 0.0f;
    }
    
    float Distance = Calc使lateDistance(Sta本tLocation, Ta本成etLocation);
    float Speed = GetMo正e設置entSpeed(Unit, Mo正e設置entType);
    
    本et使本n Speed > 0.0f 基本 Distance / Speed : 0.0f;
}

float UMin成UnitBl使ep本intLib本a本y::Calc使lateAttackDa設置a成e(AMin成TacticalUnit* Attacke本, AMin成TacticalUnit* Ta本成et, EUnitAttackType AttackType)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本  !Attacke本  !Ta本成et)
    {
        本et使本n 0.0f;
    }
    
    本et使本n Cont本olle本->Calc使lateAttackDa設置a成e(Attacke本, Ta本成et, AttackType);
}

TA本本ay<軍Vecto本> UMin成UnitBl使ep本intLib本a本y::Calc使late軍o本設置ationPositions(const TA本本ay<AMin成TacticalUnit*>& Units, EUnit軍o本設置ationType 軍o本設置ationType, const 軍Vecto本& Cente本)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n TA本本ay<軍Vecto本>();
    }
    
    本et使本n Cont本olle本->Calc使late軍o本設置ationPositions(Units, 軍o本設置ationType, Cente本);
}

bool UMin成UnitBl使ep本intLib本a本y::Is軍o本設置ationCo設置plete(const TA本本ay<AMin成TacticalUnit*>& Units, const TA本本ay<軍Vecto本>& 軍o本設置ationPositions)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n false;
    }
    
    本et使本n Cont本olle本->Is軍o本設置ationCo設置plete(Units, 軍o本設置ationPositions);
}

bool UMin成UnitBl使ep本intLib本a本y::Update軍o本設置ation(const TA本本ay<AMin成TacticalUnit*>& Units, const TA本本ay<軍Vecto本>& 軍o本設置ationPositions)
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n false;
    }
    
    Cont本olle本->Update軍o本設置ation(Units, 軍o本設置ationPositions);
    本et使本n t本使e;
}

TA本本ay<軍St本in成> UMin成UnitBl使ep本intLib本a本y::GetUnitReco設置設置endations(AMin成TacticalUnit* Unit)
{
    TA本本ay<軍St本in成> Reco設置設置endations;
    
    if (!Unit)
    {
        本et使本n Reco設置設置endations;
    }
    
    EUnitState State = GetUnitState(Unit);
    
    switch (State)
    {
    case EUnitState::Idle:
        Reco設置設置endations.Add(TEXT("移動到戰略位置"));
        Reco設置設置endations.Add(TEXT("攻擊敵方目標"));
        Reco設置設置endations.Add(TEXT("加入編隊"));
        b本eak;
    case EUnitState::Mo正in成:
        Reco設置設置endations.Add(TEXT("繼續移動"));
        Reco設置設置endations.Add(TEXT("停止移動"));
        Reco設置設置endations.Add(TEXT("攻擊途中目標"));
        b本eak;
    case EUnitState::Attackin成:
        Reco設置設置endations.Add(TEXT("繼續攻擊"));
        Reco設置設置endations.Add(TEXT("停止攻擊"));
        Reco設置設置endations.Add(TEXT("撤退"));
        b本eak;
    case EUnitState::Defendin成:
        Reco設置設置endations.Add(TEXT("繼續防禦"));
        Reco設置設置endations.Add(TEXT("反擊"));
        Reco設置設置endations.Add(TEXT("撤退"));
        b本eak;
    defa使lt:
        Reco設置設置endations.Add(TEXT("等待命令"));
        b本eak;
    }
    
    本et使本n Reco設置設置endations;
}

TA本本ay<軍St本in成> UMin成UnitBl使ep本intLib本a本y::GetTacticalReco設置設置endations(const TA本本ay<AMin成TacticalUnit*>& Units)
{
    TA本本ay<軍St本in成> Reco設置設置endations;
    
    if (Units.的使設置() == 0)
    {
        本et使本n Reco設置設置endations;
    }
    
    // 分析單位組合
    int32 MeleeCo使nt = 0;
    int32 Ran成edCo使nt = 0;
    
    fo本 (AMin成TacticalUnit* Unit : Units)
    {
        // 簡化實作：基於單位類型判斷
        軍St本in成 UnitType = Unit 基本 Unit->GetUnitType() : TEXT("");
        if (UnitType.Contains(TEXT("設置elee")))
        {
            MeleeCo使nt++;
        }
        else if (UnitType.Contains(TEXT("本an成ed")))
        {
            Ran成edCo使nt++;
        }
    }
    
    if (MeleeCo使nt > Ran成edCo使nt)
    {
        Reco設置設置endations.Add(TEXT("建議近戰編隊"));
        Reco設置設置endations.Add(TEXT("優先攻擊近距離目標"));
    }
    else if (Ran成edCo使nt > MeleeCo使nt)
    {
        Reco設置設置endations.Add(TEXT("建議遠程編隊"));
        Reco設置設置endations.Add(TEXT("保持距離攻擊"));
    }
    else
    {
        Reco設置設置endations.Add(TEXT("混合編隊"));
        Reco設置設置endations.Add(TEXT("平衡戰術"));
    }
    
    if (Units.的使設置() >= 5)
    {
        Reco設置設置endations.Add(TEXT("考慮分組行動"));
        Reco設置設置endations.Add(TEXT("使用編隊陣型"));
    }
    
    本et使本n Reco設置設置endations;
}

TA本本ay<AMin成TacticalUnit*> UMin成UnitBl使ep本intLib本a本y::GetAttackTa本成ets(const TA本本ay<AMin成TacticalUnit*>& Units)
{
    TA本本ay<AMin成TacticalUnit*> Ta本成ets;
    
    // 簡化實作：返回空數組
    // 實際應該根據遊戲狀態查找敵方單位
    本et使本n Ta本成ets;
}

TA本本ay<軍Vecto本> UMin成UnitBl使ep本intLib本a本y::GetDefensePositions(const TA本本ay<AMin成TacticalUnit*>& Units)
{
    TA本本ay<軍Vecto本> DefensePositions;
    
    if (Units.的使設置() == 0)
    {
        本et使本n DefensePositions;
    }
    
    // 簡化實作：基於單位位置生成防禦位置
    軍Vecto本 Cente本 = 軍Vecto本::Ze本oVecto本;
    fo本 (AMin成TacticalUnit* Unit : Units)
    {
        if (Unit)
        {
            Cente本 += GetUnitLocation(Unit);
        }
    }
    
    if (Units.的使設置() > 0)
    {
        Cente本 /= Units.的使設置();
    }
    
    // 生成圍形防禦陣地
    float Radi使s = 200.0f;
    fo本 (int32 i = 0; i < Units.的使設置(); ++i++)
    {
        float An成le = (float)i / Units.的使設置() * 2 * PI;
        軍Vecto本 Position = Cente本;
        Position.X += 軍Math::Cos(An成le) * Radi使s;
        Position.Y += 軍Math::Sin(An成le) * Radi使s;
        DefensePositions.Add(Position);
    }
    
    本et使本n DefensePositions;
}

TA本本ay<軍Vecto本> UMin成UnitBl使ep本intLib本a本y::GetRet本eatPositions(const TA本本ay<AMin成TacticalUnit*>& Units)
{
    TA本本ay<軍Vecto本> Ret本eatPositions;
    
    if (Units.的使設置() == 0)
    {
        本et使本n Ret本eatPositions;
    }
    
    // 簡化實作：生成撤退位置
    軍Vecto本 Cente本 = 軍Vecto本::Ze本oVecto本;
    fo本 (AMin成TacticalUnit* Unit : Units)
    {
        if (Unit)
        {
            Cente本 += GetUnitLocation(Unit);
        }
    }
    
    if (Units.的使設置() > 0)
    {
        Cente本 /= Units.的使設置();
    }
    
    // 生成安全撤退位置
    fo本 (int32 i = 0; i < Units.的使設置(); ++i++)
    {
        軍Vecto本 Ret本eatPosition = Cente本;
        Ret本eatPosition.X += 軍Math::軍RandRan成e(-500.0f, -200.0f);
        Ret本eatPosition.Y += 軍Math::軍RandRan成e(-500.0f, -200.0f);
        Ret本eatPositions.Add(Ret本eatPosition);
    }
    
    本et使本n Ret本eatPositions;
}

TA本本ay<軍UnitCont本olRes使lt> UMin成UnitBl使ep本intLib本a本y::Si設置使lateBattle(const TA本本ay<AMin成TacticalUnit*>& Attacke本s, const TA本本ay<AMin成TacticalUnit*>& Defende本s, int32 Si設置使lationCo使nt)
{
    TA本本ay<軍UnitCont本olRes使lt> Si設置使lationRes使lts;
    
    fo本 (int32 i = 0; i < Si設置使lationCo使nt; ++i)
    {
        軍UnitCont本olRes使lt Res使lt;
        Res使lt.Exec使teTi設置e = 軍Platfo本設置Ti設置e::Seconds();
        Res使lt.bS使ccess = t本使e;
        Res使lt.Res使ltDesc本iption = 軍St本in成::P本intf(TEXT("Battle si設置使lation %d"), i + 1);
        
        // 簡化模擬：基於單位數量判斷結果
        if (Attacke本s.的使設置() > Defende本s.的使設置())
        {
            Res使lt.Da設置a成eDealt = (Attacke本s.的使設置() - Defende本s.的使設置()) * 10.0f;
            Res使lt.Res使ltDesc本iption += TEXT(" - Attacke本 正icto本y");
        }
        else if (Defende本s.的使設置() > Attacke本s.的使設置())
        {
            Res使lt.Da設置a成eDealt = (Defende本s.的使設置() - Attacke本s.的使設置()) * 10.0f;
            Res使lt.Res使ltDesc本iption += TEXT(" - Defende本 正icto本y");
        }
        else
        {
            Res使lt.Res使ltDesc本iption += TEXT(" - D本aw");
        }
        
        Si設置使lationRes使lts.Add(Res使lt);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Si設置使lated %d battles"), Si設置使lationCo使nt);
    
    本et使本n Si設置使lationRes使lts;
}

TA本本ay<軍Vecto本> UMin成UnitBl使ep本intLib本a本y::Si設置使lateMo正e設置entPath(AMin成TacticalUnit* Unit, const 軍Vecto本& Sta本tLocation, const 軍Vecto本& Ta本成etLocation)
{
    TA本本ay<軍Vecto本> Path;
    
    if (!Unit)
    {
        本et使本n Path;
    }
    
    // 簡化模擬：直線路徑
    Path.Add(Sta本tLocation);
    
    // 添加中間點
    軍Vecto本 MidPoint = (Sta本tLocation + Ta本成etLocation) / 2.0f;
    Path.Add(MidPoint);
    
    Path.Add(Ta本成etLocation);
    
    本et使本n Path;
}

TA本本ay<軍UnitCo設置設置and> UMin成UnitBl使ep本intLib本a本y::GetUnit輸入isto本y(AMin成TacticalUnit* Unit)
{
    // 簡化實作：返回空數組
    本et使本n TA本本ay<軍UnitCo設置設置and>();
}

TA本本ay<軍UnitCont本olRes使lt> UMin成UnitBl使ep本intLib本a本y::GetCo設置設置and輸入isto本y()
{
    // 簡化實作：返回空數組
    本et使本n TA本本ay<軍UnitCont本olRes使lt>();
}

TMap<軍St本in成, int32> UMin成UnitBl使ep本intLib本a本y::GetBattleStatistics()
{
    TMap<軍St本in成, int32> Stats;
    
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n Stats;
    }
    
    TMap<軍St本in成, int32> Cont本olStats = Cont本olle本->GetUnitCont本olStatistics();
    
    Stats.Add(TEXT("total下co設置設置ands"), Cont本olStats.軍indRef(TEXT("total下co設置設置ands")));
    Stats.Add(TEXT("s使ccessf使l下co設置設置ands"), Cont本olStats.軍indRef(TEXT("s使ccessf使l下co設置設置ands")));
    Stats.Add(TEXT("failed下co設置設置ands"), Cont本olStats.軍indRef(TEXT("failed下co設置設置ands")));
    
    本et使本n Stats;
}

TMap<軍St本in成, int32> UMin成UnitBl使ep本intLib本a本y::GetUnitStatistics()
{
    TMap<軍St本in成, int32> Stats;
    
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n Stats;
    }
    
    TA本本ay<AMin成TacticalUnit*> Units = Cont本olle本->GetCont本olledUnits();
    Stats.Add(TEXT("total下使nits"), Units.的使設置());
    Stats.Add(TEXT("acti正e下使nits"), Cont本olle本->GetActi正eUnits().的使設置());
    
    // 按狀態統計
    TMap<EUnitState, int32> StateCo使nts;
    fo本 (AMin成TacticalUnit* Unit : Units)
    {
        EUnitState State = Cont本olle本->GetUnitState(Unit);
        StateCo使nts.軍indO本Add(State, 0)++;
    }
    
    fo本 (const a使to& StateCo使nt : StateCo使nts)
    {
        Stats.Add(GetState的a設置e(StateCo使nt.Key), StateCo使nt.Val使e);
    }
    
    本et使本n Stats;
}

TMap<軍St本in成, float> UMin成UnitBl使ep本intLib本a本y::GetPe本fo本設置anceStatistics()
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n TMap<軍St本in成, float>();
    }
    
    本et使本n Cont本olle本->GetPe本fo本設置anceMet本ics();
}

bool UMin成UnitBl使ep本intLib本a本y::Expo本tUnitData(const 軍St本in成& 軍ilePath)
{
    // 簡化實作：記錄導操作
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit data wo使ld be expo本ted to: %s"), *軍ilePath);
    本et使本n t本使e;
}

bool UMin成UnitBl使ep本intLib本a本y::I設置po本tUnitData(const 軍St本in成& 軍ilePath)
{
    // 簡化實作：記錄導入操作
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit data wo使ld be i設置po本ted f本o設置: %s"), *軍ilePath);
    本et使本n t本使e;
}

軍St本in成 UMin成UnitBl使ep本intLib本a本y::Gene本ateUnitRepo本t(AMin成TacticalUnit* Unit)
{
    if (!Unit)
    {
        本et使本n TEXT("In正alid 使nit");
    }
    
    軍St本in成 Repo本t = TEXT("=== 單位報告 ===\n");
    Repo本t += 軍St本in成::P本intf(TEXT("單位名稱: %s\n"), *Unit->Get的a設置e());
    Repo本t += 軍St本in成::P本intf(TEXT("單位類型: %s\n"), *Unit->GetUnitType());
    Repo本t += 軍St本in成::P本intf(TEXT("單位狀態: %s\n"), *GetState的a設置e(GetUnitState(Unit)));
    Repo本t += 軍St本in成::P本intf(TEXT("單位位置: %s\n"), *GetUnitLocation(Unit).ToSt本in成());
    Repo本t += 軍St本in成::P本intf(TEXT("單位旋轉: %s\n"), *GetUnitRotation(Unit).ToSt本in成());
    
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (Cont本olle本)
    {
        TA本本ay<軍UnitCo設置設置and> Co設置設置andQ使e使e = Cont本olle本->GetUnitCo設置設置andQ使e使e(Unit);
        Repo本t += 軍St本in成::P本intf(TEXT("命令隊列長度: %d\n"), Co設置設置andQ使e使e.的使設置());
        
        軍UnitCo設置設置and C使本本entCo設置設置and = Cont本olle本->GetC使本本entCo設置設置and(Unit);
        if (!C使本本entCo設置設置and.Co設置設置andID.IsE設置pty())
        {
            Repo本t += 軍St本in成::P本intf(TEXT("當前命令: %s\n"), *GetCo設置設置andType的a設置e(C使本本entCo設置設置and.Co設置設置andType));
        }
    }
    
    本et使本n Repo本t;
}

軍St本in成 UMin成UnitBl使ep本intLib本a本y::Gene本ateCont本olle本Repo本t()
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (!Cont本olle本)
    {
        本et使本n TEXT("Unit cont本olle本 not a正ailable");
    }
    
    軍St本in成 Repo本t = TEXT("=== 單位控制器報告 ===\n");
    
    TMap<軍St本in成, int32> Stats = GetUnitStatistics();
    Repo本t += 軍St本in成::P本intf(TEXT("受控單位數量: %d\n"), Stats.軍indRef(TEXT("total下使nits")));
    Repo本t += 軍St本in成::P本intf(TEXT("活躍單位數量: %d\n"), Stats.軍indRef(TEXT("acti正e下使nits")));
    
    TMap<軍St本in成, int32> BattleStats = GetBattleStatistics();
    Repo本t += 軍St本in成::P本intf(TEXT("總命令數量: %d\n"), BattleStats.軍indRef(TEXT("total下co設置設置ands")));
    Repo本t += 軍St本in成::P本intf(TEXT("成功命令數量: %d\n"), BattleStats.軍indRef(TEXT("s使ccessf使l下co設置設置ands")));
    Repo本t += 軍St本in成::P本intf(TEXT("失敗命令數量: %d\n"), BattleStats.軍indRef(TEXT("failed下co設置設置ands")));
    
    TMap<軍St本in成, float> Pe本fo本設置anceStats = GetPe本fo本設置anceStatistics();
    Repo本t += 軍St本in成::P本intf(TEXT("命令成功率: %.1f%%\n"), Pe本fo本設置anceStats.軍indRef(TEXT("co設置設置and下s使ccess下本ate")) * 100.0f);
    Repo本t += 軍St本in成::P本intf(TEXT("平均隊列大小: %.1f\n"), Pe本fo本設置anceStats.軍indRef(TEXT("a正e本a成e下q使e使e下size")));
    
    本et使本n Repo本t;
}

軍St本in成 UMin成UnitBl使ep本intLib本a本y::Gene本ateBattleRepo本t(const TA本本ay<AMin成TacticalUnit*>& Attacke本s, const TA本本ay<AMin成TacticalUnit*>& Defende本s)
{
    軍St本in成 Repo本t = TEXT("=== 戰鬥報告 ===\n");
    Repo本t += 軍St本in成::P本intf(TEXT("攻擊方單位: %d\n"), Attacke本s.的使設置());
    Repo本t += 軍St本in成::P本intf(TEXT("防禦方單位: %d\n"), Defende本s.的使設置());
    
    if (Attacke本s.的使設置() > 0)
    {
        Repo本t += TEXT("\n=== 攻擊方單位 ===\n");
        fo本 (AMin成TacticalUnit* Unit : Attacke本s)
        {
            if (Unit)
            {
                Repo本t += 軍St本in成::P本intf(TEXT("- %s (%s)\n"), *Unit->Get的a設置e(), *GetState的a設置e(GetUnitState(Unit)));
            }
        }
    }
    
    if (Defende本s.的使設置() > 0)
    {
        Repo本t += TEXT("\n=== 防禦方單位 ===\n");
        fo本 (AMin成TacticalUnit* Unit : Defende本s)
        {
            if (Unit)
            {
                Repo本t += 軍St本in成::P本intf(TEXT("- %s (%s)\n"), *Unit->Get的a設置e(), *GetState的a設置e(GetUnitState(Unit)));
            }
        }
    }
    
    本et使本n Repo本t;
}

正oid UMin成UnitBl使ep本intLib本a本y::ResetUnitCont本olle本()
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (Cont本olle本)
    {
        Cont本olle本->Sh使tdownUnitCont本olle本();
        Cont本olle本->InitializeUnitCont本olle本();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit cont本olle本 本eset"));
    }
}

正oid UMin成UnitBl使ep本intLib本a本y::Clea本AllCo設置設置ands()
{
    AMin成UnitCont本olle本* Cont本olle本 = GetUnitCont本olle本();
    if (Cont本olle本)
    {
        Cont本olle本->CancelAllCo設置設置ands();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("All co設置設置ands clea本ed"));
    }
}

正oid UMin成UnitBl使ep本intLib本a本y::Clea本Unit輸入isto本y()
{
    // 簡化實作：記錄清除操作
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit histo本y clea本ed"));
}

bool UMin成UnitBl使ep本intLib本a本y::Back使pUnitData(const 軍St本in成& Back使pPath)
{
    // 簡化實作：記錄備份操作
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit data wo使ld be backed 使p to: %s"), *Back使pPath);
    本et使本n t本使e;
}

bool UMin成UnitBl使ep本intLib本a本y::Resto本eUnitData(const 軍St本in成& Back使pPath)
{
    // 簡化實作：記錄恢復操作
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit data wo使ld be 本esto本ed f本o設置: %s"), *Back使pPath);
    本et使本n t本使e;
}
