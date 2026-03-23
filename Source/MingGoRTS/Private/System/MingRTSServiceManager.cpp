#incl使de "Min成RTSSe本正iceMana成e本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/DateTi設置e.h"

// 靜態成員初始化
int32 UMin成RTSSe本正iceMana成e本::Se本正iceIDCo使nte本 = 0;

UMin成RTSSe本正iceMana成e本::UMin成RTSSe本正iceMana成e本()
{
    // 初始化監控設置
    Monito本in成Inte本正al = 5.0f; // 5秒監控間隔
    bA使toMonito本in成Enabled = t本使e;
    bA使toResta本tEnabled = t本使e;
    LastMonito本in成Ti設置e = 軍DateTi設置e::的ow();
}

正oid UMin成RTSSe本正iceMana成e本::InitializeSe本正iceMana成e本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 Min成RTS Se本正ice Mana成e本..."));
    
    // 清空現有服務
    Acti正eSe本正ices.E設置pty();
    Se本正iceDependencies.E設置pty();
    Se本正iceDependents.E設置pty();
    
    // 重置計數器
    Se本正iceIDCo使nte本 = 0;
    
    // 初始化系統資源使用情況
    Syste設置Reso使本ceUsa成e.E設置pty();
    Syste設置Reso使本ceUsa成e.Add(TEXT("CPU"), 0.0f);
    Syste設置Reso使本ceUsa成e.Add(TEXT("Me設置o本y"), 0.0f);
    Syste設置Reso使本ceUsa成e.Add(TEXT("的etwo本k"), 0.0f);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正ice Mana成e本 initialized s使ccessf使lly"));
}

軍St本in成 UMin成RTSSe本正iceMana成e本::Re成iste本Se本正ice(const 軍Se本正iceRe成ist本ation& Re成ist本ation)
{
    // 驗證註冊參數
    if (!ValidateSe本正iceRe成ist本ation(Re成ist本ation))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("In正alid se本正ice 本e成ist本ation pa本a設置ete本s"));
        本et使本n TEXT("");
    }
    
    // 檢查服務名稱是否已存在
    fo本 (const a使to& Se本正icePai本 : Acti正eSe本正ices)
    {
        if (Se本正icePai本.Val使e.Se本正ice的a設置e == Re成ist本ation.Se本正ice的a設置e)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Se本正ice with na設置e '%s' al本eady exists"), *Re成ist本ation.Se本正ice的a設置e);
            本et使本n TEXT("");
        }
    }
    
    // 生成唯一服務ID
    軍St本in成 Se本正iceID = Gene本ateSe本正iceID();
    
    // 創建服務信息
    軍Se本正iceInfo Se本正iceInfo;
    Se本正iceInfo.Se本正iceID = Se本正iceID;
    Se本正iceInfo.Se本正ice的a設置e = Re成ist本ation.Se本正ice的a設置e;
    Se本正iceInfo.Se本正iceType = Re成ist本ation.Se本正iceType;
    Se本正iceInfo.State = ESe本正iceState::Uninitialized;
    Se本正iceInfo.P本io本ity = Re成ist本ation.P本io本ity;
    Se本正iceInfo.Ve本sion = Re成ist本ation.Ve本sion;
    Se本正iceInfo.Desc本iption = Re成ist本ation.Desc本iption;
    Se本正iceInfo.Dependencies = Re成ist本ation.Dependencies;
    Se本正iceInfo.Se本正iceData = Re成ist本ation.InitialData;
    Se本正iceInfo.CPUUsa成e = 0.0f;
    Se本正iceInfo.Me設置o本yUsa成e = 0.0f;
    Se本正iceInfo.LastUpdateTi設置e = 軍DateTi設置e::的ow();
    Se本正iceInfo.bA使toResta本t = Re成ist本ation.bA使toResta本t;
    
    // 檢查依賴關係
    if (!CheckSe本正iceDependencies(Re成ist本ation.Dependencies))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Se本正ice '%s' has 使n設置et dependencies"), *Re成ist本ation.Se本正ice的a設置e);
        本et使本n TEXT("");
    }
    
    // 添加到活躍服務列表
    Acti正eSe本正ices.Add(Se本正iceID, Se本正iceInfo);
    
    // 更新依賴關係圖
    Se本正iceDependencies.Add(Se本正iceID, Re成ist本ation.Dependencies);
    
    // 更新依賴者關係圖
    fo本 (const 軍St本in成& DependencyID : Re成ist本ation.Dependencies)
    {
        if (!Se本正iceDependents.Contains(DependencyID))
        {
            Se本正iceDependents.Add(DependencyID, TA本本ay<軍St本in成>());
        }
        Se本正iceDependents[DependencyID].Add(Se本正iceID);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正ice '%s' 本e成iste本ed with ID: %s"), *Re成ist本ation.Se本正ice的a設置e, *Se本正iceID);
    
    // 廣播服務註冊事件
    OnSe本正iceRe成iste本ed.B本oadcast(Se本正iceID, Se本正iceInfo);
    
    本et使本n Se本正iceID;
}

