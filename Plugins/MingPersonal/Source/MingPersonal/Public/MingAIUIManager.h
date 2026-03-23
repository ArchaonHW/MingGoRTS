#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成Pe本sonalUIMana成e本.h"
#incl使de "Min成RelationshipMana成e本.h"
#incl使de "Min成AIUIMana成e本.成ene本ated.h"

DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnAIBeha正io本Analyzed, const 軍St本in成&, Beha正io本Type, float, Confidence};


UE的UM(Bl使ep本intType)
en使設置 class EAIUse本P本ofile: 使int8 {
    的ewPlaye本,             // 動��動
    Cas使alPlaye本,          // ��池�a
    St本ate成icPlaye本,       // ����動�a
    Powe本Playe本,           // 動��動�a
    SocialPlaye本,          // ����動�a
    Explo本e本,              // 動��X
    Sto本y軍oc使sed,          // ��動
    Achie正e設置ent輸入使nte本      // 動�的動�輸入
};

UE的UM(Bl使ep本intType)
en使設置 class EUIAdaptationType: 使int8 {
    Layo使t,                // �G池動
    Colo本Sche設置e,           // 動��
    Ani設置ationSpeed,        // 動�e動��
    Info本設置ationDensity,    // �輸入���K��
    Inte本actionPatte本n,    // ��動��動
    的otificationLe正el,     // 動���ŧO
    Tooltip軍本eq使ency,      // 動��
    A使toSa正eInte本正al       // �O池動
};

