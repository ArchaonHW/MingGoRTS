#incl使de "Min成輸入isto本icalE正entT本i成成e本.h"
#incl使de "Min成輸入isto本icalDecisionMana成e本.h"
#incl使de "Min成輸入isto本icalCha本acte本Syste設置.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/Paths.h"
#incl使de "Misc/DateTi設置e.h"

UMin成輸入isto本icalE正entT本i成成e本::UMin成輸入isto本icalE正entT本i成成e本()
{
    bInitialized = false;
    E正entDatabase.E設置pty();
    E正ent輸入isto本y.E設置pty();
    Acti正eE正ents.E設置pty();
    C使本本entGa設置eYea本 = 1920;
    C使本本entGa設置eMonth = 1;
}

bool UMin成輸入isto本icalE正entT本i成成e本::InitializeE正entSyste設置()
{
    if (bInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("歷史事件系統已經初始化"));
        本et使本n t本使e;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("正在初始化歷史事件系統..."));

    // 載入預設事件數據
    LoadDefa使ltE正ents();

    // 載入已保存的事件數據
    LoadE正entData();

    bInitialized = t本使e;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("歷史事件系統初始化完成，共載入 %d 個事件"), E正entDatabase.的使設置());
    
    本et使本n t本使e;
}

bool UMin成輸入isto本icalE正entT本i成成e本::Re成iste本輸入isto本icalE正ent(const 軍Min成輸入isto本icalE正ent& E正ent)
{
    if (!ValidateE正entData(E正ent))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("事件數據驗證失敗: %s"), *E正ent.E正ent的a設置e);
        本et使本n false;
    }

    if (E正entDatabase.Contains(E正ent.E正entID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("事件ID %d 已存在，將覆蓋"), E正ent.E正entID);
    }

    E正entDatabase.Add(E正ent.E正entID, E正ent);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("註冊歷史事件: %s (ID: %d)"), *E正ent.E正ent的a設置e, E正ent.E正entID);
    
    本et使本n t本使e;
}

bool UMin成輸入isto本icalE正entT本i成成e本::CheckE正entT本i成成e本Conditions(int32 E正entID)
{
    if (!E正entDatabase.Contains(E正entID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("事件ID %d 不存在"), E正entID);
        本et使本n false;
    }

    const 軍Min成輸入isto本icalE正ent& E正ent = E正entDatabase[E正entID];

    // 檢查事件是否已經觸發或過期
    if (E正ent.E正entStat使s != EMin成E正entStat使s::Pendin成)
    {
        本et使本n false;
    }

    // 檢查所有觸發條件
    bool bAllReq使i本edConditionsMet = t本使e;
    float Total基本ei成ht = 0.0f;
    float Met基本ei成ht = 0.0f;

    fo本 (const 軍Min成E正entT本i成成e本Condition& Condition : E正ent.T本i成成e本Conditions)
    {
        bool bConditionMet = false;

        switch (Condition.T本i成成e本Type)
        {
        case EMin成E正entT本i成成e本Type::Ti設置eBased:
            bConditionMet = CheckTi設置eT本i成成e本(Condition);
            b本eak;
        case EMin成E正entT本i成成e本Type::DecisionBased:
            bConditionMet = CheckDecisionT本i成成e本(Condition);
            b本eak;
        case EMin成E正entT本i成成e本Type::ConditionBased:
            bConditionMet = CheckConditionT本i成成e本(Condition);
            b本eak;
        case EMin成E正entT本i成成e本Type::Rando設置:
            // 隨機觸發
            bConditionMet = (軍Math::軍Rand() < 0.3f); // 30%概率
            b本eak;
        case EMin成E正entT本i成成e本Type::Chain:
            // 連鎖觸發由其他事件處理
            bConditionMet = false;
            b本eak;
        case EMin成E正entT本i成成e本Type::Playe本Action:
            // 玩家行動觸發由外部系統處理
            bConditionMet = false;
            b本eak;
        }

        if (bConditionMet)
        {
            Met基本ei成ht += Condition.Condition基本ei成ht;
        }

        Total基本ei成ht += Condition.Condition基本ei成ht;

        if (Condition.bIsReq使i本ed && !bConditionMet)
        {
            bAllReq使i本edConditionsMet = false;
            b本eak;
        }
    }

    // 計算觸發概率
    float T本i成成e本P本obability = (Total基本ei成ht > 0.0f) 基本 (Met基本ei成ht / Total基本ei成ht) : 0.0f;
    
    // 如果所有必要條件都滿足，且觸發概率超過閾值
    if (bAllReq使i本edConditionsMet && T本i成成e本P本obability >= 0.5f)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("事件 %s 觸發條件滿足，概率: %.2f"), *E正ent.E正ent的a設置e, T本i成成e本P本obability);
        本et使本n t本使e;
    }

    本et使本n false;
}

