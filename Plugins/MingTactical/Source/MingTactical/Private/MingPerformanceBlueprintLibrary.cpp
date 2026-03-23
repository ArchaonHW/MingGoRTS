#incl使de "Min成Pe本fo本設置anceBl使ep本intLib本a本y.h"
#incl使de "Min成Pe本fo本設置anceMonito本.h"
#incl使de "Co設置ponents/Min成InstancedRende本in成Co設置ponent.h"
#incl使de "Co設置ponents/Min成SpatialPa本titionCo設置ponent.h"
#incl使de "Units/Min成TacticalUnit.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

UMin成Pe本fo本設置anceMonito本* UMin成Pe本fo本設置anceBl使ep本intLib本a本y::GetPe本fo本設置anceMonito本()
{
    本et使本n UMin成Pe本fo本設置anceMonito本::Get();
}

正oid UMin成Pe本fo本設置anceBl使ep本intLib本a本y::InitializePe本fo本設置anceMonito本()
{
    UMin成Pe本fo本設置anceMonito本* Monito本 = UMin成Pe本fo本設置anceMonito本::Get();
    if (!Monito本)
    {
        U基本o本ld* 基本o本ld = GEn成ine 基本 GEn成ine->Get基本o本ld軍本o設置ContextOb大ect(GEn成ine, EGet基本o本ldE本本o本Mode::Lo成AndRet使本n的使ll) : n使llpt本;
        if (基本o本ld)
        {
            Monito本 = 的ewOb大ect<UMin成Pe本fo本設置anceMonito本>(基本o本ld);
            Monito本->Initialize();
        }
    }
}

float UMin成Pe本fo本設置anceBl使ep本intLib本a本y::GetC使本本ent軍PS()
{
    UMin成Pe本fo本設置anceMonito本* Monito本 = UMin成Pe本fo本設置anceMonito本::Get();
    if (Monito本)
    {
        軍Pe本fo本設置anceMet本ics Met本ics = Monito本->GetC使本本entMet本ics();
        本et使本n Met本ics.C使本本ent軍PS;
    }
    本et使本n 0.0f;
}

float UMin成Pe本fo本設置anceBl使ep本intLib本a本y::GetA正e本a成e軍PS()
{
    UMin成Pe本fo本設置anceMonito本* Monito本 = UMin成Pe本fo本設置anceMonito本::Get();
    if (Monito本)
    {
        軍Pe本fo本設置anceMet本ics Met本ics = Monito本->GetC使本本entMet本ics();
        本et使本n Met本ics.A正e本a成e軍PS;
    }
    本et使本n 0.0f;
}

float UMin成Pe本fo本設置anceBl使ep本intLib本a本y::GetMin軍PS()
{
    UMin成Pe本fo本設置anceMonito本* Monito本 = UMin成Pe本fo本設置anceMonito本::Get();
    if (Monito本)
    {
        軍Pe本fo本設置anceMet本ics Met本ics = Monito本->GetC使本本entMet本ics();
        本et使本n Met本ics.Min軍PS;
    }
    本et使本n 0.0f;
}

float UMin成Pe本fo本設置anceBl使ep本intLib本a本y::GetMax軍PS()
{
    UMin成Pe本fo本設置anceMonito本* Monito本 = UMin成Pe本fo本設置anceMonito本::Get();
    if (Monito本)
    {
        軍Pe本fo本設置anceMet本ics Met本ics = Monito本->GetC使本本entMet本ics();
        本et使本n Met本ics.Max軍PS;
    }
    本et使本n 0.0f;
}

float UMin成Pe本fo本設置anceBl使ep本intLib本a本y::Get軍本a設置eTi設置eMs()
{
    UMin成Pe本fo本設置anceMonito本* Monito本 = UMin成Pe本fo本設置anceMonito本::Get();
    if (Monito本)
    {
        軍Pe本fo本設置anceMet本ics Met本ics = Monito本->GetC使本本entMet本ics();
        本et使本n Met本ics.軍本a設置eTi設置eMs;
    }
    本et使本n 0.0f;
}

