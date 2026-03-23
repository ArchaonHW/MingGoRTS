#include "MingSageBrainInteractiveConsole.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Async/Async.h"

UMingSageBrainInteractiveConsole::UMingSageBrainInteractiveConsole()
    : bIsInitialized(false)
    , bIsRunning(false)
    , bInWizardMode(false)
    , bInDialogueMode(false)
    , CurrentMode(EUserInteractionMode::CommandLine)
    , SessionState(EConsoleSessionState::Idle)
    , SessionCounter(0)
    , CurrentWizardStep(0)
{
}

bool UMingSageBrainInteractiveConsole::InitializeInteractiveConsole()
{
    if (bIsInitialized)
    {
        return true;
    }

    // 初始化 API Console
    APIConsole = NewObject<UMingSageBrainAPIConsole>(this);
    if (!APIConsole || !APIConsole->InitializeConsole())
    {
        UE_LOG(LogTemp, Error, TEXT("无法初始化 API Console"));
        return false;
    }

    bIsInitialized = true;
    SessionState = EConsoleSessionState::Idle;
    
    UE_LOG(LogTemp, Log, TEXT("圣者大脑交互式控制台已初始化"));
    return true;
}

void UMingSageBrainInteractiveConsole::ShutdownInteractiveConsole()
{
    if (!bIsInitialized)
    {
        return;
    }

    StopMainLoop();
    EndCurrentSession();

    if (APIConsole)
    {
        APIConsole->ShutdownConsole();
        APIConsole = nullptr;
    }

    bIsInitialized = false;
    UE_LOG(LogTemp, Log, TEXT("交互式控制台已关闭"));
}

void UMingSageBrainInteractiveConsole::SetInteractionMode(EUserInteractionMode Mode)
{
    if (CurrentMode == Mode)
    {
        return;
    }

    CurrentMode = Mode;
    OnInteractiveModeChanged.Broadcast(Mode);
    
    // 根据模式发送欢迎消息
    switch (Mode)
    {
    case EUserInteractionMode::Dialogue:
        bInDialogueMode = true;
        break;
    case EUserInteractionMode::Wizard:
        bInWizardMode = true;
        break;
    case EUserInteractionMode::Chat:
        StartChatSession();
        break;
    default:
        bInDialogueMode = false;
        bInWizardMode = false;
        break;
    }

    UE_LOG(LogTemp, Log, TEXT("交互模式已切换到: %s"), *GetAvailableModes()[static_cast<int32>(Mode)]);
}

TArray<FString> UMingSageBrainInteractiveConsole::GetAvailableModes() const
{
    return {
        TEXT("命令行模式"),
        TEXT("对话模式"),
        TEXT("向导模式"),
        TEXT("聊天模式"),
        TEXT("交互模式")
    };
}

void UMingSageBrainInteractiveConsole::ProcessUserInput(const FString& Input)
{
    if (!bIsInitialized || Input.IsEmpty())
    {
        return;
    }

    // 创建输入上下文
    FUserInputContext Context;
    Context.InputText = Input;
    Context.Mode = CurrentMode;
    Context.InputTime = FDateTime::Now();
    Context.SessionID = CurrentSession.SessionID;

    InputHistory.Add(Context);
    OnUserInputReceived.Broadcast(Context);

    UpdateSessionState(EConsoleSessionState::Processing);

    // 根据当前模式处理输入
    FSystemResponse Response;
    
    switch (CurrentMode)
    {
    case EUserInteractionMode::CommandLine:
        Response = ProcessCommandLineInput(Context);
        break;
    case EUserInteractionMode::Dialogue:
        Response = ProcessDialogueInput(Context);
        break;
    case EUserInteractionMode::Wizard:
        Response = ProcessWizardInput(Context);
        break;
    case EUserInteractionMode::Chat:
        Response = ProcessChatInput(Context);
        break;
    case EUserInteractionMode::Interactive:
        Response = ProcessInteractiveInput(Context);
        break;
    default:
        Response.ResponseText = TEXT("未知交互模式");
        Response.Level = EConsoleOutputLevel::Error;
        break;
    }

    SendResponse(Response);
    UpdateSessionState(EConsoleSessionState::Listening);
}

