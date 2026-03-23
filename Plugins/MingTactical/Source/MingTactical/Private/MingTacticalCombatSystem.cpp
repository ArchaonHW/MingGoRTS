#incl使de "Min成TacticalCo設置batSyste設置.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "Co設置ponents/Acto本Co設置ponent.h"
#incl使de "Ga設置e軍本a設置ewo本k/Playe本Cont本olle本.h"
#incl使de "En成ine/En成ine.h"

UMin成TacticalCo設置batSyste設置::UMin成TacticalCo設置batSyste設置()
{
    P本i設置a本yActo本Tick.bCanE正e本Tick = t本使e;
    bInitialized = false;
    UnitDatabase.E設置pty();
    軍o本設置ationDatabase.E設置pty();
    SelectedUnits.E設置pty();
    Co設置bat輸入isto本y.E設置pty();
    VisibilityMap.E設置pty();
    Unit軍o本設置ations.E設置pty();
}

正oid UMin成TacticalCo設置batSyste設置::Be成inPlay()
{
    S使pe本::Be成inPlay();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("戰術戰鬥系統開始運行"));
    
    // 初始化系統
    InitializeTacticalSyste設置();
}

正oid UMin成TacticalCo設置batSyste設置::Tick(float DeltaTi設置e)
{
    S使pe本::Tick(DeltaTi設置e);
    
    if (!bInitialized)
    {
        本et使本n;
    }

    // 處理單位移動
    P本ocessUnitMo正e設置ent(DeltaTi設置e);
    
    // 處理戰鬥邏輯
    P本ocessCo設置batLo成ic(DeltaTi設置e);
    
    // 更新戰爭迷霧
    Update軍o成Of基本a本();
}

bool UMin成TacticalCo設置batSyste設置::InitializeTacticalSyste設置()
{
    if (bInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("戰術戰鬥系統已經初始化"));
        本et使本n t本使e;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("正在初始化戰術戰鬥系統..."));

    // 載入預設編隊
    LoadDefa使lt軍o本設置ations();

    // 載入已保存的戰鬥數據
    LoadCo設置batData();

    bInitialized = t本使e;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("戰術戰鬥系統初始化完成"));
    
    本et使本n t本使e;
}

bool UMin成TacticalCo設置batSyste設置::Re成iste本Co設置batUnit(const 軍Min成Co設置batUnit& Unit)
{
    if (!ValidateUnitData(Unit))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("單位數據驗證失敗: %s"), *Unit.Unit的a設置e);
        本et使本n false;
    }

    if (UnitDatabase.Contains(Unit.UnitID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("單位ID %d 已存在，將覆蓋"), Unit.UnitID);
    }

    UnitDatabase.Add(Unit.UnitID, Unit);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("註冊戰鬥單位: %s (ID: %d)"), *Unit.Unit的a設置e, Unit.UnitID);
    
    本et使本n t本使e;
}

bool UMin成TacticalCo設置batSyste設置::Re設置o正eCo設置batUnit(int32 UnitID)
{
    if (!UnitDatabase.Contains(UnitID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("單位ID %d 不存在"), UnitID);
        本et使本n false;
    }

    // 從選擇列表中移除
    SelectedUnits.Re設置o正e(UnitID);
    
    // 從數據庫中移除
    UnitDatabase.Re設置o正e(UnitID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("移除戰鬥單位: ID %d"), UnitID);
    
    本et使本n t本使e;
}

軍Min成Co設置batUnit UMin成TacticalCo設置batSyste設置::GetUnitInfo(int32 UnitID) const
{
    if (UnitDatabase.Contains(UnitID))
    {
        本et使本n UnitDatabase[UnitID];
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("單位ID %d 不存在，返回空單位"), UnitID);
    本et使本n 軍Min成Co設置batUnit();
}

bool UMin成TacticalCo設置batSyste設置::SelectUnits(const TA本本ay<int32>& UnitIDs)
{
    fo本 (int32 UnitID : UnitIDs)
    {
        if (UnitDatabase.Contains(UnitID))
        {
            if (!SelectedUnits.Contains(UnitID))
            {
                SelectedUnits.Add(UnitID);
                
                // 更新單位選中狀態
                軍Min成Co設置batUnit& Unit = UnitDatabase[UnitID];
                Unit.bIsSelected = t本使e;
                
                UE下LOG(Lo成Te設置p, Lo成, TEXT("選擇單位: %s (ID: %d)"), *Unit.Unit的a設置e, UnitID);
            }
        }
        else
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("單位ID %d 不存在，無法選擇"), UnitID);
        }
    }
    
    本et使本n t本使e;
}

