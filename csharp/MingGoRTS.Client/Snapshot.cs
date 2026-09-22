// Snapshot — PB_Snapshot 二進制格式的 C# 讀端（little-endian）。
// 版面對齊 Bridge/PotatoBridge.cpp 的 Writer——改格式要兩端同版。
namespace MingGoRTS.Client;

public sealed class SquadView
{
    public int Id, Team, Members, MaxMembers;
    public float X, Y, Morale;
    public int Order;
    public float OrderTX, OrderTY;
    public bool Engaged, Routing, Eliminated, UnderAttack, GeneralGuard;
    public int FogEntityId, UnitClass;
}

public sealed class FogCandidate { public float X, Y, Prob; }

public sealed class FogView
{
    public int EntityId, OwnerSquadId;
    public bool Revealed;
    public float RevealedX, RevealedY;
    public List<FogCandidate> Candidates = new();
}

public sealed class Snapshot
{
    public float Elapsed;
    public int Phase, Outcome, Cp0, Cp1, Intel0, Intel1;
    public float TimeScale;
    public int GridW, GridH;
    public byte[] Blocked = Array.Empty<byte>();   // 每格 0/1
    public byte[] Terrain = Array.Empty<byte>();
    public List<SquadView> Squads = new();
    public List<FogView> Fogs = new();

    public bool IsBlocked(int x, int y) =>
        x >= 0 && y >= 0 && x < GridW && y < GridH &&
        Blocked[y * GridW + x] != 0;

    public static Snapshot? Parse(byte[] buf, int len)
    {
        if (len < 48) return null;
        int p = 0;
        int I32() { int v = BitConverter.ToInt32(buf, p); p += 4; return v; }
        float F32() { float v = BitConverter.ToSingle(buf, p); p += 4; return v; }
        byte U8() => buf[p++];

        if (BitConverter.ToUInt32(buf, 0) != 0x31534250u) return null; // 'PBS1'
        p = 4;
        if (I32() != 1) return null; // version

        var s = new Snapshot
        {
            Elapsed = F32(),
            Phase = I32(),
            Outcome = I32(),
            TimeScale = F32(),
            Cp0 = I32(),
            Cp1 = I32(),
            Intel0 = I32(),
            Intel1 = I32(),
            GridW = I32(),
            GridH = I32(),
        };
        int cells = I32();
        if (cells != s.GridW * s.GridH || p + cells * 2 > len) return null;
        s.Blocked = new byte[cells];
        s.Terrain = new byte[cells];
        for (int i = 0; i < cells; ++i) { s.Blocked[i] = U8(); s.Terrain[i] = U8(); }

        int sc = I32();
        for (int i = 0; i < sc; ++i)
        {
            var sv = new SquadView
            {
                Id = I32(), Team = I32(),
                X = F32(), Y = F32(),
                Members = I32(), MaxMembers = I32(),
                Morale = F32(), Order = I32(),
                OrderTX = F32(), OrderTY = F32(),
            };
            sv.Engaged = I32() != 0;
            sv.Routing = I32() != 0;
            sv.Eliminated = I32() != 0;
            sv.UnderAttack = I32() != 0;
            sv.FogEntityId = I32();
            sv.UnitClass = I32();
            sv.GeneralGuard = I32() != 0;
            s.Squads.Add(sv);
        }

        int fc = I32();
        for (int i = 0; i < fc; ++i)
        {
            var fv = new FogView
            {
                EntityId = I32(),
                Revealed = I32() != 0,
                OwnerSquadId = I32(),
                RevealedX = F32(),
                RevealedY = F32(),
            };
            int cc = I32();
            for (int c = 0; c < cc; ++c)
            {
                fv.Candidates.Add(new FogCandidate
                {
                    X = F32(), Y = F32(), Prob = F32(),
                });
            }
            s.Fogs.Add(fv);
        }
        return s;
    }
}
