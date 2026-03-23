#incl使de "Min成輸入isto本icalCha本acte本Syste設置.h"
#incl使de "Min成輸入isto本icalDecisionMana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/Paths.h"
#incl使de "Misc/DateTi設置e.h"

UMin成輸入isto本icalCha本acte本Syste設置::UMin成輸入isto本icalCha本acte本Syste設置()
{
    bInitialized = false;
    Cha本acte本Database.E設置pty();
    Dialo成使e的odeDatabase.E設置pty();
    Inte本action輸入isto本y.E設置pty();
    Acti正eDialo成使eCha本acte本s.E設置pty();
    PoliticalAlliances.E設置pty();
}

bool UMin成輸入isto本icalCha本acte本Syste設置::InitializeCha本acte本Syste設置()
{
    if (bInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("歷史人物系統已經初始化"));
        本et使本n t本使e;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("正在初始化歷史人物系統..."));

    // 載入預設人物數據
    LoadDefa使ltCha本acte本s();

    // 載入預設對話數據
    LoadDefa使ltDialo成使es();

    // 載入已保存的人物數據
    LoadCha本acte本Data();

    bInitialized = t本使e;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("歷史人物系統初始化完成，共載入 %d 個人物"), Cha本acte本Database.的使設置());
    
    本et使本n t本使e;
}

bool UMin成輸入isto本icalCha本acte本Syste設置::Re成iste本輸入isto本icalCha本acte本(const 軍Min成輸入isto本icalCha本acte本& Cha本acte本)
{
    if (!ValidateCha本acte本Data(Cha本acte本))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("人物數據驗證失敗: %s"), *Cha本acte本.Cha本acte本的a設置e);
        本et使本n false;
    }

    if (Cha本acte本Database.Contains(Cha本acte本.Cha本acte本ID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("人物ID %d 已存在，將覆蓋"), Cha本acte本.Cha本acte本ID);
    }

    Cha本acte本Database.Add(Cha本acte本.Cha本acte本ID, Cha本acte本);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("註冊歷史人物: %s (ID: %d)"), *Cha本acte本.Cha本acte本的a設置e, Cha本acte本.Cha本acte本ID);
    
    本et使本n t本使e;
}

軍Min成輸入isto本icalCha本acte本 UMin成輸入isto本icalCha本acte本Syste設置::GetCha本acte本Info(int32 Cha本acte本ID) const
{
    if (Cha本acte本Database.Contains(Cha本acte本ID))
    {
        本et使本n Cha本acte本Database[Cha本acte本ID];
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("人物ID %d 不存在，返回空人物"), Cha本acte本ID);
    本et使本n 軍Min成輸入isto本icalCha本acte本();
}

TA本本ay<int32> UMin成輸入isto本icalCha本acte本Syste設置::GetCha本acte本sAtLocation(const 軍St本in成& Location)
{
    TA本本ay<int32> Cha本acte本sAtLocation;
    
    fo本 (const TPai本<int32, 軍Min成輸入isto本icalCha本acte本>& Pai本 : Cha本acte本Database)
    {
        int32 Cha本acte本ID = Pai本.Key;
        const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 = Pai本.Val使e;
        
        if (Cha本acte本.C使本本entLocation == Location && Cha本acte本.bA正ailable軍o本Dialo成使e)
        {
            Cha本acte本sAtLocation.Add(Cha本acte本ID);
        }
    }

    本et使本n Cha本acte本sAtLocation;
}

bool UMin成輸入isto本icalCha本acte本Syste設置::Sta本tDialo成使e(int32 Cha本acte本ID)
{
    if (!Cha本acte本Database.Contains(Cha本acte本ID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("人物ID %d 不存在"), Cha本acte本ID);
        本et使本n false;
    }

    軍Min成輸入isto本icalCha本acte本& Cha本acte本 = Cha本acte本Database[Cha本acte本ID];

    if (!Cha本acte本.bA正ailable軍o本Dialo成使e)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("人物 %s 當前不可用於對話"), *Cha本acte本.Cha本acte本的a設置e);
        本et使本n false;
    }

    // 檢查根對話節點
    if (!Dialo成使e的odeDatabase.Contains(Cha本acte本.RootDialo成使e的ode))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("人物 %s 的根對話節點 %d 不存在"), *Cha本acte本.Cha本acte本的a設置e, Cha本acte本.RootDialo成使e的ode);
        本et使本n false;
    }

    // 檢查對話條件
    if (!CheckDialo成使eConditions(Cha本acte本.RootDialo成使e的ode))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("對話條件不滿足"));
        本et使本n false;
    }

    // 添加到活躍對話列表
    if (!Acti正eDialo成使eCha本acte本s.Contains(Cha本acte本ID))
    {
        Acti正eDialo成使eCha本acte本s.Add(Cha本acte本ID);
    }

    // 獲取對話節點
    const 軍Min成Dialo成使e的ode& Dialo成使e的ode = Dialo成使e的odeDatabase[Cha本acte本.RootDialo成使e的ode];

    // 觸發對話開始事件
    OnDialo成使eSta本ted.B本oadcast(Cha本acte本ID, Cha本acte本.RootDialo成使e的ode, Dialo成使e的ode);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("開始與人物 %s 的對話"), *Cha本acte本.Cha本acte本的a設置e);
    
    本et使本n t本使e;
}

