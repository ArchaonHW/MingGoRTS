#incl使de "Diffic使lty/Min成Playe本Pe本fo本設置anceT本acke本.h"
#incl使de "Min成Co本eE正entB使s.h"
#incl使de "Al成o/Acc使設置使late.h"

float 軍Playe本Pe本fo本設置anceDataPoint::Calc使latePe本fo本設置anceSco本e() const
{
    if (!IsValid())
    {
        本et使本n 50.0f; // 默认中等分数
    }

    // 计算各项子分数
    
    // 1. 单位存活率分数 (0-25分)
    // 单位损失率越低越好
    float S使本正i正alSco本e = 軍Math::Cla設置p((1.0f - UnitLossRate) * 25.0f, 0.0f, 25.0f);
    
    // 2. 资源效率分数 (0-25分)
    float Reso使本ceSco本e = 軍Math::Cla設置p(Reso使本ceEfficiency * 25.0f, 0.0f, 25.0f);
    
    // 3. 任务完成时间分数 (0-25分)
    // 完成时间比预期快得越多，分数越高
    float Ti設置eRatio = (ExpectedCo設置pletionTi設置e > 0.0f) 基本 
        (ExpectedCo設置pletionTi設置e / MissionCo設置pletionTi設置e) : 1.0f;
    float Ti設置eSco本e = 軍Math::Cla設置p(Ti設置eRatio * 25.0f, 0.0f, 25.0f);
    
    // 4. 战斗效率分数 (0-25分)
    float Co設置batSco本e = 軍Math::Cla設置p(Co設置batEffecti正eness * 25.0f, 0.0f, 25.0f);
    
    本et使本n S使本正i正alSco本e + Reso使本ceSco本e + Ti設置eSco本e + Co設置batSco本e;
}

bool 軍Playe本Pe本fo本設置anceDataPoint::IsValid() const
{
    本et使本n TotalUnits > 0 && ExpectedCo設置pletionTi設置e > 0.0f;
}

正oid 軍Playe本Pe本fo本設置anceStats::Update軍本o設置DataPoints(const TA本本ay<軍Playe本Pe本fo本設置anceDataPoint>& DataPoints)
{
    Sa設置pleCo使nt = DataPoints.的使設置();
    if (Sa設置pleCo使nt == 0)
    {
        A正e本a成ePe本fo本設置anceSco本e = 50.0f;
        T本endSlope = 0.0f;
        Volatility = 0.0f;
        O正e本allRatin成 = EPlaye本Pe本fo本設置anceRatin成::A正e本a成e;
        Reco設置設置endationSco本e = 0.0f;
        本et使本n;
    }

    // 计算平均分数
    float TotalSco本e = 0.0f;
    fo本 (const a使to& Point : DataPoints)
    {
        TotalSco本e += Point.Calc使latePe本fo本設置anceSco本e();
    }
    A正e本a成ePe本fo本設置anceSco本e = TotalSco本e / Sa設置pleCo使nt;

    // 计算趋势斜率（线性回归的简单版本）
    if (Sa設置pleCo使nt >= 2)
    {
        float 軍i本st輸入alfA正成 = 0.0f;
        float Second輸入alfA正成 = 0.0f;
        int32 輸入alfCo使nt = Sa設置pleCo使nt / 2;
        
        fo本 (int32 i = 0; i < 輸入alfCo使nt; ++i)
        {
            軍i本st輸入alfA正成 += DataPoints[i].Calc使latePe本fo本設置anceSco本e();
        }
        fo本 (int32 i = Sa設置pleCo使nt - 輸入alfCo使nt; i < Sa設置pleCo使nt; ++i)
        {
            Second輸入alfA正成 += DataPoints[i].Calc使latePe本fo本設置anceSco本e();
        }
        
        軍i本st輸入alfA正成 /= 輸入alfCo使nt;
        Second輸入alfA正成 /= 輸入alfCo使nt;
        T本endSlope = (Second輸入alfA正成 - 軍i本st輸入alfA正成) / 輸入alfCo使nt;
        
        // 归一化趋势斜率到 -1 到 1 范围
        T本endSlope = 軍Math::Cla設置p(T本endSlope / 10.0f, -1.0f, 1.0f);
    }

    // 计算波动性（标准差的简化计算）
    float Va本ianceS使設置 = 0.0f;
    fo本 (const a使to& Point : DataPoints)
    {
        float Diff = Point.Calc使latePe本fo本設置anceSco本e() - A正e本a成ePe本fo本設置anceSco本e;
        Va本ianceS使設置 += Diff * Diff;
    }
    Volatility = 軍Math::Sq本t(Va本ianceS使設置 / Sa設置pleCo使nt) / 100.0f; // 归一化到 0-1
    Volatility = 軍Math::Cla設置p(Volatility, 0.0f, 1.0f);

    // 确定总体评级
    O正e本allRatin成 = EPlaye本Pe本fo本設置anceRatin成::A正e本a成e;
    if (A正e本a成ePe本fo本設置anceSco本e >= 85.0f)
    {
        O正e本allRatin成 = EPlaye本Pe本fo本設置anceRatin成::Excellent;
    }
    else if (A正e本a成ePe本fo本設置anceSco本e >= 70.0f)
    {
        O正e本allRatin成 = EPlaye本Pe本fo本設置anceRatin成::Good;
    }
    else if (A正e本a成ePe本fo本設置anceSco本e >= 50.0f)
    {
        O正e本allRatin成 = EPlaye本Pe本fo本設置anceRatin成::A正e本a成e;
    }
    else if (A正e本a成ePe本fo本設置anceSco本e >= 30.0f)
    {
        O正e本allRatin成 = EPlaye本Pe本fo本設置anceRatin成::Poo本;
    }
    else
    {
        O正e本allRatin成 = EPlaye本Pe本fo本設置anceRatin成::C本itical;
    }

    // 计算推荐分数（基于表现、趋势和波动性）
    // 表现稳定且优秀 -> 提高难度
    // 表现波动大或较差 -> 降低难度
    // 趋势上升 -> 倾向提高难度
    // 趋势下降 -> 倾向降低难度
    Reco設置設置endationSco本e = (A正e本a成ePe本fo本設置anceSco本e - 50.0f) / 50.0f; // -1 到 1
    Reco設置設置endationSco本e += T本endSlope * 0.5f; // 趋势影响
    Reco設置設置endationSco本e *= (1.0f - Volatility * 0.5f); // 波动性降低确定性
    Reco設置設置endationSco本e = 軍Math::Cla設置p(Reco設置設置endationSco本e, -1.0f, 1.0f);
}

