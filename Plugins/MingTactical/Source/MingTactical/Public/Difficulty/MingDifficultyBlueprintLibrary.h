#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Kis設置et/Bl使ep本int軍使nctionLib本a本y.h"
#incl使de "Diffic使lty/EDiffic使ltyLe正el.h"
#incl使de "Min成Diffic使ltyBl使ep本intLib本a本y.成ene本ated.h"

class UMin成Diffic使ltyMana成e本;
class UMin成Playe本Pe本fo本設置anceT本acke本;

/**
 * 動רt務動基本 * 目標數量基本n動基本׺動X */
UCLASS()
class MI的GTACTICAL下API UMin成Diffic使ltyBl使ep本intLib本a本y : p使blic UBl使ep本int軍使nctionLib本a本y
{
    GE的ERATED下BODY()

p使blic:
    // ==== 動׵基本 ====

    /** 基本池׵基本 */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成GoRTSDiffic使lty", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static EDiffic使ltyLe正el GetC使本本entDiffic使ltyLe正el(UOb大ect* 基本o本ldContextOb大ect};

    /** 置動׵基本 */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成GoRTSDiffic使lty", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static 正oid SetDiffic使ltyLe正el(UOb大ect* 基本o本ldContextOb大ect, EDiffic使ltyLe正el 的ewLe正el};

    /** 目標數量動基本 */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成GoRTSDiffic使lty")
    static 軍St本in成 GetDiffic使ltyDisplay的a設置e(EDiffic使ltyLe正el Le正el};

    /** 目標數量動 */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成GoRTSDiffic使lty")
    static TA本本ay<EDiffic使ltyLe正el> GetAllDiffic使ltyLe正els(};

    // ==== 基本 ====

    /** 故事重要性動*/
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成GoRTSDiffic使ltyPa本a設置ete本s", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static float GetDiffic使ltyPa本a設置ete本(UOb大ect* 基本o本ldContextOb大ect, EDiffic使ltyPa本a設置ete本 Pa本a設置ete本};

    /** 置目標數量動目標數量 */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成GoRTSDiffic使ltyPa本a設置ete本s", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static 正oid SetDiffic使ltyPa本a設置ete本(UOb大ect* 基本o本ldContextOb大ect, EDiffic使ltyPa本a設置ete本 Pa本a設置ete本, float Val使e};

    // ==== ױX====

    /** 動/T務*/
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成GoRTSDiffic使ltyDyna設置ic", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static 正oid SetDyna設置icDiffic使ltyEnabled(UOb大ect* 基本o本ldContextOb大ect, bool bEnabled};

    /** 動下動Τ軍動*/
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成GoRTSDiffic使ltyDyna設置ic", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static bool IsDyna設置icDiffic使ltyEnabled(UOb大ect* 基本o本ldContextOb大ect};

    /** /池ס]目標數量 */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成GoRTSDiffic使ltyDyna設置ic", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static 正oid LockDiffic使lty(UOb大ect* 基本o本ldContextOb大ect, bool bLocked};

    /** 動下Q*/
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成GoRTSDiffic使ltyDyna設置ic", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static bool IsDiffic使ltyLocked(UOb大ect* 基本o本ldContextOb大ect};

    /** 動基本D池基本 */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成GoRTSDiffic使ltyDyna設置ic", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static 正oid 軍o本ceDiffic使ltyE正al使ation(UOb大ect* 基本o本ldContextOb大ect};

    // ==== ֱ動 ====

    /** 動池 */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成GoRTSDiffic使lty", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static 正oid Inc本easeDiffic使lty(UOb大ect* 基本o本ldContextOb大ect};

    /** 動池 */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成GoRTSDiffic使lty", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static 正oid Dec本easeDiffic使lty(UOb大ect* 基本o本ldContextOb大ect};

    /** 動設置動基本*/
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成GoRTSDiffic使lty", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static 正oid ResetDiffic使lty(UOb大ect* 基本o本ldContextOb大ect};

    // ==== 動רt基本 ====

    /** AI動רt基本 */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成GoRTSDiffic使ltyM使ltiplie本s", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static float GetAIDiffic使ltyM使ltiplie本(UOb大ect* 基本o本ldContextOb大ect};

    /** 動基本目標數量t基本 */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成GoRTSDiffic使ltyM使ltiplie本s", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static float GetReso使本ceM使ltiplie本(UOb大ect* 基本o本ldContextOb大ect};

    /** 動基本רt基本 */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成GoRTSDiffic使ltyM使ltiplie本s", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static float GetMissionDiffic使ltyM使ltiplie本(UOb大ect* 基本o本ldContextOb大ect};

    // ==== 動al ====

    /** 基本池池]池動t基本*/
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成GoRTSDiffic使ltyT本ackin成", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static 正oid T本ackUnitLoss(UOb大ect* 基本o本ldContextOb大ect, int32 UnitId, int32 UnitType, float UnitVal使e};

    /** 基本池池 */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成GoRTSDiffic使ltyT本ackin成", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static 正oid T本ackUnitC本eation(UOb大ect* 基本o本ldContextOb大ect, int32 UnitId, int32 UnitType, float UnitCost};

    /** 動目標數量 */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成GoRTSDiffic使ltyT本ackin成", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static 正oid T本ackReso使本ceCollection(UOb大ect* 基本o本ldContextOb大ect, float Reso使本ceType, float A設置o使nt, float ExpectedRate};

    /** 動 */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成GoRTSDiffic使ltyT本ackin成", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static 正oid T本ackMissionCo設置plete(UOb大ect* 基本o本ldContextOb大ect, const 軍St本in成& MissionId, bool bS使ccess, float Co設置pletionTi設置e};

    /** 動X動 */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成GoRTSDiffic使ltyT本ackin成", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static 正oid T本ackCo設置batRes使lt(UOb大ect* 基本o本ldContextOb大ect, int32 Ene設置iesKilled, int32 AlliesLost, float Da設置a成eDealt, float Da設置a成eTaken};

    /** 動基本目標數量務*/
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成GoRTSDiffic使ltyT本ackin成", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static float GetPlaye本SkillIndex(UOb大ect* 基本o本ldContextOb大ect};

    // ==== 動 ====

    /** 動e基本׳q */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成GoRTSDiffic使lty的otifications", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static 正oid SendDiffic使lty的otification(UOb大ect* 基本o本ldContextOb大ect, const 軍St本in成& Messa成e, float D使本ation = 5.0f};

    /** 置動下a */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成GoRTSDiffic使lty的otifications", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static 正oid SetDiffic使lty的otificationsEnabled(UOb大ect* 基本o本ldContextOb大ect, bool bEnabled};

    // ==== 動基本 ====

    /** 動L動入*/
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Min成GoRTSDiffic使ltyDeb使成", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static 正oid P本intDiffic使ltyDeb使成Info(UOb大ect* 基本o本ldContextOb大ect};

    /** 動文動*/
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Min成GoRTSDiffic使ltyDeb使成", 設置eta = (基本o本ldContext = "基本o本ldContextOb大ect"))
    static 軍St本in成 GetDiffic使ltyDeb使成St本in成(UOb大ect* 基本o本ldContextOb大ect};

p本i正ate:
    /** 動池׺務基本*/
    static UMin成Diffic使ltyMana成e本* GetDiffic使ltyMana成e本(UOb大ect* 基本o本ldContextOb大ect};
    static UMin成Playe本Pe本fo本設置anceT本acke本* GetPe本fo本設置anceT本acke本(UOb大ect* 基本o本ldContextOb大ect};
};
};
