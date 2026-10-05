#pragma once

#include "Gameplay/Json/JsonValue.h"
#include "Gameplay/Result.h"

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace Potato::Campaign {

class Ledger;

// Story 6.2 — the 章回 frame around every chapter. Each chapter
// opens on a court 敕命 (frontispiece — imperative, formulaic,
// the institution's voice) and closes with a 評斷 (the historian's
// verdict on the enemy/ourselves) plus an 懸念 cliffhanger
// (且聽下回分解 — the storyteller's hook).
//
// Content lives in potato.narrative/1 documents; the *loader* and
// registry are Story 6.5's — this type parses an already-parsed
// JsonValue document and renders from it.
//
// Frames vary by ledger state — the court cannot hide what the
// books show:
//   - judgment keys on the corruption ratchet (clean/tainted/
//     corrupt): the verdict reads what cruelty was posted.
//   - cliffhanger keys on the mandate balance (blessed/fading/
//     barren): heaven's account decides how hopeful the hook is.
// The frontispiece does NOT vary — the commission is sealed before
// the facts arrive; that distance is the fiction.

// Condition axes, resolved from the ledger.
enum class FrameMood { Clean, Tainted, Corrupt };
enum class FrameOmen { Blessed, Fading, Barren };

// Thresholds — content-facing contract, pinned by tests.
inline constexpr std::int64_t MOOD_TAINTED_AT = 10;
inline constexpr std::int64_t MOOD_CORRUPT_AT = 40;
inline constexpr std::int64_t OMEN_BLESSED_AT = 30;
inline constexpr std::int64_t OMEN_FADING_AT = 5;

FrameMood LedgerMood(const Ledger& l); // from the corruption ratchet
FrameOmen LedgerOmen(const Ledger& l); // from the 天命 balance
std::string_view MoodKey(FrameMood m); // "clean"/"tainted"/"corrupt"
std::string_view OmenKey(FrameOmen o); // "blessed"/"fading"/"barren"

// One chapter's frame. `frontispiece` is the 敕命 body; `judgment`
// and `cliffhanger` are condition-keyed variant maps (lookup falls
// back requested-key → "default" → first entry).
struct ChapterFrame {
    std::string frontispiece;
    // std::less<> gives transparent string_view lookup — same as
    // JsonValue::Object.
    std::map<std::string, std::string, std::less<>> judgment;
    std::map<std::string, std::string, std::less<>> cliffhanger;
};

class ChapterConventions {
public:
    static constexpr std::string_view SCHEMA = "potato.narrative/1";
    static constexpr std::size_t MAX_CHAPTERS = 64; // chapter space
    static constexpr std::size_t MAX_ID_LEN = 64;
    static constexpr std::size_t MAX_TEXT_LEN = 4096;
    static constexpr std::size_t MAX_VARIANTS = 16; // per frame map

    // Parse a potato.narrative/1 document body. Bad schema or bad
    // shape fails without mutating `out` — per-file rejection is
    // the loader's job (6.5), not the schema's.
    static Gameplay::Result<ChapterConventions> FromJson(
        const Gameplay::JsonValue& doc);
    Gameplay::JsonValue ToJson() const;

    const ChapterFrame* Find(std::string_view chapterId) const;
    std::size_t Size() const { return frames_.size(); }

private:
    std::map<std::string, ChapterFrame, std::less<>> frames_;
};

// The commission page — reign-title header + sealed mandate body.
// Missing chapter renders a generic commission (the court always
// has an order to give).
std::string RenderChapterOpen(const ChapterConventions& conv,
                              std::string_view chapterId);

// The closing frame — 評曰 judgment picked by ledger mood +
// cliffhanger picked by ledger omen, closed by the storyteller's
// 且聽下回分解.
std::string RenderChapterClose(const ChapterConventions& conv,
                               std::string_view chapterId,
                               const Ledger& ledger);

} // namespace Potato::Campaign
