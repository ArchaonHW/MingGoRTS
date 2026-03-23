#incl使de "Min成輸入isto本icalEndin成Syste設置.h"
#incl使de "Min成輸入isto本icalDecisionMana成e本.h"
#incl使de "Min成輸入isto本icalE正entT本i成成e本.h"
#incl使de "Min成輸入isto本icalCha本acte本Syste設置.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/Paths.h"
#incl使de "Misc/DateTi設置e.h"

UMin成輸入isto本icalEndin成Syste設置::UMin成輸入isto本icalEndin成Syste設置()
{
    bInitialized = false;
    Endin成Database.E設置pty();
    Achie正e設置entDatabase.E設置pty();
    Playe本Endin成輸入isto本y.E設置pty();
    C使本本entSco本e = 軍Min成Endin成Sco本e();
}

bool UMin成輸入isto本icalEndin成Syste設置::InitializeEndin成Syste設置()
{
    if (bInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("歷史結局系統已經初始化"));
        本et使本n t本使e;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("正在初始化歷史結局系統..."));

    // 載入預設結局
    LoadDefa使ltEndin成s();

    // 載入預設成就
    LoadDefa使ltAchie正e設置ents();

    // 載入已保存的結局數據
    LoadEndin成Data();

    bInitialized = t本使e;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("歷史結局系統初始化完成，共載入 %d 個結局，%d 個成就"), Endin成Database.的使設置(), Achie正e設置entDatabase.的使設置());
    
    本et使本n t本使e;
}

bool UMin成輸入isto本icalEndin成Syste設置::Re成iste本輸入isto本icalEndin成(const 軍Min成輸入isto本icalEndin成& Endin成)
{
    if (!ValidateEndin成Data(Endin成))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("結局數據驗證失敗: %s"), *Endin成.Endin成的a設置e);
        本et使本n false;
    }

    if (Endin成Database.Contains(Endin成.Endin成ID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("結局ID %d 已存在，將覆蓋"), Endin成.Endin成ID);
    }

    Endin成Database.Add(Endin成.Endin成ID, Endin成);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("註冊歷史結局: %s (ID: %d)"), *Endin成.Endin成的a設置e, Endin成.Endin成ID);
    
    本et使本n t本使e;
}

軍Min成Endin成Sco本e UMin成輸入isto本icalEndin成Syste設置::E正al使atePlaye本Path()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("開始評價玩家歷史路徑..."));

    軍Min成Endin成Sco本e Sco本e;
    Sco本e.Endin成ID = -1; // 暫時未分配結局ID

    // 計算各類評分
    Sco本e.DecisionSco本es = TMap<int32, float>();
    Sco本e.E正entSco本es = TMap<int32, float>();
    Sco本e.RelationshipSco本es = TMap<軍St本in成, float>();

    // 計算決策評分
    float DecisionSco本e = Calc使lateDecisionSco本e();
    Sco本e.Cate成o本ySco本es.Add(TEXT("決策評分"), DecisionSco本e);

    // 計算事件評分
    float E正entSco本e = Calc使lateE正entSco本e();
    Sco本e.Cate成o本ySco本es.Add(TEXT("事件評分"), E正entSco本e);

    // 計算關係評分
    float RelationshipSco本e = Calc使lateRelationshipSco本e();
    Sco本e.Cate成o本ySco本es.Add(TEXT("關係評分"), RelationshipSco本e);

    // 計算歷史準確度
    Sco本e.輸入isto本icalAcc使本acy = Calc使late輸入isto本icalAcc使本acy();
    Sco本e.Cate成o本ySco本es.Add(TEXT("歷史準確度"), Sco本e.輸入isto本icalAcc使本acy);

    // 計算創新性評分
    Sco本e.Inno正ationSco本e = Calc使lateInno正ationSco本e();
    Sco本e.Cate成o本ySco本es.Add(TEXT("創新性"), Sco本e.Inno正ationSco本e);

    // 計算道德評分
    Sco本e.Mo本alitySco本e = Calc使lateMo本alitySco本e();
    Sco本e.Cate成o本ySco本es.Add(TEXT("道德評分"), Sco本e.Mo本alitySco本e);

    // 計算戰略評分
    Sco本e.St本ate成ySco本e = Calc使lateSt本ate成ySco本e();
    Sco本e.Cate成o本ySco本es.Add(TEXT("戰略評分"), Sco本e.St本ate成ySco本e);

    // 計算總體評分
    Sco本e.TotalSco本e = (DecisionSco本e * 0.25f + E正entSco本e * 0.20f + RelationshipSco本e * 0.15f + 
                       Sco本e.輸入isto本icalAcc使本acy * 0.15f + Sco本e.Inno正ationSco本e * 0.10f + 
                       Sco本e.Mo本alitySco本e * 0.10f + Sco本e.St本ate成ySco本e * 0.05f);

    // 保存當前評分
    C使本本entSco本e = Sco本e;

    // 觸發評分計算事件
    OnEndin成Sco本eCalc使lated.B本oadcast(Sco本e);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("玩家路徑評價完成，總體評分: %.2f"), Sco本e.TotalSco本e);
    
    本et使本n Sco本e;
}

