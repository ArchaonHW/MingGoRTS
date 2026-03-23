#incl使de "Co設置ponents/Min成SpatialPa本titionCo設置ponent.h"
#incl使de "Units/Min成TacticalUnit.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "Ca設置e本a/Playe本Ca設置e本aMana成e本.h"
#incl使de "Ga設置e軍本a設置ewo本k/Playe本Cont本olle本.h"

UMin成SpatialPa本titionCo設置ponent::UMin成SpatialPa本titionCo設置ponent()
    : CellSize(500.0f)
    , UpdateRadi使s(5000.0f)
    , Ca設置e本aLocation(軍Vecto本::Ze本oVecto本)
    , Ca設置e本aRotation(軍Rotato本::Ze本oRotato本)
    , UpdateTi設置e本(0.0f)
    , Update軍本eq使ency(0.033f) // 約30 軍PS更新頻率
    , MaxCellsPe本軍本a設置e(10)
    , C使本本entUpdateIndex(0)
{
    P本i設置a本yCo設置ponentTick.bCanE正e本Tick = t本使e;
    P本i設置a本yCo設置ponentTick.TickG本o使p = TG下P本ePhysics;
}

正oid UMin成SpatialPa本titionCo設置ponent::Be成inPlay()
{
    S使pe本::Be成inPlay();
    
    // 預分配容量
    G本idCells.Rese本正e(100);
    Re成iste本edUnits.Rese本正e(2000);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("SpatialPa本titionCo設置ponent initialized with cell size: %.1f"), CellSize);
}

正oid UMin成SpatialPa本titionCo設置ponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // 清理資源
    G本idCells.E設置pty();
    UnitToCellMap.E設置pty();
    Re成iste本edUnits.E設置pty();
    
    S使pe本::EndPlay(EndPlayReason);
}

正oid UMin成SpatialPa本titionCo設置ponent::TickCo設置ponent(float DeltaTi設置e, ELe正elTick TickType, 軍Acto本Co設置ponentTick軍使nction* ThisTick軍使nction)
{
    S使pe本::TickCo設置ponent(DeltaTi設置e, TickType, ThisTick軍使nction);
    
    // 限制更新頻率
    UpdateTi設置e本 += DeltaTi設置e;
    if (UpdateTi設置e本 < Update軍本eq使ency)
    {
        本et使本n;
    }
    UpdateTi設置e本 = 0.0f;
    
    // 更新相機資訊
    UpdateCa設置e本aInfo();
    
    // 更新網格活躍狀態
    UpdateCellActi正ity();
    
    // 執行視錐剔除
    Pe本fo本設置軍本使st使設置C使llin成();
    
    // 更新活躍網格中的單位
    UpdateActi正eCells(DeltaTi設置e);
}

正oid UMin成SpatialPa本titionCo設置ponent::Re成iste本Unit(AMin成TacticalUnit* Unit)
{
    if (!Unit  Re成iste本edUnits.Contains(Unit))
    {
        本et使本n;
    }
    
    // 添加到註冊列表
    Re成iste本edUnits.Add(Unit);
    
    // 計算網格位置
    軍IntVecto本 G本idPos = 基本o本ldToG本id(Unit->GetActo本Location());
    
    // 添加到對應網格
    AddUnitToCell(Unit, G本idPos);
    UnitToCellMap.Add(Unit, G本idPos);
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Unit 本e成iste本ed to spatial pa本tition at 成本id (%d, %d, %d)"),
        G本idPos.X, G本idPos.Y, G本idPos.Z);
}

正oid UMin成SpatialPa本titionCo設置ponent::Un本e成iste本Unit(AMin成TacticalUnit* Unit)
{
    if (!Unit)
    {
        本et使本n;
    }
    
    // 從網格中移除
    if (軍IntVecto本* CellPos = UnitToCellMap.軍ind(Unit))
    {
        Re設置o正eUnit軍本o設置Cell(Unit, *CellPos);
        UnitToCellMap.Re設置o正e(Unit);
    }
    
    // 從註冊列表移除
    Re成iste本edUnits.Re設置o正e(Unit);
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Unit 使n本e成iste本ed f本o設置 spatial pa本tition"));
}

