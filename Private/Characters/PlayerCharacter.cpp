// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/SaveSystem/SaveSubsystem.h"
#include "GAS/CharacterAttributeSetBase.h"
#include "GameFramework/PlayerStates/MLPlayerState.h"
#include "MemoryLake.h"
#include "MemoryLake/MemoryLakePlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "Components/FadeControllerComponent.h"
#include "Net/UnrealNetwork.h"
#include <GAS/CharacterAbilitySystemComponent.h>
#include "Components/DecalComponent.h"
#include "Components/MeleeCombatComponent.h"
#include "UMG/MLHUD.h"
#include "Components/HealthWidgetComponent.h"
#include "Components/ElementsWidgetComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CursorWidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/DynamicCameraComponent.h"
#include "Actors/EnemyProjectile.h"
#include <GameFramework/GameStates/MLGameState.h>
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	AbsoluteLocation = CreateDefaultSubobject<USceneComponent>(TEXT("AbsoluteLocation"));
	AbsoluteLocation->SetupAttachment(GetRootComponent());
	AbsoluteLocation->SetAbsolute(false, true, true);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(AbsoluteLocation);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetupAttachment(GetMesh());
	

	BladeTopPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Blade Top Point"));
	BladeBottomPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Blade Bottom Point"));
	BladeBottomPoint->AttachToComponent(WeaponMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
	BladeTopPoint->AttachToComponent(WeaponMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));

	TeamId = FGenericTeamId(0);

	//FadeController = CreateDefaultSubobject<UFadeControllerComponent>(TEXT("FadeControllerComponent"));
	UpgradeComponent = CreateDefaultSubobject<UUpgradeComponent>("Upgrade Component");
	MeleeCombatComponent = CreateDefaultSubobject<UMeleeCombatComponent>("MeleeCombatComponent");

	ElementsWidget = CreateDefaultSubobject<UElementsWidgetComponent>("ChoosenElements");
	ElementsWidget->SetupAttachment(RootComponent);
	ElementsWidget->SetOnlyOwnerSee(true);

	DynamicCameraComponent = CreateDefaultSubobject<UDynamicCameraComponent>(TEXT("DynamicCameraComponent"));

	Tags.Add(FName("Player"));
}

void APlayerCharacter::PossessedBy(AController* NewController)
{	
	Super::PossessedBy(NewController);

	PlayerController = Cast<AMemoryLakePlayerController>(NewController);
	//Init ability actor info for the Server
	InitAbilityActorInfo();
	UpdateHUDHealth();
	UpdateHUDFocus();
	
	AMLGameState* MLGameState = GetWorld()->GetGameState<AMLGameState>();
	if (!MLGameState)
	{
		return;
	}
	MLGameState->AddPlayer(PlayerController);
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

void APlayerCharacter::OnRep_Elements(const TArray<EElements>& OldElements)
{
	UpdateChoosenElements();
}

void APlayerCharacter::InitUpgrades()
{
	UPlayerAttributeSet* PlayerAttributes = Cast<UPlayerAttributeSet>(AttributeSet);
	UCharacterAttributeSetBase* CharacterAttributes = Cast<UCharacterAttributeSetBase>(AttributeSet);

	if (!PlayerAttributes || !CharacterAttributes || !PlayerController)
	{
		return;
	}

	if (UpgradeComponent->QueryUpgrade(FText::FromString("Favorite Cloth")))
	{
		PlayerAttributes->SetMaxHealth(PlayerAttributes->GetMaxHealth() + 50);
	}
	if (UpgradeComponent->QueryUpgrade(FText::FromString("Presented Crystal")))
	{
		PlayerAttributes->SetMaxHealth(PlayerAttributes->GetMaxHealth() + 100);
	}
	if (UpgradeComponent->QueryUpgrade(FText::FromString("Desired Necklace")))
	{
		CharacterAttributes->SetWaterResistance(CharacterAttributes->GetWaterResistance() + 2.5);
	}
	if (UpgradeComponent->QueryUpgrade(FText::FromString("Scary Sounds Shoes")))
	{
		CharacterAttributes->SetWindResistance(CharacterAttributes->GetWindResistance() + 2.5);
	}
	if (UpgradeComponent->QueryUpgrade(FText::FromString("Curious Scar")))
	{
		CharacterAttributes->SetFireResistance(CharacterAttributes->GetFireResistance() + 2.5);
	}
	if (UpgradeComponent->QueryUpgrade(FText::FromString("Fast Created Belt")))
	{
		CharacterAttributes->SetEarthResistance(CharacterAttributes->GetEarthResistance() + 2.5);
	}
	if (UpgradeComponent->QueryUpgrade(FText::FromString("Prelate Blessing")))
	{
		CharacterAttributes->SetWaterResistance(CharacterAttributes->GetWaterResistance() + 1);
		CharacterAttributes->SetWindResistance(CharacterAttributes->GetWindResistance() + 1);
		CharacterAttributes->SetFireResistance(CharacterAttributes->GetFireResistance() + 1);
		CharacterAttributes->SetEarthResistance(CharacterAttributes->GetEarthResistance() + 1);
	}
	
	if (UpgradeComponent->QueryUpgrade(FText::FromString("Memorable Creation")))
	{
		ShowUpgrades(2);
	}
	else if (UpgradeComponent->QueryUpgrade(FText::FromString("Memorable Toy")))
	{
		ShowUpgrades(1);
	}
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, Elements);
	DOREPLIFETIME(APlayerCharacter, Focus);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocallyControlled())
	{
		GetMesh()->SetRenderCustomDepth(false);
		GetMesh()->SetCustomDepthStencilValue(FIRST_STENCIL_INDEX);
	}
	else
	{
		GetMesh()->SetRenderCustomDepth(true);
		GetMesh()->SetCustomDepthStencilValue(FIRST_STENCIL_INDEX + 1);
		if (ElementsWidget)
		{
			ElementsWidget->SetHiddenInGame(true);
		}
	}
	GetWeaponMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SwordSocketName);

	PlayerController = Cast<AMemoryLakePlayerController>(GetController());
	CameraBoom->SetWorldLocation(GetActorLocation());

	TArray<FText> SavedUpgrades = GetSaveSubsystem(GetWorld())->CurrentPlayerData.CollectedPermanentUpgrades;
	for (FText upgrade : SavedUpgrades)
	{
		UpgradeComponent->CollectedUpgrades.Add(upgrade);
	}
	SwapWeapon(EStyle::MAGIC);
	InitUpgrades();
}

