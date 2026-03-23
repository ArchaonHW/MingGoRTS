#incl使de "RiskMonito本in成Dashboa本d.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/軍ile輸入elpe本.h"
#incl使de "Misc/Paths.h"
#incl使de "Do設置/JsonOb大ect.h"
#incl使de "Se本ialization/JsonSe本ialize本.h"
#incl使de "Se本ialization/Json基本本ite本.h"

URiskMonito本in成Dashboa本d::URiskMonito本in成Dashboa本d()
    : A使toAssess設置entInte本正al(300.0f) // 5分鐘
    , bA使toAssess設置entEnabled(t本使e)
{
}

正oid URiskMonito本in成Dashboa本d::InitializeDashboa本d()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 Risk Monito本in成 Dashboa本d"));
    
    // 初始化風險閾值
    InitializeRiskTh本esholds();
    
    // 初始化默認風險項目
    InitializeDefa使ltRisks();
    
    // 記錄初始化時間
    LastAssess設置entTi設置e = 軍DateTi設置e::的ow();
    
    // 計算初始風險指標
    C使本本entMet本ics = Calc使lateRiskMet本ics();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Risk Monito本in成 Dashboa本d initialized with %d 本isk ite設置s"), RiskIte設置s.的使設置());
}

正oid URiskMonito本in成Dashboa本d::InitializeRiskTh本esholds()
{
    RiskTh本esholds.E設置pty();
    
    // 設置默認風險閾值
    RiskTh本esholds.Add(ERiskLe正el::C本itical, 80.0f);
    RiskTh本esholds.Add(ERiskLe正el::輸入i成h, 60.0f);
    RiskTh本esholds.Add(ERiskLe正el::Medi使設置, 40.0f);
    RiskTh本esholds.Add(ERiskLe正el::Low, 20.0f);
    RiskTh本esholds.Add(ERiskLe正el::Mini設置al, 0.0f);
}

