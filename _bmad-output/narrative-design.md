---
title: 'Narrative Design Document'
project: 'MingGoRTS'
date: '2026-09-17'
author: 'potat'
version: '1.0'
stepsCompleted: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]
status: 'complete'
narrativeComplexity: 'Heavy'
gdd: '_bmad-output/planning-artifacts/gdds/gdd-MingGoRTS-2026-09-17/gdd.md'
---

# Narrative Design Document

## MingGoRTS

**Narrative Complexity:** Heavy

---

## Story Foundation

### Narrative Premise

The player is the **軍師史官** — a counselor who walks between two layers
of the same China and writes the ledger with their own hand. The history
layer is marching columns, grain convoys and telegraph lines; the myth
layer is local gods, fox spirits, war-god possessions and restless dead.
Serving commander after commander across the Republican era, they write
battle doctrines by night and record the cost by morning — the roster is
their handwriting, and their own name is in it. The gods remember what
the ledger records. Every victory is audited — not by the flag planted,
but by who paid for it.

**The unifying question: 勝利要付誰的代價？— Who pays for victory?**

**Design axiom:** every narrative mechanism hangs on *the ledger and the
pen* — not on the counselor as a person. The counselor is the skin; the
ledger is the bone. Future protagonists can change; the bookkeeping
cannot.

### Frame

The story opens with the historian mid-sentence in the final chapter —
**brush lifted, not finished**. He hesitates because he refuses to write
the era's verdict until it is earned — he is waiting for an ending worth
the ink, and the player is the one who must find it. Each chapter is a
biography recalled to the page; the last page is generated on the spot
from the accumulated record. The frame is "writing, interrupted" — never
"remembering what is already settled" — so branching outcomes never make
the frame a liar.

### Core Themes

1. **勝利≠秩序 Victory Is Not Order** — winning the field and winning
   the mandate are different wars.
2. **不戰之力 The Strength of Not Fighting** — mercy as strategic force,
   but *judgment*, not doctrine: the spared may break faith, and in the
   War of Resistance the unsubduable invader makes "peace" into
   complicity. Yet even there, a ruinously costly no-battle option
   remains (yield a province, preserve the army) — judgment is real only
   when both paths hurt.
3. **名冊與記憶 The Roster and the Remembering** — the ledger counts the
   named dead on both sides *and* the civilian cost; atrocity is always a
   deliberate order, never a misclick. The counselor's accumulated record
   becomes his name — "the counselor of Broken Bridge."
4. **人神互寫 History Writes Myth, Myth Writes History** — the gods
   remember the counselor's record: aided at high 民心, feared at high
   墮落 — and on the Fallen path, darker spirits answer instead.
   The historian records the myth layer only as "據報" — 演義筆法:
   reported, never confirmed. One signature event is witnessed by
   thousands yet still entered only as "據報" — the ledger that cannot
   afford to believe.

### The Opposition

The recurring foil is a **rival counselor who serves the God of War** —
the annihilation philosophy given a face on both layers: a human enemy
whose deck you can read, and a divine patron who demands blood-tribute.
He reads you as you read him: each appearance, his deck fields a
counter-doctrine written against your most-used trigger type from the
previous battle — and the ledger *says so*: "彼之陣法，似針對我軍慣用."
He keeps a ledger of his own — and on the Fallen path, *your*
brush-strokes gradually come to read like *his*.

Once, before you have earned the right to hate him, he spares you — and
you spend the rest of the campaign owing your life to the man you oppose.
He is not incapable of mercy; he chooses not to choose it. That is what
makes him a mirror, not a monster.

### Tone and Atmosphere

**Tone:** 史詩 × 肅穆 × 神異 — epic sweep, the heavy price of war,
spectral wonder.

**Atmosphere:** muted mud-and-uniform history under saturated spectral
myth; the score is assembled live from fired doctrine cards; the
aftermath is written in brush-strokes.

**Emotional Register:** the three-beat loop — the command tent's
gravitas, the suspense of watching your script run, the mourning at the
roster — plus the fourth beat: the moment the brush stops over a name
too familiar to write.

## Story Structure

### Structure Type

**Interrupted-brush frame + episodic commander biographies + branching
outcomes** — 演義 chapter conventions adapted: each chapter opens with
the historian's題詞 (a one-line verse forecasting the chapter's
question), each enemy commander's entrance carries a **判詞** (a one-line
character judgment), and each chapter closes with the ledger's "欲知後事"
hook.

**判詞 is hearsay, not truth — and the hearsay leans low.** The
historian writes judgments from rumor, and rumor is a boasting animal:
commanders are systematically *underrated* in the ledger. The
underestimate is a fixed, learnable bias — observation's real value is
verifying whether the ledger got it right; scouting is fact-checking the
pen. High-cunning commanders can *plant* false判詞 into the rumor mill.

**The ledger is a lossy map.** Every after-action report marks how many
events it omitted ("本報告省略 N 項"). The player witnessed the real
battle and can audit the ledger against the replay — the gap between
record and replay is the historian's conscience. On the Fallen path the
omission count balloons: the ledger starts protecting its writer.

**The ledger leaks.** Your判詞 of a subdued commander is read by its
subject — and answered ("勇而無謀" may return in a later chapter carrying
proof). Your battle reports are studied by the rival; the deck he fields
next chapter is built from your own ink. The pen is double-edged: it
records, it reveals, and it provokes what it describes.

### The Ending — Written, Not Chosen

The final page is generated from the accumulated record. Four voices,
not five:

| Ending | Record | Voice |
|---|---|---|
| **霸業 Conquest** | military dominance, low 治績 | the historian's brush, in the register of annals of conquest |
| **仁政 Mandate** | high 民心/治績, limited conquest | the same brush — the unification-by-mandate variant of one hand |
| **無字 The Unwritten** | conquest AND 民心 both high — the ledgers contradict | *no text*: the historian cannot reconcile the two accounts; the page stays blank, and the era writes itself |
| **Fallen** | 墮落 100 | the rival's brush — your hand has already become his |

### Act Breakdown