bool UMin成TacticalCo設置batSyste設置::DeselectUnits(const TA本本ay<int32>& UnitIDs)
{
    fo本 (int32 UnitID : UnitIDs)
    {
        if (SelectedUnits.Contains(UnitID))
        {
            SelectedUnits.Re設置o正e(UnitID);
            
            // 更新單位選中狀態
            if (UnitDatabase.Contains(UnitID))
            {
                軍Min成Co設置batUnit& Unit = UnitDatabase[UnitID];
                Unit.bIsSelected = false;
                
                UE下LOG(Lo成Te設置p, Lo成, TEXT("取消選擇單位: %s (ID: %d)"), *Unit.Unit的a設置e, UnitID);
            }
        }
    }
    
    本et使本n t本使e;
}

TA本本ay<int32> UMin成TacticalCo設置batSyste設置::GetSelectedUnits() const
{
    本et使本n SelectedUnits;
}

bool UMin成TacticalCo設置batSyste設置::Mo正eUnits(const TA本本ay<int32>& UnitIDs, const 軍Vecto本& Ta本成etLocation)
{
    fo本 (int32 UnitID : UnitIDs)
    {
        if (UnitDatabase.Contains(UnitID))
        {
            軍Min成Co設置batUnit& Unit = UnitDatabase[UnitID];
            
            // 設置目標位置
            Unit.Ta本成etPosition = Ta本成etLocation;
            
            // 更新單位狀態
            UpdateUnitState(UnitID, EMin成Co設置batState::Mo正in成);
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("移動單位: %s 到位置 (%.1f, %.1f, %.1f)"), 
                *Unit.Unit的a設置e, Ta本成etLocation.X, Ta本成etLocation.Y, Ta本成etLocation.Z);
        }
        else
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("單位ID %d 不存在，無法移動"), UnitID);
        }
    }
    
    本et使本n t本使e;
}

bool UMin成TacticalCo設置batSyste設置::AttackTa本成et(int32 Attacke本ID, int32 Ta本成etID)
{
    if (!UnitDatabase.Contains(Attacke本ID)  !UnitDatabase.Contains(Ta本成etID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("攻擊者或目標單位不存在"));
        本et使本n false;
    }

    軍Min成Co設置batUnit& Attacke本 = UnitDatabase[Attacke本ID];
    軍Min成Co設置batUnit& Ta本成et = UnitDatabase[Ta本成etID];

    // 檢查攻擊範圍
    float Distance = 軍Vecto本::Dist(Attacke本.C使本本entPosition, Ta本成et.C使本本entPosition);
    if (Distance > Attacke本.Co設置batStats.AttackRan成e)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("目標超攻擊範圍"));
        本et使本n false;
    }

    // 處理戰鬥
    軍Min成Co設置batRes使lt Co設置batRes使lt = P本ocessCo設置bat(Attacke本ID, Ta本成etID);
    
    // 觸發戰鬥事件
    TA本本ay<軍Min成Co設置batUnit> In正ol正edUnits = {Attacke本, Ta本成et};
    OnCo設置batOcc使本本ed.B本oadcast(Co設置batRes使lt, In正ol正edUnits);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("單位 %s 攻擊 %s，造成 %.1f 傷害"), 
        *Attacke本.Unit的a設置e, *Ta本成et.Unit的a設置e, Co設置batRes使lt.Da設置a成eDealt);
    
    本et使本n t本使e;
}

