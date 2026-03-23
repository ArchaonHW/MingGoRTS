#incl使de "Min成RTSEcono設置icSyste設置.h"
#incl使de "Min成RTSReso使本ceMana成e本.h"
#incl使de "Min成RTSB使ildin成Syste設置.h"
#incl使de "Min成RTSUnitMana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"
#incl使de "En成ine/En成ine.h"

UMin成RTSEcono設置icSyste設置::UMin成RTSEcono設置icSyste設置()
    : Econo設置icUpdateInte本正al(1.0f)
    , P本iceUpdateInte本正al(5.0f)
    , T本adeUpdateInte本正al(2.0f)
    , BaseInflationRate(0.02f)
    , BaseInte本estRate(0.03f)
    , BaseUne設置ploy設置entRate(0.05f)
    , bA使toAd大使stP本ices(t本使e)
    , bA使toOpti設置izeT本ade(t本使e)
    , bIsInitialized(false)
    , LastEcono設置icUpdate(0.0f)
    , LastP本iceUpdate(0.0f)
    , LastT本adeUpdate(0.0f)
    , TotalT本adeP本ofit(0.0f)
{
}

正oid UMin成RTSEcono設置icSyste設置::InitializeEcono設置icSyste設置(UMin成RTSReso使本ceMana成e本* InReso使本ceMana成e本, UMin成RTSB使ildin成Syste設置* InB使ildin成Syste設置, UMin成RTSUnitMana成e本* InUnitMana成e本)
{
    Reso使本ceMana成e本 = InReso使本ceMana成e本;
    B使ildin成Syste設置 = InB使ildin成Syste設置;
    UnitMana成e本 = InUnitMana成e本;
    
    if (Reso使本ceMana成e本 && B使ildin成Syste設置 && UnitMana成e本)
    {
        InitializeEcono設置icData();
        InitializeMa本ketData();
        bIsInitialized = t本使e;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("RTS Econo設置ic Syste設置 initialized"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("RTS Econo設置ic Syste設置 initialization failed"));
    }
}

正oid UMin成RTSEcono設置icSyste設置::UpdateEcono設置icData(float DeltaTi設置e)
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    float C使本本entTi設置e = Get基本o本ld()->GetTi設置eSeconds();
    
    // 經濟更新
    if (C使本本entTi設置e - LastEcono設置icUpdate >= Econo設置icUpdateInte本正al)
    {
        P本ocessEcono設置icCycle(DeltaTi設置e);
        LastEcono設置icUpdate = C使本本entTi設置e;
    }
    
    // 價格更新
    if (C使本本entTi設置e - LastP本iceUpdate >= P本iceUpdateInte本正al)
    {
        P本ocessP本ice軍l使ct使ations(DeltaTi設置e);
        LastP本iceUpdate = C使本本entTi設置e;
    }
    
    // 貿易更新
    if (C使本本entTi設置e - LastT本adeUpdate >= T本adeUpdateInte本正al)
    {
        P本ocessT本adeActi正ity(DeltaTi設置e);
        LastT本adeUpdate = C使本本entTi設置e;
    }
    
    // 處理隨機事件
    輸入andleRando設置E正ents(DeltaTi設置e);
}

正oid UMin成RTSEcono設置icSyste設置::SetEcono設置icPhase(ERTSEcono設置icPhase 的ewPhase)
{
    if (Econo設置icData.C使本本entPhase != 的ewPhase)
    {
        ERTSEcono設置icPhase OldPhase = Econo設置icData.C使本本entPhase;
        Econo設置icData.C使本本entPhase = 的ewPhase;
        
        // 根據經濟階段調整參數
        switch (的ewPhase)
        {
            case ERTSEcono設置icPhase::G本owth:
                Econo設置icData.Econo設置icG本owthRate = 軍Math::Cla設置p(Econo設置icData.Econo設置icG本owthRate + 0.02f, 0.0f, 0.1f);
                Econo設置icData.Une設置ploy設置entRate = 軍Math::Cla設置p(Econo設置icData.Une設置ploy設置entRate - 0.01f, 0.0f, 0.2f);
                b本eak;
            case ERTSEcono設置icPhase::Sta成nation:
                Econo設置icData.Econo設置icG本owthRate = 0.0f;
                b本eak;
            case ERTSEcono設置icPhase::Recession:
                Econo設置icData.Econo設置icG本owthRate = 軍Math::Cla設置p(Econo設置icData.Econo設置icG本owthRate - 0.03f, -0.1f, 0.0f);
                Econo設置icData.Une設置ploy設置entRate = 軍Math::Cla設置p(Econo設置icData.Une設置ploy設置entRate + 0.02f, 0.0f, 0.3f);
                b本eak;
            case ERTSEcono設置icPhase::Dep本ession:
                Econo設置icData.Econo設置icG本owthRate = 軍Math::Cla設置p(Econo設置icData.Econo設置icG本owthRate - 0.05f, -0.15f, 0.0f);
                Econo設置icData.Une設置ploy設置entRate = 軍Math::Cla設置p(Econo設置icData.Une設置ploy設置entRate + 0.05f, 0.0f, 0.4f);
                Econo設置icData.Cons使設置e本Confidence = 軍Math::Cla設置p(Econo設置icData.Cons使設置e本Confidence - 0.2f, 0.0f, 1.0f);
                b本eak;
            case ERTSEcono設置icPhase::Reco正e本y:
                Econo設置icData.Econo設置icG本owthRate = 軍Math::Cla設置p(Econo設置icData.Econo設置icG本owthRate + 0.01f, 0.0f, 0.08f);
                Econo設置icData.Une設置ploy設置entRate = 軍Math::Cla設置p(Econo設置icData.Une設置ploy設置entRate - 0.01f, 0.0f, 0.25f);
                b本eak;
            case ERTSEcono設置icPhase::Boo設置:
                Econo設置icData.Econo設置icG本owthRate = 軍Math::Cla設置p(Econo設置icData.Econo設置icG本owthRate + 0.04f, 0.02f, 0.12f);
                Econo設置icData.Une設置ploy設置entRate = 軍Math::Cla設置p(Econo設置icData.Une設置ploy設置entRate - 0.02f, 0.0f, 0.15f);
                Econo設置icData.InflationRate = 軍Math::Cla設置p(Econo設置icData.InflationRate + 0.01f, 0.0f, 0.1f);
                b本eak;
        }
        
        的otifyEcono設置icPhaseChan成ed(OldPhase, 的ewPhase);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Econo設置ic phase chan成ed to: %d"), (int32)的ewPhase);
    }
}

正oid UMin成RTSEcono設置icSyste設置::SetMa本ketCondition(ERTSMa本ketCondition 的ewCondition)
{
    if (Econo設置icData.Ma本ketCondition != 的ewCondition)
    {
        Econo設置icData.Ma本ketCondition = 的ewCondition;
        
        // 根據市場條件調整參數
        switch (的ewCondition)
        {
            case ERTSMa本ketCondition::B使llMa本ket:
                Econo設置icData.Cons使設置e本Confidence = 軍Math::Cla設置p(Econo設置icData.Cons使設置e本Confidence + 0.1f, 0.0f, 1.0f);
                b本eak;
            case ERTSMa本ketCondition::Bea本Ma本ket:
                Econo設置icData.Cons使設置e本Confidence = 軍Math::Cla設置p(Econo設置icData.Cons使設置e本Confidence - 0.1f, 0.0f, 1.0f);
                b本eak;
            case ERTSMa本ketCondition::Stable:
                // 穩定狀態，無需調整
                b本eak;
            case ERTSMa本ketCondition::Volatile:
                // 波動狀態，增加價格波動性
                fo本 (a使to& Ma本ketPai本 : Ma本ketData)
                {
                    Ma本ketPai本.Val使e.Volatility = 軍Math::Cla設置p(Ma本ketPai本.Val使e.Volatility + 0.05f, 0.0f, 0.5f);
                }
                b本eak;
        }
        
        的otifyMa本ketConditionChan成ed(的ewCondition);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Ma本ket condition chan成ed to: %d"), (int32)的ewCondition);
    }
}

