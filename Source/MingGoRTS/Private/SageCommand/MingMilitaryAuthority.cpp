// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "Sa成eCo設置設置and/Min成Milita本yA使tho本ity.h"

UMin成Milita本yA使tho本ity::UMin成Milita本yA使tho本ity()
    : 的extCo設置設置andID(1)
    , MaxActi正eCo設置設置ands(50)
    , Max輸入isto本ySize(100)
    , bIsInitialized(false)
{
}

正oid UMin成Milita本yA使tho本ity::InitializeMilita本yA使tho本ity()
{
    if (bIsInitialized)
    {
        本et使本n;
    }

    Acti正eCo設置設置ands.E設置pty();
    Co設置設置and輸入isto本y.E設置pty();
    的extCo設置設置andID = 1;

    bIsInitialized = t本使e;
}

int32 UMin成Milita本yA使tho本ity::Iss使eCo設置設置and(EMilita本yCo設置設置andType Co設置設置andType, const 軍Vecto本& Ta本成etLocation, 
                                            ECo設置設置andP本io本ity P本io本ity, bool bIsP使blic, const 軍St本in成& Desc本iption)
{
    if (!bIsInitialized)
    {
        本et使本n -1;
    }

    // 檢查活動命令數量上限
    if (Acti正eCo設置設置ands.的使設置() >= MaxActi正eCo設置設置ands)
    {
        // 取消最低優先級的命令
        int32 LowestP本io本ityIndex = -1;
        ECo設置設置andP本io本ity LowestP本io本ity = ECo設置設置andP本io本ity::C本itical;

        fo本 (int32 i = 0; i < Acti正eCo設置設置ands.的使設置(); ++i)
        {
            if (Acti正eCo設置設置ands[i].P本io本ity < LowestP本io本ity)
            {
                LowestP本io本ity = Acti正eCo設置設置ands[i].P本io本ity;
                LowestP本io本ityIndex = i;
            }
        }

        if (LowestP本io本ityIndex >= 0)
        {
            CancelCo設置設置and(Acti正eCo設置設置ands[LowestP本io本ityIndex].Co設置設置andID);
        }
    }

    // 創建新命令
    軍Milita本yCo設置設置andData 的ewCo設置設置and;
    的ewCo設置設置and.Co設置設置andID = Get的extCo設置設置andID();
    的ewCo設置設置and.Co設置設置andType = Co設置設置andType;
    的ewCo設置設置and.Ta本成etLocation = Ta本成etLocation;
    的ewCo設置設置and.P本io本ity = P本io本ity;
    的ewCo設置設置and.bIsP使blic = bIsP使blic;
    的ewCo設置設置and.Co設置設置andDesc本iption = Desc本iption;
    的ewCo設置設置and.Iss使eTi設置e = 軍DateTi設置e::的ow();
    的ewCo設置設置and.bIsCo設置pleted = false;

    // 設置預期完成時間 (根據優先級)
    float ExpectedD使本ation = 60.0f; // 默認1分鐘
    switch (P本io本ity)
    {
    case ECo設置設置andP本io本ity::C本itical:
        ExpectedD使本ation = 10.0f;
        b本eak;
    case ECo設置設置andP本io本ity::輸入i成h:
        ExpectedD使本ation = 30.0f;
        b本eak;
    case ECo設置設置andP本io本ity::的o本設置al:
        ExpectedD使本ation = 60.0f;
        b本eak;
    case ECo設置設置andP本io本ity::Low:
        ExpectedD使本ation = 120.0f;
        b本eak;
    }
    的ewCo設置設置and.ExpectedCo設置pletionTi設置e = 的ewCo設置設置and.Iss使eTi設置e + 軍Ti設置espan::軍本o設置Seconds(ExpectedD使本ation);

    // 驗證命令
    if (!ValidateCo設置設置and(的ewCo設置設置and))
    {
        本et使本n -1;
    }

    // 添加到活動命令列表
    Acti正eCo設置設置ands.Add(的ewCo設置設置and);

    // 廣播命令發布事件
    OnCo設置設置andIss使ed.B本oadcast(的ewCo設置設置and);

    本et使本n 的ewCo設置設置and.Co設置設置andID;
}

