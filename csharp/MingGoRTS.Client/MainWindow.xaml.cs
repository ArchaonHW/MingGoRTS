// MainWindow — C# 戰場渲染端：每 ~33ms PB_Step + PB_Snapshot →
// Canvas 重畫。地形/障礙/小隊/機率雲/HUD 全由此端呈現，
// C++ 只跑無頭 BattleController。
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace MingGoRTS.Client;

public partial class MainWindow : Window
{
    private readonly GameSession session;
    private readonly DispatcherTimer timer;
    private Snapshot? snap;
    private int selected = -1;
    private bool paused;
    private float timeScale = 1.0f;
    private DateTime lastTick = DateTime.UtcNow;

    // SquadOrder 序數（與 C++ enum 對齊）
    private const int OrderAttackMove = 2;

    private static readonly Brush BrT0 = new SolidColorBrush(Color.FromRgb(0x35, 0xD0, 0xC5));
    private static readonly Brush BrT1 = new SolidColorBrush(Color.FromRgb(0xC9, 0x50, 0x4E));
    private static readonly Brush BrSel = new SolidColorBrush(Color.FromRgb(0xD4, 0xA2, 0x3C));
    private static readonly Brush BrFog = new SolidColorBrush(Color.FromArgb(110, 0x6E, 0x50, 0x8C));
    private static readonly Brush BrBlock = new SolidColorBrush(Color.FromRgb(0x3A, 0x44, 0x4E));
    private static readonly Brush BrHigh = new SolidColorBrush(Color.FromRgb(0x33, 0x3E, 0x33));
    private static readonly Brush BrMud = new SolidColorBrush(Color.FromRgb(0x2E, 0x28, 0x22));
    private static readonly Brush BrHp = new SolidColorBrush(Color.FromRgb(0x4C, 0xAF, 0x50));
    private static readonly Brush BrMorale = new SolidColorBrush(Color.FromRgb(0xD4, 0xA2, 0x3C));
    private static readonly Brush BrText = Brushes.White;

    public MainWindow()
    {
        InitializeComponent();
        session = GameSession.CreateDuanqiao();
        timer = new DispatcherTimer { Interval = TimeSpan.FromMilliseconds(33) };
        timer.Tick += (_, _) => Tick();
        timer.Start();
        KeyDown += MainWindow_KeyDown;
        Closed += (_, _) => session.Dispose();
    }

    private double Px(float world) => world / session.Cell * CellPx();
    private double CellPx() => Math.Min(
        Field.ActualWidth / session.GridW,
        Field.ActualHeight / session.GridH);
    private double Ox() => (Field.ActualWidth - CellPx() * session.GridW) / 2;
    private double Oy() => (Field.ActualHeight - CellPx() * session.GridH) / 2;

    private void Tick()
    {
        var now = DateTime.UtcNow;
        float dt = (float)Math.Min((now - lastTick).TotalSeconds, 0.1);
        lastTick = now;

        NativeBridge.PB_SetTimeScale(session.Handle,
            paused ? 0.0f : timeScale);
        snap = session.Step(paused ? 0.0001f : dt);
        if (snap == null) return;
        Render();
        UpdateHud();
    }

    private void Render()
    {
        var s = snap!;
        Field.Children.Clear();
        double cp = CellPx();

        // 地形 + 障礙（每格）
        for (int y = 0; y < s.GridH; ++y)
        {
            for (int x = 0; x < s.GridW; ++x)
            {
                int i = y * s.GridW + x;
                Brush? fill = null;
                if (s.Blocked[i] != 0) fill = BrBlock;
                else if (s.Terrain[i] == 1) fill = BrHigh;
                else if (s.Terrain[i] == 3) fill = BrMud;
                else if (s.Terrain[i] == 2) fill =
                    new SolidColorBrush(Color.FromRgb(0x24, 0x33, 0x26));
                if (fill == null) continue;
                var r = new Rectangle
                {
                    Width = cp, Height = cp, Fill = fill,
                };
                Canvas.SetLeft(r, Ox() + x * cp);
                Canvas.SetTop(r, Oy() + y * cp);
                Field.Children.Add(r);
            }
        }

        // 未揭露敵情雲（機率→大小與透明度）
        foreach (var f in s.Fogs)
        {
            if (f.Revealed) continue;
            foreach (var c in f.Candidates)
            {
                double sz = cp * (0.5 + c.Prob * 1.6);
                var e = new Ellipse
                {
                    Width = sz, Height = sz, Fill = BrFog,
                };
                Canvas.SetLeft(e, Ox() + c.X * cp - sz / 2);
                Canvas.SetTop(e, Oy() + c.Y * cp - sz / 2);
                Field.Children.Add(e);
            }
        }

        // 小隊（被未揭露雲綁定的敵隊由雲代表，不畫真身）
        var hiddenEntities = s.Fogs
            .Where(f => !f.Revealed && f.OwnerSquadId >= 0)
            .Select(f => f.OwnerSquadId).ToHashSet();
        foreach (var q in s.Squads)
        {
            if (q.Eliminated) continue;
            if (hiddenEntities.Contains(q.Id)) continue;

            double sz = cp * 0.7;
            double cx = Ox() + q.X * cp, cy = Oy() + q.Y * cp;
            var e = new Ellipse
            {
                Width = sz, Height = sz,
                Fill = q.Team == 0 ? BrT0 : BrT1,
                Stroke = q.Id == selected ? BrSel : Brushes.Transparent,
                StrokeThickness = 2,
                Opacity = q.Routing ? 0.45 : 1.0,
            };
            Canvas.SetLeft(e, cx - sz / 2);
            Canvas.SetTop(e, cy - sz / 2);
            Field.Children.Add(e);

            // 兵力條 + 士氣條
            double bw = cp * 0.9;
            var hp = new Rectangle
            {
                Width = bw * q.Members / Math.Max(1, q.MaxMembers),
                Height = 3, Fill = BrHp,
            };
            Canvas.SetLeft(hp, cx - bw / 2);
            Canvas.SetTop(hp, cy - sz / 2 - 8);
            Field.Children.Add(hp);
            var mo = new Rectangle
            {
                Width = bw * Math.Clamp(q.Morale, 0f, 1f),
                Height = 2, Fill = BrMorale,
            };
            Canvas.SetLeft(mo, cx - bw / 2);
            Canvas.SetTop(mo, cy - sz / 2 - 4);
            Field.Children.Add(mo);
        }
    }

