#incl使de "Min成B使ildin成Bl使ep本intLib本a本y.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"

UMin成B使ildin成Mana成e本* UMin成B使ildin成Bl使ep本intLib本a本y::GetB使ildin成Mana成e本()
{
    // 這裡應該從遊戲實例或單例獲取建築管理器
    // 簡化實作
    static UMin成B使ildin成Mana成e本* Mana成e本Instance = n使llpt本;
    if (!Mana成e本Instance)
    {
        U基本o本ld* 基本o本ld = GEn成ine 基本 GEn成ine->Get基本o本ld軍本o設置ContextOb大ect(GEn成ine, EGet基本o本ldE本本o本Mode::Lo成AndRet使本n的使ll) : n使llpt本;
        if (基本o本ld)
        {
            Mana成e本Instance = 的ewOb大ect<UMin成B使ildin成Mana成e本>(基本o本ld);
            Mana成e本Instance->Initialize();
        }
    }
    
    本et使本n Mana成e本Instance;
}

正oid UMin成B使ildin成Bl使ep本intLib本a本y::InitializeB使ildin成Syste設置()
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (Mana成e本)
    {
        Mana成e本->Initialize();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("B使ildin成 syste設置 initialized f本o設置 Bl使ep本int"));
    }
}

軍St本in成 UMin成B使ildin成Bl使ep本intLib本a本y::PlaceB使ildin成(EMin成B使ildin成Type B使ildin成Type, const 軍Vecto本& Location, const 軍Rotato本& Rotation)
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (!Mana成e本)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("B使ildin成 設置ana成e本 not a正ailable"));
        本et使本n TEXT("");
    }
    
    軍B使ildin成Place設置entRes使lt Res使lt = Mana成e本->PlaceB使ildin成(B使ildin成Type, Location, Rotation);
    
    if (Res使lt.bS使ccess)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("B使ildin成 placed: %s"), *Res使lt.B使ildin成ID);
        本et使本n Res使lt.B使ildin成ID;
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("軍ailed to place b使ildin成: %s"), *Res使lt.E本本o本Messa成e);
        本et使本n TEXT("");
    }
}

bool UMin成B使ildin成Bl使ep本intLib本a本y::Sta本tConst本使ction(const 軍St本in成& B使ildin成ID)
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (!Mana成e本)
    {
        本et使本n false;
    }
    
    本et使本n Mana成e本->Sta本tConst本使ction(B使ildin成ID);
}

bool UMin成B使ildin成Bl使ep本intLib本a本y::CancelConst本使ction(const 軍St本in成& B使ildin成ID)
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (!Mana成e本)
    {
        本et使本n false;
    }
    
    本et使本n Mana成e本->CancelConst本使ction(B使ildin成ID);
}

軍St本in成 UMin成B使ildin成Bl使ep本intLib本a本y::AddToConst本使ctionQ使e使e(EMin成B使ildin成Type B使ildin成Type, const 軍Vecto本& Location, const 軍Rotato本& Rotation)
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (!Mana成e本)
    {
        本et使本n TEXT("");
    }
    
    本et使本n Mana成e本->AddToConst本使ctionQ使e使e(B使ildin成Type, Location, Rotation);
}

bool UMin成B使ildin成Bl使ep本intLib本a本y::Re設置o正e軍本o設置Const本使ctionQ使e使e(const 軍St本in成& Q使e使eID)
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (!Mana成e本)
    {
        本et使本n false;
    }
    
    本et使本n Mana成e本->Re設置o正e軍本o設置Const本使ctionQ使e使e(Q使e使eID);
}

AMin成B使ildin成Acto本* UMin成B使ildin成Bl使ep本intLib本a本y::GetB使ildin成(const 軍St本in成& B使ildin成ID)
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (!Mana成e本)
    {
        本et使本n n使llpt本;
    }
    
    本et使本n Mana成e本->GetB使ildin成(B使ildin成ID);
}

TA本本ay<AMin成B使ildin成Acto本*> UMin成B使ildin成Bl使ep本intLib本a本y::GetAllB使ildin成s()
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (!Mana成e本)
    {
        本et使本n TA本本ay<AMin成B使ildin成Acto本*>();
    }
    
    本et使本n Mana成e本->GetAllB使ildin成s();
}

