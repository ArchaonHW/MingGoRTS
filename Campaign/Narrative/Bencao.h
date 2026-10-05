#pragma once

#include "Gameplay/Result.h"

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace Potato::Campaign {

// The bencao codex (Story 10.1): the desk's second book — a
// collection-layer materia medica registry beside the ledger.
// Design authority: _bmad-output/bencao-worldview.md (§3 entry
// anatomy, §4 trigger families, §8 boundaries).
//
// Content only: entries carry no executable semantics and never
// reach the sim. The unlock manifest is parsed and validated here
// but evaluated nowhere — resolution lives in Story 10.2. 批註 is
// runtime-bound (Story 10.3): files that try to author it are
// rejected, keeping the annotation voice exclusively the Scribe's.
//
// Wire shape (one file per entry):
//   {"schema":"potato.bencao/1","id":"sanqi","category":"shancao",
//    "name":"三七","aliases":["山漆","金不換"],
//    "origin":"…","nature":"甘、微苦，溫。歸肝、胃經。",
//    "indications":"止血散血，定痛…",
//    "unlock":{"kind":"ledger_tag","tag":"first_casualty"},
//    "source":"本草綱目·卷十二","lang":{"zh-tw":true}}

// The 8 classes — a bounded subset of the compendium's 16 部/60 類,
// each mapped to a gameplay surface that feeds it (worldview §3).
enum class BencaoCategory : std::uint8_t {
    Shancao,   // 山草類   "shancao"   — highland terrain
    Xicao,     // 隰草類   "xicao"     — lowland/village battles
    Ducao,     // 毒草類   "ducao"     — 墮落 events
    Manshui,   // 蔓草/水草 "manshui"  — crossings, convoys
    Gucai,     // 穀菜類   "gucai"     — villages, supply, famine
    Jinshi,    // 金石類   "jinshi"    — shrine layer, forgery theme
    Chongshou, // 蟲獸類   "chongshou" — myth encounters, wonders
    Renbu,     // 人部拾遺 "renbu"     — dark corner, Scribe-flagged
};

// Unlock trigger kinds (worldview §4). Data only — evaluation is
// Story 10.2's unlock engine.
enum class UnlockKind : std::uint8_t {
    Terrain,      // {"kind":"terrain","terrain":"<flag id>"}
    LedgerTag,    // {"kind":"ledger_tag","tag":"<tag>"}
    Governance,   // {"kind":"governance","event":"<id>"}
    MythState,    // {"kind":"myth_state","state":"<id>"}
    Corruption,   // {"kind":"corruption","at_least":<int>}
    ChapterClose, // {"kind":"chapter_close","chapter":<int|-1=any>}
};

struct BencaoEntry {
    std::string id;
    BencaoCategory category = BencaoCategory::Shancao;
    std::string name;                 // 正名 — required
    std::vector<std::string> aliases; // 釋名 — optional
    std::string origin;               // 集解 — optional
    std::string nature;               // 性味歸經 — required
    std::string indications;          // 主治 — required
    UnlockKind unlockKind = UnlockKind::ChapterClose;
    std::string unlockParam;          // terrain/tag/event/state id
    std::int64_t unlockInt = -1;      // corruption.at_least /
                                      // chapter (-1 = every chapter)
    std::string source;               // 卷/條 citation — required
    // zh-tw is inherent — the entry fields ARE the zh-tw text.
    // `lang` block is optional; when present it must assert
    // "zh-tw": true. `en` is tolerated but ignored (OQ-B3).
};

struct RejectedBencao {
    std::filesystem::path path;
    std::string error;
    std::string reason;
};

struct BencaoLoadResult {
    bool ok = false;
    std::string error;
    std::string reason;
    std::vector<RejectedBencao> rejected; // per-file failures
};

// Boot-time registry (ChapterLibrary precedent): built once by
// Load(), immutable thereafter (NFR9). Per-file isolation — a bad
// entry file is rejected and logged, never fails the library; Load
// fails wholesale only if the directory itself is unreadable.
class BencaoLibrary {
public:
    static constexpr std::string_view SCHEMA = "potato.bencao/1";
    static constexpr std::size_t MAX_ENTRIES = 1024;
    static constexpr std::size_t MAX_ID_LEN = 64;
    static constexpr std::size_t MAX_NAME_LEN = 64;
    static constexpr std::size_t MAX_TEXT_LEN = 2048;
    static constexpr std::size_t MAX_ALIASES = 16;
    static constexpr std::size_t MAX_SOURCE_LEN = 128;
    static constexpr std::size_t MAX_UNLOCK_PARAM_LEN = 64;
    // 墮落 accumulates modestly; chapter indexes share the library
    // bound (1024 covers MAX_CHAPTERS with headroom; -1 = any).
    static constexpr std::int64_t MAX_UNLOCK_INT = 1023;

    static BencaoLoadResult Load(const std::filesystem::path& dir,
                                 BencaoLibrary& out);

    const BencaoEntry* Find(std::string_view id) const;
    std::size_t Size() const { return entries_.size(); }
    // Canonical order: category ordinal, then id — the 10.4
    // renderer's browse order.
    const std::vector<BencaoEntry>& Entries() const {
        return entries_;
    }

private:
    std::vector<BencaoEntry> entries_;
};

} // namespace Potato::Campaign