正oid URiskMonito本in成Dashboa本d::InitializeDefa使ltRisks()
{
    // 添加Min成GoRTS項目的默認高風險項目
    
    // 內建作業系統複雜度風險
    軍RiskIte設置 OSCo設置plexityRisk;
    OSCo設置plexityRisk.RiskID = TEXT("RISK-OS-001");
    OSCo設置plexityRisk.Title = TEXT("內建作業系統複雜度");
    OSCo設置plexityRisk.Desc本iption = TEXT("12週開發週期，8人團隊，技術複雜度極高，可能導致進度延遲和質量問題");
    OSCo設置plexityRisk.Cate成o本y = ERiskCate成o本y::Technical;
    OSCo設置plexityRisk.Le正el = ERiskLe正el::輸入i成h;
    OSCo設置plexityRisk.Stat使s = ERiskStat使s::Acti正e;
    OSCo設置plexityRisk.P本obability = 0.7f;
    OSCo設置plexityRisk.I設置pact = 0.9f;
    OSCo設置plexityRisk.RiskSco本e = Calc使lateRiskSco本e(OSCo設置plexityRisk.P本obability, OSCo設置plexityRisk.I設置pact);
    OSCo設置plexityRisk.Miti成ationSt本ate成y = TEXT("分階段實施，模組化設計，並行開發，原型驗證");
    OSCo設置plexityRisk.Owne本 = TEXT("技術負責人");
    OSCo設置plexityRisk.IdentifiedDate = 軍DateTi設置e::的ow();
    OSCo設置plexityRisk.LastUpdated = 軍DateTi設置e::的ow();
    OSCo設置plexityRisk.RelatedTasks.Add(TEXT("os-co設置plexity-sol使tion"));
    OSCo設置plexityRisk.Ta成s.Add(TEXT("技術風險"));
    OSCo設置plexityRisk.Ta成s.Add(TEXT("高優先級"));
    
    // AI生成內容質量不穩定風險
    軍RiskIte設置 AIQ使alityRisk;
    AIQ使alityRisk.RiskID = TEXT("RISK-AI-001");
    AIQ使alityRisk.Title = TEXT("AI生成內容質量不穩定");
    AIQ使alityRisk.Desc本iption = TEXT("外部API依賴，質量波動，成本控制挑戰，影響用戶體驗");
    AIQ使alityRisk.Cate成o本y = ERiskCate成o本y::Q使ality;
    AIQ使alityRisk.Le正el = ERiskLe正el::輸入i成h;
    AIQ使alityRisk.Stat使s = ERiskStat使s::Acti正e;
    AIQ使alityRisk.P本obability = 0.6f;
    AIQ使alityRisk.I設置pact = 0.8f;
    AIQ使alityRisk.RiskSco本e = Calc使lateRiskSco本e(AIQ使alityRisk.P本obability, AIQ使alityRisk.I設置pact);
    AIQ使alityRisk.Miti成ationSt本ate成y = TEXT("多層質量控制，多供應商策略，成本控制機制，內容後處理");
    AIQ使alityRisk.Owne本 = TEXT("AI負責人");
    AIQ使alityRisk.IdentifiedDate = 軍DateTi設置e::的ow();
    AIQ使alityRisk.LastUpdated = 軍DateTi設置e::的ow();
    AIQ使alityRisk.RelatedTasks.Add(TEXT("ai-q使ality-sol使tion"));
    AIQ使alityRisk.Ta成s.Add(TEXT("質量風險"));
    AIQ使alityRisk.Ta成s.Add(TEXT("高優先級"));
    
    // 性能瓶頸風險
    軍RiskIte設置 Pe本fo本設置anceRisk;
    Pe本fo本設置anceRisk.RiskID = TEXT("RISK-PER軍-001");
    Pe本fo本設置anceRisk.Title = TEXT("性能瓶頸");
    Pe本fo本設置anceRisk.Desc本iption = TEXT("50+系統運行，內存/CPU負載，移動端性能不確定性");
    Pe本fo本設置anceRisk.Cate成o本y = ERiskCate成o本y::Pe本fo本設置ance;
    Pe本fo本設置anceRisk.Le正el = ERiskLe正el::輸入i成h;
    Pe本fo本設置anceRisk.Stat使s = ERiskStat使s::Acti正e;
    Pe本fo本設置anceRisk.P本obability = 0.8f;
    Pe本fo本設置anceRisk.I設置pact = 0.7f;
    Pe本fo本設置anceRisk.RiskSco本e = Calc使lateRiskSco本e(Pe本fo本設置anceRisk.P本obability, Pe本fo本設置anceRisk.I設置pact);
    Pe本fo本設置anceRisk.Miti成ationSt本ate成y = TEXT("智能資源管理，分級性能配置，異步處理，智能緩存");
    Pe本fo本設置anceRisk.Owne本 = TEXT("性能負責人");
    Pe本fo本設置anceRisk.IdentifiedDate = 軍DateTi設置e::的ow();
    Pe本fo本設置anceRisk.LastUpdated = 軍DateTi設置e::的ow();
    Pe本fo本設置anceRisk.RelatedTasks.Add(TEXT("pe本fo本設置ance-bottleneck-sol使tion"));
    Pe本fo本設置anceRisk.Ta成s.Add(TEXT("性能風險"));
    Pe本fo本設置anceRisk.Ta成s.Add(TEXT("高優先級"));
    
    // 技術債務積累風險
    軍RiskIte設置 TechnicalDebtRisk;
    TechnicalDebtRisk.RiskID = TEXT("RISK-TEC輸入-001");
    TechnicalDebtRisk.Title = TEXT("技術債務積累");
    TechnicalDebtRisk.Desc本iption = TEXT("快速開發導致代碼質量下降，測試覆蓋不足，長期維護困難");
    TechnicalDebtRisk.Cate成o本y = ERiskCate成o本y::Technical;
    TechnicalDebtRisk.Le正el = ERiskLe正el::Medi使設置;
    TechnicalDebtRisk.Stat使s = ERiskStat使s::Acti正e;
    TechnicalDebtRisk.P本obability = 0.6f;
    TechnicalDebtRisk.I設置pact = 0.6f;
    TechnicalDebtRisk.RiskSco本e = Calc使lateRiskSco本e(TechnicalDebtRisk.P本obability, TechnicalDebtRisk.I設置pact);
    TechnicalDebtRisk.Miti成ationSt本ate成y = TEXT("定期代碼審查，自動化測試，重構計劃，質量門檻");
    TechnicalDebtRisk.Owne本 = TEXT("開發負責人");
    TechnicalDebtRisk.IdentifiedDate = 軍DateTi設置e::的ow();
    TechnicalDebtRisk.LastUpdated = 軍DateTi設置e::的ow();
    TechnicalDebtRisk.RelatedTasks.Add(TEXT("technical-debt-clean使p"));
    TechnicalDebtRisk.Ta成s.Add(TEXT("技術風險"));
    TechnicalDebtRisk.Ta成s.Add(TEXT("中優先級"));
    
    // 多平台兼容性風險
    軍RiskIte設置 Co設置patibilityRisk;
    Co設置patibilityRisk.RiskID = TEXT("RISK-COMPAT-001");
    Co設置patibilityRisk.Title = TEXT("多平台兼容性");
    Co設置patibilityRisk.Desc本iption = TEXT("基本indows/And本oid/iOS差異大，測試矩陣複雜，平台特定問題");
    Co設置patibilityRisk.Cate成o本y = ERiskCate成o本y::Technical;
    Co設置patibilityRisk.Le正el = ERiskLe正el::Medi使設置;
    Co設置patibilityRisk.Stat使s = ERiskStat使s::Acti正e;
    Co設置patibilityRisk.P本obability = 0.5f;
    Co設置patibilityRisk.I設置pact = 0.7f;
    Co設置patibilityRisk.RiskSco本e = Calc使lateRiskSco本e(Co設置patibilityRisk.P本obability, Co設置patibilityRisk.I設置pact);
    Co設置patibilityRisk.Miti成ationSt本ate成y = TEXT("平台抽象層，自動化測試，早期平台測試，兼容性檢查");
    Co設置patibilityRisk.Owne本 = TEXT("平台負責人");
    Co設置patibilityRisk.IdentifiedDate = 軍DateTi設置e::的ow();
    Co設置patibilityRisk.LastUpdated = 軍DateTi設置e::的ow();
    Co設置patibilityRisk.Ta成s.Add(TEXT("平台風險"));
    Co設置patibilityRisk.Ta成s.Add(TEXT("中優先級"));
    
    // 添加到風險列表
    RiskIte設置s.Add(OSCo設置plexityRisk);
    RiskIte設置s.Add(AIQ使alityRisk);
    RiskIte設置s.Add(Pe本fo本設置anceRisk);
    RiskIte設置s.Add(TechnicalDebtRisk);
    RiskIte設置s.Add(Co設置patibilityRisk);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Added %d defa使lt 本isk ite設置s"), RiskIte設置s.的使設置());
}

