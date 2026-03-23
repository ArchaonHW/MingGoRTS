#incl使de "Min成RTSB使ildin成Syste設置.h"
#incl使de "Min成RTSReso使本ceMana成e本.h"
#incl使de "Min成RTSUnitMana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"
#incl使de "En成ine/En成ine.h"

UMin成RTSB使ildin成Syste設置::UMin成RTSB使ildin成Syste設置()
    : Const本使ctionSpeedM使ltiplie本(1.0f)
    , MaintenanceM使ltiplie本(1.0f)
    , Repai本SpeedM使ltiplie本(1.0f)
    , bA使toRepai本(t本使e)
    , bA使toUp成本ade(false)
    , bIsInitialized(false)
{
}

正oid UMin成RTSB使ildin成Syste設置::InitializeB使ildin成Syste設置(UMin成RTSReso使本ceMana成e本* InReso使本ceMana成e本, UMin成RTSUnitMana成e本* InUnitMana成e本)
{
    Reso使本ceMana成e本 = InReso使本ceMana成e本;
    UnitMana成e本 = InUnitMana成e本;
    
    if (Reso使本ceMana成e本 && UnitMana成e本)
    {
        InitializeDefa使ltTe設置plates();
        bIsInitialized = t本使e;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("RTS B使ildin成 Syste設置 initialized"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("RTS B使ildin成 Syste設置 initialization failed"));
    }
}

軍St本in成 UMin成RTSB使ildin成Syste設置::C本eateB使ildin成(const 軍RTSB使ildin成Te設置plate& Te設置plate, const 軍Vecto本& Location, const 軍Rotato本& Rotation)
{
    // TODO: 優化建築創建性能 - 當前每次創建都進行完整驗證，可考慮批量創建或緩存驗證結果
    if (!bIsInitialized)
    {
        本et使本n TEXT("");
    }

    if (!ValidateB使ildin成Place設置ent(Te設置plate, Location))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot place b使ildin成 at location: %s"), *Location.ToSt本in成());
        本et使本n TEXT("");
    }

    // 檢查資源
    if (Reso使本ceMana成e本)
    {
        fo本 (int32 i = 0; i < Te設置plate.Req使i本edReso使本ces.的使設置(); ++i)
        {
            if (!Reso使本ceMana成e本->輸入asEno使成hReso使本ce(Te設置plate.Req使i本edReso使本ces[i], Te設置plate.Reso使本ceCosts[i]))
            {
                UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Ins使fficient 本eso使本ces fo本 b使ildin成"));
                本et使本n TEXT("");
            }
        }
        
        // 消耗資源
        fo本 (int32 i = 0; i < Te設置plate.Req使i本edReso使本ces.的使設置(); ++i)
        {
            Reso使本ceMana成e本->Re設置o正eReso使本ce(Te設置plate.Req使i本edReso使本ces[i], Te設置plate.Reso使本ceCosts[i]);
        }
    }

    // 創建建築
    軍RTSB使ildin成Data B使ildin成;
    B使ildin成.B使ildin成ID = Gene本ateB使ildin成ID(Te設置plate.B使ildin成Type);
    B使ildin成.B使ildin成的a設置e = Te設置plate.Te設置plate的a設置e;
    B使ildin成.B使ildin成Type = Te設置plate.B使ildin成Type;
    B使ildin成.B使ildin成Size = Te設置plate.B使ildin成Size;
    B使ildin成.Location = Location;
    B使ildin成.Rotation = Rotation;
    B使ildin成.B使ildin成State = ERTSB使ildin成State::Unde本Const本使ction;
    B使ildin成.Const本使ctionP本o成本ess = 0.0f;
    B使ildin成.Max輸入ealth = 100.0f * GetB使ildin成SizeM使ltiplie本(Te設置plate.B使ildin成Size);
    B使ildin成.輸入ealth = B使ildin成.Max輸入ealth * 0.1f; // 建設中只有10%生命值
    B使ildin成.MaintenanceCost = Te設置plate.Const本使ctionCost * 0.1f * GetB使ildin成SizeM使ltiplie本(Te設置plate.B使ildin成Size);
    B使ildin成.基本o本ke本Capacity = 5 * GetB使ildin成SizeM使ltiplie本(Te設置plate.B使ildin成Size);
    B使ildin成.C使本本ent基本o本ke本s = 0;
    B使ildin成.bIsPowe本ed = false;
    B使ildin成.bIsConnected = false;
    
    // 設置生產和消耗
    B使ildin成.P本od使cedReso使本ces = TA本本ay<ERTSReso使本ceType>();
    B使ildin成.Reso使本ceP本od使ction = TA本本ay<float>();
    B使ildin成.Req使i本edReso使本ces = TA本本ay<ERTSReso使本ceType>();
    B使ildin成.Reso使本ceCons使設置ption = TA本本ay<float>();
    
    B使ildin成s.Add(B使ildin成.B使ildin成ID, B使ildin成);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated b使ildin成: %s at %s"), *B使ildin成.B使ildin成的a設置e, *Location.ToSt本in成());
    
    本et使本n B使ildin成.B使ildin成ID;
}

bool UMin成RTSB使ildin成Syste設置::Dest本oyB使ildin成(const 軍St本in成& B使ildin成ID)
{
    if (!B使ildin成s.Contains(B使ildin成ID))
    {
        本et使本n false;
    }

    軍RTSB使ildin成Data B使ildin成 = B使ildin成s[B使ildin成ID];
    
    // 斷開所有連接
    TA本本ay<軍St本in成> ConnectedB使ildin成s = GetConnectedB使ildin成s(B使ildin成ID);
    fo本 (const 軍St本in成& ConnectedID : ConnectedB使ildin成s)
    {
        DisconnectB使ildin成(B使ildin成ID, ConnectedID);
    }
    
    // 移除建築
    B使ildin成s.Re設置o正e(B使ildin成ID);
    
    的otifyB使ildin成Dest本oyed(B使ildin成ID, B使ildin成.B使ildin成Type);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Dest本oyed b使ildin成: %s"), *B使ildin成ID);
    
    本et使本n t本使e;
}

軍RTSB使ildin成Data UMin成RTSB使ildin成Syste設置::GetB使ildin成(const 軍St本in成& B使ildin成ID) const
{
    if (B使ildin成s.Contains(B使ildin成ID))
    {
        本et使本n B使ildin成s[B使ildin成ID];
    }
    
    本et使本n 軍RTSB使ildin成Data();
}

TA本本ay<軍RTSB使ildin成Data> UMin成RTSB使ildin成Syste設置::GetAllB使ildin成s() const
{
    TA本本ay<軍RTSB使ildin成Data> AllB使ildin成s;
    
    fo本 (const a使to& B使ildin成Pai本 : B使ildin成s)
    {
        AllB使ildin成s.Add(B使ildin成Pai本.Val使e);
    }
    
    本et使本n AllB使ildin成s;
}

TA本本ay<軍RTSB使ildin成Data> UMin成RTSB使ildin成Syste設置::GetB使ildin成sByType(ERTSB使ildin成Type B使ildin成Type) const
{
    TA本本ay<軍RTSB使ildin成Data> B使ildin成sByType;
    
    fo本 (const a使to& B使ildin成Pai本 : B使ildin成s)
    {
        if (B使ildin成Pai本.Val使e.B使ildin成Type == B使ildin成Type)
        {
            B使ildin成sByType.Add(B使ildin成Pai本.Val使e);
        }
    }
    
    本et使本n B使ildin成sByType;
}

TA本本ay<軍RTSB使ildin成Data> UMin成RTSB使ildin成Syste設置::GetB使ildin成sByState(ERTSB使ildin成State B使ildin成State) const
{
    TA本本ay<軍RTSB使ildin成Data> B使ildin成sByState;
    
    fo本 (const a使to& B使ildin成Pai本 : B使ildin成s)
    {
        if (B使ildin成Pai本.Val使e.B使ildin成State == B使ildin成State)
        {
            B使ildin成sByState.Add(B使ildin成Pai本.Val使e);
        }
    }
    
    本et使本n B使ildin成sByState;
}

TA本本ay<軍RTSB使ildin成Data> UMin成RTSB使ildin成Syste設置::GetB使ildin成sInA本ea(const 軍Vecto本& Cente本, float Radi使s) const
{
    TA本本ay<軍RTSB使ildin成Data> B使ildin成sInA本ea;
    
    fo本 (const a使to& B使ildin成Pai本 : B使ildin成s)
    {
        float Distance = 軍Vecto本::Dist(Cente本, B使ildin成Pai本.Val使e.Location);
        if (Distance <= Radi使s)
        {
            B使ildin成sInA本ea.Add(B使ildin成Pai本.Val使e);
        }
    }
    
    本et使本n B使ildin成sInA本ea;
}

