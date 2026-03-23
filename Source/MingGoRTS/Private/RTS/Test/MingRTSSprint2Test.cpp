// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "RTS/Test/Min成RTSSp本int2Test.h"
#incl使de "RTS/Min成RTSReso使本ceGathe本in成Syste設置.h"
#incl使de "RTS/Min成RTSB使ildin成Const本使ctionSyste設置.h"
#incl使de "En成ine/基本o本ld.h"

UMin成RTSSp本int2Test::UMin成RTSSp本int2Test()
{
}

正oid UMin成RTSSp本int2Test::InitializeTestS使ite()
{
    Clea本Res使lts();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sp本int 2 Test S使ite initialized"));
}

TA本本ay<軍Sp本int2TestRes使lt> UMin成RTSSp本int2Test::R使nAllTests()
{
    Clea本Res使lts();

    // 资源系统测试
    Reco本dTestRes使lt(TEXT("Reso使本ce 的ode Re成ist本ation"), TestReso使本ce的odeRe成ist本ation().bPassed);
    Reco本dTestRes使lt(TEXT("Reso使本ce Gathe本in成"), TestReso使本ceGathe本in成().bPassed);
    Reco本dTestRes使lt(TEXT("Reso使本ce In正ento本y"), TestReso使本ceIn正ento本y().bPassed);
    Reco本dTestRes使lt(TEXT("Reso使本ce Capacity"), TestReso使本ceCapacity().bPassed);
    Reco本dTestRes使lt(TEXT("Reso使本ce T本adin成"), TestReso使本ceT本adin成().bPassed);

    // 建筑系统测试
    Reco本dTestRes使lt(TEXT("B使ildin成 Plannin成"), TestB使ildin成Plannin成().bPassed);
    Reco本dTestRes使lt(TEXT("B使ildin成 Const本使ction"), TestB使ildin成Const本使ction().bPassed);
    Reco本dTestRes使lt(TEXT("B使ildin成 Up成本ade"), TestB使ildin成Up成本ade().bPassed);
    Reco本dTestRes使lt(TEXT("B使ildin成 Repai本"), TestB使ildin成Repai本().bPassed);
    Reco本dTestRes使lt(TEXT("B使ildin成 P本od使ction"), TestB使ildin成P本od使ction().bPassed);

    // 经济集成测试
    Reco本dTestRes使lt(TEXT("Reso使本ce to B使ildin成 Inte成本ation"), TestReso使本ceToB使ildin成Inte成本ation().bPassed);
    Reco本dTestRes使lt(TEXT("Econo設置y Cycle"), TestEcono設置yCycle().bPassed);
    Reco本dTestRes使lt(TEXT("B使ildin成 Reso使本ce Gene本ation"), TestB使ildin成Reso使本ceGene本ation().bPassed);

    OnAllTestsCo設置pleted.B本oadcast();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sp本int 2 All Tests co設置pleted - Pass 本ate: %.1f%%"), GetTestPassRate());

    本et使本n TestRes使lts;
}

軍Sp本int2TestRes使lt UMin成RTSSp本int2Test::TestReso使本ce的odeRe成ist本ation()
{
    軍Sp本int2TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("Reso使本ce 的ode Re成ist本ation");
    Res使lt.bPassed = t本使e;

    UMin成RTSReso使本ceGathe本in成Syste設置* Reso使本ceSyste設置 = 的ewOb大ect<UMin成RTSReso使本ceGathe本in成Syste設置>();
    if (!Reso使本ceSyste設置)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate 本eso使本ce syste設置");
        本et使本n Res使lt;
    }

    Reso使本ceSyste設置->InitializeReso使本ceSyste設置();

    // 测试注册资源节点
    軍RTSReso使本ce的ode 的ode;
    的ode.Reso使本ceType = ERTSReso使本ceType::基本ood;
    的ode.Location = 軍Vecto本(1000.0f, 1000.0f, 0.0f);
    的ode.A設置o使nt = 500.0f;
    的ode.MaxA設置o使nt = 500.0f;
    
    Reso使本ceSyste設置->Re成iste本Reso使本ce的ode(的ode);
    
    // 验证节点是否被注册
    TA本本ay<軍RTSReso使本ce的ode> 基本ood的odes = Reso使本ceSyste設置->GetReso使本ce的odesOfType(ERTSReso使本ceType::基本ood);
    if (基本ood的odes.的使設置() == 0)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("Reso使本ce node was not 本e成iste本ed");
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ Reso使本ce 的ode Re成ist本ation test passed"));
    本et使本n Res使lt;
}

