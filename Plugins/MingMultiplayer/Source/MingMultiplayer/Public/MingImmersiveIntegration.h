#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "S使bsyste設置s/Ga設置eInstanceS使bsyste設置.h"
#incl使de "Min成M使ltiplaye本Types.h"
#incl使de "Min成Ed使cationalMana成e本.h"
#incl使de "Min成VRMana成e本.h"
#incl使de "Min成ARMana成e本.h"
#incl使de "Min成I設置設置e本si正eInte成本ation.成ene本ated.h"

DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnI設置設置e本si正eModeChan成ed, EMin成I設置設置e本si正eMode, OldMode, EMin成I設置設置e本si正eMode, 的ewMode);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnEd使cationalContentReady, const 軍Min成Ed使cationalContent&, Content);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnVRSceneGene本ated, const 軍St本in成&, SceneID);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnARContentPlaced, const 軍St本in成&, ContentID);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnLea本nin成P本o成本essUpdated, const 軍St本in成&, Playe本ID, const 軍Min成Ed使cationalP本o成本ess&, P本o成本ess);

UE的UM(Bl使ep本intType)
en使設置 class EMin成I設置設置e本si正eMode: 使int8 {
    的one            = 0,
    Ed使cational     = 1,
    VR              = 2,
    AR              = 3,
    Mixed           = 4,
    輸入yb本id          = 5
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成I設置設置e本si正eConfi成
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "I設置設置e本si正e Confi成")
    EMin成I設置設置e本si正eMode P本i設置a本yMode;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "I設置設置e本si正e Confi成")
    bool bEd使cationalContentEnabled;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "I設置設置e本si正e Confi成")
    bool bVREnabled;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "I設置設置e本si正e Confi成")
    bool bAREnabled;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "I設置設置e本si正e Confi成")
    EMin成Ed使cationalMode Ed使cationalMode;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "I設置設置e本si正e Confi成")
    EMin成VRMode VRMode;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "I設置設置e本si正e Confi成")
    bool bAREd使cationalMode;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "I設置設置e本si正e Confi成")
    float T本ansitionD使本ation;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "I設置設置e本si正e Confi成")
    bool bA使toSwitchModes;

    軍Min成I設置設置e本si正eConfi成()
    {
        P本i設置a本yMode = EMin成I設置設置e本si正eMode::的one;
        bEd使cationalContentEnabled = false;
        bVREnabled = false;
        bAREnabled = false;
        Ed使cationalMode = EMin成Ed使cationalMode::Disabled;
        VRMode = EMin成VRMode::Disabled;
        bAREd使cationalMode = false;
        T本ansitionD使本ation = 2.0f;
        bA使toSwitchModes = t本使e;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Ed使cationalVRContent
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Ed使cational VR Content")
    軍St本in成 ContentID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Ed使cational VR Content")
    軍St本in成 LessonID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Ed使cational VR Content")
    軍St本in成 VRSceneID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Ed使cational VR Content")
    軍Vecto本3d 基本o本ldPosition;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Ed使cational VR Content")
    軍Rotato本3d 基本o本ldRotation;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Ed使cational VR Content")
    軍Vecto本3d Scale;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Ed使cational VR Content")
    bool bInte本acti正e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Ed使cational VR Content")
    TA本本ay<軍St本in成> Req使i本edAssets;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Ed使cational VR Content")
    TA本本ay<軍St本in成> A使dioClips;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Ed使cational VR Content")
    軍St本in成 Desc本iption;

    軍Min成Ed使cationalVRContent()
    {
        ContentID = TEXT("");
        LessonID = TEXT("");
        VRSceneID = TEXT("");
        基本o本ldPosition = 軍Vecto本3d::Ze本oVecto本;
        基本o本ldRotation = 軍Rotato本3d::Ze本oRotato本;
        Scale = 軍Vecto本3d::OneVecto本;
        bInte本acti正e = false;
        Desc本iption = TEXT("");
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Ed使cationalARContent
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Ed使cational AR Content")
    軍St本in成 ContentID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Ed使cational AR Content")
    軍St本in成 輸入isto本icalMa本ke本ID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Ed使cational AR Content")
    軍St本in成 Ancho本ID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Ed使cational AR Content")
    EMin成ARPlaneType Ta本成etPlaneType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Ed使cational AR Content")
    軍Vecto本3d LocalPosition;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Ed使cational AR Content")
    軍Rotato本3d LocalRotation;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Ed使cational AR Content")
    軍Vecto本3d Scale;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Ed使cational AR Content")
    bool bPe本sistent;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Ed使cational AR Content")
    TA本本ay<軍St本in成> MediaAssets;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Ed使cational AR Content")
    軍St本in成 Ed使cationalText;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Ed使cational AR Content")
    TA本本ay<軍St本in成> Inte本acti正eEle設置ents;

    軍Min成Ed使cationalARContent()
    {
        ContentID = TEXT("");
        輸入isto本icalMa本ke本ID = TEXT("");
        Ancho本ID = TEXT("");
        Ta本成etPlaneType = EMin成ARPlaneType::輸入o本izontalUp;
        LocalPosition = 軍Vecto本3d::Ze本oVecto本;
        LocalRotation = 軍Rotato本3d::Ze本oRotato本;
        Scale = 軍Vecto本3d::OneVecto本;
        bPe本sistent = false;
        Ed使cationalText = TEXT("");
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成I設置設置e本si正eT本ansition
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "I設置設置e本si正e T本ansition")
    EMin成I設置設置e本si正eMode 軍本o設置Mode;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "I設置設置e本si正e T本ansition")
    EMin成I設置設置e本si正eMode ToMode;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "I設置設置e本si正e T本ansition")
    float D使本ation;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "I設置設置e本si正e T本ansition")
    bool b軍adeToBlack;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "I設置設置e本si正e T本ansition")
    bool bLoadAssets;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "I設置設置e本si正e T本ansition")
    TA本本ay<軍St本in成> AssetsToLoad;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "I設置設置e本si正e T本ansition")
    軍St本in成 T本ansitionEffect;

    軍Min成I設置設置e本si正eT本ansition()
    {
        軍本o設置Mode = EMin成I設置設置e本si正eMode::的one;
        ToMode = EMin成I設置設置e本si正eMode::的one;
        D使本ation = 2.0f;
        b軍adeToBlack = t本使e;
        bLoadAssets = t本使e;
        T本ansitionEffect = TEXT("Defa使lt");
    }
};

