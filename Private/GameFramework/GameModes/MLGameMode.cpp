// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/GameModes/MLGameMode.h"
#include "GameFramework/GameState.h"
#include "Actors/RoomTrigger.h"
#include "Characters/PlayerCharacter.h"
#include "Characters/EnemyCharacter.h"
#include "GameFramework/GameStates/MLGameState.h"
#include "AI/EnemyAIController.h"
#include "Actors/Teleporter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../MemoryLakePlayerController.h"
#include "NavigationSystem.h"
#include "Camera/CameraActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void AMLGameMode::BeginPlay()
{
	Super::BeginPlay();
}

int32 AMLGameMode::HandleRoomEntering(ARoomTrigger* RoomTrigger, ACharacter* EnteredCharacter)
{
	if (!GetWorld() || !EnteredCharacter || !RoomTrigger)
	{
		return 0;
	}

	LastRoomTrigger = RoomTrigger;

	PlayMusic(LastRoomTrigger->GetMusicToPlay());

	//MovePlayersToCharacter(EnteredCharacter);
	int32 SpawnedEnemies = SpawnEnemies(LastRoomTrigger->GetEnemiesSpawnInfo());
	return SpawnedEnemies;
}

void AMLGameMode::PlayMusic(USoundBase* MusicToPlay)
{
	if (!MusicToPlay)
	{
		return;
	}
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		AMemoryLakePlayerController* MLController = Cast<AMemoryLakePlayerController>(Iterator->Get());
		if (MLController)
		{
			MLController->PlayMusic(MusicToPlay);
		}
	}
}

void AMLGameMode::MovePlayersToCharacter(ACharacter* PlayerCharacter)
{
	UNavigationSystemV1* NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);

	FNavLocation NavLocation;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		AMemoryLakePlayerController* MLController = Cast<AMemoryLakePlayerController>(Iterator->Get());
		if (!MLController)
		{
			continue;
		}

		APlayerCharacter* MLCharacter = Cast<APlayerCharacter>(MLController->GetPawn());
		if (MLCharacter)
		{
			double DistanceBetweenPlayers = FVector::Distance(MLCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation());
			if (MLCharacter == PlayerCharacter || DistanceBetweenPlayers < MinDistanceToMove)
			{
				continue;
			}

			NavigationSystem->GetRandomPointInNavigableRadius(PlayerCharacter->GetActorLocation(), 300.0f, NavLocation);
			MLController->MovePlayerToLocation(NavLocation.Location);
		}
	}
}

int32 AMLGameMode::SpawnEnemies(TArray<FEnemySpawnInfo> EnemiesSpawnInfo)
{
	if (EnemiesSpawnInfo.IsEmpty())
	{
		return 0;
	}

	int32 SpawnedEnemies = 0;
	for (auto& EnemySpawnInfo : EnemiesSpawnInfo)
	{
		SpawnedEnemies += SpawnEnemyClass(EnemySpawnInfo);
	}

	AddEnemiesAmount(SpawnedEnemies);
	return SpawnedEnemies;
}

int32 AMLGameMode::SpawnEnemyClass(FEnemySpawnInfo EnemySpawnInfo)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return 0;
	}

	int32 EnemyCount = FMath::RandRange(EnemySpawnInfo.EnemiesCountMin, EnemySpawnInfo.EnemiesCountMax);
	int32 SpawnedEnemies = 0;
	for (int32 i = 0; i < EnemyCount; ++i)
	{
		AAIController* AIController = World->SpawnActor<AEnemyAIController>(EnemySpawnInfo.ControllerClass);
		if (!AIController)
		{
			continue;
		}

		AEnemyCharacter* NewEnemy = World->SpawnActor<AEnemyCharacter>(EnemySpawnInfo.EnemyClass, LastRoomTrigger->GetRandomPointInRoom(), FRotator(0.0f));
		if (NewEnemy)
		{
			AIController->Possess(NewEnemy);
			NewEnemy->SetParentRoom(LastRoomTrigger);
			++SpawnedEnemies;
		}
		else
		{
			AIController->Destroy();
		}
	}
	return SpawnedEnemies;
}

void AMLGameMode::AddEnemiesAmount(int32 EnemiesToAdd)
{
	AMLGameState* MLGameState = GetGameState<AMLGameState>();
	if (!MLGameState)
	{
		return;
	}

	MLGameState->AddEnemiesAmount(EnemiesToAdd);
}