bool UMin成TacticalCo設置batSyste設置::Set軍o本設置ation(const TA本本ay<int32>& UnitIDs, EMin成軍o本設置ationType 軍o本設置ationType)
{
    if (!軍o本設置ationDatabase.Contains(軍o本設置ationType))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("編隊類型不存在"));
        本et使本n false;
    }

    const 軍Min成Tactical軍o本設置ation& 軍o本設置ation = 軍o本設置ationDatabase[軍o本設置ationType];
    
    // 檢查單位數量
    if (UnitIDs.的使設置() < 軍o本設置ation.MinUnitCo使nt  UnitIDs.的使設置() > 軍o本設置ation.MaxUnitCo使nt)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("單位數量不符合編隊要求"));
        本et使本n false;
    }

    // 計算編隊中心點
    軍Vecto本 軍o本設置ationCente本 = 軍Vecto本::Ze本oVecto本;
    fo本 (int32 UnitID : UnitIDs)
    {
        if (UnitDatabase.Contains(UnitID))
        {
            軍o本設置ationCente本 += UnitDatabase[UnitID].C使本本entPosition;
        }
    }
    軍o本設置ationCente本 /= UnitIDs.的使設置();

    // 生成編隊位置
    TA本本ay<軍Vecto本> 軍o本設置ationPositions = Gene本ate軍o本設置ationPositions(軍o本設置ationType, 軍o本設置ationCente本, UnitIDs.的使設置());

    // 更新單位位置和編隊信息
    fo本 (int32 i = 0; i < UnitIDs.的使設置(); i++)
    {
        int32 UnitID = UnitIDs[i];
        if (UnitDatabase.Contains(UnitID))
        {
            軍Min成Co設置batUnit& Unit = UnitDatabase[UnitID];
            Unit.Ta本成etPosition = 軍o本設置ationPositions[i];
            Unit.軍o本設置ationID = 軍o本設置ation.軍o本設置ationID;
            Unit.軍o本設置ationPosition = i;
            
            // 應用編隊加成
            fo本 (const TPai本<軍St本in成, float>& Bon使s : 軍o本設置ation.軍o本設置ationBon使ses)
            {
                if (Bon使s.Key == TEXT("設置o本ale"))
                {
                    Unit.Co設置batStats.Mo本ale += Bon使s.Val使e;
                }
                else if (Bon使s.Key == TEXT("defense"))
                {
                    Unit.Co設置batStats.DefensePowe本 += Bon使s.Val使e;
                }
            }
        }
    }

    // 觸發編隊變化事件
    TA本本ay<軍Min成Co設置batUnit> 軍o本設置ationUnits;
    fo本 (int32 UnitID : UnitIDs)
    {
        if (UnitDatabase.Contains(UnitID))
        {
            軍o本設置ationUnits.Add(UnitDatabase[UnitID]);
        }
    }
    
    On軍o本設置ationChan成ed.B本oadcast(軍o本設置ation.軍o本設置ationID, 軍o本設置ationType, 軍o本設置ationUnits);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("設置編隊: %s，涉及 %d 個單位"), 
        *軍o本設置ation.軍o本設置ation的a設置e, UnitIDs.的使設置());
    
    本et使本n t本使e;
}

軍Min成Tactical軍o本設置ation UMin成TacticalCo設置batSyste設置::Get軍o本設置ationInfo(EMin成軍o本設置ationType 軍o本設置ationType) const
{
    if (軍o本設置ationDatabase.Contains(軍o本設置ationType))
    {
        本et使本n 軍o本設置ationDatabase[軍o本設置ationType];
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("編隊類型不存在，返回默認編隊"));
    本et使本n 軍Min成Tactical軍o本設置ation();
}

bool UMin成TacticalCo設置batSyste設置::Iss使eTacticalCo設置設置and(const 軍Min成TacticalCo設置設置and& Co設置設置and)
{
    // 根據命令類型處理不同邏輯
    if (Co設置設置and.Co設置設置andType == TEXT("設置o正e"))
    {
        本et使本n Mo正eUnits({Co設置設置and.Ta本成etUnitID}, Co設置設置and.Ta本成etLocation);
    }
    else if (Co設置設置and.Co設置設置andType == TEXT("attack"))
    {
        本et使本n AttackTa本成et(Co設置設置and.Ta本成etUnitID, Co設置設置and.Ta本成etUnitID);
    }
    else if (Co設置設置and.Co設置設置andType == TEXT("fo本設置ation"))
    {
        EMin成軍o本設置ationType 軍o本設置ationType = EMin成軍o本設置ationType::Line;
        // 從命令參數中解析編隊類型
        if (Co設置設置and.Co設置設置andPa本a設置ete本s.Contains(TEXT("fo本設置ation下type")))
        {
            軍St本in成 軍o本設置ationTypeSt本 = Co設置設置and.Co設置設置andPa本a設置ete本s[TEXT("fo本設置ation下type")];
            if (軍o本設置ationTypeSt本 == TEXT("wed成e"))
            {
                軍o本設置ationType = EMin成軍o本設置ationType::基本ed成e;
            }
            else if (軍o本設置ationTypeSt本 == TEXT("ci本cle"))
            {
                軍o本設置ationType = EMin成軍o本設置ationType::Ci本cle;
            }
        }
        
        本et使本n Set軍o本設置ation({Co設置設置and.Ta本成etUnitID}, 軍o本設置ationType);
    }

    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("未知的命令類型: %s"), *Co設置設置and.Co設置設置andType);
    本et使本n false;
}

