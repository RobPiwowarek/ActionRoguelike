// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckLowHealth.h"

#include "AIController.h"
#include "ActionRoguelike/Gameplay/SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	if (ensure(BlackboardComponent))
	{
		AAIController* TargetController = OwnerComp.GetAIOwner();
		if (ensure(TargetController))
		{
			APawn* Pawn = TargetController->GetPawn();
			if (ensure(Pawn))
			{
				
				USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(Pawn->GetComponentByClass(USAttributeComponent::StaticClass()));

				if (ensure(AttributeComponent))
				{
					if (AttributeComponent->GetHealth() < AttributeComponent->GetMaxHealth() * 0.5)
					{
						BlackboardComponent->SetValueAsBool(LowHealthKey.SelectedKeyName, true);
					}
					else
					{
						// probably redundant and that thing could be set in BTTask but at the same time its better to provide
						// some kind of logical closure in a single place and not to worry about it in other cases.
						BlackboardComponent->SetValueAsBool(LowHealthKey.SelectedKeyName, false);
					}
				}
			}
		}
	}
}
