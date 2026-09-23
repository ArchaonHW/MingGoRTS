// Av — 旁白合成與 ffmpeg 封裝。
// TTS 走 PowerShell System.Speech（與 tools/gen_cpp_tutorial.py
// 同一條產線，免 NuGet 依賴）；時長用 ffprobe 量；
// 片段 ffmpeg 編碼後 concat 成正片。
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Text.Json;

namespace MingGoRTS.Client;

internal static class Av
{
    public const int Fps = 30;
    private const double AudioPad = 0.6; // 旁白後畫面停留秒數

    // 每場景產一支 narr_{i}.wav，回傳各段時長（含 pad）
    public static double[] SynthNarration(Scene[] scenes, string wavsDir,
        string workDir)
    {
        Directory.CreateDirectory(wavsDir);
        var manifest = Path.Combine(workDir, "narration.json");
        File.WriteAllText(manifest,
            JsonSerializer.Serialize(
                scenes.Select(s => s.Narration).ToArray()),
            new UTF8Encoding(false));
        var ps = @"
Add-Type -AssemblyName System.Speech
$texts = Get-Content -Raw -Encoding UTF8 """ + manifest.Replace("\\", "/") + @""" | ConvertFrom-Json
for ($i = 0; $i -lt $texts.Count; $i++) {
    $s = New-Object System.Speech.Synthesis.SpeechSynthesizer
    $s.SelectVoice(""Microsoft Hanhan Desktop"")
    $s.Rate = 0
    $f = """ + wavsDir.Replace("\\", "/") + @"/"" + (""narr_{0:d2}.wav"" -f $i)
    $s.SetOutputToWaveFile($f)
    $s.Speak($texts[$i]); $s.Dispose()
}";
        Run("powershell", ["-NoProfile", "-Command", ps]);
        return scenes.Select((_, i) =>
            ProbeDuration(Path.Combine(wavsDir, $"narr_{i:d2}.wav"))
            + AudioPad).ToArray();
    }

    public static double ProbeDuration(string wav)
    {
        var outp = Run("ffprobe",
            ["-v", "quiet", "-show_entries", "format=duration",
             "-of", "csv=p=0", wav]);
        return double.Parse(outp.Trim(),
            System.Globalization.CultureInfo.InvariantCulture);
    }

    // Card 場景：單幀循環 + 旁白 → 片段
    public static void EncodeCard(string png, string wav, double dur,
        string seg)
    {
        Run("ffmpeg",
            ["-y", "-loop", "1", "-framerate", Fps.ToString(),
             "-i", png, "-i", wav,
             "-af", "apad", "-t", dur.ToString("F2",
                 System.Globalization.CultureInfo.InvariantCulture),
             "-vf", "format=yuv420p",
             "-c:v", "libx264", "-preset", "medium", "-crf", "20",
             "-c:a", "aac", "-b:a", "128k", "-ar", "44100",
             "-movflags", "+faststart", seg]);
    }

    // Battle 場景：逐幀序列 + 旁白 → 片段
    public static void EncodeFrames(string framesPattern, string wav,
        double dur, string seg)
    {
        Run("ffmpeg",
            ["-y", "-framerate", Fps.ToString(),
             "-i", framesPattern, "-i", wav,
             "-af", "apad", "-t", dur.ToString("F2",
                 System.Globalization.CultureInfo.InvariantCulture),
             "-vf", "format=yuv420p",
             "-c:v", "libx264", "-preset", "medium", "-crf", "20",
             "-c:a", "aac", "-b:a", "128k", "-ar", "44100",
             "-movflags", "+faststart", seg]);
    }

    public static void Concat(string[] segs, string workDir,
        string final)
    {
        var list = Path.Combine(workDir, "concat.txt");
        File.WriteAllText(list,
            string.Concat(segs.Select(s =>
                $"file '{s.Replace("\\", "/")}'\n")),
            new UTF8Encoding(false));
        Run("ffmpeg",
            ["-y", "-f", "concat", "-safe", "0", "-i", list,
             "-c", "copy", "-movflags", "+faststart", final]);
    }

    private static string Run(string exe, string[] args)
    {
        var psi = new ProcessStartInfo(exe)
        {
            RedirectStandardOutput = true,
            RedirectStandardError = true,
            UseShellExecute = false,
        };
        foreach (var a in args) psi.ArgumentList.Add(a);
        using var p = Process.Start(psi)!;
        // stdout/stderr 同步讀會互等緩衝區死鎖——ffmpeg 進度全走 stderr
        var errTask = p.StandardError.ReadToEndAsync();
        var outp = p.StandardOutput.ReadToEnd();
        var err = errTask.GetAwaiter().GetResult();
        p.WaitForExit();
        if (p.ExitCode != 0)
            throw new InvalidOperationException(
                $"{exe} exit {p.ExitCode}: {err[..Math.Min(err.Length, 500)]}");
        return outp;
    }
}
