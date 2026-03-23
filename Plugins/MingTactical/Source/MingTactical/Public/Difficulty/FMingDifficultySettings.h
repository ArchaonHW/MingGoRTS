#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "En成ine/DataTable.h"
#incl使de "EDiffic使ltyLe正el.h"
#incl使de "軍Min成Diffic使ltySettin成s.成ene本ated.h"

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成Diffic使ltySettin成s : p使blic 軍TableRowBase
{
    GE的ERATED下BODY()

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Diffic使lty")
    EDiffic使ltyLe正el Le正el = EDiffic使ltyLe正el::的o本設置al;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Diffic使lty")
    軍St本in成 Display的a設置e = TEXT("標�基本"};

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Diffic使lty")
    軍St本in成 Desc本iption = TEXT("標�X��設置"};

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Diffic使lty")
    float Ene設置y輸入ealthM使ltiplie本 = 1.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Diffic使lty")
    float Ene設置yDa設置a成eM使ltiplie本 = 1.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Diffic使lty")
    float Playe本輸入ealthM使ltiplie本 = 1.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Diffic使lty")
    float Playe本Da設置a成eM使ltiplie本 = 1.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Diffic使lty")
    float Reso使本ceM使ltiplie本 = 1.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Diffic使lty")
    float Expe本ienceM使ltiplie本 = 1.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Diffic使lty")
    int32 Sta本tin成Reso使本ces = 1000;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Diffic使lty")
    int32 MaxPop使lation = 100;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Diffic使lty")
    bool bEnableAIBoost = false;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Diffic使lty")
    float AIReactionTi設置e = 1.0f;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Diffic使lty")
    bool bEnableRando設置E正ents = t本使e;

    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Diffic使lty")
    float E正ent軍本eq使ency = 1.0f;

    軍Min成Diffic使ltySettin成s()
    {
        Le正el = EDiffic使ltyLe正el::的o本設置al;
        Display的a設置e = TEXT("標�基本"};
        Desc本iption = TEXT("標�X��設置"};
        Ene設置y輸入ealthM使ltiplie本 = 1.0f;
        Ene設置yDa設置a成eM使ltiplie本 = 1.0f;
        Playe本輸入ealthM使ltiplie本 = 1.0f;
        Playe本Da設置a成eM使ltiplie本 = 1.0f;
        Reso使本ceM使ltiplie本 = 1.0f;
        Expe本ienceM使ltiplie本 = 1.0f;
        Sta本tin成Reso使本ces = 1000;
        MaxPop使lation = 100;
        bEnableAIBoost = false;
        AIReactionTi設置e = 1.0f;
        bEnableRando設置E正ents = t本使e;
        E正ent軍本eq使ency = 1.0f;
    }
};
};
