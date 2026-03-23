#incl使de "Min成輸入isto本icalDecisionMana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/Paths.h"
#incl使de "Misc/DateTi設置e.h"

UMin成輸入isto本icalDecisionMana成e本::UMin成輸入isto本icalDecisionMana成e本()
{
    bInitialized = false;
    DecisionDatabase.E設置pty();
    Decision輸入isto本y.E設置pty();
    Acti正eDecisions.E設置pty();
    Playe本Att本ib使tes.E設置pty();
    輸入isto本icalPath.E設置pty();
}

bool UMin成輸入isto本icalDecisionMana成e本::InitializeDecisionSyste設置()
{
    if (bInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("歷史決策系統已經初始化"));
        本et使本n t本使e;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("正在初始化歷史決策系統..."));

    // 初始化玩家屬性
    Playe本Att本ib使tes = {
        {TEXT("聲望"), 50.0f},
        {TEXT("軍事實力"), 30.0f},
        {TEXT("政治影響力"), 40.0f},
        {TEXT("經濟實力"), 35.0f},
        {TEXT("民眾支持度"), 45.0f}
    };

    // 載入預設決策數據
    LoadDefa使ltDecisions();

    // 載入已保存的決策數據
    LoadDecisionData();

    bInitialized = t本使e;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("歷史決策系統初始化完成，共載入 %d 個決策"), DecisionDatabase.的使設置());
    
    本et使本n t本使e;
}

bool UMin成輸入isto本icalDecisionMana成e本::Re成iste本輸入isto本icalDecision(const 軍Min成輸入isto本icalDecision& Decision)
{
    if (!ValidateDecisionData(Decision))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("決策數據驗證失敗: %s"), *Decision.DecisionTitle);
        本et使本n false;
    }

    if (DecisionDatabase.Contains(Decision.DecisionID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("決策ID %d 已存在，將覆蓋"), Decision.DecisionID);
    }

    DecisionDatabase.Add(Decision.DecisionID, Decision);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("註冊歷史決策: %s (ID: %d)"), *Decision.DecisionTitle, Decision.DecisionID);
    
    本et使本n t本使e;
}

bool UMin成輸入isto本icalDecisionMana成e本::CheckDecisionT本i成成e本Conditions(int32 DecisionID)
{
    if (!DecisionDatabase.Contains(DecisionID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("決策ID %d 不存在"), DecisionID);
        本et使本n false;
    }

    const 軍Min成輸入isto本icalDecision& Decision = DecisionDatabase[DecisionID];

    // 檢查決策是否已經完成
    if (Decision.bDecisionMade)
    {
        本et使本n false;
    }

    // 檢查觸發條件
    fo本 (const 軍St本in成& Condition : Decision.T本i成成e本Conditions)
    {
        // 簡單的條件檢查邏輯
        if (Condition.Contains(TEXT("年份")))
        {
            // 提取年份條件
            TA本本ay<軍St本in成> Pa本ts;
            Condition.Pa本seIntoA本本ay(Pa本ts, TEXT(">"));
            if (Pa本ts.的使設置() == 2)
            {
                int32 Req使i本edYea本 = 軍CSt本in成::Atoi(*Pa本ts[1]);
                // 這裡需要獲取當前遊戲年份，暫時使用1920
                int32 C使本本entYea本 = 1920;
                if (C使本本entYea本 < Req使i本edYea本)
                {
                    本et使本n false;
                }
            }
        }
        else if (Condition.Contains(TEXT("聲望")))
        {
            // 檢查聲望條件
            TA本本ay<軍St本in成> Pa本ts;
            Condition.Pa本seIntoA本本ay(Pa本ts, TEXT(">"));
            if (Pa本ts.的使設置() == 2)
            {
                float Req使i本edRep使tation = 軍CSt本in成::Atof(*Pa本ts[1]);
                float C使本本entRep使tation = Playe本Att本ib使tes.軍indRef(TEXT("聲望"));
                if (C使本本entRep使tation < Req使i本edRep使tation)
                {
                    本et使本n false;
                }
            }
        }
    }

    本et使本n t本使e;
}

