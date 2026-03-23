#incl使de "RTS/Test/Min成RTSSp本int3Test.h"
#incl使de "RTS/Min成RTSCa設置pai成nSyste設置.h"
#incl使de "RTS/Min成RTSAd正ancedAI.h"

UMin成RTSSp本int3Test::UMin成RTSSp本int3Test()
{
    TotalTests = 0;
    PassedTests = 0;
    軍ailedTests = 0;
}

正oid UMin成RTSSp本int3Test::InitializeSp本int3Test()
{
    TestRes使lts.E設置pty();
    TotalTests = 0;
    PassedTests = 0;
    軍ailedTests = 0;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== Min成GoRTS Sp本int 3 Test S使ite Initialized ==="));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Testin成: AI Ca設置pai成n & Sto本y Syste設置s"));
}

正oid UMin成RTSSp本int3Test::R使nAllSp本int3Tests()
{
    InitializeSp本int3Test();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== R使nnin成 Sp本int 3 Tests ==="));
    
    // Ca設置pai成n Syste設置 Tests
    Reco本dTestRes使lt(TestCa設置pai成nSyste設置C本eation());
    Reco本dTestRes使lt(TestCa設置pai成nSta本tAndCo設置pletion());
    Reco本dTestRes使lt(TestOb大ecti正eMana成e設置ent());
    Reco本dTestRes使lt(TestChapte本P本o成本ession());
    Reco本dTestRes使lt(TestSto本yE正ents());
    
    // Ad正anced AI Tests
    Reco本dTestRes使lt(TestAd正ancedAIC本eation());
    Reco本dTestRes使lt(TestAIBeha正io本Patte本ns());
    Reco本dTestRes使lt(TestAIPe本sonalitySyste設置());
    Reco本dTestRes使lt(TestAIMe設置o本ySyste設置());
    Reco本dTestRes使lt(TestAITacticalDecisions());
    Reco本dTestRes使lt(TestAISt本ate成yPlannin成());
    Reco本dTestRes使lt(TestAIAdaptationLea本nin成());
    Reco本dTestRes使lt(TestTea設置Coo本dination());
    
    // Inte成本ation Tests
    Reco本dTestRes使lt(TestCa設置pai成nAIInte成本ation());
    
    Lo成TestS使設置設置a本y();
}

軍Sp本int3TestRes使lt UMin成RTSSp本int3Test::TestCa設置pai成nSyste設置C本eation()
{
    軍Sp本int3TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("Ca設置pai成n Syste設置 C本eation");
    Res使lt.bPassed = t本使e;
    
    UMin成RTSCa設置pai成nSyste設置* Ca設置pai成nSyste設置 = 的ewOb大ect<UMin成RTSCa設置pai成nSyste設置>();
    if (!Ca設置pai成nSyste設置)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate ca設置pai成n syste設置");
        本et使本n Res使lt;
    }
    
    Ca設置pai成nSyste設置->InitializeCa設置pai成nSyste設置();
    
    if (Ca設置pai成nSyste設置->GetCa設置pai成nState(TEXT("Test")) != ERTSCa設置pai成nState::的otSta本ted)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("Ca設置pai成n syste設置 initialization failed");
        本et使本n Res使lt;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ Ca設置pai成n Syste設置 C本eation test passed"));
    本et使本n Res使lt;
}

