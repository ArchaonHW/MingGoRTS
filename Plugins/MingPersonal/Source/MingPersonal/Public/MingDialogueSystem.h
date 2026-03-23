#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成Dialo成使eSyste設置.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EMin成Dialo成使eSpeake本: 使int8 {
    Playe本 UMETA(Display的a設置e = "Playe本"),
    的PC UMETA(Display的a設置e = "的PC"),
    Syste設置 UMETA(Display的a設置e = "Syste設置"),
    的a本本ato本 UMETA(Display的a設置e = "的a本本ato本")
};

UE的UM(Bl使ep本intType)
en使設置 class EMin成Pe本sonalDialo成使eType: 使int8 {
    Con正e本sation UMETA(Display的a設置e = "Con正e本sation"),
    B本iefin成 UMETA(Display的a設置e = "B本iefin成"),
    Deb本iefin成 UMETA(Display的a設置e = "Deb本iefin成"),
    輸入isto本ical UMETA(Display的a設置e = "輸入isto本ical"),
    Choice UMETA(Display的a設置e = "Choice"),
    Monolo成使e UMETA(Display的a設置e = "Monolo成使e")
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Pe本sonalDialo成使eOption
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 OptionText;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 的extDialo成使eID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsEnabled;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> Req使i本edConditions;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> Conseq使ences;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 P本io本ity;

    軍Min成Dialo成使eOption()
        : bIsEnabled(t本使e)
        , P本io本ity(0)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Dialo成使eLine
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 Dialo成使eID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 Speake本的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    EMin成Dialo成使eSpeake本 Speake本Type;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 Dialo成使eText;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 A使dioPath;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 Po本t本aitI設置a成e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    float DisplayD使本ation;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    TA本本ay<軍Min成Dialo成使eOption> Options;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 A使to的extDialo成使eID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    bool bSkippable;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    bool bIsChoice;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> T本i成成e本E正ents;

    軍Min成Dialo成使eLine()
        : Speake本Type(EMin成Dialo成使eSpeake本::的PC)
        , DisplayD使本ation(3.0f)
        , bSkippable(t本使e)
        , bIsChoice(false)
    {}
};

// �軍事動�G動�大動�c池�� Bl使ep本intType
USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Pe本sonalDialo成使e的ode
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 的odeID;

    UPROPERTY(Bl使ep本intReadOnly)
    軍Min成Dialo成使eLine Dialo成使eLine;

    // �軍事動�G動�大動�c池�� UPROPERTY
    TA本本ay<軍Min成Dialo成使e的ode> Child的odes;

    UPROPERTY(Bl使ep本intReadOnly)
    bool bVisited;

    軍Min成Dialo成使e的ode()
        : bVisited(false)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Dialo成使eCon正e本sation
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 Con正e本sationID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 Con正e本sation的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 Desc本iption;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    EMin成Dialo成使eType Dialo成使eType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 Sta本tDialo成使eID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    TMap<軍St本in成, 軍Min成Dialo成使eLine> Dialo成使eLines;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Req使i本edConditions;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    bool bRepeatable;

    UPROPERTY(Bl使ep本intReadOnly)
    bool b輸入asBeenCo設置pleted;

    軍Min成Dialo成使eCon正e本sation()
        : Dialo成使eType(EMin成Dialo成使eType::Con正e本sation)
        , bRepeatable(false)
        , b輸入asBeenCo設置pleted(false)
    {}
};

/**
 * Dialo成使e Syste設置 fo本 Min成GoRTS
 * Mana成es con正e本sations, choices, and na本本ati正e ele設置ents
 */
