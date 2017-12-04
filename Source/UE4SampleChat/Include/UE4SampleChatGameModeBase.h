// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Online.h"
#include "GameFramework/GameModeBase.h"
#include "UE4SampleChatGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UE4SAMPLECHAT_API AUE4SampleChatGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUE4SampleChatGameModeBase ();

public:
	/**
	 * @brief Start new chat session as host
	 */
	UFUNCTION(BlueprintCallable, Category = "Network")
	void ServerHostChat (const FText& Nickname);

	/**
	 * @brief Join existing chat session
	 */
	UFUNCTION(BlueprintCallable, Category = "Network")
	void ServerJoinChat (const FText& Nickname);

	/**
	 * @brief Show main menu
	 */
	UFUNCTION(BlueprintCallable, Category = "Interface")
	void ShowMainMenu ();

	/**
	 * @brief Show chat menu
	 */
	UFUNCTION(BlueprintCallable, Category = "Interface")
	void ShowChatMenu ();

protected:
	/** Main menu widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interface")
	TSubclassOf<UUserWidget> MainMenu;

	/** Chat menu widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interface")
	TSubclassOf<UUserWidget> ChatMenu;

protected:
	/** Maximum clients allowed in one room constant */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Network")
	int32 MaxClients;

	/** Chat session name constant */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Network")
	FName SessionName;

	/** Chat level name constant */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Network")
	FName ChatLevel;

private:
	void OnCreateSessionComplete (FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete (FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete (bool bWasSuccessful);

	void ShowMenuHelper (TSubclassOf<UUserWidget> Menu);
	void CloseCurrentMenu ();

private:
	UPROPERTY()
	UUserWidget* CurrentMenu;

private:
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;
};
