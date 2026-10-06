---
name: MingGoRTS
description: 'Token-driven UI design system for a Republican-era doctrine RTS: four player-selectable themes (war-map 泥濘沙盤 / command-room 軍電作戰室 / tactical-sim 現代軍事 / ink-chronicle 水墨史卷) sharing one anchor layout, plus myth-layer full-theme seepage swaps.'
title: MingGoRTS Visual Identity
type: design-system
status: final
created: 2026-09-18
updated: 2026-09-18
themes: 'war-map 泥濘沙盤 · command-room 軍電作戰室 · tactical-sim 現代軍事 · ink-chronicle 水墨史卷'
sources:
  - _bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/gdd.md
  - _bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/epics.md
  - _bmad-output/planning-artifacts/briefs/brief-MingGoRTS-2026-09-17/brief.md
  - _bmad-output/planning-artifacts/briefs/brief-MingGoRTS-2026-09-17/addendum.md
  - _bmad-output/planning-artifacts/potato-engine-roadmap.md
  - docs/REFERENCE_GAMES_MECHANICS.md
colors:
  # ── Shared gameplay semantics ─────────────────────────────────────────────
  # Theme-agnostic defaults; a theme may re-hue a role but never re-mean it.
  # Every semantic is dual-encoded (shape+icon); hue is reinforcement only.
  color.intel: '#d8a83c'
  color.cp: '#9c2f22'
  color.morale-ok: '#8a7434'
  color.morale-warn: '#b03a2a'
  color.morale-break: '#9c2f22'
  color.fog-cloud: '#6a4a8c'
  color.friendly: '#55603c'
  color.enemy-confirmed: '#7a3a2a'
  color.sel: '#b03a2a'
  color.myth-gold: '#d4a23c'
  color.myth-foxfire: '#6fd8c8'
  # ── war-map 泥濘沙盤 (documented in direction-war-map.html) ────────────────
  theme.war-map.paper: '#d3c096'
  theme.war-map.paper-hi: '#e6d9b0'
  theme.war-map.paper-dk: '#b3a172'
  theme.war-map.mud: '#8a7a58'
  theme.war-map.ink: '#37301f'
  theme.war-map.pencil: '#4a4234'
  theme.war-map.grease: '#8f2d20'  # deepened from #b03a2a — 4.55:1 on paper (was 3.37, below floor)
  theme.war-map.river: '#7d8f92'
  theme.war-map.river-dk: '#5e7478'
  theme.war-map.friendly: '#55603c'
  theme.war-map.friendly-fill: '#55603c'  # 18% alpha tint of friendly
  theme.war-map.cloud: '#6a4a8c'
  theme.war-map.enemy: '#7a3a2a'
  theme.war-map.pin: '#222222'
  theme.war-map.wax: '#9c2f22'
  theme.war-map.wood: '#6a5236'
  theme.war-map.morale: '#8a7434'
  theme.war-map.desk: '#241f13'
  # ── command-room 軍電作戰室 (documented in direction-command-room.html) ─────
  theme.command-room.wood: '#241a12'
  theme.command-room.wood-hi: '#3a2a1a'
  theme.command-room.wood-dk: '#140d08'
  theme.command-room.brass: '#6f5226'  # deepened from #8a6a3a — 5.2:1 on slip (was 3.56, below floor)
  theme.command-room.brass-hi: '#d8b878'
  theme.command-room.brass-dk: '#5a4322'
  theme.command-room.slip: '#e6d9b8'
  theme.command-room.slip-dk: '#c9b98f'
  theme.command-room.ink: '#2e2a20'
  theme.command-room.seal: '#a8322a'
  theme.command-room.estop: '#a02020'
  theme.command-room.map: '#c9b489'
  theme.command-room.river: '#6a7a80'
  theme.command-room.friendly: '#4c5a44'
  theme.command-room.cloud: '#6a4a8c'
  theme.command-room.enemy: '#7a3a2a'
  theme.command-room.label: '#a8843c'
  theme.command-room.plate-dk: '#1a130c'
  theme.command-room.mini-f: '#7d9a72'
  theme.command-room.mini-e: '#a84a3a'
  theme.command-room.room: '#0c0805'
  # ── tactical-sim 現代軍事 (documented in direction-tactical-sim.html) ───────
  theme.tactical-sim.bg: '#0b1014'
  theme.tactical-sim.terrain: '#10181e'
  theme.tactical-sim.panel: '#0d141a'  # 90% alpha panel fill
  theme.tactical-sim.hairline: '#24333d'
  theme.tactical-sim.hairline2: '#2e4450'
  theme.tactical-sim.text: '#d9e6ea'
  theme.tactical-sim.dim: '#7d939e'
  theme.tactical-sim.faint: '#7a919c'  # brightened from #4a5f6a — 5.7:1 on panel (was 2.86, below floor)
  theme.tactical-sim.teal: '#2fd4c4'
  theme.tactical-sim.teal-dim: '#2fd4c4'  # 55% alpha teal
  theme.tactical-sim.red: '#e0483f'
  theme.tactical-sim.enemy: '#b0402f'
  theme.tactical-sim.cloud: '#35d0c5'
  theme.tactical-sim.river: '#12222b'
  theme.tactical-sim.river-edge: '#1e3642'
  theme.tactical-sim.amber: '#d8a83c'
  theme.tactical-sim.strength: '#9fb4bd'
  theme.tactical-sim.unit-text: '#aef0e8'
  theme.tactical-sim.page: '#05080b'
  # ── ink-chronicle 水墨史卷 (documented in direction-ink-chronicle.html) ─────
  theme.ink-chronicle.paper: '#efe7d4'
  theme.ink-chronicle.paper-warm: '#e8ddc4'
  theme.ink-chronicle.ink: '#2e2c26'
  theme.ink-chronicle.ink-soft: '#55524a'
  theme.ink-chronicle.ink-faint: '#2e2c26'  # 35% alpha ink wash
  theme.ink-chronicle.wash: '#5a6469'
  theme.ink-chronicle.vermilion: '#a33b28'
  theme.ink-chronicle.vermilion-dk: '#8a3a26'
  theme.ink-chronicle.cloud: '#6e508c'
  theme.ink-chronicle.friendly: '#3a382f'
  theme.ink-chronicle.morale: '#7a6a3a'
  theme.ink-chronicle.slip: '#f4edda'
  theme.ink-chronicle.scroll-edge: '#d9cfae'
