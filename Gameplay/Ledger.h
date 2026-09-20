#pragma once

// L-1/L-2 帳本機械化：複式記帳五帳戶 + 雜湊鏈 append-only 帳簿。
// 「記錄是完整帳」——借貸必相等、史官不可篡改，皆為可驗證的機械事實。
// 依據 spec-l-layer-ledger-accounting.md；自實作 FNV-1a 雜湊，不引第三方庫。

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace Potato {
namespace Gameplay {

// 戰役五帳戶：借方記所得、貸方記所付
enum class LedgerAccount {
    Martial = 0, // 武功——戰功、殲敵
    Civil,       // 民心——治理、談判所得
    Fate,        // 天命——神明眷顧
    Army,        // 軍威——兵員、震懾力
    Supply,      // 物資——糧餉、傷亡折算
    Count
};

const char* AccountName(LedgerAccount a);
// 帳戶中文名（戰報/查帳用）
const char* AccountNameZh(LedgerAccount a);
bool AccountFromName(const std::string& name, LedgerAccount& out);

// 分錄來源系統（L-7 產生軌跡）：這筆帳由哪個系統記下。
// Unknown 是合法值——舊檔與手動分錄沒有來源可查
enum class EntrySource {
    Unknown = 0, // 失考
    Battle,      // 戰陣——戰鬥結算
    Refit,       // 營務——整補營
    Negotiation, // 談判——無戰屈兵
    Governance,  // 治理——民心秩序事件
    Myth,        // 神異——神話層事件
    System,      // 紀要——系統/初始
    Count
};

const char* SourceName(EntrySource s);
// 來源中文名（史官體查帳用）
const char* SourceNameZh(EntrySource s);
bool SourceFromName(const std::string& name, EntrySource& out);

// 產生軌跡：分錄由哪個系統、何時、因何事件記下。
// 全預設值視為「無軌跡」——canon 維持舊格式，舊存檔 hash 不變
struct Provenance {
    EntrySource source = EntrySource::Unknown;
    int tick = 0;          // 產生時點（戰鬥 tick 或章節序）
    std::string eventId;   // 觸發事件識別（回放事件 id / 系統自訂）

    bool IsDefault() const {
        return source == EntrySource::Unknown && tick == 0 &&
               eventId.empty();
    }
};

// 通用鏈式雜湊（FNV-1a 64）：prevHash 與任意文本 fold——
// LedgerChain 與回放完整性根（L-5）共用；篡改任一環節即改變末端值
uint64_t LedgerHash(uint64_t prevHash, const std::string& text);

// 分錄：借貸異戶、金額恆正才合法
struct LedgerEntry {
    LedgerAccount debit = LedgerAccount::Martial; // 借方（所得入此帳）
    LedgerAccount credit = LedgerAccount::Supply; // 貸方（所付出此帳）
    int amount = 0;
    std::string memo;
    int chapter = 0;
    Provenance prov; // L-7 產生軌跡；預設值不入 hash（舊檔相容）

    bool WellFormed() const {
        return amount > 0 && debit != credit &&
               debit < LedgerAccount::Count &&
               credit < LedgerAccount::Count;
    }

    // 常用分錄模板（L-1）：勝仗以物資為貸、招募以物資為貸、
    // 無戰談判以未竟武功為貸換民心
    static LedgerEntry BattleVictory(int cost, int chapter,
                                     const std::string& memo = "");
    static LedgerEntry Recruitment(int cost, int chapter,
                                   const std::string& memo = "");
    static LedgerEntry Negotiation(int gain, int chapter,
                                   const std::string& memo = "");
};

// 雜湊鏈帳簿：每筆 {prevHash, entry, hash}，篡改歷史即斷鏈。
class LedgerChain {
public:
    struct Chained {
        LedgerEntry entry;
        uint64_t prevHash = 0;
        uint64_t hash = 0;
    };

    // 追加合法分錄；malformed（借貸同戶/金額非正）拒絕並回 false
    bool Append(const LedgerEntry& e);

    // 全鏈重算驗證：回傳第一個斷鏈/雜湊不符的索引，全合法回 -1
    int Verify() const;

    // 會計健全性：回傳第一筆 malformed 分錄索引，全合法回 -1
    // （偽帳偵測面——載入的帳可雜湊合法但借貸不成立）
    int SoundnessViolation() const;

    // L-4 偽帳注入：雜湊鏈合法但會計不成立的分錄——
    // 對手軍師攻擊記憶系統用的通道；Append() 拒收的走這裡。
    // 回傳注入索引；偵測責任在 SoundnessViolation / Verify
    int InjectForgery(const LedgerEntry& e);

    // 疑帳標記：查帳 UI 標記可疑分錄（L-4 解除對手反制的手段）。
    // 標記是書寫行為不改鏈內容；存檔隨 "suspect":[idx,...] 持久
    bool MarkSuspect(size_t idx);
    bool IsSuspect(size_t idx) const;
    size_t SuspectCount() const { return suspects.size(); }
    const std::vector<size_t>& SuspectIndices() const { return suspects; }

    // 各帳戶淨額（借方總和 − 貸方總和）；全域總和恆為 0
    std::array<int, static_cast<size_t>(LedgerAccount::Count)>
    TrialBalance() const;

    const std::vector<Chained>& Entries() const { return entries; }
    size_t Size() const { return entries.size(); }

    // 雜湊鏈根（最末筆 hash；空鏈回創世值）——供存檔/回放錨定
    uint64_t RootHash() const;

    std::string ToJson() const;
    // 載入不驗證——篡改的檔案照常讀入，由 Verify() 揭露；
    // schema 不符/欄位缺損才回 false
    bool FromJson(const std::string& json);

    // FNV-1a 64：prevHash 與分錄 canonical 字串的鏈式雜湊
    static uint64_t HashEntry(uint64_t prevHash, const LedgerEntry& e);

    static constexpr uint64_t kGenesisHash = 0xcbf29ce484222325ULL;

private:
    std::vector<Chained> entries;
    std::vector<size_t> suspects; // 已標記疑帳索引（升序不重複）
};

} // namespace Gameplay
} // namespace Potato
