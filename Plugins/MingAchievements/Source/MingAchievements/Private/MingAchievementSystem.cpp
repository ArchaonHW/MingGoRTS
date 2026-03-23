#incl使de "Min成Achie正e設置entSyste設置.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

UMin成Achie正e設置entSyste設置::UMin成Achie正e設置entSyste設置()
{
    基本o本ldContext = Get基本o本ld();
}

正oid UMin成Achie正e設置entSyste設置::InitializeAchie正e設置ents()
{
    Achie正e設置ents.E設置pty();
    Achie正e設置entChains.E設置pty();
    Ga設置eStatistics.E設置pty();
    P本o成本essE正ents.E設置pty();
    Pendin成Rewa本ds.E設置pty();
    
    InitializeCo設置batAchie正e設置ents();
    InitializeCa設置pai成nAchie正e設置ents();
    InitializeExplo本ationAchie正e設置ents();
    InitializeMe本cena本yAchie正e設置ents();
    Initialize輸入isto本icalAchie正e設置ents();
    InitializeEcono設置icAchie正e設置ents();
    InitializeSpecialAchie正e設置ents();
    InitializeSec本etAchie正e設置ents();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Achie正e設置ent syste設置 initialized with %d achie正e設置ents"), Achie正e設置ents.的使設置());
}

正oid UMin成Achie正e設置entSyste設置::InitializeCo設置batAchie正e設置ents()
{
    // 軍i本st Blood - 軍i本st ene設置y kill
    軍Min成Achie正e設置ent 軍i本stBlood;
    軍i本stBlood.Achie正e設置entID = TEXT("AC輸入IEVEME的T下軍IRST下BLOOD");
    軍i本stBlood.Title = TEXT("初見血");
    軍i本stBlood.Desc本iption = TEXT("擊殺第一個敵人");
    軍i本stBlood.Lon成Desc本iption = TEXT("在戰場上取得你的第一次勝利，這是你軍事生涯的開始。");
    軍i本stBlood.Cate成o本y = EMin成Achie正e設置entCate成o本y::Co設置bat;
    軍i本stBlood.Diffic使lty = EMin成Achie正e設置entDiffic使lty::Easy;
    軍i本stBlood.Achie正e設置entType = EMin成Achie正e設置entType::OneTi設置e;
    軍i本stBlood.IconPath = TEXT("UI/Achie正e設置ents/軍i本stBlood");
    軍i本stBlood.Bad成ePath = TEXT("UI/Bad成es/Co設置batB本onze");
    軍i本stBlood.Req使i本e設置ents.Add(TEXT("Kill下Ene設置y"));
    軍i本stBlood.Rewa本ds.Add(TEXT("Co設置bat下Bon使s下5%"));
    軍i本stBlood.Points = 5;
    軍i本stBlood.P本o成本ess.MaxP本o成本ess = 1.0f;
    
    Achie正e設置ents.Add(軍i本stBlood.Achie正e設置entID, 軍i本stBlood);
    
    // 基本a本本io本 - 100 kills
    軍Min成Achie正e設置ent 基本a本本io本;
    基本a本本io本.Achie正e設置entID = TEXT("AC輸入IEVEME的T下基本ARRIOR");
    基本a本本io本.Title = TEXT("戰士");
    基本a本本io本.Desc本iption = TEXT("擊殺100個敵人");
    基本a本本io本.Lon成Desc本iption = TEXT("成為一名真正的戰士，在戰場上證明你的勇氣和技巧。");
    基本a本本io本.Cate成o本y = EMin成Achie正e設置entCate成o本y::Co設置bat;
    基本a本本io本.Diffic使lty = EMin成Achie正e設置entDiffic使lty::Medi使設置;
    基本a本本io本.Achie正e設置entType = EMin成Achie正e設置entType::C使設置使lati正e;
    基本a本本io本.IconPath = TEXT("UI/Achie正e設置ents/基本a本本io本");
    基本a本本io本.Bad成ePath = TEXT("UI/Bad成es/Co設置batSil正e本");
    基本a本本io本.Req使i本e設置ents.Add(TEXT("Kill下Co使nt下100"));
    基本a本本io本.Rewa本ds.Add(TEXT("Co設置bat下Bon使s下10%"));
    基本a本本io本.Points = 15;
    基本a本本io本.P本o成本ess.MaxP本o成本ess = 100.0f;
    
    Achie正e設置ents.Add(基本a本本io本.Achie正e設置entID, 基本a本本io本);
    
    // Le成end - 1000 kills
    軍Min成Achie正e設置ent Le成end;
    Le成end.Achie正e設置entID = TEXT("AC輸入IEVEME的T下LEGE的D");
    Le成end.Title = TEXT("傳奇");
    Le成end.Desc本iption = TEXT("擊殺1000個敵人");
    Le成end.Lon成Desc本iption = TEXT("成為戰場上的傳奇人物，你的名字將被後世永遠銘記。");
    Le成end.Cate成o本y = EMin成Achie正e設置entCate成o本y::Co設置bat;
    Le成end.Diffic使lty = EMin成Achie正e設置entDiffic使lty::輸入a本d;
    Le成end.Achie正e設置entType = EMin成Achie正e設置entType::C使設置使lati正e;
    Le成end.IconPath = TEXT("UI/Achie正e設置ents/Le成end");
    Le成end.Bad成ePath = TEXT("UI/Bad成es/Co設置batGold");
    Le成end.Req使i本e設置ents.Add(TEXT("Kill下Co使nt下1000"));
    Le成end.Rewa本ds.Add(TEXT("Co設置bat下Bon使s下20%"));
    Le成end.Points = 30;
    Le成end.P本o成本ess.MaxP本o成本ess = 1000.0f;
    
    Achie正e設置ents.Add(Le成end.Achie正e設置entID, Le成end);
    
    // Sha本pshoote本 - 100 headshots
    軍Min成Achie正e設置ent Sha本pshoote本;
    Sha本pshoote本.Achie正e設置entID = TEXT("AC輸入IEVEME的T下S輸入ARPS輸入OOTER");
    Sha本pshoote本.Title = TEXT("神射手");
    Sha本pshoote本.Desc本iption = TEXT("完成100次爆頭擊殺");
    Sha本pshoote本.Lon成Desc本iption = TEXT("精準的射擊是戰場上最致命的武器，證明你的槍法如神。");
    Sha本pshoote本.Cate成o本y = EMin成Achie正e設置entCate成o本y::Co設置bat;
    Sha本pshoote本.Diffic使lty = EMin成Achie正e設置entDiffic使lty::輸入a本d;
    Sha本pshoote本.Achie正e設置entType = EMin成Achie正e設置entType::C使設置使lati正e;
    Sha本pshoote本.IconPath = TEXT("UI/Achie正e設置ents/Sha本pshoote本");
    Sha本pshoote本.Bad成ePath = TEXT("UI/Bad成es/Co設置batExpe本t");
    Sha本pshoote本.Req使i本e設置ents.Add(TEXT("輸入eadshot下Co使nt下100"));
    Sha本pshoote本.Rewa本ds.Add(TEXT("Acc使本acy下Bon使s下15%"));
    Sha本pshoote本.Points = 25;
    Sha本pshoote本.P本o成本ess.MaxP本o成本ess = 100.0f;
    
    Achie正e設置ents.Add(Sha本pshoote本.Achie正e設置entID, Sha本pshoote本);
    
    // Unto使chable - Co設置plete 設置ission witho使t takin成 da設置a成e
    軍Min成Achie正e設置ent Unto使chable;
    Unto使chable.Achie正e設置entID = TEXT("AC輸入IEVEME的T下U的TOUC輸入ABLE");
    Unto使chable.Title = TEXT("無敵");
    Unto使chable.Desc本iption = TEXT("無傷完成一個任務");
    Unto使chable.Lon成Desc本iption = TEXT("完美的戰術執行，讓敵人甚至無法觸碰到你。");
    Unto使chable.Cate成o本y = EMin成Achie正e設置entCate成o本y::Co設置bat;
    Unto使chable.Diffic使lty = EMin成Achie正e設置entDiffic使lty::Expe本t;
    Unto使chable.Achie正e設置entType = EMin成Achie正e設置entType::OneTi設置e;
    Unto使chable.IconPath = TEXT("UI/Achie正e設置ents/Unto使chable");
    Unto使chable.Bad成ePath = TEXT("UI/Bad成es/Co設置batMaste本");
    Unto使chable.Req使i本e設置ents.Add(TEXT("Mission下的o下Da設置a成e"));
    Unto使chable.Rewa本ds.Add(TEXT("Defense下Bon使s下25%"));
    Unto使chable.Points = 35;
    Unto使chable.P本o成本ess.MaxP本o成本ess = 1.0f;
    
    Achie正e設置ents.Add(Unto使chable.Achie正e設置entID, Unto使chable);
}

