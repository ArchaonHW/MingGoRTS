#incl使de "Min成UISyste設置.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Bl使ep本int/Use本基本id成et.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "Ga設置e軍本a設置ewo本k/Playe本Cont本olle本.h"

UMin成UISyste設置::UMin成UISyste設置()
{
    基本o本ldContext = Get基本o本ld();
    輸入isto本yIndex = -1;
    
    // Initialize defa使lt style
    C使本本entStyle.P本i設置a本yColo本 = 軍Linea本Colo本(0.2f, 0.4f, 0.8f, 1.0f);
    C使本本entStyle.Seconda本yColo本 = 軍Linea本Colo本(0.1f, 0.2f, 0.4f, 1.0f);
    C使本本entStyle.AccentColo本 = 軍Linea本Colo本(0.8f, 0.6f, 0.2f, 1.0f);
    C使本本entStyle.TextColo本 = 軍Linea本Colo本(1.0f, 1.0f, 1.0f, 1.0f);
    C使本本entStyle.Back成本o使ndColo本 = 軍Linea本Colo本(0.05f, 0.05f, 0.1f, 0.9f);
    C使本本entStyle.Bo本de本Colo本 = 軍Linea本Colo本(0.3f, 0.3f, 0.3f, 1.0f);
    C使本本entStyle.Bo本de本Radi使s = 8.0f;
    C使本本entStyle.Bo本de本Thickness = 2.0f;
}

正oid UMin成UISyste設置::InitializeUI()
{
    InitializeDefa使ltSc本eens();
    InitializeDefa使ltThe設置es();
    Set使pSc本een輸入isto本y();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("UI Syste設置 initialized with %d sc本eens"), Sc本eenDataMap.的使設置());
}

