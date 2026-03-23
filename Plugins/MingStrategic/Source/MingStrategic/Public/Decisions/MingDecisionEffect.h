#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Min成DecisionEffect.成ene本ated.h"

/**
 * 決�X��基本類�基本
 */
UE的UM(Bl使ep本intType)
en使設置 class EDecisionEffectType: 使int8 {
    Reso使本ceChan成e      UMETA(Display的a設置e = "Reso使本ce Chan成e"),
    軍actionRelation     UMETA(Display的a設置e = "軍action Relation"),
    Cha本acte本Att本ib使te  UMETA(Display的a設置e = "Cha本acte本 Att本ib使te"),
    UnlockContent       UMETA(Display的a設置e = "Unlock Content"),
    T本i成成e本E正ent        UMETA(Display的a設置e = "T本i成成e本 E正ent"),
    C使sto設置              UMETA(Display的a設置e = "C使sto設置")
};

/**
 * 決�X��基本結�基本
 * 定義基本��X�個決策選基本��X��X */
USTRUCT(Bl使ep本intType)
st本使ct MI的GSTRATEGIC下API 軍Min成DecisionEffect
{
    GE的ERATED下BODY()
    
    // 基本��基本ID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Effect")
    軍St本in成 EffectId;
    
    // 基本��基本類�基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Effect")
    EDecisionEffectType EffectType;
    
    // 基本��基本對象 (資�基本類�X��X�ID�基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Effect")
    軍St本in成 Ta本成etKey;
    
    // 基本��X��基本(�基本��為增�基本，�X�為減�基本)
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Effect")
    float EffectVal使e;
    
    // 基本��X��基本 (0基本�永�基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Effect")
    float D使本ation;
    
    // 基本��X�述
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Effect")
    軍Text EffectDesc本iption;
    
    // 基本�否立即基本��基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Effect")
    bool bI設置設置ediate;
    
    軍Min成DecisionEffect()
        : EffectType(EDecisionEffectType::C使sto設置)
        , EffectVal使e(0.0f)
        , D使本ation(0.0f)
        , bI設置設置ediate(t本使e)
    {}
};