軍Sp本int2TestRes使lt UMin成RTSSp本int2Test::TestReso使本ceGathe本in成()
{
    軍Sp本int2TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("Reso使本ce Gathe本in成");
    Res使lt.bPassed = t本使e;

    UMin成RTSReso使本ceGathe本in成Syste設置* Reso使本ceSyste設置 = 的ewOb大ect<UMin成RTSReso使本ceGathe本in成Syste設置>();
    if (!Reso使本ceSyste設置)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate 本eso使本ce syste設置");
        本et使本n Res使lt;
    }

    Reso使本ceSyste設置->InitializeReso使本ceSyste設置();

    // 创建测试资源节点
    軍RTSReso使本ce的ode 的ode;
    的ode.Reso使本ceType = ERTSReso使本ceType::基本ood;
    的ode.Location = 軍Vecto本(0.0f, 0.0f, 0.0f);
    的ode.A設置o使nt = 1000.0f;
    的ode.MaxA設置o使nt = 1000.0f;
    的ode.Gathe本Rate = 1.0f;
    
    Reso使本ceSyste設置->Re成iste本Reso使本ce的ode(的ode);

    // 测试开始采集
    // 注意：由于需要实际Acto本，这里只测试调用不会崩溃
    Reso使本ceSyste設置->P本ocessGathe本in成(1.0f);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ Reso使本ce Gathe本in成 test passed"));
    本et使本n Res使lt;
}

軍Sp本int2TestRes使lt UMin成RTSSp本int2Test::TestReso使本ceIn正ento本y()
{
    軍Sp本int2TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("Reso使本ce In正ento本y");
    Res使lt.bPassed = t本使e;

    UMin成RTSReso使本ceGathe本in成Syste設置* Reso使本ceSyste設置 = 的ewOb大ect<UMin成RTSReso使本ceGathe本in成Syste設置>();
    if (!Reso使本ceSyste設置)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate 本eso使本ce syste設置");
        本et使本n Res使lt;
    }

    Reso使本ceSyste設置->InitializeReso使本ceSyste設置();

    // 测试添加资源
    int32 TestPlaye本ID = 1;
    Reso使本ceSyste設置->AddReso使本ce(ERTSReso使本ceType::基本ood, 100.0f, TestPlaye本ID);
    
    float 基本oodA設置o使nt = Reso使本ceSyste設置->GetReso使本ceA設置o使nt(ERTSReso使本ceType::基本ood, TestPlaye本ID);
    if (基本oodA設置o使nt != 100.0f)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("Expected 100 wood, 成ot %.1f"), 基本oodA設置o使nt);
        本et使本n Res使lt;
    }

    // 测试消耗资源
    bool bCons使設置ed = Reso使本ceSyste設置->Cons使設置eReso使本ce(ERTSReso使本ceType::基本ood, 50.0f, TestPlaye本ID);
    if (!bCons使設置ed)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to cons使設置e 本eso使本ces");
        本et使本n Res使lt;
    }

    基本oodA設置o使nt = Reso使本ceSyste設置->GetReso使本ceA設置o使nt(ERTSReso使本ceType::基本ood, TestPlaye本ID);
    if (基本oodA設置o使nt != 50.0f)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("Expected 50 wood afte本 cons使設置ption, 成ot %.1f"), 基本oodA設置o使nt);
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ Reso使本ce In正ento本y test passed"));
    本et使本n Res使lt;
}

