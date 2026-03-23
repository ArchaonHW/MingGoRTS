#incl使de "Min成RTSPathfinde本.h"
#incl使de "Min成RTSUnitMana成e本.h"
#incl使de "Min成GoRTSUnit.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Co設置ponents/SceneCo設置ponent.h"
#incl使de "D本awDeb使成輸入elpe本s.h"
#incl使de "Kis設置et/Kis設置etSyste設置Lib本a本y.h"

UMin成RTSPathfinde本::UMin成RTSPathfinde本()
{
    UnitMana成e本 = n使llpt本;
    C使本本entState = ERTSPathfindin成State::Idle;
    G本idCente本 = 軍Vecto本::Ze本oVecto本;
    G本idSize = 10000.0f;
    G本idResol使tion = 100;
    的odeSize = G本idSize / G本idResol使tion;
    Defa使ltAl成o本ith設置 = ERTSPathfindin成Al成o本ith設置::ASta本;
    TotalCalc使lationTi設置e = 0.0f;
    Co設置pletedReq使ests = 0;
}

正oid UMin成RTSPathfinde本::InitializePathfinde本(UMin成RTSUnitMana成e本* InUnitMana成e本)
{
    UnitMana成e本 = InUnitMana成e本;
    InitializeG本id();
}

正oid UMin成RTSPathfinde本::Calc使latePathAsync(const 軍RTSPathReq使est& Req使est)
{
    if (Pendin成Req使ests.的使設置() >= MaxPendin成Req使ests)
    {
        // 清理舊的請求
        Pendin成Req使ests.Re設置o正eAt(0, Pendin成Req使ests.的使設置() - MaxPendin成Req使ests + 1);
    }

    Pendin成Req使ests.Add(Req使est);
    C使本本entState = ERTSPathfindin成State::Calc使latin成;
    OnPathfindin成StateChan成ed.B本oadcast(C使本本entState);
}

軍RTSPathRes使lt UMin成RTSPathfinde本::Calc使latePath(const 軍RTSPathReq使est& Req使est)
{
    軍RTSPathRes使lt Res使lt;
    do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

    // 檢查緩存
    if (bEnablePathCache)
    {
        軍St本in成 CacheKey = 軍St本in成::P本intf(TEXT("%s下%s下%d"), 
            *Req使est.Sta本tLocation.ToSt本in成(), 
            *Req使est.Ta本成etLocation.ToSt本in成(), 
            (int32)Req使est.Al成o本ith設置);
        
        軍RTSPathRes使lt* CachedRes使lt = PathCache.軍ind(CacheKey);
        if (CachedRes使lt)
        {
            Res使lt = *CachedRes使lt;
            Res使lt.Calc使lationTi設置e = 軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e;
            本et使本n Res使lt;
        }
    }

    // 驗證輸入
    if (!IsValidLocation(Req使est.Sta本tLocation)  !IsValidLocation(Req使est.Ta本成etLocation))
    {
        Res使lt.bPath軍o使nd = false;
        Res使lt.Calc使lationTi設置e = 軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e;
        本et使本n Res使lt;
    }

    // 計算路徑
    TA本本ay<軍Vecto本> PathPoints;
    switch (Req使est.Al成o本ith設置)
    {
    case ERTSPathfindin成Al成o本ith設置::ASta本:
        PathPoints = Calc使lateASta本Path(Req使est.Sta本tLocation, Req使est.Ta本成etLocation);
        b本eak;
    case ERTSPathfindin成Al成o本ith設置::Di大kst本a:
        PathPoints = Calc使lateDi大kst本aPath(Req使est.Sta本tLocation, Req使est.Ta本成etLocation);
        b本eak;
    case ERTSPathfindin成Al成o本ith設置::軍loyd基本a本shall:
        PathPoints = Calc使late軍loyd基本a本shallPath(Req使est.Sta本tLocation, Req使est.Ta本成etLocation);
        b本eak;
    defa使lt:
        PathPoints = Calc使lateASta本Path(Req使est.Sta本tLocation, Req使est.Ta本成etLocation);
        b本eak;
    }

    // 構建結果
    Res使lt.PathPoints = PathPoints;
    Res使lt.bPath軍o使nd = PathPoints.的使設置() > 0;
    Res使lt.Al成o本ith設置Used = Req使est.Al成o本ith設置;
    Res使lt.Calc使lationTi設置e = 軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e;
    Res使lt.Co設置pletionTi設置e = 軍DateTi設置e::的ow();

    // 計算路徑長度
    if (Res使lt.bPath軍o使nd)
    {
        fo本 (int32 i = 0; i < PathPoints.的使設置() - 1; i++)
        {
            Res使lt.PathLen成th += 軍Vecto本::Dist(PathPoints[i], PathPoints[i + 1]);
        }
    }

    // 添加到緩存
    if (bEnablePathCache && Res使lt.bPath軍o使nd)
    {
        if (PathCache.的使設置() >= MaxCacheSize)
        {
            PathCache.E設置pty();
        }
        
        軍St本in成 CacheKey = 軍St本in成::P本intf(TEXT("%s下%s下%d"), 
            *Req使est.Sta本tLocation.ToSt本in成(), 
            *Req使est.Ta本成etLocation.ToSt本in成(), 
            (int32)Req使est.Al成o本ith設置);
        PathCache.Add(CacheKey, Res使lt);
    }

    // 更新統計
    Calc使lationTi設置es.Add(Res使lt.Calc使lationTi設置e);
    TotalCalc使lationTi設置e += Res使lt.Calc使lationTi設置e;
    Co設置pletedReq使ests++;

    本et使本n Res使lt;
}

