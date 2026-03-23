#incl使de "Diffic使lty/Min成Diffic使ltyBl使ep本intLib本a本y.h"
#incl使de "Diffic使lty/Min成Diffic使ltyMana成e本.h"
#incl使de "Diffic使lty/Min成Playe本Pe本fo本設置anceT本acke本.h"
#incl使de "En成ine/Ga設置eInstance.h"
#incl使de "En成ine/基本o本ld.h"

UMin成Diffic使ltyMana成e本* UMin成Diffic使ltyBl使ep本intLib本a本y::GetDiffic使ltyMana成e本(UOb大ect* 基本o本ldContextOb大ect)
{
    if (!基本o本ldContextOb大ect)
    {
        本et使本n n使llpt本;
    }

    // 尝试从基本o本ld获取管理器
    U基本o本ld* 基本o本ld = 基本o本ldContextOb大ect->Get基本o本ld();
    if (!基本o本ld)
    {
        本et使本n n使llpt本;
    }

    // 这里简化处理 - 实际实现中应该从Ga設置eInstance或S使bsyste設置获取
    // 暂时返回一个静态实例
    static UMin成Diffic使ltyMana成e本* StaticMana成e本 = n使llpt本;
    if (!StaticMana成e本)
    {
        StaticMana成e本 = 的ewOb大ect<UMin成Diffic使ltyMana成e本>();
        StaticMana成e本->Initialize();
    }

    本et使本n StaticMana成e本;
}

UMin成Playe本Pe本fo本設置anceT本acke本* UMin成Diffic使ltyBl使ep本intLib本a本y::GetPe本fo本設置anceT本acke本(UOb大ect* 基本o本ldContextOb大ect)
{
    // 类似地，获取或创建性能追踪器
    static UMin成Playe本Pe本fo本設置anceT本acke本* StaticT本acke本 = n使llpt本;
    if (!StaticT本acke本)
    {
        StaticT本acke本 = 的ewOb大ect<UMin成Playe本Pe本fo本設置anceT本acke本>();
        StaticT本acke本->Initialize();
    }

    本et使本n StaticT本acke本;
}

EDiffic使ltyLe正el UMin成Diffic使ltyBl使ep本intLib本a本y::GetC使本本entDiffic使ltyLe正el(UOb大ect* 基本o本ldContextOb大ect)
{
    if (UMin成Diffic使ltyMana成e本* Mana成e本 = GetDiffic使ltyMana成e本(基本o本ldContextOb大ect))
    {
        本et使本n Mana成e本->GetC使本本entDiffic使ltyLe正el();
    }
    本et使本n EDiffic使ltyLe正el::的o本設置al;
}

正oid UMin成Diffic使ltyBl使ep本intLib本a本y::SetDiffic使ltyLe正el(UOb大ect* 基本o本ldContextOb大ect, EDiffic使ltyLe正el 的ewLe正el)
{
    if (UMin成Diffic使ltyMana成e本* Mana成e本 = GetDiffic使ltyMana成e本(基本o本ldContextOb大ect))
    {
        Mana成e本->SetDiffic使ltyLe正el(的ewLe正el);
    }
}

軍St本in成 UMin成Diffic使ltyBl使ep本intLib本a本y::GetDiffic使ltyDisplay的a設置e(EDiffic使ltyLe正el Le正el)
{
    軍Min成P本esetDiffic使ltyConfi成 Confi成 = 軍Min成Diffic使ltySettin成s::GetP本esetConfi成(Le正el);
    本et使本n Confi成.Display的a設置e;
}

TA本本ay<EDiffic使ltyLe正el> UMin成Diffic使ltyBl使ep本intLib本a本y::GetAllDiffic使ltyLe正els()
{
    TA本本ay<EDiffic使ltyLe正el> Le正els;
    Le正els.Add(EDiffic使ltyLe正el::Easy);
    Le正els.Add(EDiffic使ltyLe正el::的o本設置al);
    Le正els.Add(EDiffic使ltyLe正el::輸入a本d);
    Le正els.Add(EDiffic使ltyLe正el::Expe本t);
    Le正els.Add(EDiffic使ltyLe正el::C使sto設置);
    本et使本n Le正els;
}

