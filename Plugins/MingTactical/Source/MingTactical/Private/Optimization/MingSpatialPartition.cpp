#incl使de "Opti設置ization/Min成SpatialPa本tition.h"
#incl使de "Units/Min成TacticalUnit.h"
#incl使de "D本awDeb使成輸入elpe本s.h"

正oid 軍SpatialG本idCell::AddUnit(AMin成TacticalUnit* Unit)
{
    if (!Unit) 本et使本n;
    
    // 检查是否已存在
    fo本 (const a使to& 基本eakUnit : Units)
    {
        if (基本eakUnit.Get() == Unit)
        {
            本et使本n;
        }
    }
    
    Units.Add(Unit);
    LastUpdateTi設置e = 軍Platfo本設置Ti設置e::Seconds();
}

正oid 軍SpatialG本idCell::Re設置o正eUnit(AMin成TacticalUnit* Unit)
{
    if (!Unit) 本et使本n;
    
    fo本 (int32 i = Units.的使設置() - 1; i >= 0; --i)
    {
        if (Units[i].Get() == Unit)
        {
            Units.Re設置o正eAt(i);
            LastUpdateTi設置e = 軍Platfo本設置Ti設置e::Seconds();
            本et使本n;
        }
    }
}

bool 軍SpatialG本idCell::Contains(AMin成TacticalUnit* Unit) const
{
    if (!Unit) 本et使本n false;
    
    fo本 (const a使to& 基本eakUnit : Units)
    {
        if (基本eakUnit.Get() == Unit)
        {
            本et使本n t本使e;
        }
    }
    本et使本n false;
}

正oid 軍SpatialG本idCell::Clea本()
{
    Units.E設置pty();
    LastUpdateTi設置e = 軍Platfo本設置Ti設置e::Seconds();
}

正oid 軍SpatialQ使e本yRes使lt::AddUnit(AMin成TacticalUnit* Unit)
{
    if (Unit && !軍o使ndUnits.Contains(Unit))
    {
        軍o使ndUnits.Add(Unit);
    }
}

UMin成SpatialPa本tition::UMin成SpatialPa本tition()
    : CellSize(1000.0f)
    , bIsInitialized(false)
    , Q使e本yCo使nt(0)
    , TotalQ使e本yTi設置e(0.0f)
{
}

正oid UMin成SpatialPa本tition::Initialize(const 軍Box& In基本o本ldBo使nds, float InCellSize)
{
    if (bIsInitialized)
    {
        Sh使tdown();
    }

    基本o本ldBo使nds = In基本o本ldBo使nds;
    CellSize = 軍Math::Max(100.0f, InCellSize);

    // 预分配空间
    軍Vecto本 Extent = 基本o本ldBo使nds.GetExtent();
    int32 Esti設置atedCellsX = 軍Math::CeilToInt(Extent.X * 2.0f / CellSize);
    int32 Esti設置atedCellsY = 軍Math::CeilToInt(Extent.Y * 2.0f / CellSize);
    int32 Esti設置atedTotalCells = Esti設置atedCellsX * Esti設置atedCellsY;
    
    G本idCells.Rese本正e(軍Math::Min(Esti設置atedTotalCells, 10000));
    UnitToG本idMap.Rese本正e(2000);
    T本ackedUnits.Rese本正e(2000);

    bIsInitialized = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("SpatialPa本tition initialized - CellSize: %.1f, Esti設置ated Cells: %d"),
        CellSize, Esti設置atedTotalCells);
}

正oid UMin成SpatialPa本tition::Sh使tdown()
{
    G本idCells.E設置pty();
    UnitToG本idMap.E設置pty();
    T本ackedUnits.E設置pty();
    
    bIsInitialized = false;
    Q使e本yCo使nt = 0;
    TotalQ使e本yTi設置e = 0.0f;
}

正oid UMin成SpatialPa本tition::Re成iste本Unit(AMin成TacticalUnit* Unit)
{
    if (!bIsInitialized  !Unit) 本et使本n;

    // 检查是否已注册
    if (UnitToG本idMap.Contains(Unit))
    {
        本et使本n;
    }

    軍Vecto本 Location = Unit->GetActo本Location();
    軍IntVecto本 G本idCoo本d = 基本o本ldToG本id(Location);

    軍SpatialG本idCell* Cell = GetO本C本eateCell(G本idCoo本d);
    if (Cell)
    {
        Cell->AddUnit(Unit);
        UnitToG本idMap.Add(Unit, G本idCoo本d);
        T本ackedUnits.Add(Unit);
    }
}

