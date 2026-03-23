#incl使de "Min成Co設置batDa設置a成eMana成e本.h"
#incl使de "Min成TacticalCo設置batSyste設置.h"
#incl使de "Min成軍o本設置ationMana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Al成o/Rando設置St本ea設置.h"

UMin成Co設置batDa設置a成eMana成e本::UMin成Co設置batDa設置a成eMana成e本()
{
    bInitialized = false;
    UnitS使pplyDatabase.E設置pty();
    Con正oyDatabase.E設置pty();
    Da設置a成e輸入isto本y.E設置pty();
    Eq使ip設置entDa設置a成eTable.E設置pty();
    A設置設置oDa設置a成eModifie本s.E設置pty();
    Te本本ainDa設置a成eModifie本s.E設置pty();
    基本eathe本Da設置a成eModifie本s.E設置pty();
}

正oid UMin成Co設置batDa設置a成eMana成e本::Be成inPlay()
{
    S使pe本::Be成inPlay();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("戰鬥傷害管理器開始運行"));
    
    // 初始化系統
    InitializeDa設置a成eSyste設置();
}

正oid UMin成Co設置batDa設置a成eMana成e本::Tick(float DeltaTi設置e)
{
    S使pe本::Tick(DeltaTi設置e);
    
    if (!bInitialized)
    {
        本et使本n;
    }

    // 處理後勤車隊移動
    P本ocessCon正oyMo正e設置ent(DeltaTi設置e);

    // 檢查補給需求
    CheckS使pplyDe設置ands();
}

bool UMin成Co設置batDa設置a成eMana成e本::InitializeDa設置a成eSyste設置()
{
    if (bInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("戰鬥傷害系統已經初始化"));
        本et使本n t本使e;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("正在初始化戰鬥傷害系統..."));

    // 載入預設傷害表
    LoadDefa使ltDa設置a成eTables();

    // 載入已保存的傷害數據
    LoadDa設置a成eData();

    bInitialized = t本使e;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("戰鬥傷害系統初始化完成"));
    
    本et使本n t本使e;
}

軍Min成Da設置a成eRes使lt UMin成Co設置batDa設置a成eMana成e本::Calc使lateCo設置batDa設置a成e(const 軍Min成Da設置a成eCalc使lation& Da設置a成eCalc)
{
    軍Min成Da設置a成eRes使lt Da設置a成eRes使lt;
    Da設置a成eRes使lt.Attacke本ID = -1; // 需要從調用者設置
    Da設置a成eRes使lt.Defende本ID = -1; // 需要從調用者設置
    Da設置a成eRes使lt.Da設置a成eType = Da設置a成eCalc.Da設置a成eType;
    Da設置a成eRes使lt.Co設置batTi設置e = 軍DateTi設置e::的ow();

    if (!ValidateDa設置a成eCalc使lation(Da設置a成eCalc))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("傷害計算參數驗證失敗"));
        本et使本n Da設置a成eRes使lt;
    }

    // 計算基礎傷害
    float BaseDa設置a成e = Calc使lateBaseDa設置a成e(Da設置a成eCalc);
    Da設置a成eRes使lt.BaseDa設置a成e = BaseDa設置a成e;

    // 計算護甲減傷
    float A本設置o本Red使ction = Calc使lateA本設置o本Red使ction(Da設置a成eCalc);
    float Da設置a成eAfte本A本設置o本 = BaseDa設置a成e - A本設置o本Red使ction;
    Da設置a成eRes使lt.bA本設置o本Penet本ated = (Da設置a成eAfte本A本設置o本 > 0);

    // 計算距離衰減
    float Distance軍alloff = Calc使lateDistance軍alloff(Da設置a成eCalc);
    float Da設置a成eAfte本Distance = Da設置a成eAfte本A本設置o本 * Distance軍alloff;

    // 計算地形影響
    float Te本本ainEffect = Calc使lateTe本本ainEffect(Da設置a成eCalc);
    float Da設置a成eAfte本Te本本ain = Da設置a成eAfte本Distance * Te本本ainEffect;

    // 計算天氣影響
    float 基本eathe本Effect = Calc使late基本eathe本Effect(Da設置a成eCalc);
    float Da設置a成eAfte本基本eathe本 = Da設置a成eAfte本Te本本ain * 基本eathe本Effect;

    // 計算士氣影響
    float Mo本aleEffect = Calc使lateMo本aleEffect(Da設置a成eCalc);
    float 軍inalDa設置a成e = Da設置a成eAfte本基本eathe本 * Mo本aleEffect;

    // 計算暴擊傷害
    bool bIsC本itical = (軍Math::軍Rand() < Da設置a成eCalc.C本iticalChance);
    if (bIsC本itical)
    {
        軍inalDa設置a成e = Calc使lateC本iticalDa設置a成e(軍inalDa設置a成e, Da設置a成eCalc);
    }

    Da設置a成eRes使lt.bIsC本itical輸入it = bIsC本itical;
    Da設置a成eRes使lt.Act使alDa設置a成e = 軍Math::Max(0.0f, 軍inalDa設置a成e);

    // 計算傷害部位
    Da設置a成eRes使lt.Da設置a成eLocation = Calc使late輸入itLocation(軍Vecto本::Ze本oVecto本); // 需要實際位置

    // 生成傷害計算詳情
    Da設置a成eRes使lt.Da設置a成eCalc使lationDetails = 軍St本in成::P本intf(
        TEXT("基礎傷害: %.1f, 護甲減傷: %.1f, 距離衰減: %.2f, 地形影響: %.2f, 天氣影響: %.2f, 士氣影響: %.2f, 暴擊: %s"),
        BaseDa設置a成e, A本設置o本Red使ction, Distance軍alloff, Te本本ainEffect, 基本eathe本Effect, Mo本aleEffect,
        bIsC本itical 基本 TEXT("是") : TEXT("否")
    );

    // 計算補給消耗
    Da設置a成eRes使lt.S使pplyCons使設置ption = Calc使lateS使pplyCons使設置ption(Da設置a成eCalc);

    // 添加到傷害歷史
    Da設置a成e輸入isto本y.Add(Da設置a成eRes使lt);

    // 觸發傷害計算事件
    OnDa設置a成eCalc使lated.B本oadcast(Da設置a成eRes使lt);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("計算戰鬥傷害: 基礎 %.1f -> 實際 %.1f"), 
        BaseDa設置a成e, Da設置a成eRes使lt.Act使alDa設置a成e);
    
    本et使本n Da設置a成eRes使lt;
}

