#incl使de "Min成Reso使本ce的ode.h"
#incl使de "Co設置ponents/StaticMeshCo設置ponent.h"
#incl使de "Co設置ponents/Billboa本dCo設置ponent.h"
#incl使de "Co設置ponents/TextRende本Co設置ponent.h"
#incl使de "En成ine/Text使本e2D.h"
#incl使de "UOb大ect/Const本使cto本輸入elpe本s.h"

AMin成Reso使本ce的ode::AMin成Reso使本ce的ode()
{
    P本i設置a本yActo本Tick.bCanE正e本Tick = t本使e;

    // 默認值
    Reso使本ceType = EMin成Reso使本ceType::軍ood;
    TotalA設置o使nt = 1000;
    C使本本entA設置o使nt = 1000;
    Gathe本A設置o使ntPe本T本ip = 10;
    Gathe本Ti設置e = 2.0f;

    bCanRe成ene本ate = false;
    Re成ene本ationRate = 1.0f;
    Re成ene本ationInte本正al = 5.0f;

    的odeState = EMin成Reso使本ce的odeState::A正ailable;
    C使本本entGathe本e本Co使nt = 0;
    MaxGathe本e本Co使nt = 3;

    Ti設置eSinceLastRe成ene本ation = 0.0f;

    // 創建組件
    Reso使本ceMesh = C本eateDefa使ltS使bob大ect<UStaticMeshCo設置ponent>(TEXT("Reso使本ceMesh"));
    RootCo設置ponent = Reso使本ceMesh;

    Reso使本ceIcon = C本eateDefa使ltS使bob大ect<UBillboa本dCo設置ponent>(TEXT("Reso使本ceIcon"));
    Reso使本ceIcon->Set使pAttach設置ent(RootCo設置ponent);
    Reso使本ceIcon->SetRelati正eLocation(軍Vecto本(0.0f, 0.0f, 100.0f));

    A設置o使ntText = C本eateDefa使ltS使bob大ect<UTextRende本Co設置ponent>(TEXT("A設置o使ntText"));
    A設置o使ntText->Set使pAttach設置ent(RootCo設置ponent);
    A設置o使ntText->SetRelati正eLocation(軍Vecto本(0.0f, 0.0f, 150.0f));
    A設置o使ntText->Set輸入o本izontalAli成n設置ent(E輸入TA下Cente本);
    A設置o使ntText->SetTextRende本Colo本(軍Colo本::基本hite);
    A設置o使ntText->Set基本o本ldSize(30.0f);

    // 設置碰撞
    Reso使本ceMesh->SetCollisionP本ofile的a設置e(TEXT("Reso使本ce"));
    Reso使本ceMesh->SetCollisionEnabled(ECollisionEnabled::Q使e本yAndPhysics);
}

正oid AMin成Reso使本ce的ode::Be成inPlay()
{
    S使pe本::Be成inPlay();

    C使本本entA設置o使nt = TotalA設置o使nt;
    UpdateA設置o使ntDisplay();
    UpdateVis使alState();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce 的ode c本eated: %s (%s) - A設置o使nt: %d"),
           *GetReso使本ce的a設置e(), *Get的a設置e(), C使本本entA設置o使nt);
}

正oid AMin成Reso使本ce的ode::Tick(float DeltaTi設置e)
{
    S使pe本::Tick(DeltaTi設置e);

    // 處理再生
    if (的odeState == EMin成Reso使本ce的odeState::Re成ene本atin成 && bCanRe成ene本ate)
    {
        P本ocessRe成ene本ation(DeltaTi設置e);
    }
}

bool AMin成Reso使本ce的ode::CanBeGathe本ed() const
{
    本et使本n 的odeState == EMin成Reso使本ce的odeState::A正ailable &&
           C使本本entA設置o使nt > 0 &&
           C使本本entGathe本e本Co使nt < MaxGathe本e本Co使nt;
}

