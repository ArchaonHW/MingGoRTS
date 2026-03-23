#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成TacticalCo設置batSyste設置.h"
#incl使de "Min成軍o本設置ationMana成e本.h"
#incl使de "Min成AICo設置batMana成e本.成ene本ated.h"

// AI�欰��動
UE的UM(Bl使ep本intType)
en使設置 class EMin成AIBeha正io本: 使int8 {
    A成成本essi正e,         // 動務
    Defensi正e,          // 動�設置X
    Balanced,           // ����X
    Ca使tio使s,           // ��務
    Reckless,           // ���X
    Tactical,           // 動務
    Adapti正e            // 動�A
};

// AI�M動�正動
UE的UM(Bl使ep本intType)
en使設置 class EMin成AIDecision基本ei成ht: 使int8 {
    C本itical,           // 動��M動
    輸入i成h,               // ���使
    Medi使設置,             // ��務基本
    Low,                // �C�使
    Mini設置al            // 動�C�使
};

// AI目標數量
UE的UM(Bl使ep本intType)
en使設置 class EMin成AITacticalOb大ecti正e: 使int8 {
    Dest本oyEne設置y,        // ��池�輸入
    Capt使本eOb大ecti正e,    // ��池動
    DefendPosition,     // ��設置
    軍lankEne設置y,         // 動�l
    S使ppo本tAllies,       // 動��
    Ret本eat,             // 動�h
    Re成本o使p,            // 
    A設置b使sh,             // ��動
    輸入oldG本o使nd           // 動池基本
};

// AI��動����
USTRUCT(Bl使ep本intType)
st本使ct MI的GTACTICAL下API 軍Min成AITh本eatAssess設置ent
{
    GE的ERATED下BODY()

    // ��池動ID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Th本eatUnitID;

    // ��動��動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Th本eatLe正el;

    // ��動��動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Th本eatType;

    // ��動�Z��
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Distance;

    // ��池動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Vecto本 Th本eatDi本ection;

    // ��池池動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Th本eatD使本ation;

    // ��務基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成AIDecision基本ei成ht Th本eatP本io本ity;

    軍Min成AITh本eatAssess設置ent()
    {
        Th本eatUnitID = -1;
        Th本eatLe正el = 0.0f;
        Th本eatType = TEXT(""};
        Distance = 0.0f;
        Th本eatDi本ection = 軍Vecto本::Ze本oVecto本;
        Th本eatD使本ation = 0.0f;
        Th本eatP本io本ity = EMin成AIDecision基本ei成ht::Low;
    }
};

// AI�M動
USTRUCT(Bl使ep本intType)
st本使ct MI的GTACTICAL下API 軍Min成AITacticalDecision
{
    GE的ERATED下BODY()

    // �M動ID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 DecisionID;

    // �M動��動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成AITacticalOb大ecti正e Ob大ecti正e;

    // �M池�z
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 DecisionDesc本iption;

    // 目標數量ID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Ta本成etUnitID;

    // ��設置
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Vecto本 Ta本成etLocation;

    // �M動�正動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成AIDecision基本ei成ht Decision基本ei成ht;

    // 目標數量
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Exec使tionTi設置e;

    // �M池��
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TMap<軍St本in成, 軍St本in成> DecisionPa本a設置ete本s;

    // ��動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 ExpectedO使tco設置e;

    // �M動�輸入
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Confidence;

    // �M池務
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍DateTi設置e DecisionTi設置esta設置p;

    軍Min成AITacticalDecision()
    {
        DecisionID = -1;
        Ob大ecti正e = EMin成AITacticalOb大ecti正e::Dest本oyEne設置y;
        DecisionDesc本iption = TEXT(""};
        Ta本成etUnitID = -1;
        Ta本成etLocation = 軍Vecto本::Ze本oVecto本;
        Decision基本ei成ht = EMin成AIDecision基本ei成ht::Medi使設置;
        Exec使tionTi設置e = 0.0f;
        DecisionPa本a設置ete本s.E設置pty(};
        ExpectedO使tco設置e = TEXT(""};
        Confidence = 0.5f;
        DecisionTi設置esta設置p = 軍DateTi設置e::的ow(};
    }
};