typography:
  war-map:
    display: { fontFamily: 'PMingLiU, "Noto Serif TC", "Songti TC", serif', fontSize: '20px', fontWeight: '700', letterSpacing: '.32em' }
    body: { fontFamily: 'PMingLiU, "Noto Serif TC", "Songti TC", serif', fontSize: '14px', fontWeight: '400', lineHeight: '1.55' }
    numeric: { fontFamily: 'PMingLiU, "Noto Serif TC", "Songti TC", serif', fontSize: '26px', fontWeight: '700', letterSpacing: '.12em' }
    chronicler: { fontFamily: 'PMingLiU, "Noto Serif TC", "Songti TC", serif', fontSize: '14px', fontWeight: '400', lineHeight: '1.55', letterSpacing: '.05em' }
  command-room:
    display: { fontFamily: 'PMingLiU, "Noto Serif TC", "Songti TC", serif', fontSize: '19px', fontWeight: '700', letterSpacing: '.2em' }
    body: { fontFamily: 'PMingLiU, "Noto Serif TC", "Songti TC", serif', fontSize: '14px', fontWeight: '400', lineHeight: '1.6' }
    numeric: { fontFamily: 'PMingLiU, "Noto Serif TC", "Songti TC", serif', fontSize: '24px', fontWeight: '700', letterSpacing: '.15em' }
    chronicler: { fontFamily: 'PMingLiU, "Noto Serif TC", "Songti TC", serif', fontSize: '14px', fontWeight: '400', lineHeight: '1.6' }
  tactical-sim:
    display: { fontFamily: '"Microsoft JhengHei", "PingFang TC", "Noto Sans TC", sans-serif', fontSize: '15px', fontWeight: '700', letterSpacing: '.15em' }
    body: { fontFamily: '"Microsoft JhengHei", "PingFang TC", "Noto Sans TC", sans-serif', fontSize: '13px', fontWeight: '400', lineHeight: '1.55' }
    numeric: { fontFamily: 'Consolas, "Microsoft JhengHei", monospace', fontSize: '24px', fontWeight: '300', letterSpacing: '.14em' }
    chronicler: { fontFamily: '"Microsoft JhengHei", "PingFang TC", "Noto Sans TC", sans-serif', fontSize: '12px', fontWeight: '400', lineHeight: '1.55' }
  ink-chronicle:
    display: { fontFamily: 'PMingLiU, "Noto Serif TC", "Songti TC", serif', fontSize: '36px', fontWeight: '700', letterSpacing: '.42em' }
    body: { fontFamily: 'PMingLiU, "Noto Serif TC", "Songti TC", serif', fontSize: '15px', fontWeight: '400', lineHeight: '1.7', letterSpacing: '.06em' }
    numeric: { fontFamily: 'PMingLiU, "Noto Serif TC", "Songti TC", serif', fontSize: '22px', fontWeight: '700' }
    chronicler: { fontFamily: 'PMingLiU, "Noto Serif TC", "Songti TC", serif', fontSize: '15px', fontWeight: '400', lineHeight: '1.7', letterSpacing: '.06em' }
