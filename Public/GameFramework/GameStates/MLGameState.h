// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "../../MemoryLake.h"
#include <Kismet/GameplayStatics.h>
#include "Characters/PlayerCharacter.h"
#include "MLGameState.generated.h"

UCLASS()
class MEMORYLAKE_API AMLGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void AddDiedPlayer(APlayerController* DiedPlayer) { DiedPlayers.AddUnique(DiedPlayer); }
	FORCEINLINE void RemoveDiedPlayers() { DiedPlayers.Empty(); }
	FORCEINLINE void RemoveDiedPlayer(APlayerController* PlayerToRemove) { DiedPlayers.Remove(PlayerToRemove); }
	FORCEINLINE TArray<APlayerController*> GetDiedPlayers() const { return DiedPlayers; }

	FORCEINLINE void AddEnemiesAmount(int32 EnemiesToAdd) { CurrentEnemiesAmount += EnemiesToAdd; }
	FORCEINLINE void SubstractEnemiesAmount(int32 EnemiesToSubstract) { CurrentEnemiesAmount -= EnemiesToSubstract; }
	FORCEINLINE int32 GetEnemiesAmount() const { return CurrentEnemiesAmount; }

	FORCEINLINE void SetGameOverState(EGameOverState NewGameOverState) { GameOverState = NewGameOverState; }
	FORCEINLINE EGameOverState GetGameOverState() const { return GameOverState; }

	FORCEINLINE void AddPlayer(APlayerController* NewPlayer) { Player = NewPlayer; }
	FORCEINLINE APlayerController* GetPlayer() const { return Player; }
protected:
	int32 CurrentEnemiesAmount;

	UPROPERTY()
	TArray<APlayerController*> DiedPlayers;

	EGameOverState GameOverState;

	UPROPERTY()
	APlayerController* Player;
};
