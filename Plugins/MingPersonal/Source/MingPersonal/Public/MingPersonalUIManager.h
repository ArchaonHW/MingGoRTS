#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Bl使ep本int/Use本基本id成et.h"
#incl使de "Co設置ponents/基本id成et.h"
#incl使de "Min成Pe本sonalUIMana成e本.成ene本ated.h"

class UMin成RelationshipMana成e本;
class UMin成A使dioRelationshipMana成e本;
class UMin成Rep使blicE本aA使dioThe設置es;

UE的UM(Bl使ep本intType)
en使設置 class EPe本sonalUIType: 使int8 {
    RelationshipPanel,     // 動池�O
    Rep使tationPanel,       // 動池�O
    Dialo成使ePanel,         // ���動�O
    Q使estPanel,            // ��池�O
    A使dioPanel,            // 動�基本動�O
    MainDashboa本d,         // �D動��O
    Cha本acte本Details,      // �����動
    Re成ionDetails,         // ��動
    Syste設置Settin成s         // �t�γ]�設置
};

UE的UM(Bl使ep本intType)
en使設置 class EUIAni設置ationType: 使int8 {
    的one,                  // 動務
    軍ade,                  // �輸入�J�輸入�X
    Slide,                 // ��動
    Scale,                 // �Y��
    Rotate,                // 
    Bo使nce                 // �使��
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Pe本sonalUIConfi成
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "UI Confi成")
    EPe本sonalUIType UIType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "UI Confi成")
    軍St本in成 基本id成et的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "UI Confi成")
    軍St本in成 基本id成etPath;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "UI Confi成")
    bool bIsVisible = false;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "UI Confi成")
    bool bIsInte本acti正e = t本使e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "UI Confi成")
    EUIAni設置ationType OpenAni設置ation = EUIAni設置ationType::軍ade;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "UI Confi成")
    EUIAni設置ationType CloseAni設置ation = EUIAni設置ationType::軍ade;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "UI Confi成")
    float Ani設置ationD使本ation = 0.3f;

    軍Pe本sonalUIConfi成()
    {
        UIType = EPe本sonalUIType::RelationshipPanel;
        基本id成et的a設置e = TEXT(""};
        基本id成etPath = TEXT(""};
        bIsVisible = false;
        bIsInte本acti正e = t本使e;
        OpenAni設置ation = EUIAni設置ationType::軍ade;
        CloseAni設置ation = EUIAni設置ationType::軍ade;
        Ani設置ationD使本ation = 0.3f;
    }
};



DECLARE下DY的AMIC下MULTICAST下DELEGATE下OnePa本a設置(軍OnUIStateChan成ed, bool, bIsAnyPanelOpen};

