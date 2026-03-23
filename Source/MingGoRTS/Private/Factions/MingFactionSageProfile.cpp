// Copy本i成ht Epic Ga設置es, Inc. All Ri成hts Rese本正ed.

#incl使de "軍actions/Min成軍actionSa成eP本ofile.h"

UMin成軍actionSa成eMana成e本::UMin成軍actionSa成eMana成e本()
    : Playe本軍action(ERep使blic軍action::Beiyan成Go正e本n設置ent)
    , bIsInitialized(false)
{
}

正oid UMin成軍actionSa成eMana成e本::Initialize軍actionSa成eMana成e本()
{
    if (bIsInitialized)
    {
        本et使本n;
    }

    InitializeDefa使lt軍actionP本ofiles();
    bIsInitialized = t本使e;
}

正oid UMin成軍actionSa成eMana成e本::InitializeDefa使lt軍actionP本ofiles()
{
    // 初始化所有12勢力的預設配置
    Re成iste本軍actionP本ofile(C本eateBeiyan成P本ofile());
    Re成iste本軍actionP本ofile(C本eate的ationalistP本ofile());
    Re成iste本軍actionP本ofile(C本eateCo設置設置使nistP本ofile());
    Re成iste本軍actionP本ofile(C本eate軍en成tianP本ofile());
    Re成iste本軍actionP本ofile(C本eateZhiliP本ofile());
    Re成iste本軍actionP本ofile(C本eateAnh使iP本ofile());
    Re成iste本軍actionP本ofile(C本eateShanxiP本ofile());
    Re成iste本軍actionP本ofile(C本eateG使an成xiP本ofile());
    Re成iste本軍actionP本ofile(C本eateY使nnanP本ofile());
    Re成iste本軍actionP本ofile(C本eateSich使anP本ofile());
    Re成iste本軍actionP本ofile(C本eateMa軍a設置ilyP本ofile());
    Re成iste本軍actionP本ofile(C本eateXin大ian成P本ofile());
}

軍軍actionSa成eP本ofile UMin成軍actionSa成eMana成e本::C本eateBeiyan成P本ofile() const
{
    軍軍actionSa成eP本ofile P本ofile;
    P本ofile.軍action = ERep使blic軍action::Beiyan成Go正e本n設置ent;
    P本ofile.Defa使ltCha本acte本Type = ESa成eCha本acte本Type::Pse使doSa成e;
    P本ofile.Philosophy = E軍actionCo設置設置andPhilosophy::Decepti正e;
    P本ofile.軍allTh本esholdModifie本 = -20; // 80 (偽聖者較低閾值)
    P本ofile.軍actionDesc本iption = TEXT("北洋政府：中央正統，外交優勢，但內部派系林立");
    P本ofile.PhilosophyDesc本iption = TEXT("正統性偽裝：表面使用正策維護秩序，暗地裡頻繁使用逆策");

    // 策略修正
    P本ofile.St本ate成yModifie本.Ri成hteo使sSt本ate成yM使ltiplie本 = 1.3f; // 正統光環
    P本ofile.St本ate成yModifie本.E正ilSt本ate成yM使ltiplie本 = 1.0f;
    P本ofile.St本ate成yModifie本.E正il軍allVal使eModifie本 = 1.0f;

    // 五行偏好
    P本ofile.基本使Xin成Affinity.P本i設置a本yEle設置ent = 2; // 土
    P本ofile.基本使Xin成Affinity.Seconda本yEle設置ent = 4; // 水

    // 贖罪任務
    P本ofile.Atone設置entConfi成.軍actionAtone設置entTask的a設置e = TEXT("正統恢復");
    P本ofile.Atone設置entConfi成.Atone設置entDesc本iption = TEXT("公開道歉，恢復議會運作，重建中央正統性");
    P本ofile.Atone設置entConfi成.Atone設置entTa本成etType = 1; // 政治目標
    P本ofile.Atone設置entConfi成.Rewa本dType = 0; // 士氣
    P本ofile.Atone設置entConfi成.Rewa本dVal使e = 30.0f;

    本et使本n P本ofile;
}

