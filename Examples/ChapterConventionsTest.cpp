// ChapterConventionsTest - N-6 章節慣例文字層無頭測試
#include "Gameplay/ChapterConventions.h"
#include "Gameplay/EnemyGeneral.h"

#include <cstdio>
#include <string>

using namespace Potato::Gameplay;

static int g_pass = 0;
static int g_fail = 0;

static void Check(bool ok, const char* name) {
    if (ok) {
        g_pass++;
        printf("  [PASS] %s\n", name);
    } else {
        g_fail++;
        printf("  [FAIL] %s\n", name);
    }
}

static bool Has(const std::string& s, const char* sub) {
    return s.find(sub) != std::string::npos;
}

static EnemyGeneral MakeGeneral(const char* name, float a, float d,
                                float c) {
    char buf[256];
    std::snprintf(buf, sizeof(buf),
                  R"({"name":"%s","personality":{"aggression":%.0f,
                  "discipline":%.0f,"cunning":%.0f}})",
                  name, a, d, c);
    EnemyGeneral g;
    g.LoadFromString(buf);
    return g;
}

int main() {
    printf("=== Chapter Conventions Tests (N-6) ===\n\n");

    // ---- [1] 章開題詞 ----
    printf("[1] 題詞\n");
    std::string v1 =
        ChapterConventions::OpeningVerse(1, "斷橋");
    Check(Has(v1, "斷橋"), "題詞嵌主題");
    Check(Has(v1, "？"), "題詞是問句");
    std::string v6 =
        ChapterConventions::OpeningVerse(6, "斷橋");
    Check(v6 == v1, "韻庫輪替（第6章同第1章）");
    Check(!ChapterConventions::OpeningVerse(0, "").empty(),
          "非法章序與空主題仍產句");

    // ---- [2] 敵將入場判詞 ----
    printf("\n[2] 入場判詞\n");
    GeneralDossier dossier;
    dossier.Hear(MakeGeneral("曹洪", 90, 40, 10));
    std::string j = ChapterConventions::EntranceJudgment(dossier, "曹洪");
    Check(Has(j, "據報"), "未驗證判詞帶聽聞體");
    Check(Has(j, "曹洪"), "判詞含將名");
    Check(Has(ChapterConventions::EntranceJudgment(dossier, "無此人"),
              "虛實未聞"),
          "無檔案將領給虛實未聞");
    dossier.Verify(MakeGeneral("曹洪", 90, 40, 10));
    std::string jv =
        ChapterConventions::EntranceJudgment(dossier, "曹洪");
    Check(!Has(jv, "據報"), "驗證後判詞去聽聞體");

    // ---- [3] 章尾欲知後事 ----
    printf("\n[3] 章尾鉤\n");
    ChapterConventions::ClosingContext win;
    win.chapter = 2;
    win.playerVictory = true;
    win.nextForeshadow = "敵帳中那封回信";
    std::string h = ChapterConventions::ClosingHook(win);
    Check(Has(h, "奏凱"), "勝場收束語");
    Check(Has(h, "欲知"), "欲知後事");
    Check(Has(h, "那封回信"), "伏筆詞嵌入");
    Check(Has(h, "且聽下回分解"), "章回套語");
    ChapterConventions::ClosingContext loss;
    loss.playerVictory = false;
    loss.rivalCountered = true;
    loss.mythEventCount = 2;
    std::string h2 = ChapterConventions::ClosingHook(loss);
    Check(Has(h2, "折戟"), "敗場收束語");
    Check(Has(h2, "異象"), "神話事件入鉤");
    Check(Has(h2, "窺破"), "N-3 反制入鉤");
    ChapterConventions::ClosingContext dr;
    dr.draw = true;
    Check(Has(ChapterConventions::ClosingHook(dr), "勝負未分"),
          "平手收束語");

    // ---- [4] 結局四聲部判定 ----
    printf("\n[4] 四聲部判定\n");
    using EV = ChapterConventions::EndingVoice;
    ChapterConventions::EndingInput in;
    in.corruption = 100.f;
    Check(ChapterConventions::ResolveEnding(in) == EV::Fallen,
          "墮落滿→對手之筆");
    in.corruption = 0;
    in.annihilated = 8;
    in.subdued = 2;
    in.popularSupport = 80;
    in.civilOrder = 70;
    Check(ChapterConventions::ResolveEnding(in) == EV::Unwritten,
          "武功民心俱高→無字");
    in.popularSupport = 30;
    in.civilOrder = 20;
    Check(ChapterConventions::ResolveEnding(in) == EV::Conquest,
          "武功顯治績低→霸業");
    in.annihilated = 1;
    in.subdued = 7;
    in.popularSupport = 75;
    in.civilOrder = 65;
    Check(ChapterConventions::ResolveEnding(in) == EV::Mandate,
          "治績顯武功斂→仁政");
    // 皆未達檻：分數取高者
    in.annihilated = 5;
    in.subdued = 1;
    in.popularSupport = 10;
    in.civilOrder = 10;
    Check(ChapterConventions::ResolveEnding(in) == EV::Conquest,
          "皆低取武功→霸業");
    Check(std::string(ChapterConventions::VoiceName(EV::Unwritten)) ==
              "無字",
          "聲部名");

    // ---- [5] 結局文由累積記錄生成 ----
    printf("\n[5] 結局文生成\n");
    CampaignLedger ledger;
    ledger.RecordDisposition("g1", "格洛克", 1,
                             GeneralDisposition::Subdued);
    ledger.EarnTitle("斷橋軍師", 3);
    MythLog myths;
    myths.Record("斷橋祠", "橋神", "第1章", "神像流淚示警");

    std::string ec = ChapterConventions::ComposeEnding(
        EV::Conquest, ledger, myths);
    Check(Has(ec, "史曰"), "霸業史官語體");
    Check(Has(ec, "格洛克"), "結局引處置記錄");
    Check(Has(ec, "斷橋軍師"), "結局引稱號");
    Check(Has(ec, "橋神"), "結局引神話證詞");

    std::string eu = ChapterConventions::ComposeEnding(
        EV::Unwritten, ledger, myths);
    Check(!Has(eu, "史曰"), "無字無史官主文");
    Check(Has(eu, "格洛克"), "無字仍存累積記錄");

    std::string ef = ChapterConventions::ComposeEnding(
        EV::Fallen, ledger, myths);
    Check(Has(ef, "敵手代筆"), "對手之筆語體");

    printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
