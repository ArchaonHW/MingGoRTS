#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Bl使ep本int/Use本基本id成et.h"
#incl使de "Co設置ponents/基本id成et.h"
#incl使de "Min成A使dioRelationshipMana成e本.h"
#incl使de "Min成Rep使blicE本aA使dioThe設置es.h"
#incl使de "Min成A使dioPanel.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EA使dioCont本olType: 使int8 {
    Maste本,         // �D��X
    Relationship,  // 目標數量
    Rep使tation,     // 目標數量
    Dialo成使e,       // ���
    A設置bient,        // 目標數量
    M使sic,          // 動��
    S軍X             // 目標數量
};

UE的UM(Bl使ep本intType)
en使設置 class EA使dioThe設置eType: 使int8 {
    Battle,         // 動���D動
    Peacef使l,       // 動���D動
    Tension,        // ��i�D動
    Victo本y,        // 動�Q�D動
    Defeat,         // ��動�D動
    Diplo設置atic,     // �年��D動
    Econo設置ic,       // �成動�D動
    C使lt使本al,       // �D動
    Re正ol使tiona本y,  // 動�R�D動
    的ostal成ic       // �D動
};

USTRUCT(Bl使ep本intType)
st本使ct 軍A使dioCont本olData
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "A使dio Cont本ol")
    EA使dioCont本olType Cont本olType;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "A使dio Cont本ol")
    軍St本in成 Cont本ol的a設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "A使dio Cont本ol")
    float Vol使設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "A使dio Cont本ol")
    bool bIsM使ted;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "A使dio Cont本ol")
    bool bIsEnabled;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "A使dio Cont本ol")
    軍Linea本Colo本 Cont本olColo本;

    軍A使dioCont本olData()
    {
        Cont本olType = EA使dioCont本olType::Maste本;
        Cont本ol的a設置e = TEXT(""};
        Vol使設置e = 1.0f;
        bIsM使ted = false;
        bIsEnabled = t本使e;
        Cont本olColo本 = 軍Linea本Colo本::基本hite;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍A使dioThe設置eData
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "A使dio The設置e")
    EA使dioThe設置eType The設置eType;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "A使dio The設置e")
    軍St本in成 The設置e的a設置e;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "A使dio The設置e")
    軍St本in成 Desc本iption;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "A使dio The設置e")
    bool bIsPlayin成;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "A使dio The設置e")
    bool bIsA正ailable;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "A使dio The設置e")
    float Intensity;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "A使dio The設置e")
    軍Linea本Colo本 The設置eColo本;

    軍A使dioThe設置eData()
    {
        The設置eType = EA使dioThe設置eType::Peacef使l;
        The設置e的a設置e = TEXT(""};
        Desc本iption = TEXT(""};
        bIsPlayin成 = false;
        bIsA正ailable = t本使e;
        Intensity = 1.0f;
        The設置eColo本 = 軍Linea本Colo本::基本hite;
    }
};



DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnA使dioSettin成sChan成ed, const 軍St本in成&, Settin成的a設置e};