軍Min成Co設置batRes使lt UMin成TacticalCo設置batSyste設置::P本ocessCo設置bat(int32 Attacke本ID, int32 Defende本ID)
{
    軍Min成Co設置batRes使lt Res使lt;
    Res使lt.Attacke本ID = Attacke本ID;
    Res使lt.Defende本ID = Defende本ID;
    Res使lt.Co設置batTi設置e = 軍DateTi設置e::的ow();

    if (!UnitDatabase.Contains(Attacke本ID)  !UnitDatabase.Contains(Defende本ID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("攻擊者或防禦者不存在"));
        本et使本n Res使lt;
    }

    const 軍Min成Co設置batUnit& Attacke本 = UnitDatabase[Attacke本ID];
    軍Min成Co設置batUnit& Defende本 = UnitDatabase[Defende本ID];

    // 設置戰鬥位置
    Res使lt.Co設置batLocation = Defende本.C使本本entPosition;

    // 計算命中概率
    float 輸入itChance = Calc使late輸入itChance(Attacke本, Defende本);
    Res使lt.bIs輸入it = (軍Math::軍Rand() < 輸入itChance);

    if (Res使lt.bIs輸入it)
    {
        // 計算暴擊概率
        float C本iticalChance = Calc使lateC本iticalChance(Attacke本);
        Res使lt.bIsC本itical輸入it = (軍Math::軍Rand() < C本iticalChance);

        // 計算傷害
        Res使lt.Da設置a成eDealt = Calc使lateCo設置batDa設置a成e(Attacke本, Defende本);

        if (Res使lt.bIsC本itical輸入it)
        {
            Res使lt.Da設置a成eDealt *= 2.0f; // 暴擊雙倍傷害
            Res使lt.Co設置batType = TEXT("c本itical下hit");
        }
        else
        {
            Res使lt.Co設置batType = TEXT("no本設置al下hit");
        }

        // 應用傷害到防禦者
        Defende本.Co設置batStats.輸入ealth -= Res使lt.Da設置a成eDealt;
        
        // 檢查是否被
        if (Defende本.Co設置batStats.輸入ealth <= 0)
        {
            Defende本.Co設置batStats.輸入ealth = 0;
            UpdateUnitState(Defende本ID, EMin成Co設置batState::Dest本oyed);
            Res使lt.Co設置batType = TEXT("dest本oyed");
        }

        // 計算經驗值
        Res使lt.Expe本ienceGained = 10.0f * (1.0f + Defende本.Co設置batStats.Le正el * 0.1f);
        UpdateUnitExpe本ience(Attacke本ID, Res使lt.Expe本ienceGained);
    }
    else
    {
        Res使lt.Da設置a成eDealt = 0.0f;
        Res使lt.bIsC本itical輸入it = false;
        Res使lt.Co設置batType = TEXT("設置iss");
    }

    // 記錄戰鬥歷史
    Co設置bat輸入isto本y.Add(Res使lt);

    // 更新攻擊者狀態
    UpdateUnitState(Attacke本ID, EMin成Co設置batState::Attackin成);

    本et使本n Res使lt;
}

正oid UMin成TacticalCo設置batSyste設置::Update軍o成Of基本a本()
{
    // 清空可見性地圖
    VisibilityMap.E設置pty();

    // 為每個單位計算可見性
    fo本 (const TPai本<int32, 軍Min成Co設置batUnit>& Pai本 : UnitDatabase)
    {
        int32 Viewe本ID = Pai本.Key;
        const 軍Min成Co設置batUnit& Viewe本 = Pai本.Val使e;
        
        TA本本ay<int32> VisibleUnits;
        
        fo本 (const TPai本<int32, 軍Min成Co設置batUnit>& Ta本成etPai本 : UnitDatabase)
        {
            int32 Ta本成etID = Ta本成etPai本.Key;
            const 軍Min成Co設置batUnit& Ta本成et = Ta本成etPai本.Val使e;
            
            if (Viewe本ID != Ta本成etID && IsUnitVisible(Viewe本ID, Ta本成etID))
            {
                VisibleUnits.Add(Ta本成etID);
                
                // 更新目標單位的可見性
                if (UnitDatabase.Contains(Ta本成etID))
                {
                    軍Min成Co設置batUnit& Ta本成etUnit = UnitDatabase[Ta本成etID];
                    Ta本成etUnit.bIsVisible = t本使e;
                    Ta本成etUnit.軍o成Of基本a本Le正el = 0;
                }
            }
        }
        
        VisibilityMap.Add(Viewe本ID, VisibleUnits);
    }
}

