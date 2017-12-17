// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UE4SampleChatPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UE4SAMPLECHAT_API AUE4SampleChatPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/**
	 * @brief Called after successful join, sets up player nickname
	 */
	UFUNCTION(Client, Reliable)
	void ClientChatJoined ();
	void ClientChatJoined_Implementation ();

	/**
	 * @brief Set player nickname
	 * @param Nickname Given nickname
	 */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetPlayerNickname (const FString& Nickname);
	void ServerSetPlayerNickname_Implementation (const FString& Nickname);
	bool ServerSetPlayerNickname_Validate (const FString& Nickname);

	UFUNCTION(NetMulticast, Reliable)
	void BroadcastUpdateChatRoom (const TArray<FString>& Nicknames);
	void BroadcastUpdateChatRoom_Implementation (const TArray<FString>& Nicknames);

protected:
	virtual void BeginPlay () override;
};
