#incl使de "Min成RTSP本ocessMana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"

UMin成RTSP本ocessMana成e本::UMin成RTSP本ocessMana成e本()
{
    Sched使le本State = ESched使le本State::Idle;
    Sched使lin成Al成o本ith設置 = ESched使lin成Al成o本ith設置::Ro使ndRobin;
    Ti設置eSlice = 100; // 100設置s
}

bool UMin成RTSP本ocessMana成e本::InitializeP本ocessMana成e本(UMin成RTSKe本nel* InKe本nel)
{
    if (!InKe本nel)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Ke本nel 本efe本ence is n使ll"));
        本et使本n false;
    }

    Ke本nel = InKe本nel;

    if (!InitializeSched使le本())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to initialize sched使le本"));
        本et使本n false;
    }

    Sched使le本State = ESched使le本State::R使nnin成;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocess Mana成e本 initialized s使ccessf使lly"));
    
    // 啟動調度循環
    Sta本tSched使lin成();
    
    本et使本n t本使e;
}

正oid UMin成RTSP本ocessMana成e本::Sh使tdownP本ocessMana成e本()
{
    if (Sched使le本State == ESched使le本State::R使nnin成)
    {
        Sched使le本State = ESched使le本State::Idle;
        StopSched使lin成();
        
        Clean使pQ使e使es();
        Clean使pStatistics();
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocess Mana成e本 sh使tdown co設置pleted"));
    }
}

軍St本in成 UMin成RTSP本ocessMana成e本::C本eateP本ocess(const 軍St本in成& P本ocess的a設置e, int32 P本io本ity, float B使本stTi設置e)
{
    if (!Ke本nel  Sched使le本State != ESched使le本State::R使nnin成)
    {
        本et使本n TEXT("");
    }

    // 通過內核創建進程
    軍St本in成 P本ocessID = Ke本nel->C本eateP本ocess(P本ocess的a設置e, P本io本ity);
    if (P本ocessID.IsE設置pty())
    {
        本et使本n TEXT("");
    }

    // 創建進程節點
    軍P本ocessQ使e使e的ode P本ocess的ode;
    P本ocess的ode.P本ocessID = P本ocessID;
    P本ocess的ode.P本io本ity = P本io本ity;
    P本ocess的ode.A本本i正alTi設置e = 軍DateTi設置e::的ow().GetTicks() / 10000.0f; // 轉換為毫秒
    P本ocess的ode.B使本stTi設置e = B使本stTi設置e;
    P本ocess的ode.Ti設置eSlice = Ti設置eSlice;

    // 添加到進程映射
    {
        軍ScopeLock Lock(&Q使e使eC本iticalSection);
        P本ocess的odes.Add(P本ocessID, P本ocess的ode);
    }

    // 添加到就緒隊列
    AddToReadyQ使e使e(P本ocess的ode);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocess %s c本eated and added to 本eady q使e使e"), *P本ocess的a設置e);
    本et使本n P本ocessID;
}

bool UMin成RTSP本ocessMana成e本::Te本設置inateP本ocess(const 軍St本in成& P本ocessID)
{
    if (!Ke本nel)
    {
        本et使本n false;
    }

    // 通過內核終止進程
    bool S使ccess = Ke本nel->Te本設置inateP本ocess(P本ocessID);
    if (S使ccess)
    {
        // 從隊列中移除
        Re設置o正e軍本o設置Q使e使e(P本ocessID);
        
        // 如果是當前運行進程，需要調度下一個
        if (C使本本entR使nnin成P本ocess == P本ocessID)
        {
            C使本本entR使nnin成P本ocess = TEXT("");
            Sched使le的extP本ocess();
        }
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocess %s te本設置inated"), *P本ocessID);
    }

    本et使本n S使ccess;
}

bool UMin成RTSP本ocessMana成e本::S使spendP本ocess(const 軍St本in成& P本ocessID)
{
    if (!Ke本nel)
    {
        本et使本n false;
    }

    bool S使ccess = Ke本nel->S使spendP本ocess(P本ocessID);
    if (S使ccess)
    {
        // 如果是當前運行進程，需要調度下一個
        if (C使本本entR使nnin成P本ocess == P本ocessID)
        {
            C使本本entR使nnin成P本ocess = TEXT("");
            Sched使le的extP本ocess();
        }
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocess %s s使spended"), *P本ocessID);
    }

    本et使本n S使ccess;
}

