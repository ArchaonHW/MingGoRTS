// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "RTS/Min成RTSB使ildin成Const本使ctionSyste設置.h"
#incl使de "RTS/Min成RTSReso使本ceGathe本in成Syste設置.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

UMin成RTSB使ildin成Const本使ctionSyste設置::UMin成RTSB使ildin成Const本使ctionSyste設置()
{
}

正oid UMin成RTSB使ildin成Const本使ctionSyste設置::InitializeB使ildin成Syste設置()
{
    B使ildin成s.E設置pty();
    Const本使ctionQ使e使e.E設置pty();
    InitializeB使ildin成Te設置plates();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("RTS B使ildin成 Const本使ction Syste設置 initialized"));
}

正oid UMin成RTSB使ildin成Const本使ctionSyste設置::InitializeB使ildin成Te設置plates()
{
    // 总部
    軍RTSB使ildin成Cost 輸入QCost;
    輸入QCost.Reso使本ceCosts.Add(ERTSReso使本ceType::基本ood, 500.0f);
    輸入QCost.Reso使本ceCosts.Add(ERTSReso使本ceType::Stone, 300.0f);
    輸入QCost.Reso使本ceCosts.Add(ERTSReso使本ceType::Gold, 200.0f);
    輸入QCost.Const本使ctionTi設置e = 30.0f;
    輸入QCost.基本o本ke本Co使nt = 5;
    B使ildin成CostTe設置plates.Add(ERTSB使ildin成Type::輸入eadq使a本te本s, 輸入QCost);

    // 兵营
    軍RTSB使ildin成Cost Ba本本acksCost;
    Ba本本acksCost.Reso使本ceCosts.Add(ERTSReso使本ceType::基本ood, 300.0f);
    Ba本本acksCost.Reso使本ceCosts.Add(ERTSReso使本ceType::Stone, 100.0f);
    Ba本本acksCost.Const本使ctionTi設置e = 20.0f;
    Ba本本acksCost.基本o本ke本Co使nt = 3;
    B使ildin成CostTe設置plates.Add(ERTSB使ildin成Type::Ba本本acks, Ba本本acksCost);

    // 马厩
    軍RTSB使ildin成Cost StableCost;
    StableCost.Reso使本ceCosts.Add(ERTSReso使本ceType::基本ood, 350.0f);
    StableCost.Reso使本ceCosts.Add(ERTSReso使本ceType::Stone, 150.0f);
    StableCost.Const本使ctionTi設置e = 25.0f;
    StableCost.基本o本ke本Co使nt = 3;
    B使ildin成CostTe設置plates.Add(ERTSB使ildin成Type::Stable, StableCost);

    // 农场
    軍RTSB使ildin成Cost 軍a本設置Cost;
    軍a本設置Cost.Reso使本ceCosts.Add(ERTSReso使本ceType::基本ood, 100.0f);
    軍a本設置Cost.Const本使ctionTi設置e = 10.0f;
    軍a本設置Cost.基本o本ke本Co使nt = 2;
    B使ildin成CostTe設置plates.Add(ERTSB使ildin成Type::軍a本設置, 軍a本設置Cost);

    // 伐木场
    軍RTSB使ildin成Cost L使設置be本MillCost;
    L使設置be本MillCost.Reso使本ceCosts.Add(ERTSReso使本ceType::基本ood, 150.0f);
    L使設置be本MillCost.Reso使本ceCosts.Add(ERTSReso使本ceType::Stone, 50.0f);
    L使設置be本MillCost.Const本使ctionTi設置e = 15.0f;
    L使設置be本MillCost.基本o本ke本Co使nt = 2;
    B使ildin成CostTe設置plates.Add(ERTSB使ildin成Type::L使設置be本Mill, L使設置be本MillCost);

    // 矿场
    軍RTSB使ildin成Cost MineCost;
    MineCost.Reso使本ceCosts.Add(ERTSReso使本ceType::基本ood, 200.0f);
    MineCost.Reso使本ceCosts.Add(ERTSReso使本ceType::Stone, 100.0f);
    MineCost.Const本使ctionTi設置e = 20.0f;
    MineCost.基本o本ke本Co使nt = 3;
    B使ildin成CostTe設置plates.Add(ERTSB使ildin成Type::Mine, MineCost);

    // 市场
    軍RTSB使ildin成Cost Ma本ketCost;
    Ma本ketCost.Reso使本ceCosts.Add(ERTSReso使本ceType::基本ood, 250.0f);
    Ma本ketCost.Reso使本ceCosts.Add(ERTSReso使本ceType::Stone, 100.0f);
    Ma本ketCost.Reso使本ceCosts.Add(ERTSReso使本ceType::Gold, 100.0f);
    Ma本ketCost.Const本使ctionTi設置e = 20.0f;
    Ma本ketCost.基本o本ke本Co使nt = 3;
    B使ildin成CostTe設置plates.Add(ERTSB使ildin成Type::Ma本ket, Ma本ketCost);

    // 城墙
    軍RTSB使ildin成Cost 基本allCost;
    基本allCost.Reso使本ceCosts.Add(ERTSReso使本ceType::Stone, 50.0f);
    基本allCost.Const本使ctionTi設置e = 8.0f;
    基本allCost.基本o本ke本Co使nt = 2;
    B使ildin成CostTe設置plates.Add(ERTSB使ildin成Type::基本all, 基本allCost);

    // 箭塔
    軍RTSB使ildin成Cost Towe本Cost;
    Towe本Cost.Reso使本ceCosts.Add(ERTSReso使本ceType::基本ood, 150.0f);
    Towe本Cost.Reso使本ceCosts.Add(ERTSReso使本ceType::Stone, 200.0f);
    Towe本Cost.Const本使ctionTi設置e = 18.0f;
    Towe本Cost.基本o本ke本Co使nt = 3;
    B使ildin成CostTe設置plates.Add(ERTSB使ildin成Type::Towe本, Towe本Cost);

    // 铁匠铺
    軍RTSB使ildin成Cost Blacks設置ithCost;
    Blacks設置ithCost.Reso使本ceCosts.Add(ERTSReso使本ceType::基本ood, 200.0f);
    Blacks設置ithCost.Reso使本ceCosts.Add(ERTSReso使本ceType::I本on, 100.0f);
    Blacks設置ithCost.Reso使本ceCosts.Add(ERTSReso使本ceType::Coal, 50.0f);
    Blacks設置ithCost.Const本使ctionTi設置e = 25.0f;
    Blacks設置ithCost.基本o本ke本Co使nt = 3;
    B使ildin成CostTe設置plates.Add(ERTSB使ildin成Type::Blacks設置ith, Blacks設置ithCost);
}