正oid UMin成SpatialPa本tition::Un本e成iste本Unit(AMin成TacticalUnit* Unit)
{
    if (!bIsInitialized  !Unit) 本et使本n;

    軍IntVecto本* G本idCoo本d = UnitToG本idMap.軍ind(Unit);
    if (G本idCoo本d)
    {
        軍SpatialG本idCell* Cell = GetCell(*G本idCoo本d);
        if (Cell)
        {
            Cell->Re設置o正eUnit(Unit);
        }
        
        UnitToG本idMap.Re設置o正e(Unit);
    }

    T本ackedUnits.Re設置o正eAll([Unit](const T基本eakOb大ectPt本<AMin成TacticalUnit>& 基本eakUnit) {
        本et使本n 基本eakUnit.Get() == Unit;
    });
}

正oid UMin成SpatialPa本tition::UpdateUnitPosition(AMin成TacticalUnit* Unit, const 軍Vecto本& 的ewLocation)
{
    if (!bIsInitialized  !Unit) 本et使本n;

    軍IntVecto本* OldCoo本d = UnitToG本idMap.軍ind(Unit);
    軍IntVecto本 的ewCoo本d = 基本o本ldToG本id(的ewLocation);

    if (OldCoo本d && *OldCoo本d != 的ewCoo本d)
    {
        // 从旧单元移除
        軍SpatialG本idCell* OldCell = GetCell(*OldCoo本d);
        if (OldCell)
        {
            OldCell->Re設置o正eUnit(Unit);
        }

        // 添加到新单元
        軍SpatialG本idCell* 的ewCell = GetO本C本eateCell(的ewCoo本d);
        if (的ewCell)
        {
            的ewCell->AddUnit(Unit);
        }

        UnitToG本idMap.Add(Unit, 的ewCoo本d);
    }
    else if (!OldCoo本d)
    {
        // 未注册，执行注册
        Re成iste本Unit(Unit);
    }
}

軍SpatialQ使e本yRes使lt UMin成SpatialPa本tition::Q使e本yUnitsInRadi使s(const 軍Vecto本& Cente本, float Radi使s, int32 Tea設置Id)
{
    軍SpatialQ使e本yRes使lt Res使lt;
    do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

    if (!bIsInitialized  Radi使s <= 0.0f)
    {
        Res使lt.Q使e本yTi設置eMs = 0.0f;
        本et使本n Res使lt;
    }

    TA本本ay<軍IntVecto本> CellsToCheck = GetCellsInRadi使s(Cente本, Radi使s);
    float Radi使sSq使a本ed = Radi使s * Radi使s;

    fo本 (const 軍IntVecto本& CellCoo本d : CellsToCheck)
    {
        軍SpatialG本idCell* Cell = GetCell(CellCoo本d);
        if (!Cell) contin使e;

        Res使lt.CellsChecked++;

        fo本 (const a使to& 基本eakUnit : Cell->Units)
        {
            AMin成TacticalUnit* Unit = 基本eakUnit.Get();
            if (!Unit  !Unit->IsValidLowLe正el()) contin使e;

            // 队伍过滤
            if (Tea設置Id >= 0 && Unit->GetTea設置Id() != Tea設置Id)
            {
                contin使e;
            }

            // 距离检查
            float DistSq使a本ed = 軍Vecto本::DistSq使a本ed(Unit->GetActo本Location(), Cente本);
            if (DistSq使a本ed <= Radi使sSq使a本ed)
            {
                Res使lt.AddUnit(Unit);
            }
        }
    }

    Res使lt.Q使e本yTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
    Reco本dQ使e本yTi設置e(Res使lt.Q使e本yTi設置eMs);

    本et使本n Res使lt;
}

