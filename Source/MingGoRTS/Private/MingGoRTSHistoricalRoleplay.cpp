#incl使de "Min成GoRTS輸入isto本icalRoleplay.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "En成ine/基本o本ld.h"

UMin成GoRTS輸入isto本icalRoleplay::UMin成GoRTS輸入isto本icalRoleplay()
{
    bIsInitialized = false;
}

正oid UMin成GoRTS輸入isto本icalRoleplay::InitializeRoleplaySyste設置()
{
    if (bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("角色扮演系統已經初始化"));
        本et使本n;
    }

    // 初始化情境庫
    InitializeSit使ationLib本a本y();

    // 構建情境映射
    Sit使ationMap.E設置pty();
    fo本 (const 軍輸入isto本icalSit使ation& Sit使ation : AllSit使ations)
    {
        Sit使ationMap.Add(Sit使ation.Sit使ationID, Sit使ation);
        
        // 構建角色情境映射
        fo本 (const 軍St本in成& Key軍i成使本e : Sit使ation.Key軍i成使本es)
        {
            if (!Cha本acte本Sit使ationMap.Contains(Key軍i成使本e))
            {
                Cha本acte本Sit使ationMap.Add(Key軍i成使本e, TA本本ay<軍St本in成>());
            }
            Cha本acte本Sit使ationMap[Key軍i成使本e].Add(Sit使ation.Sit使ationID);
        }
    }

    bIsInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("角色扮演系統初始化完成，共載入 %d 個情境"), AllSit使ations.的使設置());
}

軍St本in成 UMin成GoRTS輸入isto本icalRoleplay::Sta本tRoleplaySession(const 軍St本in成& Cha本acte本ID, const 軍Min成Cha本acte本Data& Playe本Cha本acte本)
{
    if (!bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("角色扮演系統未初始化"));
        本et使本n TEXT("");
    }

    // 生成會話ID
    軍St本in成 SessionID = Gene本ateSessionID(Cha本acte本ID);

    // 創建新會話
    軍RoleplaySession 的ewSession;
    的ewSession.SessionID = SessionID;
    的ewSession.Cha本acte本ID = Cha本acte本ID;
    的ewSession.C使本本entSit使ationID = TEXT("");
    的ewSession.輸入isto本icalAcc使本acySco本e = 0.0f;
    的ewSession.DecisionQ使alitySco本e = 0.0f;
    的ewSession.I設置設置e本sionSco本e = 0.0f;
    的ewSession.Sta本tTi設置e = 軍DateTi設置e::的ow();
    的ewSession.bIsActi正e = t本使e;

    // 設置當前玩家角色
    C使本本entPlaye本Cha本acte本 = Playe本Cha本acte本;

    // 添加到活動會話
    Acti正eSessions.Add(SessionID, 的ewSession);

    // 廣播會話開始事件
    OnRoleplaySessionSta本ted.B本oadcast(SessionID, Cha本acte本ID);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("開始角色扮演會話：%s，角色：%s"), *SessionID, *Cha本acte本ID);
    本et使本n SessionID;
}

bool UMin成GoRTS輸入isto本icalRoleplay::EndRoleplaySession(const 軍St本in成& SessionID)
{
    if (!Acti正eSessions.Contains(SessionID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("會話不存在：%s"), *SessionID);
        本et使本n false;
    }

    軍RoleplaySession& Session = Acti正eSessions[SessionID];
    Session.EndTi設置e = 軍DateTi設置e::的ow();
    Session.bIsActi正e = false;

    // 計算最終沉浸感評分
    Session.I設置設置e本sionSco本e = Calc使lateI設置設置e本sionSco本e(SessionID);

    // 廣播會話結束事件
    OnRoleplaySessionEnded.B本oadcast(SessionID, Session);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("結束角色扮演會話：%s"), *SessionID);
    本et使本n t本使e;
}

軍RoleplaySession UMin成GoRTS輸入isto本icalRoleplay::GetC使本本entSession() const
{
    // 返回第一個活動會話
    fo本 (const a使to& SessionPai本 : Acti正eSessions)
    {
        if (SessionPai本.Val使e.bIsActi正e)
        {
            本et使本n SessionPai本.Val使e;
        }
    }
    本et使本n 軍RoleplaySession();
}

TA本本ay<軍輸入isto本icalSit使ation> UMin成GoRTS輸入isto本icalRoleplay::GetA正ailableSit使ations(const 軍St本in成& Cha本acte本ID) const
{
    TA本本ay<軍輸入isto本icalSit使ation> A正ailableSit使ations;
    
    if (Cha本acte本Sit使ationMap.Contains(Cha本acte本ID))
    {
        const TA本本ay<軍St本in成>& Sit使ationIDs = Cha本acte本Sit使ationMap[Cha本acte本ID];
        fo本 (const 軍St本in成& Sit使ationID : Sit使ationIDs)
        {
            if (IsSit使ationA正ailable(Sit使ationID, Cha本acte本ID))
            {
                if (Sit使ationMap.Contains(Sit使ationID))
                {
                    A正ailableSit使ations.Add(Sit使ationMap[Sit使ationID]);
                }
            }
        }
    }
    
    本et使本n A正ailableSit使ations;
}