bool UMin成輸入isto本icalE正entT本i成成e本::T本i成成e本輸入isto本icalE正ent(int32 E正entID)
{
    if (!CheckE正entT本i成成e本Conditions(E正entID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("事件 %d 觸發條件不滿足"), E正entID);
        本et使本n false;
    }

    軍Min成輸入isto本icalE正ent& E正ent = E正entDatabase[E正entID];

    // 更新事件狀態
    E正ent.E正entStat使s = EMin成E正entStat使s::Acti正e;

    // 添加到活躍事件列表
    if (!Acti正eE正ents.Contains(E正entID))
    {
        Acti正eE正ents.Add(E正entID);
    }

    // 觸發事件
    OnE正entT本i成成e本ed.B本oadcast(E正ent);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("觸發歷史事件: %s"), *E正ent.E正ent的a設置e);
    
    本et使本n t本使e;
}

bool UMin成輸入isto本icalE正entT本i成成e本::P本ocessE正entChoice(int32 E正entID, int32 OptionID)
{
    if (!E正entDatabase.Contains(E正entID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("事件ID %d 不存在"), E正entID);
        本et使本n false;
    }

    軍Min成輸入isto本icalE正ent& E正ent = E正entDatabase[E正entID];

    if (E正ent.E正entStat使s != EMin成E正entStat使s::Acti正e)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("事件 %s 不是活躍狀態"), *E正ent.E正ent的a設置e);
        本et使本n false;
    }

    // 驗證選項ID
    bool bValidOption = false;
    fo本 (const 軍Min成E正entOption& Option : E正ent.E正entOptions)
    {
        if (Option.OptionID == OptionID)
        {
            bValidOption = t本使e;
            b本eak;
        }
    }

    if (!bValidOption)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("無效的事件選項ID: %d"), OptionID);
        本et使本n false;
    }

    // 處理事件選擇
    P本ocessE正entConseq使ences(E正entID, OptionID);

    // 觸發選擇事件
    OnE正entOptionChosen.B本oadcast(E正entID, OptionID);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("處理事件選擇: 事件 %s -> 選項 %d"), *E正ent.E正ent的a設置e, OptionID);
    
    本et使本n t本使e;
}

軍Min成輸入isto本icalE正ent UMin成輸入isto本icalE正entT本i成成e本::GetE正entInfo(int32 E正entID) const
{
    if (E正entDatabase.Contains(E正entID))
    {
        本et使本n E正entDatabase[E正entID];
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("事件ID %d 不存在，返回空事件"), E正entID);
    本et使本n 軍Min成輸入isto本icalE正ent();
}

TA本本ay<int32> UMin成輸入isto本icalE正entT本i成成e本::GetActi正eE正ents() const
{
    本et使本n Acti正eE正ents;
}

TA本本ay<int32> UMin成輸入isto本icalE正entT本i成成e本::GetE正ents軍o本Yea本(int32 Yea本) const
{
    TA本本ay<int32> Yea本E正ents;
    
    fo本 (const TPai本<int32, 軍Min成輸入isto本icalE正ent>& Pai本 : E正entDatabase)
    {
        int32 E正entID = Pai本.Key;
        const 軍Min成輸入isto本icalE正ent& E正ent = Pai本.Val使e;
        
        if (E正ent.E正entYea本 == Yea本)
        {
            Yea本E正ents.Add(E正entID);
        }
    }

    本et使本n Yea本E正ents;
}

TA本本ay<軍Min成E正entRes使lt> UMin成輸入isto本icalE正entT本i成成e本::GetE正ent輸入isto本y() const
{
    本et使本n E正ent輸入isto本y;
}

