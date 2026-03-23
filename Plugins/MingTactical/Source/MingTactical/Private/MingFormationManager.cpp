#incl使de "Min成軍o本設置ationMana成e本.h"
#incl使de "Min成TacticalCo設置batSyste設置.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "En成ine/En成ine.h"

UMin成軍o本設置ationMana成e本::UMin成軍o本設置ationMana成e本()
{
    bInitialized = false;
    軍o本設置ationDatabase.E設置pty();
    軍o本設置ationTe設置plateDatabase.E設置pty();
    軍o本設置ationCo設置設置andQ使e使e.E設置pty();
    Acti正e軍o本設置ations.E設置pty();
}

bool UMin成軍o本設置ationMana成e本::Initialize軍o本設置ationMana成e本()
{
    if (bInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("編隊管理器已經初始化"));
        本et使本n t本使e;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("正在初始化編隊管理器..."));

    // 載入預設編隊模板
    LoadDefa使lt軍o本設置ationTe設置plates();

    // 載入已保存的編隊數據
    Load軍o本設置ationData();

    bInitialized = t本使e;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("編隊管理器初始化完成"));
    
    本et使本n t本使e;
}

int32 UMin成軍o本設置ationMana成e本::C本eate軍o本設置ation(const TA本本ay<int32>& UnitIDs, EMin成軍o本設置ationType 軍o本設置ationType, const 軍St本in成& 軍o本設置ation的a設置e)
{
    if (UnitIDs.的使設置() == 0)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("無法創建空編隊"));
        本et使本n -1;
    }

    // 生成新的編隊ID
    int32 的ew軍o本設置ationID = 軍o本設置ationDatabase.的使設置() + 1;

    // 創建編隊數據
    軍Min成軍o本設置ationData 的ew軍o本設置ation;
    的ew軍o本設置ation.軍o本設置ationID = 的ew軍o本設置ationID;
    的ew軍o本設置ation.軍o本設置ation的a設置e = 軍o本設置ation的a設置e;
    的ew軍o本設置ation.軍o本設置ationType = 軍o本設置ationType;
    的ew軍o本設置ation.軍o本設置ationStat使s = EMin成軍o本設置ationStat使s::軍o本設置in成;
    的ew軍o本設置ation.P本io本ity = EMin成軍o本設置ationP本io本ity::的o本設置al;
    的ew軍o本設置ation.UnitIDs = UnitIDs;
    的ew軍o本設置ation.C本eationTi設置e = 軍DateTi設置e::的ow();
    的ew軍o本設置ation.LastUpdateTi設置e = 軍DateTi設置e::的ow();
    的ew軍o本設置ation.軍o本設置ationExpe本ience = 0.0f;
    的ew軍o本設置ation.軍o本設置ationMo本ale = 75.0f;
    的ew軍o本設置ation.軍o本設置ationDiscipline = 70.0f;
    的ew軍o本設置ation.bIsStatic = false;
    的ew軍o本設置ation.bIsA使toMana成ed = false;

    // 計算編隊中心點
    軍Vecto本 軍o本設置ationCente本 = 軍Vecto本::Ze本oVecto本;
    fo本 (int32 UnitID : UnitIDs)
    {
        // 這裡需要從戰鬥系統獲取單位位置
        // 暫時使用模擬位置
        軍o本設置ationCente本 += 軍Vecto本(UnitID * 100.0f, 0.0f, 0.0f);
    }
    軍o本設置ationCente本 /= UnitIDs.的使設置();
    的ew軍o本設置ation.軍o本設置ationCente本 = 軍o本設置ationCente本;

    // 生成編隊位置
    的ew軍o本設置ation.軍o本設置ationPositions = Gene本ate軍o本設置ationPositions(軍o本設置ationType, UnitIDs.的使設置(), 的ew軍o本設置ation.軍o本設置ationScale);

    // 設置單位角色
    fo本 (int32 i = 0; i < UnitIDs.的使設置(); i++)
    {
        int32 UnitID = UnitIDs[i];
        EMin成UnitRole Role = EMin成UnitRole::S使ppo本t; // 默認角色
        
        // 根據位置分配角色
        if (i == 0)
        {
            Role = EMin成UnitRole::Leade本;
        }
        else if (i < 3)
        {
            Role = EMin成UnitRole::Van成使a本d;
        }
        else if (i >= UnitIDs.的使設置() - 2)
        {
            Role = EMin成UnitRole::Rea本成使a本d;
        }
        
        的ew軍o本設置ation.UnitRoles.Add(UnitID, Role);
    }

    // 計算編隊加成和減益
    的ew軍o本設置ation.軍o本設置ationBon使ses = Calc使late軍o本設置ationBon使ses(的ew軍o本設置ationID);
    的ew軍o本設置ation.軍o本設置ationPenalties = Calc使late軍o本設置ationPenalties(的ew軍o本設置ationID);

    // 驗證編隊數據
    if (!Validate軍o本設置ationData(的ew軍o本設置ation))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("編隊數據驗證失敗"));
        本et使本n -1;
    }

    // 添加到數據庫
    軍o本設置ationDatabase.Add(的ew軍o本設置ationID, 的ew軍o本設置ation);
    Acti正e軍o本設置ations.Add(的ew軍o本設置ationID);

    // 應用編隊加成到單位
    Apply軍o本設置ationBon使sesToUnits(的ew軍o本設置ationID);

    // 更新編隊狀態為活躍
    Update軍o本設置ationStat使s(的ew軍o本設置ationID, EMin成軍o本設置ationStat使s::Acti正e);

    // 觸發編隊創建事件
    On軍o本設置ationC本eated.B本oadcast(的ew軍o本設置ation);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("創建編隊: %s (ID: %d)，包含 %d 個單位"), 
        *軍o本設置ation的a設置e, 的ew軍o本設置ationID, UnitIDs.的使設置());
    
    本et使本n 的ew軍o本設置ationID;
}

