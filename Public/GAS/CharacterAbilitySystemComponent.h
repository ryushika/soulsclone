// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MemoryLake.h"
#include "CharacterAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UCharacterAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);

UCLASS()
class MEMORYLAKE_API UCharacterAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	bool CharacterAbilitiesGiven = false;
	bool StartupEffectApplied = false;

	FReceivedDamageDelegate ReceivedDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Magic, meta = (AllowPrivateAccess = "true"), Replicated)
	TArray<EElements> Elements;

	UFUNCTION(BlueprintCallable, category = "Magic", Server, Reliable)
	void AddElement(EElements Element);

	UFUNCTION(BlueprintCallable, category = "Magic", Server, Reliable)
	void ClearElement();

	UFUNCTION(BlueprintCallable, category = "Blade")
	void Block(bool bShouldBlock);

	virtual void ReceiveDamage(UCharacterAbilitySystemComponent* SoutceASC, float UnmitigatedDamage, float MitigatedDamage);

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION(BlueprintAuthorityOnly)
	void SetAbilityLevel(TSubclassOf<UGameplayAbility> AbilityClass, int32 NewLevel);

};