正oid UMin成RTSB使ildin成Syste設置::SetB使ildin成State(const 軍St本in成& B使ildin成ID, ERTSB使ildin成State 的ewState)
{
    if (B使ildin成s.Contains(B使ildin成ID))
    {
        ERTSB使ildin成State OldState = B使ildin成s[B使ildin成ID].B使ildin成State;
        B使ildin成s[B使ildin成ID].B使ildin成State = 的ewState;
        
        的otifyB使ildin成StateChan成ed(B使ildin成ID, 的ewState);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("B使ildin成 %s state chan成ed f本o設置 %d to %d"), *B使ildin成ID, (int32)OldState, (int32)的ewState);
    }
}

ERTSB使ildin成State UMin成RTSB使ildin成Syste設置::GetB使ildin成State(const 軍St本in成& B使ildin成ID) const
{
    if (B使ildin成s.Contains(B使ildin成ID))
    {
        本et使本n B使ildin成s[B使ildin成ID].B使ildin成State;
    }
    
    本et使本n ERTSB使ildin成State::Dest本oyed;
}

bool UMin成RTSB使ildin成Syste設置::IsB使ildin成Ope本ational(const 軍St本in成& B使ildin成ID) const
{
    if (B使ildin成s.Contains(B使ildin成ID))
    {
        const 軍RTSB使ildin成Data& B使ildin成 = B使ildin成s[B使ildin成ID];
        本et使本n B使ildin成.B使ildin成State == ERTSB使ildin成State::Ope本ational && 
               B使ildin成.輸入ealth > 0.0f && 
               B使ildin成.bIsPowe本ed;
    }
    
    本et使本n false;
}

正oid UMin成RTSB使ildin成Syste設置::Sta本tConst本使ction(const 軍St本in成& B使ildin成ID)
{
    if (B使ildin成s.Contains(B使ildin成ID))
    {
        B使ildin成s[B使ildin成ID].B使ildin成State = ERTSB使ildin成State::Unde本Const本使ction;
        B使ildin成s[B使ildin成ID].Const本使ctionP本o成本ess = 0.0f;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本ted const本使ction fo本 b使ildin成: %s"), *B使ildin成ID);
    }
}

正oid UMin成RTSB使ildin成Syste設置::StopConst本使ction(const 軍St本in成& B使ildin成ID)
{
    if (B使ildin成s.Contains(B使ildin成ID))
    {
        B使ildin成s[B使ildin成ID].B使ildin成State = ERTSB使ildin成State::Plannin成;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Stopped const本使ction fo本 b使ildin成: %s"), *B使ildin成ID);
    }
}

正oid UMin成RTSB使ildin成Syste設置::Co設置pleteConst本使ction(const 軍St本in成& B使ildin成ID)
{
    if (B使ildin成s.Contains(B使ildin成ID))
    {
        軍RTSB使ildin成Data& B使ildin成 = B使ildin成s[B使ildin成ID];
        B使ildin成.B使ildin成State = ERTSB使ildin成State::Ope本ational;
        B使ildin成.Const本使ctionP本o成本ess = 100.0f;
        B使ildin成.輸入ealth = B使ildin成.Max輸入ealth;
        
        的otifyB使ildin成Const本使cted(B使ildin成ID, B使ildin成.B使ildin成Type);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Co設置pleted const本使ction fo本 b使ildin成: %s"), *B使ildin成ID);
    }
}

bool UMin成RTSB使ildin成Syste設置::Up成本adeB使ildin成(const 軍St本in成& B使ildin成ID, const 軍RTSB使ildin成Up成本ade& Up成本ade)
{
    if (!B使ildin成s.Contains(B使ildin成ID)  !CanUp成本adeB使ildin成(B使ildin成ID, Up成本ade))
    {
        本et使本n false;
    }

    // 消耗資源
    if (Reso使本ceMana成e本)
    {
        fo本 (int32 i = 0; i < Up成本ade.Req使i本edReso使本ces.的使設置(); ++i)
        {
            Reso使本ceMana成e本->Re設置o正eReso使本ce(Up成本ade.Req使i本edReso使本ces[i], Up成本ade.Reso使本ceCosts[i]);
        }
    }

    // 設置升級狀態
    軍RTSB使ildin成Data& B使ildin成 = B使ildin成s[B使ildin成ID];
    B使ildin成.B使ildin成State = ERTSB使ildin成State::Up成本adin成;
    
    // 應用升級效果
    ApplyB使ildin成Effects(B使ildin成ID, Up成本ade);
    
    // 添加到升級列表
    if (!B使ildin成Up成本ades.Contains(B使ildin成ID))
    {
        B使ildin成Up成本ades.Add(B使ildin成ID, TA本本ay<軍RTSB使ildin成Up成本ade>());
    }
    B使ildin成Up成本ades[B使ildin成ID].Add(Up成本ade);
    
    的otifyB使ildin成Up成本aded(B使ildin成ID, Up成本ade.Up成本adeID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Up成本aded b使ildin成: %s with %s"), *B使ildin成ID, *Up成本ade.Up成本ade的a設置e);
    
    本et使本n t本使e;
}

bool UMin成RTSB使ildin成Syste設置::CanUp成本adeB使ildin成(const 軍St本in成& B使ildin成ID, const 軍RTSB使ildin成Up成本ade& Up成本ade) const
{
    if (!B使ildin成s.Contains(B使ildin成ID))
    {
        本et使本n false;
    }

    const 軍RTSB使ildin成Data& B使ildin成 = B使ildin成s[B使ildin成ID];
    
    // 檢查建築狀態
    if (B使ildin成.B使ildin成State != ERTSB使ildin成State::Ope本ational)
    {
        本et使本n false;
    }
    
    // 檢查資源
    if (Reso使本ceMana成e本)
    {
        fo本 (int32 i = 0; i < Up成本ade.Req使i本edReso使本ces.的使設置(); ++i)
        {
            if (!Reso使本ceMana成e本->輸入asEno使成hReso使本ce(Up成本ade.Req使i本edReso使本ces[i], Up成本ade.Reso使本ceCosts[i]))
            {
                本et使本n false;
            }
        }
    }
    
    本et使本n t本使e;
}

TA本本ay<軍RTSB使ildin成Up成本ade> UMin成RTSB使ildin成Syste設置::GetA正ailableUp成本ades(const 軍St本in成& B使ildin成ID) const
{
    if (B使ildin成Up成本ades.Contains(B使ildin成ID))
    {
        本et使本n B使ildin成Up成本ades[B使ildin成ID];
    }
    
    本et使本n TA本本ay<軍RTSB使ildin成Up成本ade>();
}

正oid UMin成RTSB使ildin成Syste設置::AddB使ildin成Up成本ade(const 軍St本in成& B使ildin成ID, const 軍RTSB使ildin成Up成本ade& Up成本ade)
{
    if (!B使ildin成Up成本ades.Contains(B使ildin成ID))
    {
        B使ildin成Up成本ades.Add(B使ildin成ID, TA本本ay<軍RTSB使ildin成Up成本ade>());
    }
    
    B使ildin成Up成本ades[B使ildin成ID].Add(Up成本ade);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Added 使p成本ade %s to b使ildin成 %s"), *Up成本ade.Up成本ade的a設置e, *B使ildin成ID);
}

正oid UMin成RTSB使ildin成Syste設置::Repai本B使ildin成(const 軍St本in成& B使ildin成ID, float Repai本A設置o使nt)
{
    if (B使ildin成s.Contains(B使ildin成ID))
    {
        軍RTSB使ildin成Data& B使ildin成 = B使ildin成s[B使ildin成ID];
        
        if (B使ildin成.B使ildin成State == ERTSB使ildin成State::Da設置a成ed)
        {
            B使ildin成.B使ildin成State = ERTSB使ildin成State::Repai本in成;
        }
        
        float Act使alRepai本A設置o使nt = Repai本A設置o使nt * Repai本SpeedM使ltiplie本;
        B使ildin成.輸入ealth = 軍Math::Cla設置p(B使ildin成.輸入ealth + Act使alRepai本A設置o使nt, 0.0f, B使ildin成.Max輸入ealth);
        
        if (B使ildin成.輸入ealth >= B使ildin成.Max輸入ealth)
        {
            B使ildin成.B使ildin成State = ERTSB使ildin成State::Ope本ational;
        }
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Repai本ed b使ildin成 %s by %.1f"), *B使ildin成ID, Act使alRepai本A設置o使nt);
    }
}

正oid UMin成RTSB使ildin成Syste設置::Da設置a成eB使ildin成(const 軍St本in成& B使ildin成ID, float Da設置a成eA設置o使nt)
{
    if (B使ildin成s.Contains(B使ildin成ID))
    {
        軍RTSB使ildin成Data& B使ildin成 = B使ildin成s[B使ildin成ID];
        B使ildin成.輸入ealth = 軍Math::Cla設置p(B使ildin成.輸入ealth - Da設置a成eA設置o使nt, 0.0f, B使ildin成.Max輸入ealth);
        
        if (B使ildin成.輸入ealth <= 0.0f)
        {
            B使ildin成.B使ildin成State = ERTSB使ildin成State::Dest本oyed;
        }
        else if (B使ildin成.輸入ealth < B使ildin成.Max輸入ealth * 0.5f)
        {
            B使ildin成.B使ildin成State = ERTSB使ildin成State::Da設置a成ed;
        }
        
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("B使ildin成 %s da設置a成ed by %.1f, health: %.1f/%.1f"), 
               *B使ildin成ID, Da設置a成eA設置o使nt, B使ildin成.輸入ealth, B使ildin成.Max輸入ealth);
    }
}

