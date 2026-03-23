#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Kis設置et/Bl使ep本int軍使nctionLib本a本y.h"
#incl使de "Min成M使ltiUnitCoo本dinato本.h"
#incl使de "Min成M使ltiUnitBl使ep本intLib本a本y.成ene本ated.h"

class AMin成TacticalUnit;

/**
 * 故事重要性基本 (動 TA本本ay<TA本本ay<>> O動)
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍UnitA本本ay基本本appe本
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite)
    TA本本ay<AMin成TacticalUnit*> Units;

    軍UnitA本本ay基本本appe本() {}
};

/**
 * h池動w
 * 故事重要性基本h動ըt務 */
UCLASS()
class MI的GTACTICAL下API UMin成M使ltiUnitBl使ep本intLib本a本y : p使blic UBl使ep本int軍使nctionLib本a本y
{
    GE的ERATED下BODY()

p使blic:
    /**
     * h動վ
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本")
    static AMin成M使ltiUnitCoo本dinato本* GetM使ltiUnitCoo本dinato本(};

    /**
     * 故事選項X務     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static 正oid InitializeM使ltiUnitCoo本dinato本(};

    /**
     * h動վ
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static 正oid Sh使tdownM使ltiUnitCoo本dinato本(};

    /**
     * 動目標數量
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static 軍UnitG本o使p C本eateUnitG本o使p(
        const 軍St本in成& G本o使p的a設置e,
        EUnitG本o使pType G本o使pType,
        const TA本本ay<AMin成TacticalUnit*>& Units,
        AMin成TacticalUnit* Leade本 = n使llpt本
    };

    /**
     * 動目標數量 ()
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Batch")
    static TA本本ay<軍UnitG本o使p> BatchC本eateG本o使ps(
        const TA本本ay<軍St本in成>& G本o使p的a設置es,
        const TA本本ay<EUnitG本o使pType>& G本o使pTypes,
        const TA本本ay<軍UnitA本本ay基本本appe本>& UnitA本本ays
    };

    /**
     * 目標數量
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static bool DisbandUnitG本o使p(const 軍St本in成& G本o使pID};

    /**
     * 故事選項基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本")
    static 軍UnitG本o使p GetUnitG本o使p(const 軍St本in成& G本o使pID};

    /**
     * 目標數量     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本")
    static TA本本ay<軍UnitG本o使p> GetAllUnitG本o使ps(};

    /**
     * 目標數量務基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static TA本本ay<軍UnitG本o使p> GetG本o使psByType(EUnitG本o使pType G本o使pType};

    /**
     * 動目標數量
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static TA本本ay<軍UnitG本o使p> GetActi正eG本o使ps(};

    /**
     * K池務基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static bool AddUnitToG本o使p(const 軍St本in成& G本o使pID, AMin成TacticalUnit* Unit};

    /**
     * q動ղ     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static bool Re設置o正eUnit軍本o設置G本o使p(const 軍St本in成& G本o使pID, AMin成TacticalUnit* Unit};

    /**
     * ]設置故事重要性     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static bool SetG本o使pLeade本(const 軍St本in成& G本o使pID, AMin成TacticalUnit* Leade本};

    /**
     * 目標數量O
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static 軍Coo本dinationRes使lt Iss使eCoo本dinatedCo設置設置and(const 軍Coo本dinatedCo設置設置and& Co設置設置and};

    /**
     * 目標數量動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static 軍Coo本dinationRes使lt Exec使teCoo本dinatedAttack(
        const TA本本ay<軍St本in成>& G本o使pIDs,
        const TA本本ay<AMin成TacticalUnit*>& Ta本成ets,
        ECoo本dinationSyncType SyncType
    };

    /**
     * 目標數量設置
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static 軍Coo本dinationRes使lt Exec使teCoo本dinatedDefense(
        const TA本本ay<軍St本in成>& G本o使pIDs,
        const TA本本ay<軍Vecto本>& DefensePositions,
        ECoo本dinationSyncType SyncType
    };

    /**
     * 動池ղ動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static 軍Coo本dinationRes使lt Exec使teCoo本dinatedMo正e設置ent(
        const TA本本ay<軍St本in成>& G本o使pIDs,
        const TA本本ay<軍Vecto本>& Ta本成etPositions,
        ECoo本dinationSyncType SyncType
    };

    /**
     * 目標數量h
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static 軍Coo本dinationRes使lt Exec使teCoo本dinatedRet本eat(
        const TA本本ay<軍St本in成>& G本o使pIDs,
        const TA本本ay<軍Vecto本>& Ret本eatPositions,
        ECoo本dinationSyncType SyncType
    };

    /**
     * 目標數量
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static 軍Coo本dinationRes使lt Exec使teCoo本dinatedS使ppo本t(
        const TA本本ay<軍St本in成>& S使ppo本tG本o使pIDs,
        const TA本本ay<軍St本in成>& Ta本成etG本o使pIDs,
        ECoo本dinationSyncType SyncType
    };

    /**
     * 動池ե動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static 軍Coo本dinationRes使lt Exec使teCoo本dinatedA設置b使sh(
        const TA本本ay<軍St本in成>& A設置b使she本G本o使pIDs,
        const TA本本ay<AMin成TacticalUnit*>& Ta本成ets,
        ECoo本dinationSyncType SyncType
    };

    /**
     * 目標數量l
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static 軍Coo本dinationRes使lt Exec使teCoo本dinated軍lankin成(
        const TA本本ay<軍St本in成>& 軍lanke本G本o使pIDs,
        const TA本本ay<AMin成TacticalUnit*>& Ta本成ets,
        ECoo本dinationSyncType SyncType
    };

    /**
     * 目標數量動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static 軍Coo本dinationRes使lt Exec使teCoo本dinatedEnci本cle設置ent(
        const TA本本ay<軍St本in成>& Enci本cle本G本o使pIDs,
        const TA本本ay<AMin成TacticalUnit*>& Ta本成ets,
        ECoo本dinationSyncType SyncType
    };

    /**
     * 動池լ}
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static 軍Coo本dinationRes使lt Exec使teCoo本dinatedB本eakth本o使成h(
        const TA本本ay<軍St本in成>& B本eakth本o使成hG本o使pIDs,
        const TA本本ay<軍Vecto本>& B本eakth本o使成hPoints,
        ECoo本dinationSyncType SyncType
    };

    /**
     * 目標數量O
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static bool CancelCoo本dinatedCo設置設置and(const 軍St本in成& Co設置設置andID};

    /**
     * 目標數量O
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本")
    static 軍Coo本dinatedCo設置設置and GetCoo本dinatedCo設置設置and(const 軍St本in成& Co設置設置andID};

    /**
     * 目標數量動թR動     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本")
    static TA本本ay<軍Coo本dinatedCo設置設置and> GetAllCoo本dinatedCo設置設置ands(};

    /**
     * 目標數量池O
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static TA本本ay<軍Coo本dinatedCo設置設置and> GetExec使tin成Co設置設置ands(};

    /**
     * ˬdX     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本")
    static 軍St本in成 GetCoo本dinationStat使s(const 軍St本in成& Co設置設置andID};

    /**
     * 動池ղ動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本")
    static TMap<軍St本in成, int32> GetCoo本dinationStats下BPLib本a本y(};

    /**
     * 目標數量
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本")
    static TMap<軍St本in成, float> GetCoo本dinationPe本fo本設置anceMet本ics(};

    /**
     * 動B動池X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static bool Synch本onizeUnitStates(const TA本本ay<AMin成TacticalUnit*>& Units};

    /**
     * 動     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static bool B本oadcastToG本o使p(const 軍St本in成& G本o使pID, const 軍St本in成& Messa成e};

    /**
     * 年動     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static bool RelayMessa成e(const 軍St本in成& So使本ceG本o使pID, const 軍St本in成& Ta本成etG本o使pID, const 軍St本in成& Messa成e};

    /**
     * 池輸入X動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static bool EstablishCo設置設置使nication(const 軍St本in成& G本o使pAID, const 軍St本in成& G本o使pBID, ECo設置設置使nicationP本otocol P本otocol};

    /**
     * 動池輸入X動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本")
    static bool DisconnectCo設置設置使nication(const 軍St本in成& G本o使pAID, const 軍St本in成& G本o使pBID};

    /**
     * 動池輸入動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本")
    static float GetCo設置設置使nicationQ使ality(const 軍St本in成& G本o使pAID, const 軍St本in成& G本o使pBID};

    /**
     * 目標數量池
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本")
    static 軍St本in成 GetG本o使pType的a設置e(EUnitG本o使pType G本o使pType};

    /**
     * 目標數量O池
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本")
    static 軍St本in成 GetCoo本dinatedCo設置設置andType的a設置e(ECoo本dinatedCo設置設置andType Co設置設置andType};

    /**
     * 動池B池
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本")
    static 軍St本in成 GetSyncType的a設置e(ECoo本dinationSyncType SyncType};

    /**
     * 動池輸入動ĳ動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本")
    static 軍St本in成 GetP本otocol的a設置e(ECo設置設置使nicationP本otocol P本otocol};

    /**
     * 動O
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本C本eation")
    static 軍Coo本dinatedCo設置設置and C本eateCoo本dinatedCo設置設置and(
        const 軍St本in成& Co設置設置and的a設置e,
        ECoo本dinatedCo設置設置andType Co設置設置andType,
        const TA本本ay<軍St本in成>& Pa本ticipatin成G本o使ps,
        ECoo本dinationSyncType SyncType,
        ECo設置設置使nicationP本otocol Co設置設置使nicationP本otocol
    };

    /**
     * ]設置動O動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本C本eation")
    static 軍Coo本dinatedCo設置設置and SetCo設置設置andPa本a設置ete本(const 軍Coo本dinatedCo設置設置and& Co設置設置and, const 軍St本in成& Pa本a設置ete本的a設置e, const 軍St本in成& Pa本a設置ete本Val使e};

    /**
     * K池O動q
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本C本eation")
    static 軍Coo本dinatedCo設置設置and AddCo設置設置andPhase(const 軍Coo本dinatedCo設置設置and& Co設置設置and, const 軍St本in成& Phase的a設置e};

    /**
     * ]設置目標數量動     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本C本eation")
    static 軍Coo本dinatedCo設置設置and SetExec使tionSched使le(const 軍Coo本dinatedCo設置設置and& Co設置設置and, const TMap<軍St本in成, float>& Sched使le};

    /**
     * K池B動     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本C本eation")
    static 軍Coo本dinatedCo設置設置and AddSyncPoint(const 軍Coo本dinatedCo設置設置and& Co設置設置and, const 軍St本in成& SyncPoint};

    /**
     * K動̿
     */
    U軍U的CTIO的(Cate成o本y = "M使lti Unit Coo本dinato本C本eation")
    static 軍Coo本dinatedCo設置設置and AddDependency(const 軍Coo本dinatedCo設置設置and& Co設置設置and, const 軍St本in成& Dependency};

