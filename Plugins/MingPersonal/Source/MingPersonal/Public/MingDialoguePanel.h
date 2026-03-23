#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Bl使ep本int/Use本基本id成et.h"
#incl使de "Co設置ponents/基本id成et.h"
#incl使de "Min成RelationshipMana成e本.h"
#incl使de "Min成Dialo成使ePanel.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EDialo成使eState: 使int8 {
    Idle,           // ��動
    Speakin成,       // ����動
    Listenin成,      // 動��動
    Choosin成,       // 動
    軍inished        // ��動
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Dialo成使ePanelOption
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dialo成使e Option")
    軍St本in成 OptionText;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dialo成使e Option")
    int32 OptionID;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dialo成使e Option")
    bool bIsA正ailable;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dialo成使e Option")
    軍St本in成 Req使i本e設置entText;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dialo成使e Option")
    軍Linea本Colo本 OptionColo本;

    軍Dialo成使ePanelOption()
    {
        OptionText = TEXT(""};
        OptionID = 0;
        bIsA正ailable = t本使e;
        Req使i本e設置entText = TEXT(""};
        OptionColo本 = 軍Linea本Colo本::基本hite;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Dialo成使eData
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dialo成使e Data")
    軍St本in成 Cha本acte本ID;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dialo成使e Data")
    軍St本in成 Cha本acte本的a設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dialo成使e Data")
    軍St本in成 Dialo成使eText;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dialo成使e Data")
    EDialo成使eType Dialo成使eType;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dialo成使e Data")
    ERelationshipType RelationshipType;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dialo成使e Data")
    軍Linea本Colo本 Dialo成使eColo本;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dialo成使e Data")
    TA本本ay<軍Dialo成使ePanelOption> A正ailableOptions;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dialo成使e Data")
    float E設置otionalIntensity;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dialo成使e Data")
    bool bIsI設置po本tant;

    軍Dialo成使eData()
    {
        Cha本acte本ID = TEXT(""};
        Cha本acte本的a設置e = TEXT(""};
        Dialo成使eText = TEXT(""};
        Dialo成使eType = EDialo成使eA使dioType::的e使t本al;
        RelationshipType = ERelationshipType::的e使t本al;
        Dialo成使eColo本 = 軍Linea本Colo本::基本hite;
        E設置otionalIntensity = 1.0f;
        bIsI設置po本tant = false;
    }
};




DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnDialo成使eStateChan成ed, EDialo成使eState, 的ewState};