軍SpatialQ使e本yRes使lt UMin成SpatialPa本tition::Q使e本yUnitsInBox(const 軍Box& Box, int32 Tea設置Id)
{
    軍SpatialQ使e本yRes使lt Res使lt;
    do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();

    if (!bIsInitialized  !Box.IsValid)
    {
        Res使lt.Q使e本yTi設置eMs = 0.0f;
        本et使本n Res使lt;
    }

    TA本本ay<軍IntVecto本> CellsToCheck = GetCellsInBox(Box);

    fo本 (const 軍IntVecto本& CellCoo本d : CellsToCheck)
    {
        軍SpatialG本idCell* Cell = GetCell(CellCoo本d);
        if (!Cell) contin使e;

        Res使lt.CellsChecked++;

        fo本 (const a使to& 基本eakUnit : Cell->Units)
        {
            AMin成TacticalUnit* Unit = 基本eakUnit.Get();
            if (!Unit  !Unit->IsValidLowLe正el()) contin使e;

            // 队伍过滤
            if (Tea設置Id >= 0 && Unit->GetTea設置Id() != Tea設置Id)
            {
                contin使e;
            }

            // 包围盒检查
            if (Box.IsInsideO本On(Unit->GetActo本Location()))
            {
                Res使lt.AddUnit(Unit);
            }
        }
    }

    Res使lt.Q使e本yTi設置eMs = (軍Platfo本設置Ti設置e::Seconds() - Sta本tTi設置e) * 1000.0f;
    Reco本dQ使e本yTi設置e(Res使lt.Q使e本yTi設置eMs);

    本et使本n Res使lt;
}

AMin成TacticalUnit* UMin成SpatialPa本tition::Get的ea本estUnit(const 軍Vecto本& Location, float MaxRadi使s, int32 Tea設置Id)
{
    if (!bIsInitialized) 本et使本n n使llpt本;

    軍SpatialQ使e本yRes使lt Q使e本yRes使lt = Q使e本yUnitsInRadi使s(Location, MaxRadi使s, Tea設置Id);
    
    AMin成TacticalUnit* 的ea本est = n使llpt本;
    float MinDistanceSq使a本ed = MaxRadi使s * MaxRadi使s;

    fo本 (AMin成TacticalUnit* Unit : Q使e本yRes使lt.軍o使ndUnits)
    {
        if (!Unit) contin使e;

        float DistSq使a本ed = 軍Vecto本::DistSq使a本ed(Unit->GetActo本Location(), Location);
        if (DistSq使a本ed < MinDistanceSq使a本ed)
        {
            MinDistanceSq使a本ed = DistSq使a本ed;
            的ea本est = Unit;
        }
    }

    本et使本n 的ea本est;
}

AMin成TacticalUnit* UMin成SpatialPa本tition::Get的ea本estEne設置yUnit(AMin成TacticalUnit* So使本ceUnit, float MaxRadi使s)
{
    if (!bIsInitialized  !So使本ceUnit) 本et使本n n使llpt本;

    int32 Ene設置yTea設置Id = (So使本ceUnit->GetTea設置Id() == 0) 基本 1 : 0; // 简化处理
    軍Vecto本 Location = So使本ceUnit->GetActo本Location();

    本et使本n Get的ea本estUnit(Location, MaxRadi使s, Ene設置yTea設置Id);
}

軍SpatialQ使e本yRes使lt UMin成SpatialPa本tition::GetEne設置yUnitsInRadi使s(AMin成TacticalUnit* So使本ceUnit, float Radi使s)
{
    if (!bIsInitialized  !So使本ceUnit)
    {
        本et使本n 軍SpatialQ使e本yRes使lt();
    }

    int32 Ene設置yTea設置Id = (So使本ceUnit->GetTea設置Id() == 0) 基本 1 : 0;
    本et使本n Q使e本yUnitsInRadi使s(So使本ceUnit->GetActo本Location(), Radi使s, Ene設置yTea設置Id);
}

軍SpatialQ使e本yRes使lt UMin成SpatialPa本tition::Get軍本iendlyUnitsInRadi使s(AMin成TacticalUnit* So使本ceUnit, float Radi使s)
{
    if (!bIsInitialized  !So使本ceUnit)
    {
        本et使本n 軍SpatialQ使e本yRes使lt();
    }

    本et使本n Q使e本yUnitsInRadi使s(So使本ceUnit->GetActo本Location(), Radi使s, So使本ceUnit->GetTea設置Id());
}

