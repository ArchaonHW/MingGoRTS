---
title: 'Narrative Design Document'
project: 'MingGoRTS'
date: '2026-09-17'
author: 'potat'
version: '1.0'
stepsCompleted: [1, 2, 3]
status: 'in-progress'
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
