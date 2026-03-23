using System;
using System.IO;
using System.Linq;
using System.Text;

class FixEncoding
{
    static void Main(string[] args)
    {
        string[] files = {
            @"c:\HW\MingGoRTS\Plugins\MingPersonal\Source\MingPersonal\Public\AI\MingRTSAIDynamicDifficulty.h",
            @"c:\HW\MingGoRTS\Plugins\MingPersonal\Source\MingPersonal\Public\AI\MingRTSPredictiveAI.h",
            @"c:\HW\MingGoRTS\Plugins\MingPersonal\Source\MingPersonal\Public\Cloud\MingCloudServices.h",
            @"c:\HW\MingGoRTS\Plugins\MingUI\Source\MingUI\Public\Save\MingSaveLoadWidget.h",
            @"c:\HW\MingGoRTS\Plugins\MingUI\Source\MingUI\Public\MingMainHUD.h",
            @"c:\HW\MingGoRTS\Plugins\MingUI\Source\MingUI\Public\MingUISystem.h",
            @"c:\HW\MingGoRTS\Plugins\MingUI\Source\MingUI\Tests\MingUISystemTest.cpp"
        };

        foreach (var file in files)
        {
            if (File.Exists(file))
            {
                try
                {
                    string content = File.ReadAllText(file, Encoding.UTF8);
                    
                    // Check if has t-pattern
                    if (content.Count(c => c == 't') < 50)
                    {
                        Console.WriteLine($"SKIP: {Path.GetFileName(file)} - no t-pattern");
                        continue;
                    }
                    
                    // Fix: split by 't' and join non-empty parts
                    var parts = content.Split('t');
                    var fixedParts = parts.Where(p => !string.IsNullOrEmpty(p)).ToArray();
                    string fixedContent = string.Join("", fixedParts);
                    
                    File.WriteAllText(file, fixedContent, Encoding.UTF8);
                    Console.WriteLine($"FIXED: {Path.GetFileName(file)}");
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"ERROR: {Path.GetFileName(file)} - {ex.Message}");
                }
            }
            else
            {
                Console.WriteLine($"MISSING: {Path.GetFileName(file)}");
            }
        }
        
        Console.WriteLine("\nDone!");
    }
}