| Arc | Era | Chapters | Narrative Function |
|---|---|---|---|
| I 軍閥割據 | Warlord Era | 1–4 | Learn to read enemy decks; first 無戰 options; the foil introduced — his patron glimpsed |
| II 北伐 | Northern Expedition | 5–8 | Coalition politics; betrayal & subversion — mercy can be punished; myth layer begins rewriting battlefields; the foil's deck starts countering yours; *he spares you once* |
| III 抗戰 | War of Resistance | 9–12 | Asymmetric war — the invader cannot be subdued; 無戰 here is complicity — yet a ruinously costly no-battle option remains; GovernedPeace is survival, not glory |
| IV 內戰 | Civil War | 13–15 | Former allies return as decks you already know; the foil returns for the last audit — and the God of War's final offer; the historian lays down the brush |

### Narrative System Requirements (for the GDD backlog)

- The replay event stream *is* the ledger's data source — the recording
  system and the writing system are one mechanism. Replay doubles as the
  audit tool against the ledger's omissions.
- Record **enemy commander dispositions** (subdued / slain / defected /
  retired) per chapter — the final chapter needs them.
- Record **named myth-layer events** (which shrine, which spirit, when)
  — the gods' testimony at the ending cites them.
- The historian's report is **modular**: event fragments + judgment
  templates, assembled — never fully authored prose. Reports state their
  omission count.
- Title/reputation track ("the counselor of Broken Bridge") evolves per
  chapter and is legible in the ledger's self-reference.
- 敵將判詞 is rumor-state, not truth-state — scouting verifies or
  falsifies it; high-cunning enemies can seed false判詞.
- Scope guard: the campaign frame and historian conventions are
  campaign-layer; the MVP (Broken Bridge) ships without them.

---

## Story Beats

### Major Story Beats

**Arc I 軍閥割據 (Ch 1–4)**

1. **神像流淚 The Weeping Shrine** (Inciting Incident) — no human
   employer calls the counselor first: a village shrine's statue weeps
   blood-tears / a fox spirit delivers a warning of coming war. The myth
   layer chooses the counselor before history does — the pen is picked
   up because the gods are afraid.
2. **斷橋初筆** — first battle (Broken Bridge / 格洛克); the first判詞
   is written; the first doctrine hand is fielded.
3. **殲滅的筆跡** — the rival's hand first seen in aftermath, not in
   person: an annihilated column, a ledger kept in kill-counts.
4. **初次無戰** — the first chapter offering a no-battle resolution;
   the 至聖者 path opens.

**Arc II 北伐 (Ch 5–8)**

5. **同盟誓約** — coalition forms; allied commanders whose decks you
   learn are tomorrow's enemies (dramatic irony planted).
6. **叛將之筆** — a subdued commander breaks faith; mercy punished;
   the ledger records the cost of trust.
7. **神話改寫** — first battle where a pacified or provoked myth event
   visibly rewrites the history-layer terrain.
8. **放生** — defeated and encircled, the rival spares you: "下次，我
   不再放." You owe your life to the man you oppose.
9. **戰神顯形** (Midpoint) — the God of War manifests openly; thousands
   witness. The ledger can only write "據報" — the veil between the
   layers ruptures, and the era's quiet gods begin choosing sides.

**Arc III 抗戰 (Ch 9–12)**

10. **不可收服之敵** — the invader arrives; doctrine cannot subdue them;
    無戰 here is complicity.
11. **無戰的代價** — the ruinously costly no-battle option (yield a
    province, preserve the army): judgment, not doctrine.
12. **無勝而勝試煉** — a chapter designed to be lost in the field while
    governance holds; the player learns to lose correctly.
13. **對手裂縫** — the rival's patron demands more blood; he begins to
    doubt what he serves.

**Arc IV 內戰 (Ch 13–15)**

14. **昔日戰友** — former allies return as enemy decks you already know
    how to read.
15. **最後審計** — the final audit against the rival: two pens over one
    map; his deck carries every counter he learned from your ink.
16. **直面戰神** (Climax) — the rival defeated, his patron descends to
    claim the field himself: the myth-layer final battle — fighting war
    itself, not a nation.
17. **擱筆** (Resolution) — the historian lays down the brush; the last
    page writes itself from the accumulated record — 霸業, 仁政, 無字,
    or the rival's hand.

---

## Pacing and Flow

### Narrative Tempo

**波浪漸強** — waves of rising pressure: each arc builds to one peak,
then breathes; battle chapters alternate with governance/narrative
chapters; Arc IV drops the breathing room and runs continuous pressure
to the climax.

### Tension Curve

Per-arc wave: 題詞開場（疑問）→ 部署（期待）→ 執行（懸念）→ 戰報
（帳目）→ 名冊（哀悼）→ 章末鉤子。The myth layer acts as a second,
slower wave underneath — quiet in Arc I, breaking the surface at the
midpoint, co-dominant by Arc IV.

### Story Density

- **Battle chapters**: tension peaks — doctrine stakes, readable
  enemies, outcome ladder.
- **治平 / narrative chapters**: breathing room with quiet tension —
  negotiation clocks, governance dilemmas, myth-layer encounters.
- **Aftermath (every battle)**: mandatory mourning beat — roster and
  report are never skippable flavor; they carry the ledger.

### Key Moments

**Highest tension:** 直面戰神 (Ch 15) — the war god claims the field.
**Emotional climax:** 放生 (Arc II) — owing your life to the man you
oppose; and the 無字 possibility — the historian who cannot write.
**Resolution beat:** 擱筆 — the brush comes down on a page the player
wrote without knowing it.

---

## Characters

### Protagonist

#### 軍師史官 (the Counselor-Chronicler)

**Description:** Nameless by design — the campaign's epithets accrue onto
him ("the counselor of Broken Bridge"). A returned military-academy
graduate: staff-college methods, map-and-telegraph rationalism.

**Background:** Trained abroad for a modern army that does not exist;
came home to warlords. Took a staff post as 記室 — picked up the pen as
a duty, and it became a verdict.

