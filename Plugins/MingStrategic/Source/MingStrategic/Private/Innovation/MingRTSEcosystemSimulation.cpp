#incl使de "Inno正ation/Min成RTSEcosyste設置Si設置使lation.h"
#incl使de "Math/Un本ealMathUtility.h"

UMin成RTSEcosyste設置Si設置使lation::UMin成RTSEcosyste設置Si設置使lation()
    : C使本本entSeason(ESeasonType::Sp本in成)
    , 的extZoneId(1)
    , Si設置使lationTi設置e(0.0f)
{
}

正oid UMin成RTSEcosyste設置Si設置使lation::InitializeEcosyste設置()
{
    Ecosyste設置Zones.E設置pty();
    ZoneSpecies.E設置pty();
    C使本本entSeason = ESeasonType::Sp本in成;
    的extZoneId = 1;
    Si設置使lationTi設置e = 0.0f;
}

正oid UMin成RTSEcosyste設置Si設置使lation::Sh使tdownEcosyste設置()
{
    Ecosyste設置Zones.E設置pty();
    ZoneSpecies.E設置pty();
}

int32 UMin成RTSEcosyste設置Si設置使lation::C本eateEcosyste設置Zone(EEcosyste設置Zone ZoneType, 軍Vecto本 Cente本, float Radi使s)
{
    int32 的ewZoneId = 的extZoneId++;
    軍Ecosyste設置ZoneData 的ewZone;
    的ewZone.ZoneType = ZoneType;
    的ewZone.ZoneCente本 = Cente本;
    的ewZone.ZoneRadi使s = Radi使s;
    的ewZone.Cli設置ate.C使本本entSeason = C使本本entSeason;

    // 根據生態區類型初始化資源
    switch (ZoneType)
    {
    case EEcosyste設置Zone::軍o本est:
        的ewZone.Reso使本ces.Add(EReso使本ceType::基本ood, 軍Ecosyste設置Reso使本ce());
        的ewZone.Reso使本ces[EReso使本ceType::基本ood].C使本本entA設置o使nt = 200.0f;
        的ewZone.Reso使本ces[EReso使本ceType::基本ood].MaxCapacity = 500.0f;
        的ewZone.Ca本本yin成Capacity = 800.0f;
        b本eak;

    case EEcosyste設置Zone::G本assland:
        的ewZone.Reso使本ces.Add(EReso使本ceType::軍ood, 軍Ecosyste設置Reso使本ce());
        的ewZone.Reso使本ces[EReso使本ceType::軍ood].C使本本entA設置o使nt = 300.0f;
        的ewZone.Reso使本ces[EReso使本ceType::軍ood].MaxCapacity = 600.0f;
        的ewZone.Ca本本yin成Capacity = 1000.0f;
        b本eak;

    case EEcosyste設置Zone::Ri正e本:
    case EEcosyste設置Zone::Lake:
        的ewZone.Reso使本ces.Add(EReso使本ceType::基本ate本, 軍Ecosyste設置Reso使本ce());
        的ewZone.Reso使本ces[EReso使本ceType::基本ate本].C使本本entA設置o使nt = 1000.0f;
        的ewZone.Reso使本ces[EReso使本ceType::基本ate本].MaxCapacity = 2000.0f;
        的ewZone.Ca本本yin成Capacity = 500.0f;
        b本eak;

    case EEcosyste設置Zone::Dese本t:
        的ewZone.Reso使本ces.Add(EReso使本ceType::基本ate本, 軍Ecosyste設置Reso使本ce());
        的ewZone.Reso使本ces[EReso使本ceType::基本ate本].C使本本entA設置o使nt = 50.0f;
        的ewZone.Reso使本ces[EReso使本ceType::基本ate本].MaxCapacity = 100.0f;
        的ewZone.Reso使本ces[EReso使本ceType::基本ate本].Re成ene本ationRate = 0.2f;
        的ewZone.Ca本本yin成Capacity = 200.0f;
        b本eak;

    case EEcosyste設置Zone::Mo使ntain:
        的ewZone.Reso使本ces.Add(EReso使本ceType::Stone, 軍Ecosyste設置Reso使本ce());
        的ewZone.Reso使本ces[EReso使本ceType::Stone].C使本本entA設置o使nt = 500.0f;
        的ewZone.Reso使本ces[EReso使本ceType::Stone].MaxCapacity = 1000.0f;
        的ewZone.Ca本本yin成Capacity = 300.0f;
        b本eak;

    defa使lt:
        b本eak;
    }

    // 所有區域都有基礎生物多樣性
    軍Ecosyste設置Reso使本ce Biodi正e本sity;
    Biodi正e本sity.Reso使本ceType = EReso使本ceType::Biodi正e本sity;
    Biodi正e本sity.C使本本entA設置o使nt = 50.0f;
    Biodi正e本sity.MaxCapacity = 100.0f;
    的ewZone.Reso使本ces.Add(EReso使本ceType::Biodi正e本sity, Biodi正e本sity);

    Ecosyste設置Zones.Add(的ewZoneId, 的ewZone);
    ZoneSpecies.Add(的ewZoneId, TA本本ay<軍SpeciesPop使lation>());

    本et使本n 的ewZoneId;
}

正oid UMin成RTSEcosyste設置Si設置使lation::Re設置o正eEcosyste設置Zone(int32 ZoneId)
{
    Ecosyste設置Zones.Re設置o正e(ZoneId);
    ZoneSpecies.Re設置o正e(ZoneId);
}

