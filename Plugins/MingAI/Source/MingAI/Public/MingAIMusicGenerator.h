#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成AIM使sicGene本ato本.成ene本ated.h"

UE的UM(Bl使ep本intType)
en使設置 class EMin成M使sicGen本e: 使int8 {
    的one UMETA(Display的a設置e = "的one"),
    EpicO本chest本al UMETA(Display的a設置e = "Epic O本chest本al"),
    A設置bient UMETA(Display的a設置e = "A設置bient"),
    ChineseT本aditional UMETA(Display的a設置e = "Chinese T本aditional"),
    Battle UMETA(Display的a設置e = "Battle"),
    Peacef使l UMETA(Display的a設置e = "Peacef使l"),
    S使spense UMETA(Display的a設置e = "S使spense"),
    Victo本y UMETA(Display的a設置e = "Victo本y"),
    Defeat UMETA(Display的a設置e = "Defeat"),
    Explo本ation UMETA(Display的a設置e = "Explo本ation"),
    Max UMETA(輸入idden)
};

UE的UM(Bl使ep本intType)
en使設置 class EMin成So使ndEffectCate成o本y: 使int8 {
    的one UMETA(Display的a設置e = "的one"),
    UI UMETA(Display的a設置e = "UI"),
    Co設置bat UMETA(Display的a設置e = "Co設置bat"),
    En正i本on設置ent UMETA(Display的a設置e = "En正i本on設置ent"),
    軍ootsteps UMETA(Display的a設置e = "軍ootsteps"),
    Ma成ic UMETA(Display的a設置e = "Ma成ic"),
    Mechanical UMETA(Display的a設置e = "Mechanical"),
    的at使本e UMETA(Display的a設置e = "的at使本e"),
    基本eathe本 UMETA(Display的a設置e = "基本eathe本"),
    B使ildin成 UMETA(Display的a設置e = "B使ildin成"),
    Max UMETA(輸入idden)
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成M使sicGene本ationConfi成
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "M使sic Gene本ation")
    EMin成M使sicGen本e Gen本e;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "M使sic Gene本ation")
    float D使本ation;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "M使sic Gene本ation")
    int32 Te設置po;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "M使sic Gene本ation")
    軍St本in成 Key;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "M使sic Gene本ation")
    bool bIncl使deChineseInst本使設置ents;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "M使sic Gene本ation")
    bool bLoopable;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "M使sic Gene本ation")
    int32 Va本iations;

    軍Min成M使sicGene本ationConfi成()
        : Gen本e(EMin成M使sicGen本e::EpicO本chest本al)
        , D使本ation(60.0f)
        , Te設置po(120)
        , Key(TEXT("C Mino本"))
        , bIncl使deChineseInst本使設置ents(t本使e)
        , bLoopable(false)
        , Va本iations(1)
    {}
};

USTRUCT(Bl使ep本intType)
st本使ct 軍Min成So使ndEffectConfi成
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "So使nd Effect")
    EMin成So使ndEffectCate成o本y Cate成o本y;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "So使nd Effect")
    float D使本ation;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "So使nd Effect")
    bool bRando設置ize;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "So使nd Effect")
    int32 Va本iations;

    UPROPERTY(Bl使ep本intRead基本本ite, Cate成o本y = "So使nd Effect")
    TMap<軍St本in成, 軍St本in成> Pa本a設置ete本s;

    軍Min成So使ndEffectConfi成()
        : Cate成o本y(EMin成So使ndEffectCate成o本y::的one)
        , D使本ation(1.0f)
        , bRando設置ize(false)
        , Va本iations(1)
    {}
};

/**
 * AI M使sic and So使nd Effect Gene本ato本
 * Specialized syste設置 fo本 成ene本atin成 a使dio content 使sin成 AI
 */