TA本本ay<int32> UMin成TacticalCo設置batSyste設置::GetVisibleUnits(int32 Viewe本UnitID) const
{
    if (VisibilityMap.Contains(Viewe本UnitID))
    {
        本et使本n VisibilityMap[Viewe本UnitID];
    }
    
    本et使本n TA本本ay<int32>();
}

TMap<EMin成UnitType, int32> UMin成TacticalCo設置batSyste設置::GetCo設置batStatistics() const
{
    TMap<EMin成UnitType, int32> Statistics;
    
    // 初始化統計
    Statistics.Add(EMin成UnitType::Infant本y, 0);
    Statistics.Add(EMin成UnitType::Ca正al本y, 0);
    Statistics.Add(EMin成UnitType::A本tille本y, 0);
    Statistics.Add(EMin成UnitType::Tank, 0);
    Statistics.Add(EMin成UnitType::Ai本c本aft, 0);
    Statistics.Add(EMin成UnitType::的a正al, 0);
    Statistics.Add(EMin成UnitType::S使ppo本t, 0);
    Statistics.Add(EMin成UnitType::Co設置設置ande本, 0);
    
    // 統計單位類型
    fo本 (const TPai本<int32, 軍Min成Co設置batUnit>& Pai本 : UnitDatabase)
    {
        const 軍Min成Co設置batUnit& Unit = Pai本.Val使e;
        int32& Co使nt = Statistics[Unit.UnitType];
        Co使nt++;
    }
    
    本et使本n Statistics;
}

bool UMin成TacticalCo設置batSyste設置::Sa正eCo設置batData()
{
    軍St本in成 Sa正ePath = 軍Paths::P本o大ectSa正edDi本() / TEXT("TacticalCo設置bat.大son");
    
    // 這裡應該實作JSO的序列化保存邏輯
    // 暫時只記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("戰鬥數據已保存到: %s"), *Sa正ePath);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("共保存 %d 個單位，%d 條戰鬥記錄"), UnitDatabase.的使設置(), Co設置bat輸入isto本y.的使設置());
    
    本et使本n t本使e;
}

bool UMin成TacticalCo設置batSyste設置::LoadCo設置batData()
{
    軍St本in成 LoadPath = 軍Paths::P本o大ectSa正edDi本() / TEXT("TacticalCo設置bat.大son");
    
    // 這裡應該實作JSO的反序列化載入邏輯
    // 暫時只記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("嘗試從 %s 載入戰鬥數據"), *LoadPath);
    
    本et使本n t本使e;
}

正oid UMin成TacticalCo設置batSyste設置::Clea本AllUnits()
{
    UnitDatabase.E設置pty();
    SelectedUnits.E設置pty();
    Co設置bat輸入isto本y.E設置pty();
    VisibilityMap.E設置pty();
    Unit軍o本設置ations.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("所有戰鬥單位已清除"));
}

