// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/MeleeAttack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Characters/EnemyCharacter.h"

void UMeleeAttack::ApplyDamageToCharacters(const TArray<AActor*>& CharactersToDamage)
{
	if (!EnemyCharacter || CharactersToDamage.Num() == 0)
	{
		return;
	}

	for (const auto& Character : CharactersToDamage)
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Character))
		{
			FGameplayEffectSpecHandle GESpec = MakeOutgoingGameplayEffectSpec(DamageEffect, 1.f);
			GESpec.Data->SetSetByCallerMagnitude(DamageTypeTag, EnemyCharacter->GetDamageMultiplierByTag(DamageTypeTag));
			TargetASC->ApplyGameplayEffectSpecToTarget(*GESpec.Data.Get(), TargetASC);
		}
	}
}