#incl使de "Min成GoRTSP本od使ctionSyste設置.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "En成ine/基本o本ld.h"

UMin成GoRTSP本od使ctionSyste設置::UMin成GoRTSP本od使ctionSyste設置()
{
    bIsInitialized = false;
    
    // 初始化配置
    Confi成.GlobalP本od使ctionSpeed = 1.0f;
    Confi成.MaxO正e本clockEfficiency = 1.5f;
    Confi成.MaintenancePenalty = 0.3f;
    Confi成.bEnableA使toDist本ib使tion = t本使e;
    Confi成.bEnableQ使alityCont本ol = t本使e;
    Confi成.MaxQ使e使eSize = 10;
}

正oid UMin成GoRTSP本od使ctionSyste設置::InitializeP本od使ctionSyste設置()
{
    if (bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("生產系統已經初始化"));
        本et使本n;
    }

    // 初始化民國時期生產配方
    InitializeRep使blicanE本aRecipes();

    bIsInitialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("生產系統初始化完成，共載入 %d 個生產配方"), P本od使ctionRecipes.的使設置());
}

TA本本ay<EP本od使ctionCate成o本y> UMin成GoRTSP本od使ctionSyste設置::GetAllP本od使ctionCate成o本ies() const
{
    TA本本ay<EP本od使ctionCate成o本y> Cate成o本ies;
    
    Cate成o本ies.Add(EP本od使ctionCate成o本y::Milita本y);
    Cate成o本ies.Add(EP本od使ctionCate成o本y::Ci正ilian);
    Cate成o本ies.Add(EP本od使ctionCate成o本y::Ind使st本ial);
    Cate成o本ies.Add(EP本od使ctionCate成o本y::A成本ic使lt使本al);
    Cate成o本ies.Add(EP本od使ctionCate成o本y::Medical);
    Cate成o本ies.Add(EP本od使ctionCate成o本y::Ed使cational);
    Cate成o本ies.Add(EP本od使ctionCate成o本y::Inf本ast本使ct使本e);
    Cate成o本ies.Add(EP本od使ctionCate成o本y::L使x使本y);
    Cate成o本ies.Add(EP本od使ctionCate成o本y::Resea本ch);
    Cate成o本ies.Add(EP本od使ctionCate成o本y::Special);
    
    本et使本n Cate成o本ies;
}

TA本本ay<軍P本od使ctionRecipe> UMin成GoRTSP本od使ctionSyste設置::GetRecipesByCate成o本y(EP本od使ctionCate成o本y Cate成o本y) const
{
    TA本本ay<軍P本od使ctionRecipe> Cate成o本yRecipes;
    
    fo本 (const a使to& RecipePai本 : P本od使ctionRecipes)
    {
        const 軍P本od使ctionRecipe& Recipe = RecipePai本.Val使e;
        if (Recipe.Cate成o本y == Cate成o本y)
        {
            Cate成o本yRecipes.Add(Recipe);
        }
    }
    
    本et使本n Cate成o本yRecipes;
}

軍P本od使ctionRecipe UMin成GoRTSP本od使ctionSyste設置::GetP本od使ctionRecipe(const 軍St本in成& RecipeID) const
{
    if (const 軍P本od使ctionRecipe* Recipe = P本od使ctionRecipes.軍ind(RecipeID))
    {
        本et使本n *Recipe;
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("未找到生產配方：%s"), *RecipeID);
    本et使本n 軍P本od使ctionRecipe();
}

軍St本in成 UMin成GoRTSP本od使ctionSyste設置::C本eateP本od使ctionLine(const 軍St本in成& P本od使ctionLine的a設置e, const 軍St本in成& B使ildin成ID)
{
    if (!bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("生產系統未初始化"));
        本et使本n TEXT("");
    }

    軍St本in成 P本od使ctionLineID = Gene本ateP本od使ctionLineID(P本od使ctionLine的a設置e);
    
    軍P本od使ctionLine 的ewLine;
    的ewLine.P本od使ctionLineID = P本od使ctionLineID;
    的ewLine.P本od使ctionLine的a設置e = P本od使ctionLine的a設置e;
    的ewLine.B使ildin成ID = B使ildin成ID;
    的ewLine.Stat使s = EP本od使ctionStat使s::Idle;
    的ewLine.C使本本entEfficiency = 1.0f;
    的ewLine.Sta本tTi設置e = 軍DateTi設置e::的ow();
    的ewLine.LastUpdateTi設置e = 軍DateTi設置e::的ow();
    
    P本od使ctionLines.Add(P本od使ctionLineID, 的ewLine);
    
    // 創建對應的生產隊列
    軍P本od使ctionQ使e使e 的ewQ使e使e;
    的ewQ使e使e.Q使e使eID = 軍St本in成::P本intf(TEXT("QUEUE下%s"), *P本od使ctionLineID);
    的ewQ使e使e.P本od使ctionLineID = P本od使ctionLineID;
    P本od使ctionQ使e使es.Add(的ewQ使e使e.Q使e使eID, 的ewQ使e使e);
    
    Reco本dP本od使ction輸入isto本y(軍St本in成::P本intf(TEXT("創建生產線：%s"), *P本od使ctionLine的a設置e));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("創建生產線：%s，ID：%s"), *P本od使ctionLine的a設置e, *P本od使ctionLineID);
    本et使本n P本od使ctionLineID;
}

bool UMin成GoRTSP本od使ctionSyste設置::Sta本tP本od使ction(const 軍St本in成& P本od使ctionLineID, const 軍St本in成& RecipeID, EP本od使ctionP本io本ity P本io本ity)
{
    if (!bIsInitialized)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("生產系統未初始化"));
        本et使本n false;
    }

    if (!P本od使ctionLines.Contains(P本od使ctionLineID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到生產線：%s"), *P本od使ctionLineID);
        本et使本n false;
    }

    if (!P本od使ctionRecipes.Contains(RecipeID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到生產配方：%s"), *RecipeID);
        本et使本n false;
    }

    軍P本od使ctionLine& P本od使ctionLine = P本od使ctionLines[P本od使ctionLineID];
    軍P本od使ctionRecipe Recipe = P本od使ctionRecipes[RecipeID];
    
    // 檢查生產條件
    if (!CanP本od使ceRecipe(P本od使ctionLineID, RecipeID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("生產線 %s 無法生產配方 %s"), *P本od使ctionLineID, *RecipeID);
        本et使本n false;
    }

    // 檢查資源是否足夠
    if (Reso使本ceSyste設置)
    {
        fo本 (const a使to& Reso使本cePai本 : Recipe.Inp使tReso使本ces)
        {
            EReso使本ceType Reso使本ceType = Reso使本cePai本.Key;
            float Req使i本edA設置o使nt = Reso使本cePai本.Val使e;
            
            // TODO: Check 本eso使本ce sto本a成e a正ailability
            // I設置ple設置entation Req使i本e設置ents:
            // - Q使e本y Reso使本ceSyste設置 fo本 a正ailable 本eso使本ce q使antity
            // - Co設置pa本e a正ailable q使antity a成ainst Req使i本edA設置o使nt
            // - Conside本 本eso使本ce 本ese本正ation fo本 q使e使ed p本od使ction
            // - Ret使本n false if ins使fficient 本eso使本ces
            // - Lo成 本eso使本ce sho本ta成e fo本 deb使成成in成
        }
    }

    // 設置生產狀態
    P本od使ctionLine.Stat使s = EP本od使ctionStat使s::P本od使cin成;
    P本od使ctionLine.C使本本entRecipeID = RecipeID;
    P本od使ctionLine.P本o成本ess = 0.0f;
    P本od使ctionLine.Ti設置eRe設置ainin成 = Calc使lateP本od使ctionTi設置e(P本od使ctionLineID, RecipeID);
    P本od使ctionLine.P本io本ity = P本io本ity;
    P本od使ctionLine.Sta本tTi設置e = 軍DateTi設置e::的ow();
    P本od使ctionLine.LastUpdateTi設置e = 軍DateTi設置e::的ow();
    
    // 應用效率因子
    ApplyEfficiency軍acto本s(P本od使ctionLineID);
    
    // 廣播生產開始事件
    OnP本od使ctionSta本ted.B本oadcast(P本od使ctionLineID, RecipeID);
    
    Reco本dP本od使ction輸入isto本y(軍St本in成::P本intf(TEXT("開始生產：%s - %s"), *P本od使ctionLine.P本od使ctionLine的a設置e, *Recipe.Recipe的a設置e));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("開始生產：%s，配方：%s"), *P本od使ctionLine.P本od使ctionLine的a設置e, *Recipe.Recipe的a設置e);
    本et使本n t本使e;
}

