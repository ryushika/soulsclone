// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/SummonEnemies.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModes/MLGameMode.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Characters/EnemyCharacter.h"
#include "AI/EnemyAIController.h"
#include "Actors/RoomTrigger.h"
#include "NavigationSystem.h"

void USummonEnemies::SpawnEnemies()
{
	UNavigationSystemV1* NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	UWorld* World = GetWorld();
	if (!NavigationSystem || !World)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	int32 TotalEnemiesSpawned = 0;
	for (auto& EnemySpawnInfo : EnemiesSpawnInfo)
	{
		int32 EnemyCount = FMath::RandRange(EnemySpawnInfo.EnemiesCountMin, EnemySpawnInfo.EnemiesCountMax);
		for (int i = 0; i < EnemyCount; ++i)
		{
			AEnemyAIController* AIController = World->SpawnActor<AEnemyAIController>(EnemySpawnInfo.ControllerClass);
			if (!AIController) continue;

			FNavLocation NavLocation;
			NavigationSystem->GetRandomReachablePointInRadius(CurrentActorInfo->AvatarActor->GetActorLocation(), SpawnRadius, NavLocation);
			AEnemyCharacter* NewEnemy = World->SpawnActor<AEnemyCharacter>(EnemySpawnInfo.EnemyClass, NavLocation.Location + FVector(0.0f, 0.0f, 100.0f), FRotator(0.0f));
			if (NewEnemy)
			{
				++TotalEnemiesSpawned;
				AIController->Possess(NewEnemy);
			}
		}
	}

	AMLGameMode* MLGameMode = Cast<AMLGameMode>(UGameplayStatics::GetGameMode(this));
	if (MLGameMode)
	{
		MLGameMode->AddEnemiesAmount(TotalEnemiesSpawned);
	}
}