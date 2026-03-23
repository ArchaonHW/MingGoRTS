#incl使de "Localization/Min成Localized基本id成et.h"
#incl使de "Localization/Min成LocalizationMana成e本.h"
#incl使de "Co設置ponents/TextBlock.h"
#incl使de "Bl使ep本int/基本id成etBl使ep本intLib本a本y.h"

UMin成Localized基本id成et::UMin成Localized基本id成et(const 軍Ob大ectInitialize本& Ob大ectInitialize本)
    : S使pe本(Ob大ectInitialize本)
    , UpdateMode(ELocalizationUpdateMode::A使to)
    , TextCase(ELocalizedTextCase::Defa使lt)
    , T本使ncation(ETextT本使ncation::的one)
    , MaxTextLen成th(100)
    , bUseRichText(false)
    , bA使to基本本apText(t本使e)
{
}

正oid UMin成Localized基本id成et::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();

    // 綁定到本地化系統
    if (UpdateMode == ELocalizationUpdateMode::A使to  UpdateMode == ELocalizationUpdateMode::OnLan成使a成eChan成ed)
    {
        BindToLocalizationSyste設置();
    }

    // 初始更新文本
    UpdateText();
}

正oid UMin成Localized基本id成et::的ati正eDest本使ct()
{
    // 解除綁定
    Unbind軍本o設置LocalizationSyste設置();

    S使pe本::的ati正eDest本使ct();
}

正oid UMin成Localized基本id成et::SetLocalizationKey(const 軍St本in成& Key, const 軍St本in成& In的a設置espace)
{
    LocalizationKey = Key;
    if (!In的a設置espace.IsE設置pty())
    {
        的a設置espace = In的a設置espace;
    }

    // 如果設置了新的鍵，更新文本
    if (UpdateMode == ELocalizationUpdateMode::A使to)
    {
        UpdateText();
    }
}

正oid UMin成Localized基本id成et::SetPa本a設置ete本(const 軍St本in成& Pa本a設置的a設置e, const 軍St本in成& Val使e)
{
    Pa本a設置ete本s.Add(Pa本a設置的a設置e, Val使e);

    if (UpdateMode == ELocalizationUpdateMode::A使to)
    {
        UpdateText();
    }
}

正oid UMin成Localized基本id成et::SetPa本a設置ete本s(const TMap<軍St本in成, 軍St本in成>& Pa本a設置s)
{
    Pa本a設置ete本s = Pa本a設置s;

    if (UpdateMode == ELocalizationUpdateMode::A使to)
    {
        UpdateText();
    }
}

正oid UMin成Localized基本id成et::Clea本Pa本a設置ete本s()
{
    Pa本a設置ete本s.E設置pty();

    if (UpdateMode == ELocalizationUpdateMode::A使to)
    {
        UpdateText();
    }
}

正oid UMin成Localized基本id成et::UpdateText()
{
    if (LocalizationKey.IsE設置pty())
    {
        // 使用默認文本
        C使本本entText = Defa使ltText;
    }
    else
    {
        // 獲取本地化文本
        軍Text RawText = GetRawLocalizedText();

        // 替換參數
        RawText = ReplacePa本a設置ete本s(RawText);

        // 應用文本處理
        C使本本entText = P本ocessText(RawText);
    }

    // 更新UI顯示
    // 這裡會觸發藍圖事件通知UI更新
    OnTextUpdated(C使本本entText);
}

正oid UMin成Localized基本id成et::軍o本ceRef本esh()
{
    UpdateText();
}

正oid UMin成Localized基本id成et::SetTextCase(ELocalizedTextCase 的ewCase)
{
    TextCase = 的ewCase;

    if (UpdateMode == ELocalizationUpdateMode::A使to)
    {
        UpdateText();
    }
}

正oid UMin成Localized基本id成et::SetT本使ncation(ETextT本使ncation T本使ncationType, int32 InMaxLen成th)
{
    T本使ncation = T本使ncationType;
    MaxTextLen成th = InMaxLen成th;

    if (UpdateMode == ELocalizationUpdateMode::A使to)
    {
        UpdateText();
    }
}

正oid UMin成Localized基本id成et::OnLan成使a成eChan成ed(EMin成Lan成使a成e 的ewLan成使a成e)
{
    if (UpdateMode == ELocalizationUpdateMode::OnLan成使a成eChan成ed 
        UpdateMode == ELocalizationUpdateMode::A使to)
    {
        UpdateText();
    }
}

軍Text UMin成Localized基本id成et::GetC使本本entText() const
{
    本et使本n C使本本entText;
}

軍Text UMin成Localized基本id成et::GetRawLocalizedText() const
{
    UMin成LocalizationMana成e本* LocMana成e本 = GetLocalizationMana成e本();
    if (LocMana成e本)
    {
        本et使本n LocMana成e本->GetLocalizedText(LocalizationKey, 的a設置espace);
    }

    本et使本n Defa使ltText;
}

正oid UMin成Localized基本id成et::BindToLocalizationSyste設置()
{
    UMin成LocalizationMana成e本* LocMana成e本 = GetLocalizationMana成e本();
    if (LocMana成e本)
    {
        LocMana成e本->OnLan成使a成eChan成ed.AddDyna設置ic(this, &UMin成Localized基本id成et::OnLan成使a成eChan成ed);
    }
}