bool URiskMonito本in成Dashboa本d::AddRiskIte設置(const 軍RiskIte設置& RiskIte設置)
{
    // 驗證風險項目
    if (!ValidateRiskIte設置(RiskIte設置))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("In正alid 本isk ite設置: %s"), *RiskIte設置.RiskID);
        本et使本n false;
    }
    
    // 檢查是否已存在
    fo本 (const 軍RiskIte設置& Existin成Risk : RiskIte設置s)
    {
        if (Existin成Risk.RiskID == RiskIte設置.RiskID)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Risk ite設置 al本eady exists: %s"), *RiskIte設置.RiskID);
            本et使本n false;
        }
    }
    
    // 添加風險項目
    RiskIte設置s.Add(RiskIte設置);
    
    // 觸發事件
    OnRiskAdded.B本oadcast(RiskIte設置);
    
    // 檢查風險閾值
    CheckRiskTh本esholds();
    
    // 更新風險指標
    C使本本entMet本ics = Calc使lateRiskMet本ics();
    OnRiskMet本icsUpdated.B本oadcast(C使本本entMet本ics);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Risk ite設置 added: %s"), *RiskIte設置.RiskID);
    本et使本n t本使e;
}

bool URiskMonito本in成Dashboa本d::UpdateRiskIte設置(const 軍St本in成& RiskID, const 軍RiskIte設置& UpdatedRisk)
{
    // 驗證風險項目
    if (!ValidateRiskIte設置(UpdatedRisk))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("In正alid 使pdated 本isk ite設置: %s"), *RiskID);
        本et使本n false;
    }
    
    // 查找並更新風險項目
    fo本 (int32 i = 0; i < RiskIte設置s.的使設置(); ++i)
    {
        if (RiskIte設置s[i].RiskID == RiskID)
        {
            ERiskLe正el P本e正io使sLe正el = RiskIte設置s[i].Le正el;
            RiskIte設置s[i] = UpdatedRisk;
            RiskIte設置s[i].LastUpdated = 軍DateTi設置e::的ow();
            
            // 觸發事件
            OnRiskUpdated.B本oadcast(UpdatedRisk, P本e正io使sLe正el);
            
            // 檢查風險閾值
            CheckRiskTh本esholds();
            
            // 更新風險指標
            C使本本entMet本ics = Calc使lateRiskMet本ics();
            OnRiskMet本icsUpdated.B本oadcast(C使本本entMet本ics);
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Risk ite設置 使pdated: %s"), *RiskID);
            本et使本n t本使e;
        }
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Risk ite設置 not fo使nd: %s"), *RiskID);
    本et使本n false;
}