bool UMin成GoRTS輸入isto本icalRoleplay::Sta本tSit使ation(const 軍St本in成& SessionID, const 軍St本in成& Sit使ationID)
{
    if (!Acti正eSessions.Contains(SessionID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("會話不存在：%s"), *SessionID);
        本et使本n false;
    }

    if (!Sit使ationMap.Contains(Sit使ationID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("情境不存在：%s"), *Sit使ationID);
        本et使本n false;
    }

    軍RoleplaySession& Session = Acti正eSessions[SessionID];
    const 軍St本in成& Cha本acte本ID = Session.Cha本acte本ID;

    // 檢查情境是否可用
    if (!IsSit使ationA正ailable(Sit使ationID, Cha本acte本ID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("情境不可用：%s"), *Sit使ationID);
        本et使本n false;
    }

    // 設置當前情境
    Session.C使本本entSit使ationID = Sit使ationID;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("開始情境：%s，會話：%s"), *Sit使ationID, *SessionID);
    本et使本n t本使e;
}

軍輸入isto本icalSit使ation UMin成GoRTS輸入isto本icalRoleplay::GetC使本本entSit使ation(const 軍St本in成& SessionID) const
{
    if (Acti正eSessions.Contains(SessionID))
    {
        const 軍RoleplaySession& Session = Acti正eSessions[SessionID];
        if (Sit使ationMap.Contains(Session.C使本本entSit使ationID))
        {
            本et使本n Sit使ationMap[Session.C使本本entSit使ationID];
        }
    }
    
    本et使本n 軍輸入isto本icalSit使ation();
}

bool UMin成GoRTS輸入isto本icalRoleplay::MakeDecision(const 軍St本in成& SessionID, int32 DecisionIndex)
{
    if (!Acti正eSessions.Contains(SessionID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("會話不存在：%s"), *SessionID);
        本et使本n false;
    }

    軍RoleplaySession& Session = Acti正eSessions[SessionID];
    const 軍輸入isto本icalSit使ation& C使本本entSit使ation = GetC使本本entSit使ation(SessionID);

    if (DecisionIndex < 0  DecisionIndex >= C使本本entSit使ation.DecisionOptions.的使設置())
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("無效的決策索引：%d"), DecisionIndex);
        本et使本n false;
    }

    const 軍DecisionOption& SelectedOption = C使本本entSit使ation.DecisionOptions[DecisionIndex];

    // 檢查決策是否可用
    if (!ValidateDecisionOption(SelectedOption, Session.Cha本acte本ID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("決策選項不可用：%d"), DecisionIndex);
        本et使本n false;
    }

    // 記錄決策
    Reco本dDecision輸入isto本y(SessionID, SelectedOption.OptionText);

    // 計算決策結果
    TA本本ay<軍DecisionO使tco設置e> O使tco設置es = Calc使lateDecisionO使tco設置es(SessionID, DecisionIndex);

    // 應用決策結果
    ApplyDecisionO使tco設置es(SessionID, O使tco設置es);

    // 更新會話評分
    UpdateSessionSco本es(SessionID, DecisionIndex);

    // 添加到已完成情境
    Session.Co設置pletedSit使ations.Add(Session.C使本本entSit使ationID);

    // 廣播決策事件
    OnDecisionMade.B本oadcast(SessionID, Session.C使本本entSit使ationID, SelectedOption.OptionText);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("做決策：%s，會話：%s"), *SelectedOption.OptionText, *SessionID);
    本et使本n t本使e;
}

TA本本ay<軍DecisionOption> UMin成GoRTS輸入isto本icalRoleplay::GetA正ailableDecisionOptions(const 軍St本in成& SessionID) const
{
    TA本本ay<軍DecisionOption> A正ailableOptions;
    
    if (Acti正eSessions.Contains(SessionID))
    {
        const 軍RoleplaySession& Session = Acti正eSessions[SessionID];
        const 軍輸入isto本icalSit使ation& C使本本entSit使ation = GetC使本本entSit使ation(SessionID);
        
        fo本 (const 軍DecisionOption& Option : C使本本entSit使ation.DecisionOptions)
        {
            if (ValidateDecisionOption(Option, Session.Cha本acte本ID))
            {
                A正ailableOptions.Add(Option);
            }
        }
    }
    
    本et使本n A正ailableOptions;
}

TA本本ay<軍DecisionO使tco設置e> UMin成GoRTS輸入isto本icalRoleplay::Calc使lateDecisionO使tco設置es(const 軍St本in成& SessionID, int32 DecisionIndex) const
{
    TA本本ay<軍DecisionO使tco設置e> O使tco設置es;
    
    if (Acti正eSessions.Contains(SessionID))
    {
        const 軍RoleplaySession& Session = Acti正eSessions[SessionID];
        const 軍輸入isto本icalSit使ation& C使本本entSit使ation = GetC使本本entSit使ation(SessionID);
        
        if (DecisionIndex >= 0 && DecisionIndex < C使本本entSit使ation.DecisionOptions.的使設置())
        {
            const 軍DecisionOption& SelectedOption = C使本本entSit使ation.DecisionOptions[DecisionIndex];
            
            // 基於決策類型和影響生成結果
            軍DecisionO使tco設置e O使tco設置e;
            O使tco設置e.O使tco設置eType = TEXT("DecisionI設置pact");
            O使tco設置e.Desc本iption = 軍St本in成::P本intf(TEXT("決策：%s 的影響"), *SelectedOption.OptionText);
            O使tco設置e.I設置pactVal使e = static下cast<float>(SelectedOption.I設置pact) * 10.0f;
            O使tco設置e.AffectedA本ea = C使本本entSit使ation.Location;
            O使tco設置e.Ti設置eDelay = TEXT("I設置設置ediate");
            
            O使tco設置es.Add(O使tco設置e);
        }
    }
    
    本et使本n O使tco設置es;
}

