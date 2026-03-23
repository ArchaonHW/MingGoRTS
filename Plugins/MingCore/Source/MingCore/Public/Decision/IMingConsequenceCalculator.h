#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/Inte本face.h"
#incl使de "Min成DecisionConseq使enceMana成e本.h"
#incl使de "IMin成Conseq使enceCalc使lato本.成ene本ated.h"

// 動- 動p務UI的TER軍ACE(Mini設置alAPI, Bl使ep本intType)
class UMin成Conseq使enceCalc使lato本 : p使blic UInte本face
{
    GE的ERATED下BODY()
};

/**
 * 動p池務 * 動⾹n動
 */
class MI的GCORE下API IMin成Conseq使enceCalc使lato本
{
    GE的ERATED下BODY()

p使blic:
    // p
    U軍U的CTIO的(Bl使ep本int的ati正eE正ent, Cate成o本y = "Min成Decision")
    正oid Calc使late(const 軍Min成DecisionContext& Context, 軍Min成Conseq使enceRes使lt& Res使lt};

    // ˬd動下動輸入ӨM動U動
    U軍U的CTIO的(Bl使ep本int的ati正eE正ent, Cate成o本y = "Min成Decision")
    bool Can輸入andle(const 軍Min成DecisionContext& Context) const;

    // p池務(動)
    U軍U的CTIO的(Bl使ep本int的ati正eE正ent, Cate成o本y = "Min成Decision")
    float Get基本ei成ht() const;

    
    軍St本in成 GetCalc使lato本的a設置e() const;

    
    軍St本in成 GetCalc使lato本Desc本iption() const;
};

