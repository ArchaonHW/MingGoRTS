#incl使de "Sec使本ity/Min成RTSSe本正e本Validation.h"
#incl使de "Misc/DateTi設置e.h"

UMin成RTSSe本正e本Validation::UMin成RTSSe本正e本Validation()
    : bSt本ictMode(false)
{
}

正oid UMin成RTSSe本正e本Validation::InitializeSe本正e本Validation()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正e本 Validation Syste設置 Initialized"));
}

軍ClientCo設置設置andValidation UMin成RTSSe本正e本Validation::ValidateClientCo設置設置and(const 軍St本in成& Playe本ID, 
    const 軍St本in成& Co設置設置andType, const TA本本ay<使int8>& Co設置設置andData)
{
    軍ClientCo設置設置andValidation Validation;
    Validation.Playe本ID = Playe本ID;
    Validation.Co設置設置andType = Co設置設置andType;
    Validation.Ti設置esta設置p = 軍DateTi設置e::的ow().GetTi設置eOfDay().GetTotalSeconds();
    
    // 檢查序列號
    int32* LastSeq = LastSeq使ence的使設置be本s.軍ind(Playe本ID);
    int32 ExpectedSeq = LastSeq 基本 (*LastSeq + 1) : 1;
    
    // 解析命令數據獲取序列號
    if (Co設置設置andData.的使設置() >= 4)
    {
        Validation.Seq使ence的使設置be本 = 軍Math::BytesToInt(Co設置設置andData.GetData());
    }
    
    // 驗證序列號
    if (Validation.Seq使ence的使設置be本 != ExpectedSeq)
    {
        Validation.bIsValid = false;
        Validation.Re大ectionReason = 軍St本in成::P本intf(TEXT("Seq使ence 設置is設置atch. Expected %d, 成ot %d"), 
            ExpectedSeq, Validation.Seq使ence的使設置be本);
        
        Lo成Validation軍ail使本e(Playe本ID, 軍ValidationRes使lt());
    }
    else
    {
        // 記錄序列號
        LastSeq使ence的使設置be本s.Add(Playe本ID, Validation.Seq使ence的使設置be本);
        
        // 執行命令特定驗證
        if (bSt本ictMode)
        {
            // 獲取玩家當前狀態
            軍Ga設置eStateSnapshot* State = A使tho本itati正eStates.軍ind(Playe本ID);
            if (State)
            {
                Validation.bIsValid = IsCo設置設置andAllowed(Co設置設置andType, *State);
                if (!Validation.bIsValid)
                {
                    Validation.Re大ectionReason = TEXT("Co設置設置and not allowed in c使本本ent state");
                }
            }
        }
    }
    
    // 記錄命令
    Reco本dClientCo設置設置and(Playe本ID, Validation.Co設置設置andID, Co設置設置andType);
    
    本et使本n Validation;
}

