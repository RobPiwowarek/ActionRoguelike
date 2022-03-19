// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingParticleEffect;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackDelay = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FName HandSocketName;

	UFUNCTION()
	void Attack_TimeElapsed(ACharacter* Instigator);

public:
	virtual void StartAction_Implementation(AActor* Instigator) override;

	USAction_ProjectileAttack();
};