rounded:
  sm: '2px'
  md: '4px'
  lg: '8px'
  full: '9999px'
  DEFAULT: '2px'
spacing:
  '1': '4px'
  '2': '8px'
  '3': '12px'
  '4': '16px'
  '6': '24px'
  '8': '32px'
  '11': '44px'
  hud-margin: '36px'
  panel-pad: '14px'
  roster-gap: '18px'
  feed-gap: '14px'
components:
  button:
    minHeight: '48px'
    fill: 'theme surface ({colors.theme.war-map.paper-hi} / brass plate / {colors.theme.tactical-sim.panel} / ink seal-square)'
    focusRing: '2px {colors.color.sel} equivalent per theme'
    note: 'rect-native in tactical-sim; paper/brass/seal materials elsewhere — texture or gradient, no shader'
  panel:
    fill: 'theme surface token'
    border: '1px–3px theme border token'
    elevation: 'paper drop-shadow / brass bevel / translucent flat / none'
    note: 'ImGui rect + border; torn-paper clip edges and wood grain are sprite/texture dependencies'
  bar:
    height: '3px (tactical) – 9px (war-map/ink)'
    fillOk: '{colors.color.morale-ok}'
    fillWarn: '{colors.color.morale-warn}'
    fillBreak: '{colors.color.morale-break}'
    track: 'theme ink at 10–15% alpha'
    note: 'pure draw-list rect; ink broken-stroke collapse fill is a 1-bit pattern, not a shader'
  seal-stamp:
    shape: 'circle (wax / seal stamp / estop) or square (ink seal, miniseal)'
    fill: '{colors.color.cp} family per theme'
    note: 'ImGui circle/rect + rotated quad; stamped-ink rough edge is an optional sprite'
  card-slot:
    size: '~110×56 paper card → flex chip row (tactical)'
    header: '3–5px accent strip keyed to trigger family'
    body: 'trigger→action line + cooldown / execution-rate line'
    note: 'rect + two text lines; draggable'
  cloud-marker:
    scale: 'proportional to candidate-cluster probability P'
    drift: 'slow turbulence wander; reduced-motion = static'
    fill: '{colors.color.fog-cloud} blurred radial'
    note: 'SHADER/SPRITE DEPENDENCY (gaussian blur); fallback = 3 stacked alpha circles + dashed bound ring'
  roster-row:
    cols: 'portrait? | name | status-tag | strength bar | morale bar | stamina bar (完整 density only) [ASSUMPTION: stamina bar presentation pending — Squad API exposes stamina, no mock carries it]'
    alert: 'status tag + icon + accent edge (ring / left-border / seal); never hue alone'
    note: 'rect-native'
  timeline:
    track: 'single rail with event ticks keyed to feed entries; scrub handle + click-to-jump'
    marker: 'per-event-type icon; casualty events tinted {colors.color.morale-break}'
    note: 'rect-native'
  portrait:
    source: 'VRM officer render — named commanders and squad captains (GDD art direction)'
    surfaces: 'enemy dossier header · roster row thumb · planning squad card · hero frame'
    fallback: 'seal-stamp initial glyph (周/格) in a themed frame when no portrait asset exists'
    note: 'VRM→offscreen render or pre-baked card art; UI consumes a texture region — SPRITE PIPELINE DEPENDENCY'
---

# DESIGN.md — MingGoRTS

## Brand & Style

MingGoRTS is one officer's eye looking at the same battle twice: once the night before, bent over a map table writing doctrine (the planning layer); once for real, camera down in the mud watching the orders collide (the execution layer). The brand promise is **「昨夜於此桌，仗已打完」— the battle was won last night, at this desk.** Planning and execution never cut to a different UI world: the sand table simply comes alive.

Four player-selectable personalities share that single spine. They are skins over identical anchors — the player swaps the room, never the map of where things live:

- **war-map 泥濘沙盤** — a wargame sand table mudded by fingerprints. There is almost no chrome: HUD elements are pinned paper scraps, pencil gridlines, grease-pencil circles. The most diegetic theme; the officer's pencil finishes the battle before dawn.
- **command-room 軍電作戰室** — a walnut-and-brass midnight operations room. Events arrive as telegram slips, orders land as seals, squads are dossier folders. Medium density; every element feels like it has weight and could be picked up.
- **tactical-sim 現代軍事** — SC2-school esports discipline. Flat translucent panels, hairline borders, teal probability rendering, bilingual micro-labels. For reading the board, not watching the drama. The most ImGui-native theme.
- **ink-chronicle 水墨史卷** — the chronicler's scroll. Xuan paper, layered ink dilution, vermilion seals; lowest density and most whitespace. The UI reads as a battle report being brushed live — event text is literally the chronicle.