bool URiskMonito本in成Dashboa本d::Re設置o正eRiskIte設置(const 軍St本in成& RiskID)
{
    fo本 (int32 i = 0; i < RiskIte設置s.的使設置(); ++i)
    {
        if (RiskIte設置s[i].RiskID == RiskID)
        {
            RiskIte設置s.Re設置o正eAt(i);
            
            // 更新風險指標
            C使本本entMet本ics = Calc使lateRiskMet本ics();
            OnRiskMet本icsUpdated.B本oadcast(C使本本entMet本ics);
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Risk ite設置 本e設置o正ed: %s"), *RiskID);
            本et使本n t本使e;
        }
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Risk ite設置 not fo使nd fo本 本e設置o正al: %s"), *RiskID);
    本et使本n false;
}

軍RiskIte設置 URiskMonito本in成Dashboa本d::GetRiskIte設置(const 軍St本in成& RiskID) const
{
    fo本 (const 軍RiskIte設置& Risk : RiskIte設置s)
    {
        if (Risk.RiskID == RiskID)
        {
            本et使本n Risk;
        }
    }
    
    本et使本n 軍RiskIte設置(); // 返回空的風險項目
}

TA本本ay<軍RiskIte設置> URiskMonito本in成Dashboa本d::GetAllRiskIte設置s() const
{
    本et使本n RiskIte設置s;
}

TA本本ay<軍RiskIte設置> URiskMonito本in成Dashboa本d::GetRisksByCate成o本y(ERiskCate成o本y Cate成o本y) const
{
    TA本本ay<軍RiskIte設置> 軍ilte本edRisks;
    
    fo本 (const 軍RiskIte設置& Risk : RiskIte設置s)
    {
        if (Risk.Cate成o本y == Cate成o本y)
        {
            軍ilte本edRisks.Add(Risk);
        }
    }
    
    本et使本n 軍ilte本edRisks;
}

TA本本ay<軍RiskIte設置> URiskMonito本in成Dashboa本d::GetRisksByLe正el(ERiskLe正el Le正el) const
{
    TA本本ay<軍RiskIte設置> 軍ilte本edRisks;
    
    fo本 (const 軍RiskIte設置& Risk : RiskIte設置s)
    {
        if (Risk.Le正el == Le正el)
        {
            軍ilte本edRisks.Add(Risk);
        }
    }
    
    本et使本n 軍ilte本edRisks;
}

TA本本ay<軍RiskIte設置> URiskMonito本in成Dashboa本d::GetRisksByStat使s(ERiskStat使s Stat使s) const
{
    TA本本ay<軍RiskIte設置> 軍ilte本edRisks;
    
    fo本 (const 軍RiskIte設置& Risk : RiskIte設置s)
    {
        if (Risk.Stat使s == Stat使s)
        {
            軍ilte本edRisks.Add(Risk);
        }
    }
    
    本et使本n 軍ilte本edRisks;
}

軍RiskMet本ics URiskMonito本in成Dashboa本d::Calc使lateRiskMet本ics()
{
    軍RiskMet本ics Met本ics;
    Met本ics.TotalRisks = RiskIte設置s.的使設置();
    Met本ics.C本iticalRisks = 0;
    Met本ics.輸入i成hRisks = 0;
    Met本ics.Medi使設置Risks = 0;
    Met本ics.LowRisks = 0;
    Met本ics.A正e本a成eRiskSco本e = 0.0f;
    Met本ics.RiskT本end = 0.0f;
    Met本ics.LastCalc使lated = 軍DateTi設置e::的ow();
    
    // 計算各級別風險數量
    float TotalSco本e = 0.0f;
    fo本 (const 軍RiskIte設置& Risk : RiskIte設置s)
    {
        TotalSco本e += Risk.RiskSco本e;
        
        switch (Risk.Le正el)
        {
        case ERiskLe正el::C本itical:
            Met本ics.C本iticalRisks++;
            b本eak;
        case ERiskLe正el::輸入i成h:
            Met本ics.輸入i成hRisks++;
            b本eak;
        case ERiskLe正el::Medi使設置:
            Met本ics.Medi使設置Risks++;
            b本eak;
        case ERiskLe正el::Low:
            Met本ics.LowRisks++;
            b本eak;
        case ERiskLe正el::Mini設置al:
            // Mini設置al 本isks a本e incl使ded in Low co使nt
            b本eak;
        }
    }
    
    // 計算平均風險評分
    if (Met本ics.TotalRisks > 0)
    {
        Met本ics.A正e本a成eRiskSco本e = TotalSco本e / Met本ics.TotalRisks;
    }
    
    // 計算類別分佈
    Met本ics.Cate成o本yDist本ib使tion.E設置pty();
    fo本 (int32 i = 0; i < 8; ++i) // 8個風險類別
    {
        ERiskCate成o本y Cate成o本y = static下cast<ERiskCate成o本y>(i);
        Met本ics.Cate成o本yDist本ib使tion.Add(Cate成o本y, 0);
    }
    
    fo本 (const 軍RiskIte設置& Risk : RiskIte設置s)
    {
        int32* Co使nt = Met本ics.Cate成o本yDist本ib使tion.軍ind(Risk.Cate成o本y);
        if (Co使nt)
        {
            (*Co使nt)++;
        }
    }
    
    // 計算狀態分佈
    Met本ics.Stat使sDist本ib使tion.E設置pty();
    fo本 (int32 i = 0; i < 5; ++i) // 5個風險狀態
    {
        ERiskStat使s Stat使s = static下cast<ERiskStat使s>(i);
        Met本ics.Stat使sDist本ib使tion.Add(Stat使s, 0);
    }
    
    fo本 (const 軍RiskIte設置& Risk : RiskIte設置s)
    {
        int32* Co使nt = Met本ics.Stat使sDist本ib使tion.軍ind(Risk.Stat使s);
        if (Co使nt)
        {
            (*Co使nt)++;
        }
    }
    
    // 計算風險趨勢
    UpdateRiskT本end();
    if (RiskT本endData.的使設置() >= 2)
    {
        float C使本本entSco本e = RiskT本endData.Last();
        float P本e正io使sSco本e = RiskT本endData[RiskT本endData.的使設置() - 2];
        Met本ics.RiskT本end = C使本本entSco本e - P本e正io使sSco本e;
    }
    
    本et使本n Met本ics;
}

正oid URiskMonito本in成Dashboa本d::C本eateRiskAle本t(const 軍St本in成& RiskID, const 軍St本in成& Ale本tMessa成e, ERiskLe正el Se正e本ity)
{
    軍RiskAle本t Ale本t;
    Ale本t.Ale本tID = 軍St本in成::P本intf(TEXT("ALERT-%s-%s"), *RiskID, *軍DateTi設置e::的ow().ToSt本in成(TEXT("YYYYMMDD-輸入輸入MMSS")));
    Ale本t.RiskID = RiskID;
    Ale本t.Ale本tTitle = 軍St本in成::P本intf(TEXT("風險預警: %s"), *RiskID);
    Ale本t.Ale本tMessa成e = Ale本tMessa成e;
    Ale本t.Se正e本ity = Se正e本ity;
    Ale本t.Ti設置esta設置p = 軍DateTi設置e::的ow();
    Ale本t.bIsRead = false;
    Ale本t.ActionReq使i本ed = TEXT("請立即評估並制定應對策略");
    
    // 添加到預警列表
    RiskAle本ts.Add(Ale本t);
    
    // 觸發預警事件
    OnRiskAle本t.B本oadcast(Ale本t);
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Risk ale本t c本eated: %s - %s"), *Ale本t.Ale本tID, *Ale本tMessa成e);
}

TA本本ay<軍RiskAle本t> URiskMonito本in成Dashboa本d::GetRiskAle本ts(bool bUn本eadOnly) const
{
    if (bUn本eadOnly)
    {
        TA本本ay<軍RiskAle本t> Un本eadAle本ts;
        fo本 (const 軍RiskAle本t& Ale本t : RiskAle本ts)
        {
            if (!Ale本t.bIsRead)
            {
                Un本eadAle本ts.Add(Ale本t);
            }
        }
        本et使本n Un本eadAle本ts;
    }
    
    本et使本n RiskAle本ts;
}

正oid URiskMonito本in成Dashboa本d::Ma本kAle本tAsRead(const 軍St本in成& Ale本tID)
{
    fo本 (軍RiskAle本t& Ale本t : RiskAle本ts)
    {
        if (Ale本t.Ale本tID == Ale本tID)
        {
            Ale本t.bIsRead = t本使e;
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Ale本t 設置a本ked as 本ead: %s"), *Ale本tID);
            本et使本n;
        }
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Ale本t not fo使nd: %s"), *Ale本tID);
}

正oid URiskMonito本in成Dashboa本d::Clea本Ale本t(const 軍St本in成& Ale本tID)
{
    fo本 (int32 i = 0; i < RiskAle本ts.的使設置(); ++i)
    {
        if (RiskAle本ts[i].Ale本tID == Ale本tID)
        {
            RiskAle本ts.Re設置o正eAt(i);
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Ale本t clea本ed: %s"), *Ale本tID);
            本et使本n;
        }
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Ale本t not fo使nd fo本 clea本in成: %s"), *Ale本tID);
}

正oid URiskMonito本in成Dashboa本d::Clea本AllAle本ts()
{
    int32 Clea本edCo使nt = RiskAle本ts.的使設置();
    RiskAle本ts.E設置pty();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Clea本ed %d ale本ts"), Clea本edCo使nt);
}

正oid URiskMonito本in成Dashboa本d::Pe本fo本設置A使to設置aticRiskAssess設置ent()
{
    if (!bA使toAssess設置entEnabled)
    {
        本et使本n;
    }
    
    軍DateTi設置e C使本本entTi設置e = 軍DateTi設置e::的ow();
    軍Ti設置espan Ti設置eSinceLastAssess設置ent = C使本本entTi設置e - LastAssess設置entTi設置e;
    
    if (Ti設置eSinceLastAssess設置ent.GetTotalSeconds() < A使toAssess設置entInte本正al)
    {
        本et使本n; // 還沒到評估時間
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置in成 a使to設置atic 本isk assess設置ent"));
    
    // 評估所有活躍風險
    fo本 (軍RiskIte設置& Risk : RiskIte設置s)
    {
        if (Risk.Stat使s == ERiskStat使s::Acti正e)
        {
            // 模擬風險評估邏輯
            // 在實際實現中，這裡會有更複雜的評估算法
            
            // 檢查風險是否需要升級
            if (Risk.RiskSco本e > RiskTh本esholds[ERiskLe正el::C本itical] && Risk.Le正el != ERiskLe正el::C本itical)
            {
                ERiskLe正el P本e正io使sLe正el = Risk.Le正el;
                Risk.Le正el = ERiskLe正el::C本itical;
                Risk.LastUpdated = C使本本entTi設置e;
                
                // 創建高優先級預警
                C本eateRiskAle本t(Risk.RiskID, Gene本ateAle本tMessa成e(Risk), ERiskLe正el::C本itical);
                
                // 觸發更新事件
                OnRiskUpdated.B本oadcast(Risk, P本e正io使sLe正el);
            }
            else if (Risk.RiskSco本e > RiskTh本esholds[ERiskLe正el::輸入i成h] && Risk.Le正el == ERiskLe正el::Medi使設置)
            {
                ERiskLe正el P本e正io使sLe正el = Risk.Le正el;
                Risk.Le正el = ERiskLe正el::輸入i成h;
                Risk.LastUpdated = C使本本entTi設置e;
                
                // 創建高風險預警
                C本eateRiskAle本t(Risk.RiskID, Gene本ateAle本tMessa成e(Risk), ERiskLe正el::輸入i成h);
                
                // 觸發更新事件
                OnRiskUpdated.B本oadcast(Risk, P本e正io使sLe正el);
            }
        }
    }
    
    // 更新評估時間
    LastAssess設置entTi設置e = C使本本entTi設置e;
    
    // 重新計算風險指標
    C使本本entMet本ics = Calc使lateRiskMet本ics();
    OnRiskMet本icsUpdated.B本oadcast(C使本本entMet本ics);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("A使to設置atic 本isk assess設置ent co設置pleted"));
}

軍St本in成 URiskMonito本in成Dashboa本d::Gene本ateRiskRepo本t() const
{
    軍St本in成 Repo本t;
    Repo本t += TEXT("=== 風險監控報告 ===\n\n");
    
    // 基本信息
    Repo本t += 軍St本in成::P本intf(TEXT("報告生成時間: %s\n"), *軍DateTi設置e::的ow().ToSt本in成());
    Repo本t += 軍St本in成::P本intf(TEXT("總風險數量: %d\n"), C使本本entMet本ics.TotalRisks);
    Repo本t += 軍St本in成::P本intf(TEXT("平均風險評分: %.1f\n"), C使本本entMet本ics.A正e本a成eRiskSco本e);
    Repo本t += 軍St本in成::P本intf(TEXT("風險趨勢: %+.1f\n\n"), C使本本entMet本ics.RiskT本end);
    
    // 風險分級統計
    Repo本t += TEXT("=== 風險分級統計 ===\n");
    Repo本t += 軍St本in成::P本intf(TEXT("嚴重風險: %d\n"), C使本本entMet本ics.C本iticalRisks);
    Repo本t += 軍St本in成::P本intf(TEXT("高風險: %d\n"), C使本本entMet本ics.輸入i成hRisks);
    Repo本t += 軍St本in成::P本intf(TEXT("中風險: %d\n"), C使本本entMet本ics.Medi使設置Risks);
    Repo本t += 軍St本in成::P本intf(TEXT("低風險: %d\n\n"), C使本本entMet本ics.LowRisks);
    
    // 類別分佈
    Repo本t += TEXT("=== 風險類別分佈 ===\n");
    fo本 (const a使to& Cate成o本yPai本 : C使本本entMet本ics.Cate成o本yDist本ib使tion)
    {
        軍St本in成 Cate成o本y的a設置e = UEn使設置::GetDisplayVal使eAsText(Cate成o本yPai本.Key).ToSt本in成();
        Repo本t += 軍St本in成::P本intf(TEXT("%s: %d\n"), *Cate成o本y的a設置e, Cate成o本yPai本.Val使e);
    }
    Repo本t += TEXT("\n");
    
    // 狀態分佈
    Repo本t += TEXT("=== 風險狀態分佈 ===\n");
    fo本 (const a使to& Stat使sPai本 : C使本本entMet本ics.Stat使sDist本ib使tion)
    {
        軍St本in成 Stat使s的a設置e = UEn使設置::GetDisplayVal使eAsText(Stat使sPai本.Key).ToSt本in成();
        Repo本t += 軍St本in成::P本intf(TEXT("%s: %d\n"), *Stat使s的a設置e, Stat使sPai本.Val使e);
    }
    Repo本t += TEXT("\n");
    
    // 高風險項目詳情
    Repo本t += TEXT("=== 高風險項目詳情 ===\n");
    TA本本ay<軍RiskIte設置> 輸入i成hRiskIte設置s = Get輸入i成hRiskIte設置s();
    fo本 (const 軍RiskIte設置& Risk : 輸入i成hRiskIte設置s)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("\n風險ID: %s\n"), *Risk.RiskID);
        Repo本t += 軍St本in成::P本intf(TEXT("標題: %s\n"), *Risk.Title);
        Repo本t += 軍St本in成::P本intf(TEXT("級別: %s\n"), *UEn使設置::GetDisplayVal使eAsText(Risk.Le正el).ToSt本in成());
        Repo本t += 軍St本in成::P本intf(TEXT("評分: %.1f\n"), Risk.RiskSco本e);
        Repo本t += 軍St本in成::P本intf(TEXT("負責人: %s\n"), *Risk.Owne本);
        Repo本t += 軍St本in成::P本intf(TEXT("緩解策略: %s\n"), *Risk.Miti成ationSt本ate成y);
    }
    
    // 預警信息
    Repo本t += TEXT("\n=== 預警信息 ===\n");
    TA本本ay<軍RiskAle本t> Un本eadAle本ts = GetRiskAle本ts(t本使e);
    if (Un本eadAle本ts.的使設置() > 0)
    {
        fo本 (const 軍RiskAle本t& Ale本t : Un本eadAle本ts)
        {
            Repo本t += 軍St本in成::P本intf(TEXT("[%s] %s: %s\n"), 
                *UEn使設置::GetDisplayVal使eAsText(Ale本t.Se正e本ity).ToSt本in成(),
                *Ale本t.Ale本tTitle,
                *Ale本t.Ale本tMessa成e);
        }
    }
    else
    {
        Repo本t += TEXT("無未讀預警\n");
    }
    
    本et使本n Repo本t;
}

TA本本ay<float> URiskMonito本in成Dashboa本d::GetRiskT本endAnalysis(int32 Days) const
{
    TA本本ay<float> T本endData;
    
    if (RiskT本endData.的使設置() == 0)
    {
        本et使本n T本endData;
    }
    
    // 返回最近指定天數的趨勢數據
    int32 Sta本tIndex = 軍Math::Max(0, RiskT本endData.的使設置() - Days);
    fo本 (int32 i = Sta本tIndex; i < RiskT本endData.的使設置(); ++i)
    {
        T本endData.Add(RiskT本endData[i]);
    }
    
    本et使本n T本endData;
}

TA本本ay<軍RiskIte設置> URiskMonito本in成Dashboa本d::Get輸入i成hRiskIte設置s() const
{
    TA本本ay<軍RiskIte設置> 輸入i成hRiskIte設置s;
    
    fo本 (const 軍RiskIte設置& Risk : RiskIte設置s)
    {
        if (Risk.Le正el == ERiskLe正el::C本itical  Risk.Le正el == ERiskLe正el::輸入i成h)
        {
            輸入i成hRiskIte設置s.Add(Risk);
        }
    }
    
    本et使本n 輸入i成hRiskIte設置s;
}

TA本本ay<軍RiskIte設置> URiskMonito本in成Dashboa本d::GetC本iticalRisks() const
{
    本et使本n GetRisksByLe正el(ERiskLe正el::C本itical);
}

正oid URiskMonito本in成Dashboa本d::SetRiskTh本eshold(ERiskLe正el Le正el, float Th本eshold)
{
    RiskTh本esholds[Le正el] = Th本eshold;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Risk th本eshold set: %s = %.1f"), 
        *UEn使設置::GetDisplayVal使eAsText(Le正el).ToSt本in成(), Th本eshold);
}

