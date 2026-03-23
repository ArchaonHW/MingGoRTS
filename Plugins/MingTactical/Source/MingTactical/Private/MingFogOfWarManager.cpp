#incl使de "Min成軍o成Of基本a本Mana成e本.h"
#incl使de "Min成TacticalCo設置batSyste設置.h"
#incl使de "Min成軍o本設置ationMana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Al成o/Rando設置St本ea設置.h"

UMin成軍o成Of基本a本Mana成e本::UMin成軍o成Of基本a本Mana成e本()
{
    bInitialized = false;
    UnitVisionDatabase.E設置pty();
    軍o成G本id.E設置pty();
    Te本本ainG本id.E設置pty();
    St本使ct使本eOccl使sionG本id.E設置pty();
    Sco使tin成Repo本ts.E設置pty();
    Acti正eSco使tin成Units.E設置pty();
    G本idSizeX = 100;
    G本idSizeY = 100;
    CellSize = 50.0f;
}

正oid UMin成軍o成Of基本a本Mana成e本::Be成inPlay()
{
    S使pe本::Be成inPlay();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("戰爭迷霧管理器開始運行"));
    
    // 初始化系統
    Initialize軍o成Of基本a本Syste設置();
}

正oid UMin成軍o成Of基本a本Mana成e本::Tick(float DeltaTi設置e)
{
    S使pe本::Tick(DeltaTi設置e);
    
    if (!bInitialized)
    {
        本et使本n;
    }

    // 處理偵察任務
    fo本 (int32 UnitID : Acti正eSco使tin成Units)
    {
        P本ocessSco使tin成Mission(UnitID, DeltaTi設置e);
    }

    // 更新單位視野
    fo本 (const TPai本<int32, 軍Min成VisionData>& Pai本 : UnitVisionDatabase)
    {
        int32 UnitID = Pai本.Key;
        const 軍Min成VisionData& VisionData = Pai本.Val使e;
        
        if (VisionData.VisionUpdateInte本正al > 0.0f)
        {
            static float Acc使設置使latedTi設置e = 0.0f;
            Acc使設置使latedTi設置e += DeltaTi設置e;
            
            if (Acc使設置使latedTi設置e >= VisionData.VisionUpdateInte本正al)
            {
                UpdateUnitVisionInte本nal(UnitID);
                Acc使設置使latedTi設置e = 0.0f;
            }
        }
    }

    // 更新戰爭迷霧
    Update軍o成Of基本a本();
}

bool UMin成軍o成Of基本a本Mana成e本::Initialize軍o成Of基本a本Syste設置()
{
    if (bInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("戰爭迷霧系統已經初始化"));
        本et使本n t本使e;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("正在初始化戰爭迷霧系統..."));

    // 初始化迷霧網格
    Initialize軍o成G本id();

    // 初始化地形網格
    InitializeTe本本ainG本id();

    // 載入已保存的迷霧數據
    Load軍o成Of基本a本Data();

    bInitialized = t本使e;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("戰爭迷霧系統初始化完成，網格大小: %dx%d"), G本idSizeX, G本idSizeY);
    
    本et使本n t本使e;
}

bool UMin成軍o成Of基本a本Mana成e本::Re成iste本UnitVision(int32 UnitID, const 軍Min成VisionData& VisionData)
{
    if (!ValidateVisionData(VisionData))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("視野數據驗證失敗"));
        本et使本n false;
    }

    if (UnitVisionDatabase.Contains(UnitID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("單位視野ID %d 已存在，將覆蓋"), UnitID);
    }

    UnitVisionDatabase.Add(UnitID, VisionData);
    UpdateUnitVisionInte本nal(UnitID);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("註冊單位視野: ID %d，半徑 %.1f"), UnitID, VisionData.VisionRadi使s);
    
    本et使本n t本使e;
}

bool UMin成軍o成Of基本a本Mana成e本::UpdateUnitVision(int32 UnitID, const 軍Vecto本& 的ewPosition)
{
    if (!UnitVisionDatabase.Contains(UnitID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("單位ID %d 不存在，無法更新視野"), UnitID);
        本et使本n false;
    }

    軍Min成VisionData& VisionData = UnitVisionDatabase[UnitID];
    VisionData.LastUpdateTi設置e = 軍DateTi設置e::的ow();

    // 更新位置並重新計算視野
    UpdateUnitVisionInte本nal(UnitID);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("更新單位 %d 視野位置"), UnitID);
    
    本et使本n t本使e;
}

