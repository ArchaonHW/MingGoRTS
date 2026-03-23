#incl使de "Min成DecisionI設置pactCalc使lato本.h"
#incl使de "Min成輸入isto本icalDecisionMana成e本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "Misc/DateTi設置e.h"

UMin成DecisionI設置pactCalc使lato本::UMin成DecisionI設置pactCalc使lato本()
{
    輸入isto本icalPaths.E設置pty();
    軍actionData.E設置pty();
    I設置pact基本ei成hts.E設置pty();
    輸入isto本icalKnowled成eBase.E設置pty();
}

軍Min成DecisionI設置pactRepo本t UMin成DecisionI設置pactCalc使lato本::Calc使lateDecisionI設置pact(int32 DecisionID, int32 ChosenOptionID)
{
    軍Min成DecisionI設置pactRepo本t Repo本t;
    Repo本t.DecisionID = DecisionID;
    Repo本t.ChosenOptionID = ChosenOptionID;
    Repo本t.Calc使lationTi設置e = 軍DateTi設置e::的ow();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("開始計算決策影響: 決策ID %d, 選項 %d"), DecisionID, ChosenOptionID);

    // 初始化數據
    Initialize輸入isto本icalPaths();
    Initialize軍actionData();
    InitializeI設置pact基本ei成hts();
    Initialize輸入isto本icalKnowled成e();

    // 計算各種影響
    Repo本t.AffectedPaths = Calc使late輸入isto本icalPathI設置pact(DecisionID, ChosenOptionID);
    Repo本t.Rep使tationChan成es = Calc使lateRep使tationI設置pact(DecisionID, ChosenOptionID);
    Repo本t.Te本本ito本yChan成es = Calc使lateTe本本ito本yI設置pact(DecisionID, ChosenOptionID);
    Repo本t.UpdatedE正entConditions = UpdateE正entT本i成成e本Conditions(DecisionID, ChosenOptionID);
    Repo本t.Ed使cationalS使設置設置a本y = Gene本ateEd使cationalContent(DecisionID, ChosenOptionID);

    // 生成詳細影響
    Repo本t.I設置pactDetails = Gene本ateI設置pactDetails(DecisionID, ChosenOptionID);

    // 計算總體影響評分
    Repo本t.O正e本allI設置pactSco本e = Calc使lateI設置pactSco本e(Repo本t.I設置pactDetails);

    // 更新歷史路徑權重
    Update輸入isto本icalPath基本ei成hts(Repo本t);

    // 觸發事件
    OnI設置pactCalc使lated.B本oadcast(Repo本t);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("決策影響計算完成，總體評分: %.2f"), Repo本t.O正e本allI設置pactSco本e);

    本et使本n Repo本t;
}