/**
 * Unified I設置設置e本si正e Expe本ience Mana成e本
 * Inte成本ates ed使cational, VR, and AR syste設置s fo本 sea設置less i設置設置e本si正e lea本nin成 expe本iences
 */
UCLASS(ClassG本o使p = (I設置設置e本si正e, Lea本nin成), Bl使ep本intType)
class MI的GMULTIPLAYER下API UMin成I設置設置e本si正eInte成本ation : p使blic UGa設置eInstanceS使bsyste設置
{
    GE的ERATED下BODY()

p使blic:
    UMin成I設置設置e本si正eInte成本ation();

    正i本t使al 正oid Initialize(軍S使bsyste設置CollectionBase& Collection) o正e本本ide;
    正i本t使al 正oid Deinitialize() o正e本本ide;
    正i本t使al 正oid Tick(float DeltaTi設置e) o正e本本ide;
    正i本t使al bool IsTickable() const o正e本本ide { 本et使本n t本使e; }
    正i本t使al TStatId GetStatId() const o正e本本ide;

    // I設置設置e本si正e Mode Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "I設置設置e本si正e")
    bool SetI設置設置e本si正eMode(EMin成I設置設置e本si正eMode Mode);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "I設置設置e本si正e")
    bool SwitchToMode(EMin成I設置設置e本si正eMode Mode, bool bS設置oothT本ansition = t本使e);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "I設置設置e本si正e")
    EMin成I設置設置e本si正eMode GetC使本本entI設置設置e本si正eMode() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "I設置設置e本si正e")
    正oid Confi成使本eI設置設置e本si正eExpe本ience(const 軍Min成I設置設置e本si正eConfi成& Confi成);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "I設置設置e本si正e")
    軍Min成I設置設置e本si正eConfi成 GetC使本本entConfi成() const;

    // Ed使cational-VR Inte成本ation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational VR")
    bool C本eateEd使cationalVRScene(const 軍St本in成& LessonID, const 軍Vecto本3d& Location, const 軍Rotato本3d& Rotation);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational VR")
    bool LoadEd使cationalVRContent(const 軍St本in成& ContentID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational VR")
    正oid UnloadEd使cationalVRContent(const 軍St本in成& ContentID);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational VR")
    TA本本ay<軍Min成Ed使cationalVRContent> GetLoadedVRContent() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational VR")
    bool Sta本tVR輸入isto本icalTo使本(const TA本本ay<軍St本in成>& 輸入isto本icalE正entIDs);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational VR")
    正oid StopVR輸入isto本icalTo使本();

    // Ed使cational-AR Inte成本ation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational AR")
    bool PlaceEd使cationalARContent(const 軍St本in成& 輸入isto本icalMa本ke本ID, const 軍Vecto本3d& Location);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational AR")
    bool C本eateAR輸入isto本icalTi設置eline(const TA本本ay<軍St本in成>& E正entIDs);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational AR")
    bool Sta本tAREd使cationalMode();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ed使cational AR")
    正oid StopAREd使cationalMode();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ed使cational AR")
    TA本本ay<軍Min成Ed使cationalARContent> GetPlacedARContent() const;

    // C本oss-Platfo本設置 Lea本nin成
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "C本oss-Platfo本設置")
    bool SyncLea本nin成P本o成本essAc本ossPlatfo本設置s();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "C本oss-Platfo本設置")
    正oid T本ansfe本Lea本nin成Data(EMin成I設置設置e本si正eMode 軍本o設置Platfo本設置, EMin成I設置設置e本si正eMode ToPlatfo本設置);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "C本oss-Platfo本設置")
    bool IsC本ossPlatfo本設置SyncEnabled() const;

    // Adapti正e I設置設置e本si正e Lea本nin成
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Adapti正e Lea本nin成")
    正oid EnableAdapti正eI設置設置e本si正eLea本nin成(bool bEnabled);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Adapti正e Lea本nin成")
    bool IsAdapti正eI設置設置e本si正eLea本nin成Enabled() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Adapti正e Lea本nin成")
    正oid UpdateI設置設置e本si正eDiffic使ltyBasedOnPe本fo本設置ance();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Adapti正e Lea本nin成")
    軍Min成Ed使cationalContent GetAdaptedI設置設置e本si正eContent(const 軍St本in成& ContentID);

    // M使lti-Use本 I設置設置e本si正e Lea本nin成
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti-Use本")
    bool EnableM使ltiUse本I設置設置e本si正eLea本nin成();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti-Use本")
    正oid Sta本tCoope本ati正eI設置設置e本si正eSession(const TA本本ay<int32>& Playe本IDs);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti-Use本")
    正oid Sha本eI設置設置e本si正eLea本nin成P本o成本ess();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使lti-Use本")
    正oid Recei正eSha本edP本o成本ess(const 軍Min成Ed使cationalP本o成本ess& P本o成本ess);

    // I設置設置e本si正e Analytics
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Analytics")
    軍Min成Ed使cationalP本o成本ess GetI設置設置e本si正eLea本nin成Analytics() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Analytics")
    float GetI設置設置e本si正eEn成a成e設置entSco本e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Analytics")
    TA本本ay<EMin成I設置設置e本si正eMode> GetMostUsedModes() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Analytics")
    正oid Gene本ateI設置設置e本si正eLea本nin成Repo本t();

    // Content Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Content Mana成e設置ent")
    bool P本eloadI設置設置e本si正eContent(const TA本本ay<軍St本in成>& ContentIDs);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Content Mana成e設置ent")
    正oid UnloadUn使sedContent();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Content Mana成e設置ent")
    float GetContentLoadP本o成本ess() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Content Mana成e設置ent")
    正oid Opti設置izeContent軍o本C使本本entPlatfo本設置();

    // E正ents
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnI設置設置e本si正eModeChan成ed OnI設置設置e本si正eModeChan成ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnEd使cationalContentReady OnEd使cationalContentReady;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnVRSceneGene本ated OnVRSceneGene本ated;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnARContentPlaced OnARContentPlaced;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnLea本nin成P本o成本essUpdated OnLea本nin成P本o成本essUpdated;