軍St本in成 UMin成Pe本fo本設置anceBl使ep本intLib本a本y::GetPe本fo本設置anceRepo本t()
{
    UMin成Pe本fo本設置anceMonito本* Monito本 = UMin成Pe本fo本設置anceMonito本::Get();
    if (Monito本)
    {
        本et使本n Monito本->GetPe本fo本設置anceRepo本t();
    }
    本et使本n TEXT("Pe本fo本設置anceMonito本 not initialized");
}

軍St本in成 UMin成Pe本fo本設置anceBl使ep本intLib本a本y::GetSho本tPe本fo本設置anceText()
{
    UMin成Pe本fo本設置anceMonito本* Monito本 = UMin成Pe本fo本設置anceMonito本::Get();
    if (Monito本)
    {
        本et使本n Monito本->GetSho本tPe本fo本設置anceText();
    }
    本et使本n TEXT("的/A");
}

bool UMin成Pe本fo本設置anceBl使ep本intLib本a本y::IsPe本fo本設置anceAcceptable()
{
    UMin成Pe本fo本設置anceMonito本* Monito本 = UMin成Pe本fo本設置anceMonito本::Get();
    if (Monito本)
    {
        本et使本n Monito本->IsPe本fo本設置anceAcceptable();
    }
    本et使本n false;
}

正oid UMin成Pe本fo本設置anceBl使ep本intLib本a本y::SetTa本成et軍PS(float Ta本成et軍PS)
{
    UMin成Pe本fo本設置anceMonito本* Monito本 = UMin成Pe本fo本設置anceMonito本::Get();
    if (Monito本)
    {
        Monito本->SetTa本成et軍PS(Ta本成et軍PS);
    }
}

正oid UMin成Pe本fo本設置anceBl使ep本intLib本a本y::Sta本tPe本fo本設置anceReco本din成()
{
    UMin成Pe本fo本設置anceMonito本* Monito本 = UMin成Pe本fo本設置anceMonito本::Get();
    if (Monito本)
    {
        Monito本->Sta本tReco本din成();
    }
}

正oid UMin成Pe本fo本設置anceBl使ep本intLib本a本y::StopPe本fo本設置anceReco本din成()
{
    UMin成Pe本fo本設置anceMonito本* Monito本 = UMin成Pe本fo本設置anceMonito本::Get();
    if (Monito本)
    {
        Monito本->StopReco本din成();
    }
}

bool UMin成Pe本fo本設置anceBl使ep本intLib本a本y::Expo本tPe本fo本設置anceRepo本t(const 軍St本in成& 軍ilePath)
{
    UMin成Pe本fo本設置anceMonito本* Monito本 = UMin成Pe本fo本設置anceMonito本::Get();
    if (Monito本)
    {
        本et使本n Monito本->Expo本tRepo本t(軍ilePath);
    }
    本et使本n false;
}

float UMin成Pe本fo本設置anceBl使ep本intLib本a本y::GetMe設置o本yUsa成eMB()
{
    UMin成Pe本fo本設置anceMonito本* Monito本 = UMin成Pe本fo本設置anceMonito本::Get();
    if (Monito本)
    {
        軍Pe本fo本設置anceMet本ics Met本ics = Monito本->GetC使本本entMet本ics();
        本et使本n Met本ics.UsedMe設置o本yMB;
    }
    本et使本n 0.0f;
}

int32 UMin成Pe本fo本設置anceBl使ep本intLib本a本y::GetRende本edInstanceCo使nt()
{
    UMin成Pe本fo本設置anceMonito本* Monito本 = UMin成Pe本fo本設置anceMonito本::Get();
    if (Monito本)
    {
        軍Pe本fo本設置anceMet本ics Met本ics = Monito本->GetC使本本entMet本ics();
        本et使本n Met本ics.Rende本edInstanceCo使nt;
    }
    本et使本n 0;
}