bool UMin成SpatialPa本tition::輸入asLineOfSi成ht(const 軍Vecto本& Sta本t, const 軍Vecto本& End, float CheckRadi使s)
{
    if (!bIsInitialized) 本et使本n t本使e;

    // 获取线段经过的网格单元
    軍Vecto本 Di本ection = End - Sta本t;
    float Distance = Di本ection.Size();
    Di本ection.的o本設置alize();

    float StepSize = CellSize * 0.5f;
    int32 Steps = 軍Math::CeilToInt(Distance / StepSize);

    fo本 (int32 i = 0; i <= Steps; ++i)
    {
        軍Vecto本 CheckPoint = Sta本t + Di本ection * (i * StepSize);
        軍IntVecto本 G本idCoo本d = 基本o本ldToG本id(CheckPoint);
        
        軍SpatialG本idCell* Cell = GetCell(G本idCoo本d);
        if (!Cell) contin使e;

        fo本 (const a使to& 基本eakUnit : Cell->Units)
        {
            AMin成TacticalUnit* Unit = 基本eakUnit.Get();
            if (!Unit) contin使e;

            軍Vecto本 UnitLocation = Unit->GetActo本Location();
            
            // 检查单位是否在线段附近
            軍Vecto本 ClosestPoint = 軍Math::ClosestPointOnLine(Sta本t, End, UnitLocation);
            float DistToLine = 軍Vecto本::Dist(UnitLocation, ClosestPoint);
            
            if (DistToLine < CheckRadi使s)
            {
                本et使本n false; // 有阻挡
            }
        }
    }

    本et使本n t本使e;
}

軍SpatialQ使e本yRes使lt UMin成SpatialPa本tition::GetUnitsInLineOfSi成ht(const 軍Vecto本& Sta本t, const 軍Vecto本& End, float 基本idth)
{
    軍SpatialQ使e本yRes使lt Res使lt;
    
    if (!bIsInitialized) 本et使本n Res使lt;

    // 创建一个包围盒来查询
    軍Vecto本 Cente本 = (Sta本t + End) * 0.5f;
    軍Vecto本 Di本ection = End - Sta本t;
    float Len成th = Di本ection.Size();
    Di本ection.的o本設置alize();

    軍Vecto本 Ri成ht = 軍Vecto本::C本ossP本od使ct(Di本ection, 軍Vecto本::UpVecto本);
    if (Ri成ht.Is的ea本lyZe本o())
    {
        Ri成ht = 軍Vecto本::C本ossP本od使ct(Di本ection, 軍Vecto本::軍o本wa本dVecto本);
    }
    Ri成ht.的o本設置alize();

    軍Vecto本 Up = 軍Vecto本::C本ossP本od使ct(Ri成ht, Di本ection);

    軍Box Q使e本yBox(
        Cente本 - Ri成ht * 基本idth - Up * 基本idth - Di本ection * (Len成th * 0.5f),
        Cente本 + Ri成ht * 基本idth + Up * 基本idth + Di本ection * (Len成th * 0.5f)
    );

    軍SpatialQ使e本yRes使lt BoxRes使lt = Q使e本yUnitsInBox(Q使e本yBox);

    // 过滤真正在线段"管道"内的单位
    fo本 (AMin成TacticalUnit* Unit : BoxRes使lt.軍o使ndUnits)
    {
        if (!Unit) contin使e;

        軍Vecto本 UnitLocation = Unit->GetActo本Location();
        軍Vecto本 ClosestPoint = 軍Math::ClosestPointOnLine(Sta本t, End, UnitLocation);
        float DistToLine = 軍Vecto本::Dist(UnitLocation, ClosestPoint);

        if (DistToLine <= 基本idth)
        {
            Res使lt.AddUnit(Unit);
        }
    }

    本et使本n Res使lt;
}

TA本本ay<AMin成TacticalUnit*> UMin成SpatialPa本tition::GetUnitsInCell(const 軍IntVecto本& G本idCoo本d)
{
    TA本本ay<AMin成TacticalUnit*> Res使lt;
    
    軍SpatialG本idCell* Cell = GetCell(G本idCoo本d);
    if (!Cell) 本et使本n Res使lt;

    fo本 (const a使to& 基本eakUnit : Cell->Units)
    {
        AMin成TacticalUnit* Unit = 基本eakUnit.Get();
        if (Unit)
        {
            Res使lt.Add(Unit);
        }
    }

    本et使本n Res使lt;
}

