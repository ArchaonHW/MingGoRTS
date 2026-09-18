# Feasibility Review — MingGoRTS UX Spec vs. PotatoEngine Reality

**Date:** 2026-09-18 · **Reviewer lens:** engineering feasibility under PotatoEngine (C++20, OpenGL, ImGui 1.93-WIP immediate-mode, procedural meshes, no UI sprite pipeline, no working audio backend)

**Sources checked:**
- Spec: `DESIGN.md`, `EXPERIENCE.md` (this folder)
- Engine: `Examples/DuanqiaoPlayable.cpp` (current ImGui HUD), `Rendering/OpenGLRenderer.h` (Mesh/Shader/Texture all defined here), `Rendering/Shader.h`, `Rendering/ImageCodec.h`, `Rendering/SceneRenderer.h`, `Gameplay/BattleSceneSync.h`, `Gameplay/BattlePicker.h`, `Audio/AudioSystem.h/.cpp`, `CMakeLists.txt`, `MingGoRTS_IDE/GUI/I18N.h`, `MingGoRTS_IDE/GUI/IDEGUI.cpp` (ImGui::Image precedent), `external/imgui/imgui.h` (v1.93.0 WIP, `IMGUI_HAS_TEXTURES` dynamic atlas)

**Verdict summary:** the spec is unusually honest about its own dependencies — almost every "fancy" visual has a spec-sanctioned rect fallback (decision log R4/R5). Under those fallbacks, ~80% of the spec is TRIVIAL or MODERATE on today's engine. The only genuinely heavy subsystem is **audio** (interface exists, backend is a stub), and the only structural gaps are **minimap**, **world→screen projection helper**, and **font asset licensing**.

| Class | Count | Meaning |
|---|---|---|
| TRIVIAL | ~22 | maps to ImGui primitives today |
| MODERATE | ~24 | custom draw-list calls / small shader / sprite via existing PNG→Texture→Image path |
| HEAVY | 4 | needs new subsystem (audio, true blur/RTT, rotated text, adaptive score) |
| BLOCKED | 0 | nothing impossible; HEAVY items all have spec fallbacks or are deferrable |

---

## Classification table

### Already built (free)

| Spec claim | Class | Evidence |
|---|---|---|
| Chronicler typing effect (post-battle report, 逐字敲出) | **DONE** | `DuanqiaoPlayable.cpp` L1013–1054: UTF-8-boundary-safe typewriter at 30 chars/s |
| Post-battle two-column roster (survivors/fallen + relics) | **DONE** | `DuanqiaoPlayable.cpp` L1068–1095 |
| Outcome banner (勝利/敗北/平手) | **DONE** | L992–1010 (`SetWindowFontScale` used — font scaling already proven) |
| Replay timeline scrub | **DONE** (basic) | L1099–1114 SliderFloat + event list; tick icons still needed |
| Planning deck UI (squad list / card slots / editor / validate / 開戰) | **DONE** | L744–907 — already three-column with pickers, no code entry |
| Squad roster w/ strength+morale bars, CP buttons | **DONE** (plain) | L909–980 ProgressBar + BeginDisabled |
| Event feed | **DONE** (plain) | L983–990 |
| Cloud drift (turbulence wander) | **DONE** | `BattleSceneSync::SetFogDrift` + `Quasi::TurbulenceField` wired L477–481 |
| On-map unit markers, selection ring, health bars | **DONE** | `BattleSceneSync.h` overlay system (ring/barBg/barFill meshes) |
| Alt+drag objective/rally pins | **DONE** | L586–606 |
| CJK font rendering | **DONE** | `GetGlyphRangesChineseFull()` L265 (msjh.ttc); also in QuantumFogDemo, SageCommandDemo |

### Component-by-component

