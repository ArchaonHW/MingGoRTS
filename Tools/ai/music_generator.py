#!/usr/bin/env python3
"""
MingGoRTS AI Music Generator - ?砍?單????單
雿輻 Hugging Face Transformers + MusicGen
"""

import os
import sys
import argparse
from pathlib import Path

# 閮剖?頛詨頝臬?
OUTPUT_BASE = Path("C:/HW/MingGoRTS/Content/Audio/Generated")

def setup_environment():
    """閮剖??啣?嚗炎?亦靘?隞?""
    print("? MingGoRTS AI Music Generator")
    print("=" * 50)
    
    try:
        import torch
        print(f"??PyTorch 撌脣?鋆? {torch.__version__}")
        
        # 瑼Ｘ CUDA
        if torch.cuda.is_available():
            print(f"??CUDA ?舐: {torch.cuda.get_device_name(0)}")
            device = "cuda"
        else:
            print("??雿輻 CPU 璅∪?嚗 GPU嚗?)
            device = "cpu"
            
    except ImportError:
        print("??PyTorch ?芸?鋆?)
        print("  隢銵? pip install torch --index-url https://download.pytorch.org/whl/cpu")
        sys.exit(1)
    
    try:
        import transformers
        print(f"??Transformers 撌脣?鋆? {transformers.__version__}")
    except ImportError:
        print("??Transformers ?芸?鋆?)
        print("  隢銵? pip install transformers accelerate")
        sys.exit(1)
    
    return device

def create_output_dirs():
    """?萄遣頛詨?桅?蝯?"""
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
    ???單?
    
    Args:
        prompt: ?單??膩?內閰?        duration: ?單??瑕漲嚗?嚗?        output_name: 頛詨瑼?
        style: ?身憸冽
    """
    import scipy.io.wavfile as wavfile
    
    print(f"\n? ???單?: {prompt[:50]}...")
    print(f"   ?瑕漲: {duration}蝘?| 憸冽: {style}")
    
    try:
        # 頛璅∪?
        model_name = "facebook/musicgen-small"
        
        print(f"   頛璅∪?: {model_name}...")
        processor = AutoProcessor.from_pretrained(model_name)
        
        # 雿輻 trust_remote_code ?耨甇?? config 頛
        model = MusicgenForConditionalGeneration.from_pretrained(
            model_name,
            trust_remote_code=True,
            use_safetensors=True
        )
        
        # 閮剖??箄?隡唳芋撘?        model.eval()
        
        # ??頛詨
        inputs = processor(
            text=[prompt],
            padding=True,
            return_tensors="pt",
        )
        
        # ???
        max_length = min(duration * 50, 1500)  # ??憭折摨?        
        print("   ??銝?..")
        with torch.no_grad():
            audio_values = model.generate(
                **inputs,
                max_new_tokens=max_length,
                do_sample=True,
                guidance_scale=3.0,
            )
        
        # 皞?頛詨
        output_path = OUTPUT_BASE / "Music" / style / f"{output_name}.wav"
        output_path.parent.mkdir(parents=True, exist_ok=True)
        
        # ?脣??唾?
        sampling_rate = model.config.audio_encoder.sampling_rate
        audio_np = audio_values[0, 0].cpu().numpy()
        
        # 璅????        audio_np = audio_np / max(abs(audio_np.max()), abs(audio_np.min()))
        
        wavfile.write(str(output_path), rate=sampling_rate, data=audio_np)
        
        print(f"   ??撌脣摮? {output_path}")
        return str(output_path)
        
    except Exception as e:
        print(f"   ????憭望?: {e}")
        print("   ?岫雿輻?蹂誨?寞?...")
        return generate_placeholder_music(duration, output_name, style)

