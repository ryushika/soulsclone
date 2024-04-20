// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#define FIRST_STENCIL_INDEX 252
#define ENEMY_STENCIL_INDEX 254

DECLARE_LOG_CATEGORY_EXTERN(AI, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogMemoryLake, Log, All);
DECLARE_DELEGATE_OneParam(FOnDeathDelegate, APawn*);

UENUM(BlueprintType)
enum class PlayerAbilityID : uint8
{
	None UMETA(DisplayName = "None"),
	Confirm UMETA(DisplayName = "Confirm"),
	Cancel UMETA(DisplayName = "Cancel"),
	Cone UMETA(DisplayName = "Cone"),
	Projectile UMETA(DisplayName = "Projectile"),
	Beam UMETA(DisplayName = "Beam"),
	Dash UMETA(DisplayName = "Dash"),
	Armor UMETA(DisplayName = "Armor"),
	HeavyAttack UMETA(DisplayName = "HeavyAttack"),
	Block UMETA(DisplayName = "Block"),
	Enchant UMETA(DisplayName = "Enchant"),
	GunEnchant UMETA(DisplayName = "Gun Enchant"),
	BladeLightAttack1 UMETA(DisplayName = "Light Attack 1"),
	BladeLightAttack2 UMETA(DisplayName = "Light Attack 2"),
	BladeHeavyAttack UMETA(DisplayName = "Heavy Attack"),
	PistolAttack UMETA(DisplayName = "Pistol attack"),
}; 

static void LogOnScreen(UObject* WorldContext, FString Msg, FColor Color = FColor::White, float Duration = 5.0f)
{
	if (!ensure(WorldContext))
	{
		return;
	}

	UWorld* World = WorldContext->GetWorld();
	if (!ensure(World))
	{
		return;
	}

	FString NetPrefix = World->IsNetMode(NM_Client) ? "[CLIENT]" : "[SERVER]";
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix + Msg);
	}
}

UENUM(BlueprintType)
enum class EElements : uint8
{
	WATER,
	AIR,
	FIRE,
	EARTH,
	LIGHTNING
};

UENUM(BlueprintType)
enum class EGameOverState : uint8
{
	NONE,
	WON,
	LOST
};

UENUM(BlueprintType)
enum class EStyle : uint8
{
	NONE,
	MAGIC,
	BLADE,
	GUN
};