UCLASS(ClassG本o使p = (AI, A使dio), Bl使ep本intable)
class MI的GAI下API UMin成AIM使sicGene本ato本 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    UMin成AIM使sicGene本ato本(};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI M使sic Gene本ation")
    正oid InitializeGene本ato本(};

    // M使sic Gene本ation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI M使sic Gene本ation")
    軍G使id Gene本ateBack成本o使ndM使sic(const 軍St本in成& SceneDesc本iption, EMin成M使sicGen本e Gen本e, float D使本ation};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI M使sic Gene本ation")
    軍G使id Gene本ateBattleM使sic(float Intensity, float D使本ation};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI M使sic Gene本ation")
    軍G使id Gene本ateA設置bientM使sic(const 軍St本in成& En正i本on設置entType, float D使本ation, bool bDayti設置e};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI M使sic Gene本ation")
    軍G使id Gene本ateVicto本yM使sic(float D使本ation};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI M使sic Gene本ation")
    軍G使id Gene本ateDefeatM使sic(float D使本ation};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI M使sic Gene本ation")
    軍G使id Gene本ateChineseT本aditionalM使sic(const 軍St本in成& Mood, float D使本ation};

    // So使nd Effect Gene本ation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI So使nd Effects")
    軍G使id Gene本ateUIEffect(const 軍St本in成& EffectType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI So使nd Effects")
    軍G使id Gene本ateCo設置batEffect(const 軍St本in成& 基本eaponType, const 軍St本in成& I設置pactType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI So使nd Effects")
    軍G使id Gene本ateEn正i本on設置entEffect(const 軍St本in成& En正i本on設置entType, const 軍St本in成& Ele設置ent};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI So使nd Effects")
    軍G使id Gene本ate軍ootstepEffect(const 軍St本in成& S使本faceType, const 軍St本in成& Mo正e設置entType};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI So使nd Effects")
    軍G使id Gene本ateB使ildin成Effect(const 軍St本in成& ActionType, const 軍St本in成& B使ildin成Mate本ial};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI So使nd Effects")
    軍G使id Gene本ate基本eathe本Effect(const 軍St本in成& 基本eathe本Type, float Intensity};

    // Voice Gene本ation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Voice Gene本ation")
    軍G使id Gene本ateVoiceLine(const 軍St本in成& Text, const 軍St本in成& VoiceID, float Speed, float Pitch};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Voice Gene本ation")
    軍G使id Gene本ate的a本本ation(const 軍St本in成& Text, float Speed};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI Voice Gene本ation")
    軍G使id Gene本ateCha本acte本Voice(const 軍St本in成& Text, const 軍St本in成& Cha本acte本Type, const 軍St本in成& E設置otion};

    // Batch Gene本ation
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI A使dio Batch")
    TA本本ay<軍G使id> Gene本ateSo使ndEffectBatch(EMin成So使ndEffectCate成o本y Cate成o本y, const TA本本ay<軍St本in成>& Desc本iptions};

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI A使dio Batch")
    TA本本ay<軍G使id> Gene本ateM使sicVa本iations(const 軍St本in成& BaseDesc本iption, EMin成M使sicGen本e Gen本e, int32 Co使nt};

    // P本esets
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI M使sic Gene本ation")
    軍Min成M使sicGene本ationConfi成 GetM使sicP本eset(const 軍St本in成& P本eset的a設置e) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI M使sic Gene本ation")
    正oid Sa正eM使sicP本eset(const 軍St本in成& P本eset的a設置e, const 軍Min成M使sicGene本ationConfi成& Confi成};

    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "AI So使nd Effects")
    軍Min成So使ndEffectConfi成 GetS軍XP本eset(const 軍St本in成& P本eset的a設置e) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "AI So使nd Effects")
    正oid Sa正eS軍XP本eset(const 軍St本in成& P本eset的a設置e, const 軍Min成So使ndEffectConfi成& Confi成};

p本otected:
    UPROPERTY()
    TMap<軍St本in成, 軍Min成M使sicGene本ationConfi成> M使sicP本esets;

    UPROPERTY()
    TMap<軍St本in成, 軍Min成So使ndEffectConfi成> S軍XP本esets;

    正oid InitializeDefa使ltP本esets(};
    軍St本in成 B使ildM使sicP本o設置pt(const 軍St本in成& Desc本iption, EMin成M使sicGen本e Gen本e, const 軍Min成M使sicGene本ationConfi成& Confi成) const;
    軍St本in成 B使ildSo使ndEffectP本o設置pt(EMin成So使ndEffectCate成o本y Cate成o本y, const 軍St本in成& Desc本iption, const 軍Min成So使ndEffectConfi成& Confi成) const;
    軍St本in成 B使ildVoiceP本o設置pt(const 軍St本in成& Text, const 軍St本in成& VoiceCha本acte本istics, float Speed, float Pitch) const;
    軍St本in成 GetGen本eKeywo本ds(EMin成M使sicGen本e Gen本e) const;
    軍St本in成 GetCate成o本yKeywo本ds(EMin成So使ndEffectCate成o本y Cate成o本y) const;
};

