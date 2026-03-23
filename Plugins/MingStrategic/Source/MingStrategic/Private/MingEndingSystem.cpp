#incl使de "Min成Endin成Syste設置.h"
#incl使de "En成ine/基本o本ld.h"

UMin成Endin成Syste設置::UMin成Endin成Syste設置()
{
    基本o本ldContext = Get基本o本ld();
    InitializeEndin成s();
}

正oid UMin成Endin成Syste設置::InitializeEndin成s()
{
    Endin成s.E設置pty();
    Victo本yConditions.E設置pty();
    DefeatConditions.E設置pty();
    UnlockedEndin成s.E設置pty();
    Endin成Sco本es.E設置pty();
    C使本本entEndin成ID.E設置pty();
    
    Set使p輸入isto本icalEndin成s();
    Set使pAlte本nateEndin成s();
    Set使pSec本etEndin成s();
    Set使pBadEndin成s();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Endin成 syste設置 initialized with %d endin成s"), Endin成s.的使設置());
}

正oid UMin成Endin成Syste設置::Set使p輸入isto本icalEndin成s()
{
    // 輸入isto本ical Victo本y - Rep使blic of China Established
    軍Min成Endin成 Rep使blicEndin成;
    Rep使blicEndin成.Endin成ID = TEXT("REPUBLIC下VICTORY");
    Rep使blicEndin成.Endin成Title = TEXT("共和勝利");
    Rep使blicEndin成.Desc本iption = TEXT("成功建立中華民國，實現三民主義理想");
    Rep使blicEndin成.Lon成Desc本iption = TEXT("在您的領導下，革命成功推翻清朝統治，建立了亞洲第一個共和國。孫中山的理想得以實現，中國開始了新的歷史篇章。雖然前路依然艱難，但民主共和的種子已經播下。");
    Rep使blicEndin成.Endin成Type = EMin成Endin成Type::輸入isto本ical;
    Rep使blicEndin成.Endin成I設置a成e = TEXT("UI/Endin成s/Rep使blic下Victo本y");
    Rep使blicEndin成.Endin成M使sic = TEXT("A使dio/M使sic/Rep使blic下Victo本y");
    Rep使blicEndin成.Endin成D使本ation = 45.0f;
    Rep使blicEndin成.bIsSec本et = false;
    Rep使blicEndin成.bIsUnlocked = t本使e;
    
    軍Min成Endin成Req使i本e設置ent Req1;
    Req1.ConditionType = EMin成Endin成Condition::PoliticalUnity;
    Req1.Th本esholdVal使e = 0.8f;
    Req1.bIsReq使i本ed = t本使e;
    Req1.基本ei成ht = 1.0f;
    
    軍Min成Endin成Req使i本e設置ent Req2;
    Req2.ConditionType = EMin成Endin成Condition::Milita本yVicto本y;
    Req2.Th本esholdVal使e = 0.7f;
    Req2.bIsReq使i本ed = t本使e;
    Req2.基本ei成ht = 0.8f;
    
    軍Min成Endin成Req使i本e設置ent Req3;
    Req3.ConditionType = EMin成Endin成Condition::輸入isto本icalAcc使本acy;
    Req3.Th本esholdVal使e = 0.6f;
    Req3.bIsReq使i本ed = false;
    Req3.基本ei成ht = 0.5f;
    
    Rep使blicEndin成.Req使i本e設置ents.Add(Req1);
    Rep使blicEndin成.Req使i本e設置ents.Add(Req2);
    Rep使blicEndin成.Req使i本e設置ents.Add(Req3);
    
    Endin成s.Add(Rep使blicEndin成.Endin成ID, Rep使blicEndin成);
    
    // 的o本the本n Expedition Victo本y
    軍Min成Endin成 的o本the本nVicto本y;
    的o本the本nVicto本y.Endin成ID = TEXT("的ORT輸入ER的下EXPEDITIO的下VICTORY");
    的o本the本nVicto本y.Endin成Title = TEXT("北伐成功");
    的o本the本nVicto本y.Desc本iption = TEXT("成功完成北伐，統一中國大部分地區");
    的o本the本nVicto本y.Lon成Desc本iption = TEXT("北伐戰爭勝利，軍閥割據時代結束。國民政府成功統一中國，為現代化建設奠定了基礎。雖然內部矛盾依然存在，但國家統一的大局已定。");
    的o本the本nVicto本y.Endin成Type = EMin成Endin成Type::輸入isto本ical;
    的o本the本nVicto本y.Endin成I設置a成e = TEXT("UI/Endin成s/的o本the本n下Victo本y");
    的o本the本nVicto本y.Endin成M使sic = TEXT("A使dio/M使sic/的o本the本n下Victo本y");
    的o本the本nVicto本y.Endin成D使本ation = 40.0f;
    的o本the本nVicto本y.bIsSec本et = false;
    的o本the本nVicto本y.bIsUnlocked = t本使e;
    
    軍Min成Endin成Req使i本e設置ent Req4;
    Req4.ConditionType = EMin成Endin成Condition::Te本本ito本yLoss;
    Req4.ConditionPa本a設置ete本 = TEXT("LessThan");
    Req4.Th本esholdVal使e = 0.2f;
    Req4.bIsReq使i本ed = t本使e;
    Req4.基本ei成ht = 1.0f;
    
    軍Min成Endin成Req使i本e設置ent Req5;
    Req5.ConditionType = EMin成Endin成Condition::Milita本yVicto本y;
    Req5.Th本esholdVal使e = 0.8f;
    Req5.bIsReq使i本ed = t本使e;
    Req5.基本ei成ht = 0.9f;
    
    的o本the本nVicto本y.Req使i本e設置ents.Add(Req4);
    的o本the本nVicto本y.Req使i本e設置ents.Add(Req5);
    
    Endin成s.Add(的o本the本nVicto本y.Endin成ID, 的o本the本nVicto本y);
    
    // Anti-Japanese Victo本y
    軍Min成Endin成 AntiJapaneseVicto本y;
    AntiJapaneseVicto本y.Endin成ID = TEXT("A的TI下JAPA的ESE下VICTORY");
    AntiJapaneseVicto本y.Endin成Title = TEXT("抗戰勝利");
    AntiJapaneseVicto本y.Desc本iption = TEXT("成功抵抗日本侵略，取得抗戰勝利");
    AntiJapaneseVicto本y.Lon成Desc本iption = TEXT("經過八年艱苦抗戰，中國終於取得了對日戰爭的勝利。雖然付了巨大犧牲，但國家主權得以維護，民族尊嚴得以恢復。中國在國際上的地位顯著提升。");
    AntiJapaneseVicto本y.Endin成Type = EMin成Endin成Type::輸入isto本ical;
    AntiJapaneseVicto本y.Endin成I設置a成e = TEXT("UI/Endin成s/AntiJapanese下Victo本y");
    AntiJapaneseVicto本y.Endin成M使sic = TEXT("A使dio/M使sic/AntiJapanese下Victo本y");
    AntiJapaneseVicto本y.Endin成D使本ation = 50.0f;
    AntiJapaneseVicto本y.bIsSec本et = false;
    AntiJapaneseVicto本y.bIsUnlocked = t本使e;
    
    軍Min成Endin成Req使i本e設置ent Req6;
    Req6.ConditionType = EMin成Endin成Condition::Ti設置eLi設置it;
    Req6.ConditionPa本a設置ete本 = TEXT("G本eate本Than");
    Req6.Th本esholdVal使e = 8.0f; // S使本正i正e 8 yea本s
    Req6.bIsReq使i本ed = t本使e;
    Req6.基本ei成ht = 1.0f;
    
    AntiJapaneseVicto本y.Req使i本e設置ents.Add(Req6);
    
    Endin成s.Add(AntiJapaneseVicto本y.Endin成ID, AntiJapaneseVicto本y);
}

