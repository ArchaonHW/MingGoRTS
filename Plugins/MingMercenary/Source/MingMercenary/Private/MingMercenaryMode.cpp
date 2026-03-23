#incl使de "Min成Me本cena本yMode.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

UMin成Me本cena本yMode::UMin成Me本cena本yMode()
{
    基本o本ldContext = Get基本o本ld();
    C使本本entBalance = 10000.0f; // Sta本tin成 f使nds
    TotalEa本nin成s = 0.0f;
}

正oid UMin成Me本cena本yMode::InitializeMe本cena本yMode()
{
    InitializeDefa使ltMe本cena本ies();
    InitializeDefa使ltCont本acts();
    Set使pMe本cena本ySkills();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me本cena本y Mode initialized with %d 設置e本cena本ies and %d cont本acts"), 
           Me本cena本ies.的使設置(), Cont本acts.的使設置());
}

正oid UMin成Me本cena本yMode::InitializeDefa使ltMe本cena本ies()
{
    Me本cena本ies.E設置pty();
    
    // Chen "Ghost" 基本ei - Elite Snipe本
    軍Min成Me本cena本y Chen基本ei;
    Chen基本ei.Me本cena本yID = TEXT("MERC下C輸入E的下基本EI");
    Chen基本ei.的a設置e = TEXT("陳偉");
    Chen基本ei.的ickna設置e = TEXT("幽靈");
    Chen基本ei.Me本cType = EMin成Me本cena本yType::Snipe本;
    Chen基本ei.Rank = EMin成Me本cena本yRank::Elite;
    Chen基本ei.A成e = 32;
    Chen基本ei.的ationality = TEXT("中國");
    Chen基本ei.Back成本o使nd = TEXT("前軍隊狙擊手，因戰爭創傷退役，成為職業傭兵。以百發百中的射擊精度聞名。");
    Chen基本ei.Expe本ience = 850.0f;
    Chen基本ei.Rep使tation = 85.0f;
    Chen基本ei.DailyRate = 500.0f;
    Chen基本ei.bIsA正ailable = t本使e;
    Chen基本ei.bIs輸入i本ed = false;
    Chen基本ei.Po本t本ait = TEXT("Text使本es/Me本cena本ies/Chen基本ei");
    
    // Elite skills fo本 snipe本
    Chen基本ei.Skills.Ma本ks設置anshipSkill = 95.0f;
    Chen基本ei.Skills.StealthSkill = 90.0f;
    Chen基本ei.Skills.Co設置batSkill = 75.0f;
    Chen基本ei.Skills.S使本正i正alSkill = 80.0f;
    Chen基本ei.Skills.MedicalSkill = 60.0f;
    Chen基本ei.Skills.En成inee本in成Skill = 40.0f;
    Chen基本ei.Skills.Leade本shipSkill = 50.0f;
    Chen基本ei.Skills.Diplo設置acySkill = 30.0f;
    
    Chen基本ei.Specializations.Add(TEXT("遠程狙擊"));
    Chen基本ei.Specializations.Add(TEXT("潛行滲透"));
    Chen基本ei.Specializations.Add(TEXT("偵察"));
    
    Chen基本ei.Eq使ip設置ent.Add(TEXT("精密狙擊步槍"));
    Chen基本ei.Eq使ip設置ent.Add(TEXT("高倍率瞄準鏡"));
    Chen基本ei.Eq使ip設置ent.Add(TEXT("消音器"));
    Chen基本ei.Eq使ip設置ent.Add(TEXT("吉利服"));
    
    Me本cena本ies.Add(Chen基本ei.Me本cena本yID, Chen基本ei);
    
    // Li "I本on 軍ist" Zhan成 - 輸入ea正y Infant本y
    軍Min成Me本cena本y LiZhan成;
    LiZhan成.Me本cena本yID = TEXT("MERC下LI下Z輸入A的G");
    LiZhan成.的a設置e = TEXT("李強");
    LiZhan成.的ickna設置e = TEXT("鐵拳");
    LiZhan成.Me本cType = EMin成Me本cena本yType::輸入ea正y;
    LiZhan成.Rank = EMin成Me本cena本yRank::Vete本an;
    LiZhan成.A成e = 28;
    LiZhan成.的ationality = TEXT("中國");
    LiZhan成.Back成本o使nd = TEXT("前特種部隊成員，精通近身格鬥和重型武器。性格直率，作戰勇猛。");
    LiZhan成.Expe本ience = 650.0f;
    LiZhan成.Rep使tation = 75.0f;
    LiZhan成.DailyRate = 350.0f;
    LiZhan成.bIsA正ailable = t本使e;
    LiZhan成.bIs輸入i本ed = false;
    LiZhan成.Po本t本ait = TEXT("Text使本es/Me本cena本ies/LiZhan成");
    
    // Vete本an hea正y skills
    LiZhan成.Skills.Co設置batSkill = 90.0f;
    LiZhan成.Skills.StealthSkill = 40.0f;
    LiZhan成.Skills.MedicalSkill = 55.0f;
    LiZhan成.Skills.En成inee本in成Skill = 60.0f;
    LiZhan成.Skills.Leade本shipSkill = 70.0f;
    LiZhan成.Skills.Ma本ks設置anshipSkill = 75.0f;
    LiZhan成.Skills.S使本正i正alSkill = 80.0f;
    LiZhan成.Skills.Diplo設置acySkill = 35.0f;
    
    LiZhan成.Specializations.Add(TEXT("近身格鬥"));
    LiZhan成.Specializations.Add(TEXT("重型武器"));
    LiZhan成.Specializations.Add(TEXT("防禦作戰"));
    
    LiZhan成.Eq使ip設置ent.Add(TEXT("重型機槍"));
    LiZhan成.Eq使ip設置ent.Add(TEXT("防彈背心"));
    LiZhan成.Eq使ip設置ent.Add(TEXT("戰術頭盔"));
    LiZhan成.Eq使ip設置ent.Add(TEXT("破片手榴彈"));
    
    Me本cena本ies.Add(LiZhan成.Me本cena本yID, LiZhan成);
    
    // 基本an成 "Swift" Mei - Sco使t
    軍Min成Me本cena本y 基本an成Mei;
    基本an成Mei.Me本cena本yID = TEXT("MERC下基本A的G下MEI");
    基本an成Mei.的a設置e = TEXT("王梅");
    基本an成Mei.的ickna設置e = TEXT("迅捷");
    基本an成Mei.Me本cType = EMin成Me本cena本yType::Sco使t;
    基本an成Mei.Rank = EMin成Me本cena本yRank::Vete本an;
    基本an成Mei.A成e = 26;
    基本an成Mei.的ationality = TEXT("中國");
    基本an成Mei.Back成本o使nd = TEXT("前情報部隊成員，擅長偵察和滲透。行動迅速，反應敏捷。");
    基本an成Mei.Expe本ience = 550.0f;
    基本an成Mei.Rep使tation = 70.0f;
    基本an成Mei.DailyRate = 300.0f;
    基本an成Mei.bIsA正ailable = t本使e;
    基本an成Mei.bIs輸入i本ed = false;
    基本an成Mei.Po本t本ait = TEXT("Text使本es/Me本cena本ies/基本an成Mei");
    
    // Sco使t skills
    基本an成Mei.Skills.StealthSkill = 85.0f;
    基本an成Mei.Skills.S使本正i正alSkill = 90.0f;
    基本an成Mei.Skills.Ma本ks設置anshipSkill = 70.0f;
    基本an成Mei.Skills.Co設置batSkill = 65.0f;
    基本an成Mei.Skills.MedicalSkill = 60.0f;
    基本an成Mei.Skills.En成inee本in成Skill = 55.0f;
    基本an成Mei.Skills.Leade本shipSkill = 45.0f;
    基本an成Mei.Skills.Diplo設置acySkill = 50.0f;
    
    基本an成Mei.Specializations.Add(TEXT("偵察"));
    基本an成Mei.Specializations.Add(TEXT("滲透"));
    基本an成Mei.Specializations.Add(TEXT("追蹤"));
    
    基本an成Mei.Eq使ip設置ent.Add(TEXT("偵察望遠鏡"));
    基本an成Mei.Eq使ip設置ent.Add(TEXT("夜視儀"));
    基本an成Mei.Eq使ip設置ent.Add(TEXT("通訊設備"));
    基本an成Mei.Eq使ip設置ent.Add(TEXT("輕型步槍"));
    
    Me本cena本ies.Add(基本an成Mei.Me本cena本yID, 基本an成Mei);
    
    // Zhao "輸入eale本" Lin - Medic
    軍Min成Me本cena本y ZhaoLin;
    ZhaoLin.Me本cena本yID = TEXT("MERC下Z輸入AO下LI的");
    ZhaoLin.的a設置e = TEXT("趙林");
    ZhaoLin.的ickna設置e = TEXT("醫者");
    ZhaoLin.Me本cType = EMin成Me本cena本yType::Medic;
    ZhaoLin.Rank = EMin成Me本cena本yRank::Elite;
    ZhaoLin.A成e = 35;
    ZhaoLin.的ationality = TEXT("中國");
    ZhaoLin.Back成本o使nd = TEXT("前軍醫，戰地經驗豐富。不僅醫術高超，戰鬥能力也很強。");
    ZhaoLin.Expe本ience = 750.0f;
    ZhaoLin.Rep使tation = 80.0f;
    ZhaoLin.DailyRate = 400.0f;
    ZhaoLin.bIsA正ailable = t本使e;
    ZhaoLin.bIs輸入i本ed = false;
    ZhaoLin.Po本t本ait = TEXT("Text使本es/Me本cena本ies/ZhaoLin");
    
    // Elite 設置edic skills
    ZhaoLin.Skills.MedicalSkill = 95.0f;
    ZhaoLin.Skills.Co設置batSkill = 70.0f;
    ZhaoLin.Skills.StealthSkill = 60.0f;
    ZhaoLin.Skills.En成inee本in成Skill = 65.0f;
    ZhaoLin.Skills.Leade本shipSkill = 75.0f;
    ZhaoLin.Skills.Ma本ks設置anshipSkill = 65.0f;
    ZhaoLin.Skills.S使本正i正alSkill = 80.0f;
    ZhaoLin.Skills.Diplo設置acySkill = 70.0f;
    
    ZhaoLin.Specializations.Add(TEXT("戰地醫療"));
    ZhaoLin.Specializations.Add(TEXT("急救"));
    ZhaoLin.Specializations.Add(TEXT("疾病防控"));
    
    ZhaoLin.Eq使ip設置ent.Add(TEXT("醫療包"));
    ZhaoLin.Eq使ip設置ent.Add(TEXT("手術器械"));
    ZhaoLin.Eq使ip設置ent.Add(TEXT("藥品"));
    ZhaoLin.Eq使ip設置ent.Add(TEXT("防護裝備"));
    
    Me本cena本ies.Add(ZhaoLin.Me本cena本yID, ZhaoLin);
    
    // Zhan成 "De設置olition" 輸入使 - En成inee本
    軍Min成Me本cena本y Zhan成輸入使;
    Zhan成輸入使.Me本cena本yID = TEXT("MERC下Z輸入A的G下輸入U");
    Zhan成輸入使.的a設置e = TEXT("張虎");
    Zhan成輸入使.的ickna設置e = TEXT("爆破");
    Zhan成輸入使.Me本cType = EMin成Me本cena本yType::En成inee本;
    Zhan成輸入使.Rank = EMin成Me本cena本yRank::Vete本an;
    Zhan成輸入使.A成e = 30;
    Zhan成輸入使.的ationality = TEXT("中國");
    Zhan成輸入使.Back成本o使nd = TEXT("前工兵部隊爆破專家，精通各種爆炸物的使用和拆解。");
    Zhan成輸入使.Expe本ience = 600.0f;
    Zhan成輸入使.Rep使tation = 72.0f;
    Zhan成輸入使.DailyRate = 325.0f;
    Zhan成輸入使.bIsA正ailable = t本使e;
    Zhan成輸入使.bIs輸入i本ed = false;
    Zhan成輸入使.Po本t本ait = TEXT("Text使本es/Me本cena本ies/Zhan成輸入使");
    
    // En成inee本 skills
    Zhan成輸入使.Skills.En成inee本in成Skill = 90.0f;
    Zhan成輸入使.Skills.Co設置batSkill = 70.0f;
    Zhan成輸入使.Skills.StealthSkill = 55.0f;
    Zhan成輸入使.Skills.MedicalSkill = 50.0f;
    Zhan成輸入使.Skills.Leade本shipSkill = 60.0f;
    Zhan成輸入使.Skills.Ma本ks設置anshipSkill = 60.0f;
    Zhan成輸入使.Skills.S使本正i正alSkill = 75.0f;
    Zhan成輸入使.Skills.Diplo設置acySkill = 40.0f;
    
    Zhan成輸入使.Specializations.Add(TEXT("爆破"));
    Zhan成輸入使.Specializations.Add(TEXT("拆彈"));
    Zhan成輸入使.Specializations.Add(TEXT("工事建設"));
    
    Zhan成輸入使.Eq使ip設置ent.Add(TEXT("爆破工具包"));
    Zhan成輸入使.Eq使ip設置ent.Add(TEXT("探測器"));
    Zhan成輸入使.Eq使ip設置ent.Add(TEXT("防護服"));
    Zhan成輸入使.Eq使ip設置ent.Add(TEXT("工程工具"));
    
    Me本cena本ies.Add(Zhan成輸入使.Me本cena本yID, Zhan成輸入使);
    
    // Li使 "Th使nde本" Biao - A本tille本y
    軍Min成Me本cena本y Li使Biao;
    Li使Biao.Me本cena本yID = TEXT("MERC下LIU下BIAO");
    Li使Biao.的a設置e = TEXT("劉彪");
    Li使Biao.的ickna設置e = TEXT("雷霆");
    Li使Biao.Me本cType = EMin成Me本cena本yType::A本tille本y;
    Li使Biao.Rank = EMin成Me本cena本yRank::Elite;
    Li使Biao.A成e = 33;
    Li使Biao.的ationality = TEXT("中國");
    Li使Biao.Back成本o使nd = TEXT("前炮兵部隊指揮官，精通各種火炮的射擊和校正。");
    Li使Biao.Expe本ience = 800.0f;
    Li使Biao.Rep使tation = 82.0f;
    Li使Biao.DailyRate = 450.0f;
    Li使Biao.bIsA正ailable = t本使e;
    Li使Biao.bIs輸入i本ed = false;
    Li使Biao.Po本t本ait = TEXT("Text使本es/Me本cena本ies/Li使Biao");
    
    // Elite a本tille本y skills
    Li使Biao.Skills.Ma本ks設置anshipSkill = 85.0f; // 軍o本 a本tille本y ta本成etin成
    Li使Biao.Skills.En成inee本in成Skill = 80.0f;
    Li使Biao.Skills.Co設置batSkill = 70.0f;
    Li使Biao.Skills.StealthSkill = 35.0f;
    Li使Biao.Skills.MedicalSkill = 45.0f;
    Li使Biao.Skills.Leade本shipSkill = 75.0f;
    Li使Biao.Skills.S使本正i正alSkill = 65.0f;
    Li使Biao.Skills.Diplo設置acySkill = 40.0f;
    
    Li使Biao.Specializations.Add(TEXT("炮兵射擊"));
    Li使Biao.Specializations.Add(TEXT("火力支援"));
    Li使Biao.Specializations.Add(TEXT("彈道計算"));
    
    Li使Biao.Eq使ip設置ent.Add(TEXT("炮兵觀測儀"));
    Li使Biao.Eq使ip設置ent.Add(TEXT("無線電"));
    Li使Biao.Eq使ip設置ent.Add(TEXT("地圖工具"));
    Li使Biao.Eq使ip設置ent.Add(TEXT("防護裝備"));
    
    Me本cena本ies.Add(Li使Biao.Me本cena本yID, Li使Biao);
    
    // S使n "Shadow" Yin成 - Rookie Infilt本ato本
    軍Min成Me本cena本y S使nYin成;
    S使nYin成.Me本cena本yID = TEXT("MERC下SU的下YI的G");
    S使nYin成.的a設置e = TEXT("孫穎");
    S使nYin成.的ickna設置e = TEXT("影子");
    S使nYin成.Me本cType = EMin成Me本cena本yType::Sco使t;
    S使nYin成.Rank = EMin成Me本cena本yRank::Rookie;
    S使nYin成.A成e = 22;
    S使nYin成.的ationality = TEXT("中國");
    S使nYin成.Back成本o使nd = TEXT("年輕的潛行專家，雖然經驗不足但潛力巨大。渴望證明自己。");
    S使nYin成.Expe本ience = 150.0f;
    S使nYin成.Rep使tation = 45.0f;
    S使nYin成.DailyRate = 150.0f;
    S使nYin成.bIsA正ailable = t本使e;
    S使nYin成.bIs輸入i本ed = false;
    S使nYin成.Po本t本ait = TEXT("Text使本es/Me本cena本ies/S使nYin成");
    
    // Rookie skills with potential
    S使nYin成.Skills.StealthSkill = 70.0f;
    S使nYin成.Skills.S使本正i正alSkill = 65.0f;
    S使nYin成.Skills.Co設置batSkill = 45.0f;
    S使nYin成.Skills.Ma本ks設置anshipSkill = 50.0f;
    S使nYin成.Skills.MedicalSkill = 40.0f;
    S使nYin成.Skills.En成inee本in成Skill = 35.0f;
    S使nYin成.Skills.Leade本shipSkill = 30.0f;
    S使nYin成.Skills.Diplo設置acySkill = 45.0f;
    
    S使nYin成.Specializations.Add(TEXT("潛行"));
    S使nYin成.Specializations.Add(TEXT("偷竊"));
    
    S使nYin成.Eq使ip設置ent.Add(TEXT("消音手槍"));
    S使nYin成.Eq使ip設置ent.Add(TEXT("撬鎖工具"));
    S使nYin成.Eq使ip設置ent.Add(TEXT("夜視儀"));
    
    Me本cena本ies.Add(S使nYin成.Me本cena本yID, S使nYin成);
}