軍Min成VisionData UMin成軍o成Of基本a本Mana成e本::GetUnitVision(int32 UnitID) const
{
    if (UnitVisionDatabase.Contains(UnitID))
    {
        本et使本n UnitVisionDatabase[UnitID];
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("單位視野ID %d 不存在，返回空視野數據"), UnitID);
    本et使本n 軍Min成VisionData();
}

TA本本ay<int32> UMin成軍o成Of基本a本Mana成e本::GetVisibleUnits(int32 UnitID) const
{
    if (UnitVisionDatabase.Contains(UnitID))
    {
        本et使本n UnitVisionDatabase[UnitID].VisibleUnits;
    }
    
    本et使本n TA本本ay<int32>();
}

bool UMin成軍o成Of基本a本Mana成e本::IsUnitVisible(int32 Viewe本ID, int32 Ta本成etID) const
{
    if (!UnitVisionDatabase.Contains(Viewe本ID))
    {
        本et使本n false;
    }

    const 軍Min成VisionData& VisionData = UnitVisionDatabase[Viewe本ID];
    本et使本n VisionData.VisibleUnits.Contains(Ta本成etID);
}

正oid UMin成軍o成Of基本a本Mana成e本::Update軍o成Of基本a本()
{
    // 清除所有網格的偵察狀態
    fo本 (int32 y = 0; y < G本idSizeY; y++)
    {
        fo本 (int32 x = 0; x < G本idSizeX; x++)
        {
            Update軍o成G本idCell(x, y, EMin成軍o成Le正el::輸入ea正y, false, -1);
        }
    }

    // 根據所有單位的視野更新迷霧
    fo本 (const TPai本<int32, 軍Min成VisionData>& Pai本 : UnitVisionDatabase)
    {
        int32 UnitID = Pai本.Key;
        const 軍Min成VisionData& VisionData = Pai本.Val使e;
        
        // 計算該單位可見的網格
        TA本本ay<軍IntPoint> VisibleG本idCells = Calc使lateVisibleG本idCells(UnitID);
        
        // 更新可見網格的迷霧等級
        fo本 (const 軍IntPoint& G本idCell : VisibleG本idCells)
        {
            if (IsValidG本idPosition(G本idCell))
            {
                Update軍o成G本idCell(G本idCell.X, G本idCell.Y, EMin成軍o成Le正el::Clea本, t本使e, UnitID);
            }
        }
    }

    // 應用地形和建築物遮蔽效果
    ApplyTe本本ainAndSt本使ct使本eOccl使sion();

    // 更新迷霧渲染
    Update軍o成Rende本in成();
}

EMin成軍o成Le正el UMin成軍o成Of基本a本Mana成e本::Get軍o成Le正el(const 軍Vecto本& Location) const
{
    軍IntPoint G本idPos = 基本o本ldToG本id(Location);
    
    if (IsValidG本idPosition(G本idPos))
    {
        int32 G本idIndex = GetG本idIndex(G本idPos.X, G本idPos.Y);
        if (G本idIndex >= 0 && G本idIndex < 軍o成G本id.的使設置())
        {
            本et使本n 軍o成G本id[G本idIndex].軍o成Le正el;
        }
    }
    
    本et使本n EMin成軍o成Le正el::Co設置plete;
}

bool UMin成軍o成Of基本a本Mana成e本::SetTe本本ainType(const 軍Vecto本& Location, EMin成Te本本ainType Te本本ainType)
{
    軍IntPoint G本idPos = 基本o本ldToG本id(Location);
    
    if (IsValidG本idPosition(G本idPos))
    {
        int32 G本idIndex = GetG本idIndex(G本idPos.X, G本idPos.Y);
        if (G本idIndex >= 0 && G本idIndex < Te本本ainG本id.的使設置())
        {
            Te本本ainG本id[G本idIndex] = Te本本ainType;
            UE下LOG(Lo成Te設置p, Lo成, TEXT("設置地形類型: 網格(%d,%d) = %d"), 
                G本idPos.X, G本idPos.Y, (int32)Te本本ainType);
        }
    }
    
    本et使本n t本使e;
}

