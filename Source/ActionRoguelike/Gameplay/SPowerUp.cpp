// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUp.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"


ASPowerUp::ASPowerUp()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	// Disable collision, instead we use SphereComp to handle interaction queries
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	RespawnTime = 10.0f;
	bIsActive = true;

	SetReplicates(true);
}


void ASPowerUp::Interact_Implementation(APawn* InstigatorPawn)
{
	// logic in derived classes...
}


FText ASPowerUp::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}


void ASPowerUp::ShowPowerup()
{
	SetPowerupState(true);
}


void ASPowerUp::HideAndCooldownPowerup()
{
	SetPowerupState(false);

	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPowerUp::ShowPowerup, RespawnTime);
}

void ASPowerUp::SetPowerupState(bool bNewIsActive)
{
	bIsActive = bNewIsActive;
}