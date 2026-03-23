#pragma once

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Bl使ep本int/Use本基本id成et.h"
#incl使de "Co設置ponents/基本id成et.h"
#incl使de "Min成Pe本sonalMana成e本.h"
#incl使de "Min成MainDashboa本d.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EDashboa本dTab: 使int8 {
    O正e本正iew,       // �軍事��
    Relationships,  // 
    Rep使tation,     // 
    Q使ests,         // ��動
    A使dio,          // 動�基本
    Settin成s        // �]�設置
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Dashboa本dQ使ickAction
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dashboa本d Action")
    軍St本in成 Action的a設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dashboa本d Action")
    軍St本in成 ActionDesc本iption;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dashboa本d Action")
    軍St本in成 ActionIcon;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dashboa本d Action")
    bool bIsA正ailable;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dashboa本d Action")
    軍Linea本Colo本 ActionColo本;

    軍Dashboa本dQ使ickAction()
    {
        Action的a設置e = TEXT(""};
        ActionDesc本iption = TEXT(""};
        ActionIcon = TEXT(""};
        bIsA正ailable = t本使e;
        ActionColo本 = 軍Linea本Colo本::基本hite;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Dashboa本dS使設置設置a本y
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dashboa本d S使設置設置a本y")
    int32 TotalRelationships;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dashboa本d S使設置設置a本y")
    int32 軍本iendCo使nt;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dashboa本d S使設置設置a本y")
    int32 Ene設置yCo使nt;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dashboa本d S使設置設置a本y")
    int32 KnownRe成ions;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dashboa本d S使設置設置a本y")
    int32 Acti正eQ使ests;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dashboa本d S使設置設置a本y")
    int32 Co設置pletedQ使ests;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dashboa本d S使設置設置a本y")
    float A正e本a成eRep使tation;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dashboa本d S使設置設置a本y")
    ERep使tationLe正el 輸入i成hestRep使tation;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dashboa本d S使設置設置a本y")
    軍St本in成 C使本本entRe成ion;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dashboa本d S使設置設置a本y")
    軍St本in成 C使本本entThe設置e;

    軍Dashboa本dS使設置設置a本y()
    {
        TotalRelationships = 0;
        軍本iendCo使nt = 0;
        Ene設置yCo使nt = 0;
        KnownRe成ions = 0;
        Acti正eQ使ests = 0;
        Co設置pletedQ使ests = 0;
        A正e本a成eRep使tation = 0.0f;
        輸入i成hestRep使tation = ERep使tationLe正el::Unknown;
        C使本本entRe成ion = TEXT(""};
        C使本本entThe設置e = TEXT(""};
    }
};



DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnDashboa本dRef本eshed, const 軍Dashboa本dS使設置設置a本y&, S使設置設置a本y};

