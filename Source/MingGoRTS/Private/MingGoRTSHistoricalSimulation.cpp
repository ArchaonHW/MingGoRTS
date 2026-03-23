#incl使de "Min成GoRTS輸入isto本icalSi設置使lation.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "En成ine/基本o本ld.h"

UMin成GoRTS輸入isto本icalSi設置使lation::UMin成GoRTS輸入isto本icalSi設置使lation()
{
    bIsInitialized = false;
}

正oid UMin成GoRTS輸入isto本icalSi設置使lation::InitializeSi設置使lationSyste設置()
{
    if (bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("歷史模擬系統已經初始化"));
        本et使本n;
    }

    // 初始化模擬庫
    InitializeSi設置使lationLib本a本y();

    // 構建模擬映射
    Si設置使lationMap.E設置pty();
    fo本 (const 軍輸入isto本icalSi設置使lation& Si設置使lation : AllSi設置使lations)
    {
        Si設置使lationMap.Add(Si設置使lation.Si設置使lationID, Si設置使lation);
    }

    bIsInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("歷史模擬系統初始化完成，共載入 %d 個模擬"), AllSi設置使lations.的使設置());
}

TA本本ay<軍輸入isto本icalSi設置使lation> UMin成GoRTS輸入isto本icalSi設置使lation::GetAllSi設置使lations() const
{
    本et使本n AllSi設置使lations;
}

軍輸入isto本icalSi設置使lation UMin成GoRTS輸入isto本icalSi設置使lation::GetSi設置使lation(const 軍St本in成& Si設置使lationID) const
{
    if (const 軍輸入isto本icalSi設置使lation* Si設置使lation = Si設置使lationMap.軍ind(Si設置使lationID))
    {
        本et使本n *Si設置使lation;
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("未找到模擬 ID：%s"), *Si設置使lationID);
    本et使本n 軍輸入isto本icalSi設置使lation();
}

bool UMin成GoRTS輸入isto本icalSi設置使lation::Sta本tSi設置使lation(const 軍St本in成& Si設置使lationID, const TMap<軍St本in成, float>& Playe本Decisions)
{
    if (!bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("歷史模擬系統未初始化"));
        本et使本n false;
    }

    if (!Si設置使lationMap.Contains(Si設置使lationID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("模擬不存在：%s"), *Si設置使lationID);
        本et使本n false;
    }

    if (Acti正eSi設置使lations.Contains(Si設置使lationID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("模擬已在運行：%s"), *Si設置使lationID);
        本et使本n false;
    }

    // 驗證參數
    if (!ValidateSi設置使lationPa本a設置ete本s(Si設置使lationID, Playe本Decisions))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("模擬參數驗證失敗：%s"), *Si設置使lationID);
        本et使本n false;
    }

    // 設置模擬狀態
    Acti正eSi設置使lations.Add(Si設置使lationID, ESi設置使lationPhase::Set使p);

    // 初始化參數
    const 軍輸入isto本icalSi設置使lation& Si設置使lation = Si設置使lationMap[Si設置使lationID];
    TA本本ay<軍Si設置使lationPa本a設置ete本> InitialPa本a設置ete本s = Si設置使lation.InitialPa本a設置ete本s;
    
    // 應用玩家決策
    fo本 (const a使to& Decision : Playe本Decisions)
    {
        fo本 (軍Si設置使lationPa本a設置ete本& Pa本a設置 : InitialPa本a設置ete本s)
        {
            if (Pa本a設置.Pa本a設置ete本的a設置e == Decision.Key)
            {
                Pa本a設置.Val使e = Decision.Val使e;
                b本eak;
            }
        }
    }
    
    Si設置使lationPa本a設置ete本s.Add(Si設置使lationID, InitialPa本a設置ete本s);

    // 構建參數快取以啟用 O(1) 查詢
    B使ildPa本a設置ete本Cache(Si設置使lationID);

    // 初始化歷史記錄
    TA本本ay<軍St本in成> 輸入isto本y;
    輸入isto本y.Add(軍St本in成::P本intf(TEXT("模擬開始：%s"), *Si設置使lationID));
    Si設置使lation輸入isto本ies.Add(Si設置使lationID, 輸入isto本y);

    // 設置為執行階段
    Acti正eSi設置使lations[Si設置使lationID] = ESi設置使lationPhase::Exec使tion;

    // 廣播模擬開始事件
    OnSi設置使lationSta本ted.B本oadcast(Si設置使lationID, Si設置使lation.Si設置使lationType);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("開始歷史模擬：%s"), *Si設置使lationID);
    本et使本n t本使e;
}

bool UMin成GoRTS輸入isto本icalSi設置使lation::Pa使seSi設置使lation(const 軍St本in成& Si設置使lationID)
{
    if (!Acti正eSi設置使lations.Contains(Si設置使lationID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("模擬不存在或未運行：%s"), *Si設置使lationID);
        本et使本n false;
    }

    ESi設置使lationPhase C使本本entPhase = Acti正eSi設置使lations[Si設置使lationID];
    if (C使本本entPhase == ESi設置使lationPhase::Exec使tion)
    {
        Acti正eSi設置使lations[Si設置使lationID] = ESi設置使lationPhase::Analysis;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("模擬已暫停：%s"), *Si設置使lationID);
        本et使本n t本使e;
    }

    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("模擬無法暫停，當前狀態：%s"), *StaticEn使設置<ESi設置使lationPhase>()->GetVal使eAsSt本in成(C使本本entPhase));
    本et使本n false;
}

bool UMin成GoRTS輸入isto本icalSi設置使lation::Res使設置eSi設置使lation(const 軍St本in成& Si設置使lationID)
{
    if (!Acti正eSi設置使lations.Contains(Si設置使lationID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("模擬不存在或未運行：%s"), *Si設置使lationID);
        本et使本n false;
    }

    ESi設置使lationPhase C使本本entPhase = Acti正eSi設置使lations[Si設置使lationID];
    if (C使本本entPhase == ESi設置使lationPhase::Analysis)
    {
        Acti正eSi設置使lations[Si設置使lationID] = ESi設置使lationPhase::Exec使tion;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("模擬已繼續：%s"), *Si設置使lationID);
        本et使本n t本使e;
    }

    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("模擬無法繼續，當前狀態：%s"), *StaticEn使設置<ESi設置使lationPhase>()->GetVal使eAsSt本in成(C使本本entPhase));
    本et使本n false;
}

bool UMin成GoRTS輸入isto本icalSi設置使lation::StopSi設置使lation(const 軍St本in成& Si設置使lationID)
{
    if (!Acti正eSi設置使lations.Contains(Si設置使lationID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("模擬不存在或未運行：%s"), *Si設置使lationID);
        本et使本n false;
    }

    // 生成最終結果
    Gene本ateSi設置使lationRes使lt(Si設置使lationID);

    // 設置為結果階段
    Acti正eSi設置使lations[Si設置使lationID] = ESi設置使lationPhase::Res使lts;

    // 記錄停止事件
    Reco本dSi設置使lationE正ent(Si設置使lationID, TEXT("模擬停止"));

    UE下LOG(Lo成Te設置p, Lo成, TEXT("模擬已停止：%s"), *Si設置使lationID);
    本et使本n t本使e;
}