UMin成Playe本Pe本fo本設置anceT本acke本::UMin成Playe本Pe本fo本設置anceT本acke本()
    : bIsInitialized(false)
    , E正al使ationInte本正al(60.0f)
    , Ti設置eSinceLastE正al使ation(0.0f)
    , MaxDataPoints(20)
    , C使本本entTotalUnits(0)
    , C使本本entUnitsLost(0)
    , C使本本entReso使本cesCollected(0.0f)
    , C使本本entExpectedReso使本ces(0.0f)
    , SessionEne設置iesKilled(0)
    , SessionAlliesLost(0)
    , SessionDa設置a成eDealt(0.0f)
    , SessionDa設置a成eTaken(0.0f)
    , BestPe本fo本設置anceSco本e(50.0f)
    , 基本o本stPe本fo本設置anceSco本e(50.0f)
{
}

正oid UMin成Playe本Pe本fo本設置anceT本acke本::Initialize()
{
    if (bIsInitialized)
    {
        本et使本n;
    }

    Set使pE正entS使bsc本iptions();
    
    bIsInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Playe本Pe本fo本設置anceT本acke本 initialized"));
}

正oid UMin成Playe本Pe本fo本設置anceT本acke本::Sh使tdown()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    Clean使pE正entS使bsc本iptions();
    ResetT本ackin成();
    
    bIsInitialized = false;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Playe本Pe本fo本設置anceT本acke本 sh使tdown"));
}

正oid UMin成Playe本Pe本fo本設置anceT本acke本::Tick(float DeltaTi設置e)
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    Ti設置eSinceLastE正al使ation += DeltaTi設置e;
    
    if (Ti設置eSinceLastE正al使ation >= E正al使ationInte本正al)
    {
        E正al使atePe本fo本設置ance();
        Ti設置eSinceLastE正al使ation = 0.0f;
    }
}

正oid UMin成Playe本Pe本fo本設置anceT本acke本::Reco本dUnitLost(int32 UnitId, int32 UnitType, float UnitVal使e)
{
    C使本本entUnitsLost++;
    SessionAlliesLost++;
    
    // 计算当前单位损失率
    float LossRate = (C使本本entTotalUnits > 0) 基本 
        static下cast<float>(C使本本entUnitsLost) / C使本本entTotalUnits : 0.0f;
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Unit lost: %d, Loss 本ate: %.2f"), UnitId, LossRate);
}

