// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/RangedAttack.h"
#include "Characters/EnemyCharacter.h"
#include "Actors/EnemyProjectile.h"
#include "AbilitySystemComponent.h"

void URangedAttack::LaunchProjectile(const FName& SocketName)
{
	if (!EnemyCharacter)
	{
		return;
	}
	FTransform ProjectileTransform = EnemyCharacter->GetMesh()->GetSocketTransform(SocketName);
	
	FRotator RandomDeviationRotator = FRotator::ZeroRotator;
	RandomDeviationRotator.Pitch = FMath::RandRange(0.f, RandomDeviation);
	RandomDeviationRotator.Yaw = FMath::RandRange(-RandomDeviation, RandomDeviation);

	FVector ShootDirection = EnemyCharacter->GetTargetEnemyVector() - ProjectileTransform.GetLocation();
	ShootDirection.Normalize();

	ProjectileTransform.SetRotation((ShootDirection.Rotation() + RandomDeviationRotator).Quaternion());

	AEnemyProjectile* Projectile = GetWorld()->SpawnActorDeferred<AEnemyProjectile>(ProjectileClass, ProjectileTransform, 
		EnemyCharacter, EnemyCharacter, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	FGameplayEffectSpecHandle GESpec = MakeOutgoingGameplayEffectSpec(DamageEffect, 1.f);
	GESpec.Data->SetSetByCallerMagnitude(DamageTypeTag, EnemyCharacter->GetDamageMultiplierByTag(DamageTypeTag));
	Projectile->SetDamageEffectSpecHandle(GESpec);

	Projectile->FinishSpawning(ProjectileTransform);
}