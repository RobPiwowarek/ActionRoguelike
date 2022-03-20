// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionRoguelike/Gameplay/Public/SMagicProjectile.h"

#include "SActionComponent.h"
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
	SphereComp->SetSphereRadius(20.0f);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComponent->SetupAttachment(RootComponent);
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor,
                                       UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& Hit)
{
	APawn* aaaInstigator = GetInstigator();

 	if (OtherActor && OtherActor != aaaInstigator)
	{
		USActionComponent* ActionComponent = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if (ActionComponent && ActionComponent->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity = -MovementComp->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
		
		if (ImpactSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, this->GetActorLocation(),
												  this->GetActorRotation());
		}

		if (USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, 20.0f, Hit))
		{
			Explode();

			if (ActionComponent)
			{
				ActionComponent->AddAction(GetInstigator(), BurningActionClass);
			}
		}
	}

	UGameplayStatics::PlayWorldCameraShake(this, CameraShakeClass, GetActorLocation(), 50.0f, 8000.0f, 0.1f);

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
