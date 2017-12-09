// Fill out your copyright notice in the Description page of Project Settings.

#include "Include/UE4SampleChatGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "UserWidget.h"

void AUE4SampleChatGameModeBase::ShowMainMenu ()
{
	this->ShowMenuHelper(this->MainMenu);
}

void AUE4SampleChatGameModeBase::ShowChatMenu ()
{
	this->ShowMenuHelper(this->ChatMenu);
}

void AUE4SampleChatGameModeBase::ShowMenuHelper (TSubclassOf<UUserWidget> Menu)
{
	if (Menu)
	{
		this->CloseCurrentMenu();

		this->CurrentMenu = CreateWidget<UUserWidget>(this->GetWorld(), Menu);
		if (this->CurrentMenu)
		{
			this->CurrentMenu->AddToViewport();
		}
	}
}

void AUE4SampleChatGameModeBase::CloseCurrentMenu ()
{
	if (this->CurrentMenu && this->CurrentMenu->IsInViewport())
	{
		this->CurrentMenu->RemoveFromViewport();
		this->CurrentMenu = nullptr;
	}
}
