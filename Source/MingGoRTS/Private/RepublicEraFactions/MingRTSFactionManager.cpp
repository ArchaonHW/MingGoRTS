// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Republic Era Factions - Faction Manager Implementation

#include "RepublicEraFactions/MingRTSFactionManager.h"
#include "RepublicEraFactions/MingRTSFactionBase.h"
#include "RepublicEraFactions/Factions/BeiyangGovernment.h"
#include "RepublicEraFactions/Factions/NationalistGovernment.h"
#include "RepublicEraFactions/Factions/CommunistParty.h"
#include "RepublicEraFactions/Factions/FengtianClique.h"
#include "RepublicEraFactions/Factions/ZhiliClique.h"
#include "RepublicEraFactions/Factions/AnhuiClique.h"
#include "RepublicEraFactions/Factions/ShanxiClique.h"
#include "RepublicEraFactions/Factions/GuangxiClique.h"
#include "RepublicEraFactions/Factions/YunnanClique.h"
#include "RepublicEraFactions/Factions/SichuanClique.h"
#include "RepublicEraFactions/Factions/MaFamilyForces.h"
#include "RepublicEraFactions/Factions/XinjiangForces.h"

DEFINE_LOG_CATEGORY_STATIC(LogMingRTSFactionManager, Log, All);

UMingRTSFactionManager::UMingRTSFactionManager()
{
}

void UMingRTSFactionManager::InitializeFactionManager()
{
    UE_LOG(LogMingRTSFactionManager, Log, TEXT("Initializing Faction Manager..."));
    
    RegisterAllFactions();
    
    UE_LOG(LogMingRTSFactionManager, Log, TEXT("Faction Manager initialized with %d factions"), 
        GetFactionCount());
}

void UMingRTSFactionManager::RegisterAllFactions()
{
    RegisteredFactions.Empty();
    
    RegisterBeiyangGovernment();
    RegisterNationalistGovernment();
    RegisterCommunistParty();
    RegisterFengtianClique();
    RegisterZhiliClique();
    RegisterAnhuiClique();
    RegisterShanxiClique();
    RegisterGuangxiClique();
    RegisterYunnanClique();
    RegisterSichuanClique();
    RegisterMaFamilyForces();
    RegisterXinjiangForces();
    
    UE_LOG(LogMingRTSFactionManager, Log, TEXT("Registered %d Republic Era factions"), 
        RegisteredFactions.Num());
}

void UMingRTSFactionManager::RegisterBeiyangGovernment()
{
    UBeiyangGovernment* Faction = NewObject<UBeiyangGovernment>(this);
    if (Faction)
    {
        Faction->InitializeFaction();
        RegisteredFactions.Add(FName("BeiyangGovernment"), Faction);
        UE_LOG(LogMingRTSFactionManager, Log, TEXT("Registered: Beiyang Government"));
    }
}

void UMingRTSFactionManager::RegisterNationalistGovernment()
{
    UNationalistGovernment* Faction = NewObject<UNationalistGovernment>(this);
    if (Faction)
    {
        Faction->InitializeFaction();
        RegisteredFactions.Add(FName("NationalistGovernment"), Faction);
        UE_LOG(LogMingRTSFactionManager, Log, TEXT("Registered: Nationalist Government"));
    }
}

void UMingRTSFactionManager::RegisterCommunistParty()
{
    UCommunistParty* Faction = NewObject<UCommunistParty>(this);
    if (Faction)
    {
        Faction->InitializeFaction();
        RegisteredFactions.Add(FName("CommunistParty"), Faction);
        UE_LOG(LogMingRTSFactionManager, Log, TEXT("Registered: Communist Party"));
    }
}

void UMingRTSFactionManager::RegisterFengtianClique()
{
    UFengtianClique* Faction = NewObject<UFengtianClique>(this);
    if (Faction)
    {
        Faction->InitializeFaction();
        RegisteredFactions.Add(FName("FengtianClique"), Faction);
        UE_LOG(LogMingRTSFactionManager, Log, TEXT("Registered: Fengtian Clique"));
    }
}