void UMingSageBrainInteractiveConsole::StartListening()
{
    UpdateSessionState(EConsoleSessionState::Listening);
    UE_LOG(LogTemp, Log, TEXT("开始监听用户输入..."));
}

void UMingSageBrainInteractiveConsole::StopListening()
{
    UpdateSessionState(EConsoleSessionState::Waiting);
    UE_LOG(LogTemp, Log, TEXT("停止监听用户输入"));
}

FSystemResponse UMingSageBrainInteractiveConsole::GenerateResponse(const FUserInputContext& Input)
{
    FSystemResponse Response;
    Response.Level = EConsoleOutputLevel::Info;
    
    // 分析用户意图
    FString Intent = AnalyzeUserIntent(Input.InputText);
    TArray<FString> Entities = ExtractEntities(Input.InputText);
    
    // 根据意图生成响应
    if (Intent == TEXT("help"))
    {
        Response.ResponseText = TEXT("我可以帮您:\n")
            TEXT("1. 生成音乐、场景、角色\n")
            TEXT("2. 查询系统状态\n")
            TEXT("3. 运行测试\n")
            TEXT("4. 配置系统参数\n")
            TEXT("输入 'mode' 切换交互模式");
        Response.SuggestedActions = { TEXT("生成音乐"), TEXT("查询状态"), TEXT("运行测试") };
    }
    else if (Intent == TEXT("status"))
    {
        if (APIConsole)
        {
            Response.ResponseText = APIConsole->GetConsoleStatus();
        }
        else
        {
            Response.ResponseText = TEXT("系统未初始化");
            Response.Level = EConsoleOutputLevel::Error;
        }
    }
    else if (Intent == TEXT("generate"))
    {
        Response.ResponseText = FString::Printf(TEXT("检测到生成请求: %s\n"), *Input.InputText);
        Response.ResponseText += TEXT("您可以使用:\n")
            TEXT("- generate music \"描述\"\n")
            TEXT("- generate scene \"描述\"\n")
            TEXT("- generate character \"描述\"");
        Response.bRequiresUserInput = true;
        Response.PromptForInput = TEXT("请输入具体的生成描述:");
    }
    else if (Intent == TEXT("mode"))
    {
        Response.ResponseText = TEXT("可用交互模式:\n")
            TEXT("1. command - 命令行模式\n")
            TEXT("2. dialogue - 对话模式\n")
            TEXT("3. wizard - 向导模式\n")
            TEXT("4. chat - 聊天模式\n")
            TEXT("输入 'mode <名称>' 切换模式");
    }
    else
    {
        Response.ResponseText = GenerateSmartResponse(Intent, Entities);
    }

    return Response;
}

void UMingSageBrainInteractiveConsole::SendResponse(const FSystemResponse& Response)
{
    ResponseHistory.Add(Response);
    OnSystemResponseReady.Broadcast(Response);
    
    // 记录响应
    UE_LOG(LogTemp, Log, TEXT("[系统] %s"), *Response.ResponseText);
    
    // 如果需要用户输入，等待输入
    if (Response.bRequiresUserInput && !Response.PromptForInput.IsEmpty())
    {
        UpdateSessionState(EConsoleSessionState::Waiting);
    }
}

void UMingSageBrainInteractiveConsole::SendWelcomeMessage()
{
    FSystemResponse Welcome;
    Welcome.ResponseText = TEXT("🧠 欢迎使用圣者大脑交互式控制台!\n\n")
        TEXT("当前模式: ") + GetAvailableModes()[static_cast<int32>(CurrentMode)] + TEXT("\n")
        TEXT("输入 'help' 查看可用命令\n")
        TEXT("输入 'mode' 切换交互模式\n");
    Welcome.Level = EConsoleOutputLevel::Success;
    
    SendResponse(Welcome);
}