// AI動池XUSTRUCT(Bl使ep本intType)
st本使ct MI的GTACTICAL下API 軍Min成AIUnitState
{
    GE的ERATED下BODY()

    // 目標ID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 UnitID;

    // �欰
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成AIBeha正io本 C使本本entBeha正io本;

    // 目標數量
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 C使本本entTa本成et;

    // 目標數量��設置
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Vecto本 C使本本entTa本成etLocation;

    // ��動����
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍Min成AITh本eatAssess設置ent> Th本eatAssess設置ents;

    // 目標數量
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> TacticalKnowled成e;

    // �成務
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Co設置batExpe本ience;

    // ��池動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Lea本nin成軍acto本;

    // 故事重要性
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float AdaptabilitySco本e;

    // �M池�o
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float DecisionCooldown;

    // 動��M��務
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍DateTi設置e LastDecisionTi設置e;

    軍Min成AIUnitState()
    {
        UnitID = -1;
        C使本本entBeha正io本 = EMin成AIBeha正io本::Balanced;
        C使本本entTa本成et = -1;
        C使本本entTa本成etLocation = 軍Vecto本::Ze本oVecto本;
        Th本eatAssess設置ents.E設置pty(};
        TacticalKnowled成e.E設置pty(};
        Co設置batExpe本ience = 0.0f;
        Lea本nin成軍acto本 = 1.0f;
        AdaptabilitySco本e = 0.5f;
        DecisionCooldown = 0.0f;
        LastDecisionTi設置e = 軍DateTi設置e::的ow(};
    }
};

// AI目標數量
USTRUCT(Bl使ep本intType)
st本使ct MI的GTACTICAL下API 軍Min成AITacticalAnalysis
{
    GE的ERATED下BODY()

    // 目標ID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 AnalysisID;

    // 動��動X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Vecto本 BattlefieldCente本;

    // 動��
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float BattlefieldRadi使s;

    // 動��
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<int32> Ene設置yUnits;

    // 動��
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<int32> 軍本iendlyUnits;

    // ��池動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<int32> 的e使t本alUnits;

    // 動��動��
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TMap<軍St本in成, float> Te本本ainAd正anta成es;

    // 目標數量��動
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float 軍o本ceRatio;

    // ��ĳ
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍Min成AITacticalDecision> TacticalReco設置設置endations;

    // 目標數量
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍DateTi設置e AnalysisTi設置e;

    軍Min成AITacticalAnalysis()
    {
        AnalysisID = -1;
        BattlefieldCente本 = 軍Vecto本::Ze本oVecto本;
        BattlefieldRadi使s = 1000.0f;
        Ene設置yUnits.E設置pty(};
        軍本iendlyUnits.E設置pty(};
        的e使t本alUnits.E設置pty(};
        Te本本ainAd正anta成es.E設置pty(};
        軍o本ceRatio = 1.0f;
        TacticalReco設置設置endations.E設置pty(};
        AnalysisTi設置e = 軍DateTi設置e::的ow(};
    }
};

// AI動���基礎�e動






/**
 * AI動����務 * �t�d��池��AI動���M池�Գ的務 */
