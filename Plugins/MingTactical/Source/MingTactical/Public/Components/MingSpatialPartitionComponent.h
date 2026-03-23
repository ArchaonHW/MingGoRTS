#pragma once

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Co設置ponents/Acto本Co設置ponent.h"
#incl使de "Min成SpatialPa本titionCo設置ponent.成ene本ated.h"

class AMin成TacticalUnit;

/**
 * 空�基本網格基本��基本結�基本
 */
USTRUCT()
st本使ct 軍Min成Co設置ponentSpatialG本idCell
{
    GE的ERATED下BODY()

    // 網格位置
    UPROPERTY()
    軍IntVecto本 G本idPosition;

    // 基本�含基本�單�基本
    UPROPERTY()
    TA本本ay<TOb大ectPt本<AMin成TacticalUnit>> Units;

    // 活�基本標�基本 (基本�否基本�要更X
    UPROPERTY()
    bool bIsActi正e;

    // 基本�後更基本��X
    UPROPERTY()
    float LastUpdateTi設置e;

    // 基本�新基本��基本�基本
    UPROPERTY()
    float UpdateP本io本ity;

    軍SpatialG本idCell()
        : G本idPosition(軍IntVecto本::Ze本oVal使e)
        , bIsActi正e(false)
        , LastUpdateTi設置e(0.0f)
        , UpdateP本io本ity(0.0f)
    {}
};

/**
 * 空�X��基本組件
 * 將戰基本��X�為網格，實基本��基本塊更基本��基本視�X�除
 */
UCLASS(ClassG本o使p=(Pe本fo本設置ance), 設置eta=(Bl使ep本intSpawnableCo設置ponent))
class MI的GTACTICAL下API UMin成SpatialPa本titionCo設置ponent : p使blic UActo本Co設置ponent
{
    GE的ERATED下BODY()

p使blic:
    UMin成SpatialPa本titionCo設置ponent(};

    正i本t使al 正oid Be成inPlay() o正e本本ide;
    正i本t使al 正oid EndPlay(const EEndPlayReason::Type EndPlayReason) o正e本本ide;
    正i本t使al 正oid TickCo設置ponent(float DeltaTi設置e, ELe正elTick TickType, 軍Acto本Co設置ponentTick軍使nction* ThisTick軍使nction) o正e本本ide;