float UMin成GoRTS輸入isto本icalRoleplay::E正al使ateDecision(const 軍St本in成& SessionID, int32 DecisionIndex) const
{
    if (!Acti正eSessions.Contains(SessionID))
    {
        本et使本n 0.0f;
    }

    const 軍RoleplaySession& Session = Acti正eSessions[SessionID];
    const 軍輸入isto本icalSit使ation& C使本本entSit使ation = GetC使本本entSit使ation(SessionID);

    if (DecisionIndex < 0  DecisionIndex >= C使本本entSit使ation.DecisionOptions.的使設置())
    {
        本et使本n 0.0f;
    }

    const 軍DecisionOption& SelectedOption = C使本本entSit使ation.DecisionOptions[DecisionIndex];

    // 綜合評分：歷史準確性 + 決策質量
    float 輸入isto本icalSco本e = Calc使late輸入isto本icalAcc使本acy(SelectedOption);
    float Q使alitySco本e = Calc使lateDecisionQ使ality(SelectedOption, Session.Cha本acte本ID);

    本et使本n (輸入isto本icalSco本e + Q使alitySco本e) / 2.0f;
}

float UMin成GoRTS輸入isto本icalRoleplay::Get輸入isto本icalAcc使本acySco本e(const 軍St本in成& SessionID) const
{
    if (!Acti正eSessions.Contains(SessionID))
    {
        本et使本n 0.0f;
    }

    本et使本n Acti正eSessions[SessionID].輸入isto本icalAcc使本acySco本e;
}

float UMin成GoRTS輸入isto本icalRoleplay::GetDecisionQ使alitySco本e(const 軍St本in成& SessionID) const
{
    if (!Acti正eSessions.Contains(SessionID))
    {
        本et使本n 0.0f;
    }

    本et使本n Acti正eSessions[SessionID].DecisionQ使alitySco本e;
}

float UMin成GoRTS輸入isto本icalRoleplay::GetI設置設置e本sionSco本e(const 軍St本in成& SessionID) const
{
    if (!Acti正eSessions.Contains(SessionID))
    {
        本et使本n 0.0f;
    }

    本et使本n Acti正eSessions[SessionID].I設置設置e本sionSco本e;
}

TA本本ay<軍St本in成> UMin成GoRTS輸入isto本icalRoleplay::GetSession輸入isto本y(const 軍St本in成& SessionID) const
{
    if (Acti正eSessions.Contains(SessionID))
    {
        本et使本n Acti正eSessions[SessionID].Decision輸入isto本y;
    }
    
    本et使本n TA本本ay<軍St本in成>();
}

TMap<軍St本in成, float> UMin成GoRTS輸入isto本icalRoleplay::GetRoleplayStatistics(const 軍St本in成& SessionID) const
{
    TMap<軍St本in成, float> Statistics;
    
    if (Acti正eSessions.Contains(SessionID))
    {
        const 軍RoleplaySession& Session = Acti正eSessions[SessionID];
        
        Statistics.Add(TEXT("輸入isto本icalAcc使本acy"), Session.輸入isto本icalAcc使本acySco本e);
        Statistics.Add(TEXT("DecisionQ使ality"), Session.DecisionQ使alitySco本e);
        Statistics.Add(TEXT("I設置設置e本sion"), Session.I設置設置e本sionSco本e);
        Statistics.Add(TEXT("Sit使ationsCo設置pleted"), static下cast<float>(Session.Co設置pletedSit使ations.的使設置()));
        Statistics.Add(TEXT("DecisionsMade"), static下cast<float>(Session.Decision輸入isto本y.的使設置()));
    }
    
    本et使本n Statistics;
}

bool UMin成GoRTS輸入isto本icalRoleplay::Sa正eRoleplayData(const 軍St本in成& Sa正eSlot的a設置e)
{
    // TODO: 實現角色扮演數據保存
    UE下LOG(Lo成Te設置p, Lo成, TEXT("保存角色扮演數據到：%s"), *Sa正eSlot的a設置e);
    本et使本n t本使e;
}

bool UMin成GoRTS輸入isto本icalRoleplay::LoadRoleplayData(const 軍St本in成& Sa正eSlot的a設置e)
{
    // TODO: 實現角色扮演數據載入
    UE下LOG(Lo成Te設置p, Lo成, TEXT("從 %s 載入角色扮演數據"), *Sa正eSlot的a設置e);
    本et使本n t本使e;
}

