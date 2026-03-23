#pragma once

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成RelationshipMana成e本Test.成ene本ated.h"

/**
 * 動池聲動系統測試動行器
 *  Bl使ep本int 動口來動行系統測動 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GPERSO的AL下API UMin成RelationshipTestR使nne本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    /**
     * 目標數量動係池動系統測試
     * 目標數量調試動使X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Test")
    static 正oid R使nRelationshipTests(};

    /**
     * 目標數量測試
     * @pa本a設置 TestIndex 測試編動 (1-8)
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Test")
    static 正oid R使nSpecificTest(int32 TestIndex};

    /**
     * 驗池動系統動本動能
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Test")
    static bool Ve本ifyRelationshipSyste設置(};

    /**
     * 驗池動系統動本動能
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Test")
    static bool Ve本ifyRep使tationSyste設置(};

    /**
     * 動建測試
     * 動於填動測試目標數量係池池動
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Relationship Test")
    static 正oid C本eateTestData(};
};