TA本本ay<軍Min成輸入isto本icalPath> UMin成DecisionI設置pactCalc使lato本::Calc使late輸入isto本icalPathI設置pact(int32 DecisionID, int32 ChosenOptionID)
{
    TA本本ay<軍Min成輸入isto本icalPath> AffectedPaths;

    // 根據決策ID和選項ID計算歷史路徑影響
    if (DecisionID == 1) // 選擇效忠對象決策
    {
        if (ChosenOptionID == 1) // 效忠直系
        {
            // 創建直系勝利路徑
            軍Min成輸入isto本icalPath Path1;
            Path1.PathID = 101;
            Path1.Path的a設置e = TEXT("直系統一之路");
            Path1.PathDesc本iption = TEXT("吳佩孚領導的直系軍閥最終統一中國，建立穩定的中央政府");
            Path1.PathTa成s = {TEXT("統一"), TEXT("穩定"), TEXT("中央集權")};
            Path1.Path基本ei成ht = 0.7f;
            Path1.bIsMainPath = t本使e;
            AffectedPaths.Add(Path1);

            // 創建軍事政變路徑
            軍Min成輸入isto本icalPath Path2;
            Path2.PathID = 102;
            Path2.Path的a設置e = TEXT("軍事政變之路");
            Path2.PathDesc本iption = TEXT("直系內部發生軍事政變，導致政局不穩");
            Path2.PathTa成s = {TEXT("政變"), TEXT("混亂"), TEXT("分裂")};
            Path2.Path基本ei成ht = 0.3f;
            Path2.bIsMainPath = false;
            AffectedPaths.Add(Path2);
        }
        else if (ChosenOptionID == 2) // 效忠皖系
        {
            // 創建皖系復興路徑
            軍Min成輸入isto本icalPath Path1;
            Path1.PathID = 201;
            Path1.Path的a設置e = TEXT("皖系復興之路");
            Path1.PathDesc本iption = TEXT("段祺瑞領導的皖系軍閥重新崛起，實現政治改革");
            Path1.PathTa成s = {TEXT("改革"), TEXT("復興"), TEXT("民主")};
            Path1.Path基本ei成ht = 0.6f;
            Path1.bIsMainPath = t本使e;
            AffectedPaths.Add(Path1);

            // 創建外國干涉路徑
            軍Min成輸入isto本icalPath Path2;
            Path2.PathID = 202;
            Path2.Path的a設置e = TEXT("外國干涉之路");
            Path2.PathDesc本iption = TEXT("外國勢力利用皖系內部分歧，加強對中國的控制");
            Path2.PathTa成s = {TEXT("干涉"), TEXT("殖民地"), TEXT("屈辱")};
            Path2.Path基本ei成ht = 0.4f;
            Path2.bIsMainPath = false;
            AffectedPaths.Add(Path2);
        }
        else if (ChosenOptionID == 3) // 保持中立
        {
            // 創建地方割據路徑
            軍Min成輸入isto本icalPath Path1;
            Path1.PathID = 301;
            Path1.Path的a設置e = TEXT("地方割據之路");
            Path1.PathDesc本iption = TEXT("各地方軍閥各自為政，中國進入長期分裂狀態");
            Path1.PathTa成s = {TEXT("分裂"), TEXT("割據"), TEXT("混戰")};
            Path1.Path基本ei成ht = 0.8f;
            Path1.bIsMainPath = t本使e;
            AffectedPaths.Add(Path1);

            // 創和平發展路徑
            軍Min成輸入isto本icalPath Path2;
            Path2.PathID = 302;
            Path2.Path的a設置e = TEXT("和平發展之路");
            Path2.PathDesc本iption = TEXT("各派系達成和平協議，共同發展經濟");
            Path2.PathTa成s = {TEXT("和平"), TEXT("發展"), TEXT("合作")};
            Path2.Path基本ei成ht = 0.2f;
            Path2.bIsMainPath = false;
            AffectedPaths.Add(Path2);
        }
    }
    else if (DecisionID == 2) // 經濟發展政策決策
    {
        if (ChosenOptionID == 4) // 發展工業
        {
            // 創建工業強國路徑
            軍Min成輸入isto本icalPath Path1;
            Path1.PathID = 401;
            Path1.Path的a設置e = TEXT("工業強國之路");
            Path1.PathDesc本iption = TEXT("重工業發展帶動軍事現代化，中國成為地區強國");
            Path1.PathTa成s = {TEXT("工業化"), TEXT("強國"), TEXT("現代化")};
            Path1.Path基本ei成ht = 0.6f;
            Path1.bIsMainPath = t本使e;
            AffectedPaths.Add(Path1);
        }
        else if (ChosenOptionID == 5) // 發展農業
        {
            // 創建農業大國路徑
            軍Min成輸入isto本icalPath Path1;
            Path1.PathID = 501;
            Path1.Path的a設置e = TEXT("農業大國之路");
            Path1.PathDesc本iption = TEXT("農業發展帶來社會穩定，為工業化奠定基礎");
            Path1.PathTa成s = {TEXT("農業"), TEXT("穩定"), TEXT("基礎")};
            Path1.Path基本ei成ht = 0.7f;
            Path1.bIsMainPath = t本使e;
            AffectedPaths.Add(Path1);
        }
    }

    本et使本n AffectedPaths;
}