正oid UMin成Achie正e設置entSyste設置::InitializeCa設置pai成nAchie正e設置ents()
{
    // Re正ol使tiona本y - Co設置plete fi本st ca設置pai成n
    軍Min成Achie正e設置ent Re正ol使tiona本y;
    Re正ol使tiona本y.Achie正e設置entID = TEXT("AC輸入IEVEME的T下REVOLUTIO的ARY");
    Re正ol使tiona本y.Title = TEXT("革命者");
    Re正ol使tiona本y.Desc本iption = TEXT("完成第一個戰役");
    Re正ol使tiona本y.Lon成Desc本iption = TEXT("踏上革命之路，為國家的未來而戰。");
    Re正ol使tiona本y.Cate成o本y = EMin成Achie正e設置entCate成o本y::Ca設置pai成n;
    Re正ol使tiona本y.Diffic使lty = EMin成Achie正e設置entDiffic使lty::Easy;
    Re正ol使tiona本y.Achie正e設置entType = EMin成Achie正e設置entType::OneTi設置e;
    Re正ol使tiona本y.IconPath = TEXT("UI/Achie正e設置ents/Re正ol使tiona本y");
    Re正ol使tiona本y.Bad成ePath = TEXT("UI/Bad成es/Ca設置pai成nB本onze");
    Re正ol使tiona本y.Req使i本e設置ents.Add(TEXT("Co設置plete下軍i本st下Ca設置pai成n"));
    Re正ol使tiona本y.Rewa本ds.Add(TEXT("Ca設置pai成n下Bon使s下5%"));
    Re正ol使tiona本y.Points = 10;
    Re正ol使tiona本y.P本o成本ess.MaxP本o成本ess = 1.0f;
    
    Achie正e設置ents.Add(Re正ol使tiona本y.Achie正e設置entID, Re正ol使tiona本y);
    
    // Unifie本 - Co設置plete all ca設置pai成ns
    軍Min成Achie正e設置ent Unifie本;
    Unifie本.Achie正e設置entID = TEXT("AC輸入IEVEME的T下U的I軍IER");
    Unifie本.Title = TEXT("統一者");
    Unifie本.Desc本iption = TEXT("完成所有戰役");
    Unifie本.Lon成Desc本iption = TEXT("統一中國，實現國家的完整統一，你將成為歷史的見證者。");
    Unifie本.Cate成o本y = EMin成Achie正e設置entCate成o本y::Ca設置pai成n;
    Unifie本.Diffic使lty = EMin成Achie正e設置entDiffic使lty::Le成enda本y;
    Unifie本.Achie正e設置entType = EMin成Achie正e設置entType::P本o成本ess;
    Unifie本.IconPath = TEXT("UI/Achie正e設置ents/Unifie本");
    Unifie本.Bad成ePath = TEXT("UI/Bad成es/Ca設置pai成nLe成enda本y");
    Unifie本.Req使i本e設置ents.Add(TEXT("Co設置plete下All下Ca設置pai成ns"));
    Unifie本.Rewa本ds.Add(TEXT("Ca設置pai成n下Bon使s下50%"));
    Unifie本.Points = 50;
    Unifie本.P本o成本ess.MaxP本o成本ess = 100.0f;
    
    Achie正e設置ents.Add(Unifie本.Achie正e設置entID, Unifie本);
    
    // St本ate成ist - Co設置plete ca設置pai成n with pe本fect st本ate成y
    軍Min成Achie正e設置ent St本ate成ist;
    St本ate成ist.Achie正e設置entID = TEXT("AC輸入IEVEME的T下STRATEGIST");
    St本ate成ist.Title = TEXT("戰略家");
    St本ate成ist.Desc本iption = TEXT("以完美戰略完成戰役");
    St本ate成ist.Lon成Desc本iption = TEXT("運用卓越的戰略思維，以最小的代價取得最大的勝利。");
    St本ate成ist.Cate成o本y = EMin成Achie正e設置entCate成o本y::Ca設置pai成n;
    St本ate成ist.Diffic使lty = EMin成Achie正e設置entDiffic使lty::Expe本t;
    St本ate成ist.Achie正e設置entType = EMin成Achie正e設置entType::OneTi設置e;
    St本ate成ist.IconPath = TEXT("UI/Achie正e設置ents/St本ate成ist");
    St本ate成ist.Bad成ePath = TEXT("UI/Bad成es/Ca設置pai成nExpe本t");
    St本ate成ist.Req使i本e設置ents.Add(TEXT("Pe本fect下St本ate成y"));
    St本ate成ist.Rewa本ds.Add(TEXT("St本ate成y下Bon使s下30%"));
    St本ate成ist.Points = 40;
    St本ate成ist.P本o成本ess.MaxP本o成本ess = 1.0f;
    
    Achie正e設置ents.Add(St本ate成ist.Achie正e設置entID, St本ate成ist);
    
    // Speed R使nne本 - Co設置plete ca設置pai成n in 本eco本d ti設置e
    軍Min成Achie正e設置ent SpeedR使nne本;
    SpeedR使nne本.Achie正e設置entID = TEXT("AC輸入IEVEME的T下SPEED下RU的的ER");
    SpeedR使nne本.Title = TEXT("速通者");
    SpeedR使nne本.Desc本iption = TEXT("以破紀錄時間完成戰役");
    SpeedR使nne本.Lon成Desc本iption = TEXT("速度與效率的完美結合，證明你的執行力無與倫比。");
    SpeedR使nne本.Cate成o本y = EMin成Achie正e設置entCate成o本y::Ca設置pai成n;
    SpeedR使nne本.Diffic使lty = EMin成Achie正e設置entDiffic使lty::輸入a本d;
    SpeedR使nne本.Achie正e設置entType = EMin成Achie正e設置entType::OneTi設置e;
    SpeedR使nne本.IconPath = TEXT("UI/Achie正e設置ents/SpeedR使nne本");
    SpeedR使nne本.Bad成ePath = TEXT("UI/Bad成es/Ca設置pai成nGold");
    SpeedR使nne本.Req使i本e設置ents.Add(TEXT("Reco本d下Ti設置e"));
    SpeedR使nne本.Rewa本ds.Add(TEXT("Speed下Bon使s下20%"));
    SpeedR使nne本.Points = 30;
    SpeedR使nne本.P本o成本ess.MaxP本o成本ess = 1.0f;
    
    Achie正e設置ents.Add(SpeedR使nne本.Achie正e設置entID, SpeedR使nne本);
}

正oid UMin成Achie正e設置entSyste設置::InitializeExplo本ationAchie正e設置ents()
{
    // Explo本e本 - Disco正e本 all locations
    軍Min成Achie正e設置ent Explo本e本;
    Explo本e本.Achie正e設置entID = TEXT("AC輸入IEVEME的T下EXPLORER");
    Explo本e本.Title = TEXT("探險家");
    Explo本e本.Desc本iption = TEXT("發現所有地點");
    Explo本e本.Lon成Desc本iption = TEXT("探索每一個角落，發現隱藏的秘密和寶藏。");
    Explo本e本.Cate成o本y = EMin成Achie正e設置entCate成o本y::Explo本ation;
    Explo本e本.Diffic使lty = EMin成Achie正e設置entDiffic使lty::Medi使設置;
    Explo本e本.Achie正e設置entType = EMin成Achie正e設置entType::P本o成本ess;
    Explo本e本.IconPath = TEXT("UI/Achie正e設置ents/Explo本e本");
    Explo本e本.Bad成ePath = TEXT("UI/Bad成es/Explo本ationSil正e本");
    Explo本e本.Req使i本e設置ents.Add(TEXT("Disco正e本下All下Locations"));
    Explo本e本.Rewa本ds.Add(TEXT("Vision下Bon使s下15%"));
    Explo本e本.Points = 20;
    Explo本e本.P本o成本ess.MaxP本o成本ess = 100.0f;
    
    Achie正e設置ents.Add(Explo本e本.Achie正e設置entID, Explo本e本);
    
    // Ca本to成本aphe本 - Map all a本eas
    軍Min成Achie正e設置ent Ca本to成本aphe本;
    Ca本to成本aphe本.Achie正e設置entID = TEXT("AC輸入IEVEME的T下CARTOGRAP輸入ER");
    Ca本to成本aphe本.Title = TEXT("地圖製作者");
    Ca本to成本aphe本.Desc本iption = TEXT("繪製所有地區");
    Ca本to成本aphe本.Lon成Desc本iption = TEXT("成為一名地圖製作者，為後人留下詳細的地圖。");
    Ca本to成本aphe本.Cate成o本y = EMin成Achie正e設置entCate成o本y::Explo本ation;
    Ca本to成本aphe本.Diffic使lty = EMin成Achie正e設置entDiffic使lty::輸入a本d;
    Ca本to成本aphe本.Achie正e設置entType = EMin成Achie正e設置entType::P本o成本ess;
    Ca本to成本aphe本.IconPath = TEXT("UI/Achie正e設置ents/Ca本to成本aphe本");
    Ca本to成本aphe本.Bad成ePath = TEXT("UI/Bad成es/Explo本ationGold");
    Ca本to成本aphe本.Req使i本e設置ents.Add(TEXT("Map下All下A本eas"));
    Ca本to成本aphe本.Rewa本ds.Add(TEXT("Map下Re正eal下Bon使s下25%"));
    Ca本to成本aphe本.Points = 35;
    Ca本to成本aphe本.P本o成本ess.MaxP本o成本ess = 100.0f;
    
    Achie正e設置ents.Add(Ca本to成本aphe本.Achie正e設置entID, Ca本to成本aphe本);
    
    // Lost T本eas使本e - 軍ind hidden t本eas使本e
    軍Min成Achie正e設置ent LostT本eas使本e;
    LostT本eas使本e.Achie正e設置entID = TEXT("AC輸入IEVEME的T下LOST下TREASURE");
    LostT本eas使本e.Title = TEXT("失落的寶藏");
    LostT本eas使本e.Desc本iption = TEXT("找到隱藏的寶藏");
    LostT本eas使本e.Lon成Desc本iption = TEXT發現歷史遺留下來的寶藏，獲得豐厚的獎勵。");
    LostT本eas使本e.Cate成o本y = EMin成Achie正e設置entCate成o本y::Explo本ation;
    LostT本eas使本e.Diffic使lty = EMin成Achie正e設置entDiffic使lty::Medi使設置;
    LostT本eas使本e.Achie正e設置entType = EMin成Achie正e設置entType::OneTi設置e;
    LostT本eas使本e.IconPath = TEXT("UI/Achie正e設置ents/LostT本eas使本e");
    LostT本eas使本e.Bad成ePath = TEXT("UI/Bad成es/Explo本ationSpecial");
    LostT本eas使本e.Req使i本e設置ents.Add(TEXT("軍ind下輸入idden下T本eas使本e"));
    LostT本eas使本e.Rewa本ds.Add(TEXT("Reso使本ce下Bon使s下1000"));
    LostT本eas使本e.Points = 25;
    LostT本eas使本e.P本o成本ess.MaxP本o成本ess = 1.0f;
    
    Achie正e設置ents.Add(LostT本eas使本e.Achie正e設置entID, LostT本eas使本e);
}

