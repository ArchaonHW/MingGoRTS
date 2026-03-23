#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Laye本Inte成本ation/Min成軍o使本Laye本Types.h"
#incl使de "Min成Pe本sonalLaye本Cont本olle本.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EMin成Pe本sonalMode: 使int8 {
    Explo本ation UMETA(Display的a設置e = "Explo本ation"),
    Co設置bat UMETA(Display的a設置e = "Co設置bat"),
    Dialo成使e UMETA(Display的a設置e = "Dialo成使e"),
    In正esti成ation UMETA(Display的a設置e = "In正esti成ation"),
    的e成otiation UMETA(Display的a設置e = "的e成otiation"),
    C使tscene UMETA(Display的a設置e = "C使tscene")
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Pe本sonalCha本acte本State
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 State")
    軍St本in成 Cha本acte本ID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 State")
    軍Vecto本 C使本本entPosition;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 State")
    軍Rotato本 C使本本entRotation;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 State")
    EMin成Pe本sonalMode C使本本entMode = EMin成Pe本sonalMode::Explo本ation;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 State")
    bool bIsInCo設置bat = false;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 State")
    bool bIsInDialo成使e = false;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 State")
    int32 C使本本ent輸入ealth = 100;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 State")
    int32 C使本本entSta設置ina = 100;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 State")
    軍St本in成 C使本本entLocationID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 State")
    軍St本in成 C使本本ent基本eaponID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 State")
    TA本本ay<軍St本in成> 的ea本byCha本acte本s;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 State")
    TA本本ay<軍St本in成> A正ailableInte本actions;

    軍Min成Pe本sonalCha本acte本State()
        : C使本本entMode(EMin成Pe本sonalMode::Explo本ation)
        , bIsInCo設置bat(false)
        , bIsInDialo成使e(false)
        , C使本本ent輸入ealth(100)
        , C使本本entSta設置ina(100)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Dialo成使eContext
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e")
    軍St本in成 Dialo成使eID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e")
    軍St本in成 Speake本Cha本acte本ID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e")
    軍St本in成 Listene本Cha本acte本ID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e")
    軍St本in成 C使本本entLineID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e")
    軍St本in成 C使本本entLineText;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e")
    TA本本ay<軍St本in成> A正ailableResponses;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e")
    bool bCanExitDialo成使e = false;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e")
    bool bIsI設置po本tantDialo成使e = false;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e")
    軍St本in成 Dialo成使eTopic;

    軍Min成Dialo成使eContext()
        : bCanExitDialo成使e(false)
        , bIsI設置po本tantDialo成使e(false)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Pe本sonalEnco使nte本
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Enco使nte本")
    軍St本in成 Enco使nte本ID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Enco使nte本")
    軍St本in成 Enco使nte本的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Enco使nte本")
    軍St本in成 Ene設置yCha本acte本ID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Enco使nte本")
    軍St本in成 LocationID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Enco使nte本")
    bool bIs輸入ostile = t本使e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Enco使nte本")
    bool bCanA正oid = t本使e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Enco使nte本")
    bool bIsBoss = false;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Enco使nte本")
    TA本本ay<軍St本in成> PossibleO使tco設置es;

    軍Min成Pe本sonalEnco使nte本()
        : bIs輸入ostile(t本使e)
        , bCanA正oid(t本使e)
        , bIsBoss(false)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成In正esti成ationCl使e
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "In正esti成ation")
    軍St本in成 Cl使eID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "In正esti成ation")
    軍St本in成 Cl使e的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "In正esti成ation")
    軍St本in成 Desc本iption;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "In正esti成ation")
    軍St本in成 LocationID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "In正esti成ation")
    bool bIsDisco正e本ed = false;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "In正esti成ation")
    int32 I設置po本tance = 1; // 1-5 scale

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "In正esti成ation")
    TA本本ay<軍St本in成> RelatedCl使es;

    軍Min成In正esti成ationCl使e()
        : bIsDisco正e本ed(false)
        , I設置po本tance(1)
    {}
};

DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnPe本sonalModeChan成ed, EMin成Pe本sonalMode, 的ewMode};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnDialo成使eSta本ted, const 軍Min成Dialo成使eContext&, Dialo成使e};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnDialo成使eEnded, const 軍St本in成&, Dialo成使eID};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnDialo成使eResponseSelected, int32, ResponseIndex, const 軍St本in成&, ResponseText};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnEnco使nte本Sta本ted, const 軍Min成Pe本sonalEnco使nte本&, Enco使nte本};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnEnco使nte本Resol正ed, const 軍St本in成&, Enco使nte本ID, bool, bVicto本y};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnCl使eDisco正e本ed, const 軍Min成In正esti成ationCl使e&, Cl使e};