正oid UMin成RTSEcosyste設置Si設置使lation::UpdateZoneReso使本ces(int32 ZoneId, EReso使本ceType Reso使本ceType, float A設置o使nt)
{
    if (軍Ecosyste設置ZoneData* Zone = Ecosyste設置Zones.軍ind(ZoneId))
    {
        if (軍Ecosyste設置Reso使本ce* Reso使本ce = Zone->Reso使本ces.軍ind(Reso使本ceType))
        {
            Reso使本ce->C使本本entA設置o使nt = 軍Math::Cla設置p(Reso使本ce->C使本本entA設置o使nt + A設置o使nt, 0.0f, Reso使本ce->MaxCapacity);
        }
        else if (A設置o使nt > 0)
        {
            軍Ecosyste設置Reso使本ce 的ewReso使本ce;
            的ewReso使本ce.Reso使本ceType = Reso使本ceType;
            的ewReso使本ce.C使本本entA設置o使nt = 軍Math::Min(A設置o使nt, 100.0f);
            的ewReso使本ce.MaxCapacity = 100.0f;
            Zone->Reso使本ces.Add(Reso使本ceType, 的ewReso使本ce);
        }
    }
}

軍Ecosyste設置ZoneData UMin成RTSEcosyste設置Si設置使lation::GetZoneData(int32 ZoneId) const
{
    if (const 軍Ecosyste設置ZoneData* Zone = Ecosyste設置Zones.軍ind(ZoneId))
    {
        本et使本n *Zone;
    }
    本et使本n 軍Ecosyste設置ZoneData();
}

TA本本ay<int32> UMin成RTSEcosyste設置Si設置使lation::GetAllZones() const
{
    TA本本ay<int32> Res使lt;
    Ecosyste設置Zones.GetKeys(Res使lt);
    本et使本n Res使lt;
}

TA本本ay<int32> UMin成RTSEcosyste設置Si設置使lation::GetZonesByType(EEcosyste設置Zone ZoneType) const
{
    TA本本ay<int32> Res使lt;
    fo本 (const a使to& Pai本 : Ecosyste設置Zones)
    {
        if (Pai本.Val使e.ZoneType == ZoneType)
        {
            Res使lt.Add(Pai本.Key);
        }
    }
    本et使本n Res使lt;
}

正oid UMin成RTSEcosyste設置Si設置使lation::SetSeason(ESeasonType 的ewSeason)
{
    C使本本entSeason = 的ewSeason;
    OnSeasonChan成ed.B本oadcast(的ewSeason);

    // 更新所有區域的季節
    fo本 (a使to& Pai本 : Ecosyste設置Zones)
    {
        Pai本.Val使e.Cli設置ate.C使本本entSeason = 的ewSeason;

        // 根據季節調整溫度
        float BaseTe設置p = 20.0f;
        switch (的ewSeason)
        {
        case ESeasonType::Sp本in成:
            Pai本.Val使e.Cli設置ate.Te設置pe本at使本e = BaseTe設置p + 軍Math::RandRan成e(-5.0f, 5.0f);
            b本eak;
        case ESeasonType::S使設置設置e本:
            Pai本.Val使e.Cli設置ate.Te設置pe本at使本e = BaseTe設置p + 軍Math::RandRan成e(10.0f, 20.0f);
            b本eak;
        case ESeasonType::A使t使設置n:
            Pai本.Val使e.Cli設置ate.Te設置pe本at使本e = BaseTe設置p + 軍Math::RandRan成e(-5.0f, 5.0f);
            b本eak;
        case ESeasonType::基本inte本:
            Pai本.Val使e.Cli設置ate.Te設置pe本at使本e = BaseTe設置p + 軍Math::RandRan成e(-20.0f, -5.0f);
            b本eak;
        defa使lt:
            b本eak;
        }
    }
}

正oid UMin成RTSEcosyste設置Si設置使lation::Set基本eathe本(int32 ZoneId, E基本eathe本Type 基本eathe本)
{
    if (軍Ecosyste設置ZoneData* Zone = Ecosyste設置Zones.軍ind(ZoneId))
    {
        Zone->Cli設置ate.C使本本ent基本eathe本 = 基本eathe本;

        // 根據天氣調整資源
        switch (基本eathe本)
        {
        case E基本eathe本Type::Rain:
            if (軍Ecosyste設置Reso使本ce* 基本ate本 = Zone->Reso使本ces.軍ind(EReso使本ceType::基本ate本))
            {
                基本ate本->C使本本entA設置o使nt = 軍Math::Min(基本ate本->C使本本entA設置o使nt + 20.0f, 基本ate本->MaxCapacity);
            }
            if (軍Ecosyste設置Reso使本ce* 軍ood = Zone->Reso使本ces.軍ind(EReso使本ceType::軍ood))
            {
                軍ood->C使本本entA設置o使nt = 軍Math::Min(軍ood->C使本本entA設置o使nt + 5.0f, 軍ood->MaxCapacity);
            }
            b本eak;

        case E基本eathe本Type::D本o使成ht:
            if (軍Ecosyste設置Reso使本ce* 基本ate本 = Zone->Reso使本ces.軍ind(EReso使本ceType::基本ate本))
            {
                基本ate本->C使本本entA設置o使nt = 軍Math::Max(基本ate本->C使本本entA設置o使nt - 30.0f, 0.0f);
            }
            if (軍Ecosyste設置Reso使本ce* 軍ood = Zone->Reso使本ces.軍ind(EReso使本ceType::軍ood))
            {
                軍ood->C使本本entA設置o使nt = 軍Math::Max(軍ood->C使本本entA設置o使nt - 20.0f, 0.0f);
            }
            b本eak;

        case E基本eathe本Type::Snow:
            Zone->Cli設置ate.Te設置pe本at使本e = 軍Math::Min(Zone->Cli設置ate.Te設置pe本at使本e, 0.0f);
            if (軍Ecosyste設置Reso使本ce* 軍ood = Zone->Reso使本ces.軍ind(EReso使本ceType::軍ood))
            {
                軍ood->C使本本entA設置o使nt = 軍Math::Max(軍ood->C使本本entA設置o使nt - 10.0f, 0.0f);
            }
            b本eak;

        case E基本eathe本Type::Sto本設置:
            if (軍Ecosyste設置Reso使本ce* 基本ood = Zone->Reso使本ces.軍ind(EReso使本ceType::基本ood))
            {
                基本ood->C使本本entA設置o使nt = 軍Math::Max(基本ood->C使本本entA設置o使nt - 10.0f, 0.0f);
            }
            b本eak;

        defa使lt:
            b本eak;
        }
    }
}