**Myth seepage is the brand signature.** When a myth-layer event fires, the HUD performs a *full theme swap* — the entire interface becomes the signal of which world layer the player is perceiving — rides a short transition motion, then ebbs back when the event ends. We accept the readability interruption on purpose (decision log R5): the player should feel the world slip out from under the desk.

Anti-chrome rule across all themes: decoration must be plausible inside the theme's fiction (paper, brass, glass, ink). No ornament that the fiction could not physically produce.

## Colors

Two tiers: **shared gameplay semantics** (fixed meaning across all themes — this is what the player learns once) and **theme materials** (the room the game is played in).

### Shared semantics

| Token | Default | Meaning | Dual-encoding (mandatory) |
|---|---|---|---|
| `color.intel` | `#d8a83c` | Intelligence resource accent (0–10 budget) | Number + 情報 label; hue optional |
| `color.cp` | `#9c2f22` | Command points & intervention affordance | Pip/stud/seal count + CP-n label |
| `color.morale-ok` | `#8a7434` | Morale fill, steady | Bar fill |
| `color.morale-warn` | `#b03a2a` | Morale wavering / annotation alert | Status tag + icon + accent |
| `color.morale-break` | `#9c2f22` | Morale collapse / casualty red | ⚠ icon + broken-fill + rout marker |
| `color.fog-cloud` | `#6a4a8c` | Unconfirmed enemy probability cloud | Cloud shape + `?`/P readout |
| `color.friendly` | `#55603c` | Player forces | Unit marker shape (pill/dab/piece) |
| `color.enemy-confirmed` | `#7a3a2a` | Revealed enemy units | Solid marker + 「已確認」 tag |
| `color.sel` | `#b03a2a` | Selection / focus | Ring + corner brackets |
| `color.myth-gold` | `#d4a23c` | Myth-layer accent during seepage [ASSUMPTION: no hex recorded in sources; shrine-gold family per GDD art direction] | Seepage frame + event icon |
| `color.myth-foxfire` | `#6fd8c8` | Fox-fire / spectral accent [ASSUMPTION as above] | Same |

### Semantic resolution per theme

Themes re-hue materials but never re-mean semantics. Where a theme diverges:

| Semantic | war-map | command-room | tactical-sim | ink-chronicle |
|---|---|---|---|---|
| intel | ink on `paper-hi` (no hue) | ink on `slip` | `amber` | `ink` |
| cp | `wax` dots | `brass` studs + `estop` | `teal` pips | `vermilion` dots |
| morale-ok | `morale` `#8a7434` | `brass` | `teal` | `morale` `#7a6a3a` |
| morale-warn/break | `grease`/`wax` | `seal`/`estop` | `red` | `vermilion` (broken-stroke fill) |
| fog-cloud | `cloud` `#6a4a8c` | `cloud` `#6a4a8c` | `cloud` `#35d0c5` (teal re-hue) | `cloud` `#6e508c` mist |
| friendly | `friendly` `#55603c` | `friendly` `#4c5a44` | `teal` outline | `friendly` `#3a382f` ink dab |
| enemy-confirmed | `enemy` `#7a3a2a` | `enemy` `#7a3a2a` | `enemy` `#b0402f` | `vermilion-dk` `#8a3a26` |
| selection | `grease` ring | `brass-hi` ring | `teal` ring + corners | `ink` ring |

### Theme palettes (lifted from the four direction spines)

**war-map 泥濘沙盤** — `paper #d3c096` sand-table kraft ground · `paper-hi #e6d9b0` card/slip face · `paper-dk #b3a172` fold & edge shadow · `mud #8a7a58` stain blotches · `ink #37301f` hand-written olive-black · `pencil #4a4234` annotations & gridlines · `grease #8f2d20` red crayon (attack arrows, circles, selection; deepened for contrast floor) · `river #7d8f92` / `river-dk #5e7478` water · `friendly #55603c` olive stamp (+ `friendly-fill` 18% tint) · `cloud #6a4a8c` enemy-track purple ink · `enemy #7a3a2a` confirmed red-brown · `pin #222222` · `wax #9c2f22` emergency-stop wax seal · `wood #6a5236` bridge timber · `desk #241f13` surround.

