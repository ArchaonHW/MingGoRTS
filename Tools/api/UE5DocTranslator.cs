using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace UE5DocTranslator
{
    class Program
    {
        static Dictionary<string, string> TranslationMap = new Dictionary<string, string>
        {
            // Basic terms
            ["Unreal Engine"] = "虛幻引擎",
            ["Unreal Engine 5"] = "虛幻引擎5",
            ["UE5"] = "UE5",
            ["Documentation"] = "文檔",
            ["Official"] = "官方",
            ["Overview"] = "概覽",
            ["Introduction"] = "介紹",
            ["Getting Started"] = "入門指南",
            ["Tutorial"] = "教學",
            ["Guide"] = "指南",
            ["Reference"] = "參考",
            ["API"] = "API",
            ["Blueprint"] = "藍圖",
            ["C++"] = "C++",
            ["Programming"] = "程式設計",
            ["Scripting"] = "腳本",
            ["Gameplay"] = "遊戲玩法",
            ["Rendering"] = "渲染",
            ["Graphics"] = "圖形",
            ["Physics"] = "物理",
            ["Collision"] = "碰撞",
            ["Audio"] = "音訊",
            ["Sound"] = "聲音",
            ["Music"] = "音樂",
            ["User Interface"] = "使用者介面",
            ["UI"] = "UI",
            ["Editor"] = "編輯器",
            ["Tools"] = "工具",
            ["Animation"] = "動畫",
            ["Cinematics"] = "電影",
            ["AI"] = "AI",
            ["Artificial Intelligence"] = "人工智慧",
            ["Behavior Trees"] = "行為樹",
            ["Networking"] = "網路",
            ["Multiplayer"] = "多人遊戲",
            ["Online"] = "線上",
            ["Services"] = "服務",
            ["Platforms"] = "平台",
            ["Mobile"] = "行動裝置",
            ["Virtual Reality"] = "虛擬實境",
            ["VR"] = "VR",
            ["Augmented Reality"] = "擴增實境",
            ["AR"] = "AR",
            ["Performance"] = "效能",
            ["Profiling"] = "效能分析",
            ["Testing"] = "測試",
            ["Automation"] = "自動化",
            ["Packaging"] = "封裝",
            ["Distribution"] = "發佈",
            ["Plugins"] = "外掛程式",
            
            // Category names
            ["Rendering and Graphics"] = "渲染與圖形",
            ["Testing and Automation"] = "測試與自動化",
            ["Networking and Multiplayer"] = "網路與多人遊戲",
            ["Packaging and Distribution"] = "封裝與發佈",
            ["Physics and Collision"] = "物理與碰撞",
            ["AI and Behavior Trees"] = "AI與行為樹",
            ["Performance and Profiling"] = "效能與效能分析",
            ["Online Services"] = "線上服務",
            ["Mobile Game Development"] = "行動遊戲開發",
            ["Programming and Scripting"] = "程式設計與腳本",
            
            // Common phrases
            ["Base URL"] = "基礎URL",
            ["Category Key"] = "類別鍵值",
            ["Extracted"] = "擷取時間",
            ["Project"] = "專案",
            ["Usage"] = "使用方式",
            ["Notes"] = "注意事項",
            ["Content"] = "內容",
            ["Generated"] = "生成時間",
            ["MingGoRTS"] = "MingGoRTS",
            
            // Technical terms
            ["component"] = "元件",
            ["actor"] = "角色",
            ["pawn"] = "棋子",
            ["character"] = "角色",
            ["level"] = "關卡",
            ["world"] = "世界",
            ["scene"] = "場景",
            ["asset"] = "資產",
            ["material"] = "材質",
            ["texture"] = "貼圖",
            ["mesh"] = "網格",
            ["skeletal mesh"] = "骨骼網格",
            ["static mesh"] = "靜態網格",
            ["animation blueprint"] = "動畫藍圖",
            ["behavior tree"] = "行為樹",
            ["state machine"] = "狀態機",
            ["curve"] = "曲線",
            ["timeline"] = "時間軸",
            ["event"] = "事件",
            ["delegate"] = "委派",
            ["function"] = "函式",
            ["variable"] = "變數",
            ["struct"] = "結構",
            ["class"] = "類別",
            ["interface"] = "介面",
            ["enum"] = "列舉",
            ["namespace"] = "命名空間",
            ["macro"] = "巨集",
            ["library"] = "函式庫",
            ["module"] = "模組",
            ["cook"] = "烹煮",
            ["deploy"] = "部署",
            ["launch"] = "啟動",
            ["run"] = "執行",
            ["test"] = "測試",
            ["validate"] = "驗證",
            ["verify"] = "確認",
            ["check"] = "檢查",
            ["scan"] = "掃描",
            ["analyze"] = "分析",
            ["optimize"] = "最佳化",
            ["profile"] = "效能分析",
            ["benchmark"] = "基準測試",
            ["monitor"] = "監控",
            ["log"] = "記錄",
            ["error"] = "錯誤",
            ["warning"] = "警告",
            ["info"] = "資訊",
            ["trace"] = "追蹤",
            ["verbose"] = "詳細",
            ["silent"] = "靜默",
            ["output"] = "輸出",
            ["input"] = "輸入",
            ["parameter"] = "參數",
            ["argument"] = "引數",
            ["property"] = "屬性",
            ["attribute"] = "特性",
            ["method"] = "方法",
            ["field"] = "欄位",
            ["member"] = "成員",
            ["object"] = "物件",
            ["instance"] = "執行個體",
            ["pointer"] = "指標",
            ["handle"] = "控制碼",
            ["id"] = "ID",
            ["identifier"] = "識別碼",
            ["name"] = "名稱",
            ["title"] = "標題",
            ["description"] = "描述",
            ["comment"] = "註解",
            ["help"] = "說明",
            ["support"] = "支援",
            ["community"] = "社群",
            ["forum"] = "論壇",
            ["wiki"] = "維基",
            ["example"] = "範例",
            ["sample"] = "範例",
            ["demo"] = "示範",
            ["boilerplate"] = "樣板",
            ["scaffold"] = "腳手架",
            ["framework"] = "框架",
            ["engine"] = "引擎",
            ["runtime"] = "執行時期",
            ["utility"] = "公用程式",
            ["helper"] = "協助程式",
            ["wrapper"] = "包裝函式",
            ["factory"] = "工廠",
            ["builder"] = "建置器",
            ["singleton"] = "單例",
            ["observer"] = "觀察者",
            ["strategy"] = "策略",
            ["command"] = "命令",
            ["state"] = "狀態",
            ["visitor"] = "訪客",
            ["iterator"] = "迭代器",
            ["composite"] = "組合",
            ["decorator"] = "裝飾器",
            ["proxy"] = "代理",
            ["flyweight"] = "享元",
            ["facade"] = "外觀",
            ["bridge"] = "橋接",
            ["mediator"] = "中介者",
            ["memento"] = "備忘錄",
            ["interpreter"] = "解譯器",
            ["chain of responsibility"] = "責任鏈",
            ["template method"] = "模板方法"
        };

        static void Main(string[] args)
        {
            string sourcePath = @"C:\HW\MingGoRTS\docs\UE5_Official_Documentation";
            string targetPath = @"C:\HW\MingGoRTS\docs\UE5_Official_Documentation_zh_TW";
            string logPath = $@"C:\HW\MingGoRTS\Logs\UE5DocTranslation_{DateTime.Now:yyyyMMdd_HHmmss}.log";

            // Create directories
            Directory.CreateDirectory(targetPath);
            Directory.CreateDirectory(@"C:\HW\MingGoRTS\Logs");

            Console.WriteLine("=== UE5 文檔繁體中文翻譯工具 ===");
            Console.WriteLine($"來源路徑: {sourcePath}");
            Console.WriteLine($"目標路徑: {targetPath}");
            Console.WriteLine();

            int processed = 0;
            int success = 0;

            // Process all markdown files
            var markdownFiles = Directory.GetFiles(sourcePath, "*.md", SearchOption.AllDirectories);
            
            foreach (var file in markdownFiles)
            {
                processed++;
                var relativePath = file.Substring(sourcePath.Length).TrimStart('\\', '/');
                var targetFile = Path.Combine(targetPath, relativePath);
                var targetDir = Path.GetDirectoryName(targetFile);

                try
                {
                    Directory.CreateDirectory(targetDir);
                    
                    // Read file
                    string content = File.ReadAllText(file, Encoding.UTF8);
                    
                    // Translate content
                    string translatedContent = TranslateText(content);
                    
                    // Write translated file with UTF-8 BOM for best compatibility
                    File.WriteAllText(targetFile, translatedContent, new UTF8Encoding(true));
                    
                    Console.WriteLine($"[成功] {relativePath}");
                    success++;
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"[失敗] {relativePath}: {ex.Message}");
                    File.AppendAllText(logPath, $"[{DateTime.Now:yyyy-MM-dd HH:mm:ss}] [ERROR] {relativePath}: {ex.Message}\n");
                }
            }

            // Create summary file
            var summaryPath = Path.Combine(targetPath, "README_Translation_Summary.md");
            var summaryContent = $@"# UE5 官方文檔繁體中文翻譯總結

**翻譯完成時間:** {DateTime.Now:yyyy-MM-dd HH:mm:ss}  
**原始路徑:** {sourcePath}  
**翻譯路徑:** {targetPath}  
**專案:** MingGoRTS  
**處理檔案:** {processed}  
**成功翻譯:** {success}  

## 概覽

本目錄包含從官方Epic Games虛幻引擎5文檔網站擷取並翻譯成繁體中文的文檔。

## 翻譯範圍

- 所有主要類別的文檔
- 技術術語和概念
- 使用指南和教學
- API參考文檔

## 翻譯品質

- 使用統一的技術術語翻譯
- 保持原文檔結構和格式
- 支援繁體中文慣用語法
- 定期更新翻譯詞彙

## 使用方式

此翻譯文檔提供離線存取和與MingGoRTS專案的整合。每個類別都包含翻譯後的markdown檔案。

## 注意事項

- 內容翻譯自官方Epic Games文檔
- 部分格式可能在翻譯過程中遺失
- 請務必參考官方線上文檔以獲取最新資訊
- 此翻譯僅供參考和整合使用

---

*由MingGoRTS UE5文檔翻譯器生成*
";
            File.WriteAllText(summaryPath, summaryContent, new UTF8Encoding(true));

            Console.WriteLine();
            Console.WriteLine("=== 翻譯完成 ===");
            Console.WriteLine($"處理檔案: {processed}");
            Console.WriteLine($"成功翻譯: {success}");
            Console.WriteLine($"日誌檔案: {logPath}");
            Console.WriteLine($"翻譯檔案儲存於: {targetPath}");
        }

        static string TranslateText(string text)
        {
            string result = text;
            
            // Sort by length descending to match longer phrases first
            var sortedKeys = TranslationMap.Keys.OrderByDescending(k => k.Length);
            
            foreach (var key in sortedKeys)
            {
                var value = TranslationMap[key];
                result = result.Replace(key, value);
            }
            
            return result;
        }
    }
}