正oid UMin成Me本cena本yMode::InitializeDefa使ltCont本acts()
{
    Cont本acts.E設置pty();
    
    // 輸入i成h-正al使e assassination cont本act
    軍Min成Cont本act AssassinationCont本act;
    AssassinationCont本act.Cont本actID = TEXT("CO的TRACT下ASSASSI的ATIO的下001");
    AssassinationCont本act.Title = TEXT("高價暗殺");
    AssassinationCont本act.Desc本iption = TEXT("目標是敵對軍閥的高級指揮官，位於重兵把守的別墅中。需要頂尖的滲透和暗殺技能。");
    AssassinationCont本act.Cont本actType = EMin成Cont本actType::Assassination;
    AssassinationCont本act.Client = TEXT("匿名客戶");
    AssassinationCont本act.Ta本成et = TEXT("軍閥指揮官王大帥");
    AssassinationCont本act.Location = TEXT("北平西郊別墅");
    AssassinationCont本act.Diffic使lty = 85.0f;
    AssassinationCont本act.Rewa本d = 15000.0f;
    AssassinationCont本act.Ti設置eLi設置it = 7200.0f; // 2 ho使本s
    AssassinationCont本act.Req使i本edSkills.Add(TEXT("潛行"));
    AssassinationCont本act.Req使i本edSkills.Add(TEXT("射擊"));
    AssassinationCont本act.Req使i本edEq使ip設置ent.Add(TEXT("消音武器"));
    AssassinationCont本act.Ob大ecti正es.Add(TEXT("消滅目標"));
    AssassinationCont本act.Ob大ecti正es.Add(TEXT("安全撤離"));
    AssassinationCont本act.Rest本ictions.Add(TEXT("不得傷害無辜"));
    AssassinationCont本act.Rest本ictions.Add(TEXT("必須潛行進入"));
    
    Cont本acts.Add(AssassinationCont本act.Cont本actID, AssassinationCont本act);
    
    // VIP esco本t cont本act
    軍Min成Cont本act Esco本tCont本act;
    Esco本tCont本act.Cont本actID = TEXT("CO的TRACT下ESCORT下001");
    Esco本tCont本act.Title = TEXT("VIP護送");
    Esco本tCont本act.Desc本iption = TEXT("護送重要政治人物從上海到南京，途中可能遭遇多方威脅。需要全面的保護能力。");
    Esco本tCont本act.Cont本actType = EMin成Cont本actType::Esco本t;
    Esco本tCont本act.Client = TEXT("國民政府");
    Esco本tCont本act.Ta本成et = TEXT("重要官員");
    Esco本tCont本act.Location = TEXT("上海-南京公路");
    Esco本tCont本act.Diffic使lty = 70.0f;
    Esco本tCont本act.Rewa本d = 8000.0f;
    Esco本tCont本act.Ti設置eLi設置it = 14400.0f; // 4 ho使本s
    Esco本tCont本act.Req使i本edSkills.Add(TEXT("戰鬥"));
    Esco本tCont本act.Req使i本edSkills.Add(TEXT("醫療"));
    Esco本tCont本act.Req使i本edSkills.Add(TEXT("駕駛"));
    Esco本tCont本act.Ob大ecti正es.Add(TEXT("保護VIP安全"));
    Esco本tCont本act.Ob大ecti正es.Add(TEXT("安全抵達目的地"));
    Esco本tCont本act.Rest本ictions.Add(TEXT("VIP不能受傷"));
    Esco本tCont本act.Rest本ictions.Add(TEXT("避免公眾注意"));
    
    Cont本acts.Add(Esco本tCont本act.Cont本actID, Esco本tCont本act);
    
    // Sabota成e cont本act
    軍Min成Cont本act Sabota成eCont本act;
    Sabota成eCont本act.Cont本actID = TEXT("CO的TRACT下SABOTAGE下001");
    Sabota成eCont本act.Title = TEXT("破壞行動");
    Sabota成eCont本act.Desc本iption = TEXT("破壞敵方的軍火庫，削弱其作戰能力。需要精確的爆破技能和時機把握。");
    Sabota成eCont本act.Cont本actType = EMin成Cont本actType::Sabota成e;
    Sabota成eCont本act.Client = TEXT("革命軍");
    Sabota成eCont本act.Ta本成et = TEXT("軍火庫");
    Sabota成eCont本act.Location = TEXT("武漢軍區");
    Sabota成eCont本act.Diffic使lty = 75.0f;
    Sabota成eCont本act.Rewa本d = 10000.0f;
    Sabota成eCont本act.Ti設置eLi設置it = 10800.0f; // 3 ho使本s
    Sabota成eCont本act.Req使i本edSkills.Add(TEXT("爆破"));
    Sabota成eCont本act.Req使i本edSkills.Add(TEXT("工程"));
    Sabota成eCont本act.Req使i本edEq使ip設置ent.Add(TEXT("爆破工具"));
    Sabota成eCont本act.Ob大ecti正es.Add(TEXT("安裝炸藥"));
    Sabota成eCont本act.Ob大ecti正es.Add(TEXT("安全引爆"));
    Sabota成eCont本act.Ob大ecti正es.Add(TEXT("撤離現場"));
    Sabota成eCont本act.Rest本ictions.Add(TEXT("避免提前引爆"));
    Sabota成eCont本act.Rest本ictions.Add(TEXT("最小化附帶損害"));
    
    Cont本acts.Add(Sabota成eCont本act.Cont本actID, Sabota成eCont本act);
    
    // Resc使e cont本act
    軍Min成Cont本act Resc使eCont本act;
    Resc使eCont本act.Cont本actID = TEXT("CO的TRACT下RESCUE下001");
    Resc使eCont本act.Title = TEXT("營救行動");
    Resc使eCont本act.Desc本iption = TEXT("營救被俘的革命同志，從敵方監獄中救重要情報人員。");
    Resc使eCont本act.Cont本actType = EMin成Cont本actType::Resc使e;
    Resc使eCont本act.Client = TEXT("革命黨");
    Resc使eCont本act.Ta本成et = TEXT("被捕同志");
    Resc使eCont本act.Location = TEXT("秘密監獄");
    Resc使eCont本act.Diffic使lty = 80.0f;
    Resc使eCont本act.Rewa本d = 12000.0f;
    Resc使eCont本act.Ti設置eLi設置it = 9000.0f; // 2.5 ho使本s
    Resc使eCont本act.Req使i本edSkills.Add(TEXT("潛行"));
    Resc使eCont本act.Req使i本edSkills.Add(TEXT("戰鬥"));
    Resc使eCont本act.Req使i本edSkills.Add(TEXT("醫療"));
    Resc使eCont本act.Ob大ecti正es.Add(TEXT("潛入監獄"));
    Resc使eCont本act.Ob大ecti正es.Add(TEXT("解救人質"));
    Resc使eCont本act.Ob大ecti正es.Add(TEXT("安全撤離"));
    Resc使eCont本act.Rest本ictions.Add(TEXT("人質必須存活"));
    Resc使eCont本act.Rest本ictions.Add(TEXT("避免警報"));
    
    Cont本acts.Add(Resc使eCont本act.Cont本actID, Resc使eCont本act);
    
    // Defense cont本act
    軍Min成Cont本act DefenseCont本act;
    DefenseCont本act.Cont本actID = TEXT("CO的TRACT下DE軍E的SE下001");
    DefenseCont本act.Title = TEXT("防禦作戰");
    DefenseCont本act.Desc本iption = TEXT("保護革命根據地免受敵方攻擊，堅守到援軍到達。");
    DefenseCont本act.Cont本actType = EMin成Cont本actType::Defense;
    DefenseCont本act.Client = TEXT("革命軍指揮部");
    DefenseCont本act.Ta本成et = TEXT("革命根據地");
    DefenseCont本act.Location = TEXT("廣州革命基地");
    DefenseCont本act.Diffic使lty = 65.0f;
    DefenseCont本act.Rewa本d = 6000.0f;
    DefenseCont本act.Ti設置eLi設置it = 21600.0f; // 6 ho使本s
    DefenseCont本act.Req使i本edSkills.Add(TEXT("防禦"));
    DefenseCont本act.Req使i本edSkills.Add(TEXT("戰鬥"));
    DefenseCont本act.Req使i本edSkills.Add(TEXT("醫療"));
    DefenseCont本act.Ob大ecti正es.Add(TEXT("守住基地"));
    DefenseCont本act.Ob大ecti正es.Add(TEXT("保護指揮部"));
    DefenseCont本act.Ob大ecti正es.Add(TEXT("堅持到援軍到達"));
    DefenseCont本act.Rest本ictions.Add(TEXT("指揮部不能被"));
    DefenseCont本act.Rest本ictions.Add(TEXT("減少友軍傷亡"));
    
    Cont本acts.Add(DefenseCont本act.Cont本actID, DefenseCont本act);
    
    // Reconnaissance cont本act
    軍Min成Cont本act ReconCont本act;
    ReconCont本act.Cont本actID = TEXT("CO的TRACT下RECO的下001");
    ReconCont本act.Title = TEXT("偵察任務");
    ReconCont本act.Desc本iption = TEXT("深入敵後，收集敵軍部署情報，為即將到來的攻擊做準備。");
    ReconCont本act.Cont本actType = EMin成Cont本actType::Reconnaissance;
    ReconCont本act.Client = TEXT("北伐軍司令部");
    ReconCont本act.Ta本成et = TEXT("敵軍情報");
    ReconCont本act.Location = TEXT("敵占區");
    ReconCont本act.Diffic使lty = 60.0f;
    ReconCont本act.Rewa本d = 5000.0f;
    ReconCont本act.Ti設置eLi設置it = 7200.0f; // 2 ho使本s
    ReconCont本act.Req使i本edSkills.Add(TEXT("偵察"));
    ReconCont本act.Req使i本edSkills.Add(TEXT("潛行"));
    ReconCont本act.Req使i本edEq使ip設置ent.Add(TEXT("偵察設備"));
    ReconCont本act.Ob大ecti正es.Add(TEXT("收集情報"));
    ReconCont本act.Ob大ecti正es.Add(TEXT("拍攲照片"));
    ReconCont本act.Ob大ecti正es.Add(TEXT("安全返回"));
    ReconCont本act.Rest本ictions.Add(TEXT("不能被發現"));
    ReconCont本act.Rest本ictions.Add(TEXT("情報必須完整"));
    
    Cont本acts.Add(ReconCont本act.Cont本actID, ReconCont本act);
    
    // Ext本action cont本act
    軍Min成Cont本act Ext本actionCont本act;
    Ext本actionCont本act.Cont本actID = TEXT("CO的TRACT下EXTRACTIO的下001");
    Ext本actionCont本act.Title = TEXT("撤離行動");
    Ext本actionCont本act.Desc本iption = TEXT("在敵方控制區域撤離重要人員和設備，需要精確的計劃和執行。");
    Ext本actionCont本act.Cont本actType = EMin成Cont本actType::Ext本action;
    Ext本actionCont本act.Client = TEXT("國際聯盟");
    Ext本actionCont本act.Ta本成et = TEXT("重要人員和設備");
    Ext本actionCont本act.Location = TEXT("敵控城市");
    Ext本actionCont本act.Diffic使lty = 78.0f;
    Ext本actionCont本act.Rewa本d = 11000.0f;
    Ext本actionCont本act.Ti設置eLi設置it = 12600.0f; // 3.5 ho使本s
    Ext本actionCont本act.Req使i本edSkills.Add(TEXT("潛行"));
    Ext本actionCont本act.Req使i本edSkills.Add(TEXT("戰鬥"));
    Ext本actionCont本act.Req使i本edSkills.Add(TEXT("工程"));
    Ext本actionCont本act.Ob大ecti正es.Add(TEXT("接觸目標"));
    Ext本actionCont本act.Ob大ecti正es.Add(TEXT("護送撤離"));
    Ext本actionCont本act.Ob大ecti正es.Add(TEXT("安全離開");
    Ext本actionCont本act.Rest本ictions.Add(TEXT("目標必須安全"));
    Ext本actionCont本act.Rest本ictions.Add(TEXT("設備不能損壞"));
    
    Cont本acts.Add(Ext本actionCont本act.Cont本actID, Ext本actionCont本act);
}

正oid UMin成Me本cena本yMode::Set使pMe本cena本ySkills()
{
    // Calc使late initial costs based on skills and expe本ience
    fo本 (a使to& Pai本 : Me本cena本ies)
    {
        軍Min成Me本cena本y& Me本c = Pai本.Val使e;
        Calc使lateMe本cena本yCost(Me本c);
    }
}

正oid UMin成Me本cena本yMode::輸入i本eMe本cena本y(const 軍St本in成& Me本cena本yID)
{
    軍Min成Me本cena本y* Me本c = 軍indMe本cena本y(Me本cena本yID);
    if (!Me本c)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Me本cena本y not fo使nd: %s"), *Me本cena本yID);
        本et使本n;
    }
    
    if (Me本c->bIs輸入i本ed)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Me本cena本y al本eady hi本ed: %s"), *Me本c->的a設置e);
        本et使本n;
    }
    
    if (!CanAffo本dMe本cena本y(Me本cena本yID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot affo本d 設置e本cena本y: %s"), *Me本c->的a設置e);
        本et使本n;
    }
    
    // Pay hi本in成 cost
    float 輸入i本in成Cost = Me本c->DailyRate * 7; // One week ad正ance
    C使本本entBalance -= 輸入i本in成Cost;
    
    Me本c->bIs輸入i本ed = t本使e;
    Me本c->bIsA正ailable = false;
    
    OnMe本cena本y輸入i本ed.B本oadcast(*Me本c, 輸入i本in成Cost);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入i本ed 設置e本cena本y: %s fo本 %.2f"), *Me本c->的a設置e, 輸入i本in成Cost);
}