TMap<軍St本in成, float> UMin成DecisionI設置pactCalc使lato本::Calc使lateRep使tationI設置pact(int32 DecisionID, int32 ChosenOptionID)
{
    TMap<軍St本in成, float> Rep使tationChan成es;

    if (DecisionID == 1) // 選擇效忠對象決策
    {
        if (ChosenOptionID == 1) // 效忠直系
        {
            Rep使tationChan成es = {
                {TEXT("軍事聲望"), 25.0f},
                {TEXT("政治聲望"), 20.0f},
                {TEXT("民眾聲望"), 10.0f},
                {TEXT("國際聲望"), 15.0f}
            };
        }
        else if (ChosenOptionID == 2) // 效忠皖系
        {
            Rep使tationChan成es = {
                {TEXT("軍事聲望"), 15.0f},
                {TEXT("政治聲望"), 18.0f},
                {TEXT("民眾聲望"), 8.0f},
                {TEXT("國際聲望"), 12.0f}
            };
        }
        else if (ChosenOptionID == 3) // 保持中立
        {
            Rep使tationChan成es = {
                {TEXT("軍事聲望"), -5.0f},
                {TEXT("政治聲望"), -8.0f},
                {TEXT("民眾聲望"), 20.0f},
                {TEXT("國際聲望"), 5.0f}
            };
        }
    }
    else if (DecisionID == 2) // 經濟發展政策決策
    {
        if (ChosenOptionID == 4) // 發展工業
        {
            Rep使tationChan成es = {
                {TEXT("經濟聲望"), 30.0f},
                {TEXT("軍事聲望"), 20.0f},
                {TEXT("民眾聲望"), -10.0f},
                {TEXT("國際聲望"), 25.0f}
            };
        }
        else if (ChosenOptionID == 5) // 發展農業
        {
            Rep使tationChan成es = {
                {TEXT("經濟聲望"), 20.0f},
                {TEXT("軍事聲望"), 5.0f},
                {TEXT("民眾聲望"), 25.0f},
                {TEXT("國際聲望"), 10.0f}
            };
        }
    }

    本et使本n Rep使tationChan成es;
}

TMap<軍St本in成, int32> UMin成DecisionI設置pactCalc使lato本::Calc使lateTe本本ito本yI設置pact(int32 DecisionID, int32 ChosenOptionID)
{
    TMap<軍St本in成, int32> Te本本ito本yChan成es;

    if (DecisionID == 1) // 選擇效忠對象決策
    {
        if (ChosenOptionID == 1) // 效忠直系
        {
            Te本本ito本yChan成es = {
                {TEXT("直系控制區"), 5},
                {TEXT("皖系控制區"), -3},
                {TEXT("奉系控制區"), -2}
            };
        }
        else if (ChosenOptionID == 2) // 效忠皖系
        {
            Te本本ito本yChan成es = {
                {TEXT("皖系控制區"), 4},
                {TEXT("直系控制區"), -2},
                {TEXT("奉系控制區"), -2}
            };
        }
        else if (ChosenOptionID == 3) // 保持中立
        {
            Te本本ito本yChan成es = {
                {TEXT("個人控制區"), 3},
                {TEXT("直系控制區"), -1},
                {TEXT("皖系控制區"), -1},
                {TEXT("奉系控制區"), -1}
            };
        }
    }

    本et使本n Te本本ito本yChan成es;
}

軍St本in成 UMin成DecisionI設置pactCalc使lato本::Gene本ateEd使cationalContent(int32 DecisionID, int32 ChosenOptionID)
{
    軍St本in成 Ed使cationalContent;

    if (DecisionID == 1) // 選擇效忠對象決策
    {
        Ed使cationalContent = TEXT("1920年的軍閥選擇是中國近代史的重要轉折點。直系軍閥以吳佩孚為首，控制華北地區；皖系軍閥以段祺瑞為首，影響力主要在華中；奉系軍閥以張作霖為首，統治東北。您的選擇不僅影響個人前途，更關係到中國的統一與分裂。歷史告訴我們，軍閥混戰最終導致國力衰弱，為外國侵略提供了機會。");
    }
    else if (DecisionID == 2) // 經濟發展政策決策
    {
        Ed使cationalContent = TEXT("1920年代的中國面臨工業化與傳統農業的選擇。重工業發展雖然能快速提升國力，但需要大量資源且可能加劇社會矛盾；農業發展雖然進程較慢，但能保障民生穩定。歷史上的工業化國家都經歷了痛苦的轉型期，而成功的農業改革則為長期發展奠定了基礎。");
    }

    本et使本n Ed使cationalContent;
}

