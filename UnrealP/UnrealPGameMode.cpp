// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealPGameMode.h"
#include "UnrealPCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealPGameMode::AUnrealPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