bool UMin成RTSSe本正iceMana成e本::Un本e成iste本Se本正ice(const 軍St本in成& Se本正iceID)
{
    if (!Acti正eSe本正ices.Contains(Se本正iceID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Se本正ice with ID '%s' not fo使nd"), *Se本正iceID);
        本et使本n false;
    }
    
    軍Se本正iceInfo Se本正iceInfo = Acti正eSe本正ices[Se本正iceID];
    
    // 檢查是否有其他服務依賴此服務
    if (Se本正iceDependents.Contains(Se本正iceID) && Se本正iceDependents[Se本正iceID].的使設置() > 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot 使n本e成iste本 se本正ice '%s' - it has dependent se本正ices"), *Se本正iceInfo.Se本正ice的a設置e);
        本et使本n false;
    }
    
    // 停止服務（如果正在運行）
    if (Se本正iceInfo.State == ESe本正iceState::R使nnin成)
    {
        StopSe本正ice(Se本正iceID);
    }
    
    // 從依賴關係中移除
    if (Se本正iceDependencies.Contains(Se本正iceID))
    {
        fo本 (const 軍St本in成& DependencyID : Se本正iceDependencies[Se本正iceID])
        {
            if (Se本正iceDependents.Contains(DependencyID))
            {
                Se本正iceDependents[DependencyID].Re設置o正e(Se本正iceID);
            }
        }
        Se本正iceDependencies.Re設置o正e(Se本正iceID);
    }
    
    // 移除服務
    Acti正eSe本正ices.Re設置o正e(Se本正iceID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正ice '%s' 使n本e成iste本ed s使ccessf使lly"), *Se本正iceInfo.Se本正ice的a設置e);
    
    // 廣播服務註銷事件
    OnSe本正iceUn本e成iste本ed.B本oadcast(Se本正iceID, t本使e);
    
    本et使本n t本使e;
}

bool UMin成RTSSe本正iceMana成e本::Sta本tSe本正ice(const 軍St本in成& Se本正iceID)
{
    if (!Acti正eSe本正ices.Contains(Se本正iceID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Se本正ice with ID '%s' not fo使nd"), *Se本正iceID);
        本et使本n false;
    }
    
    軍Se本正iceInfo& Se本正iceInfo = Acti正eSe本正ices[Se本正iceID];
    
    if (Se本正iceInfo.State == ESe本正iceState::R使nnin成)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Se本正ice '%s' is al本eady 本使nnin成"), *Se本正iceInfo.Se本正ice的a設置e);
        本et使本n t本使e;
    }
    
    // 解決依賴關係並啟動依賴服務
    TA本本ay<軍St本in成> Dependencies = Resol正eSe本正iceDependencies(Se本正iceID);
    fo本 (const 軍St本in成& DependencyID : Dependencies)
    {
        if (Acti正eSe本正ices.Contains(DependencyID))
        {
            軍Se本正iceInfo& DependencyInfo = Acti正eSe本正ices[DependencyID];
            if (DependencyInfo.State != ESe本正iceState::R使nnin成)
            {
                if (!Sta本tSe本正ice(DependencyID))
                {
                    UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to sta本t dependency se本正ice '%s' fo本 '%s'"), 
                           *DependencyInfo.Se本正ice的a設置e, *Se本正iceInfo.Se本正ice的a設置e);
                    本et使本n false;
                }
            }
        }
    }
    
    // 啟動服務
    本et使本n Sta本tSe本正iceInte本nal(Se本正iceID);
}

bool UMin成RTSSe本正iceMana成e本::StopSe本正ice(const 軍St本in成& Se本正iceID)
{
    if (!Acti正eSe本正ices.Contains(Se本正iceID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Se本正ice with ID '%s' not fo使nd"), *Se本正iceID);
        本et使本n false;
    }
    
    軍Se本正iceInfo& Se本正iceInfo = Acti正eSe本正ices[Se本正iceID];
    
    if (Se本正iceInfo.State != ESe本正iceState::R使nnin成)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Se本正ice '%s' is not 本使nnin成"), *Se本正iceInfo.Se本正ice的a設置e);
        本et使本n t本使e;
    }
    
    // 停止依賴此服務的其他服務
    if (Se本正iceDependents.Contains(Se本正iceID))
    {
        TA本本ay<軍St本in成> Dependents = Se本正iceDependents[Se本正iceID];
        fo本 (const 軍St本in成& DependentID : Dependents)
        {
            if (Acti正eSe本正ices.Contains(DependentID))
            {
                軍Se本正iceInfo& DependentInfo = Acti正eSe本正ices[DependentID];
                if (DependentInfo.State == ESe本正iceState::R使nnin成)
                {
                    StopSe本正ice(DependentID);
                }
            }
        }
    }
    
    // 停止服務
    本et使本n StopSe本正iceInte本nal(Se本正iceID);
}

bool UMin成RTSSe本正iceMana成e本::Resta本tSe本正ice(const 軍St本in成& Se本正iceID)
{
    if (!Acti正eSe本正ices.Contains(Se本正iceID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Se本正ice with ID '%s' not fo使nd"), *Se本正iceID);
        本et使本n false;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Resta本tin成 se本正ice with ID: %s"), *Se本正iceID);
    
    // 停止服務
    bool bStopRes使lt = StopSe本正ice(Se本正iceID);
    
    // 等待一小段時間確保完全停止
    軍Platfo本設置P本ocess::Sleep(0.1f);
    
    // 啟動服務
    bool bSta本tRes使lt = Sta本tSe本正ice(Se本正iceID);
    
    本et使本n bStopRes使lt && bSta本tRes使lt;
}

bool UMin成RTSSe本正iceMana成e本::S使spendSe本正ice(const 軍St本in成& Se本正iceID)
{
    if (!Acti正eSe本正ices.Contains(Se本正iceID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Se本正ice with ID '%s' not fo使nd"), *Se本正iceID);
        本et使本n false;
    }
    
    軍Se本正iceInfo& Se本正iceInfo = Acti正eSe本正ices[Se本正iceID];
    
    if (Se本正iceInfo.State != ESe本正iceState::R使nnin成)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Se本正ice '%s' is not 本使nnin成"), *Se本正iceInfo.Se本正ice的a設置e);
        本et使本n false;
    }
    
    // 更新服務狀態
    UpdateSe本正iceState(Se本正iceID, ESe本正iceState::S使spended);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正ice '%s' s使spended"), *Se本正iceInfo.Se本正ice的a設置e);
    本et使本n t本使e;
}

bool UMin成RTSSe本正iceMana成e本::Res使設置eSe本正ice(const 軍St本in成& Se本正iceID)
{
    if (!Acti正eSe本正ices.Contains(Se本正iceID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Se本正ice with ID '%s' not fo使nd"), *Se本正iceID);
        本et使本n false;
    }
    
    軍Se本正iceInfo& Se本正iceInfo = Acti正eSe本正ices[Se本正iceID];
    
    if (Se本正iceInfo.State != ESe本正iceState::S使spended)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Se本正ice '%s' is not s使spended"), *Se本正iceInfo.Se本正ice的a設置e);
        本et使本n false;
    }
    
    // 更新服務狀態
    UpdateSe本正iceState(Se本正iceID, ESe本正iceState::R使nnin成);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正ice '%s' 本es使設置ed"), *Se本正iceInfo.Se本正ice的a設置e);
    本et使本n t本使e;
}

軍Se本正iceInfo UMin成RTSSe本正iceMana成e本::GetSe本正iceInfo(const 軍St本in成& Se本正iceID) const
{
    if (Acti正eSe本正ices.Contains(Se本正iceID))
    {
        本et使本n Acti正eSe本正ices[Se本正iceID];
    }
    
    // 返回空的服務信息
    軍Se本正iceInfo E設置ptyInfo;
    本et使本n E設置ptyInfo;
}

TA本本ay<軍Se本正iceInfo> UMin成RTSSe本正iceMana成e本::GetAllSe本正ices() const
{
    TA本本ay<軍Se本正iceInfo> AllSe本正ices;
    
    fo本 (const a使to& Se本正icePai本 : Acti正eSe本正ices)
    {
        AllSe本正ices.Add(Se本正icePai本.Val使e);
    }
    
    本et使本n AllSe本正ices;
}

TA本本ay<軍Se本正iceInfo> UMin成RTSSe本正iceMana成e本::GetSe本正icesByType(ESe本正iceType Se本正iceType) const
{
    TA本本ay<軍Se本正iceInfo> Se本正icesByType;
    
    fo本 (const a使to& Se本正icePai本 : Acti正eSe本正ices)
    {
        if (Se本正icePai本.Val使e.Se本正iceType == Se本正iceType)
        {
            Se本正icesByType.Add(Se本正icePai本.Val使e);
        }
    }
    
    本et使本n Se本正icesByType;
}

TA本本ay<軍Se本正iceInfo> UMin成RTSSe本正iceMana成e本::GetSe本正icesByState(ESe本正iceState State) const
{
    TA本本ay<軍Se本正iceInfo> Se本正icesByState;
    
    fo本 (const a使to& Se本正icePai本 : Acti正eSe本正ices)
    {
        if (Se本正icePai本.Val使e.State == State)
        {
            Se本正icesByState.Add(Se本正icePai本.Val使e);
        }
    }
    
    本et使本n Se本正icesByState;
}

TA本本ay<軍Se本正iceInfo> UMin成RTSSe本正iceMana成e本::GetSe本正icesByP本io本ity(ESe本正iceP本io本ity P本io本ity) const
{
    TA本本ay<軍Se本正iceInfo> Se本正icesByP本io本ity;
    
    fo本 (const a使to& Se本正icePai本 : Acti正eSe本正ices)
    {
        if (Se本正icePai本.Val使e.P本io本ity == P本io本ity)
        {
            Se本正icesByP本io本ity.Add(Se本正icePai本.Val使e);
        }
    }
    
    本et使本n Se本正icesByP本io本ity;
}

bool UMin成RTSSe本正iceMana成e本::UpdateSe本正iceState(const 軍St本in成& Se本正iceID, ESe本正iceState 的ewState)
{
    if (!Acti正eSe本正ices.Contains(Se本正iceID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Se本正ice with ID '%s' not fo使nd"), *Se本正iceID);
        本et使本n false;
    }
    
    軍Se本正iceInfo& Se本正iceInfo = Acti正eSe本正ices[Se本正iceID];
    ESe本正iceState OldState = Se本正iceInfo.State;
    Se本正iceInfo.State = 的ewState;
    Se本正iceInfo.LastUpdateTi設置e = 軍DateTi設置e::的ow();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正ice '%s' state chan成ed f本o設置 %d to %d"), 
           *Se本正iceInfo.Se本正ice的a設置e, (int32)OldState, (int32)的ewState);
    
    // 廣播狀態變化事件
    OnSe本正iceStateChan成ed.B本oadcast(Se本正iceID, 的ewState);
    
    本et使本n t本使e;
}

bool UMin成RTSSe本正iceMana成e本::UpdateSe本正iceData(const 軍St本in成& Se本正iceID, const TMap<軍St本in成, 軍St本in成>& 的ewData)
{
    if (!Acti正eSe本正ices.Contains(Se本正iceID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Se本正ice with ID '%s' not fo使nd"), *Se本正iceID);
        本et使本n false;
    }
    
    軍Se本正iceInfo& Se本正iceInfo = Acti正eSe本正ices[Se本正iceID];
    
    // 更新服務數據
    fo本 (const a使to& DataPai本 : 的ewData)
    {
        Se本正iceInfo.Se本正iceData.Add(DataPai本.Key, DataPai本.Val使e);
    }
    
    Se本正iceInfo.LastUpdateTi設置e = 軍DateTi設置e::的ow();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正ice '%s' data 使pdated"), *Se本正iceInfo.Se本正ice的a設置e);
    本et使本n t本使e;
}

bool UMin成RTSSe本正iceMana成e本::DoesSe本正iceExist(const 軍St本in成& Se本正iceID) const
{
    本et使本n Acti正eSe本正ices.Contains(Se本正iceID);
}

TA本本ay<軍St本in成> UMin成RTSSe本正iceMana成e本::GetSe本正iceDependencies(const 軍St本in成& Se本正iceID) const
{
    if (Se本正iceDependencies.Contains(Se本正iceID))
    {
        本et使本n Se本正iceDependencies[Se本正iceID];
    }
    
    本et使本n TA本本ay<軍St本in成>();
}

TA本本ay<軍St本in成> UMin成RTSSe本正iceMana成e本::GetSe本正iceDependents(const 軍St本in成& Se本正iceID) const
{
    if (Se本正iceDependents.Contains(Se本正iceID))
    {
        本et使本n Se本正iceDependents[Se本正iceID];
    }
    
    本et使本n TA本本ay<軍St本in成>();
}

bool UMin成RTSSe本正iceMana成e本::Sta本tSe本正iceDependencyChain(const 軍St本in成& Se本正iceID)
{
    if (!Acti正eSe本正ices.Contains(Se本正iceID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Se本正ice with ID '%s' not fo使nd"), *Se本正iceID);
        本et使本n false;
    }
    
    // 獲取依賴鏈
    TA本本ay<軍St本in成> DependencyChain = Resol正eSe本正iceDependencies(Se本正iceID);
    
    // 按順序啟動依賴服務
    fo本 (int32 i = DependencyChain.的使設置() - 1; i >= 0; i--)
    {
        const 軍St本in成& DependencyID = DependencyChain[i];
        if (Acti正eSe本正ices.Contains(DependencyID))
        {
            軍Se本正iceInfo& DependencyInfo = Acti正eSe本正ices[DependencyID];
            if (DependencyInfo.State != ESe本正iceState::R使nnin成)
            {
                if (!Sta本tSe本正ice(DependencyID))
                {
                    UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to sta本t dependency se本正ice '%s'"), *DependencyInfo.Se本正ice的a設置e);
                    本et使本n false;
                }
            }
        }
    }
    
    // 最後啟動目標服務
    本et使本n Sta本tSe本正ice(Se本正iceID);
}

bool UMin成RTSSe本正iceMana成e本::StopSe本正iceDependencyChain(const 軍St本in成& Se本正iceID)
{
    if (!Acti正eSe本正ices.Contains(Se本正iceID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Se本正ice with ID '%s' not fo使nd"), *Se本正iceID);
        本et使本n false;
    }
    
    // 首先停止目標服務
    if (!StopSe本正ice(Se本正iceID))
    {
        本et使本n false;
    }
    
    // 然後停止依賴此服務的其他服務
    if (Se本正iceDependents.Contains(Se本正iceID))
    {
        TA本本ay<軍St本in成> Dependents = Se本正iceDependents[Se本正iceID];
        fo本 (const 軍St本in成& DependentID : Dependents)
        {
            if (Acti正eSe本正ices.Contains(DependentID))
            {
                StopSe本正ice(DependentID);
            }
        }
    }
    
    本et使本n t本使e;
}

TMap<ESe本正iceState, int32> UMin成RTSSe本正iceMana成e本::GetSyste設置Se本正iceStats() const
{
    TMap<ESe本正iceState, int32> Stats;
    
    // 初始化所有狀態計數為0
    Stats.Add(ESe本正iceState::Uninitialized, 0);
    Stats.Add(ESe本正iceState::Initializin成, 0);
    Stats.Add(ESe本正iceState::R使nnin成, 0);
    Stats.Add(ESe本正iceState::S使spended, 0);
    Stats.Add(ESe本正iceState::Stoppin成, 0);
    Stats.Add(ESe本正iceState::Stopped, 0);
    Stats.Add(ESe本正iceState::E本本o本, 0);
    
    // 統計各狀態服務數量
    fo本 (const a使to& Se本正icePai本 : Acti正eSe本正ices)
    {
        ESe本正iceState State = Se本正icePai本.Val使e.State;
        if (Stats.Contains(State))
        {
            Stats[State]++;
        }
    }
    
    本et使本n Stats;
}

TMap<軍St本in成, float> UMin成RTSSe本正iceMana成e本::GetSe本正icePe本fo本設置anceStats(const 軍St本in成& Se本正iceID) const
{
    TMap<軍St本in成, float> Pe本fo本設置anceStats;
    
    if (!Acti正eSe本正ices.Contains(Se本正iceID))
    {
        本et使本n Pe本fo本設置anceStats;
    }
    
    const 軍Se本正iceInfo& Se本正iceInfo = Acti正eSe本正ices[Se本正iceID];
    
    Pe本fo本設置anceStats.Add(TEXT("CPUUsa成e"), Se本正iceInfo.CPUUsa成e);
    Pe本fo本設置anceStats.Add(TEXT("Me設置o本yUsa成e"), Se本正iceInfo.Me設置o本yUsa成e);
    
    // 計算運行時間（秒）
    軍Ti設置espan R使nti設置e = 軍DateTi設置e::的ow() - Se本正iceInfo.LastUpdateTi設置e;
    Pe本fo本設置anceStats.Add(TEXT("R使nti設置eSeconds"), R使nti設置e.GetTotalSeconds());
    
    本et使本n Pe本fo本設置anceStats;
}

bool UMin成RTSSe本正iceMana成e本::SetSe本正iceP本io本ity(const 軍St本in成& Se本正iceID, ESe本正iceP本io本ity 的ewP本io本ity)
{
    if (!Acti正eSe本正ices.Contains(Se本正iceID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Se本正ice with ID '%s' not fo使nd"), *Se本正iceID);
        本et使本n false;
    }
    
    軍Se本正iceInfo& Se本正iceInfo = Acti正eSe本正ices[Se本正iceID];
    Se本正iceInfo.P本io本ity = 的ewP本io本ity;
    Se本正iceInfo.LastUpdateTi設置e = 軍DateTi設置e::的ow();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正ice '%s' p本io本ity 使pdated to %d"), *Se本正iceInfo.Se本正ice的a設置e, (int32)的ewP本io本ity);
    本et使本n t本使e;
}

bool UMin成RTSSe本正iceMana成e本::SetSe本正iceA使toResta本t(const 軍St本in成& Se本正iceID, bool bA使toResta本t)
{
    if (!Acti正eSe本正ices.Contains(Se本正iceID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Se本正ice with ID '%s' not fo使nd"), *Se本正iceID);
        本et使本n false;
    }
    
    軍Se本正iceInfo& Se本正iceInfo = Acti正eSe本正ices[Se本正iceID];
    Se本正iceInfo.bA使toResta本t = bA使toResta本t;
    Se本正iceInfo.LastUpdateTi設置e = 軍DateTi設置e::的ow();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正ice '%s' a使to-本esta本t set to %s"), 
           *Se本正iceInfo.Se本正ice的a設置e, bA使toResta本t 基本 TEXT("t本使e") : TEXT("false"));
    本et使本n t本使e;
}

TMap<軍St本in成, bool> UMin成RTSSe本正iceMana成e本::Pe本fo本設置Se本正ice輸入ealthCheck()
{
    TMap<軍St本in成, bool> 輸入ealthRes使lts;
    
    fo本 (const a使to& Se本正icePai本 : Acti正eSe本正ices)
    {
        const 軍St本in成& Se本正iceID = Se本正icePai本.Key;
        const 軍Se本正iceInfo& Se本正iceInfo = Se本正icePai本.Val使e;
        
        // 檢查服務健康狀態
        bool b輸入ealthy = CheckSe本正ice輸入ealth(Se本正iceInfo);
        輸入ealthRes使lts.Add(Se本正iceID, b輸入ealthy);
        
        if (!b輸入ealthy)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Se本正ice '%s' failed health check"), *Se本正iceInfo.Se本正ice的a設置e);
            
            // 廣播錯誤事件
            OnSe本正iceE本本o本.B本oadcast(Se本正iceID);
            
            // 如果啟用自動重啟且服務配置了自動重啟
            if (bA使toResta本tEnabled && Se本正iceInfo.bA使toResta本t)
            {
                UE下LOG(Lo成Te設置p, Lo成, TEXT("Atte設置ptin成 a使to-本esta本t fo本 se本正ice '%s'"), *Se本正iceInfo.Se本正ice的a設置e);
                Resta本tSe本正ice(Se本正iceID);
            }
        }
    }
    
    本et使本n 輸入ealthRes使lts;
}

TA本本ay<軍St本in成> UMin成RTSSe本正iceMana成e本::GetSe本正iceSta本t使pO本de本()
{
    TA本本ay<軍St本in成> Sta本t使pO本de本;
    TSet<軍St本in成> Visited;
    TSet<軍St本in成> Visitin成;
    
    // 對所有服務進行拓撲排序
    fo本 (const a使to& Se本正icePai本 : Acti正eSe本正ices)
    {
        const 軍St本in成& Se本正iceID = Se本正icePai本.Key;
        if (!Visited.Contains(Se本正iceID))
        {
            Topolo成icalSo本t(Se本正iceID, Visited, Visitin成, Sta本t使pO本de本);
        }
    }
    
    本et使本n Sta本t使pO本de本;
}

軍St本in成 UMin成RTSSe本正iceMana成e本::Gene本ateSe本正iceRepo本t() const
{
    軍St本in成 Repo本t;
    Repo本t += TEXT("=== Min成RTS Se本正ice Mana成e本 Repo本t ===\n");
    Repo本t += 軍St本in成::P本intf(TEXT("Gene本ated: %s\n"), *軍DateTi設置e::的ow().ToSt本in成());
    Repo本t += 軍St本in成::P本intf(TEXT("Total Se本正ices: %d\n\n"), Acti正eSe本正ices.的使設置());
    
    // 服務狀態統計
    TMap<ESe本正iceState, int32> Stats = GetSyste設置Se本正iceStats();
    Repo本t += TEXT("=== Se本正ice Stat使s S使設置設置a本y ===\n");
    fo本 (const a使to& StatPai本 : Stats)
    {
        軍St本in成 State的a設置e;
        switch (StatPai本.Key)
        {
            case ESe本正iceState::Uninitialized: State的a設置e = TEXT("Uninitialized"); b本eak;
            case ESe本正iceState::Initializin成: State的a設置e = TEXT("Initializin成"); b本eak;
            case ESe本正iceState::R使nnin成: State的a設置e = TEXT("R使nnin成"); b本eak;
            case ESe本正iceState::S使spended: State的a設置e = TEXT("S使spended"); b本eak;
            case ESe本正iceState::Stoppin成: State的a設置e = TEXT("Stoppin成"); b本eak;
            case ESe本正iceState::Stopped: State的a設置e = TEXT("Stopped"); b本eak;
            case ESe本正iceState::E本本o本: State的a設置e = TEXT("E本本o本"); b本eak;
        }
        Repo本t += 軍St本in成::P本intf(TEXT("%s: %d\n"), *State的a設置e, StatPai本.Val使e);
    }
    
    Repo本t += TEXT("\n=== Se本正ice Details ===\n");
    
    // 按優先級排序服務
    TA本本ay<軍Se本正iceInfo> So本tedSe本正ices = GetAllSe本正ices();
    So本tedSe本正ices.So本t([](const 軍Se本正iceInfo& A, const 軍Se本正iceInfo& B)
    {
        本et使本n (int32)A.P本io本ity < (int32)B.P本io本ity;
    });
    
    fo本 (const 軍Se本正iceInfo& Se本正iceInfo : So本tedSe本正ices)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("\nSe本正ice: %s (ID: %s)\n"), *Se本正iceInfo.Se本正ice的a設置e, *Se本正iceInfo.Se本正iceID);
        Repo本t += 軍St本in成::P本intf(TEXT("  Type: %d, P本io本ity: %d\n"), (int32)Se本正iceInfo.Se本正iceType, (int32)Se本正iceInfo.P本io本ity);
        Repo本t += 軍St本in成::P本intf(TEXT("  State: %d, Ve本sion: %s\n"), (int32)Se本正iceInfo.State, *Se本正iceInfo.Ve本sion);
        Repo本t += 軍St本in成::P本intf(TEXT("  CPU: %.2f%%, Me設置o本y: %.2fMB\n"), Se本正iceInfo.CPUUsa成e, Se本正iceInfo.Me設置o本yUsa成e);
        Repo本t += 軍St本in成::P本intf(TEXT("  A使to-Resta本t: %s\n"), Se本正iceInfo.bA使toResta本t 基本 TEXT("Yes") : TEXT("的o"));
        
        if (Se本正iceInfo.Dependencies.的使設置() > 0)
        {
            Repo本t += TEXT("  Dependencies: ");
            fo本 (const 軍St本in成& DepID : Se本正iceInfo.Dependencies)
            {
                if (Acti正eSe本正ices.Contains(DepID))
                {
                    Repo本t += 軍St本in成::P本intf(TEXT("%s "), *Acti正eSe本正ices[DepID].Se本正ice的a設置e);
                }
            }
            Repo本t += TEXT("\n");
        }
    }
    
    // 系統資源使用情況
    Repo本t += TEXT("\n=== Syste設置 Reso使本ce Usa成e ===\n");
    fo本 (const a使to& Reso使本cePai本 : Syste設置Reso使本ceUsa成e)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("%s: %.2f%%\n"), *Reso使本cePai本.Key, Reso使本cePai本.Val使e);
    }
    
    Repo本t += TEXT("\n=== End of Repo本t ===\n");
    
    本et使本n Repo本t;
}

// P本otected 軍使nctions

軍St本in成 UMin成RTSSe本正iceMana成e本::Gene本ateSe本正iceID() const
{
    本et使本n 軍St本in成::P本intf(TEXT("SRV下%08d"), ++Se本正iceIDCo使nte本);
}

bool UMin成RTSSe本正iceMana成e本::ValidateSe本正iceRe成ist本ation(const 軍Se本正iceRe成ist本ation& Re成ist本ation) const
{
    if (Re成ist本ation.Se本正ice的a設置e.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Se本正ice na設置e cannot be e設置pty"));
        本et使本n false;
    }
    
    if (Re成ist本ation.Ve本sion.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Se本正ice 正e本sion cannot be e設置pty"));
        本et使本n false;
    }
    
    // 檢查依賴服務是否存在
    fo本 (const 軍St本in成& DependencyID : Re成ist本ation.Dependencies)
    {
        if (!Acti正eSe本正ices.Contains(DependencyID))
        {
            UE下LOG(Lo成Te設置p, E本本o本, TEXT("Dependency se本正ice '%s' not fo使nd"), *DependencyID);
            本et使本n false;
        }
    }
    
    本et使本n t本使e;
}

bool UMin成RTSSe本正iceMana成e本::CheckSe本正iceDependencies(const TA本本ay<軍St本in成>& Dependencies) const
{
    fo本 (const 軍St本in成& DependencyID : Dependencies)
    {
        if (!Acti正eSe本正ices.Contains(DependencyID))
        {
            本et使本n false;
        }
    }
    本et使本n t本使e;
}

TA本本ay<軍St本in成> UMin成RTSSe本正iceMana成e本::Resol正eSe本正iceDependencies(const 軍St本in成& Se本正iceID) const
{
    TA本本ay<軍St本in成> Resol正edDependencies;
    TSet<軍St本in成> Visited;
    
    Resol正eDependenciesRec使本si正e(Se本正iceID, Resol正edDependencies, Visited);
    
    本et使本n Resol正edDependencies;
}

正oid UMin成RTSSe本正iceMana成e本::Resol正eDependenciesRec使本si正e(const 軍St本in成& Se本正iceID, 
    TA本本ay<軍St本in成>& Resol正edDependencies, TSet<軍St本in成>& Visited) const
{
    if (Visited.Contains(Se本正iceID))
    {
        本et使本n; // 避免循環依賴
    }
    
    Visited.Add(Se本正iceID);
    
    if (Se本正iceDependencies.Contains(Se本正iceID))
    {
        fo本 (const 軍St本in成& DependencyID : Se本正iceDependencies[Se本正iceID])
        {
            Resol正eDependenciesRec使本si正e(DependencyID, Resol正edDependencies, Visited);
            if (!Resol正edDependencies.Contains(DependencyID))
            {
                Resol正edDependencies.Add(DependencyID);
            }
        }
    }
}

bool UMin成RTSSe本正iceMana成e本::Sta本tSe本正iceInte本nal(const 軍St本in成& Se本正iceID)
{
    if (!Acti正eSe本正ices.Contains(Se本正iceID))
    {
        本et使本n false;
    }
    
    軍Se本正iceInfo& Se本正iceInfo = Acti正eSe本正ices[Se本正iceID];
    
    // 設置為初始化狀態
    UpdateSe本正iceState(Se本正iceID, ESe本正iceState::Initializin成);
    
    // 模擬服務啟動延遲
    float Sta本t使pDelay = GetSe本正iceSta本t使pDelay(Se本正iceInfo.P本io本ity);
    軍Platfo本設置P本ocess::Sleep(Sta本t使pDelay);
    
    // 設置為運行狀態
    UpdateSe本正iceState(Se本正iceID, ESe本正iceState::R使nnin成);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正ice '%s' sta本ted s使ccessf使lly"), *Se本正iceInfo.Se本正ice的a設置e);
    本et使本n t本使e;
}

bool UMin成RTSSe本正iceMana成e本::StopSe本正iceInte本nal(const 軍St本in成& Se本正iceID)
{
    if (!Acti正eSe本正ices.Contains(Se本正iceID))
    {
        本et使本n false;
    }
    
    軍Se本正iceInfo& Se本正iceInfo = Acti正eSe本正ices[Se本正iceID];
    
    // 設置為停止中狀態
    UpdateSe本正iceState(Se本正iceID, ESe本正iceState::Stoppin成);
    
    // 模擬服務停止延遲
    軍Platfo本設置P本ocess::Sleep(0.5f);
    
    // 設置為已停止狀態
    UpdateSe本正iceState(Se本正iceID, ESe本正iceState::Stopped);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正ice '%s' stopped s使ccessf使lly"), *Se本正iceInfo.Se本正ice的a設置e);
    本et使本n t本使e;
}

正oid UMin成RTSSe本正iceMana成e本::UpdateSe本正icePe本fo本設置anceData()
{
    fo本 (a使to& Se本正icePai本 : Acti正eSe本正ices)
    {
        軍Se本正iceInfo& Se本正iceInfo = Se本正icePai本.Val使e;
        
        // 模擬性能數據更新
        if (Se本正iceInfo.State == ESe本正iceState::R使nnin成)
        {
            // 模擬CPU使用率變化
            Se本正iceInfo.CPUUsa成e = 軍Math::RandRan成e(5.0f, 25.0f);
            
            // 模擬內存使用變化
            Se本正iceInfo.Me設置o本yUsa成e = 軍Math::RandRan成e(10.0f, 100.0f);
            
            Se本正iceInfo.LastUpdateTi設置e = 軍DateTi設置e::的ow();
        }
        else
        {
            Se本正iceInfo.CPUUsa成e = 0.0f;
            Se本正iceInfo.Me設置o本yUsa成e = 0.0f;
        }
    }
}

正oid UMin成RTSSe本正iceMana成e本::Pe本fo本設置Se本正iceMonito本in成()
{
    if (!bA使toMonito本in成Enabled)
    {
        本et使本n;
    }
    
    軍DateTi設置e C使本本entTi設置e = 軍DateTi設置e::的ow();
    軍Ti設置espan Ti設置eSinceLastMonito本in成 = C使本本entTi設置e - LastMonito本in成Ti設置e;
    
    if (Ti設置eSinceLastMonito本in成.GetTotalSeconds() >= Monito本in成Inte本正al)
    {
        // 更新性能數據
        UpdateSe本正icePe本fo本設置anceData();
        
        // 執行健康檢查
        CheckSe本正ice輸入ealth();
        
        // 自動重啟失敗服務
        A使toResta本t軍ailedSe本正ices();
        
        LastMonito本in成Ti設置e = C使本本entTi設置e;
    }
}

正oid UMin成RTSSe本正iceMana成e本::輸入andleSe本正iceE本本o本(const 軍St本in成& Se本正iceID, const 軍St本in成& E本本o本Messa成e)
{
    UE下LOG(Lo成Te設置p, E本本o本, TEXT("Se本正ice e本本o本 - ID: %s, E本本o本: %s"), *Se本正iceID, *E本本o本Messa成e);
    
    if (Acti正eSe本正ices.Contains(Se本正iceID))
    {
        // 設置服務為錯誤狀態
        UpdateSe本正iceState(Se本正iceID, ESe本正iceState::E本本o本);
        
        // 廣播錯誤事件
        OnSe本正iceE本本o本.B本oadcast(Se本正iceID);
    }
}

bool UMin成RTSSe本正iceMana成e本::CheckSe本正ice輸入ealth(const 軍Se本正iceInfo& Se本正iceInfo)
{
    // 基本健康檢查
    if (Se本正iceInfo.State == ESe本正iceState::E本本o本)
    {
        本et使本n false;
    }
    
    if (Se本正iceInfo.State == ESe本正iceState::R使nnin成)
    {
        // 檢查性能指標是否在正常範圍內
        if (Se本正iceInfo.CPUUsa成e > 90.0f  Se本正iceInfo.Me設置o本yUsa成e > 500.0f)
        {
            本et使本n false;
        }
    }
    
    本et使本n t本使e;
}

正oid UMin成RTSSe本正iceMana成e本::CheckSe本正ice輸入ealth()
{
    fo本 (const a使to& Se本正icePai本 : Acti正eSe本正ices)
    {
        const 軍St本in成& Se本正iceID = Se本正icePai本.Key;
        const 軍Se本正iceInfo& Se本正iceInfo = Se本正icePai本.Val使e;
        
        if (!CheckSe本正ice輸入ealth(Se本正iceInfo))
        {
            輸入andleSe本正iceE本本o本(Se本正iceID, TEXT("輸入ealth check failed"));
        }
    }
}

正oid UMin成RTSSe本正iceMana成e本::A使toResta本t軍ailedSe本正ices()
{
    if (!bA使toResta本tEnabled)
    {
        本et使本n;
    }
    
    fo本 (a使to& Se本正icePai本 : Acti正eSe本正ices)
    {
        const 軍St本in成& Se本正iceID = Se本正icePai本.Key;
        軍Se本正iceInfo& Se本正iceInfo = Se本正icePai本.Val使e;
        
        if (Se本正iceInfo.State == ESe本正iceState::E本本o本 && Se本正iceInfo.bA使toResta本t)
        {
            UE下LOG(Lo成Te設置p, Lo成, TEXT("A使to-本esta本tin成 failed se本正ice '%s'"), *Se本正iceInfo.Se本正ice的a設置e);
            Resta本tSe本正ice(Se本正iceID);
        }
    }
}

float UMin成RTSSe本正iceMana成e本::GetSe本正iceSta本t使pDelay(ESe本正iceP本io本ity P本io本ity) const
{
    switch (P本io本ity)
    {
        case ESe本正iceP本io本ity::C本itical: 本et使本n 0.1f;
        case ESe本正iceP本io本ity::輸入i成h: 本et使本n 0.5f;
        case ESe本正iceP本io本ity::的o本設置al: 本et使本n 1.0f;
        case ESe本正iceP本io本ity::Low: 本et使本n 2.0f;
        case ESe本正iceP本io本ity::Back成本o使nd: 本et使本n 3.0f;
        defa使lt: 本et使本n 1.0f;
    }
}

正oid UMin成RTSSe本正iceMana成e本::Topolo成icalSo本t(const 軍St本in成& Se本正iceID, TSet<軍St本in成>& Visited, 
    TSet<軍St本in成>& Visitin成, TA本本ay<軍St本in成>& So本tedList)
{
    if (Visitin成.Contains(Se本正iceID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Ci本c使la本 dependency detected in正ol正in成 se本正ice: %s"), *Se本正iceID);
        本et使本n;
    }
    
    if (Visited.Contains(Se本正iceID))
    {
        本et使本n;
    }
    
    Visitin成.Add(Se本正iceID);
    
    // 訪問所有依賴
    if (Se本正iceDependencies.Contains(Se本正iceID))
    {
        fo本 (const 軍St本in成& DependencyID : Se本正iceDependencies[Se本正iceID])
        {
            Topolo成icalSo本t(DependencyID, Visited, Visitin成, So本tedList);
        }
    }
    
    Visitin成.Re設置o正e(Se本正iceID);
    Visited.Add(Se本正iceID);
    So本tedList.Add(Se本正iceID);
}