void UMingSageBrainInteractiveConsole::SendHelpMessage()
{
    FSystemResponse Help;
    Help.ResponseText = TEXT("=== 圣者大脑控制台帮助 ===\n\n")
        TEXT("基本命令:\n")
        TEXT("  help, ?       - 显示此帮助\n")
        TEXT("  status        - 查看系统状态\n")
        TEXT("  mode          - 查看/切换交互模式\n")
        TEXT("  clear         - 清除屏幕\n")
        TEXT("  exit, quit    - 退出控制台\n\n")
        TEXT("生成命令:\n")
        TEXT("  generate music \"描述\"    - 生成音乐\n")
        TEXT("  generate scene \"描述\"   - 生成场景\n")
        TEXT("  generate character \"描述\" - 生成角色\n\n")
        TEXT("测试命令:\n")
        TEXT("  test          - 运行所有测试\n")
        TEXT("  test system   - 运行系统测试\n")
        TEXT("  test skills   - 运行技能测试\n\n")
        TEXT("配置命令:\n")
        TEXT("  config        - 查看配置\n")
        TEXT("  config set <键> <值> - 设置配置\n");
    Help.Level = EConsoleOutputLevel::Info;
    Help.SuggestedActions = { TEXT("生成音乐"), TEXT("查询状态"), TEXT("切换模式") };
    
    SendResponse(Help);
}

void UMingSageBrainInteractiveConsole::StartWizard(const FString& WizardName)
{
    CurrentWizardName = WizardName;
    CurrentWizardStep = 0;
    WizardData.Empty();
    bInWizardMode = true;
    
    SetInteractionMode(EUserInteractionMode::Wizard);
    
    FSystemResponse Response;
    Response.ResponseText = FString::Printf(TEXT("🧙 启动向导: %s\n"), *WizardName);
    Response.Level = EConsoleOutputLevel::Info;
    
    SendResponse(Response);
    
    // 执行第一步
    FSystemResponse FirstStep = ExecuteWizardStep();
    SendResponse(FirstStep);
}

void UMingSageBrainInteractiveConsole::ProcessWizardStep(const FString& Input)
{
    if (!bInWizardMode)
    {
        return;
    }

    // 保存当前步骤的数据
    TArray<FString> Steps = GetWizardSteps(CurrentWizardName);
    if (CurrentWizardStep < Steps.Num())
    {
        FString Key = FString::Printf(TEXT("step_%d"), CurrentWizardStep);
        WizardData.Add(Key, Input);
    }

    CurrentWizardStep++;
    
    // 执行下一步或完成
    if (CurrentWizardStep >= Steps.Num())
    {
        CompleteWizard();
    }
    else
    {
        FSystemResponse NextStep = ExecuteWizardStep();
        SendResponse(NextStep);
    }
}

void UMingSageBrainInteractiveConsole::CancelWizard()
{
    bInWizardMode = false;
    CurrentWizardName.Empty();
    CurrentWizardStep = 0;
    WizardData.Empty();
    
    FSystemResponse Response;
    Response.ResponseText = TEXT("向导已取消");
    Response.Level = EConsoleOutputLevel::Warning;
    
    SendResponse(Response);
    SetInteractionMode(EUserInteractionMode::CommandLine);
}

void UMingSageBrainInteractiveConsole::StartDialogue(const FString& Topic)
{
    bInDialogueMode = true;
    SetInteractionMode(EUserInteractionMode::Dialogue);
    
    FSystemResponse Response;
    Response.ResponseText = FString::Printf(TEXT("💬 开始对话主题: %s\n"), *Topic);
    Response.ResponseText += TEXT("您可以自由输入，我会尽力帮助您。\n")
        TEXT("输入 'end' 结束对话");
    Response.Level = EConsoleOutputLevel::Info;
    
    SendResponse(Response);
}