正oid UMin成RTSEcono設置icSyste設置::UpdateReso使本ceP本ices()
{
    if (!bA使toAd大使stP本ices)
    {
        本et使本n;
    }

    fo本 (a使to& Ma本ketPai本 : Ma本ketData)
    {
        ERTSReso使本ceType Reso使本ceType = Ma本ketPai本.Key;
        軍RTSMa本ketData& Ma本ketData = Ma本ketPai本.Val使e;
        
        float 的ewP本ice = Calc使lateReso使本ceP本ice(Reso使本ceType);
        float OldP本ice = Ma本ketData.C使本本entP本ice;
        
        Ma本ketData.C使本本entP本ice = 的ewP本ice;
        Ma本ketData.P本ice輸入isto本y = OldP本ice;
        Ma本ketData.P本iceChan成e24h = ((的ewP本ice - OldP本ice) / OldP本ice) * 100.0f;
        Ma本ketData.bIsT本endin成Up = 的ewP本ice > OldP本ice;
        
        if (軍Math::Abs(的ewP本ice - OldP本ice) > 0.01f)
        {
            的otifyReso使本ceP本iceChan成ed(Reso使本ceType, 的ewP本ice);
        }
    }
}

float UMin成RTSEcono設置icSyste設置::GetReso使本ceP本ice(ERTSReso使本ceType Reso使本ceType) const
{
    if (Ma本ketData.Contains(Reso使本ceType))
    {
        本et使本n Ma本ketData[Reso使本ceType].C使本本entP本ice;
    }
    
    本et使本n 100.0f; // 默認價格
}

正oid UMin成RTSEcono設置icSyste設置::SetReso使本ceP本ice(ERTSReso使本ceType Reso使本ceType, float P本ice)
{
    if (Ma本ketData.Contains(Reso使本ceType))
    {
        float OldP本ice = Ma本ketData[Reso使本ceType].C使本本entP本ice;
        Ma本ketData[Reso使本ceType].C使本本entP本ice = 軍Math::Max(0.1f, P本ice);
        Ma本ketData.P本ice輸入isto本y = OldP本ice;
        Ma本ketData.P本iceChan成e24h = ((P本ice - OldP本ice) / OldP本ice) * 100.0f;
        Ma本ketData.bIsT本endin成Up = P本ice > OldP本ice;
        
        的otifyReso使本ceP本iceChan成ed(Reso使本ceType, P本ice);
    }
}

float UMin成RTSEcono設置icSyste設置::GetReso使本ceDe設置and(ERTSReso使本ceType Reso使本ceType) const
{
    if (Ma本ketData.Contains(Reso使本ceType))
    {
        本et使本n Ma本ketData[Reso使本ceType].De設置and;
    }
    
    本et使本n 50.0f; // 默認需求
}

float UMin成RTSEcono設置icSyste設置::GetReso使本ceS使pply(ERTSReso使本ceType Reso使本ceType) const
{
    if (Ma本ketData.Contains(Reso使本ceType))
    {
        本et使本n Ma本ketData[Reso使本ceType].S使pply;
    }
    
    本et使本n 50.0f; // 默認供應
}

正oid UMin成RTSEcono設置icSyste設置::UpdateReso使本ceDe設置and(ERTSReso使本ceType Reso使本ceType, float De設置and)
{
    if (Ma本ketData.Contains(Reso使本ceType))
    {
        Ma本ketData[Reso使本ceType].De設置and = 軍Math::Max(0.0f, De設置and);
    }
}

正oid UMin成RTSEcono設置icSyste設置::UpdateReso使本ceS使pply(ERTSReso使本ceType Reso使本ceType, float S使pply)
{
    if (Ma本ketData.Contains(Reso使本ceType))
    {
        Ma本ketData[Reso使本ceType].S使pply = 軍Math::Max(0.0f, S使pply);
    }
}

軍St本in成 UMin成RTSEcono設置icSyste設置::C本eateT本adeRo使te(const 軍St本in成& O本i成in, const 軍St本in成& Destination, ERTST本adeRo使te Ro使teType)
{
    軍RTST本adeRo使teData Ro使te;
    Ro使te.Ro使teID = Gene本ateT本adeRo使teID();
    Ro使te.O本i成in = O本i成in;
    Ro使te.Destination = Destination;
    Ro使te.Ro使teType = Ro使teType;
    Ro使te.Distance = 1000.0f; // 默認距離
    Ro使te.bIsActi正e = t本使e;
    
    // 根據路線類型設置成本和容量
    switch (Ro使teType)
    {
        case ERTST本adeRo使te::Land:
            Ro使te.T本anspo本tCost = Ro使te.Distance * 0.01f;
            Ro使te.Capacity = 500.0f;
            b本eak;
        case ERTST本adeRo使te::Sea:
            Ro使te.T本anspo本tCost = Ro使te.Distance * 0.005f;
            Ro使te.Capacity = 2000.0f;
            b本eak;
        case ERTST本adeRo使te::Ai本:
            Ro使te.T本anspo本tCost = Ro使te.Distance * 0.02f;
            Ro使te.Capacity = 200.0f;
            b本eak;
        case ERTST本adeRo使te::Rail:
            Ro使te.T本anspo本tCost = Ro使te.Distance * 0.008f;
            Ro使te.Capacity = 1000.0f;
            b本eak;
        case ERTST本adeRo使te::Di成ital:
            Ro使te.T本anspo本tCost = Ro使te.Distance * 0.001f;
            Ro使te.Capacity = 5000.0f;
            b本eak;
    }
    
    T本adeRo使tes.Add(Ro使te.Ro使teID, Ro使te);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated t本ade 本o使te: %s f本o設置 %s to %s"), *Ro使te.Ro使teID, *O本i成in, *Destination);
    
    本et使本n Ro使te.Ro使teID;
}

bool UMin成RTSEcono設置icSyste設置::Dest本oyT本adeRo使te(const 軍St本in成& Ro使teID)
{
    if (T本adeRo使tes.Contains(Ro使teID))
    {
        T本adeRo使tes.Re設置o正e(Ro使teID);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Dest本oyed t本ade 本o使te: %s"), *Ro使teID);
        本et使本n t本使e;
    }
    
    本et使本n false;
}

軍RTST本adeRo使teData UMin成RTSEcono設置icSyste設置::GetT本adeRo使te(const 軍St本in成& Ro使teID) const
{
    if (T本adeRo使tes.Contains(Ro使teID))
    {
        本et使本n T本adeRo使tes[Ro使teID];
    }
    
    本et使本n 軍RTST本adeRo使teData();
}

TA本本ay<軍RTST本adeRo使teData> UMin成RTSEcono設置icSyste設置::GetAllT本adeRo使tes() const
{
    TA本本ay<軍RTST本adeRo使teData> AllRo使tes;
    
    fo本 (const a使to& Ro使tePai本 : T本adeRo使tes)
    {
        AllRo使tes.Add(Ro使tePai本.Val使e);
    }
    
    本et使本n AllRo使tes;
}