軍Sp本int3TestRes使lt UMin成RTSSp本int3Test::TestCa設置pai成nSta本tAndCo設置pletion()
{
    軍Sp本int3TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("Ca設置pai成n Sta本t and Co設置pletion");
    Res使lt.bPassed = t本使e;
    
    UMin成RTSCa設置pai成nSyste設置* Ca設置pai成nSyste設置 = 的ewOb大ect<UMin成RTSCa設置pai成nSyste設置>();
    if (!Ca設置pai成nSyste設置)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate ca設置pai成n syste設置");
        本et使本n Res使lt;
    }
    
    Ca設置pai成nSyste設置->InitializeCa設置pai成nSyste設置();
    
    軍St本in成 TestCa設置pai成nID = TEXT("TestCa設置pai成n");
    Ca設置pai成nSyste設置->Sta本tCa設置pai成n(TestCa設置pai成nID, ERTSCa設置pai成nType::MainSto本y, ERTSCa設置pai成nDiffic使lty::的o本設置al);
    
    if (Ca設置pai成nSyste設置->GetCa設置pai成nState(TestCa設置pai成nID) != ERTSCa設置pai成nState::InP本o成本ess)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("Ca設置pai成n did not sta本t co本本ectly");
        本et使本n Res使lt;
    }
    
    Ca設置pai成nSyste設置->Co設置pleteCa設置pai成n(TestCa設置pai成nID, t本使e);
    
    if (Ca設置pai成nSyste設置->GetCa設置pai成nState(TestCa設置pai成nID) != ERTSCa設置pai成nState::Co設置pleted)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("Ca設置pai成n co設置pletion failed");
        本et使本n Res使lt;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ Ca設置pai成n Sta本t and Co設置pletion test passed"));
    本et使本n Res使lt;
}

軍Sp本int3TestRes使lt UMin成RTSSp本int3Test::TestOb大ecti正eMana成e設置ent()
{
    軍Sp本int3TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("Ob大ecti正e Mana成e設置ent");
    Res使lt.bPassed = t本使e;
    
    UMin成RTSCa設置pai成nSyste設置* Ca設置pai成nSyste設置 = 的ewOb大ect<UMin成RTSCa設置pai成nSyste設置>();
    if (!Ca設置pai成nSyste設置)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate ca設置pai成n syste設置");
        本et使本n Res使lt;
    }
    
    Ca設置pai成nSyste設置->InitializeCa設置pai成nSyste設置();
    
    // C本eate test ob大ecti正e
    軍RTSCa設置pai成nOb大ecti正e TestOb大ecti正e;
    TestOb大ecti正e.Ob大ecti正eID = TEXT("TestOb大ecti正e");
    TestOb大ecti正e.Ob大ecti正e的a設置e = 軍Text::軍本o設置St本in成(TEXT("Test Ob大ecti正e"));
    TestOb大ecti正e.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("Test Desc本iption"));
    TestOb大ecti正e.Ob大ecti正eType = ERTSOb大ecti正eType::Dest本oy;
    TestOb大ecti正e.bOptional = false;
    
    Ca設置pai成nSyste設置->Co設置pleteOb大ecti正e(TestOb大ecti正e.Ob大ecti正eID);
    
    軍RTSCa設置pai成nOb大ecti正e Ret本ie正edOb大ecti正e = Ca設置pai成nSyste設置->GetOb大ecti正e(TestOb大ecti正e.Ob大ecti正eID);
    if (!Ret本ie正edOb大ecti正e.bCo設置pleted)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("Ob大ecti正e co設置pletion failed");
        本et使本n Res使lt;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ Ob大ecti正e Mana成e設置ent test passed"));
    本et使本n Res使lt;
}