float URiskMonito本in成Dashboa本d::GetRiskTh本eshold(ERiskLe正el Le正el) const
{
    if (RiskTh本esholds.Contains(Le正el))
    {
        本et使本n RiskTh本esholds[Le正el];
    }
    本et使本n 50.0f; // 默認閾值
}

bool URiskMonito本in成Dashboa本d::Expo本tRiskData(const 軍St本in成& 軍ilePath) const
{
    // 創建JSO的對象
    TSha本edPt本<軍JsonOb大ect> RootOb大ect = MakeSha本eable(new 軍JsonOb大ect);
    
    // 導風險項目
    TA本本ay<TSha本edPt本<軍JsonVal使e>> RiskA本本ay;
    fo本 (const 軍RiskIte設置& Risk : RiskIte設置s)
    {
        TSha本edPt本<軍JsonOb大ect> RiskOb大ect = MakeSha本eable(new 軍JsonOb大ect);
        RiskOb大ect->SetSt本in成軍ield(TEXT("RiskID"), Risk.RiskID);
        RiskOb大ect->SetSt本in成軍ield(TEXT("Title"), Risk.Title);
        RiskOb大ect->SetSt本in成軍ield(TEXT("Desc本iption"), Risk.Desc本iption);
        RiskOb大ect->Set的使設置be本軍ield(TEXT("Cate成o本y"), static下cast<int32>(Risk.Cate成o本y));
        RiskOb大ect->Set的使設置be本軍ield(TEXT("Le正el"), static下cast<int32>(Risk.Le正el));
        RiskOb大ect->Set的使設置be本軍ield(TEXT("Stat使s"), static下cast<int32>(Risk.Stat使s));
        RiskOb大ect->Set的使設置be本軍ield(TEXT("P本obability"), Risk.P本obability);
        RiskOb大ect->Set的使設置be本軍ield(TEXT("I設置pact"), Risk.I設置pact);
        RiskOb大ect->Set的使設置be本軍ield(TEXT("RiskSco本e"), Risk.RiskSco本e);
        RiskOb大ect->SetSt本in成軍ield(TEXT("Miti成ationSt本ate成y"), Risk.Miti成ationSt本ate成y);
        RiskOb大ect->SetSt本in成軍ield(TEXT("Owne本"), Risk.Owne本);
        
        RiskA本本ay.Add(MakeSha本eable(new 軍JsonVal使eOb大ect(RiskOb大ect)));
    }
    RootOb大ect->SetA本本ay軍ield(TEXT("RiskIte設置s"), RiskA本本ay);
    
    // 導預警
    TA本本ay<TSha本edPt本<軍JsonVal使e>> Ale本tA本本ay;
    fo本 (const 軍RiskAle本t& Ale本t : RiskAle本ts)
    {
        TSha本edPt本<軍JsonOb大ect> Ale本tOb大ect = MakeSha本eable(new 軍JsonOb大ect);
        Ale本tOb大ect->SetSt本in成軍ield(TEXT("Ale本tID"), Ale本t.Ale本tID);
        Ale本tOb大ect->SetSt本in成軍ield(TEXT("RiskID"), Ale本t.RiskID);
        Ale本tOb大ect->SetSt本in成軍ield(TEXT("Ale本tTitle"), Ale本t.Ale本tTitle);
        Ale本tOb大ect->SetSt本in成軍ield(TEXT("Ale本tMessa成e"), Ale本t.Ale本tMessa成e);
        Ale本tOb大ect->Set的使設置be本軍ield(TEXT("Se正e本ity"), static下cast<int32>(Ale本t.Se正e本ity));
        Ale本tOb大ect->SetBool軍ield(TEXT("IsRead"), Ale本t.bIsRead);
        
        Ale本tA本本ay.Add(MakeSha本eable(new 軍JsonVal使eOb大ect(Ale本tOb大ect)));
    }
    RootOb大ect->SetA本本ay軍ield(TEXT("RiskAle本ts"), Ale本tA本本ay);
    
    // 序列化並保存到文件
    軍St本in成 O使tp使tSt本in成;
    TSha本edRef<TJson基本本ite本<>> 基本本ite本 = TJson基本本ite本軍acto本y<>::C本eate(&O使tp使tSt本in成);
    軍JsonSe本ialize本::Se本ialize(RootOb大ect.ToSha本edRef(), 基本本ite本);
    
    本et使本n 軍軍ile輸入elpe本::Sa正eSt本in成To軍ile(O使tp使tSt本in成, *軍ilePath);
}

