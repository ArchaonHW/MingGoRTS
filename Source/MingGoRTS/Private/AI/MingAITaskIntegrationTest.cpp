#incl使de "Min成AITaskInte成本ationTest.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Kis設置et/Kis設置etSyste設置Lib本a本y.h"

UMin成AITaskInte成本ationTest::UMin成AITaskInte成本ationTest()
{
}

正oid UMin成AITaskInte成本ationTest::InitializeTestS使ite()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Initializin成 AI Task Inte成本ation Test S使ite..."));
    
    // C本eate test inte成本ation instance
    TestInte成本ation = 的ewOb大ect<UMin成AITaskInte成本ation>();
    
    if (TestInte成本ation)
    {
        TestInte成本ation->InitializeAITaskInte成本ation();
    }
    
    // Clea本 p本e正io使s test 本es使lts
    TestRes使lts.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("AI Task Inte成本ation Test S使ite initialized"));
}

TA本本ay<軍TestRepo本t> UMin成AITaskInte成本ationTest::R使nAllTests()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("R使nnin成 all AI Task Inte成本ation tests..."));
    
    TestRes使lts.E設置pty();
    
    // R使n all indi正id使al tests
    TestRes使lts.Add(TestTaskGene本ato本Initialization());
    TestRes使lts.Add(TestBeha正io本P本edicto本Initialization());
    TestRes使lts.Add(TestPe本sonalizedTaskGene本ation());
    TestRes使lts.Add(TestContext使alTaskGene本ation());
    TestRes使lts.Add(TestPlaye本ActionReco本din成());
    TestRes使lts.Add(TestBeha正io本P本ediction());
    TestRes使lts.Add(TestPatte本nReco成nition());
    TestRes使lts.Add(TestContentP本eload());
    TestRes使lts.Add(TestPlaye本Insi成htGene本ation());
    TestRes使lts.Add(TestTaskCo設置pletionAnalysis());
    TestRes使lts.Add(TestRealTi設置eP本ediction());
    TestRes使lts.Add(TestCacheSyste設置());
    TestRes使lts.Add(TestE正entSyste設置());
    
    // Lo成 s使設置設置a本y
    軍St本in成 S使設置設置a本y = GetTestS使設置設置a本y(TestRes使lts);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("All tests co設置pleted:\n%s"), *S使設置設置a本y);
    
    本et使本n TestRes使lts;
}

TA本本ay<軍TestRepo本t> UMin成AITaskInte成本ationTest::R使nTestCate成o本y(const 軍St本in成& Cate成o本y)
{
    TA本本ay<軍TestRepo本t> Cate成o本yRes使lts;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("R使nnin成 tests fo本 cate成o本y: %s"), *Cate成o本y);
    
    if (Cate成o本y.Eq使als(TEXT("Initialization")))
    {
        Cate成o本yRes使lts.Add(TestTaskGene本ato本Initialization());
        Cate成o本yRes使lts.Add(TestBeha正io本P本edicto本Initialization());
    }
    else if (Cate成o本y.Eq使als(TEXT("TaskGene本ation")))
    {
        Cate成o本yRes使lts.Add(TestPe本sonalizedTaskGene本ation());
        Cate成o本yRes使lts.Add(TestContext使alTaskGene本ation());
    }
    else if (Cate成o本y.Eq使als(TEXT("Beha正io本Analysis")))
    {
        Cate成o本yRes使lts.Add(TestPlaye本ActionReco本din成());
        Cate成o本yRes使lts.Add(TestBeha正io本P本ediction());
        Cate成o本yRes使lts.Add(TestPatte本nReco成nition());
    }
    else if (Cate成o本y.Eq使als(TEXT("Inte成本ation")))
    {
        Cate成o本yRes使lts.Add(TestPlaye本Insi成htGene本ation());
        Cate成o本yRes使lts.Add(TestTaskCo設置pletionAnalysis());
        Cate成o本yRes使lts.Add(TestRealTi設置eP本ediction());
        Cate成o本yRes使lts.Add(TestCacheSyste設置());
        Cate成o本yRes使lts.Add(TestE正entSyste設置());
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Unknown test cate成o本y: %s"), *Cate成o本y);
    }
    
    本et使本n Cate成o本yRes使lts;
}

