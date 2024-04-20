// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "EnemyInfo.h"
#include "EnemyCharacter.generated.h"

class UGameplayAbility;

UCLASS()
class MEMORYLAKE_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "GameplayAbility")
	const FTaggedMontage& GetMontageToPlay() const;

	UFUNCTION(BlueprintCallable, Category = "GameplayAbility")
	FTaggedMontage GetMontageToPlayByTag(const FGameplayTag& MontageTag) const;

	UFUNCTION(BlueprintCallable, Category = "GameplayAbility")
	TArray<AActor*> GetCharactersToDamage(const FName& SocketName);

	UFUNCTION(BlueprintCallable)
	FVector GetTargetEnemyVector() const;


	UFUNCTION(BlueprintCallable)
	const float GetDamageMultiplierByTag(const FGameplayTag& Tag) const;

	UFUNCTION(BlueprintCallable)
	class ARoomTrigger* GetParentRoom();

	UFUNCTION(BlueprintCallable)
	void SetParentRoom(class ARoomTrigger* Room);

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;
	virtual void InitializeAttributes() override;
	virtual void AddCharacterAbilities() override;
	virtual void BindToAttributeChangeDelegates() override;
	virtual void OnHealthChanged(float NewHealth);
	virtual void HandleDeath() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy")
	TArray<TSubclassOf<UGameplayAbility>> EnemyAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	float HitSphereRadius = 35.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	TArray<FTaggedMontage> AnimMontages;

	FDelegateHandle OnChangeHealthDelegateHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	TMap<FGameplayTag, float> DamageMultiplierByTag;

	class ARoomTrigger* ParentRoom;
};