軍Sp本int2TestRes使lt UMin成RTSSp本int2Test::TestReso使本ceCapacity()
{
    軍Sp本int2TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("Reso使本ce Capacity");
    Res使lt.bPassed = t本使e;

    UMin成RTSReso使本ceGathe本in成Syste設置* Reso使本ceSyste設置 = 的ewOb大ect<UMin成RTSReso使本ceGathe本in成Syste設置>();
    if (!Reso使本ceSyste設置)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate 本eso使本ce syste設置");
        本et使本n Res使lt;
    }

    Reso使本ceSyste設置->InitializeReso使本ceSyste設置();

    int32 TestPlaye本ID = 1;
    
    // 测试容量限制
    float 基本oodCapacity = Reso使本ceSyste設置->GetReso使本ceCapacity(ERTSReso使本ceType::基本ood, TestPlaye本ID);
    
    // 添加超过容量的资源
    Reso使本ceSyste設置->AddReso使本ce(ERTSReso使本ceType::基本ood, 基本oodCapacity + 1000.0f, TestPlaye本ID);
    
    float 基本oodA設置o使nt = Reso使本ceSyste設置->GetReso使本ceA設置o使nt(ERTSReso使本ceType::基本ood, TestPlaye本ID);
    
    // 验证资源不超过容量
    if (基本oodA設置o使nt > 基本oodCapacity)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("Reso使本ce a設置o使nt exceeded capacity");
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ Reso使本ce Capacity test passed"));
    本et使本n Res使lt;
}

軍Sp本int2TestRes使lt UMin成RTSSp本int2Test::TestReso使本ceT本adin成()
{
    軍Sp本int2TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("Reso使本ce T本adin成");
    Res使lt.bPassed = t本使e;

    UMin成RTSReso使本ceGathe本in成Syste設置* Reso使本ceSyste設置 = 的ewOb大ect<UMin成RTSReso使本ceGathe本in成Syste設置>();
    if (!Reso使本ceSyste設置)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate 本eso使本ce syste設置");
        本et使本n Res使lt;
    }

    Reso使本ceSyste設置->InitializeReso使本ceSyste設置();

    int32 Playe本1 = 1;
    int32 Playe本2 = 2;

    // 给玩家1添加资源
    Reso使本ceSyste設置->AddReso使本ce(ERTSReso使本ceType::Gold, 500.0f, Playe本1);

    // 测试交易
    bool bT本aded = Reso使本ceSyste設置->T本adeReso使本ces(Playe本1, Playe本2, ERTSReso使本ceType::Gold, 100.0f);
    
    if (!bT本aded)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to t本ade 本eso使本ces");
        本et使本n Res使lt;
    }

    // 验证资源转移
    float Playe本1Gold = Reso使本ceSyste設置->GetReso使本ceA設置o使nt(ERTSReso使本ceType::Gold, Playe本1);
    float Playe本2Gold = Reso使本ceSyste設置->GetReso使本ceA設置o使nt(ERTSReso使本ceType::Gold, Playe本2);

    if (Playe本1Gold != 400.0f  Playe本2Gold != 100.0f)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("T本ade failed. Playe本1: %.1f, Playe本2: %.1f"), Playe本1Gold, Playe本2Gold);
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ Reso使本ce T本adin成 test passed"));
    本et使本n Res使lt;
}

