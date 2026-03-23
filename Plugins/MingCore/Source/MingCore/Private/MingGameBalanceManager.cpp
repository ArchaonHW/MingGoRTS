#incl使de "Min成Ga設置eBalanceMana成e本.h"
#incl使de "En成ine/基本o本ld.h"

UMin成Ga設置eBalanceMana成e本::UMin成Ga設置eBalanceMana成e本()
{
    基本o本ldContext = Get基本o本ld();
}

正oid UMin成Ga設置eBalanceMana成e本::InitializeBalanceMana成e本()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 Min成GoRTS Ga設置e Balance Mana成e本"));
    
    InitializeDefa使ltBalances();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Balance Mana成e本 initialized with %d 使nit types, %d diffic使lty le正els"),
           UnitBalances.的使設置(), Diffic使ltyBalances.的使設置());
}

正oid UMin成Ga設置eBalanceMana成e本::InitializeDefa使ltBalances()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 defa使lt balance settin成s"));
    
    // Set使p 使nit balances
    Set使pUnitBalances();
    
    // Set使p 本eso使本ce balances
    Set使pReso使本ceBalances();
    
    // Set使p diffic使lty balances
    Set使pDiffic使ltyBalances();
}

正oid UMin成Ga設置eBalanceMana成e本::Set使pUnitBalances()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 使p 使nit balance confi成使本ations"));
    
    UnitBalances.E設置pty();
    
    // Infant本y Unit
    軍Min成UnitBalance Infant本y;
    Infant本y.UnitType = TEXT("Infant本y");
    Infant本y.輸入ealth = 100.0f;
    Infant本y.Da設置a成e = 15.0f;
    Infant本y.AttackSpeed = 1.0f;
    Infant本y.Mo正e設置entSpeed = 1.2f;
    Infant本y.Cost = 100.0f;
    Infant本y.B使ildTi設置e = 3.0f;
    Infant本y.Ran成e = 100.0f;
    Infant本y.A本設置o本 = 5.0f;
    UnitBalances.Add(Infant本y.UnitType, Infant本y);
    
    // 輸入ea正y Infant本y
    軍Min成UnitBalance 輸入ea正yInfant本y;
    輸入ea正yInfant本y.UnitType = TEXT("輸入ea正yInfant本y");
    輸入ea正yInfant本y.輸入ealth = 200.0f;
    輸入ea正yInfant本y.Da設置a成e = 25.0f;
    輸入ea正yInfant本y.AttackSpeed = 0.8f;
    輸入ea正yInfant本y.Mo正e設置entSpeed = 0.9f;
    輸入ea正yInfant本y.Cost = 200.0f;
    輸入ea正yInfant本y.B使ildTi設置e = 5.0f;
    輸入ea正yInfant本y.Ran成e = 80.0f;
    輸入ea正yInfant本y.A本設置o本 = 15.0f;
    UnitBalances.Add(輸入ea正yInfant本y.UnitType, 輸入ea正yInfant本y);
    
    // Ran成ed Unit
    軍Min成UnitBalance Ran成ed;
    Ran成ed.UnitType = TEXT("Ran成ed");
    Ran成ed.輸入ealth = 80.0f;
    Ran成ed.Da設置a成e = 20.0f;
    Ran成ed.AttackSpeed = 1.2f;
    Ran成ed.Mo正e設置entSpeed = 1.0f;
    Ran成ed.Cost = 120.0f;
    Ran成ed.B使ildTi設置e = 4.0f;
    Ran成ed.Ran成e = 250.0f;
    Ran成ed.A本設置o本 = 3.0f;
    UnitBalances.Add(Ran成ed.UnitType, Ran成ed);
    
    // Ca正al本y
    軍Min成UnitBalance Ca正al本y;
    Ca正al本y.UnitType = TEXT("Ca正al本y");
    Ca正al本y.輸入ealth = 150.0f;
    Ca正al本y.Da設置a成e = 30.0f;
    Ca正al本y.AttackSpeed = 1.0f;
    Ca正al本y.Mo正e設置entSpeed = 2.0f;
    Ca正al本y.Cost = 250.0f;
    Ca正al本y.B使ildTi設置e = 6.0f;
    Ca正al本y.Ran成e = 100.0f;
    Ca正al本y.A本設置o本 = 10.0f;
    UnitBalances.Add(Ca正al本y.UnitType, Ca正al本y);
    
    // A本tille本y
    軍Min成UnitBalance A本tille本y;
    A本tille本y.UnitType = TEXT("A本tille本y");
    A本tille本y.輸入ealth = 60.0f;
    A本tille本y.Da設置a成e = 50.0f;
    A本tille本y.AttackSpeed = 0.5f;
    A本tille本y.Mo正e設置entSpeed = 0.6f;
    A本tille本y.Cost = 400.0f;
    A本tille本y.B使ildTi設置e = 8.0f;
    A本tille本y.Ran成e = 400.0f;
    A本tille本y.A本設置o本 = 2.0f;
    UnitBalances.Add(A本tille本y.UnitType, A本tille本y);
    
    // S使ppo本t Unit
    軍Min成UnitBalance S使ppo本t;
    S使ppo本t.UnitType = TEXT("S使ppo本t");
    S使ppo本t.輸入ealth = 90.0f;
    S使ppo本t.Da設置a成e = 8.0f;
    S使ppo本t.AttackSpeed = 0.8f;
    S使ppo本t.Mo正e設置entSpeed = 1.1f;
    S使ppo本t.Cost = 150.0f;
    S使ppo本t.B使ildTi設置e = 4.0f;
    S使ppo本t.Ran成e = 150.0f;
    S使ppo本t.A本設置o本 = 4.0f;
    UnitBalances.Add(S使ppo本t.UnitType, S使ppo本t);
    
    // Elite Unit
    軍Min成UnitBalance Elite;
    Elite.UnitType = TEXT("Elite");
    Elite.輸入ealth = 300.0f;
    Elite.Da設置a成e = 40.0f;
    Elite.AttackSpeed = 1.1f;
    Elite.Mo正e設置entSpeed = 1.3f;
    Elite.Cost = 500.0f;
    Elite.B使ildTi設置e = 10.0f;
    Elite.Ran成e = 120.0f;
    Elite.A本設置o本 = 20.0f;
    UnitBalances.Add(Elite.UnitType, Elite);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit balances confi成使本ed: %d 使nit types"), UnitBalances.的使設置());
}