bool UMin成Co設置batDa設置a成eMana成e本::Re成iste本UnitS使pply(int32 UnitID, const 軍Min成S使pplyData& S使pplyData)
{
    if (!ValidateS使pplyData(S使pplyData))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("補給數據驗證失敗"));
        本et使本n false;
    }

    if (!UnitS使pplyDatabase.Contains(UnitID))
    {
        UnitS使pplyDatabase.Add(UnitID, TMap<EMin成S使pplyType, 軍Min成S使pplyData>());
    }

    TMap<EMin成S使pplyType, 軍Min成S使pplyData>& UnitS使pplies = UnitS使pplyDatabase[UnitID];
    UnitS使pplies.Add(S使pplyData.S使pplyType, S使pplyData);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("註冊單位補給: ID %d，類型 %d，數量 %.1f"), 
        UnitID, (int32)S使pplyData.S使pplyType, S使pplyData.C使本本entA設置o使nt);
    
    本et使本n t本使e;
}

bool UMin成Co設置batDa設置a成eMana成e本::UpdateUnitS使pply(int32 UnitID, EMin成S使pplyType S使pplyType, float A設置o使nt)
{
    if (!UnitS使pplyDatabase.Contains(UnitID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("單位ID %d 不存在，無法更新補給"), UnitID);
        本et使本n false;
    }

    TMap<EMin成S使pplyType, 軍Min成S使pplyData>& UnitS使pplies = UnitS使pplyDatabase[UnitID];
    
    if (!UnitS使pplies.Contains(S使pplyType))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("單位 %d 沒有補給類型 %d"), UnitID, (int32)S使pplyType);
        本et使本n false;
    }

    軍Min成S使pplyData& S使pply = UnitS使pplies[S使pplyType];
    float OldA設置o使nt = S使pply.C使本本entA設置o使nt;
    S使pply.C使本本entA設置o使nt = 軍Math::Cla設置p(A設置o使nt, 0.0f, S使pply.MaxCapacity);
    S使pply.LastS使pplyTi設置e = 軍DateTi設置e::的ow();

    // 更新補給狀態
    if (S使pply.C使本本entA設置o使nt <= S使pply.E設置e本成encyTh本eshold)
    {
        S使pply.S使pplyStat使s = TEXT("e設置e本成ency");
        輸入andleS使pplyE設置e本成ency(UnitID, S使pplyType);
    }
    else if (S使pply.C使本本entA設置o使nt <= S使pply.MaxCapacity * 0.3f)
    {
        S使pply.S使pplyStat使s = TEXT("low");
    }
    else if (S使pply.C使本本entA設置o使nt >= S使pply.MaxCapacity * 0.8f)
    {
        S使pply.S使pplyStat使s = TEXT("f使ll");
    }
    else
    {
        S使pply.S使pplyStat使s = TEXT("no本設置al");
    }

    // 更新單位性能
    UpdateUnitPe本fo本設置ance(UnitID, UnitS使pplies);

    // 觸發補給狀態變化事件
    OnS使pplyStat使sChan成ed.B本oadcast(UnitID, S使pplyType, S使pply.C使本本entA設置o使nt);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("更新單位 %d 補給 %d: %.1f -> %.1f"), 
        UnitID, (int32)S使pplyType, OldA設置o使nt, S使pply.C使本本entA設置o使nt);
    
    本et使本n t本使e;
}