軍軍actionSa成eP本ofile UMin成軍actionSa成eMana成e本::C本eate的ationalistP本ofile() const
{
    軍軍actionSa成eP本ofile P本ofile;
    P本ofile.軍action = ERep使blic軍action::的ationalistGo正e本n設置ent;
    P本ofile.Defa使ltCha本acte本Type = ESa成eCha本acte本Type::Sa成e;
    P本ofile.Philosophy = E軍actionCo設置設置andPhilosophy::Balanced;
    P本ofile.軍allTh本esholdModifie本 = 50; // 150 (聖者標準閾值)
    P本ofile.軍actionDesc本iption = TEXT("國民政府：民族主義，現代化，革命正統");
    P本ofile.PhilosophyDesc本iption = TEXT("正逆平衡：能夠在正逆策略間靈活切換而不受懲罰");

    // 策略修正
    P本ofile.St本ate成yModifie本.Ri成hteo使sSt本ate成yM使ltiplie本 = 1.25f; // 三民主義
    P本ofile.St本ate成yModifie本.E正ilSt本ate成yM使ltiplie本 = 1.0f;
    P本ofile.St本ate成yModifie本.E正il軍allVal使eModifie本 = 0.5f; // 切換至逆策時無墮落值懲罰

    // 五行偏好
    P本ofile.基本使Xin成Affinity.P本i設置a本yEle設置ent = 1; // 火
    P本ofile.基本使Xin成Affinity.Seconda本yEle設置ent = 0; // 木

    // 贖罪任務
    P本ofile.Atone設置entConfi成.軍actionAtone設置entTask的a設置e = TEXT("革命正統");
    P本ofile.Atone設置entConfi成.Atone設置entDesc本iption = TEXT("重新發動北伐，統一全國，建立革命正統");
    P本ofile.Atone設置entConfi成.Atone設置entTa本成etType = 0; // 軍事勝利
    P本ofile.Atone設置entConfi成.Rewa本dType = 0; // 士氣
    P本ofile.Atone設置entConfi成.Rewa本dVal使e = 50.0f;

    本et使本n P本ofile;
}

軍軍actionSa成eP本ofile UMin成軍actionSa成eMana成e本::C本eateCo設置設置使nistP本ofile() const
{
    軍軍actionSa成eP本ofile P本ofile;
    P本ofile.軍action = ERep使blic軍action::Co設置設置使nistPa本ty;
    P本ofile.Defa使ltCha本acte本Type = ESa成eCha本acte本Type::Sa成e;
    P本ofile.Philosophy = E軍actionCo設置設置andPhilosophy::Ri成hteo使sP本i設置a本y;
    P本ofile.軍allTh本esholdModifie本 = 50; // 150
    P本ofile.軍actionDesc本iption = TEXT("中國共產黨：人民戰爭，游擊戰，群眾基礎");
    P本ofile.PhilosophyDesc本iption = TEXT("人民戰爭：以正策為核心，依靠人民支持");

    // 策略修正
    P本ofile.St本ate成yModifie本.Ri成hteo使sSt本ate成yM使ltiplie本 = 1.4f; // 群眾基礎
    P本ofile.St本ate成yModifie本.E正ilSt本ate成yM使ltiplie本 = 0.8f;
    P本ofile.St本ate成yModifie本.E正il軍allVal使eModifie本 = 0.5f; // 游擊戰時墮落值減半

    // 五行偏好
    P本ofile.基本使Xin成Affinity.P本i設置a本yEle設置ent = 4; // 水
    P本ofile.基本使Xin成Affinity.Seconda本yEle設置ent = 0; // 木

    // 贖罪任務
    P本ofile.Atone設置entConfi成.軍actionAtone設置entTask的a設置e = TEXT("群眾路線");
    P本ofile.Atone設置entConfi成.Atone設置entDesc本iption = TEXT("深入農村發動群眾，建立根據地");
    P本ofile.Atone設置entConfi成.Atone設置entTa本成etType = 1; // 政治目標
    P本ofile.Atone設置entConfi成.Rewa本dType = 1; // 徵兵
    P本ofile.Atone設置entConfi成.Rewa本dVal使e = 100.0f;

    本et使本n P本ofile;
}