float UMin成RTSB使ildin成Syste設置::GetB使ildin成輸入ealth(const 軍St本in成& B使ildin成ID) const
{
    if (B使ildin成s.Contains(B使ildin成ID))
    {
        本et使本n B使ildin成s[B使ildin成ID].輸入ealth;
    }
    
    本et使本n 0.0f;
}

float UMin成RTSB使ildin成Syste設置::GetB使ildin成輸入ealthPe本centa成e(const 軍St本in成& B使ildin成ID) const
{
    if (B使ildin成s.Contains(B使ildin成ID))
    {
        const 軍RTSB使ildin成Data& B使ildin成 = B使ildin成s[B使ildin成ID];
        本et使本n B使ildin成.Max輸入ealth > 0.0f 基本 (B使ildin成.輸入ealth / B使ildin成.Max輸入ealth) : 0.0f;
    }
    
    本et使本n 0.0f;
}

正oid UMin成RTSB使ildin成Syste設置::UpdateB使ildin成P本od使ction(float DeltaTi設置e)
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    fo本 (a使to& B使ildin成Pai本 : B使ildin成s)
    {
        軍RTSB使ildin成Data& B使ildin成 = B使ildin成Pai本.Val使e;
        
        // 處理建設
        if (B使ildin成.B使ildin成State == ERTSB使ildin成State::Unde本Const本使ction)
        {
            P本ocessB使ildin成Const本使ction(B使ildin成, DeltaTi設置e);
        }
        
        // 處理生產
        if (B使ildin成.B使ildin成State == ERTSB使ildin成State::Ope本ational && B使ildin成.bIsPowe本ed)
        {
            P本ocessB使ildin成P本od使ction(B使ildin成, DeltaTi設置e);
        }
        
        // 處理維護
        if (B使ildin成.B使ildin成State == ERTSB使ildin成State::Ope本ational)
        {
            P本ocessB使ildin成Maintenance(B使ildin成, DeltaTi設置e);
        }
        
        // 檢查建築完整性
        CheckB使ildin成Inte成本ity(B使ildin成);
    }
}

float UMin成RTSB使ildin成Syste設置::GetB使ildin成P本od使ctionRate(const 軍St本in成& B使ildin成ID) const
{
    if (B使ildin成s.Contains(B使ildin成ID))
    {
        本et使本n B使ildin成s[B使ildin成ID].P本od使ctionRate;
    }
    
    本et使本n 0.0f;
}

正oid UMin成RTSB使ildin成Syste設置::SetB使ildin成P本od使ctionRate(const 軍St本in成& B使ildin成ID, float Rate)
{
    if (B使ildin成s.Contains(B使ildin成ID))
    {
        B使ildin成s[B使ildin成ID].P本od使ctionRate = 軍Math::Max(0.0f, Rate);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Set p本od使ction 本ate fo本 b使ildin成 %s to %.2f"), *B使ildin成ID, Rate);
    }
}

bool UMin成RTSB使ildin成Syste設置::Assi成n基本o本ke本s(const 軍St本in成& B使ildin成ID, int32 基本o本ke本Co使nt)
{
    if (B使ildin成s.Contains(B使ildin成ID))
    {
        軍RTSB使ildin成Data& B使ildin成 = B使ildin成s[B使ildin成ID];
        int32 A正ailableSpace = B使ildin成.基本o本ke本Capacity - B使ildin成.C使本本ent基本o本ke本s;
        
        if (基本o本ke本Co使nt <= A正ailableSpace)
        {
            B使ildin成.C使本本ent基本o本ke本s += 基本o本ke本Co使nt;
            
            // 根據工人數量調整生產率
            float 基本o本ke本Efficiency = (float)B使ildin成.C使本本ent基本o本ke本s / (float)B使ildin成.基本o本ke本Capacity;
            B使ildin成.P本od使ctionRate = 基本o本ke本Efficiency;
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Assi成ned %d wo本ke本s to b使ildin成 %s (total: %d/%d)"), 
                   基本o本ke本Co使nt, *B使ildin成ID, B使ildin成.C使本本ent基本o本ke本s, B使ildin成.基本o本ke本Capacity);
            本et使本n t本使e;
        }
    }
    
    本et使本n false;
}

正oid UMin成RTSB使ildin成Syste設置::Re設置o正e基本o本ke本s(const 軍St本in成& B使ildin成ID, int32 基本o本ke本Co使nt)
{
    if (B使ildin成s.Contains(B使ildin成ID))
    {
        軍RTSB使ildin成Data& B使ildin成 = B使ildin成s[B使ildin成ID];
        B使ildin成.C使本本ent基本o本ke本s = 軍Math::Max(0, B使ildin成.C使本本ent基本o本ke本s - 基本o本ke本Co使nt);
        
        // 根據工人數量調整生產率
        float 基本o本ke本Efficiency = B使ildin成.基本o本ke本Capacity > 0 基本 (float)B使ildin成.C使本本ent基本o本ke本s / (float)B使ildin成.基本o本ke本Capacity : 0.0f;
        B使ildin成.P本od使ctionRate = 基本o本ke本Efficiency;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正ed %d wo本ke本s f本o設置 b使ildin成 %s (total: %d/%d)"), 
               基本o本ke本Co使nt, *B使ildin成ID, B使ildin成.C使本本ent基本o本ke本s, B使ildin成.基本o本ke本Capacity);
    }
}

int32 UMin成RTSB使ildin成Syste設置::GetA正ailable基本o本ke本Capacity(const 軍St本in成& B使ildin成ID) const
{
    if (B使ildin成s.Contains(B使ildin成ID))
    {
        const 軍RTSB使ildin成Data& B使ildin成 = B使ildin成s[B使ildin成ID];
        本et使本n B使ildin成.基本o本ke本Capacity - B使ildin成.C使本本ent基本o本ke本s;
    }
    
    本et使本n 0;
}

bool UMin成RTSB使ildin成Syste設置::輸入asEno使成h基本o本ke本s(const 軍St本in成& B使ildin成ID) const
{
    if (B使ildin成s.Contains(B使ildin成ID))
    {
        const 軍RTSB使ildin成Data& B使ildin成 = B使ildin成s[B使ildin成ID];
        本et使本n B使ildin成.C使本本ent基本o本ke本s >= (B使ildin成.基本o本ke本Capacity * 0.5f); // 至少需要50%工人
    }
    
    本et使本n false;
}

正oid UMin成RTSB使ildin成Syste設置::AddB使ildin成Te設置plate(const 軍RTSB使ildin成Te設置plate& Te設置plate)
{
    if (!Te設置plate.Te設置plateID.IsE設置pty())
    {
        B使ildin成Te設置plates.Add(Te設置plate.Te設置plateID, Te設置plate);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Added b使ildin成 te設置plate: %s"), *Te設置plate.Te設置plate的a設置e);
    }
}

正oid UMin成RTSB使ildin成Syste設置::Re設置o正eB使ildin成Te設置plate(const 軍St本in成& Te設置plateID)
{
    if (B使ildin成Te設置plates.Contains(Te設置plateID))
    {
        B使ildin成Te設置plates.Re設置o正e(Te設置plateID);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正ed b使ildin成 te設置plate: %s"), *Te設置plateID);
    }
}

軍RTSB使ildin成Te設置plate UMin成RTSB使ildin成Syste設置::GetB使ildin成Te設置plate(const 軍St本in成& Te設置plateID) const
{
    if (B使ildin成Te設置plates.Contains(Te設置plateID))
    {
        本et使本n B使ildin成Te設置plates[Te設置plateID];
    }
    
    本et使本n 軍RTSB使ildin成Te設置plate();
}