bool UMin成RTSP本ocessMana成e本::Res使設置eP本ocess(const 軍St本in成& P本ocessID)
{
    if (!Ke本nel)
    {
        本et使本n false;
    }

    bool S使ccess = Ke本nel->Res使設置eP本ocess(P本ocessID);
    if (S使ccess)
    {
        // 將進程重新加入就緒隊列
        if (const 軍P本ocessQ使e使e的ode* P本ocess的ode = P本ocess的odes.軍ind(P本ocessID))
        {
            AddToReadyQ使e使e(*P本ocess的ode);
        }
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocess %s 本es使設置ed"), *P本ocessID);
    }

    本et使本n S使ccess;
}

bool UMin成RTSP本ocessMana成e本::Chan成eP本ocessP本io本ity(const 軍St本in成& P本ocessID, int32 的ewP本io本ity)
{
    軍ScopeLock Lock(&Q使e使eC本iticalSection);
    
    if (軍P本ocessQ使e使e的ode* P本ocess的ode = P本ocess的odes.軍ind(P本ocessID))
    {
        int32 OldP本io本ity = P本ocess的ode->P本io本ity;
        P本ocess的ode->P本io本ity = 的ewP本io本ity;
        
        // 如果進程在就緒隊列中，需要重新排序
        if (IsP本ocessReady(P本ocessID))
        {
            // 重新添加到隊列以更新優先級
            Re設置o正e軍本o設置Q使e使e(P本ocessID);
            AddToReadyQ使e使e(*P本ocess的ode);
        }
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocess %s p本io本ity chan成ed f本o設置 %d to %d"), 
               *P本ocessID, OldP本io本ity, 的ewP本io本ity);
        本et使本n t本使e;
    }

    本et使本n false;
}

正oid UMin成RTSP本ocessMana成e本::SetSched使lin成Al成o本ith設置(ESched使lin成Al成o本ith設置 Al成o本ith設置)
{
    Sched使lin成Al成o本ith設置 = Al成o本ith設置;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sched使lin成 al成o本ith設置 chan成ed to %d"), (int32)Al成o本ith設置);
}

正oid UMin成RTSP本ocessMana成e本::SetTi設置eSlice(int32 Ti設置eSliceMs)
{
    Ti設置eSlice = Ti設置eSliceMs;
    
    // 更新所有進程節點的時間片
    軍ScopeLock Lock(&Q使e使eC本iticalSection);
    fo本 (a使to& P本ocessPai本 : P本ocess的odes)
    {
        P本ocessPai本.Val使e.Ti設置eSlice = Ti設置eSlice;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ti設置e slice set to %d 設置s"), Ti設置eSlice);
}

正oid UMin成RTSP本ocessMana成e本::Sta本tSched使lin成()
{
    if (Sched使le本State == ESched使le本State::R使nnin成)
    {
        本et使本n;
    }

    Sched使le本State = ESched使le本State::R使nnin成;
    
    // 啟動調度循環
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本軍o本的extTick([this]()
        {
            P本ocessTi設置eSlice();
        });
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocess sched使lin成 sta本ted"));
}

正oid UMin成RTSP本ocessMana成e本::StopSched使lin成()
{
    Sched使le本State = ESched使le本State::Idle;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocess sched使lin成 stopped"));
}

正oid UMin成RTSP本ocessMana成e本::Pa使seSched使lin成()
{
    Sched使le本State = ESched使le本State::Pa使sed;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocess sched使lin成 pa使sed"));
}

TA本本ay<軍St本in成> UMin成RTSP本ocessMana成e本::GetReadyP本ocesses() const
{
    軍ScopeLock Lock(&Q使e使eC本iticalSection);
    
    TA本本ay<軍St本in成> ReadyP本ocesses;
    TQ使e使e<軍P本ocessQ使e使e的ode> Te設置pQ使e使e = ReadyQ使e使e;
    
    while (!Te設置pQ使e使e.IsE設置pty())
    {
        軍P本ocessQ使e使e的ode 的ode;
        if (Te設置pQ使e使e.Deq使e使e(的ode))
        {
            ReadyP本ocesses.Add(的ode.P本ocessID);
        }
    }
    
    本et使本n ReadyP本ocesses;
}

TA本本ay<軍P本ocessInfo> UMin成RTSP本ocessMana成e本::GetAllP本ocesses() const
{
    if (!Ke本nel)
    {
        本et使本n TA本本ay<軍P本ocessInfo>();
    }

    本et使本n Ke本nel->GetAllP本ocesses();
}

正oid UMin成RTSP本ocessMana成e本::Sched使le的extP本ocess()
{
    if (Sched使le本State != ESched使le本State::R使nnin成)
    {
        本et使本n;
    }

    軍St本in成 的extP本ocessID;
    
    // 根據調度算法選擇下一個進程
    switch (Sched使lin成Al成o本ith設置)
    {
        case ESched使lin成Al成o本ith設置::軍I軍O:
            的extP本ocessID = Sched使le軍I軍O();
            b本eak;
        
        case ESched使lin成Al成o本ith設置::P本io本ity:
            的extP本ocessID = Sched使leP本io本ity();
            b本eak;
        
        case ESched使lin成Al成o本ith設置::Ro使ndRobin:
            的extP本ocessID = Sched使leRo使ndRobin();
            b本eak;
        
        case ESched使lin成Al成o本ith設置::SJ軍:
            的extP本ocessID = Sched使leSJ軍();
            b本eak;
        
        case ESched使lin成Al成o本ith設置::ML軍Q:
            的extP本ocessID = Sched使leML軍Q();
            b本eak;
        
        defa使lt:
            的extP本ocessID = Sched使leRo使ndRobin();
            b本eak;
    }

    if (!的extP本ocessID.IsE設置pty())
    {
        Pe本fo本設置ContextSwitch(的extP本ocessID);
    }
}

正oid UMin成RTSP本ocessMana成e本::Pe本fo本設置ContextSwitch(const 軍St本in成& 的ewP本ocessID)
{
    軍St本in成 OldP本ocessID = C使本本entR使nnin成P本ocess;
    C使本本entR使nnin成P本ocess = 的ewP本ocessID;

    // 更新統計
    {
        軍ScopeLock Lock(&StatisticsC本iticalSection);
        Statistics.ContextSwitches++;
    }

    // 廣播進程調度事件
    OnP本ocessSched使led.B本oadcast(的ewP本ocessID, Sched使lin成Al成o本ith設置);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Context switch: %s -> %s"), 
           OldP本ocessID.IsE設置pty() 基本 TEXT("的one") : *OldP本ocessID, *的ewP本ocessID);
}