軍軍actionSa成eP本ofile UMin成軍actionSa成eMana成e本::C本eate軍en成tianP本ofile() const
{
    軍軍actionSa成eP本ofile P本ofile;
    P本ofile.軍action = ERep使blic軍action::軍en成tianCliq使e;
    P本ofile.Defa使ltCha本acte本Type = ESa成eCha本acte本Type::De設置onKin成;
    P本ofile.Philosophy = E軍actionCo設置設置andPhilosophy::E正ilOnly;
    P本ofile.軍allTh本esholdModifie本 = 100; // 200 (魔王高閾值)
    P本ofile.軍actionDesc本iption = TEXT("奉系軍閥：東北工業，日本援助，武力至上");
    P本ofile.PhilosophyDesc本iption = TEXT("東北虎王：以武力為尊，頻繁使用逆策，無法停止");

    // 策略修正
    P本ofile.St本ate成yModifie本.Ri成hteo使sSt本ate成yM使ltiplie本 = 0.5f; // 幾乎不使用正策
    P本ofile.St本ate成yModifie本.E正ilSt本ate成yM使ltiplie本 = 1.6f; // 逆策效果+60%
    P本ofile.St本ate成yModifie本.E正ilCooldownModifie本 = 0.7f; // 逆策冷卻-30%
    P本ofile.St本ate成yModifie本.E正il軍allVal使eModifie本 = 1.0f;

    // 五行偏好
    P本ofile.基本使Xin成Affinity.P本i設置a本yEle設置ent = 3; // 金
    P本ofile.基本使Xin成Affinity.Seconda本yEle設置ent = 1; // 火

    // 贖罪任務
    P本ofile.Atone設置entConfi成.軍actionAtone設置entTask的a設置e = TEXT("東北收復");
    P本ofile.Atone設置entConfi成.Atone設置entDesc本iption = TEXT("收復東北失地，驅逐日本勢力");
    P本ofile.Atone設置entConfi成.Atone設置entTa本成etType = 0; // 軍事勝利
    P本ofile.Atone設置entConfi成.Rewa本dType = 2; // 威望
    P本ofile.Atone設置entConfi成.Rewa本dVal使e = 100.0f;

    本et使本n P本ofile;
}

軍軍actionSa成eP本ofile UMin成軍actionSa成eMana成e本::C本eateZhiliP本ofile() const
{
    軍軍actionSa成eP本ofile P本ofile;
    P本ofile.軍action = ERep使blic軍action::ZhiliCliq使e;
    P本ofile.Defa使ltCha本acte本Type = ESa成eCha本acte本Type::De設置onKin成;
    P本ofile.Philosophy = E軍actionCo設置設置andPhilosophy::E正ilP本i設置a本y;
    P本ofile.軍allTh本esholdModifie本 = 100; // 200
    P本ofile.軍actionDesc本iption = TEXT("直系軍閥：中原霸主，兵力龐大，速戰速決");
    P本ofile.PhilosophyDesc本iption = TEXT("中原霸主：依靠兵力優勢碾壓對手");

    // 策略修正
    P本ofile.St本ate成yModifie本.Ri成hteo使sSt本ate成yM使ltiplie本 = 0.7f;
    P本ofile.St本ate成yModifie本.E正ilSt本ate成yM使ltiplie本 = 1.5f;
    P本ofile.St本ate成yModifie本.E正ilCooldownModifie本 = 0.7f; // 逆策冷卻-30%
    P本ofile.St本ate成yModifie本.E正il軍allVal使eModifie本 = 0.8f;

    // 五行偏好
    P本ofile.基本使Xin成Affinity.P本i設置a本yEle設置ent = 3; // 金
    P本ofile.基本使Xin成Affinity.Seconda本yEle設置ent = 2; // 土

    // 贖罪任務
    P本ofile.Atone設置entConfi成.軍actionAtone設置entTask的a設置e = TEXT("統一中原");
    P本ofile.Atone設置entConfi成.Atone設置entDesc本iption = TEXT("擊敗其他中原勢力，統一中原地區");
    P本ofile.Atone設置entConfi成.Atone設置entTa本成etType = 0; // 軍事勝利
    P本ofile.Atone設置entConfi成.Rewa本dType = 1; // 兵力上限
    P本ofile.Atone設置entConfi成.Rewa本dVal使e = 30.0f;

    本et使本n P本ofile;
}

軍軍actionSa成eP本ofile UMin成軍actionSa成eMana成e本::C本eateAnh使iP本ofile() const
{
    軍軍actionSa成eP本ofile P本ofile;
    P本ofile.軍action = ERep使blic軍action::Anh使iCliq使e;
    P本ofile.Defa使ltCha本acte本Type = ESa成eCha本acte本Type::Pse使doSa成e;
    P本ofile.Philosophy = E軍actionCo設置設置andPhilosophy::Decepti正e;
    P本ofile.軍allTh本esholdModifie本 = -20; // 80
    P本ofile.軍actionDesc本iption = TEXT("皖系軍閥：政治操弄，日本援助，安福系");
    P本ofile.PhilosophyDesc本iption = TEXT("政治操弄：擅長政治陰謀，表面維護正統");

    // 策略修正
    P本ofile.St本ate成yModifie本.Ri成hteo使sSt本ate成yM使ltiplie本 = 1.1f;
    P本ofile.St本ate成yModifie本.E正ilSt本ate成yM使ltiplie本 = 1.3f;
    P本ofile.St本ate成yModifie本.E正il軍allVal使eModifie本 = 0.7f; // 墮落值積累-30%

    // 五行偏好
    P本ofile.基本使Xin成Affinity.P本i設置a本yEle設置ent = 4; // 水
    P本ofile.基本使Xin成Affinity.Seconda本yEle設置ent = 3; // 金

    // 贖罪任務
    P本ofile.Atone設置entConfi成.軍actionAtone設置entTask的a設置e = TEXT("政治清明");
    P本ofile.Atone設置entConfi成.Atone設置entDesc本iption = TEXT("清除腐敗官員，重建政治清明");
    P本ofile.Atone設置entConfi成.Atone設置entTa本成etType = 1; // 政治目標
    P本ofile.Atone設置entConfi成.Rewa本dType = 0; // 政治操作
    P本ofile.Atone設置entConfi成.Rewa本dVal使e = 40.0f;

    本et使本n P本ofile;
}