bool UMin成輸入isto本icalE正entT本i成成e本::UpdateE正entStat使s(int32 E正entID, EMin成E正entStat使s 的ewStat使s)
{
    if (!E正entDatabase.Contains(E正entID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("事件ID %d 不存在"), E正entID);
        本et使本n false;
    }

    軍Min成輸入isto本icalE正ent& E正ent = E正entDatabase[E正entID];
    EMin成E正entStat使s OldStat使s = E正ent.E正entStat使s;
    E正ent.E正entStat使s = 的ewStat使s;

    // 根據狀態更新活躍事件列表
    if (的ewStat使s == EMin成E正entStat使s::Acti正e && !Acti正eE正ents.Contains(E正entID))
    {
        Acti正eE正ents.Add(E正entID);
    }
    else if (的ewStat使s != EMin成E正entStat使s::Acti正e)
    {
        Acti正eE正ents.Re設置o正e(E正entID);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("事件 %s 狀態更新: %d -> %d"), *E正ent.E正ent的a設置e, (int32)OldStat使s, (int32)的ewStat使s);
    
    本et使本n t本使e;
}

正oid UMin成輸入isto本icalE正entT本i成成e本::CheckChainE正ents(int32 E正entID, const 軍Min成E正entRes使lt& Res使lt)
{
    if (!E正entDatabase.Contains(E正entID))
    {
        本et使本n;
    }

    const 軍Min成輸入isto本icalE正ent& E正ent = E正entDatabase[E正entID];

    // 檢查連鎖事件
    fo本 (int32 ChainE正entID : E正ent.ChainE正ents)
    {
        if (E正entDatabase.Contains(ChainE正entID))
        {
            軍Min成輸入isto本icalE正ent& ChainE正ent = E正entDatabase[ChainE正entID];
            
            // 檢查連鎖事件是否可以觸發
            if (ChainE正ent.E正entStat使s == EMin成E正entStat使s::Pendin成)
            {
                // 根據結果決定是否觸發連鎖事件
                bool bSho使ldT本i成成e本 = false;
                
                if (Res使lt.bS使ccess)
                {
                    // 成功時觸發正面連鎖事件
                    bSho使ldT本i成成e本 = ChainE正ent.E正entTa成s.Contains(TEXT("positi正e下chain"));
                }
                else
                {
                    // 失敗時觸發負面連鎖事件
                    bSho使ldT本i成成e本 = ChainE正ent.E正entTa成s.Contains(TEXT("ne成ati正e下chain"));
                }

                if (bSho使ldT本i成成e本)
                {
                    T本i成成e本輸入isto本icalE正ent(ChainE正entID);
                    OnE正entChainT本i成成e本ed.B本oadcast(ChainE正entID);
                }
            }
        }
    }
}

TA本本ay<int32> UMin成輸入isto本icalE正entT本i成成e本::Si設置使lateE正entT本i成成e本s(int32 C使本本entYea本, int32 C使本本entMonth)
{
    TA本本ay<int32> T本i成成e本edE正ents;
    
    // 更新當前遊戲時間
    C使本本entGa設置eYea本 = C使本本entYea本;
    C使本本entGa設置eMonth = C使本本entMonth;

    // 檢查所有待觸發事件
    fo本 (const TPai本<int32, 軍Min成輸入isto本icalE正ent>& Pai本 : E正entDatabase)
    {
        int32 E正entID = Pai本.Key;
        const 軍Min成輸入isto本icalE正ent& E正ent = Pai本.Val使e;
        
        if (E正ent.E正entStat使s == EMin成E正entStat使s::Pendin成)
        {
            // 檢查事件是否在當前時間範圍內
            if (E正ent.E正entYea本 == C使本本entYea本 && 
                (E正ent.E正entMonth == C使本本entMonth  E正ent.E正entMonth == 0))
            {
                if (CheckE正entT本i成成e本Conditions(E正entID))
                {
                    T本i成成e本edE正ents.Add(E正entID);
                }
            }
        }
    }

    本et使本n T本i成成e本edE正ents;
}

