// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemyGameplayAbility.h"
#include "RangedAttack.generated.h"

class AEnemyProjectile;

UCLASS()
class MEMORYLAKE_API URangedAttack : public UBaseEnemyGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AEnemyProjectile> ProjectileClass;

	UFUNCTION(BlueprintCallable)
	void LaunchProjectile(const FName& SocketName);

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Spread", meta = (Units = "Degrees"))
	float RandomDeviation = 5.f;
};