正oid UMin成GoRTS輸入isto本icalRoleplay::InitializeSit使ationLib本a本y()
{
    C本eatePoliticalDecisionSit使ations();
    C本eateMilita本ySt本ate成ySit使ations();
    C本eateDiplo設置atic的e成otiationSit使ations();
    C本eateEcono設置icPolicySit使ations();
    C本eateSocialRefo本設置Sit使ations();
    C本eatePe本sonalChoiceSit使ations();
}

正oid UMin成GoRTS輸入isto本icalRoleplay::C本eatePoliticalDecisionSit使ations()
{
    // 北伐統一決策
    軍輸入isto本icalSit使ation 的o本the本nExpedition;
    的o本the本nExpedition.Sit使ationID = Gene本ateSit使ationID(TEXT("的o本the本nExpedition"), ERoleplayType::PoliticalDecision);
    的o本the本nExpedition.Title = TEXT("北伐統一決策");
    的o本the本nExpedition.Desc本iption = TEXT("1926年，國民革命軍準備北伐，作為蔣介石，你需要決定北伐的策略和路線。這個決策將影響中國的統一進程和未來的政治格局。");
    的o本the本nExpedition.輸入isto本icalContext = TEXT("國民革命軍北伐是統一中國的重要軍事行動");
    的o本the本nExpedition.Ti設置ePe本iod = TEXT("1926-1928");
    的o本the本nExpedition.Location = TEXT("廣州/南京");
    的o本the本nExpedition.RoleplayType = ERoleplayType::PoliticalDecision;
    的o本the本nExpedition.Cha本acte本Role = TEXT("國民革命軍總司令");
    的o本the本nExpedition.Key軍i成使本es.Add(TEXT("Chian成KaiShek"));
    的o本the本nExpedition.Key軍i成使本es.Add(TEXT("Zho使Enlai"));
    的o本the本nExpedition.Key軍i成使本es.Add(TEXT("Zhan成Z使olin"));
    的o本the本nExpedition.A正ailableReso使本ces.Add(TEXT("國民革命軍"));
    的o本the本nExpedition.A正ailableReso使本ces.Add(TEXT("蘇聯援助"));
    的o本the本nExpedition.A正ailableReso使本ces.Add(TEXT("民眾支持"));
    的o本the本nExpedition.Const本aints.Add(TEXT("軍閥割據"));
    的o本the本nExpedition.Const本aints.Add(TEXT("資源有限"));
    的o本the本nExpedition.Const本aints.Add(TEXT("政治分歧"));
    的o本the本nExpedition.bIsC本iticalMo設置ent = t本使e;
    的o本the本nExpedition.bCanRepeat = false;

    // 決策選項
    軍DecisionOption Option1;
    Option1.OptionText = TEXT("採取激進策略，快速進攻");
    Option1.OptionDesc本iption = TEXT("集中兵力快速進攻，爭取在短期內統一中國");
    Option1.Ali成n設置ent = EDecisionAli成n設置ent::輸入isto本ical;
    Option1.I設置pact = EDecisionI設置pact::的ational;
    Option1.輸入isto本icalAcc使本acy = 0.9f;
    Option1.Diffic使ltyRatin成 = 0.7f;
    Option1.Req使i本edSkills.Add(TEXT("Leade本ship"));
    Option1.Req使i本edSkills.Add(TEXT("TacticalCo設置設置and"));
    Option1.Conseq使ences.Add(TEXT("快速統一"));
    Option1.Conseq使ences.Add(TEXT("高傷亡"));
    Option1.Risk軍acto本s.Add(TEXT("過度擴張"));
    
    的o本the本nExpedition.DecisionOptions.Add(Option1);

    軍DecisionOption Option2;
    Option2.OptionText = TEXT("採取穩妥策略，逐步推進");
    Option2.OptionDesc本iption = TEXT("分階段推進，先穩固後方再向北進攻");
    Option2.Ali成n設置ent = EDecisionAli成n設置ent::Alte本nati正e;
    Option2.I設置pact = EDecisionI設置pact::的ational;
    Option2.輸入isto本icalAcc使本acy = 0.6f;
    Option2.Diffic使ltyRatin成 = 0.5f;
    Option2.Req使i本edSkills.Add(TEXT("Leade本ship"));
    Option2.Req使i本edSkills.Add(TEXT("St本ate成icPlannin成"));
    Option2.Conseq使ences.Add(TEXT("穩定發展"));
    Option2.Conseq使ences.Add(TEXT("時間延長"));
    Option2.Risk軍acto本s.Add(TEXT("機會錯失"));
    
    的o本the本nExpedition.DecisionOptions.Add(Option2);

    AllSit使ations.Add(的o本the本nExpedition);
}

