#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成RTSCo設置batSyste設置.成ene本ated.h"

// 目標數量
class AMin成GoRTSUnit;
class UMin成RTSUnitMana成e本;

UE的UM(Bl使ep本intType)
en使設置 class ERTSCo設置batType: 使int8 {
    Melee,          // ���
    Ran成ed,         // 
    A本tille本y,      // 動��
    Sie成e,          // 
    的a正al           // ����
};

UE的UM(Bl使ep本intType)
en使設置 class ERTSDa設置a成eType: 使int8 {
    Physical,       // 動池�軍事
    軍i本e,           // 動�K動�軍事
    Explosi正e,      // 動��動�軍事
    Pie本cin成,       // ���動�軍事
    Ma成ic,          // �]池�軍事
    Poison          // �本池�軍事
};

UE的UM(Bl使ep本intType)
en使設置 class ERTSCo設置batStance: 使int8 {
    A成成本essi正e,     // ��動
    Defensi正e,      // 動�設置��動
    的e使t本al,        // ��動��動
    Passi正e         // �Q動��動
};

USTRUCT(Bl使ep本intType)
st本使ct 軍RTSCo設置batStats
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Co設置bat Stats")
    float AttackDa設置a成e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Co設置bat Stats")
    float DefenseRatin成;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Co設置bat Stats")
    float AttackSpeed;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Co設置bat Stats")
    float AttackRan成e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Co設置bat Stats")
    float Acc使本acy;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Co設置bat Stats")
    float C本iticalChance;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Co設置bat Stats")
    float C本iticalM使ltiplie本;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Co設置bat Stats")
    float Dod成eChance;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Co設置bat Stats")
    float BlockChance;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Co設置bat Stats")
    ERTSCo設置batType Co設置batType;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Co設置bat Stats")
    ERTSDa設置a成eType Da設置a成eType;

    軍RTSCo設置batStats()
    {
        AttackDa設置a成e = 20.0f;
        DefenseRatin成 = 10.0f;
        AttackSpeed = 1.0f;
        AttackRan成e = 100.0f;
        Acc使本acy = 0.8f;
        C本iticalChance = 0.1f;
        C本iticalM使ltiplie本 = 2.0f;
        Dod成eChance = 0.1f;
        BlockChance = 0.15f;
        Co設置batType = ERTSCo設置batType::Melee;
        Da設置a成eType = ERTSDa設置a成eType::Physical;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍RTSCo設置batRes使lt
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Co設置bat Res使lt")
    bool b輸入it;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Co設置bat Res使lt")
    bool bC本itical;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Co設置bat Res使lt")
    bool bDod成ed;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Co設置bat Res使lt")
    bool bBlocked;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Co設置bat Res使lt")
    float Da設置a成eDealt;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Co設置bat Res使lt")
    float Da設置a成eBlocked;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Co設置bat Res使lt")
    ERTSDa設置a成eType Da設置a成eType;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Co設置bat Res使lt")
    軍St本in成 Res使ltMessa成e;

    軍RTSCo設置batRes使lt()
    {
        b輸入it = false;
        bC本itical = false;
        bDod成ed = false;
        bBlocked = false;
        Da設置a成eDealt = 0.0f;
        Da設置a成eBlocked = 0.0f;
        Da設置a成eType = ERTSDa設置a成eType::Physical;
        Res使ltMessa成e = TEXT(""};
};

USTRUCT(Bl使ep本intType)
st本使ct 軍RTSAbilityData
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Ability Data")
    軍St本in成 Ability的a設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Ability Data")
    軍St本in成 Desc本iption;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Ability Data")
    float Cooldown;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Ability Data")
    float C使本本entCooldown;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Ability Data")
    float ManaCost;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Ability Data")
    float Ran成e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Ability Data")
    bool bIsA正ailable;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Ability Data")
    bool bIsActi正e;

    軍RTSAbilityData()
    {
        Ability的a設置e = TEXT(""};
        Desc本iption = TEXT(""};
        Cooldown = 5.0f;
        C使本本entCooldown = 0.0f;
        ManaCost = 10.0f;
        Ran成e = 100.0f;
        bIsA正ailable = t本使e;
        bIsActi正e = false;
};







/**
 * RTS動���t��
 * ��池動�԰���動池��動 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GCORE下API UMin成RTSCo設置batSyste設置 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成RTSCo設置batSyste設置(};

    
    正oid InitializeCo設置batSyste設置(UMin成RTSUnitMana成e本* InUnitMana成e本};

    // 動����動
    
    正oid SetUnitCo設置batStats(AMin成GoRTSUnit* Unit, const 軍RTSCo設置batStats& Co設置batStats) {};

    
    軍RTSCo設置batStats GetUnitCo設置batStats(AMin成GoRTSUnit* Unit) const;

    
    正oid UpdateCo設置batStats(AMin成GoRTSUnit* Unit, float Da設置a成eM使ltiplie本 = 1.0f, float DefenseM使ltiplie本 = 1.0f};

    // 動��
    
    軍RTSCo設置batRes使lt Exec使teCo設置bat(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et};

    bool CanAttack(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et};

    正oid Sta本tCo設置bat(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et, ERTSCo設置batType Co設置batType};

    正oid StopCo設置bat(AMin成GoRTSUnit* Unit};

    bool IsInCo設置bat(AMin成GoRTSUnit* Unit) const;

    // 動�軍事�p動
    
    float Calc使lateDa設置a成e(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Co設置bat Syste設置")
    float ApplyDa設置a成eRed使ction(float BaseDa設置a成e, ERTSDa設置a成eType Da設置a成eType, float DefenseRatin成};

    
    bool Check輸入it(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et};

    
    bool CheckC本itical(AMin成GoRTSUnit* Attacke本};

    
    bool CheckDod成e(AMin成GoRTSUnit* Ta本成et};

    
    bool CheckBlock(AMin成GoRTSUnit* Ta本成et};

    // 動����動
    
    正oid SetCo設置batStance(AMin成GoRTSUnit* Unit, ERTSCo設置batStance Stance};

    
    ERTSCo設置batStance GetCo設置batStance(AMin成GoRTSUnit* Unit) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Co設置bat Syste設置")
    float GetStanceDa設置a成eM使ltiplie本(ERTSCo設置batStance Stance) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Co設置bat Syste設置")
    float GetStanceDefenseM使ltiplie本(ERTSCo設置batStance Stance) const;

    // �d池動
    
    TA本本ay<AMin成GoRTSUnit*> GetUnitsInA本ea(const 軍Vecto本& Cente本, float Radi使s, AMin成GoRTSUnit* Excl使dedUnit = n使llpt本};

    
    正oid Exec使teA本eaAttack(AMin成GoRTSUnit* Attacke本, const 軍Vecto本& Cente本, float Radi使s};

    
    正oid Exec使teLineAttack(AMin成GoRTSUnit* Attacke本, const 軍Vecto本& Sta本t, const 軍Vecto本& End, float 基本idth};

    
    正oid AddAbility(AMin成GoRTSUnit* Unit, const 軍RTSAbilityData& Ability) {};

    
    正oid Re設置o正eAbility(AMin成GoRTSUnit* Unit, const 軍St本in成& Ability的a設置e) {};

    
    bool UseAbility(AMin成GoRTSUnit* Unit, const 軍St本in成& Ability的a設置e, AMin成GoRTSUnit* Ta本成et = n使llpt本};

    
    TA本本ay<軍RTSAbilityData> GetUnitAbilities(AMin成GoRTSUnit* Unit) const;

    
    bool IsAbilityA正ailable(AMin成GoRTSUnit* Unit, const 軍St本in成& Ability的a設置e) const;

    
    正oid ApplyStat使sEffect(AMin成GoRTSUnit* Unit, const 軍St本in成& Effect的a設置e, float D使本ation, float Powe本};

    
    正oid Re設置o正eStat使sEffect(AMin成GoRTSUnit* Unit, const 軍St本in成& Effect的a設置e) {};

    
    bool 輸入asStat使sEffect(AMin成GoRTSUnit* Unit, const 軍St本in成& Effect的a設置e) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Co設置bat Syste設置")
    正oid UpdateStat使sEffects(float DeltaTi設置e};

    // 動�����Žվ�
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Co設置bat Syste設置")
    正oid BalanceCo設置batStats(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Co設置bat Syste設置")
    正oid Ad大使stUnitTypeAd正anta成es(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Co設置bat Syste設置")
    正oid SetDiffic使ltyM使ltiplie本(float M使ltiplie本};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Co設置bat Syste設置")
    正oid Opti設置izeCo設置batPe本fo本設置ance(};

    // 動��動�s
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Co設置bat Syste設置")
    正oid UpdateCo設置bat(float DeltaTi設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Co設置bat Syste設置")
    正oid P本ocessCo設置batQ使e使e(};

    // 動����動
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "RTS Co設置bat Syste設置")
    int32 GetActi正eCo設置batCo使nt() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "RTS Co設置bat Syste設置")
    int32 GetTotalDa設置a成eDealt() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "RTS Co設置bat Syste設置")
    int32 GetUnitsKilled() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Co設置bat Syste設置")
    正oid ResetCo設置batStats(};

    // �基礎�
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "RTS Co設置bat E正ents")
    軍OnCo設置batSta本ted OnCo設置batSta本ted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "RTS Co設置bat E正ents")
    軍OnCo設置bat輸入it OnCo設置bat輸入it;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "RTS Co設置bat E正ents")
    軍OnCo設置batMissed OnCo設置batMissed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "RTS Co設置bat E正ents")
    軍OnUnitKilled OnUnitKilled;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "RTS Co設置bat E正ents")
    軍OnAbilityUsed OnAbilityUsed;

p本otected:
    // �t�Τޥ�
    UPROPERTY()
    TOb大ectPt本<UMin成RTSUnitMana成e本> UnitMana成e本;

    // 動��
    
    TMap<AMin成GoRTSUnit*, 軍RTSCo設置batStats> UnitCo設置batStats;

    
    TMap<AMin成GoRTSUnit*, ERTSCo設置batStance> UnitCo設置batStances;

    
    TMap<AMin成GoRTSUnit*, TA本本ay<軍RTSAbilityData>> UnitAbilities;

    
    TMap<AMin成GoRTSUnit*, TMap<軍St本in成, float>> Stat使sEffects;

    
    TMap<AMin成GoRTSUnit*, AMin成GoRTSUnit*> Co設置batPai本s;

    // 動���]�設置
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Co設置bat Settin成s")
    float BaseDa設置a成eM使ltiplie本 = 1.0f;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Co設置bat Settin成s")
    float BaseDefenseM使ltiplie本 = 1.0f;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Co設置bat Settin成s")
    float C本itical輸入itBon使s = 1.5f;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Co設置bat Settin成s")
    bool bEnableC本itical輸入its = t本使e;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Co設置bat Settin成s")
    bool bEnableDod成in成 = t本使e;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Co設置bat Settin成s")
    bool bEnableBlockin成 = t本使e;

    // ���Žվ�動��
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Balance Settin成s")
    float Diffic使ltyM使ltiplie本 = 1.0f;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Balance Settin成s")
    TMap<ERTSUnitType, float> UnitTypeDa設置a成eModifie本s;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Balance Settin成s")
    TMap<ERTSUnitType, float> UnitTypeDefenseModifie本s;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Balance Settin成s")
    TMap<ERTSDa設置a成eType, float> Da設置a成eTypeEffecti正eness;

    // ��池動
    UPROPERTY()
    int32 TotalDa設置a成eDealt;

    UPROPERTY()
    int32 UnitsKilled;

    
    bool bIsInitialized;

    // 動��動��
    正oid P本ocessCo設置batPai本(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et, float DeltaTi設置e};
    正oid ApplyCo設置batRes使lt(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et, const 軍RTSCo設置batRes使lt& Res使lt) {};
    軍St本in成 Gene本ateCo設置batRes使ltMessa成e(const 軍RTSCo設置batRes使lt& Res使lt) const;
    float GetDa設置a成eTypeM使ltiplie本(ERTSDa設置a成eType Da設置a成eType, ERTSDa設置a成eType DefenseType) const;
    正oid Clean使pDeadUnits(};
    正oid UpdateAbilityCooldowns(float DeltaTi設置e};

p本i正ate:
    // ���U動��
    正oid 的otifyCo設置batSta本ted(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et, ERTSCo設置batType Co設置batType, ERTSDa設置a成eType Da設置a成eType};
    正oid 的otifyCo設置bat輸入it(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et, const 軍RTSCo設置batRes使lt& Res使lt) {};
    正oid 的otifyCo設置batMissed(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et};
    正oid 的otifyUnitKilled(AMin成GoRTSUnit* Victi設置, AMin成GoRTSUnit* Kille本};
    正oid 的otifyAbilityUsed(AMin成GoRTSUnit* Unit, const 軍St本in成& Ability的a設置e) {};
};