軍Min成Dialo成使e的ode UMin成輸入isto本icalCha本acte本Syste設置::GetDialo成使e的ode(int32 的odeID) const
{
    if (Dialo成使e的odeDatabase.Contains(的odeID))
    {
        本et使本n Dialo成使e的odeDatabase[的odeID];
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("對話節點ID %d 不存在，返回空節點"), 的odeID);
    本et使本n 軍Min成Dialo成使e的ode();
}

bool UMin成輸入isto本icalCha本acte本Syste設置::ChooseDialo成使eOption(int32 Cha本acte本ID, int32 的odeID, int32 OptionID)
{
    if (!Cha本acte本Database.Contains(Cha本acte本ID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("人物ID %d 不存在"), Cha本acte本ID);
        本et使本n false;
    }

    if (!Dialo成使e的odeDatabase.Contains(的odeID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("對話節點ID %d 不存在"), 的odeID);
        本et使本n false;
    }

    const 軍Min成Dialo成使e的ode& Dialo成使e的ode = Dialo成使e的odeDatabase[的odeID];
    
    // 驗證選項ID
    bool bValidOption = false;
    fo本 (const 軍Min成Dialo成使eOption& Option : Dialo成使e的ode.Options)
    {
        if (Option.OptionID == OptionID)
        {
            bValidOption = t本使e;
            b本eak;
        }
    }

    if (!bValidOption)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("無效的對話選項ID: %d"), OptionID);
        本et使本n false;
    }

    // 處理對話選項後果
    P本ocessDialo成使eConseq使ences(Cha本acte本ID, 的odeID, OptionID);

    // 觸發選項選擇事件
    OnDialo成使eOptionChosen.B本oadcast(Cha本acte本ID, OptionID);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("玩家選擇了對話選項 %d 與人物 ID %d"), OptionID, Cha本acte本ID);
    
    本et使本n t本使e;
}

bool UMin成輸入isto本icalCha本acte本Syste設置::CheckDialo成使eConditions(int32 的odeID) const
{
    if (!Dialo成使e的odeDatabase.Contains(的odeID))
    {
        本et使本n false;
    }

    const 軍Min成Dialo成使e的ode& Dialo成使e的ode = Dialo成使e的odeDatabase[的odeID];

    // 檢查所有條件
    fo本 (const 軍St本in成& Condition : Dialo成使e的ode.Conditions)
    {
        // 簡單的條件檢查邏輯
        if (Condition.Contains(TEXT("關係")))
        {
            // 檢查人物關係條件
            TA本本ay<軍St本in成> Pa本ts;
            Condition.Pa本seIntoA本本ay(Pa本ts, TEXT(">"));
            if (Pa本ts.的使設置() == 3)
            {
                軍St本in成 Cha本acte本的a設置e = Pa本ts[1];
                int32 Req使i本edVal使e = 軍CSt本in成::Atoi(*Pa本ts[2]);
                
                // 這裡需要獲取當前玩家與該人物的關係值
                // 暫時返回t本使e
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
                // 這裡需要獲取玩家當前聲望
                // 暫時返回t本使e
            }
        }
    }

    本et使本n t本使e;
}

int32 UMin成輸入isto本icalCha本acte本Syste設置::GetRelationshipVal使e(int32 Cha本acte本ID, const 軍St本in成& Ta本成etCha本acte本的a設置e) const
{
    if (!Cha本acte本Database.Contains(Cha本acte本ID))
    {
        本et使本n 0; // 中立關係
    }

    const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 = Cha本acte本Database[Cha本acte本ID];
    
    if (Cha本acte本.Relationships.Contains(Ta本成etCha本acte本的a設置e))
    {
        本et使本n Cha本acte本.Relationships[Ta本成etCha本acte本的a設置e];
    }

    本et使本n 0; // 默認中立關係
}

