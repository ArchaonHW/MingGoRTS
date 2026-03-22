#include "Localization/MingLocalizedWidget.h"
#include "Localization/MingLocalizationManager.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

UMingLocalizedWidget::UMingLocalizedWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , UpdateMode(ELocalizationUpdateMode::Auto)
    , TextCase(ELocalizedTextCase::Default)
    , Truncation(ETextTruncation::None)
    , MaxTextLength(100)
    , bUseRichText(false)
    , bAutoWrapText(true)
{
}

void UMingLocalizedWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 綁定到本地化系統
    if (UpdateMode == ELocalizationUpdateMode::Auto || UpdateMode == ELocalizationUpdateMode::OnLanguageChanged)
    {
        BindToLocalizationSystem();
    }

    // 初始更新文本
    UpdateText();
}

void UMingLocalizedWidget::NativeDestruct()
{
    // 解除綁定
    UnbindFromLocalizationSystem();

    Super::NativeDestruct();
}

void UMingLocalizedWidget::SetLocalizationKey(const FString& Key, const FString& InNamespace)
{
    LocalizationKey = Key;
    if (!InNamespace.IsEmpty())
    {
        Namespace = InNamespace;
    }

    // 如果設置了新的鍵，更新文本
    if (UpdateMode == ELocalizationUpdateMode::Auto)
    {
        UpdateText();
    }
}

void UMingLocalizedWidget::SetParameter(const FString& ParamName, const FString& Value)
{
    Parameters.Add(ParamName, Value);

    if (UpdateMode == ELocalizationUpdateMode::Auto)
    {
        UpdateText();
    }
}

void UMingLocalizedWidget::SetParameters(const TMap<FString, FString>& Params)
{
    Parameters = Params;

    if (UpdateMode == ELocalizationUpdateMode::Auto)
    {
        UpdateText();
    }
}

void UMingLocalizedWidget::ClearParameters()
{
    Parameters.Empty();

    if (UpdateMode == ELocalizationUpdateMode::Auto)
    {
        UpdateText();
    }
}

void UMingLocalizedWidget::UpdateText()
{
    if (LocalizationKey.IsEmpty())
    {
        // 使用默認文本
        CurrentText = DefaultText;
    }
    else
    {
        // 獲取本地化文本
        FText RawText = GetRawLocalizedText();

        // 替換參數
        RawText = ReplaceParameters(RawText);

        // 應用文本處理
        CurrentText = ProcessText(RawText);
    }

    // 更新UI顯示
    // 這裡會觸發藍圖事件通知UI更新
    OnTextUpdated(CurrentText);
}

void UMingLocalizedWidget::ForceRefresh()
{
    UpdateText();
}

void UMingLocalizedWidget::SetTextCase(ELocalizedTextCase NewCase)
{
    TextCase = NewCase;

    if (UpdateMode == ELocalizationUpdateMode::Auto)
    {
        UpdateText();
    }
}

void UMingLocalizedWidget::SetTruncation(ETextTruncation TruncationType, int32 InMaxLength)
{
    Truncation = TruncationType;
    MaxTextLength = InMaxLength;

    if (UpdateMode == ELocalizationUpdateMode::Auto)
    {
        UpdateText();
    }
}

void UMingLocalizedWidget::OnLanguageChanged(EMingLanguage NewLanguage)
{
    if (UpdateMode == ELocalizationUpdateMode::OnLanguageChanged ||
        UpdateMode == ELocalizationUpdateMode::Auto)
    {
        UpdateText();
    }
}

FText UMingLocalizedWidget::GetCurrentText() const
{
    return CurrentText;
}

FText UMingLocalizedWidget::GetRawLocalizedText() const
{
    UMingLocalizationManager* LocManager = GetLocalizationManager();
    if (LocManager)
    {
        return LocManager->GetLocalizedText(LocalizationKey, Namespace);
    }

    return DefaultText;
}

void UMingLocalizedWidget::BindToLocalizationSystem()
{
    UMingLocalizationManager* LocManager = GetLocalizationManager();
    if (LocManager)
    {
        LocManager->OnLanguageChanged.AddDynamic(this, &UMingLocalizedWidget::OnLanguageChanged);
    }
}

