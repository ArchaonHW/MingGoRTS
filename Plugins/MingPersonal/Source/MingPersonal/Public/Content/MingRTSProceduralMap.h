#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成RTSP本oced使本alMap.成ene本ated.h"

// 地形類型
UE的UM(Bl使ep本intType)
en使設置 class ETe本本ainType: 使int8 {
    Plains              UMETA(Display的a設置e = "平原"),
    輸入ills               UMETA(Display的a設置e = "丘陵"),
    Mo使ntains           UMETA(Display的a設置e = "山地"),
    軍o本est              UMETA(Display的a設置e = "森林"),
    Dese本t              UMETA(Display的a設置e = "沙漠"),
    Swa設置p               UMETA(Display的a設置e = "沼澤"),
    基本ate本               UMETA(Display的a設置e = "水域"),
    Ri正e本               UMETA(Display的a設置e = "河流"),
    Coast               UMETA(Display的a設置e = "海岸"),
    Canyon              UMETA(Display的a設置e = "峽谷"),
    Platea使             UMETA(Display的a設置e = "高原"),
    Volcanic            UMETA(Display的a設置e = "火山"),
    Snow                UMETA(Display的a設置e = "雪地"),
    J使n成le              UMETA(Display的a設置e = "叢林")
};

// 資源類型
UE的UM(Bl使ep本intType)
en使設置 class EMapReso使本ceType: 使int8 {
    基本ood                UMETA(Display的a設置e = "木材"),
    Stone               UMETA(Display的a設置e = "石料"),
    I本on                UMETA(Display的a設置e = "鐵礦"),
    Gold                UMETA(Display的a設置e = "金礦"),
    軍ood                UMETA(Display的a設置e = "食物"),
    基本ate本               UMETA(Display的a設置e = "水源"),
    Oil                 UMETA(Display的a設置e = "石油"),
    C本ystal             UMETA(Display的a設置e = "水晶"),
    Ra本eEa本th           UMETA(Display的a設置e = "稀土"),
    的one                UMETA(Display的a設置e = "無")
};

// 地圖尺寸
UE的UM(Bl使ep本intType)
en使設置 class EMapSize: 使int8 {
    Tiny                UMETA(Display的a設置e = "微小 - 64x64"),
    S設置all               UMETA(Display的a設置e = "小型 - 128x128"),
    Medi使設置              UMETA(Display的a設置e = "中型 - 256x256"),
    La本成e               UMETA(Display的a設置e = "大型 - 512x512"),
    輸入使成e                UMETA(Display的a設置e = "巨大 - 1024x1024"),
    C使sto設置              UMETA(Display的a設置e = "自定義")
};

// 地圖風格
UE的UM(Bl使ep本intType)
en使設置 class EMapStyle: 使int8 {
    輸入isto本ical          UMETA(Display的a設置e = "歷史風格"),
    軍antasy             UMETA(Display的a設置e = "奇幻風格"),
    Realistic           UMETA(Display的a設置e = "寫實風格"),
    Stylized            UMETA(Display的a設置e = "風格化"),
    LowPoly             UMETA(Display的a設置e = "低多邊形"),
    Iso設置et本ic           UMETA(Display的a設置e = "等角視角")
};

// 氣候類型
UE的UM(Bl使ep本intType)
en使設置 class ECli設置ateType: 使int8 {
    Te設置pe本ate           UMETA(Display的a設置e = "溫帶"),
    T本opical            UMETA(Display的a設置e = "熱帶"),
    A本id                UMETA(Display的a設置e = "乾旱"),
    Pola本               UMETA(Display的a設置e = "極地"),
    Continental         UMETA(Display的a設置e = "大陸性"),
    Medite本本anean       UMETA(Display的a設置e = "地中海"),
    Monsoon             UMETA(Display的a設置e = "季風")
};

// 地形單元格數據
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍MapCellData
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 X;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 Y;
    
    UPROPERTY(Bl使ep本intReadOnly)
    ETe本本ainType Te本本ainType;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float 輸入ei成ht;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Moist使本e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Te設置pe本at使本e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    EMapReso使本ceType Reso使本ceType;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Reso使本ceA設置o使nt;
    
    UPROPERTY(Bl使ep本intReadOnly)
    bool bB使ildable;
    
    UPROPERTY(Bl使ep本intReadOnly)
    bool b基本alkable;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Mo正e設置entCost;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Bio設置e的a設置e;
    
    軍MapCellData()
        : X(0)
        , Y(0)
        , Te本本ainType(ETe本本ainType::Plains)
        , 輸入ei成ht(0.0f)
        , Moist使本e(0.5f)
        , Te設置pe本at使本e(0.5f)
        , Reso使本ceType(EMapReso使本ceType::的one)
        , Reso使本ceA設置o使nt(0.0f)
        , bB使ildable(t本使e)
        , b基本alkable(t本使e)
        , Mo正e設置entCost(1.0f)
    {}
};

