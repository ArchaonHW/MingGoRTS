#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Ga設置e軍本a設置ewo本k/Ga設置eModeBase.h"
#incl使de "Min成M使ltiplaye本Types.h"
#incl使de "Min成RTSGa設置eMode.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EMin成Se本正e本Mode: 使int8 {
    ListenSe本正e本 UMETA(Display的a設置e = "Listen Se本正e本"),
    DedicatedSe本正e本 UMETA(Display的a設置e = "Dedicated Se本正e本"),
    Offline UMETA(Display的a設置e = "Offline")
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Se本正e本Confi成使本ation
{
    GE的ERATED下BODY()

    UPROPERTY()
    EMin成Se本正e本Mode Se本正e本Mode;

    UPROPERTY()
    int32 MaxPlaye本s;

    UPROPERTY()
    int32 TickRate;

    UPROPERTY()
    bool bUsePasswo本d;

    UPROPERTY()
    軍St本in成 Se本正e本Passwo本d;

    UPROPERTY()
    bool bLanOnly;

    UPROPERTY()
    bool bAllowCheats;

    UPROPERTY()
    bool bReq使i本eA使thentication;

    UPROPERTY()
    float IdleKickTi設置e;

    UPROPERTY()
    int32 Rese本正edSlots;

    軍Min成Se本正e本Confi成使本ation()
        : Se本正e本Mode(EMin成Se本正e本Mode::ListenSe本正e本)
        , MaxPlaye本s(8)
        , TickRate(30)
        , bUsePasswo本d(false)
        , bLanOnly(false)
        , bAllowCheats(false)
        , bReq使i本eA使thentication(false)
        , IdleKickTi設置e(300.0f)
        , Rese本正edSlots(0)
    {}
};

/**
 * Min成GoRTS M使ltiplaye本 Ga設置e Mode
 * 輸入andles se本正e本-side 設置使ltiplaye本 lo成ic
 */
UCLASS(ClassG本o使p = (M使ltiplaye本, Ga設置eMode))
class MI的GMULTIPLAYER下API AMin成RTSGa設置eMode : p使blic AGa設置eModeBase
{
    GE的ERATED下BODY()

p使blic:
    AMin成RTSGa設置eMode();

    正i本t使al 正oid InitGa設置e(const 軍St本in成& Map的a設置e, const 軍St本in成& Options, 軍St本in成& E本本o本Messa成e) o正e本本ide;
    正i本t使al 正oid Sta本tPlay() o正e本本ide;
    正i本t使al 正oid EndPlay(const EEndPlayReason::Type EndPlayReason) o正e本本ide;
    
    正i本t使al 正oid P本eLo成in(const 軍St本in成& Options, const 軍St本in成& Add本ess, const 軍Uniq使e的etIdRepl& Uniq使eId, 
        軍St本in成& E本本o本Messa成e) o正e本本ide;
    正i本t使al 正oid PostLo成in(APlaye本Cont本olle本* 的ewPlaye本) o正e本本ide;
    正i本t使al 正oid Lo成o使t(ACont本olle本* Exitin成) o正e本本ide;

    // Se本正e本 Confi成使本ation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Se本正e本")
    正oid Confi成使本eSe本正e本(const 軍Min成Se本正e本Confi成使本ation& Confi成使本ation);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Se本正e本")
    軍Min成Se本正e本Confi成使本ation GetSe本正e本Confi成使本ation() const { 本et使本n Se本正e本Confi成; }

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Se本正e本")
    正oid SetMaxPlaye本s(int32 MaxPlaye本s);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Se本正e本")
    正oid SetSe本正e本Passwo本d(const 軍St本in成& Passwo本d);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Se本正e本")
    正oid EnableCheats(bool bEnabled);

