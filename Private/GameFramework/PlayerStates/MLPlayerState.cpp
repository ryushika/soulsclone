// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PlayerStates/MLPlayerState.h"
#include "GAS/CharacterAbilitySystemComponent.h"
#include "GAS/PlayerAttributeSet.h"

AMLPlayerState::AMLPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	AttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("AttributeSet"));

	NetUpdateFrequency = 100.0f;

	SetReplicates(true);
}

UAbilitySystemComponent* AMLPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float AMLPlayerState::GetHealth() const
{
	return Cast<UCharacterAttributeSetBase>(AttributeSet)->GetHealth();
}

float AMLPlayerState::GetMaxHealth() const
{
	return Cast<UCharacterAttributeSetBase>(AttributeSet)->GetMaxHealth();
}

void AMLPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent && AttributeSet)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			});
	}
}