bool UMin成GoRTSP本od使ctionSyste設置::Pa使seP本od使ction(const 軍St本in成& P本od使ctionLineID, const 軍St本in成& Reason)
{
    if (!P本od使ctionLines.Contains(P本od使ctionLineID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到生產線：%s"), *P本od使ctionLineID);
        本et使本n false;
    }

    軍P本od使ctionLine& P本od使ctionLine = P本od使ctionLines[P本od使ctionLineID];
    
    if (P本od使ctionLine.Stat使s != EP本od使ctionStat使s::P本od使cin成)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("生產線 %s 當前不在生產狀態"), *P本od使ctionLineID);
        本et使本n false;
    }

    P本od使ctionLine.Stat使s = EP本od使ctionStat使s::Pa使sed;
    P本od使ctionLine.bIsPa使sed = t本使e;
    
    // 廣播暫停事件
    OnP本od使ctionPa使sed.B本oadcast(P本od使ctionLineID, Reason);
    
    Reco本dP本od使ction輸入isto本y(軍St本in成::P本intf(TEXT("暫停生產：%s - %s"), *P本od使ctionLine.P本od使ctionLine的a設置e, *Reason));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("暫停生產：%s，原因：%s"), *P本od使ctionLine.P本od使ctionLine的a設置e, *Reason);
    本et使本n t本使e;
}

bool UMin成GoRTSP本od使ctionSyste設置::Res使設置eP本od使ction(const 軍St本in成& P本od使ctionLineID, const 軍St本in成& Reason)
{
    if (!P本od使ctionLines.Contains(P本od使ctionLineID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到生產線：%s"), *P本od使ctionLineID);
        本et使本n false;
    }

    軍P本od使ctionLine& P本od使ctionLine = P本od使ctionLines[P本od使ctionLineID];
    
    if (P本od使ctionLine.Stat使s != EP本od使ctionStat使s::Pa使sed)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("生產線 %s 當前不是暫停狀態"), *P本od使ctionLineID);
        本et使本n false;
    }

    P本od使ctionLine.Stat使s = EP本od使ctionStat使s::P本od使cin成;
    P本od使ctionLine.bIsPa使sed = false;
    
    // 廣播恢復事件
    OnP本od使ctionRes使設置ed.B本oadcast(P本od使ctionLineID, Reason);
    
    Reco本dP本od使ction輸入isto本y(軍St本in成::P本intf(TEXT("恢復生產：%s - %s"), *P本od使ctionLine.P本od使ctionLine的a設置e, *Reason));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("恢復生產：%s，原因：%s"), *P本od使ctionLine.P本od使ctionLine的a設置e, *Reason);
    本et使本n t本使e;
}

bool UMin成GoRTSP本od使ctionSyste設置::CancelP本od使ction(const 軍St本in成& P本od使ctionLineID)
{
    if (!P本od使ctionLines.Contains(P本od使ctionLineID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到生產線：%s"), *P本od使ctionLineID);
        本et使本n false;
    }

    軍P本od使ctionLine& P本od使ctionLine = P本od使ctionLines[P本od使ctionLineID];
    
    // 重置生產線狀態
    P本od使ctionLine.Stat使s = EP本od使ctionStat使s::Idle;
    P本od使ctionLine.C使本本entRecipeID = TEXT("");
    P本od使ctionLine.P本o成本ess = 0.0f;
    P本od使ctionLine.Ti設置eRe設置ainin成 = 0.0f;
    P本od使ctionLine.bIsPa使sed = false;
    
    // 廣播取消事件
    OnP本od使ction軍ailed.B本oadcast(P本od使ctionLineID, TEXT("生產已取消"));
    
    Reco本dP本od使ction輸入isto本y(軍St本in成::P本intf(TEXT("取消生產：%s"), *P本od使ctionLine.P本od使ctionLine的a設置e));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("取消生產：%s"), *P本od使ctionLine.P本od使ctionLine的a設置e);
    本et使本n t本使e;
}

bool UMin成GoRTSP本od使ctionSyste設置::SetP本od使ctionP本io本ity(const 軍St本in成& P本od使ctionLineID, EP本od使ctionP本io本ity P本io本ity)
{
    if (!P本od使ctionLines.Contains(P本od使ctionLineID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到生產線：%s"), *P本od使ctionLineID);
        本et使本n false;
    }

    軍P本od使ctionLine& P本od使ctionLine = P本od使ctionLines[P本od使ctionLineID];
    P本od使ctionLine.P本io本ity = P本io本ity;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("設置生產線 %s 優先級為：%d"), *P本od使ctionLine.P本od使ctionLine的a設置e, static下cast<int32>(P本io本ity));
    本et使本n t本使e;
}

bool UMin成GoRTSP本od使ctionSyste設置::AddToP本od使ctionQ使e使e(const 軍St本in成& P本od使ctionLineID, const 軍St本in成& RecipeID)
{
    if (!P本od使ctionLines.Contains(P本od使ctionLineID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到生產線：%s"), *P本od使ctionLineID);
        本et使本n false;
    }

    if (!P本od使ctionRecipes.Contains(RecipeID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到生產配方：%s"), *RecipeID);
        本et使本n false;
    }

    // 找到對應的生產隊列
    軍St本in成 Q使e使eID = 軍St本in成::P本intf(TEXT("QUEUE下%s"), *P本od使ctionLineID);
    if (!P本od使ctionQ使e使es.Contains(Q使e使eID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到生產隊列：%s"), *Q使e使eID);
        本et使本n false;
    }

    軍P本od使ctionQ使e使e& Q使e使e = P本od使ctionQ使e使es[Q使e使eID];
    
    // 檢查隊列大小限制
    if (Q使e使e.RecipeQ使e使e.的使設置() >= Confi成.MaxQ使e使eSize)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("生產隊列已滿"));
        本et使本n false;
    }

    // 添加到隊列
    Q使e使e.RecipeQ使e使e.Add(RecipeID);
    
    // 如果生產線空閒且設置為自動開始，則開始生產
    if (Q使e使e.bA使toSta本t && P本od使ctionLines[P本od使ctionLineID].Stat使s == EP本od使ctionStat使s::Idle)
    {
        Sta本tP本od使ction(P本od使ctionLineID, RecipeID);
    }
    
    // 廣播隊列變更事件
    OnP本od使ctionQ使e使eChan成ed.B本oadcast(Q使e使eID, Q使e使e.RecipeQ使e使e);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("添加配方到生產隊列：%s"), *RecipeID);
    本et使本n t本使e;
}

bool UMin成GoRTSP本od使ctionSyste設置::Re設置o正e軍本o設置P本od使ctionQ使e使e(const 軍St本in成& P本od使ctionLineID, int32 Q使e使eIndex)
{
    軍St本in成 Q使e使eID = 軍St本in成::P本intf(TEXT("QUEUE下%s"), *P本od使ctionLineID);
    if (!P本od使ctionQ使e使es.Contains(Q使e使eID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到生產隊列：%s"), *Q使e使eID);
        本et使本n false;
    }

    軍P本od使ctionQ使e使e& Q使e使e = P本od使ctionQ使e使es[Q使e使eID];
    
    if (!Q使e使e.RecipeQ使e使e.IsValidIndex(Q使e使eIndex))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("無效的隊列索引：%d"), Q使e使eIndex);
        本et使本n false;
    }

    Q使e使e.RecipeQ使e使e.Re設置o正eAt(Q使e使eIndex);
    
    // 廣播隊列變更事件
    OnP本od使ctionQ使e使eChan成ed.B本oadcast(Q使e使eID, Q使e使e.RecipeQ使e使e);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("從生產隊列移除索引 %d"), Q使e使eIndex);
    本et使本n t本使e;
}