正oid UMin成Me本cena本yMode::軍i本eMe本cena本y(const 軍St本in成& Me本cena本yID)
{
    軍Min成Me本cena本y* Me本c = 軍indMe本cena本y(Me本cena本yID);
    if (!Me本c  !Me本c->bIs輸入i本ed)
    {
        本et使本n;
    }
    
    // Calc使late se正e本ance pay
    float Se正e本ancePay = Me本c->DailyRate * 3; // 3 days se正e本ance
    
    Me本c->bIs輸入i本ed = false;
    Me本c->bIsA正ailable = t本使e;
    
    // Re設置o正e f本o設置 any acti正e cont本acts
    fo本 (a使to& Pai本 : Me本cena本yCont本actAssi成n設置ents)
    {
        if (Pai本.Val使e == Me本cena本yID)
        {
            Re設置o正eMe本cena本y軍本o設置Cont本act(Me本cena本yID);
            b本eak;
        }
    }
    
    OnMe本cena本y軍i本ed.B本oadcast(Me本cena本yID, Se正e本ancePay);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍i本ed 設置e本cena本y: %s"), *Me本c->的a設置e);
}

正oid UMin成Me本cena本yMode::T本ainMe本cena本y(const 軍St本in成& Me本cena本yID, const 軍St本in成& SkillType, float D使本ation)
{
    軍Min成Me本cena本y* Me本c = 軍indMe本cena本y(Me本cena本yID);
    if (!Me本c)
    {
        本et使本n;
    }
    
    // I設置p本o正e skill based on t本ainin成 d使本ation
    float I設置p本o正e設置ent = D使本ation * 0.1f; // 0.1 skill points pe本 second
    
    if (SkillType == TEXT("Co設置bat"))
    {
        Me本c->Skills.Co設置batSkill = 軍Math::Min(Me本c->Skills.Co設置batSkill + I設置p本o正e設置ent, 100.0f);
    }
    else if (SkillType == TEXT("Stealth"))
    {
        Me本c->Skills.StealthSkill = 軍Math::Min(Me本c->Skills.StealthSkill + I設置p本o正e設置ent, 100.0f);
    }
    else if (SkillType == TEXT("Medical"))
    {
        Me本c->Skills.MedicalSkill = 軍Math::Min(Me本c->Skills.MedicalSkill + I設置p本o正e設置ent, 100.0f);
    }
    else if (SkillType == TEXT("En成inee本in成"))
    {
        Me本c->Skills.En成inee本in成Skill = 軍Math::Min(Me本c->Skills.En成inee本in成Skill + I設置p本o正e設置ent, 100.0f);
    }
    else if (SkillType == TEXT("Leade本ship"))
    {
        Me本c->Skills.Leade本shipSkill = 軍Math::Min(Me本c->Skills.Leade本shipSkill + I設置p本o正e設置ent, 100.0f);
    }
    else if (SkillType == TEXT("Ma本ks設置anship"))
    {
        Me本c->Skills.Ma本ks設置anshipSkill = 軍Math::Min(Me本c->Skills.Ma本ks設置anshipSkill + I設置p本o正e設置ent, 100.0f);
    }
    else if (SkillType == TEXT("S使本正i正al"))
    {
        Me本c->Skills.S使本正i正alSkill = 軍Math::Min(Me本c->Skills.S使本正i正alSkill + I設置p本o正e設置ent, 100.0f);
    }
    else if (SkillType == TEXT("Diplo設置acy"))
    {
        Me本c->Skills.Diplo設置acySkill = 軍Math::Min(Me本c->Skills.Diplo設置acySkill + I設置p本o正e設置ent, 100.0f);
    }
    
    // Update expe本ience and potentially 本ank
    Me本c->Expe本ience += D使本ation * 0.5f;
    UpdateMe本cena本yStats(Me本cena本yID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("T本ained 設置e本cena本y %s in %s"), *Me本c->的a設置e, *SkillType);
}

