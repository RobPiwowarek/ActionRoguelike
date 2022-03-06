// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAiCharacter.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "ActionRoguelike/Gameplay/SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ASAiCharacter::ASAiCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
}

void ASAiCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAiCharacter::OnPawnSeen);
}

void ASAiCharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIController = Cast<AAIController>(GetController());

	if (AIController)
	{
		UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();

		BlackboardComponent->SetValueAsObject("TargetActor", Pawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
	}
}
