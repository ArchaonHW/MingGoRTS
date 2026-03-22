#include "MingGoRTSCharacterCreationManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UMingGoRTSCharacterCreationManager::UMingGoRTSCharacterCreationManager()
{
    InitializeNamePools();
}

bool UMingGoRTSCharacterCreationManager::CreateCharacter(const FMingCharacterData& CharacterData)
{
    if (!ValidateCharacterData(CharacterData))
    {
        UE_LOG(LogTemp, Error, TEXT("角色數據驗證失敗"));
        return false;
    }

    // 廣播角色創建事件
    OnCharacterCreated.Broadcast(CharacterData);

    UE_LOG(LogTemp, Log, TEXT("成功創建角色：%s"), *CharacterData.CharacterName);
    return true;
}

bool UMingGoRTSCharacterCreationManager::ValidateCharacterData(const FMingCharacterData& CharacterData)
{
    return ValidateName(CharacterData.CharacterName) &&
           ValidateAttributes(CharacterData.Attributes) &&
           ValidateAge(CharacterData.Age);
}

FString UMingGoRTSCharacterCreationManager::GetBackgroundDescription(ECharacterBackground Background)
{
    switch (Background)
    {
    case ECharacterBackground::MilitaryAcademy:
        return TEXT("畢業於黃埔軍校，接受現代軍事教育，具備良好的戰術素養。統帥屬性獲得額外加成。");
    case ECharacterBackground::WarlordSon:
        return TEXT("出身軍閥世家，從小耳濡目染軍事事務，擁有豐厚的人脈資源。魅力屬性獲得額外加成。");
    case ECharacterBackground::Revolutionary:
        return TEXT("懷揣革命理想，為國家前途奮鬥的熱血青年。勇武屬性獲得額外加成。");
    case ECharacterBackground::ScholarOfficial:
        return TEXT("棄文從武的讀書人，以智謀和策略見長。智謀屬性獲得額外加成。");
    case ECharacterBackground::Merchant:
        return TEXT("富商從軍，善於理財和後勤管理。體質屬性獲得額外加成。");
    case ECharacterBackground::CommonSoldier:
        return TEXT("從基層士兵一步步成長起來，經驗豐富，深得士兵擁戴。所有屬性獲得小幅加成。");
    default:
        return TEXT("未知背景");
    }
}

TArray<FCharacterSkill> UMingGoRTSCharacterCreationManager::GetRecommendedSkills(ECharacterBackground Background)
{
    TArray<FCharacterSkill> RecommendedSkills;

    switch (Background)
    {
    case ECharacterBackground::MilitaryAcademy:
        RecommendedSkills.Add(FCharacterSkill{ FName("TacticalCommand"), TEXT("戰術指揮"), TEXT("提升部隊指揮效率"), 2, 5 });
        RecommendedSkills.Add(FCharacterSkill{ FName("MilitaryTheory"), TEXT("軍事理論"), TEXT("增強戰略規劃能力"), 1, 5 });
        break;
    case ECharacterBackground::WarlordSon:
        RecommendedSkills.Add(FCharacterSkill{ FName("Diplomacy"), TEXT("外交手腕"), TEXT("提升談判和聯盟能力"), 2, 5 });
        RecommendedSkills.Add(FCharacterSkill{ FName("ResourceManagement"), TEXT("資源管理"), TEXT("提升經濟運營效率"), 1, 5 });
        break;
    case ECharacterBackground::Revolutionary:
        RecommendedSkills.Add(FCharacterSkill{ FName("Inspiration"), TEXT("鼓舞人心"), TEXT("提升部隊士氣"), 2, 5 });
        RecommendedSkills.Add(FCharacterSkill{ FName("GuerrillaWarfare"), TEXT("游擊戰術"), TEXT("提升非對稱戰鬥能力"), 1, 5 });
        break;
    case ECharacterBackground::ScholarOfficial:
        RecommendedSkills.Add(FCharacterSkill{ FName("Strategy"), TEXT("戰略規劃"), TEXT("提升長期戰略思維"), 2, 5 });
        RecommendedSkills.Add(FCharacterSkill{ FName("IntelligenceAnalysis"), TEXT("情報分析"), TEXT("提升情報處理能力"), 1, 5 });
        break;
    case ECharacterBackground::Merchant:
        RecommendedSkills.Add(FCharacterSkill{ FName("Logistics"), TEXT("後勤管理"), TEXT("提升補給線效率"), 2, 5 });
        RecommendedSkills.Add(FCharacterSkill{ FName("Trade"), TEXT("貿易技巧"), TEXT("提升商業收益"), 1, 5 });
        break;
    case ECharacterBackground::CommonSoldier:
        RecommendedSkills.Add(FCharacterSkill{ FName("CombatExperience"), TEXT("戰鬥經驗"), TEXT("提升個人戰鬥能力"), 2, 5 });
        RecommendedSkills.Add(FCharacterSkill{ FName("MoraleBoost"), TEXT("士氣提升"), TEXT("增強部隊凝聚力"), 1, 5 });
        break;
    }

    return RecommendedSkills;
}