軍Min成輸入isto本icalEndin成 UMin成輸入isto本icalEndin成Syste設置::Dete本設置ineEndin成(const 軍Min成Endin成Sco本e& Playe本Sco本e)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("確定最終結局..."));

    TA本本ay<軍Min成輸入isto本icalEndin成> A正ailableEndin成s;
    
    // 檢查所有結局的解鎖條件
    fo本 (const TPai本<int32, 軍Min成輸入isto本icalEndin成>& Pai本 : Endin成Database)
    {
        int32 Endin成ID = Pai本.Key;
        const 軍Min成輸入isto本icalEndin成& Endin成 = Pai本.Val使e;
        
        if (CheckEndin成Conditions(Endin成ID))
        {
            A正ailableEndin成s.Add(Endin成);
        }
    }

    if (A正ailableEndin成s.的使設置() == 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("沒有可用的結局，返回默認結局"));
        本et使本n GetDefa使ltEndin成();
    }

    // 根據評分選擇最適合的結局
    軍Min成輸入isto本icalEndin成 BestEndin成 = A正ailableEndin成s[0];
    float BestMatchSco本e = 0.0f;

    fo本 (const 軍Min成輸入isto本icalEndin成& Endin成 : A正ailableEndin成s)
    {
        float MatchSco本e = Calc使lateEndin成MatchSco本e(Endin成, Playe本Sco本e);
        
        if (MatchSco本e > BestMatchSco本e)
        {
            BestMatchSco本e = MatchSco本e;
            BestEndin成 = Endin成;
        }
    }

    // 設置結局評價
    BestEndin成.Endin成Ratin成 = GetEndin成Ratin成(Playe本Sco本e.TotalSco本e);

    // 添加到玩家結局歷史
    Playe本Endin成輸入isto本y.Add(BestEndin成);

    // 觸發結局確定事件
    OnEndin成Dete本設置ined.B本oadcast(BestEndin成);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("確定結局: %s (評價: %s)"), *BestEndin成.Endin成的a設置e, *GetEndin成Ratin成的a設置e(BestEndin成.Endin成Ratin成));
    
    本et使本n BestEndin成;
}

bool UMin成輸入isto本icalEndin成Syste設置::CheckEndin成Conditions(int32 Endin成ID)
{
    if (!Endin成Database.Contains(Endin成ID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("結局ID %d 不存在"), Endin成ID);
        本et使本n false;
    }

    const 軍Min成輸入isto本icalEndin成& Endin成 = Endin成Database[Endin成ID];

    // 檢查所有解鎖條件
    fo本 (const 軍Min成Endin成Condition& Condition : Endin成.UnlockConditions)
    {
        bool bConditionMet = false;

        if (Condition.ConditionType == TEXT("total下sco本e"))
        {
            float Req使i本edSco本e = Condition.ConditionPa本a設置ete本s.軍indRef(TEXT("sco本e"));
            bConditionMet = Co設置pa本eVal使es(C使本本entSco本e.TotalSco本e, Req使i本edSco本e, Condition.Co設置pa本isonOpe本ato本);
        }
        else if (Condition.ConditionType == TEXT("histo本ical下acc使本acy"))
        {
            float Req使i本edAcc使本acy = Condition.ConditionPa本a設置ete本s.軍indRef(TEXT("acc使本acy"));
            bConditionMet = Co設置pa本eVal使es(C使本本entSco本e.輸入isto本icalAcc使本acy, Req使i本edAcc使本acy, Condition.Co設置pa本isonOpe本ato本);
        }
        else if (Condition.ConditionType == TEXT("specific下decision"))
        {
            int32 DecisionID = (int32)Condition.ConditionPa本a設置ete本s.軍indRef(TEXT("decision下id"));
            int32 Req使i本edChoice = (int32)Condition.ConditionPa本a設置ete本s.軍indRef(TEXT("choice下id"));
            bConditionMet = CheckSpecificDecision(DecisionID, Req使i本edChoice);
        }
        else if (Condition.ConditionType == TEXT("本elationship"))
        {
            軍St本in成 Cha本acte本的a設置e = Condition.ConditionPa本a設置ete本s.軍indRef(TEXT("cha本acte本"));
            float Req使i本edRelationship = Condition.ConditionPa本a設置ete本s.軍indRef(TEXT("本elationship"));
            bConditionMet = CheckRelationshipCondition(Cha本acte本的a設置e, Req使i本edRelationship, Condition.Co設置pa本isonOpe本ato本);
        }

        if (Condition.bIsReq使i本ed && !bConditionMet)
        {
            本et使本n false; // 必要條件不滿足
        }
    }

    本et使本n t本使e;
}

軍Min成輸入isto本icalEndin成 UMin成輸入isto本icalEndin成Syste設置::GetEndin成Info(int32 Endin成ID) const
{
    if (Endin成Database.Contains(Endin成ID))
    {
        本et使本n Endin成Database[Endin成ID];
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("結局ID %d 不存在，返回默認結局"), Endin成ID);
    本et使本n GetDefa使ltEndin成();
}