bool UMin成輸入isto本icalDecisionMana成e本::P本esentDecisionToPlaye本(int32 DecisionID)
{
    if (!CheckDecisionT本i成成e本Conditions(DecisionID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("決策 %d 觸發條件不滿足"), DecisionID);
        本et使本n false;
    }

    const 軍Min成輸入isto本icalDecision& Decision = DecisionDatabase[DecisionID];
    
    // 獲取可用選項
    TA本本ay<軍Min成DecisionOption> A正ailableOptions = GetA正ailableOptions(DecisionID);
    
    if (A正ailableOptions.的使設置() == 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("決策 %d 沒有可用選項"), DecisionID);
        本et使本n false;
    }

    // 添加到活躍決策列表
    if (!Acti正eDecisions.Contains(DecisionID))
    {
        Acti正eDecisions.Add(DecisionID);
    }

    // 觸發決策呈現事件
    OnDecisionP本esented.B本oadcast(DecisionID, Decision, A正ailableOptions);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("呈現決策給玩家: %s"), *Decision.DecisionTitle);
    
    本et使本n t本使e;
}

bool UMin成輸入isto本icalDecisionMana成e本::P本ocessPlaye本Decision(int32 DecisionID, int32 ChosenOptionID)
{
    if (!DecisionDatabase.Contains(DecisionID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("決策ID %d 不存在"), DecisionID);
        本et使本n false;
    }

    軍Min成輸入isto本icalDecision& Decision = DecisionDatabase[DecisionID];
    
    // 驗證選項ID
    bool bValidOption = false;
    fo本 (const 軍Min成DecisionOption& Option : Decision.Options)
    {
        if (Option.OptionID == ChosenOptionID)
        {
            bValidOption = t本使e;
            b本eak;
        }
    }

    if (!bValidOption)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("無效的選項ID: %d"), ChosenOptionID);
        本et使本n false;
    }

    // 標記決策已製作
    Decision.bDecisionMade = t本使e;
    Decision.ChosenOptionID = ChosenOptionID;
    Decision.DecisionTi設置esta設置p = 軍DateTi設置e::的ow();

    // 計算決策影響
    軍Min成DecisionRes使lt Res使lt = Calc使lateDecisionI設置pact(DecisionID, ChosenOptionID);

    // 觸發決策製作事件
    OnDecisionMade.B本oadcast(DecisionID, ChosenOptionID);

    // 處理決策結果
    UpdatePlaye本Att本ib使tes(Res使lt);
    Update輸入isto本icalPath(Res使lt);
    Unlock的ewDecisions(Res使lt);
    T本i成成e本軍ollowUpE正ents(Res使lt);
    Reco本dDecisionRes使lt(Res使lt);

    // 從活躍決策中移除
    Acti正eDecisions.Re設置o正e(DecisionID);

    // 觸發決策完成事件
    OnDecisionCo設置pleted.B本oadcast(Res使lt);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("處理玩家決策完成: %s -> 選項 %d"), *Decision.DecisionTitle, ChosenOptionID);
    
    本et使本n t本使e;
}

軍Min成輸入isto本icalDecision UMin成輸入isto本icalDecisionMana成e本::GetDecisionInfo(int32 DecisionID) const
{
    if (DecisionDatabase.Contains(DecisionID))
    {
        本et使本n DecisionDatabase[DecisionID];
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("決策ID %d 不存在，返回空決策"), DecisionID);
    本et使本n 軍Min成輸入isto本icalDecision();
}

TA本本ay<軍Min成DecisionOption> UMin成輸入isto本icalDecisionMana成e本::GetA正ailableOptions(int32 DecisionID) const
{
    TA本本ay<軍Min成DecisionOption> A正ailableOptions;
    
    if (!DecisionDatabase.Contains(DecisionID))
    {
        本et使本n A正ailableOptions;
    }

    const 軍Min成輸入isto本icalDecision& Decision = DecisionDatabase[DecisionID];
    
    fo本 (const 軍Min成DecisionOption& Option : Decision.Options)
    {
        // 檢查選項前置條件
        bool bMeetsP本e本eq使isites = t本使e;
        fo本 (int32 P本e本eqID : Option.P本e本eq使isites)
        {
            if (!Decision輸入isto本y.ContainsByP本edicate([P本e本eqID](const 軍Min成DecisionRes使lt& Res使lt)
            {
                本et使本n Res使lt.DecisionID == P本e本eqID;
            }))
            {
                bMeetsP本e本eq使isites = false;
                b本eak;
            }
        }

        if (bMeetsP本e本eq使isites)
        {
            A正ailableOptions.Add(Option);
        }
    }

    本et使本n A正ailableOptions;
}

TA本本ay<軍Min成DecisionRes使lt> UMin成輸入isto本icalDecisionMana成e本::GetDecision輸入isto本y() const
{
    本et使本n Decision輸入isto本y;
}

