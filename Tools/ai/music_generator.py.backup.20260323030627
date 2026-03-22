#!/usr/bin/env python3
"""
MingGoRTS AI Music Generator - 本地音樂生成腳本
使用 Hugging Face Transformers + MusicGen
"""

import os
import sys
import argparse
from pathlib import Path

# 設定輸出路徑
OUTPUT_BASE = Path("C:/HW/MingGoRTS/Content/Audio/Generated")

def setup_environment():
    """設定環境，檢查相依套件"""
    print("🎵 MingGoRTS AI Music Generator")
    print("=" * 50)
    
    try:
        import torch
        print(f"✓ PyTorch 已安裝: {torch.__version__}")
        
        # 檢查 CUDA
        if torch.cuda.is_available():
            print(f"✓ CUDA 可用: {torch.cuda.get_device_name(0)}")
            device = "cuda"
        else:
            print("ℹ 使用 CPU 模式（無 GPU）")
            device = "cpu"
            
    except ImportError:
        print("✗ PyTorch 未安裝")
        print("  請執行: pip install torch --index-url https://download.pytorch.org/whl/cpu")
        sys.exit(1)
    
    try:
        import transformers
        print(f"✓ Transformers 已安裝: {transformers.__version__}")
    except ImportError:
        print("✗ Transformers 未安裝")
        print("  請執行: pip install transformers accelerate")
        sys.exit(1)
    
    return device

def create_output_dirs():
    """創建輸出目錄結構"""
    dirs = [
        OUTPUT_BASE / "Music" / "Main",
        OUTPUT_BASE / "Music" / "Background",
        OUTPUT_BASE / "Music" / "Battle",
        OUTPUT_BASE / "SoundEffects" / "UI",
        OUTPUT_BASE / "SoundEffects" / "Combat",
        OUTPUT_BASE / "SoundEffects" / "Environment",
    ]
    
    for d in dirs:
        d.mkdir(parents=True, exist_ok=True)
        
    return OUTPUT_BASE

def generate_music(prompt: str, duration: int = 10, output_name: str = "generated", style: str = "default"):
    """
    生成音樂
    
    Args:
        prompt: 音樂描述提示詞
        duration: 音樂長度（秒）
        output_name: 輸出檔名
        style: 預設風格
    """
    import scipy.io.wavfile as wavfile
    
    print(f"\n🎼 生成音樂: {prompt[:50]}...")
    print(f"   長度: {duration}秒 | 風格: {style}")
    
    try:
        # 載入模型
        model_name = "facebook/musicgen-small"
        
        print(f"   載入模型: {model_name}...")
        processor = AutoProcessor.from_pretrained(model_name)
        
        # 使用 trust_remote_code 和修正的 config 載入
        model = MusicgenForConditionalGeneration.from_pretrained(
            model_name,
            trust_remote_code=True,
            use_safetensors=True
        )
        
        # 設定為評估模式
        model.eval()
        
        # 處理輸入
        inputs = processor(
            text=[prompt],
            padding=True,
            return_tensors="pt",
        )
        
        # 生成參數
        max_length = min(duration * 50, 1500)  # 限制最大長度
        
        print("   生成中...")
        with torch.no_grad():
            audio_values = model.generate(
                **inputs,
                max_new_tokens=max_length,
                do_sample=True,
                guidance_scale=3.0,
            )
        
        # 準備輸出
        output_path = OUTPUT_BASE / "Music" / style / f"{output_name}.wav"
        output_path.parent.mkdir(parents=True, exist_ok=True)
        
        # 儲存音訊
        sampling_rate = model.config.audio_encoder.sampling_rate
        audio_np = audio_values[0, 0].cpu().numpy()
        
        # 標準化音量
        audio_np = audio_np / max(abs(audio_np.max()), abs(audio_np.min()))
        
        wavfile.write(str(output_path), rate=sampling_rate, data=audio_np)
        
        print(f"   ✓ 已儲存: {output_path}")
        return str(output_path)
        
    except Exception as e:
        print(f"   ✗ 生成失敗: {e}")
        print("   嘗試使用替代方案...")
        return generate_placeholder_music(duration, output_name, style)