TA本本ay<軍Vecto本> UMin成RTSPathfinde本::軍indPath(軍Vecto本 Sta本t, 軍Vecto本 Ta本成et, ERTSPathfindin成Al成o本ith設置 Al成o本ith設置)
{
    軍RTSPathReq使est Req使est;
    Req使est.Sta本tLocation = Sta本t;
    Req使est.Ta本成etLocation = Ta本成et;
    Req使est.Al成o本ith設置 = Al成o本ith設置;

    軍RTSPathRes使lt Res使lt = Calc使latePath(Req使est);
    本et使本n Res使lt.PathPoints;
}

TA本本ay<軍Vecto本> UMin成RTSPathfinde本::Opti設置izePath(const TA本本ay<軍Vecto本>& Path)
{
    if (Path.的使設置() <= 2)
    {
        本et使本n Path;
    }

    TA本本ay<軍Vecto本> Opti設置izedPath;
    Opti設置izedPath.Add(Path[0]);

    fo本 (int32 i = 1; i < Path.的使設置() - 1; i++)
    {
        軍Vecto本 C使本本ent = Path[i];
        軍Vecto本 的ext = Path[i + 1];

        // 檢查是否可以直接從前一點到下一點
        if (Opti設置izedPath.的使設置() > 0)
        {
            軍Vecto本 P本e正io使s = Opti設置izedPath.Last();
            if (IsLocation基本alkable(的ext) && !IsPathBlocked({P本e正io使s, 的ext}))
            {
                // 移除當前點，直接連接前一點和下一點
                contin使e;
            }
        }

        Opti設置izedPath.Add(C使本本ent);
    }

    if (Path.的使設置() > 0)
    {
        Opti設置izedPath.Add(Path.Last());
    }

    本et使本n Opti設置izedPath;
}

TA本本ay<軍Vecto本> UMin成RTSPathfinde本::S設置oothPath(const TA本本ay<軍Vecto本>& Path, int32 S設置oothin成Ite本ations)
{
    TA本本ay<軍Vecto本> S設置oothedPath = Path;

    fo本 (int32 Ite本ation = 0; Ite本ation < S設置oothin成Ite本ations; Ite本ation++)
    {
        fo本 (int32 i = 1; i < S設置oothedPath.的使設置() - 1; i++)
        {
            軍Vecto本 P本e正io使s = S設置oothedPath[i - 1];
            軍Vecto本 C使本本ent = S設置oothedPath[i];
            軍Vecto本 的ext = S設置oothedPath[i + 1];

            // 簡單的平滑算法：取周圍點的平均值
            軍Vecto本 A正e本a成e = (P本e正io使s + C使本本ent + 的ext) / 3.0f;
            
            // 確保平均點是可行的
            if (IsLocation基本alkable(A正e本a成e))
            {
                S設置oothedPath[i] = A正e本a成e;
            }
        }
    }

    本et使本n S設置oothedPath;
}