**Motivation:** To prove war can be conducted *rationally* — that
doctrine replaces heroics and the pen out-writes the sword.

**Strengths:** Doctrine composition, intelligence analysis, the audit
habit — he records because he was taught to measure.

**Flaws:** Does not believe in the gods who chose him; "據報" is his
truce with what he cannot verify; treats people as ledger entries until
the roster teaches him otherwise.

**Conflicts:**
- Internal: the rationalist who must write what he cannot verify —
  every "據報" is a small surrender.
- External: warlords who want victories, not governance; a rival who is
  everything he refuses to be.

### Antagonist

#### 聶懷仁 (the Rival — [name provisional])

**Description:** A decorated general who lost everything in a warlord
battle — and came back wrong. Dying on the field, the God of War entered
him; possession works because *he already believed*. Man and god share
one body: the two layers' antagonist in a single flesh.

**Background:** A purist before the possession — 兵者兇器，聖人不得已
而用之 — and since one uses it, one uses it *completely*. Betrayed and
abandoned by his own faction; the god did not corrupt his doctrine, it
ratified it.

**Goals:** To prove war done completely is more merciful than war done
halfway — total war ends wars.

**Methods:** Annihilation doctrine, a kill-count ledger, decks that
absorb the player's signature counters. Each battle he is less man,
more patron.

**Relationship to Protagonist:** The mirror — same craft (the pen),
opposite bookkeeping. He once spared you, before you earned the right
to hate him.

**Sympathetic Elements:** He is dying by inches — the god consumes him
a battle at a time. In Arc III the patron demands more blood than even
*he* believes war needs; his doubt is the last human thing in him.

### Supporting Characters

#### 狐仙線人 (the Fox-Spirit Informant)
**Role:** Myth-layer informant — the first voice that warned of war.
**Personality:** Trade-minded, ornate, never fully trustworthy; sells
truth in installments.
**Function:** The myth layer's speaking face; channels 判詞 rumor,
shrine gossip, and the gods' stance toward the counselor.
**Key Moments:** The opening warning (Ch 1); mid-campaign offer the
player should probably refuse; on the Fallen path, stops answering —
what answers instead is worse.

#### 倖存老兵 (the Veteran)
**Role:** Squad captain mustered in Chapter 1 who can survive to the
final chapter — the roster theme made flesh.
**Personality:** Professional soldier, unimpressed by academies;
measures officers by whether they read the names.
**Function:** The protagonist's human anchor; the governance score made
personal — he is what "low casualties" actually looks like.
**Key Moments:** Teaches the academy man the difference between a
column on a map and a column on a road; a late-campaign death, if the
player lets it happen, is designed to be the campaign's most expensive
single casualty.

#### 老住持 (the Abbot)
**Role:** Village abbot / local gentry — 民心 and 秩序 personified.
**Personality:** Slow-spoken, has outlasted three armies already; judges
soldiers by what they leave behind.
**Function:** Governance outcomes rendered visible — his reception of
the player's army is the campaign's trust barometer.
**Key Moments:** The shrine that weeps blood-tears is his (Ch 1); his
compound shelters civilians in Arc III; on the Fallen path his gate
stays shut.

#### 對手的副官 (the Rival's Adjutant)
**Role:** The rival's ledger-keeper — the other pen.
**Personality:** Precise, loyal to the man not the patron; keeps the
kill-count without endorsing it.
**Function:** The second ledger's witness; proof the rival was still
auditing himself.
**Key Moments:** Present at the sparing ("下次，我不再放"); after the
last audit, brings his master's book to the player — the rival's
epilogue is delivered by the only pen that outlived him.

---

## Character Arcs

### 軍師史官 Arc — Positive (or Transformation on the Fallen path)

**Starting State:** A rationalist who believes war is a solvable
problem; the ledger is a tool, the names are data.

**Transformation Moments:**
- The weeping shrine — the gods choose a nonbeliever (Ch 1).
- The spared defeat — owes his life to annihilation's champion (Arc II).
- The veteran's loyalty — people stop being entries (mid-campaign).
- 戰神顯形 — "據報" stops being evasion and becomes a kind of faith.

**Ending State:** The man who writes the era's verdict — on the true
path, a historian who earned the brush; on the Fallen path, his
handwriting has already become the rival's.

**Lessons Learned:** The ledger is moral, not technical. Who pays for
victory is the only question that audits the auditor.

### 聶懷仁 Arc — Negative

**Starting State:** A purist general who believes total war is mercy;
already possessed before we meet him — the flashback shows he chose it.

**Transformation Moments:**
- The sparing — his last unmotivated kindness.
- Arc III's doubt — the patron demands more blood than even he believes
  war needs.
- The last audit — his deck carries your counters; his ledger carries
  your name.

**Ending State:** Defeated — and the patron discards the husk to face
the player itself. What remains of the man is in the book his adjutant
carries to your door.

**Lessons Learned:** He chose completeness over mercy — and was
consumed by the thing he fed. A war done completely devours the
soldier too.

### 倖存老兵 Arc — Flat

**Starting State:** A professional soldier under yet another officer.
**Transformation Moments:** Learns to read his own name in the ledger —
and what it costs the pen to write it.
**Ending State:** Still standing — or his death is the roster's loudest
entry. Either way, the player finally understands what the names are.

---

## World Building

### World Overview

**Setting:** A semi-fictionalized Republican-era China — the era is
isomorphic (warlords → expedition → resistance → civil war) but all
place and personal names are invented. Players recognize the age, not
the map.

**World Type:** Historical × mythic dual-layer.

**World Rules:**
- Two layers, one land. History layer: railways, grain convoys, field
  telephones, mud. Myth layer: tutelary gods, fox spirits, possessions,
  restless dead — running underneath the same geography.
- **Gods are mirrors of the people's hearts.** Shrine incense feeds a
  god; scattered hearts starve one. The myth layer's strength literally
  indexes the campaign's 民心/秩序 — a well-governed province has
  strong, lucid gods; a ravaged one has hungry, erratic ones.
- The two layers have *separate maps*. Warlord borders and the gods'
  domains overlap but do not coincide — a town can be one faction's
  territory and another god's shrine-ground.
