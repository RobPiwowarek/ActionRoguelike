// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "SGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(
	TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global damage multiplier for attribute component"), ECVF_Cheat);

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	Health = 100;
	MaxHealth = Health;
}


// Called when the game starts
void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool USAttributeComponent::ApplyHealthChange(AActor* Instigator, float Delta)
{
	AActor* Actor = GetOwner();

	if (!Actor->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	const float OldHealth = Health;
	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamageMultiplier;
	}

	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	OnHealthChanged.Broadcast(Instigator, this, Health, Health - OldHealth);

	if (Health - OldHealth < 0.0f && Health == 0.0f)
	{
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();

		if (GM)
		{
			GM->OnActorKilled(GetOwner(), Instigator);
		}
	}

	return true;
}


// Called every frame
void USAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(
		Actor->GetComponentByClass(USAttributeComponent::StaticClass()));

	if (AttributeComponent)
	{
		return AttributeComponent->IsAlive();
	}

	return false;
}

bool USAttributeComponent::Kill(AActor* Instigator)
{
	return ApplyHealthChange(Instigator, -GetMaxHealth());
}

bool USAttributeComponent::IsFullHealth()
{
	return Health == MaxHealth;
}
