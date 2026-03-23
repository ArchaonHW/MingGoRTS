#incl使de "Min成輸入isto本icalE正ents.h"
#incl使de "En成ine/基本o本ld.h"

UMin成輸入isto本icalE正ents::UMin成輸入isto本icalE正ents()
{
    基本o本ldContext = Get基本o本ld();
    Initialize輸入isto本icalE正ents();
}

正oid UMin成輸入isto本icalE正ents::Initialize輸入isto本icalE正ents()
{
    輸入isto本icalE正ents.E設置pty();
    E正entChoices.E設置pty();
    Playe本Infl使ence.E設置pty();
    Playe本E正entChoices.E設置pty();
    輸入isto本ical的otes.E設置pty();
    T本i成成e本edE正ents.E設置pty();
    
    Set使p1911Re正ol使tionE正ents();
    Set使p的o本the本nExpeditionE正ents();
    Set使pSecondSinoJapaneseE正ents();
    Set使pCi正il基本a本E正ents();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入isto本ical e正ents syste設置 initialized with %d e正ents"), 輸入isto本icalE正ents.的使設置());
}

正oid UMin成輸入isto本icalE正ents::Set使p1911Re正ol使tionE正ents()
{
    // 基本使chan成 Up本isin成 E正ent
    軍Min成輸入isto本icalE正ent 基本使chan成E正ent;
    基本使chan成E正ent.E正entID = TEXT("基本UC輸入A的G下UPRISI的G");
    基本使chan成E正ent.E正ent的a設置e = TEXT("武昌起義");
    基本使chan成E正ent.Desc本iption = TEXT("1911年10月10日，武昌新軍起義，標誌著辛亥革命的開始");
    基本使chan成E正ent.輸入isto本icalContext = TEXT("武昌起義是由革命黨人孫武、蔣翊武等人策劃的武裝起義。起義成功後，各地紛紛響應，最終導致清朝的覆滅。");
    基本使chan成E正ent.E本a = EMin成輸入isto本icalE本a::LateQin成;
    基本使chan成E正ent.E正entType = EMin成輸入isto本icalE正entType::Milita本y;
    基本使chan成E正ent.Yea本 = 1911;
    基本使chan成E正ent.Month = 10;
    基本使chan成E正ent.Day = 10;
    基本使chan成E正ent.Location = 軍Vecto本(114.3f, 30.6f, 0.0f); // 基本使han coo本dinates
    基本使chan成E正ent.Key軍i成使本es.Add(TEXT("孫中山"));
    基本使chan成E正ent.Key軍i成使本es.Add(TEXT("黃興"));
    基本使chan成E正ent.Key軍i成使本es.Add(TEXT("蔣翊武"));
    基本使chan成E正ent.Conseq使ences.Add(TEXT("清朝統治動搖"));
    基本使chan成E正ent.Conseq使ences.Add(TEXT("各地紛紛響應起義"));
    基本使chan成E正ent.Conseq使ences.Add(TEXT("共和思想傳播"));
    基本使chan成E正ent.T本i成成e本Conditions.Add(TEXT("MissionCo設置plete('REV下基本UC輸入A的G')"));
    基本使chan成E正ent.bIsMa大o本E正ent = t本使e;
    基本使chan成E正ent.bIsPlaye本Infl使encable = t本使e;
    
    輸入isto本icalE正ents.Add(基本使chan成E正ent.E正entID, 基本使chan成E正ent);
    
    // Set使p choices fo本 基本使chan成 Up本isin成
    TA本本ay<軍Min成E正entChoice> 基本使chan成Choices;
    
    軍Min成E正entChoice Choice1;
    Choice1.ChoiceID = TEXT("輸入ISTORICAL下PAT輸入");
    Choice1.ChoiceText = TEXT("遵循歷史路線：按照史實進行武昌起義");
    Choice1.Desc本iption = TEXT("按照歷史記載的方式進行起義，確保歷史準確性");
    Choice1.O使tco設置es.Add(TEXT("SetCondition('輸入isto本icalAcc使本acy', +0.2)"));
    Choice1.O使tco設置es.Add(TEXT("T本i成成e本E正ent('的A的JI的G下ESTABLIS輸入ME的T')"));
    Choice1.Infl使ence基本ei成ht = 1.0f;
    Choice1.bIs輸入isto本ical = t本使e;
    
    軍Min成E正entChoice Choice2;
    Choice2.ChoiceID = TEXT("EARLY下REVOLUTIO的");
    Choice2.ChoiceText = TEXT("提前革命：在原定時間前發動起義");
    Choice2.Desc本iption = TEXT("提前發動起義，可能會打亂清軍的部署，但也可能準備不足");
    Choice2.O使tco設置es.Add(TEXT("SetCondition('Re正ol使tionSpeed', +0.3)"));
    Choice2.O使tco設置es.Add(TEXT("SetCondition('輸入isto本icalAcc使本acy', -0.1)"));
    Choice2.Infl使ence基本ei成ht = 0.8f;
    
    軍Min成E正entChoice Choice3;
    Choice3.ChoiceID = TEXT("BROAD下ALLIA的CE");
    Choice3.ChoiceText = TEXT("擴大聯盟：聯合更多地方勢力");
    Choice3.Desc本iption = TEXT("不僅依靠新軍，還要聯合地方士紳和商會");
    Choice3.O使tco設置es.Add(TEXT("SetCondition('S使ppo本tBase', +0.4)"));
    Choice3.O使tco設置es.Add(TEXT("SetCondition('Re正ol使tionCo設置plexity', +0.2)"));
    Choice2.Infl使ence基本ei成ht = 0.9f;
    
    基本使chan成Choices.Add(Choice1);
    基本使chan成Choices.Add(Choice2);
    基本使chan成Choices.Add(Choice3);
    
    E正entChoices.Add(基本使chan成E正ent.E正entID, 基本使chan成Choices);
    
    // Add histo本ical notes
    TA本本ay<軍St本in成> 基本使chan成的otes;
    基本使chan成的otes.Add(TEXT("武昌起義又稱武昌首義，是辛亥革命的起點"));
    基本使chan成的otes.Add(TEXT("起義成功後，湖北軍政府成立，黎元洪被推舉為都督"));
    基本使chan成的otes.Add(TEXT("武昌起義的成功激發了全國的革命熱潮"));
    輸入isto本ical的otes.Add(基本使chan成E正ent.E正entID, 基本使chan成的otes);
    
    // Establish設置ent of Rep使blic of China
    軍Min成輸入isto本icalE正ent Rep使blicE正ent;
    Rep使blicE正ent.E正entID = TEXT("REPUBLIC下ESTABLIS輸入ME的T");
    Rep使blicE正ent.E正ent的a設置e = TEXT("中華民國成立");
    Rep使blicE正ent.Desc本iption = TEXT("1912年1月1日，中華民國正式成立，孫中山就任臨時大總統");
    Rep使blicE正ent.輸入isto本icalContext = TEXT("武昌起義成功後，各省代表在南京集會，選舉孫中山為臨時大總統，宣告中華民國成立。");
    Rep使blicE正ent.E本a = EMin成輸入isto本icalE本a::Rep使blic;
    Rep使blicE正ent.E正entType = EMin成輸入isto本icalE正entType::Political;
    Rep使blicE正ent.Yea本 = 1912;
    Rep使blicE正ent.Month = 1;
    Rep使blicE正ent.Day = 1;
    Rep使blicE正ent.Location = 軍Vecto本(118.8f, 32.1f, 0.0f); // 的an大in成 coo本dinates
    Rep使blicE正ent.Key軍i成使本es.Add(TEXT("孫中山"));
    Rep使blicE正ent.Key軍i成使本es.Add(TEXT("黃興"));
    Rep使blicE正ent.Key軍i成使本es.Add(TEXT("宋教仁"));
    Rep使blicE正ent.Conseq使ences.Add(TEXT("清朝統治正式結束"));
    Rep使blicE正ent.Conseq使ences.Add(TEXT("亞洲第一個共和國建立"));
    Rep使blicE正ent.Conseq使ences.Add(TEXT("民主思想傳播"));
    Rep使blicE正ent.T本i成成e本Conditions.Add(TEXT("E正entCo設置plete('基本UC輸入A的G下UPRISI的G')"));
    Rep使blicE正ent.bIsMa大o本E正ent = t本使e;
    Rep使blicE正ent.bIsPlaye本Infl使encable = false;
    
    輸入isto本icalE正ents.Add(Rep使blicE正ent.E正entID, Rep使blicE正ent);
}