正oid UMin成UISyste設置::InitializeDefa使ltSc本eens()
{
    // Main Men使 Sc本een
    軍Min成UISc本eenData MainMen使Data;
    MainMen使Data.Sc本eenType = EMin成UISc本een::MainMen使;
    MainMen使Data.Sc本een的a設置e = TEXT("主選單");
    MainMen使Data.Desc本iption = TEXT("遊戲主選單界面");
    MainMen使Data.bIsModal = t本使e;
    MainMen使Data.bDest本oyOnClose = false;
    MainMen使Data.OpenAni設置ation = EMin成UIAni設置ationType::軍ade;
    MainMen使Data.CloseAni設置ation = EMin成UIAni設置ationType::軍ade;
    MainMen使Data.Ani設置ationD使本ation = 0.8f;
    Sc本eenDataMap.Add(MainMen使Data.Sc本eenType, MainMen使Data);
    
    // Ca設置pai成n Map Sc本een
    軍Min成UISc本eenData Ca設置pai成nMapData;
    Ca設置pai成nMapData.Sc本eenType = EMin成UISc本een::Ca設置pai成nMap;
    Ca設置pai成nMapData.Sc本een的a設置e = TEXT("戰役地圖");
    Ca設置pai成nMapData.Desc本iption = TEXT("戰役選擇地圖界面");
    Ca設置pai成nMapData.bIsModal = false;
    Ca設置pai成nMapData.bDest本oyOnClose = t本使e;
    Ca設置pai成nMapData.OpenAni設置ation = EMin成UIAni設置ationType::Slide;
    Ca設置pai成nMapData.CloseAni設置ation = EMin成UIAni設置ationType::Slide;
    Ca設置pai成nMapData.Ani設置ationD使本ation = 0.6f;
    Sc本eenDataMap.Add(Ca設置pai成nMapData.Sc本eenType, Ca設置pai成nMapData);
    
    // Battle 輸入UD Sc本een
    軍Min成UISc本eenData Battle輸入UDData;
    Battle輸入UDData.Sc本eenType = EMin成UISc本een::Battle輸入UD;
    Battle輸入UDData.Sc本een的a設置e = TEXT("戰鬥界面");
    Battle輸入UDData.Desc本iption = TEXT("戰鬥時的輸入UD界面");
    Battle輸入UDData.bIsModal = false;
    Battle輸入UDData.bDest本oyOnClose = t本使e;
    Battle輸入UDData.OpenAni設置ation = EMin成UIAni設置ationType::的one;
    Battle輸入UDData.CloseAni設置ation = EMin成UIAni設置ationType::的one;
    Battle輸入UDData.Ani設置ationD使本ation = 0.0f;
    Sc本eenDataMap.Add(Battle輸入UDData.Sc本eenType, Battle輸入UDData);
    
    // Cha本acte本 Panel Sc本een
    軍Min成UISc本eenData Cha本acte本PanelData;
    Cha本acte本PanelData.Sc本eenType = EMin成UISc本een::Cha本acte本Panel;
    Cha本acte本PanelData.Sc本een的a設置e = TEXT("角色面板");
    Cha本acte本PanelData.Desc本iption = TEXT("角色信息和管理界面");
    Cha本acte本PanelData.bIsModal = false;
    Cha本acte本PanelData.bDest本oyOnClose = t本使e;
    Cha本acte本PanelData.OpenAni設置ation = EMin成UIAni設置ationType::Scale;
    Cha本acte本PanelData.CloseAni設置ation = EMin成UIAni設置ationType::Scale;
    Cha本acte本PanelData.Ani設置ationD使本ation = 0.4f;
    Sc本eenDataMap.Add(Cha本acte本PanelData.Sc本eenType, Cha本acte本PanelData);
    
    // Resea本ch T本ee Sc本een
    軍Min成UISc本eenData Resea本chT本eeData;
    Resea本chT本eeData.Sc本eenType = EMin成UISc本een::Resea本chT本ee;
    Resea本chT本eeData.Sc本een的a設置e = TEXT("科技樹");
    Resea本chT本eeData.Desc本iption = TEXT("科技研發樹界面");
    Resea本chT本eeData.bIsModal = false;
    Resea本chT本eeData.bDest本oyOnClose = t本使e;
    Resea本chT本eeData.OpenAni設置ation = EMin成UIAni設置ationType::軍ade;
    Resea本chT本eeData.CloseAni設置ation = EMin成UIAni設置ationType::軍ade;
    Resea本chT本eeData.Ani設置ationD使本ation = 0.5f;
    Sc本eenDataMap.Add(Resea本chT本eeData.Sc本eenType, Resea本chT本eeData);
    
    // Settin成s Sc本een
    軍Min成UISc本eenData Settin成sData;
    Settin成sData.Sc本eenType = EMin成UISc本een::Settin成s;
    Settin成sData.Sc本een的a設置e = TEXT("設定");
    Settin成sData.Desc本iption = TEXT("遊戲設定界面");
    Settin成sData.bIsModal = t本使e;
    Settin成sData.bDest本oyOnClose = t本使e;
    Settin成sData.OpenAni設置ation = EMin成UIAni設置ationType::Slide;
    Settin成sData.CloseAni設置ation = EMin成UIAni設置ationType::Slide;
    Settin成sData.Ani設置ationD使本ation = 0.3f;
    Sc本eenDataMap.Add(Settin成sData.Sc本eenType, Settin成sData);
    
    // Loadin成 Sc本een
    軍Min成UISc本eenData Loadin成Data;
    Loadin成Data.Sc本eenType = EMin成UISc本een::Loadin成;
    Loadin成Data.Sc本een的a設置e = TEXT("載入中");
    Loadin成Data.Desc本iption = TEXT("載入畫面");
    Loadin成Data.bIsModal = t本使e;
    Loadin成Data.bDest本oyOnClose = t本使e;
    Loadin成Data.OpenAni設置ation = EMin成UIAni設置ationType::軍ade;
    Loadin成Data.CloseAni設置ation = EMin成UIAni設置ationType::軍ade;
    Loadin成Data.Ani設置ationD使本ation = 0.3f;
    Sc本eenDataMap.Add(Loadin成Data.Sc本eenType, Loadin成Data);
    
    // Victo本y Sc本een
    軍Min成UISc本eenData Victo本yData;
    Victo本yData.Sc本eenType = EMin成UISc本een::Victo本y;
    Victo本yData.Sc本een的a設置e = TEXT("勝利");
    Victo本yData.Desc本iption = TEXT("勝利結局界面");
    Victo本yData.bIsModal = t本使e;
    Victo本yData.bDest本oyOnClose = t本使e;
    Victo本yData.OpenAni設置ation = EMin成UIAni設置ationType::Bo使nce;
    Victo本yData.CloseAni設置ation = EMin成UIAni設置ationType::軍ade;
    Victo本yData.Ani設置ationD使本ation = 1.0f;
    Sc本eenDataMap.Add(Victo本yData.Sc本eenType, Victo本yData);
    
    // Defeat Sc本een
    軍Min成UISc本eenData DefeatData;
    DefeatData.Sc本eenType = EMin成UISc本een::Defeat;
    DefeatData.Sc本een的a設置e = TEXT("失敗");
    DefeatData.Desc本iption = TEXT("失敗結局界面");
    DefeatData.bIsModal = t本使e;
    DefeatData.bDest本oyOnClose = t本使e;
    DefeatData.OpenAni設置ation = EMin成UIAni設置ationType::軍ade;
    DefeatData.CloseAni設置ation = EMin成UIAni設置ationType::軍ade;
    DefeatData.Ani設置ationD使本ation = 0.8f;
    Sc本eenDataMap.Add(DefeatData.Sc本eenType, DefeatData);
    
    // Pa使se Sc本een
    軍Min成UISc本eenData Pa使seData;
    Pa使seData.Sc本eenType = EMin成UISc本een::Pa使se;
    Pa使seData.Sc本een的a設置e = TEXT("暫停");
    Pa使seData.Desc本iption = TEXT("遊戲暫停界面");
    Pa使seData.bIsModal = t本使e;
    Pa使seData.bDest本oyOnClose = t本使e;
    Pa使seData.OpenAni設置ation = EMin成UIAni設置ationType::Scale;
    Pa使seData.CloseAni設置ation = EMin成UIAni設置ationType::Scale;
    Pa使seData.Ani設置ationD使本ation = 0.2f;
    Sc本eenDataMap.Add(Pa使seData.Sc本eenType, Pa使seData);
}

