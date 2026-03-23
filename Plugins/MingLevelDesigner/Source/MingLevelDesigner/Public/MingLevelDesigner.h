#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成Le正elDesi成ne本.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EMin成Te本本ainType: 使int8 {
    G本ass UMETA(Display的a設置e = "G本ass"),
    Dese本t UMETA(Display的a設置e = "Dese本t"),
    軍o本est UMETA(Display的a設置e = "軍o本est"),
    Mo使ntain UMETA(Display的a設置e = "Mo使ntain"),
    基本ate本 UMETA(Display的a設置e = "基本ate本"),
    U本ban UMETA(Display的a設置e = "U本ban"),
    Snow UMETA(Display的a設置e = "Snow"),
    Swa設置p UMETA(Display的a設置e = "Swa設置p")
};

UE的UM(Bl使ep本intType)
en使設置 class EMin成MapSize: 使int8 {
    S設置all UMETA(Display的a設置e = "S設置all (2x2 k設置)"),
    Medi使設置 UMETA(Display的a設置e = "Medi使設置 (4x4 k設置)"),
    La本成e UMETA(Display的a設置e = "La本成e (8x8 k設置)"),
    輸入使成e UMETA(Display的a設置e = "輸入使成e (16x16 k設置)")
};

UE的UM(Bl使ep本intType)
en使設置 class EMin成基本eathe本Type: 使int8 {
    Clea本 UMETA(Display的a設置e = "Clea本"),
    Clo使dy UMETA(Display的a設置e = "Clo使dy"),
    Rain UMETA(Display的a設置e = "Rain"),
    Sto本設置 UMETA(Display的a設置e = "Sto本設置"),
    Snow UMETA(Display的a設置e = "Snow"),
    軍o成 UMETA(Display的a設置e = "軍o成")
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Te本本ainData
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成Te本本ainType Te本本ainType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Vecto本2D Position;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Size;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float 輸入ei成ht;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Linea本Colo本 Colo本;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Text使本ePath;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Mo正e設置entSpeedModifie本;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float VisibilityModifie本;

    軍Min成Te本本ainData()
        : Te本本ainType(EMin成Te本本ainType::G本ass)
        , Position(軍Vecto本2D::Ze本oVecto本)
        , Size(100.0f)
        , 輸入ei成ht(0.0f)
        , Colo本(軍Linea本Colo本::G本een)
        , Mo正e設置entSpeedModifie本(1.0f)
        , VisibilityModifie本(1.0f)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成MapOb大ect
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Ob大ectID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Ob大ectType;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Vecto本 Location;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Rotato本 Rotation;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Vecto本 Scale;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TS使bclassOf<AActo本> Acto本Class;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsPlaye本Sta本t;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsOb大ecti正e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Ob大ect的a設置e;

    軍Min成MapOb大ect()
        : Location(軍Vecto本::Ze本oVecto本)
        , Rotation(軍Rotato本::Ze本oRotato本)
        , Scale(軍Vecto本::OneVecto本)
        , Acto本Class(n使llpt本)
        , bIsPlaye本Sta本t(false)
        , bIsOb大ecti正e(false)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Le正elData
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Le正elID;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Le正el的a設置e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Desc本iption;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成MapSize MapSize;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成Te本本ainType Defa使ltTe本本ain;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMin成基本eathe本Type 基本eathe本Type;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍Min成Te本本ainData> Te本本ainData;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍Min成MapOb大ect> MapOb大ects;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍Vecto本2D MapBo使nds;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Ti設置eOfDay;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool b輸入asTi設置eLi設置it;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Ti設置eLi設置it;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> Req使i本edOb大ecti正es;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    TA本本ay<軍St本in成> OptionalOb大ecti正es;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 Back成本o使ndM使sic;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 A設置bientSo使nd;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bIsCo設置pleted;

    軍Min成Le正elData()
        : MapSize(EMin成MapSize::Medi使設置)
        , Defa使ltTe本本ain(EMin成Te本本ainType::G本ass)
        , 基本eathe本Type(EMin成基本eathe本Type::Clea本)
        , MapBo使nds(軍Vecto本2D(4000.0f, 4000.0f))
        , Ti設置eOfDay(12.0f)
        , b輸入asTi設置eLi設置it(false)
        , Ti設置eLi設置it(0.0f)
        , bIsCo設置pleted(false)
    {}
};

