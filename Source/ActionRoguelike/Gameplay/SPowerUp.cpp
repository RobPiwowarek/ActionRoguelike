// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUp.h"

#include "SAttributeComponent.h"
#include "Public/SCharacter.h"

// Sets default values
ASPowerUp::ASPowerUp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;
}

// Called when the game starts or when spawned
void ASPowerUp::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASPowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASPowerUp::Interact_Implementation(APawn* InstigatorPawn)
{
	if (OnCooldown) return;

	if (InstigatorPawn)
	{
		ASCharacter* PlayerCharacter = Cast<ASCharacter>(InstigatorPawn);

		if (PlayerCharacter)
		{
			if (PlayerCharacter->IsAtFullHealth())
			{
				return;
			}
			else
			{
				USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(
					PlayerCharacter->GetComponentByClass(USAttributeComponent::StaticClass()));

				if (AttributeComponent)
				{
					AttributeComponent->ApplyHealthChange(nullptr, AmountRestored);
					SetActorHiddenInGame(true);
					OnCooldown = true;
					GetWorldTimerManager().SetTimer(TimerHandle_CooldownElapsed, this, &ASPowerUp::Cooldown_TimeElapsed,
					                                Cooldown);
				}
			}
		}
	}
}

void ASPowerUp::Cooldown_TimeElapsed()
{
	SetActorHiddenInGame(false);
	OnCooldown = false;
}