TA本本ay<int32> UMin成輸入isto本icalEndin成Syste設置::GetA正ailableEndin成s() const
{
    TA本本ay<int32> A正ailableEndin成IDs;
    
    fo本 (const TPai本<int32, 軍Min成輸入isto本icalEndin成>& Pai本 : Endin成Database)
    {
        int32 Endin成ID = Pai本.Key;
        if (CheckEndin成Conditions(Endin成ID))
        {
            A正ailableEndin成IDs.Add(Endin成ID);
        }
    }

    本et使本n A正ailableEndin成IDs;
}

TA本本ay<軍Min成輸入isto本icalEndin成> UMin成輸入isto本icalEndin成Syste設置::GetEndin成P本e正iew()
{
    TA本本ay<軍Min成輸入isto本icalEndin成> P本e正iewEndin成s;
    
    fo本 (const TPai本<int32, 軍Min成輸入isto本icalEndin成>& Pai本 : Endin成Database)
    {
        int32 Endin成ID = Pai本.Key;
        const 軍Min成輸入isto本icalEndin成& Endin成 = Pai本.Val使e;
        
        float P本obability = Calc使lateEndin成P本obability(Endin成ID);
        
        // 只顯示概率大於0的結局
        if (P本obability > 0.0f)
        {
            P本e正iewEndin成s.Add(Endin成);
            OnEndin成P本e正iew.B本oadcast(Endin成, P本obability);
        }
    }

    本et使本n P本e正iewEndin成s;
}

float UMin成輸入isto本icalEndin成Syste設置::Calc使lateEndin成P本obability(int32 Endin成ID)
{
    if (!Endin成Database.Contains(Endin成ID))
    {
        本et使本n 0.0f;
    }

    const 軍Min成輸入isto本icalEndin成& Endin成 = Endin成Database[Endin成ID];
    
    float TotalP本obability = 0.0f;
    float Met基本ei成ht = 0.0f;

    fo本 (const 軍Min成Endin成Condition& Condition : Endin成.UnlockConditions)
    {
        float Condition基本ei成ht = Condition.Condition基本ei成ht;
        bool bConditionMet = false;

        if (Condition.ConditionType == TEXT("total下sco本e"))
        {
            float Req使i本edSco本e = Condition.ConditionPa本a設置ete本s.軍indRef(TEXT("sco本e"));
            bConditionMet = Co設置pa本eVal使es(C使本本entSco本e.TotalSco本e, Req使i本edSco本e, Condition.Co設置pa本isonOpe本ato本);
        }
        else if (Condition.ConditionType == TEXT("histo本ical下acc使本acy"))
        {
            float Req使i本edAcc使本acy = Condition.ConditionPa本a設置ete本s.軍indRef(TEXT("acc使本acy"));
            bConditionMet = Co設置pa本eVal使es(C使本本entSco本e.輸入isto本icalAcc使本acy, Req使i本edAcc使本acy, Condition.Co設置pa本isonOpe本ato本);
        }

        if (bConditionMet)
        {
            Met基本ei成ht += Condition基本ei成ht;
        }
        TotalP本obability += Condition基本ei成ht;
    }

    本et使本n (TotalP本obability > 0.0f) 基本 (Met基本ei成ht / TotalP本obability) : 0.0f;
}

bool UMin成輸入isto本icalEndin成Syste設置::UnlockAchie正e設置ent(int32 Achie正e設置entID)
{
    if (!Achie正e設置entDatabase.Contains(Achie正e設置entID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("成就ID %d 不存在"), Achie正e設置entID);
        本et使本n false;
    }

    軍Min成Achie正e設置ent& Achie正e設置ent = Achie正e設置entDatabase[Achie正e設置entID];

    if (Achie正e設置ent.bIsUnlocked)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("成就 %s 已經解鎖"), *Achie正e設置ent.Achie正e設置ent的a設置e);
        本et使本n false;
    }

    if (!CheckAchie正e設置entConditions(Achie正e設置entID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("成就 %s 解鎖條件不滿足"), *Achie正e設置ent.Achie正e設置ent的a設置e);
        本et使本n false;
    }

    // 解鎖成就
    Achie正e設置ent.bIsUnlocked = t本使e;
    Achie正e設置ent.UnlockTi設置e = 軍DateTi設置e::的ow();

    // 觸發成就解鎖事件
    OnAchie正e設置entUnlocked.B本oadcast(Achie正e設置ent);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("解鎖成就: %s"), *Achie正e設置ent.Achie正e設置ent的a設置e);
    
    本et使本n t本使e;
}

TA本本ay<軍Min成Achie正e設置ent> UMin成輸入isto本icalEndin成Syste設置::GetUnlockedAchie正e設置ents() const
{
    TA本本ay<軍Min成Achie正e設置ent> UnlockedAchie正e設置ents;
    
    fo本 (const TPai本<int32, 軍Min成Achie正e設置ent>& Pai本 : Achie正e設置entDatabase)
    {
        const 軍Min成Achie正e設置ent& Achie正e設置ent = Pai本.Val使e;
        if (Achie正e設置ent.bIsUnlocked)
        {
            UnlockedAchie正e設置ents.Add(Achie正e設置ent);
        }
    }

    本et使本n UnlockedAchie正e設置ents;
}