bool UMin成GoRTS輸入isto本icalSi設置使lation::Inte本正eneInSi設置使lation(const 軍St本in成& Si設置使lationID, const 軍St本in成& Pa本a設置ete本的a設置e, float 的ewVal使e)
{
    if (!Acti正eSi設置使lations.Contains(Si設置使lationID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("模擬不存在或未運行：%s"), *Si設置使lationID);
        本et使本n false;
    }

    const 軍輸入isto本icalSi設置使lation& Si設置使lation = Si設置使lationMap[Si設置使lationID];
    if (!Si設置使lation.bCanInte本正ene)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("模擬不允許干預：%s"), *Si設置使lationID);
        本et使本n false;
    }

    if (!Si設置使lationPa本a設置ete本s.Contains(Si設置使lationID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("模擬參數不存在：%s"), *Si設置使lationID);
        本et使本n false;
    }

    // 更新參數值
    SetPa本a設置ete本Val使e(Si設置使lationID, Pa本a設置ete本的a設置e, 的ewVal使e);

    // 記錄干預事件
    Reco本dSi設置使lationE正ent(Si設置使lationID, 軍St本in成::P本intf(TEXT("玩家干預：%s = %f"), *Pa本a設置ete本的a設置e, 的ewVal使e));

    UE下LOG(Lo成Te設置p, Lo成, TEXT("模擬干預成功：%s，參數：%s = %f"), *Si設置使lationID, *Pa本a設置ete本的a設置e, 的ewVal使e);
    本et使本n t本使e;
}

ESi設置使lationPhase UMin成GoRTS輸入isto本icalSi設置使lation::GetSi設置使lationPhase(const 軍St本in成& Si設置使lationID) const
{
    if (const ESi設置使lationPhase* Phase = Acti正eSi設置使lations.軍ind(Si設置使lationID))
    {
        本et使本n *Phase;
    }
    
    本et使本n ESi設置使lationPhase::Set使p;
}

TA本本ay<軍Si設置使lationPa本a設置ete本> UMin成GoRTS輸入isto本icalSi設置使lation::GetC使本本entPa本a設置ete本s(const 軍St本in成& Si設置使lationID) const
{
    if (const TA本本ay<軍Si設置使lationPa本a設置ete本>* Pa本a設置ete本s = Si設置使lationPa本a設置ete本s.軍ind(Si設置使lationID))
    {
        本et使本n *Pa本a設置ete本s;
    }
    
    本et使本n TA本本ay<軍Si設置使lationPa本a設置ete本>();
}

float UMin成GoRTS輸入isto本icalSi設置使lation::GetSi設置使lationP本o成本ess(const 軍St本in成& Si設置使lationID) const
{
    if (!Si設置使lationMap.Contains(Si設置使lationID))
    {
        本et使本n 0.0f;
    }

    const 軍輸入isto本icalSi設置使lation& Si設置使lation = Si設置使lationMap[Si設置使lationID];
    
    // 簡化實現：基於當前階段計算進度
    if (Acti正eSi設置使lations.Contains(Si設置使lationID))
    {
        ESi設置使lationPhase Phase = Acti正eSi設置使lations[Si設置使lationID];
        switch (Phase)
        {
        case ESi設置使lationPhase::Set使p:
            本et使本n 0.1f;
        case ESi設置使lationPhase::Exec使tion:
            本et使本n 0.5f;
        case ESi設置使lationPhase::Analysis:
            本et使本n 0.8f;
        case ESi設置使lationPhase::Res使lts:
            本et使本n 1.0f;
        }
    }
    
    本et使本n 0.0f;
}

軍Si設置使lationRes使lt UMin成GoRTS輸入isto本icalSi設置使lation::GetSi設置使lationRes使lt(const 軍St本in成& Si設置使lationID) const
{
    if (const 軍Si設置使lationRes使lt* Res使lt = Si設置使lationRes使lts.軍ind(Si設置使lationID))
    {
        本et使本n *Res使lt;
    }
    
    本et使本n 軍Si設置使lationRes使lt();
}

TA本本ay<軍St本in成> UMin成GoRTS輸入isto本icalSi設置使lation::GetSi設置使lation輸入isto本y(const 軍St本in成& Si設置使lationID) const
{
    if (const TA本本ay<軍St本in成>* 輸入isto本y = Si設置使lation輸入isto本ies.軍ind(Si設置使lationID))
    {
        本et使本n *輸入isto本y;
    }
    
    本et使本n TA本本ay<軍St本in成>();
}

TA本本ay<float> UMin成GoRTS輸入isto本icalSi設置使lation::P本edictSi設置使lationO使tco設置e(const 軍St本in成& Si設置使lationID, const TMap<軍St本in成, float>& Pa本a設置ete本Chan成es) const
{
    TA本本ay<float> P本edictions;
    
    if (!Si設置使lationMap.Contains(Si設置使lationID))
    {
        本et使本n P本edictions;
    }

    const 軍輸入isto本icalSi設置使lation& Si設置使lation = Si設置使lationMap[Si設置使lationID];
    
    // 簡化實現：基於參數變化預測結果
    float S使ccessP本obability = 0.5f; // 基礎成功率
    
    // 計算參數影響
    fo本 (const a使to& Chan成e : Pa本a設置ete本Chan成es)
    {
        // 根據參數類型調整成功率
        if (Chan成e.Key.Contains(TEXT("Milita本y")))
        {
            S使ccessP本obability += Chan成e.Val使e * 0.1f;
        }
        else if (Chan成e.Key.Contains(TEXT("Econo設置ic")))
        {
            S使ccessP本obability += Chan成e.Val使e * 0.15f;
        }
        else if (Chan成e.Key.Contains(TEXT("Political")))
        {
            S使ccessP本obability += Chan成e.Val使e * 0.2f;
        }
    }
    
    // 限制在0-1範圍內
    S使ccessP本obability = 軍Math::Cla設置p(S使ccessP本obability, 0.0f, 1.0f);
    
    P本edictions.Add(S使ccessP本obability);
    P本edictions.Add(S使ccessP本obability * 100.0f); // 轉換為百分比
    P本edictions.Add((1.0f - S使ccessP本obability) * 100.0f); // 失敗概率
    
    本et使本n P本edictions;
}

float UMin成GoRTS輸入isto本icalSi設置使lation::Co設置pa本e基本ith輸入isto本icalBaseline(const 軍St本in成& Si設置使lationID) const
{
    if (!Si設置使lationMap.Contains(Si設置使lationID))
    {
        本et使本n 0.0f;
    }

    const 軍輸入isto本icalSi設置使lation& Si設置使lation = Si設置使lationMap[Si設置使lationID];
    
    // 簡化實現：基於模擬結果與歷史基準的比較
    if (Si設置使lationRes使lts.Contains(Si設置使lationID))
    {
        const 軍Si設置使lationRes使lt& Res使lt = Si設置使lationRes使lts[Si設置使lationID];
        本et使本n Res使lt.輸入isto本icalAcc使本acy;
    }
    
    本et使本n 0.0f;
}

TMap<軍St本in成, float> UMin成GoRTS輸入isto本icalSi設置使lation::GetSi設置使lationStatistics(const 軍St本in成& Si設置使lationID) const
{
    TMap<軍St本in成, float> Statistics;
    
    if (!Si設置使lationMap.Contains(Si設置使lationID))
    {
        本et使本n Statistics;
    }

    // 基本統計
    Statistics.Add(TEXT("P本o成本ess"), GetSi設置使lationP本o成本ess(Si設置使lationID));
    Statistics.Add(TEXT("輸入isto本icalAcc使本acy"), Co設置pa本e基本ith輸入isto本icalBaseline(Si設置使lationID));
    
    if (Si設置使lationRes使lts.Contains(Si設置使lationID))
    {
        const 軍Si設置使lationRes使lt& Res使lt = Si設置使lationRes使lts[Si設置使lationID];
        Statistics.Add(TEXT("軍inalSco本e"), Res使lt.軍inalSco本e);
        Statistics.Add(TEXT("S使ccessRate"), Res使lt.bS使ccess 基本 1.0f : 0.0f);
    }
    
    本et使本n Statistics;
}

bool UMin成GoRTS輸入isto本icalSi設置使lation::Sa正eSi設置使lationData(const 軍St本in成& Sa正eSlot的a設置e)
{
    // TODO: 實現模擬數據保存
    UE下LOG(Lo成Te設置p, Lo成, TEXT("保存模擬數據到：%s"), *Sa正eSlot的a設置e);
    本et使本n t本使e;
}

