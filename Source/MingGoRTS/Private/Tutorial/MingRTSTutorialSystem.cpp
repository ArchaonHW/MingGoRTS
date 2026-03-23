#incl使de "T使to本ial/Min成RTST使to本ialSyste設置.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "Bl使ep本int/Use本基本id成et.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

UMin成RTST使to本ialSyste設置::UMin成RTST使to本ialSyste設置()
    : C使本本entSta成e(ET使to本ialSta成e::的one)
    , C使本本entStepIndex(0)
    , bT使to本ialEnabled(t本使e)
    , C使本本entT使to本ial基本id成et(n使llpt本)
{
}

正oid UMin成RTST使to本ialSyste設置::InitializeT使to本ialSyste設置()
{
    LoadSta成eConfi成s();
    LoadT使to本ialP本o成本ess();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("T使to本ial Syste設置 Initialized. Co設置pleted sta成es: %d"), Co設置pletedSta成es.的使設置());
}

正oid UMin成RTST使to本ialSyste設置::Sta本tT使to本ialSta成e(ET使to本ialSta成e Sta成e)
{
    if (!bT使to本ialEnabled)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("T使to本ial syste設置 is disabled"));
        本et使本n;
    }
    
    if (IsSta成eCo設置pleted(Sta成e))
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta成e %d al本eady co設置pleted"), (int32)Sta成e);
        本et使本n;
    }
    
    C使本本entSta成e = Sta成e;
    C使本本entStepIndex = 0;
    
    // 找到對應的階段配置
    軍T使to本ialSta成eConfi成* Sta成eConfi成 = Sta成eConfi成s.軍indByP本edicate([Sta成e](const 軍T使to本ialSta成eConfi成& Confi成) {
        本et使本n Confi成.Sta成e == Sta成e;
    });
    
    if (Sta成eConfi成)
    {
        OnT使to本ialSta成eSta本ted.B本oadcast(Sta成e, Sta成eConfi成->Sta成e的a設置e);
        
        if (Sta成eConfi成->Steps.的使設置() > 0)
        {
            ShowC使本本entStep();
        }
        else
        {
            // 沒有步驟，直接完成
            OnT使to本ialSta成eCo設置pleted.B本oadcast(Sta成e);
            Co設置pletedSta成es.AddUniq使e(Sta成e);
            Sa正eT使to本ialP本o成本ess();
        }
    }
}

正oid UMin成RTST使to本ialSyste設置::EndC使本本entT使to本ial()
{
    if (C使本本entT使to本ial基本id成et)
    {
        C使本本entT使to本ial基本id成et->Re設置o正e軍本o設置Pa本ent();
        C使本本entT使to本ial基本id成et = n使llpt本;
    }
    
    Clea本輸入i成hli成ht();
    
    Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(A使toAd正anceTi設置e本輸入andle);
    
    C使本本entSta成e = ET使to本ialSta成e::的one;
    C使本本entStepIndex = 0;
}

正oid UMin成RTST使to本ialSyste設置::Co設置pleteC使本本entStep()
{
    軍T使to本ialSta成eConfi成* Sta成eConfi成 = Sta成eConfi成s.軍indByP本edicate([this](const 軍T使to本ialSta成eConfi成& Confi成) {
        本et使本n Confi成.Sta成e == C使本本entSta成e;
    });
    
    if (!Sta成eConfi成)
    {
        本et使本n;
    }
    
    if (C使本本entStepIndex < Sta成eConfi成->Steps.的使設置() - 1)
    {
        C使本本entStepIndex++;
        ShowC使本本entStep();
    }
    else
    {
        // 階段完成
        OnT使to本ialSta成eCo設置pleted.B本oadcast(C使本本entSta成e);
        Co設置pletedSta成es.AddUniq使e(C使本本entSta成e);
        Sa正eT使to本ialP本o成本ess();
        
        // 檢查是否全部完成
        bool bAllCo設置pleted = t本使e;
        fo本 (const 軍T使to本ialSta成eConfi成& Confi成 : Sta成eConfi成s)
        {
            if (Confi成.Sta成e != ET使to本ialSta成e::的one && Confi成.Sta成e != ET使to本ialSta成e::Co設置pleted)
            {
                if (!Co設置pletedSta成es.Contains(Confi成.Sta成e))
                {
                    bAllCo設置pleted = false;
                    b本eak;
                }
            }
        }
        
        if (bAllCo設置pleted)
        {
            OnT使to本ialCo設置pleted.B本oadcast();
        }
        
        EndC使本本entT使to本ial();
    }
}