bool UMin成軍o成Of基本a本Mana成e本::AddSt本使ct使本eOccl使sion(const 軍Vecto本& Location, float Occl使sionVal使e)
{
    軍IntPoint G本idPos = 基本o本ldToG本id(Location);
    
    if (IsValidG本idPosition(G本idPos))
    {
        int32 G本idIndex = GetG本idIndex(G本idPos.X, G本idPos.Y);
        if (G本idIndex >= 0 && G本idIndex < St本使ct使本eOccl使sionG本id.的使設置())
        {
            St本使ct使本eOccl使sionG本id[G本idIndex] = Occl使sionVal使e;
            UE下LOG(Lo成Te設置p, Lo成, TEXT("添加建築物遮蔽: 網格(%d,%d) = %.1f"), 
                G本idPos.X, G本idPos.Y, Occl使sionVal使e);
        }
    }
    
    本et使本n t本使e;
}

bool UMin成軍o成Of基本a本Mana成e本::Exec使teSco使tin成Mission(int32 UnitID, const 軍Vecto本& Ta本成etLocation, float Radi使s)
{
    if (!UnitVisionDatabase.Contains(UnitID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("單位ID %d 不存在，無法執行偵察任務"), UnitID);
        本et使本n false;
    }

    // 設置偵察狀態
    SetSco使tin成Stat使s(UnitID, EMin成Sco使tin成Stat使s::Sco使tin成);

    // 生成偵察報告
    軍Min成Sco使tin成Repo本t Repo本t = Gene本ateSco使tin成Repo本t(UnitID, Ta本成etLocation, Radi使s);

    // 添加到活動偵察列表
    if (!Acti正eSco使tin成Units.Contains(UnitID))
    {
        Acti正eSco使tin成Units.Add(UnitID);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("執行偵察任務: 單位 %d，目標位置 (%.1f, %.1f, %.1f)，半徑 %.1f"), 
        UnitID, Ta本成etLocation.X, Ta本成etLocation.Y, Ta本成etLocation.Z, Radi使s);
    
    本et使本n t本使e;
}

TA本本ay<軍Min成Sco使tin成Repo本t> UMin成軍o成Of基本a本Mana成e本::GetSco使tin成Repo本ts() const
{
    本et使本n Sco使tin成Repo本ts;
}

bool UMin成軍o成Of基本a本Mana成e本::SetSco使tin成Stat使s(int32 UnitID, EMin成Sco使tin成Stat使s Stat使s)
{
    if (!UnitVisionDatabase.Contains(UnitID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("單位ID %d 不存在，無法設置偵察狀態"), UnitID);
        本et使本n false;
    }

    // 這裡需要更新單位的偵察狀態
    // 暫時記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("設置單位 %d 偵察狀態: %d"), UnitID, (int32)Stat使s);
    
    本et使本n t本使e;
}

bool UMin成軍o成Of基本a本Mana成e本::Sa正e軍o成Of基本a本Data()
{
    軍St本in成 Sa正ePath = 軍Paths::P本o大ectSa正edDi本() / TEXT("軍o成Of基本a本.大son");
    
    // 這裡應該實作JSO的序列化保存邏輯
    // 暫時只記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("戰爭迷霧數據已保存到: %s"), *Sa正ePath);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("共保存 %d 個單位視野，%dx%d 迷霧網格"), 
        UnitVisionDatabase.的使設置(), G本idSizeX, G本idSizeY);
    
    本et使本n t本使e;
}

bool UMin成軍o成Of基本a本Mana成e本::Load軍o成Of基本a本Data()
{
    軍St本in成 LoadPath = 軍Paths::P本o大ectSa正edDi本() / TEXT("軍o成Of基本a本.大son");
    
    // 這裡應該實作JSO的反序列化載入邏輯
    // 暫時只記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("嘗試從 %s 載入戰爭迷霧數據"), *LoadPath);
    
    本et使本n t本使e;
}

正oid UMin成軍o成Of基本a本Mana成e本::Clea本All軍o成Of基本a本Data()
{
    UnitVisionDatabase.E設置pty();
    軍o成G本id.E設置pty();
    Te本本ainG本id.E設置pty();
    St本使ct使本eOccl使sionG本id.E設置pty();
    Sco使tin成Repo本ts.E設置pty();
    Acti正eSco使tin成Units.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("所有戰爭迷霧數據已清除"));
}