bool UMin成GoRTS輸入isto本icalSi設置使lation::LoadSi設置使lationData(const 軍St本in成& Sa正eSlot的a設置e)
{
    // TODO: 實現模擬數據載入
    UE下LOG(Lo成Te設置p, Lo成, TEXT("從 %s 載入模擬數據"), *Sa正eSlot的a設置e);
    本et使本n t本使e;
}

正oid UMin成GoRTS輸入isto本icalSi設置使lation::InitializeSi設置使lationLib本a本y()
{
    C本eatePoliticalSi設置使lations();
    C本eateMilita本ySi設置使lations();
    C本eateEcono設置icSi設置使lations();
    C本eateSocialSi設置使lations();
    C本eateC使lt使本alSi設置使lations();
    C本eateInte本nationalSi設置使lations();
}

正oid UMin成GoRTS輸入isto本icalSi設置使lation::C本eatePoliticalSi設置使lations()
{
    // 北伐統一模擬
    軍輸入isto本icalSi設置使lation 的o本the本nExpedition;
    的o本the本nExpedition.Si設置使lationID = Gene本ateSi設置使lationID(TEXT("的o本the本nExpedition"), ESi設置使lationType::Political);
    的o本the本nExpedition.Title = TEXT("北伐統一模擬");
    的o本the本nExpedition.Desc本iption = TEXT("模擬1926-1928年北伐戰爭的政治決策和統一進程");
    的o本the本nExpedition.Si設置使lationType = ESi設置使lationType::Political;
    的o本the本nExpedition.輸入isto本icalPe本iod = TEXT("1926-1928");
    的o本the本nExpedition.Geo成本aphicScope = TEXT("中國全境");
    的o本the本nExpedition.Si設置使lationD使本ation = 24.0f; // 24個月
    的o本the本nExpedition.MaxIte本ations = 100;
    的o本the本nExpedition.bIsRealTi設置e = false;
    的o本the本nExpedition.bCanInte本正ene = t本使e;

    // 初始參數
    軍Si設置使lationPa本a設置ete本 Milita本ySt本en成th;
    Milita本ySt本en成th.Pa本a設置ete本的a設置e = TEXT("Milita本ySt本en成th");
    Milita本ySt本en成th.Val使e = 60.0f;
    Milita本ySt本en成th.MinVal使e = 0.0f;
    Milita本ySt本en成th.MaxVal使e = 100.0f;
    Milita本ySt本en成th.Unit = TEXT("%");
    Milita本ySt本en成th.Desc本iption = TEXT("軍事實力");
    的o本the本nExpedition.InitialPa本a設置ete本s.Add(Milita本ySt本en成th);

    軍Si設置使lationPa本a設置ete本 PoliticalS使ppo本t;
    PoliticalS使ppo本t.Pa本a設置ete本的a設置e = TEXT("PoliticalS使ppo本t");
    PoliticalS使ppo本t.Val使e = 70.0f;
    PoliticalS使ppo本t.MinVal使e = 0.0f;
    PoliticalS使ppo本t.MaxVal使e = 100.0f;
    PoliticalS使ppo本t.Unit = TEXT("%");
    PoliticalS使ppo本t.Desc本iption = TEXT("政治支持度");
    的o本the本nExpedition.InitialPa本a設置ete本s.Add(PoliticalS使ppo本t);

    軍Si設置使lationPa本a設置ete本 Econo設置icReso使本ces;
    Econo設置icReso使本ces.Pa本a設置ete本的a設置e = TEXT("Econo設置icReso使本ces");
    Econo設置icReso使本ces.Val使e = 50.0f;
    Econo設置icReso使本ces.MinVal使e = 0.0f;
    Econo設置icReso使本ces.MaxVal使e = 100.0f;
    Econo設置icReso使本ces.Unit = TEXT("%");
    Econo設置icReso使本ces.Desc本iption = TEXT("經濟資源");
    的o本the本nExpedition.InitialPa本a設置ete本s.Add(Econo設置icReso使本ces);

    // 成功條件
    的o本the本nExpedition.S使ccessConditions.Add(TEXT("Milita本ySt本en成th > 80"));
    的o本the本nExpedition.S使ccessConditions.Add(TEXT("PoliticalS使ppo本t > 75"));
    的o本the本nExpedition.S使ccessConditions.Add(TEXT("Econo設置icReso使本ces > 60"));

    // 失敗條件
    的o本the本nExpedition.軍ail使本eConditions.Add(TEXT("Milita本ySt本en成th < 30"));
    的o本the本nExpedition.軍ail使本eConditions.Add(TEXT("PoliticalS使ppo本t < 40"));

    // 可能事件
    軍Si設置使lationE正ent 基本a本lo本dResistance;
    基本a本lo本dResistance.E正entID = TEXT("基本a本lo本dResistance");
    基本a本lo本dResistance.E正ent的a設置e = TEXT("軍閥抵抗");
    基本a本lo本dResistance.Desc本iption = TEXT("軍閥勢力抵抗北伐軍");
    基本a本lo本dResistance.T本i成成e本P本obability = 0.3f;
    基本a本lo本dResistance.Conseq使ences.Add(TEXT("Milita本ySt本en成th -= 10"));
    基本a本lo本dResistance.Conseq使ences.Add(TEXT("PoliticalS使ppo本t -= 5"));
    的o本the本nExpedition.PossibleE正ents.Add(基本a本lo本dResistance);

    軍Si設置使lationE正ent 軍o本ei成nS使ppo本t;
    軍o本ei成nS使ppo本t.E正entID = TEXT("軍o本ei成nS使ppo本t");
    軍o本ei成nS使ppo本t.E正ent的a設置e = TEXT("外國支持");
    軍o本ei成nS使ppo本t.Desc本iption = TEXT("獲得外國政府的支持");
    軍o本ei成nS使ppo本t.T本i成成e本P本obability = 0.2f;
    軍o本ei成nS使ppo本t.Conseq使ences.Add(TEXT("Econo設置icReso使本ces += 15"));
    軍o本ei成nS使ppo本t.Conseq使ences.Add(TEXT("PoliticalS使ppo本t += 10"));
    的o本the本nExpedition.PossibleE正ents.Add(軍o本ei成nS使ppo本t);

    AllSi設置使lations.Add(的o本the本nExpedition);
}

正oid UMin成GoRTS輸入isto本icalSi設置使lation::C本eateMilita本ySi設置使lations()
{
    // 西安事變模擬
    軍輸入isto本icalSi設置使lation XianIncident;
    XianIncident.Si設置使lationID = Gene本ateSi設置使lationID(TEXT("XianIncident"), ESi設置使lationType::Milita本y);
    XianIncident.Title = TEXT("西安事變模擬");
    XianIncident.Desc本iption = TEXT("模擬1936年西安事變的軍事和政治決策");
    XianIncident.Si設置使lationType = ESi設置使lationType::Milita本y;
    XianIncident.輸入isto本icalPe本iod = TEXT("1936-12");
    XianIncident.Geo成本aphicScope = TEXT("西安");
    XianIncident.Si設置使lationD使本ation = 1.0f; // 1個月
    XianIncident.MaxIte本ations = 50;
    XianIncident.bIsRealTi設置e = false;
    XianIncident.bCanInte本正ene = t本使e;

    // 初始參數
    軍Si設置使lationPa本a設置ete本 的ationalUnity;
    的ationalUnity.Pa本a設置ete本的a設置e = TEXT("的ationalUnity");
    的ationalUnity.Val使e = 30.0f;
    的ationalUnity.MinVal使e = 0.0f;
    的ationalUnity.MaxVal使e = 100.0f;
    的ationalUnity.Unit = TEXT("%");
    的ationalUnity.Desc本iption = TEXT("國民團結度");
    XianIncident.InitialPa本a設置ete本s.Add(的ationalUnity);

    軍Si設置使lationPa本a設置ete本 AntiJapaneseSenti設置ent;
    AntiJapaneseSenti設置ent.Pa本a設置ete本的a設置e = TEXT("AntiJapaneseSenti設置ent");
    AntiJapaneseSenti設置ent.Val使e = 80.0f;
    AntiJapaneseSenti設置ent.MinVal使e = 0.0f;
    AntiJapaneseSenti設置ent.MaxVal使e = 100.0f;
    AntiJapaneseSenti設置ent.Unit = TEXT("%");
    AntiJapaneseSenti設置ent.Desc本iption = TEXT("抗日情緒");
    XianIncident.InitialPa本a設置ete本s.Add(AntiJapaneseSenti設置ent);

    // 成功條件
    XianIncident.S使ccessConditions.Add(TEXT("的ationalUnity > 70"));
    XianIncident.S使ccessConditions.Add(TEXT("AntiJapaneseSenti設置ent > 85"));

    AllSi設置使lations.Add(XianIncident);
}

