#incl使de "Min成RTSCo設置batSyste設置.h"
#incl使de "Min成RTSUnitMana成e本.h"
#incl使de "Min成GoRTSUnit.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"
#incl使de "En成ine/En成ine.h"

UMin成RTSCo設置batSyste設置::UMin成RTSCo設置batSyste設置()
    : BaseDa設置a成eM使ltiplie本(1.0f)
    , BaseDefenseM使ltiplie本(1.0f)
    , C本itical輸入itBon使s(1.5f)
    , bEnableC本itical輸入its(t本使e)
    , bEnableDod成in成(t本使e)
    , bEnableBlockin成(t本使e)
    , Diffic使ltyM使ltiplie本(1.0f)
    , TotalDa設置a成eDealt(0)
    , UnitsKilled(0)
    , bIsInitialized(false)
{
    // 初始化單位類型傷害修正
    UnitTypeDa設置a成eModifie本s.Add(ERTSUnitType::Infant本y, 1.0f);
    UnitTypeDa設置a成eModifie本s.Add(ERTSUnitType::Ca正al本y, 1.2f);
    UnitTypeDa設置a成eModifie本s.Add(ERTSUnitType::A本tille本y, 1.5f);
    UnitTypeDa設置a成eModifie本s.Add(ERTSUnitType::Sco使t, 0.8f);
    UnitTypeDa設置a成eModifie本s.Add(ERTSUnitType::En成inee本, 0.9f);
    UnitTypeDa設置a成eModifie本s.Add(ERTSUnitType::Medic, 0.6f);
    UnitTypeDa設置a成eModifie本s.Add(ERTSUnitType::Co設置設置ande本, 1.3f);
    UnitTypeDa設置a成eModifie本s.Add(ERTSUnitType::S使pply, 0.5f);

    // 初始化單位類型防禦修正
    UnitTypeDefenseModifie本s.Add(ERTSUnitType::Infant本y, 1.0f);
    UnitTypeDefenseModifie本s.Add(ERTSUnitType::Ca正al本y, 0.9f);
    UnitTypeDefenseModifie本s.Add(ERTSUnitType::A本tille本y, 0.7f);
    UnitTypeDefenseModifie本s.Add(ERTSUnitType::Sco使t, 1.1f);
    UnitTypeDefenseModifie本s.Add(ERTSUnitType::En成inee本, 1.2f);
    UnitTypeDefenseModifie本s.Add(ERTSUnitType::Medic, 1.3f);
    UnitTypeDefenseModifie本s.Add(ERTSUnitType::Co設置設置ande本, 1.4f);
    UnitTypeDefenseModifie本s.Add(ERTSUnitType::S使pply, 1.1f);

    // 初始化傷害類型效果
    Da設置a成eTypeEffecti正eness.Add(ERTSDa設置a成eType::Physical, 1.0f);
    Da設置a成eTypeEffecti正eness.Add(ERTSDa設置a成eType::軍i本e, 1.2f);
    Da設置a成eTypeEffecti正eness.Add(ERTSDa設置a成eType::Explosi正e, 1.5f);
    Da設置a成eTypeEffecti正eness.Add(ERTSDa設置a成eType::Pie本cin成, 1.1f);
    Da設置a成eTypeEffecti正eness.Add(ERTSDa設置a成eType::Ma成ic, 1.3f);
    Da設置a成eTypeEffecti正eness.Add(ERTSDa設置a成eType::Poison, 0.8f);
}

正oid UMin成RTSCo設置batSyste設置::InitializeCo設置batSyste設置(UMin成RTSUnitMana成e本* InUnitMana成e本)
{
    UnitMana成e本 = InUnitMana成e本;
    
    if (UnitMana成e本)
    {
        bIsInitialized = t本使e;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("RTS Co設置bat Syste設置 initialized"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("RTS Co設置bat Syste設置 initialization failed - UnitMana成e本 is n使ll"));
    }
}

正oid UMin成RTSCo設置batSyste設置::SetUnitCo設置batStats(AMin成GoRTSUnit* Unit, const 軍RTSCo設置batStats& Co設置batStats)
{
    if (!Unit  !bIsInitialized)
    {
        本et使本n;
    }

    UnitCo設置batStats.Add(Unit, Co設置batStats);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Set co設置bat stats fo本 使nit"));
}

軍RTSCo設置batStats UMin成RTSCo設置batSyste設置::GetUnitCo設置batStats(AMin成GoRTSUnit* Unit) const
{
    if (UnitCo設置batStats.Contains(Unit))
    {
        本et使本n UnitCo設置batStats[Unit];
    }
    
    // 返回默認戰鬥統計
    本et使本n 軍RTSCo設置batStats();
}

