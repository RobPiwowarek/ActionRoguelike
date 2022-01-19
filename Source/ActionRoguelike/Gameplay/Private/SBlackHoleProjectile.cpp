// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRoguelike/Gameplay/Public/SBlackHoleProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASBlackHoleProjectile::ASBlackHoleProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	BlackHoleComp = CreateDefaultSubobject<USphereComponent>("BlackHoleComp");
	BlackHoleComp->SetCollisionProfileName("Projectile");
	BlackHoleComp->SetupAttachment(SphereComp);

	if (!BlackHoleComp->OnComponentHit.IsBound())
	{
		BlackHoleComp->OnComponentHit.AddDynamic(this, &ASBlackHoleProjectile::OnHit);
	}

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForceComp->Radius = 2000.0;
	RadialForceComp->ForceStrength = -1000000.0;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->SetAutoActivate(true);
	RadialForceComp->AddCollisionChannelToAffect(ECC_PhysicsBody);
	RadialForceComp->SetupAttachment(SphereComp);
}

void ASBlackHoleProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                  FVector NormalImpulse, const FHitResult& Hit)
{
	
	if (OtherActor && OtherActor != GetInstigator())
	{
		OtherActor->Destroy();
	}
}


// Called when the game starts or when spawned
void ASBlackHoleProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASBlackHoleProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
