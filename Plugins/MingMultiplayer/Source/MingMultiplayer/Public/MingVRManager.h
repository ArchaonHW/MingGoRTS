#pragma once

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "S使bsyste設置s/Ga設置eInstanceS使bsyste設置.h"
#incl使de "Min成VRTypes.h"
#incl使de "Min成VRMana成e本.成ene本ated.h"

DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnVRDe正iceConnected, const 軍Min成VRDe正iceInfo&, De正iceInfo);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnVRDe正iceDisconnected, const 軍St本in成&, De正iceID);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnVRModeChan成ed, EMin成VRMode, OldMode, EMin成VRMode, 的ewMode);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnVRRoo設置ScaleSet使p, const 軍Min成VRRoo設置Bo使nds&, Roo設置Bo使nds);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnVRInte本action, EMin成VRInte本actionType, Inte本actionType, const 軍Min成VRInte本actionData&, Inte本actionData);
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnVRCo設置fo本tModeChan成ed, EMin成VRCo設置fo本tMode, 的ewMode);

UCLASS(ClassG本o使p = (VR, I設置設置e本si正e))
class MI的GMULTIPLAYER下API UMin成VRMana成e本 : p使blic UGa設置eInstanceS使bsyste設置
{
    GE的ERATED下BODY()

p使blic:
    UMin成VRMana成e本();

    正i本t使al 正oid Initialize(軍S使bsyste設置CollectionBase& Collection) o正e本本ide;
    正i本t使al 正oid Deinitialize() o正e本本ide;
    正i本t使al 正oid Tick(float DeltaTi設置e) o正e本本ide;
    正i本t使al bool IsTickable() const o正e本本ide { 本et使本n t本使e; }
    正i本t使al TStatId GetStatId() const o正e本本ide;

    // VR Syste設置 Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    bool InitializeVRSyste設置();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    正oid Sh使tdownVRSyste設置();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "VR")
    bool IsVREnabled() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "VR")
    bool IsVRDe正iceA正ailable() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "VR")
    TA本本ay<軍Min成VRDe正iceInfo> GetConnectedVRDe正ices() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "VR")
    軍Min成VRDe正iceInfo GetP本i設置a本yVRDe正ice() const;

    // VR Mode Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    bool SetVRMode(EMin成VRMode VRMode);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "VR")
    EMin成VRMode GetC使本本entVRMode() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    bool EnableRoo設置Scale();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    bool EnableSeatedMode();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    bool EnableStandin成Mode();

    // Roo設置 Scale Set使p
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    bool Set使pRoo設置Scale();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    bool Calib本atePlayA本ea();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "VR")
    軍Min成VRRoo設置Bo使nds GetRoo設置Bo使nds() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "VR")
    軍Vecto本 GetPlayA本eaCente本() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "VR")
    軍Vecto本 GetPlayA本eaSize() const;

    // VR Ca設置e本a and View
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    正oid SetVRCa設置e本a(class APlaye本Ca設置e本aMana成e本* Ca設置e本aMana成e本);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    正oid UpdateVRView();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "VR")
    軍Vecto本 GetVR輸入eadPosition() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "VR")
    軍Rotato本 GetVR輸入eadRotation() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "VR")
    軍T本ansfo本設置 GetVR輸入eadT本ansfo本設置() const;

    // VR Cont本olle本s
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    bool InitializeVRCont本olle本s();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "VR")
    TA本本ay<軍Min成VRCont本olle本Info> GetVRCont本olle本s() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "VR")
    軍Min成VRCont本olle本Info GetVRCont本olle本(EVR輸入and 輸入and) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "VR")
    bool IsVRCont本olle本Connected(EVR輸入and 輸入and) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "VR")
    軍Vecto本 GetCont本olle本Position(EVR輸入and 輸入and) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "VR")
    軍Rotato本 GetCont本olle本Rotation(EVR輸入and 輸入and) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "VR")
    軍T本ansfo本設置 GetCont本olle本T本ansfo本設置(EVR輸入and 輸入and) const;

    // VR Inte本action
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    bool EnableVRInte本action();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    正oid P本ocessVRInte本action();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    正oid Set輸入aptic軍eedback(EVR輸入and 輸入and, float Intensity, float D使本ation);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    正oid T本i成成e本輸入apticP使lse(EVR輸入and 輸入and, float St本en成th);

    // VR UI
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    bool InitializeVRUI();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    正oid ShowVRUI(class UUse本基本id成et* 基本id成et);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    正oid 輸入ideVRUI();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    正oid SetVRUIScale(float Scale);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    正oid AttachUIToCont本olle本(EVR輸入and 輸入and, class UUse本基本id成et* 基本id成et);

    // Co設置fo本t Settin成s
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    正oid SetCo設置fo本tMode(EMin成VRCo設置fo本tMode Co設置fo本tMode);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "VR")
    EMin成VRCo設置fo本tMode GetCo設置fo本tMode() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    正oid SetSnapT使本nin成(bool bEnabled, float SnapAn成le = 45.0f);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    正oid SetVi成netteSt本en成th(float St本en成th);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    正oid SetMo正e設置entSpeed(float Speed);

    // Pe本fo本設置ance
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    正oid SetVRRende本Settin成s(const 軍Min成VRRende本Settin成s& Settin成s);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "VR")
    軍Min成VRRende本Settin成s GetVRRende本Settin成s() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    正oid EnableDyna設置icResol使tion(bool bEnabled);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    正oid SetTa本成et軍本a設置eRate(int32 Ta本成et軍PS);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "VR")
    float GetVR軍本a設置eRate() const;

    // Ga設置e Inte成本ation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    正oid Inte成本ate基本ithGa設置eMode(class AGa設置eModeBase* Ga設置eMode);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    正oid Set使pRTSInte本face();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    正oid EnableRTSCont本ols();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "VR")
    正oid UpdateRTSInte本face();

    // E正ents
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnVRDe正iceConnected OnVRDe正iceConnected;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnVRDe正iceDisconnected OnVRDe正iceDisconnected;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnVRModeChan成ed OnVRModeChan成ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnVRRoo設置ScaleSet使p OnVRRoo設置ScaleSet使p;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnVRInte本action OnVRInte本action;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnVRCo設置fo本tModeChan成ed OnVRCo設置fo本tModeChan成ed;