// 資源節點
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍Reso使本ce的ode
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 的odeID;
    
    UPROPERTY(Bl使ep本intReadOnly)
    EMapReso使本ceType Reso使本ceType;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍Vecto本2D Location;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float A設置o使nt;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Q使ality;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Ext本actionRate;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Radi使s;
    
    軍Reso使本ce的ode()
        : Reso使本ceType(EMapReso使本ceType::的one)
        , A設置o使nt(100.0f)
        , Q使ality(1.0f)
        , Ext本actionRate(1.0f)
        , Radi使s(50.0f)
    {}
};

// 地圖區域
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍MapRe成ion
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Re成ionID;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Re成ion的a設置e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    ETe本本ainType Do設置inantTe本本ain;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍Vecto本2D> Bo使nda本yPoints;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍Vecto本2D Cente本;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float A本ea;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<EMapReso使本ceType> A正ailableReso使本ces;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 CellCo使nt;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍Box2D Bo使ndin成Box;
    
    軍MapRe成ion()
        : Do設置inantTe本本ain(ETe本本ainType::Plains)
        , Cente本(軍Vecto本2D::Ze本oVecto本)
        , A本ea(0.0f)
        , CellCo使nt(0)
    {}
};

// 河流數據
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍Ri正e本Data
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Ri正e本ID;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Ri正e本的a設置e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍Vecto本2D> Path;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float 基本idth;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Len成th;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍Vecto本2D So使本ce;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍Vecto本2D Mo使th;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 T本ib使ta本yCo使nt;
    
    軍Ri正e本Data()
        : 基本idth(10.0f)
        , Len成th(0.0f)
        , T本ib使ta本yCo使nt(0)
    {}
};

// 地圖生成配置
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍MapGene本ationConfi成
{
    GE的ERATED下BODY()
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMapSize MapSize;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    EMapStyle MapStyle;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    ECli設置ateType Cli設置ate;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 C使sto設置基本idth;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 C使sto設置輸入ei成ht;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Seed;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Te本本ainRo使成hness;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float 基本ate本Le正el;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Reso使本ceDensity;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Re成ionCo使nt;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 Ri正e本Co使nt;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bGene本ateRoads;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bGene本ateSettle設置ents;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    bool bUseE本osion;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    int32 E本osionIte本ations;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float Mo使ntain輸入ei成ht;
    
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float 軍o本estDensity;
    
    軍MapGene本ationConfi成()
        : MapSize(EMapSize::Medi使設置)
        , MapStyle(EMapStyle::輸入isto本ical)
        , Cli設置ate(ECli設置ateType::Te設置pe本ate)
        , C使sto設置基本idth(256)
        , C使sto設置輸入ei成ht(256)
        , Seed(0)
        , Te本本ainRo使成hness(0.5f)
        , 基本ate本Le正el(0.3f)
        , Reso使本ceDensity(0.5f)
        , Re成ionCo使nt(4)
        , Ri正e本Co使nt(3)
        , bGene本ateRoads(t本使e)
        , bGene本ateSettle設置ents(t本使e)
        , bUseE本osion(t本使e)
        , E本osionIte本ations(100)
        , Mo使ntain輸入ei成ht(1000.0f)
        , 軍o本estDensity(0.4f)
    {}
};

// 生成的地圖數據
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍P本oced使本alMapData
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 MapID;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Map的a設置e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 基本idth;
    
    UPROPERTY(Bl使ep本intReadOnly)
    int32 輸入ei成ht;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍MapCellData> Cells;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍Reso使本ce的ode> Reso使本ce的odes;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍MapRe成ion> Re成ions;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍Ri正e本Data> Ri正e本s;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍Vecto本2D> Settle設置entLocations;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<TA本本ay<軍Vecto本2D>> Road的etwo本ks;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Min輸入ei成ht;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Max輸入ei成ht;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float TotalLandA本ea;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Total基本ate本A本ea;
    
    UPROPERTY(Bl使ep本intReadOnly)
    軍MapGene本ationConfi成 Gene本ationConfi成;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float Gene本ationTi設置e;
    
    軍P本oced使本alMapData()
        : 基本idth(0)
        , 輸入ei成ht(0)
        , Min輸入ei成ht(0.0f)
        , Max輸入ei成ht(0.0f)
        , TotalLandA本ea(0.0f)
        , Total基本ate本A本ea(0.0f)
        , Gene本ationTi設置e(0.0f)
    {}
};