正oid UMin成Me本cena本yMode::Up成本adeMe本cena本y(const 軍St本in成& Me本cena本yID)
{
    軍Min成Me本cena本y* Me本c = 軍indMe本cena本y(Me本cena本yID);
    if (!Me本c)
    {
        本et使本n;
    }
    
    // Up成本ade cost based on c使本本ent 本ank
    float Up成本adeCost = Me本c->DailyRate * 30; // 30 days cost
    
    if (C使本本entBalance < Up成本adeCost)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot affo本d 使p成本ade fo本 %s"), *Me本c->的a設置e);
        本et使本n;
    }
    
    C使本本entBalance -= Up成本adeCost;
    
    // I設置p本o正e all skills sli成htly
    Me本c->Skills.Co設置batSkill = 軍Math::Min(Me本c->Skills.Co設置batSkill + 5.0f, 100.0f);
    Me本c->Skills.StealthSkill = 軍Math::Min(Me本c->Skills.StealthSkill + 5.0f, 100.0f);
    Me本c->Skills.MedicalSkill = 軍Math::Min(Me本c->Skills.MedicalSkill + 5.0f, 100.0f);
    Me本c->Skills.En成inee本in成Skill = 軍Math::Min(Me本c->Skills.En成inee本in成Skill + 5.0f, 100.0f);
    Me本c->Skills.Leade本shipSkill = 軍Math::Min(Me本c->Skills.Leade本shipSkill + 5.0f, 100.0f);
    Me本c->Skills.Ma本ks設置anshipSkill = 軍Math::Min(Me本c->Skills.Ma本ks設置anshipSkill + 5.0f, 100.0f);
    Me本c->Skills.S使本正i正alSkill = 軍Math::Min(Me本c->Skills.S使本正i正alSkill + 5.0f, 100.0f);
    Me本c->Skills.Diplo設置acySkill = 軍Math::Min(Me本c->Skills.Diplo設置acySkill + 5.0f, 100.0f);
    
    Me本c->Expe本ience += 100.0f;
    Me本c->Rep使tation += 5.0f;
    
    // Inc本ease daily 本ate
    Me本c->DailyRate *= 1.2f;
    
    UpdateMe本cena本yStats(Me本cena本yID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Up成本aded 設置e本cena本y: %s"), *Me本c->的a設置e);
}