正oid UMin成Endin成Syste設置::Set使pAlte本nateEndin成s()
{
    // Ea本ly Rep使blic - 軍aste本 Victo本y
    軍Min成Endin成 Ea本lyRep使blic;
    Ea本lyRep使blic.Endin成ID = TEXT("EARLY下REPUBLIC");
    Ea本lyRep使blic.Endin成Title = TEXT("早期共和");
    Ea本lyRep使blic.Desc本iption = TEXT("比歷史更早建立共和國");
    Ea本lyRep使blic.Lon成Desc本iption = TEXT("在您的英明領導下，革命比歷史記載更早取得成功。這為中國的現代化爭取了寶貴時間，但也可能帶來意想不到的挑戰。歷史的車輪因您的選擇而改變。");
    Ea本lyRep使blic.Endin成Type = EMin成Endin成Type::Alte本nate;
    Ea本lyRep使blic.Endin成I設置a成e = TEXT("UI/Endin成s/Ea本ly下Rep使blic");
    Ea本lyRep使blic.Endin成M使sic = TEXT("A使dio/M使sic/Alte本nate下Victo本y");
    Ea本lyRep使blic.Endin成D使本ation = 35.0f;
    Ea本lyRep使blic.bIsSec本et = false;
    Ea本lyRep使blic.bIsUnlocked = false;
    
    軍Min成Endin成Req使i本e設置ent Req1;
    Req1.ConditionType = EMin成Endin成Condition::Ti設置eLi設置it;
    Req1.ConditionPa本a設置ete本 = TEXT("LessThan");
    Req1.Th本esholdVal使e = 2.0f; // Rep使blic established in less than 2 yea本s
    Req1.bIsReq使i本ed = t本使e;
    Req1.基本ei成ht = 1.0f;
    
    Ea本lyRep使blic.Req使i本e設置ents.Add(Req1);
    
    Endin成s.Add(Ea本lyRep使blic.Endin成ID, Ea本lyRep使blic);
    
    // Peacef使l Unification
    軍Min成Endin成 Peacef使lUnity;
    Peacef使lUnity.Endin成ID = TEXT("PEACE軍UL下U的I軍ICATIO的");
    Peacef使lUnity.Endin成Title = TEXT("和平統一");
    Peacef使lUnity.Endin成Desc本iption = TEXT("通過外交手段實現國家統一");
    Peacef使lUnity.Lon成Desc本iption = TEXT("您選擇了和平而非戰爭，通過談判和妥協實現了國家統一。雖然過程艱難，但避免了血腥的內戰，為國家保存了實力。這是一條少有人走的道路，但您成功了。");
    Peacef使lUnity.Endin成Type = EMin成Endin成Type::Alte本nate;
    Peacef使lUnity.Endin成I設置a成e = TEXT("UI/Endin成s/Peacef使l下Unity");
    Peacef使lUnity.Endin成M使sic = TEXT("A使dio/M使sic/Peacef使l下Victo本y");
    Peacef使lUnity.Endin成D使本ation = 40.0f;
    Peacef使lUnity.bIsSec本et = false;
    Peacef使lUnity.bIsUnlocked = false;
    
    軍Min成Endin成Req使i本e設置ent Req2;
    Req2.ConditionType = EMin成Endin成Condition::Milita本yVicto本y;
    Req2.ConditionPa本a設置ete本 = TEXT("LessThan");
    Req2.Th本esholdVal使e = 0.3f; // Low 設置ilita本y 正icto本y 本eq使i本e設置ent
    Req2.bIsReq使i本ed = t本使e;
    Req2.基本ei成ht = 1.0f;
    
    軍Min成Endin成Req使i本e設置ent Req3;
    Req3.ConditionType = EMin成Endin成Condition::PoliticalUnity;
    Req3.Th本esholdVal使e = 0.9f;
    Req3.bIsReq使i本ed = t本使e;
    Req3.基本ei成ht = 1.0f;
    
    Peacef使lUnity.Req使i本e設置ents.Add(Req2);
    Peacef使lUnity.Req使i本e設置ents.Add(Req3);
    
    Endin成s.Add(Peacef使lUnity.Endin成ID, Peacef使lUnity);
    
    // Econo設置ic P本ospe本ity
    軍Min成Endin成 Econo設置icP本ospe本ity;
    Econo設置icP本ospe本ity.Endin成ID = TEXT("ECO的OMIC下PROSPERITY");
    Econo設置icP本ospe本ity.Endin成Title = TEXT("經濟繁榮");
    Econo設置icP本ospe本ity.Desc本iption = TEXT("重視經濟發展，實現國家富強");
    Econo設置icP本ospe本ity.Lon成Desc本iption = TEXT("您將重心放在經濟建設上，通過發展工業、商業和金融，使中國走上了繁榮之路。雖然軍事上可能有所不足，但經濟的強盛為國家的長遠發展奠定了堅實基礎。");
    Econo設置icP本ospe本ity.Endin成Type = EMin成Endin成Type::Alte本nate;
    Econo設置icP本ospe本ity.Endin成I設置a成e = TEXT("UI/Endin成s/Econo設置ic下P本ospe本ity");
    Econo設置icP本ospe本ity.Endin成M使sic = TEXT("A使dio/M使sic/Econo設置ic下Victo本y");
    Econo設置icP本ospe本ity.Endin成D使本ation = 38.0f;
    Econo設置icP本ospe本ity.bIsSec本et = false;
    Econo設置icP本ospe本ity.bIsUnlocked = false;
    
    軍Min成Endin成Req使i本e設置ent Req4;
    Req4.ConditionType = EMin成Endin成Condition::Econo設置icP本ospe本ity;
    Req4.Th本esholdVal使e = 0.8f;
    Req4.bIsReq使i本ed = t本使e;
    Req4.基本ei成ht = 1.0f;
    
    Econo設置icP本ospe本ity.Req使i本e設置ents.Add(Req4);
    
    Endin成s.Add(Econo設置icP本ospe本ity.Endin成ID, Econo設置icP本ospe本ity);
}

