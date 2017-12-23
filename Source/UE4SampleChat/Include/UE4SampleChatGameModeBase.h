// Copyright ZX.Diablo 2017

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
	virtual void PostLogin (APlayerController* NewPlayer) override;
	virtual void Logout (AController* Exiting) override;
};
