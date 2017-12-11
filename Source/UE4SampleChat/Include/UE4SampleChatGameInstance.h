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
	* @brief Start new chat session as host
	*/
	UFUNCTION(BlueprintCallable, Category = "Network")
	void HostChat (const FText& Nickname);

	/**
	* @brief Join existing chat session
	*/
	UFUNCTION(BlueprintCallable, Category = "Network")
	void JoinChat (const FText& Nickname);

protected:
	/** Maximum clients allowed in one room constant */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Network")
	int32 MaxClients;

	/** Chat session name constant */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Network")
	FName SessionName;

	/** Chat level name constant */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Network")
	FString ChatLevel;

private:
	void OnSessionReady (FName SessionName, bool bWasSuccessful);

	AUE4SampleChatGameSession* GetGameSession () const;

private:
	AUE4SampleChatGameSession::FOnSessionReadyDelegate OnSessionReadyDelegate;
	FDelegateHandle OnSessionReadyDelegateHandle;
};
