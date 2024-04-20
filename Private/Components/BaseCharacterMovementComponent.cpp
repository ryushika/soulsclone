// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BaseCharacterMovementComponent.h"
#include "Characters/BaseCharacter.h"
#include "AbilitySystemComponent.h"

float UBaseCharacterMovementComponent::GetMaxSpeed() const
{
	float BaseSpeed = Super::GetMaxSpeed();
	ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner());
	if (!Character)
	{
		return BaseSpeed;
	}

	return Character->GetMovementSpeed();
}
