// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CharacterAttributeSetBase.h"
#include "EnemyAttributeSet.generated.h"


UCLASS()
class MEMORYLAKE_API UEnemyAttributeSet : public UCharacterAttributeSetBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_AttackSpeed)
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UEnemyAttributeSet, AttackSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_EnemyDamageMultiplier)
	FGameplayAttributeData EnemyDamageMultiplier;
	ATTRIBUTE_ACCESSORS(UEnemyAttributeSet, EnemyDamageMultiplier)

	UFUNCTION()
	void OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed);

	UFUNCTION()
	void OnRep_EnemyDamageMultiplier(const FGameplayAttributeData& OldEnemyDamageMultiplier);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
};