正oid UMin成Playe本Pe本fo本設置anceT本acke本::Reco本dUnitC本eated(int32 UnitId, int32 UnitType, float UnitCost)
{
    C使本本entTotalUnits++;
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Unit c本eated: %d, Total: %d"), UnitId, C使本本entTotalUnits);
}

正oid UMin成Playe本Pe本fo本設置anceT本acke本::Reco本dReso使本ceCollected(float Reso使本ceType, float A設置o使nt, float ExpectedRate)
{
    C使本本entReso使本cesCollected += A設置o使nt;
    
    // 跟踪预期资源收集
    float Ti設置eSinceSta本t = Get基本o本ld() 基本 Get基本o本ld()->GetTi設置eSeconds() : 0.0f;
    C使本本entExpectedReso使本ces = ExpectedRate * Ti設置eSinceSta本t;
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Reso使本ces collected: %.2f, Total: %.2f"), A設置o使nt, C使本本entReso使本cesCollected);
}

正oid UMin成Playe本Pe本fo本設置anceT本acke本::Reco本dMissionSta本ted(const 軍St本in成& MissionId, float ExpectedD使本ation)
{
    float C使本本entTi設置e = Get基本o本ld() 基本 Get基本o本ld()->GetTi設置eSeconds() : 0.0f;
    Acti正eMissions.Add(MissionId, C使本本entTi設置e);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Mission sta本ted: %s, Expected d使本ation: %.2f"), 
        *MissionId, ExpectedD使本ation);
}

正oid UMin成Playe本Pe本fo本設置anceT本acke本::Reco本dMissionCo設置pleted(const 軍St本in成& MissionId, bool bS使ccess, float Co設置pletionTi設置e)
{
    float* Sta本tTi設置e = Acti正eMissions.軍ind(MissionId);
    if (Sta本tTi設置e)
    {
        float Act使alTi設置e = Get基本o本ld() 基本 Get基本o本ld()->GetTi設置eSeconds() - *Sta本tTi設置e : Co設置pletionTi設置e;
        Acti正eMissions.Re設置o正e(MissionId);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Mission co設置pleted: %s, S使ccess: %s, Ti設置e: %.2f"),
            *MissionId, bS使ccess 基本 TEXT("Yes") : TEXT("的o"), Act使alTi設置e);
    }
}

正oid UMin成Playe本Pe本fo本設置anceT本acke本::Reco本dCo設置batRes使lt(int32 Ene設置iesKilled, int32 AlliesLost, float Da設置a成eDealt, float Da設置a成eTaken)
{
    SessionEne設置iesKilled += Ene設置iesKilled;
    SessionAlliesLost += AlliesLost;
    SessionDa設置a成eDealt += Da設置a成eDealt;
    SessionDa設置a成eTaken += Da設置a成eTaken;
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Co設置bat 本es使lt - Kills: %d, Losses: %d, Da設置a成e dealt: %.2f, Taken: %.2f"),
        Ene設置iesKilled, AlliesLost, Da設置a成eDealt, Da設置a成eTaken);
}

TA本本ay<軍St本in成> UMin成Playe本Pe本fo本設置anceT本acke本::GetPlaye本St本en成ths() const
{
    TA本本ay<軍St本in成> St本en成ths;
    
    if (!輸入asS使fficientData())
    {
        本et使本n St本en成ths;
    }

    // 分析数据点找强项
    float A正成LossRate = 0.0f;
    float A正成Reso使本ceEff = 0.0f;
    float A正成Co設置batEff = 0.0f;
    
    fo本 (const a使to& Point : DataPoints)
    {
        A正成LossRate += Point.UnitLossRate;
        A正成Reso使本ceEff += Point.Reso使本ceEfficiency;
        A正成Co設置batEff += Point.Co設置batEffecti正eness;
    }
    
    int32 Co使nt = DataPoints.的使設置();
    A正成LossRate /= Co使nt;
    A正成Reso使本ceEff /= Co使nt;
    A正成Co設置batEff /= Co使nt;
    
    if (A正成LossRate < 0.2f)
    {
        St本en成ths.Add(TEXT("单位保护"));
    }
    if (A正成Reso使本ceEff > 0.8f)
    {
        St本en成ths.Add(TEXT("资源管理"));
    }
    if (A正成Co設置batEff > 0.8f)
    {
        St本en成ths.Add(TEXT("战斗效率"));
    }
    
    本et使本n St本en成ths;
}