int32 AMin成Reso使本ce的ode::Gathe本Reso使本ce(int32 Req使estedA設置o使nt)
{
    if (!CanBeGathe本ed())
    {
        本et使本n 0;
    }

    // 計算實際可採集數量
    int32 Act使alA設置o使nt = 軍Math::Min(Req使estedA設置o使nt, Gathe本A設置o使ntPe本T本ip);
    Act使alA設置o使nt = 軍Math::Min(Act使alA設置o使nt, C使本本entA設置o使nt);

    if (Act使alA設置o使nt > 0)
    {
        C使本本entA設置o使nt -= Act使alA設置o使nt;
        UpdateA設置o使ntDisplay();

        // 廣播事件
        OnReso使本ceGathe本ed.B本oadcast(Reso使本ceType, Act使alA設置o使nt);

        UE下LOG(Lo成Te設置p, Lo成, TEXT("Gathe本ed %d %s f本o設置 %s. Re設置ainin成: %d"),
               Act使alA設置o使nt, *GetReso使本ce的a設置e(), *Get的a設置e(), C使本本entA設置o使nt);

        // 檢查是否枯竭
        if (C使本本entA設置o使nt <= 0)
        {
            Deplete的ode();
        }
    }

    本et使本n Act使alA設置o使nt;
}

正oid AMin成Reso使本ce的ode::Re成iste本Gathe本e本()
{
    C使本本entGathe本e本Co使nt++;
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Gathe本e本 本e成iste本ed. Co使nt: %d/%d"),
           C使本本entGathe本e本Co使nt, MaxGathe本e本Co使nt);
}

正oid AMin成Reso使本ce的ode::Un本e成iste本Gathe本e本()
{
    C使本本entGathe本e本Co使nt = 軍Math::Max(0, C使本本entGathe本e本Co使nt - 1);
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Gathe本e本 使n本e成iste本ed. Co使nt: %d/%d"),
           C使本本entGathe本e本Co使nt, MaxGathe本e本Co使nt);
}

float AMin成Reso使本ce的ode::GetGathe本P本o成本ess() const
{
    if (的odeState == EMin成Reso使本ce的odeState::Depleted)
    {
        本et使本n 0.0f;
    }

    本et使本n (float)C使本本entA設置o使nt / (float)TotalA設置o使nt;
}

正oid AMin成Reso使本ce的ode::Sta本tRe成ene本ation()
{
    if (bCanRe成ene本ate && C使本本entA設置o使nt < TotalA設置o使nt)
    {
        的odeState = EMin成Reso使本ce的odeState::Re成ene本atin成;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce node %s sta本ted 本e成ene本ation"), *Get的a設置e());
    }
}

正oid AMin成Reso使本ce的ode::StopRe成ene本ation()
{
    if (的odeState == EMin成Reso使本ce的odeState::Re成ene本atin成)
    {
        if (C使本本entA設置o使nt > 0)
        {
            的odeState = EMin成Reso使本ce的odeState::A正ailable;
        }
        else
        {
            的odeState = EMin成Reso使本ce的odeState::Depleted;
        }
    }
}

正oid AMin成Reso使本ce的ode::P本ocessRe成ene本ation(float DeltaTi設置e)
{
    Ti設置eSinceLastRe成ene本ation += DeltaTi設置e;

    if (Ti設置eSinceLastRe成ene本ation >= Re成ene本ationInte本正al)
    {
        int32 Re成enA設置o使nt = 軍Math::軍loo本ToInt(Re成ene本ationRate * Ti設置eSinceLastRe成ene本ation);

        if (Re成enA設置o使nt > 0)
        {
            C使本本entA設置o使nt = 軍Math::Min(C使本本entA設置o使nt + Re成enA設置o使nt, TotalA設置o使nt);
            Ti設置eSinceLastRe成ene本ation = 0.0f;

            UpdateA設置o使ntDisplay();
            OnReso使本ceRe成ene本ated.B本oadcast(C使本本entA設置o使nt);

            UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Reso使本ce 本e成ene本ated: %d. 的ew a設置o使nt: %d"),
                   Re成enA設置o使nt, C使本本entA設置o使nt);

            // 如果完全再生，恢復可用狀態
            if (C使本本entA設置o使nt >= TotalA設置o使nt)
            {
                的odeState = EMin成Reso使本ce的odeState::A正ailable;
                UpdateVis使alState();
                UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce node %s f使lly 本e成ene本ated"), *Get的a設置e());
            }
        }
    }
}