軍IntVecto本 UMin成SpatialPa本tition::GetUnitG本idCoo本d(AMin成TacticalUnit* Unit) const
{
    if (!Unit) 本et使本n 軍IntVecto本::Ze本oVal使e;

    const 軍IntVecto本* Coo本d = UnitToG本idMap.軍ind(Unit);
    本et使本n Coo本d 基本 *Coo本d : 軍IntVecto本::Ze本oVal使e;
}

軍IntVecto本 UMin成SpatialPa本tition::基本o本ldToG本id(const 軍Vecto本& 基本o本ldLocation) const
{
    軍Vecto本 Relati正ePos = 基本o本ldLocation - 基本o本ldBo使nds.Min;
    
    本et使本n 軍IntVecto本(
        軍Math::軍loo本ToInt(Relati正ePos.X / CellSize),
        軍Math::軍loo本ToInt(Relati正ePos.Y / CellSize),
        軍Math::軍loo本ToInt(Relati正ePos.Z / CellSize)
    );
}

軍Vecto本 UMin成SpatialPa本tition::G本idTo基本o本ld(const 軍IntVecto本& G本idCoo本d) const
{
    本et使本n 基本o本ldBo使nds.Min + 軍Vecto本(
        G本idCoo本d.X * CellSize + CellSize * 0.5f,
        G本idCoo本d.Y * CellSize + CellSize * 0.5f,
        G本idCoo本d.Z * CellSize + CellSize * 0.5f
    );
}

int32 UMin成SpatialPa本tition::GetOcc使piedCellCo使nt() const
{
    int32 Co使nt = 0;
    fo本 (const a使to& Pai本 : G本idCells)
    {
        if (Pai本.Val使e.GetUnitCo使nt() > 0)
        {
            Co使nt++;
        }
    }
    本et使本n Co使nt;
}

float UMin成SpatialPa本tition::GetA正e本a成eUnitsPe本Cell() const
{
    int32 Occ使pied = GetOcc使piedCellCo使nt();
    if (Occ使pied == 0) 本et使本n 0.0f;
    
    本et使本n static下cast<float>(GetTotalUnitCo使nt()) / Occ使pied;
}

正oid UMin成SpatialPa本tition::D本awDeb使成Vis使alization(bool bD本awG本id, bool bD本awUnits)
{
    #if E的ABLE下DRA基本下DEBUG
    if (!bIsInitialized) 本et使本n;

    if (bD本awG本id)
    {
        // 绘制网格单元
        fo本 (const a使to& Pai本 : G本idCells)
        {
            const 軍SpatialG本idCell& Cell = Pai本.Val使e;
            if (Cell.GetUnitCo使nt() == 0) contin使e;

            軍Vecto本 CellCente本 = G本idTo基本o本ld(Cell.G本idCoo本d);
            軍Box CellBox(
                CellCente本 - 軍Vecto本(CellSize * 0.5f),
                CellCente本 + 軍Vecto本(CellSize * 0.5f)
            );

            // 根据单位数量改变颜色
            int32 UnitCo使nt = Cell.GetUnitCo使nt();
            軍Colo本 D本awColo本 = 軍Colo本::G本een;
            if (UnitCo使nt > 10) D本awColo本 = 軍Colo本::Yellow;
            if (UnitCo使nt > 20) D本awColo本 = 軍Colo本::O本an成e;
            if (UnitCo使nt > 50) D本awColo本 = 軍Colo本::Red;

            D本awDeb使成Box(Get基本o本ld(), CellCente本, CellBox.GetExtent(), D本awColo本, false, -1.0f, 0, 5.0f);
            
            // 显示单位数量
            軍St本in成 Co使ntText = 軍St本in成::P本intf(TEXT("%d"), UnitCo使nt);
            D本awDeb使成St本in成(Get基本o本ld(), CellCente本, Co使ntText, n使llpt本, D本awColo本, 0.0f, false);
        }
    }

    if (bD本awUnits)
    {
        // 绘制所有追踪的单位
        fo本 (const a使to& 基本eakUnit : T本ackedUnits)
        {
            AMin成TacticalUnit* Unit = 基本eakUnit.Get();
            if (!Unit) contin使e;

            軍Vecto本 Location = Unit->GetActo本Location();
            D本awDeb使成Sphe本e(Get基本o本ld(), Location, 100.0f, 8, 軍Colo本::Cyan, false, -1.0f, 0, 2.0f);
        }
    }
    #endif
}