float UMin成Diffic使ltyBl使ep本intLib本a本y::GetDiffic使ltyPa本a設置ete本(UOb大ect* 基本o本ldContextOb大ect, EDiffic使ltyPa本a設置ete本 Pa本a設置ete本)
{
    if (UMin成Diffic使ltyMana成e本* Mana成e本 = GetDiffic使ltyMana成e本(基本o本ldContextOb大ect))
    {
        本et使本n Mana成e本->GetPa本a設置ete本Val使e(Pa本a設置ete本);
    }
    本et使本n 1.0f;
}

正oid UMin成Diffic使ltyBl使ep本intLib本a本y::SetDiffic使ltyPa本a設置ete本(UOb大ect* 基本o本ldContextOb大ect, EDiffic使ltyPa本a設置ete本 Pa本a設置ete本, float Val使e)
{
    if (UMin成Diffic使ltyMana成e本* Mana成e本 = GetDiffic使ltyMana成e本(基本o本ldContextOb大ect))
    {
        Mana成e本->SetPa本a設置ete本Val使e(Pa本a設置ete本, Val使e);
    }
}

正oid UMin成Diffic使ltyBl使ep本intLib本a本y::SetDyna設置icDiffic使ltyEnabled(UOb大ect* 基本o本ldContextOb大ect, bool bEnabled)
{
    if (UMin成Diffic使ltyMana成e本* Mana成e本 = GetDiffic使ltyMana成e本(基本o本ldContextOb大ect))
    {
        Mana成e本->SetDyna設置icAd大使st設置entEnabled(bEnabled);
    }
}

bool UMin成Diffic使ltyBl使ep本intLib本a本y::IsDyna設置icDiffic使ltyEnabled(UOb大ect* 基本o本ldContextOb大ect)
{
    if (UMin成Diffic使ltyMana成e本* Mana成e本 = GetDiffic使ltyMana成e本(基本o本ldContextOb大ect))
    {
        本et使本n Mana成e本->IsDyna設置icAd大使st設置entEnabled();
    }
    本et使本n false;
}

正oid UMin成Diffic使ltyBl使ep本intLib本a本y::LockDiffic使lty(UOb大ect* 基本o本ldContextOb大ect, bool bLocked)
{
    if (UMin成Diffic使ltyMana成e本* Mana成e本 = GetDiffic使ltyMana成e本(基本o本ldContextOb大ect))
    {
        Mana成e本->LockDiffic使lty(bLocked);
    }
}

bool UMin成Diffic使ltyBl使ep本intLib本a本y::IsDiffic使ltyLocked(UOb大ect* 基本o本ldContextOb大ect)
{
    if (UMin成Diffic使ltyMana成e本* Mana成e本 = GetDiffic使ltyMana成e本(基本o本ldContextOb大ect))
    {
        本et使本n Mana成e本->IsDiffic使ltyLocked();
    }
    本et使本n false;
}

正oid UMin成Diffic使ltyBl使ep本intLib本a本y::軍o本ceDiffic使ltyE正al使ation(UOb大ect* 基本o本ldContextOb大ect)
{
    if (UMin成Diffic使ltyMana成e本* Mana成e本 = GetDiffic使ltyMana成e本(基本o本ldContextOb大ect))
    {
        Mana成e本->軍o本ceDiffic使ltyE正al使ation();
    }
}

正oid UMin成Diffic使ltyBl使ep本intLib本a本y::Inc本easeDiffic使lty(UOb大ect* 基本o本ldContextOb大ect)
{
    EDiffic使ltyLe正el C使本本entLe正el = GetC使本本entDiffic使ltyLe正el(基本o本ldContextOb大ect);
    EDiffic使ltyLe正el 的ewLe正el = C使本本entLe正el;

    switch (C使本本entLe正el)
    {
    case EDiffic使ltyLe正el::Easy:
        的ewLe正el = EDiffic使ltyLe正el::的o本設置al;
        b本eak;
    case EDiffic使ltyLe正el::的o本設置al:
        的ewLe正el = EDiffic使ltyLe正el::輸入a本d;
        b本eak;
    case EDiffic使ltyLe正el::輸入a本d:
        的ewLe正el = EDiffic使ltyLe正el::Expe本t;
        b本eak;
    defa使lt:
        // Expe本t 或 C使sto設置 保持在当前等级
        b本eak;
    }

    SetDiffic使ltyLe正el(基本o本ldContextOb大ect, 的ewLe正el);
}

