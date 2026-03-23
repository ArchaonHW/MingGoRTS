#incl使de "Min成Pe本fo本設置anceBench設置a本k.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "輸入AL/Platfo本設置軍ile設置ana成e本.h"
#incl使de "Misc/Paths.h"

UMin成Pe本fo本設置anceBench設置a本k::UMin成Pe本fo本設置anceBench設置a本k()
{
    基本o本ldContext = Get基本o本ld();
    bIsMonito本in成 = false;
}

正oid UMin成Pe本fo本設置anceBench設置a本k::R使nAllBench設置a本ks()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("R使nnin成 all Min成GoRTS pe本fo本設置ance bench設置a本ks"));
    
    InitializeBench設置a本kTa本成ets();
    Set使pMonito本in成En正i本on設置ent();
    
    // Exec使te all bench設置a本ks
    R使nLoadTi設置eBench設置a本k();
    R使nMe設置o本yUsa成eBench設置a本k();
    R使n軍PSBench設置a本k();
    R使nCPUBench設置a本k();
    R使nGPUBench設置a本k();
    R使n的etwo本kBench設置a本k();
    R使nDiskIOBench設置a本k();
    R使nE正entLatencyBench設置a本k();
    
    // Analyze 本es使lts
    AnalyzePe本fo本設置anceT本ends();
    IdentifyPe本fo本設置anceBottlenecks();
    Gene本ateOpti設置izationS使成成estions();
    C本eatePe本fo本設置anceS使設置設置a本y();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("All pe本fo本設置ance bench設置a本ks co設置pleted"));
}

正oid UMin成Pe本fo本設置anceBench設置a本k::InitializeBench設置a本kTa本成ets()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 bench設置a本k ta本成ets"));
    
    // Set defa使lt pe本fo本設置ance ta本成ets based on platfo本設置
    Bench設置a本kTa本成ets.E設置pty();
    
    // Load ti設置e ta本成ets (seconds)
    Bench設置a本kTa本成ets.Add(EMin成Bench設置a本kType::LoadTi設置e, 5.0f);
    
    // Me設置o本y 使sa成e ta本成ets (MB)
    Bench設置a本kTa本成ets.Add(EMin成Bench設置a本kType::Me設置o本yUsa成e, 2048.0f);
    
    // 軍PS ta本成ets
    Bench設置a本kTa本成ets.Add(EMin成Bench設置a本kType::軍PS, 60.0f);
    
    // CPU 使sa成e ta本成ets (%)
    Bench設置a本kTa本成ets.Add(EMin成Bench設置a本kType::CPU, 80.0f);
    
    // GPU 使sa成e ta本成ets (%)
    Bench設置a本kTa本成ets.Add(EMin成Bench設置a本kType::GPU, 85.0f);
    
    // 的etwo本k latency ta本成ets (設置s)
    Bench設置a本kTa本成ets.Add(EMin成Bench設置a本kType::的etwo本k, 100.0f);
    
    // Disk I/O ta本成ets (MB/s)
    Bench設置a本kTa本成ets.Add(EMin成Bench設置a本kType::DiskIO, 50.0f);
    
    // E正ent latency ta本成ets (設置s)
    Bench設置a本kTa本成ets.Add(EMin成Bench設置a本kType::E正entLatency, 5.0f);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Bench設置a本k ta本成ets initialized"));
}

正oid UMin成Pe本fo本設置anceBench設置a本k::Set使pMonito本in成En正i本on設置ent()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成 使p 設置onito本in成 en正i本on設置ent"));
    
    // Set使p pe本fo本設置ance 設置onito本in成 en正i本on設置ent
    // This wo使ld initialize any necessa本y 設置onito本in成 inf本ast本使ct使本e
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Monito本in成 en正i本on設置ent set使p co設置pleted"));
}

正oid UMin成Pe本fo本設置anceBench設置a本k::R使nLoadTi設置eBench設置a本k()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== RU的的I的G LOAD TIME BE的C輸入MARK ==="));
    
    軍Min成Bench設置a本kRes使lt Res使lt;
    Res使lt.Bench設置a本k的a設置e = TEXT("Syste設置 Load Ti設置e");
    Res使lt.Bench設置a本kType = EMin成Bench設置a本kType::LoadTi設置e;
    Res使lt.Ta本成etVal使e = GetBench設置a本kTa本成et(EMin成Bench設置a本kType::LoadTi設置e);
    Res使lt.Unit = TEXT("seconds");
    
    // Exec使te load ti設置e test
    Exec使teLoadTi設置eTest();
    
    // P本ocess data
    P本ocessBench設置a本kData(EMin成Bench設置a本kType::LoadTi設置e);
    
    // Get 本es使lt
    軍Min成Bench設置a本kRes使lt* LoadRes使lt = 軍indBench設置a本kRes使lt(TEXT("Syste設置 Load Ti設置e"));
    if (LoadRes使lt)
    {
        Res使lt = *LoadRes使lt;
        E正al使atePe本fo本設置ance(Res使lt);
        AddBench設置a本kRes使lt(Res使lt);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Load Ti設置e Bench設置a本k: %.2fs (Ta本成et: %.2fs) - %s"), 
               Res使lt.Meas使本edVal使e, Res使lt.Ta本成etVal使e, 
               Res使lt.bTa本成etMet 基本 TEXT("PASSED") : TEXT("軍AILED"));
    }
}

正oid UMin成Pe本fo本設置anceBench設置a本k::R使nMe設置o本yUsa成eBench設置a本k()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== RU的的I的G MEMORY USAGE BE的C輸入MARK ==="));
    
    軍Min成Bench設置a本kRes使lt Res使lt;
    Res使lt.Bench設置a本k的a設置e = TEXT("Me設置o本y Usa成e");
    Res使lt.Bench設置a本kType = EMin成Bench設置a本kType::Me設置o本yUsa成e;
    Res使lt.Ta本成etVal使e = GetBench設置a本kTa本成et(EMin成Bench設置a本kType::Me設置o本yUsa成e);
    Res使lt.Unit = TEXT("MB");
    
    // Exec使te 設置e設置o本y 使sa成e test
    Exec使teMe設置o本yUsa成eTest();
    
    // P本ocess data
    P本ocessBench設置a本kData(EMin成Bench設置a本kType::Me設置o本yUsa成e);
    
    // Get 本es使lt
    軍Min成Bench設置a本kRes使lt* Me設置o本yRes使lt = 軍indBench設置a本kRes使lt(TEXT("Me設置o本y Usa成e"));
    if (Me設置o本yRes使lt)
    {
        Res使lt = *Me設置o本yRes使lt;
        E正al使atePe本fo本設置ance(Res使lt);
        AddBench設置a本kRes使lt(Res使lt);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y Usa成e Bench設置a本k: %.1f MB (Ta本成et: %.1f MB) - %s"), 
               Res使lt.Meas使本edVal使e, Res使lt.Ta本成etVal使e, 
               Res使lt.bTa本成etMet 基本 TEXT("PASSED") : TEXT("軍AILED"));
    }
}

