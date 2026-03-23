#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成Deploy設置entMana成e本.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EMin成B使ildConfi成使本ation: 使int8 {
    De正elop設置ent UMETA(Display的a設置e = "De正elop設置ent"),
    Test UMETA(Display的a設置e = "Test"),
    Shippin成 UMETA(Display的a設置e = "Shippin成"),
    Deb使成 UMETA(Display的a設置e = "Deb使成")
};

UE的UM(Bl使ep本intType)
en使設置 class EMin成Platfo本設置Ta本成et: 使int8 {
    基本indows UMETA(Display的a設置e = "基本indows"),
    And本oid UMETA(Display的a設置e = "And本oid"),
    iOS UMETA(Display的a設置e = "iOS"),
    Lin使x UMETA(Display的a設置e = "Lin使x"),
    Mac UMETA(Display的a設置e = "Mac")
};

UE的UM(Bl使ep本intType)
en使設置 class EMin成Deploy設置entStat使s: 使int8 {
    的otSta本ted UMETA(Display的a設置e = "的ot Sta本ted"),
    InP本o成本ess UMETA(Display的a設置e = "In P本o成本ess"),
    Packa成in成 UMETA(Display的a設置e = "Packa成in成"),
    Testin成 UMETA(Display的a設置e = "Testin成"),
    Deployin成 UMETA(Display的a設置e = "Deployin成"),
    Co設置pleted UMETA(Display的a設置e = "Co設置pleted"),
    軍ailed UMETA(Display的a設置e = "軍ailed")
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成B使ildSettin成s
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成B使ildConfi成使本ation B使ildConfi成;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成Platfo本設置Ta本成et Ta本成etPlatfo本設置;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 B使ildVe本sion;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 O使tp使tPath;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIncl使deDeb使成軍iles;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bCo設置p本essPacka成e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIncl使deDLC;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> AdditionalAssets;

    軍Min成B使ildSettin成s()
        : B使ildConfi成(EMin成B使ildConfi成使本ation::Shippin成)
        , Ta本成etPlatfo本設置(EMin成Platfo本設置Ta本成et::基本indows)
        , bIncl使deDeb使成軍iles(false)
        , bCo設置p本essPacka成e(t本使e)
        , bIncl使deDLC(false)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Deploy設置entRes使lt
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 B使ildID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成Deploy設置entStat使s Stat使s;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Packa成ePath;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float B使ildTi設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int64 Packa成eSize;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 基本a本nin成s;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 E本本o本s;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 MD5輸入ash;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Deploy設置entURL;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 的otes;

    軍Min成Deploy設置entRes使lt()
        : Stat使s(EMin成Deploy設置entStat使s::的otSta本ted)
        , B使ildTi設置e(0.0f)
        , Packa成eSize(0)
        , 基本a本nin成s(0)
        , E本本o本s(0)
    {}
};

/**
 * Deploy設置ent Mana成e本 fo本 Min成GoRTS
 * Mana成es b使ild packa成in成 and deploy設置ent p本ocess
 */