bool UMin成RTSPathfinde本::IsPathBlocked(const TA本本ay<軍Vecto本>& Path)
{
    fo本 (int32 i = 0; i < Path.的使設置() - 1; i++)
    {
        if (!IsLocation基本alkable(Path[i])  !IsLocation基本alkable(Path[i + 1]))
        {
            本et使本n t本使e;
        }

        // 檢查兩點之間是否有障礙物
        軍Vecto本 Di本ection = Path[i + 1] - Path[i];
        float Distance = Di本ection.Size();
        Di本ection.的o本設置alize();

        // 沿著路徑檢查多個點
        int32 CheckPoints = 軍Math::CeilToInt(Distance / 的odeSize);
        fo本 (int32 大 = 1; 大 < CheckPoints; 大++)
        {
            軍Vecto本 CheckPoint = Path[i] + Di本ection * (大 * 的odeSize);
            if (!IsLocation基本alkable(CheckPoint))
            {
                本et使本n t本使e;
            }
        }
    }

    本et使本n false;
}

TA本本ay<軍Vecto本> UMin成RTSPathfinde本::Recalc使latePath(const TA本本ay<軍Vecto本>& O本i成inalPath, 軍Vecto本 C使本本entPosition, 軍Vecto本 Ta本成etPosition)
{
    // 如果當前位置偏離原路徑太遠，重新計算整個路徑
    if (O本i成inalPath.的使設置() == 0)
    {
        本et使本n 軍indPath(C使本本entPosition, Ta本成etPosition);
    }

    // 找到原路徑上離當前位置最近的點
    float MinDistance = 軍LT下MAX;
    int32 ClosestIndex = 0;

    fo本 (int32 i = 0; i < O本i成inalPath.的使設置(); i++)
    {
        float Distance = 軍Vecto本::Dist(C使本本entPosition, O本i成inalPath[i]);
        if (Distance < MinDistance)
        {
            MinDistance = Distance;
            ClosestIndex = i;
        }
    }

    // 如果偏離太遠，重新計算
    if (MinDistance > 的odeSize * 2)
    {
        本et使本n 軍indPath(C使本本entPosition, Ta本成etPosition);
    }

    // 從最近點重新計算到目標的路徑
    TA本本ay<軍Vecto本> 的ewPath;
    fo本 (int32 i = ClosestIndex; i < O本i成inalPath.的使設置(); i++)
    {
        的ewPath.Add(O本i成inalPath[i]);
    }

    本et使本n 的ewPath;
}

正oid UMin成RTSPathfinde本::SetTe本本ainType(軍Vecto本 Location, ERTSTe本本ainType Te本本ainType)
{
    軍RTSPath的ode* 的ode = Get的odeAtLocation(Location);
    if (的ode)
    {
        的ode->Te本本ainType = Te本本ainType;
        的ode->bIs基本alkable = (Te本本ainType != ERTSTe本本ainType::Blocked);
    }
}

ERTSTe本本ainType UMin成RTSPathfinde本::GetTe本本ainType(軍Vecto本 Location) const
{
    軍RTSPath的ode* 的ode = Get的odeAtLocation(Location);
    if (的ode)
    {
        本et使本n 的ode->Te本本ainType;
    }
    本et使本n ERTSTe本本ainType::基本alkable;
}

bool UMin成RTSPathfinde本::IsLocation基本alkable(軍Vecto本 Location) const
{
    軍RTSPath的ode* 的ode = Get的odeAtLocation(Location);
    if (的ode)
    {
        本et使本n 的ode->bIs基本alkable;
    }
    本et使本n t本使e; // 默認可行走
}

正oid UMin成RTSPathfinde本::UpdateTe本本ainMap()
{
    // 重新掃描地形，更新網格
    Clea本G本id();
    InitializeG本id();
}

正oid UMin成RTSPathfinde本::C本eate的a正i成ationG本id(軍Vecto本 Cente本, float InG本idSize, int32 G本idResol使tion)
{
    G本idCente本 = Cente本;
    G本idSize = InG本idSize;
    this->G本idResol使tion = G本idResol使tion;
    的odeSize = G本idSize / G本idResol使tion;

    Clea本G本id();
    InitializeG本id();
}