def generate_placeholder_music(duration: int, output_name: str, style: str):
    """生成進階合成音樂（FM合成 + 多音色 + 節奏）"""
    import numpy as np
    import scipy.io.wavfile as wavfile
    from scipy import signal
    
    print(f"   生成進階合成音樂...")
    
    sample_rate = 44100  # CD 品質
    num_samples = int(sample_rate * duration)
    t = np.linspace(0, duration, num_samples, False)
    
    # 根據風格定義參數
    style_params = {
        "Main": {
            "base_freq": 220,      # A3
            "harmonics": [1, 2, 3, 5],
            "amplitudes": [0.4, 0.25, 0.15, 0.1],
            "mod_freq": 440,       # 調製頻率
            "mod_index": 2.0,
            "tempo": 120,          # BPM
            "brightness": 0.6
        },
        "Battle": {
            "base_freq": 110,      # A2 - 更低更震撼
            "harmonics": [1, 2, 4, 8],
            "amplitudes": [0.5, 0.3, 0.2, 0.15],
            "mod_freq": 220,
            "mod_index": 3.5,
            "tempo": 140,
            "brightness": 0.8
        },
        "Background": {
            "base_freq": 330,      # E4 - 較高更柔和
            "harmonics": [1, 1.5, 2, 3],
            "amplitudes": [0.35, 0.2, 0.15, 0.1],
            "mod_freq": 330,
            "mod_index": 1.2,
            "tempo": 90,
            "brightness": 0.3
        }
    }
    
    params = style_params.get(style, style_params["Background"])
    
    # FM 合成主音色
    base = params["base_freq"]
    mod = params["mod_freq"]
    idx = params["mod_index"]
    
    # FM 合成：carrier + modulator
    modulator = np.sin(2 * np.pi * mod * t)
    carrier_freq = base + idx * mod * modulator
    audio = np.sin(2 * np.pi * np.cumsum(carrier_freq) / sample_rate)
    
    # 添加泛音（豐富音色）
    for i, harm in enumerate(params["harmonics"]):
        freq = base * harm
        amp = params["amplitudes"][i] if i < len(params["amplitudes"]) else 0.1
        # 每個泛音使用不同相位
        phase = np.random.random() * 2 * np.pi
        audio += amp * np.sin(2 * np.pi * freq * t + phase)
    
    # 添加節奏（低音鼓 + 軍鼓模式）
    beat_duration = 60.0 / params["tempo"]
    num_beats = int(duration / beat_duration)
    
    for beat in range(num_beats):
        beat_start = int(beat * beat_duration * sample_rate)
        beat_end = min(beat_start + int(0.1 * sample_rate), num_samples)
        
        if beat_end > beat_start:
            # 低音鼓（每小節第1拍）
            if beat % 4 == 0:
                kick_t = np.linspace(0, 0.1, beat_end - beat_start, False)
                kick_freq = 60 * np.exp(-kick_t * 30)  # 指數衰減頻率
                kick = np.sin(2 * np.pi * np.cumsum(kick_freq) / sample_rate * 10) * 0.4
                kick *= np.exp(-kick_t * 20)  # 音量包絡
                audio[beat_start:beat_end] += kick
            
            # 軍鼓（第2、4拍）
            if beat % 4 == 2:
                snare_t = np.linspace(0, 0.05, beat_end - beat_start, False)
                snare = np.random.randn(beat_end - beat_start) * 0.25
                snare *= np.exp(-snare_t * 40)  # 快速衰減
                audio[beat_start:beat_end] += snare
    
    # 添加弦樂式長音（背景填充）
    if style == "Background" or style == "Main":
        for chord_tone in [1.0, 1.25, 1.5]:  # 大三和弦
            chord_freq = base * chord_tone
            chord = np.sin(2 * np.pi * chord_freq * t) * 0.15
            # 緩慢顫音
            vibrato = 1 + 0.01 * np.sin(2 * np.pi * 5 * t)
            chord *= vibrato
            audio += chord
    
    # 低通濾波器（讓聲音更溫和）
    if style == "Background":
        sos = signal.butter(4, 2000, 'low', fs=sample_rate, output='sos')
        audio = signal.sosfilt(sos, audio)
    
    # 立體聲效果（簡單延遲）
    delay_samples = int(0.02 * sample_rate)  # 20ms 延遲
    stereo_left = audio.copy()
    stereo_right = np.roll(audio, delay_samples) * 0.3 + audio * 0.7
    
    # 合併為立體聲
    stereo_audio = np.column_stack((stereo_left, stereo_right))
    
    # 標準化音量
    max_val = np.abs(stereo_audio).max()
    if max_val > 0:
        stereo_audio = stereo_audio / max_val * 0.9
    
    # 淡入淡出
    fade_duration = min(2.0, duration * 0.1)  # 最多2秒
    fade_samples = int(fade_duration * sample_rate)
    fade_in = np.linspace(0, 1, fade_samples)
    fade_out = np.linspace(1, 0, fade_samples)
    
    stereo_audio[:fade_samples, :] *= fade_in[:, np.newaxis]
    stereo_audio[-fade_samples:, :] *= fade_out[:, np.newaxis]
    
    # 轉換為 16-bit PCM
    stereo_audio = (stereo_audio * 32767).astype(np.int16)
    
    # 儲存
    output_path = OUTPUT_BASE / "Music" / style / f"{output_name}.wav"
    output_path.parent.mkdir(parents=True, exist_ok=True)
    wavfile.write(str(output_path), rate=sample_rate, data=stereo_audio)
    
    print(f"   ✓ 已儲存進階合成音樂: {output_path}")
    print(f"     參數: {params['base_freq']}Hz | {params['tempo']}BPM | FM合成 | 立體聲")
    return str(output_path)