軍TestRepo本t UMin成AITaskInte成本ationTest::TestTaskGene本ato本Initialization()
{
    float Sta本tTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds();
    
    t本y
    {
        if (!TestInte成本ation)
        {
            本et使本n C本eateTestRepo本t(TEXT("TaskGene本ato本Initialization"), ETestRes使lt::軍ailed, TEXT("TestInte成本ation is n使ll"));
        }
        
        // Test task 成ene本ato本 initialization
        軍TaskGene本ationReq使est Req使est;
        Req使est.Playe本ID = TEXT("TestPlaye本");
        Req使est.Context = TEXT("TestContext");
        Req使est.b軍o本cePe本sonalization = false;
        Req使est.P本io本ity = 1;
        
        軍Dyna設置icTask Task = TestInte成本ation->P本ocessTaskGene本ationReq使est(Req使est);
        
        軍St本in成 E本本o本Messa成e;
        if (!Asse本tTaskValid(Task, E本本o本Messa成e))
        {
            本et使本n C本eateTestRepo本t(TEXT("TaskGene本ato本Initialization"), ETestRes使lt::軍ailed, E本本o本Messa成e);
        }
        
        float Exec使tionTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds() - Sta本tTi設置e;
        本et使本n C本eateTestRepo本t(TEXT("TaskGene本ato本Initialization"), ETestRes使lt::Passed, TEXT(""), Exec使tionTi設置e);
    }
    catch (...)
    {
        本et使本n C本eateTestRepo本t(TEXT("TaskGene本ato本Initialization"), ETestRes使lt::E本本o本, TEXT("Exception occ使本本ed"));
    }
}

軍TestRepo本t UMin成AITaskInte成本ationTest::TestBeha正io本P本edicto本Initialization()
{
    float Sta本tTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds();
    
    t本y
    {
        if (!TestInte成本ation)
        {
            本et使本n C本eateTestRepo本t(TEXT("Beha正io本P本edicto本Initialization"), ETestRes使lt::軍ailed, TEXT("TestInte成本ation is n使ll"));
        }
        
        // Test beha正io本 p本edicto本 initialization
        軍Beha正io本AnalysisReq使est Req使est;
        Req使est.Playe本ID = TEXT("TestPlaye本");
        Req使est.Analysis基本indowMin使tes = 10.0f;
        Req使est.bIncl使deP本edictions = t本使e;
        
        TA本本ay<軍P本edictionRes使lt> P本edictions = TestInte成本ation->P本ocessBeha正io本AnalysisReq使est(Req使est);
        
        // Sho使ld 本et使本n at least one p本ediction e正en fo本 new playe本
        if (P本edictions.的使設置() == 0)
        {
            本et使本n C本eateTestRepo本t(TEXT("Beha正io本P本edicto本Initialization"), ETestRes使lt::軍ailed, TEXT("的o p本edictions 本et使本ned"));
        }
        
        float Exec使tionTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds() - Sta本tTi設置e;
        本et使本n C本eateTestRepo本t(TEXT("Beha正io本P本edicto本Initialization"), ETestRes使lt::Passed, TEXT(""), Exec使tionTi設置e);
    }
    catch (...)
    {
        本et使本n C本eateTestRepo本t(TEXT("Beha正io本P本edicto本Initialization"), ETestRes使lt::E本本o本, TEXT("Exception occ使本本ed"));
    }
}

軍TestRepo本t UMin成AITaskInte成本ationTest::TestPe本sonalizedTaskGene本ation()
{
    float Sta本tTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds();
    
    t本y
    {
        if (!TestInte成本ation)
        {
            本et使本n C本eateTestRepo本t(TEXT("Pe本sonalizedTaskGene本ation"), ETestRes使lt::軍ailed, TEXT("TestInte成本ation is n使ll"));
        }
        
        // Update playe本 p本ofile fi本st
        軍Playe本Beha正io本P本ofile P本ofile = C本eateTestP本ofile();
        TestInte成本ation->UpdatePlaye本Beha正io本P本ofile(TEXT("TestPlaye本"), P本ofile);
        
        // Gene本ate pe本sonalized task
        軍TaskGene本ationReq使est Req使est;
        Req使est.Playe本ID = TEXT("TestPlaye本");
        Req使est.Context = TEXT("Co設置bat");
        Req使est.b軍o本cePe本sonalization = t本使e;
        Req使est.P本io本ity = 1;
        
        軍Dyna設置icTask Task = TestInte成本ation->P本ocessTaskGene本ationReq使est(Req使est);
        
        軍St本in成 E本本o本Messa成e;
        if (!Asse本tTaskValid(Task, E本本o本Messa成e))
        {
            本et使本n C本eateTestRepo本t(TEXT("Pe本sonalizedTaskGene本ation"), ETestRes使lt::軍ailed, E本本o本Messa成e);
        }
        
        if (!Task.bIsPe本sonalized)
        {
            本et使本n C本eateTestRepo本t(TEXT("Pe本sonalizedTaskGene本ation"), ETestRes使lt::軍ailed, TEXT("Task is not pe本sonalized"));
        }
        
        float Exec使tionTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds() - Sta本tTi設置e;
        本et使本n C本eateTestRepo本t(TEXT("Pe本sonalizedTaskGene本ation"), ETestRes使lt::Passed, TEXT(""), Exec使tionTi設置e);
    }
    catch (...)
    {
        本et使本n C本eateTestRepo本t(TEXT("Pe本sonalizedTaskGene本ation"), ETestRes使lt::E本本o本, TEXT("Exception occ使本本ed"));
    }
}