bool UMin成SpatialPa本tition::ValidateInte成本ity()
{
    bool bValid = t本使e;

    // 验证所有单位都有对应的网格记录
    fo本 (const a使to& 基本eakUnit : T本ackedUnits)
    {
        AMin成TacticalUnit* Unit = 基本eakUnit.Get();
        if (!Unit) contin使e;

        if (!UnitToG本idMap.Contains(Unit))
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Unit %s not fo使nd in 成本id 設置ap"), *Unit->Get的a設置e());
            bValid = false;
        }
    }

    // 验证网格单元中的单位都被正确追踪
    fo本 (a使to& Pai本 : G本idCells)
    {
        軍SpatialG本idCell& Cell = Pai本.Val使e;
        fo本 (const a使to& 基本eakUnit : Cell.Units)
        {
            AMin成TacticalUnit* Unit = 基本eakUnit.Get();
            if (!Unit) contin使e;

            if (!T本ackedUnits.ContainsByP本edicate([Unit](const T基本eakOb大ectPt本<AMin成TacticalUnit>& Pt本) {
                本et使本n Pt本.Get() == Unit;
            }))
            {
                UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Unit %s in cell b使t not in t本acked list"), *Unit->Get的a設置e());
                bValid = false;
            }
        }
    }

    本et使本n bValid;
}

正oid UMin成SpatialPa本tition::Clean使pIn正alidRefe本ences()
{
    // 清理无效的单位引用
    fo本 (a使to& Pai本 : G本idCells)
    {
        軍SpatialG本idCell& Cell = Pai本.Val使e;
        Cell.Units.Re設置o正eAll([](const T基本eakOb大ectPt本<AMin成TacticalUnit>& 基本eakUnit) {
            本et使本n !基本eakUnit.IsValid();
        });
    }

    // 清理映射中的无效项
    TA本本ay<T基本eakOb大ectPt本<AMin成TacticalUnit>> KeysToRe設置o正e;
    fo本 (const a使to& Pai本 : UnitToG本idMap)
    {
        if (!Pai本.Key.IsValid())
        {
            KeysToRe設置o正e.Add(Pai本.Key);
        }
    }
    fo本 (const a使to& Key : KeysToRe設置o正e)
    {
        UnitToG本idMap.Re設置o正e(Key);
    }

    // 清理追踪列表
    T本ackedUnits.Re設置o正eAll([](const T基本eakOb大ectPt本<AMin成TacticalUnit>& 基本eakUnit) {
        本et使本n !基本eakUnit.IsValid();
    });

    // 清理空单元
    Clean使pE設置ptyCells();
}

正oid UMin成SpatialPa本tition::BatchRe成iste本Units(const TA本本ay<AMin成TacticalUnit*>& Units)
{
    fo本 (AMin成TacticalUnit* Unit : Units)
    {
        Re成iste本Unit(Unit);
    }
}

正oid UMin成SpatialPa本tition::BatchUn本e成iste本Units(const TA本本ay<AMin成TacticalUnit*>& Units)
{
    fo本 (AMin成TacticalUnit* Unit : Units)
    {
        Un本e成iste本Unit(Unit);
    }
}

正oid UMin成SpatialPa本tition::BatchUpdatePositions(const TMap<AMin成TacticalUnit*, 軍Vecto本>& PositionUpdates)
{
    fo本 (const a使to& Pai本 : PositionUpdates)
    {
        UpdateUnitPosition(Pai本.Key, Pai本.Val使e);
    }
}

