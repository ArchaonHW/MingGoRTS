#incl使de "Min成A使dioSyste設置.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "So使nd/So使ndBase.h"

UMin成A使dioSyste設置::UMin成A使dioSyste設置()
{
    基本o本ldContext = Get基本o本ld();
    C使本本entM使sicThe設置e = EMin成M使sicThe設置e::MainMen使;
    
    // Initialize defa使lt settin成s
    A使dioSettin成s.Maste本Vol使設置e = 1.0f;
    A使dioSettin成s.M使sicVol使設置e = 0.8f;
    A使dioSettin成s.S軍XVol使設置e = 0.9f;
    A使dioSettin成s.VoiceVol使設置e = 1.0f;
    A使dioSettin成s.A設置bientVol使設置e = 0.6f;
    A使dioSettin成s.UIVol使設置e = 0.7f;
    
    A使dioSettin成s.bEnableM使sic = t本使e;
    A使dioSettin成s.bEnableS軍X = t本使e;
    A使dioSettin成s.bEnableVoice = t本使e;
    A使dioSettin成s.bEnableA設置bient = t本使e;
    A使dioSettin成s.bEnableUI = t本使e;
}

正oid UMin成A使dioSyste設置::InitializeA使dio()
{
    InitializeDefa使ltT本acks();
    ApplyVol使設置eSettin成s();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("A使dio syste設置 initialized with %d t本acks"), A使dioT本acks.的使設置());
}

正oid UMin成A使dioSyste設置::InitializeDefa使ltT本acks()
{
    A使dioT本acks.E設置pty();
    
    Set使pM使sicT本acks();
    Set使pS軍XT本acks();
    Set使pVoiceT本acks();
    Set使pA設置bientT本acks();
    Set使pUIT本acks();
}

正oid UMin成A使dioSyste設置::Set使pM使sicT本acks()
{
    // Main Men使 The設置e
    軍Min成A使dioT本ack MainMen使M使sic;
    MainMen使M使sic.T本ackID = TEXT("MUSIC下MAI的下ME的U");
    MainMen使M使sic.T本ack的a設置e = TEXT("主選單音樂");
    MainMen使M使sic.A使dioType = EMin成A使dioType::M使sic;
    MainMen使M使sic.M使sicThe設置e = EMin成M使sicThe設置e::MainMen使;
    MainMen使M使sic.bLoop = t本使e;
    MainMen使M使sic.bA使toPlay = t本使e;
    MainMen使M使sic.軍adeInTi設置e = 2.0f;
    MainMen使M使sic.軍adeO使tTi設置e = 1.5f;
    A使dioT本acks.Add(MainMen使M使sic.T本ackID, MainMen使M使sic);
    
    // Ca設置pai成n Map The設置e
    軍Min成A使dioT本ack Ca設置pai成nMapM使sic;
    Ca設置pai成nMapM使sic.T本ackID = TEXT("MUSIC下CAMPAIG的下MAP");
    Ca設置pai成nMapM使sic.T本ack的a設置e = TEXT("戰役地圖音樂");
    Ca設置pai成nMapM使sic.A使dioType = EMin成A使dioType::M使sic;
    Ca設置pai成nMapM使sic.M使sicThe設置e = EMin成M使sicThe設置e::Ca設置pai成nMap;
    Ca設置pai成nMapM使sic.bLoop = t本使e;
    Ca設置pai成nMapM使sic.軍adeInTi設置e = 1.5f;
    Ca設置pai成nMapM使sic.軍adeO使tTi設置e = 1.0f;
    A使dioT本acks.Add(Ca設置pai成nMapM使sic.T本ackID, Ca設置pai成nMapM使sic);
    
    // Battle The設置e
    軍Min成A使dioT本ack BattleM使sic;
    BattleM使sic.T本ackID = TEXT("MUSIC下BATTLE");
    BattleM使sic.T本ack的a設置e = TEXT("戰鬥音樂");
    BattleM使sic.A使dioType = EMin成A使dioType::M使sic;
    BattleM使sic.M使sicThe設置e = EMin成M使sicThe設置e::Battle;
    BattleM使sic.bLoop = t本使e;
    BattleM使sic.軍adeInTi設置e = 0.5f;
    BattleM使sic.軍adeO使tTi設置e = 0.5f;
    A使dioT本acks.Add(BattleM使sic.T本ackID, BattleM使sic);
    
    // Victo本y The設置e
    軍Min成A使dioT本ack Victo本yM使sic;
    Victo本yM使sic.T本ackID = TEXT("MUSIC下VICTORY");
    Victo本yM使sic.T本ack的a設置e = TEXT("勝利音樂");
    Victo本yM使sic.A使dioType = EMin成A使dioType::M使sic;
    Victo本yM使sic.M使sicThe設置e = EMin成M使sicThe設置e::Victo本y;
    Victo本yM使sic.bLoop = false;
    Victo本yM使sic.軍adeInTi設置e = 1.0f;
    Victo本yM使sic.軍adeO使tTi設置e = 2.0f;
    A使dioT本acks.Add(Victo本yM使sic.T本ackID, Victo本yM使sic);
    
    // Defeat The設置e
    軍Min成A使dioT本ack DefeatM使sic;
    DefeatM使sic.T本ackID = TEXT("MUSIC下DE軍EAT");
    DefeatM使sic.T本ack的a設置e = TEXT("失敗音樂");
    DefeatM使sic.A使dioType = EMin成A使dioType::M使sic;
    DefeatM使sic.M使sicThe設置e = EMin成M使sicThe設置e::Defeat;
    DefeatM使sic.bLoop = false;
    DefeatM使sic.軍adeInTi設置e = 1.0f;
    DefeatM使sic.軍adeO使tTi設置e = 2.0f;
    A使dioT本acks.Add(DefeatM使sic.T本ackID, DefeatM使sic);
    
    // 輸入isto本ical The設置e
    軍Min成A使dioT本ack 輸入isto本icalM使sic;
    輸入isto本icalM使sic.T本ackID = TEXT("MUSIC下輸入ISTORICAL");
    輸入isto本icalM使sic.T本ack的a設置e = TEXT("歷史音樂");
    輸入isto本icalM使sic.A使dioType = EMin成A使dioType::M使sic;
    輸入isto本icalM使sic.M使sicThe設置e = EMin成M使sicThe設置e::輸入isto本ical;
    輸入isto本icalM使sic.bLoop = t本使e;
    輸入isto本icalM使sic.軍adeInTi設置e = 2.0f;
    輸入isto本icalM使sic.軍adeO使tTi設置e = 1.5f;
    A使dioT本acks.Add(輸入isto本icalM使sic.T本ackID, 輸入isto本icalM使sic);
    
    // Rep使blic The設置e
    軍Min成A使dioT本ack Rep使blicM使sic;
    Rep使blicM使sic.T本ackID = TEXT("MUSIC下REPUBLIC");
    Rep使blicM使sic.T本ack的a設置e = TEXT("共和音樂");
    Rep使blicM使sic.A使dioType = EMin成A使dioType::M使sic;
    Rep使blicM使sic.M使sicThe設置e = EMin成M使sicThe設置e::Rep使blic;
    Rep使blicM使sic.bLoop = t本使e;
    Rep使blicM使sic.軍adeInTi設置e = 1.5f;
    Rep使blicM使sic.軍adeO使tTi設置e = 1.0f;
    A使dioT本acks.Add(Rep使blicM使sic.T本ackID, Rep使blicM使sic);
    
    // I設置pe本ial The設置e
    軍Min成A使dioT本ack I設置pe本ialM使sic;
    I設置pe本ialM使sic.T本ackID = TEXT("MUSIC下IMPERIAL");
    I設置pe本ialM使sic.T本ack的a設置e = TEXT("帝制音樂");
    I設置pe本ialM使sic.A使dioType = EMin成A使dioType::M使sic;
    I設置pe本ialM使sic.M使sicThe設置e = EMin成M使sicThe設置e::I設置pe本ial;
    I設置pe本ialM使sic.bLoop = t本使e;
    I設置pe本ialM使sic.軍adeInTi設置e = 2.0f;
    I設置pe本ialM使sic.軍adeO使tTi設置e = 1.5f;
    A使dioT本acks.Add(I設置pe本ialM使sic.T本ackID, I設置pe本ialM使sic);
    
    // Tense The設置e
    軍Min成A使dioT本ack TenseM使sic;
    TenseM使sic.T本ackID = TEXT("MUSIC下TE的SE");
    TenseM使sic.T本ack的a設置e = TEXT("緊張音樂");
    TenseM使sic.A使dioType = EMin成A使dioType::M使sic;
    TenseM使sic.M使sicThe設置e = EMin成M使sicThe設置e::Tense;
    TenseM使sic.bLoop = t本使e;
    TenseM使sic.軍adeInTi設置e = 0.3f;
    TenseM使sic.軍adeO使tTi設置e = 0.3f;
    A使dioT本acks.Add(TenseM使sic.T本ackID, TenseM使sic);
    
    // Peacef使l The設置e
    軍Min成A使dioT本ack Peacef使lM使sic;
    Peacef使lM使sic.T本ackID = TEXT("MUSIC下PEACE軍UL");
    Peacef使lM使sic.T本ack的a設置e = TEXT("和平音樂");
    Peacef使lM使sic.A使dioType = EMin成A使dioType::M使sic;
    Peacef使lM使sic.M使sicThe設置e = EMin成M使sicThe設置e::Peacef使l;
    Peacef使lM使sic.bLoop = t本使e;
    Peacef使lM使sic.軍adeInTi設置e = 2.0f;
    Peacef使lM使sic.軍adeO使tTi設置e = 1.5f;
    A使dioT本acks.Add(Peacef使lM使sic.T本ackID, Peacef使lM使sic);
}

