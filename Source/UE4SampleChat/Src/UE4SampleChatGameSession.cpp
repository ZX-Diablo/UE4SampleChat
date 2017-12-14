// Fill out your copyright notice in the Description page of Project Settings.

#include "Include/UE4SampleChatGameSession.h"

AUE4SampleChatGameSession::AUE4SampleChatGameSession ()
{
	this->OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &AUE4SampleChatGameSession::OnCreateSessionComplete);
	this->OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &AUE4SampleChatGameSession::OnStartSessionComplete);
	this->OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &AUE4SampleChatGameSession::OnFindSessionsComplete);
	this->OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &AUE4SampleChatGameSession::OnJoinSessionComplete);
}

bool AUE4SampleChatGameSession::HostSession (const FUniqueNetId& UserId, FName SessionName, int32 MaxClients)
{
	auto SessionManager = Online::GetSessionInterface();

	if (SessionManager.IsValid())
	{
		FOnlineSessionSettings Settings;

		Settings.NumPublicConnections = MaxClients;
		Settings.NumPrivateConnections = 0;
		Settings.bShouldAdvertise = true;
		Settings.bAllowJoinInProgress = true;
		Settings.bIsLANMatch = true;
		Settings.bIsDedicated = false;
		Settings.bUsesStats = false;
		Settings.bAllowInvites = false;
		Settings.bUsesPresence = false;
		Settings.bAllowJoinViaPresence = false;
		Settings.bAllowJoinViaPresenceFriendsOnly = false;
		Settings.bAntiCheatProtected = false;

		this->OnCreateSessionCompleteDelegateHandle = SessionManager->AddOnCreateSessionCompleteDelegate_Handle(this->OnCreateSessionCompleteDelegate);

		SessionManager->DestroySession(SessionName);
		return SessionManager->CreateSession(UserId, SessionName, Settings);
	}

	return false;
}

bool AUE4SampleChatGameSession::FindSession (const FUniqueNetId& UserId)
{
	auto SessionManager = Online::GetSessionInterface();

	if (SessionManager.IsValid())
	{
		this->SessionSearch = MakeShared<FOnlineSessionSearch>();
		this->SessionSearch->MaxSearchResults = 1;
		this->SessionSearch->bIsLanQuery = true;
		this->SessionSearch->PingBucketSize = 50;

		this->OnFindSessionsCompleteDelegateHandle = SessionManager->AddOnFindSessionsCompleteDelegate_Handle(this->OnFindSessionsCompleteDelegate);
		return SessionManager->FindSessions(UserId, this->SessionSearch.ToSharedRef());
	}

	return false;
}

bool AUE4SampleChatGameSession::JoinSession (const FUniqueNetId& UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
	auto SessionManager = Online::GetSessionInterface();

	if (SessionManager.IsValid())
	{
		this->OnJoinSessionCompleteDelegateHandle = SessionManager->AddOnJoinSessionCompleteDelegate_Handle(this->OnJoinSessionCompleteDelegate);

		SessionManager->DestroySession(SessionName);
		return SessionManager->JoinSession(UserId, SessionName, SearchResult);
	}

	return false;
}

void AUE4SampleChatGameSession::OnCreateSessionComplete (FName SessionName, bool bWasSuccessful)
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

void AUE4SampleChatGameSession::OnStartSessionComplete (FName SessionName, bool bWasSuccessful)
{
	auto SessionManager = Online::GetSessionInterface();

	if (SessionManager.IsValid())
	{
		SessionManager->ClearOnStartSessionCompleteDelegate_Handle(this->OnStartSessionCompleteDelegateHandle);
	}

	this->TriggerOnSessionReadyDelegates(SessionName, bWasSuccessful);
}

void AUE4SampleChatGameSession::OnFindSessionsComplete (bool bWasSuccessful)
{
	auto SessionManager = Online::GetSessionInterface();

	if (SessionManager.IsValid())
	{
		SessionManager->ClearOnFindSessionsCompleteDelegate_Handle(this->OnFindSessionsCompleteDelegateHandle);
	}

	FOnlineSessionSearchResult SearchResult;

	if (bWasSuccessful && this->SessionSearch.IsValid() && this->SessionSearch->SearchState == EOnlineAsyncTaskState::Done)
	{
		if (this->SessionSearch->SearchResults.Num() > 0)
		{
			SearchResult = this->SessionSearch->SearchResults[0];
		}
	}

	this->TriggerOnSessionFoundDelegates(SearchResult, bWasSuccessful);
}

void AUE4SampleChatGameSession::OnJoinSessionComplete (FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	auto SessionManager = Online::GetSessionInterface();

	if (SessionManager.IsValid())
	{
		SessionManager->ClearOnJoinSessionCompleteDelegate_Handle(this->OnJoinSessionCompleteDelegateHandle);
	}

	this->TriggerOnSessionJoinedDelegates(Result);
}