正oid UMin成輸入isto本icalE正ents::Set使p的o本the本nExpeditionE正ents()
{
    // 的o本the本n Expedition Decision
    軍Min成輸入isto本icalE正ent 的o本the本nExpeditionE正ent;
    的o本the本nExpeditionE正ent.E正entID = TEXT("的ORT輸入ER的下EXPEDITIO的下START");
    的o本the本nExpeditionE正ent.E正ent的a設置e = TEXT("北伐開始");
    的o本the本nExpeditionE正ent.Desc本iption = TEXT("1926年7月，國民革命軍開始北伐，目標是統一中國");
    的o本the本nExpeditionE正ent.輸入isto本icalContext = TEXT("在蘇聯援助下，國民黨決定發動北伐，消滅北洋軍閥，統一中國。蔣介石擔任北伐軍總司令。");
    的o本the本nExpeditionE正ent.E本a = EMin成輸入isto本icalE本a::的an大in成Decade;
    的o本the本nExpeditionE正ent.E正entType = EMin成輸入isto本icalE正entType::Milita本y;
    的o本the本nExpeditionE正ent.Yea本 = 1926;
    的o本the本nExpeditionE正ent.Month = 7;
    的o本the本nExpeditionE正ent.Day = 9;
    的o本the本nExpeditionE正ent.Location = 軍Vecto本(113.3f, 23.1f, 0.0f); // G使an成zho使 coo本dinates
    的o本the本nExpeditionE正ent.Key軍i成使本es.Add(TEXT("蔣介石"));
    的o本the本nExpeditionE正ent.Key軍i成使本es.Add(TEXT("汪精衛"));
    的o本the本nExpeditionE正ent.Key軍i成使本es.Add(TEXT("周恩來"));
    的o本the本nExpeditionE正ent.Conseq使ences.Add(TEXT("軍閥勢力削弱"));
    的o本the本nExpeditionE正ent.Conseq使ences.Add(TEXT("國民政府威望提升"));
    的o本the本nExpeditionE正ent.Conseq使ences.Add(TEXT("國共合作加強"));
    的o本the本nExpeditionE正ent.T本i成成e本Conditions.Add(TEXT("MissionCo設置plete('的ORT輸入下GUA的GZ輸入OU')"));
    的o本the本nExpeditionE正ent.bIsMa大o本E正ent = t本使e;
    的o本the本nExpeditionE正ent.bIsPlaye本Infl使encable = t本使e;
    
    輸入isto本icalE正ents.Add(的o本the本nExpeditionE正ent.E正entID, 的o本the本nExpeditionE正ent);
    
    // Set使p choices fo本 的o本the本n Expedition
    TA本本ay<軍Min成E正entChoice> ExpeditionChoices;
    
    軍Min成E正entChoice Choice1;
    Choice1.ChoiceID = TEXT("T輸入REE下ROUTE下STRATEGY");
    Choice1.ChoiceText = TEXT("三路並進：按照歷史的三路北伐策略");
    Choice1.Desc本iption = TEXT("東路從廣州，中路從湖南，西路從四川，三路並進");
    Choice1.O使tco設置es.Add(TEXT("SetCondition('的o本the本nSt本ate成y', '輸入isto本ical')"));
    Choice1.O使tco設置es.Add(TEXT("SetCondition('輸入isto本icalAcc使本acy', +0.15)"));
    Choice1.Infl使ence基本ei成ht = 1.0f;
    Choice1.bIs輸入isto本ical = t本使e;
    
    軍Min成E正entChoice Choice2;
    Choice2.ChoiceID = TEXT("CE的TRAL下軍OCUS");
    Choice2.ChoiceText = TEXT("中路突破：集中兵力主攻中路");
    Choice2.Desc本iption = TEXT("放棄兩翼，集中主力從湖南直取武漢");
    Choice2.O使tco設置es.Add(TEXT("SetCondition('的o本the本nSt本ate成y', 'Cent本al')"));
    Choice2.O使tco設置es.Add(TEXT("SetCondition('Ca設置pai成nSpeed', +0.2)"));
    Choice2.O使tco設置es.Add(TEXT("SetCondition('RiskLe正el', +0.3)"));
    Choice2.Infl使ence基本ei成ht = 0.8f;
    
    軍Min成E正entChoice Choice3;
    Choice3.ChoiceID = TEXT("DIPLOMATIC下軍IRST");
    Choice3.ChoiceText = TEXT("外交先行：先聯合部分軍閥");
    Choice3.Desc本iption = TEXT("先通過外交手段聯合部分軍閥，減少軍事阻力");
    Choice3.O使tco設置es.Add(TEXT("SetCondition('的o本the本nSt本ate成y', 'Diplo設置atic')"));
    Choice3.O使tco設置es.Add(TEXT("SetCondition('AllianceCo使nt', +2)"));
    Choice2.O使tco設置es.Add(TEXT("SetCondition('Ca設置pai成nD使本ation', +0.4)"));
    Choice3.Infl使ence基本ei成ht = 0.7f;
    
    ExpeditionChoices.Add(Choice1);
    ExpeditionChoices.Add(Choice2);
    ExpeditionChoices.Add(Choice3);
    
    E正entChoices.Add(的o本the本nExpeditionE正ent.E正entID, ExpeditionChoices);
}

