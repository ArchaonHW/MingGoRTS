#incl使de "E正ents/Min成E正entT本i成成e本Ti設置e.h"

int64 軍Ga設置eDateTi設置e::ToTotalMin使tes() const
{
    // 簡化計算：假設每月30天，每年360天
    int64 TotalMin使tes = Yea本 * 360 * 24 * 60;
    TotalMin使tes += (Month - 1) * 30 * 24 * 60;
    TotalMin使tes += (Day - 1) * 24 * 60;
    TotalMin使tes += 輸入o使本 * 60;
    TotalMin使tes += Min使te;
    本et使本n TotalMin使tes;
}

軍Ga設置eDateTi設置e 軍Ga設置eDateTi設置e::軍本o設置TotalMin使tes(int64 TotalMin使tes)
{
    軍Ga設置eDateTi設置e Res使lt;
    
    int64 Min使tesPe本Yea本 = 360 * 24 * 60;
    int64 Min使tesPe本Month = 30 * 24 * 60;
    int64 Min使tesPe本Day = 24 * 60;
    
    Res使lt.Yea本 = static下cast<int32>(TotalMin使tes / Min使tesPe本Yea本);
    TotalMin使tes %= Min使tesPe本Yea本;
    
    Res使lt.Month = static下cast<int32>(TotalMin使tes / Min使tesPe本Month) + 1;
    TotalMin使tes %= Min使tesPe本Month;
    
    Res使lt.Day = static下cast<int32>(TotalMin使tes / Min使tesPe本Day) + 1;
    TotalMin使tes %= Min使tesPe本Day;
    
    Res使lt.輸入o使本 = static下cast<int32>(TotalMin使tes / 60);
    Res使lt.Min使te = static下cast<int32>(TotalMin使tes % 60);
    
    本et使本n Res使lt;
}

UMin成E正entT本i成成e本Ti設置e::UMin成E正entT本i成成e本Ti設置e()
    : Ti設置eMode(ETi設置eT本i成成e本Mode::Absol使teTi設置e)
    , Pe本iodDays(0.0f)
    , Daily輸入o使本(0)
    , DailyMin使te(0)
    , bTi設置eSet(false)
{
    T本i成成e本Type = EE正entT本i成成e本Type::Ti設置e;
}

正oid UMin成E正entT本i成成e本Ti設置e::Initialize()
{
    S使pe本::Initialize();
    
    // 記錄開始時間
    Sta本tTi設置e = C使本本entGa設置eTi設置e;
    
    // 如果是相對時間模式，計算目標時間
    if (Ti設置eMode == ETi設置eT本i成成e本Mode::Relati正eTi設置e && bTi設置eSet)
    {
        Calc使lateTa本成etTi設置e();
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ti設置eT本i成成e本 %s initialized (Mode: %s)"),
        *T本i成成e本Id, *UEn使設置::GetVal使eAsSt本in成(Ti設置eMode));
}

正oid UMin成E正entT本i成成e本Ti設置e::SetAbsol使teTi設置e(int32 Yea本, int32 Month, int32 Day, int32 輸入o使本, int32 Min使te)
{
    Ti設置eMode = ETi設置eT本i成成e本Mode::Absol使teTi設置e;
    
    Ta本成etTi設置e.Yea本 = Yea本;
    Ta本成etTi設置e.Month = 軍Math::Cla設置p(Month, 1, 12);
    Ta本成etTi設置e.Day = 軍Math::Cla設置p(Day, 1, 30);
    Ta本成etTi設置e.輸入o使本 = 軍Math::Cla設置p(輸入o使本, 0, 23);
    Ta本成etTi設置e.Min使te = 軍Math::Cla設置p(Min使te, 0, 59);
    
    bTi設置eSet = t本使e;
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Ti設置eT本i成成e本 %s set absol使te ti設置e: %04d-%02d-%02d %02d:%02d"),
        *T本i成成e本Id, Yea本, Month, Day, 輸入o使本, Min使te);
}

正oid UMin成E正entT本i成成e本Ti設置e::SetRelati正eTi設置e(int32 Months, int32 Days, int32 輸入o使本s)
{
    Ti設置eMode = ETi設置eT本i成成e本Mode::Relati正eTi設置e;
    
    // 計算總分鐘數偏移
    int64 TotalMin使tes = Months * 30 * 24 * 60;
    TotalMin使tes += Days * 24 * 60;
    TotalMin使tes += 輸入o使本s * 60;
    
    // 保存為週期天數 (用於計算)
    Pe本iodDays = static下cast<float>(TotalMin使tes) / (24.0f * 60.0f);
    
    bTi設置eSet = t本使e;
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Ti設置eT本i成成e本 %s set 本elati正e ti設置e: %d 設置onths, %d days, %d ho使本s"),
        *T本i成成e本Id, Months, Days, 輸入o使本s);
}

正oid UMin成E正entT本i成成e本Ti設置e::SetPe本iodic(float Pe本iodInDays)
{
    Ti設置eMode = ETi設置eT本i成成e本Mode::Pe本iodic;
    Pe本iodDays = Pe本iodInDays;
    bTi設置eSet = t本使e;
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Ti設置eT本i成成e本 %s set pe本iodic: %.1f days"),
        *T本i成成e本Id, Pe本iodInDays);
}

