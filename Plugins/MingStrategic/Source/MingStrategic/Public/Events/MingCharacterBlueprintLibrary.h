#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Kis設置et/Bl使ep本int軍使nctionLib本a本y.h"
#incl使de "E正ents/Min成輸入isto本icalCha本acte本.h"
#incl使de "Min成Cha本acte本Bl使ep本intLib本a本y.成ene本ated.h"

/**
 * 輸入目標數量 * 故事重要性值輸入t動
 */
UCLASS()
class MI的GSTRATEGIC下API UMin成Cha本acte本Bl使ep本intLib本a本y : p使blic UBl使ep本int軍使nctionLib本a本y
{
    GE的ERATED下BODY()

p使blic:
    /**
     * 正輸入務     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本")
    static UMin成輸入isto本icalCha本acte本Mana成e本* Get輸入isto本icalCha本acte本Mana成e本(};

    /**
     * 目標數量輸入動t動     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    static 正oid Initialize輸入isto本icalCha本acte本Syste設置(};

    /**
     * 動正輸入
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    static bool Re成iste本輸入isto本icalCha本acte本(const 軍Min成輸入isto本icalCha本acte本& Cha本acte本};

    /**
     * 動正輸入
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    static bool Un本e成iste本輸入isto本icalCha本acte本(const 軍St本in成& Cha本acte本ID};

    /**
     * 正輸入
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本")
    static 軍Min成輸入isto本icalCha本acte本 Get輸入isto本icalCha本acte本(const 軍St本in成& Cha本acte本ID};

    /**
     * 目標數量動輸入X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本")
    static TA本本ay<軍Min成輸入isto本icalCha本acte本> GetAll輸入isto本icalCha本acte本s(};

    /**
     * 故事重要性入X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    static TA本本ay<軍Min成輸入isto本icalCha本acte本> GetCha本acte本sBy軍action(ECha本acte本軍action 軍action};

    /**
     * 故事重要性商動輸入X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    static TA本本ay<軍Min成輸入isto本icalCha本acte本> GetCha本acte本sByP本ofession(ECha本acte本P本ofession P本ofession};

    /**
     * 動池輸入
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    static TA本本ay<軍Min成輸入isto本icalCha本acte本> GetKeyCha本acte本s(};

    /**
     * 故事重要性入X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    static TA本本ay<軍Min成輸入isto本icalCha本acte本> GetRec本使itableCha本acte本s(};

    /**
     * 動輸入
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    static TA本本ay<軍Min成輸入isto本icalCha本acte本> GetActi正eCha本acte本s(};

    /**
     * 輸入動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    static 軍Cha本acte本Inte本actionRes使lt Inte本act基本ithCha本acte本(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Inte本actionType, const 軍St本in成& Inte本actionDetails};

    /**
     * ܨt
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    static 軍Dialo成使eE正ent Sta本tDialo成使e(const 軍St本in成& Speake本ID, const 軍St本in成& A使dienceID, const 軍St本in成& Dialo成使eTopic};

    /**
     * 
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    static 軍Dialo成使eE正ent P本ocessDialo成使eOption(const 軍St本in成& Dialo成使eE正entID, const 軍St本in成& OptionID};

    /**
     * 輸入
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    static bool Rec本使itCha本acte本(const 軍St本in成& Cha本acte本ID};

    /**
     * 輸入
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    static bool Dis設置issCha本acte本(const 軍St本in成& Cha本acte本ID};

    /**
     * 輸入動X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    static bool Up成本adeCha本acte本Skill(const 軍St本in成& Cha本acte本ID, ECha本acte本SkillType SkillType};

    /**
     * 輸入
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    static TA本本ay<軍Cha本acte本Relationship> GetCha本acte本Relationships(const 軍St本in成& Cha本acte本ID};

    /**
     * K動輸入
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    static bool AddCha本acte本Relationship(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Ta本成etCha本acte本ID, ECha本acte本RelationshipType RelationshipType, float St本en成th};

    /**
     * 動s輸入
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    static bool UpdateCha本acte本Relationship(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Ta本成etCha本acte本ID, float 的ewSt本en成th};

    /**
     * 輸入動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本")
    static TMap<軍St本in成, int32> GetCha本acte本Statistics(};

    /**
     * 輸入正T動務     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    static TA本本ay<軍Min成輸入isto本icalCha本acte本> GetCha本acte本Infl使enceRankin成(};

    /**
     * 動池ˤ輸入
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    static TA本本ay<軍Min成輸入isto本icalCha本acte本> GetReco設置設置endedCha本acte本s(const 軍St本in成& Playe本軍action};

    /**
     * 動池O動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本")
    static 軍St本in成 GetGende本的a設置e(ECha本acte本Gende本 Gende本};

    /**
     * 動池年動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本")
    static 軍St本in成 GetP本ofession的a設置e(ECha本acte本P本ofession P本ofession};

    /**
     * 動池
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本")
    static 軍St本in成 Get軍action的a設置e(ECha本acte本軍action 軍action};

    /**
     * 目標數量     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本")
    static 軍St本in成 GetState的a設置e(ECha本acte本State State};

    /**
     * 目標數量池
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本")
    static 軍St本in成 GetRelationshipType的a設置e(ECha本acte本RelationshipType RelationshipType};

    /**
     * 目標數量務基本     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本")
    static 軍St本in成 GetSkillType的a設置e(ECha本acte本SkillType SkillType};

    /**
     * 動ؾ正輸入
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本C本eation")
    static 軍Min成輸入isto本icalCha本acte本 C本eate輸入isto本icalCha本acte本(
        const 軍St本in成& Cha本acte本ID,
        const 軍St本in成& Cha本acte本的a設置e,
        const 軍St本in成& Co使本tesy的a設置e,
        const 軍St本in成& En成lish的a設置e,
        const 軍St本in成& Cha本acte本Desc本iption,
        ECha本acte本Gende本 Gende本,
        ECha本acte本P本ofession P本ofession,
        ECha本acte本軍action 軍action,
        int32 Bi本thYea本,
        int32 DeathYea本,
        bool bIsKeyCha本acte本,
        bool bIsRec本使itable
    };

    /**
     * ]設置輸入動     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本C本eation")
    static 軍Min成輸入isto本icalCha本acte本 SetCha本acte本Att本ib使tes(
        const 軍Min成輸入isto本icalCha本acte本& Cha本acte本,
        float Leade本ship,
        float Milita本y,
        float Diplo設置acy,
        float Econo設置ics,
        float Intelli成ence,
        float Cha本is設置a,
        float Ad設置inist本ation,
        float St本ate成y,
        float O本ato本y,
        float 基本本itin成,
        float Science,
        float En成inee本in成,
        float Medicine,
        float A本t
    };

    /**
     * K動輸入動X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本C本eation")
    static 軍Min成輸入isto本icalCha本acte本 AddCha本acte本Skill(
        const 軍Min成輸入isto本icalCha本acte本& Cha本acte本,
        ECha本acte本SkillType SkillType,
        const 軍St本in成& Skill的a設置e,
        const 軍St本in成& SkillDesc本iption,
        int32 SkillLe正el,
        bool bIsUnlocked
    };

    /**
     * K動輸入
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本C本eation")
    static 軍Min成輸入isto本icalCha本acte本 AddCha本acte本Ta成(
        const 軍Min成輸入isto本icalCha本acte本& Cha本acte本,
        const 軍St本in成& Ta成
    };

    /**
     * K動正基礎
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本C本eation")
    static 軍Min成輸入isto本icalCha本acte本 Add輸入isto本icalE正ent(
        const 軍Min成輸入isto本icalCha本acte本& Cha本acte本,
        const 軍St本in成& E正ent
    };

    /**
     * K池的
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本C本eation")
    static 軍Min成輸入isto本icalCha本acte本 AddAchie正e設置ent(
        const 軍Min成輸入isto本icalCha本acte本& Cha本acte本,
        const 軍St本in成& Achie正e設置ent
    };

    /**
     * ]設置正T目標數量
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本C本eation")
    static 軍Min成輸入isto本icalCha本acte本 SetInfl使enceAndRep使tation(
        const 軍Min成輸入isto本icalCha本acte本& Cha本acte本,
        float Infl使ence,
        float Rep使tation
    };

    /**
     * 動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本Batch")
    static TA本本ay<軍Cha本acte本Inte本actionRes使lt> BatchInte本act基本ithCha本acte本s(const TA本本ay<軍St本in成>& Cha本acte本IDs, const 軍St本in成& Inte本actionType};

    /**
     * 目標數量
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本Batch")
    static TA本本ay<bool> BatchRec本使itCha本acte本s(const TA本本ay<軍St本in成>& Cha本acte本IDs};

    /**
     * t動X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本Syste設置")
    static bool Is輸入isto本icalCha本acte本Syste設置Initialized(};

    /**
     * t動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本Syste設置")
    static 軍St本in成 Get輸入isto本icalCha本acte本Syste設置Ve本sion(};

    /**
     * 動輸入
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本Syste設置")
    static int32 GetRe成iste本edCha本acte本Co使nt(};

    /**
     * 動輸入
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本Syste設置")
    static int32 GetActi正eCha本acte本Co使nt(};

    /**
     * 動池輸入
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本Syste設置")
    static int32 GetKeyCha本acte本Co使nt(};

    /**
     * 故事重要性入動X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本Syste設置")
    static int32 GetRec本使itableCha本acte本Co使nt(};

    /**
     * 輸入動z
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本Info")
    static 軍St本in成 GetCha本acte本Desc本iption(const 軍St本in成& Cha本acte本ID};

    /**
     * 輸入年
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本Info")
    static int32 GetCha本acte本A成e(const 軍St本in成& Cha本acte本ID};

    /**
     * ˬd輸入動下動務     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本Info")
    static bool IsCha本acte本Rec本使itable(const 軍St本in成& Cha本acte本ID};

    /**
     * ˬd輸入動下動池輸入X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本Info")
    static bool IsCha本acte本KeyCha本acte本(const 軍St本in成& Cha本acte本ID};

    /**
     * 輸入     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本Skills")
    static int32 GetCha本acte本SkillLe正el(const 軍St本in成& Cha本acte本ID, ECha本acte本SkillType SkillType};

    /**
     * 輸入務     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本Skills")
    static TA本本ay<軍Cha本acte本Skill> GetCha本acte本Skills(const 軍St本in成& Cha本acte本ID};

    /**
     * ˬd輸入動下動池X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本Skills")
    static bool 輸入asCha本acte本Skill(const 軍St本in成& Cha本acte本ID, ECha本acte本SkillType SkillType};

    /**
     * 目標數量大
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本Relationships")
    static float GetRelationshipSt本en成th(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Ta本成etCha本acte本ID};

    /**
     * 目標數量動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本Relationships")
    static ECha本acte本RelationshipType GetRelationshipType(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Ta本成etCha本acte本ID};

    /**
     * ˬd動下sb
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本Relationships")
    static bool 輸入asRelationship(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Ta本成etCha本acte本ID};

    /**
     * 輸入正TX     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本Analysis")
    static float GetCha本acte本Infl使ence(const 軍St本in成& Cha本acte本ID};

    /**
     * 輸入
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本Analysis")
    static float GetCha本acte本Rep使tation(const 軍St本in成& Cha本acte本ID};

    /**
     * 輸入     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本Analysis")
    static float GetCha本acte本Loyalty(const 軍St本in成& Cha本acte本ID};

    /**
     * 輸入動d     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本Analysis")
    static float GetCha本acte本輸入ealth(const 軍St本in成& Cha本acte本ID};

    /**
     * 輸入池X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本Analysis")
    static float GetCha本acte本Mo本ale(const 軍St本in成& Cha本acte本ID};

    /**
     * 輸入動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本Analysis")
    static int32 GetCha本acte本Le正el(const 軍St本in成& Cha本acte本ID};

    /**
     * 輸入成動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本Analysis")
    static int32 GetCha本acte本Expe本ience(const 軍St本in成& Cha本acte本ID};

    /**
     * 輸入軍事
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本Analysis")
    static float GetCha本acte本O正e本allSco本e(const 軍St本in成& Cha本acte本ID};

    /**
     * 動ĳ
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本AI")
    static TA本本ay<軍St本in成> GetInte本actionReco設置設置endations(const 軍St本in成& Cha本acte本ID};

    /**
     * ܫĳ
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本AI")
    static TA本本ay<軍St本in成> GetDialo成使eReco設置設置endations(const 軍St本in成& Speake本ID, const 軍St本in成& A使dienceID};

    /**
     * 目標數量ĳ
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本AI")
    static TA本本ay<軍St本in成> GetRec本使it設置entReco設置設置endations(const 軍St本in成& Playe本軍action};

    // 輸入 - 軍事動GTMap<TA本本ay> XBl使ep本int
    static TMap<軍St本in成, TA本本ay<軍St本in成>> GetCha本acte本的etwo本k(const 軍St本in成& Cha本acte本ID};

    /**
     * 目標數量正T
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本的etwo本k")
    static TMap<軍St本in成, float> GetRelationshipI設置pactAnalysis(const 軍St本in成& Cha本acte本ID};

    /**
     * 輸入動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本Si設置使lation")
    static TA本本ay<軍Cha本acte本Inte本actionRes使lt> Si設置使lateCha本acte本Inte本actions(const 軍St本in成& Cha本acte本ID, int32 Si設置使lationCo使nt};

    /**
     * ܵ動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本Si設置使lation")
    static TA本本ay<軍Dialo成使eE正ent> Si設置使lateDialo成使e(const 軍St本in成& Speake本ID, const 軍St本in成& A使dienceID, const 軍St本in成& Dialo成使eTopic, int32 Si設置使lationCo使nt};

    /**
     * 基本X輸入
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本Expo本t")
    static bool Expo本tCha本acte本Data(const 軍St本in成& 軍ilePath};

    /**
     * ɤJ輸入
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本I設置po本t")
    static bool I設置po本tCha本acte本Data(const 軍St本in成& 軍ilePath};

    /**
     * 輸入
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本Repo本t")
    static 軍St本in成 Gene本ateCha本acte本Repo本t(const 軍St本in成& Cha本acte本ID};

    /**
     * 動目標數量
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本Repo本t")
    static 軍St本in成 Gene本ateStatisticsRepo本t(};

    /**
     * 故事選項基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本Repo本t")
    static 軍St本in成 Gene本ateRelationshipRepo本t(const 軍St本in成& Cha本acte本ID};

    /**
     * 動正
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本輸入isto本y")
    static TA本本ay<軍Dialo成使eE正ent> GetDialo成使e輸入isto本y(};

    /**
     * ܾ正
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本輸入isto本y")
    static TA本本ay<軍Dialo成使eE正ent> GetDialo成使e輸入isto本y軍o本Cha本acte本(const 軍St本in成& Cha本acte本ID};
};
};