void UMingSageBrainInteractiveConsole::ProcessDialogueInput(const FString& Input)
{
    if (Input.ToLower() == TEXT("end") || Input.ToLower() == TEXT("结束"))
    {
        EndDialogue();
        return;
    }

    // 智能对话处理
    FUserInputContext Context;
    Context.InputText = Input;
    Context.Mode = EUserInteractionMode::Dialogue;
    Context.InputTime = FDateTime::Now();
    
    FSystemResponse Response = ProcessDialogueInput(Context);
    SendResponse(Response);
}

void UMingSageBrainInteractiveConsole::EndDialogue()
{
    bInDialogueMode = false;
    
    FSystemResponse Response;
    Response.ResponseText = TEXT("对话已结束。感谢您的交流！");
    Response.Level = EConsoleOutputLevel::Info;
    
    SendResponse(Response);
    SetInteractionMode(EUserInteractionMode::CommandLine);
}

void UMingSageBrainInteractiveConsole::StartChatSession()
{
    StartNewSession();
    
    FSystemResponse Response;
    Response.ResponseText = TEXT("💭 聊天会话已开始\n")
        TEXT("当前会话ID: ") + FString::FromInt(CurrentSession.SessionID);
    Response.Level = EConsoleOutputLevel::Info;
    
    SendResponse(Response);
}

void UMingSageBrainInteractiveConsole::ProcessChatMessage(const FString& Message)
{
    FUserInputContext Context;
    Context.InputText = Message;
    Context.Mode = EUserInteractionMode::Chat;
    Context.InputTime = FDateTime::Now();
    Context.SessionID = CurrentSession.SessionID;
    
    CurrentSession.MessageCount++;
    CurrentSession.LastActivity = FDateTime::Now();
    
    FSystemResponse Response = ProcessChatInput(Context);
    SendResponse(Response);
}

void UMingSageBrainInteractiveConsole::EndChatSession()
{
    FSystemResponse Response;
    Response.ResponseText = FString::Printf(TEXT("聊天会话已结束\n")
        TEXT("会话统计: %d 条消息"), CurrentSession.MessageCount);
    Response.Level = EConsoleOutputLevel::Info;
    
    SendResponse(Response);
    EndCurrentSession();
}

void UMingSageBrainInteractiveConsole::StartNewSession()
{
    EndCurrentSession();
    
    CurrentSession.SessionID = GenerateSessionID();
    CurrentSession.StartTime = FDateTime::Now();
    CurrentSession.LastActivity = FDateTime::Now();
    CurrentSession.MessageCount = 0;
    CurrentSession.CurrentMode = CurrentMode;
    CurrentSession.SessionVariables.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("新会话已启动: %d"), CurrentSession.SessionID);
}

void UMingSageBrainInteractiveConsole::EndCurrentSession()
{
    if (CurrentSession.SessionID != 0)
    {
        UE_LOG(LogTemp, Log, TEXT("会话已结束: %d"), CurrentSession.SessionID);
        CurrentSession = FConsoleSession();
    }
}

void UMingSageBrainInteractiveConsole::SetSessionVariable(const FString& Key, const FString& Value)
{
    CurrentSession.SessionVariables.Add(Key, Value);
}

FString UMingSageBrainInteractiveConsole::GetSessionVariable(const FString& Key) const
{
    const FString* Value = CurrentSession.SessionVariables.Find(Key);
    return Value ? *Value : FString();
}

void UMingSageBrainInteractiveConsole::ClearSessionVariables()
{
    CurrentSession.SessionVariables.Empty();
}

TArray<FString> UMingSageBrainInteractiveConsole::GetSmartSuggestions(const FString& PartialInput)
{
    TArray<FString> Suggestions;
    
    TArray<FString> Commands = {
        TEXT("help"), TEXT("status"), TEXT("generate"), TEXT("mode"),
        TEXT("test"), TEXT("config"), TEXT("clear"), TEXT("exit")
    };
    
    FString LowerInput = PartialInput.ToLower();
    for (const FString& Cmd : Commands)
    {
        if (Cmd.StartsWith(LowerInput))
        {
            Suggestions.Add(Cmd);
        }
    }
    
    return Suggestions;
}