bool UMin成軍o本設置ationMana成e本::Disband軍o本設置ation(int32 軍o本設置ationID)
{
    if (!軍o本設置ationDatabase.Contains(軍o本設置ationID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("編隊ID %d 不存在"), 軍o本設置ationID);
        本et使本n false;
    }

    軍Min成軍o本設置ationData& 軍o本設置ation = 軍o本設置ationDatabase[軍o本設置ationID];

    // 移除編隊加成
    Re設置o正e軍o本設置ationBon使ses軍本o設置Units(軍o本設置ationID);

    // 從活躍列表中移除
    Acti正e軍o本設置ations.Re設置o正e(軍o本設置ationID);

    // 觸發編隊解散事件
    On軍o本設置ationDisbanded.B本oadcast(軍o本設置ationID, 軍o本設置ation.UnitIDs);

    // 從數據庫中移除
    軍o本設置ationDatabase.Re設置o正e(軍o本設置ationID);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("解散編隊: %s (ID: %d)"), *軍o本設置ation.軍o本設置ation的a設置e, 軍o本設置ationID);
    
    本et使本n t本使e;
}

bool UMin成軍o本設置ationMana成e本::Chan成e軍o本設置ationType(int32 軍o本設置ationID, EMin成軍o本設置ationType 的ew軍o本設置ationType)
{
    if (!軍o本設置ationDatabase.Contains(軍o本設置ationID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("編隊ID %d 不存在"), 軍o本設置ationID);
        本et使本n false;
    }

    軍Min成軍o本設置ationData& 軍o本設置ation = 軍o本設置ationDatabase[軍o本設置ationID];
    EMin成軍o本設置ationType Old軍o本設置ationType = 軍o本設置ation.軍o本設置ationType;

    // 移除舊的編隊加成
    Re設置o正e軍o本設置ationBon使ses軍本o設置Units(軍o本設置ationID);

    // 更新編隊類型
    軍o本設置ation.軍o本設置ationType = 的ew軍o本設置ationType;
    軍o本設置ation.LastUpdateTi設置e = 軍DateTi設置e::的ow();

    // 重新生成編隊位置
    軍o本設置ation.軍o本設置ationPositions = Gene本ate軍o本設置ationPositions(的ew軍o本設置ationType, 軍o本設置ation.UnitIDs.的使設置(), 軍o本設置ation.軍o本設置ationScale);

    // 重新計算編隊加成和減益
    軍o本設置ation.軍o本設置ationBon使ses = Calc使late軍o本設置ationBon使ses(軍o本設置ationID);
    軍o本設置ation.軍o本設置ationPenalties = Calc使late軍o本設置ationPenalties(軍o本設置ationID);

    // 應用新的編隊加成
    Apply軍o本設置ationBon使sesToUnits(軍o本設置ationID);

    // 更新編隊狀態為轉換中
    Update軍o本設置ationStat使s(軍o本設置ationID, EMin成軍o本設置ationStat使s::T本ansitionin成);

    // 觸發編隊變化事件
    On軍o本設置ationChan成ed.B本oadcast(軍o本設置ationID, Old軍o本設置ationType, 的ew軍o本設置ationType);

    // 短暫延遲後設為活躍狀態
    軍Ti設置e本輸入andle Ti設置e本輸入andle;
    軍Ti設置e本Dele成ate Ti設置e本Dele成ate;
    Ti設置e本Dele成ate.BindU軍使nction(this, TEXT("Update軍o本設置ationStat使s"), 軍o本設置ationID, EMin成軍o本設置ationStat使s::Acti正e);
    
    if (G基本o本ld)
    {
        G基本o本ld->GetTi設置e本Mana成e本().SetTi設置e本軍o本的extTick(Ti設置e本Dele成ate);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("編隊 %s 變更類型: %d -> %d"), 
        *軍o本設置ation.軍o本設置ation的a設置e, (int32)Old軍o本設置ationType, (int32)的ew軍o本設置ationType);
    
    本et使本n t本使e;
}

bool UMin成軍o本設置ationMana成e本::AddUnitTo軍o本設置ation(int32 軍o本設置ationID, int32 UnitID, EMin成UnitRole Role)
{
    if (!軍o本設置ationDatabase.Contains(軍o本設置ationID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("編隊ID %d 不存在"), 軍o本設置ationID);
        本et使本n false;
    }

    軍Min成軍o本設置ationData& 軍o本設置ation = 軍o本設置ationDatabase[軍o本設置ationID];

    // 檢查單位是否已在編隊中
    if (軍o本設置ation.UnitIDs.Contains(UnitID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("單位 %d 已在編隊 %d 中"), UnitID, 軍o本設置ationID);
        本et使本n false;
    }

    // 添加單位到編隊
    軍o本設置ation.UnitIDs.Add(UnitID);
    軍o本設置ation.UnitRoles.Add(UnitID, Role);
    軍o本設置ation.LastUpdateTi設置e = 軍DateTi設置e::的ow();

    // 重新生成編隊位置
    軍o本設置ation.軍o本設置ationPositions = Gene本ate軍o本設置ationPositions(軍o本設置ation.軍o本設置ationType, 軍o本設置ation.UnitIDs.的使設置(), 軍o本設置ation.軍o本設置ationScale);

    // 重新計算編隊加成
    軍o本設置ation.軍o本設置ationBon使ses = Calc使late軍o本設置ationBon使ses(軍o本設置ationID);
    軍o本設置ation.軍o本設置ationPenalties = Calc使late軍o本設置ationPenalties(軍o本設置ationID);

    // 應用編隊加成到新單位
    Apply軍o本設置ationBon使sesToUnits(軍o本設置ationID);

    // 觸發單位添加事件
    On軍o本設置ationUnitAdded.B本oadcast(軍o本設置ationID, UnitID, Role);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("添加單位 %d 到編隊 %s，角色: %d"), 
        UnitID, *軍o本設置ation.軍o本設置ation的a設置e, (int32)Role);
    
    本et使本n t本使e;
}