def generate_placeholder_music(duration: int, output_name: str, style: str):
    """???脤????單?嚗M?? + 憭??+ 蝭憟?"""
    import numpy as np
    import scipy.io.wavfile as wavfile
    from scipy import signal
    
    print(f"   ???脤????單?...")
    
    sample_rate = 44100  # CD ?釭
    num_samples = int(sample_rate * duration)
    t = np.linspace(0, duration, num_samples, False)
    
    # ?寞?憸冽摰儔?
    style_params = {
        "Main": {
            "base_freq": 220,      # A3
            "harmonics": [1, 2, 3, 5],
            "amplitudes": [0.4, 0.25, 0.15, 0.1],
            "mod_freq": 440,       # 隤輯ˊ?餌?
            "mod_index": 2.0,
            "tempo": 120,          # BPM
            "brightness": 0.6
        },
        "Battle": {
            "base_freq": 110,      # A2 - ?港??湧???            "harmonics": [1, 2, 4, 8],
            "amplitudes": [0.5, 0.3, 0.2, 0.15],
            "mod_freq": 220,
            "mod_index": 3.5,
            "tempo": 140,
            "brightness": 0.8
        },
        "Background": {
            "base_freq": 330,      # E4 - 頛??湔???            "harmonics": [1, 1.5, 2, 3],
            "amplitudes": [0.35, 0.2, 0.15, 0.1],
            "mod_freq": 330,
            "mod_index": 1.2,
            "tempo": 90,
            "brightness": 0.3
        }
    }
    
    params = style_params.get(style, style_params["Background"])
    
    # FM ??銝駁??    base = params["base_freq"]
    mod = params["mod_freq"]
    idx = params["mod_index"]
    
    # FM ??嚗arrier + modulator
    modulator = np.sin(2 * np.pi * mod * t)
    carrier_freq = base + idx * mod * modulator
    audio = np.sin(2 * np.pi * np.cumsum(carrier_freq) / sample_rate)
    
    # 瘛餃?瘜嚗?撖?莎?
    for i, harm in enumerate(params["harmonics"]):
        freq = base * harm
        amp = params["amplitudes"][i] if i < len(params["amplitudes"]) else 0.1
        # 瘥??喃蝙?其??雿?        phase = np.random.random() * 2 * np.pi
        audio += amp * np.sin(2 * np.pi * freq * t + phase)
    
    # 瘛餃?蝭憟?雿曌?+ 頠?璅∪?嚗?    beat_duration = 60.0 / params["tempo"]
    num_beats = int(duration / beat_duration)
    
    for beat in range(num_beats):
        beat_start = int(beat * beat_duration * sample_rate)
        beat_end = min(beat_start + int(0.1 * sample_rate), num_samples)
        
        if beat_end > beat_start:
            # 雿曌?瘥?蝭蝚???
            if beat % 4 == 0:
                kick_t = np.linspace(0, 0.1, beat_end - beat_start, False)
                kick_freq = 60 * np.exp(-kick_t * 30)  # ?銵唳??餌?
                kick = np.sin(2 * np.pi * np.cumsum(kick_freq) / sample_rate * 10) * 0.4
                kick *= np.exp(-kick_t * 20)  # ?喲??窗
                audio[beat_start:beat_end] += kick
            
            # 頠?嚗洵2????
            if beat % 4 == 2:
                snare_t = np.linspace(0, 0.05, beat_end - beat_start, False)
                snare = np.random.randn(beat_end - beat_start) * 0.25
                snare *= np.exp(-snare_t * 40)  # 敹恍※皜?                audio[beat_start:beat_end] += snare
    
    # 瘛餃?撘行?撘?喉??憛怠?嚗?    if style == "Background" or style == "Main":
        for chord_tone in [1.0, 1.25, 1.5]:  # 憭找??憐
            chord_freq = base * chord_tone
            chord = np.sin(2 * np.pi * chord_freq * t) * 0.15
            # 蝺拇憿恍
            vibrato = 1 + 0.01 * np.sin(2 * np.pi * 5 * t)
            chord *= vibrato
            audio += chord
    
    # 雿蕪瘜Ｗ嚗??脤?湔澈??
    if style == "Background":
        sos = signal.butter(4, 2000, 'low', fs=sample_rate, output='sos')
        audio = signal.sosfilt(sos, audio)
    
    # 蝡??脫???蝪∪撱園嚗?    delay_samples = int(0.02 * sample_rate)  # 20ms 撱園
    stereo_left = audio.copy()
    stereo_right = np.roll(audio, delay_samples) * 0.3 + audio * 0.7
    
    # ?蔥?箇?擃
    stereo_audio = np.column_stack((stereo_left, stereo_right))
    
    # 璅????    max_val = np.abs(stereo_audio).max()
    if max_val > 0:
        stereo_audio = stereo_audio / max_val * 0.9
    
    # 瘛∪瘛∪
    fade_duration = min(2.0, duration * 0.1)  # ?憭?蝘?    fade_samples = int(fade_duration * sample_rate)
    fade_in = np.linspace(0, 1, fade_samples)
    fade_out = np.linspace(1, 0, fade_samples)
    
    stereo_audio[:fade_samples, :] *= fade_in[:, np.newaxis]
    stereo_audio[-fade_samples:, :] *= fade_out[:, np.newaxis]
    
    # 頧???16-bit PCM
    stereo_audio = (stereo_audio * 32767).astype(np.int16)
    
    # ?脣?
    output_path = OUTPUT_BASE / "Music" / style / f"{output_name}.wav"
    output_path.parent.mkdir(parents=True, exist_ok=True)
    wavfile.write(str(output_path), rate=sample_rate, data=stereo_audio)
    
    print(f"   ??撌脣摮脤????單?: {output_path}")
    print(f"     ?: {params['base_freq']}Hz | {params['tempo']}BPM | FM?? | 蝡???)
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
    """雿輻?身憸冽???單?"""
    if preset_name not in PRESETS:
        print(f"???芰?身: {preset_name}")
        print(f"   ?舐?身: {', '.join(PRESETS.keys())}")
        return
    
    preset = PRESETS[preset_name]
    return generate_music(
        prompt=preset["prompt"],
        duration=preset["duration"],
        output_name=preset_name,
        style=preset["folder"],
    )

def generate_all_presets():
    """?????閮剝璅?""
    print("\n? ?寥??????閮剝璅?..")
    
    results = []
    for preset_name in PRESETS:
        try:
            path = generate_preset(preset_name)
            results.append((preset_name, path, True))
        except Exception as e:
            print(f"   ??{preset_name} 憭望?: {e}")
            results.append((preset_name, None, False))
    
    print("\n" + "=" * 50)
    print("?? ??蝯?:")
    for name, path, success in results:
        status = "?? if success else "??
        print(f"   {status} {name}: {path if path else '憭望?'}")

def main():
    parser = argparse.ArgumentParser(description="MingGoRTS AI Music Generator")
    parser.add_argument("--preset", type=str, help=f"雿輻?身憸冽: {', '.join(PRESETS.keys())}")
    parser.add_argument("--prompt", type=str, help="?芸?蝢拇?蝷箄?")
    parser.add_argument("--duration", type=int, default=10, help="?單??瑕漲嚗?嚗?)
    parser.add_argument("--output", type=str, default="generated", help="頛詨瑼?")
    parser.add_argument("--all", action="store_true", help="?????閮?)
    parser.add_argument("--list", action="store_true", help="????閮?)
    
    args = parser.parse_args()
    
    # 閮剖??啣?
    device = setup_environment()
    create_output_dirs()
    
    if args.list:
        print("\n?? ?舐?身憸冽:")
        for name, config in PRESETS.items():
            print(f"   ??{name}: {config['duration']}蝘?- {config['prompt'][:60]}...")
        return
    
    if args.all:
        generate_all_presets()
    elif args.preset:
        generate_preset(args.preset)
    elif args.prompt:
        generate_music(args.prompt, args.duration, args.output)
    else:
        # ?身銵嚗＊蝷箄牧?蒂??蝭?
        print("\n?? 雿輻?寞?:")
        print("   python music_generator.py --preset main_theme")
        print("   python music_generator.py --prompt 'your custom prompt' --duration 20")
        print("   python music_generator.py --all")
        print("   python music_generator.py --list")
        print("\n? 擐活?瑁???頛芋??蝝?3.7GB嚗?隢?蝑?...")
        
        # 閰Ｗ??臬??蝭?
        response = input("\n?臬??蝭??單? 'main_theme'? (y/n): ")
        if response.lower() == 'y':
            generate_preset("main_theme")

if __name__ == "__main__":
    main()

