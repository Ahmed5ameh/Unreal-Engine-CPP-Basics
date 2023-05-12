// Copyright Epic Games, Inc. All Rights Reserved.

#include "GDESSYTGameMode.h"
#include "GDESSYTCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGDESSYTGameMode::AGDESSYTGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