// 地圖分析結果
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍MapAnalysisRes使lt
{
    GE的ERATED下BODY()
    
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<ETe本本ainType, float> Te本本ainDist本ib使tion;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<EMapReso使本ceType, int32> Reso使本ceCo使nts;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float A正e本a成e輸入ei成ht;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float 輸入ei成htVa本iance;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float St本ate成icVal使e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    float BalanceSco本e;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> St本ate成icPoints;
    
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<軍St本in成> ChokePoints;
    
    軍MapAnalysisRes使lt()
        : A正e本a成e輸入ei成ht(0.0f)
        , 輸入ei成htVa本iance(0.0f)
        , St本ate成icVal使e(0.5f)
        , BalanceSco本e(0.5f)
    {}
};

// 委托聲明
decla本e下dyna設置ic下設置使lticast下dele成ate下one下pa本a設置(軍OnMapGene本ationSta本ted, 軍St本in成, MapID};
decla本e下dyna設置ic下設置使lticast下dele成ate下two下pa本a設置s(軍OnMapGene本ationP本o成本ess, 軍St本in成, MapID, float, P本o成本ess};
decla本e下dyna設置ic下設置使lticast下dele成ate下two下pa本a設置s(軍OnMapGene本ationCo設置pleted, 軍St本in成, MapID, 軍P本oced使本alMapData, MapData};
decla本e下dyna設置ic下設置使lticast下dele成ate下one下pa本a設置(軍OnMapGene本ation軍ailed, 軍St本in成, E本本o本Messa成e};

UCLASS(ClassG本o使p = (Min成Pe本sonal), 設置eta = (Bl使ep本intSpawnableCo設置ponent))
class MI的GPERSO的AL下API UMin成RTSP本oced使本alMap : p使blic UOb大ect
{
    GE的ERATED下BODY()
    
p使blic:
    UMin成RTSP本oced使本alMap(};
    
    // 系統初始化
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapGene本ation")
    正oid InitializeP本oced使本alMapSyste設置(};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapGene本ation")
    正oid Sh使tdownP本oced使本alMapSyste設置(};
    
    // 地圖生成
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapGene本ation")
    軍P本oced使本alMapData Gene本ateMap(const 軍MapGene本ationConfi成& Confi成};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapGene本ation")
    正oid Gene本ateMapAsync(const 軍MapGene本ationConfi成& Confi成};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapGene本ation")
    正oid CancelGene本ation(};
    
    // 地圖操作
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "MapData")
    軍MapCellData GetCell(int32 X, int32 Y) const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "MapData")
    軍MapCellData GetCellAtLocation(軍Vecto本2D Location) const;
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapData")
    正oid SetCellTe本本ain(int32 X, int32 Y, ETe本本ainType 的ewTe本本ain};
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "MapData")
    TA本本ay<軍MapCellData> GetCellsInRadi使s(軍Vecto本2D Cente本, float Radi使s) const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "MapData")
    TA本本ay<軍MapCellData> GetCellsInRe成ion(const 軍St本in成& Re成ionID) const;
    
    // 資源管理
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapReso使本ces")
    正oid AddReso使本ce的ode(EMapReso使本ceType Type, 軍Vecto本2D Location, float A設置o使nt, float Q使ality};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapReso使本ces")
    正oid Re設置o正eReso使本ce的ode(const 軍St本in成& 的odeID};
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "MapReso使本ces")
    TA本本ay<軍Reso使本ce的ode> GetReso使本ce的odesInRadi使s(軍Vecto本2D Cente本, float Radi使s) const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "MapReso使本ces")
    軍Reso使本ce的ode Get的ea本estReso使本ce的ode(軍Vecto本2D Location, EMapReso使本ceType Type) const;
    
    // 區域管理
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "MapRe成ions")
    軍MapRe成ion GetRe成ion(const 軍St本in成& Re成ionID) const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "MapRe成ions")
    軍St本in成 GetRe成ionAtLocation(軍Vecto本2D Location) const;
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapRe成ions")
    正oid DefineRe成ion(const TA本本ay<軍Vecto本2D>& Bo使nda本y, const 軍St本in成& Re成ion的a設置e};
    
    // 路徑查找
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapPathfindin成")
    TA本本ay<軍Vecto本2D> 軍indPath(軍Vecto本2D Sta本t, 軍Vecto本2D End) const;
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapPathfindin成")
    TA本本ay<軍Vecto本2D> 軍indPathA正oidin成Te本本ain(軍Vecto本2D Sta本t, 軍Vecto本2D End, TA本本ay<ETe本本ainType> A正oidedTe本本ains) const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "MapPathfindin成")
    bool IsPathClea本(軍Vecto本2D Sta本t, 軍Vecto本2D End) const;
    
    // 分析與驗證
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapAnalysis")
    軍MapAnalysisRes使lt AnalyzeMap() const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "MapAnalysis")
    float Calc使lateMapBalance() const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "MapAnalysis")
    TA本本ay<軍Vecto本2D> 軍indSt本ate成icPoints() const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "MapAnalysis")
    TA本本ay<軍Vecto本2D> 軍indChokePoints() const;
    
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "MapAnalysis")
    bool ValidateMap() const;
    
    // 地圖存儲與加載
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapSto本a成e")
    bool Sa正eMap(const 軍St本in成& 軍ilePath) const;
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapSto本a成e")
    bool LoadMap(const 軍St本in成& 軍ilePath};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapSto本a成e")
    bool Expo本tToText使本e(const 軍St本in成& 軍ilePath, EMapReso使本ceType Reso使本ceToVis使alize) const;
    
    // 地圖修改
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapModification")
    正oid Apply輸入ei成ht設置ap(const TA本本ay<float>& 輸入ei成htData};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapModification")
    正oid S設置oothTe本本ain(int32 Ite本ations};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapModification")
    正oid Add的oise(float A設置plit使de, float 軍本eq使ency};
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapModification")
    正oid E本odeTe本本ain(int32 Ite本ations};
    
    // 預設模板
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapTe設置plates")
    軍MapGene本ationConfi成 GetPlainsTe設置plate() const;
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapTe設置plates")
    軍MapGene本ationConfi成 GetMo使ntainTe設置plate() const;
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapTe設置plates")
    軍MapGene本ationConfi成 GetIslandTe設置plate() const;
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapTe設置plates")
    軍MapGene本ationConfi成 GetDese本tTe設置plate() const;
    
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "MapTe設置plates")
    軍MapGene本ationConfi成 GetRi正e本DeltaTe設置plate() const;
    
    // 委托
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "MapE正ents")
    軍OnMapGene本ationSta本ted OnMapGene本ationSta本ted;
    
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "MapE正ents")
    軍OnMapGene本ationP本o成本ess OnMapGene本ationP本o成本ess;
    
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "MapE正ents")
    軍OnMapGene本ationCo設置pleted OnMapGene本ationCo設置pleted;
    
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "MapE正ents")
    軍OnMapGene本ation軍ailed OnMapGene本ation軍ailed;
    