**command-room 軍電作戰室** — `wood #241a12`/`wood-hi #3a2a1a`/`wood-dk #140d08` walnut room · `brass #6f5226`/`brass-hi #d8b878`/`brass-dk #5a4322` fittings (base deepened for contrast floor) · `slip #e6d9b8`/`slip-dk #c9b98f` telegram & dossier paper · `ink #2e2a20` · `seal #a8322a` stamp red · `estop #a02020` porcelain red button · `map #c9b489` table map · `river #6a7a80` · `friendly #4c5a44` enamel green · `cloud #6a4a8c` · `enemy #7a3a2a` · `label #a8843c` secondary brass text · `plate-dk #1a130c` clock face · `mini-f #7d9a72`/`mini-e #a84a3a` minimap dots · `room #0c0805` surround.

**tactical-sim 現代軍事** — `bg #0b1014`/`terrain #10181e` near-black field · `panel rgba(13,20,26,.90)` · `hairline #24333d`/`hairline2 #2e4450` · `text #d9e6ea`/`dim #7d939e`/`faint #7a919c` (brightened for contrast floor) · `teal #2fd4c4` primary accent (selection, morale, CP) · `red #e0483f` warning/enemy highlight · `enemy #b0402f` confirmed solid · `cloud #35d0c5` probability cyan · `river #12222b`/`river-edge #1e3642` · `amber #d8a83c` intel bar · `strength #9fb4bd` · `unit-text #aef0e8` · `page #05080b` surround.

**ink-chronicle 水墨史卷** — `paper #efe7d4`/`paper-warm #e8ddc4` xuan ground · `ink #2e2c26`/`ink-soft #55524a`/`ink-faint` layered dilution · `wash #5a6469` water rendering (alpha-graded) · `vermilion #a33b28` seal/warning red · `vermilion-dk #8a3a26` confirmed enemy · `cloud #6e508c` purple mist · `friendly #3a382f` ink dab · `morale #7a6a3a` ochre · `slip #f4edda` plan slips · `scroll-edge #d9cfae` surround.

**Contrast floor:** primary text ≥ 4.5:1 against its panel/field (WCAG-derived). Verified pairs after R-review fixes: `grease #8f2d20` on `paper #d3c096` ≈ 4.55:1 · `brass #6f5226` on `slip #e6d9b8` ≈ 5.2:1 · `faint #7a919c` on `panel #0d141a` ≈ 5.7:1 · `text` on `panel` ≈ 11:1 · `ink` on `paper` ≈ 12:1. Paper themes rely on `ink`-family text only — never set `pencil`/`ink-soft`/`faint` below 11px on paper. Gloss-tier tokens (`faint`/`pencil`/`ink-soft`) may decorate but may **never carry gameplay-critical information** — not even alongside a hue cue.

## Typography

Four roles, resolved per theme. The **chronicler** role is a *register*, not a different face: it carries 史官體 event text, the battle report, and roster entries — in tactical-sim it uses the body sans but keeps the archival voice.

| Role | war-map | command-room | tactical-sim | ink-chronicle |
|---|---|---|---|---|
| display | serif 700 20px, .32em tracking (stencil) | serif 700 19px, .2em | sans 700 15px, .15em | serif 700 36px, .42em, vertical-rl titles |
| body | serif 14px / 1.55 | serif 14px / 1.6 | sans 13px / 1.55 | serif 15px / 1.7 |
| numeric | serif 700 26px (stamped figures) | serif 700 24px on `plate-dk` | Consolas 300 24px (true mono) | serif 700 22px |
| chronicler | serif 14px / 1.55 | serif 14px / 1.6 | sans 12px / 1.55 | serif 15px / 1.7 |

Ramp observed across mocks: 10 / 11 / 12 / 14 / 15 / 19 / 22 / 26 / 36 px — roughly 1.25–1.33 steps from a 13–15px body base.

Rules:

- TC letterspacing (`.1em`–`.5em`) is part of the archival voice; keep it in TC mode, collapse to normal tracking in EN mode.
- Numerics must not jitter: fixed-advance rendering for countdowns and timers (mono in tactical-sim; tabular figures elsewhere).
- Bilingual from day one — string table, no hardcoded strings. Budget ~+30% horizontal expansion for EN [ASSUMPTION: standard CJK→Latin i18n practice].
- Labels in `faint`/`pencil`/`ink-soft` never carry gameplay-critical information — gloss only (see contrast floor).
- **Shipping fonts** — bundle libre faces: Noto Serif TC (serif roles), Noto Sans TC (sans roles), a libre mono (Noto Sans Mono) for numeric. PMingLiU / Microsoft JhengHei / Consolas are Windows dev-machine fallbacks only, never a shipping dependency. [ASSUMPTION: exact libre picks pending license check; Noto family is OFL]