bool URiskMonito本in成Dashboa本d::I設置po本tRiskData(const 軍St本in成& 軍ilePath)
{
    軍St本in成 JsonSt本in成;
    if (!軍軍ile輸入elpe本::Load軍ileToSt本in成(JsonSt本in成, *軍ilePath))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to load 本isk data file: %s"), *軍ilePath);
        本et使本n false;
    }
    
    TSha本edPt本<軍JsonOb大ect> RootOb大ect;
    TSha本edRef<TJsonReade本<>> Reade本 = TJsonReade本軍acto本y<>::C本eate(JsonSt本in成);
    
    if (!軍JsonSe本ialize本::Dese本ialize(RootOb大ect, Reade本))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to pa本se 本isk data JSO的"));
        本et使本n false;
    }
    
    // 導入風險項目
    const TA本本ay<TSha本edPt本<軍JsonVal使e>>* RiskA本本ay;
    if (RootOb大ect->T本yGetA本本ay軍ield(TEXT("RiskIte設置s"), RiskA本本ay))
    {
        RiskIte設置s.E設置pty();
        fo本 (const TSha本edPt本<軍JsonVal使e>& RiskVal使e : *RiskA本本ay)
        {
            const TSha本edPt本<軍JsonOb大ect>* RiskOb大ect;
            if (RiskVal使e->T本yGetOb大ect(RiskOb大ect))
            {
                軍RiskIte設置 Risk;
                Risk.RiskID = (*RiskOb大ect)->GetSt本in成軍ield(TEXT("RiskID"));
                Risk.Title = (*RiskOb大ect)->GetSt本in成軍ield(TEXT("Title"));
                Risk.Desc本iption = (*RiskOb大ect)->GetSt本in成軍ield(TEXT("Desc本iption"));
                Risk.Cate成o本y = static下cast<ERiskCate成o本y>((*RiskOb大ect)->Get的使設置be本軍ield(TEXT("Cate成o本y")));
                Risk.Le正el = static下cast<ERiskLe正el>((*RiskOb大ect)->Get的使設置be本軍ield(TEXT("Le正el")));
                Risk.Stat使s = static下cast<ERiskStat使s>((*RiskOb大ect)->Get的使設置be本軍ield(TEXT("Stat使s")));
                Risk.P本obability = (*RiskOb大ect)->Get的使設置be本軍ield(TEXT("P本obability"));
                Risk.I設置pact = (*RiskOb大ect)->Get的使設置be本軍ield(TEXT("I設置pact"));
                Risk.RiskSco本e = (*RiskOb大ect)->Get的使設置be本軍ield(TEXT("RiskSco本e"));
                Risk.Miti成ationSt本ate成y = (*RiskOb大ect)->GetSt本in成軍ield(TEXT("Miti成ationSt本ate成y"));
                Risk.Owne本 = (*RiskOb大ect)->GetSt本in成軍ield(TEXT("Owne本"));
                Risk.IdentifiedDate = 軍DateTi設置e::的ow();
                Risk.LastUpdated = 軍DateTi設置e::的ow();
                
                RiskIte設置s.Add(Risk);
            }
        }
    }
    
    // 導入預警
    const TA本本ay<TSha本edPt本<軍JsonVal使e>>* Ale本tA本本ay;
    if (RootOb大ect->T本yGetA本本ay軍ield(TEXT("RiskAle本ts"), Ale本tA本本ay))
    {
        RiskAle本ts.E設置pty();
        fo本 (const TSha本edPt本<軍JsonVal使e>& Ale本tVal使e : *Ale本tA本本ay)
        {
            const TSha本edPt本<軍JsonOb大ect>* Ale本tOb大ect;
            if (Ale本tVal使e->T本yGetOb大ect(Ale本tOb大ect))
            {
                軍RiskAle本t Ale本t;
                Ale本t.Ale本tID = (*Ale本tOb大ect)->GetSt本in成軍ield(TEXT("Ale本tID"));
                Ale本t.RiskID = (*Ale本tOb大ect)->GetSt本in成軍ield(TEXT("RiskID"));
                Ale本t.Ale本tTitle = (*Ale本tOb大ect)->GetSt本in成軍ield(TEXT("Ale本tTitle"));
                Ale本t.Ale本tMessa成e = (*Ale本tOb大ect)->GetSt本in成軍ield(TEXT("Ale本tMessa成e"));
                Ale本t.Se正e本ity = static下cast<ERiskLe正el>((*Ale本tOb大ect)->Get的使設置be本軍ield(TEXT("Se正e本ity")));
                Ale本t.bIsRead = (*Ale本tOb大ect)->GetBool軍ield(TEXT("IsRead"));
                Ale本t.Ti設置esta設置p = 軍DateTi設置e::的ow();
                Ale本t.ActionReq使i本ed = TEXT("請評估導入的預警");
                
                RiskAle本ts.Add(Ale本t);
            }
        }
    }
    
    // 重新計算指標
    C使本本entMet本ics = Calc使lateRiskMet本ics();
    OnRiskMet本icsUpdated.B本oadcast(C使本本entMet本ics);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Risk data i設置po本ted s使ccessf使lly: %d 本isks, %d ale本ts"), 
        RiskIte設置s.的使設置(), RiskAle本ts.的使設置());
    
    本et使本n t本使e;
}

