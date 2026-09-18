// GeneralDossierTest - N-2 敵將判詞（聽聞態）無頭測試
// 驗收：低估偏差固定 ×0.8、顯性軸判詞、狡詐≥70 放假判詞、
//       Verify 回真值並揭「前判有詐」
#include "Gameplay/GeneralDossier.h"
#include "Gameplay/EnemyGeneral.h"
#include <cstdio>
#include <cstring>
#include <cmath>

using namespace Potato::Gameplay;

static int g_pass = 0, g_fail = 0;
static void Check(bool ok, const char* name) {
    if (ok) { g_pass++; printf("  [PASS] %s\n", name); }
    else    { g_fail++; printf("  [FAIL] %s\n", name); }
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

static bool Has(const std::string& s, const char* sub) {
    return s.find(sub) != std::string::npos;
}

int main() {
    printf("=== General Dossier Tests (N-2) ===\n\n");

    // [1] 聽聞偏差：固定 ×0.8 低估
    // 注意：Hear 回傳 vector 內元素參照——後續 Hear 可能 realloc，
    // 每段斷言前都用 Find 重取，不留長命參照
    printf("[1] 聽聞偏差\n");
    GeneralDossier dossier;
    EnemyGeneral glock = EnemyGeneral::MakeGlock(); // 90/40/10
    dossier.Hear(glock);
    const HearsayEntry* g = dossier.Find("格洛克");
    Check(g != nullptr, "格洛克入冊");
    Check(std::fabs(g->estAggression - 72.0f) < 0.01f, "侵略估 90×0.8=72");
    Check(std::fabs(g->estDiscipline - 32.0f) < 0.01f, "紀律估 40×0.8=32");
    Check(!g->verified && !g->planted, "初判未驗證非偽造");

    // [2] 顯性軸判詞 + 狡詐放假判詞
    printf("\n[2] 顯性軸判詞\n");
    Check(Has(g->verdict, "好戰"), "侵略 90 顯性 → 好戰判詞");
    EnemyGeneral fox = MakeGeneral("老狐", 30, 40, 80);
    dossier.Hear(fox);
    const HearsayEntry* f = dossier.Find("老狐");
    // cunning=80 ≥ 70 → planted=true，verdict 是反向描述而非「多詐」
    Check(f->planted, "狡詐 80 ≥ 70 → planted");
    Check(!Has(f->verdict, "多詐"), "放假判詞不露真實主軸");
    // 最弱軸是 aggression 30 → 反向「怯戰」
    Check(Has(f->verdict, "怯戰"), "最弱軸 aggression → 怯戰假判");

    // [3] 中庸軸判詞
    printf("\n[3] 中庸軸\n");
    EnemyGeneral mid = MakeGeneral("庸將", 50, 55, 45);
    dossier.Hear(mid);
    const HearsayEntry* m = dossier.Find("庸將");
    Check(Has(m->verdict, "中庸"), "三軸皆中平 → 中庸判詞");
    Check(!m->planted, "中庸將不放假判詞");

    // [4] 紀律顯性
    printf("\n[4] 紀律顯性\n");
    EnemyGeneral rock = MakeGeneral("石帥", 40, 85, 50);
    dossier.Hear(rock);
    const HearsayEntry* r = dossier.Find("石帥");
    Check(Has(r->verdict, "守紀"), "紀律 85 顯性 → 守紀判詞");

    // [5] Verify 回真值 + 揭詐
    printf("\n[5] 觀測驗證\n");
    Check(dossier.Verify(fox), "Verify 命中檔案");
    f = dossier.Find("老狐");
    Check(f->verified, "verified 旗標");
    Check(std::fabs(f->estAggression - 30.0f) < 0.01f, "真值 30 回填");
    Check(Has(f->verdict, "前判有詐"), "假判詞驗證揭詐");
    Check(dossier.Verify(glock), "Verify 格洛克");
    g = dossier.Find("格洛克");
    Check(!Has(g->verdict, "前判有詐"), "真判詞不標詐");

    // [6] 同名覆蓋與查找
    printf("\n[6] 檔案管理\n");
    Check(dossier.Find("老狐") != nullptr, "Find 命中");
    Check(dossier.Find("不存在") == nullptr, "Find 查無");
    Check(dossier.Entries().size() == 4, "四將入冊");
    Check(!dossier.Verify(MakeGeneral("陌生人", 50, 50, 50)),
          "未入冊 Verify 回 false");

    printf("\n=== %d passed, %d failed ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