    private void UpdateHud()
    {
        var s = snap!;
        HudPhase.Text = s.Outcome switch
        {
            1 => "戰 勝", 2 => "戰 敗", 3 => "和 局",
            _ => paused ? "暫 停" : "交 戰",
        };
        HudClock.Text = $"時鐘 {s.Elapsed:F1}s";
        HudCp.Text = $"CP {s.Cp0}　情報 {s.Intel0}";
        HudIntel.Text = $"速度 ×{timeScale:0.##}";

        if (selected >= 0 && selected < s.Squads.Count)
        {
            var q = s.Squads[selected];
            HudSelected.Text = q.Eliminated
                ? "已全滅"
                : $"[{q.Id}] 兵力 {q.Members}/{q.MaxMembers} " +
                  $"士氣 {q.Morale:P0}{(q.Routing ? " 潰逃" : "")}";
        }
        else HudSelected.Text = "";

        HudOutcome.Text = s.Outcome switch
        {
            1 => "斷橋已下——勝利",
            2 => "全軍覆沒——戰敗",
            3 => "鳴金收兵——和局",
            _ => "",
        };
    }

    private Point WorldAt(MouseButtonEventArgs e)
    {
        var p = e.GetPosition(Field);
        double cp = CellPx();
        return new Point((p.X - Ox()) / cp, (p.Y - Oy()) / cp);
    }

    private int PickOwnSquad(Point w)
    {
        if (snap == null) return -1;
        int best = -1; double bd = 1.2;
        foreach (var q in snap.Squads)
        {
            if (q.Team != 0 || q.Eliminated) continue;
            double d = Math.Sqrt((q.X - w.X) * (q.X - w.X) +
                                 (q.Y - w.Y) * (q.Y - w.Y));
            if (d < bd) { bd = d; best = q.Id; }
        }
        return best;
    }

    private int PickFog(Point w)
    {
        if (snap == null) return -1;
        int best = -1; double bd = 1.5;
        foreach (var f in snap.Fogs)
        {
            if (f.Revealed) continue;
            foreach (var c in f.Candidates)
            {
                double d = Math.Sqrt((c.X - w.X) * (c.X - w.X) +
                                     (c.Y - w.Y) * (c.Y - w.Y));
                if (d < bd) { bd = d; best = f.EntityId; }
            }
        }
        return best;
    }

    private void Field_LeftDown(object sender, MouseButtonEventArgs e)
    {
        var w = WorldAt(e);
        int own = PickOwnSquad(w);
        if (own >= 0) { selected = own; return; }
        // 點到機率雲 → 弱觀測（1 情報，收縮不塌縮）
        int fid = PickFog(w);
        if (fid >= 0)
            NativeBridge.PB_FogProbe(session.Handle, fid,
                                     (float)w.X, (float)w.Y, 0.5f);
    }

    private void Field_RightDown(object sender, MouseButtonEventArgs e)
    {
        if (selected < 0 || snap == null) return;
        var w = WorldAt(e);
        // 點到敵方小隊 → Engage；否則 AttackMove
        int enemy = -1; double bd = 1.2;
        foreach (var q in snap.Squads)
        {
            if (q.Team != 1 || q.Eliminated) continue;
            double d = Math.Sqrt((q.X - w.X) * (q.X - w.X) +
                                 (q.Y - w.Y) * (q.Y - w.Y));
            if (d < bd) { bd = d; enemy = q.Id; }
        }
        bool ok = enemy >= 0
            ? NativeBridge.PB_InterveneEngage(session.Handle,
                                            selected, enemy, 5.0f) != 0
            : NativeBridge.PB_InterveneMove(session.Handle, selected,
                                            OrderAttackMove, (float)w.X,
                                            (float)w.Y, 5.0f) != 0;
        if (!ok) HudSelected.Text = "下令失敗（CP 不足或目標無效）";
    }

    private void BtnPause_Click(object sender, RoutedEventArgs e)
    {
        paused = !paused;
        BtnPause.Content = paused ? "繼 續" : "暫 停";
    }

    private void BtnSpeed_Click(object sender, RoutedEventArgs e)
    {
        timeScale = timeScale switch { 0.5f => 1.0f, 1.0f => 2.0f, _ => 0.5f };
        BtnSpeed.Content = $"速度 ×{timeScale:0.##}";
    }

    private void MainWindow_KeyDown(object sender, KeyEventArgs e)
    {
        if (e.Key == Key.Space) BtnPause_Click(sender, e);
        else if (e.Key == Key.D1) { timeScale = 0.5f; BtnSpeed.Content = "速度 ×0.5"; }
        else if (e.Key == Key.D2) { timeScale = 1.0f; BtnSpeed.Content = "速度 ×1"; }
        else if (e.Key == Key.D3) { timeScale = 2.0f; BtnSpeed.Content = "速度 ×2"; }
    }
}