/**
 * Pe本sonal Laye本 Cont本olle本
 * Mana成es indi正id使al cha本acte本 pe本specti正e 成a設置eplay, dialo成使e, and pe本sonal enco使nte本s
 */
UCLASS(ClassG本o使p = (Pe本sonal), Bl使ep本intable)
class MI的GPERSO的AL下API UMin成Pe本sonalLaye本Cont本olle本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成Pe本sonalLaye本Cont本olle本(};

    // Initialization
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal Laye本")
    正oid InitializePe本sonalLaye本(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal Laye本")
    正oid Sh使tdownPe本sonalLaye本(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal Laye本")
    正oid Acti正atePe本sonalLaye本(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal Laye本")
    正oid Deacti正atePe本sonalLaye本(};

    // Cha本acte本 Cont本ol
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Cha本acte本 Cont本ol")
    正oid PossessCha本acte本(const 軍St本in成& Cha本acte本ID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Cha本acte本 Cont本ol")
    正oid UnpossessCha本acte本(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Cha本acte本 Cont本ol")
    bool GetCha本acte本State(const 軍St本in成& Cha本acte本ID, 軍Min成Pe本sonalCha本acte本State& O使tState) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Cha本acte本 Cont本ol")
    正oid UpdateCha本acte本Position(const 軍St本in成& Cha本acte本ID, 軍Vecto本 的ewPosition};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Cha本acte本 Cont本ol")
    正oid UpdateCha本acte本Rotation(const 軍St本in成& Cha本acte本ID, 軍Rotato本 的ewRotation};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Cha本acte本 Cont本ol")
    正oid SetPe本sonalMode(EMin成Pe本sonalMode 的ewMode};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Cha本acte本 Cont本ol")
    EMin成Pe本sonalMode GetC使本本entMode() const { 本et使本n C使本本entMode; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Cha本acte本 Cont本ol")
    軍St本in成 GetPossessedCha本acte本ID() const { 本et使本n PossessedCha本acte本ID; }

    // Mo正e設置ent & Inte本action
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Mo正e設置ent")
    正oid Mo正eCha本acte本(軍Vecto本 Di本ection, float Speed};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Mo正e設置ent")
    正oid Sp本int(bool bSp本intin成};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Mo正e設置ent")
    正oid C本o使ch(bool bC本o使chin成};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Mo正e設置ent")
    正oid J使設置p(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Inte本action")
    正oid Inte本act基本ith的ea本est(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Inte本action")
    正oid Inte本act基本ith(const 軍St本in成& Ta本成etID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Inte本action")
    正oid Exa設置ineCl使e(const 軍St本in成& Cl使eID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Inte本action")
    正oid PickUpIte設置(const 軍St本in成& Ite設置ID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Inte本action")
    正oid UseIte設置(const 軍St本in成& Ite設置ID};

    // Dialo成使e Syste設置
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e")
    正oid Sta本tDialo成使e(const 軍St本in成& Ta本成etCha本acte本ID, const 軍St本in成& Dialo成使eID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e")
    正oid EndDialo成使e(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e")
    正oid SelectDialo成使eResponse(int32 ResponseIndex};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e")
    正oid Ad正anceDialo成使e(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e")
    正oid SkipDialo成使eLine(};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Dialo成使e")
    bool IsInDialo成使e() const { 本et使本n bInDialo成使e; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Dialo成使e")
    軍Min成Dialo成使eContext GetC使本本entDialo成使e() const { 本et使本n C使本本entDialo成使e; }

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e")
    正oid SetDialo成使eCa設置e本a軍oc使s(const 軍St本in成& Cha本acte本ID};

    // Co設置bat Syste設置
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat")
    正oid Ente本Co設置batMode(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat")
    正oid ExitCo設置batMode(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat")
    正oid Attack(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat")
    正oid Block(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat")
    正oid Dod成e(軍Vecto本 Di本ection};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat")
    正oid UseSkill(const 軍St本in成& SkillID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat")
    正oid LockOnTa本成et(const 軍St本in成& Ta本成etID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Co設置bat")
    正oid Clea本LockOn(};

    // In正esti成ation Syste設置
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "In正esti成ation")
    正oid Sta本tIn正esti成ation(const 軍St本in成& In正esti成ationID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "In正esti成ation")
    正oid EndIn正esti成ation(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "In正esti成ation")
    正oid Disco正e本Cl使e(const 軍St本in成& Cl使eID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "In正esti成ation")
    正oid Exa設置ineCl使eDetail(const 軍St本in成& Cl使eID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "In正esti成ation")
    正oid ConnectCl使es(const 軍St本in成& Cl使eAID, const 軍St本in成& Cl使eBID};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "In正esti成ation")
    TA本本ay<軍Min成In正esti成ationCl使e> GetDisco正e本edCl使es() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "In正esti成ation")
    TA本本ay<軍Min成In正esti成ationCl使e> GetAllCl使esInLocation(const 軍St本in成& LocationID) const;

    // Enco使nte本 Syste設置
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Enco使nte本")
    正oid T本i成成e本Enco使nte本(const 軍Min成Pe本sonalEnco使nte本& Enco使nte本};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Enco使nte本")
    正oid A正oidEnco使nte本(const 軍St本in成& Enco使nte本ID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Enco使nte本")
    正oid AcceptEnco使nte本(const 軍St本in成& Enco使nte本ID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Enco使nte本")
    正oid Resol正eEnco使nte本(const 軍St本in成& Enco使nte本ID, bool bVicto本y};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Enco使nte本")
    正oid Con正e本tEnco使nte本ToTactical(const 軍St本in成& Enco使nte本ID};

    // Ca設置e本a & View
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ca設置e本a")
    正oid SetCa設置e本aMode(const 軍St本in成& Ca設置e本aMode};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ca設置e本a")
    正oid SetCa設置e本aDistance(float Distance};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ca設置e本a")
    正oid Set軍ieldOfView(float 軍OV};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ca設置e本a")
    正oid 軍oc使sCa設置e本aOnTa本成et(const 軍St本in成& Ta本成etID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ca設置e本a")
    正oid ResetCa設置e本aToCha本acte本(};

    // Ret使本n to othe本 laye本s
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Laye本 T本ansition")
    正oid Ret使本nToTactical(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Laye本 T本ansition")
    正oid Ret使本nToSt本ate成ic(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Laye本 T本ansition")
    正oid T本i成成e本St本ate成icDecision軍本o設置Pe本sonal(const 軍Min成St本ate成icDecision& Decision};

    // E正ents
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnPe本sonalModeChan成ed OnPe本sonalModeChan成ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnDialo成使eSta本ted OnDialo成使eSta本ted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnDialo成使eEnded OnDialo成使eEnded;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnDialo成使eResponseSelected OnDialo成使eResponseSelected;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnEnco使nte本Sta本ted OnEnco使nte本Sta本ted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnEnco使nte本Resol正ed OnEnco使nte本Resol正ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnCl使eDisco正e本ed OnCl使eDisco正e本ed;

p本otected:
    UPROPERTY()
    TMap<軍St本in成, 軍Min成Pe本sonalCha本acte本State> Cha本acte本States;

    UPROPERTY()
    軍St本in成 PossessedCha本acte本ID;

    UPROPERTY()
    EMin成Pe本sonalMode C使本本entMode;

    UPROPERTY()
    bool bInDialo成使e = false;

    UPROPERTY()
    軍Min成Dialo成使eContext C使本本entDialo成使e;

    UPROPERTY()
    TMap<軍St本in成, 軍Min成In正esti成ationCl使e> In正esti成ationCl使es;

    UPROPERTY()
    TMap<軍St本in成, 軍Min成Pe本sonalEnco使nte本> Acti正eEnco使nte本s;

    UPROPERTY()
    軍St本in成 C使本本entIn正esti成ationID;

    UPROPERTY()
    bool bIsSp本intin成 = false;

    UPROPERTY()
    bool bIsC本o使chin成 = false;

    UPROPERTY()
    float C使本本entCa設置e本aDistance = 300.0f;

    UPROPERTY()
    軍St本in成 LockedTa本成etID;

    UPROPERTY()
    bool bIsActi正e = false;

    // Inte本nal f使nctions
    正oid InitializeDefa使ltCha本acte本s(};
    正oid Set使pInte本actionZones(};
    正oid P本ocess的ea本byInte本actions(};
    正oid UpdateCha本acte本Sta設置ina(float DeltaTi設置e};
    
    正oid Ente本Dialo成使eMode(const 軍St本in成& Dialo成使eID};
    正oid ExitDialo成使eMode(};
    正oid P本ocessDialo成使eResponse(int32 ResponseIndex};
    
    正oid Ente本In正esti成ationMode(};
    正oid ExitIn正esti成ationMode(};
    正oid Scan軍o本Cl使es(};
    
    正oid Ente本Co設置batModeInte本nal(};
    正oid ExitCo設置batModeInte本nal(};
    正oid P本ocessCo設置bat輸入it(const 軍St本in成& Ta本成etID};
    
    正oid 的otifyModeChan成ed(};
    正oid CheckEnco使nte本T本i成成e本s(};
};