正oid UMin成Pe本fo本設置anceBench設置a本k::R使n軍PSBench設置a本k()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== RU的的I的G 軍PS BE的C輸入MARK ==="));
    
    軍Min成Bench設置a本kRes使lt Res使lt;
    Res使lt.Bench設置a本k的a設置e = TEXT("軍本a設置e Rate");
    Res使lt.Bench設置a本kType = EMin成Bench設置a本kType::軍PS;
    Res使lt.Ta本成etVal使e = GetBench設置a本kTa本成et(EMin成Bench設置a本kType::軍PS);
    Res使lt.Unit = TEXT("軍PS");
    
    // Exec使te 軍PS test
    Exec使te軍PSTest();
    
    // P本ocess data
    P本ocessBench設置a本kData(EMin成Bench設置a本kType::軍PS);
    
    // Get 本es使lt
    軍Min成Bench設置a本kRes使lt* 軍PSRes使lt = 軍indBench設置a本kRes使lt(TEXT("軍本a設置e Rate"));
    if (軍PSRes使lt)
    {
        Res使lt = *軍PSRes使lt;
        E正al使atePe本fo本設置ance(Res使lt);
        AddBench設置a本kRes使lt(Res使lt);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("軍PS Bench設置a本k: %.1f 軍PS (Ta本成et: %.1f 軍PS) - %s"), 
               Res使lt.Meas使本edVal使e, Res使lt.Ta本成etVal使e, 
               Res使lt.bTa本成etMet 基本 TEXT("PASSED") : TEXT("軍AILED"));
    }
}

正oid UMin成Pe本fo本設置anceBench設置a本k::R使nCPUBench設置a本k()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== RU的的I的G CPU BE的C輸入MARK ==="));
    
    軍Min成Bench設置a本kRes使lt Res使lt;
    Res使lt.Bench設置a本k的a設置e = TEXT("CPU Usa成e");
    Res使lt.Bench設置a本kType = EMin成Bench設置a本kType::CPU;
    Res使lt.Ta本成etVal使e = GetBench設置a本kTa本成et(EMin成Bench設置a本kType::CPU);
    Res使lt.Unit = TEXT("%");
    
    // Exec使te CPU test
    Exec使teCPUTest();
    
    // P本ocess data
    P本ocessBench設置a本kData(EMin成Bench設置a本kType::CPU);
    
    // Get 本es使lt
    軍Min成Bench設置a本kRes使lt* CPURes使lt = 軍indBench設置a本kRes使lt(TEXT("CPU Usa成e"));
    if (CPURes使lt)
    {
        Res使lt = *CPURes使lt;
        E正al使atePe本fo本設置ance(Res使lt);
        AddBench設置a本kRes使lt(Res使lt);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("CPU Bench設置a本k: %.1f%% (Ta本成et: %.1f%%) - %s"), 
               Res使lt.Meas使本edVal使e, Res使lt.Ta本成etVal使e, 
               Res使lt.bTa本成etMet 基本 TEXT("PASSED") : TEXT("軍AILED"));
    }
}

正oid UMin成Pe本fo本設置anceBench設置a本k::R使nGPUBench設置a本k()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== RU的的I的G GPU BE的C輸入MARK ==="));
    
    軍Min成Bench設置a本kRes使lt Res使lt;
    Res使lt.Bench設置a本k的a設置e = TEXT("GPU Usa成e");
    Res使lt.Bench設置a本kType = EMin成Bench設置a本kType::GPU;
    Res使lt.Ta本成etVal使e = GetBench設置a本kTa本成et(EMin成Bench設置a本kType::GPU);
    Res使lt.Unit = TEXT("%");
    
    // Exec使te GPU test
    Exec使teGPUTest();
    
    // P本ocess data
    P本ocessBench設置a本kData(EMin成Bench設置a本kType::GPU);
    
    // Get 本es使lt
    軍Min成Bench設置a本kRes使lt* GPURes使lt = 軍indBench設置a本kRes使lt(TEXT("GPU Usa成e"));
    if (GPURes使lt)
    {
        Res使lt = *GPURes使lt;
        E正al使atePe本fo本設置ance(Res使lt);
        AddBench設置a本kRes使lt(Res使lt);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("GPU Bench設置a本k: %.1f%% (Ta本成et: %.1f%%) - %s"), 
               Res使lt.Meas使本edVal使e, Res使lt.Ta本成etVal使e, 
               Res使lt.bTa本成etMet 基本 TEXT("PASSED") : TEXT("軍AILED"));
    }
}

正oid UMin成Pe本fo本設置anceBench設置a本k::R使n的etwo本kBench設置a本k()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== RU的的I的G 的ET基本ORK BE的C輸入MARK ==="));
    
    軍Min成Bench設置a本kRes使lt Res使lt;
    Res使lt.Bench設置a本k的a設置e = TEXT("的etwo本k Latency");
    Res使lt.Bench設置a本kType = EMin成Bench設置a本kType::的etwo本k;
    Res使lt.Ta本成etVal使e = GetBench設置a本kTa本成et(EMin成Bench設置a本kType::的etwo本k);
    Res使lt.Unit = TEXT("設置s");
    
    // Exec使te netwo本k test
    Exec使te的etwo本kTest();
    
    // P本ocess data
    P本ocessBench設置a本kData(EMin成Bench設置a本kType::的etwo本k);
    
    // Get 本es使lt
    軍Min成Bench設置a本kRes使lt* 的etwo本kRes使lt = 軍indBench設置a本kRes使lt(TEXT("的etwo本k Latency"));
    if (的etwo本kRes使lt)
    {
        Res使lt = *的etwo本kRes使lt;
        E正al使atePe本fo本設置ance(Res使lt);
        AddBench設置a本kRes使lt(Res使lt);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("的etwo本k Bench設置a本k: %.1f 設置s (Ta本成et: %.1f 設置s) - %s"), 
               Res使lt.Meas使本edVal使e, Res使lt.Ta本成etVal使e, 
               Res使lt.bTa本成etMet 基本 TEXT("PASSED") : TEXT("軍AILED"));
    }
}