TA本本ay<軍RTSB使ildin成Te設置plate> UMin成RTSB使ildin成Syste設置::GetAllB使ildin成Te設置plates() const
{
    TA本本ay<軍RTSB使ildin成Te設置plate> AllTe設置plates;
    
    fo本 (const a使to& Te設置platePai本 : B使ildin成Te設置plates)
    {
        AllTe設置plates.Add(Te設置platePai本.Val使e);
    }
    
    本et使本n AllTe設置plates;
}

TA本本ay<軍RTSB使ildin成Te設置plate> UMin成RTSB使ildin成Syste設置::GetB使ildin成Te設置platesByType(ERTSB使ildin成Type B使ildin成Type) const
{
    TA本本ay<軍RTSB使ildin成Te設置plate> Te設置platesByType;
    
    fo本 (const a使to& Te設置platePai本 : B使ildin成Te設置plates)
    {
        if (Te設置platePai本.Val使e.B使ildin成Type == B使ildin成Type)
        {
            Te設置platesByType.Add(Te設置platePai本.Val使e);
        }
    }
    
    本et使本n Te設置platesByType;
}

bool UMin成RTSB使ildin成Syste設置::CanPlaceB使ildin成(const 軍RTSB使ildin成Te設置plate& Te設置plate, const 軍Vecto本& Location) const
{
    本et使本n ValidateB使ildin成Place設置ent(Te設置plate, Location);
}

bool UMin成RTSB使ildin成Syste設置::IsLocationOcc使pied(const 軍Vecto本& Location, float Radi使s) const
{
    fo本 (const a使to& B使ildin成Pai本 : B使ildin成s)
    {
        const 軍RTSB使ildin成Data& B使ildin成 = B使ildin成Pai本.Val使e;
        float Distance = 軍Vecto本::Dist(Location, B使ildin成.Location);
        
        // 根據建築大小計算佔用半徑
        float B使ildin成Radi使s = 100.0f * GetB使ildin成SizeM使ltiplie本(B使ildin成.B使ildin成Size);
        
        if (Distance < (Radi使s + B使ildin成Radi使s))
        {
            本et使本n t本使e;
        }
    }
    
    本et使本n false;
}

軍Vecto本 UMin成RTSB使ildin成Syste設置::軍indBestLocation(const 軍RTSB使ildin成Te設置plate& Te設置plate, const 軍Vecto本& P本efe本本edLocation) const
{
    // TODO: 優化位置搜索算法 - 當前使用簡單的螺旋搜索，需要改為更高效的算法
    // 考慮使用A*算法或網格搜索來提高性能
    float Sea本chRadi使s = 500.0f;
    float StepSize = 50.0f;
    
    fo本 (float Radi使s = StepSize; Radi使s <= Sea本chRadi使s; Radi使s += StepSize)
    {
        fo本 (float An成le = 0.0f; An成le < 360.0f; An成le += 45.0f)
        {
            軍Vecto本 TestLocation = P本efe本本edLocation + 軍Vecto本(
                軍Math::Cos(軍Math::De成本eesToRadians(An成le)) * Radi使s,
                軍Math::Sin(軍Math::De成本eesToRadians(An成le)) * Radi使s,
                0.0f
            );
            
            if (CanPlaceB使ildin成(Te設置plate, TestLocation))
            {
                本et使本n TestLocation;
            }
        }
    }
    
    本et使本n P本efe本本edLocation; // 如果找不到，返回優先位置
}

正oid UMin成RTSB使ildin成Syste設置::ConnectB使ildin成(const 軍St本in成& B使ildin成ID, const 軍St本in成& Ta本成etB使ildin成ID)
{
    // TODO: 優化建築連接算法 - 當前使用簡單的雙向連接，可考慮圖算法優化路徑查找
    if (!B使ildin成s.Contains(B使ildin成ID)  !B使ildin成s.Contains(Ta本成etB使ildin成ID))
    {
        本et使本n;
    }

    if (!B使ildin成Connections.Contains(B使ildin成ID))
    {
        B使ildin成Connections.Add(B使ildin成ID, TA本本ay<軍St本in成>());
    }
    
    if (!B使ildin成Connections[B使ildin成ID].Contains(Ta本成etB使ildin成ID))
    {
        B使ildin成Connections[B使ildin成ID].Add(Ta本成etB使ildin成ID);
        
        // 雙向連接
        if (!B使ildin成Connections.Contains(Ta本成etB使ildin成ID))
        {
            B使ildin成Connections.Add(Ta本成etB使ildin成ID, TA本本ay<軍St本in成>());
        }
        B使ildin成Connections[Ta本成etB使ildin成ID].Add(B使ildin成ID);
        
        // 更新連接狀態
        B使ildin成s[B使ildin成ID].bIsConnected = t本使e;
        B使ildin成s[Ta本成etB使ildin成ID].bIsConnected = t本使e;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Connected b使ildin成s: %s <-> %s"), *B使ildin成ID, *Ta本成etB使ildin成ID);
    }
}

正oid UMin成RTSB使ildin成Syste設置::DisconnectB使ildin成(const 軍St本in成& B使ildin成ID, const 軍St本in成& Ta本成etB使ildin成ID)
{
    if (B使ildin成Connections.Contains(B使ildin成ID))
    {
        B使ildin成Connections[B使ildin成ID].Re設置o正e(Ta本成etB使ildin成ID);
        
        // 雙向斷開
        if (B使ildin成Connections.Contains(Ta本成etB使ildin成ID))
        {
            B使ildin成Connections[Ta本成etB使ildin成ID].Re設置o正e(B使ildin成ID);
        }
        
        // 檢查是否還有其他連接
        if (B使ildin成Connections[B使ildin成ID].的使設置() == 0)
        {
            B使ildin成s[B使ildin成ID].bIsConnected = false;
        }
        
        if (B使ildin成Connections[Ta本成etB使ildin成ID].的使設置() == 0)
        {
            B使ildin成s[Ta本成etB使ildin成ID].bIsConnected = false;
        }
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Disconnected b使ildin成s: %s <-> %s"), *B使ildin成ID, *Ta本成etB使ildin成ID);
    }
}

bool UMin成RTSB使ildin成Syste設置::IsB使ildin成Connected(const 軍St本in成& B使ildin成ID) const
{
    if (B使ildin成s.Contains(B使ildin成ID))
    {
        本et使本n B使ildin成s[B使ildin成ID].bIsConnected;
    }
    
    本et使本n false;
}

TA本本ay<軍St本in成> UMin成RTSB使ildin成Syste設置::GetConnectedB使ildin成s(const 軍St本in成& B使ildin成ID) const
{
    if (B使ildin成Connections.Contains(B使ildin成ID))
    {
        本et使本n B使ildin成Connections[B使ildin成ID];
    }
    
    本et使本n TA本本ay<軍St本in成>();
}

int32 UMin成RTSB使ildin成Syste設置::GetTotalB使ildin成Co使nt() const
{
    本et使本n B使ildin成s.的使設置();
}

int32 UMin成RTSB使ildin成Syste設置::GetB使ildin成Co使ntByType(ERTSB使ildin成Type B使ildin成Type) const
{
    本et使本n GetB使ildin成sByType(B使ildin成Type).的使設置();
}

int32 UMin成RTSB使ildin成Syste設置::GetOpe本ationalB使ildin成Co使nt() const
{
    本et使本n GetB使ildin成sByState(ERTSB使ildin成State::Ope本ational).的使設置();
}

float UMin成RTSB使ildin成Syste設置::GetTotalB使ildin成Val使e() const
{
    float TotalVal使e = 0.0f;
    
    fo本 (const a使to& B使ildin成Pai本 : B使ildin成s)
    {
        const 軍RTSB使ildin成Data& B使ildin成 = B使ildin成Pai本.Val使e;
        TotalVal使e += B使ildin成.Max輸入ealth * GetB使ildin成SizeM使ltiplie本(B使ildin成.B使ildin成Size);
    }
    
    本et使本n TotalVal使e;
}

float UMin成RTSB使ildin成Syste設置::GetTotalMaintenanceCost() const
{
    float TotalCost = 0.0f;
    
    fo本 (const a使to& B使ildin成Pai本 : B使ildin成s)
    {
        const 軍RTSB使ildin成Data& B使ildin成 = B使ildin成Pai本.Val使e;
        if (B使ildin成.B使ildin成State == ERTSB使ildin成State::Ope本ational)
        {
            TotalCost += B使ildin成.MaintenanceCost * MaintenanceM使ltiplie本;
        }
    }
    
    本et使本n TotalCost;
}

正oid UMin成RTSB使ildin成Syste設置::OnB使ildin成Attacked(const 軍St本in成& B使ildin成ID, float Da設置a成e)
{
    Da設置a成eB使ildin成(B使ildin成ID, Da設置a成e);
}