- The historian records the myth layer only as "據報" — reported,
  never confirmed.

**Atmosphere:** Mud-and-uniform melancholy over saturated spectral
dread; the sound of telegraph keys under temple bells.

**Unique Elements:** A world where belief is infrastructure — losing
the people doesn't just cost you legitimacy, it literally weakens your
gods. And a war that has gone on so long it is feeding something older
than any shrine.

---

### History and Backstory

**Timeline Overview:**
- ~A generation ago: the dynasty falls; the republic is declared and
  immediately fractures.
- Warlord era (Arc I): a dozen cliques hold the provinces; the gods
  fade as railways and telegraphs replace pilgrimage routes.
- Northern Expedition (Arc II): a coalition marches to unify; the
  alliance contains its own betrayal.
- War of Resistance (Arc III): an outside invader — the unsubduable
  enemy; hearts and gods alike are tested.
- Civil War (Arc IV): former allies turn; the war that has fed the old
  god for a generation reaches its reckoning.

**Major Events:** The dynasty's fall; the first railway through shrine
country; a forgotten warlord battle where a decorated general died —
and something came back wearing him.

**Legends and Myths:** Every village has its tutelary; fox spirits
trade in truth by the installment; the old people say that when a
statue weeps blood, a great dying is coming.

**Hidden Secrets:** The nameless War God is not new — every era of
slaughter has fed it. It does not possess its servants; it *ratifies*
them. And it is stronger now than at any time in living memory,
because a generation of unbroken war is a generation of unbroken
tribute.

---

### Factions and Organizations

#### 軍閥群雄 (the Warlord Cliques)
**Purpose:** Hold province, tax grain, survive the next alliance.
**Leadership:** Personal armies around charismatic or brutal
commanders — each chapter's commander is a clique's face.
**Goals:** Expansion within the era's rules: absorb, betray, submit,
wait.
**Methods:** Column warfare, railroad denial, hostage diplomacy.
**Relationships:** Fluid — today's ally is next chapter's deck.
**Story Role:** The history-layer field; the player's employers and
opponents across Arcs I–II.

#### 革命同盟 (the Coalition)
**Purpose:** Unify the country under one government — the Northern
Expedition's banner.
**Leadership:** A council of factions that agree on unification and
nothing else.
**Goals:** End the warlord era — by absorption or annihilation.
**Methods:** Massed offensives, political commissars, manifestos.
**Relationships:** Contains the seeds of Arc IV — the coalition's
internal split is the civil war's embryo.
**Story Role:** Arc II's superstructure; allies learned here return as
readable enemies.

#### 入侵者 (the Invader)
**Purpose:** Conquest by an outside power — organized, industrial,
unsubduable by doctrine.
**Leadership:** A general staff, not a personality — deliberately
faceless.
**Goals:** Territory and submission, at a scale local war can't match.
**Methods:** Attrition, terror, logistics — the war that makes 無戰
complicity.
**Relationships:** Enemy of every human faction; the myth layer is
divided on whether to resist or feed.
**Story Role:** Arc III's crucible; the enemy the ledger cannot write
a 判詞 for.

#### 內戰兩營 (the Two Camps)
**Purpose:** The coalition's heirs, split over what unification meant.
**Leadership:** Former brothers-in-arms — both sides carry Arc II
biographies.
**Goals:** Finish the era's argument.
**Methods:** Everything learned across three arcs, pointed inward.
**Relationships:** Mirror factions; the foil's last ground.
**Story Role:** Arc IV's field — the era's final audit.

#### 安分的神 (the Quiet Gods)
**Purpose:** The myth layer's old order — village tutelaries, river
gods, mountain spirits.
**Leadership:** None — a loose commons of shrine-deities, each bound to
its ground and its people.
**Goals:** Be fed; keep their people alive enough to feed them.
**Methods:** Omens, weather, small mercies, the occasional fox-spirit
errand.
**Relationships:** Weakened by unbelief; uneasy alliance with any
governance that keeps hearts whole — which makes them the player's
natural, if unspoken, allies.
**Story Role:** The myth layer's map beneath the war; their rise and
fading charts the campaign's 民心.

#### 戰神一系 (the War God's Ascendancy)
**Purpose:** The nameless old god and what it gathers — war-possessed
soldiers, battlefield ghosts, shrines converted to blood-tribute.
**Leadership:** The God of War itself, through vessels — the Rival is
its greatest incarnation.
**Goals:** Not victory — perpetuation. It does not want the war won; it
wants the war.
**Methods:** Possession, blood-tribute, the ratification of cruelty —
it does not corrupt, it *agrees*.
**Relationships:** Enemy of the quiet gods by ecology — it eats the
hearts they need. On the Fallen path, the player's own ledger feeds it.
**Story Role:** The true antagonist; Arc IV's climactic field.

---

### Key Locations

#### 斷橋 Broken Bridge
**Description:** A half-ruined stone bridge over a ford — the only
crossing for miles; rebuilt and re-broken across the chapters.
**Narrative Significance:** The first battlefield; the tutorial's
ground; where the counselor earns his name. Rebuilt versions recur as
the era's scar tissue.
**Atmosphere:** Fog over shallow water; stone that remembers feet.
**Key Events:** The first doctrine hand (Ch 2); the myth-rewrite battle
that changes its terrain (Arc II).
**Inhabitants:** Contested ground — whoever the chapter sends.

#### 流淚村廟 (the Weeping Shrine)
**Description:** A small village shrine to a forgotten tutelary — the
Abbot's charge; its statue weeps blood-tears before the war arrives.
**Narrative Significance:** Where the myth layer chooses the
counselor; the campaign's 民心 barometer has a physical address.
**Atmosphere:** Incense and dread; quiet that is about to end.
**Key Events:** The inciting incident (Ch 1); a refugee shelter in
Arc III; a shut gate on the Fallen path.
**Inhabitants:** The Abbot; the fox spirit when she chooses to be seen.

