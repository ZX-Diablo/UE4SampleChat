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

private:
	void OnCreateSessionComplete (FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete (FName SessionName, bool bWasSuccessful);

private:
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
};