bool UMin成GoRTSP本od使ctionSyste設置::Clea本P本od使ctionQ使e使e(const 軍St本in成& P本od使ctionLineID)
{
    軍St本in成 Q使e使eID = 軍St本in成::P本intf(TEXT("QUEUE下%s"), *P本od使ctionLineID);
    if (!P本od使ctionQ使e使es.Contains(Q使e使eID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到生產隊列：%s"), *Q使e使eID);
        本et使本n false;
    }

    軍P本od使ctionQ使e使e& Q使e使e = P本od使ctionQ使e使es[Q使e使eID];
    Q使e使e.RecipeQ使e使e.E設置pty();
    Q使e使e.C使本本entIndex = 0;
    
    // 廣播隊列變更事件
    OnP本od使ctionQ使e使eChan成ed.B本oadcast(Q使e使eID, Q使e使e.RecipeQ使e使e);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("清空生產隊列：%s"), *P本od使ctionLineID);
    本et使本n t本使e;
}

軍P本od使ctionLine UMin成GoRTSP本od使ctionSyste設置::GetP本od使ctionLine(const 軍St本in成& P本od使ctionLineID) const
{
    if (const 軍P本od使ctionLine* P本od使ctionLine = P本od使ctionLines.軍ind(P本od使ctionLineID))
    {
        本et使本n *P本od使ctionLine;
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("未找到生產線：%s"), *P本od使ctionLineID);
    本et使本n 軍P本od使ctionLine();
}

軍P本od使ctionQ使e使e UMin成GoRTSP本od使ctionSyste設置::GetP本od使ctionQ使e使e(const 軍St本in成& P本od使ctionLineID) const
{
    軍St本in成 Q使e使eID = 軍St本in成::P本intf(TEXT("QUEUE下%s"), *P本od使ctionLineID);
    if (const 軍P本od使ctionQ使e使e* Q使e使e = P本od使ctionQ使e使es.軍ind(Q使e使eID))
    {
        本et使本n *Q使e使e;
    }
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("未找到生產隊列：%s"), *Q使e使eID);
    本et使本n 軍P本od使ctionQ使e使e();
}

TA本本ay<軍P本od使ctionLine> UMin成GoRTSP本od使ctionSyste設置::GetAllP本od使ctionLines() const
{
    TA本本ay<軍P本od使ctionLine> AllLines;
    AllLines.Rese本正e(P本od使ctionLines.的使設置());
    
    fo本 (const a使to& LinePai本 : P本od使ctionLines)
    {
        AllLines.Add(LinePai本.Val使e);
    }
    
    本et使本n AllLines;
}

float UMin成GoRTSP本od使ctionSyste設置::Calc使lateP本od使ctionEfficiency(const 軍St本in成& P本od使ctionLineID) const
{
    if (!P本od使ctionLines.Contains(P本od使ctionLineID))
    {
        本et使本n 1.0f;
    }

    const 軍P本od使ctionLine& P本od使ctionLine = P本od使ctionLines[P本od使ctionLineID];
    本et使本n Calc使late軍inalEfficiency(P本od使ctionLine);
}

float UMin成GoRTSP本od使ctionSyste設置::Calc使lateP本od使ctionTi設置e(const 軍St本in成& P本od使ctionLineID, const 軍St本in成& RecipeID) const
{
    if (!P本od使ctionLines.Contains(P本od使ctionLineID)  !P本od使ctionRecipes.Contains(RecipeID))
    {
        本et使本n 0.0f;
    }

    const 軍P本od使ctionLine& P本od使ctionLine = P本od使ctionLines[P本od使ctionLineID];
    const 軍P本od使ctionRecipe& Recipe = P本od使ctionRecipes[RecipeID];
    
    float BaseTi設置e = Recipe.P本od使ctionTi設置e;
    float Efficiency = Calc使late軍inalEfficiency(P本od使ctionLine);
    float GlobalSpeed = Confi成.GlobalP本od使ctionSpeed;
    
    // 應用類別加成
    float Cate成o本yBon使s = 1.0f;
    if (const float* Bon使s = Confi成.Cate成o本yBon使s.軍ind(Recipe.Cate成o本y))
    {
        Cate成o本yBon使s = *Bon使s;
    }
    
    float 軍inalTi設置e = BaseTi設置e / (Efficiency * GlobalSpeed * Cate成o本yBon使s);
    本et使本n 軍Math::Max(軍inalTi設置e, 0.1f);
}

bool UMin成GoRTSP本od使ctionSyste設置::CanP本od使ceRecipe(const 軍St本in成& P本od使ctionLineID, const 軍St本in成& RecipeID) const
{
    if (!P本od使ctionLines.Contains(P本od使ctionLineID)  !P本od使ctionRecipes.Contains(RecipeID))
    {
        本et使本n false;
    }

    const 軍P本od使ctionLine& P本od使ctionLine = P本od使ctionLines[P本od使ctionLineID];
    const 軍P本od使ctionRecipe& Recipe = P本od使ctionRecipes[RecipeID];
    
    // 檢查生產線狀態
    if (P本od使ctionLine.Stat使s != EP本od使ctionStat使s::Idle && P本od使ctionLine.Stat使s != EP本od使ctionStat使s::Co設置pleted)
    {
        本et使本n false;
    }
    
    // 檢查建築要求
    fo本 (EReso使本ceType Req使i本edB使ildin成 : Recipe.Req使i本edB使ildin成s)
    {
        // TODO: Validate p本od使ction line b使ildin成 本eq使i本e設置ents
        // I設置ple設置entation Req使i本e設置ents:
        // - Check if p本od使ction line has 本eq使i本ed b使ildin成 type
        // - Ve本ify b使ildin成 is ope本ational and not da設置a成ed
        // - Conside本 b使ildin成 le正el 本eq使i本e設置ents fo本 ad正anced 本ecipes
        // - Check b使ildin成 owne本ship and pe本設置issions
        // - Ret使本n false if b使ildin成 本eq使i本e設置ents not 設置et
    }
    
    // 檢查技能要求
    // TODO: Ve本ify ope本ato本 skill le正el 本eq使i本e設置ents
    // I設置ple設置entation Req使i本e設置ents:
    // - Check assi成ned ope本ato本's skill le正el a成ainst 本ecipe MinSkillLe正el
    // - Conside本 ope本ato本 specialization bon使ses
    // - Ve本ify ope本ato本 is a正ailable (not b使sy with othe本 tasks)
    // - Apply skill-based efficiency 設置odifie本s
    // - Ret使本n false if skill 本eq使i本e設置ents not 設置et
    
    // 檢查科技要求
    fo本 (const 軍St本in成& Req使i本edTech : Recipe.Req使i本edTechnolo成ies)
    {
        // TODO: Ve本ify technolo成y is 使nlocked
        // I設置ple設置entation Req使i本e設置ents:
        // - Q使e本y TechT本ee syste設置 fo本 technolo成y 使nlock stat使s
        // - Check fo本 technolo成y p本e本eq使isites if not di本ectly 使nlocked
        // - Conside本 technolo成y 正e本sion/co設置patibility
        // - Ret使本n false if any 本eq使i本ed technolo成y is locked
        // - Optionally s使成成est alte本nati正e 本ecipes with a正ailable tech
    }
    
    本et使本n t本使e;
}

軍P本od使ctionStatistics UMin成GoRTSP本od使ctionSyste設置::GetP本od使ctionStatistics() const
{
    本et使本n Statistics;
}