#### 軍電室 (the War Room)
**Description:** Wherever the army's map table and telegraph key are
set up — the planning layer's physical space; a different tent, the
same candle, every chapter.
**Narrative Significance:** The pen's home ground — doctrines are
written here, and the ledger too.
**Atmosphere:** Lamp-light, pinned maps, the four-beat stillness before
execution.
**Key Events:** Every planning phase; the sparing's aftermath; the
final brush-stroke.
**Inhabitants:** The counselor; the veteran reporting in.

#### 戰場祭壇 (the Field Altar)
**Description:** Not a place that exists on the history map — the war
god's ground, which appears where slaughter is thickest: an altar of
spent casings and broken weapons.
**Narrative Significance:** The ascendancy's shrine-network; where
blood-tribute is collected; the site of the last audit.
**Atmosphere:** The myth layer without mercy — saturated, wrong,
beautiful.
**Key Events:** The Rival's communion scenes; 直面戰神 (Ch 15).
**Inhabitants:** The possessed; the tribute.

---

## Dialogue Framework

### Dialogue Style

**Overall Voice:** Three pens, three registers — the ledger's
semi-classical historian voice; human speech in accessible modern
vernacular salted with era words (軍座/恁地/據報); and the fox
spirit's ornate trader's tongue — the gods' only interpreter, whose
translations may always be her own words.

**Style Elements:**
- Formality: Layered — ledger formal, speech mid-register, fox
  elaborate
- Period: Era-appropriate flavor over modern-readable syntax
- Verbosity: Concise — scenes are short, every line load-bearing
- Humor: Rare, dry — the fox's wit and the veteran's gallows humor are
  the only permitted lightness
- Profanity: Mild — soldier's oaths, never modern slang

**Character Voice Distinctions:**
- **軍師史官** — almost never speaks aloud; his voice *is* the ledger.
  In scenes: terse staff-college phrasing, questions not opinions.
- **倖存老兵** — coarse soldier-speech, era slang; measures everyone
  by whether they read the names.
- **老住持** — slow, aphoristic; a man who has outlasted armies and
  speaks like it.
- **狐仙線人** — ornate, transactional, playful; sells truth in
  installments. She is the gods' only mouth — and anything she
  translates might be hers.
- **敵將** — each speaks in the shadow of his判詞; dialogue should
  confirm *or betray* the rumor — a commander judged 勇而無謀 who
  speaks carefully is a scouting failure made audible.
- **聶懷仁** — when the patron rises, two voices layer in one mouth:
  the man's clipped military phrasing and something older speaking
  *through* the grammar. The ledger records it only as 據報.
- **對手的副官** — precise, minimal; reads the kill-count like
  weather.
- **戰神** — never speaks. Even at 直面戰神 the field is wordless —
  or the fox is not there to translate, which is the same thing.

---

### Key Conversations

#### 狐仙的警告 (Ch 1 — the first voice)
**Participants:** The counselor, the fox spirit
**When:** The inciting incident — before any human employer calls
**Topic:** A warning of coming war, delivered as a trade
**Purpose:** Opens the myth layer's mouth; establishes that gods speak
through intermediaries and intermediaries have agendas
**Tone:** Playful dread — she is enjoying this

#### 軍議 (recurring — the war room)
**Participants:** The counselor, the veteran, chapter commanders
**When:** Planning phases
**Topic:** The coming battle — terrain, the enemy's判詞, what the
rumor says versus what scouting proved
**Purpose:** Delivers判詞-as-dialogue; lets the veteran ground the
academy man; the player's deck-building is narrated as counsel
**Tone:** Lamp-lit, professional, occasionally grim humor

#### 勸降 (per-chapter — the 無戰 path's spoken face)
**Participants:** The counselor (or envoy), the enemy commander
**When:** When doctrine or negotiation events open a no-battle
resolution
**Topic:** Terms — why laying down arms serves the commander better
than the ledger does
**Purpose:** The 至聖者 philosophy spoken aloud; success or failure
written into the record
**Tone:** Chess-match civility over open knives

#### 放生 (Arc II — the debt)
**Participants:** The rival, the defeated counselor
**When:** After the player's encircled defeat
**Topic:** "下次，我不再放" — mercy as a one-time coupon
**Purpose:** The player owes a life to annihilation's champion; the
mirror cracks its first light through
**Tone:** Flat, almost bored — which makes it unbearable

#### 對手的軍帳 (Arc III — the doubt)
**Participants:** The rival, his adjutant
**When:** Mid-Arc III — witnessed or reported
**Topic:** The patron demands more blood than the purist believes war
needs
**Purpose:** Shows the second ledger and the man still auditing
himself; the foil's arc turns
**Tone:** Quiet horror in military language

#### 最後審計 (Arc IV — two pens)
**Participants:** The counselor, the rival — the only true
conversation the mirrors ever have
**When:** Before or after the final battle against him
**Topic:** Two bookkeeping methods arguing over one map
**Purpose:** The themes spoken once, plainly, between the only two
people who keep ledgers
**Tone:** Exhausted respect between enemies

---

### Branching Dialogue System

**System Type:** Event-option dialogues — discrete scenes (勸降 /
審判 / 神明提案 / 審問) presenting 2–3 options.

**Branch Triggers:** Options gated by campaign state — 民心/秩序
thresholds open conciliatory lines; verified判詞 (scouted, not
rumored) unlocks leverage lines; 墮落 gates or unlocks coercive
lines. The fox's offers appear only when her price is payable.

**Branch Scope:**
- Total branches: per-event 2–3 options, outcomes fold back into the
  ledger — events are islands, not trees
- Convergence: always converge — every path returns to the battle
  loop; the *record* of the choice persists, not the branch
- Unique content: modest — options vary a scene's ending paragraph and
  its ledger entry, not whole conversations

**Consequence System:** Every option writes a different ledger line —
and some write things the player didn't choose ("本報告省略 N 項"
grows). Subdued commanders read their own判詞 later; the pen's
wording is a consequence delayed by chapters.

---

## Environmental Storytelling

### Visual Storytelling

