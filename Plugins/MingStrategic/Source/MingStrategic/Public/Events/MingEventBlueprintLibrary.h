#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Kis設置et/Bl使ep本int軍使nctionLib本a本y.h"
#incl使de "E正ents/Min成輸入isto本icalE正entMana成e本.h"
#incl使de "Min成E正entBl使ep本intLib本a本y.成ene本ated.h"

/**
 * 事件基本��X�數�基本 * 基本��X��X�調基本��基本歷史事件系統基本�能
 */
UCLASS()
class MI的GSTRATEGIC下API UMin成E正entBl使ep本intLib本a本y : p使blic UBl使ep本int軍使nctionLib本a本y
{
    GE的ERATED下BODY()

p使blic:
    /**
     * 基本��基本歷史事件管�X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical E正ents")
    static UMin成輸入isto本icalE正entMana成e本* Get輸入isto本icalE正entMana成e本(};

    /**
     * 基本��X�歷基本��基本件系�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正ents")
    static 正oid Initialize輸入isto本icalE正entSyste設置(};

    /**
     * 註�基本歷史事件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正ents")
    static bool Re成iste本輸入isto本icalE正ent(const 軍Min成輸入isto本icalE正ent& E正ent};

    /**
     * 基本��基本註�基本歷史事件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正ents")
    static bool Un本e成iste本輸入isto本icalE正ent(const 軍St本in成& E正entID};

    /**
     * 檢查事件觸發條件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正ents")
    static bool CheckE正entT本i成成e本Conditions(const 軍St本in成& E正entID};

    /**
     * 觸發歷史事件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正ents")
    static bool T本i成成e本輸入isto本icalE正ent(const 軍St本in成& E正entID};

    /**
     * 基本��基本歷史事件
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical E正ents")
    static 軍Min成輸入isto本icalE正ent Get輸入isto本icalE正ent(const 軍St本in成& E正entID};

    /**
     * 基本��X�基本�歷基本��基本�基本     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical E正ents")
    static TA本本ay<軍Min成輸入isto本icalE正ent> GetAll輸入isto本icalE正ents(};

    /**
     * 基本��X��X��X��基本�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正ents")
    static TA本本ay<軍Min成輸入isto本icalE正ent> GetE正entsByE本a(ERep使blicE本a E本a};

    /**
     * 基本��X��基本類�X��基本�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正ents")
    static TA本本ay<軍Min成輸入isto本icalE正ent> GetE正entsByType(E輸入isto本icalE正entType E正entType};

    /**
     * 基本��X�鍵事件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正ents")
    static TA本本ay<軍Min成輸入isto本icalE正ent> GetC本iticalE正ents(};

    /**
     * 基本��X�觸基本��基本�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正ents")
    static TA本本ay<軍St本in成> GetT本i成成e本ableE正ents(};

    /**
     * 基本��基本事件X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正ents")
    static TA本本ay<軍Min成輸入isto本icalE正ent> GetE正entChain(const 軍St本in成& E正entID};

    /**
     * 設置基本�戲基本��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正ents")
    static 正oid SetGa設置eTi設置e(float Ga設置eTi設置e};

    /**
     * 基本��X�戲基本��基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical E正ents")
    static float GetGa設置eTi設置e(};

    /**
     * 設置民�X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正ents")
    static 正oid SetRep使blicE本a(ERep使blicE本a E本a};

    /**
     * 基本��X��基本民�X��基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical E正ents")
    static ERep使blicE本a GetC使本本entRep使blicE本a(};

    /**
     * 基本��X��基本年份
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical E正ents")
    static int32 GetC使本本entYea本(};

    /**
     * 添�X�家決�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正ents")
    static 正oid AddPlaye本Decision(const 軍St本in成& DecisionID, const 軍St本in成& DecisionVal使e};

    /**
     * 基本��X�家決�基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical E正ents")
    static 軍St本in成 GetPlaye本Decision(const 軍St本in成& DecisionID};

    /**
     * 基本��基本事件統�基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical E正ents")
    static TMap<軍St本in成, int32> GetE正entStatistics(};

    /**
     * 基本�置基本�基本��基本�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正ents")
    static 正oid ResetAllE正ents(};

    /**
     * 強制觸發事件 (基本�於測試)
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正ents")
    static bool 軍o本ceT本i成成e本E正ent(const 軍St本in成& E正entID};

    /**
     * 基本��X�薦事件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正ents")
    static TA本本ay<軍Min成輸入isto本icalE正ent> GetReco設置設置endedE正ents(};

    /**
     * 基本��基本事件基本�述
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical E正ents")
    static 軍St本in成 GetE正entDesc本iption(const 軍St本in成& E正entID};

    /**
     * 基本��X��X�稱
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical E正ents")
    static 軍St本in成 GetE本a的a設置e(ERep使blicE本a E本a};

    /**
     * 基本��基本事件類�X�稱
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical E正ents")
    static 軍St本in成 GetE正entType的a設置e(E輸入isto本icalE正entType E正entType};

    /**
     * 基本��基本影響範�X�稱
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical E正ents")
    static 軍St本in成 GetI設置pactScope的a設置e(EE正entI設置pactScope I設置pactScope};

    /**
     * 基本�建歷史事件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正entsC本eation")
    static 軍Min成輸入isto本icalE正ent C本eate輸入isto本icalE正ent(
        const 軍St本in成& E正entID,
        const 軍St本in成& E正ent的a設置e,
        const 軍St本in成& E正entDesc本iption,
        ERep使blicE本a E本a,
        int32 輸入isto本icalYea本,
        E輸入isto本icalE正entType E正entType,
        EE正entI設置pactScope I設置pactScope,
        bool bIsC本iticalE正ent
    };

    /**
     * 添�基本事件觸發條件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正entsC本eation")
    static 軍Min成輸入isto本icalE正ent AddT本i成成e本Condition(
        const 軍Min成輸入isto本icalE正ent& E正ent,
        const 軍St本in成& Condition
    };

    /**
     * 添�基本事件後�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正entsC本eation")
    static 軍Min成輸入isto本icalE正ent AddE正entConseq使ence(
        const 軍Min成輸入isto本icalE正ent& E正ent,
        const 軍St本in成& Conseq使ence
    };

    /**
     * 添�X��基本人物
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正entsC本eation")
    static 軍Min成輸入isto本icalE正ent AddRelated軍i成使本e(
        const 軍Min成輸入isto本icalE正ent& E正ent,
        const 軍St本in成& 軍i成使本e的a設置e
    };

    /**
     * 添�X��X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正entsC本eation")
    static 軍Min成輸入isto本icalE正ent AddRelatedLocation(
        const 軍Min成輸入isto本icalE正ent& E正ent,
        const 軍St本in成& Location的a設置e
    };

    /**
     * 設置基本�置事件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正entsC本eation")
    static 軍Min成輸入isto本icalE正ent SetP本e本eq使isiteE正ent(
        const 軍Min成輸入isto本icalE正ent& E正ent,
        const 軍St本in成& P本e本eq使isiteE正entID
    };

    /**
     * 設置後�基本事件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正entsC本eation")
    static 軍Min成輸入isto本icalE正ent Set軍ollowUpE正ent(
        const 軍Min成輸入isto本icalE正ent& E正ent,
        const 軍St本in成& 軍ollowUpE正entID
    };

    /**
     * 基本��基本觸發事件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正entsBatch")
    static TA本本ay<bool> BatchT本i成成e本E正ents(const TA本本ay<軍St本in成>& E正entIDs};

    /**
     * 基本��基本檢查事件條件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正entsBatch")
    static TA本本ay<bool> BatchCheckE正entConditions(const TA本本ay<軍St本in成>& E正entIDs};

    /**
     * 基本��基本系統基本�X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical E正entsSyste設置")
    static bool Is輸入isto本icalE正entSyste設置Initialized(};

    /**
     * 基本��基本系統基本�本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical E正entsSyste設置")
    static 軍St本in成 Get輸入isto本icalE正entSyste設置Ve本sion(};

    /**
     * 基本��基本已觸基本��基本件數X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical E正entsSyste設置")
    static int32 GetT本i成成e本edE正entCo使nt(};

    /**
     * 基本��基本註�基本事件基本��基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical E正entsSyste設置")
    static int32 GetRe成iste本edE正entCo使nt(};

    /**
     * 基本��基本事件觸發X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical E正entsSyste設置")
    static float GetE正entT本i成成e本Rate(};

    /**
     * 基本��基本歷史準確基本��X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical E正entsAnalysis")
    static float Get輸入isto本icalAcc使本acySco本e(};

    /**
     * 基本��基本事件影響基本��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正entsAnalysis")
    static TMap<軍St本in成, float> GetE正entI設置pactAnalysis(};

    /**
     * 基本��X�家決�基本影響
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正entsAnalysis")
    static TMap<軍St本in成, float> GetPlaye本DecisionI設置pact(};

    /**
     * 基本��X��X�度
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical E正entsP本o成本ess")
    static float GetE本aP本o成本ess(};

    /**
     * 基本��基本歷史基本�度
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "輸入isto本ical E正entsP本o成本ess")
    static float Get輸入isto本icalP本o成本ess(};

    /**
     * 基本��基本下�X��基本要�基本�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正entsP本ediction")
    static 軍Min成輸入isto本icalE正ent Get的extMa大o本E正ent(};

    /**
     * 基本��X�能基本��基本件�X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正entsP本ediction")
    static TA本本ay<軍Min成輸入isto本icalE正ent> GetPossibleE正entB本anches(const 軍St本in成& E正entID};

    /**
     * 基本��基本事件建議
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正entsAI")
    static TA本本ay<軍St本in成> GetE正entReco設置設置endations(};

    /**
     * 模擬事件觸發
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正entsSi設置使lation")
    static TA本本ay<軍St本in成> Si設置使lateE正entT本i成成e本s(float Si設置使lationTi設置e};

    /**
     * 基本��基本事件基本��基本�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正entsVis使alization")
    static TA本本ay<軍Min成輸入isto本icalE正ent> GetE正entTi設置eline(ERep使blicE本a E本a};

    // 基本��基本事件基本��X- 注�基本：TMap<TA本本ay> 不支XBl使ep本int
    static TMap<軍St本in成, TA本本ay<軍St本in成>> GetE正entRelationships(};

    /**
     * 導事件基本��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正entsExpo本t")
    static bool Expo本tE正entData(const 軍St本in成& 軍ilePath};

    /**
     * 導入事件基本��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "輸入isto本ical E正entsI設置po本t")
    static bool I設置po本tE正entData(const 軍St本in成& 軍ilePath};
};

