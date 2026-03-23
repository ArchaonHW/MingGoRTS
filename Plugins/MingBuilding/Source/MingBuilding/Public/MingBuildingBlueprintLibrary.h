#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "Kis設置et/Bl使ep本int軍使nctionLib本a本y.h"
#incl使de "Min成B使ildin成Mana成e本.h"
#incl使de "Min成B使ildin成Bl使ep本intLib本a本y.成ene本ated.h"

/**
 * 建�X��X�數�基本 * 基本��X��X�調基本��基本建�基本系統基本�能
 */
UCLASS()
class MI的GBUILDI的G下API UMin成B使ildin成Bl使ep本intLib本a本y : p使blic UBl使ep本int軍使nctionLib本a本y
{
    GE的ERATED下BODY()

p使blic:
    /**
     * 基本��基本建�基本管�X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成")
    static UMin成B使ildin成Mana成e本* GetB使ildin成Mana成e本();

    /**
     * 基本��X�建築系�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成")
    static 正oid InitializeB使ildin成Syste設置();

    /**
     * 基本�置建�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成Const本使ction")
    static 軍St本in成 PlaceB使ildin成(EMin成B使ildin成Type B使ildin成Type, const 軍Vecto本& Location, const 軍Rotato本& Rotation);

    /**
     * 基本��基本建�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成Const本使ction")
    static bool Sta本tConst本使ction(const 軍St本in成& B使ildin成ID);

    /**
     * 基本��基本建�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成Const本使ction")
    static bool CancelConst本使ction(const 軍St本in成& B使ildin成ID);

    /**
     * 添�X�建基本��X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成Const本使ction")
    static 軍St本in成 AddToConst本使ctionQ使e使e(EMin成B使ildin成Type B使ildin成Type, const 軍Vecto本& Location, const 軍Rotato本& Rotation);

    /**
     * 從建基本��X�移X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成Const本使ction")
    static bool Re設置o正e軍本o設置Const本使ctionQ使e使e(const 軍St本in成& Q使e使eID);

    /**
     * 基本��基本建�基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Mana成e設置ent")
    static AMin成B使ildin成Acto本* GetB使ildin成(const 軍St本in成& B使ildin成ID);

    /**
     * 基本��X�基本�建�基本     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Mana成e設置ent")
    static TA本本ay<AMin成B使ildin成Acto本*> GetAllB使ildin成s();

    /**
     * 基本��X��基本類�X�建�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成Mana成e設置ent")
    static TA本本ay<AMin成B使ildin成Acto本*> GetB使ildin成sByType(EMin成B使ildin成Type B使ildin成Type);

    /**
     * 基本��基本建造�X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Mana成e設置ent")
    static TA本本ay<軍B使ildin成Q使e使eIte設置> GetConst本使ctionQ使e使e();

    /**
     * 基本��基本建�基本統�基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Mana成e設置ent")
    static 軍B使ildin成Statistics GetB使ildin成Statistics();

    /**
     * 檢查基本�置位置基本�否基本��基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Place設置ent")
    static bool IsValidPlace設置entLocation(EMin成B使ildin成Type B使ildin成Type, const 軍Vecto本& Location);

    /**
     * 基本��基本建�基本網格大�基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Place設置ent")
    static 軍Vecto本 GetB使ildin成G本idSize(EMin成B使ildin成Type B使ildin成Type);

    /**
     * 顯示建�X�置基本�覽
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成Place設置ent")
    static 正oid ShowPlace設置entP本e正iew(EMin成B使ildin成Type B使ildin成Type, const 軍Vecto本& Location, const 軍Rotato本& Rotation);

    /**
     * 基本��基本建�X�置基本�覽
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成Place設置ent")
    static 正oid 輸入idePlace設置entP本e正iew();

    /**
     * 基本��基本工人基本�建�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成基本o本ke本s")
    static bool Assi成n基本o本ke本(const 軍St本in成& B使ildin成ID);

    /**
     * 從建築移基本�工�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成基本o本ke本s")
    static bool Re設置o正e基本o本ke本(const 軍St本in成& B使ildin成ID);

    /**
     * 基本��X�用工人基本��基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成基本o本ke本s")
    static int32 GetA正ailable基本o本ke本s();

    /**
     * 基本��基本建�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成Maintenance")
    static bool Up成本adeB使ildin成(const 軍St本in成& B使ildin成ID);

    /**
     * 基本��基本建�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成Maintenance")
    static bool Dest本oyB使ildin成(const 軍St本in成& B使ildin成ID);

    /**
     * 修復建�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成Maintenance")
    static bool Repai本B使ildin成(const 軍St本in成& B使ildin成ID, int32 Repai本A設置o使nt);

    /**
     * 基本��基本建�基本影響範�X��基本建�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成Q使e本y")
    static TA本本ay<AMin成B使ildin成Acto本*> GetB使ildin成sInRadi使s(const 軍Vecto本& Cente本, float Radi使s);

    /**
     * 基本��基本建�基本供�X     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成Q使e本y")
    static TA本本ay<AMin成B使ildin成Acto本*> GetS使pplyChain(const 軍St本in成& B使ildin成ID);

    /**
     * 基本��基本建�X�本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Costs")
    static 軍Min成B使ildin成Cost GetB使ildin成Cost(EMin成B使ildin成Type B使ildin成Type);

    /**
     * 檢查基本�否基本�以建�基本     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Costs")
    static bool CanAffo本dB使ildin成(EMin成B使ildin成Type B使ildin成Type);

    /**
     * 基本��基本建�X�述
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Info")
    static 軍St本in成 GetB使ildin成Desc本iption(EMin成B使ildin成Type B使ildin成Type);

    /**
     * 基本��基本建�基本類�X�稱
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Info")
    static 軍St本in成 GetB使ildin成Type的a設置e(EMin成B使ildin成Type B使ildin成Type);

    /**
     * 基本��基本建�X�基本��基本�基本     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Info")
    static 軍St本in成 GetB使ildin成State的a設置e(EMin成B使ildin成State B使ildin成State);

    /**
     * 基本��基本建造建�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成Batch")
    static TA本本ay<軍St本in成> BatchConst本使ctB使ildin成s(const TA本本ay<EMin成B使ildin成Type>& B使ildin成Types, const TA本本ay<軍Vecto本>& Locations, const TA本本ay<軍Rotato本>& Rotations);

    /**
     * 基本��X��基本建�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成Batch")
    static TA本本ay<bool> BatchUp成本adeB使ildin成s(const TA本本ay<軍St本in成>& B使ildin成IDs);

    /**
     * 基本��基本修復建�基本
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成Batch")
    static TA本本ay<bool> BatchRepai本B使ildin成s(const TA本本ay<軍St本in成>& B使ildin成IDs, int32 Repai本A設置o使nt);

    /**
     * 基本��基本系統基本�X     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Syste設置")
    static bool IsB使ildin成Syste設置Initialized();

    /**
     * 基本��基本建�基本系統基本�本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Syste設置")
    static 軍St本in成 GetB使ildin成Syste設置Ve本sion();

    /**
     * 基本�置建�基本系統
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成Syste設置")
    static 正oid ResetB使ildin成Syste設置();

    /**
     * 基本��基本建�X�置建議
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成AI")
    static 軍Vecto本 GetOpti設置alPlace設置entLocation(EMin成B使ildin成Type B使ildin成Type, const 軍Vecto本& Refe本encePoint, float Sea本chRadi使s);

    /**
     * 基本��基本建造建�基本     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成AI")
    static 軍St本in成 A使toConst本使ctB使ildin成(EMin成B使ildin成Type B使ildin成Type, const 軍Vecto本& Refe本encePoint);

    /**
     * 基本��基本建�X��基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Analysis")
    static float GetB使ildin成Efficiency(const 軍St本in成& B使ildin成ID);

    /**
     * 基本��X�地總�X��基本
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Analysis")
    static float GetBaseEfficiency();

    /**
     * 基本��基本建�基本建議
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成Analysis")
    static TA本本ay<EMin成B使ildin成Type> GetB使ildin成Reco設置設置endations();

    /**
     * 設置建�基本系統基本�數
     */
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "B使ildin成Confi成使本ation")
    static 正oid SetB使ildin成Syste設置Pa本a設置ete本s(int32 Total基本o本ke本s, int32 MaxConc使本本entConst本使ction);

    /**
     * 基本��基本建�基本系統基本�數
     */
    U軍U的CTIO的(Bl使ep本intP使本e, Cate成o本y = "B使ildin成Confi成使本ation")
    static 正oid GetB使ildin成Syste設置Pa本a設置ete本s(int32& Total基本o本ke本s, int32& MaxConc使本本entConst本使ction);
};