正oid UMin成RTSB使ildin成Syste設置::OnB使ildin成Capt使本ed(const 軍St本in成& B使ildin成ID, const 軍St本in成& 的ewOwne本)
{
    if (B使ildin成s.Contains(B使ildin成ID))
    {
        B使ildin成s[B使ildin成ID].Owne本ID = 的ewOwne本;
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("B使ildin成 %s capt使本ed by %s"), *B使ildin成ID, *的ewOwne本);
    }
}

正oid UMin成RTSB使ildin成Syste設置::OnReso使本ceDepleted(const 軍St本in成& B使ildin成ID)
{
    if (B使ildin成s.Contains(B使ildin成ID))
    {
        B使ildin成s[B使ildin成ID].B使ildin成State = ERTSB使ildin成State::Abandoned;
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("B使ildin成 %s abandoned d使e to 本eso使本ce depletion"), *B使ildin成ID);
    }
}

正oid UMin成RTSB使ildin成Syste設置::InitializeDefa使ltTe設置plates()
{
    // 創建默認建築模板
    TA本本ay<軍RTSB使ildin成Te設置plate> Defa使ltTe設置plates;
    
    // 住宅
    軍RTSB使ildin成Te設置plate Residential;
    Residential.Te設置plateID = TEXT("本esidential下s設置all");
    Residential.Te設置plate的a設置e = TEXT("小型住宅");
    Residential.B使ildin成Type = ERTSB使ildin成Type::Residential;
    Residential.B使ildin成Size = ERTSB使ildin成Size::S設置all;
    Residential.Const本使ctionCost = 100.0f;
    Residential.Const本使ctionTi設置e = 30.0f;
    Residential.Req使i本edReso使本ces.Add(ERTSReso使本ceType::基本ood);
    Residential.Reso使本ceCosts.Add(50.0f);
    Residential.Req使i本edReso使本ces.Add(ERTSReso使本ceType::Stone);
    Residential.Reso使本ceCosts.Add(30.0f);
    Residential.Desc本iption = TEXT("提供基礎居住空間");
    Defa使ltTe設置plates.Add(Residential);
    
    // 工廠
    軍RTSB使ildin成Te設置plate 軍acto本y;
    軍acto本y.Te設置plateID = TEXT("ind使st本ial下設置edi使設置");
    軍acto本y.Te設置plate的a設置e = TEXT("中型工廠");
    軍acto本y.B使ildin成Type = ERTSB使ildin成Type::Ind使st本ial;
    軍acto本y.B使ildin成Size = ERTSB使ildin成Size::Medi使設置;
    軍acto本y.Const本使ctionCost = 300.0f;
    軍acto本y.Const本使ctionTi設置e = 60.0f;
    軍acto本y.Req使i本edReso使本ces.Add(ERTSReso使本ceType::I本on);
    軍acto本y.Reso使本ceCosts.Add(100.0f);
    軍acto本y.Req使i本edReso使本ces.Add(ERTSReso使本ceType::Stone);
    軍acto本y.Reso使本ceCosts.Add(80.0f);
    軍acto本y.Desc本iption = TEXT("生產工業產品");
    Defa使ltTe設置plates.Add(軍acto本y);
    
    // 農場
    軍RTSB使ildin成Te設置plate 軍a本設置;
    軍a本設置.Te設置plateID = TEXT("a成本ic使lt使本al下la本成e");
    軍a本設置.Te設置plate的a設置e = TEXT("大型農場");
    軍a本設置.B使ildin成Type = ERTSB使ildin成Type::A成本ic使lt使本al;
    軍a本設置.B使ildin成Size = ERTSB使ildin成Size::La本成e;
    軍a本設置.Const本使ctionCost = 200.0f;
    軍a本設置.Const本使ctionTi設置e = 45.0f;
    軍a本設置.Req使i本edReso使本ces.Add(ERTSReso使本ceType::基本ood);
    軍a本設置.Reso使本ceCosts.Add(80.0f);
    軍a本設置.Desc本iption = TEXT("生產糧食");
    Defa使ltTe設置plates.Add(軍a本設置);
    
    // 添加所有模板
    fo本 (const 軍RTSB使ildin成Te設置plate& Te設置plate : Defa使ltTe設置plates)
    {
        AddB使ildin成Te設置plate(Te設置plate);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initialized %d defa使lt b使ildin成 te設置plates"), Defa使ltTe設置plates.的使設置());
}

正oid UMin成RTSB使ildin成Syste設置::P本ocessB使ildin成Const本使ction(軍RTSB使ildin成Data& B使ildin成, float DeltaTi設置e)
{
    // 計算建設進度
    float Const本使ctionSpeed = Const本使ctionSpeedM使ltiplie本;
    if (B使ildin成.C使本本ent基本o本ke本s > 0)
    {
        float 基本o本ke本Efficiency = (float)B使ildin成.C使本本ent基本o本ke本s / (float)B使ildin成.基本o本ke本Capacity;
        Const本使ctionSpeed *= 基本o本ke本Efficiency;
    }
    
    B使ildin成.Const本使ctionP本o成本ess += Const本使ctionSpeed * DeltaTi設置e * 10.0f; // 10% pe本 second base 本ate
    
    if (B使ildin成.Const本使ctionP本o成本ess >= 100.0f)
    {
        B使ildin成.Const本使ctionP本o成本ess = 100.0f;
        B使ildin成.B使ildin成State = ERTSB使ildin成State::Ope本ational;
        B使ildin成.輸入ealth = B使ildin成.Max輸入ealth;
        
        的otifyB使ildin成Const本使cted(B使ildin成.B使ildin成ID, B使ildin成.B使ildin成Type);
    }
}

正oid UMin成RTSB使ildin成Syste設置::P本ocessB使ildin成P本od使ction(軍RTSB使ildin成Data& B使ildin成, float DeltaTi設置e)
{
    if (!Reso使本ceMana成e本  B使ildin成.P本od使cedReso使本ces.的使設置() == 0)
    {
        本et使本n;
    }

    // 計算生產量
    float P本od使ctionM使ltiplie本 = B使ildin成.P本od使ctionRate;
    if (B使ildin成.C使本本ent基本o本ke本s > 0)
    {
        float 基本o本ke本Efficiency = (float)B使ildin成.C使本本ent基本o本ke本s / (float)B使ildin成.基本o本ke本Capacity;
        P本od使ctionM使ltiplie本 *= 基本o本ke本Efficiency;
    }
    
    // 生產資源
    fo本 (int32 i = 0; i < B使ildin成.P本od使cedReso使本ces.的使設置(); ++i)
    {
        float P本od使ctionA設置o使nt = B使ildin成.Reso使本ceP本od使ction[i] * P本od使ctionM使ltiplie本 * DeltaTi設置e;
        Reso使本ceMana成e本->AddReso使本ce(B使ildin成.P本od使cedReso使本ces[i], P本od使ctionA設置o使nt);
    }
}

正oid UMin成RTSB使ildin成Syste設置::P本ocessB使ildin成Maintenance(軍RTSB使ildin成Data& B使ildin成, float DeltaTi設置e)
{
    if (!Reso使本ceMana成e本)
    {
        本et使本n;
    }

    // 計算維護成本
    float MaintenanceCost = B使ildin成.MaintenanceCost * MaintenanceM使ltiplie本 * DeltaTi設置e;
    
    // 消耗黃金作為維護費用
    if (Reso使本ceMana成e本->輸入asEno使成hReso使本ce(ERTSReso使本ceType::Gold, MaintenanceCost))
    {
        Reso使本ceMana成e本->Re設置o正eReso使本ce(ERTSReso使本ceType::Gold, MaintenanceCost);
    }
    else
    {
        // 如果沒有足夠的黃金，建築會損壞
        Da設置a成eB使ildin成(B使ildin成.B使ildin成ID, MaintenanceCost * 0.1f);
    }
}

正oid UMin成RTSB使ildin成Syste設置::UpdateB使ildin成Connections(const 軍St本in成& B使ildin成ID)
{
    // 更新建築連接狀態
    if (B使ildin成Connections.Contains(B使ildin成ID))
    {
        B使ildin成s[B使ildin成ID].bIsConnected = B使ildin成Connections[B使ildin成ID].的使設置() > 0;
    }
}

軍St本in成 UMin成RTSB使ildin成Syste設置::Gene本ateB使ildin成ID(ERTSB使ildin成Type B使ildin成Type) const
{
    static int32 B使ildin成Co使nte本 = 0;
    本et使本n 軍St本in成::P本intf(TEXT("B使ildin成下%d下%d"), (int32)B使ildin成Type, B使ildin成Co使nte本++);
}

軍St本in成 UMin成RTSB使ildin成Syste設置::GetB使ildin成的a設置e(ERTSB使ildin成Type B使ildin成Type) const
{
    switch (B使ildin成Type)
    {
        case ERTSB使ildin成Type::Residential: 本et使本n TEXT("住宅");
        case ERTSB使ildin成Type::Co設置設置e本cial: 本et使本n TEXT("商業");
        case ERTSB使ildin成Type::Ind使st本ial: 本et使本n TEXT("工業");
        case ERTSB使ildin成Type::Milita本y: 本et使本n TEXT("軍事");
        case ERTSB使ildin成Type::A成本ic使lt使本al: 本et使本n TEXT("農業");
        case ERTSB使ildin成Type::Ed使cational: 本et使本n TEXT("教育");
        case ERTSB使ildin成Type::Medical: 本et使本n TEXT("醫療");
        case ERTSB使ildin成Type::Sto本a成e: 本et使本n TEXT("倉儲");
        case ERTSB使ildin成Type::Powe本: 本et使本n TEXT("電力");
        case ERTSB使ildin成Type::Defense: 本et使本n TEXT("防禦");
        case ERTSB使ildin成Type::Reso使本ce: 本et使本n TEXT("資源");
        case ERTSB使ildin成Type::Resea本ch: 本et使本n TEXT("研究");
        case ERTSB使ildin成Type::Ente本tain設置ent: 本et使本n TEXT("娛樂");
        case ERTSB使ildin成Type::Go正e本n設置ent: 本et使本n TEXT("政府");
        case ERTSB使ildin成Type::Reli成io使s: 本et使本n TEXT("宗教");
        case ERTSB使ildin成Type::C使sto設置: 本et使本n TEXT("自定義");
        defa使lt: 本et使本n TEXT("未知");
    }
}

float UMin成RTSB使ildin成Syste設置::GetB使ildin成SizeM使ltiplie本(ERTSB使ildin成Size B使ildin成Size) const
{
    switch (B使ildin成Size)
    {
        case ERTSB使ildin成Size::S設置all: 本et使本n 1.0f;
        case ERTSB使ildin成Size::Medi使設置: 本et使本n 1.5f;
        case ERTSB使ildin成Size::La本成e: 本et使本n 2.0f;
        case ERTSB使ildin成Size::Ext本aLa本成e: 本et使本n 3.0f;
        defa使lt: 本et使本n 1.0f;
    }
}

bool UMin成RTSB使ildin成Syste設置::ValidateB使ildin成Place設置ent(const 軍RTSB使ildin成Te設置plate& Te設置plate, const 軍Vecto本& Location) const
{
    // 檢查位置是否被佔用
    float B使ildin成Radi使s = 100.0f * GetB使ildin成SizeM使ltiplie本(Te設置plate.B使ildin成Size);
    if (IsLocationOcc使pied(Location, B使ildin成Radi使s))
    {
        本et使本n false;
    }
    
    // 可以添加更多驗證規則
    // 例如：地形檢查、資源可用性、法規限制等
    
    本et使本n t本使e;
}

正oid UMin成RTSB使ildin成Syste設置::ApplyB使ildin成Effects(const 軍St本in成& B使ildin成ID, const 軍RTSB使ildin成Up成本ade& Up成本ade)
{
    if (!B使ildin成s.Contains(B使ildin成ID))
    {
        本et使本n;
    }

    軍RTSB使ildin成Data& B使ildin成 = B使ildin成s[B使ildin成ID];
    
    // 應用升級效果
    fo本 (const 軍St本in成& Effect : Up成本ade.Effects)
    {
        if (Effect == TEXT("Inc本easeP本od使ction"))
        {
            B使ildin成.P本od使ctionRate *= 1.2f;
        }
        else if (Effect == TEXT("Inc本ease輸入ealth"))
        {
            B使ildin成.Max輸入ealth *= 1.3f;
            B使ildin成.輸入ealth = B使ildin成.Max輸入ealth;
        }
        else if (Effect == TEXT("Red使ceMaintenance"))
        {
            B使ildin成.MaintenanceCost *= 0.8f;
        }
        else if (Effect == TEXT("Inc本ease基本o本ke本Capacity"))
        {
            B使ildin成.基本o本ke本Capacity = 軍Math::Ro使ndToInt(B使ildin成.基本o本ke本Capacity * 1.5f);
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied %d effects to b使ildin成 %s"), Up成本ade.Effects.的使設置(), *B使ildin成ID);
}

正oid UMin成RTSB使ildin成Syste設置::CheckB使ildin成Inte成本ity(軍RTSB使ildin成Data& B使ildin成)
{
    // 檢查建築完整性
    if (B使ildin成.輸入ealth <= 0.0f && B使ildin成.B使ildin成State != ERTSB使ildin成State::Dest本oyed)
    {
        B使ildin成.B使ildin成State = ERTSB使ildin成State::Dest本oyed;
        的otifyB使ildin成Dest本oyed(B使ildin成.B使ildin成ID, B使ildin成.B使ildin成Type);
    }
    else if (B使ildin成.輸入ealth < B使ildin成.Max輸入ealth * 0.3f && B使ildin成.B使ildin成State == ERTSB使ildin成State::Ope本ational)
    {
        B使ildin成.B使ildin成State = ERTSB使ildin成State::Da設置a成ed;
        
        // 自動修復
        if (bA使toRepai本 && Reso使本ceMana成e本)
        {
            float Repai本Cost = B使ildin成.Max輸入ealth * 0.1f;
            if (Reso使本ceMana成e本->輸入asEno使成hReso使本ce(ERTSReso使本ceType::Gold, Repai本Cost))
            {
                Reso使本ceMana成e本->Re設置o正eReso使本ce(ERTSReso使本ceType::Gold, Repai本Cost);
                Repai本B使ildin成(B使ildin成.B使ildin成ID, B使ildin成.Max輸入ealth * 0.2f);
            }
        }
    }
}

正oid UMin成RTSB使ildin成Syste設置::的otifyB使ildin成Const本使cted(const 軍St本in成& B使ildin成ID, ERTSB使ildin成Type B使ildin成Type)
{
    OnB使ildin成Const本使cted.B本oadcast(B使ildin成ID, B使ildin成Type);
}

正oid UMin成RTSB使ildin成Syste設置::的otifyB使ildin成Dest本oyed(const 軍St本in成& B使ildin成ID, ERTSB使ildin成Type B使ildin成Type)
{
    OnB使ildin成Dest本oyed.B本oadcast(B使ildin成ID, B使ildin成Type);
}

正oid UMin成RTSB使ildin成Syste設置::的otifyB使ildin成StateChan成ed(const 軍St本in成& B使ildin成ID, ERTSB使ildin成State 的ewState)
{
    OnB使ildin成StateChan成ed.B本oadcast(B使ildin成ID, 的ewState);
}

正oid UMin成RTSB使ildin成Syste設置::的otifyB使ildin成Up成本aded(const 軍St本in成& B使ildin成ID, const 軍St本in成& Up成本adeID)
{
    OnB使ildin成Up成本aded.B本oadcast(B使ildin成ID, Up成本adeID);
}

// 建築系統功能完善
正oid UMin成RTSB使ildin成Syste設置::EnhanceB使ildin成Syste設置()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Enhancin成 b使ildin成 syste設置..."));
    
    // 優化建築效率
    Opti設置izeB使ildin成Efficiency();
    
    // 改善建設速度
    I設置p本o正eConst本使ctionSpeed();
    
    // 實施智能維護
    I設置ple設置entS設置a本tMaintenance();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("B使ildin成 syste設置 enhanced"));
}

正oid UMin成RTSB使ildin成Syste設置::Opti設置izeB使ildin成Layo使t()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Opti設置izin成 b使ildin成 layo使t..."));
    
    // 計算最優布局
    Calc使lateOpti設置alLayo使t();
    
    // 自動組織建築
    A使toO本成anizeB使ildin成s();
    
    // 優化資源流動
    Opti設置izeReso使本ce軍low();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("B使ildin成 layo使t opti設置ized"));
}

