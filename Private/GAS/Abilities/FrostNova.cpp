// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/FrostNova.h"
#include "Characters/EnemyCharacter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Actors/FrostNovaActor.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"

UFrostNova::UFrostNova()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UFrostNova::UseFrostNova()
{
	UWorld* World = GetWorld();
	UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromActorInfo();
	if (!World || !OwnerASC || !EnemyCharacter)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	AFrostNovaActor* FrostNova = World->SpawnActorDeferred<AFrostNovaActor>(FrostNovaClass, FTransform(FRotator(0.0f), TargetLocation), EnemyCharacter, EnemyCharacter, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	FrostNova->SetSpellEffect(MakeOutgoingGameplayEffectSpec(SpellEffectClass));

	FGameplayEffectSpecHandle DamageSpec = MakeOutgoingGameplayEffectSpec(DamageEffect);
	DamageSpec.Data->SetSetByCallerMagnitude(DamageTypeTag, EnemyCharacter->GetDamageMultiplierByTag(DamageTypeTag));
	FrostNova->SetDamageEffect(DamageSpec);

	FrostNova->FinishSpawning(FTransform(FRotator(0.0f), TargetLocation));
}