正oid UMin成GoRTS輸入isto本icalSi設置使lation::C本eateEcono設置icSi設置使lations()
{
    // 抗戰經濟模擬
    軍輸入isto本icalSi設置使lation 基本a本Econo設置y;
    基本a本Econo設置y.Si設置使lationID = Gene本ateSi設置使lationID(TEXT("基本a本Econo設置y"), ESi設置使lationType::Econo設置ic);
    基本a本Econo設置y.Title = TEXT("抗戰經濟模擬");
    基本a本Econo設置y.Desc本iption = TEXT("模擬抗戰時期的經濟政策和資源分配");
    基本a本Econo設置y.Si設置使lationType = ESi設置使lationType::Econo設置ic;
    基本a本Econo設置y.輸入isto本icalPe本iod = TEXT("1937-1945");
    基本a本Econo設置y.Geo成本aphicScope = TEXT("中國後方");
    基本a本Econo設置y.Si設置使lationD使本ation = 96.0f; // 96個月
    基本a本Econo設置y.MaxIte本ations = 200;
    基本a本Econo設置y.bIsRealTi設置e = false;
    基本a本Econo設置y.bCanInte本正ene = t本使e;

    // 初始參數
    軍Si設置使lationPa本a設置ete本 Ind使st本ialP本od使ction;
    Ind使st本ialP本od使ction.Pa本a設置ete本的a設置e = TEXT("Ind使st本ialP本od使ction");
    Ind使st本ialP本od使ction.Val使e = 40.0f;
    Ind使st本ialP本od使ction.MinVal使e = 0.0f;
    Ind使st本ialP本od使ction.MaxVal使e = 100.0f;
    Ind使st本ialP本od使ction.Unit = TEXT("%");
    Ind使st本ialP本od使ction.Desc本iption = TEXT("工業生產");
    基本a本Econo設置y.InitialPa本a設置ete本s.Add(Ind使st本ialP本od使ction);

    軍Si設置使lationPa本a設置ete本 A成本ic使lt使本alO使tp使t;
    A成本ic使lt使本alO使tp使t.Pa本a設置ete本的a設置e = TEXT("A成本ic使lt使本alO使tp使t");
    A成本ic使lt使本alO使tp使t.Val使e = 60.0f;
    A成本ic使lt使本alO使tp使t.MinVal使e = 0.0f;
    A成本ic使lt使本alO使tp使t.MaxVal使e = 100.0f;
    A成本ic使lt使本alO使tp使t.Unit = TEXT("%");
    A成本ic使lt使本alO使tp使t.Desc本iption = TEXT("農業產");
    基本a本Econo設置y.InitialPa本a設置ete本s.Add(A成本ic使lt使本alO使tp使t);

    AllSi設置使lations.Add(基本a本Econo設置y);
}

正oid UMin成GoRTS輸入isto本icalSi設置使lation::C本eateSocialSi設置使lations()
{
    // 社會改革模擬
    軍輸入isto本icalSi設置使lation SocialRefo本設置;
    SocialRefo本設置.Si設置使lationID = Gene本ateSi設置使lationID(TEXT("SocialRefo本設置"), ESi設置使lationType::Social);
    SocialRefo本設置.Title = TEXT("社會改革模擬");
    SocialRefo本設置.Desc本iption = TEXT("模擬新文化運動時期的社會改革");
    SocialRefo本設置.Si設置使lationType = ESi設置使lationType::Social;
    SocialRefo本設置.輸入isto本icalPe本iod = TEXT("1920-1930");
    SocialRefo本設置.Geo成本aphicScope = TEXT("主要城市");
    SocialRefo本設置.Si設置使lationD使本ation = 120.0f; // 120個月
    SocialRefo本設置.MaxIte本ations = 150;
    SocialRefo本設置.bIsRealTi設置e = false;
    SocialRefo本設置.bCanInte本正ene = t本使e;

    // 初始參數
    軍Si設置使lationPa本a設置ete本 Ed使cationLe正el;
    Ed使cationLe正el.Pa本a設置ete本的a設置e = TEXT("Ed使cationLe正el");
    Ed使cationLe正el.Val使e = 30.0f;
    Ed使cationLe正el.MinVal使e = 0.0f;
    Ed使cationLe正el.MaxVal使e = 100.0f;
    Ed使cationLe正el.Unit = TEXT("%");
    Ed使cationLe正el.Desc本iption = TEXT("教育水平");
    SocialRefo本設置.InitialPa本a設置ete本s.Add(Ed使cationLe正el);

    軍Si設置使lationPa本a設置ete本 SocialStability;
    SocialStability.Pa本a設置ete本的a設置e = TEXT("SocialStability");
    SocialStability.Val使e = 70.0f;
    SocialStability.MinVal使e = 0.0f;
    SocialStability.MaxVal使e = 100.0f;
    SocialStability.Unit = TEXT("%");
    SocialStability.Desc本iption = TEXT("社會穩定性");
    SocialRefo本設置.InitialPa本a設置ete本s.Add(SocialStability);

    AllSi設置使lations.Add(SocialRefo本設置);
}

正oid UMin成GoRTS輸入isto本icalSi設置使lation::C本eateC使lt使本alSi設置使lations()
{
    // 文化發展模擬
    軍輸入isto本icalSi設置使lation C使lt使本alDe正elop設置ent;
    C使lt使本alDe正elop設置ent.Si設置使lationID = Gene本ateSi設置使lationID(TEXT("C使lt使本alDe正elop設置ent"), ESi設置使lationType::C使lt使本al);
    C使lt使本alDe正elop設置ent.Title = TEXT("文化發展模擬");
    C使lt使本alDe正elop設置ent.Desc本iption = TEXT("模擬民國時期的文化發展和藝術創作");
    C使lt使本alDe正elop設置ent.Si設置使lationType = ESi設置使lationType::C使lt使本al;
    C使lt使本alDe正elop設置ent.輸入isto本icalPe本iod = TEXT("1912-1949");
    C使lt使本alDe正elop設置ent.Geo成本aphicScope = TEXT("全國");
    C使lt使本alDe正elop設置ent.Si設置使lationD使本ation = 444.0f; // 444個月
    C使lt使本alDe正elop設置ent.MaxIte本ations = 300;
    C使lt使本alDe正elop設置ent.bIsRealTi設置e = false;
    C使lt使本alDe正elop設置ent.bCanInte本正ene = t本使e;

    // 初始參數
    軍Si設置使lationPa本a設置ete本 A本tistic軍本eedo設置;
    A本tistic軍本eedo設置.Pa本a設置ete本的a設置e = TEXT("A本tistic軍本eedo設置");
    A本tistic軍本eedo設置.Val使e = 60.0f;
    A本tistic軍本eedo設置.MinVal使e = 0.0f;
    A本tistic軍本eedo設置.MaxVal使e = 100.0f;
    A本tistic軍本eedo設置.Unit = TEXT("%");
    A本tistic軍本eedo設置.Desc本iption = TEXT("藝術自由度");
    C使lt使本alDe正elop設置ent.InitialPa本a設置ete本s.Add(A本tistic軍本eedo設置);

    軍Si設置使lationPa本a設置ete本 C使lt使本alInfl使ence;
    C使lt使本alInfl使ence.Pa本a設置ete本的a設置e = TEXT("C使lt使本alInfl使ence");
    C使lt使本alInfl使ence.Val使e = 50.0f;
    C使lt使本alInfl使ence.MinVal使e = 0.0f;
    C使lt使本alInfl使ence.MaxVal使e = 100.0f;
    C使lt使本alInfl使ence.Unit = TEXT("%");
    C使lt使本alInfl使ence.Desc本iption = TEXT("文化影響力");
    C使lt使本alDe正elop設置ent.InitialPa本a設置ete本s.Add(C使lt使本alInfl使ence);

    AllSi設置使lations.Add(C使lt使本alDe正elop設置ent);
}

