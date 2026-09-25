// CardEditor CLI —— G-3 敵將卡編輯器（CI 可用的 headless 半邊）
//
// 用法：
//   CardEditor <card.json> [選項]
//     --assets-root <dir>       立繪存在性檢查根目錄（預設 assets）
//     --doctrine-dir <dir>      doctrine 卡池目錄（交叉驗證 sigId）
//     --set <field>=<value>     文字欄寫入（白名單內）
//     --set-personality <axis>=<v>   人格軸（夾 0..100）
//     --set-signature "name|trigger|action|threshold"
//     --set-signature-id <id>   signatureDoctrineId
//     --add-rule "name|trigger|action|threshold|priority|cooldown"
//     --remove-rule <idx>
//     --out <path>              另存新檔（預設覆寫原檔）
//     --dry-run                 套用+驗證但不寫檔
//
// 無編輯操作時 = 單卡 validate-only。任一 op 失敗或驗證 error
// → exit 1 不寫檔；成功 exit 0。

#include "Gameplay/CardEditor.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

using Potato::Gameplay::CardEditor;

namespace {

// 把 "a|b|c|d" 拆成段
std::vector<std::string> Split(const std::string& s, char sep) {
    std::vector<std::string> out;
    size_t start = 0;
    for (;;) {
        const size_t p = s.find(sep, start);
        if (p == std::string::npos) {
            out.push_back(s.substr(start));
            break;
        }
        out.push_back(s.substr(start, p - start));
        start = p + 1;
    }
    return out;
}

void PrintReport(const Potato::Gameplay::CardReport& r) {
    for (const auto& i : r.issues)
        std::printf("  %s %s: %s\n", i.error ? "ERR " : "warn",
                    i.field.c_str(), i.message.c_str());
}

} // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        std::printf("用法：CardEditor <card.json> [--set f=v] "
                    "[--set-personality a=v] [--set-signature "
                    "n|t|a|th] [--set-signature-id id] "
                    "[--add-rule n|t|a|th|p|cd] [--remove-rule i] "
                    "[--doctrine-dir dir] [--assets-root dir] "
                    "[--out path] [--dry-run]\n");
        return 2;
    }

    CardEditor ed;
    ed.SetAssetsRoot("assets");
    std::string out;
    bool dryRun = false;
    int ops = 0;

    if (!ed.Load(argv[1])) {
        std::printf("FAIL 載入：%s\n", ed.LastError().c_str());
        return 1;
    }

    for (int i = 2; i < argc; ++i) {
        const std::string arg = argv[i];
        auto need = [&](const char* flag) -> std::string {
            if (i + 1 >= argc) {
                std::printf("FAIL %s 缺參數\n", flag);
                std::exit(2);
            }
            return argv[++i];
        };
        auto ok = [&](bool r) {
            if (!r) {
                std::printf("FAIL %s：%s\n", arg.c_str(),
                            ed.LastError().c_str());
                std::exit(1);
            }
            ++ops;
        };

        if (arg == "--assets-root") ed.SetAssetsRoot(need("--assets-root"));
        else if (arg == "--doctrine-dir") ed.BindDoctrineDir(need("--doctrine-dir"));
        else if (arg == "--out") out = need("--out");
        else if (arg == "--dry-run") dryRun = true;
        else if (arg == "--set") {
            const std::string kv = need("--set");
            const size_t eq = kv.find('=');
            if (eq == std::string::npos) {
                std::printf("FAIL --set 需 field=value\n");
                return 2;
            }
            ok(ed.SetText(kv.substr(0, eq), kv.substr(eq + 1)));
        } else if (arg == "--set-personality") {
            const std::string kv = need("--set-personality");
            const size_t eq = kv.find('=');
            if (eq == std::string::npos) {
                std::printf("FAIL --set-personality 需 axis=value\n");
                return 2;
            }
            ok(ed.SetPersonality(kv.substr(0, eq),
                                 std::stof(kv.substr(eq + 1))));
        } else if (arg == "--set-signature") {
            const auto p = Split(need("--set-signature"), '|');
            if (p.size() != 4) {
                std::printf("FAIL --set-signature 需 "
                            "name|trigger|action|threshold\n");
                return 2;
            }
            ok(ed.SetSignature(p[0], p[1], p[2], std::stof(p[3])));
        } else if (arg == "--set-signature-id") {
            ok(ed.SetSignatureDoctrineId(need("--set-signature-id")));
        } else if (arg == "--add-rule") {
            const auto p = Split(need("--add-rule"), '|');
            if (p.size() != 6) {
                std::printf("FAIL --add-rule 需 "
                            "name|trigger|action|th|pri|cd\n");
                return 2;
            }
            ok(ed.AddCardRule(p[0], p[1], p[2], std::stof(p[3]),
                              std::stoi(p[4]), std::stof(p[5])));
        } else if (arg == "--remove-rule") {
            ok(ed.RemoveCardRule(std::stoi(need("--remove-rule"))));
        } else {
            std::printf("FAIL 未知旗標：%s\n", arg.c_str());
            return 2;
        }
    }

    const auto report = ed.Validate();
    std::printf("%s %s\n", report.Ok() ? "OK  " : "FAIL",
                report.path.c_str());
    PrintReport(report);

    if (!report.Ok()) {
        std::printf("---- 驗證 error，拒存\n");
        return 1;
    }
    if (dryRun || ops == 0) {
        std::printf("---- %s（未寫檔）\n",
                    dryRun ? "dry-run" : "validate-only");
        return 0;
    }

    if (!ed.Save(out.empty() ? argv[1] : out)) {
        std::printf("FAIL 存回：%s\n", ed.LastError().c_str());
        return 1;
    }
    std::printf("---- 已存回 %s（%d 個操作）\n",
                (out.empty() ? argv[1] : out).c_str(), ops);
    return 0;
}
