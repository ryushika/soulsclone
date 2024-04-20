// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EnemyInfo.h"
#include "BaseEnemyGameplayAbility.generated.h"

class AEnemyCharacter;

UCLASS()
class MEMORYLAKE_API UBaseEnemyGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UBaseEnemyGameplayAbility();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PlayAnimMontage();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void WaitForGameplayEvent();

	UPROPERTY(EditDefaultsOnly, Category = "Damage Type")
	FGameplayTag DamageTypeTag;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(BlueprintReadWrite)
	FTaggedMontage TaggedMontage;

	UPROPERTY(BlueprintReadWrite)
	TArray<UGameplayTask*> Tasks;

	UPROPERTY(BlueprintReadWrite)
	AEnemyCharacter* EnemyCharacter = nullptr;
};
