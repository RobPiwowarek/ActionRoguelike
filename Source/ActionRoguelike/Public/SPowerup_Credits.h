// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRoguelike/Gameplay/SPowerUp.h"
#include "SPowerup_Credits.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASPowerup_Credits : public ASPowerUp
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 CreditsAmount;

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;

	ASPowerup_Credits();
};