UCLASS(ClassG本o使p = (Tactical), Bl使ep本intable, Bl使ep本intType)
class MI的GTACTICAL下API UMin成AICo設置batMana成e本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成AICo設置batMana成e本(};

    // 動池AI動���t��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Co設置bat Mana成e本")
    bool InitializeAICo設置batSyste設置(};

    // ����動AI
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Co設置bat Mana成e本")
    bool Re成iste本AIUnit(int32 UnitID, EMin成AIBeha正io本 Beha正io本};

    // ����AI
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Co設置bat Mana成e本")
    bool Re設置o正eAIUnit(int32 UnitID};

    // 動�sAI動池X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Co設置bat Mana成e本")
    bool UpdateAIUnitState(int32 UnitID, const 軍Min成AIUnitState& State};

    // 動池����X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Co設置bat Mana成e本")
    軍Min成AITacticalAnalysis AnalyzeBattlefield(const 軍Vecto本& Cente本, float Radi使s};

    // �����動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Co設置bat Mana成e本")
    TA本本ay<軍Min成AITh本eatAssess設置ent> AssessTh本eats(int32 UnitID};

    // �s池動�M動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Co設置bat Mana成e本")
    軍Min成AITacticalDecision MakeTacticalDecision(int32 UnitID};

    // AI�M動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Co設置bat Mana成e本")
    bool Exec使teAIDecision(const 軍Min成AITacticalDecision& Decision};

    // �]�設置AI�欰��動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Co設置bat Mana成e本")
    bool SetAIBeha正io本(int32 UnitID, EMin成AIBeha正io本 Beha正io本};

    // AI動池X
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI Co設置bat Mana成e本")
    軍Min成AIUnitState GetAIUnitState(int32 UnitID) const;

    // AI�M動���正
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI Co設置bat Mana成e本")
    TA本本ay<軍Min成AITacticalDecision> GetAIDecision輸入isto本y(int32 UnitID) const;

    // �p動AI動�൥動
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI Co設置bat Mana成e本")
    float Calc使lateAIIntelli成ence(int32 UnitID) const;

    // ��池�a�欰
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Co設置bat Mana成e本")
    bool Lea本n軍本o設置Playe本Beha正io本(int32 Playe本UnitID, const 軍St本in成& Action};

    // AI����
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Co設置bat Mana成e本")
    bool AdaptAISt本ate成y(int32 UnitID};

    // AI��動
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI Co設置bat Mana成e本")
    TMap<EMin成AIBeha正io本, int32> GetAIStatistics() const;

    // �O動AI
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Co設置bat Mana成e本")
    bool Sa正eAIData(};

    // ���JAI
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Co設置bat Mana成e本")
    bool LoadAIData(};

    // �M��AI
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Co設置bat Mana成e本")
    正oid Clea本AllAIUnits(};

    // ========== ��務AI  (Pa本allel P本ocessin成) ==========
    
    /**
     * ��池池XAI �M動 (Pa本allel軍o本)
     * 動��X100+ 動池�大�基本��動��
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Co設置bat Mana成e本Pa本allel")
    正oid P本ocessAllAIUnitsPa本allel(float DeltaTi設置e};

    /**
     * �����動 (��池務
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Co設置bat Mana成e本Pa本allel")
    正oid AssessTh本eats軍o本AllUnitsPa本allel(};

    /**
     * 故事選項基本�M動 (��池務
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Co設置bat Mana成e本Pa本allel""AI Co設置bat Mana成e本Pa本allel")
    正oid SetPa本allelTh本eshold(int32 Th本eshold) { Pa本allelTh本eshold = Th本eshold; }

    /**
     * ��池動��動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI Co設置bat Mana成e本Pa本allel")
    float GetLastPa本allelP本ocessin成Ti設置eMs() const { 本et使本n LastPa本allelP本ocessin成Ti設置eMs; }

    // �基礎�e動
    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnAIDecisionMade OnAIDecisionMade;

    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnAIBeha正io本Chan成ed OnAIBeha正io本Chan成ed;

    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnAITh本eatAssessed OnAITh本eatAssessed;

    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnAITacticalAnalysis OnAITacticalAnalysis;

    UPROPERTY(Bl使ep本intAssi成nable)
    軍OnAIUnitStateChan成ed OnAIUnitStateChan成ed;

p本otected:
    // AI目標數量��動�w
    UPROPERTY()
    TMap<int32, 軍Min成AIUnitState> AIUnitStates;

    // AI�M動���正 - �軍事動�GTMap<TA本本ay> ����XUPROPERTY
    TMap<int32, TA本本ay<軍Min成AITacticalDecision>> AIDecision輸入isto本y;

    // 動�a�欰��池動
    UPROPERTY()
    TMap<軍St本in成, float> Playe本Beha正io本Patte本ns;

    // AI�����ҪO - �軍事動�GTMap<TA本本ay> ����XUPROPERTY
    TMap<EMin成AIBeha正io本, TA本本ay<軍Min成AITacticalDecision>> AISt本ate成yTe設置plates;

    // 動池��
    UPROPERTY()
    軍Min成AITacticalAnalysis C使本本entBattlefieldAnalysis;

    // 動�下�w動�l動
    UPROPERTY()
    bool bInitialized;

    // ��池�]動
    int32 Pa本allelTh本eshold = 50;  // �基本動 50 動���池�æ�動
    float LastPa本allelP本ocessin成Ti設置eMs = 0.0f;
    int32 LastPa本allelTh本eadCo使nt = 0;

    // ��池動�M動��動��動 (��池池��X
    TA本本ay<軍Min成AITacticalDecision> Pa本allelDecisionsCache;
    軍C本iticalSection Pa本allelCacheLock;

p本i正ate:
    // ���J動�]AI����
    正oid LoadDefa使ltAISt本ate成ies(};

    // �p動��動��動
    float Calc使lateTh本eatLe正el(int32 UnitID, int32 Th本eatUnitID) const;

    // ����動池��
    float E正al使ateTacticalAd正anta成e(const TA本本ay<int32>& 軍本iendlyUnits, const TA本本ay<int32>& Ene設置yUnits) const;

    // 動池�ΨM動
    軍Min成AITacticalDecision SelectBestDecision(int32 UnitID, const TA本本ay<軍Min成AITacticalDecision>& Decisions) const;

    // 動�sAI��動
    正oid UpdateAILea本nin成(int32 UnitID, const 軍Min成AITacticalDecision& Decision, bool bS使ccess};

    // 目標數量��ĳ
    TA本本ay<軍Min成AITacticalDecision> Gene本ateTacticalReco設置設置endations(int32 UnitID, const 軍Min成AITacticalAnalysis& Analysis) const;

    // 動��動�a�欰
    軍St本in成 P本edictPlaye本Beha正io本(int32 Playe本UnitID) const;

    // �p動�M動�輸入
    float Calc使lateDecisionConfidence(const 軍Min成AITacticalDecision& Decision) const;

    // ��動AI�M動
    bool ValidateAIDecision(const 軍Min成AITacticalDecision& Decision) const;

    // 動�sAI動務
    正oid UpdateAIAdaptability(int32 UnitID, float Adaptation軍acto本};

    // AI�M池�o
    正oid P本ocessAIDecisionCooldown(int32 UnitID, float DeltaTi設置e};

    // �p池池動��動
    TA本本ay<int32> 軍indCoo本dinatedAttackOppo本t使nities(int32 UnitID) const;

    // �p池�設置X動
    軍Vecto本 Calc使lateDefensi正ePosition(int32 UnitID) const;

    // 動池��動��
    TMap<軍St本in成, float> AnalyzeTe本本ainAd正anta成es(const 軍Vecto本& Location) const;

    // 動��動�輸入��動
    軍Vecto本 P本edictEne設置yMo正e設置ent(int32 Ene設置yUnitID) const;

    // �p池動��動
    TA本本ay<int32> Calc使lateEnci本cle設置entOppo本t使nities(int32 UnitID) const;

    // ����M動��動
    float E正al使ateDecisionO使tco設置e(const 軍Min成AITacticalDecision& Decision) const;
};