bool UMingGoRTSCharacterCreationManager::SaveCharacterToSlot(const FMingCharacterData& CharacterData, const FString& SlotName)
{
    // TODO: 實現實際的存檔邏輯
    UE_LOG(LogTemp, Log, TEXT("保存角色 %s 到存檔槽 %s"), *CharacterData.CharacterName, *SlotName);
    return true;
}

bool UMingGoRTSCharacterCreationManager::LoadCharacterFromSlot(const FString& SlotName, FMingCharacterData& OutCharacterData)
{
    // TODO: 實現實際的載入邏輯
    UE_LOG(LogTemp, Log, TEXT("從存檔槽 %s 載入角色數據"), *SlotName);
    return true;
}

bool UMingGoRTSCharacterCreationManager::IsCharacterNameAvailable(const FString& CharacterName)
{
    // TODO: 檢查名稱是否已被使用
    return !CharacterName.IsEmpty() && CharacterName.Len() >= 2 && CharacterName.Len() <= 20;
}

FString UMingGoRTSCharacterCreationManager::GenerateRandomCharacterName()
{
    FString Surname = GetRandomSurname();
    FString GivenName = GetRandomGivenName(true); // 默認生成男性名字
    return Surname + GivenName;
}

bool UMingGoRTSCharacterCreationManager::ValidateName(const FString& Name)
{
    if (Name.IsEmpty() || Name.Len() < 2 || Name.Len() > 20)
    {
        return false;
    }

    // 檢查是否包含非法字符
    for (TCHAR Char : Name)
    {
        if (!FChar::IsAlpha(Char) && Char != TEXT(' '))
        {
            return false;
        }
    }

    return true;
}

bool UMingGoRTSCharacterCreationManager::ValidateAttributes(const FCharacterAttributes& Attributes)
{
    return Attributes.Leadership >= 0 && Attributes.Leadership <= 100 &&
           Attributes.Intelligence >= 0 && Attributes.Intelligence <= 100 &&
           Attributes.Courage >= 0 && Attributes.Courage <= 100 &&
           Attributes.Charisma >= 0 && Attributes.Charisma <= 100 &&
           Attributes.Constitution >= 0 && Attributes.Constitution <= 100;
}

bool UMingGoRTSCharacterCreationManager::ValidateAge(int32 InAge)
{
    return InAge >= 18 && InAge <= 65;
}

void UMingGoRTSCharacterCreationManager::InitializeNamePools()
{
    // 常見中文姓氏
    Surnames = {
        TEXT("張"), TEXT("王"), TEXT("李"), TEXT("趙"), TEXT("劉"), TEXT("陳"), TEXT("楊"), TEXT("黃"),
        TEXT("周"), TEXT("吳"), TEXT("徐"), TEXT("孫"), TEXT("馬"), TEXT("朱"), TEXT("胡"), TEXT("郭"),
        TEXT("何"), TEXT("林"), TEXT("羅"), TEXT("高"), TEXT("梁"), TEXT("鄭"), TEXT("謝"), TEXT("唐")
    };

    // 男性名字
    MaleNames = {
        TEXT("偉"), TEXT("強"), TEXT("磊"), TEXT("洋"), TEXT("勇"), TEXT("軍"), TEXT("傑"), TEXT("濤"),
        TEXT("超"), TEXT("明"), TEXT("輝"), TEXT("鵬"), TEXT("華"), TEXT("平"), TEXT("建"), TEXT("國"),
        TEXT("志"), TEXT("中"), TEXT("文"), TEXT("新"), TEXT("德"), TEXT("海"), TEXT("龍"), TEXT("雲")
    };

    // 女性名字
    FemaleNames = {
        TEXT("靜"), TEXT("慧"), TEXT("美"), TEXT("麗"), TEXT("華"), TEXT("敏"), TEXT("芳"), TEXT("燕"),
        TEXT("玲"), TEXT("紅"), TEXT("梅"), TEXT("婷"), TEXT("雪"), TEXT("琳"), TEXT("萍"), TEXT("娟"),
        TEXT("霞"), TEXT("穎"), TEXT("潔"), TEXT("倩"), TEXT("琪"), TEXT("瑤"), TEXT("薇"), TEXT("丹")
    };
}

FString UMingGoRTSCharacterCreationManager::GetRandomSurname()
{
    if (Surnames.Num() > 0)
    {
        int32 Index = FMath::RandRange(0, Surnames.Num() - 1);
        return Surnames[Index];
    }
    return TEXT("李");
}

FString UMingGoRTSCharacterCreationManager::GetRandomGivenName(bool bIsMale)
{
    const TArray<FString>& NamePool = bIsMale ? MaleNames : FemaleNames;
    
    if (NamePool.Num() > 0)
    {
        int32 Index = FMath::RandRange(0, NamePool.Num() - 1);
        return NamePool[Index];
    }
    return bIsMale ? TEXT("偉") : TEXT("靜");
}