**Set Dressing:** Two-layer scarring — the land remembers on both
strata. History-layer scars persist physically: burnt groves, mass
graves, a bridge pier rebuilt in mismatched stone. Myth-layer scars
persist spiritually: where the dead lie thickest the quiet gods detour,
ghost-columns march at night, shrine offerings go untouched. Returning
to old ground tells the campaign's story for free — and each layer
writes the scar differently.

**Environmental Details:**
- The Weeping Shrine's offering table tracks 民心 — full bowls in
  well-governed ground, dust where order failed
- The rival's battlefields are *too* clean: annihilation leaves no
  prisoners' camps, no refugees — absence as evidence
- Rebuilt versions of Broken Bridge recur across chapters — the era's
  scar tissue, each rebuild a different stone

**Visual Symbolism:** The brush and the ledger appear physically —
the war room's candle, inkstone, and stacked report-papers are the
game's recurring still-life. On the Fallen path the player's own war
room slowly rearranges toward the rival's layout; the player's brush
in the idle frame gradually resembles his.

**Color and Lighting:** History layer muted (mud, uniform, paper);
myth layer saturated (shrine-red, spirit-glow). Progressive seepage:
approaching myth events first corrupt the local frame — fog color
wrong, shadows wrong, objects minutely displaced — before any full
manifestation theme-swap. Horror arrives as "something is off"
before it arrives as a god.

---

### Audio Storytelling

**Ambient Design:** Dual-layer ambience — history: wind, insects,
distant artillery, telegraph hum; myth: temple bells, sutra-chant,
sounds moving *against* the wind. During seepage, the myth soundscape
arrives *before* the visuals — players hear the god before they see
the wrong fog.

**Music Integration:** The doctrine-assembled score remains — fired
cards compose the battle's music. On the Fallen path the same card
triggers detune toward the rival's theme intervals.

**Voice Elements:**
- **誦名** — as the roster's names type out, a faint recitation or
  temple bell runs underneath; more dead, longer sound. Silence after
  the last name is also information.
- **電報聲** — the war room's telegraph key taps *actual* morse:
  players who read it get real intelligence early; everyone else gets
  atmosphere. When the line goes dead, the silence tells you
  communications are cut.

**Sound Design Narrative:** **The rival's drums** — the Ascendancy
carries war-drums as positional audio, not score: drum near, the Rival
near. Late on the Fallen path, the player's own camp starts to drum.

---

### Found Documents

**Document Types:**
- **敵軍家書/遺物** — letters and charms taken from the enemy dead;
  the roster theme mirrored onto the other side's names
- **截獲電報** — intercepted enemy wire traffic; reveals a
  commander's *true* intent — the fact-check counterpart to hearsay
  判詞
- **對手的帳頁** — loose pages of the rival's ledger recovered from
  defeated Ascendancy troops; the player reads how the mirror's pen
  writes *them* ("彼之陣法似針對我軍慣用" in his hand)
- **廟裡的籤詩** — fortune slips drawn at village shrines; oracle-grade
  intelligence in 據報 ambiguity — always true, never clear

**Quantity:** Modest per chapter — a handful of documents each, never
a collect-a-thon; quality targets the "that one letter" memory.

**Content Focus:** Humanity of the named dead (letters), truth behind
rumor (telegrams), the enemy's parallel bookkeeping (ledger pages),
and the gods' counsel (fortune slips).

**Discovery:**
- Required: none — all documents optional; skipping them is itself a
  ledger entry (the historian didn't look)
- Optional: all — found through scouting actions, shrine visits,
  post-battle aftermath sweeps

**Rewards:** Lore-first — but telegrams and fortune slips carry real
intelligence value (verifying判詞, hinting ambushes); ledger pages
reveal which doctrine habits the rival is currently countering.

---

## Narrative Delivery

### Cutscenes

**Quantity:** 4–5 major set-pieces across the whole campaign — only
the biggest beats earn real-time staging: 神像流淚 (Ch 1), 放生
(Arc II), 戰神顯形 (midpoint), 直面戰神 (Ch 15), 擱筆 (finale).
**Average Length:** Short — each a single beat, tens of seconds; the
candle-and-brush frame keeps them cheap and uniform.
**Style:** Real-time rendered in-engine — the war map and the
ink-chronicle themes do the staging; the ledger-page frame is the
transition device (brush lifts, page turns, scene plays, brush
returns).
**Skippable:** Yes — but skipping a major scene still writes its
ledger entry; the record happens whether or not you watch.
**Interactive Elements:** None — no QTEs; interactivity lives in the
event-option dialogues, not the cutscenes.

**Minor delivery:** all other narrative beats use the ledger-page
format — 題詞 written character by character over a still frame,
帳頁 turning, seal-stamps for chapter ends.

---

### In-Game Storytelling

**Primary Methods:**
- The ledger itself — 題詞, 判詞, after-action reports, roster,
  omission counts; the UI *is* the narrator
- Event-option dialogues at battle-loop boundaries (勸降 / 審判 /
  神明提案)
- **Mid-execution inserts** — narrative events can fire during
  real-time execution: myth seepage signs, enemy commander callouts,
  the sparing condition surfacing mid-battle
- Environmental storytelling — dual-layer scars, shrine offerings,
  the rival's too-clean fields
- Found documents — letters, telegrams, ledger pages, fortune slips

**Show vs. Tell Balance:** Show-first — the player watches the real
battle; the ledger's compressed telling arrives after and is
*auditable* against what was seen.

**Interruption Approach:** Story may interrupt execution, but rarely —
mid-battle inserts are reserved for beats that change the battle's
meaning (a god arriving, a mercy condition appearing), never for
flavor. Standard narrative stays on the beat boundaries (before
deployment, after outcome).

**Player Control:** Battle playback speed and pause are player-owned;
mid-execution inserts pause time briefly and are dismissible — but
their ledger entries still record that they happened (and that the
player looked away).

---

### Optional Content

**敵將傳記後日談:**
Every subdued/spared commander earns an epilogue mini-biography —
written into the final chapter's record. Built on the existing
disposition-tracking mechanism; the ending reads *their* fates aloud.

**神明試煉:**
Side events at village and wild shrines — a local god sets a task
(disaster relief, vow-keeping, cleansing haunted ground); passing
returns a named myth-layer blessing. The narrative face of the
民心 amplifier.

**對手的過去:**
Collectible ledger pages reconstruct the rival's
defeat-and-possession battle; gathering enough unlocks an extra
exchange at the final audit — the mirror shown its own first page.

**名冊詳傳:**
Every roster entry opens into a micro-biography — origin, enlistment,
service record, manner of death. The veteran's is the longest in the
game, because it spans all fifteen chapters.

---

### Ending Structure

**Number of Endings:** 4 — 霸業 / 仁政 / 無字 / Fallen.

**Ending Triggers:** The accumulated record — conquest metrics versus
governance metrics (民心/秩序), the 墮落 ratchet, and the
contradiction condition (conquest and hearts both high → the ledger
cannot reconcile → 無字). No final choice; the page is *generated*.

**Ending Variety:** Three hands plus silence — 霸業 and 仁政 share
the historian's brush in different registers; Fallen is written in
the rival's hand (the player's brush having become his); 無字 is a
blank page — the historian who cannot write.