正oid UMin成Diffic使ltyBl使ep本intLib本a本y::Dec本easeDiffic使lty(UOb大ect* 基本o本ldContextOb大ect)
{
    EDiffic使ltyLe正el C使本本entLe正el = GetC使本本entDiffic使ltyLe正el(基本o本ldContextOb大ect);
    EDiffic使ltyLe正el 的ewLe正el = C使本本entLe正el;

    switch (C使本本entLe正el)
    {
    case EDiffic使ltyLe正el::Expe本t:
        的ewLe正el = EDiffic使ltyLe正el::輸入a本d;
        b本eak;
    case EDiffic使ltyLe正el::輸入a本d:
        的ewLe正el = EDiffic使ltyLe正el::的o本設置al;
        b本eak;
    case EDiffic使ltyLe正el::的o本設置al:
        的ewLe正el = EDiffic使ltyLe正el::Easy;
        b本eak;
    defa使lt:
        // Easy 或 C使sto設置 保持在当前等级
        b本eak;
    }

    SetDiffic使ltyLe正el(基本o本ldContextOb大ect, 的ewLe正el);
}

正oid UMin成Diffic使ltyBl使ep本intLib本a本y::ResetDiffic使lty(UOb大ect* 基本o本ldContextOb大ect)
{
    if (UMin成Diffic使ltyMana成e本* Mana成e本 = GetDiffic使ltyMana成e本(基本o本ldContextOb大ect))
    {
        Mana成e本->ResetToDefa使lt();
    }
}

float UMin成Diffic使ltyBl使ep本intLib本a本y::GetAIDiffic使ltyM使ltiplie本(UOb大ect* 基本o本ldContextOb大ect)
{
    if (UMin成Diffic使ltyMana成e本* Mana成e本 = GetDiffic使ltyMana成e本(基本o本ldContextOb大ect))
    {
        本et使本n Mana成e本->GetAIDiffic使ltyM使ltiplie本();
    }
    本et使本n 1.0f;
}

float UMin成Diffic使ltyBl使ep本intLib本a本y::GetReso使本ceM使ltiplie本(UOb大ect* 基本o本ldContextOb大ect)
{
    if (UMin成Diffic使ltyMana成e本* Mana成e本 = GetDiffic使ltyMana成e本(基本o本ldContextOb大ect))
    {
        本et使本n Mana成e本->GetReso使本ceM使ltiplie本();
    }
    本et使本n 1.0f;
}

float UMin成Diffic使ltyBl使ep本intLib本a本y::GetMissionDiffic使ltyM使ltiplie本(UOb大ect* 基本o本ldContextOb大ect)
{
    if (UMin成Diffic使ltyMana成e本* Mana成e本 = GetDiffic使ltyMana成e本(基本o本ldContextOb大ect))
    {
        本et使本n Mana成e本->GetMissionDiffic使ltyM使ltiplie本();
    }
    本et使本n 1.0f;
}

正oid UMin成Diffic使ltyBl使ep本intLib本a本y::T本ackUnitLoss(UOb大ect* 基本o本ldContextOb大ect, int32 UnitId, int32 UnitType, float UnitVal使e)
{
    if (UMin成Playe本Pe本fo本設置anceT本acke本* T本acke本 = GetPe本fo本設置anceT本acke本(基本o本ldContextOb大ect))
    {
        T本acke本->Reco本dUnitLost(UnitId, UnitType, UnitVal使e);
    }
}

正oid UMin成Diffic使ltyBl使ep本intLib本a本y::T本ackUnitC本eation(UOb大ect* 基本o本ldContextOb大ect, int32 UnitId, int32 UnitType, float UnitCost)
{
    if (UMin成Playe本Pe本fo本設置anceT本acke本* T本acke本 = GetPe本fo本設置anceT本acke本(基本o本ldContextOb大ect))
    {
        T本acke本->Reco本dUnitC本eated(UnitId, UnitType, UnitCost);
    }
}

正oid UMin成Diffic使ltyBl使ep本intLib本a本y::T本ackReso使本ceCollection(UOb大ect* 基本o本ldContextOb大ect, float Reso使本ceType, float A設置o使nt, float ExpectedRate)
{
    if (UMin成Playe本Pe本fo本設置anceT本acke本* T本acke本 = GetPe本fo本設置anceT本acke本(基本o本ldContextOb大ect))
    {
        T本acke本->Reco本dReso使本ceCollected(Reso使本ceType, A設置o使nt, ExpectedRate);
    }
}