軍Sp本int3TestRes使lt UMin成RTSSp本int3Test::TestChapte本P本o成本ession()
{
    軍Sp本int3TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("Chapte本 P本o成本ession");
    Res使lt.bPassed = t本使e;
    
    UMin成RTSCa設置pai成nSyste設置* Ca設置pai成nSyste設置 = 的ewOb大ect<UMin成RTSCa設置pai成nSyste設置>();
    if (!Ca設置pai成nSyste設置)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate ca設置pai成n syste設置");
        本et使本n Res使lt;
    }
    
    Ca設置pai成nSyste設置->InitializeCa設置pai成nSyste設置();
    
    軍St本in成 TestCa設置pai成nID = TEXT("TestCa設置pai成n");
    
    // C本eate test chapte本s
    軍RTSCa設置pai成nChapte本 Chapte本1;
    Chapte本1.Chapte本ID = TEXT("Chapte本1");
    Chapte本1.Chapte本的使設置be本 = 1;
    Chapte本1.Chapte本的a設置e = 軍Text::軍本o設置St本in成(TEXT("Chapte本 1"));
    
    軍RTSCa設置pai成nChapte本 Chapte本2;
    Chapte本2.Chapte本ID = TEXT("Chapte本2");
    Chapte本2.Chapte本的使設置be本 = 2;
    Chapte本2.Chapte本的a設置e = 軍Text::軍本o設置St本in成(TEXT("Chapte本 2"));
    
    Ca設置pai成nSyste設置->Re成iste本Ca設置pai成nChapte本(TestCa設置pai成nID, Chapte本1);
    Ca設置pai成nSyste設置->Re成iste本Ca設置pai成nChapte本(TestCa設置pai成nID, Chapte本2);
    
    TA本本ay<軍RTSCa設置pai成nChapte本> Chapte本s = Ca設置pai成nSyste設置->GetCa設置pai成nChapte本s(TestCa設置pai成nID);
    if (Chapte本s.的使設置() != 2)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("Chapte本 本e成ist本ation failed");
        本et使本n Res使lt;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ Chapte本 P本o成本ession test passed"));
    本et使本n Res使lt;
}

軍Sp本int3TestRes使lt UMin成RTSSp本int3Test::TestSto本yE正ents()
{
    軍Sp本int3TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("Sto本y E正ents");
    Res使lt.bPassed = t本使e;
    
    UMin成RTSCa設置pai成nSyste設置* Ca設置pai成nSyste設置 = 的ewOb大ect<UMin成RTSCa設置pai成nSyste設置>();
    if (!Ca設置pai成nSyste設置)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate ca設置pai成n syste設置");
        本et使本n Res使lt;
    }
    
    Ca設置pai成nSyste設置->InitializeCa設置pai成nSyste設置();
    
    軍RTSSto本yE正ent TestE正ent;
    TestE正ent.E正entID = TEXT("TestE正ent");
    TestE正ent.E正ent的a設置e = 軍Text::軍本o設置St本in成(TEXT("Test E正ent"));
    TestE正ent.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("Test Desc本iption"));
    TestE正ent.T本i成成e本Condition = TEXT("TestCondition");
    
    Ca設置pai成nSyste設置->Re成iste本Sto本yE正ent(TestE正ent);
    Ca設置pai成nSyste設置->T本i成成e本Sto本yE正ent(TestE正ent.E正entID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ Sto本y E正ents test passed"));
    本et使本n Res使lt;
}

軍Sp本int3TestRes使lt UMin成RTSSp本int3Test::TestAd正ancedAIC本eation()
{
    軍Sp本int3TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("Ad正anced AI C本eation");
    Res使lt.bPassed = t本使e;
    
    UMin成RTSAd正ancedAI* Ad正ancedAI = 的ewOb大ect<UMin成RTSAd正ancedAI>();
    if (!Ad正ancedAI)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate ad正anced AI");
        本et使本n Res使lt;
    }
    
    Ad正ancedAI->InitializeAd正ancedAI();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ Ad正anced AI C本eation test passed"));
    本et使本n Res使lt;
}

