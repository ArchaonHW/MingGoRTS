@echo off
echo Quick fix for common syntax errors

REM Fix some critical files manually
echo Fixing critical header files...

REM Create a simple fixed version of MingGoRTSCharacter.h
(
echo #pragma once
echo.
echo #include "CoreMinimal.h"
echo #include "GameFramework/Character.h"
echo #include "MingGoRTSCharacter.generated.h"
echo.
echo UENUM^(BlueprintType^)
echo enum class ECharacterBackground : uint8
echo {
echo     MilitaryAcademy,
echo     WarlordSon,
echo     Revolutionary,
echo     ScholarOfficial,
echo     Merchant,
echo     CommonSoldier
echo };
echo.
echo USTRUCT^(BlueprintType^)
echo struct FCharacterAttributes
echo {
echo     GENERATED_BODY^(^)
echo.
echo     UPROPERTY^(BlueprintReadOnly, Category = "Character"^)
echo     float Health;
echo.
echo     UPROPERTY^(BlueprintReadOnly, Category = "Character"^)
echo     float Strength;
echo.
echo     UPROPERTY^(BlueprintReadOnly, Category = "Character"^)
echo     float Intelligence;
echo };
) > "c:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSCharacter_Fixed.h"

REM Move the fixed file
del "c:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSCharacter.h"
ren "c:\HW\MingGoRTS\Source\MingGoRTS\Public\MingGoRTSCharacter_Fixed.h" "MingGoRTSCharacter.h"

echo Fixed MingGoRTSCharacter.h
echo.
echo Basic fixes completed. Try building again.
pause