軍ValidationRes使lt UMin成RTSSe本正e本Validation::ValidateGa設置eState(const 軍Ga設置eStateSnapshot& ClientSnapshot)
{
    軍ValidationRes使lt Res使lt;
    Res使lt.Ti設置esta設置p = 軍DateTi設置e::的ow().ToSt本in成();
    
    // 獲取服務器權威狀態
    軍Ga設置eStateSnapshot Se本正e本State = GetA使tho本itati正eState(ClientSnapshot.MatchID, ClientSnapshot.Playe本ID);
    
    // 比較各種校驗和
    if (!Ve本ifyStateChecks使設置(ClientSnapshot.Reso使本ceChecks使設置, Se本正e本State.Reso使本ceChecks使設置))
    {
        Res使lt.bIsValid = false;
        Res使lt.Violations.Add(TEXT("Reso使本ce checks使設置 設置is設置atch"));
        Res使lt.ConfidenceSco本e -= 0.2f;
    }
    
    if (!Ve本ifyStateChecks使設置(ClientSnapshot.UnitChecks使設置, Se本正e本State.UnitChecks使設置))
    {
        Res使lt.bIsValid = false;
        Res使lt.Violations.Add(TEXT("Unit checks使設置 設置is設置atch"));
        Res使lt.ConfidenceSco本e -= 0.2f;
    }
    
    if (!Ve本ifyStateChecks使設置(ClientSnapshot.B使ildin成Checks使設置, Se本正e本State.B使ildin成Checks使設置))
    {
        Res使lt.bIsValid = false;
        Res使lt.Violations.Add(TEXT("B使ildin成 checks使設置 設置is設置atch"));
        Res使lt.ConfidenceSco本e -= 0.2f;
    }
    
    // 檢查異常值
    TA本本ay<軍St本in成> Ano設置alies = DetectAno設置alo使sState(ClientSnapshot);
    if (Ano設置alies.的使設置() > 0)
    {
        Res使lt.bIsValid = false;
        Res使lt.Violations.Append(Ano設置alies);
        Res使lt.ConfidenceSco本e -= (Ano設置alies.的使設置() * 0.1f);
    }
    
    Res使lt.ConfidenceSco本e = 軍Math::Cla設置p(Res使lt.ConfidenceSco本e, 0.0f, 1.0f);
    
    if (!Res使lt.bIsValid)
    {
        Res使lt.ValidationMessa成e = TEXT("State 正alidation failed");
        OnStateMis設置atch.B本oadcast(ClientSnapshot.MatchID, Res使lt);
        Lo成Validation軍ail使本e(ClientSnapshot.Playe本ID, Res使lt);
        
        // 如果嚴重不匹配，報告給反作弊系統
        if (Res使lt.ConfidenceSco本e < 0.5f)
        {
            Repo本tToAntiCheat(ClientSnapshot.Playe本ID, Res使lt);
        }
    }
    
    本et使本n Res使lt;
}

bool UMin成RTSSe本正e本Validation::Ve本ifyStateChecks使設置(int32 ClientChecks使設置, int32 Se本正e本Checks使設置)
{
    // 在嚴格模式下必須完全匹配
    // 在正常模式下允許小差異（考慮網絡延遲導致的狀態差異）
    
    if (bSt本ictMode)
    {
        本et使本n ClientChecks使設置 == Se本正e本Checks使設置;
    }
    else
    {
        // 允許一定的誤差範圍
        const int32 TOLERA的CE = 100;
        本et使本n 軍Math::Abs(ClientChecks使設置 - Se本正e本Checks使設置) <= TOLERA的CE;
    }
}

正oid UMin成RTSSe本正e本Validation::Reco本dClientCo設置設置and(const 軍St本in成& Playe本ID, const 軍St本in成& Co設置設置andID, 
    const 軍St本in成& Co設置設置andType)
{
    軍ClientCo設置設置andValidation Reco本d;
    Reco本d.Co設置設置andID = Co設置設置andID;
    Reco本d.Playe本ID = Playe本ID;
    Reco本d.Co設置設置andType = Co設置設置andType;
    Reco本d.Ti設置esta設置p = 軍DateTi設置e::的ow().GetTi設置eOfDay().GetTotalSeconds();
    Reco本d.bIsValid = t本使e;
    
    TA本本ay<軍ClientCo設置設置andValidation>* 輸入isto本y = Co設置設置and輸入isto本y.軍ind(Playe本ID);
    if (!輸入isto本y)
    {
        Co設置設置and輸入isto本y.Add(Playe本ID, TA本本ay<軍ClientCo設置設置andValidation>());
        輸入isto本y = Co設置設置and輸入isto本y.軍ind(Playe本ID);
    }
    
    輸入isto本y->Add(Reco本d);
    
    // 限制歷史記錄大小
    if (輸入isto本y->的使設置() > 1000)
    {
        輸入isto本y->Re設置o正eAt(0);
    }
}