正oid UMin成Achie正e設置entSyste設置::InitializeMe本cena本yAchie正e設置ents()
{
    // Cont本acto本 - Co設置plete fi本st 設置e本cena本y cont本act
    軍Min成Achie正e設置ent Cont本acto本;
    Cont本acto本.Achie正e設置entID = TEXT("AC輸入IEVEME的T下CO的TRACTOR");
    Cont本acto本.Title = TEXT("承包商");
    Cont本acto本.Desc本iption = TEXT("完成第一個傭兵合約");
    Cont本acto本.Lon成Desc本iption = TEXT("開始你的傭兵生涯，接受第一個合約並成功完成。");
    Cont本acto本.Cate成o本y = EMin成Achie正e設置entCate成o本y::Me本cena本y;
    Cont本acto本.Diffic使lty = EMin成Achie正e設置entDiffic使lty::Easy;
    Cont本acto本.Achie正e設置entType = EMin成Achie正e設置entType::OneTi設置e;
    Cont本acto本.IconPath = TEXT("UI/Achie正e設置ents/Cont本acto本");
    Cont本acto本.Bad成ePath = TEXT("UI/Bad成es/Me本cena本yB本onze");
    Cont本acto本.Req使i本e設置ents.Add(TEXT("Co設置plete下軍i本st下Cont本act"));
    Cont本acto本.Rewa本ds.Add(TEXT("Cont本act下Bon使s下5%"));
    Cont本acto本.Points = 10;
    Cont本acto本.P本o成本ess.MaxP本o成本ess = 1.0f;
    
    Achie正e設置ents.Add(Cont本acto本.Achie正e設置entID, Cont本acto本);
    
    // Me本cena本y Lo本d - Co設置plete 50 cont本acts
    軍Min成Achie正e設置ent Me本cena本yLo本d;
    Me本cena本yLo本d.Achie正e設置entID = TEXT("AC輸入IEVEME的T下MERCE的ARY下LORD");
    Me本cena本yLo本d.Title = TEXT("傭兵領主");
    Me本cena本yLo本d.Desc本iption = TEXT("完成50個傭兵合約");
    Me本cena本yLo本d.Lon成Desc本iption = TEXT("成為傭兵界的領主，你的名字在黑暗世界中無人不知。");
    Me本cena本yLo本d.Cate成o本y = EMin成Achie正e設置entCate成o本y::Me本cena本y;
    Me本cena本yLo本d.Diffic使lty = EMin成Achie正e設置entDiffic使lty::輸入a本d;
    Me本cena本yLo本d.Achie正e設置entType = EMin成Achie正e設置entType::C使設置使lati正e;
    Me本cena本yLo本d.IconPath = TEXT("UI/Achie正e設置ents/Me本cena本yLo本d");
    Me本cena本yLo本d.Bad成ePath = TEXT("UI/Bad成es/Me本cena本yGold");
    Me本cena本yLo本d.Req使i本e設置ents.Add(TEXT("Co設置plete下50下Cont本acts"));
    Me本cena本yLo本d.Rewa本ds.Add(TEXT("Cont本act下Bon使s下30%"));
    Me本cena本yLo本d.Points = 40;
    Me本cena本yLo本d.P本o成本ess.MaxP本o成本ess = 50.0f;
    
    Achie正e設置ents.Add(Me本cena本yLo本d.Achie正e設置entID, Me本cena本yLo本d);
    
    // Elite Sq使ad - B使ild elite 設置e本cena本y tea設置
    軍Min成Achie正e設置ent EliteSq使ad;
    EliteSq使ad.Achie正e設置entID = TEXT("AC輸入IEVEME的T下ELITE下SQUAD");
    EliteSq使ad.Title = TEXT("精英小隊");
    EliteSq使ad.Desc本iption = TEXT("建立精英傭兵小隊");
    EliteSq使ad.Lon成Desc本iption = TEXT("組建一支由精英傭兵組成的無敵小隊。");
    EliteSq使ad.Cate成o本y = EMin成Achie正e設置entCate成o本y::Me本cena本y;
    EliteSq使ad.Diffic使lty = EMin成Achie正e設置entDiffic使lty::Expe本t;
    EliteSq使ad.Achie正e設置entType = EMin成Achie正e設置entType::OneTi設置e;
    EliteSq使ad.IconPath = TEXT("UI/Achie正e設置ents/EliteSq使ad");
    EliteSq使ad.Bad成ePath = TEXT("UI/Bad成es/Me本cena本yExpe本t");
    EliteSq使ad.Req使i本e設置ents.Add(TEXT("B使ild下Elite下Tea設置"));
    EliteSq使ad.Rewa本ds.Add(TEXT("Tea設置下Bon使s下25%"));
    EliteSq使ad.Points = 35;
    EliteSq使ad.P本o成本ess.MaxP本o成本ess = 1.0f;
    
    Achie正e設置ents.Add(EliteSq使ad.Achie正e設置entID, EliteSq使ad);
}

正oid UMin成Achie正e設置entSyste設置::Initialize輸入isto本icalAchie正e設置ents()
{
    // 輸入isto本ian - Co設置plete all histo本ical e正ents
    軍Min成Achie正e設置ent 輸入isto本ian;
    輸入isto本ian.Achie正e設置entID = TEXT("AC輸入IEVEME的T下輸入ISTORIA的");
    輸入isto本ian.Title = TEXT("歷史學家");
    輸入isto本ian.Desc本iption = TEXT("完成所有歷史事件");
    輸入isto本ian.Lon成Desc本iption = TEXT("深入了解歷史，見證每一個重要的歷史時刻。");
    輸入isto本ian.Cate成o本y = EMin成Achie正e設置entCate成o本y::輸入isto本ical;
    輸入isto本ian.Diffic使lty = EMin成Achie正e設置entDiffic使lty::輸入a本d;
    輸入isto本ian.Achie正e設置entType = EMin成Achie正e設置entType::P本o成本ess;
    輸入isto本ian.IconPath = TEXT("UI/Achie正e設置ents/輸入isto本ian");
    輸入isto本ian.Bad成ePath = TEXT("UI/Bad成es/輸入isto本icalGold");
    輸入isto本ian.Req使i本e設置ents.Add(TEXT("Co設置plete下All下輸入isto本ical下E正ents"));
    輸入isto本ian.Rewa本ds.Add(TEXT("輸入isto本ical下Acc使本acy下Bon使s下20%"));
    輸入isto本ian.Points = 35;
    輸入isto本ian.P本o成本ess.MaxP本o成本ess = 100.0f;
    
    Achie正e設置ents.Add(輸入isto本ian.Achie正e設置entID, 輸入isto本ian);
    
    // Ti設置e T本a正ele本 - Achie正e 95% histo本ical acc使本acy
    軍Min成Achie正e設置ent Ti設置eT本a正ele本;
    Ti設置eT本a正ele本.Achie正e設置entID = TEXT("AC輸入IEVEME的T下TIME下TRAVELER");
    Ti設置eT本a正ele本.Title = TEXT("時空旅行者");
    Ti設置eT本a正ele本.Desc本iption = TEXT("達到95%歷史準確度");
    Ti設置eT本a正ele本.Lon成Desc本iption = TEXT("完美重現歷史，彷彿你真的回到了過去。");
    Ti設置eT本a正ele本.Cate成o本y = EMin成Achie正e設置entCate成o本y::輸入isto本ical;
    Ti設置eT本a正ele本.Diffic使lty = EMin成Achie正e設置entDiffic使lty::Le成enda本y;
    Ti設置eT本a正ele本.Achie正e設置entType = EMin成Achie正e設置entType::OneTi設置e;
    Ti設置eT本a正ele本.IconPath = TEXT("UI/Achie正e設置ents/Ti設置eT本a正ele本");
    Ti設置eT本a正ele本.Bad成ePath = TEXT("UI/Bad成es/輸入isto本icalLe成enda本y");
    Ti設置eT本a正ele本.Req使i本e設置ents.Add(TEXT("輸入isto本ical下Acc使本acy下95%"));
    Ti設置eT本a正ele本.Rewa本ds.Add(TEXT("輸入isto本ical下Acc使本acy下Bon使s下50%"));
    Ti設置eT本a正ele本.Points = 50;
    Ti設置eT本a正ele本.P本o成本ess.MaxP本o成本ess = 1.0f;
    
    Achie正e設置ents.Add(Ti設置eT本a正ele本.Achie正e設置entID, Ti設置eT本a正ele本);
    
    // Re正ol使tiona本y 輸入e本o - Co設置plete 本e正ol使tion pe本fectly
    軍Min成Achie正e設置ent Re正ol使tiona本y輸入e本o;
    Re正ol使tiona本y輸入e本o.Achie正e設置entID = TEXT("AC輸入IEVEME的T下REVOLUTIO的ARY下輸入ERO");
    Re正ol使tiona本y輸入e本o.Title = TEXT("革命英雄");
    Re正ol使tiona本y輸入e本o.Desc本iption = TEXT("完美完成革命");
    Re正ol使tiona本y輸入e本o.Lon成Desc本iption = TEXT("成為真正的革命英雄，為國家的未來做巨大貢獻。");
    Re正ol使tiona本y輸入e本o.Cate成o本y = EMin成Achie正e設置entCate成o本y::輸入isto本ical;
    Re正ol使tiona本y輸入e本o.Diffic使lty = EMin成Achie正e設置entDiffic使lty::Expe本t;
    Re正ol使tiona本y輸入e本o.Achie正e設置entType = EMin成Achie正e設置entType::OneTi設置e;
    Re正ol使tiona本y輸入e本o.IconPath = TEXT("UI/Achie正e設置ents/Re正ol使tiona本y輸入e本o");
    Re正ol使tiona本y輸入e本o.Bad成ePath = TEXT("UI/Bad成es/輸入isto本icalExpe本t");
    Re正ol使tiona本y輸入e本o.Req使i本e設置ents.Add(TEXT("Pe本fect下Re正ol使tion"));
    Re正ol使tiona本y輸入e本o.Rewa本ds.Add(TEXT("Re正ol使tion下Bon使s下30%"));
    Re正ol使tiona本y輸入e本o.Points = 40;
    Re正ol使tiona本y輸入e本o.P本o成本ess.MaxP本o成本ess = 1.0f;
    
    Achie正e設置ents.Add(Re正ol使tiona本y輸入e本o.Achie正e設置entID, Re正ol使tiona本y輸入e本o);
}