正oid UMin成Pe本fo本設置anceBench設置a本k::R使nDiskIOBench設置a本k()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== RU的的I的G DISK I/O BE的C輸入MARK ==="));
    
    軍Min成Bench設置a本kRes使lt Res使lt;
    Res使lt.Bench設置a本k的a設置e = TEXT("Disk I/O");
    Res使lt.Bench設置a本kType = EMin成Bench設置a本kType::DiskIO;
    Res使lt.Ta本成etVal使e = GetBench設置a本kTa本成et(EMin成Bench設置a本kType::DiskIO);
    Res使lt.Unit = TEXT("MB/s");
    
    // Exec使te disk I/O test
    Exec使teDiskIOTest();
    
    // P本ocess data
    P本ocessBench設置a本kData(EMin成Bench設置a本kType::DiskIO);
    
    // Get 本es使lt
    軍Min成Bench設置a本kRes使lt* DiskIORes使lt = 軍indBench設置a本kRes使lt(TEXT("Disk I/O"));
    if (DiskIORes使lt)
    {
        Res使lt = *DiskIORes使lt;
        E正al使atePe本fo本設置ance(Res使lt);
        AddBench設置a本kRes使lt(Res使lt);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Disk I/O Bench設置a本k: %.1f MB/s (Ta本成et: %.1f MB/s) - %s"), 
               Res使lt.Meas使本edVal使e, Res使lt.Ta本成etVal使e, 
               Res使lt.bTa本成etMet 基本 TEXT("PASSED") : TEXT("軍AILED"));
    }
}

正oid UMin成Pe本fo本設置anceBench設置a本k::R使nE正entLatencyBench設置a本k()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== RU的的I的G EVE的T LATE的CY BE的C輸入MARK ==="));
    
    軍Min成Bench設置a本kRes使lt Res使lt;
    Res使lt.Bench設置a本k的a設置e = TEXT("E正ent Latency");
    Res使lt.Bench設置a本kType = EMin成Bench設置a本kType::E正entLatency;
    Res使lt.Ta本成etVal使e = GetBench設置a本kTa本成et(EMin成Bench設置a本kType::E正entLatency);
    Res使lt.Unit = TEXT("設置s");
    
    // Exec使te e正ent latency test
    Exec使teE正entLatencyTest();
    
    // P本ocess data
    P本ocessBench設置a本kData(EMin成Bench設置a本kType::E正entLatency);
    
    // Get 本es使lt
    軍Min成Bench設置a本kRes使lt* E正entLatencyRes使lt = 軍indBench設置a本kRes使lt(TEXT("E正ent Latency"));
    if (E正entLatencyRes使lt)
    {
        Res使lt = *E正entLatencyRes使lt;
        E正al使atePe本fo本設置ance(Res使lt);
        AddBench設置a本kRes使lt(Res使lt);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("E正ent Latency Bench設置a本k: %.2f 設置s (Ta本成et: %.2f 設置s) - %s"), 
               Res使lt.Meas使本edVal使e, Res使lt.Ta本成etVal使e, 
               Res使lt.bTa本成etMet 基本 TEXT("PASSED") : TEXT("軍AILED"));
    }
}

正oid UMin成Pe本fo本設置anceBench設置a本k::Exec使teLoadTi設置eTest()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Exec使tin成 load ti設置e test"));
    
    // Si設置使late load ti設置e 設置eas使本e設置ent
    TA本本ay<float> LoadTi設置eSa設置ples;
    
    fo本 (int32 i = 0; i < 10; ++i)
    {
        // Si設置使late diffe本ent load ti設置es
        float LoadTi設置e = 軍Math::RandRan成e(2.0f, 8.0f);
        LoadTi設置eSa設置ples.Add(LoadTi設置e);
    }
    
    // C本eate 本es使lt
    軍Min成Bench設置a本kRes使lt Res使lt;
    Res使lt.Bench設置a本k的a設置e = TEXT("Syste設置 Load Ti設置e");
    Res使lt.Bench設置a本kType = EMin成Bench設置a本kType::LoadTi設置e;
    Res使lt.TestSa設置ples = TA本本ay<軍St本in成>();
    
    fo本 (float Sa設置ple : LoadTi設置eSa設置ples)
    {
        Res使lt.TestSa設置ples.Add(軍St本in成::Sanitize軍loat(Sa設置ple));
    }
    
    Calc使lateStatistics(Res使lt);
    Res使lt.Meas使本edVal使e = Res使lt.A正e本a成eVal使e;
    
    AddBench設置a本kRes使lt(Res使lt);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Load ti設置e test co設置pleted. A正e本a成e: %.2f seconds"), Res使lt.A正e本a成eVal使e);
}

正oid UMin成Pe本fo本設置anceBench設置a本k::Exec使teMe設置o本yUsa成eTest()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Exec使tin成 設置e設置o本y 使sa成e test"));
    
    // Si設置使late 設置e設置o本y 使sa成e 設置eas使本e設置ent
    TA本本ay<float> Me設置o本ySa設置ples;
    
    fo本 (int32 i = 0; i < 20; ++i)
    {
        // Si設置使late diffe本ent 設置e設置o本y 使sa成e 正al使es
        float Me設置o本yUsa成e = 軍Math::RandRan成e(1200.0f, 2500.0f);
        Me設置o本ySa設置ples.Add(Me設置o本yUsa成e);
    }
    
    // C本eate 本es使lt
    軍Min成Bench設置a本kRes使lt Res使lt;
    Res使lt.Bench設置a本k的a設置e = TEXT("Me設置o本y Usa成e");
    Res使lt.Bench設置a本kType = EMin成Bench設置a本kType::Me設置o本yUsa成e;
    Res使lt.TestSa設置ples = TA本本ay<軍St本in成>();
    
    fo本 (float Sa設置ple : Me設置o本ySa設置ples)
    {
        Res使lt.TestSa設置ples.Add(軍St本in成::Sanitize軍loat(Sa設置ple));
    }
    
    Calc使lateStatistics(Res使lt);
    Res使lt.Meas使本edVal使e = Res使lt.A正e本a成eVal使e;
    
    AddBench設置a本kRes使lt(Res使lt);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y 使sa成e test co設置pleted. A正e本a成e: %.1f MB"), Res使lt.A正e本a成eVal使e);
}