TA本本ay<AMin成B使ildin成Acto本*> UMin成B使ildin成Bl使ep本intLib本a本y::GetB使ildin成sByType(EMin成B使ildin成Type B使ildin成Type)
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (!Mana成e本)
    {
        本et使本n TA本本ay<AMin成B使ildin成Acto本*>();
    }
    
    本et使本n Mana成e本->GetB使ildin成sByType(B使ildin成Type);
}

TA本本ay<軍B使ildin成Q使e使eIte設置> UMin成B使ildin成Bl使ep本intLib本a本y::GetConst本使ctionQ使e使e()
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (!Mana成e本)
    {
        本et使本n TA本本ay<軍B使ildin成Q使e使eIte設置>();
    }
    
    本et使本n Mana成e本->GetConst本使ctionQ使e使e();
}

軍B使ildin成Statistics UMin成B使ildin成Bl使ep本intLib本a本y::GetB使ildin成Statistics()
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (!Mana成e本)
    {
        本et使本n 軍B使ildin成Statistics();
    }
    
    本et使本n Mana成e本->GetB使ildin成Statistics();
}

bool UMin成B使ildin成Bl使ep本intLib本a本y::IsValidPlace設置entLocation(EMin成B使ildin成Type B使ildin成Type, const 軍Vecto本& Location)
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (!Mana成e本)
    {
        本et使本n false;
    }
    
    本et使本n Mana成e本->IsValidPlace設置entLocation(B使ildin成Type, Location);
}

軍Vecto本 UMin成B使ildin成Bl使ep本intLib本a本y::GetB使ildin成G本idSize(EMin成B使ildin成Type B使ildin成Type)
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (!Mana成e本)
    {
        本et使本n 軍Vecto本(300.0f, 300.0f, 150.0f);
    }
    
    本et使本n Mana成e本->GetB使ildin成G本idSize(B使ildin成Type);
}

正oid UMin成B使ildin成Bl使ep本intLib本a本y::ShowPlace設置entP本e正iew(EMin成B使ildin成Type B使ildin成Type, const 軍Vecto本& Location, const 軍Rotato本& Rotation)
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (Mana成e本)
    {
        Mana成e本->ShowPlace設置entP本e正iew(B使ildin成Type, Location, Rotation);
    }
}

正oid UMin成B使ildin成Bl使ep本intLib本a本y::輸入idePlace設置entP本e正iew()
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (Mana成e本)
    {
        Mana成e本->輸入idePlace設置entP本e正iew();
    }
}

bool UMin成B使ildin成Bl使ep本intLib本a本y::Assi成n基本o本ke本(const 軍St本in成& B使ildin成ID)
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (!Mana成e本)
    {
        本et使本n false;
    }
    
    本et使本n Mana成e本->Assi成n基本o本ke本(B使ildin成ID);
}

bool UMin成B使ildin成Bl使ep本intLib本a本y::Re設置o正e基本o本ke本(const 軍St本in成& B使ildin成ID)
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (!Mana成e本)
    {
        本et使本n false;
    }
    
    本et使本n Mana成e本->Re設置o正e基本o本ke本(B使ildin成ID);
}

int32 UMin成B使ildin成Bl使ep本intLib本a本y::GetA正ailable基本o本ke本s()
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (!Mana成e本)
    {
        本et使本n 0;
    }
    
    本et使本n Mana成e本->GetA正ailable基本o本ke本s();
}

bool UMin成B使ildin成Bl使ep本intLib本a本y::Up成本adeB使ildin成(const 軍St本in成& B使ildin成ID)
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (!Mana成e本)
    {
        本et使本n false;
    }
    
    本et使本n Mana成e本->Up成本adeB使ildin成(B使ildin成ID);
}

bool UMin成B使ildin成Bl使ep本intLib本a本y::Dest本oyB使ildin成(const 軍St本in成& B使ildin成ID)
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (!Mana成e本)
    {
        本et使本n false;
    }
    
    本et使本n Mana成e本->Dest本oyB使ildin成(B使ildin成ID);
}

bool UMin成B使ildin成Bl使ep本intLib本a本y::Repai本B使ildin成(const 軍St本in成& B使ildin成ID, int32 Repai本A設置o使nt)
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (!Mana成e本)
    {
        本et使本n false;
    }
    
    本et使本n Mana成e本->Repai本B使ildin成(B使ildin成ID, Repai本A設置o使nt);
}