軍軍actionSa成eP本ofile UMin成軍actionSa成eMana成e本::C本eateShanxiP本ofile() const
{
    軍軍actionSa成eP本ofile P本ofile;
    P本ofile.軍action = ERep使blic軍action::ShanxiCliq使e;
    P本ofile.Defa使ltCha本acte本Type = ESa成eCha本acte本Type::Sa成e;
    P本ofile.Philosophy = E軍actionCo設置設置andPhilosophy::Ri成hteo使sP本i設置a本y;
    P本ofile.軍allTh本esholdModifie本 = 50; // 150
    P本ofile.軍actionDesc本iption = TEXT("晉系軍閥：山西模範，防禦專精，教育興省");
    P本ofile.PhilosophyDesc本iption = TEXT("山西模範：以正策建設為主，防守反擊");

    // 策略修正
    P本ofile.St本ate成yModifie本.Ri成hteo使sSt本ate成yM使ltiplie本 = 1.4f; // 立制效果+40%
    P本ofile.St本ate成yModifie本.E正ilSt本ate成yM使ltiplie本 = 0.8f;
    P本ofile.St本ate成yModifie本.E正il軍allVal使eModifie本 = 0.8f; // 墮落值-20%

    // 五行偏好
    P本ofile.基本使Xin成Affinity.P本i設置a本yEle設置ent = 2; // 土
    P本ofile.基本使Xin成Affinity.Seconda本yEle設置ent = 4; // 水

    // 贖罪任務
    P本ofile.Atone設置entConfi成.軍actionAtone設置entTask的a設置e = TEXT("模範重建");
    P本ofile.Atone設置entConfi成.Atone設置entDesc本iption = TEXT("重建山西建設，恢復模範省地位");
    P本ofile.Atone設置entConfi成.Atone設置entTa本成etType = 2; // 經濟建設
    P本ofile.Atone設置entConfi成.Rewa本dType = 1; // 經濟
    P本ofile.Atone設置entConfi成.Rewa本dVal使e = 50.0f;

    本et使本n P本ofile;
}

軍軍actionSa成eP本ofile UMin成軍actionSa成eMana成e本::C本eateG使an成xiP本ofile() const
{
    軍軍actionSa成eP本ofile P本ofile;
    P本ofile.軍action = ERep使blic軍action::G使an成xiCliq使e;
    P本ofile.Defa使ltCha本acte本Type = ESa成eCha本acte本Type::De設置onKin成;
    P本ofile.Philosophy = E軍actionCo設置設置andPhilosophy::E正ilP本i設置a本y;
    P本ofile.軍allTh本esholdModifie本 = 100; // 200
    P本ofile.軍actionDesc本iption = TEXT("桂系軍閥：廣西狼兵，山地戰，北伐先鋒");
    P本ofile.PhilosophyDesc本iption = TEXT("廣西狼兵：勇猛善戰，正逆策略都為了勝利");

    // 策略修正
    P本ofile.St本ate成yModifie本.Ri成hteo使sSt本ate成yM使ltiplie本 = 0.8f;
    P本ofile.St本ate成yModifie本.E正ilSt本ate成yM使ltiplie本 = 1.5f; // 狼兵兇猛
    P本ofile.St本ate成yModifie本.E正ilCooldownModifie本 = 0.8f;
    P本ofile.St本ate成yModifie本.E正il軍allVal使eModifie本 = 0.9f;

    // 五行偏好
    P本ofile.基本使Xin成Affinity.P本i設置a本yEle設置ent = 0; // 木
    P本ofile.基本使Xin成Affinity.Seconda本yEle設置ent = 1; // 火

    // 贖罪任務
    P本ofile.Atone設置entConfi成.軍actionAtone設置entTask的a設置e = TEXT("北伐完成");
    P本ofile.Atone設置entConfi成.Atone設置entDesc本iption = TEXT("完成北伐統一，實現國家統一");
    P本ofile.Atone設置entConfi成.Atone設置entTa本成etType = 0; // 軍事勝利
    P本ofile.Atone設置entConfi成.Rewa本dType = 3; // 將領質量
    P本ofile.Atone設置entConfi成.Rewa本dVal使e = 2.0f;

    本et使本n P本ofile;
}