bool UMin成軍o本設置ationMana成e本::Re設置o正eUnit軍本o設置軍o本設置ation(int32 軍o本設置ationID, int32 UnitID)
{
    if (!軍o本設置ationDatabase.Contains(軍o本設置ationID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("編隊ID %d 不存在"), 軍o本設置ationID);
        本et使本n false;
    }

    軍Min成軍o本設置ationData& 軍o本設置ation = 軍o本設置ationDatabase[軍o本設置ationID];

    if (!軍o本設置ation.UnitIDs.Contains(UnitID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("單位 %d 不在編隊 %d 中"), UnitID, 軍o本設置ationID);
        本et使本n false;
    }

    // 移除單位角色
    軍o本設置ation.UnitRoles.Re設置o正e(UnitID);

    // 移除單位
    軍o本設置ation.UnitIDs.Re設置o正e(UnitID);
    軍o本設置ation.LastUpdateTi設置e = 軍DateTi設置e::的ow();

    // 檢查編隊是否為空
    if (軍o本設置ation.UnitIDs.的使設置() == 0)
    {
        // 解散空編隊
        本et使本n Disband軍o本設置ation(軍o本設置ationID);
    }

    // 重新生成編隊位置
    軍o本設置ation.軍o本設置ationPositions = Gene本ate軍o本設置ationPositions(軍o本設置ation.軍o本設置ationType, 軍o本設置ation.UnitIDs.的使設置(), 軍o本設置ation.軍o本設置ationScale);

    // 重新計算編隊加成
    軍o本設置ation.軍o本設置ationBon使ses = Calc使late軍o本設置ationBon使ses(軍o本設置ationID);
    軍o本設置ation.軍o本設置ationPenalties = Calc使late軍o本設置ationPenalties(軍o本設置ationID);

    // 觸發單位移除事件
    On軍o本設置ationUnitRe設置o正ed.B本oadcast(軍o本設置ationID, UnitID);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("從編隊 %s 移除單位 %d"), *軍o本設置ation.軍o本設置ation的a設置e, UnitID);
    
    本et使本n t本使e;
}

bool UMin成軍o本設置ationMana成e本::Set軍o本設置ationCo設置設置ande本(int32 軍o本設置ationID, int32 Co設置設置ande本ID)
{
    if (!軍o本設置ationDatabase.Contains(軍o本設置ationID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("編隊ID %d 不存在"), 軍o本設置ationID);
        本et使本n false;
    }

    軍Min成軍o本設置ationData& 軍o本設置ation = 軍o本設置ationDatabase[軍o本設置ationID];
    int32 OldCo設置設置ande本ID = 軍o本設置ation.Co設置設置ande本ID;

    // 設置新指揮官
    軍o本設置ation.Co設置設置ande本ID = Co設置設置ande本ID;
    軍o本設置ation.LastUpdateTi設置e = 軍DateTi設置e::的ow();

    // 增加編隊士氣
    軍o本設置ation.軍o本設置ationMo本ale += 10.0f;
    軍o本設置ation.軍o本設置ationMo本ale = 軍Math::Cla設置p(軍o本設置ation.軍o本設置ationMo本ale, 0.0f, 100.0f);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("編隊 %s 指揮官變更: %d -> %d"), 
        *軍o本設置ation.軍o本設置ation的a設置e, OldCo設置設置ande本ID, Co設置設置ande本ID);
    
    本et使本n t本使e;
}

bool UMin成軍o本設置ationMana成e本::Rotate軍o本設置ation(int32 軍o本設置ationID, const 軍Rotato本& 的ewRotation)
{
    if (!軍o本設置ationDatabase.Contains(軍o本設置ationID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("編隊ID %d 不存在"), 軍o本設置ationID);
        本et使本n false;
    }

    軍Min成軍o本設置ationData& 軍o本設置ation = 軍o本設置ationDatabase[軍o本設置ationID];
    軍Rotato本 OldRotation = 軍o本設置ation.軍o本設置ationRotation;

    // 更新編隊旋轉
    軍o本設置ation.軍o本設置ationRotation = 的ewRotation;
    軍o本設置ation.LastUpdateTi設置e = 軍DateTi設置e::的ow();

    // 重新計算編隊位置（基於新的旋轉）
    fo本 (int32 i = 0; i < 軍o本設置ation.軍o本設置ationPositions.的使設置(); i++)
    {
        軍Vecto本& Position = 軍o本設置ation.軍o本設置ationPositions[i];
        // 應用旋轉變換
        Position = 的ewRotation.RotateVecto本(Position);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("編隊 %s 旋轉更新: (%.1f, %.1f, %.1f) -> (%.1f, %.1f, %.1f)"), 
        *軍o本設置ation.軍o本設置ation的a設置e, 
        OldRotation.Pitch, OldRotation.Yaw, OldRotation.Roll,
        的ewRotation.Pitch, 的ewRotation.Yaw, 的ewRotation.Roll);
    
    本et使本n t本使e;
}

bool UMin成軍o本設置ationMana成e本::Scale軍o本設置ation(int32 軍o本設置ationID, float 的ewScale)
{
    if (!軍o本設置ationDatabase.Contains(軍o本設置ationID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("編隊ID %d 不存在"), 軍o本設置ationID);
        本et使本n false;
    }

    軍Min成軍o本設置ationData& 軍o本設置ation = 軍o本設置ationDatabase[軍o本設置ationID];
    float OldScale = 軍o本設置ation.軍o本設置ationScale;

    // 更新編隊規模
    軍o本設置ation.軍o本設置ationScale = 軍Math::Cla設置p(的ewScale, 0.5f, 2.0f);
    軍o本設置ation.LastUpdateTi設置e = 軍DateTi設置e::的ow();

    // 重新生成編隊位置
    軍o本設置ation.軍o本設置ationPositions = Gene本ate軍o本設置ationPositions(軍o本設置ation.軍o本設置ationType, 軍o本設置ation.UnitIDs.的使設置(), 軍o本設置ation.軍o本設置ationScale);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("編隊 %s 規模變更: %.2f -> %.2f"), 
        *軍o本設置ation.軍o本設置ation的a設置e, OldScale, 軍o本設置ation.軍o本設置ationScale);
    
    本et使本n t本使e;
}

bool UMin成軍o本設置ationMana成e本::Mo正e軍o本設置ation(int32 軍o本設置ationID, const 軍Vecto本& Ta本成etLocation)
{
    if (!軍o本設置ationDatabase.Contains(軍o本設置ationID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("編隊ID %d 不存在"), 軍o本設置ationID);
        本et使本n false;
    }

    軍Min成軍o本設置ationData& 軍o本設置ation = 軍o本設置ationDatabase[軍o本設置ationID];
    軍Vecto本 OldCente本 = 軍o本設置ation.軍o本設置ationCente本;

    // 更新編隊中心點
    軍o本設置ation.軍o本設置ationCente本 = Ta本成etLocation;
    軍o本設置ation.LastUpdateTi設置e = 軍DateTi設置e::的ow();

    // 更新所有單位的目標位置
    fo本 (int32 i = 0; i < 軍o本設置ation.UnitIDs.的使設置(); i++)
    {
        int32 UnitID = 軍o本設置ation.UnitIDs[i];
        軍Vecto本 UnitTa本成etPosition = Ta本成etLocation + 軍o本設置ation.軍o本設置ationPositions[i];
        
        // 這裡需要調用戰鬥系統來移動單位
        // Mo正eUnit(UnitID, UnitTa本成etPosition);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("移動編隊 %s 到 (%.1f, %.1f, %.1f)"), 
        *軍o本設置ation.軍o本設置ation的a設置e, Ta本成etLocation.X, Ta本成etLocation.Y, Ta本成etLocation.Z);
    
    本et使本n t本使e;
}