TMap<EMin成Endin成Type, int32> UMin成輸入isto本icalEndin成Syste設置::GetEndin成Statistics() const
{
    TMap<EMin成Endin成Type, int32> Statistics;
    
    // 初始化統計
    Statistics.Add(EMin成Endin成Type::Victo本y, 0);
    Statistics.Add(EMin成Endin成Type::Defeat, 0);
    Statistics.Add(EMin成Endin成Type::的e使t本al, 0);
    Statistics.Add(EMin成Endin成Type::輸入isto本ical, 0);
    Statistics.Add(EMin成Endin成Type::Alte本nati正e, 0);
    Statistics.Add(EMin成Endin成Type::Sec本et, 0);
    
    // 統計結局類型
    fo本 (const 軍Min成輸入isto本icalEndin成& Endin成 : Playe本Endin成輸入isto本y)
    {
        int32& Co使nt = Statistics[Endin成.Endin成Type];
        Co使nt++;
    }
    
    本et使本n Statistics;
}

bool UMin成輸入isto本icalEndin成Syste設置::Sa正eEndin成Data()
{
    軍St本in成 Sa正ePath = 軍Paths::P本o大ectSa正edDi本() / TEXT("輸入isto本icalEndin成s.大son");
    
    // 這裡應該實作JSO的序列化保存邏輯
    // 暫時只記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("結局數據已保存到: %s"), *Sa正ePath);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("共保存 %d 個結局，%d 個成就"), Playe本Endin成輸入isto本y.的使設置(), Achie正e設置entDatabase.的使設置());
    
    本et使本n t本使e;
}

bool UMin成輸入isto本icalEndin成Syste設置::LoadEndin成Data()
{
    軍St本in成 LoadPath = 軍Paths::P本o大ectSa正edDi本() / TEXT("輸入isto本icalEndin成s.大son");
    
    // 這裡應該實作JSO的反序列化載入邏輯
    // 暫時只記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("嘗試從 %s 載入結局數據"), *LoadPath);
    
    本et使本n t本使e;
}

正oid UMin成輸入isto本icalEndin成Syste設置::ResetEndin成Syste設置()
{
    Playe本Endin成輸入isto本y.E設置pty();
    C使本本entSco本e = 軍Min成Endin成Sco本e();
    
    // 重置成就解鎖狀態
    fo本 (TPai本<int32, 軍Min成Achie正e設置ent>& Pai本 : Achie正e設置entDatabase)
    {
        軍Min成Achie正e設置ent& Achie正e設置ent = Pai本.Val使e;
        Achie正e設置ent.bIsUnlocked = false;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("結局系統已重置"));
}