void UMingLocalizedWidget::UnbindFromLocalizationSystem()
{
    UMingLocalizationManager* LocManager = GetLocalizationManager();
    if (LocManager)
    {
        LocManager->OnLanguageChanged.RemoveDynamic(this, &UMingLocalizedWidget::OnLanguageChanged);
    }
}

FText UMingLocalizedWidget::ProcessText(const FText& RawText) const
{
    FText ProcessedText = RawText;

    // 應用大小寫轉換
    if (TextCase != ELocalizedTextCase::Default)
    {
        ProcessedText = ApplyTextCase(ProcessedText);
    }

    // 應用截斷
    if (Truncation != ETextTruncation::None)
    {
        ProcessedText = ApplyTruncation(ProcessedText);
    }

    return ProcessedText;
}

FText UMingLocalizedWidget::ApplyTextCase(const FText& Text) const
{
    FString TextString = Text.ToString();

    switch (TextCase)
    {
    case ELocalizedTextCase::Uppercase:
        TextString = TextString.ToUpper();
        break;
    case ELocalizedTextCase::Lowercase:
        TextString = TextString.ToLower();
        break;
    case ELocalizedTextCase::TitleCase:
    {
        // 簡單的標題大小寫轉換
        TArray<FString> Words;
        TextString.ParseIntoArray(Words, TEXT(" "), true);
        for (FString& Word : Words)
        {
            if (Word.Len() > 0)
            {
                Word[0] = FChar::ToUpper(Word[0]);
                for (int32 i = 1; i < Word.Len(); i++)
                {
                    Word[i] = FChar::ToLower(Word[i]);
                }
            }
        }
        TextString = FString::Join(Words, TEXT(" "));
        break;
    }
    case ELocalizedTextCase::SentenceCase:
    {
        // 句子大小寫：首字母大寫
        if (TextString.Len() > 0)
        {
            TextString[0] = FChar::ToUpper(TextString[0]);
        }
        break;
    }
    default:
        break;
    }

    return FText::FromString(TextString);
}

FText UMingLocalizedWidget::ApplyTruncation(const FText& Text) const
{
    FString TextString = Text.ToString();

    if (TextString.Len() <= MaxTextLength)
    {
        return Text;
    }

    switch (Truncation)
    {
    case ETextTruncation::Character:
        TextString = TextString.Left(MaxTextLength);
        break;
    case ETextTruncation::Word:
    {
        // 截斷到完整的單詞
        TArray<FString> Words;
        TextString.ParseIntoArray(Words, TEXT(" "), true);
        FString Result;
        int32 CurrentLength = 0;
        for (const FString& Word : Words)
        {
            if (CurrentLength + Word.Len() > static_cast<uint32>(MaxTextLength))
            {
                break;
            }
            if (!Result.IsEmpty())
            {
                Result += TEXT(" ");
                CurrentLength++;
            }
            Result += Word;
            CurrentLength += Word.Len();
        }
        TextString = Result;
        break;
    }
    case ETextTruncation::Ellipsis:
    {
        // 使用省略號截斷
        if (MaxTextLength > 3)
        {
            TextString = TextString.Left(MaxTextLength - 3) + TEXT("...");
        }
        else
        {
            TextString = TextString.Left(MaxTextLength);
        }
        break;
    }
    default:
        break;
    }

    return FText::FromString(TextString);
}

FText UMingLocalizedWidget::ReplaceParameters(const FText& Text) const
{
    if (Parameters.Num() == 0)
    {
        return Text;
    }

    FString TextString = Text.ToString();

    for (const TPair<FString, FString>& Param : Parameters)
    {
        FString Placeholder = FString::Printf(TEXT("{%s}"), *Param.Key);
        TextString = TextString.Replace(*Placeholder, *Param.Value);
    }

    return FText::FromString(TextString);
}

UMingLocalizationManager* UMingLocalizedWidget::GetLocalizationManager() const
{
    // 從遊戲實例獲取本地化管理器
    // 實際實現需要根據項目架構調整
    return UMingLocalizationManager::GetLocalizationManager(GetWorld());
}