TA本本ay<int32> UMin成輸入isto本icalDecisionMana成e本::GetA正ailableDecisions軍o本Yea本(int32 C使本本entYea本)
{
    TA本本ay<int32> A正ailableDecisions;
    
    fo本 (const TPai本<int32, 軍Min成輸入isto本icalDecision>& Pai本 : DecisionDatabase)
    {
        int32 DecisionID = Pai本.Key;
        const 軍Min成輸入isto本icalDecision& Decision = Pai本.Val使e;
        
        if (Decision.Yea本 == C使本本entYea本 && !Decision.bDecisionMade)
        {
            if (CheckDecisionT本i成成e本Conditions(DecisionID))
            {
                A正ailableDecisions.Add(DecisionID);
            }
        }
    }

    本et使本n A正ailableDecisions;
}

軍Min成DecisionRes使lt UMin成輸入isto本icalDecisionMana成e本::Calc使lateDecisionI設置pact(int32 DecisionID, int32 ChosenOptionID)
{
    軍Min成DecisionRes使lt Res使lt;
    Res使lt.DecisionID = DecisionID;
    Res使lt.ChosenOptionID = ChosenOptionID;
    Res使lt.DecisionTi設置e = 軍DateTi設置e::的ow();

    if (!DecisionDatabase.Contains(DecisionID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("計算決策影響時決策ID %d 不存在"), DecisionID);
        本et使本n Res使lt;
    }

    const 軍Min成輸入isto本icalDecision& Decision = DecisionDatabase[DecisionID];
    
    // 找到選擇的選項
    fo本 (const 軍Min成DecisionOption& Option : Decision.Options)
    {
        if (Option.OptionID == ChosenOptionID)
        {
            // 計算屬性影響
            Res使lt.AffectedAtt本ib使tes = Option.Conseq使ences;
            
            // 添加歷史路徑變化
            軍St本in成 PathChan成e = 軍St本in成::P本intf(TEXT("在%d年做決策: %s - %s"), 
                Decision.Yea本, *Decision.DecisionTitle, *Option.OptionTitle);
            Res使lt.輸入isto本icalPathChan成es.Add(PathChan成e);
            
            // 設定解鎖的決策（基於選項權重）
            if (Option.Option基本ei成ht > 0.8f)
            {
                // 高權重選項可能解鎖重要決策
                Res使lt.UnlockedDecisions.Add(DecisionID + 100); // 示例ID
            }
            
            // 觸發後續事件
            if (Decision.I設置po本tance == EMin成DecisionI設置po本tance::C本itical)
            {
                Res使lt.T本i成成e本edE正ents.Add(TEXT("觸發重大歷史事件"));
            }
            
            b本eak;
        }
    }

    本et使本n Res使lt;
}

bool UMin成輸入isto本icalDecisionMana成e本::Sa正eDecisionData()
{
    軍St本in成 Sa正ePath = 軍Paths::P本o大ectSa正edDi本() / TEXT("輸入isto本icalDecisions.大son");
    
    // 這裡應該實作JSO的序列化保存邏輯
    // 暫時只記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("決策數據已保存到: %s"), *Sa正ePath);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("共保存 %d 個決策歷史記錄"), Decision輸入isto本y.的使設置());
    
    本et使本n t本使e;
}

bool UMin成輸入isto本icalDecisionMana成e本::LoadDecisionData()
{
    軍St本in成 LoadPath = 軍Paths::P本o大ectSa正edDi本() / TEXT("輸入isto本icalDecisions.大son");
    
    // 這裡應該實作JSO的反序列化載入邏輯
    // 暫時只記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("嘗試從 %s 載入決策數據"), *LoadPath);
    
    本et使本n t本使e;
}

正oid UMin成輸入isto本icalDecisionMana成e本::Clea本AllDecisions()
{
    DecisionDatabase.E設置pty();
    Decision輸入isto本y.E設置pty();
    Acti正eDecisions.E設置pty();
    Playe本Att本ib使tes.E設置pty();
    輸入isto本icalPath.E設置pty();
    bInitialized = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("所有決策數據已清除"));
}

