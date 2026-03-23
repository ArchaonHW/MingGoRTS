// MingGoRTS Requirement Ticket System - C++ Launcher
// C++ 需求單系統啟動器
// 提供命令行接口調用 PowerShell 腳本

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <stdexcept>

class RequirementTicketSystem {
public:
    RequirementTicketSystem(const std::wstring& scriptPath) 
        : m_scriptPath(scriptPath) {}

    // 創建需求單
    bool CreateTicket(const std::wstring& title, const std::wstring& description, 
                      const std::wstring& category = L"", const std::wstring& priority = L"",
                      const std::wstring& assignee = L"", const std::wstring& dueDate = L"") {
        std::vector<std::wstring> args = {
            L"-Action", L"create",
            L"-Title", title,
            L"-Description", description
        };
        
        if (!category.empty()) {
            args.push_back(L"-Category");
            args.push_back(category);
        }
        if (!priority.empty()) {
            args.push_back(L"-Priority");
            args.push_back(priority);
        }
        if (!assignee.empty()) {
            args.push_back(L"-Assignee");
            args.push_back(assignee);
        }
        if (!dueDate.empty()) {
            args.push_back(L"-DueDate");
            args.push_back(dueDate);
        }
        
        return ExecutePowerShell(args);
    }

    // 列出需求單
    bool ListTickets(const std::wstring& category = L"", const std::wstring& priority = L"",
                     const std::wstring& status = L"", const std::wstring& assignee = L"") {
        std::vector<std::wstring> args = { L"-Action", L"list" };
        
        if (!category.empty()) {
            args.push_back(L"-Category");
            args.push_back(category);
        }
        if (!priority.empty()) {
            args.push_back(L"-Priority");
            args.push_back(priority);
        }
        if (!status.empty()) {
            args.push_back(L"-Status");
            args.push_back(status);
        }
        if (!assignee.empty()) {
            args.push_back(L"-Assignee");
            args.push_back(assignee);
        }
        
        return ExecutePowerShell(args);
    }

    // 顯示需求單詳情
    bool ShowTicket(int ticketId) {
        std::vector<std::wstring> args = {
            L"-Action", L"show",
            L"-TicketID", std::to_wstring(ticketId)
        };
        return ExecutePowerShell(args);
    }

    // 更新需求單
    bool UpdateTicket(int ticketId, const std::wstring& status = L"", 
                      const std::wstring& assignee = L"", const std::wstring& priority = L"",
                      const std::wstring& dueDate = L"") {
        std::vector<std::wstring> args = {
            L"-Action", L"update",
            L"-TicketID", std::to_wstring(ticketId)
        };
        
        if (!status.empty()) {
            args.push_back(L"-Status");
            args.push_back(status);
        }
        if (!assignee.empty()) {
            args.push_back(L"-Assignee");
            args.push_back(assignee);
        }
        if (!priority.empty()) {
            args.push_back(L"-Priority");
            args.push_back(priority);
        }
        if (!dueDate.empty()) {
            args.push_back(L"-DueDate");
            args.push_back(dueDate);
        }
        
        return ExecutePowerShell(args);
    }

    // 添加評論
    bool AddComment(int ticketId, const std::wstring& comment) {
        std::vector<std::wstring> args = {
            L"-Action", L"comment",
            L"-TicketID", std::to_wstring(ticketId),
            L"-Description", comment
        };
        return ExecutePowerShell(args);
    }

    // 刪除需求單
    bool DeleteTicket(int ticketId) {
        std::vector<std::wstring> args = {
            L"-Action", L"delete",
            L"-TicketID", std::to_wstring(ticketId)
        };
        return ExecutePowerShell(args);
    }

    // 顯示分類統計
    bool ShowReport() {
        std::vector<std::wstring> args = { L"-Action", L"report" };
        return ExecutePowerShell(args);
    }

    // 導出 HTML 報表
    bool ExportReport() {
        std::vector<std::wstring> args = { L"-Action", L"export" };
        return ExecutePowerShell(args);
    }

    // 顯示分類列表
    bool ShowCategories() {
        std::vector<std::wstring> args = { L"-Action", L"categories" };
        return ExecutePowerShell(args);
    }

private:
    std::wstring m_scriptPath;

    bool ExecutePowerShell(const std::vector<std::wstring>& args) {
        // 構建 PowerShell 命令
        std::wstring command = L"powershell.exe -ExecutionPolicy Bypass -File \"" + m_scriptPath + L"\"";
        
        for (const auto& arg : args) {
            command += L" " + arg;
        }
        
        // 設置執行信息
        STARTUPINFOW si = { sizeof(si) };
        PROCESS_INFORMATION pi = { 0 };
        
        // 創建可寫入的命令緩衝區
        std::vector<wchar_t> cmdBuffer(command.begin(), command.end());
        cmdBuffer.push_back(L'\0');
        
        // 執行 PowerShell
        BOOL success = CreateProcessW(
            nullptr,
            cmdBuffer.data(),
            nullptr,
            nullptr,
            FALSE,
            CREATE_NEW_CONSOLE,
            nullptr,
            nullptr,
            &si,
            &pi
        );
        
        if (!success) {
            std::wcerr << L"無法啟動 PowerShell 進程. 錯誤碼: " << GetLastError() << std::endl;
            return false;
        }
        
        // 等待進程完成
        WaitForSingleObject(pi.hProcess, INFINITE);
        
        // 獲取退出碼
        DWORD exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);
        
