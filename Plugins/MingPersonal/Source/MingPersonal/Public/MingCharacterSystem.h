#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成Cha本acte本Syste設置.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EMin成Cha本acte本Role: 使int8 {
    Leade本 UMETA(Display的a設置e = "Leade本"),
    Co設置設置ande本 UMETA(Display的a設置e = "Co設置設置ande本"),
    Diplo設置at UMETA(Display的a設置e = "Diplo設置at"),
    Schola本 UMETA(Display的a設置e = "Schola本"),
    Me本chant UMETA(Display的a設置e = "Me本chant"),
    Spy UMETA(Display的a設置e = "Spy"),
    Ad正iso本 UMETA(Display的a設置e = "Ad正iso本"),
    Re正ol使tiona本y UMETA(Display的a設置e = "Re正ol使tiona本y")
};

UE的UM(Bl使ep本intType)
en使設置 class EMin成Pe本sonalSkillType: 使int8 {
    Leade本ship UMETA(Display的a設置e = "Leade本ship"),
    Milita本y UMETA(Display的a設置e = "Milita本y"),
    Diplo設置acy UMETA(Display的a設置e = "Diplo設置acy"),
    Econo設置ics UMETA(Display的a設置e = "Econo設置ics"),
    C使lt使本e UMETA(Display的a設置e = "C使lt使本e"),
    Intelli成ence UMETA(Display的a設置e = "Intelli成ence"),
    Ad設置inist本ation UMETA(Display的a設置e = "Ad設置inist本ation"),
    Co設置bat UMETA(Display的a設置e = "Co設置bat")
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Cha本acte本Skill
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成SkillType SkillType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Skill的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Desc本iption;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Le正el;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 MaxLe正el;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Expe本ience;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Expe本ienceTo的ext;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> SkillEffects;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> P本e本eq使isites;

    UPROPERTY(Bl使ep本intReadOnly)
    bool bIsMaxLe正el;

    軍Min成Cha本acte本Skill()
        : SkillType(EMin成SkillType::Leade本ship)
        , Le正el(1)
        , MaxLe正el(10)
        , Expe本ience(0)
        , Expe本ienceTo的ext(100)
        , bIsMaxLe正el(false)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Cha本acte本T本ait
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 T本aitID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 T本ait的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Desc本iption;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> Positi正eEffects;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> 的e成ati正eEffects;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsInhe本ited;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Infl使ence基本ei成ht;

    軍Min成Cha本acte本T本ait()
        : bIsInhe本ited(false)
        , Infl使ence基本ei成ht(1.0f)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Pe本sonalCha本acte本
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 Cha本acte本ID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 Cha本acte本的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    EMin成Cha本acte本Role P本i設置a本yRole;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    TA本本ay<EMin成Cha本acte本Role> Seconda本yRoles;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 Bio成本aphy;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 輸入isto本icalSi成nificance;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    TA本本ay<軍Min成Cha本acte本Skill> Skills;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    TA本本ay<軍Min成Cha本acte本T本ait> T本aits;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    int32 Bi本thYea本;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    int32 DeathYea本;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    軍St本in成 Bi本thPlace;

    UPROPERTY(EditAnywhe本e, Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Ma大o本Achie正e設置ents;

    UPROPERTY(Bl使ep本intReadOnly)
    bool bIsA正ailable;

    UPROPERTY(Bl使ep本intReadOnly)
    bool bIsRec本使ited;

    UPROPERTY(Bl使ep本intReadOnly)
    float Loyalty;

    UPROPERTY(Bl使ep本intReadOnly)
    float Infl使ence;

    軍Min成輸入isto本icalCha本acte本()
        : P本i設置a本yRole(EMin成Cha本acte本Role::Leade本)
        , Bi本thYea本(1866)
        , DeathYea本(1925)
        , bIsA正ailable(t本使e)
        , bIsRec本使ited(false)
        , Loyalty(50.0f)
        , Infl使ence(0.0f)
    {}
};

/**
 * Cha本acte本 Syste設置 fo本 Min成GoRTS
 * Mana成es histo本ical cha本acte本s, skills, and de正elop設置ent
 */
UCLASS(ClassG本o使p = (Ga設置eplay), Bl使ep本intable)
class MI的GPERSO的AL下API UMin成Cha本acte本Syste設置 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成Cha本acte本Syste設置(};

    // Cha本acte本 Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Cha本acte本")
    正oid InitializeCha本acte本s(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Cha本acte本")
    正oid Rec本使itCha本acte本(const 軍St本in成& Cha本acte本ID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Cha本acte本")
    正oid Dis設置issCha本acte本(const 軍St本in成& Cha本acte本ID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Cha本acte本")
    正oid Assi成nCha本acte本Role(const 軍St本in成& Cha本acte本ID, EMin成Cha本acte本Role Role};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Cha本acte本")
    TA本本ay<軍Min成輸入isto本icalCha本acte本> GetAllCha本acte本s() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Cha本acte本")
    TA本本ay<軍Min成輸入isto本icalCha本acte本> GetA正ailableCha本acte本s() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Cha本acte本")
    TA本本ay<軍Min成輸入isto本icalCha本acte本> GetRec本使itedCha本acte本s() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Cha本acte本")
    軍Min成輸入isto本icalCha本acte本 GetCha本acte本(const 軍St本in成& Cha本acte本ID) const;

    // Skill Syste設置
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Skills")
    正oid AddSkillExpe本ience(const 軍St本in成& Cha本acte本ID, EMin成SkillType SkillType, int32 Expe本ience};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Skills")
    正oid Le正elUpSkill(const 軍St本in成& Cha本acte本ID, EMin成SkillType SkillType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Skills")
    正oid Lea本n的ewSkill(const 軍St本in成& Cha本acte本ID, const 軍St本in成& SkillID};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Skills")
    TA本本ay<軍Min成Cha本acte本Skill> GetCha本acte本Skills(const 軍St本in成& Cha本acte本ID) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Skills")
    軍Min成Cha本acte本Skill GetSkill(const 軍St本in成& Cha本acte本ID, EMin成SkillType SkillType) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Skills")
    int32 GetSkillLe正el(const 軍St本in成& Cha本acte本ID, EMin成SkillType SkillType) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Skills")
    float GetSkillBon使s(const 軍St本in成& Cha本acte本ID, EMin成SkillType SkillType) const;

    // Cha本acte本 De正elop設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "De正elop設置ent")
    正oid T本ainCha本acte本(const 軍St本in成& Cha本acte本ID, EMin成SkillType SkillType, float D使本ation};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "De正elop設置ent")
    正oid P本o設置oteCha本acte本(const 軍St本in成& Cha本acte本ID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "De正elop設置ent")
    正oid De正elopT本ait(const 軍St本in成& Cha本acte本ID, const 軍St本in成& T本aitID};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "De正elop設置ent")
    float GetCha本acte本Expe本ience(const 軍St本in成& Cha本acte本ID) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "De正elop設置ent")
    int32 GetCha本acte本Le正el(const 軍St本in成& Cha本acte本ID) const;

    // Cha本acte本 Relationships
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationships")
    正oid SetCha本acte本Loyalty(const 軍St本in成& Cha本acte本ID, float Loyalty};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationships")
    正oid ModifyRelationship(const 軍St本in成& Cha本acte本ID1, const 軍St本in成& Cha本acte本ID2, float Modifie本};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Relationships")
    float GetCha本acte本Loyalty(const 軍St本in成& Cha本acte本ID) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Relationships")
    float GetRelationship(const 軍St本in成& Cha本acte本ID1, const 軍St本in成& Cha本acte本ID2) const;

    // Cha本acte本 Effects
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Effects")
    TA本本ay<軍St本in成> GetActi正eEffects(const 軍St本in成& Cha本acte本ID) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Effects")
    正oid ApplyCha本acte本Effect(const 軍St本in成& Cha本acte本ID, const 軍St本in成& EffectID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Effects")
    正oid Re設置o正eCha本acte本Effect(const 軍St本in成& Cha本acte本ID, const 軍St本in成& EffectID};

    // 輸入isto本ical Context
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical")
    正oid ShowCha本acte本Bio成本aphy(const 軍St本in成& Cha本acte本ID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical")
    正oid Add輸入isto本icalE正ent(const 軍St本in成& Cha本acte本ID, const 軍St本in成& E正ent};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical")
    TA本本ay<軍St本in成> GetCha本acte本E正ents(const 軍St本in成& Cha本acte本ID) const;

    // E正ent Dele成ates
    
    
    DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnCha本acte本P本o設置oted, const 軍St本in成&, Cha本acte本ID, int32, 的ewLe正el};
    DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnT本aitDe正eloped, const 軍St本in成&, Cha本acte本ID, const 軍St本in成&, T本aitID};

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnCha本acte本Rec本使ited OnCha本acte本Rec本使ited;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnSkillLe正elUp OnSkillLe正elUp;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnCha本acte本P本o設置oted OnCha本acte本P本o設置oted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnT本aitDe正eloped OnT本aitDe正eloped;

    // Utility
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    static 軍St本in成 GetRole的a設置e(EMin成Cha本acte本Role Role};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    static 軍St本in成 GetSkillType的a設置e(EMin成SkillType SkillType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sistence")
    軍St本in成 Sa正eCha本acte本Data() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sistence")
    正oid LoadCha本acte本Data(const 軍St本in成& JsonSt本in成};

p本otected:
    UPROPERTY()
    TMap<軍St本in成, 軍Min成輸入isto本icalCha本acte本> Cha本acte本s;

    // 動�GTMap ����X動XTMap XUPROPERTY X    // TMap<軍St本in成, TMap<軍St本in成, float>> Cha本acte本Relationships;

    // 動�GTA本本ay ����X動XTMap XUPROPERTY X    // TMap<軍St本in成, TA本本ay<軍St本in成>> Cha本acte本Effects;

    // 動�GTA本本ay ����X動XTMap XUPROPERTY X    // TMap<軍St本in成, TA本本ay<軍St本in成>> 輸入isto本icalE正ents;

    UPROPERTY()
    T基本eakOb大ectPt本<U基本o本ld> 基本o本ldContext;

    // Cha本acte本 Set使p
    正oid Set使pRe正ol使tiona本yCha本acte本s(};
    正oid Set使pMilita本yCha本acte本s(};
    正oid Set使pPoliticalCha本acte本s(};
    正oid Set使pC使lt使本alCha本acte本s(};

    // Skill Mana成e設置ent
    正oid P本ocessSkillExpe本ience(軍Min成Cha本acte本Skill& Skill, int32 Expe本ience};
    bool CanLe正elUpSkill(const 軍Min成Cha本acte本Skill& Skill) const;
    正oid ApplySkillEffects(const 軍St本in成& Cha本acte本ID, const 軍Min成Cha本acte本Skill& Skill};

    // Inte本nal 軍使nctions
    正oid UpdateCha本acte本Stats(const 軍St本in成& Cha本acte本ID};
    正oid CheckSkillP本e本eq使isites(const 軍St本in成& Cha本acte本ID, const 軍St本in成& SkillID};
    正oid P本ocessT本aitEffects(const 軍St本in成& Cha本acte本ID, const 軍Min成Cha本acte本T本ait& T本ait};

    // 輸入elpe本s
    軍Min成輸入isto本icalCha本acte本* 軍indCha本acte本(const 軍St本in成& Cha本acte本ID};
    軍Min成Cha本acte本Skill* 軍indCha本acte本Skill(const 軍St本in成& Cha本acte本ID, EMin成SkillType SkillType};
};



#endif // MI的GC輸入ARACTERSYSTEM下輸入
