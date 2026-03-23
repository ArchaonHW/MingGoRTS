#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Kis設置et/Bl使ep本int軍使nctionLib本a本y.h"
#incl使de "Min成UnitCont本olle本.h"
#incl使de "Min成UnitBl使ep本intLib本a本y.成ene本ated.h"

class AMin成TacticalUnit;

/**
 * 故事選項基本 * 故事重要性務池t動
 */
UCLASS()
class MI的GTACTICAL下API UMin成UnitBl使ep本intLib本a本y : p使blic UBl使ep本int軍使nctionLib本a本y
{
    GE的ERATED下BODY()

p使blic:
    /**
     * 故事重要性務     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本")
    static AMin成UnitCont本olle本* GetUnitCont本olle本(};

    /**
     * 動池챱動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本")
    static 正oid InitializeUnitCont本olle本(};

    /**
     * 故事重要性務     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本")
    static 正oid Sh使tdownUnitCont本olle本(};

    /**
     * K務基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本")
    static bool AddCont本olledUnit(AMin成TacticalUnit* Unit};

    /**
     * 動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本")
    static bool Re設置o正eCont本olledUnit(AMin成TacticalUnit* Unit};

    /**
     * 目標數量池動     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本")
    static TA本本ay<AMin成TacticalUnit*> GetCont本olledUnits(};

    /**
     * w池池動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本")
    static TA本本ay<AMin成TacticalUnit*> GetUnitsByType(const 軍St本in成& UnitType};

    /**
     * 動目標數量
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本")
    static TA本本ay<AMin成TacticalUnit*> GetActi正eUnits(};

    /**
     * 故事重要性是
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本")
    static 軍UnitCont本olRes使lt Iss使eUnitCo設置設置and(const 軍UnitCo設置設置and& Co設置設置and};

    /**
     * 故事重要性是
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本")
    static TA本本ay<軍UnitCont本olRes使lt> BatchIss使eCo設置設置ands(const TA本本ay<軍UnitCo設置設置and>& Co設置設置ands};

    /**
     * 動池O
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本")
    static bool CancelCo設置設置and(const 軍St本in成& Co設置設置andID};

    /**
     * 目標數量R動     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本")
    static bool CancelAllCo設置設置ands(};

    /**
     * 池務基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本")
    static 軍UnitCont本olRes使lt Mo正eUnitsToLocation(const TA本本ay<AMin成TacticalUnit*>& Units, const 軍Vecto本& Ta本成etLocation, EUnitMo正e設置entType Mo正e設置entType};

    /**
     * 目標數量
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本")
    static 軍UnitCont本olRes使lt AttackTa本成et(const TA本本ay<AMin成TacticalUnit*>& Units, AMin成TacticalUnit* Ta本成etUnit, EUnitAttackType AttackType};

    /**
     * 動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本")
    static 軍UnitCont本olRes使lt AttackB使ildin成(const TA本本ay<AMin成TacticalUnit*>& Units, AActo本* Ta本成etB使ildin成, EUnitAttackType AttackType};

    /**
     * 動     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本")
    static 軍UnitCont本olRes使lt StopUnits(const TA本本ay<AMin成TacticalUnit*>& Units};

    /**
     * 動設置設置
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本")
    static 軍UnitCont本olRes使lt 輸入oldPosition(const TA本本ay<AMin成TacticalUnit*>& Units, const 軍Vecto本& 輸入oldLocation};

    /**
     * 
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本")
    static 軍UnitCont本olRes使lt Pat本olPath(const TA本本ay<AMin成TacticalUnit*>& Units, const TA本本ay<軍Vecto本>& Pat本olPoints};

    /**
     * 使@
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本")
    static 軍UnitCont本olRes使lt G使a本dTa本成et(const TA本本ay<AMin成TacticalUnit*>& Units, AMin成TacticalUnit* Ta本成etToG使a本d};

    /**
     * 動h
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本")
    static 軍UnitCont本olRes使lt Ret本eatUnits(const TA本本ay<AMin成TacticalUnit*>& Units, const 軍Vecto本& Ret本eatLocation};

    /**
     * 動s動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本")
    static 軍UnitCont本olRes使lt 軍o本設置UpUnits(const TA本本ay<AMin成TacticalUnit*>& Units, EUnit軍o本設置ationType 軍o本設置ationType, const 軍Vecto本& 軍o本設置ationCente本};

    /**
     * ϥ動X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本")
    static 軍UnitCont本olRes使lt UseAbility(const TA本本ay<AMin成TacticalUnit*>& Units, const 軍St本in成& AbilityID, AMin成TacticalUnit* Ta本成etUnit = n使llpt本};

    /**
     * 故事重要性務     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本")
    static EMin成UnitState GetUnitState(AMin成TacticalUnit* Unit};

    /**
     * ]設置動池X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本")
    static bool SetUnitState(AMin成TacticalUnit* Unit, EMin成UnitState 的ewState};

    /**
     * 故事重要性是
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本")
    static TA本本ay<軍UnitCo設置設置and> GetUnitCo設置設置andQ使e使e(AMin成TacticalUnit* Unit};

    /**
     * 故事重要性是
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本")
    static 軍UnitCo設置設置and GetC使本本entCo設置設置and(AMin成TacticalUnit* Unit};

    /**
     * ˬd動池下動R動     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本")
    static bool CanExec使teCo設置設置and(AMin成TacticalUnit* Unit, const 軍UnitCo設置設置and& Co設置設置and};

    /**
     * 故事選項基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本")
    static TMap<軍St本in成, int32> GetUnitCont本olStatistics(};

    /**
     * 目標數量ʯ
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本")
    static TMap<軍St本in成, float> GetPe本fo本設置anceMet本ics(};

    /**
     * 目標數量     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本")
    static 軍St本in成 GetState的a設置e(EMin成UnitState State};

    /**
     * 動池O池
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本")
    static 軍St本in成 GetCo設置設置andType的a設置e(EUnitCo設置設置andType Co設置設置andType};

    /**
     * 動池O     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本")
    static 軍St本in成 GetP本io本ity的a設置e(EUnitCo設置設置andP本io本ity P本io本ity};

    /**
     * 動池
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本")
    static 軍St本in成 GetMo正e設置entType的a設置e(EUnitMo正e設置entType Mo正e設置entType};

    /**
     * 目標數量池
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本")
    static 軍St本in成 GetAttackType的a設置e(EUnitAttackType AttackType};

    /**
     * s動池
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本")
    static 軍St本in成 Get軍o本設置ationType的a設置e(EUnit軍o本設置ationType 軍o本設置ationType};

    /**
     * 池O
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本C本eation")
    static 軍UnitCo設置設置and C本eateUnitCo設置設置and(
        EUnitCo設置設置andType Co設置設置andType,
        EUnitCo設置設置andP本io本ity P本io本ity,
        const 軍Vecto本& Ta本成etLocation,
        AMin成TacticalUnit* Ta本成etUnit,
        AActo本* Ta本成etB使ildin成,
        EUnitMo正e設置entType Mo正e設置entType,
        EUnitAttackType AttackType,
        EUnit軍o本設置ationType 軍o本設置ationType
    };

    /**
     * ]設置動O動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本C本eation")
    static 軍UnitCo設置設置and SetCo設置設置andPa本a設置ete本(const 軍UnitCo設置設置and& Co設置設置and, const 軍St本in成& Pa本a設置ete本的a設置e, const 軍St本in成& Pa本a設置ete本Val使e};

    /**
     * ]設置動O動զ
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本C本eation")
    static 軍UnitCo設置設置and SetCo設置設置andRet本yCo使nt(const 軍UnitCo設置設置and& Co設置設置and, int32 MaxRet本yCo使nt};

    /**
     * 動目標數量
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本Batch")
    static TA本本ay<軍UnitCont本olRes使lt> BatchMo正eUnits(const TA本本ay<AMin成TacticalUnit*>& Units, const TA本本ay<軍Vecto本>& Ta本成etLocations, EUnitMo正e設置entType Mo正e設置entType};

    /**
     * 故事選項基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本Batch")
    static TA本本ay<軍UnitCont本olRes使lt> BatchAttackTa本成ets(const TA本本ay<AMin成TacticalUnit*>& Units, const TA本本ay<AMin成TacticalUnit*>& Ta本成etUnits, EUnitAttackType AttackType};

    /**
     * 目標數量動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本Batch")
    static TA本本ay<軍UnitCont本olRes使lt> BatchStopUnits(const TA本本ay<AMin成TacticalUnit*>& Units};

    /**
     * t動X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本Syste設置")
    static bool IsUnitCont本olle本Initialized(};

    /**
     * t動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本Syste設置")
    static 軍St本in成 GetUnitCont本olle本Ve本sion(};

    /**
     * 目標數量池動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本Syste設置")
    static int32 GetCont本olledUnitCo使nt(};

    /**
     * 池池動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本Syste設置")
    static int32 GetActi正eUnitCo使nt(};

    /**
     * 動池O軍事
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本Syste設置")
    static int32 GetTotalCo設置設置andCo使nt(};

    /**
     * 目標數量ROX     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本Syste設置")
    static int32 GetExec使tin成Co設置設置andCo使nt(};

    /**
     * ˬd動池下動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本Info")
    static bool IsUnitCont本olled(AMin成TacticalUnit* Unit};

    /**
     * ˬd動池下動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本Info")
    static bool IsUnitActi正e(AMin成TacticalUnit* Unit};

    /**
     * ˬd動池下動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本Info")
    static bool IsUnitIdle(AMin成TacticalUnit* Unit};

    /**
     * ˬd動池下動X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本Info")
    static bool IsUnitMo正in成(AMin成TacticalUnit* Unit};

    /**
     * ˬd動池下動X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本Info")
    static bool IsUnitAttackin成(AMin成TacticalUnit* Unit};

    /**
     * ˬd動池下     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本Info")
    static bool IsUnitDefendin成(AMin成TacticalUnit* Unit};

    /**
     * 目標數量設置
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本Info")
    static 軍Vecto本 GetUnitLocation(AMin成TacticalUnit* Unit};

    /**
     * 故事選項基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本Info")
    static 軍Rotato本 GetUnitRotation(AMin成TacticalUnit* Unit};

    /**
     * ]設置設置
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本Info")
    static bool SetUnitLocation(AMin成TacticalUnit* Unit, const 軍Vecto本& 的ewLocation};

    /**
     * ]設置目標數量
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本Info")
    static bool SetUnitRotation(AMin成TacticalUnit* Unit, const 軍Rotato本& 的ewRotation};

    /**
     * p池動Z
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本Math")
    static float Calc使lateDistance(const 軍Vecto本& PointA, const 軍Vecto本& PointB};

    /**
     * p池池務     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本Math")
    static float Calc使lateUnitDistance(AMin成TacticalUnit* UnitA, AMin成TacticalUnit* UnitB};

    /**
     * ˬd動下動池
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本Math")
    static bool IsInRan成e(const 軍Vecto本& PointA, const 軍Vecto本& PointB, float Ran成e};

    /**
     * ˬd動池下池
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本Math")
    static bool IsUnitInAttackRan成e(AMin成TacticalUnit* Attacke本, AMin成TacticalUnit* Ta本成et};

    /**
     * 池
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本Calc使lation")
    static float GetMo正e設置entSpeed(AMin成TacticalUnit* Unit, EUnitMo正e設置entType Mo正e設置entType};

    /**
     * p目標數量
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本Calc使lation")
    static float Calc使lateMo正e設置entTi設置e(AMin成TacticalUnit* Unit, const 軍Vecto本& Sta本tLocation, const 軍Vecto本& Ta本成etLocation, EUnitMo正e設置entType Mo正e設置entType};

    /**
     * p池池軍事
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本Calc使lation")
    static float Calc使lateAttackDa設置a成e(AMin成TacticalUnit* Attacke本, AMin成TacticalUnit* Ta本成et, EUnitAttackType AttackType};

    /**
     * p動s動設置
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本軍o本設置ation")
    static TA本本ay<軍Vecto本> Calc使late軍o本設置ationPositions(const TA本本ay<AMin成TacticalUnit*>& Units, EUnit軍o本設置ationType 軍o本設置ationType, const 軍Vecto本& Cente本};

    /**
     * ˬds動X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Unit Cont本olle本軍o本設置ation")
    static bool Is軍o本設置ationCo設置plete(const TA本本ay<AMin成TacticalUnit*>& Units, const TA本本ay<軍Vecto本>& 軍o本設置ationPositions};

    /**
     * 動ss動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本軍o本設置ation")
    static bool Update軍o本設置ation(const TA本本ay<AMin成TacticalUnit*>& Units, const TA本本ay<軍Vecto本>& 軍o本設置ationPositions};

    /**
     * 目標數量ĳ
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本AI")
    static TA本本ay<軍St本in成> GetUnitReco設置設置endations(AMin成TacticalUnit* Unit};

    /**
     * 目標數量ĳ
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本AI")
    static TA本本ay<軍St本in成> GetTacticalReco設置設置endations(const TA本本ay<AMin成TacticalUnit*>& Units};

    /**
     * 目標數量ĳ
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本AI")
    static TA本本ay<AMin成TacticalUnit*> GetAttackTa本成ets(const TA本本ay<AMin成TacticalUnit*>& Units};

    /**
     * 動池設置ĳ
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本AI")
    static TA本本ay<軍Vecto本> GetDefensePositions(const TA本本ay<AMin成TacticalUnit*>& Units};

    /**
     * 動池ĳ
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本AI")
    static TA本本ay<軍Vecto本> GetRet本eatPositions(const TA本本ay<AMin成TacticalUnit*>& Units};

    /**
     * 
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本Si設置使lation")
    static TA本本ay<軍UnitCont本olRes使lt> Si設置使lateBattle(const TA本本ay<AMin成TacticalUnit*>& Attacke本s, const TA本本ay<AMin成TacticalUnit*>& Defende本s, int32 Si設置使lationCo使nt};

    /**
     * 
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本Si設置使lation")
    static TA本本ay<軍Vecto本> Si設置使lateMo正e設置entPath(AMin成TacticalUnit* Unit, const 軍Vecto本& Sta本tLocation, const 軍Vecto本& Ta本成etLocation};

    /**
     * 目標數量正
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本輸入isto本y")
    static TA本本ay<軍UnitCo設置設置and> GetUnit輸入isto本y(AMin成TacticalUnit* Unit};

    /**
     * 動池O正
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本輸入isto本y")
    static TA本本ay<軍UnitCont本olRes使lt> GetCo設置設置and輸入isto本y(};

    /**
     * 目標數量
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本Statistics")
    static TMap<軍St本in成, int32> GetBattleStatistics(};

    /**
     * 目標數量動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本Statistics")
    static TMap<軍St本in成, int32> GetUnitStatistics(};

    /**
     * 目標數量
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本Statistics")
    static TMap<軍St本in成, float> GetPe本fo本設置anceStatistics(};

    /**
     * 基本X目標數量
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本Expo本t")
    static bool Expo本tUnitData(const 軍St本in成& 軍ilePath};

    /**
     * ɤJ目標數量
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本I設置po本t")
    static bool I設置po本tUnitData(const 軍St本in成& 軍ilePath};

    /**
     * 故事選項基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本Repo本t")
    static 軍St本in成 Gene本ateUnitRepo本t(AMin成TacticalUnit* Unit};

    /**
     * 目標數量X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本Repo本t")
    static 軍St本in成 Gene本ateCont本olle本Repo本t(};

    /**
     * 目標數量動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本Repo本t")
    static 軍St本in成 Gene本ateBattleRepo本t(const TA本本ay<AMin成TacticalUnit*>& Attacke本s, const TA本本ay<AMin成TacticalUnit*>& Defende本s};

    /**
     * 動設置動池X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本Syste設置")
    static 正oid ResetUnitCont本olle本(};

    /**
     * MR動     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本Syste設置")
    static 正oid Clea本AllCo設置設置ands(};

    /**
     * M正
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本Syste設置")
    static 正oid Clea本Unit輸入isto本y(};

    /**
     * 動目標數量
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本Syste設置")
    static bool Back使pUnitData(const 軍St本in成& Back使pPath};

    /**
     * 動下目標數量
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Unit Cont本olle本Syste設置")
    static bool Resto本eUnitData(const 軍St本in成& Back使pPath};
};
};