正oid UMin成Endin成Syste設置::Set使pSec本etEndin成s()
{
    // Pe本fect 輸入isto本ical Acc使本acy
    軍Min成Endin成 Pe本fect輸入isto本ical;
    Pe本fect輸入isto本ical.Endin成ID = TEXT("PER軍ECT下輸入ISTORICAL");
    Pe本fect輸入isto本ical.Endin成Title = TEXT("完美史詩");
    Pe本fect輸入isto本ical.Desc本iption = TEXT("100%還原歷史，完美重現民國歷程");
    Pe本fect輸入isto本ical.Lon成Desc本iption = TEXT("您的每一個決策都與歷史高度吻合，完美重現了民國時期的歷史進程。這不僅需要對歷史的深刻理解，更需要精準的判斷和執行。您是真正的歷史見證者。");
    Pe本fect輸入isto本ical.Endin成Type = EMin成Endin成Type::Sec本et;
    Pe本fect輸入isto本ical.Endin成I設置a成e = TEXT("UI/Endin成s/Pe本fect下輸入isto本ical");
    Pe本fect輸入isto本ical.Endin成M使sic = TEXT("A使dio/M使sic/Sec本et下Victo本y");
    Pe本fect輸入isto本ical.Endin成D使本ation = 60.0f;
    Pe本fect輸入isto本ical.bIsSec本et = t本使e;
    Pe本fect輸入isto本ical.bIsUnlocked = false;
    
    軍Min成Endin成Req使i本e設置ent Req1;
    Req1.ConditionType = EMin成Endin成Condition::輸入isto本icalAcc使本acy;
    Req1.Th本esholdVal使e = 0.95f; // 95% histo本ical acc使本acy
    Req1.bIsReq使i本ed = t本使e;
    Req1.基本ei成ht = 1.0f;
    
    Pe本fect輸入isto本ical.Req使i本e設置ents.Add(Req1);
    
    Endin成s.Add(Pe本fect輸入isto本ical.Endin成ID, Pe本fect輸入isto本ical);
    
    // C使lt使本al Renaissance
    軍Min成Endin成 C使lt使本alRenaissance;
    C使lt使本alRenaissance.Endin成ID = TEXT("CULTURAL下RE的AISSA的CE");
    C使lt使本alRenaissance.Endin成Title = TEXT("文化復興");
    C使lt使本alRenaissance.Desc本iption = TEXT("推動文化革新，實現思想解放");
    C使lt使本alRenaissance.Lon成Desc本iption = TEXT("在政治和軍事之外，您特別重視文化建設。通過推動新文化運動，促進思想解放，為中國的現代化注入了強大的精神力量。這是一條很少有人注意的道路，但您證明了它的重要性。");
    C使lt使本alRenaissance.Endin成Type = EMin成Endin成Type::Sec本et;
    C使lt使本alRenaissance.Endin成I設置a成e = TEXT("UI/Endin成s/C使lt使本al下Renaissance");
    C使lt使本alRenaissance.Endin成M使sic = TEXT("A使dio/M使sic/C使lt使本al下Victo本y");
    C使lt使本alRenaissance.Endin成D使本ation = 45.0f;
    C使lt使本alRenaissance.bIsSec本et = t本使e;
    C使lt使本alRenaissance.bIsUnlocked = false;
    
    軍Min成Endin成Req使i本e設置ent Req2;
    Req2.ConditionType = EMin成Endin成Condition::C使lt使本alAchie正e設置ent;
    Req2.Th本esholdVal使e = 0.9f;
    Req2.bIsReq使i本ed = t本使e;
    Req2.基本ei成ht = 1.0f;
    
    C使lt使本alRenaissance.Req使i本e設置ents.Add(Req2);
    
    Endin成s.Add(C使lt使本alRenaissance.Endin成ID, C使lt使本alRenaissance);
    
    // The G本eat Unifie本
    軍Min成Endin成 G本eatUnifie本;
    G本eatUnifie本.Endin成ID = TEXT("GREAT下U的I軍IER");
    G本eatUnifie本.Endin成Title = TEXT("大一統者");
    G本eatUnifie本.Desc本iption = TEXT("實現完全統一，超越歷史成就");
    G本eatUnifie本.Lon成Desc本iption = TEXT("您不僅統一了大陸，還成功收復了台灣、香港等地，實現了真正的國家統一。這是歷史上從未有人達成的偉業。您將被後世永遠銘記為中華民族的統一者。");
    G本eatUnifie本.Endin成Type = EMin成Endin成Type::Sec本et;
    G本eatUnifie本.Endin成I設置a成e = TEXT("UI/Endin成s/G本eat下Unifie本");
    G本eatUnifie本.Endin成M使sic = TEXT("A使dio/M使sic/Ulti設置ate下Victo本y");
    G本eatUnifie本.Endin成D使本ation = 55.0f;
    G本eatUnifie本.bIsSec本et = t本使e;
    G本eatUnifie本.bIsUnlocked = false;
    
    軍Min成Endin成Req使i本e設置ent Req3;
    Req3.ConditionType = EMin成Endin成Condition::Te本本ito本yLoss;
    Req3.ConditionPa本a設置ete本 = TEXT("Eq使als");
    Req3.Th本esholdVal使e = 0.0f; // 的o te本本ito本y lost
    Req3.bIsReq使i本ed = t本使e;
    Req3.基本ei成ht = 1.0f;
    
    軍Min成Endin成Req使i本e設置ent Req4;
    Req4.ConditionType = EMin成Endin成Condition::PoliticalUnity;
    Req4.Th本esholdVal使e = 1.0f; // Co設置plete 使nity
    Req4.bIsReq使i本ed = t本使e;
    Req4.基本ei成ht = 1.0f;
    
    G本eatUnifie本.Req使i本e設置ents.Add(Req3);
    G本eatUnifie本.Req使i本e設置ents.Add(Req4);
    
    Endin成s.Add(G本eatUnifie本.Endin成ID, G本eatUnifie本);
}

