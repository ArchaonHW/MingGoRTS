#include "Campaign/Chapters/Progression.h"

namespace Potato::Campaign {

using Gameplay::Result;

namespace {

// Progress vectors must match the library's chapter space.
bool SpaceMatches(const ChapterProgress& cp,
                  const ChapterLibrary& lib) {
    return cp.unlocked.size() == cp.resolved.size() &&
           cp.unlocked.size() ==
               static_cast<std::size_t>(lib.MaxIndex() + 1);
}

} // namespace

Result<int> InitializeProgress(CampaignState& state,
                               const ChapterLibrary& lib) {
    if (lib.Size() == 0 || lib.MaxIndex() < 0) {
        return Gameplay::Fail<int>("content",
                                   "chapter library is empty");
    }
    if (lib.MaxIndex() + 1 >
        static_cast<std::int64_t>(CampaignState::MAX_CHAPTERS)) {
        return Gameplay::Fail<int>(
            "content", "chapter space exceeds save format bound");
    }
    ChapterProgress& cp = state.GetChapter();
    // Boot-only: refuses to silently wipe a campaign in progress.
    // A true reset is a caller decision (construct fresh state).
    if (!cp.unlocked.empty() || !cp.resolved.empty()) {
        return Gameplay::Fail<int>(
            "state", "progress already initialized");
    }
    const std::size_t space =
        static_cast<std::size_t>(lib.MaxIndex() + 1);
    cp.unlocked.assign(space, false);
    cp.resolved.assign(space, false);
    cp.current = lib.Chapters().front().index;
    cp.unlocked[static_cast<std::size_t>(cp.current)] = true;
    return Gameplay::Ok(static_cast<int>(cp.current));
}

const char* CanResolveChapter(const CampaignState& state,
                              const ChapterLibrary& lib,
                              std::string_view& error) {
    const ChapterProgress& cp = state.GetChapter();
    if (!SpaceMatches(cp, lib)) {
        error = "state";
        return "progress vectors don't match chapter space";
    }
    const auto space = static_cast<std::int64_t>(cp.unlocked.size());
    if (space == 0) {
        error = "state";
        return "progress not initialized";
    }
    if (cp.current < 0 || cp.current > space) {
        error = "state";
        return "current out of space";
    }
    if (cp.current == space) {
        error = "campaign";
        return "campaign already complete";
    }
    if (lib.AtIndex(cp.current) == nullptr) {
        error = "state";
        return "current is not a registered chapter";
    }
    // Untrusted-state posture: a crafted save can't resolve a
    // chapter that was never unlocked, nor re-resolve one.
    const auto cur = static_cast<std::size_t>(cp.current);
    if (!cp.unlocked[cur] || cp.resolved[cur]) {
        error = "state";
        return "current chapter is not in play";
    }
    return nullptr;
}

Result<int> ResolveAndAdvance(CampaignState& state,
                              const ChapterLibrary& lib) {
    std::string_view error;
    if (const char* why = CanResolveChapter(state, lib, error)) {
        return Gameplay::Fail<int>(std::string(error), why);
    }
    ChapterProgress& cp = state.GetChapter();

    cp.resolved[static_cast<std::size_t>(cp.current)] = true;

    // Next chapter in library order (chapters_ is index-sorted;
    // sparse indexes skip dead slots naturally).
    for (const ChapterDef& c : lib.Chapters()) {
        if (c.index > cp.current) {
            cp.current = c.index;
            cp.unlocked[static_cast<std::size_t>(c.index)] = true;
            return Gameplay::Ok(static_cast<int>(c.index));
        }
    }
    const auto space =
        static_cast<int>(cp.unlocked.size()); // complete sentinel
    cp.current = space;
    return Gameplay::Ok(space);
}

} // namespace Potato::Campaign