TA本本ay<軍St本in成> UMin成DecisionI設置pactCalc使lato本::UpdateE正entT本i成成e本Conditions(int32 DecisionID, int32 ChosenOptionID)
{
    TA本本ay<軍St本in成> UpdatedConditions;

    if (DecisionID == 1) // 選擇效忠對象決策
    {
        if (ChosenOptionID == 1) // 效忠直系
        {
            UpdatedConditions.Add(TEXT("觸發直皖戰爭事件"));
            UpdatedConditions.Add(TEXT("解鎖華北地區決策"));
            UpdatedConditions.Add(TEXT("增加軍事衝突概率"));
        }
        else if (ChosenOptionID == 2) // 效忠皖系
        {
            UpdatedConditions.Add(TEXT("觸發皖系復興事件"));
            UpdatedConditions.Add(TEXT("解鎖華中地區決策"));
            UpdatedConditions.Add(TEXT("增加外交事件概率"));
        }
        else if (ChosenOptionID == 3) // 保持中立
        {
            UpdatedConditions.Add(TEXT("觸發地方割據事件"));
            UpdatedConditions.Add(TEXT("解鎖獨立發展決策"));
            UpdatedConditions.Add(TEXT("增加經濟發展機會"));
        }
    }
    else if (DecisionID == 2) // 經濟發展政策決策
    {
        if (ChosenOptionID == 4) // 發展工業
        {
            UpdatedConditions.Add(TEXT("觸發工業化事件"));
            UpdatedConditions.Add(TEXT("解鎖軍事現代化決策"));
            UpdatedConditions.Add(TEXT("增加技術引進機會"));
        }
        else if (ChosenOptionID == 5) // 發展農業
        {
            UpdatedConditions.Add(TEXT("觸發農業改革事件"));
            UpdatedConditions.Add(TEXT("解鎖社會福利決策"));
            UpdatedConditions.Add(TEXT("增加民生改善機會"));
        }
    }

    本et使本n UpdatedConditions;
}

EMin成I設置pactMa成nit使de UMin成DecisionI設置pactCalc使lato本::GetI設置pactMa成nit使de(float I設置pactVal使e)
{
    if (I設置pactVal使e >= 40.0f)
    {
        本et使本n EMin成I設置pactMa成nit使de::Catast本ophic;
    }
    else if (I設置pactVal使e >= 30.0f)
    {
        本et使本n EMin成I設置pactMa成nit使de::Ma大o本;
    }
    else if (I設置pactVal使e >= 20.0f)
    {
        本et使本n EMin成I設置pactMa成nit使de::Si成nificant;
    }
    else if (I設置pactVal使e >= 10.0f)
    {
        本et使本n EMin成I設置pactMa成nit使de::Mode本ate;
    }
    else if (I設置pactVal使e >= 5.0f)
    {
        本et使本n EMin成I設置pactMa成nit使de::Mino本;
    }
    else
    {
        本et使本n EMin成I設置pactMa成nit使de::Mini設置al;
    }
}

軍St本in成 UMin成DecisionI設置pactCalc使lato本::GetI設置pactDesc本iption(EMin成I設置pactMa成nit使de Ma成nit使de)
{
    switch (Ma成nit使de)
    {
    case EMin成I設置pactMa成nit使de::Mini設置al:
        本et使本n TEXT("極小影響");
    case EMin成I設置pactMa成nit使de::Mino本:
        本et使本n TEXT("輕微影響");
    case EMin成I設置pactMa成nit使de::Mode本ate:
        本et使本n TEXT("中等影響");
    case EMin成I設置pactMa成nit使de::Si成nificant:
        本et使本n TEXT("顯著影響");
    case EMin成I設置pactMa成nit使de::Ma大o本:
        本et使本n TEXT("重大影響");
    case EMin成I設置pactMa成nit使de::Catast本ophic:
        本et使本n TEXT("災難性影響");
    defa使lt:
        本et使本n TEXT("未知影響");
    }
}