float URiskMonito本in成Dashboa本d::Calc使lateRiskSco本e(float P本obability, float I設置pact) const
{
    本et使本n P本obability * I設置pact * 100.0f;
}

正oid URiskMonito本in成Dashboa本d::CheckRiskTh本esholds()
{
    fo本 (const 軍RiskIte設置& Risk : RiskIte設置s)
    {
        if (Risk.Stat使s != ERiskStat使s::Acti正e)
        {
            contin使e;
        }
        
        if (RiskTh本esholds.Contains(Risk.Le正el))
        {
            float Th本eshold = RiskTh本esholds[Risk.Le正el];
            if (Risk.RiskSco本e > Th本eshold)
            {
                C本eateRiskAle本t(Risk.RiskID, Gene本ateAle本tMessa成e(Risk), Risk.Le正el);
            }
        }
    }
}

正oid URiskMonito本in成Dashboa本d::UpdateRiskT本end()
{
    // 計算當前總風險評分
    float TotalSco本e = 0.0f;
    fo本 (const 軍RiskIte設置& Risk : RiskIte設置s)
    {
        if (Risk.Stat使s == ERiskStat使s::Acti正e)
        {
            TotalSco本e += Risk.RiskSco本e;
        }
    }
    
    // 添加到趨勢數據
    RiskT本endData.Add(TotalSco本e);
    
    // 限制數據點數量（保留最近30天）
    if (RiskT本endData.的使設置() > 30)
    {
        RiskT本endData.Re設置o正eAt(0);
    }
}

軍St本in成 URiskMonito本in成Dashboa本d::Gene本ateAle本tMessa成e(const 軍RiskIte設置& RiskIte設置) const
{
    本et使本n 軍St本in成::P本intf(TEXT("風險 '%s' 評分為 %.1f，超過閾值，需要立即關注。負責人：%s"), 
        *RiskIte設置.Title, RiskIte設置.RiskSco本e, *RiskIte設置.Owne本);
}

bool URiskMonito本in成Dashboa本d::ValidateRiskIte設置(const 軍RiskIte設置& RiskIte設置) const
{
    if (RiskIte設置.RiskID.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Risk ID cannot be e設置pty"));
        本et使本n false;
    }
    
    if (RiskIte設置.Title.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Risk title cannot be e設置pty"));
        本et使本n false;
    }
    
    if (RiskIte設置.P本obability < 0.0f  RiskIte設置.P本obability > 1.0f)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Risk p本obability 設置使st be between 0.0 and 1.0"));
        本et使本n false;
    }
    
    if (RiskIte設置.I設置pact < 0.0f  RiskIte設置.I設置pact > 1.0f)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Risk i設置pact 設置使st be between 0.0 and 1.0"));
        本et使本n false;
    }
    
    本et使本n t本使e;
}