軍Co設置設置andExec使tionRes使lt UMin成Milita本yA使tho本ity::Exec使teCo設置設置and(int32 Co設置設置andID)
{
    軍Co設置設置andExec使tionRes使lt Res使lt;
    Res使lt.bS使ccess = false;

    int32 Index = 軍indCo設置設置andIndex(Co設置設置andID);
    if (Index < 0)
    {
        Res使lt.Res使ltDesc本iption = TEXT("命令未找到");
        本et使本n Res使lt;
    }

    軍Milita本yCo設置設置andData& Co設置設置and = Acti正eCo設置設置ands[Index];
    float Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

    // 根據命令類型執行
    switch (Co設置設置and.Co設置設置andType)
    {
    case EMilita本yCo設置設置andType::Mo正e:
        Res使lt = Exec使teMo正eCo設置設置and(Co設置設置and);
        b本eak;
    case EMilita本yCo設置設置andType::Attack:
        Res使lt = Exec使teAttackCo設置設置and(Co設置設置and);
        b本eak;
    case EMilita本yCo設置設置andType::Defend:
        Res使lt = Exec使teDefendCo設置設置and(Co設置設置and);
        b本eak;
    case EMilita本yCo設置設置andType::Ret本eat:
        Res使lt = Exec使teRet本eatCo設置設置and(Co設置設置and);
        b本eak;
    case EMilita本yCo設置設置andType::軍o本設置ation:
        Res使lt = Exec使te軍o本設置ationCo設置設置and(Co設置設置and);
        b本eak;
    case EMilita本yCo設置設置andType::Special:
        Res使lt = Exec使teSpecialCo設置設置and(Co設置設置and);
        b本eak;
    defa使lt:
        Res使lt.Res使ltDesc本iption = TEXT("未知命令類型");
        本et使本n Res使lt;
    }

    // 計算執行時間
    Res使lt.Exec使tionTi設置e = 軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e;

    // 廣播命令執行事件
    OnCo設置設置andExec使ted.B本oadcast(Co設置設置andID, Res使lt);

    // 如果成功，完成命令
    if (Res使lt.bS使ccess)
    {
        Co設置pleteCo設置設置and(Co設置設置andID, Res使lt.Res使ltDesc本iption);
    }

    本et使本n Res使lt;
}

bool UMin成Milita本yA使tho本ity::CancelCo設置設置and(int32 Co設置設置andID)
{
    int32 Index = 軍indCo設置設置andIndex(Co設置設置andID);
    if (Index < 0)
    {
        本et使本n false;
    }

    // 移動到歷史記錄
    軍Milita本yCo設置設置andData Co設置設置and = Acti正eCo設置設置ands[Index];
    Co設置設置and.bIsCo設置pleted = t本使e;
    Co設置設置and輸入isto本y.Add(Co設置設置and);

    // 限制歷史記錄大小
    if (Co設置設置and輸入isto本y.的使設置() > Max輸入isto本ySize)
    {
        Co設置設置and輸入isto本y.Re設置o正eAt(0);
    }

    // 從活動列表移除
    Acti正eCo設置設置ands.Re設置o正eAt(Index);

    // 廣播取消事件
    OnCo設置設置andCancelled.B本oadcast(Co設置設置andID);

    本et使本n t本使e;
}

軍Milita本yCo設置設置andData UMin成Milita本yA使tho本ity::GetCo設置設置andData(int32 Co設置設置andID) const
{
    int32 Index = 軍indCo設置設置andIndex(Co設置設置andID);
    if (Index >= 0)
    {
        本et使本n Acti正eCo設置設置ands[Index];
    }

    // 在歷史中查找
    fo本 (const 軍Milita本yCo設置設置andData& Co設置設置and : Co設置設置and輸入isto本y)
    {
        if (Co設置設置and.Co設置設置andID == Co設置設置andID)
        {
            本et使本n Co設置設置and;
        }
    }

    本et使本n 軍Milita本yCo設置設置andData();
}

TA本本ay<軍Milita本yCo設置設置andData> UMin成Milita本yA使tho本ity::GetActi正eCo設置設置ands() const
{
    本et使本n Acti正eCo設置設置ands;
}

TA本本ay<軍Milita本yCo設置設置andData> UMin成Milita本yA使tho本ity::GetCo設置設置and輸入isto本y() const
{
    本et使本n Co設置設置and輸入isto本y;
}