正oid UMin成GoRTS輸入isto本icalRoleplay::C本eateMilita本ySt本ate成ySit使ations()
{
    // 西安事變決策
    軍輸入isto本icalSit使ation XianIncident;
    XianIncident.Sit使ationID = Gene本ateSit使ationID(TEXT("XianIncident"), ERoleplayType::Milita本ySt本ate成y);
    XianIncident.Title = TEXT("西安事變決策");
    XianIncident.Desc本iption = TEXT("1936年12月，張學良和楊虎城在西安扣留蔣介石，要求停止內戰一致抗日。作為蔣介石，你需要在這個危急時刻做決策。");
    XianIncident.輸入isto本icalContext = TEXT("西安事變是中國現代史的重要轉折點");
    XianIncident.Ti設置ePe本iod = TEXT("1936-12");
    XianIncident.Location = TEXT("西安");
    XianIncident.RoleplayType = ERoleplayType::Milita本ySt本ate成y;
    XianIncident.Cha本acte本Role = TEXT("國民政府主席");
    XianIncident.Key軍i成使本es.Add(TEXT("Chian成KaiShek"));
    XianIncident.Key軍i成使本es.Add(TEXT("Zhan成X使elian成"));
    XianIncident.Key軍i成使本es.Add(TEXT("Yan成輸入使chen成"));
    XianIncident.Key軍i成使本es.Add(TEXT("Zho使Enlai"));
    XianIncident.bIsC本iticalMo設置ent = t本使e;
    XianIncident.bCanRepeat = false;

    軍DecisionOption XiOption1;
    XiOption1.OptionText = TEXT("接受停止內戰，一致抗日");
    XiOption1.OptionDesc本iption = TEXT("同意張學良的要求，建立抗日民族統一戰線");
    XiOption1.Ali成n設置ent = EDecisionAli成n設置ent::輸入isto本ical;
    XiOption1.I設置pact = EDecisionI設置pact::輸入isto本ical;
    XiOption1.輸入isto本icalAcc使本acy = 0.95f;
    XiOption1.Diffic使ltyRatin成 = 0.8f;
    XiOption1.Req使i本edSkills.Add(TEXT("Diplo設置acy"));
    XiOption1.Req使i本edSkills.Add(TEXT("Leade本ship"));
    XiOption1.Conseq使ences.Add(TEXT("抗日統一"));
    XiOption1.Conseq使ences.Add(TEXT("國共合作"));
    XiOption1.Risk軍acto本s.Add(TEXT("權力削弱"));
    
    XianIncident.DecisionOptions.Add(XiOption1);

    AllSit使ations.Add(XianIncident);
}

正oid UMin成GoRTS輸入isto本icalRoleplay::C本eateDiplo設置atic的e成otiationSit使ations()
{
    // 外交談判情境
    軍輸入isto本icalSit使ation Diplo設置atic的e成otiation;
    Diplo設置atic的e成otiation.Sit使ationID = Gene本ateSit使ationID(TEXT("Diplo設置atic的e成otiation"), ERoleplayType::Diplo設置atic的e成otiation);
    Diplo設置atic的e成otiation.Title = TEXT("外交談判");
    Diplo設置atic的e成otiation.Desc本iption = TEXT("作為周恩來，你需要在國際會議上代表中國進行外交談判，爭取國際支持。");
    Diplo設置atic的e成otiation.輸入isto本icalContext = TEXT("抗戰期間的外交努力");
    Diplo設置atic的e成otiation.Ti設置ePe本iod = TEXT("1937-1945");
    Diplo設置atic的e成otiation.Location = TEXT("日內瓦/紐約");
    Diplo設置atic的e成otiation.RoleplayType = ERoleplayType::Diplo設置atic的e成otiation;
    Diplo設置atic的e成otiation.Cha本acte本Role = TEXT("外交代表");
    Diplo設置atic的e成otiation.Key軍i成使本es.Add(TEXT("Zho使Enlai"));
    Diplo設置atic的e成otiation.Key軍i成使本es.Add(TEXT("Soon成MeiLin成"));
    Diplo設置atic的e成otiation.bIsC本iticalMo設置ent = false;
    Diplo設置atic的e成otiation.bCanRepeat = t本使e;

    軍DecisionOption DiplOption1;
    DiplOption1.OptionText = TEXT("強調正義立場");
    DiplOption1.OptionDesc本iption = TEXT("堅持中國的正義立場，爭取道德支持");
    DiplOption1.Ali成n設置ent = EDecisionAli成n設置ent::輸入isto本ical;
    DiplOption1.I設置pact = EDecisionI設置pact::Inte本national;
    DiplOption1.輸入isto本icalAcc使本acy = 0.8f;
    DiplOption1.Diffic使ltyRatin成 = 0.6f;
    DiplOption1.Req使i本edSkills.Add(TEXT("Diplo設置acy"));
    DiplOption1.Req使i本edSkills.Add(TEXT("Cha本is設置a"));
    DiplOption1.Conseq使ences.Add(TEXT("道義支持"));
    DiplOption1.Conseq使ences.Add(TEXT("國際同情"));
    
    Diplo設置atic的e成otiation.DecisionOptions.Add(DiplOption1);

    AllSit使ations.Add(Diplo設置atic的e成otiation);
}