void UMingRTSFactionManager::RegisterZhiliClique()
{
    UZhiliClique* Faction = NewObject<UZhiliClique>(this);
    if (Faction)
    {
        Faction->InitializeFaction();
        RegisteredFactions.Add(FName("ZhiliClique"), Faction);
        UE_LOG(LogMingRTSFactionManager, Log, TEXT("Registered: Zhili Clique"));
    }
}

void UMingRTSFactionManager::RegisterAnhuiClique()
{
    UAnhuiClique* Faction = NewObject<UAnhuiClique>(this);
    if (Faction)
    {
        Faction->InitializeFaction();
        RegisteredFactions.Add(FName("AnhuiClique"), Faction);
        UE_LOG(LogMingRTSFactionManager, Log, TEXT("Registered: Anhui Clique"));
    }
}

void UMingRTSFactionManager::RegisterShanxiClique()
{
    UShanxiClique* Faction = NewObject<UShanxiClique>(this);
    if (Faction)
    {
        Faction->InitializeFaction();
        RegisteredFactions.Add(FName("ShanxiClique"), Faction);
        UE_LOG(LogMingRTSFactionManager, Log, TEXT("Registered: Shanxi Clique"));
    }
}

void UMingRTSFactionManager::RegisterGuangxiClique()
{
    UGuangxiClique* Faction = NewObject<UGuangxiClique>(this);
    if (Faction)
    {
        Faction->InitializeFaction();
        RegisteredFactions.Add(FName("GuangxiClique"), Faction);
        UE_LOG(LogMingRTSFactionManager, Log, TEXT("Registered: Guangxi Clique"));
    }
}

void UMingRTSFactionManager::RegisterYunnanClique()
{
    UYunnanClique* Faction = NewObject<UYunnanClique>(this);
    if (Faction)
    {
        Faction->InitializeFaction();
        RegisteredFactions.Add(FName("YunnanClique"), Faction);
        UE_LOG(LogMingRTSFactionManager, Log, TEXT("Registered: Yunnan Clique"));
    }
}

void UMingRTSFactionManager::RegisterSichuanClique()
{
    USichuanClique* Faction = NewObject<USichuanClique>(this);
    if (Faction)
    {
        Faction->InitializeFaction();
        RegisteredFactions.Add(FName("SichuanClique"), Faction);
        UE_LOG(LogMingRTSFactionManager, Log, TEXT("Registered: Sichuan Clique"));
    }
}

void UMingRTSFactionManager::RegisterMaFamilyForces()
{
    UMaFamilyForces* Faction = NewObject<UMaFamilyForces>(this);
    if (Faction)
    {
        Faction->InitializeFaction();
        RegisteredFactions.Add(FName("MaFamilyForces"), Faction);
        UE_LOG(LogMingRTSFactionManager, Log, TEXT("Registered: Ma Family Forces"));
    }
}

void UMingRTSFactionManager::RegisterXinjiangForces()
{
    UXinjiangForces* Faction = NewObject<UXinjiangForces>(this);
    if (Faction)
    {
        Faction->InitializeFaction();
        RegisteredFactions.Add(FName("XinjiangForces"), Faction);
        UE_LOG(LogMingRTSFactionManager, Log, TEXT("Registered: Xinjiang Forces"));
    }
}

UMingRTSFactionBase* UMingRTSFactionManager::GetFactionByID(const FName& FactionID) const
{
    const TObjectPtr<UMingRTSFactionBase>* FoundFaction = RegisteredFactions.Find(FactionID);
    return FoundFaction ? FoundFaction->Get() : nullptr;
}