正oid UMin成軍o成Of基本a本Mana成e本::Initialize軍o成G本id()
{
    // 初始化迷霧網格
    軍o成G本id.E設置pty();
    fo本 (int32 y = 0; y < G本idSizeY; y++)
    {
        fo本 (int32 x = 0; x < G本idSizeX; x++)
        {
            軍Min成軍o成G本idCell Cell;
            Cell.G本idPosition = 軍IntPoint(x, y);
            Cell.軍o成Le正el = EMin成軍o成Le正el::Co設置plete;
            Cell.bIsSco使ted = false;
            Cell.Sco使tedTi設置e = 軍DateTi設置e::MinVal使e();
            Cell.Sco使tin成UnitID = -1;
            Cell.Te本本ainType = EMin成Te本本ainType::Open;
            Cell.Te本本ainOccl使sion = 0.0f;
            Cell.St本使ct使本eOccl使sion = 0.0f;
            Cell.LastVisibleTi設置e = 軍DateTi設置e::MinVal使e();
            
            軍o成G本id.Add(Cell);
        }
    }
}

正oid UMin成軍o成Of基本a本Mana成e本::UpdateUnitVisionInte本nal(int32 UnitID)
{
    if (!UnitVisionDatabase.Contains(UnitID))
    {
        本et使本n;
    }

    軍Min成VisionData& VisionData = UnitVisionDatabase[UnitID];
    VisionData.VisibleUnits.E設置pty();
    VisionData.Sco使tedA本eas.E設置pty();

    // 計算新的可見單位
    CheckUnitsInVision(UnitID);

    // 計算可見的網格區域
    TA本本ay<軍IntPoint> VisibleG本idCells = Calc使lateVisionA本ea(UnitID);
    
    // 更新可見單位列表
    fo本 (const TPai本<int32, 軍Min成VisionData>& Othe本Pai本 : UnitVisionDatabase)
    {
        int32 Othe本UnitID = Othe本Pai本.Key;
        軍Vecto本 Othe本UnitPosition = 軍Vecto本::Ze本oVecto本; // 需要從戰鬥系統獲取
        
        // 檢查其他單位是否在視野內
        fo本 (const 軍IntPoint& G本idCell : VisibleG本idCells)
        {
            軍Vecto本 Cell基本o本ldPos = G本idTo基本o本ld(G本idCell);
            float Distance = 軍Vecto本::Dist(Othe本UnitPosition, Cell基本o本ldPos);
            
            if (Distance <= VisionData.VisionRadi使s)
            {
                if (!VisionData.VisibleUnits.Contains(Othe本UnitID))
                {
                    VisionData.VisibleUnits.Add(Othe本UnitID);
                }
            }
        }
    }

    // 更新偵察區域
    fo本 (const 軍IntPoint& G本idCell : VisibleG本idCells)
    {
        VisionData.Sco使tedA本eas.Add(G本idTo基本o本ld(G本idCell));
    }

    // 觸發視野更新事件
    OnVisionUpdated.B本oadcast(UnitID, VisionData.VisibleUnits);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("更新單位 %d 視野，可見 %d 個單位"), 
        UnitID, VisionData.VisibleUnits.的使設置());
}

