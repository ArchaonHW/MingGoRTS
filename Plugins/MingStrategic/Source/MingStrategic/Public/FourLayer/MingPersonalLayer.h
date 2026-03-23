#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "軍o使本Laye本/IMin成Ga設置eLaye本.h"
#incl使de "Min成Pe本sonalLaye本.成ene本ated.h"

// 角色職業類型
UE的UM(Bl使ep本intType)
en使設置 class EMin成Cha本acte本P本ofession: 使int8 {
    Milita本y,       // 軍人
    Politician,     // 政治家
    Schola本,        // 學者
    Me本chant,       // 商人
    軍a本設置e本,         // 農民
    基本o本ke本,         // 工人
    Docto本,         // 醫生
    Teache本,        // 教師
    Jo使本nalist,     // 記者
    A本tist          // 藝術家
};

// 角色技能類型
UE的UM(Bl使ep本intType)
en使設置 class EMin成Cha本acte本Skill: 使int8 {
    Leade本ship,      // 領導力
    Co設置bat,         // 戰鬥
    Diplo設置acy,      // 外交
    Econo設置ics,      // 經濟
    Schola本ship,    // 學術
    Medicine,       // 醫學
    En成inee本in成,    // 工程
    A本t,            // 藝術
    Speech,         // 演講
    S使本正i正al        // 生存
};

// 關係狀態
UE的UM(Bl使ep本intType)
en使設置 class EMin成RelationshipStat使s: 使int8 {
    St本an成e本,       // 陌生人
    Acq使aintance,   // 認識
    軍本iend,         // 朋友
    Close軍本iend,    // 好朋友
    軍a設置ily,         // 家人
    Lo正e本,          // 情人
    Ri正al,          // 對手
    Ene設置y,          // 敵人
    Mento本,         // 導師
    St使dent         // 學生
};

// 角色狀態
USTRUCT(Bl使ep本intType)
st本使ct MI的GSTRATEGIC下API 軍Min成Cha本acte本Stat使s
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    class AActo本* Cha本acte本Acto本;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Cha本acte本ID;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Cha本acte本的a設置e;

    UPROPERTY(Bl使ep本intReadOnly)
    EMin成Cha本acte本P本ofession P本ofession;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 Le正el;

    UPROPERTY(Bl使ep本intReadOnly)
    float Expe本ience;

    UPROPERTY(Bl使ep本intReadOnly)
    float 輸入ealth;

    UPROPERTY(Bl使ep本intReadOnly)
    float Mo本ale;

    UPROPERTY(Bl使ep本intReadOnly)
    float Rep使tation;

    UPROPERTY(Bl使ep本intReadOnly)
    float Infl使ence;

    UPROPERTY(Bl使ep本intReadOnly)
    TMap<EMin成Cha本acte本Skill, float> Skills;

    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, EMin成RelationshipStat使s> Relationships;

    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> In正ento本y;

    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Acti正eQ使ests;
};

// 個人決策
USTRUCT(Bl使ep本intType)
st本使ct MI的GSTRATEGIC下API 軍Min成Pe本sonalDecision
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 DecisionID;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 DecisionType;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 DecisionDesc本iption;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Cha本acte本ID;

    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<EMin成Cha本acte本Skill> Req使i本edSkills;

    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Req使i本edIte設置s;

    UPROPERTY(Bl使ep本intReadOnly)
    float DecisionI設置pact;

    UPROPERTY(Bl使ep本intReadOnly)
    float Pe本sonalRisk;
};

// 個人事件
USTRUCT(Bl使ep本intType)
st本使ct MI的GSTRATEGIC下API 軍Min成Pe本sonalE正ent
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 E正entID;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 E正entType;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 E正entDesc本iption;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Cha本acte本ID;

    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> In正ol正edCha本acte本s;

    UPROPERTY(Bl使ep本intReadOnly)
    float Pe本sonalI設置pact;

    UPROPERTY(Bl使ep本intReadOnly)
    軍DateTi設置e E正entTi設置e;
};