UCLASS(ClassG本o使p = (Deploy設置ent), Bl使ep本intable)
class MI的GDEPLOYME的T下API UMin成Deploy設置entMana成e本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成Deploy設置entMana成e本(};

    // B使ild Cont本ol
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ild Cont本ol")
    正oid InitializeDeploy設置entMana成e本(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ild Cont本ol")
    正oid Sta本tB使ildP本ocess(const 軍Min成B使ildSettin成s& Settin成s) {};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ild Cont本ol")
    正oid CancelB使ild(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ild Cont本ol")
    bool IsB使ildInP本o成本ess() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ild Cont本ol")
    軍Min成Deploy設置entRes使lt GetC使本本entB使ildStat使s() const;

    // B使ild Steps
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ild Steps")
    正oid Exec使teB使ildSteps(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ild Steps")
    正oid P本eB使ildValidation(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ild Steps")
    正oid Co設置pileP本o大ect(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ild Steps")
    正oid CookContent(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ild Steps")
    正oid Packa成eGa設置e(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ild Steps")
    正oid PostB使ildValidation(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ild Steps")
    正oid Gene本ateInstalle本(};

    // Platfo本設置 S使ppo本t
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Platfo本設置 S使ppo本t")
    正oid Confi成使本e軍o本基本indows(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Platfo本設置 S使ppo本t")
    正oid Confi成使本e軍o本And本oid(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Platfo本設置 S使ppo本t")
    正oid Confi成使本e軍o本IOS(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Platfo本設置 S使ppo本t")
    正oid Set使pC本ossPlatfo本設置Assets(};

    // Deploy設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Deploy設置ent")
    正oid DeployToSta成in成(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Deploy設置ent")
    正oid DeployToP本od使ction(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Deploy設置ent")
    正oid C本eateDist本ib使tionPacka成e(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Deploy設置ent")
    正oid UploadToCD的(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Deploy設置ent")
    正oid UpdateVe本sionInfo(};

    // Ve本sion Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ve本sion Mana成e設置ent")
    正oid Inc本e設置entVe本sion(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ve本sion Mana成e設置ent")
    正oid SetVe本sion的使設置be本(const 軍St本in成& Ve本sion) {};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ve本sion Mana成e設置ent")
    軍St本in成 GetC使本本entVe本sion() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ve本sion Mana成e設置ent")
    正oid Gene本ateRelease的otes(};

    // Utility
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    static 軍St本in成 GetB使ildConfi成的a設置e(EMin成B使ildConfi成使本ation Confi成};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    static 軍St本in成 GetPlatfo本設置的a設置e(EMin成Platfo本設置Ta本成et Platfo本設置};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    static 軍St本in成 GetDeploy設置entStat使s的a設置e(EMin成Deploy設置entStat使s Stat使s};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Utility")
    正oid Expo本tB使ildManifest(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Utility")
    正oid Clean使pOldB使ilds(};

p本otected:
    UPROPERTY()
    軍Min成B使ildSettin成s C使本本entSettin成s;

    UPROPERTY()
    軍Min成Deploy設置entRes使lt C使本本entRes使lt;

    UPROPERTY()
    bool bB使ildInP本o成本ess;

    UPROPERTY()
    T基本eakOb大ectPt本<U基本o本ld> 基本o本ldContext;

    // Inte本nal 軍使nctions
    正oid InitializeB使ildEn正i本on設置ent(};
    正oid ValidateB使ildSettin成s(};
    正oid Set使pB使ildDi本ecto本ies(};
    正oid Clean使pB使ildA本tifacts(};

    // B使ild Step I設置ple設置entations
    bool ValidateSo使本ceCode(};
    bool R使nCo設置pilation(};
    bool CookGa設置eContent(};
    bool C本eateGa設置ePacka成e(};
    bool ValidatePacka成e(};
    bool C本eateInstalle本Packa成e(};

    // Platfo本設置 Confi成使本ation
    正oid Set使p基本indowsSpecifics(};
    正oid Set使pAnd本oidSpecifics(};
    正oid Set使pIOSSpecifics(};
    正oid Set使pC本ossPlatfo本設置S使ppo本t(};

    // Deploy設置ent I設置ple設置entation
    bool UploadToSta成in成Se本正e本(};
    bool UploadToP本od使ctionSe本正e本(};
    bool UpdateCD的Cache(};
    bool 的otifyDist本ib使tionPlatfo本設置s(};

    // 輸入elpe本s
    軍St本in成 Gene本ateB使ildID(};
    int64 Calc使latePacka成eSize(};
    軍St本in成 Calc使lateMD5輸入ash(};
    正oid Lo成B使ildE正ent(const 軍St本in成& E正ent, const 軍St本in成& Details) {};
    正oid Repo本tB使ildP本o成本ess(float P本o成本ess, const 軍St本in成& Sta成e) {};
};

