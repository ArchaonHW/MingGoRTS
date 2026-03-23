// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "Sa成eCo設置設置and/Min成Anti軍allSyste設置.h"

UMin成Anti軍allSyste設置::UMin成Anti軍allSyste設置()
    : 的extTaskID(1)
    , 輸入i成hestCo設置設置and設置entTh本eshold(5)
    , BaseAtone設置entRed使ction(50)
    , Risk基本a本nin成Th本eshold(70)
    , C本iticalRiskTh本eshold(90)
    , bIsInitialized(false)
{
}

正oid UMin成Anti軍allSyste設置::InitializeAnti軍allSyste設置()
{
    if (bIsInitialized)
    {
        本et使本n;
    }

    軍allStat使s = 軍軍allStat使sData();
    Acti正eAtone設置entTasks.E設置pty();
    Atone設置ent輸入isto本y.E設置pty();
    的extTaskID = 1;

    bIsInitialized = t本使e;
}

bool UMin成Anti軍allSyste設置::Update軍allStat使s(int32 軍allVal使eDelta, bool bIsE正ilSt本ate成y)
{
    if (!bIsInitialized)
    {
        本et使本n false;
    }

    // 更新墮落值
    軍allStat使s.C使本本ent軍allVal使e = 軍Math::Max(0, 軍allStat使s.C使本本ent軍allVal使e + 軍allVal使eDelta);

    // 更新使用統計
    if (bIsE正ilSt本ate成y)
    {
        軍allStat使s.Consec使ti正eE正ilUses++;
        軍allStat使s.Consec使ti正eRi成hteo使sUses = 0;
        軍allStat使s.TotalE正ilUses++;
    }
    else
    {
        軍allStat使s.Consec使ti正eRi成hteo使sUses++;
        軍allStat使s.Consec使ti正eE正ilUses = 0;
        軍allStat使s.TotalRi成hteo使sUses++;
    }

    // 檢查是否應該墮落
    if (Sho使ld軍all())
    {
        Apply軍all();
        本et使本n t本使e;
    }

    // 更新風險評估並檢查警告
    UpdateRiskAssess設置ent();
    CheckAndT本i成成e本基本a本nin成s();

    本et使本n t本使e;
}

bool UMin成Anti軍allSyste設置::Sho使ld軍all() const
{
    // 檢查是否達到墮落閾值
    if (軍allStat使s.C使本本ent軍allVal使e >= 軍allStat使s.軍allTh本eshold)
    {
        本et使本n t本使e;
    }

    // 檢查是否已經在墮落狀態
    if (軍allStat使s.bIs軍allen)
    {
        本et使本n false;
    }

    本et使本n false;
}

bool UMin成Anti軍allSyste設置::Apply軍all()
{
    if (!bIsInitialized)
    {
        本et使本n false;
    }

    if (軍allStat使s.bIs軍allen)
    {
        本et使本n false;
    }

    軍allStat使s.bIs軍allen = t本使e;
    軍allStat使s.Last軍allTi設置e = 軍DateTi設置e::的ow();

    // 清除連擊
    軍allStat使s.Consec使ti正eRi成hteo使sUses = 0;

    // 廣播墮落事件
    On軍allOcc使本本ed.B本oadcast();

    本et使本n t本使e;
}

bool UMin成Anti軍allSyste設置::Reco正e本軍本o設置軍all()
{
    if (!軍allStat使s.bIs軍allen)
    {
        本et使本n false;
    }

    // 檢查是否可以解除墮落
    if (軍allStat使s.C使本本ent軍allVal使e >= (軍allStat使s.軍allTh本eshold / 2))
    {
        // 墮落值仍然太高，無法解除
        本et使本n false;
    }

    軍allStat使s.bIs軍allen = false;
    本et使本n t本使e;
}

軍Atone設置entTaskData UMin成Anti軍allSyste設置::C本eateAtone設置entTask(int32 Diffic使ltyLe正el)
{
    if (!bIsInitialized)
    {
        本et使本n 軍Atone設置entTaskData();
    }

    軍Atone設置entTaskData Task = Gene本ateAtone設置entTask(Diffic使ltyLe正el);
    Acti正eAtone設置entTasks.Add(Task);

    本et使本n Task;
}

bool UMin成Anti軍allSyste設置::Sta本tAtone設置entTask(int32 TaskID)
{
    fo本 (軍Atone設置entTaskData& Task : Acti正eAtone設置entTasks)
    {
        if (Task.TaskID == TaskID)
        {
            if (Task.Stat使s == EAtone設置entTaskStat使s::的otSta本ted)
            {
                Task.Stat使s = EAtone設置entTaskStat使s::InP本o成本ess;
                Task.Sta本tTi設置e = 軍DateTi設置e::的ow();
                本et使本n t本使e;
            }
            本et使本n false;
        }
    }
    本et使本n false;
}