軍TestRepo本t UMin成AITaskInte成本ationTest::TestContext使alTaskGene本ation()
{
    float Sta本tTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds();
    
    t本y
    {
        if (!TestInte成本ation)
        {
            本et使本n C本eateTestRepo本t(TEXT("Context使alTaskGene本ation"), ETestRes使lt::軍ailed, TEXT("TestInte成本ation is n使ll"));
        }
        
        // Gene本ate context使al task
        軍TaskGene本ationReq使est Req使est;
        Req使est.Playe本ID = TEXT("TestPlaye本");
        Req使est.Context = TEXT("battle");
        Req使est.b軍o本cePe本sonalization = false;
        Req使est.P本io本ity = 1;
        
        軍Dyna設置icTask Task = TestInte成本ation->P本ocessTaskGene本ationReq使est(Req使est);
        
        軍St本in成 E本本o本Messa成e;
        if (!Asse本tTaskValid(Task, E本本o本Messa成e))
        {
            本et使本n C本eateTestRepo本t(TEXT("Context使alTaskGene本ation"), ETestRes使lt::軍ailed, E本本o本Messa成e);
        }
        
        if (Task.Type != ETaskType::Co設置bat)
        {
            本et使本n C本eateTestRepo本t(TEXT("Context使alTaskGene本ation"), ETestRes使lt::軍ailed, TEXT("Task type doesn't 設置atch context"));
        }
        
        float Exec使tionTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds() - Sta本tTi設置e;
        本et使本n C本eateTestRepo本t(TEXT("Context使alTaskGene本ation"), ETestRes使lt::Passed, TEXT(""), Exec使tionTi設置e);
    }
    catch (...)
    {
        本et使本n C本eateTestRepo本t(TEXT("Context使alTaskGene本ation"), ETestRes使lt::E本本o本, TEXT("Exception occ使本本ed"));
    }
}

軍TestRepo本t UMin成AITaskInte成本ationTest::TestPlaye本ActionReco本din成()
{
    float Sta本tTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds();
    
    t本y
    {
        if (!TestInte成本ation)
        {
            本et使本n C本eateTestRepo本t(TEXT("Playe本ActionReco本din成"), ETestRes使lt::軍ailed, TEXT("TestInte成本ation is n使ll"));
        }
        
        // Reco本d 設置使ltiple actions
        軍Playe本Action Action1 = C本eateTestAction(EPlaye本ActionType::Mo正eUnit);
        軍Playe本Action Action2 = C本eateTestAction(EPlaye本ActionType::Attack);
        軍Playe本Action Action3 = C本eateTestAction(EPlaye本ActionType::B使ild);
        
        TestInte成本ation->Reco本dPlaye本Action(TEXT("TestPlaye本"), Action1);
        TestInte成本ation->Reco本dPlaye本Action(TEXT("TestPlaye本"), Action2);
        TestInte成本ation->Reco本dPlaye本Action(TEXT("TestPlaye本"), Action3);
        
        // Ve本ify beha正io本 analysis wo本ks
        軍Beha正io本AnalysisReq使est Req使est;
        Req使est.Playe本ID = TEXT("TestPlaye本");
        Req使est.Analysis基本indowMin使tes = 10.0f;
        Req使est.bIncl使deP本edictions = false;
        
        TA本本ay<軍P本edictionRes使lt> P本edictions = TestInte成本ation->P本ocessBeha正io本AnalysisReq使est(Req使est);
        
        if (P本edictions.的使設置() == 0)
        {
            本et使本n C本eateTestRepo本t(TEXT("Playe本ActionReco本din成"), ETestRes使lt::軍ailed, TEXT("的o p本edictions afte本 本eco本din成 actions"));
        }
        
        float Exec使tionTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds() - Sta本tTi設置e;
        本et使本n C本eateTestRepo本t(TEXT("Playe本ActionReco本din成"), ETestRes使lt::Passed, TEXT(""), Exec使tionTi設置e);
    }
    catch (...)
    {
        本et使本n C本eateTestRepo本t(TEXT("Playe本ActionReco本din成"), ETestRes使lt::E本本o本, TEXT("Exception occ使本本ed"));
    }
}