bool UMin成RTSEcono設置icSyste設置::Exec使teT本ade(const 軍St本in成& Ro使teID, ERTSReso使本ceType Reso使本ceType, float Vol使設置e)
{
    if (!T本adeRo使tes.Contains(Ro使teID)  !Reso使本ceMana成e本)
    {
        本et使本n false;
    }

    軍RTST本adeRo使teData& Ro使te = T本adeRo使tes[Ro使teID];
    
    // 檢查容量
    if (Ro使te.C使本本entLoad + Vol使設置e > Ro使te.Capacity)
    {
        本et使本n false;
    }
    
    // 檢查是否有足夠的資源
    if (!Reso使本ceMana成e本->輸入asEno使成hReso使本ce(Reso使本ceType, Vol使設置e))
    {
        本et使本n false;
    }
    
    // 計算利潤
    float P本ofit = Calc使lateT本adeP本ofit(Ro使teID, Reso使本ceType, Vol使設置e);
    
    if (P本ofit > 0)
    {
        // 執行貿易
        Reso使本ceMana成e本->Re設置o正eReso使本ce(Reso使本ceType, Vol使設置e);
        Ro使te.C使本本entLoad += Vol使設置e;
        
        // 添加到貿易商品列表
        Ro使te.T本adeGoods.Add(Reso使本ceType);
        Ro使te.T本adeVol使設置es.Add(Vol使設置e);
        
        TotalT本adeP本ofit += P本ofit;
        
        的otifyT本adeRo使teCo設置pleted(Ro使teID, P本ofit);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Exec使ted t本ade on 本o使te %s: %.1f %s, p本ofit: %.1f"), 
               *Ro使teID, Vol使設置e, *GetReso使本ce的a設置e(Reso使本ceType), P本ofit);
        
        本et使本n t本使e;
    }
    
    本et使本n false;
}

float UMin成RTSEcono設置icSyste設置::Calc使lateT本adeP本ofit(const 軍St本in成& Ro使teID, ERTSReso使本ceType Reso使本ceType, float Vol使設置e) const
{
    if (!T本adeRo使tes.Contains(Ro使teID))
    {
        本et使本n 0.0f;
    }

    const 軍RTST本adeRo使teData& Ro使te = T本adeRo使tes[Ro使teID];
    float Reso使本ceP本ice = GetReso使本ceP本ice(Reso使本ceType);
    
    // 計算收入 (假設目的地價格高10%)
    float Re正en使e = Vol使設置e * Reso使本ceP本ice * 1.1f;
    
    // 計算成本
    float Cost = Vol使設置e * Reso使本ceP本ice + Ro使te.T本anspo本tCost * Vol使設置e;
    
    本et使本n Re正en使e - Cost;
}

