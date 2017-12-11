// Fill out your copyright notice in the Description page of Project Settings.

#include "Include/UE4SampleChatGameInstance.h"

#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

#include "Include/UE4SampleChatGameModeBase.h"

UUE4SampleChatGameInstance::UUE4SampleChatGameInstance()
{
	this->MaxClients = 32;
	this->SessionName = TEXT("Game");
	this->ChatLevel = TEXT("/Game/Levels/ChatLevel");

	this->OnSessionReadyDelegate = AUE4SampleChatGameSession::FOnSessionReadyDelegate::CreateUObject(this, &UUE4SampleChatGameInstance::OnSessionReady);
}

void UUE4SampleChatGameInstance::HostChat (const FText& Nickname)
{
	auto GameSession = this->GetGameSession();
	auto Player = this->GetFirstGamePlayer();

	if (GameSession && Player)
	{
		this->OnSessionReadyDelegateHandle = GameSession->AddOnSessionReadyDelegate_Handle(this->OnSessionReadyDelegate);
		if (!GameSession->HostSession(*Player->GetPreferredUniqueNetId(), this->SessionName, this->MaxClients))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create session"));
		}
	}
}

void UUE4SampleChatGameInstance::JoinChat (const FText & Nickname)
{
	auto GameSession = this->GetGameSession();
	auto Player = this->GetFirstGamePlayer();

	if (GameSession && Player)
	{
		if (!GameSession->FindSession(*Player->GetPreferredUniqueNetId()))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to find session"));
		}
	}
}

void UUE4SampleChatGameInstance::OnSessionReady (FName SessionName, bool bWasSuccessful)
{
	auto GameSession = this->GetGameSession();

	if (GameSession)
	{
		GameSession->ClearOnSessionReadyDelegate_Handle(this->OnSessionReadyDelegateHandle);
	}

	if (bWasSuccessful)
	{
		auto World = this->GetWorld();

		if (World)
		{
			World->ServerTravel(this->ChatLevel + "?listen");
		}
	}
}

AUE4SampleChatGameSession* UUE4SampleChatGameInstance::GetGameSession () const
{
	auto World = this->GetWorld();

	if (World)
	{
		auto GameMode = World->GetAuthGameMode();

		if (GameMode)
		{
			return Cast<AUE4SampleChatGameSession>(GameMode->GameSession);
		}
	}

	return nullptr;
}