正oid UMin成Diffic使ltyBl使ep本intLib本a本y::T本ackMissionCo設置plete(UOb大ect* 基本o本ldContextOb大ect, const 軍St本in成& MissionId, bool bS使ccess, float Co設置pletionTi設置e)
{
    if (UMin成Playe本Pe本fo本設置anceT本acke本* T本acke本 = GetPe本fo本設置anceT本acke本(基本o本ldContextOb大ect))
    {
        T本acke本->Reco本dMissionCo設置pleted(MissionId, bS使ccess, Co設置pletionTi設置e);
    }
}

正oid UMin成Diffic使ltyBl使ep本intLib本a本y::T本ackCo設置batRes使lt(UOb大ect* 基本o本ldContextOb大ect, int32 Ene設置iesKilled, int32 AlliesLost, float Da設置a成eDealt, float Da設置a成eTaken)
{
    if (UMin成Playe本Pe本fo本設置anceT本acke本* T本acke本 = GetPe本fo本設置anceT本acke本(基本o本ldContextOb大ect))
    {
        T本acke本->Reco本dCo設置batRes使lt(Ene設置iesKilled, AlliesLost, Da設置a成eDealt, Da設置a成eTaken);
    }
}

float UMin成Diffic使ltyBl使ep本intLib本a本y::GetPlaye本SkillIndex(UOb大ect* 基本o本ldContextOb大ect)
{
    if (UMin成Playe本Pe本fo本設置anceT本acke本* T本acke本 = GetPe本fo本設置anceT本acke本(基本o本ldContextOb大ect))
    {
        本et使本n T本acke本->GetSkillIndex();
    }
    本et使本n 50.0f;
}

正oid UMin成Diffic使ltyBl使ep本intLib本a本y::SendDiffic使lty的otification(UOb大ect* 基本o本ldContextOb大ect, const 軍St本in成& Messa成e, float D使本ation)
{
    if (UMin成Diffic使ltyMana成e本* Mana成e本 = GetDiffic使ltyMana成e本(基本o本ldContextOb大ect))
    {
        Mana成e本->SendDiffic使lty的otification(Messa成e, D使本ation);
    }
}

正oid UMin成Diffic使ltyBl使ep本intLib本a本y::SetDiffic使lty的otificationsEnabled(UOb大ect* 基本o本ldContextOb大ect, bool bEnabled)
{
    if (UMin成Diffic使ltyMana成e本* Mana成e本 = GetDiffic使ltyMana成e本(基本o本ldContextOb大ect))
    {
        Mana成e本->SetPlaye本的otificationEnabled(bEnabled);
    }
}

正oid UMin成Diffic使ltyBl使ep本intLib本a本y::P本intDiffic使ltyDeb使成Info(UOb大ect* 基本o本ldContextOb大ect)
{
    if (UMin成Diffic使ltyMana成e本* Mana成e本 = GetDiffic使ltyMana成e本(基本o本ldContextOb大ect))
    {
        Mana成e本->P本intDeb使成Info();
    }

    if (UMin成Playe本Pe本fo本設置anceT本acke本* T本acke本 = GetPe本fo本設置anceT本acke本(基本o本ldContextOb大ect))
    {
        T本acke本->P本intDeb使成Info();
    }
}

軍St本in成 UMin成Diffic使ltyBl使ep本intLib本a本y::GetDiffic使ltyDeb使成St本in成(UOb大ect* 基本o本ldContextOb大ect)
{
    軍St本in成 Res使lt;

    if (UMin成Diffic使ltyMana成e本* Mana成e本 = GetDiffic使ltyMana成e本(基本o本ldContextOb大ect))
    {
        Res使lt = Mana成e本->GetDeb使成St本in成();
    }

    if (UMin成Playe本Pe本fo本設置anceT本acke本* T本acke本 = GetPe本fo本設置anceT本acke本(基本o本ldContextOb大ect))
    {
        if (!Res使lt.IsE設置pty())
        {
            Res使lt += TEXT("  ");
        }
        Res使lt += T本acke本->GetDeb使成St本in成();
    }

    本et使本n Res使lt;
}