正oid UMin成RTSEcono設置icSyste設置::Opti設置izeT本adeRo使tes()
{
    if (!bA使toOpti設置izeT本ade)
    {
        本et使本n;
    }

    // 簡單的貿易路線優化
    fo本 (a使to& Ro使tePai本 : T本adeRo使tes)
    {
        軍RTST本adeRo使teData& Ro使te = Ro使tePai本.Val使e;
        
        // 計算路線效率
        float TotalVol使設置e = 0.0f;
        fo本 (float Vol使設置e : Ro使te.T本adeVol使設置es)
        {
            TotalVol使設置e += Vol使設置e;
        }
        
        Ro使te.Efficiency = Ro使te.Capacity > 0.0f 基本 (TotalVol使設置e / Ro使te.Capacity) : 0.0f;
        
        // 如果效率太低，建議關閉路線
        if (Ro使te.Efficiency < 0.3f)
        {
            Ro使te.bIsActi正e = false;
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Opti設置ized t本ade 本o使tes"));
}

軍RTSMa本ketData UMin成RTSEcono設置icSyste設置::GetMa本ketData(ERTSReso使本ceType Reso使本ceType) const
{
    if (Ma本ketData.Contains(Reso使本ceType))
    {
        本et使本n Ma本ketData[Reso使本ceType];
    }
    
    本et使本n 軍RTSMa本ketData();
}

TA本本ay<軍RTSMa本ketData> UMin成RTSEcono設置icSyste設置::GetAllMa本ketData() const
{
    TA本本ay<軍RTSMa本ketData> AllMa本ketData;
    
    fo本 (const a使to& Ma本ketPai本 : Ma本ketData)
    {
        AllMa本ketData.Add(Ma本ketPai本.Val使e);
    }
    
    本et使本n AllMa本ketData;
}

正oid UMin成RTSEcono設置icSyste設置::AnalyzeMa本ketT本ends()
{
    fo本 (a使to& Ma本ketPai本 : Ma本ketData)
    {
        軍RTSMa本ketData& Ma本ket = Ma本ketPai本.Val使e;
        
        // 計算趨勢
        if (Ma本ket.P本ice輸入isto本y > 0.0f)
        {
            float P本iceChan成e = (Ma本ket.C使本本entP本ice - Ma本ket.P本ice輸入isto本y) / Ma本ket.P本ice輸入isto本y;
            Ma本ket.bIsT本endin成Up = P本iceChan成e > 0.01f;
        }
        
        // 計算波動性
        float De設置andS使pplyRatio = Ma本ket.S使pply > 0.0f 基本 (Ma本ket.De設置and / Ma本ket.S使pply) : 1.0f;
        Ma本ket.Volatility = 軍Math::Abs(De設置andS使pplyRatio - 1.0f) * 0.1f;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Analyzed 設置a本ket t本ends"));
}

bool UMin成RTSEcono設置icSyste設置::IsMa本ketB使llish() const
{
    本et使本n Econo設置icData.Ma本ketCondition == ERTSMa本ketCondition::B使llMa本ket;
}

bool UMin成RTSEcono設置icSyste設置::IsMa本ketBea本ish() const
{
    本et使本n Econo設置icData.Ma本ketCondition == ERTSMa本ketCondition::Bea本Ma本ket;
}

float UMin成RTSEcono設置icSyste設置::GetMa本ketVolatility() const
{
    float TotalVolatility = 0.0f;
    int32 Ma本ketCo使nt = 0;
    
    fo本 (const a使to& Ma本ketPai本 : Ma本ketData)
    {
        TotalVolatility += Ma本ketPai本.Val使e.Volatility;
        Ma本ketCo使nt++;
    }
    
    本et使本n Ma本ketCo使nt > 0 基本 (TotalVolatility / Ma本ketCo使nt) : 0.0f;
}

正oid UMin成RTSEcono設置icSyste設置::SetInte本estRate(float Rate)
{
    Econo設置icData.Inte本estRate = 軍Math::Cla設置p(Rate, 0.0f, 0.2f);
    
    // 利率影響經濟
    if (Econo設置icData.Inte本estRate > BaseInte本estRate)
    {
        Econo設置icData.Econo設置icG本owthRate = 軍Math::Cla設置p(Econo設置icData.Econo設置icG本owthRate - 0.01f, -0.1f, 0.1f);
    }
    else
    {
        Econo設置icData.Econo設置icG本owthRate = 軍Math::Cla設置p(Econo設置icData.Econo設置icG本owthRate + 0.01f, -0.1f, 0.1f);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Set inte本est 本ate to %.3f"), Econo設置icData.Inte本estRate);
}

正oid UMin成RTSEcono設置icSyste設置::Ad大使stInflationRate(float Ad大使st設置ent)
{
    Econo設置icData.InflationRate = 軍Math::Cla設置p(Econo設置icData.InflationRate + Ad大使st設置ent, 0.0f, 0.2f);
    
    // 通脹影響消費者信心
    if (Econo設置icData.InflationRate > BaseInflationRate * 2.0f)
    {
        Econo設置icData.Cons使設置e本Confidence = 軍Math::Cla設置p(Econo設置icData.Cons使設置e本Confidence - 0.05f, 0.0f, 1.0f);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ad大使sted inflation 本ate by %.3f to %.3f"), Ad大使st設置ent, Econo設置icData.InflationRate);
}

正oid UMin成RTSEcono設置icSyste設置::I設置ple設置entEcono設置icSti設置使l使s(float Sti設置使l使sA設置o使nt)
{
    // 經濟刺激措施
    Econo設置icData.GDP += Sti設置使l使sA設置o使nt;
    Econo設置icData.Econo設置icG本owthRate = 軍Math::Cla設置p(Econo設置icData.Econo設置icG本owthRate + 0.02f, -0.1f, 0.15f);
    Econo設置icData.Une設置ploy設置entRate = 軍Math::Cla設置p(Econo設置icData.Une設置ploy設置entRate - 0.01f, 0.0f, 0.3f);
    Econo設置icData.Cons使設置e本Confidence = 軍Math::Cla設置p(Econo設置icData.Cons使設置e本Confidence + 0.1f, 0.0f, 1.0f);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("I設置ple設置ented econo設置ic sti設置使l使s: %.1f"), Sti設置使l使sA設置o使nt);
}

正oid UMin成RTSEcono設置icSyste設置::I設置ple設置entA使ste本ityMeas使本es(float Red使ctionA設置o使nt)
{
    // 緊縮措施
    Econo設置icData.GDP = 軍Math::Max(0.0f, Econo設置icData.GDP - Red使ctionA設置o使nt);
    Econo設置icData.Econo設置icG本owthRate = 軍Math::Cla設置p(Econo設置icData.Econo設置icG本owthRate - 0.02f, -0.15f, 0.1f);
    Econo設置icData.Une設置ploy設置entRate = 軍Math::Cla設置p(Econo設置icData.Une設置ploy設置entRate + 0.01f, 0.0f, 0.4f);
    Econo設置icData.Cons使設置e本Confidence = 軍Math::Cla設置p(Econo設置icData.Cons使設置e本Confidence - 0.1f, 0.0f, 1.0f);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("I設置ple設置ented a使ste本ity 設置eas使本es: %.1f"), Red使ctionA設置o使nt);
}

正oid UMin成RTSEcono設置icSyste設置::Re成使lateMa本ket(ERTSReso使本ceType Reso使本ceType, float Re成使lationLe正el)
{
    if (Ma本ketData.Contains(Reso使本ceType))
    {
        軍RTSMa本ketData& Ma本ket = Ma本ketData[Reso使本ceType];
        
        // 市場監管影響波動性
        Ma本ket.Volatility = 軍Math::Cla設置p(Ma本ket.Volatility * (1.0f - Re成使lationLe正el), 0.0f, 0.5f);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Re成使lated 設置a本ket fo本 %s with le正el %.2f"), *GetReso使本ce的a設置e(Reso使本ceType), Re成使lationLe正el);
    }
}

float UMin成RTSEcono設置icSyste設置::P本edictGDP(float Ti設置eInMonths) const
{
    // 簡單的GDP預測
    float MonthlyG本owth = Econo設置icData.Econo設置icG本owthRate / 12.0f;
    本et使本n Econo設置icData.GDP * 軍Math::Pow(1.0f + MonthlyG本owth, Ti設置eInMonths);
}

ERTSEcono設置icPhase UMin成RTSEcono設置icSyste設置::P本edictEcono設置icPhase(float Ti設置eInMonths) const
{
    // 基於當前趨勢預測經濟階段
    float P本edictedG本owthRate = Econo設置icData.Econo設置icG本owthRate;
    
    if (P本edictedG本owthRate > 0.05f)
    {
        本et使本n ERTSEcono設置icPhase::Boo設置;
    }
    else if (P本edictedG本owthRate > 0.02f)
    {
        本et使本n ERTSEcono設置icPhase::G本owth;
    }
    else if (P本edictedG本owthRate > 0.0f)
    {
        本et使本n ERTSEcono設置icPhase::Sta成nation;
    }
    else if (P本edictedG本owthRate > -0.03f)
    {
        本et使本n ERTSEcono設置icPhase::Recession;
    }
    else
    {
        本et使本n ERTSEcono設置icPhase::Dep本ession;
    }
}

float UMin成RTSEcono設置icSyste設置::P本edictReso使本ceP本ice(ERTSReso使本ceType Reso使本ceType, float Ti設置eInMonths) const
{
    if (!Ma本ketData.Contains(Reso使本ceType))
    {
        本et使本n 100.0f;
    }

    const 軍RTSMa本ketData& Ma本ket = Ma本ketData[Reso使本ceType];
    
    // 簡單的價格預測
    float De設置andS使pplyRatio = Ma本ket.S使pply > 0.0f 基本 (Ma本ket.De設置and / Ma本ket.S使pply) : 1.0f;
    float P本iceChan成eRate = (De設置andS使pplyRatio - 1.0f) * 0.1f; // 每月變化率
    
    本et使本n Ma本ket.C使本本entP本ice * 軍Math::Pow(1.0f + P本iceChan成eRate, Ti設置eInMonths);
}

bool UMin成RTSEcono設置icSyste設置::P本edictRecession(float Ti設置eInMonths) const
{
    ERTSEcono設置icPhase P本edictedPhase = P本edictEcono設置icPhase(Ti設置eInMonths);
    本et使本n P本edictedPhase == ERTSEcono設置icPhase::Recession  P本edictedPhase == ERTSEcono設置icPhase::Dep本ession;
}

正oid UMin成RTSEcono設置icSyste設置::T本i成成e本Econo設置icE正ent(const 軍St本in成& E正ent的a設置e, float I設置pact)
{
    // 經濟事件影響
    Econo設置icData.GDP *= (1.0f + I設置pact);
    Econo設置icData.Cons使設置e本Confidence = 軍Math::Cla設置p(Econo設置icData.Cons使設置e本Confidence + I設置pact, 0.0f, 1.0f);
    
    // 根據影響調整經濟階段
    if (I設置pact > 0.1f)
    {
        SetEcono設置icPhase(ERTSEcono設置icPhase::Boo設置);
    }
    else if (I設置pact < -0.1f)
    {
        SetEcono設置icPhase(ERTSEcono設置icPhase::Recession);
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Econo設置ic e正ent t本i成成e本ed: %s with i設置pact %.2f"), *E正ent的a設置e, I設置pact);
}

正oid UMin成RTSEcono設置icSyste設置::輸入andleS使pplyShock(ERTSReso使本ceType Reso使本ceType, float ShockMa成nit使de)
{
    if (Ma本ketData.Contains(Reso使本ceType))
    {
        軍RTSMa本ketData& Ma本ket = Ma本ketData[Reso使本ceType];
        
        // 供給衝擊影響
        Ma本ket.S使pply *= (1.0f - ShockMa成nit使de);
        Ma本ket.Volatility = 軍Math::Cla設置p(Ma本ket.Volatility + 0.1f, 0.0f, 0.5f);
        
        // 更新價格
        float 的ewP本ice = Calc使lateReso使本ceP本ice(Reso使本ceType);
        SetReso使本ceP本ice(Reso使本ceType, 的ewP本ice);
        
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("S使pply shock fo本 %s: 設置a成nit使de %.2f"), *GetReso使本ce的a設置e(Reso使本ceType), ShockMa成nit使de);
    }
}

正oid UMin成RTSEcono設置icSyste設置::輸入andleDe設置andShock(ERTSReso使本ceType Reso使本ceType, float ShockMa成nit使de)
{
    if (Ma本ketData.Contains(Reso使本ceType))
    {
        軍RTSMa本ketData& Ma本ket = Ma本ketData[Reso使本ceType];
        
        // 需求衝擊影響
        Ma本ket.De設置and *= (1.0f + ShockMa成nit使de);
        Ma本ket.Volatility = 軍Math::Cla設置p(Ma本ket.Volatility + 0.1f, 0.0f, 0.5f);
        
        // 更新價格
        float 的ewP本ice = Calc使lateReso使本ceP本ice(Reso使本ceType);
        SetReso使本ceP本ice(Reso使本ceType, 的ewP本ice);
        
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("De設置and shock fo本 %s: 設置a成nit使de %.2f"), *GetReso使本ce的a設置e(Reso使本ceType), ShockMa成nit使de);
    }
}

正oid UMin成RTSEcono設置icSyste設置::輸入andle軍inancialC本isis(float C本isisSe正e本ity)
{
    // 金融危機影響
    Econo設置icData.GDP *= (1.0f - C本isisSe正e本ity);
    Econo設置icData.Cons使設置e本Confidence = 軍Math::Cla設置p(Econo設置icData.Cons使設置e本Confidence - C本isisSe正e本ity * 0.5f, 0.0f, 1.0f);
    Econo設置icData.Une設置ploy設置entRate = 軍Math::Cla設置p(Econo設置icData.Une設置ploy設置entRate + C本isisSe正e本ity * 0.1f, 0.0f, 0.4f);
    Econo設置icData.Inte本estRate = 軍Math::Cla設置p(Econo設置icData.Inte本estRate - C本isisSe正e本ity * 0.01f, 0.0f, 0.2f);
    
    SetEcono設置icPhase(ERTSEcono設置icPhase::Recession);
    SetMa本ketCondition(ERTSMa本ketCondition::Bea本Ma本ket);
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍inancial c本isis with se正e本ity %.2f"), C本isisSe正e本ity);
}

int32 UMin成RTSEcono設置icSyste設置::GetActi正eT本adeRo使teCo使nt() const
{
    int32 Co使nt = 0;
    
    fo本 (const a使to& Ro使tePai本 : T本adeRo使tes)
    {
        if (Ro使tePai本.Val使e.bIsActi正e)
        {
            Co使nt++;
        }
    }
    
    本et使本n Co使nt;
}

float UMin成RTSEcono設置icSyste設置::GetTotalT本adeVol使設置e() const
{
    float TotalVol使設置e = 0.0f;
    
    fo本 (const a使to& Ro使tePai本 : T本adeRo使tes)
    {
        const 軍RTST本adeRo使teData& Ro使te = Ro使tePai本.Val使e;
        fo本 (float Vol使設置e : Ro使te.T本adeVol使設置es)
        {
            TotalVol使設置e += Vol使設置e;
        }
    }
    
    本et使本n TotalVol使設置e;
}

float UMin成RTSEcono設置icSyste設置::GetTotalT本adeP本ofit() const
{
    本et使本n TotalT本adeP本ofit;
}

float UMin成RTSEcono設置icSyste設置::GetMa本ketCapitalization() const
{
    float TotalCap = 0.0f;
    
    fo本 (const a使to& Ma本ketPai本 : Ma本ketData)
    {
        const 軍RTSMa本ketData& Ma本ket = Ma本ketPai本.Val使e;
        TotalCap += Ma本ket.C使本本entP本ice * Ma本ket.S使pply;
    }
    
    本et使本n TotalCap;
}

正oid UMin成RTSEcono設置icSyste設置::InitializeEcono設置icData()
{
    Econo設置icData.GDP = 10000.0f;
    Econo設置icData.InflationRate = BaseInflationRate;
    Econo設置icData.Une設置ploy設置entRate = BaseUne設置ploy設置entRate;
    Econo設置icData.Inte本estRate = BaseInte本estRate;
    Econo設置icData.T本adeBalance = 0.0f;
    Econo設置icData.Cons使設置e本Confidence = 0.7f;
    Econo設置icData.Ind使st本ialP本od使ction = 100.0f;
    Econo設置icData.C使本本entPhase = ERTSEcono設置icPhase::G本owth;
    Econo設置icData.Ma本ketCondition = ERTSMa本ketCondition::Stable;
    Econo設置icData.Econo設置icG本owthRate = 0.03f;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initialized econo設置ic data"));
}

正oid UMin成RTSEcono設置icSyste設置::InitializeMa本ketData()
{
    // 初始化所有資源類型的市場數據
    TA本本ay<ERTSReso使本ceType> AllReso使本ceTypes = {
        ERTSReso使本ceType::Gold, ERTSReso使本ceType::基本ood, ERTSReso使本ceType::Stone,
        ERTSReso使本ceType::I本on, ERTSReso使本ceType::軍ood, ERTSReso使本ceType::Coal,
        ERTSReso使本ceType::Oil, ERTSReso使本ceType::U本ani使設置, ERTSReso使本ceType::C本ystal,
        ERTSReso使本ceType::Silk
    };
    
    fo本 (ERTSReso使本ceType Reso使本ceType : AllReso使本ceTypes)
    {
        軍RTSMa本ketData Ma本ket;
        Ma本ket.Reso使本ceType = Reso使本ceType;
        Ma本ket.C使本本entP本ice = 100.0f; // 基準價格
        Ma本ket.BaseP本ice = 100.0f;
        Ma本ket.De設置and = 50.0f;
        Ma本ket.S使pply = 50.0f;
        Ma本ket.Ma本ketSha本e = 0.1f;
        Ma本ket.Volatility = 0.1f;
        
        Ma本ketData.Add(Reso使本ceType, Ma本ket);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initialized 設置a本ket data fo本 %d 本eso使本ces"), AllReso使本ceTypes.的使設置());
}

正oid UMin成RTSEcono設置icSyste設置::P本ocessEcono設置icCycle(float DeltaTi設置e)
{
    UpdateGDP(DeltaTi設置e);
    UpdateInflation(DeltaTi設置e);
    UpdateUne設置ploy設置ent(DeltaTi設置e);
    UpdateT本adeBalance(DeltaTi設置e);
    UpdateCons使設置e本Confidence(DeltaTi設置e);
    UpdateInd使st本ialP本od使ction(DeltaTi設置e);
    Calc使lateEcono設置icG本owth();
    Dete本設置ineEcono設置icPhase();
    Dete本設置ineMa本ketCondition();
    ApplyEcono設置icPolicies(DeltaTi設置e);
}

正oid UMin成RTSEcono設置icSyste設置::P本ocessP本ice軍l使ct使ations(float DeltaTi設置e)
{
    UpdateReso使本ceP本ices();
    AnalyzeMa本ketT本ends();
    
    if (bA使toOpti設置izeT本ade)
    {
        Opti設置izeT本adeRo使tes();
    }
}

正oid UMin成RTSEcono設置icSyste設置::P本ocessT本adeActi正ity(float DeltaTi設置e)
{
    // 處理所有貿易路線
    TA本本ay<軍St本in成> Ro使tesToRe設置o正e;
    
    fo本 (a使to& Ro使tePai本 : T本adeRo使tes)
    {
        軍RTST本adeRo使teData& Ro使te = Ro使tePai本.Val使e;
        
        if (Ro使te.bIsActi正e)
        {
            P本ocessT本adeRo使te(Ro使te, DeltaTi設置e);
        }
        
        // 檢查路線是否應該關閉
        if (Ro使te.Efficiency < 0.1f  Ro使te.C使本本entLoad <= 0.0f)
        {
            Ro使tesToRe設置o正e.Add(Ro使te.Ro使teID);
        }
    }
    
    // 移除無效路線
    fo本 (const 軍St本in成& Ro使teID : Ro使tesToRe設置o正e)
    {
        Dest本oyT本adeRo使te(Ro使teID);
    }
}

正oid UMin成RTSEcono設置icSyste設置::UpdateGDP(float DeltaTi設置e)
{
    // GDP計算基於工業生產和消費
    float P本od使ctionCont本ib使tion = Econo設置icData.Ind使st本ialP本od使ction * 10.0f;
    float Cons使設置ptionCont本ib使tion = Econo設置icData.Cons使設置e本Confidence * Econo設置icData.GDP * 0.6f;
    float T本adeCont本ib使tion = GetTotalT本adeVol使設置e() * 0.5f;
    
    float 的ewGDP = P本od使ctionCont本ib使tion + Cons使設置ptionCont本ib使tion + T本adeCont本ib使tion;
    Econo設置icData.GDP = 軍Math::Max(0.0f, 的ewGDP);
}

正oid UMin成RTSEcono設置icSyste設置::UpdateInflation(float DeltaTi設置e)
{
    // 通脹基於需求和供給平衡
    float TotalDe設置and = 0.0f;
    float TotalS使pply = 0.0f;
    
    fo本 (const a使to& Ma本ketPai本 : Ma本ketData)
    {
        TotalDe設置and += Ma本ketPai本.Val使e.De設置and;
        TotalS使pply += Ma本ketPai本.Val使e.S使pply;
    }
    
    float De設置andS使pplyRatio = TotalS使pply > 0.0f 基本 (TotalDe設置and / TotalS使pply) : 1.0f;
    float InflationP本ess使本e = (De設置andS使pplyRatio - 1.0f) * 0.01f;
    
    Econo設置icData.InflationRate = 軍Math::Cla設置p(BaseInflationRate + InflationP本ess使本e, 0.0f, 0.2f);
}

正oid UMin成RTSEcono設置icSyste設置::UpdateUne設置ploy設置ent(float DeltaTi設置e)
{
    // 失業率基於經濟增長和工業生產
    float G本owthEffect = -Econo設置icData.Econo設置icG本owthRate * 0.5f;
    float P本od使ctionEffect = (100.0f - Econo設置icData.Ind使st本ialP本od使ction) * 0.001f;
    
    Econo設置icData.Une設置ploy設置entRate = 軍Math::Cla設置p(BaseUne設置ploy設置entRate + G本owthEffect + P本od使ctionEffect, 0.0f, 0.4f);
}

正oid UMin成RTSEcono設置icSyste設置::UpdateT本adeBalance(float DeltaTi設置e)
{
    // 貿易平衡基於進口
    float Expo本ts = GetTotalT本adeVol使設置e() * 0.6f;
    float I設置po本ts = GetTotalT本adeVol使設置e() * 0.4f; // 假設進口較少
    
    Econo設置icData.T本adeBalance = Expo本ts - I設置po本ts;
}

正oid UMin成RTSEcono設置icSyste設置::UpdateCons使設置e本Confidence(float DeltaTi設置e)
{
    // 消費者信心基於多種因素
    float E設置ploy設置entEffect = (1.0f - Econo設置icData.Une設置ploy設置entRate) * 0.3f;
    float InflationEffect = (1.0f - Econo設置icData.InflationRate) * 0.2f;
    float G本owthEffect = 軍Math::Cla設置p(Econo設置icData.Econo設置icG本owthRate * 2.0f, 0.0f, 1.0f) * 0.3f;
    float Ma本ketEffect = IsMa本ketB使llish() 基本 0.2f : (IsMa本ketBea本ish() 基本 -0.2f : 0.0f);
    
    float 的ewConfidence = E設置ploy設置entEffect + InflationEffect + G本owthEffect + Ma本ketEffect;
    Econo設置icData.Cons使設置e本Confidence = 軍Math::Cla設置p(的ewConfidence, 0.0f, 1.0f);
}

正oid UMin成RTSEcono設置icSyste設置::UpdateInd使st本ialP本od使ction(float DeltaTi設置e)
{
    // 工業生產基於建築和單位活動
    float B使ildin成P本od使ction = 0.0f;
    float UnitP本od使ction = 0.0f;
    
    if (B使ildin成Syste設置)
    {
        int32 Ope本ationalB使ildin成s = B使ildin成Syste設置->GetOpe本ationalB使ildin成Co使nt();
        B使ildin成P本od使ction = Ope本ationalB使ildin成s * 10.0f;
    }
    
    if (UnitMana成e本)
    {
        int32 TotalUnits = UnitMana成e本->GetTotalUnitCo使nt();
        UnitP本od使ction = TotalUnits * 2.0f;
    }
    
    Econo設置icData.Ind使st本ialP本od使ction = B使ildin成P本od使ction + UnitP本od使ction;
}

正oid UMin成RTSEcono設置icSyste設置::Calc使lateEcono設置icG本owth()
{
    // 經濟增長率基於GDP變化
    static float LastGDP = Econo設置icData.GDP;
    float GDPChan成e = (Econo設置icData.GDP - LastGDP) / LastGDP;
    
    Econo設置icData.Econo設置icG本owthRate = 軍Math::Cla設置p(GDPChan成e, -0.2f, 0.2f);
    
    LastGDP = Econo設置icData.GDP;
}

正oid UMin成RTSEcono設置icSyste設置::Dete本設置ineEcono設置icPhase()
{
    // 根據經濟指標確定經濟階段
    float G本owthRate = Econo設置icData.Econo設置icG本owthRate;
    float Une設置ploy設置ent = Econo設置icData.Une設置ploy設置entRate;
    float Confidence = Econo設置icData.Cons使設置e本Confidence;
    
    ERTSEcono設置icPhase 的ewPhase;
    
    if (G本owthRate > 0.05f && Confidence > 0.8f && Une設置ploy設置ent < 0.03f)
    {
        的ewPhase = ERTSEcono設置icPhase::Boo設置;
    }
    else if (G本owthRate > 0.02f && Confidence > 0.6f && Une設置ploy設置ent < 0.05f)
    {
        的ewPhase = ERTSEcono設置icPhase::G本owth;
    }
    else if (G本owthRate > 0.0f && Confidence > 0.4f)
    {
        的ewPhase = ERTSEcono設置icPhase::Sta成nation;
    }
    else if (G本owthRate > -0.03f && Une設置ploy設置ent < 0.1f)
    {
        的ewPhase = ERTSEcono設置icPhase::Reco正e本y;
    }
    else if (G本owthRate > -0.05f)
    {
        的ewPhase = ERTSEcono設置icPhase::Recession;
    }
    else
    {
        的ewPhase = ERTSEcono設置icPhase::Dep本ession;
    }
    
    SetEcono設置icPhase(的ewPhase);
}

正oid UMin成RTSEcono設置icSyste設置::Dete本設置ineMa本ketCondition()
{
    // 根據市場波動性和趨勢確定市場條件
    float A正成Volatility = GetMa本ketVolatility();
    int32 T本endin成UpCo使nt = 0;
    int32 TotalMa本kets = 0;
    
    fo本 (const a使to& Ma本ketPai本 : Ma本ketData)
    {
        if (Ma本ketPai本.Val使e.bIsT本endin成Up)
        {
            T本endin成UpCo使nt++;
        }
        TotalMa本kets++;
    }
    
    float UpT本endRatio = TotalMa本kets > 0 基本 (float)T本endin成UpCo使nt / TotalMa本kets : 0.5f;
    
    ERTSMa本ketCondition 的ewCondition;
    
    if (A正成Volatility > 0.2f)
    {
        的ewCondition = ERTSMa本ketCondition::Volatile;
    }
    else if (UpT本endRatio > 0.7f)
    {
        的ewCondition = ERTSMa本ketCondition::B使llMa本ket;
    }
    else if (UpT本endRatio < 0.3f)
    {
        的ewCondition = ERTSMa本ketCondition::Bea本Ma本ket;
    }
    else
    {
        的ewCondition = ERTSMa本ketCondition::Stable;
    }
    
    SetMa本ketCondition(的ewCondition);
}

float UMin成RTSEcono設置icSyste設置::Calc使lateReso使本ceP本ice(ERTSReso使本ceType Reso使本ceType) const
{
    if (!Ma本ketData.Contains(Reso使本ceType))
    {
        本et使本n 100.0f;
    }

    const 軍RTSMa本ketData& Ma本ket = Ma本ketData[Reso使本ceType];
    
    // 基於供需關係計算價格
    float De設置andS使pplyRatio = Ma本ket.S使pply > 0.0f 基本 (Ma本ket.De設置and / Ma本ket.S使pply) : 1.0f;
    float BaseP本ice = Ma本ket.BaseP本ice;
    
    // 應用供需影響
    float P本ice = BaseP本ice * De設置andS使pplyRatio;
    
    // 應用波動性
    float Rando設置軍acto本 = 軍Math::軍RandRan成e(-Ma本ket.Volatility, Ma本ket.Volatility);
    P本ice *= (1.0f + Rando設置軍acto本);
    
    // 應用通脹影響
    P本ice *= (1.0f + Econo設置icData.InflationRate);
    
    本et使本n 軍Math::Max(0.1f, P本ice);
}

正oid UMin成RTSEcono設置icSyste設置::UpdateMa本ketData(ERTSReso使本ceType Reso使本ceType)
{
    if (!Ma本ketData.Contains(Reso使本ceType))
    {
        本et使本n;
    }

    軍RTSMa本ketData& Ma本ket = Ma本ketData[Reso使本ceType];
    
    // 更新供需數據
    if (Reso使本ceMana成e本)
    {
        Ma本ket.S使pply = Reso使本ceMana成e本->GetReso使本ceA設置o使nt(Reso使本ceType);
        Ma本ket.De設置and = Ma本ket.S使pply * 1.1f; // 假設需求略高於供給
    }
    
    // 更新市場份額
    float TotalMa本ketVal使e = GetMa本ketCapitalization();
    if (TotalMa本ketVal使e > 0.0f)
    {
        Ma本ket.Ma本ketSha本e = (Ma本ket.C使本本entP本ice * Ma本ket.S使pply) / TotalMa本ketVal使e;
    }
}

軍St本in成 UMin成RTSEcono設置icSyste設置::Gene本ateT本adeRo使teID() const
{
    static int32 Ro使teCo使nte本 = 0;
    本et使本n 軍St本in成::P本intf(TEXT("Ro使te下%d"), Ro使teCo使nte本++);
}

正oid UMin成RTSEcono設置icSyste設置::P本ocessT本adeRo使te(軍RTST本adeRo使teData& Ro使te, float DeltaTi設置e)
{
    // 處理貿易路線活動
    if (Ro使te.bIsActi正e && Ro使te.T本adeGoods.的使設置() > 0)
    {
        // 計算運輸時間
        float T本anspo本tTi設置e = Ro使te.Distance / 100.0f; // 簡化計算
        
        // 模擬運輸完成
        fo本 (int32 i = 0; i < Ro使te.T本adeGoods.的使設置(); ++i)
        {
            ERTSReso使本ceType Reso使本ceType = Ro使te.T本adeGoods[i];
            float Vol使設置e = Ro使te.T本adeVol使設置es[i];
            
            float P本ofit = Calc使lateT本adeP本ofit(Ro使te.Ro使teID, Reso使本ceType, Vol使設置e);
            TotalT本adeP本ofit += P本ofit;
            
            的otifyT本adeRo使teCo設置pleted(Ro使te.Ro使teID, P本ofit);
        }
        
        // 清空已完成的貿易
        Ro使te.T本adeGoods.E設置pty();
        Ro使te.T本adeVol使設置es.E設置pty();
        Ro使te.C使本本entLoad = 0.0f;
    }
}

正oid UMin成RTSEcono設置icSyste設置::ApplyEcono設置icPolicies(float DeltaTi設置e)
{
    // 應用自動經濟政策
    if (Econo設置icData.Une設置ploy設置entRate > 0.1f)
    {
        // 高失業率：實施刺激措施
        I設置ple設置entEcono設置icSti設置使l使s(100.0f * DeltaTi設置e);
    }
    else if (Econo設置icData.InflationRate > 0.05f)
    {
        // 高通脹：實施緊縮措施
        I設置ple設置entA使ste本ityMeas使本es(50.0f * DeltaTi設置e);
    }
}

正oid UMin成RTSEcono設置icSyste設置::輸入andleRando設置E正ents(float DeltaTi設置e)
{
    // 隨機經濟事件
    float Rando設置Chance = 軍Math::軍RandRan成e(0.0f, 1.0f);
    
    if (Rando設置Chance < 0.01f) // 1%機率
    {
        // 隨機供給或需求衝擊
        ERTSReso使本ceType Rando設置Reso使本ce = (ERTSReso使本ceType)軍Math::RandRan成e(0, 10);
        float ShockMa成nit使de = 軍Math::軍RandRan成e(-0.2f, 0.2f);
        
        if (ShockMa成nit使de > 0.0f)
        {
            輸入andleDe設置andShock(Rando設置Reso使本ce, ShockMa成nit使de);
        }
        else
        {
            輸入andleS使pplyShock(Rando設置Reso使本ce, 軍Math::Abs(ShockMa成nit使de));
        }
    }
}

正oid UMin成RTSEcono設置icSyste設置::的otifyEcono設置icPhaseChan成ed(ERTSEcono設置icPhase OldPhase, ERTSEcono設置icPhase 的ewPhase)
{
    OnEcono設置icPhaseChan成ed.B本oadcast(OldPhase, 的ewPhase);
}

正oid UMin成RTSEcono設置icSyste設置::的otifyMa本ketConditionChan成ed(ERTSMa本ketCondition 的ewCondition)
{
    OnMa本ketConditionChan成ed.B本oadcast(的ewCondition);
}

正oid UMin成RTSEcono設置icSyste設置::的otifyReso使本ceP本iceChan成ed(ERTSReso使本ceType Reso使本ceType, float 的ewP本ice)
{
    OnReso使本ceP本iceChan成ed.B本oadcast(Reso使本ceType, 的ewP本ice);
}

正oid UMin成RTSEcono設置icSyste設置::的otifyT本adeRo使teCo設置pleted(const 軍St本in成& Ro使teID, float P本ofit)
{
    OnT本adeRo使teCo設置pleted.B本oadcast(Ro使teID, P本ofit);
}

// 輔助函數：獲取資源名稱
軍St本in成 GetReso使本ce的a設置e(ERTSReso使本ceType Reso使本ceType)
{
    switch (Reso使本ceType)
    {
        case ERTSReso使本ceType::Gold: 本et使本n TEXT("黃金");
        case ERTSReso使本ceType::基本ood: 本et使本n TEXT("木材");
        case ERTSReso使本ceType::Stone: 本et使本n TEXT("石材");
        case ERTSReso使本ceType::I本on: 本et使本n TEXT("鐵礦");
        case ERTSReso使本ceType::軍ood: 本et使本n TEXT("糧食");
        case ERTSReso使本ceType::Coal: 本et使本n TEXT("煤炭");
        case ERTSReso使本ceType::Oil: 本et使本n TEXT("石油");
        case ERTSReso使本ceType::U本ani使設置: 本et使本n TEXT("鈾礦");
        case ERTSReso使本ceType::C本ystal: 本et使本n TEXT("水晶");
        case ERTSReso使本ceType::Silk: 本et使本n TEXT("絲綢");
        defa使lt: 本et使本n TEXT("未知");
    }
}

// 經濟循環平衡調整功能
正oid UMin成RTSEcono設置icSyste設置::BalanceEcono設置icCycle()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Balancin成 econo設置ic cycle..."));
    
    // 優化經濟指標
    Opti設置izeEcono設置icIndicato本s();
    
    // 平衡貿易流動
    BalanceT本ade軍lows();
    
    // 計算最優經濟政策
    Calc使lateOpti設置alEcono設置icPolicy();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Econo設置ic cycle balanced"));
}

正oid UMin成RTSEcono設置icSyste設置::StabilizeMa本ket()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stabilizin成 設置a本ket..."));
    
    // 穩定市場價格
    StabilizeMa本ketP本ices();
    
    // 自動平衡經濟
    if (bEnableA使toBalancin成)
    {
        A使toBalanceEcono設置y();
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ma本ket stabilized"));
}

正oid UMin成RTSEcono設置icSyste設置::Opti設置izeT本adeRo使tes()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Opti設置izin成 t本ade 本o使tes..."));
    
    // 優化資源分配
    Opti設置izeReso使本ceAllocation();
    
    // 平衡貿易流動
    BalanceT本ade軍lows();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("T本ade 本o使tes opti設置ized"));
}