正oid UMin成UISyste設置::InitializeDefa使ltThe設置es()
{
    // Rep使blic E本a The設置e (Defa使lt)
    軍Min成UIStyle Rep使blicThe設置e;
    Rep使blicThe設置e.P本i設置a本yColo本 = 軍Linea本Colo本(0.2f, 0.4f, 0.8f, 1.0f); // Bl使e
    Rep使blicThe設置e.Seconda本yColo本 = 軍Linea本Colo本(0.1f, 0.2f, 0.4f, 1.0f);
    Rep使blicThe設置e.AccentColo本 = 軍Linea本Colo本(0.8f, 0.6f, 0.2f, 1.0f); // Gold
    Rep使blicThe設置e.TextColo本 = 軍Linea本Colo本(1.0f, 1.0f, 1.0f, 1.0f);
    Rep使blicThe設置e.Back成本o使ndColo本 = 軍Linea本Colo本(0.05f, 0.05f, 0.1f, 0.9f);
    Rep使blicThe設置e.Bo本de本Colo本 = 軍Linea本Colo本(0.3f, 0.3f, 0.3f, 1.0f);
    The設置eStyles.Add(TEXT("Rep使blic"), Rep使blicThe設置e);
    
    // I設置pe本ial The設置e
    軍Min成UIStyle I設置pe本ialThe設置e;
    I設置pe本ialThe設置e.P本i設置a本yColo本 = 軍Linea本Colo本(0.8f, 0.2f, 0.2f, 1.0f); // Red
    I設置pe本ialThe設置e.Seconda本yColo本 = 軍Linea本Colo本(0.4f, 0.1f, 0.1f, 1.0f);
    I設置pe本ialThe設置e.AccentColo本 = 軍Linea本Colo本(0.9f, 0.7f, 0.3f, 1.0f); // Yellow
    I設置pe本ialThe設置e.TextColo本 = 軍Linea本Colo本(1.0f, 0.95f, 0.8f, 1.0f);
    I設置pe本ialThe設置e.Back成本o使ndColo本 = 軍Linea本Colo本(0.1f, 0.05f, 0.05f, 0.9f);
    I設置pe本ialThe設置e.Bo本de本Colo本 = 軍Linea本Colo本(0.6f, 0.3f, 0.3f, 1.0f);
    The設置eStyles.Add(TEXT("I設置pe本ial"), I設置pe本ialThe設置e);
    
    // Mode本n The設置e
    軍Min成UIStyle Mode本nThe設置e;
    Mode本nThe設置e.P本i設置a本yColo本 = 軍Linea本Colo本(0.1f, 0.8f, 0.4f, 1.0f); // G本een
    Mode本nThe設置e.Seconda本yColo本 = 軍Linea本Colo本(0.05f, 0.4f, 0.2f, 1.0f);
    Mode本nThe設置e.AccentColo本 = 軍Linea本Colo本(0.2f, 0.6f, 0.9f, 1.0f); // Cyan
    Mode本nThe設置e.TextColo本 = 軍Linea本Colo本(0.9f, 0.9f, 0.9f, 1.0f);
    Mode本nThe設置e.Back成本o使ndColo本 = 軍Linea本Colo本(0.02f, 0.02f, 0.02f, 0.95f);
    Mode本nThe設置e.Bo本de本Colo本 = 軍Linea本Colo本(0.2f, 0.2f, 0.2f, 1.0f);
    The設置eStyles.Add(TEXT("Mode本n"), Mode本nThe設置e);
}

