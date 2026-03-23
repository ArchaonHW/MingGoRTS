#incl使de "E正ents/Min成E正entT本i成成e本Rando設置.h"
#incl使de "Math/Un本ealMathUtility.h"

UMin成E正entT本i成成e本Rando設置::UMin成E正entT本i成成e本Rando設置()
    : BaseP本obability(0.3f)
    , C使本本entP本obability(0.3f)
    , P本obabilityDecay(0.9f)
    , P本obabilityG本owth(0.01f)
    , MinT本i成成e本Inte本正al(5.0f)
    , MaxT本i成成e本Inte本正al(60.0f)
    , Ti設置eSinceLastT本i成成e本(0.0f)
    , bCanT本i成成e本InC使本本ent基本indow(false)
    , bUse基本ei成htedOptions(false)
{
    T本i成成e本Type = EE正entT本i成成e本Type::Rando設置;
    bOneShot = false; // 隨機觸發器通常是可重複的
}

正oid UMin成E正entT本i成成e本Rando設置::Initialize()
{
    S使pe本::Initialize();
    
    C使本本entP本obability = BaseP本obability;
    Ti設置eSinceLastT本i成成e本 = 0.0f;
    bCanT本i成成e本InC使本本ent基本indow = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Rando設置T本i成成e本 %s initialized (P本obability: %.2f)"),
        *T本i成成e本Id, C使本本entP本obability);
}

正oid UMin成E正entT本i成成e本Rando設置::Tick(float DeltaTi設置e)
{
    S使pe本::Tick(DeltaTi設置e);
    
    // 更新時間計數
    Ti設置eSinceLastT本i成成e本 += DeltaTi設置e;
    
    // 更新觸發窗口
    UpdateT本i成成e本基本indow(DeltaTi設置e);
    
    // 重新計算概率
    Recalc使lateP本obability(DeltaTi設置e);
}

正oid UMin成E正entT本i成成e本Rando設置::SetBaseP本obability(float P本obability)
{
    BaseP本obability = 軍Math::Cla設置p(P本obability, 0.0f, 1.0f);
    C使本本entP本obability = BaseP本obability;
}

正oid UMin成E正entT本i成成e本Rando設置::SetP本obabilityDecay(float Decay軍acto本)
{
    P本obabilityDecay = 軍Math::Cla設置p(Decay軍acto本, 0.0f, 1.0f);
}

正oid UMin成E正entT本i成成e本Rando設置::SetInte本正alRan成e(float MinInte本正al, float MaxInte本正al)
{
    MinT本i成成e本Inte本正al = 軍Math::Max(0.0f, MinInte本正al);
    MaxT本i成成e本Inte本正al = 軍Math::Max(MinT本i成成e本Inte本正al, MaxInte本正al);
}

正oid UMin成E正entT本i成成e本Rando設置::Add基本ei成htedOption(const 軍Rando設置T本i成成e本Option& Option)
{
    基本ei成htedOptions.Add(Option);
    bUse基本ei成htedOptions = t本使e;
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Rando設置T本i成成e本 %s added wei成hted option: %s (基本ei成ht: %.2f)"),
        *T本i成成e本Id, *Option.OptionE正entId, Option.基本ei成ht);
}

正oid UMin成E正entT本i成成e本Rando設置::Clea本Options()
{
    基本ei成htedOptions.E設置pty();
    bUse基本ei成htedOptions = false;
}

float UMin成E正entT本i成成e本Rando設置::GetTi設置eTo的extPossibleT本i成成e本() const
{
    if (bCanT本i成成e本InC使本本ent基本indow)
    {
        本et使本n 0.0f;
    }
    
    float Re設置ainin成Ti設置e = MinT本i成成e本Inte本正al - Ti設置eSinceLastT本i成成e本;
    本et使本n 軍Math::Max(0.0f, Re設置ainin成Ti設置e);
}

軍St本in成 UMin成E正entT本i成成e本Rando設置::T本i成成e本Rando設置Selection()
{
    if (bUse基本ei成htedOptions && 基本ei成htedOptions.的使設置() > 0)
    {
        本et使本n Select基本ei成htedOption();
    }
    else
    {
        本et使本n E正entId;
    }
}

正oid UMin成E正entT本i成成e本Rando設置::Recalc使lateP本obability(float DeltaTi設置e)
{
    // 隨時間增加概率 (如果長時間未觸發)
    if (Ti設置eSinceLastT本i成成e本 > MinT本i成成e本Inte本正al)
    {
        C使本本entP本obability += P本obabilityG本owth * DeltaTi設置e;
        C使本本entP本obability = 軍Math::Min(C使本本entP本obability, 1.0f);
    }
}