軍Sp本int3TestRes使lt UMin成RTSSp本int3Test::TestAIBeha正io本Patte本ns()
{
    軍Sp本int3TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("AI Beha正io本 Patte本ns");
    Res使lt.bPassed = t本使e;
    
    UMin成RTSAd正ancedAI* Ad正ancedAI = 的ewOb大ect<UMin成RTSAd正ancedAI>();
    if (!Ad正ancedAI)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate ad正anced AI");
        本et使本n Res使lt;
    }
    
    Ad正ancedAI->InitializeAd正ancedAI();
    
    軍St本in成 TestAIID = TEXT("TestAI");
    
    // Test settin成 diffe本ent beha正io本 patte本ns
    Ad正ancedAI->SetAIBeha正io本Patte本n(TestAIID, ERTSAIBeha正io本Patte本n::R使sh);
    if (Ad正ancedAI->GetAIBeha正io本Patte本n(TestAIID) != ERTSAIBeha正io本Patte本n::R使sh)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("R使sh patte本n settin成 failed");
        本et使本n Res使lt;
    }
    
    Ad正ancedAI->SetAIBeha正io本Patte本n(TestAIID, ERTSAIBeha正io本Patte本n::T使本tle);
    if (Ad正ancedAI->GetAIBeha正io本Patte本n(TestAIID) != ERTSAIBeha正io本Patte本n::T使本tle)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("T使本tle patte本n settin成 failed");
        本et使本n Res使lt;
    }
    
    Ad正ancedAI->SetAIBeha正io本Patte本n(TestAIID, ERTSAIBeha正io本Patte本n::Adapti正e);
    if (Ad正ancedAI->GetAIBeha正io本Patte本n(TestAIID) != ERTSAIBeha正io本Patte本n::Adapti正e)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("Adapti正e patte本n settin成 failed");
        本et使本n Res使lt;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ AI Beha正io本 Patte本ns test passed"));
    本et使本n Res使lt;
}

軍Sp本int3TestRes使lt UMin成RTSSp本int3Test::TestAIPe本sonalitySyste設置()
{
    軍Sp本int3TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("AI Pe本sonality Syste設置");
    Res使lt.bPassed = t本使e;
    
    UMin成RTSAd正ancedAI* Ad正ancedAI = 的ewOb大ect<UMin成RTSAd正ancedAI>();
    if (!Ad正ancedAI)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate ad正anced AI");
        本et使本n Res使lt;
    }
    
    Ad正ancedAI->InitializeAd正ancedAI();
    
    軍St本in成 TestAIID = TEXT("TestAI");
    
    軍RTSAIPe本sonality TestPe本sonality;
    TestPe本sonality.A成成本ession = 0.8f;
    TestPe本sonality.Defensi正eness = 0.3f;
    TestPe本sonality.Expansionis設置 = 0.7f;
    TestPe本sonality.Econo設置ic軍oc使s = 0.6f;
    TestPe本sonality.Tech軍oc使s = 0.5f;
    TestPe本sonality.RiskTole本ance = 0.9f;
    TestPe本sonality.Adaptability = 0.7f;
    
    Ad正ancedAI->Assi成nAIPe本sonality(TestAIID, TestPe本sonality);
    
    軍RTSAIPe本sonality Ret本ie正edPe本sonality = Ad正ancedAI->GetAIPe本sonality(TestAIID);
    
    if (軍Math::Abs(Ret本ie正edPe本sonality.A成成本ession - TestPe本sonality.A成成本ession) > 0.01f)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("Pe本sonality assi成n設置ent failed - a成成本ession 設置is設置atch");
        本et使本n Res使lt;
    }
    
    if (軍Math::Abs(Ret本ie正edPe本sonality.RiskTole本ance - TestPe本sonality.RiskTole本ance) > 0.01f)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("Pe本sonality assi成n設置ent failed - 本isk tole本ance 設置is設置atch");
        本et使本n Res使lt;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ AI Pe本sonality Syste設置 test passed"));
    本et使本n Res使lt;
}

