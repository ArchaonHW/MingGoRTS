// Render — 宣傳片影格渲染：DrawingVisual → RenderTargetBitmap → PNG。
// 全部軟體渲染（不需視窗/GPU），1280×720。
// Card = 標題卡靜幀；Battle = 無頭戰鬥快照逐幀（風格對齊 MainWindow）。
using System.IO;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace MingGoRTS.Client;

internal static class Render
{
    public const int W = 1280, H = 720;

    private static readonly Color BgCol = Color.FromRgb(0x16, 0x19, 0x20);
    private static readonly Brush BrT0 = Freeze(Color.FromRgb(0x35, 0xD0, 0xC5));
    private static readonly Brush BrT1 = Freeze(Color.FromRgb(0xC9, 0x50, 0x4E));
    private static readonly Brush BrGold = Freeze(Color.FromRgb(0xC9, 0xA2, 0x27));
    private static readonly Brush BrFog = Freeze(Color.FromArgb(120, 0x6E, 0x50, 0x8C));
    private static readonly Brush BrBlock = Freeze(Color.FromRgb(0x3A, 0x44, 0x4E));
    private static readonly Brush BrHigh = Freeze(Color.FromRgb(0x33, 0x3E, 0x33));
    private static readonly Brush BrMud = Freeze(Color.FromRgb(0x2E, 0x28, 0x22));
    private static readonly Brush BrForest = Freeze(Color.FromRgb(0x24, 0x33, 0x26));
    private static readonly Brush BrHp = Freeze(Color.FromRgb(0x4C, 0xAF, 0x50));
    private static readonly Brush BrDim = Freeze(Color.FromRgb(0x8E, 0x9A, 0xA4));

    private static FontFamily? serif, sans;

    private static Brush Freeze(Color c)
    {
        var b = new SolidColorBrush(c);
        b.Freeze();
        return b;
    }

    //  bundled CJK（F-5）；找不到就退回系統正黑體
    private static void EnsureFonts()
    {
        if (serif != null) return;
        var dir = Path.GetFullPath(Path.Combine(
            AppContext.BaseDirectory,
            "..", "..", "..", "..", "..", "assets", "fonts"));
        try
        {
            var uri = new Uri(dir + "/");
            serif = new FontFamily(uri, "./#Noto Serif TC");
            sans = new FontFamily(uri, "./#Noto Sans TC");
        }
        catch
        {
            serif = new FontFamily("Microsoft JhengHei");
            sans = serif;
        }
    }

    private static FormattedText T(string s, double size,
        Brush brush, bool serifFace = false)
    {
        EnsureFonts();
        return new FormattedText(s,
            System.Globalization.CultureInfo.GetCultureInfo("zh-TW"),
            FlowDirection.LeftToRight,
            new Typeface(serifFace ? serif! : sans!,
                FontStyles.Normal, FontWeights.Bold, FontStretches.Normal),
            size, brush, 1.0);
    }

    private static void Save(DrawingVisual v, string path)
    {
        var bmp = new RenderTargetBitmap(W, H, 96, 96,
            PixelFormats.Pbgra32);
        bmp.Render(v);
        var enc = new PngBitmapEncoder();
        enc.Frames.Add(BitmapFrame.Create(bmp));
        using var fs = File.Create(path);
        enc.Save(fs);
    }

    // ---- 標題卡 ----
    public static void Card(Scene s, string path)
    {
        var v = new DrawingVisual();
        using (var dc = v.RenderOpen())
        {
            dc.DrawRectangle(new SolidColorBrush(BgCol), null,
                new Rect(0, 0, W, H));
            // 金線框——章回體扉頁感
            var pen = new Pen(BrGold, 2); pen.Freeze();
            dc.DrawRectangle(null, pen, new Rect(40, 40, W - 80, H - 80));

            var title = T(s.Title, 92, BrGold, serifFace: true);
            dc.DrawText(title, new Point(
                (W - title.Width) / 2, H * 0.30));

            double y = H * 0.52;
            foreach (var line in s.Lines)
            {
                var t = T(line, 30, BrDim);
                dc.DrawText(t, new Point((W - t.Width) / 2, y));
                y += t.Height + 14;
            }
        }
        Save(v, path);
    }