void AMLGameMode::HandlePawnDied(APawn* DiedActor)
{
	if (!DiedActor)
	{
		return;
	}

	if (APlayerCharacter* MLCharacter = Cast<APlayerCharacter>(DiedActor))
	{
		HandlePlayerDeath(MLCharacter);
	}
	if (AEnemyCharacter* MLEnemy = Cast<AEnemyCharacter>(DiedActor))
	{
		HandleEnemyDeath(MLEnemy);
	}
}

void AMLGameMode::HandleEnemyDeath(AEnemyCharacter* EnemyCharacter)
{
	AMLGameState* MLGameState = GetGameState<AMLGameState>();
	if (!MLGameState || !EnemyCharacter) return;
}

void AMLGameMode::HandlePlayerDeath(APlayerCharacter* PlayerCharacter)
{
	AMLGameState* MLGameState = GetGameState<AMLGameState>();
	if (!MLGameState || !PlayerCharacter) return;

	APlayerController* DiedPlayer = PlayerCharacter->GetController<APlayerController>();
	if (!DiedPlayer)
	{
		return;
	}

	MLGameState->AddDiedPlayer(DiedPlayer);
}

void AMLGameMode::RespawnDiedPlayers()
{
	AMLGameState* MLGameState = GetGameState<AMLGameState>();
	if (!MLGameState)
	{
		return;
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (AMemoryLakePlayerController* MLController = Cast<AMemoryLakePlayerController>(Iterator->Get()))
		{
			MLController->RestoreCharacterHealth();
		}
	}

	TArray<APlayerController*> PlayersToRespawn = MLGameState->GetDiedPlayers();
	MLGameState->RemoveDiedPlayers();
	for (auto Player : PlayersToRespawn)
	{
		RespawnPlayerPawn(Player);
	}
}

void AMLGameMode::RespawnPlayerPawn(APlayerController* Player)
{
	APawn* OldPawn = Player->GetPawn();
	if (!OldPawn)
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OldPawn);
	if (!AbilitySystemComponent)
	{
		return;
	}

	FGameplayTag DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	bool bHasDeadTag = AbilitySystemComponent->HasMatchingGameplayTag(DeadTag);
	if (bHasDeadTag)
	{
		UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(OldPawn, FGameplayTagContainer(DeadTag));
	}

	Cast<APlayerCharacter>(OldPawn)->HandleRevive();
	RestartPlayer(Player);
}

void AMLGameMode::ShowUpgradesToPlayers()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (AMemoryLakePlayerController* MLController = Cast<AMemoryLakePlayerController>(Iterator->Get()))
		{
			MLController->ShowUpgrades();
		}
	}
}

void AMLGameMode::AddCurrencyToPlayers()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; Iterator++)
	{
		if (AMemoryLakePlayerController* MLController = Cast<AMemoryLakePlayerController>(Iterator->Get()))
		{
			if (APlayerCharacter* Ch = Cast<APlayerCharacter>(MLController->GetPawn()))
			{
				Ch->AddFocus(1);
				Ch->AddFoundMemory(1);
			}
		}
	}
}

bool AMLGameMode::ReadyToEndMatch_Implementation()
{
	AMLGameState* MLGameState = GetGameState<AMLGameState>();
	if (!MLGameState)
	{
		return false;
	}

	if (MLGameState->GetDiedPlayers().Num() == NumPlayers)
	{
		MLGameState->SetGameOverState(EGameOverState::LOST);
		return true;
	}

	return false;
}

void AMLGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();

	UWorld* World = GetWorld();
	AMLGameState* MLGameState = GetGameState<AMLGameState>();
	if (!World || !MLGameState)
	{
		return;
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (AMemoryLakePlayerController* MLController = Cast<AMemoryLakePlayerController>(Iterator->Get()))
		{
			if (MLGameState->GetGameOverState() == EGameOverState::WON)
			{
				MLController->ShowWonScreen();
			}
			if (MLGameState->GetGameOverState() == EGameOverState::LOST)
			{
				MLController->ShowLostScreen();
			}
			FTimerHandle Timer;
			FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AMLGameMode::MovePlayerToMenu, MLController);
			World->GetTimerManager().SetTimer(Timer, Delegate, TimeBeforeExitToMenu, false);
		}
	}
}

void AMLGameMode::MovePlayerToMenu(AMemoryLakePlayerController* Player)
{
	if (!Player)
	{
		return;
	}

	Player->ExitToMenu();
}