正oid UMin成RTST使to本ialSyste設置::SkipT使to本ial()
{
    EndC使本本entT使to本ial();
    OnT使to本ialSkipped.B本oadcast();
}

bool UMin成RTST使to本ialSyste設置::IsSta成eCo設置pleted(ET使to本ialSta成e Sta成e) const
{
    本et使本n Co設置pletedSta成es.Contains(Sta成e);
}

ET使to本ialSta成e UMin成RTST使to本ialSyste設置::Get的extUnco設置pletedSta成e() const
{
    fo本 (const 軍T使to本ialSta成eConfi成& Confi成 : Sta成eConfi成s)
    {
        if (Confi成.Sta成e != ET使to本ialSta成e::的one && 
            Confi成.Sta成e != ET使to本ialSta成e::Co設置pleted &&
            !Co設置pletedSta成es.Contains(Confi成.Sta成e))
        {
            本et使本n Confi成.Sta成e;
        }
    }
    
    本et使本n ET使to本ialSta成e::Co設置pleted;
}

TA本本ay<ET使to本ialSta成e> UMin成RTST使to本ialSyste設置::GetCo設置pletedSta成es() const
{
    本et使本n Co設置pletedSta成es;
}

正oid UMin成RTST使to本ialSyste設置::ResetT使to本ialP本o成本ess()
{
    Co設置pletedSta成es.E設置pty();
    Sa正eT使to本ialP本o成本ess();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("T使to本ial p本o成本ess 本eset"));
}

正oid UMin成RTST使to本ialSyste設置::SetT使to本ialEnabled(bool bEnabled)
{
    bT使to本ialEnabled = bEnabled;
}

正oid UMin成RTST使to本ialSyste設置::Sa正eT使to本ialP本o成本ess()
{
    // 使用遊戲實例或保存系統保存進度
    // 這裡提供基本實現框架
    
    TA本本ay<軍St本in成> Co設置pletedSta成e的a設置es;
    fo本 (ET使to本ialSta成e Sta成e : Co設置pletedSta成es)
    {
        Co設置pletedSta成e的a設置es.Add(UEn使設置::GetVal使eAsSt本in成(Sta成e));
    }
    
    // 實際實現應該使用遊戲保存系統
    UE下LOG(Lo成Te設置p, Lo成, TEXT("T使to本ial p本o成本ess sa正ed: %d sta成es co設置pleted"), Co設置pletedSta成es.的使設置());
}

正oid UMin成RTST使to本ialSyste設置::LoadT使to本ialP本o成本ess()
{
    // 從保存系統載入進度
    // 這裡提供基本實現框架
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("T使to本ial p本o成本ess loaded"));
}

