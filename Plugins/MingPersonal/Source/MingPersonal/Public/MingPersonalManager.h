#pragma once

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成RelationshipMana成e本.h"
#incl使de "Min成A使dioRelationshipMana成e本.h"
#incl使de "Min成AIUIMana成e本.h"
#incl使de "Sa正e/Min成Sa正eGa設置eMana成e本.h"
#incl使de "Sa正e/Min成Sa正eGa設置e.h"
#incl使de "Min成Pe本fo本設置anceMana成e本.h"
#incl使de "Min成Me設置o本yOpti設置ize本.h"
#incl使de "的etwo本k/Min成LobbySyste設置.h"
#incl使de "Min成LocalizationMana成e本.h"
#incl使de "Min成Uni正e本sityG使ideMana成e本.h"
#incl使de "Min成Pe本sonalMana成e本.成ene本ated.h"

/**
 * 動人層� * �動角色動基本事系動 */
UCLASS()
class MI的GPERSO的AL下API UMin成Pe本sonalMana成e本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成Pe本sonalMana成e本(};

    正i本t使al 正oid Initialize(};
    正i本t使al 正oid Sh使tdown(};

    正oid Set使pE正entS使bsc本iptions(};
    正oid Clean使pE正entS使bsc本iptions(};

    // 角色動基本
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Cha本acte本")
    正oid AddExpe本ience(int32 A設置o使nt};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Cha本acte本")
    正oid Le正elUp(};

    // 事件
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "的a本本ati正e")
    正oid T本i成成e本Dialo成使e(const 軍St本in成& Dialo成使eId};

    // 目標數量基本系統基本�X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship")
    正oid UpdateCha本acte本Relationship(const 軍St本in成& Cha本acte本ID, float Chan成eA設置o使nt, const 軍St本in成& Reason};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship")
    正oid UpdateRe成ionRep使tation(const 軍St本in成& Re成ionID, float Chan成eA設置o使nt, const 軍St本in成& Reason};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship")
    軍RelationshipData GetCha本acte本Relationship(const 軍St本in成& Cha本acte本ID) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship")
    軍Rep使tationData GetRe成ionRep使tation(const 軍St本in成& Re成ionID) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship")
    TA本本ay<軍St本in成> GetA正ailableDialo成使eOptions(const 軍St本in成& Cha本acte本ID) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship")
    float GetT本adeP本iceModifie本(const 軍St本in成& Cha本acte本ID) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship")
    bool CanAcceptQ使estByRep使tation(const 軍St本in成& Q使estID, const 軍St本in成& Re成ionID) const;

    // 事件
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship")
    正oid OnQ使estCo設置pleted(const 軍St本in成& Q使estID, const 軍St本in成& Re成ionID, const 軍St本in成& Q使estGi正e本ID};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship")
    正oid On的PCInte本action(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Inte本actionType};

    // 動基本�系�
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio")
    正oid InitializeA使dioSyste設置(UOb大ect* MetaSo使ndsSyste設置};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio")
    正oid PlayRelationshipA使dio(const 軍St本in成& Cha本acte本ID, const 軍St本in成& A使dioType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio")
    正oid PlayRep使tationA使dio(const 軍St本in成& Re成ionID, const 軍St本in成& A使dioType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio")
    正oid PlayDialo成使eA使dio(const 軍St本in成& Cha本acte本ID, const 軍St本in成& Dialo成使eType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio")
    正oid PlayRep使blicE本aThe設置e(const 軍St本in成& The設置e的a設置e, float Intensity = 1.0f};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "A使dio")
    正oid SetA使dioVol使設置e(float RelationshipVol使設置e, float Rep使tationVol使設置e, float Dialo成使eVol使設置e};

    // AI UI系統
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid InitializeAIUISyste設置(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid T本ackUse本Beha正io本(const 軍St本in成& Beha正io本Type, const 軍St本in成& Context, float Val使e = 1.0f};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid Opti設置izeUI軍o本Use本(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    正oid ShowContext使al輸入elp(const 軍St本in成& Context};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI UI")
    TA本本ay<軍St本in成> GetAIReco設置設置endations(};

    // �池池系統基本�X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sa正e Syste設置")
    正oid InitializeSa正eSyste設置(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sa正e Syste設置")
    EMin成Sa正eGa設置eRes使lt Sa正eGa設置e(int32 SlotIndex, const 軍St本in成& Sa正e的a設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sa正e Syste設置")
    EMin成Sa正eGa設置eRes使lt Q使ickSa正e(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sa正e Syste設置")
    EMin成Sa正eGa設置eRes使lt LoadGa設置e(int32 SlotIndex};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sa正e Syste設置")
    EMin成Sa正eGa設置eRes使lt Q使ickLoad(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sa正e Syste設置")
    bool DeleteSa正eGa設置e(int32 SlotIndex};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sa正e Syste設置")
    正oid SetA使toSa正eEnabled(bool bEnabled};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Sa正e Syste設置")
    bool IsA使toSa正eEnabled() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Sa正e Syste設置")
    UMin成Sa正eGa設置eMana成e本* GetSa正eGa設置eMana成e本() const;

    // 多人動基本�系�
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本")
    正oid InitializeM使ltiplaye本Syste設置(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本")
    bool C本eateM使ltiplaye本Se本正e本(const 軍Min成的etwo本kConfi成& Confi成};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本")
    bool ConnectToM使ltiplaye本Se本正e本(const 軍St本in成& Se本正e本Add本ess, int32 Po本t};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本")
    正oid Disconnect軍本o設置M使ltiplaye本Se本正e本(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本")
    bool IsM使ltiplaye本Connected() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本")
    bool IsM使ltiplaye本輸入ost() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本")
    int32 GetLocalPlaye本ID() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本")
    正oid SetPlaye本Ready(bool bReady};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本")
    正oid Sta本tM使ltiplaye本Ga設置e(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "M使ltiplaye本")
    正oid SendM使ltiplaye本ChatMessa成e(const 軍St本in成& Messa成e};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使ltiplaye本")
    UMin成的etwo本kMana成e本* Get的etwo本kMana成e本() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "M使ltiplaye本")
    UMin成LobbySyste設置* GetLobbySyste設置() const;

    // 基本系統基本�X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Localization")
    正oid InitializeLocalizationSyste設置(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Localization")
    bool SetGa設置eLan成使a成e(EMin成Lan成使a成e 的ewLan成使a成e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Localization")
    正oid SetGa設置eRe成ion(EMin成Re成ion 的ewRe成ion};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Localization")
    EMin成Lan成使a成e GetC使本本entGa設置eLan成使a成e() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Localization")
    EMin成Re成ion GetC使本本entGa設置eRe成ion() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Localization")
    軍St本in成 GetLocalizedSt本in成(const 軍St本in成& Key, const 軍St本in成& 的a設置espace = TEXT("Defa使lt")) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Localization")
    bool DownloadLan成使a成ePack(EMin成Lan成使a成e Lan成使a成e};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Localization")
    TA本本ay<軍Lan成使a成ePackInfo> GetA正ailableLan成使a成ePacks() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Localization")
    UMin成LocalizationMana成e本* GetLocalizationMana成e本() const;

    // 動基本系統
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid InitializePe本fo本設置anceSyste設置(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid SetPe本fo本設置anceLe正el(EMin成Pe本fo本設置anceLe正el 的ewLe正el};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Pe本fo本設置ance")
    EMin成Pe本fo本設置anceLe正el GetC使本本entPe本fo本設置anceLe正el() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid Opti設置izePe本fo本設置ance(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid SetTa本成et軍本a設置eRate(float Ta本成et軍PS};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Pe本fo本設置ance")
    軍Min成Pe本fo本設置anceMet本ics GetPe本fo本設置anceMet本ics() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid Sta本tPe本fo本設置anceMonito本in成(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本fo本設置ance")
    正oid StopPe本fo本設置anceMonito本in成(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y")
    正oid Opti設置izeMe設置o本y(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Me設置o本y")
    正oid SetMe設置o本yB使d成et(int32 MaxMe設置o本yMB};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Me設置o本y")
    float GetMe設置o本yUsa成ePe本cent() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Pe本fo本設置ance")
    UMin成Pe本fo本設置anceMana成e本* GetPe本fo本設置anceMana成e本() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Pe本fo本設置ance")
    UMin成Me設置o本yOpti設置ize本* GetMe設置o本yOpti設置ize本() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Uni正e本sity G使ide")
    正oid Sta本tUni正e本sityG使ide(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Uni正e本sity G使ide")
    正oid StopUni正e本sityG使ide(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Uni正e本sity G使ide")
    UMin成Uni正e本sityG使ideMana成e本* GetUni正e本sityG使ideMana成e本() const;

p本otected:
    正oid OnExpe本ienceGained(int32 A設置o使nt};
    正oid OnLe正elUp(};

p本i正ate:
    bool bIsInitialized;
    int32 C使本本entLe正el;
    int32 C使本本entExpe本ience;
    int32 Expe本ienceTo的extLe正el;

    // 目標數量基本�
    UPROPERTY()
    TOb大ectPt本<UMin成RelationshipMana成e本> RelationshipMana成e本;

    // 動基本�務
    UPROPERTY()
    TOb大ectPt本<UMin成A使dioRelationshipMana成e本> A使dioRelationshipMana成e本;

    // AI UI�務
    UPROPERTY()
    TOb大ectPt本<UMin成AIUIMana成e本> AIUIMana成e本;

    // �池基本�管務
    UPROPERTY()
    TOb大ectPt本<UMin成Sa正eGa設置eMana成e本> Sa正eGa設置eMana成e本;

    // 動基本�務
    UPROPERTY()
    TOb大ectPt本<UMin成Pe本fo本設置anceMana成e本> Pe本fo本設置anceMana成e本;

    // 故事重要性
    UPROPERTY()
    TOb大ectPt本<UMin成Me設置o本yOpti設置ize本> Me設置o本yOpti設置ize本;

    // 大廳系統
    UPROPERTY()
    TOb大ectPt本<UMin成LobbySyste設置> LobbySyste設置;

    // 基本�動
    UPROPERTY()
    TOb大ectPt本<UMin成LocalizationMana成e本> LocalizationMana成e本;

    // 高校�動�務
    UPROPERTY()
    TOb大ectPt本<UMin成Uni正e本sityG使ideMana成e本> Uni正e本sityG使ideMana成e本;
};

