#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成RelationshipMana成e本.h"
#incl使de "Min成MetaSo使ndsSyste設置.h"
#incl使de "Min成A使dioRelationshipMana成e本.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class ERelationshipA使dioType: 使int8 {
    RelationshipInc本ease,    // 故事選項基本
    RelationshipDec本ease,    // �U池動
    RelationshipB本eak,       // 故事選項基本
    的ew軍本iendship,          // �˭�X
    Bet本ayal,               // 目標數量
    Reconciliation,         // 動��
    軍i本stMeetin成,           // 動������
    軍a本ewell                // 動�O
};

UE的UM(Bl使ep本intType)
en使設置 class ERep使tationA使dioType: 使int8 {
    Rep使tationInc本ease,     // 故事選項基本
    Rep使tationDec本ease,     // �U池動
    的ewRep使tationLe正el,     // 動�n�ŭ�X
    Le成enda本yAchie正e設置ent,   // 動池�的
    Re成ion基本elco設置e,          // �w池動
    Re成ion輸入ostility         // 故事選項基本
};

UE的UM(Bl使ep本intType)
en使設置 class EDialo成使eA使dioType: 使int8 {
    軍本iendly,               // 動�n���
    輸入ostile,                // ���
    的e使t本al,                // ��動���
    Ro設置antic,               // �������
    B使siness,               // 動�年���
    Inti設置idation,           // ��動���
    Pe本s使asion,             // ��池動
    Req使est                 // ��動���
};

USTRUCT(Bl使ep本intType)
st本使ct 軍RelationshipA使dioPa本a設置s
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Relationship A使dio")
    ERelationshipA使dioType A使dioType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Relationship A使dio")
    ERelationshipType RelationshipType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Relationship A使dio")
    float RelationshipChan成e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Relationship A使dio")
    float Intensity = 1.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Relationship A使dio")
    bool bIsSi成nificantChan成e = false;

    軍RelationshipA使dioPa本a設置s()
    {
        A使dioType = ERelationshipA使dioType::RelationshipInc本ease;
        RelationshipType = ERelationshipType::的e使t本al;
        RelationshipChan成e = 0.0f;
        Intensity = 1.0f;
        bIsSi成nificantChan成e = false;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Rep使tationA使dioPa本a設置s
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Rep使tation A使dio")
    ERep使tationA使dioType A使dioType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Rep使tation A使dio")
    ERep使tationLe正el C使本本entLe正el;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Rep使tation A使dio")
    ERep使tationLe正el P本e正io使sLe正el;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Rep使tation A使dio")
    float Rep使tationChan成e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Rep使tation A使dio")
    軍St本in成 Re成ionID;

    軍Rep使tationA使dioPa本a設置s()
    {
        A使dioType = ERep使tationA使dioType::Rep使tationInc本ease;
        C使本本entLe正el = ERep使tationLe正el::Unknown;
        P本e正io使sLe正el = ERep使tationLe正el::Unknown;
        Rep使tationChan成e = 0.0f;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Dialo成使eA使dioPa本a設置s
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e A使dio")
    EDialo成使eA使dioType Dialo成使eType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e A使dio")
    ERelationshipType RelationshipType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e A使dio")
    軍St本in成 Cha本acte本ID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e A使dio")
    float E設置otionalIntensity = 1.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e A使dio")
    bool bIsI設置po本tant = false;

    軍Dialo成使eA使dioPa本a設置s()
    {
        Dialo成使eType = EDialo成使eA使dioType::的e使t本al;
        RelationshipType = ERelationshipType::的e使t本al;
        E設置otionalIntensity = 1.0f;
        bIsI設置po本tant = false;
    }
};



DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnDialo成使eA使dioPlayed, EDialo成使eA使dioType, A使dioType, USo使ndBase*, So使nd};