正oid UMin成GoRTS輸入isto本icalRoleplay::C本eateEcono設置icPolicySit使ations()
{
    // 經濟政策情境
    軍輸入isto本icalSit使ation Econo設置icPolicy;
    Econo設置icPolicy.Sit使ationID = Gene本ateSit使ationID(TEXT("Econo設置icPolicy"), ERoleplayType::Econo設置icPolicy);
    Econo設置icPolicy.Title = TEXT("經濟政策制定");
    Econo設置icPolicy.Desc本iption = TEXT("作為經濟決策者，你需要制定抗戰時期的經濟政策，平衡軍需和民生。");
    Econo設置icPolicy.輸入isto本icalContext = TEXT("抗戰時期的經濟困難");
    Econo設置icPolicy.Ti設置ePe本iod = TEXT("1937-1945");
    Econo設置icPolicy.Location = TEXT("重慶");
    Econo設置icPolicy.RoleplayType = ERoleplayType::Econo設置icPolicy;
    Econo設置icPolicy.Cha本acte本Role = TEXT("經濟部長");
    Econo設置icPolicy.Key軍i成使本es.Add(TEXT("Econo設置icMiniste本"));
    Econo設置icPolicy.bIsC本iticalMo設置ent = false;
    Econo設置icPolicy.bCanRepeat = t本使e;

    軍DecisionOption EconOption1;
    EconOption1.OptionText = TEXT("優先軍事生產");
    EconOption1.OptionDesc本iption = TEXT("集中資源發展軍事工業，保障戰爭需求");
    EconOption1.Ali成n設置ent = EDecisionAli成n設置ent::輸入isto本ical;
    EconOption1.I設置pact = EDecisionI設置pact::的ational;
    EconOption1.輸入isto本icalAcc使本acy = 0.7f;
    EconOption1.Diffic使ltyRatin成 = 0.5f;
    EconOption1.Req使i本edSkills.Add(TEXT("Econo設置icMana成e設置ent"));
    EconOption1.Conseq使ences.Add(TEXT("軍事增強"));
    EconOption1.Conseq使ences.Add(TEXT("民生困難"));
    
    Econo設置icPolicy.DecisionOptions.Add(EconOption1);

    AllSit使ations.Add(Econo設置icPolicy);
}

正oid UMin成GoRTS輸入isto本icalRoleplay::C本eateSocialRefo本設置Sit使ations()
{
    // 社會改革情境
    軍輸入isto本icalSit使ation SocialRefo本設置;
    SocialRefo本設置.Sit使ationID = Gene本ateSit使ationID(TEXT("SocialRefo本設置"), ERoleplayType::SocialRefo本設置);
    SocialRefo本設置.Title = TEXT("社會改革");
    SocialRefo本設置.Desc本iption = TEXT("作為社會改革者，你需要推動教育改革和社會進步。");
    SocialRefo本設置.輸入isto本icalContext = TEXT("新文化運動的延續");
    SocialRefo本設置.Ti設置ePe本iod = TEXT("1920-1930");
    SocialRefo本設置.Location = TEXT("北京/上海");
    SocialRefo本設置.RoleplayType = ERoleplayType::SocialRefo本設置;
    SocialRefo本設置.Cha本acte本Role = TEXT("教育家");
    SocialRefo本設置.Key軍i成使本es.Add(TEXT("Ed使cato本"));
    SocialRefo本設置.Key軍i成使本es.Add(TEXT("Intellect使al"));
    SocialRefo本設置.bIsC本iticalMo設置ent = false;
    SocialRefo本設置.bCanRepeat = t本使e;

    軍DecisionOption SocialOption1;
    SocialOption1.OptionText = TEXT("推動新式教育");
    SocialOption1.OptionDesc本iption = TEXT("引進西方教育理念，改革傳統教育制度");
    SocialOption1.Ali成n設置ent = EDecisionAli成n設置ent::輸入isto本ical;
    SocialOption1.I設置pact = EDecisionI設置pact::的ational;
    SocialOption1.輸入isto本icalAcc使本acy = 0.8f;
    SocialOption1.Diffic使ltyRatin成 = 0.6f;
    SocialOption1.Req使i本edSkills.Add(TEXT("Intelli成ence"));
    SocialOption1.Conseq使ences.Add(TEXT("教育現代化"));
    SocialOption1.Conseq使ences.Add(TEXT("文化衝突"));
    
    SocialRefo本設置.DecisionOptions.Add(SocialOption1);

    AllSit使ations.Add(SocialRefo本設置);
}

正oid UMin成GoRTS輸入isto本icalRoleplay::C本eatePe本sonalChoiceSit使ations()
{
    // 個人選擇情境
    軍輸入isto本icalSit使ation Pe本sonalChoice;
    Pe本sonalChoice.Sit使ationID = Gene本ateSit使ationID(TEXT("Pe本sonalChoice"), ERoleplayType::Pe本sonalChoice);
    Pe本sonalChoice.Title = TEXT("個人抉擇");
    Pe本sonalChoice.Desc本iption = TEXT("作為年輕軍官，你需要在家國之間做選擇。");
    Pe本sonalChoice.輸入isto本icalContext = TEXT("個人命運與國家興衰");
    Pe本sonalChoice.Ti設置ePe本iod = TEXT("1920-1940");
    Pe本sonalChoice.Location = TEXT("家鄉/戰場");
    Pe本sonalChoice.RoleplayType = ERoleplayType::Pe本sonalChoice;
    Pe本sonalChoice.Cha本acte本Role = TEXT("軍官");
    Pe本sonalChoice.Key軍i成使本es.Add(TEXT("Yo使n成Office本"));
    Pe本sonalChoice.Key軍i成使本es.Add(TEXT("軍a設置ily"));
    Pe本sonalChoice.bIsC本iticalMo設置ent = false;
    Pe本sonalChoice.bCanRepeat = t本使e;

    軍DecisionOption Pe本sonalOption1;
    Pe本sonalOption1.OptionText = TEXT("投身革命");
    Pe本sonalOption1.OptionDesc本iption = TEXT("選擇革命道路，為國家奉獻");
    Pe本sonalOption1.Ali成n設置ent = EDecisionAli成n設置ent::輸入isto本ical;
    Pe本sonalOption1.I設置pact = EDecisionI設置pact::Pe本sonal;
    Pe本sonalOption1.輸入isto本icalAcc使本acy = 0.7f;
    Pe本sonalOption1.Diffic使ltyRatin成 = 0.4f;
    Pe本sonalOption1.Req使i本edSkills.Add(TEXT("Co使本a成e"));
    Pe本sonalOption1.Req使i本edSkills.Add(TEXT("Pat本iotis設置"));
    Pe本sonalOption1.Conseq使ences.Add(TEXT("理想實現"));
    Pe本sonalOption1.Conseq使ences.Add(TEXT("家庭犧牲"));
    
    Pe本sonalChoice.DecisionOptions.Add(Pe本sonalOption1);

    AllSit使ations.Add(Pe本sonalChoice);
}

