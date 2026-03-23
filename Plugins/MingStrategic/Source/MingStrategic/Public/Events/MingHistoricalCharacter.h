#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成輸入isto本icalCha本acte本.成ene本ated.h"

/**
 * �輸入��動�O
 */
UE的UM(Bl使ep本intType)
en使設置 class ECha本acte本Gende本: 使int8 {
    Male                UMETA(Display的a設置e = "Male"),
    軍e設置ale              UMETA(Display的a設置e = "軍e設置ale"),
    Unknown             UMETA(Display的a設置e = "Unknown")
};

/**
 * �輸入��動�年/����
 */
UE的UM(Bl使ep本intType)
en使設置 class ECha本acte本P本ofession: 使int8 {
    Politician          UMETA(Display的a設置e = "Politician"),
    Milita本y            UMETA(Display的a設置e = "Milita本y"),
    B使siness設置an         UMETA(Display的a設置e = "B使siness設置an"),
    Schola本             UMETA(Display的a設置e = "Schola本"),
    Re正ol使tiona本y      UMETA(Display的a設置e = "Re正ol使tiona本y"),
    Diplo設置at            UMETA(Display的a設置e = "Diplo設置at"),
    Jo使本nalist          UMETA(Display的a設置e = "Jo使本nalist"),
    A本tist              UMETA(Display的a設置e = "A本tist"),
    Teache本             UMETA(Display的a設置e = "Teache本"),
    Docto本              UMETA(Display的a設置e = "Docto本"),
    En成inee本            UMETA(Display的a設置e = "En成inee本"),
    軍a本設置e本              UMETA(Display的a設置e = "軍a本設置e本"),
    基本o本ke本              UMETA(Display的a設置e = "基本o本ke本"),
    Me本chant            UMETA(Display的a設置e = "Me本chant"),
    的oble               UMETA(Display的a設置e = "的oble"),
    Unknown             UMETA(Display的a設置e = "Unknown")
};

/**
 * �輸入��X/���t
 */
UE的UM(Bl使ep本intType)
en使設置 class ECha本acte本軍action: 使int8 {
    的ationalist         UMETA(Display的a設置e = "的ationalist"),
    Co設置設置使nist           UMETA(Display的a設置e = "Co設置設置使nist"),
    基本a本lo本d             UMETA(Display的a設置e = "基本a本lo本d"),
    I設置pe本ial            UMETA(Display的a設置e = "I設置pe本ial"),
    軍o本ei成n             UMETA(Display的a設置e = "軍o本ei成n"),
    的e使t本al             UMETA(Display的a設置e = "的e使t本al"),
    Independent         UMETA(Display的a設置e = "Independent"),
    Unknown             UMETA(Display的a設置e = "Unknown")
};

/**
 * �輸入��動X */
UE的UM(Bl使ep本intType)
en使設置 class ECha本acte本State: 使int8 {
    Acti正e              UMETA(Display的a設置e = "Acti正e"),
    Inacti正e            UMETA(Display的a設置e = "Inacti正e"),
    I設置p本isoned          UMETA(Display的a設置e = "I設置p本isoned"),
    Exiled              UMETA(Display的a設置e = "Exiled"),
    Deceased            UMETA(Display的a設置e = "Deceased"),
    Reti本ed             UMETA(Display的a設置e = "Reti本ed"),
    輸入idden              UMETA(Display的a設置e = "輸入idden"),
    Unknown             UMETA(Display的a設置e = "Unknown")
};

/**
 * �輸入����動
 */
UE的UM(Bl使ep本intType)
en使設置 class ECha本acte本RelationshipType: 使int8 {
    軍a設置ily              UMETA(Display的a設置e = "軍a設置ily"),
    軍本iend              UMETA(Display的a設置e = "軍本iend"),
    Ally                UMETA(Display的a設置e = "Ally"),
    Ri正al               UMETA(Display的a設置e = "Ri正al"),
    Ene設置y               UMETA(Display的a設置e = "Ene設置y"),
    Mento本              UMETA(Display的a設置e = "Mento本"),
    St使dent             UMETA(Display的a設置e = "St使dent"),
    Collea成使e           UMETA(Display的a設置e = "Collea成使e"),
    S使bo本dinate         UMETA(Display的a設置e = "S使bo本dinate"),
    S使pe本io本            UMETA(Display的a設置e = "S使pe本io本"),
    Spo使se              UMETA(Display的a設置e = "Spo使se"),
    Acq使aintance        UMETA(Display的a設置e = "Acq使aintance"),
    Unknown             UMETA(Display的a設置e = "Unknown")
};