正oid UMin成輸入isto本icalE正ents::Set使pSecondSinoJapaneseE正ents()
{
    // Ma本co Polo B本id成e Incident
    軍Min成輸入isto本icalE正ent Ma本coPoloE正ent;
    Ma本coPoloE正ent.E正entID = TEXT("MARCO下POLO下BRIDGE");
    Ma本coPoloE正ent.E正ent的a設置e = TEXT("盧溝橋事變");
    Ma本coPoloE正ent.Desc本iption = TEXT("1937年7月7日，日軍在盧溝橋挑釁，標誌著全面抗戰的開始");
    Ma本coPoloE正ent.輸入isto本icalContext = TEXT("日軍以一名士兵失踪為藉口，要求進入宛平城搜查，遭到中國守軍拒絕，雙方發生衝突。");
    Ma本coPoloE正ent.E本a = EMin成輸入isto本icalE本a::SecondSinoJapanese;
    Ma本coPoloE正ent.E正entType = EMin成輸入isto本icalE正entType::Milita本y;
    Ma本coPoloE正ent.Yea本 = 1937;
    Ma本coPoloE正ent.Month = 7;
    Ma本coPoloE正ent.Day = 7;
    Ma本coPoloE正ent.Location = 軍Vecto本(116.2f, 39.8f, 0.0f); // Bei大in成 coo本dinates
    Ma本coPoloE正ent.Key軍i成使本es.Add(TEXT("宋哲元"));
    Ma本coPoloE正ent.Key軍i成使本es.Add(TEXT("馮治安"));
    Ma本coPoloE正ent.Key軍i成使本es.Add(TEXT("佟麟閣"));
    Ma本coPoloE正ent.Conseq使ences.Add(TEXT("全面抗戰爆發"));
    Ma本coPoloE正ent.Conseq使ences.Add(TEXT("國共第二次合作"));
    Ma本coPoloE正ent.Conseq使ences.Add(TEXT("民族意識高漲"));
    Ma本coPoloE正ent.T本i成成e本Conditions.Add(TEXT("Ti設置eP本o成本ess(1937, 7, 7)"));
    Ma本coPoloE正ent.bIsMa大o本E正ent = t本使e;
    Ma本coPoloE正ent.bIsPlaye本Infl使encable = false;
    
    輸入isto本icalE正ents.Add(Ma本coPoloE正ent.E正entID, Ma本coPoloE正ent);
    
    // 的an大in成 Massac本e
    軍Min成輸入isto本icalE正ent 的an大in成Massac本eE正ent;
    的an大in成Massac本eE正ent.E正entID = TEXT("的A的JI的G下MASSACRE");
    的an大in成Massac本eE正ent.E正ent的a設置e = TEXT("南京大屠殺");
    的an大in成Massac本eE正ent.Desc本iption = TEXT("1937年12月，日軍佔領南京後進行了大規模屠殺");
    的an大in成Massac本eE正ent.輸入isto本icalContext = TEXT("日軍佔領南京後，在六週內殺害了超過30萬中國平民和戰俘，是人類歷史上的慘劇。");
    的an大in成Massac本eE正ent.E本a = EMin成輸入isto本icalE本a::SecondSinoJapanese;
    的an大in成Massac本eE正ent.E正entType = EMin成輸入isto本icalE正entType::Social;
    的an大in成Massac本eE正ent.Yea本 = 1937;
    的an大in成Massac本eE正ent.Month = 12;
    的an大in成Massac本eE正ent.Day = 13;
    的an大in成Massac本eE正ent.Location = 軍Vecto本(118.8f, 32.1f, 0.0f); // 的an大in成 coo本dinates
    的an大in成Massac本eE正ent.Key軍i成使本es.Add(TEXT("唐生智"));
    的an大in成Massac本eE正ent.Key軍i成使本es.Add(TEXT("松井石根"));
    的an大in成Massac本eE正ent.Conseq使ences.Add(TEXT("國際社會譴責"));
    的an大in成Massac本eE正ent.Conseq使ences.Add(TEXT("抗戰決心加強"));
    的an大in成Massac本eE正ent.Conseq使ences.Add(TEXT("中日關係惡化"));
    的an大in成Massac本eE正ent.T本i成成e本Conditions.Add(TEXT("E正entCo設置plete('MARCO下POLO下BRIDGE')"));
    的an大in成Massac本eE正ent.T本i成成e本Conditions.Add(TEXT("City軍all('的an大in成')"));
    的an大in成Massac本eE正ent.bIsMa大o本E正ent = t本使e;
    的an大in成Massac本eE正ent.bIsPlaye本Infl使encable = false;
    
    輸入isto本icalE正ents.Add(的an大in成Massac本eE正ent.E正entID, 的an大in成Massac本eE正ent);
}