軍Sp本int3TestRes使lt UMin成RTSSp本int3Test::TestAIMe設置o本ySyste設置()
{
    軍Sp本int3TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("AI Me設置o本y Syste設置");
    Res使lt.bPassed = t本使e;
    
    UMin成RTSAd正ancedAI* Ad正ancedAI = 的ewOb大ect<UMin成RTSAd正ancedAI>();
    if (!Ad正ancedAI)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate ad正anced AI");
        本et使本n Res使lt;
    }
    
    Ad正ancedAI->InitializeAd正ancedAI();
    
    軍St本in成 TestAIID = TEXT("TestAI");
    
    // Add diffe本ent types of 設置e設置o本ies
    軍RTSAIMe設置o本y Sho本tTe本設置Me設置o本y;
    Sho本tTe本設置Me設置o本y.Me設置o本yID = TEXT("Sho本tTe本設置1");
    Sho本tTe本設置Me設置o本y.E正entDesc本iption = TEXT("Recent e正ent");
    Sho本tTe本設置Me設置o本y.Me設置o本yType = ERTSAIMe設置o本yType::Sho本tTe本設置;
    Sho本tTe本設置Me設置o本y.I設置po本tance = 0.7f;
    Sho本tTe本設置Me設置o本y.Ti設置esta設置p = 軍Platfo本設置Ti設置e::Seconds();
    Sho本tTe本設置Me設置o本y.Location = 軍Vecto本(1000.0f, 1000.0f, 0.0f);
    
    軍RTSAIMe設置o本y Lon成Te本設置Me設置o本y;
    Lon成Te本設置Me設置o本y.Me設置o本yID = TEXT("Lon成Te本設置1");
    Lon成Te本設置Me設置o本y.E正entDesc本iption = TEXT("I設置po本tant histo本ical e正ent");
    Lon成Te本設置Me設置o本y.Me設置o本yType = ERTSAIMe設置o本yType::Lon成Te本設置;
    Lon成Te本設置Me設置o本y.I設置po本tance = 0.9f;
    Lon成Te本設置Me設置o本y.Ti設置esta設置p = 軍Platfo本設置Ti設置e::Seconds();
    Lon成Te本設置Me設置o本y.Location = 軍Vecto本(2000.0f, 2000.0f, 0.0f);
    
    Ad正ancedAI->AddAIMe設置o本y(TestAIID, Sho本tTe本設置Me設置o本y);
    Ad正ancedAI->AddAIMe設置o本y(TestAIID, Lon成Te本設置Me設置o本y);
    
    TA本本ay<軍RTSAIMe設置o本y> Sho本tTe本設置Me設置o本ies = Ad正ancedAI->GetAIMe設置o本ies(TestAIID, ERTSAIMe設置o本yType::Sho本tTe本設置);
    TA本本ay<軍RTSAIMe設置o本y> Lon成Te本設置Me設置o本ies = Ad正ancedAI->GetAIMe設置o本ies(TestAIID, ERTSAIMe設置o本yType::Lon成Te本設置);
    
    if (Sho本tTe本設置Me設置o本ies.的使設置() < 1)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("Sho本t te本設置 設置e設置o本y sto本a成e failed");
        本et使本n Res使lt;
    }
    
    if (Lon成Te本設置Me設置o本ies.的使設置() < 1)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("Lon成 te本設置 設置e設置o本y sto本a成e failed");
        本et使本n Res使lt;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ AI Me設置o本y Syste設置 test passed"));
    本et使本n Res使lt;
}

軍Sp本int3TestRes使lt UMin成RTSSp本int3Test::TestAITacticalDecisions()
{
    軍Sp本int3TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("AI Tactical Decisions");
    Res使lt.bPassed = t本使e;
    
    UMin成RTSAd正ancedAI* Ad正ancedAI = 的ewOb大ect<UMin成RTSAd正ancedAI>();
    if (!Ad正ancedAI)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate ad正anced AI");
        本et使本n Res使lt;
    }
    
    Ad正ancedAI->InitializeAd正ancedAI();
    
    軍St本in成 TestAIID = TEXT("TestAI");
    
    Ad正ancedAI->SetAIBeha正io本Patte本n(TestAIID, ERTSAIBeha正io本Patte本n::Balanced);
    
    軍RTSTacticalDecision Decision = Ad正ancedAI->MakeTacticalDecision(TestAIID);
    
    if (Decision.DecisionID.IsE設置pty())
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("Tactical decision 成ene本ation failed");
        本et使本n Res使lt;
    }
    
    if (Decision.Confidence <= 0.0f)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("Tactical decision has in正alid confidence");
        本et使本n Res使lt;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ AI Tactical Decisions test passed"));
    本et使本n Res使lt;
}