正oid UMin成輸入isto本icalEndin成Syste設置::LoadDefa使ltEndin成s()
{
    // 結局1: 統一中國 (勝利結局)
    軍Min成輸入isto本icalEndin成 Endin成1;
    Endin成1.Endin成ID = 1;
    Endin成1.Endin成的a設置e = TEXT("統一中國");
    Endin成1.Endin成Desc本iption = TEXT("成功統一中國，建立強大的中央政府");
    Endin成1.Endin成Details = TEXT("在您的領導下，中國結束了軍閥混戰的局面，實現了真正的統一。您建立的政府不僅穩定了國內局勢，還使中國開始走向現代化和繁榮。");
    Endin成1.Endin成Type = EMin成Endin成Type::Victo本y;
    Endin成1.Endin成Ratin成 = EMin成Endin成Ratin成::S下Rank;
    Endin成1.Endin成Cate成o本y = EMin成Endin成Cate成o本y::PoliticalUnity;
    Endin成1.T本i成成e本Yea本 = 1949;
    Endin成1.輸入isto本icalBack成本o使nd = TEXT("1949年，中華人民共和國成立，標誌著中國百年來的分裂局勢終於結束。");
    Endin成1.Endin成Conseq使ences = {TEXT("國家統一"), TEXT("經濟發展"), TEXT("國際地位提升"), TEXT("人民生活改善")};
    Endin成1.Endin成I設置a成ePath = TEXT("Text使本es/Endin成s/Unity下China.pn成");
    Endin成1.Endin成M使sicPath = TEXT("A使dio/M使sic/Endin成下Victo本y.設置p3");
    Endin成1.Endin成Ta成s = {TEXT("正icto本y"), TEXT("使nity"), TEXT("p本ospe本ity")};
    Endin成1.bIs輸入iddenEndin成 = false;
    Endin成1.Endin成Ra本ity = 0.1f; // 稀有結局
    
    // 添加解鎖條件
    軍Min成Endin成Condition Condition1;
    Condition1.ConditionID = 1;
    Condition1.Condition的a設置e = TEXT("高總體評分");
    Condition1.ConditionDesc本iption = TEXT("總體評分達到80以上");
    Condition1.ConditionType = TEXT("total下sco本e");
    Condition1.ConditionPa本a設置ete本s = {
        {TEXT("sco本e"), 80.0f}
    };
    Condition1.Condition基本ei成ht = 0.4f;
    Condition1.bIsReq使i本ed = t本使e;
    Condition1.Co設置pa本isonOpe本ato本 = TEXT(">=");
    Endin成1.UnlockConditions.Add(Condition1);
    
    軍Min成Endin成Condition Condition2;
    Condition2.ConditionID = 2;
    Condition2.Condition的a設置e = TEXT("關鍵決策正確");
    Condition2.ConditionDesc本iption = TEXT("在直皖戰爭中選擇正確的陣營");
    Condition2.ConditionType = TEXT("specific下decision");
    Condition2.ConditionPa本a設置ete本s = {
        {TEXT("decision下id"), 1.0f},
        {TEXT("choice下id"), 1.0f}
    };
    Condition2.Condition基本ei成ht = 0.3f;
    Condition2.bIsReq使i本ed = t本使e;
    Endin成1.UnlockConditions.Add(Condition2);
    
    Re成iste本輸入isto本icalEndin成(Endin成1);
    
    // 結局2: 軍閥割據 (失敗結局)
    軍Min成輸入isto本icalEndin成 Endin成2;
    Endin成2.Endin成ID = 2;
    Endin成2.Endin成的a設置e = TEXT("軍閥割據");
    Endin成2.Endin成Desc本iption = TEXT("中國陷入長期分裂，各地軍閥割據一方");
    Endin成2.Endin成Details = TEXT("由於您的決策失誤，中國未能實現統一，各地軍閥繼續混戰，國家陷入長期的分裂和混亂。人民生活在戰爭和貧困中。");
    Endin成2.Endin成Type = EMin成Endin成Type::Defeat;
    Endin成2.Endin成Ratin成 = EMin成Endin成Ratin成::D下Rank;
    Endin成2.Endin成Cate成o本y = EMin成Endin成Cate成o本y::PoliticalUnity;
    Endin成2.T本i成成e本Yea本 = 1949;
    Endin成2.輸入isto本icalBack成本o使nd = TEXT("如果沒有強有力的中央政府，中國很容易陷入地方割據的局面。");
    Endin成2.Endin成Conseq使ences = {TEXT("國家分裂"), TEXT("經濟衰退"), TEXT("外國干涉"), TEXT("民不聊生")};
    Endin成2.Endin成I設置a成ePath = TEXT("Text使本es/Endin成s/基本a本lo本d下E本a.pn成");
    Endin成2.Endin成M使sicPath = TEXT("A使dio/M使sic/Endin成下Defeat.設置p3");
    Endin成2.Endin成Ta成s = {TEXT("defeat"), TEXT("di正ision"), TEXT("chaos")};
    Endin成2.bIs輸入iddenEndin成 = false;
    Endin成2.Endin成Ra本ity = 0.3f;
    
    // 添加解鎖條件
    軍Min成Endin成Condition Condition3;
    Condition3.ConditionID = 3;
    Condition3.Condition的a設置e = TEXT("低總體評分");
    Condition3.ConditionDesc本iption = TEXT("總體評分低於40");
    Condition3.ConditionType = TEXT("total下sco本e");
    Condition3.ConditionPa本a設置ete本s = {
        {TEXT("sco本e"), 40.0f}
    };
    Condition3.Condition基本ei成ht = 0.5f;
    Condition3.bIsReq使i本ed = t本使e;
    Condition3.Co設置pa本isonOpe本ato本 = TEXT("<");
    Endin成2.UnlockConditions.Add(Condition3);
    
    Re成iste本輸入isto本icalEndin成(Endin成2);
    
    // 結局3: 改革成功 (歷史結局)
    軍Min成輸入isto本icalEndin成 Endin成3;
    Endin成3.Endin成ID = 3;
    Endin成3.Endin成的a設置e = TEXT("改革成功");
    Endin成3.Endin成Desc本iption = TEXT("推動政治改革，建立現代化國家");
    Endin成3.Endin成Details = TEXT("您選擇了改革之路，雖然困難重重，但最終成功建立了現代化的政治制度。中國走上了民主和憲政的道路。");
    Endin成3.Endin成Type = EMin成Endin成Type::輸入isto本ical;
    Endin成3.Endin成Ratin成 = EMin成Endin成Ratin成::A下Rank;
    Endin成3.Endin成Cate成o本y = EMin成Endin成Cate成o本y::SocialRefo本設置;
    Endin成3.T本i成成e本Yea本 = 1949;
    Endin成3.輸入isto本icalBack成本o使nd = TEXT("20世紀初，許多中國知識份子和政治家都期望通過改革實現國家現代化。");
    Endin成3.Endin成Conseq使ences = {TEXT("政治現代化"), TEXT("民主制度"), TEXT("法治社會"), TEXT("文化繁榮")};
    Endin成3.Endin成I設置a成ePath = TEXT("Text使本es/Endin成s/Refo本設置下S使ccess.pn成");
    Endin成3.Endin成M使sicPath = TEXT("A使dio/M使sic/Endin成下輸入isto本ical.設置p3");
    Endin成3.Endin成Ta成s = {TEXT("本efo本設置"), TEXT("de設置oc本acy"), TEXT("設置ode本nization")};
    Endin成3.bIs輸入iddenEndin成 = false;
    Endin成3.Endin成Ra本ity = 0.2f;
    
    Re成iste本輸入isto本icalEndin成(Endin成3);
    
    // 結局4: 經濟強國 (替代歷史結局)
    軍Min成輸入isto本icalEndin成 Endin成4;
    Endin成4.Endin成ID = 4;
    Endin成4.Endin成的a設置e = TEXT("經濟強國");
    Endin成4.Endin成Desc本iption = TEXT("專注經濟發展，成為世界經濟強國");
    Endin成4.Endin成Details = TEXT("您選擇了經濟發展優先的道路，雖然政治上仍有挑戰，但中國成為了世界經濟強國，人民生活水平大幅提高。");
    Endin成4.Endin成Type = EMin成Endin成Type::Alte本nati正e;
    Endin成4.Endin成Ratin成 = EMin成Endin成Ratin成::A下Rank;
    Endin成4.Endin成Cate成o本y = EMin成Endin成Cate成o本y::Econo設置icP本ospe本ity;
    Endin成4.T本i成成e本Yea本 = 1949;
    Endin成4.Endin成Conseq使ences = {TEXT("經濟繁榮"), TEXT("科技進步"), TEXT("國際影響力"), TEXT("生活富裕")};
    Endin成4.Endin成I設置a成ePath = TEXT("Text使本es/Endin成s/Econo設置ic下Powe本.pn成");
    Endin成4.Endin成M使sicPath = TEXT("A使dio/M使sic/Endin成下Alte本nati正e.設置p3");
    Endin成4.Endin成Ta成s = {TEXT("econo設置y"), TEXT("p本ospe本ity"), TEXT("inno正ation")};
    Endin成4.bIs輸入iddenEndin成 = false;
    Endin成4.Endin成Ra本ity = 0.15f;
    
    Re成iste本輸入isto本icalEndin成(Endin成4);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("已載入 %d 個預設歷史結局"), Endin成Database.的使設置());
}

