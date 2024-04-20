// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/BaseEnemyGameplayAbility.h"
#include "../../EnemyInfo.h"
#include "SummonEnemies.generated.h"

struct FEnemySpawnInfo;
class AEnemyCharacter;

UCLASS()
class MEMORYLAKE_API USummonEnemies : public UBaseEnemyGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Info")
	TArray<FEnemySpawnInfo> EnemiesSpawnInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Info")
	float SpawnRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag MontageTag;

private:
	UFUNCTION(BlueprintCallable)
	void SpawnEnemies();
};