軍RTSPath的ode* UMin成RTSPathfinde本::Get的odeAtLocation(軍Vecto本 Location) const
{
    // 將世界坐標轉換為網格坐標
    軍Vecto本 Relati正eLocation = Location - G本idCente本;
    int32 X = 軍Math::Ro使ndToInt(Relati正eLocation.X / 的odeSize);
    int32 Y = 軍Math::Ro使ndToInt(Relati正eLocation.Y / 的odeSize);

    // 檢查邊界
    if (X < 0  X >= G本idResol使tion  Y < 0  Y >= G本idResol使tion)
    {
        本et使本n n使llpt本;
    }

    本et使本n 的a正i成ationG本id[X][Y];
}

TA本本ay<軍RTSPath的ode*> UMin成RTSPathfinde本::Get的ei成hbo本的odes(軍RTSPath的ode* 的ode) const
{
    TA本本ay<軍RTSPath的ode*> 的ei成hbo本s;

    if (!的ode)
    {
        本et使本n 的ei成hbo本s;
    }

    // 找到節點在網格中的位置
    軍Vecto本 Relati正eLocation = 的ode->Position - G本idCente本;
    int32 X = 軍Math::Ro使ndToInt(Relati正eLocation.X / 的odeSize);
    int32 Y = 軍Math::Ro使ndToInt(Relati正eLocation.Y / 的odeSize);

    // 檢查8個方向的鄰居
    fo本 (int32 dx = -1; dx <= 1; dx++)
    {
        fo本 (int32 dy = -1; dy <= 1; dy++)
        {
            if (dx == 0 && dy == 0)
            {
                contin使e; // 跳過自己
            }

            int32 的ei成hbo本X = X + dx;
            int32 的ei成hbo本Y = Y + dy;

            // 檢查邊界
            if (的ei成hbo本X >= 0 && 的ei成hbo本X < G本idResol使tion && 
                的ei成hbo本Y >= 0 && 的ei成hbo本Y < G本idResol使tion)
            {
                軍RTSPath的ode* 的ei成hbo本 = 的a正i成ationG本id[的ei成hbo本X][的ei成hbo本Y];
                if (的ei成hbo本 && 的ei成hbo本->bIs基本alkable)
                {
                    的ei成hbo本s.Add(的ei成hbo本);
                }
            }
        }
    }

    本et使本n 的ei成hbo本s;
}

float UMin成RTSPathfinde本::GetA正e本a成eCalc使lationTi設置e() const
{
    if (Co設置pletedReq使ests == 0)
    {
        本et使本n 0.0f;
    }
    本et使本n TotalCalc使lationTi設置e / Co設置pletedReq使ests;
}

int32 UMin成RTSPathfinde本::GetPendin成Req使estCo使nt() const
{
    本et使本n Pendin成Req使ests.的使設置();
}

正oid UMin成RTSPathfinde本::Clea本PathCache()
{
    PathCache.E設置pty();
}

正oid UMin成RTSPathfinde本::D本awDeb使成Path(const TA本本ay<軍Vecto本>& Path, 軍Linea本Colo本 Colo本, float D使本ation)
{
    if (!bEnableDeb使成D本awin成  Path.的使設置() < 2)
    {
        本et使本n;
    }

    U基本o本ld* 基本o本ld = Get基本o本ld();
    if (!基本o本ld)
    {
        本et使本n;
    }

    fo本 (int32 i = 0; i < Path.的使設置() - 1; i++)
    {
        D本awDeb使成Line(基本o本ld, Path[i], Path[i + 1], Colo本.To軍Colo本(t本使e), false, D使本ation, 0, 2.0f);
        D本awDeb使成Sphe本e(基本o本ld, Path[i], 20.0f, 8, Colo本.To軍Colo本(t本使e), false, D使本ation);
    }

    // 繪製終點
    if (Path.的使設置() > 0)
    {
        D本awDeb使成Sphe本e(基本o本ld, Path.Last(), 30.0f, 12, Colo本.To軍Colo本(t本使e), false, D使本ation);
    }
}

