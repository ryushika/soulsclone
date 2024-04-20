// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.h"
#include "MemoryLake.h"
#include "MeleeCombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MEMORYLAKE_API UMeleeCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMeleeCombatComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat System")
	bool bCanAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat System")
	bool bIsAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat System")
	bool bSaveAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat System")
	int AttackID;

	FTimerHandle SwordTraceTimer;

	UPROPERTY(BlueprintAssignable, Category = "Combat System")
	FAttackDelegate OnSwordTrace;

	UPROPERTY(VisibleAnywhere, Category = "Combat System")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat System")
	UGameplayAbility *EnchantAbility;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat System")
	UGameplayAbility* GunEnchantAbility;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat System")
	float TraceModifier;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable) 
	void SwordAttack();

	UFUNCTION(BlueprintCallable)
	void StartSwordTrace();

	UFUNCTION(BlueprintCallable)
	void SwordTrace();

	UFUNCTION(BlueprintCallable)
	void StopTrace();

	UFUNCTION(BlueprintCallable)
	void HeavyAttack();

	UFUNCTION(BlueprintCallable)
	void SwordAttackCombo();

	UFUNCTION(BlueprintCallable)
	void StopCombo();

	UFUNCTION()
	void ActivateAbility(FName GameplayTag);
};