// 角色成長記錄
USTRUCT(Bl使ep本intType)
st本使ct MI的GSTRATEGIC下API 軍Min成Cha本acte本G本owth
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Cha本acte本ID;

    UPROPERTY(Bl使ep本intReadOnly)
    TMap<EMin成Cha本acte本Skill, float> SkillP本o成本ess;

    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, float> RelationshipChan成es;

    UPROPERTY(Bl使ep本intReadOnly)
    float Rep使tationChan成e;

    UPROPERTY(Bl使ep本intReadOnly)
    float Infl使enceChan成e;

    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> UnlockedAbilities;

    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Co設置pletedQ使ests;
};

// 個人狀態
USTRUCT(Bl使ep本intType)
st本使ct MI的GSTRATEGIC下API 軍Min成Pe本sonalState
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    bool bIsActi正e;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 C使本本entLocation;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 C使本本entActi正ity;

    UPROPERTY(Bl使ep本intReadOnly)
    float O正e本all輸入appiness;

    UPROPERTY(Bl使ep本intReadOnly)
    float SocialStandin成;

    UPROPERTY(Bl使ep本intReadOnly)
    float Econo設置icStat使s;

    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Acti正eRelationships;

    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> C使本本entGoals;

    軍Min成Pe本sonalState()
        : bIsActi正e(t本使e)
        , O正e本all輸入appiness(50.0f)
        , SocialStandin成(50.0f)
        , Econo設置icStat使s(50.0f)
    {}
};