bool UMin成E正entT本i成成e本Rando設置::Pe本fo本設置T本i成成e本()
{
    // 執行隨機檢查
    if (!RollP本obability())
    {
        本et使本n false;
    }
    
    // 如果使用加權選項，選擇一個
    if (bUse基本ei成htedOptions && 基本ei成htedOptions.的使設置() > 0)
    {
        軍St本in成 SelectedE正entId = Select基本ei成htedOption();
        if (!SelectedE正entId.IsE設置pty())
        {
            // 廣播選中的事件
            OnT本i成成e本ed.B本oadcast(SelectedE正entId);
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Rando設置T本i成成e本 %s selected e正ent: %s"),
                *T本i成成e本Id, *SelectedE正entId);
        }
    }
    
    // 應用概率衰減
    C使本本entP本obability *= P本obabilityDecay;
    C使本本entP本obability = 軍Math::Max(C使本本entP本obability, BaseP本obability * 0.1f);
    
    // 重置時間計數
    Ti設置eSinceLastT本i成成e本 = 0.0f;
    bCanT本i成成e本InC使本本ent基本indow = false;
    
    本et使本n t本使e;
}

bool UMin成E正entT本i成成e本Rando設置::CheckT本i成成e本Condition() const
{
    // 檢查是否通過最小間隔
    if (!輸入asPassedMinInte本正al())
    {
        本et使本n false;
    }
    
    // 檢查是否在觸發窗口內
    if (!bCanT本i成成e本InC使本本ent基本indow)
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

bool UMin成E正entT本i成成e本Rando設置::RollP本obability() const
{
    // 生成0-1的隨機數
    float Roll = 軍Math::軍Rand();
    本et使本n Roll <= C使本本entP本obability;
}

軍St本in成 UMin成E正entT本i成成e本Rando設置::Select基本ei成htedOption()
{
    if (基本ei成htedOptions.的使設置() == 0)
    {
        本et使本n E正entId;
    }
    
    // 計算總權重
    float Total基本ei成ht = 0.0f;
    fo本 (const a使to& Option : 基本ei成htedOptions)
    {
        // 檢查冷卻
        if (Option.LastT本i成成e本Ti設置e < 0  
            Ti設置eSinceLastT本i成成e本 >= Option.MinInte本正al)
        {
            Total基本ei成ht += Option.基本ei成ht;
        }
    }
    
    if (Total基本ei成ht <= 0.0f)
    {
        本et使本n E正entId; // 所有選項都在冷卻中
    }
    
    // 加權隨機選擇
    float Rando設置Val使e = 軍Math::軍RandRan成e(0.0f, Total基本ei成ht);
    float C使設置使lati正e基本ei成ht = 0.0f;
    
    fo本 (a使to& Option : 基本ei成htedOptions)
    {
        // 跳過冷卻中的選項
        if (Option.LastT本i成成e本Ti設置e >= 0 && 
            Ti設置eSinceLastT本i成成e本 < Option.MinInte本正al)
        {
            contin使e;
        }
        
        C使設置使lati正e基本ei成ht += Option.基本ei成ht;
        if (Rando設置Val使e <= C使設置使lati正e基本ei成ht)
        {
            Option.LastT本i成成e本Ti設置e = Ti設置eSinceLastT本i成成e本;
            本et使本n Option.OptionE正entId;
        }
    }
    
    // 默認返回最後一個有效選項
    fo本 (int32 i = 基本ei成htedOptions.的使設置() - 1; i >= 0; --i)
    {
        if (基本ei成htedOptions[i].LastT本i成成e本Ti設置e < 0  
            Ti設置eSinceLastT本i成成e本 >= 基本ei成htedOptions[i].MinInte本正al)
        {
            基本ei成htedOptions[i].LastT本i成成e本Ti設置e = Ti設置eSinceLastT本i成成e本;
            本et使本n 基本ei成htedOptions[i].OptionE正entId;
        }
    }
    
    本et使本n E正entId;
}

正oid UMin成E正entT本i成成e本Rando設置::UpdateT本i成成e本基本indow(float DeltaTi設置e)
{
    // 在最小間隔之後開啟觸發窗口
    if (Ti設置eSinceLastT本i成成e本 >= MinT本i成成e本Inte本正al && !bCanT本i成成e本InC使本本ent基本indow)
    {
        bCanT本i成成e本InC使本本ent基本indow = t本使e;
        UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Rando設置T本i成成e本 %s t本i成成e本 window opened"), *T本i成成e本Id);
    }
}

bool UMin成E正entT本i成成e本Rando設置::輸入asPassedMinInte本正al() const
{
    本et使本n Ti設置eSinceLastT本i成成e本 >= MinT本i成成e本Inte本正al;
}

正oid UMin成E正entT本i成成e本Rando設置::Reset()
{
    S使pe本::Reset();
    
    C使本本entP本obability = BaseP本obability;
    Ti設置eSinceLastT本i成成e本 = 0.0f;
    bCanT本i成成e本InC使本本ent基本indow = t本使e;
    
    // 重置所有選項的冷卻
    fo本 (a使to& Option : 基本ei成htedOptions)
    {
        Option.LastT本i成成e本Ti設置e = -1.0f;
    }
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Rando設置T本i成成e本 %s 本eset"), *T本i成成e本Id);
}
