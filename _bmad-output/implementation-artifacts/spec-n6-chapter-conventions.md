# N-6 章節慣例文字層（題詞/判詞/欲知後事/結局四聲部）

- status: done
- size: S
- source: narrative-design §Structure Type + §The Ending — Written, Not Chosen
- test: ChapterConventionsTest 29/29 PASS

## 需求

演義章回慣例入 UI：章開題詞（一句詩問）、敵將入場判詞、
章尾「欲知後事」鉤；結局四聲部（霸業/仁政/無字/對手之筆）。

## 驗收

- [x] 章節慣例三句型皆有生成介面（UI 層呼叫 `ChapterConventions` 即呈現）
- [x] 結局文由累積記錄生成（非選項選擇）——`ComposeEnding` 只吃
      CampaignLedger + MythLog + EndingInput 統計

## 實作

`Gameplay/ChapterConventions.h/.cpp`——純文字層，全部 static，
不持有狀態；輸入全部來自累積記錄子系統。

### 章開題詞 `OpeningVerse(chapter, theme)`

五幀演義體詩問韻庫（滾滾長江/青山依舊/天下大勢/成敗轉頭/
浪花淘盡），`(chapter-1) % 5` 輪替定韻，`theme` 嵌入主題詞；
空主題落通用問句，非法章序鉗制為 1。

### 敵將入場判詞 `EntranceJudgment(dossier, name)`

接 N-2 `GeneralDossier`（聽聞態判詞）：
- 無檔案 → 「虛實未聞，帳上無此判詞」
- 未驗證 → 「據報敵將X——判詞」（聽聞體）
- 已驗證 → 直書判詞（狡詐者 dossier 已附「前判有詐」揭曉語）

### 章尾鉤 `ClosingHook(ctx)`

`ClosingContext`：勝敗/平手、下回伏筆詞、N-3 反制旗標、
本章神話事件數。組「收束語＋異象/窺破附語＋欲知……
且聽下回分解」。

### 結局四聲部 `ResolveEnding` + `ComposeEnding`

`EndingInput`：累計殲滅/收服數（武功）+ 民心/秩序（治績，
與 SageCommand 同源門檻 60）+ 墮落。

判定序（寫死）：
1. 墮落 ≥100 → `Fallen`（對手之筆）
2. 武功顯（殲滅>收服）且民心秩序俱 ≥60 → `Unwritten`（無字）
3. 僅武功顯 → `Conquest`（霸業）；僅治績顯 → `Mandate`（仁政）
4. 皆未達檻 → 兩分數取高者歸檔

`ComposeEnding`：聲部主文 + 帳本處置/稱號引用（N-4
`EndingCitations`）+ 神話證詞（N-5 `TestimonyLines`）。
無字：主文為空——史官擱筆，只剩累積記錄自身的聲音。

## 測試覆蓋（29 checks）

- 題詞嵌主題/問句/韻庫輪替/邊界
- 判詞三態（據報/驗證直書/虛實未聞）
- 章尾鉤：勝敗平三收束語、伏筆嵌入、神話與反制附語、套語
- 四聲部判定全分支（Fallen/Unwritten/Conquest/Mandate/皆低取高）
- 結局文：史官語體、處置/稱號/神話引用、無字無主文仍存記錄、
  對手之筆語體