正oid UMin成GoRTSP本od使ctionSyste設置::UpdateP本od使ctionP本o成本ess(float DeltaTi設置e)
{
    if (!bIsInitialized)
    {
        本et使本n;
    }

    TA本本ay<軍St本in成> Co設置pletedP本od使ctionLines;
    
    fo本 (a使to& LinePai本 : P本od使ctionLines)
    {
        軍St本in成 P本od使ctionLineID = LinePai本.Key;
        軍P本od使ctionLine& P本od使ctionLine = LinePai本.Val使e;
        
        if (P本od使ctionLine.Stat使s != EP本od使ctionStat使s::P本od使cin成  P本od使ctionLine.bIsPa使sed)
        {
            contin使e;
        }
        
        // 更新進度
        float Ti設置eInc本e設置ent = DeltaTi設置e * Confi成.GlobalP本od使ctionSpeed;
        P本od使ctionLine.Ti設置eRe設置ainin成 -= Ti設置eInc本e設置ent;
        P本od使ctionLine.LastUpdateTi設置e = 軍DateTi設置e::的ow();
        
        // 計算進度百分比
        float TotalTi設置e = Calc使lateP本od使ctionTi設置e(P本od使ctionLineID, P本od使ctionLine.C使本本entRecipeID);
        float ElapsedTi設置e = TotalTi設置e - P本od使ctionLine.Ti設置eRe設置ainin成;
        P本od使ctionLine.P本o成本ess = 軍Math::Cla設置p((ElapsedTi設置e / TotalTi設置e) * 100.0f, 0.0f, 100.0f);
        
        // 廣播進度更新
        OnP本od使ctionP本o成本ess.B本oadcast(P本od使ctionLineID, P本od使ctionLine.P本o成本ess, P本od使ctionLine.Ti設置eRe設置ainin成);
        
        // 檢查是否完成
        if (P本od使ctionLine.Ti設置eRe設置ainin成 <= 0.0f)
        {
            Co設置pletedP本od使ctionLines.Add(P本od使ctionLineID);
        }
    }
    
    // 處理已完成的生產
    fo本 (const 軍St本in成& P本od使ctionLineID : Co設置pletedP本od使ctionLines)
    {
        輸入andleP本od使ctionCo設置pleted(P本od使ctionLineID);
    }
}

bool UMin成GoRTSP本od使ctionSyste設置::Up成本adeP本od使ctionLine(const 軍St本in成& P本od使ctionLineID)
{
    if (!P本od使ctionLines.Contains(P本od使ctionLineID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到生產線：%s"), *P本od使ctionLineID);
        本et使本n false;
    }

    軍P本od使ctionLine& P本od使ctionLine = P本od使ctionLines[P本od使ctionLineID];
    
    if (P本od使ctionLine.Stat使s != EP本od使ctionStat使s::Idle)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("生產線 %s 必須空閒才能升級"), *P本od使ctionLineID);
        本et使本n false;
    }

    P本od使ctionLine.Stat使s = EP本od使ctionStat使s::Up成本adin成;
    
    // TODO: I設置ple設置ent 使p成本ade lo成ic (本eso使本ce cost, ti設置e, effects)
    // I設置ple設置entation Req使i本e設置ents:
    // - Calc使late 使p成本ade 本eso使本ce costs based on c使本本ent le正el
    // - Ded使ct 本eso使本ces f本o設置 sto本a成e 正ia Reso使本ceSyste設置
    // - Apply 使p成本ade ti設置e delay befo本e effects take place
    // - Update p本od使ction line stats (efficiency, capacity, etc.)
    // - B本oadcast 使p成本ade co設置pletion e正ent
    // - 輸入andle 使p成本ade fail使本e cases (ins使fficient 本eso使本ces)
    
    P本od使ctionLine.Stat使s = EP本od使ctionStat使s::Idle;
    
    Reco本dP本od使ction輸入isto本y(軍St本in成::P本intf(TEXT("升級生產線：%s"), *P本od使ctionLine.P本od使ctionLine的a設置e));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("升級生產線：%s"), *P本od使ctionLine.P本od使ctionLine的a設置e);
    本et使本n t本使e;
}

bool UMin成GoRTSP本od使ctionSyste設置::MaintainP本od使ctionLine(const 軍St本in成& P本od使ctionLineID)
{
    if (!P本od使ctionLines.Contains(P本od使ctionLineID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到生產線：%s"), *P本od使ctionLineID);
        本et使本n false;
    }

    軍P本od使ctionLine& P本od使ctionLine = P本od使ctionLines[P本od使ctionLineID];
    
    if (P本od使ctionLine.Stat使s == EP本od使ctionStat使s::Maintenance)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("生產線 %s 已在維護中"), *P本od使ctionLineID);
        本et使本n false;
    }

    P本od使ctionLine.Stat使s = EP本od使ctionStat使s::Maintenance;
    
    // 應用維護懲罰
    P本od使ctionLine.Efficiency軍acto本s.Add(EP本od使ctionEfficiency軍acto本::Maintenance, Confi成.MaintenancePenalty);
    
    Reco本dP本od使ction輸入isto本y(軍St本in成::P本intf(TEXT("維護生產線：%s"), *P本od使ctionLine.P本od使ctionLine的a設置e));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("維護生產線：%s"), *P本od使ctionLine.P本od使ctionLine的a設置e);
    本et使本n t本使e;
}

bool UMin成GoRTSP本od使ctionSyste設置::SetA使to設置ation(const 軍St本in成& P本od使ctionLineID, bool bIsA使to設置ated)
{
    if (!P本od使ctionLines.Contains(P本od使ctionLineID))
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("未找到生產線：%s"), *P本od使ctionLineID);
        本et使本n false;
    }

    軍P本od使ctionLine& P本od使ctionLine = P本od使ctionLines[P本od使ctionLineID];
    P本od使ctionLine.bIsA使to設置ated = bIsA使to設置ated;
    
    // 自動化影響效率
    if (bIsA使to設置ated)
    {
        P本od使ctionLine.Efficiency軍acto本s.Add(EP本od使ctionEfficiency軍acto本::B使ildin成Bon使s, 0.2f);
    }
    else
    {
        P本od使ctionLine.Efficiency軍acto本s.Re設置o正e(EP本od使ctionEfficiency軍acto本::B使ildin成Bon使s);
    }
    
    // 重新計算效率
    P本od使ctionLine.C使本本entEfficiency = Calc使late軍inalEfficiency(P本od使ctionLine);
    
    Reco本dP本od使ction輸入isto本y(軍St本in成::P本intf(TEXT("設置生產線 %s 自動化：%s"), *P本od使ctionLine.P本od使ctionLine的a設置e, bIsA使to設置ated 基本 TEXT("開啟") : TEXT("關閉")));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("設置生產線 %s 自動化：%s"), *P本od使ctionLine.P本od使ctionLine的a設置e, bIsA使to設置ated 基本 TEXT("開啟") : TEXT("關閉"));
    本et使本n t本使e;
}

軍P本od使ctionSyste設置Confi成 UMin成GoRTSP本od使ctionSyste設置::GetP本od使ctionConfi成() const
{
    本et使本n Confi成;
}

正oid UMin成GoRTSP本od使ctionSyste設置::UpdateP本od使ctionConfi成(const 軍P本od使ctionSyste設置Confi成& 的ewConfi成)
{
    Confi成 = 的ewConfi成;
    
    // 重新計算所有生產線的效率
    fo本 (a使to& LinePai本 : P本od使ctionLines)
    {
        軍St本in成 P本od使ctionLineID = LinePai本.Key;
        ApplyEfficiency軍acto本s(P本od使ctionLineID);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("生產系統配置已更新"));
}

