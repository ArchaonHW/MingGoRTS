# 天使軍團 WEB3 — 宣傳預告片分鏡腳本 + AI 生成 Prompts

> MiniMax-H3 原生格式 prompts（自帶音訊生成）見同目錄
> `web3-angel-legion-h3-prompts.md` — 以下為通用版。

- **規格**：16:9 橫式，總長約 50 秒
- **用途**：官網首頁 / YouTube / X(Twitter) 宣傳
- **產出方式**：各鏡頭 prompt 貼入 AI 影片工具生成後剪接；字幕與 Logo 後製加上（見「後製規範」）
- **占位符**：`{TOKEN}`、`{LOGO}`、社群連結請於剪接時替換

---

## 全局風格錨點（每個 prompt 尾部統一附加）

```
epic cinematic anime style, dark celestial atmosphere, deep violet night sky with nebula,
volumetric god rays, gold and cyan-violet bioluminescent glow, floating luminous particles
and white feathers, subtle holographic hexagonal Web3 UI accents, cinematic color grading,
film grain, high detail, 16:9 aspect ratio
```

### 通用負面 Prompt

```
low quality, blurry, distorted anatomy, extra limbs, watermark, text, subtitle, logo,
oversaturated, flat lighting, jpeg artifacts
```

> 注意：AI 影片工具生成文字幾乎必糊，**所有中文字幕與 Logo 一律後製**，prompt 已排除文字。

---

## 分鏡腳本（8 鏡，約 50s）

### S1 — Cold Open「墜落之光」｜0:00–0:05

- **畫面**：黑暗宇宙虛空中，一根白金發光的羽毛緩緩飄落，拖曳微光粒子尾跡，遠處紫羅蘭星雲
- **運鏡**：微距跟拍羽毛下落，緩慢推近
- **字幕**：當世界陷入黑暗
- **音效**：低頻 drone + 一聲心跳

```
Macro shot, a single luminous white-gold angel feather drifting down through a dark
cosmic void, faint violet nebula in the far background, the feather leaving a trail of
glowing golden particles, extremely slow falling motion, camera slowly pushing in,
mysterious and sacred mood
```

### S2 — 覺醒｜0:05–0:10

- **畫面**：羽毛觸及水面/鏡面般的地面激起光之漣漪；切到天使眼部特寫，金色瞳孔驟然睜開
- **運鏡**：漣漪擴散 → 硬切眼部大特寫
- **字幕**：她們，將再次集結
- **音效**：光爆 whoosh + 弦樂拉起

```
Extreme close-up of a beautiful anime angel warrior's closed eyes, silver eyelashes,
suddenly her eyes snap open revealing glowing golden irises, a burst of radiant light
reflecting in the pupil, light particles erupting outward, dramatic reveal moment
```

### S3 — 軍團現身｜0:10–0:17

- **畫面**：大遠景——數十位天使戰士佇立於雲海之上的浮空水晶平台，光翼依序展開如連鎖反應，聖光沖天
- **運鏡**：低空緩慢環繞上升（orbital crane）
- **字幕**：**天使軍團**（標題主字幕，後製大字）
- **音效**：史詩合唱進入

```
Epic wide aerial shot, an army of anime angel warriors standing on floating crystalline
platforms above a sea of clouds at night, dozens of glowing white-gold wings unfurling
in sequence like a chain reaction, pillars of holy light shooting into the sky, slow
orbital crane camera rising, breathtaking scale
```

### S4 — 英雄展示・聖劍武姬｜0:17–0:23

- **畫面**：¾ 身構圖，銀白盔甲女武神緩緩拔出光之聖劍，劍身符文亮起，羽翼微震
- **運鏡**：緩慢推近 + 淺景深
- **字幕**：收集傳說天使
- **備註**：此鏡與 S5 建議用 Midjourney/SD 先產 keyframe 圖，再走 image-to-video 保角色一致

