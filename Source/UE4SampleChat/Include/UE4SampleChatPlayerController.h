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

	/**
	 * Receive and display new message on client
	 * @param Date Message date
	 * @param Nickname Sender nickname
	 * @param Message Player message
	 */
	UFUNCTION(Client, Reliable)
	void ClientReceiveNewMessage (const FString& Date, const FString& Nickname, const FText& Message);
	void ClientReceiveNewMessage_Implementation (const FString& Date, const FString& Nickname, const FText& Message);

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
	 * Called after client enters (post login) or leaves (logout) chat room, updates chat room on all clients
	 */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUpdateChatRoom ();
	void ServerUpdateChatRoom_Implementation ();
	bool ServerUpdateChatRoom_Validate ();

	/**
	 * Called after chat level load, client requests update chat room from server
	 * (can't rely on post login due to asynchrony)
	 */
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Network")
	void ServerClientRequestUpdateChatRoom ();
	void ServerClientRequestUpdateChatRoom_Implementation ();
	bool ServerClientRequestUpdateChatRoom_Validate ();

	/**
	 * Client sends new message to server
	 * @param Message Player message
	 */
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Network")
	void ServerSendMessage (const FText& Message);
	void ServerSendMessage_Implementation (const FText& Message);
	bool ServerSendMessage_Validate (const FText& Message);

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

	/**
	 * Add new message into chat log
	 * @param Date Message date
	 * @param Nickname Sender nickname
	 * @param Message Player message
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Interface|Chat log")
	void AddChatLogEntry (const FString& Date, const FString& Nickname, const FText& Message);

protected:
	virtual void BeginPlay () override;

private:
	TArray<FString> GetNicknameArray () const;
};