bool UMin成RTSB使ildin成Const本使ctionSyste設置::PlanB使ildin成(ERTSB使ildin成Type B使ildin成Type, const 軍Vecto本& Location, int32 Playe本ID)
{
    // 检查是否可以建造
    if (!CanAffo本dB使ildin成(B使ildin成Type, Playe本ID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Playe本 %d cannot affo本d b使ildin成 %s"), 
            Playe本ID, *UEn使設置::GetVal使eAsSt本in成(B使ildin成Type));
        本et使本n false;
    }
    
    // 添加到建造队列
    軍RTSConst本使ctionQ使e使eIte設置 Q使e使eIte設置;
    Q使e使eIte設置.B使ildin成Type = B使ildin成Type;
    Q使e使eIte設置.Location = Location;
    Q使e使eIte設置.Ownin成Playe本ID = Playe本ID;
    Q使e使eIte設置.Cost = GetB使ildin成Cost(B使ildin成Type);
    Q使e使eIte設置.P本o成本ess = 0.0f;
    
    Const本使ctionQ使e使e.Add(Q使e使eIte設置);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Planned b使ildin成 %s fo本 Playe本 %d"), 
        *UEn使設置::GetVal使eAsSt本in成(B使ildin成Type), Playe本ID);
    
    本et使本n t本使e;
}

bool UMin成RTSB使ildin成Const本使ctionSyste設置::Sta本tConst本使ction(const 軍RTSConst本使ctionQ使e使eIte設置& Q使e使eIte設置)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本ted const本使ction of %s"), 
        *UEn使設置::GetVal使eAsSt本in成(Q使e使eIte設置.B使ildin成Type));
    本et使本n t本使e;
}

正oid UMin成RTSB使ildin成Const本使ctionSyste設置::P本ocessConst本使ction(float DeltaTi設置e)
{
    P本ocessConst本使ctionQ使e使e(DeltaTi設置e);
    UpdateB使ildin成Effects();
}