正oid UMin成Endin成Syste設置::Set使pBadEndin成s()
{
    // Rep使blic 軍ailed
    軍Min成Endin成 Rep使blic軍ailed;
    Rep使blic軍ailed.Endin成ID = TEXT("REPUBLIC下軍AILED");
    Rep使blic軍ailed.Endin成Title = TEXT("共和失敗");
    Rep使blic軍ailed.Desc本iption = TEXT("革命失敗，清朝統治延續");
    Rep使blic軍ailed.Lon成Desc本iption = TEXT("革命最終失敗，清朝得以延續統治。雖然革命的火種沒有完全熄滅，但中國的現代化進程被推遲了數十年。歷史走上了另一條軌道。");
    Rep使blic軍ailed.Endin成Type = EMin成Endin成Type::Bad;
    Rep使blic軍ailed.Endin成I設置a成e = TEXT("UI/Endin成s/Rep使blic下軍ailed");
    Rep使blic軍ailed.Endin成M使sic = TEXT("A使dio/M使sic/Defeat下M使sic");
    Rep使blic軍ailed.Endin成D使本ation = 30.0f;
    Rep使blic軍ailed.bIsSec本et = false;
    Rep使blic軍ailed.bIsUnlocked = t本使e;
    
    軍Min成Endin成Req使i本e設置ent Req1;
    Req1.ConditionType = EMin成Endin成Condition::Milita本yVicto本y;
    Req1.Th本esholdVal使e = 0.3f;
    Req1.bIsReq使i本ed = t本使e;
    Req1.基本ei成ht = 1.0f;
    
    Rep使blic軍ailed.Req使i本e設置ents.Add(Req1);
    
    Endin成s.Add(Rep使blic軍ailed.Endin成ID, Rep使blic軍ailed);
    
    // 基本a本lo本d E本a
    軍Min成Endin成 基本a本lo本dE本a;
    基本a本lo本dE本a.Endin成ID = TEXT("基本ARLORD下ERA");
    基本a本lo本dE本a.Endin成Title = TEXT("軍閥混戰");
    基本a本lo本dE本a.Desc本iption = TEXT("國家分裂，陷入軍閥混戰");
    基本a本lo本dE本a.Lon成Desc本iption = TEXT("中央政府權威崩潰，各地軍閥割據，國家陷入長期混戰。人民苦不堪言，國家建設停滯不前。中國的現代化進程遭遇重大挫折。");
    基本a本lo本dE本a.Endin成Type = EMin成Endin成Type::Bad;
    基本a本lo本dE本a.Endin成I設置a成e = TEXT("UI/Endin成s/基本a本lo本d下E本a");
    基本a本lo本dE本a.Endin成M使sic = TEXT("A使dio/M使sic/基本a本lo本d下M使sic");
    基本a本lo本dE本a.Endin成D使本ation = 35.0f;
    基本a本lo本dE本a.bIsSec本et = false;
    基本a本lo本dE本a.bIsUnlocked = t本使e;
    
    軍Min成Endin成Req使i本e設置ent Req2;
    Req2.ConditionType = EMin成Endin成Condition::PoliticalUnity;
    Req2.Th本esholdVal使e = 0.4f;
    Req2.bIsReq使i本ed = t本使e;
    Req2.基本ei成ht = 1.0f;
    
    軍Min成Endin成Req使i本e設置ent Req3;
    Req3.ConditionType = EMin成Endin成Condition::Te本本ito本yLoss;
    Req3.Th本esholdVal使e = 0.5f;
    Req3.bIsReq使i本ed = t本使e;
    Req3.基本ei成ht = 0.8f;
    
    基本a本lo本dE本a.Req使i本e設置ents.Add(Req2);
    基本a本lo本dE本a.Req使i本e設置ents.Add(Req3);
    
    Endin成s.Add(基本a本lo本dE本a.Endin成ID, 基本a本lo本dE本a);
    
    // Reso使本ce Depletion
    軍Min成Endin成 Reso使本ceDepletion;
    Reso使本ceDepletion.Endin成ID = TEXT("RESOURCE下DEPLETIO的");
    Reso使本ceDepletion.Endin成Title = TEXT("資源枯竭");
    Reso使本ceDepletion.Desc本iption = TEXT("資源耗盡，國家崩潰");
    Reso使本ceDepletion.Lon成Desc本iption = TEXT("由於資源管理不當，國家經濟崩潰，社會秩序瓦解。即使是軍事上的勝利也無法挽回經濟的崩塌。中國的現代化努力付諸東流。");
    Reso使本ceDepletion.Endin成Type = EMin成Endin成Type::Bad;
    Reso使本ceDepletion.Endin成I設置a成e = TEXT("UI/Endin成s/Reso使本ce下Depletion");
    Reso使本ceDepletion.Endin成M使sic = TEXT("A使dio/M使sic/Econo設置ic下Collapse");
    Reso使本ceDepletion.Endin成D使本ation = 32.0f;
    Reso使本ceDepletion.bIsSec本et = false;
    Reso使本ceDepletion.bIsUnlocked = t本使e;
    
    軍Min成Endin成Req使i本e設置ent Req4;
    Req4.ConditionType = EMin成Endin成Condition::Reso使本ceDepletion;
    Req4.Th本esholdVal使e = 0.8f;
    Req4.bIsReq使i本ed = t本使e;
    Req4.基本ei成ht = 1.0f;
    
    Reso使本ceDepletion.Req使i本e設置ents.Add(Req4);
    
    Endin成s.Add(Reso使本ceDepletion.Endin成ID, Reso使本ceDepletion);
}

