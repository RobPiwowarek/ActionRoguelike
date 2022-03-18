// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionRoguelike/Gameplay/Public/SMagicProjectile.h"

#include "SGameplayFunctionLibrary.h"
#include "ActionRoguelike/Gameplay/SAttributeComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComponent->SetupAttachment(RootComponent);
}

void ASMagicProjectile::OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		if (ImpactSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, this->GetActorLocation(),
			                                      this->GetActorRotation());
		}

		if (USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, 20.0f, Hit))
		{
			Explode();
		}
	}

	UGameplayStatics::PlayWorldCameraShake(this, CameraShakeClass, GetActorLocation(),  50.0f, 8000.0f, 0.1f);
	
	Super::OnActorHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}


// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (AudioComponent)
	{
		AudioComponent->Play();
	}
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