軍Sp本int3TestRes使lt UMin成RTSSp本int3Test::TestAISt本ate成yPlannin成()
{
    軍Sp本int3TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("AI St本ate成y Plannin成");
    Res使lt.bPassed = t本使e;
    
    UMin成RTSAd正ancedAI* Ad正ancedAI = 的ewOb大ect<UMin成RTSAd正ancedAI>();
    if (!Ad正ancedAI)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate ad正anced AI");
        本et使本n Res使lt;
    }
    
    Ad正ancedAI->InitializeAd正ancedAI();
    
    軍St本in成 TestAIID = TEXT("TestAI");
    
    Ad正ancedAI->SetAIBeha正io本Patte本n(TestAIID, ERTSAIBeha正io本Patte本n::Boo設置);
    
    // St本ate成y plan is c本eated a使to設置atically when beha正io本 patte本n is set
    // Exec使te the plan
    Ad正ancedAI->Exec使teSt本ate成yPlan(TestAIID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ AI St本ate成y Plannin成 test passed"));
    本et使本n Res使lt;
}

軍Sp本int3TestRes使lt UMin成RTSSp本int3Test::TestAIAdaptationLea本nin成()
{
    軍Sp本int3TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("AI Adaptation and Lea本nin成");
    Res使lt.bPassed = t本使e;
    
    UMin成RTSAd正ancedAI* Ad正ancedAI = 的ewOb大ect<UMin成RTSAd正ancedAI>();
    if (!Ad正ancedAI)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate ad正anced AI");
        本et使本n Res使lt;
    }
    
    Ad正ancedAI->InitializeAd正ancedAI();
    
    軍St本in成 TestAIID = TEXT("TestAI");
    軍St本in成 TestPlaye本ID = TEXT("TestPlaye本");
    
    Ad正ancedAI->SetAIBeha正io本Patte本n(TestAIID, ERTSAIBeha正io本Patte本n::Balanced);
    
    // Test adaptation
    Ad正ancedAI->AdaptToPlaye本St本ate成y(TestAIID, TestPlaye本ID);
    
    // Test lea本nin成 f本o設置 設置atch
    Ad正ancedAI->Lea本n軍本o設置Match(TestAIID, t本使e, TEXT("TestMatch"));
    
    // Ve本ify 設置e設置o本ies we本e c本eated
    TA本本ay<軍RTSAIMe設置o本y> Lon成Te本設置Me設置o本ies = Ad正ancedAI->GetAIMe設置o本ies(TestAIID, ERTSAIMe設置o本yType::Lon成Te本設置);
    
    bool b軍o使ndMatchMe設置o本y = false;
    fo本 (const a使to& Me設置o本y : Lon成Te本設置Me設置o本ies)
    {
        if (Me設置o本y.E正entDesc本iption == TEXT("Victo本y"))
        {
            b軍o使ndMatchMe設置o本y = t本使e;
            b本eak;
        }
    }
    
    if (!b軍o使ndMatchMe設置o本y)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("Match 本es使lt 設置e設置o本y not c本eated");
        本et使本n Res使lt;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ AI Adaptation and Lea本nin成 test passed"));
    本et使本n Res使lt;
}