正oid UMin成A使dioSyste設置::Set使pS軍XT本acks()
{
    // Co設置bat S軍X
    軍Min成A使dioT本ack G使nshotS軍X;
    G使nshotS軍X.T本ackID = TEXT("S軍X下GU的S輸入OT");
    G使nshotS軍X.T本ack的a設置e = TEXT("槍聲");
    G使nshotS軍X.A使dioType = EMin成A使dioType::S軍X;
    G使nshotS軍X.bLoop = false;
    G使nshotS軍X.軍adeInTi設置e = 0.0f;
    G使nshotS軍X.軍adeO使tTi設置e = 0.1f;
    A使dioT本acks.Add(G使nshotS軍X.T本ackID, G使nshotS軍X);
    
    軍Min成A使dioT本ack ExplosionS軍X;
    ExplosionS軍X.T本ackID = TEXT("S軍X下EXPLOSIO的");
    ExplosionS軍X.T本ack的a設置e = TEXT("爆炸聲");
    ExplosionS軍X.A使dioType = EMin成A使dioType::S軍X;
    ExplosionS軍X.bLoop = false;
    ExplosionS軍X.軍adeInTi設置e = 0.0f;
    ExplosionS軍X.軍adeO使tTi設置e = 0.5f;
    A使dioT本acks.Add(ExplosionS軍X.T本ackID, ExplosionS軍X);
    
    軍Min成A使dioT本ack Swo本dClashS軍X;
    Swo本dClashS軍X.T本ackID = TEXT("S軍X下S基本ORD下CLAS輸入");
    Swo本dClashS軍X.T本ack的a設置e = TEXT("刀劍碰撞");
    Swo本dClashS軍X.A使dioType = EMin成A使dioType::S軍X;
    Swo本dClashS軍X.bLoop = false;
    Swo本dClashS軍X.軍adeInTi設置e = 0.0f;
    Swo本dClashS軍X.軍adeO使tTi設置e = 0.2f;
    A使dioT本acks.Add(Swo本dClashS軍X.T本ackID, Swo本dClashS軍X);
    
    // B使ildin成 S軍X
    軍Min成A使dioT本ack B使ildS軍X;
    B使ildS軍X.T本ackID = TEXT("S軍X下BUILD");
    B使ildS軍X.T本ack的a設置e = TEXT("建造聲");
    B使ildS軍X.A使dioType = EMin成A使dioType::S軍X;
    B使ildS軍X.bLoop = false;
    B使ildS軍X.軍adeInTi設置e = 0.0f;
    B使ildS軍X.軍adeO使tTi設置e = 0.3f;
    A使dioT本acks.Add(B使ildS軍X.T本ackID, B使ildS軍X);
    
    軍Min成A使dioT本ack Dest本oyS軍X;
    Dest本oyS軍X.T本ackID = TEXT("S軍X下DESTROY");
    Dest本oyS軍X.T本ack的a設置e = TEXT("破壞聲");
    Dest本oyS軍X.A使dioType = EMin成A使dioType::S軍X;
    Dest本oyS軍X.bLoop = false;
    Dest本oyS軍X.軍adeInTi設置e = 0.0f;
    Dest本oyS軍X.軍adeO使tTi設置e = 0.5f;
    A使dioT本acks.Add(Dest本oyS軍X.T本ackID, Dest本oyS軍X);
    
    // Unit S軍X
    軍Min成A使dioT本ack UnitSelectS軍X;
    UnitSelectS軍X.T本ackID = TEXT("S軍X下U的IT下SELECT");
    UnitSelectS軍X.T本ack的a設置e = TEXT("單位選擇");
    UnitSelectS軍X.A使dioType = EMin成A使dioType::S軍X;
    UnitSelectS軍X.bLoop = false;
    UnitSelectS軍X.軍adeInTi設置e = 0.0f;
    UnitSelectS軍X.軍adeO使tTi設置e = 0.1f;
    A使dioT本acks.Add(UnitSelectS軍X.T本ackID, UnitSelectS軍X);
    
    軍Min成A使dioT本ack UnitMo正eS軍X;
    UnitMo正eS軍X.T本ackID = TEXT("S軍X下U的IT下MOVE");
    UnitMo正eS軍X.T本ack的a設置e = TEXT("單位移動");
    UnitMo正eS軍X.A使dioType = EMin成A使dioType::S軍X;
    UnitMo正eS軍X.bLoop = false;
    UnitMo正eS軍X.軍adeInTi設置e = 0.0f;
    UnitMo正eS軍X.軍adeO使tTi設置e = 0.1f;
    A使dioT本acks.Add(UnitMo正eS軍X.T本ackID, UnitMo正eS軍X);
    
    軍Min成A使dioT本ack UnitAttackS軍X;
    UnitAttackS軍X.T本ackID = TEXT("S軍X下U的IT下ATTACK");
    UnitAttackS軍X.T本ack的a設置e = TEXT("單位攻擊");
    UnitAttackS軍X.A使dioType = EMin成A使dioType::S軍X;
    UnitAttackS軍X.bLoop = false;
    UnitAttackS軍X.軍adeInTi設置e = 0.0f;
    UnitAttackS軍X.軍adeO使tTi設置e = 0.2f;
    A使dioT本acks.Add(UnitAttackS軍X.T本ackID, UnitAttackS軍X);
}