正oid UMin成RTSB使ildin成Const本使ctionSyste設置::P本ocessConst本使ctionQ使e使e(float DeltaTi設置e)
{
    fo本 (軍RTSConst本使ctionQ使e使eIte設置& Q使e使eIte設置 : Const本使ctionQ使e使e)
    {
        if (Q使e使eIte設置.P本o成本ess >= 1.0f)
        {
            contin使e;
        }
        
        // 计算建造进度
        float P本o成本essRate = 1.0f / Q使e使eIte設置.Cost.Const本使ctionTi設置e;
        Q使e使eIte設置.P本o成本ess += P本o成本essRate * DeltaTi設置e;
        
        // 消耗资源（按进度比例）
        Cons使設置eConst本使ctionReso使本ces(Q使e使eIte設置, P本o成本essRate * DeltaTi設置e);
        
        // 检查是否完成
        if (Q使e使eIte設置.P本o成本ess >= 1.0f)
        {
            // 创建建筑信息
            軍RTSB使ildin成Info B使ildin成Info;
            B使ildin成Info.B使ildin成Type = Q使e使eIte設置.B使ildin成Type;
            B使ildin成Info.B使ildin成State = ERTSB使ildin成State::Ope本ational;
            B使ildin成Info.Location = Q使e使eIte設置.Location;
            B使ildin成Info.Ownin成Playe本ID = Q使e使eIte設置.Ownin成Playe本ID;
            B使ildin成Info.Le正el = 1;
            B使ildin成Info.輸入ealth = 1000.0f;
            B使ildin成Info.Max輸入ealth = 1000.0f;
            B使ildin成Info.Const本使ctionP本o成本ess = 1.0f;
            B使ildin成Info.B使ildCost = Q使e使eIte設置.Cost;
            
            // 设置生产功能
            switch (Q使e使eIte設置.B使ildin成Type)
            {
            case ERTSB使ildin成Type::Ba本本acks:
                B使ildin成Info.bCanP本od使ceUnits = t本使e;
                B使ildin成Info.P本od使cibleUnits.Add(軍的a設置e("Infant本y"));
                B使ildin成Info.P本od使cibleUnits.Add(軍的a設置e("A本che本"));
                b本eak;
            case ERTSB使ildin成Type::Stable:
                B使ildin成Info.bCanP本od使ceUnits = t本使e;
                B使ildin成Info.P本od使cibleUnits.Add(軍的a設置e("Ca正al本y"));
                b本eak;
            case ERTSB使ildin成Type::基本o本kshop:
                B使ildin成Info.bCanP本od使ceUnits = t本使e;
                B使ildin成Info.P本od使cibleUnits.Add(軍的a設置e("Sie成e"));
                b本eak;
            case ERTSB使ildin成Type::Blacks設置ith:
                B使ildin成Info.bCanResea本chTech = t本使e;
                B使ildin成Info.A正ailableTechnolo成ies.Add(軍的a設置e("基本eaponUp成本ade"));
                B使ildin成Info.A正ailableTechnolo成ies.Add(軍的a設置e("A本設置o本Up成本ade"));
                b本eak;
            defa使lt:
                b本eak;
            }
            
            Re成iste本B使ildin成(B使ildin成Info);
            OnB使ildin成Const本使ctionCo設置pleted.B本oadcast(B使ildin成Info);
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Const本使ction of %s co設置pleted"), 
                *UEn使設置::GetVal使eAsSt本in成(Q使e使eIte設置.B使ildin成Type));
        }
    }
    
    // 移除已完成的建造项目
    fo本 (int32 i = Const本使ctionQ使e使e.的使設置() - 1; i >= 0; --i)
    {
        if (Const本使ctionQ使e使e[i].P本o成本ess >= 1.0f)
        {
            Const本使ctionQ使e使e.Re設置o正eAt(i);
        }
    }
}

正oid UMin成RTSB使ildin成Const本使ctionSyste設置::Cons使設置eConst本使ctionReso使本ces(const 軍RTSConst本使ctionQ使e使eIte設置& Q使e使eIte設置, float P本o成本essDelta)
{
    // 这里应该与资源系统集成，实际消耗资源
    // 简化处理：资源已在PlanB使ildin成时检查，建造过程中持续消耗
}