PRESETS = {
    "main_theme": {
        "prompt": "Epic orchestral military theme music, Chinese Republican era 1912-1949, heroic and patriotic, brass fanfare with traditional Chinese instruments erhu and pipa, majestic and inspiring, cinematic quality, 120 BPM",
        "duration": 30,
        "folder": "Main",
    },
    "menu_background": {
        "prompt": "Peaceful classical background music, Chinese traditional style mixed with Western orchestra, calm and grand atmosphere, suitable for strategy game menu, ambient and relaxing",
        "duration": 30,
        "folder": "Background",
    },
    "battle": {
        "prompt": "Intense battle music, orchestral action, Chinese war drums and military brass, tense and epic atmosphere, fast tempo 140 BPM, cinematic combat music",
        "duration": 30,
        "folder": "Battle",
    },
    "victory": {
        "prompt": "Triumphant victory fanfare, Chinese Republican era military celebration, brass and percussion, heroic and glorious, short celebratory music",
        "duration": 15,
        "folder": "Battle",
    },
    "building": {
        "prompt": "Productive construction music, light classical with Chinese folk elements, peaceful and productive atmosphere, base building strategy game music",
        "duration": 30,
        "folder": "Background",
    },
}

def generate_preset(preset_name: str):
    """使用預設風格生成音樂"""
    if preset_name not in PRESETS:
        print(f"✗ 未知預設: {preset_name}")
        print(f"   可用預設: {', '.join(PRESETS.keys())}")
        return
    
    preset = PRESETS[preset_name]
    return generate_music(
        prompt=preset["prompt"],
        duration=preset["duration"],
        output_name=preset_name,
        style=preset["folder"],
    )

def generate_all_presets():
    """生成所有預設音樂"""
    print("\n🎯 批量生成所有預設音樂...")
    
    results = []
    for preset_name in PRESETS:
        try:
            path = generate_preset(preset_name)
            results.append((preset_name, path, True))
        except Exception as e:
            print(f"   ✗ {preset_name} 失敗: {e}")
            results.append((preset_name, None, False))
    
    print("\n" + "=" * 50)
    print("📊 生成結果:")
    for name, path, success in results:
        status = "✓" if success else "✗"
        print(f"   {status} {name}: {path if path else '失敗'}")

def main():
    parser = argparse.ArgumentParser(description="MingGoRTS AI Music Generator")
    parser.add_argument("--preset", type=str, help=f"使用預設風格: {', '.join(PRESETS.keys())}")
    parser.add_argument("--prompt", type=str, help="自定義提示詞")
    parser.add_argument("--duration", type=int, default=10, help="音樂長度（秒）")
    parser.add_argument("--output", type=str, default="generated", help="輸出檔名")
    parser.add_argument("--all", action="store_true", help="生成所有預設")
    parser.add_argument("--list", action="store_true", help="列出所有預設")
    
    args = parser.parse_args()
    
    # 設定環境
    device = setup_environment()
    create_output_dirs()
    
    if args.list:
        print("\n📋 可用預設風格:")
        for name, config in PRESETS.items():
            print(f"   • {name}: {config['duration']}秒 - {config['prompt'][:60]}...")
        return
    
    if args.all:
        generate_all_presets()
    elif args.preset:
        generate_preset(args.preset)
    elif args.prompt:
        generate_music(args.prompt, args.duration, args.output)
    else:
        # 預設行為：顯示說明並生成範例
        print("\n📖 使用方法:")
        print("   python music_generator.py --preset main_theme")
        print("   python music_generator.py --prompt 'your custom prompt' --duration 20")
        print("   python music_generator.py --all")
        print("   python music_generator.py --list")
        print("\n🎵 首次執行會下載模型（約 3.7GB），請耐心等待...")
        
        # 詢問是否生成範例
        response = input("\n是否生成範例音樂 'main_theme'? (y/n): ")
        if response.lower() == 'y':
            generate_preset("main_theme")

if __name__ == "__main__":
    main()
