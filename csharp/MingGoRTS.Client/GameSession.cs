// GameSession — 斷橋場景建構 + 逐步驅動 + 快照取回。
// WPF 視窗與 --selftest 無頭路徑共用這一層。
namespace MingGoRTS.Client;

public sealed class GameSession : IDisposable
{
    public IntPtr Handle { get; }
    public int GridW { get; }
    public int GridH { get; }
    public float Cell { get; }
    private byte[] buf;

    private GameSession(IntPtr h, int w, int g, float cell)
    {
        Handle = h;
        GridW = w; GridH = g; Cell = cell;
        buf = new byte[Math.Max(4096, NativeBridge.PB_SnapshotSize(h) + 64)];
    }

    // 斷橋鏡像場（與 BattleSandboxCore 同一張圖）：20×15、
    // 中央牆缺口 y6-8、雙方各四隊。玩家 team0 拿 60CP + 8 情報。
    public static GameSession CreateDuanqiao()
    {
        const int W = 20, H = 15;
        var h = NativeBridge.PB_Create(W, H, 1.0f);
        if (h == IntPtr.Zero) throw new InvalidOperationException("PB_Create failed");
        var s = new GameSession(h, W, H, 1.0f);

        for (int y = 0; y < H; ++y)
            if (y < 6 || y > 8) NativeBridge.PB_SetObstacle(h, 10, y, 1);
        // G-4 地形層展示：高地（橋頭兩側）+ 泥濘（河岸帶）
        NativeBridge.PB_SetTerrain(h, 9, 7, 1);  // Highland
        NativeBridge.PB_SetTerrain(h, 11, 7, 1);
        for (int y = 0; y < H; ++y)
        {
            NativeBridge.PB_SetTerrain(h, 5, y, 3); // Mud
            NativeBridge.PB_SetTerrain(h, 15, y, 3);
        }

        NativeBridge.PB_SetObjective(h, 0, 18, 7);
        NativeBridge.PB_SetObjective(h, 1, 2, 7);
        NativeBridge.PB_SetRallyPoint(h, 0, 1, 7);
        NativeBridge.PB_SetRallyPoint(h, 1, 19, 7);
        NativeBridge.PB_SetCommandPoints(h, 0, 60);
        NativeBridge.PB_SetCommandPoints(h, 1, 0);
        NativeBridge.PB_SetRoutShock(h, 4.0f, 0.20f); // G-1 連鎖潰逃
        NativeBridge.PB_SetJamming(h, 1.5f);          // P-3 渡口擁擠

        NativeBridge.PB_EnableFog(h);
        NativeBridge.PB_SetupResources(h, 0, 8, 60); // intel=8, cp=60
        NativeBridge.PB_SetFogRevealRange(h, 3.0f);

        const string atkDoctrine =
            "{\"schema\":\"potato.doctrine_set/1\",\"rules\":[" +
            "{\"trigger\":\"EnemyInRange\",\"action\":\"AttackNearest\",\"threshold\":4.0,\"priority\":10}," +
            "{\"trigger\":\"AllyEngaged\",\"action\":\"DefendNearestAlly\",\"threshold\":0,\"priority\":50}," +
            "{\"trigger\":\"Always\",\"action\":\"AdvanceToObjective\",\"threshold\":0,\"priority\":100}]}";
        const string defDoctrine =
            "{\"schema\":\"potato.doctrine_set/1\",\"rules\":[" +
            "{\"trigger\":\"EnemyInRange\",\"action\":\"AttackNearest\",\"threshold\":4.0,\"priority\":10}," +
            "{\"trigger\":\"AllyEngaged\",\"action\":\"DefendNearestAlly\",\"threshold\":0,\"priority\":50}," +
            "{\"trigger\":\"Always\",\"action\":\"HoldPosition\",\"threshold\":0,\"priority\":100}]}";

        var atkPos = new (string n, float x, float y, int m, int cls)[]
        {
            ("前鋒", 3, 4, 12, 0), ("左翼", 2, 7, 11, 2),
            ("右翼", 3, 10, 11, 1), ("預備隊", 5, 7, 10, 0),
        };
        foreach (var (n, x, y, m, cls) in atkPos)
        {
            int id = NativeBridge.PB_AddSquad(h, n, 0, x, y, m);
            NativeBridge.PB_SetUnitClass(h, id, cls);
            NativeBridge.PB_SetDoctrineJson(h, id, atkDoctrine);
        }

        var defPos = new (string n, float x, float y, int m)[]
        {
            ("橋頭守軍", 12, 7, 9), ("北段守軍", 11, 3, 8),
            ("南段守軍", 11, 11, 8), ("城內守軍", 14, 7, 10),
        };
        foreach (var (n, x, y, m) in defPos)
        {
            int id = NativeBridge.PB_AddSquad(h, n, 1, x, y, m);
            NativeBridge.PB_SetDoctrineJson(h, id, defDoctrine);
            // Q-1：每支守軍一朵機率雲（真實位置附近散佈）
            int eid = NativeBridge.PB_FogAddCloud(h, n, 1, x, y,
                                                2.5f, 5, 1.2f);
            if (eid >= 0) NativeBridge.PB_FogBindSquad(h, id, eid);
        }

        if (NativeBridge.PB_BeginExecution(h) == 0)
            throw new InvalidOperationException("BeginExecution failed");
        return s;
    }

    public Snapshot? Step(float realDt)
    {
        NativeBridge.PB_Step(Handle, realDt);
        int need = NativeBridge.PB_SnapshotSize(Handle);
        if (need > buf.Length) buf = new byte[need + 64];
        int n = NativeBridge.PB_Snapshot(Handle, buf, buf.Length);
        return n > 0 ? Snapshot.Parse(buf, n) : null;
    }

    public void Dispose() => NativeBridge.PB_Destroy(Handle);
}
