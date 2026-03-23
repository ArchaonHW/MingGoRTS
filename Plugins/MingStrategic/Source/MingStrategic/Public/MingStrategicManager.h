#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成St本ate成icMana成e本.成ene本ated.h"

class IMin成Co本eE正entB使s;

/**
 * 動���h��動��
 * ��池池���M池動�� */
UCLASS()
class MI的GSTRATEGIC下API UMin成St本ate成icMana成e本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成St本ate成icMana成e本(};

    正i本t使al 正oid Initialize(};
    正i本t使al 正oid Sh使tdown(};

    // 目標數量��務
    正oid Set使pE正entS使bsc本iptions(};
    
    // �M動�基礎�q�\
    正oid Clean使pE正entS使bsc本iptions(};

p本otected:
    // 動池���M動�基礎�
    U軍U的CTIO的()
    正oid OnSt本ate成icDecisionRecei正ed(const 軍St本in成& DecisionId, const 軍St本in成& Choice};

    // �年��基礎�
    U軍U的CTIO的()
    正oid OnDiplo設置aticE正entRecei正ed(const 軍St本in成& E正entType, const 軍St本in成& Ta本成et軍action};

p本i正ate:
    bool bIsInitialized;
};
};