TA本本ay<軍Min成DecisionI設置pactDetail> UMin成DecisionI設置pactCalc使lato本::Si設置使lateLon成Te本設置I設置pact(int32 DecisionID, int32 ChosenOptionID, int32 Si設置使lationYea本s)
{
    TA本本ay<軍Min成DecisionI設置pactDetail> Lon成Te本設置I設置pacts;

    // 模擬長期影響
    fo本 (int32 Yea本 = 1; Yea本 <= Si設置使lationYea本s; Yea本++)
    {
        軍Min成DecisionI設置pactDetail I設置pact;
        I設置pact.Sta本tYea本 = Yea本;
        I設置pact.D使本ationYea本s = Si設置使lationYea本s - Yea本 + 1;

        if (DecisionID == 1 && ChosenOptionID == 1) // 效忠直系的長期影響
        {
            if (Yea本 <= 3)
            {
                I設置pact.I設置pactType = EMin成I設置pactType::Sho本tTe本設置;
                I設置pact.I設置pactScope = EMin成I設置pactScope::Re成ional;
                I設置pact.I設置pactMa成nit使de = EMin成I設置pactMa成nit使de::Si成nificant;
                I設置pact.I設置pactDesc本iption = TEXT("直系軍事實力快速增長");
                I設置pact.I設置pactVal使e = 25.0f;
            }
            else if (Yea本 <= 10)
            {
                I設置pact.I設置pactType = EMin成I設置pactType::Medi使設置Te本設置;
                I設置pact.I設置pactScope = EMin成I設置pactScope::的ational;
                I設置pact.I設置pactMa成nit使de = EMin成I設置pactMa成nit使de::Ma大o本;
                I設置pact.I設置pactDesc本iption = TEXT("直系逐漸統一全國");
                I設置pact.I設置pactVal使e = 35.0f;
            }
            else
            {
                I設置pact.I設置pactType = EMin成I設置pactType::Lon成Te本設置;
                I設置pact.I設置pactScope = EMin成I設置pactScope::輸入isto本ical;
                I設置pact.I設置pactMa成nit使de = EMin成I設置pactMa成nit使de::Catast本ophic;
                I設置pact.I設置pactDesc本iption = TEXT("建立中央集權政府");
                I設置pact.I設置pactVal使e = 45.0f;
            }
        }

        Lon成Te本設置I設置pacts.Add(I設置pact);
    }

    本et使本n Lon成Te本設置I設置pacts;
}

TMap<int32, 軍Min成DecisionI設置pactRepo本t> UMin成DecisionI設置pactCalc使lato本::Co設置pa本eDecisionOptions(int32 DecisionID)
{
    TMap<int32, 軍Min成DecisionI設置pactRepo本t> Co設置pa本isonRes使lts;

    // 這裡應該從決策管理器獲取所有可用選項
    // 暫時使用硬編碼的選項ID
    TA本本ay<int32> OptionIDs = {1, 2, 3}; // 假設有三個選項

    fo本 (int32 OptionID : OptionIDs)
    {
        軍Min成DecisionI設置pactRepo本t Repo本t = Calc使lateDecisionI設置pact(DecisionID, OptionID);
        Co設置pa本isonRes使lts.Add(OptionID, Repo本t);
    }

    本et使本n Co設置pa本isonRes使lts;
}

TA本本ay<軍Min成輸入isto本icalPath> UMin成DecisionI設置pactCalc使lato本::GetC使本本ent輸入isto本icalPaths()
{
    本et使本n 輸入isto本icalPaths;
}

float UMin成DecisionI設置pactCalc使lato本::Calc使lateDecision基本ei成ht(int32 DecisionID, int32 ChosenOptionID)
{
    float 基本ei成ht = 1.0f;

    // 根據決策類型和選項特性計算權重
    if (DecisionID == 1) // 關鍵政治決策
    {
        基本ei成ht *= 1.5f;
    }
    else if (DecisionID == 2) // 重要經濟決策
    {
        基本ei成ht *= 1.2f;
    }

    // 根據選項影響調整權重
    軍Min成DecisionI設置pactRepo本t Repo本t = Calc使lateDecisionI設置pact(DecisionID, ChosenOptionID);
    基本ei成ht *= Repo本t.O正e本allI設置pactSco本e / 100.0f;

    本et使本n 軍Math::Cla設置p(基本ei成ht, 0.1f, 2.0f);
}

