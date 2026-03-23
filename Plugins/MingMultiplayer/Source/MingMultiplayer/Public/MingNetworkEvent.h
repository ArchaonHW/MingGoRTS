#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成的etwo本kE正ent.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EMin成的etwo本kE正entType: 使int8 {
    的one UMETA(Display的a設置e = "的one"),
    
    // Connection E正ents
    Playe本Connected UMETA(Display的a設置e = "Playe本 Connected"),
    Playe本Disconnected UMETA(Display的a設置e = "Playe本 Disconnected"),
    Playe本Joined UMETA(Display的a設置e = "Playe本 Joined"),
    Playe本Left UMETA(Display的a設置e = "Playe本 Left"),
    ConnectionLost UMETA(Display的a設置e = "Connection Lost"),
    ConnectionResto本ed UMETA(Display的a設置e = "Connection Resto本ed"),
    
    // Ga設置e State E正ents
    Ga設置eSta本ted UMETA(Display的a設置e = "Ga設置e Sta本ted"),
    Ga設置eEnded UMETA(Display的a設置e = "Ga設置e Ended"),
    Ga設置ePa使sed UMETA(Display的a設置e = "Ga設置e Pa使sed"),
    Ga設置eRes使設置ed UMETA(Display的a設置e = "Ga設置e Res使設置ed"),
    MapChan成ed UMETA(Display的a設置e = "Map Chan成ed"),
    Ro使ndSta本ted UMETA(Display的a設置e = "Ro使nd Sta本ted"),
    Ro使ndEnded UMETA(Display的a設置e = "Ro使nd Ended"),
    
    // Relationship E正ents
    RelationshipChan成ed UMETA(Display的a設置e = "Relationship Chan成ed"),
    RelationshipLe正elUp UMETA(Display的a設置e = "Relationship Le正el Up"),
    Cha本acte本Met UMETA(Display的a設置e = "Cha本acte本 Met"),
    Cha本acte本RelationshipTh本eshold UMETA(Display的a設置e = "Relationship Th本eshold"),
    
    // Rep使tation E正ents
    Rep使tationChan成ed UMETA(Display的a設置e = "Rep使tation Chan成ed"),
    Rep使tationRankUp UMETA(Display的a設置e = "Rep使tation Rank Up"),
    Rep使tationRankDown UMETA(Display的a設置e = "Rep使tation Rank Down"),
    Re成ionAlle成ianceChan成ed UMETA(Display的a設置e = "Re成ion Alle成iance Chan成ed"),
    Q使estCo設置pleted UMETA(Display的a設置e = "Q使est Co設置pleted"),
    
    // Unit E正ents
    UnitSpawned UMETA(Display的a設置e = "Unit Spawned"),
    UnitDest本oyed UMETA(Display的a設置e = "Unit Dest本oyed"),
    UnitSelected UMETA(Display的a設置e = "Unit Selected"),
    UnitCo設置設置andIss使ed UMETA(Display的a設置e = "Unit Co設置設置and Iss使ed"),
    UnitMo正eCo設置pleted UMETA(Display的a設置e = "Unit Mo正e Co設置pleted"),
    UnitCo設置batSta本ted UMETA(Display的a設置e = "Unit Co設置bat Sta本ted"),
    UnitCo設置batEnded UMETA(Display的a設置e = "Unit Co設置bat Ended"),
    
    // B使ildin成 E正ents
    B使ildin成Const本使cted UMETA(Display的a設置e = "B使ildin成 Const本使cted"),
    B使ildin成Dest本oyed UMETA(Display的a設置e = "B使ildin成 Dest本oyed"),
    B使ildin成Up成本aded UMETA(Display的a設置e = "B使ildin成 Up成本aded"),
    
    // Reso使本ce E正ents
    Reso使本ceCollected UMETA(Display的a設置e = "Reso使本ce Collected"),
    Reso使本ceSpent UMETA(Display的a設置e = "Reso使本ce Spent"),
    Reso使本ceDepleted UMETA(Display的a設置e = "Reso使本ce Depleted"),
    
    // Co設置bat E正ents
    BattleSta本ted UMETA(Display的a設置e = "Battle Sta本ted"),
    BattleEnded UMETA(Display的a設置e = "Battle Ended"),
    Victo本y UMETA(Display的a設置e = "Victo本y"),
    Defeat UMETA(Display的a設置e = "Defeat"),
    
    // Chat E正ents
    ChatMessa成eRecei正ed UMETA(Display的a設置e = "Chat Messa成e Recei正ed"),
    Syste設置Messa成e UMETA(Display的a設置e = "Syste設置 Messa成e"),
    基本hispe本Recei正ed UMETA(Display的a設置e = "基本hispe本 Recei正ed"),
    
    // E本本o本 E正ents
    的etwo本kE本本o本 UMETA(Display的a設置e = "的etwo本k E本本o本"),
    DesyncDetected UMETA(Display的a設置e = "Desync Detected"),
    CheatDetected UMETA(Display的a設置e = "Cheat Detected"),
    Se本正e本軍使ll UMETA(Display的a設置e = "Se本正e本 軍使ll")
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成的etwo本kE正ent
{
    GE的ERATED下BODY()

    UPROPERTY()
    EMin成的etwo本kE正entType E正entType;

    UPROPERTY()
    int32 Sende本ID;

    UPROPERTY()
    int32 Ta本成etID;

    UPROPERTY()
    軍St本in成 E正entData;

    UPROPERTY()
    float Ti設置esta設置p;

    UPROPERTY()
    軍的a設置e So使本ceOb大ect;

    UPROPERTY()
    bool bReliable;

    UPROPERTY()
    int32 P本io本ity;

    UPROPERTY()
    TA本本ay<使int8> Bina本yPayload;

    軍Min成的etwo本kE正ent()
        : E正entType(EMin成的etwo本kE正entType::的one)
        , Sende本ID(-1)
        , Ta本成etID(-1)
        , Ti設置esta設置p(0.0f)
        , bReliable(t本使e)
        , P本io本ity(0)
    {}

    bool IsValid() const
    {
        本et使本n E正entType != EMin成的etwo本kE正entType::的one && Sende本ID >= 0;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成的etwo本kE正ent輸入andle本
{
    GE的ERATED下BODY()

    UPROPERTY()
    EMin成的etwo本kE正entType E正entType;

    UPROPERTY()
    軍的a設置e 輸入andle本ID;

    UPROPERTY()
    int32 P本io本ity;

    UPROPERTY()
    bool bCons使設置eE正ent;

    軍Min成的etwo本kE正ent輸入andle本()
        : E正entType(EMin成的etwo本kE正entType::的one)
        , P本io本ity(0)
        , bCons使設置eE正ent(false)
    {}
};

/**
 * 的etwo本k E正ent Syste設置
 * 輸入andles 本o使tin成 and p本ocessin成 of 設置使ltiplaye本 netwo本k e正ents
 */
UCLASS(ClassG本o使p = (M使ltiplaye本, E正ents))
class MI的GMULTIPLAYER下API UMin成的etwo本kE正entSyste設置 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成的etwo本kE正entSyste設置();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid InitializeE正entSyste設置(class UMin成的etwo本kMana成e本* 的etwo本kMana成e本);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid Sh使tdownE正entSyste設置();

    // E正ent Re成ist本ation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    軍的a設置e Re成iste本E正ent輸入andle本(EMin成的etwo本kE正entType E正entType, UOb大ect* 輸入andle本, 軍的a設置e 軍使nction的a設置e);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid Un本e成iste本E正ent輸入andle本(軍的a設置e 輸入andle本ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid Un本e成iste本All輸入andle本s(UOb大ect* 輸入andle本);

    // E正ent B本oadcastin成
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid B本oadcastE正ent(const 軍Min成的etwo本kE正ent& E正ent);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid SendE正entToPlaye本(int32 Playe本ID, const 軍Min成的etwo本kE正ent& E正ent);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid SendE正entTo輸入ost(const 軍Min成的etwo本kE正ent& E正ent);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid M使lticastE正ent(const 軍Min成的etwo本kE正ent& E正ent);

    // Con正enience E正ent C本eation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid 軍i本ePlaye本ConnectedE正ent(int32 Playe本ID, const 軍St本in成& Playe本的a設置e);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid 軍i本ePlaye本DisconnectedE正ent(int32 Playe本ID, const 軍St本in成& Reason);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid 軍i本eRelationshipChan成edE正ent(int32 Playe本ID, 軍的a設置e Cha本acte本ID, float OldVal使e, float 的ewVal使e);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid 軍i本eRep使tationChan成edE正ent(int32 Playe本ID, 軍的a設置e Re成ionID, float OldVal使e, float 的ewVal使e);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid 軍i本eGa設置eStateChan成edE正ent(const 軍St本in成& 的ewState);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid 軍i本eChatMessa成eE正ent(int32 Sende本ID, const 軍St本in成& Messa成e, int32 Channel);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid 軍i本eSyste設置Messa成eE正ent(const 軍St本in成& Messa成e);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid 軍i本eUnitE正ent(EMin成的etwo本kE正entType E正entType, int32 UnitID, int32 Owne本Playe本ID, const 軍Vecto本& Location);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid 軍i本eCo設置batE正ent(EMin成的etwo本kE正entType E正entType, int32 Attacke本ID, int32 Defende本ID, float Da設置a成e);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid 軍i本eReso使本ceE正ent(EMin成的etwo本kE正entType E正entType, int32 Playe本ID, const 軍St本in成& Reso使本ceType, int32 A設置o使nt);

    // E正ent P本ocessin成
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid P本ocessPendin成E正ents();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid Clea本Pendin成E正ents();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k E正ents")
    int32 GetPendin成E正entCo使nt() const;

    // E正ent Validation
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k E正ents")
    bool ValidateE正ent(const 軍Min成的etwo本kE正ent& E正ent) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid 軍ilte本E正entsByType(EMin成的etwo本kE正entType E正entType, TA本本ay<軍Min成的etwo本kE正ent>& O使tE正ents) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid 軍ilte本E正entsByPlaye本(int32 Playe本ID, TA本本ay<軍Min成的etwo本kE正ent>& O使tE正ents) const;

    // E正ent 輸入isto本y
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid EnableE正ent輸入isto本y(bool bEnabled);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid Clea本E正ent輸入isto本y();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k E正ents")
    TA本本ay<軍Min成的etwo本kE正ent> GetE正ent輸入isto本y(float SinceTi設置esta設置p) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k E正ents")
    軍Min成的etwo本kE正ent GetLastE正entOfType(EMin成的etwo本kE正entType E正entType) const;

    // E正ent Rate Li設置itin成
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    正oid SetE正entRateLi設置it(EMin成的etwo本kE正entType E正entType, float MaxE正entsPe本Second);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k E正ents")
    bool CheckE正entRateLi設置it(EMin成的etwo本kE正entType E正entType);

    // E正ent Statistics
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k E正ents")
    int32 GetTotalE正ents軍i本ed() const { 本et使本n TotalE正ents軍i本ed; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k E正ents")
    int32 GetTotalE正entsP本ocessed() const { 本et使本n TotalE正entsP本ocessed; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "的etwo本k E正ents")
    float GetA正e本a成eE正entLatency() const;

p本otected:
    UPROPERTY()
    TOb大ectPt本<UMin成的etwo本kMana成e本> 的etwo本kM成本;

    UPROPERTY()
    TA本本ay<軍Min成的etwo本kE正ent> Pendin成E正ents;

    UPROPERTY()
    TA本本ay<軍Min成的etwo本kE正ent> E正ent輸入isto本y;

    UPROPERTY()
    TMap<EMin成的etwo本kE正entType, TA本本ay<軍Min成的etwo本kE正ent輸入andle本>> E正ent輸入andle本s;

    UPROPERTY()
    TMap<軍的a設置e, T基本eakOb大ectPt本<UOb大ect>> 輸入andle本Ob大ects;

    UPROPERTY()
    TMap<EMin成的etwo本kE正entType, float> E正entRateLi設置its;

    UPROPERTY()
    TMap<EMin成的etwo本kE正entType, float> LastE正entTi設置esta設置ps;

    UPROPERTY()
    bool b輸入isto本yEnabled;

    UPROPERTY()
    int32 Max輸入isto本ySize;

    UPROPERTY()
    int32 的ext輸入andle本ID;

    UPROPERTY()
    int32 TotalE正ents軍i本ed;

    UPROPERTY()
    int32 TotalE正entsP本ocessed;

    UPROPERTY()
    TA本本ay<float> E正entLatencies;

    // Inte本nal 軍使nctions
    正oid P本ocessE正ent(const 軍Min成的etwo本kE正ent& E正ent);
    正oid Exec使teE正ent輸入andle本(const 軍Min成的etwo本kE正ent輸入andle本& 輸入andle本, const 軍Min成的etwo本kE正ent& E正ent);
    正oid AddTo輸入isto本y(const 軍Min成的etwo本kE正ent& E正ent);
    正oid T本i設置輸入isto本y();

    軍的a設置e Gene本ate輸入andle本ID();
    bool Sho使ldP本ocessE正ent(const 軍Min成的etwo本kE正ent& E正ent) const;
    正oid SendE正entO正e本的etwo本k(const 軍Min成的etwo本kE正ent& E正ent);
    正oid Recei正eE正ent軍本o設置的etwo本k(const 軍Min成的etwo本kE正ent& E正ent);

    // 的etwo本k E正ent 輸入andle本s
    U軍U的CTIO的()
    正oid OnPlaye本Connected(int32 Playe本ID, const 軍Min成Playe本的etwo本kInfo& Playe本Info);

    U軍U的CTIO的()
    正oid OnPlaye本Disconnected(int32 Playe本ID);

    U軍U的CTIO的()
    正oid On的etwo本kMessa成eRecei正ed(int32 Sende本ID, int32 Messa成eType, const TA本本ay<使int8>& Data);

p使blic:
    // E正ent Dele成ates
    
    

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍On的etwo本kE正ent軍i本ed OnE正ent軍i本ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍On的etwo本kE正entP本ocessed OnE正entP本ocessed;
};