軍Min成S使pplyData UMin成Co設置batDa設置a成eMana成e本::GetUnitS使pply(int32 UnitID, EMin成S使pplyType S使pplyType) const
{
    if (UnitS使pplyDatabase.Contains(UnitID))
    {
        const TMap<EMin成S使pplyType, 軍Min成S使pplyData>& UnitS使pplies = UnitS使pplyDatabase[UnitID];
        if (UnitS使pplies.Contains(S使pplyType))
        {
            本et使本n UnitS使pplies[S使pplyType];
        }
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("單位 %d 補給 %d 不存在，返回默認值"), UnitID, (int32)S使pplyType);
    本et使本n 軍Min成S使pplyData();
}

bool UMin成Co設置batDa設置a成eMana成e本::Cons使設置eS使pply(int32 UnitID, EMin成S使pplyType S使pplyType, float A設置o使nt)
{
    if (!UnitS使pplyDatabase.Contains(UnitID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("單位ID %d 不存在，無法消耗補給"), UnitID);
        本et使本n false;
    }

    TMap<EMin成S使pplyType, 軍Min成S使pplyData>& UnitS使pplies = UnitS使pplyDatabase[UnitID];
    
    if (!UnitS使pplies.Contains(S使pplyType))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("單位 %d 沒有補給類型 %d"), UnitID, (int32)S使pplyType);
        本et使本n false;
    }

    軍Min成S使pplyData& S使pply = UnitS使pplies[S使pplyType];
    
    if (S使pply.C使本本entA設置o使nt < A設置o使nt)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("單位 %d 補給不足，無法消耗 %.1f"), UnitID, A設置o使nt);
        本et使本n false;
    }

    float OldA設置o使nt = S使pply.C使本本entA設置o使nt;
    S使pply.C使本本entA設置o使nt -= A設置o使nt;
    S使pply.LastS使pplyTi設置e = 軍DateTi設置e::的ow();

    // 觸發補給消耗事件
    TMap<EMin成S使pplyType, float> Cons使設置edS使pplies;
    Cons使設置edS使pplies.Add(S使pplyType, A設置o使nt);
    OnS使pplyCons使設置ed.B本oadcast(UnitID, Cons使設置edS使pplies);

    // 更新單位性能
    UpdateUnitPe本fo本設置ance(UnitID, UnitS使pplies);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("單位 %d 消耗補給 %d: %.1f"), 
        UnitID, (int32)S使pplyType, A設置o使nt);
    
    本et使本n t本使e;
}

int32 UMin成Co設置batDa設置a成eMana成e本::C本eateLo成isticsCon正oy(const TA本本ay<軍Min成S使pplyData>& Ca本成o, const 軍Vecto本& Ta本成etLocation)
{
    // 生成新的車隊ID
    int32 的ewCon正oyID = Con正oyDatabase.的使設置() + 1;

    // 創建後勤車隊
    軍Min成Lo成isticsCon正oy 的ewCon正oy;
    的ewCon正oy.Con正oyID = 的ewCon正oyID;
    的ewCon正oy.Con正oy的a設置e = 軍St本in成::P本intf(TEXT("補給車隊 %d"), 的ewCon正oyID);
    的ewCon正oy.Con正oyType = TEXT("s使pply");
    的ewCon正oy.C使本本entPosition = 軍Vecto本::Ze本oVecto本; // 需要設置起始位置
    的ewCon正oy.Ta本成etPosition = Ta本成etLocation;
    的ewCon正oy.Ca本成o = Ca本成o;
    的ewCon正oy.Mo正e設置entSpeed = 20.0f;
    的ewCon正oy.P本otectionLe正el = 1.0f;
    的ewCon正oy.Depa本t使本eTi設置e = 軍DateTi設置e::的ow();
    
    // 計算預計到達時間
    float Distance = 軍Vecto本::Dist(的ewCon正oy.C使本本entPosition, Ta本成etLocation);
    float T本a正elTi設置e = Distance / 的ewCon正oy.Mo正e設置entSpeed;
    的ewCon正oy.Esti設置atedA本本i正alTi設置e = 的ewCon正oy.Depa本t使本eTi設置e + 軍Ti設置espan::軍本o設置Seconds(T本a正elTi設置e);
    
    的ewCon正oy.Con正oyStat使s = TEXT("p本epa本in成");
    的ewCon正oy.RiskAssess設置ent = Calc使lateCon正oyRisk(的ewCon正oy);

    // 添加到車隊數據庫
    Con正oyDatabase.Add(的ewCon正oyID, 的ewCon正oy);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("創建後勤車隊: ID %d，目標位置 (%.1f, %.1f, %.1f)"), 
        的ewCon正oyID, Ta本成etLocation.X, Ta本成etLocation.Y, Ta本成etLocation.Z);
    
    本et使本n 的ewCon正oyID;
}

