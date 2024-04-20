// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/CharacterAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"


void UCharacterAbilitySystemComponent::AddElement_Implementation(EElements Element)
{
	if (Elements.Num() < 3)
	{
		Elements.Add(Element);
	}
}

void UCharacterAbilitySystemComponent::ClearElement_Implementation()
{
	Elements.Empty();
}


void UCharacterAbilitySystemComponent::Block(bool bShouldBlock)
{
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Ability.Block")));
	if (bShouldBlock)
	{
		TryActivateAbilitiesByTag(TagContainer, true);
	}
	else 
	{
		TagContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("State.Blocking")));
		RemoveActiveEffectsWithGrantedTags(TagContainer);
	}
}

void UCharacterAbilitySystemComponent::ReceiveDamage(UCharacterAbilitySystemComponent* SoutceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SoutceASC, UnmitigatedDamage, MitigatedDamage);
}

void UCharacterAbilitySystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCharacterAbilitySystemComponent, Elements);
}

void UCharacterAbilitySystemComponent::SetAbilityLevel(TSubclassOf<UGameplayAbility> AbilityClass, int32 NewLevel)
{
    //Are we on the server? If not, we should not be modifying Ability Specs!
    //if (!HasAuthority())
   // {
   //     return;
   // }

    FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromClass(AbilityClass);

    if (AbilitySpec)
    {
        AbilitySpec->Level = NewLevel;
        MarkAbilitySpecDirty(*AbilitySpec);
    }
}