#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成RTSUnitMana成e本.成ene本ated.h"

class AMin成GoRTSUnit;
class AMin成GoRTSPlaye本Cont本olle本;

UE的UM(Bl使ep本intType)
en使設置 class ERTSUnitType: 使int8 {
    Infant本y,       // �B�L
    Ca正al本y,        // �M�L
    A本tille本y,      // 動��
    Sco使t,          // 
    En成inee本,       // �使�L
    Medic,          // 動務
    Co設置設置ande本,      // 動��動
    S使pply          // �ɵ�
};

UE的UM(Bl使ep本intType)
en使設置 class ERTSUnitState: 使int8 {
    Idle,           // ��動
    Mo正in成,         // ��
    Attackin成,      // 動
    Defendin成,      // 動�設置動
    Ret本eatin成,     // 動�h動
    Dead,           // ���軍事
    B使ildin成        // �سy��
};

UE的UM(Bl使ep本intType)
en使設置 class ERTS軍o本設置ationType: 使int8 {
    的one,           // 動�}X
    Line,           // �使��X
    Col使設置n,         // �a務
    基本ed成e,          // ����X
    Ci本cle,         // 動��X
    Sq使a本e          // 動��X
};

USTRUCT(Bl使ep本intType)
st本使ct 軍RTSUnitData
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Unit Data")
    軍St本in成 UnitID;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Unit Data")
    ERTSUnitType UnitType;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Unit Data")
    ERTSUnitState C使本本entState;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Unit Data")
    軍Vecto本 C使本本entLocation;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Unit Data")
    軍Vecto本 Ta本成etLocation;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Unit Data")
    float 輸入ealth;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Unit Data")
    float Max輸入ealth;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Unit Data")
    float AttackPowe本;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Unit Data")
    float DefensePowe本;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Unit Data")
    float Mo正e設置entSpeed;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Unit Data")
    float AttackRan成e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Unit Data")
    int32 UnitLe正el;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Unit Data")
    bool bIsSelected;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Unit Data")
    軍DateTi設置e LastActionTi設置e;

    軍RTSUnitData()
    {
        UnitID = TEXT(""};
        UnitType = ERTSUnitType::Infant本y;
        C使本本entState = ERTSUnitState::Idle;
        C使本本entLocation = 軍Vecto本::Ze本oVecto本;
        Ta本成etLocation = 軍Vecto本::Ze本oVecto本;
        輸入ealth = 100.0f;
        Max輸入ealth = 100.0f;
        AttackPowe本 = 10.0f;
        DefensePowe本 = 5.0f;
        Mo正e設置entSpeed = 300.0f;
        AttackRan成e = 200.0f;
        UnitLe正el = 1;
        bIsSelected = false;
};

USTRUCT(Bl使ep本intType)
st本使ct 軍RTS軍o本設置ationData
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "軍o本設置ation Data")
    ERTS軍o本設置ationType 軍o本設置ationType;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "軍o本設置ation Data")
    軍Vecto本 軍o本設置ationCente本;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "軍o本設置ation Data")
    float 軍o本設置ationSpacin成;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "軍o本設置ation Data")
    軍Rotato本 軍o本設置ationRotation;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "軍o本設置ation Data")
    TA本本ay<軍Vecto本> UnitPositions;

    軍RTS軍o本設置ationData()
    {
        軍o本設置ationType = ERTS軍o本設置ationType::的one;
        軍o本設置ationCente本 = 軍Vecto本::Ze本oVecto本;
        軍o本設置ationSpacin成 = 100.0f;
        軍o本設置ationRotation = 軍Rotato本::Ze本oRotato本;
};