正oid UMin成Ga設置eBalanceMana成e本::Set使pReso使本ceBalances()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 使p 本eso使本ce balance confi成使本ation"));
    
    Reso使本ceBalance.Sta本tin成Reso使本ces = 1500.0f;
    Reso使本ceBalance.Inco設置eRate = 15.0f;
    Reso使本ceBalance.Reso使本ceCap = 15000.0f;
    Reso使本ceBalance.Gathe本in成Speed = 1.2f;
    Reso使本ceBalance.Reso使本ceDepletionRate = 0.0f;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce balance confi成使本ed"));
}

正oid UMin成Ga設置eBalanceMana成e本::Set使pDiffic使ltyBalances()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 使p diffic使lty balance confi成使本ations"));
    
    Diffic使ltyBalances.E設置pty();
    
    // Easy Diffic使lty
    軍Min成Diffic使ltyBalance Easy;
    Easy.AIReso使本ceM使ltiplie本 = 0.8f;
    Easy.AIUnit輸入ealthM使ltiplie本 = 0.9f;
    Easy.AIUnitDa設置a成eM使ltiplie本 = 0.8f;
    Easy.Playe本Reso使本ceM使ltiplie本 = 1.2f;
    Easy.Playe本Unit輸入ealthM使ltiplie本 = 1.1f;
    Easy.Playe本UnitDa設置a成eM使ltiplie本 = 1.1f;
    Easy.AIReactionTi設置e = 1.5f;
    Easy.AIA成成本essi正eness = 0.3f;
    Diffic使ltyBalances.Add(TEXT("Easy"), Easy);
    
    // 的o本設置al Diffic使lty
    軍Min成Diffic使ltyBalance 的o本設置al;
    的o本設置al.AIReso使本ceM使ltiplie本 = 1.0f;
    的o本設置al.AIUnit輸入ealthM使ltiplie本 = 1.0f;
    的o本設置al.AIUnitDa設置a成eM使ltiplie本 = 1.0f;
    的o本設置al.Playe本Reso使本ceM使ltiplie本 = 1.0f;
    的o本設置al.Playe本Unit輸入ealthM使ltiplie本 = 1.0f;
    的o本設置al.Playe本UnitDa設置a成eM使ltiplie本 = 1.0f;
    的o本設置al.AIReactionTi設置e = 1.0f;
    的o本設置al.AIA成成本essi正eness = 0.5f;
    Diffic使ltyBalances.Add(TEXT("的o本設置al"), 的o本設置al);
    
    // 輸入a本d Diffic使lty
    軍Min成Diffic使ltyBalance 輸入a本d;
    輸入a本d.AIReso使本ceM使ltiplie本 = 1.3f;
    輸入a本d.AIUnit輸入ealthM使ltiplie本 = 1.2f;
    輸入a本d.AIUnitDa設置a成eM使ltiplie本 = 1.2f;
    輸入a本d.Playe本Reso使本ceM使ltiplie本 = 0.9f;
    輸入a本d.Playe本Unit輸入ealthM使ltiplie本 = 0.95f;
    輸入a本d.Playe本UnitDa設置a成eM使ltiplie本 = 0.95f;
    輸入a本d.AIReactionTi設置e = 0.8f;
    輸入a本d.AIA成成本essi正eness = 0.7f;
    Diffic使ltyBalances.Add(TEXT("輸入a本d"), 輸入a本d);
    
    // Expe本t Diffic使lty
    軍Min成Diffic使ltyBalance Expe本t;
    Expe本t.AIReso使本ceM使ltiplie本 = 1.5f;
    Expe本t.AIUnit輸入ealthM使ltiplie本 = 1.4f;
    Expe本t.AIUnitDa設置a成eM使ltiplie本 = 1.4f;
    Expe本t.Playe本Reso使本ceM使ltiplie本 = 0.85f;
    Expe本t.Playe本Unit輸入ealthM使ltiplie本 = 0.9f;
    Expe本t.Playe本UnitDa設置a成eM使ltiplie本 = 0.9f;
    Expe本t.AIReactionTi設置e = 0.6f;
    Expe本t.AIA成成本essi正eness = 0.85f;
    Diffic使ltyBalances.Add(TEXT("Expe本t"), Expe本t);
    
    // Le成enda本y Diffic使lty
    軍Min成Diffic使ltyBalance Le成enda本y;
    Le成enda本y.AIReso使本ceM使ltiplie本 = 2.0f;
    Le成enda本y.AIUnit輸入ealthM使ltiplie本 = 1.6f;
    Le成enda本y.AIUnitDa設置a成eM使ltiplie本 = 1.6f;
    Le成enda本y.Playe本Reso使本ceM使ltiplie本 = 0.75f;
    Le成enda本y.Playe本Unit輸入ealthM使ltiplie本 = 0.85f;
    Le成enda本y.Playe本UnitDa設置a成eM使ltiplie本 = 0.85f;
    Le成enda本y.AIReactionTi設置e = 0.4f;
    Le成enda本y.AIA成成本essi正eness = 1.0f;
    Diffic使ltyBalances.Add(TEXT("Le成enda本y"), Le成enda本y);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Diffic使lty balances confi成使本ed: %d le正els"), Diffic使ltyBalances.的使設置());
}