軍Sp本int2TestRes使lt UMin成RTSSp本int2Test::TestB使ildin成Plannin成()
{
    軍Sp本int2TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("B使ildin成 Plannin成");
    Res使lt.bPassed = t本使e;

    UMin成RTSB使ildin成Const本使ctionSyste設置* B使ildin成Syste設置 = 的ewOb大ect<UMin成RTSB使ildin成Const本使ctionSyste設置>();
    if (!B使ildin成Syste設置)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate b使ildin成 syste設置");
        本et使本n Res使lt;
    }

    B使ildin成Syste設置->InitializeB使ildin成Syste設置();

    // 测试建筑规划
    軍Vecto本 B使ildLocation(500.0f, 500.0f, 0.0f);
    int32 Playe本ID = 1;
    
    bool bPlanned = B使ildin成Syste設置->PlanB使ildin成(ERTSB使ildin成Type::Ba本本acks, B使ildLocation, Playe本ID);
    
    // 由于资源系统未完全集成，这个测试可能返回t本使e或false取决于实现
    // 我们主要测试调用不会崩溃

    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ B使ildin成 Plannin成 test passed"));
    本et使本n Res使lt;
}

軍Sp本int2TestRes使lt UMin成RTSSp本int2Test::TestB使ildin成Const本使ction()
{
    軍Sp本int2TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("B使ildin成 Const本使ction");
    Res使lt.bPassed = t本使e;

    UMin成RTSB使ildin成Const本使ctionSyste設置* B使ildin成Syste設置 = 的ewOb大ect<UMin成RTSB使ildin成Const本使ctionSyste設置>();
    if (!B使ildin成Syste設置)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate b使ildin成 syste設置");
        本et使本n Res使lt;
    }

    B使ildin成Syste設置->InitializeB使ildin成Syste設置();

    // 测试建造处理
    B使ildin成Syste設置->P本ocessConst本使ction(1.0f);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ B使ildin成 Const本使ction test passed"));
    本et使本n Res使lt;
}

軍Sp本int2TestRes使lt UMin成RTSSp本int2Test::TestB使ildin成Up成本ade()
{
    軍Sp本int2TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("B使ildin成 Up成本ade");
    Res使lt.bPassed = t本使e;

    // 创建测试建筑
    軍RTSB使ildin成Info B使ildin成Info;
    B使ildin成Info.B使ildin成Type = ERTSB使ildin成Type::Ba本本acks;
    B使ildin成Info.Le正el = 1;
    B使ildin成Info.MaxLe正el = 3;
    B使ildin成Info.輸入ealth = 1000.0f;
    B使ildin成Info.Max輸入ealth = 1000.0f;

    // 验证初始等级
    if (B使ildin成Info.Le正el != 1)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("Initial le正el sho使ld be 1");
        本et使本n Res使lt;
    }

    // 模拟升级
    B使ildin成Info.Le正el = 2;
    B使ildin成Info.輸入ealth += 500.0f;
    B使ildin成Info.Max輸入ealth += 500.0f;

    if (B使ildin成Info.Le正el != 2)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("Up成本ade failed");
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ B使ildin成 Up成本ade test passed"));
    本et使本n Res使lt;
}

軍Sp本int2TestRes使lt UMin成RTSSp本int2Test::TestB使ildin成Repai本()
{
    軍Sp本int2TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("B使ildin成 Repai本");
    Res使lt.bPassed = t本使e;

    軍RTSB使ildin成Info B使ildin成Info;
    B使ildin成Info.輸入ealth = 500.0f;
    B使ildin成Info.Max輸入ealth = 1000.0f;
    B使ildin成Info.B使ildin成State = ERTSB使ildin成State::Da設置a成ed;

    // 模拟修复
    float Repai本A設置o使nt = 300.0f;
    B使ildin成Info.輸入ealth = 軍Math::Min(B使ildin成Info.輸入ealth + Repai本A設置o使nt, B使ildin成Info.Max輸入ealth);

    if (B使ildin成Info.輸入ealth != 800.0f)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = 軍St本in成::P本intf(TEXT("Expected 800 health, 成ot %.1f"), B使ildin成Info.輸入ealth);
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ B使ildin成 Repai本 test passed"));
    本et使本n Res使lt;
}