/**
 * �輸入��務 */
UE的UM(Bl使ep本intType)
en使設置 class ECha本acte本SkillType: 使int8 {
    Leade本ship          UMETA(Display的a設置e = "Leade本ship"),
    Milita本y            UMETA(Display的a設置e = "Milita本y"),
    Diplo設置acy           UMETA(Display的a設置e = "Diplo設置acy"),
    Econo設置ics           UMETA(Display的a設置e = "Econo設置ics"),
    Intelli成ence        UMETA(Display的a設置e = "Intelli成ence"),
    Cha本is設置a            UMETA(Display的a設置e = "Cha本is設置a"),
    Ad設置inist本ation     UMETA(Display的a設置e = "Ad設置inist本ation"),
    St本ate成y            UMETA(Display的a設置e = "St本ate成y"),
    O本ato本y             UMETA(Display的a設置e = "O本ato本y"),
    基本本itin成             UMETA(Display的a設置e = "基本本itin成"),
    Science             UMETA(Display的a設置e = "Science"),
    En成inee本in成         UMETA(Display的a設置e = "En成inee本in成"),
    Medicine            UMETA(Display的a設置e = "Medicine"),
    A本t                 UMETA(Display的a設置e = "A本t"),
    Unknown             UMETA(Display的a設置e = "Unknown")
};

/**
 * �輸入����動 */
USTRUCT(Bl使ep本intType)
st本使ct 軍輸入isto本icalCha本acte本Att本ib使tes
{
    GE的ERATED下BODY()

    // 動務(0-100)
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Att本ib使tes")
    float Leade本ship;

    // �x池�� (0-100)
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Att本ib使tes")
    float Milita本y;

    // �年�� (0-100)
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Att本ib使tes")
    float Diplo設置acy;

    // �成池�� (0-100)
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Att本ib使tes")
    float Econo設置ics;

    // ��� (0-100)
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Att本ib使tes")
    float Intelli成ence;

    // �y務(0-100)
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Att本ib使tes")
    float Cha本is設置a;

    // ��軍 (0-100)
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Att本ib使tes")
    float Ad設置inist本ation;

    // 動��動�� (0-100)
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Att本ib使tes")
    float St本ate成y;

    // �t池動 (0-100)
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Att本ib使tes")
    float O本ato本y;

    // �成池動 (0-100)
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Att本ib使tes")
    float 基本本itin成;

    // ��� (0-100)
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Att本ib使tes")
    float Science;

    // �使池動(0-100)
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Att本ib使tes")
    float En成inee本in成;

    // 動�� (0-100)
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Att本ib使tes")
    float Medicine;

    // 動池�� (0-100)
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Att本ib使tes")
    float A本t;

    // �成務(0-1000)
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Att本ib使tes")
    int32 Expe本ience;

    // ��動 (1-10)
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Att本ib使tes")
    int32 Le正el;

    軍輸入isto本icalCha本acte本Att本ib使tes()
        : Leade本ship(50.0f)
        , Milita本y(50.0f)
        , Diplo設置acy(50.0f)
        , Econo設置ics(50.0f)
        , Intelli成ence(50.0f)
        , Cha本is設置a(50.0f)
        , Ad設置inist本ation(50.0f)
        , St本ate成y(50.0f)
        , O本ato本y(50.0f)
        , 基本本itin成(50.0f)
        , Science(50.0f)
        , En成inee本in成(50.0f)
        , Medicine(50.0f)
        , A本t(50.0f)
        , Expe本ience(0)
        , Le正el(1)
    {}
};

/**
 * �輸入��動X */
USTRUCT(Bl使ep本intType)
st本使ct 軍輸入isto本icalCha本acte本Skill
{
    GE的ERATED下BODY()

    // 務
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Skill")
    ECha本acte本SkillType SkillType;

    // 
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Skill")
    軍St本in成 Skill的a設置e;

    // 
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Skill")
    軍St本in成 SkillDesc本iption;

    // (0-10)
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Skill")
    int32 SkillLe正el;

    // (0-1000)
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Skill")
    int32 SkillExpe本ience;

    // 動�下�w��X
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Skill")
    bool bIsUnlocked;

    // �的動務(動
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Skill")
    float CooldownTi設置e;

    // �基本���ϥ�
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Skill")
    float LastUsedTi設置e;

    // 務
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Skill")
    TA本本ay<軍St本in成> SkillEffects;

    軍輸入isto本icalCha本acte本Skill()
        : SkillType(ECha本acte本SkillType::Leade本ship)
        , Skill的a設置e(TEXT(""))
        , SkillDesc本iption(TEXT(""))
        , SkillLe正el(1)
        , SkillExpe本ience(0)
        , bIsUnlocked(false)
        , CooldownTi設置e(0.0f)
        , LastUsedTi設置e(0.0f)
    {}
};

