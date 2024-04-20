// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/BaseEnemyGameplayAbility.h"
#include "../../EnemyInfo.h"
#include "BaseEnemyGameplayAbility.h"
#include "FrostNova.generated.h"

class AEnemyCharacter;
class AFrostNovaActor;

UCLASS()
class MEMORYLAKE_API UFrostNova : public UBaseEnemyGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFrostNova();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Frost Nova")
	TSubclassOf<AActor> DecalClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Frost Nova")
	TSubclassOf<AFrostNovaActor> FrostNovaClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Frost Nova")
	TSubclassOf<UGameplayEffect> SpellEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Frost Nova")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(BlueprintReadWrite)
	FVector TargetLocation;

	UPROPERTY(BlueprintReadWrite)
	AActor* DecalActor;
private:
	UFUNCTION(BlueprintCallable)
		void UseFrostNova();
};
