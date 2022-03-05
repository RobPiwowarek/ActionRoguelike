// Fill out your copyright notice in the Description page of Project Settings.

 
#include "AI/SBTT_HealFull.h"

#include "AIController.h"
#include "ActionRoguelike/Gameplay/SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTT_HealFull::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();

	if (ensure(MyController))
	{
		ACharacter* Pawn = Cast<ACharacter>(MyController->GetPawn());
		if (Pawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(Pawn->GetComponentByClass(USAttributeComponent::StaticClass()));

		if (ensure(AttributeComponent))
		{
			AttributeComponent->ApplyHealthChange(AttributeComponent->GetMaxHealth());
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