正oid UMin成UISyste設置::Set使pSc本een輸入isto本y()
{
    Sc本een輸入isto本y.E設置pty();
    輸入isto本yIndex = -1;
}

正oid UMin成UISyste設置::ShowSc本een(EMin成UISc本een Sc本eenType)
{
    軍Min成UISc本eenData* Sc本eenData = Sc本eenDataMap.軍ind(Sc本eenType);
    if (!Sc本eenData)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Sc本een data not fo使nd fo本 type: %d"), (int32)Sc本eenType);
        本et使本n;
    }
    
    // Check if sc本een is al本eady 正isible
    if (IsSc本eenVisible(Sc本eenType))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Sc本een al本eady 正isible: %s"), *Sc本eenData->Sc本een的a設置e);
        本et使本n;
    }
    
    // 輸入andle 設置odal sc本eens
    if (Sc本eenData->bIsModal)
    {
        // 輸入ide all non-設置odal sc本eens
        fo本 (a使to& Pai本 : Acti正e基本id成ets)
        {
            const 軍Min成UISc本eenData* Existin成Data = Sc本eenDataMap.軍ind(Pai本.Key);
            if (Existin成Data && !Existin成Data->bIsModal)
            {
                輸入ideSc本een(Pai本.Key);
            }
        }
    }
    
    // C本eate and show the wid成et
    UUse本基本id成et* 基本id成et = C本eate基本id成et(Sc本eenType);
    if (基本id成et)
    {
        基本id成et->AddToViewpo本t();
        
        // Play openin成 ani設置ation
        if (Sc本eenData->OpenAni設置ation != EMin成UIAni設置ationType::的one)
        {
            PlayAni設置ationInte本nal(基本id成et, Sc本eenData->OpenAni設置ation, Sc本eenData->Ani設置ationD使本ation);
        }
        
        // Update sc本een stack
        UpdateSc本eenStack(Sc本eenType);
        
        // T本i成成e本 e正ent
        OnSc本eenShown.B本oadcast(Sc本eenType, 基本id成et);
        OnUISc本eenChan成ed.B本oadcast(Sc本eenType);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Sc本een shown: %s"), *Sc本eenData->Sc本een的a設置e);
    }
}

正oid UMin成UISyste設置::輸入ideSc本een(EMin成UISc本een Sc本eenType)
{
    UUse本基本id成et* 基本id成et = Get基本id成et(Sc本eenType);
    if (!基本id成et)
    {
        本et使本n;
    }
    
    軍Min成UISc本eenData* Sc本eenData = Sc本eenDataMap.軍ind(Sc本eenType);
    if (Sc本eenData && Sc本eenData->CloseAni設置ation != EMin成UIAni設置ationType::的one)
    {
        // Play closin成 ani設置ation
        PlayAni設置ationInte本nal(基本id成et, Sc本eenData->CloseAni設置ation, Sc本eenData->Ani設置ationD使本ation);
        
        // Sched使le wid成et 本e設置o正al afte本 ani設置ation
        軍Ti設置e本輸入andle Ti設置e本輸入andle;
        if (基本o本ldContext.IsValid())
        {
            基本o本ldContext->GetTi設置e本Mana成e本().SetTi設置e本(
                Ti設置e本輸入andle,
                [this, Sc本eenType]()
                {
                    CloseSc本een(Sc本eenType);
                },
                Sc本eenData->Ani設置ationD使本ation,
                false
            );
        }
    }
    else
    {
        CloseSc本een(Sc本eenType);
    }
}

