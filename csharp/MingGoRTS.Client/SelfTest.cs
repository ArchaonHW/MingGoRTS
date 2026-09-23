// SelfTest — 無頭橋接驗證：建場景 → 步進 → 快照完整性。
// 供 `MingGoRTS.Client --selftest` 在無 GUI/CI 環境跑端到端。
namespace MingGoRTS.Client;

internal static class SelfTest
{
    public static int Run()
    {
        try
        {
            using var s = GameSession.CreateDuanqiao();
            Snapshot? last = null;
            int events = 0;
            for (int i = 0; i < 200; ++i)
            {
                last = s.Step(0.05f); // 10s
                events += s.DrainEvents().Length;
            }
            if (events <= 0)
            {
                Console.Error.WriteLine("no events drained");
                return 1;
            }
            if (last == null) { Console.Error.WriteLine("snapshot null"); return 1; }
            if (last.Squads.Count != 8)
            {
                Console.Error.WriteLine($"squads={last.Squads.Count} != 8");
                return 1;
            }
            if (last.GridW != 20 || last.GridH != 15)
            {
                Console.Error.WriteLine("grid mismatch");
                return 1;
            }
            if (last.Fogs.Count != 4)
            {
                Console.Error.WriteLine($"fogs={last.Fogs.Count} != 4");
                return 1;
            }
            int moved = last.Squads.Count(q =>
                q.Team == 0 && (q.X > 5.5f || q.Engaged));
            Console.WriteLine(
                $"selftest OK: squads={last.Squads.Count} " +
                $"fogs={last.Fogs.Count} elapsed={last.Elapsed:F1}s " +
                $"outcome={last.Outcome} moved={moved} " +
                $"events={events}");
            return moved > 0 ? 0 : 1; // 攻方 doctrine 應已推進
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine($"selftest failed: {ex.Message}");
            return 1;
        }
    }
}
