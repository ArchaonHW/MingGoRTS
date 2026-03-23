#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Decision/IMin成Conseq使enceCalc使lato本.h"
#incl使de "Min成Econo設置icConseq使enceCalc使lato本.成ene本ated.h"

/**
 * �成動��動�p務 * �M動�p動�M動��動�ټh�正�T
 * 動�t��動��池動��動�СB�T�Y動
 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GCORE下API UMin成Econo設置icConseq使enceCalc使lato本 : p使blic UOb大ect, p使blic IMin成Conseq使enceCalc使lato本
{
    GE的ERATED下BODY()

p使blic:
    // ��
    UMin成Econo設置icConseq使enceCalc使lato本(};

    // ��{����
    正i本t使al 正oid Calc使late下I設置ple設置entation(const 軍Min成DecisionContext& Context, 軍Min成Conseq使enceRes使lt& Res使lt) o正e本本ide;
    正i本t使al bool Can輸入andle下I設置ple設置entation(const 軍Min成DecisionContext& Context) const o正e本本ide;
    正i本t使al float Get基本ei成ht下I設置ple設置entation() const o正e本本ide;
    正i本t使al 軍St本in成 GetCalc使lato本的a設置e下I設置ple設置entation() const o正e本本ide;
    正i本t使al 軍St本in成 GetCalc使lato本Desc本iption下I設置ple設置entation() const o正e本本ide;

p本otected:
    // �p動�成動��動��動
    正oid Calc使lateEcono設置icReso使本ces(const 軍Min成DecisionContext& Context, 軍Min成Econo設置icConseq使ence& Reso使本ces};

    // �p動�成池動��動
    正oid Calc使lateEcono設置icIndicato本s(const 軍Min成DecisionContext& Context, 軍Min成Econo設置icConseq使ence& Indicato本s};

    // �p池動�]�I��動
    正oid Calc使lateInf本ast本使ct使本e(const 軍Min成DecisionContext& Context, TA本本ay<軍St本in成>& Inf本ast本使ct使本eChan成es};

    // �p動�T池動�正�T
    正oid Calc使lateT本adeRelations(const 軍Min成DecisionContext& Context, TMap<軍St本in成, int32>& T本adeChan成es};

    // ��池��p動
    正oid Calc使lateRep使blicanE本aEcono設置y(const 軍Min成DecisionContext& Context, 軍Min成Econo設置icConseq使ence& Conseq使ence};

    // 動���正�T�p動
    正oid Calc使lateC使本本encyRefo本設置I設置pact(const 軍Min成DecisionContext& Context, 軍Min成Econo設置icConseq使ence& Conseq使ence};

    // �使�年動�正動
    正oid Calc使lateInd使st本ializationI設置pact(const 軍Min成DecisionContext& Context, 軍Min成Econo設置icConseq使ence& Conseq使ence};

    // �A�年�正�T�p動
    正oid Calc使lateA成本ic使lt使本alIss使es(const 軍Min成DecisionContext& Context, 軍Min成Econo設置icConseq使ence& Conseq使ence};

    // �年�T�̿�正�T�p動
    正oid Calc使late軍o本ei成nT本adeDependence(const 軍Min成DecisionContext& Context, 軍Min成Econo設置icConseq使ence& Conseq使ence};

p本i正ate:
    // �p池務
    static constexp本 float CALCULATOR下基本EIG輸入T = 0.7f;

    // ��池動�f����動
    static const TA本本ay<軍St本in成> Rep使blicanE本aC使本本encies;

    // �成動��動��動
    static const TA本本ay<軍St本in成> Econo設置icReso使本ceTypes;

    // �]�I��動
    static const TA本本ay<軍St本in成> Inf本ast本使ct使本eTypes;

    // �T動�٦�
    static const TArray<FString> TradePartners;
};

#endif // MINGECONOMICCONSEQUENCECALCULATOR_H
