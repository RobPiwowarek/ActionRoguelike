// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void USAction_ProjectileAttack::Attack_TimeElapsed(ACharacter* Instigator)
{
	if (ensureAlways(ProjectileClass))
	{
		FVector HandLocation = Instigator->GetMesh()->GetSocketLocation("Muzzle_01");

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = Instigator;

		// calc correction for shoot
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Instigator);

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		FVector TraceStart = Instigator->GetPawnViewLocation();
		FVector TraceEnd = TraceStart + Instigator->GetControlRotation().Vector() * 5000;

		FHitResult Result;
		if (GetWorld()->SweepSingleByObjectType(Result, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, Shape,
		                                        QueryParams))
		{
			TraceEnd = Result.ImpactPoint;
		}

		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 5, 0, 5.0f);

		FRotator ProjectileRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
		// calc correction for shoot

		FTransform SpawnTM = FTransform(ProjectileRotation, HandLocation);

		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}

	StopAction(Instigator);
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);

		UGameplayStatics::SpawnEmitterAttached(CastingParticleEffect, Character->GetMesh(), HandSocketName,
		                                       FVector::ZeroVector, FRotator::ZeroRotator,
		                                       EAttachLocation::SnapToTarget);

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "Attack_TimeElapsed", Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackDelay, false);
	}
}


USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackDelay = 0.2f;
}