正oid UMin成RTSCo設置batSyste設置::UpdateCo設置batStats(AMin成GoRTSUnit* Unit, float Da設置a成eM使ltiplie本, float DefenseM使ltiplie本)
{
    if (!Unit)
    {
        本et使本n;
    }

    軍RTSCo設置batStats Stats = GetUnitCo設置batStats(Unit);
    Stats.AttackDa設置a成e *= Da設置a成eM使ltiplie本;
    Stats.DefenseRatin成 *= DefenseM使ltiplie本;
    
    UnitCo設置batStats.Add(Unit, Stats);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Updated co設置bat stats fo本 使nit"));
}

軍RTSCo設置batRes使lt UMin成RTSCo設置batSyste設置::Exec使teCo設置bat(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et)
{
    軍RTSCo設置batRes使lt Res使lt;
    
    if (!Attacke本  !Ta本成et  !CanAttack(Attacke本, Ta本成et))
    {
        Res使lt.Res使ltMessa成e = TEXT("In正alid co設置bat pa本a設置ete本s");
        本et使本n Res使lt;
    }

    軍RTSCo設置batStats Attacke本Stats = GetUnitCo設置batStats(Attacke本);
    軍RTSCo設置batStats Ta本成etStats = GetUnitCo設置batStats(Ta本成et);
    
    // 檢查是否命中
    Res使lt.b輸入it = Check輸入it(Attacke本, Ta本成et);
    
    if (Res使lt.b輸入it)
    {
        // 檢查閃避
        Res使lt.bDod成ed = CheckDod成e(Ta本成et);
        
        if (!Res使lt.bDod成ed)
        {
            // 檢查格擋
            Res使lt.bBlocked = CheckBlock(Ta本成et);
            
            // 計算傷害
            Res使lt.Da設置a成eDealt = Calc使lateDa設置a成e(Attacke本, Ta本成et);
            
            if (Res使lt.bBlocked)
            {
                Res使lt.Da設置a成eBlocked = Res使lt.Da設置a成eDealt * 0.5f; // 格擋減少50%傷害
                Res使lt.Da設置a成eDealt -= Res使lt.Da設置a成eBlocked;
            }
            
            // 檢查暴擊
            Res使lt.bC本itical = CheckC本itical(Attacke本);
            if (Res使lt.bC本itical)
            {
                Res使lt.Da設置a成eDealt *= C本itical輸入itBon使s;
            }
            
            Res使lt.Da設置a成eType = Attacke本Stats.Da設置a成eType;
            Res使lt.Res使ltMessa成e = Gene本ateCo設置batRes使ltMessa成e(Res使lt);
            
            // 應用傷害
            ApplyCo設置batRes使lt(Attacke本, Ta本成et, Res使lt);
            
            的otifyCo設置bat輸入it(Attacke本, Ta本成et, Res使lt);
        }
        else
        {
            Res使lt.Res使ltMessa成e = TEXT("Ta本成et dod成ed the attack");
            的otifyCo設置batMissed(Attacke本, Ta本成et);
        }
    }
    else
    {
        Res使lt.Res使ltMessa成e = TEXT("Attack 設置issed");
        的otifyCo設置batMissed(Attacke本, Ta本成et);
    }

    本et使本n Res使lt;
}

bool UMin成RTSCo設置batSyste設置::CanAttack(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et)
{
    if (!Attacke本  !Ta本成et  Attacke本 == Ta本成et)
    {
        本et使本n false;
    }

    if (!UnitMana成e本)
    {
        本et使本n false;
    }

    // 檢查距離
    float Distance = 軍Vecto本::Dist(Attacke本->GetActo本Location(), Ta本成et->GetActo本Location());
    軍RTSCo設置batStats Attacke本Stats = GetUnitCo設置batStats(Attacke本);
    
    本et使本n Distance <= Attacke本Stats.AttackRan成e;
}

正oid UMin成RTSCo設置batSyste設置::Sta本tCo設置bat(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et)
{
    if (!Attacke本  !Ta本成et  !CanAttack(Attacke本, Ta本成et))
    {
        本et使本n;
    }

    // 添加到戰鬥對
    Co設置batPai本s.Add(Attacke本, Ta本成et);
    
    軍RTSCo設置batStats Attacke本Stats = GetUnitCo設置batStats(Attacke本);
    
    的otifyCo設置batSta本ted(Attacke本, Ta本成et, Attacke本Stats.Co設置batType, Attacke本Stats.Da設置a成eType);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本ted co設置bat between 使nits"));
}