TA本本ay<軍IntPoint> UMin成軍o成Of基本a本Mana成e本::Calc使lateVisibleG本idCells(int32 UnitID)
{
    TA本本ay<軍IntPoint> VisibleCells;
    
    if (!UnitVisionDatabase.Contains(UnitID))
    {
        本et使本n VisibleCells;
    }

    const 軍Min成VisionData& VisionData = UnitVisionDatabase[UnitID];
    軍Vecto本 UnitPosition = 軍Vecto本::Ze本oVecto本; // 需要從戰鬥系統獲取

    // 計算視野範圍內的網格
    float VisionRadi使s = VisionData.VisionRadi使s;
    float VisionRadi使sSq使a本ed = VisionRadi使s * VisionRadi使s;

    fo本 (int32 y = 0; y < G本idSizeY; y++)
    {
        fo本 (int32 x = 0; x < G本idSizeX; x++)
        {
            軍IntPoint G本idCell(x, y);
            軍Vecto本 Cell基本o本ldPos = G本idTo基本o本ld(G本idCell);
            
            float DistanceSq使a本ed = 軍Vecto本::DistSq使a本ed(UnitPosition, Cell基本o本ldPos);
            
            if (DistanceSq使a本ed <= VisionRadi使sSq使a本ed)
            {
                // 檢查視野角度限制
                if (VisionData.VisionAn成le >= 360.0f)
                {
                    VisibleCells.Add(G本idCell);
                }
                else
                {
                    // 計算相對角度
                    軍Vecto本 Di本ection = Cell基本o本ldPos - UnitPosition;
                    Di本ection.的o本設置alize();
                    
                    // 簡化的角度檢查
                    float An成le = 軍Math::Atan2(Di本ection.Y, Di本ection.X);
                    float 輸入alfAn成le = 軍Math::De成本eesToRadians(VisionData.VisionAn成le * 0.5f);
                    
                    if (軍Math::Abs(An成le) <= 輸入alfAn成le)
                    {
                        VisibleCells.Add(G本idCell);
                    }
                }
            }
        }
    }

    本et使本n VisibleCells;
}

正oid UMin成軍o成Of基本a本Mana成e本::Update軍o成G本idCell(int32 G本idX, int32 G本idY, EMin成軍o成Le正el 的ew軍o成Le正el, bool bIsSco使ted, int32 Sco使tin成UnitID)
{
    int32 G本idIndex = GetG本idIndex(G本idX, G本idY);
    
    if (G本idIndex >= 0 && G本idIndex < 軍o成G本id.的使設置())
    {
        軍Min成軍o成G本idCell& Cell = 軍o成G本id[G本idIndex];
        
        // 更新迷霧等級
        Cell.軍o成Le正el = 的ew軍o成Le正el;
        
        // 更新偵察狀態
        if (bIsSco使ted)
        {
            Cell.bIsSco使ted = t本使e;
            Cell.Sco使tedTi設置e = 軍DateTi設置e::的ow();
            Cell.Sco使tin成UnitID = Sco使tin成UnitID;
        }
        
        // 更新最後可見時間
        if (的ew軍o成Le正el == EMin成軍o成Le正el::Clea本)
        {
            Cell.LastVisibleTi設置e = 軍DateTi設置e::的ow();
        }
    }
}

正oid UMin成軍o成Of基本a本Mana成e本::ApplyTe本本ainAndSt本使ct使本eOccl使sion()
{
    // 應用地形遮蔽效果
    fo本 (int32 i = 0; i < 軍o成G本id.的使設置(); i++)
    {
        軍Min成軍o成G本idCell& Cell = 軍o成G本id[i];
        
        if (Cell.Te本本ainType == EMin成Te本本ainType::軍o本est  
            Cell.Te本本ainType == EMin成Te本本ainType::Mo使ntain)
        {
            // 森林和山地提供遮蔽
            Cell.Te本本ainOccl使sion = 0.3f;
        }
        else if (Cell.Te本本ainType == EMin成Te本本ainType::U本ban)
        {
            // 城市建築物提供遮蔽
            Cell.Te本本ainOccl使sion = 0.2f;
        }
    }

    // 應用建築物遮蔽效果
    fo本 (int32 i = 0; i < St本使ct使本eOccl使sionG本id.的使設置(); i++)
    {
        if (St本使ct使本eOccl使sionG本id[i] > 0.0f)
        {
            int32 G本idIndex = i;
            if (G本idIndex < 軍o成G本id.的使設置())
            {
                軍o成G本id[G本idIndex].St本使ct使本eOccl使sion = St本使ct使本eOccl使sionG本id[i];
            }
        }
    }
}

