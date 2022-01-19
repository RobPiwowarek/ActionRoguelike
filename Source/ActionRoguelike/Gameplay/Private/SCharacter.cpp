// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRoguelike/Gameplay/Public/SCharacter.h"

#include "DrawDebugHelpers.h"
#include "ActionRoguelike/Gameplay/Public/SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
	
	GetCharacterMovement()->bOrientRotationToMovement = true;

	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>("InteractionComponent");
	
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();

	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	
	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();

	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = UKismetMathLibrary::GetRightVector(ControlRot);
	
	AddMovementInput(RightVector, Value);
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, AttackDelay);
}

void ASCharacter::SecondaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_SecondaryAttack, this, &ASCharacter::SecondaryAttack_TimeElapsed, AttackDelay);
}

void ASCharacter::DashAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_Dash, this, &ASCharacter::DashAttack_TimeElapsed, AttackDelay);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectileByClass(PrimaryProjectileClass);
}

void ASCharacter::SecondaryAttack_TimeElapsed()
{
	SpawnProjectileByClass(SecondaryProjectileClass);
}

void ASCharacter::DashAttack_TimeElapsed()
{
	SpawnProjectileByClass(DashProjectileClass);
}

void ASCharacter::SpawnProjectileByClass(TSubclassOf<AActor> ProjectileClass)
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	
	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	// calc correction for shoot
	FCollisionObjectQueryParams Params;
	Params.AddObjectTypesToQuery(ECC_Visibility);

	FCollisionQueryParams AdditionalParams;
	AdditionalParams.AddIgnoredActor(this);

	FVector End = CameraComp->GetComponentLocation() + CameraComp->GetComponentRotation().Vector() * 3000;
	FHitResult Result;
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Result, CameraComp->GetComponentLocation(), End, Params, AdditionalParams);
	FRotator FindLookAtRotation;

	DrawDebugLine(GetWorld(), CameraComp->GetComponentLocation(), End, FColor::Blue, false, 5);
	if (bBlockingHit) {
		FindLookAtRotation = UKismetMathLibrary::FindLookAtRotation(SpawnTM.GetLocation(), Result.Location);
	} else
	{
		FindLookAtRotation = UKismetMathLibrary::FindLookAtRotation(SpawnTM.GetLocation(), Result.TraceEnd);
	}
	// calc correction for shoot

	SpawnTM = FTransform(FindLookAtRotation, HandLocation);
	
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}

void ASCharacter::PrimaryInteract()
{
	InteractionComponent->PrimaryInteract();
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ASCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("DashAttack", IE_Pressed, this, &ASCharacter::DashAttack);
	
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
}