FString UMingSageBrainInteractiveConsole::GetNextRecommendedAction()
{
    if (ResponseHistory.Num() > 0)
    {
        const FSystemResponse& LastResponse = ResponseHistory.Last();
        if (LastResponse.SuggestedActions.Num() > 0)
        {
            return LastResponse.SuggestedActions[0];
        }
    }
    
    return TEXT("help");
}

bool UMingSageBrainInteractiveConsole::IsValidCommand(const FString& Input)
{
    TArray<FString> ValidCommands = {
        TEXT("help"), TEXT("status"), TEXT("generate"), TEXT("mode"),
        TEXT("test"), TEXT("config"), TEXT("clear"), TEXT("exit"),
        TEXT("quit"), TEXT("wizard"), TEXT("dialogue"), TEXT("chat")
    };
    
    FString LowerInput = Input.ToLower();
    for (const FString& Cmd : ValidCommands)
    {
        if (LowerInput.StartsWith(Cmd))
        {
            return true;
        }
    }
    
    return false;
}

void UMingSageBrainInteractiveConsole::RunMainLoop()
{
    if (bIsRunning)
    {
        return;
    }

    bIsRunning = true;
    SendWelcomeMessage();
    StartListening();
    
    UE_LOG(LogTemp, Log, TEXT("主循环已启动"));
}

void UMingSageBrainInteractiveConsole::StopMainLoop()
{
    bIsRunning = false;
    UpdateSessionState(EConsoleSessionState::Closed);
    
    UE_LOG(LogTemp, Log, TEXT("主循环已停止"));
}

FString UMingSageBrainInteractiveConsole::GetStatusString() const
{
    FString Status = TEXT("=== 交互式控制台状态 ===\n");
    Status += FString::Printf(TEXT("运行状态: %s\n"), bIsRunning ? TEXT("运行中") : TEXT("已停止"));
    Status += FString::Printf(TEXT("交互模式: %s\n"), *GetAvailableModes()[static_cast<int32>(CurrentMode)]);
    Status += FString::Printf(TEXT("会话状态: %d\n"), static_cast<int32>(SessionState));
    Status += FString::Printf(TEXT("当前会话: %d\n"), CurrentSession.SessionID);
    Status += FString::Printf(TEXT("消息数量: %d\n"), CurrentSession.MessageCount);
    Status += FString::Printf(TEXT("向导模式: %s\n"), bInWizardMode ? TEXT("是") : TEXT("否"));
    Status += FString::Printf(TEXT("对话模式: %s\n"), bInDialogueMode ? TEXT("是") : TEXT("否"));
    return Status;
}

FString UMingSageBrainInteractiveConsole::GetInteractivePrompt() const
{
    if (bInWizardMode)
    {
        return FString::Printf(TEXT("🧙 [%s:%d]> "), *CurrentWizardName, CurrentWizardStep + 1);
    }
    else if (bInDialogueMode)
    {
        return TEXT("💬 > ");
    }
    else if (CurrentMode == EUserInteractionMode::Chat)
    {
        return FString::Printf(TEXT("💭 [%d]> "), CurrentSession.MessageCount);
    }
    else
    {
        return TEXT("🧠 > ");
    }
}