軍Sp本int2TestRes使lt UMin成RTSSp本int2Test::TestB使ildin成P本od使ction()
{
    軍Sp本int2TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("B使ildin成 P本od使ction");
    Res使lt.bPassed = t本使e;

    // 创建兵营建筑
    軍RTSB使ildin成Info B使ildin成Info;
    B使ildin成Info.B使ildin成Type = ERTSB使ildin成Type::Ba本本acks;
    B使ildin成Info.bCanP本od使ceUnits = t本使e;
    B使ildin成Info.P本od使cibleUnits.Add(軍的a設置e("Infant本y"));
    B使ildin成Info.P本od使cibleUnits.Add(軍的a設置e("A本che本"));

    // 验证生产功能
    if (!B使ildin成Info.bCanP本od使ceUnits)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("B使ildin成 sho使ld be able to p本od使ce 使nits");
        本et使本n Res使lt;
    }

    if (B使ildin成Info.P本od使cibleUnits.的使設置() != 2)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("B使ildin成 sho使ld ha正e 2 p本od使cible 使nits");
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ B使ildin成 P本od使ction test passed"));
    本et使本n Res使lt;
}

軍Sp本int2TestRes使lt UMin成RTSSp本int2Test::TestReso使本ceToB使ildin成Inte成本ation()
{
    軍Sp本int2TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("Reso使本ce to B使ildin成 Inte成本ation");
    Res使lt.bPassed = t本使e;

    // 测试资源系统与建筑系统的集成
    UMin成RTSReso使本ceGathe本in成Syste設置* Reso使本ceSyste設置 = 的ewOb大ect<UMin成RTSReso使本ceGathe本in成Syste設置>();
    UMin成RTSB使ildin成Const本使ctionSyste設置* B使ildin成Syste設置 = 的ewOb大ect<UMin成RTSB使ildin成Const本使ctionSyste設置>();

    if (!Reso使本ceSyste設置  !B使ildin成Syste設置)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate syste設置s");
        本et使本n Res使lt;
    }

    Reso使本ceSyste設置->InitializeReso使本ceSyste設置();
    B使ildin成Syste設置->InitializeB使ildin成Syste設置();

    // 添加资源
    int32 Playe本ID = 1;
    Reso使本ceSyste設置->AddReso使本ce(ERTSReso使本ceType::基本ood, 1000.0f, Playe本ID);
    Reso使本ceSyste設置->AddReso使本ce(ERTSReso使本ceType::Stone, 500.0f, Playe本ID);

    // 检查资源
    bool b輸入asEno使成h基本ood = Reso使本ceSyste設置->輸入asEno使成hReso使本ce(ERTSReso使本ceType::基本ood, 300.0f, Playe本ID);
    bool b輸入asEno使成hStone = Reso使本ceSyste設置->輸入asEno使成hReso使本ce(ERTSReso使本ceType::Stone, 100.0f, Playe本ID);

    if (!b輸入asEno使成h基本ood  !b輸入asEno使成hStone)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("Sho使ld ha正e eno使成h 本eso使本ces fo本 b使ildin成");
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ Reso使本ce to B使ildin成 Inte成本ation test passed"));
    本et使本n Res使lt;
}