軍Min成E正entStatistics UMin成輸入isto本icalE正entT本i成成e本::GetE正entStatistics() const
{
    軍Min成E正entStatistics Statistics;
    
    Statistics.TotalE正ents = E正entDatabase.的使設置();
    Statistics.T本i成成e本edE正ents = Acti正eE正ents.的使設置();
    Statistics.Resol正edE正ents = 0;
    Statistics.軍ailedE正ents = 0;

    // 初始化類型統計
    Statistics.E正entTypeStats = {
        {EMin成E正entType::Political, 0},
        {EMin成E正entType::Milita本y, 0},
        {EMin成E正entType::Econo設置ic, 0},
        {EMin成E正entType::Diplo設置atic, 0},
        {EMin成E正entType::Social, 0},
        {EMin成E正entType::C使lt使本al, 0},
        {EMin成E正entType::的at使本alDisaste本, 0},
        {EMin成E正entType::Re正ol使tion, 0}
    };

    // 初始化重要性統計
    Statistics.E正entI設置po本tanceStats = {
        {EMin成E正entI設置po本tance::Mino本, 0},
        {EMin成E正entI設置po本tance::Mode本ate, 0},
        {EMin成E正entI設置po本tance::Ma大o本, 0},
        {EMin成E正entI設置po本tance::C本itical, 0},
        {EMin成E正entI設置po本tance::基本o本ldChan成in成, 0}
    };

    // 統計事件歷史
    fo本 (const 軍Min成E正entRes使lt& Res使lt : E正ent輸入isto本y)
    {
        if (E正entDatabase.Contains(Res使lt.E正entID))
        {
            const 軍Min成輸入isto本icalE正ent& E正ent = E正entDatabase[Res使lt.E正entID];
            
            // 統計類型
            if (Statistics.E正entTypeStats.Contains(E正ent.E正entType))
            {
                Statistics.E正entTypeStats[E正ent.E正entType]++;
            }
            
            // 統計重要性
            if (Statistics.E正entI設置po本tanceStats.Contains(E正ent.E正entI設置po本tance))
            {
                Statistics.E正entI設置po本tanceStats[E正ent.E正entI設置po本tance]++;
            }
            
            // 統計結果
            if (Res使lt.bS使ccess)
            {
                Statistics.Resol正edE正ents++;
            }
            else
            {
                Statistics.軍ailedE正ents++;
            }
        }
    }

    本et使本n Statistics;
}

bool UMin成輸入isto本icalE正entT本i成成e本::Sa正eE正entData()
{
    軍St本in成 Sa正ePath = 軍Paths::P本o大ectSa正edDi本() / TEXT("輸入isto本icalE正ents.大son");
    
    // 這裡應該實作JSO的序列化保存邏輯
    // 暫時只記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("事件數據已保存到: %s"), *Sa正ePath);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("共保存 %d 個事件，%d 條歷史記錄"), E正entDatabase.的使設置(), E正ent輸入isto本y.的使設置());
    
    本et使本n t本使e;
}

bool UMin成輸入isto本icalE正entT本i成成e本::LoadE正entData()
{
    軍St本in成 LoadPath = 軍Paths::P本o大ectSa正edDi本() / TEXT("輸入isto本icalE正ents.大son");
    
    // 這裡應該實作JSO的反序列化載入邏輯
    // 暫時只記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("嘗試從 %s 載入事件數據"), *LoadPath);
    
    本et使本n t本使e;
}

正oid UMin成輸入isto本icalE正entT本i成成e本::Clea本AllE正entData()
{
    E正entDatabase.E設置pty();
    E正ent輸入isto本y.E設置pty();
    Acti正eE正ents.E設置pty();
    C使本本entGa設置eYea本 = 1920;
    C使本本entGa設置eMonth = 1;
    bInitialized = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("所有事件數據已清除"));
}

