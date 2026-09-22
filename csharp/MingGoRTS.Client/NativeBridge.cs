// NativeBridge — PotatoBridge.dll 的 P/Invoke 宣告與載入解析。
//
// DLL 搜尋序：exe 旁 → repo build/bin/Release → build-mingw/bin
// （MinGW 產物前綴 lib）。selftest/無裝置環境以 LoadLibrary 顯式
// 探路後才觸發 P/Invoke 繫結。
using System.IO;
using System.Runtime.InteropServices;

namespace MingGoRTS.Client;

internal static class NativeBridge
{
    private const string Dll = "PotatoBridge";

    static NativeBridge()
    {
        // 顯式探路：候選路徑逐一 LoadLibrary，成功即讓後續
        // DllImport("PotatoBridge") 命中已載入模組
        var baseDir = AppContext.BaseDirectory;
        var candidates = new[]
        {
            Path.Combine(baseDir, "PotatoBridge.dll"),
            Path.Combine(baseDir, "libPotatoBridge.dll"),
            Path.GetFullPath(Path.Combine(baseDir, "..", "..", "..", "..", "..", "build", "bin", "Release", "PotatoBridge.dll")),
            Path.GetFullPath(Path.Combine(baseDir, "..", "..", "..", "..", "..", "build-mingw", "bin", "libPotatoBridge.dll")),
            Path.GetFullPath(Path.Combine(baseDir, "..", "..", "..", "..", "..", "build-mingw", "bin", "PotatoBridge.dll")),
        };
        foreach (var c in candidates)
        {
            if (File.Exists(c) && NativeLibrary.TryLoad(c, out _))
                return;
        }
        // 皆未命中 → 讓預設解析（PATH）接手；失敗在首次呼叫時拋出
    }

    // ---- 生命週期 ----
    [DllImport(Dll)] internal static extern IntPtr PB_Create(int gridW, int gridH, float cellSize);
    [DllImport(Dll)] internal static extern void PB_Destroy(IntPtr h);

    // ---- 部署 ----
    [DllImport(Dll)] internal static extern void PB_SetObstacle(IntPtr h, int x, int y, int blocked);
    [DllImport(Dll)] internal static extern void PB_SetTerrain(IntPtr h, int x, int y, int terrain);
    [DllImport(Dll)] internal static extern void PB_SetObjective(IntPtr h, int team, float x, float y);
    [DllImport(Dll)] internal static extern void PB_SetRallyPoint(IntPtr h, int team, float x, float y);
    [DllImport(Dll)] internal static extern void PB_SetCommandPoints(IntPtr h, int team, int points);
    [DllImport(Dll)] internal static extern void PB_SetRoutShock(IntPtr h, float radius, float moraleHit);
    [DllImport(Dll)] internal static extern void PB_SetCombatWidth(IntPtr h, int width);
    [DllImport(Dll)] internal static extern void PB_SetJamming(IntPtr h, float radius);
    [DllImport(Dll, CharSet = CharSet.Ansi)]
    internal static extern int PB_AddSquad(IntPtr h, string name, int team, float x, float y, int members);
    [DllImport(Dll)] internal static extern void PB_SetUnitClass(IntPtr h, int squadId, int cls);
    [DllImport(Dll)] internal static extern void PB_SetGeneralGuard(IntPtr h, int squadId, int v);
    [DllImport(Dll)] internal static extern void PB_SetSpeed(IntPtr h, int squadId, float speed);
    [DllImport(Dll, CharSet = CharSet.Ansi)]
    internal static extern int PB_SetDoctrineJson(IntPtr h, int squadId, string json);
    [DllImport(Dll)] internal static extern int PB_BeginExecution(IntPtr h);

    // ---- 敵情霧 ----
    [DllImport(Dll)] internal static extern void PB_EnableFog(IntPtr h);
    [DllImport(Dll)] internal static extern void PB_SetupResources(IntPtr h, int team, int intel, int cp);
    [DllImport(Dll)] internal static extern void PB_AddIntel(IntPtr h, int team, int amount);
    [DllImport(Dll, CharSet = CharSet.Ansi)]
    internal static extern int PB_FogAddCloud(IntPtr h, string name, int team,
        float centerX, float centerY, float radius, int count, float minSpacing);
    [DllImport(Dll)] internal static extern int PB_FogBindSquad(IntPtr h, int squadId, int entityId);
    [DllImport(Dll)] internal static extern int PB_FogObserve(IntPtr h, int entityId, float x, float y);
    [DllImport(Dll)] internal static extern int PB_FogProbe(IntPtr h, int entityId, float x, float y, float strength);
    [DllImport(Dll)] internal static extern int PB_FogReveal(IntPtr h, int entityId, float x, float y);
    [DllImport(Dll)] internal static extern void PB_SetFogRevealRange(IntPtr h, float cells);

    // ---- 執行 ----
    [DllImport(Dll)] internal static extern void PB_Step(IntPtr h, float realDt);
    [DllImport(Dll)] internal static extern void PB_SetTimeScale(IntPtr h, float scale);
    [DllImport(Dll)] internal static extern float PB_GetTimeScale(IntPtr h);
    [DllImport(Dll)] internal static extern int PB_InterveneMove(IntPtr h, int squadId, int order,
        float x, float y, float holdSeconds);
    [DllImport(Dll)] internal static extern int PB_InterveneEngage(IntPtr h, int squadId, int targetSquadId, float holdSeconds);
    [DllImport(Dll)] internal static extern int PB_GeneralRally(IntPtr h, int squadId);
    [DllImport(Dll)] internal static extern int PB_GeneralCharge(IntPtr h, int squadId);

    // ---- 快照 ----
    [DllImport(Dll)] internal static extern int PB_SnapshotSize(IntPtr h);
    [DllImport(Dll)] internal static extern int PB_Snapshot(IntPtr h, byte[] dst, int capacity);

    // ---- 戰況卷軸 ----
    [DllImport(Dll)] internal static extern int PB_EventBytes(IntPtr h);
    [DllImport(Dll)] internal static extern int PB_DrainEvents(IntPtr h, byte[] dst, int capacity);
}
