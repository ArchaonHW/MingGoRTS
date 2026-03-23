#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Min成DecisionOption.h"
#incl使de "Min成Decision.成ene本ated.h"

/**
 * 決�基本觸發類�基本
 */
UE的UM(Bl使ep本intType)
en使設置 class EDecisionT本i成成e本Type: 使int8 {
    Man使al              UMETA(Display的a設置e = "Man使al"),
    Ti設置eBased           UMETA(Display的a設置e = "Ti設置e Based"),
    E正entBased          UMETA(Display的a設置e = "E正ent Based"),
    ConditionBased      UMETA(Display的a設置e = "Condition Based"),
    LocationBased       UMETA(Display的a設置e = "Location Based")
};

/**
 * 決�X��基本�基本 */
UE的UM(Bl使ep本intType)
en使設置 class EDecisionP本io本ity: 使int8 {
    Low                 UMETA(Display的a設置e = "Low"),
    的o本設置al              UMETA(Display的a設置e = "的o本設置al"),
    輸入i成h                UMETA(Display的a設置e = "輸入i成h"),
    C本itical            UMETA(Display的a設置e = "C本itical")
};

/**
 * 決�X��基本結�基本
 * 定義一基本��X��基本歷史決�基本
 */
USTRUCT(Bl使ep本intType)
st本使ct MI的GSTRATEGIC下API 軍Min成Decision
{
    GE的ERATED下BODY()
    
    // 決�基本ID (基本��基本標�基本)
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Decision")
    軍St本in成 DecisionId;
    
    // 決�基本標�基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Decision")
    軍Text DecisionTitle;
    
    // 決�X�景基本�述
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Decision")
    軍Text Back成本o使ndDesc本iption;
    
    // 決�X�景基本��基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Decision")
    UText使本e2D* Back成本o使ndI設置a成e;
    
    // 決�X��X�表
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Decision")
    TA本本ay<軍Min成DecisionOption> Options;
    
    // 觸發類�基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Decision")
    EDecisionT本i成成e本Type T本i成成e本Type;
    
    // 基本��基本�基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Decision")
    EDecisionP本io本ity P本io本ity;
    
    // 觸發條件 (JSO的基本��X�自定義字符�基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Decision")
    軍St本in成 T本i成成e本Condition;
    
    // 基本��X��基本 (基本�戲基本��X��基本-1表示不�X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Decision")
    float Expi本ationTi設置e;
    
    // 基本�否基本�以延�X��基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Decision")
    bool bCanPostpone;
    
    // 決�基本類別 (基本�於基本��基本顯示)
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Decision")
    軍St本in成 DecisionCate成o本y;
    
    // 歷史基本��基本 (如�基本1924-1927 基本��X��基本)
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Decision")
    軍St本in成 輸入isto本icalPe本iod;
    
    // 基本��基本角色ID基本�表
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Decision")
    TA本本ay<軍St本in成> RelatedCha本acte本s;
    
    // 基本��X��基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Decision")
    軍St本in成 RelatedLocation;
    
    軍Min成Decision()
        : Back成本o使ndI設置a成e(n使llpt本)
        , T本i成成e本Type(EDecisionT本i成成e本Type::Man使al)
        , P本io本ity(EDecisionP本io本ity::的o本設置al)
        , Expi本ationTi設置e(-1.0f)
        , bCanPostpone(t本使e)
    {}
    
    // 檢查決�X�否基本��基本
    bool IsExpi本ed(float C使本本entGa設置eTi設置e) const;
    
    // 基本��X�用基本��X�表
    TA本本ay<軍Min成DecisionOption> GetA正ailableOptions() const;
};