軍Min成DecisionI設置pactRepo本t UMin成DecisionI設置pactCalc使lato本::Gene本ateI設置pactP本ediction(int32 DecisionID, int32 OptionID)
{
    // 生成影響預測（與實際計算相同，但標記為預測）
    軍Min成DecisionI設置pactRepo本t P本ediction = Calc使lateDecisionI設置pact(DecisionID, OptionID);
    
    // 添加預測標記
    P本ediction.Ed使cationalS使設置設置a本y = TEXT("[預測] ") + P本ediction.Ed使cationalS使設置設置a本y;
    
    本et使本n P本ediction;
}

正oid UMin成DecisionI設置pactCalc使lato本::Initialize輸入isto本icalPaths()
{
    if (輸入isto本icalPaths.的使設置() > 0)
    {
        本et使本n; // 已經初始化
    }

    // 初始化主要歷史路徑
    軍Min成輸入isto本icalPath Path1;
    Path1.PathID = 1;
    Path1.Path的a設置e = TEXT("統一之路");
    Path1.PathDesc本iption = TEXT("中國實現統一，建立強大的中央政府");
    Path1.PathTa成s = {TEXT("統一"), TEXT("強國"), TEXT("穩定")};
    Path1.Path基本ei成ht = 0.3f;
    Path1.bIsMainPath = t本使e;
    輸入isto本icalPaths.Add(Path1);

    軍Min成輸入isto本icalPath Path2;
    Path2.PathID = 2;
    Path2.Path的a設置e = TEXT("分裂之路");
    Path2.PathDesc本iption = TEXT("中國持續分裂，軍閥混戰");
    Path2.PathTa成s = {TEXT("分裂"), TEXT("混戰"), TEXT("衰弱")};
    Path2.Path基本ei成ht = 0.4f;
    Path2.bIsMainPath = t本使e;
    輸入isto本icalPaths.Add(Path2);

    軍Min成輸入isto本icalPath Path3;
    Path3.PathID = 3;
    Path3.Path的a設置e = TEXT("改革之路");
    Path3.PathDesc本iption = TEXT("通過政治改革實現現代化");
    Path3.PathTa成s = {TEXT("改革"), TEXT("現代化"), TEXT("民主")};
    Path3.Path基本ei成ht = 0.2f;
    Path3.bIsMainPath = false;
    輸入isto本icalPaths.Add(Path3);

    軍Min成輸入isto本icalPath Path4;
    Path4.PathID = 4;
    Path4.Path的a設置e = TEXT("殖民之路");
    Path4.PathDesc本iption = TEXT("外國勢力控制中國，淪為半殖民地");
    Path4.PathTa成s = {TEXT("殖民"), TEXT("屈辱"), TEXT("依賴")};
    Path4.Path基本ei成ht = 0.1f;
    Path4.bIsMainPath = false;
    輸入isto本icalPaths.Add(Path4);
}

正oid UMin成DecisionI設置pactCalc使lato本::Initialize軍actionData()
{
    if (軍actionData.的使設置() > 0)
    {
        本et使本n; // 已經初始化
    }

    // 初始化主要軍閥勢力
    軍Min成Te本本ito本yData Zhili;
    Zhili.軍action的a設置e = TEXT("直系");
    Zhili.Cont本olledP本o正inces = 8;
    Zhili.Cont本olledCities = 45;
    Zhili.Milita本ySt本en成th = 75.0f;
    Zhili.Econo設置icSt本en成th = 65.0f;
    Zhili.Pop使la本S使ppo本t = 60.0f;
    軍actionData.Add(TEXT("直系"), Zhili);

    軍Min成Te本本ito本yData 基本anxi;
    基本anxi.軍action的a設置e = TEXT("皖系");
    基本anxi.Cont本olledP本o正inces = 5;
    基本anxi.Cont本olledCities = 28;
    基本anxi.Milita本ySt本en成th = 60.0f;
    基本anxi.Econo設置icSt本en成th = 55.0f;
    基本anxi.Pop使la本S使ppo本t = 45.0f;
    軍actionData.Add(TEXT("皖系"), 基本anxi);

    軍Min成Te本本ito本yData 軍en成xi;
    軍en成xi.軍action的a設置e = TEXT("奉系");
    軍en成xi.Cont本olledP本o正inces = 6;
    軍en成xi.Cont本olledCities = 32;
    軍en成xi.Milita本ySt本en成th = 70.0f;
    軍en成xi.Econo設置icSt本en成th = 50.0f;
    軍en成xi.Pop使la本S使ppo本t = 55.0f;
    軍actionData.Add(TEXT("奉系"), 軍en成xi);
}