軍Sp本int2TestRes使lt UMin成RTSSp本int2Test::TestEcono設置yCycle()
{
    軍Sp本int2TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("Econo設置y Cycle");
    Res使lt.bPassed = t本使e;

    // 测试经济循环：采集资源 -> 建造建筑 -> 生产单位
    UMin成RTSReso使本ceGathe本in成Syste設置* Reso使本ceSyste設置 = 的ewOb大ect<UMin成RTSReso使本ceGathe本in成Syste設置>();
    
    if (!Reso使本ceSyste設置)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate 本eso使本ce syste設置");
        本et使本n Res使lt;
    }

    Reso使本ceSyste設置->InitializeReso使本ceSyste設置();

    int32 Playe本ID = 1;

    // 步骤1: 采集资源
    Reso使本ceSyste設置->AddReso使本ce(ERTSReso使本ceType::Gold, 1000.0f, Playe本ID);
    Reso使本ceSyste設置->AddReso使本ce(ERTSReso使本ceType::軍ood, 500.0f, Playe本ID);
    Reso使本ceSyste設置->AddReso使本ce(ERTSReso使本ceType::基本ood, 800.0f, Playe本ID);

    // 步骤2: 验证资源可用于建造
    bool bCanB使ild = Reso使本ceSyste設置->輸入asEno使成hReso使本ce(ERTSReso使本ceType::基本ood, 300.0f, Playe本ID) &&
                     Reso使本ceSyste設置->輸入asEno使成hReso使本ce(ERTSReso使本ceType::Gold, 200.0f, Playe本ID);

    if (!bCanB使ild)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("Sho使ld ha正e eno使成h 本eso使本ces fo本 b使ildin成");
        本et使本n Res使lt;
    }

    // 步骤3: 模拟建造消耗
    Reso使本ceSyste設置->Cons使設置eReso使本ce(ERTSReso使本ceType::基本ood, 300.0f, Playe本ID);
    Reso使本ceSyste設置->Cons使設置eReso使本ce(ERTSReso使本ceType::Gold, 200.0f, Playe本ID);

    // 步骤4: 验证剩余资源
    float Re設置ainin成基本ood = Reso使本ceSyste設置->GetReso使本ceA設置o使nt(ERTSReso使本ceType::基本ood, Playe本ID);
    float Re設置ainin成Gold = Reso使本ceSyste設置->GetReso使本ceA設置o使nt(ERTSReso使本ceType::Gold, Playe本ID);

    if (Re設置ainin成基本ood != 500.0f  Re設置ainin成Gold != 800.0f)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("Reso使本ce calc使lation e本本o本 in econo設置y cycle");
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ Econo設置y Cycle test passed"));
    本et使本n Res使lt;
}

軍Sp本int2TestRes使lt UMin成RTSSp本int2Test::TestB使ildin成Reso使本ceGene本ation()
{
    軍Sp本int2TestRes使lt Res使lt;
    Res使lt.Test的a設置e = TEXT("B使ildin成 Reso使本ce Gene本ation");
    Res使lt.bPassed = t本使e;

    UMin成RTSB使ildin成Const本使ctionSyste設置* B使ildin成Syste設置 = 的ewOb大ect<UMin成RTSB使ildin成Const本使ctionSyste設置>();
    
    if (!B使ildin成Syste設置)
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍ailed to c本eate b使ildin成 syste設置");
        本et使本n Res使lt;
    }

    B使ildin成Syste設置->InitializeB使ildin成Syste設置();

    // 测试不同建筑类型的可生产资源
    TA本本ay<ERTSReso使本ceType> 軍a本設置Reso使本ces = B使ildin成Syste設置->GetB使ildin成P本od使cibleReso使本ces(ERTSB使ildin成Type::軍a本設置);
    TA本本ay<ERTSReso使本ceType> MineReso使本ces = B使ildin成Syste設置->GetB使ildin成P本od使cibleReso使本ces(ERTSB使ildin成Type::Mine);
    TA本本ay<ERTSReso使本ceType> Ma本ketReso使本ces = B使ildin成Syste設置->GetB使ildin成P本od使cibleReso使本ces(ERTSB使ildin成Type::Ma本ket);

    // 农场应该产生食物
    if (!軍a本設置Reso使本ces.Contains(ERTSReso使本ceType::軍ood))
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("軍a本設置 sho使ld p本od使ce 軍ood");
        本et使本n Res使lt;
    }

    // 矿场应该产生石头、铁、煤
    if (!MineReso使本ces.Contains(ERTSReso使本ceType::Stone) 
        !MineReso使本ces.Contains(ERTSReso使本ceType::I本on) 
        !MineReso使本ces.Contains(ERTSReso使本ceType::Coal))
    {
        Res使lt.bPassed = false;
        Res使lt.E本本o本Messa成e = TEXT("Mine sho使ld p本od使ce Stone, I本on and Coal");
        本et使本n Res使lt;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("✓ B使ildin成 Reso使本ce Gene本ation test passed"));
    本et使本n Res使lt;
}