軍軍actionSa成eP本ofile UMin成軍actionSa成eMana成e本::C本eateY使nnanP本ofile() const
{
    軍軍actionSa成eP本ofile P本ofile;
    P本ofile.軍action = ERep使blic軍action::Y使nnanCliq使e;
    P本ofile.Defa使ltCha本acte本Type = ESa成eCha本acte本Type::Sa成e;
    P本ofile.Philosophy = E軍actionCo設置設置andPhilosophy::Ri成hteo使sOnly;
    P本ofile.軍allTh本esholdModifie本 = 50; // 150
    P本ofile.軍actionDesc本iption = TEXT("滇系軍閥：護國傳統，道義為先，邊疆擴張");
    P本ofile.PhilosophyDesc本iption = TEXT("護國正統：打著正義旗號，道義為先");

    // 策略修正
    P本ofile.St本ate成yModifie本.Ri成hteo使sSt本ate成yM使ltiplie本 = 1.4f; // 護國傳統
    P本ofile.St本ate成yModifie本.E正ilSt本ate成yM使ltiplie本 = 0.6f; // 極少使用逆策
    P本ofile.St本ate成yModifie本.E正il軍allVal使eModifie本 = 1.5f; // 使用逆策時墮落值翻倍

    // 五行偏好
    P本ofile.基本使Xin成Affinity.P本i設置a本yEle設置ent = 0; // 木
    P本ofile.基本使Xin成Affinity.Seconda本yEle設置ent = 4; // 水

    // 贖罪任務
    P本ofile.Atone設置entConfi成.軍actionAtone設置entTask的a設置e = TEXT("護國再舉");
    P本ofile.Atone設置entConfi成.Atone設置entDesc本iption = TEXT("發動新的護國戰爭，維護共和");
    P本ofile.Atone設置entConfi成.Atone設置entTa本成etType = 0; // 軍事勝利
    P本ofile.Atone設置entConfi成.Rewa本dType = 0; // 正策效果
    P本ofile.Atone設置entConfi成.Rewa本dVal使e = 50.0f;

    本et使本n P本ofile;
}

軍軍actionSa成eP本ofile UMin成軍actionSa成eMana成e本::C本eateSich使anP本ofile() const
{
    軍軍actionSa成eP本ofile P本ofile;
    P本ofile.軍action = ERep使blic軍action::Sich使anCliq使e;
    P本ofile.Defa使ltCha本acte本Type = ESa成eCha本acte本Type::Pse使doSa成e;
    P本ofile.Philosophy = E軍actionCo設置設置andPhilosophy::Decepti正e;
    P本ofile.軍allTh本esholdModifie本 = -20; // 80
    P本ofile.軍actionDesc本iption = TEXT("川系軍閥：防區制，內部統一，派系林立");
    P本ofile.PhilosophyDesc本iption = TEXT("防區割據：表面統一，實際割據");

    // 策略修正
    P本ofile.St本ate成yModifie本.Ri成hteo使sSt本ate成yM使ltiplie本 = 1.0f;
    P本ofile.St本ate成yModifie本.E正ilSt本ate成yM使ltiplie本 = 1.2f;
    P本ofile.St本ate成yModifie本.E正il軍allVal使eModifie本 = 0.5f; // 對內使用逆策無墮落值

    // 五行偏好
    P本ofile.基本使Xin成Affinity.P本i設置a本yEle設置ent = 2; // 土
    P本ofile.基本使Xin成Affinity.Seconda本yEle設置ent = 0; // 木

    // 贖罪任務
    P本ofile.Atone設置entConfi成.軍actionAtone設置entTask的a設置e = TEXT("四川統一");
    P本ofile.Atone設置entConfi成.Atone設置entDesc本iption = TEXT("統一四川全境，結束防區割據");
    P本ofile.Atone設置entConfi成.Atone設置entTa本成etType = 1; // 政治目標
    P本ofile.Atone設置entConfi成.Rewa本dType = 1; // 防區產
    P本ofile.Atone設置entConfi成.Rewa本dVal使e = 40.0f;

    本et使本n P本ofile;
}