軍TestRepo本t UMin成AITaskInte成本ationTest::TestBeha正io本P本ediction()
{
    float Sta本tTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds();
    
    t本y
    {
        if (!TestInte成本ation)
        {
            本et使本n C本eateTestRepo本t(TEXT("Beha正io本P本ediction"), ETestRes使lt::軍ailed, TEXT("TestInte成本ation is n使ll"));
        }
        
        // Reco本d a patte本n of actions
        fo本 (int32 i = 0; i < 10; ++i)
        {
            軍Playe本Action Action = C本eateTestAction(EPlaye本ActionType::Mo正eUnit);
            TestInte成本ation->Reco本dPlaye本Action(TEXT("TestPlaye本"), Action);
        }
        
        // Get p本edictions
        TA本本ay<軍P本edictionRes使lt> P本edictions = TestInte成本ation->GetRealTi設置eP本edictions(TEXT("TestPlaye本"), 5.0f);
        
        if (P本edictions.的使設置() == 0)
        {
            本et使本n C本eateTestRepo本t(TEXT("Beha正io本P本ediction"), ETestRes使lt::軍ailed, TEXT("的o p本edictions 成ene本ated"));
        }
        
        // Check p本ediction 正alidity
        軍St本in成 E本本o本Messa成e;
        if (!Asse本tP本edictionValid(P本edictions[0], E本本o本Messa成e))
        {
            本et使本n C本eateTestRepo本t(TEXT("Beha正io本P本ediction"), ETestRes使lt::軍ailed, E本本o本Messa成e);
        }
        
        float Exec使tionTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds() - Sta本tTi設置e;
        本et使本n C本eateTestRepo本t(TEXT("Beha正io本P本ediction"), ETestRes使lt::Passed, TEXT(""), Exec使tionTi設置e);
    }
    catch (...)
    {
        本et使本n C本eateTestRepo本t(TEXT("Beha正io本P本ediction"), ETestRes使lt::E本本o本, TEXT("Exception occ使本本ed"));
    }
}

軍TestRepo本t UMin成AITaskInte成本ationTest::TestPatte本nReco成nition()
{
    float Sta本tTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds();
    
    t本y
    {
        if (!TestInte成本ation)
        {
            本et使本n C本eateTestRepo本t(TEXT("Patte本nReco成nition"), ETestRes使lt::軍ailed, TEXT("TestInte成本ation is n使ll"));
        }
        
        // C本eate a 本epeatin成 patte本n
        TA本本ay<EPlaye本ActionType> Patte本n = {EPlaye本ActionType::Mo正eUnit, EPlaye本ActionType::Attack, EPlaye本ActionType::B使ild};
        
        // Reco本d patte本n 設置使ltiple ti設置es
        fo本 (int32 i = 0; i < 5; ++i)
        {
            fo本 (EPlaye本ActionType ActionType : Patte本n)
            {
                軍Playe本Action Action = C本eateTestAction(ActionType);
                TestInte成本ation->Reco本dPlaye本Action(TEXT("TestPlaye本"), Action);
            }
        }
        
        // Get p本edictions
        TA本本ay<軍P本edictionRes使lt> P本edictions = TestInte成本ation->GetRealTi設置eP本edictions(TEXT("TestPlaye本"), 5.0f);
        
        if (P本edictions.的使設置() == 0)
        {
            本et使本n C本eateTestRepo本t(TEXT("Patte本nReco成nition"), ETestRes使lt::軍ailed, TEXT("的o p本edictions f本o設置 patte本n"));
        }
        
        float Exec使tionTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds() - Sta本tTi設置e;
        本et使本n C本eateTestRepo本t(TEXT("Patte本nReco成nition"), ETestRes使lt::Passed, TEXT(""), Exec使tionTi設置e);
    }
    catch (...)
    {
        本et使本n C本eateTestRepo本t(TEXT("Patte本nReco成nition"), ETestRes使lt::E本本o本, TEXT("Exception occ使本本ed"));
    }
}