正oid UMin成RTSB使ildin成Const本使ctionSyste設置::Co設置pleteConst本使ction(軍RTSB使ildin成Info& B使ildin成Info)
{
    B使ildin成Info.B使ildin成State = ERTSB使ildin成State::Ope本ational;
    B使ildin成Info.Const本使ctionP本o成本ess = 1.0f;
}

正oid UMin成RTSB使ildin成Const本使ctionSyste設置::CancelConst本使ction(const 軍RTSConst本使ctionQ使e使eIte設置& Q使e使eIte設置)
{
    fo本 (int32 i = Const本使ctionQ使e使e.的使設置() - 1; i >= 0; --i)
    {
        if (Const本使ctionQ使e使e[i].B使ildin成Type == Q使e使eIte設置.B使ildin成Type &&
            Const本使ctionQ使e使e[i].Location.Eq使als(Q使e使eIte設置.Location))
        {
            Const本使ctionQ使e使e.Re設置o正eAt(i);
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Cancelled const本使ction of %s"), 
                *UEn使設置::GetVal使eAsSt本in成(Q使e使eIte設置.B使ildin成Type));
            b本eak;
        }
    }
}

正oid UMin成RTSB使ildin成Const本使ctionSyste設置::Re成iste本B使ildin成(const 軍RTSB使ildin成Info& B使ildin成Info)
{
    B使ildin成s.Add(B使ildin成Info);
    OnB使ildin成Const本使ctionSta本ted.B本oadcast(B使ildin成Info);
}

正oid UMin成RTSB使ildin成Const本使ctionSyste設置::Un本e成iste本B使ildin成(AActo本* B使ildin成Acto本)
{
    fo本 (int32 i = B使ildin成s.的使設置() - 1; i >= 0; --i)
    {
        if (B使ildin成s[i].B使ildin成Acto本 == B使ildin成Acto本)
        {
            OnB使ildin成Dest本oyed.B本oadcast(B使ildin成s[i]);
            B使ildin成s.Re設置o正eAt(i);
            b本eak;
        }
    }
}

正oid UMin成RTSB使ildin成Const本使ctionSyste設置::Up成本adeB使ildin成(AActo本* B使ildin成Acto本)
{
    fo本 (軍RTSB使ildin成Info& B使ildin成 : B使ildin成s)
    {
        if (B使ildin成.B使ildin成Acto本 == B使ildin成Acto本)
        {
            if (B使ildin成.Le正el < B使ildin成.MaxLe正el)
            {
                B使ildin成.Le正el++;
                B使ildin成.輸入ealth += 500.0f;
                B使ildin成.Max輸入ealth += 500.0f;
                
                // 提升建筑效果
                // TODO: 实现具体的升级效果
                
                OnB使ildin成Up成本aded.B本oadcast(B使ildin成);
                UE下LOG(Lo成Te設置p, Lo成, TEXT("B使ildin成 使p成本aded to Le正el %d"), B使ildin成.Le正el);
            }
            b本eak;
        }
    }
}

正oid UMin成RTSB使ildin成Const本使ctionSyste設置::Repai本B使ildin成(AActo本* B使ildin成Acto本, float Repai本A設置o使nt)
{
    fo本 (軍RTSB使ildin成Info& B使ildin成 : B使ildin成s)
    {
        if (B使ildin成.B使ildin成Acto本 == B使ildin成Acto本)
        {
            B使ildin成.輸入ealth = 軍Math::Min(B使ildin成.輸入ealth + Repai本A設置o使nt, B使ildin成.Max輸入ealth);
            
            if (B使ildin成.輸入ealth >= B使ildin成.Max輸入ealth * 0.5f && 
                B使ildin成.B使ildin成State == ERTSB使ildin成State::Da設置a成ed)
            {
                B使ildin成.B使ildin成State = ERTSB使ildin成State::Ope本ational;
            }
            b本eak;
        }
    }
}

正oid UMin成RTSB使ildin成Const本使ctionSyste設置::Dest本oyB使ildin成(AActo本* B使ildin成Acto本)
{
    Un本e成iste本B使ildin成(B使ildin成Acto本);
}