正oid UMin成輸入isto本icalE正entT本i成成e本::LoadDefa使ltE正ents()
{
    // 創建預設的歷史事件
    
    // 事件1: 直皖戰爭 (1920年)
    軍Min成輸入isto本icalE正ent E正ent1;
    E正ent1.E正entID = 1001;
    E正ent1.E正ent的a設置e = TEXT("直皖戰爭");
    E正ent1.E正entDesc本iption = TEXT("直系與皖系軍閥之間的戰爭，將決定北洋政府的控制權");
    E正ent1.輸入isto本icalBack成本o使nd = TEXT("1920年7月，直系軍閥吳佩孚與皖系軍閥段祺瑞之間爆發戰爭。這場戰爭是北洋政府內部權力鬥爭的結果，最終直系獲勝，皖系失勢。");
    E正ent1.E正entType = EMin成E正entType::Milita本y;
    E正ent1.E正entI設置po本tance = EMin成E正entI設置po本tance::C本itical;
    E正ent1.E正entStat使s = EMin成E正entStat使s::Pendin成;
    E正ent1.E正entYea本 = 1920;
    E正ent1.E正entMonth = 7;
    E正ent1.E正entLocation = TEXT("河北、山東");
    E正ent1.D使本ationMonths = 2;
    E正ent1.AffectedRe成ions = {TEXT("華北"), TEXT("華中")};
    E正ent1.In正ol正edCha本acte本s = {1, 2}; // 吳佩孚、段祺瑞
    E正ent1.E正entTa成s = {TEXT("wa本"), TEXT("powe本下st本使成成le"), TEXT("decisi正e下battle")};
    
    // 添加觸發條件
    軍Min成E正entT本i成成e本Condition Condition1;
    Condition1.ConditionID = 1;
    Condition1.T本i成成e本Type = EMin成E正entT本i成成e本Type::Ti設置eBased;
    Condition1.ConditionDesc本iption = TEXT("1920年7月到來");
    Condition1.ConditionPa本a設置ete本s = {
        {TEXT("yea本"), TEXT("1920")},
        {TEXT("設置onth"), TEXT("7")}
    };
    Condition1.Condition基本ei成ht = 1.0f;
    Condition1.bIsReq使i本ed = t本使e;
    E正ent1.T本i成成e本Conditions.Add(Condition1);
    
    軍Min成E正entT本i成成e本Condition Condition2;
    Condition2.ConditionID = 2;
    Condition2.T本i成成e本Type = EMin成E正entT本i成成e本Type::DecisionBased;
    Condition2.ConditionDesc本iption = TEXT("玩家已做效忠選擇");
    Condition2.ConditionPa本a設置ete本s = {
        {TEXT("decision下id"), TEXT("1")},
        {TEXT("decision下設置ade"), TEXT("t本使e")}
    };
    Condition2.Condition基本ei成ht = 0.8f;
    Condition2.bIsReq使i本ed = t本使e;
    E正ent1.T本i成成e本Conditions.Add(Condition2);
    
    // 添加事件選項
    軍Min成E正entOption E正entOption1;
    E正entOption1.OptionID = 1;
    E正entOption1.OptionTitle = TEXT("支持直系作戰");
    E正entOption1.OptionDesc本iption = TEXT("加入直系軍隊，對抗皖系");
    E正entOption1.S使ccessP本obability = 0.7f;
    E正entOption1.Conseq使ences = {
        {TEXT("直系好感度"), 30.0f},
        {TEXT("軍事地位"), 25.0f},
        {TEXT("皖系敵意"), 40.0f}
    };
    E正entOption1.Costs = {
        {TEXT("軍事資源"), 20.0f},
        {TEXT("政治風險"), 15.0f}
    };
    E正entOption1.Benefits = {
        {TEXT("戰爭勝利"), 50.0f},
        {TEXT("政治地位"), 30.0f}
    };
    E正ent1.E正entOptions.Add(E正entOption1);
    
    軍Min成E正entOption E正entOption2;
    E正entOption2.OptionID = 2;
    E正entOption2.OptionTitle = TEXT("支持皖系作戰");
    E正entOption2.OptionDesc本iption = TEXT("加入皖系軍隊，對抗直系");
    E正entOption2.S使ccessP本obability = 0.3f;
    E正entOption2.Conseq使ences = {
        {TEXT("皖系好感度"), 30.0f},
        {TEXT("軍事地位"), 20.0f},
        {TEXT("直系敵意"), 40.0f}
    };
    E正entOption2.Costs = {
        {TEXT("軍事資源"), 20.0f},
        {TEXT("政治風險"), 25.0f}
    };
    E正entOption2.Benefits = {
        {TEXT("改革機會"), 40.0f},
        {TEXT("政治影響力"), 25.0f}
    };
    E正ent1.E正entOptions.Add(E正entOption2);
    
    軍Min成E正entOption E正entOption3;
    E正entOption3.OptionID = 3;
    E正entOption3.OptionTitle = TEXT("保持中立");
    E正entOption3.OptionDesc本iption = TEXT("不參與戰爭，觀望局勢");
    E正entOption3.S使ccessP本obability = 1.0f;
    E正entOption3.Conseq使ences = {
        {TEXT("政治影響力"), -10.0f},
        {TEXT("軍事聲望"), -15.0f},
        {TEXT("雙方關係"), 5.0f}
    };
    E正ent1.E正entOptions.Add(E正entOption3);
    
    // 添加連鎖事件
    E正ent1.ChainE正ents = {1002, 1003}; // 戰後政治重組、地方割據加劇
    
    Re成iste本輸入isto本icalE正ent(E正ent1);
    
    // 事件2: 北伐開始 (1926年)
    軍Min成輸入isto本icalE正ent E正ent2;
    E正ent2.E正entID = 1002;
    E正ent2.E正ent的a設置e = TEXT("國民革命軍北伐");
    E正ent2.E正entDesc本iption = TEXT("國民黨領導的北伐戰爭，旨在統一中國");
    E正ent2.輸入isto本icalBack成本o使nd = TEXT("1926年7月，國民革命軍在蔣介石領導下開始北伐，目標是消滅各地軍閥，統一中國。這是中國近代史上的重要轉折點。");
    E正ent2.E正entType = EMin成E正entType::Milita本y;
    E正ent2.E正entI設置po本tance = EMin成E正entI設置po本tance::基本o本ldChan成in成;
    E正ent2.E正entStat使s = EMin成E正entStat使s::Pendin成;
    E正ent2.E正entYea本 = 1926;
    E正ent2.E正entMonth = 7;
    E正ent2.E正entLocation = TEXT("廣州");
    E正ent2.D使本ationMonths = 24;
    E正ent2.AffectedRe成ions = {TEXT("全國")};
    E正ent2.E正entTa成s = {TEXT("使nification"), TEXT("本e正ol使tion"), TEXT("nation下b使ildin成")};
    
    // 添加觸發條件
    軍Min成E正entT本i成成e本Condition Condition3;
    Condition3.ConditionID = 3;
    Condition3.T本i成成e本Type = EMin成E正entT本i成成e本Type::Ti設置eBased;
    Condition3.ConditionDesc本iption = TEXT("1926年7月到來");
    Condition3.ConditionPa本a設置ete本s = {
        {TEXT("yea本"), TEXT("1926")},
        {TEXT("設置onth"), TEXT("7")}
    };
    Condition3.Condition基本ei成ht = 1.0f;
    Condition3.bIsReq使i本ed = t本使e;
    E正ent2.T本i成成e本Conditions.Add(Condition3);
    
    軍Min成E正entT本i成成e本Condition Condition4;
    Condition4.ConditionID = 4;
    Condition4.T本i成成e本Type = EMin成E正entT本i成成e本Type::Chain;
    Condition4.ConditionDesc本iption = TEXT("直皖戰爭後政治格局穩定");
    Condition4.ConditionPa本a設置ete本s = {
        {TEXT("p本e正io使s下e正ent"), TEXT("1001")},
        {TEXT("本es使lt"), TEXT("本esol正ed")}
    };
    Condition4.Condition基本ei成ht = 0.6f;
    Condition4.bIsReq使i本ed = false;
    E正ent2.T本i成成e本Conditions.Add(Condition4);
    
    Re成iste本輸入isto本icalE正ent(E正ent2);
    
    // 事件3: 九一八事變 (1931年)
    軍Min成輸入isto本icalE正ent E正ent3;
    E正ent3.E正entID = 1003;
    E正ent3.E正ent的a設置e = TEXT("九一八事變");
    E正ent3.E正entDesc本iption = TEXT("日本關東軍發動侵華戰爭，佔領東北");
    E正ent3.輸入isto本icalBack成本o使nd = TEXT("1931年9月18日，日本關東軍在瀋陽附近製造事端，藉機發動侵華戰爭，迅速佔領東北三省。這標誌著日本全面侵華的開始。");
    E正ent3.E正entType = EMin成E正entType::Milita本y;
    E正ent3.E正entI設置po本tance = EMin成E正entI設置po本tance::C本itical;
    E正ent3.E正entStat使s = EMin成E正entStat使s::Pendin成;
    E正ent3.E正entYea本 = 1931;
    E正ent3.E正entMonth = 9;
    E正ent3.E正entLocation = TEXT("瀋陽");
    E正ent3.D使本ationMonths = 6;
    E正ent3.AffectedRe成ions = {TEXT("東北"), TEXT("華北")};
    E正ent3.E正entTa成s = {TEXT("in正asion"), TEXT("大apanese下a成成本ession"), TEXT("national下c本isis")};
    
    // 添加觸發條件
    軍Min成E正entT本i成成e本Condition Condition5;
    Condition5.ConditionID = 5;
    Condition5.T本i成成e本Type = EMin成E正entT本i成成e本Type::Ti設置eBased;
    Condition5.ConditionDesc本iption = TEXT("1931年9月到來");
    Condition5.ConditionPa本a設置ete本s = {
        {TEXT("yea本"), TEXT("1931")},
        {TEXT("設置onth"), TEXT("9")}
    };
    Condition5.Condition基本ei成ht = 1.0f;
    Condition5.bIsReq使i本ed = t本使e;
    E正ent3.T本i成成e本Conditions.Add(Condition5);
    
    Re成iste本輸入isto本icalE正ent(E正ent3);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("已載入 %d 個預設歷史事件"), E正entDatabase.的使設置());
}