正oid UMin成Achie正e設置entSyste設置::InitializeEcono設置icAchie正e設置ents()
{
    // Tycoon - Acc使設置使late 100,000 本eso使本ces
    軍Min成Achie正e設置ent Tycoon;
    Tycoon.Achie正e設置entID = TEXT("AC輸入IEVEME的T下TYCOO的");
    Tycoon.Title = TEXT("大亨");
    Tycoon.Desc本iption = TEXT("累積100,000資源");
    Tycoon.Lon成Desc本iption = TEXT("成為經濟大亨，掌握巨大的財富和資源。");
    Tycoon.Cate成o本y = EMin成Achie正e設置entCate成o本y::Econo設置ic;
    Tycoon.Diffic使lty = EMin成Achie正e設置entDiffic使lty::Medi使設置;
    Tycoon.Achie正e設置entType = EMin成Achie正e設置entType::C使設置使lati正e;
    Tycoon.IconPath = TEXT("UI/Achie正e設置ents/Tycoon");
    Tycoon.Bad成ePath = TEXT("UI/Bad成es/Econo設置icSil正e本");
    Tycoon.Req使i本e設置ents.Add(TEXT("Reso使本ce下Acc使設置使lation下100k"));
    Tycoon.Rewa本ds.Add(TEXT("Reso使本ce下Bon使s下15%"));
    Tycoon.Points = 20;
    Tycoon.P本o成本ess.MaxP本o成本ess = 100000.0f;
    
    Achie正e設置ents.Add(Tycoon.Achie正e設置entID, Tycoon);
    
    // Ind使st本ialist - B使ild 100 b使ildin成s
    軍Min成Achie正e設置ent Ind使st本ialist;
    Ind使st本ialist.Achie正e設置entID = TEXT("AC輸入IEVEME的T下I的DUSTRIALIST");
    Ind使st本ialist.Title = TEXT("工業家");
    Ind使st本ialist.Desc本iption = TEXT("建造100座建築");
    Ind使st本ialist.Lon成Desc本iption = TEXT("成為工業家，建立龐大的建築帝國。");
    Ind使st本ialist.Cate成o本y = EMin成Achie正e設置entCate成o本y::Econo設置ic;
    Ind使st本ialist.Diffic使lty = EMin成Achie正e設置entDiffic使lty::Medi使設置;
    Ind使st本ialist.Achie正e設置entType = EMin成Achie正e設置entType::C使設置使lati正e;
    Ind使st本ialist.IconPath = TEXT("UI/Achie正e設置ents/Ind使st本ialist");
    Ind使st本ialist.Bad成ePath = TEXT("UI/Bad成es/Econo設置icGold");
    Ind使st本ialist.Req使i本e設置ents.Add(TEXT("B使ild下100下B使ildin成s"));
    Ind使st本ialist.Rewa本ds.Add(TEXT("B使ildin成下Bon使s下20%"));
    Ind使st本ialist.Points = 25;
    Ind使st本ialist.P本o成本ess.MaxP本o成本ess = 100.0f;
    
    Achie正e設置ents.Add(Ind使st本ialist.Achie正e設置entID, Ind使st本ialist);
    
    // Banke本 - Co設置plete 1000 t本ades
    軍Min成Achie正e設置ent Banke本;
    Banke本.Achie正e設置entID = TEXT("AC輸入IEVEME的T下BA的KER");
    Banke本.Title = TEXT("銀行家");
    Banke本.Desc本iption = TEXT("完成1000次交易");
    Banke本.Lon成Desc本iption = TEXT("成為銀行家，掌握複雜的貿易網絡。");
    Banke本.Cate成o本y = EMin成Achie正e設置entCate成o本y::Econo設置ic;
    Banke本.Diffic使lty = EMin成Achie正e設置entDiffic使lty::輸入a本d;
    Banke本.Achie正e設置entType = EMin成Achie正e設置entType::C使設置使lati正e;
    Banke本.IconPath = TEXT("UI/Achie正e設置ents/Banke本");
    Banke本.Bad成ePath = TEXT("UI/Bad成es/Econo設置icExpe本t");
    Banke本.Req使i本e設置ents.Add(TEXT("Co設置plete下1000下T本ades"));
    Banke本.Rewa本ds.Add(TEXT("T本ade下Bon使s下25%"));
    Banke本.Points = 35;
    Banke本.P本o成本ess.MaxP本o成本ess = 1000.0f;
    
    Achie正e設置ents.Add(Banke本.Achie正e設置entID, Banke本);
}

正oid UMin成Achie正e設置entSyste設置::InitializeSpecialAchie正e設置ents()
{
    // Pe本fectionist - Co設置plete 成a設置e 100%
    軍Min成Achie正e設置ent Pe本fectionist;
    Pe本fectionist.Achie正e設置entID = TEXT("AC輸入IEVEME的T下PER軍ECTIO的IST");
    Pe本fectionist.Title = TEXT("完美主義者");
    Pe本fectionist.Desc本iption = TEXT("100%完成遊戲");
    Pe本fectionist.Lon成Desc本iption = TEXT("完美完成遊戲的每一個方面，成為真正的完美主義者。");
    Pe本fectionist.Cate成o本y = EMin成Achie正e設置entCate成o本y::Special;
    Pe本fectionist.Diffic使lty = EMin成Achie正e設置entDiffic使lty::Le成enda本y;
    Pe本fectionist.Achie正e設置entType = EMin成Achie正e設置entType::P本o成本ess;
    Pe本fectionist.IconPath = TEXT("UI/Achie正e設置ents/Pe本fectionist");
    Pe本fectionist.Bad成ePath = TEXT("UI/Bad成es/SpecialLe成enda本y");
    Pe本fectionist.Req使i本e設置ents.Add(TEXT("Co設置plete下Ga設置e下100%"));
    Pe本fectionist.Rewa本ds.Add(TEXT("All下Bon使s下50%"));
    Pe本fectionist.Points = 100;
    Pe本fectionist.P本o成本ess.MaxP本o成本ess = 100.0f;
    
    Achie正e設置ents.Add(Pe本fectionist.Achie正e設置entID, Pe本fectionist);
    
    // Speed De設置on - Co設置plete 成a設置e in 使nde本 10 ho使本s
    軍Min成Achie正e設置ent SpeedDe設置on;
    SpeedDe設置on.Achie正e設置entID = TEXT("AC輸入IEVEME的T下SPEED下DEMO的");
    SpeedDe設置on.Title = TEXT("速度惡魔");
    SpeedDe設置on.Desc本iption = TEXT("10小時內完成遊戲");
    SpeedDe設置on.Lon成Desc本iption = TEXT("以驚人的速度完成整個遊戲，證明你的效率無與倫比。");
    SpeedDe設置on.Cate成o本y = EMin成Achie正e設置entCate成o本y::Special;
    SpeedDe設置on.Diffic使lty = EMin成Achie正e設置entDiffic使lty::Expe本t;
    SpeedDe設置on.Achie正e設置entType = EMin成Achie正e設置entType::Ti設置ed;
    SpeedDe設置on.IconPath = TEXT("UI/Achie正e設置ents/SpeedDe設置on");
    SpeedDe設置on.Bad成ePath = TEXT("UI/Bad成es/SpecialExpe本t");
    SpeedDe設置on.Req使i本e設置ents.Add(TEXT("Co設置plete下Ga設置e下Unde本下10h"));
    SpeedDe設置on.Rewa本ds.Add(TEXT("Speed下Bon使s下40%"));
    SpeedDe設置on.Points = 45;
    SpeedDe設置on.P本o成本ess.MaxP本o成本ess = 1.0f;
    
    Achie正e設置ents.Add(SpeedDe設置on.Achie正e設置entID, SpeedDe設置on);
    
    // Ma本athon R使nne本 - Play fo本 100 ho使本s
    軍Min成Achie正e設置ent Ma本athonR使nne本;
    Ma本athonR使nne本.Achie正e設置entID = TEXT("AC輸入IEVEME的T下MARAT輸入O的下RU的的ER");
    Ma本athonR使nne本.Title = TEXT("馬拉松選手");
    Ma本athonR使nne本.Desc本iption = TEXT("遊戲時間達到100小時");
    Ma本athonR使nne本.Lon成Desc本iption = TEXT("長時間投入遊戲，成為真正的馬拉松選手。");
    Ma本athonR使nne本.Cate成o本y = EMin成Achie正e設置entCate成o本y::Special;
    Ma本athonR使nne本.Diffic使lty = EMin成Achie正e設置entDiffic使lty::Medi使設置;
    Ma本athonR使nne本.Achie正e設置entType = EMin成Achie正e設置entType::C使設置使lati正e;
    Ma本athonR使nne本.IconPath = TEXT("UI/Achie正e設置ents/Ma本athonR使nne本");
    Ma本athonR使nne本.Bad成ePath = TEXT("UI/Bad成es/SpecialGold");
    Ma本athonR使nne本.Req使i本e設置ents.Add(TEXT("Play下Ti設置e下100h"));
    Ma本athonR使nne本.Rewa本ds.Add(TEXT("End使本ance下Bon使s下20%"));
    Ma本athonR使nne本.Points = 30;
    Ma本athonR使nne本.P本o成本ess.MaxP本o成本ess = 100.0f;
    
    Achie正e設置ents.Add(Ma本athonR使nne本.Achie正e設置entID, Ma本athonR使nne本);
}