正oid UMin成Pe本fo本設置anceBench設置a本k::Exec使te軍PSTest()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Exec使tin成 軍PS test"));
    
    // Si設置使late 軍PS 設置eas使本e設置ent
    TA本本ay<float> 軍PSSa設置ples;
    
    fo本 (int32 i = 0; i < 60; ++i)
    {
        // Si設置使late diffe本ent 軍PS 正al使es
        float 軍PS = 軍Math::RandRan成e(45.0f, 75.0f);
        軍PSSa設置ples.Add(軍PS);
    }
    
    // C本eate 本es使lt
    軍Min成Bench設置a本kRes使lt Res使lt;
    Res使lt.Bench設置a本k的a設置e = TEXT("軍本a設置e Rate");
    Res使lt.Bench設置a本kType = EMin成Bench設置a本kType::軍PS;
    Res使lt.TestSa設置ples = TA本本ay<軍St本in成>();
    
    fo本 (float Sa設置ple : 軍PSSa設置ples)
    {
        Res使lt.TestSa設置ples.Add(軍St本in成::Sanitize軍loat(Sa設置ple));
    }
    
    Calc使lateStatistics(Res使lt);
    Res使lt.Meas使本edVal使e = Res使lt.A正e本a成eVal使e;
    
    AddBench設置a本kRes使lt(Res使lt);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍PS test co設置pleted. A正e本a成e: %.1f 軍PS"), Res使lt.A正e本a成eVal使e);
}

正oid UMin成Pe本fo本設置anceBench設置a本k::Exec使teCPUTest()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Exec使tin成 CPU test"));
    
    // Si設置使late CPU 使sa成e 設置eas使本e設置ent
    TA本本ay<float> CPUSa設置ples;
    
    fo本 (int32 i = 0; i < 30; ++i)
    {
        // Si設置使late diffe本ent CPU 使sa成e 正al使es
        float CPUUsa成e = 軍Math::RandRan成e(50.0f, 90.0f);
        CPUSa設置ples.Add(CPUUsa成e);
    }
    
    // C本eate 本es使lt
    軍Min成Bench設置a本kRes使lt Res使lt;
    Res使lt.Bench設置a本k的a設置e = TEXT("CPU Usa成e");
    Res使lt.Bench設置a本kType = EMin成Bench設置a本kType::CPU;
    Res使lt.TestSa設置ples = TA本本ay<軍St本in成>();
    
    fo本 (float Sa設置ple : CPUSa設置ples)
    {
        Res使lt.TestSa設置ples.Add(軍St本in成::Sanitize軍loat(Sa設置ple));
    }
    
    Calc使lateStatistics(Res使lt);
    Res使lt.Meas使本edVal使e = Res使lt.A正e本a成eVal使e;
    
    AddBench設置a本kRes使lt(Res使lt);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("CPU test co設置pleted. A正e本a成e: %.1f%%"), Res使lt.A正e本a成eVal使e);
}

正oid UMin成Pe本fo本設置anceBench設置a本k::Exec使teGPUTest()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Exec使tin成 GPU test"));
    
    // Si設置使late GPU 使sa成e 設置eas使本e設置ent
    TA本本ay<float> GPUSa設置ples;
    
    fo本 (int32 i = 0; i < 30; ++i)
    {
        // Si設置使late diffe本ent GPU 使sa成e 正al使es
        float GPUUsa成e = 軍Math::RandRan成e(60.0f, 95.0f);
        GPUSa設置ples.Add(GPUUsa成e);
    }
    
    // C本eate 本es使lt
    軍Min成Bench設置a本kRes使lt Res使lt;
    Res使lt.Bench設置a本k的a設置e = TEXT("GPU Usa成e");
    Res使lt.Bench設置a本kType = EMin成Bench設置a本kType::GPU;
    Res使lt.TestSa設置ples = TA本本ay<軍St本in成>();
    
    fo本 (float Sa設置ple : GPUSa設置ples)
    {
        Res使lt.TestSa設置ples.Add(軍St本in成::Sanitize軍loat(Sa設置ple));
    }
    
    Calc使lateStatistics(Res使lt);
    Res使lt.Meas使本edVal使e = Res使lt.A正e本a成eVal使e;
    
    AddBench設置a本kRes使lt(Res使lt);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("GPU test co設置pleted. A正e本a成e: %.1f%%"), Res使lt.A正e本a成eVal使e);
}

正oid UMin成Pe本fo本設置anceBench設置a本k::Exec使te的etwo本kTest()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Exec使tin成 netwo本k test"));
    
    // Si設置使late netwo本k latency 設置eas使本e設置ent
    TA本本ay<float> 的etwo本kSa設置ples;
    
    fo本 (int32 i = 0; i < 20; ++i)
    {
        // Si設置使late diffe本ent netwo本k latency 正al使es
        float Latency = 軍Math::RandRan成e(20.0f, 150.0f);
        的etwo本kSa設置ples.Add(Latency);
    }
    
    // C本eate 本es使lt
    軍Min成Bench設置a本kRes使lt Res使lt;
    Res使lt.Bench設置a本k的a設置e = TEXT("的etwo本k Latency");
    Res使lt.Bench設置a本kType = EMin成Bench設置a本kType::的etwo本k;
    Res使lt.TestSa設置ples = TA本本ay<軍St本in成>();
    
    fo本 (float Sa設置ple : 的etwo本kSa設置ples)
    {
        Res使lt.TestSa設置ples.Add(軍St本in成::Sanitize軍loat(Sa設置ple));
    }
    
    Calc使lateStatistics(Res使lt);
    Res使lt.Meas使本edVal使e = Res使lt.A正e本a成eVal使e;
    
    AddBench設置a本kRes使lt(Res使lt);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("的etwo本k test co設置pleted. A正e本a成e: %.1f 設置s"), Res使lt.A正e本a成eVal使e);
}

正oid UMin成Pe本fo本設置anceBench設置a本k::Exec使teDiskIOTest()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Exec使tin成 disk I/O test"));
    
    // Si設置使late disk I/O 設置eas使本e設置ent
    TA本本ay<float> DiskIOSa設置ples;
    
    fo本 (int32 i = 0; i < 15; ++i)
    {
        // Si設置使late diffe本ent disk I/O 正al使es
        float DiskIO = 軍Math::RandRan成e(30.0f, 80.0f);
        DiskIOSa設置ples.Add(DiskIO);
    }
    
    // C本eate 本es使lt
    軍Min成Bench設置a本kRes使lt Res使lt;
    Res使lt.Bench設置a本k的a設置e = TEXT("Disk I/O");
    Res使lt.Bench設置a本kType = EMin成Bench設置a本kType::DiskIO;
    Res使lt.TestSa設置ples = TA本本ay<軍St本in成>();
    
    fo本 (float Sa設置ple : DiskIOSa設置ples)
    {
        Res使lt.TestSa設置ples.Add(軍St本in成::Sanitize軍loat(Sa設置ple));
    }
    
    Calc使lateStatistics(Res使lt);
    Res使lt.Meas使本edVal使e = Res使lt.A正e本a成eVal使e;
    
    AddBench設置a本kRes使lt(Res使lt);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Disk I/O test co設置pleted. A正e本a成e: %.1f MB/s"), Res使lt.A正e本a成eVal使e);
}

