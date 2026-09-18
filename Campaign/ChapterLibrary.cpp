#include "Campaign/ChapterLibrary.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <system_error>

namespace Potato {
namespace Campaign {

bool ChapterDef::LoadFromFile(const std::string& path) {
    // 容錯：逐層往上找（CTest 工作目錄可能深至 build/bin/Release）
    for (const char* prefix : {"", "../", "../../", "../../../"}) {
        std::ifstream f(std::string(prefix) + path);
        if (f) {
            std::ostringstream ss;
            ss << f.rdbuf();
            return LoadFromString(ss.str());
        }
    }
    return false;
}

bool ChapterDef::LoadFromString(const std::string& json) {
    // 先重置：失敗路徑留下的是乾淨預設物件，不殘留上次載入的值
    *this = ChapterDef();

    JsonValue root;
    if (!JsonValue::ParseOk(json, root) || !root.IsObject()) {
        return false;
    }
    // schema 驗證：不符/缺欄一律拒絕，不產生半成品
    if (root["schema"].AsString() != kSchema) {
        return false;
    }

    id = root["id"].AsString();
    name = root["name"].AsString(id); // 缺名用 id 頂替
    if (id.empty()) {
        warnings.push_back("章節定義缺 id");
    }
    if (name.empty()) {
        warnings.push_back("章節定義缺 name");
    }

    // arc：越界夾回 [0,3]+警告；chapter：夾到 ≥1+警告
    {
        const double raw = root["arc"].AsNumber(0.0);
        if (!std::isfinite(raw)) {
            warnings.push_back("arc 非有限值，用預設 0");
        } else if (raw < 0.0 || raw > 3.0) {
            warnings.push_back("arc 越界 [0,3]，夾回");
        }
        arc = std::isfinite(raw)
                  ? static_cast<int>(std::clamp(raw, 0.0, 3.0))
                  : 0;
    }
    {
        const double raw = root["chapter"].AsNumber(1.0);
        if (!std::isfinite(raw)) {
            warnings.push_back("chapter 非有限值，用預設 1");
        } else if (raw < 1.0) {
            warnings.push_back("chapter<1，夾到 1");
        }
        chapter = std::isfinite(raw)
                      ? static_cast<int>(std::max(1.0, raw))
                      : 1;
    }

    map = root["map"].AsString();
    enemyDeck = root["enemy_deck"].AsString();
    next = root["next"].AsString();
    return true;
}

size_t ChapterLibrary::LoadDir(const std::string& dir) {
    if (dir.empty()) {
        return 0;
    }
    namespace fs = std::filesystem;

    // 目錄本身也吃逐層 ../ 容錯（與 LoadFromFile 同一工作目錄假設）
    fs::path root;
    for (const char* prefix : {"", "../", "../../", "../../../"}) {
        std::error_code ec;
        fs::path cand(std::string(prefix) + dir);
        if (fs::is_directory(cand, ec)) {
            root = cand;
            break;
        }
    }
    if (root.empty()) {
        return 0;
    }

    std::vector<fs::path> files;
    {
        std::error_code ec;
        for (const auto& e : fs::directory_iterator(root, ec)) {
            if (!e.is_regular_file(ec)) {
                continue;
            }
            std::string ext = e.path().extension().string();
            for (auto& c : ext) {
                c = static_cast<char>(
                    std::tolower(static_cast<unsigned char>(c)));
            }
            if (ext == ".json") {
                files.push_back(e.path());
            }
        }
    }
    std::sort(files.begin(), files.end()); // 載入順序穩定

    size_t loaded = 0;
    for (const auto& p : files) {
        std::ifstream f(p);
        if (!f) {
            continue;
        }
        std::ostringstream ss;
        ss << f.rdbuf();
        ChapterDef def;
        // 壞檔/schema 不符：跳過不中止（單檔錯誤不拖垮整庫）
        if (!def.LoadFromString(ss.str())) {
            continue;
        }
        // 同 id 撞車：後載覆蓋先載（keep latest）
        auto it = std::find_if(defs.begin(), defs.end(),
                               [&](const ChapterDef& d) {
                                   return d.id == def.id;
                               });
        if (it != defs.end()) {
            *it = def;
        } else {
            defs.push_back(def);
        }
        ++loaded;
    }
    return loaded;
}

const ChapterDef* ChapterLibrary::Find(const std::string& id) const {
    for (const auto& d : defs) {
        if (d.id == id) {
            return &d;
        }
    }
    return nullptr;
}

std::vector<const ChapterDef*> ChapterLibrary::Sorted() const {
    std::vector<const ChapterDef*> out;
    out.reserve(defs.size());
    for (const auto& d : defs) {
        out.push_back(&d);
    }
    std::sort(out.begin(), out.end(),
              [](const ChapterDef* a, const ChapterDef* b) {
                  if (a->arc != b->arc) {
                      return a->arc < b->arc;
                  }
                  if (a->chapter != b->chapter) {
                      return a->chapter < b->chapter;
                  }
                  return a->id < b->id;
              });
    return out;
}

} // namespace Campaign
} // namespace Potato
