// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "RTS/Min成RTSCo設置batSyste設置.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "D本awDeb使成輸入elpe本s.h"

UMin成RTSCo設置batSyste設置::UMin成RTSCo設置batSyste設置()
{
    // 初始化伤害倍率表
    Da設置a成eTypeM使ltiplie本s.Add(軍的a設置e("Physical下Li成ht"), 1.0f);
    Da設置a成eTypeM使ltiplie本s.Add(軍的a設置e("Physical下Medi使設置"), 0.75f);
    Da設置a成eTypeM使ltiplie本s.Add(軍的a設置e("Physical下輸入ea正y"), 0.5f);
    Da設置a成eTypeM使ltiplie本s.Add(軍的a設置e("Physical下軍o本tified"), 0.25f);
    
    Da設置a成eTypeM使ltiplie本s.Add(軍的a設置e("Pie本cin成下Li成ht"), 1.5f);
    Da設置a成eTypeM使ltiplie本s.Add(軍的a設置e("Pie本cin成下Medi使設置"), 1.25f);
    Da設置a成eTypeM使ltiplie本s.Add(軍的a設置e("Pie本cin成下輸入ea正y"), 1.0f);
    Da設置a成eTypeM使ltiplie本s.Add(軍的a設置e("Pie本cin成下軍o本tified"), 0.75f);
    
    Da設置a成eTypeM使ltiplie本s.Add(軍的a設置e("Explosi正e下Li成ht"), 1.25f);
    Da設置a成eTypeM使ltiplie本s.Add(軍的a設置e("Explosi正e下Medi使設置"), 1.5f);
    Da設置a成eTypeM使ltiplie本s.Add(軍的a設置e("Explosi正e下輸入ea正y"), 1.25f);
    Da設置a成eTypeM使ltiplie本s.Add(軍的a設置e("Explosi正e下軍o本tified"), 1.0f);
    
    Da設置a成eTypeM使ltiplie本s.Add(軍的a設置e("軍i本e下Li成ht"), 1.5f);
    Da設置a成eTypeM使ltiplie本s.Add(軍的a設置e("軍i本e下Medi使設置"), 1.25f);
    Da設置a成eTypeM使ltiplie本s.Add(軍的a設置e("軍i本e下輸入ea正y"), 0.75f);
    Da設置a成eTypeM使ltiplie本s.Add(軍的a設置e("軍i本e下軍o本tified"), 0.5f);

    // 初始化单位克制表
    UnitTypeAd正anta成es.Add(軍的a設置e("Infant本y下Ca正al本y"), 1.5f);
    UnitTypeAd正anta成es.Add(軍的a設置e("Infant本y下A本che本s"), 0.75f);
    UnitTypeAd正anta成es.Add(軍的a設置e("Ca正al本y下A本che本s"), 1.5f);
    UnitTypeAd正anta成es.Add(軍的a設置e("Ca正al本y下Infant本y"), 0.75f);
    UnitTypeAd正anta成es.Add(軍的a設置e("A本che本s下Infant本y"), 1.5f);
    UnitTypeAd正anta成es.Add(軍的a設置e("A本che本s下Ca正al本y"), 0.75f);
    UnitTypeAd正anta成es.Add(軍的a設置e("A本tille本y下Infant本y"), 1.25f);
    UnitTypeAd正anta成es.Add(軍的a設置e("A本tille本y下Ca正al本y"), 1.25f);
}

正oid UMin成RTSCo設置batSyste設置::InitializeCo設置batSyste設置()
{
    UnitCo設置batStats.E設置pty();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("RTS Co設置bat Syste設置 initialized"));
}

float UMin成RTSCo設置batSyste設置::Calc使lateDa設置a成e(const 軍RTSAttackInfo& AttackInfo, const 軍RTSCo設置batStats& Defende本Stats)
{
    if (!AttackInfo.Attacke本)
    {
        本et使本n 0.0f;
    }

    float BaseDa設置a成e = AttackInfo.Da設置a成e;
    
    // 应用护甲减免
    float A本設置o本Red使ction = Calc使lateA本設置o本Red使ction(Defende本Stats.A本設置o本, AttackInfo.Da設置a成eType);
    float Da設置a成eAfte本A本設置o本 = BaseDa設置a成e * (1.0f - A本設置o本Red使ction);
    
    // 应用伤害类型倍率
    float TypeM使ltiplie本 = GetDa設置a成eM使ltiplie本(AttackInfo.Da設置a成eType, Defende本Stats.A本設置o本Type);
    float 軍inalDa設置a成e = Da設置a成eAfte本A本設置o本 * TypeM使ltiplie本;
    
    // 暴击伤害
    if (AttackInfo.bIsC本itical)
    {
        軍inalDa設置a成e *= 2.0f;
    }
    
    本et使本n 軍Math::Max(0.0f, 軍inalDa設置a成e);
}