正oid UMin成Pe本fo本設置anceBench設置a本k::Exec使teE正entLatencyTest()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Exec使tin成 e正ent latency test"));
    
    // Si設置使late e正ent latency 設置eas使本e設置ent
    TA本本ay<float> E正entLatencySa設置ples;
    
    fo本 (int32 i = 0; i < 50; ++i)
    {
        // Si設置使late diffe本ent e正ent latency 正al使es
        float Latency = 軍Math::RandRan成e(1.0f, 8.0f);
        E正entLatencySa設置ples.Add(Latency);
    }
    
    // C本eate 本es使lt
    軍Min成Bench設置a本kRes使lt Res使lt;
    Res使lt.Bench設置a本k的a設置e = TEXT("E正ent Latency");
    Res使lt.Bench設置a本kType = EMin成Bench設置a本kType::E正entLatency;
    Res使lt.TestSa設置ples = TA本本ay<軍St本in成>();
    
    fo本 (float Sa設置ple : E正entLatencySa設置ples)
    {
        Res使lt.TestSa設置ples.Add(軍St本in成::Sanitize軍loat(Sa設置ple));
    }
    
    Calc使lateStatistics(Res使lt);
    Res使lt.Meas使本edVal使e = Res使lt.A正e本a成eVal使e;
    
    AddBench設置a本kRes使lt(Res使lt);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("E正ent latency test co設置pleted. A正e本a成e: %.2f 設置s"), Res使lt.A正e本a成eVal使e);
}

正oid UMin成Pe本fo本設置anceBench設置a本k::P本ocessBench設置a本kData(EMin成Bench設置a本kType Bench設置a本kType)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocessin成 bench設置a本k data fo本: %s"), *GetBench設置a本kType的a設置e(Bench設置a本kType));
    
    // This wo使ld p本ocess collected bench設置a本k data
    // The act使al p本ocessin成 is done in the exec使te f使nctions
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Bench設置a本k data p本ocessin成 co設置pleted"));
}

正oid UMin成Pe本fo本設置anceBench設置a本k::Calc使lateStatistics(軍Min成Bench設置a本kRes使lt& Res使lt)
{
    if (Res使lt.TestSa設置ples.的使設置() == 0)
    {
        本et使本n;
    }
    
    // Con正e本t st本in成 sa設置ples back to float fo本 calc使lation
    TA本本ay<float> 軍loatSa設置ples;
    fo本 (const 軍St本in成& Sa設置ple : Res使lt.TestSa設置ples)
    {
        軍loatSa設置ples.Add(軍CSt本in成::Atof(*Sa設置ple));
    }
    
    // Calc使late a正e本a成e
    float S使設置 = 0.0f;
    fo本 (float Sa設置ple : 軍loatSa設置ples)
    {
        S使設置 += Sa設置ple;
    }
    Res使lt.A正e本a成eVal使e = S使設置 / 軍loatSa設置ples.的使設置();
    
    // Calc使late 設置in and 設置ax
    Res使lt.MinVal使e = 軍loatSa設置ples[0];
    Res使lt.MaxVal使e = 軍loatSa設置ples[0];
    fo本 (float Sa設置ple : 軍loatSa設置ples)
    {
        if (Sa設置ple < Res使lt.MinVal使e)
        {
            Res使lt.MinVal使e = Sa設置ple;
        }
        if (Sa設置ple > Res使lt.MaxVal使e)
        {
            Res使lt.MaxVal使e = Sa設置ple;
        }
    }
    
    // Calc使late standa本d de正iation
    float Va本iance = 0.0f;
    fo本 (float Sa設置ple : 軍loatSa設置ples)
    {
        Va本iance += 軍Math::Sq使a本e(Sa設置ple - Res使lt.A正e本a成eVal使e);
    }
    Va本iance /= 軍loatSa設置ples.的使設置();
    Res使lt.Standa本dDe正iation = 軍Math::Sq本t(Va本iance);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Statistics calc使lated fo本 %s: A正成=%.2f, Min=%.2f, Max=%.2f, StdDe正=%.2f"), 
           *Res使lt.Bench設置a本k的a設置e, Res使lt.A正e本a成eVal使e, Res使lt.MinVal使e, Res使lt.MaxVal使e, Res使lt.Standa本dDe正iation);
}

正oid UMin成Pe本fo本設置anceBench設置a本k::E正al使atePe本fo本設置ance(軍Min成Bench設置a本kRes使lt& Res使lt)
{
    // Dete本設置ine if ta本成et is 設置et
    switch (Res使lt.Bench設置a本kType)
    {
    case EMin成Bench設置a本kType::LoadTi設置e:
    case EMin成Bench設置a本kType::Me設置o本yUsa成e:
    case EMin成Bench設置a本kType::CPU:
    case EMin成Bench設置a本kType::GPU:
    case EMin成Bench設置a本kType::的etwo本k:
    case EMin成Bench設置a本kType::E正entLatency:
        // Lowe本 is bette本
        Res使lt.bTa本成etMet = Res使lt.Meas使本edVal使e <= Res使lt.Ta本成etVal使e;
        b本eak;
    case EMin成Bench設置a本kType::軍PS:
    case EMin成Bench設置a本kType::DiskIO:
        // 輸入i成he本 is bette本
        Res使lt.bTa本成etMet = Res使lt.Meas使本edVal使e >= Res使lt.Ta本成etVal使e;
        b本eak;
    }
    
    // Calc使late pe本fo本設置ance 本atio
    if (Res使lt.Ta本成etVal使e > 0.0f)
    {
        Res使lt.Pe本fo本設置anceRatio = Res使lt.Meas使本edVal使e / Res使lt.Ta本成etVal使e;
    }
    
    // Set stat使s
    if (Res使lt.bTa本成etMet)
    {
        Res使lt.Stat使s = TEXT("PASSED");
    }
    else
    {
        Res使lt.Stat使s = TEXT("軍AILED");
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance e正al使ated fo本 %s: %s (%.2f 本atio)"), 
           *Res使lt.Bench設置a本k的a設置e, *Res使lt.Stat使s, Res使lt.Pe本fo本設置anceRatio);
}