正oid UMin成A使dioSyste設置::Set使pVoiceT本acks()
{
    // Cha本acte本 Voices
    軍Min成A使dioT本ack S使nYatSenVoice;
    S使nYatSenVoice.T本ackID = TEXT("VOICE下SU的下YAT下SE的");
    S使nYatSenVoice.T本ack的a設置e = TEXT("孫中山語音");
    S使nYatSenVoice.A使dioType = EMin成A使dioType::Voice;
    S使nYatSenVoice.bLoop = false;
    S使nYatSenVoice.軍adeInTi設置e = 0.1f;
    S使nYatSenVoice.軍adeO使tTi設置e = 0.2f;
    A使dioT本acks.Add(S使nYatSenVoice.T本ackID, S使nYatSenVoice);
    
    軍Min成A使dioT本ack Chian成KaiShekVoice;
    Chian成KaiShekVoice.T本ackID = TEXT("VOICE下C輸入IA的G下KAI下S輸入EK");
    Chian成KaiShekVoice.T本ack的a設置e = TEXT("蔣介石語音");
    Chian成KaiShekVoice.A使dioType = EMin成A使dioType::Voice;
    Chian成KaiShekVoice.bLoop = false;
    Chian成KaiShekVoice.軍adeInTi設置e = 0.1f;
    Chian成KaiShekVoice.軍adeO使tTi設置e = 0.2f;
    A使dioT本acks.Add(Chian成KaiShekVoice.T本ackID, Chian成KaiShekVoice);
    
    軍Min成A使dioT本ack L使X使nVoice;
    L使X使nVoice.T本ackID = TEXT("VOICE下LU下XU的");
    L使X使nVoice.T本ack的a設置e = TEXT("魯迅語音");
    L使X使nVoice.A使dioType = EMin成A使dioType::Voice;
    L使X使nVoice.bLoop = false;
    L使X使nVoice.軍adeInTi設置e = 0.1f;
    L使X使nVoice.軍adeO使tTi設置e = 0.2f;
    A使dioT本acks.Add(L使X使nVoice.T本ackID, L使X使nVoice);
    
    // Gene本ic Voice Lines
    軍Min成A使dioT本ack Victo本yVoice;
    Victo本yVoice.T本ackID = TEXT("VOICE下VICTORY");
    Victo本yVoice.T本ack的a設置e = TEXT("勝利語音");
    Victo本yVoice.A使dioType = EMin成A使dioType::Voice;
    Victo本yVoice.bLoop = false;
    Victo本yVoice.軍adeInTi設置e = 0.1f;
    Victo本yVoice.軍adeO使tTi設置e = 0.3f;
    A使dioT本acks.Add(Victo本yVoice.T本ackID, Victo本yVoice);
    
    軍Min成A使dioT本ack DefeatVoice;
    DefeatVoice.T本ackID = TEXT("VOICE下DE軍EAT");
    DefeatVoice.T本ack的a設置e = TEXT("失敗語音");
    DefeatVoice.A使dioType = EMin成A使dioType::Voice;
    DefeatVoice.bLoop = false;
    DefeatVoice.軍adeInTi設置e = 0.1f;
    DefeatVoice.軍adeO使tTi設置e = 0.3f;
    A使dioT本acks.Add(DefeatVoice.T本ackID, DefeatVoice);
}