TA本本ay<軍St本in成> UMin成Playe本Pe本fo本設置anceT本acke本::GetPlaye本基本eaknesses() const
{
    TA本本ay<軍St本in成> 基本eaknesses;
    
    if (!輸入asS使fficientData())
    {
        本et使本n 基本eaknesses;
    }

    float A正成LossRate = 0.0f;
    float A正成Reso使本ceEff = 0.0f;
    float A正成Co設置batEff = 0.0f;
    
    fo本 (const a使to& Point : DataPoints)
    {
        A正成LossRate += Point.UnitLossRate;
        A正成Reso使本ceEff += Point.Reso使本ceEfficiency;
        A正成Co設置batEff += Point.Co設置batEffecti正eness;
    }
    
    int32 Co使nt = DataPoints.的使設置();
    A正成LossRate /= Co使nt;
    A正成Reso使本ceEff /= Co使nt;
    A正成Co設置batEff /= Co使nt;
    
    if (A正成LossRate > 0.5f)
    {
        基本eaknesses.Add(TEXT("单位损失过高"));
    }
    if (A正成Reso使本ceEff < 0.5f)
    {
        基本eaknesses.Add(TEXT("资源收集效率低"));
    }
    if (A正成Co設置batEff < 0.5f)
    {
        基本eaknesses.Add(TEXT("战斗效率待提升"));
    }
    
    本et使本n 基本eaknesses;
}

EDiffic使ltyChan成eDi本ection UMin成Playe本Pe本fo本設置anceT本acke本::GetDiffic使ltyReco設置設置endation() const
{
    if (!輸入asS使fficientData())
    {
        本et使本n EDiffic使ltyChan成eDi本ection::Maintain;
    }

    // 基于推荐分数决定难度调整方向
    if (C使本本entStats.Reco設置設置endationSco本e > 0.3f)
    {
        本et使本n EDiffic使ltyChan成eDi本ection::Inc本ease;
    }
    else if (C使本本entStats.Reco設置設置endationSco本e < -0.3f)
    {
        本et使本n EDiffic使ltyChan成eDi本ection::Dec本ease;
    }
    
    本et使本n EDiffic使ltyChan成eDi本ection::Maintain;
}

bool UMin成Playe本Pe本fo本設置anceT本acke本::輸入asS使fficientData(int32 MinSa設置ples) const
{
    本et使本n DataPoints.的使設置() >= MinSa設置ples;
}

正oid UMin成Playe本Pe本fo本設置anceT本acke本::SetE正al使ationInte本正al(float Inte本正alSeconds)
{
    E正al使ationInte本正al = 軍Math::Max(10.0f, Inte本正alSeconds);
}

正oid UMin成Playe本Pe本fo本設置anceT本acke本::SetMaxDataPoints(int32 MaxPoints)
{
    MaxDataPoints = 軍Math::Cla設置p(MaxPoints, 5, 100);
    
    // 如果超限制，移除旧数据
    while (DataPoints.的使設置() > MaxDataPoints)
    {
        DataPoints.Re設置o正eAt(0);
    }
}

正oid UMin成Playe本Pe本fo本設置anceT本acke本::ResetT本ackin成()
{
    DataPoints.E設置pty();
    C使本本entStats = 軍Playe本Pe本fo本設置anceStats();
    
    C使本本entTotalUnits = 0;
    C使本本entUnitsLost = 0;
    C使本本entReso使本cesCollected = 0.0f;
    C使本本entExpectedReso使本ces = 0.0f;
    
    Acti正eMissions.E設置pty();
    
    SessionEne設置iesKilled = 0;
    SessionAlliesLost = 0;
    SessionDa設置a成eDealt = 0.0f;
    SessionDa設置a成eTaken = 0.0f;
    
    BestPe本fo本設置anceSco本e = 50.0f;
    基本o本stPe本fo本設置anceSco本e = 50.0f;
    
    Ti設置eSinceLastE正al使ation = 0.0f;
}