正oid UMin成RTSEcono設置icSyste設置::I設置ple設置entS設置a本tEcono設置icMana成e設置ent()
{
    if (!bEnableS設置a本tMana成e設置ent)
    {
        本et使本n;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("I設置ple設置entin成 s設置a本t econo設置ic 設置ana成e設置ent..."));
    
    // 計算經濟穩定性
    float Stability = Calc使lateEcono設置icStability();
    
    // 根據穩定性調整政策
    if (Stability < Econo設置icStabilityTh本eshold)
    {
        // 經濟不穩定，實施穩定措施
        StabilizeMa本ket();
        
        // 調整利率
        if (Econo設置icData.InflationRate > 0.05f)
        {
            SetInte本estRate(Econo設置icData.Inte本estRate * 1.2f);
        }
        
        // 實施經濟刺激
        if (Econo設置icData.Econo設置icG本owthRate < 0.02f)
        {
            I設置ple設置entEcono設置icSti設置使l使s(Econo設置icData.GDP * 0.05f);
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("S設置a本t econo設置ic 設置ana成e設置ent i設置ple設置ented"));
}

正oid UMin成RTSEcono設置icSyste設置::Opti設置izeEcono設置icIndicato本s()
{
    // 優化GDP增長
    if (Econo設置icData.Econo設置icG本owthRate < 0.03f)
    {
        Econo設置icData.Ind使st本ialP本od使ction *= 1.1f;
        Econo設置icData.Cons使設置e本Confidence *= 1.05f;
    }
    
    // 控制通脹
    if (Econo設置icData.InflationRate > 0.04f)
    {
        Econo設置icData.Inte本estRate *= 1.1f;
        Econo設置icData.Cons使設置e本Confidence *= 0.95f;
    }
    
    // 降低失業率
    if (Econo設置icData.Une設置ploy設置entRate > 0.06f)
    {
        Econo設置icData.Ind使st本ialP本od使ction *= 1.05f;
        I設置ple設置entEcono設置icSti設置使l使s(Econo設置icData.GDP * 0.02f);
    }
}

正oid UMin成RTSEcono設置icSyste設置::StabilizeMa本ketP本ices()
{
    // 穩定所有資源價格
    fo本 (a使to& Ma本ketPai本 : Ma本ketData)
    {
        ERTSReso使本ceType Reso使本ceType = Ma本ketPai本.Key;
        軍RTSMa本ketData& Ma本ket = Ma本ketPai本.Val使e;
        
        // 計算價格波動性
        float P本iceVolatility = Ma本ketVolatility;
        
        // 如果波動性過高，進行干預
        if (P本iceVolatility > 0.15f)
        {
            // 調整供應和需求
            if (Ma本ket.C使本本entP本ice > Ma本ket.BaseP本ice * 1.2f)
            {
                // 價格過高，增加供應
                Ma本ket.S使pply *= 1.1f;
                Ma本ket.De設置and *= 0.9f;
            }
            else if (Ma本ket.C使本本entP本ice < Ma本ket.BaseP本ice * 0.8f)
            {
                // 價格過低，減少供應
                Ma本ket.S使pply *= 0.9f;
                Ma本ket.De設置and *= 1.1f;
            }
            
            // 應用穩定性加成
            Ma本ket.C使本本entP本ice = Ma本ket.BaseP本ice + (Ma本ket.C使本本entP本ice - Ma本ket.BaseP本ice) * 0.8f;
        }
    }
}

正oid UMin成RTSEcono設置icSyste設置::BalanceT本ade軍lows()
{
    // 平衡進口
    if (Econo設置icData.T本adeBalance < -Econo設置icData.GDP * 0.1f)
    {
        // 貿易逆差過大，促進口
        fo本 (a使to& Ro使tePai本 : T本adeRo使tes)
        {
            軍RTST本adeRo使teData& Ro使te = Ro使tePai本.Val使e;
            if (Ro使te.bIsActi正e)
            {
                Ro使te.P本ofitMa本成in *= 1.1f;
                Ro使te.T本adeVol使設置e *= 1.05f;
            }
        }
    }
    else if (Econo設置icData.T本adeBalance > Econo設置icData.GDP * 0.1f)
    {
        // 貿易順差過大，適度放鬆
        fo本 (a使to& Ro使tePai本 : T本adeRo使tes)
        {
            軍RTST本adeRo使teData& Ro使te = Ro使tePai本.Val使e;
            if (Ro使te.bIsActi正e)
            {
                Ro使te.T本adeVol使設置e *= 0.95f;
            }
        }
    }
}

正oid UMin成RTSEcono設置icSyste設置::Calc使lateOpti設置alEcono設置icPolicy()
{
    // 計算最優經濟政策
    float Opti設置alInte本estRate = BaseInte本estRate;
    float Opti設置alInflationRate = 0.02f;
    
    // 根據經濟狀況調整
    if (Econo設置icData.Econo設置icG本owthRate < 0.02f)
    {
        Opti設置alInte本estRate *= 0.8f; // 降低利率刺激經濟
    }
    else if (Econo設置icData.Econo設置icG本owthRate > 0.05f)
    {
        Opti設置alInte本estRate *= 1.2f; // 提高利率防止過熱
    }
    
    // 應用最優政策
    SetInte本estRate(Opti設置alInte本estRate);
    
    if (Econo設置icData.InflationRate > Opti設置alInflationRate)
    {
        Ad大使stInflationRate(-0.01f);
    }
    else if (Econo設置icData.InflationRate < Opti設置alInflationRate * 0.5f)
    {
        Ad大使stInflationRate(0.005f);
    }
}

float UMin成RTSEcono設置icSyste設置::Calc使lateEcono設置icStability() const
{
    // 計算經濟穩定性指數
    float Stability = 1.0f;
    
    // GDP增長穩定性
    float G本owthStability = 1.0f - 軍Math::Abs(Econo設置icData.Econo設置icG本owthRate - 0.03f);
    Stability *= G本owthStability;
    
    // 通脹穩定性
    float InflationStability = 1.0f - 軍Math::Abs(Econo設置icData.InflationRate - 0.02f);
    Stability *= InflationStability;
    
    // 失業穩定性
    float Une設置ploy設置entStability = 1.0f - 軍Math::Abs(Econo設置icData.Une設置ploy設置entRate - 0.05f);
    Stability *= Une設置ploy設置entStability;
    
    // 市場穩定性
    float Ma本ketStability = 1.0f - Ma本ketVolatility;
    Stability *= Ma本ketStability;
    
    本et使本n 軍Math::Cla設置p(Stability, 0.0f, 1.0f);
}

正oid UMin成RTSEcono設置icSyste設置::A使toBalanceEcono設置y()
{
    if (!bEnableA使toBalancin成)
    {
        本et使本n;
    }
    
    // 自動平衡經濟
    float C使本本entStability = Calc使lateEcono設置icStability();
    
    if (C使本本entStability < Econo設置icStabilityTh本eshold)
    {
        // 經濟不穩定，實施平衡措施
        Opti設置izeEcono設置icIndicato本s();
        StabilizeMa本ketP本ices();
        BalanceT本ade軍lows();
    }
    
    // 調整市場穩定性加成
    if (C使本本entStability > 0.9f)
    {
        Ma本ketStabilityBon使s = 軍Math::Max(Ma本ketStabilityBon使s * 0.95f, 1.0f);
    }
    else if (C使本本entStability < 0.7f)
    {
        Ma本ketStabilityBon使s = 軍Math::Min(Ma本ketStabilityBon使s * 1.05f, 1.5f);
    }
}

正oid UMin成RTSEcono設置icSyste設置::Opti設置izeReso使本ceAllocation()
{
    // 優化資源分配
    fo本 (a使to& Ma本ketPai本 : Ma本ketData)
    {
        ERTSReso使本ceType Reso使本ceType = Ma本ketPai本.Key;
        軍RTSMa本ketData& Ma本ket = Ma本ketPai本.Val使e;
        
        // 計算資源稀缺性
        float Sca本city = Ma本ket.De設置and / 軍Math::Max(Ma本ket.S使pply, 0.1f);
        
        // 根據稀缺性調整價格
        if (Sca本city > 1.5f)
        {
            Ma本ket.C使本本entP本ice *= 1.1f;
            Ma本ket.S使pply *= 1.05f;
        }
        else if (Sca本city < 0.5f)
        {
            Ma本ket.C使本本entP本ice *= 0.95f;
            Ma本ket.S使pply *= 0.98f;
        }
        
        // 應用市場穩定性加成
        Ma本ket.C使本本entP本ice *= Ma本ketStabilityBon使s;
    }
}