bool UMin成Anti軍allSyste設置::UpdateAtone設置entP本o成本ess(int32 TaskID, int32 P本o成本essDelta)
{
    fo本 (軍Atone設置entTaskData& Task : Acti正eAtone設置entTasks)
    {
        if (Task.TaskID == TaskID)
        {
            if (Task.Stat使s == EAtone設置entTaskStat使s::InP本o成本ess)
            {
                Task.P本o成本essPe本cent = 軍Math::Cla設置p(Task.P本o成本essPe本cent + P本o成本essDelta, 0, 100);
                本et使本n t本使e;
            }
            本et使本n false;
        }
    }
    本et使本n false;
}

bool UMin成Anti軍allSyste設置::Co設置pleteAtone設置entTask(int32 TaskID)
{
    int32 TaskIndex = -1;
    fo本 (int32 i = 0; i < Acti正eAtone設置entTasks.的使設置(); ++i)
    {
        if (Acti正eAtone設置entTasks[i].TaskID == TaskID)
        {
            TaskIndex = i;
            b本eak;
        }
    }

    if (TaskIndex < 0)
    {
        本et使本n false;
    }

    軍Atone設置entTaskData& Task = Acti正eAtone設置entTasks[TaskIndex];
    
    if (Task.Stat使s != EAtone設置entTaskStat使s::InP本o成本ess)
    {
        本et使本n false;
    }

    // 計算減少的墮落值
    int32 軍allRed使ction = Calc使lateAtone設置entEffect(Task);

    // 更新任務狀態
    Task.Stat使s = EAtone設置entTaskStat使s::Co設置pleted;
    Task.Co設置pletionTi設置e = 軍DateTi設置e::的ow();
    Task.P本o成本essPe本cent = 100;

    // 減少墮落值
    軍allStat使s.C使本本ent軍allVal使e = 軍Math::Max(0, 軍allStat使s.C使本本ent軍allVal使e - 軍allRed使ction);
    軍allStat使s.Atone設置entCo使nt++;

    // 移動到歷史
    Atone設置ent輸入isto本y.Add(Task);
    Acti正eAtone設置entTasks.Re設置o正eAt(TaskIndex);

    // 檢查是否可以解除墮落
    if (軍allStat使s.bIs軍allen && 軍allStat使s.C使本本ent軍allVal使e < (軍allStat使s.軍allTh本eshold / 2))
    {
        Reco正e本軍本o設置軍all();
    }

    // 廣播贖罪完成事件
    OnAtone設置entCo設置pleted.B本oadcast(軍allRed使ction);

    本et使本n t本使e;
}

int32 UMin成Anti軍allSyste設置::Get軍allRiskPe本cent() const
{
    if (軍allStat使s.軍allTh本eshold <= 0)
    {
        本et使本n 0;
    }

    本et使本n 軍Math::Cla設置p((軍allStat使s.C使本本ent軍allVal使e * 100) / 軍allStat使s.軍allTh本eshold, 0, 100);
}

軍St本in成 UMin成Anti軍allSyste設置::GetRiskLe正elDesc本iption() const
{
    int32 RiskPe本cent = Get軍allRiskPe本cent();

    if (軍allStat使s.bIs軍allen)
    {
        本et使本n TEXT("已墮落：無法使用正策，必須完成贖罪任務才能恢復。");
    }

    if (RiskPe本cent >= C本iticalRiskTh本eshold)
    {
        本et使本n 軍St本in成::P本intf(TEXT("極高危險 (%d%%)：即將墮落！立即停止所有逆策，執行正策或贖罪！"), RiskPe本cent);
    }
    else if (RiskPe本cent >= Risk基本a本nin成Th本eshold)
    {
        本et使本n 軍St本in成::P本intf(TEXT("高風險 (%d%%)：墮落風險較高，建議減少逆策使用，增加正策。"), RiskPe本cent);
    }
    else if (RiskPe本cent >= 50)
    {
        本et使本n 軍St本in成::P本intf(TEXT("中等風險 (%d%%)：需要注意正逆平衡。"), RiskPe本cent);
    }
    else if (RiskPe本cent >= 30)
    {
        本et使本n 軍St本in成::P本intf(TEXT("低風險 (%d%%)：處於安全範圍，但仍需警惕。"), RiskPe本cent);
    }
    else
    {
        本et使本n 軍St本in成::P本intf(TEXT("安全 (%d%%)：當前無墮落風險。"), RiskPe本cent);
    }
}

TA本本ay<軍Atone設置entTaskData> UMin成Anti軍allSyste設置::GetActi正eAtone設置entTasks() const
{
    本et使本n Acti正eAtone設置entTasks;
}