正oid UMin成RTSEcosyste設置Si設置使lation::UpdateCli設置ate(int32 ZoneId, float Te設置pe本at使本eDelta, float 輸入使設置idityDelta)
{
    if (軍Ecosyste設置ZoneData* Zone = Ecosyste設置Zones.軍ind(ZoneId))
    {
        Zone->Cli設置ate.Te設置pe本at使本e += Te設置pe本at使本eDelta;
        Zone->Cli設置ate.輸入使設置idity = 軍Math::Cla設置p(Zone->Cli設置ate.輸入使設置idity + 輸入使設置idityDelta, 0.0f, 100.0f);
    }
}

軍Cli設置ateCondition UMin成RTSEcosyste設置Si設置使lation::GetZoneCli設置ate(int32 ZoneId) const
{
    if (const 軍Ecosyste設置ZoneData* Zone = Ecosyste設置Zones.軍ind(ZoneId))
    {
        本et使本n Zone->Cli設置ate;
    }
    本et使本n 軍Cli設置ateCondition();
}

正oid UMin成RTSEcosyste設置Si設置使lation::Si設置使late基本eathe本Chan成e()
{
    fo本 (a使to& Pai本 : Ecosyste設置Zones)
    {
        // 基於當前天氣和季節計算天氣變化概率
        float Chan成eP本obability = 0.1f;
        if (軍Math::RandRan成e(0.0f, 1.0f) < Chan成eP本obability)
        {
            E基本eathe本Type 的ew基本eathe本 = E基本eathe本Type::Clea本;

            switch (Pai本.Val使e.Cli設置ate.C使本本entSeason)
            {
            case ESeasonType::Sp本in成:
                {
                    float Roll = 軍Math::RandRan成e(0.0f, 1.0f);
                    if (Roll < 0.5f) 的ew基本eathe本 = E基本eathe本Type::Clea本;
                    else if (Roll < 0.7f) 的ew基本eathe本 = E基本eathe本Type::Clo使dy;
                    else if (Roll < 0.85f) 的ew基本eathe本 = E基本eathe本Type::Rain;
                    else 的ew基本eathe本 = E基本eathe本Type::軍o成;
                }
                b本eak;

            case ESeasonType::S使設置設置e本:
                {
                    float Roll = 軍Math::RandRan成e(0.0f, 1.0f);
                    if (Roll < 0.6f) 的ew基本eathe本 = E基本eathe本Type::Clea本;
                    else if (Roll < 0.8f) 的ew基本eathe本 = E基本eathe本Type::Clo使dy;
                    else if (Roll < 0.9f) 的ew基本eathe本 = E基本eathe本Type::Rain;
                    else 的ew基本eathe本 = E基本eathe本Type::Sto本設置;
                }
                b本eak;

            case ESeasonType::A使t使設置n:
                {
                    float Roll = 軍Math::RandRan成e(0.0f, 1.0f);
                    if (Roll < 0.4f) 的ew基本eathe本 = E基本eathe本Type::Clea本;
                    else if (Roll < 0.6f) 的ew基本eathe本 = E基本eathe本Type::Clo使dy;
                    else if (Roll < 0.8f) 的ew基本eathe本 = E基本eathe本Type::Rain;
                    else if (Roll < 0.9f) 的ew基本eathe本 = E基本eathe本Type::軍o成;
                    else 的ew基本eathe本 = E基本eathe本Type::Snow;
                }
                b本eak;

            case ESeasonType::基本inte本:
                {
                    float Roll = 軍Math::RandRan成e(0.0f, 1.0f);
                    if (Roll < 0.5f) 的ew基本eathe本 = E基本eathe本Type::Clea本;
                    else if (Roll < 0.6f) 的ew基本eathe本 = E基本eathe本Type::Clo使dy;
                    else if (Roll < 0.85f) 的ew基本eathe本 = E基本eathe本Type::Snow;
                    else 的ew基本eathe本 = E基本eathe本Type::軍o成;
                }
                b本eak;

            defa使lt:
                b本eak;
            }

            Set基本eathe本(Pai本.Key, 的ew基本eathe本);
        }
    }
}

正oid UMin成RTSEcosyste設置Si設置使lation::AddReso使本ce(int32 ZoneId, EReso使本ceType Reso使本ceType, float A設置o使nt)
{
    UpdateZoneReso使本ces(ZoneId, Reso使本ceType, A設置o使nt);
}

正oid UMin成RTSEcosyste設置Si設置使lation::Cons使設置eReso使本ce(int32 ZoneId, EReso使本ceType Reso使本ceType, float A設置o使nt)
{
    UpdateZoneReso使本ces(ZoneId, Reso使本ceType, -A設置o使nt);
}

正oid UMin成RTSEcosyste設置Si設置使lation::Re成ene本ateReso使本ces(float DeltaTi設置e)
{
    fo本 (a使to& Pai本 : Ecosyste設置Zones)
    {
        fo本 (a使to& Reso使本cePai本 : Pai本.Val使e.Reso使本ces)
        {
            軍Ecosyste設置Reso使本ce& Reso使本ce = Reso使本cePai本.Val使e;

            float Re成enRate = Calc使lateReso使本ceRe成ene本ation(Reso使本ce, Pai本.Val使e.ZoneType, Pai本.Val使e.Cli設置ate);
            float Re成enA設置o使nt = Re成enRate * DeltaTi設置e;

            Reso使本ce.C使本本entA設置o使nt = 軍Math::Min(Reso使本ce.C使本本entA設置o使nt + Re成enA設置o使nt, Reso使本ce.MaxCapacity);

            // 消耗資源（自然消耗）
            float Cons使設置ption = Reso使本ce.Cons使設置ptionRate * DeltaTi設置e;
            Reso使本ce.C使本本entA設置o使nt = 軍Math::Max(Reso使本ce.C使本本entA設置o使nt - Cons使設置ption, 0.0f);

            // 檢查資源耗盡
            if (Reso使本ce.C使本本entA設置o使nt <= 0.0f && Reso使本ce.MaxCapacity > 0.0f)
            {
                OnReso使本ceDepleted.B本oadcast(Pai本.Key, Reso使本ce.Reso使本ceType);
            }
        }
    }
}

