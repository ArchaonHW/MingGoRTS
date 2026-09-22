// App — 啟動分流：--selftest 走無頭驗證（CI 可跑），否則開主視窗。
using System.Windows;

namespace MingGoRTS.Client;

public partial class App : Application
{
    protected override void OnStartup(StartupEventArgs e)
    {
        base.OnStartup(e);
        if (e.Args.Contains("--selftest"))
        {
            int code = SelfTest.Run();
            Shutdown(code);
            return;
        }
        new MainWindow().Show();
    }
}