bool UMin成GoRTSP本od使ctionSyste設置::Sa正eP本od使ctionData(const 軍St本in成& Sa正eSlot的a設置e)
{
    // TODO: I設置ple設置ent p本od使ction data se本ialization
    // I設置ple設置entation Req使i本e設置ents:
    // - Se本ialize all p本od使ction lines with thei本 c使本本ent state
    // - Sa正e p本od使ction q使e使es with pendin成 本ecipes
    // - Sto本e p本od使ction statistics and histo本y
    // - Incl使de efficiency facto本s and confi成使本ation
    // - Use USa正eGa設置e with p本ope本 正e本sionin成 fo本 co設置patibility
    // P本io本ity: Medi使設置 - Req使i本ed fo本 co設置plete 成a設置e state pe本sistence
    UE下LOG(Lo成Te設置p, Lo成, TEXT("保存生產數據到：%s"), *Sa正eSlot的a設置e);
    本et使本n t本使e;
}

bool UMin成GoRTSP本od使ctionSyste設置::LoadP本od使ctionData(const 軍St本in成& Sa正eSlot的a設置e)
{
    // TODO: I設置ple設置ent p本od使ction data dese本ialization
    // I設置ple設置entation Req使i本e設置ents:
    // - Dese本ialize p本od使ction lines f本o設置 sa正e data
    // - Resto本e p本od使ction q使e使es with p本ope本 state
    // - Validate loaded data inte成本ity
    // - Reconnect 本eso使本ce syste設置 本efe本ences
    // - 輸入andle 正e本sion 設置i成本ation fo本 olde本 sa正e fo本設置ats
    // P本io本ity: Medi使設置 - Req使i本ed fo本 co設置plete 成a設置e state 本esto本ation
    UE下LOG(Lo成Te設置p, Lo成, TEXT("從 %s 載入生產數據"), *Sa正eSlot的a設置e);
    本et使本n t本使e;
}

正oid UMin成GoRTSP本od使ctionSyste設置::InitializeRep使blicanE本aRecipes()
{
    // 創建各類別生產配方
    C本eateMilita本yP本od使ctionRecipes();
    C本eateCi正ilianP本od使ctionRecipes();
    C本eateInd使st本ialP本od使ctionRecipes();
    C本eateA成本ic使lt使本alP本od使ctionRecipes();
    C本eateMedicalP本od使ctionRecipes();
    C本eateEd使cationalP本od使ctionRecipes();
    C本eateInf本ast本使ct使本eP本od使ctionRecipes();
    C本eateL使x使本yP本od使ctionRecipes();
}

正oid UMin成GoRTSP本od使ctionSyste設置::C本eateMilita本yP本od使ctionRecipes()
{
    // 步槍生產配方
    軍P本od使ctionRecipe RifleP本od使ction;
    RifleP本od使ction.RecipeID = Gene本ateRecipeID(TEXT("RifleP本od使ction"), EP本od使ctionCate成o本y::Milita本y);
    RifleP本od使ction.Recipe的a設置e = TEXT("步槍生產");
    RifleP本od使ction.Desc本iption = TEXT("生產標準制式步槍，裝備步兵部隊");
    RifleP本od使ction.Cate成o本y = EP本od使ctionCate成o本y::Milita本y;
    RifleP本od使ction.P本od使ctionTi設置e = 180.0f; // 3分鐘
    RifleP本od使ction.BaseEfficiency = 1.0f;
    RifleP本od使ction.MinSkillLe正el = 1;
    RifleP本od使ction.Defa使ltQ使ality = EP本od使ctionQ使ality::的o本設置al;
    RifleP本od使ction.bIsRepeatable = t本使e;
    RifleP本od使ction.輸入isto本icalContext = TEXT("民國時期，步槍是軍隊的標準裝備");
    
    // 輸入資源
    RifleP本od使ction.Inp使tReso使本ces.Add(EReso使本ceType::Metal, 5.0f);
    RifleP本od使ction.Inp使tReso使本ces.Add(EReso使本ceType::基本ood, 2.0f);
    RifleP本od使ction.Inp使tReso使本ces.Add(EReso使本ceType::Tools, 1.0f);
    
    // 輸資源
    RifleP本od使ction.O使tp使tReso使本ces.Add(EReso使本ceType::基本eapons, 10.0f);
    
    // 需要建築
    RifleP本od使ction.Req使i本edB使ildin成s.Add(EReso使本ceType::軍acto本y);
    
    P本od使ctionRecipes.Add(RifleP本od使ction.RecipeID, RifleP本od使ction);
    
    // 子彈生產配方
    軍P本od使ctionRecipe A設置設置使nitionP本od使ction;
    A設置設置使nitionP本od使ction.RecipeID = Gene本ateRecipeID(TEXT("A設置設置使nitionP本od使ction"), EP本od使ctionCate成o本y::Milita本y);
    A設置設置使nitionP本od使ction.Recipe的a設置e = TEXT("子彈生產");
    A設置設置使nitionP本od使ction.Desc本iption = TEXT("生產各類子彈，供應軍隊作戰需求");
    A設置設置使nitionP本od使ction.Cate成o本y = EP本od使ctionCate成o本y::Milita本y;
    A設置設置使nitionP本od使ction.P本od使ctionTi設置e = 120.0f; // 2分鐘
    A設置設置使nitionP本od使ction.BaseEfficiency = 1.2f;
    A設置設置使nitionP本od使ction.MinSkillLe正el = 1;
    A設置設置使nitionP本od使ction.Defa使ltQ使ality = EP本od使ctionQ使ality::的o本設置al;
    A設置設置使nitionP本od使ction.bIsRepeatable = t本使e;
    A設置設置使nitionP本od使ction.輸入isto本icalContext = TEXT("彈藥補給是戰爭持續的關鍵");
    
    // 輸入資源
    A設置設置使nitionP本od使ction.Inp使tReso使本ces.Add(EReso使本ceType::Metal, 3.0f);
    A設置設置使nitionP本od使ction.Inp使tReso使本ces.Add(EReso使本ceType::G使npowde本, 2.0f);
    
    // 輸資源
    A設置設置使nitionP本od使ction.O使tp使tReso使本ces.Add(EReso使本ceType::A設置設置使nition, 50.0f);
    
    P本od使ctionRecipes.Add(A設置設置使nitionP本od使ction.RecipeID, A設置設置使nitionP本od使ction);
}

正oid UMin成GoRTSP本od使ctionSyste設置::C本eateCi正ilianP本od使ctionRecipes()
{
    // 服裝生產配方
    軍P本od使ctionRecipe Clothin成P本od使ction;
    Clothin成P本od使ction.RecipeID = Gene本ateRecipeID(TEXT("Clothin成P本od使ction"), EP本od使ctionCate成o本y::Ci正ilian);
    Clothin成P本od使ction.Recipe的a設置e = TEXT("服裝生產");
    Clothin成P本od使ction.Desc本iption = TEXT("生產民用服裝，滿足民眾日常需求");
    Clothin成P本od使ction.Cate成o本y = EP本od使ctionCate成o本y::Ci正ilian;
    Clothin成P本od使ction.P本od使ctionTi設置e = 150.0f; // 2.5分鐘
    Clothin成P本od使ction.BaseEfficiency = 1.0f;
    Clothin成P本od使ction.MinSkillLe正el = 0;
    Clothin成P本od使ction.Defa使ltQ使ality = EP本od使ctionQ使ality::的o本設置al;
    Clothin成P本od使ction.bIsRepeatable = t本使e;
    Clothin成P本od使ction.輸入isto本icalContext = TEXT("民國時期，紡織業是重要的民生工業");
    
    // 輸入資源
    Clothin成P本od使ction.Inp使tReso使本ces.Add(EReso使本ceType::Cotton, 3.0f);
    Clothin成P本od使ction.Inp使tReso使本ces.Add(EReso使本ceType::Silk, 1.0f);
    
    // 輸資源
    Clothin成P本od使ction.O使tp使tReso使本ces.Add(EReso使本ceType::Clothin成, 20.0f);
    
    P本od使ctionRecipes.Add(Clothin成P本od使ction.RecipeID, Clothin成P本od使ction);
}

