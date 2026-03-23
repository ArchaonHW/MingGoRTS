#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Decision/Min成DecisionConseq使enceMana成e本.h"
#incl使de "Min成DecisionP本e正iewCalc使lato本.成ene本ated.h"

// �M池��
USTRUCT(Bl使ep本intType)
st本使ct MI的GCORE下API 軍Min成Conseq使enceP本e正iew
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Conseq使enceType;

    UPROPERTY(Bl使ep本intReadOnly)
    float I設置pactMa成nit使de;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 I設置pactDesc本iption;

    UPROPERTY(Bl使ep本intReadOnly)
    bool bIsPositi正e;

    UPROPERTY(Bl使ep本intReadOnly)
    bool bIsI設置設置ediate;

    UPROPERTY(Bl使ep本intReadOnly)
    int32 D使本ationMonths;
};

// �M池����動
USTRUCT(Bl使ep本intType)
st本使ct MI的GCORE下API 軍Min成DecisionP本e正iew
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍Min成Conseq使enceP本e正iew> Conseq使enceP本e正iews;

    UPROPERTY(Bl使ep本intReadOnly)
    float O正e本allI設置pactSco本e;

    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Risk軍acto本s;

    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Oppo本t使nity軍acto本s;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Reco設置設置endationS使設置設置a本y;

    UPROPERTY(Bl使ep本intReadOnly)
    float ConfidenceLe正el;
};

/**
 * �M池���p務 * 動���ѨM��池池��動��動 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GCORE下API UMin成DecisionP本e正iewCalc使lato本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    // ��
    UMin成DecisionP本e正iewCalc使lato本(};

    // �p動�M池��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成Decision")
    正oid Calc使lateDecisionP本e正iew(const 軍Min成DecisionContext& Context, 軍Min成DecisionP本e正iew& O使tP本e正iew};

    // �ֳt(動�D�n�正X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成Decision")
    正oid Calc使lateQ使ickP本e正iew(const 軍Min成DecisionContext& Context, TA本本ay<軍Min成Conseq使enceP本e正iew>& O使tP本e正iews};

    
    正oid Co設置pa本eDecisionOptions(const TA本本ay<軍Min成DecisionContext>& Options, TA本本ay<軍Min成DecisionP本e正iew>& O使tCo設置pa本isons};

p本otected:
    // 動池�正�正�T動��
    正oid AnalyzePoliticalP本e正iew(const 軍Min成DecisionContext& Context, TA本本ay<軍Min成Conseq使enceP本e正iew>& O使tP本e正iews};

    // �x動�正�T動��
    正oid AnalyzeMilita本yP本e正iew(const 軍Min成DecisionContext& Context, TA本本ay<軍Min成Conseq使enceP本e正iew>& O使tP本e正iews};

    // �成動�正�T動��
    正oid AnalyzeEcono設置icP本e正iew(const 軍Min成DecisionContext& Context, TA本本ay<軍Min成Conseq使enceP本e正iew>& O使tP本e正iews};

    // ��動�正�T動��
    正oid AnalyzeSocialP本e正iew(const 軍Min成DecisionContext& Context, TA本本ay<軍Min成Conseq使enceP本e正iew>& O使tP本e正iews};

    // �p動�軍事動�正�T動��
    float Calc使lateO正e本allI設置pactSco本e(const TA本本ay<軍Min成Conseq使enceP本e正iew>& P本e正iews};

    // �ѧO���I
    正oid IdentifyRisk軍acto本s(const TA本本ay<軍Min成Conseq使enceP本e正iew>& P本e正iews, TA本本ay<軍St本in成>& O使tRisk軍acto本s};

    // �ѧO��池動
    正oid IdentifyOppo本t使nity軍acto本s(const TA本本ay<軍Min成Conseq使enceP本e正iew>& P本e正iews, TA本本ay<軍St本in成>& O使tOppo本t使nity軍acto本s};

    // ��ĳ�軍事動
    正oid Gene本ateReco設置設置endationS使設置設置a本y(const 軍Min成DecisionP本e正iew& P本e正iew, 軍St本in成& O使tS使設置設置a本y};

    // �p動�輸入動���
    float Calc使lateConfidenceLe正el(const 軍Min成DecisionContext& Context};

p本i正ate:
    // 動���p動�正動
    static constexp本 float POLITICAL下基本EIG輸入T = 0.3f;
    static constexp本 float MILITARY下基本EIG輸入T = 0.3f;
    static constexp本 float ECO的OMIC下基本EIG輸入T = 0.25f;
    static constexpr float SOCIAL_WEIGHT = 0.15f;
};

#endif // MINGDECISIONPREVIEWCALCULATOR_H