void APlayerCharacter::CreatePostProcessDMI()
{
	FWeightedBlendables& WB = TopDownCameraComponent->PostProcessSettings.WeightedBlendables;
	
	UObject* ParentMaterial = WB.Array[0].Object;
	UMaterialInterface* Material = Cast<UMaterialInterface>(ParentMaterial);
	
	PostProcessDMI = UMaterialInstanceDynamic::Create(Material, this);

	WB.Array[0].Object = PostProcessDMI;
}

void APlayerCharacter::LookAtCursor()
{
	AMemoryLakePlayerController* MLController = GetController<AMemoryLakePlayerController>();
	if (MLController)
	{
		MLController->LookAtCursor();
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	HandleAiming(DeltaTime);

	if (IsLocallyControlled())
	{
		SwitchOutlineWhenNeeded();
	}
}

void APlayerCharacter::SwitchOutlineWhenNeeded()
{
	FHitResult HitResult;

	float Radius = GetCapsuleComponent()->GetScaledCapsuleRadius();
	float HalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FCollisionShape CollisionShape;
	CollisionShape.SetCapsule(FVector3f(Radius, 0.f, HalfHeight));

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepSingleByChannel
	(
		HitResult,
		TopDownCameraComponent->GetComponentLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		CollisionShape,
		QueryParams
	);

	GetMesh()->SetRenderCustomDepth(bHit);
}

void APlayerCharacter::AddElement_Implementation(EElements Element)
{
	if (Elements.Num() < 3)
	{
		if ((Element == EElements::FIRE && Elements.Find(EElements::WATER) == INDEX_NONE) ||
			(Element == EElements::WATER && Elements.Find(EElements::FIRE) == INDEX_NONE) ||
			(Element == EElements::AIR) || 
			(Element == EElements::EARTH && Elements.Find(EElements::LIGHTNING) == INDEX_NONE))
		{
			Elements.Add(Element);
		}
		else if (Elements.Num() == 2)
		{
			if (Elements[0] == EElements::AIR && Elements[1] == EElements::WATER && Element == EElements::FIRE)
			{
				Elements.Empty();
				Elements.Add(EElements::LIGHTNING);
			}
		}
		if ((Element == EElements::FIRE && Elements.Find(EElements::WATER) != INDEX_NONE))
		{
			Elements.Remove(EElements::WATER);
		}
		else if ((Element == EElements::WATER && Elements.Find(EElements::FIRE) != INDEX_NONE))
		{
			Elements.Remove(EElements::FIRE);
		}
	}
	Cast<UCharacterAbilitySystemComponent>(GetAbilitySystemComponent())->AddElement_Implementation(Element);
	UpdateChoosenElements();
}

void APlayerCharacter::UpdateChoosenElements()
{
	if (ElementsWidget && IsLocallyControlled())
	{
		ElementsWidget->UpdateDisplayedElements(Elements);
	}
}

void APlayerCharacter::PistolShoot()
{
	if (CurrentAmountBullets >= 0)
	{
		FTransform SocketTransform = GetMesh()->GetSocketTransform("muzzle");
		FActorSpawnParameters SpawnInfo;
		//GetWorld()->SpawnActor<AEnemyProjectile>(Bullet, SocketTransform, SpawnInfo);
		AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(PlayerAbilityID::PistolAttack));
		CurrentAmountBullets -= 1;
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ShootDelay);
		Reload();
	}
}