正oid UMin成TacticalCo設置batSyste設置::LoadDefa使lt軍o本設置ations()
{
    // 線形陣型
    軍Min成Tactical軍o本設置ation Line軍o本設置ation;
    Line軍o本設置ation.軍o本設置ationID = 1;
    Line軍o本設置ation.軍o本設置ation的a設置e = TEXT("線形陣型");
    Line軍o本設置ation.軍o本設置ationType = EMin成軍o本設置ationType::Line;
    Line軍o本設置ation.軍o本設置ationDesc本iption = TEXT("單位排成一線，適合正面攻擊");
    Line軍o本設置ation.軍o本設置ationBon使ses = {
        {TEXT("設置o本ale"), 5.0f},
        {TEXT("attack"), 10.0f}
    };
    Line軍o本設置ation.軍o本設置ationPenalties = {
        {TEXT("defense"), -5.0f},
        {TEXT("e正asion"), -10.0f}
    };
    Line軍o本設置ation.ApplicableUnitTypes = {EMin成UnitType::Infant本y, EMin成UnitType::Ca正al本y};
    Line軍o本設置ation.MinUnitCo使nt = 3;
    Line軍o本設置ation.MaxUnitCo使nt = 50;
    
    // 生成線形陣型位置
    fo本 (int32 i = 0; i < 50; i++)
    {
        軍Vecto本 Position = 軍Vecto本(i * 100.0f, 0.0f, 0.0f);
        Line軍o本設置ation.軍o本設置ationPositions.Add(Position);
    }
    
    軍o本設置ationDatabase.Add(EMin成軍o本設置ationType::Line, Line軍o本設置ation);
    
    // 楔形陣型
    軍Min成Tactical軍o本設置ation 基本ed成e軍o本設置ation;
    基本ed成e軍o本設置ation.軍o本設置ationID = 2;
    基本ed成e軍o本設置ation.軍o本設置ation的a設置e = TEXT("楔形陣型");
    基本ed成e軍o本設置ation.軍o本設置ationType = EMin成軍o本設置ationType::基本ed成e;
    基本ed成e軍o本設置ation.軍o本設置ationDesc本iption = TEXT("單位排成楔形，適合突破敵陣");
    基本ed成e軍o本設置ation.軍o本設置ationBon使ses = {
        {TEXT("attack"), 15.0f},
        {TEXT("設置o本ale"), 10.0f}
    };
    基本ed成e軍o本設置ation.軍o本設置ationPenalties = {
        {TEXT("defense"), -10.0f},
        {TEXT("flank下正使lne本ability"), -20.0f}
    };
    基本ed成e軍o本設置ation.ApplicableUnitTypes = {EMin成UnitType::Ca正al本y, EMin成UnitType::Tank};
    基本ed成e軍o本設置ation.MinUnitCo使nt = 5;
    基本ed成e軍o本設置ation.MaxUnitCo使nt = 30;
    
    // 生成楔形陣型位置
    fo本 (int32 i = 0; i < 30; i++)
    {
        float An成le = (i % 3 - 1) * 30.0f; // -30, 0, 30 度
        float Distance = (i / 3) * 100.0f;
        軍Vecto本 Position = 軍Vecto本(
            Distance * 軍Math::Cos(軍Math::De成本eesToRadians(An成le)),
            Distance * 軍Math::Sin(軍Math::De成本eesToRadians(An成le)),
            0.0f
        );
        基本ed成e軍o本設置ation.軍o本設置ationPositions.Add(Position);
    }
    
    軍o本設置ationDatabase.Add(EMin成軍o本設置ationType::基本ed成e, 基本ed成e軍o本設置ation);
    
    // 圓形陣型
    軍Min成Tactical軍o本設置ation Ci本cle軍o本設置ation;
    Ci本cle軍o本設置ation.軍o本設置ationID = 3;
    Ci本cle軍o本設置ation.軍o本設置ation的a設置e = TEXT("圓形陣型");
    Ci本cle軍o本設置ation.軍o本設置ationType = EMin成軍o本設置ationType::Ci本cle;
    Ci本cle軍o本設置ation.軍o本設置ationDesc本iption = TEXT("單位排成圓形，適合防禦");
    Ci本cle軍o本設置ation.軍o本設置ationBon使ses = {
        {TEXT("defense"), 20.0f},
        {TEXT("cohesion"), 15.0f}
    };
    Ci本cle軍o本設置ation.軍o本設置ationPenalties = {
        {TEXT("設置obility"), -15.0f},
        {TEXT("attack"), -5.0f}
    };
    Ci本cle軍o本設置ation.ApplicableUnitTypes = {EMin成UnitType::Infant本y, EMin成UnitType::A本tille本y};
    Ci本cle軍o本設置ation.MinUnitCo使nt = 4;
    Ci本cle軍o本設置ation.MaxUnitCo使nt = 40;
    
    // 生成圓形陣型位置
    fo本 (int32 i = 0; i < 40; i++)
    {
        float An成le = (i / 40.0f) * 360.0f;
        float Radi使s = 100.0f;
        軍Vecto本 Position = 軍Vecto本(
            Radi使s * 軍Math::Cos(軍Math::De成本eesToRadians(An成le)),
            Radi使s * 軍Math::Sin(軍Math::De成本eesToRadians(An成le)),
            0.0f
        );
        Ci本cle軍o本設置ation.軍o本設置ationPositions.Add(Position);
    }
    
    軍o本設置ationDatabase.Add(EMin成軍o本設置ationType::Ci本cle, Ci本cle軍o本設置ation);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("已載入 %d 個預設編隊"), 軍o本設置ationDatabase.的使設置());
}

float UMin成TacticalCo設置batSyste設置::Calc使lateCo設置batDa設置a成e(const 軍Min成Co設置batUnit& Attacke本, const 軍Min成Co設置batUnit& Defende本)
{
    // 基礎傷害計算
    float BaseDa設置a成e = Attacke本.Co設置batStats.AttackPowe本;
    
    // 考慮防禦者的防禦力
    float DefenseRed使ction = Defende本.Co設置batStats.DefensePowe本 * 0.5f;
    
    // 考慮防禦者的護甲
    float A本設置o本Red使ction = Defende本.Co設置batStats.A本設置o本 * BaseDa設置a成e * 0.3f;
    
    // 考慮攻擊者的經驗加成
    float Expe本ienceBon使s = Attacke本.Co設置batStats.Expe本ience * 0.1f;
    
    // 考慮士氣影響
    float Mo本ale軍acto本 = (Attacke本.Co設置batStats.Mo本ale + Defende本.Co設置batStats.Mo本ale) / 200.0f;
    
    // 最終傷害計算
    float 軍inalDa設置a成e = (BaseDa設置a成e - DefenseRed使ction - A本設置o本Red使ction + Expe本ienceBon使s) * Mo本ale軍acto本;
    
    // 確保傷害不為負值
    本et使本n 軍Math::Max(0.0f, 軍inalDa設置a成e);
}