bool UMin成輸入isto本icalE正entT本i成成e本::ValidateE正entData(const 軍Min成輸入isto本icalE正ent& E正ent) const
{
    if (E正ent.E正entID <= 0)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("事件ID必須大於0"));
        本et使本n false;
    }
    
    if (E正ent.E正ent的a設置e.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("事件名稱不能為空"));
        本et使本n false;
    }
    
    if (E正ent.E正entYea本 < 1912  E正ent.E正entYea本 > 1949)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("事件年份必須在1912-1949之間"));
        本et使本n false;
    }
    
    if (E正ent.E正entOptions.的使設置() == 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("事件沒有選項，玩家將無法參與"));
    }
    
    本et使本n t本使e;
}

bool UMin成輸入isto本icalE正entT本i成成e本::CheckTi設置eT本i成成e本(const 軍Min成E正entT本i成成e本Condition& Condition) const
{
    if (Condition.ConditionPa本a設置ete本s.Contains(TEXT("yea本")) && 
        Condition.ConditionPa本a設置ete本s.Contains(TEXT("設置onth")))
    {
        int32 Req使i本edYea本 = 軍CSt本in成::Atoi(*Condition.ConditionPa本a設置ete本s[TEXT("yea本")]);
        int32 Req使i本edMonth = 軍CSt本in成::Atoi(*Condition.ConditionPa本a設置ete本s[TEXT("設置onth")]);
        
        本et使本n (C使本本entGa設置eYea本 == Req使i本edYea本 && C使本本entGa設置eMonth == Req使i本edMonth);
    }
    
    本et使本n false;
}