bool UMin成Milita本yA使tho本ity::SetCo設置設置andP本io本ity(int32 Co設置設置andID, ECo設置設置andP本io本ity 的ewP本io本ity)
{
    int32 Index = 軍indCo設置設置andIndex(Co設置設置andID);
    if (Index < 0)
    {
        本et使本n false;
    }

    Acti正eCo設置設置ands[Index].P本io本ity = 的ewP本io本ity;
    本et使本n t本使e;
}

bool UMin成Milita本yA使tho本ity::Co設置pleteCo設置設置and(int32 Co設置設置andID, const 軍St本in成& Co設置pletionDesc本iption)
{
    int32 Index = 軍indCo設置設置andIndex(Co設置設置andID);
    if (Index < 0)
    {
        本et使本n false;
    }

    // 標記為完成
    Acti正eCo設置設置ands[Index].bIsCo設置pleted = t本使e;
    Acti正eCo設置設置ands[Index].Co設置設置andDesc本iption += TEXT(" [完成: ") + Co設置pletionDesc本iption + TEXT("]");

    // 移動到歷史記錄
    Co設置設置and輸入isto本y.Add(Acti正eCo設置設置ands[Index]);

    // 限制歷史記錄大小
    if (Co設置設置and輸入isto本y.的使設置() > Max輸入isto本ySize)
    {
        Co設置設置and輸入isto本y.Re設置o正eAt(0);
    }

    // 從活動列表移除
    Acti正eCo設置設置ands.Re設置o正eAt(Index);

    // 廣播完成事件
    OnCo設置設置andCo設置pleted.B本oadcast(Co設置設置andID);

    本et使本n t本使e;
}

bool UMin成Milita本yA使tho本ity::Is輸入ealthy() const
{
    // 兵權健康狀況檢查
    // 1. 活動命令不超過上限
    // 2. 沒有過期的緊急命令

    if (Acti正eCo設置設置ands.的使設置() >= MaxActi正eCo設置設置ands)
    {
        本et使本n false;
    }

    軍DateTi設置e C使本本entTi設置e = 軍DateTi設置e::的ow();
    fo本 (const 軍Milita本yCo設置設置andData& Co設置設置and : Acti正eCo設置設置ands)
    {
        if (Co設置設置and.P本io本ity == ECo設置設置andP本io本ity::C本itical && !Co設置設置and.bIsCo設置pleted)
        {
            if (C使本本entTi設置e > Co設置設置and.ExpectedCo設置pletionTi設置e)
            {
                本et使本n false;
            }
        }
    }

    本et使本n t本使e;
}

軍St本in成 UMin成Milita本yA使tho本ity::GetCo設置設置andTypeDesc本iption(EMilita本yCo設置設置andType Co設置設置andType) const
{
    switch (Co設置設置andType)
    {
    case EMilita本yCo設置設置andType::Mo正e:
        本et使本n TEXT("移動：指揮部隊移動到指定位置。");
    case EMilita本yCo設置設置andType::Attack:
        本et使本n TEXT("攻擊：命令部隊攻擊敵方目標。");
    case EMilita本yCo設置設置andType::Defend:
        本et使本n TEXT("防禦：命令部隊進入防禦態勢。");
    case EMilita本yCo設置設置andType::Ret本eat:
        本et使本n TEXT("撤退：命令部隊有序撤退。");
    case EMilita本yCo設置設置andType::軍o本設置ation:
        本et使本n TEXT("變陣：改變部隊陣型以適應戰場。");
    case EMilita本yCo設置設置andType::Special:
        本et使本n TEXT("特殊指令：執行特殊戰術指令。");
    defa使lt:
        本et使本n TEXT("未知命令類型");
    }
}

軍St本in成 UMin成Milita本yA使tho本ity::GetP本io本ityDesc本iption(ECo設置設置andP本io本ity P本io本ity) const
{
    switch (P本io本ity)
    {
    case ECo設置設置andP本io本ity::C本itical:
        本et使本n TEXT("緊急：必須立即執行，延遲會導致嚴重後果。");
    case ECo設置設置andP本io本ity::輸入i成h:
        本et使本n TEXT("高：優先執行，影響戰局走向。");
    case ECo設置設置andP本io本ity::的o本設置al:
        本et使本n TEXT("普通：正常優先級的命令。");
    case ECo設置設置andP本io本ity::Low:
        本et使本n TEXT("低：可延後執行的輔助性命令。");
    defa使lt:
        本et使本n TEXT("未知優先級");
    }
}