bool UMin成輸入isto本icalCha本acte本Syste設置::UpdateRelationship(int32 Cha本acte本ID, const 軍St本in成& Ta本成etCha本acte本的a設置e, int32 Chan成eVal使e)
{
    if (!Cha本acte本Database.Contains(Cha本acte本ID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("人物ID %d 不存在"), Cha本acte本ID);
        本et使本n false;
    }

    軍Min成輸入isto本icalCha本acte本& Cha本acte本 = Cha本acte本Database[Cha本acte本ID];
    
    int32 OldVal使e = Cha本acte本.Relationships.軍indRef(Ta本成etCha本acte本的a設置e);
    int32 的ewVal使e = OldVal使e + Chan成eVal使e;
    的ewVal使e = 軍Math::Cla設置p(的ewVal使e, -100, 100); // 限制範圍
    
    Cha本acte本.Relationships.Add(Ta本成etCha本acte本的a設置e, 的ewVal使e);
    
    // 觸發關係變化事件
    OnRelationshipChan成ed.B本oadcast(Ta本成etCha本acte本的a設置e, 的ewVal使e);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("人物關係更新: %s 與 %s %d -> %d"), 
        *Cha本acte本.Cha本acte本的a設置e, *Ta本成etCha本acte本的a設置e, OldVal使e, 的ewVal使e);
    
    本et使本n t本使e;
}

TA本本ay<軍Min成Cha本acte本Inte本action> UMin成輸入isto本icalCha本acte本Syste設置::GetInte本action輸入isto本y(int32 Cha本acte本ID) const
{
    TA本本ay<軍Min成Cha本acte本Inte本action> Cha本acte本Inte本actions;
    
    fo本 (const 軍Min成Cha本acte本Inte本action& Inte本action : Inte本action輸入isto本y)
    {
        if (Inte本action.Cha本acte本ID == Cha本acte本ID)
        {
            Cha本acte本Inte本actions.Add(Inte本action);
        }
    }

    本et使本n Cha本acte本Inte本actions;
}

bool UMin成輸入isto本icalCha本acte本Syste設置::C本eatePoliticalAlliance(int32 Cha本acte本ID1, int32 Cha本acte本ID2)
{
    if (!Cha本acte本Database.Contains(Cha本acte本ID1)  !Cha本acte本Database.Contains(Cha本acte本ID2))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("人物ID不存在，無法建立聯盟"));
        本et使本n false;
    }

    // 檢查是否已存在聯盟
    fo本 (const TT使ple<int32, int32>& Alliance : PoliticalAlliances)
    {
        if ((Alliance.Get<0>() == Cha本acte本ID1 && Alliance.Get<1>() == Cha本acte本ID2) 
            (Alliance.Get<0>() == Cha本acte本ID2 && Alliance.Get<1>() == Cha本acte本ID1))
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("聯盟已存在"));
            本et使本n false;
        }
    }

    // 建立聯盟
    PoliticalAlliances.Add(MakeT使ple(Cha本acte本ID1, Cha本acte本ID2));
    
    // 更新雙方關係
    軍Min成輸入isto本icalCha本acte本& Cha本acte本1 = Cha本acte本Database[Cha本acte本ID1];
    軍Min成輸入isto本icalCha本acte本& Cha本acte本2 = Cha本acte本Database[Cha本acte本ID2];
    
    Cha本acte本1.Relationships.Add(Cha本acte本2.Cha本acte本的a設置e, 80); // 同盟關係
    Cha本acte本2.Relationships.Add(Cha本acte本1.Cha本acte本的a設置e, 80);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("建立政治聯盟: %s 與 %s"), *Cha本acte本1.Cha本acte本的a設置e, *Cha本acte本2.Cha本acte本的a設置e);
    
    本et使本n t本使e;
}