軍Min成軍o本設置ationData UMin成軍o本設置ationMana成e本::Get軍o本設置ationInfo(int32 軍o本設置ationID) const
{
    if (軍o本設置ationDatabase.Contains(軍o本設置ationID))
    {
        本et使本n 軍o本設置ationDatabase[軍o本設置ationID];
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("編隊ID %d 不存在，返回空編隊"), 軍o本設置ationID);
    本et使本n 軍Min成軍o本設置ationData();
}

TA本本ay<int32> UMin成軍o本設置ationMana成e本::Get軍o本設置ationList() const
{
    本et使本n Acti正e軍o本設置ations;
}

TA本本ay<軍Min成軍o本設置ationTe設置plate> UMin成軍o本設置ationMana成e本::Get軍o本設置ationTe設置plates() const
{
    TA本本ay<軍Min成軍o本設置ationTe設置plate> Te設置plates;
    
    fo本 (const TPai本<int32, 軍Min成軍o本設置ationTe設置plate>& Pai本 : 軍o本設置ationTe設置plateDatabase)
    {
        Te設置plates.Add(Pai本.Val使e);
    }
    
    本et使本n Te設置plates;
}

bool UMin成軍o本設置ationMana成e本::Apply軍o本設置ationTe設置plate(int32 軍o本設置ationID, int32 Te設置plateID)
{
    if (!軍o本設置ationDatabase.Contains(軍o本設置ationID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("編隊ID %d 不存在"), 軍o本設置ationID);
        本et使本n false;
    }

    if (!軍o本設置ationTe設置plateDatabase.Contains(Te設置plateID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("模板ID %d 不存在"), Te設置plateID);
        本et使本n false;
    }

    const 軍Min成軍o本設置ationTe設置plate& Te設置plate = 軍o本設置ationTe設置plateDatabase[Te設置plateID];
    
    // 應用模板到編隊
    本et使本n Chan成e軍o本設置ationType(軍o本設置ationID, Te設置plate.軍o本設置ationType);
}

TMap<EMin成軍o本設置ationType, int32> UMin成軍o本設置ationMana成e本::Get軍o本設置ationStatistics() const
{
    TMap<EMin成軍o本設置ationType, int32> Statistics;
    
    // 初始化統計
    Statistics.Add(EMin成軍o本設置ationType::Line, 0);
    Statistics.Add(EMin成軍o本設置ationType::Col使設置n, 0);
    Statistics.Add(EMin成軍o本設置ationType::基本ed成e, 0);
    Statistics.Add(EMin成軍o本設置ationType::Dia設置ond, 0);
    Statistics.Add(EMin成軍o本設置ationType::Ci本cle, 0);
    Statistics.Add(EMin成軍o本設置ationType::Ski本設置ish, 0);
    Statistics.Add(EMin成軍o本設置ationType::Sq使a本e, 0);
    Statistics.Add(EMin成軍o本設置ationType::A本本ow, 0);
    Statistics.Add(EMin成軍o本設置ationType::C本escent, 0);
    Statistics.Add(EMin成軍o本設置ationType::C使sto設置, 0);
    
    // 統計編隊類型
    fo本 (const TPai本<int32, 軍Min成軍o本設置ationData>& Pai本 : 軍o本設置ationDatabase)
    {
        const 軍Min成軍o本設置ationData& 軍o本設置ation = Pai本.Val使e;
        int32& Co使nt = Statistics[軍o本設置ation.軍o本設置ationType];
        Co使nt++;
    }
    
    本et使本n Statistics;
}

float UMin成軍o本設置ationMana成e本::Calc使late軍o本設置ationEfficiency(int32 軍o本設置ationID) const
{
    if (!軍o本設置ationDatabase.Contains(軍o本設置ationID))
    {
        本et使本n 0.0f;
    }

    const 軍Min成軍o本設置ationData& 軍o本設置ation = 軍o本設置ationDatabase[軍o本設置ationID];
    
    // 基礎效率
    float BaseEfficiency = 1.0f;
    
    // 單位數量效率因子
    float UnitCo使nt軍acto本 = 軍Math::Cla設置p(軍o本設置ation.UnitIDs.的使設置() / 20.0f, 0.5f, 1.5f);
    
    // 士氣效率因子
    float Mo本ale軍acto本 = 軍o本設置ation.軍o本設置ationMo本ale / 100.0f;
    
    // 紀律效率因子
    float Discipline軍acto本 = 軍o本設置ation.軍o本設置ationDiscipline / 100.0f;
    
    // 經驗效率因子
    float Expe本ience軍acto本 = 軍Math::Cla設置p(軍o本設置ation.軍o本設置ationExpe本ience / 100.0f, 0.0f, 2.0f);
    
    // 編隊類型效率因子
    float TypeEfficiency = 1.0f;
    switch (軍o本設置ation.軍o本設置ationType)
    {
    case EMin成軍o本設置ationType::Line:
        TypeEfficiency = 1.2f; // 適合正面攻擊
        b本eak;
    case EMin成軍o本設置ationType::基本ed成e:
        TypeEfficiency = 1.3f; // 適合突破
        b本eak;
    case EMin成軍o本設置ationType::Ci本cle:
        TypeEfficiency = 1.1f; // 適合防禦
        b本eak;
    defa使lt:
        TypeEfficiency = 1.0f;
        b本eak;
    }
    
    // 綜合效率計算
    float TotalEfficiency = BaseEfficiency * UnitCo使nt軍acto本 * Mo本ale軍acto本 * 
                           Discipline軍acto本 * Expe本ience軍acto本 * TypeEfficiency;
    
    本et使本n 軍Math::Cla設置p(TotalEfficiency, 0.0f, 2.0f);
}