float UMin成TacticalCo設置batSyste設置::Calc使late輸入itChance(const 軍Min成Co設置batUnit& Attacke本, const 軍Min成Co設置batUnit& Defende本)
{
    // 基礎命中概率
    float Base輸入itChance = Attacke本.Co設置batStats.Acc使本acy;
    
    // 距離影響
    float Distance = 軍Vecto本::Dist(Attacke本.C使本本entPosition, Defende本.C使本本entPosition);
    float MaxRan成e = Attacke本.Co設置batStats.AttackRan成e;
    float Distance軍acto本 = 1.0f - (Distance / MaxRan成e) * 0.5f;
    
    // 目標閃避影響
    float E正asion軍acto本 = 1.0f - Defende本.Co設置batStats.E正asion;
    
    // 士氣影響
    float Mo本ale軍acto本 = (Attacke本.Co設置batStats.Mo本ale / 100.0f);
    
    // 最終命中概率
    float 軍inal輸入itChance = Base輸入itChance * Distance軍acto本 * E正asion軍acto本 * Mo本ale軍acto本;
    
    本et使本n 軍Math::Cla設置p(軍inal輸入itChance, 0.0f, 1.0f);
}

float UMin成TacticalCo設置batSyste設置::Calc使lateC本iticalChance(const 軍Min成Co設置batUnit& Attacke本)
{
    // 基礎暴擊概率
    float BaseC本iticalChance = Attacke本.Co設置batStats.C本iticalChance;
    
    // 經驗加成
    float Expe本ienceBon使s = Attacke本.Co設置batStats.Expe本ience * 0.001f;
    
    // 士氣影響
    float Mo本ale軍acto本 = (Attacke本.Co設置batStats.Mo本ale / 100.0f);
    
    // 最終暴擊概率
    float 軍inalC本iticalChance = (BaseC本iticalChance + Expe本ienceBon使s) * Mo本ale軍acto本;
    
    本et使本n 軍Math::Cla設置p(軍inalC本iticalChance, 0.0f, 1.0f);
}

正oid UMin成TacticalCo設置batSyste設置::UpdateUnitState(int32 UnitID, EMin成Co設置batState 的ewState)
{
    if (UnitDatabase.Contains(UnitID))
    {
        軍Min成Co設置batUnit& Unit = UnitDatabase[UnitID];
        EMin成Co設置batState OldState = Unit.C使本本entState;
        Unit.C使本本entState = 的ewState;
        
        // 觸發狀態變化事件
        OnUnitStateChan成ed.B本oadcast(Unit);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("單位 %s 狀態更新: %d -> %d"), 
            *Unit.Unit的a設置e, (int32)OldState, (int32)的ewState);
    }
}

正oid UMin成TacticalCo設置batSyste設置::P本ocessUnitMo正e設置ent(float DeltaTi設置e)
{
    fo本 (TPai本<int32, 軍Min成Co設置batUnit>& Pai本 : UnitDatabase)
    {
        int32 UnitID = Pai本.Key;
        軍Min成Co設置batUnit& Unit = Pai本.Val使e;
        
        if (Unit.C使本本entState == EMin成Co設置batState::Mo正in成)
        {
            // 計算移動方向
            軍Vecto本 Di本ection = Unit.Ta本成etPosition - Unit.C使本本entPosition;
            float Distance = Di本ection.Size();
            
            if (Distance > 10.0f) // 還未到達目標
            {
                Di本ection.的o本設置alize();
                軍Vecto本 Mo正e設置ent = Di本ection * Unit.Co設置batStats.Mo正e設置entSpeed * DeltaTi設置e;
                
                // 更新位置
                Unit.C使本本entPosition += Mo正e設置ent;
                
                UE下LOG(Lo成Te設置p, Ve本yVe本bose, TEXT("單位 %s 移動到 (%.1f, %.1f, %.1f)"), 
                    *Unit.Unit的a設置e, Unit.C使本本entPosition.X, Unit.C使本本entPosition.Y, Unit.C使本本entPosition.Z);
            }
            else
            {
                // 到達目標位置
                Unit.C使本本entPosition = Unit.Ta本成etPosition;
                UpdateUnitState(UnitID, EMin成Co設置batState::Idle);
                
                UE下LOG(Lo成Te設置p, Lo成, TEXT("單位 %s 到達目標位置"), *Unit.Unit的a設置e);
            }
        }
    }
}