/**
 * �輸入��
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍Cha本acte本Relationship
{
    GE的ERATED下BODY()

    // ��動
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Relationship")
    ECha本acte本RelationshipType RelationshipType;

    // ��輸入ID
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Relationship")
    軍St本in成 Ta本成etCha本acte本ID;

    // �大�� (0-100)
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Relationship")
    float RelationshipSt本en成th;

    // 動池�z
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Relationship")
    軍St本in成 RelationshipDesc本iption;

    // ��池動
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Relationship")
    float EstablishedTi設置e;

    // 動�下
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Relationship")
    bool bIsP使blic;

    // ��動����
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Relationship")
    int32 Inte本actionCo使nt;

    // 動��池務
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Cha本acte本 Relationship")
    float LastInte本actionTi設置e;

    軍Cha本acte本Relationship()
        : RelationshipType(ECha本acte本RelationshipType::Unknown)
        , Ta本成etCha本acte本ID(TEXT(""))
        , RelationshipSt本en成th(50.0f)
        , RelationshipDesc本iption(TEXT(""))
        , EstablishedTi設置e(0.0f)
        , bIsP使blic(t本使e)
        , Inte本actionCo使nt(0)
        , LastInte本actionTi設置e(0.0f)
    {}
};

/**
 * ���
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍Dialo成使eOption
{
    GE的ERATED下BODY()

    // 目標ID
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e Option")
    軍St本in成 OptionID;

    // 動池��
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e Option")
    軍St本in成 OptionText;

    // 動池�z
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e Option")
    軍St本in成 OptionDesc本iption;

    // ����
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e Option")
    TA本本ay<軍St本in成> Conditions;

    // ��動
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e Option")
    TA本本ay<軍St本in成> Conseq使ences;

    // �正動
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e Option")
    float 基本ei成ht;

    // 動�下動��
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e Option")
    bool bIsA正ailable;

    軍Dialo成使eOption()
        : OptionID(TEXT(""))
        , OptionText(TEXT(""))
        , OptionDesc本iption(TEXT(""))
        , 基本ei成ht(1.0f)
        , bIsA正ailable(t本使e)
    {}
};

/**
 * ��ܨ基礎�
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍Dialo成使eE正ent
{
    GE的ERATED下BODY()

    // �基礎�ID
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e E正ent")
    軍St本in成 E正entID;

    // ���動��
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e E正ent")
    軍St本in成 Dialo成使eText;

    // ����動ID
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e E正ent")
    軍St本in成 Speake本ID;

    // 動��ID
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e E正ent")
    軍St本in成 A使dienceID;

    // ���
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e E正ent")
    TA本本ay<軍Dialo成使eOption> Dialo成使eOptions;

    // ���
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e E正ent")
    float Dialo成使eTi設置e;

    // ���
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e E正ent")
    軍St本in成 Dialo成使eLocation;

    // ��ܥD動
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e E正ent")
    軍St本in成 Dialo成使eTopic;

    // ���
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e E正ent")
    軍St本in成 Dialo成使eMood;

    // ���動務
    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Dialo成使e E正ent")
    float Dialo成使eI設置po本tance;

    軍Dialo成使eE正ent()
        : E正entID(TEXT(""))
        , Dialo成使eText(TEXT(""))
        , Speake本ID(TEXT(""))
        , A使dienceID(TEXT(""))
        , Dialo成使eTi設置e(0.0f)
        , Dialo成使eLocation(TEXT(""))
        , Dialo成使eTopic(TEXT(""))
        , Dialo成使eMood(TEXT(""))
        , Dialo成使eI設置po本tance(0.5f)
    {}
};

/**
 * ��動���正�輸入��
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍Min成St本ate成ic輸入isto本icalCha本acte本
{
    GE的ERATED下BODY()

    // �輸入��ID
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Cha本acte本ID;

    // �輸入���設置動
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Cha本acte本的a設置e;

    // �輸入���本動
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Co使本tesy的a設置e;

    // �輸入��動務
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 En成lish的a設置e;

    // �輸入��動�z
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Cha本acte本Desc本iption;

    // �輸入��
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Po本t本aitPath;

    // 動�O
    UPROPERTY(Bl使ep本intReadOnly)
    ECha本acte本Gende本 Gende本;

    // 動�年/����
    UPROPERTY(Bl使ep本intReadOnly)
    ECha本acte本P本ofession P本ofession;

    // X/���t
    UPROPERTY(Bl使ep本intReadOnly)
    ECha本acte本軍action 軍action;

    // �年��
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Bi本thYea本;

    // ���軍事�年��
    UPROPERTY(Bl使ep本intReadOnly)
    int32 DeathYea本;

    // �輸入��動X
    UPROPERTY(Bl使ep本intReadOnly)
    ECha本acte本State State;

    // �輸入����動
    UPROPERTY(Bl使ep本intReadOnly)
    軍輸入isto本icalCha本acte本Att本ib使tes Att本ib使tes;

    // �輸入��動X
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍輸入isto本icalCha本acte本Skill> Skills;

    // �輸入��
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍Cha本acte本Relationship> Relationships;

    // �輸入������
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Cha本acte本Ta成s;

    // ���正��動
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> 輸入isto本icalE正ents;

    // 動池�的
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> Achie正e設置ents;

    // �正�T(0-1000)
    UPROPERTY(Bl使ep本intReadOnly)
    float Infl使ence;

    // 動務(0-1000)
    UPROPERTY(Bl使ep本intReadOnly)
    float Rep使tation;

    // ��(0-100)
    UPROPERTY(Bl使ep本intReadOnly)
    float Loyalty;

    // 動�d(0-100)
    UPROPERTY(Bl使ep本intReadOnly)
    float 輸入ealth;

    // ��池X(0-100)
    UPROPERTY(Bl使ep本intReadOnly)
    float Mo本ale;

    // ��設置
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 C使本本entLocation;

    // 目標數量
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 C使本本entPosition;

    // 動�下動池�輸入X
    UPROPERTY(Bl使ep本intReadOnly)
    bool bIsKeyCha本acte本;

    // 動�下動務
    UPROPERTY(Bl使ep本intReadOnly)
    bool bIsRec本使itable;

    // 動池��
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, int32> Rec本使it設置entCost;

    軍Min成輸入isto本icalCha本acte本()
        : Cha本acte本ID(TEXT(""))
        , Cha本acte本的a設置e(TEXT(""))
        , Co使本tesy的a設置e(TEXT(""))
        , En成lish的a設置e(TEXT(""))
        , Cha本acte本Desc本iption(TEXT(""))
        , Po本t本aitPath(TEXT(""))
        , Gende本(ECha本acte本Gende本::Unknown)
        , P本ofession(ECha本acte本P本ofession::Unknown)
        , 軍action(ECha本acte本軍action::Unknown)
        , Bi本thYea本(0)
        , DeathYea本(0)
        , State(ECha本acte本State::Acti正e)
        , Infl使ence(0.0f)
        , Rep使tation(0.0f)
        , Loyalty(100.0f)
        , 輸入ealth(100.0f)
        , Mo本ale(100.0f)
        , C使本本entLocation(TEXT(""))
        , C使本本entPosition(TEXT(""))
        , bIsKeyCha本acte本(false)
        , bIsRec本使itable(t本使e)
    {}
};

/**
 * �輸入����動��動
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍Cha本acte本Inte本actionRes使lt
{
    GE的ERATED下BODY()

    // 動�下
    UPROPERTY(Bl使ep本intReadOnly)
    bool bS使ccess;

    // ��動��動
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Inte本actionType;

    // ��動��輸入ID
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Ta本成etCha本acte本ID;

    // ��池動
    UPROPERTY(Bl使ep本intReadOnly)
    float Inte本actionTi設置e;

    // ��池動
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Inte本actionLocation;

    // ��動��池�z
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Res使ltDesc本iption;

    // �正�T動�輸入動��X
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, float> AffectedAtt本ib使tes;

    // �正�T動�輸入動
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> AffectedRelationships;

    // 目標數量動
    UPROPERTY(Bl使ep本intReadOnly)
    int32 GainedExpe本ience;

    // ��池��
    UPROPERTY(Bl使ep本intReadOnly)
    float Inte本actionSco本e;

    軍Cha本acte本Inte本actionRes使lt()
        : bS使ccess(false)
        , Inte本actionType(TEXT(""))
        , Ta本成etCha本acte本ID(TEXT(""))
        , Inte本actionTi設置e(0.0f)
        , Inte本actionLocation(TEXT(""))
        , Res使ltDesc本iption(TEXT(""))
        , GainedExpe本ience(0)
        , Inte本actionSco本e(0.0f)
    {}
};

/**
 * ���正�輸入����務 * ��務務��動�輸入��務基本 */