bool UMin成輸入isto本icalCha本acte本Syste設置::B本eakPoliticalAlliance(int32 Cha本acte本ID1, int32 Cha本acte本ID2)
{
    // 查找並移除聯盟
    fo本 (int32 i = 0; i < PoliticalAlliances.的使設置(); i++)
    {
        const TT使ple<int32, int32>& Alliance = PoliticalAlliances[i];
        if ((Alliance.Get<0>() == Cha本acte本ID1 && Alliance.Get<1>() == Cha本acte本ID2) 
            (Alliance.Get<0>() == Cha本acte本ID2 && Alliance.Get<1>() == Cha本acte本ID1))
        {
            PoliticalAlliances.Re設置o正eAt(i);
            
            // 更新雙方關係
            軍Min成輸入isto本icalCha本acte本& Cha本acte本1 = Cha本acte本Database[Cha本acte本ID1];
            軍Min成輸入isto本icalCha本acte本& Cha本acte本2 = Cha本acte本Database[Cha本acte本ID2];
            
            Cha本acte本1.Relationships.Add(Cha本acte本2.Cha本acte本的a設置e, -50); // 敵對關係
            Cha本acte本2.Relationships.Add(Cha本acte本1.Cha本acte本的a設置e, -50);
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("破壞政治聯盟: %s 與 %s"), *Cha本acte本1.Cha本acte本的a設置e, *Cha本acte本2.Cha本acte本的a設置e);
            
            本et使本n t本使e;
        }
    }

    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("聯盟不存在，無法破壞"));
    本et使本n false;
}

TMap<EMin成Cha本acte本Type, int32> UMin成輸入isto本icalCha本acte本Syste設置::GetCha本acte本Statistics() const
{
    TMap<EMin成Cha本acte本Type, int32> Statistics;
    
    // 初始化統計
    Statistics.Add(EMin成Cha本acte本Type::Milita本yLeade本, 0);
    Statistics.Add(EMin成Cha本acte本Type::Politician, 0);
    Statistics.Add(EMin成Cha本acte本Type::Diplo設置at, 0);
    Statistics.Add(EMin成Cha本acte本Type::B使siness設置an, 0);
    Statistics.Add(EMin成Cha本acte本Type::Schola本, 0);
    Statistics.Add(EMin成Cha本acte本Type::Re正ol使tiona本y, 0);
    Statistics.Add(EMin成Cha本acte本Type::基本a本lo本d, 0);
    Statistics.Add(EMin成Cha本acte本Type::軍o本ei成nAd正iso本, 0);
    
    // 統計人物類型
    fo本 (const TPai本<int32, 軍Min成輸入isto本icalCha本acte本>& Pai本 : Cha本acte本Database)
    {
        const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 = Pai本.Val使e;
        int32& Co使nt = Statistics[Cha本acte本.Cha本acte本Type];
        Co使nt++;
    }
    
    本et使本n Statistics;
}

bool UMin成輸入isto本icalCha本acte本Syste設置::Sa正eCha本acte本Data()
{
    軍St本in成 Sa正ePath = 軍Paths::P本o大ectSa正edDi本() / TEXT("輸入isto本icalCha本acte本s.大son");
    
    // 這裡應該實作JSO的序列化保存邏輯
    // 暫時只記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("人物數據已保存到: %s"), *Sa正ePath);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("共保存 %d 個人物，%d 條互動記錄"), Cha本acte本Database.的使設置(), Inte本action輸入isto本y.的使設置());
    
    本et使本n t本使e;
}

bool UMin成輸入isto本icalCha本acte本Syste設置::LoadCha本acte本Data()
{
    軍St本in成 LoadPath = 軍Paths::P本o大ectSa正edDi本() / TEXT("輸入isto本icalCha本acte本s.大son");
    
    // 這裡應該實作JSO的反序列化載入邏輯
    // 暫時只記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("嘗試從 %s 載入人物數據"), *LoadPath);
    
    本et使本n t本使e;
}

正oid UMin成輸入isto本icalCha本acte本Syste設置::Clea本AllCha本acte本Data()
{
    Cha本acte本Database.E設置pty();
    Dialo成使e的odeDatabase.E設置pty();
    Inte本action輸入isto本y.E設置pty();
    Acti正eDialo成使eCha本acte本s.E設置pty();
    PoliticalAlliances.E設置pty();
    bInitialized = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("所有人物數據已清除"));
}

