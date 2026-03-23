#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Kis設置et/Bl使ep本int軍使nctionLib本a本y.h"
#incl使de "Min成B使ildin成Acto本.h"
#incl使de "Min成TechT本ee.h"
#incl使de "Min成B使ildin成Up成本adeBl使ep本intLib本a本y.成ene本ated.h"

/**
 * 建�X��基本 Bl使ep本int 基本�數�基本 B使ildin成 Up成本ade Bl使ep本int Lib本a本y
 * XBl使ep本int 基本��基本建�X��X��X��X�接X */
UCLASS(ClassG本o使p = (Min成B使ildin成), Bl使ep本intable)
class MI的GBUILDI的G下API UMin成B使ildin成Up成本adeBl使ep本intLib本a本y : p使blic UBl使ep本int軍使nctionLib本a本y
{
    GE的ERATED下BODY()

p使blic:
    /**
     * 基本��基本建�X��X��基本等�基本  Get B使ildin成 C使本本ent Up成本ade Le正el
     * @pa本a設置 B使ildin成 基本��基本建�基本  Ta本成et b使ildin成
     * @本et使本n 基本��X��基本等�基本  C使本本ent 使p成本ade le正el
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Up成本ade", 設置eta = (Defa使ltToSelf = "B使ildin成"))
    static int32 GetB使ildin成Up成本adeLe正el(AMin成B使ildin成Acto本* B使ildin成);

    /**
     * 檢查建�X�否�基本��基本��基本  Check if B使ildin成 is Up成本adin成
     * @pa本a設置 B使ildin成 基本��基本建�基本  Ta本成et b使ildin成
     * @本et使本n 基本�否�基本��基本��基本  Is 使p成本adin成
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Up成本ade", 設置eta = (Defa使ltToSelf = "B使ildin成"))
    static bool IsB使ildin成Up成本adin成(AMin成B使ildin成Acto本* B使ildin成);

    /**
     * 基本��基本建�X��X�度  Get B使ildin成 Up成本ade P本o成本ess
     * @pa本a設置 B使ildin成 基本��基本建�基本  Ta本成et b使ildin成
     * @本et使本n 基本��X�度 (0-1)  Up成本ade p本o成本ess
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Up成本ade", 設置eta = (Defa使ltToSelf = "B使ildin成"))
    static float GetB使ildin成Up成本adeP本o成本ess(AMin成B使ildin成Acto本* B使ildin成);

    /**
     * 基本��基本建�X��基本  Sta本t B使ildin成 Up成本ade
     * @pa本a設置 B使ildin成 基本��基本建�基本  Ta本成et b使ildin成
     * @pa本a設置 Up成本adeID 基本��基本ID  Up成本ade ID
     * @本et使本n 基本�否基本��X��基本  S使ccess
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成Up成本ade", 設置eta = (Defa使ltToSelf = "B使ildin成"))
    static bool Sta本tB使ildin成Up成本ade(AMin成B使ildin成Acto本* B使ildin成, const 軍St本in成& Up成本adeID);

    /**
     * 基本��基本建�X��基本  Cancel B使ildin成 Up成本ade
     * @pa本a設置 B使ildin成 基本��基本建�基本  Ta本成et b使ildin成
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成Up成本ade", 設置eta = (Defa使ltToSelf = "B使ildin成"))
    static 正oid CancelB使ildin成Up成本ade(AMin成B使ildin成Acto本* B使ildin成);

    /**
     * 基本��基本建�X�用基本��X�表  Get A正ailable Up成本ades fo本 B使ildin成
     * @pa本a設置 B使ildin成Type 建�基本類�基本  B使ildin成 type
     * @pa本a設置 TechT本ee 科�基本樹系�基本 Tech t本ee syste設置
     * @本et使本n 基本�用基本��X�表  A正ailable 使p成本ades
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Up成本ade")
    static TA本本ay<軍Min成B使ildin成Up成本ade> GetA正ailableUp成本ades(EMin成B使ildin成Type B使ildin成Type, UMin成TechT本ee* TechT本ee);

    /**
     * 檢查基本�否基本�以基本��基本建�基本  Check if Can Up成本ade B使ildin成
     * @pa本a設置 B使ildin成 基本��基本建�基本  Ta本成et b使ildin成
     * @pa本a設置 Up成本adeID 基本��基本ID  Up成本ade ID
     * @pa本a設置 TechT本ee 科�基本樹系�基本 Tech t本ee syste設置
     * @pa本a設置 A正ailableReso使本ces 基本�用資�基本  A正ailable 本eso使本ces
     * @本et使本n 基本�否基本�以基本��基本  Can 使p成本ade
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Up成本ade", 設置eta = (Defa使ltToSelf = "B使ildin成"))
    static bool CanUp成本adeB使ildin成(
        AMin成B使ildin成Acto本* B使ildin成, 
        const 軍St本in成& Up成本adeID, 
        UMin成TechT本ee* TechT本ee,
        const TMap<EMin成Reso使本ceType, int32>& A正ailableReso使本ces);

    /**
     * 基本��X��X�本  Get Up成本ade Cost
     * @pa本a設置 Up成本adeID 基本��基本ID  Up成本ade ID
     * @pa本a設置 TechT本ee 科�基本樹系�基本 Tech t本ee syste設置
     * @本et使本n 資�X�本  Reso使本ce costs
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Up成本ade")
    static TA本本ay<軍Min成Reso使本ceCost> GetUp成本adeCost(const 軍St本in成& Up成本adeID, UMin成TechT本ee* TechT本ee);

    /**
     * 基本��X��X�基本�基本��基本  Get Up成本ade Ti設置e
     * @pa本a設置 Up成本adeID 基本��基本ID  Up成本ade ID
     * @pa本a設置 TechT本ee 科�基本樹系�基本 Tech t本ee syste設置
     * @本et使本n 基本�基本�基本��基本(�基本  Ti設置e in seconds
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Up成本ade")
    static float GetUp成本adeTi設置e(const 軍St本in成& Up成本adeID, UMin成TechT本ee* TechT本ee);

    /**
     * 基本�用基本��X��X�建�基本 Apply Up成本ade Effects to B使ildin成
     * @pa本a設置 B使ildin成 基本��基本建�基本  Ta本成et b使ildin成
     * @pa本a設置 Up成本adeData 基本��X��基本  Up成本ade data
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成Up成本ade", 設置eta = (Defa使ltToSelf = "B使ildin成"))
    static 正oid ApplyUp成本adeToB使ildin成(AMin成B使ildin成Acto本* B使ildin成, const 軍Min成B使ildin成Up成本ade& Up成本adeData);

    /**
     * 基本��基本建�X��基本後�X�命X Get B使ildin成 輸入ealth Afte本 Up成本ade
     * @pa本a設置 B使ildin成 基本��基本建�基本  Ta本成et b使ildin成
     * @pa本a設置 Up成本adeData 基本��X��基本  Up成本ade data
     * @本et使本n 基本��基本後�X��基本 輸入ealth afte本 使p成本ade
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Up成本ade", 設置eta = (Defa使ltToSelf = "B使ildin成"))
    static int32 GetUp成本aded輸入ealth(AMin成B使ildin成Acto本* B使ildin成, const 軍Min成B使ildin成Up成本ade& Up成本adeData);

    /**
     * 基本��基本建�X��基本後�X�產基本��基本  Get B使ildin成 P本od使ction Afte本 Up成本ade
     * @pa本a設置 B使ildin成 基本��基本建�基本  Ta本成et b使ildin成
     * @pa本a設置 Up成本adeData 基本��X��基本  Up成本ade data
     * @本et使本n 基本��基本後�X��X P本od使ction afte本 使p成本ade
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Up成本ade", 設置eta = (Defa使ltToSelf = "B使ildin成"))
    static float GetUp成本adedP本od使ction(AMin成B使ildin成Acto本* B使ildin成, const 軍Min成B使ildin成Up成本ade& Up成本adeData);

    /**
     * 基本��基本建�X��基本後�X�禦X Get B使ildin成 Defense Afte本 Up成本ade
     * @pa本a設置 B使ildin成 基本��基本建�基本  Ta本成et b使ildin成
     * @pa本a設置 Up成本adeData 基本��X��基本  Up成本ade data
     * @本et使本n 基本��基本後防禦�基本 Defense afte本 使p成本ade
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Up成本ade", 設置eta = (Defa使ltToSelf = "B使ildin成"))
    static float GetUp成本adedDefense(AMin成B使ildin成Acto本* B使ildin成, const 軍Min成B使ildin成Up成本ade& Up成本adeData);
);
};