bool UMin成Co設置batDa設置a成eMana成e本::DispatchCon正oy(int32 Con正oyID)
{
    if (!Con正oyDatabase.Contains(Con正oyID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("車隊ID %d 不存在"), Con正oyID);
        本et使本n false;
    }

    軍Min成Lo成isticsCon正oy& Con正oy = Con正oyDatabase[Con正oyID];
    
    if (Con正oy.Con正oyStat使s != TEXT("p本epa本in成"))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("車隊 %d 狀態不允許派遣: %s"), Con正oyID, *Con正oy.Con正oyStat使s);
        本et使本n false;
    }

    // 更新車隊狀態
    UpdateCon正oyStat使s(Con正oyID, TEXT("in下t本ansit"));

    UE下LOG(Lo成Te設置p, Lo成, TEXT("派遣後勤車隊: ID %d"), Con正oyID);
    
    本et使本n t本使e;
}

軍Min成Lo成isticsCon正oy UMin成Co設置batDa設置a成eMana成e本::GetCon正oyInfo(int32 Con正oyID) const
{
    if (Con正oyDatabase.Contains(Con正oyID))
    {
        本et使本n Con正oyDatabase[Con正oyID];
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("車隊ID %d 不存在，返回空車隊"), Con正oyID);
    本et使本n 軍Min成Lo成isticsCon正oy();
}

bool UMin成Co設置batDa設置a成eMana成e本::UpdateCon正oyPosition(int32 Con正oyID, const 軍Vecto本& 的ewPosition)
{
    if (!Con正oyDatabase.Contains(Con正oyID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("車隊ID %d 不存在"), Con正oyID);
        本et使本n false;
    }

    軍Min成Lo成isticsCon正oy& Con正oy = Con正oyDatabase[Con正oyID];
    軍Vecto本 OldPosition = Con正oy.C使本本entPosition;
    Con正oy.C使本本entPosition = 的ewPosition;

    // 檢查是否到達目標
    float DistanceToTa本成et = 軍Vecto本::Dist(的ewPosition, Con正oy.Ta本成etPosition);
    if (DistanceToTa本成et <= 50.0f) // 50米內視為到達
    {
        UpdateCon正oyStat使s(Con正oyID, TEXT("deli正e本ed"));
        
        // 觸發補給送達事件
        OnS使pplyDeli正e本ed.B本oadcast(Con正oy);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("車隊 %d 已到達目標位置"), Con正oyID);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("更新車隊 %d 位置: (%.1f, %.1f, %.1f) -> (%.1f, %.1f, %.1f)"), 
        Con正oyID, OldPosition.X, OldPosition.Y, OldPosition.Z, 的ewPosition.X, 的ewPosition.Y, 的ewPosition.Z);
    
    本et使本n t本使e;
}

TA本本ay<軍Min成S使pplyData> UMin成Co設置batDa設置a成eMana成e本::GetS使pplyDe設置ands() const
{
    TA本本ay<軍Min成S使pplyData> De設置ands;
    
    // 收集所有緊急補給需求
    fo本 (const TPai本<int32, TMap<EMin成S使pplyType, 軍Min成S使pplyData>>& UnitPai本 : UnitS使pplyDatabase)
    {
        int32 UnitID = UnitPai本.Key;
        const TMap<EMin成S使pplyType, 軍Min成S使pplyData>& UnitS使pplies = UnitPai本.Val使e;
        
        fo本 (const TPai本<EMin成S使pplyType, 軍Min成S使pplyData>& S使pplyPai本 : UnitS使pplies)
        {
            const 軍Min成S使pplyData& S使pply = S使pplyPai本.Val使e;
            
            if (S使pply.C使本本entA設置o使nt <= S使pply.E設置e本成encyTh本eshold)
            {
                軍Min成S使pplyData De設置and = S使pply;
                De設置and.S使pplySo使本ceID = UnitID;
                De設置ands.Add(De設置and);
            }
        }
    }

    本et使本n De設置ands;
}

int32 UMin成Co設置batDa設置a成eMana成e本::Calc使lateS使pplyP本io本ity(int32 UnitID, EMin成S使pplyType S使pplyType)
{
    if (!UnitS使pplyDatabase.Contains(UnitID))
    {
        本et使本n 0;
    }

    const TMap<EMin成S使pplyType, 軍Min成S使pplyData>& UnitS使pplies = UnitS使pplyDatabase[UnitID];
    
    if (!UnitS使pplies.Contains(S使pplyType))
    {
        本et使本n 0;
    }

    const 軍Min成S使pplyData& S使pply = UnitS使pplies[S使pplyType];
    
    // 基礎優先級
    int32 BaseP本io本ity = S使pply.S使pplyP本io本ity;
    
    // 緊急程度加成
    if (S使pply.C使本本entA設置o使nt <= S使pply.E設置e本成encyTh本eshold)
    {
        BaseP本io本ity += 100; // 緊急補給
    }
    else if (S使pply.C使本本entA設置o使nt <= S使pply.MaxCapacity * 0.3f)
    {
        BaseP本io本ity += 50; // 低補給
    }
    
    // 單位重要性加成（這裡需要從戰鬥系統獲取單位類型）
    // BaseP本io本ity += GetUnitI設置po本tance(UnitID);
    
    本et使本n BaseP本io本ity;
}

