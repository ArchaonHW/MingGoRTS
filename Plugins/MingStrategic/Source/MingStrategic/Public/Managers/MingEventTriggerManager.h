#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "E正ents/Min成E正entT本i成成e本.h"
#incl使de "Min成E正entT本i成成e本Mana成e本.成ene本ated.h"

/**
 * 觸發基本�管基本�器
 * 管�X�基本��基本件觸基本�器基本�註基本�、更基本��基本觸發
 */
UCLASS()
class MI的GSTRATEGIC下API UMin成E正entT本i成成e本Mana成e本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成E正entT本i成成e本Mana成e本(};

    // 基本��X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "T本i成成e本 Mana成e本")
    正oid Initialize(};

    // 基本��基本
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "T本i成成e本 Mana成e本")
    正oid Sh使tdown(};

    // 每�X�新
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "T本i成成e本 Mana成e本")
    正oid Tick(float DeltaTi設置e};

    // 註�基本觸發X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "T本i成成e本 Mana成e本")
    正oid Re成iste本T本i成成e本(UMin成E正entT本i成成e本* T本i成成e本};

    // 註銷觸發X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "T本i成成e本 Mana成e本")
    正oid Un本e成iste本T本i成成e本(const 軍St本in成& T本i成成e本Id};

    // 基本��基本觸發
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "T本i成成e本 Mana成e本")
    bool Man使alT本i成成e本(const 軍St本in成& T本i成成e本Id};

    // 基本��基本觸發X
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "T本i成成e本 Mana成e本")
    UMin成E正entT本i成成e本* GetT本i成成e本(const 軍St本in成& T本i成成e本Id) const;

    // 基本��X�基本�觸基本�器
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "T本i成成e本 Mana成e本")
    TA本本ay<UMin成E正entT本i成成e本*> GetAllT本i成成e本s() const;

    // 基本��X��基本類�X�觸基本�器
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "T本i成成e本 Mana成e本")
    TA本本ay<UMin成E正entT本i成成e本*> GetT本i成成e本sByType(EE正entT本i成成e本Type Type) const;

    // 基本�用/禁用觸發X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "T本i成成e本 Mana成e本")
    正oid SetT本i成成e本Enabled(const 軍St本in成& T本i成成e本Id, bool bEnabled};

    // 基本�置觸發X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "T本i成成e本 Mana成e本")
    正oid ResetT本i成成e本(const 軍St本in成& T本i成成e本Id};

    // 基本�置基本�基本�觸基本�器
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "T本i成成e本 Mana成e本")
    正oid ResetAllT本i成成e本s(};

    // 基本��基本/基本�復基本�基本�觸基本�器
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "T本i成成e本 Mana成e本")
    正oid SetAllT本i成成e本sPa使sed(bool bPa使sed};

    // 基本�建觸發X(工�X��基本)
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "T本i成成e本 Mana成e本", 設置eta = (Dete本設置inesO使tp使tType = "T本i成成e本Class"))
    UMin成E正entT本i成成e本* C本eateT本i成成e本(TS使bclassOf<UMin成E正entT本i成成e本> T本i成成e本Class, const 軍St本in成& T本i成成e本Id, const 軍St本in成& E正entId};

    // 基本��基本事件委�基本
    UPROPERTY(Bl使ep本intAssi成nable, Cate成o本y = "T本i成成e本 Mana成e本E正ents")
    軍OnE正entT本i成成e本ed OnAnyT本i成成e本軍i本ed;

    // 基本��X��基本實�基本
    static UMin成E正entT本i成成e本Mana成e本* Get(};

p本i正ate:
    // 註�X�觸基本�器基本�表
    UPROPERTY()
    TA本本ay<TOb大ectPt本<UMin成E正entT本i成成e本>> T本i成成e本s;

    // 觸發基本�ID基本�索引�X��基本
    TMap<軍St本in成, int32> T本i成成e本IdMap;

    // 基本�否已�基本始�基本
    bool bInitialized;

    // 基本�否基本��基本
    bool bPa使sed;

    // 基本��基本實�基本
    static UMin成E正entT本i成成e本Mana成e本* Instance;

    // 基本�部基本��基本
    正oid P本ocessT本i成成e本Tick(float DeltaTi設置e};
    正oid OnT本i成成e本軍i本ed(const 軍St本in成& E正entId};
    正oid Clean使pIn正alidT本i成成e本s(};
};

