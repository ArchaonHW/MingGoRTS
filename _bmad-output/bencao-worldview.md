---
title: 'Bencao Worldview Supplement（本草書中書）'
project: 'MingGoRTS'
date: '2026-10-05'
author: 'potat'
version: '0.1'
status: 'draft'
parent: '_bmad-output/narrative-design.md'
integration: 'collection-layer only — no mechanical/stat changes'
scholarship: 'full-fidelity entries; every 性味歸經/主治 claim must verify against《本草綱目》before content lock'
---

# Bencao Worldview（本草書中書）

> 帳記人之所為，草記世之所癒。
> *The ledger records what men do; the bencao records what the world offers to heal them.*

---

## 1. Premise: The Second Book（案頭第二書）

The office of Chronicler（幕府記室）comes with two books, inherited together
with the post:

- **The Ledger（帳本）** — the book the player *writes*: double-entry, audited,
  forgeable.
- **The Bencao（本草）** — the book the player *reads*: a field abridgement
  （節鈔本） of a great Ming compendium of materia medica, carried through the
  campaign in the same dispatch chest as the blank ledger paper.

In-fiction it is never called by its real title on the page header — it is
「本草」, the way soldiers call a rifle 「傢伙」. But its序文 fragment survives
at the front of the codex, and the Scribe treats it with a reverence he gives
nothing else: it is the one book in the war that was written to *help*.

Li Shizhen（李時珍）exists in-world only as a name in that序文 and in the
Scribe's marginalia — 「蘄州李氏嘗言…」 — a predecessor author the way the
rival generals are predecessor readers. He never appears. **The book itself
is the character**: multi-handed, worn, trustworthy in a world where the
ledger is not.

### The Mirror Structure

The two books are mechanical and thematic mirrors:

| Ledger（帳本） | Bencao（本草） |
|---|---|
| Written by the player | Written by the dead (and annotated by the living) |
| Records deeds and their costs | Records things and their virtues |
| Audited — entries can be forged | Verified — entries were tested on bodies |
| Double-entry: every credit books a debit | 是藥三分毒: every virtue carries a toxicity |
| Judges the player at the ending | Judges nothing — it only describes |

This asymmetry is the design: in a game where every text is suspect, the
bencao is the one page the player can believe — and the educational payload
rides on that trust.

---

## 2. Why Bencao Fits This Game（主題編織）

The bencao is not pasted on; its native vocabulary already *is* the game's
theme system:

| Bencao doctrine | Game pillar it mirrors |
|---|---|
| 上工治未病（the best physician treats before illness） | 至聖者無戰 — the best victory is the one never fought |
| 君臣佐使（sovereign-minister-adjutant-courier formulation） | Doctrine composition — a squad sheet *is* a方劑; cards are藥味 with roles |
| 是藥三分毒 / 大毒之藥（toxic but effective drugs） | 墮落 ratchet — effective-but-corrupting options, one-way debt |
| 調和、和解（harmonizing, reconciliation） | Governance victory path; 甘草「國老」is its patron herb |
| 綱目 taxonomy — world made legible by cataloging | The ledger's double-entry — cataloging as judgment, the premise's other face |
| 性味歸經（nature, flavor, meridian tropism） | Legibility itself: the world described by *qualities*, not by ownership or fog |

The bencao also gives the myth layer a native idiom: 神農嘗百草 is the
*founding audit* — the first person who tested claims on his own body so the
record could be trusted. MythLog folk-register and bencao empiricism are the
same epistemology at two altitudes.

---

## 3. Codex Structure（部類改編）

The real compendium runs 16部·60類·1892種. The game codex uses a **bounded
subset of 8 categories**, each mapped to a gameplay surface so unlocks fall
out of play rather than a checklist:

| Codex 類 | Source 部 | Gameplay surface that feeds it |
|---|---|---|
| 山草類 | 草部·山草 | Mountain/highland chapter terrain |
| 隰草類 | 草部·隰草 | Lowland, marsh, field battles; 民兵 villages |
| 毒草類 | 草部·毒草 | 墮落 events, atrocity forks, "effective but corrupting" choices |
| 蔓草/水草類 | 草部 | River crossings, ferries, convoy maps |
| 穀菜類 | 穀部·菜部 | Villages, 民心 events, supply/物資, famine & plague governance |
| 金石類 | 金石部 | Shrine layer, forgery theme （硃砂, 雄黃, 龍骨) |
| 蟲獸類 | 蟲/鱗/獸部 | Myth encounters, rare "wonder" entries （麝香, 牛黃, 蜈蚣） |
| 人部拾遺 | 人部 | The darkest class — body-derived materia; gated behind late grim beats, always flagged |

**Entry anatomy（本草體）** — every entry renders as a page with fixed fields:

```
【正名】 the canonical drug name
【釋名】 folk/alternate names — where the poetry lives
【集解】 source, habitat, identification — hearsay mood allowed here
【性味歸經】 nature（寒熱溫涼平）· flavor（酸苦甘辛鹹）· meridians
【主治】 indications, per the source text
【批註】 the Scribe's hand — binds the entry to *this* campaign
```

The批註 field is the load-bearing one: it is where the herb stops being
reference and becomes memory. Every unlocked entry eventually receives a
marginal note tying it to a ledger event the player authored — so the
educational text is *personalized* by play.

---

## 4. Unlock Model（收藏層——不改數值）

Pure collection layer. No stat effects, no consumables, no gating of
victories. Entries unlock as **documents**, delivered the same way as every
other text in the game:

| Trigger family | Example unlocks |
|---|---|
| **Terrain read** — chapter map's dominant terrain | Highland map → 山草類； river/ford maps → 水草類 |
| **Ledger events** — things the player booked | First Loss（初殤）→ 三七、白及（止血藥）; village burned → 地榆、伏龍肝 |
| **Governance events** | 疫癘 outbreak → 藿香、柴胡、連翹； famine relief → 穀菜類 |
| **RefitCamp** | Veteran squad with old scars → 續斷、骨碎補、杜仲（筋骨藥） |
| **Myth layer** | First shrine pacified → 菖蒲、艾； deep GodStance → 靈芝、茯神 |
| **墮落 ratchet** | First corrupt option → 附子、烏頭； deep 墮落 → 砒霜、罌粟 |
| **Chapter close** | Each 回 closing report: the Scribe「補鈔一頁」— one page arrives unasked |
| **Dossier hearsay** (optional depth) | Rival generals rumored to keep medics → 軍中藥 entries |

### The 補鈔 conceit（偽帳鄰接）

Unlocked pages arrive as 「補鈔」— pages *added to the book while you weren't
looking*. This deliberately rhymes with the forgery theme: the ledger grows
suspicious entries, the bencao grows helpful ones. The player learns to
dread marginalia in one book and welcome it in the other — same handwriting,
opposite intent. Whether some bencao pages are *also* forged (a poisoned
entry — literally) is reserved as an open question (OQ-B2) and, if used, is
the collection layer's only narrative trap.

### Frontispiece disclaimer（體例內的免責）

The codex opens on a序頁 written in the same 史官體， in-fiction honest:

> 「是冊所載，皆前人之驗、草木之性，錄以備考，非為用藥之據。
>  病家慎勿執紙上之言以試人身。」
> *What this book records is the ancients' testing and the nature of herbs,
>  kept for reference — not a prescription. Do not test paper words on a body.*

An English-facing equivalent line rides in the codex footer: *Historical
text reproduced for the fiction; not medical advice.*

---

## 5. Launch Entry Set（全真實條目）

Content budget: **48–72 entries** total, 4–6 per chapter × 8–12 chapters,
~150–250 characters each → ~10–18k characters of source-checked text.

### 5.1 Worked exemplar（全形一頁）