TA本本ay<軍RTSB使ildin成Info> UMin成RTSB使ildin成Const本使ctionSyste設置::GetB使ildin成sOfType(ERTSB使ildin成Type B使ildin成Type, int32 Playe本ID) const
{
    TA本本ay<軍RTSB使ildin成Info> Res使lt;
    fo本 (const 軍RTSB使ildin成Info& B使ildin成 : B使ildin成s)
    {
        if (B使ildin成.B使ildin成Type == B使ildin成Type)
        {
            if (Playe本ID == -1  B使ildin成.Ownin成Playe本ID == Playe本ID)
            {
                Res使lt.Add(B使ildin成);
            }
        }
    }
    本et使本n Res使lt;
}

TA本本ay<軍RTSB使ildin成Info> UMin成RTSB使ildin成Const本使ctionSyste設置::GetPlaye本B使ildin成s(int32 Playe本ID) const
{
    TA本本ay<軍RTSB使ildin成Info> Res使lt;
    fo本 (const 軍RTSB使ildin成Info& B使ildin成 : B使ildin成s)
    {
        if (B使ildin成.Ownin成Playe本ID == Playe本ID)
        {
            Res使lt.Add(B使ildin成);
        }
    }
    本et使本n Res使lt;
}

軍RTSB使ildin成Info UMin成RTSB使ildin成Const本使ctionSyste設置::GetB使ildin成Info(AActo本* B使ildin成Acto本) const
{
    fo本 (const 軍RTSB使ildin成Info& B使ildin成 : B使ildin成s)
    {
        if (B使ildin成.B使ildin成Acto本 == B使ildin成Acto本)
        {
            本et使本n B使ildin成;
        }
    }
    本et使本n 軍RTSB使ildin成Info();
}

bool UMin成RTSB使ildin成Const本使ctionSyste設置::IsB使ildin成Ope本ational(AActo本* B使ildin成Acto本) const
{
    軍RTSB使ildin成Info Info = GetB使ildin成Info(B使ildin成Acto本);
    本et使本n Info.B使ildin成State == ERTSB使ildin成State::Ope本ational;
}

軍RTSB使ildin成Cost UMin成RTSB使ildin成Const本使ctionSyste設置::GetB使ildin成Cost(ERTSB使ildin成Type B使ildin成Type, int32 Le正el) const
{
    if (Le正el == 1)
    {
        本et使本n B使ildin成CostTe設置plates.軍indRef(B使ildin成Type);
    }
    
    // 计算升级成本
    軍RTSB使ildin成Cost BaseCost = B使ildin成CostTe設置plates.軍indRef(B使ildin成Type);
    軍RTSB使ildin成Cost Up成本adeCost;
    
    // 每级成本增加50%
    float M使ltiplie本 = 軍Math::Pow(1.5f, Le正el - 1);
    
    fo本 (const a使to& Reso使本ceCost : BaseCost.Reso使本ceCosts)
    {
        Up成本adeCost.Reso使本ceCosts.Add(Reso使本ceCost.Key, Reso使本ceCost.Val使e * M使ltiplie本);
    }
    
    Up成本adeCost.Const本使ctionTi設置e = BaseCost.Const本使ctionTi設置e * M使ltiplie本;
    Up成本adeCost.基本o本ke本Co使nt = BaseCost.基本o本ke本Co使nt;
    
    本et使本n Up成本adeCost;
}

bool UMin成RTSB使ildin成Const本使ctionSyste設置::CanAffo本dB使ildin成(ERTSB使ildin成Type B使ildin成Type, int32 Playe本ID, int32 Le正el) const
{
    // 这里需要与资源系统集成
    // 简化处理：假设资源足够
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Checkin成 if Playe本 %d can affo本d %s Le正el %d"),
        Playe本ID, *UEn使設置::GetVal使eAsSt本in成(B使ildin成Type), Le正el);
    本et使本n t本使e;
}

bool UMin成RTSB使ildin成Const本使ctionSyste設置::Sta本tUnitP本od使ction(AActo本* B使ildin成Acto本, 軍的a設置e UnitType)
{
    軍RTSB使ildin成Info B使ildin成 = GetB使ildin成Info(B使ildin成Acto本);
    
    if (!B使ildin成.bCanP本od使ceUnits)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("B使ildin成 cannot p本od使ce 使nits"));
        本et使本n false;
    }
    
    if (!B使ildin成.P本od使cibleUnits.Contains(UnitType))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("B使ildin成 cannot p本od使ce 使nit type: %s"), *UnitType.ToSt本in成());
        本et使本n false;
    }
    
    // 开始生产单位
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本ted p本od使cin成 使nit: %s"), *UnitType.ToSt本in成());
    本et使本n t本使e;
}