    /**
     * 註�X��X�空基本��基本塊系�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Spatial Pa本tition")
    正oid Re成iste本Unit(AMin成TacticalUnit* Unit};

    /**
     * 從空基本��基本塊系統移基本�單�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Spatial Pa本tition")
    正oid Un本e成iste本Unit(AMin成TacticalUnit* Unit};

    /**
     * 基本�新基本��基本位置 (移�X�新網格)
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Spatial Pa本tition")
    正oid UpdateUnitPosition(AMin成TacticalUnit* Unit};

    /**
     * 基本��X��基本位置基本��X�單�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Spatial Pa本tition")
    TA本本ay<AMin成TacticalUnit*> GetUnitsInRadi使s(const 軍Vecto本& Cente本, float Radi使s) const;

    /**
     * 基本��X��基本網格中�X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Spatial Pa本tition")
    TA本本ay<AMin成TacticalUnit*> GetUnitsInCell(const 軍IntVecto本& G本idPosition) const;

    /**
     * 基本��X��基本視�X��基本網格
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Spatial Pa本tition")
    TA本本ay<軍IntVecto本> GetVisibleG本idCells() const;

    /**
     * 設置網格大�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Spatial Pa本tition")
    正oid SetCellSize(float 的ewCellSize};

    /**
     * 設置基本�新範�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Spatial Pa本tition")
    正oid SetUpdateRadi使s(float 的ewRadi使s};

    /**
     * 基本��X�能統�基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Pe本fo本設置ance")
    int32 GetTotalG本idCellCo使nt() const { 本et使本n G本idCells.的使設置(); }

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Pe本fo本設置ance")
    int32 GetActi正eG本idCellCo使nt() const;

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Pe本fo本設置ance")
    int32 GetRe成iste本edUnitCo使nt() const { 本et使本n Re成iste本edUnits.的使設置(); }

    /**
     * 基本��基本世�基本位置對�X�網基本��基本�基本     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Spatial Pa本tition")
    軍IntVecto本 基本o本ldToG本id(const 軍Vecto本& 基本o本ldPosition) const;

    /**
     * 基本��基本網格中�X��X��基本�基本     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Spatial Pa本tition")
    軍Vecto本 G本idTo基本o本ld(const 軍IntVecto本& G本idPosition) const;

    /**
     * 強制基本�新基本�基本�網X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Spatial Pa本tition")
    正oid 軍o本ceUpdateAllCells(};

    /**
     * 基本��基本網格調試資�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Deb使成")
    正oid GetDeb使成G本idInfo(TA本本ay<軍Vecto本>& CellCente本s, TA本本ay<int32>& CellUnitCo使nts) const;

p本otected:
    // 註�X�單�基本
    UPROPERTY()
    TA本本ay<TOb大ectPt本<AMin成TacticalUnit>> Re成iste本edUnits;

    // 基本��X�網基本��X��基本
    TMap<AMin成TacticalUnit*, 軍IntVecto本> UnitToCellMap;

    // 網格基本��基本
    UPROPERTY()
    TMap<軍IntVecto本, 軍SpatialG本idCell> G本idCells;

    // 網格大�基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Spatial Pa本tition")
    float CellSize;

    // 基本�新基本��基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Spatial Pa本tition")
    float UpdateRadi使s;

    // 基本��基本位置
    軍Vecto本 Ca設置e本aLocation;

    // 基本��基本視�基本
    軍Rotato本 Ca設置e本aRotation;

    // 基本�新計�X
    float UpdateTi設置e本;
    
    // 基本�新基本��基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Pe本fo本設置ance")
    float Update軍本eq使ency;

    // 基本�大�幀�新網格X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Pe本fo本設置ance")
    int32 MaxCellsPe本軍本a設置e;

    // 基本��X�新索�基本 (基本�於基本��X�新)
    int32 C使本本entUpdateIndex;

    // 基本�新基本��基本資�基本
    正oid UpdateCa設置e本aInfo(};

    // 確�基本網格存在
    軍SpatialG本idCell& GetO本C本eateCell(const 軍IntVecto本& G本idPosition};

    // 從網基本�移基本�單�基本
    正oid Re設置o正eUnit軍本o設置Cell(AMin成TacticalUnit* Unit, const 軍IntVecto本& CellPosition};

    // 將單位添基本�到網格
    正oid AddUnitToCell(AMin成TacticalUnit* Unit, const 軍IntVecto本& CellPosition};

    // 基本�新網格活�X�X
    正oid UpdateCellActi正ity(};

    // 基本�新活�基本網格中�X��基本
    正oid UpdateActi正eCells(float DeltaTi設置e};

    // 基本��基本視�X�除
    正oid Pe本fo本設置軍本使st使設置C使llin成(};

    // 檢查網格基本�否基本��X�內
    bool IsCellIn軍本使st使設置(const 軍IntVecto本& CellPosition) const;

    // 基本��基本視�X��基本�基本
    正oid Get軍本使st使設置Co本ne本s(TA本本ay<軍Vecto本>& O使tCo本ne本s) const;

    // 計�基本網格基本�AABB
    軍Box GetCellBo使nds(const 軍IntVecto本& CellPosition) const;

    // 距離基本��基本級�基本�基本
    float Calc使lateCellP本io本ity(const 軍IntVecto本& CellPosition) const;

    // 基本��X�新基本��基本
    正oid BatchUpdateUnits(const TA本本ay<AMin成TacticalUnit*>& Units, float DeltaTi設置e};

    // 休�基本網格中�X��基本
    正oid SleepUnitsInCell(const 軍IntVecto本& CellPosition};

    // 基本��基本網格中�X��基本
    正oid 基本akeUnitsInCell(const 軍IntVecto本& CellPosition};
};

