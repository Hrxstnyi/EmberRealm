// Copyright EmberRealm. All Rights Reserved.
#include "EmberGameState.h"
#include "Net/UnrealNetwork.h"

AEmberGameState::AEmberGameState()
{
	GameTime = FGameTime();
}

void AEmberGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEmberGameState, GameTime);
	DOREPLIFETIME(AEmberGameState, CurrentFaction);
	DOREPLIFETIME(AEmberGameState, GlobalTurn);
	DOREPLIFETIME(AEmberGameState, bGameOver);
	DOREPLIFETIME(AEmberGameState, Winner);
}