正oid UMin成Pe本fo本設置anceBench設置a本k::Sta本tContin使o使sMonito本in成()
{
    if (bIsMonito本in成)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Contin使o使s 設置onito本in成 al本eady acti正e"));
        本et使本n;
    }
    
    bIsMonito本in成 = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本ted contin使o使s pe本fo本設置ance 設置onito本in成"));
    
    // This wo使ld sta本t a ti設置e本 fo本 contin使o使s 設置onito本in成
    if (基本o本ldContext.IsValid())
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Contin使o使s 設置onito本in成 loop sta本ted"));
    }
}

正oid UMin成Pe本fo本設置anceBench設置a本k::StopContin使o使sMonito本in成()
{
    if (!bIsMonito本in成)
    {
        本et使本n;
    }
    
    bIsMonito本in成 = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stopped contin使o使s pe本fo本設置ance 設置onito本in成"));
}

正oid UMin成Pe本fo本設置anceBench設置a本k::UpdateMonito本in成Data()
{
    if (!bIsMonito本in成)
    {
        本et使本n;
    }
    
    CollectMonito本in成Data();
    UpdateMonito本in成Statistics();
    CheckPe本fo本設置anceTh本esholds();
}

bool UMin成Pe本fo本設置anceBench設置a本k::IsMonito本in成Acti正e() const
{
    本et使本n bIsMonito本in成;
}

TA本本ay<軍Min成Bench設置a本kRes使lt> UMin成Pe本fo本設置anceBench設置a本k::GetAllRes使lts() const
{
    TA本本ay<軍Min成Bench設置a本kRes使lt> Res使lts;
    fo本 (const a使to& Pai本 : Bench設置a本kRes使lts)
    {
        Res使lts.Add(Pai本.Val使e);
    }
    本et使本n Res使lts;
}

TA本本ay<軍Min成Bench設置a本kRes使lt> UMin成Pe本fo本設置anceBench設置a本k::GetPassedRes使lts() const
{
    TA本本ay<軍Min成Bench設置a本kRes使lt> Res使lts;
    fo本 (const a使to& Pai本 : Bench設置a本kRes使lts)
    {
        if (Pai本.Val使e.bTa本成etMet)
        {
            Res使lts.Add(Pai本.Val使e);
        }
    }
    本et使本n Res使lts;
}

TA本本ay<軍Min成Bench設置a本kRes使lt> UMin成Pe本fo本設置anceBench設置a本k::Get軍ailedRes使lts() const
{
    TA本本ay<軍Min成Bench設置a本kRes使lt> Res使lts;
    fo本 (const a使to& Pai本 : Bench設置a本kRes使lts)
    {
        if (!Pai本.Val使e.bTa本成etMet)
        {
            Res使lts.Add(Pai本.Val使e);
        }
    }
    本et使本n Res使lts;
}

float UMin成Pe本fo本設置anceBench設置a本k::GetO正e本allPe本fo本設置anceSco本e() const
{
    if (Bench設置a本kRes使lts.的使設置() == 0)
    {
        本et使本n 0.0f;
    }
    
    float TotalSco本e = 0.0f;
    int32 Res使ltCo使nt = 0;
    
    fo本 (const a使to& Pai本 : Bench設置a本kRes使lts)
    {
        const 軍Min成Bench設置a本kRes使lt& Res使lt = Pai本.Val使e;
        TotalSco本e += Calc使latePe本fo本設置anceSco本e(Res使lt);
        Res使ltCo使nt++;
    }
    
    本et使本n Res使ltCo使nt > 0 基本 TotalSco本e / Res使ltCo使nt : 0.0f;
}

bool UMin成Pe本fo本設置anceBench設置a本k::MeetsAllTa本成ets() const
{
    本et使本n Get軍ailedRes使lts().的使設置() == 0;
}

正oid UMin成Pe本fo本設置anceBench設置a本k::Gene本atePe本fo本設置anceRepo本t()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本atin成 co設置p本ehensi正e pe本fo本設置ance 本epo本t"));
    
    // C本eate detailed pe本fo本設置ance 本epo本t
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== MI的GGO RTS PER軍ORMA的CE REPORT ==="));
    
    // O正e本all pe本fo本設置ance
    float O正e本allSco本e = GetO正e本allPe本fo本設置anceSco本e();
    bool bAllTa本成etsMet = MeetsAllTa本成ets();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("O正e本all Pe本fo本設置ance Sco本e: %.1f/100"), O正e本allSco本e);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("All Ta本成ets Met: %s"), bAllTa本成etsMet 基本 TEXT("YES") : TEXT("的O"));
    
    // Indi正id使al bench設置a本k 本es使lts
    TA本本ay<軍Min成Bench設置a本kRes使lt> AllRes使lts = GetAllRes使lts();
    fo本 (const 軍Min成Bench設置a本kRes使lt& Res使lt : AllRes使lts)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT(""));
        UE下LOG(Lo成Te設置p, Lo成, TEXT("BE的C輸入MARK: %s"), *Res使lt.Bench設置a本k的a設置e);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("  Type: %s"), *GetBench設置a本kType的a設置e(Res使lt.Bench設置a本kType));
        UE下LOG(Lo成Te設置p, Lo成, TEXT("  Meas使本ed: %.2f %s", Res使lt.Meas使本edVal使e, *Res使lt.Unit);
        UE下LOG(Te設置p, Lo成, TEXT("  Ta本成et: %.2f %s", Res使lt.Ta本成etVal使e, *Res使lt.Unit);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("  Stat使s: %s"), *Res使lt.Stat使s);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("  Pe本fo本設置ance Ratio: %.2f"), Res使lt.Pe本fo本設置anceRatio);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("  A正e本a成e: %.2f %s", Res使lt.A正e本a成eVal使e, *Res使lt.Unit);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("  Min: %.2f %s", Res使lt.MinVal使e, *Res使lt.Unit);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("  Max: %.2f %s", Res使lt.MaxVal使e, *Res使lt.Unit);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("  Std De正: %.2f %s", Res使lt.Standa本dDe正iation, *Res使lt.Unit);
    }
    
    // Analysis
    AnalyzePe本fo本設置anceT本ends();
    IdentifyPe本fo本設置anceBottlenecks();
    Gene本ateOpti設置izationS使成成estions();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== E的D PER軍ORMA的CE REPORT ==="));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance 本epo本t 成ene本ated"));
}

正oid UMin成Pe本fo本設置anceBench設置a本k::Expo本tBench設置a本kRes使lts(const 軍St本in成& 軍ilePath)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Expo本tin成 bench設置a本k 本es使lts to: %s"), *軍ilePath);
    
    軍St本in成 Repo本tData = Sa正eBench設置a本kData();
    
    // This wo使ld w本ite the 本epo本t data to file
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Bench設置a本k 本es使lts expo本ted s使ccessf使lly"));
}

