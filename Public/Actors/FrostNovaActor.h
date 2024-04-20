// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "FrostNovaActor.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class MEMORYLAKE_API AFrostNovaActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFrostNovaActor();

	FORCEINLINE void SetSpellEffect(FGameplayEffectSpecHandle EffectSpecHandle) { SpellEffectHandle = EffectSpecHandle; }
	FORCEINLINE void SetDamageEffect(FGameplayEffectSpecHandle EffectSpecHandle) { DamageEffectHandle = EffectSpecHandle; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision")
	USphereComponent* SphereCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY()
	FGameplayEffectSpecHandle SpellEffectHandle;

	UPROPERTY()
	FGameplayEffectSpecHandle DamageEffectHandle;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
