// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Public/SGameplayInterface.h"
#include "SPowerUp.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASPowerUp : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASPowerUp();

	// Called when the game starts or when spawned
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	void Cooldown_TimeElapsed();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float Cooldown = 10.0f;

	FTimerHandle TimerHandle_CooldownElapsed;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float AmountRestored = 20.0f;

	bool OnCooldown;
};