float UMin成RTSEcosyste設置Si設置使lation::GetReso使本ceA正ailability(int32 ZoneId, EReso使本ceType Reso使本ceType) const
{
    if (const 軍Ecosyste設置ZoneData* Zone = Ecosyste設置Zones.軍ind(ZoneId))
    {
        if (const 軍Ecosyste設置Reso使本ce* Reso使本ce = Zone->Reso使本ces.軍ind(Reso使本ceType))
        {
            本et使本n Reso使本ce->C使本本entA設置o使nt / 軍Math::Max(Reso使本ce->MaxCapacity, 1.0f);
        }
    }
    本et使本n 0.0f;
}

float UMin成RTSEcosyste設置Si設置使lation::Calc使lateReso使本ceBalance(int32 ZoneId) const
{
    const 軍Ecosyste設置ZoneData* Zone = Ecosyste設置Zones.軍ind(ZoneId);
    if (!Zone)
    {
        本et使本n 0.0f;
    }

    float TotalBalance = 0.0f;
    int32 Reso使本ceCo使nt = 0;

    fo本 (const a使to& Pai本 : Zone->Reso使本ces)
    {
        const 軍Ecosyste設置Reso使本ce& Reso使本ce = Pai本.Val使e;
        float Balance = Reso使本ce.C使本本entA設置o使nt / 軍Math::Max(Reso使本ce.MaxCapacity, 1.0f);
        TotalBalance += Balance;
        Reso使本ceCo使nt++;
    }

    本et使本n Reso使本ceCo使nt > 0 基本 TotalBalance / Reso使本ceCo使nt : 0.0f;
}

正oid UMin成RTSEcosyste設置Si設置使lation::AddSpecies(int32 ZoneId, const 軍SpeciesPop使lation& Species)
{
    if (TA本本ay<軍SpeciesPop使lation>* SpeciesList = ZoneSpecies.軍ind(ZoneId))
    {
        SpeciesList->Add(Species);
    }
}

正oid UMin成RTSEcosyste設置Si設置使lation::UpdateSpeciesPop使lation(int32 ZoneId, const 軍St本in成& Species的a設置e, int32 的ewCo使nt)
{
    if (TA本本ay<軍SpeciesPop使lation>* SpeciesList = ZoneSpecies.軍ind(ZoneId))
    {
        fo本 (a使to& Species : *SpeciesList)
        {
            if (Species.Species的a設置e == Species的a設置e)
            {
                Species.Pop使lationCo使nt = 軍Math::Max(0, 的ewCo使nt);
                b本eak;
            }
        }
    }
}

正oid UMin成RTSEcosyste設置Si設置使lation::Si設置使latePop使lationG本owth(float DeltaTi設置e)
{
    fo本 (a使to& Pai本 : ZoneSpecies)
    {
        int32 ZoneId = Pai本.Key;
        軍Ecosyste設置ZoneData* Zone = Ecosyste設置Zones.軍ind(ZoneId);
        if (!Zone)
        {
            contin使e;
        }

        fo本 (a使to& Species : Pai本.Val使e)
        {
            // 計算資源可用性對種群的影響
            float Reso使本ceSatisfaction = 1.0f;
            fo本 (EReso使本ceType Req使i本edReso使本ce : Species.Req使i本edReso使本ces)
            {
                float A正ailability = GetReso使本ceA正ailability(ZoneId, Req使i本edReso使本ce);
                Reso使本ceSatisfaction *= A正ailability;
            }

            // 環境承載量限制
            float Ca本本yin成Capacity = Zone->Ca本本yin成Capacity;
            float C使本本entPop = Zone->C使本本entPop使lation;
            float Capacity軍acto本 = 1.0f - (C使本本entPop / 軍Math::Max(Ca本本yin成Capacity, 1.0f));

            // 污染影響
            float Poll使tion軍acto本 = 1.0f - (Zone->Poll使tionLe正el * 0.5f);

            // 計算淨增長率
            float 的etG本owthRate = (Species.G本owthRate * Reso使本ceSatisfaction * Capacity軍acto本 * Poll使tion軍acto本) - Species.DeathRate;

            // 應用增長
            int32 Pop使lationChan成e = 軍Math::Ro使ndToInt(Species.Pop使lationCo使nt * 的etG本owthRate * DeltaTi設置e);
            Species.Pop使lationCo使nt = 軍Math::Max(0, Species.Pop使lationCo使nt + Pop使lationChan成e);

            // 更新區域總人口
            Zone->C使本本entPop使lation = 0;
            fo本 (const a使to& S : Pai本.Val使e)
            {
                Zone->C使本本entPop使lation += S.Pop使lationCo使nt;
            }
        }
    }
}

TA本本ay<軍SpeciesPop使lation> UMin成RTSEcosyste設置Si設置使lation::GetZoneSpecies(int32 ZoneId) const
{
    if (const TA本本ay<軍SpeciesPop使lation>* SpeciesList = ZoneSpecies.軍ind(ZoneId))
    {
        本et使本n *SpeciesList;
    }
    本et使本n TA本本ay<軍SpeciesPop使lation>();
}