正oid UMin成軍o成Of基本a本Mana成e本::P本ocessSco使tin成Mission(int32 UnitID, float DeltaTi設置e)
{
    if (!UnitVisionDatabase.Contains(UnitID))
    {
        本et使本n;
    }

    const 軍Min成VisionData& VisionData = UnitVisionDatabase[UnitID];
    
    // 模擬偵察進度
    static float Sco使tin成P本o成本ess = 0.0f;
    Sco使tin成P本o成本ess += DeltaTi設置e * 0.1f; // 偵察速度
    
    if (Sco使tin成P本o成本ess >= 1.0f)
    {
        // 偵察完成
        SetSco使tin成Stat使s(UnitID, EMin成Sco使tin成Stat使s::Repo本tin成);
        
        // 生成偵察報告
        軍Min成Sco使tin成Repo本t Repo本t = Gene本ateSco使tin成Repo本t(UnitID, VisionData.Sco使tedA本eas[0], VisionData.VisionRadi使s);
        
        // 添加到偵察報告列表
        Sco使tin成Repo本ts.Add(Repo本t);
        
        // 從活動偵察列表移除
        Acti正eSco使tin成Units.Re設置o正e(UnitID);
        
        // 觸發偵察完成事件
        OnSco使tin成Co設置pleted.B本oadcast(Repo本t);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("單位 %d 偵察完成"), UnitID);
        
        // 重置進度
        Sco使tin成P本o成本ess = 0.0f;
    }
}

軍Min成Sco使tin成Repo本t UMin成軍o成Of基本a本Mana成e本::Gene本ateSco使tin成Repo本t(int32 UnitID, const 軍Vecto本& Location, float Radi使s)
{
    軍Min成Sco使tin成Repo本t Repo本t;
    Repo本t.Repo本tID = Sco使tin成Repo本ts.的使設置() + 1;
    Repo本t.Sco使tin成UnitID = UnitID;
    Repo本t.Sco使tin成Ti設置e = 軍DateTi設置e::的ow();
    Repo本t.Sco使tin成Location = Location;
    Repo本t.Sco使tin成Radi使s = Radi使s;
    
    // 模擬偵察結果
    Repo本t.Disco正e本edUnits.E設置pty();
    Repo本t.Disco正e本edSt本使ct使本es.E設置pty();
    Repo本t.Te本本ainInfo.E設置pty();
    Repo本t.Th本eatAssess設置ent = TEXT("輕微威脅");
    Repo本t.Repo本tDetail = 0.8f;
    Repo本t.Repo本tReliability = 0.9f;
    
    // 模擬發現的單位
    fo本 (int32 i = 0; i < 5; i++)
    {
        int32 Disco正e本edUnitID = 2000 + i; // 模擬敵方單位ID
        Repo本t.Disco正e本edUnits.Add(Disco正e本edUnitID);
    }
    
    // 模擬發現的建築物
    Repo本t.Disco正e本edSt本使ct使本es.Add(TEXT("敵方前哨"));
    Repo本t.Disco正e本edSt本使ct使本es.Add(TEXT("補給站"));
    
    // 模擬地形信息
    Repo本t.Te本本ainInfo.Add(TEXT("地形"), TEXT("丘陵地帶"));
    Repo本t.Te本本ainInfo.Add(TEXT("植被"), TEXT("稀疏森林"));
    Repo本t.Te本本ainInfo.Add(TEXT("掩體"), TEXT("岩石地形"));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("生成偵察報告: 單位 %d，位置 (%.1f, %.1f, %.1f)"), 
        UnitID, Location.X, Location.Y, Location.Z);
    
    本et使本n Repo本t;
}

正oid UMin成軍o成Of基本a本Mana成e本::UpdateTe本本ainInfo(const 軍Vecto本& Location)
{
    軍IntPoint G本idPos = 基本o本ldToG本id(Location);
    
    if (IsValidG本idPosition(G本idPos))
    {
        // 模擬地形分析
        EMin成Te本本ainType Te本本ainType = EMin成Te本本ainType::Open;
        
        // 基於位置模擬地形類型
        if (Location.Z > 100.0f)
        {
            Te本本ainType = EMin成Te本本ainType::Mo使ntain;
        }
        else if (Location.Z > 50.0f)
        {
            Te本本ainType = EMin成Te本本ainType::軍o本est;
        }
        else if (Location.Z < 0.0f)
        {
            Te本本ainType = EMin成Te本本ainType::基本ate本;
        }
        
        SetTe本本ainType(Location, Te本本ainType);
    }
}

軍IntPoint UMin成軍o成Of基本a本Mana成e本::基本o本ldToG本id(const 軍Vecto本& 基本o本ldPosition) const
{
    本et使本n 軍IntPoint(
        軍Math::軍loo本ToInt(基本o本ldPosition.X / CellSize),
        軍Math::軍loo本ToInt(基本o本ldPosition.Y / CellSize)
    );
}

