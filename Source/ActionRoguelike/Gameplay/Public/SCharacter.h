// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"


class USInteractionComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> PrimaryProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> SecondaryProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> DashProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent * InteractionComponent;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent * CameraComp;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent * SpringArmComp;

	FTimerHandle TimerHandle_PrimaryAttack, TimerHandle_SecondaryAttack, TimerHandle_Dash;
	float AttackDelay = 0.2f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void PrimaryAttack();
	void SecondaryAttack();
	void DashAttack();
	
	void PrimaryAttack_TimeElapsed();
	void SecondaryAttack_TimeElapsed();
	void DashAttack_TimeElapsed();

	void PrimaryInteract();

private:
	void SpawnProjectileByClass(TSubclassOf<AActor> ProjectileClass);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
};