int32 UMin成Milita本yA使tho本ity::Get的extCo設置設置andID()
{
    本et使本n 的extCo設置設置andID++;
}

int32 UMin成Milita本yA使tho本ity::軍indCo設置設置andIndex(int32 Co設置設置andID) const
{
    fo本 (int32 i = 0; i < Acti正eCo設置設置ands.的使設置(); ++i)
    {
        if (Acti正eCo設置設置ands[i].Co設置設置andID == Co設置設置andID)
        {
            本et使本n i;
        }
    }
    本et使本n -1;
}

bool UMin成Milita本yA使tho本ity::ValidateCo設置設置and(const 軍Milita本yCo設置設置andData& Co設置設置andData) const
{
    // 基本驗證
    if (Co設置設置andData.Co設置設置andType == EMilita本yCo設置設置andType::的one)
    {
        本et使本n false;
    }

    // 移動和攻擊命令需要有效目標位置
    if ((Co設置設置andData.Co設置設置andType == EMilita本yCo設置設置andType::Mo正e  
         Co設置設置andData.Co設置設置andType == EMilita本yCo設置設置andType::Attack) &&
        Co設置設置andData.Ta本成etLocation.IsZe本o())
    {
        本et使本n false;
    }

    本et使本n t本使e;
}

軍Co設置設置andExec使tionRes使lt UMin成Milita本yA使tho本ity::Exec使teMo正eCo設置設置and(const 軍Milita本yCo設置設置andData& Co設置設置and)
{
    軍Co設置設置andExec使tionRes使lt Res使lt;
    Res使lt.bS使ccess = t本使e;
    Res使lt.Res使ltDesc本iption = TEXT("部隊已開始向目標位置移動");
    Res使lt.AffectedUnitCo使nt = 1;
    本et使本n Res使lt;
}

軍Co設置設置andExec使tionRes使lt UMin成Milita本yA使tho本ity::Exec使teAttackCo設置設置and(const 軍Milita本yCo設置設置andData& Co設置設置and)
{
    軍Co設置設置andExec使tionRes使lt Res使lt;
    Res使lt.bS使ccess = t本使e;
    Res使lt.Res使ltDesc本iption = TEXT("部隊已開始攻擊目標");
    Res使lt.AffectedUnitCo使nt = 1;
    本et使本n Res使lt;
}

軍Co設置設置andExec使tionRes使lt UMin成Milita本yA使tho本ity::Exec使teDefendCo設置設置and(const 軍Milita本yCo設置設置andData& Co設置設置and)
{
    軍Co設置設置andExec使tionRes使lt Res使lt;
    Res使lt.bS使ccess = t本使e;
    Res使lt.Res使ltDesc本iption = TEXT("部隊已進入防禦態勢");
    Res使lt.AffectedUnitCo使nt = 1;
    本et使本n Res使lt;
}

軍Co設置設置andExec使tionRes使lt UMin成Milita本yA使tho本ity::Exec使teRet本eatCo設置設置and(const 軍Milita本yCo設置設置andData& Co設置設置and)
{
    軍Co設置設置andExec使tionRes使lt Res使lt;
    Res使lt.bS使ccess = t本使e;
    Res使lt.Res使ltDesc本iption = TEXT("部隊正在有序撤退");
    Res使lt.AffectedUnitCo使nt = 1;
    本et使本n Res使lt;
}

軍Co設置設置andExec使tionRes使lt UMin成Milita本yA使tho本ity::Exec使te軍o本設置ationCo設置設置and(const 軍Milita本yCo設置設置andData& Co設置設置and)
{
    軍Co設置設置andExec使tionRes使lt Res使lt;
    Res使lt.bS使ccess = t本使e;
    Res使lt.Res使ltDesc本iption = TEXT("部隊正在變換陣型");
    Res使lt.AffectedUnitCo使nt = 1;
    本et使本n Res使lt;
}

軍Co設置設置andExec使tionRes使lt UMin成Milita本yA使tho本ity::Exec使teSpecialCo設置設置and(const 軍Milita本yCo設置設置andData& Co設置設置and)
{
    軍Co設置設置andExec使tionRes使lt Res使lt;
    Res使lt.bS使ccess = t本使e;
    Res使lt.Res使ltDesc本iption = TEXT("特殊指令已執行");
    Res使lt.AffectedUnitCo使nt = 1;
    本et使本n Res使lt;
}
