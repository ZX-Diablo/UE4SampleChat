// Copyright ZX.Diablo 2017

#include "Include/UE4SampleChatGameInstance.h"

#include "Engine/World.h"
#include "UserWidget.h"

#include "Include/UE4SampleChatGameModeBase.h"

UUE4SampleChatGameInstance::UUE4SampleChatGameInstance ()
{
	this->MaxClients = 32;
	this->SessionName = TEXT("Game");
	this->MainLevel = TEXT("/Game/Levels/MainLevel");
	this->ChatLevel = TEXT("/Game/Levels/ChatLevel");

	this->OnSessionReadyDelegate = AUE4SampleChatGameSession::FOnSessionReadyDelegate::CreateUObject(this, &UUE4SampleChatGameInstance::OnSessionReady);
	this->OnSessionFoundDelegate = AUE4SampleChatGameSession::FOnSessionFoundDelegate::CreateUObject(this, &UUE4SampleChatGameInstance::OnSessionFound);
	this->OnSessionJoinedDelegate = AUE4SampleChatGameSession::FOnSessionJoinedDelegate::CreateUObject(this, &UUE4SampleChatGameInstance::OnSessionJoined);
	this->OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UUE4SampleChatGameInstance::OnDestroySessionComplete);
}

void UUE4SampleChatGameInstance::HostChat (const FString& Nickname)
{
	auto GameSession = this->GetGameSession();
	auto Player = this->GetFirstGamePlayer();

	this->Nickname = Nickname;

	if (GameSession && Player)
	{
		this->OnSessionReadyDelegateHandle = GameSession->AddOnSessionReadyDelegate_Handle(this->OnSessionReadyDelegate);
		if (!GameSession->HostSession(*Player->GetPreferredUniqueNetId(), this->SessionName, this->MaxClients))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create session"));
		}
	}
}

void UUE4SampleChatGameInstance::JoinChat (const FString& Nickname)
{
	auto GameSession = this->GetGameSession();
	auto Player = this->GetFirstGamePlayer();

	this->Nickname = Nickname;

	if (GameSession && Player)
	{
		this->OnSessionFoundDelegateHandle = GameSession->AddOnSessionFoundDelegate_Handle(this->OnSessionFoundDelegate);
		if (!GameSession->FindSession(*Player->GetPreferredUniqueNetId()))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to find session"));
		}
	}
}

void UUE4SampleChatGameInstance::DisconnectChat ()
{
	auto SessionManager = Online::GetSessionInterface();

	if (SessionManager.IsValid())
	{
		this->OnDestroySessionCompleteDelegateHandle = SessionManager->AddOnDestroySessionCompleteDelegate_Handle(this->OnDestroySessionCompleteDelegate);
		SessionManager->DestroySession(this->SessionName);
	}
}

void UUE4SampleChatGameInstance::ShowMainMenu ()
{
	this->ShowMenuHelper(this->MainMenu);
}

void UUE4SampleChatGameInstance::ShowChatMenu ()
{
	this->ShowMenuHelper(this->ChatMenu);
}

const FString& UUE4SampleChatGameInstance::GetStoredNickname () const
{
	return this->Nickname;
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

void UUE4SampleChatGameInstance::OnSessionFound (const FOnlineSessionSearchResult& SearchResult, bool bWasSuccessful)
{
	auto GameSession = this->GetGameSession();
	auto Player = this->GetFirstGamePlayer();

	if (GameSession && Player)
	{
		GameSession->ClearOnSessionFoundDelegate_Handle(this->OnSessionFoundDelegateHandle);

		if (bWasSuccessful && SearchResult.IsValid())
		{
			this->OnSessionJoinedDelegateHandle = GameSession->AddOnSessionJoinedDelegate_Handle(this->OnSessionJoinedDelegate);
			if (!GameSession->JoinSession(*Player->GetPreferredUniqueNetId(), this->SessionName, SearchResult))
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to join session"));
			}
		}
	}
}

void UUE4SampleChatGameInstance::OnSessionJoined (const FString& URL, EOnJoinSessionCompleteResult::Type Result)
{
	auto GameSession = this->GetGameSession();

	if (GameSession)
	{
		GameSession->ClearOnSessionJoinedDelegate_Handle(this->OnSessionJoinedDelegateHandle);
	}

	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		auto PlayerController = this->GetFirstLocalPlayerController();

		if (PlayerController && !URL.IsEmpty())
		{
			PlayerController->ClientTravel(URL, ETravelType::TRAVEL_Absolute);
		}
	}
}

void UUE4SampleChatGameInstance::OnDestroySessionComplete (FName SessionName, bool bWasSuccessful)
{
	auto SessionManager = Online::GetSessionInterface();

	if (SessionManager.IsValid())
	{
		SessionManager->ClearOnDestroySessionCompleteDelegate_Handle(this->OnDestroySessionCompleteDelegateHandle);
	}

	if (bWasSuccessful)
	{
		auto PlayerController = this->GetFirstLocalPlayerController();

		if (PlayerController)
		{
			PlayerController->ClientTravel(this->MainLevel, ETravelType::TRAVEL_Absolute);
		}
	}
}

void UUE4SampleChatGameInstance::ShowMenuHelper (TSubclassOf<UUserWidget> Menu)
{
	if (Menu)
	{
		this->CloseCurrentMenu();

		this->CurrentMenu = CreateWidget<UUserWidget>(this->GetWorld(), Menu);
		if (this->CurrentMenu)
		{
			this->CurrentMenu->AddToViewport();
		}
	}
}

void UUE4SampleChatGameInstance::CloseCurrentMenu ()
{
	if (this->CurrentMenu && this->CurrentMenu->IsInViewport())
	{
		this->CurrentMenu->RemoveFromViewport();
		this->CurrentMenu = nullptr;
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