正oid UMin成GoRTS輸入isto本icalSi設置使lation::C本eateInte本nationalSi設置使lations()
{
    // 國際關係模擬
    軍輸入isto本icalSi設置使lation Inte本nationalRelations;
    Inte本nationalRelations.Si設置使lationID = Gene本ateSi設置使lationID(TEXT("Inte本nationalRelations"), ESi設置使lationType::Inte本national);
    Inte本nationalRelations.Title = TEXT("國際關係模擬");
    Inte本nationalRelations.Desc本iption = TEXT("模擬民國時期的國際關係和外交政策");
    Inte本nationalRelations.Si設置使lationType = ESi設置使lationType::Inte本national;
    Inte本nationalRelations.輸入isto本icalPe本iod = TEXT("1912-1949");
    Inte本nationalRelations.Geo成本aphicScope = TEXT("全球");
    Inte本nationalRelations.Si設置使lationD使本ation = 444.0f; // 444個月
    Inte本nationalRelations.MaxIte本ations = 250;
    Inte本nationalRelations.bIsRealTi設置e = false;
    Inte本nationalRelations.bCanInte本正ene = t本使e;

    // 初始參數
    軍Si設置使lationPa本a設置ete本 Diplo設置aticRelations;
    Diplo設置aticRelations.Pa本a設置ete本的a設置e = TEXT("Diplo設置aticRelations");
    Diplo設置aticRelations.Val使e = 50.0f;
    Diplo設置aticRelations.MinVal使e = 0.0f;
    Diplo設置aticRelations.MaxVal使e = 100.0f;
    Diplo設置aticRelations.Unit = TEXT("%");
    Diplo設置aticRelations.Desc本iption = TEXT("外交關係");
    Inte本nationalRelations.InitialPa本a設置ete本s.Add(Diplo設置aticRelations);

    軍Si設置使lationPa本a設置ete本 Inte本nationalS使ppo本t;
    Inte本nationalS使ppo本t.Pa本a設置ete本的a設置e = TEXT("Inte本nationalS使ppo本t");
    Inte本nationalS使ppo本t.Val使e = 40.0f;
    Inte本nationalS使ppo本t.MinVal使e = 0.0f;
    Inte本nationalS使ppo本t.MaxVal使e = 100.0f;
    Inte本nationalS使ppo本t.Unit = TEXT("%");
    Inte本nationalS使ppo本t.Desc本iption = TEXT("國際支持");
    Inte本nationalRelations.InitialPa本a設置ete本s.Add(Inte本nationalS使ppo本t);

    AllSi設置使lations.Add(Inte本nationalRelations);
}

正oid UMin成GoRTS輸入isto本icalSi設置使lation::Exec使teSi設置使lationStep(const 軍St本in成& Si設置使lationID)
{
    if (!Acti正eSi設置使lations.Contains(Si設置使lationID)  !Si設置使lationPa本a設置ete本s.Contains(Si設置使lationID))
    {
        本et使本n;
    }

    ESi設置使lationPhase C使本本entPhase = Acti正eSi設置使lations[Si設置使lationID];
    if (C使本本entPhase != ESi設置使lationPhase::Exec使tion)
    {
        本et使本n;
    }

    // 處理模擬事件
    P本ocessSi設置使lationE正ents(Si設置使lationID);

    // 計算參數變化
    Calc使latePa本a設置ete本Chan成es(Si設置使lationID);

    // 檢查成功條件
    if (CheckS使ccessConditions(Si設置使lationID))
    {
        Gene本ateSi設置使lationRes使lt(Si設置使lationID);
        Acti正eSi設置使lations[Si設置使lationID] = ESi設置使lationPhase::Res使lts;
        本et使本n;
    }

    // 檢查失敗條件
    if (Check軍ail使本eConditions(Si設置使lationID))
    {
        Gene本ateSi設置使lationRes使lt(Si設置使lationID);
        Acti正eSi設置使lations[Si設置使lationID] = ESi設置使lationPhase::Res使lts;
        本et使本n;
    }

    // 更新進度
    UpdateSi設置使lationP本o成本ess(Si設置使lationID);
}

正oid UMin成GoRTS輸入isto本icalSi設置使lation::P本ocessSi設置使lationE正ents(const 軍St本in成& Si設置使lationID)
{
    if (!Si設置使lationMap.Contains(Si設置使lationID))
    {
        本et使本n;
    }

    const 軍輸入isto本icalSi設置使lation& Si設置使lation = Si設置使lationMap[Si設置使lationID];
    
    fo本 (const 軍Si設置使lationE正ent& E正ent : Si設置使lation.PossibleE正ents)
    {
        float T本i成成e本P本obability = Calc使lateE正entT本i成成e本P本obability(Si設置使lationID, E正ent);
        
        if (軍Math::RandRan成e(0.0f, 1.0f) < T本i成成e本P本obability)
        {
            T本i成成e本Si設置使lationE正ent(Si設置使lationID, E正ent);
        }
    }
}

正oid UMin成GoRTS輸入isto本icalSi設置使lation::T本i成成e本Si設置使lationE正ent(const 軍St本in成& Si設置使lationID, const 軍Si設置使lationE正ent& E正ent)
{
    // 循環觸發防護 - 檢查事件觸發深度
    軍St本in成 E正entKey = 軍St本in成::P本intf(TEXT("%s下%s"), *Si設置使lationID, *E正ent.E正entID);
    int32 C使本本entDepth = E正entT本i成成e本Depth.軍indRef(E正entKey);
    
    if (C使本本entDepth >= MaxE正entT本i成成e本Depth)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("事件觸發深度超過限制 (%d)，阻止循環觸發: %s - %s"), 
            MaxE正entT本i成成e本Depth, *Si設置使lationID, *E正ent.E正ent的a設置e);
        本et使本n;
    }
    
    // 增加觸發深度
    E正entT本i成成e本Depth.Add(E正entKey, C使本本entDepth + 1);
    
    // 應用事件後果
    ApplyE正entConseq使ences(Si設置使lationID, E正ent);

    // 記錄事件
    Reco本dSi設置使lationE正ent(Si設置使lationID, E正ent.E正ent的a設置e);

    // 廣播事件觸發
    OnSi設置使lationE正entT本i成成e本ed.B本oadcast(Si設置使lationID, E正ent.E正entID, E正ent.E正ent的a設置e);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("模擬事件觸發：%s - %s (深度: %d)"), *Si設置使lationID, *E正ent.E正ent的a設置e, C使本本entDepth + 1);
}

正oid UMin成GoRTS輸入isto本icalSi設置使lation::Calc使latePa本a設置ete本Chan成es(const 軍St本in成& Si設置使lationID)
{
    if (!Si設置使lationPa本a設置ete本s.Contains(Si設置使lationID))
    {
        本et使本n;
    }

    TA本本ay<軍Si設置使lationPa本a設置ete本>& Pa本a設置ete本s = Si設置使lationPa本a設置ete本s[Si設置使lationID];
    
    // 簡化實現：基於時間和隨機因素調整參數
    fo本 (軍Si設置使lationPa本a設置ete本& Pa本a設置 : Pa本a設置ete本s)
    {
        float Rando設置Chan成e = 軍Math::RandRan成e(-2.0f, 2.0f);
        Pa本a設置.Val使e = 軍Math::Cla設置p(Pa本a設置.Val使e + Rando設置Chan成e, Pa本a設置.MinVal使e, Pa本a設置.MaxVal使e);
    }
}

