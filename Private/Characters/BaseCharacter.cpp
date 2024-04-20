// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "GAS/CharacterAttributeSetBase.h"
#include "Components/BaseCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModes/MLGameMode.h"
#include "Components/HealthWidgetComponent.h"
#include "Characters/EnemyCharacter.h"
#include "GameplayCueNotify_Actor.h"
#include "Components/SkeletalMeshComponent.h"

ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));

	HealthWidget = CreateDefaultSubobject<UHealthWidgetComponent>(TEXT("Health widget"));
	HealthWidget->SetupAttachment(RootComponent);
	HealthWidget->bOwnerNoSee = true;
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);

	if (GetMesh())
	{
		GetMesh()->SetReceivesDecals(false);
	}
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float ABaseCharacter::GetMovementSpeed() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMovementSpeed();
	}

	return 0.0f;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	BindOnDeathDelegate();
	SetupHealthWidget();
}

void ABaseCharacter::BindOnDeathDelegate()
{
	AMLGameMode* MLGameMode = Cast<AMLGameMode>(UGameplayStatics::GetGameMode(this));
	if (!MLGameMode)
	{
		return;
	}
	OnDeathDelegate.BindUObject(MLGameMode, &AMLGameMode::HandlePawnDied);
}

void ABaseCharacter::SetupHealthWidget()
{
	if (HealthWidget)
	{
		HealthWidget->UpdateHealthBar(1.0f);
	}
}

void ABaseCharacter::InitAbilityActorInfo()
{
}

void ABaseCharacter::InitializeAttributes()
{
	for (const auto& GameplayEffect : DefaultAttributes)
	{
		ApplyEffectToSelf(GameplayEffect, 1.f);
	}
}

void ABaseCharacter::AddCharacterAbilities()
{
}

void ABaseCharacter::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, float Level)
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, Level, EffectContext);
	if (EffectHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void ABaseCharacter::BindToAttributeChangeDelegates()
{
}

void ABaseCharacter::RemoveAbilitiesAfterDeath()
{
	if (!HasAuthority() || !AbilitySystemComponent)
	{
		return;
	}

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if (Spec.SourceObject == this)
		{
			AbilitiesToRemove.Add(Spec.Handle);
			UE_LOG(LogTemp, Warning, TEXT("Ability: %s"), *Spec.Ability.GetName());
		}
	}

	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}
}

void ABaseCharacter::OnHealthChanged(float NewHealth)
{
	if (HealthWidget)
	{
		HealthWidget->UpdateHealthBar(NewHealth / (AttributeSet->GetMaxHealth()));
	}

	if (IsDead() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	{
		HandleDeath();
	}
}

bool ABaseCharacter::IsDead() const
{
	return FMath::IsNearlyZero(AttributeSet->GetHealth());
}

void ABaseCharacter::HandleDeath_Implementation()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetRenderCustomDepth(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	if (HasAuthority())
	{
		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}
	OnDeathDelegate.ExecuteIfBound(this);
	RemoveAbilitiesAfterDeath();
	if (HealthWidget)
	{
		HealthWidget->bHiddenInGame = true;
	}
}

void ABaseCharacter::HandleRevive_Implementation()
{
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	GetMesh()->SetRelativeLocation(FVector(0., 0., -90.));
	GetMesh()->SetRelativeRotation(FRotator(0.,-90.,0.));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	if (HealthWidget)
	{
		HealthWidget->bHiddenInGame = false;
	}
}

void ABaseCharacter::UpdateGameplayCues(bool bFire) const
{
	if (!GetAbilitySystemComponent())
	{
		return;
	}
	if (bFire)
	{
		if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Slowdown"))))
		{
			FGameplayTagContainer TagsContainer;
			TagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Slowdown")));
			GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(TagsContainer);
			if (WaterGameplayCue)
			{
				WaterGameplayCue->Recycle();
			}
		}
	}
	else
	{
		if (AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Burning"))))
		{
			FGameplayTagContainer TagsContainer;
			TagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Burning")));
			GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(TagsContainer);
			if (BurningGameplayCue)
			{
				BurningGameplayCue->Recycle();
			}
		}
	}
}