// MingGoRTS.Trailer — 民國史詩宣傳片產生器。
//
// 管線（對齊 tools/gen_cpp_tutorial.py 的三段式）：
//   1. PowerShell System.Speech 合成各場景旁白 wav，ffprobe 量時長
//   2. 場景影格：Card=靜幀；Battle=GameSession 無頭戰鬥 30fps 逐幀
//   3. ffmpeg 逐段編碼（畫面時長=旁白+pad）→ concat 成最終 mp4
//
// 用法：MingGoRTS.Trailer [--out <mp4>] [--frames-only]
using System.IO;

namespace MingGoRTS.Client;

internal static class Program
{
    [STAThread]
    private static int Main(string[] args)
    {
        string repo = Path.GetFullPath(Path.Combine(
            AppContext.BaseDirectory, "..", "..", "..", "..", ".."));
        string outDir = Path.Combine(repo, "output", "trailer");
        string final = Path.Combine(outDir, "duanqiao_trailer.mp4");
        for (int i = 0; i + 1 < args.Length; ++i)
            if (args[i] == "--out") final = Path.GetFullPath(args[i + 1]);

        var scenes = Scene.Script();
        string workDir = Path.Combine(outDir, "work");
        string wavsDir = Path.Combine(outDir, "wavs");
        string segsDir = Path.Combine(outDir, "segs");
        Directory.CreateDirectory(workDir);
        Directory.CreateDirectory(segsDir);

        Console.WriteLine("[tts] synthesizing narration...");
        double[] durs = Av.SynthNarration(scenes, wavsDir, workDir);

        var segs = new List<string>();
        GameSession? battle = null;
        try
        {
            for (int i = 0; i < scenes.Length; ++i)
            {
                var sc = scenes[i];
                string wav = Path.Combine(wavsDir, $"narr_{i:d2}.wav");
                string seg = Path.Combine(segsDir, $"seg_{i:d2}.mp4");
                double dur = Math.Max(durs[i], sc.MinSeconds);

                if (sc.Kind == SceneKind.Card)
                {
                    string png = Path.Combine(workDir,
                        $"card_{i:d2}.png");
                    Render.Card(sc, png);
                    Av.EncodeCard(png, wav, dur, seg);
                    Console.WriteLine(
                        $"[seg {i}] card {dur:F1}s -> {seg}");
                }
                else
                {
                    if (battle == null)
                    {
                        battle = GameSession.CreateDuanqiao();
                        // 宣傳片慢動作：0.5× 讓接戰/潰逃在鏡頭裡留得住
                        NativeBridge.PB_SetTimeScale(battle.Handle,
                            0.5f);
                    }
                    string fdir = Path.Combine(workDir, $"scene_{i:d2}");
                    Directory.CreateDirectory(fdir);
                    int frames = (int)Math.Ceiling(dur * Av.Fps);
                    ApplyCue(sc.Cue, battle);
                    for (int f = 0; f < frames; ++f)
                    {
                        var snap = battle.Step(1.0f / Av.Fps);
                        if (snap == null) break;
                        Render.Battle(snap, battle,
                            string.Join(" ", sc.Lines),
                            Path.Combine(fdir, $"f_{f:d4}.png"));
                    }
                    Av.EncodeFrames(
                        Path.Combine(fdir, "f_%04d.png"), wav,
                        dur, seg);
                    Console.WriteLine(
                        $"[seg {i}] battle {frames}f/{dur:F1}s");
                }
                segs.Add(seg);
            }
        }
        finally
        {
            battle?.Dispose();
        }

        Av.Concat(segs.ToArray(), workDir, final);
        Console.WriteLine(
            $"[done] {final} ({new FileInfo(final).Length / 1e6:F1} MB)");
        return 0;
    }

    // 腳本指令：advance=前鋒攻擊移動貫穿缺口；probe=探測第一朵未揭露雲
    private static void ApplyCue(string? cue, GameSession s)
    {
        switch (cue)
        {
            case "advance":
                NativeBridge.PB_InterveneMove(s.Handle, 0, 2,
                    18.0f, 7.0f, 30.0f);
                break;
            case "probe":
            {
                var snap = s.Step(0.0001f);
                var fog = snap?.Fogs.FirstOrDefault(f => !f.Revealed);
                var c = fog?.Candidates
                    .OrderByDescending(c => c.Prob).FirstOrDefault();
                if (fog != null && c != null)
                    NativeBridge.PB_FogProbe(s.Handle, fog.EntityId,
                        c.X, c.Y, 0.5f);
                break;
            }
        }
    }
}