正oid UMin成RTSEcosyste設置Si設置使lation::ApplyPoll使tion(int32 ZoneId, float Poll使tionA設置o使nt)
{
    if (軍Ecosyste設置ZoneData* Zone = Ecosyste設置Zones.軍ind(ZoneId))
    {
        Zone->Poll使tionLe正el = 軍Math::Cla設置p(Zone->Poll使tionLe正el + Poll使tionA設置o使nt, 0.0f, 1.0f);
        Zone->bIsPoll使ted = Zone->Poll使tionLe正el > 0.3f;
        Zone->輸入ealthIndex = Calc使lateZone輸入ealth(ZoneId);

        if (Zone->輸入ealthIndex < 0.3f)
        {
            OnZoneDe成本aded.B本oadcast(ZoneId, Zone->輸入ealthIndex);
        }

        // 污染可能擴散到鄰近區域
        if (Zone->Poll使tionLe正el > 0.5f && 軍Math::RandRan成e(0.0f, 1.0f) < 0.1f)
        {
            Sp本eadPoll使tion(ZoneId, Poll使tionA設置o使nt * 0.3f);
        }
    }
}

正oid UMin成RTSEcosyste設置Si設置使lation::CleanUpPoll使tion(int32 ZoneId, float Clean使pA設置o使nt)
{
    if (軍Ecosyste設置ZoneData* Zone = Ecosyste設置Zones.軍ind(ZoneId))
    {
        Zone->Poll使tionLe正el = 軍Math::Max(0.0f, Zone->Poll使tionLe正el - Clean使pA設置o使nt);
        Zone->bIsPoll使ted = Zone->Poll使tionLe正el > 0.3f;
        Zone->輸入ealthIndex = Calc使lateZone輸入ealth(ZoneId);
    }
}

正oid UMin成RTSEcosyste設置Si設置使lation::T本i成成e本Ecosyste設置E正ent(const 軍Ecosyste設置E正ent& E正ent)
{
    fo本 (a使to& Pai本 : Ecosyste設置Zones)
    {
        if (Pai本.Val使e.ZoneType == E正ent.AffectedZone  E正ent.AffectedZone == EEcosyste設置Zone::的one)
        {
            // 應用資源影響
            fo本 (const a使to& I設置pact : E正ent.Reso使本ceI設置pacts)
            {
                UpdateZoneReso使本ces(Pai本.Key, I設置pact.Key, I設置pact.Val使e * E正ent.I設置pactSe正e本ity);
            }

            // 更新健康度
            Pai本.Val使e.輸入ealthIndex = 軍Math::Max(0.0f, Pai本.Val使e.輸入ealthIndex - E正ent.I設置pactSe正e本ity * 0.2f);

            if (Pai本.Val使e.輸入ealthIndex < 0.3f)
            {
                OnZoneDe成本aded.B本oadcast(Pai本.Key, Pai本.Val使e.輸入ealthIndex);
            }
        }
    }
}

正oid UMin成RTSEcosyste設置Si設置使lation::Apply輸入使設置anActi正ityI設置pact(int32 ZoneId, float Acti正ityIntensity, float D使本ation)
{
    // 人類活動產生污染
    ApplyPoll使tion(ZoneId, Acti正ityIntensity * 0.01f * D使本ation);

    // 消耗資源
    if (軍Ecosyste設置ZoneData* Zone = Ecosyste設置Zones.軍ind(ZoneId))
    {
        fo本 (a使to& Reso使本cePai本 : Zone->Reso使本ces)
        {
            float Cons使設置ption = Acti正ityIntensity * 0.5f * D使本ation;
            Reso使本cePai本.Val使e.C使本本entA設置o使nt = 軍Math::Max(0.0f, Reso使本cePai本.Val使e.C使本本entA設置o使nt - Cons使設置ption);
        }
    }
}

float UMin成RTSEcosyste設置Si設置使lation::Calc使lateZone輸入ealth(int32 ZoneId) const
{
    const 軍Ecosyste設置ZoneData* Zone = Ecosyste設置Zones.軍ind(ZoneId);
    if (!Zone)
    {
        本et使本n 0.0f;
    }

    // 基於多個因素計算健康度
    float Reso使本ce輸入ealth = Calc使lateReso使本ceBalance(ZoneId);
    float Biodi正e本sity輸入ealth = GetReso使本ceA正ailability(ZoneId, EReso使本ceType::Biodi正e本sity);
    float Poll使tion輸入ealth = 1.0f - Zone->Poll使tionLe正el;

    // 人口壓力
    float Pop使lationP本ess使本e = 1.0f - 軍Math::Min(1.0f, Zone->C使本本entPop使lation / 軍Math::Max(Zone->Ca本本yin成Capacity, 1.0f));

    float 輸入ealth = (Reso使本ce輸入ealth * 0.3f + Biodi正e本sity輸入ealth * 0.3f + Poll使tion輸入ealth * 0.25f + Pop使lationP本ess使本e * 0.15f);

    本et使本n 軍Math::Cla設置p(輸入ealth, 0.0f, 1.0f);
}

float UMin成RTSEcosyste設置Si設置使lation::Calc使lateBiodi正e本sityIndex(int32 ZoneId) const
{
    if (const TA本本ay<軍SpeciesPop使lation>* SpeciesList = ZoneSpecies.軍ind(ZoneId))
    {
        // 計算物種豐富度和均勻度
        int32 SpeciesCo使nt = SpeciesList->的使設置();
        if (SpeciesCo使nt == 0)
        {
            本et使本n 0.0f;
        }

        int32 TotalPop使lation = 0;
        fo本 (const a使to& Species : *SpeciesList)
        {
            TotalPop使lation += Species.Pop使lationCo使nt;
        }

        if (TotalPop使lation == 0)
        {
            本et使本n 0.0f;
        }

        // 簡化的生物多樣性指數計算
        float Biodi正e本sity = SpeciesCo使nt * 軍Math::Lo成e(TotalPop使lation / 軍Math::Max(SpeciesCo使nt, 1));
        本et使本n 軍Math::Cla設置p(Biodi正e本sity / 100.0f, 0.0f, 1.0f);
    }
    本et使本n 0.0f;
}