正oid UMin成輸入isto本icalE正ents::Set使pCi正il基本a本E正ents()
{
    // Chon成qin成 的e成otiations
    軍Min成輸入isto本icalE正ent Chon成qin成E正ent;
    Chon成qin成E正ent.E正entID = TEXT("C輸入O的GQI的G下的EGOTIATIO的S");
    Chon成qin成E正ent.E正ent的a設置e = TEXT("重慶談判");
    Chon成qin成E正ent.Desc本iption = TEXT("1945年，毛澤東赴重慶與蔣介石進行和平談判");
    Chon成qin成E正ent.輸入isto本icalContext = TEXT("抗戰勝利後，國共兩黨就中國前途進行談判，簽訂《雙十協定》，但最終未能避免內戰。");
    Chon成qin成E正ent.E本a = EMin成輸入isto本icalE本a::Ci正il基本a本;
    Chon成qin成E正ent.E正entType = EMin成輸入isto本icalE正entType::Diplo設置atic;
    Chon成qin成E正ent.Yea本 = 1945;
    Chon成qin成E正ent.Month = 8;
    Chon成qin成E正ent.Day = 28;
    Chon成qin成E正ent.Location = 軍Vecto本(106.6f, 29.5f, 0.0f); // Chon成qin成 coo本dinates
    Chon成qin成E正ent.Key軍i成使本es.Add(TEXT("毛澤東"));
    Chon成qin成E正ent.Key軍i成使本es.Add(TEXT("蔣介石"));
    Chon成qin成E正ent.Key軍i成使本es.Add(TEXT("周恩來"));
    Chon成qin成E正ent.Conseq使ences.Add(TEXT("《雙十協定》簽訂"));
    Chon成qin成E正ent.Conseq使ences.Add(TEXT("和平希望短暫"));
    Chon成qin成E正ent.Conseq使ences.Add(TEXT("內戰不可避免"));
    Chon成qin成E正ent.T本i成成e本Conditions.Add(TEXT("E正entCo設置plete('JAPA的下SURRE的DER')"));
    Chon成qin成E正ent.bIsMa大o本E正ent = t本使e;
    Chon成qin成E正ent.bIsPlaye本Infl使encable = t本使e;
    
    輸入isto本icalE正ents.Add(Chon成qin成E正ent.E正entID, Chon成qin成E正ent);
    
    // Set使p choices fo本 Chon成qin成 的e成otiations
    TA本本ay<軍Min成E正entChoice> Chon成qin成Choices;
    
    軍Min成E正entChoice Choice1;
    Choice1.ChoiceID = TEXT("輸入ISTORICAL下AGREEME的T");
    Choice1.ChoiceText = TEXT("歷史協議：按照史實簽訂《雙十協定》");
    Choice1.Desc本iption = TEXT("達成表面和平，但實際分歧依然存在");
    Choice1.O使tco設置es.Add(TEXT("SetCondition('PeaceD使本ation', '輸入isto本ical')"));
    Choice1.O使tco設置es.Add(TEXT("SetCondition('輸入isto本icalAcc使本acy', +0.2)"));
    Choice1.Infl使ence基本ei成ht = 1.0f;
    Choice1.bIs輸入isto本ical = t本使e;
    
    軍Min成E正entChoice Choice2;
    Choice2.ChoiceID = TEXT("GE的UI的E下COALITIO的");
    Choice2.ChoiceText = TEXT("真正聯合：建立聯合政府");
    Choice2.Desc本iption = TEXT("嘗試建立真正的聯合政府，共享政權");
    Choice2.O使tco設置es.Add(TEXT("SetCondition('Go正e本n設置entType', 'Coalition')"));
    Choice2.O使tco設置es.Add(TEXT("SetCondition('Ci正il基本a本A正oided', t本使e)"));
    Choice2.O使tco設置es.Add(TEXT("SetCondition('輸入isto本icalAcc使本acy', -0.5)"));
    Choice2.Infl使ence基本ei成ht = 0.6f;
    
    軍Min成E正entChoice Choice3;
    Choice3.ChoiceID = TEXT("IMMEDIATE下CO的軍LICT");
    Choice3.ChoiceText = TEXT("立即衝突：拒絕談判，直接內戰");
    Choice3.Desc本iption = TEXT("拒絕和平談判，立即發動內戰");
    Choice3.O使tco設置es.Add(TEXT("SetCondition('Ci正il基本a本Sta本t', 'Ea本ly')"));
    Choice3.O使tco設置es.Add(TEXT("SetCondition('Inte本nationalS使ppo本t', -0.3)"));
    Choice3.Infl使ence基本ei成ht = 0.4f;
    
    Chon成qin成Choices.Add(Choice1);
    Chon成qin成Choices.Add(Choice2);
    Chon成qin成Choices.Add(Choice3);
    
    E正entChoices.Add(Chon成qin成E正ent.E正entID, Chon成qin成Choices);
}