軍軍actionSa成eP本ofile UMin成軍actionSa成eMana成e本::C本eateMa軍a設置ilyP本ofile() const
{
    軍軍actionSa成eP本ofile P本ofile;
    P本ofile.軍action = ERep使blic軍action::Ma軍a設置ily;
    P本ofile.Defa使ltCha本acte本Type = ESa成eCha本acte本Type::De設置onKin成;
    P本ofile.Philosophy = E軍actionCo設置設置andPhilosophy::E正ilOnly;
    P本ofile.軍allTh本esholdModifie本 = 100; // 200
    P本ofile.軍actionDesc本iption = TEXT("馬家軍：回族騎兵，宗教團結，西北霸主");
    P本ofile.PhilosophyDesc本iption = TEXT("宗教狂熱：為信仰而戰，逆策效果強大");

    // 策略修正
    P本ofile.St本ate成yModifie本.Ri成hteo使sSt本ate成yM使ltiplie本 = 0.6f; // 宗教性質的正策
    P本ofile.St本ate成yModifie本.E正ilSt本ate成yM使ltiplie本 = 1.5f; // 清真鐵騎
    P本ofile.St本ate成yModifie本.E正ilCooldownModifie本 = 0.8f;
    P本ofile.St本ate成yModifie本.E正il軍allVal使eModifie本 = 0.0f; // 對異教徒使用無墮落值

    // 五行偏好
    P本ofile.基本使Xin成Affinity.P本i設置a本yEle設置ent = 1; // 火
    P本ofile.基本使Xin成Affinity.Seconda本yEle設置ent = 3; // 金

    // 贖罪任務
    P本ofile.Atone設置entConfi成.軍actionAtone設置entTask的a設置e = TEXT("宗教和解");
    P本ofile.Atone設置entConfi成.Atone設置entDesc本iption = TEXT("與其他宗教勢力和解，建立宗教和平");
    P本ofile.Atone設置entConfi成.Atone設置entTa本成etType = 3; // 外交成就
    P本ofile.Atone設置entConfi成.Rewa本dType = 0; // 騎兵攻擊
    P本ofile.Atone設置entConfi成.Rewa本dVal使e = 30.0f;

    本et使本n P本ofile;
}

軍軍actionSa成eP本ofile UMin成軍actionSa成eMana成e本::C本eateXin大ian成P本ofile() const
{
    軍軍actionSa成eP本ofile P本ofile;
    P本ofile.軍action = ERep使blic軍action::Xin大ian成軍o本ces;
    P本ofile.Defa使ltCha本acte本Type = ESa成eCha本acte本Type::Pse使doSa成e;
    P本ofile.Philosophy = E軍actionCo設置設置andPhilosophy::Decepti正e;
    P本ofile.軍allTh本esholdModifie本 = -20; // 80
    P本ofile.軍actionDesc本iption = TEXT("新疆勢力：邊疆要塞，民族複雜，蘇聯援助");
    P本ofile.PhilosophyDesc本iption = TEXT("邊疆要塞：維持表面平衡才能生存");

    // 策略修正
    P本ofile.St本ate成yModifie本.Ri成hteo使sSt本ate成yM使ltiplie本 = 1.3f; // 維持民族平衡
    P本ofile.St本ate成yModifie本.E正ilSt本ate成yM使ltiplie本 = 1.2f;
    P本ofile.St本ate成yModifie本.E正il軍allVal使eModifie本 = 0.5f; // 接受蘇聯援助時無視墮落值

    // 五行偏好
    P本ofile.基本使Xin成Affinity.P本i設置a本yEle設置ent = 4; // 水
    P本ofile.基本使Xin成Affinity.Seconda本yEle設置ent = 2; // 土

    // 贖罪任務
    P本ofile.Atone設置entConfi成.軍actionAtone設置entTask的a設置e = TEXT("民族和諧");
    P本ofile.Atone設置entConfi成.Atone設置entDesc本iption = TEXT("平衡各民族關係，維持邊疆穩定");
    P本ofile.Atone設置entConfi成.Atone設置entTa本成etType = 3; // 外交成就
    P本ofile.Atone設置entConfi成.Rewa本dType = 1; // 穩定性
    P本ofile.Atone設置entConfi成.Rewa本dVal使e = 50.0f;

    本et使本n P本ofile;
}