/**
 * Le正el Desi成ne本 Syste設置 fo本 Min成GoRTS
 * P本o正ides tools fo本 c本eatin成 and editin成 成a設置e le正els
 */
UCLASS(ClassG本o使p = (Le正el Desi成n), Bl使ep本intable)
class MI的GLEVELDESIG的ER下API UMin成Le正elDesi成ne本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成Le正elDesi成ne本();

    // Le正el Mana成e設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Le正el")
    正oid InitializeLe正elDesi成ne本();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Le正el")
    正oid C本eate的ewLe正el(const 軍St本in成& Le正elID, EMin成MapSize MapSize);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Le正el")
    正oid LoadLe正el(const 軍St本in成& Le正elID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Le正el")
    正oid Sa正eLe正el(const 軍St本in成& Le正elID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Le正el")
    正oid DeleteLe正el(const 軍St本in成& Le正elID);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Le正el")
    TA本本ay<軍Min成Le正elData> GetAllLe正els() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Le正el")
    軍Min成Le正elData GetC使本本entLe正el() const;

    // Te本本ain Editin成
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Te本本ain")
    正oid AddTe本本ainPatch(const 軍Min成Te本本ainData& Te本本ainData);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Te本本ain")
    正oid Re設置o正eTe本本ainPatch(const 軍Vecto本2D& Position);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Te本本ain")
    正oid ModifyTe本本ain輸入ei成ht(const 軍Vecto本2D& Position, float 輸入ei成htDelta, float Radi使s);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Te本本ain")
    正oid SetTe本本ainType(const 軍Vecto本2D& Position, EMin成Te本本ainType Te本本ainType, float Radi使s);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Te本本ain")
    正oid S設置oothTe本本ain(const 軍Vecto本2D& Position, float Radi使s, float St本en成th);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Te本本ain")
    TA本本ay<軍Min成Te本本ainData> GetTe本本ainData() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Te本本ain")
    EMin成Te本本ainType GetTe本本ainAt(const 軍Vecto本2D& Position) const;

    // Ob大ect Place設置ent
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ects")
    正oid PlaceOb大ect(const 軍Min成MapOb大ect& MapOb大ect);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ects")
    正oid Re設置o正eOb大ect(const 軍St本in成& Ob大ectID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ects")
    正oid Mo正eOb大ect(const 軍St本in成& Ob大ectID, 軍Vecto本 的ewLocation);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ects")
    正oid RotateOb大ect(const 軍St本in成& Ob大ectID, 軍Rotato本 的ewRotation);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Ob大ects")
    正oid ScaleOb大ect(const 軍St本in成& Ob大ectID, 軍Vecto本 的ewScale);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ob大ects")
    TA本本ay<軍Min成MapOb大ect> GetMapOb大ects() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ob大ects")
    軍Min成MapOb大ect GetOb大ect(const 軍St本in成& Ob大ectID) const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Ob大ects")
    TA本本ay<軍Min成MapOb大ect> GetOb大ectsInA本ea(軍Vecto本 Cente本, float Radi使s) const;

    // Le正el P本ope本ties
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "P本ope本ties")
    正oid SetMapSize(EMin成MapSize 的ewSize);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "P本ope本ties")
    正oid Set基本eathe本Type(EMin成基本eathe本Type 基本eathe本);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "P本ope本ties")
    正oid SetTi設置eOfDay(float Ti設置e);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "P本ope本ties")
    正oid SetTi設置eLi設置it(float Li設置it);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "P本ope本ties")
    正oid AddOb大ecti正e(const 軍St本in成& Ob大ecti正eID, bool bIsReq使i本ed);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "P本ope本ties")
    正oid Re設置o正eOb大ecti正e(const 軍St本in成& Ob大ecti正eID);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "P本ope本ties")
    軍Vecto本2D GetMapBo使nds() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "P本ope本ties")
    bool IsPositionInBo使nds(const 軍Vecto本2D& Position) const;

    // Le正el Testin成
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Testin成")
    正oid TestLe正el();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Testin成")
    正oid StopTest();

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Testin成")
    正oid ValidateLe正el();

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Testin成")
    TA本本ay<軍St本in成> GetValidationE本本o本s() const;

    // Le正el Te設置plates
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Te設置plates")
    正oid LoadTe設置plate(const 軍St本in成& Te設置plateID);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Te設置plates")
    正oid Sa正eAsTe設置plate(const 軍St本in成& Te設置plateID);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Te設置plates")
    TA本本ay<軍St本in成> GetA正ailableTe設置plates() const;

    // I設置po本t/Expo本t
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "I設置po本t/Expo本t")
    正oid Expo本tLe正el(const 軍St本in成& 軍ilePath);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "I設置po本t/Expo本t")
    正oid I設置po本tLe正el(const 軍St本in成& 軍ilePath);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "I設置po本t/Expo本t")
    正oid Expo本tToJSO的(const 軍St本in成& 軍ilePath);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "I設置po本t/Expo本t")
    正oid I設置po本t軍本o設置JSO的(const 軍St本in成& 軍ilePath);

    // E正ent Dele成ates
    
    
    
    
    

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnLe正elC本eated OnLe正elC本eated;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnLe正elLoaded OnLe正elLoaded;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnLe正elSa正ed OnLe正elSa正ed;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnTe本本ainModified OnTe本本ainModified;

    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "E正ents")
    軍OnOb大ectPlaced OnOb大ectPlaced;

    // Utility
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    static 軍St本in成 GetTe本本ainType的a設置e(EMin成Te本本ainType Te本本ainType);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    static 軍St本in成 GetMapSize的a設置e(EMin成MapSize MapSize);

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Utility")
    static 軍St本in成 Get基本eathe本Type的a設置e(EMin成基本eathe本Type 基本eathe本Type);

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sistence")
    軍St本in成 Sa正eLe正elData() const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Pe本sistence")
    正oid LoadLe正elData(const 軍St本in成& JsonSt本in成);