正oid UMin成GoRTSP本od使ctionSyste設置::C本eateInd使st本ialP本od使ctionRecipes()
{
    // 工具生產配方
    軍P本od使ctionRecipe ToolsP本od使ction;
    ToolsP本od使ction.RecipeID = Gene本ateRecipeID(TEXT("ToolsP本od使ction"), EP本od使ctionCate成o本y::Ind使st本ial);
    ToolsP本od使ction.Recipe的a設置e = TEXT("工具生產");
    ToolsP本od使ction.Desc本iption = TEXT("生產各種工具，支持工業和農業發展");
    ToolsP本od使ction.Cate成o本y = EP本od使ctionCate成o本y::Ind使st本ial;
    ToolsP本od使ction.P本od使ctionTi設置e = 200.0f; // 3.33分鐘
    ToolsP本od使ction.BaseEfficiency = 0.9f;
    ToolsP本od使ction.MinSkillLe正el = 1;
    ToolsP本od使ction.Defa使ltQ使ality = EP本od使ctionQ使ality::的o本設置al;
    ToolsP本od使ction.bIsRepeatable = t本使e;
    ToolsP本od使ction.輸入isto本icalContext = TEXT("工具製造是工業化的基礎");
    
    // 輸入資源
    ToolsP本od使ction.Inp使tReso使本ces.Add(EReso使本ceType::Metal, 4.0f);
    ToolsP本od使ction.Inp使tReso使本ces.Add(EReso使本ceType::基本ood, 3.0f);
    
    // 輸資源
    ToolsP本od使ction.O使tp使tReso使本ces.Add(EReso使本ceType::Tools, 15.0f);
    
    P本od使ctionRecipes.Add(ToolsP本od使ction.RecipeID, ToolsP本od使ction);
}

正oid UMin成GoRTSP本od使ctionSyste設置::C本eateA成本ic使lt使本alP本od使ctionRecipes()
{
    // 食品加工配方
    軍P本od使ctionRecipe 軍oodP本ocessin成;
    軍oodP本ocessin成.RecipeID = Gene本ateRecipeID(TEXT("軍oodP本ocessin成"), EP本od使ctionCate成o本y::A成本ic使lt使本al);
    軍oodP本ocessin成.Recipe的a設置e = TEXT("食品加工");
    軍oodP本ocessin成.Desc本iption = TEXT("加工農產品，延長保存時間並提高價值");
    軍oodP本ocessin成.Cate成o本y = EP本od使ctionCate成o本y::A成本ic使lt使本al;
    軍oodP本ocessin成.P本od使ctionTi設置e = 100.0f; // 1.67分鐘
    軍oodP本ocessin成.BaseEfficiency = 1.1f;
    軍oodP本ocessin成.MinSkillLe正el = 0;
    軍oodP本ocessin成.Defa使ltQ使ality = EP本od使ctionQ使ality::的o本設置al;
    軍oodP本ocessin成.bIsRepeatable = t本使e;
    軍oodP本ocessin成.輸入isto本icalContext = TEXT("食品加工是農業經濟的重要環節");
    
    // 輸入資源
    軍oodP本ocessin成.Inp使tReso使本ces.Add(EReso使本ceType::軍ood, 5.0f);
    軍oodP本ocessin成.Inp使tReso使本ces.Add(EReso使本ceType::Salt, 1.0f);
    
    // 輸資源
    軍oodP本ocessin成.O使tp使tReso使本ces.Add(EReso使本ceType::P本ocessed軍ood, 8.0f);
    
    P本od使ctionRecipes.Add(軍oodP本ocessin成.RecipeID, 軍oodP本ocessin成);
}

正oid UMin成GoRTSP本od使ctionSyste設置::C本eateMedicalP本od使ctionRecipes()
{
    // 藥品生產配方
    軍P本od使ctionRecipe MedicineP本od使ction;
    MedicineP本od使ction.RecipeID = Gene本ateRecipeID(TEXT("MedicineP本od使ction"), EP本od使ctionCate成o本y::Medical);
    MedicineP本od使ction.Recipe的a設置e = TEXT("藥品生產");
    MedicineP本od使ction.Desc本iption = TEXT("生產基礎藥品，改善公共衛生");
    MedicineP本od使ction.Cate成o本y = EP本od使ctionCate成o本y::Medical;
    MedicineP本od使ction.P本od使ctionTi設置e = 240.0f; // 4分鐘
    MedicineP本od使ction.BaseEfficiency = 0.8f;
    MedicineP本od使ction.MinSkillLe正el = 2;
    MedicineP本od使ction.Defa使ltQ使ality = EP本od使ctionQ使ality::的o本設置al;
    MedicineP本od使ction.bIsRepeatable = t本使e;
    MedicineP本od使ction.輸入isto本icalContext = TEXT("民國時期，西方醫學的引進改善了公共衛生");
    
    // 輸入資源
    MedicineP本od使ction.Inp使tReso使本ces.Add(EReso使本ceType::輸入e本bs, 4.0f);
    MedicineP本od使ction.Inp使tReso使本ces.Add(EReso使本ceType::Alcohol, 2.0f);
    
    // 輸資源
    MedicineP本od使ction.O使tp使tReso使本ces.Add(EReso使本ceType::Medicine, 10.0f);
    
    P本od使ctionRecipes.Add(MedicineP本od使ction.RecipeID, MedicineP本od使ction);
}

正oid UMin成GoRTSP本od使ctionSyste設置::C本eateEd使cationalP本od使ctionRecipes()
{
    // 書籍印刷配方
    軍P本od使ctionRecipe BookP本intin成;
    BookP本intin成.RecipeID = Gene本ateRecipeID(TEXT("BookP本intin成"), EP本od使ctionCate成o本y::Ed使cational);
    BookP本intin成.Recipe的a設置e = TEXT("書籍印刷");
    BookP本intin成.Desc本iption = TEXT("印刷書籍，傳播知識和教育");
    BookP本intin成.Cate成o本y = EP本od使ctionCate成o本y::Ed使cational;
    BookP本intin成.P本od使ctionTi設置e = 300.0f; // 5分鐘
    BookP本intin成.BaseEfficiency = 0.7f;
    BookP本intin成.MinSkillLe正el = 2;
    BookP本intin成.Defa使ltQ使ality = EP本od使ctionQ使ality::Good;
    BookP本intin成.bIsRepeatable = t本使e;
    BookP本intin成.輸入isto本icalContext = TEXT("印刷業的發展促進了知識傳播和教育普及");
    
    // 輸入資源
    BookP本intin成.Inp使tReso使本ces.Add(EReso使本ceType::Pape本, 10.0f);
    BookP本intin成.Inp使tReso使本ces.Add(EReso使本ceType::Ink, 2.0f);
    
    // 輸資源
    BookP本intin成.O使tp使tReso使本ces.Add(EReso使本ceType::Books, 25.0f);
    
    P本od使ctionRecipes.Add(BookP本intin成.RecipeID, BookP本intin成);
}

正oid UMin成GoRTSP本od使ctionSyste設置::C本eateInf本ast本使ct使本eP本od使ctionRecipes()
{
    // 建材生產配方
    軍P本od使ctionRecipe B使ildin成Mate本ials;
    B使ildin成Mate本ials.RecipeID = Gene本ateRecipeID(TEXT("B使ildin成Mate本ials"), EP本od使ctionCate成o本y::Inf本ast本使ct使本e);
    B使ildin成Mate本ials.Recipe的a設置e = TEXT("建材生產");
    B使ildin成Mate本ials.Desc本iption = TEXT("生產建築材料，支持基礎設施建設");
    B使ildin成Mate本ials.Cate成o本y = EP本od使ctionCate成o本y::Inf本ast本使ct使本e;
    B使ildin成Mate本ials.P本od使ctionTi設置e = 250.0f; // 4.17分鐘
    B使ildin成Mate本ials.BaseEfficiency = 1.0f;
    B使ildin成Mate本ials.MinSkillLe正el = 1;
    B使ildin成Mate本ials.Defa使ltQ使ality = EP本od使ctionQ使ality::的o本設置al;
    B使ildin成Mate本ials.bIsRepeatable = t本使e;
    B使ildin成Mate本ials.輸入isto本icalContext = TEXT("建築材料是城市化和基礎設施建設的基礎");
    
    // 輸入資源
    B使ildin成Mate本ials.Inp使tReso使本ces.Add(EReso使本ceType::Stone, 8.0f);
    B使ildin成Mate本ials.Inp使tReso使本ces.Add(EReso使本ceType::Ce設置ent, 4.0f);
    
    // 輸資源
    B使ildin成Mate本ials.O使tp使tReso使本ces.Add(EReso使本ceType::B使ildin成Mate本ials, 30.0f);
    
    P本od使ctionRecipes.Add(B使ildin成Mate本ials.RecipeID, B使ildin成Mate本ials);
}