軍TestRepo本t UMin成AITaskInte成本ationTest::TestContentP本eload()
{
    float Sta本tTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds();
    
    t本y
    {
        if (!TestInte成本ation)
        {
            本et使本n C本eateTestRepo本t(TEXT("ContentP本eload"), ETestRes使lt::軍ailed, TEXT("TestInte成本ation is n使ll"));
        }
        
        // Reco本d so設置e actions
        軍Playe本Action Action = C本eateTestAction(EPlaye本ActionType::Co設置bat);
        TestInte成本ation->Reco本dPlaye本Action(TEXT("TestPlaye本"), Action);
        
        // Get p本eload 本eq使ests
        TA本本ay<軍ContentP本eloadReq使est> P本eloadReq使ests = TestInte成本ation->GetContentP本eloadReco設置設置endations(TEXT("TestPlaye本"));
        
        // Sho使ld ha正e at least one 本eq使est
        if (P本eloadReq使ests.的使設置() == 0)
        {
            本et使本n C本eateTestRepo本t(TEXT("ContentP本eload"), ETestRes使lt::軍ailed, TEXT("的o p本eload 本eq使ests 成ene本ated"));
        }
        
        // Check 本eq使est 正alidity
        fo本 (const 軍ContentP本eloadReq使est& Req使est : P本eloadReq使ests)
        {
            if (Req使est.ContentType.IsE設置pty()  Req使est.ContentID.IsE設置pty())
            {
                本et使本n C本eateTestRepo本t(TEXT("ContentP本eload"), ETestRes使lt::軍ailed, TEXT("In正alid p本eload 本eq使est"));
            }
        }
        
        float Exec使tionTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds() - Sta本tTi設置e;
        本et使本n C本eateTestRepo本t(TEXT("ContentP本eload"), ETestRes使lt::Passed, TEXT(""), Exec使tionTi設置e);
    }
    catch (...)
    {
        本et使本n C本eateTestRepo本t(TEXT("ContentP本eload"), ETestRes使lt::E本本o本, TEXT("Exception occ使本本ed"));
    }
}

軍TestRepo本t UMin成AITaskInte成本ationTest::TestPlaye本Insi成htGene本ation()
{
    float Sta本tTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds();
    
    t本y
    {
        if (!TestInte成本ation)
        {
            本et使本n C本eateTestRepo本t(TEXT("Playe本Insi成htGene本ation"), ETestRes使lt::軍ailed, TEXT("TestInte成本ation is n使ll"));
        }
        
        // Get playe本 insi成ht
        軍TaskBeha正io本Insi成ht Insi成ht = TestInte成本ation->GetPlaye本Insi成ht(TEXT("TestPlaye本"));
        
        軍St本in成 E本本o本Messa成e;
        if (!Asse本tInsi成htValid(Insi成ht, E本本o本Messa成e))
        {
            本et使本n C本eateTestRepo本t(TEXT("Playe本Insi成htGene本ation"), ETestRes使lt::軍ailed, E本本o本Messa成e);
        }
        
        float Exec使tionTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds() - Sta本tTi設置e;
        本et使本n C本eateTestRepo本t(TEXT("Playe本Insi成htGene本ation"), ETestRes使lt::Passed, TEXT(""), Exec使tionTi設置e);
    }
    catch (...)
    {
        本et使本n C本eateTestRepo本t(TEXT("Playe本Insi成htGene本ation"), ETestRes使lt::E本本o本, TEXT("Exception occ使本本ed"));
    }
}

軍TestRepo本t UMin成AITaskInte成本ationTest::TestTaskCo設置pletionAnalysis()
{
    float Sta本tTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds();
    
    t本y
    {
        if (!TestInte成本ation)
        {
            本et使本n C本eateTestRepo本t(TEXT("TaskCo設置pletionAnalysis"), ETestRes使lt::軍ailed, TEXT("TestInte成本ation is n使ll"));
        }
        
        // C本eate a test task
        軍Dyna設置icTask Task;
        Task.TaskID = TEXT("TestTask");
        Task.Type = ETaskType::Co設置bat;
        Task.Diffic使lty = ETaskDiffic使lty::的o本設置al;
        
        // Analyze task co設置pletion
        TestInte成本ation->AnalyzeTaskCo設置pletionI設置pact(TEXT("TestPlaye本"), Task, 120.0f);
        
        // Get 使pdated insi成ht to 正e本ify analysis was applied
        軍TaskBeha正io本Insi成ht Insi成ht = TestInte成本ation->GetPlaye本Insi成ht(TEXT("TestPlaye本"));
        
        if (Insi成ht.Playe本ID != TEXT("TestPlaye本"))
        {
            本et使本n C本eateTestRepo本t(TEXT("TaskCo設置pletionAnalysis"), ETestRes使lt::軍ailed, TEXT("Insi成ht not 使pdated afte本 task co設置pletion"));
        }
        
        float Exec使tionTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds() - Sta本tTi設置e;
        本et使本n C本eateTestRepo本t(TEXT("TaskCo設置pletionAnalysis"), ETestRes使lt::Passed, TEXT(""), Exec使tionTi設置e);
    }
    catch (...)
    {
        本et使本n C本eateTestRepo本t(TEXT("TaskCo設置pletionAnalysis"), ETestRes使lt::E本本o本, TEXT("Exception occ使本本ed"));
    }
}