```
【三七】 山草類
釋名：山漆、金不換。謂其能合金瘡，如漆之黏物；貴重難得，故云不換。
集解：生廣西、雲南山峒深處，掘根暴乾，黃黑色、團結者佳。
      （或云：軍中謂之「金不換」，以其價重於金。）
性味歸經：甘、微苦，溫。歸肝、胃經。
主治：止血散血，定痛。金刃箭傷、跌扑杖瘡血出不止者，
      嚼爛塗之或末摻之，血立止。
批註：第三回，斥候隊王二麻子中箭傷臂，軍醫即以此末敷之，血止。
      帳上記「傷一名」；頁邊此處，記「止血一錢」。同一事也，兩書各記其半。
```

That last批註 line — 「同一事也，兩書各記其半」("the same event; each book
records its half") — is the thesis of the whole supplement.

### 5.2 Thematic exemplars（六味定調）

Six anchor entries chosen so the *collection itself* teaches the theme map:

| Herb | Why it's an anchor | Unlocks on |
|---|---|---|
| **甘草** | 「國老」— 調和諸藥， mediator of all formulas; patron herb of the governance path | First governance victory condition met |
| **三七** | 金瘡要藥 — the war's most literal herb; first blood teaches its name | First Loss beat |
| **附子** | 大熱大毒，回陽救逆 — power that saves at a price; the 墮落 ratchet made botanical | First corrupt option taken |
| **靈芝** | 六芝安神 — the myth layer's own materia medica; belief made legible | First shrine pacified |
| **龍骨** | Fossil「龍骨」= oracle bones = *writing on bone* — the game's writing-as-truth thesis in mineral form; the Scribe's批註 can carry the anachronism he alone knows: 「或云殷人刻辭之骨也」 | Forgery theme surfacing (Beat 7) |
| **罌粟（米囊子）** | 本草原載：澀腸止痢固脫 — a legitimate drug whose age corrupted it; the era's wound in one entry, handled soberly | Late campaign; 墮落 ≥ threshold |

### 5.3 Launch table（首發目錄 ~30，每條需回校原文）

Confidence flags: **[A]** = well-attested standard data, still re-verify;
**[V]** = verify wording against 原文 before lock.

| Entry | 類 | 性味歸經（校對用） | 主治要點 | Unlock trigger | Flag |
|---|---|---|---|---|---|
| 三七 | 山草 | 甘微苦溫；肝胃 | 止血散瘀定痛，金瘡要藥 | 初殤 | A |
| 甘草 | 山草 | 甘平；心肺脾胃 | 補脾益氣，清熱解毒，調和諸藥 | 首次治理達標 | A |
| 黃連 | 山草 | 苦寒；心脾胃膽大腸 | 瀉火燥濕解毒 | 疫病/瘴癘地圖 | A |
| 人參 | 山草 | 甘微苦微溫；脾肺心 | 大補元氣 | 宿敵勸降線 or 重大損失後 | A |
| 當歸 | 山草 | 甘辛溫；肝心脾 | 補血活血 | 具名小隊存活 N 回 | A |
| 柴胡 | 山草 | 苦辛微寒；肝膽 | 和解退熱疏肝 | 不戰而勝章節 | A |
| 白及 | 山草 | 苦甘澀微寒；肺胃肝 | 收斂止血生肌 | 初殤（與三七同批） | A |
| 續斷 | 隰草 | 苦辛微溫；肝腎 | 補肝腎續筋骨 | 老兵小隊帶疤再戰 | A |
| 骨碎補 | 石草/隰草 | 苦溫；腎肝 | 補腎續傷 | RefitCamp 重傷歸隊 | A |
| 杜仲 | 木 | 甘溫；肝腎 | 補肝腎強筋骨 | 同上批次 | A |
| 地榆 | 隰草 | 苦酸微寒；肝大腸 | 涼血止血，燒燙傷 | 村莊被焚事件 | A |
| 伏龍肝（灶心土） | 土/金石 | 辛溫；脾胃 | 溫中止血止嘔 | 村莊被焚事件（竈神批註） | V |
| 藿香 | 芳草 | 辛微溫；脾胃肺 | 芳香化濁，暑濕疫癘 | 疫癘治理事件 | A |
| 連翹 | 隰草 | 苦微寒；肺心小腸 | 清熱解毒散結 | 疫癘治理事件 | A |
| 金銀花（忍冬） | 蔓草 | 甘寒；肺心胃 | 清熱解毒 | 疫癘/癰疽事件 | A |
| 艾 | 隰草 | 苦辛溫；肝脾腎 | 溫經止血散寒，灸百病 | 端午/神祠習俗事件 | A |
| 菖蒲 | 水草 | 辛溫；心胃 | 開竅豁痰辟穢 | 首座神祠平定 | A |
| 車前 | 隰草 | 甘寒；肝腎肺小腸 | 利水通淋 | 渡口/水澤地圖 | A |
| 紫蘇 | 芳草 | 辛溫；肺脾 | 解表散寒，解魚蟹毒 | 水鄉/食補事件 | A |
| 薄荷 | 芳草 | 辛涼；肺肝 | 疏散風熱清利咽喉 | 夏季章節 | A |
| 穀芽/粳米 | 穀 | 甘平/溫；脾胃 | 消食和中 | 糧道/輜重事件 | V |
| 茶 | 木/果 | 苦甘涼；心肺胃 | 清头目除煩渴 | 山地/南方章節 | V |
| 靈芝 | 芝（菜部） | 甘平；心肺肝腎 | 補氣安神止咳平喘 | 神祠深層 GodStance | A |
| 茯神 | 木（寓木） | 甘淡平；心脾 | 寧心安神 | Myth Knocks 後 | A |
| 龍骨 | 鱗（金石性） | 甘澀平；心肝腎 | 鎮驚安神收斂固澀 | 偽帳浮現（Beat 7） | A |
| 硃砂 | 金石 | 甘微寒（有爭議） | 清心鎮驚安神解毒 | 神祠/批註朱字事件 | V |
| 雄黃 | 金石 | 辛苦溫有毒 | 解毒殺蟲 | 端午/驅邪 MythLog | A |
| 附子 | 毒草 | 辛甘大熱有毒；心腎脾 | 回陽救逆補火助陽 | 首次墮落選項 | A |
| 烏頭 | 毒草 | 辛熱有大毒 | 祛風濕溫經止痛（與附子同株） | 墮落加深 | A |
| 蜈蚣 | 蟲 | 辛溫有毒；肝 | 息風鎮痙攻毒散結 | 以毒攻毒敘事線 | A |
| 罌粟（米囊子） | 穀 | 酸澀平（殼）；肺大腸腎 | 澀腸止痢固脫止咳 | 晚期＋墮落門檻 | V |
| 血餘炭 | 人部 | 苦平；肝胃 | 止血化瘀 | 大敗/慘烈章節 | V |

> **人部拾遺 note:** the real compendium's 人部 contains entries modern
> readers find disturbing. The codex keeps this class minimal (1–3 entries),
> always flagged in-fiction by the Scribe, and treats it as the collection's
> dark corner — the catalog's own honesty that not everything recorded
> should be repeated.

---

## 6. Voice and Register（文體譜系）

The bencao adds a **sixth document voice** to the game's correspondence
fiction (per narrative-design §Dialogue Framework):

| Voice | Register | Tell |
|---|---|---|
| HistorianReport | impersonal, elliptical | 「斬獲甚多」 |
| Court commission | imperative, formulaic | reign-title headers |
| Scribe marginalia | first-person, questioning | 「此數可疑」 |
| GeneralDossier | hearsay mood | 「據聞」「或云」 |
| MythLog | folk register | rumor, not record |
| **Bencao entry** | **descriptive-catalog — 本草體** | 「味甘，微溫，無毒。」 |
| **Bencao批註** | **the Scribe's same small hand** | 「同一事也，兩書各記其半」 |

Key distinction: 本草體 **accuses no one**. A herb is catalogued, not judged.
Where every other voice in the game has an agenda (even the Scribe doubts),
the bencao's flat descriptive confidence — this plant *is* bitter, *is*
cold, *does* stop bleeding — is the fiction's one patch of firm ground. The
educational text inherits that trust. Guard it: never put editorial opinion
in 【主治】; opinion lives only in 【批註】, in a visibly different hand.

---

## 7. Touchpoints with Existing Systems

| Existing system | Bencao hook |
|---|---|
| ChapterLibrary / NarrativePack | Codex entries ship as per-chapter content packs; unlock manifest lives in chapter JSON |
| HistorianReport | Clause-pool color: post-battle report may append 「傷者敷以三七之屬」 — the report cites the book the player is reading |
| MythLog | Folk register entries 「或云某山有靈芝」 that later unlock the sober catalog entry — rumor preceding verification, hearsay→empiricism arc in miniature |
| Ledger marginalia system | 【批註】field reuses the Scribe's marginalia rendering — same UI edge, different book |
| QuantumFog | None. The bencao has no fog: it is the anti-fog. (Deliberate contrast — keep it.) |

**Content schema (proposal):** `potato.bencao/1` — versioned JSON per
content policy; fields mirror §3 entry anatomy; unlock declared as trigger
key, resolved by campaign/codex layer, never by sim. Sim stays clean: the
codex is a Campaign-layer read model over ledger events, zero tick-path
involvement (satisfies the determinism and no-I/O constraints).

---

## 8. Boundaries and Non-Goals

- **No mechanics.** Unlocks never grant stats, consumables, or doctrine
  bonuses. The reward is the page itself and the批註 that personalizes it.
- **No invented 性味.** Every catalogued claim traces to the source text;
  fictionalization is confined to 【批註】 and 【集解】's hearsay mood — the
  two fields already marked as hands, not facts.
- **No medical advice.** The frontispiece disclaimer (§4) is a content
  requirement, not decoration.
- **毒草類 is not a how-to.** Toxic entries render efficacy *and* danger
  from the source text, framed by theme （墮落）, never usage instruction.
- **李時珍 stays a name.** No character, no quest giver. The book is the
  monument; the man stays in his序文.

---

## 9. Open Questions

- **OQ-B1: Codex home in UI** — does the bencao live beside the ledger as a
  second tab, or in the RefitCamp desk scene? (Epic F presentation call.)
- **OQ-B2: Forged pages** — may a poisoned entry (a deliberately wrong
  主治） ever enter the codex as a forgery-theme beat? Powerful but risky:
  it undermines the book-as-trust design unless telegraphed by the Scribe.
- **OQ-B3: English rendering** — 本草體 translation strategy （性味 as
  "nature/flavor" vs. calqued terms); same genre-translation problem as
  史官體 elliptical counts.
- **OQ-B4: Entry count commitment** — 48 floor vs 72 ceiling; decide with
  chapter count after E0 playtest (same gate as GDD assumption 2).
- **OQ-B5: 人部 inclusion** — ship 0, 1, or 3 entries; flag sensitivity per
  §5.3 note.

---

## Appendix: Relationship to Narrative Design Document

This supplement extends `narrative-design.md` without amending it:

- Its existence was already seeded — the parent document's appendix cites
  《本草綱目》 as governing reference ("cataloging is judgment").
- It adds one document voice (§6), one codex structure (§3), one unlock
  model (§4) — all collection-layer, no changes to beats, arcs, endings,
  or mechanics described there.
- If OQ-B2 (forged entries) is adopted, that becomes a cross-reference
  into narrative-design §Optional Content (forgery side-lines).