    // ---- 戰鬥影格 ----
    public static void Battle(Snapshot s, GameSession ses,
        string subtitle, string path)
    {
        var v = new DrawingVisual();
        using (var dc = v.RenderOpen())
        {
            dc.DrawRectangle(new SolidColorBrush(BgCol), null,
                new Rect(0, 0, W, H));

            // 世界置中映射
            double cp = Math.Min((W - 160.0) / s.GridW,
                                 (H - 140.0) / s.GridH);
            double ox = (W - cp * s.GridW) / 2;
            double oy = (H - cp * s.GridH) / 2 - 10;

            // 格線（淡描，戰場可讀性）
            var gridPen = new Pen(
                new SolidColorBrush(Color.FromArgb(28, 0x9F, 0xB2, 0xBD)),
                1);
            gridPen.Freeze();
            dc.DrawRectangle(null, gridPen,
                new Rect(ox, oy, cp * s.GridW, cp * s.GridH));

            // 地形
            for (int y = 0; y < s.GridH; ++y)
            for (int x = 0; x < s.GridW; ++x)
            {
                int i = y * s.GridW + x;
                Brush? fill = null;
                if (s.Blocked[i] != 0) fill = BrBlock;
                else if (s.Terrain[i] == 1) fill = BrHigh;
                else if (s.Terrain[i] == 2) fill = BrForest;
                else if (s.Terrain[i] == 3) fill = BrMud;
                if (fill != null)
                    dc.DrawRectangle(fill, null,
                        new Rect(ox + x * cp, oy + y * cp, cp, cp));
            }

            // 未揭露雲
            foreach (var f in s.Fogs)
            {
                if (f.Revealed) continue;
                foreach (var c in f.Candidates)
                {
                    double sz = cp * (0.5 + c.Prob * 1.6);
                    dc.DrawEllipse(BrFog, null,
                        new Point(ox + c.X * cp, oy + c.Y * cp),
                        sz / 2, sz / 2);
                }
            }

            // 指令目標線（我方有向指令）
            var ordPen = new Pen(
                new SolidColorBrush(Color.FromArgb(160, 0xD4, 0xA2, 0x3C)),
                1.5) { DashStyle = DashStyles.Dash };
            ordPen.Freeze();
            foreach (var q in s.Squads)
            {
                if (q.Team != 0 || q.Eliminated || q.Order == 0) continue;
                if (q.OrderTX == 0 && q.OrderTY == 0) continue;
                dc.DrawLine(ordPen,
                    new Point(ox + q.X * cp, oy + q.Y * cp),
                    new Point(ox + q.OrderTX * cp, oy + q.OrderTY * cp));
            }

            // 小隊
            var hidden = s.Fogs
                .Where(f => !f.Revealed && f.OwnerSquadId >= 0)
                .Select(f => f.OwnerSquadId).ToHashSet();
            foreach (var q in s.Squads)
            {
                if (q.Eliminated || hidden.Contains(q.Id)) continue;
                double sz = cp * 0.7;
                var ctr = new Point(ox + q.X * cp, oy + q.Y * cp);
                var pen = q.Id == 0 ? new Pen(BrGold, 2) : null;
                var body = new SolidColorBrush(q.Team == 0
                    ? Color.FromRgb(0x35, 0xD0, 0xC5)
                    : Color.FromRgb(0xC9, 0x50, 0x4E))
                { Opacity = q.Routing ? 0.45 : 1.0 };
                dc.DrawEllipse(body, pen, ctr, sz / 2, sz / 2);

                double bw = cp * 0.9;
                dc.DrawRectangle(BrHp, null, new Rect(
                    ctr.X - bw / 2, ctr.Y - sz / 2 - 10,
                    bw * q.Members / Math.Max(1, q.MaxMembers), 4));
                dc.DrawRectangle(BrGold, null, new Rect(
                    ctr.X - bw / 2, ctr.Y - sz / 2 - 5,
                    bw * Math.Clamp(q.Morale, 0f, 1f), 3));
            }

            // HUD 行（頂部）
            string hud = s.Outcome switch
            {
                1 => "戰 勝", 2 => "戰 敗", 3 => "和 局",
                _ => $"交 戰　{s.Elapsed:F0}s　CP {s.Cp0}　情報 {s.Intel0}",
            };
            dc.DrawText(T(hud, 24, BrGold), new Point(48, 48));

            // 底部字幕條
            if (subtitle.Length > 0)
            {
                dc.DrawRectangle(
                    new SolidColorBrush(Color.FromArgb(180, 0, 0, 0)),
                    null, new Rect(0, H - 84, W, 84));
                var sub = T(subtitle, 28, Brushes.White);
                dc.DrawText(sub, new Point(
                    (W - sub.Width) / 2, H - 62));
            }
        }
        Save(v, path);
    }
}
