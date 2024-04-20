// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"
#include "BrainComponent.h"
#include "GAS/CharacterAbilitySystemComponent.h"
#include "GAS/EnemyAttributeSet.h"
#include "Abilities/GameplayAbility.h"
#include "AI/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <GameFramework/GameStates/MLGameState.h>
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/DynamicCameraComponent.h"

AEnemyCharacter::AEnemyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(ENEMY_STENCIL_INDEX);

	AbilitySystemComponent = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UEnemyAttributeSet>(TEXT("AttributeSet"));

	UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	CharacterMovementComponent->bUseRVOAvoidance = true;
	CharacterMovementComponent->AvoidanceConsiderationRadius = 100.0f;
	Tags.Add(FName("Enemy"));
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Init ability actor info for the Server
	InitAbilityActorInfo();

	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(ENEMY_STENCIL_INDEX);
}

void AEnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();
	BindToAttributeChangeDelegates();
	AddCharacterAbilities();
}

void AEnemyCharacter::InitializeAttributes()
{
	Super::InitializeAttributes();
}

void AEnemyCharacter::AddCharacterAbilities()
{
	if (!HasAuthority() || !AbilitySystemComponent)
	{
		return;
	}

	for (const TSubclassOf<UGameplayAbility>& Ability : EnemyAbilities)
	{
		FGameplayAbilitySpecHandle Spec = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, -1, this));
	}
}

void AEnemyCharacter::BindToAttributeChangeDelegates()
{
	if (AbilitySystemComponent && AttributeSet)
	{
		OnChangeHealthDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) 
			{
				OnHealthChanged(Data.NewValue);
		});
	}
}

void AEnemyCharacter::OnHealthChanged(float NewHealth)
{
	Super::OnHealthChanged(NewHealth);

}

void AEnemyCharacter::HandleDeath()
{
	Super::HandleDeath();

	//
	AMLGameState* MLGameState = GetWorld()->GetGameState<AMLGameState>();
	if (!MLGameState)
	{
		return;
	}
	Cast<APlayerCharacter>(MLGameState->GetPlayer()->GetPawn())->DynamicCameraComponent->RemoveTarget(this);
	//

	OnDeathDelegate.Unbind();
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).Remove(OnChangeHealthDelegateHandle);
	AAIController* MyController = Cast<AAIController>(GetController());
	if (MyController)
	{
		UBrainComponent* BrainComponent = MyController->GetBrainComponent();
		if (BrainComponent)
		{
			BrainComponent->Cleanup();
		}
	}
	SetLifeSpan(5.f);
}

TArray<AActor*> AEnemyCharacter::GetCharactersToDamage(const FName& SocketName)
{
	if (!GetMesh() || !GEngine)
	{
		return TArray<AActor*>();
	}

	TArray<AActor*> ActorsToDamage;

	const FVector& HitPointOriginSphere = GetMesh()->GetSocketLocation(SocketName);
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	TArray<FOverlapResult> OverlapResults;
	if (UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(OverlapResults, HitPointOriginSphere, FQuat::Identity,
			FCollisionObjectQueryParams(), FCollisionShape::MakeSphere(HitSphereRadius), CollisionQueryParams);
		for (const FOverlapResult& OverlapResult : OverlapResults)
		{
			if (ABaseCharacter* Character = Cast<ABaseCharacter>(OverlapResult.GetActor()))
			{
				if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
				{
					ActorsToDamage.AddUnique(OverlapResult.GetActor());
				}
			}
		}
	}
	return ActorsToDamage;
}

FVector AEnemyCharacter::GetTargetEnemyVector() const
{
	const auto AIController = Cast<AEnemyAIController>(GetController());
	if (AIController)
	{
		return AIController->GetTargetEnemyVector();
	}

	return FVector::ZeroVector;
}


const float AEnemyCharacter::GetDamageMultiplierByTag(const FGameplayTag& Tag) const
{
	const float* Damage = DamageMultiplierByTag.Find(Tag);
	return Damage ? *Damage : 0.f; 
}

class ARoomTrigger* AEnemyCharacter::GetParentRoom()
{
	return ParentRoom;
}

void AEnemyCharacter::SetParentRoom(class ARoomTrigger* Room)
{
	ParentRoom = Room;
}

const FTaggedMontage& AEnemyCharacter::GetMontageToPlay() const
{
	int32 RandomIndex = FMath::RandRange(0, AnimMontages.Num() - 1);
	return AnimMontages[RandomIndex];
}

FTaggedMontage AEnemyCharacter::GetMontageToPlayByTag(const FGameplayTag& MontageTag) const
{
	const FTaggedMontage* TaggedMontage = AnimMontages.FindByPredicate([&MontageTag](const FTaggedMontage& TaggedMontage) 
		{ 
			return TaggedMontage.MontageTag == MontageTag; 
		});
	return TaggedMontage ? *TaggedMontage : FTaggedMontage();
}