正oid UMin成GoRTSP本od使ctionSyste設置::C本eateL使x使本yP本od使ctionRecipes()
{
    // 茶葉加工配方
    軍P本od使ctionRecipe TeaP本ocessin成;
    TeaP本ocessin成.RecipeID = Gene本ateRecipeID(TEXT("TeaP本ocessin成"), EP本od使ctionCate成o本y::L使x使本y);
    TeaP本ocessin成.Recipe的a設置e = TEXT("茶葉加工");
    TeaP本ocessin成.Desc本iption = TEXT("加工高品質茶葉，生產奢侈品茶");
    TeaP本ocessin成.Cate成o本y = EP本od使ctionCate成o本y::L使x使本y;
    TeaP本ocessin成.P本od使ctionTi設置e = 180.0f; // 3分鐘
    TeaP本ocessin成.BaseEfficiency = 0.9f;
    TeaP本ocessin成.MinSkillLe正el = 2;
    TeaP本ocessin成.Defa使ltQ使ality = EP本od使ctionQ使ality::Good;
    TeaP本ocessin成.bIsRepeatable = t本使e;
    TeaP本ocessin成.輸入isto本icalContext = TEXT("茶葉是中國重要的傳統口商品");
    
    // 輸入資源
    TeaP本ocessin成.Inp使tReso使本ces.Add(EReso使本ceType::TeaLea正es, 6.0f);
    
    // 輸資源
    TeaP本ocessin成.O使tp使tReso使本ces.Add(EReso使本ceType::Tea, 20.0f);
    
    P本od使ctionRecipes.Add(TeaP本ocessin成.RecipeID, TeaP本ocessin成);
}

正oid UMin成GoRTSP本od使ctionSyste設置::輸入andleP本od使ctionCo設置pleted(const 軍St本in成& P本od使ctionLineID)
{
    if (!P本od使ctionLines.Contains(P本od使ctionLineID))
    {
        本et使本n;
    }

    軍P本od使ctionLine& P本od使ctionLine = P本od使ctionLines[P本od使ctionLineID];
    軍P本od使ctionRecipe Recipe = P本od使ctionRecipes[P本od使ctionLine.C使本本entRecipeID];
    
    // 消耗輸入資源
    if (ResourceSystem)
    {
        for (const auto& ResourcePair : Recipe.InputResources)
        {
            EResourceType ResourceType = ResourcePair.Key;
            float RequiredAmount = ResourcePair.Value;
            
            // Verify sufficient resources before deduction
            float CurrentAmount = ResourceSystem->GetResourceAmount(ResourceType);
            if (CurrentAmount >= RequiredAmount)
            {
                // Deduct input resources from storage
                ResourceSystem->RemoveResource(ResourceType, RequiredAmount);
                
                // Log resource consumption for analytics
                UE_LOG(LogTemp, Log, TEXT("Production consumed: %.1f %s (Recipe: %s)"),
                    RequiredAmount, *UEnum::GetValueAsString(ResourceType), *Recipe.RecipeName);
            }
            else
            {
                // Handle partial resource availability gracefully
                if (CurrentAmount > 0)
                {
                    ResourceSystem->RemoveResource(ResourceType, CurrentAmount);
                    UE_LOG(LogTemp, Warning, TEXT("Partial resource consumption: %.1f/%.1f %s (Recipe: %s)"),
                        CurrentAmount, RequiredAmount, *UEnum::GetValueAsString(ResourceType), *Recipe.RecipeName);
                }
                
                // Trigger resource shortage events if needed
                OnResourceShortage.Broadcast(ResourceType, RequiredAmount, CurrentAmount);
            }
        }
    }
    
    // 生產輸資源
    if (ResourceSystem)
    {
        for (const auto& ResourcePair : Recipe.OutputResources)
        {
            EResourceType ResourceType = ResourcePair.Key;
            float BaseOutputAmount = ResourcePair.Value;
            
            // Apply quality modifiers to output quantities
            float QualityMultiplier = GetQualityMultiplier(ProductionLine.CurrentQuality);
            float FinalOutputAmount = BaseOutputAmount * QualityMultiplier * ProductionLine.Efficiency;
            
            // Add output resources to storage
            float AddedAmount = ResourceSystem->AddResource(ResourceType, FinalOutputAmount);
            
            // Handle storage capacity limits
            if (AddedAmount < FinalOutputAmount)
            {
                float OverflowAmount = FinalOutputAmount - AddedAmount;
                
                // Trigger resource overflow warnings if needed
                OnResourceOverflow.Broadcast(ResourceType, OverflowAmount, ResourceSystem->GetStorageCapacity(ResourceType));
                UE_LOG(LogTemp, Warning, TEXT("Resource overflow for %s: %.1f units lost due to capacity limits"),
                    *UEnum::GetValueAsString(ResourceType), OverflowAmount);
            }
            
            // Log production output for analytics and reporting
            UE_LOG(LogTemp, Log, TEXT("Production output: %.1f %s (Quality: %s, Recipe: %s)"),
                AddedAmount, *UEnum::GetValueAsString(ResourceType), 
                *UEnum::GetValueAsString(ProductionLine.CurrentQuality), *Recipe.RecipeName);
        }
    }
    
    // 更新統計
    P本od使ctionLine.P本od使ctionCo使nt++;
    Statistics.TotalP本od使ctionR使ns++;
    Statistics.S使ccessf使lR使ns++;
    Statistics.LastP本od使ctionTi設置e = 軍DateTi設置e::的ow();
    
    fo本 (const a使to& Reso使本cePai本 : Recipe.O使tp使tReso使本ces)
    {
        Statistics.TotalP本od使ced.軍indO本Add(Reso使本cePai本.Key) += Reso使本cePai本.Val使e;
        Statistics.Cate成o本yP本od使ction.軍indO本Add(Recipe.Cate成o本y) += Reso使本cePai本.Val使e;
    }
    
    fo本 (const a使to& Reso使本cePai本 : Recipe.Inp使tReso使本ces)
    {
        Statistics.TotalCons使設置ed.軍indO本Add(Reso使本cePai本.Key) += Reso使本cePai本.Val使e;
    }
    
    // 廣播完成事件
    OnP本od使ctionCo設置pleted.B本oadcast(P本od使ctionLineID, Recipe.O使tp使tReso使本ces);
    
    // 處理生產隊列
    P本ocessP本od使ctionQ使e使e(P本od使ctionLineID);
    
    // 重置生產線狀態
    P本od使ctionLine.Stat使s = EP本od使ctionStat使s::Co設置pleted;
    P本od使ctionLine.C使本本entRecipeID = TEXT("");
    P本od使ctionLine.P本o成本ess = 0.0f;
    P本od使ctionLine.Ti設置eRe設置ainin成 = 0.0f;
    
    Reco本dP本od使ction輸入isto本y(軍St本in成::P本intf(TEXT("完成生產：%s - %s"), *P本od使ctionLine.P本od使ctionLine的a設置e, *Recipe.Recipe的a設置e));
}

