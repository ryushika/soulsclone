// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "MemoryLake.h"
#include "BaseCharacter.generated.h"

class UCharacterAttributeSetBase;
class UGameplayEffect;
class UHealthWidgetComponent;
class AGameplayCueNotify_Actor;

UCLASS()
class MEMORYLAKE_API ABaseCharacter
	: public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UCharacterAttributeSetBase* GetAttributeSet() const { return AttributeSet; }

	float GetMovementSpeed() const;

public:
	FOnDeathDelegate OnDeathDelegate;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AGameplayCueNotify_Actor> BurningGameplayCue;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AGameplayCueNotify_Actor> WaterGameplayCue;

	UFUNCTION(NetMulticast, Reliable)
	virtual void HandleRevive();

	UFUNCTION(BlueprintCallable)
	void UpdateGameplayCues(bool bFire) const;

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UCharacterAttributeSetBase> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UHealthWidgetComponent> HealthWidget;

	//Put effects into correct order
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effects")
	TArray<TSubclassOf<UGameplayEffect>> DefaultAttributes;

	UPROPERTY()
	FGameplayTag DeadTag;

protected:
	virtual void BeginPlay() override;
	virtual void SetupHealthWidget();

	virtual void InitAbilityActorInfo();
	virtual void InitializeAttributes();
	virtual void AddCharacterAbilities();
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level);
	virtual void BindToAttributeChangeDelegates();
	void RemoveAbilitiesAfterDeath();
	
	virtual void OnHealthChanged(float NewHealth);

	bool IsDead() const;

	UFUNCTION(NetMulticast, Reliable)
	virtual void HandleDeath();

private:
	void BindOnDeathDelegate();
};