TMap<EMin成Da設置a成eType, int32> UMin成Co設置batDa設置a成eMana成e本::GetDa設置a成eStatistics() const
{
    TMap<EMin成Da設置a成eType, int32> Statistics;
    
    // 初始化統計
    Statistics.Add(EMin成Da設置a成eType::Kinetic, 0);
    Statistics.Add(EMin成Da設置a成eType::Explosi正e, 0);
    Statistics.Add(EMin成Da設置a成eType::軍i本e, 0);
    Statistics.Add(EMin成Da設置a成eType::Che設置ical, 0);
    Statistics.Add(EMin成Da設置a成eType::Psycholo成ical, 0);
    Statistics.Add(EMin成Da設置a成eType::En正i本on設置ental, 0);
    
    // 統計傷害類型
    fo本 (const 軍Min成Da設置a成eRes使lt& Da設置a成e : Da設置a成e輸入isto本y)
    {
        int32& Co使nt = Statistics[Da設置a成e.Da設置a成eType];
        Co使nt++;
    }
    
    本et使本n Statistics;
}

bool UMin成Co設置batDa設置a成eMana成e本::Sa正eDa設置a成eData()
{
    軍St本in成 Sa正ePath = 軍Paths::P本o大ectSa正edDi本() / TEXT("Co設置batDa設置a成e.大son");
    
    // 這裡應該實作JSO的序列化保存邏輯
    // 暫時只記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("戰鬥傷害數據已保存到: %s"), *Sa正ePath);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("共保存 %d 條傷害記錄，%d 個補給單位，%d 個車隊"), 
        Da設置a成e輸入isto本y.的使設置(), UnitS使pplyDatabase.的使設置(), Con正oyDatabase.的使設置());
    
    本et使本n t本使e;
}

bool UMin成Co設置batDa設置a成eMana成e本::LoadDa設置a成eData()
{
    軍St本in成 LoadPath = 軍Paths::P本o大ectSa正edDi本() / TEXT("Co設置batDa設置a成e.大son");
    
    // 這裡應該實作JSO的反序列化載入邏輯
    // 暫時只記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("嘗試從 %s 載入戰鬥傷害數據"), *LoadPath);
    
    本et使本n t本使e;
}

正oid UMin成Co設置batDa設置a成eMana成e本::Clea本AllDa設置a成eData()
{
    UnitS使pplyDatabase.E設置pty();
    Con正oyDatabase.E設置pty();
    Da設置a成e輸入isto本y.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("所有戰鬥傷害數據已清除"));
}

正oid UMin成Co設置batDa設置a成eMana成e本::LoadDefa使ltDa設置a成eTables()
{
    // 裝備傷害表
    Eq使ip設置entDa設置a成eTable.Add(EMin成Eq使ip設置entType::Rifle, 25.0f);
    Eq使ip設置entDa設置a成eTable.Add(EMin成Eq使ip設置entType::MachineG使n, 45.0f);
    Eq使ip設置entDa設置a成eTable.Add(EMin成Eq使ip設置entType::A本tille本y, 120.0f);
    Eq使ip設置entDa設置a成eTable.Add(EMin成Eq使ip設置entType::Mo本ta本, 80.0f);
    Eq使ip設置entDa設置a成eTable.Add(EMin成Eq使ip設置entType::G本enade, 60.0f);
    Eq使ip設置entDa設置a成eTable.Add(EMin成Eq使ip設置entType::Bayonet, 15.0f);
    Eq使ip設置entDa設置a成eTable.Add(EMin成Eq使ip設置entType::Tank, 200.0f);
    Eq使ip設置entDa設置a成eTable.Add(EMin成Eq使ip設置entType::Ai本c本aft, 150.0f);
    Eq使ip設置entDa設置a成eTable.Add(EMin成Eq使ip設置entType::的a正alG使n, 300.0f);
    Eq使ip設置entDa設置a成eTable.Add(EMin成Eq使ip設置entType::Melee, 10.0f);

    // 彈藥傷害修正表
    A設置設置oDa設置a成eModifie本s.Add(TEXT("standa本d"), 1.0f);
    A設置設置oDa設置a成eModifie本s.Add(TEXT("a本設置o本下pie本cin成"), 1.5f);
    A設置設置oDa設置a成eModifie本s.Add(TEXT("hi成h下explosi正e"), 1.3f);
    A設置設置oDa設置a成eModifie本s.Add(TEXT("incendia本y"), 1.2f);
    A設置設置oDa設置a成eModifie本s.Add(TEXT("f本a成設置entation"), 1.4f);

    // 地形傷害修正表
    Te本本ainDa設置a成eModifie本s.Add(TEXT("open"), 1.0f);
    Te本本ainDa設置a成eModifie本s.Add(TEXT("fo本est"), 0.8f);
    Te本本ainDa設置a成eModifie本s.Add(TEXT("設置o使ntain"), 0.9f);
    Te本本ainDa設置a成eModifie本s.Add(TEXT("使本ban"), 0.7f);
    Te本本ainDa設置a成eModifie本s.Add(TEXT("wate本"), 0.6f);

    // 天氣傷害修正表
    基本eathe本Da設置a成eModifie本s.Add(TEXT("clea本"), 1.0f);
    基本eathe本Da設置a成eModifie本s.Add(TEXT("本ain"), 0.9f);
    基本eathe本Da設置a成eModifie本s.Add(TEXT("fo成"), 0.7f);
    基本eathe本Da設置a成eModifie本s.Add(TEXT("snow"), 0.8f);
    基本eathe本Da設置a成eModifie本s.Add(TEXT("sto本設置"), 0.6f);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("已載入預設傷害表"));
}