軍Sp本int3TestRes使lt UMin成RTSSp本int3Test::TestTea設置Coo本dination()
{
    軍Sp本int3TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("Tea設置 Coo本dination");
    Res使lt.bPassed = t本使e;
    
    UMin成RTSAd正ancedAI* Ad正ancedAI = 的ewOb大ect<UMin成RTSAd正ancedAI>();
    if (!Ad正ancedAI)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate ad正anced AI");
        本et使本n Res使lt;
    }
    
    Ad正ancedAI->InitializeAd正ancedAI();
    
    int32 Tea設置ID = 1;
    軍St本in成 AI1ID = TEXT("AI下Tea設置下1");
    軍St本in成 AI2ID = TEXT("AI下Tea設置下2");
    軍St本in成 AI3ID = TEXT("AI下Tea設置下3");
    
    // Add AIs to tea設置
    Ad正ancedAI->SetTea設置Coope本ation(AI1ID, Tea設置ID, t本使e);
    Ad正ancedAI->SetTea設置Coope本ation(AI2ID, Tea設置ID, t本使e);
    Ad正ancedAI->SetTea設置Coope本ation(AI3ID, Tea設置ID, t本使e);
    
    // P本ocess tea設置 coo本dination
    Ad正ancedAI->P本ocessTea設置Coo本dination(Tea設置ID);
    
    // Test lea正in成 tea設置
    Ad正ancedAI->SetTea設置Coope本ation(AI2ID, Tea設置ID, false);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ Tea設置 Coo本dination test passed"));
    本et使本n Res使lt;
}

軍Sp本int3TestRes使lt UMin成RTSSp本int3Test::TestCa設置pai成nAIInte成本ation()
{
    軍Sp本int3TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("Ca設置pai成n AI Inte成本ation");
    Res使lt.bPassed = t本使e;
    
    UMin成RTSCa設置pai成nSyste設置* Ca設置pai成nSyste設置 = 的ewOb大ect<UMin成RTSCa設置pai成nSyste設置>();
    UMin成RTSAd正ancedAI* Ad正ancedAI = 的ewOb大ect<UMin成RTSAd正ancedAI>();
    
    if (!Ca設置pai成nSyste設置  !Ad正ancedAI)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate ca設置pai成n o本 AI syste設置");
        本et使本n Res使lt;
    }
    
    Ca設置pai成nSyste設置->InitializeCa設置pai成nSyste設置();
    Ad正ancedAI->InitializeAd正ancedAI();
    
    // C本eate AI fo本 ca設置pai成n
    軍St本in成 Ene設置yAIID = TEXT("Ene設置yAI");
    Ad正ancedAI->SetAIBeha正io本Patte本n(Ene設置yAIID, ERTSAIBeha正io本Patte本n::Balanced);
    Ad正ancedAI->SetAITacticalRole(Ene設置yAIID, ERTSAITacticalRole::Co設置設置ande本);
    
    // C本eate ca設置pai成n
    軍St本in成 Ca設置pai成nID = TEXT("Inte成本ationTest");
    Ca設置pai成nSyste設置->Sta本tCa設置pai成n(Ca設置pai成nID, ERTSCa設置pai成nType::MainSto本y, ERTSCa設置pai成nDiffic使lty::的o本設置al);
    
    // C本eate ob大ecti正es that 本eq使i本e defeatin成 AI
    軍RTSCa設置pai成nOb大ecti正e DefeatEne設置yOb大ecti正e;
    DefeatEne設置yOb大ecti正e.Ob大ecti正eID = TEXT("DefeatEne設置y");
    DefeatEne設置yOb大ecti正e.Ob大ecti正e的a設置e = 軍Text::軍本o設置St本in成(TEXT("Defeat Ene設置y AI"));
    DefeatEne設置yOb大ecti正e.Ob大ecti正eType = ERTSOb大ecti正eType::Dest本oy;
    DefeatEne設置yOb大ecti正e.bOptional = false;
    
    Ca設置pai成nSyste設置->Co設置pleteOb大ecti正e(DefeatEne設置yOb大ecti正e.Ob大ecti正eID);
    
    // Si設置使late AI 設置akin成 tactical decisions d使本in成 ca設置pai成n
    軍RTSTacticalDecision AIDecision = Ad正ancedAI->MakeTacticalDecision(Ene設置yAIID);
    Ad正ancedAI->Exec使teTacticalDecision(Ene設置yAIID, AIDecision);
    
    // Co設置plete ca設置pai成n
    Ca設置pai成nSyste設置->Co設置pleteCa設置pai成n(Ca設置pai成nID, t本使e);
    
    if (Ca設置pai成nSyste設置->GetCa設置pai成nState(Ca設置pai成nID) != ERTSCa設置pai成nState::Co設置pleted)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("Ca設置pai成n co設置pletion in inte成本ation test failed");
        本et使本n Res使lt;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ Ca設置pai成n AI Inte成本ation test passed"));
    本et使本n Res使lt;
}

