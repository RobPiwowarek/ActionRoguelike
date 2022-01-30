// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "GameFramework/Actor.h"
#include "SDashProjectile.generated.h"

class URadialForceComponent;
class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystemComponent;

UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASDashProjectile();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float Teleport_Delay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float Detonate_Delay;

	FTimerHandle TimerHandle_DelayedDetonate;

	void Teleport_TimeElapsed();

	virtual void Explode_Implementation() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