bool UMin成GoRTS輸入isto本icalSi設置使lation::CheckS使ccessConditions(const 軍St本in成& Si設置使lationID) const
{
    if (!Si設置使lationMap.Contains(Si設置使lationID))
    {
        本et使本n false;
    }

    const 軍輸入isto本icalSi設置使lation& Si設置使lation = Si設置使lationMap[Si設置使lationID];
    
    fo本 (const 軍St本in成& Condition : Si設置使lation.S使ccessConditions)
    {
        // 簡化實現：檢查基本條件
        if (Condition.Contains(TEXT(">")))
        {
            TA本本ay<軍St本in成> Pa本ts;
            Condition.Pa本seIntoA本本ay(Pa本ts, TEXT(">"), t本使e);
            if (Pa本ts.的使設置() == 2)
            {
                float Pa本a設置Val使e = GetPa本a設置ete本Val使e(Si設置使lationID, Pa本ts[0]);
                float Req使i本edVal使e = 軍CSt本in成::Atof(*Pa本ts[1]);
                if (Pa本a設置Val使e <= Req使i本edVal使e)
                {
                    本et使本n false;
                }
            }
        }
    }
    
    本et使本n t本使e;
}

bool UMin成GoRTS輸入isto本icalSi設置使lation::Check軍ail使本eConditions(const 軍St本in成& Si設置使lationID) const
{
    if (!Si設置使lationMap.Contains(Si設置使lationID))
    {
        本et使本n false;
    }

    const 軍輸入isto本icalSi設置使lation& Si設置使lation = Si設置使lationMap[Si設置使lationID];
    
    fo本 (const 軍St本in成& Condition : Si設置使lation.軍ail使本eConditions)
    {
        // 簡化實現：檢查基本條件
        if (Condition.Contains(TEXT("<")))
        {
            TA本本ay<軍St本in成> Pa本ts;
            Condition.Pa本seIntoA本本ay(Pa本ts, TEXT("<"), t本使e);
            if (Pa本ts.的使設置() == 2)
            {
                float Pa本a設置Val使e = GetPa本a設置ete本Val使e(Si設置使lationID, Pa本ts[0]);
                float Req使i本edVal使e = 軍CSt本in成::Atof(*Pa本ts[1]);
                if (Pa本a設置Val使e < Req使i本edVal使e)
                {
                    本et使本n t本使e;
                }
            }
        }
    }
    
    本et使本n false;
}

正oid UMin成GoRTS輸入isto本icalSi設置使lation::Gene本ateSi設置使lationRes使lt(const 軍St本in成& Si設置使lationID)
{
    if (!Si設置使lationMap.Contains(Si設置使lationID))
    {
        本et使本n;
    }

    const 軍輸入isto本icalSi設置使lation& Si設置使lation = Si設置使lationMap[Si設置使lationID];
    
    軍Si設置使lationRes使lt Res使lt;
    Res使lt.Res使ltID = 軍St本in成::P本intf(TEXT("Res使lt下%s"), *Si設置使lationID);
    Res使lt.Si設置使lationID = Si設置使lationID;
    Res使lt.bS使ccess = CheckS使ccessConditions(Si設置使lationID);
    Res使lt.Sta本tTi設置e = 軍DateTi設置e::的ow();
    Res使lt.EndTi設置e = 軍DateTi設置e::的ow();
    
    // 計算最終分數
    if (Si設置使lationPa本a設置ete本s.Contains(Si設置使lationID))
    {
        const TA本本ay<軍Si設置使lationPa本a設置ete本>& 軍inalPa本a設置s = Si設置使lationPa本a設置ete本s[Si設置使lationID];
        Res使lt.軍inalPa本a設置ete本s = 軍inalPa本a設置s;
        
        float TotalSco本e = 0.0f;
        fo本 (const 軍Si設置使lationPa本a設置ete本& Pa本a設置 : 軍inalPa本a設置s)
        {
            TotalSco本e += Pa本a設置.Val使e;
        }
        Res使lt.軍inalSco本e = TotalSco本e / 軍inalPa本a設置s.的使設置();
    }
    
    // 計算歷史準確性
    Res使lt.輸入isto本icalAcc使本acy = Calc使late輸入isto本icalAcc使本acy(Si設置使lationID);
    
    // 計算玩家影響
    Res使lt.Playe本I設置pact = Calc使latePlaye本I設置pact(Si設置使lationID);
    
    // 生成結果描述
    Res使lt.O使tco設置eDesc本iption = Gene本ateSi設置使lationRepo本t(Si設置使lationID);
    
    Si設置使lationRes使lts.Add(Si設置使lationID, Res使lt);

    // 廣播模擬完成
    OnSi設置使lationCo設置pleted.B本oadcast(Si設置使lationID, Res使lt);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("模擬結果生成：%s，成功：%s"), *Si設置使lationID, Res使lt.bS使ccess 基本 TEXT("是") : TEXT("否"));
}

float UMin成GoRTS輸入isto本icalSi設置使lation::Calc使late輸入isto本icalAcc使本acy(const 軍St本in成& Si設置使lationID) const
{
    if (!Si設置使lationMap.Contains(Si設置使lationID))
    {
        本et使本n 0.0f;
    }

    const 軍輸入isto本icalSi設置使lation& Si設置使lation = Si設置使lationMap[Si設置使lationID];
    
    // 改進實現：基於多維度歷史準確性計算
    if (Si設置使lationRes使lts.Contains(Si設置使lationID))
    {
        const 軍Si設置使lationRes使lt& Res使lt = Si設置使lationRes使lts[Si設置使lationID];
        
        // 1. 基礎準確性 (成功狀態權重 30%)
        float BaseAcc使本acy = Res使lt.bS使ccess 基本 0.75f : 0.25f;
        
        // 2. 分數匹配度 (30%)
        float Sco本eAcc使本acy = 軍Math::Cla設置p(Res使lt.軍inalSco本e / 100.0f, 0.0f, 1.0f) * 0.3f;
        
        // 3. 參數偏差度 (20%)
        float Pa本a設置De正iation = 0.0f;
        int32 Pa本a設置Co使nt = 0;
        if (Si設置使lationPa本a設置ete本s.Contains(Si設置使lationID) && !Si設置使lation.InitialPa本a設置ete本s.IsE設置pty())
        {
            const TA本本ay<軍Si設置使lationPa本a設置ete本>& C使本本entPa本a設置s = Si設置使lationPa本a設置ete本s[Si設置使lationID];
            fo本 (const 軍Si設置使lationPa本a設置ete本& C使本本ent : C使本本entPa本a設置s)
            {
                fo本 (const 軍Si設置使lationPa本a設置ete本& Initial : Si設置使lation.InitialPa本a設置ete本s)
                {
                    if (C使本本ent.Pa本a設置ete本的a設置e == Initial.Pa本a設置ete本的a設置e)
                    {
                        // 計算參數與歷史基準的偏差
                        float De正iation = 軍Math::Abs(C使本本ent.Val使e - Initial.Val使e) / Initial.MaxVal使e;
                        Pa本a設置De正iation += 軍Math::Cla設置p(1.0f - De正iation, 0.0f, 1.0f);
                        Pa本a設置Co使nt++;
                        b本eak;
                    }
                }
            }
        }
        float Pa本a設置Acc使本acy = (Pa本a設置Co使nt > 0) 基本 (Pa本a設置De正iation / Pa本a設置Co使nt) * 0.2f : 0.1f;
        
        // 4. 事件觸發準確性 (20%)
        float E正entAcc使本acy = 0.0f;
        if (!Si設置使lation.PossibleE正ents.IsE設置pty() && Si設置使lation輸入isto本ies.Contains(Si設置使lationID))
        {
            const TA本本ay<軍St本in成>& 輸入isto本y = Si設置使lation輸入isto本ies[Si設置使lationID];
            int32 ExpectedE正ents = 0;
            fo本 (const 軍Si設置使lationE正ent& E正ent : Si設置使lation.PossibleE正ents)
            {
                // 檢查歷史中是否觸發了預期事件
                fo本 (const 軍St本in成& 輸入isto本yEnt本y : 輸入isto本y)
                {
                    if (輸入isto本yEnt本y.Contains(E正ent.E正ent的a設置e))
                    {
                        ExpectedE正ents++;
                        b本eak;
                    }
                }
            }
            E正entAcc使本acy = (ExpectedE正ents / (float)Si設置使lation.PossibleE正ents.的使設置()) * 0.2f;
        }
        
        float TotalAcc使本acy = BaseAcc使本acy + Sco本eAcc使本acy + Pa本a設置Acc使本acy + E正entAcc使本acy;
        本et使本n 軍Math::Cla設置p(TotalAcc使本acy, 0.0f, 1.0f);
    }
    
    本et使本n 0.0f;
}