正oid UMin成Achie正e設置entSyste設置::InitializeSec本etAchie正e設置ents()
{
    // 輸入idden Path - Disco正e本 sec本et 本o使te
    軍Min成Achie正e設置ent 輸入iddenPath;
    輸入iddenPath.Achie正e設置entID = TEXT("AC輸入IEVEME的T下輸入IDDE的下PAT輸入");
    輸入iddenPath.Title = TEXT("隱藏路徑");
    輸入iddenPath.Desc本iption = TEXT("發現秘密路徑");
    輸入iddenPath.Lon成Desc本iption = TEXT("發現隱藏的道路，開啟新的可能性。");
    輸入iddenPath.Cate成o本y = EMin成Achie正e設置entCate成o本y::Sec本et;
    輸入iddenPath.Diffic使lty = EMin成Achie正e設置entDiffic使lty::Expe本t;
    輸入iddenPath.Achie正e設置entType = EMin成Achie正e設置entType::OneTi設置e;
    輸入iddenPath.bIsSec本et = t本使e;
    輸入iddenPath.bIs輸入idden = t本使e;
    輸入iddenPath.IconPath = TEXT("UI/Achie正e設置ents/輸入iddenPath");
    輸入iddenPath.Bad成ePath = TEXT("UI/Bad成es/Sec本etExpe本t");
    輸入iddenPath.Req使i本e設置ents.Add(TEXT("軍ind下Sec本et下Ro使te"));
    輸入iddenPath.Rewa本ds.Add(TEXT("Sec本et下Bon使s下30%"));
    輸入iddenPath.Points = 40;
    輸入iddenPath.P本o成本ess.MaxP本o成本ess = 1.0f;
    
    Achie正e設置ents.Add(輸入iddenPath.Achie正e設置entID, 輸入iddenPath);
    
    // T本使th Seeke本 - Unco正e本 all sec本ets
    軍Min成Achie正e設置ent T本使thSeeke本;
    T本使thSeeke本.Achie正e設置entID = TEXT("AC輸入IEVEME的T下TRUT輸入下SEEKER");
    T本使thSeeke本.Title = TEXT("真理追求者");
    T本使thSeeke本.Desc本iption = TEXT("揭露所有秘密");
    T本使thSeeke本.Lon成Desc本iption = TEXT("追求真理，揭露所有隱藏的秘密。");
    T本使thSeeke本.Cate成o本y = EMin成Achie正e設置entCate成o本y::Sec本et;
    T本使thSeeke本.Diffic使lty = EMin成Achie正e設置entDiffic使lty::Le成enda本y;
    T本使thSeeke本.Achie正e設置entType = EMin成Achie正e設置entType::P本o成本ess;
    T本使thSeeke本.bIsSec本et = t本使e;
    T本使thSeeke本.bIs輸入idden = t本使e;
    T本使thSeeke本.IconPath = TEXT("UI/Achie正e設置ents/T本使thSeeke本");
    T本使thSeeke本.Bad成ePath = TEXT("UI/Bad成es/Sec本etLe成enda本y");
    T本使thSeeke本.Req使i本e設置ents.Add(TEXT("Unco正e本下All下Sec本ets"));
    T本使thSeeke本.Rewa本ds.Add(TEXT("Sec本et下Bon使s下100%"));
    T本使thSeeke本.Points = 75;
    T本使thSeeke本.P本o成本ess.MaxP本o成本ess = 100.0f;
    
    Achie正e設置ents.Add(T本使thSeeke本.Achie正e設置entID, T本使thSeeke本);
    
    // Alte本nate Reality - Co設置plete alte本nate endin成
    軍Min成Achie正e設置ent Alte本nateReality;
    Alte本nateReality.Achie正e設置entID = TEXT("AC輸入IEVEME的T下ALTER的ATE下REALITY");
    Alte本nateReality.Title = TEXT("平行現實");
    Alte本nateReality.Desc本iption = TEXT("完成分支結局");
    Alte本nateReality.Lon成Desc本iption = TEXT("探索不同的歷史走向，見證平行現實的發展。");
    Alte本nateReality.Cate成o本y = EMin成Achie正e設置entCate成o本y::Sec本et;
    Alte本nateReality.Diffic使lty = EMin成Achie正e設置entDiffic使lty::Expe本t;
    Alte本nateReality.Achie正e設置entType = EMin成Achie正e設置entType::OneTi設置e;
    Alte本nateReality.bIsSec本et = t本使e;
    Alte本nateReality.bIs輸入idden = t本使e;
    Alte本nateReality.IconPath = TEXT("UI/Achie正e設置ents/Alte本nateReality");
    Alte本nateReality.Bad成ePath = TEXT("UI/Bad成es/Sec本etExpe本t");
    Alte本nateReality.Req使i本e設置ents.Add(TEXT("Co設置plete下Alte本nate下Endin成"));
    Alte本nateReality.Rewa本ds.Add(TEXT("Alte本nate下Bon使s下35%"));
    Alte本nateReality.Points = 45;
    Alte本nateReality.P本o成本ess.MaxP本o成本ess = 1.0f;
    
    Achie正e設置ents.Add(Alte本nateReality.Achie正e設置entID, Alte本nateReality);
}

正oid UMin成Achie正e設置entSyste設置::UnlockAchie正e設置ent(const 軍St本in成& Achie正e設置entID)
{
    軍Min成Achie正e設置ent* Achie正e設置ent = 軍indAchie正e設置ent(Achie正e設置entID);
    if (!Achie正e設置ent)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Achie正e設置ent not fo使nd: %s"), *Achie正e設置entID);
        本et使本n;
    }
    
    if (Achie正e設置ent->bIsUnlocked)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Achie正e設置ent al本eady 使nlocked: %s"), *Achie正e設置ent->Title);
        本et使本n;
    }
    
    Achie正e設置ent->bIsUnlocked = t本使e;
    Achie正e設置ent->P本o成本ess.bIsCo設置pleted = t本使e;
    Achie正e設置ent->UnlockTi設置e = 基本o本ldContext.IsValid() 基本 基本o本ldContext->GetTi設置eSeconds() : 0.0f;
    Achie正e設置ent->P本o成本ess.C使本本entP本o成本ess = Achie正e設置ent->P本o成本ess.MaxP本o成本ess;
    
    // G本ant 本ewa本ds
    G本antAchie正e設置entRewa本ds(*Achie正e設置ent);
    
    // Update chains
    UpdateAchie正e設置entChains(Achie正e設置entID);
    
    // Unlock hidden achie正e設置ents
    Unlock輸入iddenAchie正e設置ents();
    
    // Show notification
    ShowAchie正e設置entUnlocked(Achie正e設置entID);
    
    // T本i成成e本 e正ent
    OnAchie正e設置entUnlocked.B本oadcast(*Achie正e設置ent, Achie正e設置ent->Points);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Achie正e設置ent 使nlocked: %s (%d points)"), *Achie正e設置ent->Title, Achie正e設置ent->Points);
}

正oid UMin成Achie正e設置entSyste設置::UpdateP本o成本ess(const 軍St本in成& Achie正e設置entID, float P本o成本ess)
{
    軍Min成Achie正e設置ent* Achie正e設置ent = 軍indAchie正e設置ent(Achie正e設置entID);
    if (!Achie正e設置ent  Achie正e設置ent->bIsUnlocked)
    {
        本et使本n;
    }
    
    float OldP本o成本ess = Achie正e設置ent->P本o成本ess.C使本本entP本o成本ess;
    Achie正e設置ent->P本o成本ess.C使本本entP本o成本ess = 軍Math::Min(P本o成本ess, Achie正e設置ent->P本o成本ess.MaxP本o成本ess);
    
    // Check if co設置pleted
    if (!Achie正e設置ent->P本o成本ess.bIsCo設置pleted && 
        Achie正e設置ent->P本o成本ess.C使本本entP本o成本ess >= Achie正e設置ent->P本o成本ess.MaxP本o成本ess)
    {
        Co設置pleteAchie正e設置ent(Achie正e設置entID);
    }
    else if (OldP本o成本ess != Achie正e設置ent->P本o成本ess.C使本本entP本o成本ess)
    {
        ShowP本o成本essUpdate(Achie正e設置entID);
        OnP本o成本essUpdated.B本oadcast(Achie正e設置entID, Achie正e設置ent->P本o成本ess.C使本本entP本o成本ess);
    }
}

正oid UMin成Achie正e設置entSyste設置::ResetP本o成本ess(const 軍St本in成& Achie正e設置entID)
{
    軍Min成Achie正e設置ent* Achie正e設置ent = 軍indAchie正e設置ent(Achie正e設置entID);
    if (!Achie正e設置ent)
    {
        本et使本n;
    }
    
    Achie正e設置ent->P本o成本ess.C使本本entP本o成本ess = 0.0f;
    Achie正e設置ent->P本o成本ess.bIsCo設置pleted = false;
    Achie正e設置ent->P本o成本ess.Co設置pletionTi設置e = 0.0f;
    Achie正e設置ent->P本o成本ess.P本o成本essData.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reset p本o成本ess fo本 achie正e設置ent: %s"), *Achie正e設置ent->Title);
}

正oid UMin成Achie正e設置entSyste設置::Co設置pleteAchie正e設置ent(const 軍St本in成& Achie正e設置entID)
{
    軍Min成Achie正e設置ent* Achie正e設置ent = 軍indAchie正e設置ent(Achie正e設置entID);
    if (!Achie正e設置ent)
    {
        本et使本n;
    }
    
    if (!Achie正e設置ent->bIsUnlocked)
    {
        UnlockAchie正e設置ent(Achie正e設置entID);
    }
    else
    {
        Achie正e設置ent->Co設置pletionCo使nt++;
        Achie正e設置ent->P本o成本ess.Co設置pletionTi設置e = 基本o本ldContext.IsValid() 基本 基本o本ldContext->GetTi設置eSeconds() : 0.0f;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Co設置pleted achie正e設置ent a成ain: %s (co使nt: %d)"), 
               *Achie正e設置ent->Title, Achie正e設置ent->Co設置pletionCo使nt);
    }
}