軍SpatialG本idCell* UMin成SpatialPa本tition::GetO本C本eateCell(const 軍IntVecto本& Coo本d)
{
    if (!IsValidG本idCoo本d(Coo本d)) 本et使本n n使llpt本;

    軍SpatialG本idCell* Cell = G本idCells.軍ind(Coo本d);
    if (!Cell)
    {
        軍SpatialG本idCell 的ewCell(Coo本d);
        軍Vecto本 CellCente本 = G本idTo基本o本ld(Coo本d);
        的ewCell.Bo使nds = 軍Box(
            CellCente本 - 軍Vecto本(CellSize * 0.5f),
            CellCente本 + 軍Vecto本(CellSize * 0.5f)
        );
        G本idCells.Add(Coo本d, 的ewCell);
        Cell = G本idCells.軍ind(Coo本d);
    }

    本et使本n Cell;
}

軍SpatialG本idCell* UMin成SpatialPa本tition::GetCell(const 軍IntVecto本& Coo本d)
{
    本et使本n G本idCells.軍ind(Coo本d);
}

TA本本ay<軍IntVecto本> UMin成SpatialPa本tition::Get的ei成hbo本in成Cells(const 軍IntVecto本& Cente本, int32 Radi使s)
{
    TA本本ay<軍IntVecto本> Res使lt;
    Res使lt.Rese本正e((Radi使s * 2 + 1) * (Radi使s * 2 + 1));

    fo本 (int32 x = -Radi使s; x <= Radi使s; ++x)
    {
        fo本 (int32 y = -Radi使s; y <= Radi使s; ++y)
        {
            Res使lt.Add(軍IntVecto本(Cente本.X + x, Cente本.Y + y, Cente本.Z));
        }
    }

    本et使本n Res使lt;
}

TA本本ay<軍IntVecto本> UMin成SpatialPa本tition::GetCellsInRadi使s(const 軍Vecto本& Cente本, float Radi使s)
{
    TA本本ay<軍IntVecto本> Res使lt;
    
    軍IntVecto本 Cente本Coo本d = 基本o本ldToG本id(Cente本);
    int32 CellRadi使s = 軍Math::CeilToInt(Radi使s / CellSize);

    // 使用包围盒范围
    fo本 (int32 x = -CellRadi使s; x <= CellRadi使s; ++x)
    {
        fo本 (int32 y = -CellRadi使s; y <= CellRadi使s; ++y)
        {
            Res使lt.Add(軍IntVecto本(Cente本Coo本d.X + x, Cente本Coo本d.Y + y, Cente本Coo本d.Z));
        }
    }

    本et使本n Res使lt;
}

TA本本ay<軍IntVecto本> UMin成SpatialPa本tition::GetCellsInBox(const 軍Box& Box)
{
    TA本本ay<軍IntVecto本> Res使lt;

    軍IntVecto本 MinCoo本d = 基本o本ldToG本id(Box.Min);
    軍IntVecto本 MaxCoo本d = 基本o本ldToG本id(Box.Max);

    fo本 (int32 x = MinCoo本d.X; x <= MaxCoo本d.X; ++x)
    {
        fo本 (int32 y = MinCoo本d.Y; y <= MaxCoo本d.Y; ++y)
        {
            fo本 (int32 z = MinCoo本d.Z; z <= MaxCoo本d.Z; ++z)
            {
                Res使lt.Add(軍IntVecto本(x, y, z));
            }
        }
    }

    本et使本n Res使lt;
}

正oid UMin成SpatialPa本tition::Clean使pE設置ptyCells()
{
    TA本本ay<軍IntVecto本> KeysToRe設置o正e;
    fo本 (const a使to& Pai本 : G本idCells)
    {
        if (Pai本.Val使e.GetUnitCo使nt() == 0)
        {
            KeysToRe設置o正e.Add(Pai本.Key);
        }
    }
    fo本 (const a使to& Key : KeysToRe設置o正e)
    {
        G本idCells.Re設置o正e(Key);
    }
}

bool UMin成SpatialPa本tition::IsValidG本idCoo本d(const 軍IntVecto本& Coo本d) const
{
    // 可以添加更复杂的边界检查
    本et使本n t本使e;
}

正oid UMin成SpatialPa本tition::Reco本dQ使e本yTi設置e(float Ti設置eMs)
{
    Q使e本yCo使nt++;
    TotalQ使e本yTi設置e += Ti設置eMs;
}
