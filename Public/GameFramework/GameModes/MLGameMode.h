// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "../../MemoryLake.h"
#include "MLGameMode.generated.h"

class ARoomTrigger;
class AEnemyCharacter;
class AEnemyAIController;
class APlayerCharacter;
struct FEnemySpawnInfo;
class UNiagaraSystem;
class AMemoryLakePlayerController;

UCLASS()
class MEMORYLAKE_API AMLGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	int32 HandleRoomEntering(ARoomTrigger* RoomTrigger, ACharacter* EnteredCharacter);
	void HandlePawnDied(APawn* DiedActor);

	void AddEnemiesAmount(int32 EnemiesToAdd);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	int32 PlayersRequired;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float MinDistanceToMove;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float TimeBeforeExitToMenu = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	TArray<USoundBase*> DefaultMusic;

	TArray<class ATeleporter*> AllPortals;
protected:

	virtual void BeginPlay() override;

	void RespawnPlayerPawn(APlayerController* Player);

	virtual bool ReadyToEndMatch_Implementation() override;
	virtual void HandleMatchHasEnded() override;

	void PlayMusic(USoundBase* MusicToPlay);

private:
	UPROPERTY()
	ARoomTrigger* LastRoomTrigger;

private:
	void MovePlayersToCharacter(ACharacter* Character);

	int32 SpawnEnemies(TArray<FEnemySpawnInfo> EnemiesSpawnInfo);
	int32 SpawnEnemyClass(FEnemySpawnInfo EnemySpawnInfo);

	void HandleEnemyDeath(AEnemyCharacter* EnemyCharacter);
	void HandlePlayerDeath(APlayerCharacter* PlayerCharacter);

	void RespawnDiedPlayers();
	void ShowUpgradesToPlayers();
	void AddCurrencyToPlayers();

	void MovePlayerToMenu(AMemoryLakePlayerController* Player);
};