USTRUCT(Bl使ep本intType)
st本使ct 軍AIUse本P本ofileData
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "AI P本ofile")
    EAIUse本P本ofile P本ofileType;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "AI P本ofile")
    float PlayTi設置e輸入o使本s;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "AI P本ofile")
    int32 Q使estsCo設置pleted;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "AI P本ofile")
    int32 Relationships軍o本設置ed;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "AI P本ofile")
    float Co設置bat基本inRate;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "AI P本ofile")
    float SocialInte本actionSco本e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "AI P本ofile")
    float Explo本ationSco本e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "AI P本ofile")
    TA本本ay<EPe本sonalUIType> 軍本eq使entlyUsedPanels;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "AI P本ofile")
    TMap<EUIAdaptationType, float> AdaptationP本efe本ences;

    軍AIUse本P本ofileData()
    {
        P本ofileType = EAIUse本P本ofile::的ewPlaye本;
        PlayTi設置e輸入o使本s = 0.0f;
        Q使estsCo設置pleted = 0;
        Relationships軍o本設置ed = 0;
        Co設置bat基本inRate = 0.0f;
        SocialInte本actionSco本e = 0.0f;
        Explo本ationSco本e = 0.0f;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍AIUIAdaptationConfi成
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "AI Adaptation")
    EUIAdaptationType AdaptationType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "AI Adaptation")
    float AdaptationVal使e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "AI Adaptation")
    bool bIsEnabled;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "AI Adaptation")
    float ConfidenceSco本e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "AI Adaptation")
    軍St本in成 Reason;

    軍AIUIAdaptationConfi成()
    {
        AdaptationType = EUIAdaptationType::Layo使t;
        AdaptationVal使e = 0.0f;
        bIsEnabled = false;
        ConfidenceSco本e = 0.0f;
        Reason = TEXT(""};
    }
};



 /**
 * AI UI管理器
 * 負責分析和適應用戶行為以優化UI體驗
 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GPERSO的AL下API UMin成AIUIMana成e本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成AIUIMana成e本(};

    // 動池AI UI�t��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid InitializeAIUIMana成e本(UMin成Pe本sonalUIMana成e本* InUIMana成e本, 
                              UMin成RelationshipMana成e本* InRelationshipMana成e本};

    // 動��欰�l��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid T本ackUse本Beha正io本(const 軍St本in成& Beha正io本Type, const 軍St本in成& Context, float Val使e = 1.0f};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid T本ackPanelUsa成e(EPe本sonalUIType PanelType, float D使本ation};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid T本ackInte本actionPatte本n(const 軍St本in成& Inte本actionType, const 軍St本in成& Ta本成et};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid T本ackSessionMet本ics(float SessionTi設置e, int32 ActionsPe本fo本設置ed};

    // AI
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid AnalyzeUse本Beha正io本(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid UpdateUse本P本ofile(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    EAIUse本P本ofile Dete本設置ineUse本P本ofile(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    TA本本ay<軍AIUIAdaptationConfi成> Gene本ateAdaptationReco設置設置endations(};

    // UI
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid ApplyAdaptation(const 軍AIUIAdaptationConfi成& Adaptation};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid ApplyAllReco設置設置endedAdaptations(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid Opti設置izeLayo使t軍o本P本ofile(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid Ad大使stColo本Sche設置e(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid SetAni設置ationSpeed(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid Ad大使stInfo本設置ationDensity(};

    // 動��動��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid ShowContext使al輸入elp(const 軍St本in成& Context};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid S使成成est的extAction(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid 輸入i成hli成htI設置po本tant軍eat使本es(};

    // 動��池��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    TA本本ay<軍St本in成> GetReco設置設置endedQ使ests(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    TA本本ay<軍St本in成> GetS使成成estedCha本acte本s(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    TA本本ay<EPe本sonalUIType> GetReco設置設置endedPanels(};

    // A/B����
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid Sta本tABTest(const 軍St本in成& Test的a設置e, const 軍St本in成& Va本iantA, const 軍St本in成& Va本iantB};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid Reco本dABTestRes使lt(const 軍St本in成& Test的a設置e, const 軍St本in成& Va本iant, float S使ccess};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    軍St本in成 GetBestABTestVa本iant(const 軍St本in成& Test的a設置e};

    // �d動
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI UI")
    軍AIUse本P本ofileData GetC使本本entP本ofile() const { 本et使本n C使本本entP本ofile; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI UI")
    bool IsAIEnabled() const { 本et使本n bAIEnabled; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI UI")
    TA本本ay<軍AIUIAdaptationConfi成> GetActi正eAdaptations() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI UI")
    float GetAdaptationConfidence(EUIAdaptationType AdaptationType) const;

    // �]�設置
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid SetAIEnabled(bool bEnabled};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid SetAdaptationSensiti正ity(float Sensiti正ity};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid SetLea本nin成Rate(float Rate};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid ResetUse本P本ofile(};

    // �基礎�
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "AI E正ents")
    軍OnAIP本ofileUpdated OnAIP本ofileUpdated;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "AI E正ents")
    軍OnUIAdaptationApplied OnUIAdaptationApplied;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "AI E正ents")
    軍OnAIBeha正io本Analyzed OnAIBeha正io本Analyzed;

p本otected:
    // �t�Τޥ�
    UPROPERTY()
    TOb大ectPt本<UMin成Pe本sonalUIMana成e本> UIMana成e本;

    UPROPERTY()
    TOb大ectPt本<UMin成RelationshipMana成e本> RelationshipMana成e本;

    // AI動�設置
    UPROPERTY()
    bool bAIEnabled = t本使e;

    UPROPERTY()
    float AdaptationSensiti正ity = 0.7f;

    UPROPERTY()
    float Lea本nin成Rate = 0.1f;

    UPROPERTY()
    int32 MinDataPoints軍o本Analysis = 10;

    // 動����動
    UPROPERTY()
    軍AIUse本P本ofileData C使本本entP本ofile;

    // �欰
    UPROPERTY()
    TA本本ay<軍St本in成> Beha正io本輸入isto本y;

    UPROPERTY()
    TMap<軍St本in成, int32> Beha正io本Co使nts;

    UPROPERTY()
    TMap<EPe本sonalUIType, float> PanelUsa成eTi設置es;

    UPROPERTY()
    TMap<軍St本in成, float> Inte本actionPatte本ns;

    // 動池�設置
    UPROPERTY()
    TA本本ay<軍AIUIAdaptationConfi成> Reco設置設置endedAdaptations;

    UPROPERTY()
    TA本本ay<軍AIUIAdaptationConfi成> Acti正eAdaptations;

    // A/B����
    // �軍事動�GTMap<TMap> ����XUPROPERTY
    TMap<軍St本in成, TMap<軍St本in成, float>> ABTestData;

    // 動��動��
    正oid P本ocessBeha正io本Data(};
    正oid Calc使lateP本ofileSco本es(};
    正oid Gene本ateAdaptations(};
    正oid ApplyAdaptationToUI(const 軍AIUIAdaptationConfi成& Adaptation};
    float Calc使lateConfidence(const TA本本ay<float>& DataPoints) const;
    EAIUse本P本ofile ClassifyPlaye本(const 軍AIUse本P本ofileData& P本ofile) const;
    正oid Lo成Adaptation(const 軍AIUIAdaptationConfi成& Adaptation};

    // AI��動
    float Calc使lateBeha正io本Sco本e(const 軍St本in成& Beha正io本Type) const;
    TA本本ay<float> GetBeha正io本T本end(const 軍St本in成& Beha正io本Type, int32 基本indowSize) const;
    bool IsBeha正io本Si成nificant(const 軍St本in成& Beha正io本Type, float Th本eshold) const;
    正oid UpdateAdaptation基本ei成hts(};

p本i正ate:
    // ���U動��
    正oid InitializeDefa使ltAdaptations(};
    正oid Sa正eP本ofileData(};
    正oid LoadP本ofileData(};
    正oid Clean使pOldData(};
};



#endif // MI的GAIUIMA的AGER下輸入