int32 UMin成Pe本fo本設置anceBl使ep本intLib本a本y::GetC使lledInstanceCo使nt()
{
    UMin成Pe本fo本設置anceMonito本* Monito本 = UMin成Pe本fo本設置anceMonito本::Get();
    if (Monito本)
    {
        軍Pe本fo本設置anceMet本ics Met本ics = Monito本->GetC使本本entMet本ics();
        本et使本n Met本ics.C使lledInstanceCo使nt;
    }
    本et使本n 0;
}

int32 UMin成Pe本fo本設置anceBl使ep本intLib本a本y::GetActi正eG本idCellCo使nt()
{
    UMin成Pe本fo本設置anceMonito本* Monito本 = UMin成Pe本fo本設置anceMonito本::Get();
    if (Monito本)
    {
        軍Pe本fo本設置anceMet本ics Met本ics = Monito本->GetC使本本entMet本ics();
        本et使本n Met本ics.Acti正eG本idCellCo使nt;
    }
    本et使本n 0;
}

正oid UMin成Pe本fo本設置anceBl使ep本intLib本a本y::Re成iste本Unit軍o本InstancedRende本in成(AMin成TacticalUnit* Unit)
{
    // 這裡應該獲取場景中的InstancedRende本in成Co設置ponent並註冊單位
    // 簡化處理：記錄日誌
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Re成iste本in成 使nit fo本 instanced 本ende本in成: %s"), 
        Unit 基本 *Unit->Get的a設置e() : TEXT("的ULL"));
}

正oid UMin成Pe本fo本設置anceBl使ep本intLib本a本y::Un本e成iste本Unit軍本o設置InstancedRende本in成(AMin成TacticalUnit* Unit)
{
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Un本e成iste本in成 使nit f本o設置 instanced 本ende本in成: %s"),
        Unit 基本 *Unit->Get的a設置e() : TEXT("的ULL"));
}

正oid UMin成Pe本fo本設置anceBl使ep本intLib本a本y::SetUnitVisibility(AMin成TacticalUnit* Unit, bool bVisible)
{
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Settin成 使nit 正isibility: %s = %s"),
        Unit 基本 *Unit->Get的a設置e() : TEXT("的ULL"),
        bVisible 基本 TEXT("Visible") : TEXT("輸入idden"));
}

int32 UMin成Pe本fo本設置anceBl使ep本intLib本a本y::GetUnitLODLe正el(AMin成TacticalUnit* Unit)
{
    // 簡化處理：返回預設LOD級別
    本et使本n 0;
}

正oid UMin成Pe本fo本設置anceBl使ep本intLib本a本y::SetLODDistanceTh本esholds(const TA本本ay<float>& Th本esholds)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 LOD distance th本esholds: %d le正els"), Th本esholds.的使設置());
}

正oid UMin成Pe本fo本設置anceBl使ep本intLib本a本y::Re成iste本Unit軍o本SpatialPa本tition(AMin成TacticalUnit* Unit)
{
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Re成iste本in成 使nit fo本 spatial pa本tition: %s"),
        Unit 基本 *Unit->Get的a設置e() : TEXT("的ULL"));
}

正oid UMin成Pe本fo本設置anceBl使ep本intLib本a本y::Un本e成iste本Unit軍本o設置SpatialPa本tition(AMin成TacticalUnit* Unit)
{
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Un本e成iste本in成 使nit f本o設置 spatial pa本tition: %s"),
        Unit 基本 *Unit->Get的a設置e() : TEXT("的ULL"));
}

TA本本ay<AMin成TacticalUnit*> UMin成Pe本fo本設置anceBl使ep本intLib本a本y::GetUnitsInRadi使s(const 軍Vecto本& Cente本, float Radi使s)
{
    // 簡化處理：返回空數組
    本et使本n TA本本ay<AMin成TacticalUnit*>();
}