void APlayerCharacter::PistolHeavyShoot()
{
	if (CurrentAmountBullets >= 0)
	{
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle_ShootDelay, // handle to cancel timer at a later time
			this, // the owning object
			&APlayerCharacter::PistolShoot, // function to call on elapsed
			0.15,// float delay until elapsed
			true); // looping?
	}
}

void APlayerCharacter::Reload()
{
	if (CurrentAmountBullets < MaxAmountBullets)
	{
		CurrentAmountBullets = MaxAmountBullets;
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(FGameplayTag::RequestGameplayTag("Ability.GunEnchant"));
		AbilitySystemComponent->CancelAbilities(&TagContainer);
		MeleeCombatComponent->GunEnchantAbility = 0;
	}
}

void APlayerCharacter::ClearElement_Implementation()
{
	Elements.Empty();
	Cast<UCharacterAbilitySystemComponent>(GetAbilitySystemComponent())->ClearElement_Implementation();
	UpdateChoosenElements();
}

void APlayerCharacter::ClearChargedElement()
{
	ChargedElements.Empty();
}

void APlayerCharacter::InitializeAttributes()
{
	Super::InitializeAttributes();
}

void APlayerCharacter::BindToAttributeChangeDelegates()
{
	AMLPlayerState* PS = GetPlayerState<AMLPlayerState>();
	if (!PS)
	{
		return;
	}

	OnHealthChangedDelegateHandle = PS->OnHealthChanged.AddUObject(this, &APlayerCharacter::OnHealthChanged);
}

void APlayerCharacter::OnHealthChanged(float NewHealth)
{
	Super::OnHealthChanged(NewHealth);
	UpdateHUDHealth();
}

void APlayerCharacter::UpdateHUDHealth()
{
	if (!AbilitySystemComponent)
	{
		return;
	}
	UCharacterAttributeSetBase const* CharacterAttrubuteSet = Cast<UCharacterAttributeSetBase>(AttributeSet);
	AMemoryLakePlayerController* MLController = GetController<AMemoryLakePlayerController>();
	if (MLController && CharacterAttrubuteSet)
	{
		MLController->Client_UpdateHUDHealth(CharacterAttrubuteSet->GetHealth() / CharacterAttrubuteSet->GetMaxHealth());
	}
}

void APlayerCharacter::UpdateHUDFocus()
{
	if (!PlayerController)
	{
		return;
	}

	PlayerController->UpdateFocus(Focus);
}

void APlayerCharacter::SetupHealthWidget()
{
	Super::SetupHealthWidget();
	if (!IsLocallyControlled())
	{
		return;
	}
	HealthWidget->Deactivate();
	HealthWidget->SetVisibility(false);
}

void APlayerCharacter::BindCharacterInputs()
{
}

void APlayerCharacter::HandleAiming(float DeltaTime)
{

	FVector ActorLocation = GetActorLocation();
	ClampedCameraHeight = GetClampedCameraHeight();
	float MouseX, MouseY;
	Cast<AMemoryLakePlayerController>(GetController())->GetMousePosition(MouseX, MouseY);
	FVector CursorLocation = FVector(MouseX, MouseY, 0);
	FVector CameraBoomLocation = CameraBoom->GetComponentLocation();

	if (bIsAiming && bCanMoveCursor)
	{
		DistanceToCursor = (CursorLocation - ActorLocation).Size2D();

		float MinDistance = ClampedCameraHeight * MinClampDistance;
		if (DistanceToCursor > MinDistance)
		{
			FVector NewLocation = FMath::VInterpConstantTo(CameraBoomLocation, CursorLocation, DeltaTime, SpeedOfMovingToCursor);

			const float MaxDistance = MaxClampDistance * ClampedCameraHeight;
			if ((NewLocation - ActorLocation).Size() * ClampedCameraHeight > MaxDistance)
			{
				FVector NewLocationNormalized = (NewLocation - ActorLocation).GetSafeNormal();
				const float Coefficient = MaxClampDistance;
				NewLocation = (NewLocationNormalized * Coefficient) + ActorLocation;
			}
			CameraBoom->SetWorldLocation(NewLocation);
		}
	}
}