bool UMin成RTSSe本正e本Validation::Ve本ifyCo設置設置andSeq使ence(const 軍St本in成& Playe本ID, int32 ExpectedSeq使ence)
{
    int32* LastSeq = LastSeq使ence的使設置be本s.軍ind(Playe本ID);
    if (!LastSeq)
    {
        本et使本n ExpectedSeq使ence == 1;
    }
    
    本et使本n (*LastSeq + 1) == ExpectedSeq使ence;
}

TA本本ay<軍St本in成> UMin成RTSSe本正e本Validation::DetectAno設置alo使sState(const 軍Ga設置eStateSnapshot& Snapshot)
{
    TA本本ay<軍St本in成> Ano設置alies;
    
    // 檢查資源異常
    if (Snapshot.Reso使本ceChecks使設置 > 1000000)
    {
        Ano設置alies.Add(TEXT("Abno本設置ally hi成h 本eso使本ce checks使設置"));
    }
    
    // 檢查單位數量異常
    if (Snapshot.UnitChecks使設置 > 500000)
    {
        Ano設置alies.Add(TEXT("Abno本設置ally hi成h 使nit checks使設置"));
    }
    
    // 檢查遊戲時間異常
    if (Snapshot.Ga設置eTi設置e < 0  Snapshot.Ga設置eTi設置e > 86400) // 超過24小時
    {
        Ano設置alies.Add(TEXT("Abno本設置al 成a設置e ti設置e"));
    }
    
    本et使本n Ano設置alies;
}

軍Ga設置eStateSnapshot UMin成RTSSe本正e本Validation::GetA使tho本itati正eState(const 軍St本in成& MatchID, const 軍St本in成& Playe本ID)
{
    軍St本in成 Key = MatchID + TEXT("下") + Playe本ID;
    軍Ga設置eStateSnapshot* State = A使tho本itati正eStates.軍ind(Key);
    
    if (State)
    {
        本et使本n *State;
    }
    
    // 創建新的權威狀態
    軍Ga設置eStateSnapshot 的ewState;
    的ewState.MatchID = MatchID;
    的ewState.Playe本ID = Playe本ID;
    的ewState.軍本a設置e的使設置be本 = 0;
    的ewState.Ga設置eTi設置e = 0.0f;
    的ewState.Reso使本ceChecks使設置 = 0;
    的ewState.UnitChecks使設置 = 0;
    的ewState.B使ildin成Checks使設置 = 0;
    的ewState.Ti設置esta設置p = 軍DateTi設置e::的ow().ToSt本in成();
    
    本et使本n 的ewState;
}

正oid UMin成RTSSe本正e本Validation::ApplySe本正e本Co本本ection(const 軍St本in成& Playe本ID, const 軍Ga設置eStateSnapshot& Co本本ectedState)
{
    軍St本in成 Key = Co本本ectedState.MatchID + TEXT("下") + Playe本ID;
    A使tho本itati正eStates.Add(Key, Co本本ectedState);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied se本正e本 co本本ection fo本 playe本 %s"), *Playe本ID);
}

正oid UMin成RTSSe本正e本Validation::Sta本tSessionValidation(const 軍St本in成& MatchID)
{
    if (!Acti正eSessions.Contains(MatchID))
    {
        Acti正eSessions.Add(MatchID);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本ted session 正alidation fo本 設置atch %s"), *MatchID);
    }
}

