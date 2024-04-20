// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PlayerAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

void UPlayerAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Stamina, OldStamina);
}

void UPlayerAttributeSet::OnRep_FireDamage(const FGameplayAttributeData& OldFireDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, FireDamage, OldFireDamage);
}

void UPlayerAttributeSet::OnRep_FirePeriodDamage(const FGameplayAttributeData& OldFirePeriodDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, FirePeriodDamage, OldFirePeriodDamage);
}

void UPlayerAttributeSet::OnRep_FirePeriodDuration(const FGameplayAttributeData& OldFirePeriodDuration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, FirePeriodDuration, OldFirePeriodDuration);
}

void UPlayerAttributeSet::OnRep_WaterDamage(const FGameplayAttributeData& OldWaterDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, WaterDamage, OldWaterDamage);
}

void UPlayerAttributeSet::OnRep_WaterSlowPower(const FGameplayAttributeData& OldWaterSlowPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, WaterSlowPower, OldWaterSlowPower);
}

void UPlayerAttributeSet::OnRep_WaterSlowDuration(const FGameplayAttributeData& OldWaterSlowDuration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, WaterSlowDuration, OldWaterSlowDuration);
}

void UPlayerAttributeSet::OnRep_EarthDamage(const FGameplayAttributeData& OldEarthDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, EarthDamage, OldEarthDamage);
}

void UPlayerAttributeSet::OnRep_EarthStunDuration(const FGameplayAttributeData& OldEarthStunDuration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, EarthStunDuration, OldEarthStunDuration);
}

void UPlayerAttributeSet::OnRep_WindDamage(const FGameplayAttributeData& OldWindDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, WindDamage, OldWindDamage);
}

void UPlayerAttributeSet::OnRep_WindKnockBack(const FGameplayAttributeData& OldWindKnockBack)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, WindKnockBack, OldWindKnockBack);
}

void UPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, FireDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, FirePeriodDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, FirePeriodDuration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, WaterDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, WaterSlowPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, WaterSlowDuration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, EarthDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, EarthStunDuration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, WindDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, WindKnockBack, COND_None, REPNOTIFY_Always);
}

void UPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

