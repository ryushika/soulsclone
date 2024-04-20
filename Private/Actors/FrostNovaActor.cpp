// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/FrostNovaActor.h"
#include "Components/SphereComponent.h"
#include "Characters/PlayerCharacter.h"
#include "NiagaraComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AFrostNovaActor::AFrostNovaActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereCollision->SetGenerateOverlapEvents(true);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = SphereCollision;
	
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Water Splash"));
	NiagaraComponent->SetupAttachment(SphereCollision);
}

void AFrostNovaActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (SphereCollision)
	{
		SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AFrostNovaActor::OnComponentBeginOverlap);
	}
	SetLifeSpan(1.0f);
}

void AFrostNovaActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* OverlappedPlayer = Cast<APlayerCharacter>(OtherActor);
	if (!OverlappedPlayer)
	{
		return;
	}
	UAbilitySystemComponent* TargetASC = OverlappedPlayer->GetAbilitySystemComponent();
	if (TargetASC && HasAuthority())
	{
		TargetASC->ApplyGameplayEffectSpecToSelf(*(SpellEffectHandle.Data.Get()));
		TargetASC->ApplyGameplayEffectSpecToSelf(*(DamageEffectHandle.Data.Get()));
	}

}
