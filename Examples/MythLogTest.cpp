// MythLogTest - N-5 神話事件具名記錄無頭測試
#include "Gameplay/MythLog.h"

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

int main() {
    printf("=== Myth Log Tests (N-5) ===\n\n");

    // ---- [1] 三欄記錄 ----
    printf("[1] 具名記錄\n");
    MythLog log;
    log.Record("斷橋祠", "橋神", "第1章", "神像流淚示警");
    log.Record("白狐塚", "狐仙", "第3章", "夜營獻計");
    Check(log.Count() == 2, "兩筆事件");
    const MythEvent& e0 = log.Events()[0];
    Check(e0.shrine == "斷橋祠" && e0.spirit == "橋神" &&
              e0.when == "第1章",
          "祠/靈/時三欄");

    // ---- [2] 滲透觸發介面 ----
    printf("\n[2] 滲透觸發\n");
    int seepCount = 0;
    std::string lastSpirit;
    log.SetSeepageCallback([&](const MythEvent& e) {
        ++seepCount;
        lastSpirit = e.spirit;
    });
    log.Record("關帝廟", "關聖", "第5章", "戰神附身");
    Check(seepCount == 1, "回呼觸發一次");
    Check(lastSpirit == "關聖", "回呼帶事件內容");
    Check(log.Count() == 3, "事件仍入日誌");

    // ---- [3] 神的證詞 ----
    printf("\n[3] 結局證詞\n");
    auto lines = log.TestimonyLines();
    Check(lines.size() == 3, "三行證詞");
    Check(lines[0].find("斷橋祠") != std::string::npos &&
              lines[0].find("橋神") != std::string::npos,
          "證詞含祠靈");
    Check(lines[2].find("戰神附身") != std::string::npos,
          "證詞含記事");

    // ---- [4] 序列化持久化 ----
    printf("\n[4] 持久化\n");
    std::string json = log.ToJson();
    Check(json.find("potato.myth_log/1") != std::string::npos,
          "schema 標記");
    MythLog log2;
    Check(log2.FromJson(json), "回讀");
    Check(log2.Count() == 3, "事件數保存");
    Check(log2.Events()[1].spirit == "狐仙", "內容一致");
    Check(!log2.FromJson("{bad"), "壞 JSON 拒絕");
    Check(!log2.FromJson("{\"schema\":\"potato.other/1\"}"),
          "錯 schema 拒絕");

    printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