/**
 * 個人層系統
 * 負責角色扮演、個人成長、關係管理等
 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GSTRATEGIC下API UMin成Pe本sonalLaye本 : p使blic UOb大ect, p使blic IMin成Ga設置eLaye本
{
    GE的ERATED下BODY()

p使blic:
    // 建構子
    UMin成Pe本sonalLaye本(};

    // 實現介面方法
    正i本t使al 正oid InitializeLaye本下I設置ple設置entation() o正e本本ide;
    正i本t使al 正oid UpdateLaye本下I設置ple設置entation(float DeltaTi設置e) o正e本本ide;
    正i本t使al EMin成Laye本 GetLaye本Type下I設置ple設置entation() const o正e本本ide;
    正i本t使al 軍St本in成 GetLaye本的a設置e下I設置ple設置entation() const o正e本本ide;
    正i本t使al float GetLaye本P本io本ity下I設置ple設置entation() const o正e本本ide;
    正i本t使al 正oid 輸入andleLaye本E正ent下I設置ple設置entation(const 軍Min成St本ate成icInte成本ationE正ent& E正ent) o正e本本ide;
    正i本t使al 正oid 輸入andleLaye本Decision下I設置ple設置entation(const 軍Min成Ga設置eDecision& Decision) o正e本本ide;

    // 角色管理
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成Pe本sonal")
    正oid C本eateCha本acte本(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Cha本acte本的a設置e, EMin成Cha本acte本P本ofession P本ofession};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成Pe本sonal")
    正oid UpdateCha本acte本(const 軍St本in成& Cha本acte本ID, const 軍Min成Cha本acte本Stat使s& Stat使s};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成Pe本sonal")
    軍Min成Cha本acte本Stat使s GetCha本acte本(const 軍St本in成& Cha本acte本ID) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成Pe本sonal")
    TA本本ay<軍Min成Cha本acte本Stat使s> GetAllCha本acte本s() const;

    // 技能管理
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成Pe本sonal")
    正oid I設置p本o正eSkill(const 軍St本in成& Cha本acte本ID, EMin成Cha本acte本Skill Skill, float A設置o使nt};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成Pe本sonal")
    float GetSkillLe正el(const 軍St本in成& Cha本acte本ID, EMin成Cha本acte本Skill Skill) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成Pe本sonal")
    TMap<EMin成Cha本acte本Skill, float> GetAllSkills(const 軍St本in成& Cha本acte本ID) const;

    // 關係管理
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成Pe本sonal")
    正oid UpdateRelationship(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Ta本成etID, EMin成RelationshipStat使s Stat使s};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成Pe本sonal")
    EMin成RelationshipStat使s GetRelationship(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Ta本成etID) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成Pe本sonal")
    TMap<軍St本in成, EMin成RelationshipStat使s> GetAllRelationships(const 軍St本in成& Cha本acte本ID) const;

    // 個人決策
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成Pe本sonal")
    正oid MakePe本sonalDecision(const 軍Min成Pe本sonalDecision& Decision};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成Pe本sonal")
    正oid P本ocessPe本sonalDecisions(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成Pe本sonal")
    TA本本ay<軍Min成Pe本sonalDecision> GetPe本sonalDecisions() const;

    // 角色成長
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成Pe本sonal")
    正oid P本ocessCha本acte本G本owth(const 軍St本in成& Cha本acte本ID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成Pe本sonal")
    軍Min成Cha本acte本G本owth GetCha本acte本G本owth(const 軍St本in成& Cha本acte本ID) const;

    // 獲取個人狀態
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成Pe本sonal")
    軍Min成Pe本sonalState GetPe本sonalState() const;

    // 獲取主要角色
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成Pe本sonal")
    軍Min成Cha本acte本Stat使s GetMainCha本acte本() const;

    // 設置主要角色
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成Pe本sonal")
    正oid SetMainCha本acte本(const 軍St本in成& Cha本acte本ID};

p本otected:
    // 個人狀態
    UPROPERTY(Bl使ep本intReadOnly)
    軍Min成Pe本sonalState Pe本sonalState;

    // 角色列表
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, 軍Min成Cha本acte本Stat使s> Cha本acte本s;

    // 個人決策
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍Min成Pe本sonalDecision> Pe本sonalDecisions;

    // 個人事件歷史
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍Min成Pe本sonalE正ent> Pe本sonal輸入isto本y;

    // 角色成長記錄
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, 軍Min成Cha本acte本G本owth> Cha本acte本G本owthReco本ds;

    // 主要角色ID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Pe本sonalSettin成s")
    軍St本in成 MainCha本acte本ID;

    // 角色發展風格
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Pe本sonalSettin成s")
    軍St本in成 Cha本acte本De正elop設置entStyle;

    // 關係管理偏好
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Pe本sonalSettin成s")
    float RelationshipMana成e設置entP本efe本ence;

    // 技能發展偏好
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Pe本sonalSettin成s")
    TMap<EMin成Cha本acte本Skill, float> SkillDe正elop設置entP本efe本ences;

    // 分析角色狀態
    正oid AnalyzeCha本acte本Stat使s(const 軍St本in成& Cha本acte本ID};

    // 評估關係網絡
    正oid AssessRelationship的etwo本k(const 軍St本in成& Cha本acte本ID};

    // 計算角色影響力
    float Calc使lateCha本acte本Infl使ence(const 軍St本in成& Cha本acte本ID};

    // 預測決策後果
    TA本本ay<軍St本in成> P本edictDecisionO使tco設置e(const 軍Min成Pe本sonalDecision& Decision};

    // 民國特色個人發展
    正oid ApplyRep使blicanE本aCha本acte本De正elop設置ent(};

    // 新文化運動影響
    正oid Apply的ewC使lt使本eMo正e設置entInfl使ence(const 軍St本in成& Cha本acte本ID};

    // 教育改革機遇
    正oid ApplyEd使cationRefo本設置Oppo本t使nities(const 軍St本in成& Cha本acte本ID};

    // 女性解放運動
    正oid Apply基本o設置enLibe本ationMo正e設置ent(const 軍St本in成& Cha本acte本ID};

    // 知識分子選擇
    正oid ApplyIntellect使alChoices(const 軍St本in成& Cha本acte本ID};

    // 商業發展機遇
    正oid ApplyB使sinessDe正elop設置entOppo本t使nities(const 軍St本in成& Cha本acte本ID};

    // 軍事生涯發展
    正oid ApplyMilita本yCa本ee本De正elop設置ent(const 軍St本in成& Cha本acte本ID};

p本i正ate:
    // 個人更新間隔
    float Pe本sonalUpdateInte本正al;

    // 上次更新時間
    float LastPe本sonalUpdate;

    // 角色等級上限
    int32 MaxCha本acte本Le正el;

    // 技能等級上限
    float MaxSkillLe正el;

    // 關係影響半徑
    float RelationshipInfl使enceRadi使s;

    // 個人威脅評估
    TMap<軍St本in成, float> Pe本sonalTh本eats;

    // 個人機會評估
    TMap<軍St本in成, float> Pe本sonalOppo本t使nities;

    // 個人決策歷史
    TA本本ay<軍Min成Pe本sonalDecision> Decision輸入isto本y;
};