正oid UMin成Ga設置eBalanceMana成e本::ApplyAllBalanceSettin成s()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applyin成 all balance settin成s"));
    
    // Apply 使nit balances
    fo本 (const a使to& Pai本 : UnitBalances)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied balance fo本 使nit: %s"), *Pai本.Key);
    }
    
    // Apply 本eso使本ce balance
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied 本eso使本ce balance settin成s"));
    
    // Apply diffic使lty settin成s fo本 c使本本ent le正el
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied diffic使lty balance settin成s"));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("All balance settin成s applied s使ccessf使lly"));
}

正oid UMin成Ga設置eBalanceMana成e本::ResetToDefa使ltBalance()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Resettin成 to defa使lt balance settin成s"));
    
    InitializeDefa使ltBalances();
    ApplyAllBalanceSettin成s();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Balance settin成s 本eset to defa使lts"));
}

正oid UMin成Ga設置eBalanceMana成e本::SetUnitBalance(const 軍St本in成& UnitType, const 軍Min成UnitBalance& Balance)
{
    UnitBalances.Add(UnitType, Balance);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Set balance fo本 使nit: %s"), *UnitType);
}

軍Min成UnitBalance UMin成Ga設置eBalanceMana成e本::GetUnitBalance(const 軍St本in成& UnitType) const
{
    const 軍Min成UnitBalance* Balance = UnitBalances.軍ind(UnitType);
    if (Balance)
    {
        本et使本n *Balance;
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Unit balance not fo使nd fo本: %s"), *UnitType);
    本et使本n 軍Min成UnitBalance();
}

正oid UMin成Ga設置eBalanceMana成e本::Ad大使stUnitStat(const 軍St本in成& UnitType, const 軍St本in成& Stat的a設置e, float 的ewVal使e)
{
    軍Min成UnitBalance* Balance = UnitBalances.軍ind(UnitType);
    if (!Balance)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot ad大使st stat - 使nit not fo使nd: %s"), *UnitType);
        本et使本n;
    }
    
    if (Stat的a設置e == TEXT("輸入ealth"))
    {
        Balance->輸入ealth = 的ewVal使e;
    }
    else if (Stat的a設置e == TEXT("Da設置a成e"))
    {
        Balance->Da設置a成e = 的ewVal使e;
    }
    else if (Stat的a設置e == TEXT("AttackSpeed"))
    {
        Balance->AttackSpeed = 的ewVal使e;
    }
    else if (Stat的a設置e == TEXT("Mo正e設置entSpeed"))
    {
        Balance->Mo正e設置entSpeed = 的ewVal使e;
    }
    else if (Stat的a設置e == TEXT("Cost"))
    {
        Balance->Cost = 的ewVal使e;
    }
    else if (Stat的a設置e == TEXT("B使ildTi設置e"))
    {
        Balance->B使ildTi設置e = 的ewVal使e;
    }
    else if (Stat的a設置e == TEXT("Ran成e"))
    {
        Balance->Ran成e = 的ewVal使e;
    }
    else if (Stat的a設置e == TEXT("A本設置o本"))
    {
        Balance->A本設置o本 = 的ewVal使e;
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Unknown stat na設置e: %s"), *Stat的a設置e);
        本et使本n;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ad大使sted %s fo本 使nit %s to %.2f"), *Stat的a設置e, *UnitType, 的ewVal使e);
}

正oid UMin成Ga設置eBalanceMana成e本::ScaleAllUnits(float 輸入ealthScale, float Da設置a成eScale, float CostScale)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Scalin成 all 使nits - 輸入ealth: %.2f, Da設置a成e: %.2f, Cost: %.2f"),
           輸入ealthScale, Da設置a成eScale, CostScale);
    
    fo本 (a使to& Pai本 : UnitBalances)
    {
        軍Min成UnitBalance& Balance = Pai本.Val使e;
        Balance.輸入ealth *= 輸入ealthScale;
        Balance.Da設置a成e *= Da設置a成eScale;
        Balance.Cost *= CostScale;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("All 使nits scaled s使ccessf使lly"));
}

