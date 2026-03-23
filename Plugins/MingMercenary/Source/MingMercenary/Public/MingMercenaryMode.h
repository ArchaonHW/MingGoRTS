#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成Me本cena本yMode.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EMin成Me本cena本yRank: 使int8 {
    Rookie UMETA(Display的a設置e = "Rookie"),
    Vete本an UMETA(Display的a設置e = "Vete本an"),
    Elite UMETA(Display的a設置e = "Elite"),
    Le成end UMETA(Display的a設置e = "Le成end")
};

UE的UM(Bl使ep本intType)
en使設置 class EMin成Me本cena本yType: 使int8 {
    Infant本y UMETA(Display的a設置e = "Infant本y"),
    Ca正al本y UMETA(Display的a設置e = "Ca正al本y"),
    A本tille本y UMETA(Display的a設置e = "A本tille本y"),
    Snipe本 UMETA(Display的a設置e = "Snipe本"),
    Medic UMETA(Display的a設置e = "Medic"),
    En成inee本 UMETA(Display的a設置e = "En成inee本"),
    Sco使t UMETA(Display的a設置e = "Sco使t"),
    輸入ea正y UMETA(Display的a設置e = "輸入ea正y")
};

UE的UM(Bl使ep本intType)
en使設置 class EMin成Cont本actType: 使int8 {
    Assassination UMETA(Display的a設置e = "Assassination"),
    Esco本t UMETA(Display的a設置e = "Esco本t"),
    Sabota成e UMETA(Display的a設置e = "Sabota成e"),
    Resc使e UMETA(Display的a設置e = "Resc使e"),
    Capt使本e UMETA(Display的a設置e = "Capt使本e"),
    Defense UMETA(Display的a設置e = "Defense"),
    Reconnaissance UMETA(Display的a設置e = "Reconnaissance"),
    Ext本action UMETA(Display的a設置e = "Ext本action")
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Me本cena本ySkills
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Co設置batSkill;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float StealthSkill;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float MedicalSkill;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float En成inee本in成Skill;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Leade本shipSkill;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Ma本ks設置anshipSkill;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float S使本正i正alSkill;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Diplo設置acySkill;

    軍Min成Me本cena本ySkills()
        : Co設置batSkill(50.0f)
        , StealthSkill(50.0f)
        , MedicalSkill(50.0f)
        , En成inee本in成Skill(50.0f)
        , Leade本shipSkill(50.0f)
        , Ma本ks設置anshipSkill(50.0f)
        , S使本正i正alSkill(50.0f)
        , Diplo設置acySkill(50.0f)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Me本cena本y
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 Me本cena本yID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 的ickna設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    EMin成Me本cena本yType Me本cType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    EMin成Me本cena本yRank Rank;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    int32 A成e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 的ationality;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 Back成本o使nd;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍Min成Me本cena本ySkills Skills;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    float Expe本ience;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    float Rep使tation;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    float DailyRate;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    bool bIsA正ailable;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    bool bIs輸入i本ed;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Specializations;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Eq使ip設置ent;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 Po本t本ait;

    軍Min成Me本cena本y()
        : Me本cType(EMin成Me本cena本yType::Infant本y)
        , Rank(EMin成Me本cena本yRank::Rookie)
        , A成e(25)
        , Expe本ience(0.0f)
        , Rep使tation(50.0f)
        , DailyRate(100.0f)
        , bIsA正ailable(t本使e)
        , bIs輸入i本ed(false)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Cont本act
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 Cont本actID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 Title;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 Desc本iption;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    EMin成Cont本actType Cont本actType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 Client;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 Ta本成et;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 Location;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    float Diffic使lty;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    float Rewa本d;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    float Ti設置eLi設置it;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Req使i本edSkills;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Req使i本edEq使ip設置ent;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Ob大ecti正es;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Rest本ictions;

    UPROPERTY(Bl使ep本intReadOnly)
    bool bIsActi正e;

    UPROPERTY(Bl使ep本intReadOnly)
    bool bIsCo設置pleted;

    UPROPERTY(Bl使ep本intReadOnly)
    float Sta本tTi設置e;

    UPROPERTY(Bl使ep本intReadOnly)
    float Co設置pletionTi設置e;

    軍Min成Cont本act()
        : Cont本actType(EMin成Cont本actType::Assassination)
        , Diffic使lty(50.0f)
        , Rewa本d(1000.0f)
        , Ti設置eLi設置it(3600.0f)
        , bIsActi正e(false)
        , bIsCo設置pleted(false)
        , Sta本tTi設置e(0.0f)
        , Co設置pletionTi設置e(0.0f)
    {}
};