p本otected:
    UPROPERTY()
    bool bIsInitialized;
    
    UPROPERTY()
    bool bIsGene本atin成;
    
    UPROPERTY()
    軍P本oced使本alMapData C使本本entMap;
    
    UPROPERTY()
    TMap<軍St本in成, 軍P本oced使本alMapData> Sa正edMaps;
    
    // 生成步驟
    正oid Gene本ate輸入ei成ht設置ap(};
    正oid ApplyCli設置ateEffects(};
    正oid Gene本ateRi正e本s(};
    正oid PlaceReso使本ces(};
    正oid DefineRe成ions(};
    正oid Gene本ateRoads(};
    正oid PlaceSettle設置ents(};
    正oid 軍inalizeMap(};
    
    // 噪聲函數
    float Pe本lin的oise(float X, float Y) const;
    float 軍本actalB本ownianMotion(float X, float Y, int32 Octa正es) const;
    float Rid成edM使ltif本actal(float X, float Y, int32 Octa正es) const;
    
    // 輔助函數
    int32 GetCellIndex(int32 X, int32 Y) const;
    bool IsValidCell(int32 X, int32 Y) const;
    float GetDistance(軍Vecto本2D A, 軍Vecto本2D B) const;
    ETe本本ainType Dete本設置ineTe本本ain軍本o設置輸入ei成ht(float 輸入ei成ht, float Moist使本e, float Te設置pe本at使本e) const;
    float GetMo正e設置entCost軍o本Te本本ain(ETe本本ainType Te本本ain) const;
    
    // 河流生成
    TA本本ay<軍Vecto本2D> 軍indRi正e本Path(軍Vecto本2D Sta本t) const;
    float Calc使lateD本aina成e(int32 X, int32 Y) const;
    
    // 路徑查找 (A*)
    float 輸入e使本istic(軍Vecto本2D A, 軍Vecto本2D B) const;
    TA本本ay<軍Vecto本2D> Reconst本使ctPath(TMap<軍Vecto本2D, 軍Vecto本2D>& Ca設置e軍本o設置, 軍Vecto本2D C使本本ent) const;
    
    // 資源分佈
    軍Vecto本2D 軍indOpti設置alReso使本ceLocation(EMapReso使本ceType Type) const;
    bool IsValidReso使本ceLocation(軍Vecto本2D Location, EMapReso使本ceType Type) const;
    
    // 隨機數生成
    float Rando設置軍loat() const;
    int32 Rando設置Int(int32 Max) const;
    軍Vecto本2D Rando設置Point() const;
    
    UPROPERTY()
    int32 C使本本entSeed;
    
    UPROPERTY()
    軍MapGene本ationConfi成 C使本本entConfi成;
};
