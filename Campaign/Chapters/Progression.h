#pragma once

#include "Campaign/Chapters/ChapterLibrary.h"
#include "Campaign/State/CampaignState.h"
#include "Gameplay/Result.h"

#include <string_view>

namespace Potato::Campaign {

// Chapter shell & progression (Story 3.4): binds the
// ChapterLibrary registry to the persisted ChapterProgress.
//
// Chapter space contract: `unlocked`/`resolved` are sized to
// `lib.MaxIndex()+1` and indexed BY ChapterDef.index — sparse
// indexes leave dead flag slots. `current` holds a ChapterDef
// .index (or MaxIndex()+1 = campaign-complete sentinel).
//
// Carry-forward is structural: roster and ledger live in
// CampaignState and progression never resets them — the same
// objects flow into the next chapter.

// Sizes the chapter vectors and points `current` at the library's
// first chapter (unlocked). Fails on an empty library or one
// whose MaxIndex exceeds the save format's bound.
Gameplay::Result<int> InitializeProgress(
    CampaignState& state, const ChapterLibrary& lib);

// Records the current chapter as resolved (win OR converted
// defeat — the caller decides; Epic 4 owns conversion), then
// unlocks and advances to the next chapter in index order. Past
// the last chapter, `current` becomes the complete sentinel.
// Fails when: campaign already complete, `current` isn't a
// registered chapter, or the progress vectors don't match the
// library's chapter space.
Gameplay::Result<int> ResolveAndAdvance(
    CampaignState& state, const ChapterLibrary& lib);

// The same gates ResolveAndAdvance enforces, exported as a pure
// predicate. ConcludeChapter (Story 4.4) preflights with it so a
// ledger seal only lands when the advance cannot fail — the gate
// list is shared, not mirrored, so the two can never drift.
// Returns nullptr when the current chapter can close, else a
// static reason; `error` receives the gate's error class.
const char* CanResolveChapter(const CampaignState& state,
                              const ChapterLibrary& lib,
                              std::string_view& error);

} // namespace Potato::Campaign
