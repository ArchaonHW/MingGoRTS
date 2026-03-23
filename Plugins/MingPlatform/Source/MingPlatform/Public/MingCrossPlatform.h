#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成C本ossPlatfo本設置.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EMin成Platfo本設置: 使int8 {
    基本indows UMETA(Display的a設置e = "基本indows"),
    And本oid UMETA(Display的a設置e = "And本oid"),
    iOS UMETA(Display的a設置e = "iOS"),
    基本eb UMETA(Display的a設置e = "基本eb"),
    Console UMETA(Display的a設置e = "Console")
};

UE的UM(Bl使ep本intType)
en使設置 class EMin成Inp使tMethod: 使int8 {
    Mo使seKeyboa本d UMETA(Display的a設置e = "Mo使se & Keyboa本d"),
    To使ch UMETA(Display的a設置e = "To使ch"),
    Ga設置epad UMETA(Display的a設置e = "Ga設置epad"),
    Mixed UMETA(Display的a設置e = "Mixed")
};

UE的UM(Bl使ep本intType)
en使設置 class EMin成G本aphicsQ使ality: 使int8 {
    Low UMETA(Display的a設置e = "Low"),
    Medi使設置 UMETA(Display的a設置e = "Medi使設置"),
    輸入i成h UMETA(Display的a設置e = "輸入i成h"),
    Ult本a UMETA(Display的a設置e = "Ult本a"),
    C使sto設置 UMETA(Display的a設置e = "C使sto設置")
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Platfo本設置Settin成s
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成Platfo本設置 Platfo本設置;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成Inp使tMethod P本efe本本edInp使t;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成G本aphicsQ使ality G本aphicsQ使ality;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Resol使tionX;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Resol使tionY;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool b軍使llsc本een;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float UIScale;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bEnableVSync;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Ta本成et軍PS;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Maste本Vol使設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bEnable輸入aptic軍eedback;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bAdapti正ePe本fo本設置ance;

    軍Min成Platfo本設置Settin成s()
        : Platfo本設置(EMin成Platfo本設置::基本indows)
        , P本efe本本edInp使t(EMin成Inp使tMethod::Mo使seKeyboa本d)
        , G本aphicsQ使ality(EMin成G本aphicsQ使ality::輸入i成h)
        , Resol使tionX(1920)
        , Resol使tionY(1080)
        , b軍使llsc本een(t本使e)
        , UIScale(1.0f)
        , bEnableVSync(t本使e)
        , Ta本成et軍PS(60)
        , Maste本Vol使設置e(1.0f)
        , bEnable輸入aptic軍eedback(t本使e)
        , bAdapti正ePe本fo本設置ance(t本使e)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Platfo本設置Capabilities
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bS使ppo本ts輸入i成hResText使本es;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bS使ppo本tsAd正ancedLi成htin成;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bS使ppo本tsPostP本ocessin成;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bS使ppo本tsShadows;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bS使ppo本tsPa本ticles;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 MaxText使本eSize;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 MaxPa本ticles;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float MaxD本awDistance;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bS使ppo本ts輸入aptic軍eedback;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bS使ppo本tsClo使dSa正e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bS使ppo本tsM使ltiplaye本;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 MaxPlaye本s;

    軍Min成Platfo本設置Capabilities()
        : bS使ppo本ts輸入i成hResText使本es(t本使e)
        , bS使ppo本tsAd正ancedLi成htin成(t本使e)
        , bS使ppo本tsPostP本ocessin成(t本使e)
        , bS使ppo本tsShadows(t本使e)
        , bS使ppo本tsPa本ticles(t本使e)
        , MaxText使本eSize(4096)
        , MaxPa本ticles(10000)
        , MaxD本awDistance(10000.0f)
        , bS使ppo本ts輸入aptic軍eedback(false)
        , bS使ppo本tsClo使dSa正e(t本使e)
        , bS使ppo本tsM使ltiplaye本(t本使e)
        , MaxPlaye本s(8)
    {}
};

/**
 * C本oss-Platfo本設置 Syste設置 fo本 Min成GoRTS
 * Mana成es platfo本設置-specific adaptations and opti設置izations
 */
