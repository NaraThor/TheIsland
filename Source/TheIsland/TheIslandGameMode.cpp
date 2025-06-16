// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheIslandGameMode.h"
#include "TheIslandCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATheIslandGameMode::ATheIslandGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
