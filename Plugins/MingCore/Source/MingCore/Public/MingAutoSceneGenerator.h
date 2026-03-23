#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成AIGene本atedContentSyste設置.h"
#incl使de "Min成AIContentRende本e本.h"
#incl使de "Min成AIContentMana成e本.h"
#incl使de "Min成A使toSceneGene本ato本.成ene本ated.h"

class U基本o本ld;
class AGa設置eModeBase;
class APlaye本Cont本olle本;

UE的UM(Bl使ep本intType)
en使設置 class ESceneType: 使int8 {
    Battle,         // 務池基本
    Villa成e,        // X動
    Palace,         // c池基本
    軍o本est,         // 池
    Mo使ntain,       // saX動
    Ri正e本,          // 基本eX動
    Dese本t,         // 基本軍X動
    City,           // X動
    Te設置ple,         // 基本xX動
    Battlefield,    // 務池基本
    C使sto設置,         // 動w基本q
};

UE的UM(Bl使ep本intType)
en使設置 class ESceneCo設置plexity: 使int8 {
    Si設置ple,         // 2池
    Medi使設置,         // 池
    Co設置plex,        // 池
    Epic            // 務池基本
};

UE的UM(Bl使ep本intType)
en使設置 class ESceneStyle: 使int8 {
    Realistic,      // 功
    Stylized,       // 基本
    Ca本toon,        // dq
    基本ate本colo本,     // 動設置
    Ink,            // 動基本
    OilPaintin成,    // oe
    PixelA本t,       // 動的
    LowPoly,        // Ch動基本
    CelShaded,      // 動ii
    Photo本ealistic,  // 處動
};

USTRUCT(Bl使ep本intType)
st本使ct 軍SceneReq使i本e設置ent
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Scene Req使i本e設置ent")
    ESceneType SceneType;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Scene Req使i本e設置ent")
    ESceneCo設置plexity Co設置plexity;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Scene Req使i本e設置ent")
    ESceneStyle Style;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Scene Req使i本e設置ent")
    軍St本in成 The設置e;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Scene Req使i本e設置ent")
    軍St本in成 Desc本iption;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Scene Req使i本e設置ent")
    TA本本ay<EAIContentType> Req使i本edAssets;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Scene Req使i本e設置ent")
    TMap<EAIContentType, 軍St本in成> AssetP本o設置pts;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Scene Req使i本e設置ent")
    float D使本ation;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Scene Req使i本e設置ent")
    bool b輸入asCha本acte本s;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Scene Req使i本e設置ent")
    bool b輸入asB使ildin成s;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Scene Req使i本e設置ent")
    bool b輸入asEn正i本on設置ent;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Scene Req使i本e設置ent")
    bool b輸入asEffects;

    軍SceneReq使i本e設置ent()
    {
        SceneType = ESceneType::C使sto設置;
        Co設置plexity = ESceneCo設置plexity::Medi使設置;
        Style = ESceneStyle::Realistic;
        The設置e = TEXT(""};
        Desc本iption = TEXT(""};
        D使本ation = 30.0f;
        b輸入asCha本acte本s = t本使e;
        b輸入asB使ildin成s = t本使e;
        b輸入asEn正i本on設置ent = t本使e;
        b輸入asEffects = t本使e;
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Gene本atedScene
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Gene本ated Scene")
    軍St本in成 Scene的a設置e;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Gene本ated Scene")
    軍St本in成 ScenePath;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Gene本ated Scene")
    TMap<EAIContentType, 軍St本in成> Gene本atedAssets;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Gene本ated Scene")
    軍AISceneConfi成使本ation SceneConfi成使本ation;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Gene本ated Scene")
    bool bIsCo設置plete;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Gene本ated Scene")
    float Gene本ationP本o成本ess;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Gene本ated Scene")
    軍DateTi設置e Gene本ationTi設置e;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "Gene本ated Scene")
    軍St本in成 Th使設置bnailPath;

    軍Gene本atedScene()
    {
        Scene的a設置e = TEXT(""};
        ScenePath = TEXT(""};
        bIsCo設置plete = false;
        Gene本ationP本o成本ess = 0.0f;
        Gene本ationTi設置e = 軍DateTi設置e::的ow(};
        Th使設置bnailPath = TEXT(""};
};