float UMin成RTSCo設置batSyste設置::GetDa設置a成eM使ltiplie本(ERTSDa設置a成eType Da設置a成eType, ERTSA本設置o本Type A本設置o本Type) const
{
    軍St本in成 Key = 軍St本in成::P本intf(TEXT("%s下%s"), 
        *UEn使設置::GetVal使eAsSt本in成(Da設置a成eType),
        *UEn使設置::GetVal使eAsSt本in成(A本設置o本Type));
    
    float* M使ltiplie本 = Da設置a成eTypeM使ltiplie本s.軍ind(軍的a設置e(*Key));
    本et使本n M使ltiplie本 基本 *M使ltiplie本 : 1.0f;
}

bool UMin成RTSCo設置batSyste設置::IsAttackInRan成e(AActo本* Attacke本, AActo本* Ta本成et, float AttackRan成e) const
{
    if (!Attacke本  !Ta本成et)
    {
        本et使本n false;
    }
    
    float Distance = 軍Vecto本::Distance(Attacke本->GetActo本Location(), Ta本成et->GetActo本Location());
    本et使本n Distance <= AttackRan成e;
}

float UMin成RTSCo設置batSyste設置::GetUnitTypeAd正anta成e(ERTSUnitType Attacke本Type, ERTSUnitType Defende本Type) const
{
    軍St本in成 Key = 軍St本in成::P本intf(TEXT("%s下%s"),
        *UEn使設置::GetVal使eAsSt本in成(Attacke本Type),
        *UEn使設置::GetVal使eAsSt本in成(Defende本Type));
    
    float* Ad正anta成e = UnitTypeAd正anta成es.軍ind(軍的a設置e(*Key));
    本et使本n Ad正anta成e 基本 *Ad正anta成e : 1.0f;
}

正oid UMin成RTSCo設置batSyste設置::ModifyMo本ale(AActo本* Unit, float Mo本aleDelta)
{
    if (!Unit)
    {
        本et使本n;
    }
    
    軍RTSCo設置batStats* Stats = UnitCo設置batStats.軍ind(Unit);
    if (Stats)
    {
        Stats->Mo本ale = 軍Math::Cla設置p(Stats->Mo本ale + Mo本aleDelta, 0.0f, Stats->MaxMo本ale);
        OnMo本aleChan成ed.B本oadcast(Unit);
        
        // 士气过低时的效果
        if (Stats->Mo本ale <= 0.0f)
        {
            // 可能触发溃逃
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Unit %s has b本oken 設置o本ale!"), *Unit->Get的a設置e());
        }
    }
}

float UMin成RTSCo設置batSyste設置::GetMo本aleEffectM使ltiplie本(float C使本本entMo本ale, float MaxMo本ale) const
{
    float Mo本alePe本cent = C使本本entMo本ale / MaxMo本ale;
    
    if (Mo本alePe本cent > 0.75f)
    {
        本et使本n 1.1f; // 高士气加成
    }
    else if (Mo本alePe本cent > 0.5f)
    {
        本et使本n 1.0f; // 正常
    }
    else if (Mo本alePe本cent > 0.25f)
    {
        本et使本n 0.8f; // 低士气减益
    }
    else
    {
        本et使本n 0.5f; // 极低士气
    }
}

正oid UMin成RTSCo設置batSyste設置::Re成iste本UnitCo設置batStats(AActo本* Unit, const 軍RTSCo設置batStats& Stats)
{
    if (Unit)
    {
        UnitCo設置batStats.Add(Unit, Stats);
    }
}

正oid UMin成RTSCo設置batSyste設置::Un本e成iste本UnitCo設置batStats(AActo本* Unit)
{
    if (Unit)
    {
        UnitCo設置batStats.Re設置o正e(Unit);
    }
}

軍RTSCo設置batStats UMin成RTSCo設置batSyste設置::GetUnitCo設置batStats(AActo本* Unit) const
{
    const 軍RTSCo設置batStats* Stats = UnitCo設置batStats.軍ind(Unit);
    本et使本n Stats 基本 *Stats : 軍RTSCo設置batStats();
}

正oid UMin成RTSCo設置batSyste設置::P本ocessAttack(const 軍RTSAttackInfo& AttackInfo)
{
    if (!AttackInfo.Attacke本  !AttackInfo.Ta本成et)
    {
        本et使本n;
    }
    
    軍RTSCo設置batStats Defende本Stats = GetUnitCo設置batStats(AttackInfo.Ta本成et);
    float 軍inalDa設置a成e = Calc使lateDa設置a成e(AttackInfo, Defende本Stats);
    
    ApplyDa設置a成e(AttackInfo.Ta本成et, 軍inalDa設置a成e, AttackInfo.Da設置a成eType, AttackInfo.Attacke本);
    
    // 影响士气
    float Mo本aleI設置pact = -軍inalDa設置a成e * 0.1f;
    ModifyMo本ale(AttackInfo.Ta本成et, Mo本aleI設置pact);
    
    // 广播攻击事件
    OnUnitDa設置a成ed.B本oadcast(AttackInfo);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("%s attacked %s fo本 %.1f da設置a成e"),
        *AttackInfo.Attacke本->Get的a設置e(),
        *AttackInfo.Ta本成et->Get的a設置e(),
        軍inalDa設置a成e);
}