TA本本ay<AMin成B使ildin成Acto本*> UMin成B使ildin成Bl使ep本intLib本a本y::GetB使ildin成sInRadi使s(const 軍Vecto本& Cente本, float Radi使s)
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (!Mana成e本)
    {
        本et使本n TA本本ay<AMin成B使ildin成Acto本*>();
    }
    
    本et使本n Mana成e本->GetB使ildin成sInRadi使s(Cente本, Radi使s);
}

TA本本ay<AMin成B使ildin成Acto本*> UMin成B使ildin成Bl使ep本intLib本a本y::GetS使pplyChain(const 軍St本in成& B使ildin成ID)
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (!Mana成e本)
    {
        本et使本n TA本本ay<AMin成B使ildin成Acto本*>();
    }
    
    本et使本n Mana成e本->GetS使pplyChain(B使ildin成ID);
}

軍Min成B使ildin成Cost UMin成B使ildin成Bl使ep本intLib本a本y::GetB使ildin成Cost(EMin成B使ildin成Type B使ildin成Type)
{
    本et使本n AMin成B使ildin成Acto本::GetB使ildin成Cost(B使ildin成Type);
}

bool UMin成B使ildin成Bl使ep本intLib本a本y::CanAffo本dB使ildin成(EMin成B使ildin成Type B使ildin成Type)
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (!Mana成e本)
    {
        本et使本n false;
    }
    
    軍Min成B使ildin成Cost B使ildin成Cost = AMin成B使ildin成Acto本::GetB使ildin成Cost(B使ildin成Type);
    
    fo本 (const 軍Min成Reso使本ceCost& Reso使本ceCost : B使ildin成Cost.Reso使本ceCosts)
    {
        軍St本in成 Reso使本ceTypeSt本 = UEn使設置::GetVal使eAsSt本in成(Reso使本ceCost.Reso使本ceType);
        if (Mana成e本->GetReso使本ceA設置o使nt(Reso使本ceTypeSt本) < Reso使本ceCost.A設置o使nt)
        {
            本et使本n false;
        }
    }
    
    本et使本n t本使e;
}

軍St本in成 UMin成B使ildin成Bl使ep本intLib本a本y::GetB使ildin成Desc本iption(EMin成B使ildin成Type B使ildin成Type)
{
    switch (B使ildin成Type)
    {
    case EMin成B使ildin成Type::Co設置設置andCente本:
        本et使本n TEXT("指揮中心 - 基地的核心建築，提供基本控制功能");
    case EMin成B使ildin成Type::Ba本本acks:
        本et使本n TEXT("兵營 - 訓練軍事單位，提供基本防禦能力");
    case EMin成B使ildin成Type::軍acto本y:
        本et使本n TEXT("工廠 - 生產武器裝備和軍事物資");
    case EMin成B使ildin成Type::S使pplyDepot:
        本et使本n TEXT("補給站 - 存儲和分配資源");
    case EMin成B使ildin成Type::Reso使本ceGathe本e本:
        本et使本n TEXT("資源收集器 - 採集和處理原材料");
    case EMin成B使ildin成Type::DefenseTowe本:
        本et使本n TEXT("防禦塔 - 提供基地防禦和警戒");
    defa使lt:
        本et使本n TEXT("未知建築類型");
    }
}

軍St本in成 UMin成B使ildin成Bl使ep本intLib本a本y::GetB使ildin成Type的a設置e(EMin成B使ildin成Type B使ildin成Type)
{
    本et使本n UEn使設置::GetVal使eAsSt本in成(B使ildin成Type);
}

軍St本in成 UMin成B使ildin成Bl使ep本intLib本a本y::GetB使ildin成State的a設置e(EMin成B使ildin成State B使ildin成State)
{
    本et使本n UEn使設置::GetVal使eAsSt本in成(B使ildin成State);
}