TArray<UMingRTSFactionBase*> UMingRTSFactionManager::GetAllFactions() const
{
    TArray<UMingRTSFactionBase*> Result;
    for (const auto& Pair : RegisteredFactions)
    {
        if (Pair.Value)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

TArray<UMingRTSFactionBase*> UMingRTSFactionManager::GetFactionsByDifficulty(EFactionDifficulty Difficulty) const
{
    TArray<UMingRTSFactionBase*> Result;
    for (const auto& Pair : RegisteredFactions)
    {
        if (Pair.Value && Pair.Value->GetFactionAttributes().Difficulty == Difficulty)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

bool UMingRTSFactionManager::AreAllFactionsRegistered() const
{
    return RegisteredFactions.Num() == 12;
}

UMingRTSFactionBase* UMingRTSFactionManager::GetRecommendedFactionForNewPlayers() const
{
    // Beiyang Government is recommended for new players (Easy difficulty)
    return GetFactionByID(FName("BeiyangGovernment"));
}

bool UMingRTSFactionManager::ValidateAllFactions() const
{
    bool bValid = true;
    
    for (const auto& Pair : RegisteredFactions)
    {
        if (!Pair.Value)
        {
            UE_LOG(LogMingRTSFactionManager, Warning, TEXT("Faction %s is null"), *Pair.Key.ToString());
            bValid = false;
            continue;
        }
        
        FFactionAttributes Attrs = Pair.Value->GetFactionAttributes();
        if (Attrs.FactionID.IsNone())
        {
            UE_LOG(LogMingRTSFactionManager, Warning, TEXT("Faction %s has invalid ID"), *Pair.Key.ToString());
            bValid = false;
        }
        
        if (Pair.Value->GetUniqueUnits().Num() == 0)
        {
            UE_LOG(LogMingRTSFactionManager, Warning, TEXT("Faction %s has no unique units"), *Pair.Key.ToString());
            bValid = false;
        }
        
        if (Pair.Value->GetUniqueMechanics().Num() == 0)
        {
            UE_LOG(LogMingRTSFactionManager, Warning, TEXT("Faction %s has no unique mechanics"), *Pair.Key.ToString());
            bValid = false;
        }
    }
    
    return bValid;
}

FString UMingRTSFactionManager::GetFactionBalanceReport() const
{
    FString Report = TEXT("=== Republic Era Factions Balance Report ===\n\n");
    
    // Count by difficulty
    int32 VeryEasyCount = GetFactionsByDifficulty(EFactionDifficulty::VeryEasy).Num();
    int32 EasyCount = GetFactionsByDifficulty(EFactionDifficulty::Easy).Num();
    int32 NormalCount = GetFactionsByDifficulty(EFactionDifficulty::Normal).Num();
    int32 HardCount = GetFactionsByDifficulty(EFactionDifficulty::Hard).Num();
    int32 VeryHardCount = GetFactionsByDifficulty(EFactionDifficulty::VeryHard).Num();
    
    Report += FString::Printf(TEXT("Difficulty Distribution:\n"));
    Report += FString::Printf(TEXT("  Very Easy: %d\n"), VeryEasyCount);
    Report += FString::Printf(TEXT("  Easy: %d\n"), EasyCount);
    Report += FString::Printf(TEXT("  Normal: %d\n"), NormalCount);
    Report += FString::Printf(TEXT("  Hard: %d\n"), HardCount);
    Report += FString::Printf(TEXT("  Very Hard: %d\n\n"), VeryHardCount);
    
    // Total units and mechanics
    Report += FString::Printf(TEXT("Total Unique Units: %d\n"), GetTotalUniqueUnitsCount());
    Report += FString::Printf(TEXT("Total Unique Mechanics: %d\n\n"), GetTotalUniqueMechanicsCount());
    
    // List all factions
    Report += TEXT("Registered Factions:\n");
    for (const auto& Pair : RegisteredFactions)
    {
        if (Pair.Value)
        {
            FFactionAttributes Attrs = Pair.Value->GetFactionAttributes();
            Report += FString::Printf(TEXT("  - %s (%s)\n"), 
                *Attrs.FactionName.ToString(),
                *Attrs.FactionNameEnglish.ToString());
        }
    }
    
    return Report;
}

int32 UMingRTSFactionManager::GetTotalUniqueUnitsCount() const
{
    int32 Total = 0;
    for (const auto& Pair : RegisteredFactions)
    {
        if (Pair.Value)
        {
            Total += Pair.Value->GetUniqueUnits().Num();
        }
    }
    return Total;
}

int32 UMingRTSFactionManager::GetTotalUniqueMechanicsCount() const
{
    int32 Total = 0;
    for (const auto& Pair : RegisteredFactions)
    {
        if (Pair.Value)
        {
            Total += Pair.Value->GetUniqueMechanics().Num();
        }
    }
    return Total;
}