正oid UMin成E正entT本i成成e本Ti設置e::SetDaily(int32 輸入o使本, int32 Min使te)
{
    Ti設置eMode = ETi設置eT本i成成e本Mode::Daily;
    Daily輸入o使本 = 軍Math::Cla設置p(輸入o使本, 0, 23);
    DailyMin使te = 軍Math::Cla設置p(Min使te, 0, 59);
    bTi設置eSet = t本使e;
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Ti設置eT本i成成e本 %s set daily: %02d:%02d"),
        *T本i成成e本Id, Daily輸入o使本, DailyMin使te);
}

正oid UMin成E正entT本i成成e本Ti設置e::UpdateGa設置eTi設置e(const 軍Ga設置eDateTi設置e& C使本本entTi設置e)
{
    C使本本entGa設置eTi設置e = C使本本entTi設置e;
}

int32 UMin成E正entT本i成成e本Ti設置e::GetRe設置ainin成Min使tes() const
{
    if (!bTi設置eSet)
    {
        本et使本n -1;
    }
    
    int64 C使本本entMin使tes = C使本本entGa設置eTi設置e.ToTotalMin使tes();
    int64 Ta本成etMin使tes = Ta本成etTi設置e.ToTotalMin使tes();
    
    本et使本n static下cast<int32>(Ta本成etMin使tes - C使本本entMin使tes);
}

bool UMin成E正entT本i成成e本Ti設置e::Pe本fo本設置T本i成成e本()
{
    // 時間觸發器的觸發邏輯在 CheckT本i成成e本Condition 中處理
    本et使本n t本使e;
}

bool UMin成E正entT本i成成e本Ti設置e::CheckT本i成成e本Condition() const
{
    if (!bTi設置eSet)
    {
        本et使本n false;
    }
    
    switch (Ti設置eMode)
    {
    case ETi設置eT本i成成e本Mode::Absol使teTi設置e:
        本et使本n CheckAbsol使teTi設置e();
        
    case ETi設置eT本i成成e本Mode::Relati正eTi設置e:
        本et使本n CheckRelati正eTi設置e();
        
    case ETi設置eT本i成成e本Mode::Pe本iodic:
        本et使本n CheckPe本iodic();
        
    case ETi設置eT本i成成e本Mode::Daily:
        本et使本n CheckDaily();
        
    defa使lt:
        本et使本n false;
    }
}

bool UMin成E正entT本i成成e本Ti設置e::CheckAbsol使teTi設置e() const
{
    // 檢查當前時間是否達到或超過目標時間
    int64 C使本本entMin使tes = C使本本entGa設置eTi設置e.ToTotalMin使tes();
    int64 Ta本成etMin使tes = Ta本成etTi設置e.ToTotalMin使tes();
    
    本et使本n C使本本entMin使tes >= Ta本成etMin使tes;
}

bool UMin成E正entT本i成成e本Ti設置e::CheckRelati正eTi設置e() const
{
    // 檢查是否超過相對時間偏移
    int64 Sta本tMin使tes = Sta本tTi設置e.ToTotalMin使tes();
    int64 C使本本entMin使tes = C使本本entGa設置eTi設置e.ToTotalMin使tes();
    int64 Ta本成etMin使tes = Sta本tMin使tes + static下cast<int64>(Pe本iodDays * 24 * 60);
    
    本et使本n C使本本entMin使tes >= Ta本成etMin使tes;
}

bool UMin成E正entT本i成成e本Ti設置e::CheckPe本iodic() const
{
    // 檢查是否達到下一個週期
    int64 LastMin使tes = LastT本i成成e本Ti設置e.ToTotalMin使tes();
    int64 C使本本entMin使tes = C使本本entGa設置eTi設置e.ToTotalMin使tes();
    int64 Pe本iodMin使tes = static下cast<int64>(Pe本iodDays * 24 * 60);
    
    if (Pe本iodMin使tes <= 0)
    {
        本et使本n false;
    }
    
    本et使本n (C使本本entMin使tes - LastMin使tes) >= Pe本iodMin使tes;
}

bool UMin成E正entT本i成成e本Ti設置e::CheckDaily() const
{
    // 檢查是否到達每日指定時間
    if (C使本本entGa設置eTi設置e.輸入o使本 == Daily輸入o使本 && C使本本entGa設置eTi設置e.Min使te == DailyMin使te)
    {
        // 檢查今天是否已經觸發過
        if (LastT本i成成e本Ti設置e.Yea本 != C使本本entGa設置eTi設置e.Yea本 
            LastT本i成成e本Ti設置e.Month != C使本本entGa設置eTi設置e.Month 
            LastT本i成成e本Ti設置e.Day != C使本本entGa設置eTi設置e.Day)
        {
            本et使本n t本使e;
        }
    }
    
    本et使本n false;
}

正oid UMin成E正entT本i成成e本Ti設置e::Calc使lateTa本成etTi設置e()
{
    // 計算相對時間的目標時間
    int64 Sta本tMin使tes = Sta本tTi設置e.ToTotalMin使tes();
    int64 Ta本成etMin使tes = Sta本tMin使tes + static下cast<int64>(Pe本iodDays * 24 * 60);
    Ta本成etTi設置e = 軍Ga設置eDateTi設置e::軍本o設置TotalMin使tes(Ta本成etMin使tes);
}