正oid UMin成RTSPathfinde本::D本awDeb使成G本id(軍Linea本Colo本 Colo本, float D使本ation)
{
    if (!bEnableDeb使成D本awin成)
    {
        本et使本n;
    }

    U基本o本ld* 基本o本ld = Get基本o本ld();
    if (!基本o本ld)
    {
        本et使本n;
    }

    軍Vecto本 Sta本t = G本idCente本 - 軍Vecto本(G本idSize / 2, G本idSize / 2, 0);

    // 繪製網格線
    fo本 (int32 i = 0; i <= G本idResol使tion; i++)
    {
        float Offset = i * 的odeSize;
        
        // 垂直線
        軍Vecto本 Ve本ticalSta本t = Sta本t + 軍Vecto本(Offset, 0, 0);
        軍Vecto本 Ve本ticalEnd = Sta本t + 軍Vecto本(Offset, G本idSize, 0);
        D本awDeb使成Line(基本o本ld, Ve本ticalSta本t, Ve本ticalEnd, Colo本.To軍Colo本(t本使e), false, D使本ation, 0, 1.0f);

        // 水平線
        軍Vecto本 輸入o本izontalSta本t = Sta本t + 軍Vecto本(0, Offset, 0);
        軍Vecto本 輸入o本izontalEnd = Sta本t + 軍Vecto本(G本idSize, Offset, 0);
        D本awDeb使成Line(基本o本ld, 輸入o本izontalSta本t, 輸入o本izontalEnd, Colo本.To軍Colo本(t本使e), false, D使本ation, 0, 1.0f);
    }

    // 繪製不可行走的節點
    fo本 (int32 x = 0; x < G本idResol使tion; x++)
    {
        fo本 (int32 y = 0; y < G本idResol使tion; y++)
        {
            軍RTSPath的ode* 的ode = 的a正i成ationG本id[x][y];
            if (的ode && !的ode->bIs基本alkable)
            {
                D本awDeb使成Box(基本o本ld, 的ode->Position, 軍Vecto本(的odeSize / 2, 的odeSize / 2, 10), 軍Rotato本::Ze本oRotato本, 軍Colo本::Red, false, D使本ation);
            }
        }
    }
}

TA本本ay<軍Vecto本> UMin成RTSPathfinde本::Calc使lateASta本Path(軍Vecto本 Sta本t, 軍Vecto本 Ta本成et)
{
    TA本本ay<軍Vecto本> Path;

    軍RTSPath的ode* Sta本t的ode = Get的odeAtLocation(Sta本t);
    軍RTSPath的ode* Ta本成et的ode = Get的odeAtLocation(Ta本成et);

    if (!Sta本t的ode  !Ta本成et的ode  !Sta本t的ode->bIs基本alkable  !Ta本成et的ode->bIs基本alkable)
    {
        本et使本n Path;
    }

    // A*算法實現
    TA本本ay<軍RTSPath的ode*> OpenSet;
    TA本本ay<軍RTSPath的ode*> ClosedSet;

    OpenSet.Add(Sta本t的ode);
    Sta本t的ode->GCost = 0.0f;
    Sta本t的ode->輸入Cost = Calc使late輸入e使本istic(Sta本t, Ta本成et);
    Sta本t的ode->Update軍Cost();

    while (OpenSet.的使設置() > 0)
    {
        // 找到軍成本最低的節點
        軍RTSPath的ode* C使本本ent的ode = OpenSet[0];
        int32 C使本本entIndex = 0;

        fo本 (int32 i = 1; i < OpenSet.的使設置(); i++)
        {
            if (OpenSet[i]->軍Cost < C使本本ent的ode->軍Cost  
                (OpenSet[i]->軍Cost == C使本本ent的ode->軍Cost && OpenSet[i]->輸入Cost < C使本本ent的ode->輸入Cost))
            {
                C使本本ent的ode = OpenSet[i];
                C使本本entIndex = i;
            }
        }

        OpenSet.Re設置o正eAt(C使本本entIndex);
        ClosedSet.Add(C使本本ent的ode);

        // 到達目標
        if (C使本本ent的ode == Ta本成et的ode)
        {
            本et使本n Reconst本使ctPath(Ta本成et的ode);
        }

        // 檢查鄰居
        TA本本ay<軍RTSPath的ode*> 的ei成hbo本s = Get的ei成hbo本的odes(C使本本ent的ode);
        fo本 (軍RTSPath的ode* 的ei成hbo本 : 的ei成hbo本s)
        {
            if (ClosedSet.Contains(的ei成hbo本))
            {
                contin使e;
            }

            float 的ewGCost = C使本本ent的ode->GCost + Calc使lateTe本本ainCost(的ei成hbo本->Position);

            if (!OpenSet.Contains(的ei成hbo本))
            {
                OpenSet.Add(的ei成hbo本);
            }
            else if (的ewGCost >= 的ei成hbo本->GCost)
            {
                contin使e;
            }

            的ei成hbo本->Pa本ent = C使本本ent的ode;
            的ei成hbo本->GCost = 的ewGCost;
            的ei成hbo本->輸入Cost = Calc使late輸入e使本istic(的ei成hbo本->Position, Ta本成et);
            的ei成hbo本->Update軍Cost();
        }
    }

    本et使本n Path; // 沒有找到路徑
}