正oid UMin成Pe本fo本設置anceBench設置a本k::SetBench設置a本kTa本成et(EMin成Bench設置a本kType Bench設置a本kType, float Ta本成etVal使e)
{
    Bench設置a本kTa本成ets.Add(Bench設置a本kType, Ta本成etVal使e);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Bench設置a本k ta本成et set: %s = %.2f"), 
           *GetBench設置a本kType的a設置e(Bench設置a本kType), Ta本成etVal使e);
}

float UMin成Pe本fo本設置anceBench設置a本k::GetBench設置a本kTa本成et(EMin成Bench設置a本kType Bench設置a本kType) const
{
    const float* Ta本成et = Bench設置a本kTa本成ets.軍ind(Bench設置a本kType);
    本et使本n Ta本成et 基本 *Ta本成et : 0.0f;
}

正oid UMin成Pe本fo本設置anceBench設置a本k::LoadDefa使ltTa本成ets()
{
    InitializeBench設置a本kTa本成ets();
}

軍St本in成 UMin成Pe本fo本設置anceBench設置a本k::GetBench設置a本kType的a設置e(EMin成Bench設置a本kType Bench設置a本kType)
{
    switch (Bench設置a本kType)
    {
    case EMin成Bench設置a本kType::LoadTi設置e: 本et使本n TEXT("載入時間");
    case EMin成Bench設置a本kType::Me設置o本yUsa成e: 本et使本n TEXT("記憶體使用");
    case EMin成Bench設置a本kType::軍PS: 本et使本n TEXT("幀率");
    case EMin成Bench設置a本kType::CPU: 本et使本n TEXT("CPU使用");
    case EMin成Bench設置a本kType::GPU: 本et使本n TEXT("GPU使用");
    case EMin成Bench設置a本kType::的etwo本k: 本et使本n TEXT("網路延遲");
    case EMin成Bench設置a本kType::DiskIO: 本et使本n TEXT("磁碟I/O");
    case EMin成Bench設置a本kType::E正entLatency: 本et使本n TEXT("事件延遲");
    defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成Pe本fo本設置anceBench設置a本k::Sa正eBench設置a本kData() const
{
    軍St本in成 Res使lt = TEXT("{\n");
    Res使lt += 軍St本in成::P本intf(TEXT("  \"total下bench設置a本ks\": %d,\n"), Bench設置a本kRes使lts.的使設置());
    Res使lt += 軍St本in成::P本intf(TEXT("  \"passed下bench設置a本ks\": %d,\n"), GetPassedRes使lts().的使設置());
    Res使lt += 軍St本in成::P本intf(TEXT("  \"failed下bench設置a本ks\": %d,\n"), Get軍ailedRes使lts().的使設置());
    Res使lt += 軍St本in成::P本intf(TEXT("  \"o正e本all下sco本e\": %.1f,\n"), GetO正e本allPe本fo本設置anceSco本e());
    Res使lt += 軍St本in成::P本intf(TEXT("  \"all下ta本成ets下設置et\": %s,\n"), MeetsAllTa本成ets() 基本 TEXT("t本使e") : TEXT("false"));
    Res使lt += 軍St本in成::P本intf(TEXT("  \"設置onito本in成下acti正e\": %s\n"), bIsMonito本in成 基本 TEXT("t本使e") : TEXT("false"));
    Res使lt += TEXT("}\n");
    
    本et使本n Res使lt;
}

正oid UMin成Pe本fo本設置anceBench設置a本k::LoadBench設置a本kData(const 軍St本in成& JsonSt本in成)
{
    // Pa本se JSO的 and 本esto本e bench設置a本k data
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loadin成 bench設置a本k data"));
}

正oid UMin成Pe本fo本設置anceBench設置a本k::CollectMonito本in成Data()
{
    // Collect pe本fo本設置ance data fo本 contin使o使s 設置onito本in成
    CollectPe本fo本設置anceSa設置ple(EMin成Bench設置a本kType::軍PS);
    CollectPe本fo本設置anceSa設置ple(EMin成Bench設置a本kType::Me設置o本yUsa成e);
    CollectPe本fo本設置anceSa設置ple(EMin成Bench設置a本kType::CPU);
    CollectPe本fo本設置anceSa設置ple(EMin成Bench設置a本kType::GPU);
}

正oid UMin成Pe本fo本設置anceBench設置a本k::UpdateMonito本in成Statistics()
{
    // Update 設置onito本in成 statistics
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Updatin成 設置onito本in成 statistics"));
}

正oid UMin成Pe本fo本設置anceBench設置a本k::CheckPe本fo本設置anceTh本esholds()
{
    // Check if pe本fo本設置ance th本esholds a本e exceeded
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Checkin成 pe本fo本設置ance th本esholds"));
}

正oid UMin成Pe本fo本設置anceBench設置a本k::CollectPe本fo本設置anceSa設置ple(EMin成Bench設置a本kType Bench設置a本kType)
{
    // Collect a sin成le pe本fo本設置ance sa設置ple
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Collectin成 pe本fo本設置ance sa設置ple: %s"), *GetBench設置a本kType的a設置e(Bench設置a本kType));
}

正oid UMin成Pe本fo本設置anceBench設置a本k::AnalyzePe本fo本設置anceT本ends()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Analyzin成 pe本fo本設置ance t本ends"));
    
    // Analyze pe本fo本設置ance t本ends o正e本 ti設置e
    // This wo使ld look at histo本ical data and identify t本ends
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance t本end analysis co設置pleted"));
}

正oid UMin成Pe本fo本設置anceBench設置a本k::IdentifyPe本fo本設置anceBottlenecks()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Identifyin成 pe本fo本設置ance bottlenecks"));
    
    // Identify pe本fo本設置ance bottlenecks
    TA本本ay<軍Min成Bench設置a本kRes使lt> 軍ailedRes使lts = Get軍ailedRes使lts();
    
    fo本 (const 軍Min成Bench設置a本kRes使lt& Res使lt : 軍ailedRes使lts)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("PER軍ORMA的CE BOTTLE的ECK: %s - %.2f %s (ta本成et: %.2f %s)"), 
               *Res使lt.Bench設置a本k的a設置e, Res使lt.Meas使本edVal使e, *Res使lt.Unit, 
               Res使lt.Ta本成etVal使e, *Res使lt.Unit);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance bottleneck identification co設置pleted"));
}

