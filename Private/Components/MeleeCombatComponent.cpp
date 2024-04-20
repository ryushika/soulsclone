// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/MeleeCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Controller.h"
#include "GameplayTagContainer.h"

UMeleeCombatComponent::UMeleeCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bIsAttacking = false;
	TraceModifier = 1;
}

void UMeleeCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* ControlledPawn = GetOwner();
	if (!ensure(ControlledPawn)) return;
	AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ControlledPawn);
}

void UMeleeCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UMeleeCombatComponent::SwordAttack()
{
	if (bIsAttacking)
	{
		bSaveAttack = true;
	}

	else 
	{
		bIsAttacking = true;
		switch (AttackID)
		{
		case 0:
			ActivateAbility("Ability.BladeAttack1");
			AttackID = 1;
			break;
		case 1:
			ActivateAbility("Ability.BladeAttack2");
			AttackID = 2;
			break;
		case 2:
			ActivateAbility("Ability.HeavyAttack");
			AttackID = 0;
			break;
		default:
			break;
		}
	}
}

void UMeleeCombatComponent::SwordAttackCombo()
{
	if (bSaveAttack)
	{
		bSaveAttack = false;
		switch (AttackID)
		{
		case 0:
			ActivateAbility("Ability.BladeAttack1");
			AttackID = 1;
			break;
		case 1:
			ActivateAbility("Ability.BladeAttack2");
			AttackID = 2;
			break;
		case 2:
			ActivateAbility("Ability.HeavyAttack");
			AttackID = 0;
			break;
		default:
			break;
		}
	}
	else 
	{
		StopCombo();
	}
}

void UMeleeCombatComponent::HeavyAttack()
{
	if (!bIsAttacking)
	{
		bIsAttacking = true;
		ActivateAbility("Ability.HeavyAttack");
	}
}

void UMeleeCombatComponent::StopCombo()
{
	bIsAttacking = false;
	AttackID = 0;
}

void UMeleeCombatComponent::StartSwordTrace()
{
	GetWorld()->GetTimerManager().SetTimer(SwordTraceTimer, this, &UMeleeCombatComponent::SwordTrace, 0.1f, true);
}

void UMeleeCombatComponent::SwordTrace()
{
	OnSwordTrace.Broadcast();
}

void UMeleeCombatComponent::StopTrace()
{
	GetWorld()->GetTimerManager().ClearTimer(SwordTraceTimer);
}


void UMeleeCombatComponent::ActivateAbility(FName GameplayTag)
{ 
	if (!AbilitySystemComponent)  GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ASC is Null!"));;
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(FGameplayTag::RequestGameplayTag(GameplayTag));
	AbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer, true);
}

