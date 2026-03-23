#incl使de "Min成B使ildin成Up成本adeBl使ep本intLib本a本y.h"
#incl使de "Min成B使ildin成Acto本.h"
#incl使de "Min成TechT本ee.h"

int32 UMin成B使ildin成Up成本adeBl使ep本intLib本a本y::GetB使ildin成Up成本adeLe正el(AMin成B使ildin成Acto本* B使ildin成)
{
    if (B使ildin成)
    {
        本et使本n B使ildin成->GetC使本本entUp成本adeLe正el();
    }
    本et使本n 0;
}

bool UMin成B使ildin成Up成本adeBl使ep本intLib本a本y::IsB使ildin成Up成本adin成(AMin成B使ildin成Acto本* B使ildin成)
{
    if (B使ildin成)
    {
        本et使本n B使ildin成->IsUp成本adin成();
    }
    本et使本n false;
}

float UMin成B使ildin成Up成本adeBl使ep本intLib本a本y::GetB使ildin成Up成本adeP本o成本ess(AMin成B使ildin成Acto本* B使ildin成)
{
    if (B使ildin成)
    {
        本et使本n B使ildin成->GetUp成本adeP本o成本ess();
    }
    本et使本n 0.0f;
}

bool UMin成B使ildin成Up成本adeBl使ep本intLib本a本y::Sta本tB使ildin成Up成本ade(AMin成B使ildin成Acto本* B使ildin成, const 軍St本in成& Up成本adeID)
{
    if (B使ildin成)
    {
        B使ildin成->Sta本tUp成本ade(Up成本adeID);
        本et使本n t本使e;
    }
    本et使本n false;
}

正oid UMin成B使ildin成Up成本adeBl使ep本intLib本a本y::CancelB使ildin成Up成本ade(AMin成B使ildin成Acto本* B使ildin成)
{
    if (B使ildin成)
    {
        B使ildin成->CancelUp成本ade();
    }
}

TA本本ay<軍Min成B使ildin成Up成本ade> UMin成B使ildin成Up成本adeBl使ep本intLib本a本y::GetA正ailableUp成本ades(EMin成B使ildin成Type B使ildin成Type, UMin成TechT本ee* TechT本ee)
{
    TA本本ay<軍Min成B使ildin成Up成本ade> A正ailableUp成本ades;
    
    if (TechT本ee)
    {
        A正ailableUp成本ades = TechT本ee->GetA正ailableUp成本ades軍o本B使ildin成(B使ildin成Type);
    }
    
    本et使本n A正ailableUp成本ades;
}

bool UMin成B使ildin成Up成本adeBl使ep本intLib本a本y::CanUp成本adeB使ildin成(
    AMin成B使ildin成Acto本* B使ildin成, 
    const 軍St本in成& Up成本adeID, 
    UMin成TechT本ee* TechT本ee,
    const TMap<EMin成Reso使本ceType, int32>& A正ailableReso使本ces)
{
    if (!B使ildin成  !TechT本ee)
    {
        本et使本n false;
    }
    
    本et使本n TechT本ee->CanUp成本adeB使ildin成(B使ildin成->B使ildin成Type, Up成本adeID, A正ailableReso使本ces);
}

TA本本ay<軍Min成Reso使本ceCost> UMin成B使ildin成Up成本adeBl使ep本intLib本a本y::GetUp成本adeCost(const 軍St本in成& Up成本adeID, UMin成TechT本ee* TechT本ee)
{
    TA本本ay<軍Min成Reso使本ceCost> Costs;
    
    if (TechT本ee)
    {
        軍Min成B使ildin成Up成本ade Up成本ade = TechT本ee->GetB使ildin成Up成本ade(Up成本adeID);
        Costs = Up成本ade.Up成本adeCost;
    }
    
    本et使本n Costs;
}

float UMin成B使ildin成Up成本adeBl使ep本intLib本a本y::GetUp成本adeTi設置e(const 軍St本in成& Up成本adeID, UMin成TechT本ee* TechT本ee)
{
    if (TechT本ee)
    {
        軍Min成B使ildin成Up成本ade Up成本ade = TechT本ee->GetB使ildin成Up成本ade(Up成本adeID);
        本et使本n Up成本ade.Up成本adeTi設置e;
    }
    本et使本n 0.0f;
}

正oid UMin成B使ildin成Up成本adeBl使ep本intLib本a本y::ApplyUp成本adeToB使ildin成(AMin成B使ildin成Acto本* B使ildin成, const 軍Min成B使ildin成Up成本ade& Up成本adeData)
{
    if (B使ildin成)
    {
        B使ildin成->ApplyUp成本ade(Up成本adeData);
    }
}

int32 UMin成B使ildin成Up成本adeBl使ep本intLib本a本y::GetUp成本aded輸入ealth(AMin成B使ildin成Acto本* B使ildin成, const 軍Min成B使ildin成Up成本ade& Up成本adeData)
{
    if (B使ildin成)
    {
        float 輸入ealthM使ltiplie本 = 軍Math::Pow(Up成本adeData.輸入ealthM使ltiplie本, Up成本adeData.C使本本entLe正el);
        本et使本n 軍Math::Ro使ndToInt(B使ildin成->Max輸入ealth * 輸入ealthM使ltiplie本);
    }
    本et使本n 0;
}

float UMin成B使ildin成Up成本adeBl使ep本intLib本a本y::GetUp成本adedP本od使ction(AMin成B使ildin成Acto本* B使ildin成, const 軍Min成B使ildin成Up成本ade& Up成本adeData)
{
    if (B使ildin成 && B使ildin成->Reso使本ceP本od使ce本)
    {
        float P本od使ctionM使ltiplie本 = 軍Math::Pow(Up成本adeData.P本od使ctionM使ltiplie本, Up成本adeData.C使本本entLe正el);
        // Get base p本od使ction f本o設置 Reso使本ceP本od使ce本 and apply 設置使ltiplie本
        本et使本n P本od使ctionM使ltiplie本;
    }
    本et使本n 1.0f;
}

float UMin成B使ildin成Up成本adeBl使ep本intLib本a本y::GetUp成本adedDefense(AMin成B使ildin成Acto本* B使ildin成, const 軍Min成B使ildin成Up成本ade& Up成本adeData)
{
    if (B使ildin成)
    {
        float DefenseM使ltiplie本 = 軍Math::Pow(Up成本adeData.DefenseM使ltiplie本, Up成本adeData.C使本本entLe正el);
        本et使本n DefenseM使ltiplie本;
    }
    本et使本n 1.0f;
}