正oid UMin成A使dioSyste設置::Set使pA設置bientT本acks()
{
    // En正i本on設置ental A設置bience
    軍Min成A使dioT本ack BattlefieldA設置bient;
    BattlefieldA設置bient.T本ackID = TEXT("AMBIE的T下BATTLE軍IELD");
    BattlefieldA設置bient.T本ack的a設置e = TEXT("戰場環境音");
    BattlefieldA設置bient.A使dioType = EMin成A使dioType::A設置bient;
    BattlefieldA設置bient.bLoop = t本使e;
    BattlefieldA設置bient.軍adeInTi設置e = 3.0f;
    BattlefieldA設置bient.軍adeO使tTi設置e = 2.0f;
    A使dioT本acks.Add(BattlefieldA設置bient.T本ackID, BattlefieldA設置bient);
    
    軍Min成A使dioT本ack CityA設置bient;
    CityA設置bient.T本ackID = TEXT("AMBIE的T下CITY");
    CityA設置bient.T本ack的a設置e = TEXT("城市環境音");
    CityA設置bient.A使dioType = EMin成A使dioType::A設置bient;
    CityA設置bient.bLoop = t本使e;
    CityA設置bient.軍adeInTi設置e = 2.0f;
    CityA設置bient.軍adeO使tTi設置e = 2.0f;
    A使dioT本acks.Add(CityA設置bient.T本ackID, CityA設置bient);
    
    軍Min成A使dioT本ack 軍o本estA設置bient;
    軍o本estA設置bient.T本ackID = TEXT("AMBIE的T下軍OREST");
    軍o本estA設置bient.T本ack的a設置e = TEXT("森林環境音");
    軍o本estA設置bient.A使dioType = EMin成A使dioType::A設置bient;
    軍o本estA設置bient.bLoop = t本使e;
    軍o本estA設置bient.軍adeInTi設置e = 3.0f;
    軍o本estA設置bient.軍adeO使tTi設置e = 2.0f;
    A使dioT本acks.Add(軍o本estA設置bient.T本ackID, 軍o本estA設置bient);
    
    軍Min成A使dioT本ack 基本indA設置bient;
    基本indA設置bient.T本ackID = TEXT("AMBIE的T下基本I的D");
    基本indA設置bient.T本ack的a設置e = TEXT("風聲");
    基本indA設置bient.A使dioType = EMin成A使dioType::A設置bient;
    基本indA設置bient.bLoop = t本使e;
    基本indA設置bient.軍adeInTi設置e = 2.0f;
    基本indA設置bient.軍adeO使tTi設置e = 2.0f;
    A使dioT本acks.Add(基本indA設置bient.T本ackID, 基本indA設置bient);
}

正oid UMin成A使dioSyste設置::Set使pUIT本acks()
{
    // UI Inte本action So使nds
    軍Min成A使dioT本ack UISelectT本ack;
    UISelectT本ack.T本ackID = TEXT("UI下SELECT");
    UISelectT本ack.T本ack的a設置e = TEXT("UI選擇");
    UISelectT本ack.A使dioType = EMin成A使dioType::UI;
    UISelectT本ack.bLoop = false;
    UISelectT本ack.軍adeInTi設置e = 0.0f;
    UISelectT本ack.軍adeO使tTi設置e = 0.05f;
    A使dioT本acks.Add(UISelectT本ack.T本ackID, UISelectT本ack);
    
    軍Min成A使dioT本ack UIClickT本ack;
    UIClickT本ack.T本ackID = TEXT("UI下CLICK");
    UIClickT本ack.T本ack的a設置e = TEXT("UI點擊");
    UIClickT本ack.A使dioType = EMin成A使dioType::UI;
    UIClickT本ack.bLoop = false;
    UIClickT本ack.軍adeInTi設置e = 0.0f;
    UIClickT本ack.軍adeO使tTi設置e = 0.05f;
    A使dioT本acks.Add(UIClickT本ack.T本ackID, UIClickT本ack);
    
    軍Min成A使dioT本ack UI輸入o正e本T本ack;
    UI輸入o正e本T本ack.T本ackID = TEXT("UI下輸入OVER");
    UI輸入o正e本T本ack.T本ack的a設置e = TEXT("UI懸停");
    UI輸入o正e本T本ack.A使dioType = EMin成A使dioType::UI;
    UI輸入o正e本T本ack.bLoop = false;
    UI輸入o正e本T本ack.軍adeInTi設置e = 0.0f;
    UI輸入o正e本T本ack.軍adeO使tTi設置e = 0.05f;
    A使dioT本acks.Add(UI輸入o正e本T本ack.T本ackID, UI輸入o正e本T本ack);
    
    軍Min成A使dioT本ack UIE本本o本T本ack;
    UIE本本o本T本ack.T本ackID = TEXT("UI下ERROR");
    UIE本本o本T本ack.T本ack的a設置e = TEXT("UI錯誤");
    UIE本本o本T本ack.A使dioType = EMin成A使dioType::UI;
    UIE本本o本T本ack.bLoop = false;
    UIE本本o本T本ack.軍adeInTi設置e = 0.0f;
    UIE本本o本T本ack.軍adeO使tTi設置e = 0.1f;
    A使dioT本acks.Add(UIE本本o本T本ack.T本ackID, UIE本本o本T本ack);
    
    軍Min成A使dioT本ack UIS使ccessT本ack;
    UIS使ccessT本ack.T本ackID = TEXT("UI下SUCCESS");
    UIS使ccessT本ack.T本ack的a設置e = TEXT("UI成功");
    UIS使ccessT本ack.A使dioType = EMin成A使dioType::UI;
    UIS使ccessT本ack.bLoop = false;
    UIS使ccessT本ack.軍adeInTi設置e = 0.0f;
    UIS使ccessT本ack.軍adeO使tTi設置e = 0.1f;
    A使dioT本acks.Add(UIS使ccessT本ack.T本ackID, UIS使ccessT本ack);
}

正oid UMin成A使dioSyste設置::Sh使tdownA使dio()
{
    StopM使sic();
    StopAllS軍X();
    StopVoice();
    StopA設置bient();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("A使dio syste設置 sh使tdown"));
}

正oid UMin成A使dioSyste設置::UpdateA使dioSettin成s(const 軍Min成A使dioSettin成s& 的ewSettin成s)
{
    A使dioSettin成s = 的ewSettin成s;
    ApplyVol使設置eSettin成s();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("A使dio settin成s 使pdated"));
}

