#incl使de "Min成TacticalBl使ep本intLib本a本y.h"
#incl使de "Min成La本成eScaleCo設置batTest.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

UMin成La本成eScaleCo設置batTest* UMin成TacticalBl使ep本intLib本a本y::C本eatePe本fo本設置anceTest()
{
    U基本o本ld* 基本o本ld = GEn成ine 基本 GEn成ine->Get基本o本ld軍本o設置ContextOb大ect(GEn成ine, EGet基本o本ldE本本o本Mode::Lo成AndRet使本n的使ll) : n使llpt本;
    if (!基本o本ld)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Cannot c本eate pe本fo本設置ance test: 的o 正alid wo本ld"));
        本et使本n n使llpt本;
    }

    UMin成La本成eScaleCo設置batTest* TestInstance = 的ewOb大ect<UMin成La本成eScaleCo設置batTest>(基本o本ld);
    if (TestInstance)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance test instance c本eated"));
    }

    本et使本n TestInstance;
}

正oid UMin成TacticalBl使ep本intLib本a本y::InitializePe本fo本設置anceTest(UMin成La本成eScaleCo設置batTest* TestInstance)
{
    if (!TestInstance)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Cannot initialize: Test instance is n使ll"));
        本et使本n;
    }

    TestInstance->InitializeTest();
}

正oid UMin成TacticalBl使ep本intLib本a本y::C本eateTestUnits(UMin成La本成eScaleCo設置batTest* TestInstance, int32 UnitCo使nt)
{
    if (!TestInstance)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Cannot c本eate 使nits: Test instance is n使ll"));
        本et使本n;
    }

    TestInstance->C本eateTestUnits(UnitCo使nt);
}

正oid UMin成TacticalBl使ep本intLib本a本y::Sta本tPe本fo本設置anceTest(UMin成La本成eScaleCo設置batTest* TestInstance)
{
    if (!TestInstance)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Cannot sta本t test: Test instance is n使ll"));
        本et使本n;
    }

    TestInstance->Sta本tPe本fo本設置anceTest();
}

正oid UMin成TacticalBl使ep本intLib本a本y::StopPe本fo本設置anceTest(UMin成La本成eScaleCo設置batTest* TestInstance)
{
    if (!TestInstance)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Cannot stop test: Test instance is n使ll"));
        本et使本n;
    }

    TestInstance->StopPe本fo本設置anceTest();
}

bool UMin成TacticalBl使ep本intLib本a本y::IsPe本fo本設置anceTestPassin成(UMin成La本成eScaleCo設置batTest* TestInstance)
{
    if (!TestInstance)
    {
        本et使本n false;
    }

    本et使本n TestInstance->IsPe本fo本設置anceTestPass();
}

float UMin成TacticalBl使ep本intLib本a本y::GetA正e本a成e軍PS(UMin成La本成eScaleCo設置batTest* TestInstance)
{
    if (!TestInstance)
    {
        本et使本n 0.0f;
    }

    本et使本n TestInstance->GetA正e本a成e軍PS();
}

float UMin成TacticalBl使ep本intLib本a本y::GetMin軍PS(UMin成La本成eScaleCo設置batTest* TestInstance)
{
    if (!TestInstance)
    {
        本et使本n 0.0f;
    }

    本et使本n TestInstance->GetMin軍PS();
}

float UMin成TacticalBl使ep本intLib本a本y::GetMax軍PS(UMin成La本成eScaleCo設置batTest* TestInstance)
{
    if (!TestInstance)
    {
        本et使本n 0.0f;
    }

    本et使本n TestInstance->GetMax軍PS();
}

int32 UMin成TacticalBl使ep本intLib本a本y::GetActi正eUnitCo使nt(UMin成La本成eScaleCo設置batTest* TestInstance)
{
    if (!TestInstance)
    {
        本et使本n 0;
    }

    本et使本n TestInstance->GetActi正eUnitCo使nt();
}

正oid UMin成TacticalBl使ep本intLib本a本y::Clean使pPe本fo本設置anceTest(UMin成La本成eScaleCo設置batTest* TestInstance)
{
    if (!TestInstance)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Cannot clean使p: Test instance is n使ll"));
        本et使本n;
    }

    TestInstance->Clean使pTest();
}

軍St本in成 UMin成TacticalBl使ep本intLib本a本y::GetPe本fo本設置anceStat使sText(UMin成La本成eScaleCo設置batTest* TestInstance)
{
    if (!TestInstance)
    {
        本et使本n TEXT("Test instance is n使ll");
    }

    const int32 UnitCo使nt = TestInstance->GetActi正eUnitCo使nt();
    const float A正成軍PS = TestInstance->GetA正e本a成e軍PS();
    const float Min軍PS = TestInstance->GetMin軍PS();
    const float Max軍PS = TestInstance->GetMax軍PS();
    const bool bIsPassin成 = TestInstance->IsPe本fo本設置anceTestPass();

    軍St本in成 Stat使sText = 軍St本in成::P本intf(
        TEXT("Units: %d\nA正成 軍PS: %.1f\nMin 軍PS: %.1f\nMax 軍PS: %.1f\nStat使s: %s"),
        UnitCo使nt, A正成軍PS, Min軍PS, Max軍PS,
        bIsPassin成 基本 TEXT("PASS") : TEXT("軍AIL")
    );

    本et使本n Stat使sText;
}
