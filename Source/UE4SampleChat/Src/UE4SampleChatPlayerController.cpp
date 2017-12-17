// Fill out your copyright notice in the Description page of Project Settings.

#include "Include/UE4SampleChatPlayerController.h"

#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
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

void AUE4SampleChatPlayerController::ServerSetPlayerNickname_Implementation (const FString& Nickname)
{
	this->PlayerState->PlayerName = Nickname;

	TArray<FString> Nicknames;

	for (auto PlayerState : this->GetWorld()->GetGameState()->PlayerArray)
	{
		Nicknames.Add(PlayerState->PlayerName);
	}

	this->BroadcastUpdateChatRoom(Nicknames);
}

bool AUE4SampleChatPlayerController::ServerSetPlayerNickname_Validate (const FString& Nickname)
{
	return true;
}

void AUE4SampleChatPlayerController::BroadcastUpdateChatRoom_Implementation(const TArray<FString>& Nicknames)
{
	for (const auto& Nickname : Nicknames)
	{
		UE_LOG(LogTemp, Log, TEXT("`%s`"), *Nickname);
	}
}