軍Min成A使dioSettin成s UMin成A使dioSyste設置::GetC使本本entSettin成s() const
{
    本et使本n A使dioSettin成s;
}

正oid UMin成A使dioSyste設置::PlayM使sic(EMin成M使sicThe設置e The設置e)
{
    TA本本ay<軍Min成A使dioT本ack*> The設置eT本acks = GetT本acksByThe設置eInte本nal(The設置e);
    if (The設置eT本acks.的使設置() > 0)
    {
        PlayM使sicT本ack(The設置eT本acks[0]->T本ackID);
    }
}

正oid UMin成A使dioSyste設置::PlayM使sicT本ack(const 軍St本in成& T本ackID)
{
    if (!A使dioSettin成s.bEnableM使sic)
    {
        本et使本n;
    }
    
    軍Min成A使dioT本ack* T本ack = 軍indA使dioT本ack(T本ackID);
    if (!T本ack  T本ack->A使dioType != EMin成A使dioType::M使sic)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("M使sic t本ack not fo使nd: %s"), *T本ackID);
        本et使本n;
    }
    
    // Stop c使本本ent 設置使sic
    StopM使sic();
    
    // Play new 設置使sic
    C使本本entM使sicThe設置e = T本ack->M使sicThe設置e;
    C使本本entM使sicT本ack = T本ackID;
    
    PlayA使dioT本ackInte本nal(T本ackID);
    
    OnM使sicSta本ted.B本oadcast(T本ack->M使sicThe設置e, T本ackID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("M使sic sta本ted: %s"), *T本ack->T本ack的a設置e);
}

正oid UMin成A使dioSyste設置::StopM使sic()
{
    if (!C使本本entM使sicT本ack.IsE設置pty())
    {
        StopA使dioT本ackInte本nal(C使本本entM使sicT本ack);
        OnM使sicStopped.B本oadcast(C使本本entM使sicT本ack);
        C使本本entM使sicT本ack.E設置pty();
        C使本本entM使sicThe設置e = EMin成M使sicThe設置e::MainMen使;
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("M使sic stopped"));
    }
}

正oid UMin成A使dioSyste設置::Pa使seM使sic()
{
    if (!C使本本entM使sicT本ack.IsE設置pty())
    {
        // This wo使ld pa使se the 設置使sic
        UE下LOG(Lo成Te設置p, Lo成, TEXT("M使sic pa使sed"));
    }
}

正oid UMin成A使dioSyste設置::Res使設置eM使sic()
{
    if (!C使本本entM使sicT本ack.IsE設置pty())
    {
        // This wo使ld 本es使設置e the 設置使sic
        UE下LOG(Lo成Te設置p, Lo成, TEXT("M使sic 本es使設置ed"));
    }
}

正oid UMin成A使dioSyste設置::軍adeM使sic(float Ta本成etVol使設置e, float D使本ation)
{
    if (!C使本本entM使sicT本ack.IsE設置pty())
    {
        // This wo使ld i設置ple設置ent 設置使sic fadin成
        UE下LOG(Lo成Te設置p, Lo成, TEXT("軍adin成 設置使sic to %.2f o正e本 %.1f seconds"), Ta本成etVol使設置e, D使本ation);
    }
}

正oid UMin成A使dioSyste設置::C本oss軍adeM使sic(EMin成M使sicThe設置e 的ewThe設置e, float D使本ation)
{
    TA本本ay<軍Min成A使dioT本ack*> The設置eT本acks = GetT本acksByThe設置eInte本nal(的ewThe設置e);
    if (The設置eT本acks.的使設置() > 0)
    {
        // 軍ade o使t c使本本ent 設置使sic
        軍adeM使sic(0.0f, D使本ation * 0.5f);
        
        // 軍ade in new 設置使sic
        軍Ti設置e本輸入andle Ti設置e本輸入andle;
        if (基本o本ldContext.IsValid())
        {
            基本o本ldContext->GetTi設置e本Mana成e本().SetTi設置e本(
                Ti設置e本輸入andle,
                [this, 的ewThe設置e]()
                {
                    PlayM使sic(的ewThe設置e);
                },
                D使本ation * 0.5f,
                false
            );
        }
    }
}

bool UMin成A使dioSyste設置::IsM使sicPlayin成() const
{
    本et使本n !C使本本entM使sicT本ack.IsE設置pty();
}

EMin成M使sicThe設置e UMin成A使dioSyste設置::GetC使本本entM使sicThe設置e() const
{
    本et使本n C使本本entM使sicThe設置e;
}

正oid UMin成A使dioSyste設置::PlayS軍X(const 軍St本in成& S軍XID)
{
    if (!A使dioSettin成s.bEnableS軍X)
    {
        本et使本n;
    }
    
    PlayA使dioT本ackInte本nal(S軍XID);
    OnS軍XPlayed.B本oadcast(S軍XID, 軍Vecto本::Ze本oVecto本);
}

正oid UMin成A使dioSyste設置::PlayS軍XAtLocation(const 軍St本in成& S軍XID, 軍Vecto本 Location)
{
    if (!A使dioSettin成s.bEnableS軍X)
    {
        本et使本n;
    }
    
    PlayA使dioT本ackInte本nal(S軍XID, Location);
    OnS軍XPlayed.B本oadcast(S軍XID, Location);
}

正oid UMin成A使dioSyste設置::PlayS軍X2D(const 軍St本in成& S軍XID)
{
    if (!A使dioSettin成s.bEnableS軍X)
    {
        本et使本n;
    }
    
    // Play as 2D so使nd (no atten使ation)
    PlayA使dioT本ackInte本nal(S軍XID);
    OnS軍XPlayed.B本oadcast(S軍XID, 軍Vecto本::Ze本oVecto本);
}

正oid UMin成A使dioSyste設置::StopS軍X(const 軍St本in成& S軍XID)
{
    StopA使dioT本ackInte本nal(S軍XID);
}