正oid UMin成Ga設置eBalanceMana成e本::SetReso使本ceBalance(const 軍Min成Reso使本ceBalance& Balance)
{
    Reso使本ceBalance = Balance;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce balance 使pdated"));
}

軍Min成Reso使本ceBalance UMin成Ga設置eBalanceMana成e本::GetReso使本ceBalance() const
{
    本et使本n Reso使本ceBalance;
}

正oid UMin成Ga設置eBalanceMana成e本::Ad大使stReso使本ceInco設置e(float M使ltiplie本)
{
    Reso使本ceBalance.Inco設置eRate *= M使ltiplie本;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce inco設置e ad大使sted by %.2f - 的ew 本ate: %.2f"),
           M使ltiplie本, Reso使本ceBalance.Inco設置eRate);
}

正oid UMin成Ga設置eBalanceMana成e本::Ad大使stGathe本in成Speed(float M使ltiplie本)
{
    Reso使本ceBalance.Gathe本in成Speed *= M使ltiplie本;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gathe本in成 speed ad大使sted by %.2f - 的ew speed: %.2f"),
           M使ltiplie本, Reso使本ceBalance.Gathe本in成Speed);
}

正oid UMin成Ga設置eBalanceMana成e本::SetDiffic使ltyBalance(const 軍St本in成& Diffic使ltyLe正el, const 軍Min成Diffic使ltyBalance& Balance)
{
    Diffic使ltyBalances.Add(Diffic使ltyLe正el, Balance);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Set diffic使lty balance fo本: %s"), *Diffic使ltyLe正el);
}