**True Ending:** None — deliberately. All four are valid writings of
the era; 無字 is the hardest to reach, not the best. The design
refuses to canonize an answer to "who pays for victory."

**Replayability:** Each ending re-reads the whole campaign — chapters
are branchable at the ledger level (different choices write different
books), and the rival's adaptive counter-deck means no two audits
read alike.

---

## Gameplay Integration

### Narrative-Gameplay Connection

**Integration Approach:** Total integration — the ledger is the
mechanic. The replay event stream is the narrative's data source;
doctrine cards are the player's authored sentences; the after-action
report is gameplay output rendered as historiography. Story and
systems share one spine.

**Mechanic-Theme Alignment:**
- Doctrine authorship = the counselor's craft made playable
- Intelligence economy = the audit habit made costly (scout to
  fact-check the pen)
- Roster persistence = the names theme made data
- Myth-layer strength = 民心 made visible — belief is infrastructure
- The 墮落 ratchet = the Fallen theme made irreversible on purpose

**Story-Gameplay Balance:** The three-beat loop is the pacing unit —
planning (gravitas), execution (suspense), aftermath (mourning).
Narrative rides the loop's boundaries; mid-execution inserts are rare
and reserved.

**Ludonarrative Considerations — the four guards:**
1. **Mercy betrayed must be fair.** 叛將之筆 is scripted, not random —
  判詞 carries the risk signal (cunning flags); mercy is a priced
  investment, never a trap.
2. **Omissions are auditable.** The ledger's失真是 never a lie told
  to the player — omission counts are displayed, replay always
  reconciles; the distortion is the gameplay, not a cheat.
3. **無戰 is a strategy, not a skip.** Subduing without war costs
  intelligence, 民心, and timing — a doctrine of its own, not a
  button.
4. **墮落 is visible.** The ratchet telegraphs: brush changes,
  omission counts, the drums. You write the Fall; the game doesn't
  do it to you.

**Arc-gated mechanics** — the era's toolkit matures with the story:
- Arc I: reading enemy decks, the判詞 layer
- Arc II: myth seepage, coalition/betrayal events
- Arc III: the invader's unsubduable rules, war-god interventions
- Arc IV: the rival's counter-deck reads *your* habits — the systems
  turn on their maker

---

### Story Gating

**Gating Approach:** Soft gates within hard arcs — the four arcs are
fixed in order (the era marches one way), but chapters *inside* an
arc are freely sequenced: a sandbox of fronts, a fixed spine of time.

**Story-Locked Elements:** Arc mechanics (above); the rival's full
appearances are beat-gated; the Field Altar opens only where
slaughter is thickest; 無字 unreachable by accident — it requires
the contradiction built deliberately.

**Cutscene Triggers:** The 4–5 major set-pieces are beat-bound
(神像流淚 / 放生 / 戰神顯形 / 直面戰神 / 擱筆); 放生 additionally
requires the scripted defeat condition — miss it and the beat does
not occur.

**Mandatory Story Beats:** Arc transitions and their major set-pieces;
chapter-level beats inside an arc are optional in order but not in
consequence — skipped fronts still write their ledger lines.

**Optional Narrative:** God trials, found documents, commander
epilogues, roster biographies, the rival's past — all optional, all
written into the final page if touched.

---

### Player Agency

**Agency Level:** Meaningful choices — the ending is generated from
the accumulated record, never chosen from a menu. The player's agency
is authorship: every doctrine hand, mercy order, and ledger habit is
a sentence in the verdict.

**Player Influence:** Doctrine composition; mercy and atrocity
(deliberate orders only); event options; which fronts are
taken within an arc; whether判詞 is trusted or verified; whether the
fox's price is paid.

**Choice System:**
- Choice types: doctrinal (deck authorship), tactical (mid-battle
  interventions), moral (mercy/atrocity/no-battle calls), epistemic
  (scout vs. trust the rumor)
- Consequence scope: chapter outcomes ripple into dispositions,
  god-stance, title-track, and the ending's hand
- Timing: planning phase (deck), event scenes (options), aftermath
  (what the ledger becomes)

**Role-Playing Freedom:** 筆法即人格 — no chosen personality. The
historian's voice is *generated* from conduct: habitual omission
writes an evasive pen; habitual verification writes an exacting one;
habitual clemency writes a merciful one. The player does not pick a
mask — the ledger shows what the hands did.

---

## Production Planning

### Writing Scope

**Estimated Word Count:** ~50,000+ (per language)

**Content Breakdown:**
- Main story (17 beats, arc transitions, key conversations): ~15,000
- Modular ledger corpus (event fragments + judgment templates +
  題詞/判詞 libraries): ~15,000