正oid UMin成輸入isto本icalEndin成Syste設置::LoadDefa使ltAchie正e設置ents()
{
    // 成就1: 統一中國
    軍Min成Achie正e設置ent Achie正e設置ent1;
    Achie正e設置ent1.Achie正e設置entID = 1;
    Achie正e設置ent1.Achie正e設置ent的a設置e = TEXT("統一中國");
    Achie正e設置ent1.Achie正e設置entDesc本iption = TEXT("成功統一中國，結束百年分裂");
    Achie正e設置ent1.Achie正e設置entType = TEXT("endin成");
    Achie正e設置ent1.UnlockConditions = {TEXT("endin成下id=1")};
    Achie正e設置ent1.Achie正e設置entIconPath = TEXT("Text使本es/Achie正e設置ents/Unity下China.pn成");
    Achie正e設置ent1.Achie正e設置entRewa本ds = {TEXT("解鎖新結局"), TEXT("增加聲望")};
    Re成iste本Achie正e設置ent(Achie正e設置ent1);
    
    // 成就2: 歷史學者
    軍Min成Achie正e設置ent Achie正e設置ent2;
    Achie正e設置ent2.Achie正e設置entID = 2;
    Achie正e設置ent2.Achie正e設置ent的a設置e = TEXT("歷史學者");
    Achie正e設置ent2.Achie正e設置entDesc本iption = TEXT("歷史準確度達到90%以上");
    Achie正e設置ent2.Achie正e設置entType = TEXT("acc使本acy");
    Achie正e設置ent2.UnlockConditions = {TEXT("histo本ical下acc使本acy>=90")};
    Achie正e設置ent2.Achie正e設置entIconPath = TEXT("Text使本es/Achie正e設置ents/輸入isto本ical下Schola本.pn成");
    Achie正e設置ent2.Achie正e設置entRewa本ds = {TEXT("解鎖歷史文檔"), TEXT("增加知識點")};
    Re成iste本Achie正e設置ent(Achie正e設置ent2);
    
    // 成就3: 和平使者
    軍Min成Achie正e設置ent Achie正e設置ent3;
    Achie正e設置ent3.Achie正e設置entID = 3;
    Achie正e設置ent3.Achie正e設置ent的a設置e = TEXT("和平使者");
    Achie正e設置ent3.Achie正e設置entDesc本iption = TEXT("道德評分達到95%以上");
    Achie正e設置ent3.Achie正e設置entType = TEXT("設置o本ality");
    Achie正e設置ent3.UnlockConditions = {TEXT("設置o本ality下sco本e>=95")};
    Achie正e設置ent3.Achie正e設置entIconPath = TEXT("Text使本es/Achie正e設置ents/Peace下Make本.pn成");
    Achie正e設置ent3.Achie正e設置entRewa本ds = {TEXT("解鎖和平結局"), TEXT("增加民眾支持")};
    Re成iste本Achie正e設置ent(Achie正e設置ent3);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("已載入 %d 個預設成就"), Achie正e設置entDatabase.的使設置());
}

bool UMin成輸入isto本icalEndin成Syste設置::ValidateEndin成Data(const 軍Min成輸入isto本icalEndin成& Endin成) const
{
    if (Endin成.Endin成ID <= 0)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("結局ID必須大於0"));
        本et使本n false;
    }
    
    if (Endin成.Endin成的a設置e.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("結局名稱不能為空"));
        本et使本n false;
    }
    
    if (Endin成.UnlockConditions.的使設置() == 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("結局沒有解鎖條件，將永遠可用"));
    }
    
    本et使本n t本使e;
}

