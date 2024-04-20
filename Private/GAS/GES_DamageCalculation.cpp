// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GES_DamageCalculation.h"
#include "GAS/CharacterAttributeSetBase.h"
#include "GAS/EnemyAttributeSet.h"
#include "GAS/PlayerAttributeSet.h"
#include "AbilitySystemComponent.h"

struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(WaterResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(WindResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(EarthResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(WaterDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(WindDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(EarthDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(EnemyDamageMultiplier);

	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, WaterDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, FireDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, WindDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, EarthDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, LightningDamage, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSetBase, WaterResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSetBase, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSetBase, WindResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSetBase, EarthResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSetBase, LightningResistance, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSetBase, Health, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UEnemyAttributeSet, EnemyDamageMultiplier, Source, false);
	}
};

static const FDamageStatics& DamageStatics()
{
	static FDamageStatics DmgStatics;
	return DmgStatics;
}

UGES_DamageCalculation::UGES_DamageCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().WaterResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().WindResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().EarthResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);

	RelevantAttributesToCapture.Add(DamageStatics().WaterDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().FireDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().WindDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().EarthDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningDamageDef);

	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);

	RelevantAttributesToCapture.Add(DamageStatics().EnemyDamageMultiplierDef);
}

void UGES_DamageCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
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

	float BaseWaterDamage = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage.Water")), false, -1.f), 0.0f);
	float BaseFireDamage = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage.Fire")), false, -1.f), 0.0f);
	float BaseEarthDamage = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage.Earth")), false, -1.f), 0.0f);
	float BaseWindDamage = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage.Wind")), false, -1.f), 0.0f);
	float BaseLightningDamage = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage.Lightning")), false, -1.f), 0.0f);

	float WaterResist = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().WaterResistanceDef, EvaluateParameters, WaterResist);
	float FireResist = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().FireResistanceDef, EvaluateParameters, FireResist);
	float EarthResist = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().EarthResistanceDef, EvaluateParameters, EarthResist);
	float WindResist = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().WindResistanceDef, EvaluateParameters, WindResist);
	float LightningResist = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().LightningResistanceDef, EvaluateParameters, LightningResist);

	float WaterDamageMult = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().WaterDamageDef, EvaluateParameters, WaterDamageMult);
	float FireDamageMult = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().FireDamageDef, EvaluateParameters, FireDamageMult);
	float EarthDamageMult = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().EarthDamageDef, EvaluateParameters, EarthDamageMult);
	float WindDamageMult = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().WindDamageDef, EvaluateParameters, WindDamageMult);
	float LightningDamageMult = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().LightningDamageDef, EvaluateParameters,LightningDamageMult);
	float EnemyDamageMult = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().EnemyDamageMultiplierDef, EvaluateParameters, EnemyDamageMult);

	float WaterDone = FMath::IsNearlyZero(WaterResist) ? (BaseWaterDamage * WaterDamageMult) : (BaseWaterDamage * WaterDamageMult / WaterResist);
	float FireDone = FMath::IsNearlyZero(FireResist) ? (BaseFireDamage * FireDamageMult) : (BaseFireDamage * FireDamageMult / FireResist);
	float EarthDone = FMath::IsNearlyZero(EarthResist) ? (BaseEarthDamage * EarthDamageMult) : (BaseEarthDamage * EarthDamageMult / EarthResist);
	float WindDone = FMath::IsNearlyZero(WindResist) ? (BaseWindDamage * WindDamageMult) : (BaseWindDamage * WindDamageMult / WindResist);
	float LightningDone = FMath::IsNearlyZero(LightningResist) ? (BaseLightningDamage * LightningDamageMult) : (BaseLightningDamage * LightningDamageMult / LightningResist);

	float EnemyDone = 0.f;
	EnemyDone += !FMath::IsNearlyZero(WaterDone) ? WaterDone : 0.f;
	EnemyDone += !FMath::IsNearlyZero(FireDone) ? FireDone : 0.f;
	EnemyDone += !FMath::IsNearlyZero(EarthDone) ? EarthDone : 0.f;
	EnemyDone += !FMath::IsNearlyZero(WindDone) ? WindDone : 0.f;
	EnemyDone += !FMath::IsNearlyZero(LightningDone) ? LightningDone : 0.f;
	EnemyDone *= EnemyDamageMult;

	float DamageDone = WaterDone + FireDone + EarthDone + WindDone + LightningDone;
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().HealthProperty, 
		EGameplayModOp::Additive, FMath::IsNearlyZero(EnemyDone) ? -DamageDone : -EnemyDone));
}