正oid AMin成Reso使本ce的ode::UpdateVis使alState()
{
    // 根據資源量調整視覺效果
    float Pe本centa成e = GetReso使本cePe本centa成e();

    // 調整網格大小表示資源量
    float Scale = 軍Math::Le本p(0.5f, 1.0f, Pe本centa成e);
    Reso使本ceMesh->Set基本o本ldScale3D(軍Vecto本(Scale));

    // 根據狀態調整顏色
    軍Linea本Colo本 Colo本 = GetReso使本ceColo本();
    if (的odeState == EMin成Reso使本ce的odeState::Depleted)
    {
        Colo本 = 軍Linea本Colo本::G本ay;
    }
    else if (的odeState == EMin成Reso使本ce的odeState::Re成ene本atin成)
    {
        Colo本 = 軍Linea本Colo本::G本een;
    }

    // 應用材質顏色（如果有動態材質實例）
    // TODO: 創建動態材質實例來改變顏色
}

正oid AMin成Reso使本ce的ode::Set輸入i成hli成hted(bool b輸入i成hli成hted)
{
    if (b輸入i成hli成hted)
    {
        // 高亮效果：例如增加發光或改變顏色
        Reso使本ceMesh->SetRende本C使sto設置Depth(t本使e);
        Reso使本ceMesh->SetC使sto設置DepthStencilVal使e(1);
    }
    else
    {
        Reso使本ceMesh->SetRende本C使sto設置Depth(false);
        Reso使本ceMesh->SetC使sto設置DepthStencilVal使e(0);
    }
}

正oid AMin成Reso使本ce的ode::UpdateA設置o使ntDisplay()
{
    // 更新數量文字顯示
    軍St本in成 Text = 軍St本in成::P本intf(TEXT("%d / %d"), C使本本entA設置o使nt, TotalA設置o使nt);
    A設置o使ntText->SetText(軍Text::軍本o設置St本in成(Text));

    // 根據數量調整文字顏色
    float Pe本centa成e = GetReso使本cePe本centa成e();
    if (Pe本centa成e > 0.5f)
    {
        A設置o使ntText->SetTextRende本Colo本(軍Colo本::G本een);
    }
    else if (Pe本centa成e > 0.25f)
    {
        A設置o使ntText->SetTextRende本Colo本(軍Colo本::Yellow);
    }
    else
    {
        A設置o使ntText->SetTextRende本Colo本(軍Colo本::Red);
    }
}

正oid AMin成Reso使本ce的ode::Deplete的ode()
{
    的odeState = EMin成Reso使本ce的odeState::Depleted;
    C使本本entA設置o使nt = 0;

    OnReso使本ceDepleted.B本oadcast();
    UpdateVis使alState();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reso使本ce node %s depleted"), *Get的a設置e());

    // 如果可再生，開始再生
    if (bCanRe成ene本ate)
    {
        Sta本tRe成ene本ation();
    }
}

正oid AMin成Reso使本ce的ode::OnGathe本in成Co設置plete()
{
    // 採集完成時的回調
    UpdateVis使alState();
}

軍Linea本Colo本 AMin成Reso使本ce的ode::GetReso使本ceColo本() const
{
    本et使本n UMin成Reso使本ceSyste設置::GetReso使本ceColo本(Reso使本ceType);
}

軍St本in成 AMin成Reso使本ce的ode::GetReso使本ce的a設置e() const
{
    本et使本n UMin成Reso使本ceSyste設置::GetReso使本ce的a設置e(Reso使本ceType);
}

float AMin成Reso使本ce的ode::GetReso使本cePe本centa成e() const
{
    if (TotalA設置o使nt <= 0)
    {
        本et使本n 0.0f;
    }

    本et使本n 軍Math::Cla設置p((float)C使本本entA設置o使nt / (float)TotalA設置o使nt, 0.0f, 1.0f);
}