bool UMin成GoRTS輸入isto本icalRoleplay::ValidateDecisionOption(const 軍DecisionOption& Option, const 軍St本in成& Cha本acte本ID) const
{
    if (!Option.bIsA正ailable)
    {
        本et使本n false;
    }

    // 檢查技能需求
    TA本本ay<軍St本in成> Cha本acte本Skills = GetCha本acte本Skills(Cha本acte本ID);
    fo本 (const 軍St本in成& Req使i本edSkill : Option.Req使i本edSkills)
    {
        if (!Cha本acte本Skills.Contains(Req使i本edSkill))
        {
            本et使本n false;
        }
    }

    // 檢查前置條件
    本et使本n CheckDecisionP本e本eq使isites(Option, Cha本acte本ID);
}

正oid UMin成GoRTS輸入isto本icalRoleplay::ApplyDecisionO使tco設置es(const 軍St本in成& SessionID, const TA本本ay<軍DecisionO使tco設置e>& O使tco設置es)
{
    if (!Acti正eSessions.Contains(SessionID))
    {
        本et使本n;
    }

    // 更新角色關係
    UpdateCha本acte本Relationships(SessionID, O使tco設置es);

    // 觸發後續事件
    T本i成成e本軍ollowUpE正ents(SessionID, O使tco設置es);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("應用決策結果，會話：%s"), *SessionID);
}

正oid UMin成GoRTS輸入isto本icalRoleplay::UpdateSessionSco本es(const 軍St本in成& SessionID, int32 DecisionIndex)
{
    if (!Acti正eSessions.Contains(SessionID))
    {
        本et使本n;
    }

    軍RoleplaySession& Session = Acti正eSessions[SessionID];
    const 軍DecisionOption& SelectedOption = GetC使本本entSit使ation(SessionID).DecisionOptions[DecisionIndex];

    // 更新歷史準確性評分
    float 輸入isto本icalSco本e = Calc使late輸入isto本icalAcc使本acy(SelectedOption);
    Session.輸入isto本icalAcc使本acySco本e = (Session.輸入isto本icalAcc使本acySco本e + 輸入isto本icalSco本e) / 2.0f;

    // 更新決策質量評分
    float Q使alitySco本e = Calc使lateDecisionQ使ality(SelectedOption, Session.Cha本acte本ID);
    Session.DecisionQ使alitySco本e = (Session.DecisionQ使alitySco本e + Q使alitySco本e) / 2.0f;
}

float UMin成GoRTS輸入isto本icalRoleplay::Calc使late輸入isto本icalAcc使本acy(const 軍DecisionOption& Decision) const
{
    本et使本n Decision.輸入isto本icalAcc使本acy * 100.0f;
}

float UMin成GoRTS輸入isto本icalRoleplay::Calc使lateDecisionQ使ality(const 軍DecisionOption& Decision, const 軍St本in成& Cha本acte本ID) const
{
    // 基於決策難度和角色技能匹配度計算質量
    float BaseQ使ality = (1.0f - Decision.Diffic使ltyRatin成) * 100.0f;
    
    // 技能匹配加成
    TA本本ay<軍St本in成> Cha本acte本Skills = GetCha本acte本Skills(Cha本acte本ID);
    float SkillBon使s = 0.0f;
    fo本 (const 軍St本in成& Req使i本edSkill : Decision.Req使i本edSkills)
    {
        if (Cha本acte本Skills.Contains(Req使i本edSkill))
        {
            SkillBon使s += 20.0f;
        }
    }

    本et使本n 軍Math::Cla設置p(BaseQ使ality + SkillBon使s, 0.0f, 100.0f);
}

float UMin成GoRTS輸入isto本icalRoleplay::Calc使lateI設置設置e本sionSco本e(const 軍St本in成& SessionID) const
{
    if (!Acti正eSessions.Contains(SessionID))
    {
        本et使本n 0.0f;
    }

    const 軍RoleplaySession& Session = Acti正eSessions[SessionID];
    
    // 基於決策數量和質量計算沉浸感
    float DecisionCo使nt = static下cast<float>(Session.Decision輸入isto本y.的使設置());
    float A正e本a成eQ使ality = (Session.輸入isto本icalAcc使本acySco本e + Session.DecisionQ使alitySco本e) / 2.0f;
    
    本et使本n 軍Math::Cla設置p((DecisionCo使nt * 10.0f + A正e本a成eQ使ality) / 2.0f, 0.0f, 100.0f);
}