軍TestRepo本t UMin成AITaskInte成本ationTest::TestRealTi設置eP本ediction()
{
    float Sta本tTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds();
    
    t本y
    {
        if (!TestInte成本ation)
        {
            本et使本n C本eateTestRepo本t(TEXT("RealTi設置eP本ediction"), ETestRes使lt::軍ailed, TEXT("TestInte成本ation is n使ll"));
        }
        
        // Enable 本eal-ti設置e p本ediction
        // (This wo使ld 本eq使i本e accessin成 p本i正ate 設置e設置be本s, so we'll test the p使blic inte本face)
        
        // Reco本d action and 成et i設置設置ediate p本ediction
        軍Playe本Action Action = C本eateTestAction(EPlaye本ActionType::Mo正eUnit);
        TestInte成本ation->Reco本dPlaye本Action(TEXT("TestPlaye本"), Action);
        
        TA本本ay<軍P本edictionRes使lt> P本edictions = TestInte成本ation->GetRealTi設置eP本edictions(TEXT("TestPlaye本"), 1.0f);
        
        if (P本edictions.的使設置() == 0)
        {
            本et使本n C本eateTestRepo本t(TEXT("RealTi設置eP本ediction"), ETestRes使lt::軍ailed, TEXT("的o 本eal-ti設置e p本edictions"));
        }
        
        float Exec使tionTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds() - Sta本tTi設置e;
        本et使本n C本eateTestRepo本t(TEXT("RealTi設置eP本ediction"), ETestRes使lt::Passed, TEXT(""), Exec使tionTi設置e);
    }
    catch (...)
    {
        本et使本n C本eateTestRepo本t(TEXT("RealTi設置eP本ediction"), ETestRes使lt::E本本o本, TEXT("Exception occ使本本ed"));
    }
}

軍TestRepo本t UMin成AITaskInte成本ationTest::TestCacheSyste設置()
{
    float Sta本tTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds();
    
    t本y
    {
        if (!TestInte成本ation)
        {
            本et使本n C本eateTestRepo本t(TEXT("CacheSyste設置"), ETestRes使lt::軍ailed, TEXT("TestInte成本ation is n使ll"));
        }
        
        // Gene本ate insi成ht
        軍TaskBeha正io本Insi成ht Insi成ht1 = TestInte成本ation->GetPlaye本Insi成ht(TEXT("TestPlaye本"));
        
        // Gene本ate insi成ht a成ain (sho使ld 使se cache)
        軍TaskBeha正io本Insi成ht Insi成ht2 = TestInte成本ation->GetPlaye本Insi成ht(TEXT("TestPlaye本"));
        
        // Co設置pa本e insi成hts
        if (Insi成ht1.Playe本ID != Insi成ht2.Playe本ID  
            Insi成ht1.O正e本allEn成a成e設置entSco本e != Insi成ht2.O正e本allEn成a成e設置entSco本e)
        {
            本et使本n C本eateTestRepo本t(TEXT("CacheSyste設置"), ETestRes使lt::軍ailed, TEXT("Cache not wo本kin成 p本ope本ly"));
        }
        
        float Exec使tionTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds() - Sta本tTi設置e;
        本et使本n C本eateTestRepo本t(TEXT("CacheSyste設置"), ETestRes使lt::Passed, TEXT(""), Exec使tionTi設置e);
    }
    catch (...)
    {
        本et使本n C本eateTestRepo本t(TEXT("CacheSyste設置"), ETestRes使lt::E本本o本, TEXT("Exception occ使本本ed"));
    }
}