## Layout & Spacing

**One anchor contract across all themes.** Theme swaps and myth seepage restyle surfaces; they never move anchors — muscle memory is the contract.

- **Top center / top rail** — phase 「交戰」, battle clock, timescale ×n, pause key hint.
- **Top right** — intel readout, CP pips, 急停 emergency stop.
- **Left column** — squad roster (4 squads + reserved hero frame row).
- **Right column** — doctrine event feed (paper notes / telegrams / log rows / chronicle lines).
- **Bottom right** — CP intervention buttons (攻進/撤回/駐守) above minimap.
- **Bottom left** — doctrine card strip (read-only mirror of the committed plan; stratagem menu reserved here).
- **Center** — battlefield/table/scroll.

8px base grid (`{spacing.2}`); organic themes (war-map, ink-chronicle) permit ±2° rotation and irregular card offsets for hand-placed feel, but components still land on the grid. HUD safe inset `{spacing.hud-margin}` ≈ 36px at 1080p reference; paper themes may breathe wider, tactical-sim tightens to 16px.

HUD density is a player setting (極簡/標準/完整): density changes *how many* surfaces are pinned, never *where* they anchor. See EXPERIENCE.md → Information Architecture for per-tier inventory.

## Elevation & Depth

- **war-map** — paper drop-shadows under every pinned card; desk vignette frames the stage. Depth = physical layering of paper.
- **command-room** — heaviest: brass bevels, inset table shadow, lamp-lit radial falloff, room vignette.
- **tactical-sim** — flat. Depth only via panel translucency and hairline separation; selection gets a soft glow, not a shadow.
- **ink-chronicle** — no cast shadows on UI chrome; depth is ink dilution and the bleed halo around dabs.

Global rule: at most three surface layers (field → pinned surfaces → transient stamps/toasts/seals). Pause and post-battle **replace** the view; nothing stacks into nested modals. CP-seizure and seepage transitions may add one full-screen scrim layer, and no more.

## Shapes

- **war-map** — torn/clip-path polygon edges on slips, rotated pinned cards, elliptical grease rings. Torn edges are sprite/mask work; plain rects with rotation are the acceptable ImGui fallback.
- **command-room** — 0–5px radii; folder ears, torn slip tops, double-ring seal stamps, circular estop.
- **tactical-sim** — 0–2px, strictly rectilinear; 1px hairlines everywhere. Fully draw-list native.
- **ink-chronicle** — 2–4px; pill-shaped ink dabs, square seal chops, brush-stroke strokes with flying-white ends (gradient-masked rects — texture dependency).
- Pills (`{rounded.full}`) are reserved for unit markers and pips; seals are circular (wax/stamp) or square (ink chop) per theme fiction.
- Focus/selection ring: 2px equivalent of `{colors.color.sel}` — ellipse (war-map/ink), double border (command-room), ring + corner brackets (tactical-sim).

## Components

Anatomy first, then how the *same* component dresses in each theme. Implementation notes flag what is ImGui draw-list native vs. sprite/shader dependency (decision log R4: token-table-driven ImGui style swap; special FX marked as dependencies).

### Button / CP intervention button
Rect-native. war-map: thick-ink-bordered paper chip, slight rotation. command-room: brass gradient plate button, `hot` state in `seal` red. tactical-sim: flat cell with `[1]` keycap hint, `hot` = `teal` tint. ink-chronicle: square seal chop, single character (攻/撤/守). Disabled state = drained ink/dimmed brass/`faint` text — never hue alone (add strike or hollow state).

### Panel / surface
Rect + border. war-map paper cards carry drop-shadow and pin dot sprite; command-room adds folder ear and bevel (2-stop gradient rect acceptable); tactical-sim is a single translucent rect + hairline (cheapest); ink-chronicle is a bare rect with `ink-faint` border, often replaced by a brush-stroke divider.

### Bar (兵力 strength / 士氣 morale / stamina)
Pure draw-list rects in all themes. war-map: 9px bordered box, ink fill for strength, `morale` ochre for morale. command-room: 7px inset slot, `friendly`/`brass` fills. tactical-sim: 3px hairline track, `strength`/`teal` fills, mono percentage. ink-chronicle: 9px rounded brush fill with tapering end; **collapse state swaps to a broken-stroke repeating fill** (1-bit pattern — dual-encoding, not a shader).