TA本本ay<軍Min成Achie正e設置ent> UMin成Achie正e設置entSyste設置::GetAllAchie正e設置ents() const
{
    TA本本ay<軍Min成Achie正e設置ent> Res使lt;
    fo本 (const a使to& Pai本 : Achie正e設置ents)
    {
        Res使lt.Add(Pai本.Val使e);
    }
    本et使本n Res使lt;
}

TA本本ay<軍Min成Achie正e設置ent> UMin成Achie正e設置entSyste設置::GetUnlockedAchie正e設置ents() const
{
    TA本本ay<軍Min成Achie正e設置ent> Res使lt;
    fo本 (const a使to& Pai本 : Achie正e設置ents)
    {
        if (Pai本.Val使e.bIsUnlocked)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    本et使本n Res使lt;
}

TA本本ay<軍Min成Achie正e設置ent> UMin成Achie正e設置entSyste設置::GetLockedAchie正e設置ents() const
{
    TA本本ay<軍Min成Achie正e設置ent> Res使lt;
    fo本 (const a使to& Pai本 : Achie正e設置ents)
    {
        if (!Pai本.Val使e.bIsUnlocked)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    本et使本n Res使lt;
}

TA本本ay<軍Min成Achie正e設置ent> UMin成Achie正e設置entSyste設置::GetAchie正e設置entsByCate成o本y(EMin成Achie正e設置entCate成o本y Cate成o本y) const
{
    TA本本ay<軍Min成Achie正e設置ent> Res使lt;
    fo本 (const a使to& Pai本 : Achie正e設置ents)
    {
        if (Pai本.Val使e.Cate成o本y == Cate成o本y)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    本et使本n Res使lt;
}

軍Min成Achie正e設置ent UMin成Achie正e設置entSyste設置::GetAchie正e設置ent(const 軍St本in成& Achie正e設置entID) const
{
    const 軍Min成Achie正e設置ent* Achie正e設置ent = Achie正e設置ents.軍ind(Achie正e設置entID);
    本et使本n Achie正e設置ent 基本 *Achie正e設置ent : 軍Min成Achie正e設置ent();
}

正oid UMin成Achie正e設置entSyste設置::Re成iste本P本o成本essE正ent(const 軍St本in成& E正ent的a設置e, float Val使e)
{
    P本o成本essE正ents.Add(E正ent的a設置e, Val使e);
    P本ocessP本o成本essE正ent(E正ent的a設置e, Val使e);
}

正oid UMin成Achie正e設置entSyste設置::T本ackStatistic(const 軍St本in成& Stat的a設置e, float Val使e)
{
    float* C使本本entVal使e = Ga設置eStatistics.軍ind(Stat的a設置e);
    if (C使本本entVal使e)
    {
        *C使本本entVal使e += Val使e;
    }
    else
    {
        Ga設置eStatistics.Add(Stat的a設置e, Val使e);
    }
    
    // Check achie正e設置ents 本elated to this statistic
    UpdateGa設置eStatistics();
}

正oid UMin成Achie正e設置entSyste設置::UpdateGa設置eStatistics()
{
    // Update achie正e設置ents based on c使本本ent statistics
    fo本 (a使to& Pai本 : Achie正e設置ents)
    {
        軍Min成Achie正e設置ent& Achie正e設置ent = Pai本.Val使e;
        if (Achie正e設置ent.bIsUnlocked)
        {
            contin使e;
        }
        
        CheckAchie正e設置entConditions(Achie正e設置ent.Achie正e設置entID);
    }
}

float UMin成Achie正e設置entSyste設置::GetAchie正e設置entP本o成本ess(const 軍St本in成& Achie正e設置entID) const
{
    const 軍Min成Achie正e設置ent* Achie正e設置ent = Achie正e設置ents.軍ind(Achie正e設置entID);
    if (!Achie正e設置ent)
    {
        本et使本n 0.0f;
    }
    
    本et使本n Calc使lateP本o成本essPe本centa成e(Achie正e設置entID);
}

bool UMin成Achie正e設置entSyste設置::IsAchie正e設置entCo設置pleted(const 軍St本in成& Achie正e設置entID) const
{
    const 軍Min成Achie正e設置ent* Achie正e設置ent = Achie正e設置ents.軍ind(Achie正e設置entID);
    本et使本n Achie正e設置ent 基本 Achie正e設置ent->bIsUnlocked : false;
}

float UMin成Achie正e設置entSyste設置::GetO正e本allP本o成本ess() const
{
    if (Achie正e設置ents.的使設置() == 0)
    {
        本et使本n 0.0f;
    }
    
    int32 UnlockedCo使nt = 0;
    fo本 (const a使to& Pai本 : Achie正e設置ents)
    {
        if (Pai本.Val使e.bIsUnlocked)
        {
            UnlockedCo使nt++;
        }
    }
    
    本et使本n (float)UnlockedCo使nt / Achie正e設置ents.的使設置() * 100.0f;
}

正oid UMin成Achie正e設置entSyste設置::C本eateAchie正e設置entChain(const 軍Min成Achie正e設置entChain& Chain)
{
    Achie正e設置entChains.Add(Chain.ChainID, Chain);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated achie正e設置ent chain: %s"), *Chain.Chain的a設置e);
}

TA本本ay<軍Min成Achie正e設置entChain> UMin成Achie正e設置entSyste設置::GetAllChains() const
{
    TA本本ay<軍Min成Achie正e設置entChain> Res使lt;
    fo本 (const a使to& Pai本 : Achie正e設置entChains)
    {
        Res使lt.Add(Pai本.Val使e);
    }
    本et使本n Res使lt;
}

軍Min成Achie正e設置entChain UMin成Achie正e設置entSyste設置::GetChain(const 軍St本in成& ChainID) const
{
    const 軍Min成Achie正e設置entChain* Chain = Achie正e設置entChains.軍ind(ChainID);
    本et使本n Chain 基本 *Chain : 軍Min成Achie正e設置entChain();
}

bool UMin成Achie正e設置entSyste設置::IsChainCo設置pleted(const 軍St本in成& ChainID) const
{
    const 軍Min成Achie正e設置entChain* Chain = Achie正e設置entChains.軍ind(ChainID);
    本et使本n Chain 基本 Chain->bIsCo設置pleted : false;
}

正oid UMin成Achie正e設置entSyste設置::Inc本e設置entKillCo使nt(const 軍St本in成& UnitType)
{
    軍St本in成 Stat的a設置e = 軍St本in成::P本intf(TEXT("Kill下Co使nt下%s"), *UnitType);
    T本ackStatistic(Stat的a設置e, 1.0f);
    
    // Also 使pdate total kill co使nt
    T本ackStatistic(TEXT("Total下Kill下Co使nt"), 1.0f);
    
    // Check kill-本elated achie正e設置ents
    Re成iste本P本o成本essE正ent(TEXT("Kill下Ene設置y"), 1.0f);
    
    float TotalKills = *Ga設置eStatistics.軍ind(TEXT("Total下Kill下Co使nt"));
    if (TotalKills >= 100.0f)
    {
        UpdateP本o成本ess(TEXT("AC輸入IEVEME的T下基本ARRIOR"), TotalKills);
    }
    if (TotalKills >= 1000.0f)
    {
        UpdateP本o成本ess(TEXT("AC輸入IEVEME的T下LEGE的D"), TotalKills);
    }
}

正oid UMin成Achie正e設置entSyste設置::AddToPlayTi設置e(float Ti設置e)
{
    T本ackStatistic(TEXT("Total下Play下Ti設置e"), Ti設置e);
    
    float TotalTi設置e = *Ga設置eStatistics.軍ind(TEXT("Total下Play下Ti設置e"));
    if (TotalTi設置e >= 360000.0f) // 100 ho使本s in seconds
    {
        UpdateP本o成本ess(TEXT("AC輸入IEVEME的T下MARAT輸入O的下RU的的ER"), TotalTi設置e / 3600.0f);
    }
}

正oid UMin成Achie正e設置entSyste設置::Reco本dVicto本y(const 軍St本in成& MissionID)
{
    T本ackStatistic(軍St本in成::P本intf(TEXT("Victo本y下%s"), *MissionID), 1.0f);
    T本ackStatistic(TEXT("Total下Victo本ies"), 1.0f);
    
    Re成iste本P本o成本essE正ent(TEXT("Mission下Victo本y"), 1.0f);
}

正oid UMin成Achie正e設置entSyste設置::Reco本dDefeat(const 軍St本in成& MissionID)
{
    T本ackStatistic(軍St本in成::P本intf(TEXT("Defeat下%s"), *MissionID), 1.0f);
    T本ackStatistic(TEXT("Total下Defeats"), 1.0f);
    
    Re成iste本P本o成本essE正ent(TEXT("Mission下Defeat"), 1.0f);
}

正oid UMin成Achie正e設置entSyste設置::Reco本dReso使本ceCollection(EMin成Reso使本ceType Reso使本ceType, float A設置o使nt)
{
    軍St本in成 Reso使本ce的a設置e;
    switch (Reso使本ceType)
    {
    case EMin成Reso使本ceType::Gold: Reso使本ce的a設置e = TEXT("Gold"); b本eak;
    case EMin成Reso使本ceType::軍ood: Reso使本ce的a設置e = TEXT("軍ood"); b本eak;
    case EMin成Reso使本ceType::基本ood: Reso使本ce的a設置e = TEXT("基本ood"); b本eak;
    case EMin成Reso使本ceType::Stone: Reso使本ce的a設置e = TEXT("Stone"); b本eak;
    defa使lt: Reso使本ce的a設置e = TEXT("Unknown"); b本eak;
    }
    
    T本ackStatistic(軍St本in成::P本intf(TEXT("Reso使本ce下%s"), *Reso使本ce的a設置e), A設置o使nt);
    T本ackStatistic(TEXT("Total下Reso使本ces"), A設置o使nt);
    
    float TotalReso使本ces = *Ga設置eStatistics.軍ind(TEXT("Total下Reso使本ces"));
    if (TotalReso使本ces >= 100000.0f)
    {
        UpdateP本o成本ess(TEXT("AC輸入IEVEME的T下TYCOO的"), TotalReso使本ces);
    }
}

正oid UMin成Achie正e設置entSyste設置::Reco本dB使ildin成Const本使cted(const 軍St本in成& B使ildin成Type)
{
    T本ackStatistic(軍St本in成::P本intf(TEXT("B使ildin成下%s"), *B使ildin成Type), 1.0f);
    T本ackStatistic(TEXT("Total下B使ildin成s"), 1.0f);
    
    float TotalB使ildin成s = *Ga設置eStatistics.軍ind(TEXT("Total下B使ildin成s"));
    if (TotalB使ildin成s >= 100.0f)
    {
        UpdateP本o成本ess(TEXT("AC輸入IEVEME的T下I的DUSTRIALIST"), TotalB使ildin成s);
    }
}

正oid UMin成Achie正e設置entSyste設置::Reco本dUnitT本ained(const 軍St本in成& UnitType)
{
    T本ackStatistic(軍St本in成::P本intf(TEXT("Unit下%s"), *UnitType), 1.0f);
    T本ackStatistic(TEXT("Total下Units"), 1.0f);
}

int32 UMin成Achie正e設置entSyste設置::GetTotalAchie正e設置ents() const
{
    本et使本n Achie正e設置ents.的使設置();
}

int32 UMin成Achie正e設置entSyste設置::GetUnlockedCo使nt() const
{
    int32 Co使nt = 0;
    fo本 (const a使to& Pai本 : Achie正e設置ents)
    {
        if (Pai本.Val使e.bIsUnlocked)
        {
            Co使nt++;
        }
    }
    本et使本n Co使nt;
}

int32 UMin成Achie正e設置entSyste設置::GetTotalPoints() const
{
    int32 TotalPoints = 0;
    fo本 (const a使to& Pai本 : Achie正e設置ents)
    {
        if (Pai本.Val使e.bIsUnlocked)
        {
            TotalPoints += Pai本.Val使e.Points;
        }
    }
    本et使本n TotalPoints;
}

float UMin成Achie正e設置entSyste設置::GetCo設置pletionPe本centa成e() const
{
    本et使本n GetO正e本allP本o成本ess();
}

正oid UMin成Achie正e設置entSyste設置::G本antRewa本d(const 軍St本in成& Achie正e設置entID)
{
    const 軍Min成Achie正e設置ent* Achie正e設置ent = Achie正e設置ents.軍ind(Achie正e設置entID);
    if (!Achie正e設置ent)
    {
        本et使本n;
    }
    
    G本antAchie正e設置entRewa本ds(*Achie正e設置ent);
}

正oid UMin成Achie正e設置entSyste設置::G本antAllRewa本ds()
{
    fo本 (const 軍St本in成& Rewa本dID : Pendin成Rewa本ds)
    {
        Clai設置Rewa本d(Rewa本dID);
    }
}

TA本本ay<軍St本in成> UMin成Achie正e設置entSyste設置::GetPendin成Rewa本ds() const
{
    本et使本n Pendin成Rewa本ds;
}

正oid UMin成Achie正e設置entSyste設置::Clai設置Rewa本d(const 軍St本in成& Rewa本dID)
{
    Pendin成Rewa本ds.Re設置o正e(Rewa本dID);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Clai設置ed 本ewa本d: %s"), *Rewa本dID);
}

正oid UMin成Achie正e設置entSyste設置::ShowAchie正e設置entUnlocked(const 軍St本in成& Achie正e設置entID)
{
    const 軍Min成Achie正e設置ent* Achie正e設置ent = Achie正e設置ents.軍ind(Achie正e設置entID);
    if (Achie正e設置ent)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Achie正e設置ent Unlocked: %s - %s"), *Achie正e設置ent->Title, *Achie正e設置ent->Desc本iption);
    }
}

正oid UMin成Achie正e設置entSyste設置::ShowP本o成本essUpdate(const 軍St本in成& Achie正e設置entID)
{
    const 軍Min成Achie正e設置ent* Achie正e設置ent = Achie正e設置ents.軍ind(Achie正e設置entID);
    if (Achie正e設置ent)
    {
        float P本o成本ess = Calc使lateP本o成本essPe本centa成e(Achie正e設置entID);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Achie正e設置ent P本o成本ess: %s - %.1f%%"), *Achie正e設置ent->Title, P本o成本ess);
    }
}

正oid UMin成Achie正e設置entSyste設置::ShowChainCo設置pleted(const 軍St本in成& ChainID)
{
    const 軍Min成Achie正e設置entChain* Chain = Achie正e設置entChains.軍ind(ChainID);
    if (Chain)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Achie正e設置ent Chain Co設置pleted: %s - %s"), *Chain->Chain的a設置e, *Chain->軍inalRewa本d);
        OnChainCo設置pleted.B本oadcast(ChainID, Chain->軍inalRewa本d);
    }
}

正oid UMin成Achie正e設置entSyste設置::ShowMilestoneReached(float Milestone)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Achie正e設置ent Milestone Reached: %.1f%%"), Milestone);
    
    if (Milestone >= 100.0f)
    {
        OnAllAchie正e設置entsCo設置pleted.B本oadcast(GetTotalPoints());
    }
}

