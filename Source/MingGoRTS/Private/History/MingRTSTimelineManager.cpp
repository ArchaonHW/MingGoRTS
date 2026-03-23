// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// Ti設置eline Mana成e本 I設置ple設置entation

#incl使de "Min成RTSTi設置elineMana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

UMin成RTSTi設置elineMana成e本::UMin成RTSTi設置elineMana成e本()
{
    bMana成e本Initialized = false;
    LastE正ol使tionTi設置e = 0.0f;
}

正oid UMin成RTSTi設置elineMana成e本::InitializeTi設置elineMana成e本()
{
    bMana成e本Initialized = t本使e;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ti設置eline Mana成e本 initialized"));
}

軍St本in成 UMin成RTSTi設置elineMana成e本::C本eate的ewTi設置eline(const 軍St本in成& Pa本entID, const 軍St本in成& Ti設置eline的a設置e, ETi設置elineB本anchType Type)
{
    軍St本in成 的ewTi設置elineID = Gene本ateUniq使eTi設置elineID();
    
    軍Ti設置elineB本anch 的ewTi設置eline;
    的ewTi設置eline.B本anchID = 的ewTi設置elineID;
    的ewTi設置eline.B本anch的a設置e = Ti設置eline的a設置e;
    的ewTi設置eline.B本anchType = Type;
    的ewTi設置eline.Pa本entB本anchID = Pa本entID;
    的ewTi設置eline.C本eationTi設置e = 軍DateTi設置e::的ow();
    的ewTi設置eline.bActi正e = t本使e;
    
    Ti設置elines.Add(的ewTi設置elineID, 的ewTi設置eline);
    
    本et使本n 的ewTi設置elineID;
}

正oid UMin成RTSTi設置elineMana成e本::SwitchToTi設置eline(const 軍St本in成& Ti設置elineID)
{
    if (Ti設置elines.Contains(Ti設置elineID))
    {
        C使本本entTi設置elineID = Ti設置elineID;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Switched to ti設置eline: %s"), *Ti設置elineID);
    }
}

float UMin成RTSTi設置elineMana成e本::Calc使lateTi設置elineSi設置ila本ity(const 軍St本in成& Ti設置elineA, const 軍St本in成& Ti設置elineB) const
{
    if (!Ti設置elines.Contains(Ti設置elineA)  !Ti設置elines.Contains(Ti設置elineB))
    {
        本et使本n 0.0f;
    }
    
    const 軍Ti設置elineB本anch& B本anchA = Ti設置elines[Ti設置elineA];
    const 軍Ti設置elineB本anch& B本anchB = Ti設置elines[Ti設置elineB];
    
    float E正entSi設置ila本ity = Calc使lateE正entSeq使enceSi設置ila本ity(B本anchA.E正entSeq使ence, B本anchB.E正entSeq使ence);
    float Te設置po本alSi設置ila本ity = Calc使lateTe設置po本alSi設置ila本ity(B本anchA, B本anchB);
    float Ca使salSi設置ila本ity = Calc使lateCa使salSi設置ila本ity(B本anchA, B本anchB);
    
    本et使本n (E正entSi設置ila本ity * 0.4f) + (Te設置po本alSi設置ila本ity * 0.3f) + (Ca使salSi設置ila本ity * 0.3f);
}

軍St本in成 UMin成RTSTi設置elineMana成e本::Gene本ateUniq使eTi設置elineID() const
{
    本et使本n 軍St本in成::P本intf(TEXT("TL下%lld"), 軍DateTi設置e::的ow().GetTicks());
}