### Seal / stamp
The signature control: circular wax (war-map estop, `{colors.color.cp}`), double-ring round seal stamp (command-room seizure `接管中`), square ink chop (ink-chronicle 止/急), or bordered chip (tactical-sim estop). Drawn as circle/rect + rotated text quad; rough stamped-ink texture is an optional sprite. Used for: 急停, CP order confirmation, telegram 達/急 stamps, 開戰 commit.

### Card slot (doctrine)
war-map: 110×56 paper card, 3px top strip in trigger-family accent. command-room: taller dossier card in a brass drawer. tactical-sim: flex chip with 2px top strip + mono condition line. ink-chronicle: vertical 籤條 slip, 5px top strip. All: name line + condition/cooldown line; drag-and-drop; empty slot shows dashed outline + 「空槽」.

### Cloud marker (probability fog)
SHADER/SPRITE DEPENDENCY: 3 blurred radial blobs + bound ring + label. Scale ∝ cluster probability; slow turbulence drift. war-map/ink: purple ink blobs (`{colors.color.fog-cloud}`), conic reveal ring (approximate with segmented arc polylines if shader unavailable). command-room: same + `?` seal ring. tactical-sim: `cloud` cyan blobs + dashed bound ellipse + `P=0.62` mono readout. Fallback without blur: 3 stacked alpha circles.

### Roster row (squad card)
Rect-native. war-map: pinned paper card + warn crayon ellipse around wavering squads. command-room: dossier folder + seizure stamp overlay variant. tactical-sim: list row with 2px left border; collapse state pulses the border (`red`, 1.1s cycle — the fastest animation anywhere; photosensitivity cap). ink-chronicle: brush-bar entry + `warn` boxed status tag. Status tag is the dual-encode anchor: text + icon + accent edge, never hue alone.

### Timeline (post-battle replay)
Single rail; event ticks keyed to feed entries; scrub handle; click tick → jump. Casualty ticks in `{colors.color.morale-break}`; doctrine ticks in trigger-family accent; myth ticks in `{colors.color.myth-gold}`. Rect-native; tick icons are sprites.

### Also themed
- **Unit marker** — pill/dab/piece chip with name; friendly outline vs enemy solid fill (shape + fill = the dual encode).
- **Minimap** — bordered rect, river strip; **dots are shape-coded, never hue-coded**: friendly = square, enemy = diamond, cloud = blurred circle (hue only reinforces; red/green pairs are banned as sole encoders — the most common colorblind pair). Camera frame = outline bracket.
- **Feed entry** — timestamp + text + stamp/icon; urgent = `morale-break` tint + 急 glyph.
- **Dossier panel** — enemy-general card: three labeled meters (aggression/discipline/cunning) filling as intel unlocks.

### Officer portrait (VRM)

Named commanders and squad captains carry faces — the mourning arc needs them. war-map: sepia pin-up photograph, taped corners. command-room: framed dossier photo with brass corner brackets. tactical-sim: flat avatar tile + rank pip. ink-chronicle: ink-sketch bust with vermilion name seal. Fallback when no portrait asset exists: themed frame + seal-stamp initial glyph (周/格). Surfaces: enemy dossier header, roster row thumb, planning squad card, hero frame. SPRITE PIPELINE DEPENDENCY — the UI consumes a texture region; the 3D VRM render happens offscreen or ships as pre-baked card art.

### Surfaces beyond the battle

Same themes, different rooms — non-battle surfaces inherit each theme's materials with density scaled for focus:

- **Title 標題** — the theme's largest display type on a single dominant surface: war-map = creased campaign map with the title inked on it; command-room = dossier cover on the desk; tactical-sim = bare boot screen; ink-chronicle = full scroll with vermilion seal. Menu items as pinned slips / brass tabs / flat rows / vertical ink entries.
- **Planning war-room 規劃層** — mockup: [mockups/planning-war-room.html](mockups/planning-war-room.html). The map table is the centerpiece: deploy zones (dashed outlines), objective/rally pins, reserved plan-arrow layer. Card slots are physical objects (paper cards / dossier cards / chips / 籤條); the editor is stamp-blocks with steppers — never a text field. Validate warnings live in a footer strip; 開戰 is the largest seal-stamp on screen.
- **Pause 暫停** — full-view replacement, never a stacking modal: war-map = the map with a 「暫」 grease circle; command-room = lamp-dimmed room; tactical-sim = scrim + centered panel; ink-chronicle = 「止」 seal over the scroll.
- **Settings 設定** — same dossier/slip/panel/scroll treatment; theme picker shows a live mini-preview of each theme (mandatory — a style choice must be seeable).
- **Post-battle 戰後** — mockup: [mockups/post-battle-chronicle.html](mockups/post-battle-chronicle.html). The chronicler's page: outcome calligraphy first, then report, then roster, then timeline — reading order is the mourning arc. Fallen rows dim + carry the 殞 mini-seal.

