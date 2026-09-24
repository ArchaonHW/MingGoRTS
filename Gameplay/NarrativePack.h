#pragma once

// NarrativePack —— C-2 敘事內容包：戰報碎片/判詞/題詞以
// assets/narrative/*.json 資料驅動，文本改動不重新編譯。
//
// 檔案格式 potato.narrative_pack/1：
//   {"schema":"potato.narrative_pack/1","entries":[
//     {"id":"historian.peace.negotiation",
//      "zh-TW":"遣使入帳…","en":"…",
//      "register":"historian","tags":["peace","outcome"]}]}
//
// 語義：
// - 條目以 id 為鍵；同 id 後載覆蓋（pack 可疊層擴充）
// - 壞檔/壞條目跳過並記 Warnings()，不中止其他載入
// - Find(id, locale)：指定 locale 缺 → zh-TW → en → 任一值；
//   全缺回 nullptr 並累計 LookupMisses（回退由呼叫端做）
// - 碎片可含 {key} 佔位符——呼叫端 Render() 代換

#include <map>
#include <string>
#include <vector>

namespace Potato {
namespace Gameplay {

struct NarrativeEntry {
    std::string id;
    std::map<std::string, std::string> locales; // "zh-TW"/"en"…
    std::string reg;                            // historian/fox/human/rival_hand
    std::vector<std::string> tags;              // 事件型別標籤
};

class NarrativePack {
public:
    // 遞迴掃 dir 下 *.json；回傳成功載入的條目數
    int LoadDir(const std::string& dir);
    int LoadFile(const std::string& path);
    void Clear();

    // 查碎片：locale 降級鏈 zh-TW → en → 首值；未命中回 nullptr
    const std::string* Find(const std::string& id,
                            const std::string& locale = "zh-TW") const;
    const NarrativeEntry* Entry(const std::string& id) const;

    // Find + 佔位符代換一併做：pairs = {"{g}", "格洛克"}…
    // 未命中回 fallback（並計入 LookupMisses）
    std::string Frag(const std::string& id, const std::string& fallback,
                     const std::map<std::string, std::string>& pairs = {},
                     const std::string& locale = "zh-TW") const;

    size_t Size() const { return entries.size(); }
    const std::vector<std::string>& Warnings() const { return warnings; }
    int LookupMisses() const { return lookupMisses; }

private:
    std::map<std::string, NarrativeEntry> entries;
    std::vector<std::string> warnings;
    mutable int lookupMisses = 0;
};

} // namespace Gameplay
} // namespace Potato
