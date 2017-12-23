// Copyright ZX.Diablo 2017

#include "Include/UE4SampleChatGameModeBase.h"

#include "Include/UE4SampleChatPlayerController.h"

void AUE4SampleChatGameModeBase::PostLogin (APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto PlayerController = Cast<AUE4SampleChatPlayerController>(NewPlayer);

	if (PlayerController)
	{
		PlayerController->ClientChatJoined();
	}
}

void AUE4SampleChatGameModeBase::Logout (AController* Exiting)
{
	Super::Logout(Exiting);

	auto PlayerController = Cast<AUE4SampleChatPlayerController>(Exiting);

	if (PlayerController)
	{
		PlayerController->ServerUpdateChatRoom();
	}
}