// 处理函数实现
FSystemResponse UMingSageBrainInteractiveConsole::ProcessCommandLineInput(const FUserInputContext& Input)
{
    FString LowerInput = Input.InputText.ToLower();
    
    // 检查模式切换命令
    if (LowerInput.StartsWith(TEXT("mode ")))
    {
        FString ModeName = LowerInput.RightChop(5).TrimStartAndEnd();
        
        if (ModeName == TEXT("dialogue") || ModeName == TEXT("对话"))
        {
            StartDialogue(TEXT("general"));
            FSystemResponse Response;
            Response.ResponseText = TEXT("已切换到对话模式");
            Response.Level = EConsoleOutputLevel::Success;
            return Response;
        }
        else if (ModeName == TEXT("wizard") || ModeName == TEXT("向导"))
        {
            StartWizard(TEXT("general"));
            FSystemResponse Response;
            Response.ResponseText = TEXT("已切换到向导模式");
            Response.Level = EConsoleOutputLevel::Success;
            return Response;
        }
        else if (ModeName == TEXT("chat") || ModeName == TEXT("聊天"))
        {
            SetInteractionMode(EUserInteractionMode::Chat);
            FSystemResponse Response;
            Response.ResponseText = TEXT("已切换到聊天模式");
            Response.Level = EConsoleOutputLevel::Success;
            return Response;
        }
        else if (ModeName == TEXT("command") || ModeName == TEXT("命令"))
        {
            SetInteractionMode(EUserInteractionMode::CommandLine);
            FSystemResponse Response;
            Response.ResponseText = TEXT("已切换到命令行模式");
            Response.Level = EConsoleOutputLevel::Success;
            return Response;
        }
    }
    
    // 直接通过 API Console 执行
    if (APIConsole)
    {
        FConsoleCommandResult Result = APIConsole->ExecuteCommand(Input.InputText);
        
        FSystemResponse Response;
        Response.ResponseText = Result.Message;
        Response.Level = Result.bSuccess ? EConsoleOutputLevel::Success : EConsoleOutputLevel::Error;
        Response.ResponseData = Result.Data;
        
        return Response;
    }
    
    FSystemResponse Response;
    Response.ResponseText = TEXT("API Console 未初始化");
    Response.Level = EConsoleOutputLevel::Error;
    return Response;
}

FSystemResponse UMingSageBrainInteractiveConsole::ProcessDialogueInput(const FUserInputContext& Input)
{
    FString LowerInput = Input.InputText.ToLower();
    
    // 退出对话
    if (LowerInput == TEXT("end") || LowerInput == TEXT("exit") || LowerInput == TEXT("quit") ||
        LowerInput == TEXT("结束") || LowerInput == TEXT("退出"))
    {
        EndDialogue();
        
        FSystemResponse Response;
        Response.ResponseText = TEXT("已退出对话模式，返回命令行模式");
        Response.Level = EConsoleOutputLevel::Info;
        return Response;
    }
    
    // 生成智能对话响应
    return GenerateResponse(Input);
}

FSystemResponse UMingSageBrainInteractiveConsole::ProcessWizardInput(const FUserInputContext& Input)
{
    FString LowerInput = Input.InputText.ToLower();
    
    // 取消向导
    if (LowerInput == TEXT("cancel") || LowerInput == TEXT("取消"))
    {
        CancelWizard();
        
        FSystemResponse Response;
        Response.ResponseText = TEXT("向导已取消");
        Response.Level = EConsoleOutputLevel::Warning;
        return Response;
    }
    
    // 处理向导步骤
    ProcessWizardStep(Input.InputText);
    
    FSystemResponse Response;
    return Response;
}

FSystemResponse UMingSageBrainInteractiveConsole::ProcessChatInput(const FUserInputContext& Input)
{
    FString LowerInput = Input.InputText.ToLower();
    
    // 退出聊天
    if (LowerInput == TEXT("end") || LowerInput == TEXT("exit") || LowerInput == TEXT("quit") ||
        LowerInput == TEXT("结束") || LowerInput == TEXT("退出"))
    {
        EndChatSession();
        
        FSystemResponse Response;
        Response.ResponseText = TEXT("聊天会话已结束");
        Response.Level = EConsoleOutputLevel::Info;
        return Response;
    }
    
    // 生成聊天响应
    return GenerateResponse(Input);
}

FSystemResponse UMingSageBrainInteractiveConsole::ProcessInteractiveInput(const FUserInputContext& Input)
{
    // 交互模式结合所有功能
    return GenerateResponse(Input);
}

