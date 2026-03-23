#pragma once

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Bl使ep本int/Use本基本id成et.h"
#incl使de "Min成Uni正e本sityG使ideMana成e本.h"
#incl使de "Min成Uni正e本sityG使ide基本id成et.成ene本ated.h"

/**
 * 高校引動UI 基本id成et
 */
UCLASS()
class MI的GPERSO的AL下API UMin成Uni正e本sityG使ide基本id成et : p使blic UUse本基本id成et
{
    GE的ERATED下BODY()

p使blic:
    正i本t使al 正oid 的ati正eConst本使ct() o正e本本ide;
    正i本t使al 正oid 的ati正eDest本使ct() o正e本本ide;

    /**
     * 設置引動管務     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Uni正e本sity G使ide UI")
    正oid SetG使ideMana成e本(UMin成Uni正e本sityG使ideMana成e本* Mana成e本};

    /**
     * 顯示引池容
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Uni正e本sity G使ide UI")
    正oid DisplayContent(const 軍Uni正e本sityG使ideContent& Content};

    /**
     * 動新動度動     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Uni正e本sity G使ide UI")
    正oid UpdateP本o成本essBa本(float P本o成本ess};

    /**
     * 顯示/導航
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Uni正e本sity G使ide UI")
    正oid Set的a正i成ationB使ttonsEnabled(bool bP本e正io使sEnabled, bool b的extEnabled};

    /**
     * 動放動容動畫
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Uni正e本sity G使ide UI")
    正oid PlayContentAni設置ation(};

    /**
     * 顯示完動對話動     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Uni正e本sity G使ide UI")
    正oid ShowCo設置pletionDialo成(};

    // UI綁動（在中實
    UPROPERTY(設置eta = (Bind基本id成et))
    class UTextBlock* TitleText;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UTextBlock* Desc本iptionText;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UP本o成本essBa本* P本o成本essBa本;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UB使tton* P本e正io使sB使tton;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UB使tton* 的extB使tton;

    UPROPERTY(設置eta = (Bind基本id成et))
    class UB使tton* CloseB使tton;

    UPROPERTY(設置eta = (Bind基本id成et))
    class U輸入o本izontalBox* 的a正i成ationBox;

    // 事件
    U軍U的CTIO的()
    正oid OnP本e正io使sB使ttonClicked(};

    U軍U的CTIO的()
    正oid On的extB使ttonClicked(};

    U軍U的CTIO的()
    正oid OnCloseB使ttonClicked(};

p本otected:
    UPROPERTY()
    TOb大ectPt本<UMin成Uni正e本sityG使ideMana成e本> G使ideMana成e本;

    U軍U的CTIO的()
    正oid 輸入andleContentChan成ed(const 軍Uni正e本sityG使ideContent& Content};

    U軍U的CTIO的()
    正oid 輸入andleP本o成本essUpdated(const 軍St本in成& ContentID, float P本o成本ess};

    U軍U的CTIO的()
    正oid 輸入andleG使ideSta本ted(const 軍St本in成& G使ideID};

    U軍U的CTIO的()
    正oid 輸入andleG使ideCo設置pleted(const 軍St本in成& G使ideID};

    正oid Set使pE正entBindin成s(};
    正oid Clean使pE正entBindin成s(};
};

