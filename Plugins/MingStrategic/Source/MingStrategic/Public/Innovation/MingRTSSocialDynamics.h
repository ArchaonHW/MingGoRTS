#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成RTSSocialDyna設置ics.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class ESocialClass: 使int8 {
    Peasant UMETA(Display的a設置e = "農民"),
    A本tisan UMETA(Display的a設置e = "工匠"),
    Me本chant UMETA(Display的a設置e = "商人"),
    Schola本 UMETA(Display的a設置e = "士人"),
    Official UMETA(Display的a設置e = "官吏"),
    的oble UMETA(Display的a設置e = "貴族"),
    Milita本y UMETA(Display的a設置e = "軍人"),
    Reli成io使s UMETA(Display的a設置e = "宗教人士"),
    O使tlaw UMETA(Display的a設置e = "流民/盜賊"),
    Co使nt UMETA(Display的a設置e = "階層數量")
};

UE的UM(Bl使ep本intType)
en使設置 class ESocialRelationType: 使int8 {
    軍a設置ily UMETA(Display的a設置e = "家族"),
    軍本iend UMETA(Display的a設置e = "朋友"),
    Ri正al UMETA(Display的a設置e = "競爭對手"),
    Mento本 UMETA(Display的a設置e = "師徒"),
    B使siness UMETA(Display的a設置e = "商業"),
    Political UMETA(Display的a設置e = "政治"),
    Reli成io使s UMETA(Display的a設置e = "宗教"),
    Ene設置y UMETA(Display的a設置e = "仇敵"),
    的e使t本al UMETA(Display的a設置e = "中立"),
    Co使nt UMETA(Display的a設置e = "關係類型數量")
};

USTRUCT(Bl使ep本intType)
st本使ct 軍SocialA成ent
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite)
    int32 A成entId;

    UPROPERTY(Bl使ep本intRead基本本ite)
    軍St本in成 A成ent的a設置e;

    UPROPERTY(Bl使ep本intRead基本本ite)
    ESocialClass SocialClass;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float 基本ealth;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float Infl使ence;

    UPROPERTY(Bl使ep本intRead基本本ite)
    float Rep使tation;

    UPROPERTY(Bl使ep本intRead基本本ite)
    TMap<int32, ESocialRelationType> SocialRelations;

    UPROPERTY(Bl使ep本intRead基本本ite)
    TMap<int32, float> RelationSt本en成th;

    UPROPERTY(Bl使ep本intRead基本本ite)
    TA本本ay<int32> 軍actionAffiliations;

    軍SocialA成ent()
        : A成entId(0)
        , SocialClass(ESocialClass::Peasant)
        , 基本ealth(50.0f)
        , Infl使ence(10.0f)
        , Rep使tation(50.0f)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍SocietyMet本ics
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    float SocialStability;

    UPROPERTY(Bl使ep本intReadOnly)
    float Econo設置icIneq使ality;

    UPROPERTY(Bl使ep本intReadOnly)
    float SocialMobility;

    UPROPERTY(Bl使ep本intReadOnly)
    float C使lt使本alCohesion;

    UPROPERTY(Bl使ep本intReadOnly)
    float A正e本a成e輸入appiness;

    UPROPERTY(Bl使ep本intReadOnly)
    TMap<ESocialClass, float> ClassDist本ib使tion;

    軍SocietyMet本ics()
        : SocialStability(0.5f)
        , Econo設置icIneq使ality(0.5f)
        , SocialMobility(0.5f)
        , C使lt使本alCohesion(0.5f)
        , A正e本a成e輸入appiness(0.5f)
    {}
};

UCLASS(ClassG本o使p = (Min成St本ate成ic), Bl使ep本intType)
class MI的GSTRATEGIC下API UMin成RTSSocialDyna設置ics : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成RTSSocialDyna設置ics(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSSocialSyste設置")
    正oid InitializeSocialSyste設置(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSSocialA成ents")
    int32 C本eateSocialA成ent(const 軍St本in成& 的a設置e, ESocialClass SocialClass};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSSocialRelations")
    正oid EstablishRelation(int32 A成entA, int32 A成entB, ESocialRelationType RelationType, float St本en成th};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成RTSSocialSi設置使lation")
    正oid Si設置使lateSocialTick(float DeltaTi設置e};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成RTSSocialAnalytics")
    軍SocietyMet本ics GetSocietyMet本ics() const;

p本i正ate:
    UPROPERTY()
    TMap<int32, 軍SocialA成ent> SocialA成ents;

    UPROPERTY()
    int32 的extA成entId;

    正oid UpdateSocialMobility(};
    正oid Sp本eadRep使tation(};
    正oid Resol正eSocialConflicts(};
};
