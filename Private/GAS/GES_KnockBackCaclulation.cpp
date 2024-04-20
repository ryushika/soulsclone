// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GES_KnockBackCaclulation.h"
#include "GAS/GES_DamageCalculation.h"
#include "GAS/EnemyAttributeSet.h"
#include "GAS/PlayerAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Characters/BaseCharacter.h>

struct FKnockBackStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(WindKnockBack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Mass);

	FKnockBackStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, WindKnockBack, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, Mass, Target, false);
	}
};

static const FKnockBackStatics& KnockBackStatics()
{
	static FKnockBackStatics KnockBackStatics;
	return KnockBackStatics;
}

UGES_KnockBackCaclulation::UGES_KnockBackCaclulation()
{
	RelevantAttributesToCapture.Add(FKnockBackStatics().WindKnockBackDef);

	RelevantAttributesToCapture.Add(FKnockBackStatics().MassDef);
}

void UGES_KnockBackCaclulation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetABSC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetABSC ? TargetABSC->GetAvatarActor() : nullptr;

	UAbilitySystemComponent* SourceABSC = ExecutionParams.GetSourceAbilitySystemComponent();
	AActor* SourceActor = SourceABSC ? SourceABSC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	float WindKnockBack = 80.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FKnockBackStatics().WindKnockBackDef, EvaluateParameters, WindKnockBack);
	float Mass = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FKnockBackStatics().MassDef, EvaluateParameters, Mass);
	float KnockBackStr = WindKnockBack * 500 / Mass * Spec.GetLevel();

	FVector Direction = TargetActor->GetActorLocation() - SourceActor->GetActorLocation();
	Direction.Normalize(1);
	FVector KnockBackVector = Direction * KnockBackStr;
	ABaseCharacter* TargetCharacter = Cast<ABaseCharacter>(TargetActor);
	TargetCharacter->GetMovementComponent()->Velocity += KnockBackVector;
}
