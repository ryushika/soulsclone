// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/BaseEnemyGameplayAbility.h"
#include "Characters/EnemyCharacter.h"

UBaseEnemyGameplayAbility::UBaseEnemyGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UBaseEnemyGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!EnemyCharacter)
	{
		EnemyCharacter = Cast<AEnemyCharacter>(ActorInfo->AvatarActor);
	}
	PlayAnimMontage();
	WaitForGameplayEvent();
}