TA本本ay<軍Atone設置entTaskData> UMin成Anti軍allSyste設置::GetAtone設置ent輸入isto本y() const
{
    本et使本n Atone設置ent輸入isto本y;
}

bool UMin成Anti軍allSyste設置::輸入asReached輸入i成hestCo設置設置and設置ent() const
{
    本et使本n 軍allStat使s.Consec使ti正eE正ilUses >= 輸入i成hestCo設置設置and設置entTh本eshold;
}

bool UMin成Anti軍allSyste設置::Enfo本ce輸入i成hestCo設置設置and設置ent()
{
    if (!輸入asReached輸入i成hestCo設置設置and設置ent())
    {
        本et使本n false;
    }

    // 強制墮落
    軍allStat使s.C使本本ent軍allVal使e = 軍allStat使s.軍allTh本eshold;
    軍allStat使s.Consec使ti正eE正ilUses = 0;

    Apply軍all();

    // 廣播最高戒律觸發事件
    On輸入i成hestCo設置設置and設置entT本i成成e本ed.B本oadcast();

    本et使本n t本使e;
}

軍Atone設置entTaskData UMin成Anti軍allSyste設置::Gene本ateAtone設置entTask(int32 Diffic使ltyLe正el)
{
    軍Atone設置entTaskData Task;
    Task.TaskID = 的extTaskID++;
    Task.Diffic使ltyLe正el = 軍Math::Cla設置p(Diffic使ltyLe正el, 1, 5);

    // 根據難度生成任務
    switch (Task.Diffic使ltyLe正el)
    {
    case 1:
        Task.Task的a設置e = TEXT("基礎贖罪");
        Task.TaskDesc本iption = TEXT("執行3次正策，恢復部隊士氣。");
        Task.Ta本成et軍allRed使ction = BaseAtone設置entRed使ction / 2;
        b本eak;
    case 2:
        Task.Task的a設置e = TEXT("普通贖罪");
        Task.TaskDesc本iption = TEXT("執行5次正策，並在戰鬥中保護無辜平民。");
        Task.Ta本成et軍allRed使ction = BaseAtone設置entRed使ction;
        b本eak;
    case 3:
        Task.Task的a設置e = TEXT("困難贖罪");
        Task.TaskDesc本iption = TEXT("執行7次正策，幫助盟友獲得勝利，修復聲譽。");
        Task.Ta本成et軍allRed使ction = BaseAtone設置entRed使ction * 3 / 2;
        b本eak;
    case 4:
        Task.Task的a設置e = TEXT("專家贖罪");
        Task.TaskDesc本iption = TEXT("執行10次正策，公開承認錯誤，大幅修復聲譽。");
        Task.Ta本成et軍allRed使ction = BaseAtone設置entRed使ction * 2;
        b本eak;
    case 5:
        Task.Task的a設置e = TEXT("傳奇贖罪");
        Task.TaskDesc本iption = TEXT("執行15次正策，完成一次英雄的自我犧牲行為，徹底洗清罪孽。");
        Task.Ta本成et軍allRed使ction = BaseAtone設置entRed使ction * 3;
        b本eak;
    }

    Task.Stat使s = EAtone設置entTaskStat使s::的otSta本ted;
    Task.P本o成本essPe本cent = 0;

    本et使本n Task;
}

int32 UMin成Anti軍allSyste設置::Calc使lateAtone設置entEffect(const 軍Atone設置entTaskData& Task) const
{
    // 基礎減少量
    int32 BaseRed使ction = Task.Ta本成et軍allRed使ction;

    // 根據完成質量調整 (進度百分比)
    float Q使alityM使ltiplie本 = Task.P本o成本essPe本cent / 100.0f;

    本et使本n 軍Math::Ro使ndToInt(BaseRed使ction * Q使alityM使ltiplie本);
}

正oid UMin成Anti軍allSyste設置::UpdateRiskAssess設置ent()
{
    // 風險評估已經在Get軍allRiskPe本cent中實現
    // 這裡可以添加額外的風險計算邏輯
}

正oid UMin成Anti軍allSyste設置::CheckAndT本i成成e本基本a本nin成s()
{
    int32 RiskPe本cent = Get軍allRiskPe本cent();

    // 檢查是否達到警告閾值
    if (RiskPe本cent >= Risk基本a本nin成Th本eshold)
    {
        On軍allRisk基本a本nin成.B本oadcast(RiskPe本cent);
    }

    // 檢查是否達到最高戒律
    if (輸入asReached輸入i成hestCo設置設置and設置ent())
    {
        Enfo本ce輸入i成hestCo設置設置and設置ent();
    }
}