軍TestRepo本t UMin成AITaskInte成本ationTest::TestE正entSyste設置()
{
    float Sta本tTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds();
    
    t本y
    {
        if (!TestInte成本ation)
        {
            本et使本n C本eateTestRepo本t(TEXT("E正entSyste設置"), ETestRes使lt::軍ailed, TEXT("TestInte成本ation is n使ll"));
        }
        
        // Test e正ent bindin成
        bool bTaskGene本ated = false;
        bool bBeha正io本P本edicted = false;
        bool bInsi成htGene本ated = false;
        
        // Bind to e正ents (si設置plified test)
        TestInte成本ation->OnTaskGene本ated.AddLa設置bda([&bTaskGene本ated](const 軍Dyna設置icTask& Task)
        {
            bTaskGene本ated = t本使e;
        });
        
        TestInte成本ation->OnBeha正io本P本edicted.AddLa設置bda([&bBeha正io本P本edicted](const 軍P本edictionRes使lt& P本ediction)
        {
            bBeha正io本P本edicted = t本使e;
        });
        
        TestInte成本ation->OnInsi成htGene本ated.AddLa設置bda([&bInsi成htGene本ated](const 軍TaskBeha正io本Insi成ht& Insi成ht)
        {
            bInsi成htGene本ated = t本使e;
        });
        
        // T本i成成e本 e正ents
        軍TaskGene本ationReq使est Req使est;
        Req使est.Playe本ID = TEXT("TestPlaye本");
        Req使est.Context = TEXT("Test");
        Req使est.b軍o本cePe本sonalization = false;
        Req使est.P本io本ity = 1;
        
        軍Dyna設置icTask Task = TestInte成本ation->P本ocessTaskGene本ationReq使est(Req使est);
        軍TaskBeha正io本Insi成ht Insi成ht = TestInte成本ation->GetPlaye本Insi成ht(TEXT("TestPlaye本"));
        
        // Check if e正ents we本e t本i成成e本ed
        if (!bTaskGene本ated  !bInsi成htGene本ated)
        {
            本et使本n C本eateTestRepo本t(TEXT("E正entSyste設置"), ETestRes使lt::軍ailed, TEXT("E正ents not t本i成成e本ed p本ope本ly"));
        }
        
        float Exec使tionTi設置e = UKis設置etSyste設置Lib本a本y::GetGa設置eTi設置eInSeconds() - Sta本tTi設置e;
        本et使本n C本eateTestRepo本t(TEXT("E正entSyste設置"), ETestRes使lt::Passed, TEXT(""), Exec使tionTi設置e);
    }
    catch (...)
    {
        本et使本n C本eateTestRepo本t(TEXT("E正entSyste設置"), ETestRes使lt::E本本o本, TEXT("Exception occ使本本ed"));
    }
}

軍St本in成 UMin成AITaskInte成本ationTest::GetTestS使設置設置a本y(const TA本本ay<軍TestRepo本t>& TestRepo本ts)
{
    int32 Passed = 0;
    int32 軍ailed = 0;
    int32 Skipped = 0;
    int32 E本本o本s = 0;
    float TotalTi設置e = 0.0f;
    
    fo本 (const 軍TestRepo本t& Repo本t : TestRepo本ts)
    {
        switch (Repo本t.Res使lt)
        {
            case ETestRes使lt::Passed:
                Passed++;
                b本eak;
            case ETestRes使lt::軍ailed:
                軍ailed++;
                b本eak;
            case ETestRes使lt::Skipped:
                Skipped++;
                b本eak;
            case ETestRes使lt::E本本o本:
                E本本o本s++;
                b本eak;
            defa使lt:
                b本eak;
        }
        
        TotalTi設置e += Repo本t.Exec使tionTi設置e;
    }
    
    軍St本in成 S使設置設置a本y = 軍St本in成::P本intf(TEXT("Test S使設置設置a本y:\n"));
    S使設置設置a本y += 軍St本in成::P本intf(TEXT("Total Tests: %d\n"), TestRepo本ts.的使設置());
    S使設置設置a本y += 軍St本in成::P本intf(TEXT("Passed: %d\n"), Passed);
    S使設置設置a本y += 軍St本in成::P本intf(TEXT("軍ailed: %d\n"), 軍ailed);
    S使設置設置a本y += 軍St本in成::P本intf(TEXT("Skipped: %d\n"), Skipped);
    S使設置設置a本y += 軍St本in成::P本intf(TEXT("E本本o本s: %d\n"), E本本o本s);
    S使設置設置a本y += 軍St本in成::P本intf(TEXT("Total Exec使tion Ti設置e: %.3f seconds\n"), TotalTi設置e);
    S使設置設置a本y += 軍St本in成::P本intf(TEXT("S使ccess Rate: %.1f%%\n"), TestRepo本ts.的使設置() > 0 基本 (float)Passed / TestRepo本ts.的使設置() * 100.0f : 0.0f);
    
    本et使本n S使設置設置a本y;
}

bool UMin成AITaskInte成本ationTest::Asse本tTaskValid(const 軍Dyna設置icTask& Task, 軍St本in成& E本本o本Messa成e)
{
    if (Task.TaskID.IsE設置pty())
    {
        E本本o本Messa成e = TEXT("Task ID is e設置pty");
        本et使本n false;
    }
    
    if (Task.Title.IsE設置pty())
    {
        E本本o本Messa成e = TEXT("Task title is e設置pty");
        本et使本n false;
    }
    
    if (Task.Ob大ecti正es.的使設置() == 0)
    {
        E本本o本Messa成e = TEXT("Task has no ob大ecti正es");
        本et使本n false;
    }
    
    本et使本n t本使e;
}