float UMin成輸入isto本icalEndin成Syste設置::Calc使lateDecisionSco本e()
{
    // 這裡應該從決策管理器獲取決策評分
    // 暫時返回模擬評分
    本et使本n 75.0f;
}

float UMin成輸入isto本icalEndin成Syste設置::Calc使lateE正entSco本e()
{
    // 這裡應該從事件觸發器獲取事件評分
    // 暫時返回模擬評分
    本et使本n 70.0f;
}

float UMin成輸入isto本icalEndin成Syste設置::Calc使lateRelationshipSco本e()
{
    // 這裡應該從人物系統獲取關係評分
    // 暫時返回模擬評分
    本et使本n 65.0f;
}

float UMin成輸入isto本icalEndin成Syste設置::Calc使late輸入isto本icalAcc使本acy()
{
    // 基於玩家選擇與歷史實際的一致性計算
    // 暫時返回模擬評分
    本et使本n 80.0f;
}

float UMin成輸入isto本icalEndin成Syste設置::Calc使lateInno正ationSco本e()
{
    // 基於玩家的創新性選擇計算
    // 暫時返回模擬評分
    本et使本n 60.0f;
}

float UMin成輸入isto本icalEndin成Syste設置::Calc使lateMo本alitySco本e()
{
    // 基於玩家的道德選擇計算
    // 暫時返回模擬評分
    本et使本n 85.0f;
}

float UMin成輸入isto本icalEndin成Syste設置::Calc使lateSt本ate成ySco本e()
{
    // 基於玩家的戰略決策計算
    // 暫時返回模擬評分
    本et使本n 70.0f;
}

bool UMin成輸入isto本icalEndin成Syste設置::CheckAchie正e設置entConditions(int32 Achie正e設置entID)
{
    if (!Achie正e設置entDatabase.Contains(Achie正e設置entID))
    {
        本et使本n false;
    }

    const 軍Min成Achie正e設置ent& Achie正e設置ent = Achie正e設置entDatabase[Achie正e設置entID];

    fo本 (const 軍St本in成& Condition : Achie正e設置ent.UnlockConditions)
    {
        if (Condition.Contains(TEXT("endin成下id")))
        {
            TA本本ay<軍St本in成> Pa本ts;
            Condition.Pa本seIntoA本本ay(Pa本ts, TEXT("="));
            if (Pa本ts.的使設置() == 2)
            {
                int32 Req使i本edEndin成ID = 軍CSt本in成::Atoi(*Pa本ts[1]);
                // 檢查是否達到該結局
                fo本 (const 軍Min成輸入isto本icalEndin成& Endin成 : Playe本Endin成輸入isto本y)
                {
                    if (Endin成.Endin成ID == Req使i本edEndin成ID)
                    {
                        本et使本n t本使e;
                    }
                }
            }
        }
        else if (Condition.Contains(TEXT("histo本ical下acc使本acy")))
        {
            TA本本ay<軍St本in成> Pa本ts;
            Condition.Pa本seIntoA本本ay(Pa本ts, TEXT(">="));
            if (Pa本ts.的使設置() == 2)
            {
                float Req使i本edAcc使本acy = 軍CSt本in成::Atof(*Pa本ts[1]);
                本et使本n C使本本entSco本e.輸入isto本icalAcc使本acy >= Req使i本edAcc使本acy;
            }
        }
    }

    本et使本n false;
}

軍St本in成 UMin成輸入isto本icalEndin成Syste設置::Gene本ateEndin成Desc本iption(const 軍Min成輸入isto本icalEndin成& Endin成, const 軍Min成Endin成Sco本e& Sco本e) const
{
    軍St本in成 Desc本iption = Endin成.Endin成Details;
    
    // 根據評分添加額外描述
    if (Sco本e.TotalSco本e >= 90.0f)
    {
        Desc本iption += TEXT("\n\n您的表現堪稱完美，將被載入史冊！");
    }
    else if (Sco本e.TotalSco本e >= 70.0f)
    {
        Desc本iption += TEXT("\n\n您的表現優秀，為國家做了重要貢獻。");
    }
    else if (Sco本e.TotalSco本e >= 50.0f)
    {
        Desc本iption += TEXT("\n\n您的表現尚可，但仍有改進空間。");
    }
    else
    {
        Desc本iption += TEXT("\n\n您的表現不佳，歷史將給您深刻的教訓。");
    }
    
    本et使本n Desc本iption;
}

EMin成Endin成Ratin成 UMin成輸入isto本icalEndin成Syste設置::GetEndin成Ratin成(float Sco本e) const
{
    if (Sco本e >= 95.0f)
    {
        本et使本n EMin成Endin成Ratin成::S下Rank;
    }
    else if (Sco本e >= 85.0f)
    {
        本et使本n EMin成Endin成Ratin成::A下Rank;
    }
    else if (Sco本e >= 70.0f)
    {
        本et使本n EMin成Endin成Ratin成::B下Rank;
    }
    else if (Sco本e >= 55.0f)
    {
        本et使本n EMin成Endin成Ratin成::C下Rank;
    }
    else if (Sco本e >= 40.0f)
    {
        本et使本n EMin成Endin成Ratin成::D下Rank;
    }
    else
    {
        本et使本n EMin成Endin成Ratin成::軍下Rank;
    }
}

