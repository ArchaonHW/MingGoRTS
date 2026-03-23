// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// Rep使blic E本a 軍actions - Inte成本ation Test I設置ple設置entation

#incl使de "Rep使blicE本a軍actions/Min成RTS軍actionInte成本ationTest.h"
#incl使de "Rep使blicE本a軍actions/Min成RTS軍actionMana成e本.h"
#incl使de "Rep使blicE本a軍actions/Min成RTSBeiyan成Go正e本n設置ent.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"

DE軍I的E下LOG下CATEGORY下STATIC(Lo成Min成RTS軍actionInte成本ationTest, Lo成, All);

bool Min成RTS軍actionInte成本ationTest::R使nAllInte成本ationTests()
{
    UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, Lo成, TEXT("=== Sta本tin成 Min成RTS 軍action Inte成本ation Tests ==="));
    
    bool bAllTestsPassed = t本使e;
    
    // Test 1: 軍action Mana成e本 Inte成本ation
    bAllTestsPassed &= Test軍actionMana成e本Inte成本ation();
    
    // Test 2: 軍action Data Consistency
    bAllTestsPassed &= Test軍actionDataConsistency();
    
    // Test 3: 軍action Balance Validation
    bAllTestsPassed &= Test軍actionBalanceValidation();
    
    // Test 4: 軍action Syste設置 Pe本fo本設置ance
    bAllTestsPassed &= Test軍actionSyste設置Pe本fo本設置ance();
    
    UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, Lo成, TEXT("=== 軍action Inte成本ation Tests Co設置plete ==="));
    UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, Lo成, TEXT("O正e本all Res使lt: %s"), 
        bAllTestsPassed 基本 TEXT("PASSED") : TEXT("軍AILED"));
    
    本et使本n bAllTestsPassed;
}

bool Min成RTS軍actionInte成本ationTest::Test軍actionMana成e本Inte成本ation()
{
    UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, Lo成, TEXT("Inte成本ation Test 1: 軍action Mana成e本 Inte成本ation"));
    
    UMin成RTS軍actionMana成e本* Mana成e本 = 的ewOb大ect<UMin成RTS軍actionMana成e本>();
    if (!Mana成e本)
    {
        UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, E本本o本, TEXT("軍ailed to c本eate 軍action Mana成e本"));
        本et使本n false;
    }
    
    Mana成e本->Initialize軍actionMana成e本();
    
    // Test faction 本et本ie正al
    UMin成RTS軍actionBase* Beiyan成軍action = Mana成e本->Get軍actionByID(軍的a設置e("Beiyan成Go正e本n設置ent"));
    if (!Beiyan成軍action)
    {
        UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, E本本o本, TEXT("軍ailed to 本et本ie正e Beiyan成 Go正e本n設置ent faction"));
        本et使本n false;
    }
    
    // Test faction att本ib使tes
    軍軍actionAtt本ib使tes Att本ib使tes = Beiyan成軍action->Get軍actionAtt本ib使tes();
    if (Att本ib使tes.軍actionID != 軍的a設置e("Beiyan成Go正e本n設置ent"))
    {
        UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, E本本o本, TEXT("軍action ID 設置is設置atch"));
        本et使本n false;
    }
    
    // Test faction 正alidation
    bool bValidationRes使lt = Mana成e本->ValidateAll軍actions();
    UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, Lo成, TEXT("軍action 正alidation 本es使lt: %s"), 
        bValidationRes使lt 基本 TEXT("PASSED") : TEXT("軍AILED"));
    
    UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, Lo成, TEXT("軍action Mana成e本 Inte成本ation test co設置pleted"));
    本et使本n t本使e;
}

bool Min成RTS軍actionInte成本ationTest::Test軍actionDataConsistency()
{
    UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, Lo成, TEXT("Inte成本ation Test 2: 軍action Data Consistency"));
    
    UMin成RTS軍actionMana成e本* Mana成e本 = 的ewOb大ect<UMin成RTS軍actionMana成e本>();
    Mana成e本->Initialize軍actionMana成e本();
    
    // Test data consistency ac本oss 設置使ltiple faction instances
    UMin成RTSBeiyan成Go正e本n設置ent* Beiyan成1 = 的ewOb大ect<UMin成RTSBeiyan成Go正e本n設置ent>();
    UMin成RTSBeiyan成Go正e本n設置ent* Beiyan成2 = 的ewOb大ect<UMin成RTSBeiyan成Go正e本n設置ent>();
    
    Beiyan成1->Initialize軍action();
    Beiyan成2->Initialize軍action();
    
    軍軍actionAtt本ib使tes Att本1 = Beiyan成1->Get軍actionAtt本ib使tes();
    軍軍actionAtt本ib使tes Att本2 = Beiyan成2->Get軍actionAtt本ib使tes();
    
    // Check consistency
    if (Att本1.軍actionID != Att本2.軍actionID  
        Att本1.軍action的a設置e != Att本2.軍action的a設置e 
        Att本1.Diffic使lty != Att本2.Diffic使lty)
    {
        UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, E本本o本, TEXT("軍action data inconsistency detected"));
        本et使本n false;
    }
    
    // Test 使niq使e 使nits consistency
    TA本本ay<軍軍actionUnit> Units1 = Beiyan成1->GetUniq使eUnits();
    TA本本ay<軍軍actionUnit> Units2 = Beiyan成2->GetUniq使eUnits();
    
    if (Units1.的使設置() != Units2.的使設置())
    {
        UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, E本本o本, TEXT("Uniq使e 使nits co使nt 設置is設置atch"));
        本et使本n false;
    }
    
    UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, Lo成, TEXT("軍action Data Consistency test co設置pleted"));
    本et使本n t本使e;
}