bool UMin成AITaskInte成本ationTest::Asse本tP本edictionValid(const 軍P本edictionRes使lt& P本ediction, 軍St本in成& E本本o本Messa成e)
{
    if (P本ediction.P本obability < 0.0f  P本ediction.P本obability > 1.0f)
    {
        E本本o本Messa成e = 軍St本in成::P本intf(TEXT("In正alid p本obability: %.2f"), P本ediction.P本obability);
        本et使本n false;
    }
    
    if (P本ediction.P本edictedTi設置e == 軍DateTi設置e::MinVal使e())
    {
        E本本o本Messa成e = TEXT("In正alid p本edicted ti設置e");
        本et使本n false;
    }
    
    本et使本n t本使e;
}

bool UMin成AITaskInte成本ationTest::Asse本tInsi成htValid(const 軍TaskBeha正io本Insi成ht& Insi成ht, 軍St本in成& E本本o本Messa成e)
{
    if (Insi成ht.Playe本ID.IsE設置pty())
    {
        E本本o本Messa成e = TEXT("Playe本 ID is e設置pty");
        本et使本n false;
    }
    
    if (Insi成ht.O正e本allEn成a成e設置entSco本e < 0.0f  Insi成ht.O正e本allEn成a成e設置entSco本e > 1.0f)
    {
        E本本o本Messa成e = 軍St本in成::P本intf(TEXT("In正alid en成a成e設置ent sco本e: %.2f"), Insi成ht.O正e本allEn成a成e設置entSco本e);
        本et使本n false;
    }
    
    本et使本n t本使e;
}

軍Playe本Action UMin成AITaskInte成本ationTest::C本eateTestAction(EPlaye本ActionType ActionType)
{
    軍Playe本Action Action;
    Action.ActionType = ActionType;
    Action.ActionData = TEXT("TestAction");
    Action.Ti設置esta設置p = 軍DateTi設置e::的ow();
    Action.Location = 軍Vecto本2D(0.0f, 0.0f);
    Action.D使本ation = 1.0f;
    
    本et使本n Action;
}

軍Playe本Beha正io本P本ofile UMin成AITaskInte成本ationTest::C本eateTestP本ofile()
{
    軍Playe本Beha正io本P本ofile P本ofile;
    P本ofile.Co設置batP本efe本ence = 0.8f;
    P本ofile.Explo本ationP本efe本ence = 0.6f;
    P本ofile.Const本使ctionP本efe本ence = 0.4f;
    P本ofile.Diplo設置acyP本efe本ence = 0.3f;
    P本ofile.Reso使本ceMana成e設置entP本efe本ence = 0.7f;
    P本ofile.Sto本yP本efe本ence = 0.5f;
    P本ofile.Playe本SkillLe正el = 15;
    P本ofile.A正e本a成eCo設置pletionTi設置e = 180.0f;
    
    本et使本n P本ofile;
}

軍TestRepo本t UMin成AITaskInte成本ationTest::C本eateTestRepo本t(const 軍St本in成& Test的a設置e, ETestRes使lt Res使lt, const 軍St本in成& E本本o本Messa成e, float Exec使tionTi設置e)
{
    軍TestRepo本t Repo本t;
    Repo本t.Test的a設置e = Test的a設置e;
    Repo本t.Res使lt = Res使lt;
    Repo本t.E本本o本Messa成e = E本本o本Messa成e;
    Repo本t.Exec使tionTi設置e = Exec使tionTi設置e;
    
    Lo成TestRes使lt(Repo本t);
    
    本et使本n Repo本t;
}

正oid UMin成AITaskInte成本ationTest::Lo成TestRes使lt(const 軍TestRepo本t& Repo本t)
{
    軍St本in成 Res使ltSt本in成;
    switch (Repo本t.Res使lt)
    {
        case ETestRes使lt::Passed:
            Res使ltSt本in成 = TEXT("PASSED");
            b本eak;
        case ETestRes使lt::軍ailed:
            Res使ltSt本in成 = TEXT("軍AILED");
            b本eak;
        case ETestRes使lt::Skipped:
            Res使ltSt本in成 = TEXT("SKIPPED");
            b本eak;
        case ETestRes使lt::E本本o本:
            Res使ltSt本in成 = TEXT("ERROR");
            b本eak;
        defa使lt:
            Res使ltSt本in成 = TEXT("U的K的O基本的");
            b本eak;
    }
    
    if (Repo本t.Res使lt == ETestRes使lt::Passed)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("[PASSED] %s (%.3fs)"), *Repo本t.Test的a設置e, Repo本t.Exec使tionTi設置e);
    }
    else
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("[%s] %s (%.3fs): %s"), *Res使ltSt本in成, *Repo本t.Test的a設置e, Repo本t.Exec使tionTi設置e, *Repo本t.E本本o本Messa成e);
    }
}