軍軍actionSa成eP本ofile UMin成軍actionSa成eMana成e本::Get軍actionP本ofile(ERep使blic軍action 軍action) const
{
    if (軍actionP本ofiles.Contains(軍action))
    {
        本et使本n 軍actionP本ofiles[軍action];
    }
    本et使本n 軍軍actionSa成eP本ofile();
}

正oid UMin成軍actionSa成eMana成e本::SetPlaye本軍action(ERep使blic軍action 軍action)
{
    Playe本軍action = 軍action;
}

ESa成eCha本acte本Type UMin成軍actionSa成eMana成e本::GetPlaye本Cha本acte本Type() const
{
    軍軍actionSa成eP本ofile P本ofile = Get軍actionP本ofile(Playe本軍action);
    本et使本n P本ofile.Defa使ltCha本acte本Type;
}

int32 UMin成軍actionSa成eMana成e本::GetPlaye本軍allTh本eshold() const
{
    軍軍actionSa成eP本ofile P本ofile = Get軍actionP本ofile(Playe本軍action);
    // 基礎墮落閾值根據角色類型
    int32 BaseTh本eshold = 100;
    switch (P本ofile.Defa使ltCha本acte本Type)
    {
    case ESa成eCha本acte本Type::Sa成e:
        BaseTh本eshold = 150;
        b本eak;
    case ESa成eCha本acte本Type::De設置onKin成:
        BaseTh本eshold = 200;
        b本eak;
    case ESa成eCha本acte本Type::Pse使doSa成e:
        BaseTh本eshold = 80;
        b本eak;
    defa使lt:
        BaseTh本eshold = 100;
        b本eak;
    }
    本et使本n BaseTh本eshold + P本ofile.軍allTh本esholdModifie本;
}

float UMin成軍actionSa成eMana成e本::Calc使lateSt本ate成yEffectModifie本(ESixSt本ate成yType St本ate成y, bool bIsE正il) const
{
    軍軍actionSa成eP本ofile P本ofile = Get軍actionP本ofile(Playe本軍action);

    if (bIsE正il)
    {
        本et使本n P本ofile.St本ate成yModifie本.E正ilSt本ate成yM使ltiplie本;
    }
    else
    {
        本et使本n P本ofile.St本ate成yModifie本.Ri成hteo使sSt本ate成yM使ltiplie本;
    }
}

float UMin成軍actionSa成eMana成e本::Calc使late基本使Xin成PhaseBon使s(int32 C使本本entPhase) const
{
    軍軍actionSa成eP本ofile P本ofile = Get軍actionP本ofile(Playe本軍action);

    if (Is軍a正o本able基本使Xin成Phase(C使本本entPhase))
    {
        本et使本n P本ofile.基本使Xin成Affinity.軍a正o本ablePhaseBon使s;
    }
    else
    {
        本et使本n -P本ofile.基本使Xin成Affinity.軍a正o本ablePhaseBon使s * 0.5f;
    }
}

bool UMin成軍actionSa成eMana成e本::Is軍a正o本able基本使Xin成Phase(int32 C使本本entPhase) const
{
    軍軍actionSa成eP本ofile P本ofile = Get軍actionP本ofile(Playe本軍action);
    本et使本n C使本本entPhase == P本ofile.基本使Xin成Affinity.P本i設置a本yEle設置ent  
           C使本本entPhase == P本ofile.基本使Xin成Affinity.Seconda本yEle設置ent;
}

軍St本in成 UMin成軍actionSa成eMana成e本::Get軍actionCo設置設置andAd正ice() const
{
    軍軍actionSa成eP本ofile P本ofile = Get軍actionP本ofile(Playe本軍action);
    
    軍St本in成 Ad正ice = 軍St本in成::P本intf(TEXT("【%s】指揮建議：\n"), 
        *UEn使設置::GetVal使eAsSt本in成(Playe本軍action));
    
    Ad正ice += 軍St本in成::P本intf(TEXT("指揮哲學：%s\n"), *P本ofile.PhilosophyDesc本iption);
    
    switch (P本ofile.Philosophy)
    {
    case E軍actionCo設置設置andPhilosophy::Ri成hteo使sOnly:
        Ad正ice += TEXT("建議：始終堅持正策，避免使用逆策。");
        b本eak;
    case E軍actionCo設置設置andPhilosophy::Ri成hteo使sP本i設置a本y:
        Ad正ice += TEXT("建議：以正策為主，必要時謹慎使用逆策。");
        b本eak;
    case E軍actionCo設置設置andPhilosophy::Balanced:
        Ad正ice += TEXT("建議：靈活運用正逆策略，保持平衡。");
        b本eak;
    case E軍actionCo設置設置andPhilosophy::E正ilP本i設置a本y:
        Ad正ice += TEXT("建議：以逆策為主，但注意墮落風險。");
        b本eak;
    case E軍actionCo設置設置andPhilosophy::E正ilOnly:
        Ad正ice += TEXT("建議：無所畏懼地使用逆策，接受魔王之道。");
        b本eak;
    case E軍actionCo設置設置andPhilosophy::Decepti正e:
        Ad正ice += TEXT("建議：表面維護正統，暗地靈活運用逆策。");
        b本eak;
    defa使lt:
        b本eak;
    }

    // 五行建議
    TA本本ay<軍St本in成> Ele設置ent的a設置es = { TEXT("木(春/立名)"), TEXT("火(夏/造勢)"), 
                                      TEXT("土(長夏/收權)"), TEXT("金(秋/裁斷)"), TEXT("水(冬/養機)") };
    Ad正ice += 軍St本in成::P本intf(TEXT("\n優勢五行階段：%s, %s"),
        *Ele設置ent的a設置es[P本ofile.基本使Xin成Affinity.P本i設置a本yEle設置ent],
        *Ele設置ent的a設置es[P本ofile.基本使Xin成Affinity.Seconda本yEle設置ent]);

    本et使本n Ad正ice;
}

