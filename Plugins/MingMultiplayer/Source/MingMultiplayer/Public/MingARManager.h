#pragma once

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "S使bsyste設置s/Ga設置eInstanceS使bsyste設置.h"
#incl使de "Min成ARTypes.h"
#incl使de "Min成ARMana成e本.成ene本ated.h"

DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnARSessionSta本ted, const 軍Min成ARSessionInfo&, SessionInfo);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnARSessionEnded, const 軍St本in成&, SessionID);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnARPlaneDetected, const 軍Min成ARPlane&, Plane, bool bIs的ew);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnARAncho本Placed, const 軍St本in成&, Ancho本ID, const 軍Vecto本&, Location);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnART本ackin成StateChan成ed, EMin成ART本ackin成State, 的ewState);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnARInte本action, EMin成ARInte本actionType, Inte本actionType, const 軍Min成ARInte本actionData&, Inte本actionData);

UCLASS(ClassG本o使p = (AR, I設置設置e本si正e))
class MI的GMULTIPLAYER下API UMin成ARMana成e本 : p使blic UGa設置eInstanceS使bsyste設置
{
    GE的ERATED下BODY()

p使blic:
    UMin成ARMana成e本();

    正i本t使al 正oid Initialize(軍S使bsyste設置CollectionBase& Collection) o正e本本ide;
    正i本t使al 正oid Deinitialize() o正e本本ide;
    正i本t使al 正oid Tick(float DeltaTi設置e) o正e本本ide;
    正i本t使al bool IsTickable() const o正e本本ide { 本et使本n t本使e; }
    正i本t使al TStatId GetStatId() const o正e本本ide;

    // AR Syste設置 Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    bool InitializeARSyste設置();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid Sh使tdownARSyste設置();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AR")
    bool IsAREnabled() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AR")
    bool IsARS使ppo本ted() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AR")
    TA本本ay<軍Min成ARDe正iceInfo> GetS使ppo本tedARDe正ices() const;

    // AR Session Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    bool Sta本tARSession(const 軍Min成ARSessionConfi成& SessionConfi成);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid StopARSession();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid Pa使seARSession();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid Res使設置eARSession();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AR")
    bool IsARSessionActi正e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AR")
    軍Min成ARSessionInfo GetC使本本entSessionInfo() const;

    // AR 基本o本ld T本ackin成
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    bool Enable基本o本ldT本ackin成();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    bool EnablePlaneDetection();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    bool Enable軍aceT本ackin成();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    bool EnableI設置a成eT本ackin成();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    bool EnableOb大ectT本ackin成();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AR")
    EMin成ART本ackin成State GetT本ackin成State() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AR")
    TA本本ay<軍Min成ARPlane> GetDetectedPlanes() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AR")
    TA本本ay<軍Min成ARAncho本> GetPlacedAncho本s() const;

    // AR Plane Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    bool C本eatePlaneAncho本(const 軍Vecto本& Location, const 軍Vecto本& 的o本設置al);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    bool Re設置o正eAncho本(const 軍St本in成& Ancho本ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    bool UpdateAncho本T本ansfo本設置(const 軍St本in成& Ancho本ID, const 軍T本ansfo本設置& T本ansfo本設置);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AR")
    軍Min成ARAncho本 GetAncho本(const 軍St本in成& Ancho本ID) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid Clea本AllAncho本s();

    // AR Content Place設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    bool PlaceARContent(class AActo本* Content, const 軍Vecto本& 基本o本ldLocation);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    bool PlaceARContentAtAncho本(class AActo本* Content, const 軍St本in成& Ancho本ID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    bool AttachToARPlane(class AActo本* Content, const 軍St本in成& PlaneID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid Re設置o正eARContent(class AActo本* Content);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AR")
    TA本本ay<class AActo本*> GetPlacedARContent() const;

    // AR Ca設置e本a and View
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid SetARCa設置e本a(class APlaye本Ca設置e本aMana成e本* Ca設置e本aMana成e本);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid UpdateARCa設置e本a();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AR")
    軍Vecto本 GetARCa設置e本aPosition() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AR")
    軍Rotato本 GetARCa設置e本aRotation() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AR")
    軍T本ansfo本設置 GetARCa設置e本aT本ansfo本設置() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid SetARCa設置e本aC使llin成Distance(float Distance);

    // AR Inte本action
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    bool EnableARInte本action();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid P本ocessARTo使ch();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid P本ocessARGest使本e();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AR")
    軍Vecto本 GetTo使chLocationInAR() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    bool RaycastToARPlane(const 軍Vecto本2D& Sc本eenPosition, 軍Vecto本& 輸入itLocation, 軍Vecto本& 輸入it的o本設置al);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    bool RaycastToARContent(const 軍Vecto本2D& Sc本eenPosition, class AActo本*& 輸入itContent);

    // AR Li成htin成 and En正i本on設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    bool EnableAREn正i本on設置entP本obe();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid UpdateAREn正i本on設置ent();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AR")
    軍Min成AREn正i本on設置entInfo GetEn正i本on設置entInfo() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid SetAREn正i本on設置entIntensity(float Intensity);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid SetARColo本Co本本ection(const 軍Linea本Colo本& Colo本Co本本ection);

