// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "MLPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterAttributeValueChange, float);

class UCharacterAttributeSetBase;

UCLASS()
class MEMORYLAKE_API AMLPlayerState
	: public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMLPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UCharacterAttributeSetBase* GetAttributeSet() const { return AttributeSet; }

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;

	virtual void BeginPlay() override;
	
	FCharacterAttributeValueChange OnHealthChanged;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UCharacterAttributeSetBase> AttributeSet;
};