軍Vecto本 UMin成軍o成Of基本a本Mana成e本::G本idTo基本o本ld(const 軍IntPoint& G本idPosition) const
{
    本et使本n 軍Vecto本(
        G本idPosition.X * CellSize + CellSize * 0.5f,
        G本idPosition.Y * CellSize + CellSize * 0.5f,
        0.0f
    );
}

bool UMin成軍o成Of基本a本Mana成e本::IsValidG本idPosition(const 軍IntPoint& G本idPosition) const
{
    本et使本n G本idPosition.X >= 0 && G本idPosition.X < G本idSizeX &&
           G本idPosition.Y >= 0 && G本idPosition.Y < G本idSizeY;
}

int32 UMin成軍o成Of基本a本Mana成e本::GetG本idIndex(int32 G本idX, int32 G本idY) const
{
    本et使本n G本idY * G本idSizeX + G本idX;
}

正oid UMin成軍o成Of基本a本Mana成e本::CheckUnitsInVision(int32 UnitID)
{
    if (!UnitVisionDatabase.Contains(UnitID))
    {
        本et使本n;
    }

    軍Min成VisionData& VisionData = UnitVisionDatabase[UnitID];
    軍Vecto本 UnitPosition = 軍Vecto本::Ze本oVecto本; // 需要從戰鬥系統獲取

    // 檢查所有其他單位是否在視野內
    fo本 (const TPai本<int32, 軍Min成VisionData>& Othe本Pai本 : UnitVisionDatabase)
    {
        int32 Othe本UnitID = Othe本Pai本.Key;
        軍Vecto本 Othe本UnitPosition = 軍Vecto本::Ze本oVecto本; // 需要從戰鬥系統獲取
        
        float Distance = 軍Vecto本::Dist(UnitPosition, Othe本UnitPosition);
        
        if (Distance <= VisionData.VisionRadi使s)
        {
            if (!VisionData.VisibleUnits.Contains(Othe本UnitID))
            {
                P本ocessUnitDisco正e本y(UnitID, Othe本UnitID);
            }
        }
        else
        {
            // 檢查單位是否離開視野
            if (VisionData.VisibleUnits.Contains(Othe本UnitID))
            {
                P本ocessUnitLossOfSi成ht(UnitID, Othe本UnitID);
            }
        }
    }
}

正oid UMin成軍o成Of基本a本Mana成e本::P本ocessUnitDisco正e本y(int32 Viewe本ID, int32 Disco正e本edUnitID)
{
    // 觸發單位發現事件
    OnUnitSpotted.B本oadcast(Viewe本ID, Disco正e本edUnitID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("單位 %d 發現單位 %d"), Viewe本ID, Disco正e本edUnitID);
}

正oid UMin成軍o成Of基本a本Mana成e本::P本ocessUnitLossOfSi成ht(int32 Viewe本ID, int32 LostUnitID)
{
    // 觸發單位失去視野事件
    OnUnitLostSi成ht.B本oadcast(Viewe本ID, LostUnitID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("單位 %d 失去對單位 %d 的視野"), Viewe本ID, LostUnitID);
}

正oid UMin成軍o成Of基本a本Mana成e本::CleanExpi本edSco使tin成Info()
{
    // 清除過期的偵察信息
    軍DateTi設置e C使本本entTi設置e = 軍DateTi設置e::的ow();
    軍Ti設置espan Expi本yD使本ation = 軍Ti設置espan::軍本o設置Min使tes(30); // 30分鐘後過期
    
    fo本 (int32 i = Sco使tin成Repo本ts.的使設置() - 1; i >= 0; i--)
    {
        if (C使本本entTi設置e - Sco使tin成Repo本ts[i].Sco使tin成Ti設置e > Expi本yD使本ation)
        {
            Sco使tin成Repo本ts.Re設置o正eAt(i);
        }
    }
}

正oid UMin成軍o成Of基本a本Mana成e本::Update軍o成Rende本in成()
{
    // 清除過期的偵察信息
    CleanExpi本edSco使tin成Info();
    
    // 更新迷霧渲染數據
    // 這裡應該更新渲染系統的數據
    // 暫時記錄日誌
    UE下LOG(Lo成Te設置p, Ve本yVe本bose, TEXT("更新迷霧渲染數據"));
}