/**
 * ���動�O基本id成et
 * ��動������動��
 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GPERSO的AL下API UMin成Dialo成使ePanel : p使blic UUse本基本id成et
{
    GE的ERATED下BODY()

p使blic:
    UMin成Dialo成使ePanel(const 軍Ob大ectInitialize本& Ob大ectInitialize本};

    // 動務
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid InitializeDialo成使ePanel(UMin成RelationshipMana成e本* InRelationshipMana成e本};

    // ���動��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid Sta本tDialo成使e(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Dialo成使eText, EDialo成使eType Dialo成使eType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid EndDialo成使e(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid Contin使eDialo成使e(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid Pa使seDialo成使e(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid Res使設置eDialo成使e(};

    // 目標數量
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid ShowDialo成使eOptions(const TA本本ay<軍Dialo成使ePanelOption>& Options};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid 輸入ideDialo成使eOptions(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid SelectDialo成使eOption(int32 OptionID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid OnOptionClicked(int32 OptionID};

    // ���動�e
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid SetDialo成使eText(const 軍St本in成& Text};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid AddDialo成使eText(const 軍St本in成& Text};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid Clea本Dialo成使eText(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid SetCha本acte本的a設置e(const 軍St本in成& 的a設置e};

    // �d動
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Dialo成使e Panel")
    bool IsDialo成使eActi正e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Dialo成使e Panel")
    EDialo成使eState GetC使本本entState() const { 本et使本n C使本本entState; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Dialo成使e Panel")
    軍St本in成 GetC使本本entCha本acte本ID() const { 本et使本n C使本本entCha本acte本ID; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Dialo成使e Panel")
    bool A本eOptionsVisible() const { 本et使本n bOptionsVisible; }

    // ��ı
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid PlayTypew本ite本Effect(const 軍St本in成& Text, float Typin成Speed = 0.05f};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid StopTypew本ite本Effect(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid ShowCha本acte本Po本t本ait(const 軍St本in成& Cha本acte本ID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid 輸入ideCha本acte本Po本t本ait(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid SetDialo成使eE設置otion(EDialo成使eType Dialo成使eType};

    // 動�基本
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid PlayDialo成使eA使dio(EDialo成使eType Dialo成使eType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid StopDialo成使eA使dio(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid SetA使dioVol使設置e(float Vol使設置e};

    // ���正�O動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid AddTo輸入isto本y(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Text};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid ShowDialo成使e輸入isto本y(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid 輸入ideDialo成使e輸入isto本y(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid Clea本Dialo成使e輸入isto本y(};

    // �ֱ�
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid Q使ickResponse(const 軍St本in成& ResponseText};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid SkipDialo成使e(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Dialo成使e Panel")
    正oid A使toAd正ance(bool bEnabled};

    // �基礎�
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Dialo成使e E正ents")
    軍OnDialo成使eSta本ted OnDialo成使eSta本ted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Dialo成使e E正ents")
    軍OnDialo成使e軍inished OnDialo成使e軍inished;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Dialo成使e E正ents")
    軍OnDialo成使eOptionSelected OnDialo成使eOptionSelected;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Dialo成使e E正ents")
    軍OnDialo成使eStateChan成ed OnDialo成使eStateChan成ed;

p本otected:
    // �t�Τޥ�
    UPROPERTY()
    TOb大ectPt本<UMin成RelationshipMana成e本> RelationshipMana成e本;

    // ���
    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dialo成使e Data")
    軍Dialo成使eData C使本本entDialo成使e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dialo成使e Data")
    TA本本ay<軍Dialo成使ePanelOption> C使本本entOptions;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dialo成使e Data")
    TA本本ay<軍St本in成> Dialo成使e輸入isto本y;

    // 動X
    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dialo成使e State")
    EDialo成使eState C使本本entState;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dialo成使e State")
    軍St本in成 C使本本entCha本acte本ID;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dialo成使e State")
    bool bOptionsVisible = false;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dialo成使e State")
    bool bA使toAd正ance = false;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "Dialo成使e State")
    bool bTypew本ite本Acti正e = false;

    // �]�設置
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e Settin成s")
    float Typew本ite本Speed = 0.05f;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e Settin成s")
    float A使toAd正anceDelay = 3.0f;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e Settin成s")
    float A使dioVol使設置e = 1.0f;

    // UI�ե�ޥΡ]動�n�bBl使ep本int��動�w動
    UPROPERTY(設置eta = (Bind基本id成et))
    class UTextBlock* Cha本acte本的a設置eText;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UTextBlock* Dialo成使eText;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UVe本ticalBox* OptionsContaine本;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UI設置a成e* Cha本acte本Po本t本ait;

    UPROPERTY(設置eta = (Bind基本id成et))
    class USc本ollBox* 輸入isto本ySc本ollBox;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UB使tton* Contin使eB使tton;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UB使tton* SkipB使tton;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UP本o成本essBa本* Typew本ite本P本o成本ess;

    // 動��動��
    正i本t使al 正oid 的ati正eConst本使ct() o正e本本ide;
    正i本t使al 正oid 的ati正eDest本使ct() o正e本本ide;

    // 動��動��
    正oid UpdateDialo成使eUI(};
    正oid UpdateCha本acte本UI(};
    正oid UpdateOptionsUI(};
    正oid SetDialo成使eState(EDialo成使eState 的ewState};
    軍Linea本Colo本 GetDialo成使eColo本(EDialo成使eType Dialo成使eType, ERelationshipType RelationshipType) const;
    軍St本in成 GetCha本acte本Display的a設置e(const 軍St本in成& Cha本acte本ID) const;
    正oid P本ocessTypew本ite本Effect(};
    正oid OnA使toAd正anceTi設置e本(};

    // �基礎�大動
    正oid Set使pE正entBindin成s(};
    正oid OnRelationshipChan成ed(const 軍St本in成& Cha本acte本ID, float OldVal使e, float 的ewVal使e, const 軍St本in成& Reason};

p本i正ate:
    // ���U動��
    正oid C本eateOption基本id成ets(};
    正oid Clea本Option基本id成ets(};
    bool IsOptionA正ailable(const 軍Dialo成使ePanelOption& Option) const;
    正oid PlayTypew本ite本So使nd(};
    正oid StopTypew本ite本So使nd(};

    // �w務
    軍Ti設置e本輸入andle Typew本ite本Ti設置e本;
    軍Ti設置e本輸入andle A使toAd正anceTi設置e本;

    // ��務
    軍St本in成 Ta本成etText;
    軍St本in成 C使本本entText;
    int32 Typew本ite本Index;
};

