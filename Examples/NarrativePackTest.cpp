// NarrativePackTest —— C-2 敘事內容包無頭測試：
// 載入/查詢/locale 降級/壞檔跳過/佔位符代換/HistorianReport 消費。

#include "Gameplay/NarrativePack.h"
#include "Gameplay/HistorianReport.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;
using namespace Potato;
using namespace Potato::Gameplay;

static int failures = 0;
static int checks = 0;

static void Check(bool cond, const char* name) {
    ++checks;
    if (!cond) {
        ++failures;
        std::printf("FAIL: %s\n", name);
    }
}

static bool Has(const std::string& s, const std::string& sub) {
    return s.find(sub) != std::string::npos;
}

int main() {
    // ---- 正式內容包載入（工作目錄可能是 bin——兩層都試）----
    std::string assetDir = "assets/narrative";
    if (!fs::is_directory(assetDir)) assetDir = "../../assets/narrative";
    NarrativePack pack;
    const int n = pack.LoadDir(assetDir);
    Check(n >= 12, "載入正式包條目數");
    Check(pack.Size() >= 12, "Size 與載入數一致");
    Check(pack.Warnings().empty(), "正式包無警告");

    const std::string* v = pack.Find("historian.outcome.victory");
    Check(v && Has(*v, "我軍克敵"), "zh-TW 查詢命中");
    const std::string* e = pack.Find("historian.outcome.victory", "en");
    Check(e && Has(*e, "prevailed"), "en locale 命中");

    const NarrativeEntry* ent = pack.Entry("historian.peace.negotiation");
    Check(ent != nullptr, "Entry 取條目");
    Check(ent && ent->reg == "historian", "register 保留");
    Check(ent && !ent->tags.empty() && ent->tags[0] == "peace",
          "tags 保留");

    // ---- locale 降級：自造只含 zh-TW 的條目 ----
    {
        const fs::path dir = "narrative_pack_test_tmp";
        fs::create_directories(dir);
        {
            std::ofstream f(dir / "partial.json");
            f << R"({"schema":"potato.narrative_pack/1","entries":[
                {"id":"x.only_zh","zh-TW":"只有中文","register":"fox",
                 "tags":["t1","t2"]},
                {"id":"x.nolocales"},
                {"zh-TW":"沒有 id 的條目"}
            ]})";
        }
        {
            std::ofstream f(dir / "bad.json");
            f << "not json at all {{{";
        }
        {
            std::ofstream f(dir / "wrongschema.json");
            f << R"({"schema":"potato.other/9","entries":[{"id":"x"}]})";
        }
        NarrativePack p2;
        const int m = p2.LoadDir(dir.string());
        Check(m == 1, "部分壞檔目錄仍載入好條目");
        Check(p2.Warnings().size() >= 3, "壞檔/壞條目各記警告");
        const std::string* z = p2.Find("x.only_zh", "en");
        Check(z && *z == "只有中文", "en 缺→zh-TW 降級");
        Check(p2.Find("nope.missing") == nullptr, "查無 id 回 nullptr");
        Check(p2.LookupMisses() == 1, "未命中計數");

        // 佔位符代換
        NarrativePack p3;
        const std::string r = pack.Frag(
            "historian.peace.subversion", "FALLBACK",
            {{"{g}", "格洛克"}});
        Check(Has(r, "格洛克") && !Has(r, "{g}"), "Frag 代換 {g}");
        const int before = pack.LookupMisses();
        const std::string fb =
            pack.Frag("missing.id", "回退文", {});
        Check(fb == "回退文" && pack.LookupMisses() == before + 1,
              "Frag 查無→回退+計 miss");

        std::error_code ec;
        fs::remove_all(dir, ec);
    }

    // ---- HistorianReport 消費 pack ----
    {
        HistorianInput in;
        in.battleName = "斷橋之役";
        in.peacePathZh = "談判";
        in.peaceGeneral = "格洛克";
        in.pack = &pack;
        HistorianReport rep = ComposeHistorianReport(in);
        Check(Has(rep.text, "遣使入帳"), "pack 碎片入戰報");
        Check(Has(rep.text, "格洛克"), "敵將名代入");
        Check(Has(rep.text, "不戰而定"), "和平收尾句");
        Check(!Has(rep.text, "{g}") && !Has(rep.text, "{battle}"),
              "佔位符全部代換");
    }
    {
        // 無 pack → 內建字串，輸出應逐字相同
        HistorianInput a;
        a.battleName = "斷橋之役";
        a.peacePathZh = "談判";
        a.peaceGeneral = "格洛克";
        HistorianReport noPack = ComposeHistorianReport(a);
        a.pack = &pack;
        HistorianReport withPack = ComposeHistorianReport(a);
        Check(noPack.text == withPack.text,
              "pack 與內建字串逐字一致（正式包同步）");
    }
    {
        // 戰鬥語域 outcome 也走 pack
        HistorianInput in;
        in.battleName = "斷橋之役";
        in.outcome = BattleOutcome::Victory;
        in.elapsedSec = 42.0f;
        in.pack = &pack;
        HistorianReport rep = ComposeHistorianReport(in);
        Check(Has(rep.text, "我軍克敵"), "戰鬥語域 outcome 取 pack");
    }

    std::printf("%d checks, %d failures\n", checks, failures);
    return failures == 0 ? 0 : 1;
}