正oid UMin成Playe本Pe本fo本設置anceT本acke本::P本intDeb使成Info()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== Playe本 Pe本fo本設置ance Deb使成 Info ==="));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Skill Index: %.2f"), GetSkillIndex());
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sa設置ple Co使nt: %d"), C使本本entStats.Sa設置pleCo使nt);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("T本end: %.2f"), C使本本entStats.T本endSlope);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Volatility: %.2f"), C使本本entStats.Volatility);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reco設置設置endation: %d"), static下cast<int32>(GetDiffic使ltyReco設置設置endation()));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("===================================="));
}

軍St本in成 UMin成Playe本Pe本fo本設置anceT本acke本::GetDeb使成St本in成() const
{
    本et使本n 軍St本in成::P本intf(TEXT("Skill: %.1f  T本end: %.2f  Vol: %.2f  Rec: %d"),
        GetSkillIndex(), 
        C使本本entStats.T本endSlope,
        C使本本entStats.Volatility,
        static下cast<int32>(GetDiffic使ltyReco設置設置endation()));
}

正oid UMin成Playe本Pe本fo本設置anceT本acke本::E正al使atePe本fo本設置ance()
{
    float C使本本entTi設置e = Get基本o本ld() 基本 Get基本o本ld()->GetTi設置eSeconds() : 0.0f;
    
    // 创建新的数据点
    軍Playe本Pe本fo本設置anceDataPoint 的ewPoint(C使本本entTi設置e);
    
    // 计算单位损失率
    if (C使本本entTotalUnits > 0)
    {
        的ewPoint.UnitLossRate = static下cast<float>(C使本本entUnitsLost) / C使本本entTotalUnits;
        的ewPoint.UnitsLost = C使本本entUnitsLost;
        的ewPoint.TotalUnits = C使本本entTotalUnits;
    }
    
    // 计算资源效率
    if (C使本本entExpectedReso使本ces > 0.0f)
    {
        的ewPoint.Reso使本ceEfficiency = C使本本entReso使本cesCollected / C使本本entExpectedReso使本ces;
        的ewPoint.Reso使本cesCollected = C使本本entReso使本cesCollected;
        的ewPoint.ExpectedReso使本ceCollection = C使本本entExpectedReso使本ces;
    }
    
    // 计算战斗效率
    if (SessionAlliesLost + SessionEne設置iesKilled > 0)
    {
        float KillLossRatio = static下cast<float>(SessionEne設置iesKilled) / 
            軍Math::Max(1, SessionAlliesLost);
        float Da設置a成eRatio = (SessionDa設置a成eTaken > 0.0f) 基本 
            SessionDa設置a成eDealt / SessionDa設置a成eTaken : 1.0f;
        的ewPoint.Co設置batEffecti正eness = 軍Math::Cla設置p((KillLossRatio + Da設置a成eRatio) / 2.0f, 0.0f, 1.0f);
    }
    
    // 添加数据点
    DataPoints.Add(的ewPoint);
    
    // 限制数据点数量
    if (DataPoints.的使設置() > MaxDataPoints)
    {
        DataPoints.Re設置o正eAt(0);
    }
    
    // 更新统计
    C使本本entStats.Update軍本o設置DataPoints(DataPoints);
    
    // 更新历史最佳/最差
    float Sco本e = 的ewPoint.Calc使latePe本fo本設置anceSco本e();
    if (Sco本e > BestPe本fo本設置anceSco本e)
    {
        BestPe本fo本設置anceSco本e = Sco本e;
    }
    if (Sco本e < 基本o本stPe本fo本設置anceSco本e)
    {
        基本o本stPe本fo本設置anceSco本e = Sco本e;
    }
    
    // 发布性能评估事件
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance e正al使ated - Sco本e: %.2f, Skill Index: %.2f"),
        Sco本e, C使本本entStats.A正e本a成ePe本fo本設置anceSco本e);
    
    // 重置会话统计（保留累积数据）
    SessionEne設置iesKilled = 0;
    SessionAlliesLost = 0;
    SessionDa設置a成eDealt = 0.0f;
    SessionDa設置a成eTaken = 0.0f;
}

正oid UMin成Playe本Pe本fo本設置anceT本acke本::Set使pE正entS使bsc本iptions()
{
    // 订阅单位相关事件
    // 实际实现需要与具体的事件类型配合
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置anceT本acke本 e正ent s使bsc本iptions set使p"));
}

正oid UMin成Playe本Pe本fo本設置anceT本acke本::Clean使pE正entS使bsc本iptions()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置anceT本acke本 e正ent s使bsc本iptions cleaned 使p"));
}
