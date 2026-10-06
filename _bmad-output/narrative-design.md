---
title: 'Narrative Design Document'
project: 'MingGoRTS'
date: '2026-09-29'
author: 'potat'
version: '1.0'
stepsCompleted: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]
status: 'complete'
narrativeComplexity: 'Heavy'
gdd: '_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-29/gdd.md'
---

# Narrative Design Document

## MingGoRTS

### Document Status

This narrative document is being created through the GDS Narrative Workflow.

**Narrative Complexity:** Heavy
**Steps Completed:** 1 of 11 (Initialize)

---

## Story Foundation

### Narrative Premise

You are the last chronicler of a falling dynasty — not a commander, but a
writer of war. In a world where history and myth occupy the same map, you
compose doctrine for named squads and watch them execute your words inside
a fog you cannot fully see. Victory is not annihilation but an enemy
laying down arms; and the question you finally face is whether what you
wrote was the truth — or the version your memory wanted to believe.

### Core Themes

1. **Writing vs. Truth** — the battle report is a literary form, not a
   transcript; the player writes history while being written by it
   (embodied by HistorianReport, IntelLedger, and the four-voice ending).
2. **Subversion vs. Force** — violence is always an option and always has
   a price; the 墮落 ratchet is the theme's mechanical incarnation.
3. **Governance vs. Conquest** — holding the map is not winning the
   people; 民心 is the hardest doctrine to write.
4. **History vs. Myth** — the dual-layer world: every historical event
   has a mythological counterpart, and the player chooses which layer to
   believe.

### Tone and Atmosphere

**Tone:** Historian register (史官體) — restrained, distanced, favoring
elliptical counts ("斬獲甚多" rather than numbers) and hearsay mood
("據聞", "或云").

**Atmosphere:** Yellowed archives and ink-wash album leaves; pixel-art
visuals carrying grave text — deliberate contrast between cute
presentation and heavy events.

**Emotional Register:** Clear-eyed compassion (清醒的悲憫) — neither
despairing nor heroic.

---

## Story Structure

### Structure Type

**Episodic (chapter-form 章回體) with branching endings**

Not a classical three-act: a chronicle modeled on 編年/回目 conventions —
each chapter is a self-contained 回 with its own conventions. The 回目 are
place-bound: each anchors to a region or deed on the open campaign map,
its order the player's march rather than a fixed index — an 8–12
anchored-scenario arc inside a living world; the ledger's final state
branches into the four-voice ending.

### Act Breakdown

| Phase | Chapters | Function |
|---|---|---|
| Opening movement | Ch.1–3 | Learn to write: doctrine literacy, first casualties, first ledger entries |
| Middle movement | Ch.4–8 | The fog thickens: rival generals read the player back, myth layer intrudes, governance pressure rises |
| Late movement | Ch.9–12 | Writing confronts truth: forged entries surface, four-voice reckoning approaches |
| Coda | Ending | Four-voice ending determined by final ledger state |

## Story Beats

### Major Story Beats

**Opening movement (Ch.1–3)**

1. **Commission（受命）** — the player assumes the chronicler's post; the
   first doctrine is handed over; chapter conventions are established.
2. **First Writing（首書）** — the first battle: the player's words are
   executed for the first time — well or badly.
3. **First Loss（初殤）** — the first named squad member dies; the ledger
   records its first irreversible death.
4. **First Governance（治平初試）** — first governance intervention;
   the first choice point on 民心/秩序.

**Middle movement (Ch.4–8)**

5. **Being Read（被讀）** — a rival general counters the player's
   signature doctrine for the first time (RivalDeck narrativized).
6. **Myth Knocks（神話叩門）** — the myth layer forces itself into a
   battle for the first time (earth god manifestation / vengeful dead).
7. **Forgery（偽帳）** — the ledger shows its first forged trace; the
   "is what you wrote true?" theme formally surfaces.
8. **Midpoint: Rout or Rule（大敗或大治）** — a decisive pivot (military
   collapse converting to governance play, or a governance triumph).
9. **The Dossier Lies（敵將判詞）** — a GeneralDossier hearsay-mood
   judgment is proven wrong.