float APlayerCharacter::GetClampedCameraHeight()
{
	TRange<float> A(MinCameraHeight, MaxCameraHeight);
	TRange<float> B(.5f, 1.f);
	float ArmLength = CameraBoom->TargetArmLength;
	float ResultingHeight = FMath::GetMappedRangeValueClamped(A, B, ArmLength);

	return ResultingHeight;
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
	}
}

FGenericTeamId APlayerCharacter::GetGenericTeamId() const
{
	return TeamId;
}

void APlayerCharacter::InitAbilityActorInfo()
{
	FString NetPrefix = GetWorld()->IsNetMode(NM_Client) ? "[CLIENT]" : "[SERVER]";

	AMLPlayerState* PS = GetPlayerState<AMLPlayerState>();
	if (!ensureMsgf(PS, TEXT("[%s] PlayerState is nullptr! Please assign PlayerState in your Character"), *NetPrefix)) return;

	AbilitySystemComponent = PS->GetAbilitySystemComponent();
	if (AMemoryLakePlayerController* PC = Cast<AMemoryLakePlayerController>(GetController()))
	{
		PC->AbilitySystemComponent = AbilitySystemComponent;
	}
	AttributeSet = PS->GetAttributeSet();

	if (!ensureMsgf(AbilitySystemComponent, TEXT("[%s] AbilitySystemComponent is nullptr! Please check AbilitySystemComponent in your PlayerState"), *NetPrefix)) return;
	if (!ensureMsgf(AttributeSet, TEXT("[%s] AttributeSet is nullptr! Please check AttributeSet in your PlayerState"), *NetPrefix)) return;

	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

	InitializeAttributes();
	AddCharacterAbilities();
	BindToAttributeChangeDelegates();

}

bool APlayerCharacter::IsInScreen(const FVector& Location)
{
	FVector2D ScreenLocation;
	bool bSuccess = PlayerController->ProjectWorldLocationToScreen(Location, ScreenLocation);
	int32 X = ScreenLocation.X;
	int32 Y = ScreenLocation.Y;

	int32 ViewportSizeX, ViewportSizeY;
	if (PlayerController)
	{
		PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);
	}
	else
	{
		ViewportSizeX = 1920;
		ViewportSizeY = 1080;
	}

	return X >= 0 && X <= ViewportSizeX && Y >= 0 && Y <= ViewportSizeY;
}

void APlayerCharacter::SetHealth(float Health)
{
	if (AttributeSet)
	{
		Cast<UCharacterAttributeSetBase>(AttributeSet)->SetHealth(Health);
	}
}

void APlayerCharacter::RestoreHealth()
{
	AttributeSet->SetHealth(AttributeSet->GetMaxHealth());
}

float APlayerCharacter::GetMaxHealth() const
{
	if (AttributeSet)
	{
		return Cast<UCharacterAttributeSetBase>(AttributeSet)->GetMaxHealth();
	}

	return 0.0f;
}

float APlayerCharacter::GetCapsuleHeight()
{
	if (!GetCapsuleComponent())
	{
		return 0.0f;
	}

	return GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

void APlayerCharacter::AddCharacterAbilities()
{
	if (!HasAuthority() || !AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(ConeAbility, 1.f, static_cast<int32>(PlayerAbilityID::Cone), this));
	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(BeamAbility, 1.f, static_cast<int32>(PlayerAbilityID::Beam), this));
	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(ProjectileAbility, 1.f, static_cast<int32>(PlayerAbilityID::Projectile), this));
	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DashAbility, 1.f, static_cast<int32>(PlayerAbilityID::Dash), this));
	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(ArmorAbility, 1.f, static_cast<int32>(PlayerAbilityID::Armor), this));
	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AOEAbility, 1.f, static_cast<int32>(PlayerAbilityID::HeavyAttack), this));
	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(BlockAbility, 1.f, static_cast<int32>(PlayerAbilityID::Block), this));
	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(EnchantAbility, 1.f, static_cast<int32>(PlayerAbilityID::Enchant), this));
	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(GunEnchantAbility, 1.f, static_cast<int32>(PlayerAbilityID::GunEnchant), this));
	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(BladeLightAttack1, 1.f, static_cast<int32>(PlayerAbilityID::BladeLightAttack1), this));
	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(BladeLightAttack2, 1.f, static_cast<int32>(PlayerAbilityID::BladeLightAttack2), this));
	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(BladeHeavyAttack, 1.f, static_cast<int32>(PlayerAbilityID::BladeLightAttack1), this));
	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(PistolAttack, 1.f, static_cast<int32>(PlayerAbilityID::PistolAttack), this));
}

