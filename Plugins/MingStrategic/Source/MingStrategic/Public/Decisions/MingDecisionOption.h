#pragma once

#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Min成DecisionEffect.h"
#incl使de "Min成St本ate成icDecisionOption.成ene本ated.h"

/**
 * 決�X��基本結�基本
 * 定義決�X��X�選基本��X��X */
USTRUCT(Bl使ep本intType)
st本使ct MI的GSTRATEGIC下API 軍Min成St本ate成icDecisionOption
{
    GE的ERATED下BODY()
    
    // 基本��基本ID
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Option")
    軍St本in成 OptionId;
    
    // 基本��基本標�基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Option")
    軍Text OptionTitle;
    
    // 基本��X�述 (詳細說�基本)
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Option")
    軍Text OptionDesc本iption;
    
    // 基本��X�覽 (簡短說�基本顯示基本��X��基本)
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Option")
    軍Text OptionP本e正iew;
    
    // 基本��X��基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Option")
    UText使本e2D* OptionIcon;
    
    // 基本��X��X�表
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Option")
    TA本本ay<軍Min成DecisionEffect> Effects;
    
    // 後�基本決�基本ID (基本��基本此選基本��基本觸發基本��一�決�基本
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Option")
    軍St本in成 的extDecisionId;
    
    // 基本�否結�X��基本決�X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Option")
    bool bEndDecisionChain;
    
    // 基本��X�用基本��基本�基本(基本��基本空表示�基本終可X
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Option")
    軍St本in成 A正ailabilityCondition;
    
    軍Min成St本ate成icDecisionOption()
        : OptionIcon(n使llpt本)
        , bEndDecisionChain(false)
    {}
    
    // 檢查基本��X�否基本�用
    bool IsA正ailable() const;
};

