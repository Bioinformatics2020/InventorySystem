// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventorySystemGameMode.h"
#include "InventorySystemCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include <GameFramework/PlayerState.h>

AInventorySystemGameMode::AInventorySystemGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPFinder(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	check(PlayerPawnBPFinder.Class);
	DefaultPawnClass = PlayerPawnBPFinder.Class;

	static ConstructorHelpers::FClassFinder<APlayerController> PCBPFinder(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/PC_BP.PC_BP_C'"));
	check(PCBPFinder.Class);
	PlayerControllerClass = PCBPFinder.Class;

	static ConstructorHelpers::FClassFinder<APlayerState> PlayerStateBPFinder(TEXT("Blueprint'/Game/BP_InventoryPlayerState.BP_InventoryPlayerState_C'"));
	check(PlayerStateBPFinder.Class);
	PlayerStateClass = PlayerStateBPFinder.Class;
}
