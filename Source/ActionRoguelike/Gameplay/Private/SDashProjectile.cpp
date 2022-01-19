// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRoguelike/Gameplay/Public/SDashProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASDashProjectile::ASDashProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	if (!SphereComp->OnComponentHit.IsBound())
	{
		SphereComp->OnComponentHit.AddDynamic(this, &ASDashProjectile::OnHit);
	}
	
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	ExitEffectComp = CreateDefaultSubobject<UParticleSystemComponent>("ExitEffectComp");
	ExitEffectComp->SetAutoActivate(false);
	ExitEffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;

	Teleport_Delay = 0.2f;
}

void ASDashProjectile::OnHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                  FVector NormalImpulse, const FHitResult& Hit)
{
	
	if (!bDestroyed && OtherActor && OtherActor != GetInstigator())
	{
		this->bDestroyed = true;
		GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ASDashProjectile::Teleport_TimeElapsed, Teleport_Delay);
	}
}

void ASDashProjectile::Teleport_TimeElapsed()
{
	GetInstigator()->SetActorLocation(this->GetActorLocation());
	this->MovementComp->StopMovementImmediately();
	UGameplayStatics::SpawnEmitterAtLocation(this, ExitEffectComp->Template, GetActorLocation());
	Destroy();
}

// Called when the game starts or when spawned
void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ASDashProjectile::Teleport_TimeElapsed, Teleport_Delay);
}

// Called every frame
void ASDashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
