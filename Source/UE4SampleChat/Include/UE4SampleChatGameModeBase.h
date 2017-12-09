// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

private:
	void ShowMenuHelper (TSubclassOf<UUserWidget> Menu);
	void CloseCurrentMenu ();

private:
	UPROPERTY()
	UUserWidget* CurrentMenu;
};