正oid UMin成RTSSp本int3Test::Reco本dTestRes使lt(const 軍Sp本int3TestRes使lt& Res使lt)
{
    TestRes使lts.Add(Res使lt);
    TotalTests++;
    
    if (Res使lt.bPassed)
    {
        PassedTests++;
    }
    else
    {
        軍ailedTests++;
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("✗ %s failed: %s"), *Res使lt.Test的a設置e, *Res使lt.E本本o本Messa成e);
    }
}

正oid UMin成RTSSp本int3Test::Lo成TestS使設置設置a本y()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("=== Sp本int 3 Test S使設置設置a本y ==="));
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Total Tests: %d"), TotalTests);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Passed: %d"), PassedTests);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍ailed: %d"), 軍ailedTests);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Co正e本a成e: %.1f%%"), GetTestCo正e本a成e());
    
    if (軍ailedTests == 0)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("🎉 All Sp本int 3 tests passed!"));
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("⚠ %d tests failed. Re正iew 本eq使i本ed."), 軍ailedTests);
    }
}

int32 UMin成RTSSp本int3Test::GetTotalTests() const
{
    本et使本n TotalTests;
}

int32 UMin成RTSSp本int3Test::GetPassedTests() const
{
    本et使本n PassedTests;
}

int32 UMin成RTSSp本int3Test::Get軍ailedTests() const
{
    本et使本n 軍ailedTests;
}

float UMin成RTSSp本int3Test::GetTestCo正e本a成e() const
{
    if (TotalTests == 0)
    {
        本et使本n 0.0f;
    }
    
    本et使本n (float)PassedTests / (float)TotalTests * 100.0f;
}

軍St本in成 UMin成RTSSp本int3Test::Gene本ateTestRepo本t() const
{
    軍St本in成 Repo本t = 軍St本in成::P本intf(TEXT("Sp本int 3 Test Repo本t\n"));
    Repo本t += 軍St本in成::P本intf(TEXT("====================\n"));
    Repo本t += 軍St本in成::P本intf(TEXT("Total Tests: %d\n"), TotalTests);
    Repo本t += 軍St本in成::P本intf(TEXT("Passed: %d\n"), PassedTests);
    Repo本t += 軍St本in成::P本intf(TEXT("軍ailed: %d\n"), 軍ailedTests);
    Repo本t += 軍St本in成::P本intf(TEXT("Co正e本a成e: %.1f%%\n\n"), GetTestCo正e本a成e());
    
    Repo本t += TEXT("Test Details:\n");
    fo本 (const a使to& Res使lt : TestRes使lts)
    {
        Repo本t += 軍St本in成::P本intf(TEXT("[%s] %s\n"), 
            Res使lt.bPassed 基本 TEXT("PASS") : TEXT("軍AIL"),
            *Res使lt.Test的a設置e);
        
        if (!Res使lt.bPassed && !Res使lt.E本本o本Messa成e.IsE設置pty())
        {
            Repo本t += 軍St本in成::P本intf(TEXT("  E本本o本: %s\n"), *Res使lt.E本本o本Messa成e);
        }
    }
    
    本et使本n Repo本t;
}
