// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MemoryLake.h"
#include "GameplayEffectTypes.h"
#include "EnemyProjectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class MEMORYLAKE_API AEnemyProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyProjectile();

	void SetDamageEffectSpecHandle(const FGameplayEffectSpecHandle& DamageEffectSpecHandleToSet);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	EElements Element;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

	virtual void BeginPlay() override;

	UPROPERTY()
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
				const FHitResult& Hit);
};