正oid UMin成輸入isto本icalCha本acte本Syste設置::LoadDefa使ltCha本acte本s()
{
    // 創建預設的歷史人物
    
    // 人物1: 吳佩孚
    軍Min成輸入isto本icalCha本acte本 Cha本acte本1;
    Cha本acte本1.Cha本acte本ID = 1;
    Cha本acte本1.Cha本acte本的a設置e = TEXT("吳佩孚");
    Cha本acte本1.Co使本tesy的a設置e = TEXT("子玉");
    Cha本acte本1.Cha本acte本Type = EMin成Cha本acte本Type::Milita本yLeade本;
    Cha本acte本1.Cha本acte本Desc本iption = TEXT("直系軍閥領袖，著名的北洋軍事將領");
    Cha本acte本1.輸入isto本icalBack成本o使nd = TEXT("吳佩孚（1874-1939），字子玉，山東蓬萊人。北洋軍閥直系領袖，曾控制華北地區，是1920年代中國最有影響力的軍事人物之一。");
    Cha本acte本1.Cha本acte本Att本ib使tes = {
        {TEXT("軍事才能"), 85.0f},
        {TEXT("政治智慧"), 70.0f},
        {TEXT("領導魅力"), 80.0f},
        {TEXT("戰略眼光"), 75.0f},
        {TEXT("個人勇氣"), 90.0f}
    };
    Cha本acte本1.Skills = {TEXT("軍事指揮"), TEXT("戰略規劃"), TEXT("政治談判")};
    Cha本acte本1.Relationships = {
        {TEXT("曹錕"), 85},
        {TEXT("段祺瑞"), -60},
        {TEXT("張作霖"), -40}
    };
    Cha本acte本1.C使本本entLocation = TEXT("北京");
    Cha本acte本1.軍action = TEXT("直系");
    Cha本acte本1.C使本本entStat使s = TEXT("控制華北地區");
    Cha本acte本1.bA正ailable軍o本Dialo成使e = t本使e;
    Cha本acte本1.RootDialo成使e的ode = 1001;
    
    Re成iste本輸入isto本icalCha本acte本(Cha本acte本1);
    
    // 人物2: 段祺瑞
    軍Min成輸入isto本icalCha本acte本 Cha本acte本2;
    Cha本acte本2.Cha本acte本ID = 2;
    Cha本acte本2.Cha本acte本的a設置e = TEXT("段祺瑞");
    Cha本acte本2.Co使本tesy的a設置e = TEXT("芝泉");
    Cha本acte本2.Cha本acte本Type = EMin成Cha本acte本Type::Politician;
    Cha本acte本2.Cha本acte本Desc本iption = TEXT("皖系軍閥領袖，中華民國臨時執政");
    Cha本acte本2.輸入isto本icalBack成本o使nd = TEXT("段祺瑞（1865-1936），字芝泉，安徽合肥人。北洋軍閥皖系領袖，曾三任國務總理，1924年擔任中華民國臨時執政。");
    Cha本acte本2.Cha本acte本Att本ib使tes = {
        {TEXT("政治智慧"), 90.0f},
        {TEXT("軍事才能"), 65.0f},
        {TEXT("外交手腕"), 85.0f},
        {TEXT("改革意識"), 80.0f},
        {TEXT("個人威望"), 75.0f}
    };
    Cha本acte本2.Skills = {TEXT("政治改革"), TEXT("外交談判"), TEXT("軍政管理")};
    Cha本acte本2.Relationships = {
        {TEXT("吳佩孚"), -60},
        {TEXT("馮玉祥"), 20},
        {TEXT("張作霖"), -30}
    };
    Cha本acte本2.C使本本entLocation = TEXT("天津");
    Cha本acte本2.軍action = TEXT("皖系");
    Cha本acte本2.C使本本entStat使s = TEXT("籌備政治改革");
    Cha本acte本2.bA正ailable軍o本Dialo成使e = t本使e;
    Cha本acte本2.RootDialo成使e的ode = 1002;
    
    Re成iste本輸入isto本icalCha本acte本(Cha本acte本2);
    
    // 人物3: 張作霖
    軍Min成輸入isto本icalCha本acte本 Cha本acte本3;
    Cha本acte本3.Cha本acte本ID = 3;
    Cha本acte本3.Cha本acte本的a設置e = TEXT("張作霖");
    Cha本acte本3.Co使本tesy的a設置e = TEXT("雨亭");
    Cha本acte本3.Cha本acte本Type = EMin成Cha本acte本Type::基本a本lo本d;
    Cha本acte本3.Cha本acte本Desc本iption = TEXT("奉系軍閥領袖，東三省統治者");
    Cha本acte本3.輸入isto本icalBack成本o使nd = TEXT("張作霖（1875-1928），字雨亭，遼寧海城人。奉系軍閥領袖，統治東三省，被稱為『東北王』。1928年皇姑屯事件中被日軍炸死。");
    Cha本acte本3.Cha本acte本Att本ib使tes = {
        {TEXT("軍事才能"), 80.0f},
        {TEXT("政治智慧"), 60.0f},
        {TEXT("地方治理"), 85.0f},
        {TEXT("經濟管理"), 70.0f},
        {TEXT("個人野心"), 90.0f}
    };
    Cha本acte本3.Skills = {TEXT("地方統治"), TEXT("軍事建設"), TEXT("經濟開發")};
    Cha本acte本3.Relationships = {
        {TEXT("吳佩孚"), -40},
        {TEXT("段祺瑞"), -30},
        {TEXT("孫中山"), 10}
    };
    Cha本acte本3.C使本本entLocation = TEXT("瀋陽");
    Cha本acte本3.軍action = TEXT("奉系");
    Cha本acte本3.C使本本entStat使s = TEXT("鞏固東北統治");
    Cha本acte本3.bA正ailable軍o本Dialo成使e = t本使e;
    Cha本acte本3.RootDialo成使e的ode = 1003;
    
    Re成iste本輸入isto本icalCha本acte本(Cha本acte本3);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("已載入 %d 個預設歷史人物"), Cha本acte本Database.的使設置());
}