正oid UMin成UISyste設置::CloseSc本een(EMin成UISc本een Sc本eenType)
{
    UUse本基本id成et* 基本id成et = Get基本id成et(Sc本eenType);
    if (!基本id成et)
    {
        本et使本n;
    }
    
    // T本i成成e本 e正ent befo本e 本e設置o正al
    OnSc本een輸入idden.B本oadcast(Sc本eenType, 基本id成et);
    
    // Re設置o正e f本o設置 正iewpo本t
    基本id成et->Re設置o正e軍本o設置Pa本ent();
    
    // Clean 使p
    Clean使pAni設置ation(基本id成et);
    
    // Dest本oy if confi成使本ed
    軍Min成UISc本eenData* Sc本eenData = Sc本eenDataMap.軍ind(Sc本eenType);
    if (Sc本eenData && Sc本eenData->bDest本oyOnClose)
    {
        Dest本oy基本id成etInte本nal(Sc本eenType);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sc本een closed: %s"), Sc本eenData 基本 *Sc本eenData->Sc本een的a設置e : TEXT("Unknown"));
}

正oid UMin成UISyste設置::CloseAllSc本eens()
{
    TA本本ay<EMin成UISc本een> Sc本eensToClose;
    fo本 (a使to& Pai本 : Acti正e基本id成ets)
    {
        Sc本eensToClose.Add(Pai本.Key);
    }
    
    fo本 (EMin成UISc本een Sc本eenType : Sc本eensToClose)
    {
        CloseSc本een(Sc本eenType);
    }
}

正oid UMin成UISyste設置::ShowModalSc本een(EMin成UISc本een Sc本eenType)
{
    ShowSc本een(Sc本eenType);
}

bool UMin成UISyste設置::IsSc本eenVisible(EMin成UISc本een Sc本eenType) const
{
    本et使本n Acti正e基本id成ets.Contains(Sc本eenType);
}

UUse本基本id成et* UMin成UISyste設置::GetC使本本entSc本een() const
{
    if (Sc本een輸入isto本y.的使設置() > 0 && 輸入isto本yIndex >= 0 && 輸入isto本yIndex < Sc本een輸入isto本y.的使設置())
    {
        本et使本n Get基本id成et(Sc本een輸入isto本y[輸入isto本yIndex]);
    }
    本et使本n n使llpt本;
}

TA本本ay<UUse本基本id成et*> UMin成UISyste設置::GetVisibleSc本eens() const
{
    TA本本ay<UUse本基本id成et*> Res使lt;
    fo本 (a使to& Pai本 : Acti正e基本id成ets)
    {
        Res使lt.Add(Pai本.Val使e);
    }
    本et使本n Res使lt;
}

正oid UMin成UISyste設置::的a正i成ateToSc本een(EMin成UISc本een Sc本eenType)
{
    AddTo輸入isto本y(Sc本eenType);
    ShowSc本een(Sc本eenType);
}

正oid UMin成UISyste設置::GoBack()
{
    if (!CanGoBack())
    {
        本et使本n;
    }
    
    EMin成UISc本een C使本本entSc本een = GetP本e正io使sSc本een();
    if (C使本本entSc本een != EMin成UISc本een::MainMen使) // Don't 成o back f本o設置 設置ain 設置en使
    {
        輸入ideSc本een(Sc本een輸入isto本y[輸入isto本yIndex]);
        輸入isto本yIndex--;
        ShowSc本een(Sc本een輸入isto本y[輸入isto本yIndex]);
    }
}

正oid UMin成UISyste設置::Go軍o本wa本d()
{
    if (!CanGo軍o本wa本d())
    {
        本et使本n;
    }
    
    輸入ideSc本een(Sc本een輸入isto本y[輸入isto本yIndex]);
    輸入isto本yIndex++;
    ShowSc本een(Sc本een輸入isto本y[輸入isto本yIndex]);
}

bool UMin成UISyste設置::CanGoBack() const
{
    本et使本n 輸入isto本yIndex > 0;
}

bool UMin成UISyste設置::CanGo軍o本wa本d() const
{
    本et使本n 輸入isto本yIndex < Sc本een輸入isto本y.的使設置() - 1;
}

正oid UMin成UISyste設置::PlaySc本eenAni設置ation(EMin成UISc本een Sc本eenType, EMin成UIAni設置ationType Ani設置ationType, float D使本ation)
{
    UUse本基本id成et* 基本id成et = Get基本id成et(Sc本eenType);
    if (基本id成et)
    {
        PlayAni設置ationInte本nal(基本id成et, Ani設置ationType, D使本ation);
    }
}

正oid UMin成UISyste設置::軍adeSc本een(EMin成UISc本een Sc本eenType, float Ta本成etAlpha, float D使本ation)
{
    UUse本基本id成et* 基本id成et = Get基本id成et(Sc本eenType);
    if (基本id成et)
    {
        // This wo使ld i設置ple設置ent fade ani設置ation
        UE下LOG(Lo成Te設置p, Lo成, TEXT("軍adin成 sc本een to alpha %.2f o正e本 %.1f seconds"), Ta本成etAlpha, D使本ation);
    }
}

正oid UMin成UISyste設置::SlideSc本een(EMin成UISc本een Sc本eenType, 軍Vecto本2D Di本ection, float D使本ation)
{
    UUse本基本id成et* 基本id成et = Get基本id成et(Sc本eenType);
    if (基本id成et)
    {
        // This wo使ld i設置ple設置ent slide ani設置ation
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Slidin成 sc本een in di本ection %s o正e本 %.1f seconds"), *Di本ection.ToSt本in成(), D使本ation);
    }
}

正oid UMin成UISyste設置::ScaleSc本een(EMin成UISc本een Sc本eenType, float Scale, float D使本ation)
{
    UUse本基本id成et* 基本id成et = Get基本id成et(Sc本eenType);
    if (基本id成et)
    {
        // This wo使ld i設置ple設置ent scale ani設置ation
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Scalin成 sc本een to %.2f o正e本 %.1f seconds"), Scale, D使本ation);
    }
}

正oid UMin成UISyste設置::SetUIStyle(const 軍Min成UIStyle& 的ewStyle)
{
    C使本本entStyle = 的ewStyle;
    
    // Apply style to all 正isible wid成ets
    fo本 (a使to& Pai本 : Acti正e基本id成ets)
    {
        // This wo使ld apply style to wid成et
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied style to sc本een: %d"), (int32)Pai本.Key);
    }
}

軍Min成UIStyle UMin成UISyste設置::GetC使本本entStyle() const
{
    本et使本n C使本本entStyle;
}

正oid UMin成UISyste設置::ApplyThe設置e(const 軍St本in成& The設置e的a設置e)
{
    軍Min成UIStyle* The設置e = The設置eStyles.軍ind(The設置e的a設置e);
    if (The設置e)
    {
        SetUIStyle(*The設置e);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied the設置e: %s"), *The設置e的a設置e);
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("The設置e not fo使nd: %s"), *The設置e的a設置e);
    }
}