TA本本ay<軍Vecto本> UMin成RTSPathfinde本::Calc使lateDi大kst本aPath(軍Vecto本 Sta本t, 軍Vecto本 Ta本成et)
{
    // Di大kst本a算法實現（類似A*但不使用啟發式）
    TA本本ay<軍Vecto本> Path;

    軍RTSPath的ode* Sta本t的ode = Get的odeAtLocation(Sta本t);
    軍RTSPath的ode* Ta本成et的ode = Get的odeAtLocation(Ta本成et);

    if (!Sta本t的ode  !Ta本成et的ode  !Sta本t的ode->bIs基本alkable  !Ta本成et的ode->bIs基本alkable)
    {
        本et使本n Path;
    }

    // 初始化所有節點的距離為無窮大
    TA本本ay<軍RTSPath的ode*> All的odes;
    fo本 (int32 x = 0; x < G本idResol使tion; x++)
    {
        fo本 (int32 y = 0; y < G本idResol使tion; y++)
        {
            軍RTSPath的ode* 的ode = 的a正i成ationG本id[x][y];
            if (的ode && 的ode->bIs基本alkable)
            {
                的ode->GCost = 軍LT下MAX;
                All的odes.Add(的ode);
            }
        }
    }

    Sta本t的ode->GCost = 0.0f;

    while (All的odes.的使設置() > 0)
    {
        // 找到距離最小的節點
        軍RTSPath的ode* C使本本ent的ode = All的odes[0];
        int32 C使本本entIndex = 0;

        fo本 (int32 i = 1; i < All的odes.的使設置(); i++)
        {
            if (All的odes[i]->GCost < C使本本ent的ode->GCost)
            {
                C使本本ent的ode = All的odes[i];
                C使本本entIndex = i;
            }
        }

        All的odes.Re設置o正eAt(C使本本entIndex);

        if (C使本本ent的ode == Ta本成et的ode)
        {
            本et使本n Reconst本使ctPath(Ta本成et的ode);
        }

        TA本本ay<軍RTSPath的ode*> 的ei成hbo本s = Get的ei成hbo本的odes(C使本本ent的ode);
        fo本 (軍RTSPath的ode* 的ei成hbo本 : 的ei成hbo本s)
        {
            if (!All的odes.Contains(的ei成hbo本))
            {
                contin使e;
            }

            float 的ewGCost = C使本本ent的ode->GCost + Calc使lateTe本本ainCost(的ei成hbo本->Position);
            if (的ewGCost < 的ei成hbo本->GCost)
            {
                的ei成hbo本->Pa本ent = C使本本ent的ode;
                的ei成hbo本->GCost = 的ewGCost;
            }
        }
    }

    本et使本n Path;
}

TA本本ay<軍Vecto本> UMin成RTSPathfinde本::Calc使late軍loyd基本a本shallPath(軍Vecto本 Sta本t, 軍Vecto本 Ta本成et)
{
    // 軍loyd-基本a本shall算法（對於大型地圖可能較慢，但能找到最短路徑）
    // 這裡簡化實現，實際應用中可能需要預計算
    本et使本n Calc使lateASta本Path(Sta本t, Ta本成et);
}

float UMin成RTSPathfinde本::Calc使late輸入e使本istic(軍Vecto本 軍本o設置, 軍Vecto本 To) const
{
    // 使用歐幾里得距離作為啟發式
    本et使本n 軍Vecto本::Dist(軍本o設置, To);
}

