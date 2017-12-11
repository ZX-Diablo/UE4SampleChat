// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Online.h"
#include "GameFramework/GameSession.h"
#include "UE4SampleChatGameSession.generated.h"

/**
 * 
 */
UCLASS()
class UE4SAMPLECHAT_API AUE4SampleChatGameSession : public AGameSession
{
	GENERATED_BODY()
	
public:
	AUE4SampleChatGameSession ();

public:
	/**
	 * @brief Create new session
	 * @param UserId User ID
	 * @param SessionName Session name
	 * @param MaxClients Maximum public clients allowed
	 * @return Success flag
	 */
	bool HostSession (const FUniqueNetId& UserId, FName SessionName, int32 MaxClients);

	/**
	 * @brief Find session to join
	 * @param UserId Searching user ID
	 * @return Success flag
	 */
	bool FindSession (const FUniqueNetId& UserId);

public:
	/**
	 * @brief Event triggered when session is created and started
	 */
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSessionReady, FName, bool);
	typedef FOnSessionReady::FDelegate FOnSessionReadyDelegate;

	/**
	 * @brief Delegate fired when a session is created and started
	 * @param SessionName Session name
	 * @param bWasSuccessful Success flag
	 */
	DEFINE_ONLINE_DELEGATE_TWO_PARAM(OnSessionReady, FName, bool);

private:
	void OnCreateSessionComplete (FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete (FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete (bool bWasSuccessful);

private:
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;
};
