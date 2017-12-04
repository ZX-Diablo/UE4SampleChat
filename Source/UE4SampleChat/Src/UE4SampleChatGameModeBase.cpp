// Fill out your copyright notice in the Description page of Project Settings.

#include "Include/UE4SampleChatGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "UserWidget.h"

AUE4SampleChatGameModeBase::AUE4SampleChatGameModeBase ()
{
	this->MaxClients = 32;
	this->SessionName = TEXT("SampleChatSession");
	this->ChatLevel = TEXT("ChatLevel");

	this->OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &AUE4SampleChatGameModeBase::OnCreateSessionComplete);
	this->OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &AUE4SampleChatGameModeBase::OnStartSessionComplete);
	this->OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &AUE4SampleChatGameModeBase::OnFindSessionsComplete);
}

void AUE4SampleChatGameModeBase::ServerHostChat (const FText& Nickname)
{
	auto Online = IOnlineSubsystem::Get();

	if (Online)
	{
		auto SessionManager = Online->GetSessionInterface();
		auto IdentityManager = Online->GetIdentityInterface();

		if (SessionManager.IsValid() && IdentityManager.IsValid())
		{
			FOnlineSessionSettings Settings;

			Settings.NumPublicConnections = this->MaxClients;
			Settings.NumPrivateConnections = 0;
			Settings.bShouldAdvertise = true;
			Settings.bAllowJoinInProgress = true;
			Settings.bIsLANMatch = true;
			Settings.bIsDedicated = false;
			Settings.bUsesStats = false;
			Settings.bAllowInvites = false;
			Settings.bUsesPresence = true;
			Settings.bAllowJoinViaPresence = false;
			Settings.bAllowJoinViaPresenceFriendsOnly = false;
			Settings.bAntiCheatProtected = false;

			SessionManager->DestroySession(this->SessionName);

			this->OnCreateSessionCompleteDelegateHandle = SessionManager->AddOnCreateSessionCompleteDelegate_Handle(this->OnCreateSessionCompleteDelegate);
			if (!SessionManager->CreateSession(*IdentityManager->GetUniquePlayerId(0), this->SessionName, Settings))
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to create session"));
			}
		}
	}
}

void AUE4SampleChatGameModeBase::ServerJoinChat (const FText & Nickname)
{
	auto Online = IOnlineSubsystem::Get();

	if (Online)
	{
		auto SessionManager = Online->GetSessionInterface();
		auto IdentityManager = Online->GetIdentityInterface();

		if (SessionManager.IsValid() && IdentityManager.IsValid())
		{
			this->SessionSearch = MakeShared<FOnlineSessionSearch>();
			this->SessionSearch->MaxSearchResults = 1;
			this->SessionSearch->bIsLanQuery = true;

			this->OnFindSessionsCompleteDelegateHandle = SessionManager->AddOnFindSessionsCompleteDelegate_Handle(this->OnFindSessionsCompleteDelegate);
			if (!SessionManager->FindSessions(*IdentityManager->GetUniquePlayerId(0), this->SessionSearch.ToSharedRef()))
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to find sessions"));
			}
		}
	}
}

void AUE4SampleChatGameModeBase::ShowMainMenu ()
{
	this->ShowMenuHelper(this->MainMenu);
}

void AUE4SampleChatGameModeBase::ShowChatMenu ()
{
	this->ShowMenuHelper(this->ChatMenu);
}

void AUE4SampleChatGameModeBase::OnCreateSessionComplete (FName SessionName, bool bWasSuccessful)
{
	auto Online = IOnlineSubsystem::Get();

	if (Online)
	{
		auto SessionManager = Online->GetSessionInterface();

		if (SessionManager.IsValid())
		{
			SessionManager->ClearOnCreateSessionCompleteDelegate_Handle(this->OnCreateSessionCompleteDelegateHandle);

			if (bWasSuccessful)
			{
				this->OnStartSessionCompleteDelegateHandle = SessionManager->AddOnStartSessionCompleteDelegate_Handle(this->OnStartSessionCompleteDelegate);
				SessionManager->StartSession(SessionName);
			}
		}
	}
}

void AUE4SampleChatGameModeBase::OnStartSessionComplete (FName SessionName, bool bWasSuccessful)
{
	auto Online = IOnlineSubsystem::Get();

	if (Online)
	{
		auto SessionManager = Online->GetSessionInterface();

		if (SessionManager.IsValid())
		{
			SessionManager->ClearOnStartSessionCompleteDelegate_Handle(this->OnStartSessionCompleteDelegateHandle);
		}
	}

	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(this->GetWorld(), this->ChatLevel, true, "listen");
	}
}

void AUE4SampleChatGameModeBase::OnFindSessionsComplete (bool bWasSuccessful)
{
	auto Online = IOnlineSubsystem::Get();

	if (Online)
	{
		auto SessionManager = Online->GetSessionInterface();

		if (SessionManager.IsValid())
		{
			SessionManager->ClearOnFindSessionsCompleteDelegate_Handle(this->OnFindSessionsCompleteDelegateHandle);
		}
	}

	if (bWasSuccessful && this->SessionSearch.IsValid() && this->SessionSearch->SearchState == EOnlineAsyncTaskState::Done)
	{
		if (this->SessionSearch->SearchResults.Num() > 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Find session `%s`"), *this->SessionSearch->SearchResults[0].Session.OwningUserName);
		}
	}
}

void AUE4SampleChatGameModeBase::ShowMenuHelper (TSubclassOf<UUserWidget> Menu)
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

void AUE4SampleChatGameModeBase::CloseCurrentMenu ()
{
	if (this->CurrentMenu && this->CurrentMenu->IsInViewport())
	{
		this->CurrentMenu->RemoveFromViewport();
		this->CurrentMenu = nullptr;
	}
}