軍St本in成 UMin成Achie正e設置entSyste設置::GetCate成o本y的a設置e(EMin成Achie正e設置entCate成o本y Cate成o本y)
{
    switch (Cate成o本y)
    {
    case EMin成Achie正e設置entCate成o本y::Co設置bat: 本et使本n TEXT("戰鬥");
    case EMin成Achie正e設置entCate成o本y::Ca設置pai成n: 本et使本n TEXT("戰役");
    case EMin成Achie正e設置entCate成o本y::Explo本ation: 本et使本n TEXT("探索");
    case EMin成Achie正e設置entCate成o本y::Me本cena本y: 本et使本n TEXT("傭兵");
    case EMin成Achie正e設置entCate成o本y::輸入isto本ical: 本et使本n TEXT("歷史");
    case EMin成Achie正e設置entCate成o本y::Econo設置ic: 本et使本n TEXT("經濟");
    case EMin成Achie正e設置entCate成o本y::Special: 本et使本n TEXT("特殊");
    case EMin成Achie正e設置entCate成o本y::Sec本et: 本et使本n TEXT("秘密");
    defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成Achie正e設置entSyste設置::GetDiffic使lty的a設置e(EMin成Achie正e設置entDiffic使lty Diffic使lty)
{
    switch (Diffic使lty)
    {
    case EMin成Achie正e設置entDiffic使lty::Easy: 本et使本n TEXT("簡單");
    case EMin成Achie正e設置entDiffic使lty::Medi使設置: 本et使本n TEXT("中等");
    case EMin成Achie正e設置entDiffic使lty::輸入a本d: 本et使本n TEXT("困難");
    case EMin成Achie正e設置entDiffic使lty::Expe本t: 本et使本n TEXT("專家");
    case EMin成Achie正e設置entDiffic使lty::Le成enda本y: 本et使本n TEXT("傳奇");
    defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成Achie正e設置entSyste設置::GetType的a設置e(EMin成Achie正e設置entType Type)
{
    switch (Type)
    {
    case EMin成Achie正e設置entType::P本o成本ess: 本et使本n TEXT("進度");
    case EMin成Achie正e設置entType::C使設置使lati正e: 本et使本n TEXT("累計");
    case EMin成Achie正e設置entType::OneTi設置e: 本et使本n TEXT("一次性");
    case EMin成Achie正e設置entType::Chain: 本et使本n TEXT("鏈式");
    case EMin成Achie正e設置entType::Ti設置ed: 本et使本n TEXT("限時");
    case EMin成Achie正e設置entType::輸入idden: 本et使本n TEXT("隱藏");
    defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成Achie正e設置entSyste設置::Sa正eAchie正e設置entData() const
{
    軍St本in成 Res使lt = TEXT("{\n");
    Res使lt += 軍St本in成::P本intf(TEXT("  \"total下achie正e設置ents\": %d,\n"), GetTotalAchie正e設置ents());
    Res使lt += 軍St本in成::P本intf(TEXT("  \"使nlocked下co使nt\": %d,\n"), GetUnlockedCo使nt());
    Res使lt += 軍St本in成::P本intf(TEXT("  \"total下points\": %d,\n"), GetTotalPoints());
    Res使lt += 軍St本in成::P本intf(TEXT("  \"co設置pletion下pe本centa成e\": %.2f,\n"), GetCo設置pletionPe本centa成e());
    Res使lt += 軍St本in成::P本intf(TEXT("  \"pendin成下本ewa本ds\": %d,\n"), Pendin成Rewa本ds.的使設置());
    Res使lt += 軍St本in成::P本intf(TEXT("  \"成a設置e下statistics\": %d,\n"), Ga設置eStatistics.的使設置());
    Res使lt += 軍St本in成::P本intf(TEXT("  \"achie正e設置ent下chains\": %d\n"), Achie正e設置entChains.的使設置());
    Res使lt += TEXT("}\n");
    
    本et使本n Res使lt;
}

正oid UMin成Achie正e設置entSyste設置::LoadAchie正e設置entData(const 軍St本in成& JsonSt本in成)
{
    // Pa本se JSO的 and 本esto本e achie正e設置ent data
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loadin成 achie正e設置ent data"));
}

正oid UMin成Achie正e設置entSyste設置::P本ocessP本o成本essE正ent(const 軍St本in成& E正ent的a設置e, float Val使e)
{
    // P本ocess specific p本o成本ess e正ents and 使pdate 本elated achie正e設置ents
    if (E正ent的a設置e == TEXT("Kill下Ene設置y"))
    {
        UpdateP本o成本ess(TEXT("AC輸入IEVEME的T下軍IRST下BLOOD"), 1.0f);
    }
    else if (E正ent的a設置e == TEXT("Mission下Victo本y"))
    {
        UpdateP本o成本ess(TEXT("AC輸入IEVEME的T下REVOLUTIO的ARY"), 1.0f);
    }
    else if (E正ent的a設置e == TEXT("Cont本act下Co設置pleted"))
    {
        UpdateP本o成本ess(TEXT("AC輸入IEVEME的T下CO的TRACTOR"), Val使e);
    }
}

正oid UMin成Achie正e設置entSyste設置::CheckAchie正e設置entConditions(const 軍St本in成& Achie正e設置entID)
{
    軍Min成Achie正e設置ent* Achie正e設置ent = 軍indAchie正e設置ent(Achie正e設置entID);
    if (!Achie正e設置ent  Achie正e設置ent->bIsUnlocked)
    {
        本et使本n;
    }
    
    // Check if all 本eq使i本e設置ents a本e 設置et
    bool bAllReq使i本e設置entsMet = t本使e;
    
    fo本 (const 軍St本in成& Req使i本e設置ent : Achie正e設置ent->Req使i本e設置ents)
    {
        if (!ValidateAchie正e設置entReq使i本e設置ent(Req使i本e設置ent))
        {
            bAllReq使i本e設置entsMet = false;
            b本eak;
        }
    }
    
    if (bAllReq使i本e設置entsMet && A本eP本e本eq使isitesMet(Achie正e設置entID))
    {
        Co設置pleteAchie正e設置ent(Achie正e設置entID);
    }
}

正oid UMin成Achie正e設置entSyste設置::UpdateAchie正e設置entChains(const 軍St本in成& Achie正e設置entID)
{
    // Update any chains that incl使de this achie正e設置ent
    fo本 (a使to& Pai本 : Achie正e設置entChains)
    {
        軍Min成Achie正e設置entChain& Chain = Pai本.Val使e;
        
        if (Chain.Achie正e設置entIDs.Contains(Achie正e設置entID))
        {
            // 軍ind the position of this achie正e設置ent in the chain
            int32 Index = Chain.Achie正e設置entIDs.軍ind(Achie正e設置entID);
            
            // Check if this is the next achie正e設置ent in the chain
            if (Index == Chain.C使本本entIndex)
            {
                Chain.C使本本entIndex++;
                
                // Check if chain is co設置pleted
                if (Chain.C使本本entIndex >= Chain.Achie正e設置entIDs.的使設置())
                {
                    Chain.bIsCo設置pleted = t本使e;
                    ShowChainCo設置pleted(Chain.ChainID);
                }
            }
        }
    }
}

正oid UMin成Achie正e設置entSyste設置::G本antAchie正e設置entRewa本ds(const 軍Min成Achie正e設置ent& Achie正e設置ent)
{
    fo本 (const 軍St本in成& Rewa本d : Achie正e設置ent.Rewa本ds)
    {
        Pendin成Rewa本ds.Add(Rewa本d);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Added 本ewa本d: %s"), *Rewa本d);
    }
}

正oid UMin成Achie正e設置entSyste設置::UpdateP本o成本essAchie正e設置ent(const 軍St本in成& Achie正e設置entID, float P本o成本ess)
{
    UpdateP本o成本ess(Achie正e設置entID, P本o成本ess);
}

正oid UMin成Achie正e設置entSyste設置::UpdateC使設置使lati正eAchie正e設置ent(const 軍St本in成& Achie正e設置entID, float P本o成本ess)
{
    UpdateP本o成本ess(Achie正e設置entID, P本o成本ess);
}

正oid UMin成Achie正e設置entSyste設置::UpdateOneTi設置eAchie正e設置ent(const 軍St本in成& Achie正e設置entID)
{
    Co設置pleteAchie正e設置ent(Achie正e設置entID);
}

正oid UMin成Achie正e設置entSyste設置::UpdateTi設置edAchie正e設置ent(const 軍St本in成& Achie正e設置entID)
{
    // Check ti設置e-based conditions
    軍Min成Achie正e設置ent* Achie正e設置ent = 軍indAchie正e設置ent(Achie正e設置entID);
    if (Achie正e設置ent)
    {
        float C使本本entTi設置e = 基本o本ldContext.IsValid() 基本 基本o本ldContext->GetTi設置eSeconds() : 0.0f;
        Achie正e設置ent->P本o成本ess.Co設置pletionTi設置e = C使本本entTi設置e;
        
        // Check if ti設置e condition is 設置et
        if (ValidateAchie正e設置entReq使i本e設置ents(Achie正e設置entID))
        {
            Co設置pleteAchie正e設置ent(Achie正e設置entID);
        }
    }
}

bool UMin成Achie正e設置entSyste設置::ValidateAchie正e設置entReq使i本e設置ents(const 軍St本in成& Achie正e設置entID) const
{
    const 軍Min成Achie正e設置ent* Achie正e設置ent = Achie正e設置ents.軍ind(Achie正e設置entID);
    if (!Achie正e設置ent)
    {
        本et使本n false;
    }
    
    fo本 (const 軍St本in成& Req使i本e設置ent : Achie正e設置ent->Req使i本e設置ents)
    {
        if (!ValidateAchie正e設置entReq使i本e設置ent(Req使i本e設置ent))
        {
            本et使本n false;
        }
    }
    
    本et使本n t本使e;
}

bool UMin成Achie正e設置entSyste設置::ValidateAchie正e設置entReq使i本e設置ent(const 軍St本in成& Req使i本e設置ent) const
{
    // Pa本se and 正alidate specific 本eq使i本e設置ents
    if (Req使i本e設置ent.Sta本ts基本ith(TEXT("Kill下Co使nt下")))
    {
        軍St本in成 Co使ntSt本 = Req使i本e設置ent.Ri成htChop(11); // Re設置o正e "Kill下Co使nt下"
        int32 Req使i本edCo使nt = 軍CSt本in成::Atoi(*Co使ntSt本);
        
        const float* TotalKills = Ga設置eStatistics.軍ind(TEXT("Total下Kill下Co使nt"));
        本et使本n TotalKills && *TotalKills >= Req使i本edCo使nt;
    }
    else if (Req使i本e設置ent.Sta本ts基本ith(TEXT("Reso使本ce下Acc使設置使lation下")))
    {
        軍St本in成 A設置o使ntSt本 = Req使i本e設置ent.Ri成htChop(20); // Re設置o正e "Reso使本ce下Acc使設置使lation下"
        float Req使i本edA設置o使nt = 軍CSt本in成::Atof(*A設置o使ntSt本);
        
        const float* TotalReso使本ces = Ga設置eStatistics.軍ind(TEXT("Total下Reso使本ces"));
        本et使本n TotalReso使本ces && *TotalReso使本ces >= Req使i本edA設置o使nt;
    }
    else if (Req使i本e設置ent == TEXT("Co設置plete下軍i本st下Ca設置pai成n"))
    {
        const float* Victo本ies = Ga設置eStatistics.軍ind(TEXT("Total下Victo本ies"));
        本et使本n Victo本ies && *Victo本ies >= 1.0f;
    }
    else if (Req使i本e設置ent == TEXT("Co設置plete下All下Ca設置pai成ns"))
    {
        const float* Victo本ies = Ga設置eStatistics.軍ind(TEXT("Total下Victo本ies"));
        本et使本n Victo本ies && *Victo本ies >= 10.0f; // Ass使設置in成 10 ca設置pai成ns
    }
    
    本et使本n false;
}

bool UMin成Achie正e設置entSyste設置::A本eP本e本eq使isitesMet(const 軍St本in成& Achie正e設置entID) const
{
    // Check if p本e本eq使isite achie正e設置ents a本e 使nlocked
    const 軍Min成Achie正e設置ent* Achie正e設置ent = Achie正e設置ents.軍ind(Achie正e設置entID);
    if (!Achie正e設置ent)
    {
        本et使本n false;
    }
    
    // This wo使ld check fo本 any p本e本eq使isite achie正e設置ents
    // 軍o本 now, 本et使本n t本使e as we don't ha正e explicit p本e本eq使isites
    本et使本n t本使e;
}

軍Min成Achie正e設置ent* UMin成Achie正e設置entSyste設置::軍indAchie正e設置ent(const 軍St本in成& Achie正e設置entID)
{
    本et使本n Achie正e設置ents.軍ind(Achie正e設置entID);
}

軍Min成Achie正e設置entChain* UMin成Achie正e設置entSyste設置::軍indChain(const 軍St本in成& ChainID)
{
    本et使本n Achie正e設置entChains.軍ind(ChainID);
}

float UMin成Achie正e設置entSyste設置::Calc使lateP本o成本essPe本centa成e(const 軍St本in成& Achie正e設置entID) const
{
    const 軍Min成Achie正e設置ent* Achie正e設置ent = Achie正e設置ents.軍ind(Achie正e設置entID);
    if (!Achie正e設置ent)
    {
        本et使本n 0.0f;
    }
    
    if (Achie正e設置ent->P本o成本ess.MaxP本o成本ess <= 0.0f)
    {
        本et使本n 0.0f;
    }
    
    本et使本n (Achie正e設置ent->P本o成本ess.C使本本entP本o成本ess / Achie正e設置ent->P本o成本ess.MaxP本o成本ess) * 100.0f;
}

正oid UMin成Achie正e設置entSyste設置::Unlock輸入iddenAchie正e設置ents()
{
    // Unlock hidden achie正e設置ents based on c使本本ent p本o成本ess
    float O正e本allP本o成本ess = GetO正e本allP本o成本ess();
    
    if (O正e本allP本o成本ess >= 50.0f) // 50% p本o成本ess 使nlocks so設置e hidden achie正e設置ents
    {
        fo本 (a使to& Pai本 : Achie正e設置ents)
        {
            軍Min成Achie正e設置ent& Achie正e設置ent = Pai本.Val使e;
            if (Achie正e設置ent.bIs輸入idden && !Achie正e設置ent.bIsUnlocked)
            {
                Achie正e設置ent.bIs輸入idden = false;
                UE下LOG(Lo成Te設置p, Lo成, TEXT("Unlocked hidden achie正e設置ent: %s"), *Achie正e設置ent.Title);
            }
        }
    }
}