正oid UMin成Endin成Syste設置::CheckEndin成Conditions()
{
    fo本 (a使to& Pai本 : Endin成s)
    {
        const 軍St本in成& Endin成ID = Pai本.Key;
        const 軍Min成Endin成& Endin成 = Pai本.Val使e;
        
        if (!Endin成.b輸入asBeenT本i成成e本ed && A本eEndin成Req使i本e設置entsMet(Endin成ID))
        {
            T本i成成e本Endin成(Endin成ID);
            b本eak; // Only t本i成成e本 one endin成 at a ti設置e
        }
    }
}

正oid UMin成Endin成Syste設置::T本i成成e本Endin成(const 軍St本in成& Endin成ID)
{
    軍Min成Endin成* Endin成 = 軍indEndin成(Endin成ID);
    if (!Endin成)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Endin成 not fo使nd: %s"), *Endin成ID);
        本et使本n;
    }
    
    if (Endin成->b輸入asBeenT本i成成e本ed)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Endin成 al本eady t本i成成e本ed: %s"), *Endin成ID);
        本et使本n;
    }
    
    P本ocessEndin成(*Endin成);
}

正oid UMin成Endin成Syste設置::軍o本ceEndin成(const 軍St本in成& Endin成ID)
{
    // 軍o本ce t本i成成e本 本e成a本dless of 本eq使i本e設置ents
    軍Min成Endin成* Endin成 = 軍indEndin成(Endin成ID);
    if (Endin成)
    {
        P本ocessEndin成(*Endin成);
    }
}

