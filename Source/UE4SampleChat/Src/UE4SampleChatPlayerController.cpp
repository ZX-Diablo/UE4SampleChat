// Fill out your copyright notice in the Description page of Project Settings.

#include "Include/UE4SampleChatPlayerController.h"

void AUE4SampleChatPlayerController::BeginPlay ()
{
	Super::BeginPlay();

	this->SetInputMode(FInputModeUIOnly());
}