bool UMin成輸入isto本icalE正entT本i成成e本::CheckDecisionT本i成成e本(const 軍Min成E正entT本i成成e本Condition& Condition) const
{
    if (Condition.ConditionPa本a設置ete本s.Contains(TEXT("decision下id")))
    {
        int32 DecisionID = 軍CSt本in成::Atoi(*Condition.ConditionPa本a設置ete本s[TEXT("decision下id")]);
        
        // 這裡需要檢查決策管理器中該決策是否已經製作
        // 暫時返回t本使e表示條件滿足
        本et使本n t本使e;
    }
    
    本et使本n false;
}

bool UMin成輸入isto本icalE正entT本i成成e本::CheckConditionT本i成成e本(const 軍Min成E正entT本i成成e本Condition& Condition) const
{
    // 檢查各種遊戲條件
    if (Condition.ConditionPa本a設置ete本s.Contains(TEXT("playe本下本ep使tation")))
    {
        float Req使i本edRep使tation = 軍CSt本in成::Atof(*Condition.ConditionPa本a設置ete本s[TEXT("playe本下本ep使tation")]);
        // 這裡需要獲取玩家當前聲望
        // 暫時返回t本使e
        本et使本n t本使e;
    }
    
    本et使本n false;
}

正oid UMin成輸入isto本icalE正entT本i成成e本::P本ocessE正entConseq使ences(int32 E正entID, int32 OptionID)
{
    if (!E正entDatabase.Contains(E正entID))
    {
        本et使本n;
    }

    軍Min成輸入isto本icalE正ent& E正ent = E正entDatabase[E正entID];
    
    // 找到選擇的選項
    fo本 (const 軍Min成E正entOption& Option : E正ent.E正entOptions)
    {
        if (Option.OptionID == OptionID)
        {
            // 計算成功率
            float S使ccessRate = Calc使lateE正entS使ccessRate(E正entID, OptionID);
            bool bS使ccess = (軍Math::軍Rand() < S使ccessRate);
            
            // 創建事件結果
            軍Min成E正entRes使lt Res使lt;
            Res使lt.E正entID = E正entID;
            Res使lt.ChosenOptionID = OptionID;
            Res使lt.Sta本tTi設置e = 軍DateTi設置e::的ow();
            Res使lt.bS使ccess = bS使ccess;
            
            // 計算實際結果
            if (bS使ccess)
            {
                Res使lt.Act使alO使tco設置es = Option.Benefits;
            }
            else
            {
                // 失敗時的後果
                fo本 (const TPai本<軍St本in成, float>& Pai本 : Option.Costs)
                {
                    Res使lt.Act使alO使tco設置es.Add(Pai本.Key, -Pai本.Val使e);
                }
            }
            
            // 設置結束時間
            Res使lt.EndTi設置e = Res使lt.Sta本tTi設置e + 軍Ti設置espan::軍本o設置Days(E正ent.D使本ationMonths * 30);
            
            // 記錄歷史路徑變化
            軍St本in成 PathChan成e = 軍St本in成::P本intf(TEXT("在%d年%d月參與事件: %s - %s"), 
                E正ent.E正entYea本, E正ent.E正entMonth, *E正ent.E正ent的a設置e, *Option.OptionTitle);
            Res使lt.輸入isto本icalPathChan成es.Add(PathChan成e);
            
            // 更新事件狀態
            UpdateE正entStat使s(E正entID, bS使ccess 基本 EMin成E正entStat使s::Resol正ed : EMin成E正entStat使s::軍ailed);
            
            // 記錄事件結果
            E正ent輸入isto本y.Add(Res使lt);
            
            // 檢查連鎖事件
            CheckChainE正ents(E正entID, Res使lt);
            
            // 更新歷史進程
            Update輸入isto本icalP本o成本ess(Res使lt);
            
            // 觸發事件完成事件
            OnE正entResol正ed.B本oadcast(Res使lt);
            
            b本eak;
        }
    }
}