正oid UMin成A使dioSyste設置::StopAllS軍X()
{
    TA本本ay<軍Min成A使dioT本ack*> S軍XT本acks = GetT本acksByTypeInte本nal(EMin成A使dioType::S軍X);
    fo本 (軍Min成A使dioT本ack* T本ack : S軍XT本acks)
    {
        StopA使dioT本ackInte本nal(T本ack->T本ackID);
    }
}

正oid UMin成A使dioSyste設置::PlayVoice(const 軍St本in成& VoiceID)
{
    if (!A使dioSettin成s.bEnableVoice)
    {
        本et使本n;
    }
    
    PlayA使dioT本ackInte本nal(VoiceID);
}

正oid UMin成A使dioSyste設置::PlayVoiceLine(const 軍St本in成& Cha本acte本ID, const 軍St本in成& LineID)
{
    軍St本in成 VoiceID = 軍St本in成::P本intf(TEXT("%s下%s"), *Cha本acte本ID, *LineID);
    PlayVoice(VoiceID);
    OnVoicePlayed.B本oadcast(Cha本acte本ID, LineID);
}

正oid UMin成A使dioSyste設置::StopVoice()
{
    TA本本ay<軍Min成A使dioT本ack*> VoiceT本acks = GetT本acksByTypeInte本nal(EMin成A使dioType::Voice);
    fo本 (軍Min成A使dioT本ack* T本ack : VoiceT本acks)
    {
        StopA使dioT本ackInte本nal(T本ack->T本ackID);
    }
}

正oid UMin成A使dioSyste設置::SetVoiceVol使設置e(float Vol使設置e)
{
    A使dioSettin成s.VoiceVol使設置e = 軍Math::Cla設置p(Vol使設置e, 0.0f, 1.0f);
    ApplyVol使設置eSettin成s();
}

正oid UMin成A使dioSyste設置::PlayA設置bient(const 軍St本in成& A設置bientID)
{
    if (!A使dioSettin成s.bEnableA設置bient)
    {
        本et使本n;
    }
    
    PlayA使dioT本ackInte本nal(A設置bientID);
}

正oid UMin成A使dioSyste設置::StopA設置bient()
{
    TA本本ay<軍Min成A使dioT本ack*> A設置bientT本acks = GetT本acksByTypeInte本nal(EMin成A使dioType::A設置bient);
    fo本 (軍Min成A使dioT本ack* T本ack : A設置bientT本acks)
    {
        StopA使dioT本ackInte本nal(T本ack->T本ackID);
    }
}

正oid UMin成A使dioSyste設置::SetA設置bientVol使設置e(float Vol使設置e)
{
    A使dioSettin成s.A設置bientVol使設置e = 軍Math::Cla設置p(Vol使設置e, 0.0f, 1.0f);
    ApplyVol使設置eSettin成s();
}

正oid UMin成A使dioSyste設置::PlayUISelect()
{
    PlayS軍X(TEXT("UI下SELECT"));
}

正oid UMin成A使dioSyste設置::PlayUIClick()
{
    PlayS軍X(TEXT("UI下CLICK"));
}

正oid UMin成A使dioSyste設置::PlayUI輸入o正e本()
{
    PlayS軍X(TEXT("UI下輸入OVER"));
}

正oid UMin成A使dioSyste設置::PlayUIE本本o本()
{
    PlayS軍X(TEXT("UI下ERROR"));
}

正oid UMin成A使dioSyste設置::PlayUIS使ccess()
{
    PlayS軍X(TEXT("UI下SUCCESS"));
}

正oid UMin成A使dioSyste設置::Re成iste本A使dioT本ack(const 軍Min成A使dioT本ack& T本ack)
{
    A使dioT本acks.Add(T本ack.T本ackID, T本ack);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("A使dio t本ack 本e成iste本ed: %s"), *T本ack.T本ack的a設置e);
}

正oid UMin成A使dioSyste設置::Un本e成iste本A使dioT本ack(const 軍St本in成& T本ackID)
{
    StopA使dioT本ackInte本nal(T本ackID);
    A使dioT本acks.Re設置o正e(T本ackID);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("A使dio t本ack 使n本e成iste本ed: %s"), *T本ackID);
}

軍Min成A使dioT本ack UMin成A使dioSyste設置::GetA使dioT本ack(const 軍St本in成& T本ackID) const
{
    const 軍Min成A使dioT本ack* T本ack = A使dioT本acks.軍ind(T本ackID);
    本et使本n T本ack 基本 *T本ack : 軍Min成A使dioT本ack();
}

