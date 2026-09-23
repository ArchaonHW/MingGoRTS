// Scene — 宣傳片腳本資料：一段 = 一個畫面來源 + 一段旁白。
// Card 場景渲染單張靜幀循環；Battle 場景逐步驅動無頭戰鬥逐幀渲染。
namespace MingGoRTS.Client;

internal enum SceneKind { Card, Battle }

internal sealed class Scene
{
    public SceneKind Kind;
    public string Narration = "";
    public string Title = "";          // Card：主標
    public string[] Lines = [];        // Card：副標行 / Battle：底部字幕
    public double MinSeconds;          // Battle：至少演幾秒（旁白更長則以旁白為準）
    public string? Cue;                // Battle：開場觸發的腳本指令

    // 民國史詩宣傳片腳本——斷橋渡口一役
    public static Scene[] Script() =>
    [
        new Scene
        {
            Kind = SceneKind.Card,
            Title = "斷 橋",
            Lines =
            [
                "MingGoRTS · 民國史詩即時戰略",
                "民國十四年，渡口斷橋",
                "至聖者無戰 · 治平者無勝",
            ],
            Narration = "民國十四年，斷橋渡口。一座橋，兩支軍。" +
                        "這不是推演的沙盤，是一場可以被書寫的戰役。",
        },
        new Scene
        {
            Kind = SceneKind.Card,
            Title = "情報是打出來的",
            Lines =
            [
                "敵軍不是模型，是一團機率",
                "探測收斂雲團 · 觀測確定真身",
                "糾纏、假訊、臨界揭露——霧即是戰場",
            ],
            Narration = "在斷橋，敵軍不是明擺著的棋子。" +
                        "每一支守軍開局只是一團機率雲——" +
                        "探測收斂它，觀測確定它，或者，被它騙過去。",
        },
        new Scene
        {
            Kind = SceneKind.Battle,
            Lines = ["機率雲散佈於彼岸——探測，或者直搗"],
            Narration = "戰幕拉開。紫色的霧是敵軍可能的所在，" +
                        "金線是我軍正在執行的命令。",
            MinSeconds = 12,
            Cue = "advance",
        },
        new Scene
        {
            Kind = SceneKind.Battle,
            Lines = ["接戰 · 士氣崩線 · 恐慌沿隊伍擴散"],
            Narration = "橋頭接戰。士氣是第二條生命線——" +
                        "一隊潰逃，恐慌會沿著隊伍擴散。",
            MinSeconds = 14,
            Cue = "probe",
        },
        new Scene
        {
            Kind = SceneKind.Card,
            Title = "每一筆都入帳",
            Lines =
            [
                "複式帳本記民心與武功",
                "史官戰報 · 章回體收場",
                "勝敗之外，還有第三條路：罷兵",
            ],
            Narration = "打下來的每一仗都入帳——民心、武功、" +
                        "陣亡名錄，複式帳本筆筆可查。" +
                        "而最高明的勝利，是帳上記著：不戰而定。",
        },
        new Scene
        {
            Kind = SceneKind.Card,
            Title = "斷 橋",
            Lines =
            [
                "MingGoRTS — 至聖者無戰",
                "C++20 · 無頭引擎 · 確定性回放",
                "史詩，由帳本與戰報寫成",
            ],
            Narration = "斷橋。一座橋的攻防，一部帳本的史詩。",
        },
    ];
}