| # | Spec component / claim | Class | Implementation path | Notes |
|---|---|---|---|---|
| 1 | Button (48px, theme fill, focus ring, disabled=hollow/strike) | TRIVIAL | `ImGui::Button` + `PushStyleColor/Var`; focus via `ImGuiCol_Border`+nav | Brass gradient = `AddRectFilledMultiColor` (2-stop) — MODERATE dressing |
| 2 | Panel (fill/border/elevation) | TRIVIAL | Window bg + border style vars | Paper drop-shadow = offset alpha rect (MODERATE-cheap); torn edges & wood grain = sprite (MODERATE) |
| 3 | Bar (兵力/士氣/stamina, 3–9px) | TRIVIAL | `ProgressBar` or draw-list rect | Broken-stroke collapse fill = 1-bit pattern → small custom `AddRectFilled` loop or 4px tile texture — MODERATE |
| 4 | Seal/stamp (circle wax, double-ring, square chop) | MODERATE | `AddCircle`×2 + centered text; rotated text NOT possible — keep text axis-aligned | Rough stamped edge = optional PNG sprite |
| 5 | Card slot (110×56, accent strip, drag, dashed 空槽) | TRIVIAL | Rect + 2 text lines; `BeginDragDropSource/Target` for reorder | Dashed outline = segmented polyline (MODERATE, ~15 lines) |
| 6 | Cloud marker (blurred radial blobs + bound ring + P readout) | MODERATE | Spec fallback: 3 stacked alpha circles — or better, **procedural radial-gradient texture** (generate RGBA in code → `Texture::LoadFromMemory` → billboard quad in-world, or `ImGui::Image` overlay) | True gaussian blur = HEAVY (see #19); conic reveal ring = `PathArcTo` segments (MODERATE); drift already done |
| 7 | Roster row (name/tag/bars, alert edge, collapse pulse) | TRIVIAL | Rects + `ProgressBar`; 1.1s pulse = `sin(time)` alpha on border rect | |
| 8 | Timeline (rail + event ticks + scrub + click-jump) | MODERATE | Custom draw-list rail + tick rects; `InvisibleButton` per tick for jump | Base slider already exists |
| 9 | Unit marker (pill/dab/piece + name) | TRIVIAL–MODERATE | Rounded rect draw-list; "ink dab" organic shape = sprite | Name label on-map needs world→screen (#26) |
| 10 | **Minimap** (river strip, f/e/c dots, camera frame, pins) | MODERATE | **2D draw-list minimap** from `BattleMap` grid data — river = rect per blocked row-run, dots = `AddCircleFilled`, camera frame = projected frustum corners. Do NOT render-to-texture (no RTT API in `OpenGLRenderer`) | New surface, no existing system; ~150–250 lines. Camera-frame projection needs view-matrix math — `Matrix4` exists |
| 11 | Feed entry (timestamp + text + stamp + click-jump) | TRIVIAL | `Selectable` + colored 達/急 glyph | Camera jump target = stored event position |
| 12 | Dossier panel (3 meters + habit line) | TRIVIAL | Three `ProgressBar`s gated by intel unlock state | |
| 13 | **Runtime theme swap (token table → ImGuiStyle)** | TRIVIAL–MODERATE | `ImGuiStyle` is a plain struct — build `ThemeTokens` → `ApplyTo(ImGuiStyle&)`; per-theme fonts via `PushFont`/`PopFont` | Feasible exactly as spec'd (R4). Fonts per theme = the only real cost (see risk R2) |
| 14 | 4-theme asset cost | MODERATE | Token colors = free. Per-theme sprite set (paper grain, torn edges, brass plate, stamp roughness, ink strokes) is art-production cost, not engine work | Spec's rect-fallback means themes can ship token-only and upgrade |
| 15 | **Myth seepage full-theme transition** | MODERATE | Crossfade = fullscreen scrim rect with lerped alpha while swapping style mid-fade (TRIVIAL). Theatrical variants (ink bleed/lamp flicker/glitch) = animated scrim via radial texture or staggered per-panel alpha — each MODERATE | Anchors are `SetNextWindowPos` constants → structurally guaranteed not to move. Motion-reduction crossfade is the cheap path and it's spec'd |
| 16 | Ink-brush strokes w/ flying-white ends | MODERATE | Gradient-masked rect: `AddRectFilledMultiColor` with alpha taper, or thin sprite | Real brush texture = sprite (art cost only) |
| 17 | Torn-paper edges | MODERATE | Jagged polyline along edge (procedural, ~20 lines) or PNG edge sprite | Spec already flags sprite dependency |
| 18 | Circular stamps (wax/estop/double-ring seal) | MODERATE | `AddCircle`/`AddCircleFilled` + text | Same as #4 |
| 19 | **Gaussian blur (cloud fill, "blurred radial")** | HEAVY | Requires FBO ping-pong; `OpenGLRenderer` exposes no render-target API — would need raw GL in example code or a new `Framebuffer` class | **Spec already provides fallback** (stacked alpha circles). Recommend: procedural radial-gradient texture — visually 90% there, zero new subsystem |
| 20 | **Vertical-rl titles (ink-chronicle)** | MODERATE | No writing-mode in ImGui, but **CJK vertical = upright glyphs stacked** — draw each glyph on its own line via `AddText` per char (~25 lines helper). No rotation needed. | Caveat: only safe for pure-CJK strings (「戰報」「探·六息」). Latin/digits in vertical context must be forbidden or rendered horizontal-inline |
| 21 | Chronicler typing | DONE | See above | |
| 22 | ±2° rotated pinned cards | MODERATE | Draw-list has no transform — rotate the 4 rect corners manually for the card body, keep text axis-aligned inside | Rotated TEXT = HEAVY (needs glyph-quad transform or text-to-texture). Accept spec's "plain rects acceptable" or rotate container only |
| 23 | Desk vignette / lamp radial falloff | MODERATE | Fullscreen gradient: 4 edge rects with `AddRectFilledMultiColor`, or one radial texture | |
| 24 | Paper drop-shadows | MODERATE | Offset semi-transparent rect behind each card — cheap, no shader | |
| 25 | Dashed borders/underlines (war-map marginalia) | MODERATE | Segmented `AddPolyline` helper (~15 lines) — ImGui has no dash support | |
| 26 | On-map overlays: entanglement links, rout-chain arcs (G-1), plan arrows (G-5), reveal rings, on-map labels | MODERATE | Needs a ~20-line `WorldToScreen(cam, worldPos)` helper (view·proj matrix exists on `Camera`); then all are screen-space draw-list polylines/arcs | `BattlePicker` only has `ScreenToWorldRay` — inverse projection doesn't exist yet. Alternatively draw as flat 3D meshes (ring mesh already works this way) |
| 27 | Terrain overlay (G-4) | MODERATE | Translucent screen grid or per-cell tinted quads (same technique as water tiles) | |
| 28 | Seizure countdown 「接管中 08s」 stamp on roster + on-map | TRIVIAL–MODERATE | Roster: text+stamp overlay TRIVIAL; on-map ring = reveal-ring code (#26) | |
| 29 | HUD density tiers (極簡/標準/完整) | TRIVIAL | Conditional rendering — surfaces already independent windows | |
| 30 | Settings screen (language/theme/density/scale/motion/seepage/bindings) | TRIVIAL–MODERATE | Standard widgets; binding remap needs InputManager hookup | 音效 slider is dead until audio exists |
| 31 | **UI scale 75–150%** | MODERATE | `io.FontGlobalScale` scales **text only** — not enough. Correct path in ImGui 1.93: `style.ScaleAllSizes(f)` + per-size font rendering (1.93 dynamic atlas rasterizes at requested size → crisp). Store base style, rebuild scaled copy on change | Independent 字級 = `PushFont`/per-window scale. Must re-validate all fixed `SetNextWindowSize` values against scaled content |
| 32 | Bilingual string table (TC/EN, +30% EN budget) | TRIVIAL–MODERATE | Pattern exists: `MingGoRTS_IDE/GUI/I18N.h` (enum-keyed, compiled-in). Recommend data-driven JSON via `Serialization/JsonParser.h` instead — AGENTS.md says don't add a third-party JSON lib | All current demo strings are hardcoded literals — needs a full pass, mechanical |
| 33 | Fixed-advance numerics (timers/countdowns) | TRIVIAL | Consolas/mono font for numeric role, or per-digit column layout | Spec's mono-in-tactical-sim is free; "tabular figures elsewhere" just needs consistent-width rendering (CJK fonts' full-width digits are already fixed-advance) |
| 34 | TC letterspacing .1–.5em | MODERATE | `ImFontConfig::GlyphExtraAdvanceX` (imgui.h L3657) — per-font-instance extra advance | Needs separate `ImFont` per tracking variant (font × size × tracking matrix = memory cost, see R2). EN mode collapse = same font with spacing=0 instance |
| 35 | Hold-to-confirm 急停 | TRIVIAL | `IsMouseDown` + timer overlay | |
| 36 | Keyboard/gamepad reachability, no hover-only info | MODERATE | `io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard/Gamepad`; custom draw-list widgets need `InvisibleButton`+`NavHighlight` wiring | Discipline item more than subsystem |
| 37 | Radial menu (reserved, not shipped) | MODERATE (deferred) | Pie of `InvisibleButton`s around cursor — doable when needed | Spec only requires not designing it out — free to comply |
| 38 | Reduced-motion toggle | TRIVIAL | Global bool gating drift strength (`SetFogDrift(...,0)` already supports 0), pulse alpha, transitions | Turbulence strength=0 is already the designed off-switch |
| 39 | Screen shake | TRIVIAL | Camera target offset + decay | Spec defaults it OFF anyway |
| 40 | CJK font assets (PMingLiU/msjh/Noto Serif TC) | MODERATE + legal risk | Currently loads `C:/Windows/Fonts/msjh.ttc` — **cannot ship** (Microsoft license, Windows-only). Must bundle e.g. Noto Serif TC + Noto Sans TC (~15–25MB each TTF) | 1.93 dynamic atlas keeps runtime memory fine; asset size + license are the real cost. Consolas (tactical-sim numeric) same problem → bundle a free mono |
| 41 | **Audio: doctrine motif score, 離魂 muffle, routed din, typing audio, 音效 setting** | HEAVY | `Audio/AudioSystem.h` is a **stub**: OpenAL FetchContent commented out (`CMakeLists.txt` L108–115), `SetupOpenAL()` is log-only (AudioSystem.cpp L246–248), `LoadWAV` parses but nothing plays. Needs: vendored backend (miniaudio or openal-soft as `external/<lib>/` — allowed by vendoring rules) + mixer + per-event SFX | "That card's motif joins the live score" implies **adaptive layered music** — additional system on top of playback. 離魂 muffle = mix bus with lowpass — DSP feature |
| 42 | Adaptive score (motifs join/leave per doctrine) | HEAVY | Same as #41 — stems/layers mixing subsystem | Can ship battle silent or with 2–3 static loops first |
| 43 | Rotated text | HEAVY | No draw-list transform; would need glyph-quad transform pass or render-text-to-texture | Avoid by design (#22) |
| 44 | True scene blur / 離魂 visual muffling | HEAVY | Needs RTT + blur shader (see #19) | Visual fallback: darken + desaturate scrim — MODERATE |
| 45 | SDF text | NOT NEEDED | ImGui 1.93 rasterizes glyphs at requested size on demand — crisp at 75–150% without SDF | Remove from dependency list |

---

## Top risks

| # | Risk | Severity | Mitigation |
|---|---|---|---|
| R1 | **Audio is a hollow stub.** Every "Game Feel & Juice" item except visual ones depends on it: motif score, 離魂 muffle, routed din, brush-paper typing audio, the 音效 settings slider. OpenAL is commented out of CMake; `SetupOpenAL` logs and returns. | High | Vendor `miniaudio` (single-header, fits `external/<lib>/` + README rule) in an audio milestone; ship battle silent until then. Adaptive score = phase 2 of that milestone |
| R2 | **Font licensing + instance matrix.** `msjh.ttc`/PMingLiU/Consolas are Windows system fonts — not shippable. Spec wants 4 themes × serif/sans × 4 type roles × tracking variants = many `ImFont` instances. | High | Bundle Noto Serif TC + Noto Sans TC + one free mono. Collapse tracking variants to two `GlyphExtraAdvanceX` settings (normal/.2em). Runtime memory is fine under the 1.93 dynamic atlas; asset download size (~40–50MB fonts) is the cost |
| R3 | **No render-target/FBO API.** Anything wanting real blur (cloud fill, 離魂 visual muffling, ink-bleed transition over live scene) can't ping-pong the framebuffer through `OpenGLRenderer`. | Medium | Use spec fallbacks: procedural radial-gradient textures for clouds, scrim+desaturate for muffling, stylized animated-scrim transitions. Only build `Framebuffer` if a shipped scene truly needs it |
| R4 | **No draw-list transform → no rotated text, no true clip paths.** Rotated cards, torn edges, tilted stamps all hit this. | Medium | Rotate card geometry only (text stays upright — spec-sanctioned); torn edges via procedural jagged polylines or sprites; never rotate text |
| R5 | **Minimap + world→screen overlays are unbuilt surface area.** Minimap, entanglement links, rout arcs, plan arrows, reveal rings, on-map labels all need either a `WorldToScreen` helper or flat-mesh 3D equivalents. | Medium | One ~20-line projection helper unlocks most of it; minimap as 2D draw-list from `BattleMap` grid (not scene RTT). Schedule as one overlay sprint |
| R6 | **UI scale is not one flag.** `FontGlobalScale` alone produces big-text-in-small-layout. Needs `ScaleAllSizes` + per-size fonts + audit of every hardcoded `SetNextWindowSize/Pos` (current demo hardcodes many). | Medium | Centralize all HUD geometry in an anchor/layout struct parameterized by scale — do this when porting the demo HUD to the real UI layer, not after |
| R7 | **Seepage-transition readability through the swap.** Spec accepts the interruption but requires dual-encoded state to stay legible mid-crossfade. | Low-Med | During transition, keep shape/icon encodings (which survive hue shifts) and never dual-encode by hue — already a spec rule; enforce in widget code |

---

## Recommended scope trims

1. **Defer all audio juice to an audio milestone.** Ship v1 silent except (optionally) a vendored-miniaudio minimal pass: UI clicks + one ambient loop. The motif-score and 離魂-muffle fantasies need a mixer + DSP — don't block the UX milestone on them. Remove or grey the 音效 slider until then.
2. **Ship 2 themes fully, 2 as token-only.** tactical-sim is nearly free (rect-native, flat, hairlines — pure draw-list) and war-map needs only shadow rects + jagged polylines. command-room (bevels, lamp falloff) and ink-chronicle (brush sprites, vertical text) can launch as token-color variants and get their sprite/draw pass in a polish milestone. The anchor contract makes this safe — same layout, thinner dressing.
3. **Clouds: procedural radial-gradient texture, never blur.** One 64×64 generated RGBA blob → `Texture::LoadFromMemory` → billboarded quad or `ImGui::Image`. Matches the spec's own fallback path and looks better than 3 stacked circles.
4. **Vertical text: CJK-glyph stacking only.** A ~25-line `AddVerticalText` for ink-chronicle titles/labels. Forbid Latin/digits in vertical contexts (string-table lint or convention) — sidesteps the rotation problem entirely.
5. **Rotation: rotate containers, never text.** ±2° card tilt via manual corner math on rects; text axis-aligned. Or take the spec's explicit fallback (plain rects) for v1.
6. **Minimap = 2D draw-list from grid data.** River as rects, dots as circles, camera frame via corner projection. ~250 lines, zero new engine surface.
7. **Fonts: bundle 3 faces** (Noto Serif TC, Noto Sans TC, one free mono), 2 tracking variants, rely on 1.93 dynamic atlas for sizes. Drop PMingLiU/Consolas references from spec → mark as "or bundled equivalent."
8. **SDF text: remove from dependency list.** Unnecessary with the current ImGui.
9. **Springs for later:** radial menu (gamepad), true blur pipeline, adaptive score, rotated text — all deferred, none required for the spec's core fantasy (anchor-stable HUD + theme swap + chronicler voice), which is achievable with today's primitives.

---

## Asset/production cost note (non-engineering)

The 4-theme cost splits into: **tokens** (free — the YAML already is the data), **per-theme draw dressing** (engineering, classified above), and **optional sprite sets** (paper grain, torn edges, brass plates, stamp roughness, ink strokes — real 2D art production, est. 15–40 assets per dressed theme). The spec's fallback design means art can lag code safely: every sprite has a rect equivalent already specified.