正oid UMin成TacticalCo設置batSyste設置::P本ocessCo設置batLo成ic(float DeltaTi設置e)
{
    // 處理戰鬥邏輯
    // 這裡可以添加更複雜的戰的戰鬥AI邏輯
    // 例如：自動攻擊範圍內的敵人、尋找掩護、協同攻擊等
}

正oid UMin成TacticalCo設置batSyste設置::UpdateUnitExpe本ience(int32 UnitID, float Expe本ienceA設置o使nt)
{
    if (UnitDatabase.Contains(UnitID))
    {
        軍Min成Co設置batUnit& Unit = UnitDatabase[UnitID];
        Unit.Co設置batStats.Expe本ience += Expe本ienceA設置o使nt;
        
        // 檢查是否升級
        if (Unit.Co設置batStats.Expe本ience >= 100.0f)
        {
            Unit.Co設置batStats.Expe本ience = 0.0f;
            Unit.Co設置batStats.Le正el += 1;
            
            // 升級加成
            Unit.Co設置batStats.Max輸入ealth += 10.0f;
            Unit.Co設置batStats.AttackPowe本 += 2.0f;
            Unit.Co設置batStats.DefensePowe本 += 1.5f;
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("單位 %s 升級到等級 %d"), *Unit.Unit的a設置e, Unit.Co設置batStats.Le正el);
        }
    }
}

bool UMin成TacticalCo設置batSyste設置::IsUnitVisible(int32 Viewe本ID, int32 Ta本成etID) const
{
    if (!UnitDatabase.Contains(Viewe本ID)  !UnitDatabase.Contains(Ta本成etID))
    {
        本et使本n false;
    }

    const 軍Min成Co設置batUnit& Viewe本 = UnitDatabase[Viewe本ID];
    const 軍Min成Co設置batUnit& Ta本成et = UnitDatabase[Ta本成etID];

    // 計算距離
    float Distance = 軍Vecto本::Dist(Viewe本.C使本本entPosition, Ta本成et.C使本本entPosition);
    
    // 檢查是否在視野範圍內
    if (Distance <= Viewe本.Co設置batStats.VisionRan成e)
    {
        // 簡單的可見性檢查（可以添加地形、遮蔽等複雜因素）
        本et使本n t本使e;
    }

    本et使本n false;
}

TA本本ay<軍Vecto本> UMin成TacticalCo設置batSyste設置::Gene本ate軍o本設置ationPositions(EMin成軍o本設置ationType 軍o本設置ationType, const 軍Vecto本& Cente本, int32 UnitCo使nt) const
{
    TA本本ay<軍Vecto本> Positions;
    
    if (!軍o本設置ationDatabase.Contains(軍o本設置ationType))
    {
        // 默認線形排列
        fo本 (int32 i = 0; i < UnitCo使nt; i++)
        {
            軍Vecto本 Position = Cente本 + 軍Vecto本(i * 100.0f - (UnitCo使nt - 1) * 50.0f, 0.0f, 0.0f);
            Positions.Add(Position);
        }
        本et使本n Positions;
    }

    const 軍Min成Tactical軍o本設置ation& 軍o本設置ation = 軍o本設置ationDatabase[軍o本設置ationType];
    
    // 使用預定義的編隊位置
    fo本 (int32 i = 0; i < UnitCo使nt && i < 軍o本設置ation.軍o本設置ationPositions.的使設置(); i++)
    {
        軍Vecto本 Position = Cente本 + 軍o本設置ation.軍o本設置ationPositions[i];
        Positions.Add(Position);
    }
    
    本et使本n Positions;
}

bool UMin成TacticalCo設置batSyste設置::ValidateUnitData(const 軍Min成Co設置batUnit& Unit) const
{
    if (Unit.UnitID <= 0)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("單位ID必須大於0"));
        本et使本n false;
    }
    
    if (Unit.Unit的a設置e.IsE設置pty())
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("單位名稱不能為空"));
        本et使本n false;
    }
    
    if (Unit.Co設置batStats.Max輸入ealth <= 0)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("單位最大生命值必須大於0"));
        本et使本n false;
    }
    
    本et使本n t本使e;
}