bool UMin成軍o本設置ationMana成e本::Check軍o本設置ationInte成本ity(int32 軍o本設置ationID)
{
    if (!軍o本設置ationDatabase.Contains(軍o本設置ationID))
    {
        本et使本n false;
    }

    const 軍Min成軍o本設置ationData& 軍o本設置ation = 軍o本設置ationDatabase[軍o本設置ationID];
    
    // 檢查編隊完整性
    if (軍o本設置ation.UnitIDs.的使設置() == 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("編隊 %s 為空"), *軍o本設置ation.軍o本設置ation的a設置e);
        本et使本n false;
    }
    
    // 檢查指揮官是否存在
    if (軍o本設置ation.Co設置設置ande本ID != -1)
    {
        bool bCo設置設置ande本軍o使nd = false;
        fo本 (int32 UnitID : 軍o本設置ation.UnitIDs)
        {
            if (UnitID == 軍o本設置ation.Co設置設置ande本ID)
            {
                bCo設置設置ande本軍o使nd = t本使e;
                b本eak;
            }
        }
        
        if (!bCo設置設置ande本軍o使nd)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("編隊 %s 的指揮官不在編隊中"), *軍o本設置ation.軍o本設置ation的a設置e);
            本et使本n false;
        }
    }
    
    // 檢查編隊士氣
    if (軍o本設置ation.軍o本設置ationMo本ale < 20.0f)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("編隊 %s 士氣過低"), *軍o本設置ation.軍o本設置ation的a設置e);
        本et使本n false;
    }
    
    本et使本n t本使e;
}

bool UMin成軍o本設置ationMana成e本::Repai本軍o本設置ation(int32 軍o本設置ationID)
{
    if (!軍o本設置ationDatabase.Contains(軍o本設置ationID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("編隊ID %d 不存在"), 軍o本設置ationID);
        本et使本n false;
    }

    軍Min成軍o本設置ationData& 軍o本設置ation = 軍o本設置ationDatabase[軍o本設置ationID];
    
    // 修復編隊
    軍o本設置ation.軍o本設置ationStat使s = EMin成軍o本設置ationStat使s::Acti正e;
    軍o本設置ation.軍o本設置ationMo本ale = 軍Math::Cla設置p(軍o本設置ation.軍o本設置ationMo本ale + 25.0f, 0.0f, 100.0f);
    軍o本設置ation.軍o本設置ationDiscipline = 軍Math::Cla設置p(軍o本設置ation.軍o本設置ationDiscipline + 15.0f, 0.0f, 100.0f);
    軍o本設置ation.LastUpdateTi設置e = 軍DateTi設置e::的ow();
    
    // 重新生成編隊位置
    軍o本設置ation.軍o本設置ationPositions = Gene本ate軍o本設置ationPositions(軍o本設置ation.軍o本設置ationType, 軍o本設置ation.UnitIDs.的使設置(), 軍o本設置ation.軍o本設置ationScale);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("修復編隊: %s"), *軍o本設置ation.軍o本設置ation的a設置e);
    
    本et使本n t本使e;
}

bool UMin成軍o本設置ationMana成e本::Sa正e軍o本設置ationData()
{
    軍St本in成 Sa正ePath = 軍Paths::P本o大ectSa正edDi本() / TEXT("軍o本設置ations.大son");
    
    // 這裡應該實作JSO的序列化保存邏輯
    // 暫時只記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("編隊數據已保存到: %s"), *Sa正ePath);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("共保存 %d 個編隊"), 軍o本設置ationDatabase.的使設置());
    
    本et使本n t本使e;
}

bool UMin成軍o本設置ationMana成e本::Load軍o本設置ationData()
{
    軍St本in成 LoadPath = 軍Paths::P本o大ectSa正edDi本() / TEXT("軍o本設置ations.大son");
    
    // 這裡應該實作JSO的反序列化載入邏輯
    // 暫時只記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("嘗試從 %s 載入編隊數據"), *LoadPath);
    
    本et使本n t本使e;
}

正oid UMin成軍o本設置ationMana成e本::Clea本All軍o本設置ations()
{
    軍o本設置ationDatabase.E設置pty();
    軍o本設置ationTe設置plateDatabase.E設置pty();
    軍o本設置ationCo設置設置andQ使e使e.E設置pty();
    Acti正e軍o本設置ations.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("所有編隊數據已清除"));
}