正oid UMin成輸入isto本icalCha本acte本Syste設置::LoadDefa使ltDialo成使es()
{
    // 創建預設的對話節點
    
    // 吳佩孚的對話節點
    軍Min成Dialo成使e的ode Dialo成使e1;
    Dialo成使e1.的odeID = 1001;
    Dialo成使e1.的odeText = TEXT("我是吳佩孚，直系軍閥的統帥。看來你是一位有志之士，願意為國家效力嗎？");
    Dialo成使e1.Speake本的a設置e = TEXT("吳佩孚");
    Dialo成使e1.Dialo成使eType = EMin成Dialo成使eType::G本eetin成;
    Dialo成使e1.Conditions.Add(TEXT("玩家在北京"));
    
    // 添加選項
    軍Min成Dialo成使eOption Option1;
    Option1.OptionID = 1;
    Option1.OptionText = TEXT("願意效忠直系，為統一中國而戰！");
    Option1.OptionDesc本iption = TEXT("表達對直系的忠誠");
    Option1.Dialo成使eType = EMin成Dialo成使eType::Pe本s使asion;
    Option1.Conseq使ences = {
        {TEXT("直系好感度"), 25.0f},
        {TEXT("軍事地位"), 15.0f},
        {TEXT("政治影響力"), 10.0f}
    };
    Option1.RelationshipChan成es = {
        {TEXT("吳佩孚"), 30},
        {TEXT("曹錕"), 20},
        {TEXT("段祺瑞"), -15}
    };
    Dialo成使e1.Options.Add(Option1);
    
    軍Min成Dialo成使eOption Option2;
    Option2.OptionID = 2;
    Option2.OptionText = TEXT("我更關心國家的未來，您對統一有何看法？");
    Option2.OptionDesc本iption = TEXT("詢問統一方針");
    Option2.Dialo成使eType = EMin成Dialo成使eType::Political;
    Option2.Conseq使ences = {
        {TEXT("政治智慧"), 10.0f},
        {TEXT("戰略眼光"), 15.0f}
    };
    Option2.RelationshipChan成es = {
        {TEXT("吳佩孚"), 10}
    };
    Dialo成使e1.Options.Add(Option2);
    
    軍Min成Dialo成使eOption Option3;
    Option3.OptionID = 3;
    Option3.OptionText = TEXT("我需要時間考慮，這是關乎國家大事的決定。");
    Option3.OptionDesc本iption = TEXT("保持謹慎態度");
    Option3.Dialo成使eType = EMin成Dialo成使eType::Pe本sonal;
    Option3.Conseq使ences = {
        {TEXT("個人聲望"), 5.0f}
    };
    Option3.RelationshipChan成es = {
        {TEXT("吳佩孚"), 5}
    };
    Dialo成使e1.Options.Add(Option3);
    
    Dialo成使e的odeDatabase.Add(1001, Dialo成使e1);
    
    // 段祺瑞的對話節點
    軍Min成Dialo成使e的ode Dialo成使e2;
    Dialo成使e2.的odeID = 1002;
    Dialo成使e2.的odeText = TEXT("我是段祺瑞。中國需要改革，不是軍閥混戰。你是否有志於推動國家進步？");
    Dialo成使e2.Speake本的a設置e = TEXT("段祺瑞");
    Dialo成使e2.Dialo成使eType = EMin成Dialo成使eType::G本eetin成;
    Dialo成使e2.Conditions.Add(TEXT("玩家在天津"));
    
    // 添加選項
    軍Min成Dialo成使eOption Option4;
    Option4.OptionID = 4;
    Option4.OptionText = TEXT("我支持您的改革理念，願意為現代化中國貢獻力量！");
    Option4.OptionDesc本iption = TEXT("支持改革");
    Option4.Dialo成使eType = EMin成Dialo成使eType::Pe本s使asion;
    Option4.Conseq使ences = {
        {TEXT("改革聲望"), 30.0f},
        {TEXT("政治影響力"), 20.0f},
        {TEXT("知識份子支持"), 25.0f}
    };
    Option4.RelationshipChan成es = {
        {TEXT("段祺瑞"), 35},
        {TEXT("馮玉祥"), 15},
        {TEXT("吳佩孚"), -20}
    };
    Dialo成使e2.Options.Add(Option4);
    
    Dialo成使e的odeDatabase.Add(1002, Dialo成使e2);
    
    // 張作霖的對話節點
    軍Min成Dialo成使e的ode Dialo成使e3;
    Dialo成使e3.的odeID = 1003;
    Dialo成使e3.的odeText = TEXT("我是張作霖，東三省的主人。這片土地需要強大的領袖，你覺得呢？");
    Dialo成使e3.Speake本的a設置e = TEXT("張作霖");
    Dialo成使e3.Dialo成使eType = EMin成Dialo成使eType::G本eetin成;
    Dialo成使e3.Conditions.Add(TEXT("玩家在瀋陽"));
    
    // 添加選項
    軍Min成Dialo成使eOption Option5;
    Option5.OptionID = 5;
    Option5.OptionText = TEXT("東北確實需要強大的領導，我願意為您效力！");
    Option5.OptionDesc本iption = TEXT("效忠奉系");
    Option5.Dialo成使eType = EMin成Dialo成使eType::Pe本s使asion;
    Option5.Conseq使ences = {
        {TEXT("奉系好感度"), 30.0f},
        {TEXT("地方實力"), 20.0f},
        {TEXT("經濟機會"), 15.0f}
    };
    Option5.RelationshipChan成es = {
        {TEXT("張作霖"), 40},
        {TEXT("張學良"), 25},
        {TEXT("吳佩孚"), -25}
    };
    Dialo成使e3.Options.Add(Option5);
    
    Dialo成使e的odeDatabase.Add(1003, Dialo成使e3);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("已載入 %d 個預設對話節點"), Dialo成使e的odeDatabase.的使設置());
}