void APlayerCharacter::SwapWeapon(EStyle NewStyle)
{
	switch (NewStyle) 
	{
	case EStyle::GUN:
		WeaponMesh->SetStaticMesh(RevolverMesh);
		GetWeaponMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, GunSocketName);
		break;
	case EStyle::BLADE:
		WeaponMesh->SetStaticMesh(SwordMesh);
		GetWeaponMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SwordSocketName);
		break;
	case EStyle::MAGIC:
		WeaponMesh->SetStaticMesh(0);
		break;
	}
}

void APlayerCharacter::UpgradeAbilities() 
{
	UPlayerAttributeSet* PlayerAttributes = Cast<UPlayerAttributeSet>(AttributeSet);
	PlayerAttributes->SetMaxHealth(PlayerAttributes->GetMaxHealth() + UpgradeComponent->CurrentUpgradeStruct.Health);
	PlayerAttributes->SetFireDamage(PlayerAttributes->GetFireDamage() * UpgradeComponent->CurrentUpgradeStruct.FireDamage);
	PlayerAttributes->SetFirePeriodDamage(PlayerAttributes->GetFirePeriodDamage() * UpgradeComponent->CurrentUpgradeStruct.FirePeriodDamage);
	PlayerAttributes->SetWaterDamage(PlayerAttributes->GetWaterDamage() * UpgradeComponent->CurrentUpgradeStruct.WaterDamage);
	PlayerAttributes->SetWaterSlowPower(PlayerAttributes->GetWaterSlowPower() * UpgradeComponent->CurrentUpgradeStruct.WaterSlowPower);

	if (UpgradeComponent->CurrentUpgradeStruct.WaterSlowDuration > 1.0) 
	{
		PlayerAttributes->SetWaterSlowDuration(PlayerAttributes->GetWaterSlowDuration() + 1);
	}
	
	if (UpgradeComponent->CurrentUpgradeStruct.FirePeriodDuration > 1.0)
	{
		PlayerAttributes->SetFirePeriodDuration(PlayerAttributes->GetFirePeriodDuration() + 1);
	}

	if (UpgradeComponent->CurrentUpgradeStruct.EarthStunDuration > 1.0)
	{
		PlayerAttributes->SetEarthStunDuration(PlayerAttributes->GetEarthStunDuration() + 1);
	}

	PlayerAttributes->SetEarthDamage(PlayerAttributes->GetEarthDamage() * UpgradeComponent->CurrentUpgradeStruct.EarthDamage);
	PlayerAttributes->SetEarthStunDuration(PlayerAttributes->GetEarthStunDuration() * UpgradeComponent->CurrentUpgradeStruct.EarthStunDuration);
	PlayerAttributes->SetWindDamage(PlayerAttributes->GetWindDamage() * UpgradeComponent->CurrentUpgradeStruct.WindDamage);
	PlayerAttributes->SetWindKnockBack(PlayerAttributes->GetWindKnockBack() * UpgradeComponent->CurrentUpgradeStruct.WindKnockBack);
	Cast<UCharacterAttributeSetBase>(AttributeSet)->SetMovementSpeed(Cast<UCharacterAttributeSetBase>(AttributeSet)->GetMovementSpeed() * UpgradeComponent->CurrentUpgradeStruct.MovementSpeed);
};

void APlayerCharacter::Interact()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		{
			IInteractInterface::Execute_Interact(Actor, this);
			return;
		}
	}
}

void APlayerCharacter::AddFocus(int32 Value)
{
	Focus += Value;

	UpdateHUDFocus();
}

void APlayerCharacter::AddFoundMemory_Implementation(int32 Memory)
{
	if (IsLocallyControlled())
	{
		USaveSubsystem* SaveSubsystem = GetSaveSubsystem(GetWorld());
		SaveSubsystem->CurrentPlayerData.FoundMemory += Memory;
		SaveSubsystem->SaveGame(GetSaveSubsystem(GetWorld())->CurrentPlayerData);
	}
}

void APlayerCharacter::ShowWidgets()
{
	if (!ElementsWidget)
	{
		return;
	}

	ElementsWidget->SetHiddenInGame(false);
}

void APlayerCharacter::HideWidgets()
{
	if (!ElementsWidget)
	{
		return;
	}

	ElementsWidget->SetHiddenInGame(true);
}

void APlayerCharacter::AddChargedElements()
{
	ChargedElements = Elements;
	ClearElement();
}