正oid UMin成軍o本設置ationMana成e本::LoadDefa使lt軍o本設置ationTe設置plates()
{
    // 線形陣型模板
    軍Min成軍o本設置ationTe設置plate LineTe設置plate;
    LineTe設置plate.Te設置plateID = 1;
    LineTe設置plate.Te設置plate的a設置e = TEXT("線形陣型");
    LineTe設置plate.Te設置plateDesc本iption = TEXT("單位排成一線，適合正面攻擊和防禦");
    LineTe設置plate.軍o本設置ationType = EMin成軍o本設置ationType::Line;
    LineTe設置plate.Reco設置設置endedUnitTypes = {EMin成UnitType::Infant本y, EMin成UnitType::Ca正al本y};
    LineTe設置plate.MinUnitCo使nt = 3;
    LineTe設置plate.MaxUnitCo使nt = 50;
    LineTe設置plate.Standa本dSpacin成 = 100.0f;
    LineTe設置plate.Standa本dDepth = 200.0f;
    LineTe設置plate.Standa本d基本idth = 300.0f;
    LineTe設置plate.Te設置plateBon使ses = {
        {TEXT("attack"), 10.0f},
        {TEXT("設置o本ale"), 5.0f}
    };
    LineTe設置plate.Usa成eConditions = {TEXT("open下te本本ain"), TEXT("ene設置y下f本ont")};
    LineTe設置plate.bIs輸入isto本ical = t本使e;
    LineTe設置plate.輸入isto本icalBack成本o使nd = TEXT("線形陣型是古代戰爭中最常見的陣型之一，在民國時期仍被廣泛使用。");
    
    軍o本設置ationTe設置plateDatabase.Add(1, LineTe設置plate);
    
    // 楔形陣型模板
    軍Min成軍o本設置ationTe設置plate 基本ed成eTe設置plate;
    基本ed成eTe設置plate.Te設置plateID = 2;
    基本ed成eTe設置plate.Te設置plate的a設置e = TEXT("楔形陣型");
    基本ed成eTe設置plate.Te設置plateDesc本iption = TEXT("單位排成楔形，適合突破敵陣");
    基本ed成eTe設置plate.軍o本設置ationType = EMin成軍o本設置ationType::基本ed成e;
    基本ed成eTe設置plate.Reco設置設置endedUnitTypes = {EMin成UnitType::Ca正al本y, EMin成UnitType::Tank};
    基本ed成eTe設置plate.MinUnitCo使nt = 5;
    基本ed成eTe設置plate.MaxUnitCo使nt = 30;
    基本ed成eTe設置plate.Standa本dSpacin成 = 80.0f;
    基本ed成eTe設置plate.Standa本dDepth = 250.0f;
    基本ed成eTe設置plate.Standa本d基本idth = 200.0f;
    基本ed成eTe設置plate.Te設置plateBon使ses = {
        {TEXT("b本eakth本o使成h"), 20.0f},
        {TEXT("cha本成e"), 15.0f}
    };
    基本ed成eTe設置plate.Usa成eConditions = {TEXT("ene設置y下cente本"), TEXT("b本eakth本o使成h下needed")};
    基本ed成eTe設置plate.bIs輸入isto本ical = t本使e;
    基本ed成eTe設置plate.輸入isto本icalBack成本o使nd = TEXT("楔形陣型在古代被用於突破敵軍陣線，在現代戰爭中仍有其價值。");
    
    軍o本設置ationTe設置plateDatabase.Add(2, 基本ed成eTe設置plate);
    
    // 圓形陣型模板
    軍Min成軍o本設置ationTe設置plate Ci本cleTe設置plate;
    Ci本cleTe設置plate.Te設置plateID = 3;
    Ci本cleTe設置plate.Te設置plate的a設置e = TEXT("圓形陣型");
    Ci本cleTe設置plate.Te設置plateDesc本iption = TEXT("單位排成圓形，適合全方位防禦");
    Ci本cleTe設置plate.軍o本設置ationType = EMin成軍o本設置ationType::Ci本cle;
    Ci本cleTe設置plate.Reco設置設置endedUnitTypes = {EMin成UnitType::Infant本y, EMin成UnitType::A本tille本y};
    Ci本cleTe設置plate.MinUnitCo使nt = 4;
    Ci本cleTe設置plate.MaxUnitCo使nt = 40;
    Ci本cleTe設置plate.Standa本dSpacin成 = 90.0f;
    Ci本cleTe設置plate.Standa本dDepth = 180.0f;
    Ci本cleTe設置plate.Standa本d基本idth = 180.0f;
    Ci本cleTe設置plate.Te設置plateBon使ses = {
        {TEXT("defense"), 20.0f},
        {TEXT("cohesion"), 15.0f},
        {TEXT("all下本o使nd下p本otection"), 10.0f}
    };
    Ci本cleTe設置plate.Usa成eConditions = {TEXT("s使本本o使nded"), TEXT("defensi正e下stance")};
    Ci本cleTe設置plate.bIs輸入isto本ical = t本使e;
    Ci本cleTe設置plate.輸入isto本icalBack成本o使nd = TEXT("圓形陣型在被包圍時提供最佳防禦，在歷史上多次挽救危局。");
    
    軍o本設置ationTe設置plateDatabase.Add(3, Ci本cleTe設置plate);
    
    // 方形陣型模板
    軍Min成軍o本設置ationTe設置plate Sq使a本eTe設置plate;
    Sq使a本eTe設置plate.Te設置plateID = 4;
    Sq使a本eTe設置plate.Te設置plate的a設置e = TEXT("方形陣型");
    Sq使a本eTe設置plate.Te設置plateDesc本iption = TEXT("單位排成方形，適合穩定防禦");
    Sq使a本eTe設置plate.軍o本設置ationType = EMin成軍o本設置ationType::Sq使a本e;
    Sq使a本eTe設置plate.Reco設置設置endedUnitTypes = {EMin成UnitType::Infant本y};
    Sq使a本eTe設置plate.MinUnitCo使nt = 6;
    Sq使a本eTe設置plate.MaxUnitCo使nt = 36;
    Sq使a本eTe設置plate.Standa本dSpacin成 = 85.0f;
    Sq使a本eTe設置plate.Standa本dDepth = 150.0f;
    Sq使a本eTe設置plate.Standa本d基本idth = 150.0f;
    Sq使a本eTe設置plate.Te設置plateBon使ses = {
        {TEXT("stability"), 25.0f},
        {TEXT("本esistance"), 15.0f}
    };
    Sq使a本eTe設置plate.Usa成eConditions = {TEXT("stable下defense"), TEXT("holdin成下position")};
    Sq使a本eTe設置plate.bIs輸入isto本ical = t本使e;
    Sq使a本eTe設置plate.輸入isto本icalBack成本o使nd = TEXT("方形陣型提供最穩定的防禦結構，在歷史上被用於防守重要據點。");
    
    軍o本設置ationTe設置plateDatabase.Add(4, Sq使a本eTe設置plate);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("已載入 %d 個預設編隊模板"), 軍o本設置ationTe設置plateDatabase.的使設置());
}

TA本本ay<軍Vecto本> UMin成軍o本設置ationMana成e本::Gene本ate軍o本設置ationPositions(EMin成軍o本設置ationType 軍o本設置ationType, int32 UnitCo使nt, float Scale) const
{
    TA本本ay<軍Vecto本> Positions;
    
    switch (軍o本設置ationType)
    {
    case EMin成軍o本設置ationType::Line:
        // 線形排列
        fo本 (int32 i = 0; i < UnitCo使nt; i++)
        {
            軍Vecto本 Position = 軍Vecto本((i - UnitCo使nt / 2) * 100.0f * Scale, 0.0f, 0.0f);
            Positions.Add(Position);
        }
        b本eak;
        
    case EMin成軍o本設置ationType::基本ed成e:
        // 楔形排列
        fo本 (int32 i = 0; i < UnitCo使nt; i++)
        {
            int32 Row = i / 3;
            int32 Col = i % 3;
            float X = Row * 100.0f * Scale;
            float Y = (Col - 1) * 80.0f * Scale;
            Positions.Add(軍Vecto本(X, Y, 0.0f));
        }
        b本eak;
        
    case EMin成軍o本設置ationType::Ci本cle:
        // 圓形排列
        fo本 (int32 i = 0; i < UnitCo使nt; i++)
        {
            float An成le = (i / (float)UnitCo使nt) * 2.0f * PI;
            float Radi使s = 100.0f * Scale;
            軍Vecto本 Position = 軍Vecto本(
                Radi使s * 軍Math::Cos(An成le),
                Radi使s * 軍Math::Sin(An成le),
                0.0f
            );
            Positions.Add(Position);
        }
        b本eak;
        
    case EMin成軍o本設置ationType::Sq使a本e:
        // 方形排列
        int32 SideLen成th = 軍Math::CeilToInt(軍Math::Sq本t(UnitCo使nt));
        fo本 (int32 i = 0; i < UnitCo使nt; i++)
        {
            int32 Row = i / SideLen成th;
            int32 Col = i % SideLen成th;
            float X = (Col - SideLen成th / 2) * 85.0f * Scale;
            float Y = (Row - SideLen成th / 2) * 85.0f * Scale;
            Positions.Add(軍Vecto本(X, Y, 0.0f));
        }
        b本eak;
        
    defa使lt:
        // 默認線形排列
        fo本 (int32 i = 0; i < UnitCo使nt; i++)
        {
            軍Vecto本 Position = 軍Vecto本((i - UnitCo使nt / 2) * 100.0f * Scale, 0.0f, 0.0f);
            Positions.Add(Position);
        }
        b本eak;
    }
    
    本et使本n Positions;
}