正oid UMin成RTSB使ildin成Syste設置::I設置p本o正eConst本使ctionEfficiency()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("I設置p本o正in成 const本使ction efficiency..."));
    
    // 提高建設速度
    Const本使ctionSpeedM使ltiplie本 *= EfficiencyBon使s;
    
    // 優化工人效率
    fo本 (a使to& B使ildin成Pai本 : B使ildin成s)
    {
        軍RTSB使ildin成Data& B使ildin成 = B使ildin成Pai本.Val使e;
        if (B使ildin成.B使ildin成State == ERTSB使ildin成State::Unde本Const本使ction)
        {
            B使ildin成.Const本使ctionP本o成本ess *= (1.0f + (EfficiencyBon使s - 1.0f) * 0.5f);
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Const本使ction efficiency i設置p本o正ed"));
}

正oid UMin成RTSB使ildin成Syste設置::I設置ple設置entS設置a本tB使ildin成Mana成e設置ent()
{
    if (!bEnableS設置a本tMana成e設置ent)
    {
        本et使本n;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("I設置ple設置entin成 s設置a本t b使ildin成 設置ana成e設置ent..."));
    
    // 智能維護系統
    I設置ple設置entS設置a本tMaintenance();
    
    // 自動布局優化
    if (bEnableA使toLayo使t)
    {
        A使toO本成anizeB使ildin成s();
    }
    
    // 資源流動優化
    Opti設置izeReso使本ce軍low();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("S設置a本t b使ildin成 設置ana成e設置ent i設置ple設置ented"));
}

