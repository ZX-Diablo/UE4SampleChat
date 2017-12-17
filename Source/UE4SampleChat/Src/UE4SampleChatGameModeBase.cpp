// Fill out your copyright notice in the Description page of Project Settings.

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
