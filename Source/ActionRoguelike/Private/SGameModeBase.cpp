// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "ActionRoguelike/Gameplay/SAttributeComponent.h"
#include "ActionRoguelike/Gameplay/Public/SCharacter.h"
#include "AI/SAiCharacter.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."),
                                                ECVF_Cheat);

void ASGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via console command"))
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* EnvQueryInstance = UEnvQueryManager::RunEQSQuery(
		this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAiCharacter> It(GetWorld()); It; ++It)
	{
		ASAiCharacter* Bot = *It;

		USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(
			Bot->GetComponentByClass(USAttributeComponent::StaticClass()));

		if (ensure(AttributeComponent) && AttributeComponent->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	float MaxBotCount = 1.0f;

	if (NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity, Skipping spawn"));
		return;
	}

	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NrOfAliveBots >= MaxBotCount)
	{
		return;
	}

	if (ensure(EnvQueryInstance))
	{
		EnvQueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                      EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query failed"))
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);

	if (Player != nullptr)
	{
		// because of network, it would override the timer handle if it was stored in header file
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor),
	       *GetNameSafe(Killer));
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed,
	                                SpawnTimerInterval, true);
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAiCharacter> It(GetWorld()); It; ++It)
	{
		ASAiCharacter* Bot = *It;

		USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(
			Bot->GetComponentByClass(USAttributeComponent::StaticClass()));

		if (ensure(AttributeComponent) && AttributeComponent->IsAlive())
		{
			// todo: pass in player for kill credit
			AttributeComponent->Kill(this);
		}
	}
}
