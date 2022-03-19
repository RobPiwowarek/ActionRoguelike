// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"


void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);

	USActionComponent* Component = GetOwningComponent();
	if (ensure(Component))
	{
		Component->ActiveGameplayTags.RemoveTags(GrantsTags);
	}

	bIsRunning = false;
}

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	USActionComponent* Component = GetOwningComponent();
	if (ensure(Component))
	{
		Component->ActiveGameplayTags.AppendTags(GrantsTags);
	}
	bIsRunning = true;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}

bool USAction::IsRunning() const
{
	return bIsRunning;
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
		return false;

	USActionComponent* Component = GetOwningComponent();
	if (ensure(Component))
	{
		if (Component->ActiveGameplayTags.HasAny(BlockedTags))
		{
			return false;
		}
	}
	return true;
}

UWorld* USAction::GetWorld() const
{
	UActorComponent* Component = Cast<UActorComponent>(GetOuter());
	if (Component)
	{
		return Component->GetWorld();
	}

	return nullptr;
}