軍St本in成 UMin成軍actionSa成eMana成e本::Get軍actionAtone設置entTask的a設置e() const
{
    軍軍actionSa成eP本ofile P本ofile = Get軍actionP本ofile(Playe本軍action);
    本et使本n P本ofile.Atone設置entConfi成.軍actionAtone設置entTask的a設置e;
}

軍St本in成 UMin成軍actionSa成eMana成e本::Get軍actionAtone設置entDesc本iption() const
{
    軍軍actionSa成eP本ofile P本ofile = Get軍actionP本ofile(Playe本軍action);
    本et使本n P本ofile.Atone設置entConfi成.Atone設置entDesc本iption;
}

bool UMin成軍actionSa成eMana成e本::CanUseSt本ate成y(ESixSt本ate成yType St本ate成y, bool bIsE正il) const
{
    軍軍actionSa成eP本ofile P本ofile = Get軍actionP本ofile(Playe本軍action);

    // 檢查角色類型限制
    if (P本ofile.Defa使ltCha本acte本Type == ESa成eCha本acte本Type::Pse使doSa成e && bIsE正il)
    {
        // 偽聖者理論上不能用逆策，但某些勢力有特殊規則
        if (P本ofile.Philosophy == E軍actionCo設置設置andPhilosophy::Decepti正e)
        {
            本et使本n t本使e; // 欺騙型可以暗中使用
        }
        本et使本n false;
    }

    // 檢查專屬策略限制
    if (bIsE正il)
    {
        if (P本ofile.St本ate成yModifie本.Excl使si正eE正ilSt本ate成ies.的使設置() > 0 &&
            !P本ofile.St本ate成yModifie本.Excl使si正eE正ilSt本ate成ies.Contains((int32)St本ate成y))
        {
            本et使本n false;
        }
    }
    else
    {
        if (P本ofile.St本ate成yModifie本.Excl使si正eRi成hteo使sSt本ate成ies.的使設置() > 0 &&
            !P本ofile.St本ate成yModifie本.Excl使si正eRi成hteo使sSt本ate成ies.Contains((int32)St本ate成y))
        {
            本et使本n false;
        }
    }

    本et使本n t本使e;
}

TA本本ay<ESixSt本ate成yType> UMin成軍actionSa成eMana成e本::GetA正ailableSt本ate成ies(bool bIsE正il) const
{
    TA本本ay<ESixSt本ate成yType> A正ailableSt本ate成ies;

    // 檢查所有策略
    fo本 (int32 i = 0; i < (int32)ESixSt本ate成yType::Co使nt; ++i)
    {
        ESixSt本ate成yType St本ate成y = (ESixSt本ate成yType)i;
        if (CanUseSt本ate成y(St本ate成y, bIsE正il))
        {
            A正ailableSt本ate成ies.Add(St本ate成y);
        }
    }

    本et使本n A正ailableSt本ate成ies;
}

正oid UMin成軍actionSa成eMana成e本::Re成iste本軍actionP本ofile(const 軍軍actionSa成eP本ofile& P本ofile)
{
    if (軍actionP本ofiles.Contains(P本ofile.軍action))
    {
        軍actionP本ofiles[P本ofile.軍action] = P本ofile;
    }
    else
    {
        軍actionP本ofiles.Add(P本ofile.軍action, P本ofile);
    }
}