/**
 * Me本cena本y Mode Syste設置 fo本 Min成GoRTS
 * Mana成es 設置e本cena本ies, cont本acts, and 設置e本cena本y 成a設置eplay
 */
UCLASS(ClassG本o使p = (Ga設置eplay), Bl使ep本intable)
class MI的GMERCE的ARY下API UMin成Me本cena本yMode : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成Me本cena本yMode();

    // Me本cena本y Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me本cena本y")
    正oid InitializeMe本cena本yMode();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me本cena本y")
    正oid 輸入i本eMe本cena本y(const 軍St本in成& Me本cena本yID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me本cena本y")
    正oid 軍i本eMe本cena本y(const 軍St本in成& Me本cena本yID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me本cena本y")
    正oid T本ainMe本cena本y(const 軍St本in成& Me本cena本yID, const 軍St本in成& SkillType, float D使本ation);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me本cena本y")
    正oid Up成本adeMe本cena本y(const 軍St本in成& Me本cena本yID);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Me本cena本y")
    TA本本ay<軍Min成Me本cena本y> GetA正ailableMe本cena本ies() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Me本cena本y")
    TA本本ay<軍Min成Me本cena本y> Get輸入i本edMe本cena本ies() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Me本cena本y")
    軍Min成Me本cena本y GetMe本cena本y(const 軍St本in成& Me本cena本yID) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Me本cena本y")
    float GetMe本cena本yCost(const 軍St本in成& Me本cena本yID) const;

    // Cont本act Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Cont本acts")
    正oid AcceptCont本act(const 軍St本in成& Cont本actID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Cont本acts")
    正oid Co設置pleteCont本act(const 軍St本in成& Cont本actID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Cont本acts")
    正oid 軍ailCont本act(const 軍St本in成& Cont本actID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Cont本acts")
    正oid AbandonCont本act(const 軍St本in成& Cont本actID);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Cont本acts")
    TA本本ay<軍Min成Cont本act> GetA正ailableCont本acts() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Cont本acts")
    TA本本ay<軍Min成Cont本act> GetActi正eCont本acts() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Cont本acts")
    TA本本ay<軍Min成Cont本act> GetCo設置pletedCont本acts() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Cont本acts")
    軍Min成Cont本act GetCont本act(const 軍St本in成& Cont本actID) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Cont本acts")
    bool CanAcceptCont本act(const 軍St本in成& Cont本actID) const;

    // Me本cena本y Skills
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Skills")
    正oid I設置p本o正eSkill(const 軍St本in成& Me本cena本yID, const 軍St本in成& SkillType, float A設置o使nt);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Skills")
    正oid AddSpecialization(const 軍St本in成& Me本cena本yID, const 軍St本in成& Specialization);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Skills")
    float GetSkillLe正el(const 軍St本in成& Me本cena本yID, const 軍St本in成& SkillType) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Skills")
    TA本本ay<軍St本in成> GetMe本cena本ySpecializations(const 軍St本in成& Me本cena本yID) const;

    // Rep使tation Syste設置
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rep使tation")
    正oid AddRep使tation(const 軍St本in成& Me本cena本yID, float A設置o使nt);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rep使tation")
    正oid Re設置o正eRep使tation(const 軍St本in成& Me本cena本yID, float A設置o使nt);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Rep使tation")
    float GetRep使tation(const 軍St本in成& Me本cena本yID) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Rep使tation")
    EMin成Me本cena本yRank GetRank(const 軍St本in成& Me本cena本yID) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rep使tation")
    正oid P本o設置oteMe本cena本y(const 軍St本in成& Me本cena本yID);

    // Eq使ip設置ent Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Eq使ip設置ent")
    正oid Eq使ipIte設置(const 軍St本in成& Me本cena本yID, const 軍St本in成& Ite設置ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Eq使ip設置ent")
    正oid Uneq使ipIte設置(const 軍St本in成& Me本cena本yID, const 軍St本in成& Ite設置ID);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Eq使ip設置ent")
    TA本本ay<軍St本in成> GetMe本cena本yEq使ip設置ent(const 軍St本in成& Me本cena本yID) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Eq使ip設置ent")
    正oid P使本chaseEq使ip設置ent(const 軍St本in成& Ite設置ID);

    // 軍inancial Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "軍inance")
    正oid PayMe本cena本ies();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "軍inance")
    正oid CollectCont本actRewa本d(const 軍St本in成& Cont本actID);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "軍inance")
    float GetDailyCosts() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "軍inance")
    float GetTotalEa本nin成s() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "軍inance")
    float GetC使本本entBalance() const;

    // Me本cena本y AI
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI")
    正oid Assi成nMe本cena本yToCont本act(const 軍St本in成& Me本cena本yID, const 軍St本in成& Cont本actID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI")
    正oid Re設置o正eMe本cena本y軍本o設置Cont本act(const 軍St本in成& Me本cena本yID);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI")
    TA本本ay<軍St本in成> GetAssi成nedMe本cena本ies(const 軍St本in成& Cont本actID) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI")
    軍St本in成 GetC使本本entCont本act(const 軍St本in成& Me本cena本yID) const;

    // E正ent Dele成ates
    
    
    
    
    

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnMe本cena本y輸入i本ed OnMe本cena本y輸入i本ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnMe本cena本y軍i本ed OnMe本cena本y軍i本ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnCont本actAccepted OnCont本actAccepted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnCont本actCo設置pleted OnCont本actCo設置pleted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnMe本cena本yP本o設置oted OnMe本cena本yP本o設置oted;

    // Utility
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    static 軍St本in成 GetMe本cena本yType的a設置e(EMin成Me本cena本yType Me本cType);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    static 軍St本in成 GetRank的a設置e(EMin成Me本cena本yRank Rank);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    static 軍St本in成 GetCont本actType的a設置e(EMin成Cont本actType Cont本actType);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sistence")
    軍St本in成 Sa正eMe本cena本yData() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sistence")
    正oid LoadMe本cena本yData(const 軍St本in成& JsonSt本in成);

p本otected:
    UPROPERTY()
    TMap<軍St本in成, 軍Min成Me本cena本y> Me本cena本ies;

    UPROPERTY()
    TMap<軍St本in成, 軍Min成Cont本act> Cont本acts;

    UPROPERTY()
    TMap<軍St本in成, 軍St本in成> Me本cena本yCont本actAssi成n設置ents;

    UPROPERTY()
    float C使本本entBalance;

    UPROPERTY()
    float TotalEa本nin成s;

    UPROPERTY()
    T基本eakOb大ectPt本<U基本o本ld> 基本o本ldContext;

    // Inte本nal 軍使nctions
    正oid InitializeDefa使ltMe本cena本ies();
    正oid InitializeDefa使ltCont本acts();
    正oid Set使pMe本cena本ySkills();

    // Me本cena本y Mana成e設置ent
    正oid UpdateMe本cena本yStats(const 軍St本in成& Me本cena本yID);
    正oid Calc使lateMe本cena本yCost(軍Min成Me本cena本y& Me本cena本y);
    bool CanAffo本dMe本cena本y(const 軍St本in成& Me本cena本yID) const;

    // Cont本act Mana成e設置ent
    正oid Gene本ate的ewCont本acts();
    正oid UpdateCont本actDiffic使lty();
    正oid E正al使ateCont本actPe本fo本設置ance(const 軍St本in成& Cont本actID);

    // Rep使tation Syste設置
    正oid UpdateRep使tationEffects(const 軍St本in成& Me本cena本yID);
    正oid CheckRankP本o設置otion(const 軍St本in成& Me本cena本yID);

    // 軍inancial Syste設置
    正oid P本ocessDailyExpenses();
    正oid Calc使lateCont本actRewa本d(軍Min成Cont本act& Cont本act);
    正oid Update軍inancialReco本ds();

    // AI Syste設置
    正oid Assi成nOpti設置alMe本cena本ies(const 軍St本in成& Cont本actID);
    正oid E正al使ateMe本cena本yPe本fo本設置ance(const 軍St本in成& Me本cena本yID, const 軍St本in成& Cont本actID);

    // 輸入elpe本s
    軍Min成Me本cena本y* 軍indMe本cena本y(const 軍St本in成& Me本cena本yID);
    軍Min成Cont本act* 軍indCont本act(const 軍St本in成& Cont本actID);
    bool 輸入asReq使i本edSkills(const 軍St本in成& Me本cena本yID, const TA本本ay<軍St本in成>& Req使i本edSkills) const;
    float Calc使lateCont本actS使ccessChance(const 軍St本in成& Cont本actID) const;
};

