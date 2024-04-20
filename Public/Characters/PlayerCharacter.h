// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Characters/BaseCharacter.h"
#include "EnhancedInputLibrary.h"
#include "InputActionValue.h"
#include "Components/UpgradeComponent.h"
#include "GameFramework/InteractInterface.h"
#include "../GAS/PlayerAttributeSet.h"
#include "../MemoryLake.h"

#include "PlayerCharacter.generated.h"

class UElementsWidgetComponent;
class UMeleeCombatComponent;
class UArrowComponent;

UCLASS()
class MEMORYLAKE_API APlayerCharacter
	: public ABaseCharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

	void SwitchOutlineWhenNeeded();

	void HandleDecalMovement();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ShowUpgrades(int32 NumUpgrades);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PlayNiagaraEffect();

	UFUNCTION(BlueprintCallable)
	void UpgradeAbilities();

	UFUNCTION(BlueprintCallable)
	void AddFocus(int32 Value);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void AddFoundMemory(int32 Value);

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	virtual void SetHealth(float Health);
	virtual void RestoreHealth();
	float GetMaxHealth() const;

	float GetCapsuleHeight();

	virtual void AddCharacterAbilities() override;

	void SwapWeapon(EStyle NewStyle);

	FORCEINLINE UUpgradeComponent* GetUpgradeComponent() const { return UpgradeComponent; }

	FORCEINLINE UMeleeCombatComponent* GetMeleeCombatComponent() const { return MeleeCombatComponent; }

	FORCEINLINE class UStaticMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TSubclassOf<class UGameplayAbility> ConeAbility;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TSubclassOf<class UGameplayAbility> BeamAbility;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TSubclassOf<class UGameplayAbility> ProjectileAbility;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TSubclassOf<class UGameplayAbility> DashAbility;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TSubclassOf<class UGameplayAbility> ArmorAbility;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TSubclassOf<class UGameplayAbility> AOEAbility;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TSubclassOf<class UGameplayAbility> BlockAbility;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TSubclassOf<class UGameplayAbility> EnchantAbility;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TSubclassOf<class UGameplayAbility> GunEnchantAbility;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Blade Attacks")
	TSubclassOf<class UGameplayAbility> BladeLightAttack1;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Blade Attacks")
	TSubclassOf<class UGameplayAbility> BladeLightAttack2;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Blade Attacks")
	TSubclassOf<class UGameplayAbility> BladeHeavyAttack;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Blade Attacks")
	TSubclassOf<class UGameplayAbility> PistolAttack;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDynamicCameraComponent* DynamicCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* BladeTopPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* BladeBottomPoint;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Actor Components")
	class UUpgradeComponent* UpgradeComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Actor Components")
	class UMeleeCombatComponent* MeleeCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Magic, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = OnRep_Elements)
	TArray<EElements> Elements;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Magic, meta = (AllowPrivateAccess = "true"))
	TArray<EElements> ChargedElements;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magic, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> ElementalOrbs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 Focus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool bBlocking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentAmountBullets = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmountBullets = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SwordSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName GunSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 UpgradesLeftToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> SwordMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> RevolverMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractAction;

	UFUNCTION()
	virtual void OnRep_Elements(const TArray<EElements>& OldElements);

	UFUNCTION(BlueprintCallable, category = "Magic", Server, Reliable)
	void AddElement(EElements Element);

	UFUNCTION(BlueprintCallable, category = "Magic", Server, Reliable)
	void ClearElement();

	UFUNCTION(BlueprintCallable, category = "Magic")
	void ClearChargedElement();

	UFUNCTION(BlueprintCallable, category = "Magic")
	void InitUpgrades();

	UFUNCTION(BlueprintCallable, category = "Interact")
	void Interact();

	FORCEINLINE void ResumeCursorWidget() { bCanMoveCursor = true; };
	FORCEINLINE void StopCursorWidget() { bCanMoveCursor = false; };

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void ShowWidgets();
	void HideWidgets();
	
	void AddChargedElements();

protected:
	virtual void BeginPlay() override;

	//DMI - Dynamic Material Instance
	void CreatePostProcessDMI();

	void LookAtCursor();

	virtual void SetupHealthWidget() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Initialize the Character's attributes. Must run on Server but we run it on Client too
	// so that we don't have to wait. The Server's replication to the Client won't matter since
	// the values should be the same.
	virtual void InitializeAttributes() override;

	virtual void BindToAttributeChangeDelegates() override;
	virtual void OnHealthChanged(float NewHealth) override;
	
	virtual void BindCharacterInputs();

	void HandleAiming(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	bool bShouldLookAtCursor = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float MinCameraHeight = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float MaxCameraHeight = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming")
	bool bIsAiming = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aiming")
	float ClampedCameraHeight = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aiming")
	float DistanceToCursor = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming")
	float SpeedOfMovingToCursor = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming")
	float ReturnCameraSpeed = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming")
	float MinClampDistance = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming")
	float MaxClampDistance = 300.f;

	// The only way I could get some components to be 
	// absolute and follow the character at the same time
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class USceneComponent> AbsoluteLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UFadeControllerComponent> FadeController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UElementsWidgetComponent> ElementsWidget;


private:
	virtual FGenericTeamId GetGenericTeamId() const override;

	virtual void InitAbilityActorInfo() override;

	float GetClampedCameraHeight();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* WeaponMesh;

	bool IsInScreen(const FVector& Location);

	UPROPERTY()
	bool bCanMoveCursor = true;

	void UpdateHUDHealth();
	void UpdateHUDFocus();
	void UpdateChoosenElements();

	UPROPERTY()
	TObjectPtr<class AMemoryLakePlayerController> PlayerController;

	UPROPERTY()
	FGenericTeamId TeamId;

	UPROPERTY()
	TArray<TSubclassOf<class UGameplayAbility>> CharacterAbilities;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> PostProcessDMI;

	bool bIsCursorInScreen = false;
	FDelegateHandle OnHealthChangedDelegateHandle;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AEnemyProjectile> Bullet;
	void PistolShoot();
	FTimerHandle TimerHandle_ShootDelay;
	void PistolHeavyShoot();
	void Reload();
};
