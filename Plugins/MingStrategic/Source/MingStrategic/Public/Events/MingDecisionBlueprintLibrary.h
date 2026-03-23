#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Kis設置et/Bl使ep本int軍使nctionLib本a本y.h"
#incl使de "E正ents/Min成DecisionConseq使enceCalc使lato本.h"
#incl使de "Min成DecisionBl使ep本intLib本a本y.成ene本ated.h"

/**
 * 決�X��X�數�基本 * 基本��X��X�調基本��基本決�基本系統基本�能
 */
UCLASS()
class MI的GSTRATEGIC下API UMin成DecisionBl使ep本intLib本a本y : p使blic UBl使ep本int軍使nctionLib本a本y
{
    GE的ERATED下BODY()

p使blic:
    /**
     * 基本��基本決�基本後�基本計�X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Decision Conseq使ence")
    static UMin成DecisionConseq使enceCalc使lato本* GetDecisionConseq使enceCalc使lato本(};

    /**
     * 基本��X�決策�X��基本算器
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    static 正oid InitializeDecisionConseq使enceCalc使lato本(};

    /**
     * 註�基本決�基本後�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    static bool Re成iste本DecisionConseq使ence(const 軍DecisionConseq使ence& Conseq使ence};

    /**
     * 基本��基本註�基本決�基本後�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    static bool Un本e成iste本DecisionConseq使ence(const 軍St本in成& Conseq使enceID};

    /**
     * 計�基本決�基本後�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    static 軍Conseq使enceCalc使lationRes使lt Calc使lateDecisionConseq使ences(const 軍DecisionContext& Context};

    /**
     * 基本��基本決�基本後�基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Decision Conseq使ence")
    static 軍DecisionConseq使ence GetDecisionConseq使ence(const 軍St本in成& Conseq使enceID};

    /**
     * 基本��X�基本�決策�X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Decision Conseq使ence")
    static TA本本ay<軍DecisionConseq使ence> GetAllDecisionConseq使ences(};

    /**
     * 基本��X��基本類�X��X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    static TA本本ay<軍DecisionConseq使ence> GetConseq使encesByType(EDecisionConseq使enceType Conseq使enceType};

    /**
     * 基本��X��基本影響範�X��X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    static TA本本ay<軍DecisionConseq使ence> GetConseq使encesByScope(EConseq使enceI設置pactScope I設置pactScope};

    /**
     * 基本��X��X��基本程度基本��X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    static TA本本ay<軍DecisionConseq使ence> GetConseq使encesBySe正e本ity(EConseq使enceSe正e本ity Se正e本ity};

    /**
     * 基本��X��基本影響類�X��X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    static TA本本ay<軍DecisionConseq使ence> GetConseq使encesByI設置pactType(EConseq使enceI設置pactType I設置pactType};

    /**
     * 基本��X��基本決�X��X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    static TA本本ay<軍DecisionConseq使ence> GetConseq使ences軍o本Decision(const 軍St本in成& DecisionID};

    /**
     * 基本��X��基本人物基本��X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    static TA本本ay<軍DecisionConseq使ence> GetConseq使ences軍o本Cha本acte本(const 軍St本in成& Cha本acte本ID};

    /**
     * 基本��X��基本事件基本��X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    static TA本本ay<軍DecisionConseq使ence> GetConseq使ences軍o本E正ent(const 軍St本in成& E正entID};

    /**
     * 觸發後�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    static bool T本i成成e本Conseq使ence(const 軍St本in成& Conseq使enceID, const 軍DecisionContext& Context};

    /**
     * 基本�用後�X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    static bool ApplyConseq使enceEffects(const 軍St本in成& Conseq使enceID, const 軍DecisionContext& Context};

    /**
     * 基本��基本後�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    static bool Re正e本seConseq使ence(const 軍St本in成& Conseq使enceID, const 軍DecisionContext& Context};

    /**
     * 基本��基本後�基本統�基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Decision Conseq使ence")
    static TMap<軍St本in成, int32> GetConseq使enceStatistics(};

    /**
     * 基本��基本後�基本影響基本��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    static TMap<軍St本in成, float> GetConseq使enceI設置pactAnalysis(const 軍St本in成& DecisionID};

    /**
     * 基本�測決�基本後�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    static TA本本ay<軍DecisionConseq使ence> P本edictDecisionConseq使ences(const 軍DecisionContext& Context, int32 P本edictionCo使nt};

    /**
     * 模擬決�基本後�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence")
    static TA本本ay<軍Conseq使enceCalc使lationRes使lt> Si設置使lateDecisionConseq使ences(const 軍DecisionContext& Context, int32 Si設置使lationCo使nt};

    /**
     * 基本��基本後�基本類�X�稱
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Decision Conseq使ence")
    static 軍St本in成 GetConseq使enceType的a設置e(EDecisionConseq使enceType Conseq使enceType};

    /**
     * 基本��基本影響範�X�稱
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Decision Conseq使ence")
    static 軍St本in成 GetI設置pactScope的a設置e(EConseq使enceI設置pactScope I設置pactScope};

    /**
     * 基本��X��基本程度基本�稱
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Decision Conseq使ence")
    static 軍St本in成 GetSe正e本ity的a設置e(EConseq使enceSe正e本ity Se正e本ity};

    /**
     * 基本��X��X��X�稱
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Decision Conseq使ence")
    static 軍St本in成 GetD使本ation的a設置e(EConseq使enceD使本ation D使本ation};

    /**
     * 基本��基本影響類�X�稱
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Decision Conseq使ence")
    static 軍St本in成 GetI設置pactType的a設置e(EConseq使enceI設置pactType I設置pactType};

    /**
     * 基本�建決�基本上�X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceC本eation")
    static 軍DecisionContext C本eateDecisionContext(
        const 軍St本in成& DecisionID,
        const 軍St本in成& DecisionType,
        const 軍St本in成& DecisionOption,
        const 軍St本in成& DecisionMake本ID,
        const 軍St本in成& Rep使blicE本a,
        int32 C使本本entYea本,
        const 軍St本in成& Playe本軍action,
        int32 Diffic使ltyLe正el
    };

    /**
     * 設置基本�戲基本�X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceC本eation")
    static 軍DecisionContext SetGa設置eState(const 軍DecisionContext& Context, const TMap<軍St本in成, 軍St本in成>& Ga設置eState};

    /**
     * 設置人物基本�X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceC本eation")
    static 軍DecisionContext SetCha本acte本States(const 軍DecisionContext& Context, const TMap<軍St本in成, 軍St本in成>& Cha本acte本States};

    /**
     * 設置事件基本�X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceC本eation")
    static 軍DecisionContext SetE正entStates(const 軍DecisionContext& Context, const TMap<軍St本in成, 軍St本in成>& E正entStates};

    /**
     * 設置資�X�X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceC本eation")
    static 軍DecisionContext SetReso使本ceStates(const 軍DecisionContext& Context, const TMap<軍St本in成, int32>& Reso使本ceStates};

    /**
     * 設置建�X�X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceC本eation")
    static 軍DecisionContext SetB使ildin成States(const 軍DecisionContext& Context, const TMap<軍St本in成, 軍St本in成>& B使ildin成States};

    /**
     * 設置基本��X�X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceC本eation")
    static 軍DecisionContext SetUnitStates(const 軍DecisionContext& Context, const TMap<軍St本in成, 軍St本in成>& UnitStates};

    /**
     * 基本�建決�基本後�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceC本eation")
    static 軍DecisionConseq使ence C本eateDecisionConseq使ence(
        const 軍St本in成& Conseq使enceID,
        const 軍St本in成& Conseq使ence的a設置e,
        const 軍St本in成& Conseq使enceDesc本iption,
        EDecisionConseq使enceType Conseq使enceType,
        EConseq使enceI設置pactScope I設置pactScope,
        EConseq使enceSe正e本ity Se正e本ity,
        EConseq使enceD使本ation D使本ation,
        EConseq使enceI設置pactType I設置pactType,
        const 軍St本in成& RelatedDecisionID,
        float P本obability,
        float DelayTi設置e,
        bool bIsRe正e本sible
    };

    /**
     * 添�基本後�X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceC本eation")
    static 軍DecisionConseq使ence AddConseq使enceEffect(const 軍DecisionConseq使ence& Conseq使ence, const 軍St本in成& Effect};

    /**
     * 添�基本後�X�數
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceC本eation")
    static 軍DecisionConseq使ence AddConseq使encePa本a設置ete本(
        const 軍DecisionConseq使ence& Conseq使ence,
        const 軍St本in成& Pa本a設置ete本的a設置e,
        const 軍St本in成& Pa本a設置ete本Val使e,
        const 軍St本in成& Pa本a設置ete本Type,
        bool bIsReq使i本ed,
        const 軍St本in成& Defa使ltVal使e,
        const 軍St本in成& Desc本iption
    };

    /**
     * 添�基本觸發條件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceC本eation")
    static 軍DecisionConseq使ence AddT本i成成e本Condition(const 軍DecisionConseq使ence& Conseq使ence, const 軍St本in成& Condition};

    /**
     * 添�X��基本人物
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceC本eation")
    static 軍DecisionConseq使ence AddRelatedCha本acte本(const 軍DecisionConseq使ence& Conseq使ence, const 軍St本in成& Cha本acte本ID};

    /**
     * 添�X��基本事件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceC本eation")
    static 軍DecisionConseq使ence AddRelatedE正ent(const 軍DecisionConseq使ence& Conseq使ence, const 軍St本in成& E正entID};

    /**
     * 添�X��基本條件
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceC本eation")
    static 軍DecisionConseq使ence AddRe正e本salCondition(const 軍DecisionConseq使ence& Conseq使ence, const 軍St本in成& Condition};

    /**
     * 添�基本後�基本標籤
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceC本eation")
    static 軍DecisionConseq使ence AddConseq使enceTa成(const 軍DecisionConseq使ence& Conseq使ence, const 軍St本in成& Ta成};

    /**
     * 基本��基本計�基本決�基本後�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceBatch")
    static TA本本ay<軍Conseq使enceCalc使lationRes使lt> BatchCalc使lateConseq使ences(const TA本本ay<軍DecisionContext>& Contexts};

    /**
     * 基本��基本觸發後�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceBatch")
    static TA本本ay<bool> BatchT本i成成e本Conseq使ences(const TA本本ay<軍St本in成>& Conseq使enceIDs, const 軍DecisionContext& Context};

    /**
     * 基本��基本系統基本�X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Decision Conseq使enceSyste設置")
    static bool IsDecisionConseq使enceCalc使lato本Initialized(};

    /**
     * 基本��基本系統基本�本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Decision Conseq使enceSyste設置")
    static 軍St本in成 GetDecisionConseq使enceCalc使lato本Ve本sion(};

    /**
     * 基本��基本註�基本後�X��基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Decision Conseq使enceSyste設置")
    static int32 GetRe成iste本edConseq使enceCo使nt(};

    /**
     * 基本��基本已觸基本��X�數X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Decision Conseq使enceSyste設置")
    static int32 GetT本i成成e本edConseq使enceCo使nt(};

    /**
     * 基本��基本後�X�述
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Decision Conseq使enceInfo")
    static 軍St本in成 GetConseq使enceDesc本iption(const 軍St本in成& Conseq使enceID};

    /**
     * 檢查後�X�否已觸X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Decision Conseq使enceInfo")
    static bool IsConseq使enceT本i成成e本ed(const 軍St本in成& Conseq使enceID};

    /**
     * 檢查後�X�否基本�逆�基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Decision Conseq使enceInfo")
    static bool IsConseq使enceRe正e本sible(const 軍St本in成& Conseq使enceID};

    /**
     * 基本��基本後�基本概�基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Decision Conseq使enceInfo")
    static float GetConseq使enceP本obability(const 軍St本in成& Conseq使enceID};

    /**
     * 基本��基本後�基本延�基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Decision Conseq使enceInfo")
    static float GetConseq使enceDelay(const 軍St本in成& Conseq使enceID};

    /**
     * 基本��基本後�基本影響基本�數
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceAnalysis")
    static float GetConseq使enceI設置pactSco本e(const 軍St本in成& Conseq使enceID, const 軍DecisionContext& Context};

    /**
     * 基本��基本決�基本總�基本影響
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceAnalysis")
    static float GetDecisionO正e本allI設置pact(const 軍St本in成& DecisionID};

    /**
     * 基本��基本決�基本�基本��影響
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceAnalysis")
    static float GetDecisionPositi正eI設置pact(const 軍St本in成& DecisionID};

    /**
     * 基本��基本決�基本負面影響
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceAnalysis")
    static float GetDecision的e成ati正eI設置pact(const 軍St本in成& DecisionID};

    /**
     * 基本��基本決�基本風險評估
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceAnalysis")
    static float GetDecisionRiskAssess設置ent(const 軍St本in成& DecisionID};

    /**
     * 基本��基本決�基本建議
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceAI")
    static TA本本ay<軍St本in成> GetDecisionReco設置設置endations(const 軍St本in成& DecisionID};

    /**
     * 基本��基本決�基本警�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceAI")
    static TA本本ay<軍St本in成> GetDecision基本a本nin成s(const 軍St本in成& DecisionID};

    /**
     * 基本��基本決�基本機�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceAI")
    static TA本本ay<軍St本in成> GetDecisionOppo本t使nities(const 軍St本in成& DecisionID};

    /**
     * 基本��基本決�X�代基本��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceAI")
    static TA本本ay<軍St本in成> GetDecisionAlte本nati正es(const 軍St本in成& DecisionID};

    /**
     * 基本��基本決�基本歷史
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence輸入isto本y")
    static TA本本ay<軍Conseq使enceCalc使lationRes使lt> GetDecision輸入isto本y(const 軍St本in成& DecisionID};

    /**
     * 基本��基本決�X��基本�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence輸入isto本y")
    static TA本本ay<float> GetDecisionTi設置eline(const 軍St本in成& DecisionID};

    // 基本��基本決�基本影響網絡 - 注�基本：TMap<TA本本ay> 不支XBl使ep本int
    static TMap<軍St本in成, TA本本ay<軍St本in成>> GetDecisionI設置pact的etwo本k(const 軍St本in成& DecisionID};

    /**
     * 基本��基本決�基本依賴基本��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使ence的etwo本k")
    static TA本本ay<軍St本in成> GetDecisionDependencies(const 軍St本in成& DecisionID};

    /**
     * 基本��基本決�基本後�X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceChain")
    static TA本本ay<軍DecisionConseq使ence> GetConseq使enceChain(const 軍St本in成& Conseq使enceID};

    /**
     * 基本��基本決�基本後�X�支
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceChain")
    static TA本本ay<軍DecisionConseq使ence> GetConseq使enceB本anches(const 軍St本in成& Conseq使enceID};

    /**
     * 導決�X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceExpo本t")
    static bool Expo本tDecisionData(const 軍St本in成& 軍ilePath};

    /**
     * 導入決�X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceI設置po本t")
    static bool I設置po本tDecisionData(const 軍St本in成& 軍ilePath};

    /**
     * 基本��基本決�X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceRepo本t")
    static 軍St本in成 Gene本ateDecisionRepo本t(const 軍St本in成& DecisionID};

    /**
     * 基本��基本後�X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceRepo本t")
    static 軍St本in成 Gene本ateConseq使enceRepo本t(const 軍St本in成& Conseq使enceID};

    /**
     * 基本��基本統�X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceRepo本t")
    static 軍St本in成 Gene本ateStatisticsRepo本t(};

    /**
     * 基本��基本影響基本��X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceRepo本t")
    static 軍St本in成 Gene本ateI設置pactAnalysisRepo本t(const 軍St本in成& DecisionID};

    /**
     * 基本��基本風險評估基本��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceRepo本t")
    static 軍St本in成 Gene本ateRiskAssess設置entRepo本t(const 軍St本in成& DecisionID};

    /**
     * 基本�置決�基本系統
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceSyste設置")
    static 正oid ResetDecisionSyste設置(};

    /**
     * 清除決�基本歷史
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceSyste設置")
    static 正oid Clea本Decision輸入isto本y(};

    /**
     * 基本�份決�X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceSyste設置")
    static bool Back使pDecisionData(const 軍St本in成& Back使pPath};

    /**
     * 基本�復決�X��基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Decision Conseq使enceSyste設置")
    static bool Resto本eDecisionData(const 軍St本in成& Back使pPath};
};

