// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/EnemyProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/ShapeComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AEnemyProjectile::AEnemyProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;

	bReplicates = true;
}

void AEnemyProjectile::SetDamageEffectSpecHandle(const FGameplayEffectSpecHandle& DamageEffectSpecHandleToSet)
{
	DamageEffectSpecHandle = DamageEffectSpecHandleToSet;
}

void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		return;
	}

	UShapeComponent* ShapeCollision = GetComponentByClass<UShapeComponent>();
	if (!ShapeCollision)
	{
		return;
	}

	ShapeCollision->OnComponentHit.AddDynamic(this, &AEnemyProjectile::OnProjectileHit);
}

void AEnemyProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetOwner())
	{
		Destroy();
		return;
	}

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (TargetASC && HasAuthority())
	{
		TargetASC->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data.Get(), TargetASC);

	}
	Destroy();
}