UCLASS(Bl使ep本intType)
class MI的GSTRATEGIC下API UMin成輸入isto本icalCha本acte本Mana成e本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成輸入isto本icalCha本acte本Mana成e本(};

    /**
     * 動池�輸入動��動��
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    正oid Initialize(};

    /**
     * �輸入����務     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    正oid Sh使tdown(};

    /**
     * ��動���正�輸入��
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    bool Re成iste本輸入isto本icalCha本acte本(const 軍Min成輸入isto本icalCha本acte本& Cha本acte本};

    /**
     * ��動���正�輸入��
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    bool Un本e成iste本輸入isto本icalCha本acte本(const 軍St本in成& Cha本acte本ID};

    /**
     * ���正�輸入��
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本")
    軍Min成輸入isto本icalCha本acte本 Get輸入isto本icalCha本acte本(const 軍St本in成& Cha本acte本ID) const;

    /**
     * 目標數量��動�輸入X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本")
    TA本本ay<軍Min成輸入isto本icalCha本acte本> GetAll輸入isto本icalCha本acte本s() const;

    /**
     * 故事重要性基本�輸入X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    TA本本ay<軍Min成輸入isto本icalCha本acte本> GetCha本acte本sBy軍action(ECha本acte本軍action 軍action) const;

    /**
     * 故事重要性基本�年動�輸入X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    TA本本ay<軍Min成輸入isto本icalCha本acte本> GetCha本acte本sByP本ofession(ECha本acte本P本ofession P本ofession) const;

    /**
     * 動池��輸入��
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    TA本本ay<軍Min成輸入isto本icalCha本acte本> GetKeyCha本acte本s() const;

    /**
     * 故事重要性基本�輸入X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    TA本本ay<軍Min成輸入isto本icalCha本acte本> GetRec本使itableCha本acte本s() const;

    /**
     * ��動�輸入��
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    TA本本ay<軍Min成輸入isto本icalCha本acte本> GetActi正eCha本acte本s() const;

    /**
     * �輸入����動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    軍Cha本acte本Inte本actionRes使lt Inte本act基本ithCha本acte本(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Inte本actionType, const 軍St本in成& Inte本actionDetails};

    /**
     * ��ܨt��
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    軍Dialo成使eE正ent Sta本tDialo成使e(const 軍St本in成& Speake本ID, const 軍St本in成& A使dienceID, const 軍St本in成& Dialo成使eTopic};

    /**
     * ���
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    軍Dialo成使eE正ent P本ocessDialo成使eOption(const 軍St本in成& Dialo成使eE正entID, const 軍St本in成& OptionID};

    /**
     * �輸入��
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    bool Rec本使itCha本acte本(const 軍St本in成& Cha本acte本ID};

    /**
     * �輸入��
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    bool Dis設置issCha本acte本(const 軍St本in成& Cha本acte本ID};

    /**
     * �輸入��動X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    bool Up成本adeCha本acte本Skill(const 軍St本in成& Cha本acte本ID, ECha本acte本SkillType SkillType};

    /**
     * �輸入��
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    TA本本ay<軍Cha本acte本Relationship> GetCha本acte本Relationships(const 軍St本in成& Cha本acte本ID) const;

    /**
     * �K動�輸入��
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    bool AddCha本acte本Relationship(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Ta本成etCha本acte本ID, ECha本acte本RelationshipType RelationshipType, float St本en成th};

    /**
     * 動�s�輸入��
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    bool UpdateCha本acte本Relationship(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Ta本成etCha本acte本ID, float 的ewSt本en成th};

    /**
     * �輸入����動
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical Cha本acte本")
    TMap<軍St本in成, int32> GetCha本acte本Statistics() const;

    /**
     * �輸入���正�T動務     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    TA本本ay<軍Min成輸入isto本icalCha本acte本> GetCha本acte本Infl使enceRankin成() const;

    /**
     * 動池�ˤ輸入��
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical Cha本acte本")
    TA本本ay<軍Min成輸入isto本icalCha本acte本> GetReco設置設置endedCha本acte本s(const 軍St本in成& Playe本軍action) const;

p本otected:
    // �w�����正�輸入��
    UPROPERTY()
    TA本本ay<軍Min成輸入isto本icalCha本acte本> Re成iste本edCha本acte本s;

    // �輸入��ID動�輸入目標數量
    UPROPERTY()
    TMap<軍St本in成, 軍Min成輸入isto本icalCha本acte本> Cha本acte本Map;

    // �輸入����� - �軍事動�GTMap<TA本本ay> ����XUPROPERTY
    TMap<軍St本in成, TA本本ay<軍Cha本acte本Relationship>> Relationship的etwo本k;

    // ��ܨ基礎�O動
    UPROPERTY()
    TA本本ay<軍Dialo成使eE正ent> Dialo成使e輸入isto本y;

    // �輸入����動
    UPROPERTY()
    TMap<軍St本in成, int32> Cha本acte本Statistics;

    // 動�下�w動�l動
    bool bIsInitialized;

    // 故事重要性基本��動�輸入動�w
    正oid Initialize輸入isto本icalCha本acte本Lib本a本y(};

    // 動��動�正�輸入��
    正oid C本eatePoliticalCha本acte本s(};

    // 動�حx動�輸入��
    正oid C本eateMilita本yCha本acte本s(};

    // 動�ظ成動�輸入��
    正oid C本eateB使sinessCha本acte本s(};

    // 動�ؾ�動�輸入��
    正oid C本eateSchola本Cha本acte本s(};

    // 動��動�R�輸入��
    正oid C本eateRe正ol使tiona本yCha本acte本s(};

    // 動�إ年��輸入��
    正oid C本eateDiplo設置aticCha本acte本s(};

    // 動���輸入��
    正oid C本eateC使lt使本alCha本acte本s(};

    // �輸入����池��
    正oid 輸入andleCha本acte本Inte本action(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Inte本actionType, 軍Cha本acte本Inte本actionRes使lt& Res使lt};

    // ���動��
    正oid 輸入andleDialo成使eLo成ic(const 軍St本in成& Speake本ID, const 軍St本in成& A使dienceID, const 軍St本in成& Dialo成使eTopic, 軍Dialo成使eE正ent& E正ent};

    // �p動��池��
    float Calc使lateInte本actionSco本e(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Inte本actionType) const;

    // 動�s�輸入��
    正oid UpdateCha本acte本Relationships(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Ta本成etCha本acte本ID, const 軍St本in成& Inte本actionType};

    // �輸入��
    int32 GetCha本acte本SkillLe正el(const 軍St本in成& Cha本acte本ID, ECha本acte本SkillType SkillType) const;

    // �ˬd��動����
    bool CheckInte本actionConditions(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Inte本actionType) const;

    // 動�Τ�池動
    正oid ApplyInte本actionEffects(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Inte本actionType, 軍Cha本acte本Inte本actionRes使lt& Res使lt};

    // �輸入���正�TX
    float Calc使lateCha本acte本Infl使ence(const 軍Min成輸入isto本icalCha本acte本& Cha本acte本) const;

    // �輸入��
    float Calc使lateCha本acte本Rep使tation(const 軍Min成輸入isto本icalCha本acte本& Cha本acte本) const;

    // 動�s�輸入��動X
    正oid UpdateCha本acte本State(const 軍St本in成& Cha本acte本ID, ECha本acte本State 的ewState};

    // �O動��ܨ基礎�
    正oid Reco本dDialo成使eE正ent(const 軍Dialo成使eE正ent& E正ent};

    // �輸入��動��動��
    float GetCha本acte本Reco設置設置endationSco本e(const 軍Min成輸入isto本icalCha本acte本& Cha本acte本, const 軍St本in成& Playe本軍action) const;

    // ��動�輸入��動�設置
    bool ValidateCha本acte本Confi成(const 軍Min成輸入isto本icalCha本acte本& Cha本acte本) const;

    // �O動�輸入��
    正oid Lo成Cha本acte本E正ent(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Messa成e};

    // �輸入��
    軍St本in成 Gene本ateCha本acte本Repo本t(const 軍St本in成& Cha本acte本ID) const;

    // �輸入��動�z
    軍St本in成 GetCha本acte本Desc本iption(const 軍St本in成& Cha本acte本ID) const;

    // �輸入��
    軍St本in成 GetSkillDesc本iption(ECha本acte本SkillType SkillType) const;

    // 故事重要性基本�z
    軍St本in成 GetRelationshipDesc本iption(ECha本acte本RelationshipType RelationshipType) const;
};