```
Three-quarter shot of a anime valkyrie angel in ornate silver-white armor slowly
drawing a radiant holy sword, glowing runes igniting along the blade, her luminous
wings subtly trembling, shallow depth of field, slow push-in camera, heroic pose
```

### S5 — 英雄展示・聖詠法姬｜0:23–0:28

- **畫面**：另一位法師型天使雙手凝聚全息魔法陣，六邊形光紋（隱喻鏈上資產）環繞旋轉
- **運鏡**：環繞中景
- **字幕**：打造你的專屬軍團

```
Medium shot of an anime mage angel conjuring a holographic magic circle between her
palms, rotating hexagonal light sigils orbiting her, glowing circuit-like rune patterns,
hair and robes flowing in ethereal wind, slow orbiting camera
```

### S6 — 戰鬥蒙太奇｜0:28–0:34

- **畫面**：快剪——天使戰士俯衝穿越雲層、光矛與暗影魔物對撞、羽毛與火星四濺
- **運鏡**：手持動感跟拍、快速剪接（建議拆成 2 段生成再剪）
- **字幕**：率領她們，征戰天界戰場
- **音效**：鼓點加速、金屬撞擊

```
Dynamic action shot, anime angel warriors diving through clouds at high speed wielding
spears of light, clashing against shadow demons, feathers and golden embers scattering,
motion blur, handheld dynamic camera tracking the dive, intense battle energy
```

### S7 — Web3 資產層｜0:34–0:41

- **畫面**：英雄周圍浮現全息 NFT 卡牌框，一顆發光 token 硬幣翻轉，背景浮現六邊形區塊鏈網格延伸至地平線
- **運鏡**：繞主體半弧 + 前推
- **字幕**：每一位天使，都是真正屬於你的資產｜PLAY · OWN · EARN
- **備註**：卡牌框內容留空，後製疊實際卡面

```
An anime angel hero standing as holographic trading card frames materialize and float
around her, a glowing golden token coin spinning in slow motion in the foreground, a
vast hexagonal blockchain grid extending to the horizon behind, semi-arc camera move
pushing forward, futuristic Web3 aesthetic
```

### S8 — End Card「天使再臨」｜0:41–0:50

- **畫面**：漫天羽毛向中心匯聚成軍團徽章光印，定格為 Logo 位；下方浮現 Coming Soon
- **運鏡**：粒子向心收束 → 定格
- **字幕**：天使軍團 WEB3｜Coming Soon｜{社群連結}
- **音效**：合唱最高音 + 光印定音

```
Countless glowing white feathers and light particles converging toward the center of
a dark sky, assembling into a radiant circular legion emblem made of light, the emblem
pulsing once then holding still, slow majestic camera hold, triumphant finale mood
```

---

## 工具路由建議

| 鏡頭 | 建議工具 | 模式 |
|---|---|---|
| S1, S2, S8（抽象/粒子系） | Kling 2.x / Sora / Hailuo | text-to-video |
| S3, S6（大場面/動作） | Kling 2.x / Runway Gen-4 | text-to-video |
| S4, S5, S7（角色一致性重要） | Runway / Kling / Pika | **image-to-video**（先生成 keyframe 圖） |

**角色一致性流程**：Midjourney/SD 用同一 seed 或 character reference 產 S4/S5/S7 的靜帧 → 以圖生影片，prompt 保留動作描述、刪除外觀描述。

## 後製規範

- 剪接：CapCut / Premiere；各鏡生成 5–6s，S6 剪成 2–3 段快切
- 字幕：思源黑體 Heavy / 金色描邊 + 淡青紫光暈，置底安全區
- Logo / token 符號 / 卡面：PNG 疊加，勿讓 AI 生成
- 音樂方向：史詩管弦 + 電子低頻（參考：Two Steps From Hell 風格），音效點位已標於各鏡
- 調色：壓暗部偏紫、高光偏金，統一 8 鏡色調

## 30s 精簡版（廣告投放用）

取 S1 → S3 → S6 → S7 → S8，各鏡剪至 3–5s，字幕只留：標題 / PLAY·OWN·EARN / Coming Soon。
