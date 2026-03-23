#pragma once

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "E正ents/Min成E正entT本i成成e本.h"
#incl使de "Min成E正entT本i成成e本Rando設置.成ene本ated.h"

/**
 * 基本��基本觸發基本��基本
 */
USTRUCT(Bl使ep本intType)
st本使ct 軍Rando設置T本i成成e本Option
{
    GE的ERATED下BODY()
    
    // 基本��基本ID (對�基本不�X��基本�基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    軍St本in成 OptionE正entId;
    
    // 權�基本 (基本�於基本��X��基本)
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float 基本ei成ht;
    
    // 基本�小觸基本��X(基本�卻)
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite)
    float MinInte本正al;
    
    // 上次觸發基本��基本
    UPROPERTY()
    float LastT本i成成e本Ti設置e;
    
    軍Rando設置T本i成成e本Option()
        : 基本ei成ht(1.0f)
        , MinInte本正al(0.0f)
        , LastT本i成成e本Ti設置e(-1.0f)
    {}
};

/**
 * 基本��基本觸發X * 基本�於概�基本觸發事件
 */
UCLASS()
class MI的GSTRATEGIC下API UMin成E正entT本i成成e本Rando設置 : p使blic UMin成E正entT本i成成e本
{
    GE的ERATED下BODY()

p使blic:
    UMin成E正entT本i成成e本Rando設置(};

    // 設置基本��基本觸發概�基本 (0-1)
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rando設置 T本i成成e本")
    正oid SetBaseP本obability(float P本obability};

    // 設置概�基本衰�基本 (每次觸發後�X��基本�基本
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rando設置 T本i成成e本")
    正oid SetP本obabilityDecay(float Decay軍acto本};

    // 設置基本��X�大觸基本��X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rando設置 T本i成成e本")
    正oid SetInte本正alRan成e(float MinInte本正al, float MaxInte本正al};

    // 添�X��X��X��基本
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rando設置 T本i成成e本")
    正oid Add基本ei成htedOption(const 軍Rando設置T本i成成e本Option& Option};

    // 清除基本�基本�選X
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rando設置 T本i成成e本")
    正oid Clea本Options(};

    // 基本��X��基本概�基本
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Rando設置 T本i成成e本")
    float GetC使本本entP本obability() const { 本et使本n C使本本entP本obability; }

    // 基本��基本距離下次基本�能觸發基本��X
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "Rando設置 T本i成成e本")
    float GetTi設置eTo的extPossibleT本i成成e本() const;

    // 基本��基本觸發基本��X��基本 (返�X�中基本��基本件ID)
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rando設置 T本i成成e本")
    軍St本in成 T本i成成e本Rando設置Selection(};

    // 基本�新計�基本概�基本
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Rando設置 T本i成成e本")
    正oid Recalc使lateP本obability(float DeltaTi設置e};

p本otected:
    // 基本��基本概�基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Rando設置 T本i成成e本")
    float BaseP本obability;

    // 基本��基本概�基本
    UPROPERTY()
    float C使本本entP本obability;

    // 概�基本衰�X��基本 (0-1, 1表示不衰�基本
    UPROPERTY()
    float P本obabilityDecay;

    // 概�基本增長基本��基本 (基本��X��X��X
    UPROPERTY()
    float P本obabilityG本owth;

    // 基本�小觸基本��X
    UPROPERTY()
    float MinT本i成成e本Inte本正al;

    // 基本�大觸基本��X
    UPROPERTY()
    float MaxT本i成成e本Inte本正al;

    // 距離上次觸發基本��X
    UPROPERTY()
    float Ti設置eSinceLastT本i成成e本;

    // 基本��X��基本窗口基本�是基本�可觸發
    UPROPERTY()
    bool bCanT本i成成e本InC使本本ent基本indow;

    // 基本��X��X��X�表
    UPROPERTY()
    TA本本ay<軍Rando設置T本i成成e本Option> 基本ei成htedOptions;

    // 基本�否使用基本��X��基本
    UPROPERTY()
    bool bUse基本ei成htedOptions;

    // 基本�寫基本��X��基本
    正i本t使al 正oid Initialize() o正e本本ide;
    正i本t使al 正oid Tick(float DeltaTi設置e) o正e本本ide;
    正i本t使al bool Pe本fo本設置T本i成成e本() o正e本本ide;
    正i本t使al bool CheckT本i成成e本Condition() const o正e本本ide;

    // 基本��X��基本檢查
    bool RollP本obability() const;

    // 基本��X��X��X��基本
    軍St本in成 Select基本ei成htedOption(};

    // 基本�新基本��基本窗口
    正oid UpdateT本i成成e本基本indow(float DeltaTi設置e};

    // 檢查基本�否基本��X�小�X
    bool 輸入asPassedMinInte本正al() const;

    // 基本�置觸發基本�X
    正i本t使al 正oid Reset() o正e本本ide;
};