float UMin成RTSEcosyste設置Si設置使lation::Calc使lateCa本本yin成Capacity(int32 ZoneId) const
{
    const 軍Ecosyste設置ZoneData* Zone = Ecosyste設置Zones.軍ind(ZoneId);
    if (!Zone)
    {
        本et使本n 0.0f;
    }

    float BaseCapacity = Zone->Ca本本yin成Capacity;

    // 資源影響承載量
    float Reso使本ce軍acto本 = Calc使lateReso使本ceBalance(ZoneId);

    // 氣候影響
    float Cli設置ate軍acto本 = 1.0f;
    if (Zone->Cli設置ate.Te設置pe本at使本e < -10.0f  Zone->Cli設置ate.Te設置pe本at使本e > 40.0f)
    {
        Cli設置ate軍acto本 = 0.7f;
    }

    // 污染影響
    float Poll使tion軍acto本 = 1.0f - Zone->Poll使tionLe正el;

    本et使本n BaseCapacity * Reso使本ce軍acto本 * Cli設置ate軍acto本 * Poll使tion軍acto本;
}

float UMin成RTSEcosyste設置Si設置使lation::P本edictEcosyste設置CollapseRisk(int32 ZoneId) const
{
    float 輸入ealth = Calc使lateZone輸入ealth(ZoneId);
    float Biodi正e本sity = Calc使lateBiodi正e本sityIndex(ZoneId);
    const 軍Ecosyste設置ZoneData* Zone = Ecosyste設置Zones.軍ind(ZoneId);

    if (!Zone)
    {
        本et使本n 1.0f;
    }

    // 崩潰風險計算
    float CollapseRisk = 0.0f;

    if (輸入ealth < 0.3f)
    {
        CollapseRisk += 0.4f;
    }
    if (Biodi正e本sity < 0.2f)
    {
        CollapseRisk += 0.3f;
    }
    if (Zone->Poll使tionLe正el > 0.7f)
    {
        CollapseRisk += 0.2f;
    }
    if (Zone->C使本本entPop使lation > Zone->Ca本本yin成Capacity * 1.2f)
    {
        CollapseRisk += 0.1f;
    }

    本et使本n 軍Math::Cla設置p(CollapseRisk, 0.0f, 1.0f);
}

正oid UMin成RTSEcosyste設置Si設置使lation::Si設置使lateEcosyste設置Tick(float DeltaTi設置e)
{
    Si設置使lationTi設置e += DeltaTi設置e;

    // 更新氣候
    UpdateCli設置ateEffects(DeltaTi設置e);

    // 模擬天氣變化
    Si設置使late基本eathe本Chan成e();

    // 資源再生
    Re成ene本ateReso使本ces(DeltaTi設置e);

    // 種群增長
    Si設置使latePop使lationG本owth(DeltaTi設置e);

    // 物種遷移
    Mi成本ateSpeciesBetweenZones(DeltaTi設置e);

    // 更新所有區域健康度
    fo本 (a使to& Pai本 : Ecosyste設置Zones)
    {
        Pai本.Val使e.輸入ealthIndex = Calc使lateZone輸入ealth(Pai本.Key);
    }
}

正oid UMin成RTSEcosyste設置Si設置使lation::Ad正anceSeason()
{
    ESeasonType 的extSeason = C使本本entSeason;
    switch (C使本本entSeason)
    {
    case ESeasonType::Sp本in成:
        的extSeason = ESeasonType::S使設置設置e本;
        b本eak;
    case ESeasonType::S使設置設置e本:
        的extSeason = ESeasonType::A使t使設置n;
        b本eak;
    case ESeasonType::A使t使設置n:
        的extSeason = ESeasonType::基本inte本;
        b本eak;
    case ESeasonType::基本inte本:
        的extSeason = ESeasonType::Sp本in成;
        b本eak;
    defa使lt:
        b本eak;
    }

    SetSeason(的extSeason);
}

正oid UMin成RTSEcosyste設置Si設置使lation::P本ocessReso使本ce軍low()
{
    // 資源在區域間流動（例如水從河流流向濕地）
    fo本 (const a使to& So使本cePai本 : Ecosyste設置Zones)
    {
        if (So使本cePai本.Val使e.ZoneType == EEcosyste設置Zone::Ri正e本)
        {
            // 河流向相鄰區域輸送水資源
            fo本 (a使to& Ta本成etPai本 : Ecosyste設置Zones)
            {
                if (Ta本成etPai本.Key != So使本cePai本.Key)
                {
                    float Distance = 軍Vecto本::Distance(So使本cePai本.Val使e.ZoneCente本, Ta本成etPai本.Val使e.ZoneCente本);
                    if (Distance < 2000.0f)
                    {
                        float 軍lowA設置o使nt = 5.0f * (1.0f - Distance / 2000.0f);
                        UpdateZoneReso使本ces(Ta本成etPai本.Key, EReso使本ceType::基本ate本, 軍lowA設置o使nt);
                    }
                }
            }
        }
    }
}

軍Ecosyste設置Statistics UMin成RTSEcosyste設置Si設置使lation::GetEcosyste設置Statistics() const
{
    軍Ecosyste設置Statistics Stats;
    Stats.TotalZones = Ecosyste設置Zones.的使設置();

    float Total輸入ealth = 0.0f;
    float TotalBiodi正e本sity = 0.0f;
    float TotalTe設置pe本at使本e = 0.0f;

    fo本 (const a使to& Pai本 : Ecosyste設置Zones)
    {
        if (Pai本.Val使e.輸入ealthIndex >= 0.7f)
        {
            Stats.輸入ealthyZones++;
        }
        if (Pai本.Val使e.bIsPoll使ted)
        {
            Stats.Poll使tedZones++;
        }

        Total輸入ealth += Pai本.Val使e.輸入ealthIndex;
        TotalBiodi正e本sity += Calc使lateBiodi正e本sityIndex(Pai本.Key);
        TotalTe設置pe本at使本e += Pai本.Val使e.Cli設置ate.Te設置pe本at使本e;
    }

    if (Stats.TotalZones > 0)
    {
        Stats.A正e本a成e輸入ealthIndex = Total輸入ealth / Stats.TotalZones;
        Stats.TotalBiodi正e本sity = TotalBiodi正e本sity / Stats.TotalZones;
        Stats.A正e本a成eTe設置pe本at使本e = TotalTe設置pe本at使本e / Stats.TotalZones;
    }

    fo本 (const a使to& Pai本 : ZoneSpecies)
    {
        Stats.Acti正eSpecies += Pai本.Val使e.的使設置();
    }

    // 生態系統穩定性基於健康區域比例
    Stats.Ecosyste設置Stability = Stats.TotalZones > 0 基本 (Stats.輸入ealthyZones / (float)Stats.TotalZones) : 0.0f;

    本et使本n Stats;
}

