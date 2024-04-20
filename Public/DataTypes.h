// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/SaveGame.h"

#include "DataTypes.generated.h"

USTRUCT(BlueprintType)
struct FUpgradeStruct : public FTableRowBase
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Upgrade Name"))
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Upgrade Description"))
	FText Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Upgrade Icon"))
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Upgrade Level"))
	int32 Level;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Upgrade Requisites"))
	FText Prerequisite;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Cost"))
	int32 Cost;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Health = 1.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MovementSpeed = 1.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FireDamage = 1.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FirePeriodDamage = 1.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FirePeriodDuration = 1.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WaterDamage = 1.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WaterSlowPower = 1.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WaterSlowDuration = 1.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float EarthDamage = 1.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float EarthStunDuration = 1.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WindDamage = 1.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WindKnockBack = 1.0;

};

USTRUCT(BlueprintType, Blueprintable)
struct FPlayerSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int FoundMemory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int FoundMemorySpent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TArray<FText> CollectedPermanentUpgrades;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	float CurrentSoundSettings;

	FPlayerSaveData()
	{
		FoundMemory = 0;
		FoundMemorySpent = 0;
		CollectedPermanentUpgrades = TArray<FText>();
		CurrentSoundSettings = 0.5;
	}
};


/**
 *
 */
UCLASS()
class MEMORYLAKE_API UMemoryLakeSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FPlayerSaveData PlayerSaveData;
};