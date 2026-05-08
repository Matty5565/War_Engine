#include "WarEngineGameMode.h"
#include "UObject/ConstructorHelpers.h"

AWarEngineGameMode::AWarEngineGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DynamicPawnBP(TEXT("/CesiumForUnreal/DynamicPawn"));
	if (DynamicPawnBP.Succeeded())
	{
		DefaultPawnClass = DynamicPawnBP.Class;
	}
}
