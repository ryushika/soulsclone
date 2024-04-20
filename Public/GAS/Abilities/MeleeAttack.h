// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemyGameplayAbility.h"
#include "MeleeAttack.generated.h"

UCLASS()
class MEMORYLAKE_API UMeleeAttack : public UBaseEnemyGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable)
	void ApplyDamageToCharacters(const TArray<AActor*>& CharactersToDamage);
};
