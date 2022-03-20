// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRoguelike/Gameplay/Public/SInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "ActionRoguelike/Gameplay/Public/SGameplayInterface.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(
	TEXT("su.DebugDrawInteraction"), false, TEXT("Disable/Enable debug drawing"), ECVF_Cheat);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	TraceSphereRadius = 30.0f;
	TraceDistance = 500;
	CollisionChannel = ECC_WorldDynamic;
	// ...
}

void USInteractionComponent::FindBestInteractable()
{
	FCollisionObjectQueryParams Params;
	Params.AddObjectTypesToQuery(CollisionChannel);

	AActor* Owner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

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

	FCollisionShape Shape;
	Shape.SetSphere(TraceSphereRadius);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, Params, Shape);

	// clear ref before trying to fill
	FocusedActor = nullptr;

	for (FHitResult Hit : Hits)
	{
		if (Hit.GetActor())
		{
			if (Hit.GetActor()->Implements<USGameplayInterface>())
			{
				FocusedActor = Hit.GetActor();
				break;
			}
		}
	}

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;
			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}

	if (CVarDebugDrawInteraction.GetValueOnGameThread())
	{
		FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 10);
	}
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestInteractable();
	// ...
}


void USInteractionComponent::PrimaryInteract()
{
	if (FocusedActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact.");
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());

	ISGameplayInterface::Execute_Interact(FocusedActor, MyPawn);
}