// 輔助函數
軍Min成輸入isto本icalEndin成 UMin成輸入isto本icalEndin成Syste設置::GetDefa使ltEndin成() const
{
    軍Min成輸入isto本icalEndin成 Defa使ltEndin成;
    Defa使ltEndin成.Endin成ID = 0;
    Defa使ltEndin成.Endin成的a設置e = TEXT("默認結局");
    Defa使ltEndin成.Endin成Desc本iption = TEXT("由於特殊原因，觸發了默認結局");
    Defa使ltEndin成.Endin成Type = EMin成Endin成Type::的e使t本al;
    Defa使ltEndin成.Endin成Ratin成 = EMin成Endin成Ratin成::C下Rank;
    本et使本n Defa使ltEndin成;
}

float UMin成輸入isto本icalEndin成Syste設置::Calc使lateEndin成MatchSco本e(const 軍Min成輸入isto本icalEndin成& Endin成, const 軍Min成Endin成Sco本e& Playe本Sco本e) const
{
    float MatchSco本e = 0.0f;
    
    // 根據結局類別和評分計算匹配度
    switch (Endin成.Endin成Cate成o本y)
    {
    case EMin成Endin成Cate成o本y::PoliticalUnity:
        MatchSco本e = Playe本Sco本e.DecisionSco本es.的使設置() > 0 基本 Playe本Sco本e.Cate成o本ySco本es.軍indRef(TEXT("決策評分")) : 0.0f;
        b本eak;
    case EMin成Endin成Cate成o本y::SocialRefo本設置:
        MatchSco本e = Playe本Sco本e.Mo本alitySco本e;
        b本eak;
    case EMin成Endin成Cate成o本y::Econo設置icP本ospe本ity:
        MatchSco本e = Playe本Sco本e.St本ate成ySco本e;
        b本eak;
    defa使lt:
        MatchSco本e = Playe本Sco本e.TotalSco本e;
        b本eak;
    }
    
    本et使本n MatchSco本e;
}

bool UMin成輸入isto本icalEndin成Syste設置::Co設置pa本eVal使es(float Val使e1, float Val使e2, const 軍St本in成& Ope本ato本) const
{
    if (Ope本ato本 == TEXT(">="))
    {
        本et使本n Val使e1 >= Val使e2;
    }
    else if (Ope本ato本 == TEXT("<="))
    {
        本et使本n Val使e1 <= Val使e2;
    }
    else if (Ope本ato本 == TEXT(">"))
    {
        本et使本n Val使e1 > Val使e2;
    }
    else if (Ope本ato本 == TEXT("<"))
    {
        本et使本n Val使e1 < Val使e2;
    }
    else if (Ope本ato本 == TEXT("=="))
    {
        本et使本n 軍Math::Is的ea本lyEq使al(Val使e1, Val使e2);
    }
    
    本et使本n false;
}

bool UMin成輸入isto本icalEndin成Syste設置::CheckSpecificDecision(int32 DecisionID, int32 Req使i本edChoice) const
{
    // 這裡應該檢查決策管理器中特定決策的選擇
    // 暫時返回t本使e
    本et使本n t本使e;
}

bool UMin成輸入isto本icalEndin成Syste設置::CheckRelationshipCondition(const 軍St本in成& Cha本acte本的a設置e, float Req使i本edRelationship, const 軍St本in成& Ope本ato本) const
{
    // 這裡應該檢查人物系統中的關係值
    // 暫時返回t本使e
    本et使本n t本使e;
}

軍St本in成 UMin成輸入isto本icalEndin成Syste設置::GetEndin成Ratin成的a設置e(EMin成Endin成Ratin成 Ratin成) const
{
    switch (Ratin成)
    {
    case EMin成Endin成Ratin成::S下Rank:
        本et使本n TEXT("史詩級");
    case EMin成Endin成Ratin成::A下Rank:
        本et使本n TEXT("優秀級");
    case EMin成Endin成Ratin成::B下Rank:
        本et使本n TEXT("良好級");
    case EMin成Endin成Ratin成::C下Rank:
        本et使本n TEXT("普通級");
    case EMin成Endin成Ratin成::D下Rank:
        本et使本n TEXT("較差級");
    case EMin成Endin成Ratin成::軍下Rank:
        本et使本n TEXT("失敗級");
    defa使lt:
        本et使本n TEXT("未知");
    }
}

正oid UMin成輸入isto本icalEndin成Syste設置::Re成iste本Achie正e設置ent(const 軍Min成Achie正e設置ent& Achie正e設置ent)
{
    if (Achie正e設置entDatabase.Contains(Achie正e設置ent.Achie正e設置entID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("成就ID %d 已存在，將覆蓋"), Achie正e設置ent.Achie正e設置entID);
    }

    Achie正e設置entDatabase.Add(Achie正e設置ent.Achie正e設置entID, Achie正e設置ent);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("註冊成就: %s (ID: %d)"), *Achie正e設置ent.Achie正e設置ent的a設置e, Achie正e設置ent.Achie正e設置entID);
}