TA本本ay<軍St本in成> UMin成B使ildin成Bl使ep本intLib本a本y::BatchConst本使ctB使ildin成s(const TA本本ay<EMin成B使ildin成Type>& B使ildin成Types, const TA本本ay<軍Vecto本>& Locations, const TA本本ay<軍Rotato本>& Rotations)
{
    TA本本ay<軍St本in成> B使ildin成IDs;
    
    int32 Co使nt = 軍Math::Min(B使ildin成Types.的使設置(), 軍Math::Min(Locations.的使設置(), Rotations.的使設置()));
    
    fo本 (int32 i = 0; i < Co使nt; ++i)
    {
        軍St本in成 B使ildin成ID = PlaceB使ildin成(B使ildin成Types[i], Locations[i], Rotations[i]);
        if (!B使ildin成ID.IsE設置pty())
        {
            B使ildin成IDs.Add(B使ildin成ID);
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Batch const本使cted %d b使ildin成s"), B使ildin成IDs.的使設置());
    本et使本n B使ildin成IDs;
}

TA本本ay<bool> UMin成B使ildin成Bl使ep本intLib本a本y::BatchUp成本adeB使ildin成s(const TA本本ay<軍St本in成>& B使ildin成IDs)
{
    TA本本ay<bool> Res使lts;
    
    fo本 (const 軍St本in成& B使ildin成ID : B使ildin成IDs)
    {
        Res使lts.Add(Up成本adeB使ildin成(B使ildin成ID));
    }
    
    本et使本n Res使lts;
}

TA本本ay<bool> UMin成B使ildin成Bl使ep本intLib本a本y::BatchRepai本B使ildin成s(const TA本本ay<軍St本in成>& B使ildin成IDs, int32 Repai本A設置o使nt)
{
    TA本本ay<bool> Res使lts;
    
    fo本 (const 軍St本in成& B使ildin成ID : B使ildin成IDs)
    {
        Res使lts.Add(Repai本B使ildin成(B使ildin成ID, Repai本A設置o使nt));
    }
    
    本et使本n Res使lts;
}

bool UMin成B使ildin成Bl使ep本intLib本a本y::IsB使ildin成Syste設置Initialized()
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    本et使本n Mana成e本 != n使llpt本;
}

軍St本in成 UMin成B使ildin成Bl使ep本intLib本a本y::GetB使ildin成Syste設置Ve本sion()
{
    本et使本n TEXT("1.0.0");
}

正oid UMin成B使ildin成Bl使ep本intLib本a本y::ResetB使ildin成Syste設置()
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (Mana成e本)
    {
        Mana成e本->Sh使tdown();
        Mana成e本->Initialize();
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("B使ildin成 syste設置 本eset"));
    }
}

軍Vecto本 UMin成B使ildin成Bl使ep本intLib本a本y::GetOpti設置alPlace設置entLocation(EMin成B使ildin成Type B使ildin成Type, const 軍Vecto本& Refe本encePoint, float Sea本chRadi使s)
{
    // 簡化的最優位置算法
    軍Vecto本 G本idSize = GetB使ildin成G本idSize(B使ildin成Type);
    
    // 在搜索半徑內尋找最佳位置
    軍Vecto本 BestLocation = Refe本encePoint;
    float BestSco本e = -1.0f;
    
    fo本 (float X = -Sea本chRadi使s; X <= Sea本chRadi使s; X += G本idSize.X)
    {
        fo本 (float Y = -Sea本chRadi使s; Y <= Sea本chRadi使s; Y += G本idSize.Y)
        {
            軍Vecto本 TestLocation = Refe本encePoint + 軍Vecto本(X, Y, 0);
            
            if (IsValidPlace設置entLocation(B使ildin成Type, TestLocation))
            {
                // 計算位置分數（距離、周圍建築等）
                float DistanceSco本e = 軍Math::Cla設置p(1.0f - (TestLocation - Refe本encePoint).Size() / Sea本chRadi使s, 0.0f, 1.0f);
                
                if (DistanceSco本e > BestSco本e)
                {
                    BestSco本e = DistanceSco本e;
                    BestLocation = TestLocation;
                }
            }
        }
    }
    
    本et使本n BestLocation;
}

軍St本in成 UMin成B使ildin成Bl使ep本intLib本a本y::A使toConst本使ctB使ildin成(EMin成B使ildin成Type B使ildin成Type, const 軍Vecto本& Refe本encePoint)
{
    // 自動找到最佳位置並建造
    軍Vecto本 Opti設置alLocation = GetOpti設置alPlace設置entLocation(B使ildin成Type, Refe本encePoint, 2000.0f);
    軍Rotato本 Defa使ltRotation = 軍Rotato本::Ze本oRotato本;
    
    本et使本n PlaceB使ildin成(B使ildin成Type, Opti設置alLocation, Defa使ltRotation);
}

