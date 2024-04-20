// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayersLobbyWidget.generated.h"

UCLASS()
class MEMORYLAKE_API UPlayersLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Lobby")
	const FName& GetLobbyName() const { return LobbyName; }
	
	UFUNCTION(BlueprintPure, Category = "Lobby")
	const float& GetSessionToStart() const { return SessionToStart; }

	UFUNCTION(BlueprintPure, Category = "Lobby")
	const int32& GetMaxPlayers() const { return MaxPlayers; }

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void SetLobbyName(const FName& NewLobbyName) { LobbyName = NewLobbyName; }

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void SetMaxPlayers(int32 NewMaxPlayers) { MaxPlayers = NewMaxPlayers; }

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void SetSessionToStart(float Delay) {SessionToStart = Delay;}

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void ReturnToMainMenu();
	
protected:
	UPROPERTY(meta = (BindWidget))
	FName LobbyName;

	UPROPERTY(meta = (BindWidget))
	float SessionToStart;

	UPROPERTY(meta = (BindWidget))
	int32 MaxPlayers;

	UPROPERTY(BlueprintReadWrite, Category = "Lobby")
	FName MainMenuMapName;
};