FSystemResponse UMingSageBrainInteractiveConsole::ExecuteWizardStep()
{
    TArray<FString> Steps = GetWizardSteps(CurrentWizardName);
    
    FSystemResponse Response;
    Response.Level = EConsoleOutputLevel::Info;
    Response.bRequiresUserInput = true;
    
    if (CurrentWizardStep < Steps.Num())
    {
        FString StepName = Steps[CurrentWizardStep];
        
        if (CurrentWizardName == TEXT("music"))
        {
            switch (CurrentWizardStep)
            {
            case 0:
                Response.ResponseText = TEXT("🎵 音乐生成向导\n\n")
                    TEXT("第1步: 描述您想要的音乐\n")
                    TEXT("例如: 史诗级战斗音乐，中国传统风格");
                break;
            case 1:
                Response.ResponseText = TEXT("第2步: 设置音乐时长 (秒)\n")
                    TEXT("建议: 30-180秒");
                break;
            case 2:
                Response.ResponseText = TEXT("第3步: 选择音乐风格\n")
                    TEXT("可选: orchestral, traditional, electronic, ambient");
                break;
            default:
                Response.ResponseText = FString::Printf(TEXT("步骤 %d: %s"), CurrentWizardStep + 1, *StepName);
                break;
            }
        }
        else
        {
            Response.ResponseText = FString::Printf(TEXT("步骤 %d/%d: %s"),
                CurrentWizardStep + 1, Steps.Num(), *StepName);
        }
    }
    else
    {
        Response.ResponseText = TEXT("向导已完成！");
        Response.bRequiresUserInput = false;
    }
    
    return Response;
}

TArray<FString> UMingSageBrainInteractiveConsole::GetWizardSteps(const FString& WizardName)
{
    if (WizardName == TEXT("music"))
    {
        return { TEXT("描述"), TEXT("时长"), TEXT("风格"), TEXT("确认") };
    }
    else if (WizardName == TEXT("scene"))
    {
        return { TEXT("场景类型"), TEXT("描述"), TEXT("复杂度"), TEXT("风格"), TEXT("确认") };
    }
    else if (WizardName == TEXT("character"))
    {
        return { TEXT("角色名称"), TEXT("描述"), TEXT("风格"), TEXT("确认") };
    }
    else
    {
        return { TEXT("输入1"), TEXT("输入2"), TEXT("确认") };
    }
}

void UMingSageBrainInteractiveConsole::CompleteWizard()
{
    bInWizardMode = false;
    
    // 处理向导完成后的操作
    FSystemResponse Response;
    Response.ResponseText = TEXT("✅ 向导完成！\n\n")
        TEXT("收集的数据:\n");
    
    for (const auto& Pair : WizardData)
    {
        Response.ResponseText += FString::Printf(TEXT("  %s: %s\n"), *Pair.Key, *Pair.Value);
    }
    
    Response.ResponseText += TEXT("\n正在执行操作...");
    Response.Level = EConsoleOutputLevel::Success;
    
    SendResponse(Response);
    
    // 实际执行生成操作
    if (CurrentWizardName == TEXT("music"))
    {
        FString Description = WizardData.FindRef(TEXT("step_0"));
        FString Duration = WizardData.FindRef(TEXT("step_1"));
        FString Style = WizardData.FindRef(TEXT("step_2"));
        
        if (APIConsole)
        {
            FString Result = APIConsole->ExecuteCommand(
                FString::Printf(TEXT("generate music \"%s\" --duration=%s --style=%s"),
                    *Description, *Duration, *Style)).Message;
            
            FSystemResponse ExecResponse;
            ExecResponse.ResponseText = Result;
            ExecResponse.Level = EConsoleOutputLevel::Success;
            SendResponse(ExecResponse);
        }
    }
    
    // 清理向导状态
    CurrentWizardName.Empty();
    CurrentWizardStep = 0;
    WizardData.Empty();
    
    SetInteractionMode(EUserInteractionMode::CommandLine);
}

