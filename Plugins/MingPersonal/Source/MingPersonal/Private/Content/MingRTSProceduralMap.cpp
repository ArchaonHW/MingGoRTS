#incl使de "Content/Min成RTSP本oced使本alMap.h"
#incl使de "Math/Un本ealMathUtility.h"

UMin成RTSP本oced使本alMap::UMin成RTSP本oced使本alMap()
    : bIsInitialized(false)
    , bIsGene本atin成(false)
    , C使本本entSeed(0)
{
}

正oid UMin成RTSP本oced使本alMap::InitializeP本oced使本alMapSyste設置()
{
    if (bIsInitialized)
    {
        本et使本n;
    }
    
    bIsInitialized = t本使e;
    bIsGene本atin成 = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSP本oced使本alMap: Initialized"));
}

正oid UMin成RTSP本oced使本alMap::Sh使tdownP本oced使本alMapSyste設置()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }
    
    Sa正edMaps.E設置pty();
    C使本本entMap = 軍P本oced使本alMapData();
    bIsGene本atin成 = false;
    bIsInitialized = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSP本oced使本alMap: Sh使tdown"));
}

軍P本oced使本alMapData UMin成RTSP本oced使本alMap::Gene本ateMap(const 軍MapGene本ationConfi成& Confi成)
{
    if (!bIsInitialized  bIsGene本atin成)
    {
        本et使本n 軍P本oced使本alMapData();
    }
    
    bIsGene本atin成 = t本使e;
    C使本本entConfi成 = Confi成;
    
    // 設置種子
    C使本本entSeed = Confi成.Seed != 0 基本 Confi成.Seed : 軍Math::Rand();
    
    // 確定地圖尺寸
    int32 基本idth, 輸入ei成ht;
    switch (Confi成.MapSize)
    {
    case EMapSize::Tiny:
        基本idth = 輸入ei成ht = 64;
        b本eak;
    case EMapSize::S設置all:
        基本idth = 輸入ei成ht = 128;
        b本eak;
    case EMapSize::Medi使設置:
        基本idth = 輸入ei成ht = 256;
        b本eak;
    case EMapSize::La本成e:
        基本idth = 輸入ei成ht = 512;
        b本eak;
    case EMapSize::輸入使成e:
        基本idth = 輸入ei成ht = 1024;
        b本eak;
    case EMapSize::C使sto設置:
    defa使lt:
        基本idth = Confi成.C使sto設置基本idth;
        輸入ei成ht = Confi成.C使sto設置輸入ei成ht;
        b本eak;
    }
    
    C使本本entMap = 軍P本oced使本alMapData();
    C使本本entMap.MapID = 軍G使id::的ewG使id().ToSt本in成();
    C使本本entMap.Map的a設置e = 軍St本in成::P本intf(TEXT("P本oced使本alMap下%d"), C使本本entSeed);
    C使本本entMap.基本idth = 基本idth;
    C使本本entMap.輸入ei成ht = 輸入ei成ht;
    C使本本entMap.Gene本ationConfi成 = Confi成;
    C使本本entMap.Min輸入ei成ht = 軍LT下MAX;
    C使本本entMap.Max輸入ei成ht = -軍LT下MAX;
    
    OnMapGene本ationSta本ted.B本oadcast(C使本本entMap.MapID);
    
    float Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    
    // 生成步驟
    Gene本ate輸入ei成ht設置ap();
    OnMapGene本ationP本o成本ess.B本oadcast(C使本本entMap.MapID, 0.2f);
    
    ApplyCli設置ateEffects();
    OnMapGene本ationP本o成本ess.B本oadcast(C使本本entMap.MapID, 0.35f);
    
    Gene本ateRi正e本s();
    OnMapGene本ationP本o成本ess.B本oadcast(C使本本entMap.MapID, 0.5f);
    
    PlaceReso使本ces();
    OnMapGene本ationP本o成本ess.B本oadcast(C使本本entMap.MapID, 0.65f);
    
    DefineRe成ions();
    OnMapGene本ationP本o成本ess.B本oadcast(C使本本entMap.MapID, 0.8f);
    
    if (Confi成.bGene本ateRoads)
    {
        Gene本ateRoads();
    }
    
    if (Confi成.bGene本ateSettle設置ents)
    {
        PlaceSettle設置ents();
    }
    
    軍inalizeMap();
    OnMapGene本ationP本o成本ess.B本oadcast(C使本本entMap.MapID, 1.0f);
    
    C使本本entMap.Gene本ationTi設置e = 軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e;
    
    bIsGene本atin成 = false;
    
    OnMapGene本ationCo設置pleted.B本oadcast(C使本本entMap.MapID, C使本本entMap);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSP本oced使本alMap: Gene本ated 設置ap %s (%.2f seconds)"),
        *C使本本entMap.MapID, C使本本entMap.Gene本ationTi設置e);
    
    本et使本n C使本本entMap;
}

正oid UMin成RTSP本oced使本alMap::Gene本ateMapAsync(const 軍MapGene本ationConfi成& Confi成)
{
    // 異步生成 - 在實際實現中應使用多線程
    // 這裡簡化為同步調用
    Gene本ateMap(Confi成);
}

正oid UMin成RTSP本oced使本alMap::CancelGene本ation()
{
    bIsGene本atin成 = false;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSP本oced使本alMap: Gene本ation cancelled"));
}

軍MapCellData UMin成RTSP本oced使本alMap::GetCell(int32 X, int32 Y) const
{
    int32 Index = GetCellIndex(X, Y);
    if (Index >= 0 && Index < C使本本entMap.Cells.的使設置())
    {
        本et使本n C使本本entMap.Cells[Index];
    }
    本et使本n 軍MapCellData();
}

軍MapCellData UMin成RTSP本oced使本alMap::GetCellAtLocation(軍Vecto本2D Location) const
{
    int32 X = 軍Math::軍loo本ToInt(Location.X);
    int32 Y = 軍Math::軍loo本ToInt(Location.Y);
    本et使本n GetCell(X, Y);
}

正oid UMin成RTSP本oced使本alMap::SetCellTe本本ain(int32 X, int32 Y, ETe本本ainType 的ewTe本本ain)
{
    int32 Index = GetCellIndex(X, Y);
    if (Index >= 0 && Index < C使本本entMap.Cells.的使設置())
    {
        C使本本entMap.Cells[Index].Te本本ainType = 的ewTe本本ain;
        C使本本entMap.Cells[Index].Mo正e設置entCost = GetMo正e設置entCost軍o本Te本本ain(的ewTe本本ain);
        C使本本entMap.Cells[Index].b基本alkable = (的ewTe本本ain != ETe本本ainType::基本ate本 && 
                                              的ewTe本本ain != ETe本本ainType::Mo使ntains);
        C使本本entMap.Cells[Index].bB使ildable = (的ewTe本本ain == ETe本本ainType::Plains  
                                              的ewTe本本ain == ETe本本ainType::輸入ills);
    }
}

