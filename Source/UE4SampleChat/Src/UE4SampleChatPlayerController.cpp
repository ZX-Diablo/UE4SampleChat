// Copyright ZX.Diablo 2017

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

void AUE4SampleChatPlayerController::ClientUpdateChatRoom_Implementation (const TArray<FString>& NicknameArray)
{
	this->ClearChatRoom();
	for (const auto& Nickname : NicknameArray)
	{
		this->AddChatRoomEntry(Nickname);
	}
}

void AUE4SampleChatPlayerController::ClientReceiveNewMessage_Implementation (const FString& Date, const FString& Nickname, const FText& Message)
{
	this->AddChatLogEntry(Date, Nickname, Message);
}

void AUE4SampleChatPlayerController::ServerSetPlayerNickname_Implementation (const FString& Nickname)
{
	this->PlayerState->PlayerName = Nickname;
	this->ServerUpdateChatRoom();
}

bool AUE4SampleChatPlayerController::ServerSetPlayerNickname_Validate (const FString& Nickname)
{
	return true;
}

void AUE4SampleChatPlayerController::ServerUpdateChatRoom_Implementation ()
{
	TArray<FString> NicknameArray = this->GetNicknameArray();

	for (auto It = this->GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		auto PlayerController = Cast<AUE4SampleChatPlayerController>(*It);

		if (PlayerController)
		{
			PlayerController->ClientUpdateChatRoom(NicknameArray);
		}
	}
}

bool AUE4SampleChatPlayerController::ServerUpdateChatRoom_Validate ()
{
	return true;
}

void AUE4SampleChatPlayerController::ServerClientRequestUpdateChatRoom_Implementation ()
{
	TArray<FString> NicknameArray = this->GetNicknameArray();

	this->ClientUpdateChatRoom(NicknameArray);
}

bool AUE4SampleChatPlayerController::ServerClientRequestUpdateChatRoom_Validate ()
{
	return true;
}

void AUE4SampleChatPlayerController::ServerSendMessage_Implementation (const FText& Message)
{
	FString Date = FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S"));

	for (auto It = this->GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		auto PlayerController = Cast<AUE4SampleChatPlayerController>(*It);

		if (PlayerController)
		{
			PlayerController->ClientReceiveNewMessage(Date, this->PlayerState->PlayerName, Message);
		}
	}
}

bool AUE4SampleChatPlayerController::ServerSendMessage_Validate (const FText& Message)
{
	return true;
}

TArray<FString> AUE4SampleChatPlayerController::GetNicknameArray () const
{
	TArray<FString> NicknameArray;

	for (auto PlayerState : this->GetWorld()->GetGameState()->PlayerArray)
	{
		// Check for disconnected players
		if (!PlayerState->GetOwner()->IsActorBeingDestroyed())
		{
			NicknameArray.Add(PlayerState->PlayerName);
		}
	}

	return NicknameArray;
}
