# N-5 神話事件具名記錄 + 滲透掛鉤

- status: done
- size: S
- source: narrative-design §System Requirements + UX myth seepage
- test: MythLogTest 14/14 PASS

## 需求

具名神話事件日誌（哪座祠、哪尊靈、何時）——結局時神的證詞
引用之；同時作為 UX 神話滲透主題切換的事件來源（玩家可映射）。

## 驗收

- [x] 事件日誌含祠/靈/時三欄並持久化
- [x] 滲透觸發介面已定義（即使 E-6 未做也可 stub）

## 實作

`Gameplay/MythLog.h/.cpp`

```cpp
struct MythEvent {
    std::string shrine;   // 祠（地名/廟名）
    std::string spirit;   // 靈（神祇/精怪名）
    std::string when;     // 時（章節或戰役時間標記）
    std::string detail;   // 記事
};

class MythLog {
    void Record(shrine, spirit, when, detail);      // 入日誌 + 滲透觸發
    void SetSeepageCallback(std::function<void(const MythEvent&)>);
    std::vector<std::string> TestimonyLines() const; // 結局神的證詞
    std::string ToJson() const;                      // potato.myth_log/1
    bool FromJson(const std::string&);
};
```

設計決策：

- 在規格三欄（祠/靈/時）外增加 `detail` 記事欄——結局證詞與
  滲透提示都需要事件內容，不只定位。
- 滲透掛鉤為輕量回呼介面：每次 `Record` 同步觸發，UX 端
  （E-6 神話層未完成前）可註冊 stub 消費——主題切換、提示
  浮現等映射完全由消費方決定，日誌層不耦合 UI。
- `TestimonyLines` 每筆事件產一行「時·祠之靈——記事」格式
  證詞，供結局四聲部（N-6）引用。
- 序列化走 `Serialization/JsonParser.h`，schema
  `potato.myth_log/1`；壞 JSON / 錯 schema 回傳 false 且不改動
  既有日誌。

## 測試覆蓋（14 checks）

- 三欄記錄與欄位保存
- 滲透回呼觸發次數與事件內容傳遞
- 證詞行數、祠靈引用、記事引用
- schema 標記、JSON 回讀一致、壞 JSON 拒絕、錯 schema 拒絕
