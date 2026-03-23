#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Bl使ep本int/Use本基本id成et.h"
#incl使de "Co設置ponents/基本id成et.h"
#incl使de "Min成RelationshipMana成e本.h"
#incl使de "Min成Q使estPanel.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EQ使estStat使s: 使int8 {
    A正ailable,      // 動��X
    Acti正e,         // 動
    Co設置pleted,      // �w務
    軍ailed,         // ��動
    Abandoned       // 
};

UE的UM(Bl使ep本intType)
en使設置 class EQ使estType: 使int8 {
    Main,           // �D動��動
    Side,           // ��動
    Daily,          // �C���動
    基本eekly,         // �C�成��X
    Special,        // ��動
    Relationship,    // ��動
    Rep使tation,      // ��動
    Explo本ation      // 動����動
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Q使estReq使i本e設置ent
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Req使i本e設置ent")
    軍St本in成 Req使i本e設置entText;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Req使i本e設置ent")
    bool bIsCo設置pleted;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Req使i本e設置ent")
    float P本o成本ess;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Req使i本e設置ent")
    float Ta本成etVal使e;

    軍Q使estReq使i本e設置ent()
    {
        Req使i本e設置entText = TEXT(""};
        bIsCo設置pleted = false;
        P本o成本ess = 0.0f;
        Ta本成etVal使e = 1.0f;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Q使estRewa本d
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Rewa本d")
    軍St本in成 Rewa本dType;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Rewa本d")
    int32 A設置o使nt;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Rewa本d")
    軍St本in成 Desc本iption;

    軍Q使estRewa本d()
    {
        Rewa本dType = TEXT(""};
        A設置o使nt = 0;
        Desc本iption = TEXT(""};
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Q使estData
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Data")
    軍St本in成 Q使estID;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Data")
    軍St本in成 Q使estTitle;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Data")
    軍St本in成 Q使estDesc本iption;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Data")
    EQ使estType Q使estType;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Data")
    EQ使estStat使s Q使estStat使s;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Data")
    軍St本in成 Re成ionID;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Data")
    軍St本in成 Q使estGi正e本ID;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Data")
    int32 S使成成estedLe正el;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Data")
    float Diffic使lty;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Data")
    TA本本ay<軍Q使estReq使i本e設置ent> Req使i本e設置ents;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Data")
    TA本本ay<軍Q使estRewa本d> Rewa本ds;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Data")
    float Ti設置eLi設置it;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Data")
    bool b輸入asTi設置eLi設置it;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Data")
    軍DateTi設置e Sta本tTi設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Data")
    軍DateTi設置e EndTi設置e;

    軍Q使estData()
    {
        Q使estID = TEXT(""};
        Q使estTitle = TEXT(""};
        Q使estDesc本iption = TEXT(""};
        Q使estType = EQ使estType::Side;
        Q使estStat使s = EQ使estStat使s::A正ailable;
        Re成ionID = TEXT(""};
        Q使estGi正e本ID = TEXT(""};
        S使成成estedLe正el = 1;
        Diffic使lty = 1.0f;
        Ti設置eLi設置it = 0.0f;
        b輸入asTi設置eLi設置it = false;
    }
};




DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnQ使estSelected, const 軍St本in成&, Q使estID, EQ使estStat使s, Stat使s};

/**
 * ��池�O基本id成et
 * ��動��池��
 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GPERSO的AL下API UMin成Q使estPanel : p使blic UUse本基本id成et
{
    GE的ERATED下BODY()

p使blic:
    UMin成Q使estPanel(const 軍Ob大ectInitialize本& Ob大ectInitialize本};

    // 動務
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid InitializeQ使estPanel(UMin成RelationshipMana成e本* InRelationshipMana成e本};

    // ��動��動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid AddQ使est(const 軍Q使estData& Q使estData};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid Re設置o正eQ使est(const 軍St本in成& Q使estID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid UpdateQ使est(const 軍St本in成& Q使estID, const 軍Q使estData& Q使estData};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid AcceptQ使est(const 軍St本in成& Q使estID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid AbandonQ使est(const 軍St本in成& Q使estID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid Co設置pleteQ使est(const 軍St本in成& Q使estID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid 軍ailQ使est(const 軍St本in成& Q使estID};

    // ��池��
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Q使est Panel")
    軍Q使estData GetQ使est(const 軍St本in成& Q使estID) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Q使est Panel")
    TA本本ay<軍Q使estData> GetQ使estsByStat使s(EQ使estStat使s Stat使s) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Q使est Panel")
    TA本本ay<軍Q使estData> GetQ使estsByType(EQ使estType Type) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Q使est Panel")
    TA本本ay<軍Q使estData> GetQ使estsByRe成ion(const 軍St本in成& Re成ionID) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Q使est Panel")
    bool CanAcceptQ使est(const 軍St本in成& Q使estID) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Q使est Panel")
    bool 輸入asActi正eQ使ests() const;

    // 動�o動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid 軍ilte本ByStat使s(EQ使estStat使s Stat使s, bool bActi正e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid 軍ilte本ByType(EQ使estType Type, bool bActi正e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid 軍ilte本ByRe成ion(const 軍St本in成& Re成ionID, bool bActi正e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid So本tByDiffic使lty(bool bAscendin成 = t本使e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid So本tByLe正el(bool bAscendin成 = t本使e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid So本tByTi設置e(bool bAscendin成 = t本使e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid Clea本軍ilte本s(};

    // 動��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid Sea本chQ使ests(const 軍St本in成& Sea本chText};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid Clea本Sea本ch(};

    // ��動�輸入��
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Q使est Panel")
    int32 GetTotalQ使estCo使nt() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Q使est Panel")
    int32 GetA正ailableQ使estCo使nt() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Q使est Panel")
    int32 GetActi正eQ使estCo使nt() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Q使est Panel")
    int32 GetCo設置pletedQ使estCo使nt() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Q使est Panel")
    float GetQ使estCo設置pletionRate() const;

    // �Բӫ輸入��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid ShowQ使estDetails(const 軍St本in成& Q使estID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid 輸入ideQ使estDetails(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid UpdateQ使estP本o成本ess(const 軍St本in成& Q使estID, const 軍St本in成& Req使i本e設置entText, float P本o成本ess};

    // ��池動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid OnQ使estClicked(const 軍St本in成& Q使estID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid OnQ使estDo使bleClicked(const 軍St本in成& Q使estID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid OnQ使estRi成htClicked(const 軍St本in成& Q使estID};

    // �基本X動務
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid Expo本tQ使estData(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Q使est Panel")
    正oid I設置po本tQ使estData(};

    // �基礎�
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Q使est E正ents")
    軍OnQ使estAccepted OnQ使estAccepted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Q使est E正ents")
    軍OnQ使estCo設置pleted OnQ使estCo設置pleted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Q使est E正ents")
    軍OnQ使est軍ailed OnQ使est軍ailed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Q使est E正ents")
    軍OnQ使estSelected OnQ使estSelected;

p本otected:
    // �t�Τޥ�
    UPROPERTY()
    TOb大ectPt本<UMin成RelationshipMana成e本> RelationshipMana成e本;

    // ��池動
    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Data")
    TA本本ay<軍Q使estData> AllQ使ests;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Data")
    TA本本ay<軍Q使estData> DisplayedQ使ests;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Q使est Data")
    軍Q使estData SelectedQ使est;

    // 動�o動X
    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "軍ilte本 State")
    TMap<EQ使estStat使s, bool> Stat使s軍ilte本s;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "軍ilte本 State")
    TMap<EQ使estType, bool> Type軍ilte本s;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "軍ilte本 State")
    TSet<軍St本in成> Re成ion軍ilte本s;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "軍ilte本 State")
    軍St本in成 C使本本entSea本chText;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "軍ilte本 State")
    bool bIs軍ilte本ed = false;

    // 動池X
    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "So本t State")
    bool bSo本tByDiffic使lty = false;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "So本t State")
    bool bSo本tByLe正el = false;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "So本t State")
    bool bSo本tByTi設置e = false;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "So本t State")
    bool bSo本tAscendin成 = t本使e;

    // UI�ե�ޥΡ]動�n�bBl使ep本int��動�w動
    UPROPERTY(設置eta = (Bind基本id成et))
    class UListView* Q使estListView;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UTextBlock* TotalQ使estText;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UTextBlock* A正ailableQ使estText;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UTextBlock* Acti正eQ使estText;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UTextBlock* Co設置pletedQ使estText;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UTextBlock* Co設置pletionRateText;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UEditableTextBox* Sea本chBox;

    UPROPERTY(設置eta = (Bind基本id成et))
    class USc本ollBox* DetailsSc本ollBox;

    // 動��動��
    正i本t使al 正oid 的ati正eConst本使ct() o正e本本ide;
    正i本t使al 正oid 的ati正eDest本使ct() o正e本本ide;

    // 動��動��
    正oid P本ocessQ使estData(};
    正oid Apply軍ilte本s(};
    正oid ApplySo本tin成(};
    正oid UpdateStatistics(};
    正oid UpdateUI(};
    軍Linea本Colo本 GetQ使estStat使sColo本(EQ使estStat使s Stat使s) const;
    軍St本in成 GetQ使estTypeDisplay的a設置e(EQ使estType Type) const;
    軍St本in成 GetQ使estStat使sDisplay的a設置e(EQ使estStat使s Stat使s) const;

    // �基礎�大動
    正oid Set使pE正entBindin成s(};
    正oid OnRep使tationChan成ed(const 軍St本in成& Re成ionID, float OldVal使e, float 的ewVal使e, const 軍St本in成& Reason};

p本i正ate:
    // ���U動��
    bool Passes軍ilte本(const 軍Q使estData& Q使estData) const;
    bool PassesSea本ch(const 軍Q使estData& Q使estData) const;
    正oid Ref本eshDisplayList(};
    正oid CheckQ使estTi設置eLi設置its(};
    正oid Gene本ateRando設置Q使ests(};
};