/**
 * 動�基本��務 * �t�d動池�n動�t��池�基本
 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GPERSO的AL下API UMin成A使dioRelationshipMana成e本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成A使dioRelationshipMana成e本(};

    // 動池��動�t動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Relationship")
    正oid InitializeA使dioRelationshipSyste設置(UOb大ect* MetaSo使ndsSyste設置};

    // 動池�基本
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Relationship")
    正oid PlayRelationshipA使dio(const 軍RelationshipA使dioPa本a設置s& Pa本a設置s};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Relationship")
    正oid OnRelationshipChan成ed(const 軍St本in成& Cha本acte本ID, float OldVal使e, float 的ewVal使e, const 軍St本in成& Reason};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Relationship")
    正oid OnRelationshipTypeChan成ed(const 軍St本in成& Cha本acte本ID, ERelationshipType OldType, ERelationshipType 的ewType};

    // 動池�基本
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Rep使tation")
    正oid PlayRep使tationA使dio(const 軍Rep使tationA使dioPa本a設置s& Pa本a設置s};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Rep使tation")
    正oid OnRep使tationChan成ed(const 軍St本in成& Re成ionID, float OldVal使e, float 的ewVal使e, const 軍St本in成& Reason};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Rep使tation")
    正oid OnRep使tationLe正elChan成ed(const 軍St本in成& Re成ionID, ERep使tationLe正el OldLe正el, ERep使tationLe正el 的ewLe正el};

    // ���動�基本
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Dialo成使e")
    正oid PlayDialo成使eA使dio(const 軍Dialo成使eA使dioPa本a設置s& Pa本a設置s};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Dialo成使e")
    正oid OnDialo成使eSta本ted(const 軍St本in成& Cha本acte本ID, EDialo成使eA使dioType Dialo成使eType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Dialo成使e")
    正oid OnDialo成使eEnded(const 軍St本in成& Cha本acte本ID, bool b基本asS使ccessf使l};

    // ��池動�D池�基本
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio The設置e")
    正oid PlayRep使blicE本aThe設置e(const 軍St本in成& The設置e的a設置e, float Intensity = 1.0f};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio The設置e")
    正oid PlayRe成ionalA設置bient(const 軍St本in成& Re成ionID, ERep使tationLe正el Rep使tationLe正el};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio The設置e")
    正oid PlayCha本acte本The設置e(const 軍St本in成& Cha本acte本ID, ERelationshipType RelationshipType};

    // 動�基本�]�設置
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Settin成s")
    正oid SetA使dioIntensity(float Intensity};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Settin成s")
    正oid SetRelationshipA使dioVol使設置e(float Vol使設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Settin成s")
    正oid SetRep使tationA使dioVol使設置e(float Vol使設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio Settin成s")
    正oid SetDialo成使eA使dioVol使設置e(float Vol使設置e};

    // 動�基本�d動
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使dio Stat使s")
    bool IsA使dioSyste設置Initialized() const { 本et使本n bIsInitialized; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使dio Stat使s")
    float GetA使dioIntensity() const { 本et使本n A使dioIntensity; }

    // �基礎�
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "A使dio E正ents")
    軍OnRelationshipA使dioPlayed OnRelationshipA使dioPlayed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "A使dio E正ents")
    軍OnRep使tationA使dioPlayed OnRep使tationA使dioPlayed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "A使dio E正ents")
    軍OnDialo成使eA使dioPlayed OnDialo成使eA使dioPlayed;

p本otected:
    // 動�基本�t�Τޥ�
    UPROPERTY()
    TOb大ectPt本<UMin成MetaSo使ndsSyste設置> MetaSo使ndsSyste設置;

    // 動�基本�]�設置
    UPROPERTY()
    float A使dioIntensity = 1.0f;

    UPROPERTY()
    float RelationshipA使dioVol使設置e = 1.0f;

    UPROPERTY()
    float Rep使tationA使dioVol使設置e = 1.0f;

    UPROPERTY()
    float Dialo成使eA使dioVol使設置e = 1.0f;

    // 動X
    UPROPERTY()
    bool bIsInitialized = false;

    // 動��動�基本動池��
    USo使ndBase* Gene本ateRelationshipA使dio(const 軍RelationshipA使dioPa本a設置s& Pa本a設置s};
    USo使ndBase* Gene本ateRep使tationA使dio(const 軍Rep使tationA使dioPa本a設置s& Pa本a設置s};
    USo使ndBase* Gene本ateDialo成使eA使dio(const 軍Dialo成使eA使dioPa本a設置s& Pa本a設置s};
    USo使ndBase* Gene本ateRep使blicE本aA使dio(const 軍St本in成& The設置e的a設置e, float Intensity};

    // 動�基本動�ƭp動
    float Calc使lateRelationshipIntensity(float RelationshipChan成e, bool bIsSi成nificant) const;
    float Calc使lateRep使tationIntensity(float Rep使tationChan成e, ERep使tationLe正el Le正el) const;
    float Calc使lateDialo成使eIntensity(ERelationshipType RelationshipType, float E設置otionalIntensity) const;

    // 動�基本動��
    正oid PlayA使dioAtLocation(USo使ndBase* So使nd, 軍Vecto本 Location, float Vol使設置eM使ltiplie本 = 1.0f};
    正oid PlayA使dio2D(USo使ndBase* So使nd, float Vol使設置eM使ltiplie本 = 1.0f};

    // ��池池�基本�D動
    UPROPERTY()
    TMap<軍St本in成, 軍St本in成> Rep使blicE本aThe設置es;

    UPROPERTY()
    TMap<ERelationshipA使dioType, 軍St本in成> RelationshipA使dioThe設置es;

    UPROPERTY()
    TMap<ERep使tationA使dioType, 軍St本in成> Rep使tationA使dioThe設置es;

    UPROPERTY()
    TMap<EDialo成使eA使dioType, 軍St本in成> Dialo成使eA使dioThe設置es;

p本i正ate:
    // 動池�D動
    正oid InitializeRep使blicE本aThe設置es(};
    正oid InitializeRelationshipA使dioThe設置es(};
    正oid InitializeRep使tationA使dioThe設置es(};
    正oid InitializeDialo成使eA使dioThe設置es(};

    // 動池�基本�D動��動
    軍St本in成 GetRelationshipA使dioPath(ERelationshipA使dioType A使dioType) const;
    軍St本in成 GetRep使tationA使dioPath(ERep使tationA使dioType A使dioType) const;
    軍St本in成 GetDialo成使eA使dioPath(EDialo成使eA使dioType A使dioType) const;
    軍St本in成 GetRep使blicE本aThe設置ePath(const 軍St本in成& The設置e的a設置e) const;
};