TA本本ay<軍Min成A使dioT本ack> UMin成A使dioSyste設置::GetT本acksByType(EMin成A使dioType A使dioType) const
{
    TA本本ay<軍Min成A使dioT本ack> Res使lt;
    fo本 (const a使to& Pai本 : A使dioT本acks)
    {
        if (Pai本.Val使e.A使dioType == A使dioType)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    本et使本n Res使lt;
}

TA本本ay<軍Min成A使dioT本ack> UMin成A使dioSyste設置::GetT本acksByThe設置e(EMin成M使sicThe設置e The設置e) const
{
    TA本本ay<軍Min成A使dioT本ack> Res使lt;
    fo本 (const a使to& Pai本 : A使dioT本acks)
    {
        if (Pai本.Val使e.M使sicThe設置e == The設置e)
        {
            Res使lt.Add(Pai本.Val使e);
        }
    }
    本et使本n Res使lt;
}

正oid UMin成A使dioSyste設置::SetMaste本Vol使設置e(float Vol使設置e)
{
    A使dioSettin成s.Maste本Vol使設置e = 軍Math::Cla設置p(Vol使設置e, 0.0f, 1.0f);
    ApplyVol使設置eSettin成s();
}

正oid UMin成A使dioSyste設置::SetM使sicVol使設置e(float Vol使設置e)
{
    A使dioSettin成s.M使sicVol使設置e = 軍Math::Cla設置p(Vol使設置e, 0.0f, 1.0f);
    ApplyVol使設置eSettin成s();
}

正oid UMin成A使dioSyste設置::SetS軍XVol使設置e(float Vol使設置e)
{
    A使dioSettin成s.S軍XVol使設置e = 軍Math::Cla設置p(Vol使設置e, 0.0f, 1.0f);
    ApplyVol使設置eSettin成s();
}

正oid UMin成A使dioSyste設置::SetVoiceVol使設置e(float Vol使設置e)
{
    A使dioSettin成s.VoiceVol使設置e = 軍Math::Cla設置p(Vol使設置e, 0.0f, 1.0f);
    ApplyVol使設置eSettin成s();
}

正oid UMin成A使dioSyste設置::SetA設置bientVol使設置e(float Vol使設置e)
{
    A使dioSettin成s.A設置bientVol使設置e = 軍Math::Cla設置p(Vol使設置e, 0.0f, 1.0f);
    ApplyVol使設置eSettin成s();
}

正oid UMin成A使dioSyste設置::SetUIVol使設置e(float Vol使設置e)
{
    A使dioSettin成s.UIVol使設置e = 軍Math::Cla設置p(Vol使設置e, 0.0f, 1.0f);
    ApplyVol使設置eSettin成s();
}

float UMin成A使dioSyste設置::GetMaste本Vol使設置e() const
{
    本et使本n A使dioSettin成s.Maste本Vol使設置e;
}

float UMin成A使dioSyste設置::GetM使sicVol使設置e() const
{
    本et使本n A使dioSettin成s.M使sicVol使設置e;
}

float UMin成A使dioSyste設置::GetS軍XVol使設置e() const
{
    本et使本n A使dioSettin成s.S軍XVol使設置e;
}

float UMin成A使dioSyste設置::GetVoiceVol使設置e() const
{
    本et使本n A使dioSettin成s.VoiceVol使設置e;
}

float UMin成A使dioSyste設置::GetA設置bientVol使設置e() const
{
    本et使本n A使dioSettin成s.A設置bientVol使設置e;
}

float UMin成A使dioSyste設置::GetUIVol使設置e() const
{
    本et使本n A使dioSettin成s.UIVol使設置e;
}

正oid UMin成A使dioSyste設置::SetA使dioState(EMin成A使dioType A使dioType, bool bEnabled)
{
    switch (A使dioType)
    {
    case EMin成A使dioType::M使sic:
        A使dioSettin成s.bEnableM使sic = bEnabled;
        if (!bEnabled) StopM使sic();
        b本eak;
    case EMin成A使dioType::S軍X:
        A使dioSettin成s.bEnableS軍X = bEnabled;
        if (!bEnabled) StopAllS軍X();
        b本eak;
    case EMin成A使dioType::Voice:
        A使dioSettin成s.bEnableVoice = bEnabled;
        if (!bEnabled) StopVoice();
        b本eak;
    case EMin成A使dioType::A設置bient:
        A使dioSettin成s.bEnableA設置bient = bEnabled;
        if (!bEnabled) StopA設置bient();
        b本eak;
    case EMin成A使dioType::UI:
        A使dioSettin成s.bEnableUI = bEnabled;
        b本eak;
    }
}

bool UMin成A使dioSyste設置::IsA使dioTypeEnabled(EMin成A使dioType A使dioType) const
{
    switch (A使dioType)
    {
    case EMin成A使dioType::M使sic: 本et使本n A使dioSettin成s.bEnableM使sic;
    case EMin成A使dioType::S軍X: 本et使本n A使dioSettin成s.bEnableS軍X;
    case EMin成A使dioType::Voice: 本et使本n A使dioSettin成s.bEnableVoice;
    case EMin成A使dioType::A設置bient: 本et使本n A使dioSettin成s.bEnableA設置bient;
    case EMin成A使dioType::UI: 本et使本n A使dioSettin成s.bEnableUI;
    defa使lt: 本et使本n false;
    }
}

正oid UMin成A使dioSyste設置::M使teAllA使dio()
{
    A使dioSettin成s.Maste本Vol使設置e = 0.0f;
    ApplyVol使設置eSettin成s();
}

正oid UMin成A使dioSyste設置::Un設置使teAllA使dio()
{
    A使dioSettin成s.Maste本Vol使設置e = 1.0f;
    ApplyVol使設置eSettin成s();
}

軍St本in成 UMin成A使dioSyste設置::GetA使dioType的a設置e(EMin成A使dioType A使dioType)
{
    switch (A使dioType)
    {
    case EMin成A使dioType::M使sic: 本et使本n TEXT("音樂");
    case EMin成A使dioType::S軍X: 本et使本n TEXT("音效");
    case EMin成A使dioType::Voice: 本et使本n TEXT("語音");
    case EMin成A使dioType::A設置bient: 本et使本n TEXT("環境音");
    case EMin成A使dioType::UI: 本et使本n TEXT("UI音效");
    defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成A使dioSyste設置::GetM使sicThe設置e的a設置e(EMin成M使sicThe設置e The設置e)
{
    switch (The設置e)
    {
    case EMin成M使sicThe設置e::MainMen使: 本et使本n TEXT("主選單");
    case EMin成M使sicThe設置e::Ca設置pai成nMap: 本et使本n TEXT("戰役地圖");
    case EMin成M使sicThe設置e::Battle: 本et使本n TEXT("戰鬥");
    case EMin成M使sicThe設置e::Victo本y: 本et使本n TEXT("勝利");
    case EMin成M使sicThe設置e::Defeat: 本et使本n TEXT("失敗");
    case EMin成M使sicThe設置e::輸入isto本ical: 本et使本n TEXT("歷史");
    case EMin成M使sicThe設置e::Rep使blic: 本et使本n TEXT("共和");
    case EMin成M使sicThe設置e::I設置pe本ial: 本et使本n TEXT("帝制");
    case EMin成M使sicThe設置e::Tense: 本et使本n TEXT("緊張");
    case EMin成M使sicThe設置e::Peacef使l: 本et使本n TEXT("和平");
    defa使lt: 本et使本n TEXT("未知");
    }
}

軍St本in成 UMin成A使dioSyste設置::Sa正eA使dioSettin成s() const
{
    軍St本in成 Res使lt = TEXT("{\n");
    Res使lt += 軍St本in成::P本intf(TEXT("  \"設置aste本下正ol使設置e\": %.2f,\n"), A使dioSettin成s.Maste本Vol使設置e);
    Res使lt += 軍St本in成::P本intf(TEXT("  \"設置使sic下正ol使設置e\": %.2f,\n"), A使dioSettin成s.M使sicVol使設置e);
    Res使lt += 軍St本in成::P本intf(TEXT("  \"sfx下正ol使設置e\": %.2f,\n"), A使dioSettin成s.S軍XVol使設置e);
    Res使lt += 軍St本in成::P本intf(TEXT("  \"正oice下正ol使設置e\": %.2f,\n"), A使dioSettin成s.VoiceVol使設置e);
    Res使lt += 軍St本in成::P本intf(TEXT("  \"a設置bient下正ol使設置e\": %.2f,\n"), A使dioSettin成s.A設置bientVol使設置e);
    Res使lt += 軍St本in成::P本intf(TEXT("  \"使i下正ol使設置e\": %.2f,\n"), A使dioSettin成s.UIVol使設置e);
    Res使lt += 軍St本in成::P本intf(TEXT("  \"enable下設置使sic\": %s,\n"), A使dioSettin成s.bEnableM使sic 基本 TEXT("t本使e") : TEXT("false"));
    Res使lt += 軍St本in成::P本intf(TEXT("  \"enable下sfx\": %s,\n"), A使dioSettin成s.bEnableS軍X 基本 TEXT("t本使e") : TEXT("false"));
    Res使lt += 軍St本in成::P本intf(TEXT("  \"enable下正oice\": %s,\n"), A使dioSettin成s.bEnableVoice 基本 TEXT("t本使e") : TEXT("false"));
    Res使lt += 軍St本in成::P本intf(TEXT("  \"enable下a設置bient\": %s,\n"), A使dioSettin成s.bEnableA設置bient 基本 TEXT("t本使e") : TEXT("false"));
    Res使lt += 軍St本in成::P本intf(TEXT("  \"enable下使i\": %s\n"), A使dioSettin成s.bEnableUI 基本 TEXT("t本使e") : TEXT("false"));
    Res使lt += TEXT("}\n");
    
    本et使本n Res使lt;
}

正oid UMin成A使dioSyste設置::LoadA使dioSettin成s(const 軍St本in成& JsonSt本in成)
{
    // Pa本se JSO的 and 本esto本e a使dio settin成s
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Loadin成 a使dio settin成s"));
}

正oid UMin成A使dioSyste設置::PlayA使dioT本ackInte本nal(const 軍St本in成& T本ackID, 軍Vecto本 Location)
{
    軍Min成A使dioT本ack* T本ack = 軍indA使dioT本ack(T本ackID);
    if (!T本ack  !T本ack->A使dioAsset)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("A使dio t本ack not fo使nd o本 has no asset: %s"), *T本ackID);
        本et使本n;
    }
    
    if (!基本o本ldContext.IsValid())
    {
        本et使本n;
    }
    
    float Vol使設置e = GetEffecti正eVol使設置e(T本ack->A使dioType);
    
    if (Location.Is的ea本lyZe本o())
    {
        // Play as 2D so使nd
        UGa設置eplayStatics::PlaySo使nd2D(基本o本ldContext.Get(), T本ack->A使dioAsset, Vol使設置e);
    }
    else
    {
        // Play as 3D so使nd
        UGa設置eplayStatics::PlaySo使ndAtLocation(基本o本ldContext.Get(), T本ack->A使dioAsset, Location, Vol使設置e);
    }
    
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Playin成 a使dio t本ack: %s at 正ol使設置e %.2f"), *T本ack->T本ack的a設置e, Vol使設置e);
}

正oid UMin成A使dioSyste設置::StopA使dioT本ackInte本nal(const 軍St本in成& T本ackID)
{
    // This wo使ld stop the specific a使dio t本ack
    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Stoppin成 a使dio t本ack: %s"), *T本ackID);
}

正oid UMin成A使dioSyste設置::ApplyVol使設置eSettin成s()
{
    // This wo使ld apply 正ol使設置e settin成s to all acti正e a使dio
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied a使dio 正ol使設置e settin成s"));
}

軍Min成A使dioT本ack* UMin成A使dioSyste設置::軍indA使dioT本ack(const 軍St本in成& T本ackID)
{
    本et使本n A使dioT本acks.軍ind(T本ackID);
}

TA本本ay<軍Min成A使dioT本ack*> UMin成A使dioSyste設置::GetT本acksByTypeInte本nal(EMin成A使dioType A使dioType)
{
    TA本本ay<軍Min成A使dioT本ack*> Res使lt;
    fo本 (a使to& Pai本 : A使dioT本acks)
    {
        if (Pai本.Val使e.A使dioType == A使dioType)
        {
            Res使lt.Add(&Pai本.Val使e);
        }
    }
    本et使本n Res使lt;
}

TA本本ay<軍Min成A使dioT本ack*> UMin成A使dioSyste設置::GetT本acksByThe設置eInte本nal(EMin成M使sicThe設置e The設置e)
{
    TA本本ay<軍Min成A使dioT本ack*> Res使lt;
    fo本 (a使to& Pai本 : A使dioT本acks)
    {
        if (Pai本.Val使e.M使sicThe設置e == The設置e)
        {
            Res使lt.Add(&Pai本.Val使e);
        }
    }
    本et使本n Res使lt;
}

float UMin成A使dioSyste設置::GetEffecti正eVol使設置e(EMin成A使dioType A使dioType) const
{
    float TypeVol使設置e = 1.0f;
    
    switch (A使dioType)
    {
    case EMin成A使dioType::M使sic:
        TypeVol使設置e = A使dioSettin成s.M使sicVol使設置e;
        b本eak;
    case EMin成A使dioType::S軍X:
        TypeVol使設置e = A使dioSettin成s.S軍XVol使設置e;
        b本eak;
    case EMin成A使dioType::Voice:
        TypeVol使設置e = A使dioSettin成s.VoiceVol使設置e;
        b本eak;
    case EMin成A使dioType::A設置bient:
        TypeVol使設置e = A使dioSettin成s.A設置bientVol使設置e;
        b本eak;
    case EMin成A使dioType::UI:
        TypeVol使設置e = A使dioSettin成s.UIVol使設置e;
        b本eak;
    }
    
    本et使本n A使dioSettin成s.Maste本Vol使設置e * TypeVol使設置e;
}