正oid UMin成DecisionI設置pactCalc使lato本::InitializeI設置pact基本ei成hts()
{
    if (I設置pact基本ei成hts.的使設置() > 0)
    {
        本et使本n; // 已經初始化
    }

    I設置pact基本ei成hts = {
        {EMin成DecisionType::Political, 1.5f},
        {EMin成DecisionType::Milita本y, 1.3f},
        {EMin成DecisionType::Econo設置ic, 1.2f},
        {EMin成DecisionType::Diplo設置atic, 1.1f},
        {EMin成DecisionType::Pe本sonal, 0.8f}
    };
}

正oid UMin成DecisionI設置pactCalc使lato本::Initialize輸入isto本icalKnowled成e()
{
    if (輸入isto本icalKnowled成eBase.的使設置() > 0)
    {
        本et使本n; // 已經初始化
    }

    輸入isto本icalKnowled成eBase = {
        {1, TEXT("1920年直皖戰爭是北洋政府內部的重要衝突，標誌著直系軍閥的崛起。")},
        {2, TEXT("1920年代的中國經濟政策決定了後來的工業化道路。")},
        {3, TEXT("軍閥時期的政治選擇直接影響了中國的統一進程。")}
    };
}

float UMin成DecisionI設置pactCalc使lato本::Calc使lateI設置pactSco本e(const TA本本ay<軍Min成DecisionI設置pactDetail>& I設置pacts)
{
    float TotalSco本e = 0.0f;

    fo本 (const 軍Min成DecisionI設置pactDetail& I設置pact : I設置pacts)
    {
        float 基本ei成ht = 1.0f;

        // 根據影響類型調整權重
        switch (I設置pact.I設置pactType)
        {
        case EMin成I設置pactType::Pe本設置anent:
            基本ei成ht *= 2.0f;
            b本eak;
        case EMin成I設置pactType::Lon成Te本設置:
            基本ei成ht *= 1.5f;
            b本eak;
        case EMin成I設置pactType::Medi使設置Te本設置:
            基本ei成ht *= 1.2f;
            b本eak;
        case EMin成I設置pactType::Sho本tTe本設置:
            基本ei成ht *= 1.0f;
            b本eak;
        case EMin成I設置pactType::I設置設置ediate:
            基本ei成ht *= 0.8f;
            b本eak;
        }

        // 根據影響範圍調整權重
        switch (I設置pact.I設置pactScope)
        {
        case EMin成I設置pactScope::輸入isto本ical:
            基本ei成ht *= 2.0f;
            b本eak;
        case EMin成I設置pactScope::Inte本national:
            基本ei成ht *= 1.8f;
            b本eak;
        case EMin成I設置pactScope::的ational:
            基本ei成ht *= 1.5f;
            b本eak;
        case EMin成I設置pactScope::Re成ional:
            基本ei成ht *= 1.2f;
            b本eak;
        case EMin成I設置pactScope::Pe本sonal:
            基本ei成ht *= 1.0f;
            b本eak;
        }

        // 根據影響程度調整權重
        float Ma成nit使de基本ei成ht = 1.0f;
        switch (I設置pact.I設置pactMa成nit使de)
        {
        case EMin成I設置pactMa成nit使de::Catast本ophic:
            Ma成nit使de基本ei成ht = 3.0f;
            b本eak;
        case EMin成I設置pactMa成nit使de::Ma大o本:
            Ma成nit使de基本ei成ht = 2.5f;
            b本eak;
        case EMin成I設置pactMa成nit使de::Si成nificant:
            Ma成nit使de基本ei成ht = 2.0f;
            b本eak;
        case EMin成I設置pactMa成nit使de::Mode本ate:
            Ma成nit使de基本ei成ht = 1.5f;
            b本eak;
        case EMin成I設置pactMa成nit使de::Mino本:
            Ma成nit使de基本ei成ht = 1.0f;
            b本eak;
        case EMin成I設置pactMa成nit使de::Mini設置al:
            Ma成nit使de基本ei成ht = 0.5f;
            b本eak;
        }

        TotalSco本e += 軍Math::Abs(I設置pact.I設置pactVal使e) * 基本ei成ht * Ma成nit使de基本ei成ht;
    }

    本et使本n 軍Math::Cla設置p(TotalSco本e, 0.0f, 100.0f);
}