10. **The Corrupt Fork（墮落歧路）** — the player first faces an
    "effective but corrupting" option, and the ratchet leaves a mark.

**Late movement (Ch.9–12)**

11. **Confluence（雙層合流）** — historical and mythological accounts of
    the same event confirm/contradict each other; the player must choose
    a layer.
12. **Judgment of the Brush（筆之審判）** — ledger audit event: forgeries
    and suspect entries are laid open; the player answers for what they
    wrote.
13. **The Final Chapter（最後一回）** — the last campaign battle; every
    accumulated doctrine, roster bond, and rival read settles at once.
14. **Four Voices（四聲部）** — the ledger's final state determines the
    four-voice ending.

### Beat Placement by Act

**Act 1: Setup (Ch.1–3)** — Beats 1–4: learning to write; first costs
recorded.

**Act 2: Confrontation (Ch.4–8)** — Beats 5–10: being read, myth
intrusion, forgery, midpoint pivot, the corrupting fork.

**Act 3: Resolution (Ch.9–12 + coda)** — Beats 11–14: confluence,
accountability, final chapter, four-voice reckoning.

**World-map anchoring:** Beats 1–4 anchor to the starting region
(commission). Beat 8 anchors to a ledger-threshold trigger anywhere on
the map. Beats 11–14 anchor to late-campaign regions/conditions. All
other beats bind to world-state triggers at Epic 12 spec time.

---

## Pacing and Flow

### Narrative Tempo

Slow burn — the chapter form is inherently cumulative; meaning accretes
across entries rather than exploding in setpieces.

### Tension Curve

Waves with rising amplitude — within each chapter, a three-part cycle:
the calm of writing → the loss of control in execution → the aftertaste
of the report. Amplitude increases chapter over chapter.

### Story Density

Text density is low during battle (watching words execute) and high in
post-battle reports and the ledger — narrative weight concentrates in
"writing after the fact." Main chapters are mandatory; forgery side-lines
and myth depth are optional.

### Key Moments

**Highest tension:** Beats 11–12 (Confluence / Judgment of the Brush)
**Emotional climax:** Beat 10 (the Corrupt Fork) or 13 (Final Chapter),
player-dependent
**Resolution beat:** Beat 14 (Four Voices)

## Characters

### Protagonist(s)

#### The Chronicler（幕府記室）

**Description:** The player's office, not a face — the staff chronicler of a
falling dynasty's last field army. Never named, never seen; exists only
as a signature on reports and a hand that writes doctrine.

**Background:** Inherited the post, not the war. Whatever he was before is
deliberately unwritten — the game never tells you.

**Motivation:** To leave a true record — or at least a record that can be
believed.

**Strengths:** Authorship — the power to write what others must execute.

**Flaws:** Distance — he never stands on the field; everything he knows is
fog, hearsay, and his own handwriting.

**Conflicts:**

- Internal: is he recording the war, or writing the war he wished for?
- External: rivals read his doctrines; forgers write in his margins.

#### The Commander（統帥）

The diegetic protagonist — the general whose war the chronicle records.
Chosen at campaign start: a predefined named general carrying authored
priors, or the player's self-created commander (`potato.character/1` —
name, origin, personality priors, starting doctrine-deck seed).

**Function:** the hand on the map — marches the warband, chooses where
the 回目 happen. The Chronicler remains the voice; the Commander is
whose deeds get written.

**Relationship to the Chronicler:** the Commander never speaks in the
record; the Chronicler never stands on the field. Authorship stays with
the player — the Commander is the hand on the map, the Chronicler the
hand on the page.

---

### Antagonist(s)

#### The Rival Generals（敵將群）

Four archetypes, never met — known only through hearsay-mood dossiers
and the doctrine behavior they field.

**1. The Cautious（謹慎型）** — a master of not-fighting; fights the player
with the player's own philosophy. *Defection path:* can be persuaded by a
sustained governance record （民心 as argument).

**2. The Cruel（殘酷型）** — efficient, corrupted; a mirror who walked the
墮 road the player might take. No defection path — he is the warning.