FString UMingSageBrainInteractiveConsole::AnalyzeUserIntent(const FString& Input)
{
    FString LowerInput = Input.ToLower();
    
    if (LowerInput.Contains(TEXT("help")) || LowerInput.Contains(TEXT("帮助")) ||
        LowerInput.Contains(TEXT("?")) || LowerInput.Contains(TEXT("怎么用")))
    {
        return TEXT("help");
    }
    else if (LowerInput.Contains(TEXT("status")) || LowerInput.Contains(TEXT("状态")) ||
             LowerInput.Contains(TEXT("情况")))
    {
        return TEXT("status");
    }
    else if (LowerInput.Contains(TEXT("generate")) || LowerInput.Contains(TEXT("生成")) ||
             LowerInput.Contains(TEXT("创建")) || LowerInput.Contains(TEXT("music")) ||
             LowerInput.Contains(TEXT("音乐")) || LowerInput.Contains(TEXT("scene")) ||
             LowerInput.Contains(TEXT("场景")))
    {
        return TEXT("generate");
    }
    else if (LowerInput.Contains(TEXT("mode")) || LowerInput.Contains(TEXT("模式")) ||
             LowerInput.Contains(TEXT("切换")))
    {
        return TEXT("mode");
    }
    else if (LowerInput.Contains(TEXT("test")) || LowerInput.Contains(TEXT("测试")))
    {
        return TEXT("test");
    }
    else if (LowerInput.Contains(TEXT("config")) || LowerInput.Contains(TEXT("配置")) ||
             LowerInput.Contains(TEXT("设置")))
    {
        return TEXT("config");
    }
    else if (LowerInput.Contains(TEXT("exit")) || LowerInput.Contains(TEXT("quit")) ||
             LowerInput.Contains(TEXT("退出")) || LowerInput.Contains(TEXT("结束")))
    {
        return TEXT("exit");
    }
    
    return TEXT("general");
}

TArray<FString> UMingSageBrainInteractiveConsole::ExtractEntities(const FString& Input)
{
    TArray<FString> Entities;
    
    // 简单实体提取（可以扩展为更复杂的 NLP）
    TArray<FString> Tokens;
    Input.ParseIntoArray(Tokens, TEXT(" "), true);
    
    for (const FString& Token : Tokens)
    {
        if (Token.Len() > 2) // 忽略短词
        {
            Entities.Add(Token);
        }
    }
    
    return Entities;
}

FString UMingSageBrainInteractiveConsole::GenerateSmartResponse(const FString& Intent, const TArray<FString>& Entities)
{
    if (Intent == TEXT("general"))
    {
        FString Response = TEXT("我理解您的输入。");
        
        if (Entities.Num() > 0)
        {
            Response += TEXT("\n\n检测到的关键词: ");
            for (const FString& Entity : Entities)
            {
                Response += Entity + TEXT(" ");
            }
        }
        
        Response += TEXT("\n\n您可以:\n")
            TEXT("- 输入 'help' 查看帮助\n")
            TEXT("- 输入 'mode' 切换交互模式\n")
            TEXT("- 直接描述您需要的功能");
        
        return Response;
    }
    else if (Intent == TEXT("exit"))
    {
        return TEXT("再见！感谢您的使用。");
    }
    
    return TEXT("收到您的输入，正在处理...");
}

void UMingSageBrainInteractiveConsole::UpdateSessionState(EConsoleSessionState NewState)
{
    if (SessionState != NewState)
    {
        SessionState = NewState;
        OnSessionStateChanged.Broadcast(NewState);
    }
}

void UMingSageBrainInteractiveConsole::UpdateInteractionMode(EUserInteractionMode NewMode)
{
    if (CurrentMode != NewMode)
    {
        CurrentMode = NewMode;
        CurrentSession.CurrentMode = NewMode;
        OnInteractiveModeChanged.Broadcast(NewMode);
    }
}

void UMingSageBrainInteractiveConsole::LogInteraction(const FString& Direction, const FString& Content)
{
    UE_LOG(LogTemp, Log, TEXT("[%s] %s: %s"), *GetTimestamp(), *Direction, *Content);
}

FString UMingSageBrainInteractiveConsole::GetTimestamp() const
{
    return FDateTime::Now().ToString(TEXT("%H:%M:%S"));
}

int32 UMingSageBrainInteractiveConsole::GenerateSessionID()
{
    return ++SessionCounter;
}