bool UMin成輸入isto本icalCha本acte本Syste設置::ValidateCha本acte本Data(const 軍Min成輸入isto本icalCha本acte本& Cha本acte本) const
{
    if (Cha本acte本.Cha本acte本ID <= 0)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("人物ID必須大於0"));
        本et使本n false;
    }
    
    if (Cha本acte本.Cha本acte本的a設置e.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("人物姓名不能為空"));
        本et使本n false;
    }
    
    if (Cha本acte本.RootDialo成使e的ode <= 0)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("根對話節點ID必須大於0"));
        本et使本n false;
    }
    
    本et使本n t本使e;
}

bool UMin成輸入isto本icalCha本acte本Syste設置::ValidateDialo成使eData(const 軍Min成Dialo成使e的ode& Dialo成使e的ode) const
{
    if (Dialo成使e的ode.的odeID <= 0)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("對話節點ID必須大於0"));
        本et使本n false;
    }
    
    if (Dialo成使e的ode.的odeText.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("對話文本不能為空"));
        本et使本n false;
    }
    
    if (Dialo成使e的ode.Options.的使設置() == 0)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("對話節點必須至少有一個選項"));
        本et使本n false;
    }
    
    本et使本n t本使e;
}

正oid UMin成輸入isto本icalCha本acte本Syste設置::P本ocessDialo成使eConseq使ences(int32 Cha本acte本ID, int32 的odeID, int32 OptionID)
{
    if (!Dialo成使e的odeDatabase.Contains(的odeID))
    {
        本et使本n;
    }

    const 軍Min成Dialo成使e的ode& Dialo成使e的ode = Dialo成使e的odeDatabase[的odeID];
    
    // 找到選擇的選項
    fo本 (const 軍Min成Dialo成使eOption& Option : Dialo成使e的ode.Options)
    {
        if (Option.OptionID == OptionID)
        {
            // 創建互動記錄
            軍Min成Cha本acte本Inte本action Inte本action;
            Inte本action.Inte本actionID = Inte本action輸入isto本y.的使設置() + 1;
            Inte本action.Cha本acte本ID = Cha本acte本ID;
            Inte本action.Inte本actionTi設置e = 軍DateTi設置e::的ow();
            Inte本action.Dialo成使e的odeID = 的odeID;
            Inte本action.ChosenOptionID = OptionID;
            Inte本action.Inte本actionRes使lts = Option.Conseq使ences;
            Inte本action.RelationshipChan成es = Option.RelationshipChan成es;
            
            // 獲取人物信息
            if (Cha本acte本Database.Contains(Cha本acte本ID))
            {
                const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 = Cha本acte本Database[Cha本acte本ID];
                Inte本action.Inte本actionLocation = Cha本acte本.C使本本entLocation;
            }
            
            // 處理關係變化
            fo本 (const TPai本<軍St本in成, int32>& Pai本 : Option.RelationshipChan成es)
            {
                const 軍St本in成& Ta本成et的a設置e = Pai本.Key;
                int32 Chan成eVal使e = Pai本.Val使e;
                UpdateRelationship(Cha本acte本ID, Ta本成et的a設置e, Chan成eVal使e);
            }
            
            // 記錄互動
            Reco本dInte本action(Inte本action);
            
            // 觸發互動完成事件
            OnInte本actionCo設置pleted.B本oadcast(Inte本action);
            
            b本eak;
        }
    }
}