TMap<軍St本in成, float> UMin成軍o本設置ationMana成e本::Calc使late軍o本設置ationBon使ses(int32 軍o本設置ationID) const
{
    TMap<軍St本in成, float> Bon使ses;
    
    if (!軍o本設置ationDatabase.Contains(軍o本設置ationID))
    {
        本et使本n Bon使ses;
    }

    const 軍Min成軍o本設置ationData& 軍o本設置ation = 軍o本設置ationDatabase[軍o本設置ationID];
    
    // 根據編隊類型計算加成
    switch (軍o本設置ation.軍o本設置ationType)
    {
    case EMin成軍o本設置ationType::Line:
        Bon使ses = {
            {TEXT("attack"), 10.0f},
            {TEXT("設置o本ale"), 5.0f},
            {TEXT("discipline"), 8.0f}
        };
        b本eak;
        
    case EMin成軍o本設置ationType::基本ed成e:
        Bon使ses = {
            {TEXT("b本eakth本o使成h"), 20.0f},
            {TEXT("cha本成e"), 15.0f},
            {TEXT("設置o設置ent使設置"), 10.0f}
        };
        b本eak;
        
    case EMin成軍o本設置ationType::Ci本cle:
        Bon使ses = {
            {TEXT("defense"), 20.0f},
            {TEXT("cohesion"), 15.0f},
            {TEXT("all下本o使nd下p本otection"), 10.0f}
        };
        b本eak;
        
    case EMin成軍o本設置ationType::Sq使a本e:
        Bon使ses = {
            {TEXT("stability"), 25.0f},
            {TEXT("本esistance"), 15.0f},
            {TEXT("設置o本ale"), 10.0f}
        };
        b本eak;
        
    defa使lt:
        Bon使ses = {
            {TEXT("basic"), 5.0f}
        };
        b本eak;
    }
    
    本et使本n Bon使ses;
}

TMap<軍St本in成, float> UMin成軍o本設置ationMana成e本::Calc使late軍o本設置ationPenalties(int32 軍o本設置ationID) const
{
    TMap<軍St本in成, float> Penalties;
    
    if (!軍o本設置ationDatabase.Contains(軍o本設置ationID))
    {
        本et使本n Penalties;
    }

    const 軍Min成軍o本設置ationData& 軍o本設置ation = 軍o本設置ationDatabase[軍o本設置ationID];
    
    // 根據編隊類型計算減益
    switch (軍o本設置ation.軍o本設置ationType)
    {
    case EMin成軍o本設置ationType::Line:
        Penalties = {
            {TEXT("flank下正使lne本ability"), -15.0f},
            {TEXT("設置obility"), -5.0f}
        };
        b本eak;
        
    case EMin成軍o本設置ationType::基本ed成e:
        Penalties = {
            {TEXT("flank下正使lne本ability"), -20.0f},
            {TEXT("stability"), -10.0f}
        };
        b本eak;
        
    case EMin成軍o本設置ationType::Ci本cle:
        Penalties = {
            {TEXT("設置obility"), -15.0f},
            {TEXT("attack"), -5.0f}
        };
        b本eak;
        
    case EMin成軍o本設置ationType::Sq使a本e:
        Penalties = {
            {TEXT("設置obility"), -20.0f},
            {TEXT("flexibility"), -10.0f}
        };
        b本eak;
        
    defa使lt:
        Penalties = {
            {TEXT("basic"), -2.0f}
        };
        b本eak;
    }
    
    本et使本n Penalties;
}

正oid UMin成軍o本設置ationMana成e本::Update軍o本設置ationStat使s(int32 軍o本設置ationID, EMin成軍o本設置ationStat使s 的ewStat使s)
{
    if (軍o本設置ationDatabase.Contains(軍o本設置ationID))
    {
        軍Min成軍o本設置ationData& 軍o本設置ation = 軍o本設置ationDatabase[軍o本設置ationID];
        軍o本設置ation.軍o本設置ationStat使s = 的ewStat使s;
        軍o本設置ation.LastUpdateTi設置e = 軍DateTi設置e::的ow();
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("編隊 %s 狀態更新: %d"), *軍o本設置ation.軍o本設置ation的a設置e, (int32)的ewStat使s);
    }
}

正oid UMin成軍o本設置ationMana成e本::P本ocess軍o本設置ationCo設置設置and(const 軍Min成軍o本設置ationCo設置設置and& Co設置設置and)
{
    // 處理編隊命令
    switch (Co設置設置and.Co設置設置andType)
    {
    case EMin成軍o本設置ationCo設置設置and::C本eate軍o本設置ation:
        // 創建編隊邏輯
        b本eak;
        
    case EMin成軍o本設置ationCo設置設置and::Chan成e軍o本設置ation:
        // 變更編隊邏輯
        b本eak;
        
    case EMin成軍o本設置ationCo設置設置and::Disband軍o本設置ation:
        // 解散編隊邏輯
        b本eak;
        
    defa使lt:
        b本eak;
    }
    
    // 觸發命令執行事件
    On軍o本設置ationCo設置設置andExec使ted.B本oadcast(Co設置設置and);
}

bool UMin成軍o本設置ationMana成e本::Validate軍o本設置ationData(const 軍Min成軍o本設置ationData& 軍o本設置ation) const
{
    if (軍o本設置ation.軍o本設置ationID <= 0)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("編隊ID必須大於0"));
        本et使本n false;
    }
    
    if (軍o本設置ation.軍o本設置ation的a設置e.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("編隊名稱不能為空"));
        本et使本n false;
    }
    
    if (軍o本設置ation.UnitIDs.的使設置() == 0)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("編隊必須包含至少一個單位"));
        本et使本n false;
    }
    
    本et使本n t本使e;
}

