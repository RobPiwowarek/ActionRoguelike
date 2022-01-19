// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRoguelike/Gameplay/Public/SInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "ActionRoguelike/Gameplay/Public/SGameplayInterface.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams Params;
	Params.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* Owner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);

	TArray<FHitResult> Hits;
	// FHitResult Result;
	// bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Result, EyeLocation, End, Params);

	// if (Result.GetActor())
	// {
	// 	if (Result.GetActor()->Implements<USGameplayInterface>())
	// 	{
	// 		ISGameplayInterface::Execute_Interact(Result.GetActor(), Cast<APawn>(Owner));
	// 	}
	// }

	float Radius = 30.0f;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, Params, Shape);

	for (FHitResult Hit : Hits)
	{
		if (Hit.GetActor())
		{
			if (Hit.GetActor()->Implements<USGameplayInterface>())
			{
				ISGameplayInterface::Execute_Interact(Hit.GetActor(), Cast<APawn>(Owner));
				break;
			}
		}
	}
	
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	
	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 10);
}

