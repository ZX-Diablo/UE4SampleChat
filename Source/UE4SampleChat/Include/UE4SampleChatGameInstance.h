// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Include/UE4SampleChatGameSession.h"
#include "Engine/GameInstance.h"
#include "UE4SampleChatGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE4SAMPLECHAT_API UUE4SampleChatGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UUE4SampleChatGameInstance ();

public:
	/**
	 * Start new chat session as host
	 * @param Nickname Player nickname
	 */
	UFUNCTION(BlueprintCallable, Category = "Network")
	void HostChat (const FString& Nickname);

	/**
	 * Join existing chat session
	 * @param Nickname Player nickname
	 */
	UFUNCTION(BlueprintCallable, Category = "Network")
	void JoinChat (const FString& Nickname);

	/**
	 * Disconnect current chat session
	 */
	UFUNCTION(BlueprintCallable, Category = "Network")
	void DisconnectChat ();

	/**
	 * Show main menu
	 */
	UFUNCTION(BlueprintCallable, Category = "Interface")
	void ShowMainMenu ();

	/**
	 * Show chat menu
	 */
	UFUNCTION(BlueprintCallable, Category = "Interface")
	void ShowChatMenu ();

public:
	/**
	 * Get player nickname stored before host/join
	 * (Game instance is used as temporary storage of local player settings)
	 * @return Player nickname
	 */
	const FString& GetStoredNickname () const;

protected:
	/** Main menu widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interface")
	TSubclassOf<UUserWidget> MainMenu;

	/** Chat menu widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interface")
	TSubclassOf<UUserWidget> ChatMenu;

	/** Current active menu */
	UPROPERTY(BlueprintReadOnly, Category = "Interface")
	UUserWidget* CurrentMenu;

	/** Maximum clients allowed in one room constant */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Network")
	int32 MaxClients;

	/** Chat session name constant */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Network")
	FName SessionName;

	/** Main level name constant */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Network")
	FString MainLevel;

	/** Chat level name constant */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Network")
	FString ChatLevel;

private:
	void OnSessionReady (FName SessionName, bool bWasSuccessful);
	void OnSessionFound (const FOnlineSessionSearchResult& SearchResult, bool bWasSuccessful);
	void OnSessionJoined (const FString& URL, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete (FName SessionName, bool bWasSuccessful);

	void ShowMenuHelper (TSubclassOf<UUserWidget> Menu);
	void CloseCurrentMenu ();

	AUE4SampleChatGameSession* GetGameSession () const;

private:
	AUE4SampleChatGameSession::FOnSessionReadyDelegate OnSessionReadyDelegate;
	AUE4SampleChatGameSession::FOnSessionFoundDelegate OnSessionFoundDelegate;
	AUE4SampleChatGameSession::FOnSessionJoinedDelegate OnSessionJoinedDelegate;
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	FDelegateHandle OnSessionReadyDelegateHandle;
	FDelegateHandle OnSessionFoundDelegateHandle;
	FDelegateHandle OnSessionJoinedDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

private:
	FString Nickname;
};