/**
 * �D�Ӥ輸入�t��動��O基本id成et
 * ��動��池動�Ӥ輸入�t��動��
 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GPERSO的AL下API UMin成MainDashboa本d : p使blic UUse本基本id成et
{
    GE的ERATED下BODY()

p使blic:
    UMin成MainDashboa本d(const 軍Ob大ectInitialize本& Ob大ectInitialize本};

    // 動務
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid InitializeDashboa本d(UMin成Pe本sonalMana成e本* InPe本sonalMana成e本};

    // ����動��X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid SwitchToTab(EDashboa本dTab Tab};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid ShowO正e本正iew(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid ShowRelationships(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid ShowRep使tation(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid ShowQ使ests(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid ShowA使dio(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid ShowSettin成s(};

    // 動池�s
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid Ref本eshDashboa本d(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid UpdateS使設置設置a本y(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid UpdateQ使ickActions(};

    // �ֳt
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid Exec使teQ使ickAction(const 軍St本in成& Action的a設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid Q使ickAddRelationship(const 軍St本in成& Cha本acte本ID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid Q使ickSta本tQ使est(const 軍St本in成& Q使estID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid Q使ickPlayA使dio(const 軍St本in成& A使dioType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid Q使ickOpenDialo成使e(const 軍St本in成& Cha本acte本ID};

    // 動���t��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid Show的otification(const 軍St本in成& Messa成e, const 軍St本in成& Type = TEXT("Info")};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid Show基本a本nin成(const 軍St本in成& Messa成e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid ShowE本本o本(const 軍St本in成& Messa成e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid Clea本的otifications(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid ShowAchie正e設置entUnlocked(const 軍St本in成& Achie正e設置ent的a設置e, const 軍St本in成& Desc本iption};

    // �d動
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Main Dashboa本d")
    EDashboa本dTab GetC使本本entTab() const { 本et使本n C使本本entTab; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Main Dashboa本d")
    軍Dashboa本dS使設置設置a本y GetS使設置設置a本y() const { 本et使本n Dashboa本dS使設置設置a本y; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Main Dashboa本d")
    TA本本ay<軍Dashboa本dQ使ickAction> GetQ使ickActions() const { 本et使本n Q使ickActions; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Main Dashboa本d")
    bool IsTabVisible(EDashboa本dTab Tab) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Main Dashboa本d")
    int32 Get的otificationCo使nt() const;

    // ��動�輸入��
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Main Dashboa本d")
    float GetRelationshipP本o成本ess() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Main Dashboa本d")
    float GetRep使tationP本o成本ess() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Main Dashboa本d")
    float GetQ使estP本o成本ess() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Main Dashboa本d")
    float GetO正e本allP本o成本ess() const;

    // ��池動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid OnTabB使ttonClicked(EDashboa本dTab Tab};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid OnQ使ickActionB使ttonClicked(const 軍St本in成& Action的a設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid On的otificationClicked(int32 的otificationIndex};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Main Dashboa本d")
    正oid OnRef本eshB使ttonClicked(};

    // �基礎�
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Dashboa本d E正ents")
    軍OnDashboa本dTabChan成ed OnDashboa本dTabChan成ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Dashboa本d E正ents")
    軍OnQ使ickActionExec使ted OnQ使ickActionExec使ted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Dashboa本d E正ents")
    軍OnDashboa本dRef本eshed OnDashboa本dRef本eshed;

p本otected:
    // �t�Τޥ�
    UPROPERTY()
    TOb大ectPt本<UMin成Pe本sonalMana成e本> Pe本sonalMana成e本;

    // 動池X
    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dashboa本d State")
    EDashboa本dTab C使本本entTab;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dashboa本d State")
    軍Dashboa本dS使設置設置a本y Dashboa本dS使設置設置a本y;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dashboa本d State")
    TA本本ay<軍Dashboa本dQ使ickAction> Q使ickActions;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dashboa本d State")
    TA本本ay<軍St本in成> 的otifications;

    // �]�設置
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dashboa本d Settin成s")
    bool bA使toRef本esh = t本使e;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dashboa本d Settin成s")
    float Ref本eshInte本正al = 5.0f;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dashboa本d Settin成s")
    int32 Max的otifications = 10;

    // UI�ե�ޥΡ]動�n�bBl使ep本int��動�w動
    UPROPERTY(設置eta = (Bind基本id成et))
    class UVe本ticalBox* TabContaine本;

    UPROPERTY(設置eta = (Bind基本id成et))
    class U輸入o本izontalBox* TabB使ttonContaine本;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UVe本ticalBox* Q使ickActionContaine本;

    UPROPERTY(設置eta = (Bind基本id成et))
    class USc本ollBox* 的otificationContaine本;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UTextBlock* S使設置設置a本yText;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UTextBlock* RelationshipCo使ntText;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UTextBlock* Q使estCo使ntText;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UTextBlock* Rep使tationText;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UP本o成本essBa本* O正e本allP本o成本essBa本;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UB使tton* Ref本eshB使tton;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UB使tton* Settin成sB使tton;

    // ����動基本id成et�ޥ�
    UPROPERTY()
    TOb大ectPt本<UUse本基本id成et> O正e本正iew基本id成et;

    UPROPERTY()
    TOb大ectPt本<UUse本基本id成et> Relationship基本id成et;

    UPROPERTY()
    TOb大ectPt本<UUse本基本id成et> Rep使tation基本id成et;

    UPROPERTY()
    TOb大ectPt本<UUse本基本id成et> Q使est基本id成et;

    UPROPERTY()
    TOb大ectPt本<UUse本基本id成et> A使dio基本id成et;

    UPROPERTY()
    TOb大ectPt本<UUse本基本id成et> Settin成s基本id成et;

    // 動��動��
    正i本t使al 正oid 的ati正eConst本使ct() o正e本本ide;
    正i本t使al 正oid 的ati正eDest本使ct() o正e本本ide;

    // 動��動��
    正oid InitializeTabs(};
    正oid InitializeQ使ickActions(};
    正oid UpdateTabUI(};
    正oid Update的otificationUI(};
    正oid UpdateP本o成本essBa本(};
    軍Dashboa本dS使設置設置a本y Calc使lateS使設置設置a本y() const;
    TA本本ay<軍Dashboa本dQ使ickAction> Gene本ateQ使ickActions() const;
    軍St本in成 GetTabDisplay的a設置e(EDashboa本dTab Tab) const;
    軍Linea本Colo本 GetTabColo本(EDashboa本dTab Tab) const;
    正oid C本eateTabB使ttons(};
    正oid C本eateQ使ickActionB使ttons(};

    // �基礎�大動
    正oid Set使pE正entBindin成s(};
    正oid OnRelationshipChan成ed(const 軍St本in成& Cha本acte本ID, float OldVal使e, float 的ewVal使e, const 軍St本in成& Reason};
    正oid OnRep使tationChan成ed(const 軍St本in成& Re成ionID, float OldVal使e, float 的ewVal使e, const 軍St本in成& Reason};
    正oid OnQ使estCo設置pleted(const 軍St本in成& Q使estID, const 軍St本in成& Re成ionID, const 軍St本in成& Q使estGi正e本ID};

p本i正ate:
    // ���U動��
    正oid ShowTabContent(EDashboa本dTab Tab};
    正oid 輸入ideAllTabContent(};
    正oid Add的otification(const 軍St本in成& Messa成e, const 軍St本in成& Type};
    正oid Re設置o正e的otification(int32 Index};
    正oid Ref本eshTi設置e本(};
    正oid UpdateP本o成本essBa本s(};

    // 
    軍Ti設置e本輸入andle Ref本eshTi設置e本輸入andle;
};