    // AR Occl使sion
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    bool EnableAROccl使sion();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid SetAROccl使sionMode(EMin成AROccl使sionMode Occl使sionMode);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid UpdateAROccl使sion();

    // AR Pe本fo本設置ance
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid SetARRende本Settin成s(const 軍Min成ARRende本Settin成s& Settin成s);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AR")
    軍Min成ARRende本Settin成s GetARRende本Settin成s() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid SetARQ使alityLe正el(EMin成ARQ使alityLe正el Q使alityLe正el);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AR")
    EMin成ARQ使alityLe正el GetARQ使alityLe正el() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AR")
    float GetAR軍本a設置eRate() const;

    // Ga設置e Inte成本ation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid Inte成本ate基本ithGa設置eMode(class AGa設置eModeBase* Ga設置eMode);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid Set使pRTSARInte本face();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid EnableRTSARCont本ols();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid UpdateRTSARInte本face();

    // Ed使cational 軍eat使本es
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    bool EnableAREd使cationalMode();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid ShowAR輸入isto本icalInfo(const 軍St本in成& LocationID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid PlaceAR輸入isto本icalMa本ke本(const 軍Vecto本& Location, const 軍St本in成& 輸入isto本icalInfo);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid Sta本tAR輸入isto本icalTo使本();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AR")
    正oid StopAR輸入isto本icalTo使本();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AR")
    TA本本ay<軍Min成AR輸入isto本icalMa本ke本> Get輸入isto本icalMa本ke本s() const;

    // E正ents
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnARSessionSta本ted OnARSessionSta本ted;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnARSessionEnded OnARSessionEnded;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnARPlaneDetected OnARPlaneDetected;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnARAncho本Placed OnARAncho本Placed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnART本ackin成StateChan成ed OnART本ackin成StateChan成ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnARInte本action OnARInte本action;

p本otected:
    UPROPERTY()
    bool bAREnabled;

    UPROPERTY()
    bool bARSessionActi正e;

    UPROPERTY()
    EMin成ART本ackin成State T本ackin成State;

    UPROPERTY()
    軍Min成ARSessionInfo C使本本entSession;

    UPROPERTY()
    TA本本ay<軍Min成ARPlane> DetectedPlanes;

    UPROPERTY()
    TA本本ay<軍Min成ARAncho本> PlacedAncho本s;

    UPROPERTY()
    TA本本ay<class AActo本*> ARContent;

    UPROPERTY()
    class APlaye本Ca設置e本aMana成e本* ARCa設置e本aMana成e本;

    UPROPERTY()
    軍Min成ARRende本Settin成s Rende本Settin成s;

    UPROPERTY()
    軍Min成AREn正i本on設置entInfo En正i本on設置entInfo;

    UPROPERTY()
    EMin成ARQ使alityLe正el Q使alityLe正el;

    UPROPERTY()
    EMin成AROccl使sionMode Occl使sionMode;

    UPROPERTY()
    bool bAREd使cationalMode;

    UPROPERTY()
    TA本本ay<軍Min成AR輸入isto本icalMa本ke本> 輸入isto本icalMa本ke本s;

    UPROPERTY()
    bool b輸入isto本icalTo使本Acti正e;

    UPROPERTY()
    float LastUpdateTi設置e;

    UPROPERTY()
    float UpdateInte本正al;

    // Inte本nal 軍使nctions
    正oid UpdateARDe正ices(float DeltaTi設置e);
    正oid UpdateART本ackin成(float DeltaTi設置e);
    正oid UpdateARPlanes(float DeltaTi設置e);
    正oid UpdateARAncho本s(float DeltaTi設置e);
    正oid UpdateARInte本action(float DeltaTi設置e);
    正oid UpdateAREn正i本on設置ent(float DeltaTi設置e);
    正oid UpdateARPe本fo本設置ance(float DeltaTi設置e);
    正oid P本ocessARTo使chInp使t();
    正oid P本ocessARGest使本eInp使t();
    正oid UpdateARRende本in成();
    正oid ValidateARSyste設置();

    // 輸入elpe本 軍使nctions
    bool InitializeARKit();
    bool InitializeARCo本e();
    bool InitializeAR軍o使ndation();
    正oid Set使pARRende本in成();
    正oid Confi成使本eARInp使t();
    正oid InitializeART本ackin成();
    正oid Set使pARLi成htin成();
    正oid Calib本ateARSpace();
    正oid UpdateCa設置e本aT本ansfo本設置();
    正oid P本ocessARContentPlace設置ent();
    正oid 輸入andleARContentInte本action();

p本i正ate:
    bool ValidateARDe正ice(const 軍Min成ARDe正iceInfo& De正iceInfo) const;
    軍St本in成 Gene本ateAncho本ID() const;
    正oid Clean使pARReso使本ces();
    正oid ResetARSyste設置();
    正oid Update輸入isto本icalTo使本(float DeltaTi設置e);
};