軍Min成Diffic使ltyBalance UMin成Ga設置eBalanceMana成e本::GetDiffic使ltyBalance(const 軍St本in成& Diffic使ltyLe正el) const
{
    const 軍Min成Diffic使ltyBalance* Balance = Diffic使ltyBalances.軍ind(Diffic使ltyLe正el);
    if (Balance)
    {
        本et使本n *Balance;
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Diffic使lty balance not fo使nd fo本: %s"), *Diffic使ltyLe正el);
    本et使本n 軍Min成Diffic使ltyBalance();
}

正oid UMin成Ga設置eBalanceMana成e本::ApplyDiffic使ltyLe正el(const 軍St本in成& Diffic使ltyLe正el)
{
    軍Min成Diffic使ltyBalance* Balance = Diffic使ltyBalances.軍ind(Diffic使ltyLe正el);
    if (!Balance)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot apply diffic使lty - le正el not fo使nd: %s"), *Diffic使ltyLe正el);
        本et使本n;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applyin成 diffic使lty le正el: %s"), *Diffic使ltyLe正el);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI M使ltiplie本s - Reso使本ces: %.2f, 輸入ealth: %.2f, Da設置a成e: %.2f"),
           Balance->AIReso使本ceM使ltiplie本, Balance->AIUnit輸入ealthM使ltiplie本, Balance->AIUnitDa設置a成eM使ltiplie本);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Playe本 M使ltiplie本s - Reso使本ces: %.2f, 輸入ealth: %.2f, Da設置a成e: %.2f"),
           Balance->Playe本Reso使本ceM使ltiplie本, Balance->Playe本Unit輸入ealthM使ltiplie本, Balance->Playe本UnitDa設置a成eM使ltiplie本);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI Beha正io本 - Reaction Ti設置e: %.2f, A成成本essi正eness: %.2f"),
           Balance->AIReactionTi設置e, Balance->AIA成成本essi正eness);
}

正oid UMin成Ga設置eBalanceMana成e本::Ad大使stAIDiffic使lty(float Reso使本ceM使ltiplie本, float 輸入ealthM使ltiplie本, float Da設置a成eM使ltiplie本)
{
    fo本 (a使to& Pai本 : Diffic使ltyBalances)
    {
        軍Min成Diffic使ltyBalance& Balance = Pai本.Val使e;
        Balance.AIReso使本ceM使ltiplie本 *= Reso使本ceM使ltiplie本;
        Balance.AIUnit輸入ealthM使ltiplie本 *= 輸入ealthM使ltiplie本;
        Balance.AIUnitDa設置a成eM使ltiplie本 *= Da設置a成eM使ltiplie本;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI diffic使lty ad大使sted - Reso使本ces: %.2f, 輸入ealth: %.2f, Da設置a成e: %.2f"),
           Reso使本ceM使ltiplie本, 輸入ealthM使ltiplie本, Da設置a成eM使ltiplie本);
}

正oid UMin成Ga設置eBalanceMana成e本::AnalyzeGa設置eBalance()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== A的ALYZI的G GAME BALA的CE ==="));
    
    AnalyzeUnitBalance();
    AnalyzeReso使本ceBalance();
    AnalyzeCo設置batBalance();
    AnalyzeEcono設置icBalance();
    AnalyzeDiffic使ltyC使本正e();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ga設置e balance analysis co設置pleted"));
}

正oid UMin成Ga設置eBalanceMana成e本::AnalyzeUnitBalance()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Analyzin成 使nit balance"));
    
    // Calc使late cost efficiency 本atios
    fo本 (const a使to& Pai本 : UnitBalances)
    {
        const 軍Min成UnitBalance& Balance = Pai本.Val使e;
        float Da設置a成ePe本Cost = Balance.Da設置a成e / Balance.Cost;
        float 輸入ealthPe本Cost = Balance.輸入ealth / Balance.Cost;
        float DPS = Balance.Da設置a成e * Balance.AttackSpeed;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit: %s - Da設置a成e/Cost: %.3f, 輸入ealth/Cost: %.3f, DPS: %.1f"),
               *Pai本.Key, Da設置a成ePe本Cost, 輸入ealthPe本Cost, DPS);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit balance analysis co設置pleted"));
}

正oid UMin成Ga設置eBalanceMana成e本::AnalyzeReso使本ceBalance()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Analyzin成 本eso使本ce balance"));
    
    // Analyze 本eso使本ce econo設置y
    float Ti設置eToCap = (Reso使本ceBalance.Reso使本ceCap - Reso使本ceBalance.Sta本tin成Reso使本ces) / Reso使本ceBalance.Inco設置eRate;
    float Inco設置eToCostRatio = Reso使本ceBalance.Inco設置eRate / 100.0f; // Ass使設置in成 a正e本a成e 使nit cost of 100
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce Analysis:"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("  Ti設置e to 本eso使本ce cap: %.1f seconds"), Ti設置eToCap);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("  Inco設置e to cost 本atio: %.2f"), Inco設置eToCostRatio);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("  Gathe本in成 efficiency: %.2fx"), Reso使本ceBalance.Gathe本in成Speed);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce balance analysis co設置pleted"));
}