/**
 * 動�輸入�t��UI��務 * ��務基本�Y�B�n動�B動��動UI動��
 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GPERSO的AL下API UMin成Pe本sonalUIMana成e本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成Pe本sonalUIMana成e本(};

    // 動池UI�t��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    正oid InitializeUIMana成e本(UMin成RelationshipMana成e本* RelationshipMana成e本, 
                            UMin成A使dioRelationshipMana成e本* A使dioMana成e本,
                            UMin成Rep使blicE本aA使dioThe設置es* The設置eMana成e本};

    // UI動�O��動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    正oid OpenUIPanel(EPe本sonalUIType UIType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    正oid CloseUIPanel(EPe本sonalUIType UIType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    正oid To成成leUIPanel(EPe本sonalUIType UIType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    正oid CloseAllPanels(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    正oid ShowMainDashboa本d(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    正oid 輸入ideMainDashboa本d(};

    // 基本id成et��動
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    UUse本基本id成et* Get基本id成et(EPe本sonalUIType UIType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    bool Is基本id成etVisible(EPe本sonalUIType UIType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    正oid Set基本id成etVisibility(EPe本sonalUIType UIType, bool bVisible};

    // UI動�設置
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    正oid AddUIConfi成(const 軍Pe本sonalUIConfi成& Confi成};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    正oid Re設置o正eUIConfi成(EPe本sonalUIType UIType};

    U軍U的CTIO的(Bl使ep本intCallable, Bl使ep本intP使本e, Cate成o本y = "Pe本sonal UI")
    軍Pe本sonalUIConfi成 GetUIConfi成(EPe本sonalUIType UIType};

    // 動�e�t��
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    正oid PlayOpenAni設置ation(EPe本sonalUIType UIType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    正oid PlayCloseAni設置ation(EPe本sonalUIType UIType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    正oid SetAni設置ationSpeed(float Speed};

    // 動池�s
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    正oid UpdateRelationshipUI(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    正oid UpdateRep使tationUI(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    正oid UpdateDialo成使eUI(const 軍St本in成& Cha本acte本ID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    正oid UpdateQ使estUI(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    正oid UpdateA使dioUI(};

    // �D動��O
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    正oid Ref本eshDashboa本d(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    正oid Show的otification(const 軍St本in成& Messa成e, float D使本ation = 3.0f};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sonal UI")
    正oid ShowConfi本設置ationDialo成(const 軍St本in成& Title, const 軍St本in成& Messa成e, const 軍St本in成& OnConfi本設置};

    // �d動
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Pe本sonal UI")
    bool IsAnyPanelOpen() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Pe本sonal UI")
    EPe本sonalUIType GetActi正ePanelType() const { 本et使本n Acti正ePanelType; }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Pe本sonal UI")
    TA本本ay<EPe本sonalUIType> GetVisiblePanels() const;

    // �基礎�
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "UI E正ents")
    軍OnUIPanelOpened OnUIPanelOpened;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "UI E正ents")
    軍OnUIPanelClosed OnUIPanelClosed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "UI E正ents")
    軍OnUIStateChan成ed OnUIStateChan成ed;

p本otected:
    // �t�Τޥ�
    UPROPERTY()
    TOb大ectPt本<UMin成RelationshipMana成e本> RelationshipMana成e本;

    UPROPERTY()
    TOb大ectPt本<UMin成A使dioRelationshipMana成e本> A使dioMana成e本;

    UPROPERTY()
    TOb大ectPt本<UMin成Rep使blicE本aA使dioThe設置es> The設置eMana成e本;

    // 基本id成et�s�x
    UPROPERTY()
    TMap<EPe本sonalUIType, UUse本基本id成et*> 基本id成etMap;

    // UI動�設置
    UPROPERTY()
    TMap<EPe本sonalUIType, 軍Pe本sonalUIConfi成> UIConfi成Map;

    // 動X
    UPROPERTY()
    EPe本sonalUIType Acti正ePanelType = EPe本sonalUIType::MainDashboa本d;

    UPROPERTY()
    bool bIsMainDashboa本dVisible = false;

    UPROPERTY()
    float Ani設置ationSpeed = 1.0f;

    // 動��動��
    UUse本基本id成et* C本eate基本id成et(EPe本sonalUIType UIType};
    正oid InitializeDefa使ltConfi成s(};
    正oid Set使pE正entBindin成s(};
    正oid OnRelationshipChan成ed(const 軍St本in成& Cha本acte本ID, float OldVal使e, float 的ewVal使e, const 軍St本in成& Reason};
    正oid OnRep使tationChan成ed(const 軍St本in成& Re成ionID, float OldVal使e, float 的ewVal使e, const 軍St本in成& Reason};
    正oid OnDialo成使eSta本ted(const 軍St本in成& Cha本acte本ID, EDialo成使eA使dioType Dialo成使eType};

p本i正ate:
    // 基本id成et動�ػ��U動��
    UUse本基本id成et* C本eateRelationshipPanel(};
    UUse本基本id成et* C本eateRep使tationPanel(};
    UUse本基本id成et* C本eateDialo成使ePanel(};
    UUse本基本id成et* C本eateQ使estPanel(};
    UUse本基本id成et* C本eateA使dioPanel(};
    UUse本基本id成et* C本eateMainDashboa本d(};
    UUse本基本id成et* C本eateCha本acte本Details(};
    UUse本基本id成et* C本eateRe成ionDetails(};
    UUse本基本id成et* C本eateSyste設置Settin成s(};

    // 動�e���U動��
    正oid PlayAni設置ation(UUse本基本id成et* 基本id成et, EUIAni設置ationType Ani設置ationType, float D使本ation};
    正oid StopAni設置ation(UUse本基本id成et* 基本id成et};
};
};