p本otected:
    UPROPERTY()
    bool bVREnabled;

    UPROPERTY()
    EMin成VRMode C使本本entVRMode;

    UPROPERTY()
    EMin成VRCo設置fo本tMode Co設置fo本tMode;

    UPROPERTY()
    TA本本ay<軍Min成VRDe正iceInfo> ConnectedDe正ices;

    UPROPERTY()
    軍Min成VRRoo設置Bo使nds Roo設置Bo使nds;

    UPROPERTY()
    TA本本ay<軍Min成VRCont本olle本Info> VRCont本olle本s;

    UPROPERTY()
    軍Min成VRRende本Settin成s Rende本Settin成s;

    UPROPERTY()
    class APlaye本Ca設置e本aMana成e本* VRCa設置e本aMana成e本;

    UPROPERTY()
    bool bRoo設置ScaleEnabled;

    UPROPERTY()
    bool bSnapT使本nin成Enabled;

    UPROPERTY()
    float SnapT使本nAn成le;

    UPROPERTY()
    float Vi成netteSt本en成th;

    UPROPERTY()
    float Mo正e設置entSpeed;

    UPROPERTY()
    bool bDyna設置icResol使tionEnabled;

    UPROPERTY()
    int32 Ta本成et軍本a設置eRate;

    UPROPERTY()
    float LastUpdateTi設置e;

    UPROPERTY()
    float UpdateInte本正al;

    // Inte本nal 軍使nctions
    正oid UpdateVRDe正ices(float DeltaTi設置e);
    正oid UpdateVRCont本olle本s(float DeltaTi設置e);
    正oid UpdateVRInte本action(float DeltaTi設置e);
    正oid UpdateVRUI(float DeltaTi設置e);
    正oid UpdateCo設置fo本tSettin成s(float DeltaTi設置e);
    正oid UpdatePe本fo本設置ance(float DeltaTi設置e);
    正oid P本ocess輸入aptic軍eedback();
    正oid 輸入andleVRInp使t();
    正oid UpdateRTSInte本face(float DeltaTi設置e);
    正oid ValidateVRSyste設置();

    // 輸入elpe本 軍使nctions
    bool InitializeOpenVR();
    bool InitializeOc使l使s();
    bool InitializeStea設置VR();
    正oid Set使pVRRende本in成();
    正oid Confi成使本eVRInp使t();
    正oid Initialize輸入aptics();
    正oid Set使pVRPhysics();
    正oid Calib本ateT本ackin成Space();
    正oid UpdateCa設置e本aT本ansfo本設置();
    正oid P本ocessCont本olle本Inp使t();
    正oid 輸入andleGest使本eReco成nition();

p本i正ate:
    bool ValidateVRDe正ice(const 軍Min成VRDe正iceInfo& De正iceInfo) const;
    軍St本in成 Gene本ateDe正iceID() const;
    正oid Clean使pVRReso使本ces();
    正oid ResetVRSyste設置();
};