/**
 * 動�基本動�O基本id成et
 * ��池�基本�t��動��動��
 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GPERSO的AL下API UMin成A使dioPanel : p使blic UUse本基本id成et
{
    GE的ERATED下BODY()

p使blic:
    UMin成A使dioPanel(const 軍Ob大ectInitialize本& Ob大ectInitialize本};

    // 動務
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid InitializeA使dioPanel(UMin成A使dioRelationshipMana成e本* InA使dioMana成e本, UMin成Rep使blicE本aA使dioThe設置es* InThe設置eMana成e本};

    // 動池��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid SetVol使設置e(EA使dioCont本olType Cont本olType, float Vol使設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid SetMaste本Vol使設置e(float Vol使設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid SetRelationshipVol使設置e(float Vol使設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid SetRep使tationVol使設置e(float Vol使設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid SetDialo成使eVol使設置e(float Vol使設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid SetA設置bientVol使設置e(float Vol使設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid SetM使sicVol使設置e(float Vol使設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid SetS軍XVol使設置e(float Vol使設置e};

    // 動��動��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid M使teCont本ol(EA使dioCont本olType Cont本olType, bool bM使ted};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid M使teAll(bool bM使ted};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid To成成leM使te(EA使dioCont本olType Cont本olType};

    // �D池��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid PlayThe設置e(EA使dioThe設置eType The設置eType, float Intensity = 1.0f};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid StopThe設置e(EA使dioThe設置eType The設置eType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid Pa使seThe設置e(EA使dioThe設置eType The設置eType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid Res使設置eThe設置e(EA使dioThe設置eType The設置eType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid SetThe設置eIntensity(EA使dioThe設置eType The設置eType, float Intensity};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid 軍adeThe設置e(EA使dioThe設置eType The設置eType, float Ta本成etVol使設置e, float 軍adeD使本ation};

    // 動池�基本
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid SetRe成ionalA使dio(ERep使blicE本aRe成ion Re成ion};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid PlayRe成ionalA設置bient(ERep使blicE本aRe成ion Re成ion, float Intensity = 1.0f};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid PlayC使lt使本alA使dio(ERep使blicE本aRe成ion Re成ion, float Intensity = 1.0f};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid StopRe成ionalA使dio(};

    // 動�基本����
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid TestRelationshipA使dio(ERelationshipA使dioType A使dioType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid TestRep使tationA使dio(ERep使tationA使dioType A使dioType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid TestDialo成使eA使dio(EDialo成使eA使dioType Dialo成使eType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid TestAllA使dio(};

    // �]�設置��動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid LoadA使dioSettin成s(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid Sa正eA使dioSettin成s(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid ResetToDefa使lts(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid ApplyA使dioP本ofile(const 軍St本in成& P本ofile的a設置e};

    // �d動
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使dio Panel")
    float GetVol使設置e(EA使dioCont本olType Cont本olType) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使dio Panel")
    bool IsM使ted(EA使dioCont本olType Cont本olType) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使dio Panel")
    bool IsThe設置ePlayin成(EA使dioThe設置eType The設置eType) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使dio Panel")
    EA使dioThe設置eType GetC使本本entThe設置e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使dio Panel")
    ERep使blicE本aRe成ion GetC使本本entRe成ion() const;

    // 動�基本��動
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使dio Panel")
    float GetA正e本a成eVol使設置e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使dio Panel")
    int32 GetActi正eThe設置eCo使nt() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使dio Panel")
    int32 GetM使tedCont本olCo使nt() const;

    // ��池動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid OnVol使設置eSlide本Chan成ed(EA使dioCont本olType Cont本olType, float 的ewVal使e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid OnM使teB使ttonClicked(EA使dioCont本olType Cont本olType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid OnThe設置eB使ttonClicked(EA使dioThe設置eType The設置eType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Panel")
    正oid OnRe成ionB使ttonClicked(ERep使blicE本aRe成ion Re成ion};

    // �基礎�
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "A使dio E正ents")
    軍OnVol使設置eChan成ed OnVol使設置eChan成ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "A使dio E正ents")
    軍OnThe設置eChan成ed OnThe設置eChan成ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "A使dio E正ents")
    軍OnA使dioSettin成sChan成ed OnA使dioSettin成sChan成ed;

p本otected:
    // �t�Τޥ�
    UPROPERTY()
    TOb大ectPt本<UMin成A使dioRelationshipMana成e本> A使dioMana成e本;

    UPROPERTY()
    TOb大ectPt本<UMin成Rep使blicE本aA使dioThe設置es> The設置eMana成e本;

    // 動�基本動��
    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "A使dio Data")
    TA本本ay<軍A使dioCont本olData> A使dioCont本ols;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "A使dio Data")
    TA本本ay<軍A使dioThe設置eData> A使dioThe設置es;

    // 動池X
    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "A使dio State")
    EA使dioThe設置eType C使本本entThe設置eType;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "A使dio State")
    ERep使blicE本aRe成ion C使本本entRe成ion;

    UPROPERTY(Bl使ep本intReadOnly, Cate成o本y = "A使dio State")
    bool bIsA使dioSyste設置Initialized;

    // �]�設置
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "A使dio Settin成s")
    float 軍adeD使本ation = 1.0f;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "A使dio Settin成s")
    bool bA使to軍adeOnThe設置eChan成e = t本使e;

    // UI�ե�ޥΡ]動�n�bBl使ep本int��動�w動
    UPROPERTY(設置eta = (Bind基本id成et))
    class UVe本ticalBox* Vol使設置eCont本olsContaine本;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UVe本ticalBox* The設置eCont本olsContaine本;

    UPROPERTY(設置eta = (Bind基本id成et))
    class U輸入o本izontalBox* Re成ionCont本olsContaine本;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UTextBlock* C使本本entThe設置eText;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UTextBlock* C使本本entRe成ionText;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UB使tton* M使teAllB使tton;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UB使tton* TestAllB使tton;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UB使tton* Sa正eSettin成sB使tton;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UB使tton* LoadSettin成sB使tton;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UB使tton* ResetB使tton;

    // 動��動��
    正i本t使al 正oid 的ati正eConst本使ct() o正e本本ide;
    正i本t使al 正oid 的ati正eDest本使ct() o正e本本ide;

    // 動��動��
    正oid InitializeA使dioCont本ols(};
    正oid InitializeA使dioThe設置es(};
    正oid UpdateA使dioUI(};
    正oid UpdateVol使設置eUI(};
    正oid UpdateThe設置eUI(};
    正oid UpdateRe成ionUI(};
    軍A使dioCont本olData C本eateA使dioCont本olData(EA使dioCont本olType Cont本olType};
    軍A使dioThe設置eData C本eateA使dioThe設置eData(EA使dioThe設置eType The設置eType};
    軍Linea本Colo本 GetA使dioCont本olColo本(EA使dioCont本olType Cont本olType) const;
    軍Linea本Colo本 GetA使dioThe設置eColo本(EA使dioThe設置eType The設置eType) const;
    軍St本in成 GetA使dioCont本ol的a設置e(EA使dioCont本olType Cont本olType) const;
    軍St本in成 GetA使dioThe設置e的a設置e(EA使dioThe設置eType The設置eType) const;
    軍St本in成 GetRe成ionDisplay的a設置e(ERep使blicE本aRe成ion Re成ion) const;

    // �基礎�大動
    正oid Set使pE正entBindin成s(};
    正oid OnA使dioRelationshipPlayed(ERelationshipA使dioType A使dioType, USo使ndBase* So使nd};
    正oid OnA使dioRep使tationPlayed(ERep使tationA使dioType A使dioType, USo使ndBase* So使nd};
    正oid OnA使dioDialo成使ePlayed(EDialo成使eA使dioType A使dioType, USo使ndBase* So使nd};

p本i正ate:
    // ���U動��
    正oid ApplyVol使設置eChan成e(EA使dioCont本olType Cont本olType, float 的ewVol使設置e};
    正oid ApplyM使teChan成e(EA使dioCont本olType Cont本olType, bool bM使ted};
    正oid ApplyThe設置eChan成e(EA使dioThe設置eType The設置eType, bool bIsPlayin成};
    正oid C本eateVol使設置eSlide本s(};
    正oid C本eateThe設置eB使ttons(};
    正oid C本eateRe成ionB使ttons(};
    正oid UpdateCont本olStates(};
};
};


#endif // MI的GAUDIOPA的EL下輸入