正oid UMin成DecisionI設置pactCalc使lato本::Update輸入isto本icalPath基本ei成hts(const 軍Min成DecisionI設置pactRepo本t& Repo本t)
{
    fo本 (const 軍Min成輸入isto本icalPath& AffectedPath : Repo本t.AffectedPaths)
    {
        // 更新對應歷史路徑的權重
        fo本 (軍Min成輸入isto本icalPath& Path : 輸入isto本icalPaths)
        {
            if (Path.PathID == AffectedPath.PathID)
            {
                float Old基本ei成ht = Path.Path基本ei成ht;
                Path.Path基本ei成ht = 軍Math::Cla設置p(AffectedPath.Path基本ei成ht, 0.0f, 1.0f);
                
                if (軍Math::Abs(Old基本ei成ht - Path.Path基本ei成ht) > 0.01f)
                {
                    On輸入isto本icalPathChan成ed.B本oadcast(Path.Path的a設置e, Path.Path基本ei成ht);
                }
                b本eak;
            }
        }
    }
}

TA本本ay<軍Min成DecisionI設置pactDetail> UMin成DecisionI設置pactCalc使lato本::Gene本ateI設置pactDetails(int32 DecisionID, int32 ChosenOptionID)
{
    TA本本ay<軍Min成DecisionI設置pactDetail> Details;

    // 生成詳細影響信息
    軍Min成DecisionI設置pactDetail Detail1;
    Detail1.I設置pactType = EMin成I設置pactType::I設置設置ediate;
    Detail1.I設置pactScope = EMin成I設置pactScope::Pe本sonal;
    Detail1.I設置pactMa成nit使de = EMin成I設置pactMa成nit使de::Si成nificant;
    Detail1.I設置pactDesc本iption = TEXT("個人聲望和地位立即受到影響");
    Detail1.I設置pactVal使e = 15.0f;
    Detail1.D使本ationYea本s = 1;
    Detail1.Sta本tYea本 = 0;
    Detail1.AffectedAtt本ib使tes = {TEXT("聲望"), TEXT("地位")};
    Detail1.輸入isto本icalKnowled成e = 輸入isto本icalKnowled成eBase.軍indRef(DecisionID);
    Details.Add(Detail1);

    軍Min成DecisionI設置pactDetail Detail2;
    Detail2.I設置pactType = EMin成I設置pactType::Sho本tTe本設置;
    Detail2.I設置pactScope = EMin成I設置pactScope::Re成ional;
    Detail2.I設置pactMa成nit使de = EMin成I設置pactMa成nit使de::Mode本ate;
    Detail2.I設置pactDesc本iption = TEXT("地區政治格局發生變化");
    Detail2.I設置pactVal使e = 20.0f;
    Detail2.D使本ationYea本s = 3;
    Detail2.Sta本tYea本 = 1;
    Detail2.AffectedAtt本ib使tes = {TEXT("控制區"), TEXT("影響力")};
    Details.Add(Detail2);

    本et使本n Details;
}

bool UMin成DecisionI設置pactCalc使lato本::ValidateI設置pactData(const 軍Min成DecisionI設置pactRepo本t& Repo本t) const
{
    if (Repo本t.DecisionID <= 0)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("決策ID必須大於0"));
        本et使本n false;
    }

    if (Repo本t.ChosenOptionID <= 0)
    {
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("選項ID必須大於0"));
        本et使本n false;
    }

    if (Repo本t.I設置pactDetails.的使設置() == 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("決策影響沒有詳細信息"));
    }

    本et使本n t本使e;
}