/**
 * RTS��務 * ��池動RTS動池��動�B��動�B�}動池X */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GCORE下API UMin成RTSUnitMana成e本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成RTSUnitMana成e本(};

    
    正oid InitializeUnitMana成e本(AMin成GoRTSPlaye本Cont本olle本* InPlaye本Cont本olle本};

    // ��動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    正oid Re成iste本Unit(AMin成GoRTSUnit* Unit};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    正oid Un本e成iste本Unit(AMin成GoRTSUnit* Unit};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    AMin成GoRTSUnit* GetUnit(const 軍St本in成& UnitID) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    TA本本ay<AMin成GoRTSUnit*> GetAllUnits() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    TA本本ay<AMin成GoRTSUnit*> GetUnitsByType(ERTSUnitType UnitType) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    TA本本ay<AMin成GoRTSUnit*> GetUnitsByState(ERTSUnitState State) const;

    // �t��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    正oid SelectUnit(AMin成GoRTSUnit* Unit, bool bAddToSelection = false};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    正oid SelectUnits(const TA本本ay<AMin成GoRTSUnit*>& Units};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    正oid SelectUnitsInRect(軍Vecto本2D Sc本eenSta本t, 軍Vecto本2D Sc本eenEnd};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    正oid DeselectAllUnits(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    正oid DeselectUnit(AMin成GoRTSUnit* Unit};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    TA本本ay<AMin成GoRTSUnit*> GetSelectedUnits() const { 本et使本n SelectedUnits; }

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    int32 GetSelectedUnitCo使nt() const { 本et使本n SelectedUnits.的使設置(); }

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    bool IsUnitSelected(AMin成GoRTSUnit* Unit) const;

    // ��動�t��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    正oid Mo正eUnits(const TA本本ay<AMin成GoRTSUnit*>& Units, const 軍Vecto本& Ta本成etLocation};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    正oid Mo正eSelectedUnits(const 軍Vecto本& Ta本成etLocation};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    正oid StopUnitMo正e設置ent(AMin成GoRTSUnit* Unit};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    正oid StopAllUnitMo正e設置ent(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    bool IsUnitMo正in成(AMin成GoRTSUnit* Unit) const;

    // X�t��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    正oid Set軍o本設置ation(ERTS軍o本設置ationType 軍o本設置ationType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    正oid Apply軍o本設置ationToSelectedUnits(const 軍Vecto本& Cente本};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    軍RTS軍o本設置ationData Calc使late軍o本設置ation(ERTS軍o本設置ationType 軍o本設置ationType, const TA本本ay<AMin成GoRTSUnit*>& Units, const 軍Vecto本& Cente本};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    正oid Update軍o本設置ation(};

    
    正oid SetUnitState(AMin成GoRTSUnit* Unit, ERTSUnitState 的ewState};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    ERTSUnitState GetUnitState(AMin成GoRTSUnit* Unit) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    正oid UpdateUnitStates(float DeltaTi設置e};

    // 動���t��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    正oid AttackUnit(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    正oid StopAttack(AMin成GoRTSUnit* Unit};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    bool CanAttackUnit(AMin成GoRTSUnit* Attacke本, AMin成GoRTSUnit* Ta本成et) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    TA本本ay<AMin成GoRTSUnit*> GetUnitsInRan成e(AMin成GoRTSUnit* Unit, float Ran成e) const;

    // 目標數量
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    軍RTSUnitData GetUnitData(AMin成GoRTSUnit* Unit) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    正oid SetUnitData(AMin成GoRTSUnit* Unit, const 軍RTSUnitData& UnitData};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    正oid UpdateUnit輸入ealth(AMin成GoRTSUnit* Unit, float 輸入ealthChan成e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    bool IsUnitAli正e(AMin成GoRTSUnit* Unit) const;

    // ��動�基本動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    TA本本ay<軍Vecto本> Calc使latePath(AMin成GoRTSUnit* Unit, const 軍Vecto本& Ta本成etLocation};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    bool 輸入asValidPath(AMin成GoRTSUnit* Unit) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    正oid Recalc使latePaths(};

    // �s池��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    正oid C本eateUnitG本o使p(const TA本本ay<AMin成GoRTSUnit*>& Units, const 軍St本in成& G本o使p的a設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    正oid SelectUnitG本o使p(const 軍St本in成& G本o使p的a設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "RTS Unit Mana成e本")
    TA本本ay<軍St本in成> GetUnitG本o使ps() const;

    // ��動�輸入��
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "RTS Unit Mana成e本")
    int32 GetTotalUnitCo使nt() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "RTS Unit Mana成e本")
    int32 GetUnitCo使ntByType(ERTSUnitType UnitType) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "RTS Unit Mana成e本")
    int32 GetAli正eUnitCo使nt() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "RTS Unit Mana成e本")
    int32 GetMo正in成UnitCo使nt() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "RTS Unit Mana成e本")
    int32 GetAttackin成UnitCo使nt() const;

    // �基礎�
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "RTS Unit E正ents")
    軍OnUnitSelected OnUnitSelected;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "RTS Unit E正ents")
    軍OnUnitStateChan成ed OnUnitStateChan成ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "RTS Unit E正ents")
    軍OnUnitsMo正ed OnUnitsMo正ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "RTS Unit E正ents")
    軍OnUnitDa設置a成ed OnUnitDa設置a成ed;

p本otected:
    // �t�Τޥ�
    UPROPERTY()
    TOb大ectPt本<AMin成GoRTSPlaye本Cont本olle本> Playe本Cont本olle本;

    // �s�x
    
    TMap<軍St本in成, AMin成GoRTSUnit*> AllUnits;

    
    TA本本ay<AMin成GoRTSUnit*> SelectedUnits;

    
    TMap<軍St本in成, TA本本ay<AMin成GoRTSUnit*>> UnitG本o使ps;

    // X動基本
    UPROPERTY()
    ERTS軍o本設置ationType C使本本ent軍o本設置ation;

    UPROPERTY()
    軍RTS軍o本設置ationData C使本本ent軍o本設置ationData;

    
    bool bIsInitialized;

    // �]�設置
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "RTS Settin成s")
    float SelectionBoxMa本成in = 50.0f;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "RTS Settin成s")
    float 軍o本設置ationSpacin成 = 150.0f;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "RTS Settin成s")
    bool bA使to軍o本設置ation = t本使e;

    // 動��動��
    正oid UpdateUnitSelection(};
    正oid P本ocessUnitMo正e設置ent(float DeltaTi設置e};
    正oid P本ocessCo設置bat(float DeltaTi設置e};
    軍Vecto本 Calc使late軍o本設置ationPosition(AMin成GoRTSUnit* Unit, const 軍RTS軍o本設置ationData& 軍o本設置ation) const;
    bool IsUnitInSelectionRect(AMin成GoRTSUnit* Unit, 軍Vecto本2D Sc本eenSta本t, 軍Vecto本2D Sc本eenEnd) const;
    正oid Clean使pDeadUnits(};
    軍St本in成 Gene本ateUnitID(ERTSUnitType UnitType) const;

p本i正ate:
    // ���U動��
    正oid 的otifyUnitSelected(AMin成GoRTSUnit* Unit, bool bIsSelected};
    正oid 的otifyUnitStateChan成ed(AMin成GoRTSUnit* Unit, ERTSUnitState 的ewState};
    正oid 的otifyUnitsMo正ed(const TA本本ay<AMin成GoRTSUnit*>& Units, const 軍Vecto本& Ta本成etLocation, bool bIs軍o本設置ation};
    正oid 的otifyUnitDa設置a成ed(AMin成GoRTSUnit* Unit};
};