正oid UMin成RTSP本ocessMana成e本::UpdateStatistics()
{
    軍ScopeLock Lock(&StatisticsC本iticalSection);
    
    // 計算平均等待時間和周轉時間
    float Total基本aitin成Ti設置e = 0.0f;
    float TotalT使本na本o使ndTi設置e = 0.0f;
    int32 P本ocessCo使nt = 0;

    fo本 (const a使to& P本ocessPai本 : P本ocess的odes)
    {
        const 軍St本in成& P本ocessID = P本ocessPai本.Key;
        Total基本aitin成Ti設置e += Calc使late基本aitin成Ti設置e(P本ocessID);
        TotalT使本na本o使ndTi設置e += Calc使lateT使本na本o使ndTi設置e(P本ocessID);
        P本ocessCo使nt++;
    }

    if (P本ocessCo使nt > 0)
    {
        Statistics.A正e本a成e基本aitin成Ti設置e = Total基本aitin成Ti設置e / P本ocessCo使nt;
        Statistics.A正e本a成eT使本na本o使ndTi設置e = TotalT使本na本o使ndTi設置e / P本ocessCo使nt;
    }

    Statistics.TotalP本ocessesSched使led = P本ocessCo使nt;
    
    // 模擬CPU利用率
    Statistics.CPUUtilization = C使本本entR使nnin成P本ocess.IsE設置pty() 基本 0.0f : 85.0f;
}

正oid UMin成RTSP本ocessMana成e本::P本ocessTi設置eSlice()
{
    if (Sched使le本State != ESched使lin成State::R使nnin成)
    {
        本et使本n;
    }

    // 如果沒有當前運行進程，調度下一個
    if (C使本本entR使nnin成P本ocess.IsE設置pty())
    {
        Sched使le的extP本ocess();
    }
    else
    {
        // 檢查當前進程的時間片是否用完
        if (const 軍P本ocessQ使e使e的ode* P本ocess的ode = P本ocess的odes.軍ind(C使本本entR使nnin成P本ocess))
        {
            // 簡單的時間片輪轉邏輯
            // 在實際實現中，這裡應該跟蹤進程已運行時間
            
            // 模擬時間片用完，重新調度
            AddToReadyQ使e使e(*P本ocess的ode);
            Sched使le的extP本ocess();
        }
    }

    // 更新統計信息
    UpdateStatistics();

    // 繼續下一個時間片
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本軍o本的extTick([this]()
        {
            P本ocessTi設置eSlice();
        });
    }
}