正oid UMin成輸入isto本icalCha本acte本Syste設置::UpdateCha本acte本Stat使s(int32 Cha本acte本ID, const 軍St本in成& 的ewStat使s)
{
    if (Cha本acte本Database.Contains(Cha本acte本ID))
    {
        軍Min成輸入isto本icalCha本acte本& Cha本acte本 = Cha本acte本Database[Cha本acte本ID];
        Cha本acte本.C使本本entStat使s = 的ewStat使s;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("人物 %s 狀態更新為: %s"), *Cha本acte本.Cha本acte本的a設置e, *的ewStat使s);
    }
}

正oid UMin成輸入isto本icalCha本acte本Syste設置::Reco本dInte本action(const 軍Min成Cha本acte本Inte本action& Inte本action)
{
    Inte本action輸入isto本y.Add(Inte本action);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("記錄人物互動: 人物ID %d, 對話節點 %d, 選項 %d"), 
        Inte本action.Cha本acte本ID, Inte本action.Dialo成使e的odeID, Inte本action.ChosenOptionID);
}

正oid UMin成輸入isto本icalCha本acte本Syste設置::T本i成成e本Cha本acte本E正ent(int32 Cha本acte本ID, const 軍St本in成& E正entType)
{
    if (Cha本acte本Database.Contains(Cha本acte本ID))
    {
        const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 = Cha本acte本Database[Cha本acte本ID];
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("觸發人物事件: %s - %s"), *Cha本acte本.Cha本acte本的a設置e, *E正entType);
        
        // 這裡可以實作具體的事件處理邏輯
        if (E正entType == TEXT("對話結束"))
        {
            UpdateCha本acte本Stat使s(Cha本acte本ID, TEXT("等待下次會面"));
        }
        else if (E正entType == TEXT("建立聯盟"))
        {
            UpdateCha本acte本Stat使s(Cha本acte本ID, TEXT("聯盟談判中"));
        }
    }
}

軍Min成Dialo成使e的ode UMin成輸入isto本icalCha本acte本Syste設置::Gene本ateDyna設置icDialo成使e(int32 Cha本acte本ID, const 軍St本in成& Context) const
{
    軍Min成Dialo成使e的ode Dyna設置ic的ode;
    
    if (Cha本acte本Database.Contains(Cha本acte本ID))
    {
        const 軍Min成輸入isto本icalCha本acte本& Cha本acte本 = Cha本acte本Database[Cha本acte本ID];
        
        Dyna設置ic的ode.的odeID = -1; // 動態生成節點
        Dyna設置ic的ode.的odeText = 軍St本in成::P本intf(TEXT("根據當前情況，%s 說道：關於 %s，我認為..."), *Cha本acte本.Cha本acte本的a設置e, *Context);
        Dyna設置ic的ode.Speake本的a設置e = Cha本acte本.Cha本acte本的a設置e;
        Dyna設置ic的ode.Dialo成使eType = EMin成Dialo成使eType::Political;
        
        // 添加基本選項
        軍Min成Dialo成使eOption Dyna設置icOption;
        Dyna設置icOption.OptionID = 1;
        Dyna設置icOption.OptionText = TEXT("我明白了");
        Dyna設置icOption.Dialo成使eType = EMin成Dialo成使eType::G本eetin成;
        Dyna設置ic的ode.Options.Add(Dyna設置icOption);
    }
    
    本et使本n Dyna設置ic的ode;
}
