// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CharacterAttributeSetBase.h"
#include "PlayerAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class MEMORYLAKE_API UPlayerAttributeSet : public UCharacterAttributeSetBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Movement", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Stamina)

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, AttackSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_FireDamage)
	FGameplayAttributeData FireDamage;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, FireDamage)

	UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_FirePeriodDamage)
	FGameplayAttributeData FirePeriodDamage;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, FirePeriodDamage)

	UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_FirePeriodDuration)
	FGameplayAttributeData FirePeriodDuration;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, FirePeriodDuration)

	UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_WaterDamage)
	FGameplayAttributeData WaterDamage;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, WaterDamage)

	UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_WaterSlowPower)
	FGameplayAttributeData WaterSlowPower;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, WaterSlowPower)

	UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_WaterSlowDuration)
	FGameplayAttributeData WaterSlowDuration;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, WaterSlowDuration)

	UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_EarthDamage)
	FGameplayAttributeData EarthDamage;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, EarthDamage)

	UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_EarthStunDuration)
	FGameplayAttributeData EarthStunDuration;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, EarthStunDuration)

	UPROPERTY(BlueprintReadOnly, Category = "Combat", ReplicatedUsing = OnRep_WindDamage)
	FGameplayAttributeData WindDamage;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, WindDamage)

	UPROPERTY(BlueprintReadWrite, Category = "Combat", ReplicatedUsing = OnRep_WindKnockBack)
	FGameplayAttributeData WindKnockBack;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, WindKnockBack)

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	FGameplayAttributeData LightningDamage;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, LightningDamage)

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UFUNCTION()
	virtual void OnRep_FireDamage(const FGameplayAttributeData& OldFireDamage);

	UFUNCTION()
	virtual void OnRep_FirePeriodDamage(const FGameplayAttributeData& OldFirePeriodDamage);

	UFUNCTION()
	virtual void OnRep_FirePeriodDuration(const FGameplayAttributeData& OldFirePeriodDuration);

	UFUNCTION()
	virtual void OnRep_WaterDamage(const FGameplayAttributeData& OldWaterDamage);

	UFUNCTION()
	virtual void OnRep_WaterSlowPower(const FGameplayAttributeData& OldWaterSlowPower);

	UFUNCTION()
	virtual void OnRep_WaterSlowDuration(const FGameplayAttributeData& OldWaterSlowDuration);

	UFUNCTION()
	virtual void OnRep_EarthDamage(const FGameplayAttributeData& OldEarthDamage);

	UFUNCTION()
	virtual void OnRep_EarthStunDuration(const FGameplayAttributeData& OldEarthStunDuration);

	UFUNCTION()
	virtual void OnRep_WindDamage(const FGameplayAttributeData& OldWindDamage);

	UFUNCTION()
	virtual void OnRep_WindKnockBack(const FGameplayAttributeData& OldWindKnockBack);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
};