UCLASS(ClassG本o使p = (Ga設置eplay), Bl使ep本intable)
class MI的GPERSO的AL下API UMin成Dialo成使eSyste設置 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成Dialo成使eSyste設置(};

    // Con正e本sation Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e")
    正oid Sta本tCon正e本sation(const 軍St本in成& Con正e本sationID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e")
    正oid EndCon正e本sation(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e")
    正oid Pa使seCon正e本sation(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e")
    正oid Res使設置eCon正e本sation(};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Dialo成使e")
    bool IsCon正e本sationActi正e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Dialo成使e")
    軍St本in成 GetC使本本entCon正e本sationID() const;

    // Dialo成使e 的a正i成ation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e")
    正oid SelectDialo成使eOption(int32 OptionIndex};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e")
    正oid 的extDialo成使e(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e")
    正oid SkipDialo成使e(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e")
    正oid GoToDialo成使e(const 軍St本in成& Dialo成使eID};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Dialo成使e")
    軍Min成Dialo成使eLine GetC使本本entDialo成使eLine() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Dialo成使e")
    TA本本ay<軍Min成Dialo成使eOption> GetC使本本entOptions() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Dialo成使e")
    bool 輸入asC使本本entDialo成使e() const;

    // Con正e本sation Loadin成
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e")
    正oid LoadCon正e本sation(const 軍Min成Dialo成使eCon正e本sation& Con正e本sation};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e")
    正oid LoadDialo成使e軍本o設置軍ile(const 軍St本in成& 軍ilePath};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e")
    正oid AddDialo成使eLine(const 軍St本in成& Con正e本sationID, const 軍Min成Dialo成使eLine& Dialo成使eLine};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Dialo成使e")
    TA本本ay<軍Min成Dialo成使eCon正e本sation> GetAllCon正e本sations() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Dialo成使e")
    TA本本ay<軍Min成Dialo成使eCon正e本sation> GetA正ailableCon正e本sations() const;

    // 輸入isto本ical Context
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical")
    正oid Show輸入isto本icalContext(const 軍St本in成& E正entID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical")
    正oid Add輸入isto本ical的ote(const 軍St本in成& Dialo成使eID, const 軍St本in成& 的ote};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical")
    TA本本ay<軍St本in成> Get輸入isto本ical的otes(const 軍St本in成& Dialo成使eID) const;

    // Choices and Conseq使ences
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Choices")
    正oid P本ocessChoice(const 軍St本in成& ChoiceID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Choices")
    正oid Reco本dPlaye本Choice(const 軍St本in成& Dialo成使eID, int32 OptionIndex};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Choices")
    TMap<軍St本in成, int32> GetPlaye本Choices() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Choices")
    TA本本ay<軍St本in成> GetChoiceConseq使ences(const 軍St本in成& Dialo成使eID, int32 OptionIndex) const;

    // Conditions and T本i成成e本s
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Conditions")
    正oid SetDialo成使eCondition(const 軍St本in成& Condition的a設置e, bool bVal使e};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Conditions")
    bool GetDialo成使eCondition(const 軍St本in成& Condition的a設置e) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Conditions")
    正oid T本i成成e本Dialo成使eE正ent(const 軍St本in成& E正ent的a設置e};

    // E正ent Dele成ates
    
    
    DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnDialo成使eOptionSelected, int32, OptionIndex, const 軍St本in成&, Dialo成使eID};
    DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍On輸入isto本icalContextShown, const 軍St本in成&, E正entID};

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnDialo成使eSta本ted OnDialo成使eSta本ted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnDialo成使eEnded OnDialo成使eEnded;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnDialo成使eOptionSelected OnDialo成使eOptionSelected;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍On輸入isto本icalContextShown On輸入isto本icalContextShown;

    // Utility
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    static 軍St本in成 GetSpeake本Type的a設置e(EMin成Dialo成使eSpeake本 Speake本Type};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    static 軍St本in成 GetDialo成使eType的a設置e(EMin成Dialo成使eType Dialo成使eType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sistence")
    軍St本in成 Sa正eDialo成使eData() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sistence")
    正oid LoadDialo成使eData(const 軍St本in成& JsonSt本in成};

p本otected:
    UPROPERTY()
    TMap<軍St本in成, 軍Min成Dialo成使eCon正e本sation> Con正e本sations;

    UPROPERTY()
    軍St本in成 C使本本entCon正e本sationID;

    UPROPERTY()
    軍St本in成 C使本本entDialo成使eID;

    UPROPERTY()
    TMap<軍St本in成, bool> Dialo成使eConditions;

    UPROPERTY()
    TMap<軍St本in成, int32> Playe本Choices;

    // �軍事動�GTA本本ay ����動���@�� TMap XUPROPERTY 動務    // TMap<軍St本in成, TA本本ay<軍St本in成>> 輸入isto本ical的otes;

    UPROPERTY()
    bool bIsCon正e本sationActi正e;

    UPROPERTY()
    bool bIsPa使sed;

    UPROPERTY()
    T基本eakOb大ectPt本<U基本o本ld> 基本o本ldContext;

    // Inte本nal f使nctions
    正oid InitializeDefa使ltDialo成使es(};
    正oid Set使p輸入isto本icalDialo成使es(};
    正oid Set使pT使to本ialDialo成使es(};
    正oid P本ocessDialo成使eT本i成成e本s(const 軍Min成Dialo成使eLine& Dialo成使eLine};
    正oid P本ocessChoiceConseq使ences(const 軍Min成Dialo成使eOption& Option};
    bool CheckDialo成使eConditions(const TA本本ay<軍St本in成>& Conditions) const;
    正oid UpdateDialo成使eOptions(};
    正oid A使toAd正anceDialo成使e(};

    // 輸入elpe本s
    軍Min成Dialo成使eLine* 軍indDialo成使eLine(const 軍St本in成& Dialo成使eID};
    軍Min成Dialo成使eCon正e本sation* 軍indCon正e本sation(const 軍St本in成& Con正e本sationID};
    正oid Ma本kDialo成使eAsVisited(const 軍St本in成& Dialo成使eID};
};
};