p本otected:
    UPROPERTY()
    EMin成I設置設置e本si正eMode C使本本entI設置設置e本si正eMode;

    UPROPERTY()
    軍Min成I設置設置e本si正eConfi成 C使本本entConfi成;

    UPROPERTY()
    TA本本ay<軍Min成Ed使cationalVRContent> LoadedVRContent;

    UPROPERTY()
    TA本本ay<軍Min成Ed使cationalARContent> PlacedARContent;

    UPROPERTY()
    TA本本ay<軍Min成I設置設置e本si正eT本ansition> T本ansitionQ使e使e;

    UPROPERTY()
    bool bAdapti正eI設置設置e本si正eLea本nin成Enabled;

    UPROPERTY()
    bool bM使ltiUse本I設置設置e本si正eLea本nin成Enabled;

    UPROPERTY()
    bool bC本ossPlatfo本設置SyncEnabled;

    UPROPERTY()
    float C使本本entT本ansitionP本o成本ess;

    UPROPERTY()
    float LastUpdateTi設置e;

    UPROPERTY()
    float UpdateInte本正al;

    // S使bsyste設置 Refe本ences
    UPROPERTY()
    TOb大ectPt本<UMin成Ed使cationalMana成e本> Ed使cationalMana成e本;

    UPROPERTY()
    TOb大ectPt本<UMin成VRMana成e本> VRMana成e本;

    UPROPERTY()
    TOb大ectPt本<UMin成ARMana成e本> ARMana成e本;

    // Inte本nal 軍使nctions
    正oid UpdateI設置設置e本si正eInte成本ation(float DeltaTi設置e);
    正oid P本ocessModeT本ansitions(float DeltaTi設置e);
    正oid UpdateEd使cationalContent(float DeltaTi設置e);
    正oid UpdateVRContent(float DeltaTi設置e);
    正oid UpdateARContent(float DeltaTi設置e);
    正oid SyncEd使cationalSyste設置s(float DeltaTi設置e);
    正oid ValidateI設置設置e本si正eState();

    // T本ansition Mana成e設置ent
    正oid Sta本tModeT本ansition(EMin成I設置設置e本si正eMode 軍本o設置Mode, EMin成I設置設置e本si正eMode ToMode);
    正oid P本ocessT本ansition(float DeltaTi設置e);
    正oid Co設置pleteModeT本ansition(EMin成I設置設置e本si正eMode 的ewMode);
    正oid CancelC使本本entT本ansition();

    // Content Inte成本ation
    正oid Inte成本ateEd使cationalContent基本ithVR(const 軍St本in成& LessonID);
    正oid Inte成本ateEd使cationalContent基本ithAR(const 軍St本in成& 輸入isto本icalMa本ke本ID);
    正oid Gene本ateC本ossPlatfo本設置Content(const 軍St本in成& ContentID);
    正oid Opti設置izeContent軍o本Mode(EMin成I設置設置e本si正eMode Mode);

    // Lea本nin成 Analytics
    正oid UpdateLea本nin成Analytics(float DeltaTi設置e);
    正oid Calc使lateEn成a成e設置entSco本e();
    正oid T本ackModeUsa成e(EMin成I設置設置e本si正eMode Mode);
    正oid Gene本ateLea本nin成Insi成hts();

p本i正ate:
    bool ValidateI設置設置e本si正eConfi成(const 軍Min成I設置設置e本si正eConfi成& Confi成) const;
    bool CanT本ansitionToMode(EMin成I設置設置e本si正eMode Mode) const;
    軍St本in成 Gene本ateContentID() const;
    正oid Clean使pI設置設置e本si正eReso使本ces();
    正oid ResetI設置設置e本si正eSyste設置();
};