        // 清理
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        
        return exitCode == 0;
    }
};

// 輔助函數：字符串轉換
std::wstring StringToWString(const std::string& str) {
    if (str.empty()) return std::wstring();
    int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    std::wstring result(size - 1, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &result[0], size);
    return result;
}

// 打印使用說明
void PrintUsage(const char* programName) {
    std::cout << "MingGoRTS Requirement Ticket System - C++ Launcher\n";
    std::cout << "==================================================\n\n";
    std::cout << "用法: " << programName << " <命令> [參數...]\n\n";
    std::cout << "命令:\n";
    std::cout << "  create <標題> <描述> [分類] [優先級] [指派] [截止日期]\n";
    std::cout << "    - 創建新需求單\n";
    std::cout << "  list [分類] [優先級] [狀態] [指派]\n";
    std::cout << "    - 列出需求單\n";
    std::cout << "  show <票單ID>\n";
    std::cout << "    - 顯示需求單詳情\n";
    std::cout << "  update <票單ID> [狀態] [指派] [優先級] [截止日期]\n";
    std::cout << "    - 更新需求單\n";
    std::cout << "  comment <票單ID> <評論內容>\n";
    std::cout << "    - 添加評論\n";
    std::cout << "  delete <票單ID>\n";
    std::cout << "    - 刪除需求單\n";
    std::cout << "  report\n";
    std::cout << "    - 顯示統計報表\n";
    std::cout << "  export\n";
    std::cout << "    - 導出 HTML 報表\n";
    std::cout << "  categories\n";
    std::cout << "    - 顯示分類列表\n\n";
    std::cout << "示例:\n";
    std::cout << "  " << programName << " create \"修復記憶體洩漏\" \"戰鬥系統崩潰問題\"\n";
    std::cout << "  " << programName << " list\n";
    std::cout << "  " << programName << " show 1\n";
    std::cout << "  " << programName << " update 1 InProgress Developer1\n";
}

int main(int argc, char* argv[]) {
    // 檢查參數
    if (argc < 2) {
        PrintUsage(argv[0]);
        return 1;
    }

    // 設置 PowerShell 腳本路徑
    std::wstring scriptPath = L"C:\\HW\\MingGoRTS\\Tools\\requirement\\RequirementTicketSystem.ps1";
    
    RequirementTicketSystem system(scriptPath);
    std::string command = argv[1];
    bool success = false;

    try {
        if (command == "create") {
            if (argc < 4) {
                std::cerr << "錯誤: create 命令需要標題和描述參數\n";
                return 1;
            }
            std::wstring title = StringToWString(argv[2]);
            std::wstring description = StringToWString(argv[3]);
            std::wstring category = (argc > 4) ? StringToWString(argv[4]) : L"";
            std::wstring priority = (argc > 5) ? StringToWString(argv[5]) : L"";
            std::wstring assignee = (argc > 6) ? StringToWString(argv[6]) : L"";
            std::wstring dueDate = (argc > 7) ? StringToWString(argv[7]) : L"";
            
            success = system.CreateTicket(title, description, category, priority, assignee, dueDate);
        }
        else if (command == "list") {
            std::wstring category = (argc > 2) ? StringToWString(argv[2]) : L"";
            std::wstring priority = (argc > 3) ? StringToWString(argv[3]) : L"";
            std::wstring status = (argc > 4) ? StringToWString(argv[4]) : L"";
            std::wstring assignee = (argc > 5) ? StringToWString(argv[5]) : L"";
            
            success = system.ListTickets(category, priority, status, assignee);
        }
        else if (command == "show") {
            if (argc < 3) {
                std::cerr << "錯誤: show 命令需要票單ID參數\n";
                return 1;
            }
            int ticketId = std::stoi(argv[2]);
            success = system.ShowTicket(ticketId);
        }
        else if (command == "update") {
            if (argc < 3) {
                std::cerr << "錯誤: update 命令需要票單ID參數\n";
                return 1;
            }
            int ticketId = std::stoi(argv[2]);
            std::wstring status = (argc > 3) ? StringToWString(argv[3]) : L"";
            std::wstring assignee = (argc > 4) ? StringToWString(argv[4]) : L"";
            std::wstring priority = (argc > 5) ? StringToWString(argv[5]) : L"";
            std::wstring dueDate = (argc > 6) ? StringToWString(argv[6]) : L"";
            
            success = system.UpdateTicket(ticketId, status, assignee, priority, dueDate);
        }
        else if (command == "comment") {
            if (argc < 4) {
                std::cerr << "錯誤: comment 命令需要票單ID和評論內容參數\n";
                return 1;
            }
            int ticketId = std::stoi(argv[2]);
            std::wstring comment = StringToWString(argv[3]);
            success = system.AddComment(ticketId, comment);
        }
        else if (command == "delete") {
            if (argc < 3) {
                std::cerr << "錯誤: delete 命令需要票單ID參數\n";
                return 1;
            }
            int ticketId = std::stoi(argv[2]);
            success = system.DeleteTicket(ticketId);
        }
        else if (command == "report") {
            success = system.ShowReport();
        }
        else if (command == "export") {
            success = system.ExportReport();
        }
        else if (command == "categories") {
            success = system.ShowCategories();
        }
        else {
            std::cerr << "錯誤: 未知命令 '" << command << "'\n";
            PrintUsage(argv[0]);
            return 1;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "錯誤: " << e.what() << std::endl;
        return 1;
    }

    return success ? 0 : 1;
}