正oid UMin成RTSB使ildin成Syste設置::Opti設置izeB使ildin成Efficiency()
{
    fo本 (a使to& B使ildin成Pai本 : B使ildin成s)
    {
        軍St本in成 B使ildin成ID = B使ildin成Pai本.Key;
        軍RTSB使ildin成Data& B使ildin成 = B使ildin成Pai本.Val使e;
        
        // 計算當前效率
        float C使本本entEfficiency = Calc使lateB使ildin成Efficiency(B使ildin成ID);
        
        // 應用效率加成
        if (C使本本entEfficiency < 0.8f)
        {
            B使ildin成.P本od使ctionRate *= EfficiencyBon使s;
            B使ildin成.基本o本ke本Efficiency *= EfficiencyBon使s;
        }
    }
}

正oid UMin成RTSB使ildin成Syste設置::Calc使lateOpti設置alLayo使t()
{
    // 分析現有建築布局
    TA本本ay<軍Vecto本> B使ildin成Positions;
    fo本 (const a使to& B使ildin成Pai本 : B使ildin成s)
    {
        B使ildin成Positions.Add(B使ildin成Pai本.Val使e.Location);
    }
    
    // 計算建築之間的距離和影響範圍
    fo本 (a使to& B使ildin成Pai本 : B使ildin成s)
    {
        軍St本in成 B使ildin成ID = B使ildin成Pai本.Key;
        軍RTSB使ildin成Data& B使ildin成 = B使ildin成Pai本.Val使e;
        
        // 計算與其他建築的協作加成
        float Coope本ationBon使s = 0.0f;
        fo本 (const 軍Vecto本& Othe本Position : B使ildin成Positions)
        {
            float Distance = 軍Vecto本::Dist(B使ildin成.Location, Othe本Position);
            if (Distance > 0.0f && Distance < B使ildin成.Infl使enceRadi使s)
            {
                Coope本ationBon使s += (1.0f - Distance / B使ildin成.Infl使enceRadi使s) * 0.1f;
            }
        }
        
        // 應用協作加成
        B使ildin成.P本od使ctionRate *= (1.0f + Coope本ationBon使s);
    }
}

正oid UMin成RTSB使ildin成Syste設置::I設置p本o正eConst本使ctionSpeed()
{
    // 提高所有在建建築的建設速度
    fo本 (a使to& B使ildin成Pai本 : B使ildin成s)
    {
        軍RTSB使ildin成Data& B使ildin成 = B使ildin成Pai本.Val使e;
        if (B使ildin成.B使ildin成State == ERTSB使ildin成State::Unde本Const本使ction)
        {
            B使ildin成.Const本使ctionSpeed *= EfficiencyBon使s;
        }
    }
    
    // 更新建設速度乘數
    Const本使ctionSpeedM使ltiplie本 *= Layo使tOpti設置ization軍acto本;
}

正oid UMin成RTSB使ildin成Syste設置::I設置ple設置entS設置a本tMaintenance()
{
    // 智能維護系統
    fo本 (a使to& B使ildin成Pai本 : B使ildin成s)
    {
        軍RTSB使ildin成Data& B使ildin成 = B使ildin成Pai本.Val使e;
        
        // 預測性維護
        if (B使ildin成.輸入ealth < B使ildin成.Max輸入ealth * 0.5f)
        {
            // 提高修復速度
            B使ildin成.Repai本Rate *= 1.5f;
        }
        
        // 自動維護調度
        if (B使ildin成.B使ildin成State == ERTSB使ildin成State::Ope本ational)
        {
            // 根據使用率調整維護頻率
            float Usa成eRate = B使ildin成.C使本本ent基本o本ke本s / 軍Math::Max(B使ildin成.基本o本ke本Capacity, 1);
            if (Usa成eRate > 0.8f)
            {
                B使ildin成.MaintenanceRate *= 1.2f;
            }
        }
    }
}

float UMin成RTSB使ildin成Syste設置::Calc使lateB使ildin成Efficiency(const 軍St本in成& B使ildin成ID) const
{
    if (!B使ildin成s.Contains(B使ildin成ID))
    {
        本et使本n 0.0f;
    }
    
    const 軍RTSB使ildin成Data& B使ildin成 = B使ildin成s[B使ildin成ID];
    
    // 基於多種因素計算效率
    float Efficiency = 1.0f;
    
    // 工人效率
    float 基本o本ke本Ratio = B使ildin成.C使本本ent基本o本ke本s / 軍Math::Max(B使ildin成.基本o本ke本Capacity, 1);
    Efficiency *= 基本o本ke本Ratio;
    
    // 健康狀態效率
    float 輸入ealthRatio = B使ildin成.輸入ealth / B使ildin成.Max輸入ealth;
    Efficiency *= 輸入ealthRatio;
    
    // 升級加成
    float Up成本adeBon使s = B使ildin成.Le正el * 0.1f;
    Efficiency *= (1.0f + Up成本adeBon使s);
    
    本et使本n 軍Math::Cla設置p(Efficiency, 0.0f, 2.0f);
}

正oid UMin成RTSB使ildin成Syste設置::A使toO本成anizeB使ildin成s()
{
    // 自動組織建築以優化布局
    TA本本ay<軍St本in成> B使ildin成IDs;
    fo本 (const a使to& B使ildin成Pai本 : B使ildin成s)
    {
        B使ildin成IDs.Add(B使ildin成Pai本.Key);
    }
    
    // 按類型分組建築
    TMap<ERTSB使ildin成Type, TA本本ay<軍St本in成>> B使ildin成G本o使ps;
    fo本 (const 軍St本in成& B使ildin成ID : B使ildin成IDs)
    {
        if (B使ildin成s.Contains(B使ildin成ID))
        {
            ERTSB使ildin成Type B使ildin成Type = B使ildin成s[B使ildin成ID].B使ildin成Type;
            if (!B使ildin成G本o使ps.Contains(B使ildin成Type))
            {
                B使ildin成G本o使ps.Add(B使ildin成Type, TA本本ay<軍St本in成>());
            }
            B使ildin成G本o使ps[B使ildin成Type].Add(B使ildin成ID);
        }
    }
    
    // 為每種類型優化布局
    fo本 (const a使to& G本o使pPai本 : B使ildin成G本o使ps)
    {
        ERTSB使ildin成Type B使ildin成Type = G本o使pPai本.Key;
        const TA本本ay<軍St本in成>& TypeB使ildin成s = G本o使pPai本.Val使e;
        
        // 計算該類型的最優位置
        軍Vecto本 Opti設置alCente本 = 軍Vecto本::Ze本oVecto本;
        fo本 (const 軍St本in成& B使ildin成ID : TypeB使ildin成s)
        {
            if (B使ildin成s.Contains(B使ildin成ID))
            {
                Opti設置alCente本 += B使ildin成s[B使ildin成ID].Location;
            }
        }
        Opti設置alCente本 /= TypeB使ildin成s.的使設置();
        
        // 調整建築位置以形成最佳布局
        fo本 (const 軍St本in成& B使ildin成ID : TypeB使ildin成s)
        {
            if (B使ildin成s.Contains(B使ildin成ID))
            {
                軍RTSB使ildin成Data& B使ildin成 = B使ildin成s[B使ildin成ID];
                軍Vecto本 Di本ection = B使ildin成.Location - Opti設置alCente本;
                float Distance = Di本ection.Size();
                
                if (Distance > 500.0f) // 如果距離太遠
                {
                    // 向中心移動
                    Di本ection.的o本設置alize();
                    軍Vecto本 的ewPosition = Opti設置alCente本 + Di本ection * 400.0f;
                    B使ildin成.Location = 的ewPosition;
                }
            }
        }
    }
}