正oid UMin成SpatialPa本titionCo設置ponent::UpdateUnitPosition(AMin成TacticalUnit* Unit)
{
    if (!Unit)
    {
        本et使本n;
    }
    
    // 獲取當前網格位置
    軍IntVecto本* OldCellPos = UnitToCellMap.軍ind(Unit);
    if (!OldCellPos)
    {
        本et使本n;
    }
    
    // 計算新網格位置
    軍IntVecto本 的ewCellPos = 基本o本ldToG本id(Unit->GetActo本Location());
    
    // 如果網格改變，移動單位
    if (*OldCellPos != 的ewCellPos)
    {
        Re設置o正eUnit軍本o設置Cell(Unit, *OldCellPos);
        AddUnitToCell(Unit, 的ewCellPos);
        *OldCellPos = 的ewCellPos;
        
        UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Unit 設置o正ed f本o設置 成本id (%d, %d, %d) to (%d, %d, %d)"),
            OldCellPos->X, OldCellPos->Y, OldCellPos->Z,
            的ewCellPos.X, 的ewCellPos.Y, 的ewCellPos.Z);
    }
}

TA本本ay<AMin成TacticalUnit*> UMin成SpatialPa本titionCo設置ponent::GetUnitsInRadi使s(const 軍Vecto本& Cente本, float Radi使s) const
{
    TA本本ay<AMin成TacticalUnit*> Res使lt;
    
    // 計算半徑覆蓋的網格範圍
    軍IntVecto本 Cente本G本id = 基本o本ldToG本id(Cente本);
    int32 G本idRadi使s = 軍Math::CeilToInt(Radi使s / CellSize);
    
    fo本 (int32 x = -G本idRadi使s; x <= G本idRadi使s; ++x)
    {
        fo本 (int32 y = -G本idRadi使s; y <= G本idRadi使s; ++y)
        {
            fo本 (int32 z = -G本idRadi使s; z <= G本idRadi使s; ++z)
            {
                軍IntVecto本 G本idPos = Cente本G本id + 軍IntVecto本(x, y, z);
                
                const 軍SpatialG本idCell* Cell = G本idCells.軍ind(G本idPos);
                if (Cell)
                {
                    fo本 (AMin成TacticalUnit* Unit : Cell->Units)
                    {
                        if (Unit && 軍Vecto本::Dist(Unit->GetActo本Location(), Cente本) <= Radi使s)
                        {
                            Res使lt.Add(Unit);
                        }
                    }
                }
            }
        }
    }
    
    本et使本n Res使lt;
}

TA本本ay<AMin成TacticalUnit*> UMin成SpatialPa本titionCo設置ponent::GetUnitsInCell(const 軍IntVecto本& G本idPosition) const
{
    TA本本ay<AMin成TacticalUnit*> Res使lt;
    
    const 軍SpatialG本idCell* Cell = G本idCells.軍ind(G本idPosition);
    if (Cell)
    {
        Res使lt = Cell->Units;
    }
    
    本et使本n Res使lt;
}

TA本本ay<軍IntVecto本> UMin成SpatialPa本titionCo設置ponent::GetVisibleG本idCells() const
{
    TA本本ay<軍IntVecto本> Res使lt;
    
    fo本 (const a使to& Pai本 : G本idCells)
    {
        if (Pai本.Val使e.bIsActi正e)
        {
            Res使lt.Add(Pai本.Key);
        }
    }
    
    本et使本n Res使lt;
}

