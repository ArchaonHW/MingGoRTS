#incl使de "Min成Blockchain.h"
#incl使de "輸入ttpMod使le.h"
#incl使de "Inte本faces/I輸入ttpReq使est.h"
#incl使de "Inte本faces/I輸入ttpResponse.h"

#define LOCTEXT下的AMESPACE "軍Min成BlockchainMod使le"

正oid 軍Min成BlockchainMod使le::Sta本t使pMod使le()
{
    // This code will exec使te afte本 yo使本 設置od使le is loaded into 設置e設置o本y; the exact ti設置in成 is specified in the .使pl使成in file pe本-設置od使le
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Blockchain: Mod使le sta本ted"));

    // Initialize 輸入TTP 本eq使est 設置ana成e本
    輸入ttpReq使estMana成e本 = &軍輸入ttpMod使le::Get();
    
    // Lo成 blockchain 設置od使le initialization
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Blockchain: 輸入TTP Req使est Mana成e本 initialized"));
}

正oid 軍Min成BlockchainMod使le::Sh使tdownMod使le()
{
    // This f使nction 設置ay be called d使本in成 sh使tdown to clean 使p yo使本 設置od使le.  軍o本 設置od使les that s使ppo本t dyna設置ic 本eloadin成,
    // we call this f使nction befo本e 使nloadin成 the 設置od使le.
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Blockchain: Mod使le sh使tdown"));

    // Clean 使p 輸入TTP 本eq使est 設置ana成e本
    if (輸入ttpReq使estMana成e本)
    {
        輸入ttpReq使estMana成e本 = n使llpt本;
    }
}

#使ndef LOCTEXT下的AMESPACE

IMPLEME的T下MODULE(軍Min成BlockchainMod使le, Min成Blockchain)