    /**
     * 目標數量動
     */
    static TA本本ay<軍UnitG本o使p> BatchC本eateG本o使ps(
        const TA本本ay<軍St本in成>& G本o使p的a設置es,
        const TA本本ay<EUnitG本o使pType>& G本o使pTypes,
        const TA本本ay<TA本本ay<AMin成TacticalUnit*>>& UnitA本本ays
    };

    /**
     * 故事選項是
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Batch")
    static TA本本ay<軍Coo本dinationRes使lt> BatchExec使teCo設置設置ands(const TA本本ay<軍Coo本dinatedCo設置設置and>& Co設置設置ands};

    /**
     * 故事重要性是
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Batch")
    static TA本本ay<bool> BatchCancelCo設置設置ands(const TA本本ay<軍St本in成>& Co設置設置andIDs};

    /**
     * t動X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本Syste設置")
    static bool IsM使ltiUnitCoo本dinato本Initialized(};

    /**
     * t動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本Syste設置")
    static 軍St本in成 GetM使ltiUnitCoo本dinato本Ve本sion(};

    /**
     * 目標數量軍事
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本Syste設置")
    static int32 GetTotalG本o使pCo使nt(};

    /**
     * 池池動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本Syste設置")
    static int32 GetActi正eG本o使pCo使nt(};

    /**
     * 目標數量O軍事
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本Syste設置")
    static int32 GetTotalCo設置設置andCo使nt(};

    /**
     * 目標數量ROX     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本Syste設置")
    static int32 GetExec使tin成Co設置設置andCo使nt(};

    /**
     * ˬd動池下sb
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本Info")
    static bool DoesG本o使pExist(const 軍St本in成& G本o使pID};

    /**
     * ˬd動池下動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本Info")
    static bool IsG本o使pActi正e(const 軍St本in成& G本o使pID};

    /**
     * ˬd動池下動X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本Info")
    static bool CanG本o使pDisband(const 軍St本in成& G本o使pID};

    /**
     * 故事選項X動基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本Info")
    static int32 GetG本o使pUnitCo使nt(const 軍St本in成& G本o使pID};

    /**
     * 故事選項X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本Info")
    static AMin成TacticalUnit* GetG本o使pLeade本(const 軍St本in成& G本o使pID};

    /**
     * 故事重要性務     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本Info")
    static 軍St本in成 GetG本o使pStat使s(const 軍St本in成& G本o使pID};

    /**
     * ˬd動O動下sb
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本Info")
    static bool DoesCo設置設置andExist(const 軍St本in成& Co設置設置andID};

    /**
     * ˬd動O動下
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本Info")
    static bool IsCo設置設置andExec使tin成(const 軍St本in成& Co設置設置andID};

    /**
     * ˬd動O動下w務     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本Info")
    static bool IsCo設置設置andCo設置pleted(const 軍St本in成& Co設置設置andID};

    /**
     * ˬd動O動下w     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本Info")
    static bool IsCo設置設置andCancelled(const 軍St本in成& Co設置設置andID};

    /**
     * ˬd動輸入X下sb
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使lti Unit Coo本dinato本Info")
    static bool DoesCo設置設置使nicationExist(const 軍St本in成& G本o使pAID, const 軍St本in成& G本o使pBID};

    /**
     * p池動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Calc使lation")
    static float Calc使lateCoo本dinationSco本e(const TA本本ay<軍St本in成>& G本o使pIDs, ECoo本dinatedCo設置設置andType Co設置設置andType};

    /**
     * p池B
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Calc使lation")
    static float Calc使lateSyncAcc使本acy(const 軍St本in成& Co設置設置andID};

    /**
     * p池輸入
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Calc使lation")
    static float Calc使lateCo設置設置使nicationDelay(const 軍St本in成& So使本ceG本o使pID, const 軍St本in成& Ta本成etG本o使pID};

    /**
     * p池務基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Calc使lation")
    static float Calc使lateG本o使pCoo本dinationCapability(const 軍St本in成& G本o使pID};

    /**
     * p池務基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Calc使lation")
    static float Calc使lateO正e本allCoo本dinationEfficiency(};

    /**
     * 目標數量B務     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Opti設置ization")
    static ECoo本dinationSyncType GetOpti設置alSyncType(const TA本本ay<軍St本in成>& G本o使pIDs, ECoo本dinatedCo設置設置andType Co設置設置andType};

    /**
     * 動池γq輸入動ĳ
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Opti設置ization")
    static ECo設置設置使nicationP本otocol GetOpti設置alCo設置設置使nicationP本otocol(const TA本本ay<軍St本in成>& G本o使pIDs};

    /**
     * 動池իĳ
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本AI")
    static TA本本ay<軍St本in成> GetCoo本dinationReco設置設置endations(const TA本本ay<軍St本in成>& G本o使pIDs};

    /**
     * 目標數量ĳ
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本AI")
    static TA本本ay<軍St本in成> GetTacticalReco設置設置endations(const TA本本ay<軍St本in成>& G本o使pIDs, ECoo本dinatedCo設置設置andType Co設置設置andType};

    /**
     * 目標數量ĳ
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本AI")
    static TA本本ay<AMin成TacticalUnit*> GetAttackTa本成ets(const TA本本ay<軍St本in成>& G本o使pIDs};

    /**
     * 動池設置ĳ
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本AI")
    static TA本本ay<軍Vecto本> GetDefensePositions(const TA本本ay<軍St本in成>& G本o使pIDs};

    /**
     * 動ĳ
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本AI")
    static TA本本ay<軍Vecto本> GetMo正e設置entTa本成ets(const TA本本ay<軍St本in成>& G本o使pIDs};

    /**
     * 動池hĳ
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本AI")
    static TA本本ay<軍Vecto本> GetRet本eatPositions(const TA本本ay<軍St本in成>& G本o使pIDs};

    /**
     * 動յ動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Si設置使lation")
    static TA本本ay<軍Coo本dinationRes使lt> Si設置使lateCoo本dination(
        const TA本本ay<軍St本in成>& G本o使pIDs,
        ECoo本dinatedCo設置設置andType Co設置設置andType,
        ECoo本dinationSyncType SyncType,
        int32 Si設置使lationCo使nt
    };

    /**
     * 動輸入
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Si設置使lation")
    static TA本本ay<float> Si設置使lateCo設置設置使nicationQ使ality(
        const TA本本ay<軍St本in成>& G本o使pIDs,
        ECo設置設置使nicationP本otocol P本otocol,
        int32 Si設置使lationCo使nt
    };

    /**
     * 目標數量正
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本輸入isto本y")
    static TA本本ay<軍St本in成> GetG本o使p輸入isto本y(const 軍St本in成& G本o使pID};

    /**
     * 動池O正
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本輸入isto本y")
    static TA本本ay<軍St本in成> GetCo設置設置and輸入isto本y(};

    /**
     * 動池輸入正
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本輸入isto本y")
    static TA本本ay<軍St本in成> GetCo設置設置使nication輸入isto本y(};

    /**
     * 動池ղ動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Statistics")
    static TMap<軍St本in成, int32> GetCoo本dinationStats下BPLib本a本y(};

    /**
     * 目標數量動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Statistics")
    static TMap<軍St本in成, int32> GetG本o使pStatistics(};

    /**
     * 動池輸入動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Statistics")
    static TMap<軍St本in成, int32> GetCo設置設置使nicationStatistics(};

    /**
     * 目標數量
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Statistics")
    static TMap<軍St本in成, float> GetPe本fo本設置anceStatistics(};

    /**
     * 基本X動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Expo本t")
    static bool Expo本tCoo本dinationData(const 軍St本in成& 軍ilePath};

    /**
     * ɤJ動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本I設置po本t")
    static bool I設置po本tCoo本dinationData(const 軍St本in成& 軍ilePath};

    /**
     * 故事選項基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Repo本t")
    static 軍St本in成 Gene本ateG本o使pRepo本t(const 軍St本in成& G本o使pID};

    /**
     * 目標數量X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Repo本t")
    static 軍St本in成 Gene本ateCoo本dinato本Repo本t(};

    /**
     * 動池O
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Repo本t")
    static 軍St本in成 Gene本ateCo設置設置andRepo本t(const 軍St本in成& Co設置設置andID};

    /**
     * 目標數量池動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Repo本t")
    static 軍St本in成 Gene本ateCoo本dinationAnalysisRepo本t(};

    /**
     * 動設置h動վ
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Syste設置")
    static 正oid ResetM使ltiUnitCoo本dinato本(};

    /**
     * M     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Syste設置")
    static 正oid Clea本AllG本o使ps(};

    /**
     * MR動     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Syste設置")
    static 正oid Clea本AllCo設置設置ands(};

    /**
     * Mq輸入X動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Syste設置")
    static 正oid Clea本AllCo設置設置使nications(};

    /**
     * 
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Syste設置")
    static bool Back使pCoo本dinationData(const 軍St本in成& Back使pPath};

    /**
     * 動下動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti Unit Coo本dinato本Syste設置")
    static bool Resto本eCoo本dinationData(const 軍St本in成& Back使pPath};
};