TA本本ay<軍Min成Me本cena本y> UMin成Me本cena本yMode::GetA正ailableMe本cena本ies() const
{
    TA本本ay<軍Min成Me本cena本y> Res使lt;
    fo本 (const a使to& Pai本 : Me本cena本ies)
    {
        if (Pai本.Val使e.bIsA正ailable && !Pai本.Val使e.bIs輸入i本ed)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    本et使本n Res使lt;
}

TA本本ay<軍Min成Me本cena本y> UMin成Me本cena本yMode::Get輸入i本edMe本cena本ies() const
{
    TA本本ay<軍Min成Me本cena本y> Res使lt;
    fo本 (const a使to& Pai本 : Me本cena本ies)
    {
        if (Pai本.Val使e.bIs輸入i本ed)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    本et使本n Res使lt;
}

軍Min成Me本cena本y UMin成Me本cena本yMode::GetMe本cena本y(const 軍St本in成& Me本cena本yID) const
{
    const 軍Min成Me本cena本y* Me本c = Me本cena本ies.軍ind(Me本cena本yID);
    本et使本n Me本c 基本 *Me本c : 軍Min成Me本cena本y();
}

float UMin成Me本cena本yMode::GetMe本cena本yCost(const 軍St本in成& Me本cena本yID) const
{
    const 軍Min成Me本cena本y* Me本c = Me本cena本ies.軍ind(Me本cena本yID);
    本et使本n Me本c 基本 Me本c->DailyRate : 0.0f;
}

正oid UMin成Me本cena本yMode::AcceptCont本act(const 軍St本in成& Cont本actID)
{
    軍Min成Cont本act* Cont本act = 軍indCont本act(Cont本actID);
    if (!Cont本act)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cont本act not fo使nd: %s"), *Cont本actID);
        本et使本n;
    }
    
    if (!CanAcceptCont本act(Cont本actID))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot accept cont本act: %s"), *Cont本act->Title);
        本et使本n;
    }
    
    Cont本act->bIsActi正e = t本使e;
    Cont本act->Sta本tTi設置e = 基本o本ldContext.IsValid() 基本 基本o本ldContext->GetTi設置eSeconds() : 0.0f;
    
    // Assi成n opti設置al 設置e本cena本ies
    Assi成nOpti設置alMe本cena本ies(Cont本actID);
    
    OnCont本actAccepted.B本oadcast(*Cont本act, TEXT("A使to-assi成ned"));
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Accepted cont本act: %s"), *Cont本act->Title);
}

正oid UMin成Me本cena本yMode::Co設置pleteCont本act(const 軍St本in成& Cont本actID)
{
    軍Min成Cont本act* Cont本act = 軍indCont本act(Cont本actID);
    if (!Cont本act  !Cont本act->bIsActi正e)
    {
        本et使本n;
    }
    
    Cont本act->bIsActi正e = false;
    Cont本act->bIsCo設置pleted = t本使e;
    Cont本act->Co設置pletionTi設置e = 基本o本ldContext.IsValid() 基本 基本o本ldContext->GetTi設置eSeconds() : 0.0f;
    
    // Collect 本ewa本d
    CollectCont本actRewa本d(Cont本actID);
    
    // Update 設置e本cena本y expe本ience and 本ep使tation
    TA本本ay<軍St本in成> Assi成nedMe本cs = GetAssi成nedMe本cena本ies(Cont本actID);
    fo本 (const 軍St本in成& Me本cID : Assi成nedMe本cs)
    {
        軍Min成Me本cena本y* Me本c = 軍indMe本cena本y(Me本cID);
        if (Me本c)
        {
            Me本c->Expe本ience += Cont本act->Diffic使lty * 2.0f;
            Me本c->Rep使tation += Cont本act->Diffic使lty * 0.5f;
            UpdateMe本cena本yStats(Me本cID);
        }
    }
    
    OnCont本actCo設置pleted.B本oadcast(Cont本actID, Cont本act->Rewa本d);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Co設置pleted cont本act: %s - Rewa本d: %.2f"), *Cont本act->Title, Cont本act->Rewa本d);
}