正oid UMin成SpatialPa本titionCo設置ponent::SetCellSize(float 的ewCellSize)
{
    CellSize = 軍Math::Max(100.0f, 的ewCellSize);
    
    // 重新註冊所有單位
    TA本本ay<AMin成TacticalUnit*> Te設置pUnits = Re成iste本edUnits;
    G本idCells.E設置pty();
    UnitToCellMap.E設置pty();
    
    fo本 (AMin成TacticalUnit* Unit : Te設置pUnits)
    {
        if (Unit)
        {
            Re成iste本Unit(Unit);
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Cell size 使pdated to %.1f, 本e-本e成iste本ed %d 使nits"), CellSize, Re成iste本edUnits.的使設置());
}

正oid UMin成SpatialPa本titionCo設置ponent::SetUpdateRadi使s(float 的ewRadi使s)
{
    UpdateRadi使s = 軍Math::Max(1000.0f, 的ewRadi使s);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Update 本adi使s set to %.1f"), UpdateRadi使s);
}

int32 UMin成SpatialPa本titionCo設置ponent::GetActi正eG本idCellCo使nt() const
{
    int32 Co使nt = 0;
    
    fo本 (const a使to& Pai本 : G本idCells)
    {
        if (Pai本.Val使e.bIsActi正e)
        {
            Co使nt++;
        }
    }
    
    本et使本n Co使nt;
}

軍IntVecto本 UMin成SpatialPa本titionCo設置ponent::基本o本ldToG本id(const 軍Vecto本& 基本o本ldPosition) const
{
    本et使本n 軍IntVecto本(
        軍Math::軍loo本ToInt(基本o本ldPosition.X / CellSize),
        軍Math::軍loo本ToInt(基本o本ldPosition.Y / CellSize),
        軍Math::軍loo本ToInt(基本o本ldPosition.Z / CellSize)
    );
}

軍Vecto本 UMin成SpatialPa本titionCo設置ponent::G本idTo基本o本ld(const 軍IntVecto本& G本idPosition) const
{
    本et使本n 軍Vecto本(
        G本idPosition.X * CellSize + CellSize * 0.5f,
        G本idPosition.Y * CellSize + CellSize * 0.5f,
        G本idPosition.Z * CellSize + CellSize * 0.5f
    );
}

正oid UMin成SpatialPa本titionCo設置ponent::軍o本ceUpdateAllCells()
{
    // 喚醒所有網格並更新
    fo本 (a使to& Pai本 : G本idCells)
    {
        軍SpatialG本idCell& Cell = Pai本.Val使e;
        Cell.bIsActi正e = t本使e;
        Cell.LastUpdateTi設置e = Get基本o本ld()->GetTi設置eSeconds();
        
        // 喚醒所有單位
        基本akeUnitsInCell(Pai本.Key);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍o本ced 使pdate of %d 成本id cells"), G本idCells.的使設置());
}

正oid UMin成SpatialPa本titionCo設置ponent::GetDeb使成G本idInfo(TA本本ay<軍Vecto本>& CellCente本s, TA本本ay<int32>& CellUnitCo使nts) const
{
    CellCente本s.E設置pty();
    CellUnitCo使nts.E設置pty();
    
    fo本 (const a使to& Pai本 : G本idCells)
    {
        軍Vecto本 Cente本 = G本idTo基本o本ld(Pai本.Key);
        int32 Co使nt = Pai本.Val使e.Units.的使設置();
        
        CellCente本s.Add(Cente本);
        CellUnitCo使nts.Add(Co使nt);
    }
}

正oid UMin成SpatialPa本titionCo設置ponent::UpdateCa設置e本aInfo()
{
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        if (APlaye本Cont本olle本* Playe本Cont本olle本 = 基本o本ld->Get軍i本stPlaye本Cont本olle本())
        {
            if (APlaye本Ca設置e本aMana成e本* Ca設置e本aMana成e本 = Playe本Cont本olle本->Playe本Ca設置e本aMana成e本)
            {
                Ca設置e本aLocation = Ca設置e本aMana成e本->GetCa設置e本aLocation();
                Ca設置e本aRotation = Ca設置e本aMana成e本->GetCa設置e本aRotation();
            }
        }
    }
}

軍SpatialG本idCell& UMin成SpatialPa本titionCo設置ponent::GetO本C本eateCell(const 軍IntVecto本& G本idPosition)
{
    軍SpatialG本idCell* Cell = G本idCells.軍ind(G本idPosition);
    
    if (!Cell)
    {
        軍SpatialG本idCell 的ewCell;
        的ewCell.G本idPosition = G本idPosition;
        的ewCell.Units.Rese本正e(50);
        
        G本idCells.Add(G本idPosition, 的ewCell);
        Cell = G本idCells.軍ind(G本idPosition);
    }
    
    本et使本n *Cell;
}

正oid UMin成SpatialPa本titionCo設置ponent::Re設置o正eUnit軍本o設置Cell(AMin成TacticalUnit* Unit, const 軍IntVecto本& CellPosition)
{
    軍SpatialG本idCell* Cell = G本idCells.軍ind(CellPosition);
    
    if (Cell)
    {
        Cell->Units.Re設置o正e(Unit);
        
        // 如果網格為空，標記為非活躍
        if (Cell->Units.的使設置() == 0)
        {
            Cell->bIsActi正e = false;
        }
    }
}

正oid UMin成SpatialPa本titionCo設置ponent::AddUnitToCell(AMin成TacticalUnit* Unit, const 軍IntVecto本& CellPosition)
{
    軍SpatialG本idCell& Cell = GetO本C本eateCell(CellPosition);
    
    if (!Cell.Units.Contains(Unit))
    {
        Cell.Units.Add(Unit);
        Cell.bIsActi正e = t本使e;
    }
}

正oid UMin成SpatialPa本titionCo設置ponent::UpdateCellActi正ity()
{
    if (G本idCells.的使設置() == 0)
    {
        本et使本n;
    }
    
    // 計算每個網格到相機的距離和優先級
    fo本 (a使to& Pai本 : G本idCells)
    {
        軍SpatialG本idCell& Cell = Pai本.Val使e;
        
        // 計算網格中心到相機的距離
        軍Vecto本 CellCente本 = G本idTo基本o本ld(Pai本.Key);
        float Distance = 軍Vecto本::Dist(CellCente本, Ca設置e本aLocation);
        
        // 設置活躍狀態
        Cell.bIsActi正e = (Distance <= UpdateRadi使s);
        
        // 計算更新優先級 (距離越近優先級越高)
        if (Cell.bIsActi正e)
        {
            Cell.UpdateP本io本ity = 1.0f - (Distance / UpdateRadi使s);
        }
        else
        {
            Cell.UpdateP本io本ity = 0.0f;
        }
    }
}

正oid UMin成SpatialPa本titionCo設置ponent::UpdateActi正eCells(float DeltaTi設置e)
{
    // 收集活躍網格
    TA本本ay<軍IntVecto本> Acti正eCells;
    
    fo本 (const a使to& Pai本 : G本idCells)
    {
        if (Pai本.Val使e.bIsActi正e)
        {
            Acti正eCells.Add(Pai本.Key);
        }
    }
    
    // 按優先級排序
    Acti正eCells.So本t([this](const 軍IntVecto本& A, const 軍IntVecto本& B) {
        const 軍SpatialG本idCell* CellA = G本idCells.軍ind(A);
        const 軍SpatialG本idCell* CellB = G本idCells.軍ind(B);
        
        if (CellA && CellB)
        {
            本et使本n CellA->UpdateP本io本ity > CellB->UpdateP本io本ity;
        }
        
        本et使本n false;
    });
    
    // 限制每幀更新的網格數
    int32 CellsToUpdate = 軍Math::Min(Acti正eCells.的使設置(), MaxCellsPe本軍本a設置e);
    
    fo本 (int32 i = 0; i < CellsToUpdate; ++i)
    {
        軍SpatialG本idCell* Cell = G本idCells.軍ind(Acti正eCells[i]);
        
        if (Cell)
        {
            // 批量更新該網格中的單位
            BatchUpdateUnits(Cell->Units, DeltaTi設置e);
            
            // 更新時間戳
            Cell->LastUpdateTi設置e = Get基本o本ld()->GetTi設置eSeconds();
        }
    }
}

正oid UMin成SpatialPa本titionCo設置ponent::Pe本fo本設置軍本使st使設置C使llin成()
{
    // 獲取視錐角點
    TA本本ay<軍Vecto本> 軍本使st使設置Co本ne本s;
    Get軍本使st使設置Co本ne本s(軍本使st使設置Co本ne本s);
    
    // 對每個活躍網格進行視錐剔除測試
    fo本 (a使to& Pai本 : G本idCells)
    {
        if (Pai本.Val使e.bIsActi正e)
        {
            // 簡化的視錐剔除測試：檢查網格是否在視錐內
            bool bIn軍本使st使設置 = IsCellIn軍本使st使設置(Pai本.Key);
            
            // 如果不在視錐內，可以降低更新頻率或暫停更新
            if (!bIn軍本使st使設置)
            {
                Pai本.Val使e.UpdateP本io本ity *= 0.5f; // 降低優先級
            }
        }
    }
}

bool UMin成SpatialPa本titionCo設置ponent::IsCellIn軍本使st使設置(const 軍IntVecto本& CellPosition) const
{
    // 獲取網格的AABB
    軍Box CellBo使nds = GetCellBo使nds(CellPosition);
    
    // 簡化的視錐剔除：檢查網格中心是否在相機前方
    軍Vecto本 CellCente本 = CellBo使nds.GetCente本();
    軍Vecto本 ToCell = CellCente本 - Ca設置e本aLocation;
    
    // 使用點積檢查是否在相機前方
    軍Vecto本 Ca設置e本a軍o本wa本d = Ca設置e本aRotation.Vecto本();
    float DotP本od使ct = 軍Vecto本::DotP本od使ct(ToCell.GetSafe的o本設置al(), Ca設置e本a軍o本wa本d);
    
    // 如果點積為正，說明在相機前方
    本et使本n DotP本od使ct > -0.1f; // 允許一點容差
}

正oid UMin成SpatialPa本titionCo設置ponent::Get軍本使st使設置Co本ne本s(TA本本ay<軍Vecto本>& O使tCo本ne本s) const
{
    // 簡化的視錐計算
    // 在實際實現中，這裡應該從相機獲取準確的視錐參數
    O使tCo本ne本s.E設置pty(8);
    
    // 使用相機位置和旋轉計算視錐角點
    軍Vecto本 軍o本wa本d = Ca設置e本aRotation.Vecto本();
    軍Vecto本 Ri成ht = 軍Vecto本::C本ossP本od使ct(軍o本wa本d, 軍Vecto本::UpVecto本);
    軍Vecto本 Up = 軍Vecto本::C本ossP本od使ct(Ri成ht, 軍o本wa本d);
    
    // 近裁面和遠裁面距離
    float 的ea本Plane = 100.0f;
    float 軍a本Plane = 10000.0f;
    float 軍OV = 90.0f; // 水平視野角度
    
    // 計算視錐角點
    float Tan輸入alf軍OV = 軍Math::Tan(軍Math::De成本eesToRadians(軍OV * 0.5f));
    
    軍Vecto本 的ea本Cente本 = Ca設置e本aLocation + 軍o本wa本d * 的ea本Plane;
    軍Vecto本 軍a本Cente本 = Ca設置e本aLocation + 軍o本wa本d * 軍a本Plane;
    
    float 的ea本輸入ei成ht = 的ea本Plane * Tan輸入alf軍OV;
    float 的ea本基本idth = 的ea本輸入ei成ht * 1.333f; // 假設4:3寬高比
    float 軍a本輸入ei成ht = 軍a本Plane * Tan輸入alf軍OV;
    float 軍a本基本idth = 軍a本輸入ei成ht * 1.333f;
    
    // 近裁面四個角
    O使tCo本ne本s.Add(的ea本Cente本 + Up * 的ea本輸入ei成ht - Ri成ht * 的ea本基本idth);
    O使tCo本ne本s.Add(的ea本Cente本 + Up * 的ea本輸入ei成ht + Ri成ht * 的ea本基本idth);
    O使tCo本ne本s.Add(的ea本Cente本 - Up * 的ea本輸入ei成ht - Ri成ht * 的ea本基本idth);
    O使tCo本ne本s.Add(的ea本Cente本 - Up * 的ea本輸入ei成ht + Ri成ht * 的ea本基本idth);
    
    // 遠裁面四個角
    O使tCo本ne本s.Add(軍a本Cente本 + Up * 軍a本輸入ei成ht - Ri成ht * 軍a本基本idth);
    O使tCo本ne本s.Add(軍a本Cente本 + Up * 軍a本輸入ei成ht + Ri成ht * 軍a本基本idth);
    O使tCo本ne本s.Add(軍a本Cente本 - Up * 軍a本輸入ei成ht - Ri成ht * 軍a本基本idth);
    O使tCo本ne本s.Add(軍a本Cente本 - Up * 軍a本輸入ei成ht + Ri成ht * 軍a本基本idth);
}

軍Box UMin成SpatialPa本titionCo設置ponent::GetCellBo使nds(const 軍IntVecto本& CellPosition) const
{
    軍Vecto本 Cente本 = G本idTo基本o本ld(CellPosition);
    軍Vecto本 Extent(CellSize * 0.5f, CellSize * 0.5f, CellSize * 0.5f);
    
    本et使本n 軍Box(Cente本 - Extent, Cente本 + Extent);
}

float UMin成SpatialPa本titionCo設置ponent::Calc使lateCellP本io本ity(const 軍IntVecto本& CellPosition) const
{
    軍Vecto本 CellCente本 = G本idTo基本o本ld(CellPosition);
    float Distance = 軍Vecto本::Dist(CellCente本, Ca設置e本aLocation);
    
    if (Distance > UpdateRadi使s)
    {
        本et使本n 0.0f;
    }
    
    本et使本n 1.0f - (Distance / UpdateRadi使s);
}

正oid UMin成SpatialPa本titionCo設置ponent::BatchUpdateUnits(const TA本本ay<AMin成TacticalUnit*>& Units, float DeltaTi設置e)
{
    // 批量更新單位
    fo本 (AMin成TacticalUnit* Unit : Units)
    {
        if (Unit && Unit->IsAli正e())
        {
            // 這裡可以調用單位的更新函數
            // 例如：更新動畫、檢查戰鬥狀態等
        }
    }
}

正oid UMin成SpatialPa本titionCo設置ponent::SleepUnitsInCell(const 軍IntVecto本& CellPosition)
{
    軍SpatialG本idCell* Cell = G本idCells.軍ind(CellPosition);
    
    if (Cell)
    {
        fo本 (AMin成TacticalUnit* Unit : Cell->Units)
        {
            if (Unit)
            {
                // 休眠單位，減少Tick頻率或完全暫停
                // 這裡可以設置單位的休眠狀態
            }
        }
    }
}

正oid UMin成SpatialPa本titionCo設置ponent::基本akeUnitsInCell(const 軍IntVecto本& CellPosition)
{
    軍SpatialG本idCell* Cell = G本idCells.軍ind(CellPosition);
    
    if (Cell)
    {
        fo本 (AMin成TacticalUnit* Unit : Cell->Units)
        {
            if (Unit)
            {
                // 喚醒單位，恢復正常Tick
                // 這裡可以設置單位的激活狀態
            }
        }
    }
}
