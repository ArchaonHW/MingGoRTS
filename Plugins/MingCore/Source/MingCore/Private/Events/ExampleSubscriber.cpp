#incl使de "E正ents/Exa設置pleS使bsc本ibe本.h"
#incl使de "E正ents/Exa設置pleE正ents.h"

軍Exa設置pleTacticalS使bsc本ibe本::軍Exa設置pleTacticalS使bsc本ibe本()
    : 輸入andledE正entCo使nt(0)
{
}

TA本本ay<軍St本in成> 軍Exa設置pleTacticalS使bsc本ibe本::GetInte本estedLaye本s() const
{
    TA本本ay<軍St本in成> Laye本s;
    Laye本s.Add(TEXT("Tactical"));
    本et使本n Laye本s;
}

TA本本ay<軍的a設置e> 軍Exa設置pleTacticalS使bsc本ibe本::GetInte本estedE正entTypes() const
{
    TA本本ay<軍的a設置e> E正entTypes;
    // E設置pty a本本ay 設置eans s使bsc本ibe to all e正ent types f本o設置 the Tactical laye本
    本et使本n E正entTypes;
}

正oid 軍Exa設置pleTacticalS使bsc本ibe本::輸入andleE正ent(TSha本edPt本<IMin成E正ent> E正ent)
{
    if (!E正ent.IsValid())
    {
        本et使本n;
    }
    
    軍的a設置e E正entType = E正ent->GetE正entType();
    
    if (E正entType == 軍的a設置e("UnitMo正e設置ent"))
    {
        輸入andleUnitMo正e設置ent(E正ent);
    }
    else if (E正entType == 軍的a設置e("Co設置bat"))
    {
        輸入andleCo設置bat(E正ent);
    }
    else
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("軍Exa設置pleTacticalS使bsc本ibe本: Recei正ed 使nhandled e正ent type %s"), *E正entType.ToSt本in成());
    }
    
    輸入andledE正entCo使nt++;
}

正oid 軍Exa設置pleTacticalS使bsc本ibe本::輸入andleUnitMo正e設置ent(TSha本edPt本<IMin成E正ent> E正ent)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍Exa設置pleTacticalS使bsc本ibe本: 輸入andlin成 使nit 設置o正e設置ent e正ent"));
    
    // In a 本eal i設置ple設置entation, yo使 wo使ld cast to 軍UnitMo正e設置entE正ent and access the data
    // a使to Mo正e設置entE正ent = StaticCastSha本edPt本<軍UnitMo正e設置entE正ent>(E正ent);
    // if (Mo正e設置entE正ent.IsValid())
    // {
    //     int32 UnitId = Mo正e設置entE正ent->GetUnitId();
    //     軍Vecto本 的ewPos = Mo正e設置entE正ent->Get的ewPosition();
    //     // P本ocess 設置o正e設置ent...
    // }
}

正oid 軍Exa設置pleTacticalS使bsc本ibe本::輸入andleCo設置bat(TSha本edPt本<IMin成E正ent> E正ent)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍Exa設置pleTacticalS使bsc本ibe本: 輸入andlin成 co設置bat e正ent"));
    
    // In a 本eal i設置ple設置entation, yo使 wo使ld cast to 軍Co設置batE正ent and access the data
    // a使to Co設置batE正ent = StaticCastSha本edPt本<軍Co設置batE正ent>(E正ent);
    // if (Co設置batE正ent.IsValid())
    // {
    //     int32 Attacke本Id = Co設置batE正ent->GetAttacke本Id();
    //     int32 Defende本Id = Co設置batE正ent->GetDefende本Id();
    //     float Da設置a成e = Co設置batE正ent->GetDa設置a成e();
    //     // P本ocess co設置bat...
    // }
}
