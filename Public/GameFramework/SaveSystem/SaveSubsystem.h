// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataTypes.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSubsystem.generated.h"

/**
 * 
 */
UCLASS(Config = Engine)
class MEMORYLAKE_API USaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerSaveData CurrentPlayerData;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	UMemoryLakeSaveGame *CurrentSaveGame;

protected:
	bool ServerCheck(bool LogWarning) const;

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AutoSaveGame(FText Title = FText());

	UFUNCTION(BlueprintCallable)
	void SaveGame(FPlayerSaveData SaveStruct);

	UFUNCTION(BlueprintCallable)
	void LoadGame();

};

inline USaveSubsystem* GetSaveSubsystem(UWorld* WorldContext)
{
	if (IsValid(WorldContext) && WorldContext->IsGameWorld())
	{
		UGameInstance* GI = WorldContext->GetGameInstance();
		if (IsValid(GI))
			return GI->GetSubsystem<USaveSubsystem>();
	}

	return nullptr;
}