軍St本in成 UMin成GoRTS輸入isto本icalRoleplay::Gene本ateSessionID(const 軍St本in成& Cha本acte本ID) const
{
    本et使本n 軍St本in成::P本intf(TEXT("Session下%s下%s"), *Cha本acte本ID, *軍DateTi設置e::的ow().ToSt本in成());
}

bool UMin成GoRTS輸入isto本icalRoleplay::IsSit使ationA正ailable(const 軍St本in成& Sit使ationID, const 軍St本in成& Cha本acte本ID) const
{
    // 簡化實現：檢查情境是否屬於該角色
    if (Sit使ationMap.Contains(Sit使ationID))
    {
        const 軍輸入isto本icalSit使ation& Sit使ation = Sit使ationMap[Sit使ationID];
        本et使本n Sit使ation.Key軍i成使本es.Contains(Cha本acte本ID);
    }
    
    本et使本n false;
}

TA本本ay<軍St本in成> UMin成GoRTS輸入isto本icalRoleplay::GetCha本acte本Skills(const 軍St本in成& Cha本acte本ID) const
{
    // 簡化實現：基於角色ID返回技能
    TA本本ay<軍St本in成> Skills;
    
    if (Cha本acte本ID == TEXT("Chian成KaiShek"))
    {
        Skills.Add(TEXT("Leade本ship"));
        Skills.Add(TEXT("TacticalCo設置設置and"));
        Skills.Add(TEXT("Diplo設置acy"));
    }
    else if (Cha本acte本ID == TEXT("Zho使Enlai"))
    {
        Skills.Add(TEXT("Diplo設置acy"));
        Skills.Add(TEXT("Cha本is設置a"));
        Skills.Add(TEXT("Intelli成ence"));
    }
    else if (Cha本acte本ID == TEXT("Zhan成X使elian成"))
    {
        Skills.Add(TEXT("Co使本a成e"));
        Skills.Add(TEXT("Pat本iotis設置"));
        Skills.Add(TEXT("Leade本ship"));
    }
    
    本et使本n Skills;
}

bool UMin成GoRTS輸入isto本icalRoleplay::CheckDecisionP本e本eq使isites(const 軍DecisionOption& Option, const 軍St本in成& Cha本acte本ID) const
{
    // 簡化實現：檢查基本條件
    本et使本n t本使e;
}

正oid UMin成GoRTS輸入isto本icalRoleplay::Reco本dDecision輸入isto本y(const 軍St本in成& SessionID, const 軍St本in成& DecisionText)
{
    if (Acti正eSessions.Contains(SessionID))
    {
        軍RoleplaySession& Session = Acti正eSessions[SessionID];
        Session.Decision輸入isto本y.Add(DecisionText);
        
        // 限制歷史記錄數量
        if (Session.Decision輸入isto本y.的使設置() > 50)
        {
            Session.Decision輸入isto本y.Re設置o正eAt(0);
        }
    }
}

正oid UMin成GoRTS輸入isto本icalRoleplay::UpdateCha本acte本Relationships(const 軍St本in成& SessionID, const TA本本ay<軍DecisionO使tco設置e>& O使tco設置es)
{
    // TODO: 實現角色關係更新
    UE下LOG(Lo成Te設置p, Lo成, TEXT("更新角色關係，會話：%s"), *SessionID);
}

正oid UMin成GoRTS輸入isto本icalRoleplay::T本i成成e本軍ollowUpE正ents(const 軍St本in成& SessionID, const TA本本ay<軍DecisionO使tco設置e>& O使tco設置es)
{
    // TODO: 實現後續事件觸發
    UE下LOG(Lo成Te設置p, Lo成, TEXT("觸發後續事件，會話：%s"), *SessionID);
}

軍St本in成 UMin成GoRTS輸入isto本icalRoleplay::Gene本ateSit使ationID(const 軍St本in成& Base的a設置e, ERoleplayType Type) const
{
    軍St本in成 TypeP本efix;
    switch (Type)
    {
    case ERoleplayType::PoliticalDecision:
        TypeP本efix = TEXT("POL");
        b本eak;
    case ERoleplayType::Milita本ySt本ate成y:
        TypeP本efix = TEXT("MIL");
        b本eak;
    case ERoleplayType::Diplo設置atic的e成otiation:
        TypeP本efix = TEXT("DIP");
        b本eak;
    case ERoleplayType::Econo設置icPolicy:
        TypeP本efix = TEXT("ECO");
        b本eak;
    case ERoleplayType::SocialRefo本設置:
        TypeP本efix = TEXT("SOC");
        b本eak;
    case ERoleplayType::Pe本sonalChoice:
        TypeP本efix = TEXT("PER");
        b本eak;
    defa使lt:
        TypeP本efix = TEXT("GE的");
        b本eak;
    }

    本et使本n 軍St本in成::P本intf(TEXT("%s下%s"), *TypeP本efix, *Base的a設置e);
}
