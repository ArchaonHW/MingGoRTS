// ReplayIntegrityTest - L-5 回放雜湊根完整性無頭測試
// 「回放即審計」：存檔帶 rootHash，篡改檔拒載，舊版降級警告
#include "Gameplay/BattleRecorder.h"

#include <cstdio>
#include <fstream>
#include <sstream>
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

static const char* kPath = "replay_integrity_test.json";

static std::string ReadAll(const char* path) {
    std::ifstream f(path);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

static void WriteAll(const char* path, const std::string& s) {
    std::ofstream f(path);
    f << s;
}

int main() {
    printf("=== Replay Integrity Tests (L-5) ===\n\n");

    BattleRecorder rec;
    rec.AddRecord(0.0f, "deploy 前鋒");
    rec.AddRecord(3.5f, "fog:reveal 敵軍於南岸");
    rec.AddRecord(9.25f, "前鋒 is routing!");
    rec.AddRecord(12.0f, "敵軍 ELIMINATED");

    // ---- [1] 雜湊根 ----
    printf("[1] 雜湊根\n");
    Check(rec.RootHash() != 0, "非零根雜湊");
    BattleRecorder rec2;
    rec2.AddRecord(0.0f, "deploy 前鋒");
    rec2.AddRecord(3.5f, "fog:reveal 敵軍於南岸");
    rec2.AddRecord(9.25f, "前鋒 is routing!");
    rec2.AddRecord(12.0f, "敵軍 ELIMINATED");
    Check(rec.RootHash() == rec2.RootHash(), "同事件流同根");
    rec2.AddRecord(13.0f, "extra");
    Check(rec.RootHash() != rec2.RootHash(), "加筆改根");

    // ---- [2] 存檔含 rootHash ----
    printf("\n[2] 存檔\n");
    Check(rec.SaveToFile(kPath), "存檔成功");
    std::string json = ReadAll(kPath);
    Check(json.find("potato.battle_replay/1") != std::string::npos,
          "schema 標記");
    Check(json.find("rootHash") != std::string::npos, "rootHash 入檔");

    // ---- [3] 合法回放驗證通過 ----
    printf("\n[3] 合法載入\n");
    {
        BattleRecorder back;
        Check(back.LoadFromFile(kPath), "合法檔載入");
        Check(!back.WasLegacyLoad(), "非舊版旗標");
        Check(back.Count() == 4, "四筆回讀");
        Check(back.RootHash() == rec.RootHash(), "根雜湊一致");
    }

    // ---- [4] 篡改事件 → 拒載 ----
    printf("\n[4] 篡改偵測\n");
    {
        std::string bad = json;
        const size_t pos = bad.find("is routing");
        Check(pos != std::string::npos, "找到篡改目標");
        if (pos != std::string::npos) {
            bad.replace(pos, 10, "was routd"); // 等長置換
        }
        WriteAll(kPath, bad);
        BattleRecorder forged;
        Check(!forged.LoadFromFile(kPath), "篡改事件拒載");
        Check(forged.Count() == 0, "拒載不留髒資料");
    }
    {
        // 篡改 rootHash 欄位本身
        std::string bad = json;
        const size_t pos = bad.find("rootHash");
        const size_t vpos = bad.find('"', pos + 10) + 1;
        Check(vpos > 0 && bad[vpos] != '"', "找到 hash 值位");
        bad[vpos] = (bad[vpos] == '0') ? 'f' : '0';
        WriteAll(kPath, bad);
        BattleRecorder forged;
        Check(!forged.LoadFromFile(kPath), "篡改根值拒載");
    }
    {
        // 篡改時間戳
        std::string bad = json;
        const size_t pos = bad.find("9.25");
        Check(pos != std::string::npos, "找到時間目標");
        if (pos != std::string::npos) {
            bad.replace(pos, 4, "8.75");
        }
        WriteAll(kPath, bad);
        BattleRecorder forged;
        Check(!forged.LoadFromFile(kPath), "篡改時間戳拒載");
    }

    // ---- [5] 舊版無 rootHash 降級 ----
    printf("\n[5] 舊版降級\n");
    {
        std::string legacy =
            "{\n  \"schema\": \"potato.battle_replay/1\",\n"
            "  \"events\": [\n"
            "    {\"t\": 0, \"event\": \"old event\"}\n"
            "  ]\n}\n";
        WriteAll(kPath, legacy);
        BattleRecorder back;
        Check(back.LoadFromFile(kPath), "舊版檔降級載入");
        Check(back.WasLegacyLoad(), "舊版旗標立起");
        Check(back.Count() == 1, "舊版事件保留");
    }
    {
        WriteAll(kPath, "{bad json");
        BattleRecorder back;
        Check(!back.LoadFromFile(kPath), "壞 JSON 拒絕");
    }

    std::remove(kPath);
    printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