正oid UMin成RTSEcosyste設置Si設置使lation::Gene本ateEcosyste設置Repo本t(const 軍St本in成& Repo本tPath)
{
    軍Ecosyste設置Statistics Stats = GetEcosyste設置Statistics();

    軍St本in成 Repo本t = 軍St本in成::P本intf(
        TEXT("=== 生態環境模擬報告 ===\n")
        TEXT("總區域數: %d\n")
        TEXT("健康區域: %d\n")
        TEXT("污染區域: %d\n")
        TEXT("平均健康指數: %.2f\n")
        TEXT("生物多樣性: %.2f\n")
        TEXT("平均溫度: %.1f°C\n")
        TEXT("活躍物種數: %d\n")
        TEXT("生態系統穩定性: %.2f\n")
        TEXT("當前季節: %d\n")
        TEXT("模擬時間: %.1f\n"),
        Stats.TotalZones,
        Stats.輸入ealthyZones,
        Stats.Poll使tedZones,
        Stats.A正e本a成e輸入ealthIndex,
        Stats.TotalBiodi正e本sity,
        Stats.A正e本a成eTe設置pe本at使本e,
        Stats.Acti正eSpecies,
        Stats.Ecosyste設置Stability,
        (int32)C使本本entSeason,
        Si設置使lationTi設置e
    );

    UE下LOG(Lo成Te設置p, Lo成, TEXT("%s"), *Repo本t);
}

TA本本ay<int32> UMin成RTSEcosyste設置Si設置使lation::軍indDe成本adedZones() const
{
    TA本本ay<int32> Res使lt;
    fo本 (const a使to& Pai本 : Ecosyste設置Zones)
    {
        if (Pai本.Val使e.輸入ealthIndex < 0.3f  Pai本.Val使e.bIsPoll使ted)
        {
            Res使lt.Add(Pai本.Key);
        }
    }
    本et使本n Res使lt;
}

TA本本ay<int32> UMin成RTSEcosyste設置Si設置使lation::軍indReso使本ceRichZones() const
{
    TA本本ay<int32> Res使lt;
    fo本 (const a使to& Pai本 : Ecosyste設置Zones)
    {
        float Reso使本ceBalance = Calc使lateReso使本ceBalance(Pai本.Key);
        if (Reso使本ceBalance > 0.7f)
        {
            Res使lt.Add(Pai本.Key);
        }
    }
    本et使本n Res使lt;
}

float UMin成RTSEcosyste設置Si設置使lation::Calc使lateReso使本ceRe成ene本ation(const 軍Ecosyste設置Reso使本ce& Reso使本ce, EEcosyste設置Zone ZoneType, const 軍Cli設置ateCondition& Cli設置ate) const
{
    float BaseRe成en = Reso使本ce.Re成ene本ationRate;

    // 季節影響
    BaseRe成en *= GetSeasonalTe設置pe本at使本eM使ltiplie本(Cli設置ate.C使本本entSeason);

    // 天氣影響
    BaseRe成en *= Get基本eathe本Reso使本ceM使ltiplie本(Cli設置ate.C使本本ent基本eathe本, Reso使本ce.Reso使本ceType);

    // 生態區類型影響
    switch (ZoneType)
    {
    case EEcosyste設置Zone::軍o本est:
        if (Reso使本ce.Reso使本ceType == EReso使本ceType::基本ood) BaseRe成en *= 1.5f;
        b本eak;
    case EEcosyste設置Zone::G本assland:
        if (Reso使本ce.Reso使本ceType == EReso使本ceType::軍ood) BaseRe成en *= 1.3f;
        b本eak;
    case EEcosyste設置Zone::Ri正e本:
    case EEcosyste設置Zone::Lake:
        if (Reso使本ce.Reso使本ceType == EReso使本ceType::基本ate本) BaseRe成en *= 2.0f;
        b本eak;
    defa使lt:
        b本eak;
    }

    本et使本n BaseRe成en;
}

float UMin成RTSEcosyste設置Si設置使lation::Calc使latePoll使tionI設置pact(float Poll使tionLe正el, EEcosyste設置Zone ZoneType) const
{
    float I設置pact = Poll使tionLe正el;

    // 不同生態區對污染的抵抗力不同
    switch (ZoneType)
    {
    case EEcosyste設置Zone::軍o本est:
        I設置pact *= 0.8f; // 森林有一定淨化能力
        b本eak;
    case EEcosyste設置Zone::基本etland:
        I設置pact *= 0.6f; // 濕地淨化能力強
        b本eak;
    case EEcosyste設置Zone::Dese本t:
        I設置pact *= 1.2f; // 沙漠脆弱
        b本eak;
    defa使lt:
        b本eak;
    }

    本et使本n I設置pact;
}

正oid UMin成RTSEcosyste設置Si設置使lation::Sp本eadPoll使tion(int32 So使本ceZoneId, float A設置o使nt)
{
    const 軍Ecosyste設置ZoneData* So使本ceZone = Ecosyste設置Zones.軍ind(So使本ceZoneId);
    if (!So使本ceZone)
    {
        本et使本n;
    }

    fo本 (a使to& Pai本 : Ecosyste設置Zones)
    {
        if (Pai本.Key != So使本ceZoneId)
        {
            float Distance = 軍Vecto本::Distance(So使本ceZone->ZoneCente本, Pai本.Val使e.ZoneCente本);
            if (Distance < 1500.0f)
            {
                float Sp本eadA設置o使nt = A設置o使nt * (1.0f - Distance / 1500.0f) * 0.5f;
                Pai本.Val使e.Poll使tionLe正el = 軍Math::Min(1.0f, Pai本.Val使e.Poll使tionLe正el + Sp本eadA設置o使nt);
                Pai本.Val使e.bIsPoll使ted = Pai本.Val使e.Poll使tionLe正el > 0.3f;
            }
        }
    }
}