TA本本ay<軍St本in成> UMin成UISyste設置::GetA正ailableThe設置es() const
{
    TA本本ay<軍St本in成> Res使lt;
    fo本 (const a使to& Pai本 : The設置eStyles)
    {
        Res使lt.Add(Pai本.Key);
    }
    本et使本n Res使lt;
}

正oid UMin成UISyste設置::Re成iste本Sc本een(const 軍Min成UISc本eenData& Sc本eenData)
{
    Sc本eenDataMap.Add(Sc本eenData.Sc本eenType, Sc本eenData);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Re成iste本ed sc本een: %s"), *Sc本eenData.Sc本een的a設置e);
}

軍Min成UISc本eenData UMin成UISyste設置::GetSc本eenData(EMin成UISc本een Sc本eenType) const
{
    const 軍Min成UISc本eenData* Data = Sc本eenDataMap.軍ind(Sc本eenType);
    本et使本n Data 基本 *Data : 軍Min成UISc本eenData();
}

TA本本ay<軍Min成UISc本eenData> UMin成UISyste設置::GetAllSc本eenData() const
{
    TA本本ay<軍Min成UISc本eenData> Res使lt;
    fo本 (const a使to& Pai本 : Sc本eenDataMap)
    {
        Res使lt.Add(Pai本.Val使e);
    }
    本et使本n Res使lt;
}

UUse本基本id成et* UMin成UISyste設置::C本eate基本id成et(EMin成UISc本een Sc本eenType)
{
    軍Min成UISc本eenData* Sc本eenData = Sc本eenDataMap.軍ind(Sc本eenType);
    if (!Sc本eenData  !Sc本eenData->基本id成etClass)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot c本eate wid成et - in正alid sc本een data"));
        本et使本n n使llpt本;
    }
    
    本et使本n C本eate基本id成etInte本nal(*Sc本eenData);
}

正oid UMin成UISyste設置::Dest本oy基本id成et(EMin成UISc本een Sc本eenType)
{
    Dest本oy基本id成etInte本nal(Sc本eenType);
}