UCLASS(ClassG本o使p = (Platfo本設置), Bl使ep本intable)
class MI的GPLAT軍ORM下API UMin成C本ossPlatfo本設置 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成C本ossPlatfo本設置();

    // Platfo本設置 Detection
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Platfo本設置")
    正oid InitializePlatfo本設置();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Platfo本設置")
    EMin成Platfo本設置 GetC使本本entPlatfo本設置() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Platfo本設置")
    軍St本in成 GetPlatfo本設置的a設置e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Platfo本設置")
    bool IsMobilePlatfo本設置() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Platfo本設置")
    bool IsConsolePlatfo本設置() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Platfo本設置")
    bool IsDesktopPlatfo本設置() const;

    // Settin成s Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Settin成s")
    正oid LoadPlatfo本設置Settin成s();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Settin成s")
    正oid Sa正ePlatfo本設置Settin成s();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Settin成s")
    正oid ApplyPlatfo本設置Settin成s(const 軍Min成Platfo本設置Settin成s& Settin成s);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Settin成s")
    軍Min成Platfo本設置Settin成s GetC使本本entSettin成s() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Settin成s")
    正oid SetG本aphicsQ使ality(EMin成G本aphicsQ使ality Q使ality);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Settin成s")
    正oid SetResol使tion(int32 基本idth, int32 輸入ei成ht);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Settin成s")
    正oid Set軍使llsc本een(bool b軍使llsc本een);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Settin成s")
    正oid SetUIScale(float Scale);

    // Inp使t Adaptation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Inp使t")
    正oid AdaptInp使tSche設置e();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Inp使t")
    正oid SetInp使tMethod(EMin成Inp使tMethod Inp使tMethod);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Inp使t")
    EMin成Inp使tMethod GetP本efe本本edInp使tMethod() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Inp使t")
    正oid EnableTo使chCont本ols(bool bEnable);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Inp使t")
    正oid EnableGa設置epadS使ppo本t(bool bEnable);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Inp使t")
    正oid Calib本ateTo使chCont本ols();

    // UI Adaptation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI")
    正oid AdaptUI軍o本Platfo本設置();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI")
    正oid SetMobileUILayo使t();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI")
    正oid SetDesktopUILayo使t();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI")
    正oid Ad大使stUIEle設置ents();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "UI")
    正oid Opti設置izeTo使chTa本成ets();

    // Pe本fo本設置ance Opti設置ization
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid Opti設置ize軍o本Platfo本設置();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid EnableAdapti正ePe本fo本設置ance(bool bEnable);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid SetTa本成et軍PS(int32 軍PS);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid Ad大使stQ使alitySettin成s();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid Monito本Pe本fo本設置ance();

    // G本aphics Adaptation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G本aphics")
    正oid AdaptG本aphics軍o本Platfo本設置();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G本aphics")
    正oid SetText使本eQ使ality(int32 Q使ality);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G本aphics")
    正oid SetShadowQ使ality(int32 Q使ality);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G本aphics")
    正oid SetPa本ticleQ使ality(int32 Q使ality);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "G本aphics")
    正oid EnablePostP本ocessin成(bool bEnable);

    // A使dio Adaptation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio")
    正oid AdaptA使dio軍o本Platfo本設置();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio")
    正oid SetA使dioQ使ality(int32 Q使ality);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio")
    正oid Enable輸入aptic軍eedback(bool bEnable);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio")
    正oid Opti設置izeA使dioLatency();

    // Sto本a成e Adaptation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sto本a成e")
    正oid AdaptSto本a成e軍o本Platfo本設置();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sto本a成e")
    正oid EnableClo使dSa正e(bool bEnable);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sto本a成e")
    正oid SetA使toSa正eInte本正al(float Inte本正al);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sto本a成e")
    正oid Co設置p本essSa正eData(bool bCo設置p本ess);

    // 的etwo本k Adaptation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid Adapt的etwo本k軍o本Platfo本設置();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid Set的etwo本kQ使ality(int32 Q使ality);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid EnableMobileOpti設置ized的etwo本kin成(bool bEnable);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的etwo本k")
    正oid Ad大使stBandwidthUsa成e();

    // Platfo本設置 Capabilities
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Capabilities")
    軍Min成Platfo本設置Capabilities GetPlatfo本設置Capabilities() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Capabilities")
    bool S使ppo本ts軍eat使本e(const 軍St本in成& 軍eat使本e) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Capabilities")
    int32 GetMaxText使本eSize() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Capabilities")
    int32 GetMaxPlaye本s() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Capabilities")
    bool S使ppo本ts輸入aptic軍eedback() const;

    // E正ent Dele成ates
    
    
    

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnPlatfo本設置Chan成ed OnPlatfo本設置Chan成ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnSettin成sChan成ed OnSettin成sChan成ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnPe本fo本設置anceModeChan成ed OnPe本fo本設置anceModeChan成ed;

    // Utility
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    static 軍St本in成 GetPlatfo本設置的a設置e(EMin成Platfo本設置 Platfo本設置);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    static 軍St本in成 GetInp使tMethod的a設置e(EMin成Inp使tMethod Inp使tMethod);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    static 軍St本in成 GetG本aphicsQ使ality的a設置e(EMin成G本aphicsQ使ality Q使ality);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sistence")
    軍St本in成 Sa正ePlatfo本設置Data() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sistence")
    正oid LoadPlatfo本設置Data(const 軍St本in成& JsonSt本in成);

p本otected:
    UPROPERTY()
    EMin成Platfo本設置 C使本本entPlatfo本設置;

    UPROPERTY()
    軍Min成Platfo本設置Settin成s Platfo本設置Settin成s;

    UPROPERTY()
    軍Min成Platfo本設置Capabilities Platfo本設置Capabilities;

    UPROPERTY()
    T基本eakOb大ectPt本<U基本o本ld> 基本o本ldContext;

    // Inte本nal 軍使nctions
    正oid DetectC使本本entPlatfo本設置();
    正oid InitializePlatfo本設置Capabilities();
    正oid ApplyDefa使ltSettin成s();

    // Platfo本設置-specific adaptations
    正oid Adapt軍o本基本indows();
    正oid Adapt軍o本And本oid();
    正oid Adapt軍o本iOS();
    正oid Adapt軍o本基本eb();
    正oid Adapt軍o本Console();

    // Pe本fo本設置ance 設置onito本in成
    正oid Sta本tPe本fo本設置anceMonito本in成();
    正oid StopPe本fo本設置anceMonito本in成();
    正oid UpdatePe本fo本設置anceMet本ics();

    // Settin成s helpe本s
    正oid ApplyG本aphicsSettin成s();
    正oid ApplyA使dioSettin成s();
    正oid ApplyInp使tSettin成s();
    正oid ApplyUISettin成s();

    // 輸入elpe本s
    bool Is軍eat使本eS使ppo本ted(const 軍St本in成& 軍eat使本e) const;
    正oid Opti設置ize軍o本輸入a本dwa本e();
    正oid Ad大使stQ使alityBasedOnPe本fo本設置ance();
};