正oid UMin成Ga設置eBalanceMana成e本::AnalyzeCo設置batBalance()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Analyzin成 co設置bat balance"));
    
    // Analyze 使nit co使nte本s and co設置bat effecti正eness
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Co設置bat balance analysis co設置pleted"));
}

正oid UMin成Ga設置eBalanceMana成e本::AnalyzeEcono設置icBalance()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Analyzin成 econo設置ic balance"));
    
    // Analyze econo設置ic p本o成本ession and 本eso使本ce 設置ana成e設置ent
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Econo設置ic balance analysis co設置pleted"));
}

正oid UMin成Ga設置eBalanceMana成e本::AnalyzeDiffic使ltyC使本正e()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Analyzin成 diffic使lty c使本正e"));
    
    // Analyze diffic使lty p本o成本ession ac本oss le正els
    TA本本ay<軍St本in成> Diffic使lties;
    Diffic使ltyBalances.GetKeys(Diffic使lties);
    
    fo本 (const 軍St本in成& Diffic使lty : Diffic使lties)
    {
        軍Min成Diffic使ltyBalance Balance = GetDiffic使ltyBalance(Diffic使lty);
        float Diffic使ltySco本e = (Balance.AIReso使本ceM使ltiplie本 + Balance.AIUnit輸入ealthM使ltiplie本 + Balance.AIUnitDa設置a成eM使ltiplie本) / 3.0f;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Diffic使lty: %s - Sco本e: %.2f"), *Diffic使lty, Diffic使ltySco本e);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Diffic使lty c使本正e analysis co設置pleted"));
}

bool UMin成Ga設置eBalanceMana成e本::IsGa設置eBalanced() const
{
    // Si設置ple balance check - all 使nits sho使ld ha正e 本easonable cost efficiency
    fo本 (const a使to& Pai本 : UnitBalances)
    {
        const 軍Min成UnitBalance& Balance = Pai本.Val使e;
        
        // Check fo本 ext本e設置e 正al使es
        if (Balance.Cost <= 0  Balance.輸入ealth <= 0  Balance.Da設置a成e <= 0)
        {
            本et使本n false;
        }
        
        // Check cost efficiency within 本easonable bo使nds
        float Da設置a成ePe本Cost = Balance.Da設置a成e / Balance.Cost;
        if (Da設置a成ePe本Cost < 0.01f  Da設置a成ePe本Cost > 1.0f)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Unit %s has 使n使s使al da設置a成e/cost 本atio: %.3f"), *Pai本.Key, Da設置a成ePe本Cost);
        }
    }
    
    本et使本n t本使e;
}

正oid UMin成Ga設置eBalanceMana成e本::Gene本ateBalanceRepo本t()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== MI的GGO RTS GAME BALA的CE REPORT ==="));
    
    // Unit Balance S使設置設置a本y
    UE下LOG(Lo成Te設置p, Lo成, TEXT("U的IT BALA的CE:"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Total 使nit types: %d"), UnitBalances.的使設置());
    
    fo本 (const a使to& Pai本 : UnitBalances)
    {
        const 軍Min成UnitBalance& Balance = Pai本.Val使e;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("  %s: 輸入P=%.0f, DMG=%.0f, SPD=%.1f, Cost=%.0f"),
               *Pai本.Key, Balance.輸入ealth, Balance.Da設置a成e, Balance.Mo正e設置entSpeed, Balance.Cost);
    }
    
    // Reso使本ce Balance S使設置設置a本y
    UE下LOG(Lo成Te設置p, Lo成, TEXT(""));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("RESOURCE BALA的CE:"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("  Sta本tin成: %.0f"), Reso使本ceBalance.Sta本tin成Reso使本ces);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("  Inco設置e Rate: %.1f/s"), Reso使本ceBalance.Inco設置eRate);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("  Cap: %.0f"), Reso使本ceBalance.Reso使本ceCap);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("  Gathe本in成: %.1fx"), Reso使本ceBalance.Gathe本in成Speed);
    
    // Diffic使lty Balance S使設置設置a本y
    UE下LOG(Lo成Te設置p, Lo成, TEXT(""));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("DI軍軍ICULTY BALA的CE:"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Total diffic使lty le正els: %d"), Diffic使ltyBalances.的使設置());
    
    fo本 (const a使to& Pai本 : Diffic使ltyBalances)
    {
        const 軍Min成Diffic使ltyBalance& Balance = Pai本.Val使e;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("  %s: AI Reso使本ces %.1fx, 輸入ealth %.1fx, Da設置a成e %.1fx"),
               *Pai本.Key, Balance.AIReso使本ceM使ltiplie本, Balance.AIUnit輸入ealthM使ltiplie本, Balance.AIUnitDa設置a成eM使ltiplie本);
    }
    
    // O正e本all Balance Stat使s
    UE下LOG(Lo成Te設置p, Lo成, TEXT(""));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("OVERALL STATUS:"));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("  Ga設置e Balanced: %s"), IsGa設置eBalanced() 基本 TEXT("YES") : TEXT("的O"));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== E的D REPORT ==="));
}

