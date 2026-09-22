#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
gen_cpp_tutorial.py — 依 MingGoRTS 開發實戰經驗生成 C++ 教學影片。

流程：
  1. PIL + NotoSansTC 渲染 1920x1080 投影片（暗色主題）
  2. Windows SAPI (Microsoft Hanhan, zh-TW) 產生每頁旁白 WAV
  3. ffmpeg 逐頁合成片段（時長 = 旁白 + 緩衝），concat 成最終 mp4

輸出：output/videos/cpp_dev_pitfalls.mp4
工作檔：output/tutorial/（frames/ wavs/ segs/）
"""

import json
import subprocess
import sys
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "output" / "tutorial"  # 每集再分子目錄（frames/ wavs/ segs/）

W, H = 1920, 1080
FPS = 30
AUDIO_PAD = 0.7  # 旁白結束後畫面停留秒數

FONT_DIR = ROOT / "assets" / "fonts"
FONT_BOLD = FONT_DIR / "NotoSansTC-Bold.otf"
FONT_REG = FONT_DIR / "NotoSansTC-Regular.otf"
FONT_MONO_CANDIDATES = [
    Path("C:/Windows/Fonts/consola.ttf"),
    Path("C:/Windows/Fonts/cour.ttf"),
    FONT_REG,
]

BG = (22, 25, 32)          # 深夜墨色
PANEL = (33, 37, 48)       # 內容面板
ACCENT = (201, 162, 39)    # 金
TEXT = (232, 230, 222)     # 米白
DIM = (150, 155, 165)      # 次要文字
CODE_BG = (15, 17, 22)     # 程式碼底色
CODE_FG = (180, 220, 180)  # 程式碼文字
TAG_COLORS = {
    "症狀": (255, 170, 90),
    "根因": (255, 110, 110),
    "解法": (120, 220, 140),
    "做法": (130, 190, 255),
    "細節": (255, 170, 90),
    "啟示": (120, 220, 140),
}


def pick_font(candidates, size):
    for p in candidates:
        if p.exists():
            return ImageFont.truetype(str(p), size)
    return ImageFont.load_default()


def wrap(draw, text, font, max_w):
    """依像素寬度折行（逐字元，適合中文混排）。"""
    lines, cur = [], ""
    for ch in text:
        if ch == "\n":
            lines.append(cur)
            cur = ""
            continue
        trial = cur + ch
        if draw.textlength(trial, font=font) > max_w and cur:
            lines.append(cur)
            cur = ch
        else:
            cur = trial
    if cur:
        lines.append(cur)
    return lines


def draw_slide(spec, idx, total, path):
    img = Image.new("RGB", (W, H), BG)
    d = ImageDraw.Draw(img)

    f_tag = pick_font([FONT_BOLD], 30)
    f_title = pick_font([FONT_BOLD], 64)
    f_sub = pick_font([FONT_REG], 36)
    f_head = pick_font([FONT_BOLD], 34)
    f_body = pick_font([FONT_REG], 33)
    f_code = pick_font(FONT_MONO_CANDIDATES, 30)
    f_code_cjk = pick_font([FONT_REG], 30)  # Consolas 無 CJK 字形，含中文的行改用 Noto
    f_foot = pick_font([FONT_REG], 24)

    # 頂部飾條
    d.rectangle([0, 0, W, 8], fill=ACCENT)

    if spec.get("kind") == "cover":
        # 封面
        d.rectangle([0, H // 2 - 190, W, H // 2 + 190], fill=PANEL)
        tw = d.textlength(spec["title"], font=f_title)
        d.text(((W - tw) / 2, H // 2 - 130), spec["title"], font=f_title, fill=ACCENT)
        for i, line in enumerate(spec["lines"]):
            lw = d.textlength(line, font=f_sub)
            d.text(((W - lw) / 2, H // 2 - 10 + i * 56), line, font=f_sub, fill=TEXT)
        img.save(path)
        return

    # 一般頁：坑位標籤 + 標題
    tag = spec.get("tag", "")
    if tag:
        tw = d.textlength(tag, font=f_tag)
        d.rectangle([60, 44, 60 + tw + 28, 92], fill=ACCENT)
        d.text((74, 52), tag, font=f_tag, fill=BG)
        title_x = 60 + tw + 52
    else:
        title_x = 60
    d.text((title_x, 36), spec["title"], font=f_title, fill=TEXT)
    d.line([60, 122, W - 60, 122], fill=ACCENT, width=2)

    # 內容區：sections 佔左 ~60%，程式碼放下半或右側
    y = 152
    content_w = W - 120
    for sec in spec.get("sections", []):
        head, body = sec
        color = TAG_COLORS.get(head, ACCENT)
        d.text((60, y), head, font=f_head, fill=color)
        y += 52
        for line in wrap(d, body, f_body, content_w):
            d.text((92, y), line, font=f_body, fill=TEXT)
            y += 46
        y += 18

    code = spec.get("code")
    if code:
        lines = code.strip("\n").split("\n")
        box_h = len(lines) * 42 + 32
        box_y = min(y + 10, H - box_h - 70)
        d.rectangle([60, box_y, W - 60, box_y + box_h], fill=CODE_BG,
                    outline=(70, 75, 88), width=2)
        for i, ln in enumerate(lines):
            font = f_code_cjk if any(ord(c) > 0x2E7F for c in ln) else f_code
            d.text((84, box_y + 16 + i * 42), ln, font=font, fill=CODE_FG)

    # 頁碼
    foot = f"{idx + 1} / {total}"
    fw = d.textlength(foot, font=f_foot)
    d.text((W - 60 - fw, H - 44), foot, font=f_foot, fill=DIM)
    img.save(path)


# ---------------- 內容（源自 AGENTS.md / 實戰紀錄） ----------------

SLIDES = [
    {
        "kind": "cover",
        "title": "從實戰學 C++",
        "lines": ["MingGoRTS / PotatoEngine 開發坑位實錄",
                  "八個用 bug 換來的教訓"],
        "narration": "這支影片整理了 MingGoRTS 專案在開發 PotatoEngine 時"
                     "真正踩過的八個 C++ 坑。每一個都曾經讓人追了半天。"
                     "我們直接從最痛的開始。",
    },
    {
        "tag": "坑位 1",
        "title": "MSBuild 增量建置的時間戳陷阱",
        "sections": [
            ("症狀", "原始碼明明改對了，跑出來的行為卻是舊版——測試一直紅。"),
            ("根因", "MSBuild 增量建置只比對時間戳。平行工具寫檔時保留了舊 mtime，"
                    "導致 .obj 比 .cpp 新、內容卻是舊版——你在跑「看不見的舊碼」。"),
            ("解法", "touch 該 .cpp 或直接 rebuild 該 target；"
                    "診斷可用 dumpbin /SYMBOLS 確認 .obj 是否引用預期符號。"),
        ],
        "code": 'dumpbin /SYMBOLS build\\Foo.obj | findstr ExpectedSymbol',
        "narration": "第一個坑：原始碼檢查正確，行為卻不符。"
                     "原因是 MSBuild 只比對時間戳——當物件檔的時間比原始碼新、"
                     "內容卻是舊版時，測試跑的是看不見的舊碼。"
                     "解法很簡單：touch 一下原始檔，或 rebuild 該 target。"
                     "要確認的話，用 dumpbin 看物件檔引用的符號。",
    },
    {
        "tag": "坑位 2",
        "title": "MinGW 的 libstdc++ DLL 錯配",
        "sections": [
            ("症狀", "git-bash 手編的 binary，-O0 能跑、-O1/-O2 直接 segfault。"),
            ("根因", "binary 依 PATH 載入 libstdc++-6.dll；Git 自帶的 mingw64/bin "
                    "若排在 scoop MinGW 前面，會載到版本不符的 DLL——"
                    "O0 僥倖通過，最佳化一開就炸。"),
            ("解法", "執行前把正確的 mingw/bin 提到 PATH 最前；"
                    "或連結時加 -static-libstdc++ -static-libgcc 一勞永逸。"),
        ],
        "code": 'g++ -O2 main.cpp -static-libstdc++ -static-libgcc -o main.exe',
        "narration": "第二個坑更陰險：最佳化等級決定會不會當掉。"
                     "原因不是程式碼，而是執行時載入了錯版本的 libstdc++ DLL——"
                     "Git Bash 自帶的 MinGW 排在前面就會出事。"
                     "解法：調整 PATH 順序，或直接靜態連結標準庫。",
    },
    {
        "tag": "坑位 3",
        "title": "禁用的 C 字串函式",
        "sections": [
            ("症狀", "CI 的 security-scan 直接擋下 build。"),
            ("根因", "gets / strcpy / strcat / sprintf / vsprintf / scanf "
                    "沒有邊界檢查，是 buffer overflow 的經典入口。"),
            ("解法", "一律改用有長度參數的版本：strncpy、snprintf、strncat。"
                    "注意 strncpy 不保證結尾 NUL——後面要自己補。"),
        ],
        "code": 'char buf[64];\nstrncpy(buf, src, sizeof(buf) - 1);\nbuf[sizeof(buf) - 1] = \'\\0\';',
        "narration": "第三個坑：老 C 函式。gets、strcpy、sprintf 這些沒有邊界檢查的函式，"
                     "在這個專案會被 CI 直接擋下。"
                     "改用 strncpy 和 snprintf，但要記得 strncpy 不保證補 NUL，"
                     "最後一個 byte 要自己設零。",
    },
    {
        "tag": "坑位 4",
        "title": "固定長度 char buffer 不能 = std::string",
        "sections": [
            ("症狀", "IDE 的 GUI state 用固定長度 char buffer，"
                    "直接指派 std::string 會編不過或截斷。"),
            ("根因", "ImGui 的 InputText 需要穩定的 char 指標與固定容量，"
                    "std::string 的內部緩衝區大小不受控。"),
            ("解法", "strncpy + 結尾裝 '\\0'，或先 memset 清乾淨再拷貝——"
                    "寫成工具函式，全專案統一走同一條路。"),
        ],
        "code": 'memset(state.developmentResponse, 0, sizeof(state.developmentResponse));\nstrncpy(state.developmentResponse, resp.c_str(),\n        sizeof(state.developmentResponse) - 1);',
        "narration": "第四個坑在 GUI 層：ImGui 要的是固定容量的 char buffer。"
                     "不能直接指派 std::string——要先 memset 清零，"
                     "再 strncpy 進去並保留 NUL 的空間。"
                     "我們後來把它抽成工具函式，避免每個地方重複踩。",
    },
    {
        "tag": "坑位 5",
        "title": "存檔一定要 tmp + rename",
        "sections": [
            ("症狀", "寫到一半當機，存檔變成截斷的半成品，下次載入直接壞掉。"),
            ("根因", "fopen/fwrite 是直接覆寫原檔——中途失敗就留下不一致狀態。"),
            ("解法", "先寫到 .tmp，成功後 rename 覆蓋。rename 在 NTFS/POSIX 都是原子操作，"
                    "讀方只看到舊版或完整新版。壞 schema 的檔案要拒載、不動現況。"),
        ],
        "code": 'WriteAll(path + ".tmp", bytes);   // 完整寫入暫存\nstd::rename((path + ".tmp").c_str(), path.c_str());  // 原子替換',
        "narration": "第五個坑關乎玩家的存檔。直接覆寫原檔，寫到一半出事就全毀。"
                     "正確做法是先寫暫存檔，確認完整之後用 rename 原子替換——"
                     "這個專案所有存檔路徑都走這個模式，"
                     "而且壞掉的 schema 一律拒絕載入，不動現有存檔。",
    },
    {
        "tag": "坑位 6",
        "title": "依賴方向只能單向",
        "sections": [
            ("症狀", "底層庫開始 include 上層的 header——循環依賴讓編譯順序失控。"),
            ("根因", "圖方便的「暫時反向依賴」一旦進來就拔不掉，"
                    "分層就變成一團漿糊。"),
            ("解法", "本專案硬性規定 PotatoEngine ← Gameplay ← Campaign。"
                    "下層需要上層資訊時，用介面注入或事件回呼，"
                    "而不是直接 include。"),
        ],
        "code": '// 依賴方向（箭頭 = 依賴）\nPotatoEngine  <-  Gameplay  <-  Campaign  <-  Examples',
        "narration": "第六個坑是架構層面的：依賴方向只能單向。"
                     "引擎不依賴玩法、玩法不依賴戰役。"
                     "下層需要上層的東西時，用介面注入或事件回呼。"
                     "一個暫時的反向 include，往往就是架構腐敗的開始。",
    },
    {
        "tag": "坑位 7",
        "title": "RL 輸出層必須是 linear",
        "sections": [
            ("症狀", "DQN 指揮官怎麼訓練都學不起來，Q 值永遠非負。"),
            ("根因", "輸出層用了 ReLU——負的 Q 值和 logits 全被截斷成 0，"
                    "網路在結構上表達不了「這步很糟」。"),
            ("解法", "PG / AC / DQN 的輸出層用 linear，讓 Q 值與 logits 無界；"
                    "激活函式只放在隱藏層。"),
        ],
        "code": 'net.AddLayer({hidden, 128, Activation::ReLU});\nnet.AddLayer({128, nActions, Activation::Linear});  // Q 值需無界',
        "narration": "第七個坑在強化學習。我們訓練 DQN 指揮官時，"
                     "Q 值永遠學不出負值——因為輸出層用了 ReLU，"
                     "負值在結構上就被截掉了。"
                     "記住：輸出層用 linear，激活函式留給隱藏層。",
    },
    {
        "tag": "坑位 8",
        "title": "平行開發的提交衛生",
        "sections": [
            ("症狀", "乾淨 checkout 之後 build 直接斷——CMakeLists 引用了"
                    "根本沒進版控的檔案。"),
            ("根因", "多 session 併行時 git add -A 把別人的半成品、"
                    "或漏掉了自己的新檔，都會污染歷史。"),
            ("解法", "提交前 git status 認清哪些是自己改的，逐檔 stage；"
                    "CMakeLists 引用的新檔必須同 commit 入帳。"),
        ],
        "code": 'git add Gameplay/Foo.cpp Gameplay/Foo.h CMakeLists.txt\n# 不用 -A；未追蹤檔被引用 = 乾淨 checkout 斷 build',
        "narration": "最後一個坑是協作面的。這個 repo 常常有多個 session 並行，"
                     "提交前一定要認清哪些檔是自己改的，逐檔 stage，不要用 git add dash A。"
                     "特別注意：CMakeLists 引用了還沒入帳的新檔，"
                     "別人乾淨 checkout 就會斷 build。",
    },
    {
        "kind": "cover",
        "title": "坑位清單",
        "lines": ["時間戳不等於內容 · DLL 版本看 PATH · C 函式要帶長度",
                  "buffer 記得補 NUL · 存檔走 tmp+rename · 依賴只許單向",
                  "輸出層用 linear · 提交只加自己的檔"],
        "narration": "八個坑總結在這裡。它們共同的教訓是："
                     "工具的假設永遠要驗證，邊界永遠要自己守住。"
                     "希望這些用 bug 換來的經驗，能幫你少追幾個小時。",
    },
]


def render_slides(slides, frames_dir):
    frames_dir.mkdir(parents=True, exist_ok=True)
    total = len(slides)
    for i, spec in enumerate(slides):
        p = frames_dir / f"slide_{i:02d}.png"
        draw_slide(spec, i, total, p)
    print(f"[frames] {total} slides -> {frames_dir}")


def synth_narration(slides, wavs_dir, work_dir):
    wavs_dir.mkdir(parents=True, exist_ok=True)
    durs = []
    manifest = work_dir / "narration.json"
    manifest.write_text(
        json.dumps([s["narration"] for s in slides], ensure_ascii=False),
        encoding="utf-8")
    ps = r'''
Add-Type -AssemblyName System.Speech
$texts = Get-Content -Raw -Encoding UTF8 "{manifest}" | ConvertFrom-Json
$out = "{wavs}"
for ($i = 0; $i -lt $texts.Count; $i++) {{
    $s = New-Object System.Speech.Synthesis.SpeechSynthesizer
    $s.SelectVoice("Microsoft Hanhan Desktop")
    $s.Rate = 0
    $f = Join-Path $out ("narr_{{0:d2}}.wav" -f $i)
    $s.SetOutputToWaveFile($f)
    $s.Speak($texts[$i])
    $s.Dispose()
}}
'''.format(manifest=str(manifest).replace("\\", "/"),
           wavs=str(wavs_dir).replace("\\", "/"))
    subprocess.run(["powershell", "-NoProfile", "-Command", ps], check=True)
    return measure_durations(slides, wavs_dir)


def measure_durations(slides, wavs_dir):
    durs = []
    for i in range(len(slides)):
        w = wavs_dir / f"narr_{i:02d}.wav"
        dur = float(subprocess.run(
            ["ffprobe", "-v", "quiet", "-show_entries", "format=duration",
             "-of", "csv=p=0", str(w)],
            capture_output=True, text=True, check=True).stdout.strip())
        durs.append(dur + AUDIO_PAD)
    print(f"[tts] {len(durs)} wavs, total {sum(durs):.1f}s")
    return durs


def encode(durs, frames_dir, wavs_dir, segs_dir, work_dir, final):
    segs_dir.mkdir(parents=True, exist_ok=True)
    final.parent.mkdir(parents=True, exist_ok=True)
    seg_list = []
    for i, dur in enumerate(durs):
        seg = segs_dir / f"seg_{i:02d}.mp4"
        subprocess.run([
            "ffmpeg", "-y", "-loop", "1", "-framerate", str(FPS),
            "-i", str(frames_dir / f"slide_{i:02d}.png"),
            "-i", str(wavs_dir / f"narr_{i:02d}.wav"),
            "-af", "apad", "-t", f"{dur:.2f}",
            "-vf", "format=yuv420p",
            "-c:v", "libx264", "-preset", "medium", "-crf", "20",
            "-c:a", "aac", "-b:a", "128k", "-ar", "44100",
            "-movflags", "+faststart", str(seg)],
            check=True, capture_output=True)
        seg_list.append(seg)
    concat = work_dir / "concat.txt"
    concat.write_text(
        "".join(f"file '{s.as_posix()}'\n" for s in seg_list), encoding="utf-8")
    subprocess.run([
        "ffmpeg", "-y", "-f", "concat", "-safe", "0", "-i", str(concat),
        "-c", "copy", "-movflags", "+faststart", str(final)],
        check=True, capture_output=True)
    print(f"[done] {final} ({final.stat().st_size / 1e6:.1f} MB)")


# ---------------- 第二集：資料完整性設計 ----------------

SLIDES_EP2 = [
    {
        "kind": "cover",
        "title": "資料即真相",
        "lines": ["從實戰學 C++ 第二集",
                  "存檔 · 帳本 · 回放的完整性設計"],
        "narration": "第二集換個角度——不講踩坑，講設計。"
                     "MingGoRTS 把存檔、帳本和回放做成了一套可以自我驗證的資料系統。"
                     "原則只有一句話：資料即真相，顯示只是視圖。",
    },
    {
        "tag": "設計 1",
        "title": "回放錄的是事件，不是畫面",
        "sections": [
            ("做法", "BattleRecorder 錄的是事件流：每個 tick 發生了什麼、"
                    "參數是什麼。record-is-truth——顯示只是視圖。"),
            ("細節", "回放就是重放狀態轉移，能停在任意 tick 檢查中間態；"
                    "bug 重現不需要「那個時候的螢幕」。"),
            ("啟示", "除錯、回放、審計共用同一份真相——"
                    "不用為三個需求寫三套系統。"),
        ],
        "code": 'recorder.Record(tick, Event{kind, payload});\n// 回放 = 依序重放事件 → 重建任意時點狀態',
        "narration": "第一個設計：回放錄的是事件，不是畫面。"
                     "BattleRecorder 記下每個 tick 的事件與參數，"
                     "回放就是重放狀態轉移——能停在任何時間點看中間態。"
                     "除錯、回放、審計因此共用同一份真相。",
    },
    {
        "tag": "設計 2",
        "title": "雜湊鏈帳簿 LedgerChain",
        "sections": [
            ("做法", "每筆 entry 帶前一筆的 FNV-1a 雜湊，append-only——"
                    "歷史只能往後長，不能回頭改。"),
            ("細節", "Verify() 從頭走一遍：任何一筆被動過，鏈就在那裡斷開，"
                    "斷點位置直接指出竄改處。"),
            ("啟示", "不需要區塊鏈等級的基礎建設——一條雜湊鏈"
                    "就能回答「這本帳有沒有被動過」。"),
        ],
        "code": 'entry.hash = FNV1a(prev.hash + entry.bytes);\nbool ok = chain.Verify();  // 斷鏈即竄改',
        "narration": "第二個設計是雜湊鏈帳簿。每一筆帳都咬住前一筆的雜湊值，"
                     "只能 append、不能回頭改。"
                     "Verify 從頭走一遍，哪裡斷鏈就是哪裡被動過手腳。"
                     "不需要區塊鏈等級的東西，一條雜湊鏈就夠了。",
    },
    {
        "tag": "設計 3",
        "title": "檔頭放完整性根 rootHash",
        "sections": [
            ("做法", "回放檔 potato.battle_replay/1 的檔頭帶整檔的"
                    "雜湊根 rootHash——載入前先驗。"),
            ("細節", "hash 對不上 → 拒載；版本是舊的 → 走降級路徑並警告。"
                    "兩種異常分開處理，不混為一談。"),
            ("啟示", "「回放即審計」：同一個機制同時服務"
                    "正確性檢查與防篡改偵測。"),
        ],
        "code": 'if (file.rootHash != ComputeHash(file))  reject();     // 篡改\nelse if (file.version < CURRENT)        migrate();   // 降級載入',
        "narration": "第三個設計：檔頭放完整性根。回放檔帶一個涵蓋整檔的 rootHash，"
                     "載入前先驗——被改過就拒載，舊版本則走降級路徑。"
                     "兩種異常分開處理：篡改是安全問題，舊版只是相容問題。"
                     "回放因此本身就是審計工具。",
    },
    {
        "tag": "設計 4",
        "title": "複式記帳：借貸必相等",
        "sections": [
            ("做法", "Ledger 五帳戶：武功、民心、天命、軍威、物資——"
                    "每筆記錄借貸兩方，總和必相等。"),
            ("細節", "SoundnessViolation() 掃不平衡的 entry——"
                    "假帳在結構上就站不住，不用等對帳才發現。"),
            ("啟示", "把會計恆等式變成程式 invariant："
                    "違反不變量的資料在寫入當下就現形。"),
        ],
        "code": 'ledger.Post({debit: "軍威", credit: "武功", amount});\n// Σdebit ≡ Σcredit，否則 SoundnessViolation()',
        "narration": "第四個設計直接搬了會計學：五個帳戶，借貸必相等。"
                     "武功、民心、天命、軍威、物資——每一筆都有對應的兩方。"
                     "不平衡的記錄在結構上就站不住，"
                     "SoundnessViolation 一掃就現形。"
                     "把恆等式變成 invariant，錯誤會自己發聲。",
    },
    {
        "tag": "設計 5",
        "title": "schema 一律版本化",
        "sections": [
            ("做法", "所有持久檔帶 potato.<name>/<ver> 標頭——"
                    "卡片、地圖、回放、名冊、帳簿都一樣。"),
            ("細節", "認識的舊版 → 降級載入；不認識的 schema → 拒絕，"
                    "而且不動現況。版本欄位是廉價的保險。"),
            ("啟示", "「拒絕且不動現況」很重要——"
                    "壞檔不該有機會污染好存檔。"),
        ],
        "code": '{ "schema": "potato.ledger_chain/1", ... }\n// 舊版降級、壞檔拒載——版本欄位是廉價的保險',
        "narration": "第五個設計是紀律問題：所有持久檔都帶 schema 版本。"
                     "認識的舊版走降級，不認識的直接拒絕、而且不動現況。"
                     "版本欄位很便宜，相容性事故很貴。"
                     "這條規矩從第一天就立下，後面全部受用。",
    },
    {
        "tag": "設計 6",
        "title": "把「做假帳」做成機制",
        "sections": [
            ("做法", "InjectForgery 是對手注入偽帳的正式通道；"
                    "MarkSuspect 的疑帳標記隨 \"suspect\" 欄位持久化。"),
            ("細節", "假帳不是 bug，是玩法——玩家要用 Verify 和"
                    "史官戰報的查帳段把它抓出來。"),
            ("啟示", "完整性機制反過來就是攻擊面——"
                    "防守做好了，進攻本身就是遊戲內容。"),
        ],
        "code": 'chain.InjectForgery(fakeEntry);   // 對手的攻擊面\nentry.MarkSuspect();              // \"suspect\": true 隨檔持久',
        "narration": "第六個設計最有趣：把做假帳做成遊戲機制。"
                     "InjectForgery 是對手注入偽帳的正式通道，"
                     "被標記的疑帳會隨檔案持久化。"
                     "玩家要用驗證工具和戰報查帳段把假帳抓出來——"
                     "完整性防守反過來就是玩法的攻擊面。",
    },
    {
        "tag": "設計 7",
        "title": "省略要明說",
        "sections": [
            ("做法", "HistorianReport 的省略計數恆在場——"
                    "「本報告省略 N 項」永遠印得出來。"),
            ("細節", "戰報查帳段直接消費 LedgerChain；"
                    "省略不是隱瞞，是註記。"),
            ("啟示", "系統對讀者隱藏了多少東西，"
                    "自己應該有能力回答——這也是完整性的一部分。"),
        ],
        "code": 'report.Footer() = "本報告省略 " + omitted + " 項";\n// 省略是註記，不是隱瞞',
        "narration": "第七個設計很小但很重要：省略要明說。"
                     "史官戰報永遠帶著「本報告省略幾項」的計數。"
                     "讀者知道有多少東西沒寫進來——省略是註記，不是隱瞞。"
                     "系統藏了多少資訊，自己要有能力回答。",
    },
    {
        "tag": "設計 8",
        "title": "完整性是三層防線",
        "sections": [
            ("做法", "寫入原子（tmp+rename）＋格式版本（schema）"
                    "＋內容雜湊（rootHash / 雜湊鏈）。"),
            ("細節", "三層各自獨立：寫一半當機由第一層擋，"
                    "格式演進由第二層接，惡意竄改由第三層抓。"),
            ("啟示", "完整性不是一個功能，"
                    "是一組互相覆蓋的防線——單層都有死角。"),
        ],
        "code": 'WriteTmp → Rename      // 層一：寫入原子\nschema/version        // 層二：格式版本\nrootHash / chain      // 層三：內容雜湊',
        "narration": "最後把前面的東西收攏：完整性是三層防線。"
                     "寫入層用 tmp 加 rename 擋當機，"
                     "格式層用 schema 版本接演進，"
                     "內容層用 rootHash 和雜湊鏈抓竄改。"
                     "每一層都有死角，疊起來才完整。",
    },
    {
        "kind": "cover",
        "title": "第二集清單",
        "lines": ["錄事件不錄畫面 · 雜湊鏈只許 append · 檔頭放完整性根",
                  "借貸必相等 · schema 版本化 · 假帳也是內容",
                  "省略要明說 · 三層防線互相覆蓋"],
        "narration": "第二集的八個設計收在這裡。共同主題只有一個："
                     "真相只存一份，而且它能自我驗證。"
                     "下一集我們聊架構分層與無頭測試——"
                     "怎麼讓玩法層完全不需要顯卡也能驗證。",
    },
]

# ---------------- 第三集：架構分層與無頭測試 ----------------

SLIDES_EP3 = [
    {
        "kind": "cover",
        "title": "沒有顯卡也要能測",
        "lines": ["從實戰學 C++ 第三集",
                  "架構分層與無頭測試實錄"],
        "narration": "第三集談測試的物理條件：怎麼讓整個玩法層"
                     "不需要顯卡、不需要視窗就能跑。"
                     "答案是分層——而且每一層的依賴方向都不可妥協。",
    },
    {
        "tag": "設計 1",
        "title": "玩法層全部 headless",
        "sections": [
            ("做法", "Gameplay 層不碰 OpenGL、不建 context——"
                    "FlowField、Doctrine、BattleController 全是純邏輯。"),
            ("細節", "渲染需要的資料經介面送出（BattleSceneSync），"
                    "玩法層自己不知道畫面的存在。"),
            ("啟示", "headless 不是測試技巧，是分層的副產品——"
                    "邏輯不依賴呈現，測試自然無頭。"),
        ],
        "code": '// Gameplay/ 內禁止：\n#include <GL/gl.h>        // ← 出現就是分層破洞\nglfwCreateWindow(...)   // ← 同上',
        "narration": "第一個設計：玩法層全部無頭。"
                     "Gameplay 裡面不准出現 GL 標頭、不准建 context。"
                     "渲染層需要的資料用介面送出去——玩法層不知道畫面存在。"
                     "能做到無頭測試，是分層做對了的副產品。",
    },
    {
        "tag": "設計 2",
        "title": "三拍狀態機讓戰鬥可單步",
        "sections": [
            ("做法", "BattleController 三拍：Planning → Execution → Aftermath，"
                    "每拍是離散狀態轉移。"),
            ("細節", "離散拍 = 天然的檢查點：測試可以停在任意拍斷言中間態，"
                    "不用追連續時間軸。"),
            ("啟示", "把連續系統切成離散階段，"
                    "可測性是自己長出來的。"),
        ],
        "code": 'enum class Beat { Planning, Execution, Aftermath };\ncontroller.Step();   // 一拍一拍推進，每拍後可斷言',
        "narration": "第二個設計：三拍狀態機。戰鬥切成規劃、執行、結算三拍，"
                     "每一拍是離散的狀態轉移。"
                     "測試可以一拍一拍推進，在任何一拍後面放斷言——"
                     "不用追連續時間軸，可測性是自己長出來的。",
    },
    {
        "tag": "設計 3",
        "title": "POTATO_TESTS：註冊即紀律",
        "sections": [
            ("做法", "CTest 啟用，測試執行檔集中登記在根 CMakeLists 的"
                    "POTATO_TESTS 清單——加進去就自動註冊。"),
            ("細節", "驗證的定義寫死了：建置 + ctest -C Release 全綠。"
                    "沒有「我這邊有過」的空間。"),
            ("啟示", "測試基礎設施的門檻越低，"
                    "「順手補個測試」越會真的發生。"),
        ],
        "code": 'set(POTATO_TESTS ... MyNewTest)  # 加名字即註冊\ncd build && ctest -C Release   # 全綠才算完',
        "narration": "第三個設計是流程面：POTATO_TESTS 清單。"
                     "測試執行檔加個名字就自動註冊進 CTest，"
                     "驗證的定義只有一條——建置加 ctest 全綠。"
                     "門檻夠低，順手補測試才會真的發生。",
    },
    {
        "tag": "設計 4",
        "title": "雙編譯器是免費的 lint",
        "sections": [
            ("做法", "本地必須 MSVC 與 MinGW 雙過；"
                    "CI 再用 Linux g++ 掃第三遍。"),
            ("細節", "編譯器之間的警告差異會抓出不同類別的 UB 和"
                    "未初始化——一個 codebase 兩種靜態分析。"),
            ("啟示", "MinGW 專用連結用 if(WIN32 AND NOT MSVC) 守衛，"
                    "不要把平台特例散進共用段落。"),
        ],
        "code": 'if(WIN32 AND NOT MSVC)\n    target_link_libraries(t PRIVATE psapi)  # MinGW 專用\nendif()',
        "narration": "第四個設計：雙編譯器驗證。MSVC 和 MinGW 都要過，"
                     "CI 再用 g++ 掃第三遍。"
                     "不同編譯器抓不同類別的未定義行為——等於免費的多重 lint。"
                     "平台特例用條件守衛收攏，別散進共用段落。",
    },
    {
        "tag": "設計 5",
        "title": "無頭 demo 就是回歸測試",
        "sections": [
            ("做法", "DoctrineBattleDemo、AutoPlannerDemo、DuanqiaoDemo"
                    "全是無頭執行檔——回傳非零即失敗。"),
            ("細節", "demo 同時是展示、文件與測試："
                    "想懂系統怎麼用就讀 demo，想知道有沒有壞就跑它。"),
            ("啟示", "與其寫會過期的文件，"
                    "不如寫會紅燈的 demo。"),
        ],
        "code": 'int main() {\n    RunDoctrineBattle();\n    return failures ? 1 : 0;   // 非零 = ctest 紅\n}',
        "narration": "第五個設計：demo 即測試。這個專案的玩法 demo 全是無頭執行檔，"
                     "回傳非零就讓 ctest 亮紅燈。"
                     "它同時是展示、文件和回歸測試——"
                     "與其寫會過期的文件，不如寫會報警的 demo。",
    },
    {
        "tag": "設計 6",
        "title": "RL 訓練要完全可重現",
        "sections": [
            ("做法", "DQNAgent::SeedWeights + SetSeed + seeded env——"
                    "BattleCommandEnv 包 BattleController 成 RLEnvironment。"),
            ("細節", "種子固定後整趟訓練逐 bit 可重現；"
                    "不確定的環境沒辦法比較兩次訓練的差異。"),
            ("啟示", "先能重現，才談得上調參——"
                    "隨機性要全部走 seed，一個都不放過。"),
        ],
        "code": 'agent.SeedWeights(42);\nagent.SetSeed(42);\nenv.SetSeed(42);   // 三處都要，漏一個就不可重現',
        "narration": "第六個設計在 AI 層：訓練要完全可重現。"
                     "權重初始化、agent 的亂數、環境的亂數——三個 seed 都要固定，"
                     "漏一個就不可重現。"
                     "先能重現，才談得上調參和比較。",
    },
    {
        "tag": "設計 7",
        "title": "單一 CMakeLists 的取捨",
        "sections": [
            ("做法", "全專案只有根目錄一份 CMakeLists——"
                    "所有 target 定義集中一處。"),
            ("細節", "好處是沒有「該加在哪層」的歧義；"
                    "代價是它是高碰撞熱區，平行開發要會拆 hunk。"),
            ("啟示", "新 target 的源檔必須同 commit 入帳——"
                    "引用未追蹤檔 = 乾淨 checkout 斷 build。"),
        ],
        "code": 'add_executable(MyTest Examples/MyTest.cpp)\n# 同一個 commit 必須包含 Examples/MyTest.cpp',
        "narration": "第七個設計是建置組織：全專案一份 CMakeLists。"
                     "target 集中一處，沒有該加哪層的歧義；"
                     "代價是它是碰撞熱區。"
                     "鐵律只有一條：新 target 的源檔要跟著同一個 commit 進來。",
    },
    {
        "tag": "設計 8",
        "title": "垂直切片驗收整個架構",
        "sections": [
            ("做法", "DuanqiaoPlayable：標題殼 → 章節選擇 → 戰鬥 → "
                    "史官戰報 → 整補 → 再戰，端對端可玩。"),
            ("細節", "它不是 demo 那麼簡單——是各層接縫的整合測試："
                    "Gameplay、Campaign、存檔、戰報全要走通。"),
            ("啟示", "package_demo.bat 產出自包含 dist 包——"
                    "能交給別人跑，才是真的做完。"),
        ],
        "code": 'tools\\package_demo.bat   →   dist/MingGoRTS-Demo/\n# 自包含：拷走就能跑，不依賴開發環境',
        "narration": "第八個設計是垂直切片。斷橋切片從標題畫面一路走到再戰，"
                     "戰鬥、戰役、存檔、戰報的接縫全部要打通。"
                     "它不是展示品，是整合測試。"
                     "而且用腳本打包成自包含目錄——能交給別人跑，才算真的做完。",
    },
    {
        "kind": "cover",
        "title": "第三集清單",
        "lines": ["玩法層不碰 GL · 離散拍可單步 · 註冊即紀律",
                  "雙編譯器免費 lint · demo 即測試 · 訓練全 seed",
                  "單一 CMakeLists · 垂直切片驗收接縫"],
        "narration": "第三集收在這裡。三集的共同主題："
                     "可測性、完整性、可維護性都不是事後補的，"
                     "是分層和紀律長出來的。這個系列先到這裡——"
                     "實戰繼續，坑位也會繼續。",
    },
]

# ---------------- 第四集：玩法系統實作 ----------------

SLIDES_EP4 = [
    {
        "kind": "cover",
        "title": "規則即內容",
        "lines": ["從實戰學 C++ 第四集",
                  "Doctrine · 尋路 · 情報霧的實作錄"],
        "narration": "第四集進入玩法層。MingGoRTS 的玩家不點選單位——"
                     "他們書寫規則。這集講八個把設計變成程式碼的實作："
                     "規則解譯器、群體尋路、機率情報霧。",
    },
    {
        "tag": "設計 1",
        "title": "Doctrine：玩家寫的是規則",
        "sections": [
            ("做法", "trigger → condition → action → modifier 四段式卡片；"
                    "戰鬥中由解譯器即時執行，每條規則有冷卻。"),
            ("細節", "規則是資料不是程式——卡片存在 assets/cards 的 "
                    "JSON，解譯器是唯一需要懂語法的地方。"),
            ("啟示", "把「玩家意圖」做成資料結構，"
                    "平衡性調整就變成改卡片，不是改引擎。"),
        ],
        "code": '{ "trigger": "enemy_in_range",\n  "condition": "morale > 40",\n  "action": "volley", "cooldown": 8 }',
        "narration": "第一個設計是這個遊戲的核心：Doctrine 卡解譯器。"
                     "玩家寫 trigger、condition、action 四段式規則，"
                     "戰鬥中由解譯器執行，每條有冷卻防止刷屏。"
                     "關鍵是規則是資料——平衡調整改的是 JSON，不是引擎。",
    },
    {
        "tag": "設計 2",
        "title": "FlowField：一次場算養整群",
        "sections": [
            ("做法", "群體尋路不用每單位 A*——從目標反向擴散一張"
                    "流場，所有單位查表走。"),
            ("細節", "場的代價是一次 BFS/Dijkstra；"
                    "之後每單位每拍只查一格，單位越多越划算。"),
            ("啟示", "「每個實體各算各的」是最貴的直覺——"
                    "把問題反過來從目標出發，常常直接降一個量級。"),
        ],
        "code": 'field.BuildFrom(goal);            // 一次反向擴散\nfor (auto& u : squad)\n    u.dir = field.DirAt(u.pos);  // 每單位只查表',
        "narration": "第二個設計是群體尋路。不讓每個單位各跑 A*，"
                     "而是從目標反向擴散一張流場，所有單位查表走。"
                     "場的成本是一次圖搜索，之後每單位每拍只查一格。"
                     "把問題從目標反過來想，常常直接降一個量級。",
    },
    {
        "tag": "設計 3",
        "title": "QuantumFog：敵軍是機率雲",
        "sections": [
            ("做法", "未觀測的敵軍不是「藏起來的棋子」，"
                    "是疊加態機率雲——觀測才塌縮成位置。"),
            ("細節", "疊加/觀測/探測/衰減/糾纏/人格先驗六個操作；"
                    "偵察不是開地圖，是把機率分布收窄。"),
            ("啟示", "情報不確定性做成一等公民後，"
                    "「被騙」變成有數學結構的事件，不是劇情殺。"),
        ],
        "code": 'fog.Superpose(posA, 0.6, posB, 0.4);\nfog.Probe(scout);        // 探測 → 分布收窄\nauto p = fog.Observe();  // 觀測 → 塌縮',
        "narration": "第三個設計最敢：敵軍是機率雲。"
                     "沒觀測到的部隊不是藏起來的棋子，是疊加態分布，"
                     "觀測才塌縮。偵察變成把機率收窄的數學操作。"
                     "被假情報騙到，因此是有結構的事件，不是劇情殺。",
    },
    {
        "tag": "設計 4",
        "title": "情報確定度即時縮放加成",
        "sections": [
            ("做法", "BattlePlan 計畫箭頭的加成不是固定值——"
                    "依當下情報確定度即時縮放。"),
            ("細節", "計畫畫在機率雲上：敵軍分布越不確定，"
                    "同一支箭頭的期望加成越低。"),
            ("啟示", "把「情報價值」直接變成數值迴圈——"
                    "偵察不再是有沒有做的差別，是划不划算的計算。"),
        ],
        "code": 'bonus = plan.base * fog.Certainty(plan.region);\n// 情報確定度 0..1 直接進加成公式',
        "narration": "第四個設計把兩個系統接起來：計畫加成乘以情報確定度。"
                     "敵軍分布越不確定，同一個計畫的期望回報越低。"
                     "情報價值直接進數值迴圈——偵察從「有沒有做」"
                     "變成「划不划算」的連續決策。",
    },
    {
        "tag": "設計 5",
        "title": "敵將是人格，不是難度滑桿",
        "sections": [
            ("做法", "EnemyGeneral 是人格腳本：謹慎/暴躁/多疑影響"
                    "trigger 偏好與沉不住氣的時機。"),
            ("細節", "人格同時是 QuantumFog 的先驗——"
                    "多疑的將領機率雲散得更開，讀將就等於讀分布。"),
            ("啟示", "AI 差異化靠行為分布，不靠數值灌水——"
                    "玩家讀懂人格就是讀懂對手。"),
        ],
        "code": 'general.prior = Personality::Suspicious;\n// 人格 → 機率雲先驗散度 + 腳本 trigger 偏好',
        "narration": "第五個設計：敵將是人格，不是難度滑桿。"
                     "謹慎、暴躁、多疑各自影響腳本偏好，"
                     "還同時是機率雲的先驗——多疑的將領分布散得更開。"
                     "讀懂敵將人格，就是讀懂機率分布本身。",
    },
    {
        "tag": "設計 6",
        "title": "模板配兵的誠實回饋",
        "sections": [
            ("做法", "SquadTemplate 巢狀 JSON → 小隊實例化；"
                    "BudgetedBuild 在點數不足時逐項跳過。"),
            ("細節", "skipReasons 與項目逐項對齊："
                    "unknown_id / over_budget——不是「配兵失敗」一句話。"),
            ("啟示", "批量操作的回饋要對齊到項目粒度，"
                    "玩家才知道該改哪一項。"),
        ],
        "code": 'result = tmpl.BudgetedBuild(points);\n// result.skipReasons[i] 對齊 items[i]——逐項交代',
        "narration": "第六個設計是工程細節：模板配兵的失敗回饋。"
                     "點數不夠時逐項跳過，skipReasons 與項目一一對齊——"
                     "不知道 ID 和超出預算是不同的原因。"
                     "批量操作的回饋要到項目粒度，玩家才知道改哪裡。",
    },
    {
        "tag": "設計 7",
        "title": "敘事是帶掛鉤的資料",
        "sections": [
            ("做法", "ChapterConventions 四聲部（題詞/判詞/欲知後事/結局）"
                    "與 MythLog 滲透掛鉤都是資料結構。"),
            ("細節", "神話事件具名入帳（potato.myth_log/1），"
                    "滲透狀態是 0–3 狀態機——敘事可以參與規則判定。"),
            ("啟示", "敘事系統化的門檻是「能被規則引用」——"
                    "一旦入帳，寫作就是寫資料。"),
        ],
        "code": 'mythLog.Record("狐仙托夢", Seepage::L2);\n// 具名事件 → 狀態機 → 可被 Doctrine condition 引用',
        "narration": "第七個設計：敘事是帶掛鉤的資料。"
                     "神話事件具名入帳，滲透是零到三的狀態機，"
                     "可以被規則的 condition 引用。"
                     "敘事一旦入帳，寫作就變成寫資料——"
                     "寫手和系統談的是同一個結構。",
    },
    {
        "tag": "設計 8",
        "title": "整補是戰鬥間的經濟迴圈",
        "sections": [
            ("做法", "RefitCamp 跨場整補：部署、醫治、招募、掠奪——"
                    "四個動作消耗不同資源。"),
            ("細節", "傷亡持久在名冊（Roster）上："
                    "戰鬥的代價跨場累積，整補是它的釋放閥。"),
            ("啟示", "兩場戰鬥之間不是選單，是經濟——"
                    "資源迴圈讓「打贏但傷亡慘重」成為真實代價。"),
        ],
        "code": 'camp.Deploy(squad); camp.Heal(injured);\ncamp.Recruit(template); camp.Plunder(convoy);',
        "narration": "第八個設計把戰鬥之間填上：整補是經濟迴圈。"
                     "部署、醫治、招募、掠奪各消耗不同資源，"
                     "而傷亡持久在名冊上——戰鬥的代價跨場累積。"
                     "兩場戰鬥之間不是選單，是資源管理。",
    },
    {
        "kind": "cover",
        "title": "第四集清單",
        "lines": ["規則是資料 · 場養整群 · 敵軍是機率雲",
                  "情報進公式 · 人格即先驗 · 失敗逐項交代",
                  "敘事帶掛鉤 · 戰間是經濟"],
        "narration": "第四集收在這裡。這集的共同主題："
                     "把抽象設計落成資料結構，玩法深度就來自資料之間的連結。"
                     "四集下來——坑位、完整性、測試、玩法——"
                     "這個專案的方法論都在這了。",
    },
]

# ---------------- 第五集：相容性工程（L-7 provenance 實錄） ----------------

SLIDES_EP5 = [
    {
        "kind": "cover",
        "title": "舊檔不變，新功能上鏈",
        "lines": ["從實戰學 C++ 第五集",
                  "L-7 分錄產生軌跡的相容性工程"],
        "narration": "第五集用一個今天剛落地的例子："
                     "幫帳簿分錄加上「產生軌跡」欄位——"
                     "既要讓新帳記下來源，又不能讓舊存檔的雜湊鏈斷掉。"
                     "這是相容性工程最典型的拉扯。",
    },
    {
        "tag": "設計 1",
        "title": "可選欄位勝過強制欄位",
        "sections": [
            ("做法", "schema 不升版：potato.ledger_chain/1 的每筆分錄"
                    "加一個**可選**的 prov 欄位。"),
            ("細節", "升版要動所有讀寫端；可選欄位只動需要它的人——"
                    "舊讀者略過，新讀者取用。"),
            ("啟示", "能加可選欄位就不要升版——"
                    "版本號是稀缺資源，加欄位是免費的。"),
        ],
        "code": '{ "debit":"Martial", "credit":"Supply", "amount":120,\n  "chapter":1, "note":"斷橋之捷",\n  "prov":{"source":"battle","tick":42,"event":"ev-x"} }',
        "narration": "第一個設計：schema 不升版。"
                     "給每筆分錄加一個可選的 prov 欄位——"
                     "舊讀者看不懂就略過，新讀者才取用。"
                     "能加可選欄位就不要升版，版本號是稀缺資源。",
    },
    {
        "tag": "設計 2",
        "title": "條件式 canonical：預設值不進雜湊",
        "sections": [
            ("做法", "Canon() 只在 prov 非預設時才附加 prov 段——"
                    "全預設值維持舊格式字串。"),
            ("細節", "舊格式 \"M|S|120|1|斷橋\" → 新格式 "
                    "\"M|S|120|1|斷橋|Battle|42|ev-x\"。"
                    "預設 prov 產生的是逐位相同的舊字串。"),
            ("啟示", "hash 的輸入格式本身就是相容性邊界——"
                    "控制 canon 就控制了哪些改變會「看得見」。"),
        ],
        "code": 'std::string Canon(e) {\n    s = debit|credit|amount|chapter|note;   // 舊格式\n    if (e.prov != Provenance{})              // 非預設才附加\n        s += "|" + prov.source + "|" + tick + "|" + eventId;\n    return s;\n}',
        "narration": "第二個設計是關鍵技巧：條件式 canonical。"
                     "雜湊的輸入字串，在 prov 全是預設值時維持舊格式——"
                     "舊存檔的 hash 一個 bit 都不變，Verify 照過。"
                     "控制了 canon 格式，就控制了哪些改變會被鏈看見。",
    },
    {
        "tag": "設計 3",
        "title": "副作用變特性：事後補登必斷鏈",
        "sections": [
            ("做法", "舊分錄若被事後補上 prov——canon 改變 → "
                    "hash 改變 → 鏈在該筆斷開。"),
            ("細節", "本來是「相容性約束」，實作後發現是防禦特性："
                    "竄改產生軌跡和竄改金額一樣會被抓。"),
            ("啟示", "設計時先問「這個限制的反面是什麼」——"
                    "它常常就是你要的保證。"),
        ],
        "code": '// 已上鏈分錄補 prov → canon 變 → hash 變\nchain.Verify();   // 回報該筆索引——斷鏈揭露',
        "narration": "第三個設計是意外收穫。條件式 canon 本來是相容性手段，"
                     "但它的反面是：對已上鏈的舊分錄事後補 prov，"
                     "字串變了、雜湊變了、鏈就斷在那一筆。"
                     "竄改產生軌跡跟竄改金額一樣會被抓——"
                     "限制反過來看就是保證。",
    },
    {
        "tag": "設計 4",
        "title": "降級與拒載是兩條路",
        "sections": [
            ("做法", "無 prov 的 /1 舊檔 → 降級載入（prov=Unknown）；"
                    "不認識的 schema → 拒絕且不動現況。"),
            ("細節", "「資料缺欄位」和「格式不認識」是不同故障："
                    "前者是時間差，後者是空間差——處理不能共用。"),
            ("啟示", "載入器的分叉點寫清楚："
                    "降級是服務，拒載是保護。"),
        ],
        "code": 'if (schema != "potato.ledger_chain/1") return {};  // 拒載\nauto prov = obj.Get("prov");   // 缺欄位 → Unknown，非錯誤',
        "narration": "第四個設計：降級與拒載分開。"
                     "缺 prov 欄位的舊檔走降級，來源標記為失考；"
                     "完全不認識的 schema 才拒絕。"
                     "資料缺欄位是時間差，格式不認識是空間差——"
                     "降級是服務，拒載是保護。",
    },
    {
        "tag": "設計 5",
        "title": "Unknown 是合法值，不是錯誤",
        "sections": [
            ("做法", "EntrySource 列舉第一個值就是 Unknown（失考）——"
                    "舊檔與手動分錄天然落在這裡。"),
            ("細節", "不對 prov 做必填強制：史官體直接寫「失考」——"
                    "查帳段照樣計數，缺來源也是資訊。"),
            ("啟示", "給「不知道」一個一等公民的值，"
                    "比用 null 或例外乾淨得多。"),
        ],
        "code": 'enum class EntrySource { Unknown, Battle, Refit,\n    Negotiation, Governance, Myth, System, Count };\n// Unknown → 史官體「失考」，照樣入查帳統計',
        "narration": "第五個設計：不知道也是值。"
                     "EntrySource 的第一個列舉就是失考——"
                     "舊檔、手動分錄天然落在這裡，不是錯誤。"
                     "給不知道一個一等公民的值，比 null 或例外乾淨。",
    },
    {
        "tag": "設計 6",
        "title": "偽帳可以偽造來源——而且允許",
        "sections": [
            ("做法", "InjectForgery 通道照常接受帶 prov 的分錄——"
                    "偽造來源是攻擊面的一部分。"),
            ("細節", "偵測責任不變：Verify 抓結構斷鏈，"
                    "查帳段與疑帳標記抓語意破綻——各管各的層。"),
            ("啟示", "防禦設計不是堵死所有路——"
                    "是讓每條路都有對應的偵測層。"),
        ],
        "code": 'chain.InjectForgery(entry.WithProv(faked));\n// 偽造 prov 合法進場——偵測在別層：斷鏈/疑帳/查帳統計',
        "narration": "第六個設計很反直覺：允許偽造來源。"
                     "偽帳通道照常收帶 prov 的分錄——因為偵測責任分層："
                     "Verify 管結構斷鏈，疑帳標記和查帳統計管語意破綻。"
                     "防禦不是堵死所有路，是每條路都有人看。",
    },
    {
        "tag": "設計 7",
        "title": "查帳段要會說「失考」",
        "sections": [
            ("做法", "史官查帳段附來源統計：戰陣 3、營務 1、失考 1——"
                    "Unknown 也計數。"),
            ("細節", "來源分布本身就是訊號：失考比例飆高"
                    "代表新流程沒接上或有人在降級舊格式。"),
            ("啟示", "統計輸出要含「其他/未知」桶——"
                    "沒有它，異常會被靜靜地藏進來。"),
        ],
        "code": '"本章記帳 5 筆，借貸相符——戰陣 3、營務 1、失考 1，武功增 120。"',
        "narration": "第七個設計在輸出端：查帳段把來源統計印出來，"
                     "失考也要計數。"
                     "來源分布本身就是訊號——失考比例飆高，"
                     "代表流程沒接上或有人在用舊格式。"
                     "統計一定要留一個未知桶，異常才不會靜靜地藏進來。",
    },
    {
        "tag": "設計 8",
        "title": "I/O 矩陣先行，實作殿後",
        "sections": [
            ("做法", "spec 先列 I/O 與邊界矩陣：模板帶源、剝除 prov、"
                    "舊檔降級、偽造 prov、查帳統計——每列一個斷言。"),
            ("細節", "LedgerProvenanceTest 31 個檢查點對齊矩陣，"
                    "MSVC + MinGW 雙過才算完。"),
            ("啟示", "相容性功能的測試成本在「組合」——"
                    "先窮舉組合再實作，比補測試便宜十倍。"),
        ],
        "code": 'Check(HashEntry(g, a) != HashEntry(g, c), "不同 source → 雜湊不同");\nCheck(Verify(old_chain) == -1,           "無 prov 舊檔 → 鏈完整");',
        "narration": "第八個設計是方法：I/O 矩陣先行。"
                     "規格先把場景列成表格——帶源、剝除、降級、偽造，"
                     "每列一個斷言，實作對著打勾。"
                     "相容性功能的成本在組合，先窮舉比事後補測試便宜十倍。"
                     "這項工作最後是三十一個檢查點，雙編譯器全綠。",
    },
    {
        "kind": "cover",
        "title": "第五集清單",
        "lines": ["可選欄位不升版 · 預設值不進雜湊 · 限制的反面是保證",
                  "降級與拒載分路 · 失考是合法值 · 偵測分層不堵路",
                  "未知桶要計數 · 矩陣先行"],
        "narration": "第五集收在這裡。相容性的核心心法："
                     "新功能是加給新讀者的，舊檔的世界一個 bit 都不動。"
                     "做到這一點，演化就不是風險，是常態。",
    },
]

# ---------------- 第六集：拆倉工程（engine/game split 實錄） ----------------

SLIDES_EP6 = [
    {
        "kind": "cover",
        "title": "monorepo 的離婚協議",
        "lines": ["從實戰學 C++ 第六集",
                  "引擎/遊戲拆倉的邊界固化實錄"],
        "narration": "第六集講一場正在進行的手術："
                     "把 PotatoEngine 從 MingGoRTS monorepo 裡拆出去。"
                     "重點不是搬檔案——是先讓邊界成立，歷史才切得開。",
    },
    {
        "tag": "設計 1",
        "title": "先固化邊界，再動歷史",
        "sections": [
            ("做法", "Phase 1 全部在 monorepo 內做：清耦合、修 CMake、"
                    "上守衛——filter-repo 是最後才碰的刀。"),
            ("細節", "邊界沒固化就切歷史，切出來的是兩個都編不過的 repo——"
                    "破壞要在還能 ctest 的地方先修完。"),
            ("啟示", "拆分順序：能驗證的地方先做破壞性修改，"
                    "不可逆操作（歷史重寫）放最後。"),
        ],
        "code": '# Phase 1 邊界固化（monorepo 內，可 ctest）\n# Phase 2 filter-repo（不可逆，最後做）\n# Phase 3 消費端接線',
        "narration": "第一個設計是順序：先在 monorepo 裡固化邊界，"
                     "歷史拆分放最後。"
                     "filter-repo 是不可逆操作——邊界沒固化就切，"
                     "切出來的是兩個都編不過的 repo。"
                     "能驗證的地方先做破壞性修改，不可逆的刀最後才落下。",
    },
    {
        "tag": "設計 2",
        "title": "CMAKE_SOURCE_DIR 的信任問題",
        "sections": [
            ("做法", "全檔 137 處 CMAKE_SOURCE_DIR 換成 "
                    "CMAKE_CURRENT_SOURCE_DIR。"),
            ("細節", "前者假設「我是頂層專案」；被 add_subdirectory "
                    "消費時指向的是別人的根——引擎的所有路徑全錯位。"),
            ("啟示", "寫 CMake 時每個 SOURCE_DIR 都在回答"
                    "「我是誰的根」——library 沒資格用絕對答案。"),
        ],
        "code": '# 拆倉前（假設自己是頂層）\n${CMAKE_SOURCE_DIR}/assets/...\n# 拆倉後（自己是誰都行）\n${CMAKE_CURRENT_SOURCE_DIR}/assets/...',
        "narration": "第二個設計是一百三十七處替換。"
                     "CMAKE_SOURCE_DIR 假設自己是頂層專案——"
                     "一旦被當成子目錄消費，它指向的是別人的根，"
                     "所有路徑全錯位。"
                     "library 寫 CMake 沒資格用絕對答案。",
    },
    {
        "tag": "設計 3",
        "title": "standalone 守衛：被消費時只剩 lib",
        "sections": [
            ("做法", "if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)"
                    "包住 examples/tests/install/CPack——"
                    "被消費時只產出 lib target。"),
            ("細節", "消費方要的是引擎本體；引擎自己的 demo、測試、"
                    "安裝規則在被引用時全是噪音甚至衝突源。"),
            ("啟示", "library 的 CMake 要回答兩種身分："
                    "獨立建置時是專案，被引用時是供應商。"),
        ],
        "code": 'if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)\n    # 只有獨立建置才註冊\n    add_subdirectory(tests) / install(...) / cpack\nendif()',
        "narration": "第三個設計是雙重身分：standalone 守衛。"
                     "只有自己當頂層時才註冊測試和安裝規則，"
                     "被消費時只產出函式庫 target。"
                     "引擎的 demo 和測試在別人的 build 裡全是噪音。",
    },
    {
        "tag": "設計 4",
        "title": "測試跟著依賴走",
        "sections": [
            ("做法", "連結 Gameplay 的測試全部留在遊戲側——"
                    "引擎 repo 只留純引擎的測試。"),
            ("細節", "連結遊戲層的 test 留在引擎 repo，"
                    "引擎就永遠不可能獨立建置——測試也是依賴。"),
            ("啟示", "搬程式碼前先問「它連結誰」——"
                    "依賴方向決定檔案歸屬，不是檔案長得像誰。"),
        ],
        "code": '# 引擎 repo：只收連結 PotatoEngine 的測試\n# 遊戲 repo：連結 Gameplay/Campaign 的測試全留\n# 判準 = target_link_libraries 的方向',
        "narration": "第四個設計：測試跟著依賴走。"
                     "連結遊戲層的測試留在遊戲側，引擎只留純引擎的。"
                     "否則引擎永遠拆不出去——測試也是依賴。"
                     "檔案歸屬看它連結誰，不看它長得像誰。",
    },
    {
        "tag": "設計 5",
        "title": "鏡像是唯讀的，真相只有一份",
        "sections": [
            ("做法", "上游 PotatoEngine repo 是產生的唯讀鏡像——"
                    "repo 內引擎是唯一真相來源，上游不直接編輯。"),
            ("細節", "sync 腳本單向推送；上游找不到對應的殘留檔"
                    "自動移入 legacy/，不刪不留陷阱。"),
            ("啟示", "雙向同步是地獄——指定唯一真相來源，"
                    "另一邊只做投影，衝突就消失了。"),
        ],
        "code": 'tools/sync-potatoengine-upstream.sh\n# repo 內引擎 → 上游鏡像（單向）\n# 上游殘留 → legacy/（不刪、可稽核）',
        "narration": "第五個設計是同步策略：真相只有一份。"
                     "上游 repo 是唯讀鏡像，sync 腳本單向推送，"
                     "殘留檔自動收進 legacy 目錄，不刪、可稽核。"
                     "雙向同步是地獄——指定唯一真相，衝突就消失了。",
    },
    {
        "tag": "設計 6",
        "title": "drift check：鏡像會說謊",
        "sections": [
            ("做法", "CI 加 drift 檢查——比對 repo 內引擎與上游鏡像"
                    "是否一致，漂移即紅。"),
            ("細節", "鏡像遲早會過期；與其相信流程會記得 sync，"
                    "不如讓 CI 每天驗屍。"),
            ("啟示", "凡是「靠人記得」的同步，"
                    "都要配一個「忘記會紅燈」的檢查。"),
        ],
        "code": '# CI step：\ntools/sync-potatoengine-upstream.sh --check\n# 鏡像 ≠ 真相來源 → exit 1',
        "narration": "第六個設計是防呆：drift check。"
                     "鏡像遲早會過期——與其相信每個人記得跑 sync，"
                     "不如讓 CI 比對漂移、不一致就亮紅。"
                     "靠人記得的同步，都要配忘記會報警的檢查。",
    },
    {
        "tag": "設計 7",
        "title": "filter-repo 前先備份與反哺",
        "sections": [
            ("做法", "歷史拆分用 git filter-repo；動刀前兩件事："
                    "備份現 repo、把上游獨有測試反向同步回本 repo。"),
            ("細節", "filter-repo 重寫歷史——上游 repo 既有的測試"
                    "若不先拉回，切完就隨舊歷史一起蒸發。"),
            ("啟示", "重寫歷史前盤點「誰擁有什麼」——"
                    "只存在於將被重寫區段的東西先救出來。"),
        ],
        "code": '# 動刀前：\ncp -r PotatoEngine PotatoEngine.bak     # 備份\ngit checkout main -- tests-from-upstream # 反哺拉回',
        "narration": "第七個設計是手術前的清單：備份加反哺。"
                     "filter-repo 會重寫歷史——上游獨有的測試"
                     "若不先拉回主 repo，切完就跟舊歷史一起蒸發。"
                     "動刀前先盤點：誰擁有什麼，只存在於舊區段的先救出來。",
    },
    {
        "tag": "設計 8",
        "title": "驗收基準不動：68/68",
        "sections": [
            ("做法", "Phase 1 全程不動測試基準——MSVC + MinGW 建置、"
                    "ctest 68/68，改前改後同一個數字。"),
            ("細節", "邊界固化是純搬移+守衛：行為不該變，"
                    "紅燈只可能來自搬壞——所以紅燈就是 bug，不用討論。"),
            ("啟示", "重構類改動要選「不動的驗收」——"
                    "基準一旦跟著改，你就失去了錯誤的定義。"),
        ],
        "code": '# 改前：ctest 68/68\n# 改後：ctest 68/68（同一個數字）\n# 邊界固化不許行為變化 → 紅燈 = 搬壞',
        "narration": "第八個設計收尾：驗收基準不動。"
                     "邊界固化是純搬移——行為不該變，"
                     "所以 ctest 六十八項全綠改前改後是同一個數字。"
                     "重構要選不動的驗收；基準跟著改，就失去了錯誤的定義。",
    },
    {
        "kind": "cover",
        "title": "第六集清單",
        "lines": ["邊界先固化 · 別假設自己是頂層 · 被消費只剩 lib",
                  "測試跟依賴走 · 真相只有一份 · 忘記會紅燈",
                  "動刀前備份反哺 · 驗收基準不動"],
        "narration": "第六集收在這裡。拆倉的心法其實跟寫模組一樣："
                     "介面先想清楚，搬動只是最後一步。"
                     "六集下來——從踩坑到拆倉——"
                     "這個專案最貴的資產不是程式碼，是這些紀律。",
    },
]

# ---------------- 第七集：兩種 AI ----------------

SLIDES_EP7 = [
    {
        "kind": "cover",
        "title": "兩種 AI",
        "lines": ["從實戰學 C++ 第七集",
                  "玩遊戲的 AI 與寫程式的 AI"],
        "narration": "第七集講這個專案裡的兩種機器學習："
                     "遊戲內學玩家的 DQN 指揮官與讀牌對手，"
                     "和遊戲外學專案的 IDE 智能建議。"
                     "同一套思維，兩個戰場。",
    },
    {
        "tag": "設計 1",
        "title": "把戰鬥包成 gym 環境",
        "sections": [
            ("做法", "BattleCommandEnv 把 BattleController 包成 "
                    "RLEnvironment——obs/action/reward/done 四個介面。"),
            ("細節", "RL 不碰戰鬥內部：env 只看得到觀測向量，"
                    "動作空間是「介入指令」不是單位操作。"),
            ("啟示", "RL 的第一工程問題是介面——"
                    "把現有系統壓進四個函式，比調參重要。"),
        ],
        "code": 'class BattleCommandEnv : RLEnvironment {\n    Obs Reset();  StepResult Step(Action a);\n    // obs = 戰場摘要向量，action = CP 介入指令\n}',
        "narration": "第一個設計：RL 的介面先行。"
                     "把戰鬥控制器包成 gym 式環境——觀測、動作、獎勵、終止四個介面。"
                     "重點：agent 學的不是操作單位，是什麼時候介入。"
                     "RL 的第一工程問題永遠是介面設計。",
    },
    {
        "tag": "設計 2",
        "title": "DQN 學的是介入時機",
        "sections": [
            ("做法", "DQN 指揮官的動作空間是稀少的 CP 介入——"
                    "學「什麼時候值得花」，不是學怎麼打。"),
            ("細節", "CP 稀少所以介入是貴的："
                    "Q 值要分出「計畫還撐得住」和「計畫崩潰」的差。"),
            ("啟示", "動作空間越小越貴，"
                    "學到的策略越像判斷力而非操作。"),
        ],
        "code": '// action ∈ { 按兵不動, 補位, 撤退, 強攻, ... }\n// CP 稀少 → 「不行動」本身就是要學會的選項',
        "narration": "第二個設計：DQN 學介入時機。"
                     "指揮點稀少，所以每次介入都很貴——"
                     "連「按兵不動」都是要學會的選項。"
                     "動作空間越小越貴，學到的越像判斷力。",
    },
    {
        "tag": "設計 3",
        "title": "BattlePlanner：AI 參謀不是 AI 對手",
        "sections": [
            ("做法", "AutoPlanner 幫玩家生計畫箭頭——"
                    "參謀角色：給建議，不奪決策權。"),
            ("細節", "規劃輸出要能被 Doctrine 系統消化——"
                    "AI 產物和玩家手寫的是同一種資料。"),
            ("啟示", "輔助型 AI 的產物要走玩家同樣的管道——"
                    "不能驗證的建議不是建議，是黑箱。"),
        ],
        "code": 'plan = planner.Propose(state);\n// 產出 = 與玩家手寫相同的 BattlePlan 資料\n// → 同一條驗證/執行管線',
        "narration": "第三個設計是定位：參謀不奪權。"
                     "AI 規劃器產出的計畫，和玩家手寫的是同一種資料、"
                     "走同一條驗證管線。"
                     "輔助型 AI 的建議必須能被檢查——"
                     "不能驗證的建議不是建議，是黑箱。",
    },
    {
        "tag": "設計 4",
        "title": "RivalDeck：對手會讀你的習慣",
        "sections": [
            ("做法", "統計玩家歷史慣用的 trigger 類型，"
                    "預寫反制牌組——你越愛用什麼，對手越準備什麼。"),
            ("細節", "讀的是行為分布不是即時反應："
                    "慣性越強的玩家，被反制得越狠。"),
            ("啟示", "對手 AI 最有感的差異化不是更聰明——"
                    "是記得你做過什麼。"),
        ],
        "code": 'stats = ledger.CountPlayerTriggers();\nrival.Prebuild(stats.TopCounters());\n// 慣用 trigger 的分布 → 反制牌組的權重',
        "narration": "第四個設計：對手會讀你。"
                     "系統統計你慣用的觸發條件，對手預寫反制牌組——"
                     "你越固定，被針對得越狠。"
                     "對手 AI 最有感的不是更聰明，是記得你做過什麼。",
    },
    {
        "tag": "設計 5",
        "title": "IDE 建議要 mutex-guarded 非同步",
        "sections": [
            ("做法", "dev assistant 的 AI 推論在背景執行緒跑，"
                    "結果經 mutex 回寫 GUI state。"),
            ("細節", "推論幾百毫秒起跳，卡進渲染迴圈整個 IDE 就結凍——"
                    "而且 GUI state 是固定長度 buffer，回寫要走安全路徑。"),
            ("啟示", "AI 功能進 UI 的代價是執行緒紀律——"
                    "慢的不進主迴圈，寫入要走鎖。"),
        ],
        "code": 'std::thread([this]{\n    auto s = assistant.Suggest(ctx);\n    std::lock_guard lk(mtx_);\n    SafeCopy(state.suggestion, s);  // buffer 安全寫\n}).detach();',
        "narration": "第五個設計跨到工具側：IDE 的 AI 建議。"
                     "推論要幾百毫秒——卡進渲染迴圈整個介面就結凍，"
                     "所以背景執行緒跑、mutex 守護回寫，"
                     "而且回寫要走固定 buffer 的安全路徑——"
                     "第一集那個坑在這裡回收了。",
    },
    {
        "tag": "設計 6",
        "title": "建議引擎內建專案規範",
        "sections": [
            ("做法", "智能建議掛專案規範護欄：不會建議第三方 JSON 庫、"
                    "會警告反向依賴、知道 banned C 函式清單。"),
            ("細節", "AI 不知道專案的規矩就會給「看起來對」的爛建議——"
                    "護欄把 AGENTS.md 的規則變成檢查程式。"),
            ("啟示", "讓 AI 讀規則不如讓 AI 過不了規則——"
                    "寫進檢查器的規範才是真的規範。"),
        ],
        "code": 'if (suggests("nlohmann/json"))\n    warn("專案規範：用 Serialization/JsonParser.h");\n// 護欄在產生端，不在審查端',
        "narration": "第六個設計是護欄：建議引擎內建專案規範。"
                     "它知道不能推第三方 JSON、知道依賴方向、"
                     "知道禁用函式清單。"
                     "讓 AI 讀規則不如讓 AI 過不了規則——"
                     "寫進檢查器的規範才是真的規範。",
    },
    {
        "tag": "設計 7",
        "title": "遮罩防誤報：註解不該觸發規範",
        "sections": [
            ("做法", "規範掃描先把註解和字串字面量遮罩——"
                    "註解裡寫 strcpy 是教學，不是違規。"),
            ("細節", "不遮罩的話，解釋「為什麼禁用 strcpy」的註解"
                    "本身就會觸發禁令——誤報會讓人關掉檢查。"),
            ("啟示", "檢查器的第一品質是零誤報——"
                    "誤報多的護欄最後都被靜音。"),
        ],
        "code": '// 「禁用 strcpy」——這行註解本身含 banned 字樣\nmasked = StripCommentsAndStrings(src);\nScan(masked);   // 掃遮罩後的程式碼',
        "narration": "第七個設計很小但決定成敗：遮罩防誤報。"
                     "規範掃描先遮掉註解和字串——"
                     "不然解釋為什麼禁用 strcpy 的註解本身就違規。"
                     "檢查器的第一品質是零誤報：誤報多的護欄最後都被靜音。",
    },
    {
        "tag": "設計 8",
        "title": "建議權重持久化：IDE 會學你",
        "sections": [
            ("做法", "智能建議的採納/略過回饋落成權重，"
                    "持久化在專案裡——suggestion_weights_test.json 這類檔。"),
            ("細節", "學習跨 session 累積：常被略過的建議類型權重下降，"
                    "IDE 越用越懂這個專案的口味。"),
            ("啟示", "工具學習的邊界是「只學這個專案」——"
                    "權重存 repo 層級，不污染全域模型。"),
        ],
        "code": 'weights.Update(kind, adopted ? +lr : -lr);\nweights.Save("suggestion_weights.json");\n// repo 層級學習——不污染全域模型',
        "narration": "第八個設計收尾：IDE 會學你。"
                     "建議的採納和略過回饋成權重、持久化在專案裡。"
                     "常被略過的建議類型自動降權——越用越懂這個專案。"
                     "重點是權重留在 repo 層級，不污染全域模型。",
    },
    {
        "kind": "cover",
        "title": "第七集清單",
        "lines": ["介面先行 · 學介入時機 · 建議要能驗證 · 對手讀習慣",
                  "慢的離開主迴圈 · 護欄在產生端",
                  "零誤報才有護欄 · 學習限定專案範圍"],
        "narration": "第七集收在這裡。兩種 AI 的共同教訓："
                     "不管學的是玩家還是專案，介面、邊界、可驗證性"
                     "永遠比模型本身重要。"
                     "下一集我們回到工具鏈——講專案怎麼養自己。",
    },
]

# ---------------- 第八集：專案養自己 ----------------

SLIDES_EP8 = [
    {
        "kind": "cover",
        "title": "專案養自己",
        "lines": ["從實戰學 C++ 第八集",
                  "佇列 · spec · 平行協作的秩序機器"],
        "narration": "第八集講最後一層：專案怎麼養自己。"
                     "當多個開發 session 在同一個 repo 並行——"
                     "秩序不是靠記憶，是靠檔案、佇列和檢查器長出來的。",
    },
    {
        "tag": "設計 1",
        "title": "佇列檔是真相，聊天不是",
        "sections": [
            ("做法", "auto-dev-queue.yaml 記每項工作："
                    "id、驗收條件、status、落地 commit 號。"),
            ("細節", "狀態機只有幾個值：queued → implementing → done；"
                    "done 的 note 必須附 commit——宣稱完成要拿證據。"),
            ("啟示", "進度存在檔案裡就不存在「我以為做完了」——"
                    "換 session 接手零成本。"),
        ],
        "code": '- id: L-7\n  status: done\n  note: "a5f1f12 落地：Provenance 掛入 LedgerEntry..."',
        "narration": "第一個設計：佇列檔是真相。"
                     "每項工作帶驗收條件和狀態機，"
                     "標完成還要附落地 commit——宣稱完成要拿證據。"
                     "進度在檔案裡，換誰接手都零成本。",
    },
    {
        "tag": "設計 2",
        "title": "spec 檔是 session 間的交接介面",
        "sections": [
            ("做法", "每項佇列工作先寫 spec：Intent、Boundaries"
                    "（Always/Ask First/Never）、I/O 矩陣、Code Map。"),
            ("細節", "「Ask First」和「Never」是給下一個執行者的護欄——"
                    "範圍外的事不用猜，spec 寫死了。"),
            ("啟示", "交接文件的價值不在寫得多，在寫掉歧義——"
                    "八個固定欄位比八頁散文有用。"),
        ],
        "code": 'spec-l7-ledger-provenance.md\n├── Intent / Approach\n├── Always / Ask First / Never\n├── I/O & Edge-Case Matrix\n└── Tasks & Acceptance',
        "narration": "第二個設計：spec 是交接介面。"
                     "固定欄位——意圖、邊界、I/O 矩陣、程式碼地圖、驗收。"
                     "Ask First 和 Never 是給下一個執行者的護欄。"
                     "交接文件的價值在消歧義，固定欄位比散文有用。",
    },
    {
        "tag": "設計 3",
        "title": "逐檔 stage：git add 自己的路徑",
        "sections": [
            ("做法", "多 session 併行時禁用 git add -A——"
                    "提交前 git status 認清哪些是自己改的，逐檔加。"),
            ("細節", "別人的半成品被掃進你的 commit，"
                    "回滚時會連別人的工作一起砍掉。"),
            ("啟示", "提交單位是「這次工作的邊界」——"
                    "邊界糊了，歷史就不能當文件讀。"),
        ],
        "code": 'git add Gameplay/Ledger.cpp Gameplay/Ledger.h \\\n      Examples/LedgerRegistryTest.cpp CMakeLists.txt\n# 不是 git add -A',
        "narration": "第三個設計是提交紀律：逐檔 stage。"
                     "這個 repo 常有多個 session 並行——"
                     "git add 全選會把別人的半成品掃進你的 commit。"
                     "提交的邊界糊了，歷史就不能當文件讀。",
    },
    {
        "tag": "設計 4",
        "title": "高碰撞熱區要會拆 hunk",
        "sections": [
            ("做法", "CMakeLists.txt、佇列檔是熱區——"
                    "多人同改時逐 hunk 拆：checkout HEAD → "
                    "重放自己的編輯 → stage → 還原工作檔。"),
            ("細節", "git add -p 能做但要逐段判斷；"
                    "熱區檔先想清楚「哪些行是我的」再碰。"),
            ("啟示", "碰撞檔不可怕——可怕的是"
                    "把別人的 hunk 當自己的簽出去。"),
        ],
        "code": 'git stash        # 或：checkout HEAD -- hot_file\ngit add -p CMakeLists.txt   # 只挑自己的 hunk\n# 還原工作檔 → 別人的改動還在',
        "narration": "第四個設計處理碰撞：熱區檔要會拆 hunk。"
                     "CMakeLists 和佇列檔人人都碰——"
                     "用 add 減 p 只挑自己的段落，"
                     "或者乾脆還原檔案重放自己的編輯再 stage。"
                     "把別人的改動簽成自己的，比衝突更糟。",
    },
    {
        "tag": "設計 5",
        "title": "批量紅燈先懷疑競爭，不是 bug",
        "sections": [
            ("做法", "測試同時寫同名 JSON 會互撞——"
                    "ctest 批量失敗時先單獨重跑，過了就是競爭不是 bug。"),
            ("細節", "本專案測試會寫 replay/roster JSON 到工作目錄；"
                    "並行跑時檔案互踩，單跑就綠。"),
            ("啟示", "診斷順序：先排除環境因素（競爭/順序/殘留），"
                    "再當真 bug 追——省下半天瞎忙。"),
        ],
        "code": 'ctest -C Release              # 批量紅\nctest -R LedgerForgeTest     # 單跑 → 綠？\n# 綠 = 平行競爭；紅 = 真 bug',
        "narration": "第五個設計是診斷順序：批量紅燈先懷疑競爭。"
                     "測試寫同名 JSON 會互踩——單獨重跑綠了就是競爭。"
                     "先排除環境因素再追 bug，"
                     "這條順序在這個專案省過很多次瞎忙。",
    },
    {
        "tag": "設計 6",
        "title": "IDE 用自家引擎蓋：dogfooding",
        "sections": [
            ("做法", "MingGoRTS_IDE 建立在 PotatoEngine 上——"
                    "開發工具本身就是引擎的最大客戶。"),
            ("細節", "引擎不好用第一個撞到的是自己的 IDE："
                    "字體、buffer、非同步的問題全在自家工具先現形。"),
            ("啟示", "吃自己的狗糧是最誠實的 QA——"
                    "引擎的痛點變成開發者的痛點，隔天就修。"),
        ],
        "code": 'MingGoRTS_IDE/   # 連結 PotatoEngine\n# 引擎的 API 難不難用？IDE 天天在回答',
        "narration": "第六個設計：dogfooding。"
                     "IDE 用自家引擎蓋——引擎難用的地方，"
                     "自己的工具第一個撞到。"
                     "吃自己的狗糧是最誠實的 QA：痛點天天在眼前，隔天就修。",
    },
    {
        "tag": "設計 7",
        "title": "AGENTS.md 是給下一個 session 的記憶",
        "sections": [
            ("做法", "驗證指令、坑位、平行衛生全寫進 AGENTS.md——"
                    "agent 每次開工先讀它。"),
            ("細節", "「MinGW DLL 坑」「時間戳陷阱」這種教訓"
                    "寫下來才不會每個 session 重新踩一遍。"),
            ("啟示", "文件的讀者是下一個不認識你的 session——"
                    "寫給忘了一切的人看，才寫得夠清楚。"),
        ],
        "code": '# AGENTS.md 固定段落：\n# Where things are / Running and verifying /\n# Known pitfalls / 平行開發衛生',
        "narration": "第七個設計：AGENTS.md 是交接記憶。"
                     "建置指令、驗證方式、已知坑位、平行衛生全寫在裡面——"
                     "讀者是下一個對專案零記憶的 session。"
                     "寫給忘了一切的人看，才會寫得夠清楚。",
    },
    {
        "tag": "設計 8",
        "title": "conventional commits 是給工具讀的",
        "sections": [
            ("做法", "feat/fix/docs/ci/chore 前綴 + commitlint CI 強制——"
                    "歷史是機器可讀的。"),
            ("細節", "佇列 note 附 commit 短碼就能被工具追溯；"
                    "changelog、回歸定位、拆倉範圍界定全都吃這個格式。"),
            ("啟示", "commit message 的第一讀者是工具不是人——"
                    "格式化的歷史讓 repo 變成可查詢的資料庫。"),
        ],
        "code": 'feat(gameplay): L-6 帳簿登錄編號派生\nfix(gameplay): ConvoyState::def 改值儲存修懸空指標\n# 型別(範圍): 主旨 —— commitlint 不過就打回',
        "narration": "第八個設計：格式化歷史。"
                     "conventional commits 加 commitlint 強制——"
                     "佇列能附 commit 號追溯、拆倉能按範圍切、"
                     "回歸能二分定位。"
                     "commit 的第一讀者是工具，歷史就是資料庫。",
    },
    {
        "kind": "cover",
        "title": "第八集清單",
        "lines": ["狀態進檔案 · spec 消歧義 · 只簽自己的檔",
                  "熱區拆 hunk · 批量紅先想競爭 · 吃自己狗糧",
                  "寫給零記憶的讀者 · 歷史是資料庫"],
        "narration": "第八集收在這裡，也是系列終點。"
                     "八集的共同答案：能規模化的不是天才，是秩序——"
                     "讓檔案記住、讓檢查器攔截、讓歷史可讀。"
                     "專案養自己，養的就是這些。",
    },
]

# ---------------- 第九集：把未解決問題請進引擎 ----------------

SLIDES_EP9 = [
    {
        "kind": "cover",
        "title": "把未解決問題請進引擎",
        "lines": ["從實戰學 C++ 第九集",
                  "數學/物理研究 → 引擎功能的裁決實錄"],
        "narration": "第九集講一個反常的實驗：拿著名未解決問題清單"
                     "對照引擎需求，能用的取局部結構，不能用的誠實拒絕。"
                     "研究的產物不是論文，是裁決表。",
    },
    {
        "tag": "設計 1",
        "title": "方法論：取結構，不取炫技",
        "sections": [
            ("做法", "每個未解決問題問一句：「局部可落地的近似是什麼」——"
                    "能進引擎的採用，不能的寫明拒絕理由。"),
            ("細節", "裁決表三欄：問題、引擎可取的局部、裁決——"
                    "採用/備選/拒絕/已涵蓋，每項都留判斷理由。"),
            ("啟示", "研究的價值在裁決不在篇幅——"
                    "「拒絕」寫清楚，下一個人就不會重新幻想一遍。"),
        ],
        "code": '| N 體 | 數值積分本來就是全部 → 辛積分器 | 採用 |\n| 湍流 | 無散度是關鍵不變量 → curl noise | 採用 |\n| TSP  | flow-field 已最優，無需求      | 拒絕 |',
        "narration": "先講方法論：取結構不取炫技。"
                     "每個未解決問題只問——局部可落地的近似是什麼。"
                     "裁決表四種結果：採用、備選、拒絕、已涵蓋，"
                     "每項留理由——拒絕寫清楚，下個人就不用重新幻想。",
    },
    {
        "tag": "設計 2",
        "title": "準隨機序列：取樣打敗 PRNG",
        "sections": [
            ("做法", "QuasiRandom：R2 黃金比例序列、Halton、"
                    "van der Corput——低差異序列做均勻取樣。"),
            ("細節", "star-discrepancy 的理論下界還是開放問題，"
                    "但工程上準隨機序列的覆蓋率已嚴格優於偽隨機。"),
            ("啟示", "「理論上還沒最優」不妨礙「實務上已更好」——"
                    "取樣均勻度看圖就懂，不必等證明。"),
        ],
        "code": 'auto s = QuasiRandom::R2(n, seed);\n// 同 n 下覆蓋均勻度 > mt19937——\n// 散布取樣/ jitter 都走這裡',
        "narration": "第一個採用：準隨機序列。"
                     "低差異序列的理論下界還是開放問題，"
                     "但工程上它的覆蓋率已經嚴格優於偽隨機。"
                     "理論還沒最優，不妨礙實務已經更好——"
                     "均勻度看圖就知道，不必等證明。",
    },
    {
        "tag": "設計 3",
        "title": "Poisson-disk：球體堆積的 2D 近似",
        "sections": [
            ("做法", "Bridson Poisson-disk 取樣 = 2D 藍噪——"
                    "點與點保持最小距離，天然不擠不空。"),
            ("細節", "高維球體堆積仍未解，但 2D 的近似夠好："
                    "產生據點、樹林、資源分布都靠它。"),
            ("啟示", "維度是難度的開關——2D 近似成熟的東西，"
                    "別被高維未解嚇到不敢用。"),
        ],
        "code": 'auto pts = QuasiRandom::PoissonDisk(rect, minDist, seed);\n// minDist 保證互斥——據點/植被/資源天然不擠',
        "narration": "第二個採用：Poisson-disk 藍噪取樣。"
                     "高維球體堆積還沒解，但二維的 Bridson 近似夠好——"
                     "點之間保證最小距離，產據點、樹林、資源都靠它。"
                     "維度是難度的開關，別被高維未解嚇到。",
    },
    {
        "tag": "設計 4",
        "title": "N 體無解 → 積分器品質就是全部",
        "sections": [
            ("做法", "PhysicsWorld::SetIntegrator(VelocityVerlet)——"
                    "辛積分器長期保能量，不漂。"),
            ("細節", "n≥3 無一般閉式解 → 物理本來就只能數值積分；"
                    "既然只能積分，積分器的長期行為就是全部。"),
            ("啟示", "承認無解之後，問題轉成工程選擇——"
                    "Euler 會漏能量，symplectic 不會。"),
        ],
        "code": 'world.SetIntegrator(Integrator::VelocityVerlet);\n// 辛積分器：長期能量守恆——Euler 會漂，它不會',
        "narration": "第三個採用來自 N 體問題。三體以上沒有一般解析解——"
                     "但物理本來就走數值積分，所以積分器品質就是全部。"
                     "Velocity Verlet 是辛積分器，長期保能量不漂移。"
                     "承認無解之後，問題變成純粹的工程選擇。",
    },
    {
        "tag": "設計 5",
        "title": "湍流不可解 → 無散度是關鍵不變量",
        "sections": [
            ("做法", "CurlNoise：用向量勢的 curl 構造流場——"
                    "散度嚴格為零，看起來就是湍流。"),
            ("細節", "Navier–Stokes 存在性是千禧年難題，"
                    "但「不可壓縮」的關鍵不變量可以解析構造。"),
            ("啟示", "抓不變量比求解容易——"
                    "符合不變量的場，視覺上就對了九成。"),
        ],
        "code": 'auto f = TurbulenceField(curlNoise, seed);\n// ∇·f ≡ 0 解析成立——不是逼近，是構造出來的',
        "narration": "第四個採用最漂亮：湍流。"
                     "Navier-Stokes 是千禧年難題，解不了——"
                     "但不可壓縮流的不變量「無散度」可以解析構造："
                     "向量勢取 curl，散度恆等於零。"
                     "抓不變量比求解容易，符合不變量，看起來就對九成。",
    },
    {
        "tag": "設計 6",
        "title": "Collatz：備選變落地",
        "sections": [
            ("做法", "Collatz 停滯時間 → 確定性雜湊場："
                    "CollatzField01 產地形特徵、CollatzFeature 分桶。"),
            ("細節", "研究時裁決是「備選——無明確消費者」；"
                    "需求出現後直接落地，連同擁擠模型 JammingModel 一起。"),
            ("啟示", "「備選」不是垃圾桶——"
                    "是等消費者的貨架，需求到了就拿。"),
        ],
        "code": 'CollatzField01(x, y, seed);   // 重尾分布的地形特徵\nJammingSpeedFactor(density); // 擁擠 → 移速懲罰',
        "narration": "第五個故事是備選變落地。"
                     "Collatz 停滯時間在研究時被判備選——有趣但沒消費者。"
                     "需求出現後直接落地成確定性雜湊場，"
                     "連擁擠阻塞模型也一起進來。"
                     "備選不是垃圾桶，是等消費者的貨架。",
    },
    {
        "tag": "設計 7",
        "title": "退化輸入是採用條款的一部分",
        "sections": [
            ("做法", "採用的每個模型都要回答退化輸入："
                    "零輸入、NaN、退化參數、熔斷上限。"),
            ("細節", "Collatz 有無窮迴圈風險 → 停滯時間帶熔斷；"
                    "密度 NaN → 回退化值，不傳播。"),
            ("啟示", "數學函式進引擎的第一條款是魯棒性——"
                    "未定義的輸入要有定義好的行為。"),
        ],
        "code": 'CollatzStoppingTime(n, kMaxIter);  // 熔斷上限\nJammingSpeedFactor(NaN) → kFreeFlow;  // 不傳播',
        "narration": "第六個設計是採用的附加條款：退化輸入要有定義。"
                     "Collatz 有無窮迴圈風險就加熔斷上限，"
                     "密度傳進 NaN 回退化值不傳播。"
                     "數學函式進引擎的第一條款永遠是魯棒性。",
    },
    {
        "tag": "設計 8",
        "title": "拒絕欄跟採用欄一樣值錢",
        "sections": [
            ("做法", "Navier–Stokes 全模擬、TSP 巡邏、素數雜湊——"
                    "裁決表明白寫拒絕，理由留下。"),
            ("細節", "拒絕理由都指向同一個判準：沒有消費者。"
                    "「酷」不是採用條件，「有系統要它」才是。"),
            ("啟示", "寫下拒絕的研究才算完成——"
                    "不然每個人都會把同一條死路再走一遍。"),
        ],
        "code": '| Navier–Stokes | 無流體需求，shader 動畫夠 | 拒絕 |\n| TSP           | flow-field 已最優          | 拒絕 |\n| 素數分佈      | xxHash 類已有更好解        | 拒絕 |',
        "narration": "最後一個設計：拒絕欄跟採用欄一樣值錢。"
                     "全流體模擬、TSP、素數雜湊都寫明拒絕，"
                     "理由同一個判準——沒有消費者。"
                     "酷不是採用條件。"
                     "把拒絕寫下來，下個人就不會把死路再走一遍。",
    },
    {
        "kind": "cover",
        "title": "第九集清單",
        "lines": ["取結構不取炫技 · 準隨機 > PRNG · 2D 近似就夠",
                  "積分器品質是全部 · 不變量比求解容易 · 備選是貨架",
                  "退化輸入要定義 · 拒絕欄一樣值錢"],
        "narration": "第九集收在這裡。未解決問題清單不是炫技素材庫——"
                     "是一張裁決表：採用的進引擎，拒絕的留理由，"
                     "備選的等消費者。"
                     "理論照亮邊界，工程填滿邊界內的地。",
    },
]

# ---------------- 第十集：戰爭不是目的 ----------------

SLIDES_EP10 = [
    {
        "kind": "cover",
        "title": "戰爭不是目的",
        "lines": ["從實戰學 C++ 第十集",
                  "治理軸與敘事系統的實錄"],
        "narration": "第十集講這個遊戲最反常的地方："
                     "軍事勝利不是目的，治理才是。"
                     "兩個哲學軸——至聖者無戰、治平者無勝——"
                     "不是文案，是寫進系統的機制。",
    },
    {
        "tag": "設計 1",
        "title": "哲學軸寫成硬規則",
        "sections": [
            ("做法", "「至聖者無戰」= 倒戈優於殲滅、嚇阻優於接戰；"
                    "「治平者無勝」= 民心秩序是跨戰鬥持久軸。"),
            ("細節", "設計宣言直接對應系統：倒戈有受降路徑、"
                    "治理有累加器、章節有無戰勝利條件。"),
            ("啟示", "價值觀要落地就得有對應的機制——"
                    "沒有機制支撐的核心理念只是牆上的標語。"),
        ],
        "code": '// 設計宣言 → 機制對照\n"至聖者無戰" → 受降/嚇阻/顛覆路徑\n"治平者無勝" → Campaign::Governance 累加器',
        "narration": "第一個設計：哲學軸寫成硬規則。"
                     "不戰而屈人之兵不是口號——倒戈有受降路徑、"
                     "嚇阻有判定、治理有持久累加器。"
                     "價值觀要落地，就得有機制支撐，否則只是標語。",
    },
    {
        "tag": "設計 2",
        "title": "民心秩序是戰役資產",
        "sections": [
            ("做法", "Campaign::Governance 累加器跨戰鬥持久——"
                    "佔村、護輜、暴行都進民心/秩序兩軸。"),
            ("細節", "戰場敗北仍可轉化為治理勝利："
                    "輸了戰鬥但贏了民心，章節照樣往好結局走。"),
            ("啟示", "持久軸讓每場戰鬥都有事後意義——"
                    "單場勝負不再是唯一的記分板。"),
        ],
        "code": 'gov.Accumulate(占村, +2);   // 民心軸\ngov.Accumulate(暴行, +3);   // 秩序軸（負面也記）\n// 跨章節持久——存進 potato.campaign/1',
        "narration": "第二個設計：民心秩序是戰役資產。"
                     "治理累加器跨戰鬥持久——佔村、護輜、暴行都記帳。"
                     "輸了戰鬥但贏了民心，章節照樣往好結局走。"
                     "持久軸讓每場戰鬥都有事後意義。",
    },
    {
        "tag": "設計 3",
        "title": "墮落是只增不減的棘輪",
        "sections": [
            ("做法", "corruption ratchet：暴行一旦發生就單向累積——"
                    "做好事能贏民心，洗不掉墮落值。"),
            ("細節", "棘輪的意義是「不遺忘」："
                    "焚村的代價永久留在軸上，影響後續章節判定。"),
            ("啟示", "單向累積器是「代價不可逆」的最簡實作——"
                    "比複雜的聲望系統更有重量。"),
        ],
        "code": 'gov.corruption += atrocity;  // 只增不減\n// ratchet：沒有對應的 -= 路徑——不遺忘',
        "narration": "第三個設計很狠：墮落棘輪。"
                     "暴行只增不減——做好事能贏民心，但洗不掉墮落值。"
                     "單向累積器是代價不可逆的最簡實作，"
                     "焚村的重量永久留在帳上。",
    },
    {
        "tag": "設計 4",
        "title": "GovernanceField：資訊分層給視野",
        "sections": [
            ("做法", "戰場治理追蹤由「有地圖知識的呼叫端」每拍驅動——"
                    "佔領/焚村/護輜/劫輜事件經此入帳。"),
            ("細節", "看不見的暴行不入帳：戰場系統不預設全知，"
                    "情報層和治理層的邊界是一致的。"),
            ("啟示", "「誰看得見」決定「誰記得」——"
                    "知識分層本身就是玩法，不是渲染細節。"),
        ],
        "code": 'field.RecordGovernanceEvent(ev);   // 由有視野的呼叫端驅動\n// 看不見的暴行不入帳——全知不是預設',
        "narration": "第四個設計呼應情報霧：治理追蹤由有視野的呼叫端驅動，"
                     "看不見的暴行不入帳。"
                     "誰看得見決定誰記得——"
                     "知識分層本身就是玩法，不是渲染細節。",
    },
    {
        "tag": "設計 5",
        "title": "無戰章節是系統不是過場",
        "sections": [
            ("做法", "談判/嚇阻/顛覆是完整路徑："
                    "有閾值判定、有消耗、有失敗態——不是跳過戰鬥按鈕。"),
            ("細節", "不戰而勝要達成條件：民心夠高、情報夠確定、"
                    "敵將人格吃這一套——三條路各自有代價。"),
            ("啟示", "「另一種勝法」要成立，"
                    "就得有跟戰鬥同等深度的系統撐著。"),
        ],
        "code": 'if (deterrence.Holds(intel, morale, foe.personality))\n    chapter.Resolve(NoBattle::Deterrence);\n// 有閾值、有消耗、有失敗態——不是 skip',
        "narration": "第五個設計：無戰章節是系統不是過場。"
                     "談判、嚇阻、顛覆各有閾值、消耗和失敗態。"
                     "要讓不戰而勝成立，"
                     "就得有跟戰鬥同等深度的系統撐著。",
    },
    {
        "tag": "設計 6",
        "title": "神話行為是民心放大器",
        "sections": [
            ("做法", "歷史層與神話層跑在同一張地圖："
                    "安撫神社、平息怨靈直接轉化為下一場的治理資本。"),
            ("細節", "滲透是 0–3 狀態機，MythLog 具名入帳——"
                    "神話不是彩蛋，是民心軸的槓桿。"),
            ("啟示", "雙層世界要能互相計價——"
                    "神話行動換治理資本，兩層才真的是一張圖。"),
        ],
        "code": 'mythLog.Record("安撫神社", Seepage::L2);\ngov.Accumulate(ShrinePacified, +2);\n// 神話層行為 → 民心軸資本',
        "narration": "第六個設計把神話層接上經濟："
                     "安撫神社直接轉化為治理資本。"
                     "滲透是零到三的狀態機、事件具名入帳——"
                     "神話不是彩蛋，是民心軸的槓桿。"
                     "雙層世界要能互相計價，才真的是一張圖。",
    },
    {
        "tag": "設計 7",
        "title": "章回體例是格式強制",
        "sections": [
            ("做法", "ChapterConventions 四聲部：題詞、敵將判詞、"
                    "欲知後事、結局——每章固定結構。"),
            ("細節", "格式強制讓敘事有韻律：玩家知道每章的節拍，"
                    "系統也知道在哪個聲部掛什麼資料。"),
            ("啟示", "敘事的品質來自一致性——"
                    "固定聲部是給寫作的骨架，不是限制。"),
        ],
        "code": 'chapter.Render(\n    Epigraph(),        // 題詞\n    foe.Judgement(),   // 敵將判詞（聽聞態）\n    Teaser(),          // 欲知後事\n    ending);           // 結局',
        "narration": "第七個設計是形式：章回體例四聲部。"
                     "題詞、敵將判詞、欲知後事、結局——每章固定結構。"
                     "格式強制讓敘事有韻律，"
                     "也讓系統知道在哪個聲部掛什麼資料。"
                     "固定聲部是骨架，不是限制。",
    },
    {
        "tag": "設計 8",
        "title": "四手結局：軌跡決定收束",
        "sections": [
            ("做法", "EndingPage 不按勝負分結局——"
                    "按治理軌跡：民心/秩序/墮落的累積組合出四手結局。"),
            ("細節", "全勝但暴行纍纍的帳，跟節制推進的帳，"
                    "收到的不是同一個天下。"),
            ("啟示", "結局是帳本的結算頁——"
                    "玩家怎麼打，決定他配得什麼樣的結尾。"),
        ],
        "code": 'ending = EndingPage::Resolve(\n    gov.morale, gov.order, gov.corruption);\n// 勝負只是輸入之一——軌跡才是本體',
        "narration": "第八個設計收束全遊戲：四手結局。"
                     "不按勝負，按治理軌跡——民心、秩序、墮落的組合。"
                     "全勝但暴行纍纍，和節制推進，收到的不是同一個天下。"
                     "結局是帳本的結算頁：怎麼打，決定配得什麼結尾。",
    },
    {
        "kind": "cover",
        "title": "第十集清單",
        "lines": ["價值觀要有機制 · 治理是戰役資產 · 棘輪不遺忘",
                  "看得見才記得 · 無戰要有深度 · 神話可計價",
                  "體例是骨架 · 結局是結算"],
        "narration": "第十集收在這裡。這個遊戲最大的工程選擇："
                     "把「為什麼而戰」做成跟「怎麼戰」一樣硬的系統。"
                     "十集下來，從坑位到哲學——"
                     "程式碼最終要回答的，還是設計的初衷。",
    },
]

# ---------------- 第十一集：戰場的身體性 ----------------

SLIDES_EP11 = [
    {
        "kind": "cover",
        "title": "戰場的身體性",
        "lines": ["從實戰學 C++ 第十一集",
                  "點選 · 擁擠 · 地形的物理層"],
        "narration": "第十一集講戰場的物理層——"
                     "上一集研究貨架上的擁擠模型，今天真的接進戰鬥了。"
                     "從滑鼠點選到人群踩踏，"
                     "這一層讓抽象的規則有了身體。",
    },
    {
        "tag": "設計 1",
        "title": "點選是幾何，不是事件",
        "sections": [
            ("做法", "BattlePicker：螢幕座標 → 世界射線 → 地面求交 → "
                    "格子換算 → 小隊包圍球命中判定。"),
            ("細節", "透視相機俯視戰場：命中/未命中/最近命中三態分開，"
                    "「沒點到但差一點」要給最近者。"),
            ("啟示", "點選層全部 headless 可測——"
                    "幾何計算不需要真的畫面。"),
        ],
        "code": 'ray = camera.ScreenToRay(mouse);\nif (ray.HitGround(ground))       // 地面求交\n    squad = picker.Nearest(p, r); // 包圍球最近命中',
        "narration": "第一個設計：點選是幾何。"
                     "滑鼠座標變成世界射線，跟地面求交、換算格子、"
                     "再打小隊的包圍球——命中、未命中、最近命中三態分開。"
                     "整條鏈都是純幾何，不需要畫面就能測。",
    },
    {
        "tag": "設計 2",
        "title": "擁擠是可開關的物理層",
        "sections": [
            ("做法", "BattleController::SetJamming——"
                    "擁擠群體的 crowdFactor 下降，孤隊不受影響。"),
            ("細節", "開關式設計：關閉時倍率復位為 1，"
                    "不開此玩法的戰鬥零成本、零回歸風險。"),
            ("啟示", "物理層掛鉤要做成可關閉——"
                    "能關的機制才敢進核心迴圈。"),
        ],
        "code": 'controller.SetJamming(true);\n// crowded → speed × crowdFactor\n// off    → 倍率復位 1.0（無回歸）',
        "narration": "第二個設計是掛鉤的形狀：擁擠做成可開關層。"
                     "SetJamming 一開，擁擠群體的移速因子下降、孤隊不受影響；"
                     "關掉倍率復位為一，不用此機制的戰鬥零回歸風險。"
                     "能關的機制才敢進核心迴圈。",
    },
    {
        "tag": "設計 3",
        "title": "測試斷言行為，不是數值",
        "sections": [
            ("做法", "JammingBattleTest 的核心斷言：同方向移動時，"
                    "擁擠群真的比孤隊「慢」——測結果不測內部。"),
            ("細節", "不只檢查 crowdFactor 數值下降，"
                    "還要跑一段路證明位移真的變小——"
                    "數值對但行為沒變 = 接了假線。"),
            ("啟示", "接線類功能的測試要落在可觀測行為上——"
                    "「接上了」由效果證明，不由 setter 回讀證明。"),
        ],
        "code": 'Check(distCrowded < distLone, "擁擠群真的慢");\n// 不只驗 factor 下降——跑一段路看位移',
        "narration": "第三個設計是測試哲學：斷言行為不斷言數值。"
                     "擁擠測試不只檢查因子下降，還真的讓兩隊跑一段路，"
                     "斷言擁擠群位移更小。"
                     "接線接沒接上，要由可觀測的效果證明。",
    },
    {
        "tag": "設計 4",
        "title": "地形加成是分類不是數字",
        "sections": [
            ("做法", "TerrainBonus 把地形效果做成類別規則——"
                    "高地射擊加成、林地隱蔽、渡口阻滯各自命名。"),
            ("細節", "加成走 modifier 通道進 Doctrine 系統——"
                    "地形是規則的輸入，不是傷害公式裡的魔法常數。"),
            ("啟示", "地圖數值化要經過同一條 modifier 管線——"
                    "繞過它的地形效果都是第二套系統的開始。"),
        ],
        "code": 'bonus = map.TerrainAt(pos).Modifier(action);\n// 高地/林地/渡口 → 同一條 modifier 通道',
        "narration": "第四個設計：地形加成走規則管線。"
                     "高地、林地、渡口的加成是命名的 modifier，"
                     "進 Doctrine 的同一條通道——"
                     "地形是規則的輸入，不是公式裡的魔法常數。"
                     "繞過管線的效果，都是第二套系統的開始。",
    },
    {
        "tag": "設計 5",
        "title": "互動點是地圖的釘子",
        "sections": [
            ("做法", "BattleMap 的 JSON 帶互動點：渡口、神社、糧道——"
                    "垂直切片直接把互動物綁上去。"),
            ("細節", "互動點是資料不是硬編碼座標——"
                    "設計師改 JSON 就能挪釘子，不動程式。"),
            ("啟示", "地圖 schema 裡預留互動層，"
                    "比「之後再說」的座標常數耐用十倍。"),
        ],
        "code": '{ "schema": "potato.battle_map/1",\n  "interactives": [\n    {"kind":"ford","pos":[12,7]},\n    {"kind":"shrine","pos":[3,15]} ] }',
        "narration": "第五個設計：互動點是地圖上的釘子。"
                     "渡口、神社、糧道都寫在 battle map 的 JSON 裡，"
                     "設計師改資料就能挪位置。"
                     "schema 裡預留互動層，比以後再補的座標常數耐用十倍。",
    },
    {
        "tag": "設計 6",
        "title": "運輸隊是會走路的資源",
        "sections": [
            ("做法", "Convoy 是實體不是數值：有路徑、有位置、"
                    "會被劫——護輜抵達和劫輜成功是兩種結局。"),
            ("細節", "因為是實體，它走得到的地方才有補給——"
                    "補給線是幾何問題，不是帳面問題。"),
            ("啟示", "把資源做成實體，"
                    "「斷糧道」就自然變成可以執行的戰術。"),
        ],
        "code": 'convoy.Step(route);          // 實體沿路移動\nif (ambush.Intercepts(convoy))\n    field.Record(劫輜);        // 入治理帳',
        "narration": "第六個設計：運輸隊是會走路的資源。"
                     "它有路徑有位置會被劫——護輜抵達和劫輜成功是兩種結局。"
                     "因為是實體，走得到的地方才有補給；"
                     "斷糧道因此是能執行的戰術，不是文案。",
    },
    {
        "tag": "設計 7",
        "title": "戰後結算是管線不是頁面",
        "sections": [
            ("做法", "PostBattle 把結算拆成管線：傷亡入名冊、"
                    "事件入帳、整補進 RefitCamp、報告交史官。"),
            ("細節", "每站消費上一站的產物——結算不是一個大函式，"
                    "是幾個各自可測的小轉換。"),
            ("啟示", "「戰後」之所以複雜是因為它什麼都碰——"
                    "拆成管線，每段才能獨立驗。"),
        ],
        "code": 'post.Settle(battle)\n    → roster.ApplyCasualties()\n    → ledger.Post(...)\n    → refit.Open(camp)        // 逐站可測',
        "narration": "第七個設計：戰後結算是管線。"
                     "傷亡入名冊、事件入帳、整補進營、報告交史官——"
                     "每站消費上一站的產物。"
                     "戰後複雜是因為它什麼都碰，拆成管線才各自可驗。",
    },
    {
        "tag": "設計 8",
        "title": "貨架研究的落地節奏",
        "sections": [
            ("做法", "研究裁決「備選」→ 需求出現 → 引擎層先落地"
                    "（QuasiModelsTest）→ 玩法接線（JammingBattleTest）。"),
            ("細節", "兩層測試分開：模型測試連結 PotatoEngine"
                    "（可同步上游），玩法測試連結 Gameplay——"
                    "依賴層級連測試都不混。"),
            ("啟示", "研究的投資回報看落地節奏——"
                    "備選欄存在的意義就是這一刻。"),
        ],
        "code": 'QuasiModelsTest  → link PotatoEngine  # 引擎層，可上游\nJammingBattleTest → link Gameplay     # 玩法層\n# 同功能的兩層測試，依賴不混',
        "narration": "第八個設計收束前兩集：貨架研究的落地節奏。"
                     "擁擠模型從備選欄進來，先在引擎層落地測試、"
                     "再接進玩法層——兩層測試連結不同的庫，依賴不混。"
                     "備選欄存在的意義，就是需求出現的這一刻。",
    },
    {
        "kind": "cover",
        "title": "第十一集清單",
        "lines": ["點選是幾何 · 物理層可開關 · 斷言行為不斷言數值",
                  "地形走 modifier · 互動點是資料 · 運輸隊是實體",
                  "結算是管線 · 備選等消費者"],
        "narration": "第十一集收在這裡。物理層的共同設計："
                     "做成掛鉤、可以關閉、行為可測。"
                     "戰場有了身體，規則才有重量。",
    },
]

SLIDES_EP12 = [
    {
        "kind": "cover",
        "title": "帳目怎麼被相信",
        "lines": ["從實戰學 C++ 第十二集",
                  "seeded 抽驗 · 三態具結 · 可重現的隨機"],
        "narration": "第十二集回到帳本——這次不是寫帳，是查帳。"
                     "史官怎麼證明一本帳可信？答案是查證："
                     "不全量重算，而是 seeded 抽驗加三態具結。"
                     "工程上最精彩的部分是：怎麼讓隨機抽樣可重現。",
    },
    {
        "tag": "設計 1",
        "title": "確信是光譜，不是布林",
        "sections": [
            ("做法", "AssuranceVerdict 三態：Assured 相符 / Qualified 存疑 / "
                    "Adverse 拒絕——對應 ISSA 5000 確信光譜。"),
            ("細節", "鏈斷或抽驗不符 → 拒絕具結；鏈完整但有 malformed "
                    "分錄或疑帳在場 → 存疑；全部乾淨才相符。"),
            ("啟示", "「可信嗎」的答案不該是 true/false——"
                    "真實世界的查證結論本來就有附帶條件的中間態。"),
        ],
        "code": 'enum class AssuranceVerdict {\n    Assured,    // 相符：鏈完整+抽驗全過+無疑帳\n    Qualified,  // 存疑：完整但有異常\n    Adverse     // 拒絕：斷鏈或抽驗不符\n};',
        "narration": "第一個設計：確信是光譜。"
                     "查證結論分三態——鏈斷或抽驗不符就拒絕具結，"
                     "鏈完整但有壞分錄或疑帳就存疑，全部乾淨才相符。"
                     "「可信嗎」的答案不是布林——"
                     "真實查證本來就有附帶條件的中間態。",
    },
    {
        "tag": "設計 2",
        "title": "自引種：帳簿自己當種子",
        "sections": [
            ("做法", "seed==0 時用 RootHash() 當種子——"
                    "同一本帳永遠抽出同一批樣本。"),
            ("細節", "外部 seed 仍可指定（重查時換樣本），"
                    "但預設路徑讓「抽驗結果」成為帳簿的決定性函數。"),
            ("啟示", "抽樣要能重複才叫證據——"
                    "每次抽不同的抽查，質疑者永遠可以說「你再抽一次」。"),
        ],
        "code": 'if (seed == 0)\n    seed = ledger.RootHash(); // 自引種\n// 同帳簿 → 同樣本 → 同結論',
        "narration": "第二個設計最優雅：自引種。"
                     "seed 為零時直接用帳簿的 rootHash 當種子——"
                     "同一本帳永遠抽出同一批樣本，"
                     "抽驗結果變成帳簿的決定性函數。"
                     "抽樣要能重複才叫證據，"
                     "不然質疑者永遠能說你再抽一次。",
    },
    {
        "tag": "設計 3",
        "title": "可重現的隨機：自己洗，別用 distro",
        "sections": [
            ("做法", "Fisher-Yates 部分洗牌取前 k 個互異索引，"
                    "引擎用 mt19937_64——標準化的演算法。"),
            ("細節", "關鍵：取樣用 rng() % (n-i) 而不是 "
                    "uniform_int_distribution——後者的輸出"
                    "隨標準庫實作而異，MSVC/MinGW/g++ 會抽不同樣本。"),
            ("啟示", "要跨編譯器可重現，只能用標準規定輸出的零件——"
                    "引擎有規定，distribution 沒有。"),
        ],
        "code": 'std::mt19937_64 rng(seed);\nfor (int i = 0; i < k; ++i) {\n    size_t j = i + rng() % (n - i); // 不用 distro\n    std::swap(idx[i], idx[j]);\n}',
        "narration": "第三個設計是跨編譯器的隨機。"
                     "Fisher-Yates 部分洗牌，引擎用 mt19937_64——"
                     "它有標準化的輸出。關鍵在取樣用模運算，"
                     "不用 uniform_int_distribution——"
                     "後者的結果隨標準庫實作而異，"
                     "三個編譯器會抽出不同樣本，查證就不再可重現。",
    },
    {
        "tag": "設計 4",
        "title": "抽驗驗什麼：重算，不是抽查",
        "sections": [
            ("做法", "每筆樣本做兩件事：重算 HashEntry 比對儲存值，"
                    "檢查 prevHash 真的指向前一筆的 hash。"),
            ("細節", "第一筆的 prevHash 要對 kGenesisHash——"
                    "邊界筆的鏈環最容易被忘記。"),
            ("啟示", "抽驗的公信力來自「重算」——"
                    "只比對記錄間的一致性，篡改者可以連記錄一起改。"),
        ],
        "code": 'expectPrev = (at == 0) ? kGenesisHash\n                         : es[at-1].hash;\nif (c.prevHash != expectPrev ||\n    HashEntry(c.prevHash, c.entry) != c.hash)\n    r.sampleBad = at;',
        "narration": "第四個設計：抽驗抽的是重算。"
                     "每筆樣本重算雜湊比對儲存值，"
                     "再檢查鏈環真的接回前一筆——第一筆要對創世雜湊。"
                     "公信力來自重算：只比對記錄彼此的一致性，"
                     "篡改者可以把記錄連同雜湊一起改掉。",
    },
    {
        "tag": "設計 5",
        "title": "結論之外，證據全留",
        "sections": [
            ("做法", "LedgerAssurance 不只回 verdict——sampled、"
                    "sampleBad、brokenAt、unsoundAt、suspectCount 全保留。"),
            ("細節", "報告層和測試各自取用需要的欄位——"
                    "結構體是證據袋，不是只有判決書。"),
            ("啟示", "查證型 API 要回「過程的全部痕跡」——"
                    "只回 enum，除錯時就什麼都沒有。"),
        ],
        "code": 'struct LedgerAssurance {\n    AssuranceVerdict verdict;\n    int sampled, sampleSize, sampleBad;\n    int brokenAt, unsoundAt, suspectCount;\n}; // 證據袋，不只是判決',
        "narration": "第五個設計：證據全留。"
                     "回傳結構除了結論，還帶抽了幾筆、哪筆不符、"
                     "鏈斷在哪、疑帳幾筆——報告層拿它寫話術，"
                     "測試拿它逐欄位斷言。"
                     "只回 enum 的查證 API，出問題時什麼線索都不剩。",
    },
    {
        "tag": "設計 6",
        "title": "史官話術：三態三種口氣",
        "sections": [
            ("做法", "HistorianReport 依 verdict 產生不同具結文字："
                    "相符說「帳目相符」、存疑附原因、拒絕說「不予採信」。"),
            ("細節", "存疑要說出為什麼：第幾筆借貸不成立、"
                    "另有幾筆疑帳——附帶條件的結論必須附帶條件本身。"),
            ("啟示", "機器結論翻成人話時，中間態最難寫——"
                    "「存疑」若不明說理由，玩家會當它是通過。"),
        ],
        "code": 'if (Adverse)   "鏈斷於第N筆——不予採信"\nif (Qualified) "抽驗皆符，然{why}——存疑"\nelse           "抽驗皆符——帳目相符"',
        "narration": "第六個設計是把結論寫成人話。"
                     "相符就說帳目相符；拒絕就說鏈斷在第幾筆；"
                     "最難的是存疑——必須說出為什麼："
                     "哪筆借貸不成立、另有幾筆疑帳。"
                     "附帶條件的結論，必須附帶條件本身。",
    },
    {
        "tag": "設計 7",
        "title": "opt-in 的查證",
        "sections": [
            ("做法", "assuranceSample>0 才啟動——報告不帶此欄位時，"
                    "整段查證文字不存在。"),
            ("細節", "和 L-7 provenance 同一招：可選欄位缺席時"
                    "輸出逐位不變，舊報告格式零回歸。"),
            ("啟示", "新功能進「報告類」輸出的安全姿勢都是 opt-in——"
                    "預設路徑不變，願意查證的人才付抽驗成本。"),
        ],
        "code": 'if (in.assuranceSample > 0) {\n    auto as = AssureLedger(*in.ledger,\n        in.assuranceSeed, in.assuranceSample);\n    ... // 不帶欄位 → 這段不存在\n}',
        "narration": "第七個設計：查證是 opt-in 的。"
                     "報告帶 assuranceSample 才啟動，不帶整段不存在——"
                     "和上一集 provenance 同一招，"
                     "可選欄位缺席時輸出逐位不變，舊格式零回歸。"
                     "願意查證的人才付抽驗的成本。",
    },
    {
        "tag": "設計 8",
        "title": "帳本能力的累積式設計",
        "sections": [
            ("做法", "L-4 疑帳 → L-6 登錄字號 → L-7 來源 → L-8 確信——"
                    "每一層都讀前面留下的資料。"),
            ("細節", "AssureLedger 直接整合：Verify 斷點、"
                    "SoundnessViolation、suspectCount——"
                    "查證不是新掃描，是舊檢查的組合判決。"),
            ("啟示", " ledger 系列功能的真正價值在累積——"
                    "單獨看每個都不大，組起來是完整的查證管線。"),
        ],
        "code": 'r.brokenAt   = ledger.Verify();          // L-2\nr.unsoundAt  = ledger.SoundnessViolation(); // L-3\nr.suspectCount = ledger.SuspectCount();     // L-4\n// L-8 = 舊檢查的組合判決 + 新抽驗',
        "narration": "第八個設計收束整個帳本系列：能力是累積的。"
                     "確信函式直接整合前面每一層——"
                     "驗鏈、健全掃描、疑帳計數，加上新的抽驗。"
                     "查證不是又一次掃描，是舊檢查的組合判決。"
                     "單獨看每個功能都不大，組起來才是完整管線。",
    },
    {
        "kind": "cover",
        "title": "第十二集清單",
        "lines": ["三態光譜 · 自引種 · 標準化引擎加模運算",
                  "抽驗是重算 · 證據袋 · 中間態要附理由",
                  "opt-in 查證 · 舊檢查組合成新判決"],
        "narration": "第十二集收在這裡。查證的工程精髓："
                     "結論是光譜、抽樣可重現、證據全保留。"
                     "帳本寫了八集——從記帳到查帳，信任是被工程出來的。",
    },
]

SLIDES_EP13 = [
    {
        "kind": "cover",
        "title": "當擱置理由消失",
        "lines": ["從實戰學 C++ 第十三集",
                  "備選變採用 · Collatz 上地圖 · 擁擠不分敵我"],
        "narration": "第十三集講研究貨架的第二次落地。"
                     "第九集的裁決表裡，Collatz 和擁擠模型都躺在備選欄，"
                     "理由寫得很清楚：無消費者。"
                     "這一集記錄理由消失的那一刻——"
                     "它們現在真的長在地圖和戰場上了。",
    },
    {
        "tag": "設計 1",
        "title": "裁決表是活文件",
        "sections": [
            ("做法", "UNSOLVED 文件不只記採用/拒絕——「備選」欄"
                    "附擱置理由；理由消失時改判並記錄原因。"),
            ("細節", "Collatz：「無明確消費者」→ MapGenerator 需要"
                    "「非週期、稀有、確定性」的特徵點——正是停滯時間"
                    "分佈的形狀。jamming：「等密度玩法」→ 斷橋渡口"
                    "多隊擁擠已是日常。"),
            ("啟示", "備選欄要寫清楚「什麼條件成立就翻身」——"
                    "讓未來的自己知道什麼訊號出現該回來撿。"),
        ],
        "code": '| Collatz | 停滯時間做地形特徵 | 備選→採用 |\n| jamming | 擁擠→移速懲罰       | 備選→採用 |\n# 擱置理由消失 = 改判條件達成',
        "narration": "第一個設計：裁決表是活文件。"
                     "Collatz 當初的擱置理由是「無明確消費者」——"
                     "程序地圖生成器落地後，需要非週期、稀有、"
                     "確定性的特徵點，正是停滯時間分佈的形狀。"
                     "擁擠模型也一樣，斷橋渡口的多隊堵塞已是日常場景。"
                     "備選欄寫清楚翻身條件，"
                     "未來的自己才知道什麼訊號該回來撿。",
    },
    {
        "tag": "設計 2",
        "title": "重尾分佈是一種地形語言",
        "sections": [
            ("做法", "CollatzField01 把停滯時間正規化成 [0,1] 場值——"
                    "重尾分佈下大部分格子低值，極少數格衝到頂。"),
            ("細節", "featureThr=0.90 → 約百分之一的格子成特徵點；"
                    "稀有不靠機率抽籤，靠分佈形狀自然長出來。"),
            ("啟示", "「稀有物」的正確做法不是 random<p 就生成——"
                    "用重尾場加閾值，稀有度、位置、形狀全是確定性的。"),
        ],
        "code": 'else if (cfg.featureThr > 0 &&\n    Quasi::CollatzField01(x, y,\n        seed ^ 0xC011A7u) >= cfg.featureThr)\n    c = Cell::DeepForest; // ~1% 格',
        "narration": "第二個設計：重尾分佈是一種地形語言。"
                     "Collatz 場值大部分格子很低，極少數衝到頂——"
                     "閾值設零點九，約百分之一的格子變成特徵點。"
                     "稀有度不是靠機率抽籤，是靠分佈形狀長出來的——"
                     "位置、數量、形狀全部由 seed 決定。",
    },
    {
        "tag": "設計 3",
        "title": "場間去相關：給每個場一個域名",
        "sections": [
            ("做法", "seed ^ 0xC011A7u——同一張圖的不同場用"
                    "不同域常數 XOR，避免特徵點跟高度場對齊。"),
            ("細節", "共用裸 seed 的場會有隱性相關——"
                    "古林深處若總是長在高地旁，玩家會學到不存在的規律。"),
            ("啟示", "一個 seed 多個消費者時，用域名 XOR 切開——"
                    "域常數就是這個場的命名空間。"),
        ],
        "code": 'height = Field(x, y, seed ^ DOM_HEIGHT);\nveget  = Field(x, y, seed ^ DOM_VEGET);\nfeat   = CollatzField01(x, y,\n         seed ^ 0xC011A7u); // 域名隔離',
        "narration": "第三個設計是 seed 的用法。"
                     "同一張圖的高度場、植被場、特徵場各用一個域常數 XOR——"
                     "共用裸 seed 的場會有隱性相關，"
                     "古林若總是長在高地旁，玩家會學到不存在的規律。"
                     "域常數就是每個場的命名空間。",
    },
    {
        "tag": "設計 4",
        "title": "表現層 vs 規則層：一個格子兩個名字",
        "sections": [
            ("做法", "DeepForest 在 JSON 裡 type 仍寫 forest——"
                    "規則層吃同一種減傷；note 寫「古林深處」給玩家看。"),
            ("細節", "cell 枚舉多一種，但 type 欄位不擴——"
                    "loader、modifier、AI 全部不用知道新類別存在。"),
            ("啟示", "新增內容時分清「給規則的名字」和「給人的名字」——"
                    "規則名不動，下游零修改。"),
        ],
        "code": 'if (p.cell == Cell::DeepForest)\n    js << "\"type\": \"forest\""     // 規則層不變\n     << "\"note\": \"古林深處...\"";   // 表現層新增',
        "narration": "第四個設計：一個格子兩個名字。"
                     "古林深處在規則層仍是 forest，吃同樣的守方減傷；"
                     "note 欄位才寫古林深處給玩家看。"
                     "規則名不動，loader、modifier、AI 都不用改——"
                     "新增內容要分清給規則的名字和給人的名字。",
    },
    {
        "tag": "設計 5",
        "title": "opt-in 閾值：同一把尺第三次出現",
        "sections": [
            ("做法", "featureThr<=0 關閉、jamRadius<=0 關閉、"
                    "assuranceSample<=0 不查——三個功能同一把尺。"),
            ("細節", "預設值都是不啟動；開了才付成本。"
                    "MapGenTest 專門斷言閾值歸零時特徵點消失。"),
            ("啟示", "「小於等於零等於關」變成 repo 慣例後，"
                    "新功能的預設行為就不用再討論——慣例即文件。"),
        ],
        "code": 'featureThr <= 0     → 不產特徵點\njamRadius <= 0      → 不算擁擠\nassuranceSample <= 0 → 不出查證句\n// 預設關閉 = 零回歸風險',
        "narration": "第五個設計是慣例的力量。"
                     "特徵閾值、擁擠半徑、抽驗筆數——"
                     "三個功能都用小於等於零代表關閉，預設都是不啟動。"
                     "同一把尺出現三次之後，"
                     "新功能的預設行為不用再討論——慣例本身就是文件。",
    },
    {
        "tag": "設計 6",
        "title": "擁擠不分陣營",
        "sections": [
            ("做法", "ApplyJamming 計算密度時敵我一律計入——"
                    "渡口堵成一團時，堵住你的可能是我方預備隊。"),
            ("細節", "物理層不認陣營：crowdFactor 是密度的函數，"
                    "不是外交的函數。想繞開？疏開隊形。"),
            ("啟示", "底層模型越誠實，湧現的戰術越真——"
                    "預備隊別站太密，是玩家自己悟出來的紀律。"),
        ],
        "code": 'for (auto* other : squads) {\n    // 敵我一律計入——擁擠是物理不是外交\n    density += CrowdDensity(s, other, r);\n}\ns.SetCrowdFactor(JammingSpeedFactor(density));',
        "narration": "第六個設計：擁擠不分陣營。"
                     "計算密度時敵我一律計入——渡口堵成一團時，"
                     "堵住你的可能是我方自己的預備隊。"
                     "底層模型越誠實，湧現的戰術越真："
                     "疏開隊形不是規則強制的，是玩家自己悟出的紀律。",
    },
    {
        "tag": "設計 7",
        "title": "一個因子，三方管線",
        "sections": [
            ("做法", "crowdFactor 的生命週期分三段："
                    "BattleController 算密度、Squad 存當前值、"
                    "GetEffectiveSpeed 消費。"),
            ("細節", "Squad 不知道密度怎麼算、Controller 不知道"
                    "速度怎麼用——SetCrowdFactor 帶 clamp，"
                    "非法值在寫入點被擋。"),
            ("啟示", "跨系統因子要切清「誰算、誰存、誰用」——"
                    "三方各管一段，測試才能各測一段。"),
        ],
        "code": 'controller.ApplyJamming();     // 算\nsquad.SetCrowdFactor(f);      // 存（clamp 0~1）\nspeed = base * crowdFactor;   // 用',
        "narration": "第七個設計是管線分工。"
                     "擁擠因子分三段：控制器算密度、小隊存當前值、"
                     "有效速度消費它。小隊不知道密度怎麼來，"
                     "控制器不知道速度怎麼用，"
                     "寫入點用 clamp 擋住非法值。"
                     "誰算、誰存、誰用切乾淨，測試才能各測一段。",
    },
    {
        "tag": "設計 8",
        "title": "改判的收尾動作",
        "sections": [
            ("做法", "備選翻採用不是改一個字——研究文件加「第二批」"
                    "落地記錄：產出檔、消費者、擱置理由為何消失。"),
            ("細節", "MapGenTest 三斷言鎖行為：降閾值要產格、"
                    "關閉要消失、產物要能過 loader——"
                    "新格子類別第一個風險就是序列化鏈。"),
            ("啟示", "研究文件的價值在「為什麼」欄——"
                    "改判記錄讓下一個備選項知道翻身需要什麼證據。"),
        ],
        "code": 'Check(f1.find("古林深處") != npos, "降閾值產格");\nCheck(GenerateJson(42, off).find("古林")\n      == npos, "關閉即消失");\nCheck(fm.LoadFromString(f1), "過 loader");',
        "narration": "第八個設計是改判的收尾。"
                     "研究文件不只改表格，還補第二批落地記錄："
                     "產出什麼檔、誰消費、當初的擱置理由為什麼消失。"
                     "測試三個斷言鎖住行為：降閾值要產格、"
                     "關閉要消失、產物要過 loader。"
                     "改判記錄會告訴下一個備選項，翻身需要什麼證據。",
    },
    {
        "kind": "cover",
        "title": "第十三集清單",
        "lines": ["備選寫翻身條件 · 重尾是地形語言 · seed 用域名切",
                  "規則名不動表現名新增 · 小於等於零即關",
                  "擁擠不分敵我 · 誰算誰存誰用 · 改判要記錄"],
        "narration": "第十三集收在這裡。貨架的完整生命週期："
                     "評估時寫清楚翻身條件，落地時守住分層慣例，"
                     "改判時記錄理由消失。"
                     "研究不是一次性採購，是會回頭的投資。",
    },
]

SLIDES_EP14 = [
    {
        "kind": "cover",
        "title": "拆倉實彈演習",
        "lines": ["從實戰學 C++ 第十四集",
                  "一份 CMakeLists 拆成兩份 · 先演練再搬家"],
        "narration": "第十四集是拆倉第二階段。"
                     "上一階段固化邊界，這一階段在 staging 目錄裡"
                     "真的把一千行的根清單拆成兩份："
                     "引擎五百行、遊戲六百行。"
                     "先演練，再搬家。",
    },
    {
        "tag": "設計 1",
        "title": "在 repo 外先演一遍",
        "sections": [
            ("做法", "_split_stage/ 目錄放兩份候選 CMakeLists——"
                    "engine-src 與 game-src，不碰真 repo。"),
            ("細節", "同目錄留演習紀錄：paths 清單、diff、"
                    "未追蹤檔清單——拆分的每一步都有產物可審。"),
            ("啟示", "不可逆操作前先找個能失敗的地方失敗——"
                    "staging 的意義是把「猜」變成「看過」。"),
        ],
        "code": '_split_stage/\n  engine-src/CMakeLists.txt  # 508 行\n  game-src/CMakeLists.txt  # 607 行\n  game-side.diff             # 審查用',
        "narration": "第一個設計：先在 repo 外面演一遍。"
                     "staging 目錄放兩份候選清單，配 paths 清單和 diff——"
                     "每一步都有產物可審。"
                     "不可逆操作之前，先找個能失敗的地方失敗。"
                     "staging 的意義是把猜變成看過。",
    },
    {
        "tag": "設計 2",
        "title": "一千行不是對半拆",
        "sections": [
            ("做法", "508+607 不等於 1037 除以二——兩邊各自保留"
                    "自己需要的：依賴解析歸引擎，遊戲 AI 庫歸遊戲。"),
            ("細節", "引擎側留 GLFW/glad/OpenGL 與 install 規則；"
                    "遊戲側留 RL/NLP/LLM 等十個 AI 庫加 "
                    "Gameplay/Campaign——重疊的是編譯旗標與慣例。"),
            ("啟示", "拆檔案的單位是「職責」不是「行數」——"
                    "每邊要能獨立回答：我編什麼、我測什麼。"),
        ],
        "code": '# engine-src：PotatoEngine + 依賴 + install\n# game-src ：MingGoRTS + AI 庫×10\n#            + Gameplay/Campaign\n# 重疊的只有旗標與慣例',
        "narration": "第二個設計：拆檔不是對半分。"
                     "引擎側拿走依賴解析和 install 規則，"
                     "遊戲側拿走十個 AI 庫和玩法層——"
                     "重疊的只有編譯旗標這種各邊都要的東西。"
                     "拆分單位是職責不是行數："
                     "每邊要能獨立回答我編什麼、我測什麼。",
    },
    {
        "tag": "設計 3",
        "title": "add_subdirectory 勝過 find_package",
        "sections": [
            ("做法", "遊戲側用 POTATO_ENGINE_ROOT 指向 sibling "
                    "checkout，直接 add_subdirectory 引擎源碼。"),
            ("細節", "不發 binary 包、不走 find_package——"
                    "引擎還在同節奏開發，源碼消費比成品消費誠實；"
                    "改引擎當場看見遊戲怎麼壞。"),
            ("啟示", "同團隊的庫用源碼相依，跨團隊的庫才發包——"
                    "耦合方式要跟溝通成本匹配。"),
        ],
        "code": 'add_subdirectory(${POTATO_ENGINE_ROOT}\n    ${CMAKE_BINARY_DIR}/PotatoEngine)\n# sibling checkout，不發包',
        "narration": "第三個設計：消費方式選源碼。"
                     "遊戲側用一個 cache 變數指向隔壁的引擎 checkout，"
                     "直接 add 它的目錄——不發二進位包、"
                     "不走 find_package。"
                     "引擎還在同節奏開發，源碼相依讓改動當場見效。"
                     "耦合方式要跟溝通成本匹配。",
    },
    {
        "tag": "設計 4",
        "title": "standalone 守衛是契約",
        "sections": [
            ("做法", "引擎清單的 examples/tests/install 整段包在 "
                    "SOURCE_DIR 等於 CURRENT_SOURCE_DIR 的判斷裡。"),
            ("細節", "被 add_subdirectory 消費時只產出函式庫 "
                    "target——引擎自己的測試不會跑進遊戲的建置圖。"),
            ("啟示", "「被消費時長什麼樣」要寫成條件而不是默契——"
                    "守衛就是兩個 repo 之間的介面宣告。"),
        ],
        "code": 'if(CMAKE_SOURCE_DIR STREQUAL\n   CMAKE_CURRENT_SOURCE_DIR)\n  # examples/tests/install 僅獨立建置\nendif() # STANDALONE',
        "narration": "第四個設計：standalone 守衛。"
                     "引擎自己的範例和測試整段包在原始目錄判斷裡——"
                     "被遊戲當子目錄消費時，只產出函式庫 target。"
                     "被消費時長什麼樣要寫成條件，不靠默契——"
                     "守衛就是兩個 repo 之間的介面宣告。",
    },
    {
        "tag": "設計 5",
        "title": "project() 是身份證",
        "sections": [
            ("做法", "遊戲側 project 改名 MingGoRTS——"
                    "不再共用 PotatoEngine 的專案名。"),
            ("細節", "專案名影響版本變數、VS 方案名、CPack 包名——"
                    "拆倉時改名是把「這是另一個產品」寫進建置層。"),
            ("啟示", "分家不只是目錄分開——project 名、版本號、"
                    "輸出命名都要跟著換身份。"),
        ],
        "code": '# 舊：project(PotatoEngine ...) 共用\n# 新：\nproject(MingGoRTS VERSION 1.0.0 ...)\n# 遊戲有了自己的建置身份',
        "narration": "第五個設計：project 改名。"
                     "遊戲側不再共用引擎的專案名，改成 MingGoRTS——"
                     "專案名牽動版本變數、方案名、打包名。"
                     "分家不只是目錄分開，身份要一路換到建置層。",
    },
    {
        "tag": "設計 6",
        "title": "install() 是留給未來的門",
        "sections": [
            ("做法", "引擎側保留十六個目錄的 header install 規則——"
                    "裝出去就是 include/PotatoEngine 的乾淨版面。"),
            ("細節", "現在沒人用 install，但介面先留好——"
                    "哪天引擎要發 SDK 或給第三方用，不用再補規則。"),
            ("啟示", "消費介面在拆分時一次做到位——"
                    "事後補 install 的代價是重排 include 樹。"),
        ],
        "code": 'install(DIRECTORY Rendering/\n    DESTINATION include/PotatoEngine\n    FILES_MATCHING PATTERN "*.h")\n# ×16 目錄——SDK 版面預留',
        "narration": "第六個設計：install 是留給未來的門。"
                     "引擎側把十六個目錄的標頭安裝規則都寫好——"
                     "現在沒人用，但哪天要發 SDK，版面已經就位。"
                     "消費介面要在拆分當下做到位，"
                     "事後補的代價是重排整棵 include 樹。",
    },
    {
        "tag": "設計 7",
        "title": "測試跟著被測物走",
        "sections": [
            ("做法", "兩邊各有自己的 POTATO_TESTS 與 "
                    "enable_testing——引擎測引擎的、遊戲測遊戲的。"),
            ("細節", "判準只看連結：連 PotatoEngine 的測試留引擎側、"
                    "連 Gameplay 的歸遊戲側——誰的元件壞了誰的 "
                    "ctest 紅，責任歸屬清楚。"),
            ("啟示", "拆倉後的 ctest 綠燈才有意義——"
                    "紅燈要能直接指出是哪個 repo 的責任。"),
        ],
        "code": '# engine: MinimalTest/PhysicsTest/...\n# game  : Gameplay*/Campaign*/Ledger*\n# 連結誰的庫，測試歸誰',
        "narration": "第七個設計：測試跟著被測物走。"
                     "兩份清單各自帶自己的測試清單，"
                     "判準只看連結——連引擎庫的測試留引擎側，"
                     "連玩法庫的歸遊戲側。"
                     "拆完之後 ctest 的紅綠燈才有意義："
                     "紅了直接指出是哪個 repo 的責任。",
    },
    {
        "tag": "設計 8",
        "title": "演習紀錄也是產物",
        "sections": [
            ("做法", "staging 目錄連 paths 清單、diff、"
                    "未追蹤檔清單一起留——審查者看的是過程不是結果。"),
            ("細節", "uncommitted.diff 記錄 staging 當下工作樹狀態——"
                    "拆分清單是對哪個版本拆的，有憑有據。"),
            ("啟示", "大改動的審查成本在重建上下文——"
                    "把演習的副產品整理好，審查者就不用猜。"),
        ],
        "code": 'engine-paths.txt    # 引擎側檔案清單\ngame-side.diff      # 遊戲側差異\nuncommitted-status  # 當下工作樹快照',
        "narration": "第八個設計：演習紀錄也是產物。"
                     "清單、diff、工作樹快照全部留在 staging——"
                     "審查者要看的是過程不是結果。"
                     "大改動最貴的是重建上下文，"
                     "副產品整理好，審查的人就不用猜。",
    },
    {
        "kind": "cover",
        "title": "第十四集清單",
        "lines": ["repo 外先演 · 按職責拆不按行數 · 源碼相依",
                  "standalone 守衛是介面 · project 換身份",
                  "install 留門 · 測試跟被測物走 · 紀錄即產物"],
        "narration": "第十四集收在這裡。拆倉的完整手法："
                     "先演練、按職責拆、守衛寫成條件、"
                     "測試跟著被測物走。"
                     "拆得乾淨的標誌是——兩邊都能獨立說清楚自己是誰。",
    },
]

SLIDES_EP15 = [
    {
        "kind": "cover",
        "title": "測試的遷徙",
        "lines": ["從實戰學 C++ 第十五集",
                  "拆倉收尾 · if(TARGET) 守衛 · 七十一支測試分家"],
        "narration": "第十五集接續拆倉演習——這次看測試怎麼搬家。"
                     "七十一支測試分成兩邊：引擎二十八、遊戲四十三。"
                     "清單怎麼切、註冊怎麼寫、舊家怎麼除名，"
                     "每個細節都藏著一個判斷。",
    },
    {
        "tag": "設計 1",
        "title": "連結決定歸屬",
        "sections": [
            ("做法", "QuasiModelsTest 連 PotatoEngine → 搬進引擎側；"
                    "JammingBattleTest 連 Gameplay → 留在遊戲側。"),
            ("細節", "判定不看測什麼功能、看 target_link_libraries "
                    "寫了誰——連結邊就是 repo 邊。"),
            ("啟示", "搬家前先問「它依賴誰」——依賴關係在,"
                    "歸屬就是唯一解，不用開會決定。"),
        ],
        "code": 'QuasiModelsTest  → PotatoEngine → 引擎側\nJammingBattleTest → Gameplay   → 遊戲側\n# 連結邊 = repo 邊',
        "narration": "第一個設計：連結決定歸屬。"
                     "模型測試連引擎庫就搬進引擎側，"
                     "玩法測試連 Gameplay 就留遊戲側——"
                     "判定不看它測什麼，看 target_link_libraries "
                     "寫了誰。依賴關係在，歸屬就是唯一解。",
    },
    {
        "tag": "設計 2",
        "title": "if(TARGET)：清單可以超前",
        "sections": [
            ("做法", "註冊改成 foreach 加 if(TARGET)——清單裡的名字"
                    "若這個 repo 沒有對應 target，靜靜跳過。"),
            ("細節", "兩邊可以共用命名慣例甚至暫存名字——"
                    "清單是意圖，target 是事實，守衛負責對齊兩者。"),
            ("啟示", "註冊表設計成「寫了沒有也不會死」——"
                    "搬遷期間清單先行、target 後到，都不會斷建置。"),
        ],
        "code": 'foreach(t IN LISTS POTATO_TESTS)\n  if(TARGET ${t})          # 沒有就跳過\n    add_test(NAME ${t} COMMAND ${t})\n  endif()\nendforeach()',
        "narration": "第二個設計：註冊加守衛。"
                     "foreach 配 if(TARGET)——清單裡的名字"
                     "如果這個 repo 沒有對應 target，就靜靜跳過。"
                     "清單是意圖、target 是事實，守衛對齊兩者。"
                     "搬遷期間清單先行、target 後到，都不會斷建置。",
    },
    {
        "tag": "設計 3",
        "title": "工作目錄是隱形依賴",
        "sections": [
            ("做法", "每支測試設 WORKING_DIRECTORY 到 bin——"
                    "因為有測試要載同目錄的 DLL（FakeCheat.dll）。"),
            ("細節", "ctest 預設工作目錄是測試的 build 目錄——"
                    "不設這行，載 DLL 的測試會在錯的地方找檔案。"),
            ("啟示", "「測試在本機跑得起來、ctest 卻紅」的經典元兇——"
                    "可執行檔的隱性依賴：同目錄的 DLL、相對路徑的檔案。"),
        ],
        "code": 'set_tests_properties(${t} PROPERTIES\n    WORKING_DIRECTORY\n    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})\n# FakeCheat.dll 就在 bin/',
        "narration": "第三個設計：工作目錄是隱形依賴。"
                     "每支測試的工作目錄設到 bin——"
                     "因為紅隊測試要載同目錄的假作弊 DLL。"
                     "不設這行，ctest 會在錯的地方找檔案。"
                     "本機跑得動、ctest 卻紅，元兇常常就是這個。",
    },
    {
        "tag": "設計 4",
        "title": "拆倉會照出陳年累贅",
        "sections": [
            ("做法", "遊戲側清單裡發現重複的 enable_testing()——"
                    "演練過程順手清掉。"),
            ("細節", "monorepo 時期多寫一次沒人痛；拆成兩份後，"
                    "每一份都要被獨立審視，冗餘無處可藏。"),
            ("啟示", "拆分的副作用是免費審計——"
                    "一行一行重讀的過程，本身就是程式碼審查。"),
        ],
        "code": '-enable_testing()\n-enable_testing()  # 重複，拆倉時刪\n+enable_testing()',
        "narration": "第四個設計是拆倉的副作用。"
                     "遊戲側發現重複的 enable_testing——"
                     "monorepo 時期多寫一次沒人痛，"
                     "拆成兩份後每行都要被獨立審視。"
                     "一行行重讀的過程，本身就是免費的程式碼審計。",
    },
    {
        "tag": "設計 5",
        "title": "除名要和落戶同批",
        "sections": [
            ("做法", "monorepo 同步移除已搬走的測試——"
                    "target 定義和 POTATO_TESTS 條目一起刪。"),
            ("細節", "只刪 target 留名字：if(TARGET) 會讓它安靜消失、"
                    "看起來像沒事；只刪名字留 target：編了不測。"),
            ("啟示", "遷徙的最後一步是舊地址的註銷——"
                    "兩個編輯點（定義+清單）要同批動。"),
        ],
        "code": '# monorepo 側：\n- add_executable(JammingBattleTest ...)\n- target_link_libraries(... Gameplay)\n- POTATO_TESTS 條目\n# 三處同批刪',
        "narration": "第五個設計：除名和落戶同批。"
                     "monorepo 把搬走的測試連 target 定義"
                     "帶清單條目一起刪。"
                     "只刪一邊會留下兩種安靜的錯："
                     "編了不測，或清單裡躺著不存在名字。"
                     "遷徙的最後一步，是舊地址的註銷。",
    },
    {
        "tag": "設計 6",
        "title": "兩份清單，一條指令",
        "sections": [
            ("做法", "兩邊 POTATO_TESTS 各自獨立但同名——"
                    "ctest -C Release 在任何一邊都能跑。"),
            ("細節", "肌肉記憶不破壞：進 engine build 目錄跑 28 支，"
                    "進 game build 目錄跑 43 支——指令一樣。"),
            ("啟示", "工具面對使用者的 API 不要因拆分改變——"
                    "變的是清單內容，不變的是驗證手勢。"),
        ],
        "code": 'cd build-engine && ctest -C Release  # 28 支\ncd build-game   && ctest -C Release  # 43 支\n# 同一個手勢',
        "narration": "第六個設計：指令不變。"
                     "兩邊清單各自獨立，但名字都叫 POTATO_TESTS、"
                     "驗證都是 ctest 同一條指令。"
                     "肌肉記憶不破壞——變的是清單內容，"
                     "不變的是驗證手勢。",
    },
    {
        "tag": "設計 7",
        "title": "全量清單是一次盤點",
        "sections": [
            ("做法", "拆倉逼出完整測試目錄：引擎側二十八支、"
                    "遊戲側四十三支——每支都要被點名歸屬。"),
            ("細節", "Ledger 系全數歸遊戲側（LedgerChain 在 "
                    "Gameplay）；Rendering/Physics 測試歸引擎——"
                    "清單本身就是依賴圖的寫照。"),
            ("啟示", "拆完的兩份清單是意外的資產——"
                    "它比任何文件都準確地描述了系統邊界。"),
        ],
        "code": '# engine: Render/Physics/AI/Security… 28\n# game  : Ledger×8/Battle×12/Campaign×5… 43\n# 清單 = 依賴圖的寫照',
        "narration": "第七個設計：全量清單是盤點。"
                     "每支測試都被點名歸屬——Ledger 系全數歸遊戲，"
                     "渲染物理歸引擎。"
                     "拆完的兩份清單是意外的資產："
                     "它比任何文件都準確地描述了系統邊界。",
    },
    {
        "tag": "設計 8",
        "title": "遷徙的完成定義",
        "sections": [
            ("做法", "一支測試搬完 = 新家能跑 + 舊家除名 + "
                    "兩側 ctest 數字加起來等於搬前總數。"),
            ("細節", "28+43=71 對得上 monorepo 的清單長度——"
                    "帳要平，測試數量也是帳。"),
            ("啟示", "遷徙不是「搬過去」是「平了帳」——"
                    "兩側綠燈且總數對得上，才算搬完。"),
        ],
        "code": '# 搬前：monorepo 71 支\n# 搬後：28 + 43 = 71\n# 帳平了才算完成',
        "narration": "第八個設計收束：遷徙的完成定義。"
                     "新家能跑、舊家除名、兩側總數對得上搬前——"
                     "二十八加四十三等於七十一，帳要平。"
                     "遷徙不是搬過去，是平了帳。",
    },
    {
        "kind": "cover",
        "title": "第十五集清單",
        "lines": ["連結定歸屬 · if(TARGET) 讓清單超前 · 工作目錄是隱形依賴",
                  "拆倉照出累贅 · 除名落戶同批 · 指令不變",
                  "清單即依賴圖 · 帳平才算搬完"],
        "narration": "第十五集收在這裡。測試搬家的完整手法："
                     "歸屬看連結、註冊加守衛、除名同批、"
                     "最後對帳。拆倉到了這一步，"
                     "剩下的是把 staging 變成真 repo。",
    },
]

SLIDES_EP16 = [
    {
        "kind": "cover",
        "title": "一份規格的解剖",
        "lines": ["從實戰學 C++ 第十六集",
                  "D-1 MythLayer spec · 意圖凍結 · 權限階梯 · I/O 矩陣"],
        "narration": "第十六集不講程式，講寫給程式的合約。"
                     "D-1 神話滲透層的規格書剛完成——"
                     "它幾乎是「spec 該長什麼樣」的教科書："
                     "凍結區塊、權限階梯、I/O 矩陣、逐項驗證指令。"
                     "這一集把它拆開看。",
    },
    {
        "tag": "設計 1",
        "title": "意圖區塊會凍結",
        "sections": [
            ("做法", "spec 的 Intent/Boundaries/I-O 矩陣整段包在 "
                    "frozen-after-approval 標記裡——核准後不准改。"),
            ("細節", "改意圖要「人類重新談判」——"
                    "標記不是給編譯器看的，是給執行任務的 agent 看的。"),
            ("啟示", "把「哪些不能動」寫進文件本身——"
                    "凍結宣告讓下游敢放心引用這份規格。"),
        ],
        "code": '<frozen-after-approval\n  reason="human-owned intent —\n  do not modify unless\n  human renegotiates">',
        "narration": "第一個設計：意圖區塊會凍結。"
                     "規格的核心段落包在凍結標記裡——核准後不准改，"
                     "要改得人類重新談判。"
                     "這個標記不是給編譯器，是給執行任務的 agent 看的："
                     "哪些不能動，寫進文件本身。",
    },
    {
        "tag": "設計 2",
        "title": "權限是階梯不是開關",
        "sections": [
            ("做法", "Boundaries 分三層：Always 直接做、"
                    "Ask First 先問、Never 永不碰。"),
            ("細節", "Ask First 不是模糊的「謹慎」——"
                    "它點名具體情境：接 BattleController 要問、"
                    "改 MythLog schema 要問。"),
            ("啟示", "權限階梯把「自主度」變成資料——"
                    "執行者不用猜這件事該不該請示。"),
        ],
        "code": 'Always:   四階 enum、章內單調、record-is-truth\nAskFirst: 接 BattleController、改既有 schema\nNever:    做視覺/神社/貨幣、Gameplay 含 Campaign',
        "narration": "第二個設計：權限是階梯。"
                     "三層——Always 直接做、Ask First 先問、Never 永不碰。"
                     "中間層不是模糊的謹慎，它點名具體情境："
                     "接戰鬥控制器要問、改既有 schema 要問。"
                     "自主度變成資料，執行者不用猜該不該請示。",
    },
    {
        "tag": "設計 3",
        "title": "I/O 矩陣先於實作",
        "sections": [
            ("做法", "十一列情境表格：輸入/狀態 → 預期輸出 → "
                    "錯誤處理——在寫任何一行程式之前。"),
            ("細節", "涵蓋跳階、跨章重推導、壞檔、缺段降級、"
                    "渲染唯讀——邊界情形是設計文件，不是測試才想。"),
            ("啟示", "矩陣的每一列幾乎就是一個測試案例——"
                    "spec 寫得好，測試案例是抄的，不是想的。"),
        ],
        "code": '| 單次大壓力 Feed | 連跨多階，每階各錄 |\n| 無 myth_layer 段舊檔 | 載入成功全 Quiet |\n| Level() 存取 | const，無 mutator 外漏 |',
        "narration": "第三個設計：I/O 矩陣先於實作。"
                     "十一列情境——跳階、跨章重推導、壞檔拒載、"
                     "舊檔缺段降級——全部在寫程式之前定好。"
                     "矩陣的每一列幾乎就是一個測試案例："
                     "規格寫得好，測試是抄出來的，不是想出來的。",
    },
    {
        "tag": "設計 4",
        "title": "記憶需要兩個變數",
        "sections": [
            ("做法", "每區域存 {level, pressure} 兩個量——"
                    "壓力是流體可加可減，等級是棘輪只升不降。"),
            ("細節", "單變數做不到「章內不遺忘」："
                    "單看壓力，仁政會洗掉暴行；單看等級，"
                    "壓力微振盪會反覆觸發轉換。"),
            ("啟示", "「有記憶的系統」= 快變數 + 慢變數——"
                    "一個記當下、一個記歷史，分開才能各自演化。"),
        ],
        "code": 'struct Region {\n    Seepage level;    // 棘輪：只升不降\n    float   pressure; // 流體：可加可減\n};',
        "narration": "第四個設計：記憶需要兩個變數。"
                     "每個區域存等級和壓力——壓力是流體可加可減，"
                     "等級是棘輪只升不降。"
                     "單變數做不到章內不遺忘：單看壓力，"
                     "仁政會洗掉暴行；單看等級，"
                     "微振盪會反覆觸發轉換。"
                     "有記憶的系統等於快變數加慢變數。",
    },
    {
        "tag": "設計 5",
        "title": "章節邊界是重推導點",
        "sections": [
            ("做法", "AdvanceChapter 觸發 DeriveFrom——"
                    "用治理快照（墮落/秩序/民怨）重新注入壓力再棘輪。"),
            ("細節", "跨章同樣只升不降，但新章會重新計價治理——"
                    "「土地不會遺忘」是機制不是台詞。"),
            ("啟示", "持久狀態需要明確的「結算時機」——"
                    "章節邊界就是那個時機，快照進、壓力出。"),
        ],
        "code": 'AdvanceChapter()\n  → Myths().DeriveFrom(govSnapshot)\n// 治理帳 → 壓力注入 → 棘輪推進\n// 清廉快照也拉不回等級',
        "narration": "第五個設計：章節邊界是重推導點。"
                     "過章時用治理快照重新注入壓力再棘輪——"
                     "墮落高、秩序低的帳目會把各區域往前推，"
                     "但清廉的治理也拉不回已升的等級。"
                     "土地不會遺忘，是機制不是台詞。",
    },
    {
        "tag": "設計 6",
        "title": "神明有脾氣，脾氣有數值",
        "sections": [
            ("做法", "BindSpirit 把區域綁守護靈——favor 低於 "
                    "kAngryFavor 時，該區暴行的壓力加成。"),
            ("細節", "favor 表夾 [0,100]、AdjustFavor 供 D-3 "
                    "天命兌換預留——耦合點在 spec 裡就宣告了。"),
            ("啟示", "「怒神降罪」翻成機制只是一行："
                    "某個欄位低於閾值時乘個係數——"
                    "敘事和數值之間的橋是表格。"),
        ],
        "code": 'if (Favor(spirit) < kAngryFavor)\n    pressure += delta * kAngryMul;\n// favor [0,100] 夾取，\n// D-3 天命兌換讀同一張表',
        "narration": "第六個設計：神明有脾氣，脾氣有數值。"
                     "區域綁守護靈，favor 低於怒神閾值時，"
                     "暴行事件的壓力加乘。"
                     "favor 表順便為天命兌換預留欄位——"
                     "敘事和數值之間的橋，是一張夾取在零到一百的表。",
    },
    {
        "tag": "設計 7",
        "title": "轉換要留痕跡",
        "sections": [
            ("做法", "每次跨階產生 transition 記錄（區域/從/到/章節），"
                    "並經回呼發一筆 MythEvent 給 MythLog。"),
            ("細節", "跳階時每階各錄一筆——連跨三階留三筆痕跡，"
                    "不是一筆「0 到 3」。"),
            ("啟示", "狀態機的歷史和狀態本身同價——"
                    "「怎麼到這裡的」要能完整回放。"),
        ],
        "code": 'for (; level < next; ++level)\n    Record({region, level, level+1, ch});\n    cb(MythEvent{...}); // 每階一筆',
        "narration": "第七個設計：轉換要留痕跡。"
                     "每次跨階錄一筆轉換記錄，並回呼給神話日誌——"
                     "連跨三階就留三筆，不是一筆零到三。"
                     "狀態機的歷史和狀態同價："
                     "怎麼到這裡的，要能完整回放。",
    },
    {
        "tag": "設計 8",
        "title": "Never 清單是邊界守護",
        "sections": [
            ("做法", "spec 明列不做什麼：不做視覺（D-5）、不做神社"
                    "（D-2）、不做貨幣（D-3）、不做入侵觸發（D-4）。"),
            ("細節", "每個 Never 都對應一個排隊中的 epic——"
                    "不做不是沒想到，是刻意留給對的任務。"),
            ("啟示", "範圍控制的最強形式是寫下來——"
                    "「這個 spec 不做」比「以後再說」更能防止漫溢。"),
        ],
        "code": 'Never: 視覺(D-5) 神社(D-2) 貨幣(D-3) 入侵(D-4)\n// 每個 Never = 一個已在排隊的 epic\n// 留白是有主的',
        "narration": "第八個設計：Never 清單是邊界守護。"
                     "規格明列不做視覺、不做神社、不做貨幣、不做入侵——"
                     "每個不做都對應一個排隊中的史詩。"
                     "留白不是沒想到，是有主的。"
                     "範圍控制的最強形式，是把它寫下來。",
    },
    {
        "kind": "cover",
        "title": "第十六集清單",
        "lines": ["意圖凍結 · 權限三層 · I/O 矩陣先於程式",
                  "記憶 = 快變數+慢變數 · 章節是結算點 · 怒神是數值",
                  "轉換留痕 · Never 留白有主"],
        "narration": "第十六集收在這裡。好規格的共同點："
                     "把不能動的寫死、把要問的點名、"
                     "把不做的留白標出主人。"
                     "合約清楚，執行的人才能真正自主。",
    },
]

SLIDES_EP17 = [
    {
        "kind": "cover",
        "title": "不戰而勝的三種帳",
        "lines": ["從實戰學 C++ 第十七集",
                  "E-1 無戰路徑 · 三種貨幣 · 帳本先於機制"],
        "narration": "第十七集講不戰而勝怎麼入帳。"
                     "E-1 無戰路徑落地了：談判、嚇阻、顛覆——"
                     "三條路各自用不同的貨幣計價。"
                     "而神話滲透層的程式也寫完了，"
                     "從規格到程式正好對照著看。",
    },
    {
        "tag": "設計 1",
        "title": "三條路，三種貨幣",
        "sections": [
            ("做法", "NoBattleDef 三路徑獨立開關：談判收民心淨額、"
                    "嚇阻看軍威門檻、顛覆數敵將 verified 判詞條數。"),
            ("細節", "同一個 requirement 欄位，語義隨路徑而異——"
                    "註解裡明寫各路的計價單位，讀者不靠猜。"),
            ("啟示", "「無戰」不是一個選項是三個——"
                    "每條路付的帳不同，玩家才真的在選擇手段。"),
        ],
        "code": 'negotiation = 民心消費額（成功即支出）\ndeterrence  = 軍威門檻（不消耗）\nsubversion  = verified 判詞條數（情報帳）',
        "narration": "第一個設計：三條路三種貨幣。"
                     "談判消耗民心淨額、嚇阻看軍威門檻、"
                     "顛覆數敵將的查實判詞條數。"
                     "同一個欄位語義隨路徑而異——"
                     "無戰不是一個選項是三個，"
                     "付的帳不同，玩家才真的在選手段。",
    },
    {
        "tag": "設計 2",
        "title": "消耗型與門檻型要分清",
        "sections": [
            ("做法", "談判的民心是支出——用了就扣；"
                    "嚇阻的軍威是門檻——達標就開放，不消耗。"),
            ("細節", "兩種經濟語義差在「用不用還」："
                    "軍威達標可以連續嚇阻多章，民心每談一次都要再掙。"),
            ("啟示", "門檻資源鼓勵累積、消耗資源鼓勵權衡——"
                    "混用的話，玩家算不清每個選擇的機會成本。"),
        ],
        "code": 'if (civilNet >= req) { Spend(civilNet, req); Pass(); }\nif (armyNet >= req)  Pass(); // 達標即開，不扣\n// 一個是錢包，一個是門卡',
        "narration": "第二個設計：消耗型和門檻型要分清。"
                     "民心是錢包——談一次扣一次；"
                     "軍威是門卡——達標就開放，不消耗。"
                     "門檻資源鼓勵累積、消耗資源鼓勵權衡，"
                     "混在一起玩家就算不清機會成本。",
    },
    {
        "tag": "設計 3",
        "title": "情報帳是戰略貨幣",
        "sections": [
            ("做法", "顛覆路徑的代價不是資源是紀錄——"
                    "敵將帳上 verified 判詞條數達標才開放。"),
            ("細節", "要求的是「查實過的」判詞不是偵察次數——"
                    "情報品質（可查證）變成可累積的戰略資產。"),
            ("啟示", "讓「資料品質」成為貨幣，"
                    "玩家就會為帳目可信度投資——"
                    "這是 GBAA 思路進玩法的最直接一例。"),
        ],
        "code": 'subversion.requirement = 3;\n// 敵將 verified 判詞 >= 3 才開放\n// 偵察不算數，查實才算',
        "narration": "第三個設計：情報帳是戰略貨幣。"
                     "顛覆路徑收的不是資源是紀錄——"
                     "敵將帳上查實過的判詞達標才開放。"
                     "偵察次數不算數，可查證的品質才算——"
                     "玩家因此願意為帳目可信度投資。",
    },
    {
        "tag": "設計 4",
        "title": "帳本先於機制",
        "sections": [
            ("做法", "EntrySource 加嚇阻/顛覆兩個來源、"
                    "GeneralDisposition 加議和/懾服——"
                    "機制未動，帳目先就位。"),
            ("細節", "記錄型別先行擴充：等無戰結算真的發生時，"
                    "分錄來源和結局處置都已經有名可記。"),
            ("啟示", "record-is-truth 的 repo 裡，"
                    "新機制的第一步永遠是「它入帳叫什麼」——"
                    "名字先於實作。"),
        ],
        "code": 'enum EntrySource { ..., Deterrence, Subversion };\nenum GeneralDisposition { ..., Negotiated, Intimidated };\n// 機制未到，帳目先有名',
        "narration": "第四個設計：帳本先於機制。"
                     "來源列舉先加嚇阻和顛覆，"
                     "敵將處置先加議和和懾服——"
                     "等無戰結算真的發生時，帳目已經有名可記。"
                     "在 record-is-truth 的 repo 裡，"
                     "新機制的第一步永遠是它入帳叫什麼。",
    },
    {
        "tag": "設計 5",
        "title": "章節欄位的 opt-in 第四例",
        "sections": [
            ("做法", "章節定義的 no_battle 欄位可選——"
                    "全預設就是純戰鬥章節，舊章節定義不用改。"),
            ("細節", "三路徑各有 enabled 開關："
                    "一章可只開談判不開顛覆——設計粒度到單路徑。"),
            ("啟示", "同一把尺第四次出現：新功能缺席時等於不存在——"
                    "舊內容零修改、新內容逐章投放。"),
        ],
        "code": 'struct NoBattleDef {\n    NoBattleOptionDef negotiation, deterrence,\n                      subversion;  // 各自 enabled\n    bool Offered() const { ... }\n}; // 全預設 = 純戰鬥章節',
        "narration": "第五個設計：又是 opt-in。"
                     "章節定義加可選的無戰欄位，三路徑各自獨立開關——"
                     "全預設就是純戰鬥章節，舊定義一行不用改。"
                     "一章可以只開談判不開顛覆，"
                     "投放粒度細到單一路徑。",
    },
    {
        "tag": "設計 6",
        "title": "規格到程式的對照",
        "sections": [
            ("做法", "MythLayer 落地逐項對上 spec：四階 enum、"
                    "閾值 10/30/60、怒神 25 以下乘 1.5、"
                    "DeriveFrom 四個換算率常數。"),
            ("細節", "spec 的 I/O 矩陣變成 MythLayerTest 的斷言——"
                    "連「未知區域視為 Quiet 自動建檔」都有對應測試。"),
            ("啟示", "規格寫得夠細，實作就沒有「自由發揮」的空間——"
                    "code review 變成逐項對照，不是重審設計。"),
        ],
        "code": 'static constexpr float kAnomalies = 10.0f;\nstatic constexpr float kSeep      = 30.0f;\nstatic constexpr float kManifest  = 60.0f;\nstatic constexpr float kAngryFavor = 25.0f;',
        "narration": "第六個設計：規格到程式的對照。"
                     "滲透層落地逐項對上規格：四階枚舉、"
                     "三個閾值常數、怒神加成、章節換算率。"
                     "規格寫得夠細，實作就沒有自由發揮的空間——"
                     "審查變成逐項對照，不是重新審設計。",
    },
    {
        "tag": "設計 7",
        "title": "唯讀出口是架構邊界",
        "sections": [
            ("做法", "渲染層只能讀：Level()/Levels()/Pressure() "
                    "全 const，Transitions() 回 const 引用——"
                    "沒有任何 mutator 外漏給消費端以外的路徑。"),
            ("細節", "spec Never 欄寫死的邊界在 API 層落地——"
                    "「渲染層只能讀」靠型別系統執行，不靠自律。"),
            ("啟示", "邊界要用 const 和回傳型別來砌——"
                    "看得見的唯讀比文件上的「請勿修改」有效十倍。"),
        ],
        "code": 'const std::vector<SeepageTransition>&\n    Transitions() const { return transitions; }\n// 回 const 參考——寫入路徑根本不存在',
        "narration": "第七個設計：唯讀出口是架構邊界。"
                     "渲染層拿到的全是 const——等級、快照、壓力，"
                     "連轉換記錄都只回 const 參考。"
                     "規格 Never 欄寫的邊界，靠型別系統執行不靠自律。"
                     "看得見的唯讀，比文件上的請勿修改有效十倍。",
    },
    {
        "tag": "設計 8",
        "title": "拆倉掃除：別再自己編引擎",
        "sections": [
            ("做法", "staging 遊戲側發現 GUI 庫直接編譯了引擎的 "
                    "ImageCodec.cpp/Logger.cpp——改為連結 "
                    "PotatoEngine target。"),
            ("細節", "同一份源碼編兩次 = 兩份符號、兩倍編譯時間，"
                    "還會在拆倉時產生「到底算誰的」歧義。"),
            ("啟示", "重複編譯別人的源檔是隱形 vendoring——"
                    "有 target 就連 target，源檔只屬於它的 repo。"),
        ],
        "code": '- Rendering/ImageCodec.cpp  # 直接編進 GUI 庫\n- Logging/Logger.cpp\n+ target_link_libraries(... PotatoEngine)\n# 連 target，不搬源檔',
        "narration": "第八個設計是拆倉掃出的毛病。"
                     "介面庫原本直接編譯了引擎的兩個源檔——"
                     "改成連結引擎 target。"
                     "同一份源碼編兩次是隱形的 vendoring："
                     "兩份符號、兩倍時間，拆倉時還分不清歸屬。"
                     "有 target 就連 target，源檔只屬於它的 repo。",
    },
    {
        "kind": "cover",
        "title": "第十七集清單",
        "lines": ["三路三貨幣 · 消耗型≠門檻型 · 查實紀錄是資產",
                  "帳目先有名 · 章節欄位 opt-in · spec 逐項對照",
                  "const 砌邊界 · 連 target 不搬源檔"],
        "narration": "第十七集收在這裡。無戰路徑的設計精髓："
                     "手段不同，計價單位就不同；"
                     "機制未動，帳目先有名。"
                     "而不戰而勝能被記帳，它才真的算贏過。",
    },
]

SLIDES_EP18 = [
    {
        "kind": "cover",
        "title": "中計也是一種結局",
        "lines": ["從實戰學 C++ 第十八集",
                  "NoBattleResolver · 四態判定 · 假情報的代價"],
        "narration": "第十八集看無戰路徑的判定器落地。"
                     "上一集講三種貨幣，這一集講判定引擎本身——"
                     "四態結局裡有一個最狠的：中計。"
                     "你的假情報沒驗證就敢用，敵人就在等你。",
    },
    {
        "tag": "設計 1",
        "title": "判定器無狀態",
        "sections": [
            ("做法", "NoBattleResolver 本身不存任何東西——"
                    "Gather() 從帳本和判詞檔案匯出 NoBattleContext "
                    "快照，判定只吃快照。"),
            ("細節", "static Gather 是「標準匯入法」——"
                    "但呼叫端也可以手工構造 context 餵假資料測邊界。"),
            ("啟示", "判定邏輯和資料蒐集分開——"
                    "無狀態的 resolver 可測、可重放、可換資料源。"),
        ],
        "code": 'static NoBattleContext Gather(ledger,\n    dossier, generalName);\nResolve(path, def, ctx, ledger, campaign)\n// resolver 無成員變數',
        "narration": "第一個設計：判定器無狀態。"
                     "resolver 不存任何東西——Gather 從帳本和判詞檔案"
                     "匯出一份 context 快照，判定只吃快照。"
                     "要測邊界？手工構造一份假 context 餵進去就行。"
                     "判定和蒐集分開，才能可測、可重放、可換源。",
    },
    {
        "tag": "設計 2",
        "title": "四態結局的光譜",
        "sections": [
            ("做法", "NotOffered / Success / Failed / Backfired——"
                    "未定義不寫帳、過了跳戰鬥、沒過進戰鬥、"
                    "中計也進戰鬥但帳上留不同記錄。"),
            ("細節", "Failed 和 Backfired 結果都是敵首波優勢，"
                    "但分錄來源不同——顛覆中計記 Subversion，"
                    "強行談判記 Negotiation 失敗。"),
            ("啟示", "同樣的壞結果要有不同的名字——"
                    "帳目要能區分「能力不足」和「被騙」。"),
        ],
        "code": 'NotOffered → 不寫帳（此路不存在）\nSuccess    → 跳過戰鬥直進結算\nFailed     → 進戰鬥+敵首波優勢\nBackfired  → 中計！同懲罰但帳不同',
        "narration": "第二個設計：四態光譜。"
                     "沒定義的路不寫帳；過了直接跳過戰鬥；"
                     "沒過進戰鬥、敵人拿首波優勢；"
                     "中計同樣進戰鬥——但帳上的來源不同。"
                     "同樣的壞結果要有不同的名字："
                     "能力不足和被騙，帳目要能區分。",
    },
    {
        "tag": "設計 3",
        "title": "假情報的非對稱",
        "sections": [
            ("做法", "verifiedIntel 只數「已查實且非偽造」的判詞；"
                    "plantedUnrefuted 只看「植入了但沒驗證」的判詞。"),
            ("細節", "驗證過的假情報已被駁斥揭曉——不再是陷阱。"
                    "只有「懶得驗證的假情報」才會變成 Backfired。"),
            ("啟示", "查證工作流在這裡有了真實代價——"
                    "省下的驗證步驟，就是留給敵人的陷阱門。"),
        ],
        "code": 'verifiedIntel    = verified && !planted\nplantedUnrefuted = planted  && !verified\n// 驗證過的假情報 = 已駁斥\n// 沒驗證的假情報 = 中計',
        "narration": "第三個設計最狠：假情報的非對稱。"
                     "情報分兩種——驗證過的 planted 已經駁斥揭曉，"
                     "不算陷阱；真正會咬人的是「植入了但你懶得驗證」。"
                     "查證工作流在這裡有了真實代價："
                     "省下的驗證步驟，就是留給敵人的陷阱門。",
    },
    {
        "tag": "設計 4",
        "title": "灰顯不隱藏",
        "sections": [
            ("做法", "CheckOptions 對每條已定義路徑產生報表——"
                    "門檻、現值、原因全列出，未達標的 available=false "
                    "但不消失。"),
            ("細節", "reason 欄位是人讀說明——UI 拿去做灰顯 "
                    "tooltip，玩家看得到「差多少」而不是「不能按」。"),
            ("啟示", "鎖住的選項要展示門檻——"
                    "可見的目標是進度感，隱藏的鎖只是牆。"),
        ],
        "code": 'struct NoBattleOptionInfo {\n    bool available; int current;\n    int threshold;  string reason;\n}; // 灰顯可見，不隱藏',
        "narration": "第四個設計：灰顯不隱藏。"
                     "報表把門檻和現值都攤開——沒達標的路灰顯但不消失，"
                     "reason 欄位直接給 UI 做提示。"
                     "玩家看得到差多少，而不是只看到牆。"
                     "可見的目標是進度感。",
    },
    {
        "tag": "設計 5",
        "title": "強行嘗試有明碼代價",
        "sections": [
            ("做法", "available=false 的路也能硬選——Resolve 判 "
                    "Failed，代價是敵首波優勢，不是「按不下去」。"),
            ("細節", "把「不可能」做成「昂貴」——玩家可以賭，"
                    "但帳上會記他賭輸了。"),
            ("啟示", "硬鎖定擋住的是好奇心；明碼代價放行的是選擇——"
                    "失敗本身成為敘事材料。"),
        ],
        "code": 'if (current < threshold) {\n    verdict = Failed;\n    enemyAdvantage = true;  // 賭輸的代價\n}',
        "narration": "第五個設計：強行嘗試有明碼代價。"
                     "沒達標的路不是按不下去，是可以硬選——"
                     "判定失敗、敵人拿首波優勢，帳上記一筆。"
                     "硬鎖定擋住的是好奇心，明碼代價放行的是選擇——"
                     "失敗本身成了敘事材料。",
    },
    {
        "tag": "設計 6",
        "title": "跨層交棒用旗標不用依賴",
        "sections": [
            ("做法", "resolver 在 Campaign 層，不碰 BattleController——"
                    "失敗結果只設 enemyAdvantage 旗標，"
                    "交給戰鬥層兌現首波優勢。"),
            ("細節", "「敵首波優勢怎麼實現」是戰鬥層的事——"
                    "resolver 只保證旗標被帶到，語義兌現歸對方。"),
            ("啟示", "跨層結果用資料交棒——旗標越界、"
                    "依賴不越界，方向永遠單向。"),
        ],
        "code": 'NoBattleResult r = resolver.Resolve(...);\nif (r.enemyAdvantage)\n    battle.GrantFirstStrike(); // 戰鬥層兌現\n// resolver 不認識 BattleController',
        "narration": "第六個設計：跨層交棒用旗標。"
                     "判定器住戰役層但不碰戰鬥控制器——"
                     "失敗只設一個布林旗標，"
                     "首波優勢怎麼兌現是戰鬥層的事。"
                     "資料越界、依賴不越界，方向永遠單向。",
    },
    {
        "tag": "設計 7",
        "title": "判定也要入帳",
        "sections": [
            ("做法", "Success/Failed/Backfired 都寫分錄——"
                    "provenance 帶 source=路徑來源、tick=章節序、"
                    "eventId=<章節>:<路徑>。"),
            ("細節", "連 NotOffered 都有定義行為：不寫任何東西——"
                    "「沒發生過」本身就是答案，不留假記錄。"),
            ("啟示", "record-is-truth 的極致是連「判定」都入帳——"
                    "戰後翻帳能重建當初每個選擇的合法性和代價。"),
        ],
        "code": 'Post(ledger, entry,\n  prov{source=Deterrence,\n       tick=chapter,\n       eventId="ch3:deterrence"});\n// 判定出處可查可引',
        "narration": "第七個設計：判定也要入帳。"
                     "三種真結局都寫分錄，來源欄位就是路徑，"
                     "事件編號是章節冒號路徑——判定出處可查可引。"
                     "而沒定義的路不寫任何東西："
                     "沒發生過本身就是答案，不留假記錄。",
    },
    {
        "tag": "設計 8",
        "title": "敵將處置是結局的一部分",
        "sections": [
            ("做法", "Success 除了分錄還記敵將處置——"
                    "談判成功記 Negotiated、嚇阻成功記 Intimidated。"),
            ("細節", "處置寫進 CampaignLedger 的不是「贏了」而是"
                    "「怎麼贏的」——結局評語引用的是處置方式。"),
            ("啟示", "勝利記錄要帶手段——「斷橋軍師」的帳本裡，"
                    "議和和授首是不同品格的贏。"),
        ],
        "code": 'case Negotiation: disposition = Negotiated;\ncase Deterrence:  disposition = Intimidated;\n// 記的不是贏了，是怎麼贏的',
        "narration": "第八個設計：敵將處置是結局的一部分。"
                     "成功不只寫分錄，還記敵將怎麼退場——"
                     "議和記議和、懾服記懾服。"
                     "戰役帳記的不是贏了，是怎麼贏的——"
                     "結局評語引用的是處置方式，"
                     "議和和授首是不同品格的贏。",
    },
    {
        "kind": "cover",
        "title": "第十八集清單",
        "lines": ["無狀態快照判定 · 中計是獨立結局 · 未驗證才是陷阱",
                  "灰顯攤門檻 · 失敗明碼代價 · 旗標跨層",
                  "判定入帳 · 處置記手段"],
        "narration": "第十八集收在這裡。判定器的品格："
                     "無狀態、全留帳、壞結局各有名字。"
                     "而最深刻的設計是——你的懶惰會被寫進帳本，"
                     "中計這個詞從此有了資料結構。",
    },
]

SLIDES_EP19 = [
    {
        "kind": "cover",
        "title": "綠燈之後",
        "lines": ["從實戰學 C++ 第十九集",
                  "審查的報酬：再入懸空 · NaN 磚檔 · 預設不對稱"],
        "narration": "第十九集講測試全綠之後發生的事。"
                     "MythLayer 實作完成、矩陣全過——"
                     "然後審查又抓出五個真 bug。"
                     "這一集是「能跑」和「對」之間的那段距離。",
    },
    {
        "tag": "設計 1",
        "title": "回呼再入會懸空引用",
        "sections": [
            ("做法", "Ratchet 原本邊升階邊發事件——回呼若在 "
                    "Region& 存活途中再入層（Feed/BindSpirit），"
                    "unordered_map rehash，引用懸空。"),
            ("細節", "修法是 collect-then-dispatch：Ratchet 只把事件"
                    "推進 pending 向量，全部變異完成後 Dispatch 統一派出。"),
            ("啟示", "持有容器內部引用時呼叫外部程式碼是經典陷阱——"
                    "事件延後到沒有活引用的時機再發。"),
        ],
        "code": 'Ratchet(region, r, pending); // 只收集\nDispatch(pending);          // 變異完才派\n// 回呼再入層：此時無活 Region&',
        "narration": "第一個抓到的最陰險：回呼再入懸空。"
                     "棘輪原本邊升階邊發事件——如果回呼在持有"
                     "區域引用的途中再入層，unordered_map 一 rehash，"
                     "引用就懸空。修法是收集後派送："
                     "變異完、沒有活引用了，才派出事件。",
    },
    {
        "tag": "設計 2",
        "title": "NaN 會把存檔永久磚掉",
        "sections": [
            ("做法", "FromJson 對壓力值先查 isfinite 再夾零——"
                    "非有限值一律歸零。"),
            ("細節", "不放行的話：%g 會把 nan/inf 寫進存檔，"
                    "parser 讀不回來——不是這筆壞掉，"
                    "是整個戰役存檔再也載不進去。"),
            ("啟示", "序列化鏈的每個值都要能來回——"
                    "寫得出去但讀不回來的值，是存檔格式的地雷。"),
        ],
        "code": 'const float p = e["p"].AsFloat(0);\nr.pressure = isfinite(p)\n    ? max(0.f, p) : 0.f;\n// 不然 %g 寫出 nan → 永久磚檔',
        "narration": "第二個抓到的是存檔地雷。"
                     "壓力載入先查是否有限再夾零——"
                     "不擋的話，nan 或 inf 會被格式化寫進存檔，"
                     "而解析器讀不回來——整個戰役永久磚掉。"
                     "寫得出去但讀不回來的值，就是格式的地雷。",
    },
    {
        "tag": "設計 3",
        "title": "讀寫預設值要對稱",
        "sections": [
            ("做法", "Favor() 讀未登錄靈回中立值 50，但 "
                    "AdjustFavor 用 favor[spirit] 從 0 起算——"
                    "正向調整反而造出怒神。"),
            ("細節", "修法：try_emplace(spirit, kNeutralFavor)——"
                    "寫入端和讀取端用同一個預設起算。"),
            ("啟示", "「沒見過的值是什麼」只能有一個答案——"
                    "讀端和寫端各自定預設，就是對稱性 bug 的溫床。"),
        ],
        "code": '// 修前：favor[s] 預設 0，讀端預設 50\n//        AdjustFavor(+10) → 10 = 怒神！\n// 修後：try_emplace(s, kNeutralFavor)\n//        50+10=60，讀寫對稱',
        "narration": "第三個抓到的是對稱性 bug。"
                     "讀取端說沒見過的神明是中立的五十，"
                     "寫入端卻從零起算——正向調整反而造出怒神。"
                     "修法用 try_emplace 以中立值起算："
                     "沒見過的值只能有一個答案。",
    },
    {
        "tag": "設計 4",
        "title": "缺段容忍 vs 壞段拒絕",
        "sections": [
            ("做法", "欄位缺席容忍（舊存檔降級），"
                    "但存在而型別錯誤要拒絕——regions 不是陣列就 "
                    "return false。"),
            ("細節", "兩種「不對」要分開：沒有=可以理解，"
                    "有但壞=檔案受損。混為一談會把壞檔當舊檔吞掉。"),
            ("啟示", "向後相容的精確語義：缺是版本問題、"
                    "壞是完整性問題——前者降級、後者拒絕。"),
        ],
        "code": 'if (!rj.IsNull() && !rj.IsArray())\n    return false; // 在而型別錯=壞檔\n// IsNull（缺席）= 舊檔降級容忍',
        "narration": "第四個抓到的是相容語義的精度。"
                     "欄位缺席要容忍——那是舊版存檔；"
                     "存在但型別錯要拒絕——那是壞檔。"
                     "缺是版本問題、壞是完整性問題，"
                     "混為一談會把損毀檔案當舊檔吞掉。",
    },
    {
        "tag": "設計 5",
        "title": "降級也要清殘態",
        "sections": [
            ("做法", "FromJson(null) 原本直接回 true——"
                    "重用同一物件時舊的 regions/favor/transitions "
                    "全部殘留。改成降級時清空。"),
            ("細節", "「載入空層」和「不載入」語義不同——"
                    "回 true 的意思是新狀態有效，不是維持現狀。"),
            ("啟示", "FromJson 的契約是「讓物件變成檔案說的樣子」——"
                    "檔案說沒有，物件就不該有。"),
        ],
        "code": 'if (j.IsNull()) {\n    regions.clear(); favor.clear();\n    transitions.clear(); chapter = 1;\n    return true; // 空層，不是「不動」\n}',
        "narration": "第五個抓到的是降級的殘態。"
                     "空段載入原本直接回 true——重用物件時，"
                     "舊的區域記錄全部殘留。"
                     "「載入空層」和「不載入」是不同語義："
                     "回 true 意味著新狀態有效，"
                     "檔案說沒有，物件就不該有。",
    },
    {
        "tag": "設計 6",
        "title": "swap 慣例會弄丟 callback",
        "sections": [
            ("做法", "CampaignState 載入用「建新物件再 swap」——"
                    "但 MythLayer 上註冊的事件回呼會隨舊物件蒸發。"
                    "補：newMyth 接手舊回呼。"),
            ("細節", "swap 慣例保證壞檔不污染現狀，"
                    "但「外部註冊的東西」不在 JSON 裡——"
                    "要手動搬。"),
            ("啟示", "copy-and-swap 的盲點：只搬序列化狀態，"
                    "不搬運行時註冊物——回呼、觀察者、快取都要清點。"),
        ],
        "code": 'MythLayer newMyth;\nnewMyth.FromJson(j);\nnewMyth.SetEventCallback(\n    myth.EventCallback()); // 搬走註冊物\ncamp = newCamp;',
        "narration": "第六個抓到的是 swap 慣例的盲點。"
                     "載入先建新物件再交換——保證壞檔不污染現狀，"
                     "但註冊在舊物件上的事件回呼不在 JSON 裡，"
                     "會隨舊物件蒸發。修法是明確接手。"
                     "copy-and-swap 只搬序列化狀態，"
                     "運行時註冊物要自己清點。",
    },
    {
        "tag": "設計 7",
        "title": "怒神加成要認事件類型",
        "sections": [
            ("做法", "怒神加成原本看 delta>0——改成認事件型別："
                    "只有 Atrocity/VillageBurned 才吃加成。"),
            ("細節", "正值不等於暴行——其他正壓力事件不該被神明"
                    "放大。語義判斷要用分類，不用數值符號。"),
            ("啟示", "「條件該看什麼」要回看需求——"
                    "怒神恨的是暴行不是數字。"),
        ],
        "code": 'const bool atrocity =\n    ev == Atrocity ||\n    ev == VillageBurned;\nif (atrocity && Favor(s) < kAngryFavor)\n    delta *= kAngerBoost;',
        "narration": "第七個抓到的是條件的語義。"
                     "怒神加成原本只看壓力是正的——"
                     "但正值不等於暴行。改成認事件型別，"
                     "只有屠殺和焚村才吃神明加成。"
                     "條件該看什麼要回看需求："
                     "怒神恨的是暴行，不是數字。",
    },
    {
        "tag": "設計 8",
        "title": "imported target 的可見域",
        "sections": [
            ("做法", "staging 引擎側把 glfw imported target 提升 "
                    "GLOBAL——讓 add_subdirectory 消費端也連得到。"),
            ("細節", "imported target 預設只在定義它的目錄樹可見——"
                    "遊戲 repo 當子目錄消費時，不提升就找不到 glfw。"),
            ("啟示", "拆倉的驗收要包含「被消費時的樣子」——"
                    "引擎自己建得過不代表被 add_subdirectory 也過。"),
        ],
        "code": 'add_library(glfw STATIC IMPORTED GLOBAL)\n// 或事後提升：\nset_target_properties(glfw PROPERTIES\n    IMPORTED_GLOBAL TRUE)',
        "narration": "第八個是拆倉演習抓到的："
                     "imported target 預設只在定義它的目錄可見，"
                     "遊戲 repo 當子目錄消費時連不到 glfw。"
                     "解法是把 target 提升 GLOBAL。"
                     "驗收要包含被消費時的樣子——"
                     "自己建得過不代表被引用也過。",
    },
    {
        "kind": "cover",
        "title": "第十九集清單",
        "lines": ["回呼延後派 · NaN 歸零防磚檔 · 讀寫預設對稱",
                  "缺容忍壞拒絕 · 降級清殘態 · swap 要搬註冊物",
                  "條件認型別 · imported 要 GLOBAL"],
        "narration": "第十九集收在這裡。五個 bug 的共同點："
                     "全都過得了開心路徑測試——"
                     "審查的報酬是抓那些「測試沒想過會發生」的事。"
                     "綠燈是起點，不是終點。",
    },
]

SLIDES_EP20 = [
    {
        "kind": "cover",
        "title": "誠實的近似",
        "lines": ["從實戰學 C++ 第二十集",
                  "Golomb 尺 · 間歇陣風 · 近似要標明犧牲了什麼"],
        "narration": "第二十集講第三批未解問題落地——"
                     "這次的重點不是數學，是誠實。"
                     "兩個新 header 都在類註解裡明寫"
                     "「我犧牲了什麼」：貪心尺不保最優、"
                     "陣風場不保無散度。近似工程的第一課。",
    },
    {
        "tag": "設計 1",
        "title": "一把每對距離都不同的尺",
        "sections": [
            ("做法", "Golomb 尺：n 個刻度、所有 pairwise 距離互異——"
                    "抽樣抽頭間距不重複，就沒有共振條紋。"),
            ("細節", "最優尺只在 n≤27 被證明；引擎內建已驗證表 "
                    "n≤12（OEIS A079426），更大走貪心構造。"),
            ("啟示", "「未解」不妨礙取用——"
                    "已證明的部分做成表，未證明的部分降級成近似。"),
        ],
        "code": '// 最優表 n<=12：\n// n=7 → {0,1,4,10,18,23,25}\n// 任意兩刻度差全異 → 無共振紋',
        "narration": "第一個設計：一把每對距離都不同的尺。"
                     "Golomb 尺的 n 個刻度，任兩個間距互不重複——"
                     "抽樣間距不重複，就不會有共振條紋。"
                     "最優尺只在二十七以下被證明，"
                     "引擎內建驗證表到十二，更大走貪心。"
                     "未解不妨礙取用：已證明的做成表，"
                     "未證明的降級成近似。",
    },
    {
        "tag": "設計 2",
        "title": "保性質，不保最優",
        "sections": [
            ("做法", "貪心構造逐步追加最小合法刻度——全異距性質"
                    "保留，但尺長不再最優（n=13 貪心約 114、"
                    "最優 106）。"),
            ("細節", "類註解直接寫明這個差距——"
                    "在乎緊緻性的呼叫方請自查 Marks(n).back()。"),
            ("啟示", "降級路徑要宣告代價——「性質還在、"
                    "最優沒了」寫在文件裡，呼叫方才選得起。"),
        ],
        "code": '// 貪心：新刻度 c 合法 ⟺\n// {c - mᵢ} 全異且不入既有差集\n// 註：n=13 貪心≈114 > 最優 106\n// ——寫在類註解裡，不藏',
        "narration": "第二個設計：保性質不保最優。"
                     "貪心逐步加最小合法刻度，全異距還在，"
                     "但尺長不是最優——十三刻度貪心一百一十四，"
                     "最優一百零六。這個差距直接寫在類註解裡。"
                     "降級路徑要宣告代價，呼叫方才選得起。",
    },
    {
        "tag": "設計 3",
        "title": "編隊位：黃金角加 Golomb 半徑",
        "sections": [
            ("做法", "FormationOffsets：第 i 槽放黃金角 i×137.5°、"
                    "半徑取 Golomb 刻度的螺旋位——相鄰槽間距全異。"),
            ("細節", "兩個去對稱手段疊用：角度靠黃金角無週期、"
                    "半徑靠全異距無同心環——編隊不再像尺規作圖。"),
            ("啟示", "「看起來不人工」常常是兩個去相關手段的疊加——"
                    "各管一個軸，效果乘法。"),
        ],
        "code": 'r = marks[i] * spacing;    // Golomb 半徑\na = i * 137.5°;            // 黃金角\npos[i] = (r·cos a, r·sin a)\n// 角無週期 × 距無重複',
        "narration": "第三個設計：編隊槽位。"
                     "第 i 槽放黃金角乘 i 的方向、"
                     "Golomb 刻度的半徑——角度去週期、半徑去對稱，"
                     "兩個手段各管一個軸，疊起來編隊就不再像"
                     "尺規作圖。看起來不人工，"
                     "常常是去相關手段的乘法。",
    },
    {
        "tag": "設計 4",
        "title": "間歇性是乘出來的",
        "sections": [
            ("做法", "GustField：基底 CurlNoise 流場上乘一個"
                    "對數正態強度場 g——exp(σχ̂−σ²/2)，"
                    "集合平均仍是一。"),
            ("細節", "多數時空點 g<1 微風，少數點 g≫1 爆發——"
                    "平滑場畫不出「一陣一陣」，重尾調製才會。"),
            ("啟示", "疊加給平滑、乘性給突發——"
                    "要「偶爾很強」的訊號，找重尾分佈不要找正態。"),
        ],
        "code": 'g = exp(σ·χ̂ − σ²/2)   // E[g]≈1\nv_out = g × curlNoise(p,t)\n// 微風常態 × 偶發爆發',
        "narration": "第四個設計：間歇性是乘出來的。"
                     "湍流的間歇性——能量集中在稀疏突發區——"
                     "用對數正態強度場乘上無散度基底流："
                     "多數點微風、少數點爆發。"
                     "疊加給平滑、乘性給突發——"
                     "要偶爾很強的訊號，找重尾不要找正態。",
    },
    {
        "tag": "設計 5",
        "title": "誠實註解一：我破了不變量",
        "sections": [
            ("做法", "GustField 類註解明寫：g·v 不再嚴格無散度——"
                    "∇·(gv)=∇g·v≠0，要嚴格不變量請用基底場。"),
            ("細節", "犧牲是刻意換的：精確不變量換突發外觀——"
                    "但不是默默換，是寫在文件裡讓你知情。"),
            ("啟示", "近似的第一原則是標明破了什麼——"
                    "消費者才能判斷這個犧牲在自己的場景可不可接受。"),
        ],
        "code": '// 誠實性：g·v 不再嚴格無散度\n// ∇·(gv) = ∇g·v ≠ 0\n// 要嚴格 → 用 TurbulenceField 本體',
        "narration": "第五個設計是誠實註解第一條。"
                     "類註解明寫：乘上強度場之後不再嚴格無散度，"
                     "要嚴格不變量請回頭用基底場。"
                     "犧牲是刻意換的——用不變量換外觀——"
                     "但不是默默換，是讓你知情。"
                     "近似的第一原則：標明破了什麼。",
    },
    {
        "tag": "設計 6",
        "title": "誠實註解二：降級階梯標級別",
        "sections": [
            ("做法", "兩個 header 的降級路徑都標了級別——"
                    "貪心尺「性質在、最優沒」；陣風場「外觀在、"
                    "不變量沒」。"),
            ("細節", "還附逃生門：在乎緊緻性查 back()、"
                    "在乎無散度用 TurbulenceField——"
                    "降級不只認錯，還指路。"),
            ("啟示", "好的近似 API 像藥品仿單——"
                    "效用、副作用、禁忌症全寫，自己決定用不用。"),
        ],
        "code": '// 降級宣告 + 逃生門：\n// 「非最優，在乎請查 .back()」\n// 「非無散度，在乎請用 TurbulenceField」',
        "narration": "第六個設計：降級要標級別還要指路。"
                     "兩個類都寫清楚犧牲什麼，還附逃生門——"
                     "在乎尺長查末端值、在乎散度用基底場。"
                     "好的近似像藥品仿單：效用、副作用、禁忌症全寫，"
                     "用不用你自己決定。",
    },
    {
        "tag": "設計 7",
        "title": "審查過程本身入帳",
        "sections": [
            ("做法", "D-1 審查的完整 diff 存成 "
                    "implementation-artifacts/d1-review.diff——"
                    "「怎麼被審的」變成可引用的檔案。"),
            ("細節", "上一集的五個修法就在這份 diff 裡——"
                    "審查不只是通過不通過，是留下證據的文件。"),
            ("啟示", "record-is-truth 連開發過程都適用——"
                    "審查痕跡入檔，之後的人才能看到為什麼改。"),
        ],
        "code": '_bmad-output/implementation-artifacts/\n    d1-review.diff   # 審查→修法全記錄\n# 過程也是產物',
        "narration": "第七個設計：審查過程本身入帳。"
                     "D-1 審查的完整差異存成檔案放進產物目錄——"
                     "上一集那五個修法都在裡面。"
                     "審查不只是通過不通過，是留下證據："
                     "之後的人能看到為什麼改。",
    },
    {
        "tag": "設計 8",
        "title": "延期的東西也要有名字",
        "sections": [
            ("做法", "deferred-work.md 追蹤被延後的工作——"
                    "每項有 id、原因、喚醒條件，不是散落各處的 TODO。"),
            ("細節", "和 UNSOLVED 備選欄同一個邏輯：延後不是忘掉，"
                    "是「附條件的等」——條件寫下來才知道何時回頭。"),
            ("啟示", "專案裡最危險的不是延後的工作，"
                    "是沒人記得為什麼延後的工作。"),
        ],
        "code": '# deferred-work.md\n# 每項：id + 擱置原因 + 喚醒條件\n# 延後 ≠ 忘掉；沒寫原因的才算丟',
        "narration": "第八個設計：延期的東西也要有名字。"
                     "deferred-work 檔案追蹤每項延後工作——"
                     "有編號、有原因、有喚醒條件。"
                     "和備選欄同一個邏輯：延後不是忘掉，"
                     "是附條件的等。最危險的不是延後的工作，"
                     "是沒人記得為什麼延後的工作。",
    },
    {
        "kind": "cover",
        "title": "第二十集清單",
        "lines": ["全異距尺 · 保性質不保最優 · 黃金角乘 Golomb 半徑",
                  "乘性給突發 · 破不變量要寫明 · 降級附逃生門",
                  "審查入檔 · 延期有喚醒條件"],
        "narration": "第二十集收在這裡。近似的工程倫理："
                     "犧牲什麼寫下來、降級路徑標級別、"
                     "逃生門留給在乎的人。"
                     "不完美的工具誠實標註，"
                     "勝過假裝完美的工具。",
    },
]

SLIDES_EP21 = [
    {
        "kind": "cover",
        "title": "給審查者一張地圖",
        "lines": ["從實戰學 C++ 第二十一集",
                  "Suggested Review Order · 可選掛鉤 · 陣風接上雲霧"],
        "narration": "第二十一集看兩件收尾的事。"
                     "D-1 spec 標 done 之後補了一段"
                     "給審查者的建議閱讀順序；"
                     "而第三批的陣風場當天就接上了霧標記的漂移。"
                     "收尾動作做對，工作才算真的完。",
    },
    {
        "tag": "設計 1",
        "title": "spec 長出閱讀路徑",
        "sections": [
            ("做法", "done 的 spec 尾端補 Suggested Review Order——"
                    "按子系統分群，每條帶 file:line 連結。"),
            ("細節", "群組照關注點切：狀態機核心→神明態度→"
                    "跨章重推導→持久化→外圍——"
                    "不是照檔案切、是照腦袋裡的問題切。"),
            ("啟示", "審查者的第一個成本是「從哪裡開始讀」——"
                    "給路徑，審查就從漫遊變成按圖施工。"),
        ],
        "code": '## Suggested Review Order\n- 狀態機核心  MythLayer.cpp:37\n- 神明態度    MythLayer.cpp:97\n- 持久化      MythLayer.cpp:191\n- 外圍        MythLayerTest.cpp:34',
        "narration": "第一個設計：spec 長出閱讀路徑。"
                     "完成的規格尾端補上建議審查順序——"
                     "按子系統分群、每條帶行號連結。"
                     "分群照關注點切不照檔案切："
                     "狀態機核心、神明態度、跨章、持久化、外圍。"
                     "審查的第一個成本是從哪裡開始讀——"
                     "給路徑，漫遊就變成按圖施工。",
    },
    {
        "tag": "設計 2",
        "title": "閱讀順序就是設計順序",
        "sections": [
            ("做法", "第一條指向 Seepage enum 和閾值常數——"
                    "先讀語義再看機制，不是先讀演算法。"),
            ("細節", "enum 的四個名字（Quiet→Manifest）就是"
                    "整個系統的詞彙表——詞彙定了，"
                    "後面每一行都有上下文。"),
            ("啟示", "導讀的第一站永遠是型別和常數——"
                    "那是系統的字典，其餘都是造句。"),
        ],
        "code": '// 先讀：\nenum class Seepage { Quiet, Anomalies,\n                     Seep, Manifest };\n// 四個名字 = 全系統的詞彙表',
        "narration": "第二個設計：閱讀順序就是設計順序。"
                     "導讀第一條指向枚舉和閾值常數——"
                     "先讀語義再看機制。"
                     "四個階名就是整個系統的詞彙表，"
                     "詞彙定了，後面每一行才有上下文。"
                     "導讀的第一站永遠是型別——那是字典。",
    },
    {
        "tag": "設計 3",
        "title": "研究落地的同一批接線",
        "sections": [
            ("做法", "GustField 進 repo 的同輪，"
                    "BattleSceneSync 多了 SetFogDriftGust——"
                    "霧標記的漂移乘上間歇強度。"),
            ("細節", "機率雲標記本來乘 TurbulenceField 平滑漂移，"
                    "現在再乘 g——E[g]=1 所以平均漂移量不變，"
                    "變的是忽強忽弱的外觀。"),
            ("啟示", "研究的投資回報看接線速度——"
                    "第三批落地當天就有消費者，貨架沒有灰塵。"),
        ],
        "code": 'dv = drift->Sample(pos, t);\ng  = gust ? gust->Intensity(pos, t) : 1;\npos += dv * strength * g;\n// E[g]=1：均值不變，外觀變',
        "narration": "第三個設計：研究落地同一批就接線。"
                     "陣風場進 repo 的同輪，場景同步多了"
                     "霧漂移陣風掛鉤——標記位置再乘間歇強度。"
                     "均值仍是一，所以平均漂移量不變，"
                     "變的是忽強忽弱的外觀。"
                     "研究的投資回報，看接線速度。",
    },
    {
        "tag": "設計 4",
        "title": "可選掛鉤的指標形式",
        "sections": [
            ("做法", "fogDrift、fogGust、selected 都是 "
                    "const T* 成員——nullptr 預設值就是「不啟用」。"),
            ("細節", "生命週期歸呼叫端——sync 只讀不持有；"
                    "同一類三個可選掛鉤，全是同一把尺。"),
            ("啟示", "渲染層的可選增強用裸指標當開關——"
                    "非所有權、可空、零成本——三個語意一次滿足。"),
        ],
        "code": 'const TurbulenceField* fogDrift = nullptr;\nconst GustField*       fogGust  = nullptr;\nconst Squad*           selected = nullptr;\n// nullptr = 關，生命週期歸呼叫端',
        "narration": "第四個設計：可選掛鉤的指標形式。"
                     "漂移場、陣風場、選取的小隊——"
                     "三個都是 const 指標，nullptr 就是關。"
                     "生命週期歸呼叫端，sync 只讀不持有。"
                     "渲染層的可選增強用裸指標當開關："
                     "非所有權、可空、零成本，三個語意一次滿足。",
    },
    {
        "tag": "設計 5",
        "title": "外觀掛鉤不碰邏輯",
        "sections": [
            ("做法", "Gust 調製只影響標記的視覺位置——"
                    "機率雲的疊加值、觀測結果、判定邏輯一概不碰。"),
            ("細節", "P-4 是表現層消費者：行為近似的外觀交給眼睛，"
                    "帳目和判定留在邏輯層——視覺抖動不進帳本。"),
            ("啟示", "「給人看的」和「給規則的」資料路徑要分開——"
                    "外觀近似再怎麼失真，都不會污染判定。"),
        ],
        "code": '// 標記位置 = 外觀（乘 gust）\n// cloud[i].second = 機率值（不乘）\n// 眼睛看到飄忽，帳本記的仍是機率',
        "narration": "第五個設計：外觀掛鉤不碰邏輯。"
                     "陣風只乘在標記的視覺位置上——"
                     "機率值、觀測、判定一概不碰。"
                     "眼睛看到飄忽的雲，帳本記的仍是乾淨的機率。"
                     "給人看的和給規則的路徑要分開——"
                     "外觀再失真，也污染不了判定。",
    },
    {
        "tag": "設計 6",
        "title": "複合場把基底當參數",
        "sections": [
            ("做法", "GustField::Sample(p, t, flow) 把基底流場當參數——"
                    "調製器不內建流場，誰的流都能乘。"),
            ("細節", "建構參數只管調製器自己：octaves、sigma、"
                    "seed——基底流在取樣時才注入。"),
            ("啟示", "效果器設計成「輸入即依賴」——"
                    "不綁特定場，換個基底就換個風格。"),
        ],
        "code": 'Vector3 Sample(p, t, const TurbulenceField& flow)\n// 調製器不持有流場——\n// 取樣當下才注入基底',
        "narration": "第六個設計：複合場把基底當參數。"
                     "陣風的取樣把基底流場當參數傳入——"
                     "調製器不持有流場，誰的流都能乘。"
                     "建構參數只管自己，基底在取樣當下注入。"
                     "效果器設計成輸入即依賴："
                     "不綁特定場，換個基底就換個風格。",
    },
    {
        "tag": "設計 7",
        "title": "E[g]=1 是可審計的承諾",
        "sections": [
            ("做法", "間歇強度的設計目標寫死：集合平均為一——"
                    "exp 裡的 −σ²/2 項就是為了拉回均值。"),
            ("細節", "不校正的 lognormal 會讓平均漂移越乘越大——"
                    "那個 −σ²/2 是歸一化項，不是裝飾。"),
            ("啟示", "調製器要宣告「不改變什麼」——"
                    "E[g]=1 是可被測試斷言的不變量，"
                    "也是審查者第一個要驗的數。"),
        ],
        "code": 'g = exp(σ·χ̂ − σ²/2)\n// −σ²/2 = 歸一化：讓 E[g]=1\n// 沒這項，風越乘越強',
        "narration": "第七個設計：均值為一是可審計的承諾。"
                     "指數裡那個負二分之 sigma 平方是歸一化項——"
                     "沒有它，對數正態會讓平均漂移越乘越大。"
                     "調製器要宣告不改變什麼："
                     "E[g]=1 是可斷言的不變量，"
                     "也是審查者第一個該驗的數。",
    },
    {
        "tag": "設計 8",
        "title": "收尾清單裡的隱藏項",
        "sections": [
            ("做法", "D-1 的完成定義長出三層：程式落地 → "
                    "審查修法 → spec 補閱讀路徑——"
                    "done 是迭代出來的狀態。"),
            ("細節", "回頭看狀態機：draft → in-progress → "
                    "in-review → done——每個轉換都有產物："
                    "spec、程式、review.diff、導讀。"),
            ("啟示", "功能的完成度看留下的文件厚度——"
                    "四個狀態四份產物，少一份都不算完。"),
        ],
        "code": 'draft → in-progress → in-review → done\nspec  → 程式+測試  → review.diff → 導讀\n# 每個轉換留一份產物',
        "narration": "第八個設計收束：完成是迭代的狀態。"
                     "D-1 走過四個階段，每個轉換留一份產物——"
                     "規格、程式和測試、審查差異檔、閱讀路徑。"
                     "功能的完成度看留下的文件厚度："
                     "四份產物少一份，都不算真的完。",
    },
    {
        "kind": "cover",
        "title": "第二十一集清單",
        "lines": ["導讀按問題分群 · 第一站是型別 · 落地即接線",
                  "const 指標當開關 · 外觀不碰判定 · 基底當參數",
                  "E[g]=1 可斷言 · done 有四份產物"],
        "narration": "第二十一集收在這裡。收尾的品質："
                     "給審查者地圖、給研究找消費者、"
                     "給調製器一個可審計的承諾。"
                     "功能做完不是句點，是可以被審查的起點。",
    },
]

SLIDES_EP22 = [
    {
        "kind": "cover",
        "title": "給隨機寫斷言",
        "lines": ["從實戰學 C++ 第二十二集",
                  "測分佈不測數值 · 均值帶 · 重尾形狀 · 退化即規格"],
        "narration": "第二十二集講一個沒講過的測試問題："
                     "隨機場怎麼斷言？"
                     "陣風場和 Golomb 尺的測試檔給出完整答案——"
                     "不斷言任何一個數，斷言分佈的形狀。"
                     "統計斷言是這一集的主題。",
    },
    {
        "tag": "設計 1",
        "title": "分佈斷言三件套",
        "sections": [
            ("做法", "四千個取樣點只斷三件事：均值在帶內、"
                    "存在遠大於均值的突發、多數點低於均值。"),
            ("細節", "不斷言任何單點的值——隨機場的「對」"
                    "是分佈形狀的對，不是逐點的對。"),
            ("啟示", "隨機系統的規格要寫成統計量——"
                    "均值、極值比、分位佔比，都是可以斷言的形狀。"),
        ],
        "code": 'CHECK(|mean − 1.0| < 0.12)   // 均值帶\nCHECK(max > 2·mean)          // 突發存在\nCHECK(frac(g<1) > 0.5)       // 重尾形狀',
        "narration": "第一個設計：分佈斷言三件套。"
                     "四千個取樣點只斷三件事——均值在帶內、"
                     "有遠大於均值的突發、多數點低於均值。"
                     "不斷言任何單點：隨機場的對是形狀的對，"
                     "不是逐點的對。",
    },
    {
        "tag": "設計 2",
        "title": "把承諾變成斷言",
        "sections": [
            ("做法", "上一集的 E[g]=1 承諾，這集變成 "
                    "|mean−1|<0.12 的實測斷言。"),
            ("細節", "容差不是隨便給的——四千樣本的 "
                    "lognormal 均值波動範圍內；"
                    "太緊會 flaky、太鬆沒意義。"),
            ("啟示", "文件裡的不變量必須有一個對應的 CHECK——"
                    "寫得出斷言的承諾才算承諾，"
                    "寫不出的只是願望。"),
        ],
        "code": '// 類註解：E[g] ≈ 1\n// 測試：\nCHECK(fabs(mean − 1.0) < 0.12)\n// 承諾 → 可斷言 → 已斷言',
        "narration": "第二個設計：承諾變斷言。"
                     "上一集說的均值為一，這集變成正負零點一二的"
                     "實測斷言。容差不是隨便給的——"
                     "太緊會 flaky、太鬆沒意義。"
                     "文件裡的不變量必須有一個對應的檢查——"
                     "寫得出斷言的承諾才算承諾。",
    },
    {
        "tag": "設計 3",
        "title": "間歇性要兩個斷言才成立",
        "sections": [
            ("做法", "「一陣一陣」拆成兩半：max>2×mean 證明突發存在、"
                    "frac(g<1)>50% 證明多數是微風——"
                    "兩者同時成立才是重尾間歇。"),
            ("細節", "只有突發沒有多數弱點 = 均勻放大；"
                    "只有弱點沒有突發 = 整體偏弱——"
                    "間歇是兩端的對比。"),
            ("啟示", "複合性質要拆成可獨立為假的斷言——"
                    "「忽強忽弱」其實是兩個聯合條件。"),
        ],
        "code": '// 突發存在 ∧ 多數偏弱\nCHECK(mx > 2*mean);\nCHECK(frac_below_mean > 0.5);\n// 只過一個都不算間歇',
        "narration": "第三個設計：間歇性要兩個斷言。"
                     "有突發、而且多數點偏弱——兩者同時成立"
                     "才是忽強忽弱。只有突發是均勻放大，"
                     "只有弱點是整體偏弱。"
                     "複合性質要拆成能獨立為假的斷言。",
    },
    {
        "tag": "設計 4",
        "title": "決定性三件套",
        "sections": [
            ("做法", "同參數同輸出（可回放）、不同時間不同值"
                    "（有演化）、不同 seed 不同場（有分離）——"
                    "隨機場的三個基本品質各一個 CHECK。"),
            ("細節", "「隨機」在引擎裡的全名是「確定性偽隨機」——"
                    "可回放是功能，不是限制。"),
            ("啟示", "偽隨機的驗收是三條等式與不等式——"
                    "相等驗決定性、不等驗演化和分離。"),
        ],
        "code": 'I(p,t) == I(p,t)        // 可回放\nI(p,0) != I(p,5)        // 時間演化\nI42(p) != I43(p)        // seed 分離',
        "narration": "第四個設計：決定性三件套。"
                     "同參數同輸出、隨時間演化、異 seed 異場——"
                     "三條各一個檢查。"
                     "引擎裡的隨機全名是確定性偽隨機，"
                     "可回放是功能不是限制。",
    },
    {
        "tag": "設計 5",
        "title": "退化輸入是活的規格",
        "sections": [
            ("做法", "sigma=0 → 強度恆一；n≤0 → 空尺表；"
                    "表外 n → 空——每個退化輸入一行斷言。"),
            ("細節", "退化行為不是「順便處理」是「明寫的承諾」——"
                    "測試把它固定下來，重構的人不敢改語義。"),
            ("啟示", "邊界行為沒寫進測試，就只是碰巧的實作細節——"
                    "有 CHECK 的退化才是規格。"),
        ],
        "code": 'GustField(σ=0).Intensity() == 1.0\nGolombMarks(0) == {}\nOptimalGolombRuler(99) == {}\n// 每個退化輸入一行斷言',
        "narration": "第五個設計：退化輸入是活的規格。"
                     "sigma 等於零強度恆一、n 非正回空表、"
                     "表外查詢回空——每個退化輸入一行斷言。"
                     "沒寫進測試的邊界行為只是碰巧的實作細節，"
                     "有斷言的退化才是規格。",
    },
    {
        "tag": "設計 6",
        "title": "答案表當測試資料",
        "sections": [
            ("做法", "尺表驗證內嵌已知最優長度表 "
                    "kOptLen={0,1,3,6,11,17,25,34,44,55,72,85}——"
                    "每階對照 OEIS 值。"),
            ("細節", "外部權威（已發表的最優尺）變成測試的"
                    "黃金標準——實作錯一個刻度就會偏離表值。"),
            ("啟示", "有外部權威答案的演算法，測試直接把答案"
                    "抄進來對照——OEIS 就是免費的測試資料集。"),
        ],
        "code": 'const int kOptLen[] = {0,1,3,6,11,17,\n                       25,34,44,55,72,85};\nCHECK(m.back() == kOptLen[n-1]);\n// OEIS A079426 = 黃金標準',
        "narration": "第六個設計：答案表當測試資料。"
                     "尺表驗證內嵌已發表的最優長度表，"
                     "每階對照——實作錯一個刻度就偏離表值。"
                     "有外部權威答案的演算法，"
                     "測試直接把答案抄進來對照——"
                     "公開數表就是免費的黃金標準。",
    },
    {
        "tag": "設計 7",
        "title": "複合斷言走因式分解",
        "sections": [
            ("做法", "Sample 的驗證不比較整個向量場——"
                    "斷言 Sample(p,t) ≡ flow(p,t) × Intensity(p,t) "
                    "這條因式分解等式。"),
            ("細節", "組合正確性只要驗「乘法真的乘了」——"
                    "流場和強度場各自的正確性由自己的測試負責。"),
            ("啟示", "組合器的測試是驗結合律不是驗結果——"
                    "零件各自驗過，組合只需要驗接縫。"),
        ],
        "code": 'v = gust.Sample(p, t, flow);\nCHECK(v.x == flow.x * gust.Intensity());\n// 不驗「對的風」——驗「真的乘了」',
        "narration": "第七個設計：複合斷言走因式分解。"
                     "複合取樣的測試不比較整個場——"
                     "只斷言輸出等於基底乘強度這條等式。"
                     "流場和強度場的正確性由各自的測試負責，"
                     "組合器只需要驗接縫：乘法真的乘了。",
    },
    {
        "tag": "設計 8",
        "title": "佇列的下一波",
        "sections": [
            ("做法", "D-1、E-1 入帳後，queued 排著："
                    "E-4 無戰敘事結算、D-2 神社實體、D-3 天命貨幣、"
                    "D-4 神話入侵、F-2~F-5 表現層收尾。"),
            ("細節", "Epic D 的後續全掛在 MythLayer 骨幹上——"
                    "D-1 先行的原因在此：骨幹不立，四個故事沒處掛。"),
            ("啟示", "佇列的序不是隨機的——"
                    "先做別人都依賴的骨幹，做骨幹的先做 spec。"),
        ],
        "code": 'queued: E-4 無戰敘事 · D-2 神社實體\n        D-3 天命貨幣 · D-4 神話入侵\n        F-2 UI scale · F-4 音訊\n# 全掛在已立的骨幹上',
        "narration": "第八個設計看佇列的下一波。"
                     "無戰敘事、神社實體、天命貨幣、神話入侵——"
                     "Epic D 的後續全掛在滲透層骨幹上。"
                     "佇列的序不是隨機的：先做別人都依賴的骨幹，"
                     "做骨幹的先做規格。",
    },
    {
        "kind": "cover",
        "title": "第二十二集清單",
        "lines": ["均值帶+極值+形狀三件套 · 承諾變斷言",
                  "間歇是兩個聯合條件 · 決定性三件套",
                  "退化即規格 · 權威表當資料 · 驗接縫不驗結果"],
        "narration": "第二十二集收在這裡。隨機系統的測試心法："
                     "不斷言數值、斷言形狀；不斷言結果、"
                     "斷言不變量和接縫。"
                     "隨機不是測不了——是要用統計量測。",
    },
]

SLIDES_EP23 = [
    {
        "kind": "cover",
        "title": "歷史手術",
        "lines": ["從實戰學 C++ 第二十三集",
                  "filter-repo 實戰 · 路徑清單 · format-patch 補植"],
        "narration": "第二十三集記一件大事：拆倉的歷史手術完成了。"
                     "git filter-repo 把一份歷史切成兩份——"
                     "引擎一百六十五檔、遊戲兩千三百五十一檔，"
                     "雙側建置測試全綠。"
                     "這一集記手術台上學到的每個坑。",
    },
    {
        "tag": "設計 1",
        "title": "path: 前綴產出空 repo",
        "sections": [
            ("做法", "--paths-from-file 的路徑行不加前綴——"
                    "寫 path:Core/ 會被當字面路徑，"
                    "濾完得到一個空 repo。"),
            ("細節", "錯誤不報錯是這類工具最壞的模式——"
                    "產出空倉庫之前，沒有任何訊號告訴你清單寫錯了。"),
            ("啟示", "讀工具的檔案格式文件讀到「每行是什麼」——"
                    "格式容錯的工具，錯得安靜。"),
        ],
        "code": '# 錯：path:Core/   ← 被當字面路徑\n# 對：Core/\n# 結果差異：一個空 repo，安靜產出',
        "narration": "第一個坑：路徑行不能加前綴。"
                     "手冊上某些格式要 path 冒號開頭——"
                     "這裡不行，寫了就被當字面路徑，"
                     "濾完得到一個空 repo，而且不報錯。"
                     "格式容錯的工具，錯得安靜。",
    },
    {
        "tag": "設計 2",
        "title": "清單要和 git ls-files 對帳",
        "sections": [
            ("做法", "路徑清單對照切點時刻的 git ls-files——"
                    "切點之後新增的檔（LedgerRegistryTest 等）"
                    "要補進清單再濾。"),
            ("細節", "清單寫於規劃時、執行於切割時——"
                    "中間落地的新檔不會自己出現在清單裡。"),
            ("啟示", "手術清單的效期到切點為止——"
                    "動刀前最後一步是重新對帳，不是複習舊清單。"),
        ],
        "code": '# 清單擬於 T0，切割於 T1\n# T0~T1 新增的檔不在清單\n# → 切割前：diff(ls-files, 清單)',
        "narration": "第二個坑：清單要對帳。"
                     "路徑清單擬於規劃時、執行於切割時——"
                     "中間落地的新檔不會自己出現在清單裡。"
                     "動刀前最後一步是拿切點時刻的 "
                     "git ls-files 重新對帳，不是複習舊清單。",
    },
    {
        "tag": "設計 3",
        "title": "在途 commit 用 format-patch 補植",
        "sections": [
            ("做法", "切割當下平行 session 還在提交——"
                    "L-6/L-8 的新 commit 用 format-patch 匯出、"
                    "補植到各側新歷史。"),
            ("細節", "切割是快照不是結界——在途工作不等人，"
                    "移植比叫所有人停手便宜。"),
            ("啟示", "大遷移的時機選「相對平靜」而非「完全靜止」——"
                    "補植管線準備好，就不用封凍整個專案。"),
        ],
        "code": 'git format-patch <range> --stdout > p.patch\n# 在新 repo：\ngit am p.patch   # 在途工作補植',
        "narration": "第三個坑：在途的提交怎麼辦。"
                     "切割當下別的 session 還在提交——"
                     "答案是 format-patch：匯出、補植到新歷史。"
                     "切割是快照不是結界——"
                     "準備好補植管線，就不用封凍整個專案。",
    },
    {
        "tag": "設計 4",
        "title": "GLFW_INCLUDE_NONE 的傳染",
        "sections": [
            ("做法", "GUI 庫為何不連 PotatoEngine：引擎 PUBLIC "
                    "定義 GLFW_INCLUDE_NONE——連了它，"
                    "IDEGUI 靠 glfw3.h 帶入系統 gl.h 的路被斷。"),
            ("細節", "最終走第三條路：兩個源檔改指 "
                    "${POTATO_ENGINE_ROOT}/——編譯進 GUI 庫但"
                    "源檔住引擎 repo。"),
            ("啟示", "compile definition 是會傳染的 API——"
                    "「連我的庫」其實是「吃我的編譯設定」，"
                    "評估消費方式要把傳染項算進去。"),
        ],
        "code": '# 引擎 PUBLIC 定義 GLFW_INCLUDE_NONE\n# 連 PotatoEngine → gl.h 不再被帶入\n# → GUI 庫缺符號\n# 解法：源檔走 ${POTATO_ENGINE_ROOT}/ 引用',
        "narration": "第四個坑解開上一集的謎：為什麼不連引擎庫。"
                     "引擎公開定義 GLFW_INCLUDE_NONE——"
                     "連了它，介面庫靠 glfw 帶入系統標頭的路就斷了。"
                     "最終走第三條路：源檔用引擎根目錄路徑引用。"
                     "compile definition 是會傳染的 API——"
                     "連庫其實是吃它的編譯設定。",
    },
    {
        "tag": "設計 5",
        "title": "驗收數字要對帳",
        "sections": [
            ("做法", "雙側驗證：引擎 28/28 MSVC+MinGW、"
                    "遊戲 44/44 雙工具鏈——合計 72。"),
            ("細節", "72 = 68 基線 + L-6/L-8/Jamming/QuasiModels "
                    "四支新增——每個數字都解釋得掉，不多不少。"),
            ("啟示", "拆倉後的綠燈要會算帳——"
                    "總數對得上基線加增量，才是真的沒丟測試。"),
        ],
        "code": '# engine 28 + game 44 = 72\n# = 68 基線 + 4 新增\n# 對不上的綠燈不算綠燈',
        "narration": "第五個坑：驗收數字要對帳。"
                     "引擎二十八、遊戲四十四，合計七十二——"
                     "正好是六十八基線加四支新增測試。"
                     "每個數字都解釋得掉，不多不少。"
                     "拆完之後的綠燈要會算帳——"
                     "對不上的綠燈不算綠燈。",
    },
    {
        "tag": "設計 6",
        "title": "置換決策：動哪邊不動哪邊",
        "sections": [
            ("做法", "遊戲側維持 C:\\HWC\\MingGoRTS 不動——"
                    "平行 session 在途、引擎仍是 repo 內真相；"
                    "PotatoEngine 鏡像由 filter-repo 產物取代。"),
            ("細節", "舊鏡像不刪——備份成 PotatoEngine-mirror-old，"
                    "萬一新歷史有問題還有退路。"),
            ("啟示", "置換選「變動最小那側先」——"
                    "在途工作多的那側不動，先讓新產物接住鏡像位。"),
        ],
        "code": 'MingGoRTS/        # 不動（在途多）\nPotatoEngine/     # ← filter-repo 產物\nPotatoEngine-mirror-old/  # 舊鏡像備份',
        "narration": "第六個坑：置換決策。"
                     "遊戲側不動——那邊平行工作在途；"
                     "先讓引擎 repo 的產物接住鏡像的位置，"
                     "舊鏡像備份留退路。"
                     "置換選變動最小那側先動，"
                     "在途多的那側等收斂。",
    },
    {
        "tag": "設計 7",
        "title": "跨層的證據是字串不是 enum",
        "sections": [
            ("做法", "E-4 和平語域的 HistorianInput 用 "
                    "peacePathZh 字串傳「談判/嚇阻/顛覆」——"
                    "不用 NoBattlePath enum。"),
            ("細節", "enum 住 Campaign 層，HistorianReport 在 "
                    "Gameplay——依賴方向不許回頭含入，"
                    "所以跨層的資料降級成字串。"),
            ("啟示", "分層的代價有時是「用字串不用 enum」——"
                    "這不是偷懶，是邊界在型別上的價格。"),
        ],
        "code": 'std::string peacePathZh; // 不用 NoBattlePath\n// Gameplay 不能含 Campaign 標頭\n// → 跨層值用字串載',
        "narration": "第七個坑是跨層的型別價格。"
                     "和平語域的路徑名用字串傳不用枚舉——"
                     "因為枚舉住戰役層，史官在玩法層，"
                     "依賴方向不許回頭。"
                     "分層的代價有時是用字串不用枚舉——"
                     "這是邊界在型別上的價格，不是偷懶。",
    },
    {
        "tag": "設計 8",
        "title": "重寫歷史的備註",
        "sections": [
            ("做法", "工作計畫的交接節明寫：新引擎 repo 歷史重寫過，"
                    "推送需要 force——把這個事實留在文件裡。"),
            ("細節", "filter-repo 的產物和原歷史沒有共同祖先——"
                    "push 不是 fast-forward 問題，是整棵樹換掉。"),
            ("啟示", "不可逆操作的交接要自帶警告——"
                    "下一個碰它的人要先知道這不是普通的 push。"),
        ],
        "code": '# 待辦明寫：\n# 「新引擎 repo 歷史重寫過，\n#   推送需 force」\n# 警告跟在操作後面，不分開',
        "narration": "第八個設計：重寫歷史要自帶警告。"
                     "工作計畫明寫新 repo 推送需要 force——"
                     "重寫過的歷史和原歷史沒有共同祖先。"
                     "不可逆操作的交接要自帶警告，"
                     "下一個碰它的人要先知道這不是普通的 push。",
    },
    {
        "kind": "cover",
        "title": "第二十三集清單",
        "lines": ["路徑不加前綴 · 清單對帳 ls-files · format-patch 補植",
                  "定義會傳染 · 綠燈要算帳 · 動最少那側",
                  "跨層用字串 · 重寫自帶警告"],
        "narration": "第二十三集收在這裡。歷史手術的全部要領："
                     "工具文件讀細、清單對帳、在途補植、"
                     "驗收算帳。monorepo 的離婚協議簽完了——"
                     "兩個 repo，各自有自己的歷史。",
    },
]

SLIDES_EP24 = [
    {
        "kind": "cover",
        "title": "場域模式",
        "lines": ["從實戰學 C++ 第二十四集",
                  "ShrineField · 回呼當出口 · 一次性結算 · 呼叫端接線"],
        "narration": "第二十四集講一個模式的誕生。"
                     "D-2 神社實體落地成 ShrineField——"
                     "GovernanceField 的姊妹類。"
                     "當第二個類長得跟第一個一模一樣，"
                     "模式就不再是巧合，是架構。",
    },
    {
        "tag": "設計 1",
        "title": "模式成形靠第二例",
        "sections": [
            ("做法", "ShrineField 複製 GovernanceField 的形狀："
                    "由「有地圖知識的呼叫端」每拍驅動，"
                    "BattleController 始終不認識地圖。"),
            ("細節", "戰鬥狀態機只管戰鬥；地圖語義（神社圈、"
                    "治理事件）由場域追蹤器轉譯後才進來。"),
            ("啟示", "一次是特例、兩次是模式——"
                    "姊妹類出現時，回看第一個類哪些是模式、"
                    "哪些只是它自己的細節。"),
        ],
        "code": '// GovernanceField：治理事件轉譯\n// ShrineField   ：神社互動轉譯\n// 共同形狀：呼叫端驅動 × controller 無知',
        "narration": "第一個設計：模式成形靠第二例。"
                     "神社場域追蹤器複製治理場域的形狀——"
                     "由有地圖知識的呼叫端每拍驅動，"
                     "戰鬥控制器始終不認識地圖。"
                     "一次是特例、兩次是模式：姊妹類出現時，"
                     "回頭看第一個類哪些是模式、哪些是細節。",
    },
    {
        "tag": "設計 2",
        "title": "回呼是全部出口",
        "sections": [
            ("做法", "對外介面只有兩個 std::function："
                    "onEvent 發事件字串、onFavor 發 "
                    "(spirit, delta)——不直接呼叫任何外層類。"),
            ("細節", "favor 真正的家在上層 MythLayer，"
                    "但 Gameplay 不能依賴 Campaign——"
                    "所以只發回呼，接線交給呼叫端。"),
            ("啟示", "下層模組不知道上層存在時，"
                    "回呼就是依賴方向的物理實現——"
                    "資料往上流，依賴不回流。"),
        ],
        "code": 'SetEventCallback(cb);   // → recorder / MythLog\nSetFavorCallback(cb);   // → myth.AdjustFavor\n// 類裡沒有任何 Campaign 標頭',
        "narration": "第二個設計：回呼是全部出口。"
                     "神社類對外只有兩個 function——"
                     "發事件字串、發好感增量。"
                     "好感的家在上層滲透系統，但玩法層不能回頭依賴，"
                     "所以只發回呼，接線交給呼叫端。"
                     "資料往上流，依賴不回流。",
    },
    {
        "tag": "設計 3",
        "title": "一次性結算的語義",
        "sections": [
            ("做法", "節點狀態機：Idle→Occupied→"
                    "Appeased/Provoked/Offered——ApplyChoice 只在 "
                    "Occupied 受理，結算後回 false。"),
            ("細節", "「拜過的神社不再受理」——一個互動點"
                    "只能結算一次，選擇因此有重量。"),
            ("啟示", "一次性互動比可重複互動更敘事——"
                    "不能反悔的選擇才是選擇。"),
        ],
        "code": 'bool ApplyChoice(i, choice) {\n    if (shrines[i].visual != Occupied)\n        return false;  // 拜過不受理\n    ...              // 只結算一次\n}',
        "narration": "第三個設計：一次性結算。"
                     "節點狀態機五態，選擇只在已進駐時受理——"
                     "拜過的神社不再受理。"
                     "一個互動點只能結算一次，選擇才有重量。"
                     "不能反悔的選擇，才是選擇。",
    },
    {
        "tag": "設計 4",
        "title": "選擇走既有 recorder",
        "sections": [
            ("做法", "選擇記錄不建通道——呼叫端把事件字串接進 "
                    "recorder.AddRecord，回放雜湊鏈自動覆蓋。"),
            ("細節", "BattleController 一行沒改——"
                    "神社選擇出現在回放事件流裡，"
                    "和移動、交戰同等待遇。"),
            ("啟示", "新互動入帳的最短路徑是「用舊通道」——"
                    "事件流已經是 record-is-truth 的，別另開。"),
        ],
        "code": 'onEvent → [&](msg){\n    recorder.AddRecord(tick, msg); }\n// 回放鏈自動覆蓋新互動',
        "narration": "第四個設計：選擇走既有的記錄器。"
                     "不建通道——事件字串接進錄製器，"
                     "回放雜湊鏈自動覆蓋新互動。"
                     "戰鬥控制器一行沒改，"
                     "神社選擇就和移動交戰同等待遇。"
                     "入帳的最短路徑，是用舊通道。",
    },
    {
        "tag": "設計 5",
        "title": "Bind 濾型別不濟座標",
        "sections": [
            ("做法", "Bind 收整張地圖的互動物清單，"
                    "只挑 type==\"shrine\" 的建節點——"
                    "內容側加神社不用碰程式。"),
            ("細節", "節點存 mapIndex 回指原始互動物——"
                    "渲染對帳用，地圖資料和場域狀態可互相核對。"),
            ("啟示", "「哪些物件屬於這個系統」由資料宣告——"
                    "型別標籤是內容側的擴充點。"),
        ],
        "code": 'void Bind(interactables) {\n    for (auto& m : interactables)\n        if (m.type == "shrine")\n            shrines.push_back({...});\n}',
        "narration": "第五個設計：綁定濾型別。"
                     "收整張地圖的互動物，只挑神社型別建節點——"
                     "內容側在地圖上加神社不用碰程式。"
                     "節點還存回原始互動物的索引，"
                     "渲染和狀態可以互相對帳。",
    },
    {
        "tag": "設計 6",
        "title": "視覺態給渲染層讀",
        "sections": [
            ("做法", "GetShrines() 回 const 參考——"
                    "渲染層讀 ShrineVisual 上色、讀 offering "
                    "字串做環境敘事，但不許寫。"),
            ("細節", "供品態是內容側給定的字串——"
                    "「這座祠供什麼」是地圖資料不是程式常數。"),
            ("啟示", "視覺態和邏輯態可以是同一個 enum——"
                    "重點是出口唯讀，誰讀不會改變狀態。"),
        ],
        "code": 'enum ShrineVisual { Idle, Occupied,\n    Appeased, Provoked, Offered };\n// 渲染層讀色調，邏輯層讀受理',
        "narration": "第六個設計：視覺態給渲染層讀。"
                     "五個視覺態供渲染上色，供品字串供環境敘事——"
                     "「這座祠供什麼」是地圖資料不是程式常數。"
                     "視覺態和邏輯態共用一個枚舉，"
                     "重點是出口唯讀，誰讀都不會改變狀態。",
    },
    {
        "tag": "設計 7",
        "title": "staging 解散了",
        "sections": [
            ("做法", "_split_stage/engine-src/CMakeLists.txt "
                    "刪除——演習場在真刀真槍完成後功成身退。"),
            ("細節", "演習產物的生命週期到「正式完成」為止——"
                    "留著它會被誤認為還在演習。"),
            ("啟示", "臨時產物要記得收——完成後還躺著的 "
                    "staging 檔，是給未來自己的假線索。"),
        ],
        "code": '# _split_stage/ 完成使命\n# → 檔案清掉，別留演習場\n#   誤導後來的人',
        "narration": "第七個設計是收尾：演習場解散。"
                     "拆分驗證完畢，staging 的候選清單刪掉——"
                     "演習產物的生命週期到正式完成為止。"
                     "留著它會被誤認為還在演習。"
                     "臨時產物要記得收，"
                     "躺著的演習場是給未來的假線索。",
    },
    {
        "tag": "設計 8",
        "title": "守衛也出現在主 repo",
        "sections": [
            ("做法", "主 CMakeLists 尾部出現雙層 "
                    "endif() # STANDALONE——monorepo 本體"
                    "也補上 standalone 守衛。"),
            ("細節", "拆倉產生的知識回流：本體在過渡期同時是"
                    "「遊戲 repo」和「含引擎的 monorepo」——"
                    "守衛讓兩種消費方式都安全。"),
            ("啟示", "過渡期架構要同時滿足新舊消費者——"
                    "守衛是最便宜的雙模開關。"),
        ],
        "code": 'endif() # STANDALONE\nendif() # STANDALONE\n# 雙層：本體也進過渡期',
        "narration": "第八個設計：守衛回流主 repo。"
                     "主清單尾部出現雙層 standalone 結尾——"
                     "本體在過渡期同時是遊戲 repo 和含引擎的倉庫，"
                     "守衛讓兩種消費方式都安全。"
                     "過渡期架構要同時滿足新舊消費者，"
                     "守衛是最便宜的雙模開關。",
    },
    {
        "kind": "cover",
        "title": "第二十四集清單",
        "lines": ["第二例成模式 · 回呼即依賴方向 · 一次結算有重量",
                  "入帳用舊通道 · Bind 濾型別 · 視覺態唯讀",
                  "演習場解散 · 過渡期雙模守衛"],
        "narration": "第二十四集收在這裡。場域模式的全部要件："
                     "呼叫端驅動、回呼當出口、一次結算、"
                     "舊通道入帳。Epic D 的第二張地圖，"
                     "是照著第一張的模式長出來的。",
    },
]

SLIDES_EP25 = [
    {
        "kind": "cover",
        "title": "敘事也要對帳",
        "lines": ["從實戰學 C++ 第二十五集",
                  "E-4 落地 · 無戰體詞彙 · 收服側加權 · 結局重數帳本"],
        "narration": "第二十五集講敘事層學會對帳。"
                     "E-4 無戰敘事落地進章回體例——"
                     "和平路徑有了自己的收兵詞，"
                     "結局計分把無戰章節計入收服側，"
                     "而最漂亮的一筆是：結局文案不信用進來的計數，"
                     "它自己翻帳本重數。",
    },
    {
        "tag": "設計 1",
        "title": "無戰體有自己的詞彙",
        "sections": [
            ("做法", "ClosingHook 給三條和平路徑各配專屬收兵詞："
                    "談判→「談笑罷兵」、嚇阻→「不戰而退敵」、"
                    "顛覆→「敵營自潰」。"),
            ("細節", "路徑身份保留到文字層——讀者從收兵詞"
                    "就能分辨這章是談下來的還是嚇跑的。"),
            ("啟示", "系統性的差異要穿透到表面——"
                    "如果三條路共用一句「和平收場」，"
                    "底下的機制區分就白做了。"),
        ],
        "code": 'if (peacePathZh == "談判") hook = "談笑罷兵";\nelse if (peacePathZh == "嚇阻") hook = "不戰而退敵";\nelse if (peacePathZh == "顛覆") hook = "敵營自潰";',
        "narration": "第一個設計：無戰體有自己的詞彙。"
                     "三條和平路徑各配專屬收兵詞——"
                     "談判是談笑罷兵、嚇阻是不戰而退敵、顛覆是敵營自潰。"
                     "路徑的身份保留到文字層。"
                     "系統性的差異要穿透到表面——"
                     "三條路共用一句話，底下的區分就白做了。",
    },
    {
        "tag": "設計 2",
        "title": "不戰之局不入勝負",
        "sections": [
            ("做法", "peacePathZh 非空的檢查排在 draw/victory "
                    "判定之前——和平章節根本不走勝負分支。"),
            ("細節", "優先序即語義：「不戰」不是第三種勝負結果，"
                    "是勝負框架之外的路徑。"),
            ("啟示", "分支順序在說話——把新類別插在舊判定的"
                    "哪個位置，決定它是「特例」還是「另一類」。"),
        ],
        "code": 'if (!ctx.peacePathZh.empty()) { ... }\nelse if (ctx.draw) { ... }\nelse if (ctx.playerVictory) { ... }\n// 和平檢查在最前——不入勝負',
        "narration": "第二個設計：不戰之局不入勝負。"
                     "和平路徑的檢查排在平手和勝利判定之前——"
                     "和平章節根本不走勝負分支。"
                     "不戰不是第三種勝負結果，是勝負框架之外的路徑。"
                     "分支順序在說話：插在哪個位置，"
                     "決定它是特例還是另一類。",
    },
    {
        "tag": "設計 3",
        "title": "收服側加權",
        "sections": [
            ("做法", "ResolveEnding 把 peaceChapters 併入 "
                    "subdued：subdued = in.subdued + "
                    "in.peaceChapters——不戰而勝是收服的最高形式。"),
            ("細節", "增補後的 subdued 同時餵兩個地方："
                    "conquestHigh 比較、fallthrough 的 "
                    "conquestScore 計分。"),
            ("啟示", "加權要穿透全管線——只改一處的增補"
                    "會讓主判定和 fallback 用不同版本的數字。"),
        ],
        "code": 'const int subdued = in.subdued + in.peaceChapters;\nconst bool conquestHigh = in.annihilated > subdued;\n// fallback 計分也用同一個 subdued',
        "narration": "第三個設計：收服側加權。"
                     "無戰章節併入收服數——不戰而勝是收服的最高形式。"
                     "重點是增補後的數字同時餵主判定和降級計分。"
                     "只改一處的增補，"
                     "會讓兩條路用不同版本的數字——"
                     "加權要穿透全管線。",
    },
    {
        "tag": "設計 4",
        "title": "結局自己翻帳本",
        "sections": [
            ("做法", "Mandate 結局要寫「凡 N 章不戰而屈」時，"
                    "不信 EndingInput.peaceChapters——"
                    "自己遍歷 ledger.Records() 數處置記錄。"),
            ("細節", "Negotiated/Intimidated/Defected 三種"
                    "處置都算不戰之勝——計數從權威記錄重導。"),
            ("啟示", "敘事對帳：會寫進史書的數字，"
                    "必須能被帳本驗證——輸入欄位是快取，"
                    "帳本才是真相。"),
        ],
        "code": 'int peaceN = 0;\nfor (auto& rec : ledger.Records())\n    if (rec.disposition == Negotiated || ...)\n        ++peaceN;\n// 不信入參，重數帳本',
        "narration": "第四個設計是整集最漂亮的：結局自己翻帳本。"
                     "要寫「凡幾章不戰而屈」時，"
                     "不信用進來的計數欄位——"
                     "自己遍歷帳本記錄、數三種和平處置。"
                     "會寫進史書的數字必須能被帳本驗證："
                     "輸入欄位是快取，帳本才是真相。",
    },
    {
        "tag": "設計 5",
        "title": "可驗證的宣稱",
        "sections": [
            ("做法", "「凡 N 章不戰而屈，仁者之師也」——"
                    "N 是可數的事實宣稱，讀者回查帳本可驗。"),
            ("細節", "peaceN>0 才附加這句——沒有無戰章節的 "
                    "Mandate 結局不憑空邀功。"),
            ("啟示", "敘事系統的最高境界是誠實——"
                    "每句史筆背後都有記錄撐著，"
                    "邀功的話說不出口。"),
        ],
        "code": 'if (peaceN > 0) {\n    snprintf(buf, "凡 %d 章不戰而屈，仁者之師也。",\n             peaceN);\n    text += buf;  // 有帳才說\n}',
        "narration": "第五個設計：可驗證的宣稱。"
                     "凡幾章不戰而屈——這個數字是可數的事實宣稱，"
                     "讀者回查帳本可驗。"
                     "而且計數為零時這句根本不附加——"
                     "沒有無戰章節的結局不憑空邀功。"
                     "敘事系統的最高境界是誠實。",
    },
    {
        "tag": "設計 6",
        "title": "跨層用字串不用枚舉",
        "sections": [
            ("做法", "peacePathZh 是 std::string 不是 "
                    "enum——Gameplay 層不收上層的型別。"),
            ("細節", "字串是依賴方向的價格：上層決定詞彙，"
                    "下層只比對字面值——失去型別安全，"
                    "換來單向依賴不破。"),
            ("啟示", "架構邊界有代價且要明付——"
                    "寧可在介面上用弱型別，"
                    "也不為了漂亮引入反向 include。"),
        ],
        "code": 'std::string peacePathZh;  // 不是 enum class Path\n// 上層擁有詞彙，下層只比對字面值\n// 弱型別 = 依賴方向的價格',
        "narration": "第六個設計：跨層用字串不用枚舉。"
                     "和平路徑傳下來的是字串不是枚舉——"
                     "玩法層不收上層的型別。"
                     "字串是依賴方向的價格：上層擁有詞彙，"
                     "下層只比對字面值。"
                     "寧可用弱型別，也不為漂亮引入反向依賴。",
    },
    {
        "tag": "設計 7",
        "title": "垂直切片先吃到",
        "sections": [
            ("做法", "DuanqiaoPlayable 接上 GustField——"
                    "P-4 陣風場讓霧標記漂移忽快忽慢，"
                    "玩家看得見的研究落地。"),
            ("細節", "sigma=0.7、seed=42 固定——"
                    "展示用的參數也守決定性規矩，"
                    "切片裡的雲每次演示飄法相同。"),
            ("啟示", "研究落地的最後一哩是可玩切片——"
                    "測試證明它對，切片證明它值得。"),
        ],
        "code": 'Quasi::GustField fogGust(5, 42, 0.08f, 0.7f);\nsync.SetFogDriftGust(&fogGust);\n// 切片可見：雲忽快忽慢',
        "narration": "第七個設計：垂直切片先吃到。"
                     "渡口切片接上陣風場——霧標記的漂移忽快忽慢，"
                     "玩家看得見的研究落地。"
                     "參數固定種子守決定性規矩，"
                     "每次演示雲的飄法相同。"
                     "測試證明它對，切片證明它值得。",
    },
    {
        "tag": "設計 8",
        "title": "同步白名單要養",
        "sections": [
            ("做法", "sync 腳本的 TESTS 清單補上 "
                    "UnsolvedBatch3Test——鏡像同步是白名單制，"
                    "新測試要手動入列。"),
            ("細節", "白名單的代價是遺忘——引擎側每多一支測試，"
                    "上游鏡像就少一份覆蓋，直到有人記得補。"),
            ("啟示", "白名單 vs 黑名單是安全姿態的選擇："
                    "寧可漏同步（可補）也不可多同步（難收）。"),
        ],
        "code": 'TESTS="... QuasiModelsTest \\\n    UnsolvedBatch3Test FakeCheatModule"\n# 新測試要手動入列——白名單制',
        "narration": "第八個設計：同步白名單要養。"
                     "鏡像同步腳本的測試清單補上批次三——"
                     "白名單制意味新測試要手動入列。"
                     "白名單的代價是遺忘，但這是刻意的安全姿態："
                     "寧可漏同步可以補，也不可多同步難收。",
    },
    {
        "kind": "cover",
        "title": "第二十五集清單",
        "lines": ["無戰體詞彙 · 勝負框架之外 · 收服側加權穿透全管線",
                  "結局重數帳本 · 可驗證的宣稱 · 字串是依賴的價格",
                  "切片吃到陣風 · 白名單要養"],
        "narration": "第二十五集收在這裡。敘事層學會對帳——"
                     "史筆的每個數字都能被帳本驗證，"
                     "沒有記錄撐著的話說不出口。"
                     "E-4 把「不戰而勝」從機制寫進了文體，"
                     "章回體例從此認得和平。",
    },
]

SLIDES_EP26 = [
    {
        "kind": "cover",
        "title": "沒有戰鬥的戰報",
        "lines": ["從實戰學 C++ 第二十六集",
                  "和平語域 · 戰歷行的無戰對句 · 不入陣亡錄 · 制度追上現實"],
        "narration": "第二十六集講一篇沒有戰鬥的戰報怎麼寫。"
                     "E-4 落地進史官系統——無戰章節不經戰鬥狀態機，"
                     "但戰報照樣要成文。"
                     "答案是：不刪欄位，給每個欄位配無戰對句。",
    },
    {
        "tag": "設計 1",
        "title": "同一份版面，兩種文體",
        "sections": [
            ("做法", "組裝段開頭一個 bool 分岔：peace = "
                    "!peacePathZh.empty()——和平章節走自己的"
                    "整段文體，不是在戰報裡挖洞。"),
            ("細節", "戰報的版面結構不變：史官曰開場、"
                    "敵將去向、查帳段、省略計數——"
                    "無戰版只是換了內容，沒有換骨架。"),
            ("啟示", "新類別的輸出要撐滿舊版面——"
                    "讓讀者看不出「這份是特別的」，"
                    "格式的一致本身就是誠實。"),
        ],
        "code": 'const bool peace = !in.peacePathZh.empty();\nif (peace) { 史官曰...兵不血刃... }\nelse      { 史官曰...戰歷 N 秒... }\n// 版面不變，文體分岔',
        "narration": "第一個設計：同一份版面，兩種文體。"
                     "組裝段開頭一個布林分岔——和平章節走整段自己的文體，"
                     "不是在戰報裡挖洞。"
                     "史官曰、敵將去向、查帳段、省略計數，版面不變。"
                     "讓讀者看不出這份是特別的——"
                     "格式的一致本身就是誠實。",
    },
    {
        "tag": "設計 2",
        "title": "路徑句點名貨幣",
        "sections": [
            ("做法", "三條路徑的句子各自點名它消耗的貨幣："
                    "談判「以民心曉之」、嚇阻「軍威所懾」、"
                    "顛覆「內應發於敵帳」。"),
            ("細節", "敘事句同時是機制的註腳——讀完戰報"
                    "就知道這章動用的是民心、軍威還是情報帳。"),
            ("啟示", "文字和機制同源——句子裡的每個關鍵詞"
                    "都對應一個真實扣過的資源欄位。"),
        ],
        "code": '談判 → "以民心曉之...議和而去"\n嚇阻 → "軍威所懾...懾服而去"\n顛覆 → "內應發於敵帳...變節投誠"',
        "narration": "第二個設計：路徑句點名貨幣。"
                     "談判寫以民心曉之、嚇阻寫軍威所懾、"
                     "顛覆寫內應發於敵帳——"
                     "敘事句同時是機制的註腳，"
                     "讀完就知道這章動用了哪本帳。"
                     "文字和機制同源，"
                     "每個關鍵詞都對應一個真實扣過的欄位。",
    },
    {
        "tag": "設計 3",
        "title": "「不入陣亡錄」是儀式句",
        "sections": [
            ("做法", "三條路徑句各以「不入陣亡錄」收尾——"
                    "重複不是冗贅，是儀式性的否定宣告。"),
            ("細節", "戰報的天職是記死亡——無戰章節要"
                    "明說「這裡沒有死訊」，不能讓空白"
                    "被誤讀成漏記。"),
            ("啟示", "否定也要書寫——「什麼沒有發生」"
                    "和「什麼發生了」同樣是記錄的職責。"),
        ],
        "code": '"議和而去——不入陣亡錄。"\n"懾服而去——不入陣亡錄。"\n"變節投誠——不入陣亡錄。"\n// 重複三次的否定，是儀式',
        "narration": "第三個設計：不入陣亡錄是儀式句。"
                     "三條路徑句各以同一句收尾——重複不是冗贅，"
                     "是儀式性的否定宣告。"
                     "戰報的天職是記死亡，無戰章節要明說"
                     "這裡沒有死訊，不能讓空白被誤讀成漏記。"
                     "否定也要書寫。",
    },
    {
        "tag": "設計 4",
        "title": "戰歷行的無戰對句",
        "sections": [
            ("做法", "戰鬥版寫「戰歷 N 秒」的位置，"
                    "和平版寫「不戰而定」——欄位不省略，"
                    "配對句。"),
            ("細節", "測試用雙斷言鎖住：!Has(戰歷) 確認"
                    "秒數行消失、Has(不戰而定) 確認對句在場。"),
            ("啟示", "取代要成對驗證——只斷言「舊句不在」"
                    "會放過「什麼都沒寫」的退化；"
                    "缺席和出席都要斷言。"),
        ],
        "code": 'Check(!Has(r.text, "戰歷"));      // 秒數行消失\nCheck(Has(r.text, "不戰而定"));   // 對句在場\n// 取代 = 缺席斷言 + 出席斷言',
        "narration": "第四個設計：戰歷行的無戰對句。"
                     "寫秒數的位置改寫不戰而定——欄位不省略，配對句。"
                     "測試用雙斷言鎖住：舊句消失、新句在場。"
                     "只斷言舊句不在，"
                     "會放過什麼都沒寫的退化——"
                     "缺席和出席都要斷言。",
    },
    {
        "tag": "設計 5",
        "title": "查帳段不認文體",
        "sections": [
            ("做法", "來源統計與省略計數對和平章節同規——"
                    "「本報告省略 N 項」在無戰戰報裡照樣在場。"),
            ("細節", "查帳機器不分文體：它數的是記錄，"
                    "記錄照樣存在——省略計數的恆在場"
                    "是完整性宣示。"),
            ("啟示", "下游的誠實機制不該感知上游的新類別——"
                    "同規通過的設計，比加特判的設計耐用。"),
        ],
        "code": '// 和平語域註解：\n// 「查帳段與省略計數同規」\n// ——審計機器不認文體，只認記錄',
        "narration": "第五個設計：查帳段不認文體。"
                     "來源統計和省略計數對和平章節同規——"
                     "本報告省略幾項，在無戰戰報裡照樣在場。"
                     "審計機器數的是記錄，不分文體。"
                     "下游的誠實機制不該感知上游的新類別——"
                     "同規通過，比加特判耐用。",
    },
    {
        "tag": "設計 6",
        "title": "字串介面要配泛稱句",
        "sections": [
            ("做法", "peacePathZh 不在三詞彙之內時降級到"
                    "「偃旗息鼓」——字串介面的 else 分支"
                    "是給未來路徑的座位。"),
            ("細節", "弱型別跨層的配套：下層不認得新詞時，"
                    "仍要產出合理的文體——不能因為上層"
                    "發明了新路徑就寫出空白。"),
            ("啟示", "字串比對一定要有 else——"
                    "弱型別的邊界上，降級句是相容性的一部分。"),
        ],
        "code": 'else {\n    t += "「" + g + "」偃旗息鼓——不入陣亡錄。";\n}\n// 不認得的路徑詞 → 泛稱句',
        "narration": "第六個設計：字串介面要配泛稱句。"
                     "路徑詞不在三詞彙內時降級到偃旗息鼓——"
                     "字串介面的 else 分支是給未來路徑留的座位。"
                     "上層發明新詞時下層不能寫出空白。"
                     "弱型別的邊界上，降級句是相容性的一部分。",
    },
    {
        "tag": "設計 7",
        "title": "制度追上現實",
        "sections": [
            ("做法", "AGENTS.md 改寫：上游 PotatoEngine 從"
                    "「唯讀鏡像」改述為「獨立引擎 repo，"
                    "扁平模組佈局」——文件追上拆倉後的世界。"),
            ("細節", "CI workflow 路徑清單同步更新："
                    "GUI 移出觸發清單、Media 加入——"
                    "觸發條件反映新邊界。"),
            ("啟示", "大手術之後的紙上作業是收尾不是附錄——"
                    "文件慢一天，就有一天的協作者"
                    "拿著舊地圖走新世界。"),
        ],
        "code": '# AGENTS.md：唯讀鏡像 → 獨立引擎 repo\n# workflow：- GUI/**  + Media/**\n# 紙上作業是收尾，不是附錄',
        "narration": "第七個設計：制度追上現實。"
                     "交接文件裡的上游從唯讀鏡像改述為獨立引擎倉庫，"
                     "扁平模組佈局；CI 觸發路徑同步更新。"
                     "大手術之後的紙上作業是收尾不是附錄——"
                     "文件慢一天，"
                     "就有一天的協作者拿著舊地圖走新世界。",
    },
    {
        "tag": "設計 8",
        "title": "驗收寫進 spec",
        "sections": [
            ("做法", "E-4 spec 標 done 時把驗證紀錄烙進文件："
                    "MSVC 12/12、MinGW 25/25、banned 函式"
                    "掃描無新增——結果是文件的一部分。"),
            ("細節", "spec 的 Verification 段從「待跑清單」"
                    "變成「跑過的成績單」——狀態欄說 done，"
                    "證據欄說為什麼。"),
            ("啟示", "done 要有成績單才可信——"
                    "狀態欄是宣告，驗證欄是證據，"
                    "兩欄齊全才算數。"),
        ],
        "code": "status: 'done'\nVerification:\n  MSVC 12/12 全綠\n  MinGW 25/25 PASS\n  banned 函式掃描無新增",
        "narration": "第八個設計：驗收寫進規格書。"
                     "標完成時把驗證紀錄烙進文件——"
                     "雙工具鏈成績和禁用函式掃描都在。"
                     "狀態欄說完成了，證據欄說為什麼。"
                     "done 要有成績單才可信："
                     "宣告和證據，兩欄齊全才算數。",
    },
    {
        "kind": "cover",
        "title": "第二十六集清單",
        "lines": ["版面不變文體分岔 · 路徑句點名貨幣 · 否定也要書寫",
                  "缺席+出席雙斷言 · 查帳段同規 · else 配泛稱句",
                  "文件追上拆倉 · done 附成績單"],
        "narration": "第二十六集收在這裡。沒有戰鬥的戰報"
                     "靠對句撐滿版面——每個欄位都有無戰版本，"
                     "每句史筆都有帳本撐著。"
                     "和平不是戰報的空白，是另一種成文。",
    },
]

SLIDES_EP27 = [
    {
        "kind": "cover",
        "title": "審查的肌肉記憶",
        "lines": ["從實戰學 C++ 第二十七集",
                  "D-2 審查輪 · collect-then-dispatch · 先結算再回呼 · "
                  "mapIndex 定址"],
        "narration": "第二十七集講一個比修正本身更重要的訊號。"
                     "D-2 神社實作進審查輪——而修掉的每一個雷，"
                     "都是 D-1 審查抓過的同一類。"
                     "上次的教訓這次主動避開：審查長出了肌肉記憶。",
    },
    {
        "tag": "設計 1",
        "title": "collect-then-dispatch 第二次",
        "sections": [
            ("做法", "Update 掃描時把發現事件收進 pending "
                    "vector，掃完才逐筆派出——回呼若在迭代中"
                    "再入（例如呼叫 Bind）不會懸空迭代器。"),
            ("細節", "D-1 審查輪抓過同款：MythLayer 邊升階"
                    "邊發事件導致 Region& 懸空——這次沒等"
                    "審查抓，寫的時候就避開了。"),
            ("啟示", "審查的最高回報不是修好 bug——"
                    "是下一個功能不再踩同一個坑。"),
        ],
        "code": 'std::vector<std::string> pending;\nfor (auto& s : shrines) {\n    ...; pending.push_back(...);\n}\nfor (auto& m : pending) onEvent(m);',
        "narration": "第一個設計：收集再派出，第二次。"
                     "掃描時把事件收進待發清單，掃完才逐筆派——"
                     "回呼在迭代中再入也不會懸空迭代器。"
                     "上次審查輪抓過同款雷，這次寫的時候就避開。"
                     "審查的最高回報不是修好蟲，"
                     "是下一個功能不再踩同一個坑。",
    },
    {
        "tag": "設計 2",
        "title": "先結算再發回呼",
        "sections": [
            ("做法", "ApplyChoice 先把 visual 設成終態，"
                    "才發 onEvent/onFavor——回呼裡再入 "
                    "ApplyChoice 會因非 Occupied 被拒。"),
            ("細節", "順序即不變量：發通知時物件已結算完畢——"
                    "觀察者看到的永遠是「已完成的現實」，"
                    "不是進行中。"),
            ("啟示", "對外發通知前先把內部態收斂——"
                    "回呼是觀察者的起點，"
                    "不該是你的中途點。"),
        ],
        "code": 's->visual = next;   // 先結算\nonEvent("神社[#..]:.."); // 再通知\nonFavor(spirit, delta);\n// 再入 ApplyChoice → 已被拒',
        "narration": "第二個設計：先結算再發回呼。"
                     "視覺態先設成終態才發通知——"
                     "回呼裡再入同一個選擇會因已結算被拒。"
                     "順序即不變量：觀察者看到的永遠是完成的現實，"
                     "不該是你的中途點。",
    },
    {
        "tag": "設計 3",
        "title": "非法輸入要「什麼都不做」",
        "sections": [
            ("做法", "switch 補 default 分支——非法 "
                    "ShrineChoice 值直接回 false；越界 "
                    "mapIndex 同樣拒絕，兩種拒絕都不發回呼。"),
            ("細節", "測試斷言「什麼都沒發生」：節點未結算、"
                    "回呼計數仍為一（只有發現事件）——"
                    "拒絕的乾淨程度被量化驗證。"),
            ("啟示", "enum 形參也會收到界外值——"
                    "static_cast 能造出任何值，"
                    "default 分支是對呼叫端的防禦。"),
        ],
        "code": 'default:\n    return false; // 非法列舉：不結算不發回呼\nCheck(cb == 1, "非法選擇不發回呼");',
        "narration": "第三個設計：非法輸入要什麼都不做。"
                     "switch 補預設分支，越界索引同樣拒絕——"
                     "兩種拒絕都不發回呼。"
                     "測試斷言什麼都沒發生：節點沒結算、"
                     "回呼計數不動。"
                     "枚舉形參也能被強轉造出界外值——"
                     "預設分支是對呼叫端的防禦。",
    },
    {
        "tag": "設計 4",
        "title": "用原圖索引定址",
        "sections": [
            ("做法", "IsPending/ApplyChoice/GetShrine 一律收 "
                    "mapIndex——原圖互動物的索引，不是內部 "
                    "vector 的位置。"),
            ("細節", "與 GovernanceField::IsBurned 同慣例——"
                    "渲染層拿到地圖索引就能直接對帳，"
                    "不用懂場域的內部排列。"),
            ("啟示", "對外識別碼要用呼叫端本來就有的那把尺——"
                    "內部索引是實作細節，"
                    "洩漏到介面就是耦合。"),
        ],
        "code": 'bool ApplyChoice(size_t mapIndex, ...);\nShrineState* Find(size_t mapIndex);\n// 內部 vector 順序 ≠ 對外定址',
        "narration": "第四個設計：用原圖索引定址。"
                     "三個查詢口一律收原圖互動物索引——"
                     "跟治理場域的慣例一致，"
                     "渲染層拿地圖索引直接對帳。"
                     "對外識別碼要用呼叫端本來就有的尺——"
                     "內部索引洩漏到介面就是耦合。",
    },
    {
        "tag": "設計 5",
        "title": "潰逃的兵不拜拜",
        "sections": [
            ("做法", "進駐判定多了 IsRouting() 檢查——"
                    "士氣歸零的潰逃隊經過神社圈不觸發互動。"),
            ("細節", "觸發條件寫進領域語義：「活著的、"
                    "還在戰鬥的我軍」才有資格做選擇——"
                    "逃兵沒有拜神這個動作。"),
            ("啟示", "幾何條件之外要問敘事條件——"
                    "「誰進圈」不只是座標問題，"
                    "也是狀態問題。"),
        ],
        "code": 'if (sq->GetFaction() != 0 ||\n    sq->IsRouting())\n    continue;  // 敵隊與潰逃隊都不算',
        "narration": "第五個設計：潰逃的兵不拜拜。"
                     "進駐判定補上潰逃檢查——"
                     "士氣歸零的隊伍經過神社圈不觸發。"
                     "觸發條件寫進領域語義：還在戰鬥的我軍"
                     "才有資格做選擇。"
                     "誰進圈不只是座標問題，也是狀態問題。",
    },
    {
        "tag": "設計 6",
        "title": "const 把唯讀寫進簽名",
        "sections": [
            ("做法", "Update(BattleController&) 改 "
                    "Update(const BattleController&)——"
                    "場域只讀戰鬥狀態，簽名把這件事說死。"),
            ("細節", "同批順手把半徑比較改成 "
                    "LengthSquared()<=r²——省一次開根號，"
                    "判定結果等價。"),
            ("啟示", "const 是介面誠實度——呼叫端看一眼簽名"
                    "就知道這個函式不會動它的物件。"),
        ],
        "code": 'void Update(const BattleController& battle);\n// const：我讀你，但不動你\nif (d.LengthSquared() <= r*r) ...',
        "narration": "第六個設計：const 把唯讀寫進簽名。"
                     "驅動函式改收唯讀參考——場域只讀戰鬥狀態，"
                     "簽名把這件事說死。"
                     "順手把半徑比較換成平方距離，省一次開根號。"
                     "const 是介面誠實度——"
                     "看一眼簽名就知道它不動你的物件。",
    },
    {
        "tag": "設計 7",
        "title": "防不住的危險寫成警告",
        "sections": [
            ("做法", "Bind 註解明寫：重綁會清掉已結算狀態，"
                    "重拜同一節點會二次入帳 favor——"
                    "熱重載場景勿複用本物件。"),
            ("細節", "這個坑防不住（綁定本就該可重來），"
                    "但能標明——把隱性代價變成介面文件。"),
            ("啟示", "不是所有危險都能用型別擋——"
                    "擋不住的寫成警告註解，"
                    "讓誤用至少是「看過警告後的選擇」。"),
        ],
        "code": '// Bind 註解：重綁清掉已結算狀態——\n// favor 是持久資源，重拜會二次入帳\n// 熱重載場景勿複用本物件',
        "narration": "第七個設計：防不住的危險寫成警告。"
                     "綁定函式註解明寫：重綁會清掉結算狀態，"
                     "重拜同一節點好感會二次入帳。"
                     "這個坑防不住但能標明——"
                     "把隱性代價變成介面文件，"
                     "讓誤用至少是看過警告後的選擇。",
    },
    {
        "tag": "設計 8",
        "title": "測試也一起硬化",
        "sections": [
            ("做法", "setvbuf 關 stdout 緩衝（崩潰時 log "
                    "不丟）；所有 [] 索引改 .at()（越界即拋"
                    "不默讀）；BattleController 給明確尺寸。"),
            ("細節", "新增負空間斷言：「非法選擇不發回呼」"
                    "數的是沒發生的事——cb==1 斷言的是"
                    "靜默。"),
            ("啟示", "測試基礎設施的坑也是坑——"
                    "緩衝丟輸出、[] 默讀越界，"
                    "都會讓測試報告撒謊。"),
        ],
        "code": 'setvbuf(stdout, nullptr, _IONBF, 0);\nshrines.at(0)   // 越界即拋，不默讀\nCheck(cb == 1); // 斷言靜默',
        "narration": "第八個設計：測試也一起硬化。"
                     "輸出關緩衝讓崩潰不丟日誌；"
                     "索引全改點 at 越界即拋不默讀；"
                     "斷言開始數沒發生的事——非法選擇的靜默被量化。"
                     "測試基礎設施的坑也是坑，"
                     "會讓測試報告撒謊的都要先堵。",
    },
    {
        "kind": "cover",
        "title": "第二十七集清單",
        "lines": ["collect-then-dispatch 第二次 · 先結算再回呼",
                  "非法輸入什麼都不做 · mapIndex 定址 · 潰逃不拜",
                  "const 寫進簽名 · 防不住寫警告 · 測試硬化"],
        "narration": "第二十七集收在這裡。D-2 的審查輪"
                     "幾乎是 D-1 的重播——但這次的修正"
                     "寫在實作當下，不等審查抓。"
                     "組織學到了：同樣的雷，"
                     "第二次是自己避開的。",
    },
]

SLIDES_EP28 = [
    {
        "kind": "cover",
        "title": "一個滑桿的工程",
        "lines": ["從實戰學 C++ 第二十八集",
                  "F-2 UI scale · 絕對重建 · 單一守衛 · 延期的格式"],
        "narration": "第二十八集講一個 UI 縮放滑桿背後的工程。"
                     "聽起來只是把數字乘進去——實作卻是"
                     "絕對重建、單一守衛、錨點豁免三件事。"
                     "外加延期工作清單第一次寫出格式。",
    },
    {
        "tag": "設計 1",
        "title": "絕對重建，不比值累乘",
        "sections": [
            ("做法", "ApplyScaled 每次從基底 token 重來："
                    "先 Apply(s, id) 重灌主題，再 "
                    "ScaleAllSizes(scale) 整體縮放。"),
            ("細節", "不用「新 scale / 舊 scale」累乘——"
                    "累乘會漂移，換主題還會殘留上一套主題"
                    "的縮放痕跡。"),
            ("啟示", "可重建的狀態就不要增量修改——"
                    "同參數恆同結果，"
                    "是除錯時最值錢的性質。"),
        ],
        "code": 'void ApplyScaled(s, id, scale) {\n    Apply(s, id);          // 重灌基底\n    s.ScaleAllSizes(ClampScale(scale));\n}\n// 絕對重建：同參數恆同 style',
        "narration": "第一個設計：絕對重建，不比值累乘。"
                     "每次從基底主題重灌再整體縮放——"
                     "不用新舊比例累乘，因為累乘會漂移，"
                     "換主題還會殘留舊縮放。"
                     "可重建的狀態就不要增量修改："
                     "同參數恆同結果，是除錯時最值錢的性質。",
    },
    {
        "tag": "設計 2",
        "title": "ClampScale 是單一守衛",
        "sections": [
            ("做法", "範圍 0.75 到 1.5 只有一個 "
                    "ClampScale——滑桿即時預覽和存檔載入"
                    "共用同一道閘。"),
            ("細節", "讀檔路徑在 Save 前也過 ClampScale——"
                    "壞值不管從 UI 還是手改設定檔進來，"
                    "都被同一個函式攔。"),
            ("啟示", "範圍守衛只能有一個——"
                    "入口各自 clamp 是範圍漂移的開始，"
                    "守衛收斂到一處才是一個規格。"),
        ],
        "code": 'inline float ClampScale(float s) {\n    return clamp(s, 0.75f, 1.5f);\n}\n// 滑桿、讀檔、Save 共用一道閘',
        "narration": "第二個設計：收斂到一個守衛。"
                     "零點七五到一點五只有一個鉗制函式——"
                     "滑桿、讀檔、存檔共用同一道閘。"
                     "入口各自攔是範圍漂移的開始；"
                     "守衛收斂到一處，才是一個規格。",
    },
    {
        "tag": "設計 3",
        "title": "Px 閘門與錨點豁免",
        "sections": [
            ("做法", "寫死尺寸一律走 Px(base, scale)；"
                    "但錨點計算豁免——右緣面板用 "
                    "ww - Px(w+8, s) 保持貼齊（UX-DR4）。"),
            ("細節", "縮放的是「東西多大」，不是「東西在哪」——"
                    "面板放大兩倍還是要貼右緣，"
                    "座標算式裡只有尺寸項過閘。"),
            ("啟示", "全域變換要標豁免——"
                    "「所有數字都乘」的規則一定傷到"
                    "幾個不該乘的位置，豁免要寫進慣例。"),
        ],
        "code": 'Px(w + 8, s)      // 尺寸過閘\nww - Px(w + 8, s) // 錨點算式內仍貼齊\n// 縮放尺寸，不縮放位置',
        "narration": "第三個設計：縮放閘門配錨點豁免。"
                     "寫死尺寸一律過 Px 函式，"
                     "但錨點算式豁免——右緣面板放大後仍要貼齊。"
                     "縮放的是東西多大、不是東西在哪。"
                     "全域變換一定要標豁免，"
                     "而且要寫進慣例不是留在腦中。",
    },
    {
        "tag": "設計 4",
        "title": "設定檔也是一份 schema",
        "sections": [
            ("做法", "potato.settings/1：theme 存 int 序數、"
                    "uiScale 過 ClampScale——缺檔給預設、"
                    "壞檔拒絕不動現況。"),
            ("細節", "寫入沿用 tmp+rename 原子替換、"
                    "先 create_directories——與 "
                    "CampaignState 同規，不另造輪子。"),
            ("啟示", "連「一個小小的設定檔」都進 schema "
                    "紀律——格式版本標籤和原子寫入"
                    "不是大檔案的專利。"),
        ],
        "code": 'schema: "potato.settings/1"\ntheme: int 序數  uiScale: 鉗制後\n// tmp+rename + 缺檔預設 + 壞檔拒絕',
        "narration": "第四個設計：設定檔也是一份 schema。"
                     "主題存序數、縮放先鉗制；"
                     "缺檔給預設、壞檔拒絕不動現況；"
                     "寫入沿用 tmp 加改名原子替換——"
                     "跟戰役存檔同規。"
                     "版本標籤和原子寫入，不是大檔案的專利。",
    },
    {
        "tag": "設計 5",
        "title": "延期的格式",
        "sections": [
            ("做法", "deferred-work.md 新增結構化條目："
                    "source_spec + summary + evidence——"
                    "每筆延期寫明哪份規格、為何延、"
                    "誰的證據。"),
            ("細節", "D-2 審查輪留下兩筆：team==0 寫死"
                    "橫跨三站、Bind 重綁會二次入帳 favor——"
                    "都附「哪個審查 lens 抓的」。"),
            ("啟示", "「先不修」也要有文件形狀——"
                    "沒有格式的延期會變成失憶，"
                    "有格式的延期是可以醒來的沉睡。"),
        ],
        "code": '- source_spec: spec-d2-...\n  summary: team==0 跨三站寫死\n  evidence: Blind-hunter review\n# 延期有編號、有理由、有出處',
        "narration": "第五個設計：延期的格式。"
                     "延期清單新增結構化條目——"
                     "來源規格、摘要、證據，"
                     "還標明是哪個審查鏡頭抓到的。"
                     "先不修也要有文件形狀："
                     "沒格式的延期是失憶，"
                     "有格式的延期是可以醒來的沉睡。",
    },
    {
        "tag": "設計 6",
        "title": "跨站的問題不逐站修",
        "sections": [
            ("做法", "team==0 當玩家寫死在 ShrineField、"
                    "GovernanceField、BattleController 三處——"
                    "審查記下來但不在 D-2 修。"),
            ("細節", "延期理由寫得很精準：這是慣例級限制，"
                    "需要的是「三站共用的玩家方參數」，"
                    "不是單站補丁——補了反而三處分叉。"),
            ("啟示", "認出「這是架構題不是 bug」也是功力——"
                    "逐站修慣例級問題，"
                    "修完得到三份不同的慣例。"),
        ],
        "code": '// ShrineField / GovernanceField /\n// BattleController 皆寫死 team==0\n// → 要的是共用參數，不是逐站補丁',
        "narration": "第六個設計：跨站的問題不逐站修。"
                     "玩家方寫死在三個站點——"
                     "審查記下來但不在本功能修。"
                     "理由很精準：這是慣例級限制，"
                     "要的是三站共用參數，不是單站補丁——"
                     "逐站修完只會得到三份不同的慣例。",
    },
    {
        "tag": "設計 7",
        "title": "白名單降到檔案級",
        "sections": [
            ("做法", "同步腳本的 AI/ 目錄改成檔案級白名單："
                    "只有 NeuralNetwork.h/.cpp 歸引擎，"
                    "AI*/RAG/RL 全歸遊戲側。"),
            ("細節", "MODULES 目錄級 → AI_FILES 檔案級——"
                    "同步粒度跟著所有權邊界走，"
                    "目錄不再是邊界的單位。"),
            ("啟示", "混合目錄是拆分後的常態——"
                    "白名單要能降粒度，"
                    "所有權切到哪一層清單就下到哪一層。"),
        ],
        "code": 'MODULES="Audio Core ... Media ..."\nAI_FILES="NeuralNetwork.h NeuralNetwork.cpp"\n# 混合目錄 → 檔案級白名單',
        "narration": "第七個設計：白名單降到檔案級。"
                     "AI 目錄是混合產權——只有神經網路歸引擎，"
                     "其餘全歸遊戲側。"
                     "同步從目錄級降到檔案級，"
                     "粒度跟著所有權邊界走。"
                     "混合目錄是拆分後的常態，"
                     "清單要能下到邊界所在的層。",
    },
    {
        "tag": "設計 8",
        "title": "審查鏡頭有名字",
        "sections": [
            ("做法", "延期條目的 evidence 欄標明發現者："
                    "「Blind-hunter review」、"
                    "「Edge-case-hunter review」——"
                    "審查鏡頭是具名角色。"),
            ("細節", "盲獵人抓隱性假設（team==0）、"
                    "邊界獵人抓重綁二次入帳——"
                    "不同鏡頭看見不同類的雷。"),
            ("啟示", "審查分鏡頭讓覆蓋可審計——"
                    "事後能回答「這類雷是誰負責看的」，"
                    "漏看也能歸因。"),
        ],
        "code": 'evidence: Blind-hunter review    # 隱性假設\nevidence: Edge-case-hunter review # 邊界情況\n# 鏡頭具名 → 覆蓋可審計',
        "narration": "第八個設計：審查鏡頭有名字。"
                     "證據欄標明發現者——盲獵人抓隱性假設、"
                     "邊界獵人抓重綁二次入帳。"
                     "不同鏡頭看見不同類的雷，"
                     "覆蓋因此可審計："
                     "事後能回答這類雷是誰負責看的，"
                     "漏看也能歸因。",
    },
    {
        "kind": "cover",
        "title": "第二十八集清單",
        "lines": ["絕對重建 · 單一守衛 · 尺寸過閘錨點豁免",
                  "設定檔同規 schema · 延期有格式 · 跨站不逐站修",
                  "白名單降粒度 · 審查鏡頭具名"],
        "narration": "第二十八集收在這裡。一個滑桿的工程"
                     "是三道守衛：絕對重建、單一鉗制、"
                     "錨點豁免。而延期清單長出格式——"
                     "不修什麼，跟修什麼一樣有學問。",
    },
]

SLIDES_EP29 = [
    {
        "kind": "cover",
        "title": "給像素寫斷言",
        "lines": ["從實戰學 C++ 第二十九集",
                  "UIScaleTest · ImGui 進無頭測試 · 舊 bug 變斷言 · "
                  "審查產物再入檔"],
        "narration": "第二十九集講一件聽起來矛盾的事："
                     "無頭測試使用者介面。"
                     "UI 管線沒有螢幕怎麼測？"
                     "答案是：把 ImGui 本體編進測試，"
                     "斷言的不是畫面，是 style 結構裡的數字。",
    },
    {
        "tag": "設計 1",
        "title": "把 ImGui 編進測試",
        "sections": [
            ("做法", "UIScaleTest 的 target 直接編譯四個 "
                    "imgui TU（imgui/imgui_draw/imgui_tables/"
                    "imgui_widgets）——測試吃真實的 "
                    "ImGuiStyle 結構。"),
            ("細節", "不需要 renderer：style 的縮放運算是"
                    "純數學——ScaleAllSizes 不碰 GPU，"
                    "無頭照跑。"),
            ("啟示", "「UI 程式碼不能無頭測」常常只是"
                    "還沒切對邊——樣式資料和渲染器"
                    "分開，前者就是普通數值。"),
        ],
        "code": 'add_executable(UIScaleTest\n    Examples/UIScaleTest.cpp\n    external/imgui/imgui.cpp\n    external/imgui/imgui_draw.cpp ...)\n# 測試目標自帶依賴 TU',
        "narration": "第一個設計：把 ImGui 本體編進測試。"
                     "測試目標直接編譯四個 imgui 源檔——"
                     "吃的是真實的 style 結構，不是替身。"
                     "縮放運算是純數學不碰顯卡，無頭照跑。"
                     "介面不能無頭測，常常只是還沒切對邊——"
                     "樣式資料和渲染器分開，前者就是普通數值。",
    },
    {
        "tag": "設計 2",
        "title": "建 context 不開 frame",
        "sections": [
            ("做法", "ImGui::CreateContext() 後直接操作 "
                    "ImGuiStyle——不 BeginFrame、不 Render，"
                    "測試活在幀循環之外。"),
            ("細節", "註解寫得很誠實：「ImGuiStyle 方法不需 "
                    "frame，但建 context 保險」——"
                    "依賴沒查清就先建，比假設安全。"),
            ("啟示", "函式庫的最小可測面要試出來——"
                    "哪些 API 需要完整生命週期、"
                    "哪些只要 context 存在，是分層知識。"),
        ],
        "code": 'ImGui::CreateContext();\n// 不 BeginFrame / 不 Render\n// style 運算活在幀循環之外',
        "narration": "第二個設計：建上下文不開幀。"
                     "建完 context 直接操作樣式結構——"
                     "不進幀循環、不渲染。"
                     "註解寫得誠實：方法不需幀，但建上下文保險。"
                     "函式庫的最小可測面要試出來——"
                     "哪些介面需要完整生命週期，"
                     "哪些只要上下文存在。",
    },
    {
        "tag": "設計 3",
        "title": "舊 bug 直接寫成斷言",
        "sections": [
            ("做法", "測試項明寫「換主題+scale=1 → 完全回基底，"
                    "U-1 粗版 bug 對照」——上一版的比值累乘"
                    "殘留 bug 變成永久回歸斷言。"),
            ("細節", "斷言的是失敗模式本身：切主題後"
                    "WindowPadding 必須逐位回到新基底——"
                    "殘留舊縮放就亮紅燈。"),
            ("啟示", "修過的 bug 要留斷言在測試裡——"
                    "修正是修好當下，斷言是修好永遠。"),
        ],
        "code": 'ApplyScaled(s, TacticalSim, 1.0f);\nCheck(s == base(TacticalSim),\n      "換主題+scale=1 → 完全回基底");\n// U-1 粗版 bug 的永久回歸斷言',
        "narration": "第三個設計：舊 bug 直接寫成斷言。"
                     "換主題配一倍縮放必須完全回到基底——"
                     "上一版比值累乘的殘留蟲，"
                     "變成永久的回歸斷言。"
                     "修正是修好當下，斷言是修好永遠。",
    },
    {
        "tag": "設計 4",
        "title": "斷言乘法律不斷言感覺",
        "sections": [
            ("做法", "逐欄位驗證：scaled.WindowPadding.x "
                    "== base × 1.5——斷言的是數值同一律，"
                    "不是「看起來差不多」。"),
            ("細節", "Nearly() 容差萬分之一——浮點斷言"
                    "帶明確 tolerance，不是 == 也不是"
                    "肉眼看截圖。"),
            ("啟示", "UI 測試的誠實路線是數值——"
                    "截圖比對脆弱且難維護，"
                    "欄位級斷言便宜又精確。"),
        ],
        "code": 'Check(Nearly(scaled.WindowPadding.x,\n             base.WindowPadding.x * 1.5f),\n      "WindowPadding = 基底×1.5");',
        "narration": "第四個設計：斷言乘法律，不斷言感覺。"
                     "逐欄位驗證縮放後等於基底乘一點五——"
                     "斷的是數值同一律不是看起來差不多。"
                     "容差萬分之一的浮點比較，"
                     "比截圖比對便宜又精確。"
                     "介面測試的誠實路線是數值。",
    },
    {
        "tag": "設計 5",
        "title": "閘門值也要測",
        "sections": [
            ("做法", "ClampScale 三點斷言：0.5→0.75、"
                    "2.0→1.5、1.0 不動；Px(300, 9.9) 也鉗"
                    "到 450——閘門的邊界就是測試點。"),
            ("細節", "Px 的「越界也鉗」是複合斷言——"
                    "證明閘門在函式內部，"
                    "不靠呼叫端自律。"),
            ("啟示", "鉗制函式的測試點是邊界三態："
                    "下界之上、上界之下、中間不動——"
                    "三點定一條鉗制線。"),
        ],
        "code": 'ClampScale(0.5)→0.75  ClampScale(2.0)→1.5\nClampScale(1.0)→1.0（不動）\nPx(300, 9.9)=450 // 閘門在函式內',
        "narration": "第五個設計：閘門值也要測。"
                     "鉗制三點斷言：越下界收斂、越上界收斂、"
                     "中間值不動；連 Px 的九點九倍都鉗回上限。"
                     "閘門在函式內部不靠呼叫端自律——"
                     "三點定一條鉗制線。",
    },
    {
        "tag": "設計 6",
        "title": "設定檔的四格矩陣",
        "sections": [
            ("做法", "UISettings 測試照 I/O 矩陣走："
                    "roundtrip 逐值相符、越界值存前鉗制、"
                    "缺檔回預設不報錯、壞檔拒絕不動現況。"),
            ("細節", "測完自己刪暫存檔——"
                    "寫檔測試不留 artifacts，"
                    "平行跑不互撞。"),
            ("啟示", "持久化的測試是四格：存取一致、"
                    "輸入鉗制、缺檔降級、壞檔拒絕——"
                    "每格對應一種現實會發生的事。"),
        ],
        "code": 'Save→Load 逐值相符 / 越界值存前鉗制\n缺檔→預設不報錯 / 壞檔→拒絕不動現況\n// 測完刪暫存，平行不互撞',
        "narration": "第六個設計：設定檔的四格矩陣。"
                     "存取逐值相符、越界值存檔前鉗制、"
                     "缺檔回預設、壞檔拒絕——"
                     "四格各對應一種現實會發生的事。"
                     "測完自己刪暫存檔，平行跑不互撞。",
    },
    {
        "tag": "設計 7",
        "title": "審查產物變慣例",
        "sections": [
            ("做法", "d2-review.diff 入檔——繼 d1 之後第二份"
                    "審查 diff 存進 implementation-artifacts，"
                    "review 留檔從特例變慣例。"),
            ("細節", "留檔的價值在可比性：兩份 diff 並排"
                    "看得出審查抓的雷類是否在收斂——"
                    "組織學習的量化證據。"),
            ("啟示", "做一次是行為、做兩次是流程——"
                    "慣例的形成不需要宣佈，"
                    "重複發生就夠了。"),
        ],
        "code": '_bmad-output/\n  d1-review.diff\n  d2-review.diff   # 第二份：慣例成形\n# 審查留檔從特例變流程',
        "narration": "第七個設計：審查產物變慣例。"
                     "第二份審查 diff 入檔——繼上次之後，"
                     "留檔從特例變流程。"
                     "兩份並排看得出抓到的雷類是否收斂，"
                     "是組織學習的量化證據。"
                     "做一次是行為、做兩次是流程——"
                     "慣例不需要宣佈，重複發生就夠了。",
    },
    {
        "tag": "設計 8",
        "title": "spec 先行到 UI",
        "sections": [
            ("做法", "spec-f2-ui-scale-pipeline.md 落地——"
                    "連一個縮放滑桿都先走 spec 流程："
                    "凍結區塊、I/O 矩陣、驗收條件。"),
            ("細節", "F 系列接續 D/E 的節奏：spec→實作→"
                    "測試→review→done，UI 功能不再例外。"),
            ("啟示", "流程的成熟度看「最不值得走流程的事」"
                    "還走不走——滑桿也走 spec，"
                    "表示流程已是預設值不是儀式。"),
        ],
        "code": '# spec-f2-ui-scale-pipeline.md\n# 連滑桿都走 spec 流程\n# → 流程是預設值，不是儀式',
        "narration": "第八個設計：規格先行到介面層。"
                     "連一個縮放滑桿都先走完整流程——"
                     "凍結區塊、輸出入矩陣、驗收條件。"
                     "流程的成熟度，看最不值得走流程的事還走不走。"
                     "滑桿也走規格，表示流程已是預設值，"
                     "不是做給誰看的儀式。",
    },
    {
        "kind": "cover",
        "title": "第二十九集清單",
        "lines": ["ImGui 進無頭測試 · context 不開 frame",
                  "舊 bug 變斷言 · 乘法律斷言 · 閘門三點",
                  "設定四格 · 審查留檔成慣例 · 滑桿走 spec"],
        "narration": "第二十九集收在這裡。無頭測介面的關鍵"
                     "是切對邊：樣式資料是數值、渲染器才是顯卡。"
                     "而 F 系列的第一支 epic 證明，"
                     "流程已經是預設值。",
    },
]

SLIDES_EP30 = [
    {
        "kind": "cover",
        "title": "新 repo 的第一口呼吸",
        "lines": ["從實戰學 C++ 第三十集",
                  "PotatoEngine 獨立 · NeuralArtDemo · 權重不入 git · "
                  "邊界寫進自己的 AGENTS"],
        "narration": "第三十集是個里程碑：拆出來的引擎 repo "
                     "有了第一個專屬作品。"
                     "NeuralArtDemo——載入預訓權重、"
                     "六個神經圖形模型各跑一次推論。"
                     "拆倉至此不再是文件，是日常。",
    },
    {
        "tag": "設計 1",
        "title": "新功能長在新 repo",
        "sections": [
            ("做法", "NeuralArtDemo 直接加在 PotatoEngine "
                    "repo 的 Examples/——拆倉後第一個"
                    "「只屬於引擎」的新增。"),
            ("細節", "遊戲 repo 永遠看不到這支 demo——"
                    "引擎有了自己的展示層，"
                    "不再借用遊戲的 Examples 目錄。"),
            ("啟示", "repo 拆分真正的完成指標是這個："
                    "新工作自然地發生在新家，"
                    "而不是繼續寄生在舊地址。"),
        ],
        "code": '# PotatoEngine/Examples/NeuralArtDemo.cpp\n# 遊戲 repo 看不見的第一個作品\n# 拆分完成的真指標：新工作長在新家',
        "narration": "第一個設計：新功能長在新倉庫。"
                     "示範程式直接加在引擎 repo——"
                     "遊戲側永遠看不到這支 demo，"
                     "引擎有了自己的展示層。"
                     "拆分真正的完成指標是："
                     "新工作自然發生在新家，"
                     "不再寄生舊地址。",
    },
    {
        "tag": "設計 2",
        "title": "權重是本機產物不入 git",
        "sections": [
            ("做法", "assets/neural/ 在 .gitignore 內——"
                    ".pnn 權重由 NeuralArtTool 本機重訓產生，"
                    "repo 只留能產生權重的工具。"),
            ("細節", "二進位資產入庫的代價：歷史膨脹、"
                    "diff 無意義、clone 變重——"
                    "權重是可再生的，工具才是資產。"),
            ("啟示", "「可再生產物不入版本控制」——"
                    "入庫的是生成能力和配方，"
                    "不是生成結果。"),
        ],
        "code": '# assets/neural/ → .gitignore\n# 權重可再生 → 不入庫\n# repo 留的是產生權重的工具',
        "narration": "第二個設計：權重是本機產物，不入版本庫。"
                     "神經網路權重由工具本機重訓產生，"
                     "倉庫只留能產生權重的工具。"
                     "二進位資產入庫讓歷史膨脹、差異無意義——"
                     "可再生產物不入版本控制，"
                     "入庫的是生成能力不是生成結果。",
    },
    {
        "tag": "設計 3",
        "title": "自給自足的測試輸入",
        "sections": [
            ("做法", "MakeTestImage 程序化產生 32×32 測試圖："
                    "對角漸層加色彩帶——demo 不依賴任何"
                    "外部素材檔。"),
            ("細節", "連 colorize 的灰階輸入都是現場算 luma "
                    "得到的——輸入全鏈路可在任何乾淨"
                    "checkout 上重建。"),
            ("啟示", "demo 的素材依賴要歸零——"
                    "clone 下來就能跑，"
                    "是範例程式的最低體面。"),
        ],
        "code": 'FImage MakeTestImage(w, h) {\n    // 對角漸層 + 色彩帶\n    // 自給自足，不需外部素材\n}',
        "narration": "第三個設計：自給自足的測試輸入。"
                     "三十二乘三十二的測試圖程序化產生，"
                     "連上色的灰階輸入都是現場算亮度得到。"
                     "示範的素材依賴歸零——"
                     "clone 下來就能跑，"
                     "是範例程式的最低體面。",
    },
    {
        "tag": "設計 4",
        "title": "鏈式推論即整合煙囪",
        "sections": [
            ("做法", "地形模型的高度場→HeightmapToMesh 轉網格、"
                    "同一份 heights 再餵法線貼圖模型——"
                    "六個模型裡有三個串成管線。"),
            ("細節", "demo 順便驗證了模型間的資料契約："
                    "高度場的格式剛好是法線模型的輸入——"
                    "能串起來本身就是斷言。"),
            ("啟示", "示範程式偷渡整合測試——"
                    "「跑完六張圖」其實驗證了六個模型"
                    "加一條資料管線。"),
        ],
        "code": 'tg.GenerateHeightmap(...) → heights\n  ├→ HeightmapToMesh(heights) → GridMesh\n  └→ nm.Apply(heights, 64, 64) → 法線圖\n// 能串起來本身就是斷言',
        "narration": "第四個設計：鏈式推論是隱形整合測試。"
                     "地形的高度場轉網格、再餵給法線模型——"
                     "六個模型裡三個串成管線。"
                     "能串起來本身就驗證了資料契約："
                     "上游的輸出格式剛好是下游的輸入。"
                     "示範程式偷渡整合測試。",
    },
    {
        "tag": "設計 5",
        "title": "latent 全零的語義",
        "sections": [
            ("做法", "生成類模型餵 latent 全零向量——"
                    "INR 的慣例：零點是「重現訓練圖」的"
                    "錨點，demo 因此有確定性輸出。"),
            ("細節", "隨機 latent 的 demo 每次長不同——"
                    "全零讓 demo 可目視比對："
                    "這次跑出來跟上次是不是同一張。"),
            ("啟示", "生成模型的 demo 要挑確定性輸入——"
                    "「每次不同」的示範無法當回歸。"),
        ],
        "code": 'gen.Generate(zeros(LatentDim), 64, 64);\n// latent=0 = 重現訓練圖的錨點\n// demo 因此可目視回歸',
        "narration": "第五個設計：潛向量全零的語義。"
                     "生成模型餵零向量——零點是重現訓練圖的錨點，"
                     "示範因此有確定性輸出可目視比對。"
                     "隨機潛向量的示範每次長不同，無法當回歸。"
                     "生成模型的 demo，要挑確定性輸入。",
    },
    {
        "tag": "設計 6",
        "title": "失敗回非零的老規矩",
        "sections": [
            ("做法", "任一模型載入或推論失敗就累計 "
                    "g_failures，結尾回非零——demo 同時是"
                    "可進 CI 的煙囪測試。"),
            ("細節", "權重是 gitignore 的本機檔——"
                    "在沒有權重的環境會 FAIL 而非 SKIP，"
                    "刻意讓「缺權重」可見。"),
            ("啟示", "demo 和測試的差別只在輸出——"
                    "回傳碼對齊測試慣例，"
                    "示範程式就自動獲得 CI 資格。"),
        ],
        "code": 'Check(LoadNetFromFile(...), "載入 h3_sr2x.pnn");\n...\nreturn g_failures == 0 ? 0 : 1;\n// 回傳碼 = CI 資格',
        "narration": "第六個設計：失敗回非零的老規矩。"
                     "任一模型失敗就計數、結尾回非零——"
                     "示範程式同時是可進持續整合的煙囪測試。"
                     "缺權重時報失敗而非跳過，"
                     "刻意讓環境問題可見。"
                     "示範和測試的差別只在輸出——"
                     "回傳碼對齊，就自動獲得資格。",
    },
    {
        "tag": "設計 7",
        "title": "邊界寫進自己的 AGENTS",
        "sections": [
            ("做法", "新 repo 的 AGENTS.md 自載邊界規則："
                    "不得引用 Gameplay/Campaign 標頭、"
                    "standalone 守衛、glfw 要 GLOBAL imported。"),
            ("細節", "連 GLFW_INCLUDE_NONE 的傳染問題都"
                    "寫成條文——IDE_GUI 為何直編源碼不連 "
                    "lib，理由永久入檔（當年的謎底）。"),
            ("啟示", "新 repo 的第一份文件要把「為什麼」"
                    "寫死——邊界規則沒入檔，"
                    "三個月後就只剩「不知道為什麼」。"),
        ],
        "code": '# PotatoEngine/AGENTS.md\n# - 不得引用上層標頭\n# - glfw 必須 GLOBAL imported\n# - GLFW_INCLUDE_NONE 傳染條款',
        "narration": "第七個設計：邊界寫進自己的交接文件。"
                     "新倉庫的規則檔自載邊界——不得引用上層標頭、"
                     "守衛條款、連當年 include 傳染的謎底都入檔。"
                     "新家的第一份文件要把為什麼寫死——"
                     "規則沒入檔，三個月後只剩不知道為什麼。",
    },
    {
        "tag": "設計 8",
        "title": "兩個 repo 兩條河",
        "sections": [
            ("做法", "引擎 repo 有自己的 git 歷史"
                    "（filter-repo 保留）、自己的佇列節奏、"
                    "自己的建置驗證（28 測試雙工具鏈）。"),
            ("細節", "遊戲側繼續走 spec/queue/review 循環——"
                    "兩個 repo 各自心跳，"
                    "靠 add_subdirectory 在編譯期見面。"),
            ("啟示", "拆分的終態不是「一個變兩個」——"
                    "是兩個各自完整的節奏，"
                    "只在介面上相遇。"),
        ],
        "code": '# PotatoEngine: 自己的歷史/AGENTS/28 tests\n# MingGoRTS  : spec→review→done 循環\n# 見面點：add_subdirectory（編譯期）',
        "narration": "第八個設計：兩個倉庫兩條河。"
                     "引擎有自己的歷史、文件、二十八支測試；"
                     "遊戲繼續走規格審查循環。"
                     "拆分的終態不是一個變兩個——"
                     "是兩個各自完整的節奏，"
                     "只在編譯期的介面上相遇。",
    },
    {
        "kind": "cover",
        "title": "第三十集清單",
        "lines": ["新工作長在新家 · 權重可再生不入庫 · 素材依賴歸零",
                  "鏈式推論偷渡整合測試 · latent 全零錨點 · 回傳碼=CI",
                  "邊界入檔 · 兩個 repo 各自心跳"],
        "narration": "第三十集收在這裡。拆倉線從第六集的"
                     "策略、第十四集的演習、第二十三集的手術，"
                     "走到今天的第一口呼吸——"
                     "引擎 repo 有了自己的作品和規則，"
                     "拆分完成了。",
    },
]

SLIDES_EP31 = [
    {
        "kind": "cover",
        "title": "天命的匯率",
        "lines": ["從實戰學 C++ 第三十一集",
                  "D-3 FateExchange · 灰顯價目 · 零副作用拒絕 · "
                  "型別化回呼"],
        "narration": "第三十一集，神話雙層的經濟環路收口。"
                     "D-2 神社產出 favor、D-3 讓 favor 兌換成"
                     "民心與秩序——神明的香火錢，"
                     "終於能換成人間的治理資本。",
    },
    {
        "tag": "設計 1",
        "title": "D 系列的經濟閉環",
        "sections": [
            ("做法", "FateExchange 把 MythLayer 的 favor "
                    "兌換成 Governance 的民心/秩序——"
                    "D-1 產生壓力、D-2 產生 favor、"
                    "D-3 讓 favor 變現。"),
            ("細節", "favor 已是持久資源（myth_layer 段入檔）——"
                    "兌換消耗的是「和神明的關係」本身，"
                    "不是戰鬥資源。"),
            ("啟示", "系統串成經濟體的標誌：上游的產出"
                    "剛好是下游的貨幣——中間沒有"
                    "為兌換而生的新資源。"),
        ],
        "code": '// 神社選擇 → favor（D-2）\n// favor ──FateExchange──→ 民心/秩序（D-3）\n// 中間沒有新造貨幣',
        "narration": "第一個設計：系列經濟閉環。"
                     "兌換器把滲透層的好感換成治理軸——"
                     "神社產出好感、好感變現成民心秩序。"
                     "消耗的是和神明的關係本身，不是戰鬥資源。"
                     "經濟體成形的標誌：上游產出剛好是下游貨幣，"
                     "中間沒有為兌換新造的資源。",
    },
    {
        "tag": "設計 2",
        "title": "灰顯價目，慣例互引",
        "sections": [
            ("做法", "Options() 永遠列出全部價目，買不起的"
                    "附 affordable=false + reason——"
                    "註解明寫「照 NoBattleResolver::"
                    "CheckOptions 慣例」。"),
            ("細節", "慣例第一次被具名引用——"
                    "「灰顯不隱藏」從單一實作的風格，"
                    "升級成被跨類援引的專案慣例。"),
            ("啟示", "註解裡寫慣例出處是知識管理的成人禮——"
                    "後人知道該去哪裡看先例，"
                    "也知道偏離要給理由。"),
        ],
        "code": '// 「照 NoBattleResolver::CheckOptions 慣例」\nOptions() → 全價目 + affordable + reason\n// 門檻可見是設計本體',
        "narration": "第二個設計：灰顯價目，慣例互引。"
                     "價目永遠全列、買不起的附原因——"
                     "註解明寫照無戰判定器的慣例。"
                     "慣例第一次被具名引用，"
                     "從單一實作的風格升級成專案共識。"
                     "寫出處讓後人知道去哪看先例，"
                     "也知道偏離要給理由。",
    },
    {
        "tag": "設計 3",
        "title": "零副作用的驗證序",
        "sections": [
            ("做法", "Convert 的四道拒絕全排在變異之前："
                    "未綁定→額度盡→favor 不足→非法 enum——"
                    "任何一關不過都「什麼都沒發生」。"),
            ("細節", "enum 驗證特意排在最後一道、緊貼變異——"
                    "註解寫「先驗型別再放行，保零副作用」："
                    "所有前置條件清了才輪到型別把關。"),
            ("啟示", "拒絕序即不變量——檢查和變異之間"
                    "不能夾任何可能失敗的操作，"
                    "否則留下半套副作用。"),
        ],
        "code": 'if (!myths || !gov || s.empty()) return false;\nif (inBattle && uses >= kMax) return false;\nif (Favor(s) < kFateCost)    return false;\nif (非法 enum)               return false;\n// 全過才變異——拒絕零副作用',
        "narration": "第三個設計：零副作用的驗證序。"
                     "四道拒絕全排在變異之前——"
                     "未綁定、額度盡、好感不足、非法枚舉，"
                     "任一不過就什麼都沒發生。"
                     "檢查和變異之間不能夾會失敗的操作，"
                     "否則留下半套副作用。",
    },
    {
        "tag": "設計 4",
        "title": "同名軸寫對那一層",
        "sections": [
            ("做法", "兌換一律寫 Campaign::Governance——"
                    "註解明寫「戰中 SageCommand 同名軸不碰」："
                    "民心/秩序在兩層各有一條，只寫帳本真相層。"),
            ("細節", "戰鬥內的同名軸是即時演出用的影子——"
                    "持久資產的兌換若寫進影子，"
                    "章節結算時就蒸發了。"),
            ("啟示", "同名不同層的欄位要註解劃界——"
                    "「寫哪一個」是架構決定，"
                    "不是命名撞車。"),
        ],
        "code": 'gov->AdjustPopularSupport(kFateGain);\n// 寫 Governance（帳本真相層）\n// 不碰 SageCommand 同名軸（影子）',
        "narration": "第四個設計：同名軸寫對那一層。"
                     "民心和秩序在戰鬥層、戰役層各有一條——"
                     "兌換只寫帳本真相層的治理器，"
                     "不碰戰鬥內的影子軸。"
                     "寫進影子的持久資產，章節結算就蒸發。"
                     "同名不同層要註解劃界。",
    },
    {
        "tag": "設計 5",
        "title": "戰中限次，章界不限",
        "sections": [
            ("做法", "每場戰限 kMaxBattleUses=2 次——"
                    "battleUses 是執行期計數不入存檔；"
                    "章節邊界的 Convert(inBattle=false) "
                    "只受 favor 餘額。"),
            ("細節", "額度計數刻意不持久——存檔裡沒有"
                    "「本場已用幾次」欄位，重開戰鬥"
                    "由呼叫端 ResetBattleUses()。"),
            ("啟示", "同一個操作在不同情境配不同限額——"
                    "限制寫在呼叫端驅動的計數器，"
                    "不在持久狀態。"),
        ],
        "code": 'kMaxBattleUses = 2;   // 戰中限兩次\nConvert(spirit, t, inBattle=false)\n// 章界不限次，只受 favor 餘額\n// 計數器不入存檔——重開由呼叫端 reset',
        "narration": "第五個設計：戰中限次，章界不限。"
                     "戰鬥內限兩次、計數器是執行期狀態不入存檔；"
                     "章節邊界的兌換只受好感餘額。"
                     "同一個操作在不同情境配不同限額——"
                     "限制寫在呼叫端驅動的計數器，"
                     "不在持久狀態裡。",
    },
    {
        "tag": "設計 6",
        "title": "回呼從字串升級成結構",
        "sections": [
            ("做法", "onEvent 攜帶 FateEvent 結構"
                    "（spirit/target/cost/gain/inBattle）——"
                    "不再是 ShrineField 那種格式化字串。"),
            ("細節", "呼叫端拿到結構化資料自己決定去向："
                    "戰中接 recorder.AddRecord、"
                    "章界接 LedgerChain::Append("
                    "EntrySource::Myth)。"),
            ("啟示", "回呼酬載的成熟度光譜：字串→結構——"
                    "下游需要機器可讀欄位時，"
                    "就該從「事件訊息」升級「事件物件」。"),
        ],
        "code": 'struct FateEvent { spirit, target,\n    cost, gain, inBattle };\n// 呼叫端自己決定：recorder or LedgerChain',
        "narration": "第六個設計：回呼從字串升級成結構。"
                     "事件攜帶型別化結構——神明、標的、"
                     "成本、收益、是否戰中，五個欄位。"
                     "呼叫端拿到結構化資料自己決定去向："
                     "戰中進記錄器、章界進帳本鏈。"
                     "下游需要機器可讀欄位時，"
                     "事件訊息就該升級成事件物件。",
    },
    {
        "tag": "設計 7",
        "title": "先變異後派出，第三次",
        "sections": [
            ("做法", "扣 favor、加治理軸、計額度全部完成後"
                    "才發 onEvent——註解寫「先變異後派出，"
                    "回呼可安全再入本層」。"),
            ("細節", "D-1 審查的 collect-then-dispatch、"
                    "D-2 的先結算再回呼——同一條紀律"
                    "第三次出現，已成起手式。"),
            ("啟示", "紀律成熟的標誌是變成背景知識——"
                    "不再被審查抓、不再需要想起，"
                    "寫的時候手就這麼放。"),
        ],
        "code": 'myths->AdjustFavor(spirit, -kFateCost);\ngov->Adjust...(kFateGain);\n++battleUses;\nonEvent({...}); // 變異完才派出',
        "narration": "第七個設計：先變異後派出，第三次。"
                     "扣好感、加治理、計額度全部完成才發事件——"
                     "從上次審查抓出來的雷，"
                     "到這次寫作當下的起手式。"
                     "紀律成熟的標誌是變成背景知識："
                     "不再需要想起，手就這麼放。",
    },
    {
        "tag": "設計 8",
        "title": "價目寫死但說清楚",
        "sections": [
            ("做法", "kFateCost=20 / kFateGain=5 / "
                    "kMaxBattleUses=2 是類內 constexpr——"
                    "註解坦白「寫死可調參」。"),
            ("細節", "不過早抽象成設定檔：價目表只有三個數，"
                    "constexpr 讓調平衡的人一眼找到、"
                    "編譯期可查所有引用點。"),
            ("啟示", "「寫死」標明「可調」是誠實標註——"
                    "承認這是平衡參數、指出調的位置，"
                    "比假裝已經資料驅動更健康。"),
        ],
        "code": 'static constexpr float kFateCost = 20.0f;\nstatic constexpr float kFateGain = 5.0f;\nstatic constexpr int   kMaxBattleUses = 2;\n// 「寫死可調參」——承認是平衡常數',
        "narration": "第八個設計：價目寫死但說清楚。"
                     "三個平衡常數是類內常數表達式，"
                     "註解坦白寫死可調參。"
                     "不過早抽象成設定檔——三個數的價目表，"
                     "常數讓調平衡的人一眼找到。"
                     "承認是平衡參數、指出調的位置，"
                     "比假裝資料驅動健康。",
    },
    {
        "kind": "cover",
        "title": "第三十一集清單",
        "lines": ["經濟閉環無新造貨幣 · 慣例被具名引用 · 驗證序零副作用",
                  "同名軸寫帳本層 · 情境配額 · 字串升級結構",
                  "紀律第三次 · 寫死可調參"],
        "narration": "第三十一集收在這裡。天命兌換器裡"
                     "沒有一個新發明——灰顯、零副作用、"
                     "先變異後派出全是慣例的重演。"
                     "成熟的程式庫不是沒有新功能，"
                     "是新功能全部由舊紀律長出來。",
    },
]

SLIDES_EP32 = [
    {
        "kind": "cover",
        "title": "規矩進了工具",
        "lines": ["從實戰學 C++ 第三十二集",
                  "IDE 建議引擎學會專案規範 · 路徑感知 · 誤報防線 · "
                  "AGENTS.md 變可執行"],
        "narration": "第三十二集講規矩的落地方式升級。"
                     "IDE 的建議引擎新增四條專案護欄——"
                     "我們在影片和文件裡教過的慣例，"
                     "現在寫程式當下就會被工具提醒。",
    },
    {
        "tag": "規則 1",
        "title": "R4：同一段碼，兩種判決",
        "sections": [
            ("做法", "引擎層檔案 include imgui 或呼叫 "
                    "ImGui:: → VeryHigh 違規；同樣的碼放在 "
                    "Examples/ 下完全合法。"),
            ("細節", "規則是路徑感知的：判的不是語法內容，"
                    "是「這段碼出現在哪一層」——"
                    "引擎保持無頭可測是架構級禁令。"),
            ("啟示", "lint 的最高級是懂架構——"
                    "不是「這個 API 不好」，"
                    "是「這個 API 不該出現在這一層」。"),
        ],
        "code": 'Rendering/Panel.cpp + ImGui:: → 違規\nExamples/demo.cpp  + ImGui:: → 合法\n// 判的是層級，不是語法',
        "narration": "第一條規則：同一段碼，兩種判決。"
                     "引擎層檔案碰 ImGui 是最高級違規，"
                     "範例目錄裡同樣的呼叫完全合法。"
                     "規則判的不是語法內容，"
                     "是這段碼出現在哪一層。"
                     "檢查工具的最高境界是懂架構。",
    },
    {
        "tag": "規則 2",
        "title": "R5：第十二集變成 lint",
        "sections": [
            ("做法", "rand() 與 uniform_*_distribution → "
                    "Nondeterministic RNG 建議——L-8 查證器"
                    "學過的跨標準庫教訓，變成寫碼當下的提醒。"),
            ("細節", "mt19937_64 加取模的寫法不觸發——"
                    "規則不只禁壞的，還認得好的替代長什麼樣。"),
            ("啟示", "文件教過的坑會被遺忘，"
                    "工具攔下的坑不會——"
                    "把 lesson 變成 lint 才是真的學會。"),
        ],
        "code": 'rand()              → 建議\nuniform_int_distribution → 建議\nmt19937_64 + %     → 放行（認得好的）',
        "narration": "第二條規則：查證器的教訓變成檢查項。"
                     "rand 和均勻分佈類觸發非確定性警告——"
                     "當年為了跨編譯器重現性避開的坑，"
                     "現在寫的當下就被攔。"
                     "而合法的替代寫法不會被誤傷。"
                     "文件教過的坑會被遺忘，"
                     "工具攔下的不會。",
    },
    {
        "tag": "規則 3",
        "title": "誤報是規則的天敵",
        "sections": [
            ("做法", "R5 用 \\b 詞邊界排除 srand 誤報；"
                    "掃描走 maskedText——註解和字串裡的"
                    "「rand()」不觸發。"),
            ("細節", "每條規則都配誤報測試：srand 不報、"
                    "字串裡的 ImGui:: 不報、呼叫 SaveToFile "
                    "的消費端不誤觸 R6。"),
            ("啟示", "lint 的第一品質是零誤報——"
                    "會亂叫的規則會被關掉，"
                    "關掉的規則等於沒有。"),
        ],
        "code": 'srand(42)      → 不報（\\b 邊界）\n"rand()" 在字串 → 不報（maskedText）\n// 會亂叫的規則會被關掉',
        "narration": "第三條規則：誤報是規則的天敵。"
                     "詞邊界排除 srand 的誤傷，"
                     "遮罩文本讓註解字串裡的字眼不觸發。"
                     "每條規則都配了誤報測試。"
                     "檢查的第一品質是零誤報——"
                     "會亂叫的規則會被關掉，"
                     "關掉的規則等於沒有。",
    },
    {
        "tag": "規則 4",
        "title": "R3/R6：形狀級規則",
        "sections": [
            ("做法", "header 無 pragma once/guard → "
                    "Missing include guard；objectValue 寫入"
                    "但沒有 schema 字串 → 持久化文件缺標籤。"),
            ("細節", "規則認得檔案型別：.cpp 不報 include "
                    "guard；也認得消費端——呼叫 "
                    "SaveToFile 的檔不誤報缺 schema。"),
            ("啟示", "形狀規則抓的是「忘了做」不是「做錯了」"
                    "——guard 和 schema 都是"
                    "寫得快時最容易漏的 boilerplate。"),
        ],
        "code": '.h 無 guard → 建議；.cpp 不報\nobjectValue[...] 無 schema → 建議\n// 抓「忘了做」，不抓「做錯了」',
        "narration": "第四條規則：形狀級的提醒。"
                     "標頭檔缺保護巨集、持久化文件缺架構標籤——"
                     "規則認得檔案型別也認得消費端。"
                     "這類規則抓的是忘了做，不是做錯了："
                     "保護巨集和標籤都是寫得快時"
                     "最容易漏的樣板。",
    },
    {
        "tag": "設計 1",
        "title": "AGENTS.md 變可執行",
        "sections": [
            ("做法", "四條規則各自對應一條文件慣例："
                    "headless 引擎、確定性 RNG、"
                    "header guard、schema 標籤——"
                    "文件的規範進了建議引擎。"),
            ("細節", "GenerateSuggestions 的測試覆寫在 "
                    "SuggestionRulesTest——規則本身"
                    "也是測過的程式碼。"),
            ("啟示", "規範的最終形態是可執行——"
                    "AGENTS.md 給讀的人看，"
                    "lint 給寫的當下用，兩者同源。"),
        ],
        "code": '# 文件慣例 → GetProjectRuleSuggestions\n# AGENTS.md 給讀的人看\n# lint 給寫的當下用——同源',
        "narration": "第一個設計：交接文件變成可執行檔。"
                     "四條規則各自對應一條寫過的慣例——"
                     "規範從紙面走進建議引擎。"
                     "文件給讀的人看，檢查給寫的當下用，"
                     "兩者同源。規範的最終形態是可執行。",
    },
    {
        "tag": "設計 2",
        "title": "新 repo 的 AGENTS 改口",
        "sections": [
            ("做法", "staging 遊戲 repo 的 AGENTS.md 更新："
                    "「本 repo 不含引擎源碼，一律在上游編輯，"
                    "用 POTATO_ENGINE_ROOT 消費」。"),
            ("細節", "同一份文件換了世界觀——"
                    "從「引擎在這裡」改口「引擎在隔壁」；"
                    "交接文件的措辭就是邊界的法律文本。"),
            ("啟示", "repo 身份的改變要落在第一句話——"
                    "讀文件的人最先看到的是"
                    "「這裡有什麼、這裡沒有什麼」。"),
        ],
        "code": '# game-src/AGENTS.md：\n# 「本 repo 不含引擎源碼」\n# 措辭 = 邊界的法律文本',
        "narration": "第二個設計：新倉庫的文件改口。"
                     "演習場的交接文件更新——"
                     "從引擎在這裡，改口成引擎在隔壁。"
                     "交接文件的措辭就是邊界的法律文本，"
                     "身份改變要落在第一句話。",
    },
    {
        "tag": "設計 3",
        "title": "第三份審查留檔",
        "sections": [
            ("做法", "d3-review.diff 入檔——D-1、D-2、D-3 "
                    "三份審查 diff 並排躺在 "
                    "implementation-artifacts。"),
            ("細節", "三份可以開始做趨勢分析：抓到的雷類"
                    "從「回呼再入」收斂到「慣例級限制」——"
                    "審查產出從修 bug 變成量測工具。"),
            ("啟示", "留檔的複利是可比性——"
                    "單份 diff 是修復記錄，"
                    "三份並排是組織學習曲線。"),
        ],
        "code": '_bmad-output/\n  d1-review.diff  d2-review.diff\n  d3-review.diff  # 第三份\n# 單份是修復，三份是曲線',
        "narration": "第三個設計：第三份審查留檔。"
                     "三份審查差異並排——"
                     "抓到的雷從回呼再入收斂到慣例級限制，"
                     "審查產出從修蟲變成量測工具。"
                     "留檔的複利是可比性："
                     "單份是修復記錄，三份並排是學習曲線。",
    },
    {
        "tag": "設計 4",
        "title": "工具鏈的自食期",
        "sections": [
            ("做法", "建議引擎吃的是自己 repo 的規範——"
                    "dogfooding 的第二層：不只用自己的引擎"
                    "蓋 IDE，IDE 還用自己的規矩檢查自己。"),
            ("細節", "R5 的判詞直接引用 L-8 的教訓文字——"
                    "「uniform_* 跨標準庫實作異果」"
                    "從研究筆記原句進了錯誤訊息。"),
            ("啟示", "工具的規則庫是專案歷史的結晶——"
                    "每條 lint 背後都站著一次"
                    "真的踩過的坑。"),
        ],
        "code": '# 判詞引用研究筆記原文：\n# 「uniform_* 跨標準庫實作異果」\n# 每條 lint 背後站著一次真坑',
        "narration": "第四個設計：工具鏈的自食期。"
                     "建議引擎吃自己倉庫的規範——"
                     "不只用自己的引擎蓋工具，"
                     "工具還用自己的規矩檢查自己。"
                     "判詞直接引用研究筆記的原文。"
                     "規則庫是專案歷史的結晶："
                     "每條檢查背後都站著一次真的踩坑。",
    },
    {
        "kind": "cover",
        "title": "第三十二集清單",
        "lines": ["路徑感知判決 · 教訓變 lint · 誤報是天敵 · 抓忘了做",
                  "文件可執行化 · 新 repo 改口 · 第三份留檔 · 自食期"],
        "narration": "第三十二集收在這裡。規矩走完了"
                     "最後一哩路：從踩坑、寫文件、"
                     "做影片，到住進工具裡在寫碼當下說話。"
                     "專案的記憶，從此不只靠人記得。",
    },
]

SLIDES_EP33 = [
    {
        "kind": "cover",
        "title": "訓練與推論一張臉",
        "lines": ["從實戰學 C++ 第三十三集",
                  "BattleCommandAI 契約層 · env 畢業進 lib · "
                  "特徵順序即介面 · 防漂移靠同一份碼"],
        "narration": "第三十三集講強化學習落地時"
                     "最危險的那個 bug：訓練和推論的特徵化"
                     "不一樣。"
                     "本專案的解法是讓兩邊呼叫同一份程式碼——"
                     "契約層 BattleCommandAI。",
    },
    {
        "tag": "設計 1",
        "title": "環境從 demo 畢業",
        "sections": [
            ("做法", "BattleCommandEnv 從 BattleTrainerDemo "
                    "內嵌的 293 行類別，畢業成 "
                    "Gameplay/BattleCommandEnv.h/.cpp——"
                    "訓練環境成為玩法庫的正式成員。"),
            ("細節", "Gameplay lib 因此新增 "
                    "ReinforcementLearning 連結——"
                    "RL 不再是範例程式的私藏，"
                    "是玩法層的依賴。"),
            ("啟示", "「demo 裡長出來的東西要正式化」"
                    "的判準：當它被第二個消費者需要，"
                    "就該從範例檔搬進庫。"),
        ],
        "code": '# 293 行內嵌類 → Gameplay/BattleCommandEnv\n# Gameplay 新增連結 ReinforcementLearning\n# 第二個消費者出現 = 該搬進庫了',
        "narration": "第一個設計：環境從示範程式畢業。"
                     "兩百九十三行的內嵌類搬進玩法庫，"
                     "玩法庫因此新增對強化學習模組的連結——"
                     "RL 不再是範例的私藏，是玩法層的依賴。"
                     "判準很簡單：第二個消費者出現，"
                     "就該搬進庫。",
    },
    {
        "tag": "設計 2",
        "title": "特徵順序就是權重介面",
        "sections": [
            ("做法", "BattleCommandAI.h 集中定義："
                    "kCommandStateSize=15、動作表五項、"
                    "FeaturizeCommandState/ApplyCommandAction "
                    "兩個自由函式。"),
            ("細節", "註解把後果寫死：「特徵順序與動作表"
                    "就是權重檔的介面，勿更動」——"
                    "改順序等於讓已訓練權重變垃圾。"),
            ("啟示", "ML 系統的 ABI 是特徵排列——"
                    "它不是內部實作，"
                    "是序列化邊界的一部分。"),
        ],
        "code": 'kCommandStateSize = 15;\nkCommandActionCount = 5;\n// 「特徵順序即權重檔介面，勿更動」\n// 改排列 = 舊權重變垃圾',
        "narration": "第二個設計：特徵順序就是權重介面。"
                     "契約標頭集中定義十五維狀態、五個動作、"
                     "特徵化與動作套用兩個自由函式。"
                     "註解把後果寫死：特徵順序是權重檔的介面，"
                     "改排列等於讓已訓練的權重變垃圾。"
                     "機器學習系統的二進位介面，"
                     "是特徵排列不是程式碼。",
    },
    {
        "tag": "設計 3",
        "title": "防漂移靠同一份碼",
        "sections": [
            ("做法", "訓練環境和遊戲內指揮官都呼叫同一個 "
                    "FeaturizeCommandState——兩邊不存在"
                    "「各自的特徵化實作」可漂移。"),
            ("細節", "訓練-推論偏斜是 ML 部署最陰險的 bug："
                    "兩份「看起來一樣」的特徵化，"
                    "模型表現靜靜地崩壞。"),
            ("啟示", "對付偏斜不要靠「記得同步兩邊」——"
                    "讓兩邊不存在：共用函式是"
                    "唯一能證明一致的寫法。"),
        ],
        "code": 'env.Reset() → FeaturizeCommandState(b,...)\nai.Tick()   → FeaturizeCommandState(b,...)\n// 同一份碼 → 漂移不存在',
        "narration": "第三個設計：防漂移靠同一份碼。"
                     "訓練環境和遊戲內指揮官呼叫同一個特徵化函式——"
                     "兩邊不存在各自的實作可以漂移。"
                     "訓練推論偏斜是部署最陰險的蟲："
                     "兩份看起來一樣的特徵化，"
                     "讓模型表現靜靜崩壞。"
                     "唯一能證明一致的寫法，是共用。",
    },
    {
        "tag": "設計 4",
        "title": "節奏也是契約",
        "sections": [
            ("做法", "決策節奏常數進契約層："
                    "tick 0.1 秒、每 10 tick 決策一次、"
                    "介入令持續 1.2 秒——"
                    "「換節奏權重即失效」。"),
            ("細節", "模型學到的是「每秒一次的決策節奏下"
                    "什麼動作好」——推論端換節奏，"
                    "學到的時間觀全錯。"),
            ("啟示", "RL 的隱形輸入是時間——"
                    "幀率、決策間隔、動作持續期都是"
                    "權重介面的一部分。"),
        ],
        "code": 'kCommandTickDt = 0.1f;\nkCommandTicksPerDecision = 10; // 1s\nkCommandHoldSeconds = 1.2f;\n// 換節奏 → 權重失效',
        "narration": "第四個設計：節奏也是契約。"
                     "決策間隔、動作持續期都進契約層——"
                     "換節奏權重即失效。"
                     "模型學到的是每秒一次決策下的時間觀，"
                     "推論端換節奏學到的全錯。"
                     "強化學習的隱形輸入是時間。",
    },
    {
        "tag": "設計 5",
        "title": "wait 是第零動作",
        "sections": [
            ("做法", "動作空間五項：wait/advance/engage/"
                    "hold/retreat——wait 排 0 且是唯一"
                    "不耗 CP 的動作。"),
            ("細節", "非 wait 動作對每支存活小隊走 "
                    "Intervene 各扣 1 CP——「介入」有"
                    "明確計價，什麼都不做也是學來的選項。"),
            ("啟示", "動作空間的第一格要給「不行動」——"
                    "CP 預算下，「不介入」"
                    "才是需要學會的高級動作。"),
        ],
        "code": '0 wait   不介入（省 CP）\n1 advance 全軍推進（每隊 -1 CP）\n2 engage 各隊接戰\n// 「不行動」也是要學的動作',
        "narration": "第五個設計：不行動是第零動作。"
                     "五個動作裡等待排零且是唯一不耗指揮點的——"
                     "其餘動作對每支存活小隊各扣一點。"
                     "介入有明確計價，什麼都不做也是學來的選項。"
                     "動作空間的第一格，要留給不行動。",
    },
    {
        "tag": "設計 6",
        "title": "指揮官進遊戲",
        "sections": [
            ("做法", "BattleCommanderAI 是推論消費端："
                    "載入 trained_battle_agent.json 權重，"
                    "即時戰場代打——RL 從訓練 demo "
                    "畢業成玩法功能。"),
            ("細節", "同一個 env 在訓練側是健身房、"
                    "在遊戲側是觀測器——契約層讓"
                    "「訓練時的世界」和「推論時的世界」"
                    "逐位相同。"),
            ("啟示", "RL 落地的完成式是消費者進產品層——"
                    "模型檔不再是 demo 的輸出物，"
                    "是遊戲載入的資產。"),
        ],
        "code": '// BattleCommanderAI:\n//   載入 trained_battle_agent.json\n//   即時戰場代打——權重成為遊戲資產',
        "narration": "第六個設計：指揮官進遊戲。"
                     "推論端載入訓練好的權重即時代打——"
                     "強化學習從訓練示範畢業成玩法功能。"
                     "契約層讓訓練時的世界和推論時的世界"
                     "逐位相同。"
                     "模型檔不再是示範的輸出物，"
                     "是遊戲載入的資產。",
    },
    {
        "tag": "設計 7",
        "title": "demo 瘦身后還是測試",
        "sections": [
            ("做法", "BattleTrainerDemo 減掉整個 env 類——"
                    "改 include Gameplay/BattleCommandEnv.h，"
                    "訓練迴圈照跑、驗收標準不變。"),
            ("細節", "demo 變薄但驗收的刺還在：環境機制、"
                    "reward 曲線向上、打贏基線、"
                    "策略可序列化回讀。"),
            ("啟示", "搬進庫的東西要留一個消費者——"
                    "demo 從「定義者」變「使用者」，"
                    "正好是契約層的活體測試。"),
        ],
        "code": '# demo 從「定義 env」變「使用 env」\n# 驗收不變：reward 向上/贏基線/可回讀\n# demo 本身是契約的活體測試',
        "narration": "第七個設計：示範瘦身後還是測試。"
                     "訓練程式砍掉內嵌環境改從庫引用——"
                     "驗收標準一條沒少。"
                     "搬進庫的東西要留一個消費者："
                     "示範從定義者變使用者，"
                     "正好是契約層的活體測試。",
    },
    {
        "tag": "設計 8",
        "title": "依賴加進 lib 的份量",
        "sections": [
            ("做法", "Gameplay 的 target_link_libraries "
                    "多了 ReinforcementLearning——"
                    "註解寫明理由「訓練-推論共用契約」。"),
            ("細節", "加依賴是架構事件：Gameplay 從此"
                    "認得 State/Action/RLEnvironment 的"
                    "型別——引擎的 AI 詞彙進了玩法層。"),
            ("啟示", "lib 的連結清單是設計宣言——"
                    "每加一個依賴都要能回答"
                    "「為什麼這層需要它」。"),
        ],
        "code": 'target_link_libraries(Gameplay PUBLIC\n    PotatoEngine Quantum ReinforcementLearning)\n# 註解：訓練-推論共用契約',
        "narration": "第八個設計：依賴加進庫的份量。"
                     "玩法庫的連結清單多了強化學習模組，"
                     "註解寫明理由是共用契約。"
                     "加依賴是架構事件——玩法層從此認得"
                     "狀態、動作、環境的型別。"
                     "連結清單是設計宣言，"
                     "每個依賴都要能回答為什麼。",
    },
    {
        "kind": "cover",
        "title": "第三十三集清單",
        "lines": ["env 畢業進 lib · 特徵順序=權重介面 · 同一份碼防漂移",
                  "節奏即契約 · 不行動是第零動作 · 權重成遊戲資產",
                  "demo 是活體測試 · 連結清單是宣言"],
        "narration": "第三十三集收在這裡。強化學習落地"
                     "最難的不是訓練，是讓訓練和推論"
                     "看到同一個世界——答案不是小心，"
                     "是讓兩邊根本沒有各自的實作。",
    },
]

SLIDES_EP34 = [
    {
        "kind": "cover",
        "title": "帳要記實得",
        "lines": ["從實戰學 C++ 第三十四集",
                  "D-3 審查輪 · 幻影錢包 · NaN 毒化 · gain 報實得 · "
                  "戰鬥時鐘為準"],
        "narration": "第三十四集，天命兌換器的審計輪。"
                     "審查抓到的不是語法錯誤，是經濟漏洞："
                     "沒登錄的神明名字可以憑空鑄幣，"
                     "記帳的增益可能和實際位移不符。"
                     "這集講兌換器怎麼補成誠實的。",
    },
    {
        "tag": "修正 1",
        "title": "幻影錢包防線",
        "sections": [
            ("做法", "Convert 新增 HasSpirit 檢查——"
                    "未登錄的 spirit 名字不得以 "
                    "kNeutralFavor=50 起算扣費。"),
            ("細節", "原邏輯的漏洞：Favor() 對陌生名字"
                    "回中立值 50，超過價目 20——"
                    "「隨便編個神名」就能無限兌換。"),
            ("啟示", "「預設值當餘額」是經濟系統的經典漏洞——"
                    "查不到的帳戶要拒絕，"
                    "不能假裝它有錢。"),
        ],
        "code": 'if (!myths->HasSpirit(spirit)) return false;\n// 未登錄名字不得憑 kNeutralFavor 鑄幣\n// 「查無此神」≠「此神有 50」',
        "narration": "第一個修正：幻影錢包防線。"
                     "兌換新增神明登錄檢查——"
                     "原本的漏洞是陌生名字回中立值五十，"
                     "隨便編個神名就能無限兌換。"
                     "預設值當餘額是經濟系統的經典漏洞："
                     "查不到的帳戶要拒絕，不能假裝它有錢。",
    },
    {
        "tag": "修正 2",
        "title": "顯示不等於可成交",
        "sections": [
            ("做法", "Options 對未登錄 spirit 仍按中立 50 "
                    "顯示價目——但 Convert 拒絕成交；"
                    "灰顯層和結算層各守各的判據。"),
            ("細節", "為什麼顯示不跟著拒絕：UI 要讓玩家"
                    "「看得見價目才知道要去認識這位神」——"
                    "資訊可見和交易可行是兩個維度。"),
            ("啟示", "報表層誠實 ≠ 交易層放行——"
                    "顯示的是「如果存在會是多少」，"
                    "成交的是「實際存在才有」。"),
        ],
        "code": 'Options("新神") → affordable 照顯示\nConvert("新神")  → 拒絕\n// 顯示≠成交：兩層各守判據',
        "narration": "第二個修正：顯示不等於可成交。"
                     "價目表對陌生神明仍按中立值顯示——"
                     "玩家要看得見價格才知道去結緣；"
                     "但成交口拒絕。"
                     "資訊可見和交易可行是兩個維度，"
                     "報表層和結算層各守各的判據。",
    },
    {
        "tag": "修正 3",
        "title": "gain 記實得不記名目",
        "sections": [
            ("做法", "FateEvent.gain 改記 after−before——"
                    "民心 98 時兌換名目 +5 實得 +2，"
                    "帳上就是 2。"),
            ("細節", "註解寫死原則：「record-is-truth："
                    "入帳數字即帳本實際位移」——"
                    "夾取吃掉的部分不入帳。"),
            ("啟示", "事件記錄要記「實際發生的位移」"
                    "不是「名目上的操作」——"
                    "回放和審計都要靠實得數字對帳。"),
        ],
        "code": 'const float before = Axis(*gov, target);\n...Adjust...(kFateGain);\nonEvent({..., after - before, ...});\n// 名目 5 實得 2 → 帳上記 2',
        "narration": "第三個修正：增益記實得不記名目。"
                     "民心九十八時兌換，名目加五實得加二，"
                     "帳上就記二。"
                     "入帳數字必須是帳本實際位移——"
                     "被夾取吃掉的部分不入帳，"
                     "回放和審計要靠實得對帳。",
    },
    {
        "tag": "修正 4",
        "title": "滿軸拒絕白扣費",
        "sections": [
            ("做法", "目標軸已滿 100 → Convert 直接拒絕，"
                    "favor 不扣；Options 同項灰顯"
                    "「目標已滿，兌換無益」。"),
            ("細節", "不擋的話玩家付出 20 favor 實得 0——"
                    "「收錢不給貨」在 UI 上比鎖定更傷信任。"),
            ("啟示", "零收益的成交要拒絕——"
                    "實得為零的交易是陷阱選項，"
                    "灰顯理由寫明「無益」幫玩家理解。"),
        ],
        "code": 'if (before >= 100.0f) return false;\n// Options: "目標已滿，兌換無益"\n// 收錢不給貨比鎖定更傷信任',
        "narration": "第四個修正：滿軸拒絕白扣費。"
                     "目標軸滿格時兌換直接拒絕、好感不扣，"
                     "價目同項灰顯寫明兌換無益。"
                     "收錢不給貨在介面上比鎖定更傷信任——"
                     "零收益的成交是陷阱選項。",
    },
    {
        "tag": "修正 5",
        "title": "NaN 是會傳染的毒",
        "sections": [
            ("做法", "Governance 讀檔改用 finiteOr："
                    "strtod 會把字串 \"nan\" 讀成真 NaN，"
                    "clamp(NaN) 還是 NaN——軸永久毒化。"),
            ("細節", "Adjust 也加自癒：現值非有限先回 "
                    "50 再調整——毒化過的檔案載入後"
                    "能慢慢回到可用狀態。"),
            ("啟示", "NaN 的可怕在不顯性失敗——"
                    "比較全假、夾取無效、欄位靜靜壞死。"
                    "所有外部數值入口都要 finite 檢查。"),
        ],
        "code": 'clamp(NaN) → NaN  // 夾取擋不住\nfiniteOr(v, def): isfinite? clamp : def\n// 外部數值入口全要 finite 檢查',
        "narration": "第五個修正：NaN 是會傳染的毒。"
                     "字串解析會把 nan 讀成真的非數值，"
                     "而夾取擋不住它——軸永久壞死，"
                     "所有比較靜靜回假。"
                     "讀檔加有限檢查、調整加自癒。"
                     "所有外部數值入口都要查有限性。",
    },
    {
        "tag": "修正 6",
        "title": "!(大於等於) 取代 小於",
        "sections": [
            ("做法", "favor 檢查從 favor<cost 改成 "
                    "!(favor>=cost)——NaN 時兩種寫法"
                    "行為相反，後者兩分支同拒。"),
            ("細節", "favor<cost 遇 NaN 回 false → 放行；"
                    "!(favor>=cost) 遇 NaN 回 true → 拒絕。"
                    "同價的判據，毒化下分流相反。"),
            ("啟示", "守衛條件要寫「允許式」而非「拒絕式」——"
                    "預設拒絕只在明確滿足時放行，"
                    "毒資料自然落入拒絕側。"),
        ],
        "code": 'favor < cost     // NaN→放行（危險）\n!(favor >= cost) // NaN→拒絕（安全）\n// 守衛寫允許式：毒資料落拒絕側',
        "narration": "第六個修正：守衛條件的寫法有方向。"
                     "小於比較遇到非數值會放行，"
                     "非大於等於遇到非數值會拒絕——"
                     "同價的判據，毒化下分流相反。"
                     "守衛要寫允許式：明確滿足才放行，"
                     "毒資料自然落進拒絕側。",
    },
    {
        "tag": "修正 7",
        "title": "再入不堵但要寫明",
        "sections": [
            ("做法", "回呼內再入 Convert 合法——測試證明"
                    "各計一次額度各扣一次費；文件寫明"
                    "「呼叫端自傷，favor 有界故可封頂」。"),
            ("細節", "Bind 重綁順帶歸零 battleUses——"
                    "比照 ShrineField::Bind 的場域"
                    "生命週期慣例。"),
            ("啟示", "再入的政策要選邊寫明——"
                    "堵（防禦）或不堵（文件化自傷）都行，"
                    "不行的是沒想過。"),
        ],
        "code": '// 回呼內 Convert 合法（測試證明）\n// 「呼叫端自傷，favor 有界可封頂」\n// Bind 重綁歸零額度（場域慣例）',
        "narration": "第七個修正：再入不堵但要寫明。"
                     "回呼裡再呼叫兌換是合法的——"
                     "測試證明各計一次額度各扣一次費，"
                     "文件寫明呼叫端自傷但資源有界可封頂。"
                     "重綁順帶歸零額度比照場域慣例。"
                     "再入政策堵或不堵都行，"
                     "不行的是沒想過。",
    },
    {
        "tag": "修正 8",
        "title": "時鐘以戰鬥為準",
        "sections": [
            ("做法", "BattleCommandAI 的 elapsed 從 "
                    "+= dt 累加改成讀 battle.GetElapsed()——"
                    "暫停/子彈時間下與 env 語義一致。"),
            ("細節", "自累加的鐘會和戰鬥時鐘漂移："
                    "暫停時 dt 仍在跑、慢動作時兩鐘"
                    "速率不同——權重學到的時間觀被破壞。"),
            ("啟示", "「現在幾點」要問權威時鐘——"
                    "自己數 dt 是同一份時間的"
                    "第二份真相，必然分叉。"),
        ],
        "code": 'snap.elapsed += dt;            // 自數→漂移\nsnap.elapsed = battle.GetElapsed(); // 權威時鐘\n// 暫停/慢動作下與 env 語義一致',
        "narration": "第八個修正：時鐘以戰鬥為準。"
                     "歷時從自己累加改成讀戰鬥時鐘——"
                     "暫停和慢動作下兩種算法會分叉，"
                     "權重學到的時間觀被破壞。"
                     "現在幾點要問權威時鐘，"
                     "自己數時間是同一份真相的第二份副本，"
                     "必然分叉。",
    },
    {
        "kind": "cover",
        "title": "第三十四集清單",
        "lines": ["幻影錢包要登錄 · 顯示≠成交 · gain 記實得",
                  "滿軸拒絕白扣費 · NaN 毒化防線 · 守衛寫允許式",
                  "再入政策寫明 · 時間問權威時鐘"],
        "narration": "第三十四集收在這裡。審計輪修的不是"
                     "崩潰是誠實：錢包要存在、帳要記實得、"
                     "毒資料落拒絕側、時間只有一個主人。"
                     "經濟系統的品質，"
                     "全在這些不放行的地方。",
    },
]

SLIDES_EP35 = [
    {
        "kind": "cover",
        "title": "規格書的修正案",
        "lines": ["從實戰學 C++ 第三十五集",
                  "Spec Change Log · 觸發/裁決/修訂/KEEP · "
                  "越界要授權 · done 附成績單"],
        "narration": "第三十五集講一份規格書怎麼收場。"
                     "D-3 標 done 時，spec 尾端多了一個"
                     "新章節：Spec Change Log——"
                     "把審查輪改過什麼、誰裁決的、"
                     "哪些刻意不動，全部烙進文件。"
                     "規格書學會了記自己的修正案。",
    },
    {
        "tag": "設計 1",
        "title": "修正案的四段式",
        "sections": [
            ("做法", "Change Log 條目固定四段：觸發"
                    "（審查發現什麼）→ 裁決（誰怎麼定）→ "
                    "修訂（改了哪些）→ KEEP（什麼不動）。"),
            ("細節", "D-3 的觸發寫得很完整：「未登錄 spirit "
                    "走 kNeutralFavor 起算、章節邊界不限次"
                    " → 幻影錢包」——漏洞的可復現路徑"
                    "留在文件裡。"),
            ("啟示", "修正要留痕到規格書——diff 記改了什麼，"
                    "change log 記為什麼改，"
                    "兩者缺一再讀的人都只能猜。"),
        ],
        "code": '## Spec Change Log\n- 觸發：三路審查 — 幻影錢包\n- 裁決（使用者）：顯示保留/成交拒絕\n- 修訂：HasSpirit + 滿軸 + gain實效\n- KEEP：回呼翻轉/全列價目/...',
        "narration": "第一個設計：修正案的四段式。"
                     "觸發、裁決、修訂、保留——"
                     "觸發段把漏洞的可復現路徑留在文件裡，"
                     "修訂段列出每個改動的去向。"
                     "差異記錄改了什麼，變更日誌記為什麼改，"
                     "兩者缺一，再讀的人只能猜。",
    },
    {
        "tag": "設計 2",
        "title": "裁決欄寫明是誰",
        "sections": [
            ("做法", "「裁決（使用者）」——agent 提出"
                    "發現和選項，方向由人類定："
                    "Options 維持中立顯示、Convert 拒絕。"),
            ("細節", "顯示層不跟著拒絕是產品判斷不是"
                    "技術判斷——「價目看得見」是設計意圖，"
                    "只能由握有意圖的人裁。"),
            ("啟示", "審查發現的分歧要標明裁決者——"
                    "「誰決定的」和「決定什麼」"
                    "同樣是文件內容。"),
        ],
        "code": '裁決（使用者）：\n  Options 維持中立 50 顯示（設計意圖）\n  Convert 對未登錄 spirit 拒絕（技術）\n# 產品判斷由握有意圖的人裁',
        "narration": "第二個設計：裁決欄寫明是誰。"
                     "代理提出發現和選項，方向由使用者定——"
                     "顯示層留價目是產品判斷，"
                     "成交口拒絕是技術判斷。"
                     "審查發現的分歧要標明裁決者："
                     "誰決定的，和決定什麼同樣是文件內容。",
    },
    {
        "tag": "設計 3",
        "title": "越界要授權入檔",
        "sections": [
            ("做法", "MythLayer 補 HasSpirit() 越過了 "
                    "spec 自己的「不改公開介面」邊界——"
                    "change log 明寫「由使用者授權」。"),
            ("細節", "spec 的邊界不是不能越，"
                    "是越界本身要成為記錄的一部分——"
                    "悄悄越界讓邊界失去意義。"),
            ("啟示", "規則的例外要留批文——"
                    "「這次越界是誰批的」入檔，"
                    "邊界才不會在例外中溶解。"),
        ],
        "code": '# MythLayer::HasSpirit 越過\n# spec「不改公開介面」邊界\n# → 「由使用者授權」入檔\n# 例外的批文也是記錄',
        "narration": "第三個設計：越界要授權入檔。"
                     "為滲透層補唯讀查詢，越過了規格書自己的"
                     "不改介面邊界——變更日誌明寫由使用者授權。"
                     "邊界不是不能越，是越界要成為記錄的一部分。"
                     "規則的例外要留批文，"
                     "邊界才不會在例外中溶解。",
    },
    {
        "tag": "設計 4",
        "title": "KEEP 清單寫「不動什麼」",
        "sections": [
            ("做法", "修正案的結尾是 KEEP：回呼翻轉依賴、"
                    "Options 全列價目、戰中額度執行期計數、"
                    "先變異後派出——審查看過且決定不動。"),
            ("細節", "「看過不改」和「沒看過」天差地別——"
                    "KEEP 把前者變成後人的定心丸："
                    "這些設計是存活下來的，不是漏網的。"),
            ("啟示", "審查的產出包括「不改的清單」——"
                    "被考慮過而保留的設計，"
                    "和修正項同樣是結論。"),
        ],
        "code": 'KEEP：回呼翻轉依賴 / 全列價目 /\n     執行期計數 / 先變異後派出\n# 「看過不改」≠「沒看過」',
        "narration": "第四個設計：保留清單寫明不動什麼。"
                     "回呼翻轉、全列價目、執行期計數——"
                     "審查看過且決定不動的全列出來。"
                     "看過不改和沒看過天差地別："
                     "保留清單讓後人知道這些設計是"
                     "存活下來的，不是漏網的。"
                     "不改的清單和修正項同樣是結論。",
    },
    {
        "tag": "設計 5",
        "title": "測試跟著修正案長",
        "sections": [
            ("做法", "FateExchangeTest 依修訂項逐條加案："
                    "幻影拒絕、gain 實效、favor==cost 邊界、"
                    "多神隔離、回呼再入——52 項。"),
            ("細節", "每條修訂對應測試案例——"
                    "幻影錢包有「拒絕不鑄錢包」斷言，"
                    "gain 實效有「98+名目5→記2」斷言。"),
            ("啟示", "修正案沒有測試等於沒修——"
                    "change log 的每一行修訂，"
                    "都要能在測試裡指到對應的紅燈。"),
        ],
        "code": '// 幻影拒絕 / gain=2非名目5 /\n// favor==cost 邊界 / 多神隔離 / 再入\n// 每條修訂 → 至少一個紅燈斷言',
        "narration": "第五個設計：測試跟著修正案長。"
                     "幻影拒絕、增益記實得、邊界值、"
                     "多神隔離、回呼再入——"
                     "五十二項斷言每條對應一條修訂。"
                     "修正案沒有測試等於沒修："
                     "變更日誌的每一行，"
                     "都要能在測試裡指到對應的紅燈。",
    },
    {
        "tag": "設計 6",
        "title": "建議引擎吃新慣例",
        "sections": [
            ("做法", "BattleCommanderAITest 修正型別："
                    "GetCommandPoints 回 int 不是 float——"
                    "測試用對的型別接，斷言才有意義。"),
            ("細節", "CommandPoints 是整數資源——"
                    "用 float 接 int 回傳是隱性擴寬，"
                    "等值比較時浮點陷阱全開。"),
            ("啟示", "接 API 先看回傳型別——"
                    "「能編過」的接法和「語義對」的接法"
                    "是兩件事。"),
        ],
        "code": 'const int cp = battle.GetCommandPoints(0);\n// 不是 float——整數資源用整數接\n// 隱性擴寬在等值比較時埋雷',
        "narration": "第六個設計：小修正也是知識。"
                     "指揮官測試把回傳值從浮點收成整數——"
                     "指揮點是整數資源，用浮點接是隱性擴寬，"
                     "等值比較時浮點陷阱全開。"
                     "接介面先看回傳型別："
                     "能編過的接法和語義對的接法是兩件事。",
    },
    {
        "tag": "設計 7",
        "title": "done 的成績單常態化",
        "sections": [
            ("做法", "佇列註記「86bedfa，三路審查補丁後 "
                    "52/52 雙工具鏈」——commit 號、"
                    "測試數、工具鏈三欄齊全。"),
            ("細節", "連漏洞名都入註記：「幻影錢包/滿軸白扣"
                    "已堵」——讀佇列的人不用翻 spec "
                    "就知道這輪修過什麼級別的東西。"),
            ("啟示", "done 註記的資訊密度持續上升——"
                    "從「已完成」到 commit 號到漏洞名，"
                    "狀態欄變成索引。"),
        ],
        "code": 'status: done\nnote: 86bedfa（三路審查後 52/52 雙工具鏈）\n      幻影錢包/滿軸白扣已堵\n# 狀態欄 = 索引不是宣告',
        "narration": "第七個設計：完成的成績單常態化。"
                     "佇列註記帶 commit 號、測試數、雙工具鏈，"
                     "連修過的漏洞名都入列。"
                     "讀佇列的人不用翻規格書，"
                     "就知道這輪修過什麼級別的東西。"
                     "狀態欄從宣告變成索引。",
    },
    {
        "tag": "設計 8",
        "title": "spec 生命週期走完全程",
        "sections": [
            ("做法", "D-3 走完 draft→in-progress→in-review"
                    "→done+changelog——規格書狀態機的"
                    "全路徑第一次跑通。"),
            ("細節", "in-review 階段產出 review.diff、"
                    "deferred 條目、change log 三種產物——"
                    "審查輪本身有自己的交付物清單。"),
            ("啟示", "流程跑通一次的證據是產物齊全——"
                    "每個狀態轉換都留下了"
                    "該階段該有的文件。"),
        ],
        "code": '# draft → in-progress → in-review\n# → done + ChangeLog + review.diff\n#   + deferred 條目\n# 每個轉換留下該有的產物',
        "narration": "第八個設計：規格書生命週期走完全程。"
                     "草稿、進行中、審查中、完成附修正案——"
                     "四個狀態第一次全部跑通。"
                     "審查階段產出三種產物："
                     "審查差異、延期條目、變更日誌。"
                     "流程跑通的證據是產物齊全："
                     "每個狀態轉換都留下該有的文件。",
    },
    {
        "kind": "cover",
        "title": "第三十五集清單",
        "lines": ["四段式修正案 · 裁決署名 · 越界留批文 · KEEP 清單",
                  "修訂配測試 · 型別接對 · done 附索引 · 全程跑通"],
        "narration": "第三十五集收在這裡。規格書最後學會的"
                     "是記自己的修正案——觸發、裁決、修訂、"
                     "保留，四段讓「改了什麼」和"
                     "「為什麼改」都留在原地。"
                     "文件誠實至此，功能才算真的 done。",
    },
]

SLIDES_EP36 = [
    {
        "kind": "cover",
        "title": "資產驗明正身",
        "lines": ["從實戰學 C++ 第三十六集",
                  "F-5 字體打包 · sfnt magic · 失敗不污染 atlas · "
                  "回退鏈明確化"],
        "narration": "第三十六集講字體打包的最後一哩。"
                     "F-5 收尾做的不只是「把字體放進去」——"
                     "引用要驗真身、檔頭要驗 magic、"
                     "缺檔要有降級鏈、失敗不能污染 atlas。"
                     "資產和程式碼一樣，要被驗明正身。",
    },
    {
        "tag": "設計 1",
        "title": "引用檢查：名字對得到檔案",
        "sections": [
            ("做法", "遍歷每個主題的 FontRel 引用路徑，"
                    "驗證檔案存在且大小超過 1MB——"
                    "引用完整性本身是測試項。"),
            ("細節", "1MB 門檻是校準過的：真字體檔以 MB 計，"
                    "空檔或誤放的小檔會被抓到——"
                    "「存在」之外還要「夠大像字體」。"),
            ("啟示", "配置檔裡的路徑引用是隱形依賴——"
                    "主題說它用某個字體，"
                    "測試就要證明那個字體真的在。"),
        ],
        "code": 'for (id : allThemes)\n    Check(file_size(FontRel(id)) > 1MB,\n          "FontRel 引用檔存在且非空");\n// 引用完整性 = 測試項',
        "narration": "第一個設計：引用檢查。"
                     "遍歷每個主題宣告的字體路徑，"
                     "驗證檔案存在且超過一 MB——"
                     "大小門檻是校準過的，"
                     "誤放的小檔會被抓到。"
                     "配置裡的路徑引用是隱形依賴："
                     "主題說用某字體，測試就證明它在。",
    },
    {
        "tag": "設計 2",
        "title": "magic bytes 驗真身",
        "sections": [
            ("做法", "讀檔頭四 bytes：OTTO（OpenType/CFF）"
                    "或 00 01 00 00（TrueType）——"
                    "內容驗證，不只是檔名驗證。"),
            ("細節", "抓的到兩種現實災難：截斷的字體檔"
                    "（下載一半）和誤放的非字型檔"
                    "（rename 充數）。"),
            ("啟示", "「是對的檔案」要驗內容不驗名字——"
                    "副檔名會說謊，magic bytes 不太會。"),
        ],
        "code": 'memcmp(magic, "OTTO", 4) == 0 ||\nmagic == 00 01 00 00  // TrueType\n// 副檔名會說謊，magic 不太會',
        "narration": "第二個設計：魔術位元組驗真身。"
                     "讀檔頭四個位元組——O-T-T-O 或零零零一，"
                     "是內容驗證不是檔名驗證。"
                     "抓得到截斷的字體和誤放的假檔。"
                     "是對的檔案要驗內容——"
                     "副檔名會說謊，檔頭不會。",
    },
    {
        "tag": "設計 3",
        "title": "失敗不污染 atlas",
        "sections": [
            ("做法", "LoadFont 對缺檔回 nullptr，且測試斷言 "
                    "io.Fonts->Fonts.size() 不變——"
                    "失敗的載入不留半個條目在字體集裡。"),
            ("細節", "污染型失敗最難抓：回 nullptr 看似乾淨，"
                    "但 atlas 裡多了個壞條目會在"
                    "幾幀後的渲染才炸。"),
            ("啟示", "載入函式的失敗要原子——"
                    "回空之外，容器狀態也要回到"
                    "呼叫前的樣子。"),
        ],
        "code": 'const int before = io.Fonts->Fonts.size();\nCheck(LoadFont(io, "__missing__.otf") == nullptr);\nCheck(io.Fonts->Fonts.size() == before);\n// 失敗原子：回空且不污染',
        "narration": "第三個設計：失敗不污染字體集。"
                     "缺檔回空指標之外，測試還斷言"
                     "字體集大小沒變——失敗的載入不留半個條目。"
                     "污染型失敗最難抓：回空看似乾淨，"
                     "壞條目卻在幾幀後的渲染才炸。"
                     "載入函式的失敗要原子。",
    },
    {
        "tag": "設計 4",
        "title": "回退鏈明確化",
        "sections": [
            ("做法", "serif→sans→內建字三級回退寫成"
                    "顯式賦值鏈——fontFallback 逐級補位，"
                    "兩個指標最後都保證非空。"),
            ("細節", "原本依賴 PushFont(nullptr) 會退回"
                    "目前字體的隱含行為——能用，"
                    "但那是「剛好能用」不是「設計如此」。"),
            ("啟示", "函式庫的隱含語義不是契約——"
                    "依賴它等於把正確性押在"
                    "別人的實作細節上。"),
        ],
        "code": 'fontFallback = sans ?: serif ?: builtin;\nfontSans  = fontSans  ?: fontFallback;\nfontSerif = fontSerif ?: fontFallback;\n// 隱含退回行為 → 顯式回退鏈',
        "narration": "第四個設計：回退鏈明確化。"
                     "襯線、無襯線、內建字三級回退寫成顯式賦值鏈，"
                     "兩個指標最後保證非空。"
                     "原本依賴函式庫對空指標的隱含退回——能用，"
                     "但那是剛好能用不是設計如此。"
                     "函式庫的隱含語義不是契約。",
    },
    {
        "tag": "設計 5",
        "title": "內建字是最後的地板",
        "sections": [
            ("做法", "兩個字體都缺時 AddFontDefault()——"
                    "「字體缺檔也要保證 atlas 非空」："
                    "回退鏈的盡頭是一定存在的字。"),
            ("細節", "ImGui 內建字體編譯進二進位——"
                    "不讀檔、不失敗、永遠在場，"
                    "這才叫地板。"),
            ("啟示", "降級鏈的最後一棒不能依賴外部資源——"
                    "地板必須是編譯進來的東西，"
                    "否則鏈條有斷點。"),
        ],
        "code": 'if (!fontSans && !fontSerif)\n    io.Fonts->AddFontDefault();\n// 地板 = 編譯進來的、永遠在場的',
        "narration": "第五個設計：內建字是最後的地板。"
                     "兩個字體都缺時退回內建字——"
                     "它編譯進二進位、不讀檔不失敗永遠在場。"
                     "降級鏈的最後一棒不能依賴外部資源，"
                     "地板必須是編譯進來的東西。",
    },
    {
        "tag": "設計 6",
        "title": "license 跟字體一起打包",
        "sections": [
            ("做法", "測試項裡有「OFL LICENSE 隨字體打包」——"
                    "開源字體的授權檔和字體本體"
                    "一起被驗證存在。"),
            ("細節", "vendored 規則延伸到資產：第三方東西"
                    "入庫要帶授權——字體和 tinygltf "
                    "守同一條規矩。"),
            ("啟示", "license 是資產的一部分不是附件——"
                    "「授權檔在不在」可以是測試項，"
                    "合規做成自動檢查。"),
        ],
        "code": 'Check(OFL_LICENSE 隨字體打包);\n// 授權檔在不在 = 測試項\n// 合規做成自動檢查',
        "narration": "第六個設計：授權跟字體一起打包。"
                     "測試裡有授權檔存在的斷言——"
                     "第三方資產的規矩延伸到字體："
                     "入庫要帶授權，合規做成自動檢查。"
                     "授權檔是資產的一部分，不是附件。",
    },
    {
        "tag": "設計 7",
        "title": "決策有編號可引",
        "sections": [
            ("做法", "程式註解引 UX-DR3（字體回退鏈的"
                    "設計決策記錄）——程式碼裡直接點名"
                    "文件裡的決策編號。"),
            ("細節", "UX 文件的決策項有穩定編號——"
                    "程式註解說「為什麼這樣」時引編號，"
                    "讀碼的人能跳回決策的完整上下文。"),
            ("啟示", "決策記錄要有可引用的 ID——"
                    "DR3 比「那個字體的決定」"
                    "耐搜尋得多。"),
        ],
        "code": '// F-5/UX-DR3：chronicler 字體回退鏈\n// 程式註解 → 決策記錄編號\n// DR3 比「那個字體的決定」耐搜尋',
        "narration": "第七個設計：決策有編號可引。"
                     "程式註解直接點名設計決策記錄的編號——"
                     "讀碼的人能跳回決策的完整上下文。"
                     "決策記錄要有可引用的識別碼："
                     "DR 三比「那個字體的決定」耐搜尋得多。",
    },
    {
        "tag": "設計 8",
        "title": "驗證做到檔頭層",
        "sections": [
            ("做法", "無頭測試直接讀二進位檔頭——"
                    "資產驗證不需要渲染器，"
                    "ifstream 加 memcmp 就夠。"),
            ("細節", "本系列的驗證光譜又補一格："
                    "帳本驗雜湊、存檔驗 schema、"
                    "字體驗 magic——「不信就驗」"
                    "貫穿所有資料入口。"),
            ("啟示", "驗證的層級要和資料的層級對齊——"
                    "二進位資產就用二進位的方式驗，"
                    "不必上升到「渲染出來看看」。"),
        ],
        "code": '// 帳本驗雜湊 · 存檔驗 schema · 字體驗 magic\n// 「不信就驗」貫穿所有資料入口\n// 二進位資產用二進位方式驗',
        "narration": "第八個設計：驗證做到檔頭層。"
                     "無頭測試直接讀二進位檔頭——"
                     "資產驗證不需要渲染器。"
                     "本系列的驗證光譜又補一格：帳本驗雜湊、"
                     "存檔驗架構標籤、字體驗魔術位元組——"
                     "不信就驗，貫穿所有資料入口。",
    },
    {
        "kind": "cover",
        "title": "第三十六集清單",
        "lines": ["引用完整性是測試項 · magic 驗真身 · 失敗原子",
                  "回退鏈顯式 · 地板編譯進來 · license 隨檔",
                  "決策有編號 · 驗證對齊資料層級"],
        "narration": "第三十六集收在這裡。字體打包的最後一哩"
                     "全是驗證：引用對得到檔、檔頭是對的格式、"
                     "失敗不留痕、回退有地板。"
                     "資產入庫和程式碼入庫，守同一套規矩。",
    },
]

SLIDES_EP37 = [
    {
        "kind": "cover",
        "title": "神話越界",
        "lines": ["從實戰學 C++ 第三十七集",
                  "D-4 MythIncursion · 第三方鬼軍 · 假訊武器化 · "
                  "十五秒解決窗口"],
        "narration": "第三十七集，神話層打回來了。"
                     "D-4 入侵事件讓滲透到頂的區域在戰鬥中"
                     "生出鬼軍——玩家拜神、兌換、忽視的帳，"
                     "神話層開始用實體收。",
    },
    {
        "tag": "設計 1",
        "title": "滲透快照只吃 int",
        "sections": [
            ("做法", "Update 收 seepageLevel 是 int 不是 "
                    "MythLayer::Seepage——註解明寫「與序數對齊"
                    "但此處只吃 int，不引 Campaign 型別」。"),
            ("細節", "Gameplay 層認得滲透的概念，"
                    "不認得滲透的型別——序數透過 int 越界，"
                    "enum 定義留在上層。"),
            ("啟示", "跨層傳語義不傳型別——"
                    "序數契約（0-3）寫在註解，"
                    "型別依賴留在原地。"),
        ],
        "code": 'void Update(dt, battle, int seepageLevel,\n            region, spirit);\n// int 越界，enum 留下——型別依賴不過層',
        "narration": "第一個設計：滲透快照只吃整數。"
                     "驅動口收的是 int 不是上層枚舉——"
                     "序數對齊寫在註解，型別定義留在上層。"
                     "跨層傳語義不傳型別："
                     "玩法層認得滲透的概念，"
                     "不認得滲透的型別。",
    },
    {
        "tag": "設計 2",
        "title": "稀有性是閂不是機率",
        "sections": [
            ("做法", "每場最多一次：armed 由 threshold>0 "
                    "決定（Quiet 章永不入侵）、fired 閂住"
                    "不重觸、resolved 封頂不再結算。"),
            ("細節", "生成失敗不消耗閂——CreateSquad 回空時"
                    "直接 return 下拍重試，"
                    "「試過沒成」不等於「觸發過」。"),
            ("啟示", "稀有事件用閂不用骰——"
                    "每場一次的敘事重量來自確定性，"
                    "不來自機率。"),
        ],
        "code": 'armed = threshold > 0;  // Quiet 章不入侵\nif (!ghost) return;  // 失敗不耗閂\nstate.fired = true;  // 一生一次',
        "narration": "第二個設計：稀有性是閂不是機率。"
                     "每場最多一次——滲透為零的章節永不入侵，"
                     "觸發閂住不重發。"
                     "生成失敗不消耗閂，下一拍重試："
                     "試過沒成不等於觸發過。"
                     "每場一次的敘事重量來自確定性，"
                     "不來自骰子。",
    },
    {
        "tag": "設計 3",
        "title": "第三方 team=2",
        "sections": [
            ("做法", "鬼軍用 CreateSquad(team=2) 中場生成——"
                    "不屬玩家不屬敵軍的第三方存在，"
                    "無 doctrine 的 Hold 幽影。"),
            ("細節", "「威懾性存在」不是來打的："
                    "十二員象徵性戰力佔在場上，"
                    "是神話層對戰場的實體註腳。"),
            ("啟示", "陣營值也是詞彙——"
                    "team 2 的出現讓「不屬於戰爭雙方」"
                    "成為可編碼的概念。"),
        ],
        "code": 'ghost = battle.CreateSquad("鬼軍:"+s,\n    kGhostTeam=2, spawnPos, 12);\n// 第三方：不屬戰爭雙方的存在',
        "narration": "第三個設計：第三方陣營。"
                     "鬼軍用二號陣營中場生成——"
                     "不屬玩家不屬敵軍，無教條的駐守幽影。"
                     "十二員象徵性戰力不是來打的，"
                     "是神話層對戰場的實體註腳。"
                     "陣營值也是詞彙：二號讓不屬於戰爭雙方"
                     "成為可編碼的概念。",
    },
    {
        "tag": "設計 4",
        "title": "假訊武器化情報系統",
        "sections": [
            ("做法", "FoxRumor 在情報霧裡放一朵假雲："
                    "AddEntityCloud 指向 spawnPos 偏移八格的"
                    "虛構方位——真身其實在原地。"),
            ("細節", "「無主雲不渲染」所以假雲必須綁真小隊；"
                    "無 fog 的戰鬥降級為純現身——"
                    "騙局需要情報系統在場才成立。"),
            ("啟示", "最有神話感的機制是攻擊玩家的資訊——"
                    "假訊不打兵，打的是地圖上的雲。"),
        ],
        "code": 'fake = spawnPos; fake.x += 8.0f;\nAddEntityCloud(ghost, team1, fake, ...);\nBindFogSquad(ghost, eid); // 雲需真身\n// 假訊攻擊的是情報，不是兵',
        "narration": "第四個設計：假訊武器化情報系統。"
                     "狐仙路徑在迷霧裡放一朵假雲——"
                     "指向偏移八格的虛構方位，真身在原地。"
                     "無主的雲不渲染所以必須綁真小隊；"
                     "沒有情報霧的戰鬥降級成純現身。"
                     "最有神話感的機制是攻擊玩家的資訊："
                     "假訊不打兵，打地圖上的雲。",
    },
    {
        "tag": "設計 5",
        "title": "十五秒是一個劇場",
        "sections": [
            ("做法", "pending 窗口 kResolveWindow=15 秒，"
                    "由 Update 的 dt 累積結算——"
                    "逾時自動按 ignored 收場，全確定性。"),
            ("細節", "「忽視」是預設結局不是無結局——"
                    "不動十五秒本身就是一種回應，"
                    "逾時和主動無視殊途同歸。"),
            ("啟示", "限時選擇把「不做決定」變成決定——"
                    "窗口的存在讓無視有了代價形狀。"),
        ],
        "code": 'state.windowLeft -= dt;\nif (windowLeft <= 0) Settle(false);\n// 逾時=忽視：不動也是回應',
        "narration": "第五個設計：十五秒是一個劇場。"
                     "解決窗口由時間累積結算，全確定性——"
                     "逾時自動按忽視收場。"
                     "忽視是預設結局不是無結局："
                     "不動十五秒本身就是一種回應。"
                     "限時選擇讓不做決定也有了代價形狀。",
    },
    {
        "tag": "設計 6",
        "title": "消散借用殲滅語義",
        "sections": [
            ("做法", "安撫成功讓鬼隊消散——實作是 "
                    "ApplyCasualties(全員)，"
                    "註解坦白「無 RemoveSquad API」。"),
            ("細節", "不為消散新造移除介面——"
                    "傷亡歸零本來就是「這支隊不存在了」"
                    "的既有動詞，復用它語義剛好。"),
            ("啟示", "缺介面時先看有沒有同義動詞——"
                    "「全員陣亡」和「消散」在戰鬥系統裡"
                    "是同一個終態。"),
        ],
        "code": 'ghost->ApplyCasualties(ghost->GetMembers());\n// 消散 = 殲滅語義（無 RemoveSquad API）\n// 缺介面 → 找同義動詞',
        "narration": "第六個設計：消散借用殲滅語義。"
                     "安撫成功讓鬼隊全員傷亡歸零——"
                     "沒有移除小隊的介面，"
                     "但全員陣亡本來就是同一個終態。"
                     "缺介面時先看有沒有同義動詞，"
                     "不急著新造。",
    },
    {
        "tag": "設計 7",
        "title": "三回呼三去向",
        "sections": [
            ("做法", "三路出口分工：IncursionEvent→MythLog "
                    "具名欄+暫停提示；OutcomeEvent→治理加分"
                    "或滲透注壓；字串→recorder 入帳。"),
            ("細節", "連注壓量都不寫死——「呼叫端決定」："
                    "忽視的神會多生氣是戰役層的敘事判斷，"
                    "不是入侵器的常數。"),
            ("啟示", "回呼粒度對齊消費者——"
                    "型別化事件給帳本、結果事件給治理、"
                    "字串給回放，三路各取所需。"),
        ],
        "code": 'onIncursion → MythLog.Record + SetTimeScale(0)\nonOutcome   → gov.Adjust* / MythLayer 注壓\nonEvent     → recorder.AddRecord(t, msg)\n// 注壓量呼叫端決定',
        "narration": "第七個設計：三回呼三去向。"
                     "入侵事件進神話誌配暫停提示、"
                     "結局事件進治理或滲透、字串進記錄器——"
                     "連忽視的神會多生氣都不寫死，"
                     "注壓量是呼叫端的敘事判斷。"
                     "回呼粒度對齊消費者，三路各取所需。",
    },
    {
        "tag": "設計 8",
        "title": "MythLayer 補唯讀出口",
        "sections": [
            ("做法", "MythLayer 新增 SpiritOf(region)——"
                    "查區域綁定守護靈名的唯讀查詢，"
                    "入侵事件的具名欄靠它填。"),
            ("細節", "資料流動方向：Campaign 讀自己的 "
                    "SpiritOf、快照餵給 Gameplay 的 Update——"
                    "Gameplay 永遠只收到結果字串。"),
            ("啟示", "上層補唯讀口是配合下層的方式——"
                    "下層要名字但不認得型別，"
                    "由上層查出來再餵下去。"),
        ],
        "code": 'std::string SpiritOf(region) const;\n// Campaign 查完餵快照給 Update\n// Gameplay 只收字串不碰型別',
        "narration": "第八個設計：上層補唯讀出口。"
                     "滲透層新增按區域查守護靈的唯讀查詢——"
                     "資料流向是戰役層查出來、"
                     "快照餵給玩法層，"
                     "玩法層永遠只收到結果字串。"
                     "下層要名字但不認得型別，"
                     "由上層查出來餵下去。",
    },
    {
        "kind": "cover",
        "title": "第三十七集清單",
        "lines": ["int 越界型別留下 · 閂不耗失敗 · 第三方陣營",
                  "假訊打情報不打兵 · 逾時即忽視 · 消散借殲滅",
                  "三回呼三去向 · 上層補唯讀口"],
        "narration": "第三十七集收在這裡。神話雙層的迴路"
                     "至此完整：壓力積累、神社選擇、"
                     "天命變現、入侵收帳——"
                     "玩家對神明做的每件事，"
                     "神話層都記得，也都會回來。",
    },
]

SLIDES_EP38 = [
    {
        "kind": "cover",
        "title": "靜音契約",
        "lines": ["從實戰學 C++ 第三十八集",
                  "F-4 MiniaudioBackend · 降級契約 · "
                  "語意音效層 · 滲透音景"],
        "narration": "第三十八集，遊戲學會沉默。"
                     "F-4 把音訊後端從 OpenAL 換成 miniaudio——"
                     "但這集的主角不是新後端，是它的降級契約："
                     "沒有裝置、沒有音檔，遊戲照常跑。",
    },
    {
        "tag": "設計 1",
        "title": "換心臟不換血管",
        "sections": [
            ("做法", "InitializeAudioManager 內把 "
                    "OpenALAudioManager 換成 "
                    "MiniaudioAudioManager——IAudioManager "
                    "介面一個方法沒改。"),
            ("細節", "所有呼叫端（UI、戰鬥、demo）"
                    "透過同一介面走新後端——"
                    "切換只動一行工廠，"
                    "消費者零感知。"),
            ("啟示", "抽象介面的真正回報："
                    "換實作當天，"
                    "全世界只有一行 diff。"),
        ],
        "code": '- new OpenALAudioManager();\n+ new MiniaudioAudioManager();\n// IAudioManager 介面不變\n// 呼叫端 diff = 0',
        "narration": "第一個設計：換心臟不換血管。"
                     "音訊管理器的工廠函式裡，"
                     "OpenAL 實作被換成 miniaudio 實作，"
                     "介面一個方法都沒動。"
                     "所有呼叫端透過同一層抽象走新後端，"
                     "diff 只有一行——"
                     "這就是當年定義介面的回報。",
    },
    {
        "tag": "設計 2",
        "title": "音訊失敗不擋遊戲",
        "sections": [
            ("做法", "無輸出裝置或初始化失敗 → "
                    "available_=false、所有操作靜音 no-op，"
                    "但 Initialize() 仍回 true。"),
            ("細節", "契約寫進類註解：Shutdown 冪等、"
                    "失敗後可再 Initialize 重試——"
                    "降級行為是文件化的承諾。"),
            ("啟示", "音訊是可選增強——"
                    "遊戲不是音訊的人質，"
                    "靜音的遊戲仍是完整的遊戲。"),
        ],
        "code": 'bool Initialize() override {\n  if (!device) { available_ = false;\n                 return true; }  // 靜音照跑\n}',
        "narration": "第二個設計：音訊失敗不擋遊戲。"
                     "沒有輸出裝置、初始化失敗，"
                     "標記不可用、所有操作靜音空轉——"
                     "但初始化照樣回傳成功。"
                     "這不是 bug 是契約："
                     "靜音的遊戲仍是完整的遊戲，"
                     "音訊是可選增強不是前置條件。",
    },
    {
        "tag": "設計 3",
        "title": "Null 後端是測試口",
        "sections": [
            ("做法", "InitializeNullBackend() 強制 "
                    "ma_backend_null——有完整輸出管線，"
                    "只是不吃真實裝置。"),
            ("細節", "無頭 CI 也能跑音訊路徑："
                    "播放請求、時序、狀態機全部可驗，"
                    "驗的不是聲音是流程。"),
            ("啟示", "測試口長在公開 API 上——"
                    "不靠 #ifdef 不靠 mock，"
                    "後端自己提供靜音模式。"),
        ],
        "code": 'bool InitializeNullBackend();\n// 強制 ma_backend_null\n// 有管線無裝置——CI 可驗流程',
        "narration": "第三個設計：null 後端是測試口。"
                     "強制虛擬後端——完整的播放管線都在，"
                     "只是不輸出到真實裝置。"
                     "無頭環境能跑音訊路徑、"
                     "驗播放請求和狀態轉換，"
                     "驗的不是聲音是流程。"
                     "而且這個口長在公開介面上，"
                     "不靠條件編譯不靠替身。",
    },
    {
        "tag": "設計 4",
        "title": "Vendored 規矩再現",
        "sections": [
            ("做法", "external/miniaudio/ 新子目錄、"
                    "附 README 記來源與 license——"
                    "沿用 tinygltf 立下的 vendored 規則。"),
            ("細節", "MINIAUDIO_IMPLEMENTATION 集中在 "
                    "MiniaudioImpl.cpp 單一 TU；"
                    "公開標頭用 opaque 指標，"
                    "不洩漏 miniaudio.h。"),
            ("啟示", "第二次引入第三方時，"
                    "規矩已經是檢查清單——"
                    "不是每次重新辯論。"),
        ],
        "code": 'external/miniaudio/ + README.md\n// impl 巨集只住 MiniaudioImpl.cpp\n// 公開標頭：opaque 指標不透型',
        "narration": "第四個設計：vendored 規矩再現。"
                     "miniaudio 以新增子目錄形式進入 external，"
                     "附來源與授權的 README——"
                     "完全是 tinygltf 當年立下的規則。"
                     "實作巨集集中在單一編譯單元，"
                     "公開標頭只用不透明指標，"
                     "第三方型別不透出介面。",
    },
    {
        "tag": "設計 5",
        "title": "缺檔記 failed 不重試",
        "sections": [
            ("做法", "LoadAudio 遇到缺檔或解碼失敗 → "
                    "回 nullptr 並把路徑記入 failed 集合，"
                    "後續請求不再重試。"),
            ("細節", "失敗是快取的判決不是暫時狀態——"
                    "每幀重試 IO 的失敗是最貴的失敗，"
                    "記住「試過了」比「再試一次」便宜。"),
            ("啟示", "降級鏈的每一級都單向——"
                    "壞掉的資產不會突然復活，"
                    "遊戲也不為它反覆付錢。"),
        ],
        "code": 'if (failed_.count(path)) return nullptr;\nauto* d = decode(path);\nif (!d) failed_.insert(path);  // 記住失敗',
        "narration": "第五個設計：缺檔記為失敗不再重試。"
                     "載入失敗的路徑進入失敗集合，"
                     "之後的請求直接回空——"
                     "每幀重試一次磁碟讀取是最貴的失敗方式。"
                     "記住「試過了」比「再試一次」便宜，"
                     "降級鏈每一級都是單向的。",
    },
    {
        "tag": "設計 6",
        "title": "語意音效層",
        "sections": [
            ("做法", "AudioCues 薄分派層：呼叫端說事件名 "
                    "(\"doctrine\"/\"seepage_up\"/\"ui_click\")，"
                    "音檔路徑由 Defaults 表決定。"),
            ("細節", "未知事件、無管理器、無裝置 → "
                    "一律 no-op；「音效是可選增強，"
                    "不參與遊戲邏輯成敗」寫在類註解。"),
            ("啟示", "呼叫端不認得路徑、"
                    "分派層不認得玩法——"
                    "語意層是兩邊的翻譯器。"),
        ],
        "code": 'cues.Play("doctrine");     // 事件名非路徑\ncues.SetSeepageLevel(2);  // 滲透音景\n// 無映射/無裝置 → no-op',
        "narration": "第六個設計：語意音效層。"
                     "呼叫端用事件名播放——學說觸發、"
                     "滲透上升、介面點擊——"
                     "音檔路徑由預設對照表決定。"
                     "未知事件、無管理器、無裝置一律空轉，"
                     "音效是可選增強這句話直接寫在類註解。"
                     "語意層是玩法和資產之間的翻譯器。",
    },
    {
        "tag": "設計 7",
        "title": "滲透聲先於形",
        "sections": [
            ("做法", "SetSeepageLevel(0/1/2) 驅動滲透音景："
                    "1 級低音景 bed 循環，"
                    "2 級疊高頻層——UX 決策"
                    "「滲透聲先於形」。"),
            ("細節", "神話層的音訊化早於視覺化——"
                    "D-1 的滲透序數越層後，"
                    "第一個消費者是耳朵不是眼睛。"),
            ("啟示", "UX 決策落成介面參數——"
                    "「聲先於形」不是文案，"
                    "是 SetSeepageLevel 的分層語義。"),
        ],
        "code": 'SetSeepageLevel(1);  // 低音景 bed\nSetSeepageLevel(2);  // 疊高頻層\n// 序數越界後的第一個消費者是耳朵',
        "narration": "第七個設計：滲透聲先於形。"
                     "滲透等級驅動音景分層——"
                     "一級是低音底床、二級疊上高頻層。"
                     "UX 文件的決策「滲透聲先於形」"
                     "落成介面參數："
                     "神話層的音訊化早於視覺化，"
                     "序數越界後第一個消費者是耳朵。",
    },
    {
        "tag": "設計 8",
        "title": "掃毒進引擎層",
        "sections": [
            ("做法", "Security/ContentScanner 新模組："
                    "EICAR 特徵、黑名單、啟發式規則、"
                    "隔離區——掃的是遊戲要載入的內容。"),
            ("細節", "ContentScannerTest 無頭可測、"
                    "連結 PotatoEngine——"
                    "資產管線的安全檢查變成引擎責任。"),
            ("啟示", "防線長在載入路徑上——"
                    "不是外掛掃毒軟體，"
                    "是引擎自己驗明正身。"),
        ],
        "code": 'ContentScanner scanner;\nscanner.Quarantine(path);   // 疑似內容隔離\nscanner.ScanFile(path);     // EICAR/黑名單/啟發式',
        "narration": "第八個設計：掃毒進了引擎層。"
                     "內容掃描器新模組——特徵比對、"
                     "黑名單、啟發式規則、隔離區，"
                     "掃的是遊戲要載入的內容本身。"
                     "測試無頭可跑、只連引擎庫——"
                     "資產管線的安全檢查，"
                     "正式變成引擎自己的責任。",
    },
    {
        "kind": "cover",
        "title": "第三十八集清單",
        "lines": ["介面換心一行 diff · 靜音照跑 · null 後端測試口",
                  "vendored 規矩再現 · 失敗記住不重試 · 語意分派",
                  "滲透聲先於形 · 掃毒進引擎層"],
        "narration": "第三十八集收在這裡。降級契約這條線"
                     "越收越齊：字體有回退鏈、設定有預設值、"
                     "霧標記有降級分支——"
                     "現在音訊有了靜音契約。"
                     "每個子系統都被問過同一句話："
                     "你不在的時候，遊戲還能跑嗎？",
    },
]

SLIDES_EP39 = [
    {
        "kind": "cover",
        "title": "幻影卡死勝利",
        "lines": ["從實戰學 C++ 第三十九集",
                  "D-4 審查輪 · 隱形隊死鎖 · dt 守衛 · "
                  "重 Arm 消散 · 帳序修正"],
        "narration": "第三十九集，入侵事件過審查。"
                     "上一集的神話入侵綠燈後被審出一串真 bug——"
                     "最狠的一個：被忽視的幻影會卡死勝利判定，"
                     "因為沒有人能攻擊看不見的隊伍。",
    },
    {
        "tag": "修正 1",
        "title": "看不見的隊伍卡死結局",
        "sections": [
            ("原碼", "忽視結局讓幻影留存——"
                    "「狐仙假訊被忽視就留著」"
                    "聽起來合理，實際是死鎖。"),
            ("問題", "幻影是不可被攻擊的隱形隊——"
                    "勝利判定等全場清空，"
                    "它永遠不清空，戰鬥永遠不結束。"),
            ("修法", "假訊任何結局都消散："
                    "先 fog->Reveal 再殲滅——"
                    "情報層造假不留實體卡判定。"),
        ],
        "code": '// 鬼軍：安撫才散（它是真的）\n// 假訊：任何結局都散\nif (ghost && (pacified ||\n    kind == FoxRumor)) { Reveal(); 殲滅; }',
        "narration": "第一個修正：看不見的隊伍卡死結局。"
                     "原本忽視結局讓幻影留存——"
                     "但幻影是不可被攻擊的隱形隊伍，"
                     "勝利判定等全場清空，它永遠不清空，"
                     "戰鬥就永遠不結束。"
                     "修法是假訊任何結局都消散——"
                     "情報層的造假，不留實體卡判定。",
    },
    {
        "tag": "修正 2",
        "title": "兩種入侵兩種死法",
        "sections": [
            ("鬼軍", "安撫才消散、忽視留存——"
                    "它是真的第三方，"
                    "你不管它它就站在那。"),
            ("假訊", "安撫忽視都消散——"
                    "幻影本是情報層造假，"
                    "結算即戳破，不論哪種戳法。"),
            ("啟示", "同一句 Resolve 底下，"
                    "兩種 kind 的死法寫成顯式條件——"
                    "語義差異不靠呼叫端記。"),
        ],
        "code": '// pacified || kind==FoxRumor\n// 鬼軍忽視留存（真實）\n// 假訊忽視也散（造假被戳破）',
        "narration": "第二個修正：兩種入侵兩種死法。"
                     "鬼軍安撫才散、忽視留存——它是真的。"
                     "假訊安撫忽視都散——幻影本是造假，"
                     "結算即戳破。"
                     "同一個解決函式底下，"
                     "語義差異寫成顯式條件，"
                     "不靠呼叫端自己記得。",
    },
    {
        "tag": "修正 3",
        "title": "dt 守衛擋暫停與毒",
        "sections": [
            ("問題", "pending 期間無條件 "
                    "windowLeft -= dt——暫停中餵 0 或"
                    "毒輸入餵 NaN 都會動窗口。"),
            ("修法", "只在 dt>0 且 isfinite 時累積——"
                    "暫停不逾時、NaN 不腐蝕窗口。"),
            ("啟示", "時間輸入要驗兩件事："
                    "方向（正）和品質（有限）——"
                    "NaN 過得了所有比較。"),
        ],
        "code": 'if (state.pending && dt > 0.0f &&\n    std::isfinite(dt)) {\n  windowLeft -= dt;\n}',
        "narration": "第三個修正：dt 守衛擋暫停與毒。"
                     "原本待決期間無條件扣窗口——"
                     "暫停中餵零、毒輸入餵 NaN 都會動。"
                     "現在只在正且有限的 dt 下累積："
                     "暫停不逾時、NaN 不腐蝕。"
                     "時間輸入要驗方向和品質兩件事——"
                     "NaN 過得了所有比較運算。",
    },
    {
        "tag": "修正 4",
        "title": "重 Arm 先收屍",
        "sections": [
            ("問題", "重 Arm 只重置 state——"
                    "上一場的鬼隊還活著，"
                    "兩場的幻影疊在同一個 battle。"),
            ("修法", "Arm 開頭先消散舊鬼隊、"
                    "清 ghost/fog/ghostEid——"
                    "重綁=新場域的語義寫進實作。"),
            ("啟示", "註解同步立契約："
                    "「一次 Arm 對應一場 battle——"
                    "battle 必須活得比解決窗口久」。"),
        ],
        "code": 'void Arm(...) {\n  if (ghost && !ghost->IsEliminated())\n    ghost->ApplyCasualties(全員); // 收屍\n  ghost=nullptr; fog=nullptr; eid=-1;\n}',
        "narration": "第四個修正：重綁先收屍。"
                     "原本重 Arm 只重置狀態結構，"
                     "上一場的鬼隊還活著——"
                     "兩場戰的幻影疊在一起。"
                     "現在 Arm 開頭先消散舊鬼隊、"
                     "清空指標和霧實體——"
                     "重綁等於新場域，語義寫進實作，"
                     "生命週期契約寫進註解。",
    },
    {
        "tag": "修正 5",
        "title": "帳序要扛再入",
        "sections": [
            ("問題", "onIncursion 先於 onEvent 字串——"
                    "回呼裡再入 Resolve 時，"
                    "帳本先記結算才記觸發。"),
            ("修法", "交換順序：入帳字串先派出——"
                    "再入結算時 ledger 仍保持"
                    "「觸發→結算」次序。"),
            ("啟示", "回呼順序是帳序——"
                    "多個回呼的相對次序，"
                    "要按「讀者翻帳的順序」排。"),
        ],
        "code": 'onEvent("入侵:"+...);   // 先落字串\nonIncursion({...});        // 再發事件\n// 回呼再入 Resolve 時帳序仍正確',
        "narration": "第五個修正：帳序要扛再入。"
                     "原本事件回呼先於帳本字串——"
                     "回呼裡再入解決時，"
                     "帳本會先記結算才記觸發。"
                     "交換順序之後，"
                     "再入路徑的帳序仍然正確。"
                     "多個回呼的相對次序，"
                     "要按讀者翻帳的順序排。",
    },
    {
        "tag": "修正 6",
        "title": "指標用畢即棄",
        "sections": [
            ("做法", "Settle 結尾 ghost=nullptr——"
                    "結算後不再持指，"
                    "防殲滅隊的懸空引用。"),
            ("契約", "Update 註解新增呼叫端契約："
                    "勿在 battle 的 Emit/Update 回呼內"
                    "呼叫——CreateSquad 的 push_back "
                    "會讓迭代懸空。"),
            ("啟示", "防不住的寫成契約、"
                    "防得住的寫成程式——"
                    "UAF 兩條防線各管一段。"),
        ],
        "code": 'ghost->ApplyCasualties(...);\nghost = nullptr;  // 防 UAF\n// 註解：勿在 battle 回呼內呼叫 Update',
        "narration": "第六個修正：指標用畢即棄。"
                     "結算後鬼隊指標立刻清空，"
                     "防殲滅隊的懸空引用。"
                     "而 Update 的註解補上呼叫端契約："
                     "不要在戰鬥回呼內驅動——"
                     "生成小隊的向量擴張會讓迭代懸空。"
                     "防得住的寫成程式，"
                     "防不住的寫成契約。",
    },
    {
        "tag": "修正 7",
        "title": "字串解析只抄一份",
        "sections": [
            ("做法", "新增 ParseIncursionKind 共用解析器——"
                    "章節定義和呼叫端不再各抄一份 "
                    "\"ghost_legion\" 字面值。"),
            ("配套", "ChapterLibrary 驗 "
                    "incursion.seepage 須為 1-3 整數——"
                    "rawS != floor(rawS) 拒掉 2.5。"),
            ("啟示", "字串越界的地方立共用閘門——"
                    "接線字面值集中一處，"
                    "錯字只會錯在一個地方。"),
        ],
        "code": 'bool ParseIncursionKind(s, out);\n// "ghost_legion"/"fox_rumor" 集中一處\n// seepage=2.5 → 非整數拒絕+警告',
        "narration": "第七個修正：字串解析只抄一份。"
                     "新增共用解析器，"
                     "章節定義和呼叫端不再各抄字面值。"
                     "配套是章節庫驗證滲透閾值"
                     "必須是一到三的整數——"
                     "二點五被地板檢查拒掉。"
                     "字串越界的地方立共用閘門，"
                     "錯字只會錯在一個地方。",
    },
    {
        "tag": "修正 8",
        "title": "真相與呈現分層",
        "sections": [
            ("做法", "state.spirit 存原始名（可為空），"
                    "「境靈」降級名只進顯示字串——"
                    "OutcomeEvent 如實回傳原始值。"),
            ("測試", "斷言 out.spirit==\"橋姬\"——"
                    "結局回傳的不是顯示名是帳本名。"),
            ("啟示", "空值正規化是呈現層的事——"
                    "帳本存真相，"
                    "「顯示給誰看」不污染「記了什麼」。"),
        ],
        "code": 'state.spirit = spirit;  // 存原始名\n// 顯示字串才用 display =\n//   empty ? "境靈" : spirit',
        "narration": "第八個修正：真相與呈現分層。"
                     "狀態存原始靈名——可能為空，"
                     "「境靈」這個降級顯示名只進字串，"
                     "結局事件如實回傳原始值。"
                     "空值正規化是呈現層的事——"
                     "帳本存真相，"
                     "顯示給誰看不污染記了什麼。",
    },
    {
        "kind": "cover",
        "title": "第三十九集清單",
        "lines": ["隱形隊死鎖 · 兩種死法 · dt 正且有限",
                  "重 Arm 收屍 · 帳序扛再入 · 指標即棄",
                  "解析集中 · 真相與呈現分層"],
        "narration": "第三十九集收在這裡。入侵的審查輪"
                     "抓到八個修正——最狠的是一個"
                     "沒寫在任何規格裡的死鎖："
                     "看不見的敵人，讓勝利永遠不來。"
                     "審查檔案攢到第四份，"
                     "每一份都在回答同一句話："
                     "綠燈之後，誰還在看？",
    },
]

SLIDES_EP40 = [
    {
        "kind": "cover",
        "title": "資訊冗餘可裁",
        "lines": ["從實戰學 C++ 第四十集",
                  "F-3 HUD 密度 · 門檻矩陣 · 恆在件不列舉 · "
                  "面板層級裁撤"],
        "narration": "第四十集，HUD 學會少說話。"
                     "F-3 給介面加了三檔密度——"
                     "但真正有趣的是裁撤的方式："
                     "關鍵情報的保證，不是靠檢查，"
                     "是靠不列舉。",
    },
    {
        "tag": "設計 1",
        "title": "門檻矩陣一頁看完",
        "sections": [
            ("做法", "Feature 枚舉 × Density 序數——"
                    "HUDShown(feature, density) 是全部裁撤"
                    "邏輯的單一來源。"),
            ("清單", "EventLog/Minimap/EnemyDossier/"
                    "SquadDetail/HelpHints 需 Standard；"
                    "FogIntel/ExecStats 僅 Verbose。"),
            ("啟示", "密度政策是一張表不是一灘 if——"
                    "七個功能、兩段門檻，"
                    "檔頭註解一頁說完。"),
        ],
        "code": 'enum class Feature {\n  EventLog, Minimap, EnemyDossier,\n  SquadDetail, HelpHints,\n  FogIntel, ExecStats, Count\n};',
        "narration": "第一個設計：門檻矩陣一頁看完。"
                     "功能枚舉乘上密度序數，"
                     "一個 HUDShown 就是全部裁撤邏輯。"
                     "七個功能兩段門檻——"
                     "前五項標準檔就開，"
                     "霧情報和執行統計只有詳盡檔才給。"
                     "密度政策是一張表，"
                     "不是一灘散落的條件判斷。",
    },
    {
        "tag": "設計 2",
        "title": "恆在件不列舉",
        "sections": [
            ("做法", "階段/CP/時鐘、全軍控制面、"
                    "結算視窗沒有枚舉值——"
                    "它們不經閘門。"),
            ("註解", "「關鍵情報三檔恆在，"
                    "由不列舉保證」——"
                    "恆在不是檢查結果，"
                    "是結構性不存在閘門。"),
            ("啟示", "最強的不變量是「無法表達違反」——"
                    "想裁關鍵件？先新增枚舉值，"
                    "而新增會過審查的眼睛。"),
        ],
        "code": '// 恆在件沒有枚舉值\n// 「關鍵情報三檔恆在\n//   由不列舉保證」\n// 想裁它？先過審查',
        "narration": "第二個設計：恆在件不列舉。"
                     "階段、指揮點、時鐘、控制面、"
                     "結算視窗——沒有枚舉值，不經閘門。"
                     "關鍵情報三檔恆在這句話，"
                     "由不列舉保證："
                     "恆在不是檢查的結果，"
                     "是結構上不存在閘門。"
                     "想裁它？得先新增枚舉值——"
                     "而新增會過審查的眼睛。",
    },
    {
        "tag": "設計 3",
        "title": "裁面板不裁行",
        "sections": [
            ("做法", "每個 Feature 對應一個面板或"
                    "一組同質資訊行——"
                    "密度閘門包整個 ImGui 視窗區塊。"),
            ("對比", "逐行裁撤會讓密度語義"
                    "散進每個 Text 呼叫——"
                    "維護時無法回答"
                    "「精簡檔長什麼樣」。"),
            ("啟示", "裁撤粒度=理解粒度——"
                    "玩家感知的是面板的有無，"
                    "不是行數的增減。"),
        ],
        "code": 'if (HUDShown(Feature::EventLog, hd)) {\n  ImGui::Begin("戰況", ...);\n  ...\n  ImGui::End();\n}  // 整個面板進出',
        "narration": "第三個設計：裁面板不裁行。"
                     "每個功能枚舉對應一個面板"
                     "或一組同質資訊行——"
                     "閘門包住整個視窗區塊。"
                     "逐行裁撤會讓密度語義散進每個文字呼叫，"
                     "之後沒人答得出精簡檔長什麼樣。"
                     "裁撤粒度等於理解粒度："
                     "玩家感知的是面板的有無。",
    },
    {
        "tag": "設計 4",
        "title": "兩段門檻用序數比",
        "sections": [
            ("做法", "need = FogIntel/ExecStats ? 2 : 1；"
                    "回傳 (int)density >= need——"
                    "密度序數直接當門檻數值。"),
            ("細節", "枚舉序數就是語義序數："
                    "Minimal=0 < Standard=1 < Verbose=2，"
                    "「至少要哪檔」是比較不是映射表。"),
            ("啟示", "序數設計讓閘門是一行——"
                    "枚舉值排成有意義的順序，"
                    "比較運算自己就是規則。"),
        ],
        "code": 'const int need = (f==FogIntel ||\n                  f==ExecStats) ? 2 : 1;\nreturn (int)d >= need;  // 序數即門檻',
        "narration": "第四個設計：兩段門檻用序數比。"
                     "詳盡專屬的兩項要二，其餘要一——"
                     "回傳就是密度序數大於等於門檻。"
                     "枚舉值排成有意義的順序，"
                     "比較運算自己就是規則，"
                     "不需要映射表。",
    },
    {
        "tag": "設計 5",
        "title": "序數鉗制共用守衛",
        "sections": [
            ("做法", "DensityFromInt 把任意 int 鉗回 "
                    "0-2——設定檔值、外部輸入、"
                    "未來 UI 都走同一個閘。"),
            ("配套", "UISettings 的 hudDensity 是"
                    "可選欄位：舊檔沒有 → Standard；"
                    "越界 → 鉗回——schema 演化不炸檔。"),
            ("啟示", "clamp 在邊界不在使用點——"
                    "讀進來就合法，"
                    "下游不用再問「這數字可信嗎」。"),
        ],
        "code": 'DensityFromInt(int v) {\n  if (v<0) return Minimal;\n  if (v>2) return Verbose;  // 鉗回\n}\n// 舊檔無欄位 → Standard',
        "narration": "第五個設計：序數鉗制共用守衛。"
                     "整數轉密度把任意值鉗回零到二——"
                     "設定檔、外部輸入都走同一個閘。"
                     "設定檔的密度欄位是可選的："
                     "舊檔沒有就標準檔，越界就鉗回。"
                     "鉗制在邊界不在使用點——"
                     "讀進來就合法。",
    },
    {
        "tag": "設計 6",
        "title": "測試驗矩陣不驗像素",
        "sections": [
            ("做法", "HUDDensityTest 同 UIScaleTest 配方——"
                    "imgui 四個編譯單元直接進 target，"
                    "驗門檻矩陣和序數鉗制。"),
            ("斷言", "七功能 × 三密度的全矩陣、"
                    "DensityFromInt 邊界、"
                    "settings 持久化 roundtrip。"),
            ("啟示", "UI 測試的可測面是資料面——"
                    "「這功能這密度顯不顯示」"
                    "是布林不是像素。"),
        ],
        "code": '// 7 features × 3 densities 全矩陣\nCheck(!HUDShown(EventLog, Minimal));\nCheck(HUDShown(FogIntel, Verbose));\nCheck(!HUDShown(FogIntel, Standard));',
        "narration": "第六個設計：測試驗矩陣不驗像素。"
                     "沿用縮放測試的配方——"
                     "imgui 編譯單元直接進測試目標，"
                     "驗七功能乘三密度的全矩陣、"
                     "序數鉗制的邊界、設定的往返。"
                     "介面測試的可測面是資料面——"
                     "顯不顯示是布林不是像素。",
    },
    {
        "tag": "設計 7",
        "title": "同步腳本跟著長",
        "sections": [
            ("做法", "sync 白名單補 MiniaudioBackendTest "
                    "和 external/miniaudio——"
                    "F-4 的產物跨 repo 對齊。"),
            ("細節", "上游 PotatoEngine 同批註冊"
                    " miniaudio 私有 include 與測試——"
                    "兩個 repo 的 CMake 同步演化。"),
            ("啟示", "白名單同步的代價顯形——"
                    "每次落地都要記得餵清單，"
                    "但漏了可補、多了難收。"),
        ],
        "code": 'for dep in glad_gen tinygltf miniaudio\nEXAMPLES="... MiniaudioBackendTest ..."\n# 上游 CMake 同批註冊',
        "narration": "第七個設計：同步腳本跟著長。"
                     "白名單補上後端測試和 miniaudio 依賴，"
                     "上游 repo 同批註冊私有引用路徑和測試。"
                     "白名單同步的代價顯形——"
                     "每次落地都要記得餵清單，"
                     "但漏了可以補，多了難收。",
    },
    {
        "tag": "設計 8",
        "title": "設定頁的最後一塊",
        "sections": [
            ("做法", "標題頁加「HUD 密度」三檔下拉——"
                    "settingsDirty 才落盤、"
                    "放開控制項才原子寫。"),
            ("定型", "F-2 的持久化配方直接複用："
                    "tmp+rename、拖曳中不寫、"
                    "收起頁面也照存。"),
            ("啟示", "第二個設定項走的是"
                    "第一個鋪好的路——"
                    "UX 基礎設施的邊際成本在遞減。"),
        ],
        "code": 'ImGui::Combo("HUD 密度", &hd,\n             "精簡\\0標準\\0詳盡\\0");\n// settingsDirty→放開才 tmp+rename',
        "narration": "第八個設計：設定頁的最後一塊。"
                     "標題頁加上密度三檔下拉，"
                     "沿用縮放滑桿鋪好的持久化配方——"
                     "髒標記、放開才寫、暫存檔原子替換。"
                     "第二個設定項走的是第一個鋪好的路，"
                     "體驗基礎設施的邊際成本在遞減。",
    },
    {
        "kind": "cover",
        "title": "第四十集清單",
        "lines": ["門檻矩陣 · 恆在件不列舉 · 裁面板不裁行",
                  "序數即門檻 · 邊界鉗制 · 驗矩陣不驗像素",
                  "同步餵清單 · 設定項走舊路"],
        "narration": "第四十集收在這裡。F 系列四件全落地："
                     "縮放管「多大」、字體管「真假」、"
                     "音訊管「有沒有」、密度管「說多少」——"
                     "玩家體驗的四個旋鈕，"
                     "每一個背後都是同一套工程："
                     "閘門集中、降級單向、缺省安全。",
    },
]

SLIDES_EP41 = [
    {
        "kind": "cover",
        "title": "帳補在帳後",
        "lines": ["從實戰學 C++ 第四十一集",
                  "spec-d4 done · 七項發現三類裁決 · "
                  "觀測方語義 · 補帳 commit"],
        "narration": "第四十一集，帳本的最後一頁。"
                     "D-4 規格標 done——附的不是喜報，"
                     "是一份七項發現的修正案，"
                     "和一個專門修註冊漏帶的補帳提交。"
                     "做完和說完之間，還有一道工序。",
    },
    {
        "tag": "設計 1",
        "title": "三段式裁決成慣例",
        "sections": [
            ("格式", "觸發（七項發現逐條列）/"
                    "修訂（對應修法）/KEEP（看過不改）——"
                    "D-3 首創，D-4 直接沿用。"),
            ("意義", "Change Log 讓規格有修改史——"
                    "審查不是附錄，"
                    "是規格自身的一部分。"),
            ("啟示", "第二次用同格式就是慣例——"
                    "好格式不靠推廣，"
                    "靠下一份文件自動長出來。"),
        ],
        "code": '## Spec Change Log\n- 觸發: (1)...(7) 七項發現\n- 修訂: 對應修法\n- KEEP: 看過不改的清單',
        "narration": "第一個設計：三段式裁決成慣例。"
                     "觸發列七項發現、修訂對應修法、"
                     "KEEP 寫看過不改——"
                     "這是天命兌換首創的格式，"
                     "神話入侵直接沿用。"
                     "好格式不靠推廣，"
                     "靠下一份文件自動長出來。",
    },
    {
        "tag": "設計 2",
        "title": "觀測方不是敵方",
        "sections": [
            ("發現", "假雲以 team=1 註冊——"
                    "但 QuantumFog 的 team 參數是"
                    "**觀測方**語義，不是陣營。"),
            ("後果", "觀測行動扣錯陣營的情報——"
                    "玩家看假雲，扣的是敵方情報點，"
                    "計費方向整個反了。"),
            ("啟示", "參數名騙人——叫 team 的不一定"
                    "是「誰的隊」；語義註解"
                    "沒寫的地方，猜錯是遲早的。"),
        ],
        "code": 'AddEntityCloud(name, /*觀測方=*/0, ...);\n// 不是 enemyTeam=1——\n// team 參數是「誰在觀測」',
        "narration": "第二個設計：觀測方不是敵方。"
                     "假雲原本用陣營一註冊——"
                     "但迷霧系統的隊伍參數是觀測方語義，"
                     "玩家觀測假雲扣的卻是敵方情報點，"
                     "計費方向整個反了。"
                     "參數名會騙人：叫 team 的"
                     "不一定是誰的隊。",
    },
    {
        "tag": "設計 3",
        "title": "「境靈」會鑄幻影錢包",
        "sections": [
            ("發現", "state.spirit 存降級顯示名「境靈」"
                    "——OutcomeEvent 回傳後，"
                    "下游會把它當真靈名入帳。"),
            ("連鎖", "D-3 的幻影錢包換了張臉回來："
                    "顯示名一旦跨層當真名，"
                    "「查無此神」又會收費。"),
            ("修法", "存原始名（可為空），"
                    "降級名只進顯示字串——"
                    "真相與呈現分層（EP39 已述）。"),
        ],
        "code": '// 下游拿「境靈」去 MythLayer 查\n// → 查無此神 → 若無 HasSpirit\n//   又是幻影錢包\nstate.spirit = spirit;  // 存原始名',
        "narration": "第三個設計：境靈會鑄幻影錢包。"
                     "降級顯示名一旦進了狀態欄，"
                     "結局事件回傳後下游會當真靈名——"
                     "天命兌換抓過的幻影錢包，"
                     "換了張臉從下游回來。"
                     "同一個 bug 家族跨史詩重現，"
                     "這次在源頭就分層："
                     "帳本存真相，顯示名只進字串。",
    },
    {
        "tag": "設計 4",
        "title": "漏帶的補帳 commit",
        "sections": [
            ("事發", "D-4 落地後發現 CMake 註冊"
                    "沒進帳——平行 session 的 "
                    "index 競態把註冊 hunk 漏了。"),
            ("修法", "專門一個 build commit 補回："
                    "「D-4 CMake 註冊補回——"
                    "平行 session index 競態漏帶」，"
                    "訊息連原因都寫。"),
            ("啟示", "AGENTS.md 的平行衛生條款"
                    "不是假設情境——hot zone 檔案"
                    "的漏帶要補得快、補得有名。"),
        ],
        "code": '07bcb70 build(gameplay):\n  D-4 CMake 註冊補回——\n  平行 session index 競態漏帶',
        "narration": "第四個設計：漏帶的補帳提交。"
                     "入侵落地後發現建置註冊沒進帳——"
                     "平行工作階段的索引競態"
                     "把註冊區塊漏掉了。"
                     "修法是專門一個建置提交補回，"
                     "訊息連原因都寫清楚。"
                     "平行衛生條款不是假設情境——"
                     "熱區檔案的漏帶，"
                     "要補得快、補得有名。",
    },
    {
        "tag": "設計 5",
        "title": "KEEP 清單的立場",
        "sections": [
            ("內容", "回呼翻轉依賴、team=2 第三方、"
                    "無 doctrine 的 Hold、一次性 latch、"
                    "warnings 降級——五項看過不改。"),
            ("意義", "KEEP 是裁決不是省略——"
                    "「看過決定保留」和「沒看過」"
                    "在帳面上長得一樣，"
                    "靠清單區分。"),
            ("啟示", "保留項寫出來，"
                    "下次審查才知道這些是"
                    "有意為之不是漏看。"),
        ],
        "code": 'KEEP: 回呼翻轉依賴、team=2、\n      無 doctrine Hold、一次性 latch、\n      warnings 降級\n// 「看過不改」也是結論',
        "narration": "第五個設計：KEEP 清單的立場。"
                     "五項看過不改：回呼依賴、第三方陣營、"
                     "無學說的持守、一次性觸發、警告降級。"
                     "看過決定保留和根本沒看過，"
                     "在帳面上長得一樣——靠清單區分。"
                     "保留項寫出來，"
                     "下次審查才知道是有意為之。",
    },
    {
        "tag": "設計 6",
        "title": "狀態的三處落點",
        "sections": [
            ("現況", "spec 標 done、佇列標 done"
                    "（附成績單）、sprint-status 還在 "
                    "review——同一件事三份檔案。"),
            ("代價", "狀態分散在多份追蹤檔，"
                    "同步是手工的——"
                    "每一份落點記的時間都不一樣。"),
            ("啟示", "多源狀態的代價換來分層視角："
                    "佇列記成績、規格記結論、"
                    "衝刺記節奏——不同檔案答不同問題。"),
        ],
        "code": 'spec:   done + Change Log\nqueue:  done + 50/50 雙工具鏈\nsprint: review  ← 還沒追平',
        "narration": "第六個設計：狀態的三處落點。"
                     "規格標完成、佇列標完成附成績單、"
                     "衝刺狀態還停在審查——"
                     "同一件事三份檔案，"
                     "同步是手工的。"
                     "但分散換來分層視角："
                     "佇列記成績、規格記結論、"
                     "衝刺記節奏——不同檔案答不同問題。",
    },
    {
        "tag": "設計 7",
        "title": "導讀第二例",
        "sections": [
            ("做法", "Review Order 按關注點分四群："
                    "觸發與生成/結算/章節定義/測試，"
                    "每站附 file:line。"),
            ("沿用", "D-1 首創的導讀格式——"
                    "審查者不用從 diff 推結構，"
                    "按設計順序讀。"),
            ("啟示", "導讀是給未來讀者的地圖——"
                    "六個月後的審查者，"
                    "和今天的審查者一樣需要它。"),
        ],
        "code": 'Suggested Review Order\n  觸發與生成 → MythIncursion.cpp:60\n  結算       → MythIncursion.cpp:133\n  章節定義   → ChapterLibrary.cpp:180',
        "narration": "第七個設計：導讀第二例。"
                     "審查順序按關注點分四群，"
                     "每站附檔案行號——"
                     "審查者不用從差異推結構，"
                     "按設計順序讀。"
                     "導讀是給未來讀者的地圖："
                     "六個月後的審查者，"
                     "和今天的一樣需要它。",
    },
    {
        "tag": "設計 8",
        "title": "50/50 的分母",
        "sections": [
            ("成績", "佇列附註：「三路審查補丁後 "
                    "50/50 雙工具鏈」——"
                    "審查補丁也算進測試總數。"),
            ("細節", "測試數隨審查輪成長——"
                    "每個發現都變成新斷言，"
                    "分母自己記錄了審查的產出。"),
            ("啟示", "成績單寫在佇列不是簡報——"
                    "驗證數字跟著工作項走，"
                    "追溯時不用找另一份檔案。"),
        ],
        "code": 'status: done\nnote: 三路審查補丁後 50/50 雙工具鏈\n# 分母成長史=審查產出史',
        "narration": "第八個設計：五十個斷言的分母。"
                     "佇列附註三路審查補丁後雙工具鏈全綠——"
                     "審查補丁也算進測試總數，"
                     "每個發現都變成新斷言。"
                     "分母自己記錄了審查的產出，"
                     "驗證數字跟著工作項走，"
                     "追溯時不用找另一份檔案。",
    },
    {
        "kind": "cover",
        "title": "第四十一集清單",
        "lines": ["三段式裁決成慣例 · 觀測方語義 · 顯示名鑄錢包",
                  "漏帶補帳 · KEEP 即裁決 · 狀態三落點",
                  "導讀第二例 · 分母記審查"],
        "narration": "第四十一集收在這裡。Epic D 四件全落地，"
                     "審查檔案攢到四份——"
                     "每一份的收法都一樣："
                     "發現具名、修法對應、保留署名。"
                     "剩下 D-5 視覺消費端——"
                     "神話層的最後一哩，"
                     "是讓玩家真的看見它。",
    },
]

SLIDES_EP42 = [
    {
        "kind": "cover",
        "title": "先聲後形",
        "lines": ["從實戰學 C++ 第四十二集",
                  "D-5 spec draft · 等級→呈現映射 · "
                  "指令包 · scrim 過渡"],
        "narration": "第四十二集，神話層的最後一哩開規格。"
                     "滲透等級落地四集了，"
                     "玩家至今看不見它——"
                     "D-5 要給等級配一張臉，"
                     "而規格的第一個決定是："
                     "呈現層是一個純函式。",
    },
    {
        "tag": "設計 1",
        "title": "呈現層是純函式",
        "sections": [
            ("做法", "SeepageStage 吃 int 滲透序數、"
                    "吐 SeepageVisual 指令包——"
                    "不讀 Campaign、不碰渲染器。"),
            ("隔離", "MythIncursion 同款型別隔離——"
                    "序數越界、enum 留下，"
                    "Gameplay 層第二次用這招。"),
            ("啟示", "「等級該長什麼樣」是純映射——"
                    "純函式讓視效決策本身"
                    "可以無頭驗證。"),
        ],
        "code": 'SeepageVisual SeepageStage(int level);\n// int 進、指令包出\n// 不讀 Campaign 不碰 renderer',
        "narration": "第一個設計：呈現層是純函式。"
                     "滲透舞台吃整數序數、"
                     "吐視覺指令包——不讀戰役層、"
                     "不碰渲染器。"
                     "和入侵事件同款的型別隔離，"
                     "玩法層第二次用這招。"
                     "等級該長什麼樣是純映射，"
                     "純函式讓視效決策本身可以無頭驗證。",
    },
    {
        "tag": "設計 2",
        "title": "三檔漸進是戲劇結構",
        "sections": [
            ("等級 1", "局部異常——霧色偏移，"
                    "世界開始不對勁但說不上哪裡。"),
            ("等級 2", "聲先於形+物件微移——"
                    "音景先報、視覺跟上，"
                    "UX「滲透聲先於形」落實。"),
            ("等級 3", "全主題切換——token swap "
                    "加 scrim 過渡，"
                    "整個介面換成神話層的語域。"),
        ],
        "code": 'L1: 霧色偏移     局部異常\nL2: 音景+微移    聲先於形\nL3: 主題切換     全面降臨',
        "narration": "第二個設計：三檔漸進是戲劇結構。"
                     "一級局部異常，霧色偏移，"
                     "世界不對勁但說不上哪裡。"
                     "二級聲先於形——音景先報、視覺跟上，"
                     "UX 的滲透聲先於形落實成順序。"
                     "三級全主題切換，"
                     "整個介面換成神話層的語域——"
                     "滲透不是一個開關，是三幕戲。",
    },
    {
        "tag": "設計 3",
        "title": "指令包不是直接畫",
        "sections": [
            ("做法", "SeepageVisual 是資料：霧色偏移、"
                    "微移幅度、音景等級、主題覆寫、"
                    "scrim 曲線——消費端自己決定怎麼用。"),
            ("對比", "直接畫的呈現層測不了——"
                    "指令包讓「等級 2 該輸出什麼」"
                    "成為斷言對象。"),
            ("啟示", "輸出資料不輸出副作用——"
                    "渲染層唯讀的保證，"
                    "從介面形狀開始。"),
        ],
        "code": 'struct SeepageVisual {\n  float fogTintDrift;   // 霧色偏移\n  float microShift;     // 微移幅度\n  int   soundscape;     // 音景等級\n  ThemeId override;     // 主題覆寫\n  ScrimCurve scrim;     // 過渡曲線\n};',
        "narration": "第三個設計：指令包不是直接畫。"
                     "視覺指令包是資料——霧色偏移、微移幅度、"
                     "音景等級、主題覆寫、過渡曲線，"
                     "消費端自己決定怎麼用。"
                     "直接畫的呈現層測不了，"
                     "指令包讓等級該輸出什麼成為斷言對象。"
                     "輸出資料不輸出副作用，"
                     "渲染層唯讀從介面形狀開始保證。",
    },
    {
        "tag": "設計 4",
        "title": "scrim 走 draw-list 不走 RTT",
        "sections": [
            ("做法", "過渡用 ForegroundDrawList "
                    "全屏矩形——DuanqiaoPlayable "
                    "themeFade 同款先例，無離屏目標。"),
            ("界線", "Never 清單明寫：不做 shader/"
                    "sprite 特效——draw-list 是上限，"
                    "墨暈湍流貼圖不進這故事。"),
            ("啟示", "效果預算寫在規格裡——"
                    "「夠用」是有意裁決，"
                    "不是技術上限。"),
        ],
        "code": '// ForegroundDrawList 全屏 rect\n// themeFade 同款先例\n// Never: shader/sprite 特效',
        "narration": "第四個設計：scrim 走繪製清單"
                     "不走離屏渲染。全屏矩形蓋住再揭開，"
                     "主題淡出的同款先例，"
                     "不需要渲染目標。"
                     "Never 清單明寫不做著色器特效——"
                     "繪製清單是上限，夠用是有意裁決。",
    },
    {
        "tag": "設計 5",
        "title": "減動效是分支不是開關",
        "sections": [
            ("做法", "motion-reduction 開啟時過渡"
                    "降為短 crossfade——scrim 時長驟減、"
                    "無漸變演出，功能不缺席。"),
            ("細節", "走 UISettings 可選新欄——"
                    "舊檔預設降級，"
                    "schema 語義不變。"),
            ("啟示", "無障礙是效果的分支——"
                    "減動效不是關掉過渡，"
                    "是換一種過渡。"),
        ],
        "code": '// motion-reduction → 短 crossfade\n// scrim 時長驟減、無漸變演出\n// 可選欄位、舊檔降級',
        "narration": "第五個設計：減動效是分支不是開關。"
                     "減動效開啟時，過渡降為短交叉淡化——"
                     "時長驟減、沒有漸變演出，"
                     "但功能不缺席。"
                     "走設定的可選新欄位，舊檔降級。"
                     "無障礙不是把效果關掉，"
                     "是換一種效果。",
    },
    {
        "tag": "設計 6",
        "title": "變化才過渡",
        "sections": [
            ("做法", "等級變化才觸發過渡——"
                    "同等級重餵不重播；"
                    "scrim alpha 時序由過渡狀態機管。"),
            ("對比", "無差分驅動會讓每幀重播——"
                    "過渡是事件不是狀態，"
                    "狀態機只認邊緣。"),
            ("啟示", "動畫觸發要邊緣檢測——"
                    "level changed 是條件，"
                    "level 本身是輸入。"),
        ],
        "code": 'if (level != lastLevel) {\n  transition.Begin(curve);\n  lastLevel = level;\n}\n// 同等級重餵不重播',
        "narration": "第六個設計：變化才過渡。"
                     "等級改變才觸發過渡，"
                     "同等級重餵不重播——"
                     "過渡是事件不是狀態，"
                     "狀態機只認邊緣。"
                     "動畫觸發要邊緣檢測："
                     "等級改變是條件，等級本身是輸入。",
    },
    {
        "tag": "設計 7",
        "title": "耳朵比眼睛先到位",
        "sections": [
            ("巧合", "F-4 先落地——"
                    "AudioCues::SetSeepageLevel 已在庫，"
                    "D-5 的音景消費端是現成的。"),
            ("順序", "UX「滲透聲先於形」"
                    "在落地順序上成真："
                    "聲音管線比視覺消費端先存在。"),
            ("啟示", "介面先行讓後到者撿現成——"
                    "音景等級欄位在指令包裡，"
                    "只是接上已在的口。"),
        ],
        "code": 'SeepageVisual.soundscape →\n  AudioCues::SetSeepageLevel(level)\n// F-4 的口，D-5 的訊號',
        "narration": "第七個設計：耳朵比眼睛先到位。"
                     "音訊里程碑先落地，"
                     "滲透音景介面已在庫——"
                     "D-5 的音景消費端是現成的。"
                     "滲透聲先於形這句 UX 文案，"
                     "在落地順序上成真："
                     "聲音管線比視覺消費端先存在。",
    },
    {
        "tag": "設計 8",
        "title": "Ask First 有標價",
        "sections": [
            ("問題 1", "Manifest 目標主題——"
                    "預設切 InkChronicle（史卷語域），"
                    "要換主題或做變體 token "
                    "需使用者裁決。"),
            ("問題 2", "ChapterDef 要不要正式 "
                    "region 欄位——目前用章節地圖名"
                    "當 MythLayer 區域鍵的慣例。"),
            ("啟示", "Ask First 清單寫具體情境——"
                    "不是「不確定就問」，"
                    "是點名哪兩個決定要人來做。"),
        ],
        "code": 'Ask First:\n- L3 目標主題: InkChronicle 預設?\n- ChapterDef.region 正式欄位?\n// 點名的決定，不是泛指的不確定',
        "narration": "第八個設計：先問清單有標價。"
                     "兩個點名的裁決：顯化級的目標主題"
                     "預設史卷語域對不對、"
                     "章節定義要不要正式的區域欄位。"
                     "Ask First 不是「不確定就問」——"
                     "是點名哪兩個決定要人來做，"
                     "問題本身就是設計文件。",
    },
    {
        "kind": "cover",
        "title": "第四十二集清單",
        "lines": ["純函式呈現層 · 三檔戲劇結構 · 指令包",
                  "draw-list 上限 · 減動效分支 · 邊緣觸發",
                  "聲先於形成真 · Ask First 點名"],
        "narration": "第四十二集收在這裡。Epic D 的故事"
                     "即將走完：壓力、神社、兌換、入侵、"
                     "最後是給滲透一張臉。"
                     "而這集的規格本身在示範一件事——"
                     "連「好不好看」都可以先寫成"
                     "純函式和指令包再開工。",
    },
]

SLIDES_EP43 = [
    {
        "kind": "cover",
        "title": "指令包的刻度",
        "lines": ["從實戰學 C++ 第四十三集",
                  "D-5 SeepageStage 落地 · scrim 三拍曲線 · "
                  "升才過渡 · 數值表即腳本"],
        "narration": "第四十三集，滲透長出臉了。"
                     "上一集規格裡的指令包，"
                     "這集填上數值——scrim 曲線的三拍、"
                     "每級的偏移量，全是有名字的常數。",
    },
    {
        "tag": "設計 1",
        "title": "scrim 的三拍曲線",
        "sections": [
            ("結構", "ScrimCurve{attack, hold, release, peak}——"
                    "淡入→峰值停→淡出，"
                    "過渡形狀本身是資料。"),
            ("兩檔", "kNormal{0.35,0.25,0.90,0.55} 戲劇化；"
                    "kReduced{0.05,0,0.12,0.40} "
                    "只留短 crossfade——constexpr 表切換。"),
            ("啟示", "動效可及性不是刪效果是換曲線——"
                    "同一個 ScrimAlpha 函式，"
                    "換一組常數換一種脾氣。"),
        ],
        "code": 'constexpr ScrimCurve kNormal {0.35,0.25,0.90,0.55};\nconstexpr ScrimCurve kReduced{0.05,0.00,0.12,0.40};\n// 減動效 = 換曲線不關效果',
        "narration": "第一個設計：scrim 的三拍曲線。"
                     "淡入、峰值停頓、淡出——"
                     "過渡形狀本身是資料結構。"
                     "常規檔戲劇化、減動效檔只留短淡化，"
                     "同一個透明度函式換一組常數"
                     "就換一種脾氣——"
                     "動效可及性是換曲線不是刪效果。",
    },
    {
        "tag": "設計 2",
        "title": "數值表是戲劇腳本",
        "sections": [
            ("遞進", "L1 霧色偏移+陰影錯位 0.3；"
                    "L2 加微移 0.12、音景升 2；"
                    "L3 錯位 0.8、微移 0.22、"
                    "主題覆寫開。"),
            ("節制", "L3 音景停在 2——"
                    "「無專屬音態：bed+layer 延續」"
                    "註解明寫不加碼。"),
            ("啟示", "等級表逐欄可讀——"
                    "哪一檔多了什麼，"
                    "switch 三行說完。"),
        ],
        "code": 'case 1: tint+shadow0.3,  audio=1\ncase 2: +jitter0.12,     audio=2\ncase 3: shadow0.8 jitter0.22\n        audio=2 +themeOverride',
        "narration": "第二個設計：數值表是戲劇腳本。"
                     "一級只有霧色偏移和陰影錯位，"
                     "二級加物件微移、音景升滿，"
                     "三級錯位和微移加大、主題覆寫開啟。"
                     "但三級的音景停在二——"
                     "註解明寫沒有專屬音態，"
                     "音景的頂比視覺的頂先到，"
                     "這是刻意的節制。",
    },
    {
        "tag": "設計 3",
        "title": "升才過渡降不過渡",
        "sections": [
            ("做法", "lv>level 才 scrimT=0 啟動——"
                    "等級回落直接換指令包，"
                    "不播過渡。"),
            ("語義", "降臨要儀式、退去不必——"
                    "滲透升級是事件，"
                    "回落只是快照更新。"),
            ("啟示", "不對稱的觸發方向是語義——"
                    "什麼時候播演出，"
                    "本身就是敘事判斷。"),
        ],
        "code": 'if (lv != level) {\n  if (lv > level) scrimT = 0; // 升才播\n  visual = ForLevel(lv);      // 降直換\n}',
        "narration": "第三個設計：升才過渡降不過渡。"
                     "等級上升才啟動 scrim，"
                     "回落直接換指令包不播演出。"
                     "降臨要儀式、退去不必——"
                     "不對稱的觸發方向本身就是語義，"
                     "什麼時候播演出是敘事判斷。",
    },
    {
        "tag": "設計 4",
        "title": "主題覆寫是門閂",
        "sections": [
            ("做法", "themeOverride 在 level>=3 期間"
                    "恆真——滲透不回落的章節內語義；"
                    "等級回落時清除。"),
            ("對比", "旗標不是過渡狀態是持續態——"
                    "scrim 播完旗標還在，"
                    "演出結束、身份留下。"),
            ("啟示", "「演出」和「狀態」分開存——"
                    "scrimT 管過場、"
                    "themeOverride 管身分。"),
        ],
        "code": 'case 3: v.themeOverride = true;\n// scrim 播完旗標仍在\n// 演出是 scrimT，身分是旗標',
        "narration": "第四個設計：主題覆寫是門閂。"
                     "顯化級期間旗標恆真——"
                     "過渡播完它還在，"
                     "演出結束、身份留下。"
                     "演出和狀態分開存："
                     "scrimT 管過場，旗標管身分。",
    },
    {
        "tag": "設計 5",
        "title": "守衛肌肉第三次",
        "sections": [
            ("做法", "Update 入口 clamp(0,3)；"
                    "scrim 計時只在 dt>0 且 "
                    "isfinite 時推進。"),
            ("傳承", "和 MythIncursion 審查輪"
                    "同一招——暫停餵 0 不推進、"
                    "NaN 不腐蝕計時。"),
            ("啟示", "上個 epic 被抓的雷，"
                    "這個 epic 直接寫對——"
                    "審查的產出是起手式。"),
        ],
        "code": 'const int lv = clamp(seepageLevel,0,3);\nif (scrimT>=0 && dt>0 && isfinite(dt))\n  scrimT += dt;  // 暫停不推進',
        "narration": "第五個設計：守衛肌肉第三次。"
                     "入口鉗制零到三，"
                     "計時只在正且有限的 dt 下推進——"
                     "和入侵事件審查輪同一招，"
                     "這次沒等審查抓。"
                     "上個史詩被抓的雷，"
                     "這個史詩直接寫對。",
    },
    {
        "tag": "設計 6",
        "title": "斷言時間軸",
        "sections": [
            ("做法", "測試驗曲線的時間性質："
                    "峰值曾達 >0.3、降檔全程 <0.3s、"
                    "常規 >0.8s、結束 alpha 歸零。"),
            ("配套", "同等級重餵不重播、"
                    "dt<=0 時 alpha 凍結、"
                    "負等級鉗 0 越界鉗 3。"),
            ("啟示", "動效的斷言斷的是時間軸——"
                    "「比較短」「曾到峰」「歸零」，"
                    "演出品質是可驗的。"),
        ],
        "code": 'Check(t < 0.30, "降檔過渡0.3s內");\nCheck(t2 > 0.8, "常規較長");\nCheck(t2 > t, "reduction明顯更短");\nCheck(alpha==0, "結束歸零");',
        "narration": "第六個設計：斷言時間軸。"
                     "測試驗的是曲線的時間性質——"
                     "降檔過渡零點三秒內結束、"
                     "常規檔超過零點八秒、"
                     "減動效明顯更短、結束歸零。"
                     "動效的斷言斷在時間軸上，"
                     "演出品質是可驗證的。",
    },
    {
        "tag": "設計 7",
        "title": "接線三站各守一邊",
        "sections": [
            ("渲染", "BattleSceneSync 補唯讀霧色 "
                    "setter——指令包進、"
                    "渲染層不回寫。"),
            ("設定", "UISettings 加 motionReduction "
                    "可選欄——舊檔降級，"
                    "schema 語義不動。"),
            ("玩法", "DuanqiaoPlayable 戰鬥迴圈"
                    "讀 Myths().Level(region) 注入——"
                    "Campaign 查值、Gameplay 收 int。"),
        ],
        "code": '// sync.SetFogTint(visual.fogTint)\n// cues.SetSeepageLevel(visual.audioLevel)\n// stage.Update(dt, myths.Level(region))',
        "narration": "第七個設計：接線三站各守一邊。"
                     "場景同步補唯讀霧色口、"
                     "設定加可選減動效欄、"
                     "可玩切片在戰鬥迴圈查滲透等級注入。"
                     "戰役層查值、玩法層收整數、"
                     "渲染層只吃指令包——"
                     "依賴方向一站沒破。",
    },
    {
        "tag": "設計 8",
        "title": "Epic D 閉環",
        "sections": [
            ("全鏈", "D-1 壓力累積 → D-2 神社選擇 → "
                    "D-3 天命兌換 → D-4 入侵事件 → "
                    "D-5 視覺消費端。"),
            ("回顧", "序數從 D-1 定義那天起只走 int——"
                    "五個系統消費它，"
                    "沒有一個碰 MythLayer 型別。"),
            ("啟示", "一個序數五個消費者——"
                    "帳本、神社、兌換、入侵、視效，"
                    "型別隔離讓每站各自演化。"),
        ],
        "code": 'Seepage ordinal (int)\n  → MythIncursion.Update\n  → SeepageStage.Update\n  → AudioCues.SetSeepageLevel\n// 同一序數，五個消費者',
        "narration": "第八個設計：Epic D 閉環。"
                     "壓力累積、神社選擇、天命兌換、"
                     "入侵事件、視覺消費端——"
                     "滲透序數從定義那天起只走整數，"
                     "五個系統消費它，"
                     "沒有一個碰戰役層型別。"
                     "型別隔離讓每一站各自演化。",
    },
    {
        "kind": "cover",
        "title": "第四十三集清單",
        "lines": ["三拍曲線 · 數值即腳本 · 升才過渡",
                  "覆寫門閂 · 守衛肌肉 · 斷言時間軸",
                  "接線三站 · Epic D 閉環"],
        "narration": "第四十三集收在這裡。神話層從"
                     "看不見的壓力，走到看得見的臉——"
                     "序數走完全程，"
                     "最後一個消費者是玩家的眼睛。"
                     "Epic D 五件至此齊全。",
    },
]

SLIDES_EP44 = [
    {
        "kind": "cover",
        "title": "明文不落記憶體",
        "lines": ["從實戰學 C++ 第四十四集",
                  "ProtectedValue · HMAC 簽章 · "
                  "上游回步 · 輪詢改事件"],
        "narration": "第四十四集，防線進了記憶體。"
                     "拆倉後第一次反向同步——"
                     "資料防護層在上游引擎 repo 長出來，"
                     "回步進遊戲倉。這集講兩件事："
                     "數值怎麼藏、和河道為什麼倒著流。",
    },
    {
        "tag": "設計 1",
        "title": "混淆容器的三招",
        "sections": [
            ("存放", "cipher = value XOR key XOR 位置——"
                    "明文永不落記憶體，"
                    "掃描已知值找不到位址。"),
            ("重配", "每次 Set 重新配 key——"
                    "「搜舊值→篩新值」的差分流程"
                    "也失效。"),
            ("偵測", "seal 完整性欄位——"
                    "不經 Set 的直接 patch，"
                    "下一次 Get 時 IsTampered 回真。"),
        ],
        "code": 'ProtectedValue<int> score(0);\nscore += 100;            // 明文不落記憶體\nif (score.IsTampered()) { /* 被 patch */ }',
        "narration": "第一個設計：混淆容器的三招。"
                     "密文是值異或金鑰再混位置——"
                     "明文不落記憶體，掃值掃不到位址。"
                     "每次寫入重配金鑰，"
                     "差分篩選也失效。"
                     "封印欄位讓繞過介面的直接改寫，"
                     "在下一次讀取時現形。",
    },
    {
        "tag": "設計 2",
        "title": "誠實的威脅模型",
        "sections": [
            ("註解", "「這是混淆+偵測，"
                    "不是密碼學防護」——"
                    "硬體中斷點觀察解碼路徑"
                    "仍可推出結構。"),
            ("邊界", "「高價值狀態應伺服器權威；"
                    "本類適合單機體感數值」——"
                    "適用域和不適用域都寫。"),
            ("啟示", "安全元件的第一要件是"
                    "自知之明——唬人的防護"
                    "比沒有防護更危險。"),
        ],
        "code": '// 混淆+偵測 ≠ 密碼學防護\n// 適合: 分數/彈藥/經驗值\n// 不適合: 排名/商城餘額\n//   → 應伺服器權威',
        "narration": "第二個設計：誠實的威脅模型。"
                     "類註解明寫這是混淆加偵測，"
                     "不是密碼學防護——"
                     "下硬體中斷點的對手仍能破解。"
                     "適用域和不適用域都寫清楚："
                     "體感數值可以用，"
                     "排名和餘額要伺服器權威。"
                     "唬人的防護比沒有防護更危險。",
    },
    {
        "tag": "設計 3",
        "title": "簽章 blob 的形狀",
        "sections": [
            ("格式", "blob = [原始資料 || "
                    "HMAC-SHA256 尾 32B]——"
                    "簽章只是尾巴，資料仍是明文。"),
            ("驗證", "常數時間比對；短於 32B 或 "
                    "MAC 不符一律 false；"
                    "out=nullptr 可「只驗不取」。"),
            ("啟示", "格式設計讓用途分層——"
                    "驗證和取出是兩個操作，"
                    "只驗證不用付出解密成本。"),
        ],
        "code": 'blob = data || HMAC-SHA256(key, data)\nVerifySignedData(key, blob, nullptr);\n// 只驗不取——驗證≠取出',
        "narration": "第三個設計：簽章區塊的形狀。"
                     "資料接三十二位元組的訊息鑑別碼尾巴——"
                     "簽章只是尾巴，資料仍是明文。"
                     "驗證用常數時間比對，"
                     "還能只驗不取——"
                     "驗證和取出是兩個操作，"
                     "只驗證不付出解密成本。",
    },
    {
        "tag": "設計 4",
        "title": "隨機數的三層備援",
        "sections": [
            ("鏈條", "BCryptGenRandom → /dev/urandom → "
                    "多來源混合 xorshift——"
                    "最後備援註明「非密碼學級」。"),
            ("誠實", "連降級都標品質——"
                    "備援可用不等於同級，"
                    "標籤跟著強度走。"),
            ("啟示", "降級鏈要帶品質標籤——"
                    "「能用」和「同樣安全」"
                    "是兩個斷言。"),
        ],
        "code": 'BCryptGenRandom → /dev/urandom\n  → xorshift(多來源混合)\n// 最後備援標「非密碼學級」',
        "narration": "第四個設計：隨機數的三層備援。"
                     "系統加密源、作業系統熵池、"
                     "最後是多來源混合的自研產生器——"
                     "備援可用但標明非密碼學級。"
                     "降級鏈每一級都帶品質標籤："
                     "能用和同樣安全是兩個斷言。",
    },
    {
        "tag": "設計 5",
        "title": "河道倒著流",
        "sections": [
            ("事實", "ProtectedValue/SignData 在上游 "
                    "PotatoEngine 先長出來——"
                    "這批是反向回步進遊戲倉。"),
            ("意義", "同步腳本一直是單向："
                    "遊戲倉→上游。這次倒過來——"
                    "引擎 repo 開始自己產出。"),
            ("啟示", "拆倉的完成標誌不是搬完——"
                    "是上游開始反哺，"
                    "新東西長在新家再回步。"),
        ],
        "code": '# 以前: monorepo → PotatoEngine\n# 這批: PotatoEngine → monorepo\n#   ProtectedValue.h + 簽章 API 回步',
        "narration": "第五個設計：河道倒著流。"
                     "資料防護層在上游引擎倉先長出來，"
                     "這批是反向回步進遊戲倉——"
                     "同步腳本一直是單向的，這次倒過來。"
                     "拆倉的完成標誌不是搬完，"
                     "是上游開始反哺："
                     "新東西長在新家，再回步。",
    },
    {
        "tag": "設計 6",
        "title": "音景從輪詢改事件",
        "sections": [
            ("原狀", "戰鬥迴圈每幀 "
                    "SetSeepageLevel + prevLevel "
                    "手動做邊緣檢測。"),
            ("現狀", "Myths().SetEventCallback 持有"
                    "邊緣偵測——滲透層發事件時"
                    "順手升音景、放 cue。"),
            ("紅利", "回呼在 LoadFromFile 後存活——"
                    "註解明寫跨存檔重置仍有效，"
                    "接線一次管一輩子。"),
        ],
        "code": 'campaign.Myths().SetEventCallback([&](ev){\n  int mx = max over Levels();\n  cues.SetSeepageLevel(map(mx));\n});\n// 邊緣檢測搬回訊號源',
        "narration": "第六個設計：音景從輪詢改事件。"
                     "原本戰鬥迴圈每幀餵等級、"
                     "手動記上一級做邊緣檢測。"
                     "現在滲透層的事件回呼直接持有——"
                     "訊號源發變化時順手升音景放音效，"
                     "而且回呼在讀檔重置後仍存活。"
                     "接線一次，管一輩子。",
    },
    {
        "tag": "設計 7",
        "title": "回步帶測試同行",
        "sections": [
            ("配套", "SecurityDataTest + AntiTamperTest "
                    "隨碼回步——進 POTATO_TESTS，"
                    "上游同名註冊對齊。"),
            ("規矩", "sync 白名單一次補三項——"
                    "ContentScanner/SecurityData/"
                    "AntiTamper，對齊上游既有。"),
            ("啟示", "回步的最小單位是"
                    "「碼+測試+註冊」——"
                    "只搬碼不搬測試是半套。"),
        ],
        "code": '# 回步單位 = 碼+測試+註冊\nSecurityDataTest  AntiTamperTest\nContentScannerTest  ← 白名單補三項',
        "narration": "第七個設計：回步帶測試同行。"
                     "兩支防護測試隨碼回步進清單，"
                     "同步白名單一次補三項對齊上游。"
                     "回步的最小單位是碼加測試加註冊——"
                     "只搬碼不搬測試是半套。",
    },
    {
        "tag": "設計 8",
        "title": "防線是層疊的",
        "sections": [
            ("盤點", "Security 模組至此有："
                    "ContentScanner（載入前掃）+ "
                    "紅隊測試 + ProtectedValue（運行時）+ "
                    "簽章（持久層）。"),
            ("分工", "四道防線管四個時刻——"
                    "載入前、建置期、運行中、落盤後，"
                    "各有管區不重疊。"),
            ("啟示", "防禦縱深是按時刻分層的——"
                    "每一層只需要擋住"
                    "自己那個時間點的攻擊。"),
        ],
        "code": '載入前  ContentScanner 掃描\n建置期  紅隊測試驗偵測\n運行中  ProtectedValue 混淆\n落盤後  HMAC 簽章防竄改',
        "narration": "第八個設計：防線是層疊的。"
                     "安全模組至此四道防線："
                     "載入前掃描、建置期紅隊驗證、"
                     "運行中混淆、落盤後簽章——"
                     "各管一個時刻不重疊。"
                     "防禦縱深按時間軸分層，"
                     "每一層只需擋住"
                     "自己那個時間點的攻擊。",
    },
    {
        "kind": "cover",
        "title": "第四十四集清單",
        "lines": ["混淆三招 · 誠實威脅模型 · blob 尾巴",
                  "備援帶標籤 · 河道倒著流 · 輪詢改事件",
                  "回步帶測試 · 四道防線"],
        "narration": "第四十四集收在這裡。拆倉的敘事"
                     "翻過最後一頁——上游 repo 不再只是"
                     "鏡像的終點，是新元件的產地。"
                     "而遊戲倉繼續它的節奏："
                     "接線從輪詢改成事件，"
                     "每一處都更靠近訊號源。",
    },
]

SLIDES_EP45 = [
    {
        "kind": "cover",
        "title": "看見即點到",
        "lines": ["從實戰學 C++ 第四十五集",
                  "D-5 審查輪 · 渲染與點選分岔 · "
                  "相位綁格子 · 滲透有了生產者"],
        "narration": "第四十五集，視覺層過審查。"
                     "滲透視效的審查輪抓到一個"
                     "只有「長出來之後」才看得見的 bug："
                     "標記被位移了，但點選射線沒有——"
                     "你看見的，點不到。",
    },
    {
        "tag": "修正 1",
        "title": "渲染位移點選沒位移",
        "sections": [
            ("發現", "陰影錯位只加在渲染路徑——"
                    "PickFogCloud 仍用原格座標比對，"
                    "玩家點的是「沒偏移的幽靈」。"),
            ("修法", "點選函式收 seepShadow 參數，"
                    "判定位置套用同一個位移——"
                    "看見即點到。"),
            ("啟示", "視覺修飾改的不只是畫面——"
                    "凡是「位置」的地方都要問："
                    "我用的是哪個版本的位置？"),
        ],
        "code": '// 渲染: pos + shadow*0.5\n// 點選原本: pos        ← 分岔\nPickFogCloud(..., seepShadow);\n// 判定與渲染同一位移',
        "narration": "第一個修正：渲染位移了，點選沒有。"
                     "陰影錯位只加在渲染路徑，"
                     "點選仍用原格座標比對——"
                     "玩家點的是沒偏移的幽靈。"
                     "修法是點選函式收同一個位移參數，"
                     "看見即點到。"
                     "視覺修飾改的不只是畫面，"
                     "是每個用到位置的地方。",
    },
    {
        "tag": "修正 2",
        "title": "相位綁格子不綁槽位",
        "sections": [
            ("發現", "微移相位用可見槽位索引 i——"
                    "候選被篩掉時，"
                    "倖存標記的相位整批跳變。"),
            ("修法", "相位改綁候選格座標——"
                    "格子在不在，相位都在那，"
                    "篩選不再擾動倖存者。"),
            ("啟示", "動畫的種子要綁資料身分——"
                    "綁陣列位置是綁給容器看，"
                    "綁格子是綁給世界看。"),
        ],
        "code": '- ph = i * 1.7f;              // 槽位\n+ ph = cell.x*1.7f + cell.y*2.3f;\n// 篩選不再讓倖存標記跳變',
        "narration": "第二個修正：相位綁格子不綁槽位。"
                     "微移相位原本用可見槽位索引——"
                     "候選被篩掉時倖存標記整批跳變。"
                     "改成綁候選格的座標："
                     "格子在不在，相位都在那。"
                     "動畫的種子要綁資料身分，"
                     "不綁容器位置。",
    },
    {
        "tag": "修正 3",
        "title": "曲線檔觸發時快照",
        "sections": [
            ("發現", "過渡途中切 motion-reduction，"
                    "ScrimAlpha 當幀換曲線——"
                    "alpha 值跳變、畫面閃爍。"),
            ("修法", "觸發瞬間把 reduced 快照成 "
                    "scrimReduced——整場過渡"
                    "用同一條曲線走完。"),
            ("啟示", "進行中的動畫要快照設定——"
                    "「當下生效」對狀態是對的，"
                    "對演出是跳變。"),
        ],
        "code": 'if (lv > level) {\n  scrimT = 0;\n  scrimReduced = reduced; // 快照\n}\n// 途中切設定不跳變',
        "narration": "第三個修正：曲線檔觸發時快照。"
                     "過渡途中切減動效設定，"
                     "透明度當幀換曲線會跳變。"
                     "改成觸發瞬間快照曲線檔——"
                     "整場過渡用同一條曲線走完。"
                     "當下生效對狀態是對的，"
                     "對演出是跳變。",
    },
    {
        "tag": "修正 4",
        "title": "回落即終結",
        "sections": [
            ("發現", "等級回落時進行中的 scrim "
                    "繼續播完——降臨演出在"
                    "退去之後還在演。"),
            ("修法", "lv<level 時 scrimT=-1 直接終結；"
                    "release 除法加 >0 守衛防零曲線。"),
            ("啟示", "狀態機的每個轉換都要想"
                    "「另一個方向呢」——"
                    "上升有儀式，下降有善後。"),
        ],
        "code": '} else {\n  scrimT = -1.0f; // 回落即終結\n}\n// release>0 才除——防零曲線',
        "narration": "第四個修正：回落即終結。"
                     "等級回落時進行中的過渡照播完——"
                     "降臨演出在退去之後還在演。"
                     "現在回落直接終結，"
                     "淡出除法也補了防零守衛。"
                     "狀態機的每個轉換都要問"
                     "另一個方向呢。",
    },
    {
        "tag": "修正 5",
        "title": "暫停連 scrim 一起凍",
        "sections": [
            ("做法", "餵 Update 的是 "
                    "dt×timeScale——暫停時縮放後為零，"
                    "scrim 與世界同凍。"),
            ("契約", "標頭註解寫死：dt 必須是"
                    "已縮放秒數——暫停中餵 0 "
                    "不推進計時。"),
            ("啟示", "時間的正確來源是一個值"
                    "不是兩個值——呼叫端交的是"
                    "世界時間不是牆鐘。"),
        ],
        "code": 'seepageStage.Update(\n  dt * battle.GetTimeScale(), ...);\n// 暫停→dt=0→scrim 同凍',
        "narration": "第五個修正：暫停連過渡一起凍。"
                     "餵進來的 dt 乘上時間倍率——"
                     "暫停時是零，過渡與世界同凍。"
                     "契約寫在標頭：dt 必須是"
                     "已縮放的秒數。"
                     "時間的正確來源是世界時間，"
                     "不是牆上的鐘。",
    },
    {
        "tag": "修正 6",
        "title": "Manifest 要有可見信號",
        "sections": [
            ("發現", "玩家主題已是 InkChronicle 時，"
                    "L3 切到同主題＝沒有信號——"
                    "顯化級的儀式感落空。"),
            ("修法", "已在 InkChronicle 則切 "
                    "WarMap——覆寫的意義是"
                    "「和現在不一樣」。"),
            ("啟示", "覆寫語義是差異不是目標——"
                    "「切到某主題」不如"
                    "「切到不是現在的主題」。"),
        ],
        "code": 'manifestTheme =\n  theme==InkChronicle ? WarMap\n                      : InkChronicle;\n// 覆寫的意義 = 可見差異',
        "narration": "第六個修正：顯化要有可見信號。"
                     "玩家本來就用史卷主題時，"
                     "切到同主題等於沒信號。"
                     "現在已在史卷就改切戰圖——"
                     "覆寫的意義是差異不是目標："
                     "要的是和現在不一樣。",
    },
    {
        "tag": "修正 7",
        "title": "滲透有了生產者",
        "sections": [
            ("缺口", "D-1~D-5 消費端全落地，"
                    "但 playable 裡沒人餵 MythLayer——"
                    "等級永遠是零，視效永不觸發。"),
            ("修法", "戰後治理事件逐筆 "
                    "Feed(seepageRegion, gev)——"
                    "戰場暴行即滲透來源。"),
            ("啟示", "消費端齊了不算完——"
                    "沒有生產者的管線是裝飾，"
                    "接線的最後一站是上游。"),
        ],
        "code": 'for (gev : battle.GetGovernanceEvents())\n  for (i < cnt)\n    campaign.Myths().Feed(region, gev);\n// 暴行入滲透——producer 接通',
        "narration": "第七個修正：滲透有了生產者。"
                     "消費端全部落地，"
                     "但可玩切片裡沒人餵滲透層——"
                     "等級永遠是零，視效永不觸發。"
                     "現在戰後治理事件逐筆餵進去："
                     "戰場暴行即滲透來源。"
                     "接線的最後一站是上游。",
    },
    {
        "tag": "修正 8",
        "title": "解綁清場不留妝",
        "sections": [
            ("做法", "Unbind 補清 seepageTint/"
                    "jitter/shadow——D-5 修飾"
                    "不跨場殘留。"),
            ("配套", "設定讀取從 AsInt 改 AsNumber——"
                    "避開異常值 static_cast<int> "
                    "的未定義行為。"),
            ("啟示", "生命週期函式是新欄位的"
                    "必考題——加欄位那天就要問"
                    "「Unbind 清了沒」。"),
        ],
        "code": 'Unbind() {\n  seepageTint = Vector3();\n  seepageJitter = seepageShadow = 0;\n}\n// AsNumber 比對避 AsInt UB',
        "narration": "第八個修正：解綁清場不留妝。"
                     "解除綁定時清空滲透修飾三件套，"
                     "不讓上一場的妝留到下一場。"
                     "配套是設定讀取改用浮點比對，"
                     "避開異常值轉整數的未定義行為。"
                     "加欄位那天就要問："
                     "解綁清了沒。",
    },
    {
        "kind": "cover",
        "title": "第四十五集清單",
        "lines": ["看見即點到 · 相位綁格子 · 曲線快照",
                  "回落即終結 · 世界時間 · 可見信號",
                  "生產者接通 · 解綁清場"],
        "narration": "第四十五集收在這裡。D-5 進審查——"
                     "視覺層的 bug 清一色是「一致性 bug」："
                     "渲染和點選、相位和篩選、"
                     "設定和快照、消費者和生產者。"
                     "長出臉之後才發現，"
                     "臉的每個部位都要對齊。",
    },
]

SLIDES_EP46 = [
    {
        "kind": "cover",
        "title": "鏡像沙盤",
        "lines": ["從實戰學 C++ 第四十六集",
                  "G-4 BattleSandbox · 對稱即公平 · "
                  "關掉所有旋鈕 · 整數抖動"],
        "narration": "第四十六集，牌組進了風洞。"
                     "G-4 沙盤模擬器：兩副學說牌組"
                     "在鏡像斷橋場景對局五十場——"
                     "平衡測試的第一原理是："
                     "除了牌組，什麼都不許不同。",
    },
    {
        "tag": "設計 1",
        "title": "對稱即公平",
        "sections": [
            ("場景", "20×15 斷橋鏡像——中央牆缺口 "
                    "y6-8、雙方各四隊同兵數、"
                    "目標與集結點左右對稱。"),
            ("細節", "站位抖動對兩邊一致套用——"
                    "seed 只改佈局不改公平性，"
                    "地圖不偏心。"),
            ("啟示", "測牌組品質先隔離變因——"
                    "對稱場景把「誰贏」變成"
                    "「哪副牌好」。"),
        ],
        "code": 'SetObstacle(10, y)  // 中央牆缺口 y6-8\nSetObjective(0, {18,7});\nSetObjective(1, {2, 7});  // 鏡像\n// 抖動兩邊一致套用',
        "narration": "第一個設計：對稱即公平。"
                     "二十乘十五的斷橋鏡像場——"
                     "中央牆只留三格缺口，"
                     "雙方各四隊同兵數，"
                     "目標和集結點左右對稱。"
                     "站位抖動對兩邊一致套用，"
                     "種子只改佈局不改公平性。",
    },
    {
        "tag": "設計 2",
        "title": "關掉所有旋鈕",
        "sections": [
            ("做法", "SetCommandPoints 雙方歸零——"
                    "沙盤只測學說，"
                    "指揮介入是干擾項。"),
            ("做法", "moraleExecThreshold 保持 0——"
                    "戰鬥內部無 RNG，"
                    "同 seed 跨場完全可重現。"),
            ("啟示", "實驗的第一步是關旋鈕——"
                    "量到的是牌組差異，"
                    "不是系統噪音。"),
        ],
        "code": 'SetCommandPoints(0,0); // 無 CP 介入\nSetCommandPoints(1,0);\n// moraleExecThreshold=0\n//   → 戰鬥內部無 RNG',
        "narration": "第二個設計：關掉所有旋鈕。"
                     "雙方指揮點歸零——沙盤只測學說，"
                     "指揮介入是干擾項。"
                     "士氣處決門檻保持零，"
                     "戰鬥內部沒有隨機數，"
                     "同種子跨場完全可重現。"
                     "實驗的第一步是關旋鈕。",
    },
    {
        "tag": "設計 3",
        "title": "整數抖動",
        "sections": [
            ("做法", "站位抖動用 "
                    "rng() % (2m+1) 純整數運算——"
                    "uniform_real_distribution "
                    "跨標準庫實作異果。"),
            ("傳承", "L-8 同款守衛第四度出現——"
                    "CI 可重現的隨機，"
                    "只能是整數的隨機。"),
            ("啟示", "跨平台確定性是一種紀律——"
                    "浮點分佈函式的差異"
                    "小到看不見、大到輸掉對局。"),
        ],
        "code": 'const int m = (int)(range*100);\nbase + (rng() % (2*m+1) - m)/100.0f;\n// uniform_real 跨庫異果\n//   → 純整數抖動',
        "narration": "第三個設計：整數抖動。"
                     "站位抖動用純整數運算——"
                     "浮點分佈函式跨標準庫實作結果不同，"
                     "查證式史官當年立的守衛"
                     "第四次出現。"
                     "CI 可重現的隨機，"
                     "只能是整數的隨機。",
    },
    {
        "tag": "設計 4",
        "title": "工具和測試同一顆心臟",
        "sections": [
            ("做法", "BattleSandboxCore.h 是 "
                    "header-only——CLI 入口和無頭測試"
                    "編譯同一份邏輯。"),
            ("對比", "CLI 寫一遍、測試另寫一遍的"
                    "工具會漂移——共用核心"
                    "讓「工具的輸出」和"
                    "「測試驗證的行為」是同一件。"),
            ("啟示", "可測性的形狀是 header——"
                    "核心不帶 main，"
                    "兩個殼各自長出來。"),
        ],
        "code": 'BattleSandbox.cpp  (CLI 殼)\nBattleSandboxTest.cpp (測試殼)\n  ↓ 共用\nBattleSandboxCore.h (header-only)',
        "narration": "第四個設計：工具和測試同一顆心臟。"
                     "沙盤核心是僅標頭庫——"
                     "命令列入口和無頭測試編譯同一份邏輯。"
                     "各寫一遍的工具會漂移，"
                     "共用核心讓工具的輸出和"
                     "測試驗證的行為是同一件事。",
    },
    {
        "tag": "設計 5",
        "title": "勝率之外的計分",
        "sections": [
            ("做法", "每場記 trigger 命中數——"
                    "trigA/trigB 分牌組累計，"
                    "勝負之外記「哪條規則開過火」。"),
            ("價值", "牌組的質地是觸發分佈——"
                    "贏了但核心 trigger 沒命中，"
                    "和靠它贏是兩種贏法。"),
            ("啟示", "平衡工具要輸出機制用量——"
                    "勝率說誰強，"
                    "觸發數說為什麼。"),
        ],
        "code": 'struct SandboxMatch {\n  int winnerTeam;  // -1 = 和局\n  float duration;\n  map<string,int> trigA, trigB;\n};',
        "narration": "第五個設計：勝率之外的計分。"
                     "每場記觸發命中數——"
                     "勝負之外記哪條規則開過火。"
                     "牌組的質地是觸發分佈："
                     "贏了但核心觸發沒命中，"
                     "和靠它贏是兩種贏法。"
                     "勝率說誰強，觸發數說為什麼。",
    },
    {
        "tag": "設計 6",
        "title": "報表有 schema",
        "sections": [
            ("做法", "輸出 potato.sandbox_report/1——"
                    "seed/場數/勝負/和局/均時長/"
                    "觸發累計全入檔。"),
            ("細節", "和局與超時是一等結局 "
                    "winnerTeam=-1——"
                    "「沒打完」也是資料。"),
            ("啟示", "工具輸出用版本化 schema——"
                    "報表是資產，"
                    "可以 diff 可以回放可以入帳。"),
        ],
        "code": '"schema": "potato.sandbox_report/1"\n{seed, matches, winsA, winsB,\n draws, totalDuration, trigA, trigB}',
        "narration": "第六個設計：報表有綱目。"
                     "輸出是帶版本號的報表綱目——"
                     "種子場數勝負和局時長觸發全入檔，"
                     "和局與超時是一等結局。"
                     "工具輸出用版本化綱目："
                     "報表是資產，可以比對、"
                     "回放、入帳。",
    },
    {
        "tag": "設計 7",
        "title": "五十場的分母",
        "sections": [
            ("做法", "matches 預設 50——"
                    "批次統計讓單場噪音"
                    "攤成勝率估計。"),
            ("細節", "matchSeed 由場次派生——"
                    "每場佈局不同但序列確定，"
                    "重跑同報表。"),
            ("啟示", "平衡結論的基本單位是批次——"
                    "一場是軼事，"
                    "五十場是資料。"),
        ],
        "code": 'matches = 50;  // 預設批次\nfor (i=0; i<matches; ++i)\n  Run(cfg, seed + i);  // 派生 seed\n// 一場是軼事，五十場是資料',
        "narration": "第七個設計：五十場的分母。"
                     "預設五十場——批次統計把單場噪音"
                     "攤成勝率估計。"
                     "每場種子由場次派生，"
                     "佈局不同但序列確定。"
                     "平衡結論的基本單位是批次："
                     "一場是軼事，五十場是資料。",
    },
    {
        "tag": "設計 8",
        "title": "資料迴路閉合",
        "sections": [
            ("鏈路", "牌組 JSON（內容）→ 沙盤對局"
                    "（模擬）→ 報表（資料）→ "
                    "調整牌組——設計迭代有了引擎。"),
            ("位置", "沙盤住 Examples/ 不住玩法層——"
                    "它是工具不是規則，"
                    "平衡實驗不污染戰鬥本體。"),
            ("啟示", "內容工具的歸屬看讀者——"
                    "給設計師的放 Examples，"
                    "給系統的進 Gameplay。"),
        ],
        "code": 'doctrine_set.json\n  → BattleSandbox (Examples/)\n  → sandbox_report/1\n  → 調牌組 → 再跑',
        "narration": "第八個設計：資料迴路閉合。"
                     "牌組檔進沙盤、報表出來、"
                     "調完再跑——設計迭代有了引擎。"
                     "沙盤住在範例層不住玩法層，"
                     "它是工具不是規則，"
                     "平衡實驗不污染戰鬥本體。",
    },
    {
        "kind": "cover",
        "title": "第四十六集清單",
        "lines": ["鏡像場隔離牌組 · 關旋鈕 · 整數抖動",
                  "共用核心 · 觸發計分 · 報表 schema",
                  "五十場分母 · 資料迴路閉合"],
        "narration": "第四十六集收在這裡。學說系統"
                     "從實作、審查、測試走到風洞——"
                     "「這副牌強不強」終於是"
                     "可以量的事。"
                     "設計師的下一個問題，"
                     "不用等下一場實戰才知道答案。",
    },
]

SLIDES_EP47 = [
    {
        "kind": "cover",
        "title": "工作池的謙遜",
        "lines": ["從實戰學 C++ 第四十七集",
                  "JobSystem 回步 · help-run · "
                  "不選 work-stealing · stale index 二度"],
        "narration": "第四十七集，執行緒池進引擎。"
                     "上游回步的 JobSystem——"
                     "但它最有意思的不是「有」，"
                     "是「不選」：不選無鎖竊取、"
                     "不選優先級、不選綁核，"
                     "並發設計的勇氣是選無聊。",
    },
    {
        "tag": "設計 1",
        "title": "選 deque 不選竊取",
        "sections": [
            ("做法", "MPMC 佇列 = deque + mutex + "
                    "condition_variable——"
                    "教科書三件套，沒有無鎖結構。"),
            ("理由", "註解明寫：「遊戲規模下"
                    "無鎖竊取的收益遠低於其複雜度」"
                    "——性能方案按規模選。"),
            ("啟示", "並發的第一美德是無聊——"
                    "看得懂的鎖，"
                    "比看不懂的無鎖更能上線。"),
        ],
        "code": 'std::deque<Job> queue;\nstd::mutex queueMutex;\nstd::condition_variable cv;\n// 不引入 work-stealing deques',
        "narration": "第一個設計：選佇列不選竊取。"
                     "多產多銷佇列就用教科書三件套，"
                     "註解明寫為什麼不做無鎖竊取——"
                     "遊戲規模下收益遠低於複雜度。"
                     "並發的第一美德是無聊："
                     "看得懂的鎖比看不懂的無鎖"
                     "更能上線。",
    },
    {
        "tag": "設計 2",
        "title": "等待的人幫忙做",
        "sections": [
            ("做法", "ParallelFor 的等待由呼叫執行緒"
                    "參與執行——help-run：等佇列"
                    "清空時自己也下去搬。"),
            ("收益", "workers 全忙時不自鎖，"
                    "還縮短尾部等待——"
                    "最後一件工作不必排隊等閒。"),
            ("啟示", "等待本身是浪費的資源——"
                    "呼叫執行緒是免費的"
                    "第 N+1 個工人。"),
        ],
        "code": 'ParallelFor(count, fn, batchSize);\n// 等待=呼叫執行緒也執行\n// 全忙不自鎖+縮短尾部',
        "narration": "第二個設計：等待的人幫忙做。"
                     "並行迴圈的等待由呼叫執行緒"
                     "參與執行——工人全忙時不會自鎖，"
                     "還縮短尾部等待。"
                     "等待本身是浪費的資源："
                     "呼叫執行緒是免費的第 N 加一個工人。",
    },
    {
        "tag": "設計 3",
        "title": "巢狀安全靠推進保證",
        "sections": [
            ("做法", "工作內可再 Submit/"
                    "ParallelFor——巢狀投遞"
                    "不死鎖。"),
            ("機制", "靠的不是特殊遞迴處理——"
                    "是 help-run：等待者直接執行"
                    "巢狀工作，推進必然發生。"),
            ("啟示", "巢狀安全是推進保證的推論——"
                    "設計對了「等」，"
                    "「巢」自然安全。"),
        ],
        "code": '// 工作內再投遞不死鎖\nParallelFor(n, [&](i){\n  js.ParallelFor(m, ...); // 巢狀 OK\n});\n// help-run 保證推進',
        "narration": "第三個設計：巢狀安全靠推進保證。"
                     "工作裡可以再投遞再並行——"
                     "靠的不是特殊遞迴處理，"
                     "是幫跑機制：等待者直接執行"
                     "巢狀工作，推進必然發生。"
                     "設計對了等，巢自然安全。",
    },
    {
        "tag": "設計 4",
        "title": "析構是契約的一部分",
        "sections": [
            ("做法", "解構先排空佇列再 join "
                    "workers——剩餘工作執行完"
                    "才收工。"),
            ("配套", "PotatoEngine::Shutdown "
                    "先收池再收其他子系統——"
                    "關閉順序寫進引擎生命週期。"),
            ("啟示", "池的生命週期是契約——"
                    "「最後一件工作做完了嗎」"
                    "由型別保證不靠呼叫端記。"),
        ],
        "code": '~JobSystem();  // 排空佇列才 join\n// Shutdown 先收池\n// 契約: 析構=所有工作完成',
        "narration": "第四個設計：析構是契約的一部分。"
                     "解構先排空佇列才收工人——"
                     "剩餘工作執行完才算完。"
                     "引擎關閉順序裡先收池再收子系統。"
                     "最後一件工作做完了嗎，"
                     "由型別保證不靠呼叫端記。",
    },
    {
        "tag": "設計 5",
        "title": "不做的寫在文件",
        "sections": [
            ("清單", "「無優先級、無取消、"
                    "無親和性綁核——"
                    "需要這些時再迭代」。"),
            ("比對", "每個不做都對應一個"
                    "真實複雜度：優先級要排序佇列、"
                    "取消要協作旗標、綁核要平台 API。"),
            ("啟示", "缺席的功能具名——"
                    "使用者看到邊界，"
                    "維護者看到門在哪裡留著。"),
        ],
        "code": '// 誠實性：\n// 無優先級、無取消、無綁核\n// ——需要這些時再迭代',
        "narration": "第五個設計：不做的寫在文件。"
                     "沒有優先級、沒有取消、沒有綁核——"
                     "需要時再迭代。"
                     "每個不做都對應真實複雜度。"
                     "缺席的功能具名："
                     "使用者看到邊界，"
                     "維護者看到門留在哪裡。",
    },
    {
        "tag": "設計 6",
        "title": "設定欄位變契約",
        "sections": [
            ("落地", "EngineConfig 的 enableJobSystem/"
                    "workerThreads 實接——"
                    "Initialize 建池、"
                    "GetJobSystem() 取用。"),
            ("對比", "設定檔裡躺著的欄位"
                    "第一次長出實作——"
                    "宣告變成承諾。"),
            ("啟示", "config 欄位是延遲的介面——"
                    "寫進設定那天就在許諾，"
                    "實作落地才算履約。"),
        ],
        "code": 'cfg.enableJobSystem=true\n  → Initialize() 建池\n  → GetJobSystem() 取用\n  → Shutdown() 先收池',
        "narration": "第六個設計：設定欄位變契約。"
                     "引擎設定的開關和執行緒數實接——"
                     "初始化建池、取用介面、"
                     "關閉先收。"
                     "設定檔裡躺著的欄位第一次長出實作，"
                     "寫進設定那天就在許諾，"
                     "實作落地才算履約。",
    },
    {
        "tag": "設計 7",
        "title": "stale index 二度",
        "sections": [
            ("事發", "JobSystem 的提交夾帶 stale "
                    "index——誤刪了前一個 D-5 "
                    "提交的內容。"),
            ("修法", "4d42ff2 用 worktree 最新版補回，"
                    "commit 訊息明寫「stale index "
                    "提交誤刪」——事故入帳。"),
            ("啟示", "平行衛生條款第二次被驗證——"
                    "這次教訓寫進 commit message，"
                    "未來的考古者看得到原因。"),
        ],
        "code": '7224e0d D-5 落地\nbb11633 stale index 誤刪內容\n4d42ff2 補回+審查修正\n// 訊息連事故原因都寫',
        "narration": "第七個設計：過期索引二度。"
                     "工作池的提交夾帶過期索引，"
                     "誤刪了前一個提交的內容——"
                     "用工作目錄最新版補回，"
                     "提交訊息明寫事故原因。"
                     "平行衛生條款第二次被驗證，"
                     "這次連考古證據都留下。",
    },
    {
        "tag": "設計 8",
        "title": "並發測試驗契約",
        "sections": [
            ("斷言", "JobSystemTest 驗五件事："
                    "投遞、批次、help-run、"
                    "巢狀、析構排空。"),
            ("性質", "全是契約斷言不是競速斷言——"
                    "「全部完成」「不死鎖」「排空」"
                    "可確定地驗。"),
            ("啟示", "並發測試驗語義不驗時序——"
                    "快慢交給機器，"
                    "對錯交給斷言。"),
        ],
        "code": '// 投遞/批次/help-run/巢狀/排空\nCheck(allDone, "ParallelFor 全完成");\nCheck(!deadlock, "巢狀不死鎖");\n// 驗語義不驗時序',
        "narration": "第八個設計：並發測試驗契約。"
                     "測試驗五件事：投遞、批次、幫跑、"
                     "巢狀、排空——全是契約斷言。"
                     "全部完成、不死鎖、排空，"
                     "都可確定地驗證。"
                     "並發測試驗語義不驗時序："
                     "快慢交給機器，對錯交給斷言。",
    },
    {
        "kind": "cover",
        "title": "第四十七集清單",
        "lines": ["無聊的並發 · help-run 等待 · 巢狀靠推進",
                  "析構排空 · 不做具名 · config 履約",
                  "stale index 二度 · 驗語義不驗時序"],
        "narration": "第四十七集收在這裡。引擎回步第二批——"
                     "上游持續產出，遊戲倉持續受惠。"
                     "而工作池教的是同一件事："
                     "最強的並發設計，"
                     "是把複雜度擋在門外的那種。",
    },
]

SLIDES_EP48 = [
    {
        "kind": "cover",
        "title": "extern C 是國界",
        "lines": ["從實戰學 C++ 第四十八集",
                  "PotatoBridge C ABI · void* 護照 · "
                  "PBS1 二進制快照 · C# 第二張臉"],
        "narration": "第四十八集，引擎學會外交。"
                     "PotatoBridge 用 C ABI 把無頭戰鬥"
                     "核心包成 DLL——C# 客戶端經 P/Invoke "
                     "消費。跨語言邊界的每一個決定，"
                     "都是這系列講過的課。",
    },
    {
        "tag": "設計 1",
        "title": "void* 是護照",
        "sections": [
            ("做法", "PB_Create 回 void*——"
                    "背後是 PB_Battle 聚合 "
                    "controller+fog+resources+索引，"
                    "整個會話一隻把手。"),
            ("細節", "Squad 不直接出境——"
                    "squadId 整數間接定址，"
                    "SquadAt 越界回 nullptr。"),
            ("啟示", "邊界兩邊各管各的所有權——"
                    "C# 看到的是護照號碼，"
                    "物件永遠不過境。"),
        ],
        "code": 'void* h = PB_Create(20,15,1.0f);\nPB_AddSquad(h, "前鋒", 0, x, y, 40);\n// Squad* 不過境——squadId 定址',
        "narration": "第一個設計：不透明指標是護照。"
                     "建構回傳 void 指標，"
                     "背後聚合戰鬥、迷霧、資源和索引。"
                     "小隊不直接出境——整數編號定址，"
                     "越界回空。"
                     "邊界兩邊各管各的所有權，"
                     "物件永遠不過境。",
    },
    {
        "tag": "設計 2",
        "title": "C ABI 不是 C++",
        "sections": [
            ("做法", "extern \"C\" + dllexport/"
                    "visibility——符號名不 mangling，"
                    "P/Invoke 按名字找得到。"),
            ("紀律", "邊界上沒有例外、沒有 STL——"
                    "回傳 int/void*，錯誤是"
                    "回傳碼不是 throw。"),
            ("啟示", "ABI 是語言的公約數——"
                    "C 連結是兩個 runtime "
                    "都聽得懂的方言。"),
        ],
        "code": '#define PB_API extern \"C\" \\\n  __declspec(dllexport)\n// 無例外無 STL——\n// 錯誤走 int 回傳碼',
        "narration": "第二個設計：C ABI 不是 C++。"
                     "外部 C 連結加匯出標記——"
                     "符號名不重整，受控端按名字找得到。"
                     "邊界上沒有例外、沒有標準庫型別，"
                     "錯誤是回傳碼不是拋出。"
                     "ABI 是語言的公約數。",
    },
    {
        "tag": "設計 3",
        "title": "組態走 JSON 幀走二進制",
        "sections": [
            ("分工", "學說設定走 "
                    "PB_SetDoctrineJson——"
                    "一次性組態用文字協定。"),
            ("分工", "渲染快照走二進制——"
                    "每幀資料用結構化位元組，"
                    "JSON 的解析成本不進熱路徑。"),
            ("啟示", "格式按頻率選——"
                    "寫一次的用人讀的，"
                    "讀萬次的用機器讀的。"),
        ],
        "code": 'PB_SetDoctrineJson(h, id, json);\n// 組態=文字（人讀）\nPB_Snapshot(h, dst, cap);\n// 逐幀=二進制（機器讀）',
        "narration": "第三個設計：組態走文字幀走二進制。"
                     "學說設定吃 JSON 字串——"
                     "一次性組態用人讀的協定。"
                     "渲染快照是結構化位元組——"
                     "每幀資料不付解析成本。"
                     "格式按頻率選：寫一次的用人讀的，"
                     "讀萬次的用機器讀的。",
    },
    {
        "tag": "設計 4",
        "title": "綱目即線協定",
        "sections": [
            ("做法", "學說用既有 "
                    "potato.doctrine_set/1 格式——"
                    "檔案格式直接當線協定，"
                    "不發明第二種表示。"),
            ("收益", "存檔能跑的牌組線上也能跑——"
                    "解析器、驗證、降級"
                    "全部免費繼承。"),
            ("啟示", "跨邊界資料用既有 schema——"
                    "每多一種格式，"
                    "就多一個要驗證的宇宙。"),
        ],
        "code": '// doctrine_set/1 檔案格式\n//   = P/Invoke 線協定\n// 解析/驗證/降級免費繼承',
        "narration": "第四個設計：綱目即線協定。"
                     "學說直接用既有的版本化檔案格式——"
                     "檔案格式當線協定，不發明第二種。"
                     "存檔能跑的牌組線上也能跑，"
                     "解析驗證降級全部免費繼承。"
                     "每多一種格式就多一個要驗證的宇宙。",
    },
    {
        "tag": "設計 5",
        "title": "快照有魔數有版本",
        "sections": [
            ("做法", "'PBS1' magic + version=1 開頭——"
                    "little-endian Writer 逐欄寫出，"
                    "讀端照鏡像結構解析。"),
            ("細節", "PB_SnapshotSize 先問大小、"
                    "PB_Snapshot 再填緩衝——"
                    "緩衝歸呼叫端，格式歸 DLL。"),
            ("啟示", "二進制協定第一欄是身分——"
                    "魔數驗「這是不是我的格式」，"
                    "版本驗「是不是我懂的代」。"),
        ],
        "code": 'w.U32(0x31534250); // \'PBS1\'\nw.U32(1);            // version\n// Size→Snapshot 兩段呼叫\n// 緩衝歸呼叫端 格式歸DLL',
        "narration": "第五個設計：快照有魔數有版本。"
                     "協定開頭是魔數加版本號，"
                     "小端寫入器逐欄輸出，"
                     "受控端照鏡像結構解析。"
                     "先問大小再填緩衝的兩段式呼叫——"
                     "緩衝歸呼叫端，格式歸動態庫。"
                     "二進制協定第一欄永遠是身分。",
    },
    {
        "tag": "設計 6",
        "title": "全量不變更集",
        "sections": [
            ("做法", "快照每幀輸出全部格子和小隊——"
                    "「一次性讀不用協調變更集」，"
                    "註解明寫理由。"),
            ("取捨", "差分協定要兩端對齊狀態——"
                    "斷線重連、亂序、掉幀都是坑；"
                    "全量的頻寬換的是零協調。"),
            ("啟示", "簡單協定勝過聰明協定——"
                    "這個規模的資料量，"
                    "對齊成本比頻寬貴。"),
        ],
        "code": '// 全格輸出（0/1）\n// 「一次性讀不用協調變更集」\n// 頻寬換零協調',
        "narration": "第六個設計：全量不變更集。"
                     "快照每幀輸出全部格子和小隊，"
                     "註解明寫不用協調變更集。"
                     "差分協定要兩端對齊狀態——"
                     "掉幀亂序都是坑；"
                     "這個規模的資料量，"
                     "全量頻寬換的是零協調。",
    },
    {
        "tag": "設計 7",
        "title": "C# 是第二張臉",
        "sections": [
            ("事實", "csharp/MingGoRTS.Client 是 "
                    "WPF 客戶端——同一個引擎核心，"
                    "原生 IDE 之外長出 managed 前端。"),
            ("配套", "NativeBridge 包 DllImport、"
                    "GameSession 管會話、"
                    "SelfTest 在受控側自驗。"),
            ("啟示", "橋的品質由異質消費者驗證——"
                    "第二種語言調用時，"
                    "介面的含糊處無所遁形。"),
        ],
        "code": 'csharp/MingGoRTS.Client (WPF)\n  NativeBridge → DllImport 薄殼\n  GameSession  → 會話生命期\n  Snapshot.cs  → 鏡像解析器',
        "narration": "第七個設計：C# 是第二張臉。"
                     "WPF 客戶端用同一個引擎核心——"
                     "原生開發環境之外長出受控前端。"
                     "橋接薄殼、會話管理、"
                     "鏡像解析器各管一層。"
                     "橋的品質由異質消費者驗證："
                     "第二種語言調用時，"
                     "介面的含糊處無所遁形。",
    },
    {
        "tag": "設計 8",
        "title": "錯誤是回傳碼",
        "sections": [
            ("做法", "邊界函式回 int——"
                    "0/1/-1 是全部錯誤語言，"
                    "沒有例外能跨 DLL 邊界。"),
            ("細節", "SquadAt 越界回空、"
                    "Snapshot 容量不足回 0——"
                    "非法輸入安靜降級不崩潰。"),
            ("啟示", "FFI 的防禦在形狀——"
                    "指標檢查、範圍檢查、容量檢查，"
                    "每一個參數都不被信任。"),
        ],
        "code": 'PB_Snapshot: capacity<=0 → return 0\nPB_AddSquad: 失敗 → return -1\n// 每個參數都不被信任\n// 非法輸入降級不崩潰',
        "narration": "第八個設計：錯誤是回傳碼。"
                     "邊界函式回整數——"
                     "零一負一是全部錯誤語言，"
                     "沒有例外能跨邊界。"
                     "越界回空、容量不足回零，"
                     "非法輸入安靜降級不崩潰。"
                     "FFI 的防禦在形狀："
                     "每一個參數都不被信任。",
    },
    {
        "kind": "cover",
        "title": "第四十八集清單",
        "lines": ["void* 護照 · C ABI 方言 · 格式按頻率",
                  "schema 即線協定 · PBS1 魔數 · 全量零協調",
                  "C# 第二張臉 · 錯誤是回傳碼"],
        "narration": "第四十八集收在這裡。引擎第一次"
                     "有了外國人當消費者——"
                     "不透明把手、純 C 方言、"
                     "文字給人、二進制給幀。"
                     "每一條都是舊課："
                     "隔離、契約、誠實標籤，"
                     "只是這次講的是兩種語言。",
    },
]

SLIDES_EP49 = [
    {
        "kind": "cover",
        "title": "偏離追認",
        "lines": ["從實戰學 C++ 第四十九集",
                  "D-5 done · 驗收條大於字面 · "
                  "DLL 自含執行庫 · NSDMI"],
        "narration": "第四十九集，規格被違背——"
                     "然後被追認。D-5 的 scrim 沒用規格寫的"
                     "前景繪製清單，改用背景層——"
                     "因為那樣更滿足驗收條。"
                     "這集講：凍結的文字，"
                     "什麼時候可以被更好的答案推翻。",
    },
    {
        "tag": "設計 1",
        "title": "字面輸給驗收條",
        "sections": [
            ("規格", "凍結區寫：scrim 用 "
                    "ForegroundDrawList 全屏矩形。"),
            ("實作", "改用 GetBackgroundDrawList——"
                    "罩住 3D 場景底色，"
                    "但不遮 HUD 面板。"),
            ("理由", "「關鍵資訊雙編碼恆在」是驗收條——"
                    "前景層會把士氣/倒數/CP 一起罩住，"
                    "字面方案其實不過驗收。"),
        ],
        "code": '- ForegroundDrawList 全屏 rect\n//   → 連 HUD 一起罩住\n+ GetBackgroundDrawList\n//   → 罩場景不罩面板',
        "narration": "第一個設計：字面輸給驗收條。"
                     "凍結規格寫用前景繪製清單，"
                     "實作改用背景層——罩住場景底色，"
                     "但不遮介面面板。"
                     "因為關鍵資訊恆在是驗收條，"
                     "前景層會把士氣倒數指揮點一起罩住——"
                     "照字面做反而不過驗收。",
    },
    {
        "tag": "設計 2",
        "title": "偏離要追認才合法",
        "sections": [
            ("做法", "Review Log 具名條目「偏離追認」"
                    "——寫明偏離了哪條、為什麼、"
                    "哪條驗收被更好滿足。"),
            ("對比", "悶著偏離是欺騙、"
                    "改凍結區是越權——"
                    "追認是第三條路："
                    "留原文字、記偏離理由。"),
            ("啟示", "規格的地位來自記錄的誠實——"
                    "可以被違背的規格"
                    "比假裝全對的規格更可信。"),
        ],
        "code": 'Review Log:\n- 偏離追認: scrim 用\n  BackgroundDrawList（非凍結\n  區 ForegroundDrawList）\n  → 更滿足驗收條',
        "narration": "第二個設計：偏離要追認才合法。"
                     "審查日誌具名條目——寫明偏離哪條、"
                     "為什麼、哪條驗收被更好滿足。"
                     "悶著偏離是欺騙、改凍結區是越權，"
                     "追認是第三條路："
                     "留原文字、記偏離理由。"
                     "可以被違背的規格，"
                     "比假裝全對的規格更可信。",
    },
    {
        "tag": "設計 3",
        "title": "常態路徑跳過建構",
        "sections": [
            ("發現", "L0 常態路徑 lv==level 不會走 "
                    "ForLevel——未初始化的純量欄位"
                    "把垃圾值送進 sync。"),
            ("修法", "SeepageVisual 全欄位補 "
                    "NSDMI 預設值——宣告處即合法，"
                    "不依賴建構路徑。"),
            ("啟示", "「預設=零」要靠宣告保證——"
                    "常態路徑不走初始化函式時，"
                    "欄位預設值是唯一防線。"),
        ],
        "code": 'float shadowOffset = 0.0f; // NSDMI\nfloat jitterAmp    = 0.0f;\nbool themeOverride = false;\n// lv==level 不走 ForLevel 也合法',
        "narration": "第三個設計：常態路徑跳過建構。"
                     "零級常態路徑不走等級建構函式，"
                     "未初始化的欄位把垃圾值送進同步層。"
                     "全部欄位補上宣告處預設值——"
                     "宣告即合法，不依賴建構路徑。"
                     "常態路徑不走初始化時，"
                     "欄位預設值是唯一防線。",
    },
    {
        "tag": "設計 4",
        "title": "讀檔不發事件",
        "sections": [
            ("發現", "音景原本靠 Myths 事件回呼——"
                    "但 FromJson 讀檔不發事件，"
                    "載入的存檔滲透已 3 級卻沒聲音。"),
            ("修法", "改每幀消費 "
                    "svfx.audioLevel——單一映射源，"
                    "讀檔後自動補齊。"),
            ("啟示", "事件回呼只蓋「變化」——"
                    "「已經是這樣」的狀態"
                    "要輪詢才看得到。"),
        ],
        "code": '// FromJson 不發事件\n// → 事件回呼漏掉已載入的等級\n// 改: 每幀 svfx.audioLevel → cues\n//   單一映射源自動補齊',
        "narration": "第四個設計：讀檔不發事件。"
                     "音景原本靠事件回呼驅動——"
                     "但讀檔重建不發事件，"
                     "載入存檔裡已滲透三級的世界"
                     "卻沒有聲音。"
                     "改成每幀消費指令包的音景欄位，"
                     "單一映射源自動補齊。"
                     "事件只蓋變化，輪詢才看得到現狀。",
    },
    {
        "tag": "設計 5",
        "title": "DLL 要自含執行庫",
        "sections": [
            ("發現", "C# 行程的 PATH 沒有 MinGW "
                    "bin——MinGW 編的 PotatoBridge "
                    "載入即 0x8007007E。"),
            ("修法", "target_link_options 加 "
                    "-static-libgcc -static-libstdc++ "
                    "-static——DLL 自含執行庫。"),
            ("啟示", "AGENTS.md 的已知坑"
                    "變成建置規則——"
                    "寫進文件的坑要再寫進 CMake。"),
        ],
        "code": 'if(WIN32 AND NOT MSVC)\n  target_link_options(PotatoBridge\n    PRIVATE -static-libgcc\n            -static-libstdc++ -static)\n// C# 行程 PATH 無 MinGW bin',
        "narration": "第五個設計：DLL 要自含執行庫。"
                     "受控行程的環境路徑沒有 MinGW 目錄，"
                     "動態依賴的橋接庫載入即失敗。"
                     "連結選項加上三個靜態旗標，"
                     "DLL 自含執行庫。"
                     "寫進文件的已知坑，"
                     "還要再寫進建置規則。",
    },
    {
        "tag": "設計 6",
        "title": "Producer 是審查發現的",
        "sections": [
            ("發現", "「duanqiao」區域沒有任何 "
                    "Feed 來源——滲透管線全通"
                    "但上游是空的。"),
            ("修法", "戰後治理事件計數逐筆餵 "
                    "Myths().Feed——暴行注壓，"
                    "區域由此建檔。"),
            ("啟示", "「沒有資料流」是審查"
                    "才抓得到的 bug——"
                    "每個零件都對，"
                    "整條線是斷的。"),
        ],
        "code": 'for (gev : battle.GetGovernanceEvents())\n  for (i < count)\n    Myths().Feed("duanqiao", gev);\n// 零件都對、管線是斷的',
        "narration": "第六個設計：生產者是審查發現的。"
                     "滲透消費端全落地，"
                     "但區域沒有任何餵食來源——"
                     "管線全通、上游是空的。"
                     "戰後治理事件逐筆餵進滲透層，"
                     "區域由此建檔。"
                     "每個零件都對、整條線是斷的——"
                     "這種 bug 只有審查抓得到。",
    },
    {
        "tag": "設計 7",
        "title": "測試家族繼續擴編",
        "sections": [
            ("新增", "ProfilerTest——區段計時契約："
                    "巢狀/啟停/序列化報表。"),
            ("新增", "SignedSaveFileTest——"
                    "簽章存檔往返/竄改偵測/"
                    "錯金鑰/截短四格。"),
            ("啟示", "引擎能力先測試後消費——"
                    "Profiler 和簽章存檔"
                    "還沒人用，契約已先立。"),
        ],
        "code": 'ProfilerTest      區段計時契約\nSignedSaveFileTest 往返/竄改\n                    /錯金鑰/截短\n// 能力未消費、契約已先立',
        "narration": "第七個設計：測試家族繼續擴編。"
                     "區段計時測試驗巢狀啟停和報表，"
                     "簽章存檔測試驗往返竄改偵測"
                     "錯金鑰和截短四格。"
                     "能力還沒被消費、契約已先立——"
                     "引擎的習慣是測試先行。",
    },
    {
        "tag": "設計 8",
        "title": "Epic D 的結帳單",
        "sections": [
            ("狀態", "spec-d5 done + Review Order + "
                    "33/33 雙工具鏈——Epic D 五件"
                    "全部結帳。"),
            ("盤點", "D-1~D-5 各有 spec/實作/"
                    "審查檔/change log——"
                    "一個 epic 五套完整證據。"),
            ("啟示", "流程跑滿的標誌是文件厚度——"
                    "五份 spec、五份審查、"
                    "五份修正紀錄，"
                    "厚度本身就是品質證明。"),
        ],
        "code": 'Epic D 結帳:\n  D-1~D-5 spec done\n  d1~d4 review.diff + d5 log\n  測試 33/33 雙工具鏈',
        "narration": "第八個設計：Epic D 的結帳單。"
                     "規格完成附導讀和雙工具鏈成績——"
                     "五個故事各有一套完整證據："
                     "規格、實作、審查檔、修正紀錄。"
                     "流程跑滿的標誌是文件厚度——"
                     "厚度本身就是品質證明。",
    },
    {
        "kind": "cover",
        "title": "第四十九集清單",
        "lines": ["字面輸給驗收 · 偏離要追認 · NSDMI 防線",
                  "讀檔無事件 · DLL 自含 · producer 補線",
                  "測試先立 · Epic D 結帳"],
        "narration": "第四十九集收在這裡。Epic D 五件"
                     "全部結帳——而最後一課留給規格自己："
                     "凍結的文字可以輸給更好的答案，"
                     "只要偏離被具名、被記錄、被追認。"
                     "誠實的流程不怕被違背。",
    },
]

EPISODES = {
    "ep1": (SLIDES, "cpp_dev_pitfalls_1.mp4"),
    "ep2": (SLIDES_EP2, "cpp_data_integrity_2.mp4"),
    "ep3": (SLIDES_EP3, "cpp_architecture_testing_3.mp4"),
    "ep4": (SLIDES_EP4, "cpp_gameplay_systems_4.mp4"),
    "ep5": (SLIDES_EP5, "cpp_compatibility_5.mp4"),
    "ep6": (SLIDES_EP6, "cpp_repo_split_6.mp4"),
    "ep7": (SLIDES_EP7, "cpp_two_ais_7.mp4"),
    "ep8": (SLIDES_EP8, "cpp_self_hosting_8.mp4"),
    "ep9": (SLIDES_EP9, "cpp_unsolved_problems_9.mp4"),
    "ep10": (SLIDES_EP10, "cpp_governance_design_10.mp4"),
    "ep11": (SLIDES_EP11, "cpp_battle_physicality_11.mp4"),
    "ep12": (SLIDES_EP12, "cpp_ledger_assurance_12.mp4"),
    "ep13": (SLIDES_EP13, "cpp_shelf_adoption_13.mp4"),
    "ep14": (SLIDES_EP14, "cpp_split_rehearsal_14.mp4"),
    "ep15": (SLIDES_EP15, "cpp_test_migration_15.mp4"),
    "ep16": (SLIDES_EP16, "cpp_spec_anatomy_16.mp4"),
    "ep17": (SLIDES_EP17, "cpp_no_battle_paths_17.mp4"),
    "ep18": (SLIDES_EP18, "cpp_resolver_verdicts_18.mp4"),
    "ep19": (SLIDES_EP19, "cpp_review_payoff_19.mp4"),
    "ep20": (SLIDES_EP20, "cpp_honest_approximation_20.mp4"),
    "ep21": (SLIDES_EP21, "cpp_review_map_21.mp4"),
    "ep22": (SLIDES_EP22, "cpp_statistical_assertions_22.mp4"),
    "ep23": (SLIDES_EP23, "cpp_history_surgery_23.mp4"),
    "ep24": (SLIDES_EP24, "cpp_field_pattern_24.mp4"),
    "ep25": (SLIDES_EP25, "cpp_narrative_audit_25.mp4"),
    "ep26": (SLIDES_EP26, "cpp_peace_register_26.mp4"),
    "ep27": (SLIDES_EP27, "cpp_review_muscle_27.mp4"),
    "ep28": (SLIDES_EP28, "cpp_ui_scale_28.mp4"),
    "ep29": (SLIDES_EP29, "cpp_headless_ui_29.mp4"),
    "ep30": (SLIDES_EP30, "cpp_engine_repo_30.mp4"),
    "ep31": (SLIDES_EP31, "cpp_fate_exchange_31.mp4"),
    "ep32": (SLIDES_EP32, "cpp_rules_in_tools_32.mp4"),
    "ep33": (SLIDES_EP33, "cpp_rl_contract_33.mp4"),
    "ep34": (SLIDES_EP34, "cpp_honest_exchange_34.mp4"),
    "ep35": (SLIDES_EP35, "cpp_spec_changelog_35.mp4"),
    "ep36": (SLIDES_EP36, "cpp_asset_verification_36.mp4"),
    "ep37": (SLIDES_EP37, "cpp_myth_incursion_37.mp4"),
    "ep38": (SLIDES_EP38, "cpp_audio_degradation_38.mp4"),
    "ep39": (SLIDES_EP39, "cpp_incursion_review_39.mp4"),
    "ep40": (SLIDES_EP40, "cpp_hud_density_40.mp4"),
    "ep41": (SLIDES_EP41, "cpp_audit_trail_41.mp4"),
    "ep42": (SLIDES_EP42, "cpp_seepage_visual_spec_42.mp4"),
    "ep43": (SLIDES_EP43, "cpp_seepage_stage_43.mp4"),
    "ep44": (SLIDES_EP44, "cpp_data_protection_44.mp4"),
    "ep45": (SLIDES_EP45, "cpp_see_and_click_45.mp4"),
    "ep46": (SLIDES_EP46, "cpp_mirror_sandbox_46.mp4"),
    "ep47": (SLIDES_EP47, "cpp_job_system_47.mp4"),
    "ep48": (SLIDES_EP48, "cpp_c_abi_bridge_48.mp4"),
    "ep49": (SLIDES_EP49, "cpp_ratified_deviation_49.mp4"),
}


if __name__ == "__main__":
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    key = args[0] if args else "ep1"
    slides, fname = EPISODES[key]
    work = OUT / key
    frames_d, wavs_d, segs_d = work / "frames", work / "wavs", work / "segs"
    final = ROOT / "output" / "videos" / fname
    render_slides(slides, frames_d)
    if "--skip-tts" in sys.argv:
        durs = measure_durations(slides, wavs_d)
    else:
        durs = synth_narration(slides, wavs_d, work)
    encode(durs, frames_d, wavs_d, segs_d, work, final)