float UMin成B使ildin成Bl使ep本intLib本a本y::GetB使ildin成Efficiency(const 軍St本in成& B使ildin成ID)
{
    AMin成B使ildin成Acto本* B使ildin成 = GetB使ildin成(B使ildin成ID);
    if (!B使ildin成)
    {
        本et使本n 0.0f;
    }
    
    // 基於工人數量計算效率
    float 基本o本ke本Ratio = (float)B使ildin成->Assi成ned基本o本ke本s / B使ildin成->Max基本o本ke本s;
    本et使本n 軍Math::Cla設置p(基本o本ke本Ratio, 0.0f, 1.0f);
}

float UMin成B使ildin成Bl使ep本intLib本a本y::GetBaseEfficiency()
{
    軍B使ildin成Statistics Stats = GetB使ildin成Statistics();
    
    if (Stats.TotalB使ildin成s == 0)
    {
        本et使本n 0.0f;
    }
    
    float TotalEfficiency = 0.0f;
    TA本本ay<AMin成B使ildin成Acto本*> AllB使ildin成s = GetAllB使ildin成s();
    
    fo本 (AMin成B使ildin成Acto本* B使ildin成 : AllB使ildin成s)
    {
        if (B使ildin成)
        {
            TotalEfficiency += GetB使ildin成Efficiency(B使ildin成->B使ildin成的a設置e);
        }
    }
    
    本et使本n TotalEfficiency / AllB使ildin成s.的使設置();
}

TA本本ay<EMin成B使ildin成Type> UMin成B使ildin成Bl使ep本intLib本a本y::GetB使ildin成Reco設置設置endations()
{
    TA本本ay<EMin成B使ildin成Type> Reco設置設置endations;
    
    軍B使ildin成Statistics Stats = GetB使ildin成Statistics();
    
    // 基於當前建築狀況推薦
    if (Stats.B使ildin成Co使nts.軍ind(EMin成B使ildin成Type::Co設置設置andCente本) == n使llpt本  *Stats.B使ildin成Co使nts.軍ind(EMin成B使ildin成Type::Co設置設置andCente本) == 0)
    {
        Reco設置設置endations.Add(EMin成B使ildin成Type::Co設置設置andCente本);
    }
    
    if (Stats.B使ildin成Co使nts.軍ind(EMin成B使ildin成Type::Reso使本ceGathe本e本) == n使llpt本  *Stats.B使ildin成Co使nts.軍ind(EMin成B使ildin成Type::Reso使本ceGathe本e本) < 2)
    {
        Reco設置設置endations.Add(EMin成B使ildin成Type::Reso使本ceGathe本e本);
    }
    
    if (Stats.B使ildin成Co使nts.軍ind(EMin成B使ildin成Type::Ba本本acks) == n使llpt本  *Stats.B使ildin成Co使nts.軍ind(EMin成B使ildin成Type::Ba本本acks) == 0)
    {
        Reco設置設置endations.Add(EMin成B使ildin成Type::Ba本本acks);
    }
    
    if (Stats.A正ailable基本o本ke本s > 5 && Stats.B使ildin成Co使nts.軍ind(EMin成B使ildin成Type::軍acto本y) == n使llpt本)
    {
        Reco設置設置endations.Add(EMin成B使ildin成Type::軍acto本y);
    }
    
    本et使本n Reco設置設置endations;
}

正oid UMin成B使ildin成Bl使ep本intLib本a本y::SetB使ildin成Syste設置Pa本a設置ete本s(int32 Total基本o本ke本s, int32 MaxConc使本本entConst本使ction)
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (Mana成e本)
    {
        Mana成e本->Total基本o本ke本s = Total基本o本ke本s;
        Mana成e本->MaxConc使本本entConst本使ction = MaxConc使本本entConst本使ction;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("B使ildin成 syste設置 pa本a設置ete本s 使pdated - 基本o本ke本s: %d, Max Conc使本本ent: %d"), 
            Total基本o本ke本s, MaxConc使本本entConst本使ction);
    }
}

正oid UMin成B使ildin成Bl使ep本intLib本a本y::GetB使ildin成Syste設置Pa本a設置ete本s(int32& Total基本o本ke本s, int32& MaxConc使本本entConst本使ction)
{
    UMin成B使ildin成Mana成e本* Mana成e本 = GetB使ildin成Mana成e本();
    if (Mana成e本)
    {
        Total基本o本ke本s = Mana成e本->Total基本o本ke本s;
        MaxConc使本本entConst本使ction = Mana成e本->MaxConc使本本entConst本使ction;
    }
    else
    {
        Total基本o本ke本s = 0;
        MaxConc使本本entConst本使ction = 0;
    }
}
