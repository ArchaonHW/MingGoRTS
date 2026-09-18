# 天使軍團 WEB3 — MiniMax-H3 生成 Prompts（T2VA，16:9）

配合主腳本 `web3-angel-legion-trailer-16x9.md` 使用。已改寫為 H3 原生三段式格式
（`integrated_multimodal_description` / `overall_soundscape` / `non_diegetic_music`），
依 `skills/h3-prompt-writing/references/base-en.txt` 規範撰寫。

- **共 5 次生成**（≤15s/次、24 FPS、自帶 32kHz 立體聲——音樂音效由模型生成，免配樂）
- **模式**：T2VA（純文字）；若之後產 keyframe 圖走 I2VA/Ref2VA，格式見 `references/ref-en.txt`
- **字幕與 Logo 一律後製**，prompt 不含任何文字
- API 欄位：`task: "t2va"`、`aspect_ratio: "16:9"`、`duration_seconds`、prompt 貼下文

---

## Gen A — Cold Open「墜落之光 → 覺醒」｜duration_seconds: 10

```
integrated_multimodal_description: [Shot 1] 3D CG anime style, macro shot in a dark cosmic void: a single luminous white-gold angel feather drifts downward through the frame, leaving a thin trail of golden particles, a faint violet nebula glowing far in the background; the camera pushes in with small amplitude at slow speed, tracking the feather's slow spiral descent. [Shot 2] At 00:05.000, the camera cuts to an extreme close-up of a beautiful anime angel warrior's closed eyes with silver eyelashes; her eyes snap open revealing glowing golden irises as a burst of radiant light reflects in the pupil and light particles erupt outward.

overall_soundscape: A deep low drone hums beneath the void with a soft shimmering chime following the feather's fall, then a sharp resonant whoosh bursts with the flash of the opening eyes.

non_diegetic_music: Sparse sustained strings at a slow tempo, joined by a rising choir swell that crescendos into the eye-opening flash.
```

## Gen B — 軍團現身｜duration_seconds: 12

```
integrated_multimodal_description: [Shot 1] 3D CG anime style, epic wide aerial shot above a night sea of clouds: dozens of anime angel warriors in silver-white armor stand in ranks on floating crystalline platforms; starting from the front rank, glowing white-gold wings unfurl in sequence like a chain reaction sweeping through the legion, pillars of holy light shooting into the violet sky; the camera performs an arc shot with large amplitude at slow speed, rising as it orbits the army. [Shot 2] At 00:08.000, the camera cuts to a low-angle shot looking up past the warriors' silhouettes as the light pillars converge overhead into a single blinding column.

overall_soundscape: Deep rumbling wind rolls over the cloud sea, layered with sequential metallic wing-unfurling sweeps and a rising resonant hum as each light pillar ignites.

non_diegetic_music: A full epic choir enters at a moderate tempo over heavy taiko drums, building to a powerful sustained climax.
```

## Gen C — 英雄展示・武姬+法姬｜duration_seconds: 11

```
integrated_multimodal_description: [Shot 1] 3D CG anime style, three-quarter shot: a valkyrie angel in ornate silver-white armor slowly draws a radiant holy sword, glowing cyan runes igniting one by one along the blade, her luminous wings trembling subtly; shallow depth of field; the camera pushes in with small amplitude at slow speed. [Shot 2] At 00:05.500, the camera cuts to a medium shot of a second mage-type angel in flowing white-and-gold robes conjuring a holographic magic circle between her palms, rotating hexagonal light sigils orbiting her as her hair and sleeves flow in ethereal wind; the camera performs an arc shot with small amplitude around her.

overall_soundscape: A drawn metallic sword ring echoes as runes ignite with soft electric crackles; in the second half, layered crystalline chimes and a low magical hum pulse with each rotating sigil.

non_diegetic_music: Heroic brass phrases at a moderate tempo over driving string ostinatos, carrying momentum across the cut.
```

## Gen D — 戰鬥蒙太奇｜duration_seconds: 8

```
integrated_multimodal_description: [Shot 1] 3D CG anime style, tracking shot at fast speed: three winged angel warriors dive through night clouds at high speed with light spears leveled, motion blur streaking their wakes; the camera shakes slightly while tracking the lead diver. [Shot 2] At 00:03.000, the camera cuts to a mid-air clash where a light spear collides with a shadow demon's dark blade, erupting in gold sparks and scattering feathers. [Shot 3] At 00:05.500, the camera cuts to a wide shot of the sky battle, several angels wheeling between cloud pillars as blooms of light explode across the frame.

overall_soundscape: Rushing wind and whipping fabric during the dives give way to a sharp metal-on-metal impact, crackling energy discharges, and deep distant booms rolling across the sky.

non_diegetic_music: Aggressive taiko drum patterns at a fast tempo with staccato brass hits landing on each cut.
```

## Gen E — Web3 資產層 → End Card｜duration_seconds: 10

```
integrated_multimodal_description: [Shot 1] 3D CG anime style, medium shot: an angel hero stands calmly as translucent holographic trading-card frames materialize one by one floating around her, each rim glowing cyan; in the foreground a golden token coin spins slowly in mid-air; behind her a vast hexagonal light grid extends to the horizon; the camera performs an arc shot with small amplitude while pushing forward. [Shot 2] At 00:05.500, the camera cuts to a wide dark sky where countless glowing white feathers and light particles converge toward the center, assembling into a radiant circular legion emblem made of light; the emblem pulses once and holds still as the camera settles into a static shot.

overall_soundscape: Soft electronic chimes mark each card frame materializing, a rising metallic shimmer follows the spinning coin, then a sweeping cascade of crystalline tones as feathers converge, ending in a single resonant pulse.

non_diegetic_music: The epic choir-and-taiko theme returns at full force, resolving on a sustained triumphant final chord.
```

---

## 呼叫範例（本地部署或相容端點）

```bash
curl -X POST http://localhost:30010/v1/videos \
  -H 'Content-Type: application/json' \
  -d '{
    "task": "t2va",
    "prompt": "<上方 Gen A 的 prompt 整段貼入>",
    "conditions": [],
    "target": {"short_edge": 768, "aspect_ratio": "16:9", "duration_seconds": 10},
    "seed": 0
  }'
```

參考 `C:\HWC\MiniMax-H3-main\scripts\readme\reproducible-768p-t2va-request.sh`；
open-platform API 版本見同目錄 `full-2k-t2va-reference-*-open-platform-api.sh`。

## 部署現實檢查（本機）

- **權重未下載**：repo 內只有 config/index，transformer 33B BF16 + Qwen3-VL-32B encoder 合計約百 GB
- **本機無 NVIDIA GPU**（`nvidia-smi` 不存在）→ 本地推理不可行
- **建議路線**：hailuoai.video 網頁版或 platform.minimax.io API 直接生成，prompt 照貼
- **注意**：開源版僅含推理（H3-Base）；H3-Context-IR 與 Regenerate-2K 未開源，**無訓練程式碼**——若目標是 fine-tune/自訓練，需另尋訓練框架（如 diffusers 自訓 loop）且需多卡 GPU

## 一致性提示

- 跨生成角色一致：先用圖像模型產 2–3 張角色定妝圖，之後改用 **Ref2VA**（全參考模式，≤9 圖），prompt 結構見 `references/ref-en.txt` 六段式
- 風格錨點已統一為 `3D CG anime style` + 紫金聖光 + 全息六邊形元素；每次生成的 Shot 1 開頭都重述風格
- seed 可固定同一值重抽，或換 seed 抽多版本挑片