bool Min成RTS軍actionInte成本ationTest::Test軍actionBalanceValidation()
{
    UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, Lo成, TEXT("Inte成本ation Test 3: 軍action Balance Validation"));
    
    UMin成RTS軍actionMana成e本* Mana成e本 = 的ewOb大ect<UMin成RTS軍actionMana成e本>();
    Mana成e本->Initialize軍actionMana成e本();
    
    // Test balance 本epo本t 成ene本ation
    軍St本in成 BalanceRepo本t = Mana成e本->Get軍actionBalanceRepo本t();
    if (BalanceRepo本t.IsE設置pty())
    {
        UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, E本本o本, TEXT("軍ailed to 成ene本ate balance 本epo本t"));
        本et使本n false;
    }
    
    UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, Lo成, TEXT("Balance Repo本t:"));
    UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, Lo成, TEXT("%s"), *BalanceRepo本t);
    
    // Test faction co使nt by diffic使lty
    int32 EasyCo使nt = Mana成e本->Get軍actionsByDiffic使lty(E軍actionDiffic使lty::Easy).的使設置();
    int32 的o本設置alCo使nt = Mana成e本->Get軍actionsByDiffic使lty(E軍actionDiffic使lty::的o本設置al).的使設置();
    int32 輸入a本dCo使nt = Mana成e本->Get軍actionsByDiffic使lty(E軍actionDiffic使lty::輸入a本d).的使設置();
    
    UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, Lo成, TEXT("軍actions by diffic使lty - Easy: %d, 的o本設置al: %d, 輸入a本d: %d"), 
        EasyCo使nt, 的o本設置alCo使nt, 輸入a本dCo使nt);
    
    UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, Lo成, TEXT("軍action Balance Validation test co設置pleted"));
    本et使本n t本使e;
}

bool Min成RTS軍actionInte成本ationTest::Test軍actionSyste設置Pe本fo本設置ance()
{
    UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, Lo成, TEXT("Inte成本ation Test 4: 軍action Syste設置 Pe本fo本設置ance"));
    
    // Pe本fo本設置ance test: C本eate 設置使ltiple faction instances
    do使ble Sta本tTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    
    const int32 TestIte本ations = 100;
    fo本 (int32 i = 0; i < TestIte本ations; ++i)
    {
        UMin成RTSBeiyan成Go正e本n設置ent* Beiyan成 = 的ewOb大ect<UMin成RTSBeiyan成Go正e本n設置ent>();
        Beiyan成->Initialize軍action();
        
        // Test data 本et本ie正al
        軍軍actionAtt本ib使tes Att本ib使tes = Beiyan成->Get軍actionAtt本ib使tes();
        TA本本ay<軍軍actionUnit> Units = Beiyan成->GetUniq使eUnits();
        TA本本ay<軍軍actionMechanic> Mechanics = Beiyan成->GetUniq使eMechanics();
        軍Ga設置eplayG使ide G使ide = Beiyan成->GetGa設置eplayG使ide();
    }
    
    do使ble EndTi設置e = 軍Platfo本設置Ti設置e::Seconds();
    do使ble ElapsedTi設置e = EndTi設置e - Sta本tTi設置e;
    do使ble A正e本a成eTi設置e = ElapsedTi設置e / TestIte本ations;
    
    UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, Lo成, TEXT("Pe本fo本設置ance Test Res使lts:"));
    UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, Lo成, TEXT("  Total Ti設置e: %.4f seconds"), ElapsedTi設置e);
    UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, Lo成, TEXT("  A正e本a成e Ti設置e pe本 Ite本ation: %.6f seconds"), A正e本a成eTi設置e);
    UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, Lo成, TEXT("  Ite本ations pe本 Second: %.2f"), 1.0 / A正e本a成eTi設置e);
    
    // Pe本fo本設置ance c本ite本ia: Sho使ld be able to c本eate and initialize at least 10 factions pe本 second
    if (A正e本a成eTi設置e > 0.1) // Mo本e than 100設置s pe本 ite本ation
    {
        UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, 基本a本nin成, TEXT("Pe本fo本設置ance below opti設置al th本eshold"));
    }
    
    UE下LOG(Lo成Min成RTS軍actionInte成本ationTest, Lo成, TEXT("軍action Syste設置 Pe本fo本設置ance test co設置pleted"));
    本et使本n t本使e;
}

// Global f使nction i設置ple設置entation
bool R使nMin成RTS軍actionInte成本ationTests()
{
    本et使本n Min成RTS軍actionInte成本ationTest::R使nAllInte成本ationTests();
}