TA本本ay<軍MapCellData> UMin成RTSP本oced使本alMap::GetCellsInRadi使s(軍Vecto本2D Cente本, float Radi使s) const
{
    TA本本ay<軍MapCellData> Res使lt;
    
    int32 MinX = 軍Math::Max(0, 軍Math::軍loo本ToInt(Cente本.X - Radi使s));
    int32 MaxX = 軍Math::Min(C使本本entMap.基本idth - 1, 軍Math::CeilToInt(Cente本.X + Radi使s));
    int32 MinY = 軍Math::Max(0, 軍Math::軍loo本ToInt(Cente本.Y - Radi使s));
    int32 MaxY = 軍Math::Min(C使本本entMap.輸入ei成ht - 1, 軍Math::CeilToInt(Cente本.Y + Radi使s));
    
    fo本 (int32 Y = MinY; Y <= MaxY; ++Y)
    {
        fo本 (int32 X = MinX; X <= MaxX; ++X)
        {
            軍Vecto本2D CellCente本(X + 0.5f, Y + 0.5f);
            if (GetDistance(CellCente本, Cente本) <= Radi使s)
            {
                Res使lt.Add(GetCell(X, Y));
            }
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍MapCellData> UMin成RTSP本oced使本alMap::GetCellsInRe成ion(const 軍St本in成& Re成ionID) const
{
    TA本本ay<軍MapCellData> Res使lt;
    
    fo本 (const 軍MapRe成ion& Re成ion : C使本本entMap.Re成ions)
    {
        if (Re成ion.Re成ionID == Re成ionID)
        {
            // 使用邊界框快速篩選
            int32 MinX = 軍Math::Max(0, 軍Math::軍loo本ToInt(Re成ion.Bo使ndin成Box.Min.X));
            int32 MaxX = 軍Math::Min(C使本本entMap.基本idth - 1, 軍Math::CeilToInt(Re成ion.Bo使ndin成Box.Max.X));
            int32 MinY = 軍Math::Max(0, 軍Math::軍loo本ToInt(Re成ion.Bo使ndin成Box.Min.Y));
            int32 MaxY = 軍Math::Min(C使本本entMap.輸入ei成ht - 1, 軍Math::CeilToInt(Re成ion.Bo使ndin成Box.Max.Y));
            
            fo本 (int32 Y = MinY; Y <= MaxY; ++Y)
            {
                fo本 (int32 X = MinX; X <= MaxX; ++X)
                {
                    軍Vecto本2D Point(X + 0.5f, Y + 0.5f);
                    // 檢查點是否在多邊形內
                    if (Re成ion.Bo使ndin成Box.IsInside(Point))
                    {
                        Res使lt.Add(GetCell(X, Y));
                    }
                }
            }
            b本eak;
        }
    }
    
    本et使本n Res使lt;
}

正oid UMin成RTSP本oced使本alMap::AddReso使本ce的ode(EMapReso使本ceType Type, 軍Vecto本2D Location, float A設置o使nt, float Q使ality)
{
    軍Reso使本ce的ode 的ode;
    的ode.的odeID = 軍G使id::的ewG使id().ToSt本in成();
    的ode.Reso使本ceType = Type;
    的ode.Location = Location;
    的ode.A設置o使nt = A設置o使nt;
    的ode.Q使ality = Q使ality;
    
    C使本本entMap.Reso使本ce的odes.Add(的ode);
    
    // 更新對應單元格的資源信息
    int32 X = 軍Math::軍loo本ToInt(Location.X);
    int32 Y = 軍Math::軍loo本ToInt(Location.Y);
    int32 Index = GetCellIndex(X, Y);
    if (Index >= 0 && Index < C使本本entMap.Cells.的使設置())
    {
        C使本本entMap.Cells[Index].Reso使本ceType = Type;
        C使本本entMap.Cells[Index].Reso使本ceA設置o使nt = A設置o使nt;
    }
}

正oid UMin成RTSP本oced使本alMap::Re設置o正eReso使本ce的ode(const 軍St本in成& 的odeID)
{
    fo本 (int32 i = 0; i < C使本本entMap.Reso使本ce的odes.的使設置(); ++i)
    {
        if (C使本本entMap.Reso使本ce的odes[i].的odeID == 的odeID)
        {
            // 清除單元格資源
            軍Vecto本2D Location = C使本本entMap.Reso使本ce的odes[i].Location;
            int32 X = 軍Math::軍loo本ToInt(Location.X);
            int32 Y = 軍Math::軍loo本ToInt(Location.Y);
            int32 Index = GetCellIndex(X, Y);
            if (Index >= 0 && Index < C使本本entMap.Cells.的使設置())
            {
                C使本本entMap.Cells[Index].Reso使本ceType = EMapReso使本ceType::的one;
                C使本本entMap.Cells[Index].Reso使本ceA設置o使nt = 0.0f;
            }
            
            C使本本entMap.Reso使本ce的odes.Re設置o正eAt(i);
            b本eak;
        }
    }
}

TA本本ay<軍Reso使本ce的ode> UMin成RTSP本oced使本alMap::GetReso使本ce的odesInRadi使s(軍Vecto本2D Cente本, float Radi使s) const
{
    TA本本ay<軍Reso使本ce的ode> Res使lt;
    
    fo本 (const 軍Reso使本ce的ode& 的ode : C使本本entMap.Reso使本ce的odes)
    {
        if (GetDistance(的ode.Location, Cente本) <= Radi使s)
        {
            Res使lt.Add(的ode);
        }
    }
    
    本et使本n Res使lt;
}

軍Reso使本ce的ode UMin成RTSP本oced使本alMap::Get的ea本estReso使本ce的ode(軍Vecto本2D Location, EMapReso使本ceType Type) const
{
    軍Reso使本ce的ode 的ea本est;
    float MinDistance = 軍LT下MAX;
    
    fo本 (const 軍Reso使本ce的ode& 的ode : C使本本entMap.Reso使本ce的odes)
    {
        if (的ode.Reso使本ceType == Type  Type == EMapReso使本ceType::的one)
        {
            float Dist = GetDistance(的ode.Location, Location);
            if (Dist < MinDistance)
            {
                MinDistance = Dist;
                的ea本est = 的ode;
            }
        }
    }
    
    本et使本n 的ea本est;
}

軍MapRe成ion UMin成RTSP本oced使本alMap::GetRe成ion(const 軍St本in成& Re成ionID) const
{
    fo本 (const 軍MapRe成ion& Re成ion : C使本本entMap.Re成ions)
    {
        if (Re成ion.Re成ionID == Re成ionID)
        {
            本et使本n Re成ion;
        }
    }
    本et使本n 軍MapRe成ion();
}

軍St本in成 UMin成RTSP本oced使本alMap::GetRe成ionAtLocation(軍Vecto本2D Location) const
{
    fo本 (const 軍MapRe成ion& Re成ion : C使本本entMap.Re成ions)
    {
        if (Re成ion.Bo使ndin成Box.IsInside(Location))
        {
            本et使本n Re成ion.Re成ionID;
        }
    }
    本et使本n 軍St本in成();
}

正oid UMin成RTSP本oced使本alMap::DefineRe成ion(const TA本本ay<軍Vecto本2D>& Bo使nda本y, const 軍St本in成& Re成ion的a設置e)
{
    軍MapRe成ion Re成ion;
    Re成ion.Re成ionID = 軍G使id::的ewG使id().ToSt本in成();
    Re成ion.Re成ion的a設置e = Re成ion的a設置e;
    Re成ion.Bo使nda本yPoints = Bo使nda本y;
    
    // 計算邊界框和中心
    if (Bo使nda本y.的使設置() > 0)
    {
        軍Vecto本2D Min = Bo使nda本y[0];
        軍Vecto本2D Max = Bo使nda本y[0];
        軍Vecto本2D S使設置 = 軍Vecto本2D::Ze本oVecto本;
        
        fo本 (const 軍Vecto本2D& Point : Bo使nda本y)
        {
            Min.X = 軍Math::Min(Min.X, Point.X);
            Min.Y = 軍Math::Min(Min.Y, Point.Y);
            Max.X = 軍Math::Max(Max.X, Point.X);
            Max.Y = 軍Math::Max(Max.Y, Point.Y);
            S使設置 += Point;
        }
        
        Re成ion.Bo使ndin成Box = 軍Box2D(Min, Max);
        Re成ion.Cente本 = S使設置 / Bo使nda本y.的使設置();
        Re成ion.A本ea = (Max.X - Min.X) * (Max.Y - Min.Y);
    }
    
    // 統計區域內的地形
    TA本本ay<軍MapCellData> Re成ionCells = GetCellsInRe成ion(Re成ion.Re成ionID);
    TMap<ETe本本ainType, int32> Te本本ainCo使nts;
    
    fo本 (const 軍MapCellData& Cell : Re成ionCells)
    {
        Te本本ainCo使nts.軍indO本Add(Cell.Te本本ainType)++;
    }
    
    // 找主要地形
    int32 MaxCo使nt = 0;
    fo本 (const a使to& Pai本 : Te本本ainCo使nts)
    {
        if (Pai本.Val使e > MaxCo使nt)
        {
            MaxCo使nt = Pai本.Val使e;
            Re成ion.Do設置inantTe本本ain = Pai本.Key;
        }
    }
    
    Re成ion.CellCo使nt = Re成ionCells.的使設置();
    
    C使本本entMap.Re成ions.Add(Re成ion);
}

TA本本ay<軍Vecto本2D> UMin成RTSP本oced使本alMap::軍indPath(軍Vecto本2D Sta本t, 軍Vecto本2D End) const
{
    本et使本n 軍indPathA正oidin成Te本本ain(Sta本t, End, TA本本ay<ETe本本ainType>());
}

TA本本ay<軍Vecto本2D> UMin成RTSP本oced使本alMap::軍indPathA正oidin成Te本本ain(軍Vecto本2D Sta本t, 軍Vecto本2D End, TA本本ay<ETe本本ainType> A正oidedTe本本ains) const
{
    TA本本ay<軍Vecto本2D> Path;
    
    // A* 路徑查找實現
    TA本本ay<軍Vecto本2D> OpenSet;
    TA本本ay<軍Vecto本2D> ClosedSet;
    TMap<軍Vecto本2D, 軍Vecto本2D> Ca設置e軍本o設置;
    TMap<軍Vecto本2D, float> GSco本e;
    TMap<軍Vecto本2D, float> 軍Sco本e;
    
    OpenSet.Add(Sta本t);
    GSco本e.Add(Sta本t, 0.0f);
    軍Sco本e.Add(Sta本t, 輸入e使本istic(Sta本t, End));
    
    while (OpenSet.的使設置() > 0)
    {
        // 找到軍值最小的節點
        軍Vecto本2D C使本本ent = OpenSet[0];
        float Min軍 = 軍Sco本e[C使本本ent];
        int32 C使本本entIndex = 0;
        
        fo本 (int32 i = 1; i < OpenSet.的使設置(); ++i)
        {
            if (軍Sco本e[OpenSet[i]] < Min軍)
            {
                Min軍 = 軍Sco本e[OpenSet[i]];
                C使本本ent = OpenSet[i];
                C使本本entIndex = i;
            }
        }
        
        if (GetDistance(C使本本ent, End) < 1.0f)
        {
            Path = Reconst本使ctPath(Ca設置e軍本o設置, C使本本ent);
            Path.Add(End);
            本et使本n Path;
        }
        
        OpenSet.Re設置o正eAt(C使本本entIndex);
        ClosedSet.Add(C使本本ent);
        
        // 檢查鄰居
        const int32 的ei成hbo本s[8][2] = {{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}};
        
        fo本 (int32 i = 0; i < 8; ++i)
        {
            軍Vecto本2D 的ei成hbo本(C使本本ent.X + 的ei成hbo本s[i][0], C使本本ent.Y + 的ei成hbo本s[i][1]);
            
            if (!IsValidCell(的ei成hbo本.X, 的ei成hbo本.Y))
            {
                contin使e;
            }
            
            if (ClosedSet.Contains(的ei成hbo本))
            {
                contin使e;
            }
            
            軍MapCellData Cell = GetCell(的ei成hbo本.X, 的ei成hbo本.Y);
            
            // 檢查是否應該避開
            if (A正oidedTe本本ains.Contains(Cell.Te本本ainType))
            {
                contin使e;
            }
            
            if (!Cell.b基本alkable)
            {
                contin使e;
            }
            
            float Tentati正eGSco本e = GSco本e[C使本本ent] + Cell.Mo正e設置entCost;
            
            if (!OpenSet.Contains(的ei成hbo本))
            {
                OpenSet.Add(的ei成hbo本);
            }
            else if (Tentati正eGSco本e >= GSco本e[的ei成hbo本])
            {
                contin使e;
            }
            
            Ca設置e軍本o設置.Add(的ei成hbo本, C使本本ent);
            GSco本e.Add(的ei成hbo本, Tentati正eGSco本e);
            軍Sco本e.Add(的ei成hbo本, GSco本e[的ei成hbo本] + 輸入e使本istic(的ei成hbo本, End));
        }
    }
    
    本et使本n Path; // 返回空路徑表示未找到
}

bool UMin成RTSP本oced使本alMap::IsPathClea本(軍Vecto本2D Sta本t, 軍Vecto本2D End) const
{
    TA本本ay<軍Vecto本2D> Path = 軍indPath(Sta本t, End);
    本et使本n Path.的使設置() > 0;
}

軍MapAnalysisRes使lt UMin成RTSP本oced使本alMap::AnalyzeMap() const
{
    軍MapAnalysisRes使lt Res使lt;
    
    // 地形分佈
    int32 TotalCells = C使本本entMap.Cells.的使設置();
    fo本 (const 軍MapCellData& Cell : C使本本entMap.Cells)
    {
        Res使lt.Te本本ainDist本ib使tion.軍indO本Add(Cell.Te本本ainType)++;
    }
    
    // 轉換為百分比
    fo本 (a使to& Pai本 : Res使lt.Te本本ainDist本ib使tion)
    {
        Pai本.Val使e /= TotalCells;
    }
    
    // 資源統計
    fo本 (const 軍Reso使本ce的ode& 的ode : C使本本entMap.Reso使本ce的odes)
    {
        Res使lt.Reso使本ceCo使nts.軍indO本Add(的ode.Reso使本ceType)++;
    }
    
    // 高度統計
    float S使設置輸入ei成ht = 0.0f;
    fo本 (const 軍MapCellData& Cell : C使本本entMap.Cells)
    {
        S使設置輸入ei成ht += Cell.輸入ei成ht;
    }
    Res使lt.A正e本a成e輸入ei成ht = S使設置輸入ei成ht / TotalCells;
    
    float Va本ianceS使設置 = 0.0f;
    fo本 (const 軍MapCellData& Cell : C使本本entMap.Cells)
    {
        Va本ianceS使設置 += 軍Math::Sq使a本e(Cell.輸入ei成ht - Res使lt.A正e本a成e輸入ei成ht);
    }
    Res使lt.輸入ei成htVa本iance = Va本ianceS使設置 / TotalCells;
    
    // 戰略價值評估
    Res使lt.St本ate成icPoints = 軍indSt本ate成icPoints();
    Res使lt.ChokePoints = 軍indChokePoints();
    
    // 平衡性評估
    Res使lt.BalanceSco本e = Calc使lateMapBalance();
    Res使lt.St本ate成icVal使e = Res使lt.St本ate成icPoints.的使設置() / 10.0f; // 簡化計算
    
    本et使本n Res使lt;
}

float UMin成RTSP本oced使本alMap::Calc使lateMapBalance() const
{
    // 檢查各種地形和資源的分佈是否均衡
    float Balance = 1.0f;
    
    // 檢查起始位置資源公平性
    // 這裡簡化為檢查資源總量
    float TotalReso使本ces = 0.0f;
    fo本 (const 軍Reso使本ce的ode& 的ode : C使本本entMap.Reso使本ce的odes)
    {
        TotalReso使本ces += 的ode.A設置o使nt * 的ode.Q使ality;
    }
    
    // 假設理想資源數量
    float IdealReso使本ces = C使本本entMap.基本idth * C使本本entMap.輸入ei成ht * 0.001f;
    float Reso使本ceRatio = TotalReso使本ces / IdealReso使本ces;
    
    if (Reso使本ceRatio > 1.5f  Reso使本ceRatio < 0.5f)
    {
        Balance -= 0.2f;
    }
    
    // 檢查地形多樣性
    if (C使本本entMap.Re成ions.的使設置() < 3)
    {
        Balance -= 0.2f;
    }
    
    本et使本n 軍Math::Cla設置p(Balance, 0.0f, 1.0f);
}

TA本本ay<軍Vecto本2D> UMin成RTSP本oced使本alMap::軍indSt本ate成icPoints() const
{
    TA本本ay<軍Vecto本2D> St本ate成icPoints;
    
    // 尋找高地形點和資源集中區域
    fo本 (int32 Y = 5; Y < C使本本entMap.輸入ei成ht - 5; Y += 10)
    {
        fo本 (int32 X = 5; X < C使本本entMap.基本idth - 5; X += 10)
        {
            軍MapCellData Cell = GetCell(X, Y);
            
            // 高地
            if (Cell.輸入ei成ht > C使本本entMap.Max輸入ei成ht * 0.8f)
            {
                St本ate成icPoints.Add(軍Vecto本2D(X + 0.5f, Y + 0.5f));
            }
            // 資源豐富區域
            else if (Cell.Reso使本ceType != EMapReso使本ceType::的one && Cell.Reso使本ceA設置o使nt > 200.0f)
            {
                St本ate成icPoints.Add(軍Vecto本2D(X + 0.5f, Y + 0.5f));
            }
        }
    }
    
    本et使本n St本ate成icPoints;
}

TA本本ay<軍Vecto本2D> UMin成RTSP本oced使本alMap::軍indChokePoints() const
{
    TA本本ay<軍Vecto本2D> ChokePoints;
    
    // 尋找狹窄通道
    fo本 (int32 Y = 1; Y < C使本本entMap.輸入ei成ht - 1; ++Y)
    {
        fo本 (int32 X = 1; X < C使本本entMap.基本idth - 1; ++X)
        {
            軍MapCellData Cell = GetCell(X, Y);
            
            if (!Cell.b基本alkable)
            {
                contin使e;
            }
            
            // 計算可通行的鄰居數量
            int32 基本alkable的ei成hbo本s = 0;
            const int32 的ei成hbo本s[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
            
            fo本 (int32 i = 0; i < 4; ++i)
            {
                軍MapCellData 的ei成hbo本 = GetCell(X + 的ei成hbo本s[i][0], Y + 的ei成hbo本s[i][1]);
                if (的ei成hbo本.b基本alkable)
                {
                    基本alkable的ei成hbo本s++;
                }
            }
            
            // 如果只有2個可通行方向，可能是瓶頸
            if (基本alkable的ei成hbo本s == 2)
            {
                ChokePoints.Add(軍Vecto本2D(X + 0.5f, Y + 0.5f));
            }
        }
    }
    
    本et使本n ChokePoints;
}

bool UMin成RTSP本oced使本alMap::ValidateMap() const
{
    // 基本驗證
    if (C使本本entMap.基本idth == 0  C使本本entMap.輸入ei成ht == 0)
    {
        本et使本n false;
    }
    
    if (C使本本entMap.Cells.的使設置() != C使本本entMap.基本idth * C使本本entMap.輸入ei成ht)
    {
        本et使本n false;
    }
    
    // 檢查是否有足夠的行走空間
    int32 基本alkableCo使nt = 0;
    fo本 (const 軍MapCellData& Cell : C使本本entMap.Cells)
    {
        if (Cell.b基本alkable)
        {
            基本alkableCo使nt++;
        }
    }
    
    // 至少30%的地圖應該可行走
    float 基本alkableRatio = static下cast<float>(基本alkableCo使nt) / C使本本entMap.Cells.的使設置();
    if (基本alkableRatio < 0.3f)
    {
        本et使本n false;
    }
    
    // 檢查是否有足夠的資源
    if (C使本本entMap.Reso使本ce的odes.的使設置() < 5)
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

bool UMin成RTSP本oced使本alMap::Sa正eMap(const 軍St本in成& 軍ilePath) const
{
    // 序列化地圖數據
    Sa正edMaps.Add(C使本本entMap.MapID, C使本本entMap);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSP本oced使本alMap: Sa正ed 設置ap %s to %s"), *C使本本entMap.MapID, *軍ilePath);
    本et使本n t本使e;
}

bool UMin成RTSP本oced使本alMap::LoadMap(const 軍St本in成& 軍ilePath)
{
    // 反序列化地圖數據
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSP本oced使本alMap: Loadin成 設置ap f本o設置 %s"), *軍ilePath);
    本et使本n false; // 簡化實現
}

bool UMin成RTSP本oced使本alMap::Expo本tToText使本e(const 軍St本in成& 軍ilePath, EMapReso使本ceType Reso使本ceToVis使alize) const
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成RTSP本oced使本alMap: Expo本tin成 設置ap to text使本e %s"), *軍ilePath);
    本et使本n t本使e; // 簡化實現
}

正oid UMin成RTSP本oced使本alMap::Apply輸入ei成ht設置ap(const TA本本ay<float>& 輸入ei成htData)
{
    if (輸入ei成htData.的使設置() != C使本本entMap.Cells.的使設置())
    {
        本et使本n;
    }
    
    fo本 (int32 i = 0; i < C使本本entMap.Cells.的使設置(); ++i)
    {
        C使本本entMap.Cells[i].輸入ei成ht = 輸入ei成htData[i];
        C使本本entMap.Min輸入ei成ht = 軍Math::Min(C使本本entMap.Min輸入ei成ht, 輸入ei成htData[i]);
        C使本本entMap.Max輸入ei成ht = 軍Math::Max(C使本本entMap.Max輸入ei成ht, 輸入ei成htData[i]);
    }
}

正oid UMin成RTSP本oced使本alMap::S設置oothTe本本ain(int32 Ite本ations)
{
    fo本 (int32 Ite本 = 0; Ite本 < Ite本ations; ++Ite本)
    {
        TA本本ay<float> 的ew輸入ei成hts;
        的ew輸入ei成hts.Set的使設置(C使本本entMap.Cells.的使設置());
        
        fo本 (int32 Y = 0; Y < C使本本entMap.輸入ei成ht; ++Y)
        {
            fo本 (int32 X = 0; X < C使本本entMap.基本idth; ++X)
            {
                float S使設置 = GetCell(X, Y).輸入ei成ht;
                int32 Co使nt = 1;
                
                // 平均鄰居高度
                const int32 的ei成hbo本s[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
                fo本 (int32 i = 0; i < 4; ++i)
                {
                    int32 的X = X + 的ei成hbo本s[i][0];
                    int32 的Y = Y + 的ei成hbo本s[i][1];
                    if (IsValidCell(的X, 的Y))
                    {
                        S使設置 += GetCell(的X, 的Y).輸入ei成ht;
                        Co使nt++;
                    }
                }
                
                的ew輸入ei成hts[GetCellIndex(X, Y)] = S使設置 / Co使nt;
            }
        }
        
        // 應用新高度
        fo本 (int32 i = 0; i < C使本本entMap.Cells.的使設置(); ++i)
        {
            C使本本entMap.Cells[i].輸入ei成ht = 的ew輸入ei成hts[i];
        }
    }
}

正oid UMin成RTSP本oced使本alMap::Add的oise(float A設置plit使de, float 軍本eq使ency)
{
    fo本 (int32 Y = 0; Y < C使本本entMap.輸入ei成ht; ++Y)
    {
        fo本 (int32 X = 0; X < C使本本entMap.基本idth; ++X)
        {
            int32 Index = GetCellIndex(X, Y);
            float 的oise = Pe本lin的oise(X * 軍本eq使ency, Y * 軍本eq使ency) * A設置plit使de;
            C使本本entMap.Cells[Index].輸入ei成ht += 的oise;
        }
    }
}

正oid UMin成RTSP本oced使本alMap::E本odeTe本本ain(int32 Ite本ations)
{
    fo本 (int32 Ite本 = 0; Ite本 < Ite本ations; ++Ite本)
    {
        fo本 (int32 Y = 1; Y < C使本本entMap.輸入ei成ht - 1; ++Y)
        {
            fo本 (int32 X = 1; X < C使本本entMap.基本idth - 1; ++X)
            {
                int32 Index = GetCellIndex(X, Y);
                float C使本本ent輸入ei成ht = C使本本entMap.Cells[Index].輸入ei成ht;
                
                // 找到最低的鄰居
                float Min的ei成hbo本輸入ei成ht = C使本本ent輸入ei成ht;
                const int32 的ei成hbo本s[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
                
                fo本 (int32 i = 0; i < 4; ++i)
                {
                    float 的輸入 = GetCell(X + 的ei成hbo本s[i][0], Y + 的ei成hbo本s[i][1]).輸入ei成ht;
                    Min的ei成hbo本輸入ei成ht = 軍Math::Min(Min的ei成hbo本輸入ei成ht, 的輸入);
                }
                
                // 侵蝕
                float Diff = C使本本ent輸入ei成ht - Min的ei成hbo本輸入ei成ht;
                if (Diff > 0.01f)
                {
                    C使本本entMap.Cells[Index].輸入ei成ht -= Diff * 0.1f;
                }
            }
        }
    }
}

// 生成步驟實現
正oid UMin成RTSP本oced使本alMap::Gene本ate輸入ei成ht設置ap()
{
    C使本本entMap.Cells.Set的使設置(C使本本entMap.基本idth * C使本本entMap.輸入ei成ht);
    
    // 使用分形布朗運動生成地形
    fo本 (int32 Y = 0; Y < C使本本entMap.輸入ei成ht; ++Y)
    {
        fo本 (int32 X = 0; X < C使本本entMap.基本idth; ++X)
        {
            float Xf = X / static下cast<float>(C使本本entMap.基本idth);
            float Yf = Y / static下cast<float>(C使本本entMap.輸入ei成ht);
            
            float 輸入ei成ht = 軍本actalB本ownianMotion(Xf * 4.0f, Yf * 4.0f, 6);
            
            // 添加大型地形特徵
            輸入ei成ht += Pe本lin的oise(Xf * 1.0f, Yf * 1.0f) * 0.5f;
            
            // 歸一化
            輸入ei成ht = (輸入ei成ht + 1.0f) * 0.5f;
            
            int32 Index = GetCellIndex(X, Y);
            C使本本entMap.Cells[Index].X = X;
            C使本本entMap.Cells[Index].Y = Y;
            C使本本entMap.Cells[Index].輸入ei成ht = 輸入ei成ht * C使本本entConfi成.Mo使ntain輸入ei成ht;
            
            C使本本entMap.Min輸入ei成ht = 軍Math::Min(C使本本entMap.Min輸入ei成ht, C使本本entMap.Cells[Index].輸入ei成ht);
            C使本本entMap.Max輸入ei成ht = 軍Math::Max(C使本本entMap.Max輸入ei成ht, C使本本entMap.Cells[Index].輸入ei成ht);
        }
    }
}

正oid UMin成RTSP本oced使本alMap::ApplyCli設置ateEffects()
{
    // 基於氣候類型應用溫度和濕度
    fo本 (int32 Y = 0; Y < C使本本entMap.輸入ei成ht; ++Y)
    {
        fo本 (int32 X = 0; X < C使本本entMap.基本idth; ++X)
        {
            int32 Index = GetCellIndex(X, Y);
            軍MapCellData& Cell = C使本本entMap.Cells[Index];
            
            // 基於緯度（Y坐標）的溫度
            float Latit使de = Y / static下cast<float>(C使本本entMap.輸入ei成ht);
            Cell.Te設置pe本at使本e = 1.0f - 軍Math::Abs(Latit使de - 0.5f) * 2.0f;
            
            // 濕度噪聲
            Cell.Moist使本e = Pe本lin的oise(X * 0.01f, Y * 0.01f) * 0.5f + 0.5f;
            
            // 根據高度調整溫度
            float 輸入ei成htRatio = (Cell.輸入ei成ht - C使本本entMap.Min輸入ei成ht) / (C使本本entMap.Max輸入ei成ht - C使本本entMap.Min輸入ei成ht);
            Cell.Te設置pe本at使本e -= 輸入ei成htRatio * 0.3f;
            
            // 確定地形類型
            Cell.Te本本ainType = Dete本設置ineTe本本ain軍本o設置輸入ei成ht(輸入ei成htRatio, Cell.Moist使本e, Cell.Te設置pe本at使本e);
            Cell.Mo正e設置entCost = GetMo正e設置entCost軍o本Te本本ain(Cell.Te本本ainType);
            Cell.b基本alkable = (Cell.Te本本ainType != ETe本本ainType::基本ate本 && 
                              Cell.Te本本ainType != ETe本本ainType::Mo使ntains);
            Cell.bB使ildable = (Cell.Te本本ainType == ETe本本ainType::Plains  
                               Cell.Te本本ainType == ETe本本ainType::輸入ills);
        }
    }
}

正oid UMin成RTSP本oced使本alMap::Gene本ateRi正e本s()
{
    fo本 (int32 i = 0; i < C使本本entConfi成.Ri正e本Co使nt; ++i)
    {
        // 找到高地的起點
        軍Vecto本2D So使本ce;
        bool 軍o使ndSo使本ce = false;
        
        fo本 (int32 Atte設置pt = 0; Atte設置pt < 100; ++Atte設置pt)
        {
            int32 X = Rando設置Int(C使本本entMap.基本idth);
            int32 Y = Rando設置Int(C使本本entMap.輸入ei成ht);
            軍MapCellData Cell = GetCell(X, Y);
            
            float 輸入ei成htRatio = (Cell.輸入ei成ht - C使本本entMap.Min輸入ei成ht) / (C使本本entMap.Max輸入ei成ht - C使本本entMap.Min輸入ei成ht);
            if (輸入ei成htRatio > 0.7f)
            {
                So使本ce = 軍Vecto本2D(X, Y);
                軍o使ndSo使本ce = t本使e;
                b本eak;
            }
        }
        
        if (!軍o使ndSo使本ce)
        {
            contin使e;
        }
        
        // 生成河流路徑
        軍Ri正e本Data Ri正e本;
        Ri正e本.Ri正e本ID = 軍G使id::的ewG使id().ToSt本in成();
        Ri正e本.Ri正e本的a設置e = 軍St本in成::P本intf(TEXT("Ri正e本下%d"), i);
        Ri正e本.So使本ce = So使本ce;
        Ri正e本.Path = 軍indRi正e本Path(So使本ce);
        
        if (Ri正e本.Path.的使設置() > 0)
        {
            Ri正e本.Mo使th = Ri正e本.Path.Last();
            Ri正e本.Len成th = 0.0f;
            fo本 (int32 大 = 1; 大 < Ri正e本.Path.的使設置(); ++大)
            {
                Ri正e本.Len成th += GetDistance(Ri正e本.Path[大-1], Ri正e本.Path[大]);
            }
            
            C使本本entMap.Ri正e本s.Add(Ri正e本);
            
            // 標記河流地形
            fo本 (const 軍Vecto本2D& Point : Ri正e本.Path)
            {
                int32 X = 軍Math::軍loo本ToInt(Point.X);
                int32 Y = 軍Math::軍loo本ToInt(Point.Y);
                SetCellTe本本ain(X, Y, ETe本本ainType::Ri正e本);
            }
        }
    }
}

正oid UMin成RTSP本oced使本alMap::PlaceReso使本ces()
{
    int32 Ta本成etReso使本ceCo使nt = 軍Math::軍loo本ToInt(C使本本entMap.基本idth * C使本本entMap.輸入ei成ht * C使本本entConfi成.Reso使本ceDensity * 0.001f);
    
    fo本 (int32 i = 0; i < Ta本成etReso使本ceCo使nt; ++i)
    {
        EMapReso使本ceType Type = static下cast<EMapReso使本ceType>(Rando設置Int(static下cast<int32>(EMapReso使本ceType::的one)));
        軍Vecto本2D Location = 軍indOpti設置alReso使本ceLocation(Type);
        
        if (IsValidReso使本ceLocation(Location, Type))
        {
            float A設置o使nt = 100.0f + Rando設置軍loat() * 400.0f;
            float Q使ality = 0.5f + Rando設置軍loat() * 0.5f;
            AddReso使本ce的ode(Type, Location, A設置o使nt, Q使ality);
        }
    }
}

正oid UMin成RTSP本oced使本alMap::DefineRe成ions()
{
    // 使用簡單的區域劃分
    int32 Re成ionsX = 軍Math::CeilToInt(軍Math::Sq本t(static下cast<float>(C使本本entConfi成.Re成ionCo使nt)));
    int32 Re成ionsY = 軍Math::CeilToInt(static下cast<float>(C使本本entConfi成.Re成ionCo使nt) / Re成ionsX);
    
    int32 Re成ion基本idth = C使本本entMap.基本idth / Re成ionsX;
    int32 Re成ion輸入ei成ht = C使本本entMap.輸入ei成ht / Re成ionsY;
    
    int32 Re成ionIndex = 0;
    fo本 (int32 RY = 0; RY < Re成ionsY && Re成ionIndex < C使本本entConfi成.Re成ionCo使nt; ++RY)
    {
        fo本 (int32 RX = 0; RX < Re成ionsX && Re成ionIndex < C使本本entConfi成.Re成ionCo使nt; ++RX)
        {
            TA本本ay<軍Vecto本2D> Bo使nda本y;
            int32 MinX = RX * Re成ion基本idth;
            int32 MaxX = 軍Math::Min((RX + 1) * Re成ion基本idth, C使本本entMap.基本idth);
            int32 MinY = RY * Re成ion輸入ei成ht;
            int32 MaxY = 軍Math::Min((RY + 1) * Re成ion輸入ei成ht, C使本本entMap.輸入ei成ht);
            
            Bo使nda本y.Add(軍Vecto本2D(MinX, MinY));
            Bo使nda本y.Add(軍Vecto本2D(MaxX, MinY));
            Bo使nda本y.Add(軍Vecto本2D(MaxX, MaxY));
            Bo使nda本y.Add(軍Vecto本2D(MinX, MaxY));
            
            軍St本in成 Re成ion的a設置e = 軍St本in成::P本intf(TEXT("Re成ion下%d"), Re成ionIndex);
            DefineRe成ion(Bo使nda本y, Re成ion的a設置e);
            
            Re成ionIndex++;
        }
    }
}

正oid UMin成RTSP本oced使本alMap::Gene本ateRoads()
{
    // 在區域中心之間生成道路
    fo本 (int32 i = 0; i < C使本本entMap.Re成ions.的使設置(); ++i)
    {
        fo本 (int32 大 = i + 1; 大 < C使本本entMap.Re成ions.的使設置(); ++大)
        {
            if (Rando設置軍loat() < 0.3f) // 30%概率連接
            {
                軍Vecto本2D Sta本t = C使本本entMap.Re成ions[i].Cente本;
                軍Vecto本2D End = C使本本entMap.Re成ions[大].Cente本;
                
                TA本本ay<軍Vecto本2D> Path = 軍indPath(Sta本t, End);
                if (Path.的使設置() > 0)
                {
                    C使本本entMap.Road的etwo本ks.Add(Path);
                }
            }
        }
    }
}

正oid UMin成RTSP本oced使本alMap::PlaceSettle設置ents()
{
    // 在每個區域放置定居點
    fo本 (const 軍MapRe成ion& Re成ion : C使本本entMap.Re成ions)
    {
        // 尋找合適的位置（平坦、有資源）
        軍Vecto本2D BestLocation = Re成ion.Cente本;
        float BestSco本e = -1.0f;
        
        fo本 (int32 Atte設置pt = 0; Atte設置pt < 50; ++Atte設置pt)
        {
            float OffsetX = (Rando設置軍loat() - 0.5f) * Re成ion.Bo使ndin成Box.GetExtent().X * 2.0f;
            float OffsetY = (Rando設置軍loat() - 0.5f) * Re成ion.Bo使ndin成Box.GetExtent().Y * 2.0f;
            軍Vecto本2D Location = Re成ion.Cente本 + 軍Vecto本2D(OffsetX, OffsetY);
            
            int32 X = 軍Math::軍loo本ToInt(Location.X);
            int32 Y = 軍Math::軍loo本ToInt(Location.Y);
            
            if (!IsValidCell(X, Y))
            {
                contin使e;
            }
            
            軍MapCellData Cell = GetCell(X, Y);
            
            if (!Cell.bB使ildable)
            {
                contin使e;
            }
            
            // 評分：平坦度 + 資源接近度
            float Sco本e = 1.0f;
            Sco本e -= 軍Math::Abs(Cell.輸入ei成ht - C使本本entMap.Min輸入ei成ht) / (C使本本entMap.Max輸入ei成ht - C使本本entMap.Min輸入ei成ht);
            
            // 附近資源加成
            TA本本ay<軍Reso使本ce的ode> 的ea本byReso使本ces = GetReso使本ce的odesInRadi使s(Location, 50.0f);
            Sco本e += 的ea本byReso使本ces.的使設置() * 0.1f;
            
            if (Sco本e > BestSco本e)
            {
                BestSco本e = Sco本e;
                BestLocation = Location;
            }
        }
        
        if (BestSco本e > 0.0f)
        {
            C使本本entMap.Settle設置entLocations.Add(BestLocation);
        }
    }
}

正oid UMin成RTSP本oced使本alMap::軍inalizeMap()
{
    // 計算最終統計數據
    float LandA本ea = 0.0f;
    float 基本ate本A本ea = 0.0f;
    
    fo本 (const 軍MapCellData& Cell : C使本本entMap.Cells)
    {
        if (Cell.Te本本ainType == ETe本本ainType::基本ate本  Cell.Te本本ainType == ETe本本ainType::Ri正e本)
        {
            基本ate本A本ea += 1.0f;
        }
        else
        {
            LandA本ea += 1.0f;
        }
    }
    
    C使本本entMap.TotalLandA本ea = LandA本ea;
    C使本本entMap.Total基本ate本A本ea = 基本ate本A本ea;
}

// 模板配置
軍MapGene本ationConfi成 UMin成RTSP本oced使本alMap::GetPlainsTe設置plate() const
{
    軍MapGene本ationConfi成 Confi成;
    Confi成.Te本本ainRo使成hness = 0.2f;
    Confi成.基本ate本Le正el = 0.2f;
    Confi成.軍o本estDensity = 0.1f;
    Confi成.Mo使ntain輸入ei成ht = 500.0f;
    本et使本n Confi成;
}

軍MapGene本ationConfi成 UMin成RTSP本oced使本alMap::GetMo使ntainTe設置plate() const
{
    軍MapGene本ationConfi成 Confi成;
    Confi成.Te本本ainRo使成hness = 0.8f;
    Confi成.基本ate本Le正el = 0.3f;
    Confi成.軍o本estDensity = 0.3f;
    Confi成.Mo使ntain輸入ei成ht = 2000.0f;
    Confi成.Ri正e本Co使nt = 5;
    本et使本n Confi成;
}

軍MapGene本ationConfi成 UMin成RTSP本oced使本alMap::GetIslandTe設置plate() const
{
    軍MapGene本ationConfi成 Confi成;
    Confi成.Te本本ainRo使成hness = 0.4f;
    Confi成.基本ate本Le正el = 0.5f;
    Confi成.軍o本estDensity = 0.5f;
    Confi成.Cli設置ate = ECli設置ateType::T本opical;
    本et使本n Confi成;
}

軍MapGene本ationConfi成 UMin成RTSP本oced使本alMap::GetDese本tTe設置plate() const
{
    軍MapGene本ationConfi成 Confi成;
    Confi成.Cli設置ate = ECli設置ateType::A本id;
    Confi成.Te本本ainRo使成hness = 0.3f;
    Confi成.基本ate本Le正el = 0.15f;
    Confi成.軍o本estDensity = 0.05f;
    Confi成.Reso使本ceDensity = 0.8f; // 更多礦產資源
    本et使本n Confi成;
}

軍MapGene本ationConfi成 UMin成RTSP本oced使本alMap::GetRi正e本DeltaTe設置plate() const
{
    軍MapGene本ationConfi成 Confi成;
    Confi成.Te本本ainRo使成hness = 0.3f;
    Confi成.基本ate本Le正el = 0.4f;
    Confi成.軍o本estDensity = 0.6f;
    Confi成.Ri正e本Co使nt = 6;
    Confi成.Cli設置ate = ECli設置ateType::Monsoon;
    本et使本n Confi成;
}

// 噪聲函數
float UMin成RTSP本oced使本alMap::Pe本lin的oise(float X, float Y) const
{
    // 簡化實現 - 實際應使用標準Pe本lin噪聲算法
    float Val使e = 軍Math::Sin(X * 12.9898f + Y * 78.233f + C使本本entSeed) * 43758.5453f;
    Val使e = 軍Math::軍本actional(Val使e);
    本et使本n Val使e * 2.0f - 1.0f;
}

float UMin成RTSP本oced使本alMap::軍本actalB本ownianMotion(float X, float Y, int32 Octa正es) const
{
    float Total = 0.0f;
    float 軍本eq使ency = 1.0f;
    float A設置plit使de = 1.0f;
    float MaxVal使e = 0.0f;
    
    fo本 (int32 i = 0; i < Octa正es; ++i)
    {
        Total += Pe本lin的oise(X * 軍本eq使ency, Y * 軍本eq使ency) * A設置plit使de;
        MaxVal使e += A設置plit使de;
        A設置plit使de *= 0.5f;
        軍本eq使ency *= 2.0f;
    }
    
    本et使本n Total / MaxVal使e;
}

float UMin成RTSP本oced使本alMap::Rid成edM使ltif本actal(float X, float Y, int32 Octa正es) const
{
    float Si成nal = Pe本lin的oise(X, Y);
    float Total = 1.0f - 軍Math::Abs(Si成nal);
    float 基本ei成ht = Total;
    
    fo本 (int32 i = 1; i < Octa正es; ++i)
    {
        X *= 2.0f;
        Y *= 2.0f;
        基本ei成ht = 軍Math::Cla設置p(Total * 2.0f, 0.0f, 1.0f);
        Si成nal = Pe本lin的oise(X, Y);
        Total += (1.0f - 軍Math::Abs(Si成nal)) * 基本ei成ht * 0.5f;
    }
    
    本et使本n Total;
}

// 輔助函數
int32 UMin成RTSP本oced使本alMap::GetCellIndex(int32 X, int32 Y) const
{
    if (X < 0  X >= C使本本entMap.基本idth  Y < 0  Y >= C使本本entMap.輸入ei成ht)
    {
        本et使本n -1;
    }
    本et使本n Y * C使本本entMap.基本idth + X;
}

bool UMin成RTSP本oced使本alMap::IsValidCell(int32 X, int32 Y) const
{
    本et使本n X >= 0 && X < C使本本entMap.基本idth && Y >= 0 && Y < C使本本entMap.輸入ei成ht;
}

float UMin成RTSP本oced使本alMap::GetDistance(軍Vecto本2D A, 軍Vecto本2D B) const
{
    本et使本n 軍Math::Sq本t(軍Math::Sq使a本e(A.X - B.X) + 軍Math::Sq使a本e(A.Y - B.Y));
}

ETe本本ainType UMin成RTSP本oced使本alMap::Dete本設置ineTe本本ain軍本o設置輸入ei成ht(float 輸入ei成ht, float Moist使本e, float Te設置pe本at使本e) const
{
    if (輸入ei成ht < C使本本entConfi成.基本ate本Le正el)
    {
        本et使本n ETe本本ainType::基本ate本;
    }
    else if (輸入ei成ht < C使本本entConfi成.基本ate本Le正el + 0.05f)
    {
        本et使本n ETe本本ainType::Coast;
    }
    else if (輸入ei成ht > 0.8f)
    {
        本et使本n Te設置pe本at使本e < 0.3f 基本 ETe本本ainType::Snow : ETe本本ainType::Mo使ntains;
    }
    else if (輸入ei成ht > 0.6f)
    {
        本et使本n ETe本本ainType::輸入ills;
    }
    else if (Te設置pe本at使本e > 0.7f && Moist使本e > 0.5f)
    {
        本et使本n ETe本本ainType::J使n成le;
    }
    else if (Moist使本e > C使本本entConfi成.軍o本estDensity)
    {
        本et使本n ETe本本ainType::軍o本est;
    }
    else if (Moist使本e < 0.2f)
    {
        本et使本n ETe本本ainType::Dese本t;
    }
    else if (Moist使本e > 0.8f && Te設置pe本at使本e > 0.5f)
    {
        本et使本n ETe本本ainType::Swa設置p;
    }
    
    本et使本n ETe本本ainType::Plains;
}

float UMin成RTSP本oced使本alMap::GetMo正e設置entCost軍o本Te本本ain(ETe本本ainType Te本本ain) const
{
    switch (Te本本ain)
    {
    case ETe本本ainType::Plains: 本et使本n 1.0f;
    case ETe本本ainType::輸入ills: 本et使本n 1.5f;
    case ETe本本ainType::Mo使ntains: 本et使本n 3.0f;
    case ETe本本ainType::軍o本est: 本et使本n 1.3f;
    case ETe本本ainType::Dese本t: 本et使本n 1.2f;
    case ETe本本ainType::Swa設置p: 本et使本n 2.0f;
    case ETe本本ainType::基本ate本: 本et使本n 5.0f;
    case ETe本本ainType::Ri正e本: 本et使本n 2.5f;
    case ETe本本ainType::Coast: 本et使本n 1.1f;
    case ETe本本ainType::Canyon: 本et使本n 2.5f;
    case ETe本本ainType::Platea使: 本et使本n 1.4f;
    case ETe本本ainType::Volcanic: 本et使本n 2.0f;
    case ETe本本ainType::Snow: 本et使本n 2.5f;
    case ETe本本ainType::J使n成le: 本et使本n 1.8f;
    defa使lt: 本et使本n 1.0f;
    }
}

// 河流生成輔助
TA本本ay<軍Vecto本2D> UMin成RTSP本oced使本alMap::軍indRi正e本Path(軍Vecto本2D Sta本t) const
{
    TA本本ay<軍Vecto本2D> Path;
    軍Vecto本2D C使本本ent = Sta本t;
    Path.Add(C使本本ent);
    
    fo本 (int32 Step = 0; Step < 1000; ++Step)
    {
        float C使本本ent輸入ei成ht = GetCellAtLocation(C使本本ent).輸入ei成ht;
        
        // 找到最低的鄰居
        軍Vecto本2D Lowest的ei成hbo本 = C使本本ent;
        float Lowest輸入ei成ht = C使本本ent輸入ei成ht;
        
        const int32 的ei成hbo本s[8][2] = {{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}};
        
        fo本 (int32 i = 0; i < 8; ++i)
        {
            軍Vecto本2D 的ei成hbo本(C使本本ent.X + 的ei成hbo本s[i][0], C使本本ent.Y + 的ei成hbo本s[i][1]);
            
            if (!IsValidCell(的ei成hbo本.X, 的ei成hbo本.Y))
            {
                contin使e;
            }
            
            float 的ei成hbo本輸入ei成ht = GetCell(的ei成hbo本.X, 的ei成hbo本.Y).輸入ei成ht;
            if (的ei成hbo本輸入ei成ht < Lowest輸入ei成ht)
            {
                Lowest輸入ei成ht = 的ei成hbo本輸入ei成ht;
                Lowest的ei成hbo本 = 的ei成hbo本;
            }
        }
        
        // 如果沒有更低的地方，或者到達水域，停止
        if (Lowest的ei成hbo本 == C使本本ent  GetCell(Lowest的ei成hbo本.X, Lowest的ei成hbo本.Y).Te本本ainType == ETe本本ainType::基本ate本)
        {
            b本eak;
        }
        
        C使本本ent = Lowest的ei成hbo本;
        Path.Add(C使本本ent);
    }
    
    本et使本n Path;
}

// A*輔助函數
float UMin成RTSP本oced使本alMap::輸入e使本istic(軍Vecto本2D A, 軍Vecto本2D B) const
{
    本et使本n GetDistance(A, B);
}

TA本本ay<軍Vecto本2D> UMin成RTSP本oced使本alMap::Reconst本使ctPath(TMap<軍Vecto本2D, 軍Vecto本2D>& Ca設置e軍本o設置, 軍Vecto本2D C使本本ent) const
{
    TA本本ay<軍Vecto本2D> TotalPath;
    TotalPath.Add(C使本本ent);
    
    while (Ca設置e軍本o設置.Contains(C使本本ent))
    {
        C使本本ent = Ca設置e軍本o設置[C使本本ent];
        TotalPath.Inse本t(C使本本ent, 0);
    }
    
    本et使本n TotalPath;
}

// 資源生成輔助
軍Vecto本2D UMin成RTSP本oced使本alMap::軍indOpti設置alReso使本ceLocation(EMapReso使本ceType Type) const
{
    // 根據資源類型選擇合適的位置
    fo本 (int32 Atte設置pt = 0; Atte設置pt < 100; ++Atte設置pt)
    {
        軍Vecto本2D Location(Rando設置軍loat() * C使本本entMap.基本idth, Rando設置軍loat() * C使本本entMap.輸入ei成ht);
        
        if (IsValidReso使本ceLocation(Location, Type))
        {
            本et使本n Location;
        }
    }
    
    本et使本n 軍Vecto本2D(C使本本entMap.基本idth / 2.0f, C使本本entMap.輸入ei成ht / 2.0f);
}

bool UMin成RTSP本oced使本alMap::IsValidReso使本ceLocation(軍Vecto本2D Location, EMapReso使本ceType Type) const
{
    int32 X = 軍Math::軍loo本ToInt(Location.X);
    int32 Y = 軍Math::軍loo本ToInt(Location.Y);
    
    if (!IsValidCell(X, Y))
    {
        本et使本n false;
    }
    
    軍MapCellData Cell = GetCell(X, Y);
    
    // 檢查地形適合性
    switch (Type)
    {
    case EMapReso使本ceType::基本ood:
        本et使本n Cell.Te本本ainType == ETe本本ainType::軍o本est  Cell.Te本本ainType == ETe本本ainType::J使n成le;
    case EMapReso使本ceType::Stone:
    case EMapReso使本ceType::I本on:
        本et使本n Cell.Te本本ainType == ETe本本ainType::Mo使ntains  Cell.Te本本ainType == ETe本本ainType::輸入ills;
    case EMapReso使本ceType::Gold:
        本et使本n Cell.Te本本ainType == ETe本本ainType::Mo使ntains;
    case EMapReso使本ceType::基本ate本:
        本et使本n Cell.Te本本ainType == ETe本本ainType::基本ate本  Cell.Te本本ainType == ETe本本ainType::Ri正e本;
    case EMapReso使本ceType::軍ood:
        本et使本n Cell.Te本本ainType == ETe本本ainType::Plains  Cell.Te本本ainType == ETe本本ainType::Coast;
    defa使lt:
        本et使本n Cell.bB使ildable;
    }
}

// 隨機數生成
float UMin成RTSP本oced使本alMap::Rando設置軍loat() const
{
    // 簡化實現
    本et使本n 軍Math::軍Rand();
}

int32 UMin成RTSP本oced使本alMap::Rando設置Int(int32 Max) const
{
    本et使本n 軍Math::Rand() % Max;
}

軍Vecto本2D UMin成RTSP本oced使本alMap::Rando設置Point() const
{
    本et使本n 軍Vecto本2D(Rando設置軍loat() * C使本本entMap.基本idth, Rando設置軍loat() * C使本本entMap.輸入ei成ht);
}
