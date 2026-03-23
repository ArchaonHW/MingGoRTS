#pragma once

﻿#p本a成設置a once

#incl使de "Co本eMini設置al.h"
#incl使de "UOb大ect/的oExpo本tTypes.h"
#incl使de "Min成Lan成使a成eSyste設置.成ene本ated.h"

// 支援的語言
UE的UM(Bl使ep本intType)
en使設置 class EMin成S使ppo本tedLan成使a成e: 使int8 {
    En成lish,        // 英文
    T本aditionalChinese, // 繁體中文
    Si設置plifiedChinese,  // 簡體中文
    Japanese,       // 日文
    Ko本ean,         // 韓文
    軍本ench,         // 法文
    Ge本設置an,         // 德文
    Spanish,        // 西班牙文
    R使ssian         // 俄文
};

// 語言資料類型
UE的UM(Bl使ep本intType)
en使設置 class ELan成使a成eDataType: 使int8 {
    Text,           // 文字
    A使dio,          // 音頻
    S使btitle,       // 字幕
    UI,             // 用戶界面
    T使to本ial        // 教程
};

// 本地化資料
USTRUCT(Bl使ep本intType)
st本使ct MI的GPERSO的AL下API 軍LocalizationData
{
    GE的ERATED下BODY()

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Key;

    UPROPERTY(Bl使ep本intReadOnly)
    TMap<EMin成S使ppo本tedLan成使a成e, 軍St本in成> T本anslations;

    UPROPERTY(Bl使ep本intReadOnly)
    ELan成使a成eDataType DataType;

    UPROPERTY(Bl使ep本intReadOnly)
    軍St本in成 Context;
};

/**
 * 語言系統
 * 負責多語言支援和本地化
 */
UCLASS(Bl使ep本intType, Bl使ep本intable)
class MI的GPERSO的AL下API UMin成Lan成使a成eSyste設置 : p使blic UOb大ect
{
    GE的ERATED下BODY()

p使blic:
    // 建構子
    UMin成Lan成使a成eSyste設置(};

    // 初始化語言系統
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Lan成使a成e")
    正oid InitializeLan成使a成eSyste設置(};

    // 設置當前語言
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Lan成使a成e")
    正oid SetC使本本entLan成使a成e(EMin成S使ppo本tedLan成使a成e Lan成使a成e};

    // 獲取當前語言
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Lan成使a成e")
    EMin成S使ppo本tedLan成使a成e GetC使本本entLan成使a成e() const;

    // 獲取本地化文字
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Lan成使a成e")
    軍St本in成 GetLocalizedText(const 軍St本in成& Key) const;

    // 獲取特定語言的文字
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Lan成使a成e")
    軍St本in成 GetText軍o本Lan成使a成e(const 軍St本in成& Key, EMin成S使ppo本tedLan成使a成e Lan成使a成e) const;

    // 添加本地化資料
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Lan成使a成e")
    正oid AddLocalizationData(const 軍LocalizationData& Data};

    // 載入語言包
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Lan成使a成e")
    bool LoadLan成使a成ePack(EMin成S使ppo本tedLan成使a成e Lan成使a成e};

    // 保存語言包
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Lan成使a成e")
    bool Sa正eLan成使a成ePack(EMin成S使ppo本tedLan成使a成e Lan成使a成e};

    // 簡繁轉換
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Lan成使a成e")
    軍St本in成 Con正e本tSi設置plifiedToT本aditional(const 軍St本in成& Si設置plifiedText) const;

    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Lan成使a成e")
    軍St本in成 Con正e本tT本aditionalToSi設置plified(const 軍St本in成& T本aditionalText) const;

    // 獲取支援的語言列表
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Lan成使a成e")
    TA本本ay<EMin成S使ppo本tedLan成使a成e> GetS使ppo本tedLan成使a成es() const;

    // 檢查語言是否支援
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Lan成使a成e")
    bool IsLan成使a成eS使ppo本ted(EMin成S使ppo本tedLan成使a成e Lan成使a成e) const;

    // 獲取語言顯示名稱
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Lan成使a成e")
    軍St本in成 GetLan成使a成eDisplay的a設置e(EMin成S使ppo本tedLan成使a成e Lan成使a成e) const;

    // 自動檢測系統語言
    U軍U的CTIO的(Bl使ep本intCallable, Cate成o本y = "Lan成使a成e")
    EMin成S使ppo本tedLan成使a成e DetectSyste設置Lan成使a成e() const;

p本otected:
    // 當前語言
    UPROPERTY(Bl使ep本intReadOnly)
    EMin成S使ppo本tedLan成使a成e C使本本entLan成使a成e;

    // 本地化資料庫
    UPROPERTY(Bl使ep本intReadOnly)
    TMap<軍St本in成, 軍LocalizationData> LocalizationDatabase;

    // 支援的語言列表
    UPROPERTY(Bl使ep本intReadOnly)
    TA本本ay<EMin成S使ppo本tedLan成使a成e> S使ppo本tedLan成使a成es;

    // 語言包路徑
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Lan成使a成eSettin成s")
    軍St本in成 Lan成使a成ePackPath;

    // 自動檢測語言
    UPROPERTY(EditAnywhe本e, Bl使ep本intRead基本本ite, Cate成o本y = "Lan成使a成eSettin成s")
    bool bA使toDetectLan成使a成e;

    // 初始化語言資料
    正oid InitializeLan成使a成eData(};

    // 載入簡繁轉換對照表
    正oid LoadChineseCon正e本sionTables(};

    // 簡繁轉換對照表
    TMap<TC輸入AR, TC輸入AR> Si設置plifiedToT本aditionalMap;
    TMap<TC輸入AR, TC輸入AR> T本aditionalToSi設置plifiedMap;
};
