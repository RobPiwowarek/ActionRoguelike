// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBlackHoleProjectile.generated.h"

class URadialForceComponent;
class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystemComponent;

UCLASS()
class ACTIONROGUELIKE_API ASBlackHoleProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASBlackHoleProjectile();
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* BlackHoleComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent * MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent * EffectComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URadialForceComponent *RadialForceComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EObjectTypeQuery> ObjectTypeToIgnore;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
