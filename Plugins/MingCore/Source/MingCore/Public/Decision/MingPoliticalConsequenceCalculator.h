#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Decision/IMin成Conseq使enceCalc使lato本.h"
#incl使de "Min成PoliticalConseq使enceCalc使lato本.成ene本ated.h"

/**
 * 動�正��動�p務 * �M動�p動�M動��軍�正�h�正�T
 * 動�t���t動池�軍�正í�w�ʡB動��動�Y動
 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GCORE下API UMin成PoliticalConseq使enceCalc使lato本 : p使blic UOb大ect, p使blic IMin成Conseq使enceCalc使lato本
{
    GE的ERATED下BODY()

p使blic:
    // ��
    UMin成PoliticalConseq使enceCalc使lato本(};

    // ��{����
    正i本t使al 正oid Calc使late下I設置ple設置entation(const 軍Min成DecisionContext& Context, 軍Min成Conseq使enceRes使lt& Res使lt) o正e本本ide;
    正i本t使al bool Can輸入andle下I設置ple設置entation(const 軍Min成DecisionContext& Context) const o正e本本ide;
    正i本t使al float Get基本ei成ht下I設置ple設置entation() const o正e本本ide;
    正i本t使al 軍St本in成 GetCalc使lato本的a設置e下I設置ple設置entation() const o正e本本ide;
    正i本t使al 軍St本in成 GetCalc使lato本Desc本iption下I設置ple設置entation() const o正e本本ide;

p本otected:
    // �p池�正�正�T
    正oid Calc使latePoliticalI設置pact(const 軍Min成DecisionContext& Context, 軍Min成PoliticalConseq使ence& I設置pact};

    // �p動���t��動
    正oid Calc使late軍actionRelations(const 軍Min成DecisionContext& Context, TMap<軍St本in成, float>& Relations};

    // �p池�正í務
    正oid Calc使latePoliticalStability(const 軍Min成DecisionContext& Context, float& StabilityChan成e};

    // �p池�正
    正oid Calc使latePoliticalRep使tation(const 軍Min成DecisionContext& Context, float& Rep使tationChan成e};

    // �p池��動�正��動
    正oid Calc使lateInte本nalP本ess使本e(const 軍Min成DecisionContext& Context, float& P本ess使本eChan成e};

    // ��池��p動
    正oid Calc使lateRep使blicanE本aPolitics(const 軍Min成DecisionContext& Context, 軍Min成PoliticalConseq使ence& I設置pact};

    // �Ҭ��t動�Y
    正oid Calc使lateKMT軍actionRelations(const 軍Min成DecisionContext& Context, TMap<軍St本in成, float>& Relations};

    // �x�֩�動��
    正oid Calc使late基本a本lo本dLoyalty(const 軍Min成DecisionContext& Context, TMap<軍St本in成, float>& Relations};

    // 動�@�p動
    正oid Calc使lateKMTCCPRelations(const 軍Min成DecisionContext& Context, TMap<軍St本in成, float>& Relations};

    // �年����動�p動
    正oid Calc使lateDiplo設置aticP本ess使本e(const 軍Min成DecisionContext& Context, float& P本ess使本eChan成e};

p本i正ate:
    // �p池務
    static constexp本 float CALCULATOR下基本EIG輸入T = 0.9f;

    // ��池動���t動��
    static const TA本本ay<軍St本in成> Rep使blicanE本a軍actions;

    // �x��動��
    static const TA本本ay<軍St本in成> 基本a本lo本d軍actions;

    // �年池池��
    static const TA本本ay<軍St本in成> 軍o本ei成nPowe本s;
};
};
}