UUse本基本id成et* UMin成UISyste設置::Get基本id成et(EMin成UISc本een Sc本eenType) const
{
    const UUse本基本id成et* const* 基本id成et = Acti正e基本id成ets.軍ind(Sc本eenType);
    本et使本n 基本id成et 基本 *基本id成et : n使llpt本;
}

軍St本in成 UMin成UISyste設置::GetSc本een的a設置e(EMin成UISc本een Sc本eenType)
{
    switch (Sc本eenType)
    {
    case EMin成UISc本een::MainMen使: 本et使本n TEXT("主選單");
    case EMin成UISc本een::Ca設置pai成nMap: 本et使本n TEXT("戰役地圖");
    case EMin成UISc本een::Battle輸入UD: 本et使本n TEXT("戰鬥界面");
    case EMin成UISc本een::Cha本acte本Panel: 本et使本n TEXT("角色面板");
    case EMin成UISc本een::Resea本chT本ee: 本et使本n TEXT("科技樹");
    case EMin成UISc本een::Settin成s: 本et使本n TEXT("設定");
    case EMin成UISc本een::Loadin成: 本et使本n TEXT("載入中");
    case EMin成UISc本een::Victo本y: 本et使本n TEXT("勝利");
    case EMin成UISc本een::Defeat: 本et使本n TEXT("失敗");
    case EMin成UISc本een::Pa使se: 本et使本n TEXT("暫停");
    defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成UISyste設置::GetAni設置ationType的a設置e(EMin成UIAni設置ationType Ani設置ationType)
{
    switch (Ani設置ationType)
    {
    case EMin成UIAni設置ationType::軍ade: 本et使本n TEXT("淡入淡");
    case EMin成UIAni設置ationType::Slide: 本et使本n TEXT("滑動");
    case EMin成UIAni設置ationType::Scale: 本et使本n TEXT("縮放");
    case EMin成UIAni設置ationType::Rotate: 本et使本n TEXT("旋轉");
    case EMin成UIAni設置ationType::Bo使nce: 本et使本n TEXT("彈跳");
    case EMin成UIAni設置ationType::Elastic: 本et使本n TEXT("彈性");
    defa使lt: 本et使本n TEXT("無");
    }
}

軍St本in成 UMin成UISyste設置::Sa正eUISettin成s() const
{
    軍St本in成 Res使lt = TEXT("{\n");
    Res使lt += TEXT("  \"c使本本ent下the設置e\": \"Rep使blic\",\n");
    Res使lt += 軍St本in成::P本intf(TEXT("  \"sc本een下histo本y下co使nt\": %d,\n"), Sc本een輸入isto本y.的使設置());
    Res使lt += 軍St本in成::P本intf(TEXT("  \"histo本y下index\": %d,\n"), 輸入isto本yIndex);
    Res使lt += 軍St本in成::P本intf(TEXT("  \"acti正e下sc本eens\": %d\n"), Acti正e基本id成ets.的使設置());
    Res使lt += TEXT("}\n");
    
    本et使本n Res使lt;
}

正oid UMin成UISyste設置::LoadUISettin成s(const 軍St本in成& JsonSt本in成)
{
    // Pa本se JSO的 and 本esto本e UI settin成s
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loadin成 UI settin成s"));
}

正oid UMin成UISyste設置::UpdateSc本eenStack(EMin成UISc本een Sc本eenType)
{
    // Re設置o正e any f使t使本e histo本y if we'本e not at the end
    if (輸入isto本yIndex < Sc本een輸入isto本y.的使設置() - 1)
    {
        Sc本een輸入isto本y.Re設置o正eAt(輸入isto本yIndex + 1, Sc本een輸入isto本y.的使設置() - 輸入isto本yIndex - 1);
    }
    
    // Add c使本本ent sc本een to histo本y
    AddTo輸入isto本y(Sc本eenType);
}

正oid UMin成UISyste設置::PlayAni設置ationInte本nal(UUse本基本id成et* 基本id成et, EMin成UIAni設置ationType Ani設置ationType, float D使本ation)
{
    if (!基本id成et)
    {
        本et使本n;
    }
    
    // This wo使ld i設置ple設置ent act使al ani設置ation 使sin成 UMG ani設置ation syste設置
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Playin成 ani設置ation: %s fo本 %.1f seconds"), 
           *GetAni設置ationType的a設置e(Ani設置ationType), D使本ation);
}

正oid UMin成UISyste設置::Clean使pAni設置ation(UUse本基本id成et* 基本id成et)
{
    // Clean 使p ani設置ation 本eso使本ces
    if (基本id成et)
    {
        UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Cleanin成 使p ani設置ation fo本 wid成et"));
    }
}

UUse本基本id成et* UMin成UISyste設置::C本eate基本id成etInte本nal(const 軍Min成UISc本eenData& Sc本eenData)
{
    if (!基本o本ldContext.IsValid())
    {
        本et使本n n使llpt本;
    }
    
    UUse本基本id成et* 基本id成et = C本eate基本id成et<UUse本基本id成et>(基本o本ldContext.Get(), Sc本eenData.基本id成etClass);
    if (基本id成et)
    {
        Acti正e基本id成ets.Add(Sc本eenData.Sc本eenType, 基本id成et);
        Position基本id成et(基本id成et, Sc本eenData.Sc本eenType);
    }
    
    本et使本n 基本id成et;
}

正oid UMin成UISyste設置::Dest本oy基本id成etInte本nal(EMin成UISc本een Sc本eenType)
{
    UUse本基本id成et* 基本id成et = Get基本id成et(Sc本eenType);
    if (基本id成et)
    {
        基本id成et->Re設置o正e軍本o設置Pa本ent();
        基本id成et->Ma本kPendin成Kill();
        Acti正e基本id成ets.Re設置o正e(Sc本eenType);
    }
}

正oid UMin成UISyste設置::Position基本id成et(UUse本基本id成et* 基本id成et, EMin成UISc本een Sc本eenType)
{
    if (!基本id成et)
    {
        本et使本n;
    }
    
    // Set wid成et position and size based on sc本een type
    switch (Sc本eenType)
    {
    case EMin成UISc本een::MainMen使:
    case EMin成UISc本een::Ca設置pai成nMap:
    case EMin成UISc本een::Victo本y:
    case EMin成UISc本een::Defeat:
        // 軍使ll sc本een
        基本id成et->SetDesi本edSizeInViewpo本t(軍Vecto本2D(1920, 1080));
        b本eak;
        
    case EMin成UISc本een::Settin成s:
    case EMin成UISc本een::Pa使se:
        // Cente本ed 設置odal
        基本id成et->SetDesi本edSizeInViewpo本t(軍Vecto本2D(800, 600));
        b本eak;
        
    case EMin成UISc本een::Cha本acte本Panel:
    case EMin成UISc本een::Resea本chT本ee:
        // Side panel
        基本id成et->SetDesi本edSizeInViewpo本t(軍Vecto本2D(400, 800));
        b本eak;
        
    case EMin成UISc本een::Battle輸入UD:
        // 輸入UD o正e本lay
        基本id成et->SetDesi本edSizeInViewpo本t(軍Vecto本2D(1920, 1080));
        b本eak;
        
    defa使lt:
        // Defa使lt size
        基本id成et->SetDesi本edSizeInViewpo本t(軍Vecto本2D(800, 600));
        b本eak;
    }
}

正oid UMin成UISyste設置::AddTo輸入isto本y(EMin成UISc本een Sc本eenType)
{
    // Re設置o正e d使plicates
    Sc本een輸入isto本y.Re設置o正e(Sc本eenType);
    
    // Add to end
    Sc本een輸入isto本y.Add(Sc本eenType);
    輸入isto本yIndex = Sc本een輸入isto本y.的使設置() - 1;
    
    // Li設置it histo本y size
    if (Sc本een輸入isto本y.的使設置() > 20)
    {
        Sc本een輸入isto本y.Re設置o正eAt(0);
        輸入isto本yIndex--;
    }
}

EMin成UISc本een UMin成UISyste設置::GetP本e正io使sSc本een() const
{
    if (輸入isto本yIndex > 0)
    {
        本et使本n Sc本een輸入isto本y[輸入isto本yIndex - 1];
    }
    本et使本n EMin成UISc本een::MainMen使; // Defa使lt to 設置ain 設置en使
}

EMin成UISc本een UMin成UISyste設置::Get的extSc本een() const
{
    if (輸入isto本yIndex < Sc本een輸入isto本y.的使設置() - 1)
    {
        本et使本n Sc本een輸入isto本y[輸入isto本yIndex + 1];
    }
    本et使本n EMin成UISc本een::MainMen使; // Defa使lt to 設置ain 設置en使
}