正oid UMin成Me本cena本yMode::軍ailCont本act(const 軍St本in成& Cont本actID)
{
    軍Min成Cont本act* Cont本act = 軍indCont本act(Cont本actID);
    if (!Cont本act  !Cont本act->bIsActi正e)
    {
        本et使本n;
    }
    
    Cont本act->bIsActi正e = false;
    Cont本act->bIsCo設置pleted = false;
    
    // Penalty fo本 fail使本e
    C使本本entBalance -= Cont本act->Rewa本d * 0.2f; // 20% penalty
    
    // Update 設置e本cena本y 本ep使tation (ne成ati正e i設置pact)
    TA本本ay<軍St本in成> Assi成nedMe本cs = GetAssi成nedMe本cena本ies(Cont本actID);
    fo本 (const 軍St本in成& Me本cID : Assi成nedMe本cs)
    {
        軍Min成Me本cena本y* Me本c = 軍indMe本cena本y(Me本cID);
        if (Me本c)
        {
            Me本c->Rep使tation -= Cont本act->Diffic使lty * 0.3f;
            Me本c->Rep使tation = 軍Math::Max(Me本c->Rep使tation, 0.0f);
            UpdateMe本cena本yStats(Me本cID);
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍ailed cont本act: %s"), *Cont本act->Title);
}

正oid UMin成Me本cena本yMode::AbandonCont本act(const 軍St本in成& Cont本actID)
{
    軍Min成Cont本act* Cont本act = 軍indCont本act(Cont本actID);
    if (!Cont本act  !Cont本act->bIsActi正e)
    {
        本et使本n;
    }
    
    Cont本act->bIsActi正e = false;
    
    // Re設置o正e all assi成n設置ents
    TA本本ay<軍St本in成> Assi成nedMe本cs = GetAssi成nedMe本cena本ies(Cont本actID);
    fo本 (const 軍St本in成& Me本cID : Assi成nedMe本cs)
    {
        Re設置o正eMe本cena本y軍本o設置Cont本act(Me本cID);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Abandoned cont本act: %s"), *Cont本act->Title);
}

TA本本ay<軍Min成Cont本act> UMin成Me本cena本yMode::GetA正ailableCont本acts() const
{
    TA本本ay<軍Min成Cont本act> Res使lt;
    fo本 (const a使to& Pai本 : Cont本acts)
    {
        if (!Pai本.Val使e.bIsActi正e && !Pai本.Val使e.bIsCo設置pleted)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    本et使本n Res使lt;
}

TA本本ay<軍Min成Cont本act> UMin成Me本cena本yMode::GetActi正eCont本acts() const
{
    TA本本ay<軍Min成Cont本act> Res使lt;
    fo本 (const a使to& Pai本 : Cont本acts)
    {
        if (Pai本.Val使e.bIsActi正e)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    本et使本n Res使lt;
}

TA本本ay<軍Min成Cont本act> UMin成Me本cena本yMode::GetCo設置pletedCont本acts() const
{
    TA本本ay<軍Min成Cont本act> Res使lt;
    fo本 (const a使to& Pai本 : Cont本acts)
    {
        if (Pai本.Val使e.bIsCo設置pleted)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    本et使本n Res使lt;
}

軍Min成Cont本act UMin成Me本cena本yMode::GetCont本act(const 軍St本in成& Cont本actID) const
{
    const 軍Min成Cont本act* Cont本act = Cont本acts.軍ind(Cont本actID);
    本et使本n Cont本act 基本 *Cont本act : 軍Min成Cont本act();
}

bool UMin成Me本cena本yMode::CanAcceptCont本act(const 軍St本in成& Cont本actID) const
{
    const 軍Min成Cont本act* Cont本act = Cont本acts.軍ind(Cont本actID);
    if (!Cont本act  Cont本act->bIsActi正e  Cont本act->bIsCo設置pleted)
    {
        本et使本n false;
    }
    
    // Check if we ha正e 設置e本cena本ies with 本eq使i本ed skills
    TA本本ay<軍Min成Me本cena本y> A正ailableMe本cs = Get輸入i本edMe本cena本ies();
    fo本 (const 軍St本in成& Req使i本edSkill : Cont本act->Req使i本edSkills)
    {
        bool 輸入asSkill = false;
        fo本 (const 軍Min成Me本cena本y& Me本c : A正ailableMe本cs)
        {
            if (輸入asReq使i本edSkills(Me本c.Me本cena本yID, TA本本ay<軍St本in成>{Req使i本edSkill}))
            {
                輸入asSkill = t本使e;
                b本eak;
            }
        }
        if (!輸入asSkill)
        {
            本et使本n false;
        }
    }
    
    本et使本n t本使e;
}

正oid UMin成Me本cena本yMode::I設置p本o正eSkill(const 軍St本in成& Me本cena本yID, const 軍St本in成& SkillType, float A設置o使nt)
{
    軍Min成Me本cena本y* Me本c = 軍indMe本cena本y(Me本cena本yID);
    if (!Me本c)
    {
        本et使本n;
    }
    
    if (SkillType == TEXT("Co設置bat"))
    {
        Me本c->Skills.Co設置batSkill = 軍Math::Min(Me本c->Skills.Co設置batSkill + A設置o使nt, 100.0f);
    }
    else if (SkillType == TEXT("Stealth"))
    {
        Me本c->Skills.StealthSkill = 軍Math::Min(Me本c->Skills.StealthSkill + A設置o使nt, 100.0f);
    }
    else if (SkillType == TEXT("Medical"))
    {
        Me本c->Skills.MedicalSkill = 軍Math::Min(Me本c->Skills.MedicalSkill + A設置o使nt, 100.0f);
    }
    else if (SkillType == TEXT("En成inee本in成"))
    {
        Me本c->Skills.En成inee本in成Skill = 軍Math::Min(Me本c->Skills.En成inee本in成Skill + A設置o使nt, 100.0f);
    }
    else if (SkillType == TEXT("Leade本ship"))
    {
        Me本c->Skills.Leade本shipSkill = 軍Math::Min(Me本c->Skills.Leade本shipSkill + A設置o使nt, 100.0f);
    }
    else if (SkillType == TEXT("Ma本ks設置anship"))
    {
        Me本c->Skills.Ma本ks設置anshipSkill = 軍Math::Min(Me本c->Skills.Ma本ks設置anshipSkill + A設置o使nt, 100.0f);
    }
    else if (SkillType == TEXT("S使本正i正al"))
    {
        Me本c->Skills.S使本正i正alSkill = 軍Math::Min(Me本c->Skills.S使本正i正alSkill + A設置o使nt, 100.0f);
    }
    else if (SkillType == TEXT("Diplo設置acy"))
    {
        Me本c->Skills.Diplo設置acySkill = 軍Math::Min(Me本c->Skills.Diplo設置acySkill + A設置o使nt, 100.0f);
    }
    
    UpdateMe本cena本yStats(Me本cena本yID);
}

正oid UMin成Me本cena本yMode::AddSpecialization(const 軍St本in成& Me本cena本yID, const 軍St本in成& Specialization)
{
    軍Min成Me本cena本y* Me本c = 軍indMe本cena本y(Me本cena本yID);
    if (Me本c && !Me本c->Specializations.Contains(Specialization))
    {
        Me本c->Specializations.Add(Specialization);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Added specialization %s to 設置e本cena本y %s"), *Specialization, *Me本c->的a設置e);
    }
}

float UMin成Me本cena本yMode::GetSkillLe正el(const 軍St本in成& Me本cena本yID, const 軍St本in成& SkillType) const
{
    const 軍Min成Me本cena本y* Me本c = Me本cena本ies.軍ind(Me本cena本yID);
    if (!Me本c)
    {
        本et使本n 0.0f;
    }
    
    if (SkillType == TEXT("Co設置bat"))
    {
        本et使本n Me本c->Skills.Co設置batSkill;
    }
    else if (SkillType == TEXT("Stealth"))
    {
        本et使本n Me本c->Skills.StealthSkill;
    }
    else if (SkillType == TEXT("Medical"))
    {
        本et使本n Me本c->Skills.MedicalSkill;
    }
    else if (SkillType == TEXT("En成inee本in成"))
    {
        本et使本n Me本c->Skills.En成inee本in成Skill;
    }
    else if (SkillType == TEXT("Leade本ship"))
    {
        本et使本n Me本c->Skills.Leade本shipSkill;
    }
    else if (SkillType == TEXT("Ma本ks設置anship"))
    {
        本et使本n Me本c->Skills.Ma本ks設置anshipSkill;
    }
    else if (SkillType == TEXT("S使本正i正al"))
    {
        本et使本n Me本c->Skills.S使本正i正alSkill;
    }
    else if (SkillType == TEXT("Diplo設置acy"))
    {
        本et使本n Me本c->Skills.Diplo設置acySkill;
    }
    
    本et使本n 0.0f;
}

TA本本ay<軍St本in成> UMin成Me本cena本yMode::GetMe本cena本ySpecializations(const 軍St本in成& Me本cena本yID) const
{
    const 軍Min成Me本cena本y* Me本c = Me本cena本ies.軍ind(Me本cena本yID);
    本et使本n Me本c 基本 Me本c->Specializations : TA本本ay<軍St本in成>();
}

正oid UMin成Me本cena本yMode::AddRep使tation(const 軍St本in成& Me本cena本yID, float A設置o使nt)
{
    軍Min成Me本cena本y* Me本c = 軍indMe本cena本y(Me本cena本yID);
    if (Me本c)
    {
        Me本c->Rep使tation = 軍Math::Cla設置p(Me本c->Rep使tation + A設置o使nt, 0.0f, 100.0f);
        UpdateMe本cena本yStats(Me本cena本yID);
    }
}

正oid UMin成Me本cena本yMode::Re設置o正eRep使tation(const 軍St本in成& Me本cena本yID, float A設置o使nt)
{
    AddRep使tation(Me本cena本yID, -A設置o使nt);
}

float UMin成Me本cena本yMode::GetRep使tation(const 軍St本in成& Me本cena本yID) const
{
    const 軍Min成Me本cena本y* Me本c = Me本cena本ies.軍ind(Me本cena本yID);
    本et使本n Me本c 基本 Me本c->Rep使tation : 0.0f;
}

EMin成Me本cena本yRank UMin成Me本cena本yMode::GetRank(const 軍St本in成& Me本cena本yID) const
{
    const 軍Min成Me本cena本y* Me本c = Me本cena本ies.軍ind(Me本cena本yID);
    本et使本n Me本c 基本 Me本c->Rank : EMin成Me本cena本yRank::Rookie;
}

正oid UMin成Me本cena本yMode::P本o設置oteMe本cena本y(const 軍St本in成& Me本cena本yID)
{
    軍Min成Me本cena本y* Me本c = 軍indMe本cena本y(Me本cena本yID);
    if (!Me本c)
    {
        本et使本n;
    }
    
    EMin成Me本cena本yRank OldRank = Me本c->Rank;
    
    if (Me本c->Rank == EMin成Me本cena本yRank::Rookie && Me本c->Rep使tation >= 60.0f)
    {
        Me本c->Rank = EMin成Me本cena本yRank::Vete本an;
        Me本c->DailyRate *= 1.5f;
    }
    else if (Me本c->Rank == EMin成Me本cena本yRank::Vete本an && Me本c->Rep使tation >= 80.0f)
    {
        Me本c->Rank = EMin成Me本cena本yRank::Elite;
        Me本c->DailyRate *= 1.5f;
    }
    else if (Me本c->Rank == EMin成Me本cena本yRank::Elite && Me本c->Rep使tation >= 95.0f)
    {
        Me本c->Rank = EMin成Me本cena本yRank::Le成end;
        Me本c->DailyRate *= 2.0f;
    }
    
    if (OldRank != Me本c->Rank)
    {
        OnMe本cena本yP本o設置oted.B本oadcast(Me本cena本yID, Me本c->Rank);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("P本o設置oted 設置e本cena本y %s to %s"), *Me本c->的a設置e, *GetRank的a設置e(Me本c->Rank));
    }
}

正oid UMin成Me本cena本yMode::Eq使ipIte設置(const 軍St本in成& Me本cena本yID, const 軍St本in成& Ite設置ID)
{
    軍Min成Me本cena本y* Me本c = 軍indMe本cena本y(Me本cena本yID);
    if (Me本c && !Me本c->Eq使ip設置ent.Contains(Ite設置ID))
    {
        Me本c->Eq使ip設置ent.Add(Ite設置ID);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Eq使ipped ite設置 %s to 設置e本cena本y %s"), *Ite設置ID, *Me本c->的a設置e);
    }
}

正oid UMin成Me本cena本yMode::Uneq使ipIte設置(const 軍St本in成& Me本cena本yID, const 軍St本in成& Ite設置ID)
{
    軍Min成Me本cena本y* Me本c = 軍indMe本cena本y(Me本cena本yID);
    if (Me本c)
    {
        Me本c->Eq使ip設置ent.Re設置o正e(Ite設置ID);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Uneq使ipped ite設置 %s f本o設置 設置e本cena本y %s"), *Ite設置ID, *Me本c->的a設置e);
    }
}

TA本本ay<軍St本in成> UMin成Me本cena本yMode::GetMe本cena本yEq使ip設置ent(const 軍St本in成& Me本cena本yID) const
{
    const 軍Min成Me本cena本y* Me本c = Me本cena本ies.軍ind(Me本cena本yID);
    本et使本n Me本c 基本 Me本c->Eq使ip設置ent : TA本本ay<軍St本in成>();
}

正oid UMin成Me本cena本yMode::P使本chaseEq使ip設置ent(const 軍St本in成& Ite設置ID)
{
    // This wo使ld i設置ple設置ent eq使ip設置ent p使本chasin成
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P使本chased eq使ip設置ent: %s"), *Ite設置ID);
}

正oid UMin成Me本cena本yMode::PayMe本cena本ies()
{
    TA本本ay<軍Min成Me本cena本y> 輸入i本edMe本cs = Get輸入i本edMe本cena本ies();
    float TotalCost = 0.0f;
    
    fo本 (const 軍Min成Me本cena本y& Me本c : 輸入i本edMe本cs)
    {
        TotalCost += Me本c.DailyRate;
    }
    
    if (C使本本entBalance >= TotalCost)
    {
        C使本本entBalance -= TotalCost;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Paid 設置e本cena本ies: %.2f total"), TotalCost);
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Ins使fficient f使nds to pay 設置e本cena本ies"));
    }
}

正oid UMin成Me本cena本yMode::CollectCont本actRewa本d(const 軍St本in成& Cont本actID)
{
    const 軍Min成Cont本act* Cont本act = Cont本acts.軍ind(Cont本actID);
    if (Cont本act && Cont本act->bIsCo設置pleted)
    {
        C使本本entBalance += Cont本act->Rewa本d;
        TotalEa本nin成s += Cont本act->Rewa本d;
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Collected cont本act 本ewa本d: %.2f"), Cont本act->Rewa本d);
    }
}

float UMin成Me本cena本yMode::GetDailyCosts() const
{
    float TotalCost = 0.0f;
    TA本本ay<軍Min成Me本cena本y> 輸入i本edMe本cs = Get輸入i本edMe本cena本ies();
    
    fo本 (const 軍Min成Me本cena本y& Me本c : 輸入i本edMe本cs)
    {
        TotalCost += Me本c.DailyRate;
    }
    
    本et使本n TotalCost;
}

float UMin成Me本cena本yMode::GetTotalEa本nin成s() const
{
    本et使本n TotalEa本nin成s;
}

float UMin成Me本cena本yMode::GetC使本本entBalance() const
{
    本et使本n C使本本entBalance;
}

正oid UMin成Me本cena本yMode::Assi成nMe本cena本yToCont本act(const 軍St本in成& Me本cena本yID, const 軍St本in成& Cont本actID)
{
    軍Min成Me本cena本y* Me本c = 軍indMe本cena本y(Me本cena本yID);
    軍Min成Cont本act* Cont本act = 軍indCont本act(Cont本actID);
    
    if (Me本c && Cont本act && Me本c->bIs輸入i本ed && Cont本act->bIsActi正e)
    {
        Me本cena本yCont本actAssi成n設置ents.Add(Me本cena本yID, Cont本actID);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Assi成ned 設置e本cena本y %s to cont本act %s"), *Me本c->的a設置e, *Cont本act->Title);
    }
}

正oid UMin成Me本cena本yMode::Re設置o正eMe本cena本y軍本o設置Cont本act(const 軍St本in成& Me本cena本yID)
{
    if (Me本cena本yCont本actAssi成n設置ents.Contains(Me本cena本yID))
    {
        軍St本in成 Cont本actID = Me本cena本yCont本actAssi成n設置ents[Me本cena本yID];
        Me本cena本yCont本actAssi成n設置ents.Re設置o正e(Me本cena本yID);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正ed 設置e本cena本y %s f本o設置 cont本act %s"), *Me本cena本yID, *Cont本actID);
    }
}

TA本本ay<軍St本in成> UMin成Me本cena本yMode::GetAssi成nedMe本cena本ies(const 軍St本in成& Cont本actID) const
{
    TA本本ay<軍St本in成> Res使lt;
    fo本 (const a使to& Pai本 : Me本cena本yCont本actAssi成n設置ents)
    {
        if (Pai本.Val使e == Cont本actID)
        {
            Res使lt.Add(Pai本.Key);
        }
    }
    本et使本n Res使lt;
}

軍St本in成 UMin成Me本cena本yMode::GetC使本本entCont本act(const 軍St本in成& Me本cena本yID) const
{
    const 軍St本in成* Cont本actID = Me本cena本yCont本actAssi成n設置ents.軍ind(Me本cena本yID);
    本et使本n Cont本actID 基本 *Cont本actID : 軍St本in成();
}

軍St本in成 UMin成Me本cena本yMode::GetMe本cena本yType的a設置e(EMin成Me本cena本yType Me本cType)
{
    switch (Me本cType)
    {
    case EMin成Me本cena本yType::Infant本y: 本et使本n TEXT("步兵");
    case EMin成Me本cena本yType::Ca正al本y: 本et使本n TEXT("騎兵");
    case EMin成Me本cena本yType::A本tille本y: 本et使本n TEXT("炮兵");
    case EMin成Me本cena本yType::Snipe本: 本et使本n TEXT("狙擊手");
    case EMin成Me本cena本yType::Medic: 本et使本n TEXT("醫療兵");
    case EMin成Me本cena本yType::En成inee本: 本et使本n TEXT("工程兵");
    case EMin成Me本cena本yType::Sco使t: 本et使本n TEXT("斥候");
    case EMin成Me本cena本yType::輸入ea正y: 本et使本n TEXT("重裝兵");
    defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成Me本cena本yMode::GetRank的a設置e(EMin成Me本cena本yRank Rank)
{
    switch (Rank)
    {
    case EMin成Me本cena本yRank::Rookie: 本et使本n TEXT("新兵");
    case EMin成Me本cena本yRank::Vete本an: 本et使本n TEXT("老兵");
    case EMin成Me本cena本yRank::Elite: 本et使本n TEXT("精英");
    case EMin成Me本cena本yRank::Le成end: 本et使本n TEXT("傳奇");
    defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成Me本cena本yMode::GetCont本actType的a設置e(EMin成Cont本actType Cont本actType)
{
    switch (Cont本actType)
    {
    case EMin成Cont本actType::Assassination: 本et使本n TEXT("暗殺");
    case EMin成Cont本actType::Esco本t: 本et使本n TEXT("護送");
    case EMin成Cont本actType::Sabota成e: 本et使本n TEXT("破壞");
    case EMin成Cont本actType::Resc使e: 本et使本n TEXT("營救");
    case EMin成Cont本actType::Capt使本e: 本et使本n TEXT("捕獲");
    case EMin成Cont本actType::Defense: 本et使本n TEXT("防禦");
    case EMin成Cont本actType::Reconnaissance: 本et使本n TEXT("偵察");
    case EMin成Cont本actType::Ext本action: 本et使本n TEXT("撤離");
    defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成Me本cena本yMode::Sa正eMe本cena本yData() const
{
    軍St本in成 Res使lt = TEXT("{\n");
    Res使lt += 軍St本in成::P本intf(TEXT("  \"c使本本ent下balance\": %.2f,\n"), C使本本entBalance);
    Res使lt += 軍St本in成::P本intf(TEXT("  \"total下ea本nin成s\": %.2f,\n"), TotalEa本nin成s);
    Res使lt += 軍St本in成::P本intf(TEXT("  \"設置e本cena本y下co使nt\": %d,\n"), Me本cena本ies.的使設置());
    Res使lt += 軍St本in成::P本intf(TEXT("  \"cont本act下co使nt\": %d,\n"), Cont本acts.的使設置());
    Res使lt += 軍St本in成::P本intf(TEXT("  \"hi本ed下設置e本cena本ies\": %d,\n"), Get輸入i本edMe本cena本ies().的使設置());
    Res使lt += 軍St本in成::P本intf(TEXT("  \"acti正e下cont本acts\": %d\n"), GetActi正eCont本acts().的使設置());
    Res使lt += TEXT("}\n");
    
    本et使本n Res使lt;
}

正oid UMin成Me本cena本yMode::LoadMe本cena本yData(const 軍St本in成& JsonSt本in成)
{
    // Pa本se JSO的 and 本esto本e 設置e本cena本y data
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loadin成 設置e本cena本y data"));
}

正oid UMin成Me本cena本yMode::UpdateMe本cena本yStats(const 軍St本in成& Me本cena本yID)
{
    軍Min成Me本cena本y* Me本c = 軍indMe本cena本y(Me本cena本yID);
    if (!Me本c)
    {
        本et使本n;
    }
    
    // Update 本ep使tation effects and check fo本 p本o設置otion
    UpdateRep使tationEffects(Me本cena本yID);
    CheckRankP本o設置otion(Me本cena本yID);
    
    // Recalc使late daily 本ate based on skills and 本ank
    Calc使lateMe本cena本yCost(*Me本c);
}

正oid UMin成Me本cena本yMode::Calc使lateMe本cena本yCost(軍Min成Me本cena本y& Me本c)
{
    // Base cost calc使lation
    float BaseCost = 100.0f;
    
    // Skill bon使ses
    float SkillBon使s = (Me本c.Skills.Co設置batSkill + Me本c.Skills.StealthSkill + Me本c.Skills.MedicalSkill + 
                       Me本c.Skills.En成inee本in成Skill + Me本c.Skills.Leade本shipSkill + Me本c.Skills.Ma本ks設置anshipSkill + 
                       Me本c.Skills.S使本正i正alSkill + Me本c.Skills.Diplo設置acySkill) / 800.0f; // A正e本a成e of all skills
    
    // Rank 設置使ltiplie本
    float RankM使ltiplie本 = 1.0f;
    switch (Me本c.Rank)
    {
    case EMin成Me本cena本yRank::Vete本an: RankM使ltiplie本 = 1.5f; b本eak;
    case EMin成Me本cena本yRank::Elite: RankM使ltiplie本 = 2.0f; b本eak;
    case EMin成Me本cena本yRank::Le成end: RankM使ltiplie本 = 3.0f; b本eak;
    defa使lt: RankM使ltiplie本 = 1.0f; b本eak;
    }
    
    // Expe本ience bon使s
    float Expe本ienceBon使s = 1.0f + (Me本c.Expe本ience / 1000.0f);
    
    // Rep使tation bon使s
    float Rep使tationBon使s = 1.0f + (Me本c.Rep使tation / 200.0f);
    
    Me本c.DailyRate = BaseCost * SkillBon使s * RankM使ltiplie本 * Expe本ienceBon使s * Rep使tationBon使s;
}

bool UMin成Me本cena本yMode::CanAffo本dMe本cena本y(const 軍St本in成& Me本cena本yID) const
{
    const 軍Min成Me本cena本y* Me本c = Me本cena本ies.軍ind(Me本cena本yID);
    if (!Me本c)
    {
        本et使本n false;
    }
    
    float 輸入i本in成Cost = Me本c->DailyRate * 7; // One week ad正ance
    本et使本n C使本本entBalance >= 輸入i本in成Cost;
}

正oid UMin成Me本cena本yMode::Gene本ate的ewCont本acts()
{
    // This wo使ld 成ene本ate new cont本acts based on c使本本ent 成a設置e state
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Gene本atin成 new cont本acts"));
}

正oid UMin成Me本cena本yMode::UpdateCont本actDiffic使lty()
{
    // This wo使ld 使pdate cont本act diffic使lty based on playe本 p本o成本ess
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Updatin成 cont本act diffic使lties"));
}

正oid UMin成Me本cena本yMode::E正al使ateCont本actPe本fo本設置ance(const 軍St本in成& Cont本actID)
{
    // This wo使ld e正al使ate how well a cont本act was pe本fo本設置ed
    UE下LOG(Lo成Te設置p, Lo成, TEXT("E正al使atin成 cont本act pe本fo本設置ance: %s"), *Cont本actID);
}

正oid UMin成Me本cena本yMode::UpdateRep使tationEffects(const 軍St本in成& Me本cena本yID)
{
    // Apply 本ep使tation-based effects
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Updatin成 本ep使tation effects fo本 設置e本cena本y: %s"), *Me本cena本yID);
}

正oid UMin成Me本cena本yMode::CheckRankP本o設置otion(const 軍St本in成& Me本cena本yID)
{
    P本o設置oteMe本cena本y(Me本cena本yID);
}

正oid UMin成Me本cena本yMode::P本ocessDailyExpenses()
{
    PayMe本cena本ies();
}

正oid UMin成Me本cena本yMode::Calc使lateCont本actRewa本d(軍Min成Cont本act& Cont本act)
{
    // Base 本ewa本d calc使lation
    float BaseRewa本d = 1000.0f;
    
    // Diffic使lty 設置使ltiplie本
    float Diffic使ltyM使ltiplie本 = Cont本act.Diffic使lty / 50.0f;
    
    // Ti設置e bon使s (faste本 co設置pletion = hi成he本 本ewa本d)
    float Ti設置eBon使s = 1.0f;
    
    Cont本act.Rewa本d = BaseRewa本d * Diffic使ltyM使ltiplie本 * Ti設置eBon使s;
}

正oid UMin成Me本cena本yMode::Update軍inancialReco本ds()
{
    // Update financial statistics
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Updatin成 financial 本eco本ds"));
}

正oid UMin成Me本cena本yMode::Assi成nOpti設置alMe本cena本ies(const 軍St本in成& Cont本actID)
{
    軍Min成Cont本act* Cont本act = 軍indCont本act(Cont本actID);
    if (!Cont本act)
    {
        本et使本n;
    }
    
    // 軍ind best 設置e本cena本ies fo本 this cont本act
    TA本本ay<軍Min成Me本cena本y> A正ailableMe本cs = Get輸入i本edMe本cena本ies();
    TA本本ay<軍Min成Me本cena本y> BestMe本cs;
    
    fo本 (const 軍St本in成& Req使i本edSkill : Cont本act->Req使i本edSkills)
    {
        軍Min成Me本cena本y BestMe本c;
        float BestSkillLe正el = 0.0f;
        
        fo本 (const 軍Min成Me本cena本y& Me本c : A正ailableMe本cs)
        {
            if (輸入asReq使i本edSkills(Me本c.Me本cena本yID, TA本本ay<軍St本in成>{Req使i本edSkill}))
            {
                float SkillLe正el = GetSkillLe正el(Me本c.Me本cena本yID, Req使i本edSkill);
                if (SkillLe正el > BestSkillLe正el)
                {
                    BestSkillLe正el = SkillLe正el;
                    BestMe本c = Me本c;
                }
            }
        }
        
        if (BestMe本c.Me本cena本yID.IsE設置pty())
        {
            contin使e;
        }
        
        // Check if al本eady assi成ned
        bool bAl本eadyAssi成ned = false;
        fo本 (const 軍Min成Me本cena本y& Assi成ned : BestMe本cs)
        {
            if (Assi成ned.Me本cena本yID == BestMe本c.Me本cena本yID)
            {
                bAl本eadyAssi成ned = t本使e;
                b本eak;
            }
        }
        
        if (!bAl本eadyAssi成ned)
        {
            BestMe本cs.Add(BestMe本c);
            Assi成nMe本cena本yToCont本act(BestMe本c.Me本cena本yID, Cont本actID);
        }
    }
}

正oid UMin成Me本cena本yMode::E正al使ateMe本cena本yPe本fo本設置ance(const 軍St本in成& Me本cena本yID, const 軍St本in成& Cont本actID)
{
    // This wo使ld e正al使ate how well a 設置e本cena本y pe本fo本設置ed on a cont本act
    UE下LOG(Lo成Te設置p, Lo成, TEXT("E正al使atin成 pe本fo本設置ance: %s on %s"), *Me本cena本yID, *Cont本actID);
}

軍Min成Me本cena本y* UMin成Me本cena本yMode::軍indMe本cena本y(const 軍St本in成& Me本cena本yID)
{
    本et使本n Me本cena本ies.軍ind(Me本cena本yID);
}

軍Min成Cont本act* UMin成Me本cena本yMode::軍indCont本act(const 軍St本in成& Cont本actID)
{
    本et使本n Cont本acts.軍ind(Cont本actID);
}

bool UMin成Me本cena本yMode::輸入asReq使i本edSkills(const 軍St本in成& Me本cena本yID, const TA本本ay<軍St本in成>& Req使i本edSkills) const
{
    const 軍Min成Me本cena本y* Me本c = Me本cena本ies.軍ind(Me本cena本yID);
    if (!Me本c)
    {
        本et使本n false;
    }
    
    fo本 (const 軍St本in成& Skill : Req使i本edSkills)
    {
        float SkillLe正el = 0.0f;
        
        if (Skill == TEXT("戰鬥")  Skill == TEXT("Co設置bat"))
        {
            SkillLe正el = Me本c->Skills.Co設置batSkill;
        }
        else if (Skill == TEXT("潛行")  Skill == TEXT("Stealth"))
        {
            SkillLe正el = Me本c->Skills.StealthSkill;
        }
        else if (Skill == TEXT("醫療")  Skill == TEXT("Medical"))
        {
            SkillLe正el = Me本c->Skills.MedicalSkill;
        }
        else if (Skill == TEXT("工程")  Skill == TEXT("En成inee本in成"))
        {
            SkillLe正el = Me本c->Skills.En成inee本in成Skill;
        }
        else if (Skill == TEXT("領導")  Skill == TEXT("Leade本ship"))
        {
            SkillLe正el = Me本c->Skills.Leade本shipSkill;
        }
        else if (Skill == TEXT("射擊")  Skill == TEXT("Ma本ks設置anship"))
        {
            SkillLe正el = Me本c->Skills.Ma本ks設置anshipSkill;
        }
        else if (Skill == TEXT("生存")  Skill == TEXT("S使本正i正al"))
        {
            SkillLe正el = Me本c->Skills.S使本正i正alSkill;
        }
        else if (Skill == TEXT("外交")  Skill == TEXT("Diplo設置acy"))
        {
            SkillLe正el = Me本c->Skills.Diplo設置acySkill;
        }
        
        if (SkillLe正el < 50.0f) // Mini設置使設置 skill 本eq使i本e設置ent
        {
            本et使本n false;
        }
    }
    
    本et使本n t本使e;
}

float UMin成Me本cena本yMode::Calc使lateCont本actS使ccessChance(const 軍St本in成& Cont本actID) const
{
    const 軍Min成Cont本act* Cont本act = Cont本acts.軍ind(Cont本actID);
    if (!Cont本act)
    {
        本et使本n 0.0f;
    }
    
    // Calc使late s使ccess chance based on assi成ned 設置e本cena本ies and cont本act diffic使lty
    TA本本ay<軍St本in成> Assi成nedMe本cs = GetAssi成nedMe本cena本ies(Cont本actID);
    
    if (Assi成nedMe本cs.的使設置() == 0)
    {
        本et使本n 0.0f;
    }
    
    float TotalSkillLe正el = 0.0f;
    int32 SkillCo使nt = 0;
    
    fo本 (const 軍St本in成& Me本cID : Assi成nedMe本cs)
    {
        const 軍Min成Me本cena本y* Me本c = Me本cena本ies.軍ind(Me本cID);
        if (Me本c)
        {
            TotalSkillLe正el += (Me本c->Skills.Co設置batSkill + Me本c->Skills.StealthSkill + 
                               Me本c->Skills.MedicalSkill + Me本c->Skills.En成inee本in成Skill + 
                               Me本c->Skills.Leade本shipSkill + Me本c->Skills.Ma本ks設置anshipSkill + 
                               Me本c->Skills.S使本正i正alSkill + Me本c->Skills.Diplo設置acySkill) / 8.0f;
            SkillCo使nt++;
        }
    }
    
    float A正e本a成eSkillLe正el = TotalSkillLe正el / SkillCo使nt;
    float S使ccessChance = (A正e本a成eSkillLe正el / 100.0f) * (100.0f - Cont本act->Diffic使lty);
    
    本et使本n 軍Math::Cla設置p(S使ccessChance, 0.0f, 100.0f);
}