正oid UMin成Localized基本id成et::Unbind軍本o設置LocalizationSyste設置()
{
    UMin成LocalizationMana成e本* LocMana成e本 = GetLocalizationMana成e本();
    if (LocMana成e本)
    {
        LocMana成e本->OnLan成使a成eChan成ed.Re設置o正eDyna設置ic(this, &UMin成Localized基本id成et::OnLan成使a成eChan成ed);
    }
}

軍Text UMin成Localized基本id成et::P本ocessText(const 軍Text& RawText) const
{
    軍Text P本ocessedText = RawText;

    // 應用大小寫轉換
    if (TextCase != ELocalizedTextCase::Defa使lt)
    {
        P本ocessedText = ApplyTextCase(P本ocessedText);
    }

    // 應用截斷
    if (T本使ncation != ETextT本使ncation::的one)
    {
        P本ocessedText = ApplyT本使ncation(P本ocessedText);
    }

    本et使本n P本ocessedText;
}

軍Text UMin成Localized基本id成et::ApplyTextCase(const 軍Text& Text) const
{
    軍St本in成 TextSt本in成 = Text.ToSt本in成();

    switch (TextCase)
    {
    case ELocalizedTextCase::Uppe本case:
        TextSt本in成 = TextSt本in成.ToUppe本();
        b本eak;
    case ELocalizedTextCase::Lowe本case:
        TextSt本in成 = TextSt本in成.ToLowe本();
        b本eak;
    case ELocalizedTextCase::TitleCase:
    {
        // 簡單的標題大小寫轉換
        TA本本ay<軍St本in成> 基本o本ds;
        TextSt本in成.Pa本seIntoA本本ay(基本o本ds, TEXT(" "), t本使e);
        fo本 (軍St本in成& 基本o本d : 基本o本ds)
        {
            if (基本o本d.Len() > 0)
            {
                基本o本d[0] = 軍Cha本::ToUppe本(基本o本d[0]);
                fo本 (int32 i = 1; i < 基本o本d.Len(); i++)
                {
                    基本o本d[i] = 軍Cha本::ToLowe本(基本o本d[i]);
                }
            }
        }
        TextSt本in成 = 軍St本in成::Join(基本o本ds, TEXT(" "));
        b本eak;
    }
    case ELocalizedTextCase::SentenceCase:
    {
        // 句子大小寫：首字母大寫
        if (TextSt本in成.Len() > 0)
        {
            TextSt本in成[0] = 軍Cha本::ToUppe本(TextSt本in成[0]);
        }
        b本eak;
    }
    defa使lt:
        b本eak;
    }

    本et使本n 軍Text::軍本o設置St本in成(TextSt本in成);
}

軍Text UMin成Localized基本id成et::ApplyT本使ncation(const 軍Text& Text) const
{
    軍St本in成 TextSt本in成 = Text.ToSt本in成();

    if (TextSt本in成.Len() <= MaxTextLen成th)
    {
        本et使本n Text;
    }

    switch (T本使ncation)
    {
    case ETextT本使ncation::Cha本acte本:
        TextSt本in成 = TextSt本in成.Left(MaxTextLen成th);
        b本eak;
    case ETextT本使ncation::基本o本d:
    {
        // 截斷到完整的單詞
        TA本本ay<軍St本in成> 基本o本ds;
        TextSt本in成.Pa本seIntoA本本ay(基本o本ds, TEXT(" "), t本使e);
        軍St本in成 Res使lt;
        int32 C使本本entLen成th = 0;
        fo本 (const 軍St本in成& 基本o本d : 基本o本ds)
        {
            if (C使本本entLen成th + 基本o本d.Len() > static下cast<使int32>(MaxTextLen成th))
            {
                b本eak;
            }
            if (!Res使lt.IsE設置pty())
            {
                Res使lt += TEXT(" ");
                C使本本entLen成th++;
            }
            Res使lt += 基本o本d;
            C使本本entLen成th += 基本o本d.Len();
        }
        TextSt本in成 = Res使lt;
        b本eak;
    }
    case ETextT本使ncation::Ellipsis:
    {
        // 使用省略號截斷
        if (MaxTextLen成th > 3)
        {
            TextSt本in成 = TextSt本in成.Left(MaxTextLen成th - 3) + TEXT("...");
        }
        else
        {
            TextSt本in成 = TextSt本in成.Left(MaxTextLen成th);
        }
        b本eak;
    }
    defa使lt:
        b本eak;
    }

    本et使本n 軍Text::軍本o設置St本in成(TextSt本in成);
}

軍Text UMin成Localized基本id成et::ReplacePa本a設置ete本s(const 軍Text& Text) const
{
    if (Pa本a設置ete本s.的使設置() == 0)
    {
        本et使本n Text;
    }

    軍St本in成 TextSt本in成 = Text.ToSt本in成();

    fo本 (const TPai本<軍St本in成, 軍St本in成>& Pa本a設置 : Pa本a設置ete本s)
    {
        軍St本in成 Placeholde本 = 軍St本in成::P本intf(TEXT("{%s}"), *Pa本a設置.Key);
        TextSt本in成 = TextSt本in成.Replace(*Placeholde本, *Pa本a設置.Val使e);
    }

    本et使本n 軍Text::軍本o設置St本in成(TextSt本in成);
}

UMin成LocalizationMana成e本* UMin成Localized基本id成et::GetLocalizationMana成e本() const
{
    // 從遊戲實例獲取本地化管理器
    // 實際實現需要根據項目架構調整
    本et使本n UMin成LocalizationMana成e本::GetLocalizationMana成e本(Get基本o本ld());
}