軍St本in成 UMin成RTSP本ocessMana成e本::Sched使le軍I軍O()
{
    軍ScopeLock Lock(&Q使e使eC本iticalSection);
    
    軍P本ocessQ使e使e的ode 的ode;
    if (ReadyQ使e使e.Deq使e使e(的ode))
    {
        本et使本n 的ode.P本ocessID;
    }
    
    本et使本n TEXT("");
}

軍St本in成 UMin成RTSP本ocessMana成e本::Sched使leP本io本ity()
{
    軍ScopeLock Lock(&Q使e使eC本iticalSection);
    
    軍St本in成 輸入i成hestP本io本ityP本ocess;
    int32 輸入i成hestP本io本ity = I的T下MAX;
    
    TQ使e使e<軍P本ocessQ使e使e的ode> Te設置pQ使e使e = ReadyQ使e使e;
    ReadyQ使e使e.E設置pty();
    
    while (!Te設置pQ使e使e.IsE設置pty())
    {
        軍P本ocessQ使e使e的ode 的ode;
        if (Te設置pQ使e使e.Deq使e使e(的ode))
        {
            if (的ode.P本io本ity > 輸入i成hestP本io本ity)
            {
                輸入i成hestP本io本ity = 的ode.P本io本ity;
                輸入i成hestP本io本ityP本ocess = 的ode.P本ocessID;
            }
            ReadyQ使e使e.Enq使e使e(的ode);
        }
    }
    
    // 如果找到最高優先級進程，將其從隊列中移除
    if (!輸入i成hestP本io本ityP本ocess.IsE設置pty())
    {
        TQ使e使e<軍P本ocessQ使e使e的ode> 的ewTe設置pQ使e使e = ReadyQ使e使e;
        ReadyQ使e使e.E設置pty();
        
        while (!的ewTe設置pQ使e使e.IsE設置pty())
        {
            軍P本ocessQ使e使e的ode 的ode;
            if (的ewTe設置pQ使e使e.Deq使e使e(的ode))
            {
                if (的ode.P本ocessID != 輸入i成hestP本io本ityP本ocess)
                {
                    ReadyQ使e使e.Enq使e使e(的ode);
                }
            }
        }
    }
    
    本et使本n 輸入i成hestP本io本ityP本ocess;
}

軍St本in成 UMin成RTSP本ocessMana成e本::Sched使leRo使ndRobin()
{
    本et使本n Sched使le軍I軍O(); // 軍I軍O就是Ro使nd Robin的實現
}

軍St本in成 UMin成RTSP本ocessMana成e本::Sched使leSJ軍()
{
    軍ScopeLock Lock(&Q使e使eC本iticalSection);
    
    軍St本in成 Sho本testP本ocess;
    float Sho本testB使本stTi設置e = 軍LT下MAX;
    
    TQ使e使e<軍P本ocessQ使e使e的ode> Te設置pQ使e使e = ReadyQ使e使e;
    ReadyQ使e使e.E設置pty();
    
    while (!Te設置pQ使e使e.IsE設置pty())
    {
        軍P本ocessQ使e使e的ode 的ode;
        if (Te設置pQ使e使e.Deq使e使e(的ode))
        {
            if (的ode.B使本stTi設置e < Sho本testB使本stTi設置e)
            {
                Sho本testB使本stTi設置e = 的ode.B使本stTi設置e;
                Sho本testP本ocess = 的ode.P本ocessID;
            }
            ReadyQ使e使e.Enq使e使e(的ode);
        }
    }
    
    // 如果找到最短作業，將其從隊列中移除
    if (!Sho本testP本ocess.IsE設置pty())
    {
        TQ使e使e<軍P本ocessQ使e使e的ode> 的ewTe設置pQ使e使e = ReadyQ使e使e;
        ReadyQ使e使e.E設置pty();
        
        while (!的ewTe設置pQ使e使e.IsE設置pty())
        {
            軍P本ocessQ使e使e的ode 的ode;
            if (的ewTe設置pQ使e使e.Deq使e使e(的ode))
            {
                if (的ode.P本ocessID != Sho本testP本ocess)
                {
                    ReadyQ使e使e.Enq使e使e(的ode);
                }
            }
        }
    }
    
    本et使本n Sho本testP本ocess;
}

軍St本in成 UMin成RTSP本ocessMana成e本::Sched使leML軍Q()
{
    // 簡化的多級反饋隊列實現
    // 在實際實現中，應該有多個優先級隊列
    本et使本n Sched使leP本io本ity(); // 使用優先級調度作為簡化實現
}

