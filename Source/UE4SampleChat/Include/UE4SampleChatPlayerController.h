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
	 * Called after successful join, sets up player nickname
	 */
	UFUNCTION(Client, Reliable)
	void ClientChatJoined ();
	void ClientChatJoined_Implementation ();

	/**
	 * Update client chat room
	 * @param NicknameArray List of users in room
	 */
	UFUNCTION(Client, Reliable)
	void ClientUpdateChatRoom (const TArray<FString>& NicknameArray);
	void ClientUpdateChatRoom_Implementation (const TArray<FString>& NicknameArray);

public:
	/**
	 * Set player nickname
	 * @param Nickname Given nickname
	 */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetPlayerNickname (const FString& Nickname);
	void ServerSetPlayerNickname_Implementation (const FString& Nickname);
	bool ServerSetPlayerNickname_Validate (const FString& Nickname);

	/**
	 * Called after chat level load, client requests update chat room from server
	 * (can't rely on broadcast due to asynchrony)
	 */
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Network")
	void ServerClientRequestUpdateChatRoom ();
	void ServerClientRequestUpdateChatRoom_Implementation ();
	bool ServerClientRequestUpdateChatRoom_Validate ();

public:
	/**
	 * Tell all clients to update chat rooms after new client joins
	 * @param NicknameArray List of users in room
	 */
	UFUNCTION(NetMulticast, Reliable)
	void BroadcastUpdateChatRoom (const TArray<FString>& NicknameArray);
	void BroadcastUpdateChatRoom_Implementation (const TArray<FString>& NicknameArray);

public:
	/**
	 * Clear chat room widget
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Interface|Chat room")
	void ClearChatRoom ();

	/**
	 * Add new nickname entry into chat room
	 * @param Nickname Nickname to add
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Interface|Chat room")
	void AddChatRoomEntry (const FString& Nickname);

protected:
	virtual void BeginPlay () override;

private:
	void UpdateChatRoom (const TArray<FString>& NicknameArray);

	TArray<FString> GetNicknameArray () const;
};