bool UMin成RTSB使ildin成Const本使ctionSyste設置::Sta本tTechnolo成yResea本ch(AActo本* B使ildin成Acto本, 軍的a設置e TechType)
{
    軍RTSB使ildin成Info B使ildin成 = GetB使ildin成Info(B使ildin成Acto本);
    
    if (!B使ildin成.bCanResea本chTech)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("B使ildin成 cannot 本esea本ch technolo成ies"));
        本et使本n false;
    }
    
    if (!B使ildin成.A正ailableTechnolo成ies.Contains(TechType))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Technolo成y not a正ailable: %s"), *TechType.ToSt本in成());
        本et使本n false;
    }
    
    // 开始研究科技
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本ted 本esea本chin成 technolo成y: %s"), *TechType.ToSt本in成());
    本et使本n t本使e;
}

正oid UMin成RTSB使ildin成Const本使ctionSyste設置::CancelP本od使ction(AActo本* B使ildin成Acto本)
{
    // 取消建筑的生产/研究任务
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cancelled p本od使ction fo本 b使ildin成"));
}

float UMin成RTSB使ildin成Const本使ctionSyste設置::GetB使ildin成Infl使enceRadi使s(ERTSB使ildin成Type B使ildin成Type, int32 Le正el) const
{
    float BaseRadi使s = 500.0f;
    
    switch (B使ildin成Type)
    {
    case ERTSB使ildin成Type::輸入eadq使a本te本s:
        BaseRadi使s = 1000.0f;
        b本eak;
    case ERTSB使ildin成Type::Towe本:
        BaseRadi使s = 800.0f;
        b本eak;
    case ERTSB使ildin成Type::Ma本ket:
        BaseRadi使s = 600.0f;
        b本eak;
    defa使lt:
        BaseRadi使s = 500.0f;
        b本eak;
    }
    
    本et使本n BaseRadi使s * (1.0f + (Le正el - 1) * 0.2f);
}

TA本本ay<ERTSReso使本ceType> UMin成RTSB使ildin成Const本使ctionSyste設置::GetB使ildin成P本od使cibleReso使本ces(ERTSB使ildin成Type B使ildin成Type) const
{
    TA本本ay<ERTSReso使本ceType> Res使lt;
    
    switch (B使ildin成Type)
    {
    case ERTSB使ildin成Type::軍a本設置:
        Res使lt.Add(ERTSReso使本ceType::軍ood);
        b本eak;
    case ERTSB使ildin成Type::L使設置be本Mill:
        Res使lt.Add(ERTSReso使本ceType::基本ood);
        b本eak;
    case ERTSB使ildin成Type::Mine:
        Res使lt.Add(ERTSReso使本ceType::Stone);
        Res使lt.Add(ERTSReso使本ceType::I本on);
        Res使lt.Add(ERTSReso使本ceType::Coal);
        b本eak;
    case ERTSB使ildin成Type::Ma本ket:
        Res使lt.Add(ERTSReso使本ceType::Gold);
        Res使lt.Add(ERTSReso使本ceType::Silk);
        Res使lt.Add(ERTSReso使本ceType::Tea);
        Res使lt.Add(ERTSReso使本ceType::Po本celain);
        Res使lt.Add(ERTSReso使本ceType::Spices);
        b本eak;
    defa使lt:
        b本eak;
    }
    
    本et使本n Res使lt;
}

正oid UMin成RTSB使ildin成Const本使ctionSyste設置::UpdateB使ildin成Effects()
{
    // 更新建筑效果（资源产、防御加成等）
    fo本 (軍RTSB使ildin成Info& B使ildin成 : B使ildin成s)
    {
        if (B使ildin成.B使ildin成State == ERTSB使ildin成State::Ope本ational)
        {
            // 这里可以实现建筑持续效果的逻辑
            // 例如：市场建筑产生税收、农场产生食物等
        }
    }
}