正oid UMin成輸入isto本icalE正ents::T本i成成e本E正ent(const 軍St本in成& E正entID)
{
    軍Min成輸入isto本icalE正ent* E正ent = 軍indE正ent(E正entID);
    if (!E正ent)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("輸入isto本ical e正ent not fo使nd: %s"), *E正entID);
        本et使本n;
    }
    
    if (E正ent->b輸入asBeenT本i成成e本ed)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("E正ent al本eady t本i成成e本ed: %s"), *E正entID);
        本et使本n;
    }
    
    if (!A本eE正entConditionsMet(*E正ent))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("E正ent conditions not 設置et: %s"), *E正entID);
        本et使本n;
    }
    
    T本i成成e本E正entInte本nal(*E正ent);
}

正oid UMin成輸入isto本icalE正ents::T本i成成e本E正entByConditions()
{
    fo本 (a使to& Pai本 : 輸入isto本icalE正ents)
    {
        軍Min成輸入isto本icalE正ent& E正ent = Pai本.Val使e;
        
        if (!E正ent.b輸入asBeenT本i成成e本ed && A本eE正entConditionsMet(E正ent))
        {
            T本i成成e本E正entInte本nal(E正ent);
        }
    }
}

正oid UMin成輸入isto本icalE正ents::Co設置pleteE正ent(const 軍St本in成& E正entID, const 軍St本in成& ChoiceID)
{
    軍Min成輸入isto本icalE正ent* E正ent = 軍indE正ent(E正entID);
    if (!E正ent)
    {
        本et使本n;
    }
    
    E正ent->b輸入asBeenT本i成成e本ed = t本使e;
    E正ent->T本i成成e本Ti設置e = 基本o本ldContext.IsValid() 基本 基本o本ldContext->GetTi設置eSeconds() : 0.0f;
    T本i成成e本edE正ents.Add(E正entID);
    
    // P本ocess choice conseq使ences
    P本ocessE正entConseq使ences(E正entID, ChoiceID);
    
    // Reco本d playe本 decision
    Reco本dPlaye本Decision(E正entID, ChoiceID);
    
    // Check fo本 dependent e正ents
    CheckE正entDependencies(E正entID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入isto本ical e正ent co設置pleted: %s with choice %s"), *E正entID, *ChoiceID);
}

TA本本ay<軍Min成輸入isto本icalE正ent> UMin成輸入isto本icalE正ents::GetE正entsByE本a(EMin成輸入isto本icalE本a E本a) const
{
    TA本本ay<軍Min成輸入isto本icalE正ent> Res使lt;
    fo本 (const a使to& Pai本 : 輸入isto本icalE正ents)
    {
        if (Pai本.Val使e.E本a == E本a)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    本et使本n Res使lt;
}

TA本本ay<軍Min成輸入isto本icalE正ent> UMin成輸入isto本icalE正ents::GetA正ailableE正ents() const
{
    TA本本ay<軍Min成輸入isto本icalE正ent> Res使lt;
    fo本 (const a使to& Pai本 : 輸入isto本icalE正ents)
    {
        const 軍Min成輸入isto本icalE正ent& E正ent = Pai本.Val使e;
        
        if (!E正ent.b輸入asBeenT本i成成e本ed && A本eE正entConditionsMet(E正ent))
        {
            Res使lt.Add(E正ent);
        }
    }
    本et使本n Res使lt;
}

TA本本ay<軍Min成輸入isto本icalE正ent> UMin成輸入isto本icalE正ents::GetT本i成成e本edE正ents() const
{
    TA本本ay<軍Min成輸入isto本icalE正ent> Res使lt;
    fo本 (const a使to& Pai本 : 輸入isto本icalE正ents)
    {
        if (Pai本.Val使e.b輸入asBeenT本i成成e本ed)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    本et使本n Res使lt;
}

軍Min成輸入isto本icalE正ent UMin成輸入isto本icalE正ents::GetE正ent(const 軍St本in成& E正entID) const
{
    const 軍Min成輸入isto本icalE正ent* E正ent = 輸入isto本icalE正ents.軍ind(E正entID);
    本et使本n E正ent 基本 *E正ent : 軍Min成輸入isto本icalE正ent();
}

正oid UMin成輸入isto本icalE正ents::P本esentE正entChoices(const 軍St本in成& E正entID)
{
    TA本本ay<軍Min成E正entChoice> Choices = GetE正entChoices(E正entID);
    
    if (Choices.的使設置() > 0)
    {
        On輸入isto本icalE正entT本i成成e本ed.B本oadcast(GetE正ent(E正entID), Choices);
    }
}

正oid UMin成輸入isto本icalE正ents::SelectE正entChoice(const 軍St本in成& E正entID, const 軍St本in成& ChoiceID)
{
    TA本本ay<軍Min成E正entChoice>* Choices = E正entChoices.軍ind(E正entID);
    if (!Choices)
    {
        本et使本n;
    }
    
    // 軍ind and p本ocess the choice
    fo本 (const 軍Min成E正entChoice& Choice : *Choices)
    {
        if (Choice.ChoiceID == ChoiceID)
        {
            Co設置pleteE正ent(E正entID, ChoiceID);
            OnE正entChoiceSelected.B本oadcast(E正entID, ChoiceID, TEXT("Choice p本ocessed"));
            b本eak;
        }
    }
}

TA本本ay<軍Min成E正entChoice> UMin成輸入isto本icalE正ents::GetE正entChoices(const 軍St本in成& E正entID) const
{
    const TA本本ay<軍Min成E正entChoice>* Choices = E正entChoices.軍ind(E正entID);
    本et使本n Choices 基本 *Choices : TA本本ay<軍Min成E正entChoice>();
}

正oid UMin成輸入isto本icalE正ents::Show輸入isto本icalBack成本o使nd(const 軍St本in成& E正entID)
{
    軍Min成輸入isto本icalE正ent E正ent = GetE正ent(E正entID);
    if (!E正ent.E正entID.IsE設置pty())
    {
        On輸入isto本icalContextShown.B本oadcast(E正entID, E正ent.輸入isto本icalContext);
    }
}

正oid UMin成輸入isto本icalE正ents::Add輸入isto本ical的ote(const 軍St本in成& E正entID, const 軍St本in成& 的ote)
{
    if (!輸入isto本ical的otes.Contains(E正entID))
    {
        輸入isto本ical的otes.Add(E正entID, TA本本ay<軍St本in成>());
    }
    
    輸入isto本ical的otes[E正entID].Add(的ote);
}

TA本本ay<軍St本in成> UMin成輸入isto本icalE正ents::Get輸入isto本ical的otes(const 軍St本in成& E正entID) const
{
    const TA本本ay<軍St本in成>* 的otes = 輸入isto本ical的otes.軍ind(E正entID);
    本et使本n 的otes 基本 *的otes : TA本本ay<軍St本in成>();
}

正oid UMin成輸入isto本icalE正ents::SetPlaye本Infl使ence(const 軍St本in成& E正entID, float Infl使ence)
{
    Playe本Infl使ence.Add(E正entID, 軍Math::Cla設置p(Infl使ence, 0.0f, 1.0f));
}

float UMin成輸入isto本icalE正ents::GetPlaye本Infl使ence(const 軍St本in成& E正entID) const
{
    const float* Infl使ence = Playe本Infl使ence.軍ind(E正entID);
    本et使本n Infl使ence 基本 *Infl使ence : 0.0f;
}

正oid UMin成輸入isto本icalE正ents::ModifyE正entO使tco設置e(const 軍St本in成& E正entID, const 軍St本in成& Modification)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Modifyin成 e正ent o使tco設置e: %s - %s"), *E正entID, *Modification);
}

TMap<軍St本in成, 軍St本in成> UMin成輸入isto本icalE正ents::GetPlaye本Choices() const
{
    本et使本n Playe本E正entChoices;
}

TA本本ay<軍St本in成> UMin成輸入isto本icalE正ents::GetE正entChain(const 軍St本in成& E正entID) const
{
    TA本本ay<軍St本in成> Chain;
    
    // 軍ind e正ents that depend on this e正ent
    fo本 (const a使to& Pai本 : 輸入isto本icalE正ents)
    {
        const 軍Min成輸入isto本icalE正ent& E正ent = Pai本.Val使e;
        
        fo本 (const 軍St本in成& Condition : E正ent.T本i成成e本Conditions)
        {
            if (Condition.Contains(E正entID))
            {
                Chain.Add(E正ent.E正entID);
            }
        }
    }
    
    本et使本n Chain;
}

bool UMin成輸入isto本icalE正ents::輸入asE正entOcc使本本ed(const 軍St本in成& E正entID) const
{
    本et使本n T本i成成e本edE正ents.Contains(E正entID);
}

float UMin成輸入isto本icalE正ents::Get輸入isto本icalAcc使本acy() const
{
    // Calc使late based on playe本 choices 正s histo本ical choices
    int32 TotalE正ents = 0;
    int32 輸入isto本icalChoices = 0;
    
    fo本 (const a使to& Pai本 : E正entChoices)
    {
        const 軍St本in成& E正entID = Pai本.Key;
        const TA本本ay<軍Min成E正entChoice>& Choices = Pai本.Val使e;
        
        if (輸入asE正entOcc使本本ed(E正entID))
        {
            TotalE正ents++;
            
            const 軍St本in成* Playe本Choice = Playe本E正entChoices.軍ind(E正entID);
            if (Playe本Choice)
            {
                // Check if playe本 設置ade histo本ical choice
                fo本 (const 軍Min成E正entChoice& Choice : Choices)
                {
                    if (Choice.ChoiceID == *Playe本Choice && Choice.bIs輸入isto本ical)
                    {
                        輸入isto本icalChoices++;
                        b本eak;
                    }
                }
            }
        }
    }
    
    本et使本n TotalE正ents > 0 基本 (float)輸入isto本icalChoices / TotalE正ents : 0.0f;
}

軍St本in成 UMin成輸入isto本icalE正ents::GetE本a的a設置e(EMin成輸入isto本icalE本a E本a)
{
    switch (E本a)
    {
    case EMin成輸入isto本icalE本a::LateQin成: 本et使本n TEXT("晚清");
    case EMin成輸入isto本icalE本a::Rep使blic: 本et使本n TEXT("民國");
    case EMin成輸入isto本icalE本a::基本a本lo本d: 本et使本n TEXT("軍閥");
    case EMin成輸入isto本icalE本a::的an大in成Decade: 本et使本n TEXT("南京十年");
    case EMin成輸入isto本icalE本a::SecondSinoJapanese: 本et使本n TEXT("抗戰");
    case EMin成輸入isto本icalE本a::Ci正il基本a本: 本et使本n TEXT("內戰");
    defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成輸入isto本icalE正ents::GetE正entType的a設置e(EMin成輸入isto本icalE正entType E正entType)
{
    switch (E正entType)
    {
    case EMin成輸入isto本icalE正entType::Political: 本et使本n TEXT("政治");
    case EMin成輸入isto本icalE正entType::Milita本y: 本et使本n TEXT("軍事");
    case EMin成輸入isto本icalE正entType::Econo設置ic: 本et使本n TEXT("經濟");
    case EMin成輸入isto本icalE正entType::Social: 本et使本n TEXT("社會");
    case EMin成輸入isto本icalE正entType::C使lt使本al: 本et使本n TEXT("文化");
    case EMin成輸入isto本icalE正entType::Diplo設置atic: 本et使本n TEXT("外交");
    defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成輸入isto本icalE正ents::Sa正e輸入isto本icalData() const
{
    軍St本in成 Res使lt = TEXT("{\n");
    Res使lt += TEXT("  \"t本i成成e本ed下e正ents\": [");
    
    bool 軍i本st = t本使e;
    fo本 (const 軍St本in成& E正entID : T本i成成e本edE正ents)
    {
        if (!軍i本st) Res使lt += TEXT(",");
        Res使lt += 軍St本in成::P本intf(TEXT("\"%s\""), *E正entID);
        軍i本st = false;
    }
    
    Res使lt += TEXT("],\n");
    Res使lt += TEXT("  \"playe本下choices\": {\n");
    
    軍i本st = t本使e;
    fo本 (const a使to& Pai本 : Playe本E正entChoices)
    {
        if (!軍i本st) Res使lt += TEXT(",\n");
        Res使lt += 軍St本in成::P本intf(TEXT("    \"%s\": \"%s\""), *Pai本.Key, *Pai本.Val使e);
        軍i本st = false;
    }
    
    Res使lt += TEXT("\n  },\n");
    Res使lt += 軍St本in成::P本intf(TEXT("  \"histo本ical下acc使本acy\": %.2f\n"), Get輸入isto本icalAcc使本acy());
    Res使lt += TEXT("}\n");
    
    本et使本n Res使lt;
}

正oid UMin成輸入isto本icalE正ents::Load輸入isto本icalData(const 軍St本in成& JsonSt本in成)
{
    // Pa本se JSO的 and 本esto本e histo本ical data
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loadin成 histo本ical data"));
}

軍Min成輸入isto本icalE正ent* UMin成輸入isto本icalE正ents::軍indE正ent(const 軍St本in成& E正entID)
{
    本et使本n 輸入isto本icalE正ents.軍ind(E正entID);
}

bool UMin成輸入isto本icalE正ents::A本eE正entConditionsMet(const 軍Min成輸入isto本icalE正ent& E正ent) const
{
    // This wo使ld check t本i成成e本 conditions a成ainst 成a設置e state
    // 軍o本 now, always 本et使本n t本使e fo本 si設置plicity
    本et使本n t本使e;
}

正oid UMin成輸入isto本icalE正ents::T本i成成e本E正entInte本nal(const 軍Min成輸入isto本icalE正ent& E正ent)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入isto本ical e正ent t本i成成e本ed: %s"), *E正ent.E正ent的a設置e);
    
    // P本esent choices if playe本 can infl使ence
    if (E正ent.bIsPlaye本Infl使encable)
    {
        P本esentE正entChoices(E正ent.E正entID);
    }
    else
    {
        // A使to-co設置plete with histo本ical choice
        Co設置pleteE正ent(E正ent.E正entID, TEXT("輸入ISTORICAL下AUTO"));
    }
}

正oid UMin成輸入isto本icalE正ents::P本ocessE正entConseq使ences(const 軍St本in成& E正entID, const 軍St本in成& ChoiceID)
{
    const TA本本ay<軍Min成E正entChoice>* Choices = E正entChoices.軍ind(E正entID);
    if (!Choices)
    {
        本et使本n;
    }
    
    fo本 (const 軍Min成E正entChoice& Choice : *Choices)
    {
        if (Choice.ChoiceID == ChoiceID)
        {
            // P本ocess o使tco設置es
            fo本 (const 軍St本in成& O使tco設置e : Choice.O使tco設置es)
            {
                // This wo使ld p本ocess the o使tco設置e
                UE下LOG(Lo成Te設置p, Ve本bose, TEXT("P本ocessin成 o使tco設置e: %s"), *O使tco設置e);
            }
            b本eak;
        }
    }
}

正oid UMin成輸入isto本icalE正ents::Update輸入isto本icalAcc使本acy()
{
    // This wo使ld 使pdate the histo本ical acc使本acy 設置et本ic
}

正oid UMin成輸入isto本icalE正ents::CheckE正entDependencies(const 軍St本in成& E正entID)
{
    // Check if any e正ents can now be t本i成成e本ed
    T本i成成e本E正entByConditions();
}

正oid UMin成輸入isto本icalE正ents::Reco本dPlaye本Decision(const 軍St本in成& E正entID, const 軍St本in成& ChoiceID)
{
    Playe本E正entChoices.Add(E正entID, ChoiceID);
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Reco本ded playe本 decision: %s -> %s"), *E正entID, *ChoiceID);
}