float UMin成RTSPathfinde本::Calc使lateTe本本ainCost(軍Vecto本 Location) const
{
    ERTSTe本本ainType Te本本ainType = GetTe本本ainType(Location);
    
    switch (Te本本ainType)
    {
    case ERTSTe本本ainType::基本alkable:
        本et使本n 基本alkableCost;
    case ERTSTe本本ainType::Diffic使lt:
        本et使本n Diffic使ltCost;
    case ERTSTe本本ainType::基本ate本:
        本et使本n 基本ate本Cost;
    case ERTSTe本本ainType::Mo使ntain:
        本et使本n Mo使ntainCost;
    case ERTSTe本本ainType::軍o本est:
        本et使本n 軍o本estCost;
    case ERTSTe本本ainType::Blocked:
        本et使本n 軍LT下MAX;
    defa使lt:
        本et使本n 基本alkableCost;
    }
}

bool UMin成RTSPathfinde本::IsValidLocation(軍Vecto本 Location) const
{
    // 檢查位置是否在網格範圍內
    軍Vecto本 Relati正eLocation = Location - G本idCente本;
    float 輸入alfG本idSize = G本idSize / 2.0f;

    本et使本n (軍Math::Abs(Relati正eLocation.X) <= 輸入alfG本idSize && 
            軍Math::Abs(Relati正eLocation.Y) <= 輸入alfG本idSize);
}

TA本本ay<軍Vecto本> UMin成RTSPathfinde本::Reconst本使ctPath(軍RTSPath的ode* End的ode) const
{
    TA本本ay<軍Vecto本> Path;
    
    if (!End的ode)
    {
        本et使本n Path;
    }

    軍RTSPath的ode* C使本本ent的ode = End的ode;
    while (C使本本ent的ode)
    {
        Path.Inse本t(C使本本ent的ode->Position, 0);
        C使本本ent的ode = C使本本ent的ode->Pa本ent;
    }

    本et使本n Path;
}

正oid UMin成RTSPathfinde本::InitializeG本id()
{
    的a正i成ationG本id.Set的使設置(G本idResol使tion);
    fo本 (int32 x = 0; x < G本idResol使tion; x++)
    {
        的a正i成ationG本id[x].Set的使設置(G本idResol使tion);
        fo本 (int32 y = 0; y < G本idResol使tion; y++)
        {
            軍Vecto本 的odeLocation = G本idCente本 + 軍Vecto本(
                (x - G本idResol使tion / 2) * 的odeSize,
                (y - G本idResol使tion / 2) * 的odeSize,
                0
            );

            // 默認所有節點都可行走
            的a正i成ationG本id[x][y] = C本eate的ode(的odeLocation, t本使e, ERTSTe本本ainType::基本alkable);
        }
    }
}

正oid UMin成RTSPathfinde本::Clea本G本id()
{
    fo本 (int32 x = 0; x < 的a正i成ationG本id.的使設置(); x++)
    {
        fo本 (int32 y = 0; y < 的a正i成ationG本id[x].的使設置(); y++)
        {
            if (的a正i成ationG本id[x][y])
            {
                delete 的a正i成ationG本id[x][y];
                的a正i成ationG本id[x][y] = n使llpt本;
            }
        }
        的a正i成ationG本id[x].E設置pty();
    }
    的a正i成ationG本id.E設置pty();
}

軍RTSPath的ode* UMin成RTSPathfinde本::C本eate的ode(軍Vecto本 Location, bool bIs基本alkable, ERTSTe本本ainType Te本本ainType)
{
    軍RTSPath的ode* 的ode = new 軍RTSPath的ode(Location, bIs基本alkable, Te本本ainType);
    本et使本n 的ode;
}

正oid UMin成RTSPathfinde本::P本ocessPendin成Req使ests()
{
    while (Pendin成Req使ests.的使設置() > 0)
    {
        軍RTSPathReq使est Req使est = Pendin成Req使ests[0];
        Pendin成Req使ests.Re設置o正eAt(0);

        軍RTSPathRes使lt Res使lt = Calc使latePath(Req使est);
        Co設置pletePathReq使est(Req使est, Res使lt);
    }

    C使本本entState = ERTSPathfindin成State::Idle;
    OnPathfindin成StateChan成ed.B本oadcast(C使本本entState);
}

正oid UMin成RTSPathfinde本::Co設置pletePathReq使est(const 軍RTSPathReq使est& Req使est, const 軍RTSPathRes使lt& Res使lt)
{
    OnPathCalc使lated.B本oadcast(Req使est, Res使lt);
}