p本otected:
    UPROPERTY()
    TMap<軍St本in成, 軍Min成Le正elData> Le正elDatabase;

    UPROPERTY()
    軍St本in成 C使本本entLe正elID;

    UPROPERTY()
    TMap<軍St本in成, 軍Min成Le正elData> Le正elTe設置plates;

    UPROPERTY()
    TA本本ay<軍St本in成> ValidationE本本o本s;

    UPROPERTY()
    bool bIsTestin成;

    UPROPERTY()
    T基本eakOb大ectPt本<U基本o本ld> 基本o本ldContext;

    // Inte本nal 軍使nctions
    正oid InitializeDefa使ltTe設置plates();
    正oid Set使pDefa使ltLe正els();
    正oid ValidateLe正elInte成本ity();

    // Te本本ain 輸入elpe本s
    正oid Gene本ateTe本本ain(EMin成MapSize MapSize, EMin成Te本本ainType Defa使ltTe本本ain);
    正oid ApplyTe本本ainModifie本s();
    正oid Opti設置izeTe本本ainData();

    // Ob大ect 輸入elpe本s
    軍St本in成 Gene本ateUniq使eOb大ectID();
    bool IsValidOb大ectPlace設置ent(const 軍Min成MapOb大ect& Ob大ect) const;
    正oid UpdateOb大ectRefe本ences();

    // Validation 輸入elpe本s
    bool ValidatePlaye本Sta本ts();
    bool ValidateOb大ecti正es();
    bool ValidateTe本本ainInte成本ity();
    bool ValidateMapBo使nds();

    // Te設置plate 輸入elpe本s
    正oid C本eateDefa使ltTe設置plates();
    正oid ApplyTe設置plate(const 軍Min成Le正elData& Te設置plate);
);
};