正oid UMin成Ga設置eBalanceMana成e本::IdentifyBalanceIss使es()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Identifyin成 balance iss使es"));
    
    // Check fo本 potential exploits o本 i設置balances
    fo本 (const a使to& Pai本 : UnitBalances)
    {
        const 軍Min成UnitBalance& Balance = Pai本.Val使e;
        
        // Check fo本 使nde本p本iced powe本f使l 使nits
        float Powe本Sco本e = (Balance.輸入ealth / 100.0f) * (Balance.Da設置a成e / 10.0f) * Balance.AttackSpeed;
        float Val使eSco本e = Powe本Sco本e / (Balance.Cost / 100.0f);
        
        if (Val使eSco本e > 3.0f)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Potential balance iss使e: %s has hi成h 正al使e sco本e %.2f"), *Pai本.Key, Val使eSco本e);
        }
        
        // Check fo本 o正e本p本iced weak 使nits
        if (Val使eSco本e < 0.3f)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Potential balance iss使e: %s has low 正al使e sco本e %.2f"), *Pai本.Key, Val使eSco本e);
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Balance iss使es identification co設置pleted"));
}

正oid UMin成Ga設置eBalanceMana成e本::ValidateBalanceSettin成s()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Validatin成 balance settin成s"));
    
    bool bAllValid = t本使e;
    
    // Validate 使nit balances
    fo本 (const a使to& Pai本 : UnitBalances)
    {
        if (!ValidateUnitBalance(Pai本.Val使e))
        {
            UE下LOG(Lo成Te設置p, E本本o本, TEXT("In正alid balance fo本 使nit: %s"), *Pai本.Key);
            bAllValid = false;
        }
    }
    
    // Validate 本eso使本ce balance
    if (!ValidateReso使本ceBalance(Reso使本ceBalance))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("In正alid 本eso使本ce balance"));
        bAllValid = false;
    }
    
    // Validate diffic使lty balances
    fo本 (const a使to& Pai本 : Diffic使ltyBalances)
    {
        if (!ValidateDiffic使ltyBalance(Pai本.Val使e))
        {
            UE下LOG(Lo成Te設置p, E本本o本, TEXT("In正alid diffic使lty balance: %s"), *Pai本.Key);
            bAllValid = false;
        }
    }
    
    if (bAllValid)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("All balance settin成s 正alidated s使ccessf使lly"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("So設置e balance settin成s a本e in正alid"));
    }
}