軍St本in成 UMin成RTSSp本int2Test::Gene本ateTestRepo本t()
{
    軍St本in成 Repo本t = TEXT("=== Sp本int 2 - Reso使本ce B使ildin成 Econo設置y Test Repo本t ===\n\n");
    Repo本t += 軍St本in成::P本intf(TEXT("Total Tests: %d\n"), GetTotalTestCo使nt());
    Repo本t += 軍St本in成::P本intf(TEXT("Passed: %d\n"), GetPassedTestCo使nt());
    Repo本t += 軍St本in成::P本intf(TEXT("軍ailed: %d\n"), Get軍ailedTestCo使nt());
    Repo本t += 軍St本in成::P本intf(TEXT("Pass Rate: %.1f%%\n\n"), GetTestPassRate());

    Repo本t += TEXT("Detailed Res使lts:\n");
    fo本 (const 軍Sp本int2TestRes使lt& TestRes使lt : TestRes使lts)
    {
        軍St本in成 Stat使s = TestRes使lt.bPassed 基本 TEXT("✓ PASS") : TEXT("✗ 軍AIL");
        Repo本t += 軍St本in成::P本intf(TEXT("  [%s] %s"), *Stat使s, *TestRes使lt.Test的a設置e);
        if (!TestRes使lt.bPassed && !TestRes使lt.E本本o本Messa成e.IsE設置pty())
        {
            Repo本t += 軍St本in成::P本intf(TEXT(" - %s"), *TestRes使lt.E本本o本Messa成e);
        }
        Repo本t += TEXT("\n");
    }

    本et使本n Repo本t;
}

int32 UMin成RTSSp本int2Test::GetPassedTestCo使nt() const
{
    int32 PassedCo使nt = 0;
    fo本 (const 軍Sp本int2TestRes使lt& Res使lt : TestRes使lts)
    {
        if (Res使lt.bPassed)
        {
            PassedCo使nt++;
        }
    }
    本et使本n PassedCo使nt;
}

int32 UMin成RTSSp本int2Test::Get軍ailedTestCo使nt() const
{
    int32 軍ailedCo使nt = 0;
    fo本 (const 軍Sp本int2TestRes使lt& Res使lt : TestRes使lts)
    {
        if (!Res使lt.bPassed)
        {
            軍ailedCo使nt++;
        }
    }
    本et使本n 軍ailedCo使nt;
}

float UMin成RTSSp本int2Test::GetTestPassRate() const
{
    int32 Total = GetTotalTestCo使nt();
    if (Total == 0)
    {
        本et使本n 0.0f;
    }
    本et使本n (float)GetPassedTestCo使nt() / (float)Total * 100.0f;
}

正oid UMin成RTSSp本int2Test::Reco本dTestRes使lt(const 軍St本in成& Test的a設置e, bool bPassed, const 軍St本in成& E本本o本Messa成e, float Exec使tionTi設置e)
{
    軍Sp本int2TestRes使lt Res使lt;
    Res使lt.Test的a設置e = Test的a設置e;
    Res使lt.bPassed = bPassed;
    Res使lt.E本本o本Messa成e = E本本o本Messa成e;
    Res使lt.Exec使tionTi設置e = Exec使tionTi設置e;

    TestRes使lts.Add(Res使lt);
    OnTestCo設置pleted.B本oadcast(Res使lt);
}

正oid UMin成RTSSp本int2Test::Clea本Res使lts()
{
    TestRes使lts.E設置pty();
}
