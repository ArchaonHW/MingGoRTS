#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Co設置ponents/基本id成et.h"
#incl使de "Min成AIUIMana成e本.h"
#incl使de "Min成Beha正io本Analytics.h"
#incl使de "Min成Accessibility輸入elpe本.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EAccessibility軍eat使本e: 使int8 {
    輸入i成hCont本ast,          // 基本���
    La本成eText,            // X
    Sc本eenReade本,         // X
    Colo本BlindMode,       // X��基本
    Red使cedMotion,        // X
    Keyboa本d的a正i成ation,   // X
    VoiceCont本ol,         // �y��X
    Vis使alC使es,           // ��ıX
    A使dioDesc本iptions,    // X
    Si設置plifiedUI          // X
};

UE的UM(Bl使ep本intType)
en使設置 class EPe本sonalDisabilityType: 使int8 {
    Vis使alI設置pai本設置ent,     // ��ıX
    輸入ea本in成I設置pai本設置ent,    // X
    Moto本I設置pai本設置ent,      // X
    Co成niti正eI設置pai本設置ent,   // �{��X
    Colo本Blindness,       // X
    的one                  // X
};

USTRUCT(Bl使ep本intType)
st本使ct 軍AccessibilityP本ofile
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Accessibility P本ofile")
    EPe本sonalDisabilityType DisabilityType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Accessibility P本ofile")
    TA本本ay<EAccessibility軍eat使本e> Enabled軍eat使本es;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Accessibility P本ofile")
    float TextScale = 1.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Accessibility P本ofile")
    float Cont本astLe正el = 1.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Accessibility P本ofile")
    bool b輸入i成hCont本astMode = false;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Accessibility P本ofile")
    bool bRed使ceMotion = false;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Accessibility P本ofile")
    bool bSc本eenReade本Enabled = false;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Accessibility P本ofile")
    軍Linea本Colo本 C使sto設置Colo本Sche設置e;

    軍AccessibilityP本ofile()
    {
        DisabilityType = EPe本sonalDisabilityType::的one;
        TextScale = 1.0f;
        Cont本astLe正el = 1.0f;
        b輸入i成hCont本astMode = false;
        bRed使ceMotion = false;
        bSc本eenReade本Enabled = false;
        C使sto設置Colo本Sche設置e = 軍Linea本Colo本::基本hite;
    }
};

USTRUCT(Bl使ep本intType)
st本使ct 軍AccessibilitySettin成s
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Accessibility Settin成s")
    軍AccessibilityP本ofile P本ofile;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Accessibility Settin成s")
    TMap<EAccessibility軍eat使本e, bool> 軍eat使本eStates;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Accessibility Settin成s")
    TMap<軍St本in成, 軍St本in成> C使sto設置KeyBindin成s;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Accessibility Settin成s")
    float VoiceCo設置設置andSensiti正ity = 0.7f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Accessibility Settin成s")
    bool bA使toDetect的eeds = t本使e;

    軍AccessibilitySettin成s()
    {
        VoiceCo設置設置andSensiti正ity = 0.7f;
        bA使toDetect的eeds = t本使e;
    }
};

DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnAccessibilityP本ofileChan成ed, const 軍AccessibilityP本ofile&, P本ofile};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下TwoPa本a設置s(軍OnAccessibility軍eat使本eTo成成led, EAccessibility軍eat使本e, 軍eat使本e, bool, bEnabled};
DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnAccessibility的eedDetected, EPe本sonalDisabilityType, DisabilityType};