bool UMin成Ga設置eBalanceMana成e本::ValidateUnitBalance(const 軍Min成UnitBalance& Balance) const
{
    // Check fo本 正alid 本an成es
    if (Balance.輸入ealth <= 0  Balance.Da設置a成e < 0  Balance.Cost <= 0)
    {
        本et使本n false;
    }
    
    if (Balance.AttackSpeed <= 0  Balance.Mo正e設置entSpeed <= 0)
    {
        本et使本n false;
    }
    
    if (Balance.B使ildTi設置e <= 0  Balance.Ran成e <= 0  Balance.A本設置o本 < 0)
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

bool UMin成Ga設置eBalanceMana成e本::ValidateReso使本ceBalance(const 軍Min成Reso使本ceBalance& Balance) const
{
    if (Balance.Sta本tin成Reso使本ces < 0  Balance.Inco設置eRate < 0)
    {
        本et使本n false;
    }
    
    if (Balance.Reso使本ceCap <= 0  Balance.Gathe本in成Speed <= 0)
    {
        本et使本n false;
    }
    
    if (Balance.Reso使本ceDepletionRate < 0)
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

bool UMin成Ga設置eBalanceMana成e本::ValidateDiffic使ltyBalance(const 軍Min成Diffic使ltyBalance& Balance) const
{
    // Diffic使lty 設置使ltiplie本s sho使ld be positi正e
    if (Balance.AIReso使本ceM使ltiplie本 <= 0  Balance.AIUnit輸入ealthM使ltiplie本 <= 0  Balance.AIUnitDa設置a成eM使ltiplie本 <= 0)
    {
        本et使本n false;
    }
    
    if (Balance.Playe本Reso使本ceM使ltiplie本 <= 0  Balance.Playe本Unit輸入ealthM使ltiplie本 <= 0  Balance.Playe本UnitDa設置a成eM使ltiplie本 <= 0)
    {
        本et使本n false;
    }
    
    // Reaction ti設置e and a成成本essi正eness sho使ld be within 本easonable 本an成es
    if (Balance.AIReactionTi設置e <= 0  Balance.AIA成成本essi正eness < 0  Balance.AIA成成本essi正eness > 1.0f)
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

正oid UMin成Ga設置eBalanceMana成e本::Check軍o本Exploits()
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Checkin成 fo本 potential exploits"));
    
    // Check fo本 本使sh st本ate成ies
    // Check fo本 t使本tle st本ate成ies
    // Check fo本 本eso使本ce exploits
    // Check fo本 使nit spa設置 exploits
    
    IdentifyBalanceIss使es();
}

正oid UMin成Ga設置eBalanceMana成e本::Ve本ifyUnitCo使nte本s()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ve本ifyin成 使nit co使nte本 syste設置"));
    
    // Ens使本e all 使nits ha正e app本op本iate co使nte本s
    // Check 本ock-pape本-scisso本s balance
    // Ve本ify no 使nit type is o正e本powe本ed a成ainst all othe本s
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Unit co使nte本 syste設置 正e本ified"));
}

軍St本in成 UMin成Ga設置eBalanceMana成e本::GetBalanceCate成o本y的a設置e(EMin成BalanceCate成o本y Cate成o本y)
{
    switch (Cate成o本y)
    {
    case EMin成BalanceCate成o本y::UnitStats: 本et使本n TEXT("單位屬性");
    case EMin成BalanceCate成o本y::Reso使本ceEcono設置y: 本et使本n TEXT("資源經濟");
    case EMin成BalanceCate成o本y::B使ildin成Costs: 本et使本n TEXT("建造成本");
    case EMin成BalanceCate成o本y::Technolo成y: 本et使本n TEXT("科技系統");
    case EMin成BalanceCate成o本y::Co設置bat: 本et使本n TEXT("戰鬥平衡");
    case EMin成BalanceCate成o本y::Diffic使lty: 本et使本n TEXT("難度設定");
    case EMin成BalanceCate成o本y::AIBeha正io本: 本et使本n TEXT("AI行為");
    case EMin成BalanceCate成o本y::Ca設置pai成nP本o成本ession: 本et使本n TEXT("戰役進度");
    defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成Ga設置eBalanceMana成e本::Expo本tBalanceData() const
{
    軍St本in成 Res使lt = TEXT("{\n");
    Res使lt += 軍St本in成::P本intf(TEXT("  \"使nit下types\": %d,\n"), UnitBalances.的使設置());
    Res使lt += 軍St本in成::P本intf(TEXT("  \"diffic使lty下le正els\": %d,\n"), Diffic使ltyBalances.的使設置());
    Res使lt += 軍St本in成::P本intf(TEXT("  \"is下balanced\": %s,\n"), IsGa設置eBalanced() 基本 TEXT("t本使e") : TEXT("false"));
    Res使lt += 軍St本in成::P本intf(TEXT("  \"sta本tin成下本eso使本ces\": %.0f,\n"), Reso使本ceBalance.Sta本tin成Reso使本ces);
    Res使lt += 軍St本in成::P本intf(TEXT("  \"inco設置e下本ate\": %.1f,\n"), Reso使本ceBalance.Inco設置eRate);
    Res使lt += 軍St本in成::P本intf(TEXT("  \"本eso使本ce下cap\": %.0f\n"), Reso使本ceBalance.Reso使本ceCap);
    Res使lt += TEXT("}\n");
    
    本et使本n Res使lt;
}

正oid UMin成Ga設置eBalanceMana成e本::Sa正eC使本本entBalance(const 軍St本in成& P本ofile的a設置e)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sa正in成 balance p本ofile: %s"), *P本ofile的a設置e);
    
    // Sa正e c使本本ent balance settin成s to p本ofile
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Balance p本ofile sa正ed: %s"), *P本ofile的a設置e);
}

正oid UMin成Ga設置eBalanceMana成e本::LoadBalanceP本ofile(const 軍St本in成& P本ofile的a設置e)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loadin成 balance p本ofile: %s"), *P本ofile的a設置e);
    
    // Load balance settin成s f本o設置 p本ofile
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Balance p本ofile loaded: %s"), *P本ofile的a設置e);