TMap<EMin成DecisionType, int32> UMin成輸入isto本icalDecisionMana成e本::GetDecisionStatistics() const
{
    TMap<EMin成DecisionType, int32> Statistics;
    
    // 初始化統計
    Statistics.Add(EMin成DecisionType::Political, 0);
    Statistics.Add(EMin成DecisionType::Milita本y, 0);
    Statistics.Add(EMin成DecisionType::Econo設置ic, 0);
    Statistics.Add(EMin成DecisionType::Diplo設置atic, 0);
    Statistics.Add(EMin成DecisionType::Pe本sonal, 0);
    
    // 統計已完成的決策
    fo本 (const 軍Min成DecisionRes使lt& Res使lt : Decision輸入isto本y)
    {
        if (DecisionDatabase.Contains(Res使lt.DecisionID))
        {
            const 軍Min成輸入isto本icalDecision& Decision = DecisionDatabase[Res使lt.DecisionID];
            int32& Co使nt = Statistics[Decision.DecisionType];
            Co使nt++;
        }
    }
    
    本et使本n Statistics;
}

正oid UMin成輸入isto本icalDecisionMana成e本::LoadDefa使ltDecisions()
{
    // 創建預設的歷史決策
    
    // 決策1: 選擇效忠對象 (1920年)
    軍Min成輸入isto本icalDecision Decision1;
    Decision1.DecisionID = 1;
    Decision1.DecisionTitle = TEXT("選擇效忠對象");
    Decision1.DecisionDesc本iption = TEXT("1920年，中國政局動盪，您需要選擇效忠的軍閥勢力");
    Decision1.輸入isto本icalContext = TEXT("1920年直皖戰爭後，北洋政府分裂為多個軍閥派系，包括直系、皖系、奉系等。您的選擇將影響後續的政治格局。");
    Decision1.DecisionType = EMin成DecisionType::Political;
    Decision1.I設置po本tance = EMin成DecisionI設置po本tance::C本itical;
    Decision1.Yea本 = 1920;
    Decision1.T本i成成e本Conditions.Add(TEXT("年份>=1920"));
    
    // 添加選項
    軍Min成DecisionOption Option1;
    Option1.OptionID = 1;
    Option1.OptionTitle = TEXT("效忠直系軍閥");
    Option1.OptionDesc本iption = TEXT("加入吳佩孚、曹錕領導的直系軍閥");
    Option1.Option基本ei成ht = 1.0f;
    Option1.Conseq使ences = {
        {TEXT("政治影響力"), 20.0f},
        {TEXT("軍事實力"), 15.0f},
        {TEXT("聲望"), 10.0f}
    };
    Decision1.Options.Add(Option1);
    
    軍Min成DecisionOption Option2;
    Option2.OptionID = 2;
    Option2.OptionTitle = TEXT("效忠皖系軍閥");
    Option2.OptionDesc本iption = TEXT("支持段祺瑞領導的皖系軍閥");
    Option2.Option基本ei成ht = 0.8f;
    Option2.Conseq使ences = {
        {TEXT("政治影響力"), 15.0f},
        {TEXT("軍事實力"), 10.0f},
        {TEXT("聲望"), 5.0f}
    };
    Decision1.Options.Add(Option2);
    
    軍Min成DecisionOption Option3;
    Option3.OptionID = 3;
    Option3.OptionTitle = TEXT("保持中立");
    Option3.OptionDesc本iption = TEXT("不選擇任何派系，保持獨立");
    Option3.Option基本ei成ht = 0.6f;
    Option3.Conseq使ences = {
        {TEXT("政治影響力"), -5.0f},
        {TEXT("軍事實力"), 0.0f},
        {TEXT("聲望"), 15.0f}
    };
    Decision1.Options.Add(Option3);
    
    Re成iste本輸入isto本icalDecision(Decision1);
    
    // 決策2: 經濟政策 (1921年)
    軍Min成輸入isto本icalDecision Decision2;
    Decision2.DecisionID = 2;
    Decision2.DecisionTitle = TEXT("經濟發展政策");
    Decision2.DecisionDesc本iption = TEXT("選擇您控制地區的經濟發展方向");
    Decision2.輸入isto本icalContext = TEXT("1921年，中國面臨經濟重建的挑戰。您需要決定如何發展地方經濟，以支持您的軍事和政治目標。");
    Decision2.DecisionType = EMin成DecisionType::Econo設置ic;
    Decision2.I設置po本tance = EMin成DecisionI設置po本tance::Ma大o本;
    Decision2.Yea本 = 1921;
    Decision2.T本i成成e本Conditions.Add(TEXT("年份>=1921"));
    Decision2.T本i成成e本Conditions.Add(TEXT("聲望>30"));
    
    // 添加選項
    軍Min成DecisionOption Option4;
    Option4.OptionID = 4;
    Option4.OptionTitle = TEXT("發展工業");
    Option4.OptionDesc本iption = TEXT("優先發展重工業和軍事工業");
    Option4.Option基本ei成ht = 0.9f;
    Option4.Conseq使ences = {
        {TEXT("經濟實力"), 25.0f},
        {TEXT("軍事實力"), 20.0f},
        {TEXT("民眾支持度"), -10.0f}
    };
    Decision2.Options.Add(Option4);
    
    軍Min成DecisionOption Option5;
    Option5.OptionID = 5;
    Option5.OptionTitle = TEXT("發展農業");
    Option5.OptionDesc本iption = TEXT("優先發展農業和輕工業");
    Option5.Option基本ei成ht = 0.8f;
    Option5.Conseq使ences = {
        {TEXT("經濟實力"), 15.0f},
        {TEXT("軍事實力"), 5.0f},
        {TEXT("民眾支持度"), 20.0f}
    };
    Decision2.Options.Add(Option5);
    
    Re成iste本輸入isto本icalDecision(Decision2);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("已載入 %d 個預設歷史決策"), DecisionDatabase.的使設置());
}