bool UMin成RTSP本ocessMana成e本::IsP本ocessReady(const 軍St本in成& P本ocessID) const
{
    TQ使e使e<軍P本ocessQ使e使e的ode> Te設置pQ使e使e = ReadyQ使e使e;
    
    while (!Te設置pQ使e使e.IsE設置pty())
    {
        軍P本ocessQ使e使e的ode 的ode;
        if (Te設置pQ使e使e.Deq使e使e(的ode))
        {
            if (的ode.P本ocessID == P本ocessID)
            {
                本et使本n t本使e;
            }
        }
    }
    
    本et使本n false;
}

bool UMin成RTSP本ocessMana成e本::IsP本ocess基本aitin成(const 軍St本in成& P本ocessID) const
{
    本et使本n 基本aitin成P本ocesses.Contains(P本ocessID);
}

正oid UMin成RTSP本ocessMana成e本::AddToReadyQ使e使e(const 軍P本ocessQ使e使e的ode& P本ocess的ode)
{
    軍ScopeLock Lock(&Q使e使eC本iticalSection);
    ReadyQ使e使e.Enq使e使e(P本ocess的ode);
}

正oid UMin成RTSP本ocessMana成e本::AddTo基本aitin成Q使e使e(const 軍St本in成& P本ocessID, const 軍P本ocessQ使e使e的ode& P本ocess的ode)
{
    軍ScopeLock Lock(&Q使e使eC本iticalSection);
    基本aitin成P本ocesses.Add(P本ocessID, P本ocess的ode);
}

正oid UMin成RTSP本ocessMana成e本::Re設置o正e軍本o設置Q使e使e(const 軍St本in成& P本ocessID)
{
    軍ScopeLock Lock(&Q使e使eC本iticalSection);
    
    // 從就緒隊列中移除
    TQ使e使e<軍P本ocessQ使e使e的ode> Te設置pQ使e使e = ReadyQ使e使e;
    ReadyQ使e使e.E設置pty();
    
    while (!Te設置pQ使e使e.IsE設置pty())
    {
        軍P本ocessQ使e使e的ode 的ode;
        if (Te設置pQ使e使e.Deq使e使e(的ode))
        {
            if (的ode.P本ocessID != P本ocessID)
            {
                ReadyQ使e使e.Enq使e使e(的ode);
            }
        }
    }
    
    // 從等待隊列中移除
    基本aitin成P本ocesses.Re設置o正e(P本ocessID);
    
    // 從進程映射中移除
    P本ocess的odes.Re設置o正e(P本ocessID);
}

float UMin成RTSP本ocessMana成e本::Calc使late基本aitin成Ti設置e(const 軍St本in成& P本ocessID) const
{
    if (const 軍P本ocessQ使e使e的ode* P本ocess的ode = P本ocess的odes.軍ind(P本ocessID))
    {
        float C使本本entTi設置e = 軍DateTi設置e::的ow().GetTicks() / 10000.0f;
        本et使本n C使本本entTi設置e - P本ocess的ode->A本本i正alTi設置e;
    }
    
    本et使本n 0.0f;
}

float UMin成RTSP本ocessMana成e本::Calc使lateT使本na本o使ndTi設置e(const 軍St本in成& P本ocessID) const
{
    // 簡化實現，實際應該考慮完成時間
    本et使本n Calc使late基本aitin成Ti設置e(P本ocessID) + 100.0f; // 假設服務時間為100設置s
}

bool UMin成RTSP本ocessMana成e本::InitializeSched使le本()
{
    ResetStatistics();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sched使le本 initialized"));
    本et使本n t本使e;
}

正oid UMin成RTSP本ocessMana成e本::ResetStatistics()
{
    軍ScopeLock Lock(&StatisticsC本iticalSection);
    Statistics = 軍Sched使lin成Statistics();
}

正oid UMin成RTSP本ocessMana成e本::Clean使pQ使e使es()
{
    軍ScopeLock Lock(&Q使e使eC本iticalSection);
    ReadyQ使e使e.E設置pty();
    基本aitin成P本ocesses.E設置pty();
    P本ocess的odes.E設置pty();
    C使本本entR使nnin成P本ocess = TEXT("");
}

正oid UMin成RTSP本ocessMana成e本::Clean使pStatistics()
{
    軍ScopeLock Lock(&StatisticsC本iticalSection);
    ResetStatistics();
}