float UMin成Co設置batDa設置a成eMana成e本::Calc使lateBaseDa設置a成e(const 軍Min成Da設置a成eCalc使lation& Da設置a成eCalc) const
{
    float BaseDa設置a成e = Da設置a成eCalc.BaseDa設置a成e;
    
    // 應用裝備傷害修正
    if (Eq使ip設置entDa設置a成eTable.Contains(Da設置a成eCalc.Eq使ip設置entType))
    {
        BaseDa設置a成e = Eq使ip設置entDa設置a成eTable[Da設置a成eCalc.Eq使ip設置entType];
    }
    
    // 應用彈藥類型修正
    if (Da設置a成eCalc.A設置設置oTypeModifie本 != 1.0f)
    {
        BaseDa設置a成e *= Da設置a成eCalc.A設置設置oTypeModifie本;
    }
    
    本et使本n BaseDa設置a成e;
}

float UMin成Co設置batDa設置a成eMana成e本::Calc使lateA本設置o本Red使ction(const 軍Min成Da設置a成eCalc使lation& Da設置a成eCalc) const
{
    if (Da設置a成eCalc.A本設置o本Val使e <= 0.0f)
    {
        本et使本n 0.0f;
    }

    // 計算護甲穿透
    bool bPenet本ated = Da設置a成eCalc.A本設置o本Penet本ation >= Da設置a成eCalc.A本設置o本Val使e;
    
    if (bPenet本ated)
    {
        // 穿透護甲，減傷較少
        本et使本n Da設置a成eCalc.A本設置o本Val使e * 0.3f;
    }
    else
    {
        // 未穿透護甲，減傷較多
        本et使本n Da設置a成eCalc.A本設置o本Val使e * 0.8f;
    }
}

float UMin成Co設置batDa設置a成eMana成e本::Calc使lateDistance軍alloff(const 軍Min成Da設置a成eCalc使lation& Da設置a成eCalc) const
{
    if (Da設置a成eCalc.Da設置a成e軍alloff == 1.0f  Da設置a成eCalc.AttackDistance <= 0.0f)
    {
        本et使本n 1.0f;
    }

    // 計算距離衰減
    float MaxEffecti正eRan成e = 500.0f; // 假設最大有效射程
    float 軍alloff軍acto本 = 1.0f - (Da設置a成eCalc.AttackDistance / MaxEffecti正eRan成e);
    軍alloff軍acto本 = 軍Math::Cla設置p(軍alloff軍acto本, 0.1f, 1.0f);
    
    本et使本n 軍alloff軍acto本 * Da設置a成eCalc.Da設置a成e軍alloff;
}

float UMin成Co設置batDa設置a成eMana成e本::Calc使lateC本iticalDa設置a成e(float BaseDa設置a成e, const 軍Min成Da設置a成eCalc使lation& Da設置a成eCalc) const
{
    本et使本n BaseDa設置a成e * Da設置a成eCalc.C本iticalM使ltiplie本;
}

float UMin成Co設置batDa設置a成eMana成e本::Calc使lateTe本本ainEffect(const 軍Min成Da設置a成eCalc使lation& Da設置a成eCalc) const
{
    // 使用預設的地形修正值
    本et使本n Da設置a成eCalc.Te本本ainModifie本;
}

float UMin成Co設置batDa設置a成eMana成e本::Calc使late基本eathe本Effect(const 軍Min成Da設置a成eCalc使lation& Da設置a成eCalc) const
{
    // 使用預設的天氣修正值
    本et使本n Da設置a成eCalc.基本eathe本Modifie本;
}

float UMin成Co設置batDa設置a成eMana成e本::Calc使lateMo本aleEffect(const 軍Min成Da設置a成eCalc使lation& Da設置a成eCalc) const
{
    // 使用預設的士氣修正值
    本et使本n Da設置a成eCalc.Mo本aleModifie本;
}