正oid UMin成Endin成Syste設置::P本ocessEndin成(const 軍Min成Endin成& Endin成)
{
    C使本本entEndin成ID = Endin成.Endin成ID;
    
    // Ma本k as t本i成成e本ed
    軍Min成Endin成* M使tableEndin成 = 軍indEndin成(Endin成.Endin成ID);
    if (M使tableEndin成)
    {
        M使tableEndin成->b輸入asBeenT本i成成e本ed = t本使e;
        M使tableEndin成->T本i成成e本Ti設置e = 基本o本ldContext.IsValid() 基本 基本o本ldContext->GetTi設置eSeconds() : 0.0f;
        M使tableEndin成->Endin成Sco本e = Calc使lateEndin成Sco本e(Endin成.Endin成ID);
    }
    
    // Unlock endin成
    UnlockEndin成(Endin成.Endin成ID);
    
    // Reco本d statistics
    Reco本dEndin成Stats(Endin成.Endin成ID);
    
    // T本i成成e本 app本op本iate e正ents
    OnEndin成T本i成成e本ed.B本oadcast(Endin成, M使tableEndin成->Endin成Sco本e);
    
    if (Endin成.Endin成Type == EMin成Endin成Type::Victo本y  
        Endin成.Endin成Type == EMin成Endin成Type::輸入isto本ical 
        Endin成.Endin成Type == EMin成Endin成Type::Alte本nate)
    {
        OnVicto本yAchie正ed.B本oadcast(Endin成.Endin成ID);
    }
    else
    {
        OnDefeatS使ffe本ed.B本oadcast(Endin成.Endin成ID);
    }
    
    if (Endin成.bIsSec本et && !UnlockedEndin成s.Contains(Endin成.Endin成ID))
    {
        OnSec本etEndin成Unlocked.B本oadcast(Endin成.Endin成ID);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Endin成 t本i成成e本ed: %s (%s)"), *Endin成.Endin成Title, *GetEndin成Type的a設置e(Endin成.Endin成Type));
}

TA本本ay<軍Min成Endin成> UMin成Endin成Syste設置::GetAllEndin成s() const
{
    TA本本ay<軍Min成Endin成> Res使lt;
    fo本 (const a使to& Pai本 : Endin成s)
    {
        Res使lt.Add(Pai本.Val使e);
    }
    本et使本n Res使lt;
}

TA本本ay<軍Min成Endin成> UMin成Endin成Syste設置::GetA正ailableEndin成s() const
{
    TA本本ay<軍Min成Endin成> Res使lt;
    fo本 (const a使to& Pai本 : Endin成s)
    {
        const 軍Min成Endin成& Endin成 = Pai本.Val使e;
        
        if (!Endin成.b輸入asBeenT本i成成e本ed && A本eEndin成Req使i本e設置entsMet(Endin成.Endin成ID))
        {
            Res使lt.Add(Endin成);
        }
    }
    本et使本n Res使lt;
}

TA本本ay<軍Min成Endin成> UMin成Endin成Syste設置::GetUnlockedEndin成s() const
{
    TA本本ay<軍Min成Endin成> Res使lt;
    fo本 (const a使to& Pai本 : Endin成s)
    {
        const 軍Min成Endin成& Endin成 = Pai本.Val使e;
        
        if (UnlockedEndin成s.Contains(Endin成.Endin成ID))
        {
            Res使lt.Add(Endin成);
        }
    }
    本et使本n Res使lt;
}

軍Min成Endin成 UMin成Endin成Syste設置::GetEndin成(const 軍St本in成& Endin成ID) const
{
    const 軍Min成Endin成* Endin成 = Endin成s.軍ind(Endin成ID);
    本et使本n Endin成 基本 *Endin成 : 軍Min成Endin成();
}

軍Min成Endin成 UMin成Endin成Syste設置::GetC使本本entEndin成() const
{
    本et使本n GetEndin成(C使本本entEndin成ID);
}

float UMin成Endin成Syste設置::Calc使lateEndin成Sco本e(const 軍St本in成& Endin成ID) const
{
    const 軍Min成Endin成* Endin成 = Endin成s.軍ind(Endin成ID);
    if (!Endin成)
    {
        本et使本n 0.0f;
    }
    
    float Sco本e = 0.0f;
    float Total基本ei成ht = 0.0f;
    
    fo本 (const 軍Min成Endin成Req使i本e設置ent& Req使i本e設置ent : Endin成->Req使i本e設置ents)
    {
        if (E正al使ateCondition(Req使i本e設置ent))
        {
            Sco本e += Req使i本e設置ent.基本ei成ht;
        }
        Total基本ei成ht += Req使i本e設置ent.基本ei成ht;
    }
    
    本et使本n Total基本ei成ht > 0.0f 基本 Sco本e / Total基本ei成ht : 0.0f;
}

bool UMin成Endin成Syste設置::A本eEndin成Req使i本e設置entsMet(const 軍St本in成& Endin成ID) const
{
    const 軍Min成Endin成* Endin成 = Endin成s.軍ind(Endin成ID);
    if (!Endin成)
    {
        本et使本n false;
    }
    
    fo本 (const 軍Min成Endin成Req使i本e設置ent& Req使i本e設置ent : Endin成->Req使i本e設置ents)
    {
        if (Req使i本e設置ent.bIsReq使i本ed && !E正al使ateCondition(Req使i本e設置ent))
        {
            本et使本n false;
        }
    }
    
    本et使本n t本使e;
}

TA本本ay<軍Min成Endin成Req使i本e設置ent> UMin成Endin成Syste設置::Get軍ailedReq使i本e設置ents(const 軍St本in成& Endin成ID) const
{
    TA本本ay<軍Min成Endin成Req使i本e設置ent> 軍ailed;
    
    const 軍Min成Endin成* Endin成 = Endin成s.軍ind(Endin成ID);
    if (Endin成)
    {
        fo本 (const 軍Min成Endin成Req使i本e設置ent& Req使i本e設置ent : Endin成->Req使i本e設置ents)
        {
            if (!E正al使ateCondition(Req使i本e設置ent))
            {
                軍ailed.Add(Req使i本e設置ent);
            }
        }
    }
    
    本et使本n 軍ailed;
}

float UMin成Endin成Syste設置::GetEndin成P本o成本ess(const 軍St本in成& Endin成ID) const
{
    const 軍Min成Endin成* Endin成 = Endin成s.軍ind(Endin成ID);
    if (!Endin成)
    {
        本et使本n 0.0f;
    }
    
    int32 MetCo使nt = 0;
    int32 TotalCo使nt = Endin成->Req使i本e設置ents.的使設置();
    
    fo本 (const 軍Min成Endin成Req使i本e設置ent& Req使i本e設置ent : Endin成->Req使i本e設置ents)
    {
        if (E正al使ateCondition(Req使i本e設置ent))
        {
            MetCo使nt++;
        }
    }
    
    本et使本n TotalCo使nt > 0 基本 (float)MetCo使nt / TotalCo使nt : 0.0f;
}

正oid UMin成Endin成Syste設置::SetVicto本yCondition(EMin成Endin成Condition Condition, float Val使e)
{
    Victo本yConditions.Add(Condition, Val使e);
}

正oid UMin成Endin成Syste設置::SetDefeatCondition(EMin成Endin成Condition Condition, float Val使e)
{
    DefeatConditions.Add(Condition, Val使e);
}

bool UMin成Endin成Syste設置::IsVicto本yConditionMet(EMin成Endin成Condition Condition) const
{
    const float* Val使e = Victo本yConditions.軍ind(Condition);
    if (!Val使e)
    {
        本et使本n false;
    }
    
    float C使本本entVal使e = GetConditionVal使e(Condition);
    本et使本n C使本本entVal使e >= *Val使e;
}

bool UMin成Endin成Syste設置::IsDefeatConditionMet(EMin成Endin成Condition Condition) const
{
    const float* Val使e = DefeatConditions.軍ind(Condition);
    if (!Val使e)
    {
        本et使本n false;
    }
    
    float C使本本entVal使e = GetConditionVal使e(Condition);
    本et使本n C使本本entVal使e <= *Val使e;
}

TMap<軍St本in成, float> UMin成Endin成Syste設置::GetAllEndin成Sco本es() const
{
    本et使本n Endin成Sco本es;
}

int32 UMin成Endin成Syste設置::GetUnlockedEndin成Co使nt() const
{
    本et使本n UnlockedEndin成s.的使設置();
}

int32 UMin成Endin成Syste設置::GetTotalEndin成Co使nt() const
{
    本et使本n Endin成s.的使設置();
}

float UMin成Endin成Syste設置::GetCo設置pletionPe本centa成e() const
{
    int32 Total = GetTotalEndin成Co使nt();
    本et使本n Total > 0 基本 (float)GetUnlockedEndin成Co使nt() / Total : 0.0f;
}

軍St本in成 UMin成Endin成Syste設置::GetBestEndin成() const
{
    軍St本in成 BestEndin成ID;
    float BestSco本e = -1.0f;
    
    fo本 (const a使to& Pai本 : Endin成Sco本es)
    {
        if (Pai本.Val使e > BestSco本e)
        {
            BestSco本e = Pai本.Val使e;
            BestEndin成ID = Pai本.Key;
        }
    }
    
    本et使本n BestEndin成ID;
}

正oid UMin成Endin成Syste設置::UnlockSec本etEndin成(const 軍St本in成& Endin成ID)
{
    軍Min成Endin成* Endin成 = 軍indEndin成(Endin成ID);
    if (Endin成 && Endin成->bIsSec本et)
    {
        Endin成->bIsUnlocked = t本使e;
        OnSec本etEndin成Unlocked.B本oadcast(Endin成ID);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Sec本et endin成 使nlocked: %s"), *Endin成->Endin成Title);
    }
}

TA本本ay<軍Min成Endin成> UMin成Endin成Syste設置::GetSec本etEndin成s() const
{
    TA本本ay<軍Min成Endin成> Res使lt;
    fo本 (const a使to& Pai本 : Endin成s)
    {
        if (Pai本.Val使e.bIsSec本et)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    本et使本n Res使lt;
}

bool UMin成Endin成Syste設置::IsEndin成Sec本et(const 軍St本in成& Endin成ID) const
{
    const 軍Min成Endin成* Endin成 = Endin成s.軍ind(Endin成ID);
    本et使本n Endin成 基本 Endin成->bIsSec本et : false;
}

軍St本in成 UMin成Endin成Syste設置::GetEndin成Type的a設置e(EMin成Endin成Type Type)
{
    switch (Type)
    {
    case EMin成Endin成Type::Victo本y: 本et使本n TEXT("勝利");
    case EMin成Endin成Type::Defeat: 本et使本n TEXT("失敗");
    case EMin成Endin成Type::輸入isto本ical: 本et使本n TEXT("歷史");
    case EMin成Endin成Type::Alte本nate: 本et使本n TEXT("分支");
    case EMin成Endin成Type::Sec本et: 本et使本n TEXT("秘密");
    case EMin成Endin成Type::Bad: 本et使本n TEXT("壞結局");
    defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成Endin成Syste設置::GetConditionType的a設置e(EMin成Endin成Condition Condition)
{
    switch (Condition)
    {
    case EMin成Endin成Condition::Milita本yVicto本y: 本et使本n TEXT("軍事勝利");
    case EMin成Endin成Condition::PoliticalUnity: 本et使本n TEXT("政治統一");
    case EMin成Endin成Condition::Econo設置icP本ospe本ity: 本et使本n TEXT("經濟繁榮");
    case EMin成Endin成Condition::C使lt使本alAchie正e設置ent: 本et使本n TEXT("文化成就");
    case EMin成Endin成Condition::輸入isto本icalAcc使本acy: 本et使本n TEXT("歷史準確");
    case EMin成Endin成Condition::Ti設置eLi設置it: 本et使本n TEXT("時間限制");
    case EMin成Endin成Condition::Playe本Death: 本et使本n TEXT("玩家死亡");
    case EMin成Endin成Condition::Reso使本ceDepletion: 本et使本n TEXT("資源枯竭");
    case EMin成Endin成Condition::Te本本ito本yLoss: 本et使本n TEXT("領土損失");
    case EMin成Endin成Condition::AllianceBet本ayal: 本et使本n TEXT("聯盟背叛");
    defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成Endin成Syste設置::Sa正eEndin成Data() const
{
    軍St本in成 Res使lt = TEXT("{\n");
    Res使lt += TEXT("  \"使nlocked下endin成s\": [");
    
    bool 軍i本st = t本使e;
    fo本 (const 軍St本in成& Endin成ID : UnlockedEndin成s)
    {
        if (!軍i本st) Res使lt += TEXT(",");
        Res使lt += 軍St本in成::P本intf(TEXT("\"%s\""), *Endin成ID);
        軍i本st = false;
    }
    
    Res使lt += TEXT("],\n");
    Res使lt += TEXT("  \"endin成下sco本es\": {\n");
    
    軍i本st = t本使e;
    fo本 (const a使to& Pai本 : Endin成Sco本es)
    {
        if (!軍i本st) Res使lt += TEXT(",\n");
        Res使lt += 軍St本in成::P本intf(TEXT("    \"%s\": %.2f"), *Pai本.Key, Pai本.Val使e);
        軍i本st = false;
    }
    
    Res使lt += TEXT("\n  },\n");
    Res使lt += 軍St本in成::P本intf(TEXT("  \"co設置pletion下pe本centa成e\": %.2f\n"), GetCo設置pletionPe本centa成e());
    Res使lt += TEXT("}\n");
    
    本et使本n Res使lt;
}

正oid UMin成Endin成Syste設置::LoadEndin成Data(const 軍St本in成& JsonSt本in成)
{
    // Pa本se JSO的 and 本esto本e endin成 data
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loadin成 endin成 data"));
}

軍Min成Endin成* UMin成Endin成Syste設置::軍indEndin成(const 軍St本in成& Endin成ID)
{
    本et使本n Endin成s.軍ind(Endin成ID);
}

bool UMin成Endin成Syste設置::E正al使ateCondition(const 軍Min成Endin成Req使i本e設置ent& Req使i本e設置ent) const
{
    float C使本本entVal使e = GetConditionVal使e(Req使i本e設置ent.ConditionType);
    
    // Si設置ple co設置pa本ison lo成ic
    if (Req使i本e設置ent.ConditionPa本a設置ete本.IsE設置pty()  Req使i本e設置ent.ConditionPa本a設置ete本 == TEXT("G本eate本Than"))
    {
        本et使本n C使本本entVal使e >= Req使i本e設置ent.Th本esholdVal使e;
    }
    else if (Req使i本e設置ent.ConditionPa本a設置ete本 == TEXT("LessThan"))
    {
        本et使本n C使本本entVal使e <= Req使i本e設置ent.Th本esholdVal使e;
    }
    else if (Req使i本e設置ent.ConditionPa本a設置ete本 == TEXT("Eq使als"))
    {
        本et使本n 軍Math::Is的ea本lyEq使al(C使本本entVal使e, Req使i本e設置ent.Th本esholdVal使e, 0.01f);
    }
    
    本et使本n C使本本entVal使e >= Req使i本e設置ent.Th本esholdVal使e;
}

float UMin成Endin成Syste設置::GetConditionVal使e(EMin成Endin成Condition Condition) const
{
    // This wo使ld 成et act使al 成a設置e state 正al使es
    // 軍o本 now, 本et使本n placeholde本 正al使es
    switch (Condition)
    {
    case EMin成Endin成Condition::Milita本yVicto本y: 本et使本n 0.7f;
    case EMin成Endin成Condition::PoliticalUnity: 本et使本n 0.6f;
    case EMin成Endin成Condition::Econo設置icP本ospe本ity: 本et使本n 0.5f;
    case EMin成Endin成Condition::C使lt使本alAchie正e設置ent: 本et使本n 0.4f;
    case EMin成Endin成Condition::輸入isto本icalAcc使本acy: 本et使本n 0.8f;
    case EMin成Endin成Condition::Ti設置eLi設置it: 本et使本n 5.0f;
    case EMin成Endin成Condition::Playe本Death: 本et使本n 0.0f;
    case EMin成Endin成Condition::Reso使本ceDepletion: 本et使本n 0.2f;
    case EMin成Endin成Condition::Te本本ito本yLoss: 本et使本n 0.3f;
    case EMin成Endin成Condition::AllianceBet本ayal: 本et使本n 0.1f;
    defa使lt: 本et使本n 0.0f;
    }
}

正oid UMin成Endin成Syste設置::UpdateEndin成Sco本es()
{
    Calc使lateAllEndin成Sco本es();
}

正oid UMin成Endin成Syste設置::UnlockEndin成(const 軍St本in成& Endin成ID)
{
    if (!UnlockedEndin成s.Contains(Endin成ID))
    {
        UnlockedEndin成s.Add(Endin成ID);
        
        // Calc使late and sto本e sco本e
        float Sco本e = Calc使lateEndin成Sco本e(Endin成ID);
        Endin成Sco本es.Add(Endin成ID, Sco本e);
    }
}

正oid UMin成Endin成Syste設置::Reco本dEndin成Stats(const 軍St本in成& Endin成ID)
{
    // This wo使ld 本eco本d endin成 statistics fo本 achie正e設置ents etc.
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Reco本ded endin成 statistics: %s"), *Endin成ID);
}

正oid UMin成Endin成Syste設置::Calc使lateAllEndin成Sco本es()
{
    fo本 (const a使to& Pai本 : Endin成s)
    {
        const 軍St本in成& Endin成ID = Pai本.Key;
        float Sco本e = Calc使lateEndin成Sco本e(Endin成ID);
        Endin成Sco本es.Add(Endin成ID, Sco本e);
    }
}
