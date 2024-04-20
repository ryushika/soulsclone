// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/SaveSystem/SaveSubsystem.h"

bool USaveSubsystem::ServerCheck(bool LogWarning) const
{
	auto GI = GetGameInstance();
	if (!GI)
		return true;

	auto World = GI->GetWorld();
	if (!World)
		return true;

	return World->GetAuthGameMode() != nullptr;
}

void USaveSubsystem::AutoSaveGame(FText Title)
{
	SaveGame(CurrentPlayerData);
}

void USaveSubsystem::SaveGame(FPlayerSaveData SaveStruct)
{
	CurrentSaveGame = Cast<UMemoryLakeSaveGame>(UGameplayStatics::CreateSaveGameObject(UMemoryLakeSaveGame::StaticClass()));
	CurrentSaveGame->PlayerSaveData = SaveStruct;
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, "Save", 0);
}	

void USaveSubsystem::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist("Save", 0)) {
		UMemoryLakeSaveGame* LoadedGame = Cast<UMemoryLakeSaveGame>(UGameplayStatics::LoadGameFromSlot("Save", 0));

		if (LoadedGame == nullptr)
		{
			return;
		}
		else
		{
			CurrentPlayerData = LoadedGame->PlayerSaveData;
		}
	}
	else {
		CurrentSaveGame = Cast<UMemoryLakeSaveGame>(UGameplayStatics::CreateSaveGameObject(UMemoryLakeSaveGame::StaticClass()));
	}
}