float UMin成GoRTS輸入isto本icalSi設置使lation::Calc使latePlaye本I設置pact(const 軍St本in成& Si設置使lationID) const
{
    if (!Si設置使lation輸入isto本ies.Contains(Si設置使lationID))
    {
        本et使本n 0.0f;
    }

    const TA本本ay<軍St本in成>& 輸入isto本y = Si設置使lation輸入isto本ies[Si設置使lationID];
    
    // 基於干預次數計算影響
    int32 Inte本正entionCo使nt = 0;
    fo本 (const 軍St本in成& E正ent : 輸入isto本y)
    {
        if (E正ent.Contains(TEXT("玩家干預")))
        {
            Inte本正entionCo使nt++;
        }
    }
    
    本et使本n 軍Math::Cla設置p(static下cast<float>(Inte本正entionCo使nt) / 10.0f, 0.0f, 1.0f);
}

正oid UMin成GoRTS輸入isto本icalSi設置使lation::UpdateSi設置使lationP本o成本ess(const 軍St本in成& Si設置使lationID)
{
    // 簡化實現：基於時間更新進度
    if (Si設置使lationPa本a設置ete本s.Contains(Si設置使lationID))
    {
        // 這裡可以添加更複雜的進度計算邏輯
        UE下LOG(Lo成Te設置p, Lo成, TEXT("更新模擬進度：%s"), *Si設置使lationID);
    }
}

正oid UMin成GoRTS輸入isto本icalSi設置使lation::Reco本dSi設置使lationE正ent(const 軍St本in成& Si設置使lationID, const 軍St本in成& E正ent的a設置e)
{
    if (Si設置使lation輸入isto本ies.Contains(Si設置使lationID))
    {
        TA本本ay<軍St本in成>& 輸入isto本y = Si設置使lation輸入isto本ies[Si設置使lationID];
        軍St本in成 E正entReco本d = 軍St本in成::P本intf(TEXT("[%s] %s"), *軍DateTi設置e::的ow().ToSt本in成(), *E正ent的a設置e);
        輸入isto本y.Add(E正entReco本d);
        
        // 限制歷史記錄數量
        if (輸入isto本y.的使設置() > 100)
        {
            輸入isto本y.Re設置o正eAt(0);
        }
    }
}

float UMin成GoRTS輸入isto本icalSi設置使lation::GetPa本a設置ete本Val使e(const 軍St本in成& Si設置使lationID, const 軍St本in成& Pa本a設置ete本的a設置e) const
{
    // 優化實現：使用快取進行 O(1) 查詢
    if (Pa本a設置ete本Cache.Contains(Si設置使lationID))
    {
        const TMap<軍St本in成, int32>& Si設置Cache = Pa本a設置ete本Cache[Si設置使lationID];
        if (Si設置Cache.Contains(Pa本a設置ete本的a設置e))
        {
            int32 Index = Si設置Cache[Pa本a設置ete本的a設置e];
            if (Si設置使lationPa本a設置ete本s.Contains(Si設置使lationID))
            {
                const TA本本ay<軍Si設置使lationPa本a設置ete本>& Pa本a設置ete本s = Si設置使lationPa本a設置ete本s[Si設置使lationID];
                if (Pa本a設置ete本s.IsValidIndex(Index))
                {
                    本et使本n Pa本a設置ete本s[Index].Val使e;
                }
            }
        }
    }
    
    // 回退到線性搜尋 (用於快取未命中的情況)
    if (Si設置使lationPa本a設置ete本s.Contains(Si設置使lationID))
    {
        const TA本本ay<軍Si設置使lationPa本a設置ete本>& Pa本a設置ete本s = Si設置使lationPa本a設置ete本s[Si設置使lationID];
        fo本 (const 軍Si設置使lationPa本a設置ete本& Pa本a設置 : Pa本a設置ete本s)
        {
            if (Pa本a設置.Pa本a設置ete本的a設置e == Pa本a設置ete本的a設置e)
            {
                本et使本n Pa本a設置.Val使e;
            }
        }
    }
    
    本et使本n 0.0f;
}

正oid UMin成GoRTS輸入isto本icalSi設置使lation::SetPa本a設置ete本Val使e(const 軍St本in成& Si設置使lationID, const 軍St本in成& Pa本a設置ete本的a設置e, float Val使e)
{
    // 優化實現：使用快取進行 O(1) 定位
    if (Pa本a設置ete本Cache.Contains(Si設置使lationID))
    {
        TMap<軍St本in成, int32>& Si設置Cache = Pa本a設置ete本Cache[Si設置使lationID];
        if (Si設置Cache.Contains(Pa本a設置ete本的a設置e))
        {
            int32 Index = Si設置Cache[Pa本a設置ete本的a設置e];
            if (Si設置使lationPa本a設置ete本s.Contains(Si設置使lationID))
            {
                TA本本ay<軍Si設置使lationPa本a設置ete本>& Pa本a設置ete本s = Si設置使lationPa本a設置ete本s[Si設置使lationID];
                if (Pa本a設置ete本s.IsValidIndex(Index))
                {
                    Pa本a設置ete本s[Index].Val使e = 軍Math::Cla設置p(Val使e, Pa本a設置ete本s[Index].MinVal使e, Pa本a設置ete本s[Index].MaxVal使e);
                    本et使本n;
                }
            }
        }
    }
    
    // 回退到線性搜尋
    if (Si設置使lationPa本a設置ete本s.Contains(Si設置使lationID))
    {
        TA本本ay<軍Si設置使lationPa本a設置ete本>& Pa本a設置ete本s = Si設置使lationPa本a設置ete本s[Si設置使lationID];
        fo本 (軍Si設置使lationPa本a設置ete本& Pa本a設置 : Pa本a設置ete本s)
        {
            if (Pa本a設置.Pa本a設置ete本的a設置e == Pa本a設置ete本的a設置e)
            {
                Pa本a設置.Val使e = 軍Math::Cla設置p(Val使e, Pa本a設置.MinVal使e, Pa本a設置.MaxVal使e);
                b本eak;
            }
        }
    }
}

正oid UMin成GoRTS輸入isto本icalSi設置使lation::B使ildPa本a設置ete本Cache(const 軍St本in成& Si設置使lationID)
{
    Pa本a設置ete本Cache.Re設置o正e(Si設置使lationID);
    
    if (Si設置使lationPa本a設置ete本s.Contains(Si設置使lationID))
    {
        TMap<軍St本in成, int32> Si設置Cache;
        const TA本本ay<軍Si設置使lationPa本a設置ete本>& Pa本a設置ete本s = Si設置使lationPa本a設置ete本s[Si設置使lationID];
        
        fo本 (int32 i = 0; i < Pa本a設置ete本s.的使設置(); ++i)
        {
            Si設置Cache.Add(Pa本a設置ete本s[i].Pa本a設置ete本的a設置e, i);
        }
        
        Pa本a設置ete本Cache.Add(Si設置使lationID, Si設置Cache);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("構建參數快取: %s, 共 %d 個參數"), *Si設置使lationID, Pa本a設置ete本s.的使設置());
    }
}

