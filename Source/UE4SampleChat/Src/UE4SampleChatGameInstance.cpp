// Fill out your copyright notice in the Description page of Project Settings.

#include "Include/UE4SampleChatGameInstance.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UUE4SampleChatGameInstance::UUE4SampleChatGameInstance()
{
	this->MaxClients = 32;
	this->SessionName = TEXT("Game");
	this->ChatLevel = TEXT("ChatLevel");

	this->OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UUE4SampleChatGameInstance::OnCreateSessionComplete);
	this->OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UUE4SampleChatGameInstance::OnStartSessionComplete);
	this->OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UUE4SampleChatGameInstance::OnFindSessionsComplete);
}

void UUE4SampleChatGameInstance::HostChat (const FText& Nickname)
{
	auto SessionManager = Online::GetSessionInterface();
	auto IdentityManager = Online::GetIdentityInterface();

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

void UUE4SampleChatGameInstance::JoinChat (const FText & Nickname)
{
	auto SessionManager = Online::GetSessionInterface();
	auto IdentityManager = Online::GetIdentityInterface();

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

void UUE4SampleChatGameInstance::OnCreateSessionComplete (FName SessionName, bool bWasSuccessful)
{
	auto SessionManager = Online::GetSessionInterface();

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

void UUE4SampleChatGameInstance::OnStartSessionComplete (FName SessionName, bool bWasSuccessful)
{
	auto SessionManager = Online::GetSessionInterface();

	if (SessionManager.IsValid())
	{
		SessionManager->ClearOnStartSessionCompleteDelegate_Handle(this->OnStartSessionCompleteDelegateHandle);
	}

	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(this->GetWorld(), this->ChatLevel, true, "listen");
	}
}

void UUE4SampleChatGameInstance::OnFindSessionsComplete (bool bWasSuccessful)
{
	auto SessionManager = Online::GetSessionInterface();

	if (SessionManager.IsValid())
	{
		SessionManager->ClearOnFindSessionsCompleteDelegate_Handle(this->OnFindSessionsCompleteDelegateHandle);
	}

	if (bWasSuccessful && this->SessionSearch.IsValid() && this->SessionSearch->SearchState == EOnlineAsyncTaskState::Done)
	{
		if (this->SessionSearch->SearchResults.Num() > 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Find session `%s`"), *this->SessionSearch->SearchResults[0].Session.OwningUserName);
		}
	}
}