- Optional content (commander epilogues, roster biographies, god
  trials, rival's ledger pages, fortune slips): ~12,000
- Endings (four hands/registers + final-page generation strings):
  ~4,000
- UI/system text (event options, titles, warnings, 省略 markers):
  ~5,000

**Scene Count:** ~15 chapters + 5 major set-pieces + ~6 recurring
conversation types.

**Dialogue Lines:** ~600–900 lines — concentrated in event-option
scenes and the six key conversations; the ledger carries the rest.

**Branching Complexity:** Branches are islands, not trees — each event
produces 2–3 variant ledger entries rather than divergent scenes; the
modular report system multiplies coverage without multiplying prose.
The Fallen path re-inks existing beats rather than authoring new ones —
its cost is register, not volume.

---

### Localization

**Target Languages:** 繁體中文 (primary) + English.

**Cultural Adaptation Notes:** The historian registers are the hard
problem — semi-classical ledger Chinese has no off-the-shelf English
equivalent. Working rule: the English ledger reads as *archival
Victorian reportage* (dispatches, casualty lists, "it is reported
that…"), preserving the 據報 device rather than imitating the
calligraphy. 判詞 translate as single-line epithets; 籤詩 as
delphic verse. Terms kept romanized-with-gloss: 民心, 判詞, 無戰.

**Technical Considerations:**
- Text expansion buffer: EN ~+30% over CJK — ledger UI and報告
  templates need flexible line-wrapping, not fixed boxes
- UI flexibility: the ledger-page format tolerates reflow better than
  HUD strings; flag fixed-width elements early
- Audio approach: subtitle-only localization — no dubbed English
  track planned; the voiced set-pieces keep one voice track

---

### Voice Acting

**Approach:** Key scenes only — the 4–5 major set-pieces are voiced;
everything else is text + ambient sound design.

**Characters Needing Voices:** ~6 — the rival (two-voice layering is
the showcase), the fox spirit, the veteran, the abbot, the adjutant,
and enemy-commander callouts in the major beats.

**Dialogue Volume:** ~80–150 recorded lines total — cutscene lines
plus battle-callout barks for the beats.

**Voice Cast Notes:** The rival's two-voice layering is the technical
centerpiece — record the man's line and the patron's shadow-line
separately, mix with offset/doubling; the god's register should sit
*under* the human voice, not replace it. The fox needs playfulness
with menace underneath.

**Recording Approach:** Placeholder scratch first (TTS or internal)
to time the scenes; professional recording only for the final
set-pieces once chapter scripts lock.

---

## Appendix: Character Relationships

### Relationship Map

```
                     戰神 (無名古神)
                         |
                   (附身·認可)
                         |
   對手的副官 ──(記帳·忠於人非神)── 聶懷仁
        |                            |
   (遞帳·身後)                  (鏡像·放生·最後審計)
        |                            |
        └───────────→ 軍師史官 ←───────────┘
                         |
              ┌──────────┼──────────┐
              |          |          |
        (教導/錨)   (情報·交易)  (治理·民心)
              |          |          |
        倖存老兵    狐仙線人     老住持
              \          |          /
               \    (代言·不可全信) /
                \        |         /
                 安分的神 ──(生態敵對)── 戰神一系
```

### Relationship Key

- 戰神 → 聶懷仁: possession/ratification — man and patron in one flesh
- 聶懷仁 → 軍師史官: mirror/foil — same craft, opposite bookkeeping;
  spared him once
- 對手的副官 → 聶懷仁: loyal ledger-keeper → 軍師史官: delivers the
  second book
- 狐仙線人 → 軍師史官: myth-layer informant — sells truth in
  installments
- 狐仙線人 → 安分的神: their only mouth — her translations may be her
  own
- 倖存老兵 → 軍師史官: human anchor — the roster's living entry
- 老住持 → 軍師史官: governance barometer — receives armies by what
  they leave
- 安分的神 ↔ 戰神一系: ecological enemies — it eats the hearts they
  need

---

## Appendix: Story Timeline

### Chronological Events

```
[BACKSTORY: 王朝傾覆→軍閥割據;對手兵敗被附身;神明因不信而衰]
|
v
[ARC I 軍閥割據 Ch1–4]
├── 神像流淚 (神話先動 — inciting incident)
├── 斷橋初筆 (首戰 / 首判詞)
├── 殲滅的筆跡 (對手現於戰報)
└── 初次無戰
|
v
[ARC II 北伐 Ch5–8]
├── 同盟誓約 (盟友 = 未來敵人)
├── 叛將之筆 (慈悲被辜負)
├── 神話改寫 (神蹟改地形)
├── 放生 (欠對手一命)
└── 戰神顯形 ★MIDPOINT (萬人目睹,帳寫「據報」)
|
v
[ARC III 抗戰 Ch9–12]
├── 不可收服之敵 (無戰 = 共謀)
├── 無戰的代價 (讓省保軍)
├── 無勝而勝試煉 (學會正確地輸)
└── 對手裂縫 (戰神索血過量)
|
v
[ARC IV 內戰 Ch13–15]
├── 昔日戰友 (熟人 deck 成敵)
├── 最後審計 (對手讀過你的筆)
├── 直面戰神 ★CLIMAX (對戰爭本身)
└── 擱筆 (四結局:霸業/仁政/無字/對手之筆)
```

### Timeline Notes

Arc order fixed; chapter order within arcs free (弧內自由序). Beat 8
(放生) is conditional — requires the scripted defeat. The myth layer
runs a second, slower wave: quiet in Arc I, surfacing at the midpoint,
co-dominant by Arc IV.

---

## Appendix: References and Inspirations

- **民國無雙** — the 演義 romanticization of Republican-era figures:
  era personalities written as larger-than-life heroes. Informs the
  判詞 tradition of treating commanders as legends-in-the-making —
  the readable deck as literary device.
- **中山立志傳** — the chronicle/life-saga model of living through
  the whole Republican timeline: personal history crossing four eras.
  Informs the campaign's biographical spine — one counselor's ledger
  across warlord, expedition, resistance, and civil war.
- Supporting registers already embedded in the design: 三國演義
  chapter conventions (題詞/判詞/欲知後事), 孫子兵法 as the
  doctrine layer's philosophy, and 據報 historiography as the myth
  layer's interface.