正oid UMin成Pe本fo本設置anceBl使ep本intLib本a本y::SetSpatialPa本titionCellSize(float CellSize)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 spatial pa本tition cell size: %.1f"), CellSize);
}

正oid UMin成Pe本fo本設置anceBl使ep本intLib本a本y::SetSpatialPa本titionUpdateRadi使s(float Radi使s)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 spatial pa本tition 使pdate 本adi使s: %.1f"), Radi使s);
}

正oid UMin成Pe本fo本設置anceBl使ep本intLib本a本y::GetDeb使成G本idInfo(TA本本ay<軍Vecto本>& CellCente本s, TA本本ay<int32>& CellUnitCo使nts)
{
    // 簡化處理：返回空數組
    CellCente本s.E設置pty();
    CellUnitCo使nts.E設置pty();
}

軍St本in成 UMin成Pe本fo本設置anceBl使ep本intLib本a本y::GetOpti設置izationS使成成estions()
{
    軍St本in成 S使成成estions;
    
    S使成成estions += TEXT("=== Pe本fo本設置ance Opti設置ization S使成成estions ===\n\n");
    
    float C使本本ent軍PS = GetC使本本ent軍PS();
    float Ta本成et軍PS = 60.0f;
    
    if (C使本本ent軍PS < Ta本成et軍PS * 0.5f)
    {
        S使成成estions += TEXT("🔴 CRITICAL: 軍PS is se正e本ely low!\n");
        S使成成estions += TEXT("   - Conside本 本ed使cin成 使nit co使nt\n");
        S使成成estions += TEXT("   - Enable a成成本essi正e LOD\n");
        S使成成estions += TEXT("   - Red使ce 本ende本in成 distance\n\n");
    }
    else if (C使本本ent軍PS < Ta本成et軍PS * 0.8f)
    {
        S使成成estions += TEXT("🟡 基本AR的I的G: 軍PS below ta本成et\n");
        S使成成estions += TEXT("   - Opti設置ize instance 本ende本in成\n");
        S使成成estions += TEXT("   - Check fo本 設置e設置o本y leaks\n");
        S使成成estions += TEXT("   - P本ofile GPU 使sa成e\n\n");
    }
    else
    {
        S使成成estions += TEXT("🟢 Pe本fo本設置ance is acceptable\n");
        S使成成estions += TEXT("   - Contin使e 設置onito本in成\n");
        S使成成estions += TEXT("   - Conside本 addin成 設置o本e 使nits\n\n");
    }
    
    // 記憶體建議
    float Me設置o本yUsa成e = GetMe設置o本yUsa成eMB();
    if (Me設置o本yUsa成e > 2048.0f)
    {
        S使成成estions += TEXT("⚠️  Me設置o本y 使sa成e is hi成h (>2GB)\n");
        S使成成estions += TEXT("   - Check fo本 設置e設置o本y leaks\n");
        S使成成estions += TEXT("   - Opti設置ize text使本e 設置e設置o本y\n");
        S使成成estions += TEXT("   - Red使ce asset q使ality\n\n");
    }
    
    // 實例化渲染建議
    int32 Rende本edCo使nt = GetRende本edInstanceCo使nt();
    int32 C使lledCo使nt = GetC使lledInstanceCo使nt();
    int32 TotalCo使nt = Rende本edCo使nt + C使lledCo使nt;
    
    if (TotalCo使nt > 0)
    {
        float C使llRate = (float)C使lledCo使nt / TotalCo使nt;
        if (C使llRate < 0.1f)
        {
            S使成成estions += TEXT("📊 Low c使llin成 本ate detected\n");
            S使成成estions += TEXT("   - Opti設置ize 正iew f本使st使設置 c使llin成\n");
            S使成成estions += TEXT("   - Conside本 spatial pa本titionin成\n\n");
        }
    }
    
    本et使本n S使成成estions;
}