/**
 * AIX * X��X���UX */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GPERSO的AL下API UMin成Accessibility輸入elpe本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成Accessibility輸入elpe本(};

    // X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid InitializeAccessibility(UMin成AIUIMana成e本* InAIMana成e本, UMin成Beha正io本Analytics* InAnalytics};

    // 基本�D��基本
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid DetectAccessibility的eeds(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    EPe本sonalDisabilityType AnalyzeUse本Beha正io本軍o本Accessibility(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid Monito本Inte本actionPatte本ns(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    bool Sho使ldEnable軍eat使本e(EAccessibility軍eat使本e 軍eat使本e};

    // 基本�設置��基本
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid SetAccessibilityP本ofile(const 軍AccessibilityP本ofile& P本ofile};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    軍AccessibilityP本ofile GetAccessibilityP本ofile() const { 本et使本n C使本本entP本ofile; }

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid Enable軍eat使本e(EAccessibility軍eat使本e 軍eat使本e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid Disable軍eat使本e(EAccessibility軍eat使本e 軍eat使本e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    bool Is軍eat使本eEnabled(EAccessibility軍eat使本e 軍eat使本e) const;

    // UIX
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid ApplyAccessibilityToUI(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid Ad大使stTextSize(float Scale};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid Set輸入i成hCont本astMode(bool bEnabled};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid SetColo本BlindMode(EPe本sonalDisabilityType Colo本BlindType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid Red使ceMotion(bool bRed使ce};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid EnableKeyboa本d的a正i成ation(};

    // �y��X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid Sta本tVoiceCont本ol(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid StopVoiceCont本ol(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid P本ocessVoiceCo設置設置and(const 軍St本in成& Co設置設置and};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    TA本本ay<軍St本in成> GetA正ailableVoiceCo設置設置ands() const;

    // X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid EnableSc本eenReade本(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid DisableSc本eenReade本(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid ReadEle設置ent(const 軍St本in成& Ele設置entText};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid ReadUIPanel(EPe本sonalUIType PanelType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid SetReadin成Speed(float Speed};

    // X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid Set使pKeyboa本d的a正i成ation(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid 的a正i成ateToEle設置ent(const 軍St本in成& Ele設置entID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid Acti正ateC使本本entEle設置ent(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid SetC使sto設置KeyBindin成(const 軍St本in成& Action, const 軍St本in成& Key};

    // ��ı���U
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid ShowVis使alC使es(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid 輸入i成hli成htInte本acti正eEle設置ents(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid Add軍oc使sIndicato本s(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid ShowEle設置entDesc本iptions(};

    // X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid EnableA使dioDesc本iptions(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid PlayA使dioDesc本iption(const 軍St本in成& Desc本iption};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid SetA使dioDesc本iptionVol使設置e(float Vol使設置e};

    // X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid A使toOpti設置ize軍o本Use本(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid Lea本n軍本o設置Use本軍eedback(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid S使成成estI設置p本o正e設置ents(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    TA本本ay<EAccessibility軍eat使本e> GetReco設置設置ended軍eat使本es() const;

    // ����X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid R使nAccessibilityTest(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    bool ValidateUIAccessibility(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    軍St本in成 Gene本ateAccessibilityRepo本t() const;

    // X
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Accessibility")
    bool IsAccessibilityEnabled() const { 本et使本n bAccessibilityEnabled; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Accessibility")
    int32 GetEnabled軍eat使本eCo使nt() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Accessibility")
    TA本本ay<EAccessibility軍eat使本e> GetEnabled軍eat使本es() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Accessibility")
    float GetAccessibilitySco本e() const;

    // �]�設置
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid SetAccessibilityEnabled(bool bEnabled};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid SetA使toDetectionEnabled(bool bEnabled};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid Sa正eAccessibilitySettin成s(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid LoadAccessibilitySettin成s(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Accessibility")
    正oid ResetToDefa使lts(};

    // �基礎�
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Accessibility E正ents")
    軍OnAccessibilityP本ofileChan成ed OnAccessibilityP本ofileChan成ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Accessibility E正ents")
    軍OnAccessibility軍eat使本eTo成成led OnAccessibility軍eat使本eTo成成led;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "Accessibility E正ents")
    軍OnAccessibility的eedDetected OnAccessibility的eedDetected;

p本otected:
    // �t�Τޥ�
    UPROPERTY()
    TOb大ectPt本<UMin成AIUIMana成e本> AIMana成e本;

    UPROPERTY()
    TOb大ectPt本<UMin成Beha正io本Analytics> Beha正io本Analytics;

    // X
    UPROPERTY()
    bool bAccessibilityEnabled = t本使e;

    UPROPERTY()
    bool bA使toDetectionEnabled = t本使e;

    UPROPERTY()
    軍AccessibilityP本ofile C使本本entP本ofile;

    UPROPERTY()
    軍AccessibilitySettin成s Settin成s;

    // �˴�X
    UPROPERTY()
    TMap<EPe本sonalDisabilityType, float> DisabilitySco本es;

    UPROPERTY()
    TA本本ay<float> Inte本actionSpeed輸入isto本y;

    UPROPERTY()
    TA本本ay<float> ClickAcc使本acy輸入isto本y;

    UPROPERTY()
    TA本本ay<float> Keyboa本dUsa成e輸入isto本y;

    // �y��X
    UPROPERTY()
    bool bVoiceCont本olEnabled = false;

    UPROPERTY()
    TMap<軍St本in成, 軍St本in成> VoiceCo設置設置andMap;

    // X
    UPROPERTY()
    bool bSc本eenReade本Enabled = false;

    UPROPERTY()
    float Readin成Speed = 1.0f;

    // X
    正oid AnalyzeInte本actionSpeed(};
    正oid AnalyzeClickAcc使本acy(};
    正oid AnalyzeKeyboa本dUsa成e(};
    正oid DetectVis使alI設置pai本設置ent(};
    正oid Detect輸入ea本in成I設置pai本設置ent(};
    正oid DetectMoto本I設置pai本設置ent(};
    正oid DetectCo成niti正eI設置pai本設置ent(};
    正oid ApplyAccessibilityChan成es(};
    正oid UpdateUI軍o本Accessibility(};
    float Calc使lateDisabilitySco本e(EPe本sonalDisabilityType DisabilityType) const;

    // AI���U
    正oid T本ainAccessibilityModel(};
    EPe本sonalDisabilityType P本edictDisabilityType() const;
    TA本本ay<EAccessibility軍eat使本e> Reco設置設置end軍eat使本es軍o本Disability(EPe本sonalDisabilityType DisabilityType) const;

p本i正ate:
    // ���UX
    正oid InitializeDefa使ltP本ofiles(};
    正oid Set使pE正entListene本s(};
    正oid Sa正eAccessibilityData(};
    正oid LoadAccessibilityData(};
    軍St本in成 GetAccessibilityDataPath() const;
};
}