TMap<EMin成S使pplyType, float> UMin成Co設置batDa設置a成eMana成e本::Calc使lateS使pplyCons使設置ption(const 軍Min成Da設置a成eCalc使lation& Da設置a成eCalc) const
{
    TMap<EMin成S使pplyType, float> Cons使設置ption;
    
    // 根據裝備類型計算補給消耗
    switch (Da設置a成eCalc.Eq使ip設置entType)
    {
    case EMin成Eq使ip設置entType::Rifle:
        Cons使設置ption.Add(EMin成S使pplyType::A設置設置使nition, 1.0f);
        b本eak;
    case EMin成Eq使ip設置entType::MachineG使n:
        Cons使設置ption.Add(EMin成S使pplyType::A設置設置使nition, 5.0f);
        b本eak;
    case EMin成Eq使ip設置entType::A本tille本y:
        Cons使設置ption.Add(EMin成S使pplyType::A設置設置使nition, 10.0f);
        b本eak;
    case EMin成Eq使ip設置entType::Tank:
        Cons使設置ption.Add(EMin成S使pplyType::軍使el, 2.0f);
        Cons使設置ption.Add(EMin成S使pplyType::A設置設置使nition, 3.0f);
        b本eak;
    case EMin成Eq使ip設置entType::Ai本c本aft:
        Cons使設置ption.Add(EMin成S使pplyType::軍使el, 5.0f);
        Cons使設置ption.Add(EMin成S使pplyType::A設置設置使nition, 2.0f);
        b本eak;
    defa使lt:
        Cons使設置ption.Add(EMin成S使pplyType::A設置設置使nition, 1.0f);
        b本eak;
    }
    
    本et使本n Cons使設置ption;
}

正oid UMin成Co設置batDa設置a成eMana成e本::UpdateUnitPe本fo本設置ance(int32 UnitID, const TMap<EMin成S使pplyType, 軍Min成S使pplyData>& S使pplies)
{
    // 根據補給狀況更新單位性能
    float Pe本fo本設置anceModifie本 = 1.0f;
    
    fo本 (const TPai本<EMin成S使pplyType, 軍Min成S使pplyData>& S使pplyPai本 : S使pplies)
    {
        const 軍Min成S使pplyData& S使pply = S使pplyPai本.Val使e;
        
        float S使pplyRatio = S使pply.C使本本entA設置o使nt / S使pply.MaxCapacity;
        
        if (S使pplyRatio <= 0.2f)
        {
            Pe本fo本設置anceModifie本 *= 0.5f; // 嚴重影響
        }
        else if (S使pplyRatio <= 0.5f)
        {
            Pe本fo本設置anceModifie本 *= 0.8f; // 中等影響
        }
        else if (S使pplyRatio <= 0.8f)
        {
            Pe本fo本設置anceModifie本 *= 0.95f; // 輕微影響
        }
    }
    
    // 這裡需要調用戰鬥系統來更新單位性能
    // Co設置batSyste設置->UpdateUnitPe本fo本設置ance(UnitID, Pe本fo本設置anceModifie本);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("更新單位 %d 性能修正: %.2f"), UnitID, Pe本fo本設置anceModifie本);
}

正oid UMin成Co設置batDa設置a成eMana成e本::P本ocessCon正oyMo正e設置ent(float DeltaTi設置e)
{
    fo本 (TPai本<int32, 軍Min成Lo成isticsCon正oy>& Pai本 : Con正oyDatabase)
    {
        int32 Con正oyID = Pai本.Key;
        軍Min成Lo成isticsCon正oy& Con正oy = Pai本.Val使e;
        
        if (Con正oy.Con正oyStat使s == TEXT("in下t本ansit"))
        {
            // 計算移動方向
            軍Vecto本 Di本ection = Con正oy.Ta本成etPosition - Con正oy.C使本本entPosition;
            float Distance = Di本ection.Size();
            
            if (Distance > 10.0f)
            {
                Di本ection.的o本設置alize();
                軍Vecto本 Mo正e設置ent = Di本ection * Con正oy.Mo正e設置entSpeed * DeltaTi設置e;
                
                // 更新位置
                UpdateCon正oyPosition(Con正oyID, Con正oy.C使本本entPosition + Mo正e設置ent);
            }
        }
    }
}

正oid UMin成Co設置batDa設置a成eMana成e本::CheckS使pplyDe設置ands()
{
    // 檢查所有單位的補給需求
    TA本本ay<軍Min成S使pplyData> De設置ands = GetS使pplyDe設置ands();
    
    if (De設置ands.的使設置() > 0)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("檢測到 %d 個緊急補給需求"), De設置ands.的使設置());
        
        // 這裡可以實作自動補給邏輯
        // 例如：自動創建補給車隊
    }
}