**3. The Fox（狐疑型）** — deception and intelligence warfare incarnate;
the QuantumFog made personal. His dossier is the least reliable.

**4. The Nemesis（宿敵型）** — the rival who learns the player best;
grows across the campaign. *Defection path:* in the late movement he can
be induced to lay down arms — the highest payoff of the subversion pillar.

**Sympathetic elements:** every dossier hints each general is also
writing someone else's history.

---

### Supporting Characters

#### The Scribe（書吏）

**Role:** Keeper of the ledger; the only recurring voice beside the
player's own.

**Personality:** Precise, faintly sardonic; writes small marginal notes
（頁邊批註） that annotate, question, and occasionally contradict the main
entries.

**Function:** The ledger's human voice — and the living carrier of the
forgery theme: some marginalia may not be his.

**Key Moments:** Beats 7 (first forged trace — he denies writing it) and
12 (Judgment of the Brush — he testifies, or doesn't).

#### The Roster（具名小隊）

**Role:** Emergent protagonists — generated names, player-grown bonds.

**Function:** Their arcs are carried by numbers: chapters survived,
merits, scars, desertion or steadfastness. The HistorianReport makes them
legible as people.

---

## Character Arcs

### The Chronicler Arc

**Starting State:** A clerk with a brush and a blank ledger.

**Transformation Moments:**

- First Loss (beat 3) — learns ink has weight
- Forgery (beat 7) — learns the page can lie without him
- Judgment of the Brush (beat 12) — must answer for the writing

**Ending State:** One of four voices — which one is the ledger's
verdict, not the player's claim.

**Lessons Learned:** Authorship is not innocence; every ellipsis is a
choice.

### The Nemesis Arc（宿敵）

**Starting State:** An opposing signature.

**Transformation Moments:** Being Read (5) — he starts reading back;
Confluence (11) — his account and the player's finally disagree openly.

**Ending State:** Defeated, defected, or the last hand still writing on
the other side.

**Lessons Learned:** He was never the enemy of truth — only its other
author.

## World Building

### World Overview

**Setting:** The last years of a dynasty — late-imperial China in form
but unnamed in text; the world is only ever called by place-names and
reign-titles inside the fiction, never by ours.

**World Type:** Dual-layer historical fantasy — a historical layer
(fords, supply lines, telegraph wires) and a mythological layer (earth
gods, foxes, the vengeful dead) occupying the same map.

**World Rules:**

- Both layers are real, but they are *believed differently*: the
  historical layer is what clerks record; the myth layer is what people
  believe. Mechanically, myth actions amplify the 民心 axis.
- Every location exists twice: once as terrain, once as a shrine.
- Writing is causality: an order written is a thing done — the premise's
  literalization.

**Atmosphere:** A world being described while it disappears — archives
yellow faster than the war ends.

**Unique Elements:** There is no "real" version of any event; only the
historical account and the mythic account, which disagree.

---

### History and Backstory

**Timeline Overview:** A long dynasty past its mandate; the game covers
its final campaign season, chapter by chapter.

**Major Events:** Never enumerated — the world's past arrives only as
dossier hearsay, marginal notes, and what the myth layer remembers.
Canon is intentionally assembled from contradictory accounts.

**Legends and Myths:** Local earth gods hold jurisdiction over specific
terrain; foxes broker information; the unburied dead of old battles
still march certain roads at certain hours.

**Hidden Secrets:** The forgery layer — someone has been writing in the
ledger who shouldn't be. Whether this is a person, an institution, or
the myth layer itself is a question the campaign leaves open until the
Judgment of the Brush.

---

### Factions and Organizations

#### The Court（朝廷）

**Purpose:** The institution the player serves; distant, decaying,
legible only through the commissions it sends.

**Story Role:** Source of chapter mandates; its demands increasingly
conflict with what the ledger shows.

#### The Rival Camp（敵營）

**Purpose:** The opposing military apparatus — a mirror bureaucracy with
its own chroniclers.

**Story Role:** Its four generals field doctrines; its own ledger is
never shown but implied — the player wonders who writes their reports.

#### The Believers（民間）

**Purpose:** Not an organization but the populace as a faction — the
collective holder of 民心 and the myth layer's constituency.

**Story Role:** They are the only faction that can "win" a chapter
without a battle.

---

### Key Locations

#### The Ledger（帳本）

**Description:** The campaign's true location — not a place but a book.
Everywhere else is reported from here.

**Narrative significance:** The only space where all three factions'
versions coexist.

**Atmosphere:** Margins crowded with the Scribe's small handwriting.

#### The Field（戰場）— per chapter

**Description:** Each chapter's map, read twice: terrain to the eye,
jurisdiction to the gods.

**Key events:** Doctrine execution, certainty collapse, the moment a
written plan meets fog.

#### The Refit Camp（整補營）

**Description:** Between-chapter space where names are read aloud,
wounds counted, doctrine rewritten.

**Narrative significance:** The only place the player "meets" the
roster — through report, not presence.

#### The Shrine Layer（神祠層）

**Description:** The myth map under every battle map — same geography,
different owners.

**Key events:** Myth Knocks, Confluence; where GodStance lives.

#### The Archive Seal（史館印鑑）

**Description:** The mint layer in-fiction — replay-verified victories
receive a seal; seals may be struck into keepsake plates (藏書票) of the
chronicle's documents: frontispieces, materia-medica entries, general
dossiers.

**Narrative significance:** The only part of the chronicle that claims
to be witnessed outside its own pages.

## Dialogue Framework

### Dialogue Style

**Overall Voice:** There is no spoken dialogue — the game's entire text
is written correspondence between document types. Voices differ by
document genre, not by speech.

**Style Elements:**

- Formality: Formal, period-register — 史官體 throughout
- Period: Archaic-official; no modern idiom
- Verbosity: Terse — elliptical counts, omissions as style
- Humor: Dry — only the Scribe's marginalia may be sardonic
- Profanity: None

**Character Voice Distinctions (by document type):**

| Voice | Register | Tell |
|---|---|---|
| HistorianReport | Impersonal, third-person, omits numbers | 「斬獲甚多」 |
| Court commission | Imperative, formulaic | reign-title headers, mandates |
| Scribe marginalia | First-person, small, questioning | 「此數可疑」 |
| GeneralDossier | Hearsay mood | 「據聞」「或云」 |
| MythLog | Folk register — the people talking | rumors, not records |
| Enemy doctrines | Never voiced — only observed behavior | silence as voice |

---

### Key Conversations（文件往來式）

#### The Mandate Exchange（朝廷來文 ↔ 覆文）

**Participants:** The Court → The Chronicler
**When:** Chapter opening
**Topic:** What the court demands this chapter
**Purpose:** Establishes chapter stakes; later chapters' mandates begin
contradicting the ledger
**Tone:** Increasingly estranged

#### The Marginalia Dialogue（書吏 ↔ 記室）

**Participants:** The Scribe ↔ The Chronicler
**When:** Woven through the ledger, post-battle
**Topic:** The Scribe annotates, doubts, corrects the official entries
**Purpose:** The game's only intimacy; the forgery theme lives here
**Tone:** Trust fraying in small handwriting

#### The Unreplied（敵營）

**Participants:** Rival generals → no one
**When:** Dossier updates, post-chapter
**Topic:** What rivals are rumored to have said about the player
**Purpose:** Being Read, made personal — the player is *written about*
**Tone:** Hearsay about yourself

---

### Branching Dialogue System

**System:** No dialogue trees — branching is documentary, not
conversational.

**Branch Triggers:** Ledger state and GodStance select which *document
variants* render (a report written after a massacre reads differently
than after a surrender; the Scribe's marginalia shifts with suspicion).

**Branch Scope:**

- Variant text pools per document type — bounded, compositional
- Convergence: all branches feed the same four-voice ending
- Unique content: variants are modular clauses, not full rewrites

**Consequence System:** The words the game writes about you are the
consequence — a forged victory is still a victory until someone audits.

## Environmental Storytelling

### Visual Storytelling

**Set Dressing:**

- The battlefield is read, not seen — fog renders as *uncertainty*, not
  darkness: low-certainty regions visually blur/grain, clouds drift as
  probability rather than concealment.
- Aftermath persists: a village burned in Ch.3 stays burned on the map
  and in the ledger — terrain remembers what text records.

**Environmental Details:**

- The shrine layer co-renders: same road, but spirit-lanterns mark
  jurisdictions; the same ford is a chokepoint above and a crossing-of-
  the-dead below.
- Pixel-art units acting out *written* orders — the gap between the
  player's neat doctrine and the messy field is the visual thesis.

**Visual Symbolism:**

- The brush and the page recur as UI framing — every screen is a
  document; even the map is a page someone drew.
- Marginalia: the Scribe's small notes literally occupy the page edges.

**Color and Lighting:**

- Historical layer: sepia/ink-wash album-leaf palette.
- Myth layer: the same palette, inverted accents — lantern-red,
  underworld-green highlights on identical geometry.

---

### Audio Storytelling

**Ambient Design:**

- Page-sonics as the ground layer: paper, brush, binding. The ledger
  sounds like a book; the field sounds distant — heard through the
  report, not the ear.

**Music Integration:**

- (Open item OQ-1 — direction deferred to Epic F.) Narrative intent:
  restraint; music appears only at document thresholds — chapter
  openings, the audit, the four voices.

**Voice Elements:**

- No voice acting. The myth layer may carry non-verbal sonic texture
  (bells, fox-cry) as its only "voice."

**Sound Design Narrative:**

- The sound of writing under stress: when the plan collapses, the
  player's remaining agency is literally a pen sound.

---

### Found Documents

**Approach:** The game *is* found documents — but structured discovery
still applies:

**Document Types:**

- NarrativePack inserts — chapter-bound text bundles unlocked by ledger
  state
- Marginalia — the Scribe's notes, some authentic, some not
- Dossier fragments — rival-generalia recovered post-chapter
- MythLog entries — folk accounts of the same battle the report records

**Quantity:** Bounded per chapter (a handful per 回） — density in
rereading, not collecting.

**Content Focus:** Contradiction — every found document offers the same
event from another layer or hand; the reward is discrepancy, not lore.

**Discovery:**

- Required: chapter commissions and reports
- Optional: forgery side-lines, myth depth, dossier hearsay

**Rewards:** Certainty — documents adjust what the fog lets you see;
a dossier fragment is a prior update, not a collectible.

## Narrative Delivery

### Cutscenes

**Quantity:** 0 — no rendered cutscenes.
**Style:** Document pages replace them: a chapter opens on a commission
page (court mandate), closes on a report page (HistorianReport). The
"cinematic" is a page turning.
**Skippable:** Pages are readable at player pace; no forced timing.
**Interactive Elements:** The report page accepts the player's signature —
the single interactive ritual of the fiction.

**Major "cutscene-equivalents":**

- Chapter commission page (each 回 opening)
- HistorianReport page (each 回 closing)
- The audit spread (Judgment of the Brush)
- The four-voice ending folios

---

### In-Game Storytelling

**Primary Methods:**

- Doctrine writing is the narrative act — the story is what the player
  writes, then watches execute
- Document streams post-battle: report, marginalia, dossier updates,
  MythLog entries
- The field itself: fog certainty, persistence scars, shrine layer —
  story read from terrain

**Show vs. Tell Balance:** Radical inversion — the game *tells* (text)
what it cannot show (fog), and *shows* (pixel field) what it cannot
guarantee is true.

**Interruption Approach:** Zero interruptions mid-execution — doctrine
phase is player-paced; execution is uninterrupted observation.

**Player Control:** All text is page-based and dismissible; nothing
auto-scrolls.

---

### Optional Content

**Forgery side-lines:** Chasing forged entries — optional audit trails
that change who the Judgment of the Brush names.

**Myth depth:** Shrine-layer engagement beyond mechanical need —
GodStance can be deepened into narrative payoff.

**Dossier hearsay:** Optional rival-generalia; reading them updates
priors (narrative reward = mechanical intel).

**Secret variant:** A fifth implied voice — what the ledger would say
if every entry were true — is hinted at but never rendered.

---

### Ending Structure

**Number of Endings:** 4 voices + variants

**Ending Triggers:** Final ledger state — the fold of accumulated
entries （民心/秩序/墮落/integrity), not a final choice. There is no
"pick your ending" prompt; the book closes and tells you what you wrote.

**Ending Variety:** Voice-level difference — each voice is a different
relationship to truth (the chronicle believed, the chronicle doubted,
the chronicle forged, the chronicle abandoned). Exact ledger conditions
open: GDD OQ-2.

**True Ending:** Deliberately none — the four voices are verdicts, not
ranks. The unwritten fifth is the only "true" ending, and it is
unreachable by design.

**Replayability:** A second campaign reads differently once you know
the ledger is a narrator — replay value is interpretive, not
unlock-based.

## Gameplay Integration

### Narrative-Gameplay Connection

**Integration Approach:**

- No separation exists: the core verb (writing doctrine) IS the
  narrative premise (you are a chronicler who writes war). Mechanics
  don't carry the story — they *commit* it.
- The ledger makes every action double-entry: gameplay state and
  narrative record are the same data viewed twice.

**Mechanic-Theme Alignment:**

| Mechanic | Theme it embodies |
|---|---|
| Doctrine interpreter | Authorship — you write, others die |
| QuantumFog | Writing vs. truth — you command what you cannot see |
| Ledger (double-entry) | Every deed has two accounts — the tactical and the moral |
| 墮落 ratchet | Efficient corruption leaves ink you cannot erase |
| Hearsay dossiers | Everyone else is also a text |
| Four-voice ending | You are judged by what accumulated, not what you intended |

**Story-Gameplay Balance:** Battle = gameplay-dense, text-sparse;
post-battle = text-dense, consequence-dense. The alternation IS the
rhythm (per Pacing section).

**Ludonarrative Considerations:** The classic dissonance is inverted
and exploited — the game about writing truth is a game where the truth
is unverifiable. That's not a flaw; it's the thesis.

---

### Story Gating

**Gating Approach:** Scenarios are gated by world-state predicates —
region control, POI resolution, ledger thresholds; the prerequisite
graph replaces the hard chapter sequence, and mandatory beats anchor to
fixed places or ledger states. Inside scenarios, narrative content is
soft-gated by ledger state.

**Story-Locked Elements:**

- Chapter N+1 unlocks only on chapter N resolution (military OR
  governance path — defeat is not a gate, it's a detour)
- NarrativePack inserts unlock by ledger thresholds
- Rival defection paths unlock by sustained governance record

**"Cutscene" Triggers:** Chapter open/close pages, the audit spread,
ending folios — all page-based.

**Mandatory Story Beats:** Beats 1–4 (tutorialized), 8 (midpoint pivot),
13–14 (final chapter + four voices).

**Optional Narrative:** Forgery chasing, deep GodStance, dossier
completion, MythLog depth.

---

### Player Agency

**Agency Level:** Meaningful choices, dynamically authored — the player
writes, the ledger remembers, the ending judges.

**Player Influence:**

- Tactical: doctrine composition per chapter (full authorship)
- Strategic: military vs. governance resolution per chapter
- Moral: corrupt options always available, ratchet never forgives
- Narrative: the player cannot choose the ending — they can only choose
  what the ledger contains

**Choice System:**

- Choice types: compositional (doctrine), consequential (atrocity
  forks), interpretive (which layer to believe)
- Consequence scope: ledger-level — everything posts; nothing is
  flavor-only
- Timing: doctrine-phase authoring, mid-execution CP interventions,
  post-battle report signature

**Role-Playing Freedom:** The Chronicler has no dialogue choices — the
role-play IS the doctrine style and the report signature. What kind of
chronicler you are is expressed in what you write and what you omit.

## Production Planning

### Writing Scope

**Estimated Word Count:** ~40,000–60,000 words equivalent (Heavy, but
compositional — most text is assembled from clause pools, not authored
as full branches)

**Content Breakdown:**

- Main story: ~15k — chapter commissions, reports, beat-specific text
  (14 beats × 8–12 chapters), ending folios ×4
- Side content: ~10k — forgery lines, dossier arcs, defection paths
- Document systems: ~15k — HistorianReport clause pools, marginalia
  pools, MythLog register, GodStance variants
- UI/system text: ~5k — chapter conventions, ledger labels, card text

**Scene Count:** 8–12 chapters × (commission + report) + audit spread +
4 ending folios

**Dialogue Lines:** N/A — no spoken lines; ~200–400 document-fragment
units in the correspondence voices

**Branching Complexity:** Clause-level, not path-level — variants
multiply linearly with ledger states, not exponentially with choices.

---

### Localization

**Target Languages:** 繁體中文 primary (the fiction *is* Chinese
historiography — 史官體， 回目， reign-titles); English secondary.

**Cultural Adaptation Notes:** English version is a translation of a
genre convention, not just language — elliptical counts and hearsay
mood need adaptation strategy, not literal translation.

**Technical Considerations:**

- Text expansion: CJK→EN typically expands ~1.5–2× — page layouts must
  tolerate it
- UI flexibility: ledger/page metaphors are culturally universal
  enough to survive
- Audio approach: N/A (no voice)

---

### Voice Acting

**Approach:** Text only — no voice acting.

**Rationale:** The fiction is *written* — a voice would break the
document frame. The myth layer's non-verbal sonic texture (bells,
fox-cry) covers the only place voice could exist.

## Appendix: Character Relationships

### Relationship Map

`
                THE COURT（朝廷）
                      │
                （發令 / 疏離）
                      ▼
    RIVAL GENERALS ──（被讀 / 反制）──► THE CHRONICLER
    （敵將群四型）        ◄───────────   （幕府記室）
       │                （書寫 / 署名）      │
       │                                    │
       └──（鏡像 / 可能歸降）                 ▼
                                  THE SCRIBE（書吏）
                                    （批註 / 保管帳本）
                                          │
                              （帳本連結一切）
                                          │
              ┌───────────────────────────┼────────────┐
              ▼                           ▼            ▼
        THE ROSTER              THE BELIEVERS    MYTH ENTITIES
        （具名小隊）             （民心持有者）    （神祠層）
`

### Relationship Key

- The Chronicler → The Roster: writes their fate; they execute it
- Rival Generals → The Chronicler: read and counter his writing; two
  may defect
- The Nemesis ↔ The Chronicler: mutual authorship — each is the other's
  most faithful reader
- The Scribe → The Ledger: annotates and (possibly) forges; the only
  hand besides the player's
- The Court → The Chronicler: mandates; distance grows as the ledger
  diverges from the throne's account
- The Believers → GodStance: belief determines the gods' posture
- Myth Entities → Ledger: remembered differently than recorded

---

## Appendix: Story Timeline

### Chronological Events

`
[受命 Commission]
     │
     ▼
[開幕動 Ch.1–3]
  ├── 首書 First Writing
  ├── 初殤 First Loss
  └── 治平初試 First Governance
     │
     ▼
[中盤動 Ch.4–8]
  ├── 被讀 Being Read
  ├── 神話叩門 Myth Knocks
  ├── 偽帳 Forgery
  ├── ◆ 中點：大敗或大治
  ├── 敵將判詞 The Dossier Lies
  └── 墮落歧路 The Corrupt Fork
     │
     ▼
[末盤動 Ch.9–12]
  ├── 雙層合流 Confluence
  ├── 筆之審判 Judgment of the Brush
  └── 最後一回 The Final Chapter
     │
     ▼
[四聲部 Four Voices — 帳本終態判決]
`

### Timeline Notes

Chapter sequence is fixed; beat 8's nature (rout vs. rule) is
player-dependent; forgery side-lines run parallel to Ch.4–12.

---

## Appendix: References and Inspirations

- **李時珍《本草綱目》** — the governing reference: a Ming compendium
  that is itself an act of authorship — an editor deciding what enters
  the record, weighing empirical test against inherited hearsay. Models
  the Chronicler's work: the world made legible as a book, where
  cataloging is judgment and every entry is a verdict on what deserves
  to be believed. Informs the ledger's taxonomy, the dossier form, and
  the fiction's confidence that a book can hold a world.
