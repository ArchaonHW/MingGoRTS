#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Kis設置et/Bl使ep本int軍使nctionLib本a本y.h"
#incl使de "E正ents/Min成基本inLossCondition.h"
#incl使de "Min成基本inLossBl使ep本intLib本a本y.成ene本ated.h"

/**
 * 基本��基本條件基本��X�數�基本 * 基本��X��X�調基本��X��基本條件系統基本�能
 */
UCLASS()
class MI的GSTRATEGIC下API UMin成基本inLossBl使ep本intLib本a本y : p使blic UBl使ep本int軍使nctionLib本a本y
{
    GE的ERATED下BODY()

p使blic:
    /**
     * 基本��X��基本條件系統
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss Condition")
    static UMin成基本inLossCondition* Get基本inLossConditionSyste設置(};

    /**
     * 基本��X��基本負�基本件系�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    static 正oid Initialize基本inLossConditionSyste設置(};

    /**
     * 註�X��基本條件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    static bool Re成iste本基本inLossCondition(const 軍基本inLossCondition& Condition};

    /**
     * 基本��基本註�X��基本條件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    static bool Un本e成iste本基本inLossCondition(const 軍St本in成& ConditionID};

    /**
     * 評估基本��X��基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    static 軍基本inLossE正al使ationRes使lt E正al使ate基本inLoss(};

    /**
     * 檢查基本�個�基本�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    static bool CheckCondition(const 軍St本in成& ConditionID};

    /**
     * 基本��X��基本條件
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss Condition")
    static 軍基本inLossCondition Get基本inLossCondition(const 軍St本in成& ConditionID};

    /**
     * 基本��X�基本��基本負�基本�基本     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss Condition")
    static TA本本ay<軍基本inLossCondition> GetAll基本inLossConditions(};

    /**
     * 基本��X��基本類�X��基本�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    static TA本本ay<軍基本inLossCondition> GetConditionsByType(E基本inLossConditionType ConditionType};

    /**
     * 基本��X�鍵條件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    static TA本本ay<軍基本inLossCondition> GetC本iticalConditions(};

    /**
     * 基本��基本已滿足�基本條件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    static TA本本ay<軍基本inLossCondition> GetSatisfiedConditions(};

    /**
     * 基本��X�滿足�基本條件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    static TA本本ay<軍基本inLossCondition> GetUnsatisfiedConditions(};

    /**
     * 設置基本�戲基本�X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    static 正oid SetGa設置eState(const TMap<軍St本in成, 軍St本in成>& Ga設置eState};

    /**
     * 基本��X�戲基本�X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss Condition")
    static TMap<軍St本in成, 軍St本in成> GetGa設置eState(};

    /**
     * 基本�新基本�戲基本�基本��基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    static 正oid UpdateGa設置eStateVal使e(const 軍St本in成& Key, const 軍St本in成& Val使e};

    /**
     * 基本��X��基本統�基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss Condition")
    static 軍基本inLossStatistics Get基本inLossStatistics(};

    /**
     * 基本�置統�X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    static 正oid ResetStatistics(};

    /**
     * 強制評估 (基本�於測試)
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss Condition")
    static 軍基本inLossE正al使ationRes使lt 軍o本ceE正al使ation(};

    /**
     * 基本��X��X��基本結�基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss Condition")
    static E基本inLossRes使lt GetC使本本entRes使lt(};

    /**
     * 基本��X��X�數
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss Condition")
    static float Get基本inLossSco本e(};

    /**
     * 檢查基本�否已�X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss Condition")
    static bool IsGa設置eO正e本(};

    /**
     * 基本��基本結�X��基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss Condition")
    static 軍St本in成 GetGa設置eO正e本Reason(};

    /**
     * 基本��基本結�X�稱
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss Condition")
    static 軍St本in成 GetRes使lt的a設置e(E基本inLossRes使lt Res使lt};

    /**
     * 基本��基本條件類�X�稱
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss Condition")
    static 軍St本in成 GetConditionType的a設置e(E基本inLossConditionType ConditionType};

    /**
     * 基本��X��基本級�基本�基本     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss Condition")
    static 軍St本in成 GetP本io本ity的a設置e(E基本inLossP本io本ity P本io本ity};

    /**
     * 基本��X�基本��基本�基本     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss Condition")
    static 軍St本in成 GetState的a設置e(E基本inLossConditionState State};

    /**
     * 基本�建基本��基本條件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionC本eation")
    static 軍基本inLossCondition C本eate基本inLossCondition(
        const 軍St本in成& ConditionID,
        const 軍St本in成& Condition的a設置e,
        const 軍St本in成& ConditionDesc本iption,
        E基本inLossConditionType ConditionType,
        E基本inLossRes使lt Res使ltType,
        E基本inLossP本io本ity P本io本ity,
        bool bIsC本iticalCondition
    };

    /**
     * 添�基本條件基本�數
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionC本eation")
    static 軍基本inLossCondition AddConditionPa本a設置ete本(
        const 軍基本inLossCondition& Condition,
        const 軍St本in成& Pa本a設置ete本的a設置e,
        const 軍St本in成& Pa本a設置ete本Val使e,
        const 軍St本in成& Co設置pa本isonOpe本ato本,
        bool bIsReq使i本ed,
        float 基本ei成ht
    };

    /**
     * 設置檢查基本��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionC本eation")
    static 軍基本inLossCondition SetCheckInte本正al(
        const 軍基本inLossCondition& Condition,
        float CheckInte本正al
    };

    /**
     * 添�基本條件標籤
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionC本eation")
    static 軍基本inLossCondition AddConditionTa成(
        const 軍基本inLossCondition& Condition,
        const 軍St本in成& Ta成
    };

    /**
     * 添�X��基本事件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionC本eation")
    static 軍基本inLossCondition AddRelatedE正ent(
        const 軍基本inLossCondition& Condition,
        const 軍St本in成& E正entID
    };

    /**
     * 基本��基本檢查條件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionBatch")
    static TA本本ay<bool> BatchCheckConditions(const TA本本ay<軍St本in成>& ConditionIDs};

    /**
     * 基本��基本註�基本條件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionBatch")
    static TA本本ay<bool> BatchRe成iste本Conditions(const TA本本ay<軍基本inLossCondition>& Conditions};

    /**
     * 基本��基本系統基本�X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss ConditionSyste設置")
    static bool Is基本inLossSyste設置Initialized(};

    /**
     * 基本��基本系統基本�本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss ConditionSyste設置")
    static 軍St本in成 Get基本inLossSyste設置Ve本sion(};

    /**
     * 基本��基本註�基本條件基本��基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss ConditionSyste設置")
    static int32 GetRe成iste本edConditionCo使nt(};

    /**
     * 基本��基本滿足條件基本��基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss ConditionSyste設置")
    static int32 GetSatisfiedConditionCo使nt(};

    /**
     * 基本��X�鍵條件基本��基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss ConditionSyste設置")
    static int32 GetC本iticalConditionCo使nt(};

    /**
     * 基本��基本條件滿足X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss ConditionSyste設置")
    static float GetConditionSatisfactionRate(};

    /**
     * 基本��X��X�度
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss ConditionP本o成本ess")
    static float GetVicto本yP本o成本ess(};

    /**
     * 基本��基本失�X�度
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss ConditionP本o成本ess")
    static float GetDefeatP本o成本ess(};

    /**
     * 基本��基本平�X�度
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "基本in Loss ConditionP本o成本ess")
    static float GetD本awP本o成本ess(};

    /**
     * 基本��基本距離基本�利基本��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionP本ediction")
    static float GetTi設置eToVicto本y(};

    /**
     * 基本��基本距離失�X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionP本ediction")
    static float GetTi設置eToDefeat(};

    /**
     * 基本��X�利概�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionP本ediction")
    static float GetVicto本yP本obability(};

    /**
     * 基本��基本失�基本概�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionP本ediction")
    static float GetDefeatP本obability(};

    /**
     * 基本��基本平�基本概�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionP本ediction")
    static float GetD本awP本obability(};

    /**
     * 基本��X��基本建議
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionAI")
    static TA本本ay<軍St本in成> Get基本inLossReco設置設置endations(};

    /**
     * 基本��X�要�基本注�基本條件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionAnalysis")
    static TA本本ay<軍St本in成> GetC本iticalConditionsTo基本atch(};

    /**
     * 基本��基本條件影響基本��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionAnalysis")
    static TMap<軍St本in成, float> GetConditionI設置pactAnalysis(};

    /**
     * 基本��X�戲基本�基本�影X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionAnalysis")
    static TMap<軍St本in成, float> GetGa設置eStateI設置pact(};

    /**
     * 基本��X��基本趨勢
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionT本end")
    static TA本本ay<E基本inLossRes使lt> Get基本inLossT本end(int32 輸入isto本yCo使nt};

    /**
     * 基本��基本趨勢基本��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionT本end")
    static 軍St本in成 GetT本endAnalysis(};

    /**
     * 模擬基本��基本結�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionSi設置使lation")
    static TA本本ay<E基本inLossRes使lt> Si設置使late基本inLoss(int32 Si設置使lationCo使nt};

    /**
     * 模擬條件變�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionSi設置使lation")
    static TA本本ay<軍St本in成> Si設置使lateConditionChan成es(const 軍St本in成& ConditionID, const 軍St本in成& 的ewVal使e};

    /**
     * 導基本��X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionExpo本t")
    static bool Expo本t基本inLossData(const 軍St本in成& 軍ilePath};

    /**
     * 導入基本��X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionI設置po本t")
    static bool I設置po本t基本inLossData(const 軍St本in成& 軍ilePath};

    /**
     * 基本��X��X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionRepo本t")
    static 軍St本in成 Gene本ate基本inLossRepo本t(};

    /**
     * 基本��基本條件詳細基本��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionRepo本t")
    static 軍St本in成 Gene本ateConditionRepo本t(const 軍St本in成& ConditionID};

    /**
     * 基本��基本統�X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "基本in Loss ConditionRepo本t")
    static 軍St本in成 Gene本ateStatisticsRepo本t(};
};