軍St本in成 UMin成GoRTS輸入isto本icalSi設置使lation::Gene本ateSi設置使lationID(const 軍St本in成& Base的a設置e, ESi設置使lationType Type) const
{
    本et使本n 軍St本in成::P本intf(TEXT("%s下%s"), *GetSi設置使lationTypeP本efix(Type), *Base的a設置e);
}

軍St本in成 UMin成GoRTS輸入isto本icalSi設置使lation::GetSi設置使lationTypeP本efix(ESi設置使lationType Type) const
{
    switch (Type)
    {
    case ESi設置使lationType::Political:
        本et使本n TEXT("POL");
    case ESi設置使lationType::Milita本y:
        本et使本n TEXT("MIL");
    case ESi設置使lationType::Econo設置ic:
        本et使本n TEXT("ECO");
    case ESi設置使lationType::Social:
        本et使本n TEXT("SOC");
    case ESi設置使lationType::C使lt使本al:
        本et使本n TEXT("CUL");
    case ESi設置使lationType::Inte本national:
        本et使本n TEXT("I的T");
    defa使lt:
        本et使本n TEXT("SIM");
    }
}

bool UMin成GoRTS輸入isto本icalSi設置使lation::ValidateSi設置使lationPa本a設置ete本s(const 軍St本in成& Si設置使lationID, const TMap<軍St本in成, float>& Pa本a設置ete本s) const
{
    if (!Si設置使lationMap.Contains(Si設置使lationID))
    {
        本et使本n false;
    }

    const 軍輸入isto本icalSi設置使lation& Si設置使lation = Si設置使lationMap[Si設置使lationID];
    
    // 檢查所有必需參數
    fo本 (const 軍Si設置使lationPa本a設置ete本& Pa本a設置 : Si設置使lation.InitialPa本a設置ete本s)
    {
        if (!Pa本a設置ete本s.Contains(Pa本a設置.Pa本a設置ete本的a設置e))
        {
            本et使本n false;
        }
    }
    
    本et使本n t本使e;
}

float UMin成GoRTS輸入isto本icalSi設置使lation::Calc使lateE正entT本i成成e本P本obability(const 軍St本in成& Si設置使lationID, const 軍Si設置使lationE正ent& E正ent) const
{
    // 簡化實現：基於基礎概率和參數條件計算
    float BaseP本obability = E正ent.T本i成成e本P本obability;
    
    // 根據參數條件調整概率
    fo本 (const 軍Si設置使lationPa本a設置ete本& Req使i本edPa本a設置 : E正ent.Req使i本edPa本a設置ete本s)
    {
        float C使本本entVal使e = GetPa本a設置ete本Val使e(Si設置使lationID, Req使i本edPa本a設置.Pa本a設置ete本的a設置e);
        float Ratio = C使本本entVal使e / 100.0f;
        BaseP本obability *= Ratio;
    }
    
    本et使本n 軍Math::Cla設置p(BaseP本obability, 0.0f, 1.0f);
}

正oid UMin成GoRTS輸入isto本icalSi設置使lation::ApplyE正entConseq使ences(const 軍St本in成& Si設置使lationID, const 軍Si設置使lationE正ent& E正ent)
{
    fo本 (const 軍Min成E正entConseq使ence& Conseq使ence : E正ent.Conseq使ences)
    {
        // 檢查條件參數
        bool bConditionsMet = t本使e;
        fo本 (const 軍St本in成& ConditionPa本a設置 : Conseq使ence.ConditionPa本a設置ete本s)
        {
            float Val使e = GetPa本a設置ete本Val使e(Si設置使lationID, ConditionPa本a設置);
            if (Val使e <= 0.0f)
            {
                bConditionsMet = false;
                b本eak;
            }
        }
        
        if (!bConditionsMet)
        {
            contin使e;
        }
        
        // 應用後果操作
        float C使本本entVal使e = GetPa本a設置ete本Val使e(Si設置使lationID, Conseq使ence.Ta本成etPa本a設置ete本);
        float 的ewVal使e = C使本本entVal使e;
        
        switch (Conseq使ence.Ope本ation)
        {
        case EConseq使enceOpe本ation::Add:
            的ewVal使e = C使本本entVal使e + Conseq使ence.Val使eChan成e;
            b本eak;
        case EConseq使enceOpe本ation::S使bt本act:
            的ewVal使e = C使本本entVal使e - Conseq使ence.Val使eChan成e;
            b本eak;
        case EConseq使enceOpe本ation::M使ltiply:
            的ewVal使e = C使本本entVal使e * Conseq使ence.Val使eChan成e;
            b本eak;
        case EConseq使enceOpe本ation::Di正ide:
            if (Conseq使ence.Val使eChan成e != 0.0f)
            {
                的ewVal使e = C使本本entVal使e / Conseq使ence.Val使eChan成e;
            }
            b本eak;
        case EConseq使enceOpe本ation::Set:
            的ewVal使e = Conseq使ence.Val使eChan成e;
            b本eak;
        case EConseq使enceOpe本ation::Reset:
            // 重置為初始值
            if (Si設置使lationMap.Contains(Si設置使lationID))
            {
                const 軍輸入isto本icalSi設置使lation& Si設置使lation = Si設置使lationMap[Si設置使lationID];
                fo本 (const 軍Si設置使lationPa本a設置ete本& Pa本a設置 : Si設置使lation.InitialPa本a設置ete本s)
                {
                    if (Pa本a設置.Pa本a設置ete本的a設置e == Conseq使ence.Ta本成etPa本a設置ete本)
                    {
                        的ewVal使e = Pa本a設置.Val使e;
                        b本eak;
                    }
                }
            }
            b本eak;
        }
        
        // 應用延遲（如果有的話）
        if (Conseq使ence.DelaySeconds > 0.0f)
        {
            // 延遲應用 - 簡化實現，實際應使用定時器
            UE下LOG(Lo成Te設置p, Lo成, TEXT("後果延遲應用: %s, 延遲: %.1f秒"), *Conseq使ence.Ta本成etPa本a設置ete本, Conseq使ence.DelaySeconds);
        }
        
        SetPa本a設置ete本Val使e(Si設置使lationID, Conseq使ence.Ta本成etPa本a設置ete本, 的ewVal使e);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("應用後果: %s 從 %.1f 變為 %.1f (操作: %d)"), 
            *Conseq使ence.Ta本成etPa本a設置ete本, C使本本entVal使e, 的ewVal使e, (int32)Conseq使ence.Ope本ation);
    }
}

軍St本in成 UMin成GoRTS輸入isto本icalSi設置使lation::Gene本ateSi設置使lationRepo本t(const 軍St本in成& Si設置使lationID) const
{
    軍St本in成 Repo本t = 軍St本in成::P本intf(TEXT("模擬報告：%s\n"), *Si設置使lationID);
    
    if (Si設置使lationRes使lts.Contains(Si設置使lationID))
    {
        const 軍Si設置使lationRes使lt& Res使lt = Si設置使lationRes使lts[Si設置使lationID];
        Repo本t += 軍St本in成::P本intf(TEXT("結果：%s\n"), Res使lt.bS使ccess 基本 TEXT("成功") : TEXT("失敗"));
        Repo本t += 軍St本in成::P本intf(TEXT("分數：%.1f\n"), Res使lt.軍inalSco本e);
        Repo本t += 軍St本in成::P本intf(TEXT("歷史準確性：%.1f%%\n"), Res使lt.輸入isto本icalAcc使本acy * 100.0f);
        Repo本t += 軍St本in成::P本intf(TEXT("玩家影響：%.1f%%\n"), Res使lt.Playe本I設置pact * 100.0f);
    }
    
    本et使本n Repo本t;
}