Battle HUD key-screen mocks (all four personalities): [direction-war-map](mockups/direction-war-map.html) · [direction-command-room](mockups/direction-command-room.html) · [direction-tactical-sim](mockups/direction-tactical-sim.html) · [direction-ink-chronicle](mockups/direction-ink-chronicle.html). Spines win on any conflict with a mock.

## Do's and Don'ts

- DO dual-encode every gameplay color: faction = marker shape + fill, morale = icon + bar pattern, cloud = blob shape + `?`/P label. DON'T use hue alone for faction, morale, or intel state — anywhere.
- DO keep the 史官體 register in all event, report, and roster text (「後衛營 士氣<30→撤退 執行率70%」). DON'T write system-speak ("Morale threshold triggered") into chronicle surfaces.
- DON'T put hover-only information anywhere — every hover affordance needs a selection/focus equivalent that keyboard and gamepad can reach.
- DO keep anchor positions identical across theme swaps and seepage; DON'T let a theme move where CP, the roster, or the feed live.
- DON'T hardcode a single string — everything flows through the bilingual TC/EN string table from day one.
- DO make myth seepage dramatic but bounded: full-theme swap with transition motion, anchored to event start/end; with motion-reduction on, fall back to a crossfade.
- DON'T let the seepage swap obscure gameplay state — morale-break, seizure countdown, and reveal timers stay dual-encoded and legible through the transition.
- DO keep countdown and timer numerics fixed-advance; DON'T let digits reflow the layout as they tick.
- DON'T stack modals — pause and post-battle replace the view.
- DO reserve the planned slots: hero frame row in roster, stratagem menu beside CP panel, terrain-layer toggle (G-4), counter hints (G-2), rout-chain arcs (G-1), plan-arrow drawing layer (G-5), refit surface (G-9).
- DON'T add chrome a theme's fiction can't produce: no glass in the scroll, no paper grain in the sim.
- DO let the 極簡 density collapse surfaces to badges and pips — never to zero information.

## Implementation & Phasing Notes

Feasibility pass against PotatoEngine (C++20 / OpenGL / ImGui draw-list) — recorded so the spec doesn't promise what the engine can't draw:

- **Theme swap is cheap**: `ImGuiStyle` is a struct — the token table drives an `ApplyToStyle` at runtime; anchors are `SetNextWindowPos` constants so swaps never move furniture. Myth-seepage transition = full-screen scrim crossfade (trivial path); theatrical ink-bleed/flicker variants are sprite/shader options, not requirements.
- **Draw-list ceiling**: no rotated text, no clip-path, no transform — rotated cards rotate the *container* only; torn edges use procedural polyline or a baked sprite; conic reveal rings approximate with segmented arc polylines.
- **CJK vertical text is a ~25-line helper** (upright glyphs stacked vertically) — but vertical layout is banned for Latin/numeric runs; use it for titles and seals only.
- **UI scale needs real plumbing**: `FontGlobalScale` alone only scales text — 75–150% requires `ScaleAllSizes` + dynamic font atlas (ImGui ≥1.93) + all HUD coordinates flowing through the token table. Hardcoded pixel positions anywhere in HUD code is a spec violation.
- **Minimap & world-space overlays** need a `WorldToScreen` helper (~20 lines) — currently absent; minimap renders from `BattleMap` grid data via 2D draw-list (no scene RTT needed).
- **No RTT/FBO in the renderer** — true blur is unavailable; cloud softness uses procedural radial-gradient sprites; 離魂 visual muting uses a scrim, not a blur.
- **Audio is a shell**: `AudioSystem.h` exists but OpenAL is commented out in CMake — every audio-juice item in EXPERIENCE.md §Game Feel (card-motif score, 離魂 muffle, routed din, brush strokes) is gated on an audio milestone (vendored miniaudio is the recommended path). The spec treats audio as an additive layer: all signals have visual equivalents first.
- **Font bundling**: ship 3 libre faces (see Typography); CJK atlas memory is manageable via dynamic atlas.
- **Phasing recommendation** [ASSUMPTION — not yet a user decision]: tactical-sim + war-map ship as the first two *complete* themes (lowest asset cost, cover both ends of the readability↔atmosphere axis); command-room + ink-chronicle follow with their sprite asset batches. Token tables for all four exist from day one, so partial themes degrade to token colors + rect geometry per the Fallback rule.
- **Sprite pipeline gap**: portraits, stamp textures, ink-brush strokes, torn-paper edges all want a texture-atlas path that doesn't exist yet — flagged as the single largest shared dependency.