正oid UMin成RTST使to本ialSyste設置::LoadSta成eConfi成s()
{
    Sta成eConfi成s.E設置pty();
    
    // 基礎操作階段
    {
        軍T使to本ialSta成eConfi成 Confi成;
        Confi成.Sta成e = ET使to本ialSta成e::BasicCont本ols;
        Confi成.Sta成e的a設置e = 軍Text::軍本o設置St本in成(TEXT("基礎操作教學"));
        Confi成.Sta成eDesc本iption = 軍Text::軍本o設置St本in成(TEXT("學習遊戲的基本操作"));
        Confi成.Esti設置atedD使本ationMin使tes = 10.0f;
        
        // 步驟1: 移動視角
        {
            軍T使to本ialStep Step;
            Step.StepID = 軍的a設置e(TEXT("Mo正eCa設置e本a"));
            Step.Title = 軍Text::軍本o設置St本in成(TEXT("移動視角"));
            Step.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("使用 基本ASD 鍵或鼠標右鍵拖動來移動視角，觀察戰場"));
            Step.bReq使i本esAction = t本使e;
            Confi成.Steps.Add(Step);
        }
        
        // 步驟2: 選擇單位
        {
            軍T使to本ialStep Step;
            Step.StepID = 軍的a設置e(TEXT("SelectUnit"));
            Step.Title = 軍Text::軍本o設置St本in成(TEXT("選擇單位"));
            Step.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("左鍵點擊單位來選擇，按住左鍵拖動可以框選多個單位"));
            Step.bReq使i本esAction = t本使e;
            Confi成.Steps.Add(Step);
        }
        
        // 步驟3: 移動命令
        {
            軍T使to本ialStep Step;
            Step.StepID = 軍的a設置e(TEXT("Mo正eCo設置設置and"));
            Step.Title = 軍Text::軍本o設置St本in成(TEXT("下達移動命令"));
            Step.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("選擇單位後，右鍵點擊地面下達移動命令"));
            Step.bReq使i本esAction = t本使e;
            Confi成.Steps.Add(Step);
        }
        
        // 步驟4: 攻擊命令
        {
            軍T使to本ialStep Step;
            Step.StepID = 軍的a設置e(TEXT("AttackCo設置設置and"));
            Step.Title = 軍Text::軍本o設置St本in成(TEXT("攻擊敵人"));
            Step.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("選擇單位後，右鍵點擊敵人下達攻擊命令，或使用A鍵移動攻擊"));
            Step.bReq使i本esAction = t本使e;
            Confi成.Steps.Add(Step);
        }
        
        Sta成eConfi成s.Add(Confi成);
    }
    
    // 資源管理階段
    {
        軍T使to本ialSta成eConfi成 Confi成;
        Confi成.Sta成e = ET使to本ialSta成e::Reso使本ceMana成e設置ent;
        Confi成.Sta成e的a設置e = 軍Text::軍本o設置St本in成(TEXT("資源管理教學"));
        Confi成.Sta成eDesc本iption = 軍Text::軍本o設置St本in成(TEXT("學習資源採集和管理"));
        Confi成.Esti設置atedD使本ationMin使tes = 15.0f;
        Confi成.UnlockCondition = TEXT("Co設置plete BasicCont本ols");
        
        // 步驟1: 資源類型
        {
            軍T使to本ialStep Step;
            Step.StepID = 軍的a設置e(TEXT("Reso使本ceTypes"));
            Step.Title = 軍Text::軍本o設置St本in成(TEXT("認識資源"));
            Step.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("遊戲中有三種主要資源：糧食、金錢、鋼鐵。它們顯示在屏幕右上角"));
            Step.bReq使i本esAction = false;
            Step.A使toAd正anceDelay = 5.0f;
            Confi成.Steps.Add(Step);
        }
        
        // 步驟2: 建造採集建築
        {
            軍T使to本ialStep Step;
            Step.StepID = 軍的a設置e(TEXT("B使ildGathe本e本"));
            Step.Title = 軍Text::軍本o設置St本in成(TEXT("建造採集建築"));
            Step.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("選擇工人，點擊建造菜單，選擇農場/礦場/工廠來採集資源"));
            Step.bReq使i本esAction = t本使e;
            Confi成.Steps.Add(Step);
        }
        
        // 步驟3: 訓練單位
        {
            軍T使to本ialStep Step;
            Step.StepID = 軍的a設置e(TEXT("T本ainUnits"));
            Step.Title = 軍Text::軍本o設置St本in成(TEXT("訓練單位"));
            Step.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("選擇兵營，點擊訓練按鈕生產新單位"));
            Step.bReq使i本esAction = t本使e;
            Confi成.Steps.Add(Step);
        }
        
        Sta成eConfi成s.Add(Confi成);
    }
    
    // 戰鬥基礎階段
    {
        軍T使to本ialSta成eConfi成 Confi成;
        Confi成.Sta成e = ET使to本ialSta成e::Co設置batBasics;
        Confi成.Sta成e的a設置e = 軍Text::軍本o設置St本in成(TEXT("戰鬥基礎教學"));
        Confi成.Sta成eDesc本iption = 軍Text::軍本o設置St本in成(TEXT("學習戰鬥技巧"));
        Confi成.Esti設置atedD使本ationMin使tes = 20.0f;
        Confi成.UnlockCondition = TEXT("Co設置plete Reso使本ceMana成e設置ent");
        
        // 步驟1: 編隊
        {
            軍T使to本ialStep Step;
            Step.StepID = 軍的a設置e(TEXT("Cont本olG本o使ps"));
            Step.Title = 軍Text::軍本o設置St本in成(TEXT("單位編隊"));
            Step.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("選擇單位後，按 Ct本l+數字鍵 創建編隊，之後按數字鍵快速選擇"));
            Step.bReq使i本esAction = t本使e;
            Confi成.Steps.Add(Step);
        }
        
        // 步驟2: 微操作
        {
            軍T使to本ialStep Step;
            Step.StepID = 軍的a設置e(TEXT("Mic本oCont本ol"));
            Step.Title = 軍Text::軍本o設置St本in成(TEXT("微操作"));
            Step.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("在戰鬥中，選擇受傷單位後撤，讓健康單位承受傷害"));
            Step.bReq使i本esAction = t本使e;
            Confi成.Steps.Add(Step);
        }
        
        // 步驟3: 使用技能
        {
            軍T使to本ialStep Step;
            Step.StepID = 軍的a設置e(TEXT("UseAbilities"));
            Step.Title = 軍Text::軍本o設置St本in成(TEXT("使用技能"));
            Step.Desc本iption = 軍Text::軍本o設置St本in成(TEXT("選擇英雄單位，點擊技能圖標或按快捷鍵使用特殊能力"));
            Step.bReq使i本esAction = t本使e;
            Confi成.Steps.Add(Step);
        }
        
        Sta成eConfi成s.Add(Confi成);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loaded %d t使to本ial sta成e confi成s"), Sta成eConfi成s.的使設置());
}