正oid UMin成RTSCo設置batSyste設置::StopCo設置bat(AMin成GoRTSUnit* Unit)
{
    if (!Unit)
    {
        本et使本n;
    }

    // 從戰鬥對中移除
    Co設置batPai本s.Re設置o正e(Unit);
    
    // 檢查是否為目標
    TA本本ay<AMin成GoRTSUnit*> Attacke本sToRe設置o正e;
    fo本 (a使to& Co設置batPai本 : Co設置batPai本s)
    {
        if (Co設置batPai本.Val使e == Unit)
        {
            Attacke本sToRe設置o正e.Add(Co設置batPai本.Key);
        }
    }
    
    fo本 (AMin成GoRTSUnit* Attacke本 : Attacke本sToRe設置o正e)
    {
        Co設置batPai本s.Re設置o正e(Attacke本);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stopped co設置bat fo本 使nit"));
}

bool UMin成RTSCo設置batSyste設置::IsInCo設置bat(AMin成GoRTSUnit* Unit) const
{
    本et使本n Co設置batPai本s.Contains(Unit);
}

float UMin成RTSCo設置batSyste設置::Calc使lateDa設置a成e(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et)
{
    if (!Attacke本  !Ta本成et)
    {
        本et使本n 0.0f;
    }

    軍RTSCo設置batStats Attacke本Stats = GetUnitCo設置batStats(Attacke本);
    軍RTSCo設置batStats Ta本成etStats = GetUnitCo設置batStats(Ta本成et);
    
    // 基礎傷害計算
    float BaseDa設置a成e = Attacke本Stats.AttackDa設置a成e * BaseDa設置a成eM使ltiplie本;
    
    // 應用姿態加成
    ERTSCo設置batStance Attacke本Stance = GetCo設置batStance(Attacke本);
    float StanceM使ltiplie本 = GetStanceDa設置a成eM使ltiplie本(Attacke本Stance);
    BaseDa設置a成e *= StanceM使ltiplie本;
    
    // 應用傷害類型加成
    float TypeM使ltiplie本 = GetDa設置a成eTypeM使ltiplie本(Attacke本Stats.Da設置a成eType, Ta本成etStats.Da設置a成eType);
    BaseDa設置a成e *= TypeM使ltiplie本;
    
    // 應用防禦減免
    float 軍inalDa設置a成e = ApplyDa設置a成eRed使ction(BaseDa設置a成e, Attacke本Stats.Da設置a成eType, Ta本成etStats.DefenseRatin成);
    
    本et使本n 軍Math::Max(0.0f, 軍inalDa設置a成e);
}

float UMin成RTSCo設置batSyste設置::ApplyDa設置a成eRed使ction(float BaseDa設置a成e, ERTSDa設置a成eType Da設置a成eType, float DefenseRatin成)
{
    float DefenseM使ltiplie本 = BaseDefenseM使ltiplie本;
    float DefenseVal使e = DefenseRatin成 * DefenseM使ltiplie本;
    
    // 根據傷害類型調整防禦效果
    switch (Da設置a成eType)
    {
        case ERTSDa設置a成eType::Physical:
            // 物理傷害受防禦影響最大
            DefenseVal使e *= 1.0f;
            b本eak;
        case ERTSDa設置a成eType::軍i本e:
        case ERTSDa設置a成eType::Explosi正e:
            // 火焰和爆炸傷害防禦效果較低
            DefenseVal使e *= 0.5f;
            b本eak;
        case ERTSDa設置a成eType::Pie本cin成:
            // 穿刺傷害部分忽略防禦
            DefenseVal使e *= 0.7f;
            b本eak;
        case ERTSDa設置a成eType::Ma成ic:
        case ERTSDa設置a成eType::Poison:
            // 魔法和毒素傷害防禦效果最低
            DefenseVal使e *= 0.3f;
            b本eak;
    }
    
    float Da設置a成eRed使ction = DefenseVal使e / (DefenseVal使e + 100.0f);
    本et使本n BaseDa設置a成e * (1.0f - Da設置a成eRed使ction);
}

bool UMin成RTSCo設置batSyste設置::Check輸入it(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et)
{
    if (!Attacke本  !Ta本成et)
    {
        本et使本n false;
    }

    軍RTSCo設置batStats Attacke本Stats = GetUnitCo設置batStats(Attacke本);
    
    // 基礎命中率
    float 輸入itChance = Attacke本Stats.Acc使本acy;
    
    // 距離影響命中率
    float Distance = 軍Vecto本::Dist(Attacke本->GetActo本Location(), Ta本成et->GetActo本Location());
    float DistancePenalty = Distance / Attacke本Stats.AttackRan成e;
    輸入itChance -= DistancePenalty * 0.2f;
    
    // 隨機判定
    float Rando設置Roll = 軍Math::軍RandRan成e(0.0f, 1.0f);
    本et使本n Rando設置Roll <= 輸入itChance;
}

bool UMin成RTSCo設置batSyste設置::CheckC本itical(AMin成GoRTSUnit* Attacke本)
{
    if (!Attacke本  !bEnableC本itical輸入its)
    {
        本et使本n false;
    }

    軍RTSCo設置batStats Attacke本Stats = GetUnitCo設置batStats(Attacke本);
    float Rando設置Roll = 軍Math::軍RandRan成e(0.0f, 1.0f);
    
    本et使本n Rando設置Roll <= Attacke本Stats.C本iticalChance;
}

bool UMin成RTSCo設置batSyste設置::CheckDod成e(AMin成GoRTSUnit* Ta本成et)
{
    if (!Ta本成et  !bEnableDod成in成)
    {
        本et使本n false;
    }

    軍RTSCo設置batStats Ta本成etStats = GetUnitCo設置batStats(Ta本成et);
    float Rando設置Roll = 軍Math::軍RandRan成e(0.0f, 1.0f);
    
    本et使本n Rando設置Roll <= Ta本成etStats.Dod成eChance;
}

bool UMin成RTSCo設置batSyste設置::CheckBlock(AMin成GoRTSUnit* Ta本成et)
{
    if (!Ta本成et  !bEnableBlockin成)
    {
        本et使本n false;
    }

    軍RTSCo設置batStats Ta本成etStats = GetUnitCo設置batStats(Ta本成et);
    float Rando設置Roll = 軍Math::軍RandRan成e(0.0f, 1.0f);
    
    本et使本n Rando設置Roll <= Ta本成etStats.BlockChance;
}

正oid UMin成RTSCo設置batSyste設置::SetCo設置batStance(AMin成GoRTSUnit* Unit, ERTSCo設置batStance Stance)
{
    if (!Unit)
    {
        本et使本n;
    }

    UnitCo設置batStances.Add(Unit, Stance);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Set co設置bat stance: %d"), (int32)Stance);
}

ERTSCo設置batStance UMin成RTSCo設置batSyste設置::GetCo設置batStance(AMin成GoRTSUnit* Unit) const
{
    if (UnitCo設置batStances.Contains(Unit))
    {
        本et使本n UnitCo設置batStances[Unit];
    }
    
    本et使本n ERTSCo設置batStance::的e使t本al;
}

float UMin成RTSCo設置batSyste設置::GetStanceDa設置a成eM使ltiplie本(ERTSCo設置batStance Stance) const
{
    switch (Stance)
    {
        case ERTSCo設置batStance::A成成本essi正e:
            本et使本n 1.2f; // 攻擊姿態增加20%傷害
        case ERTSCo設置batStance::Defensi正e:
            本et使本n 0.8f; // 防禦姿態減少20%傷害
        case ERTSCo設置batStance::的e使t本al:
            本et使本n 1.0f; // 中立姿態無加成
        case ERTSCo設置batStance::Passi正e:
            本et使本n 0.6f; // 被動姿態減少40%傷害
        defa使lt:
            本et使本n 1.0f;
    }
}

float UMin成RTSCo設置batSyste設置::GetStanceDefenseM使ltiplie本(ERTSCo設置batStance Stance) const
{
    switch (Stance)
    {
        case ERTSCo設置batStance::A成成本essi正e:
            本et使本n 0.8f; // 攻擊姿態減少20%防禦
        case ERTSCo設置batStance::Defensi正e:
            本et使本n 1.3f; // 防禦姿態增加30%防禦
        case ERTSCo設置batStance::的e使t本al:
            本et使本n 1.0f; // 中立姿態無加成
        case ERTSCo設置batStance::Passi正e:
            本et使本n 1.1f; // 被動姿態增加10%防禦
        defa使lt:
            本et使本n 1.0f;
    }
}

TA本本ay<AMin成GoRTSUnit*> UMin成RTSCo設置batSyste設置::GetUnitsInA本ea(const 軍Vecto本& Cente本, float Radi使s, AMin成GoRTSUnit* Excl使dedUnit)
{
    TA本本ay<AMin成GoRTSUnit*> UnitsInA本ea;
    
    if (!UnitMana成e本)
    {
        本et使本n UnitsInA本ea;
    }

    TA本本ay<AMin成GoRTSUnit*> AllUnits = UnitMana成e本->GetAllUnits();
    
    fo本 (AMin成GoRTSUnit* Unit : AllUnits)
    {
        if (Unit && Unit != Excl使dedUnit)
        {
            float Distance = 軍Vecto本::Dist(Cente本, Unit->GetActo本Location());
            if (Distance <= Radi使s)
            {
                UnitsInA本ea.Add(Unit);
            }
        }
    }
    
    本et使本n UnitsInA本ea;
}

正oid UMin成RTSCo設置batSyste設置::Exec使teA本eaAttack(AMin成GoRTSUnit* Attacke本, const 軍Vecto本& Cente本, float Radi使s)
{
    if (!Attacke本)
    {
        本et使本n;
    }

    TA本本ay<AMin成GoRTSUnit*> UnitsInA本ea = GetUnitsInA本ea(Cente本, Radi使s, Attacke本);
    
    fo本 (AMin成GoRTSUnit* Ta本成et : UnitsInA本ea)
    {
        Exec使teCo設置bat(Attacke本, Ta本成et);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Exec使ted a本ea attack affectin成 %d 使nits"), UnitsInA本ea.的使設置());
}

正oid UMin成RTSCo設置batSyste設置::Exec使teLineAttack(AMin成GoRTSUnit* Attacke本, const 軍Vecto本& Sta本t, const 軍Vecto本& End, float 基本idth)
{
    if (!Attacke本)
    {
        本et使本n;
    }

    // 簡化的線性攻擊實現
    軍Vecto本 Di本ection = (End - Sta本t).GetSafe的o本設置al();
    float Distance = 軍Vecto本::Dist(Sta本t, End);
    
    TA本本ay<AMin成GoRTSUnit*> AllUnits = UnitMana成e本->GetAllUnits();
    
    fo本 (AMin成GoRTSUnit* Unit : AllUnits)
    {
        if (Unit && Unit != Attacke本)
        {
            軍Vecto本 UnitLocation = Unit->GetActo本Location();
            軍Vecto本 ToUnit = UnitLocation - Sta本t;
            
            // 檢查是否在攻擊線上
            float P本o大ection = 軍Vecto本::DotP本od使ct(ToUnit, Di本ection);
            if (P本o大ection >= 0.0f && P本o大ection <= Distance)
            {
                軍Vecto本 ClosestPoint = Sta本t + Di本ection * P本o大ection;
                float Distance軍本o設置Line = 軍Vecto本::Dist(ClosestPoint, UnitLocation);
                
                if (Distance軍本o設置Line <= 基本idth)
                {
                    Exec使teCo設置bat(Attacke本, Unit);
                }
            }
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Exec使ted line attack"));
}

正oid UMin成RTSCo設置batSyste設置::AddAbility(AMin成GoRTSUnit* Unit, const 軍RTSAbilityData& Ability)
{
    if (!Unit)
    {
        本et使本n;
    }

    if (!UnitAbilities.Contains(Unit))
    {
        UnitAbilities.Add(Unit, TA本本ay<軍RTSAbilityData>());
    }
    
    UnitAbilities[Unit].Add(Ability);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Added ability: %s"), *Ability.Ability的a設置e);
}

正oid UMin成RTSCo設置batSyste設置::Re設置o正eAbility(AMin成GoRTSUnit* Unit, const 軍St本in成& Ability的a設置e)
{
    if (!Unit  !UnitAbilities.Contains(Unit))
    {
        本et使本n;
    }

    TA本本ay<軍RTSAbilityData>& Abilities = UnitAbilities[Unit];
    
    fo本 (int32 i = 0; i < Abilities.的使設置(); ++i)
    {
        if (Abilities[i].Ability的a設置e == Ability的a設置e)
        {
            Abilities.Re設置o正eAt(i);
            b本eak;
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正ed ability: %s"), *Ability的a設置e);
}

bool UMin成RTSCo設置batSyste設置::UseAbility(AMin成GoRTSUnit* Unit, const 軍St本in成& Ability的a設置e, AMin成GoRTSUnit* Ta本成et)
{
    if (!Unit  !UnitAbilities.Contains(Unit))
    {
        本et使本n false;
    }

    TA本本ay<軍RTSAbilityData>& Abilities = UnitAbilities[Unit];
    
    fo本 (軍RTSAbilityData& Ability : Abilities)
    {
        if (Ability.Ability的a設置e == Ability的a設置e && Ability.bIsA正ailable && Ability.C使本本entCooldown <= 0.0f)
        {
            // 使用技能
            Ability.C使本本entCooldown = Ability.Cooldown;
            Ability.bIsActi正e = t本使e;
            
            // 這裡應該實現技能效果
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Used ability: %s"), *Ability的a設置e);
            
            的otifyAbilityUsed(Unit, Ability的a設置e);
            本et使本n t本使e;
        }
    }
    
    本et使本n false;
}

TA本本ay<軍RTSAbilityData> UMin成RTSCo設置batSyste設置::GetUnitAbilities(AMin成GoRTSUnit* Unit) const
{
    if (UnitAbilities.Contains(Unit))
    {
        本et使本n UnitAbilities[Unit];
    }
    
    本et使本n TA本本ay<軍RTSAbilityData>();
}

bool UMin成RTSCo設置batSyste設置::IsAbilityA正ailable(AMin成GoRTSUnit* Unit, const 軍St本in成& Ability的a設置e) const
{
    if (!Unit  !UnitAbilities.Contains(Unit))
    {
        本et使本n false;
    }

    const TA本本ay<軍RTSAbilityData>& Abilities = UnitAbilities[Unit];
    
    fo本 (const 軍RTSAbilityData& Ability : Abilities)
    {
        if (Ability.Ability的a設置e == Ability的a設置e && Ability.bIsA正ailable && Ability.C使本本entCooldown <= 0.0f)
        {
            本et使本n t本使e;
        }
    }
    
    本et使本n false;
}

正oid UMin成RTSCo設置batSyste設置::ApplyStat使sEffect(AMin成GoRTSUnit* Unit, const 軍St本in成& Effect的a設置e, float D使本ation, float Powe本)
{
    if (!Unit)
    {
        本et使本n;
    }

    if (!Stat使sEffects.Contains(Unit))
    {
        Stat使sEffects.Add(Unit, TMap<軍St本in成, float>());
    }
    
    Stat使sEffects[Unit].Add(Effect的a設置e, D使本ation);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied stat使s effect: %s"), *Effect的a設置e);
}

正oid UMin成RTSCo設置batSyste設置::Re設置o正eStat使sEffect(AMin成GoRTSUnit* Unit, const 軍St本in成& Effect的a設置e)
{
    if (!Unit  !Stat使sEffects.Contains(Unit))
    {
        本et使本n;
    }

    Stat使sEffects[Unit].Re設置o正e(Effect的a設置e);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正ed stat使s effect: %s"), *Effect的a設置e);
}

bool UMin成RTSCo設置batSyste設置::輸入asStat使sEffect(AMin成GoRTSUnit* Unit, const 軍St本in成& Effect的a設置e) const
{
    if (!Unit  !Stat使sEffects.Contains(Unit))
    {
        本et使本n false;
    }

    本et使本n Stat使sEffects[Unit].Contains(Effect的a設置e);
}

正oid UMin成RTSCo設置batSyste設置::UpdateStat使sEffects(float DeltaTi設置e)
{
    fo本 (a使to& Stat使sPai本 : Stat使sEffects)
    {
        AMin成GoRTSUnit* Unit = Stat使sPai本.Key;
        TMap<軍St本in成, float>& Effects = Stat使sPai本.Val使e;
        
        TA本本ay<軍St本in成> EffectsToRe設置o正e;
        
        fo本 (a使to& EffectPai本 : Effects)
        {
            EffectPai本.Val使e -= DeltaTi設置e;
            if (EffectPai本.Val使e <= 0.0f)
            {
                EffectsToRe設置o正e.Add(EffectPai本.Key);
            }
        }
        
        fo本 (const 軍St本in成& Effect的a設置e : EffectsToRe設置o正e)
        {
            Re設置o正eStat使sEffect(Unit, Effect的a設置e);
        }
    }
}

正oid UMin成RTSCo設置batSyste設置::UpdateCo設置bat(float DeltaTi設置e)
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    UpdateAbilityCooldowns(DeltaTi設置e);
    UpdateStat使sEffects(DeltaTi設置e);
    P本ocessCo設置batQ使e使e();
    Clean使pDeadUnits();
}

正oid UMin成RTSCo設置batSyste設置::P本ocessCo設置batQ使e使e()
{
    // 處理所有戰鬥對
    TA本本ay<AMin成GoRTSUnit*> P本ocessedAttacke本s;
    
    fo本 (a使to& Co設置batPai本 : Co設置batPai本s)
    {
        AMin成GoRTSUnit* Attacke本 = Co設置batPai本.Key;
        AMin成GoRTSUnit* Ta本成et = Co設置batPai本.Val使e;
        
        if (Attacke本 && Ta本成et && !P本ocessedAttacke本s.Contains(Attacke本))
        {
            Exec使teCo設置bat(Attacke本, Ta本成et);
            P本ocessedAttacke本s.Add(Attacke本);
        }
    }
}

int32 UMin成RTSCo設置batSyste設置::GetActi正eCo設置batCo使nt() const
{
    本et使本n Co設置batPai本s.的使設置();
}

int32 UMin成RTSCo設置batSyste設置::GetTotalDa設置a成eDealt() const
{
    本et使本n TotalDa設置a成eDealt;
}

int32 UMin成RTSCo設置batSyste設置::GetUnitsKilled() const
{
    本et使本n UnitsKilled;
}

正oid UMin成RTSCo設置batSyste設置::ResetCo設置batStats()
{
    TotalDa設置a成eDealt = 0;
    UnitsKilled = 0;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reset co設置bat statistics"));
}

正oid UMin成RTSCo設置batSyste設置::P本ocessCo設置batPai本(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et, float DeltaTi設置e)
{
    // 這裡可以實現持續戰鬥邏輯
    // 例如：攻擊速度計算、持續傷害等
}

正oid UMin成RTSCo設置batSyste設置::ApplyCo設置batRes使lt(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et, const 軍RTSCo設置batRes使lt& Res使lt)
{
    if (!Attacke本  !Ta本成et  !UnitMana成e本)
    {
        本et使本n;
    }

    // 更新傷害統計
    TotalDa設置a成eDealt += 軍Math::Ro使ndToInt(Res使lt.Da設置a成eDealt);
    
    // 應用傷害到目標
    UnitMana成e本->UpdateUnit輸入ealth(Ta本成et, -Res使lt.Da設置a成eDealt);
    
    // 檢查是否擊殺
    if (!UnitMana成e本->IsUnitAli正e(Ta本成et))
    {
        UnitsKilled++;
        的otifyUnitKilled(Ta本成et, Attacke本);
        StopCo設置bat(Ta本成et);
        StopCo設置bat(Attacke本);
    }
}

軍St本in成 UMin成RTSCo設置batSyste設置::Gene本ateCo設置batRes使ltMessa成e(const 軍RTSCo設置batRes使lt& Res使lt) const
{
    軍St本in成 Messa成e;
    
    if (Res使lt.bDod成ed)
    {
        Messa成e = TEXT("Dod成ed!");
    }
    else if (Res使lt.bBlocked)
    {
        Messa成e = 軍St本in成::P本intf(TEXT("Blocked! %.1f da設置a成e"), Res使lt.Da設置a成eDealt);
    }
    else if (Res使lt.bC本itical)
    {
        Messa成e = 軍St本in成::P本intf(TEXT("C本itical 輸入it! %.1f da設置a成e"), Res使lt.Da設置a成eDealt);
    }
    else
    {
        Messa成e = 軍St本in成::P本intf(TEXT("輸入it! %.1f da設置a成e"), Res使lt.Da設置a成eDealt);
    }
    
    本et使本n Messa成e;
}

float UMin成RTSCo設置batSyste設置::GetDa設置a成eTypeM使ltiplie本(ERTSDa設置a成eType Da設置a成eType, ERTSDa設置a成eType DefenseType) const
{
    // 傷害類型相互作用矩陣
    switch (Da設置a成eType)
    {
        case ERTSDa設置a成eType::Physical:
            本et使本n 1.0f;
        case ERTSDa設置a成eType::軍i本e:
            本et使本n 1.2f; // 火焰對大多數目標有效
        case ERTSDa設置a成eType::Explosi正e:
            本et使本n 1.5f; // 爆炸傷害對群體有效
        case ERTSDa設置a成eType::Pie本cin成:
            本et使本n 1.1f; // 穿刺傷害對輕甲有效
        case ERTSDa設置a成eType::Ma成ic:
            本et使本n 1.3f; // 魔法傷害對無防禦目標有效
        case ERTSDa設置a成eType::Poison:
            本et使本n 0.8f; // 毒素傷害持續但初始傷害較低
        defa使lt:
            本et使本n 1.0f;
    }
}

正oid UMin成RTSCo設置batSyste設置::Clean使pDeadUnits()
{
    if (!UnitMana成e本)
    {
        本et使本n;
    }

    TA本本ay<AMin成GoRTSUnit*> DeadUnits;
    
    fo本 (const a使to& Co設置batPai本 : Co設置batPai本s)
    {
        AMin成GoRTSUnit* Attacke本 = Co設置batPai本.Key;
        AMin成GoRTSUnit* Ta本成et = Co設置batPai本.Val使e;
        
        if (!UnitMana成e本->IsUnitAli正e(Attacke本))
        {
            DeadUnits.Add(Attacke本);
        }
        
        if (!UnitMana成e本->IsUnitAli正e(Ta本成et))
        {
            DeadUnits.Add(Ta本成et);
        }
    }
    
    fo本 (AMin成GoRTSUnit* DeadUnit : DeadUnits)
    {
        StopCo設置bat(DeadUnit);
    }
}

正oid UMin成RTSCo設置batSyste設置::UpdateAbilityCooldowns(float DeltaTi設置e)
{
    fo本 (a使to& AbilityPai本 : UnitAbilities)
    {
        TA本本ay<軍RTSAbilityData>& Abilities = AbilityPai本.Val使e;
        
        fo本 (軍RTSAbilityData& Ability : Abilities)
        {
            if (Ability.C使本本entCooldown > 0.0f)
            {
                Ability.C使本本entCooldown -= DeltaTi設置e;
                if (Ability.C使本本entCooldown <= 0.0f)
                {
                    Ability.bIsA正ailable = t本使e;
                    Ability.bIsActi正e = false;
                }
            }
        }
    }
}

正oid UMin成RTSCo設置batSyste設置::的otifyCo設置batSta本ted(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et, ERTSCo設置batType Co設置batType, ERTSDa設置a成eType Da設置a成eType)
{
    OnCo設置batSta本ted.B本oadcast(Attacke本, Ta本成et, Co設置batType, Da設置a成eType);
}

正oid UMin成RTSCo設置batSyste設置::的otifyCo設置bat輸入it(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et, const 軍RTSCo設置batRes使lt& Res使lt)
{
    OnCo設置bat輸入it.B本oadcast(Attacke本, Ta本成et, Res使lt);
}

正oid UMin成RTSCo設置batSyste設置::的otifyCo設置batMissed(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et)
{
    OnCo設置batMissed.B本oadcast(Attacke本, Ta本成et);
}

正oid UMin成RTSCo設置batSyste設置::的otifyUnitKilled(AMin成GoRTSUnit* Victi設置, AMin成GoRTSUnit* Kille本)
{
    OnUnitKilled.B本oadcast(Victi設置, Kille本);
}

正oid UMin成RTSCo設置batSyste設置::的otifyAbilityUsed(AMin成GoRTSUnit* Unit, const 軍St本in成& Ability的a設置e)
{
    OnAbilityUsed.B本oadcast(Unit, Ability的a設置e);
}

// 戰鬥平衡調整功能
正oid UMin成RTSCo設置batSyste設置::BalanceCo設置batStats()
{
    // 根據遊戲數據平衡戰鬥統計
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Balancin成 co設置bat stats..."));

    // 調整基礎傷害和防禦
    BaseDa設置a成eM使ltiplie本 *= Diffic使ltyM使ltiplie本;
    BaseDefenseM使ltiplie本 *= Diffic使ltyM使ltiplie本;

    // 更新所有單位的戰鬥統計
    fo本 (a使to& StatPai本 : UnitCo設置batStats)
    {
        AMin成GoRTSUnit* Unit = StatPai本.Key;
        軍RTSCo設置batStats& Stats = StatPai本.Val使e;

        // 應用單位類型修正
        if (UnitTypeDa設置a成eModifie本s.Contains(Unit->GetUnitType()))
        {
            Stats.AttackDa設置a成e *= UnitTypeDa設置a成eModifie本s[Unit->GetUnitType()];
        }

        if (UnitTypeDefenseModifie本s.Contains(Unit->GetUnitType()))
        {
            Stats.DefenseRatin成 *= UnitTypeDefenseModifie本s[Unit->GetUnitType()];
        }

        // 應用傷害類型效果
        if (Da設置a成eTypeEffecti正eness.Contains(Stats.Da設置a成eType))
        {
            Stats.AttackDa設置a成e *= Da設置a成eTypeEffecti正eness[Stats.Da設置a成eType];
        }
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Co設置bat stats balanced s使ccessf使lly"));
}

正oid UMin成RTSCo設置batSyste設置::Ad大使stUnitTypeAd正anta成es()
{
    // 調整單位類型之間的優勢關係
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ad大使stin成 使nit type ad正anta成es..."));

    // 步兵 正s 騎兵：步兵有優勢
    UnitTypeDa設置a成eModifie本s[ERTSUnitType::Infant本y] = 1.2f;
    UnitTypeDefenseModifie本s[ERTSUnitType::Ca正al本y] = 0.8f;

    // 騎兵 正s 火炮：騎兵有優勢
    UnitTypeDa設置a成eModifie本s[ERTSUnitType::Ca正al本y] = 1.3f;
    UnitTypeDefenseModifie本s[ERTSUnitType::A本tille本y] = 0.7f;

    // 火炮 正s 建築：火炮有優勢
    UnitTypeDa設置a成eModifie本s[ERTSUnitType::A本tille本y] = 1.5f;

    // 斥候 正s 偵察：斥候有優勢
    UnitTypeDa設置a成eModifie本s[ERTSUnitType::Sco使t] = 1.1f;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit type ad正anta成es ad大使sted"));
}

正oid UMin成RTSCo設置batSyste設置::SetDiffic使ltyM使ltiplie本(float M使ltiplie本)
{
    Diffic使ltyM使ltiplie本 = 軍Math::Cla設置p(M使ltiplie本, 0.1f, 3.0f);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Diffic使lty 設置使ltiplie本 set to: %f"), Diffic使ltyM使ltiplie本);
}

正oid UMin成RTSCo設置batSyste設置::Opti設置izeCo設置batPe本fo本設置ance()
{
    // 優化戰鬥系統性能
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Opti設置izin成 co設置bat pe本fo本設置ance..."));

    // 清理無效的戰鬥對
    TA本本ay<AMin成GoRTSUnit*> In正alidPai本s;
    fo本 (a使to& Co設置batPai本 : Co設置batPai本s)
    {
        if (!Co設置batPai本.Key  !Co設置batPai本.Val使e  
            !UnitMana成e本->IsUnitAli正e(Co設置batPai本.Key)  
            !UnitMana成e本->IsUnitAli正e(Co設置batPai本.Val使e))
        {
            In正alidPai本s.Add(Co設置batPai本.Key);
        }
    }

    fo本 (AMin成GoRTSUnit* In正alidUnit : In正alidPai本s)
    {
        Co設置batPai本s.Re設置o正e(In正alidUnit);
    }

    // 清理無效的單位統計
    TA本本ay<AMin成GoRTSUnit*> In正alidStats;
    fo本 (a使to& StatPai本 : UnitCo設置batStats)
    {
        if (!StatPai本.Key  !UnitMana成e本->IsUnitAli正e(StatPai本.Key))
        {
            In正alidStats.Add(StatPai本.Key);
        }
    }

    fo本 (AMin成GoRTSUnit* In正alidUnit : In正alidStats)
    {
        UnitCo設置batStats.Re設置o正e(In正alidUnit);
        UnitCo設置batStances.Re設置o正e(In正alidUnit);
        UnitAbilities.Re設置o正e(In正alidUnit);
        Stat使sEffects.Re設置o正e(In正alidUnit);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Co設置bat pe本fo本設置ance opti設置ized"));
}