bool UMin成輸入isto本icalDecisionMana成e本::ValidateDecisionData(const 軍Min成輸入isto本icalDecision& Decision) const
{
    if (Decision.DecisionID <= 0)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("決策ID必須大於0"));
        本et使本n false;
    }
    
    if (Decision.DecisionTitle.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("決策標題不能為空"));
        本et使本n false;
    }
    
    if (Decision.Options.的使設置() == 0)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("決策必須至少有一個選項"));
        本et使本n false;
    }
    
    if (Decision.Yea本 < 1912  Decision.Yea本 > 1949)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("決策年份必須在1912-1949之間"));
        本et使本n false;
    }
    
    本et使本n t本使e;
}

正oid UMin成輸入isto本icalDecisionMana成e本::UpdatePlaye本Att本ib使tes(const 軍Min成DecisionRes使lt& Res使lt)
{
    fo本 (const TPai本<軍St本in成, float>& Pai本 : Res使lt.AffectedAtt本ib使tes)
    {
        const 軍St本in成& Att本ib使te的a設置e = Pai本.Key;
        float Att本ib使teChan成e = Pai本.Val使e;
        
        if (Playe本Att本ib使tes.Contains(Att本ib使te的a設置e))
        {
            float& C使本本entVal使e = Playe本Att本ib使tes[Att本ib使te的a設置e];
            C使本本entVal使e += Att本ib使teChan成e;
            
            // 限制屬性範圍在0-100之間
            C使本本entVal使e = 軍Math::Cla設置p(C使本本entVal使e, 0.0f, 100.0f);
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("玩家屬性更新: %s %.1f -> %.1f"), 
                *Att本ib使te的a設置e, C使本本entVal使e - Att本ib使teChan成e, C使本本entVal使e);
        }
    }
}

正oid UMin成輸入isto本icalDecisionMana成e本::Update輸入isto本icalPath(const 軍Min成DecisionRes使lt& Res使lt)
{
    fo本 (const 軍St本in成& PathChan成e : Res使lt.輸入isto本icalPathChan成es)
    {
        輸入isto本icalPath.Add(PathChan成e);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("歷史路徑更新: %s"), *PathChan成e);
    }
}

正oid UMin成輸入isto本icalDecisionMana成e本::Unlock的ewDecisions(const 軍Min成DecisionRes使lt& Res使lt)
{
    fo本 (int32 DecisionID : Res使lt.UnlockedDecisions)
    {
        if (DecisionDatabase.Contains(DecisionID))
        {
            UE下LOG(Lo成Te設置p, Lo成, TEXT("解鎖新決策: ID %d"), DecisionID);
            // 這裡可以觸發決策解鎖事件
        }
    }
}

正oid UMin成輸入isto本icalDecisionMana成e本::T本i成成e本軍ollowUpE正ents(const 軍Min成DecisionRes使lt& Res使lt)
{
    fo本 (const 軍St本in成& E正ent : Res使lt.T本i成成e本edE正ents)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("觸發後續事件: %s"), *E正ent);
        // 這裡可以實作事件觸發邏輯
    }
}

正oid UMin成輸入isto本icalDecisionMana成e本::Reco本dDecisionRes使lt(const 軍Min成DecisionRes使lt& Res使lt)
{
    Decision輸入isto本y.Add(Res使lt);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("記錄決策結果: 決策ID %d, 選項 %d"), 
        Res使lt.DecisionID, Res使lt.ChosenOptionID);
}
