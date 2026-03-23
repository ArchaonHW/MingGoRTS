#pragma once

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/Inte本face.h"
#incl使de "Laye本Inte成本ation/Min成軍o使本Laye本Types.h"
#incl使de "IMin成Ga設置eLaye本.成ene本ated.h"

// 基本�戲層�基本介面
UI的TER軍ACE(Mini設置alAPI, Bl使ep本intType)
class UMin成Ga設置eLaye本 : p使blic UInte本face
{
    GE的ERATED下BODY()
};

/**
 * 基本�戲層�基本介面
 * 基本�基本��基本層系統�X��基本介面
 */
class MI的GSTRATEGIC下API IMin成Ga設置eLaye本
{
    GE的ERATED下BODY()

p使blic:
    // 基本��X�層�基本
    U軍U的CTIO的(Bl使ep本int的ati正eE正ent, Cate成o本y = "Min成Laye本")
    正oid InitializeLaye本(};

    // 基本�新層�基本
    U軍U的CTIO的(Bl使ep本int的ati正eE正ent, Cate成o本y = "Min成Laye本")
    正oid UpdateLaye本(float DeltaTi設置e};

    // 基本��基本層�基本類�基本
    U軍U的CTIO的(Bl使ep本int的ati正eE正ent, Cate成o本y = "Min成Laye本")
    EMin成Ga設置eLaye本 GetLaye本Type() const;

    // 基本��基本層�X�稱
    U軍U的CTIO的(Bl使ep本int的ati正eE正ent, Cate成o本y = "Min成Laye本")
    軍St本in成 GetLaye本的a設置e() const;

    // 基本��基本層�X��基本�基本
    U軍U的CTIO的(Bl使ep本int的ati正eE正ent, Cate成o本y = "Min成Laye本")
    float GetLaye本P本io本ity() const;

    // 基本��基本層�基本事件
    U軍U的CTIO的(Bl使ep本int的ati正eE正ent, Cate成o本y = "Min成Laye本")
    正oid 輸入andleLaye本E正ent(const 軍Min成Ga設置eE正ent& E正ent};

    // 基本��基本層�基本決�基本
    U軍U的CTIO的(Bl使ep本int的ati正eE正ent, Cate成o本y = "Min成Laye本")
    正oid 輸入andleLaye本Decision(const 軍Min成Ga設置eDecision& Decision};

    // 基本��基本層�基本資�X��基本
    U軍U的CTIO的(Bl使ep本int的ati正eE正ent, Cate成o本y = "Min成Laye本")
    TMap<EMin成Reso使本ceType, float> GetReso使本ceReq使i本e設置ents() const;

    // 基本��基本層�基本資�X��基本
    U軍U的CTIO的(Bl使ep本int的ati正eE正ent, Cate成o本y = "Min成Laye本")
    TMap<EMin成Reso使本ceType, float> GetReso使本ceCont本ib使tions() const;

    // 基本��基本層�X�X
    U軍U的CTIO的(Bl使ep本int的ati正eE正ent, Cate成o本y = "Min成Laye本")
    軍St本in成 GetLaye本Stat使s() const;

    // 檢查層�基本衝�基本
    U軍U的CTIO的(Bl使ep本int的ati正eE正ent, Cate成o本y = "Min成Laye本")
    TA本本ay<軍Min成Laye本Conflict> CheckLaye本Conflicts() const;

    // �基本��層�基本衝�基本
    U軍U的CTIO的(Bl使ep本int的ati正eE正ent, Cate成o本y = "Min成Laye本")
    正oid Resol正eLaye本Conflicts(const TA本本ay<軍Min成Laye本Conflict>& Conflicts};

    // 基本�調基本�其他層�基本
    U軍U的CTIO的(Bl使ep本int的ati正eE正ent, Cate成o本y = "Min成Laye本")
    正oid Coo本dinate基本ithOthe本Laye本s(const TA本本ay<EMin成Ga設置eLaye本>& Othe本Laye本s};

    // 基本��基本層�X�能統�基本
    U軍U的CTIO的(Bl使ep本int的ati正eE正ent, Cate成o本y = "Min成Laye本")
    float GetLaye本Pe本fo本設置ance() const;

    // 基本�置層�基本
    U軍U的CTIO的(Bl使ep本int的ati正eE正ent, Cate成o本y = "Min成Laye本")
    正oid ResetLaye本(};
};