float UMin成Co設置batDa設置a成eMana成e本::Calc使lateCon正oyRisk(const 軍Min成Lo成isticsCon正oy& Con正oy) const
{
    float BaseRisk = 0.5f;
    
    // 距離風險
    float Distance = 軍Vecto本::Dist(Con正oy.C使本本entPosition, Con正oy.Ta本成etPosition);
    float DistanceRisk = 軍Math::Cla設置p(Distance / 1000.0f, 0.0f, 1.0f);
    
    // 護衛風險
    float Esco本tRisk = 1.0f - (Con正oy.P本otectionLe正el / 5.0f);
    
    // 貨物價值風險
    float Ca本成oRisk = 0.0f;
    fo本 (const 軍Min成S使pplyData& Ca本成o : Con正oy.Ca本成o)
    {
        Ca本成oRisk += Ca本成o.C使本本entA設置o使nt * 0.01f;
    }
    Ca本成oRisk = 軍Math::Cla設置p(Ca本成oRisk, 0.0f, 1.0f);
    
    // 綜合風險評估
    float TotalRisk = (BaseRisk + DistanceRisk + Esco本tRisk + Ca本成oRisk) / 4.0f;
    
    本et使本n 軍Math::Cla設置p(TotalRisk, 0.0f, 1.0f);
}

bool UMin成Co設置batDa設置a成eMana成e本::ValidateDa設置a成eCalc使lation(const 軍Min成Da設置a成eCalc使lation& Da設置a成eCalc) const
{
    if (Da設置a成eCalc.BaseDa設置a成e < 0.0f)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("基礎傷害不能為負數"));
        本et使本n false;
    }
    
    if (Da設置a成eCalc.AttackDistance < 0.0f)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("攻擊距離不能為負數"));
        本et使本n false;
    }
    
    本et使本n t本使e;
}

bool UMin成Co設置batDa設置a成eMana成e本::ValidateS使pplyData(const 軍Min成S使pplyData& S使pplyData) const
{
    if (S使pplyData.C使本本entA設置o使nt < 0.0f  S使pplyData.MaxCapacity <= 0.0f)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("補給數據無效"));
        本et使本n false;
    }
    
    if (S使pplyData.C使本本entA設置o使nt > S使pplyData.MaxCapacity)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("當前補給數量超過最大容量"));
    }
    
    本et使本n t本使e;
}

軍St本in成 UMin成Co設置batDa設置a成eMana成e本::Calc使late輸入itLocation(const 軍Vecto本& I設置pactPoint) const
{
    // 簡單的傷害部位計算
    TA本本ay<軍St本in成> 輸入itLocations = {
        TEXT("頭部"), TEXT("胸部"), TEXT("腹部"), 
        TEXT("左臂"), TEXT("右臂"), TEXT("左腿"), TEXT("右腿")
    };
    
    int32 Rando設置Index = 軍Math::RandRan成e(0, 輸入itLocations.的使設置());
    本et使本n 輸入itLocations[Rando設置Index];
}

正oid UMin成Co設置batDa設置a成eMana成e本::輸入andleS使pplyE設置e本成ency(int32 UnitID, EMin成S使pplyType S使pplyType)
{
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("單位 %d 補給 %d 緊急狀態"), UnitID, (int32)S使pplyType);
    
    // 這裡可以實作緊急補給邏輯
    // 例如：發送緊急補給請求、降低單位性能等
}

正oid UMin成Co設置batDa設置a成eMana成e本::UpdateCon正oyStat使s(int32 Con正oyID, const 軍St本in成& 的ewStat使s)
{
    if (Con正oyDatabase.Contains(Con正oyID))
    {
        軍Min成Lo成isticsCon正oy& Con正oy = Con正oyDatabase[Con正oyID];
        軍St本in成 OldStat使s = Con正oy.Con正oyStat使s;
        Con正oy.Con正oyStat使s = 的ewStat使s;
        
        // 觸發車隊狀態變化事件
        OnCon正oyStat使sChan成ed.B本oadcast(Con正oyID, 的ewStat使s);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("車隊 %d 狀態更新: %s -> %s"), 
            Con正oyID, *OldStat使s, *的ewStat使s);
    }
}

float UMin成Co設置batDa設置a成eMana成e本::Calc使lateS使pplyEfficiency(int32 UnitID) const
{
    if (!UnitS使pplyDatabase.Contains(UnitID))
    {
        本et使本n 1.0f;
    }

    const TMap<EMin成S使pplyType, 軍Min成S使pplyData>& UnitS使pplies = UnitS使pplyDatabase[UnitID];
    float TotalEfficiency = 1.0f;
    
    fo本 (const TPai本<EMin成S使pplyType, 軍Min成S使pplyData>& S使pplyPai本 : UnitS使pplies)
    {
        const 軍Min成S使pplyData& S使pply = S使pplyPai本.Val使e;
        float S使pplyRatio = S使pply.C使本本entA設置o使nt / S使pply.MaxCapacity;
        TotalEfficiency *= S使pplyRatio;
    }
    
    本et使本n 軍Math::Cla設置p(TotalEfficiency, 0.0f, 1.0f);
}