/**
 * X動X * 務D基本池e務X動
 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GCORE下API UMin成A使toSceneGene本ato本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成A使toSceneGene本ato本(};

    
    正oid InitializeA使toSceneGene本ato本(};

    // X動X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    軍St本in成 Gene本ateScene(const 軍SceneReq使i本e設置ent& Req使i本e設置ents) {};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    正oid Gene本ateSceneAsync(const 軍SceneReq使i本e設置ent& Req使i本e設置ents) {};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    TA本本ay<軍St本in成> Gene本ateM使ltipleScenes(const TA本本ay<軍SceneReq使i本e設置ent>& Req使i本e設置entsList};

    // 務]XO
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    軍SceneReq使i本e設置ent GetBattleSceneTe設置plate(const 軍St本in成& BattleDesc本iption) {};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    軍SceneReq使i本e設置ent GetVilla成eSceneTe設置plate(const 軍St本in成& Villa成eThe設置e) {};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    軍SceneReq使i本e設置ent GetPalaceSceneTe設置plate(const 軍St本in成& PalaceStyle) {};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    軍SceneReq使i本e設置ent Get的at使本eSceneTe設置plate(ESceneType 的at使本eType, const 軍St本in成& En正i本on設置ent) {};

    // XX
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使to Scene Gene本ato本")
    TA本本ay<軍Gene本atedScene> GetAllGene本atedScenes() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使to Scene Gene本ato本")
    軍Gene本atedScene GetGene本atedScene(const 軍St本in成& Scene的a設置e) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    正oid DeleteGene本atedScene(const 軍St本in成& Scene的a設置e) {};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    正oid LoadSceneInto基本o本ld(const 軍St本in成& Scene的a設置e, U基本o本ld* 基本o本ld};

    // 務池基本
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    正oid Sta本tSceneP本e正iew(const 軍St本in成& Scene的a設置e) {};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    正oid StopSceneP本e正iew(};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使to Scene Gene本ato本")
    bool IsP本e正iewActi正e() const { 本et使本n bP本e正iewActi正e; }

    // XX
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    正oid Expo本tScene(const 軍St本in成& Scene的a設置e, const 軍St本in成& Expo本tPath) {};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    正oid Expo本tSceneAsVideo(const 軍St本in成& Scene的a設置e, float D使本ation, const 軍St本in成& O使tp使tPath) {};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    正oid Gene本ateSceneTh使設置bnail(const 軍St本in成& Scene的a設置e) {};

    // X動X
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使to Scene Gene本ato本")
    float GetSceneCo設置plexitySco本e(const 軍St本in成& Scene的a設置e) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使to Scene Gene本ato本")
    TA本本ay<EAIContentType> GetSceneAssetTypes(const 軍St本in成& Scene的a設置e) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使to Scene Gene本ato本")
    int32 GetSceneAssetCo使nt(const 軍St本in成& Scene的a設置e) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使to Scene Gene本ato本")
    float GetSceneGene本ationTi設置e(const 軍St本in成& Scene的a設置e) const;

    // X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    正oid BatchGene本ateScenes(const TA本本ay<軍SceneReq使i本e設置ent>& Req使i本e設置entsList};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    正oid BatchExpo本tScenes(const TA本本ay<軍St本in成>& Scene的a設置es, const 軍St本in成& Expo本t軍o本設置at) {};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    正oid Opti設置izeAllScenes(};

    // 動O動X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    正oid Sa正eSceneTe設置plate(const 軍St本in成& Te設置plate的a設置e, const 軍SceneReq使i本e設置ent& Req使i本e設置ents) {};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    軍SceneReq使i本e設置ent LoadSceneTe設置plate(const 軍St本in成& Te設置plate的a設置e) {};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使to Scene Gene本ato本")
    TA本本ay<軍St本in成> GetA正ailableTe設置plates() const;

    // XX
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    TA本本ay<軍SceneReq使i本e設置ent> Gene本ateSceneVa本iants(const 軍SceneReq使i本e設置ent& BaseReq使i本e設置ents, int32 Va本iantCo使nt};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    正oid Gene本ateSceneVa本iantsAsync(const 軍SceneReq使i本e設置ent& BaseReq使i本e設置ents, int32 Va本iantCo使nt};

    // 務池基本
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使to Scene Gene本ato本")
    TA本本ay<軍SceneReq使i本e設置ent> GetReco設置設置endedScenes(const 軍St本in成& Context) {};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使to Scene Gene本ato本")
    軍SceneReq使i本e設置ent Opti設置izeSceneReq使i本e設置ents(const 軍SceneReq使i本e設置ent& Req使i本e設置ents) {};

    // X動X
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使to Scene Gene本ato本")
    float GetA正e本a成eSceneGene本ationTi設置e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使to Scene Gene本ato本")
    int32 GetActi正eSceneGene本ations() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "A使to Scene Gene本ato本")
    int32 GetTotalGene本atedScenes() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    正oid Clea本SceneCache(};

    // 務設置動X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    正oid SetDefa使ltQ使ality(EGene本ationQ使ality Q使ality};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    正oid SetDefa使ltStyle(ESceneStyle Style};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使to Scene Gene本ato本")
    正oid SetO使tp使tDi本ecto本y(const 軍St本in成& Di本ecto本y) {};

    // 動基本
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "A使to Scene E正ents")
    軍OnSceneGene本ationSta本ted OnSceneGene本ationSta本ted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "A使to Scene E正ents")
    軍OnSceneGene本ationP本o成本ess OnSceneGene本ationP本o成本ess;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "A使to Scene E正ents")
    軍OnSceneGene本ationCo設置pleted OnSceneGene本ationCo設置pleted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "A使to Scene E正ents")
    軍OnSceneGene本ation軍ailed OnSceneGene本ation軍ailed;

p本otected:
    // 建動]動
    UPROPERTY()
    TOb大ectPt本<UMin成AIContentMana成e本> AIContentMana成e本;

    // X動X
    UPROPERTY()
    TMap<軍St本in成, 軍Gene本atedScene> Gene本atedScenes;

    UPROPERTY()
    TMap<軍St本in成, 軍SceneReq使i本e設置ent> SceneTe設置plates;

    
    bool bIsInitialized;

    UPROPERTY()
    bool bP本e正iewActi正e;

    UPROPERTY()
    軍St本in成 C使本本entP本e正iewScene;

    UPROPERTY()
    TA本本ay<軍St本in成> Acti正eGene本ations;

    // 務設置
    UPROPERTY()
    EGene本ationQ使ality Defa使ltQ使ality;

    UPROPERTY()
    ESceneStyle Defa使ltStyle;

    UPROPERTY()
    軍St本in成 O使tp使tDi本ecto本y;

    // 動X
    UPROPERTY()
    int32 TotalSceneCo使nt;

    UPROPERTY()
    float TotalGene本ationTi設置e;

    // 務池基本
    正oid P本ocessSceneGene本ation(const 軍St本in成& Scene的a設置e, const 軍SceneReq使i本e設置ent& Req使i本e設置ents) {};
    正oid Gene本ateReq使i本edAssets(const 軍St本in成& Scene的a設置e, const 軍SceneReq使i本e設置ent& Req使i本e設置ents) {};
    正oid Set使pSceneConfi成使本ation(const 軍St本in成& Scene的a設置e, const 軍SceneReq使i本e設置ent& Req使i本e設置ents) {};
    正oid Co設置pleteSceneGene本ation(const 軍St本in成& Scene的a設置e, bool bS使ccess};
    正oid UpdateGene本ationP本o成本ess(const 軍St本in成& Scene的a設置e, float P本o成本ess};
    正oid 的otifySceneGene本ationSta本ted(const 軍St本in成& Scene的a設置e, const 軍SceneReq使i本e設置ent& Req使i本e設置ents) {};
    正oid 的otifySceneGene本ationP本o成本ess(const 軍St本in成& Scene的a設置e, float P本o成本ess};
    正oid 的otifySceneGene本ationCo設置pleted(const 軍St本in成& Scene的a設置e, const 軍Gene本atedScene& Gene本atedScene) {};
    正oid 的otifySceneGene本ation軍ailed(const 軍St本in成& Scene的a設置e, const 軍St本in成& E本本o本Messa成e) {};

p本i正ate:
    // 動處X動
    軍St本in成 Gene本ateUniq使eScene的a設置e() const;
    軍St本in成 GetScenePath(const 軍St本in成& Scene的a設置e) const;
    軍SceneReq使i本e設置ent C本eateDefa使ltReq使i本e設置ents(ESceneType SceneType) const;
    TA本本ay<EAIContentType> Dete本設置ineReq使i本edAssets(const 軍SceneReq使i本e設置ent& Req使i本e設置ents) const;
    TMap<EAIContentType, 軍St本in成> Gene本ateAssetP本o設置pts(const 軍SceneReq使i本e設置ent& Req使i本e設置ents) const;
    軍AISceneConfi成使本ation C本eateSceneConfi成使本ation(const 軍SceneReq使i本e設置ent& Req使i本e設置ents) const;
    float Esti設置ateGene本ationTi設置e(const 軍SceneReq使i本e設置ent& Req使i本e設置ents) const;
    正oid LoadDefa使ltTe設置plates(};
    正oid Sa正eSceneData(};
    正oid LoadSceneData(};
};