正oid UMin成Pe本fo本設置anceBench設置a本k::Gene本ateOpti設置izationS使成成estions()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本atin成 opti設置ization s使成成estions"));
    
    // Gene本ate opti設置ization s使成成estions based on 本es使lts
    TA本本ay<軍Min成Bench設置a本kRes使lt> AllRes使lts = GetAllRes使lts();
    
    fo本 (const 軍Min成Bench設置a本kRes使lt& Res使lt : AllRes使lts)
    {
        if (!Res使lt.bTa本成etMet)
        {
            軍St本in成 S使成成estion;
            
            switch (Res使lt.Bench設置a本kType)
            {
            case EMin成Bench設置a本kType::LoadTi設置e:
                S使成成estion = TEXT("Opti設置ize asset loadin成 and conside本 async loadin成");
                b本eak;
            case EMin成Bench設置a本kType::Me設置o本yUsa成e:
                S使成成estion = TEXT("Red使ce 設置e設置o本y 使sa成e th本o使成h text使本e co設置p本ession and asset poolin成");
                b本eak;
            case EMin成Bench設置a本kType::軍PS:
                S使成成estion = TEXT("Opti設置ize 本ende本in成 pipeline and 本ed使ce d本aw calls");
                b本eak;
            case EMin成Bench設置a本kType::CPU:
                S使成成estion = TEXT("Opti設置ize AI calc使lations and 本ed使ce CPU-intensi正e ope本ations");
                b本eak;
            case EMin成Bench設置a本kType::GPU:
                S使成成estion = TEXT("Opti設置ize shade本s and 本ed使ce o正e本d本aw");
                b本eak;
            case EMin成Bench設置a本kType::的etwo本k:
                S使成成estion = TEXT("Opti設置ize netwo本k code and 本ed使ce packet size");
                b本eak;
            case EMin成Bench設置a本kType::DiskIO:
                S使成成estion = TEXT("Opti設置ize file access patte本ns and 使se cachin成");
                b本eak;
            case EMin成Bench設置a本kType::E正entLatency:
                S使成成estion = TEXT("Opti設置ize e正ent syste設置 and 本ed使ce e正ent p本ocessin成 o正e本head");
                b本eak;
            }
            
            UE下LOG(Lo成Te設置p, Lo成, TEXT("OPTIMIZATIO的 SUGGESTIO的: %s - %s"), *Res使lt.Bench設置a本k的a設置e, *S使成成estion);
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Opti設置ization s使成成estions 成ene本ated"));
}

正oid UMin成Pe本fo本設置anceBench設置a本k::C本eatePe本fo本設置anceS使設置設置a本y()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eatin成 pe本fo本設置ance s使設置設置a本y"));
    
    float O正e本allSco本e = GetO正e本allPe本fo本設置anceSco本e();
    bool bAllTa本成etsMet = MeetsAllTa本成ets();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== PER軍ORMA的CE SUMMARY ==="));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("O正e本all Sco本e: %.1f/100"), O正e本allSco本e);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ta本成ets Met: %d/%d"), GetPassedRes使lts().的使設置(), Bench設置a本kRes使lts.的使設置());
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stat使s: %s"), bAllTa本成etsMet 基本 TEXT("OPTIMAL") : TEXT("的EEDS OPTIMIZATIO的"));
    
    if (!bAllTa本成etsMet)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("P本io本ity A本eas fo本 Opti設置ization:"));
        
        TA本本ay<軍Min成Bench設置a本kRes使lt> 軍ailedRes使lts = Get軍ailedRes使lts();
        fo本 (const 軍Min成Bench設置a本kRes使lt& Res使lt : 軍ailedRes使lts)
        {
            float I設置pact = Res使lt.Pe本fo本設置anceRatio - 1.0f; // 輸入ow 設置使ch o正e本 ta本成et
            UE下LOG(Lo成Te設置p, Lo成, TEXT("  - %s (%.1f%% o正e本 ta本成et)"), *Res使lt.Bench設置a本k的a設置e, I設置pact * 100.0f);
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== E的D SUMMARY ==="));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance s使設置設置a本y c本eated"));
}

軍Min成Bench設置a本kRes使lt* UMin成Pe本fo本設置anceBench設置a本k::軍indBench設置a本kRes使lt(const 軍St本in成& Bench設置a本k的a設置e)
{
    本et使本n Bench設置a本kRes使lts.軍ind(Bench設置a本k的a設置e);
}

正oid UMin成Pe本fo本設置anceBench設置a本k::AddBench設置a本kRes使lt(const 軍Min成Bench設置a本kRes使lt& Res使lt)
{
    Bench設置a本kRes使lts.Add(Res使lt.Bench設置a本k的a設置e, Res使lt);
}

正oid UMin成Pe本fo本設置anceBench設置a本k::Lo成Bench設置a本kE正ent(const 軍St本in成& E正ent, const 軍St本in成& Details)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("[Bench設置a本k] %s: %s"), *E正ent, *Details);
}

float UMin成Pe本fo本設置anceBench設置a本k::Calc使latePe本fo本設置anceSco本e(const 軍Min成Bench設置a本kRes使lt& Res使lt) const
{
    if (Res使lt.Ta本成etVal使e <= 0.0f)
    {
        本et使本n 0.0f;
    }
    
    float Sco本e = 0.0f;
    
    switch (Res使lt.Bench設置a本kType)
    {
    case EMin成Bench設置a本kType::LoadTi設置e:
    case EMin成Bench設置a本kType::Me設置o本yUsa成e:
    case EMin成Bench設置a本kType::CPU:
    case EMin成Bench設置a本kType::GPU:
    case EMin成Bench設置a本kType::的etwo本k:
    case EMin成Bench設置a本kType::E正entLatency:
        // Lowe本 is bette本 - sco本e based on how 設置使ch 使nde本 ta本成et we a本e
        if (Res使lt.Meas使本edVal使e <= Res使lt.Ta本成etVal使e)
        {
            Sco本e = 100.0f;
        }
        else
        {
            Sco本e = 軍Math::Max(0.0f, 100.0f - ((Res使lt.Meas使本edVal使e / Res使lt.Ta本成etVal使e - 1.0f) * 100.0f));
        }
        b本eak;
    case EMin成Bench設置a本kType::軍PS:
    case EMin成Bench設置a本kType::DiskIO:
        // 輸入i成he本 is bette本 - sco本e based on how 設置使ch o正e本 ta本成et we a本e
        if (Res使lt.Meas使本edVal使e >= Res使lt.Ta本成etVal使e)
        {
            Sco本e = 100.0f;
        }
        else
        {
            Sco本e = 軍Math::Max(0.0f, (Res使lt.Meas使本edVal使e / Res使lt.Ta本成etVal使e) * 100.0f);
        }
        b本eak;
    }
    
    本et使本n 軍Math::Cla設置p(Sco本e, 0.0f, 100.0f);
}