正oid UMin成GoRTSP本od使ctionSyste設置::輸入andleP本od使ction軍ailed(const 軍St本in成& P本od使ctionLineID, const 軍St本in成& E本本o本Messa成e)
{
    if (!P本od使ctionLines.Contains(P本od使ctionLineID))
    {
        本et使本n;
    }

    軍P本od使ctionLine& P本od使ctionLine = P本od使ctionLines[P本od使ctionLineID];
    
    // 更新統計
    Statistics.TotalP本od使ctionR使ns++;
    Statistics.軍ailedR使ns++;
    
    // 重置生產線狀態
    P本od使ctionLine.Stat使s = EP本od使ctionStat使s::軍ailed;
    P本od使ctionLine.C使本本entRecipeID = TEXT("");
    P本od使ctionLine.P本o成本ess = 0.0f;
    P本od使ctionLine.Ti設置eRe設置ainin成 = 0.0f;
    
    // 廣播失敗事件
    OnP本od使ction軍ailed.B本oadcast(P本od使ctionLineID, E本本o本Messa成e);
    
    Reco本dP本od使ction輸入isto本y(軍St本in成::P本intf(TEXT("生產失敗：%s - %s"), *P本od使ctionLine.P本od使ctionLine的a設置e, *E本本o本Messa成e));
}

正oid UMin成GoRTSP本od使ctionSyste設置::UpdateP本od使ctionLineStat使s(const 軍St本in成& P本od使ctionLineID)
{
    if (!P本od使ctionLines.Contains(P本od使ctionLineID))
    {
        本et使本n;
    }

    軍P本od使ctionLine& P本od使ctionLine = P本od使ctionLines[P本od使ctionLineID];
    
    // 根據條件更新狀態
    if (P本od使ctionLine.Stat使s == EP本od使ctionStat使s::P本od使cin成 && P本od使ctionLine.Ti設置eRe設置ainin成 <= 0.0f)
    {
        P本od使ctionLine.Stat使s = EP本od使ctionStat使s::Co設置pleted;
    }
}

正oid UMin成GoRTSP本od使ctionSyste設置::P本ocessP本od使ctionQ使e使e(const 軍St本in成& P本od使ctionLineID)
{
    軍St本in成 Q使e使eID = 軍St本in成::P本intf(TEXT("QUEUE下%s"), *P本od使ctionLineID);
    if (!P本od使ctionQ使e使es.Contains(Q使e使eID))
    {
        本et使本n;
    }

    軍P本od使ctionQ使e使e& Q使e使e = P本od使ctionQ使e使es[Q使e使eID];
    
    // 如果隊列為空，設置為空閒
    if (Q使e使e.RecipeQ使e使e.的使設置() == 0)
    {
        if (軍P本od使ctionLine* P本od使ctionLine = P本od使ctionLines.軍ind(P本od使ctionLineID))
        {
            P本od使ctionLine->Stat使s = EP本od使ctionStat使s::Idle;
        }
        本et使本n;
    }
    
    // 如果是循環隊列，移動到下一個
    if (Q使e使e.bIsLoopin成)
    {
        Q使e使e.C使本本entIndex = (Q使e使e.C使本本entIndex + 1) % Q使e使e.RecipeQ使e使e.的使設置();
    }
    else
    {
        // 移除已完成的配方
        if (Q使e使e.C使本本entIndex < Q使e使e.RecipeQ使e使e.的使設置())
        {
            Q使e使e.RecipeQ使e使e.Re設置o正eAt(Q使e使e.C使本本entIndex);
            if (Q使e使e.C使本本entIndex >= Q使e使e.RecipeQ使e使e.的使設置())
            {
                Q使e使e.C使本本entIndex = 0;
            }
        }
    }
    
    // 開始下一個生產
    if (Q使e使e.RecipeQ使e使e.的使設置() > 0 && Q使e使e.bA使toSta本t)
    {
        軍St本in成 的extRecipeID = Q使e使e.RecipeQ使e使e[Q使e使e.C使本本entIndex];
        Sta本tP本od使ction(P本od使ctionLineID, 的extRecipeID);
    }
}

正oid UMin成GoRTSP本od使ctionSyste設置::ApplyEfficiency軍acto本s(const 軍St本in成& P本od使ctionLineID)
{
    if (!P本od使ctionLines.Contains(P本od使ctionLineID))
    {
        本et使本n;
    }

    軍P本od使ctionLine& P本od使ctionLine = P本od使ctionLines[P本od使ctionLineID];
    P本od使ctionLine.C使本本entEfficiency = Calc使late軍inalEfficiency(P本od使ctionLine);
}

float UMin成GoRTSP本od使ctionSyste設置::Calc使late軍inalEfficiency(const 軍P本od使ctionLine& P本od使ctionLine) const
{
    float 軍inalEfficiency = 1.0f;
    
    // 應用所有效率因子
    fo本 (const a使to& 軍acto本Pai本 : P本od使ctionLine.Efficiency軍acto本s)
    {
        軍inalEfficiency *= 軍acto本Pai本.Val使e;
    }
    
    // 限制最大效率
    float MaxEfficiency = Confi成.MaxO正e本clockEfficiency;
    軍inalEfficiency = 軍Math::Cla設置p(軍inalEfficiency, 0.1f, MaxEfficiency);
    
    本et使本n 軍inalEfficiency;
}

軍St本in成 UMin成GoRTSP本od使ctionSyste設置::GetP本od使ctionCate成o本y的a設置e(EP本od使ctionCate成o本y Cate成o本y) const
{
    const UEn使設置* En使設置Pt本 = StaticEn使設置<EP本od使ctionCate成o本y>();
    if (En使設置Pt本)
    {
        本et使本n En使設置Pt本->Get的a設置eSt本in成ByVal使e(static下cast<int64>(Cate成o本y));
    }
    本et使本n TEXT("Unknown");
}

軍St本in成 UMin成GoRTSP本od使ctionSyste設置::GetP本od使ctionStat使s的a設置e(EP本od使ctionStat使s Stat使s) const
{
    const UEn使設置* En使設置Pt本 = StaticEn使設置<EP本od使ctionStat使s>();
    if (En使設置Pt本)
    {
        本et使本n En使設置Pt本->Get的a設置eSt本in成ByVal使e(static下cast<int64>(Stat使s));
    }
    本et使本n TEXT("Unknown");
}

bool UMin成GoRTSP本od使ctionSyste設置::ValidateRecipeData(const 軍P本od使ctionRecipe& Recipe) const
{
    // 基本驗證
    if (Recipe.RecipeID.IsE設置pty()  Recipe.Recipe的a設置e.IsE設置pty())
    {
        本et使本n false;
    }
    
    if (Recipe.P本od使ctionTi設置e <= 0.0f)
    {
        本et使本n false;
    }
    
    if (Recipe.Inp使tReso使本ces.的使設置() == 0  Recipe.O使tp使tReso使本ces.的使設置() == 0)
    {
        本et使本n false;
    }
    
    本et使本n t本使e;
}

軍St本in成 UMin成GoRTSP本od使ctionSyste設置::Gene本ateP本od使ctionLineID(const 軍St本in成& Base的a設置e) const
{
    本et使本n 軍St本in成::P本intf(TEXT("PROD下LI的E下%s下%d"), *Base的a設置e, 軍Math::RandRan成e(1000, 9999));
}

軍St本in成 UMin成GoRTSP本od使ctionSyste設置::Gene本ateRecipeID(const 軍St本in成& Base的a設置e, EP本od使ctionCate成o本y Cate成o本y) const
{
    軍St本in成 Cate成o本y的a設置e = GetP本od使ctionCate成o本y的a設置e(Cate成o本y);
    本et使本n 軍St本in成::P本intf(TEXT("RECIPE下%s下%s下%d"), *Cate成o本y的a設置e, *Base的a設置e, 軍Math::RandRan成e(1000, 9999));
}

正oid UMin成GoRTSP本od使ctionSyste設置::Reco本dP本od使ction輸入isto本y(const 軍St本in成& E正ent)
{
    P本od使ction輸入isto本y.Add(E正ent);
    
    // 限制歷史記錄數量
    if (P本od使ction輸入isto本y.的使設置() > 1000)
    {
        P本od使ction輸入isto本y.Re設置o正eAt(0);
    }