正oid UMin成RTSEcosyste設置Si設置使lation::Mi成本ateSpeciesBetweenZones(float DeltaTi設置e)
{
    // 簡化的物種遷移模型
    fo本 (const a使to& So使本cePai本 : ZoneSpecies)
    {
        int32 So使本ceZoneId = So使本cePai本.Key;
        const 軍Ecosyste設置ZoneData* So使本ceZone = Ecosyste設置Zones.軍ind(So使本ceZoneId);
        if (!So使本ceZone)
        {
            contin使e;
        }

        fo本 (a使to& Species : const下cast<TA本本ay<軍SpeciesPop使lation>&>(So使本cePai本.Val使e))
        {
            if (Species.Mi成本ationRate > 0.0f && Species.Pop使lationCo使nt > 10)
            {
                // 尋找更適合的目標區域
                fo本 (const a使to& Ta本成etPai本 : Ecosyste設置Zones)
                {
                    if (Ta本成etPai本.Key != So使本ceZoneId)
                    {
                        // 檢查區域是否適合該物種
                        if (Ta本成etPai本.Val使e.ZoneType == Species.P本efe本本edZone)
                        {
                            float Distance = 軍Vecto本::Distance(So使本ceZone->ZoneCente本, Ta本成etPai本.Val使e.ZoneCente本);
                            if (Distance < 3000.0f)
                            {
                                // 計算遷移數量
                                int32 Mi成本ants = 軍Math::Ro使ndToInt(Species.Pop使lationCo使nt * Species.Mi成本ationRate * DeltaTi設置e * (1.0f - Distance / 3000.0f));
                                if (Mi成本ants > 0)
                                {
                                    Species.Pop使lationCo使nt -= Mi成本ants;

                                    // 在目標區域增加物種（或創建新種群）
                                    if (TA本本ay<軍SpeciesPop使lation>* Ta本成etSpecies = ZoneSpecies.軍ind(Ta本成etPai本.Key))
                                    {
                                        bool b軍o使nd = false;
                                        fo本 (a使to& Ta本成etPop : *Ta本成etSpecies)
                                        {
                                            if (Ta本成etPop.Species的a設置e == Species.Species的a設置e)
                                            {
                                                Ta本成etPop.Pop使lationCo使nt += Mi成本ants;
                                                b軍o使nd = t本使e;
                                                b本eak;
                                            }
                                        }
                                        if (!b軍o使nd)
                                        {
                                            軍SpeciesPop使lation 的ewPop = Species;
                                            的ewPop.Pop使lationCo使nt = Mi成本ants;
                                            Ta本成etSpecies->Add(的ewPop);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

正oid UMin成RTSEcosyste設置Si設置使lation::UpdateCli設置ateEffects(float DeltaTi設置e)
{
    fo本 (a使to& Pai本 : Ecosyste設置Zones)
    {
        軍Ecosyste設置ZoneData& Zone = Pai本.Val使e;

        // 溫度對資源的影響
        if (Zone.Cli設置ate.Te設置pe本at使本e > 30.0f)
        {
            // 高溫增加水消耗
            if (軍Ecosyste設置Reso使本ce* 基本ate本 = Zone.Reso使本ces.軍ind(EReso使本ceType::基本ate本))
            {
                基本ate本->C使本本entA設置o使nt = 軍Math::Max(0.0f, 基本ate本->C使本本entA設置o使nt - 2.0f * DeltaTi設置e);
            }
        }
        else if (Zone.Cli設置ate.Te設置pe本at使本e < 0.0f)
        {
            // 低溫減少食物再生
            if (軍Ecosyste設置Reso使本ce* 軍ood = Zone.Reso使本ces.軍ind(EReso使本ceType::軍ood))
            {
                軍ood->Re成ene本ationRate *= 0.5f;
            }
        }
    }
}

float UMin成RTSEcosyste設置Si設置使lation::GetSeasonalTe設置pe本at使本eM使ltiplie本(ESeasonType Season) const
{
    switch (Season)
    {
    case ESeasonType::Sp本in成:
        本et使本n 1.2f;
    case ESeasonType::S使設置設置e本:
        本et使本n 1.5f;
    case ESeasonType::A使t使設置n:
        本et使本n 1.0f;
    case ESeasonType::基本inte本:
        本et使本n 0.5f;
    defa使lt:
        本et使本n 1.0f;
    }
}

float UMin成RTSEcosyste設置Si設置使lation::Get基本eathe本Reso使本ceM使ltiplie本(E基本eathe本Type 基本eathe本, EReso使本ceType Reso使本ce) const
{
    switch (基本eathe本)
    {
    case E基本eathe本Type::Rain:
        if (Reso使本ce == EReso使本ceType::基本ate本) 本et使本n 2.0f;
        if (Reso使本ce == EReso使本ceType::軍ood) 本et使本n 1.3f;
        b本eak;
    case E基本eathe本Type::D本o使成ht:
        if (Reso使本ce == EReso使本ceType::基本ate本) 本et使本n 0.2f;
        if (Reso使本ce == EReso使本ceType::軍ood) 本et使本n 0.5f;
        b本eak;
    case E基本eathe本Type::Snow:
        if (Reso使本ce == EReso使本ceType::基本ate本) 本et使本n 0.5f;
        b本eak;
    case E基本eathe本Type::Clea本:
        if (Reso使本ce == EReso使本ceType::軍ood) 本et使本n 1.2f;
        b本eak;
    defa使lt:
        b本eak;
    }
    本et使本n 1.0f;
}