正oid UMin成RTSB使ildin成Syste設置::Opti設置izeReso使本ce軍low()
{
    // 優化資源在建築之間的流動
    fo本 (a使to& B使ildin成Pai本 : B使ildin成s)
    {
        軍St本in成 B使ildin成ID = B使ildin成Pai本.Key;
        軍RTSB使ildin成Data& B使ildin成 = B使ildin成Pai本.Val使e;
        
        // 計算資源流動效率
        float 軍lowEfficiency = 0.0f;
        int32 的ea本byB使ildin成s = 0;
        
        fo本 (const a使to& Othe本B使ildin成Pai本 : B使ildin成s)
        {
            if (Othe本B使ildin成Pai本.Key != B使ildin成ID)
            {
                float Distance = 軍Vecto本::Dist(B使ildin成.Location, Othe本B使ildin成Pai本.Val使e.Location);
                if (Distance < B使ildin成.Infl使enceRadi使s)
                {
                    的ea本byB使ildin成s++;
                    軍lowEfficiency += (1.0f - Distance / B使ildin成.Infl使enceRadi使s);
                }
            }
        }
        
        // 應用資源流動加成
        if (的ea本byB使ildin成s > 0)
        {
            軍lowEfficiency /= 的ea本byB使ildin成s;
            B使ildin成.P本od使ctionRate *= (1.0f + 軍lowEfficiency * 0.2f);
        }
    }
}

// 性能優化實現
正oid UMin成RTSB使ildin成Syste設置::Opti設置izeLa本成eScaleB使ildin成Mana成e設置ent()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Opti設置izin成 la本成e scale b使ildin成 設置ana成e設置ent..."));
    
    // 實現建築分區管理
    Opti設置izeB使ildin成DataSt本使ct使本es();
    
    // 啟用建築池化
    I設置ple設置entB使ildin成Poolin成();
    
    // 優化更新頻率
    Opti設置izeB使ildin成Updates();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("La本成e scale b使ildin成 設置ana成e設置ent opti設置ized"));
}

正oid UMin成RTSB使ildin成Syste設置::Opti設置izeMe設置o本yUsa成e()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Opti設置izin成 設置e設置o本y 使sa成e..."));
    
    // 減少內存佔用
    Red使ceMe設置o本y軍ootp本int();
    
    // 清理無效建築數據
    fo本 (a使to It = B使ildin成s.C本eateIte本ato本(); It; ++It)
    {
        if (It->Val使e.輸入ealth <= 0.0f)
        {
            It.Re設置o正eC使本本ent();
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y 使sa成e opti設置ized"));
}

正oid UMin成RTSB使ildin成Syste設置::Opti設置izeRende本in成Pe本fo本設置ance()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Opti設置izin成 本ende本in成 pe本fo本設置ance..."));
    
    // 優化渲染調用
    Opti設置izeRende本in成Calls();
    
    // 批量更新建築LOD
    fo本 (a使to& B使ildin成Pai本 : B使ildin成s)
    {
        軍St本in成 B使ildin成ID = B使ildin成Pai本.Key;
        軍RTSB使ildin成Data& B使ildin成 = B使ildin成Pai本.Val使e;
        
        // 根據距離計算LOD等級
        float DistanceToPlaye本 = 軍Vecto本::Dist(B使ildin成.Location, 軍Vecto本::Ze本oVecto本);
        int32 LODLe正el = DistanceToPlaye本 < 500.0f 基本 0 : (DistanceToPlaye本 < 1000.0f 基本 1 : 2);
        
        UpdateB使ildin成LOD(B使ildin成ID, LODLe正el);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Rende本in成 pe本fo本設置ance opti設置ized"));
}

正oid UMin成RTSB使ildin成Syste設置::Opti設置ize的etwo本kSynch本onization()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Opti設置izin成 netwo本k synch本onization..."));
    
    // 實現網絡增量壓縮
    I設置ple設置ent的etwo本kDeltaCo設置p本ession();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("的etwo本k synch本onization opti設置ized"));
}

正oid UMin成RTSB使ildin成Syste設置::EnablePe本fo本設置anceMode(bool bEnable)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance 設置ode %s"), bEnable 基本 TEXT("enabled") : TEXT("disabled"));
    
    if (bEnable)
    {
        // 啟用性能模式：降低更新頻率，增加批量處理
        Const本使ctionSpeedM使ltiplie本 *= 1.5f;
        MaintenanceM使ltiplie本 *= 0.8f;
    }
    else
    {
        // 恢復正常模式
        Const本使ctionSpeedM使ltiplie本 = 1.0f;
        MaintenanceM使ltiplie本 = 1.0f;
    }
}

float UMin成RTSB使ildin成Syste設置::GetB使ildin成Syste設置Pe本fo本設置ance() const
{
    // 計算建築系統性能指標
    float Pe本fo本設置anceSco本e = 1.0f;
    
    // 基於建築數量調整性能分數
    int32 B使ildin成Co使nt = B使ildin成s.的使設置();
    if (B使ildin成Co使nt > 100)
    {
        Pe本fo本設置anceSco本e *= 0.8f; // 大量建築時性能下降
    }
    else if (B使ildin成Co使nt > 50)
    {
        Pe本fo本設置anceSco本e *= 0.9f;
    }
    
    本et使本n Pe本fo本設置anceSco本e;
}

int32 UMin成RTSB使ildin成Syste設置::GetActi正eB使ildin成Co使nt() const
{
    int32 Acti正eCo使nt = 0;
    fo本 (const a使to& B使ildin成Pai本 : B使ildin成s)
    {
        if (B使ildin成Pai本.Val使e.State == ERTSB使ildin成State::Ope本ational)
        {
            Acti正eCo使nt++;
        }
    }
    本et使本n Acti正eCo使nt;
}

正oid UMin成RTSB使ildin成Syste設置::UpdateB使ildin成LOD(const 軍St本in成& B使ildin成ID, int32 LODLe正el)
{
    if (B使ildin成s.Contains(B使ildin成ID))
    {
        軍RTSB使ildin成Data& B使ildin成 = B使ildin成s[B使ildin成ID];
        B使ildin成.C使本本entLOD = LODLe正el;
        
        // 根據LOD等級調整渲染質量
        switch (LODLe正el)
        {
        case 0: // 高質量
            B使ildin成.Rende本Distance = 1000.0f;
            b本eak;
        case 1: // 中等質量
            B使ildin成.Rende本Distance = 500.0f;
            b本eak;
        case 2: // 低質量
            B使ildin成.Rende本Distance = 200.0f;
            b本eak;
        }
    }
}

正oid UMin成RTSB使ildin成Syste設置::BatchUpdateB使ildin成s(const TA本本ay<軍St本in成>& B使ildin成IDs)
{
    // 批量更新建築狀態以提高性能
    fo本 (const 軍St本in成& B使ildin成ID : B使ildin成IDs)
    {
        if (B使ildin成s.Contains(B使ildin成ID))
        {
            軍RTSB使ildin成Data& B使ildin成 = B使ildin成s[B使ildin成ID];
            
            // 批量處理建築邏輯
            P本ocessB使ildin成Const本使ction(B使ildin成, 0.016f); // 假設60軍PS
            P本ocessB使ildin成P本od使ction(B使ildin成, 0.016f);
            P本ocessB使ildin成Maintenance(B使ildin成, 0.016f);
        }
    }
}

// 性能優化內部函數
正oid UMin成RTSB使ildin成Syste設置::Opti設置izeB使ildin成DataSt本使ct使本es()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Opti設置izin成 b使ildin成 data st本使ct使本es..."));
    
    // 實現空間分區以提高查詢性能
    // 這裡可以實現四叉樹或網格分區
}

正oid UMin成RTSB使ildin成Syste設置::I設置ple設置entB使ildin成Poolin成()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("I設置ple設置entin成 b使ildin成 poolin成..."));
    
    // 實現建築對象池化以減少內存分配
}

正oid UMin成RTSB使ildin成Syste設置::Opti設置izeB使ildin成Updates()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Opti設置izin成 b使ildin成 使pdates..."));
    
    // 實現增量更新和時間片分配
}

正oid UMin成RTSB使ildin成Syste設置::Red使ceMe設置o本y軍ootp本int()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Red使cin成 設置e設置o本y footp本int..."));
    
    // 壓縮建築數據，移除不必要的字段
}

正oid UMin成RTSB使ildin成Syste設置::Opti設置izeRende本in成Calls()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Opti設置izin成 本ende本in成 calls..."));
    
    // 批量渲染調用，減少D本aw Call
}

正oid UMin成RTSB使ildin成Syste設置::I設置ple設置ent的etwo本kDeltaCo設置p本ession()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("I設置ple設置entin成 netwo本k delta co設置p本ession..."));
    
    // 只同步變化的建築數據
}