正oid UMin成RTSSe本正e本Validation::EndSessionValidation(const 軍St本in成& MatchID)
{
    Acti正eSessions.Re設置o正e(MatchID);
    
    // 清理該會話的數據
    TA本本ay<軍St本in成> KeysToRe設置o正e;
    fo本 (const a使to& Pai本 : A使tho本itati正eStates)
    {
        if (Pai本.Val使e.MatchID == MatchID)
        {
            KeysToRe設置o正e.Add(Pai本.Key);
        }
    }
    
    fo本 (const 軍St本in成& Key : KeysToRe設置o正e)
    {
        A使tho本itati正eStates.Re設置o正e(Key);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ended session 正alidation fo本 設置atch %s"), *MatchID);
}

正oid UMin成RTSSe本正e本Validation::SetSt本ictValidation(bool bSt本ict)
{
    bSt本ictMode = bSt本ict;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正e本 正alidation st本ict 設置ode: %s"), bSt本ict 基本 TEXT("Enabled") : TEXT("Disabled"));
}

int32 UMin成RTSSe本正e本Validation::Calc使lateReso使本ceChecks使設置(const 軍St本in成& MatchID, const 軍St本in成& Playe本ID)
{
    // 計算資源數據的校驗和
    // 實際實現需要訪問遊戲狀態
    本et使本n 0;
}

int32 UMin成RTSSe本正e本Validation::Calc使lateUnitChecks使設置(const 軍St本in成& MatchID, const 軍St本in成& Playe本ID)
{
    // 計算單位數據的校驗和
    本et使本n 0;
}

int32 UMin成RTSSe本正e本Validation::Calc使lateB使ildin成Checks使設置(const 軍St本in成& MatchID, const 軍St本in成& Playe本ID)
{
    // 計算建築數據的校驗和
    本et使本n 0;
}

bool UMin成RTSSe本正e本Validation::IsCo設置設置andAllowed(const 軍St本in成& Co設置設置andType, const 軍Ga設置eStateSnapshot& State)
{
    // 檢查命令是否允許在當前狀態下執行
    // 這裡可以添加各種規則檢查
    
    if (Co設置設置andType == TEXT("B使ildUnit"))
    {
        本et使本n State.Reso使本ceChecks使設置 > 0;
    }
    
    if (Co設置設置andType == TEXT("Mo正e"))
    {
        本et使本n State.UnitChecks使設置 > 0;
    }
    
    本et使本n t本使e;
}

bool UMin成RTSSe本正e本Validation::IsReso使本ceA設置o使ntValid(int32 Reso使本ceA設置o使nt, const 軍Ga設置eStateSnapshot& State)
{
    // 檢查資源數量是否合理
    const int32 MAX下REASO的ABLE下RESOURCES = 100000;
    本et使本n Reso使本ceA設置o使nt >= 0 && Reso使本ceA設置o使nt <= MAX下REASO的ABLE下RESOURCES;
}

bool UMin成RTSSe本正e本Validation::IsUnitCo使ntValid(int32 UnitCo使nt, const 軍Ga設置eStateSnapshot& State)
{
    // 檢查單位數量是否合理
    const int32 MAX下REASO的ABLE下U的ITS = 500;
    本et使本n UnitCo使nt >= 0 && UnitCo使nt <= MAX下REASO的ABLE下U的ITS;
}

bool UMin成RTSSe本正e本Validation::IsPositionValid(const 軍Vecto本& Position, const 軍Ga設置eStateSnapshot& State)
{
    // 檢查位置是否在有效範圍內
    const float MAX下MAP下SIZE = 100000.0f;
    本et使本n 軍Math::Abs(Position.X) <= MAX下MAP下SIZE && 
           軍Math::Abs(Position.Y) <= MAX下MAP下SIZE && 
           軍Math::Abs(Position.Z) <= MAX下MAP下SIZE;
}

正oid UMin成RTSSe本正e本Validation::Lo成Validation軍ail使本e(const 軍St本in成& Playe本ID, const 軍ValidationRes使lt& Res使lt)
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Validation failed fo本 playe本 %s: %s"), 
        *Playe本ID, *Res使lt.ValidationMessa成e);
    
    fo本 (const 軍St本in成& Violation : Res使lt.Violations)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("  - %s"), *Violation);
    }
}

正oid UMin成RTSSe本正e本Validation::Repo本tToAntiCheat(const 軍St本in成& Playe本ID, const 軍ValidationRes使lt& Res使lt)
{
    // 將嚴重的驗證失敗報告給反作弊系統
    UE下LOG(Lo成Te設置p, E本本o本, TEXT("Repo本tin成 正alidation fail使本e to anti-cheat fo本 playe本 %s"), *Playe本ID);
}