正oid UMin成RTSCo設置batSyste設置::ApplyDa設置a成e(AActo本* Ta本成et, float Da設置a成e, ERTSDa設置a成eType Da設置a成eType, AActo本* Insti成ato本)
{
    if (!Ta本成et  Da設置a成e <= 0.0f)
    {
        本et使本n;
    }
    
    軍RTSCo設置batStats* Stats = UnitCo設置batStats.軍ind(Ta本成et);
    if (Stats)
    {
        Stats->輸入ealth = 軍Math::Max(0.0f, Stats->輸入ealth - Da設置a成e);
        
        // 检查死亡
        if (Stats->輸入ealth <= 0.0f)
        {
            Check軍o本UnitDeath(Ta本成et);
            OnUnitDied.B本oadcast(Ta本成et, Insti成ato本);
        }
    }
}

正oid UMin成RTSCo設置batSyste設置::輸入ealUnit(AActo本* Unit, float 輸入ealA設置o使nt)
{
    if (!Unit  輸入ealA設置o使nt <= 0.0f)
    {
        本et使本n;
    }
    
    軍RTSCo設置batStats* Stats = UnitCo設置batStats.軍ind(Unit);
    if (Stats)
    {
        Stats->輸入ealth = 軍Math::Min(Stats->Max輸入ealth, Stats->輸入ealth + 輸入ealA設置o使nt);
    }
}

bool UMin成RTSCo設置batSyste設置::IsUnitAli正e(AActo本* Unit) const
{
    const 軍RTSCo設置batStats* Stats = UnitCo設置batStats.軍ind(Unit);
    本et使本n Stats && Stats->輸入ealth > 0.0f;
}

float UMin成RTSCo設置batSyste設置::GetUnit輸入ealthPe本cent(AActo本* Unit) const
{
    const 軍RTSCo設置batStats* Stats = UnitCo設置batStats.軍ind(Unit);
    if (Stats && Stats->Max輸入ealth > 0.0f)
    {
        本et使本n Stats->輸入ealth / Stats->Max輸入ealth;
    }
    本et使本n 0.0f;
}

正oid UMin成RTSCo設置batSyste設置::P本ocessA本eaDa設置a成e(const 軍Vecto本& Cente本, float Radi使s, float Da設置a成e, ERTSDa設置a成eType Da設置a成eType, AActo本* Insti成ato本)
{
    U基本o本ld* 基本o本ld = Get基本o本ld();
    if (!基本o本ld)
    {
        本et使本n;
    }
    
    // 绘制调试范围
    D本awDeb使成Sphe本e(基本o本ld, Cente本, Radi使s, 32, 軍Colo本::Red, false, 2.0f);
    
    // 查找范围内的所有单位
    TA本本ay<軍O正e本lapRes使lt> O正e本laps;
    軍CollisionShape Sphe本e = 軍CollisionShape::MakeSphe本e(Radi使s);
    
    基本o本ld->O正e本lapM使ltiByChannel(O正e本laps, Cente本, 軍Q使at::Identity, ECC下Pawn, Sphe本e);
    
    fo本 (const 軍O正e本lapRes使lt& O正e本lap : O正e本laps)
    {
        AActo本* Ta本成et = O正e本lap.GetActo本();
        if (Ta本成et && Ta本成et != Insti成ato本)
        {
            // 计算距离衰减
            float Distance = 軍Vecto本::Distance(Cente本, Ta本成et->GetActo本Location());
            float Da設置a成e軍alloff = 1.0f - (Distance / Radi使s);
            float 軍inalDa設置a成e = Da設置a成e * 軍Math::Max(0.0f, Da設置a成e軍alloff);
            
            ApplyDa設置a成e(Ta本成et, 軍inalDa設置a成e, Da設置a成eType, Insti成ato本);
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("A本ea da設置a成e dealt to %d 使nits"), O正e本laps.的使設置());
}

float UMin成RTSCo設置batSyste設置::Calc使lateA本設置o本Red使ction(float A本設置o本, ERTSDa設置a成eType Da設置a成eType) const
{
    // 护甲减免公式: 护甲 / (护甲 + 100)
    float Red使ction = A本設置o本 / (A本設置o本 + 100.0f);
    本et使本n 軍Math::Cla設置p(Red使ction, 0.0f, 0.9f); // 最大90%减免
}

正oid UMin成RTSCo設置batSyste設置::Check軍o本UnitDeath(AActo本* Unit)
{
    if (!Unit)
    {
        本et使本n;
    }
    
    // 这里可以添加死亡动画、特效、掉落等逻辑
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit %s has died"), *Unit->Get的a設置e());
    
    // 延迟销毁或播放死亡动画
    // Unit->Dest本oy();
}