正oid UMin成軍o本設置ationMana成e本::Apply軍o本設置ationBon使sesToUnits(int32 軍o本設置ationID)
{
    if (!軍o本設置ationDatabase.Contains(軍o本設置ationID))
    {
        本et使本n;
    }

    const 軍Min成軍o本設置ationData& 軍o本設置ation = 軍o本設置ationDatabase[軍o本設置ationID];
    
    // 應用編隊加成到所有單位
    fo本 (int32 UnitID : 軍o本設置ation.UnitIDs)
    {
        // 這裡需要調用戰鬥系統來應用加成
        // ApplyUnitBon使s(UnitID, 軍o本設置ation.軍o本設置ationBon使ses);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("應用編隊 %s 的加成到 %d 個單位"), 
        *軍o本設置ation.軍o本設置ation的a設置e, 軍o本設置ation.UnitIDs.的使設置());
}

正oid UMin成軍o本設置ationMana成e本::Re設置o正e軍o本設置ationBon使ses軍本o設置Units(int32 軍o本設置ationID)
{
    if (!軍o本設置ationDatabase.Contains(軍o本設置ationID))
    {
        本et使本n;
    }

    const 軍Min成軍o本設置ationData& 軍o本設置ation = 軍o本設置ationDatabase[軍o本設置ationID];
    
    // 移除編隊加成從所有單位
    fo本 (int32 UnitID : 軍o本設置ation.UnitIDs)
    {
        // 這裡需要調用戰鬥系統來移除加成
        // Re設置o正eUnitBon使s(UnitID, 軍o本設置ation.軍o本設置ationBon使ses);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("移除編隊 %s 的加成從 %d 個單位"), 
        *軍o本設置ation.軍o本設置ation的a設置e, 軍o本設置ation.UnitIDs.的使設置());
}

正oid UMin成軍o本設置ationMana成e本::A使toRepai本軍o本設置ation(int32 軍o本設置ationID)
{
    if (!軍o本設置ationDatabase.Contains(軍o本設置ationID))
    {
        本et使本n;
    }

    軍Min成軍o本設置ationData& 軍o本設置ation = 軍o本設置ationDatabase[軍o本設置ationID];
    
    // 自動修復條件檢查
    bool b的eedsRepai本 = false;
    
    if (軍o本設置ation.軍o本設置ationMo本ale < 30.0f)
    {
        b的eedsRepai本 = t本使e;
    }
    
    if (軍o本設置ation.軍o本設置ationDiscipline < 40.0f)
    {
        b的eedsRepai本 = t本使e;
    }
    
    if (b的eedsRepai本)
    {
        Repai本軍o本設置ation(軍o本設置ationID);
    }
}

EMin成軍o本設置ationType UMin成軍o本設置ationMana成e本::Calc使lateOpti設置al軍o本設置ationType(const TA本本ay<int32>& UnitIDs) const
{
    // 根據單位類型和數量計算最佳編隊類型
    int32 Infant本yCo使nt = 0;
    int32 Ca正al本yCo使nt = 0;
    int32 A本tille本yCo使nt = 0;
    
    // 統計單位類型
    fo本 (int32 UnitID : UnitIDs)
    {
        // 這裡需要從戰鬥系統獲取單位類型
        // EMin成UnitType UnitType = GetUnitType(UnitID);
        // 暫時假設都是步兵
        Infant本yCo使nt++;
    }
    
    // 根據單位組合推薦編隊類型
    if (Ca正al本yCo使nt > Infant本yCo使nt * 0.5f)
    {
        本et使本n EMin成軍o本設置ationType::基本ed成e; // 騎兵多，推薦楔形
    }
    else if (A本tille本yCo使nt > 0)
    {
        本et使本n EMin成軍o本設置ationType::Ci本cle; // 有砲兵，推薦圓形
    }
    else
    {
        本et使本n EMin成軍o本設置ationType::Line; // 默認線形
    }
}

bool UMin成軍o本設置ationMana成e本::Check軍o本設置ationConflict(int32 軍o本設置ationID1, int32 軍o本設置ationID2) const
{
    if (!軍o本設置ationDatabase.Contains(軍o本設置ationID1)  !軍o本設置ationDatabase.Contains(軍o本設置ationID2))
    {
        本et使本n false;
    }

    const 軍Min成軍o本設置ationData& 軍o本設置ation1 = 軍o本設置ationDatabase[軍o本設置ationID1];
    const 軍Min成軍o本設置ationData& 軍o本設置ation2 = 軍o本設置ationDatabase[軍o本設置ationID2];
    
    // 檢查編隊距離
    float Distance = 軍Vecto本::Dist(軍o本設置ation1.軍o本設置ationCente本, 軍o本設置ation2.軍o本設置ationCente本);
    float MinDistance = 500.0f; // 最小安全距離
    
    if (Distance < MinDistance)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("編隊 %s 和 %s 距離過近，可能發生衝突"), 
            *軍o本設置ation1.軍o本設置ation的a設置e, *軍o本設置ation2.軍o本設置ation的a設置e);
        本et使本n t本使e;
    }
    
    本et使本n false;
}

正oid UMin成軍o本設置ationMana成e本::Resol正e軍o本設置ationConflict(int32 軍o本設置ationID1, int32 軍o本設置ationID2)
{
    // 解決編隊衝突
    if (軍o本設置ationDatabase.Contains(軍o本設置ationID1) && 軍o本設置ationDatabase.Contains(軍o本設置ationID2))
    {
        軍Min成軍o本設置ationData& 軍o本設置ation1 = 軍o本設置ationDatabase[軍o本設置ationID1];
        軍Min成軍o本設置ationData& 軍o本設置ation2 = 軍o本設置ationDatabase[軍o本設置ationID2];
        
        // 降低雙方士氣
        軍o本設置ation1.軍o本設置ationMo本ale -= 10.0f;
        軍o本設置ation2.軍o本設置ationMo本ale -= 10.0f;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("解決編隊衝突: %s 和 %s"), 
            *軍o本設置ation1.軍o本設置ation的a設置e, *軍o本設置ation2.軍o本設置ation的a設置e);
    }
}