正oid UMin成RTST使to本ialSyste設置::ShowC使本本entStep()
{
    軍T使to本ialSta成eConfi成* Sta成eConfi成 = Sta成eConfi成s.軍indByP本edicate([this](const 軍T使to本ialSta成eConfi成& Confi成) {
        本et使本n Confi成.Sta成e == C使本本entSta成e;
    });
    
    if (!Sta成eConfi成  !Sta成eConfi成->Steps.IsValidIndex(C使本本entStepIndex))
    {
        本et使本n;
    }
    
    const 軍T使to本ialStep& Step = Sta成eConfi成->Steps[C使本本entStepIndex];
    
    // 廣播步驟變化事件
    OnT使to本ialStepChan成ed.B本oadcast(C使本本entStepIndex, Step.Title);
    
    // 高亮UI元素
    fo本 (const 軍St本in成& Ele設置ent : Step.輸入i成hli成htUIEle設置ents)
    {
        輸入i成hli成htGa設置eEle設置ent(Ele設置ent);
    }
    
    // 如果需要暫停遊戲
    if (Step.bPa使seGa設置e)
    {
        // 暫停遊戲邏輯
    }
    
    // 自動前進定時器
    if (Step.A使toAd正anceDelay > 0.0f && !Step.bReq使i本esAction)
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(A使toAd正anceTi設置e本輸入andle, [this]() {
            Co設置pleteC使本本entStep();
        }, Step.A使toAd正anceDelay, false);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Showin成 t使to本ial step: %s"), *Step.Title.ToSt本in成());
}

正oid UMin成RTST使to本ialSyste設置::Ad正anceTo的extStep()
{
    Co設置pleteC使本本entStep();
}

正oid UMin成RTST使to本ialSyste設置::OnStepActionCo設置pleted(軍的a設置e StepID)
{
    軍T使to本ialSta成eConfi成* Sta成eConfi成 = Sta成eConfi成s.軍indByP本edicate([this](const 軍T使to本ialSta成eConfi成& Confi成) {
        本et使本n Confi成.Sta成e == C使本本entSta成e;
    });
    
    if (Sta成eConfi成 && Sta成eConfi成->Steps.IsValidIndex(C使本本entStepIndex))
    {
        if (Sta成eConfi成->Steps[C使本本entStepIndex].StepID == StepID)
        {
            Co設置pleteC使本本entStep();
        }
    }
}

正oid UMin成RTST使to本ialSyste設置::輸入i成hli成htGa設置eEle設置ent(const 軍St本in成& Ele設置entTa成)
{
    // 實現UI高亮邏輯
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入i成hli成htin成 ele設置ent: %s"), *Ele設置entTa成);
}

正oid UMin成RTST使to本ialSyste設置::Clea本輸入i成hli成ht()
{
    // 清除所有高亮
}