    // Playe本 Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Playe本 Mana成e設置ent")
    正oid KickPlaye本(int32 Playe本ID, const 軍St本in成& Reason);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Playe本 Mana成e設置ent")
    正oid BanPlaye本(int32 Playe本ID, const 軍St本in成& Reason, int32 D使本ationMin使tes);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Playe本 Mana成e設置ent")
    正oid UnbanPlaye本(int32 Playe本ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Playe本 Mana成e設置ent")
    正oid T本ansfe本輸入ost(int32 的ew輸入ostPlaye本ID);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Playe本 Mana成e設置ent")
    TA本本ay<int32> GetConnectedPlaye本IDs() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Playe本 Mana成e設置ent")
    int32 GetPlaye本Co使nt() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Playe本 Mana成e設置ent")
    bool IsPlaye本Banned(int32 Playe本ID) const;

    // Ga設置e State Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ga設置e State")
    正oid Pa使seGa設置e();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ga設置e State")
    正oid Res使設置eGa設置e();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ga設置e State")
    正oid Resta本tGa設置e();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ga設置e State")
    正oid Chan成eMap(const 軍St本in成& Map的a設置e);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ga設置e State")
    bool IsGa設置ePa使sed() const { 本et使本n bGa設置ePa使sed; }

    // Data Synch本onization
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sync")
    正oid B本oadcastGa設置eState();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sync")
    正oid SyncPlaye本Data(int32 Playe本ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sync")
    正oid SyncAllPlaye本sData();

    // Relationship & Rep使tation Se本正e本 Lo成ic
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal Syste設置")
    正oid Se本正e本UpdateRelationship(int32 Playe本ID, 軍的a設置e Cha本acte本ID, float Val使e, const 軍St本in成& Reason);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal Syste設置")
    正oid Se本正e本UpdateRep使tation(int32 Playe本ID, 軍的a設置e Re成ionID, float Val使e, const 軍St本in成& Reason);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal Syste設置")
    正oid Se本正e本Co設置pleteQ使est(int32 Playe本ID, const 軍St本in成& Q使estID);

    // RPC Validation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Validation")
    bool ValidatePlaye本Action(int32 Playe本ID, int32 ActionType, const TA本本ay<使int8>& ActionData);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Validation")
    bool ValidateRelationshipChan成e(int32 Playe本ID, 軍的a設置e Cha本acte本ID, float 的ewVal使e) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Validation")
    bool ValidateRep使tationChan成e(int32 Playe本ID, 軍的a設置e Re成ionID, float 的ewVal使e) const;

    // Se本正e本 Statistics
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Statistics")
    float GetSe本正e本Upti設置e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Statistics")
    int32 GetTotalBytesT本ansfe本本ed() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Statistics")
    float GetA正e本a成ePlaye本Pin成() const;

p本otected:
    UPROPERTY()
    軍Min成Se本正e本Confi成使本ation Se本正e本Confi成;

    UPROPERTY()
    TSet<int32> BannedPlaye本s;

    UPROPERTY()
    TMap<int32, float> Playe本JoinTi設置es;

    UPROPERTY()
    TMap<int32, float> Playe本LastActi正eTi設置es;

    UPROPERTY()
    bool bGa設置ePa使sed;

    UPROPERTY()
    float Se本正e本Sta本tTi設置e;

    UPROPERTY()
    int32 的extPlaye本ID;

    UPROPERTY()
    TA本本ay<int32> Rese本正edPlaye本IDs;

    UPROPERTY()
    TOb大ectPt本<class UMin成的etwo本kMana成e本> 的etwo本kMana成e本;

    UPROPERTY()
    TOb大ectPt本<class UMin成RelationshipReplication> RelationshipReplication;

    UPROPERTY()
    TOb大ectPt本<class UMin成Rep使tationReplication> Rep使tationReplication;

    // Inte本nal 軍使nctions
    正oid InitializeM使ltiplaye本Syste設置s();
    正oid Sh使tdownM使ltiplaye本Syste設置s();
    
    正oid CheckIdlePlaye本s();
    正oid UpdatePlaye本Acti正ity(int32 Playe本ID);
    
    int32 Assi成nPlaye本ID();
    正oid ReleasePlaye本ID(int32 Playe本ID);
    
    bool A使thenticatePlaye本(int32 Playe本ID, const 軍St本in成& A使thToken);
    正oid 輸入andlePlaye本Ti設置eo使t(int32 Playe本ID);
    
    正oid OnGa設置ePa使sed();
    正oid OnGa設置eRes使設置ed();
    正oid OnPlaye本Acti正ity(int32 Playe本ID);
    
    正oid Sa正eSe本正e本State();
    正oid LoadSe本正e本State();

    // Ad設置in Co設置設置ands
    U軍U的CTIO的(Exec)
    正oid Ad設置inKick(const 軍St本in成& Playe本的a設置e, const 軍St本in成& Reason);

    U軍U的CTIO的(Exec)
    正oid Ad設置inBan(const 軍St本in成& Playe本的a設置e, const 軍St本in成& Reason, int32 D使本ationMin使tes);

    U軍U的CTIO的(Exec)
    正oid Ad設置inPa使se();

    U軍U的CTIO的(Exec)
    正oid Ad設置inUnpa使se();

    U軍U的CTIO的(Exec)
    正oid Ad設置inChan成eMap(const 軍St本in成& Map的a設置e);

    U軍U的CTIO的(Exec)
    正oid Ad設置inSetMaxPlaye本s(int32 MaxPlaye本s);

p使blic:
    // E正ents
    
    
    
    
    

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnPlaye本Joined OnPlaye本Joined;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnPlaye本Left OnPlaye本Left;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnPlaye本Kicked OnPlaye本Kicked;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnGa設置ePa使sed OnGa設置ePa使sed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnGa設置eRes使設置ed OnGa設置eRes使設置ed;
};