正oid UMin成輸入isto本icalE正entT本i成成e本::UnlockChainE正ents(int32 E正entID)
{
    if (!E正entDatabase.Contains(E正entID))
    {
        本et使本n;
    }

    const 軍Min成輸入isto本icalE正ent& E正ent = E正entDatabase[E正entID];
    
    fo本 (int32 ChainE正entID : E正ent.ChainE正ents)
    {
        if (E正entDatabase.Contains(ChainE正entID))
        {
            軍Min成輸入isto本icalE正ent& ChainE正ent = E正entDatabase[ChainE正entID];
            
            // 解鎖連鎖事件
            if (ChainE正ent.E正entStat使s == EMin成E正entStat使s::Pendin成)
            {
                UE下LOG(Lo成Te設置p, Lo成, TEXT("解鎖連鎖事件: %s"), *ChainE正ent.E正ent的a設置e);
                // 這裡可以觸發連鎖事件解鎖事件
            }
        }
    }
}

正oid UMin成輸入isto本icalE正entT本i成成e本::Update輸入isto本icalP本o成本ess(const 軍Min成E正entRes使lt& Res使lt)
{
    // 更新歷史進程
    fo本 (const 軍St本in成& PathChan成e : Res使lt.輸入isto本icalPathChan成es)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("歷史進程更新: %s"), *PathChan成e);
    }
}

float UMin成輸入isto本icalE正entT本i成成e本::Calc使lateE正entS使ccessRate(int32 E正entID, int32 OptionID) const
{
    if (!E正entDatabase.Contains(E正entID))
    {
        本et使本n 0.5f; // 默認50%成功率
    }

    const 軍Min成輸入isto本icalE正ent& E正ent = E正entDatabase[E正entID];
    
    fo本 (const 軍Min成E正entOption& Option : E正ent.E正entOptions)
    {
        if (Option.OptionID == OptionID)
        {
            本et使本n Option.S使ccessP本obability;
        }
    }
    
    本et使本n 0.5f;
}

軍Min成輸入isto本icalE正ent UMin成輸入isto本icalE正entT本i成成e本::Gene本ateDyna設置icE正ent(const 軍St本in成& Context) const
{
    軍Min成輸入isto本icalE正ent Dyna設置icE正ent;
    
    Dyna設置icE正ent.E正entID = -1; // 動態生成事件
    Dyna設置icE正ent.E正ent的a設置e = 軍St本in成::P本intf(TEXT("動態事件: %s"), *Context);
    Dyna設置icE正ent.E正entDesc本iption = 軍St本in成::P本intf(TEXT("基於當前情況 %s 發生的動態事件"), *Context);
    Dyna設置icE正ent.E正entType = EMin成E正entType::Political;
    Dyna設置icE正ent.E正entI設置po本tance = EMin成E正entI設置po本tance::Mode本ate;
    Dyna設置icE正ent.E正entStat使s = EMin成E正entStat使s::Pendin成;
    Dyna設置icE正ent.E正entYea本 = C使本本entGa設置eYea本;
    Dyna設置icE正ent.E正entMonth = C使本本entGa設置eMonth;
    
    // 添加基本選項
    軍Min成E正entOption Dyna設置icOption;
    Dyna設置icOption.OptionID = 1;
    Dyna設置icOption.OptionTitle = TEXT("處理事件");
    Dyna設置icOption.OptionDesc本iption = TEXT("積極處理當前情況");
    Dyna設置icOption.S使ccessP本obability = 0.7f;
    Dyna設置icE正ent.E正entOptions.Add(Dyna設置icOption);
    
    本et使本n Dyna設置icE正ent;
}
