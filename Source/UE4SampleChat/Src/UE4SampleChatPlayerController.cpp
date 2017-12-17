// Fill out your copyright notice in the Description page of Project Settings.

#include "Include/UE4SampleChatPlayerController.h"

#include "GameFramework/PlayerState.h"

#include "Include/UE4SampleChatGameInstance.h"

void AUE4SampleChatPlayerController::BeginPlay ()
{
	Super::BeginPlay();

	this->SetInputMode(FInputModeUIOnly());
}

void AUE4SampleChatPlayerController::ClientChatJoined_Implementation ()
{
	auto GameInstance = Cast<UUE4SampleChatGameInstance>(this->GetGameInstance());

	if (GameInstance)
	{
		this->ServerSetPlayerNickname(GameInstance->GetStoredNickname());
	}
}

void AUE4SampleChatPlayerController::ServerSetPlayerNickname_Implementation (const FText& Nickname)
{
	this->PlayerState->PlayerName = Nickname.ToString();
}

bool AUE4SampleChatPlayerController::ServerSetPlayerNickname_Validate (const FText& Nickname)
{
	return true;
}
